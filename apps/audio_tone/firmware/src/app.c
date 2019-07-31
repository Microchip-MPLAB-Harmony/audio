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

#include "bsp/bsp.h"
#include "app.h"        // also beings in app_tone_lookup_table.h

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// table designed for 48 kHz or 96 kHz samples/sec only
// maximum value is MAX_AUDIO_NUM_SAMPLES (currently 384)
        
// table designed for 48 kHz samples/sec only
uint16_t samples[SAMPLE_STEPS] =
{
     192, // 250 Hz at 48000 samples/sec
      96, // 500
      48, // 1000
      24, // 2 kHz at 48000 samples/sec
};

uint16_t volumeLevels[VOLUME_STEPS] =
{
#ifdef DRV_AK4954_VOLUME
    0 /* off */, 192, 208, 224          // if using AK4954, use narrower range  
#else            
    0 /* off */, 128, 192, 255
#endif            
          
};

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

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/// *****************************************************************************
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
    //LED2_Toggle();        // uncomment to calibrate timer -- should toggle every 1 ms
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

void fillInNumSamplesTable(uint8_t bufferNum)
{
    uint16_t i;
    
    if (bufferNum==1)
    {
        appData.numNumSamples1 = MAX_AUDIO_NUM_SAMPLES / samples[appData.sampleTableIndex];
        for (i=0; i < appData.numNumSamples1; i++)
        {
            appData.numSamples1[i] = samples[appData.sampleTableIndex];
        }
    }
    else
    {
        appData.numNumSamples2 = MAX_AUDIO_NUM_SAMPLES / samples[appData.sampleTableIndex]; 
        for (i=0; i < appData.numNumSamples2; i++)
        {
            appData.numSamples2[i] = samples[appData.sampleTableIndex];
        }        
    }
}

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
bool firstSync;

void APP_Initialize ( void )
{
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */   
    appData.state = APP_STATE_INIT;
    
    appData.volumeIndex = INIT_VOLUME_IDX;
    appData.volume = volumeLevels[appData.volumeIndex];    
    
    appData.buttonDelay = 0;
    appData.buttonMode = false;    
    
    _audioCodecInitialize (&appData.codecData);

    appData.sampleTableIndex = INIT_SAMPLE_INDEX;   

    appData.pingPong = 1; 
 
    fillInNumSamplesTable(1);
    fillInNumSamplesTable(2);
    
    firstSync = true;
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
    APP_Button_Tasks();
    
    /* Check the application's current state. */
    switch ( appData.state )
    {       
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Open the timer Driver */
            tmrHandle = SYS_TIME_CallbackRegisterMS(App_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
               appData.state = APP_STATE_CODEC_OPEN;
            }            
        }
        break;
        
        case APP_STATE_CODEC_OPEN:
        {
			// see if codec is done initializing
            SYS_STATUS status = DRV_CODEC_Status(sysObjdrvCodec0);
            if (SYS_STATUS_READY == status)
            {
                // This means the driver can now be be opened.
                /* A client opens the driver object to get an Handle */
                appData.codecData.handle = DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                    DRV_IO_INTENT_WRITE | DRV_IO_INTENT_EXCLUSIVE);       
                if(appData.codecData.handle != DRV_HANDLE_INVALID)
                {
                    DRV_CODEC_VolumeSet(appData.codecData.handle,
                        DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);
                        
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                }            
            }
        }
        break;
        
        /* Set a handler for the audio buffer completion event */
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {         
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);                                 

            fillInNumSamplesTable(1);                  

            APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 1,
                    appData.sampleRate, appData.numSamples1, appData.numNumSamples1);
                                 
            appData.state = APP_STATE_CODEC_ADD_BUFFER;           
        }
        break;
       
        case APP_STATE_CODEC_ADD_BUFFER:
        {           
            if (firstSync)
            {
                firstSync = false;
                DRV_CODEC_LRCLK_Sync(appData.codecData.handle);
            } 
            
            if (appData.pingPong==1)
            {
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                        &appData.codecData.writeBufHandle1,
                        appData.codecData.txbufferObject1,
                        appData.codecData.bufferSize1);

                if(appData.codecData.writeBufHandle1 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {   
                    // just sent buffer 1 to DMA, so fill in buffer 2 for next time
                    fillInNumSamplesTable(2);

                    APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 2,
                            appData.sampleRate, appData.numSamples2, appData.numNumSamples2);

                    appData.pingPong = 2;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                }               
            }
            else
            {                
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                        &appData.codecData.writeBufHandle2,
                        appData.codecData.txbufferObject2,
                        appData.codecData.bufferSize2);
                if(appData.codecData.writeBufHandle2 != DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    // just sent buffer 2 to DMA, so fill in buffer 1 for next time 
                    fillInNumSamplesTable(1);
                    
                    APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 1,
                            appData.sampleRate, appData.numSamples1, appData.numNumSamples1);
             
                    appData.pingPong = 1;
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                }               
            }                       
        }
        break;

        /* Audio data Transmission under process */
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
        {
        }
        break;
         
        default:
        {
            /* TODO: Handle error in application's state machine. */
        }
        break;             
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
        {
            if ( (appData.buttonDelay==0)&&
                 (SWITCH_Get()==SWITCH_STATE_PRESSED))                
            {
                appData.buttonDelay=BUTTON_DEBOUNCE;       
                appData.buttonState=BUTTON_STATE_PRESSED;               
            }
        }
        break;
        
        case BUTTON_STATE_PRESSED:
        { 
            if (appData.buttonDelay>0)
            {
                break;      // still debouncing
            }
            
            if(SWITCH_Get()==SWITCH_STATE_PRESSED) 
            {                
                appData.buttonDelay=LONG_BUTTON_PRESS;          // 1 sec is long press
                appData.buttonState=BUTTON_STATE_BUTTON0_PRESSED;                  
            }
        }
        break;
          
        case BUTTON_STATE_BUTTON0_PRESSED:
        {
            if ((appData.buttonDelay>0)&&
                (SWITCH_Get()!=SWITCH_STATE_PRESSED))     // SW01 pressed and released < 1 sec
            {
                if (appData.buttonMode)     // if modifying frequency
                {
                    appData.sampleTableIndex++;
                    if (appData.sampleTableIndex >= SAMPLE_STEPS)
                    {
                        appData.sampleTableIndex = 0;    
                    }
                                     
                    if (0==appData.volume)
                    {
                        // after changing freq, if volume 0 make sure we can hear it
                        appData.volumeIndex++;
                        appData.volume = volumeLevels[appData.volumeIndex];
                        DRV_CODEC_VolumeSet(appData.codecData.handle,
                            DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);                        
                        DRV_CODEC_MuteOff(appData.codecData.handle);                        
                    }                                                
                }
                else
                {
                    uint8_t oldVolumeIndex;
                    
                    oldVolumeIndex = appData.volumeIndex;
                    appData.volumeIndex++;
                    if (appData.volumeIndex >= VOLUME_STEPS)
                    {
                        appData.volumeIndex = 0;    
                    }
                    
                    appData.volume = volumeLevels[appData.volumeIndex];
                    
                    if (0==appData.volume)
                    {
                        DRV_CODEC_MuteOn(appData.codecData.handle);                       
                    }
                    else
                    {
                        DRV_CODEC_VolumeSet(appData.codecData.handle,
                            DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);                                      
                        if (0==volumeLevels[oldVolumeIndex])
                        {
                            // if volume was 0, unmute codec
                            DRV_CODEC_MuteOff(appData.codecData.handle);                     
                        }
                    }                             
                }
                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              
            }
            else if ((appData.buttonDelay==0)&&
                     (SWITCH_Get()==SWITCH_STATE_PRESSED))  // SW0 still pressed after 1 sec
            {
                appData.buttonMode = !appData.buttonMode;
                if (appData.buttonMode)
                {
                    LED1_On();
                }
                else
                {
                    LED1_Off();
                }               
                
                appData.buttonState=BUTTON_STATE_WAIT_FOR_RELEASE;                
            }                          
        } 
        break;

        case BUTTON_STATE_WAIT_FOR_RELEASE:
        {
            if (SWITCH_Get()!=SWITCH_STATE_PRESSED)
            {
                appData.buttonDelay=BUTTON_DEBOUNCE;
                appData.buttonState=BUTTON_STATE_IDLE;
            }
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
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
        {
            //Signal to APP that Tx is complete. 
            appData.state = APP_STATE_CODEC_ADD_BUFFER; 
        }        
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
        } 
        break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
        } 
        break;
    }
}

/*******************************************************************************
 End of File
 */
