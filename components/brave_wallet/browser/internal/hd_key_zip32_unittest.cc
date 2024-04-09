// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/brave_wallet/browser/internal/hd_key_zip32.h"

#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "brave/components/brave_wallet/common/zcash_utils.h"
#include "testing/gtest/include/gtest/gtest.h"

static_assert(BUILDFLAG(ENABLE_ORCHARD));

namespace brave_wallet {

TEST(HDKeyZip32Test, GenerateFromSeed) {
  // https://github.com/zcash/librustzcash/blob/zcash_address-0.3.2/components/zcash_address/src/kind/unified/address/test_vectors.rs
  auto hd_key =
      HDKeyZip32::GenerateFromSeed(
          {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
           0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
           0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f})
          ->DeriveHardenedChild(kZip32Purpose)
          ->DeriveHardenedChild(static_cast<uint32_t>(mojom::CoinType::ZEC));

  {
    constexpr std::array<uint8_t, kOrchardRawBytesSize> expected = {
        0xce, 0xcb, 0xe5, 0xe6, 0x89, 0xa4, 0x53, 0xa3, 0xfe, 0x10, 0xcc,
        0xf7, 0x61, 0x7e, 0x6c, 0x1f, 0xb3, 0x82, 0x81, 0x9d, 0x7f, 0xc9,
        0x20, 0x0a, 0x1f, 0x42, 0x09, 0x2a, 0xc8, 0x4a, 0x30, 0x37, 0x8f,
        0x8c, 0x1f, 0xb9, 0x0d, 0xff, 0x71, 0xa6, 0xd5, 0x04, 0x2d};
    EXPECT_EQ(expected, hd_key->DeriveHardenedChild(1)
                            ->GetDiversifiedAddress(3, OrchardKind::External)
                            .value());
  }

  {
    constexpr std::array<uint8_t, kOrchardRawBytesSize> expected = {
        0x24, 0xf8, 0xa6, 0x0c, 0xbd, 0x97, 0xe0, 0x12, 0x61, 0x8d, 0x56,
        0x05, 0x4a, 0xd3, 0x92, 0x41, 0x41, 0x1a, 0x28, 0xfd, 0xd5, 0x0e,
        0xe3, 0x5e, 0xfa, 0x91, 0x15, 0x2f, 0x60, 0xd5, 0xfa, 0x21, 0x17,
        0x2e, 0x5d, 0x45, 0x8d, 0xdb, 0xcb, 0x6b, 0x70, 0x98, 0x96};
    EXPECT_EQ(expected, hd_key->DeriveHardenedChild(1)
                            ->GetDiversifiedAddress(7, OrchardKind::External)
                            .value());
  }

  {
    constexpr std::array<uint8_t, kOrchardRawBytesSize> expected = {
        0x1f, 0x24, 0x29, 0x4e, 0xd1, 0xb4, 0x05, 0xc7, 0xb3, 0xb1, 0xc3,
        0xf1, 0x3d, 0xb5, 0xb9, 0xb2, 0x7b, 0x5d, 0x0f, 0x2a, 0xca, 0x9d,
        0x58, 0x9a, 0x69, 0xe5, 0xbe, 0x00, 0xeb, 0x97, 0x86, 0x21, 0xe6,
        0x77, 0x6e, 0x87, 0xea, 0x32, 0x6d, 0x47, 0xa3, 0x4c, 0x1a};
    EXPECT_EQ(expected, hd_key->DeriveHardenedChild(1)
                            ->GetDiversifiedAddress(8, OrchardKind::External)
                            .value());
  }

  {
    constexpr std::array<uint8_t, kOrchardRawBytesSize> expected = {
        0x95, 0x3f, 0x3c, 0x78, 0xd1, 0x03, 0xc3, 0x2b, 0x60, 0x55, 0x92,
        0x99, 0x46, 0x2e, 0xbb, 0x27, 0x34, 0x89, 0x64, 0xb8, 0x92, 0xac,
        0xad, 0x10, 0x48, 0x2f, 0xe5, 0x02, 0xc9, 0x9f, 0x0d, 0x52, 0x49,
        0x59, 0xba, 0x7b, 0xe4, 0xf1, 0x88, 0xe3, 0xa2, 0x71, 0x38};
    EXPECT_EQ(expected, hd_key->DeriveHardenedChild(2)
                            ->GetDiversifiedAddress(0, OrchardKind::External)
                            .value());
  }
}

}  // namespace brave_wallet
