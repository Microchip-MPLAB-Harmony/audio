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
#include "peripheral/clk/plib_clk.h"
#include "peripheral/nvic/plib_nvic.h"
#include "peripheral/pio/plib_pio.h"
#include "bsp/bsp.h"
#include "system/int/sys_int.h"
#include "system/ports/sys_ports.h"
#include "osal/osal.h"
#include "driver/i2s/drv_i2s.h"
#include "driver/i2c/drv_i2c.h"
#include "peripheral/twihs/plib_twihs0.h"
#include "system/time/sys_time.h"

#include "audio_codec.h"
#include "driver/wm8904/drv_wm8904_local.h"
#include "driver/wm8904/drv_wm8904.h"
#include "app_tone_lookup_table.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

#define MAX_AUDIO_NUM_SAMPLES   9600    // for array    

//#define MAX_AUDIO_SAMPLES     480  // 100 Hz
//#define MIN_AUDIO_SAMPLES     24   // 2 kHz
    
#define INIT_SAMPLE_INDEX     1  // 400  
     
#define AUDIO_FORMAT_WIDTH  16
//#define AUDIO_FORMAT_WIDTH  32    

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
    
    /* TODO: Define any additional data used by the application. */       
    //APP: SPI-I2S client handle
    AUDIO_CODEC_DATA codecData; //Client and State 
    
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
void Audio_Codec_TxBufferComplete(void);

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

#endif /* APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */
