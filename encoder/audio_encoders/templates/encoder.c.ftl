/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    encoder.c

  Summary:
    This file provides source code for encoder general
    interface functions.

  Description:
    This header file provides function implementation for
    encoder file. encoder interface file is supposed to wrap one encoder or
    multiple encoders to provide application an uniform encoder interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END
#ifndef _HAR_ENCODER_H
#define _HAR_ENCODER_H
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

<#if CONFIG_AUDIO_ENCODER_TYPE == "PCM">
#include "pcm/pcm_enc.h"
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "ADPCM">
#include "adpcm/adpcm_enc.h"
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "OPUS">
#include "opus/opus_enc.h"
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "SPEEX">
#include "speex/speex_enc.h"
</#if>

#include "encoder.h"
#include <string.h>
// Harmony Generated Headers
////////////////////////////////////////////////////



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

static EncoderState encoder_state;
static StreamInfo stream_info;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************


inline StreamInfo *encoder_get_stream_info()
{
    return &stream_info;
}
inline EncoderState *encoder_get_encoder_state()
{
    return &encoder_state;
}
bool encoder_init(EncoderType encoder_type)
{
    // initialize encoder state
    
    bool ret = false;
    encoder_state.encoder_type = encoder_type;
<#if CONFIG_AUDIO_ENCODER_TYPE == "PCM">
	ret = pcm_encoder_init();
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "ADPCM">
	ret = adpcm_encoder_init();
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "OPUS">
	ret = opus_encoder_init_2(stream_info.channel, stream_info.sample_rate);
</#if>
	return ret;
}
// customer wants to save it on storage eventually as an audio file
// this function will he helpful
bool encoder_set_stream_info(StreamInfo si) 
{
    memcpy((void*)&(stream_info), &si, sizeof(StreamInfo));
    return true;
}

bool encoder_stream_set_sample_rate(int sample_rate)
{
    stream_info.sample_rate = sample_rate;
    return true;
}

bool encoder_stream_set_btirate(int bps)
{
    stream_info.bps = bps;
    return true;
}
bool encoder_stream_set_bitdepth(int bit_depth)
{
    if(bit_depth == 8 || bit_depth==16|| bit_depth==24 || bit_depth==32)
        stream_info.bit_depth = bit_depth;
    else
        stream_info.bit_depth = 16; // give a regular one
    return true;
}
bool encoder_stream_set_channel(int channel)
{
    if(channel==1 || channel==2) // only support mono and stereo these two modes
    {
        stream_info.channel = channel;
        return true;
    }
    return false;
}

/*********** setup audio file information if customer wants to save
 *********** on storage ************/
bool encoder_set_file_info(FileInfo fi)
{
    memcpy((void*)&(encoder_state.file_info), &fi, sizeof(FileInfo));
    return true;
}

bool encoder_set_audio_data_size(int audio_size)
{
    encoder_state.file_info.audio_size = audio_size;
    return true;
}

/*******************************************************************************
  Function:
    bool encode_one_frame(void *pin,
     uint32_t insize,
     void *pout,
     uint32_t outsize)

  Remarks:
    See prototype in encoder.h.
 */
// encoder state should be passed in each encoder
// why? ::: encoder state provides stream info like channel number, bit-depth
// which in most cases are needed info for arbitrary specific encoder
// how? ::: how to pass this structure to encoder. 
//          1> pass in initialize function.
//                  cons: state info will last the whole life of encoder, hard 
//                  to reconfigure.
//                  pros: only need to do once.
//          2> pass in encode function
//                  pros and cons: opposite of 1>
//                  case: vbr, variable bitrate, very useful
//                  
bool encode_one_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize)
{
    bool ret = false;
<#if CONFIG_AUDIO_ENCODER_TYPE == "PCM">
	ret = pcm_encode_frame(pin, insize, pout, outsize);
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "ADPCM">
	ret = adpcm_encode_frame(pin, insize, pout, outsize);
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "OPUS">
	//Opus can ONLY encode frames of 2.5, 5, 10, 20, 40, or 60 ms
	// pin must be an opus frame
	ret = opus_encode_frame(pin, insize, pout, outsize);
</#if>

    return ret;
}

/*******************************************************************************
  Function:
    bool encoder_free()

  Remarks:
    See prototype in encoder.h.
 */
bool encoder_free()
{
    bool ret = true;
<#if CONFIG_AUDIO_ENCODER_TYPE == "PCM">
	ret = pcm_encoder_free();
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "ADPCM">
	ret = adpcm_encoder_free();
<#elseif CONFIG_AUDIO_ENCODER_TYPE == "OPUS">
	ret = opus_encoder_free();
</#if>
	return ret;
}
/* *****************************************************************************
 End of File
 */

#endif
