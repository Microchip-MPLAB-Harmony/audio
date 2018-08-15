/* 
 * File:   wma.c
 * Author: C16105
 *
 * Created on September 17, 2014, 10:20 AM
 */

#include "../wma_lib_api.h"
#include "wma_dec.h"
#include <stdlib.h>
#include <string.h>


static void *WMA_State=NULL;
static Chnl_Info channelInfo;

static int32_t wma_packet_offset;
int nDecodedSamples;
static WMA_DECODER_STATES wmaState;
int frame_count=0; //streams=0,
static uint8_t wmaErrorCount=0;

int errorStatus;
static SYS_FS_HANDLE currentWMAFileHandle;
static SetReadBytesReadFlagInAppData setReadBytesReadFlagCB;
static GetReadBytesInAppData         getReadBytesCB;

void WMA_Initialize(SYS_FS_HANDLE wmaFilehandle, uint32_t inputBufferSize)
{
    currentWMAFileHandle = wmaFilehandle;
    uint8_t wmaHeader[inputBufferSize];
    SYS_FS_FileRead(currentWMAFileHandle, wmaHeader, inputBufferSize);

    int32_t WMA_State_Size = WMA_GetStateSize();
    // this allocation will be freed in WMA_FileDecodeClr function
    WMA_State = malloc(WMA_State_Size);
    memset(&channelInfo,0,sizeof(channelInfo));

    /*WMA_Status WMAStatus =*/ WMA_Decoder_Init (WMA_State, &channelInfo,(char*)wmaHeader);
    
    wma_packet_offset = WMA_Header_Current_Offset(WMA_State);

    if( channelInfo.WMAState == 1)
    {
        setReadBytesReadFlagCB(WMA_PacketDatasize(WMA_State), true);     
    }
    wmaState = WMA_DECODE_FRAME;
}

int16_t WMA_Decoder( uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written )
{
    
    switch (wmaState)
    {
        case WMA_GET_FRAME_SIZE:
        {
            if(!nDecodedSamples)
            {
                if( channelInfo.WMAState == 1)
                {
                    setReadBytesReadFlagCB(WMA_PacketDatasize(WMA_State), true);
                }
            }
            wmaState = WMA_DECODE_FRAME;
            break;
        }

        case WMA_DECODE_FRAME:
        {
            
            setReadBytesReadFlagCB(-1, false);
            if(!nDecodedSamples)
            {
                if( channelInfo.WMAState == 1)
                {
                    errorStatus = WMA_PacketData(WMA_State,(char *)input, getReadBytesCB());
                }
                errorStatus = WMA_Decode(WMA_State,&nDecodedSamples, &channelInfo.WMAState);
            }
            *read = getReadBytesCB();
        }
        if (errorStatus>=0)
        {
            wmaErrorCount =0;
            if (nDecodedSamples > 0)
            {
                nDecodedSamples = WMA_FileGetPCM (WMA_State, output, nDecodedSamples);
                *written = nDecodedSamples * channelInfo.Channels * BYTES_PER_SAMPLE;
                nDecodedSamples = 0;
                frame_count++;
            }
            wmaState = WMA_GET_FRAME_SIZE;
            return (int16_t)true;
        }
        else if ( ++ wmaErrorCount > WMA_ERROR_COUNT_MAX )
        {
            return (int16_t)( false );
        }
    }
    return (int16_t)true;
}


void WMA_FreeMemory()
{
    if(WMA_State)
    {
        WMA_FileDecodeClr(WMA_State);
        WMA_State = NULL;
    }
}


int32_t WMA_GetHeaderPacketOffset(){
    return wma_packet_offset;
}

int32_t WMA_SamplingFrequency_Get(void)
{
    return (int32_t)channelInfo.SamplesPerSec;
}


int32_t WMA_BitRate_Get(void)
{
    return (int32_t)channelInfo.BitRate;
}
uint8_t WMA_GetChannels(){
    return channelInfo.Channels;
}
bool isWMAdecoder_enabled()
{
    return true;
}
void WMA_RegisterAppCallback(SetReadBytesReadFlagInAppData fptr0, GetReadBytesInAppData fptr1)
{
    setReadBytesReadFlagCB = fptr0;
    getReadBytesCB         = fptr1;
}
