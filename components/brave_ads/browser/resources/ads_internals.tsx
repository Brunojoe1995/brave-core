// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as AdsInternalsMojo from 'gen/brave/components/services/bat_ads/public/interfaces/bat_ads.mojom.m.js'
import * as React from 'react'
import styled from 'styled-components'
import { render } from 'react-dom'
import 'react-json-view-lite/dist/index.css';

const Container = styled.div`
  margin: 4px;
  display: flex;
  flex-direction: column;
  gap: 10px;
`

const ButtonContainer = styled.div`
  display: flex;
  flex-direction: row;
  gap: 4px;
`

const StateContainer = styled.div`
  padding: 4px;
`

const Table = styled.table`
  table-layout: auto;
  width: 100%;
  border-collapse: collapse;

  th, td {
    border: 1px solid #ddd;
    padding: 8px;
    text-align: left;
  }

  th {
    background-color: #f2f2f2;
  }
`

const API = AdsInternalsMojo.AdsInternals.getRemote()

const TABLE_COLUMN_URL_PATTERN = 'URL Pattern';
const TABLE_COLUMN_EXPIRES_AT = 'Expires At';

interface AdsInternal {
  TABLE_COLUMN_URL_PATTERN: string
  TABLE_COLUMN_EXPIRES_AT: number
}

const App: React.FC = () => {
  const [adsInternals, setAdsInternals] = React.useState<AdsInternal[]>([])

  const getAdsInternals = React.useCallback(async () => {
    try {
      const response = await API.getAdsInternals()
      setAdsInternals(JSON.parse(response.response) || [])
    } catch (error) {
      console.error('Failed to fetch ads internals:', error)
    }
  }, [])

  const clearAdsData = React.useCallback(async () => {
    try {
      await API.clearAdsData()
      getAdsInternals()
    } catch (error) {
      console.error("Failed to clear ads data", error)
    }
  }, [getAdsInternals])

  React.useEffect(() => {
    getAdsInternals()
  }, [getAdsInternals])

  return (
    <Container>
      <h2>Ads internals</h2>

      <ButtonContainer>
        <button onClick={getAdsInternals}>Fetch Brave Ads conversion URL patterns</button>
      </ButtonContainer>

      <StateContainer>
        <b>Active Brave Ads conversion URL patterns:</b><br /><br />
        <BraveSearchAdsTable data={adsInternals} /><br /><br />
      </StateContainer>

      <ButtonContainer>
        <button onClick={clearAdsData}>Clear Ads Data</button>
      </ButtonContainer>
    </Container>
  )
}

const formatUnixEpochToLocalTime = (epoch: number) => {
  const date = new Date(epoch * 1000)  // Convert seconds to milliseconds
  return date.toLocaleString()
}

const BraveSearchAdsTable: React.FC<{ data: AdsInternal[] }> = React.memo(({ data }) => {
  if (data.length === 0) return <p>No Brave Ads conversion URL patterns are currently being matched.</p>

  const tableColumnOrder = [TABLE_COLUMN_URL_PATTERN, TABLE_COLUMN_EXPIRES_AT]

  const uniqueRows = React.useMemo(() => {
    return Array.from(new Set(data.map(item => JSON.stringify(item))))
      .map(item => JSON.parse(item))
  }, [data])

  return (
    <Table>
      <thead>
        <tr>
          {tableColumnOrder.map((header) => (
            <th key={header}>{header}</th>
          ))}
        </tr>
      </thead>
      <tbody>
        {uniqueRows.map((row, index) => (
          <tr key={index}>
            {tableColumnOrder.map((header) => (
              <td key={header}>
                {header === TABLE_COLUMN_EXPIRES_AT ? formatUnixEpochToLocalTime(row[header]) : row[header]}
              </td>
            ))}
          </tr>
        ))}
      </tbody>
    </Table>
  )
})

document.addEventListener('DOMContentLoaded', () => {
  render(<App />, document.getElementById('root'))
})
