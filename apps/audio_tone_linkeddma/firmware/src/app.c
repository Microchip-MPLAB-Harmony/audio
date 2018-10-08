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
    0 /* off */, 128, 192, 255
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

// must be aligned in case of cache clean call
__attribute__ ((aligned (32))) XDMAC_DESCRIPTOR_VIEW_1 pTxLinkedListDesc[2]; 

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
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

void getNumSamples(uint8_t bufferNum)
{   
    if (bufferNum==1)
    {
        appData.numSamples1 = samples[appData.sampleTableIndex];
    }
    else
    {
        appData.numSamples2 = samples[appData.sampleTableIndex];       
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
    
    appData.volumeIndex = INIT_VOLUME_IDX;
    appData.volume = volumeLevels[appData.volumeIndex];    
    
    appData.buttonDelay = 0;
    appData.buttonMode = false;    
    
    _audioCodecInitialize (&appData.codecData);

    appData.sampleTableIndex = INIT_SAMPLE_INDEX; 
   
    appData.pingPong = 1;
    
    appData.freqChange = false;              
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
                        
                    appData.state = APP_STATE_INIT_TRANSFER;
                }            
            }                                                
        }    
        break;
        
        /* Set a handler for the audio buffer completion event */
        case APP_STATE_INIT_TRANSFER:
        {                   
            getNumSamples(1);                  

            APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 1,
                    appData.sampleRate, appData.numSamples1);

            getNumSamples(2);
                
            APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 2,
                    appData.sampleRate, appData.numSamples2);
                       
            DRV_I2S_InitWriteLinkedListTransfer(
                    DRV_CODEC_GetI2SDriver(appData.codecData.handle),
                    pTxLinkedListDesc, 0, 0, appData.codecData.txbufferObject1,
                    appData.codecData.bufferSize1);
            DRV_I2S_InitWriteLinkedListTransfer(
                    DRV_CODEC_GetI2SDriver(appData.codecData.handle),
                    pTxLinkedListDesc, 1, 1, appData.codecData.txbufferObject2,
                    appData.codecData.bufferSize2);            
           
            appData.state = APP_STATE_START_TRANSFER;           
        }
        break;
       
        case APP_STATE_START_TRANSFER:
        {                     
            DRV_I2S_StartWriteLinkedListTransfer(
                    DRV_CODEC_GetI2SDriver(appData.codecData.handle), pTxLinkedListDesc,
                    (DRV_I2S_LL_CALLBACK)Audio_Codec_TxBufferComplete, true);
                                           
            appData.state = APP_STATE_TOGGLE_BUFFERS;
        }
        break;
        
        case APP_STATE_TOGGLE_BUFFERS:
        {
            if (appData.freqChange)
            {                             
                if (appData.pingPong == 1)
                {
                    getNumSamples(2);

                    APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 2,
                            appData.sampleRate, appData.numSamples2);
                                        
                    DRV_I2S_WriteNextLinkedListTransfer(
                            DRV_CODEC_GetI2SDriver(appData.codecData.handle),
                            pTxLinkedListDesc, 0, 1, 1,
                            appData.codecData.txbufferObject2,
                            appData.codecData.bufferSize2);
                }
                else
                {                  
                    getNumSamples(1);                  

                    APP_TONE_LOOKUP_TABLE_Initialize(&appData.codecData, 1,
                            appData.sampleRate, appData.numSamples1);
                    
                    DRV_I2S_WriteNextLinkedListTransfer(
                            DRV_CODEC_GetI2SDriver(appData.codecData.handle),
                            pTxLinkedListDesc, 1, 0, 0,
                            appData.codecData.txbufferObject1,
                            appData.codecData.bufferSize1);               
                }

                appData.pingPong = 3 - appData.pingPong;
                
                appData.freqChange = false;
            }
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
                    
                    appData.freqChange = true;
                                     
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
//******************************************************************************

void Audio_Codec_TxBufferComplete(void)
{
    // not used
}


/*******************************************************************************
 End of File
 */
