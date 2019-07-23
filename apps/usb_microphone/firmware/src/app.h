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

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

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
#include "configuration.h"
#include "definitions.h"
//#include "app_config.h"  //Selected by project configuration

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

//Buttons
#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000
#define GAIN_STEPS    4 


//LED State
typedef enum
{
    LED_OFF = 0,     //LED OFF - No Blink
    LED_ON,          //LED ON - No Blink
    MUTE_BLINK,       //Fast 1 sec blink
    CONNECTED_BLINK,  //Slow 2 sec blink
    LED_ERROR_BLINK,  //Very Fast Blink
} LED_STATE;

//Button States
typedef enum
{
    BUTTON_STATE_IDLE=0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_BUTTON0_PRESSED,                   
    BUTTON_STATE_WAIT_FOR_RELEASE    
} BUTTON_STATES;

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

//USB Microphone Demo Descriptor Definitions
#define APP_ID_INPUT_TERMINAL      0x01
#define APP_ID_OUTPUT_TERMINAL     0x02
#define APP_ID_MIXER_UNIT          0x03
#define APP_ID_SELECTOR_UNIT       0x04
#define APP_ID_FEATURE_UNIT        0x05
#define APP_ID_PROCESSING_UNIT     0x06
#define APP_ID_EXTENSION_UNIT      0x07

#define APP_ID_INPUT_TERMINAL_MIC  0x04
#define APP_ID_FEATURE_UNIT_MIC    0x05
#define APP_ID_OUTPUT_TERMINAL_MIC 0x06

#define USB_DEVICE_AUDIO_MAX_INTERFACE_COLLECTIONS      1
#define USB_DEVICE_AUDIO_NUM_FEATURE_UNITS              1
#define USB_DEVICE_AUDIO_FEATURE_UNIT_ID                APP_ID_FEATURE_UNIT
#define USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID           0x00
#define USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1       0x01
#define USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_2       0x02
#define USB_DEVICE_HID_INTERFACE_ID                     0x03

// *****************************************************************************
/* Application Audio Control

  Summary:
    Defines a list of audio controls supported by this application.

  Description:
    This enumeration defines audio controls supported by this application. This
    could be used for checking which Control Transfer is currently active.
 
 NOTE:  Must match the Full Speed Descriptor used for enumerations 
        (usb_device_init_data.c)
*/

//USB Device - Controls
typedef enum
{
    APP_USB_CONTROL_NONE,
    APP_USB_AUDIO_MUTE_CONTROL
} APP_AUDIO_CONTROLS;


//USB Device - Alternate Settings
typedef enum
{
    APP_USB_SPEAKER_PLAYBACK_NONE   = 0,
    APP_USB_SPEAKER_PLAYBACK_STEREO = 1,
} APP_ALTERNATE_PLAYBACK_SETTINGS;

typedef enum
{
    APP_USB_MIC_RECORD_NONE         = 0,
    APP_USB_MIC_RECORD_MONO         = 1,
} APP_ALTERNATE_RECORD_SETTINGS;

// *****************************************************************************
/* Application States

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
    APP_STATE_CODEC_OPEN,
    APP_STATE_CODEC_SET_BUFFER_HANDLER,
    APP_STATE_USB_DEVICE_OPEN,
    APP_STATE_WAIT_FOR_CONFIGURATION,
    APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE,
    APP_STATE_INITIALIZE_CODEC_QUEUE,
    APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE,
    APP_STATE_CODEC_READ,
    APP_STATE_MUTE_AUDIO_PLAYBACK,
    APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD,
    APP_STATE_MIC_USB_INTERFACE_ALTERNATE_SETTING_RCVD,
    APP_STATE_IDLE,
    APP_STATE_ERROR,
    NUM_APP_STATE,
} APP_STATES;

// *****************************************************************************
/* Application USART client for BT

  Summary:
    Application USART client for BT.

  Description:
    This object holds the BT USART's client handle, read and write buffer handle
    created and the context
*/
typedef struct
{
    DRV_HANDLE handle;
    DRV_CODEC_BUFFER_HANDLE readBufHandle[2];
    bool isReadBufHandleAvailable[2];
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    uint8_t *rxbufferObject;
    uint8_t *txbufferObject;
    uint8_t rxBufIdx;
    uint8_t txBufIdx;

    /* Codec WriteRead Complete to rxBufIdx */
    bool isCodecReadComplete;
    size_t bufferSize;
} APP_CODEC_CLIENT;

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
    /* Application's current state*/
    volatile APP_STATES state;

     /* device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE   usbDevHandle;

    /* Instance number of Audio Function driver */
    USB_DEVICE_AUDIO_INDEX audioInstance;

    /* USBdevice in configured state */
    bool isConfigured;

    bool codecConfigured;

    
    bool isUsbWriteComplete[2];
    USB_DEVICE_AUDIO_TRANSFER_HANDLE writeTransferHandle[2];
    
    /* Holds active value of USB Audio Streaming Interface Alternate setting.
     * Every USB Audio Streaming should have 1 or more Alternate settings.
     * Alternate setting Zero (or Default Alternate setting) never has any
     * data payload. An audio streaming can have multiple number of alternate
     settings each having it own sampling rate, bandwidth etc. */
    APP_ALTERNATE_PLAYBACK_SETTINGS activeInterfaceAlternateSetting;
    APP_ALTERNATE_RECORD_SETTINGS activeInterfaceAlternateSettingMic;
    uint8_t activeInterface;

    /* Holds current value of Audio Control Mute settings. A value True indicates
     * audio is currently muted. */
     bool dacMute;

     unsigned char midCtrl;
     unsigned char midMax;
     unsigned char midMin;
     unsigned char midRes;
     bool autoGain;

    /* This is to indicate which Audio Control Transfer is currently
     * in progress. */
    APP_AUDIO_CONTROLS currentAudioControl;

    APP_CODEC_CLIENT codecClient;

    //Codec Volume
    uint8_t       micGain;
    int           micGainIndex;

    //Button Handler
    DRV_HANDLE    tmrHandle;
    bool          gainMuteMode;
    uint32_t      buttonDelay;
    BUTTON_STATES buttonState;

    LED_STATE     ledState;
    int           blinkDelay;
    int           sampleFreq;

    bool          lrSync;

} APP_DATA;

//typedef struct
//{
//    APP_MIC_STATES state;
//
//} APP_MIC_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.*/
void APP_USBDeviceEventHandler
(
    USB_DEVICE_EVENT events,
    void * eventData,
    uintptr_t context
);

void APP_USBDeviceAudioEventHandler
(
    USB_DEVICE_AUDIO_INDEX iAudio ,
    USB_DEVICE_AUDIO_EVENT event ,
    void * pData,
    uintptr_t context
);


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
void APP_Tasks ( void );

/*******************************************************************************
  Function:
    void APP_CODECBufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
        DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context )

  Summary:
    Event Handler for codec Task.

  Description:
    This is the Event Handler for Codec Tx Complete Events.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
*/
void APP_CODECBufferEventHandler(DRV_CODEC_BUFFER_EVENT event, DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

