/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { BackgroundType } from '../lib/new_tab_model'

import * as mojom from 'gen/brave/components/brave_new_tab/common/new_tab_page.mojom.m.js'

export * from 'gen/brave/components/brave_new_tab/common/new_tab_page.mojom.m.js'

export function backgroundTypeFromMojo(type: number): BackgroundType {
  switch (type) {
    case mojom.SelectedBackgroundType.kBrave: return 'brave'
    case mojom.SelectedBackgroundType.kCustom: return 'custom'
    case mojom.SelectedBackgroundType.kSolid: return 'solid'
    case mojom.SelectedBackgroundType.kGradient: return 'gradient'
    default: return 'none'
  }
}

export function backgroundTypeToMojo(type: BackgroundType) {
  switch (type) {
    case 'brave': return mojom.SelectedBackgroundType.kBrave
    case 'custom': return mojom.SelectedBackgroundType.kCustom
    case 'solid': return mojom.SelectedBackgroundType.kSolid
    case 'gradient': return mojom.SelectedBackgroundType.kGradient
    case 'none': return mojom.SelectedBackgroundType.kSolid
  }
}
