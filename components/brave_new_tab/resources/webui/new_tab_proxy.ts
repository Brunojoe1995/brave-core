/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as mojom from './new_tab_mojom'

interface BrowserProxy {
  callbackRouter: mojom.NewTabPageCallbackRouter
  pageHandler: mojom.NewTabPageHandlerRemote
}

let instance: BrowserProxy | null

export function getNewTabProxy() {
  if (!instance) {
    const callbackRouter = new mojom.NewTabPageCallbackRouter()
    const pageHandler = mojom.NewTabPageHandler.getRemote()
    pageHandler.setNewTabPage(callbackRouter.$.bindNewPipeAndPassRemote())
    instance = { pageHandler, callbackRouter }
  }
  return instance
}
