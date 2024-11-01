// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_UI_WEBUI_ADS_INTERNALS_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_ADS_INTERNALS_UI_H_

#include <optional>
#include <string>

#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "brave/components/brave_ads/browser/ads_service.h"
#include "brave/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/receiver.h"

class AdsInternalsUI : public content::WebUIController,
                       bat_ads::mojom::AdsInternals {
 public:
  AdsInternalsUI(content::WebUI* const web_ui, const std::string& name);

  AdsInternalsUI(const AdsInternalsUI&) = delete;
  AdsInternalsUI& operator=(const AdsInternalsUI&) = delete;

  AdsInternalsUI(AdsInternalsUI&&) noexcept = delete;
  AdsInternalsUI& operator=(AdsInternalsUI&&) noexcept = delete;

  ~AdsInternalsUI() override;

  void BindInterface(
      mojo::PendingReceiver<bat_ads::mojom::AdsInternals> pending_receiver);

 private:
  void GetInternalsCallback(GetAdsInternalsCallback callback,
                            std::optional<base::Value::List> value);

  // bat_ads::mojom::AdsInternals:
  void GetAdsInternals(GetAdsInternalsCallback callback) override;
  void ClearAdsData(ClearAdsDataCallback callback) override;

  raw_ptr<brave_ads::AdsService> ads_service_ = nullptr;  // Not owned.

  mojo::Receiver<bat_ads::mojom::AdsInternals> receiver_{this};

  base::WeakPtrFactory<AdsInternalsUI> weak_ptr_factory_{this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // BRAVE_BROWSER_UI_WEBUI_ADS_INTERNALS_UI_H_
