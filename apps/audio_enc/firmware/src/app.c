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

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define LED_PRD_MIN 50
#define LED_PRD_MAX 10000

#define _10ms       10
#define _20ms       2*_10ms
#define _50ms       5*_10ms
#define _100ms      2*_50ms
#define _200ms      2*_100ms
#define _500ms      5*_100ms
#define _1sec       2*_500ms

uint32_t bytesRemaining = 0;


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
USB_DATA usbData;
BTN_DATA btnData;
LED_DATA ledData;

DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Input_Buffer[2][NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer[2][NUM_SAMPLES];

const HAR_ENCODER *rtEncoderInst;
//static uint8_t pheader[AUDIO_FILE_METADATA_HEADER_SIZE];
static WAVHEADER pheader;
static dWAVHEADER   decWavHdr;
static uint32_t _audio_frame_count = 0;
static StreamInfo streamInfo;
static uint32_t encoded_data_size;
static EncoderType encType;

uint16_t inSize, numRead, numWritten;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

static void App_TimerCallback( uintptr_t context)
{
    if( btnData.delay )
    {      
        btnData.delay--;
    }
    if( ledData.led1Delay )
    {      
        ledData.led1Delay--;
    }
    if( ledData.led2Delay )
    {      
        ledData.led2Delay--;
    }
    if( appData.closeDelay )
    {
        appData.closeDelay--;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void _audioCodecInitialize (AUDIO_CODEC_DATA* codecData)
{
    codecData->handle = DRV_HANDLE_INVALID;
    codecData->context = 0;
    codecData->bufferHandler = 
           (DRV_CODEC_BUFFER_EVENT_HANDLER) Audio_Codec_BufferEventHandler;
    codecData->rxbufferObject = (uint8_t *)App_Audio_Input_Buffer[0];
    codecData->txbufferObject = (uint8_t *)App_Audio_Input_Buffer[1];
    codecData->bufferSize = sizeof(App_Audio_Input_Buffer[0]);
}



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
        {
            usbData.deviceIsConnected = true;
            LED_Set_Mode( 1, LED_STATE_OFF, 0 );
            break;
        }
        case SYS_FS_EVENT_UNMOUNT:
        {
            usbData.deviceIsConnected = false;
            usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            appData.state = APP_STATE_WAITING_FOR_USB_MSD;
            LED_Set_Mode( 1, LED_STATE_TOGGLE, _50ms );
            LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
            break;
        }
        default:
        {
            break;
        }
    }
}

//******************************************************************************
// 
// Audio_Codec_RxBufferComplete() - Set APP_Tasks Next state to buffer complete.
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
            //Signal to APP that Rx is complete.
            if( appData.encoding )
            {
                appData.state = APP_STATE_CODEC_READ; 
                appData.encCmpltEvnt++;
            }
            else if( appData.playback )
            {
                appData.state = APP_STATE_DECODE_AND_PLAYBACK;
            }
            break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        case DRV_CODEC_BUFFER_EVENT_ABORT:
        default:
            // Indicate an error happened
            appData.state = APP_STATE_ERROR;
            break;
    }
}

void APP_Set_FileExt( void )
{
#ifdef WAV_CONTAINER
    strcpy(appData.ext, "wav");
    return;
#endif
    strcpy(appData.ext, "txt");
    return;
}

/* Get a fileName for the first non-existent file */
bool APP_Set_FileName( void )
{
    char strFn[30];
    bool retStat = false;
    APP_Set_FileExt();
    APP_Set_BitDepth();
    APP_Set_SampleRate();
    for( int16_t i = 0; i < 1000; i++ )
    {
        sprintf(strFn, "/mnt/myDrive1/enc%s%s%03d%c", appData.srStr, appData.bdStr, i, '.');
        strcat(strFn, appData.ext);
        appData.fileHandle = SYS_FS_FileOpen(strFn, (SYS_FS_FILE_OPEN_READ));
        
        if(appData.fileHandle != SYS_FS_HANDLE_INVALID)
        {
            // File exist.  Try again.
            retStat = false;
        }
        else
        {
            // File open failed. File doesn't exist.
            retStat = true;
            strcpy( appData.fileName, strFn );
        }
        SYS_FS_FileClose( appData.fileHandle );
        if( retStat )
            break;
    }
    return retStat;
}

void APP_Set_BitDepth( void )
{
    strcpy( appData.bdStr, "16b");
}

void APP_Set_SampleRate( void )
{
    switch( appData.sampleRateMode )
    {
        case _8K:
            appData.sampleRate = 8000;
            strcpy(appData.srStr, "8K");
           break;
        case _16K:
        default:
            appData.sampleRate = 16000;
            strcpy(appData.srStr, "16K");
           break;
        case _44_1K:
            appData.sampleRate = 44100;
            strcpy(appData.srStr, "44_1K");
           break;
        case _48K:
            appData.sampleRate = 48000;
            strcpy(appData.srStr, "48K");
           break;
        case _96K:
            appData.sampleRate = 96000;
            strcpy(appData.srStr, "96K");
           break;
    }
    DRV_CODEC_SamplingRateSet(appData.codecData.handle, appData.sampleRate);
}

bool APP_Encode( void )
{
    uint32_t retVal;
    pheader.dataLen = 16;
    pheader.datasize = SYS_FS_FileSize( appData.dataFileHandle );
    if( pheader.datasize == -1 )
    {
        return false;
    }
    pheader.filesize = WAV_HEADER_SIZE + pheader.datasize;
    if ( SYS_FS_FileWrite( appData.fileHandle, &pheader, WAV_HEADER_SIZE ) == WAV_HEADER_SIZE )
    {
        do 
        {
            retVal = SYS_FS_FileRead( appData.dataFileHandle, App_Audio_Input_Buffer[appData.rxBufIdx], BUFFER_SIZE );
            if( retVal != -1 )
            {
                pcm_encode_frame( App_Audio_Input_Buffer[appData.rxBufIdx], retVal, App_Audio_Output_Buffer[appData.txBufIdx], &retVal );
                SYS_FS_FileWrite( appData.fileHandle, App_Audio_Output_Buffer[appData.txBufIdx], retVal );
            }
        } while( !SYS_FS_FileEOF( appData.dataFileHandle ));
    }
    else
    {
        return false;
    }
    return true;
}

void APP_Decode( void )
{
    inSize = BUFFER_SIZE;
    
    WAV_Decoder( (void *)App_Audio_Input_Buffer[appData.rxBufIdx], inSize, &numRead,
                (void *)App_Audio_Output_Buffer[appData.txBufIdx], &numWritten);
    appData.codecData.bufferSize = numWritten;
    appData.codecData.txbufferObject = (uint8_t *)App_Audio_Output_Buffer[appData.txBufIdx];
    appData.codecData.rxbufferObject = (uint8_t *)App_Audio_Input_Buffer[appData.rxBufIdx];
}

void APP_EncoderInit( void )
{
//    const HAR_ENCODER *rtEncoderInst;
    _audio_frame_count = 0;
    streamInfo.sample_rate = appData.sampleRate;
    streamInfo.channel = 2;
    streamInfo.bit_depth = 16;
    streamInfo.bps = streamInfo.sample_rate * streamInfo.channel * streamInfo.bit_depth;
    encoded_data_size = SYS_FS_FileSize( appData.dataFileHandle );
    encType = PCM;
    wav_riff_fill_header(&pheader, encType, &streamInfo, encoded_data_size);
}

void APP_DecoderInit( void )
{
    // 
    WAV_Initialize_N( (uint8_t *)&decWavHdr, appData.fileHandle );
    appData.codecData.txbufferObject = (uint8_t *)App_Audio_Output_Buffer[appData.txBufIdx];
    appData.codecData.rxbufferObject = (uint8_t *)App_Audio_Input_Buffer[appData.rxBufIdx];
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    usbData.state = USB_STATE_INIT;
    LED_Set_Mode( 1, LED_STATE_ON, _50ms );
    LED_Set_Mode( 2, LED_STATE_ON, _50ms );
    LED_Set_Mode( 1, LED_STATE_OFF, _50ms );
    LED_Set_Mode( 2, LED_STATE_OFF, _50ms );
    appData.sampleRate = 16000;
    appData.sampleRateMode = _16K;
    appData.encoding = false;
    appData.playback = false;
    appData.errorCnt = 0;
    appData.rxBufIdx = 0;
    appData.txBufIdx = 1;
    appData.encCmpltEvnt = 1;
    appData.fileHandle = SYS_FS_HANDLE_INVALID;
    appData.dataFileHandle = SYS_FS_HANDLE_INVALID;
    _audioCodecInitialize( &appData.codecData );
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    uint32_t retval;
    USB_Tasks();
    BTN_Tasks();
    LED_Tasks();

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            // open the timer Driver
            appData.tmrHandle = SYS_TIME_CallbackRegisterMS(App_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != appData.tmrHandle )
            {
                appData.state = APP_STATE_CODEC_OPEN;
                LED_Set_Mode( 1, LED_STATE_TOGGLE, _50ms );
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
                                                    DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_EXCLUSIVE);     
                    if(appData.codecData.handle != DRV_HANDLE_INVALID)
                    {
                        DRV_CODEC_MuteOff( appData.codecData.handle );
                        DRV_CODEC_VolumeSet(appData.codecData.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT, 200 );
                        appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                    }            
                }
            break;
        }
        // set a handler for the audio buffer completion event
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);
            appData.state = APP_STATE_WAITING_FOR_USB_MSD;
            break;
        }
        case APP_STATE_WAITING_FOR_USB_MSD:
        {
            if( usbData.deviceIsConnected )
            {
                appData.state = APP_STATE_SET_SAMPLERATE;
                appData.encCmpltEvnt = 1;
                LED_Set_Mode( 1, LED_STATE_OFF, 0 );
            }
            break;
        }
        case APP_STATE_SET_SAMPLERATE:
        {
            if( appData.sampleRate )
            {
                appData.state = APP_STATE_WAITING_FOR_START;
                LED_Set_Mode( 2, LED_STATE_ON, 0 );
            }
            break;
        }
        case APP_STATE_WAITING_FOR_START:
        {
            if( appData.encoding )
            {
                appData.playback = false;
                // Open the file to capture raw input data
                appData.dataFileHandle = SYS_FS_FileOpen("/mnt/myDrive1/capture.raw", (SYS_FS_FILE_OPEN_WRITE));
                if(appData.dataFileHandle == SYS_FS_HANDLE_INVALID)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                appData.codecData.rxbufferObject = (uint8_t *) App_Audio_Input_Buffer[appData.rxBufIdx];
                appData.codecData.txbufferObject = (uint8_t *) App_Audio_Input_Buffer[appData.txBufIdx];

                appData.state = APP_STATE_CODEC_READ;
                // Indicate we are reading audio from the mic
                LED_Set_Mode( 2, LED_STATE_TOGGLE, _50ms );
            }
            break;
        }
        case APP_STATE_CODEC_READ:
        {
            if( appData.encoding )
            {
                DRV_CODEC_BufferAddWriteRead( appData.codecData.handle, &appData.codecData.rdBufHandle,
                        appData.codecData.txbufferObject, appData.codecData.rxbufferObject, appData.codecData.bufferSize );
                appData.encCmpltEvnt--;
                if( appData.codecData.rdBufHandle != DRV_CODEC_BUFFER_HANDLE_INVALID )
                {
                    appData.rxBufIdx = 1 - appData.rxBufIdx;
                    appData.txBufIdx = 1 - appData.rxBufIdx;
                    
                    appData.codecData.rxbufferObject = (uint8_t *) App_Audio_Input_Buffer[appData.rxBufIdx];
                    appData.codecData.txbufferObject = (uint8_t *) App_Audio_Input_Buffer[appData.txBufIdx];

                    // Write out the data we just received from the codec
                    if( SYS_FS_FileWrite( appData.dataFileHandle, App_Audio_Input_Buffer[appData.txBufIdx], BUFFER_SIZE ) == -1 )
                    {
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
            }
            appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
            break;
        }
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
        {
            if( !appData.playback )
            {
                if( !appData.encoding )
                {
                    appData.state = APP_STATE_SET_FILENAME;
                    // Close the raw data capture file
                    SYS_FS_FileClose( appData.dataFileHandle );
                    LED_Set_Mode( 2, LED_STATE_OFF, 0 );
                }
                else if( appData.encCmpltEvnt )
                {
                    appData.state = APP_STATE_CODEC_READ;
                }
            }
            break;
        }
        case APP_STATE_SET_FILENAME:
        {
            if( APP_Set_FileName() )
            {
                appData.state = APP_STATE_OPEN_ENCODE_FILE;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        case APP_STATE_OPEN_ENCODE_FILE:
        {
            // Encode file to write
            appData.fileHandle = SYS_FS_FileOpen(appData.fileName,
                                        (SYS_FS_FILE_OPEN_WRITE));
            if( appData.fileHandle != SYS_FS_HANDLE_INVALID )
            {
                // File open is successful
                appData.state = APP_STATE_ENCODE_INIT;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        case APP_STATE_ENCODE_INIT:
        {
            appData.dataFileHandle = SYS_FS_FileOpen("/mnt/myDrive1/capture.raw", (SYS_FS_FILE_OPEN_READ));
            if(appData.dataFileHandle != SYS_FS_HANDLE_INVALID)
            {
                APP_EncoderInit();
                appData.state = APP_STATE_ENCODE_AND_WRITE_RESULT;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        case APP_STATE_ENCODE_AND_WRITE_RESULT:
        {
            APP_Encode();
            appData.closeDelay = _50ms;
            appData.state = APP_STATE_CLOSE_FILE;
            break;
        }
        case APP_STATE_CLOSE_FILE:
        {
            if( !appData.closeDelay )
            {
                appData.state = APP_STATE_WAITING_FOR_USB_MSD;
                SYS_FS_FileClose( appData.fileHandle );
                SYS_FS_FileClose( appData.dataFileHandle );
                LED_Set_Mode( 2, LED_STATE_OFF, _100ms );
                break;
            }
            break;
        }
        case APP_STATE_OPEN_PLAYBACK_FILE:
        {
            // Encode file to write
            appData.fileHandle = SYS_FS_FileOpen(appData.fileName,
                                        (SYS_FS_FILE_OPEN_READ));
            if( appData.fileHandle != SYS_FS_HANDLE_INVALID )
            {
                // File open is successful
                appData.playback = true;
                appData.state = APP_STATE_DECODE_INIT;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        case APP_STATE_DECODE_INIT:
        {
            retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *)&decWavHdr, sizeof(WAVHEADER));
            if(( retval == -1 ) || SYS_FS_FileEOF( appData.fileHandle ))
            {
                appData.state = APP_STATE_CLOSE_FILE;
                break;
            }
            bytesRemaining = SYS_FS_FileSize( appData.fileHandle ) - retval;
            APP_DecoderInit();
            retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer[appData.rxBufIdx], BUFFER_SIZE);
            if(( retval == -1 ) || SYS_FS_FileEOF( appData.fileHandle ))
            {
                appData.state = APP_STATE_CLOSE_FILE;
                break;
            }
            bytesRemaining -= retval;
            APP_Decode();
            appData.lrSync = true;
            appData.state = APP_STATE_DECODE_AND_PLAYBACK;
            break;
        }
        case APP_STATE_DECODE_AND_PLAYBACK:
        {
            if( appData.playback )
            {
                if( appData.lrSync )
                {
                    DRV_CODEC_LRCLK_Sync(appData.codecData.handle);
                    appData.lrSync = false;
                }
                // Output data
                DRV_CODEC_BufferAddWriteRead( appData.codecData.handle, &appData.codecData.rdBufHandle,
                        appData.codecData.txbufferObject, appData.codecData.rxbufferObject, appData.codecData.bufferSize );
                if( appData.codecData.rdBufHandle != DRV_CODEC_BUFFER_HANDLE_INVALID )
                {
                    appData.rxBufIdx = 1 - appData.rxBufIdx;
                    appData.txBufIdx = 1 - appData.rxBufIdx;
                    
                    // Read and decode the next buffer
                    retval = SYS_FS_FileRead( appData.fileHandle, (uint8_t *) App_Audio_Input_Buffer[appData.rxBufIdx], BUFFER_SIZE);
                    if(( retval == -1 ) || SYS_FS_FileEOF( appData.fileHandle ))
                    {
                        appData.state = APP_STATE_CLOSE_FILE;
                        appData.playback = false;
                        break;
                    }
                    bytesRemaining -= retval;
                    APP_Decode();
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    break;
                }
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_CLOSE_FILE;
            }
            break;
        }
        case APP_STATE_ERROR:
        /* The default state should never be executed. */
        default:
        {
            if( !appData.closeDelay )
            {
                appData.state = APP_STATE_CLOSE_FILE;
                appData.closeDelay = _1sec;
                appData.errorCnt++;
                appData.encoding = false;
                LED_Set_Mode( 1, LED_STATE_TOGGLE, _50ms );
                LED_Set_Mode( 2, LED_STATE_TOGGLE, _50ms );
            }
           /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

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
            SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(0);
            usbData.state = USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
        }
        case USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
            if(USB_HOST_BusIsEnabled(0))
            {
                usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        }
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
            break;
        }
        case USB_STATE_IDLE:
        {
            break;
        }
    }
}

void BTN_ShortPress( void )
{
    switch( appData.state )
    {
        case APP_STATE_WAITING_FOR_START:
        case APP_STATE_CODEC_READ:
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
            appData.encoding = !appData.encoding;
        default:
        break;
    }
}

void BTN_LongPress( void )
{
    switch( appData.state )
    {
        case APP_STATE_SET_SAMPLERATE:
        case APP_STATE_WAITING_FOR_START:
            appData.state = APP_STATE_OPEN_PLAYBACK_FILE;
        default:
        break;
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

void LED_Tasks( void )
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
/*******************************************************************************
 End of File
 */
