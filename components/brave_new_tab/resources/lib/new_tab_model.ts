/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

export type BackgroundType = 'brave' | 'custom' | 'solid' | 'gradient' | 'none'

interface BraveBackground {
  type: 'brave'
  author: string
  imageUrl: string
  link: string
}

interface ColorBackground {
  type: 'solid' | 'gradient'
  cssValue: string
}

interface CustomBackground {
  type: 'custom'
  imageUrl: string
}

interface SponsoredImageBackground {
  type: 'sponsored'
  imageUrl: string,
  creativeInstanceId: string
  wallpaperId: string
}

export type Background =
  BraveBackground |
  ColorBackground |
  CustomBackground |
  SponsoredImageBackground

export interface NewTabState {
  backgroundsEnabled: boolean
  sponsoredImagesEnabled: boolean
  braveBackgrounds: BraveBackground[]
  customBackgrounds: string[]
  selectedBackgroundType: BackgroundType
  selectedBackground: string
  currentBackground: Background | null
  sponsoredImageBackground: SponsoredImageBackground | null
}

export function defaultState(): NewTabState {
  return {
    backgroundsEnabled: true,
    sponsoredImagesEnabled: true,
    braveBackgrounds: [],
    customBackgrounds: [],
    selectedBackgroundType: 'none',
    selectedBackground: '',
    currentBackground: null,
    sponsoredImageBackground: null,
  }
}

export interface NewTabModel {
  getState: () => NewTabState
  addListener: (listener: (state: NewTabState) => void) => () => void
  setBackgroundsEnabled: (enabled: boolean) => void
  setSponsoredImagesEnabled: (enabled: boolean) => void
  selectBackground: (type: BackgroundType, value: string) => void
  showCustomBackgroundChooser: () => Promise<boolean>
  addCustomBackgrounds: () => Promise<void>
  removeCustomBackground: (background: string) => void
}

export function defaultModel(): NewTabModel {
  const state = defaultState()
  return {
    getState() { return state },
    addListener() { return () => {} },
    setBackgroundsEnabled(enabled) {},
    setSponsoredImagesEnabled(enabled) {},
    selectBackground(type, value) {},
    async showCustomBackgroundChooser() { return false },
    async addCustomBackgrounds() {},
    removeCustomBackground(background) {},
  }
}
