/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c for usb_headset application

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

    The USB playback interface is a multifrequency stream with 16 bit stereo 
    data.  The USB record interface should be at the same frequency as playback
    (as set from the HOST) and is mono 16 bit.


 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END


//*****************************************************************************
//*****************************************************************************
// Section: Included Files
//*****************************************************************************
//*****************************************************************************

#include "app.h"
#include "app_globals.h"

#define q15 int16_t 

//DEBUG
#undef  DEBUG_AUDIO_PATH
#undef  DEBUG_TONE_CODEC  //Sin Tone Output
#undef  DEBUG_MIC_LOOPBACK
#define DEBUG_SAMPLECOUNT     14000
uint32_t Debug_var;
//DEBUG variables

int printCnt1 = 0;
int printCnt2 = 0;


uint32_t interruptStatus;

#if 0
typedef enum _ERROR_NUM 
    {ERR_GENERIC, ERR_TIMER, ERR_CODEC, ERR_CODEC_EVENT,
     ERR_CODEC_BUFFER,ERR_USB, ERR_USB_EVENT, ERR_USB_BUFFER
    } ERROR_NUM;
char * ERROR_STR[] = 
    { 
      "Generic Error",     //0
      "Timer Not Ready",   //1 
      "Codec Not Ready",   //2 
      "Codec Event Error", //3 
      "Codec Buffer Error",//4 
      "USB Not Ready",     //5,
      "USB Event Error",   //6
      "USB Buffer Error",  //7
    };
#endif //0


//Playback Buffer QUEUE Initialization 
//N, where N gives the fraction of the QUEUE to be filled, i.e. 1/N
//and USB reads generated and completed and Codec AddWrites before
//transitioning to APP_STATE_PROCESS_DATA timing. 
#define QUEUE_USB_INIT_PART   2  

bool queueFull;
bool queueEmpty;

uint32_t someVar;  //Codec context variable
static int skip = 1;


//******************************************************************************
//NOTE: Cache coherency and 16 byte alignment required for MZ processor,
//      -->as the cache page size on PIC32MZ is 16 bytes.
//      You don?t want to run into an issue where linker allocates the data 
//      structure in the same cache page (line) as another data structure and 
//      then a line flush causes coherency issues.

//MICBUFFERIDX micBufferIdx = MICBUFFER_NONE;
//DRV_I2S_DATA16 APP_MAKE_BUFFER_DMA_READY 
//    txBuffer[2][APP_MAX_NO_OF_SAMPLES_IN_A_USB_FRAME];

//Acoustic Echo Canceller 
//int16_t speakerBufferNum; //Playback Buffer associated with current mic input buffer

//uint32_t timerStart, timerEnd;
//uint32_t cycles = 0;


USB_DEVICE_AUDIO_TRANSFER_HANDLE writeTransferHandle =
                                     USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;

//NOTE: APP_PLAYBACK BUFFER is what is transferred using DMA--> it is padded
// and placed at the beginning of the struct and the struct attribute is the
// allocated with the COHERENT and aligned(16) attributes so that it is 
// placed at the correct page boundary.
static APP_BUFFER_QUEUE CACHE_ALIGN appWRQueue;

//USB Audio Processing Buffers
APP_RW_BUFFER     * currentQueue;
DRV_I2S_DATA16    * currentPlayback;
DRV_I2S_DATA16    * currentRecord;
int16_t           * currentAecBuffer; //MicIn AEC Processed Audio buffer.

//CODEC Audio Processing Buffers
#ifdef DEBUG_MIC_LOOPBACK
APP_RW_BUFFER     * currentQueueH;
DRV_I2S_DATA16    * currentPlaybackH;
DRV_I2S_DATA16    * currentRecordH;
#endif

//AEC Processing Record Output Buffers
//int16_t           * xOutQ1d15[APP_MAX_RECORD_BUFFER_BYTES];  //Speaker Output Processed Buffer
int16_t           yInQ1d15[APP_MAX_RECORD_BUFFER_BYTES];   //MicInput Processed buffer 

//Button Timer
DRV_HANDLE tmrHandle;

#ifdef DEBUG_TONE_CODEC
static DRV_I2S_DATA16 CACHE_ALIGN 
       sinBuffer[APP_MAX_NO_OF_SAMPLES_USB_FRAME] =
{
#include    "sin_1Khz_p5_48Ksps.dat"
};
#endif

//Display, Button, and Timer
void _APP_Button_Tasks(void);
void _APP_LED_Tasks(void);
static void _APP_TimerCallback( uintptr_t context);

//==============================================================================
// Application Playback Buffer Queue
//==============================================================================
static int     _APP_SetUSBReadBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle);
static int     _APP_SetUSBWriteBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle);
//static void    _APP_SetAecBufferReady(int idx);
static int     _APP_SetCodecWriteReadComplete(DRV_CODEC_BUFFER_HANDLE handle);
static void    _APP_Init_RWBufferQueue();
//static void    _APP_ClearCodecReturnBuffer(DRV_CODEC_BUFFER_HANDLE handle);
static int16_t _APP_GetNextIdx(int16_t index);
static bool    _APP_USBReadAllBufferReady();
static void    _aecSumStereoChannels(DRV_I2S_DATA16 * input, 
                                     q15 * output, 
                                     int numSamples);
static bool hpInterfaceChanged = false;
static bool mpInterfaceChanged = false;
static USB_DEVICE_AUDIO_RESULT audioErrRead;
static USB_DEVICE_AUDIO_RESULT audioErrWrite;
//static uint32_t usbWriteError=0;

uint16_t volumeLevels[VOLUME_STEPS] =
{
    0 /* off */, 64, 128, 255
};

//NOTE:  Mute/Max gain levels
//#define MIC_GAIN_STEPS 4
//const uint8_t micGainLevels[MIC_GAIN_STEPS] =
//{
//    0 /* off */, 
//    64,
//    128,
//    255
//};

//Application Class Data
APP_DATA appData =
{
    //Device Layer Handle  
    .usbDevHandle = -1,

    //USB Audio Instance index for this app object 0
    .audioInstance = 0,

     //app state
    .state = APP_STATE_INIT,

    //Device configured state */
    .isConfigured = false,

    //Initialize active interface setting to 0.
    .activeInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE,
    
    .activeMicInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE,

    // DAC is not muted initially 
    .dacMute = false,        //USB Mute command
    .ledState = LED_OFF,
    .blinkDelay = 0,
    .muteEn = false,
    
    //<STUB> - change to 48000 Hz after downsampling to AEC
    //NOTE:  The record and playback sample rates must be the same to
    //       match the codec sampling rate.
    //USB Sampling Frequency
    .sampleFreq    = DRV_CODEC_AUDIO_SAMPLING_RATE,
    .sampleFreqMic = DRV_CODEC_AUDIO_SAMPLING_RATE,

    .usbReadBufSize  = APP_MAX_PLAYBACK_BUFFER_BYTES,  //1ms Playback 
    .usbWriteBufSize = APP_MAX_RECORD_BUFFER_BYTES,    //1ms Record

    .codecClientWriteRead.context = (uintptr_t)&someVar,
    .codecClientWriteRead.bufferHandler = 
        (DRV_CODEC_BUFFER_EVENT_HANDLER) APP_CODECBufferEventHandlerWriteRead,
    .codecClientWriteRead.bufferSize = CODEC_MAX_WR_BUFFER_SIZE_BYTES,   //1ms @ 8000Hz 16bit-Stereo
    
    // No Audio control in progress.
    .currentAudioControl = APP_USB_CONTROL_NONE,

    .playbackActive = false,
    .recordActive   = false,

    //Initialize Mic Settings
    .micGainIndex = 3,
    .micGain      = 128
};


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************


//******************************************************************************
// APP_USBDeviceEventHandler()
//******************************************************************************
void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, 
                               void * pEventData, 
                               uintptr_t context )
{
    volatile USB_DEVICE_EVENT_DATA_CONFIGURED* configuredEventData;


    //SYS_PRINT("%s\r\n",usbDevEventStr[event]);

    switch( event )
    {
        case USB_DEVICE_EVENT_RESET:
            break;

        case USB_DEVICE_EVENT_DECONFIGURED:
            // USB device is reset or device is de-configured.
            // This means that USB device layer is about to de-initialize
            // all function drivers. So close handles to previously opened
            // function drivers.

            // Also turn ON LEDs to indicate reset/de-configured state.
            /* Switch on red and orange, switch off green */
            appData.ledState = LED_OFF; 
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            /* check the configuration */
            configuredEventData = 
                     (USB_DEVICE_EVENT_DATA_CONFIGURED *)pEventData;
            if(configuredEventData->configurationValue == 1)
            {
                /* the device is in configured state */
                /* Switch on green and switch off red and orange */
                appData.ledState = LED_CONNECTED_BLINK; 
                USB_DEVICE_AUDIO_EventHandlerSet(
                        0,
                        APP_USBDeviceAudioEventHandler ,
                        (uintptr_t)NULL);
                /* mark that set configuration is complete */
                appData.isConfigured = true;
                SYS_MESSAGE("APR: USB Device Configured\r\n");
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            /* Switch on green and orange, switch off red */

            SYS_MESSAGE("APS: USB Device Suspended\r\n");
            appData.ledState = LED_MUTE_BLINK; 
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            appData.ledState = LED_CONNECTED_BLINK; 
            SYS_MESSAGE("APT: USB Device Attached\r\n");
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS was removed. Notify USB stack about the event*/
            SYS_MESSAGE("APU: USB Device DEtached\r\n");
            USB_DEVICE_Detach (appData.usbDevHandle);
            appData.ledState = LED_OFF; 
            break;

        case USB_DEVICE_EVENT_ERROR:
            SYS_MESSAGE("APV: USB Device ERROR\r\n");
            //Code ERROR
            errorNum = ERR_USB; 
            appData.ledState = LED_ERROR_BLINK; 
            appData.state = APP_STATE_ERROR;
            break;

        default:
            break;
    }
} //End APP_USBDeviceEventHandler()


//******************************************************************************
// APP_USBDeviceAudioEventHandler()
//******************************************************************************
void APP_USBDeviceAudioEventHandler(USB_DEVICE_AUDIO_INDEX iAudio,
                                    USB_DEVICE_AUDIO_EVENT event ,
                                    void * pData,
                                    uintptr_t context)
{
    volatile USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED *interfaceInfo;
    volatile USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *readEventData;
    volatile USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE *writeEventData;
    uint8_t entityID;
    uint8_t controlSelector;

    if ( iAudio == 0 )
    {

        //SYS_PRINT("%s\r\n",usbAudioEventStr[event]);

        switch (event)
        {
            case USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED:
                /* We have received a request from USB host to change the Interface-
                   Alternate setting.*/
                interfaceInfo = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED *)
                   pData;
                if(interfaceInfo->interfaceNumber == APP_PLAYBACK_INTERFACE)
                {
                    if (appData.activeInterfaceAlternateSetting != 
                        interfaceInfo->interfaceAlternateSetting)
                    {

                        hpInterfaceChanged = true;
                        mpInterfaceChanged = false;
                        appData.state = APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                        appData.activeInterfaceAlternateSetting = 
                                       interfaceInfo->interfaceAlternateSetting;
                    }
                    
                }
                if(interfaceInfo->interfaceNumber == APP_RECORDING_INTERFACE)
                {
                    if (appData.activeMicInterfaceAlternateSetting 
                        != interfaceInfo->interfaceAlternateSetting)
                    {

                        hpInterfaceChanged = false;
                        mpInterfaceChanged = true;
                        appData.state = APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                        appData.activeMicInterfaceAlternateSetting = 
                                       interfaceInfo->interfaceAlternateSetting;
                    }
                    
                }

            break;

            case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
            {
                //We have received an audio frame from the Host.
                //Now send this audio frame to Audio Codec for Playback.
                readEventData = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *)pData;

                //Make USB Read Buffer Queue Ready for Data (Clear data flags)
                if (_APP_SetUSBReadBufferReady(readEventData->handle))
                {
                    appWRQueue.usbReadCompleteCnt++;
                }
                else
                {
                    SYS_MESSAGE("[EUB-R]");
                    errorNum = ERR_USB_BUFFER;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                queueEmpty = false;

                //Check if the this is the initial loading of the playback
                //queue with USB Rx Data 
                if (appData.state == 
                       APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS)
                {
                    if(_APP_USBReadAllBufferReady())
                    {
                        SYS_MESSAGE("[UQRR]");
                        appWRQueue.usbInitialReadsCompleteFlag = true; //ALL Ready for Codec Playback
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
            {
                writeEventData = (USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE *)pData;
                if (_APP_SetUSBWriteBufferReady(writeEventData->handle))
                {
                    appWRQueue.usbWriteCompleteCnt++;
                }
                else
                {
                    SYS_MESSAGE("[EUB-W]");
                    errorNum = ERR_USB_BUFFER;
                    appData.state = APP_STATE_ERROR;
                    break;
                }

#if 1 //Not in usb_smart_speaker
                //Check if the this is the initial loading of the playback
                //queue with USB Rx Data 
                if (appData.state == 
                       APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS)
                {
                    if (_APP_USBReadAllBufferReady())
                    {
                        //SYS_MESSAGE("_USB: READ COMPLETE QUEUE READY(W)\r\n");
                        SYS_MESSAGE("[UQWR]");
                        appWRQueue.usbInitialReadsCompleteFlag = true; //ALL Ready for Codec Playback
                    }
                }
#endif // 
            }
            break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR:
            {
                if(((USB_SETUP_PACKET*)pData)->Recipient == 
                     USB_SETUP_REQUEST_RECIPIENT_INTERFACE)
                {
                    entityID = 
                       ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)pData)->entityID;
                    if ((entityID == APP_ID_FEATURE_UNIT) || 
                        (entityID == APP_ID_FEATURE_UNIT_MICROPHONE) || 
                        (entityID == APP_ID_FEATURE_UNIT_SIDE_TONING)) 
                    {
                       controlSelector = 
                          ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                              pData)->controlSelector;
                       if (controlSelector == USB_AUDIO_MUTE_CONTROL)
                       {
                           //A control write transfer received from Host. 
                           //Now receive data from Host.
                           USB_DEVICE_ControlReceive(appData.usbDevHandle, 
                                                    (void *) &(appData.dacMute), 
                                                    1);
                           appData.currentAudioControl = 
                                   APP_USB_AUDIO_MUTE_CONTROL;
                       }
                       
                    }
                    else if (entityID == APP_ID_MIXER_UNIT)
                    {
                        USB_DEVICE_ControlStatus(
                                appData.usbDevHandle, 
                                USB_DEVICE_CONTROL_STATUS_ERROR);
                    }
                }
                else if (((USB_SETUP_PACKET*)pData)->Recipient == 
                           USB_SETUP_REQUEST_RECIPIENT_ENDPOINT)
                {
                    controlSelector = 
                       ((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                          pData)->controlSelector;

                    if (controlSelector == USB_AUDIO_SAMPLING_FREQ_CONTROL)
                    {
                        if (((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                                pData)->endpointNumber == MICROPHONE_EP)
                        {
                           //A control write transfer received from Host. Now receive data from Host.
                           USB_DEVICE_ControlReceive(appData.usbDevHandle, 
                                                     (void *) &(appData.sampleFreqMic), 
                                                     3);
                           //SYS_MESSAGE("_USB: Audio Sampling Freq Control Rcvd: MP\r\n");
                           appData.currentAudioControl = 
                                   APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_MP;
                        }
                        
                        else if (((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                                pData)->endpointNumber == HEADPHONE_EP)
                        {
                           //A control write transfer received from Host. Now receive data from Host.
                           //SYS_MESSAGE("_USB: Audio Sampling Freq Control Rcvd: HP\r\n");
                           USB_DEVICE_ControlReceive(appData.usbDevHandle, 
                                                     (void *) &(appData.sampleFreq), 
                                                     3);
                           appData.currentAudioControl = 
                                   APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP;
                        }
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_CUR:
            {
                if (((USB_SETUP_PACKET*)pData)->Recipient == 
                     USB_SETUP_REQUEST_RECIPIENT_INTERFACE)
                {
                    entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)
                                  pData)->entityID;
                    if ((entityID == APP_ID_FEATURE_UNIT) || 
                         (entityID == APP_ID_FEATURE_UNIT_MICROPHONE) || 
                         (entityID == APP_ID_FEATURE_UNIT_SIDE_TONING))
                    {
                       controlSelector = 
                              ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                                            pData)->controlSelector;
                       if (controlSelector == USB_AUDIO_MUTE_CONTROL)
                       {
                           /*Handle Get request*/
                           USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                                 (void *)&(appData.dacMute), 
                                                 1);
                           //SYS_PRINT("_USB: ControlGetCUR FU/FUM/FUST Mute\r\n");
                       }
                    }
                    
                    else if (entityID == APP_ID_MIXER_UNIT)
                    {
                        USB_DEVICE_ControlStatus(
                                appData.usbDevHandle, 
                                USB_DEVICE_CONTROL_STATUS_ERROR);
                        //SYS_PRINT("_USB: ControlGetCUR MU ERROR\r\n");
                    }
                }
                else if (((USB_SETUP_PACKET*)pData)->Recipient == 
                           USB_SETUP_REQUEST_RECIPIENT_ENDPOINT)
                {
                    controlSelector = ((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                                         pData)->controlSelector;
                    if (controlSelector == USB_AUDIO_SAMPLING_FREQ_CONTROL)
                    {
                        if (((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                                pData)->endpointNumber == MICROPHONE_EP)
                        {
                           //A control write transfer received from Host. 
                           //Now receive data from Host.
                           USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                                  (void *)&(appData.sampleFreqMic), 
                                                  3);
                           //SYS_PRINT("_USB: ControlGetCUR MP Fs=%d \r\n",
                           //        appData.sampleFreqMic);
                        }
                        
                        else if (((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                                       pData)->endpointNumber == HEADPHONE_EP)
                        {
                           //A control write transfer received from Host. Now receive data from Host.
                           USB_DEVICE_ControlSend(
                                   appData.usbDevHandle, 
                                   (void *)&(appData.sampleFreq), 3 );
                           //SYS_PRINT("_USB: ControlGetCUR HP Fs=%d \r\n",
                           //        appData.sampleFreq);
                        }
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MIN:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MIN:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MAX:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MAX:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_RES:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_RES:
            case USB_DEVICE_AUDIO_EVENT_ENTITY_GET_MEM:
                /* Stall request */
                USB_DEVICE_ControlStatus(
                        appData.usbDevHandle, 
                        USB_DEVICE_CONTROL_STATUS_ERROR);
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            {
                USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK );

                if (appData.currentAudioControl == APP_USB_AUDIO_MUTE_CONTROL)
                {
                    //SYS_PRINT("_USB: ControlTransRCVD - MUTE \r\n");
                    appData.state = APP_STATE_MUTE_AUDIO_PLAYBACK;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                    //Handle Mute Control Here.
                }
                
                if (appData.currentAudioControl == 
                    APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP)
                {
                    //DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle, 
                    //                          appData.sampleFreq);
                    if (appData.sampleFreq == SAMPLING_RATE_48000)
                    {
                        appData.codecClientWriteRead.bufferSize = 192;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_32000)
                    {
                        appData.codecClientWriteRead.bufferSize = 128;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_24000)
                    {
                        appData.codecClientWriteRead.bufferSize = 96;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_16000)
                    {
                        appData.codecClientWriteRead.bufferSize = 64;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_8000)
                    {
                        appData.codecClientWriteRead.bufferSize = 32;
                    }
                    
                    //SYS_PRINT("_USB: HP Set Fs=%d (buffer=%d)\r\n",
                    //        appData.sampleFreq,
                    //        appData.codecClientWriteRead.bufferSize);
                    //NOTE:  Change Sampling Frequency then Reinit Playback Queue
                    //appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    //appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                    hpInterfaceChanged = 1;
                    appData.state = APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP; //RTOS Version
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                }
                else if (appData.currentAudioControl == 
                         APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_MP)
                {
                    //This not in RTOS version
                    //DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle, 
                    //                          appData.sampleFreqMic);

                    //NOTE:  buffersize is in stereo samples (/2) in RTOS version
                    if (appData.sampleFreqMic == SAMPLING_RATE_48000)
                    {
                        appData.codecClientWriteRead.bufferSize = 192; //int16 values
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_32000)
                    {
                        appData.codecClientWriteRead.bufferSize = 128;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_24000)
                    {
                        appData.codecClientWriteRead.bufferSize = 96;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_16000)
                    {
                         appData.codecClientWriteRead.bufferSize = 64;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_8000)
                    {
                         appData.codecClientWriteRead.bufferSize = 32;
                    }
                    //SYS_PRINT("APP: MP Set Fs=%d (buffer=%d)\r\n",
                    //        appData.sampleFreqMic,
                    //        appData.codecClientWriteRead.bufferSize);
                    
                    //NOTE:  The value received here is not used.  
                    //       Playback sets the sampling 
                    //       frequency.  The Mic frequency if different will
                    //       cause problems.
                    
                    //NOTE:  Change Sampling Frequency then Reinit Playback Queue
                    //appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    //appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                    mpInterfaceChanged = 1;
                    appData.state = APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                }
            }
            break;

            case  USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

            default:
                SYS_ASSERT ( false , "Invalid callback" );
                SYS_PRINT("USB Audio: Invalid Callback\r\n");
            break;

        } //End switch (event)
    }//end of if  if ( iAudio == 0 )

}//End APP_AudioEventCallback()


//******************************************************************************
// APP_Initialize() - USB Playback and Record
//
// Application Initialize. It is called from the SYS_Initialized() function.
//******************************************************************************
void APP_Initialize()
{
    {
        char *initMessage = 
            "\r\nApplication created " __DATE__ " " __TIME__"!\r\n";
        //SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, initMessage, 
        //        strlen(initMessage));
        SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, initMessage, 
                strlen(initMessage));
    }
    SYS_MESSAGE("- Starting: usb_headset \r\n");
    SYS_PRINT("MIC%d",DRV_CODEC_WHICH_MIC_INPUT+1);

    appData.playbackActive = false;
    appData.recordActive   = false;

#if defined(DEBUG_CLOCK_TUNING) || defined(DEBUG_BUFFER_TIMING)
#ifdef __CORE_CM7_H_GENERIC
    TEST1_Clear();  //USB RQ   or queueEmpty
    TEST2_Clear();  //USB RC   or queueFull
    TEST3_Clear();  //Codec RQ or CLK+ 
    TEST4_Clear();  //Codec RC or Queue Leve = Target
    //TEST5_Clear();  // Queue Level > target
#endif //__CORE_CM7_H_GENERIC
#endif   
} //End APP_Initialize()


//******************************************************************************
// APP_Tasks()
//
// Application tasks routine. This function implements the
// application state machine.
//******************************************************************************
void APP_Tasks()
{
    int i;

    _APP_Button_Tasks();
    _APP_LED_Tasks();

#define NUMSTATEHIST 10
    static int __attribute__((unused)) stateHist[NUMSTATEHIST];
    static int __attribute__((unused)) stateHistIdx = 0;

    if (appData.lastState!=appData.state)
    {
        //New state
        //char strSrc[6] = "";
        stateHist[stateHistIdx++] = (int) appData.state;
        if (stateHistIdx >= NUMSTATEHIST) stateHistIdx=0;
        //SYS_CONSOLE_Message(0,stateStr[appData.state]);
        //itoa((int)appData.state,strSrc,10);
        //strcat(strSrc,";");        
        SYS_PRINT("%d;",appData.state);
    }
    appData.lastState = appData.state;

    switch(appData.state)
    {
        //======================================================================
        // Application's initial state. 
        //======================================================================
        case APP_STATE_INIT:
        {
            //SYS_MESSAGE("APP: APP_STATE_INIT\r\n");
            tmrHandle = SYS_TIME_CallbackRegisterMS(_APP_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
                SYS_MESSAGE("Timer Configured\r\n");
               appData.state = APP_STATE_CODEC_OPEN;
            } 
            else
            {
                SYS_MESSAGE("ERROR - --Timer Handle Invalid");
                errorNum = ERR_TIMER;
                appData.state = APP_STATE_ERROR;
                break;
            }
        }
        break;

        //======================================================================
        // Configure CODEC
        //
        // Transition from: APP_STATE_WAIT_FOR_CONFIGURATION
        //
        // Wait until: SYS_STATUS_READY 
        //
        // Transition To: 
        //
        //======================================================================
        case APP_STATE_CODEC_OPEN:
        {
            SYS_STATUS codecStatus;
            codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);

            if (SYS_STATUS_READY == codecStatus)
            {

                // A client opens the driver object to get an Handle 
                appData.codecClientWriteRead.handle = 
                    DRV_CODEC_Open(DRV_CODEC_INDEX_0,
                                   DRV_IO_INTENT_READWRITE | 
                                   DRV_IO_INTENT_EXCLUSIVE);
                       
                if (appData.codecClientWriteRead.handle != DRV_HANDLE_INVALID) 
                {
                    SYS_MESSAGE("CODEC Ready\r\n");
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                }
                else
                {
                    SYS_DEBUG(0, "ERROR - Can't open CODEC\r\n");
                    errorNum = ERR_CODEC;
                    appData.state = APP_STATE_ERROR;
                }
            }
            else
            {
                //Wait for CODEC to Initialize
               appData.state = APP_STATE_CODEC_OPEN;
            }
        }
        break;
        
        //======================================================================
        // Set a handler for the audio buffer completion event 
        // --Then APP_STATE_USB_DEVICE_OPEN
        //======================================================================
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {
            DRV_CODEC_BufferEventHandlerSet(
                        appData.codecClientWriteRead.handle,
                        appData.codecClientWriteRead.bufferHandler,
                        appData.codecClientWriteRead.context);

//Enable SPI Data In
#if !defined(__CORE_CM7_H_GENERIC)
            //PLIB_SPI_PinEnable(SPI_ID_1, SPI_PIN_DATA_IN);  
#endif    
            
            if (DRV_CODEC_I2S_MASTER_MODE == true)
            {
                SYS_PRINT("--Codec I2S is Master\r\n");
            }
            else
            {
                SYS_PRINT("--Codec I2S is Slave Fs = %d Hz\r\n",
                        appData.sampleFreq);
            }

            appData.codecConfigured = true;
            
            appData.state = APP_STATE_USB_DEVICE_OPEN;
        }
        break;

 
        //---------------------------------------------------------------------
        // Wait for USB Connection to be configured
        //---------------------------------------------------------------------
        case APP_STATE_USB_DEVICE_OPEN:        
        {
            /* Open the device layer */
            //Open USB for Read/Write
            appData.usbDevHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0,
                                                   DRV_IO_INTENT_READWRITE);

            if(appData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get 
                 * event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.usbDevHandle, 
                                           APP_USBDeviceEventHandler, 0);
                SYS_MESSAGE("USB Device Layer Open\r\n");

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                //The Device Layer is not ready to be opened. 
                //--We should try again later.
                SYS_MESSAGE("WAIT - USB Device Layer Not Ready\r\n");
            }
        }
        //Fall Through

        //======================================================================
        // Wait for USB Connection to be configured
        // --then APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE
        //======================================================================
        case APP_STATE_WAIT_FOR_CONFIGURATION:
        {
            //Check if Host has configured the Device.
            if (appData.isConfigured == true)
            {
                SYS_MESSAGE("USB Device Configured\r\n");
                appData.state = APP_STATE_IDLE;
                //appData.state = APP_STATE_CODEC_OPEN;
            }
        }
        break;


        //======================================================================
        // Initial USB Read Request
        //
        // Transition From:
        //         APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        //              --16Khz playback alternate setting
        //       
        //       
        //       USB Audio Control: APP_USB_AUDIO_MUTE_CONTROL
        //       to state  APP_STATE_MUTE_AUDIO_PLAYBACK 
        //
        // Wait for 16Khz alt playback 
        // -->Initiate USB Rx to all playback queue buffers
        //
        // Transition To: 
        //     APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS
        //======================================================================
        case APP_STATE_INITIAL_USB_READ_REQUESTS:
        {
            //if (appData.activeInterfaceAlternateSetting == 
            //         APP_USB_SPEAKER_PLAYBACK_STEREO)
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO)  ||
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_MONO))
            {
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                TEST1_Set();    //USB RW   :  queueEmpty
                TEST2_Clear();  //USB WRC  : !queueFull
                TEST3_Clear();  //Codec WR  :
                TEST4_Clear();  //Codec WRC :
                //TEST5_Clear();  //! QL > Target
#endif //__CORE_CM7_H_GENERIC
#endif
                printCnt1 = 16;
                printCnt2 = 16;

                appData.ledState = LED_ON; //Streaming

                //Fill the queue with USB Read Data
                appWRQueue.usbInitialReadsCompleteFlag = false;  //Actually USB Init Q Read Complete
                static bool errState = false;
                errState = false;
                for(i = 0;i < APP_QUEUE_SIZE;i++)
                {
                    //USB Read to Head of Codec Playback Buffer
                    currentQueue     = &(appWRQueue.buffer[i]);
                    currentPlayback  = (DRV_I2S_DATA16 *)
                                      (currentQueue->playbackbuffer);
                    //currentRecord only used by codec
                    currentAecBuffer = (int16_t *) (currentQueue->aecbuffer);

                    if ((currentQueue != NULL) && 
                        (currentQueue->codecInUse == false) && 
                        (currentQueue->usbInUse   == false)) 
                    {


                        //Read Stereo Playback buffer
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                        TEST1_Toggle();
#endif //__CORE_CM7_H_GENERIC
#endif
                        //Initial Playback
                        if (appData.activeInterfaceAlternateSetting == 
                                 APP_USB_SPEAKER_PLAYBACK_STEREO)
                        {
                            if (printCnt1-- > 0) SYS_MESSAGE("{r}");
                            audioErrRead = USB_DEVICE_AUDIO_Read(
                                    USB_DEVICE_INDEX_0, 
                                    &currentQueue->usbReadHandle, 
                                    1, //Interface # 
                                    currentPlayback,
                                    appData.usbReadBufSize); 

                            if (audioErrRead == USB_DEVICE_AUDIO_RESULT_OK)
                            {
                                currentQueue->usbInUse        = true;
                                currentQueue->usbReadComplete = false;
                                appWRQueue.usbReadQueueCnt++;
                            }
                            else
                            {
                                //USB Write Failed
                                //Write Mono Record Buffer - AEC
                                if (audioErrRead==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                                {
                                    SYS_MESSAGE("{QROv}");
                                    //char strDst[32] = "QWOV h=";
                                    //DEBUG_PRINT1(strDst, appWRQueue.headIdx);
                                }
                                else SYS_MESSAGE("{ERe}");
                                errState = true;
                                currentQueue->usbInUse = false;
                                break;
                            }
                        } 
                        else
                        {
                            //Allow Record Only
                            currentQueue->usbReadComplete = true;
                        }

                        
                        //Initial Record
                        //NOTE:  Initially write a ZERO buffer since MIC 
                        //       has not been accessed yet.
                        if (appData.activeMicInterfaceAlternateSetting == 
                                 APP_USB_MIC_RECORD_MONO)
                        {
                            //memset(currentAecBuffer, 0, 
                            memset(currentAecBuffer, 1, 
                                   appData.usbWriteBufSize);

                            //Write Mono Record Buffer - AEC
                            if (printCnt2-- > 0) SYS_MESSAGE("{w}");
                            audioErrWrite = USB_DEVICE_AUDIO_Write(
                                    USB_DEVICE_INDEX_0, 
                                    &currentQueue->usbWriteHandle, 
                                    2,  //Interface #
                                    currentAecBuffer,  //Zeros
                                    appData.usbWriteBufSize);

                            if (audioErrWrite == USB_DEVICE_AUDIO_RESULT_OK)
                            {
                                currentQueue->usbInUse = true;
                                currentQueue->usbWriteComplete = false;
                                appWRQueue.usbWriteQueueCnt++;
                            }
                            else
                            {
                                //USB Write Failed
                                //Write Mono Record Buffer - AEC
                                if (audioErrWrite==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                                {
                                    SYS_MESSAGE("{QWOv}");
                                    //char strDst[32] = "QWOV h=";
                                    //DEBUG_PRINT1(strDst, appWRQueue.headIdx);
                                }
                                else SYS_MESSAGE("{EWe}");
                                errState = true;
                                currentQueue->usbInUse = false;
                                break;
                            }
                        }
                        else
                        {
                            //Allow Playback Only
                            currentQueue->usbWriteComplete = true;
                        }

                        appWRQueue.headIdx = _APP_GetNextIdx(appWRQueue.headIdx);

                    } //End Buffer Available Check
                    else
                    {
                        currentQueue->usbInUse = false;
                        errState = true;
                        break;
                    }

                } //End USB RW Queing Loop

                //Check for Error
                if (errState == false)
                {
                    //SYS_PRINT("APP: USB Initial R/W Queuel URQ=%d UWQ=%d\r\n",
                    //          appWRQueue.usbReadQueueCnt,
                    //          appWRQueue.usbWriteQueueCnt);
                    appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                }
                else
                {
                    SYS_MESSAGE("[EUB-U]");
                    errorNum = ERR_USB_BUFFER;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            
            }//End Alternate Settings Playback/Record
            else
            {
                //Not ready for Playback/Record
                SYS_MESSAGE("[UNR]\r\n"); 
                appData.state = APP_STATE_IDLE;
            }
        } //End case APP_STATE_INITIAL_USB_READS
        break;


        //======================================================================
        // Initial Codec WriteRead Request
        //
        // Transition From:
        //     APP_SUBMIT_INITIAL_CODEC_WRITEREAD_REQUEST
        //     USB_DEVICE_AUDIO_EVENT_READ_COMPLETE
        //
        // Wait: until both interfaces are active and usbReadCompletFlag
        //
        // Transition To: APP_STATE_PROCESS_DATA 
        //======================================================================
        case APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS:
        {    
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO)  ||
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_MONO))
            {
                if (appWRQueue.usbInitialReadsCompleteFlag == true)
                {

                    static bool errState = false;
                    errState = false;
                    for (i = 0;
                         i < APP_QUEUE_SIZE/QUEUE_USB_INIT_PART; 
                         i++)
                    {
                        //int8_t headIdx = appWRQueue.headIdx; //Queue HEAD
                        int8_t tailIdx  = appWRQueue.tailIdx;   //Queue TAIL
                        currentQueue    = &(appWRQueue.buffer[tailIdx]);
                        currentPlayback = (DRV_I2S_DATA16 *)
                                              (currentQueue->playbackbuffer);
                        currentRecord   = (DRV_I2S_DATA16 *)
                                              (currentQueue->recordbuffer);
                        currentQueue->bufferIdx = tailIdx;

                        //Initiate write and read
                        if ((currentQueue->usbReadComplete==true) && 
                            (currentQueue->usbWriteComplete==true) && 
                            (currentQueue->codecInUse == false))
                        {
                            //Initial CODEC Write
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                            TEST3_Toggle();
#endif //__CORE_CM7_H_GENERIC
#endif
                            //temp = _CP0_GET_COUNT();
                            //coreTimerDiff = temp - loopTimerValue; 
                            //loopTimerValue = temp;
                            //cycleCnt = coreTimerDiff * 2; 
                            //timeUs = cycleCnt*1e6/SYS_CLK_FREQ;
                            //SYS_PRINT("APC: AEC(%d) WIX(%d) RIX(%d) CYCLES %d (%d Us)\r\n",

                            //appData.codecClientWriteRead.handle = 
                            //        DRV_CODEC_BUFFER_HANDLE_INVALID; 

                            SYS_MESSAGE("[CWR]");                       
                            DRV_CODEC_BufferAddWriteRead(
                                    appData.codecClientWriteRead.handle, 
                                    &(currentQueue->codecWRHandle),
#ifdef DEBUG_TONE_CODEC
                                    sinBuffer,
#else
                                    currentPlayback,
#endif
                                    currentRecord, 
                                    appData.codecClientWriteRead.bufferSize); 

                            if (currentQueue->codecWRHandle != 
                                DRV_CODEC_BUFFER_HANDLE_INVALID)
                            {
                                //++TAIL
                                currentQueue->codecInUse       = true;
                                currentQueue->usbReadComplete  = false;
                                currentQueue->usbWriteComplete = false;
     
                                appWRQueue.codecWRQueueCnt++;

                                //SYS_PRINT("CQ%d %02x)\r\n",
                                //          i,currentQueue->codecWRHandle);
     
                                //TAIL++  
                                appWRQueue.tailIdx = _APP_GetNextIdx(tailIdx);
                            }
                            else
                            {
                                currentQueue->codecInUse = false;
                                errState                 = true;
                                break;
                            }
                        }
                        else
                        {

                            SYS_MESSAGE("[EUB-C]");
                            errorNum   = ERR_USB_BUFFER;
                            errState = true;
                            break;
                        }
                    } //End Initial Codec WR Queue Request Loop

                    if (errState == false)
                    {
                        SYS_PRINT(
                        "APD: Codec Initial WR Complete CWRQ=%d CWRC=%d URC=%d UWC=%d\r\n",
                                  appWRQueue.codecWRQueueCnt,
                                  appWRQueue.codecWRCompleteCnt,
                                  appWRQueue.usbReadCompleteCnt,
                                  appWRQueue.usbWriteCompleteCnt);
                        appWRQueue.usbInitialReadsCompleteFlag = false;
                        appData.state = APP_STATE_PROCESS_DATA;
                        
                    }
                    else
                    {
                        errorNum = ERR_CODEC_BUFFER; 
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                } //End usbInitialReadsCompleteFlag
                else
                {
                    //Wait for USB to complete
                    appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                    break;
                }
            } //End Alternate Settings Playback
            else
            {
                //Not Ready for Playback/Record
                SYS_MESSAGE("[UNR]\r\n"); 
                appData.state = APP_STATE_IDLE;
            }
        }
        break;

        //======================================================================
        // Process USB Read buffers to Codec Playback  and Codec read mic data to
        // USB Write buffers
        //
        // The codec mic data runs through the echo canceller and is then sent
        // to the USB record ping-pong buffer
        //
        // Transition From:
        //    APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD
        //    --When the alt settings are not really changed from NONE

        //    APP_STATE_SUBMIT_INITIAL_CODEC_WRITEREAD_REQUEST:
        //    --When both alt setting is APP_USB_SPEAKER_PLAYBACK_STEREO
        //       && appWRQueue.usbInitialReadsCompleteFlag
        //======================================================================
        case APP_STATE_PROCESS_DATA:
        {
            //Disable interrupts - critical section
            #if defined(__arm__)
            //See the following page:
            //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
            __disable_irq(); // Set PRIMASK
            #elif defined(__mips__)
            interruptStatus = __builtin_disable_interrupts(); 
            #endif

            //USB Playback/Record 
            if ((appData.activeInterfaceAlternateSetting == 
                  APP_USB_SPEAKER_PLAYBACK_STEREO)  ||
                (appData.activeMicInterfaceAlternateSetting == 
                APP_USB_MIC_RECORD_MONO))
            {
                int8_t tailIdx  = appWRQueue.tailIdx;  //Queue TAIL
                currentQueue    = &(appWRQueue.buffer[tailIdx]);

                int8_t headIdx = appWRQueue.headIdx;  //Queue HEAD

                //--------------------------------------------------------------
                // Codec WR Queueing
                //--USB Write and Read Completed, ready for 
                //  speaker Playback/Mic Record

#ifdef DEBUG_MIC_LOOPBACK
                //Loopback the current record buffer used for USB IN 
                //to the codec playback
                currentQueueH   = &(appWRQueue.buffer[headIdx]);
                currentPlayback = (DRV_I2S_DATA16 *)
                                      (currentQueueH->recordbuffer);
#else
                currentPlayback = (DRV_I2S_DATA16 *)
                                      (currentQueue->playbackbuffer);
#endif
                currentRecord   = (DRV_I2S_DATA16 *) 
                                      (currentQueue->recordbuffer);
                currentQueue->bufferIdx = tailIdx;

                //------------------------------------------------------------------ 
                //Codec WR after USB RW Complete
                //--Playback USB Read Buffer
                //--Get new Codec Read Buffer (with echo)
                if ((currentQueue->usbWriteComplete == true)  &&
                    (currentQueue->usbReadComplete  == true)  &&
                    (currentQueue->codecInUse       == false) &&
                    (currentQueue->codecWRComplete  == false) &&
                    (currentQueue->codecAecComplete == false))
                {
                        //CODEC WRITE/READ Request
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                    TEST3_Toggle();
#endif //__CORE_CM7_H_GENERIC
#endif
                    //appData.codecClientWriteRead.handle = 
                        //DRV_CODEC_BUFFER_HANDLE_INVALID; 
                    DRV_CODEC_BufferAddWriteRead(
                            appData.codecClientWriteRead.handle, 
                            &currentQueue->codecWRHandle,
#ifdef DEBUG_TONE_CODEC
                                    sinBuffer,
#else
                                    currentPlayback,
#endif
                                    currentRecord, 
                                    appData.codecClientWriteRead.bufferSize);

                    if (currentQueue->codecWRHandle != 
                        DRV_CODEC_BUFFER_HANDLE_INVALID)
                    {
                        
                        currentQueue->codecInUse       = true;
                        //currentQueue->usbReadComplete  = false;
                        //currentQueue->usbWriteComplete = false;
                        currentQueue->codecWRComplete  = false;
                        currentQueue->codecAecComplete = false;

                        appWRQueue.codecWRQueueCnt++;

                        //++TAIL  (Next Codec WR)
                        appWRQueue.usbCompleteBufferLevel--;
                        appWRQueue.tailIdx = _APP_GetNextIdx(appWRQueue.tailIdx);

                        if (appWRQueue.codecWRQueueCnt % 5000 == 0)
                        {
                            SYS_PRINT("WRQC: %d",appWRQueue.codecWRQueueCnt);
                        }

                        //Check for QUEUE Empty 
                        //if (appWRQueue.usbCompleteBufferLevel == 0)
                        //{
                           //QUEUE is empty do not execute USB Read command;
                        //   SYS_PRINT("APP: Codec Playback Queue EMPTY(%d)", 
                                     //appWRQueue.
                                     //usbCompleteBufferLevel);
                           //queueEmpty = true;
                        //}
                        //else
                        //{
                        //   queueEmpty = false;
                        //}
                    }
                    else
                    {
                        SYS_PRINT("APH: ERROR Codec WR Error\r\n");
                        errorNum = ERR_CODEC_BUFFER;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                } //End Codec WRITE/READ Bufffer Ready

                //--------------------------------------------------------------
                // USB Read and Write Request to HEAD
                currentQueue    = &appWRQueue.buffer[headIdx];
                currentPlayback = (DRV_I2S_DATA16 *) 
                                  (currentQueue->playbackbuffer);
                currentRecord   = (DRV_I2S_DATA16 *) 
                                  (currentQueue->recordbuffer);
                currentAecBuffer  = (int16_t *) currentQueue->aecbuffer;

                if (appWRQueue.usbCompleteBufferLevel  == 
                    APP_QUEUE_SIZE)
                {
                    SYS_MESSAGE("[QOv]\r\n");
                    queueFull = true;
                }

                //Check Playback/Record QUEUE Buffers Ready 
                //--Playback and Record data buffers ready
                if ( (currentPlayback != NULL) && 
                     (currentRecord   != NULL) && 
                     (currentQueue->codecWRComplete == true) )
                {

                    //USB Read (Playback stream) 
                    //NOTE:  Buffer size always at the maximum value (1ms @ 48000Hz)
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                        TEST1_Toggle();
#endif //__CORE_CM7_H_GENERIC
#endif
                    //SYS_MESSAGE("{R}");
                    audioErrRead  = USB_DEVICE_AUDIO_Read(
                                        USB_DEVICE_INDEX_0, 
                                        &currentQueue->usbReadHandle, 
                                        1, currentPlayback,  
                                        appData.usbReadBufSize);

                    //BYPASS THE CODEC - LOOPBACK USB READ to USB WRITE
                    //USB Write (Record stream)
                    if (appData.recordActive == 1) 
                    {
                        //--------------------------
                        //TODO:  Acoustic Echo Cancelling
                        //--Discard empty channel and convert to mono buffer
                        //--Acoustic Echo Cancellation (if enabled)
                        //Stereo Mic audio Input to Mono 
                        _aecSumStereoChannels(
                                        (DRV_I2S_DATA16 *) currentRecord, 
                                        //(DRV_I2S_DATA16 *) currentPlayback, //Loopback
                                        yInQ1d15,
                                        appData.codecClientWriteRead.bufferSize/
                                        APP_INT16_STEREO_SAMPLE_BYTES);

                        

                        //NO ECHO CANCELLER

                        //Stereo Mic audio to Mono USB
                        memcpy(currentAecBuffer, yInQ1d15, 
                                appData.codecClientWriteRead.bufferSize/
                                                APP_INT16_STEREO_SAMPLE_BYTES);
    
                        //SYS_MESSAGE("{W}");
                        audioErrWrite = USB_DEVICE_AUDIO_Write(
                                        USB_DEVICE_INDEX_0, 
                                        &currentQueue->usbWriteHandle, 
                                        2, currentAecBuffer,
                                appData.usbWriteBufSize);
                                        //appData.codecClientWriteRead.bufferSize/
                                        //        APP_INT16_MONO_SAMPLE_BYTES);
                    }
                    else
                    {
                        audioErrWrite = USB_DEVICE_AUDIO_RESULT_OK;
                    }

                    if ((audioErrRead == USB_DEVICE_AUDIO_RESULT_OK) &&
                        (audioErrWrite== USB_DEVICE_AUDIO_RESULT_OK))
                    {
                        //Valid USB Operations
                        currentQueue->usbInUse         = true;
                        currentQueue->usbReadComplete  = false;
                        currentQueue->codecInUse       = false;                
                        currentQueue->codecWRComplete  = false;
                        currentQueue->codecAecComplete = false;
                        appWRQueue.usbReadQueueCnt++; 

                        if (appData.recordActive == false)
                        {
                            //Don't wait for Write Complete
                            currentQueue->usbWriteComplete = true;
                        }
                        else
                        {
                            //Wait for write complete
                            currentQueue->usbWriteComplete = false;
                            appWRQueue.usbWriteQueueCnt++; 
                        }

#if 1
                        if ((appWRQueue.usbWriteQueueCnt+1) % 5000 == 0)
                        {
                            SYS_PRINT("UWQ %d", appWRQueue.usbWriteQueueCnt+1);
                        }
                        if ((appWRQueue.usbReadQueueCnt+1) % 5000 == 0)
                        {
                            SYS_PRINT("URQ %d", appWRQueue.usbReadQueueCnt+1);
                        }
#endif //0

                        //Next Buffer
                        appWRQueue.headIdx = _APP_GetNextIdx(appWRQueue.headIdx);
                    }
                    else
                    {
                        //ERROR
                        //currentQueue->usbInUse = false;
                        SYS_PRINT("[URW Q Error]\r\n");
                        errorNum = ERR_CODEC_BUFFER;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                    
                } //End codecAecComplete (USB Read/Write)

            } //End Record and Playback Alternate Setting
            else
            {
                //Not Ready for Playback/Record
                appData.state = APP_STATE_IDLE;
            }
                
            //Enable Interrupts
            //#if defined(__CORE_CM7_H_GENERIC)
            #if defined(__arm__)
            __enable_irq(); // Clear PRIMASK
            #elif defined(__mips__)
            __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
            #endif

        } //End case APP_STATE_PROCESS_DATA
        break;

        //======================================================================
        //======================================================================
        case APP_STATE_MUTE_AUDIO_PLAYBACK:
        {
            if ((appData.activeInterfaceAlternateSetting ==
                   APP_USB_SPEAKER_PLAYBACK_NONE) &&
                 (appData.activeMicInterfaceAlternateSetting == 
                   APP_USB_MIC_RECORD_NONE))
            {
                appData.ledState = LED_MUTE_BLINK; 
                SYS_MESSAGE("[MuON]");
                DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                appData.muteStatus = true;

                appData.state = APP_STATE_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting ==
                   APP_USB_SPEAKER_PLAYBACK_STEREO) 
            {
                    SYS_PRINT("[MuteOFF]");
                DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                appData.muteStatus = false;
                appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
                appData.playbackActive = true;
                if (appData.activeMicInterfaceAlternateSetting == 
                   APP_USB_MIC_RECORD_MONO)
                {
                    appData.recordActive   = true;
                }
            }
            else if (appData.activeMicInterfaceAlternateSetting == 
                     APP_USB_MIC_RECORD_NONE)
            {
                appData.recordActive = false;
            }
            else if (appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_NONE)
            {
                appData.playbackActive = false;
            }
        }
        break;

        //======================================================================
        //======================================================================
        case APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP:
        {
            //Changes sampling rate for Record and Playback (both stereo)
            SYS_PRINT("[HPFs%d]\r\n", appData.sampleFreq);
                    
            DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle, 
                                              appData.sampleFreq);
            //Reinit the queue
            hpInterfaceChanged = 1;
            appData.state = APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
        }
        break;


        //======================================================================
        //======================================================================
        case APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP:
        {
            //Changes sampling rate for Record and Playback (both stereo)
            SYS_PRINT("[MPFs%d]\r\n", appData.sampleFreqMic);
                    
            if (appData.sampleFreqMic == appData.sampleFreq)
            {
                //DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle, 
                //                                  appData.sampleFreqMic);
            }
            //Reinit the queue
            mpInterfaceChanged = 1;
            appData.state = APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
        }
        break;
        
        //======================================================================
        //======================================================================
        case APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        {
            SYS_STATUS codecStatus;

            //SYS_PRINT("ALt Setting RCVD: HP=%d MP=%d\r\n",
            //        hpInterfaceChanged, mpInterfaceChanged);

            //Check for No Audio
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE)  &&
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_NONE))
            {

                appData.playbackActive = false;
                appData.recordActive   = false;

                hpInterfaceChanged = 0;
                mpInterfaceChanged = 0;
      
                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
                    if (appData.playbackActive == false && 
                        appData.recordActive == false)
                    {
                        SYS_MESSAGE("[pima]");
                        appData.ledState = LED_MUTE_BLINK; 
                        if (DRV_CODEC_ClientReady(0) == true)
                        {
                            DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                            appData.muteStatus = true;
                        }
                        else
                        {
                            SYS_MESSAGE("[NR]\r\n");
                        }
                    }    
                    appData.state = APP_STATE_IDLE;
                    break;
                }
                else
                {
                    //Codec Not Ready 
                    SYS_MESSAGE("[CNR]\r\n");
                    errorNum = ERR_CODEC;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            } //End !MP || !HP

            //Playback - Headphone interface ready
            if (hpInterfaceChanged && 
                (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO))
            {
                appData.playbackActive = true;

                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {

                    if (DRV_CODEC_ClientReady(0) == true)
                    {
                        hpInterfaceChanged = 0;

                        //NOTE:  Playback without record allowed.
                        _APP_Init_RWBufferQueue();
                        SYS_MESSAGE("[PA]");
                        DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                        appData.muteStatus = false;
                        appData.state =  APP_STATE_INITIAL_USB_READ_REQUESTS;

                    }
                }
                else
                {
                    //Code ERROR
                    SYS_MESSAGE("[CNR]\r\n");
                    errorNum = ERR_CODEC;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
            else if (hpInterfaceChanged && 
                (appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_NONE))
            {
                //SYS_MESSAGE("HP Interface Off\r\n");
                SYS_MESSAGE("[pi]");
                appData.state = APP_STATE_IDLE;
            }

            //Record - Mic Interface Ready
            if (mpInterfaceChanged && 
                (appData.activeMicInterfaceAlternateSetting == 
                        APP_USB_MIC_RECORD_MONO))
            {
                appData.recordActive   = 1;

                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {

                    if (DRV_CODEC_ClientReady(0) == true)
                    {

                        SYS_MESSAGE("[MA]");

                        _APP_Init_RWBufferQueue();
                        DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                        appData.muteStatus = false;

                        //NOTE:  Need playback working with record
                        //appData.state =  APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                        if (appData.playbackActive == true)
                        {
                            mpInterfaceChanged = 0;
                            appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
                            break;
                        }
                        else
                        {
                            SYS_MESSAGE("[mi]");
                            //IDLE and Wait for Playback to become Active
                            // -- TODO:   Record without playback
                            //appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                            appData.state = APP_STATE_IDLE;  //No Audio
                            break;
                        }
                    }
                    else
                    {
                        SYS_MESSAGE("[mNR]\r\n");
                        //errorNum = ERR_CODEC;
                        //appData.state = APP_STATE_ERROR;
                        appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                        break;
                    }
                }
                else
                {
                    SYS_MESSAGE("[mNR]\r\n");
                    errorNum = ERR_CODEC;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            } //End Mic USB Interface Alt Setting
            else if (mpInterfaceChanged && 
                (appData.activeMicInterfaceAlternateSetting == 
                     APP_USB_MIC_RECORD_NONE))
            {
                appData.recordActive   = 0;
                SYS_PRINT("[mi]r\n");
                appData.state = APP_STATE_IDLE;
            }

        } //End case APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        break;

        //No Audio
        case APP_STATE_IDLE:
        {
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE) &&
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_NONE))
            {
                appData.ledState = LED_MUTE_BLINK; 
                DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                appData.muteStatus = true;
                appData.playbackActive = false;
                appData.recordActive   = false;
            }
        }
        break;

        case APP_STATE_ERROR:
        {
            /* The default state should never be executed. */
            static int printErr = 1;
            if (printErr == 1)
            {
                printErr = 0;
                SYS_PRINT("APP ERROR:  %s", ERROR_STR[errorNum]);
                SYS_DEBUG(0, "APP ERROR:  HALT!!!");
            }
        }
        break;

        default:
        {
            SYS_DEBUG(0,"APP ERROR:  Invalid STATE -- HALT!!!");
            break;
        }
    } //End switch(appData.state)

} //End APP_Tasks()

//******************************************************************************
//  APP_LED_Tasks())
//******************************************************************************
void _APP_LED_Tasks()
{
    //LED1 
    if(!appData.blinkDelay)
    {
        switch(appData.ledState)
        {
            //Connection State
            case LED_ON:
                LED1_On();
            break;

            case LED_OFF:
                LED1_Off();
            break;

            case LED_CONNECTED_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED1_Toggle();
                    appData.blinkDelay = 1000;  
                }
            break;

            case LED_MUTE_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED1_Toggle();
                    appData.blinkDelay = 500;  
                }
            break;

            case LED_ERROR_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED1_Toggle();
                    appData.blinkDelay = 250;  
                }
            break;



            default:
                LED1_Off();

        } //End switch(state)
    } //End LED1
} //End _APP_LED_Tasks()


//******************************************************************************
// APP_Button_Tasks()
//******************************************************************************
void _APP_Button_Tasks()
{
   //BUTTON PROCESSING
    /* Check the buttons' currentPlayBuffer state. */      
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
          
        //Short Press - change volume/mute on/off
        //Long Press - volume/mute mode
        case BUTTON_STATE_BUTTON0_PRESSED:
        {
            if ((appData.buttonDelay>0)&&
                (SWITCH_Get()!=SWITCH_STATE_PRESSED))     // SW01 pressed and released < 1 sec
            {
                //New volume
                appData.volumeIndex++;
                if (appData.volumeIndex >= VOLUME_STEPS)
                {
                    appData.volumeIndex = 0;    
                }
                appData.volume = volumeLevels[appData.volumeIndex];
                
                if (0==appData.volume)
                {
                    appData.ledState = LED_MUTE_BLINK;
                    appData.muteEn = true;
                    SYS_MESSAGE("[MO]\r\n");
                    DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                    appData.muteStatus = true;
                }
                else
                {
                    appData.ledState = LED_ON;
                    appData.muteEn = false;
                    SYS_PRINT("APX: Mute Audio Off\r\n");
                    DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                    appData.muteStatus = false;
                    DRV_CODEC_VolumeSet(appData.codecClientWriteRead.handle, 
                                       DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                       appData.volume);
                    #ifdef DRV_WM8904_INSTANCES_NUMBER 
                    //TODO: Not implemented for AK4954
                    //DRV_CODEC_MicGainSet(appData.codecClientWriteRead.handle, 
                    //                     appData.micGain);
                    #endif 

                }

                //Set the debounce value
                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              
            }
            else if ((appData.buttonDelay==0)&&
                     (SWITCH_Get()==SWITCH_STATE_PRESSED))  
            {

                //if (appData.muteEn == true)
                //{
                //    appData.ledState = LED_ON;
                //    appData.muteEn   =false;
                //    SYS_PRINT("APP: Mute Audio Off\r\n");
                //    DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                //}
                //else
                //{
                //    appData.ledState = MUTE_BLINK;
                //    appData.muteEn   =true;
                //    DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                //    appData.muteStatus = false;
                //    appData.muteStatus = true;
                //}
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
} //End APP_Button_Tasks()

//******************************************************************************
// APP_CODECBufferEventHandlerWriteRead()
//
// Application CODEC buffer Event handler.
// This function is called back by the CODEC driver when
// a CODEC data buffer TX completes.
//******************************************************************************
void APP_CODECBufferEventHandlerWriteRead(DRV_CODEC_BUFFER_EVENT event,
        DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context )
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
        {
            appWRQueue.codecWRCompleteCnt++;

            //This buffer is ready for USB Write/Read after AEC is performed
            //_APP_ClearCodecReturnBuffer(handle);
            static volatile int16_t bufferIdx = -1;
            bufferIdx = _APP_SetCodecWriteReadComplete(handle);
            //SYS_PRINT("__CC%d %08x)\r\n",
            //    appWRQueue.lastCWRCIdx, handle);
            if (bufferIdx != -1)
            {
                appWRQueue.usbCompleteBufferLevel--;

                if (bufferIdx != appWRQueue.lastCWRCIdx)
                {
                    SYS_PRINT(
                        "[CBErr %d-%d h=0x%8x]\r\n",
                        appWRQueue.lastCWRCIdx, bufferIdx, handle);
                }
            }
            else
            {
                SYS_PRINT(
                    "[CBErr -1-%d h=0x%8x]\r\n",
                    appWRQueue.lastCWRCIdx, bufferIdx, handle);
            }

            if (appWRQueue.usbCompleteBufferLevel <= 0)
            {
                //USB Read needs to complete before next Codec Write.
                SYS_MESSAGE("APQ: QUEUE Empty\r\n");
                appWRQueue.usbCompleteBufferLevel = 0;
                queueEmpty = true;
            }
        }
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
            errorNum = ERR_CODEC_EVENT;
            appData.state = APP_STATE_ERROR;
        } 
        break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
            errorNum = ERR_CODEC_EVENT;
            appData.state = APP_STATE_ERROR;
        } 
        break;

    }
} //End APP_CODECBufferEventHandlerWriteRead()

// ****************************************************************************
// APP_TimerCallback()
// --Callback for the periodic timer interrupt
//******************************************************************************
static void _APP_TimerCallback( uintptr_t context)
{
    if (appData.buttonDelay > 0)
    {      
        appData.buttonDelay--;
    }
    if (appData.blinkDelay > 0)
    {
        appData.blinkDelay--;
    }
    //if (appData.clockStableDelayMs > 0)
    //{
    //    appData.clockStableDelayMs--;
    //}
    //else
    //{
    //    appData.clockStableDelayFlag = true;  //Stable after delay
    //}
} 

//******************************************************************************
// _APP_SetUSBReadBufferReady()
//Set the USB Read Buffer Data Ready 
//******************************************************************************
static int _APP_SetUSBReadBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle)
{
    appWRQueue.lastURCIdx = _APP_GetNextIdx((int16_t) appWRQueue.lastURCIdx);

    int i=0;
    for(i=0;i<APP_QUEUE_SIZE;i++)
    {
        if (appWRQueue.buffer[i].usbReadHandle == handle)
        {
            appWRQueue.buffer[i].usbReadComplete = true;
            if (appWRQueue.buffer[i].usbWriteComplete == true)
            {
                //Both USB Read and Write requests are complete
                appWRQueue.buffer[i].usbInUse = false;
                appWRQueue.usbCompleteBufferLevel++;
            }
            return 1;
            //break;
        }
    }
    return 0; //FAIL
}
//******************************************************************************
// _APP_SetUSBWriteBufferReady()
//Set the USB Write Buffer Data Ready 
//******************************************************************************
static int _APP_SetUSBWriteBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle)
{
    appWRQueue.lastUWCIdx = _APP_GetNextIdx((int16_t) appWRQueue.lastUWCIdx);

    int i=0;
    for(i=0;i<APP_QUEUE_SIZE;i++)
    {
        if (appWRQueue.buffer[i].usbWriteHandle == handle)
        {
            appWRQueue.buffer[i].usbWriteComplete = true;
            if (appWRQueue.buffer[i].usbReadComplete == true)
            {
                //Both USB Read and Write requests completed
                appWRQueue.buffer[i].usbInUse = false;
                appWRQueue.usbCompleteBufferLevel++;
            }
            return 1;
            //break;
        }
    }
    return 0; //FAIL
}

//******************************************************************************
// _APP_USBReadAllBufferReady()
//    Check if USB Read Buffer Queue is Ready for CODEC Writes 
//******************************************************************************
static bool _APP_USBReadAllBufferReady()
{
    int i = 0;

    for (i=0; i<APP_QUEUE_SIZE/QUEUE_USB_INIT_PART; i++)
    {
        if ((appWRQueue.buffer[i].usbReadComplete != true) ||
            (appWRQueue.buffer[i].usbWriteComplete != true))
        {
            return false;
        }
    }
    return true;
}

//******************************************************************************
// _APP_Init_RWBufferQueue()
//Initialize Codec Playback Buffer Queue
//******************************************************************************
static void _APP_Init_RWBufferQueue()
{
    int i=0;
    appWRQueue.tailIdx     =  0;
    appWRQueue.lastCWRCIdx = -1;
    appWRQueue.headIdx     =  0;
    appWRQueue.lastURCIdx  = -1;
    appWRQueue.lastUWCIdx  = -1;
    appWRQueue.aecIdx = 0;

    for(i=0;i<APP_QUEUE_SIZE;i++)
    {
        appWRQueue.buffer[i].codecInUse       = false;
        appWRQueue.buffer[i].usbInUse         = false;
        appWRQueue.buffer[i].usbReadComplete  = false;
        appWRQueue.buffer[i].usbWriteComplete = false;
        appWRQueue.buffer[i].codecWRComplete  = false;
        appWRQueue.buffer[i].codecAecComplete = false;
        appWRQueue.buffer[i].codecWRHandle    = DRV_CODEC_BUFFER_HANDLE_INVALID;
    }
    appWRQueue.usbCompleteBufferLevel = 0;
    appWRQueue.codecWRQueueCnt        = 0;
    appWRQueue.codecWRCompleteCnt     = 0;
    appWRQueue.usbReadQueueCnt        = 0;
    appWRQueue.usbWriteQueueCnt       = 0;
    appWRQueue.usbReadCompleteCnt     = 0;
    appWRQueue.usbWriteCompleteCnt    = 0;


    appWRQueue.usbInitialReadsCompleteFlag = false;

    queueFull  = false;
    queueEmpty = true;

    if (DRV_CODEC_ClientReady(0))
    {
        DRV_CODEC_ReadQueuePurge(appData.codecClientWriteRead.handle);
        DRV_CODEC_WriteQueuePurge(appData.codecClientWriteRead.handle);
    }
    skip=1;
} //End _APP_Init_RWBufferQueue()


//******************************************************************************
//_APP_GetNextIdx()
//Increment the Head or Tail Index to Codec Playback Buffer Queue
//******************************************************************************
static int16_t _APP_GetNextIdx(int16_t index)
{
    return (index+1)%APP_QUEUE_SIZE;
}

//******************************************************************************
// _APP_SetCodecWriteReadComplete()
//Set the Codec Read Buffer Complete 
//******************************************************************************
static int _APP_SetCodecWriteReadComplete(DRV_CODEC_BUFFER_HANDLE handle)
{
    static int i;

    appWRQueue.lastCWRCIdx = _APP_GetNextIdx(appWRQueue.lastCWRCIdx);
    for(i=0; i<APP_QUEUE_SIZE;i++)
    {
        if(appWRQueue.buffer[i].codecWRHandle == handle)
        {
            appWRQueue.buffer[i].codecInUse      = false;
            appWRQueue.buffer[i].codecWRComplete = true; 
            return i;
        }
    }
    return -1;
}

//******************************************************************************
// _aecSumStereoChannels
//
// Summary:
//   Adds up the stereo playback channels to make a mono playback 
//
//   NOTE:  Same as _APP_DiscardTheEmptyChannelFromMicrophone()
//
// Arguments:
//    DRV_I2S_DATA16 *input   - [in] Pointer to stereo input buffer
//    q15            *output  - [in] Pointer to mono output buffer
//    int          numSamples - [in] #Stereo Samples 
//
// Return Value: //   None
//
//******************************************************************************
static void _aecSumStereoChannels(DRV_I2S_DATA16 * input, 
                                  q15 * output, 
                                  int numSamples)
{
    int i;

    for(i = 0;i<numSamples; i++)
    {
        output[i] =  libq_q15_Add_q15_q15(input->leftData, input->rightData);
    }
}

/*******************************************************************************
 End of File
 */

