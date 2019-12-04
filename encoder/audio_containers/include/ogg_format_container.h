/*******************************************************************************
  MPLAB Harmony Middleware Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ogg_format_container.h

  Summary:
    ogg format container header file.

  Description:
    ogg format container header file.
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
#ifndef _OGG_FORMAT_CONTAINER_H    /* Guard against multiple inclusion */
#define _OGG_FORMAT_CONTAINER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

// audio container needs information from encoder to encapsulate an audio file
//#include "../encoder.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    #include "ogg/ogg.h"
    #include "encoder.h"
    /**
     */
typedef void (*OGG_CONTAINER_PAGEOUT_CALLBACK) (ogg_page *og); 
#ifndef _OGG_FORMAT_CONTAINER_C



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    bool ogg_container_stream_init(int serialno, 
        OGG_CONTAINER_PAGEOUT_CALLBACK pageout_cb,
        uint8_t num_packets_to_one_ogg_page);
    
    void ogg_container_config_set_pageout_callback(
        OGG_CONTAINER_PAGEOUT_CALLBACK pageout_cb);

    
    void ogg_container_stream_destroy();
    int ogg_container_encapsulate_packet(EncoderType encoderType, void *encoded, 
        int size, 
        int framecount, 
        int framesize, 
        int inputSampleRate,
        bool lastpacket);
    
//    int32_t ogg_opus_encapsulate(void *encoded, 
//        int size, 
//        int framecount, 
//        int framesize, 
//        int inputSampleRate,
//        bool lastpacket);
//    int32_t ogg_speex_encapsulate(void *encoded, 
//        int size, 
//        int framecount, 
//        int framesize, 
//        int inputSampleRate,
//        bool lastpacket);
    
    int32_t ogg_opus_fill_header(
            int channel, 
            int inputSampleRate,
            int bitrate,
            void *pheader, 
            uint32_t size);
    int32_t ogg_speex_fill_header(
            int channel,
            int inputSampleRate,
            int bitrate,
            void *pheader, 
            uint32_t size);
#else
    
    /**
     * Private Functions and Structures
     * 
     *  
     */
#include <stdint.h>

    /**
     * Private Structure
     * Not allow for public 
     */
    typedef struct
    {
        OGG_CONTAINER_PAGEOUT_CALLBACK pageout_callback;
        uint8_t NUM_PACKETS_TO_ONE_PAGE;
            
    }OGG_CONFIG_S;
    int32_t ogg_opus_get_header_packets(void* buffer, int channel, 
        int inputSampleRate, int preskip, int gain, int channelmap);
    
    int32_t ogg_speex_get_header_packet(void* buffer, int nb_channel, 
        int inputSampleRate, int bitrate, int frames_per_packet);
#endif


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _OGG_FORMAT_CONTAINER_H */

/* *****************************************************************************
 End of File
 */
