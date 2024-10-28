/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { NewTabModel , NewTabState, defaultState } from '../lib/new_tab_model'
import { getNewTabProxy } from './new_tab_proxy'
import { createStore } from '../lib/store'
import { getCurrentBackground } from '../lib/background'
import { backgroundTypeFromMojo, backgroundTypeToMojo } from './new_tab_mojom'
import { debounceEvent } from './debouncer'

export function createNewTabModel(): NewTabModel {
  const { pageHandler, callbackRouter } = getNewTabProxy()
  const store = createStore<NewTabState>(defaultState())

  function updateCurrentBackground() {
    store.update({
      currentBackground: getCurrentBackground(store.getState())
    })
  }

  async function updateBackgroundsEnabled() {
    const { enabled } = await pageHandler.getBackgroundsEnabled()
    store.update({ backgroundsEnabled: enabled })
  }

  async function updateSponsoredImagesEnabled() {
    const { enabled } = await pageHandler.getSponsoredImagesEnabled()
    store.update({ sponsoredImagesEnabled: enabled })
  }

  async function updateBraveBackgrounds() {
    const { backgrounds } = await pageHandler.getBraveBackgrounds()
    store.update({
      braveBackgrounds: backgrounds.map((item) => ({ type: 'brave', ...item }))
    })
  }

  async function updateSelectedBackground() {
    const { background } = await pageHandler.getSelectedBackground()
    if (background) {
      store.update({
        selectedBackgroundType: backgroundTypeFromMojo(background.type),
        selectedBackground: background.value
      })
    } else {
      store.update({
        selectedBackgroundType: 'none',
        selectedBackground: ''
      })
    }
  }

  async function updateCustomBackgrounds() {
    const { backgrounds } = await pageHandler.getCustomBackgrounds()
    store.update({ customBackgrounds: backgrounds })
  }

  async function updateSponsoredImageBackground() {
    const { background } = await pageHandler.getSponsoredImageBackground()
    store.update({
      sponsoredImageBackground:
        background ? { type: 'sponsored', ...background } : null
    })
  }

  async function loadData() {
    await Promise.all([
      updateBackgroundsEnabled(),
      updateSponsoredImagesEnabled(),
      updateBraveBackgrounds(),
      updateCustomBackgrounds(),
      updateSelectedBackground(),
      updateSponsoredImageBackground()
    ])

    updateCurrentBackground()
  }

  callbackRouter.onBackgroundPrefsUpdated.addListener(
    debounceEvent(async () => {
      await Promise.all([
        updateCustomBackgrounds(),
        updateSelectedBackground(),
      ])
      updateCurrentBackground()
    }))

  loadData()

  return {
    getState: store.getState,

    addListener: store.addListener,

    setBackgroundsEnabled(enabled) {
      store.update({ backgroundsEnabled: enabled })
      pageHandler.setBackgroundsEnabled(enabled)
    },

    setSponsoredImagesEnabled(enabled) {
      store.update({ sponsoredImagesEnabled: enabled })
      pageHandler.setSponsoredImagesEnabled(enabled)
    },

    selectBackground(type, value) {
      store.update({
        selectedBackgroundType: type,
        selectedBackground: value
      })
      pageHandler.selectBackground({ type: backgroundTypeToMojo(type), value })
    },

    async showCustomBackgroundChooser() {
      const { imagesSelected } = await pageHandler.showCustomBackgroundChooser()
      return imagesSelected
    },

    async addCustomBackgrounds() {
      await pageHandler.addCustomBackgrounds()
    },

    async removeCustomBackground(background) {
      await pageHandler.removeCustomBackground(background)
    },
  }
}
