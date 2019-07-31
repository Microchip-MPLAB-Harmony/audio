/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef APP_H
#define APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"
#include "app_config.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END  
   
#define INIT_SAMPLE_INDEX     1  // 500  
    
#define INIT_VOLUME_IDX           1

#define VOLUME_STEPS              4
 
#define SAMPLE_STEPS              4

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

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
    DRV_CODEC_BUFFER_HANDLE writeBufHandle1, writeBufHandle2;
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    uint8_t *txbufferObject1;
    uint8_t *txbufferObject2;
    size_t bufferSize1, bufferSize2;
} AUDIO_CODEC_DATA;

#include "app_tone_lookup_table.h"      // must come after AUDIO_CODEC_DATA  

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
    
    uint32_t buttonDelay;
    
    BUTTON_STATES buttonState;
    
    uint8_t volume;
       
    uint8_t pingPong;
    
    uint32_t sampleRate;  
    
    uint16_t numNumSamples1, numNumSamples2;
    
    uint16_t numSamples1[MAX_AUDIO_NUM_SAMPLES], numSamples2[MAX_AUDIO_NUM_SAMPLES];
    
    AUDIO_CODEC_DATA codecData;
    
    uint8_t sampleTableIndex;
    
    bool buttonMode;
    
    uint8_t volumeIndex;   
    
    uint16_t max_audio_samples;    

} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
//APP: API to CODEC Event Handler
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
    DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );

void APP_Button_Tasks( void );

// make E70 board SWITCH compatible with V71 board SWITCH1
#ifdef SWITCH1_STATE_PRESSED
#define SWITCH_Get             SWITCH1_Get
#define SWITCH_STATE_PRESSED   SWITCH1_STATE_PRESSED
#endif

// make E70 board SWITCH compatible with MZ EF SK board BSP_SWITCH_1
#ifdef BSP_SWITCH_1_STATE_PRESSED
#define SWITCH_Get             BSP_SWITCH_1_Get
#define SWITCH_STATE_PRESSED   BSP_SWITCH_1_STATE_PRESSED
#endif

// make E70 board LED1 compatible with MZ EF SK board BSP_LED_1
#ifdef BSP_LED_1_On
#define LED1_On   BSP_LED_1_On 
#define LED1_Off  BSP_LED_1_Off
#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* APP_H */

