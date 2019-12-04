<#--
/*******************************************************************************
  Audio Decoder Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    audio_decoders_config.h.ftl

  Summary:
    Audio Decoders Freemarker Template File

  Description:

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->

/*** Audio Decoders Configuration ***/
<#if CONFIG_USE_WMA_DECODER == true>
#define WMA_DECODER_ENABLED
</#if>
<#if CONFIG_USE_MP3_DECODER == true>
#define MP3_DECODER_ENABLED
</#if>
<#if CONFIG_USE_AAC_DECODER == true>
#define AAC_DECODER_ENABLED
</#if>
<#if CONFIG_USE_OGG_SPEEX_DECODER == true>
#define OGG_SPEEX_DECODER_ENABLED
</#if>
<#if CONFIG_USE_OGG_OPUS_DECODER == true>
#define OGG_OPUS_DECODER_ENABLED
</#if>
<#if CONFIG_USE_WAV_STREAMING == true>
#define WAV_STREAMING_ENABLED
</#if>
<#if CONFIG_USE_ADPCM_STREAMING == true>
#define ADPCM_STREAMING_ENABLED
</#if>
<#if CONFIG_USE_FLAC_DECODER == true>
#define FLAC_DECODER_ENABLED
</#if>

/* Customize buffer size depends on device SRAM */
#define DECODER_MAX_INPUT_BUFFER_SIZE (1024*7)
#define DECODER_MAX_OUTPUT_BUFFER_SIZE (1024*7)

<#--
/*******************************************************************************
 End of File
*/
-->
