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

#include "gfx/libaria/libaria_events.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

uint32_t pulseDurations[3] = {1, 5, 10};

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
    Called every 1 ms by timer services.  It then decrements appData.buttonDelay if
    non-zero.

  Parameters:
    context      - Contains index into driver's instance object array

  Remarks:
    None
*/

static volatile uint32_t msCounter;

// gets called every ms 
void App_TimerCallback( uintptr_t context)
{
    if (appData.buttonDelay)
    {      
        appData.buttonDelay--;
    }        
    msCounter++;

#ifdef LIBARIA_EVENTS_DEFINED    // if project is regenerated via Bamboo, this will remain undefined 
    KeyRepeatTask();    // process touch key-repeat events in libaria_events.c
#endif    

    //LED1_Toggle();      // uncomment to calibrate timer -- should toggle every 1 ms
}

static uint32_t _calcDelayLoop()
{
    uint32_t loopCtr;
    msCounter = loopCtr = 0;
    while (msCounter==0);
    while (msCounter<2)
    {
        loopCtr++;
    }
    return loopCtr;        
}

void SetAppVolume(uint32_t perCentVolume)
{
    if (appData.codecData.handle != DRV_HANDLE_INVALID)
    {
        uint32_t oldVolume = appData.volume;
        appData.volume = 255*perCentVolume / 100;
        if (0==appData.volume)
        {
            DRV_CODEC_MuteOn(appData.codecData.handle);                       
        }
        else
        {
            DRV_CODEC_VolumeSet(appData.codecData.handle,
                DRV_CODEC_CHANNEL_LEFT_RIGHT, appData.volume);                                      
            if (0==oldVolume)
            {
                // if volume was 0, unmute codec
                DRV_CODEC_MuteOff(appData.codecData.handle);                     
            }
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

void initSweep()
{
    appData.startNumSamples = (uint32_t)((double)appData.sampleRate / (double)appData.startFrequency);  // # of samples for one cycle
    double startPeriod = 1000000.0*(1.0/(double)appData.startFrequency);        // length of one cycle, in 탎
    double endPeriod;
    if (appData.sweepEnabled)
    {
        appData.endNumSamples = (uint32_t)((double)appData.sampleRate / (double)appData.endFrequency);
        endPeriod = 1000000.0*(1.0/(double)appData.endFrequency);
    }
    else
    {
        appData.endNumSamples = (uint32_t)((double)appData.sampleRate / (double)appData.startFrequency);
        endPeriod = 1000000.0*(1.0/(double)appData.startFrequency);       
    }
    double avgPeriod = (startPeriod + endPeriod) / 2.0;
    double totalNumberCycles = (1000.0*(double)appData.duration) / avgPeriod;
    appData.cyclesPerFreq = (uint32_t)(totalNumberCycles / (abs(appData.startNumSamples - appData.endNumSamples) + 1));
    if (appData.cyclesPerFreq < 1)
    {
        appData.cyclesPerFreq = 1;       // currently must have steps of 1 cycle
    }
    appData.cyclesPerFreqCtr = appData.cyclesPerFreq;
    
    appData.currNumSamples = appData.startNumSamples;       // this value is decremented when a new loop begins
    // compute how many loops of the same currNumSamples we need to use
    appData.loopsPerBuffer = (uint32_t)(((double)(appData.cyclesPerFreqCtr*appData.currNumSamples)/
            (double)MAX_AUDIO_NUM_SAMPLES) + 1.0);   
    appData.cyclesPerBuffer = appData.cyclesPerFreqCtr / appData.loopsPerBuffer;
    appData.currLoop = appData.loopsPerBuffer;   
}

void fillInNumSamplesTable(uint8_t bufferNum)
{
    uint16_t i;
    
    // for each cycle:
    if (bufferNum==1)
    {
        appData.numNumSamples1 = appData.cyclesPerBuffer;        
        for (i=0; i < appData.cyclesPerBuffer; i++)
        {
            appData.numSamples1[i] = appData.currNumSamples;
        }
    }
    else
    {
        appData.numNumSamples2 = appData.cyclesPerBuffer;         
        for (i=0; i < appData.cyclesPerBuffer; i++)
        {
            appData.numSamples2[i] = appData.currNumSamples;
        }       
    }
    appData.currLoop--;
    if (appData.currLoop)
    {
        if (appData.cyclesPerFreqCtr > appData.cyclesPerBuffer)
        {
            appData.cyclesPerFreqCtr -= appData.cyclesPerBuffer;
        }
        else
        {
            appData.cyclesPerBuffer = appData.cyclesPerFreqCtr;
        }
    }
    else
    {
        appData.cyclesPerBuffer = 0;
    }
    
    if (appData.cyclesPerBuffer==0)
    {
        appData.currNumSamples--;
        if (appData.currNumSamples < appData.endNumSamples)
        {
            appData.currNumSamples = appData.startNumSamples;
            if (false==appData.loopEnabled)
            {
#ifdef LIBARIA_EVENTS_DEFINED    // if project is regenerated via Bamboo, this will remain undefined                 
                HiddenPlayPauseButton_ReleasedEvent(NULL);      // simulate pressing and releasing button
#endif                
            }
            initSweep();
            
        }
        else
        {        
            appData.cyclesPerFreqCtr = appData.cyclesPerFreq;
            // compute how many loops of the same currNumSamples we need to use
            appData.loopsPerBuffer = (uint32_t)(((double)(appData.cyclesPerFreqCtr*appData.currNumSamples)/
                    (double)MAX_AUDIO_NUM_SAMPLES) + 1.0);
            appData.cyclesPerBuffer = appData.cyclesPerFreqCtr / appData.loopsPerBuffer;
            appData.currLoop = appData.loopsPerBuffer;     
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
    appData.volume = 25*255/100;    // 25%    
    
    appData.buttonDelay = 0;
    appData.buttonMode = false;
    
    appData.sampleRate = 48000;
    
    appData.wave_mode = SINE_WAVE;    
    
    _audioCodecInitialize (&appData.codecData);

    appData.sampleTableIndex = INIT_SAMPLE_INDEX; 
   
    appData.pingPong = 1;
    
    appData.freqChange = false;
    
    firstSync = true;
    
    appData.newVolume = -1;
    
    appData.sweepEnabled = false;

    appData.loopEnabled = true;
    
    appData.playOrPaused = false;
    
    appData.lastPlayOrPaused = false;
      
#ifdef LIBARIA_EVENTS_DEFINED    // if project is regenerated via Bamboo, this will remain undefined    
    InitLabels();
#endif    
    
    initSweep();    

}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
DRV_HANDLE tmrHandle=SYS_TIME_HANDLE_INVALID;
DRV_HANDLE tmrHandle2=SYS_TIME_HANDLE_INVALID;

static uint32_t delayFactor;
static void _toggleTriggerOutputSub()
{
// delayFactor set up in _calcDelayLoop as # of times loop executed in 1000 ms,
// so this varies depending on clock frequency    
#define DELAY_FACTOR_DIVISOR 660  // adjusted so delayFactor/DELAY_FACTOR_DIVISOR corresponds to approx. 1 탎
    uint32_t i;
    uint32_t pulseWidth = pulseDurations[appData.pulseDurIdx];     // in 탎, settable to 1, 5, or 10 탎 
    uint32_t ctr = (delayFactor * pulseWidth) / DELAY_FACTOR_DIVISOR;
    TRIGGER_OUTPUT_Set();
    for (i=0; i < ctr; i++)
    {
           __NOP();
    }
    TRIGGER_OUTPUT_Clear(); 
}

void App_TimerCallback2( uintptr_t context)
{
    _toggleTriggerOutputSub();
}

void _toggleTriggerOutput()
{
    uint32_t delayTrigger;
    
    delayTrigger = (1000*appData.duration*appData.pulseDelay)/100;
    if (delayTrigger>=100)
    {
        tmrHandle2 = SYS_TIME_CallbackRegisterUS(App_TimerCallback2, 
                (uintptr_t)0, delayTrigger, SYS_TIME_SINGLE);
    }
    else
    {
        __ASM volatile ("cpsid if");    // disable interrupts briefly so to generate steady pulse        
        _toggleTriggerOutputSub();
        __ASM volatile ("cpsie if");    // re-enable interrupts        
    }
}

void APP_Tasks ( void )
{       
    if (appData.newVolume != -1)    // global set up in libaria_events.c
    {
        SetAppVolume(appData.newVolume);
        appData.newVolume = -1;
    }
    
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
               _calcDelayLoop();                
               delayFactor = _calcDelayLoop(); 
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

            APP_TONE_LOOKUP_TABLE_Initialize(appData.wave_mode, &appData.codecData, 1,
                    appData.sampleRate, appData.numSamples1, appData.numNumSamples1);
                                 
            appData.state = APP_STATE_CODEC_ADD_BUFFER;           
        }
        break;
       
        case APP_STATE_CODEC_ADD_BUFFER:
        {
            if (appData.playOrPaused)
            {
                if (false==appData.lastPlayOrPaused) 
                {                
                    appData.pingPong = 1;
                    
                    initSweep();                    
                    fillInNumSamplesTable(1);                  

                    APP_TONE_LOOKUP_TABLE_Initialize(appData.wave_mode, &appData.codecData, 1,
                        appData.sampleRate, appData.numSamples1, appData.numNumSamples1);
            
                    firstSync = true;
                }
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
                        _toggleTriggerOutput();                    

                        // just sent buffer 1 to DMA, so fill in buffer 2 for next time
                        fillInNumSamplesTable(2);

                        APP_TONE_LOOKUP_TABLE_Initialize(appData.wave_mode, &appData.codecData, 2,
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
                        _toggleTriggerOutput();                    

                        // just sent buffer 2 to DMA, so fill in buffer 1 for next time 
                        fillInNumSamplesTable(1);

                        APP_TONE_LOOKUP_TABLE_Initialize(appData.wave_mode, &appData.codecData, 1,
                                appData.sampleRate, appData.numSamples1, appData.numNumSamples1);

                        appData.pingPong = 1;
                        appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                    }               
                }
            }
            
            appData.lastPlayOrPaused = appData.playOrPaused;
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

/*******************************************************************************
 End of File
 */
