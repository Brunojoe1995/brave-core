/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_MAC_FEATURES_H_
#define BRAVE_BROWSER_MAC_FEATURES_H_

#include "base/feature_list.h"

namespace {

BASE_DECLARE_FEATURE(kBraveUseOmaha4);

}

bool ShouldUseSparkle();

bool ShouldUseOmaha4();

#endif  // BRAVE_BROWSER_MAC_FEATURES_H_
