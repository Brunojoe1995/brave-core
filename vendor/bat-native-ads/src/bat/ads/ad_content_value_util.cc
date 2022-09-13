/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/ad_content_value_util.h"

#include "base/values.h"
#include "bat/ads/ad_content_info.h"
#include "bat/ads/ad_type.h"

namespace ads {

namespace {

constexpr char kAdType[] = "adType";
constexpr char kPlacementId[] = "placementId";
constexpr char kCreativeInstanceId[] = "creativeInstanceId";
constexpr char kCreativeSetId[] = "creativeSetId";
constexpr char kCampaignId[] = "campaignId";
constexpr char kAdvertiserId[] = "advertiserId";
constexpr char kBrand[] = "brand";
constexpr char kBrandInfo[] = "brandInfo";
constexpr char kBrandDisplayUrl[] = "brandDisplayUrl";
constexpr char kBrandUrl[] = "brandUrl";
constexpr char kLikeAction[] = "likeAction";
constexpr char kAdAction[] = "adAction";
constexpr char kSavedAd[] = "savedAd";
constexpr char kFlaggedAd[] = "flaggedAd";

constexpr char kLegacyAdType[] = "type";
constexpr char kLegacyPlacementId[] = "uuid";
constexpr char kLegacyCreativeInstanceId[] = "creative_instance_id";
constexpr char kLegacyCreativeSetId[] = "creative_set_id";
constexpr char kLegacyCampaignId[] = "campaign_id";
constexpr char kLegacyAdvertiserId[] = "advertiser_id";
constexpr char kLegacyBrandInfo[] = "brand_info";
constexpr char kLegacyBrandDisplayUrl[] = "brand_display_url";
constexpr char kLegacyBrandUrl[] = "brand_url";
constexpr char kLegacyLikeAction[] = "like_action";
constexpr char kLegacyAdAction[] = "ad_action";
constexpr char kLegacySavesAd[] = "saved_ad";
constexpr char kLegacyFlaggedAd[] = "flagged_ad";

}  // namespace

base::Value::Dict AdContentToValue(const AdContentInfo& ad_content) {
  base::Value::Dict dict;

  dict.Set(kAdType, ad_content.type.ToString());
  dict.Set(kPlacementId, ad_content.placement_id);
  dict.Set(kCreativeInstanceId, ad_content.creative_instance_id);
  dict.Set(kCreativeSetId, ad_content.creative_set_id);
  dict.Set(kCampaignId, ad_content.campaign_id);
  dict.Set(kAdvertiserId, ad_content.advertiser_id);
  dict.Set(kBrand, ad_content.brand);
  dict.Set(kBrandInfo, ad_content.brand_info);
  dict.Set(kBrandDisplayUrl, ad_content.brand_display_url);
  dict.Set(kBrandUrl, ad_content.brand_url.spec());
  dict.Set(kLikeAction, static_cast<int>(ad_content.like_action_type));
  dict.Set(kAdAction, ad_content.confirmation_type.ToString());
  dict.Set(kSavedAd, ad_content.is_saved);
  dict.Set(kFlaggedAd, ad_content.is_flagged);

  return dict;
}

AdContentInfo AdContentFromValue(const base::Value::Dict& root) {
  AdContentInfo ad_content;

  if (const auto* value = root.FindString(kAdType)) {
    ad_content.type = AdType(*value);
  } else if (const auto* value = root.FindString(kLegacyAdType)) {
    ad_content.type = AdType(*value);
  } else {
    ad_content.type = AdType::kNotificationAd;
  }

  if (const auto* value = root.FindString(kPlacementId)) {
    ad_content.placement_id = *value;
  } else if (const auto* value = root.FindString(kLegacyPlacementId)) {
    ad_content.placement_id = *value;
  }

  if (const auto* value = root.FindString(kCreativeInstanceId)) {
    ad_content.creative_instance_id = *value;
  } else if (const auto* value = root.FindString(kLegacyCreativeInstanceId)) {
    ad_content.creative_instance_id = *value;
  }

  if (const auto* value = root.FindString(kCreativeSetId)) {
    ad_content.creative_set_id = *value;
  } else if (const auto* value = root.FindString(kLegacyCreativeSetId)) {
    ad_content.creative_set_id = *value;
  }

  if (const auto* value = root.FindString(kCampaignId)) {
    ad_content.campaign_id = *value;
  } else if (const auto* value = root.FindString(kLegacyCampaignId)) {
    ad_content.campaign_id = *value;
  }

  if (const auto* value = root.FindString(kAdvertiserId)) {
    ad_content.advertiser_id = *value;
  } else if (const auto* value = root.FindString(kLegacyAdvertiserId)) {
    ad_content.advertiser_id = *value;
  }

  if (const auto* value = root.FindString(kBrand)) {
    ad_content.brand = *value;
  }

  if (const auto* value = root.FindString(kBrandInfo)) {
    ad_content.brand_info = *value;
  } else if (const auto* value = root.FindString(kLegacyBrandInfo)) {
    ad_content.brand_info = *value;
  }

  if (const auto* value = root.FindString(kBrandDisplayUrl)) {
    ad_content.brand_display_url = *value;
  } else if (const auto* value = root.FindString(kLegacyBrandDisplayUrl)) {
    ad_content.brand_display_url = *value;
  }

  if (const auto* value = root.FindString(kBrandUrl)) {
    ad_content.brand_url = GURL(*value);
  } else if (const auto* value = root.FindString(kLegacyBrandUrl)) {
    ad_content.brand_url = GURL(*value);
  }

  if (const auto value = root.FindInt(kLikeAction)) {
    ad_content.like_action_type = static_cast<AdContentLikeActionType>(*value);
  } else if (const auto value = root.FindInt(kLegacyLikeAction)) {
    ad_content.like_action_type = static_cast<AdContentLikeActionType>(*value);
  }

  if (const auto* value = root.FindString(kAdAction)) {
    ad_content.confirmation_type = ConfirmationType(*value);
  } else if (const auto* value = root.FindString(kLegacyAdAction)) {
    ad_content.confirmation_type = ConfirmationType(*value);
  }

  if (const auto value = root.FindBool(kSavedAd)) {
    ad_content.is_saved = *value;
  } else if (const auto value = root.FindBool(kLegacySavesAd)) {
    ad_content.is_saved = *value;
  }

  if (const auto value = root.FindBool(kFlaggedAd)) {
    ad_content.is_flagged = *value;
  } else if (const auto value = root.FindBool(kLegacyFlaggedAd)) {
    ad_content.is_flagged = *value;
  }

  return ad_content;
}

}  // namespace ads
