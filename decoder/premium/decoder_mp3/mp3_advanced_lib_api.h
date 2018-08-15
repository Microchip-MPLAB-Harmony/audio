/*******************************************************************************
 MP3 Decoder Library Interface File

Company:
    Microchip Technology Inc.

  File Name:
    decoder_mp3_advanced.h

  Summary:
    This file consists of the abstract function and input output buffer size
    declaration for decoding purpose

  Description:
    The header file consists of function declaration for the abstract functions 
    to invoke decoding . The header file also defines the size of input samples
    and i/p and o/p buffer.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014-2015 released Microchip Technology Inc.  All rights reserved.
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

#ifndef __DECODER_MP3_H
#define __DECODER_MP3_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif

// DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>



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


typedef struct
{
	int16_t bitsPerWord;
	int16_t channels;
	int32_t samplingRate;
	int32_t bitRate;
	int32_t MPEG_Type;
} MP3_CHANNEL_INFO;



/*******************************************************************************
  Function:
    int32_t MP3DecoderGetStateSize(void);
	
  Summary:
    This function allocates memory used for decoding.

  Description:
    Based on the return value, the application allocates the required memory for
    the MP3 Decoder state. This function is called only once before initializing
    the decoder state. If dynamic memory allocation (malloc) is not needed, it
    is not necessary to call this function and the MP3 Decoder state buffer will
    be statically defined at the compile time with the size of the MP3 decoder
    state structure.

  Precondition:
    None.
	
  Parameters:
    None.

  Returns:
    This function returns the size of the MP3 Decoder state structure in bytes.

  Example:
    <code>
    Word mp3StateSize;
    mp3StateSize = MP3DecoderGetStateSize();
    </code>
	
*/
int32_t MP3DecoderGetStateSize ( void );

/*******************************************************************************
  Function:
    void MP3DecoderInit(void *state);

  Summary:
    The function views the received pointer as a structure pointer and clears
    the entire structure.

  Description:
    The function views the received pointer as a structure pointer and clears
    the entire structure. Subsequently, it initializes the selective variable
    with the default values. The function is called only once before initiating
    the decoding process.

  Precondition:
    None.
	
  Parameters:
    *state      - The pointer to the allocated state memory is passed.

  Returns:
    None.

  Example:
    <code>
    MP3Initialize ( mp3.decoderVars );
    </code>
	
*/
void	MP3DecoderInit(void *State);


/*******************************************************************************
 Function:
    int16_t MP3Decode ( void *state, uint8_t *In, uint16_t inSize, uint16_t *used,
                         MP3_CHANNEL_INFO *format, void *outBuffer, uint16_t *outSize);

  Summary:
    This function is called once to decode one frame.

  Description:
    This function is called once to decode one frame. This function will be
    called in an infinite while loop until the end of the frame is detected or
    the decode function returns the value '0'. Unlike most speech applications
    where the encoded data for each frame is deterministic, this is not the case
    for audio applications. Therefore, the application provides a portion of
    input data (usually more than what is necessary for a single frame). The
    decoder parses the input and decodes it with a portion of the data. The
    decode function passes the information of how many bytes of data were used
    to the application so that the application saves the remaining data and
    presents to the decoder for the subsequent frame.

  Precondition:
    None.
	
  Parameters:
    *state      - The pointer to the state memory is passed.
    *in         - The pointer to the input buffer , where the encoded  bit
                  stream is available.
    inSize      - A variable of data type uint16_t  indicating the number of valid
                  bytes available in the input buffer.
    *used       - The pointer to a variable is passed to the function. After 
                  decoding the frame the function writes the number of bytes
                  consumed from the current frame decoder operation.
    *format     - The pointer to a channel info structure is passed to the function. The decoder 
                  updates this structure after every call. Additional member 'MPEG_Type' to 
                  indicate the MPEG version is added in this structure.
    *outBuffer  - The pointer to the output sample buffer where the decided pcm
                  samples are to be written,
    *outSize    - The pointer to a variable is passed to the function. After 
                  decoding the frame the function writes the number of valid
                  bytes available in the output buffer.

  Returns:
    This function returns either the value '0' or '1' or '2' to indicate the status of
    the decode operation.
	- 0		- indicates success
    - 1     - indicates that no data is available (thus marking end of file)
	- 2		- indicates failure or invalid or corrupted data

  Example:
    <code>
    MP3Decode ( mp3.decoderVars, input, inSize, read, &mp3.format, output, written )
    </code>
	
*/

int16_t MP3Decode ( void *state, uint8_t *In, uint16_t inSize, uint16_t *used,
                     MP3_CHANNEL_INFO *format, void *outBuffer, uint16_t *outSize);

#ifdef __cplusplus
}
#endif
	
#endif
