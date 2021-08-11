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
    internally by the application (such as the "APP_STATES" definition).  
   
    The application implements a bidirectional USB audio interface that 
    receives stereo USB audio buffered for playback through a CODEC.  The CODEC
    The CODEC converts the stereo audio to a standard line-out which is then 
    can be amplified for speaker output.  At the same time CODEC microphone 
    audio is sent through an echo canceller to remove the echo received through
    the acoustic path to the microphone, allowing near speech to the microphone
    to be buffered and transmitted over the mono USB interface.
   
    The USB inteface is defined with the following characteristics (showing
    excerpt from the fullSpeedConfigurationDescriptor struct in 
    system_config.h:
   
        // USB Microphone Type 1 Format Type Descriptor
        0x11,                  // Size of the descriptor, in bytes (bLength)
        USB_AUDIO_CS_INTERFACE,// CS_INTERFACE Descriptor Type (bDescriptorType)
        USB_AUDIO_FORMAT_TYPE ,// FORMAT_TYPE subtype. (bDescriptorSubtype)
        0x01,                  // FORMAT_TYPE_1. (bFormatType)
        0x01,                  // one channel.(bNrChannels)
        0x02,                  // 2 bytes per audio subframe.(bSubFrameSize)
        0x10,                  // 16 bits per sample.(bBitResolution)
        0x03,                  // 3 frequency supported. (bSamFreqType)
        0x40,0x1F,0x00,        // 8000 Hz
        0x80,0x3E,0x00,        // Sampling Frequency = 16000 Hz(tSamFreq)
        //0xC0,0x5D,0x00,        // 24000
        0x00,0x7D,0x00,        // 32000
        0x80,0xBB,0x00,        // Sampling Frequency = 48000 Hz(tSamFreq)
     
        //USB Headset Type 1 Format Type Descriptor 
        0x11,                  // Size of the descriptor, in bytes (bLength)
        USB_AUDIO_CS_INTERFACE,// CS_INTERFACE Descriptor Type (bDescriptorType)
        USB_AUDIO_FORMAT_TYPE ,// FORMAT_TYPE subtype. (bDescriptorSubtype)
        0x01,                  // FORMAT_TYPE_1. (bFormatType)
        0x02,                  // two channel.(bNrChannels)
        0x02,                  // 2 byte per audio subframe.(bSubFrameSize)
        0x10,                  // 16 bits per sample.(bBitResolution)
        0x03,                  // 3 frequency supported. (bSamFreqType)
        0x40,0x1F,0x00,        // 8000 Hz
        0x80,0x3E,0x00,        // 16000 Hz
        //0xC0,0x5D,0x00,        // 24000 Hz
        0x00,0x7D,0x00,        // 32000 Hz
        0x80,0xBB,0x00,        // Sampling Frequency = 48000 Hz(tSamFreq)
 
    1 ms packets are used for both USB audio stream endpoints.
   
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012-2018 released Microchip Technology Inc.  All rights reserved.

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
//#include "definitions.h"
#include "configuration.h"
#include "definitions.h"

//#if defined(__CORE_CM7_H_GENERIC) || defined(__CORE_CM4_H_GENERIC) 
#if defined(__arm__)
#include "packs/CMSIS/CMSIS/DSP/Include/arm_math.h"
#else
#define sysObjdrvCodec0        sysObj.drvak4954Codec0
#endif

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
//      USB descriptor generated in system_init.c
//==============================================================================

//USB Audio Streaming Endpoints
#define MICROPHONE_EP 0x81
#define HEADPHONE_EP 0x01   

//USB Audio Streaming Sample Rates 
#define SAMPLING_RATE_8000    0x1F40   //Used for testing of AEC
#define SAMPLING_RATE_11025   0x2B11
#define SAMPLING_RATE_12000   0x2EE0
#define SAMPLING_RATE_16000   0x3E80
#define SAMPLING_RATE_22050   0x5622
#define SAMPLING_RATE_24000   0x5DC0
#define SAMPLING_RATE_32000   0x7D00
#define SAMPLING_RATE_44100   0xAC44
#define SAMPLING_RATE_48000   0xBB80   //Used by the Smart Speaker 

//USB Descriptor definitions
#define USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID  0x00
#define APP_ID_INPUT_TERMINAL                  0x01
#define APP_ID_FEATURE_UNIT                    0x02
#define APP_ID_OUTPUT_TERMINAL                 0x03
#define APP_ID_INPUT_TERMINAL_MICROPHONE       0x04    
#define APP_ID_FEATURE_UNIT_MICROPHONE         0x05
#define APP_ID_OUTPUT_TERMINAL_MICROPHONE      0x06    
#define APP_ID_FEATURE_UNIT_SIDE_TONING        0x07    
#define APP_ID_MIXER_UNIT                      0x08    

#define USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1   0x01

#define APP_PLAYBACK_INTERFACE  0x01
#define APP_RECORDING_INTERFACE 0x02


//Set queue size the same for the Codec and USB modules
//#define APP_PLAYBACK_QUEUE_BUFFER_SIZE 32
//Initialize from the I2S RX Queue value (which should match the USB endpoint 0
//queue and USB audio queue values)
//#define APP_PLAYBACK_QUEUE_BUFFER_SIZE QUEUE_SIZE_RX_IDX0

#define AUDIO_FORMAT_WIDTH  16
//#define AUDIO_FORMAT_WIDTH  32  

//USB RX Queue Size should be same as APP_QUEUE_SIZE
#define APP_QUEUE_SIZE DRV_I2S_QUEUE_SIZE_IDX0                 

//QUEUE Initial Reads
//N, where N gives the fraction of the QUEUE to be filled, i.e. 1/N
//and USB reads generated and completed and Codec AddWrites before
//transitioning to APP_PROCESS_DATA timing. 
#define QUEUE_USB_INIT_PART  2  
#define APP_INIT_QUEUE_LEVEL  (APP_QUEUE_SIZE/QUEUE_USB_INIT_PART) 
//DRV_I2S_
#define CODEC_RECORD_SAMPLE_SIZE_BYTES   sizeof(DRV_I2S_DATA16) //stereo 16 bit
#define CODEC_PLAYBACK_SAMPLE_SIZE_BYTES sizeof(DRV_I2S_DATA16) //stereo 16 bit

//Max USB packet size is 1ms
//  1ms packet @ 48Khz = 48 samples/packet
//  DRV_I2S_DATA16 { .leftData .rightData } uint16_t each channel
//  --Q1.15 data/2channels
#define USB_FRAME_SIZE_MS   1000
#define USB_MAX_SAMPLE_RATE_HZ  48000
#define APP_MAX_NO_OF_SAMPLES_USB_FRAME USB_MAX_SAMPLE_RATE_HZ/USB_FRAME_SIZE_MS  //48  for 48Khz sampling  
#define APP_INT16_STEREO_SAMPLE_BYTES        sizeof(DRV_I2S_DATA16)    //4 bytes
#define APP_INT16_MONO_SAMPLE_BYTES          2
#define USB_RECORD_SAMPLE_SIZE_BYTES         APP_INT16_MONO_SAMPLE_BYTES    //2 bytes
#define USB_PLAYBACK_SAMPLE_SIZE_BYTES       APP_INT16_STEREO_SAMPLE_BYTES  //4 bytes 
#define APP_MAX_PLAYBACK_BUFFER_BYTES \
           (APP_MAX_NO_OF_SAMPLES_USB_FRAME*USB_PLAYBACK_SAMPLE_SIZE_BYTES)  //
#define APP_MAX_RECORD_BUFFER_BYTES   \
           (APP_MAX_NO_OF_SAMPLES_USB_FRAME*USB_RECORD_SAMPLE_SIZE_BYTES)    //

//Buttons
#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000
#define VOLUME_STEPS    4 




// *****************************************************************************

/* Application USBAudio Control

  Summary:
    Defines a list of audio controls supported by this application.

  Description:
    This enumeration defines audio controls supported by this application. This
 could be used for checking which Control Transfer is currently active.
 */
typedef enum {
    APP_USB_CONTROL_NONE = 0,
    APP_USB_AUDIO_MUTE_CONTROL, //Mute the codec
    APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_MP, //Change the Record frequency
    APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP, //Change the Playback frequency 
} APP_AUDIO_CONTROLS;

typedef enum {
    APP_USB_SPEAKER_PLAYBACK_NONE = 0,
    APP_USB_SPEAKER_PLAYBACK_STEREO = 1
} APP_SPEAKER_ALTERNATE_SETTINGS;

typedef enum {
    APP_USB_MIC_RECORD_NONE = 0,
    APP_USB_MIC_RECORD_MONO = 1
} APP_MIC_ALTERNATE_SETTINGS;

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
    APP_STATE_INIT = 0,
    APP_STATE_USB_DEVICE_OPEN,
    //APP_STATE_CODEC_OPEN,
    //APP_STATE_CODEC_SET_BUFFER_HANDLER,
    APP_STATE_INITIAL_USB_READ_REQUESTS,
    APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS,
    APP_STATE_MUTE_AUDIO_PLAYBACK,
    APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD,
    APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP,
    APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP,
    //APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD_HP, //headphone
    //APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD_MP, //microphone
    APP_STATE_WAIT_FOR_CONFIGURATION,
    APP_STATE_PROCESS_DATA,
    APP_STATE_IDLE,
    APP_STATE_ERROR,

#if 0
        //SD Card states
        APP_STATE_MOUNT_DISK, /* The app mounts the disk */
        APP_STATE_UNMOUNT_DISK, /* The app unmounts the disk */
        APP_STATE_MOUNT_DISK_AGAIN, /* The app mounts the disk again */
        APP_STATE_SET_CURRENT_DRIVE, /* Set the current drive */
        APP_STATE_OPEN_FIRST_FILE, /* The app opens the file to read */
        APP_STATE_OPEN_SECOND_FILE, /* The app opens the file to write */
        APP_STATE_CREATE_DIRECTORY, /* Create directory */
        APP_STATE_READ_WRITE_TO_FILE, /* The app reads from a file and writes to another file */
        APP_STATE_CLOSE_FILE, /* The app closes the file*/
#endif


} APP_STATES;

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

#ifdef PIC32MZEFC2
// SWITCH<1,2,3>_Get()   E70CULT: SWITCH =   MZC2: SWITCH1
// SWITCH<1,2,3>_STATE_PRESSED 
// SWITCH<1,2,3>_STATE_RELEASED
#define SWITCH_Get SWITCH1_Get 
#define SWITCH_STATE_PRESSED SWITCH1_STATE_PRESSED  //Make same as SAME70CULT
#endif //PIC32MZEFC2

//LED State
typedef enum
{
    LED_OFF = 0,     //LED OFF - No Blink
    LED_ON,          //LED ON - No Blink
    LED_MUTE_BLINK,       //Fast 1 sec blink
    LED_CONNECTED_BLINK,  //Slow 2 sec blink
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
    APP_STATES   volatile  state;
    APP_STATES   volatile  lastState; //Debug info

    /* device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE usbDevHandle;

    /* Instance number of Audio Function driver */
    USB_DEVICE_AUDIO_INDEX audioInstance;

    /* device configured state */
    bool isConfigured;

    bool codecConfigured;

    uint32_t               usbReadBufSize;
    uint32_t               usbWriteBufSize;

    /* Holds active value of USB Audio Streaming Interface Alternate setting.
     * Every USB Audio Streaming should have 1 or more Alternate settings.
     * Alternate setting Zero (or Default Alternate setting) never has any
     * data payload. An audio streaming can have multiple number of alternate
     settings each having it own sampling rate, bandwidth etc. */
    APP_SPEAKER_ALTERNATE_SETTINGS activeInterfaceAlternateSetting;
    
    APP_MIC_ALTERNATE_SETTINGS activeMicInterfaceAlternateSetting;

    /* Holds current value of Audio Control Mute settings. A value True indicates
     * audio is currently muted. */
    bool dacMute;

    bool          volatile playbackActive;
    bool          volatile recordActive;

    uint32_t      volatile sampleFreq;

    uint32_t      volatile sampleFreqMic;

    /* This is to indicate which Audio Control Transfer is currently
     * in progress. */
    APP_AUDIO_CONTROLS currentAudioControl;

    /* CODEC client handle */
    APP_CODEC_CLIENT codecClientWriteRead;

    //Codec Playback Volume
    int           volatile volume;
    int           volatile volumeIndex;
    bool          volatile muteEn;

    //Codec Mic Gain
    uint8_t       volatile micGain;
    int           volatile micGainIndex;

    //Button Handler
    DRV_HANDLE    tmrHandle;
    bool          volMuteMode;
    uint32_t      volatile buttonDelay;
    BUTTON_STATES volatile buttonState;

    LED_STATE     volatile ledState;
    int           volatile blinkDelay;

    bool          volatile lrSync;

} APP_DATA;

// Application Codec Playback Buffer Queue
// NOTE(S):  
//     1) For MZ must be aligned/coherant  to 16 byte pages for DMA cache coherency 
//        For E70 aligned to 32 byte pages (use this) 
//     2) The USB record buffer is mono, while the codec record buffer is stereo
//        --echo is cancelled to the mono buffer.

typedef struct {
    uint8_t CACHE_ALIGN
            playbackbuffer[APP_MAX_PLAYBACK_BUFFER_BYTES]; //
    uint8_t CACHE_ALIGN
            recordbuffer[APP_MAX_PLAYBACK_BUFFER_BYTES]; // For Stereo Codec
    uint8_t CACHE_ALIGN
            aecbuffer[APP_MAX_RECORD_BUFFER_BYTES]; //
    DRV_CODEC_BUFFER_HANDLE codecWRHandle; //4 bytes (ptr)
    USB_DEVICE_AUDIO_TRANSFER_HANDLE usbReadHandle; //4 bytes (ptr)
    USB_DEVICE_AUDIO_TRANSFER_HANDLE usbWriteHandle; //4 bytes (ptr)
    int32_t bufferIdx;  //4 byte
    bool codecInUse; //1 byte
    bool usbInUse; //1 byte
    bool codecWRComplete; //1 byte
    bool codecAecComplete; //1 byte
    bool usbReadComplete; //1 byte
    bool usbWriteComplete; //1 byte
    char padding[22]; // bytes  (32 - 22+10 = 0 )
} APP_RW_BUFFER;

// NOTE:  
//    APP_QUEUE_SIZE is the USB RX Queue size from system_config.h 
//    which should be the same as the USB TX Queue size
//
typedef struct {
    APP_RW_BUFFER CACHE_ALIGN
            buffer[APP_QUEUE_SIZE];
    int16_t  tailIdx;                 //USB read/Write Idx (HEAD)
    int16_t  lastCWRCIdx;             //Last Codec WR Complete Idx
    int16_t  headIdx;                 //Codec write/read Idx (TAIL)
    int16_t  lastURCIdx;             //Last USB Complete Idx
    int16_t  lastUWCIdx;             //Last USB Complete Idx
    int16_t  aecIdx;                  //Codec AEC (preTail)
    uint32_t usbCompleteBufferLevel;  //#Read and Write completed
    uint32_t usbReadCompleteCnt;      //#of read buffers ready to write
    uint32_t usbWriteCompleteCnt;     //#of write buffers ready to read 
    uint32_t usbReadQueueCnt;         //#of usb Reads queued
    uint32_t usbWriteQueueCnt;        //#of usb Reads queued
    uint32_t codecWRQueueCnt;      //#of codec writeReads queued
    uint32_t codecWRCompleteCnt;   //#of codec writeReads completed
    bool     usbInitialReadsCompleteFlag;
    uint8_t  padding[7];            // (2*6 + 7*4 + 1) % 32 = 41 - 32 
}APP_BUFFER_QUEUE;

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
    void APP_CODECBufferEventHandler<Write,Read>(DRV_CODEC_BUFFER_EVENT event,
                                                 DRV_CODEC_BUFFER_HANDLE handle, 
 *                                               uintptr_t context )

  Summary:
    Event Handler for codec client Task.

  Description:
    This is the Event Handler for Codec Tx and Rx buffer complete events.

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
    These routines must be called from within the SYS_Tasks() routine,i.e.
    APP_Tasks().
*/
void APP_CODECBufferEventHandlerWriteRead(DRV_CODEC_BUFFER_EVENT event, 
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


