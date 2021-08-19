/*******************************************************************************
 FLAC Decoder Library Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    flac_dec.h

  Summary:
  This header file consists of flac decoder library interface function declarations.
    
  Description:
    FLAC decoder interface function declarations:
  it is interface functions for easy use in Harmony audio framework.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 released Microchip Technology Inc.  All rights reserved.

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

#ifndef FLAC_DEC_H
#define FLAC_DEC_H

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
//DOM-IGNORE-END

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "configuration.h"
#include "system/fs/sys_fs.h"

// *****************************************************************************
/* DecoderEventHandlerCB - The Template for the callback Function:

  typedef void (*DecoderEventHandlerCB)(uint32_t event, void *cb_param, void *ctxt);

  Summary:
    Event Handler template for application callbacks from Decoder Library
    
  Description:
    FLAC Decoder library invokes these callbacks upon appropriate events during decoding
            
  Precondition:
    Successful Registration via function FLAC_RegisterDecoderEventHandlerCallback

  Parameters:
    event - The specific event that causes the callback
    cb_param - Pointer to a callback specific parameter
    ctxt - pointer to the context set during the event registration

  Returns:
    None

  Example:

  Remarks:
    None.
*/
typedef void (*DecoderEventHandlerCB)(uint32_t event, void *cb_param, void *ctxt);
/*  The set of callbacks to be called upon corresponding decoder actions

  Summary:
    Application defined callbacks for FLAC Library
  Description:
    These callbacks will be called from FLAC upon different events
*/
typedef struct
{
    DecoderEventHandlerCB flacDecAppReadCB;
    DecoderEventHandlerCB flacDecAppSeekCB;
    DecoderEventHandlerCB flacDecAppTellCB; 
    DecoderEventHandlerCB flacDecAppLengthCB; 
    DecoderEventHandlerCB flacDecAppEoFCB; 
    DecoderEventHandlerCB flacDecAppWriteCB;
    DecoderEventHandlerCB flacDecAppMetadataCB;
    DecoderEventHandlerCB flacDecAppErrorCB;
} FLAC_DEC_APPLICATIONCB_SET;

/*  Set of Write status upon decoder writecallback

  Summary:
    Status of decode process from FLAC 
  Description:
    Status of decoding activity 
*/
typedef enum
{
    FLAC_DEC_WRITE_EVT_CONTINUE,
    FLAC_DEC_WRITE_EVT_ERR            
} FLAC_DEC_APPLICATIONCB_EVT;

/*  The parameter format for Write callbacks

  Summary:
    The pointer to Decoded Buffer and its size in number of Samples
  Description:
    Upon a Decoded FLAC Buffer, the write callback is invoked with these params
*/
typedef struct
{
    int32_t *decBuf;
    uint32_t *decSize;
} FLAC_DEC_APPCB_WRITE_DATA;



// *****************************************************************************
/* Function isFLACdecoder_enabled:
        bool isFLACdecoder_enabled();

  Summary:
    Returns a boolean that represents if the FLAC decoder is included or not.

  Description:
    Return a boolean if the FLAC decoder is included or not.       

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    This function returns a boolean value.
  - 0 - FLAC decoder is not enabled.
    - 1 - FLAC decoder is enabled.

  Example:
    <code>
  bool flac_enabled = isFLACdecoder_enabled();
    </code>

  Remarks:
    None.
*/

bool isFLACdecoder_enabled(void);

// *****************************************************************************
/* Function FLAC_Initialize:
    bool FLAC_Initialize (SYS_FS_HANDLE fhandle, char *input_buffer);

  Summary:
    An abstraction function on FLAC decoder library, initialize necessary FLAC decoder state variables.

  Description:
    This function provides decoder initialize function to audio applications.
  
  Precondition:
    A FLAC audio file should be opened for read successfully.

  Parameters:
    fhandle       - file handle to a FLAC audio file opened in read mode
    input_buffer  - input buffer pointer, into which the file will be read.
  Returns:
    This function returns a boolean value.
    - 0 - FLAC decoder initialization failed.
    - 1 - FLAC decoder initialization succeed.

  Example:
    <code>
  bool flac_init_ret = FLAC_Initialize (flacHandle, input_buffer);
    </code>

  Remarks:
    None.
*/
bool FLAC_Initialize (SYS_FS_HANDLE fhandle, void *input_buffer);

// *****************************************************************************
/* Function FLAC_RegisterDecoderEventHandlerCallback:

       void FLAC_RegisterDecoderEventHandlerCallback(DecoderEventHandlerCB fptr);

  Summary:
    Register the set of decoder event handler functions to FLAC decoder.

  Description:
    This function registers a set of event handler functions for propagating FLAC decoding information 
  to the upper level.
  
  Precondition:
    None.

  Parameters:
    fptr  - event handler function pointer.

  Returns:
    None.

  Example:
    <code>
    FLAC_RegisterDecoderEventHandlerCallback(fptr);
    </code>

  Remarks:
    None.
*/
void FLAC_RegisterDecoderEventHandlerCallback(FLAC_DEC_APPLICATIONCB_SET *flac_decoder_appCB_set, void *ctxt);

// *****************************************************************************
/* Function FLAC_Decoder:

       bool FLAC_DecodeSingleFrame(uint8_t *output, uint32_t *written);

  Summary:
    An abstraction function on FLAC decoder library, 
  this function decodes one FLAC frame and returns the decoded data.

  Description:
    This function decodes an encoded frame and returns decoded data, 
  and provides an abstraction interface to use FLAC decode functions from library.
  
  Precondition:
    FLAC_Initialize function must be called successfully before this function.

  Parameters:
    output  - output buffer pointer which holds decoded data.
    written - size of decoded data.

  Returns:
    This function returns a boolean value.
    - 0 - FLAC decoder decodes failed.
    - 1 - FLAC decoder decodes succeed.

  Example:
    <code>
    bool success = FLAC_DecodeSingleFrame(output, written);
    </code>

  Remarks:
    None.
*/
bool FLAC_DecodeSingleFrame(uint8_t *output, uint32_t *written);

// *****************************************************************************
/* Function FLAC_GetChannels:

       uint8_t FLAC_GetChannels();

  Summary:
    Returns number of channels of the FLAC file.

  Description:
    This function returns total number of audio channels contained in the FLAC file.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  Number of audio channels

  Example:
    <code>
    num_channel = FLAC_GetChannels();
    </code>

  Remarks:
    None.
*/
uint8_t FLAC_GetChannels(void);

// *****************************************************************************
/* Function FLAC_GetBitRate:

       int32_t FLAC_GetBitRate();

  Summary:
    Returns bit rate of the FLAC audio file.

  Description:
    This function returns bit rate of the FLAC audio file.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  Bit rate of the FLAC audio file.

  Example:
    <code>
    bitrate = FLAC_GetBitRate();
    </code>

  Remarks:
    None.
*/
int32_t FLAC_GetBitRate(void);

// *****************************************************************************
/* Function FLAC_GetSamplingRate:

       int32_t FLAC_GetSamplingRate();

  Summary:
    Returns sample rate of the FLAC audio file.

  Description:
    This function returns sample rate of the FLAC audio file.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  Sample rate of the FLAC audio file.

  Example:
    <code>
    samplerate = FLAC_GetSamplingRate();
    </code>

  Remarks:
    None.
*/
int32_t FLAC_GetSamplingRate(void);

// *****************************************************************************
/* Function FLAC_GetBlockSize:

       int32_t FLAC_GetBlockSize();

  Summary:
    Returns size of next packet to be decoded.

  Description:
    This function returns size of next packet to be decoded.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  Size of next packet to be decoded.

  Example:
    <code>
    blocksize = FLAC_GetBlockSize();
    SYS_FS_FileRead(fptr, input, blocksize);
    </code>

  Remarks:
    None.
*/
int32_t FLAC_GetBlockSize(void);

// *****************************************************************************
/* Function FLAC_GetBitdepth:

       uint8_t FLAC_GetBitdepth();

  Summary:
    Returns bitdepth of the FLAC audio file.

  Description:
    This function returns bitdepth of the FLAC audio file.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  Bitdepth of the FLAC audio file.

  Example:
    <code>
    bitdepth = FLAC_GetBitdepth();
    </code>

  Remarks:
    None.
*/
uint8_t FLAC_GetBitdepth(void);

// *****************************************************************************
/* Function FLAC_GetDuration:

       uint32_t FLAC_GetDuration();

  Summary:
    Returns track length of the FLAC audio file.

  Description:
    This function returns track length of the FLAC audio file.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
  track length of the FLAC audio file.

  Example:
    <code>
    length_in_seconds = FLAC_GetDuration();
    </code>

  Remarks:
    None.
*/
uint32_t FLAC_GetDuration(void);
// *****************************************************************************
/* Function FLAC_Cleanup:

       void FLAC_Cleanup();

  Summary:
    A clean up function for deallocating memory resources of a FLAC decoder.

  Description:
    This function must be called after a FLAC audio file is decoded to free the memory resources.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
    void

  Example:
    <code>
    FLAC_Cleanup();
    </code>

  Remarks:
    None.
*/
void FLAC_Cleanup(void);
// *****************************************************************************
/* Function FLAC_DecoderIsFileValid:

       bool FLAC_DecoderIsFileValid(void);

  Summary:
    Function to check if a given FLAC file is valid or not.

  Description:
    Returns true if the given file on which decoding is attempted is valid.
  
  Precondition:
  FLAC_Initialize function must be called before this function.

  Parameters:
    None.

  Returns:
    bool

  Example:
    <code>
    is_valid = FLAC_DecoderIsFileValid();
    </code>

  Remarks:
    None.
*/
bool FLAC_DecoderIsFileValid(void);
#ifdef __cplusplus
}
#endif

#endif // FLAC_DEC_H
