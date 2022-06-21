// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {
  AppsListType,
  BraveWallet,
  WalletAccountType
} from '../../constants/types'

export const getMockedTransactionInfo = (): BraveWallet.TransactionInfo => {
  return {
    id: '1',
    fromAddress: '0x8b52c24d6e2600bdb8dbb6e8da849ed38ab7e81f',
    txHash: '',
    txDataUnion: {
      ethTxData1559: {
        baseData: {
          to: '0x8b52c24d6e2600bdb8dbb6e8da849ed38ab7e81f',
          value: '0x01706a99bf354000', // 103700000000000000 wei (0.1037 ETH)
          // data: new Uint8Array(0),
          data: [] as number[],
          nonce: '0x03',
          gasLimit: '0x5208', // 2100
          gasPrice: '0x22ecb25c00' // 150 Gwei
        },
        chainId: '1337',
        maxPriorityFeePerGas: '',
        maxFeePerGas: '',
        gasEstimation: undefined
      },
      ethTxData: {} as any,
      filTxData: undefined,
      solanaTxData: {} as any
    },
    txStatus: BraveWallet.TransactionStatus.Approved,
    txType: BraveWallet.TransactionType.Other,
    txParams: [],
    txArgs: [],
    createdTime: { microseconds: 0 as unknown as bigint },
    submittedTime: { microseconds: 0 as unknown as bigint },
    confirmedTime: { microseconds: 0 as unknown as bigint },
    originInfo: {
      origin: {
        scheme: 'https',
        host: 'brave.com',
        port: 443,
        nonceIfOpaque: undefined
      },
      originSpec: 'https://brave.com',
      eTldPlusOne: 'brave.com'
    }
  }
}

export const mockNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x1',
  chainName: 'Ethereum Main Net',
  rpcUrls: ['https://mainnet.infura.io/v3/'],
  blockExplorerUrls: ['https://etherscan.io'],
  symbol: 'ETH',
  symbolName: 'Ethereum',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.ETH,
  data: undefined
}

export const mockFilecoinMainnetNetwork: BraveWallet.NetworkInfo = {
  chainId: 'f',
  chainName: 'Filecoin Mainnet',
  rpcUrls: ['https://api.node.glif.io/rpc/v0'],
  blockExplorerUrls: ['https://filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.FIL,
  data: undefined
}

export const mockFilecoinTestnetNetwork: BraveWallet.NetworkInfo = {
  chainId: 't',
  chainName: 'Filecoin Testnet',
  rpcUrls: ['https://calibration.node.glif.io/rpc/v0'],
  blockExplorerUrls: ['https://calibration.filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.FIL,
  data: undefined
}

export const mockSolanaMainnetNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x65',
  chainName: 'Solana Mainnet Beta',
  rpcUrls: ['https://mainnet-beta-solana.brave.com/rpc'],
  blockExplorerUrls: ['https://explorer.solana.com'],
  symbol: 'SOL',
  symbolName: 'Solana',
  decimals: 9,
  iconUrls: [],
  coin: BraveWallet.CoinType.SOL,
  data: undefined
}

export const mockSolanaTestnetNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x66',
  chainName: 'Solana Testnet',
  rpcUrls: ['https://testnet-solana.brave.com/rpc'],
  blockExplorerUrls: ['https://explorer.solana.com?cluster=testnet'],
  symbol: 'SOL',
  symbolName: 'Solana',
  decimals: 9,
  iconUrls: [],
  coin: BraveWallet.CoinType.SOL,
  data: undefined
}

export const mockERC20Token: BraveWallet.BlockchainToken = {
  contractAddress: 'mockContractAddress',
  name: 'Dog Coin',
  symbol: 'DOG',
  logo: '',
  isErc20: true,
  isErc721: false,
  decimals: 18,
  visible: true,
  tokenId: '',
  coingeckoId: '',
  coin: BraveWallet.CoinType.ETH,
  chainId: BraveWallet.MAINNET_CHAIN_ID
}

export const mockAccount: WalletAccountType = {
  id: 'mockId',
  name: 'mockAccountName',
  address: 'mockAddress',
  nativeBalanceRegistry: {
    '0x1': '123456'
  },
  coin: BraveWallet.CoinType.ETH,
  accountType: 'Primary',
  tokenBalanceRegistry: {}
}

export const mockSolanaAccount: WalletAccountType = {
  id: 'mockId-2',
  name: 'MockSolanaAccount',
  address: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
  nativeBalanceRegistry: {
    [BraveWallet.SOLANA_MAINNET]: '1000000000',
    [BraveWallet.SOLANA_DEVNET]: '1000000000',
    [BraveWallet.SOLANA_TESTNET]: '1000000000'
  },
  coin: BraveWallet.CoinType.SOL,
  accountType: 'Primary',
  tokenBalanceRegistry: {}
}

export const mockAssetPrices: BraveWallet.AssetPrice[] = [
  {
    fromAsset: 'ETH',
    price: '4000',
    toAsset: 'mockValue',
    assetTimeframeChange: 'mockValue'
  },
  {
    fromAsset: 'DOG',
    price: '100',
    toAsset: 'mockValue',
    assetTimeframeChange: 'mockValue'
  }
]

export const mockAddresses: string[] = [
  '0xea674fdde714fd979de3edf0f56aa9716b898ec8',
  '0xdbf41e98f541f19bb044e604d2520f3893eefc79',
  '0xcee177039c99d03a6f74e95bbba2923ceea43ea2'
]

export const mockFilAddresses: string[] = [
  't1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'f1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  't3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a'
]

export const mockFilInvalilAddresses: string[] = [
  '',
  't1lqarsh4nkg545ilaoqdsbtj4uofplt6sto2ziy',
  'f1lqarsh4nkg545ilaoqdsbtj4uofplt6sto2f6ziy',
  't3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3ju3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jfpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'a1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'b1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'c3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'd3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a'
]

export const mockAppsList: AppsListType[] = [
  {
    category: 'category1',
    categoryButtonText: 'categoryButtonText1',
    appList: [
      {
        name: 'foo',
        description: 'description1'
      }
    ] as BraveWallet.AppItem[]
  },
  {
    category: 'category2',
    categoryButtonText: 'categoryButtonText2',
    appList: [
      {
        name: 'bar',
        description: 'description2'
      }
    ] as BraveWallet.AppItem[]
  }
]

export const mockSolDappSignTransactionRequest: BraveWallet.SignTransactionRequest = {
  'originInfo': {
    'origin': {
      'scheme': 'https',
      'host': 'f40y4d.csb.app',
      'port': 443,
      'nonceIfOpaque': undefined
    },
    'originSpec': 'https://f40y4d.csb.app',
    'eTldPlusOne': 'csb.app'
  },
  'id': 0,
  'fromAddress': mockSolanaAccount.address,
  'txData': {
    'ethTxData': undefined,
    'ethTxData1559': undefined,
    'filTxData': undefined,
    'solanaTxData': {
      'recentBlockhash': 'B7Kg79jDm48LMdB4JB2hu82Yfsuz5xYm2cQDBYmKdDSn',
      'lastValidBlockHeight': 0 as unknown as bigint,
      'feePayer': mockSolanaAccount.address,
      'toWalletAddress': '',
      'splTokenMintAddress': '',
      'lamports': 0 as unknown as bigint,
      'amount': 0 as unknown as bigint,
      'txType': 12,
      'instructions': [
        {
          'programId': '11111111111111111111111111111111',
          'accountMetas': [
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            },
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            }
          ],
          'data': [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0]
        }
      ],
      'sendOptions': undefined
    }
  }
}

// BraveWallet.TransactionInfo (selectedPendingTransaction)
export const mockSolDappSignAndSendTransactionRequest = {
  'id': 'e1eae32d-5bc2-40ac-85e5-2a4a5fbe8a5f',
  'fromAddress': mockSolanaAccount.address,
  'txHash': '',
  'txDataUnion': {
    'ethTxData': undefined,
    'ethTxData1559': undefined,
    'filTxData': undefined,
    'solanaTxData': {
      'recentBlockhash': 'C115cyMDVoGGYNd4r8vFy5qPJEUdoJQQCXMYYKQTQimn',
      'lastValidBlockHeight': 0 as unknown as bigint,
      'feePayer': mockSolanaAccount.address,
      'toWalletAddress': '',
      'splTokenMintAddress': '',
      'lamports': 0 as unknown as bigint,
      'amount': 0 as unknown as bigint,
      'txType': 11,
      'instructions': [
        {
          'programId': '11111111111111111111111111111111',
          'accountMetas': [
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            },
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            }
          ],
          'data': [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0]
        }
      ]
    }
  },
  'txStatus': 0,
  'txType': 11,
  'txParams': [],
  'txArgs': [],
  'createdTime': { 'microseconds': 1654540245386000 as unknown as bigint },
  'submittedTime': { 'microseconds': 0 as unknown as bigint },
  'confirmedTime': { 'microseconds': 0 as unknown as bigint },
  'originInfo': {
    'origin': {
      'scheme': 'https',
      'host': 'f40y4d.csb.app',
      'port': 443,
      'nonceIfOpaque': undefined
    },
    'originSpec': 'https://f40y4d.csb.app',
    'eTldPlusOne': 'csb.app'
  }
}

export const mockSolDappSignAllTransactionsRequest: BraveWallet.SignAllTransactionsRequest = {
  'originInfo': {
    'origin': {
      'scheme': 'https',
      'host': 'f40y4d.csb.app',
      'port': 443,
      'nonceIfOpaque': undefined
    },
    'originSpec': 'https://f40y4d.csb.app',
    'eTldPlusOne': 'csb.app'
  },
  'id': 3,
  'fromAddress': mockSolanaAccount.address,
  'txDatas': [
    {
      'ethTxData': undefined,
      'ethTxData1559': undefined,
      'filTxData': undefined,
      'solanaTxData': {
        'recentBlockhash': '8Yq6DGZBh9oEJsCVhUjTqN9kPiLoeYJ7J4n9TnpPYjqW',
        'lastValidBlockHeight': 0 as unknown as bigint,
        'feePayer': mockSolanaAccount.address,
        'toWalletAddress': '',
        'splTokenMintAddress': '',
        'lamports': 0 as unknown as bigint,
        'amount': 0 as unknown as bigint,
        'txType': 12,
        'instructions': [{
          'programId': '11111111111111111111111111111111',
          'accountMetas': [
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            },
            {
              'pubkey': mockSolanaAccount.address,
              'isSigner': true,
              'isWritable': true
            }
          ],
          'data': [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0]
        }],
        'sendOptions': undefined
      }
    },
    {
      'ethTxData': undefined,
      'ethTxData1559': undefined,
      'filTxData': undefined,
      'solanaTxData': {
        'recentBlockhash': '8Yq6DGZBh9oEJsCVhUjTqN9kPiLoeYJ7J4n9TnpPYjqW',
        'lastValidBlockHeight': 0 as unknown as bigint,
        'feePayer': mockSolanaAccount.address,
        'toWalletAddress': '',
        'splTokenMintAddress': '',
        'lamports': 0 as unknown as bigint,
        'amount': 0 as unknown as bigint,
        'txType': 12,
        'instructions': [
          {
            'programId': '11111111111111111111111111111111',
            'accountMetas': [
              {
                'pubkey': mockSolanaAccount.address,
                'isSigner': true,
                'isWritable': true
              },
              {
                'pubkey': mockSolanaAccount.address,
                'isSigner': true,
                'isWritable': true
              }
            ],
            'data': [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0]
          }
        ],
        'sendOptions': undefined
      }
    }
  ]
}
