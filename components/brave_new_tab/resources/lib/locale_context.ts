/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

export interface Locale<K extends string = string> {
  getString: (key: K) => string
  getPluralString: (key: K, count: number) => Promise<string>
}

export const LocaleContext = React.createContext<Locale>({
  getString: () => '',
  getPluralString: async () => ''
})

export function useLocale<K extends string = string>(): Locale<K> {
  return React.useContext(LocaleContext)
}

export function usePluralString<K extends string = string>(
  key: K,
  count: number | undefined | null
) {
  const { getPluralString } = useLocale<K>()
  const [value, setValue] = React.useState('')

  React.useEffect(() => {
    if (typeof count !== 'number') {
      setValue('')
      return
    }
    let canUpdate = true
    getPluralString(key, count).then((newValue) => {
      if (canUpdate) {
        setValue(newValue)
      }
    })
    return () => { canUpdate = false }
  }, [getPluralString, count])

  return value
}
