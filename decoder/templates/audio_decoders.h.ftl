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
    This file is auto generated depends on decoders selected in MCC.
    
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
<#if CONFIG_USE_WAV_STREAMING == true>
#include "audio/decoder/audio_decoders/wav/wav_dec.h"
</#if>
<#if CONFIG_USE_MP3_DECODER == true>
#include "audio/decoder/audio_decoders/mp3/mp3_dec.h"
</#if>
<#if CONFIG_USE_AAC_DECODER == true>
#include "audio/decoder/audio_decoders/aac/aac_dec.h"
</#if>
<#if CONFIG_USE_WMA_DECODER == true>
#include "audio/decoder/audio_decoders/wma/wma_dec.h"
</#if>
<#if CONFIG_USE_OGG_SPEEX_DECODER == true>
#include "audio/decoder/audio_decoders/speex/speex_dec.h"
</#if>
<#if CONFIG_USE_OGG_OPUS_DECODER == true>
#include "audio/decoder/audio_decoders/opus/opus_dec.h"
</#if>
<#if CONFIG_USE_FLAC_DECODER == true>
#include "audio/decoder/audio_decoders/flac/flac_dec.h"
</#if>
<#if CONFIG_USE_ADPCM_STREAMING == true>
#include "audio/decoder/audio_decoders/adpcm/adpcm_dec.h"
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
