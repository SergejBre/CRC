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
//! \cond COND_API_LIBRARY_CRC_TUTORIAL
//!
//------------------------------------------------------------------------------
//!
//! \if COND_API_LIBRARY_CRC_TUTORIAL
//! \defgroup API_LIBRARY_CRC_TUTORIAL_EXAMPLES01 Examples 01
//! \ingroup API_LIBRARY_CRC_TUTORIAL
//!
//! \brief
//! Examples for CRC-16 Reversed Straightforward LFSR Algorithm
//!
//! \details
//! Example 1 shows the calculation of the CRC checksum.
//! There is an algorithm CRC-16 reversed Straightforward LFSR used.
//!
//! \endif // COND_API_LIBRARY_CRC_TUTORIAL
//------------------------------------------------------------------------------
//!
//! \addtogroup API_LIBRARY_CRC_TUTORIAL_EXAMPLES01
//! \{
//!
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "examples.h"
#include <stdio.h>

//------------------------------------------------------------------------------
//!
//! \brief
//! Examples for CRC-16 Reversed Straightforward LFSR Algorithm
//!
//! \details
//! Example 1 shows the calculation of the CRC checksum.\n
//! For example, consider the preparation of initial parameters (arguments)
//! for calculating CRC with the generator polynomial CRC-16 \f$G(x)=x^{16}+x^{15}+x^{2}+1\f$,
//! in binary representation: 0b11000000000000101.
//! As an argument for the function is polynomial in the reversed reciprocal form:
//! 0b1100000000000010 (0xC002)
//! - polynomial = 0xC002;
//! - inputXOR = 0b0000000000000000 (= 0x00);
//! - outputXOR = 0b0000000000000000 (= 0x00);
//! - Bits Order: LSB
//! - inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
//! - outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
//! - for Directly (LFSR) process has to table = 0
//! \.
//!
//! \code{.cpp}
//! api_crc_device_s device;
//! // for each Directly (LFSR) process, the table reference must be null!
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
//!
//! \note
//! For the normal functioning of the example of the binary file examples01_in.bin should be present.
//!
//------------------------------------------------------------------------------
int main(void)
{
    /******************************************************************************/
    /* Initialization Structures and Variables                                    */
    /******************************************************************************/
    uint64_t polynomial = 0x00;
    uint64_t inputXOR = 0x00;
    uint64_t outputXOR = 0x00;
    api_crc_order_e inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    api_crc_order_e outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    api_crc_device_s device = {
        polynomial,
        inputXOR,
        outputXOR,
        0U,
        inputOrder,
        outputOrder
    };
    api_crc_state_t state = 0U;
    uint8_t offset = 0U;
    uint64_t crc_Value = 0U;
	uint32_t data_size = 0U;

    //------------------------------------------------------------------------------
    // Example 1.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Examples 1.                                                                */\n");
    printf("/* Examples for CRC-16 Reversed Straightforward LFSR Algorithm                */\n");
    printf("/******************************************************************************/\n");
    printf("\n");
    printf("/******************************************************************************/\n");
    printf("/* CRC-16                                                                     */\n");
    printf("/* generator polynomial: G(x) = x^16 + x^15 + x^2 + 1                         */\n");
    printf("/* initial:              0b0000000000000000 (0x00)                            */\n");
    printf("/* outputXOR:            0b0000000000000000 (0x00)                            */\n");
    printf("/* Bits Order:           LSB                                                  */\n");
    printf("/* data:                 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/* CRC-16 checksum       0xBB3D                                               */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0xC002;
    inputXOR = 0x00;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    uint8_t data[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
    data_size = (sizeof(data)/sizeof(uint8_t)) << 3;
    crc_Value = 0xBB3D;
    api_crc_status_e status = API_CRC_STATUS_SUCCESS;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling...
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }
    printf("api_crc_init status code = %d\n", status);

    if (crc_processing(&device, &state, data, offset, data_size) == 1)
    {
        printf("CRC Process failed.\n");
        return 1;
    }
    printf("api_crc_init status code = %d\n", status);

    output_Check(crc_Value, &state);

	return 0;
}

//------------------------------------------------------------------------------
//!
//! \} // API_LIBRARY_CRC_TUTORIAL_EXAMPLES01
//!
//------------------------------------------------------------------------------
//!
//! \endcond // COND_API_LIBRARY_CRC_TUTORIAL
//! \endcond // COND_API_LIBRARY_CRC
//! \endcond // COND_API_LIBRARY
//! \endcond // COND_API
//!
//------------------------------------------------------------------------------
