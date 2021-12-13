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

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "../inc/api_crc_process.h"

//------------------------------------------------------------------------------
// Preprocessor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Enums
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Functions
//------------------------------------------------------------------------------

void api_crc_processForwardBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size)
{
    api_crc_register_t polynomial = 0;
    uint32_t counter = 0U;
    uint32_t bytes = 0U;
    int8_t index = 0U;
    int8_t startOffset = 0U;
    int8_t endOffset = 0U;

    if(size == 0U)
    {
        return;
    }

    polynomial = device->polynomial >> ((sizeof(device->polynomial) - sizeof(api_crc_register_t)) << 3U);
    bytes = ((uint64_t)offset + (uint64_t)size + 7U) >> 3U;
    startOffset = 7U - offset;

    while(counter < bytes)
//      while(counter < bytes-1)
    {
        endOffset = ((counter + 1U) != bytes) ? (0U) : (8U - offset - size + (counter << 3U));

        for(index = startOffset; index >= endOffset; index--)
        {
            if((((*reg) >> ((sizeof(api_crc_register_t) << 3U) - 1U)) ^ ((buffer[counter] >> index) & 1U)) != 0U)
            {
                (*reg) = ((*reg) << 1U) ^ polynomial;
            }
            else
            {
                (*reg) = (*reg) << 1U;
            }
        }

        startOffset = 7U;
        counter++;
    }
/*    endOffset = ((counter + 1U) != bytes) ? (0U) : (8U - offset - size + (counter << 3U));

    for(index = startOffset; index >= endOffset; index--)
    {
        if((((*reg) >> ((sizeof(api_crc_register_t) << 3U) - 1U)) ^ ((buffer[counter] >> index) & 1U)) != 0U)
        {
            (*reg) = ((*reg) << 1U) ^ polynomial;
        }
        else
        {
            (*reg) = (*reg) << 1U;
        }
    }*/
}


void api_crc_processReverseBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size)
{
    api_crc_register_t polynomial = 0U;
    uint32_t counter = 0U;
    uint32_t bytes = 0U;
    uint8_t index = 0U;
    uint8_t startOffset = 0U;
    uint8_t endOffset = 0U;

    if(size == 0U)
    {
        return;
    }

    polynomial = device->polynomial;
    bytes = ((uint64_t)offset + (uint64_t)size + 7U) >> 3U;
    startOffset = offset;

    while(counter < bytes)
    {
        endOffset = ((counter + 1U) != bytes) ? (8U) : (offset + size - (counter << 3U));

        for(index = startOffset; index < endOffset; index++)
        {
            if((((*reg) & 1U) ^ ((buffer[counter] >> index) & 1U)) != 0U)
            {
                (*reg) = ((*reg) >> 1U) ^ polynomial;
            }
            else
            {
                (*reg) = (*reg) >> 1U;
            }
        }

        startOffset = 0U;
        counter++;
    }
}

void api_crc_processForwardLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const api_crc_readTable)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size)
{
    uint32_t counter = 0U;

    for(counter = 0U; counter < size; counter++)
    {
        (*reg) = ((*reg) << (sizeof(buffer[0U]) << 3U)) ^ api_crc_readTable(device->table, ((*reg) >> ((sizeof(api_crc_register_t) - sizeof(buffer[0U])) << 3U)) ^ buffer[counter]);
    }
}

void api_crc_processReverseLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const api_crc_readTable)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size)
{
    uint32_t counter = 0U;

    for(counter = 0U; counter < size; counter++)
    {
        (*reg) = ((*reg) >> (sizeof(buffer[0U]) << 3U)) ^ api_crc_readTable(device->table, ((*reg) & 0xFFULL) ^ buffer[counter]);
    }
}

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
