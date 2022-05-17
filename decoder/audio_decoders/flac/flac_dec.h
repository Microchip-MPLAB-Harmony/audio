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
    For easy use in Harmony audio framework.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
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
/*  DecoderEventHandlerCB - The Callback function template for the application defined 
    callback Functions. This will be passed from application in the form of FLAC_DEC_APPLICATIONCB_SET:
    
    typedef void (*DecoderEventHandlerCB)(uint32_t event, void *cb_param, void *ctxt);

    Summary:
        Event Handler functin callback template for application's callbacks
    
    Description:
        FLAC Decoder library invokes these callbacks upon appropriate events during decoding, read, seek, etc
            
    Precondition:
        Successful Registration via function FLAC_RegisterDecoderEventHandlerCallback by application

    Parameters:
        event - The specific event that causes the callback
        cb_param - Pointer to a callback specific parameter
        ctxt - User defined pointer to a context set during the event registration, to be invoked upon callback 

    Returns:
        None

    Example:

    Remarks:
        None.
*/
typedef void (*DecoderEventHandlerCB)(uint32_t event, void *cb_param, void *ctxt);

/*  The set of callbacks an application is interested; 
    One per corresponding decoder action.
    This will be passed to FLAC library via FLAC_RegisterDecoderEventHandlerCallback

    Summary:
        Application defined callbacks for FLAC Library
        
    Description:
        These callbacks will be called from FLAC Library upon different events
*/
typedef struct
{
    DecoderEventHandlerCB flacDecAppReadCB;
    DecoderEventHandlerCB flacDecAppSeekCB;
    DecoderEventHandlerCB flacDecAppTellCB; 
    DecoderEventHandlerCB flacDecAppLengthCB; 
    DecoderEventHandlerCB flacDecAppEoFCB; 
    DecoderEventHandlerCB flacDecAppWriteCB; /* Callbacks with Raw bytes after FLAC decoding */
    DecoderEventHandlerCB flacDecAppMetadataCB;
    DecoderEventHandlerCB flacDecAppErrorCB;
} FLAC_DEC_APPLICATIONCB_SET;

/*  Set of Write status values upon decoder writecallback; 
    see flacDecAppWriteCB

    Summary:
        Status of decode process from FLAC 
        
    Description:
        Status of decoding activity 
*/
typedef enum
{
    FLAC_DEC_WRITE_EVT_CONTINUE, /* Success */
    FLAC_DEC_WRITE_EVT_ERR  /* Some error */          
} FLAC_DEC_APPLICATIONCB_EVT;

/*  The Write callback specific parameter

    Summary:
        The pointer to Decoded Buffer and its size in number of Samples
    
    Parameters:
        decBuf  : Buffer containing decoded samples corresponding to the channels
        decSize : The number of samples carried by the buffer
    
    Description:
        Upon a Decoded FLAC Buffer, the write callback is invoked with these params
*/
typedef struct
{
    int32_t *decBuf;
    uint32_t *decSize;
} FLAC_DEC_APPCB_WRITE_DATA;

// *****************************************************************************
/*  Function FLAC_isDecoder_enabled:
        bool FLAC_isDecoder_enabled(void);

    Summary:
        Returns a boolean that represents if the FLAC decoder is included or not.

    Description:
        Returns a boolean if the FLAC decoder is included or not.       

    Precondition:
        None.

    Parameters:
        None.

    Returns:
        This function returns a boolean value.
        0 - FLAC decoder is not enabled.
        1 - FLAC decoder is enabled.

    Example:
        <code>
            bool flac_enabled = FLAC_isDecoder_enabled();
        </code>

  Remarks:
    None.
*/

bool FLAC_isDecoder_enabled(void);

// *****************************************************************************
/*  Function FLAC_Initialize:
        bool FLAC_Initialize(SYS_FS_HANDLE fhandle, char *input_buffer);

    Summary:
        Function to initialize necessary FLAC decoder state variables.

    Description:
        This function provides decoder initialize function to audio applications.
  
    Precondition:
        A FLAC audio file should be opened for read successfully.

    Parameters:
        fhandle       - A valid file handle to a FLAC audio file, opened in read mode
        input_buffer  - input buffer pointer, into which the file will be read.
        
    Returns:
    This function returns a boolean value.
        0 - FLAC decoder initialization failed.
        1 - FLAC decoder initialization succeeded.

    Example:
    <code>
        bool flac_init_ret = FLAC_Initialize (flacHandle, input_buffer);
    </code>

    Remarks:
        None.
*/
bool FLAC_Initialize (SYS_FS_HANDLE fhandle, void *input_buffer);

// *****************************************************************************
/*  Function FLAC_RegisterDecoderEventHandlerCallback:
        void FLAC_RegisterDecoderEventHandlerCallback(FLAC_DEC_APPLICATIONCB_SET *flac_decoder_appCB_set, void *ctxt);

    Summary:
        Registers the set of application defined decoder event handler functions to FLAC decoder Library.

    Description:
        This function registers a set of event handler functions for propagating FLAC decoding information to the application.
  
    Precondition:
        None.

    Parameters:
        flac_decoder_appCB_set  - pointer to a struct that contains various application defined callbacks
        ctxt - The context that will be invoked upon callback invocation

    Returns:
        None.

    Example:
        <code>
        FLAC_DEC_APPLICATIONCB_SET flac_dec_appCB_set = {NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            FLAC_DecoderWriteEventHandler,
            NULL,
            NULL
        };
                
        FLAC_RegisterDecoderEventHandlerCallback(&flac_dec_appCB_set, ctxt);
        </code>

    Remarks:
        None.
*/
void FLAC_RegisterDecoderEventHandlerCallback(FLAC_DEC_APPLICATIONCB_SET *flac_decoder_appCB_set, void *ctxt);

// *****************************************************************************
/*  Function FLAC_DecodeSingleFrame:
        bool FLAC_DecodeSingleFrame(uint8_t *output);

    Summary:
        A wrapper function on FLAC decoder library, this function tries to decode one FLAC frame and returns the decoded data.

    Description:
        This function decodes an encoded frame and returns decoded data, thus provides an abstraction interface to use FLAC decode functions from library.
  
    Precondition:
        FLAC_Initialize function must be called successfully before this function.

    Parameters:
        output  - output buffer pointer to hold decoded data.

    Returns:
        This function returns a boolean value.
        0 - FLAC decoding failed, or end of file reached.
        1 - FLAC decoding succeeded.

    Example:
        <code>
        bool success = FLAC_DecodeSingleFrame(output, written);
        </code>

    Remarks:
        None.
*/
bool FLAC_DecodeSingleFrame(uint8_t *output);

// *****************************************************************************
/*  Function FLAC_GetChannels:
        uint8_t FLAC_GetChannels(void);

    Summary:
        Returns the number of channels contained in the FLAC file.

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
/*  Function FLAC_GetBitRate:
        int32_t FLAC_GetBitRate(void);

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
/*  Function FLAC_GetSamplingRate:
        int32_t FLAC_GetSamplingRate(void);

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
/*  Function FLAC_GetBlockSize:
        int32_t FLAC_GetBlockSize(void);

    Summary:
        Blocksize represents the number of samples in any of a block's subblocks. 
        For example, a one second block sampled at 44.1KHz has a blocksize of 44100, regardless of the number of channels. 
        This function returns that value.

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
/*  Function FLAC_GetBitdepth:
        uint8_t FLAC_GetBitdepth(void);

    Summary:
        Returns number of bits per sample of the FLAC audio file.

    Description:
        This function returns bitdepth that describes the resolution of the sound data that is captured and stored in the FLAC
  
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
/*  Function FLAC_GetDuration:
        uint32_t FLAC_GetDuration(void);

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
    void FLAC_Cleanup(void);

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
/*  Function FLAC_DecoderIsFileValid:
        bool FLAC_DecoderIsFileValid(void);

    Summary:
        Function to check if a given FLAC file is valid or not, as per the supported features of this implementation.

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
