/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import Icon from '@brave/leo/react/icon'
import ProgressRing from '@brave/leo/react/progressRing'
import Toggle from '@brave/leo/react/toggle'

import { BackgroundType } from '../../lib/new_tab_model'
import { useNewTabModel, useNewTabState } from '../../lib/new_tab_context'
import { useLocale } from '../../lib/use_locale'
import { useCallbackWrapper } from '../../lib/use_callback_wrapper'
import { inlineCSSVars } from '../../lib/inline_css_vars'
import { BackgroundTypePanel } from './background_type_panel'

import {
  backgroundCSSValue,
  gradientPreviewBackground,
  solidPreviewBackground } from '../../lib/background'

import { style } from './background_panel.style'

export function BackgroundPanel() {
  const { getString } = useLocale()
  const model = useNewTabModel()
  const wrapCallback = useCallbackWrapper()

  const [
    backgroundsEnabled,
    sponsoredImagesEnabled,
    selectedBackgroundType,
    selectedBackground,
    braveBackgrounds,
    customBackgrounds
  ] = useNewTabState((state) => [
    state.backgroundsEnabled,
    state.sponsoredImagesEnabled,
    state.selectedBackgroundType,
    state.selectedBackground,
    state.braveBackgrounds,
    state.customBackgrounds
  ])

  const [panelType, setPanelType] = React.useState<BackgroundType>('none')
  const [uploading, setUploading] = React.useState(false)

  function getTypePreviewValue(type: BackgroundType) {
    const isSelectedType = type === selectedBackgroundType
    switch (type) {
      case 'brave':
        return braveBackgrounds.at(0)?.imageUrl || ''
      case 'custom':
        if (isSelectedType && selectedBackground) {
          return selectedBackground
        }
        return customBackgrounds.at(0) ?? ''
      case 'solid':
        if (isSelectedType && selectedBackground) {
          return selectedBackground
        }
        return solidPreviewBackground
      case 'gradient':
        if (isSelectedType && selectedBackground) {
          return selectedBackground
        }
        return gradientPreviewBackground
      case 'none':
        return ''
    }
  }

  function renderUploadPreview() {
    return (
      <div className='preview upload'>
        {uploading ? <ProgressRing /> : <Icon name='upload' />}
        {getString('uploadBackgroundLabel')}
      </div>
    )
  }

  function renderTypePreview(type: BackgroundType) {
    if (type === 'custom' && customBackgrounds.length === 0) {
      return renderUploadPreview()
    }
    return (
      <div
        className='preview'
        style={inlineCSSVars({
          '--preview-background':
            backgroundCSSValue(type, getTypePreviewValue(type))
        })}
      >
        {
          type === selectedBackgroundType &&
            <span className='selected-marker'>
              <Icon name='check-normal' />
            </span>
        }
      </div>
    )
  }

  function showCustomBackgroundChooser() {
    model.showCustomBackgroundChooser().then(wrapCallback((selected) => {
      if (selected && !uploading) {
        setUploading(true)
        model.addCustomBackgrounds().then(wrapCallback(() => {
          setUploading(false)
        }))
      }
    }))
  }

  function onCustomPreviewClick() {
    if (customBackgrounds.length === 0) {
      showCustomBackgroundChooser()
    } else {
      setPanelType('custom')
    }
  }

  if (panelType !== 'none') {
    return (
      <div {...style}>
        <BackgroundTypePanel
          backgroundType={panelType}
          isUploading={uploading}
          renderUploadOption={() => (
            <button onClick={showCustomBackgroundChooser}>
              {renderUploadPreview()}
            </button>
          )}
          onClose={() => { setPanelType('none') }}
        />
      </div>
    )
  }

  return (
    <div {...style}>
      <div className='toggle-row'>
        <label>{getString('showBackgroundsLabel')}</label>
        <Toggle
          size='small'
          checked={backgroundsEnabled}
          onChange={({ checked }) => { model.setBackgroundsEnabled(checked) }}
        />
      </div>
      {
        backgroundsEnabled && <>
          <div className='background-options'>
            <div className='background-option'>
              <button onClick={onCustomPreviewClick}>
                {renderTypePreview('custom')}
                {getString('customBackgroundLabel')}
              </button>
            </div>
            <div className='background-option'>
              <button onClick={() => model.selectBackground('brave', '')}>
                {renderTypePreview('brave')}
                {getString('braveBackgroundLabel')}
              </button>
            </div>
            <div className='background-option'>
              <button onClick={() => setPanelType('solid')}>
                {renderTypePreview('solid')}
                {getString('solidBackgroundLabel')}
              </button>
            </div>
            <div className='background-option'>
              <button onClick={() => setPanelType('gradient')}>
                {renderTypePreview('gradient')}
                {getString('gradientBackgroundLabel')}
              </button>
            </div>
          </div>
          <div className='toggle-row'>
            <label>{getString('showSponsoredImagesLabel')}</label>
            <Toggle
              size='small'
              checked={sponsoredImagesEnabled}
              onChange={({ checked }) => {
                model.setSponsoredImagesEnabled(checked)
              }}
            />
          </div>
        </>
      }
    </div>
  )
}
