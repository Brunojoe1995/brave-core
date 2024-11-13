/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_ORCHARD_SYNC_STATE_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_ORCHARD_SYNC_STATE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "brave/components/brave_wallet/browser/internal/orchard_shard_tree_manager.h"
#include "brave/components/brave_wallet/browser/zcash/zcash_orchard_storage.h"
#include "brave/components/brave_wallet/common/zcash_utils.h"

namespace brave_wallet {

class ZCashOrchardSyncState {
 public:
  explicit ZCashOrchardSyncState(base::FilePath path_to_database);
  ~ZCashOrchardSyncState();

  base::expected<ZCashOrchardStorage::AccountMeta, ZCashOrchardStorage::Error>
  RegisterAccount(mojom::AccountIdPtr account_id,
                  uint64_t account_birthday_block);

  base::expected<ZCashOrchardStorage::AccountMeta, ZCashOrchardStorage::Error>
  GetAccountMeta(mojom::AccountIdPtr account_id);

  std::optional<ZCashOrchardStorage::Error> HandleChainReorg(
      mojom::AccountIdPtr account_id,
      uint32_t reorg_block_id,
      const std::string& reorg_block_hash);

  base::expected<std::vector<OrchardNote>, ZCashOrchardStorage::Error>
  GetSpendableNotes(mojom::AccountIdPtr account_id);

  base::expected<std::vector<OrchardNoteSpend>, ZCashOrchardStorage::Error>
  GetNullifiers(mojom::AccountIdPtr account_id);

  std::optional<ZCashOrchardStorage::Error> UpdateNotes(
      mojom::AccountIdPtr account_id,
      OrchardBlockScanner::Result block_scanner_results,
      const uint32_t latest_scanned_block,
      const std::string& latest_scanned_block_hash);

  base::expected<bool, ZCashOrchardStorage::Error> ResetAccountSyncState(
      mojom::AccountIdPtr account_id);
  void ResetDatabase();

  base::expected<std::optional<uint32_t>, ZCashOrchardStorage::Error>
  GetLatestShardIndex(mojom::AccountIdPtr account_id);

  base::expected<std::optional<uint32_t>, ZCashOrchardStorage::Error>
  GetMaxCheckpointedHeight(mojom::AccountIdPtr account_id,
                           uint32_t chain_tip_height,
                           size_t min_confirmations);

  base::expected<std::vector<OrchardInput>, ZCashOrchardStorage::Error>
  CalculateWitnessForCheckpoint(mojom::AccountIdPtr account_id,
                                std::vector<OrchardInput> notes,
                                uint32_t checkpoint_position);

 private:
  void OverrideShardTreeManagerForTesting(
      const mojom::AccountIdPtr& account_id,
      std::unique_ptr<OrchardShardTreeManager> manager);

  OrchardShardTreeManager* GetOrCreateShardTreeManager(
      const mojom::AccountIdPtr& account_id);

  scoped_refptr<ZCashOrchardStorage> storage_;
  std::map<mojom::AccountIdPtr, std::unique_ptr<OrchardShardTreeManager>>
      shard_tree_managers_;
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_ORCHARD_SYNC_STATE_H_
