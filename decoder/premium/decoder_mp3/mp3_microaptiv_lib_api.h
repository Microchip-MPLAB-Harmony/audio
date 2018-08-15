/*******************************************************************************
 MP3 Decoder Library Interface File

Company:
    Microchip Technology Inc.

  File Name:
    decoder_mp3_microaptiv.h

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
//DOM-IGNORE-END

#ifndef __DECODER_MP3_H
#define __DECODER_MP3_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif

// DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>




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
	uint16_t Kbps;
	uint16_t sampleRate;
	uint8_t bitsPerSample;
	uint8_t channels;
}
MP3_FORMAT;


/*******************************************************************************
  Function:
    uint32_t MP3DecoderGetStateSize(void);
	
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
uint32_t MP3DecoderGetStateSize ( void );

/*******************************************************************************
  Function:
    bool MP3Initialize(void *state);

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
    This function returns a boolean value.
    - 0 - indicates unsuccessful initialization
    - 1 - indicates successful initialization

  Example:
    <code>
    MP3Initialize ( mp3.decoderVars );
    </code>
	
*/
bool MP3Initialize ( void *state );

/*******************************************************************************
 Function:
    uint16_t MP3Decode ( void *state, uint8_t *in, uint16_t inSize, uint16_t *used,
                         MP3_FORMAT  *format, void *outBuffer, uint16_t *outSize);

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
    *format     - The pointer to a channel info structure is passed to the
                  function the decider updates this structure after every call
    *outBuffer  - The pointer to the output sample buffer where the decided pcm
                  samples are to be written,
    *outSize    - The pointer to a variable is passed to the function. After 
                  decoding the frame the function writes the number of valid
                  bytes available in the output buffer.

  Returns:
    This function returns either the value '0' or '1' to indicate the status of
    the decode operation.
	- 1		- indicates success
	- 0		- indicates failure or end of available encoded data. Upon receiving
			  the value '0' the application aborts processing the channel.

  Example:
    <code>
    MP3Decode ( mp3.decoderVars, input, inSize, read, &mp3.format, output, written )
    </code>
	
*/

uint16_t MP3Decode(void *state, uint8_t *in, uint16_t inSize, uint16_t *used,
        MP3_FORMAT *format, void *outBuffer, uint16_t *outSize);

#ifdef __cplusplus
}
#endif

#endif
