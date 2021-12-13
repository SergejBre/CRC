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
//! \defgroup API_LIBRARY_CRC Cyclic Redundancy Code (CRC)
//! \ingroup API_LIBRARY
//!
//! \brief
//! Documentation for CRC Generic library
//!
//! \details
//! The documentation consists of four groups
//!
//------------------------------------------------------------------------------
//!
//! \if COND_API_LIBRARY_CRC_SPECIFICATION
//! \defgroup API_LIBRARY_CRC_SPECIFICATION Specification
//! \ingroup API_LIBRARY_CRC
//!
//! \brief
//! The general requirements for project CRC Generic library
//!
//! \details
//! <b>The General</b>\n (Software Requirements Specification)
//!
//! What is to be done by the CRC-Generic library?\n
//! The functional and non-functional requirements of the CRC-Generic library?\n
//! The application cases for the library?\n
//! The relevant to library data models?\n
//!
//! The cyclic redundancy check (CRC) is a method for determining a check value
//! for data in order to detect errors in transmission or storage binary data.
//! Ideally, the process can even correct the received data automatically to avoid retransmission.\n
//!
//! The CRC Generic library is designed to use on a generic way the any desired
//! CRC process and provides an interface to the communications applications.\n
//!
//! The following features are offered of the CRC library (see also \ref CRC_GENERIC_SECTION_WORKFLOW):
//!  - A interface for setting CRC process and the initiation of relevant data
//!  - Check and validation of the initial data
//!  - The use of all relevant generators polynomials up to and including CRC-64
//!  - Works with any data formats (MSB, and LSB order of the significant bits)
//!  - The library processed the data streams with offset
//!  \.
//!
//! The library is currently available as:
//!  - 64-bit Linux static library (<tt>lib/libCRC_generic_static.a</tt>)
//!  - 64-bit Linux shared library (<tt>lib/libCRC_generic_dynamic.so</tt>)
//!  \.
//!
//! The library is built and can be used with GNU compiler gcc (version >= 4.7.2) and 64-bit Debian GNU/Linux 8.2 (jessie).
//! The installation process is described here: \ref CRC_GENERIC_INSTALLATION.
//!
//! \endif
//!
//------------------------------------------------------------------------------
//!
//! \cond COND_API_LIBRARY_CRC_INTERFACE
//!
//------------------------------------------------------------------------------
//!
//! \defgroup API_LIBRARY_CRC_INTERFACE Interface
//! \ingroup API_LIBRARY_CRC
//! \{
//!
//! \brief
//! The Interface of the CRC Generic library
//!
//! \details
//! The Interface of the CRC Generic library consists of the 6 public functions:
//! - The initialization function: \ref api_crc_init
//! - The LUT table function: \ref api_crc_table
//! - The function gives size of the table back: \ref api_crc_sizeofTable
//! - The method for cleaning of old data: \ref api_crc_reset
//! - The method for calculate the CRC proof sum: \ref api_crc_process
//! - The method for return the requested CRC value: \ref api_crc_finalize
//! \.
//! a data structure: \ref api_crc_device_s \n
//!
//! and a state variable: \ref api_crc_state_t \n
//!
//! TODO Define alignment of each variable / parameter
//! TODO Define format of each variable / parameter
//!
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "../inc/api_crc.h"
#include "../cfg/api_crc_cfg.h"

#if (OPTIMIZE == BUILD_FOR_PERFORM)
#include "../inc/api_crc_process.h"
#endif // OPTIMIZE

//#include <stdio.h>

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

// Format Polynomial: right aligned with least significant bit omitted --> x^n + x^n-1 + ... + x^1==0th-Bit
// Count leading zeroes to determine the order of the polynomial
// inputXOR and outputXOR sind ebenso right aligned, aber dieses Mal beginnend mit dem LSB

// Bearbeitung:
// Normal MSB nach LSB: Links-Ausrichtung aller werte
// - - - -

// Reflected Input dreht nicht den Input XOR oder den Output XOR

    // Ein paar Fälle:
    // 1. Offset am Anfang
    // 2. Ende gleich am Anfang
    // 3. startOffset = 7 und endOffset = 0
    // 4. startOffset = 7 und endOffset != 0


static uint64_t api_crc_reverse64(uint64_t const value)
{
    uint64_t temp = value;

    temp = ((temp >>  1U) & 0x5555555555555555U) | ((temp & 0x5555555555555555U) <<  1U);
    temp = ((temp >>  2U) & 0x3333333333333333U) | ((temp & 0x3333333333333333U) <<  2U);
    temp = ((temp >>  4U) & 0x0F0F0F0F0F0F0F0FU) | ((temp & 0x0F0F0F0F0F0F0F0FU) <<  4U);
    temp = ((temp >>  8U) & 0x00FF00FF00FF00FFU) | ((temp & 0x00FF00FF00FF00FFU) <<  8U);
    temp = ((temp >> 16U) & 0x0000FFFF0000FFFFU) | ((temp & 0x0000FFFF0000FFFFU) << 16U);
    temp = ((temp >> 32U) & 0x00000000FFFFFFFFU) | ((temp & 0x00000000FFFFFFFFU) << 32U);
    return temp;
}


// TODO Preprocessor decision, register shall be used for internal processing,
// TODO which limits the width of CRC polynomials but increases speed in
// TODO processing. Implementation needs to be adjusted in order to get it
// TODO working for all configurations. Currently works for register == uint64.
//typedef uint8_t api_crc_register_t;
//typedef uint16_t api_crc_register_t;
//typedef uint32_t api_crc_register_t;
//typedef uint64_t api_crc_register_t;
// TODO END


static api_crc_register_t api_crc_readForwardTable08(uint64_t const *const table, uint8_t const index)
{
    return (((api_crc_register_t)(((uint8_t const *const)table)[index])) << ((sizeof(api_crc_register_t) - sizeof(uint8_t)) << 3U));
}


static api_crc_register_t api_crc_readForwardTable16(uint64_t const *const table, uint8_t const index)
{
    return (((api_crc_register_t)(((uint16_t const *const)table)[index])) << ((sizeof(api_crc_register_t) - sizeof(uint16_t)) << 3U));
}


static api_crc_register_t api_crc_readForwardTable32(uint64_t const *const table, uint8_t const index)
{
    return (((api_crc_register_t)(((uint32_t const *const)table)[index])) << ((sizeof(api_crc_register_t) - sizeof(uint32_t)) << 3U));
}

#ifdef PLATFORM_X86_64
static api_crc_register_t api_crc_readForwardTable64(uint64_t const *const table, uint8_t const index)
{
    return (((api_crc_register_t)(((uint64_t const *const)table)[index])) << ((sizeof(api_crc_register_t) - sizeof(uint64_t)) << 3U));
}
#endif

static api_crc_register_t api_crc_readReverseTable08(uint64_t const *const table, uint8_t const index)
{
    return ((api_crc_register_t)(((uint8_t const *const)table)[index]));
}


static api_crc_register_t api_crc_readReverseTable16(uint64_t const *const table, uint8_t const index)
{
    return ((api_crc_register_t)(((uint16_t const *const)table)[index]));
}


static api_crc_register_t api_crc_readReverseTable32(uint64_t const *const table, uint8_t const index)
{
    return ((api_crc_register_t)(((uint32_t const *const)table)[index]));
}

#ifdef PLATFORM_X86_64
static api_crc_register_t api_crc_readReverseTable64(uint64_t const *const table, uint8_t const index)
{
    return ((api_crc_register_t)(((uint64_t const *const)table)[index]));
}
#endif

static void api_crc_writeForwardTable08(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint8_t *const)table)[index] = (uint8_t)(reg >> ((sizeof(api_crc_register_t) - sizeof(uint8_t)) << 3U));
}


static void api_crc_writeForwardTable16(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint16_t *const)table)[index] = (uint16_t)(reg >> ((sizeof(api_crc_register_t) - sizeof(uint16_t)) << 3U));
}


static void api_crc_writeForwardTable32(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint32_t *const)table)[index] = (uint32_t)(reg >> ((sizeof(api_crc_register_t) - sizeof(uint32_t)) << 3U));
}

#ifdef PLATFORM_X86_64
static void api_crc_writeForwardTable64(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint64_t *const)table)[index] = (uint64_t)(reg >> ((sizeof(api_crc_register_t) - sizeof(uint64_t)) << 3U));
}
#endif

static void api_crc_writeReverseTable08(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint8_t *const)table)[index] = (uint8_t)reg;
}


static void api_crc_writeReverseTable16(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint16_t *const)table)[index] = (uint16_t)reg;
}


static void api_crc_writeReverseTable32(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint32_t *const)table)[index] = (uint32_t)reg;
}

#ifdef PLATFORM_X86_64
static void api_crc_writeReverseTable64(api_crc_register_t const reg, uint64_t *const table, uint8_t const index)
{
    ((uint64_t *const)table)[index] = (uint64_t)reg;
}
#endif

static uint16_t api_crc_order2Size(uint8_t const order)
{
    uint16_t size = 0;

    if(order <= 8U)
    {
        size = 1U << 8U;
    }
    else if(order <= 16)
    {
        size = 2U << 8U;
    }
    else if(order <= 32)
    {
        size = 4U << 8U;
    }
    else if(order <= 64)
    {
        size = 8U << 8U;
    }
    else
    {
        // Not possible as the biggest possible register type consists of at
        // most 64 bits.
        size = 0;
    }

    return size;
}

#if (OPTIMIZE != BUILD_FOR_PERFORM)
static void api_crc_processForwardBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size)
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

//    while(counter < bytes)
      while(counter < bytes-1)
    {
//        endOffset = ((counter + 1U) != bytes) ? (0U) : (8U - offset - size + (counter << 3U));

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
}


static void api_crc_processForwardLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const api_crc_readTable)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size)
{
//    printf("processForwardLUT\n");
    uint32_t counter = 0U;

    for(counter = 0U; counter < size; counter++)
    {
        (*reg) = ((*reg) << (sizeof(buffer[0U]) << 3U)) ^ api_crc_readTable(device->table, ((*reg) >> ((sizeof(api_crc_register_t) - sizeof(buffer[0U])) << 3U)) ^ buffer[counter]);
    }
}


static void api_crc_processReverseBit(api_crc_device_s const *const device, api_crc_register_t *const reg, uint8_t const *const buffer, uint8_t const offset, uint32_t const size)
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


static void api_crc_processReverseLUT(api_crc_device_s const *const device, api_crc_register_t *const reg, api_crc_register_t (*const api_crc_readTable)(uint64_t const *const, uint8_t const), uint8_t const *const buffer, uint32_t const size)
{
//    printf("processReverseLUT\n");
    uint32_t counter = 0U;

    for(counter = 0U; counter < size; counter++)
    {
        (*reg) = ((*reg) >> (sizeof(buffer[0U]) << 3U)) ^ api_crc_readTable(device->table, ((*reg) & 0xFFULL) ^ buffer[counter]);
    }
}

#endif // OPTIMIZE

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//!
//! \brief
//! The initialization function is used to fill the data necessary for the calculation CRC.
//!
//! \details
//! The method is suitable for the initialization of the parameters that are relewant
//! for computing CRC. The call of the function is required each time,
//! if you need a new method (CRC-16, CRC-32, etc.) for calculating CRC.
//! Before calling this function to be prepared for the initialization values:
//! This applies to all fountain polynomial generator (for details see the description of the arguments).
//! When you call the function is the validation of arguments, except for pointer to the table.
//! Check for the correct value before You use the function ::api_crc_process !
//!
//!
//! \param[in] polynomial Binary polynomial aligned to the Least Significant Bit
//! (LSB) while the power X^0 has been explicitly omitted as always set (1).
//! For example, for the polynomial CRC-16 \f$G(x)=x^{16}+x^{15}+x^{2}+1\f$, binary representation:
//! 0b11000000000000101 cast the smallest of bits in its binary representation:
//! 0b1100000000000010 = 0xC002.
//!
//! \param[in] inputXOR Initial value for XORing with the state register.
//!
//! \param[in] outputXOR Final value for XORing with the state register.
//!
//! \param[in] table Pointer to address space containing a suitably sized lookup
//! table (entry as well as table size) for given parameters in order to use a
//! fast implementation for byte-wise processing while a NULL pointer indicates
//! the use of a slow implementation for bit-wise processing.
//!
//! \param[in] inputOrder ::api_crc_order_e Each data byte is either processed from Least
//! Significant Bit (LSB) to the Most Significant Bit (MSB) or vice versa.
//!
//! \param[in] outputOrder ::api_crc_order_e Calculated checksum is either stored from Least
//! Significant Bit(LSB) to the Most Significant Bit (MSB) or vice versa.
//!
//! \param[in,out] *device ::api_crc_device_s Specification of Cyclic Redundancy Code for given
//! parameters.
//!
//! \retval ::API_CRC_STATUS_SUCCESS Exclusive access has been granted.
//! \retval ::API_CRC_STATUS_INVALID_PARAMETER Provided parameter(s) are invalid.
//! \retval ::API_CRC_STATUS_NOT_SUPPORTED Function call is not supported.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_init"];
//!
//! CALLER=>FU[label = "api_crc_table(polynomial,inputXOR,outputXOR,table,inputOrder,outputOrder,&device)"];
//!
//! FU=>FU[label = "(device == 0)||(((uint64_t)table & 7U) != 0U)||(...)"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_INVALID_PARAMETER", linecolor = "red"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_SUCCESS", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! For example, consider the preparation of initial parameters (arguments)
//! for calculating CRC with the generator polynomial CRC-16 \f$G(x)=x^{16}+x^{15}+x^{2}+1\f$,
//! in binary representation: 0b11000000000000101).
//! as an argument for the function is polynomial = 0b1100000000000010 (0xC002),
//! - polynomial = 0xC002;
//! - inputXOR = 0b0000000000000000 (= 0x00);
//! - outputXOR = 0b0000000000000000 (= 0x00);
//! - Bits Order: LSB
//! - inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
//! - outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
//! - for Directly (LFSR) process has to table = 0
//! \code{.cpp}
//! api_crc_device_s device;
//! // for Directly (LFSR) process
//! uint64_t *table = 0U;
//!
//! // init of the device
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//! - for LUT-process argument must be a valid pointer to the table.
//! \code{.cpp}
//! api_crc_device_s device;
//!
//! // for LUT-process
//! uint32_t table_size = api_crc_sizeofTable(polynomial);
//! uint16_t table[256];
//!
//! // init of the device
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // Filling in the table
//! if (api_crc_table(&device, (uint64_t*) table, table_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//!
//! \pre
//! Before calling the method, the pointer must be points to the table as follows:
//! - for Directly (LFSR) process
//! \code{.cpp}
//! api_crc_device_s device;
//! // for Directly (LFSR) process
//! uint64_t *table = 0U;
//!
//! // init of the device
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//! - this method requires a call to the function ::api_crc_table for LUT-process.
//! \code{.cpp}
//! api_crc_device_s device;
//!
//! // for LUT-process
//! uint32_t table_size = api_crc_sizeofTable(polynomial);
//! uint16_t table[256];
//!
//! // Filling in the table
//! if (api_crc_table(&device, (uint64_t*) table, table_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // init of the device
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//!
//! \post
//! In principle after calling this method should also be called function ::api_crc_reset for full initialization.
//! (<tt>state = inputXOR</tt>). In order to avoid incorrect filling of the data, the function ::api_crc_init must be called
//!  before the function ::api_crc_table
//!
//! \note
//! - The polynomial to be converted.
//! - If you want to use the LUT process, should then be passed a valid pointer to the LUT table,
//! otherwise (Directly LFSR process) should then be passed a null pointer.
//!
//! \warning
//! When you call the function is the validation of arguments, except for pointer to the table.
//! Check argument *table for the correct value before You use the function ::api_crc_process.
//!
//------------------------------------------------------------------------------
api_crc_status_e api_crc_init(uint64_t const polynomial, uint64_t const inputXOR, uint64_t const outputXOR, uint64_t const *const table, api_crc_order_e const inputOrder, api_crc_order_e const outputOrder, api_crc_device_s *const device)
{
    uint8_t zeroes = __builtin_clzll(polynomial);
    uint8_t order = (sizeof(polynomial) << 3U) - zeroes;
//    printf("Order is = %d\n", order);
//    printf("Max Value %llx\n", (-1ULL >> zeroes));

    if ((device == 0) || (((uint64_t)table & 7U) != 0U) || (order > (sizeof(api_crc_register_t) << 3U))
            || (inputXOR > (-1ULL >> zeroes)) || (outputXOR > (-1ULL >> zeroes))
            || ((inputOrder != API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
                && (inputOrder != API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT))
            || ((outputOrder != API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
                && (outputOrder != API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT)))
    {
        return API_CRC_STATUS_INVALID_PARAMETER;
    }

    device->polynomial = (((polynomial & (-1ULL >> (order + 1))) << 1U) | 1U) << zeroes;
    device->inputXOR = inputXOR << zeroes;
    device->table = table;
    device->inputOrder = inputOrder;
    device->outputOrder = outputOrder;

    if(inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
        device->polynomial = api_crc_reverse64(device->polynomial);
        device->inputXOR = api_crc_reverse64(device->inputXOR);

        if(outputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
        {
            device->outputXOR = outputXOR;
        }
        else
        {
            device->outputXOR = api_crc_reverse64(outputXOR) >> zeroes;
        }
    }
    else
    {
        if(outputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
        {
            device->outputXOR = api_crc_reverse64(outputXOR);
        }
        else
        {
            device->outputXOR = outputXOR << zeroes;
        }
    }

    return API_CRC_STATUS_SUCCESS;
}

//------------------------------------------------------------------------------
//!
//! \brief
//! The function fills the LUT table with the previously calculated values.
//!
//! \details
//! - The function decides at the width of the generator polynomial a type of table.
//! - For example, for the polynomial CRC-16 (0xC002) a table of type uint16_t is used,
//! for polynomial CRC-32 is a table of type uint32_t, etc.
//!
//! \param[in,out] *device ::api_crc_device_s Specification of Cyclic Redundancy Code for given
//! parameters.
//!
//! \param[in,out] *table Pointer to address space containing a suitably sized lookup
//! table (entry as well as table size) for given parameters in order to use a
//! fast implementation for byte-wise processing. For example, uint16_t table[256], uint32_t table[256], etc.
//!
//! \param[in] size The size of the table in bytes. To explore the proper size of the table,
//! it is strongly discouraged to use the function api_crc_sizeofTable(polynomial).
//! For example, for polynomial CRC-16 (0xC002) is size of table = 512 bytes.
//!
//! \retval ::API_CRC_STATUS_SUCCESS Exclusive access has been granted.
//! \retval ::API_CRC_STATUS_INVALID_PARAMETER Provided parameter(s) are invalid.
//! \retval ::API_CRC_STATUS_NOT_SUPPORTED Function call is not supported.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_table"];
//!
//! CALLER=>FU[label = "api_crc_table(&device,table,size)"];
//!
//! FU=>FU[label = "(device == 0)||(table == 0)||(size != api_crc_order2Size())"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_INVALID_PARAMETER", linecolor = "red"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_SUCCESS", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! For example, consider the preparation of initial parameters (arguments)
//! for calculating CRC32 with the generator polynomial CRC32: \f$G(x)=x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11}+x^{10}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1\f$,
//! in binary representation: 0b100000100110000010001110110110111.\n
//! As an argument for the function is polynomial = 0b10000010011000001000111011011011 (0x82608EDB),
//! - polynomial = 0x82608EDB;
//! - inputXOR = 0xFFFFFFFF;
//! - outputXOR = 0x00000000;
//! - Bits Order: MSB
//! \.
//!
//! \code{.cpp}
//! api_crc_device_s device;
//!
//! // for LUT-process
//! uint32_t table_size = api_crc_sizeofTable(polynomial);
//! uint16_t table[256];
//!
//! // Filling in the table
//! if (api_crc_table(&device, (uint64_t*) table, table_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // init of the device
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//!
//! \pre
//! requires a value <tt>size</tt>, which can be calculated using the function ::api_crc_sizeofTable.
//! In order to avoid incorrect filling of the data, the function ::api_crc_init must be called
//! before the function ::api_crc_table
//!
//! \post
//! Nothing.
//!
//! \note
//! the table that was created with the help of this function could be makes also use for further calculations.
//!
//! \warning
//! Nothing.
//------------------------------------------------------------------------------
api_crc_status_e api_crc_table(api_crc_device_s *device, uint64_t *const table, uint16_t size)
{
    api_crc_register_t reg = 0;
    void (*api_crc_processBit)(api_crc_device_s const *const, api_crc_register_t *const, uint8_t const *const, uint8_t const, uint32_t const) = 0U;
    void (*api_crc_writeTable)(api_crc_register_t const reg, uint64_t *const table, uint8_t const index) = 0U;
    uint16_t counter = 0;
    uint8_t order = 0;

    if((device == 0) || (table == 0)
            || (size != api_crc_order2Size((sizeof(device->polynomial) << 3U) - ((device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT) ? (__builtin_clzll(device->polynomial)) : (__builtin_ctzll(device->polynomial))))))
    {
        return API_CRC_STATUS_INVALID_PARAMETER;
    }
//    printf("Size of Table is %d\n", api_crc_order2Size((sizeof(device->polynomial) << 3U) - ((device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT) ? (__builtin_clzll(device->polynomial)) : (__builtin_ctzll(device->polynomial)))));

    if(device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
        api_crc_processBit = api_crc_processReverseBit;
        order = (sizeof(device->polynomial) << 3U) - __builtin_clzll(device->polynomial);

        if(order <= 8U)
        {
            api_crc_writeTable = api_crc_writeReverseTable08;
        }
        else if(order <= 16U)
        {
            api_crc_writeTable = api_crc_writeReverseTable16;
        }
        else if(order <= 32U)
        {
            api_crc_writeTable = api_crc_writeReverseTable32;
        }
#ifdef PLATFORM_X86_64
        else if(order <= 64U)
        {
            api_crc_writeTable = api_crc_writeReverseTable64;
        }
#endif
        else
        {
            // Not possible as the biggest possible register type consists of at
            // most 64 bits.
        }
    }
    else
    {
        api_crc_processBit = api_crc_processForwardBit;
        order = (sizeof(device->polynomial) << 3U) - __builtin_ctzll(device->polynomial);

        if(order <= 8U)
        {
            api_crc_writeTable = api_crc_writeForwardTable08;
        }
        else if(order <= 16U)
        {
            api_crc_writeTable = api_crc_writeForwardTable16;
        }
        else if(order <= 32U)
        {
            api_crc_writeTable = api_crc_writeForwardTable32;
        }
#ifdef PLATFORM_X86_64
        else if(order <= 64U)
        {
            api_crc_writeTable = api_crc_writeForwardTable64;
        }
#endif
        else
        {
            // Not possible as the biggest possible register type consists of at
            // most 64 bits.
        }
    }

    for(counter = 0; counter <= 255; counter++)
    {
        reg = 0U;
        api_crc_processBit(device, &reg, (uint8_t *)&counter, 0, 8);
        api_crc_writeTable(reg, table, counter);
    }

    device->table = table;
    return API_CRC_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
//!
//! \brief
//! This function returns the size of LUT table.
//!
//! \details
//!
//! \param[in] polynomial The Generator polynomial, which is taken from the initial data.
//! For example, for the polynomial CRC-16 \f$G(x)=x^{16}+x^{15}+x^{2}+1\f$, binary representation:
//! 0b11000000000000101 cast the smallest of bits in its binary representation:
//! 0b1100000000000010 = 0xC002.
//!
//! \return size of LUT table in bytes.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_sizeofTable"];
//!
//! CALLER=>FU[label = "api_crc_sizeofTable(polynomial)"];
//!
//! FU>>CALLER[label = "return size of LUT in bytes", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! - for polynomial CRC-16 (0x8005) is size of table = 512 bytes,
//! - for polynomial CRC32 (0x82608EDB) is size of table = 1024 bytes
//! \.
//! \code{.cpp}
//! uint16_t size = 0U;
//!
//! // size = 512;
//! size = api_crc_sizeofTable(0x8005);
//!
//! // size = 1024;
//! size = api_crc_sizeofTable(0x82608EDB);
//! \endcode
//!
//! \pre
//! Nothing.
//!
//! \post
//! Nothing.
//!
//! \note
//! The polynomial to be converted (see description of function ::api_crc_init.
//!
//! \warning
//! Nothing.
//!
//------------------------------------------------------------------------------
uint16_t api_crc_sizeofTable(uint64_t const polynomial)
{
    return api_crc_order2Size((sizeof(polynomial) << 3U) - __builtin_clzll(polynomial));
}


//------------------------------------------------------------------------------
//!
//! \brief
//! The method used to api_crc_reset cleaning of old data.
//!
//! \details
//! The function resets the variable state to initial value, i.e. state = device.inputXOR
//!
//! \param[in] *device ::api_crc_device_s Specification of Cyclic Redundancy Code for given parameters.
//!
//! \param[in,out] *state ::api_crc_state_t State register for storing intermediate as well as the final value of the
//! Cyclic Redundancy Code (CRC) checksum calculation.
//!
//! \retval ::API_CRC_STATUS_SUCCESS Exclusive access has been granted.
//! \retval ::API_CRC_STATUS_INVALID_PARAMETER Provided parameter(s) are invalid.
//! \retval ::API_CRC_STATUS_NOT_SUPPORTED Function call is not supported.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_reset"];
//!
//! CALLER=>FU[label = "api_crc_reset(&device, &state)"];
//!
//! FU=>FU[label = "(device == NULL) || (state == NULL)"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_INVALID_PARAMETER", linecolor = "red"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_SUCCESS", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! For example, consider the preparation of parameters for calculating CRC32 with
//! the generator polynomial CRC32 \f$G(x)=x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11}+x^{10}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1\f$,
//! - polynomial = 0x82608EDB;
//! - inputXOR = 0xFFFFFFFF;
//! - outputXOR = 0x00000000;
//! - Bits Order: MSB
//! - inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
//! - outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
//! \.
//! \code{.cpp}
//! // state = 0x00
//! api_crc_state_t state = 0UL;
//! ...
//!
//! // state = 0xFFFFFFFF (state = device.inputXOR)
//! if (api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//!
//! \pre
//! Nothing.
//!
//! \post
//! Nothing.
//!
//! \note
//! according to the process flow, this function should be called in no case befor the function ::api_crc_finalize.
//!
//! \warning
//! Nothing.
//!
//------------------------------------------------------------------------------
api_crc_status_e api_crc_reset(api_crc_device_s const *const device, api_crc_state_t *const state)
{
    if((device == NULL) || (state == NULL))
    {
        return API_CRC_STATUS_INVALID_PARAMETER;
    }

    *state = device->inputXOR;
    return API_CRC_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
//!
//! \brief
//! The method calculates the CRC proof sum.
//!
//! \details
//! Before starting calculations a decision on the algorithm (Directly LFSR or LUT)
//!
//! \param[in] *device ::api_crc_device_s Specification of Cyclic Redundancy Code for given parameters.
//!
//! \param[in,out] *state ::api_crc_state_t State register for storing intermediate as well as the final value of the
//! Cyclic Redundancy Code (CRC) checksum calculation.
//!
//! \param[in] *buffer pointer to the data processing of the type <tt>*uint8_t</tt>
//!
//! \param[in] offset A variable of type <tt>uint8_t</tt>
//!
//! \param[in] size The size of the buffer in Bits.
//! The function does not control the buffer size. It can lead to the index overflows!
//!
//! \retval ::API_CRC_STATUS_SUCCESS Exclusive access has been granted.
//! \retval ::API_CRC_STATUS_INVALID_PARAMETER Provided parameter(s) are invalid.
//! \retval ::API_CRC_STATUS_NOT_SUPPORTED Function call is not supported.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_process"];
//!
//! CALLER=>FU[label = "api_crc_process(&device,&state,buffer,offset,size)"];
//!
//! FU=>FU[label = "(device == NULL) || (state == NULL) || (buffer == NULL) || (offset >= 8U).."];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_INVALID_PARAMETER", linecolor = "red"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_SUCCESS", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! For example one, let's Directly LFSR process CRC-16 with the generator polynomial
//! CRC-16 \f$G(x)=x^{16}+x^{15}+x^{2}+1\f$, in binary representation: 0b11000000000000101.
//! the arguments for the CRC-16:
//! - <tt>polynomial = 0xC002;</tt>
//! - <tt>inputXOR = 0x00;</tt>
//! - <tt>outputXOR = 0x00;</tt>
//! - Bits Order: LSB
//! - <tt>inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;</tt>
//! - <tt>outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;</tt>
//! - for Directly LFSR process has to <tt>table = 0;</tt>
//! - <tt>buffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};</tt>
//! - CRC-Rest = 0xBB3D;
//! \.
//! \code{.cpp}
//! api_crc_device_s device;
//! api_crc_state_t state = 0UL;
//! uint8_t buffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
//! uint32_t size = sizeof(buffer) << 3;
//! uint8_t offset = 0U;
//!
//! // for Directly LFSR process has to table = 0
//! if (api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // (state = device.inputXOR)
//! if (api_crc_reset(device, state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! if (api_crc_process(&device, &state, buffer, offset, size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // state = 0xBB3D
//! if (api_crc_status_e api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//! For example two, let's LUT process CRC32 with the generator polynomial
//! the generator polynomial CRC32 \f$G(x)=x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11}+x^{10}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1\f$,
//! - <tt>polynomial = 0x82608EDB;</tt>
//! - <tt>inputXOR = 0x00000000;</tt>
//! - <tt>outputXOR = 0x00000000;</tt>
//! - Bits Order: MSB
//! - <tt>inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;</tt>
//! - <tt>outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;</tt>
//! - for Directly LFSR process has to <tt>table = 0;</tt>
//! - <tt>buffer[] = {  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };</tt>
//! - CRC-Rest = 0x24a56cf5;
//! \.
//! \code{.cpp}
//! api_crc_device_s device;
//! api_crc_state_t state = 0UL;
//! uint8_t buffer = {
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
//! };
//! uint32_t buffer_size = sizeof(buffer) << 3;
//! uint8_t offset = 0U;
//! uint16_t table_size = api_crc_sizeofTable(polynomial);
//! uint32_t table32[256];
//!
//! if (api_crc_table(&device, (uint64_t*) table32, table_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // LUT process
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table32, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // (state = device.inputXOR)
//! if (api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! if (api_crc_process(&device, &state, buffer, offset, buffer_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // state = 0x24a56cf5
//! if (api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! ...
//! \endcode
//!
//! \pre
//! - For the first calculation requires calling the functions ::api_crc_init and ::api_crc_reset.
//! - For each next calculation with the same Init merely requires only the calling function ::api_crc_reset.
//! \.
//!
//! \post
//! After expiration, the function should be called ::api_crc_finalize to get the correct CRC values.
//!
//! \note
//! The function does not control the buffer size. It can lead to the index overflows!
//!
//! \warning
//! The function does not control the buffer size. It can lead to the index overflows!
//!
//------------------------------------------------------------------------------
api_crc_status_e api_crc_process(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const *const buffer, uint8_t const offset, uint32_t const size)
{
    void (*api_crc_processBit)(api_crc_device_s const *const, api_crc_register_t *const, uint8_t const *const, uint8_t const, uint32_t const) = 0U;
    void (*api_crc_processLUT)(api_crc_device_s const *const, api_crc_register_t *const, api_crc_register_t (*)(uint64_t const *const, uint8_t const), uint8_t const *const, uint32_t const) = 0U;
    api_crc_register_t (*api_crc_readTable)(uint64_t const *const, uint8_t const) = 0U;
    api_crc_register_t reg = 0U;
    uint8_t order = 0U;

    if((device == NULL) || (state == NULL) || (buffer == NULL) || (offset >= 8U)
            || (((uint64_t)((void *)((uint64_t)buffer + (((uint64_t)offset + (uint64_t)size + 7ULL) >> 3U)))) < (uint64_t)buffer))
    {
        return API_CRC_STATUS_INVALID_PARAMETER;
    }
//    printf("check of parameters %llu and %llu\n", (unsigned long long) ((uint64_t)((uint64_t)buffer + ((uint64_t)size >> 3U))), (unsigned long long) (uint64_t) buffer);
//    printf("check of parameters %llu\n", (unsigned long long) size);

    if(device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
//        printf("processReverse\n");
        api_crc_processBit = api_crc_processReverseBit;
        api_crc_processLUT = api_crc_processReverseLUT;
        reg = (*state);
        order = (sizeof(device->polynomial) << 3U) - __builtin_clzll(device->polynomial);

        if(order <= 8U)
        {
            api_crc_readTable = api_crc_readReverseTable08;
        }
        else if(order <= 16U)
        {
            api_crc_readTable = api_crc_readReverseTable16;
        }
        else if(order <= 32U)
        {
            api_crc_readTable = api_crc_readReverseTable32;
        }
#ifdef PLATFORM_X86_64
        else if(order <= 64U)
        {
            api_crc_readTable = api_crc_readReverseTable64;
        }
#endif
        else
        {
            // Not possible as the biggest possible register type consists of at
            // most 64 bits.
        }
    }
    else
    {
//        printf("processForward\n");
        api_crc_processBit = api_crc_processForwardBit;
        api_crc_processLUT = api_crc_processForwardLUT;
        reg = (*state) >> ((sizeof(api_crc_state_t) - sizeof(api_crc_register_t)) << 3);
        order = (sizeof(device->polynomial) << 3U) - __builtin_ctzll(device->polynomial);

        if(order <= 8U)
        {
            api_crc_readTable = api_crc_readForwardTable08;
        }
        else if(order <= 16U)
        {
            api_crc_readTable = api_crc_readForwardTable16;
        }
        else if(order <= 32U)
        {
            api_crc_readTable = api_crc_readForwardTable32;
        }
#ifdef PLATFORM_X86_64
        else if(order <= 64U)
        {
            api_crc_readTable = api_crc_readForwardTable64;
        }
#endif
        else
        {
            // Not possible as the biggest possible register type consists of at
            // most 64 bits.
        }
    }

    if(device->table == NULL)
    {
        api_crc_processBit(device, &reg, buffer, offset, size);
    }
    else
    {
        api_crc_processBit(device, &reg, buffer, offset, (offset == 0U) ? (0U) : (((8U - offset) < size) ? (8U - offset) : (size)));
        api_crc_processLUT(device, &reg, api_crc_readTable, (offset == 0U) ? (&buffer[0U]) : (&buffer[1U]), (size - ((offset == 0U) ? (0U) : (((8U - offset) < size) ? (8U - offset) : (size)))) >> 3U);
        api_crc_processBit(device, &reg, &buffer[((uint64_t)offset + (uint64_t)size) >> 3U], 0U, ((((uint64_t)offset + (uint64_t)size) >> 3U) == 0U) ? (0U) : (((uint64_t)offset + (uint64_t)size) & 7U));
    }

    if(device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
        (*state) = (api_crc_state_t)reg;
    }
    else
    {
        (*state) = (api_crc_state_t)reg << ((sizeof(api_crc_state_t) - sizeof(api_crc_register_t)) << 3);
    }

    return API_CRC_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
//!
//! \brief
//! This method returns the requested value and taking order predetermined values
//! outputOrder, outputOrder and the value outputXOR.
//!
//! \details
//! The function was, depending on which method (Direct or Reverse) processed,
//! returns the correct value of the state variable (register).
//!
//! \param[in] *device ::api_crc_device_s Specification of Cyclic Redundancy Code for given parameters.
//!
//! \param[in,out] *state ::api_crc_state_t State register for storing intermediate as well as the final value of the
//! Cyclic Redundancy Code (CRC) checksum calculation.
//!
//! \retval ::API_CRC_STATUS_SUCCESS Exclusive access has been granted.
//! \retval ::API_CRC_STATUS_INVALID_PARAMETER Provided parameter(s) are invalid.
//! \retval ::API_CRC_STATUS_NOT_SUPPORTED Function call is not supported.
//!
//! \par Message Sequence Chart
//!
//! \msc
//! hscale = "1", arcgradient = "8";
//!
//! CALLER[label = "Caller"], FU[label = "api_crc_finalize"];
//!
//! CALLER=>FU[label = "api_crc_finalize(&device, &state)"];
//!
//! FU=>FU[label = "(device == NULL) || (state == NULL)"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_INVALID_PARAMETER", linecolor = "red"];
//!
//! FU>>CALLER[label = "API_CRC_STATUS_SUCCESS", linecolor = "#379B7A"];
//! \endmsc
//!
//! \par Example
//! For example, let's LUT process CRC32 with the generator polynomial
//! the generator polynomial CRC32 \f$G(x)=x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11}+x^{10}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1\f$,
//! - <tt>polynomial = 0x82608EDB;</tt>
//! - <tt>inputXOR = 0x00000000;</tt>
//! - <tt>outputXOR = 0x00000000;</tt>
//! - Bits Order: MSB
//! - <tt>inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;</tt>
//! - <tt>outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;</tt>
//! - for Directly LFSR process has to <tt>table = 0</tt>
//! - <tt>buffer[] = {  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };</tt>
//! - CRC-Rest = 0x24a56cf5;
//! \.
//! \code{.cpp}
//! api_crc_device_s device;
//! api_crc_state_t state = 0UL;
//! uint8_t buffer = {
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//!     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
//! };
//! uint32_t buffer_size = sizeof(buffer) << 3;
//! uint8_t offset = 0U;
//! uint16_t table_size = api_crc_sizeofTable(polynomial);
//! uint32_t table32[256];
//!
//! if (api_crc_table(&device, (uint64_t*) table32, table_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // LUT process
//! if (api_crc_init(polynomial, inputXOR, outputXOR, table32, inputOrder, outputOrder, &device) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // (state = device.inputXOR)
//! if (api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! if (api_crc_process(&device, &state, buffer, offset, buffer_size) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//!
//! // before the start of api_crc_finalize was state = 0x24a56cf500000000
//! if (api_crc_status_e api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS)
//! {
//!    // Error handling...
//!    return 1;
//! }
//! // after the start of api_crc_finalize was state = 0x24a56cf5
//! ...
//! \endcode
//!
//! \pre
//! The method also needs the valid values for its arguments for a correct call.
//! Therefore, the functions should be called ::api_crc_init, ::api_crc_reset and ::api_crc_process before.
//!
//! \post
//! Nothing.
//!
//! \note
//! Nothing.
//!
//! \warning
//! Nothing.
//!
//------------------------------------------------------------------------------
api_crc_status_e api_crc_finalize(api_crc_device_s const *const device, api_crc_state_t *const state)
{
    uint8_t order = 0U;

    if((device == NULL) || (state == NULL))
    {
        return API_CRC_STATUS_INVALID_PARAMETER;
    }

    (*state) ^= device->outputXOR;

    if(device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
        if(device->outputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
        {
            order = 0U;
        }
        else
        {
            (*state) = api_crc_reverse64(*state);
            order = __builtin_ctzll(device->polynomial);
        }
    }
    else
    {
        if(device->outputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
        {
            (*state) = api_crc_reverse64(*state);
            order = 0U;
        }
        else
        {
            order = __builtin_ctzll(device->polynomial);
        }
    }

    (*state) >>= order;
    return API_CRC_STATUS_SUCCESS;
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
