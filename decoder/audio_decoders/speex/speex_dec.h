/*******************************************************************************
 Speex Decoder Library Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    speex_dec.h

  Summary:
    This file is an abstraction layer of the Speex Library.

  Description:
    This file contains the Speex Decoder-specific definitions and function
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

#ifndef SPEEX_DEC_SUPPORT_H
#define	SPEEX_DEC_SUPPORT_H


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
//DOM-IGNORE-END


#include "../ogg/ogg.h"
#include "include/speex/speex.h"
    
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
    
#define	OGG_ID_SPEEX			"Speex   "	// The Speex packet ID
#define	SPEEX_STRING_LENGTH		8			// The size of the Speex string	

#define SPEEX_VERSION			"speex-1.2beta3" 	// The Speex version string
#define	SPEEX_VERSION_LENGTH		20			    // The size of the Speex version string	

#define	SPEEX_VERSION_ID		1			        // Version identifier

#define SPEEX_VENDOR_STR		"Encoded by Microchip Audio Library ver 1.0"	// comment in Ogg header
    
    
#define	SPX_CODEC_BUFF_MUL		6	// Values to adjust the average decoder buffer size 
#define	SPX_CODEC_BUFF_DIV		5	// 20% is sufficient

    
#define SPEEX_INPUT_BUFFER_SIZE 1024
#define SPEEX_OUTPUT_BUFFER_SIZE 1024*7   
    

typedef struct
{
	SpeexBits	spxBits;		// codec control structure
	void*		spxState;		// codec state
	int		inBuffSize;		    // input buffer size		
	int		outBuffSize;
	int		nOutBytes;		   // bytes available in the output buffer 
}spxCdcDcpt;	// Speex codec descriptor
		        // common part of encoder/decoder
		        // since we don't take the trouble to have a type to distinguish 
				// between descriptors, we need to have a common part


typedef struct
{
	spxCdcDcpt	cdc;			// common part
	int		framesPerPacket;	// frames per Ogg packet
	int		frameSize;		    // size of the frames
	int		processedPktNo;		// counter of processed packets
	int		currPktNo;		    // number of the currently received packet from the stream
	int		nInBytes;		    // bytes available in the input buffer	
	int		outFrameSize;
}spxDecDcpt;

    
typedef struct
{
	int		pktBytes;		// how many bytes in this packet
	int		pktSeqNo;		// packet sequence number
}sSpxPktDcpt;	            // decoder data packet descriptor	
    
//typedef enum
//{
//	OGG_FLAG_CONTINUATION	 = 0x1,	// continuation packet
//	OGG_FLAG_BOS		     = 0x2,	// first page of the logical stream
//	OGG_FLAG_EOS		     = 0x4,	// last page of the logical stream
//	
//}eOggHeaderFlags;	// Flags in the Ogg header

typedef enum
{
	SPX_FLAG_WB		    = 0x1,		// wide/narrow band
	SPX_FLAG_VBR		= 0x2,		// VBR
	SPX_FLAG_BRATE_OVR	= 0x4,		// bit rate setting overrides the encoder quality
	SPX_FLAG_PRCPT_ENH	= 0x8,		// decoder perceptual enhancement on/off
}eSpxFlags;		// Speex flags
// Note: at most 8 flags are supported right now! 

typedef struct  __attribute__((packed))
{
	int		streamNo;		// stream number inside the container
	int		streamVer;		// stream version
	unsigned short	frameSamples;		// Size of frames, in samples
	unsigned short	bitRate;		    // encoder bit rate 
	unsigned char	framesPerPacket;	// Number of frames stored per Ogg packet, <10
	unsigned char	packetsPerPage;		// number of packets in an Ogg page <= 255
	unsigned char	complexity;		    // encoder complexity 1-10
	unsigned char	qualFactor;		    // encoder quality factor 1-10
	char		    spxFlags;		    // eSpxFlags: run time flags, wideband, VBR
	char		    reserved[3];		// future expansion/padding
}sSpxRunDcpt;	// run-time Speex descriptor obtained from the stream with AudioStreamGetRunInfo()



//typedef enum
//{
//	OGG_ID_MAGIC	=  0x5367674F   //'SggO', OggS, the magic page capture
//
//}eOggId;
    
typedef struct  __attribute__((packed))
{
	int32_t	pageCapture;		// should be OGG_ID_MAGIC
	int8_t	struct_ver;         // version of the Ogg file format. Should be 0 (RFC3533)
	int8_t	headerFlags;		// an eOggHeaderFlags value
	int64_t	granulePos;         // stream dependent position info
	int32_t	streamNo;           // logical bit stream identifier
	int32_t	pageNo;             // page sequence number
	int32_t	pageCrc;            // CRC32 checksum of the page
    int8_t	pageSegments;		// number of page segments to follow
	uint8_t	segmentTbl[255];	// actually segmentTbl[pageSegments]; contains the lace 
	                            // values for all segments in the page
}sOggPageSegHdr;	// header of an Ogg page, full segment info included
    
typedef struct  __attribute__((packed))
{
	char	speexString[SPEEX_STRING_LENGTH];	// identify the Speex bit-stream: OGG_ID_SPEEX
	char	speexVer[SPEEX_VERSION_LENGTH];		// Speex version that encoded the file 
	int32_t	speexVerId;		       	// Speex version (for checking compatibility)
	int32_t	headerSize;				// sizeof(SpeexHeader)
	int32_t	sampleRate;				// Sampling rate used
	int32_t	wBand;					// 0 for narrowband, 1 for wideband
	int32_t	modeBitsStreamVer;		// Version ID of the bit-stream
	int32_t	nChannels;				// Number of channels encoded
	int32_t	bitRate;				// Bit-rate used
	int32_t	frameSamples;			// Size of frames, samples
	int32_t	vbr;					// 1 for a VBR encoding, 0 otherwise
	int32_t	framesPerPacket;		// Number of frames stored per Ogg packet
	int32_t	extraHeaders;			// Number of additional headers after the comments
	int32_t	reserved1;				// Reserved for future use, 0
	int32_t	reserved2;				// Reserved for future use, 0
}sSpeexHeader;		// the Speex header, the first page in the Speex stream


typedef struct
{
	sSpxRunDcpt	runDcpt;		// global info
	sOggPageSegHdr	pageHdr;    // current page header
	int		segIx;			    // current packet segment index in the current page
	int		pktIx;			    // current packet index, 0 -> ...
	int		prevBytes;		    // previous value of the bytes in the encoded output buffer 
}sSpxStreamDcpt;		        // info needed by the stream at run-time

typedef void(*pProgressFnc)(int nBytes);			// progress display function
typedef struct
{
	int		progressStep;		// no of bytes to process before calling the progress callback
	int		progressCnt;		// current counter
	pProgressFnc	progressFnc;		// progress callback
}progressDcpt;	// encoder/decoder progress activity descriptor

typedef enum{
    SPEEX_SUCCESS = 1,
    SPEEX_READ_ERROR,
    SPEEX_STREAM_ERROR,
    SPEEX_BUFF_ERROR,
    SPEEX_STREAM_END,
    SPEEX_PLAYBACK_ERROR,
    SPEEX_OUT_OF_MEM_ERROR,
    SPEEX_DISK_ERROR,
    SPEEX_GENERAL_ERROR
}SPEEX_ERROR_MSG;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

//*******************************************************************************
/* Function:
    SPEEX_ERROR_MSG SPEEX_Initialize(uintptr_t spx_file_handle);

  Summary:
    Reads the Speex header page and initializes the Speex Decoder state.

  Description:
    The function internally calls the Speex Codec Decoder initialization function.
    The function is called only once before initiating the decoding process.

  Precondition:
    Call SPEEX_Cleanup function before this function for safety.
	
  Parameters:
    - spx_file_handle       - Current file handle from application

  Returns:
    This function returns an enum value.
    - SPEEX_SUCCESS         - Indicates success
    - SPEEX_READ_ERROR      - Indicates read failure
    - SPEEX_STREAM_ERROR    - Indicates Ogg-Speex parsing error
    - SPEEX_STREAM_END      - Indicates end of the stream

  Example:
    <code>
    APP_ERROR_MSG res = SPEEX_Initialize(spx_file_handle);
    </code>
	
*/
SPEEX_ERROR_MSG SPEEX_Initialize(uintptr_t spx_file_handle);



//*******************************************************************************
/* Function:
    SPEEX_ERROR_MSG SPEEX_Decoder(uint8_t *input, uint16_t inSize, uint16_t *read, 
	                              int16_t *output, uint16_t *written);

  Summary:
    Called once to decode one packet.

  Description:
    This function is called once to decode one frame. This function will be
    called in an infinite while loop until the end of the frame is detected or
    the decode function returns failure value.

  Precondition:
    None.
	
  Parameters:
    *input      - The pointer to the input buffer , where the encoded bit
                  stream is available
    inSize      - A variable of data type uint16_t  indicating the number of valid
                  bytes available in the input buffer
    *read       - The pointer to a variable is passed to the function. After 
                  decoding the frame the function writes the number of bytes
                  consumed from the current frame decoder operation.
    *output     - The pointer to the output sample buffer where the decided PCM
                  samples are to be written,
    *written    - The pointer to a variable is passed to the function. After 
                  decoding the frame the function writes the number of valid
                  bytes available in the output buffer.

  Returns:
    This function returns an enum value.
    - SPEEX_SUCCESS         - Indicates success
    - SPEEX_READ_ERROR      - Indicates read failure
    - SPEEX_STREAM_ERROR    - Indicates Ogg-Speex parsing error
    - SPEEX_STREAM_END      - Indicates end of the stream

  Example:
    <code>
    res = SPEEX_Decoder (input,inSize,read,output,written);
    </code>
	
*/
SPEEX_ERROR_MSG SPEEX_Decoder(uint8_t *input, uint16_t inSize, uint16_t *read, 
                              int16_t *output, uint16_t *written);

//*******************************************************************************
/* Function:
    int32_t SPEEX_DiskRead(uint8_t *inBuff)

  Summary:
    Reads one packet of Ogg-Speex audio.

  Description:
    This function is called once to read one packet.

  Precondition:
    None.
	
  Parameters:
    *inBuff      - The pointer to the input buffer, where one packet data is to
                   to be written

  Returns:
    This function returns the size of reading bytes.

  Example:
    <code>
    appData.nBytesRead = SPEEX_DiskRead(ptr);
    </code>
	
*/
int32_t SPEEX_DiskRead(uint8_t *inBuff);

//*******************************************************************************
/* Function:
    void SPEEX_Cleanup()

  Summary:
    Frees the memory allocated by the Speex Decoder.

  Description:
    This function frees the memory that was allocated by the Speex Decoder and is 
	called when the Speex Decoder ends.

  Precondition:
    None.
	
  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SPEEX_Cleanup();
    </code>
	
*/
void SPEEX_Cleanup();

//*******************************************************************************
/* Function:
    bool isSPEEXdecoder_enabled()

  Summary:
    Indicates whether the Speex Decoder is enabled.

  Description:
    This function indicates whether the Speex Decoder is enabled.

  Precondition:
    None.
	
  Parameters:
    None.

  Returns:
    This function returns a Boolean value.
    - true		- Indicates that the Speex Decoder is enabled
    - false     - Indicates that the Speex Decoder is disabled
 
  Example:
    <code>
    appData.SPEEX_decoder_enabled   = isSPEEXdecoder_enabled();
    </code>
	
*/
bool isSPEEXdecoder_enabled();

//*******************************************************************************
/* Function:
    int32_t SPEEX_GetSamplingRate()

  Summary:
    Returns the sampling rate of Ogg-Speex audio.

  Description:
    This function returns the sampling rate of Ogg-Speex audio.

  Precondition:
    This function can only be called after the SPEEX_Initialize function.
	
  Parameters:
    None.

  Returns:
    This function returns the sampling rate.
  
  Example:
    <code>
    sampling_frequency = SPEEX_GetSamplingRate();
    </code>
	
*/
int32_t SPEEX_GetSamplingRate();

//*******************************************************************************
/* Function:
    int32_t SPEEX_GetBitrate()

  Summary:
    Returns the bit rate of Ogg-Speex audio.

  Description:
    This function returns the bit rate of Ogg-Speex audio.

  Precondition:
    This function can only be called after the SPEEX_Initialize function.
	
  Parameters:
    None.

  Returns:
    This function returns the bit rate.
     
  Example:
    <code>
    decoderBitrate = SPEEX_GetBitrate()/1000;
    </code>
	
*/
int32_t SPEEX_GetBitrate();

//*******************************************************************************
/* Function:
    uint8_t SPEEX_GetChannels()

  Summary:
    Returns channel number of Ogg-Speex audio.

  Description:
    This function returns the channel number of Ogg-Speex audio.

  Precondition:
    This function can only be called after the SPEEX_Initialize function.
	
  Parameters:
    None.

  Returns:
    This function returns channel number
     
  Example:
    <code>
    channelNumber = SPEEX_GetChannels();
    </code>
	
*/
uint8_t SPEEX_GetChannels();


#ifdef __cplusplus
}
#endif

#endif	/* SPEEX_DEC_SUPPORT_H */