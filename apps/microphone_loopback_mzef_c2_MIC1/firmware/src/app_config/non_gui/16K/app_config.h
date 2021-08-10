/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_config.h

  Summary:
    This header file provides prototypes and definitions for the application,
    specific to this configuration.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END
 
#define MAX_AUDIO_NUM_SAMPLES   160     // 10 ms of audio at 16,000 samples/second    

#define MAX_BUFFERS             150     // first two buffers used for ping-pong
                                        // buffers; > 2 buffers used for delay     
    
#define INIT_BUFFER_DELAY_IDX     2     // init for 1 sec delay
    
#define INIT_VOLUME_IDX           1
    
#define VOLUME_STEPS              4 
    
#define DELAY_STEPS               4       

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct 
{
    /* Left channel data */
    int32_t leftData;

    /* Right channel data */
    int32_t rightData;

} DRV_I2S_DATA32m;

#define DRV_I2S_DATA DRV_I2S_DATA32m

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
            
    /* TODO: Define states used by the application state machine. */
    APP_STATE_CODEC_OPEN,
    APP_STATE_CODEC_SET_BUFFER_HANDLER,
    APP_STATE_CODEC_ADD_BUFFER,
    APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE,                 
} APP_STATES;      

typedef enum
{
    BUTTON_STATE_IDLE=0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_BUTTON0_PRESSED,                   
    BUTTON_STATE_WAIT_FOR_RELEASE    
} BUTTON_STATES;

// *****************************************************************************
// CODEC Data
//
//  Summary:
//    Holds codec data
//
//  Description:
//    This structure holds the codec's data.
//
//  Remarks:
// *****************************************************************************
typedef struct
{
    DRV_HANDLE handle;
    DRV_CODEC_BUFFER_HANDLE writeBufHandle;
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    uint8_t *txbufferObject;
    uint8_t *rxbufferObject;
    size_t bufferSize;
} AUDIO_CODEC_DATA;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
   /* The application's current state */
    APP_STATES state;
    
    /* handle to opened timer */
    DRV_HANDLE tmrHandle;
    
    uint8_t volume;

    AUDIO_CODEC_DATA codecData;

    uint16_t txBufferIdx, rxBufferIdx;
    
    bool delayEnabled; 
    
    uint32_t buttonDelay;
    
    BUTTON_STATES buttonState;
    
    bool buttonMode;
    
    uint8_t volumeIndex;
    
    uint8_t delayTableIndex;   

} APP_DATA;

// make E70 board SWITCH compatible with V71 board SWITCH0
#ifdef SWITCH0_STATE_PRESSED
#define SWITCH_Get             SWITCH0_Get
#define SWITCH_STATE_PRESSED   SWITCH0_STATE_PRESSED
#else
#ifdef SWITCH1_STATE_PRESSED
#define SWITCH_Get             SWITCH1_Get
#define SWITCH_STATE_PRESSED   SWITCH1_STATE_PRESSED
#endif
#endif

#ifdef LED1_On
#define LED_On                 LED1_On
#define LED_Off                LED1_Off
#define LED_Toggle             LED1_Toggle
#endif

#endif /* APP_CONFIG_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


