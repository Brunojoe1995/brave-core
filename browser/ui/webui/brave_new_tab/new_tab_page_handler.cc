// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/webui/brave_new_tab/new_tab_page_handler.h"

#include <utility>

#include "base/barrier_callback.h"
#include "base/containers/contains.h"
#include "brave/browser/brave_browser_process.h"
#include "brave/browser/ntp_background/custom_background_file_manager.h"
#include "brave/browser/ntp_background/ntp_background_prefs.h"
#include "brave/browser/ui/webui/brave_new_tab/custom_image_chooser.h"
#include "brave/components/ntp_background_images/browser/ntp_background_images_data.h"
#include "brave/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "brave/components/ntp_background_images/browser/url_constants.h"
#include "brave/components/ntp_background_images/browser/view_counter_service.h"
#include "brave/components/ntp_background_images/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace brave_new_tab {

namespace {

std::string GetCustomImageURL(const std::string& image_name) {
  return CustomBackgroundFileManager::Converter(image_name).To<GURL>().spec();
}

std::string CustomImageNameFromURL(const std::string& url) {
  return CustomBackgroundFileManager::Converter(GURL(url)).To<std::string>();
}

mojom::SponsoredImageBackgroundPtr ReadSponsoredImageData(
    const base::Value::Dict& data) {
  using ntp_background_images::kCampaignIdKey;
  using ntp_background_images::kCreativeInstanceIDKey;
  using ntp_background_images::kIsBackgroundKey;
  using ntp_background_images::kWallpaperIDKey;
  using ntp_background_images::kWallpaperImageURLKey;

  auto is_background = data.FindBool(kIsBackgroundKey);
  if (is_background.value_or(false)) {
    return nullptr;
  }

  auto background = mojom::SponsoredImageBackground::New();

  if (auto* creative_instance_id = data.FindString(kCreativeInstanceIDKey)) {
    background->creative_instance_id = *creative_instance_id;
  }

  if (auto* wallpaper_id = data.FindString(kWallpaperIDKey)) {
    background->wallpaper_id = *wallpaper_id;
  }

  if (auto* campaign_id = data.FindString(kCampaignIdKey)) {
    background->campaign_id = *campaign_id;
  }

  if (auto* image_url = data.FindString(kWallpaperImageURLKey)) {
    background->image_url = *image_url;
  }

  return background;
}

}  // namespace

NewTabPageHandler::NewTabPageHandler(
    mojo::PendingReceiver<mojom::NewTabPageHandler> receiver,
    std::unique_ptr<CustomImageChooser> custom_image_chooser,
    std::unique_ptr<CustomBackgroundFileManager> custom_file_manager,
    PrefService* pref_service,
    ntp_background_images::ViewCounterService* view_counter_service,
    bool is_restored_page)
    : receiver_(this, std::move(receiver)),
      update_observer_(pref_service,
                       base::BindRepeating(&NewTabPageHandler::OnUpdate,
                                           base::Unretained(this))),
      custom_image_chooser_(std::move(custom_image_chooser)),
      custom_file_manager_(std::move(custom_file_manager)),
      pref_service_(pref_service),
      view_counter_service_(view_counter_service),
      page_restored_(is_restored_page) {}

NewTabPageHandler::~NewTabPageHandler() = default;

void NewTabPageHandler::SetNewTabPage(
    mojo::PendingRemote<mojom::NewTabPage> page) {
  page_.reset();
  page_.Bind(std::move(page));
}

void NewTabPageHandler::GetBackgroundsEnabled(
    GetBackgroundsEnabledCallback callback) {
  bool backgrounds_enabled = pref_service_->GetBoolean(
      ntp_background_images::prefs::kNewTabPageShowBackgroundImage);
  std::move(callback).Run(backgrounds_enabled);
}

void NewTabPageHandler::SetBackgroundsEnabled(
    bool enabled,
    SetBackgroundsEnabledCallback callback) {
  pref_service_->SetBoolean(
      ntp_background_images::prefs::kNewTabPageShowBackgroundImage, enabled);
  std::move(callback).Run();
}

void NewTabPageHandler::GetSponsoredImagesEnabled(
    GetSponsoredImagesEnabledCallback callback) {
  bool sponsored_images_enabled = pref_service_->GetBoolean(
      ntp_background_images::prefs::
          kNewTabPageShowSponsoredImagesBackgroundImage);
  std::move(callback).Run(sponsored_images_enabled);
}

void NewTabPageHandler::SetSponsoredImagesEnabled(
    bool enabled,
    SetSponsoredImagesEnabledCallback callback) {
  pref_service_->SetBoolean(ntp_background_images::prefs::
                                kNewTabPageShowSponsoredImagesBackgroundImage,
                            enabled);
  std::move(callback).Run();
}

void NewTabPageHandler::GetBraveBackgrounds(
    GetBraveBackgroundsCallback callback) {
  auto* service = g_brave_browser_process->ntp_background_images_service();
  if (!service) {
    std::move(callback).Run({});
    return;
  }

  auto* image_data = service->GetBackgroundImagesData();
  if (!image_data || !image_data->IsValid()) {
    std::move(callback).Run({});
    return;
  }

  std::vector<mojom::BraveBackgroundPtr> backgrounds;
  backgrounds.reserve(image_data->backgrounds.size());

  for (auto& background : image_data->backgrounds) {
    auto value = mojom::BraveBackground::New();
    value->image_url = image_data->url_prefix +
                       background.image_file.BaseName().AsUTF8Unsafe();
    value->author = background.author;
    value->link = background.link;
    backgrounds.push_back(std::move(value));
  }

  std::move(callback).Run(std::move(backgrounds));
}

void NewTabPageHandler::GetCustomBackgrounds(
    GetCustomBackgroundsCallback callback) {
  auto backgrounds = NTPBackgroundPrefs(pref_service_).GetCustomImageList();
  for (auto& background : backgrounds) {
    background = GetCustomImageURL(background);
  }
  std::move(callback).Run(std::move(backgrounds));
}

void NewTabPageHandler::GetSelectedBackground(
    GetSelectedBackgroundCallback callback) {
  auto background = mojom::SelectedBackground::New();

  NTPBackgroundPrefs bg_prefs(pref_service_);
  switch (bg_prefs.GetType()) {
    case NTPBackgroundPrefs::Type::kBrave:
      background->type = mojom::SelectedBackgroundType::kBrave;
      break;
    case NTPBackgroundPrefs::Type::kCustomImage:
      background->type = mojom::SelectedBackgroundType::kCustom;
      if (!bg_prefs.ShouldUseRandomValue()) {
        background->value = GetCustomImageURL(bg_prefs.GetSelectedValue());
      }
      break;
    case NTPBackgroundPrefs::Type::kColor:
      if (!bg_prefs.ShouldUseRandomValue()) {
        background->value = bg_prefs.GetSelectedValue();
        background->type = base::Contains(background->value, "gradient")
                               ? mojom::SelectedBackgroundType::kGradient
                               : mojom::SelectedBackgroundType::kSolid;
      } else if (bg_prefs.GetSelectedValue() == "gradient") {
        background->type = mojom::SelectedBackgroundType::kGradient;
      } else {
        background->type = mojom::SelectedBackgroundType::kSolid;
      }
      break;
  }

  std::move(callback).Run(std::move(background));
}

void NewTabPageHandler::GetSponsoredImageBackground(
    GetSponsoredImageBackgroundCallback callback) {
  // TODO(zenparsing): Can view_counter_service_ be null?
  auto data = page_restored_
                  ? view_counter_service_->GetNextWallpaperForDisplay()
                  : view_counter_service_->GetCurrentWallpaperForDisplay();

  if (!data) {
    std::move(callback).Run(nullptr);
    return;
  }

  auto sponsored_image = ReadSponsoredImageData(*data);
  if (sponsored_image) {
    view_counter_service_->BrandedWallpaperWillBeDisplayed(
        sponsored_image->wallpaper_id, sponsored_image->creative_instance_id,
        sponsored_image->campaign_id);
  }

  std::move(callback).Run(std::move(sponsored_image));
}

void NewTabPageHandler::SelectBackground(
    mojom::SelectedBackgroundPtr background,
    SelectBackgroundCallback callback) {
  bool random = background->value.empty();
  std::string pref_value = background->value;

  auto bg_prefs = NTPBackgroundPrefs(pref_service_);

  switch (background->type) {
    case mojom::SelectedBackgroundType::kBrave:
      bg_prefs.SetType(NTPBackgroundPrefs::Type::kBrave);
      break;
    case mojom::SelectedBackgroundType::kSolid:
      bg_prefs.SetType(NTPBackgroundPrefs::Type::kColor);
      if (random) {
        pref_value = "solid";
      }
      break;
    case mojom::SelectedBackgroundType::kGradient:
      bg_prefs.SetType(NTPBackgroundPrefs::Type::kColor);
      if (random) {
        pref_value = "gradient";
      }
      break;
    case mojom::SelectedBackgroundType::kCustom:
      bg_prefs.SetType(NTPBackgroundPrefs::Type::kCustomImage);
      if (!random) {
        pref_value = CustomImageNameFromURL(background->value);
      }
      break;
  }

  bg_prefs.SetSelectedValue(pref_value);
  bg_prefs.SetShouldUseRandomValue(random);

  std::move(callback).Run();
}

void NewTabPageHandler::ShowCustomBackgroundChooser(
    ShowCustomBackgroundChooserCallback callback) {
  custom_image_chooser_->ShowDialog(
      base::BindOnce(&NewTabPageHandler::OnCustomBackgroundsSelected,
                     base::Unretained(this), std::move(callback)));
}

void NewTabPageHandler::AddCustomBackgrounds(
    AddCustomBackgroundsCallback callback) {
  // Move the chosen image paths into a local variable.
  std::vector image_paths = std::move(custom_image_paths_);

  // Create a repeating callback that will gather up the results of saving the
  // custom images to the user's profile.
  auto on_image_saved = base::BarrierCallback<base::FilePath>(
      image_paths.size(),
      base::BindOnce(&NewTabPageHandler::OnCustomBackgroundsSaved,
                     base::Unretained(this), std::move(callback)));

  // Since `CustomBackgroundFileManager` will run callbacks with a const ref
  // to a base::FilePath, we need another step to copy the path.
  auto copy_path = base::BindRepeating(
      [](const base::FilePath& path) { return base::FilePath(path); });

  for (auto& path : image_paths) {
    custom_file_manager_->SaveImage(path, copy_path.Then(on_image_saved));
  }
}

void NewTabPageHandler::RemoveCustomBackground(
    const std::string& background_url,
    RemoveCustomBackgroundCallback callback) {
  auto converter = CustomBackgroundFileManager::Converter(
      GURL(background_url), custom_file_manager_.get());
  auto file_path = std::move(converter).To<base::FilePath>();
  custom_file_manager_->RemoveImage(
      file_path,
      base::BindOnce(&NewTabPageHandler::OnCustomBackgroundRemoved,
                     base::Unretained(this), std::move(callback), file_path));
}

void NewTabPageHandler::OnCustomBackgroundsSelected(
    ShowCustomBackgroundChooserCallback callback,
    std::vector<base::FilePath> paths) {
  custom_image_paths_ = std::move(paths);
  std::move(callback).Run(!custom_image_paths_.empty());
}

void NewTabPageHandler::OnCustomBackgroundsSaved(
    AddCustomBackgroundsCallback callback,
    std::vector<base::FilePath> paths) {
  auto bg_prefs = NTPBackgroundPrefs(pref_service_);

  constexpr int kMaxCustomImageBackgrounds = 24;
  auto can_add_image = [&bg_prefs] {
    return bg_prefs.GetCustomImageList().size() < kMaxCustomImageBackgrounds;
  };

  std::string file_name;

  // For each successfully saved image, either add it to the custom image list
  // or remove the file from the user's profile.
  for (auto& path : paths) {
    if (!path.empty()) {
      if (can_add_image()) {
        file_name =
            CustomBackgroundFileManager::Converter(path).To<std::string>();
        bg_prefs.AddCustomImageToList(file_name);
      } else {
        custom_file_manager_->RemoveImage(path, base::DoNothing());
      }
    }
  }

  // Select the last added image file as the current background.
  if (!file_name.empty()) {
    bg_prefs.SetType(NTPBackgroundPrefs::Type::kCustomImage);
    bg_prefs.SetSelectedValue(file_name);
    bg_prefs.SetShouldUseRandomValue(false);
  }

  std::move(callback).Run();
}

void NewTabPageHandler::OnCustomBackgroundRemoved(
    RemoveCustomBackgroundCallback callback,
    base::FilePath path,
    bool success) {
  if (!success) {
    std::move(callback).Run();
    return;
  }

  auto file_name =
      CustomBackgroundFileManager::Converter(path).To<std::string>();

  auto bg_prefs = NTPBackgroundPrefs(pref_service_);
  bg_prefs.RemoveCustomImageFromList(file_name);

  // If we are removing the currently selected background, either select the
  // first remaining custom background, or, if there are none left, then select
  // a default background.
  if (bg_prefs.GetType() == NTPBackgroundPrefs::Type::kCustomImage &&
      bg_prefs.GetSelectedValue() == file_name) {
    auto custom_images = bg_prefs.GetCustomImageList();
    if (custom_images.empty()) {
      bg_prefs.SetType(NTPBackgroundPrefs::Type::kBrave);
      bg_prefs.SetSelectedValue("");
      bg_prefs.SetShouldUseRandomValue(true);
    } else {
      bg_prefs.SetSelectedValue(custom_images.front());
    }
  }

  std::move(callback).Run();
}

void NewTabPageHandler::OnUpdate(UpdateObserver::Source update_source) {
  if (!page_.is_bound()) {
    return;
  }
  switch (update_source) {
    case UpdateObserver::Source::kBackgroundPrefs:
      page_->OnBackgroundPrefsUpdated();
      break;
  }
}

}  // namespace brave_new_tab
