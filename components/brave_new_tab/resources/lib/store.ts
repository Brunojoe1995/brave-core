/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

type Listener<State> = (state: State) => void

type UpdateFunction<State> = (state: State) => Partial<State>

export interface Store<State> {
  getState: () => State
  update: (source: Partial<State> | UpdateFunction<State>) => void
  addListener: (listener: Listener<State>) => (() => void)
}

export function createStore<State>(initialState: State): Store<State> {
  const listeners = new Set<Listener<State>>()
  const state = { ...initialState }
  let notificationQueued = false

  function notify() {
    if (notificationQueued) {
      return
    }
    notificationQueued = true
    queueMicrotask(() => {
      notificationQueued = false
      for (const listener of listeners) {
        if (notificationQueued) {
          break
        }
        try {
          listener(state)
        } catch (e) {
          queueMicrotask(() => { throw e })
        }
      }
    })
  }

  return {

    getState() {
      return state
    },

    update(source: Partial<State> | UpdateFunction<State>) {
      if (typeof source === 'function') {
        source = source(state)
      }
      for (const [key, value] of Object.entries(source)) {
        if (value !== undefined) {
          (state as any)[key] = value
        }
      }
      notify()
    },

    addListener(listener: Listener<State>) {
      if (!listeners.has(listener)) {
        listeners.add(listener)
      }
      return () => {
        listeners.delete(listener)
      }
    },

  }
}
