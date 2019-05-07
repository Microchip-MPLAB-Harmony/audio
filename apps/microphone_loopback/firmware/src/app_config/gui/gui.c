/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the GUI versions of the application.

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
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

// called every millisecond from App_TimerCallback
void KeyRepeatTask ( void )
{
#ifdef LIBARIA_EVENTS_DEFINED    // if project is regenerated via Bamboo, this will remain undefined     
    KeyRepeatTask_sub();    // in turn calls routine in libaria_events.c
#endif    
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void _setAppVolume(uint32_t perCentVolume)
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

static uint8_t oldMicGain;

void _setAppMicGain(uint32_t perCentMicGain)
{
    uint8_t micGain = 31*perCentMicGain / 100;
    
    if (appData.codecData.handle != DRV_HANDLE_INVALID)
    {
        if (0==micGain)
        {
            DRV_CODEC_MicMuteOn(appData.codecData.handle);
        }
        else
        {
            DRV_CODEC_MicGainSet(appData.codecData.handle, micGain); 
            if (0==oldMicGain)
            {
                DRV_CODEC_MicMuteOff(appData.codecData.handle);   
            }            
        }
    }
    oldMicGain = micGain;
}

static uint8_t oldDelayEnabled;
static uint32_t oldDelay;

void _setAppDelay(uint32_t delay)
{   
    appData.txBufferIdx = 0;
    appData.rxBufferIdx = delay/10;

    uint16_t i,j;
    for (i=0; i < MAX_BUFFERS; i++)
    {
        for (j=0; j < MAX_AUDIO_NUM_SAMPLES; j++)
        {
            micBuffer[i][j].leftData = 0;
            micBuffer[i][j].rightData = 0;
        }
    }

    appData.codecData.txbufferObject = (uint8_t *) micBuffer[appData.txBufferIdx];
    appData.codecData.rxbufferObject = (uint8_t *) micBuffer[appData.rxBufferIdx]; 
    
    oldDelay = delay;
}

/******************************************************************************
  Function:
    void APP_Tasks_sub ( void )

  Remarks:
    called from APP_Tasks
 */

void APP_Initialize_sub ( void )
{
    appData.volume = 255*INITIAL_VOLUME_PERCENT / 100;  // for initial setting in APP_STATE_CODEC_OPEN
    
    appData.newVolume = -1;    
    appData.newDelay = -1;
    appData.newMicGain = -1;
    
    oldDelayEnabled = appData.delayEnabled;
    
    appData.txBufferIdx = 0;
    appData.rxBufferIdx = 1;     
   
    if (appData.delayEnabled)
    {
        _setAppDelay(INITIAL_DELAY);
    }
    else
    {
        oldDelay = INITIAL_DELAY;   
    }
#ifdef LIBARIA_EVENTS_DEFINED    // if project is regenerated via Bamboo, this will remain undefined   
    InitGUI();         // located in libaria_events.c
#endif    
}

void APP_Tasks_sub ( void )
{
    if (appData.state > APP_STATE_CODEC_OPEN)
    {
        if (appData.delayEnabled != oldDelayEnabled)
        {
            if (appData.delayEnabled)
            {
                _setAppDelay(oldDelay);
            }
            else
            {
                appData.txBufferIdx = 0;
                appData.rxBufferIdx = 1;                                      

                appData.codecData.txbufferObject = (uint8_t *) micBuffer[0];
                appData.codecData.rxbufferObject = (uint8_t *) micBuffer[1];             
            }        

            oldDelayEnabled = appData.delayEnabled;
        }

        if (appData.newVolume != -1)    // global set up in libaria_events.c
        {
            _setAppVolume(appData.newVolume);
            appData.newVolume = -1;
        }

        if (appData.newDelay != -1)    // global set up in libaria_events.c
        {
            _setAppDelay(appData.newDelay);
            appData.newDelay = -1;
        } 

        if (appData.newMicGain != -1)    // global set up in libaria_events.c
        {
            _setAppMicGain(appData.newMicGain);
            appData.newMicGain = -1;
        }
    }
}   
