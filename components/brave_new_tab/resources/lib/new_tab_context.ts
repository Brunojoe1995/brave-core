/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { NewTabModel, NewTabState, defaultModel } from './new_tab_model'
import { useModelState } from './use_model_state'

export const NewTabModelContext =
  React.createContext<NewTabModel>(defaultModel())

export function useNewTabModel(): NewTabModel {
  return React.useContext(NewTabModelContext)
}

export function useNewTabState<T>(map: (state: NewTabState) => T): T {
  return useModelState(useNewTabModel(), map)
}
