/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { useNewTabState } from '../lib/new_tab_context'

import { style } from './background.style'

// Loads an image in the background and resolves when the image has either
// loaded or was unable to load.
function loadImageInBackground(url: string): Promise<void> {
  return new Promise((resolve) => {
    if (!url) {
      resolve()
    }
    const onReady = () => {
      image.removeEventListener('load', onReady)
      image.removeEventListener('error', onReady)
      resolve()
    }
    const image = new Image()
    image.addEventListener('load', onReady)
    image.addEventListener('error', onReady)
    image.src = url
  })
}

function setBackgroundVariable(value: string) {
  if (value) {
    document.body.style.setProperty('--ntp-background', value)
  } else {
    document.body.style.removeProperty('--ntp-background')
  }
}

function ImageBackground(props: { url: string }) {
  // In order to avoid a "flash-of-unloaded-image", load the image in the
  // background and only update the background CSS variable when the image has
  // finished loading.
  React.useEffect(() => {
    let cancel = false
    loadImageInBackground(props.url).then(() => {
      if (!cancel) {
        setBackgroundVariable(`url(${CSS.escape(props.url)})`)
      }
    })
    return () => { cancel = true }
  }, [props.url])

  return <div className='image-background' />
}

function ColorBackground(props: { colorValue: string }) {
  React.useEffect(() => {
    setBackgroundVariable(props.colorValue)
  }, [props.colorValue])

  return <div className='color-background' />
}

export function Background() {
  const currentBackground = useNewTabState((state) => state.currentBackground)

  function renderBackground() {
    if (!currentBackground) {
      return <ColorBackground colorValue='transparent' />
    }

    switch (currentBackground.type) {
      case 'brave':
      case 'custom':
      case 'sponsored':
        return <ImageBackground url={currentBackground.imageUrl} />
      case 'solid':
      case 'gradient':
        return <ColorBackground colorValue={currentBackground.cssValue} />
    }
  }

  return (
    <div {...style}>
      {renderBackground()}
    </div>
  )
}
