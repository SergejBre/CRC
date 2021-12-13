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
//! \addtogroup API_LIBRARY_CRC
//! \{
//!
//------------------------------------------------------------------------------

#ifndef API_CRC_CFG_H
#define API_CRC_CFG_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Preprocessor
//------------------------------------------------------------------------------

//****************** Set options and details of include files ******************
#define BUILD_FOR_PERFORM 0	//!< Macro for optimization of build CRC library for high performance
#define BUILD_FOR_SIZE 1	//!< Macro for optimization of build CRC library for small size
#define BUILD_FOR_DATA 2	//!< Macro for optimization of build CRC library for small data

// Activate just one of the following 3 statements: BUILD_FOR_PERFORM, BUILD_FOR_SIZE, BUILD_FOR_DATA
#define OPTIMIZE BUILD_FOR_PERFORM

//! Define target platform: x86_64 or x86_32, or 16-bit, or 8-bit.
//! Activate just one of the following 4 statements:
//! \code{.cpp}
//! // CPU x86_64 (64-bit)
//! //#define PLATFORM_X86_64
//!
//! // CPU x86_32 (32-bit)
//! #define PLATFORM_X86_32
//!
//! // CPU 16-bit
//! //#define PLATFORM_X86_16
//!
//! // CPU 8-bit
//! //#define PLATFORM_X86_8
//! \endcode
//!
// CPU x86_64 (64-bit)
#define PLATFORM_X86_64

// CPU x86_32 (32-bit)
//#define PLATFORM_X86_32

// CPU 16-bit
//#define PLATFORM_X86_16

// CPU 8-bit
//#define PLATFORM_X86_8
//****************** end of list of user-definable parameters ******************

//------------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

#if defined( PLATFORM_X86_64 )
// Processor x86 64-bit
typedef uint64_t api_crc_register_t;

#elif defined( PLATFORM_X86_32 )
//! Processor x86 32-bit
typedef uint32_t api_crc_register_t;

#elif defined( PLATFORM_X86_16 )
// Processor 16-bit
typedef uint16_t api_crc_register_t;

#elif defined( PLATFORM_X86_8 )
// Processor 8-bit
typedef uint8_t api_crc_register_t;

#endif // PLATFORM_X86

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Inline Functions
//------------------------------------------------------------------------------

#endif // API_CRC_CFG_H


//------------------------------------------------------------------------------
//!
//! \} // API_LIBRARY_CRC
//!
//------------------------------------------------------------------------------
//!
//! \endcond // COND_API_LIBRARY_CRC
//! \endcond // COND_API_LIBRARY
//! \endcond // COND_API
//!
//------------------------------------------------------------------------------
