//------------------------------------------------------------------------------
//  Home Office
//  Nürnberg, Germany
//  E-Mail: sergej1@email.ua
//
//  Copyright (C) 2021 free Project CRC Generic library. All rights reserved.
//------------------------------------------------------------------------------
//  The CRC Generic library is designed as a universal solution for most practical
//  CRC methods and provides a flexible API for communication application developers
//------------------------------------------------------------------------------
//!
//! \cond COND_API
//! \cond COND_API_LIBRARY
//! \cond COND_API_LIBRARY_CRC
//! \cond COND_API_LIBRARY_CRC_INTERFACE
//!
//------------------------------------------------------------------------------
//!
//! \addtogroup API_LIBRARY_CRC_INTERFACE
//! \{
//!
//------------------------------------------------------------------------------

#ifndef API_CRC_PROCESS_H
#define API_CRC_PROCESS_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "api_crc.h"
#include "../cfg/api_crc_cfg.h"

//------------------------------------------------------------------------------
// Preprocessor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void api_crc_processForwardBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size);
void api_crc_processReverseBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size);
void api_crc_processForwardLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size);
void api_crc_processReverseLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size);

#endif // API_CRC_PROCESS_H


//------------------------------------------------------------------------------
//!
//! \} // API_LIBRARY_CRC_INTERFACE
//!
//------------------------------------------------------------------------------
//!
//! \endcond // COND_API_LIBRARY_CRC_INTERFACE
//! \endcond // COND_API_LIBRARY_CRC
//! \endcond // COND_API_LIBRARY
//! \endcond // COND_API
//!
//------------------------------------------------------------------------------
