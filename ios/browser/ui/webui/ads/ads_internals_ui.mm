// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/ios/browser/ui/webui/ads/ads_internals_ui.h"

#include <cstddef>
#include <string>
#include <utility>

#include "base/containers/span.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/notreached.h"
#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/task/thread_pool.h"
#include "brave/components/brave_ads/browser/resources/grit/ads_internals_generated_map.h"
#include "brave/components/brave_ads/core/public/ads.h"
#include "brave/components/brave_ads/core/public/prefs/pref_names.h"
#include "brave/components/constants/webui_url_constants.h"
#include "brave/components/l10n/common/localization_util.h"
#include "brave/ios/browser/brave_ads/ads_service_factory_ios.h"
#include "brave/ios/browser/brave_ads/ads_service_impl_ios.h"
#include "components/grit/brave_components_resources.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/ui/util/pasteboard_util.h"
#include "ios/web/public/web_state.h"
#include "ios/web/public/webui/url_data_source_ios.h"
#include "ios/web/public/webui/web_ui_ios.h"
#include "ios/web/public/webui/web_ui_ios_data_source.h"
#include "ios/web/public/webui/web_ui_ios_message_handler.h"
#include "ui/base/webui/resource_path.h"
#include "ui/base/webui/web_ui_util.h"

namespace {

web::WebUIIOSDataSource* CreateAndAddWebUIDataSource(
    web::WebUIIOS* web_ui,
    const std::string& name,
    const webui::ResourcePath* resource_map,
    std::size_t resource_map_size,
    int html_resource_id) {
  web::WebUIIOSDataSource* source = web::WebUIIOSDataSource::Create(name);
  web::WebUIIOSDataSource::Add(ProfileIOS::FromWebUIIOS(web_ui), source);
  source->UseStringsJs();

  // Add required resources.
  source->AddResourcePaths(
      UNSAFE_TODO(base::make_span(resource_map, resource_map_size)));
  source->SetDefaultResource(html_resource_id);
  return source;
}

}  // namespace

AdsInternalsUI::AdsInternalsUI(web::WebUIIOS* web_ui, const GURL& url)
    : web::WebUIIOSController(web_ui, url.host()) {
  CreateAndAddWebUIDataSource(web_ui, url.host(), kAdsInternalsGenerated,
                              kAdsInternalsGeneratedSize,
                              IDR_ADS_INTERNALS_HTML);

  // Bind Mojom Interface
  web_ui->GetWebState()->GetInterfaceBinderForMainFrame()->AddInterface(
      base::BindRepeating(&AdsInternalsUI::BindInterface,
                          base::Unretained(this)));
}

AdsInternalsUI::~AdsInternalsUI() {}

void AdsInternalsUI::BindInterface(
    mojo::PendingReceiver<bat_ads::mojom::AdsInternals> pending_receiver) {
  if (receiver_.is_bound()) {
    receiver_.reset();
  }

  receiver_.Bind(std::move(pending_receiver));
}

///////////////////////////////////////////////////////////////////////////////

brave_ads::AdsServiceImplIOS* AdsInternalsUI::GetAdsService() {
  ProfileIOS* const profile = ProfileIOS::FromWebUIIOS(web_ui());
  CHECK(profile);
  return brave_ads::AdsServiceFactoryIOS::GetForBrowserState(profile);
}

void AdsInternalsUI::GetAdsInternals(GetAdsInternalsCallback callback) {
  brave_ads::AdsServiceImplIOS* ads_service = GetAdsService();
  if (!ads_service) {
    return std::move(callback).Run("");
  }

  ads_service->GetInternals(
      base::BindOnce(&AdsInternalsUI::GetInternalsCallback,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
}

void AdsInternalsUI::ClearAdsData(ClearAdsDataCallback callback) {
  brave_ads::AdsServiceImplIOS* ads_service = GetAdsService();
  if (!ads_service) {
    return std::move(callback).Run(/*success=*/false);
  }

  ads_service->ClearData(base::BindOnce(
      [](ClearAdsDataCallback callback) {
        std::move(callback).Run(/*success=*/true);
      },
      std::move(callback)));
}

void AdsInternalsUI::GetInternalsCallback(
    GetAdsInternalsCallback callback,
    std::optional<base::Value::List> value) {
  if (!value) {
    return std::move(callback).Run("");
  }

  std::string json;
  CHECK(base::JSONWriter::Write(*value, &json));
  std::move(callback).Run(json);
}
