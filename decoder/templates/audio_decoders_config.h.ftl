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
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
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
