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

#ifndef __API_CRC_H__
#define __API_CRC_H__

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

//------------------------------------------------------------------------------
// Preprocessor
//------------------------------------------------------------------------------

//! \brief
//! Macro for static initialization of a Cyclic Reduncancy Code (CRC) device as
//! lvalue but without any sanity checks.
#define API_CRC_INIT(POLYNOMIAL, INPUTXOR, OUTPUTXOR, TABLE, INPUTORDER, OUTPUTORDER) (api_crc_device_s){(uint64_t)POLYNOMIAL, (uint64_t)INPUTXOR, (uint64_t)OUTPUTXOR, (uint64_t *)TABLE, (api_crc_order_e)INPUTORDER, (api_crc_order_e)OUTPUTORDER}


//------------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------------

//! \enum api_crc_order_e
//!
//! \brief
//! Bit order for processing data bytes and storing the CRC checksum.
typedef enum
{
    API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT = 0, //!< Bytes are processed / filled beginning with the Least Significant Bit (LSB).
    API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT = 1, //!< Bytes are processed / filled beginning with the Most Significant Bit (MSB).
} api_crc_order_e;

//! \enum api_crc_status_e
//!
//! \brief
//! Return values for Cyclic Redundancy Code (CRC) function calls.
typedef enum
{
    API_CRC_STATUS_SUCCESS = 0,				//!< Execution has been successful.
    API_CRC_STATUS_INVALID_PARAMETER = 1,	//!< Provided parameter(s) are invalid.
    API_CRC_STATUS_FAILED = 2,				//!< Error occurred during execution.
    API_CRC_STATUS_NOT_SUPPORTED = 3,		//!< Function call is not supported.
} api_crc_status_e;

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

//! \struct api_crc_device_s
//!
//! \brief
//! Device structure for calculating a Cyclic Redundancy Code (CRC) checksum.
typedef struct
{
    uint64_t polynomial;		//!< Generator polynomial for calculating the CRC checksum.
    uint64_t inputXOR;			//!< Initial value for XORing with the polynomial division state register.
    uint64_t outputXOR;			//!< Final value for XORing with the polynomial division state register.
    uint64_t const *table;		//!< Lookup table for processing one input byte per iteration.
    api_crc_order_e inputOrder;	//!< Bit order for processing input bytes.
    api_crc_order_e outputOrder;//!< Bit order for storing the CRC checksum.
} api_crc_device_s;

//! \var api_crc_state_t
//!
//! \brief
//! State register for storing intermediate as well as the final value of the
//! Cyclic Redundancy Code (CRC) checksum calculation.
typedef uint64_t api_crc_state_t;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

api_crc_status_e api_crc_init(uint64_t const, uint64_t const, uint64_t const, uint64_t const *const, api_crc_order_e const, api_crc_order_e const, api_crc_device_s *const);
api_crc_status_e api_crc_table(api_crc_device_s *const, uint64_t *const, uint16_t const);
uint16_t api_crc_sizeofTable(uint64_t const);

api_crc_status_e api_crc_reset(api_crc_device_s const *const, api_crc_state_t *const);
api_crc_status_e api_crc_process(api_crc_device_s const *const, api_crc_state_t *const, uint8_t const *const, uint8_t const, uint32_t const);
api_crc_status_e api_crc_finalize(api_crc_device_s const *const, api_crc_state_t *const);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Inline Functions
//------------------------------------------------------------------------------

#endif // __API_CRC_H__

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
