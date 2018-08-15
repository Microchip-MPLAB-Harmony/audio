/*******************************************************************************
MP3 Decoder Library Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    wma.h

  Summary:
    WMA Decoder support API.

  Description:
    This header file consists of support function declarations.

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

#ifndef WMA_H
#define	WMA_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END


#include <stdbool.h>
#include <stdint.h>

#include "system/fs/sys_fs.h"
#include "../support/wma_dec.h"
#define WMA_ERROR_COUNT_MAX 1
typedef enum
{
    WMA_GET_FRAME_SIZE,
    WMA_DECODE_FRAME,
}WMA_DECODER_STATES;

typedef void(*SetReadBytesReadFlagInAppData)(int32_t val, bool b);
typedef int32_t(*GetReadBytesInAppData)();

void WMA_Initialize(SYS_FS_HANDLE wmaFilehandle, uint32_t inputBufferSize);
void WMA_FreeMemory(void);
int32_t WMA_GetHeaderPacketOffset();
int32_t WMA_SamplingFrequency_Get(void);
int32_t WMA_BitRate_Get(void);
int16_t WMA_Decoder( uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written );
bool isWMAdecoder_enabled();
uint8_t WMA_GetChannels();
void WMA_RegisterAppCallback(SetReadBytesReadFlagInAppData fptr0, GetReadBytesInAppData fptr1);
#ifdef __cplusplus
}
#endif

#endif	/* WMA_H */

