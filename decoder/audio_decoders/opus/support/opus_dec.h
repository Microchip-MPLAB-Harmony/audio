/*******************************************************************************
  Ogg-Opus Decoder Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    opus_dec.h

  Summary:
    Contains the Opus decoder specific defintions and function prototypes.
	
  Description:
    This file contains the opus decoder specific defintions and function
    prototypes.
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
 
#ifndef DECODER_OPUS_DEC_SUPPORT_H
#define DECODER_OPUS_DEC_SUPPORT_H

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
//DOM-IGNORE-END

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "system/fs/sys_fs.h"
#include "../../ogg/ogg.h"
#include "../include/opus.h"
/******************Ogg Container Structures ********/


typedef enum{
    OPUS_SUCCESS = 1,
    OPUS_READ_ERROR,
    OPUS_STREAM_ERROR,
    OPUS_BUFF_ERROR,
    OPUS_STREAM_END,
    OPUS_PLAYBACK_ERROR,
    OPUS_OUT_OF_MEM_ERROR,
    OPUS_DISK_ERROR,
    OPUS_GENERAL_ERROR
}OPUS_ERROR_MSG;


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



typedef struct  __attribute__((packed))
{
	int32_t	pageCapture;		// should be OGG_ID_MAGIC
	int8_t	struct_ver;         // version of the Ogg file format. Should be 0 (RFC3533)
	int8_t	headerFlags;		// an eOggHeaderFlags value
	int64_t	granulePos;         // stream dependent position info
	int32_t	streamNo;           // logical bit stream identifier
	int32_t	pageNo;             // page sequence number
	int32_t	pageCrc;            // CRC32 checksum of the page
    uint8_t	pageSegments;		// number of page segments to follow
	uint8_t	segmentTbl[255];	// actually segmentTbl[pageSegments]; contains the lace 
	                            // values for all segments in the page
}sOggPageHdr;	// header of an Ogg page, full segment info included

typedef struct
{
	int		pktBytes;		// how many bytes in this packet
	int		pktSeqNo;		// packet sequence number
}sOpusPktDcpt;	            // decoder data packet descriptor	

typedef struct
{
	sOggPageHdr	pageHdr;        // current page header
	int		segIx;			    // current packet segment index in the current page
	int		pktIx;			    // current packet index, 0 -> ...
	int		prevBytes;		    // previous value of the bytes in the encoded output buffer 
}sOpusStreamDcpt;		        // info needed by the stream at run-time

typedef struct
{
	int		processedPktNo;		// counter of processed packets
	int		currPktNo;		    // number of the currently received packet from the stream
	int		nInBytes;		    // bytes available in the input buffer	
}opusDecDcpt;


/*****************************MACROS*******************************************/
#define OPUS_INPUT_BUFFER_SIZE (1024*2)
#define OPUS_OUTPUT_BUFFER_SIZE (1024*7)
#define OPUS_MAX_FRAME_SIZE (960*6) // 120ms @ 48Khz

/*******************************************************************************
  Function:
    bool isOPUSdecoder_enabled()

  Summary:
    Checks if Opus decoder is enabled by MHC configuration.

  Description:
    The function checks if Opus decoder is enabled by MHC configuration.

  Precondition:
    None.
	
  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    bool opusEnabled = isOPUSdecoder_enabled ();
    </code>
	
*/
bool 		   isOPUSdecoder_enabled();

/*******************************************************************************
  Function:
    int32_t        OPUS_DiskRead(uint8_t *inBuff)

  Summary:
    Called once to read one Opus packet.

  Description:
    This function is called once to read one Opus packet.

  Precondition:
    None.
	
  Parameters:
    *inBuff       - The pointer to the input buffer, where the Opus encoded data 
                    are to be written.
                   

  Returns:
    This function returns the size of this Opus packet.

  Example:
    <code>
    int32_t bytesRead = OPUS_DiskRead(inputBuffer);
    </code>
	
*/
int32_t        OPUS_DiskRead(uint8_t *inBuff);


/*******************************************************************************
  Function:
    int32_t        OPUS_GetSamplingRate()

  Summary:
    Returns the sampling rate of Opus audio.

  Description:
    This function returns the sampling rate of this Opus file, return value is 
    always 48000, Opus playback sample rate should be 48000 as described in the 
    OggOpus spec.

  Precondition:
    None.
	
  Parameters:
    None.
                   
  Returns:
    The sampling rate of Opus audio.

  Example:
    <code>
    int32_t sampleRate = OPUS_GetSamplingRate();
    </code>
	
*/
int32_t        OPUS_GetSamplingRate();


/*******************************************************************************
  Function:
    uint8_t        OPUS_GetChannels()

  Summary:
    Returns the number of channels for this Opus audio.

  Description:
    This function returns the number of channels for this Opus audio.

  Precondition:
    None.
	
  Parameters:
    None.           

  Returns:
    * -1 - 1 channel, mono mode
    * -2 - 2 channels, stereo mode

  Example:
    <code>
    uint8_t channelNumber = OPUS_GetChannels();
    </code>
	
*/
uint8_t        OPUS_GetChannels();

/*******************************************************************************
  Function:
    OPUS_ERROR_MSG OPUS_Initialize(const SYS_FS_HANDLE opus_file_handler)

  Summary:
    Initializes the Opus decoder and creates an Opus decoder state structure.

  Description:
    This function initializes the Opus decoder and creates an Opus decoder 
    state structure.

  Precondition:
    None.
	
  Parameters:
    opus_file_handler  - The file handler of current Opus file which will be 
                         decoded.

  Returns:
    The status code of this function.

  Example:
    <code>
    OPUS_ERROR_MSG ret = OPUS_Initialize(fileHandler);
    </code>
	
*/
OPUS_ERROR_MSG OPUS_Initialize(const SYS_FS_HANDLE opus_file_handler);

/*******************************************************************************
  Function:
    OPUS_ERROR_MSG OPUS_Decoder(const uint8_t *input, uint16_t inSize, uint16_t *read, 
                              int16_t *output, uint16_t *written, uint16_t outSize)

  Summary:
    Called once to decode one Opus packet.

  Description:
    This function is called once to decode one packet, this function will be called 
    in an infinite while loop until the end of the packet is detected or the
    decode function returns failure value.

  Precondition:
    None.
	
  Parameters:
    *input   - The pointer to the input buffer, where the encoded bit stream is 
               available
    inSize   - A variable of data type uint16_t indicating the number of valid 
               bytes available in the input buffer
    *read    - The pointer to a variable is passed to the function. After decoding 
               the packet the function writes the number of bytes consumed from the 
               current frame decoder operation
    *output  - The pointer to the output sample buffer where the decided PCM 
               samples are to be written
    *written - The pointer to a variable is passed to the function. After decoding 
               the frame the function writes the number of valid bytes available 
               in the output buffer
    outSize  - A variable that indicates the size of output buffer, used for 
               avoiding buffer overwritten.

  Returns:
    The status code of this function.

  Example:
    <code>
    OPUS_ERROR_MSG ret = OPUS_Decoder(input, inSize, read, output, written, outSize);
    </code>
	
*/
OPUS_ERROR_MSG OPUS_Decoder(const uint8_t *input, uint16_t inSize, uint16_t *read, 
                              int16_t *output, uint16_t *written, uint16_t outSize);

//*******************************************************************************
/* Function:
    void OPUS_Cleanup()

  Summary:
    Frees the memory allocated by the Opus Decoder.

  Description:
    This function frees the memory that was allocated by the Opus Decoder and is 
	called when the Opus Decoder ends.

  Precondition:
    None.
	
  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    OPUS_Cleanup();
    </code>
	
*/
void OPUS_Cleanup();

#ifdef __cplusplus
}
#endif

#endif