/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h for usb_speaker

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
Copyright (c) 2012-2019 released Microchip Technology Inc.  All rights reserved.

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

//configuration.h contains defines that use API definitions from devices
//and libraries in system_definitions.h.  However, system_definitions.h
//should not be included here (it includes app.h).  So just copy the 
//#includes required for system_config.h from system_definitions.h
#include "configuration.h"
#include "definitions.h"
#include "peripheral/clk/plib_clk.h"
#include "peripheral/nvic/plib_nvic.h"
#include "peripheral/pio/plib_pio.h"
#include "bsp/bsp.h"
#include "system/int/sys_int.h"
#include "system/ports/sys_ports.h"
#include "osal/osal.h"

#include "peripheral/twihs/plib_twihs0.h"
#include "system/time/sys_time.h"
#include "driver/i2c/drv_i2c.h"

#include "audio/driver/i2s/drv_i2s.h"
#include "audio/driver/codec/wm8904/drv_wm8904_local.h"
#include "audio/driver/codec/wm8904/drv_wm8904.h"

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

//==============================================================================
//NOTE: The following corresponds to the USB interfaces and must match the
//      USB descriptor generated to system_init.c
//      48 samples in frame buffer
//      2x2=4  bytes per 16bit stereo sample
//      4x48=192 bytes in frame buffer
//==============================================================================

#define APP_BSUBFRAMESIZE_3     //4bytes per 24 bit sample

#define APP_MAX_NO_OF_SAMPLES_IN_A_USB_FRAME 96

#ifdef APP_BSUBFRAMESIZE_3 
#define APP_USB_SAMPLE_SIZE_BYTES  6
#else
#define APP_USB_SAMPLE_SIZE_BYTES  8
#endif //APP_BSUBFRAMESIZE_3

#define APP_CODEC_SAMPLE_SIZE_BYTES 8
#define APP_MAX_NO_BYTES_IN_USB_BUFFER (APP_MAX_NO_OF_SAMPLES_IN_A_USB_FRAME*APP_USB_SAMPLE_SIZE_BYTES)
#define APP_MAX_NO_BYTES_IN_CODEC_BUFFER (APP_MAX_NO_OF_SAMPLES_IN_A_USB_FRAME*APP_CODEC_SAMPLE_SIZE_BYTES)

#define APP_ID_FEATURE_UNIT                       0x05
#define USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID     0x00
#define APP_ID_INPUT_TERMINAL                     0x01
#define APP_ID_OUTPUT_TERMINAL                    0x02
#define USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1 0x01

#define SAMPLING_RATE_96000   96000
#define SAMPLING_RATE_48000   48000
#define SAMPLING_RATE_32000   32000
#define SAMPLING_RATE_24000   24000
#define SAMPLING_RATE_16000   16000
    
//TODO:Define from the configuration.h
//Set queue size the same for the Codec and USB modules
//#define APP_PLAYBACK_QUEUE_BUFFER_SIZE 64
//Initialize from the I2S RX Queue value (which should match the USB endpoint 0
//queue and USB audio queue values)
//#define APP_PLAYBACK_QUEUE_BUFFER_SIZE QUEUE_SIZE_RX_IDX0

#define AUDIO_FORMAT_WIDTH  16
//#define AUDIO_FORMAT_WIDTH  32  

//USB RX Queue Size should be same as APP_QUEUING_DEPTH
#define APP_QUEUING_DEPTH  DRV_I2S_QUEUE_SIZE_IDX0                 

//QUEUE Initial Reads
//N, where N gives the fraction of the QUEUE to be filled, i.e. 1/N
//and USB reads generated and completed and Codec AddWrites before
//transitioning to APP_PROCESS_DATA timing. 
#define QUEUE_USB_INIT_PART  2  
#define APP_INIT_QUEUE_LEVEL  (APP_QUEUING_DEPTH/QUEUE_USB_INIT_PART) 

//Buttons
#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000
#define VOLUME_STEPS    4 

// *****************************************************************************
/* Application States
States
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
    APP_STATE_INITIAL_USB_READ_REQUEST,
    APP_STATE_INITIAL_CODEC_WRITE_REQUEST,
    APP_PROCESS_DATA,
    APP_MUTE_AUDIO_PLAYBACK,
    APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD,
    APP_SAMPLING_FREQUENCY_CHANGE,
    APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD_HP, //headphone
    APP_IDLE,
    APP_STATE_ERROR,
} APP_STATES;

// *****************************************************************************
/* Application Audio Control

  Summary:
    Defines a list of audio controls supported by this application.

  Description:
    This enumeration defines audio controls supported by this application. This
 could be used for checking which Control Transfer is currently active.
*/
typedef enum
{
    APP_USB_CONTROL_NONE,
    APP_USB_AUDIO_MUTE_CONTROL,
    APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP,
} APP_AUDIO_CONTROLS;


typedef enum
{
    APP_USB_SPEAKER_PLAYBACK_NONE   = 0,
    APP_USB_SPEAKER_PLAYBACK_STEREO = 1
} APP_ALTERNATE_SETTINGS;

//Button States
typedef enum
{
    BUTTON_STATE_IDLE=0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_BUTTON0_PRESSED,                   
    BUTTON_STATE_WAIT_FOR_RELEASE    
} BUTTON_STATES;

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
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    size_t bufferSize;
} APP_CODEC_CLIENT;

//LED State
typedef enum
{
    LED_OFF = 0,     //LED OFF - No Blink
    LED_ON,          //LED ON - No Blink
    MUTE_BLINK,       //Fast 1 sec blink
    CONNECTED_BLINK,  //Slow 2 sec blink
    LED_ERROR_BLINK,  //Very Fast Blink
} LED_STATE;

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
    APP_STATES lastState;  //Debug info

     /* device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE   usbDevHandle;

    /* Instance number of Audio Function driver */
    USB_DEVICE_AUDIO_INDEX audioInstance;

    /* device configured state */
    bool isConfigured;

    bool codecConfigured;

    uint32_t usbReadBufSize;

    /* Holds active value of USB Audio Streaming Interface Alternate setting.
     * Every USB Audio Streaming should have 1 or more Alternate settings.
     * Alternate setting Zero (or Default Alternate setting) never has any
     * data payload. An audio streaming can have multiple number of alternate
     settings each having it own sampling rate, bandwidth etc. */
    APP_ALTERNATE_SETTINGS activeInterfaceAlternateSetting;
    
    APP_ALTERNATE_SETTINGS activeMicInterfaceAlternateSetting;

    /* Holds current value of Audio Control Mute settings. A value True indicates
     * audio is currently muted. */
     bool dacMute;
     
     uint32_t sampleFreq;
     
    /* This is to indicate which Audio Control Transfer is currently
     * in progress. */
    APP_AUDIO_CONTROLS currentAudioControl;

    /* CODEC client handle */ 
    APP_CODEC_CLIENT codecClientWrite;

    //Codec Volume
    int volume;
    int volumeIndex;
    bool muteEn;

    //Button Handler
    DRV_HANDLE tmrHandle;
    bool volMuteMode;
    uint32_t buttonDelay;
    BUTTON_STATES buttonState;

    LED_STATE ledState;
    int       blinkDelay;

    bool lrSync;
    bool clockStableDelayFlag;
    int32_t clockStableDelayMs;

} APP_DATA;


// Application Codec Playback Buffer Queue
typedef struct
{
#ifdef APP_BSUBFRAMESIZE_3
    // 96*3*2 = 32*18
    uint8_t buffer[APP_MAX_NO_BYTES_IN_USB_BUFFER]; //24bit packed sample buffer
#endif //APP_BSUBFRAMESIZE_3
    uint8_t buffer32[APP_MAX_NO_BYTES_IN_CODEC_BUFFER]; //32 bit sample buffer
    DRV_CODEC_BUFFER_HANDLE codecWriteHandle;        //4 bytes (ptr)
    USB_DEVICE_AUDIO_TRANSFER_HANDLE usbReadHandle;  //4 bytes (ptr)
    bool codecInUse;   //1 byte   (Buffer queued to write)
    bool usbInUse;     //1 byte   (Buffer queued for read)
    bool usbReadCompleted;  //1 byte (Buffer read completed)
    //char padding[5];   //MIPS    
    char padding[21];    //SAM aligned 32:  32 - 11 
}APP_PLAYBACK_BUFFER;


//NOTE: APP_PLAYBACK BUFFER is what is transferred using DMA--> it is padded
// and placed at the beginning of the struct and the struct attribute is the
// allocated with the COHERENT and aligned(16) (MIPS) or aligned(32)(E70) 
// attributes
typedef struct
{
    APP_PLAYBACK_BUFFER playbackBuffer[APP_QUEUING_DEPTH];
    uint8_t  codecWriteIdx;               //Next Buffer for Codec TX
    uint8_t  usbReadIdx;                  //Next Buffer for USB RX 
    uint8_t  usbReadCompleteBufferLevel;  //#of read buffers ready to write
    uint8_t  previousBufferLevel;         //for Change in buffer level
    uint32_t usbReadCompleteCnt;          //
    uint32_t  usbReadQueueCnt;             //#of usb Reads queued
    uint32_t codecWriteQueueCnt;          //Total #of codec writes queued
    uint32_t codecWriteCompleteCnt;       //Total #of codec writes completed
    //uint8_t  padding[16];                 //Aligned 32:  32 - 16
} APP_PLAYBACK_BUFFER_QUEUE;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Interface Routines
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

void APP_CODECBufferEventHandler(DRV_CODEC_BUFFER_EVENT event, 
                                 DRV_CODEC_BUFFER_HANDLE handle, 
                                 uintptr_t context );



//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */
/*******************************************************************************
 End of File
 */
