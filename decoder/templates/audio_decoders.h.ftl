/*******************************************************************************
  Audio Decoder Middleware Header File

  Company:
    Microchip Technology Inc.

  File Name:
    audio_decoder.h

  Summary:
    Audio decoder abstracted middleware header file.

  Description:
    Audio decoder abstracted middleware header file.
    
  Remarks:
    This file is auto generated depends on decoders selected in MHC.
    
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
<#if CONFIG_USE_WAV_STREAMING == true>
#include "decoder/audio_decoders/wav/wav_dec.h"
</#if>
<#if CONFIG_USE_MP3_DECODER == true>
#include "decoder/premium/decoder_mp3/support/mp3_dec.h"
</#if>
<#if CONFIG_USE_AAC_DECODER == true>
#include "decoder/premium/decoder_aac/support/aac_dec.h"
</#if>
<#if CONFIG_USE_WMA_DECODER == true>
#include "decoder/premium/decoder_wma/support/wma_dec.h"
</#if>
<#if CONFIG_USE_OGG_SPEEX_DECODER == true>
#include "decoder/audio_decoders/speex/speex_dec.h"
</#if>
<#if CONFIG_USE_OGG_OPUS_DECODER == true>
#include "decoder/audio_decoders/opus/support/opus_dec.h"
</#if>
<#if CONFIG_USE_FLAC_DECODER == true>
#include "decoder/audio_decoders/flac/flac_dec.h"
</#if>
<#if CONFIG_USE_ADPCM_STREAMING == true>
#include "decoder/audio_decoders/adpcm/adpcm_dec.h"
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
