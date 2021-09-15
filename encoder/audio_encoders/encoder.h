/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    encoder.h

  Summary:
    This header file provides prototypes and definitions for encoder general 
    interface functions.

  Description:
    This header file provides function prototypes and data type definitions for
    encoder file. encoder interface file is supposed to wrap one encoder or 
    multiple encoders to provide application an uniform encoder interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _ENCODER_H    /* Guard against multiple inclusion */
#define _ENCODER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdarg.h>


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


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    typedef enum 
    {
        WAV_RIFF_PCM_FILE=0,  // Linear PCM/uncompressed
        WAV_RIFF_ADPCM_FILE, // IMA ADPCM
        OGG_OPUS_FILE,
        OGG_SPEEX_FILE
    }AUDIO_FILE_FORMAT;
    
    typedef enum
    {
        PCM=0, // Raw data
        ADPCM, // IMA ADPCM
        OPUS,  // http://opus-codec.org/
        SPEEX  // https://www.speex.org/    
    }EncoderType;

    typedef struct
    {
        int sample_rate;
        int bit_depth;
        int bps;
        int channel; // channel number
    }StreamInfo;
    
    // For 
    typedef struct 
    {
        int audio_size;
        //  be more..
    }FileInfo;
    
    typedef struct{
        // common codec properties
        //StreamInfo stream_info;
        FileInfo file_info;
        EncoderType encoder_type;
    }EncoderState; // encoder runtime structure
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    
  /*******************************************************************************
  Function:
    void encoder_init ( void )

  Summary:
   A initialization function for a encoder or multiple encoders

  Description:
    A initialization function for a encoder or multiple encoders

  Precondition:
    

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    encoder_init();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
//    inline StreamInfo *encoder_get_stream_info();
//    inline EncoderState *encoder_get_encoder_state();
    
    /*************** set stream info ***************
     **********************************************/
//    bool encoder_set_stream_info(StreamInfo si);
//    bool encoder_stream_set_sample_rate(int sample_rate);
//    bool encoder_stream_set_btirate(int bps);
//    bool encoder_stream_set_bitdepth(int bit_depth);
//    bool encoder_stream_set_channel(int channel);
    
    
    /************ set file info *****************
     ********************************************/
//    bool encoder_set_file_info(FileInfo fi);
//    bool encoder_set_audio_data_size(int audio_size);
    
/***Interfaces for Encoder Functions *****/
typedef bool(*har_encoder_init_fp)(int channel, int inputSampleRate);
typedef bool(*har_encoder_free_fp)(void);
typedef bool(*har_encode_one_frame_fp)(void *pin, uint32_t insize, void *pout, uint32_t *outsize);
typedef int(*har_encoder_get_header_size)(void);

typedef struct HAR_ENCODER_INTERFACE
{
	EncoderType encoderType;
	har_encoder_init_fp enc_init;
	har_encoder_free_fp enc_free;
	har_encode_one_frame_fp enc_one_frame;
}HAR_ENCODER;

/**Create by template depends on selected encoders **/
extern const HAR_ENCODER speexEncoderInst;
extern const HAR_ENCODER pcmEncoderInst;
extern const HAR_ENCODER adpcmEncoderInst;
extern const HAR_ENCODER opusEncoderInst;

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
