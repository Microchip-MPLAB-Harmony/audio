/*******************************************************************************
AAC Decoder Library Support File (Deprecated)

  Company:
    Microchip Technology Inc.

  File Name:
    aac.h

  Summary:
    AAC decoder interface header file.
	This file is deprecated, use aac_dec.h instead.

  Description:
    This header file describes the interface functions of AAC decoder.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.
Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).
You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef AAC_H
#define AAC_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

#include "../aac_lib_api.h" // aac library header file
#include "system/fs/sys_fs.h"
#include <stdbool.h>
#include <stdint.h>



typedef enum
{
    AAC_GET_FRAME_SIZE,
    AAC_DECODE_FRAME,
}AAC_DECODER_STATES;

typedef enum
{
            SAMPLING_FREQUENCY_IDX0=0,
            SAMPLING_FREQUENCY_IDX1,
            SAMPLING_FREQUENCY_IDX2,
            SAMPLING_FREQUENCY_IDX3,
            SAMPLING_FREQUENCY_IDX4,
            SAMPLING_FREQUENCY_IDX5,
            SAMPLING_FREQUENCY_IDX6,
            SAMPLING_FREQUENCY_IDX7,
            SAMPLING_FREQUENCY_IDX8,
            SAMPLING_FREQUENCY_IDX9,
            SAMPLING_FREQUENCY_IDX10,
            SAMPLING_FREQUENCY_IDX11,
            SAMPLING_FREQUENCY_IDX12,
            SAMPLING_FREQUENCY_IDX13,
            SAMPLING_FREQUENCY_IDX14,
            SAMPLING_FREQUENCY_IDX15,

}AAC_SAMPLING_FREQUENCY_INDEX;


#define AAC_ERROR_COUNT_MAX 1
typedef void (*SetReadBytesInAppData)(int32_t val);

bool AAC_Initialize(void *heap,uint16_t size,uint8_t *ptr, SYS_FS_HANDLE aacFilehandle);
int16_t AAC_Decoder( uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written );
int32_t AAC_GetSamplingFrequency(uint8_t *ptr);
bool isAACdecoder_enabled();
uint8_t AAC_GetChannels();
void AAC_RegisterDecoderEventHandlerCallback(SetReadBytesInAppData fptr);
#ifdef __cplusplus
}
#endif

#endif
