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
//! \addtogroup API_LIBRARY_CRC_TUTORIAL
//! \{
//!
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "examples.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
//!
//! \brief
//! The function is used to print results of the CRC-residual checks.
//!
//! \param[in] crc_Value setpoint of the CRC-residual
//!
//! \param[in] *state actual value of the CRC-residual
//!
//!
//------------------------------------------------------------------------------
void output_Check(const uint64_t crc_Value, api_crc_state_t *const state)
{
    printf("/******************************************************************************/\n");
    printf("/* Output for Check function output_Check(crc_Value, state)                   */\n");
    printf("/******************************************************************************/\n");
    if (crc_Value == (*state))
    {
        printf("The review of the process was needed, because the checksums match: %llx = %llx\n\n\n", (unsigned long long) crc_Value, (unsigned long long) (*state));
    }
    else
    {
        printf("Unfortunately, the review of the procedure was not necessary, because the checksums no match: %llx != %llx\n\n\n", (unsigned long long) crc_Value, (unsigned long long) (*state));
    }
}

//------------------------------------------------------------------------------
//!
//! \brief
//! The function returns the highest degree of significant binary numbers.
//!
//! \details
//! The function calculates the higher potency of the binary representation of numbers.
//! The process is a Devision by 2^32, 2^16, etc. In this case, with the higher powers
//! (>= 32, then >= 16, >= 8, etc.) started.
//!
//! \param[in] value the binary number.
//!
//! \return order the highest degree of significant binary numbers.
//!
//------------------------------------------------------------------------------
uint8_t crc_Order(uint64_t value)
{
    uint8_t indx = 0x00;
    uint8_t order;
    uint32_t tmp;
    for (order = 32; order > 0; order /= 2)
    {
        if ((tmp = value/(1L << order)) > 0)
        {
            value = tmp;
            indx += order;
        }
    }
    indx++;

    return indx;
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

int crc_solver(api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const offset, char *const file)
{
    uint8_t *buffer = 0U;
    uint32_t buffer_size = 0U;

    FILE *infile;
    uint8_t v;
    infile = fopen(file, "rb");
    if (!infile)
    {
        printf("The file %s was not found.\n", file);
        return 1;
    }
    else
    {
		// counting the number of bytes in a file
        while(fscanf(infile, "%c", &v) == 1)
        {
            buffer_size++;
            if ( buffer_size >= MAX_BUFFER_SIZE )
            {
                printf("buffer overflow, buffer_size >= MAX_BUFFER_SIZE\n");
				fclose(infile);
                return 1;
            }
        }

        buffer = (uint8_t *) malloc(buffer_size + sizeof(uint8_t));
        if ( buffer == NULL )
        {
			printf("memory allocation for the buffer failed\n");
			fclose(infile);
			return 1;
		}
        printf("Created a buffer for %d values\n", buffer_size);

		fseek(infile, 0, SEEK_SET);
        if ( fread(buffer, sizeof(uint8_t), buffer_size, infile) != buffer_size )
        {
			printf("there was an error reading the file\n");
			fclose(infile);
			return 1;
		}

        fclose(infile);
    }

    // buffer size in bits
    buffer_size <<= 3;

    if (crc_processing(device, state, buffer, offset, buffer_size) == 1)
    {
        printf("CRC Process failed.\n");
        free(buffer);
        return 1;
    }

    free(buffer);

    return 0;
}

int crc_coder(uint64_t polynomial, api_crc_device_s const *const device, api_crc_state_t *const state, uint8_t const offset, char *const in_file, char *const out_file)
{
    uint8_t *buffer = 0U;
    uint32_t buffer_size = 0U;
    uint8_t orderByte = (crc_Order(polynomial) + 7) >> 3;
    uint8_t endoffset = 0U;
    if (crc_Order(polynomial) % 8 > 0)
    {
        endoffset = 8 - crc_Order(polynomial) % 8;
    }
    printf("endoffset = %d\n", endoffset);
    printf("orderByte = %d\n", orderByte);

    FILE *infile, *outfile;
    uint8_t v;
    uint32_t n = 0U;
    infile = fopen(in_file,"rb");
    if (!infile)
    {
        printf("The file %s was not found.\n", in_file);
        return 1;
    }
    else
    {
		// counting the number of bytes in a file
        while(fscanf(infile, "%c", &v) == 1)
        {
            buffer_size++;
            if (!(buffer_size < MAX_BUFFER_SIZE))
            {
                printf("buffer overflow, buffer_size >= MAX_BUFFER_SIZE");
				fclose(infile);
                return 1;
            }
        }
        fclose(infile);

        buffer = (uint8_t *) malloc(buffer_size + sizeof(uint8_t));
        if ( buffer == NULL )
        {
			printf("memory allocation for the buffer failed\n");
			fclose(infile);
			return 1;
		}

		fseek(infile, 0, SEEK_SET);
        if ( fread(buffer, sizeof(uint8_t), buffer_size, infile) != buffer_size )
        {
			printf("there was an error reading the file\n");
			fclose(infile);
			return 1;
		}

        fclose(infile);

        for (n = 0; n < buffer_size + orderByte; n++)
        {
            if (n >= buffer_size)
            {
                buffer[n] = 0U;
            }
            printf("%x ", buffer[n]);
        }
        printf("\n");
    }

    // buffer size in bits
    buffer_size *= 8;

    if (crc_processing(device, state, buffer, offset, buffer_size) == 1)
    {
        printf("CRC Process failed.\n");
        free(buffer);
        return 1;
    }

    // buffer size in bytes
    buffer_size = (buffer_size + 7U) >> 3;
    buffer_size += orderByte;

    if (device->inputOrder == API_CRC_ORDER_MOST_SIGNIFICANT_BIT_TO_LEAST_SIGNIFICANT_BIT)
    {
        (*state) <<= (endoffset);
        printf("state<< = %llx\n", (unsigned long long) (*state));
        for (n = 1; n <= orderByte; n++)
        {
            buffer[buffer_size - n] = (*state) & MASKBYTE;
            (*state) = (*state) >> 8;
        }
    }
    else if (device->inputOrder == API_CRC_ORDER_LEAST_SIGNIFICANT_BIT_TO_MOST_SIGNIFICANT_BIT)
    {
        for (n = orderByte; n > 0; n--)
        {
            buffer[buffer_size - n] = (*state) & MASKBYTE;
            (*state) = (*state) >> 8;
        }
    }
    else
    {
        printf("undefined significant bits order\n");
        return 1;
    }

    if(!(outfile = fopen(out_file,"wb")))
    {
        printf("The file %s can no open.\n", out_file);
        return 1;
    }
    else
    {
        fwrite(buffer, sizeof(uint8_t), buffer_size, outfile);
        fclose(outfile);
    }
    printf("new buffer: ");
    for (n = 0; n<buffer_size; n++)
    {
        printf("%x ", buffer[n]);
    }
    printf("\n");

    // buffer size in bits
    buffer_size *= 8;
    buffer_size -= endoffset;

    if (crc_processing(device, state, buffer, offset, buffer_size) == 1)
    {
        printf("CRC Process failed.\n");
        free(buffer);
        return 1;
    }

    free(buffer);

    return 0;
}

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
