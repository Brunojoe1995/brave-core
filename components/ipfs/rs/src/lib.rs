// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

pub mod car_header;
pub mod block_decoder;
pub mod unixfs_data;

use crate::car_header::{decode_carv1_header, decode_carv2_header};
use crate::block_decoder::{decode_block_content};

//use std::error::Error;
use std::fmt;

#[allow(unsafe_op_in_unsafe_fn)]
#[cxx::bridge(namespace = ipfs)]
mod ffi {

    extern "Rust" {
        fn decode_carv1_header(data: &CxxVector<u8>) -> CarV1HeaderResult;
        fn decode_carv2_header(data: &CxxVector<u8>) -> CarV2HeaderResult;
//        fn decode_block_info(offset: usize, data: &CxxVector<u8>) -> BlockDecodeResult;
        fn decode_block_content(offset: usize, data: &CxxVector<u8>) -> BlockDecodeResult;
    }
    #[derive(Debug)]
    pub struct CarV1Header {
        pub version: u64,
        pub roots: Vec<String>,
    }

    #[derive(Debug)]
    pub struct Characteristics {
        data: [u64; 2],
    }

    #[derive(Debug)]
    pub struct CarV2Header {
        characteristics: Characteristics,
        data_offset: u64,
        data_size: u64,
        index_offset: u64,
//        data: CarV1Header,
    }

    #[derive(Debug)]
    pub struct ErrorData {
        error: String,
        error_code: u16,
    }    

    #[derive(Debug)]
    pub struct CarV1HeaderResult {
        data: CarV1Header,
        error: ErrorData
    }

    #[derive(Debug)]
    pub struct CarV2HeaderResult {
        data: CarV2Header,
        error: ErrorData
    }

    #[derive(Debug)]
    pub struct BlockDecodeResult {
        data_offset: usize,
        cid: String,
        meta_data: String,
        content_data: Vec<u8>,
        verified: bool,
        is_content: bool,
        error: ErrorData
    }


    // #[derive(Debug)]
    // pub struct BlockContentDecodeResult {
    //     data_offset: usize,
    //     cid: String,
    //     data: Vec<u8>,
    //     verified: bool,
    //     error: ErrorData
    // }
}

impl std::error::Error for ffi::ErrorData {}
    
impl fmt::Display for ffi::ErrorData {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Error: {}  Code: {}", self.error, self.error_code)
    }
}

impl Default for ffi::BlockDecodeResult {
    fn default() -> ffi::BlockDecodeResult {
        ffi::BlockDecodeResult {
            data_offset: 0,
            cid: String::new(),
            meta_data: String::new(),
            content_data: Vec::new(),
            verified: false,
            is_content: false,
            error: ffi::ErrorData { error: String::new(), error_code: 0 },
        }
    }
}
