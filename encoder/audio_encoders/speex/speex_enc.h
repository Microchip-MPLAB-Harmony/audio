/*******************************************************************************
  MPLAB Harmony Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    speex_enc.h

  Summary:
    speex encoder source header.

  Description:
    speex encoder wrapper file of speex prebuilt library.
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

#ifndef _HAR_SPEEX_ENC_H    /* Guard against multiple inclusion */
#define _HAR_SPEEX_ENC_H


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
    
#define	OGG_ID_SPEEX			"Speex   "	// The Speex packet ID
#define	SPEEX_STRING_LENGTH		8			// The size of the Speex string	

#define SPEEX_VERSION			"speex-1.2beta3" 	// The Speex version string
#define	SPEEX_VERSION_LENGTH		20			    // The size of the Speex version string	

#define	SPEEX_VERSION_ID		1			        // Version identifier

#define SPEEX_VENDOR_STR		"Encoded by Microchip Harmony Framework"	// comment in Ogg header
    
#define DISABLE_WARNINGS


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
        bool speex_encoder_init_2(int channel, int inputSampleRate)

       Summary:
        Initialize speex encoder.

       Description:
        This functions takes streaming configurations, creates speex encoder instance.

       PreCondition:
        Make sure the heap size is large enough for speex encoder structure.

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
    bool speex_encoder_init_2(int channel, int inputSampleRate);

    // *****************************************************************************
    /* Function:
        bool speex_encode_frame(void *pin, 
            uint32_t insize, 
            void *pout, 
            uint32_t *outsize)

       Summary:
        encode one speex frame.

       Description:
        This function takes input buffer in pcm data, encodes it to an speex frame.

       PreCondition:
        this input buffer size must be size of one speex frame.

       Parameters:
        pin            - Input signal (interleaved if 2 channels). 
        insize         - Input size. length is frame_size*channels*sizeof(speex_int16)
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
    bool speex_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize);
    // *****************************************************************************
    /* Function:
        bool speex_encoder_free()

       Summary:
        Free speex encoder instance.

       Description:
        This function frees the speex encoder instance 
        which was created in speex_encoder_init_2 function

       PreCondition:
        Opus encoder must be initialized before calling this function.

       Parameters:
        None.

       Returns:
        bool           - Return true if free speex encoder is successful.s
                         otherwise return false.

       Example:
        None.

       Remarks:
        None.
    */
    bool speex_encoder_free();
    
    uint32_t speex_encoder_get_framesize();
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _HAR_SPEEX_ENC_H */

/* *****************************************************************************
 End of File
 */
