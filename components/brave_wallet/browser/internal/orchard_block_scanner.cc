/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/internal/orchard_block_scanner.h"

#include "brave/components/brave_wallet/browser/zcash/rust/orchard_decoded_blocks_bunde.h"

namespace brave_wallet {

OrchardBlockScanner::Result::Result() = default;

OrchardBlockScanner::Result::Result(
    std::vector<OrchardNote> discovered_notes,
    std::vector<OrchardNoteSpend> spent_notes,
    std::unique_ptr<orchard::OrchardDecodedBlocksBundle> scanned_blocks)
    : discovered_notes(std::move(discovered_notes)),
      found_spends(std::move(spent_notes)),
      scanned_blocks(std::move(scanned_blocks)) {}

OrchardBlockScanner::Result::Result(OrchardBlockScanner::Result&&) = default;
OrchardBlockScanner::Result& OrchardBlockScanner::Result::operator=(
    OrchardBlockScanner::Result&&) = default;

OrchardBlockScanner::Result::~Result() = default;

OrchardBlockScanner::OrchardBlockScanner(
    const OrchardFullViewKey& full_view_key)
    : decoder_(orchard::OrchardBlockDecoder::FromFullViewKey(full_view_key)) {}

OrchardBlockScanner::~OrchardBlockScanner() = default;

base::expected<OrchardBlockScanner::Result, OrchardBlockScanner::ErrorCode>
OrchardBlockScanner::ScanBlocks(
    OrchardTreeState tree_state,
    std::vector<zcash::mojom::CompactBlockPtr> blocks) {
  std::unique_ptr<orchard::OrchardDecodedBlocksBundle> result =
      decoder_->ScanBlocks(tree_state, blocks);
  if (!result) {
    DVLOG(1) << "Failed to parse block range.";
    return base::unexpected(ErrorCode::kInputError);
  }

  if (!result->GetDiscoveredNotes()) {
    DVLOG(1) << "Failed to resolve discovered notes.";
    return base::unexpected(ErrorCode::kInputError);
  }

  std::vector<OrchardNoteSpend> found_spends;
  std::vector<OrchardNote> found_notes = result->GetDiscoveredNotes().value();

  for (const auto& block : blocks) {
    for (const auto& tx : block->vtx) {
      // We only scan orchard actions here
      for (const auto& orchard_action : tx->orchard_actions) {
        if (orchard_action->nullifier.size() != kOrchardNullifierSize) {
          return base::unexpected(ErrorCode::kInputError);
        }

        OrchardNoteSpend spend;
        // Nullifier is a public information about some note being spent.
        // Here we are trying to find a known spendable notes which nullifier
        // matches nullifier from the processed transaction.
        base::ranges::copy(orchard_action->nullifier, spend.nullifier.begin());
        spend.block_id = block->height;
        found_spends.push_back(std::move(spend));
      }
    }
  }

  return Result(
      {std::move(found_notes), std::move(found_spends), std::move(result)});
}

// static
OrchardBlockScanner::Result OrchardBlockScanner::CreateResultForTesting(
    const OrchardTreeState& tree_state,
    const std::vector<OrchardCommitment>& commitments) {
  auto builder = orchard::OrchardDecodedBlocksBundle::CreateTestingBuilder();
  for (const auto& commitment : commitments) {
    builder->AddCommitment(commitment);
  }
  builder->SetPriorTreeState(tree_state);
  return Result{{}, {}, builder->Complete()};
}

}  // namespace brave_wallet
