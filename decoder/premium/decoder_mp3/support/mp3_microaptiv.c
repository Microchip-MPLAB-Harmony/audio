/*******************************************************************************
  MP3 Decoder Microaptiv Library (Deprecated)

  Company:
    Microchip Technology Inc.

  File Name:
    mp3_microaptiv.c

  Summary:
   Contains the functional implementation of mp3 microaptiv decoder.
   This file is deprecated, use mp3_dec_microaptiv.c instead.

  Description:
   This file contains the functional implementation of mp3 decoder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#define MP3_C
#include "../mp3_microaptiv_lib_api.h"
#include "mp3.h"
////////////////////////////////////////////////////////////////////////////////

void *mp3DecoderVars;
MP3_FORMAT mp3Format;
MP3_STATE mp3State;
uint16_t mp3ErrorCount;
static MP3_DEC   mp3Decoder;
static SYS_FS_HANDLE currentMP3FileHandle;
static DecoderEventHandlerCB decEventCB; // decoder event handler callback function
static int32_t findFirstFrameHeader(void *buf, int32_t prevFilePtr);
#define READBLOCKSIZE 2048
////////////////////////////////////PROFILE/////////////////////////////////////
#undef PROFILING

#ifdef PROFILING
unsigned int timerStart, timerEnd, cycles;
int decodeCount = 0;
#endif


////////////////////////////////////////////////////////////////////////////////

bool MP3_Initialize ( void *heap, uint16_t size, SYS_FS_HANDLE mp3Filehandle)
{
    bool ret = false;
    if ( size < MP3_STATE_SIZE )
    {
        return ( false );
    }
    else
    {
        currentMP3FileHandle = mp3Filehandle;
        uint8_t tagHeader[sizeof(ID3V2_TAG_HEADER)];
        // Read ID3 tag header
        SYS_FS_FileRead(currentMP3FileHandle, tagHeader, sizeof(ID3V2_TAG_HEADER));
        int32_t ID3Size = ID3_Initialize (tagHeader);
        if(ID3Size != 0){ // ID3 tag found
            int32_t ID3TagSize = ID3Size + sizeof(ID3V2_TAG_HEADER);
            uint8_t *id3Frame = heap;
            uint32_t skip=0;
            uint32_t read = 0;
            int8_t ret = 0;
            
            SYS_FS_FileRead(currentMP3FileHandle, id3Frame, READBLOCKSIZE);
            do{
                // If the id3 frame header fall off the read block
                if(sizeof(ID3V22_FRAME) + skip > READBLOCKSIZE)
                {
                    //read more
                    memcpy(id3Frame, id3Frame+skip, READBLOCKSIZE - skip);

                    if(ID3Size < skip)
                        SYS_FS_FileRead(currentMP3FileHandle, id3Frame + READBLOCKSIZE - skip, ID3Size);
                    else
                        SYS_FS_FileRead(currentMP3FileHandle, id3Frame + READBLOCKSIZE - skip, skip);

                    skip = 0;
                }    
                ret = 1;
                read = ID3_Parse_Frame((uint8_t*)(id3Frame+skip), READBLOCKSIZE - skip, &ret);
                if(ret == -2){
                    break;
                }
                // id3 frame content fall off read block
                if(ret == -1)
                {
                    // if its too large, just skip this frame
                    if(read > READBLOCKSIZE){
                        //SYS_FS_SEEK_CUR
                        SYS_FS_FileSeek(currentMP3FileHandle, read - READBLOCKSIZE + skip, SYS_FS_SEEK_CUR);
                        SYS_FS_FileRead(currentMP3FileHandle, id3Frame, READBLOCKSIZE);
                        ID3Size -= read;
                        skip = 0;
                        continue;
                    }
                    // move the left bytes to head of pointer
                    memcpy(id3Frame, id3Frame+skip, READBLOCKSIZE - skip);

                    // read more
                    SYS_FS_FileRead(currentMP3FileHandle, id3Frame+READBLOCKSIZE-skip, skip);
                    skip = 0;
                    continue;
                }
                if(read == 0)
                {
                    break;
                }

                ID3Size -= read;
                skip += read;

            }while(ID3Size > 0);

#if defined(PROFILING_INIT)
        asm volatile("mfc0   %0, $9" : "=r"(timerEnd));
        timetaken = (timerEnd - timerStart)/1000;
        Nop();
#endif
            SYS_FS_FileSeek(currentMP3FileHandle, ID3TagSize, SYS_FS_SEEK_SET);
            mp3Decoder.firstFramePos = findFirstFrameHeader(heap, ID3TagSize);
            if(mp3Decoder.firstFramePos==-1){
                // can not find first frame
                return false;
            }
    
        }else{ // Skip ID3 tag parser, find first frame
            SYS_FS_FileSeek(currentMP3FileHandle, 0, SYS_FS_SEEK_SET);
            mp3Decoder.firstFramePos = findFirstFrameHeader(heap, 0);
            if(mp3Decoder.firstFramePos == -1){
                // can not find first frame
                return false;
            }
        }

        // read first frame header and potential XING header if this MP3 is VBR audio
        SYS_FS_FileRead(currentMP3FileHandle, heap, sizeof(MP3_FRAME_HEADER)+MP3_XING_HEADER_START_STEREO+sizeof(MP3_XING_HEADER));
        mp3Decoder.isVBR = false;
        mp3Decoder.mp3SampleRate = 0;
        mp3Decoder.mp3BitRate = 0;
        mp3Decoder.mp3Duration = 0;
        mp3Decoder.mp3ValidBytes = 0;
        // get sampling rate and bit rate from first frame header.
        bool parse_frameheader = MP3_ParseVBR (heap);
        if(!parse_frameheader) return false;
        // At this point, mp3SampleRate and mp3BitRate are calculated
        if(mp3Decoder.mp3Duration == 0)
        {
            mp3Decoder.mp3Duration = (SYS_FS_FileSize(currentMP3FileHandle) - mp3Decoder.firstFramePos ) / 125 / mp3Decoder.mp3BitRate;
        }
        if(mp3Decoder.mp3ValidBytes == 0){
            mp3Decoder.mp3ValidBytes = (SYS_FS_FileSize(currentMP3FileHandle) - mp3Decoder.firstFramePos);
        }

        if(parse_frameheader)
        {
            // Display MP3 file parameters
            MP3_EventHandler(MP3_EVENT_SAMPLERATE, mp3Decoder.mp3SampleRate);
            MP3_EventHandler(MP3_EVENT_BITRATE, mp3Decoder.mp3BitRate);
            MP3_EventHandler(MP3_EVENT_TRACK_TIME, mp3Decoder.mp3Duration);
            
        }
        /*
        else{
          // cannot find the first frame in this MP3 file
        }
        */
        mp3State = MP3_STATE_STREAM;

        mp3DecoderVars = heap;
        //memset(heap,0, sizeof(heap));

        mp3ErrorCount = 0;

        // get ready for decoding
        SYS_FS_FileSeek(currentMP3FileHandle, mp3Decoder.firstFramePos, SYS_FS_SEEK_SET);
        
        
        ret = MP3Initialize ( mp3DecoderVars );
        MP3_EventHandler(MP3_EVENT_STREAM_START, 0);

        return ret;
    }
}

/////////////////////////////////////////////////////////////
uint32_t MP3_UpdatePlaytime(){

    uint32_t playtime;

    playtime = (SYS_FS_FileTell(currentMP3FileHandle) - mp3Decoder.firstFramePos)>>12;
    playtime *= mp3Decoder.mp3Duration;
    playtime /= mp3Decoder.mp3ValidBytes >> 12;
    return playtime;
}

bool MP3_Decode ( uint8_t *input, uint16_t inSize, uint16_t *read, uint8_t *output, uint16_t *written )
{
    *read = 0;
    *written = 0;

    switch ( mp3State )
    {
        case MP3_STATE_STREAM:
        {
            
#ifdef PROFILING
            decodeCount++;
            if(decodeCount >= 100 && decodeCount <=150)
            {
                asm volatile("mtc0   $0,$9");
                asm volatile("mfc0   %0, $9" : "=r"(timerStart));
            }
#endif
        
            if(MP3Decode ( mp3DecoderVars, input, inSize, read, &mp3Format, output, written ))
            {
                
#ifdef PROFILING   
                if(decodeCount >= 100 && decodeCount <= 150)
                {
                    asm volatile("mfc0   %0, $9" : "=r"(timerEnd));
                    cycles += (timerEnd - timerStart);
                    
                }
                if(decodeCount == 150)
                {
                    cycles = cycles/25;
                }
#endif
                *written *= 2;
                mp3ErrorCount = 0;

#if defined(DISP_VBR)
                if ( mp3Decoder.mp3BitRate != mp3Format.Kbps )
                {
                    mp3Decoder.mp3BitRate = mp3Format.Kbps;
                    {
                        // need bit rate to calculate track duration
                        MP3_EventHandler ( MP3_EVENT_BITRATE, mp3Decoder.mp3BitRate );
                        
                    }
                }
#endif
                return ( true );
            }
            else
            {
                MP3Initialize ( mp3DecoderVars );
                
                if ( ++ mp3ErrorCount > MP3_ERROR_COUNT_MAX )
                {
                    asm("Nop");

                    return ( false );
                }
                return ( true );
            }
            break;
        }
    }

    return ( false );
}

////////////////////////////////////////////////////////////////////////////////

bool MP3_ParseVBR ( uint8_t* data )
{
    MP3_FRAME_HEADER *frameHeader;
    MP3_XING_HEADER *xingHeader;
    uint32_t frames, time;
    uint16_t samplerate;

    frameHeader = (MP3_FRAME_HEADER *)data;
    data += 4;

    
    if (( frameHeader->mpeg == 0x03 ) && ( frameHeader->layer == 0x01 ))   //mpeg-1 layer-3
    {
        const uint16_t samplerateList[4] = { 44100, 48000, 32000, 0 };

        samplerate = samplerateList[frameHeader->samprate];

        if ( samplerate != 0 )
        {
            mp3Decoder.mp3SampleRate = samplerate;
        }
        else
        {
            return ( false );
        }
        
        if ( frameHeader->stereo == MP3_HEADER_CHANNELS_MONO )
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_MONO );
        }
        else
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_STEREO );
        }

        if (( memcmp ( xingHeader->tag, "Xing", 4 ) == 0 )
        || ( memcmp ( xingHeader->tag, "Info", 4 ) == 0 ))
        {
            mp3Decoder.isVBR = true;

            frames = 0;
            if ( xingHeader->frameInfo )
            {
                frames = (( xingHeader->frames[0] * 0x100 + xingHeader->frames[1] ) * 0x100 + xingHeader->frames[2] ) * 0x100 + xingHeader->frames[3];
            }

            if ( frames != 0 )
            {
                // time in seconds
                time = ( frames * MP3MPEG1L3_SAMPLES_PER_FRAME + samplerate / 2 ) / samplerate;
                if(time != 0){
                    // kilobits per sec = (bytes / time)*8/1000;
                    mp3Decoder.mp3ValidBytes = (( xingHeader->size[0] * 0x100 + xingHeader->size[1] ) * 0x100 + xingHeader->size[2] ) * 0x100 + xingHeader->size[3];
                    mp3Decoder.mp3BitRate = (mp3Decoder.mp3ValidBytes)/125/time;
                }
                mp3Decoder.mp3Duration = time;
            }
        }
        else
        {
            const uint16_t bitrateList[16] = { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 };
            uint16_t bitrate = bitrateList[frameHeader->bitrate];

            if ( bitrate != 0 )
            {
                mp3Decoder.mp3BitRate = bitrate;
            }
            else
            {
                return ( false );
            }
        }

        return ( true );
    }
    //http://id3.org/mp3Frame
    if(( frameHeader->mpeg == 0x02 ) && ( frameHeader->layer == 0x01 )) //MPEG 2 Layer III
    {
        const uint16_t samplerateList[4] = { 22050, 24000, 16000, 0 };
        samplerate = samplerateList[frameHeader->samprate];

        if ( samplerate != 0 )
        {
            mp3Decoder.mp3SampleRate = samplerate;
        }
        else
        {
            return ( false );
        }
        
        if ( frameHeader->stereo == MP3_HEADER_CHANNELS_MONO )
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_MONO );
        }
        else
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_STEREO );
        }
        
        if (( memcmp ( xingHeader->tag, "Xing", 4 ) == 0 )
        || ( memcmp ( xingHeader->tag, "Info", 4 ) == 0 ))
        {
           
            mp3Decoder.isVBR = true;
            frames = 0;
            if ( xingHeader->frameInfo )
            {
                frames = (( xingHeader->frames[0] * 0x100 + xingHeader->frames[1] ) * 0x100 + xingHeader->frames[2] ) * 0x100 + xingHeader->frames[3];
            }

            if ( frames != 0 )
            {
                time = ( frames * MP3MPEG2L3_SAMPLES_PER_FRAME + samplerate / 2 ) / samplerate;
                if(time != 0){
                    // kilobits per sec = (bytes / time)*8/1000;
                    mp3Decoder.mp3ValidBytes = (( xingHeader->size[0] * 0x100 + xingHeader->size[1] ) * 0x100 + xingHeader->size[2] ) * 0x100 + xingHeader->size[3];
                    mp3Decoder.mp3BitRate = (uint8_t)(mp3Decoder.mp3ValidBytes)/125/time;
                }
                mp3Decoder.mp3Duration = time;
            }
        }
        else
        {
            const uint16_t bitrateList[16] = { 0, 8, 16, 24, 32, 64, 80, 56, 64, 128, 160, 112, 128, 256, 320, 0 };
            uint16_t bitrate = bitrateList[frameHeader->bitrate];

            if ( bitrate != 0 )
            {
                mp3Decoder.mp3BitRate = bitrate;
            }
            else
            {
                return ( false );
            }
        }

        // This microaptiv version doesn't support MPEG 2,
        // Use advanced MP3 library for full layer 3 support.
        return ( false );
    }
    
    //http://www.codeproject.com/Articles/8295/MPEG-Audio-Frame-Header#SamplingRate
    if(( frameHeader->mpeg == 0x00 ) && ( frameHeader->layer == 0x01 )) //MPEG 2.5 Layer III
    {
        const uint16_t samplerateList[4] = { 11025, 12000, 8000, 0 };
        samplerate = samplerateList[frameHeader->samprate];

        if ( samplerate != 0 )
        {
            mp3Decoder.mp3SampleRate = samplerate;
        }
        else
        {
            return ( false );
        }
        
        if ( frameHeader->stereo == MP3_HEADER_CHANNELS_MONO )
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_MONO );
        }
        else
        {
            xingHeader = (MP3_XING_HEADER *)( data + MP3_XING_HEADER_START_STEREO );
        }
        
        if (( memcmp ( xingHeader->tag, "Xing", 4 ) == 0 )
        || ( memcmp ( xingHeader->tag, "Info", 4 ) == 0 ))
        {
            mp3Decoder.isVBR = true;
            frames = 0;
            if ( xingHeader->frameInfo )
            {
                frames = (( xingHeader->frames[0] * 0x100 + xingHeader->frames[1] ) * 0x100 + xingHeader->frames[2] ) * 0x100 + xingHeader->frames[3];
            }

            if ( frames != 0 )
            {
                time = ( frames * MP3MPEG2L3_SAMPLES_PER_FRAME + samplerate / 2 ) / samplerate;
                if(time != 0){
                    // kilobits per sec = (bytes / time)*8/1000;
                    mp3Decoder.mp3ValidBytes = (( xingHeader->size[0] * 0x100 + xingHeader->size[1] ) * 0x100 + xingHeader->size[2] ) * 0x100 + xingHeader->size[3];
                    mp3Decoder.mp3BitRate = (uint8_t)(mp3Decoder.mp3ValidBytes)/125/time;
                }
                mp3Decoder.mp3Duration = time;
            }
        }
        else
        {
            const uint16_t bitrateList[16] = { 0, 8, 16, 24, 32, 64, 80, 56, 64, 128, 160, 112, 128, 256, 320, 0 };
            uint16_t bitrate = bitrateList[frameHeader->bitrate];

            if ( bitrate != 0 )
            {
                mp3Decoder.mp3BitRate = bitrate;
            }
            else
            {
                return ( false );
            }
        }

        // This microaptiv version doesn't support MPEG 2.5,
        // Use advanced MP3 library for full layer 3 support.
        return ( false );
    }

    return ( false );
}

void MP3_RegisterDecoderEventHandlerCallback(DecoderEventHandlerCB fptr)
{
    decEventCB = fptr;
}

bool MP3_EventHandler ( MP3_EVENT event, uint32_t data )
{
    switch ( event )
    {
        case MP3_EVENT_STREAM_START:
            decEventCB ( MP3_EVENT_STREAM_START, data );
            return ( true );

        case MP3_EVENT_SAMPLERATE:
            decEventCB ( MP3_EVENT_SAMPLERATE, data );
            return ( true );

        case MP3_EVENT_BITRATE:
            decEventCB ( MP3_EVENT_BITRATE, data );
            return ( true );

        case MP3_EVENT_TRACK_TIME:
            decEventCB ( MP3_EVENT_TRACK_TIME, data );
            return ( true );
        case MP3_EVENT_TAG_ARTIST:
        case MP3_EVENT_TAG_ALBUM:
        case MP3_EVENT_TAG_TITLE:
            return true;
    }

    return ( false );
}

bool isMP3decoder_enabled()
{
    return true;
}

uint8_t MP3_GetChannels(){
    int ret = 0;
    switch(mp3Decoder.stereo){
        case 0: // Stereo
        case 1: // Joint stereo (Stereo)
        case 2: // Dual channel (2 mono channels)
            ret = 2;
            break;
        case 3: // Single channel (Mono)
            ret = 1;
            break;
        default:
            //something wrong
            break;
    }
    return ret;
}

uint32_t MP3_GetAudioSize(){
    return mp3Decoder.mp3ValidBytes;
}

////////////////////////////////////////////////////////////////////////////////

static int32_t findFirstFrameHeader(void *buf, int32_t prevFilePtr){
    uint8_t *frame_cursor = buf;
    uint32_t ret = 1;
    uint32_t offset = 0;

    ret = SYS_FS_FileRead(currentMP3FileHandle, frame_cursor, READBLOCKSIZE);
    do{
        // exceed reading block
        if(frame_cursor == (uint8_t*)(buf)+ READBLOCKSIZE-1)
        {
            uint8_t tmp = frame_cursor[READBLOCKSIZE-1];
            frame_cursor = buf;
            frame_cursor[0] = tmp;      
            ret = SYS_FS_FileRead(currentMP3FileHandle, frame_cursor+1, READBLOCKSIZE-1);
        }
        // <editor-fold defaultstate="collapsed" desc="Frame Header Bits">
        // frame sync bits, first 11 bits must be set
        // AAAAAAAA AAABBCCD
        // Sign	Length  Position            Description
        // A	11      (31-21)     Frame sync (all bits must be set)
        // B	2       (20,19)         MPEG Audio version ID
        //                              00 - MPEG Version 2.5 (later extension of MPEG 2)
        //                              01 - reserved
        //                              10 - MPEG Version 2 (ISO/IEC 13818-3)
        //                              11 - MPEG Version 1 (ISO/IEC 11172-3)
        //
        // C	2       (18,17)         Layer description
        //                              00 - reserved
        //                              01 - Layer III
        //                              10 - Layer II
        //                              11 - Layer I
        //
        // D	1       (16)            Protection bit
        //                              0 - Protected by CRC (16bit CRC follows header)
        //                              1 - Not protected
        // </editor-fold>
        if(frame_cursor[0] == 0xFF && ((frame_cursor[1]&0xE0) == 0xE0 || (frame_cursor[1]&0xF0) == 0xF0))
        {
            // found first frame header
            break;
        }
        frame_cursor++;
        offset++;
        
    }while(ret != READBLOCKSIZE);

    if(ret != READBLOCKSIZE){ // end of file
        // wrong MP3 file
        return -1;
    }

    // Set file pointer to the beginning of first frame
    SYS_FS_FileSeek(currentMP3FileHandle, prevFilePtr+offset, SYS_FS_SEEK_SET);
    return prevFilePtr+offset;
}