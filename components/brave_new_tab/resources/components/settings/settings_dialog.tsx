/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import Dialog from '@brave/leo/react/dialog'
import Navigation from '@brave/leo/react/navigation'
import NavigationItem from '@brave/leo/react/navigationItem'

import { BackgroundPanel } from './background_panel'
import { useLocale } from '../../lib/use_locale'

import { style } from './settings_dialog.style'

interface Props {
  onClose: () => void
}

type PanelType = 'background'

export function SettingsDialog(props: Props) {
  const { getString } = useLocale()

  const [currentPanel, setCurrentPanel] =
    React.useState<PanelType>('background')

  function renderPanel() {
    switch (currentPanel) {
      case 'background': return <BackgroundPanel />
    }
  }

  return (
    <div {...style}>
      <Dialog isOpen animate={false} showClose onClose={props.onClose}>
        <h3>
          {getString('settingsTitle')}
        </h3>
        <div className='panel-body'>
          <nav>
            <Navigation>
              <NavigationItem
                isCurrent={currentPanel === 'background'}
                onClick={() => setCurrentPanel('background')}
              >
                {getString('backgroundSettingsTitle')}
              </NavigationItem>
            </Navigation>
          </nav>
          <section>
            {renderPanel()}
          </section>
        </div>
      </Dialog>
    </div>
  )
}
