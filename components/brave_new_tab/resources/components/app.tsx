/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import Icon from '@brave/leo/react/icon'

import { Background } from './background'
import { SettingsDialog } from './settings/settings_dialog'

import { style } from './app.style'

export function App() {
  const [showSettings, setShowSettings] = React.useState(false)

  return (
    <div {...style}>
      <button
        className='settings'
        onClick={() => setShowSettings(true)}
      >
        <Icon name='settings' />
      </button>
      <Background />
      {
        showSettings &&
          <SettingsDialog onClose={() => setShowSettings(false)} />
      }
    </div>
  )
}
