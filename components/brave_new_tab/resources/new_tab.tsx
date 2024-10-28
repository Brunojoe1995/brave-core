/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { createRoot } from 'react-dom/client'
import { setIconBasePath } from '@brave/leo/react/icon'

import { LocaleContext } from './lib/locale_context'
import { NewTabModelContext } from './lib/new_tab_context'
import { createNewTabModel } from './webui/webui_new_tab_model'
import { createLocale } from './webui/webui_locale'
import { App } from './components/app'

setIconBasePath('chrome://resources/brave-icons')

function whenDocumentReady() {
  return new Promise<void>((resolve) => {
    if (document.readyState === 'loading') {
      document.addEventListener('DOMContentLoaded', () => resolve())
    } else {
      resolve()
    }
  })
}

whenDocumentReady().then(() => {
  const newTabModel = createNewTabModel()

  Object.assign(self, {
    [Symbol.for('newTabModel')]: newTabModel
  })

  createRoot(document.getElementById('root')!).render(
    <LocaleContext.Provider value={createLocale()}>
      <NewTabModelContext.Provider value={newTabModel}>
        <App />
      </NewTabModelContext.Provider>
    </LocaleContext.Provider>
  )
})
