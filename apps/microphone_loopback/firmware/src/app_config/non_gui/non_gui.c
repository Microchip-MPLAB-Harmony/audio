/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    non_gui.c

  Summary:
    This file contains the source code for the non-GUI versions of the application.

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

uint16_t volumeLevels[VOLUME_STEPS] =
{
#ifdef DRV_AK4954_VOLUME
    0 /* off */, 192, 208, 224          // if using AK4954, use narrower range  
#else            
    0 /* off */, 128, 192, 255
#endif  
};

uint16_t delayIndices[DELAY_STEPS] =
{
    25, 50, 100, 149     // .25, .50, 1, 1.5 secs            
};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

// called every millisecond from App_TimerCallback
void KeyRepeatTask()
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

void _initDelay()
{
    appData.txBufferIdx = 0;  
    appData.rxBufferIdx = delayIndices[appData.delayTableIndex];    

    uint16_t i,j;
    for (i=0; i < MAX_BUFFERS; i++)
    {
        for (j=0; j < MAX_AUDIO_NUM_SAMPLES; j++)
        {
            micBuffer[i][j].leftData = 0;
            micBuffer[i][j].rightData = 0;
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_Initialize_sub ( void )
{
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */   
    appData.state = APP_STATE_INIT;
    
    appData.buttonDelay = 0;
    appData.buttonMode = appData.delayEnabled = true;      
    
    appData.volumeIndex = INIT_VOLUME_IDX;
    appData.volume = volumeLevels[appData.volumeIndex];         

    appData.delayTableIndex = INIT_BUFFER_DELAY_IDX;
           
    if (appData.delayEnabled)
    {
        LED1_On();
        _initDelay();

    }
    else
    {
        appData.txBufferIdx = 0;
        appData.rxBufferIdx = 1;          
    }
    
    appData.codecData.txbufferObject = (uint8_t *) micBuffer[appData.txBufferIdx];
    appData.codecData.rxbufferObject = (uint8_t *) micBuffer[appData.rxBufferIdx];     
}

/******************************************************************************
  Function:
    void APP_Tasks_sub ( void )

  Remarks:
    called from APP_Tasks
 */

#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000

void APP_Tasks_sub()
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
                if (appData.buttonMode)     // if modifying delay
                {
                    appData.delayTableIndex++;
                    if (appData.delayTableIndex >= DELAY_STEPS)
                    {
                        appData.delayTableIndex = 0;    
                    }
                    
                    _initDelay();      // re-init
                    
                    appData.codecData.txbufferObject = (uint8_t *) micBuffer[appData.txBufferIdx];
                    appData.codecData.rxbufferObject = (uint8_t *) micBuffer[appData.rxBufferIdx];                     
                                     
                    if (0==appData.volume)
                    {
                        // after changing delay, if volume 0 make sure we can hear it
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
                appData.delayEnabled = appData.buttonMode;
                
                if (appData.delayEnabled)
                {
                    LED1_On();
                    
                    _initDelay();
                }
                else
                {
                    LED1_Off();
                    
                    appData.txBufferIdx = 0;
                    appData.rxBufferIdx = 1;                                      
                }
                
                appData.codecData.txbufferObject = (uint8_t *) micBuffer[appData.txBufferIdx];
                appData.codecData.rxbufferObject = (uint8_t *) micBuffer[appData.rxBufferIdx];  
                
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
