// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_UI_WEBUI_BRAVE_NEW_TAB_NEW_TAB_PAGE_HANDLER_H_
#define BRAVE_BROWSER_UI_WEBUI_BRAVE_NEW_TAB_NEW_TAB_PAGE_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "brave/browser/ui/webui/brave_new_tab/update_observer.h"
#include "brave/components/brave_new_tab/common/new_tab_page.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class CustomBackgroundFileManager;
class PrefService;

namespace ntp_background_images {
class ViewCounterService;
}

namespace brave_new_tab {

class CustomImageChooser;

class NewTabPageHandler : public mojom::NewTabPageHandler {
 public:
  NewTabPageHandler(
      mojo::PendingReceiver<mojom::NewTabPageHandler> receiver,
      std::unique_ptr<CustomImageChooser> custom_image_chooser,
      std::unique_ptr<CustomBackgroundFileManager> custom_file_manager,
      PrefService* pref_service,
      ntp_background_images::ViewCounterService* view_counter_service,
      bool is_restored_page);

  ~NewTabPageHandler() override;

  // mojom::NewTabPageHandler:
  void SetNewTabPage(mojo::PendingRemote<mojom::NewTabPage> page) override;
  void GetBackgroundsEnabled(GetBackgroundsEnabledCallback callback) override;
  void SetBackgroundsEnabled(bool enabled,
                             SetBackgroundsEnabledCallback callback) override;
  void GetSponsoredImagesEnabled(
      GetSponsoredImagesEnabledCallback callback) override;
  void SetSponsoredImagesEnabled(
      bool enabled,
      SetSponsoredImagesEnabledCallback callback) override;
  void GetBraveBackgrounds(GetBraveBackgroundsCallback callback) override;
  void GetCustomBackgrounds(GetCustomBackgroundsCallback callback) override;
  void GetSelectedBackground(GetSelectedBackgroundCallback callback) override;
  void GetSponsoredImageBackground(
      GetSponsoredImageBackgroundCallback callback) override;
  void SelectBackground(mojom::SelectedBackgroundPtr background,
                        SelectBackgroundCallback callback) override;
  void ShowCustomBackgroundChooser(
      ShowCustomBackgroundChooserCallback callback) override;
  void AddCustomBackgrounds(AddCustomBackgroundsCallback callback) override;
  void RemoveCustomBackground(const std::string& background_url,
                              RemoveCustomBackgroundCallback callback) override;

 private:
  void OnCustomBackgroundsSelected(ShowCustomBackgroundChooserCallback callback,
                                   std::vector<base::FilePath> paths);

  void OnCustomBackgroundsSaved(AddCustomBackgroundsCallback callback,
                                std::vector<base::FilePath> paths);

  void OnCustomBackgroundRemoved(RemoveCustomBackgroundCallback callback,
                                 base::FilePath path,
                                 bool success);

  void OnUpdate(UpdateObserver::Source update_source);

  mojo::Receiver<mojom::NewTabPageHandler> receiver_;
  mojo::Remote<mojom::NewTabPage> page_;
  UpdateObserver update_observer_;
  std::unique_ptr<CustomImageChooser> custom_image_chooser_;
  std::unique_ptr<CustomBackgroundFileManager> custom_file_manager_;
  raw_ptr<PrefService> pref_service_;
  raw_ptr<ntp_background_images::ViewCounterService> view_counter_service_;
  std::vector<base::FilePath> custom_image_paths_;
  bool page_restored_ = false;
};

}  // namespace brave_new_tab

#endif  // BRAVE_BROWSER_UI_WEBUI_BRAVE_NEW_TAB_NEW_TAB_PAGE_HANDLER_H_
