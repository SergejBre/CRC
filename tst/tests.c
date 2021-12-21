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
//! \if COND_API_LIBRARY_CRC_TESTS
//! \defgroup API_LIBRARY_CRC_TESTS Unit Tests
//! \ingroup API_LIBRARY_CRC
//!
//! \brief
//! Unit-Test Cases for the CRC Generic library
//!
//! \details
//! Unit test is used to verifiability of the code of all functions and interfaces.\n
//! The test concept
//! - Testing the interface of the CRC library
//! - Test of CRC-algorithms
//! - Performance tests
//! \.
//! For the details see \ref CRC_GENERIC_SECTION_RUNTESTS.
//!
//! \endif
//!
//------------------------------------------------------------------------------
//!
//! \cond COND_API_LIBRARY_CRC_TESTS
//!
//------------------------------------------------------------------------------
//!
//! \defgroup API_LIBRARY_CRC_TESTS Unit Tests
//! \ingroup API_LIBRARY_CRC
//! \{
//!
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "../inc/api_crc.h"

#include <stdio.h>

#include <time.h>


//------------------------------------------------------------------------------
// Outputs Functions
//------------------------------------------------------------------------------

int crc_processing(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const *const buffer, uint8_t const offset, uint32_t const buffer_size);
void output_Check(const uint64_t crc_Value, api_crc_state_t *const state);

//------------------------------------------------------------------------------
//!
//! \brief
//! Unit-Test Cases for the CRC Generic library
//!
//! \details
//! The test consists of three test blocks.
//! 1. block tests the CRC library interface.
//! The function arguments of public functions are tested in accordance with the validity
//! and the value fields.
//! 2. block is intended for testing of CRC algorithms.
//! Here the method Directly Forwad / Reverse LFSR and
//! LUT Forward / Reverse process in the separate tests cases are tested.
//! 3. block tests the performance of different CRC (Direcktly and LUT) method
//! based on the CRC32 polynomial.
//! \.
//!
//! \par Example
//! The tests can be run as follows:\n
//! \code{.sh}
//!  ~/crc cd tst
//!  ~/crc/tst$ ./CRC_generic_tests
//! \endcode
//!
//! \pre
//! The tests are also recommended as further examples on how to use
//! the CRC-Generic library. See \ref CRC_GENERIC_SECTION_BUILDPROCESS.
//!
//! \note
//! The tests are also recommended as further examples on how to use
//! the CRC-Generic library. See \ref CRC_GENERIC_SECTION_BUILDPROCESS.
//!
//------------------------------------------------------------------------------
int main(void)
{

    /******************************************************************************/
    /* Initialization Structures and Variables                                    */
    /******************************************************************************/
    uint64_t polynomial = 0xC002;
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
    uint64_t table[256] = {0U};
    uint8_t offset = 0U;
//    uint8_t *buffer = 0U;
    uint32_t buffer_size = 0U;
    uint64_t crc_Value = 0U;
    api_crc_status_e status = API_CRC_STATUS_SUCCESS;

    //------------------------------------------------------------------------------
    // Block 1.
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Test Case 1.1.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.1.                                                             */\n");
    printf("/* Review of api_crc_init(polynomial,inputXOR,outputXOR,table,inputOrder,...) */\n");
    printf("/******************************************************************************/\n");
    if ((api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) == API_CRC_STATUS_SUCCESS)
            & (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, outputOrder, 0U) == API_CRC_STATUS_INVALID_PARAMETER)
            // muss typedef uint32_t api_crc_register_t;
//            & (api_crc_init(0xFFFFFFFFFFFFFFFF, inputXOR, outputXOR, table, inputOrder, outputOrder, &device) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_init(0xC002, 0xFFFFF, outputXOR, table, inputOrder, outputOrder, &device) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_init(0xC002, inputXOR, 0xFFFFF, table, inputOrder, outputOrder, &device) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_init(polynomial, inputXOR, outputXOR, table, 2, outputOrder, &device) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_init(polynomial, inputXOR, outputXOR, table, inputOrder, 2, &device) == API_CRC_STATUS_INVALID_PARAMETER)
            )
    {
        printf("Status of api_crc_init(polynomial,inputXOR,outputXOR,table,inputOrder,...) is Ok\n\n\n");
    }
    else
    {
        printf("Status of api_crc_init(polynomial,inputXOR,outputXOR,table,inputOrder,...) is not Ok\n\n\n");
    }

    //------------------------------------------------------------------------------
    // Test Case 1.2.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.2.                                                             */\n");
    printf("/* The review for the function api_crc_table(device, table, size)             */\n");
    printf("/******************************************************************************/\n");
//    printf("Size of Table = %d\n", api_crc_sizeofTable(polynomial));
    uint16_t size = api_crc_sizeofTable(polynomial);
    if ((api_crc_table(&device, (uint64_t*) table, size) == API_CRC_STATUS_SUCCESS)
            & (api_crc_table(0U, (uint64_t*) table, size) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_table(&device, 0U, size) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_table(&device, (uint64_t*) table, 1024) == API_CRC_STATUS_INVALID_PARAMETER)
        )
     {
         printf("Status of api_crc_table(device, table, size) is Ok\n\n\n");
     }
     else
     {
         printf("Status of api_crc_table(device, table, size) is not Ok\n\n\n");
     }

    //------------------------------------------------------------------------------
    // Test Case 1.3.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.3.                                                             */\n");
    printf("/* The review for the function api_crc_sizeofTable(polynomial)                */\n");
    printf("/******************************************************************************/\n");
//    printf("Size of Table = %d\n", api_crc_sizeofTable(polynomial));
    if ((api_crc_sizeofTable(polynomial) == 512))
     {
         printf("Status of the function api_crc_sizeofTable(polynomial) is Ok\n\n\n");
     }
     else
     {
         printf("Status of the function api_crc_sizeofTable(polynomial) is not Ok\n\n\n");
     }

    //------------------------------------------------------------------------------
    // Test Case 1.4.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.4.                                                             */\n");
    printf("/* The review for the function  api_crc_reset(device, state)                  */\n");
    printf("/******************************************************************************/\n");
    if ((api_crc_reset(&device, &state) == API_CRC_STATUS_SUCCESS)
            & (api_crc_reset(0U, &state) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_reset(&device, 0U) == API_CRC_STATUS_INVALID_PARAMETER)
       )
    {
        printf("Status of api_crc_reset(&device, &state) is Ok\n\n\n");
    }
    else
    {
        printf("Status of api_crc_reset(&device, &state) is not Ok\n\n\n");
    }

    //------------------------------------------------------------------------------
    // Test Case 1.5.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.5.                                                             */\n");
    printf("/* Review for function api_crc_process(device, state, buffer, offset, size)   */\n");
    printf("/******************************************************************************/\n");
    uint8_t bufferTest[] = {0x31};
    buffer_size = (sizeof(bufferTest)/sizeof(uint8_t))*8;
    printf("Size of buffer in Bits = %d\n", buffer_size);
    if ((api_crc_process(&device, &state, bufferTest, offset, buffer_size) == API_CRC_STATUS_SUCCESS)
            // Aufpassen! Indexüberlauf bringt keine Meldung.
//            & (api_crc_process(&device, &state, bufferTest, offset, buffer_size + 16) == API_CRC_STATUS_SUCCESS)
            & (api_crc_process(&device, &state, 0U, offset, 0UL) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_process(&device, &state, 0U, offset, 8) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_process(0U, &state, bufferTest, offset, buffer_size) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_process(&device, 0U, bufferTest, offset, buffer_size) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_process(&device, &state, bufferTest, -1, buffer_size) == API_CRC_STATUS_INVALID_PARAMETER)
       )
    {
        printf("Status of api_crc_process(device, state, buffer, offset, size) is Ok\n\n\n");
    }
    else
    {
        printf("Status of api_crc_process(device, state, buffer, offset, size) is not Ok\n\n\n");
    }

    //------------------------------------------------------------------------------
    // Test Case 1.6.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 1.6.                                                             */\n");
    printf("/* The review for the function api_crc_finalize(device, state)                */\n");
    printf("/******************************************************************************/\n");
    if ((api_crc_finalize(&device, &state) == API_CRC_STATUS_SUCCESS)
            & (api_crc_finalize(0U, &state) == API_CRC_STATUS_INVALID_PARAMETER)
            & (api_crc_finalize(&device, 0U) == API_CRC_STATUS_INVALID_PARAMETER)
       )
    {
        printf("Status of api_crc_finalize(&device, &state) is Ok\n\n\n");
    }
    else
    {
        printf("Status of api_crc_finalize(&device, &state) is not Ok\n\n\n");
    }

    //------------------------------------------------------------------------------
    // Block 2.
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Test Case 2.1.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.1.                                                             */\n");
    printf("/* CRC-16                                                                     */\n");
    printf("/* generator polynomial: G(x) = x^16 + x^15 + x^2 + 1                         */\n");
    printf("/* initial:              0b0000000000000000 (0x00)                            */\n");
    printf("/* outputXOR:            0b0000000000000000 (0x00)                            */\n");
    printf("/* Bits Order:           LSB                                                  */\n");
    printf("/* data:            0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39              */\n");
    printf("/* CRC-16:          0xBB3D                                                    */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0xC002;
    inputXOR = 0x00;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    uint8_t bufferCRC16[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    buffer_size = (sizeof(bufferCRC16)/sizeof(uint8_t))*8;
    crc_Value = 0xBB3D;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC16, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC checkS
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.2.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.2.                                                             */\n");
    printf("/* ### 1. G3 : CENELEC ###                                                    */\n");
    printf("/* generator polynomial: G(x) = x^5 + x^2 + 1                                 */\n");
    printf("/* initial:              0b11111 (0x1F)                                       */\n");
    printf("/* outputXOR:            0b11111 (0x1F)                                       */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            1010100100001010001111110001 (A9 0A 3F 1)                 */\n");
    printf("/* CRC8:            00111 (0x07)                                              */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x12;
    inputXOR = 0x1F;
    outputXOR = 0x1F;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC05[] = {0xA9, 0x0A, 0x3F, 0x10};
    buffer_size = 28;
    crc_Value = 0x07;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC05, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC checkS
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.3.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.3.                                                             */\n");
    printf("/* ### 2. G3 : FCC ###                                                        */\n");
    printf("/* generator polynomial: G(x) = x^8 + x^2 + x + 1                             */\n");
    printf("/* initial:              0b11111111 (0xFF)                                    */\n");
    printf("/* outputXOR:            0b11111111 (0xFF)                                    */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            1010100100000010000100111111111111111111111111110001000000*/\n");
    printf("/*                  (A9 02 13 FF FF FF 10 0)                                  */\n");
    printf("/* CRC8:            11110010 (0xF2)                                           */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x83;
    inputXOR = 0xFF;
    outputXOR = 0xFF;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC08[] = {0xA9, 0x02, 0x13, 0xFF, 0xFF, 0xFF, 0x10, 0x00};
    buffer_size = 58;
    crc_Value = 0xF2;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC08, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC check
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.4.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.4.                                                             */\n");
    printf("/* ### 3. Prime : CRC8 ###                                                    */\n");
    printf("/* generator polynomial: G(x) = x^8 + x^2 + x + 1                             */\n");
    printf("/* initial:              0b00000000 (0x00)                                    */\n");
    printf("/* outputXOR:            0b00000000 (0x00)                                    */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            010100011000011110010001011110000000011010000001011111011 */\n");
    printf("/*                  1000010010001                                             */\n");
    printf("/*                  (0x51 0x87 0x91 0x78 0x06 0x81 0x7D 0xC2 0x44)            */\n");
    printf("/* CRC8:            00101111 (0x2F)                                           */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x83;
    inputXOR = 0x00;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC08_[] = {0x51, 0x87, 0x91, 0x78, 0x06, 0x81, 0x7D, 0xC2, 0x44};
    buffer_size = 70;
    crc_Value = 0x2F;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC08_, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC check
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.5.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.5.                                                             */\n");
    printf("/* ### 4. Prime : CRC12 ###                                                   */\n");
    printf("/* generator polynomial: G(x) = x^12 + x^11 + x^3 + x^2 + x + 1               */\n");
    printf("/* initial:              0b000000000000 (0x00)                                */\n");
    printf("/* outputXOR:            0b000000000000 (0x00)                                */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            010100000110000000111000                                  */\n");
    printf("/*                  (0x50, 0x60, 0x38)                                        */\n");
    printf("/* CRC12:           010010101010 (0x4AA)                                      */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0xC07;
    inputXOR = 0x00;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC12[] = {0x50, 0x60, 0x38};
    buffer_size = (sizeof(bufferCRC12)/sizeof(uint8_t))*8;
    crc_Value = 0x4AA;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC12, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC check
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.6.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.6.                                                             */\n");
    printf("/* ### 5. Prime : CRC32 ###                                                   */\n");
    printf("/* generator polynom:  G(x)=x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1*/\n");
    printf("/* initial:              0b00000000000000000000000000000000 (0x00)            */\n");
    printf("/* outputXOR:            0b00000000000000000000000000000000 (0x00)            */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/*                  0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/*                  0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/*                  0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/*                  0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39         */\n");
    printf("/* CRC32:           0x24a56cf5                                                */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x82608EDB;
    inputXOR = 0x00;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC32[] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
    };
    buffer_size = (sizeof(bufferCRC32)/sizeof(uint8_t))*8;
    crc_Value = 0x24a56cf5;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC32, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC check
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Test Case 2.7.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 2.7.                                                             */\n");
    printf("/* ### SFN : CRC32 ###                                                        */\n");
    printf("/* generator polynom:  G(x)=x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1*/\n");
    printf("/* initial:              0b11111111111111111111111111111111 (0xFFFFFFFF)      */\n");
    printf("/* outputXOR:            0b00000000000000000000000000000000 (0x00)            */\n");
    printf("/* Bits Order:           MSB                                                  */\n");
    printf("/* data:            00000100000001010000011000000111 (04 05 06 07)            */\n");
    printf("/*                  00001000000010010000101000001011 (08 09 0A 0B)            */\n");
    printf("/*                  00001100000011010000111000001111 (0C 0D 0E 0F)            */\n");
    printf("/*                  00010000000100010001001000010011 (10 11 12 13)            */\n");
    printf("/*                  00010100000101010001011000010111 (14 15 16 17)            */\n");
    printf("/*                  00011000000110010001101000011011 (18 19 1A 1B)            */\n");
    printf("/*                  00011100000111010001111000011111 (1C 1D 1E 1F)            */\n");
    printf("/*                  00100000                         (20)                     */\n");
    printf("/* CRC32:           00010111101011000010011000001110 (0x17AC260E)             */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x82608EDB;
    inputXOR = 0xFFFFFFFF;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    uint8_t bufferCRC32_[] = {
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B,
        0x1C, 0x1D, 0x1E, 0x1F,
        0x20
    };
    buffer_size = (sizeof(bufferCRC32_)/sizeof(uint8_t))*8;
    crc_Value = 0x17AC260E;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }

    // CRC processing
    if (crc_processing(&device, &state, bufferCRC32_, offset, buffer_size) == 1)
    {
        return 1;
    }

    // CRC check
    output_Check(crc_Value, &state);

    //------------------------------------------------------------------------------
    // Block 3.
    //------------------------------------------------------------------------------
    uint32_t n, N = 1966080U;
    uint8_t ARR[N];

    for (n = 0U; n < N; n++)
    {
		ARR[n] = 0x30 + n%10;
	}

//    printf("Value of array[] in bytes %d\n", n);
    buffer_size = (n << 3);
    clock_t clockStart, clockEnd;
    double clockElapsedSeconds;

    //------------------------------------------------------------------------------
    // Test Case 3.1.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 3.1.                                                             */\n");
    printf("/* Performance Test for CRC-32 Forward LFSR                                   */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x82608EDB;
    inputXOR = 0xFFFFFFFF;
    outputXOR = 0x00000000;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    crc_Value = 0x57b403d1;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }
    printf("api_crc_init status code = %d\n", status);

    // init of the Register state
    if ((status = api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_reset status code = %d\n", status);
        return 1;
    }
    printf("api_crc_reset status code = %d\n", status);

    // CRC processing
    clockStart = clock();
    status = api_crc_process(&device, &state, ARR, offset, buffer_size);
    clockEnd = clock();
    if (status != API_CRC_STATUS_SUCCESS)
    {
        // Error handling...
        printf("api_crc_process status code = %d\n", status);
        return 1;
    }
    printf("api_crc_process status code = %d\n", status);

    // Finalisation
    if ((status = api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_finalize status code = %d\n", status);
        return 1;
    }
    printf("api_crc_finalize status code = %d\n", status);
    printf("CRC Rest = %llx\n", (unsigned long long) state);

    output_Check(crc_Value, &state);

    clockElapsedSeconds = ((double)(clockEnd - clockStart)) / CLOCKS_PER_SEC;
    printf("Processing for %d bits took %f seconds\n", buffer_size, clockElapsedSeconds);
    printf("The speed of Processing %.2f bits per millisecond\n\n\n", buffer_size/clockElapsedSeconds/1000);
#if 1
    //------------------------------------------------------------------------------
    // Test Case 3.2.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 3.2.                                                             */\n");
    printf("/* Performance Test for CRC-32 Reverse LFSR                                   */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x82608EDB;
    inputXOR = 0xFFFFFFFF;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT;
    crc_Value = 0x4C36D339;

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, 0U, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }
    printf("api_crc_init status code = %d\n", status);

    // init of the Register state
    if ((status = api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_reset status code = %d\n", status);
        return 1;
    }
    printf("api_crc_reset status code = %d\n", status);

    // CRC processing
    clockStart = clock();
    status = api_crc_process(&device, &state, ARR, offset, buffer_size);
    clockEnd = clock();
    if (status != API_CRC_STATUS_SUCCESS)
    {
        // Error handling...
        printf("api_crc_process status code = %d\n", status);
        return 1;
    }
    printf("api_crc_process status code = %d\n", status);

    // Finalisation
    if ((status = api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_finalize status code = %d\n", status);
        return 1;
    }
    printf("api_crc_finalize status code = %d\n", status);
    printf("CRC Rest = %llx\n", (unsigned long long) state);

    output_Check(crc_Value, &state);

    clockElapsedSeconds = ((double)(clockEnd - clockStart)) / CLOCKS_PER_SEC;
    printf("Processing for %d bits took %f seconds\n", buffer_size, clockElapsedSeconds);
    printf("The speed of Processing %.2f bits per millisecond\n\n\n", buffer_size/clockElapsedSeconds/1000);
#endif
    //------------------------------------------------------------------------------
    // Test Case 3.3.
    //------------------------------------------------------------------------------
    printf("/******************************************************************************/\n");
    printf("/* Test Case 3.3.                                                             */\n");
    printf("/* Performance Test for CRC-32 Forward LUT Process                            */\n");
    printf("/******************************************************************************/\n");
    polynomial = 0x82608EDB;
    inputXOR = 0xFFFFFFFF;
    outputXOR = 0x00;
    inputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    outputOrder = API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT;
    crc_Value = 0x57b403d1;
    uint32_t table32[256] = {0U};

    // init of the device
    if ((status = api_crc_init(polynomial, inputXOR, outputXOR, (uint64_t*) table32, inputOrder, outputOrder, &device)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_init status code = %d\n", status);
        return 1;
    }
    printf("api_crc_init status code = %d\n", status);

    // init of the table
    uint16_t table_size = api_crc_sizeofTable(polynomial);
    printf("table_size = %d\n", table_size);
    if ((status = api_crc_table(&device, (uint64_t*) table32, table_size)) != API_CRC_STATUS_SUCCESS)
    {
        // Error handling
        printf("api_crc_table status code = %d\n", status);
        return 1;
    }
    printf("api_crc_table status code = %d\n", status);

    // init of the Register state
    if ((status = api_crc_reset(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_reset status code = %d\n", status);
        return 1;
    }
    printf("api_crc_reset status code = %d\n", status);

    // CRC processing
    clockStart = clock();
    status = api_crc_process(&device, &state, ARR, offset, buffer_size);
    clockEnd = clock();
    if (status != API_CRC_STATUS_SUCCESS)
    {
        // Error handling...
        printf("api_crc_process status code = %d\n", status);
        return 1;
    }
    printf("api_crc_process status code = %d\n", status);

    // Finalisation
    if ((status = api_crc_finalize(&device, &state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_finalize status code = %d\n", status);
        return 1;
    }
    printf("api_crc_finalize status code = %d\n", status);
    printf("CRC Rest = %llx\n", (unsigned long long) state);

    output_Check(crc_Value, &state);

    clockElapsedSeconds = ((double)(clockEnd - clockStart)) / CLOCKS_PER_SEC;
    printf("Processing for %d bits took %f seconds\n", buffer_size, clockElapsedSeconds);
    printf("The speed of Processing %.2f bits per millisecond\n\n", buffer_size/clockElapsedSeconds/1000);

    return 0;
}


//------------------------------------------------------------------------------
//!
//! \brief
//! The function is used to print results of the CRC-process.
//!
//! \details
//! For details and parameters, see the function \ref api_crc_process
//!
//------------------------------------------------------------------------------
int crc_processing(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const *const buffer, uint8_t const offset, uint32_t const buffer_size)
{
    api_crc_status_e status = API_CRC_STATUS_SUCCESS;

    // init of the Register state
    if ((status = api_crc_reset(device, state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_reset status code = %d\n", status);
        return 1;
    }
    printf("api_crc_reset status code = %d\n", status);

    // Processing of the data
    printf("buffer_size in Bits = %d\n", buffer_size);
    if ((status = api_crc_process(device, state, buffer, offset, buffer_size) != API_CRC_STATUS_SUCCESS))
    {
        printf("api_crc_process status code = %d\n", status);
        return 1;
    }
    printf("api_crc_process status code = %d\n", status);
    printf("CRC Rest (state) = %llx\n", (unsigned long long) (*state));

    // Finalisation
    if ((status = api_crc_finalize(device, state) != API_CRC_STATUS_SUCCESS))
    {
        // Error handling...
        printf("api_crc_finalize status code = %d\n", status);
        return 1;
    }
    printf("api_crc_finalize status code = %d\n", status);
    printf("CRC Rest = %llx\n", (unsigned long long) (*state));

    return 0;
}


void output_Check(const uint64_t crc_Value, api_crc_state_t *const state)
{
    printf("/******************************************************************************/\n");
    printf("/* Output for Check function output_Check(crc_Value, state)                   */\n");
    printf("/******************************************************************************/\n");
    if (crc_Value == (*state))
    {
        printf("The review of the process was needed, because the checksums match: %llx = %llx\n\n", (unsigned long long) crc_Value, (unsigned long long) (*state));
    }
    else
    {
        printf("Unfortunately, the review of the procedure was not necessary, because the checksums no match: %llx != %llx\n\n", (unsigned long long) crc_Value, (unsigned long long) (*state));
    }
}


//------------------------------------------------------------------------------
//!
//! \} // API_LIBRARY_CRC_TESTS
//!
//------------------------------------------------------------------------------
//!
//! \endcond // COND_API_LIBRARY_CRC_TESTS
//! \endcond // COND_API_LIBRARY_CRC
//! \endcond // COND_API_LIBRARY
//! \endcond // COND_API
//!
//------------------------------------------------------------------------------
