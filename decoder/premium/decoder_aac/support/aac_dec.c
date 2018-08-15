/*******************************************************************************
  AAC-LC Decoder Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    aac_dec.c

  Summary:
   Contains the functional implementation of aac decoder interface.

  Description:
   This file contains the functional implementation of opus decoder interface.
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

#include "aac_dec.h"

int32_t aac_State_Size;
void *haacDecoderInfo;
int32_t samp_Freq;
int16_t chnls, samp_Freq_Indx, i, size, check;
int16_t numInSamples,outsize,outFlag;
int32_t restBytes;
int8_t aacErrorCount;
int32_t aac_frame_count=0;
static uint8_t channelNum;
static int32_t aacFrameSize = 0;
static SYS_FS_HANDLE currentAACFilehandle;
AAC_DECODER_STATES aacState;
static SetReadBytesInAppData setReadBytesCB;
////////////////////////////////////////////////////////////////////////////////
#undef PROFILING

#ifdef PROFILING
uint32_t timerStart, timerEnd, cycles;
uint32_t packetSize = 0;
uint16_t decodeCount = 0;
float cyclesPerByte = 0.0;
//uint32_t cyclesArray[352];
#endif
////////////////////////////////////////////////////////////////////////////////

/*********************STATIC FUNCTIONS DECLARATION*****************************/
static int8_t GetChannels(uint8_t *ptr);



bool AAC_Initialize(void *heap,uint16_t size,uint8_t *ptr, SYS_FS_HANDLE aacFilehandle)
{
    if ( size < AACDECODER_STATE_SIZE )
    {
        return ( false );
    }
    else
    {
        
        haacDecoderInfo = heap;
        currentAACFilehandle = aacFilehandle;
        SYS_FS_FileRead(currentAACFilehandle, ptr, AAC_FRAME_HEADER_SIZE);
//        AACDecoder_GetStateSize();
        AACDecoder_Init(haacDecoderInfo,ptr);
        
        
        aacState = AAC_GET_FRAME_SIZE;
        
        aac_frame_count = 0;
        channelNum = GetChannels(ptr);
        return (true);
    }
}


void AAC_RegisterDecoderEventHandlerCallback(SetReadBytesInAppData fptr)
{
    setReadBytesCB = fptr;
}

int16_t AAC_Decoder( uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written )
{
    int16_t errorStatus;

    switch (aacState)
    {
        case AAC_GET_FRAME_SIZE:
        {
            aacFrameSize = AACDecoder_GetFrameSize(haacDecoderInfo,input);
            aacState = AAC_DECODE_FRAME;
            *read = AAC_FRAME_HEADER_SIZE;
            setReadBytesCB(aacFrameSize);
            *written =0;
            errorStatus = true;
            break;
        }
        case AAC_DECODE_FRAME:
        {
            
#ifdef PROFILING
            decodeCount++;
            if(decodeCount >= 100 && decodeCount <=150)
//            if(decodeCount == 100)
            {
                asm volatile("mtc0   $0,$9");
                asm volatile("mfc0   %0, $9" : "=r"(timerStart));
            }
#endif
            
            int32_t restBytes = 0;
            errorStatus = AAC_Decode(haacDecoderInfo,(uint8_t*)input,(INTPCM*)output, &outFlag, &restBytes);

#ifdef PROFILING   
                if(decodeCount >= 100 && decodeCount <= 150)
//            if(decodeCount == 100)
//                {
                    asm volatile("mfc0   %0, $9" : "=r"(timerEnd));
                    cycles += (timerEnd - timerStart);
//                    cyclesArray[aac_frame_count] = cycles;
                    

                    
//                    packetSize += (aacFrameSize+7);
                    
//                }
                    
                if(decodeCount == 150)
                {
                    cyclesPerByte = cycles/50;//(float)packetSize;
                }
#endif
            *read = aacFrameSize;
            
            setReadBytesCB(AAC_FRAME_HEADER_SIZE);
            aacState = AAC_GET_FRAME_SIZE;
            aac_frame_count++;
            
            if (!errorStatus)
            {
                aacErrorCount =0;
                if (outFlag)
                {
                    aacFrameSize = AACDecoder_InterleaveSamples((INTPCM*)output, haacDecoderInfo);

                    // Sample Size * (16bits/8bits))
                    *written = aacFrameSize * 2;
                }
                return (int16_t)true;
            }
            else
            {
                if ( ++ aacErrorCount > AAC_ERROR_COUNT_MAX )
                {
                    return ( false );
                }

                return (int16_t)true;
            }
            break;
        }
    }
    return (int16_t)errorStatus;
}





int32_t AAC_GetSamplingFrequency(uint8_t *ptr)
{
    AAC_SAMPLING_FREQUENCY_INDEX index;
    int32_t fs_variable;

    fs_variable = *(ptr+2);
    index = (fs_variable & 0x3c)>>2;
    switch (index)
    {
        case SAMPLING_FREQUENCY_IDX0:
        {
            return 96000;
        }

        case SAMPLING_FREQUENCY_IDX1:
        {
            return 88200;
        }
        case SAMPLING_FREQUENCY_IDX2:
        {
            return 64000;
        }
        case SAMPLING_FREQUENCY_IDX3:
        {
            return 48000;
        }
        case SAMPLING_FREQUENCY_IDX4:
        {
            return 44100;
        }
        case SAMPLING_FREQUENCY_IDX5:
        {
            return 32000;
        }

        case SAMPLING_FREQUENCY_IDX6:
        {
            return 24000;
        }
        case SAMPLING_FREQUENCY_IDX7:
        {
            return 22050;
        }
        case SAMPLING_FREQUENCY_IDX8:
        {
            return 16000;
        }
        case SAMPLING_FREQUENCY_IDX9:
        {
            return 12000;
        }
         case SAMPLING_FREQUENCY_IDX10:
        {
            return 11025;
        }

        case SAMPLING_FREQUENCY_IDX11:
        {
            return 8000;
        }
        case SAMPLING_FREQUENCY_IDX12:
        {
            return 7350;
        }
        case SAMPLING_FREQUENCY_IDX13:
        {
            return 44100;
        }
        case SAMPLING_FREQUENCY_IDX14:
        {
            return 44100;
        }
        case SAMPLING_FREQUENCY_IDX15:
        {
            return 44100;
        }

    }

    return 44100;

}


bool isAACdecoder_enabled()
{
	return true;
}

uint8_t AAC_GetChannels()
{
    return channelNum;
}

/*****************************STATIC FUNCTIONS IMPLEMENTATION******************/

static int8_t GetChannels(uint8_t *ptr)
{
    return (int8_t)(ptr[3] >> 6) & 0x3;
}