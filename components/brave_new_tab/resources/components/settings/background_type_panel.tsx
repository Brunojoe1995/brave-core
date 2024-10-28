/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import Icon from '@brave/leo/react/icon'
import Toggle from '@brave/leo/react/toggle'

import { BackgroundType } from '../../lib/new_tab_model'
import { useNewTabModel, useNewTabState } from '../../lib/new_tab_context'
import { useLocale } from '../../lib/use_locale'
import { inlineCSSVars } from '../../lib/inline_css_vars'

import {
  backgroundCSSValue,
  solidBackgrounds,
  gradientBackgrounds } from '../../lib/background'

interface Props {
  backgroundType: BackgroundType
  isUploading: boolean
  renderUploadOption: () => React.ReactNode
  onClose: () => void
}

export function BackgroundTypePanel(props: Props) {
  const { getString } = useLocale()
  const model = useNewTabModel()

  const [
    selectedBackgroundType,
    selectedBackground,
    customBackgrounds,
    currentBackground
  ] = useNewTabState((state) => [
    state.selectedBackgroundType,
    state.selectedBackground,
    state.customBackgrounds,
    state.currentBackground
  ])

  const type = props.backgroundType

  function panelTitle() {
    switch (props.backgroundType) {
      case 'custom': return getString('customBackgroundTitle')
      case 'gradient': return getString('gradientBackgroundTitle')
      case 'solid': return getString('solidBackgroundTitle')
      default: return ''
    }
  }

  function panelValues() {
    switch (props.backgroundType) {
      case 'custom': return customBackgrounds
      case 'gradient': return gradientBackgrounds
      case 'solid': return solidBackgrounds
      default: return []
    }
  }

  function onRandomizeToggle(detail: { checked: boolean }) {
    if (detail.checked) {
      model.selectBackground(type, '')
    } else if (currentBackground) {
      switch (currentBackground.type) {
        case 'custom':
          model.selectBackground(type, currentBackground.imageUrl)
          break
        case 'solid':
        case 'gradient':
          model.selectBackground(type, currentBackground.cssValue)
          break
        default:
          break
      }
    }
  }

  function renderOption(value: string) {
    const isSelected =
      selectedBackgroundType === type &&
      selectedBackground === value

    const classNames = ['background-option']
    if (type === 'custom') {
      classNames.push('can-remove')
    }

    return (
      <div key={value} className={classNames.join(' ')}>
        <button onClick={() => { model.selectBackground(type, value) }}>
          <div
            className='preview'
            style={inlineCSSVars({
              '--preview-background': backgroundCSSValue(type, value)
            })}
          >
            {
              isSelected &&
                <span className='selected-marker'>
                  <Icon name='check-normal' />
                </span>
            }
          </div>
        </button>
        {
          type === 'custom' &&
            <button
              className='remove-image'
              onClick={() => { model.removeCustomBackground(value) }}
            >
              <Icon name='close' />
            </button>
        }
      </div>
    )
  }

  const values = panelValues()

  return (
    <>
      <h4>
        <button onClick={props.onClose}>
          <Icon name='arrow-left' />
          {panelTitle()}
        </button>
      </h4>
      <div className='toggle-row'>
        <label>{getString('randomizeBackgroundLabel')}</label>
        <Toggle
          size='small'
          checked={selectedBackgroundType === type && !selectedBackground}
          disabled={values.length === 0}
          onChange={onRandomizeToggle}
        />
      </div>
      <div className='background-options'>
        {values.map(renderOption)}
        {type === 'custom' && props.renderUploadOption()}
      </div>
    </>
  )
}
