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
//!
//------------------------------------------------------------------------------
//!
//! \if COND_API_LIBRARY_CRC_TUTORIAL
//! \defgroup API_LIBRARY_CRC_TUTORIAL Tutorial
//! \ingroup API_LIBRARY_CRC
//!
//! \brief
//! Tutorial for CRC Generic library
//!
//! \details
//! Tutorial is here \ref tutorial
//!
//! \endif
//!
//------------------------------------------------------------------------------
//!
//! \addtogroup API_LIBRARY_CRC_TUTORIAL
//! \{
//!
//------------------------------------------------------------------------------
#ifndef EXAMPLES_H
#define EXAMPLES_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "../inc/api_crc.h"

#define MAX_BUFFER_SIZE (UINT32_MAX << 3)
#define MASKBYTE 255U

//------------------------------------------------------------------------------
// function prototypes for the Examples
//------------------------------------------------------------------------------
int crc_processing(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const *const buffer, uint8_t const offset, uint32_t const size);
int crc_solver(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const offset, char *const file);
int crc_coder(uint64_t polynomial, api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const offset, char *const infile, char *const outfile);
uint8_t crc_Order(uint64_t value);
void output_Check(const uint64_t crc_Value, api_crc_state_t *const state);

#endif // EXAMPLES_H
//------------------------------------------------------------------------------
//!
//! \} // API_LIBRARY_CRC_TUTORIAL
//!
//------------------------------------------------------------------------------
//!
//! \endcond // COND_API_LIBRARY_CRC_TUTORIAL
//! \endcond // COND_API_LIBRARY_CRC
//! \endcond // COND_API_LIBRARY
//! \endcond // COND_API
//!
//------------------------------------------------------------------------------
