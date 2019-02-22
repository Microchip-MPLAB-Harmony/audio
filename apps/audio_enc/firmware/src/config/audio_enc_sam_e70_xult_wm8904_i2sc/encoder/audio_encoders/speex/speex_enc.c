/*******************************************************************************
  MPLAB Harmony Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    speex_enc.c

  Summary:
    speex encoder source file.

  Description:
    speex encoder wrapper file of speex prebuilt library.
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

#include "speex_enc.h"
#include "encoder.h"
#include "speex/include/speex/speex.h"
#include "speex/include/speex/speex_header.h"
#include "speex/include/speex/speex_stereo.h"
#include <string.h>


// *****************************************************************************
// *****************************************************************************
// Section: Variable Definitions
// *****************************************************************************
// *****************************************************************************


static SpeexBits bits;
static void *enc_state = NULL;
// expressed in samples, not bytes
static uint32_t g_frame_size = 0;
static int g_speex_codec_mode = SPEEX_MODEID_NB;
static uint8_t g_channel = 1;
//static StreamInfo g_si;

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
    bool speex_encoder_init_2(int channel, int inputSampleRate)

   Summary:
    Initialize speex encoder.

   Description:
    This function takes streaming configurations, creates speex encoder instance.

   PreCondition:
    Make sure the heap size is large enough for speex encoder structure.

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
bool speex_encoder_init_2(int channel, int inputSampleRate)
{
	// create a speex encoder structure
    const SpeexMode *mode;
    g_channel = channel;
    switch(inputSampleRate)
    {
        case 8000:
            mode = &speex_nb_mode;
            break;
            
        case 16000:
            mode = &speex_wb_mode;
            g_speex_codec_mode = SPEEX_MODEID_WB;
            break;
            /* Not Support for now
        case 32000:
            mode = &speex_uwb_mode;
            g_speex_codec_mode = SPEEX_MODEID_UWB;
            break;
             */
        default:
            // Sample Rate not support
            return false;
    }
    speex_bits_init(&bits);
    enc_state = speex_encoder_init(mode);

    speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,&g_frame_size);
    
    if(enc_state == NULL || g_frame_size == 0)
        return false;
    return true;
}

// *****************************************************************************
/* Function:
    bool speex_encode_frame(void *pin, 
        uint32_t insize, 
        void *pout, 
        uint32_t *outsize)

   Summary:
    encode one speex frame.

   Description:
    This function takes input buffer in pcm data, encodes it to an speex frame.

   PreCondition:
    this input buffer size must be size of one speex frame.

   Parameters:
    pin            - Input signal (interleaved if 2 channels). 
    insize         - Input size. length is frame_size*channels*sizeof(speex_int16)
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

bool speex_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize)
{
    // for every input frame, frame size is 20ms audio
    speex_bits_reset(&bits);
    if(g_channel == 2)
        speex_encode_stereo_int(pin, speex_encoder_get_framesize(), &bits);
    speex_encode_int(enc_state, pin, &bits);
    // assume pout size is same as insize
    *outsize = speex_bits_write(&bits, pout, insize); 
    if(*outsize < 0) return false;
    return true;
}

// *****************************************************************************
/* Function:
    bool speex_encoder_free()

   Summary:
    Free speex encoder instance.

   Description:
    This function frees the speex encoder instance 
    which was created in speex_encoder_init_2 function

   PreCondition:
    Opus encoder must be initialized before calling this function.

   Parameters:
    None.
    
   Returns:
    bool           - Return true if free speex encoder is successful.s
                     otherwise return false.
    
   Example:
    None.
    
   Remarks:
    None.
*/
bool speex_encoder_free()
{
    // free all resources
    speex_bits_destroy(&bits);
    if(enc_state != NULL)
        speex_encoder_destroy(enc_state);
    return true;
}

/* Function:
    uint32_t speex_encoder_get_framesize()

   Summary:
    Free speex encoder instance.

   Description:
    In practice, frame_size will correspond to 20 ms 
    when using 8, 16, or 32 kHz sampling rate.

   PreCondition:
    Speex encoder must be initialized before calling this function.

   Parameters:
    None.
    
   Returns:
    uint32_t         - Return the frame size used by the mode you are using
    
   Example:
    None.
    
   Remarks:
    None.
*/
uint32_t speex_encoder_get_framesize()
{
    if(g_frame_size != 0) return g_frame_size;
    // enc_state must not be null
    speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,&g_frame_size);
    return g_frame_size;
}

int speex_encoder_get_header_size()
{
    return sizeof(SpeexHeader);
}


// *****************************************************************************
/* Function:
    int32_t ogg_speex_get_id_header(void* buffer, int channel, 
        int inputSampleRate, int preskip, int gain, int channelmap)

   Summary:
    Create Opus ID header for Ogg packet

   Description:
    This function takes audio streaming configurations, and uses them to create
    a speex ID packet. More detail in https://wiki.xiph.org/OggOpus#ID_Header

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
int32_t ogg_speex_get_header_packet(void* buffer, int nb_channel, 
        int inputSampleRate, int bitrate, int frames_per_packet)
{
    uint32_t ret = 0; // size of header
    SpeexHeader *idHeader = (SpeexHeader*) buffer;
    memcpy(idHeader->speex_string, OGG_ID_SPEEX, SPEEX_STRING_LENGTH);
    memcpy(idHeader->speex_version, SPEEX_VERSION, sizeof(SPEEX_VERSION));
    idHeader->speex_version_id = SPEEX_VERSION_ID;
    idHeader->header_size = sizeof(SpeexHeader);
    idHeader->rate = inputSampleRate;
    idHeader->mode = g_speex_codec_mode;
    idHeader->mode_bitstream_version = 4; // current bitstream version 
    idHeader->nb_channels = nb_channel;
    
    idHeader->bitrate  = bitrate;
    
    idHeader->frame_size = speex_encoder_get_framesize();
    idHeader->vbr = 0; // support CBR
    
    idHeader->frames_per_packet = frames_per_packet;
    idHeader->extra_headers = 0;
    idHeader->reserved1 = 0;
    idHeader->reserved2 = 0;
    
    ret += sizeof(SpeexHeader);
    // Not Support Channel Mapping Table
    return ret;         
}
    
const HAR_ENCODER speexEncoderInst = {
	SPEEX,
	speex_encoder_init_2,
	speex_encoder_free,
	speex_encode_frame
};
/* *****************************************************************************
 End of File
 */
