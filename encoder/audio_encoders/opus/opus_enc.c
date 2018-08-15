/*******************************************************************************
  MPLAB Harmony Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    opus_enc.c

  Summary:
    opus encoder source file.

  Description:
    opus encoder wrapper file of opus prebuilt library.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.
*/

#include "opus_enc.h"
#include "opus/include/opus.h"
#include "encoder.h"
#include <string.h>


// *****************************************************************************
// *****************************************************************************
// Section: Variable Definitions
// *****************************************************************************
// *****************************************************************************

/*Opus Encoder Instance*/
static OpusEncoder *enc;


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
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************
/* Function:
    bool opus_encoder_init_2(int channel, int inputSampleRate)

   Summary:
    Initialize opus encoder.

   Description:
    This function takes streaming configurations, creates opus encoder instance.

   PreCondition:
    Make sure the heap size is large enough for opus encoder structure.

   Parameters:
    channel             - Channel number of audio data which needs to be encoded later. 
    inputSampleRate     - Sample rate of audio data which needs to be encoded later
    
   Returns:
    bool           - Return true if initializing is successful, 
                     otherwise return false.
    
   Example:
    None.
    
   Remarks:
    None.
*/
bool opus_encoder_init_2(int channel, int inputSampleRate)
{
	// create a opus encoder structure
    int size;
    int error;
    
    size = opus_encoder_get_size(channel);
    enc = (OpusEncoder *)malloc(size);
    error = opus_encoder_init(enc, inputSampleRate, channel, OPUS_APPLICATION_AUDIO);
    if(error == OPUS_OK)
        return true;
    return false;
}

// *****************************************************************************
/* Function:
    bool opus_encode_frame(void *pin, 
        uint32_t insize, 
        void *pout, 
        uint32_t *outsize)

   Summary:
    encode one opus frame.

   Description:
    This function takes input buffer in pcm data, encodes it to an opus frame.

   PreCondition:
    this input buffer size must be size of one opus frame.

   Parameters:
    pin            - Input signal (interleaved if 2 channels). 
    insize         - Input size. length is frame_size*channels*sizeof(opus_int16)
    pout           - (out) Output payload. This must contain storage for at least 
                     \a max_data_bytes.
    outsize        - Size of output buffer.
    
   Returns:
    bool           - Return true if encoding a frame is successful, 
                     otherwise return false.
    
   Example:
    None.
    
   Remarks:
    None.
*/

bool opus_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize)
{
    // call encode api from opus library
    *outsize = opus_encode(enc, pin, insize/4, pout, 1280);//OUTPUT_BUFFER_SIZE
    if(*outsize < 0) return false;
    return true;
}

// *****************************************************************************
/* Function:
    bool opus_encoder_free()

   Summary:
    Free opus encoder instance.

   Description:
    This function frees the opus encoder instance 
    which was created in opus_encoder_init_2 function

   PreCondition:
    Opus encoder must be initialized before calling this function.

   Parameters:
    None.
    
   Returns:
    bool           - Return true if free opus encoder is successful.s
                     otherwise return false.
    
   Example:
    None.
    
   Remarks:
    None.
*/
bool opus_encoder_free()
{
    if(enc != NULL) free((void *)enc);
    // full implementation to do later
    return true;
}


// *****************************************************************************
/* Function:
    int32_t ogg_opus_get_id_header(void* buffer, int channel, 
        int inputSampleRate, int preskip, int gain, int channelmap)

   Summary:
    Create Opus ID header for Ogg packet

   Description:
    This function takes audio streaming configurations, and uses them to create
    a opus ID packet. More detail in https://wiki.xiph.org/OggOpus#ID_Header

   PreCondition:
    None.

   Parameters:
    buffer          - Allocated Buffer for this Opus ID header packet.
    channel         - Channel number of audio data which needs to be encoded later. 
    inputSampleRate - Sample rate of audio data which needs to be encoded later.             
    preskip         - This is the number of samples (at 48 kHz) to discard from 
                      the decoder output when starting playback, 
                      and also the number to subtract from a page's granule 
                      position to calculate its PCM sample position.
    
   Returns:
    int32_t         - The length of ID header packet (in bytes) on success or 
                      zero on failure.
    
   Example:
    None.
    
   Remarks:
    None.
*/
int32_t ogg_opus_get_header_packets(void* buffer, int channel, 
        int inputSampleRate, int preskip, int gain, int channelmap)
{
    uint32_t ret = 0; // size of header
    sOpusHeader *idHeader = (sOpusHeader*) buffer;
    memcpy(idHeader->signature, "OpusHead", 8);
    ret += 8;
    idHeader->version = 1; // fixed
    ret++;
    idHeader->channels = channel;
    ret++;
    idHeader->input_sample_rate = inputSampleRate;
    ret+=4; // sample rate occupy 4 bytes
    idHeader->preskip = preskip;
    ret+=2;
    idHeader->gain = gain;
    ret+=2;
    idHeader->channel_mapping = channelmap;
    ret+=1;
    
    
    // Not Support Channel Mapping Table
    return ret;         
}

const HAR_ENCODER opusEncoderInst = {
	OPUS,
	opus_encoder_init_2,
	opus_encoder_free,
	opus_encode_frame
};
    

/* *****************************************************************************
 End of File
 */
