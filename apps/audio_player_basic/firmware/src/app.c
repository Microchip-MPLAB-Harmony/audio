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
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint16_t volumeLevels[VOL_LVLS_MAX] =
{
    0,      // Mute
    16,     // -66 dB
    64,     // -48 dB
    255     // 0 dB
};



DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer1[NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer2[NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Single_Chnl_Buffer[NUM_SAMPLES/2];
#ifdef USE_CLEAR_BUFFER
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Clear_Buffer[NUM_SAMPLES];
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
    if (appData.buttonDelay)
    {      
        appData.buttonDelay--;
    }
    if (appData.led1Delay)
    {
        appData.led1Delay--;
    }
    if (appData.led2Delay)
    {
        appData.led2Delay--;
    }
    if (appData.playbackDelay)
    {
        appData.playbackDelay--;
    }
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
	appData.deviceIsConnected = false;
    
    appData.volLevel = VOL_LVL_LOW;
   
    appData.buttonDelay = 0;
    appData.buttonMode = false;    
    appData.led1Delay = 0;
    appData.led2Delay = 0;
    appData.playbackDelay = 2000;

#ifdef  USE_CLEAR_BUFFER    
    for(uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        App_Audio_Output_Clear_Buffer[i].leftData = 0x0000;
        App_Audio_Output_Clear_Buffer[i].rightData = 0x0000;
    }
#endif
    _audioCodecInitialize (&appData.codecData);
    
    appData.pingPong = 1;    
    APP_PlayerInitialize();
}

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
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
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
            appData.deviceIsConnected = true;
            break;
            
        case SYS_FS_EVENT_UNMOUNT:
            appData.deviceIsConnected = false;
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
    if(strcmp(lowercase, "wav")==0)
    {
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
WAV_FILE_HEADER wavHeader;

void APP_Tasks ( void )
{
    int retval;
    
   	APP_Button_Tasks();
    DISK_Tasks();
    APP_LED_Tasks();
    
    // check the application's current state
    switch ( appData.state )
    {       
        // application's initial state.
        case APP_STATE_INIT:
            // open the timer Driver
            tmrHandle = SYS_TIME_CallbackRegisterMS(App_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
               appData.state = APP_STATE_CODEC_OPEN;
            }            
            break;
        
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
                        appData.state = APP_STATE_BUS_ENABLE;
                        appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                        srand(appData.playbackDelay);
                    }            
                }
            }
            break;
        
        
        // set a handler for the audio buffer completion event
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);
            appData.state = APP_STATE_OPEN_FILE;
            appData.state = APP_STATE_BUS_ENABLE;
            break;

        case APP_STATE_BUS_ENABLE:
           // set the event handler and enable the bus
            SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(0);
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
            
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
            if(USB_HOST_BusIsEnabled(0))
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
       
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(appData.deviceIsConnected)
            {
                appData.state = APP_STATE_DEVICE_CONNECTED;
            }
            break;

        case APP_STATE_DEVICE_CONNECTED:
            // device was mounted. We can try scanning the disk
            appData.state = APP_STATE_READY_TO_SCAN;
            break;
        
        case APP_STATE_READY_TO_SCAN:
            break;
        
        case APP_STATE_SCANNING:
            break;
        
        case APP_STATE_OPEN_FILE:
            if(!appData.totalAudioFiles)
            {
                appData.state = APP_STATE_SCANNING;
                break;
            }
            if(appData.playbackDelay)
                break;
            // try opening the file for reading
            appData.fileHandle = SYS_FS_FileOpen(appData.fileName, (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                // could not open the file -- 
                appData.state = APP_STATE_SCANNING;
            }
            else
            {
                // file opened successfully -- read from file
                appData.state = APP_STATE_READ_FILE;
                // Read the header data
                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) &wavHeader, sizeof(wavHeader));
            }
            break;

        case APP_STATE_READ_FILE:
            // Go to next track if it's not a WAVE header or the data is not in PCM format or 8000 > samplerate > 96000
            if((wavHeader.audioFormat != 1) || (strncmp(wavHeader.format, "WAVE", 4) != 0) || 
                    (strncmp(wavHeader.chunkID, "RIFF", 4) != 0) || (wavHeader.sampleRate < 8000) || (wavHeader.sampleRate > 96000))
            {
                appData.state = APP_STATE_SCANNING;
                break;
            }
            
            // Set sample rate to what was found in the header
            DRV_CODEC_SamplingRateSet(appData.codecData.handle, wavHeader.sampleRate);

            // Set volume to user chosen level since it could have been changed for a single channel .wav with 8 bit samples
            appData.volume = volumeLevels[appData.volLevel];
            DRV_CODEC_VolumeSet(appData.codecData.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);
            
            // try reading the file  
            // If the file is single channel, arrange to be stereo format
            if(wavHeader.numChannels == 1)
            {
                switch(wavHeader.bitsPerSample)
                {
                    case 16:
                        /***Clicking happens during the SYS_FS_FileRead()***/
                        retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/2);
                        for(int16_t i=0; i<NUM_SAMPLES/2; i++)
                        {
                            App_Audio_Output_Buffer1[i*2].rightData = App_Single_Chnl_Buffer[i].leftData;
                            App_Audio_Output_Buffer1[i*2].leftData = App_Single_Chnl_Buffer[i].leftData;
                            App_Audio_Output_Buffer1[(i*2)+1].rightData = App_Single_Chnl_Buffer[i].rightData;
                            App_Audio_Output_Buffer1[(i*2)+1].leftData = App_Single_Chnl_Buffer[i].rightData;
                        }
                        break;
                        
                    case 8:
                        retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/4);
                        for(int16_t i=0; i<NUM_SAMPLES/4; i++)
                        {
                            // Data in LSB
                            App_Audio_Output_Buffer1[i*4].rightData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                            App_Audio_Output_Buffer1[i*4].leftData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+1].rightData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+1].leftData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+2].rightData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+2].leftData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+3].rightData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                            App_Audio_Output_Buffer1[(i*4)+3].leftData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                        }
                        // Temporarily set volume to max level since there is no data in the MSB
                        DRV_CODEC_VolumeSet(appData.codecData.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT, volumeLevels[VOL_LVL_HIGH]);                                      
                        break;
                        
                    default:
                        // Currently non supported bits/sample
                       retval = -1;
                       break;
                }
            }
            else
            {
                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Output_Buffer1, BUFFER_SIZE);
            }
            if (retval == -1)
            {
                // read was not successful -- assume end of file
                appData.state = APP_STATE_SCANNING;
            }
            else
            {
                // Adjust the number of bytes that need to be sent out
                if(wavHeader.numChannels == 1)
                {
                    // 8 bits/sample
                    if(wavHeader.bitsPerSample == 8)
                    {
                        retval *= 4;
                    }
                    else
                    {
                        // 16 bits/sample
                        retval *= 2;
                    }
                }
#ifdef USE_CLEAR_BUFFER
                appData.codecData.txbufferObject1 = (uint8_t *) App_Audio_Output_Clear_Buffer;
#else
                appData.codecData.txbufferObject1 = (uint8_t *) App_Audio_Output_Buffer1;
#endif
                appData.codecData.bufferSize1 = retval;
                appData.state = APP_STATE_CODEC_ADD_BUFFER;
            }
            break;        
       
        case APP_STATE_CODEC_ADD_BUFFER:
            // Don't try to r/w more data if the storage device is not mounted
            if(!appData.deviceIsConnected)
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
                break;
            }
            if (appData.buttonMode || (appData.volLevel == VOL_LVL_MUTE))
            {
                break;      // paused
            }
            
            DRV_CODEC_LRCLK_Sync(appData.codecData.handle);
            
            if (appData.pingPong==1)
            {
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                                            &appData.codecData.writeBufHandle1,
                                            appData.codecData.txbufferObject1,
                                            appData.codecData.bufferSize1);

                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle1 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = 2;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    
                    // try reading the file            
                    if(wavHeader.numChannels == 1)
                    {
                        switch(wavHeader.bitsPerSample)
                        {
                            case 16:
                                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/2);
                                for(int16_t i=0; i<NUM_SAMPLES/2; i++)
                                {
                                    App_Audio_Output_Buffer2[i*2].rightData = App_Single_Chnl_Buffer[i].leftData;
                                    App_Audio_Output_Buffer2[i*2].leftData = App_Single_Chnl_Buffer[i].leftData;
                                    App_Audio_Output_Buffer2[(i*2)+1].rightData = App_Single_Chnl_Buffer[i].rightData;
                                    App_Audio_Output_Buffer2[(i*2)+1].leftData = App_Single_Chnl_Buffer[i].rightData;
                                }
                                break;

                            case 8:
                                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/4);
                                for(int16_t i=0; i<NUM_SAMPLES/4; i++)
                                {
                                    // Data in LSB
                                    App_Audio_Output_Buffer2[i*4].rightData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                                    App_Audio_Output_Buffer2[i*4].leftData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+1].rightData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+1].leftData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+2].rightData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+2].leftData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+3].rightData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer2[(i*4)+3].leftData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                                }
                                break;

                            default:
                                // Currently non supported bits/sample
                               retval = -1;
                               break;
                        }
                    }
                    else
                    {
                        retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Output_Buffer2, BUFFER_SIZE);
                    }
                    if (retval == -1)
                    {
                        // read was not successful -- assume end of file
                        appData.state = APP_STATE_SCANNING;
                    }
                    else
                    {
                        if(wavHeader.numChannels == 1)
                        {
                            // 8 bits/sample
                            if(wavHeader.bitsPerSample == 8)
                            {
                                retval *= 4;
                            }
                            else
                            {
                                // 16 bits/sample
                                retval *= 2;
                            }
                        }
#ifdef USE_CLEAR_BUFFER
                        appData.codecData.txbufferObject2 = (uint8_t *) App_Audio_Output_Clear_Buffer;
#else
                        appData.codecData.txbufferObject2 = (uint8_t *) App_Audio_Output_Buffer2;
#endif
                        appData.codecData.bufferSize2 = retval;
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
                                            appData.codecData.bufferSize2);
                
                // initiated a write to the codec via I2S, read next buffer full
                if(appData.codecData.writeBufHandle2 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    appData.pingPong = 1;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    // try reading the file            
                    if(wavHeader.numChannels == 1)
                    {
                        switch(wavHeader.bitsPerSample)
                        {
                            case 16:
                                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/2);
                                for(int16_t i=0; i<NUM_SAMPLES/2; i++)
                                {
                                    App_Audio_Output_Buffer1[i*2].rightData = App_Single_Chnl_Buffer[i].leftData;
                                    App_Audio_Output_Buffer1[i*2].leftData = App_Single_Chnl_Buffer[i].leftData;
                                    App_Audio_Output_Buffer1[(i*2)+1].rightData = App_Single_Chnl_Buffer[i].rightData;
                                    App_Audio_Output_Buffer1[(i*2)+1].leftData = App_Single_Chnl_Buffer[i].rightData;
                                }
                                break;

                            case 8:
                                retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Single_Chnl_Buffer, BUFFER_SIZE/4);
                                for(int16_t i=0; i<NUM_SAMPLES/4; i++)
                                {
                                    // Data in LSB
                                    App_Audio_Output_Buffer1[i*4].rightData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                                    App_Audio_Output_Buffer1[i*4].leftData = App_Single_Chnl_Buffer[i].leftData & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+1].rightData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+1].leftData = (App_Single_Chnl_Buffer[i].leftData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+2].rightData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+2].leftData = App_Single_Chnl_Buffer[i].rightData & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+3].rightData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                                    App_Audio_Output_Buffer1[(i*4)+3].leftData = (App_Single_Chnl_Buffer[i].rightData >> 8) & 0x00FF;
                                }
                                break;

                            default:
                                // Currently non supported bits/sample
                               retval = -1;
                               break;
                        }
                    }
                    else
                    {
                        retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Output_Buffer1, BUFFER_SIZE);
                    }
                    if (retval == -1)
                    {
                        // read was not successful -- assume end of file
                        // this one needs to be modified
                        appData.state = APP_STATE_SCANNING;
                    }
                    else
                    {
                        if(wavHeader.numChannels == 1)
                        {
                            // 8 bits/sample
                            if(wavHeader.bitsPerSample == 8)
                            {
                                retval *= 4;
                            }
                            else
                            {
                                // 16 bits/sample
                                retval *= 2;
                            }
                        }
#ifdef USE_CLEAR_BUFFER
                        appData.codecData.txbufferObject1 = (uint8_t *) App_Audio_Output_Clear_Buffer;
#else
                        appData.codecData.txbufferObject1 = (uint8_t *) App_Audio_Output_Buffer1;
#endif
                        appData.codecData.bufferSize1 = retval;
                    }                    
                }               
                else
                {
                    appData.state = APP_STATE_SCANNING;
                }
            }                       
            break;

        // audio data Transmission under process
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
            break;
        
        case APP_STATE_NO_MEDIA:
            appData.state = APP_STATE_SCANNING;
		default:
            break;
    }
}
 

void APP_LED_Tasks( void )
{
    if(!appData.led1Delay)
    {
        switch(appData.state)
        {
            case APP_STATE_INIT:
            case APP_STATE_CODEC_OPEN:
            case APP_STATE_BUS_ENABLE:
            case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
            case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
                LED1_Toggle();
                appData.led1Delay = 100;
                appData.totalAudioFiles = 0;
                break;
            default:
                if(appData.volLevel == VOL_LVL_MUTE)
                {
                    LED1_Toggle();
                    appData.led1Delay = 500;
                }
                else
                {
                    switch(appData.playerBtnMode)
                    {
                        case VOL_ADJUST:
                        default:
                            LED1_Off();
                            break;

                        case LINEAR_TRACK_CHANGE:
                            LED1_On();
                            break;
                    }
                    appData.led1Delay = 500;
                }
                break;
        }
    }
    if(!appData.led2Delay)
    {
        if(appData.totalAudioFiles)
        {
            LED2_On();
        }
        else
        {
            LED2_Off();
        }
        appData.led2Delay = 500;
    }
}



#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000

void APP_Button_Tasks()
{
   //BUTTON PROCESSING
    /* Check the buttons' current state. */      
    switch ( appData.buttonState )
    {
        case BUTTON_STATE_IDLE:
            if ( (appData.buttonDelay==0)&&
                 (SWITCH_Get()==SWITCH_STATE_PRESSED))                
            {
                appData.buttonDelay=BUTTON_DEBOUNCE;       
                appData.buttonState=BUTTON_STATE_PRESSED;               
            }
            break;
        
        case BUTTON_STATE_PRESSED:
            if (appData.buttonDelay>0)
            {
                break;      // still debouncing
            }
            
            if(SWITCH_Get()==SWITCH_STATE_PRESSED) 
            {                
                appData.buttonDelay=LONG_BUTTON_PRESS;          // 1 sec is long press
                appData.buttonState=BUTTON_STATE_BUTTON0_PRESSED;                  
            }
            break;
          
        case BUTTON_STATE_BUTTON0_PRESSED:
            if ((appData.buttonDelay>0)&&
                (SWITCH_Get()!=SWITCH_STATE_PRESSED))     // SW01 pressed and released < 1 sec
            {
                switch(appData.playerBtnMode)
                {
                    case VOL_ADJUST:
                        appData.volLevel++;
                        appData.volLevel %= VOL_LVLS_MAX;
                        switch(appData.volLevel)
                        {
                            case VOL_LVL_LOW:
                            case VOL_LVL_MED:
                            case VOL_LVL_HIGH:
                            case VOL_LVL_MUTE:
                                appData.volume = volumeLevels[appData.volLevel];
                                DRV_CODEC_VolumeSet(appData.codecData.handle,
                                    DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);                                      
                            default:
                                break;
                        }
                        break;
                    case LINEAR_TRACK_CHANGE:
//                    case RANDOM_TRACK_CHANGE:
                    default:
                        SYS_FS_FileSeek(appData.fileHandle, 0, SYS_FS_SEEK_END);
                        break;
                }

                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              
            }
            else if ((appData.buttonDelay==0)&&
                     (SWITCH_Get()==SWITCH_STATE_PRESSED))  // SW0 still pressed after 1 sec
            {
                if(appData.volLevel != VOL_LVL_MUTE)
                {
                    // Index through modes if we are not muted
                    appData.playerBtnMode++;
                    appData.playerBtnMode %= MAX_MODES;
                }
                appData.buttonState=BUTTON_STATE_WAIT_FOR_RELEASE;                
            }                          
            break;

        case BUTTON_STATE_WAIT_FOR_RELEASE:
            if (SWITCH_Get()!=SWITCH_STATE_PRESSED)
            {
                appData.buttonDelay=BUTTON_DEBOUNCE;
                appData.buttonState=BUTTON_STATE_IDLE;
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
