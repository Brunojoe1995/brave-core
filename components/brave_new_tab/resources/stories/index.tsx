/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../lib/locale_context'
import { NewTabModelContext } from '../lib/new_tab_context'
import { createNewTabModel } from './storybook_new_tab_model'
import { createLocale } from './storybook_locale'
import { App } from '../components/app'

export default {
  title: 'New Tab/Next'
}

export function NTPNext() {
  const newTabModel = React.useMemo(() => createNewTabModel(), [])
  return (
    <LocaleContext.Provider value={createLocale()}>
      <NewTabModelContext.Provider value={newTabModel}>
        <div style={{ position: 'absolute', inset: 0 }}>
          <App />
        </div>
      </NewTabModelContext.Provider>
    </LocaleContext.Provider>
  )
}
