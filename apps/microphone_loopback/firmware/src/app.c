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

#include "app.h"        // also beings in app_tone_lookup_table.h

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

DRV_I2S_DATA __attribute__ ((aligned (32))) micBuffer[MAX_BUFFERS][MAX_AUDIO_NUM_SAMPLES];

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
    KeyRepeatTask();    // used to debounce button or touch events
    //LED2_Toggle();      // uncomment to calibrate timer -- should toggle every 1 ms
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void _audioCodecDataInitialize (AUDIO_CODEC_DATA* codecData)
{
    codecData->handle = DRV_HANDLE_INVALID;
    codecData->context = 0;
    codecData->bufferHandler = 
           (DRV_CODEC_BUFFER_EVENT_HANDLER) Audio_Codec_BufferEventHandler;
    codecData->txbufferObject = (uint8_t *) micBuffer[0];
    codecData->rxbufferObject = (uint8_t *) micBuffer[1];
    codecData->bufferSize = sizeof(micBuffer[0]);
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
       
    _audioCodecDataInitialize (&appData.codecData);
    
    appData.delayEnabled = true;  
   
    APP_Initialize_sub();        // do specific gui/non-gui initialization    
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
    APP_Tasks_sub();        // do specific gui/non-gui processing    
    
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
                    DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_EXCLUSIVE);       
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
            
            appData.state = APP_STATE_CODEC_ADD_BUFFER;           
        }
        break;
       
        case APP_STATE_CODEC_ADD_BUFFER:
        {
            DRV_CODEC_BufferAddWriteRead(appData.codecData.handle,
                    &appData.codecData.writeBufHandle,
                    appData.codecData.txbufferObject, appData.codecData.rxbufferObject,
                    appData.codecData.bufferSize);
            
            if(appData.codecData.writeBufHandle != DRV_CODEC_BUFFER_HANDLE_INVALID)
            {
                // set up for next time
                if (appData.delayEnabled)
                {
                    appData.txBufferIdx++;
                    if (appData.txBufferIdx >= MAX_BUFFERS)
                    {
                        appData.txBufferIdx = 0;
                    }
                    appData.rxBufferIdx++;
                    if (appData.rxBufferIdx >= MAX_BUFFERS)
                    {
                        appData.rxBufferIdx = 0;
                    }                        
                } 
                else
                {
                    appData.txBufferIdx = 1 - appData.txBufferIdx;      // swap between 0 and 1
                    appData.rxBufferIdx = 1 - appData.txBufferIdx;
                }
                
                appData.codecData.txbufferObject = (uint8_t *) micBuffer[appData.txBufferIdx];
                appData.codecData.rxbufferObject = (uint8_t *) micBuffer[appData.rxBufferIdx];                
        
                appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
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

