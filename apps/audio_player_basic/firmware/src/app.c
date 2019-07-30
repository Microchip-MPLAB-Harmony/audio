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
#ifdef GFX_ENABLED
#include "gfx/libaria/libaria_init.h"
#include "gfx/libaria/inc/libaria_widget_circular_slider.h"
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
uint16_t volumeLevels[VOL_LVL_MAX] =
{
    0,      // Mute
    16,     // -66 dB   : VOL_LVL_LOW
    64,     // -48 dB   : VOL_LVL_MED
    255     // 0 dB     : VOL_LVL_HIGH
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

static WAV_FILE_HEADER wavHdr;
extern WAV_DEC wavDecoder;

#ifdef GFX_ENABLED
extern laCircularSliderWidget * VolumeWidget;
extern laProgressBarWidget * ProgressBarWidget;
extern laLabelWidget * lblSampleRate;
extern laLabelWidget * lblChannels;
extern laLabelWidget * lblBitDepth;
extern laLabelWidget * lblTrackTitle;
extern laLabelWidget * lblVolumePercent;
extern laLabelWidget * lblCurrPosition;
extern laLabelWidget * lblEndPosition;

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
USB_DATA usbData;
BTN_DATA btnData;

static uint16_t rd, wrtn;

/* This is the string that will written to the file */
const uint8_t writeData[12]  __attribute__((aligned(16))) = "Hello World ";

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
    if (appData.playbackDelay)
    {
        appData.playbackDelay--;
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


APP_DECODER_TYPE APP_GetCurrentFileType ( char *ext )
{
    char lowercase[4];
    if(ext[0] == '\0' || ext[1] =='\0'
            || ext[2] == '\0')
    {
        return APP_DECODER_UNKNOWN;
    }

    
    lowercase[0] = char_tolower(ext[0]);
    lowercase[1] = char_tolower(ext[1]);
    lowercase[2] = char_tolower(ext[2]);
    lowercase[3] = '\0';
    
#ifdef WAV_STREAMING_ENABLED
    if(strcmp(lowercase, "wav")==0)
    {
        return APP_DECODER_WAV;
    }
#endif
    
    return APP_DECODER_UNKNOWN;
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
void APP_Set_GUI_BitDepthStr( void )
{
    static char bdStr[5];
    if( appData.bit_depth )
    {
        switch( appData.bit_depth )
        {
            case 8:
                strcpy( bdStr, "8b");
                break;
            case 16:
            default:
                strcpy( bdStr, "16b");
                break;
        }
        laTmpStr = laString_CreateFromCharBuffer( bdStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( lblBitDepth, laTmpStr );
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
        laLabelWidget_SetText( lblSampleRate, laTmpStr );
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
            laLabelWidget_SetText( lblVolumePercent, laTmpStr );
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
        laLabelWidget_SetText( lblChannels, laTmpStr );
        laString_Destroy( &laTmpStr );
    }
}

void APP_Set_GUI_TrackPositionStr( void )
{
    uint32_t time, mins, secs;
    static char totalTimeStr[10];
    static char currTimeStr[10];
    if( appData.sampleRate && appData.numOfChnls )
    {
        time = (appData.fileSize - sizeof( WAV_FILE_HEADER))/(appData.sampleRate*2*appData.numOfChnls);

        mins = time / 60;
        secs = time % 60;
        sprintf( totalTimeStr, "%02d:%02d", (int)mins, (int)secs);
        laTmpStr = laString_CreateFromCharBuffer( totalTimeStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( lblEndPosition, laTmpStr );
        laString_Destroy( &laTmpStr );

        time = (appData.currPos - sizeof( WAV_FILE_HEADER))/(appData.sampleRate*2*appData.numOfChnls);
        mins = time / 60;
        secs = time % 60;
        sprintf( currTimeStr, "%02d:%02d", (int)mins, (int)secs);
        laTmpStr = laString_CreateFromCharBuffer( currTimeStr, 
                                GFXU_StringFontIndexLookup( &stringTable, string_StringChars, 0 ));
        laLabelWidget_SetText( lblCurrentPosition, laTmpStr );
        laString_Destroy( &laTmpStr );
        
        laProgressBarWidget_SetValue( ProgressBarWidget, (appData.currPos - sizeof( WAV_FILE_HEADER))*100/(appData.fileSize- sizeof( WAV_FILE_HEADER)) );
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
        laLabelWidget_SetText( lblTrackTitle, laTmpStr );
        laString_Destroy( &laTmpStr );
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
    appData.state = APP_STATE_IDLE;
    usbData.state = USB_STATE_INIT;
	usbData.deviceIsConnected = false;
    btnData.state = BTN_STATE_IDLE;
    appData.volLevel = VOL_LVL_LOW;
    appData.prevVol = 0;
    appData.volume = volumeLevels[appData.volLevel];
    appData.buttonDelay = 0;
    LED_Set_Mode( 1, LED_STATE_TOGGLE, _100ms );
    LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
    appData.playbackDelay = _1sec;
#ifdef GFX_ENABLED
    appData.guiUpdate = _200ms;
#endif
    _audioCodecInitialize (&appData.codecData);
    
    appData.pingPong = true;
    appData.headphone_out = true;
    APP_PlayerInitialize();
}

#ifndef USE_SDMMC
USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
        default:
            break;
    }
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
            usbData.deviceIsConnected = true;
            LED_Set_Mode( 1, LED_STATE_OFF, 0 );
            break;
            
        case SYS_FS_EVENT_UNMOUNT:
            usbData.deviceIsConnected = false;
            appData.usbConnect = false;
            usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            LED_Set_Mode( 1, LED_STATE_TOGGLE, 0 );
            LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
            
        default:
            break;
    }
}
#endif

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
        return true;
    }
#endif
    return false;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
DRV_HANDLE tmrHandle;

void APP_Tasks ( void )
{
    int retval = 0;
    
    USB_Tasks();
    DISK_Tasks();
    APP_LED_Tasks();
#ifndef GFX_ENABLED
   	BTN_Tasks();
#endif
    
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
                        //srand(appData.playbackDelay);
                    }            
                }
            }
            break;
        
        
        // set a handler for the audio buffer completion event
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;

        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(usbData.deviceIsConnected)
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
            break;
        
        case APP_STATE_SCANNING:
            // Wait until the next file has been chosen to open and play
            break;
        
        case APP_STATE_OPEN_FILE:
            if( !appData.totalAudioFiles )
            {
                appData.state = APP_STATE_SCANNING;
                break;
            }
            appData.pause = false;
            LED_Set_Mode( 2, LED_STATE_ON, _50ms );
            // try opening the file for reading
            appData.fileHandle = SYS_FS_FileOpen(appData.fileName, (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                // could not open the file -- 
                appData.state = APP_STATE_SCANNING;
            }
            else
            {
                appData.fileSize = SYS_FS_FileSize( appData.fileHandle );
                if( appData.fileSize == -1 )
                {
                    appData.state = APP_STATE_SCANNING;
                }
                else
                {
                    // file opened successfully -- read from file
                    appData.state = APP_STATE_READ_FILE;
                    // Read the header data
                    retval = SYS_FS_FileRead( appData.fileHandle, &wavHdr, sizeof(wavHdr));
                    WAV_Initialize_N( (uint8_t *)&wavHdr, appData.fileHandle );
                }
            }
            break;

        case APP_STATE_READ_FILE:
            
            // Go to next track if it's not a WAVE header or the data is not in PCM format or 8000 > samplerate > 96000
            appData.numOfChnls = WAV_GetChannels();
            appData.bytesRemaining = WAV_HdrGetDataLen();
            appData.sampleRate = WAV_GetSampleRate();
            appData.playbackDuration = WAV_GetDuration();
            appData.bit_depth = wavHdr.bitsPerSample;
            appData.codecData.bufferSize1 = BUFFER_SIZE;
            appData.codecData.bufferSize2 = BUFFER_SIZE;
            
            if((strncmp(wavHdr.chunkID, "RIFF", 4) != 0) || (strncmp(wavHdr.format, "WAVE", 4) != 0) || 
                    (strncmp(wavHdr.subChunk1Id, "fmt ", 4) != 0 ) || (strncmp(wavHdr.subChunk2Id, "data", 4) != 0 ) ||
                    (wavHdr.audioFormat != 1) || (appData.sampleRate < 8000) || (appData.sampleRate > 96000))
            {
                appData.state = APP_STATE_SCANNING;
                break;
            }
            
            // Set sample rate to what was found in the header
            DRV_CODEC_SamplingRateSet(appData.codecData.handle, appData.sampleRate);
            
            retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer, appData.codecData.bufferSize1);
            if ( (retval == -1) || (retval == 0) )
            {
                // read was not successful or no data read -- assume end of file
                appData.state = APP_STATE_SCANNING;
            }
            else
            {
#ifndef DATA32_ENABLED
                WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                        (int16_t *) App_Audio_Output_Buffer1, &wrtn);
#else
                WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                        (int16_t *) App_Audio_Output_TempBuf, &wrtn);
                for( int i = 0; i < NUM_SAMPLES; i++ )
                {
                    App_Audio_Output_Buffer1[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
                    App_Audio_Output_Buffer1[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;
                }
                wrtn *= 2;
#endif
                appData.codecData.txbufferObject1 = (uint8_t *)App_Audio_Output_Buffer1;
                //appData.codecData.bufferSize1 = wrtn;
                appData.lrSync = true;
                appData.state = APP_STATE_CODEC_ADD_BUFFER;
            }
            break;        
       
        case APP_STATE_CODEC_ADD_BUFFER:
            if(appData.playbackDelay)
                break;
            // Don't try to r/w more data if the storage device is not mounted
            if(!usbData.deviceIsConnected)
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
                break;
            }
            if ( appData.pause || (appData.volLevel == VOL_LVL_MUTE))
            {
                break;      // paused
            }
            
            if(appData.lrSync)
            {
                DRV_CODEC_LRCLK_Sync(appData.codecData.handle);
#ifndef USE_SDMMC
                appData.lrSync = false;
#endif
            }
            if ( appData.pingPong )
            {
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                                            &appData.codecData.writeBufHandle1,
                                            appData.codecData.txbufferObject1,
                                            wrtn);

                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle1 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = false;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer, appData.codecData.bufferSize2);
                    if ( (retval == -1) || (retval == 0) )
                    {
                        // read was not successful or no data read -- assume end of file
                        appData.state = APP_STATE_SCANNING;
                    }
                    else
                    {
#ifndef DATA32_ENABLED
                        WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                                (int16_t *) App_Audio_Output_Buffer2, &wrtn);
#else
                        WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                                (int16_t *) App_Audio_Output_TempBuf, &wrtn);
                        for( int i = 0; i < NUM_SAMPLES; i++ )
                        {
                            App_Audio_Output_Buffer2[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
                            App_Audio_Output_Buffer2[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;
                        }
                        wrtn *= 2;
#endif
                        appData.codecData.txbufferObject2 = (uint8_t *) App_Audio_Output_Buffer2;
                        //appData.codecData.bufferSize2 = wrtn;
                    }                    
                    
                }
                else
                {
                    //appData.state = APP_STATE_ERROR;
                    // this one needs to be modified
                    appData.state = APP_STATE_SCANNING;
                }
            }
            else
            {                
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                                            &appData.codecData.writeBufHandle2,
                                            appData.codecData.txbufferObject2,
                                            wrtn);
                
                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle2 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = true;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    // try reading the file            
                    retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer, appData.codecData.bufferSize1);
                    if ( (retval == -1) || (retval == 0) )
                    {
                        // read was not successful or no data read -- assume end of file
                        // this one needs to be modified
                        appData.state = APP_STATE_SCANNING;
                    }
                    else
                    {
#ifndef DATA32_ENABLED
                        WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                                (int16_t *) App_Audio_Output_Buffer1, &wrtn);
#else
                        WAV_Decoder( (uint8_t *)App_Audio_Input_Buffer, (uint16_t)retval, &rd,
                                (int16_t *) App_Audio_Output_TempBuf, &wrtn);
                        // The library only deals with 16 bit data.  32 data needs to be manipulated..
                        for( int i = 0; i < NUM_SAMPLES; i++ )
                        {
                            App_Audio_Output_Buffer1[i].leftData = (int32_t)App_Audio_Output_TempBuf[i].leftData<<16;
                            App_Audio_Output_Buffer1[i].rightData = (int32_t)App_Audio_Output_TempBuf[i].rightData<<16;
                        }
                        wrtn *= 2;
#endif
                        appData.codecData.txbufferObject1 = (uint8_t *)App_Audio_Output_Buffer1;
                    }                    
                }               
                else
                {
                    appData.state = APP_STATE_SCANNING;
                }
            }
#ifdef GFX_ENABLED
            APP_Update_GUI_Tasks();
#endif
            if( appData.prevVol != appData.volume )
            {
                DRV_CODEC_VolumeSet(appData.codecData.handle,
                    DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);
                appData.prevVol = appData.volume;
            }
            appData.currPos = SYS_FS_FileTell( appData.fileHandle );
            break;

        // audio data Transmission under process
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
            break;
            
        case APP_STATE_CLOSE_FILE:
            SYS_FS_FileClose( appData.fileHandle );
            appData.state = APP_STATE_SCANNING;
            break;
        
        case APP_STATE_NO_MEDIA:
            appData.state = APP_STATE_SCANNING;
            appData.pause = true;
		default:
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



void USB_Tasks ( void )
{
    switch(usbData.state)
    {
        case USB_STATE_INIT:
        {
            usbData.state = USB_STATE_BUS_ENABLE;
            break;
        }
        case USB_STATE_BUS_ENABLE:
        {
           // set the event handler and enable the bus
#ifndef USE_SDMMC
            SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(0);
            usbData.state = USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
#else
            usbData.state = USB_STATE_MOUNT_DISK;
#endif
            break;
        }
        case USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
#ifndef USE_SDMMC
            if(USB_HOST_BusIsEnabled(0))

            {
                usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
#endif
            break;
        }

#ifdef USE_SDMMC            
        case USB_STATE_MOUNT_DISK:
            if( SYS_FS_Mount( SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) == SYS_FS_RES_SUCCESS )
            {
                usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
                usbData.deviceIsConnected = true;
                LED_Set_Mode( 1, LED_STATE_OFF, _50ms );
            }
            break;
#endif

        case USB_STATE_WAIT_FOR_DEVICE_ATTACH:
        {
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(usbData.deviceIsConnected)
            {
                usbData.state = USB_STATE_DEVICE_CONNECTED;
            }
            break;
        }
        case USB_STATE_DEVICE_CONNECTED:
        {
            // device was mounted. We can r/w the disk
            usbData.state = USB_STATE_IDLE;
            appData.state = APP_STATE_INIT;
            break;
        }
        case USB_STATE_IDLE:
        default:
        {
            break;
        }
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
            SYS_FS_FileSeek( appData.fileHandle, 0, SYS_FS_SEEK_END );
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
            //Signal to APP that Tx is complete.
                appData.state = APP_STATE_CODEC_ADD_BUFFER; 
            break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        case DRV_CODEC_BUFFER_EVENT_ABORT:
            appData.state = APP_STATE_SCANNING;
            break;
    }
}

bool DISK_EventHandler ( DISK_EVENT event, uint32_t variable,SYS_FS_HANDLE fileHandle)
{
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
            /*Have the audio files list*/
            /*Put in display list widget*/
            /*Shoud we create this list before user request it*/
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

/*******************************************************************************
 End of File
 */
