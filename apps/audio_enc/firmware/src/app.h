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

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"
#include "user.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************

// *****************************************************************************
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
    DRV_CODEC_BUFFER_HANDLE rdBufHandle;
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    uint8_t *rxbufferObject;
    uint8_t *txbufferObject;
    size_t bufferSize;
} AUDIO_CODEC_DATA;

extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Input_Buffer[2][NUM_SAMPLES];
extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer[2][NUM_SAMPLES];

//  512 samples
//  Audio       Sample Rate
//  64ms        8K
//  32ms        16K
//  ??ms        44.1K
//  10.6ms      48K
//  5.3ms       96K

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
    APP_STATE_INIT,
    APP_STATE_CODEC_OPEN,
    APP_STATE_CODEC_SET_BUFFER_HANDLER,
    APP_STATE_WAITING_FOR_USB_MSD,
    APP_STATE_SET_SAMPLERATE,
    APP_STATE_WAITING_FOR_START,
    APP_STATE_CODEC_READ,
    APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE,
    APP_STATE_SET_FILENAME,
    APP_STATE_OPEN_ENCODE_FILE,
    APP_STATE_ENCODE_INIT,
    APP_STATE_ENCODE_AND_WRITE_RESULT,
    APP_STATE_CLOSE_FILE,
    APP_STATE_OPEN_PLAYBACK_FILE,
    APP_STATE_DECODE_INIT,
    APP_STATE_DECODE_AND_PLAYBACK,
    APP_STATE_ERROR,
} APP_STATES;

/* State machine for bringing up the USB MSD */
typedef enum
{
    USB_STATE_INIT,
	USB_STATE_BUS_ENABLE,
    USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
    USB_STATE_WAIT_FOR_DEVICE_ATTACH,
    USB_STATE_DEVICE_CONNECTED,
    USB_STATE_IDLE,
} USB_STATES;

typedef enum
{
    BTN_STATE_IDLE,
    BTN_STATE_PRESSED,
    BTN_STATE_BUTTON0_PRESSED,                   
    BTN_STATE_WAIT_FOR_RELEASE    
} BTN_STATES;

typedef enum
{
    LED_STATE_ON,
    LED_STATE_OFF,
    LED_STATE_TOGGLE,
    LED_STATE_BLINK,
} LED_STATES;

/* LED status */
//  LED1    On                  I2SC mode; Sample rate is set at compile time
//  LED1    Off                 SSC mode; 0 sample rate; Shouldn't be allowed to happen
//  LED1    1 blink/_500ms Off  SSC mode; 8K sample rate
//  LED1    2 blink/_500ms Off  SSC mode; 16K sample rate
//  LED1    3 blink/_500ms Off  SSC mode; 48K sample rate
//  LED1    4 blink/_500ms Off  SSC mode; 96K sample rate

//  LED2    On                  Long press  ->  Enter sample rate selection
//  LED2    On                  Short press ->  Use current sample rate ( 0 not allowed )
//  LED2    _500ms Toggle       Long press  ->  Cycle through sample rates
//  LED2    _500ms Toggle       Short press ->  Select current sample rate


typedef enum
{
    _8K,
    _16K,
    _44_1K,
    _48K,
    _96K,
    MAX_SAMPLE_RATE_MODES,
} CODEC_SAMPLE_RATE_MODE;

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
    
    /* SYS_FS File handle for 1st file */
    SYS_FS_HANDLE fileHandle, dataFileHandle;
    SYS_FS_FSTAT fileStatus, dataFileStatus;
    // Handle for data file
    char fileName[50], ext[4], srStr[6], bdStr[4];

    /* handle to opened timer */
    DRV_HANDLE tmrHandle;
    AUDIO_CODEC_DATA codecData;
    CODEC_SAMPLE_RATE_MODE sampleRateMode;
    uint32_t sampleRate;
    bool setSampleRate, encoding, pingPong, headphone_out, lrSync, playback;
    uint8_t rxBufIdx, txBufIdx;
    uint32_t closeDelay, errorCnt;
    int32_t encCmpltEvnt;
} APP_DATA;

typedef struct
{
    USB_STATES state;
    bool deviceIsConnected;
} USB_DATA;

typedef struct
{
    BTN_STATES state;
    uint32_t delay;
} BTN_DATA;

typedef struct
{
    LED_STATES led1State;
    LED_STATES led2State;
    uint32_t led1Delay;
    uint32_t led2Delay;
    uint32_t led1Period;
    uint32_t led2Period;
    uint32_t led1Blinks;        // Number of blinks at _100ms on/off rate
    uint32_t led2Blinks;
    uint32_t led1BlinkCnt;      // Number of remaining blinks before _1sec off
    uint32_t led2BlinkCnt;
    bool led1On;
    bool led2On;
} LED_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

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

bool APP_Set_FileName( void );
void APP_Set_FileExt( void );
void APP_Set_SampleRate( void );
void APP_Set_BitDepth( void );
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
    DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context);
void USB_Tasks( void );
void BTN_Tasks( void );
void LED_Tasks( void );
void LED_Set_Mode( uint8_t led, LED_STATES state, uint32_t prd);

#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

