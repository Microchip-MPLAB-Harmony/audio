/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "disk.h"
#include <stdio.h>
#ifdef GFX_ENABLED
#include "gfx/libaria/libaria_init.h"
#include "gfx/libaria/inc/libaria_widget_circular_slider.h"
#endif
#ifdef MP3_DECODER_ENABLED
#include "audio/decoder/audio_decoders/mp3/fixpnt/pub/mp3dec.h"
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint16_t volumeLevels[VOL_LVL_MAX] =
{
#ifdef DRV_AK4954_VOLUME
    0 /* off */, 192, 208, 224          // if using AK4954, use narrower range  
#else            
    0 /* off */, 64, 128, 255            
#endif 
};

#ifdef DATA32_ENABLED
DRV_I2S_DATA32m __attribute__ ((aligned (32))) App_Audio_Output_Buffer1[NUM_SAMPLES];
DRV_I2S_DATA32m __attribute__ ((aligned (32))) App_Audio_Output_Buffer2[NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_TempBuf[NUM_SAMPLES];
#else
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer1[NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer2[NUM_SAMPLES];
#endif
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Input_Buffer[NUM_SAMPLES];

static uint16_t rd, wrtn;
int32_t bytesRead;

#ifdef WAV_STREAMING_ENABLED
static dWAVHEADER wavHdr;
extern WAV_DEC wavDecoder;
#endif

#ifdef ADPCM_STREAMING_ENABLED
static ADPCMHEADER adpcmHdr;
#endif

#ifdef MP3_DECODER_ENABLED
void APP_ID3_EventHandler(int event, uint32_t data);
void * hMP3Decoder;

#define READBUF_SIZE	1940
#if (NUM_SAMPLES*2) > READBUF_SIZE
// if size of generic input buffer is > size needed for MP3 buffer, we can use share it
uint8_t* MP3readBuf = (uint8_t*)App_Audio_Input_Buffer;
#else
uint8_t MP3readBuf [READBUF_SIZE];
#endif
uint8_t *MP3readPtr;
int32_t MP3bytesLeft;
int32_t MP3offset;

#define NUM_MP3_OUTPUT_SAMPLES  1152
#if NUM_SAMPLES < NUM_MP3_OUTPUT_SAMPLES
#error NUM_SAMPLES too small
#endif
int16_t MP3outBuf[2*NUM_MP3_OUTPUT_SAMPLES];
#endif

#ifdef GFX_ENABLED
extern laCircularSliderWidget * VolumeWidget;
extern laProgressBarWidget * ProgressBarWidget;
extern laLabelWidget * txtFilename;
extern laLabelWidget * txtSampleRate;
extern laLabelWidget * txtChannels;
extern laLabelWidget * txtBitDepth;
extern laLabelWidget * txtVolumePercent;
extern laLabelWidget * txtCurrentPosition;
extern laLabelWidget * txtEndPosition;
extern laLabelWidget * txtArtist;
extern laLabelWidget * txtTrackTitle;
extern laLabelWidget * txtAlbumTitle;
extern laImageWidget* PlayStopBtn;

laString laTmpStr;
#endif

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/
APP_DATA appData;
LED_DATA ledData;
BTN_DATA btnData;

#define char_tolower(c)      (char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions. */
// ****************************************************************************
/*
  Function:
        static void WM8904_TimerCallback
        (
            uintptr_t context
        )

  Summary:
    Implements the handler for timer callback function.

  Description:
    Called every 1 ms by timer services.  It then decrements WM8904Delay if
    non-zero.

  Parameters:
    context      - Contains index into driver's instance object array

  Remarks:
    None
*/
static void App_TimerCallback( uintptr_t context)
{
    if (ledData.led1Delay)
    {
        ledData.led1Delay--;
    }
    if (ledData.led2Delay)
    {
        ledData.led2Delay--;
    }
#ifdef GFX_ENABLED
    if (appData.guiUpdate)
    {
        appData.guiUpdate--;
    }
#else
    if (btnData.delay)
    {      
        btnData.delay--;
    }
#endif
    //LED2_Toggle();        // to check timing
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
APP_DATA * APP_GetAppDataInstance()
{
    return &appData;
}

void APP_PlayerInitialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.fileHandle = SYS_FS_HANDLE_INVALID;
    appData.fileStatus.lfsize = FAT_FS_MAX_LFN;
    appData.readBytes = 0;
    appData.current_filesize = 0;
    
    DISK_Initialize();
}

/* TODO:  Add any necessary local functions.
*/
#ifdef GFX_ENABLED
void APP_Init_GUI_Strings()
{
    laTmpStr = laString_CreateFromCharBuffer( "", 
                            GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
    laLabelWidget_SetText( txtFilename, laTmpStr );
    laLabelWidget_SetText( txtSampleRate, laTmpStr );
    laLabelWidget_SetText( txtChannels, laTmpStr );
    laLabelWidget_SetText( txtBitDepth, laTmpStr );
    laLabelWidget_SetText( txtArtist, laTmpStr );
    laLabelWidget_SetText( txtTrackTitle, laTmpStr );    
    laLabelWidget_SetText( txtAlbumTitle, laTmpStr );    
    
    laTmpStr = laString_CreateFromCharBuffer( "00:00", 
                            GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
    laLabelWidget_SetText( txtCurrentPosition, laTmpStr );
    laLabelWidget_SetText( txtEndPosition, laTmpStr );
    laString_Destroy( &laTmpStr );
    
    laProgressBarWidget_SetValue( ProgressBarWidget, 0 );

    laImageWidget_SetImage(PlayStopBtn, &playBtn);              
}

void APP_Set_GUI_BitDepthStr( void )
{
    static char bdStr[5];
    if( appData.bitDepth )
    {
        switch( appData.bitDepth )
        {
            case 8:
                strcpy(bdStr, "8b");
                break;
            case 16:
            default:
                strcpy(bdStr, "16b");
                break;
        }
        laTmpStr = laString_CreateFromCharBuffer( bdStr, 
                    GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtBitDepth, laTmpStr );
        laString_Destroy( &laTmpStr );
    }
}

void APP_Set_GUI_SampleRateStr( void )
{
    static char srStr[10];
    
    if( appData.sampleRate )
    {
        sprintf( srStr, "%d", (int)appData.sampleRate );
        laTmpStr = laString_CreateFromCharBuffer( srStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtSampleRate, laTmpStr );
        laString_Destroy( &laTmpStr );
    }
}
void APP_Get_GUI_Volume( void )
{
    static char volPctStr[5];
    uint16_t volPercent;
    
    if( (appData.state != APP_STATE_IDLE) && (appData.state != APP_STATE_INIT) && (appData.state != APP_STATE_CODEC_OPEN) )
    {
        // Returns 32 bit value
        volPercent = laCircularSliderWidget_GetValue( VolumeWidget );
        if( appData.oldVolPercent != volPercent )
        {
            appData.volume = (volPercent * 255) / 100;
            //DRV_CODEC_VolumeSet( appData.codecData.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume );
            snprintf( volPctStr, sizeof(volPctStr)-1, "%d%s", (uint8_t)volPercent, "%");
            laTmpStr = laString_CreateFromCharBuffer( volPctStr, 
                                    GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
            laLabelWidget_SetText( txtVolumePercent, laTmpStr );
            laString_Destroy( &laTmpStr );
            appData.oldVolPercent = appData.volPercent;
        }
    }
}
void APP_Set_GUI_ChnlStr( void )
{
    static char chnlStr[10];
    
    if( appData.numOfChnls )
    {
        switch( appData.numOfChnls )
        {
            case 1:
                strcpy( chnlStr, "Mono");
                break;
            case 2:
            default:
                strcpy( chnlStr, "Stereo");
                break;
            case 3:
                strcpy( chnlStr, "2.1");
                break;
            case 6:
                strcpy( chnlStr, "5.1");
                break;
            case 8:
                strcpy( chnlStr, "7.1");
                break;
        }
        laTmpStr = laString_CreateFromCharBuffer( chnlStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtChannels, laTmpStr );
        laString_Destroy( &laTmpStr );
    }
}

void APP_Set_GUI_TrackPositionStr( void )
{
    uint32_t dur, time, mins, secs;
    static char totalTimeStr[10];
    static char currTimeStr[10];
    if( appData.sampleRate && appData.numOfChnls )
    {
        switch( appData.currentStreamType )
        {
#ifdef WAV_STREAMING_ENABLED
            case APP_STREAM_WAV:
                dur = (appData.fileSize - sizeof( WAV_FILE_HEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
            case APP_STREAM_ADPCM:
                dur = 4*(appData.fileSize - sizeof( ADPCMHEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef MP3_DECODER_ENABLED
            case APP_STREAM_MP3:
                dur = appData.playbackDuration;
                break;
#endif                
            default:
                dur = 0;
                break;
        }
        mins = dur / 60;
        secs = dur % 60;
        sprintf( totalTimeStr, "%02d:%02d", (int)mins, (int)secs);
        laTmpStr = laString_CreateFromCharBuffer( totalTimeStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtEndPosition, laTmpStr );
        laString_Destroy( &laTmpStr );

        switch( appData.currentStreamType )
        {
#ifdef WAV_STREAMING_ENABLED
            case APP_STREAM_WAV:
                time = (appData.currPos - sizeof( WAV_FILE_HEADER))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
            case APP_STREAM_ADPCM:
                time = 4*(appData.currPos - sizeof( ADPCMHEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef MP3_DECODER_ENABLED
            case APP_STREAM_MP3:
                time = ((appData.currPos-appData.mp3FirstFrame)*appData.playbackDuration)/appData.fileSize;
                break;
#endif                 
            default:
                time = 0;
                break;
        }
        mins = time / 60;
        secs = time % 60;
        sprintf( currTimeStr, "%02d:%02d", (int)mins, (int)secs);
        laTmpStr = laString_CreateFromCharBuffer( currTimeStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtCurrentPosition, laTmpStr );
        laString_Destroy( &laTmpStr );
        
        if (time >= dur)
        {
            appData.state = APP_STATE_CLOSE_FILE;   // if at or past duration, force file to close
        }        
        
        switch( appData.currentStreamType )
        {
#ifdef WAV_STREAMING_ENABLED
            case APP_STREAM_WAV:
                laProgressBarWidget_SetValue( ProgressBarWidget, (100*(appData.currPos-sizeof( WAV_FILE_HEADER)))/(appData.fileSize-sizeof( WAV_FILE_HEADER)) );
                break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
            case APP_STREAM_ADPCM:
                laProgressBarWidget_SetValue( ProgressBarWidget, (100*(appData.currPos-sizeof( ADPCMHEADER)))/(appData.fileSize-sizeof( ADPCMHEADER)) );
                break;
#endif
#ifdef MP3_DECODER_ENABLED
            case APP_STREAM_MP3:
                laProgressBarWidget_SetValue( ProgressBarWidget, (100*(appData.currPos-appData.mp3FirstFrame))/(appData.fileSize-appData.mp3FirstFrame) );
                break;
#endif                
            default:
                break;
        }
    }
}

void APP_Set_GUI_FileNameStr( void )
{
    static char fileNameStr[20];
    
    if( strlen(appData.fileName) > strlen("/mnt/myDrive1/") )
    {
        sprintf( fileNameStr, "%s", &appData.fileName[strlen("/mnt/myDrive1/")]);
        laTmpStr = laString_CreateFromCharBuffer( fileNameStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( txtFilename, laTmpStr );
        laString_Destroy( &laTmpStr );
    }
}
#else
void APP_UpdateTrackPosition()
{
    uint32_t dur, time;
    if( appData.sampleRate && appData.numOfChnls )
    {
        switch( appData.currentStreamType )
        {
#ifdef WAV_STREAMING_ENABLED
            case APP_STREAM_WAV:
                dur = (appData.fileSize - sizeof( WAV_FILE_HEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                time = (appData.currPos - sizeof( WAV_FILE_HEADER))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
            case APP_STREAM_ADPCM:
                dur = 4*(appData.fileSize - sizeof( ADPCMHEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                time = 4*(appData.currPos - sizeof( ADPCMHEADER ))/(appData.sampleRate*2*appData.numOfChnls);
                break;
#endif
#ifdef MP3_DECODER_ENABLED
            case APP_STREAM_MP3:
                dur = appData.playbackDuration;
                time = ((appData.currPos-appData.mp3FirstFrame)*appData.playbackDuration)/appData.fileSize;
                break;
#endif                
            default:
                time = dur = 0;
                break;
        }
       
        if (time >= dur)
        {
            appData.state = APP_STATE_CLOSE_FILE;   // if at or past duration, force file to close
        }        
    }
}
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void _audioCodecInitialize (AUDIO_CODEC_DATA* codecData)
{
    codecData->handle = DRV_HANDLE_INVALID;
    codecData->context = 0;
    codecData->bufferHandler = 
           (DRV_CODEC_BUFFER_EVENT_HANDLER) Audio_Codec_BufferEventHandler;
    codecData->txbufferObject1 = NULL;
    codecData->txbufferObject2 = NULL;
    codecData->bufferSize1 = 0;
    codecData->bufferSize2 = 0;
}


/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{  
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */   
    appData.state = APP_STATE_INIT;
    btnData.state = BTN_STATE_IDLE;
    appData.playerBtnMode = VOL_ADJUST;
    appData.volLevel = VOL_LVL_LOW;
    appData.prevVol = 0;
    appData.volume = volumeLevels[appData.volLevel];
    appData.buttonDelay = 0;
    LED_Set_Mode( 1, LED_STATE_TOGGLE, _100ms );
    LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
#ifdef GFX_ENABLED
    appData.guiUpdate = _200ms;
    APP_Init_GUI_Strings();
#endif
    _audioCodecInitialize (&appData.codecData);
    
    appData.deviceIsConnected = false;
    appData.usbConnect = false;
    appData.pingPong = true;
    appData.headphone_out = true;
    APP_PlayerInitialize();
}

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    printf ("USB event %d\r\n",event);
    
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
        default:
            break;
    }
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void * eventData, uintptr_t context)
{
    printf("FS event %d\r\n",event);
    
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
            appData.deviceIsConnected = true;
            LED_Set_Mode( 1, LED_STATE_OFF, 0 );
            break;
            
        case SYS_FS_EVENT_UNMOUNT:
            appData.deviceIsConnected = false;
            appData.usbConnect = false;
            LED_Set_Mode( 1, LED_STATE_TOGGLE, 0 );
            LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
#ifdef GFX_ENABLED
            APP_Init_GUI_Strings();
#endif            
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
            
        default:
            break;
    }
}

bool APP_IsSupportedAudioFile(char *name)
{
    uint8_t i = strlen(name);
    while(i--)
    {
        if(name[i] == '.')
            break;
    }
            
    if(name[i] == '\0' || name[i+1] =='\0'
            || name[i+2] == '\0' || name[i+3] == '\0')
    {
        return false;
    }

    char lowercase[4];
    lowercase[0] = char_tolower(name[i+1]);
    lowercase[1] = char_tolower(name[i+2]);
    lowercase[2] = char_tolower(name[i+3]);
    lowercase[3] = '\0';
    
#ifdef WAV_STREAMING_ENABLED
    if( strcmp( lowercase, "wav" ) == 0 )
    {
        strcpy( appData.ext, "wav" );
        appData.currentStreamType = APP_STREAM_WAV;
        return true;
    }
#endif
#ifdef ADPCM_STREAMING_ENABLED
    if( strcmp( lowercase, "pcm" ) == 0 )
    {
        strcpy( appData.ext, "pcm" );
        appData.currentStreamType = APP_STREAM_ADPCM;
        return true;
    }
#endif
#ifdef MP3_DECODER_ENABLED
    if( strcmp( lowercase, "mp3" ) == 0 )
    {
        strcpy( appData.ext, "mp3" );
        appData.currentStreamType = APP_STREAM_MP3;
        return true;
    }    
#endif    
    strcpy( appData.ext, "n/a" );
    appData.currentStreamType = APP_STREAM_UNKNOWN;
    return false;
}

void APP_ValidateFile( void )
{
    switch( appData.currentStreamType )
    {
#ifdef WAV_STREAMING_ENABLED
        case APP_STREAM_WAV:
            // Read the header data
            bytesRead = SYS_FS_FileRead( appData.fileHandle, &wavHdr, sizeof(wavHdr));
            // If end of file is reached then close this file
            if ((0==bytesRead) || (-1==bytesRead) || SYS_FS_FileEOF(appData.fileHandle))
            {
                appData.state = APP_STATE_CLOSE_FILE; 
                break;
            }             
            WAV_Initialize_N( (uint8_t *)&wavHdr, appData.fileHandle );                     
            
            appData.numOfChnls = WAV_GetChannels();
            appData.bytesRemaining = WAV_GetDataLen();
            appData.sampleRate = WAV_GetSampleRate();
            appData.playbackDuration = WAV_GetDuration();
            appData.bitDepth = WAV_GetBitsPerSample();
            appData.bufferSize1 = BUFFER_SIZE;
            appData.bufferSize2 = BUFFER_SIZE;
            
            if((wavHdr.format == 0x46464952 ) && ( wavHdr.filetype == 0x45564157 ) && 
                ( wavHdr.frmtchunk_marker == 0x20746d66 ) & (wavHdr.type_frmt == 1) && 
                (appData.sampleRate >= 8000) && (appData.sampleRate <= 96000) &&
                (appData.numOfChnls == 2))
            {
                appData.validFile = true;
            }
            break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
        case APP_STREAM_ADPCM:
            bytesRead = SYS_FS_FileRead( appData.fileHandle, &adpcmHdr, sizeof(adpcmHdr));
            // If end of file is reached then close this file
            if ((0==bytesRead) || (-1==bytesRead) || SYS_FS_FileEOF(appData.fileHandle))
            {
                appData.state = APP_STATE_CLOSE_FILE; 
                break;
            }             
            ADPCM_Initialize((uint8_t *)&adpcmHdr);            
            appData.numOfChnls = ADPCM_GetChannels();
            appData.bytesRemaining = ADPCM_GetDataLen();
            appData.sampleRate = ADPCM_GetSampleRate();
            //appData.playbackDuration = adpcmHdr.extralen/adpcmHdr.bytesPerSec;
            appData.bitDepth = ADPCM__GetBitsPerSample();
            appData.bufferSize1 = BUFFER_SIZE/8;
            appData.bufferSize2 = BUFFER_SIZE/8;
            
            if((adpcmHdr.format == 0x46464952 ) && ( adpcmHdr.filetype == 0x45564157 ) && 
                ( adpcmHdr.frmtchunk_marker == 0x20746d66 ) && (adpcmHdr.type_frmt == 17) && 
                (appData.sampleRate >= 8000) && (appData.sampleRate <= 96000))
            {
                appData.validFile = true;
            }
            break;
#endif
#ifdef MP3_DECODER_ENABLED
        case APP_STREAM_MP3:
            // Read the mp3 file. Fill the first buffer of data from the input file.
            bytesRead = SYS_FS_FileRead(appData.fileHandle, MP3readBuf, READBUF_SIZE);                           
            // If end of file is reached then close this file
            if ((0==bytesRead) || (-1==bytesRead) /*|| SYS_FS_FileEOF(appData.fileHandle)*/)
            {               
                appData.state = APP_STATE_CLOSE_FILE; 
                break;
            }                  
            MP3bytesLeft = bytesRead;
            // find start of first MP3 frame - assume EOF if no sync found
            MP3offset = MP3_FindSyncWord(hMP3Decoder,MP3readPtr, MP3bytesLeft);
            if (MP3offset < 0) 
            {
                // This frame does not contain the frame sync. Get another frame.
                MP3readPtr = MP3readBuf;
                // This will call APP_ValidateFile again.
                appData.state = APP_STATE_INIT_READ_FILE;
                break;
            }

            // Update the read pointer and number of bytes left in the current frame.
            MP3readPtr += MP3offset;
            MP3bytesLeft -= MP3offset;

            // Determine the audio format from the first frame header
            //if (MP3_GetSampleRate() != 44100 || MP3_GetChannels() != 2)
            if (MP3_GetChannels() != 2)                
            {
                // This means that the MP3 file is either a mono audio stream or
                // not sampled at 44100Hz.  We currently don't want to handle
                // these type of files even if the decoder can decode them
                 appData.state = APP_STATE_CLOSE_FILE;
                 break;
            }
            
            uint8_t *scratch = (uint8_t*)MP3outBuf;  //typically need 512 bytes; will steal from outbuf
 
            read_ID3Tags(appData.fileHandle, scratch, (ID3_EVENT_HANDLER)APP_ID3_EventHandler);
            appData.bitRate = MP3_GetBitRate(appData.fileHandle, MP3offset, scratch); 
            if (0==appData.bitRate)
            {
                 appData.state = APP_STATE_CLOSE_FILE;
                 break;                
            }
            
            appData.numOfChnls = MP3_GetChannels();            
            appData.sampleRate = MP3_GetSampleRate();
            appData.bitDepth = MP3_GetBitsPerSample();          
            appData.playbackDuration = (appData.fileSize - MP3offset) / 125 / appData.bitRate;
            appData.mp3FirstFrame = MP3offset;            
    
            printf("\r\nbitrate: %d\r\n",(int)appData.bitRate);
            printf("num channels: %d\r\n",(int)appData.numOfChnls);
            printf("sample rate: %d\r\n",(int)appData.sampleRate);                    
            printf("bits/sample: %d\r\n",(int)appData.bitDepth);
            printf("duration: %d\r\n",(int)appData.playbackDuration);
            printf("first frame: %d\r\n",(int)appData.mp3FirstFrame);                                              
           
            // Start reading the MP3 file
            appData.state = APP_STATE_READ_FILE;
            appData.validFile = true;
            break;                       
#endif        
        default:
            appData.bufferSize1 = 0;
            appData.bufferSize2 = 0;
            appData.validFile = false;
            break;
    }
}

bool APP_Decoder(uint8_t * inBuf, uint16_t inSize, uint16_t * bytesRd, int16_t * outBuf, uint16_t * outSize )
{
    bool ret = false;
    switch( appData.currentStreamType )
    {
#ifdef WAV_STREAMING_ENABLED
        case APP_STREAM_WAV:
            ret = WAV_Decoder( inBuf, inSize, bytesRd, outBuf, outSize );
            break;
#endif
#ifdef ADPCM_STREAMING_ENABLED
        case APP_STREAM_ADPCM:
            ret = ADPCM_Decoder( inBuf, inSize, bytesRd, outBuf, outSize );
            break;
#endif
        default:
            break;
    }
    return ret;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
DRV_HANDLE tmrHandle;

APP_STATES oldAppState = -1;

bool firstRead = true;

bool oldPause = false;

void APP_Tasks ( void )
{  
    DISK_Tasks();
    APP_LED_Tasks();
#ifndef GFX_ENABLED
   	BTN_Tasks();
#endif
    if (oldAppState != appData.state)
    {
        if ((appData.state < APP_STATE_READ_FILE) ||
            (appData.state > APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE))
            printf("a%d ",appData.state);
        oldAppState = appData.state;
    }
    
    // check the application's current state
    switch ( appData.state )
    {       
        // application's initial state.
        case APP_STATE_INIT:
        {
            // open the timer Driver
            tmrHandle = SYS_TIME_CallbackRegisterMS(App_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
               appData.state = APP_STATE_CODEC_OPEN;
            }
            break;
        }
        case APP_STATE_CODEC_OPEN:
            {
                // see if codec is done initializing
                SYS_STATUS status = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == status)
                {
                    // This means the driver can now be be opened.
                    // a client opens the driver object to get an Handle
                    appData.codecData.handle = DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                        DRV_IO_INTENT_WRITE | DRV_IO_INTENT_EXCLUSIVE);       
                    if(appData.codecData.handle != DRV_HANDLE_INVALID)
                    {
                        appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                    }            
                }
            break;
        }
        
        
        // set a handler for the audio buffer completion event
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);
            appData.state = APP_STATE_BUS_ENABLE;           

            DRV_CODEC_VolumeSet(appData.codecData.handle,
                DRV_CODEC_CHANNEL_LEFT_RIGHT, 75/*appData.volume*/);                
            break;
            
        case APP_STATE_BUS_ENABLE:
        {
           // set the event handler and enable the bus
            SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(0);
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
        }
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
            if(USB_HOST_BusIsEnabled(0))
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        }            

        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(appData.deviceIsConnected)
            {
                appData.state = APP_STATE_DEVICE_CONNECTED;
                appData.usbConnect = true;
                LED_Set_Mode( 1, LED_STATE_OFF, _50ms );
            }
            break;

        case APP_STATE_DEVICE_CONNECTED:
            // device was mounted. We can try scanning the disk
            appData.state = APP_STATE_READY_TO_SCAN;
            break;
        
        case APP_STATE_READY_TO_SCAN:
            // Wait until the disk has been scanned for all requested file types
            //__NOP();
            break;
        
        case APP_STATE_SCANNING:
            // Wait until the next file has been chosen to open and play
            //__NOP();
            break;
        
        case APP_STATE_OPEN_FILE:
            if( !appData.totalAudioFiles )
            {
                appData.state = APP_STATE_SCANNING;
                break;
            }
            appData.pause = oldPause = false;
            LED_Set_Mode( 2, LED_STATE_ON, _50ms );
            
            printf("filename %s found\r\n",appData.fileName);
            
            if( APP_IsSupportedAudioFile( appData.fileName ))
            {
                // try opening the file for reading
                appData.fileHandle = SYS_FS_FileOpen(appData.fileName, (SYS_FS_FILE_OPEN_READ));
                if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
                {
                    // could not open the file -- 
                    appData.state = APP_STATE_CLOSE_FILE;
                }
                else
                {
                    appData.fileSize = SYS_FS_FileSize( appData.fileHandle );
                    if( appData.fileSize == -1 )
                    {
                        appData.state = APP_STATE_CLOSE_FILE;
                    }
                    else
                    {
                        // Read the header data
                        appData.validFile = false;
                        
                        appData.lrSync = true;  
#ifdef MP3_DECODER_ENABLED
                        rd = wrtn = 0; // avoid undefined if WAV etc not enabled
                        rd++; wrtn++;                        
                        if (APP_STREAM_MP3==appData.currentStreamType)
                        {

                            // if MP3 file ...
                            hMP3Decoder = MP3InitDecoder();
                            MP3bytesLeft = 0;
                            MP3readPtr = MP3readBuf;
                        }
#endif                        
                        appData.pingPong = true;
                        firstRead = true;                          
                        appData.state = APP_STATE_INIT_READ_FILE;                                              
                    }
                }
            }
            else 
            {
                appData.state = APP_STATE_SCANNING;
            }
            break;
        case APP_STATE_INIT_READ_FILE:
            
            APP_ValidateFile();         
                    
            if( !appData.validFile )
            {
                // Look for next file
                appData.state = APP_STATE_CLOSE_FILE;
            }
            else
            { 
                DRV_CODEC_SamplingRateSet(appData.codecData.handle, appData.sampleRate);
                appData.state = APP_STATE_READ_FILE;
            }
            break;
            
        case APP_STATE_READ_FILE:
            if (appData.pause)
            {
                if (oldPause==false)
                {
                    DRV_I2S_WriteQueuePurge(DRV_CODEC_GetI2SDriver(appData.codecData.handle));                
                }
                oldPause = appData.pause;                
                break;
            }
            else
            {
                if (oldPause==true)
                {
                    firstRead = true;                
                }
                oldPause = appData.pause;                
            }

#ifdef GFX_ENABLED
            laImageWidget_SetImage(PlayStopBtn, &pauseBtn); 
#endif
                
#ifdef MP3_DECODER_ENABLED                       
            if (APP_STREAM_MP3==appData.currentStreamType)
            {            
                if (MP3bytesLeft < READBUF_SIZE) 
                {
                    bytesRead = MP3_FillReadBuffer(MP3readBuf, MP3readPtr, READBUF_SIZE, 
                            MP3bytesLeft, appData.fileHandle);
                    if ((0==bytesRead) || (-1==bytesRead) || SYS_FS_FileEOF(appData.fileHandle))
                    {
                        // read was not successful or no data read -- assume end of file
                        appData.state = APP_STATE_CLOSE_FILE;
                        break;
                    }                                       
                    MP3bytesLeft += bytesRead;
                    MP3readPtr = MP3readBuf;
                }            
                // Decode the frame */
                int32_t err = MP3Decode(hMP3Decoder, &MP3readPtr,(int*)&MP3bytesLeft, MP3outBuf, 0);
                if (err) 
                {
                    /* error occurred */
                    switch (err) 
                    {
                        case -1:
                        case -3:
                        default:
                            appData.state = APP_STATE_CLOSE_FILE;
                            break;
                        case -2:
                            /* More data is needed. Read the file. */
                            appData.state = APP_STATE_READ_FILE;
                            break;
                        case -6:
                            appData.state = APP_STATE_INIT_READ_FILE;
                            break;
                    }
                    break;
                } 

                int i, j;
                j = 0;
                if (appData.pingPong)     // will write to I2S from buffer 1    
                {
#ifndef DATA32_ENABLED 
                    for (i=0; i < 2*NUM_MP3_OUTPUT_SAMPLES;)
                    {
#ifdef SWAPCHANNELS                         
                        App_Audio_Output_Buffer1[j].leftData = (int16_t)MP3outBuf[i+1];
                        App_Audio_Output_Buffer1[j].rightData = (int16_t)MP3outBuf[i];
#else
                        App_Audio_Output_Buffer1[j].leftData = (int16_t)MP3outBuf[i];
                        App_Audio_Output_Buffer1[j].rightData = (int16_t)MP3outBuf[i+1];                        
#endif
                        i += 2;
                        j++;
                    }                    
                    appData.codecData.bufferSize1 = 4*NUM_MP3_OUTPUT_SAMPLES;   // # of bytes                    
#else
                    for (i=0; i < 2*NUM_MP3_OUTPUT_SAMPLES;
                    {
#ifdef SWAPCHANNELS                         
                        App_Audio_Output_Buffer1[j].leftData = (int32_t)MP3outBuf[i+1]<<16;
                        App_Audio_Output_Buffer1[j].rightData = (int32_t)MP3outBuf[i]<<16;
#else
                        App_Audio_Output_Buffer1[j].leftData = (int32_t)MP3outBuf[i]<<16;
                        App_Audio_Output_Buffer1[j].rightData = (int32_t)MP3outBuf[i+1]<<16;                        
#endif                        
                        i += 2;
                        j++;                        
                    }
                    appData.codecData.bufferSize1 = 8*NUM_MP3_OUTPUT_SAMPLES;   // # of bytes                    
#endif
                    appData.codecData.txbufferObject1 = (uint8_t *)App_Audio_Output_Buffer1;
                }
                else
                {
#ifndef DATA32_ENABLED 
                    for (i=0; i < 2*NUM_MP3_OUTPUT_SAMPLES;)
                    {
#ifdef SWAPCHANNELS                         
                        App_Audio_Output_Buffer2[j].leftData = (int16_t)MP3outBuf[i+1];
                        App_Audio_Output_Buffer2[j].rightData = (int16_t)MP3outBuf[i];
#else
                        App_Audio_Output_Buffer2[j].leftData = (int16_t)MP3outBuf[i];
                        App_Audio_Output_Buffer2[j].rightData = (int16_t)MP3outBuf[i+1];                        
#endif                        
                        i += 2;
                        j++;
                    }
                    appData.codecData.bufferSize2 = 4*NUM_MP3_OUTPUT_SAMPLES; 
#else
                    for (i=0; i < 2*NUM_MP3_OUTPUT_SAMPLES;)
                    {
#ifdef SWAPCHANNELS                         
                        App_Audio_Output_Buffer2[j].leftData = (int32_t)MP3outBuf[i+1]<<16;
                        App_Audio_Output_Buffer2[j].rightData = (int32_t)MP3outBuf[i]<<16;
#else
                        App_Audio_Output_Buffer2[j].leftData = (int32_t)MP3outBuf[i]<<16;
                        App_Audio_Output_Buffer2[j].rightData = (int32_t)MP3outBuf[i+1]<<16;                        
#endif
                        i += 2;
                        j++;                        
                    }
                    appData.codecData.bufferSize2 = 8*NUM_MP3_OUTPUT_SAMPLES;                     
#endif                   
                    appData.codecData.txbufferObject2 = (uint8_t *)App_Audio_Output_Buffer2;               
                }
            }
            else
#endif
			// not MP3
            {
                bytesRead = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer, appData.bufferSize1);
                if ((0==bytesRead) || (-1==bytesRead) || SYS_FS_FileEOF(appData.fileHandle))
                {
                    // read was not successful or no data read -- assume end of file
                    appData.state = APP_STATE_CLOSE_FILE;
                }
                else
                {
                    if (appData.pingPong)     // will write to I2S from buffer 1    
                    {                    
#ifndef DATA32_ENABLED
                        APP_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)bytesRead, &rd,
                                (int16_t *) App_Audio_Output_Buffer1, &wrtn);
#ifdef SWAPCHANNELS                       
                        int i;
                        for( i = 0; i < NUM_SAMPLES; i++ )
                        {
                            uint16_t temp = App_Audio_Output_Buffer1[i].leftData;
                            App_Audio_Output_Buffer1[i].leftData = App_Audio_Output_Buffer1[i].rightData;
                            App_Audio_Output_Buffer1[i].rightData = temp;
                        }
#endif
#else
                        APP_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)bytesRead, &rd,
                                (int16_t *) App_Audio_Output_TempBuf, &wrtn);
                        int i;
                        for( i = 0; i < NUM_SAMPLES; i++ )
                        {
#ifdef SWAPCHANNELS                             
                            App_Audio_Output_Buffer1[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;
                            App_Audio_Output_Buffer1[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
#else
                            App_Audio_Output_Buffer1[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
                            App_Audio_Output_Buffer1[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;                            
#endif
                        }
                        wrtn *= 2;
#endif
                        appData.codecData.txbufferObject1 = (uint8_t *)App_Audio_Output_Buffer1;
                        appData.codecData.bufferSize1 = wrtn;
                    }
                    else
                    {
#ifndef DATA32_ENABLED
                        APP_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)bytesRead, &rd,
                                (int16_t *) App_Audio_Output_Buffer2, &wrtn);
#ifdef SWAPCHANNELS                       
                        int i;
                        for( i = 0; i < NUM_SAMPLES; i++ )
                        {
                            uint16_t temp = App_Audio_Output_Buffer2[i].leftData;
                            App_Audio_Output_Buffer2[i].leftData = App_Audio_Output_Buffer2[i].rightData;
                            App_Audio_Output_Buffer2[i].rightData = temp;
                        }
#endif                        
#else
                        APP_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)bytesRead, &rd,
                                (int16_t *) App_Audio_Output_TempBuf, &wrtn);
                        int i;
                        for( i = 0; i < NUM_SAMPLES; i++ )
                        {
#ifdef SWAPCHANNELS                             
                            App_Audio_Output_Buffer2[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;
                            App_Audio_Output_Buffer2[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
#else
                            App_Audio_Output_Buffer2[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
                            App_Audio_Output_Buffer2[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;                            
#endif                            
                        }
                        wrtn *= 2;
#endif
                        appData.codecData.txbufferObject2 = (uint8_t *) App_Audio_Output_Buffer2;
                        appData.codecData.bufferSize2 = wrtn;                        
                    }
                }               
            }
            
            if (firstRead)
            {
                appData.state = APP_STATE_CODEC_ADD_BUFFER;
                firstRead = false;
            }
            else
            {
                appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;    
            }
            break;
            
        case APP_STATE_CODEC_ADD_BUFFER:
            if (appData.lrSync)            
            {
                DRV_CODEC_LRCLK_Sync(appData.codecData.handle);
#ifndef ALWAYS_DO_LRSYNC                
                appData.lrSync = false;
#endif                
            }
            
            if(!appData.deviceIsConnected)
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
                break;
            }            
            
            if ( appData.pingPong )     // writing from 1, will decode  into 2     
            {
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                                            &appData.codecData.writeBufHandle1,
                                            appData.codecData.txbufferObject1,
                                            appData.codecData.bufferSize1);

                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle1 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = false;
                    appData.state = APP_STATE_READ_FILE;
                }
                else
                {
                    appData.state = APP_STATE_CLOSE_FILE;
                }
            }
            else
            {                
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                                            &appData.codecData.writeBufHandle2,
                                            appData.codecData.txbufferObject2,
                                            appData.codecData.bufferSize2);
                
                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle2 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = true;
                    appData.state = APP_STATE_READ_FILE;
                }               
                else
                {
                    appData.state = APP_STATE_CLOSE_FILE;
                }
            }
            appData.currPos = SYS_FS_FileTell( appData.fileHandle );            
#ifdef GFX_ENABLED
            APP_Update_GUI_Tasks();
#else
            APP_UpdateTrackPosition();            
#endif
            if( appData.prevVol != appData.volume )
            {
                DRV_CODEC_VolumeSet(appData.codecData.handle,
                    DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);
                appData.prevVol = appData.volume;
            }            
            break;                                  
            
        // audio data Transmission under process
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
            //__NOP();
            break;
            
        case APP_STATE_CLOSE_FILE:
#ifdef GFX_ENABLED
            APP_Init_GUI_Strings();
#endif            
            printf("closing the file\r\n");
            SYS_FS_FileClose( appData.fileHandle );
            DRV_I2S_WriteQueuePurge(DRV_CODEC_GetI2SDriver(appData.codecData.handle));          
            appData.state = APP_STATE_SCANNING;
            break;
            
        case APP_STATE_CLOSE_AND_REOPEN_FILE:
#ifdef GFX_ENABLED
            APP_Init_GUI_Strings();
#endif            
            printf("closing and reopening the file\r\n");
            SYS_FS_FileClose( appData.fileHandle );
            DRV_I2S_WriteQueuePurge(DRV_CODEC_GetI2SDriver(appData.codecData.handle));          
            appData.state = APP_STATE_OPEN_FILE;
            break;            
        
        case APP_STATE_NO_MEDIA:
#ifdef GFX_ENABLED
            APP_Init_GUI_Strings();
#endif             
            appData.state = APP_STATE_SCANNING;
            appData.pause = true;           
		default:
            //__NOP();
            break;
    }
}

#ifdef GFX_ENABLED
void APP_Update_GUI_Tasks( void )
{
    if( !appData.guiUpdate )
    {
        switch( appData.guiState )
        {
            case GUI_STATE_VOLUME:
                APP_Get_GUI_Volume();
                break;
            case GUI_STATE_BIT_DEPTH:
                APP_Set_GUI_BitDepthStr();
                break;
            case GUI_STATE_TRACK_POSITION:
                APP_Set_GUI_TrackPositionStr();
                break;
            case GUI_STATE_FILENAME:
                APP_Set_GUI_FileNameStr();
                break;
            case GUI_STATE_CHANNELS:
                APP_Set_GUI_ChnlStr();
                break;
            case GUI_STATE_SAMPLE_RATE:
                APP_Set_GUI_SampleRateStr();
            default:
                break;
        }
        appData.guiState++;
        appData.guiState %= GUI_STATE_MAX;
        appData.guiUpdate = _20ms;
    }
}
#endif

void APP_LED_Tasks( void )
{
    if( !ledData.led1Delay )
    {
        switch( ledData.led1State )
        {
            case LED_STATE_ON:
                LED1_On();
                ledData.led1On = true;
                ledData.led1Delay = ledData.led1Period;
                break;
            case LED_STATE_OFF:
            default:
                LED1_Off();
                ledData.led1On = false;
                ledData.led1Delay = ledData.led1Period;
                break;
            case LED_STATE_TOGGLE:
                LED1_Toggle();
                ledData.led1On = !ledData.led1On;
                ledData.led1Delay = ledData.led1Period;
                break;
            case LED_STATE_BLINK:
                ledData.led1Delay = ( ledData.led1BlinkCnt ) ? _200ms : _500ms;
                if( ledData.led1On )
                {
                    LED1_Off();
                    ledData.led1On = false;
                    ledData.led1Blinks = ( ledData.led1Blinks ) ? : 1;
                    ledData.led1BlinkCnt = ( ledData.led1BlinkCnt ) ? : ledData.led1Blinks;
                }
                else
                {
                    LED1_On();
                    ledData.led1On = true;
                    ledData.led1BlinkCnt--;
                }
                break;
        }
        
    }
    if( !ledData.led2Delay )
    {
        switch( ledData.led2State )
        {
            case LED_STATE_ON:
                LED2_On();
                ledData.led2On = true;
                ledData.led2Delay = ledData.led2Period;
                break;
            case LED_STATE_OFF:
            default:
                LED2_Off();
                ledData.led2On = false;
                ledData.led2Delay = ledData.led2Period;
                break;
            case LED_STATE_TOGGLE:
                LED2_Toggle();
                ledData.led2On = !ledData.led2On;
                ledData.led2Delay = ledData.led2Period;
                break;
            case LED_STATE_BLINK:
                ledData.led2Delay = ( ledData.led2BlinkCnt ) ? _200ms : _500ms;
                if( ledData.led2On )
                {
                    LED2_Off();
                    ledData.led2On = false;
                    ledData.led2Blinks = ( ledData.led2Blinks ) ? : 1;
                    ledData.led2BlinkCnt = ( ledData.led2BlinkCnt ) ? : ledData.led2Blinks;
                }
                else
                {
                    LED2_On();
                    ledData.led2On = true;
                    ledData.led2BlinkCnt--;
                }
                break;
        }
    }
}

void LED_Set_Mode( uint8_t led, LED_STATES state, uint32_t prd_blinks)
{
    switch( state )
    {
        case LED_STATE_ON:
        case LED_STATE_OFF:
        case LED_STATE_TOGGLE:
        default:
            switch( led )
            {
                case 1:
                    ledData.led1State = state;
                    ledData.led1Period = ( prd_blinks <= LED_PRD_MAX ) ? prd_blinks = ( prd_blinks > LED_PRD_MIN )?
                        prd_blinks : LED_PRD_MIN : LED_PRD_MAX;
                    break;
                case 2:
                    ledData.led2State = state;
                    ledData.led2Period = ( prd_blinks <= LED_PRD_MAX ) ? prd_blinks = ( prd_blinks > LED_PRD_MIN )?
                        prd_blinks : LED_PRD_MIN : LED_PRD_MAX;
                default:
                    break;
            }
            break;
        case LED_STATE_BLINK:
            switch( led )
            {
                case 1:
                    ledData.led1State = state;
                    ledData.led1Blinks = ( !prd_blinks ) ? ledData.led1Blinks : prd_blinks;
                    ledData.led1BlinkCnt = ledData.led1Blinks;
                    break;
                case 2:
                    ledData.led2State = state;
                    ledData.led2Blinks = ( !prd_blinks ) ? ledData.led2Blinks : prd_blinks;
                    ledData.led2BlinkCnt = ledData.led2Blinks;
                default:
                    break;
            }
            break;
    }
}

void BTN_ShortPress( void )
{
    switch( appData.playerBtnMode )
    {
        case VOL_ADJUST:
            appData.volLevel++;
            appData.volLevel %= VOL_LVL_MAX;
            if( appData.volLevel == VOL_LVL_MUTE )
            {
                LED_Set_Mode( 1, LED_STATE_TOGGLE, _500ms );
            }
            else
            {
                LED_Set_Mode( 1, LED_STATE_OFF, _500ms );
            }
            appData.volume = volumeLevels[appData.volLevel];
            break;
        case LINEAR_TRACK_CHANGE:
            appData.state = APP_STATE_CLOSE_FILE;
            break;
        default:
            break;
    }
}

void BTN_LongPress( void )
{
    appData.playerBtnMode++;
    appData.playerBtnMode %= MAX_MODES;
    if( appData.playerBtnMode == LINEAR_TRACK_CHANGE )
    {
        LED_Set_Mode( 1, LED_STATE_ON, _50ms );
    }
    else
    {
        LED_Set_Mode( 1, LED_STATE_OFF, _50ms );
    }
}
    
    
void BTN_Tasks()
{
   //BUTTON PROCESSING
    /* Check the buttons' current state. */
    switch ( btnData.state )
    {
        case BTN_STATE_IDLE:
            if(( btnData.delay == 0 ) &&
                 ( SWITCH_Get() == SWITCH_STATE_PRESSED ))                
            {
                btnData.delay = BUTTON_DEBOUNCE;       
                btnData.state = BTN_STATE_PRESSED;               
            }
            break;

        case BTN_STATE_PRESSED:
            if( btnData.delay > 0 )
            {
                break;      // still debouncing
            }

            if( SWITCH_Get() == SWITCH_STATE_PRESSED ) 
            {                
                btnData.delay = LONG_BUTTON_PRESS;          // 1 sec is long press
                btnData.state = BTN_STATE_BUTTON0_PRESSED;                  
            }
            break;

        case BTN_STATE_BUTTON0_PRESSED:
            if(( btnData.delay > 0 ) &&
                ( SWITCH_Get() != SWITCH_STATE_PRESSED ))     // SW01 pressed and released < 1 sec
            {
                BTN_ShortPress();
                btnData.delay = BUTTON_DEBOUNCE;                
                btnData.state = BTN_STATE_IDLE;    
                
            }
            else if(( btnData.delay == 0 ) &&
                     ( SWITCH_Get() == SWITCH_STATE_PRESSED ))  // SW0 still pressed after 1 sec
            {
                BTN_LongPress();
                btnData.state = BTN_STATE_WAIT_FOR_RELEASE;                
            }                          
            break;

        case BTN_STATE_WAIT_FOR_RELEASE:
            if( SWITCH_Get() != SWITCH_STATE_PRESSED )
            {
                btnData.delay = BUTTON_DEBOUNCE;
                btnData.state = BTN_STATE_IDLE;
            }

        /* The default state should never be executed. */
        default:
            /* TODO: Handle error in application's state machine. */
            break;
    }
}
//******************************************************************************
// 
// Audio_Codec_TxBufferComplete() - Set APP_Tasks Next state to buffer complete.
//
// NOTE: Called from APP_CODECBufferEventHandler().
//
// TODO: Put the appData instance pointer in the AUDIO_CODEC_DATA instance on
//       initialization of codecData and let the audio_codec instance change
//       the codec data and state. CAL
//
//******************************************************************************
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
    DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context)
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
            // Signal to APP that DMA has finished
            appData.state = APP_STATE_CODEC_ADD_BUFFER;
//                printf("!");
            break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        case DRV_CODEC_BUFFER_EVENT_ABORT:
            appData.state = APP_STATE_SCANNING;
            break;
    }
}

bool DISK_EventHandler ( DISK_EVENT event, uint32_t variable,SYS_FS_HANDLE fileHandle)
{
    printf("disk event %d\r\n",event);
    
    switch ( event )
    {
        case DISK_EVENT_REMOVED:
            appData.state = APP_STATE_NO_MEDIA;
            return ( true );

        case DISK_EVENT_INSERTED:
            appData.state = APP_STATE_WAITING;
            return ( true );

        case DISK_EVENT_SCANNING_STARTED:
            appData.state = APP_STATE_SCANNING;
            return ( true );

        case DISK_EVENT_SCANNING_FINISHED:
            return ( true );

        case DISK_EVENT_TRACK_CHANGED:
            return ( true );

        case DISK_EVENT_FILE_OPEN_ERROR:
        case DISK_EVENT_END_OF_FILE:
            DISK_CloseFile(appData.fileHandle);
            return ( true );
            
        default:
            break;
    }
    return ( false );
}

#ifdef MP3_DECODER_ENABLED
void APP_ID3_EventHandler(int event, uint32_t data)
{
#ifdef GFX_ENABLED
    laTmpStr = laString_CreateFromCharBuffer( (char*)data, 
        GFXU_StringFontIndexLookup(&stringTable, string_StringChars, 0));        
    switch ( event )
    {
        case ID3_EVENT_TAG_ARTIST:
            printf("Artist: %s\r\n",(char*)data);
            laLabelWidget_SetText(txtArtist, laTmpStr);
            break;
        case ID3_EVENT_TAG_ALBUM:
            printf("Album: %s\r\n",(char*)data);
            laLabelWidget_SetText(txtAlbumTitle, laTmpStr);
            break;
        case ID3_EVENT_TAG_TITLE:
            printf("Title: %s\r\n",(char*)data);
            laLabelWidget_SetText(txtTrackTitle, laTmpStr);
            break;
    }
    laString_Destroy( &laTmpStr );
#endif
}
#endif

/*******************************************************************************
 End of File
 */
