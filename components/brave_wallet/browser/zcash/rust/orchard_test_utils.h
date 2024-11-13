// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_RUST_ORCHARD_TEST_UTILS_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_RUST_ORCHARD_TEST_UTILS_H_

#include <memory>

#include "brave/components/brave_wallet/common/zcash_utils.h"

namespace brave_wallet::orchard {

class OrchardTestUtils {
 public:
  virtual ~OrchardTestUtils() = default;

  virtual OrchardCommitmentValue CreateMockCommitmentValue(uint32_t position,
                                                           uint32_t rseed) = 0;

  static std::unique_ptr<OrchardTestUtils> Create();
};

}  // namespace brave_wallet::orchard

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_RUST_ORCHARD_TEST_UTILS_H_
