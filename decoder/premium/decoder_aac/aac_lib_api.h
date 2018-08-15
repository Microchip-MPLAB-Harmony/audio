/*******************************************************************************
AAC Decoder Library Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    decoder_aac.h

  Summary:
    This file consists of the abstract function and input output buffer size
    declaration for decoding purposes.

  Description:
    The header file consists of function declaration for the abstract functions 
    to invoke decoding . The header file also defines the size of input samples
    and i/p and o/p buffer.
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.

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


#ifndef __DECODER_AAC_H
#define __DECODER_AAC_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "stdint.h"

#define MAX_STACK           0x5000                 // in Bytes
#define	AAC_PROFILE         1
#define STACK               1

#define INPUT_BUF_SIZE        (6144*2/8)           // in Bytes
#define FRAME_SIZE            1024                 // in Samples
#define AAC_FRAME_HEADER_SIZE     7                    // in Bytes
#define AACDECODER_STATE_SIZE 11032                // in Bytes


// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************


typedef short INTPCM;

typedef struct
{
	int32_t	riff;
	int32_t	len;
	int32_t	wave;
	int32_t	fmt;
	int32_t	sublen;
	int16_t	mode;			// 1 for uncompressed PCM
	int16_t	num_chnls;		// 1 for mono and 2 for stereo
	int32_t	samp_freq;
	int32_t	bytes_per_sec;		// = bytes_per_sample * samp_freq
	int16_t	bytes_per_sample;	// = bits_per_sample/8 * num_channels : bytes per set of sample
	int16_t	bits_per_sample;	// = bits per word (left or right - not both)
	int32_t	data;
	int32_t	data_len;
}WAVE;


/*******************************************************************************
  Function:
    int32_t  AAC_Decode(void *DecoderState,uint8_t *inBufferptr,short *pTimeData, int16_t *outFlag)
	
  Summary:
    This function decodes AAC data.

  Description:
    This function is called once to decode one frame. This function will be
    called in an infinite while loop until the end of frame is detected or the
    decode function returns the value '1'.
	
  Precondition:
    None.

  Parameters:
    *DecoderState  - The pointer to the state memory is passed.

    *inBufferptr   - Pointer to the input buffer, where the encoded bit stream is
                     available. Based on the return value from
                     AACDecoder_GetFrameSize(), the application fetches the
                     needed bytes from the bit stream file and fills in this buffer.

    *pTimeData     - Pointer to the buffer where the output samples are to be stored

    *outFlag       - The pointer to a variable for output flag:
			         0 for no valid data (for initial few frames)
			         1 for valid data present (for all subsequent frames)
                     After decoding the frame, the function indicates whether
                     valid data is present in the output buffer.

    * restBytes    - Pointer to the variable that indicates how many bytes have not decoded.
                   - Normally the number is 0.
  Returns:
    This function returns either the value '0' or '1' to indicate the status of
    the decode operation.
	- 0		- indicates success
	- 1		- indicates failure or end of available encoded data. Upon receiving 
	          the value '1', the application aborts processing the channel.

  Example:
    <code>
    errorStatus = AAC_Decode(haacDecoderInfo, inBuffer, outBuf, &outFlag, &restbit);
	</code>
	
*/

int16_t AAC_Decode(void *DecoderState, uint8_t *inBufferptr, INTPCM *pTimeData,
			int16_t *outFlag, int32_t *restBytes);


/*******************************************************************************
  Function:
    int32_t  AACDecoder_GetStateSize(void)

  Summary:
    This function allocates memory for each channel being used.

  Description:
    Based on the return value, the application allocates the required memory for
    the AAC Decoder state. This function is called only once for each channel
    before the channel is initialized.

    If dynamic memory allocation (malloc) is not to be used, this function
    need not be called and the AAC Decoder state buffer shall be statically defined
    at compile time with the size of the AAC Decoder state structure (constant).

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    This function returns the size of the AAC Decoder state structure in bytes.

  Example:
    <code>
    int32_t aac_State_Size;
    aac_State_Size = AACDecoder_GetStateSize();
	</code>
*/

int32_t AACDecoder_GetStateSize(void);

/*******************************************************************************
  Function:
    int32_t AACDecoder_GetFrameSize(void *haacDecoderInfo, uint8_t *adts_ptr);

  Summary:
    Used to get the AAC Decoder frame size.

  Description:
    This function is called every time before calling the AAC decode function.
    The frame size is extracted from the relevant field of the ADTS header for
    returning and also the frame size (in bits) is updated in the channel state.

  Precondition:
    None.

  Parameters:
    *haacDecoderInfo     - The pointer to the ADTS Header is passed.
                           The application extracts the next available seven
                           bytes from the file and passes them as ADTS header.
    *adts_ptr            - The pointer to the state memory is passed.

  Returns:
    This function returns the size (in bytes) of the next frame to be decoded.

*/

int32_t AACDecoder_GetFrameSize(void *haacDecoderInfo, uint8_t *adts_ptr);

/*******************************************************************************
  Function:
    void AACDecoder_Init( void *haacDecoderInfo, uint8_t *adts_ptr);

  Summary:
   Views the received point and clears the structure.

  Description:
   This function is called only once for each channel before starting the
   decoding of the first frame. This function views the received pointer as
   structure pointer and clears the entire structure. Subsequently it initializes
   the selective variables with the default values. Also, the information on
   sampling frequency and number of channels are extracted from the header and
   are stored in the channel state structure.

   After returning from this function, the application resets the file pointer
   to origin as these seven bytes will be fetched again to extract the frame size
   through the function mentioned next.

  Precondition:
    None.

  Parameters:
    *haacDecoderInfo     - The pointer for the structure that stores decoder
                           information
    *adts_ptr            - pointer to AAC header structure

  Returns:
    None.

  Example:
    <code>
    int32_t aac_State_Size;
    AACDecoder_Init(haacDecoderInfo, appData.data);
	</code>
	
*/

void AACDecoder_Init(void *haacDecoderInfo,uint8_t *adts_ptr);

/*******************************************************************************
  Function:
    void AAC_WriteWavHeader(void *haacDecoderInfo, WAVE *wav);

  Summary:
    Generates the .wav file header, if applicable.

  Description:
    This function is called only if the output needs to be written into a .wav
    file. If the output is for play out or for writing into a .pcm file, this
    function shall not be called. In case of wave file generation, the
    application shall leave 44 bytes space free at the start of the file and
    begin writing decoded samples from the 45th byte location onwards. Once the
    entire data had been decoded, this function is called. The function generates
    the .wav file header using the information available in the state memory and
    fills the WaveBuf array. The application copies the content of this buffer
    into the file in the reserved place available at the start of the file.

  Precondition:
    None.
	
  Parameters:
    *haacDecoderInfo     - The pointer for the structure that stores decoder
                           information
    *adts_ptr            - pointer to WAVE header buffer

  Returns:
    None.

  Example:
    <code>
    AAC_WriteWavHeader(haacDecoderInfo, wav);
	</code>
	
*/

void AAC_WriteWavHeader(void *haacDecoderInfo, WAVE *wav);                       

/*******************************************************************************
  Function:
    int32_t  AACDecoder_InterleaveSamples (void *pState, int16_t *outBuf);

  Summary:
    This function interleaves left and right channel data.

  Description:
    For stereo audio, the left and right channels are interleaved here
    and stored in the same I/O buffer. For mono channels, nothing is done.

  Precondition:
    None.
	
  Parameters:
    *pState	- The pointer to the state memory is passed. The details about
              the number of channels, mono or stereo, is used.
    *outBuf	- Pointer to the I/O buffer.

  Returns:
    This function returns the number of samples in the I/O buffer:
    - 1024 - for mono
    - 2048 - for stereo

  Example:
    <code>
    outSize = AACDecoder_InterleaveSamples(outBuf, haacDecoderInfo);
	</code>
	
*/

int16_t AACDecoder_InterleaveSamples(INTPCM *pTimeCh0, void *state);     

#ifdef __cplusplus
}
#endif

#endif

