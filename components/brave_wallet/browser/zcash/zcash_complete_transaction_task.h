// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_COMPLETE_TRANSACTION_TASK_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_COMPLETE_TRANSACTION_TASK_H_

#include <memory>
#include <string>

#include "brave/components/brave_wallet/browser/internal/orchard_bundle_manager.h"
#include "brave/components/brave_wallet/browser/keyring_service.h"
#include "brave/components/brave_wallet/browser/zcash/zcash_orchard_storage.h"
#include "brave/components/brave_wallet/browser/zcash/zcash_transaction.h"
#include "brave/components/brave_wallet/common/buildflags.h"

namespace brave_wallet {

class ZCashWalletService;

// Completes transaction by signing transparent inputs and generating orchard
// part(if needed)
class ZCashCompleteTransactionTask {
 public:
  using ZCashCompleteTransactionTaskCallback =
      base::OnceCallback<void(base::expected<ZCashTransaction, std::string>)>;
  explicit ZCashCompleteTransactionTask(
      ZCashWalletService* zcash_wallet_service,
      const std::string& chain_id,
      const ZCashTransaction& transaction,
      const mojom::AccountIdPtr& account_id,
      ZCashCompleteTransactionTaskCallback callback);
  ~ZCashCompleteTransactionTask();

  void Start();

 private:
  void ScheduleWorkOnTask();
  void WorkOnTask();

  void GetLatestBlock();
  void OnGetLatestBlockHeight(
      base::expected<zcash::mojom::BlockIDPtr, std::string> result);

#if BUILDFLAG(ENABLE_ORCHARD)
  void GetMaxCheckpointedHeight();
  void OnGetMaxCheckpointedHeight(
      base::expected<std::optional<uint32_t>, ZCashOrchardStorage::Error>
          result);

  void CalculateWitness();
  void OnWitnessCalulcateResult(
      base::expected<std::vector<OrchardInput>, ZCashOrchardStorage::Error>
          result);

  void GetTreeState();
  void OnGetTreeState(
      base::expected<zcash::mojom::TreeStatePtr, std::string> result);

  void SignOrchardPart();
  void OnSignOrchardPartComplete(
      std::unique_ptr<OrchardBundleManager> orchard_bundle_manager);
#endif  // BUILDFLAG(ENABLE_ORCHARD)

  void SignTransparentPart();

  raw_ptr<ZCashWalletService> zcash_wallet_service_;  // Owns `this`.
  std::string chain_id_;
  ZCashTransaction transaction_;
  mojom::AccountIdPtr account_id_;
  ZCashCompleteTransactionTaskCallback callback_;

  std::optional<std::string> error_;
  std::optional<uint32_t> chain_tip_height_;

#if BUILDFLAG(ENABLE_ORCHARD)
  std::optional<std::vector<OrchardInput>> witness_inputs_;
  std::optional<uint32_t> anchor_block_height_;
  std::optional<zcash::mojom::TreeStatePtr> anchor_tree_state_;
#endif  // BUILDFLAG(ENABLE_ORCHARD)

  base::WeakPtrFactory<ZCashCompleteTransactionTask> weak_ptr_factory_{this};
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ZCASH_ZCASH_COMPLETE_TRANSACTION_TASK_H_
