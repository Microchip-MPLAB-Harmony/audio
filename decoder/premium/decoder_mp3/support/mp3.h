/*******************************************************************************
 MP3 Decoder Library Interface File (Deprecated)

  Company:
    Microchip Technology Inc.

  File Name:
    mp3.h

  Summary:
    MP3 Decoder support API.
	This file is deprecated, use mp3_dec.h instead

  Description:
    This header file consists of support function declarations.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.
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

#ifndef MP3_H
#define MP3_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END
    
#include <string.h>
#include "id3.h"
#include "system/fs/sys_fs.h"
    
typedef union
{
    uint32_t data;
    struct
    {
        uint8_t sync0      : 8;

        uint8_t CRC       : 1;
        uint8_t layer     : 2;
        uint8_t mpeg      : 2;
        uint8_t sync1      : 3;

        uint8_t private   : 1;
        uint8_t padding   : 1;
        uint8_t samprate  : 2;
        uint8_t bitrate   : 4;

        uint8_t nc        : 4;
        uint8_t extend    : 2;
        uint8_t stereo   : 2;
    };
}
MP3_FRAME_HEADER;

typedef struct
{
    int8_t tag[4];
    union
    {
        uint32_t flags;
        struct
        {
            uint32_t              :24;
            uint32_t frameInfo    :1;
            uint32_t sizeInfo     :1;
            uint32_t tocInfo      :1;
            uint32_t qualityInfo  :1;
            uint32_t              :4;
        };
    };

    uint8_t frames[4];
    uint8_t size[4];
    uint8_t toc[100];
    uint8_t quality;
}
MP3_XING_HEADER;

//CBR VBR enum defination
enum {
    STREAM_CBR,
    STREAM_VBR
};

typedef enum
{
    MP3_STATE_STREAM
}
MP3_STATE;

typedef struct{
    uint16_t mp3SampleRate;
    uint32_t mp3BitRate;
    uint32_t mp3Duration;
    
    // first frame position in file
    
    uint32_t firstFramePos;
    bool     isVBR;
    
    // for VBR MP3, mp3ValidBytes is the number of bytes in file is given by XING header,
    // for CBR MP3, mp3ValidBytes is MP3 audio file size - first frame position
    uint32_t mp3ValidBytes;
    uint8_t stereo;
}MP3_DEC;

typedef enum
{
    // Align TAG_ARTIST, TAG_ALBUM and TAG_TITLE with ID3 EVENT_TAGS
    MP3_EVENT_TAG_ARTIST, 
    MP3_EVENT_TAG_ALBUM,
    MP3_EVENT_TAG_TITLE,
    MP3_EVENT_STREAM_START,
    MP3_EVENT_SAMPLERATE,
    MP3_EVENT_BITRATE,
    MP3_EVENT_TRACK_TIME
}
MP3_EVENT;

#define MP3_HEADER_SAMPLERATE_44100         0b00
#define MP3_HEADER_SAMPLERATE_48000         0b01
#define MP3_HEADER_SAMPLERATE_32000         0b10
#define MP3_HEADER_SAMPLERATE_RESV          0b11

#define MP3_HEADER_CHANNELS_STEREO          0b00
#define MP3_HEADER_CHANNELS_JOINT           0b01
#define MP3_HEADER_CHANNELS_DUAL            0b10
#define MP3_HEADER_CHANNELS_MONO            0b11

#define MP3_XING_HEADER_START_MONO          17
#define MP3_XING_HEADER_START_STEREO        32

#define MP3_STATE_SIZE                      16876//11024
#define MP3_ERROR_COUNT_MAX                 1
#define MP3MPEG1L3_SAMPLES_PER_FRAME        1152 // MPEG1 LayerIII, samples per frame
#define MP3MPEG2L3_SAMPLES_PER_FRAME        576  // MPEG2/2.5 LayerIII, samples per frame

#define MP3_IN_FRAME_SIZE       6144//1538
#define MP3_OUT_FRAME_SIZE      1152 * 4
#define MP3_STATE_SIZE          16876//11024

typedef bool (*DecoderEventHandlerCB)(uint32_t event, uint32_t data);
////////////////////////////////////////////////////////////////////////////////

bool MP3_EventHandler ( MP3_EVENT event, uint32_t data );
void MP3_RegisterDecoderEventHandlerCallback(DecoderEventHandlerCB fptr);
bool MP3_Initialize ( void *heap, uint16_t size, SYS_FS_HANDLE mp3Filehandle);
bool MP3_Decode ( uint8_t *input, uint16_t inSize, uint16_t *read, uint8_t *output, uint16_t *written );
bool MP3_ParseVBR ( uint8_t* data );
bool isMP3decoder_enabled();
uint32_t MP3_UpdatePlaytime();
uint32_t MP3_GetAudioSize();
uint8_t MP3_GetChannels();
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
