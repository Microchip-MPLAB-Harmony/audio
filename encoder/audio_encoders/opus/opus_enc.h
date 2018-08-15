/*******************************************************************************
  MPLAB Harmony Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    opus_enc.h

  Summary:
    opus encoder source header.

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

#ifndef _OPUS_ENC_H    /* Guard against multiple inclusion */
#define _OPUS_ENC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */
    
typedef struct {
    char signature[8]; // Magic signature: "OpusHead"
    uint8_t version;   // Version number: 0x01 for this spec
    uint8_t channels; // Number of channels: 1..255
    uint16_t preskip; // This is the number of samples (at 48kHz) to discard from the decoder output when starting playback
    uint32_t input_sample_rate; // Original input sample rate in Hz, this is not the sample rate to use for playback of the encoded data
    uint16_t gain; // output gain in dB
    uint8_t channel_mapping; // This byte indicates the order and semantic meaning of various channels encoded in each Opus packet
    /* The rest is only used if channel_mapping != 0 */
    int8_t nb_streams; // This field indicates the total number of streams so the decoder can correctly parse the packed Opus packets inside the Ogg packet
    int8_t nb_coupled; // Describes the number of streams whose decoders should be configured to produce two channels
    unsigned char stream_map[255];
} sOpusHeader;
    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */
     
     // *****************************************************************************
    /* Function:
        bool opus_encoder_init_2(int channel, int inputSampleRate)

       Summary:
        Initialize opus encoder.

       Description:
        This functions takes streaming configurations, creates opus encoder instance.

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
    bool opus_encoder_init_2(int channel, int inputSampleRate);

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
    bool opus_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize);
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
    bool opus_encoder_free();
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PCM_ENC_H */

/* *****************************************************************************
 End of File
 */
