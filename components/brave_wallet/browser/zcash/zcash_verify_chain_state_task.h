/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_VERIFY_CHAIN_STATE_TASK_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_VERIFY_CHAIN_STATE_TASK_

#include "brave/components/brave_wallet/browser/zcash/zcash_shield_sync_service.h"

namespace brave_wallet {

class ZCashVerifyChainStateTask {
 public:
  using ZCashVerifyChainStateTaskCallback = base::OnceCallback<void(
      base::expected<bool, ZCashShieldSyncService::Error>)>;
  ZCashVerifyChainStateTask(ZCashShieldSyncService* sync_service,
                            ZCashVerifyChainStateTaskCallback callback);
  ~ZCashVerifyChainStateTask();

  void Start();

 private:
  void WorkOnTask();
  void ScheduleWorkOnTask();

  void GetAccountMeta();
  void OnGetAccountMeta(base::expected<ZCashOrchardStorage::AccountMeta,
                                       ZCashOrchardStorage::Error> result);

  void GetChainTipBlock();
  void OnGetChainTipBlock(
      base::expected<zcash::mojom::BlockIDPtr, std::string> result);

  void VerifyChainState();

  // Verifies that last known scanned block hash is unchanged
  void GetTreeStateForLatestScannedBlock();
  void OnGetTreeStateForChainVerification(
      base::expected<zcash::mojom::TreeStatePtr, std::string> tree_state);

  // Resolves block hash for the block we are going to fallback
  void GetTreeStateForChainReorg(uint32_t new_block_id);
  void OnGetTreeStateForChainReorg(
      uint32_t new_block_height,
      base::expected<zcash::mojom::TreeStatePtr, std::string> tree_state);
  void OnDatabaseUpdatedForChainReorg(
      std::optional<ZCashOrchardStorage::Error> error);

  raw_ptr<ZCashShieldSyncService> sync_service_; /* Owns this */
  ZCashVerifyChainStateTaskCallback callback_;

  std::optional<ZCashShieldSyncService::Error> error_;
  std::optional<ZCashOrchardStorage::AccountMeta> account_meta_;
  // Latest block in the blockchain
  std::optional<uint32_t> chain_tip_block_;
  bool chain_state_verified_ = false;

  base::WeakPtrFactory<ZCashVerifyChainStateTask> weak_ptr_factory_{this};
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_VERIFY_CHAIN_STATE_TASK_
