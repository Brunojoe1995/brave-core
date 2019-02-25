/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/widevine/widevine_utils.h"

#include "brave/browser/brave_browser_process_impl.h"
#include "brave/browser/widevine/widevine_permission_request.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/permissions/permission_request_manager.h"
#include "chrome/browser/subresource_filter/chrome_subresource_filter_client.h"

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
#include <string>

#include "base/bind.h"
#include "brave/browser/brave_drm_tab_helper.h"
#include "brave/browser/widevine/brave_widevine_bundle_manager.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
#include "brave/common/pref_names.h"
#include "chrome/browser/component_updater/widevine_cdm_component_installer.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/prefs/pref_service.h"
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
namespace {
// Returns true when Widevine install is trigerred by content settings bubble
// while permission bubble is visible.
bool IsPermissionBubbleForInstallIsVisible(
    content::WebContents* web_contents) {
  return !!PermissionRequestManager::FromWebContents(web_contents)->
      GetBubbleWindow();
}

content::WebContents* GetActiveWebContents() {
  if (Browser* browser = chrome::FindLastActive())
    return browser->tab_strip_model()->GetActiveWebContents();
  return nullptr;
}

bool IsActiveTabRequestedWidevine() {
  bool is_active_tab_requested_widevine = false;
  if (content::WebContents* active_web_contents = GetActiveWebContents()) {
    BraveDrmTabHelper* drm_helper =
        BraveDrmTabHelper::FromWebContents(active_web_contents);
    is_active_tab_requested_widevine = drm_helper->ShouldShowWidevineOptIn();
  }

  return is_active_tab_requested_widevine;
}

void OnWidevineInstallDone(const std::string& error) {
  if (!error.empty()) {
    LOG(ERROR) << __func__ << ": " << error;
    return;
  }

  DVLOG(1) << __func__ << ": Widevine install success";
  if (IsActiveTabRequestedWidevine()) {
    chrome::FindLastActive()->window()->UpdateToolbar(nullptr);

    auto* web_contents = GetActiveWebContents();
    // There is no way to hide existing permission bubble w/o user gesture
    // except reloading. To handle this, reloading again.
    // This can happen when user installs Widevine via content settings bubble
    // while permission bubble is visible.
    // In this case, next request permission for request isn't added because
    // user already recognized well about content settings bubble.
    if (IsPermissionBubbleForInstallIsVisible(web_contents)) {
      ChromeSubresourceFilterClient::FromWebContents(web_contents)
          ->OnReloadRequested();
    } else {
      RequestWidevinePermission(web_contents);
    }
  }
}
}  // namespace
#endif

int GetWidevineTitleTextResourceId() {
  int message_id = -1;
#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
  message_id = IDS_NOT_INSTALLED_WIDEVINE_TITLE;
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  message_id = manager->GetWidevineContentSettingsBubbleTitleText();
#endif

  DCHECK(message_id != -1);
  return message_id;
}

int GetWidevineLinkTextForContentSettingsBubbleResourceId() {
  int message_id = -1;
#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
  message_id = IDS_INSTALL_AND_RUN_WIDEVINE;
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  message_id = manager->GetWidevineContentSettingsBubbleLinkText();
#endif

  DCHECK(message_id != -1);
  return message_id;
}

int GetWidevineBlockedImageMessageResourceId() {
  int message_id = -1;
#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
  message_id = IDS_WIDEVINE_NOT_INSTALLED_MESSAGE;
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  message_id = manager->GetWidevineBlockedImageMessage();
#endif

  DCHECK(message_id != -1);
  return message_id;
}

int GetWidevineBlockedImageTooltipResourceId() {
  int tooltip_id = -1;
#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
  tooltip_id = IDS_WIDEVINE_NOT_INSTALLED_EXPLANATORY_TEXT;
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  tooltip_id = manager->GetWidevineBlockedImageTooltip();
#endif

  DCHECK(tooltip_id != -1);
  return tooltip_id;
}

int GetWidevinePermissionRequestTextFrangmentResourceId() {
  int message_id = -1;

#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
  message_id = IDS_WIDEVINE_PERMISSION_REQUEST_TEXT_FRAGMENT;
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  message_id = manager->GetWidevineContentSettingsBubbleTitleText();
#endif

  DCHECK(message_id != -1);
  return message_id;
}

void RequestWidevinePermission(content::WebContents* web_contents) {
  PermissionRequestManager::FromWebContents(web_contents)->AddRequest(
      new WidevinePermissionRequest(web_contents));
}

#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT)
void EnableWidevineCdmComponent(content::WebContents* web_contents) {
  DCHECK(web_contents);

  PrefService* prefs = ProfileManager::GetActiveUserProfile()->GetPrefs();
  if (prefs->GetBoolean(kWidevineOptedIn))
    return;

  prefs->SetBoolean(kWidevineOptedIn, true);
  RegisterWidevineCdmComponent(g_brave_browser_process->component_updater());
  ChromeSubresourceFilterClient::FromWebContents(web_contents)
      ->OnReloadRequested();
}
#endif

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
void InstallBundleOrRestartBrowser() {
  auto* manager = g_brave_browser_process->brave_widevine_bundle_manager();
  if (manager->needs_restart()) {
    manager->WillRestart();
    if (!manager->is_test()) {
      // Prevent relaunch during the browser test.
      // This will cause abnormal termination.
      chrome::AttemptRelaunch();
    }
    return;
  }

  // User can request install again because |kWidevineOptedIn| is set when
  // install is finished. In this case, just waiting previous install request.
  if (!manager->in_progress()) {
    manager->InstallWidevineBundle(base::BindOnce(&OnWidevineInstallDone),
                                   true);
  }
}
#endif
