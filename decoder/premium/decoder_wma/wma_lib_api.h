/*******************************************************************************
WMA Decoder Library Interface File

Company:
    Microchip Technology Inc.

  File Name:
    wma_lib_api.h

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
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.
Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement). In 
addition to Microchip license,Windows Media Component license is required
to use this software. You should refer to the license agreement accompanying 
this Software for additional information regarding your rights and obligations.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/


#define __DECODER_WMA_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

#include "stdint.h"
#include "stdio.h"
#include "../decoder_wma/support/wma_dec.h"


#define MAX_SAMPLES 2048                    //in Bytes
//#define WAVE_HEADER_SIZE 44                 //in Bytes
#define BYTES_PER_SAMPLE 2
#define WMA_MAX_INPUT_BUFFER_SIZE 1024*20   //in Bytes

typedef enum
{
    WMA_NO_ERR = 0,
    WMA_NEED_DATA = 1,
    WMA_ERR = 2
} WMA_Status;

typedef struct
{
    long SamplesPerSec;     //Sampling Frequency
    long Channels;          //No of Channels
    long BitRate;           //Bit Rate
    long Version;           //WMA Version
    long ByteUsed;          //Used Input Data
    int WMAState;           //Input or Decode State
    long PacketSize;        //Size of Packet

} Chnl_Info;

/*******************************************************************************
  Function:
    uint32_t WMA_GetStateSize(void);

  Summary:
    This function requires the required memory for the WMA Decoder state.

  Description:
    Based on the return value, the application allocates the required memory for
    the WMA Decoder state. This function is called only once for each channel
    before the channel is initialized. If dynamic memory allocation (malloc) is
    not needed, it is not necessary to call this function and the WMA Decoder
    state buffer will be statically defined at compile time with the size of the
    WMA Decoder state structure (constant).

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    This function returns the size of the WMA Decoder state structure in bytes.

  Example:
    <code>
    unsigned long nDecodedSamples;
    nDecodedSamples = WMA_GetStateSize();
    </code>

*/
uint32_t WMA_GetStateSize(void);

#ifndef WIN32
/*******************************************************************************
  Function:
    WMA_Status WMA_Decoder_Init (void* pState, Chnl_Info *chnl, char * WMADataBuf);

  Summary:
    This function reads the pointers and clears the structure.

  Description:
    This function views the received pointer as structure pointer and clears the
    entire structure. Subsequently, it initializes the selective variables with
    the default values. This function is called only once for each file before
    starting the decoding of an entire packet.

  Precondition:
    None.

  Parameters:
    pState      - The pointer to the allocated state memory.
    *chnl       - Holds the essential audio file information and state of the file
    WMADataBuf  - The pointer to the input buffer

  Returns:
    This function returns the error result.

  Example:
    <code>
    void *WMA_State;
    Chnl_Info chnlInfo;
    WMA_Status WMA_status;
    char WMA_InBuf[MAX_INPUT_BUF];

    WMA_Status = WMA_Decoder_Init (WMA_State, &chnlInfo, WMA_InBuf);
    </code>

*/
WMA_Status WMA_Decoder_Init (void* pState, Chnl_Info *chnl, char * WMADataBuf);
/*******************************************************************************
  Function:
    void WMA_WaveHdr(char *WavHdr, void *hWMA, uint32_t DataLen);

  Summary:
    Generates the .wav file header and fills the WavBuf array.

  Description:
    For wave file generation, the application will leave 44 bytes of free space
    at the beginning of the file and begins writing decoded samples from the 45th
    byte location onwards. Once the entire data had been decoded, this function
    is called. The function generates the .wav file header and fills the WavBuf
    array. The application copies the contents of this buffer into the reserved
    place available at the beginning of the file.

  Precondition:
     None.

  Parameters:
    *WavHdr         - The pointer to the header buffer of a WAVE file. The
                      buffer size is 44 bytes.
    *hWMA           - The pointer to the WMA Handler
    DataLen         - Integer variable indicating the total number of samples
                      written into the file

  Returns:
    None.

  Example:
    <code>
    WMA_WaveHdr(WavBuf, WMA_State, DataLen);
    </code>

*/
void WMA_WaveHdr(char *WavHdr, void *hWMA, uint32_t DataLen);
/*******************************************************************************
  Function:
    void WMA_GetEncodeData(void *fWMA, char *pData, int pDtSz);

  Summary:
    Retrieves encoded data from the WAVE file header.

  Description:
    The function is used to retrieve the encoded data from the WAVE file header.

  Precondition:
    None.

  Parameters:
    *fWMA - The WMA file
	*pData - The pointer to the data
	pDtSz - The pointer to the data size

  Returns:
    None.

*/
void WMA_GetEncodeData(void *fWMA, char *pData, int pDtSz);
/*******************************************************************************
  Function:
    uint32_t WMA_FileGetPCM (void *pInt, short *pCh, uint32_t max_nsamples);

  Summary:
    Extracts the 16-bit samples.

  Description:
    This function is used to extract the 16-bit samples.

  Precondition:
    None.

  Parameters:
    *pInt           - The pointer to the WMA handler
    *pCh            - The pointer to the output data
    max_nsamples    - Total number of samples

  Returns:
    This function returns the number of samples.

  Example:
    <code>
    nDecodedSamples = WMA_FileGetPCM (WMA_State, WMA_OutBuf, nDecodedSamples);
    </code>

*/

uint32_t WMA_FileGetPCM (void *pInt, short *pCh, uint32_t max_nsamples);

/*******************************************************************************
  Function:
    int WMA_Parser(void *pInt);

  Summary:
    Parses the WMA data.

  Description:
    This function is used to parse the WMA data.

  Precondition:
    None.

  Parameters:
    *pInt           - The pointer to the WMA handler

  Returns:
    None.

*/

int WMA_Parser(void *pInt);	
						
/*******************************************************************************
  Function:
    int WMA_PacketData(void *p, char * InBuf, int residue);

  Summary:
    Extracts the encoded bitstream from the packet's payload.

  Description:
    This function is used to extract the encoded bitstream from the packet’s
    payload. This function is repeated until there are no bytes to decode.

  Precondition:
    None.

  Parameters:
    *p          - The pointer to the WMA handler
    InBuf       - The pointer to the input buffer
    residue     - The pointer to the input file

  Returns:
    None.

  Example:
    <code>
    void *WMA_State;
    Chnl_Info chnlInfo;
    Int res, residue;
    res = WMA_PacketData(WMA_State,WMA_InBuf,residue);
    </code>

*/

int WMA_PacketData(void *p, char * InBuf, int residue);

/*******************************************************************************
  Function:
    int32_t WMA_Decode (void* paudec, int* pcSamplesReady, int* paudecState);

  Summary:
    This function is called once to decode one or multiple frames.

  Description:
    This function is called once to decode one or multiple frames. This function
    will be called in an infinite while loop until the end-of-frame is detected
    or the decode function returns an error value. Unlike most speech applications,
    where the encoded data for each frame is deterministic, this is not the case
    in audio applications. Therefore, the application provides a chunk of input
    packet data (usually more than what is necessary for a single frame). The
    decoder parses them and decodes with a portion of the data. The decode function
    passes the information of how many bytes of data were used to the application
    so that the application saves the remaining data and presents to the decoder
    for the subsequent frame.

  Precondition:
    None.

  Parameters:
    paudec          - The pointer to the WMA Decoder handler
    pcSamplesReady  - The pointer to the number of decoded samples
    paudecState     - The pointer to the state of the decoder

  Returns:
    This function returns the error type.

  Example:
    <code>
    Int res;
    res = WMA_Decode(WMA_State, &nDecodedSamples, &chnlInfo.WMAState);
    </code>

  Remarks:
    None.

*/
int32_t WMA_Decode (void* paudec, int* pcSamplesReady, int* paudecState);

/*******************************************************************************
  Function:
    int32_t WMA_PacketDatasize(void *p);

  Summary:
    This function determines the packet size for decoding.

  Description:
    This function determines the packet size for decoding and sends request to
    the main program to read the number of bytes data for decoding. This function
    is called repetitively until there are no bytes to decode.

  Precondition:
    None.

  Parameters:
    *p      - The pointer to the WMA handler

  Returns:
    This function returns the packet data size.

  Example:
    <code>
    void *WMA_State;
    int packetdatasize;
    packetdatasize = WMA_PacketDatasize(WMA_State);
    </code>

*/

int32_t WMA_PacketDatasize(void *p);

/*******************************************************************************
  Function:
    uint32_t WMA_Header_Current_Offset(tWMAFileStateInternal *pInt)

  Summary:
    This function returns the amount of data used and updates the pointer.

  Description:
    This function returns the amount of data used and updates the file pointer
    location to read input data from the current offset after parsing the header
    information. This function is called only once for each file before starting
    the decoding of entire packets.

  Precondition:
    None.

  Parameters:
    *pInt       - The pointer to the WMA Handler

  Returns:
    This function returns the amount of data used.

  Example:
    <code>
    Uint32_t packet_offset;
    packet_offset = WMA_Header_Current_Offset(WMA_State);
    </code>

*/

uint32_t WMA_Header_Current_Offset(void *pInt);


#endif


/************************************************************************
  Function:
    void WMA_FileDecodeClr(void* WMA_State);
	
  Summary:
    Clears all of the allocated memory.
	
  Description:
    This function is used to clear all of the allocated WMA state memory.
	
  Precondition:
    None.
	
  Parameters:
    * WMA_State	- The pointer to the WMA State memory.

  Returns:
    None.
	
***********************************************************************/
void WMA_FileDecodeClr(void* WMA_State);

#ifndef WIN32
static __inline int stpnr(int x)
{
    __asm__ volatile( "move %0,$sp \n\t": "=r"(x));

    return x;
}

#ifdef __cplusplus
}
#endif

#endif