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
Copyright (c) 2020 released Microchip Technology Inc.  All rights reserved.

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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "app_globals.h"

extern USB_DEVICE_IRP gUSBDeviceAudioIRP[USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED];
int IrpQueueCount(void);

#define q15 int16_t

//DEBUG
#undef  DEBUG_BUFFER_TIMING
#undef  DEBUG_TONE_CODEC  //Sin Tone Output
#undef  DEBUG_MIC_LOOPBACK

#undef DEBUG_G55_PRINT
#ifdef DEBUG_G55_PRINT
void DEBUG_PRINT1(char * strPre, int i, char * strPost);
void DEBUG_PRINT2(char * strPre, int i, char * strMid,  int i2, char * strPost);
char * paStr[] = { "pi", "PA" }; 
char * maStr[] = { "mi", "MA" }; 
char strPrint[32] = "";
#endif //DEBUG_G55_PRINT

static int _printCnt1;
static int _printCnt2;
static bool _initQ = true; 
static bool _first = false;
#define REPORTINTVL 5000

//Playback Buffer QUEUE Initialization 
//N, where N gives the fraction of the QUEUE to be filled, i.e. 1/N
//and USB reads generated and completed and Codec AddWrites before
//transitioning to APP_STATE_PROCESS_DATA timing. 
#define QUEUE_USB_INIT_PART   2  

bool queueFull;
bool queueEmpty;

uint32_t someVar;  //Codec context variable

//appWRQueue
static int16_t tailIdx;
static int16_t headIdx;
static int16_t usbPlaybackQueueLevel    = 0;
static int16_t usbRecordQueueLevel      = 0;
static bool    usbPlaybackRestart;
static bool    usbRecordRestart;

#if defined(__mips__)
uint32_t interruptStatus;
#endif

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
static CACHE_ALIGN APP_BUFFER_QUEUE appWRQueue;

//Audio Processing Buffers
APP_RW_BUFFER     * currentQueueT;
APP_RW_BUFFER     * currentQueueH;
int16_t           * currentAecBuffer; //MicIn AEC Processed Audio buffer.
DRV_I2S_DATA16    * currentPlayback;
DRV_I2S_DATA16    * currentRecord;

#ifdef DEBUG_MIC_LOOPBACK
DRV_I2S_DATA16    * currentPlaybackH;
DRV_I2S_DATA16    * currentRecordH;
#endif

//int16_t           * xOutQ1d15[APP_MAX_RECORD_BUFFER_BYTES];  //Speaker Output Processed Buffer
int16_t           yInQ1d15[APP_MAX_RECORD_BUFFER_BYTES];   //MicInput Processed buffer 

APP_RW_BUFFER     * nextQueue;
DRV_I2S_DATA16    * nextPlayback;
DRV_I2S_DATA16    * nextRecord;
int16_t           * nextAecBuffer; //MicIn AEC Processed Audio buffer.

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
void CRLF();

//==============================================================================
// Application Playback Buffer Queue
//==============================================================================
static int     _APP_SetUSBReadBufferReady(
                        USB_DEVICE_AUDIO_TRANSFER_HANDLE handle, 
                        uint16_t frameLengthBytes);
static int     _APP_SetUSBWriteBufferReady(
                        USB_DEVICE_AUDIO_TRANSFER_HANDLE handle, 
                        uint16_t frameLengthBytes);
//static void    _APP_SetAecBufferReady(int idx);
static int     _APP_SetCodecWriteReadComplete(DRV_CODEC_BUFFER_HANDLE handle);
static void    _APP_Init_RWBufferQueue();
static int16_t _APP_GetNextIdx(int16_t index);
static bool    _APP_USBInitAllBufferReady();

static void _aecSumStereoChannels(DRV_I2S_DATA16 * input, 
                                  q15 * output, 
                                  int numSamples);

static bool hpInterfaceChanged = false;
static bool mpInterfaceChanged = false;
 USB_DEVICE_AUDIO_RESULT audioErrRead;
 USB_DEVICE_AUDIO_RESULT audioErrWrite;
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


APP_DATA appData;

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

    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
        {
            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
        }
        break;

        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
        {
            // USB device is reset or device is de-configured.
            // This means that USB device layer is about to de-initialize
            // all function drivers. So close handles to previously opened
            // function drivers.

            // Also turn ON LEDs to indicate reset/de-configured state.
            /* Switch on red and orange, switch off green */
            appData.isConfigured = false;
            appData.ledState = LED_OFF; 
        }
        break;

        case USB_DEVICE_EVENT_CONFIGURED:
        {
            /* check the configuration */
            configuredEventData = 
                     (USB_DEVICE_EVENT_DATA_CONFIGURED *)pEventData;
            if(configuredEventData->configurationValue == 1)
            {
                /* the device is in configured state */
                appData.ledState = LED_CONNECTED_BLINK; 
                USB_DEVICE_AUDIO_EventHandlerSet(
                        0,
                        APP_USBDeviceAudioEventHandler ,
                        (uintptr_t)NULL);
                /* mark that set configuration is complete */
                appData.isConfigured = true;
                SYS_MESSAGE("APP: USB Device Configured\r\n");
                appData.ledState = LED_ON;
            }
        }
        break;

        case USB_DEVICE_EVENT_SUSPENDED:
        {
            /* Switch on green and orange, switch off red */
            SYS_MESSAGE("APP: USB Suspended\r\n");
            appData.ledState = LED_MUTE_BLINK; 
        }
        break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
        {
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            appData.ledState = LED_CONNECTED_BLINK; 
            SYS_MESSAGE("APP: USB Attached\r\n");
        }
        break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
        {
            /* VBUS was removed. Notify USB stack about the event*/
            SYS_MESSAGE("APP: USB DEtached\r\n");
            USB_DEVICE_Detach (appData.usbDevHandle);
            appData.isConfigured = false;
            appData.ledState = LED_OFF; 
        }
        break;

        case USB_DEVICE_EVENT_ERROR:
        {
            //Code ERROR
            SYS_MESSAGE("APP: USB Device ERROR\r\n");
            errNum = ERR_USB; 
            appData.isConfigured = false;
            appData.ledState = LED_ERROR_BLINK; 
            appData.state = APP_STATE_ERROR;
        }
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
    volatile USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE * readEventData;
    volatile USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE * writeEventData;
    USB_SETUP_REQUEST_RECIPIENT setupReqRecipient;
    uint8_t entityID;
    uint8_t controlSelector;
    uint8_t endpointNumber;

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
                        //SYS_PRINT("\r\n_USB: USB Audio Playback Interface Setting: %d\r\n",
                        //               interfaceInfo->interfaceAlternateSetting);
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
                        //SYS_PRINT("\r\n_USB: Audio Record Interface Setting: %d\r\n",
                        //               interfaceInfo->interfaceAlternateSetting);
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

                usbPlaybackQueueLevel--;
                appWRQueue.usbReadCompleteCnt++;

                if (readEventData->status == USB_DEVICE_AUDIO_RESULT_OK)
                {

                    //Make USB Read Buffer Queue Ready for Data (Clear data flags)
                    _APP_SetUSBReadBufferReady(readEventData->handle, 
                                               readEventData->length);
                    queueEmpty = false;

                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[] = "{rc";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbPlaybackQueueLevel, strPost);
                        SYS_PRINT("{rc%d}",usbPlaybackQueueLevel);
                    }

                    //Check if the this is the initial loading of the playback
                    //queue with USB Rx Data 
                    if (appData.state == 
                           APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS)
                    {
                        if(_APP_USBInitAllBufferReady())
                        {
                            //SYS_MESSAGE("_USB: RW Queue READY\r\n");
                            SYS_MESSAGE("[UQRRdy]\r\n");
                            appWRQueue.usbInitialBufsReady = true; 
                        }
                    }
                }
                else if ((readEventData->status == 
                          USB_DEVICE_AUDIO_RESULT_ERROR_ENDPOINT_HALTED) ||
                         (readEventData->status == 
                          USB_DEVICE_AUDIO_RESULT_ERROR_TERMINATED_BY_HOST))
                {
                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[32] = "{Rab";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbPlaybackQueueLevel, strPost);
                        SYS_PRINT("{RAbort%d(%x)}",
                            usbPlaybackQueueLevel,(uint8_t)readEventData->status);
                    }
                    _APP_SetUSBReadBufferReady(readEventData->handle, 0);
                }
                else
                {
                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[32] = "{Rerr";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbPlaybackQueueLevel, strPost);
                        SYS_PRINT("{RErr%d(%x)}",
                            usbPlaybackQueueLevel,(uint8_t)readEventData->status);
                    }
                    _APP_SetUSBReadBufferReady(readEventData->handle, 0);
                }
            } //End  case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
            break;

            case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
            {
                writeEventData = (USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE *)pData;

                usbRecordQueueLevel--;
                appWRQueue.usbWriteCompleteCnt++;

                //Audio frame was written to the Host.
                //Now get next frame from Audio i2sc0.
                if(writeEventData->status == USB_DEVICE_AUDIO_RESULT_OK)
                {

                    
                    _APP_SetUSBWriteBufferReady(writeEventData->handle,
                                                writeEventData->length);


                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[32] = "{wc";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbRecordQueueLevel, strPost);
                        SYS_PRINT("{wc%d}",usbRecordQueueLevel);
                    }

                    //Check if the this is the initial loading of the playback
                    //queue with USB Rx Data 
                    if (appData.state == 
                           APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS)
                    {
                        if (_APP_USBInitAllBufferReady() && 
                            appWRQueue.usbInitialBufsReady == false)
                        { 
                            //ALL Ready for i2sc0 Playback
                            //SYS_MESSAGE("_USB: RW Queue READY\r\n");
                            SYS_MESSAGE("[UQRR]\r\n");
                            appWRQueue.usbInitialBufsReady = true;
                        }
                    }
                }
                else if ((writeEventData->status == 
                          USB_DEVICE_AUDIO_RESULT_ERROR_ENDPOINT_HALTED) ||
                         (writeEventData->status == 
                          USB_DEVICE_AUDIO_RESULT_ERROR_TERMINATED_BY_HOST)
                   )
                {
                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[32] = "{Wab";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbRecordQueueLevel, strPost);
                        SYS_PRINT("{WAbort%d(%x)}",
                            usbRecordQueueLevel,(uint8_t)writeEventData->status);
                    }
                    _APP_SetUSBWriteBufferReady(writeEventData->handle, 0);
                }
                else
                {
                    if (_printCnt2-- > 0) 
                    {
                        //static char strPre[32] = "{Wer";
                        //static char strPost[] = "}";
                        //DEBUG_PRINT1(strPre, usbRecordQueueLevel, strPost);
                        SYS_PRINT("{WErr%d(%x)}",
                            usbRecordQueueLevel,(uint8_t)writeEventData->status);
                    }
                    _APP_SetUSBWriteBufferReady(writeEventData->handle, 0);
                }

            } //End case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
            break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR:
            {
                setupReqRecipient = ((USB_SETUP_PACKET*)pData)->Recipient;

                if (setupReqRecipient == USB_SETUP_REQUEST_RECIPIENT_INTERFACE)
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
                                pData)->endpointNumber == APP_ENDPOINT_MICROPHONE)
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
                                pData)->endpointNumber == APP_ENDPOINT_HEADPHONE)
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
                        endpointNumber = ((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*) pData)->endpointNumber;

                        if (endpointNumber == APP_ENDPOINT_MICROPHONE)
                        {
                           //A control write transfer received from Host. 
                           //Now receive data from Host.
                           USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                                  (void *)&(appData.sampleFreqMic), 
                                                  3);
                           //SYS_PRINT("_USB: ControlGetCUR MP Fs=%d \r\n",
                           //        appData.sampleFreqMic);
                        }
                        
                        else if (endpointNumber == APP_ENDPOINT_HEADPHONE)
                        {
                            //A control write transfer received from Host. Now receive data from Host.
                            USB_DEVICE_ControlSend(
                                   appData.usbDevHandle, 
                                   (void *)&(appData.sampleFreq), 3 );
                            //SYS_PRINT("_USB: ControlGetCUR HP Fs=%d \r\n",
                            //       appData.sampleFreq);
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
                        appData.usbReadBufSize                  = 192;
                        appData.usbWriteBufSize                 = 96;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_32000)
                    {
                        appData.codecClientWriteRead.bufferSize = 128;
                        appData.usbReadBufSize                  = 128;
                        appData.usbWriteBufSize                 = 64;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_24000)
                    {
                        appData.codecClientWriteRead.bufferSize = 96;
                        appData.usbReadBufSize                  = 96;
                        appData.usbWriteBufSize                 = 48;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_16000)
                    {
                        appData.codecClientWriteRead.bufferSize = 64;
                        appData.usbReadBufSize                  = 64;
                        appData.usbWriteBufSize                 = 32;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_8000)
                    {
                        appData.codecClientWriteRead.bufferSize = 32;
                        appData.usbReadBufSize                  = 32;
                        appData.usbWriteBufSize                 = 16;
                    }
                    
                    //SYS_PRINT("_USB: HP Set Fs=%d (buffer=%d)\r\n",
                    //        appData.sampleFreq,
                    //        appData.codecClientWriteRead.bufferSize);
                    //NOTE:  Change Sampling Frequency then Reinit Playback Queue
                    //appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    
                    //NOTE:  G55 Audio USB Headset does not allow Sample Rate Change
                    //appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                    //hpInterfaceChanged = 1;
                    //appData.state = APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP; //RTOS Version
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
                        appData.usbReadBufSize                  = 192;
                        appData.usbWriteBufSize                 = 96;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_32000)
                    {
                        appData.codecClientWriteRead.bufferSize = 128;
                        appData.usbReadBufSize                  = 128;
                        appData.usbWriteBufSize                 = 64;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_24000)
                    {
                        appData.codecClientWriteRead.bufferSize = 96;
                        appData.usbReadBufSize                  = 96;
                        appData.usbWriteBufSize                 = 48;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_16000)
                    {      
                        appData.codecClientWriteRead.bufferSize = 64;
                        appData.usbReadBufSize                  = 64;
                        appData.usbWriteBufSize                 = 32;
                    }
                    else if (appData.sampleFreqMic == SAMPLING_RATE_8000)
                    {
                        appData.codecClientWriteRead.bufferSize = 32;
                        appData.usbReadBufSize                  = 32;
                        appData.usbWriteBufSize                 = 16;
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
                    //mpInterfaceChanged = 1;
                    //appData.state = APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                }
            } 
            break;

            case  USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

            default:
                SYS_ASSERT ( false , "Invalid callback" );
                //SYS_PRINT("USB Audio: Invalid Callback\r\n");
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
    //Disable interrupts - critical section
    #if defined(__arm__)
    //See the following page:
    //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
    __disable_irq(); // Set PRIMASK
    #elif defined(__mips__)
    interruptStatus = __builtin_disable_interrupts(); 
    #endif
        {
            char __attribute__((unused)) *initMessage = 
                "\r\nApplication created " __DATE__ " " __TIME__ " initialized!\r\n";
            //SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, initMessage, 
            //        strlen(initMessage));
            //SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, initMessage, 
            //        strlen(initMessage));
            SYS_MESSAGE(initMessage); 
        }
        SYS_MESSAGE("- Starting: USB Headset \r\n");

        //Device Layer Handle  
        appData.usbDevHandle = -1;

        //USB Audio Instance index for this app object 0
        appData.audioInstance = 0;

         //app state
        appData.state = APP_STATE_INIT;

        //Device configured state */
        appData.isConfigured        = false;
        appData.sofEventHasOccurred = false;

        //Initialize active interface setting to 0.
        appData.activeInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE;
        
        appData.activeMicInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE;

        // DAC is not muted initially 
        appData.dacMute = false,        //USB Mute command
        appData.ledState = LED_OFF;
#ifndef ONE_LED_VERSION
        appData.led2State = LED_OFF;
#endif
        appData.blinkDelay = 0;
        appData.muteEn = false;

        appData.playbackActive = false;
        appData.recordActive   = false;

        appData.autoGain = true;
        appData.dacMute = false;
        appData.dacVolume = 0x00; 
        appData.dacVolumeMin = 0x00; 
        appData.dacVolumeMax = 0x00FF;
        appData.dacVolumeRes = 0x0002;
        appData.mixerControl = 0x0000;

        //<STUB> - change to 48000 Hz after downsampling to AEC
        //NOTE:  The record and playback sample rates must be the same to
        //       match the codec sampling rate.
        //USB Sampling Frequency
        appData.sampleFreq    = DRV_CODEC_AUDIO_SAMPLING_RATE;
        appData.sampleFreqMic = DRV_CODEC_AUDIO_SAMPLING_RATE;

        appData.usbReadBufSize              = APP_MAX_PLAYBACK_BUFFER_BYTES;
        appData.usbWriteBufSize             = APP_MAX_RECORD_BUFFER_BYTES;

        appData.currentAudioControl = APP_USB_CONTROL_NONE;

        appData.playbackActive = false;
        appData.recordActive   = false;

        //Initialize Mic Settings
        appData.micGainIndex = 3;
        appData.micGain      = DRV_CODEC_MIC_GAIN;

        appData.codecClientWriteRead.context = (uintptr_t)&someVar;
        appData.codecClientWriteRead.bufferHandler = 
            (DRV_CODEC_BUFFER_EVENT_HANDLER) APP_CODECBufferEventHandlerWriteRead;
        appData.codecClientWriteRead.bufferSize = CODEC_MAX_WR_BUFFER_SIZE_BYTES;   //1ms @ 8000Hz 16bit-Stereo
    
        _APP_Init_RWBufferQueue();

#ifdef DEBUG_TEST_SIGNALS
        TEST1_Clear();  //USB RQ   or queueEmpty
        TEST2_Clear();  //USB RC   or queueFull
        TEST3_Clear();  //i2sc0 RQ or CLK+ 
        TEST4_Clear();  //i2sc0 RC or Queue Leve = Target
        TEST5_Clear();  // Queue Level > target
#endif //DEBUG_TEST_SIGNALS
    //Enable Interrupts
    #if defined(__arm__)
    __enable_irq(); // Clear PRIMASK
    #elif defined(__mips__)
    __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
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

#define NUMSTATEHIST 10
    static int __attribute__((unused)) stateHist[NUMSTATEHIST];
    static int __attribute__((unused)) stateHistIdx = 0;

    if (appData.lastState!=appData.state)
    {
        //New state
        stateHist[stateHistIdx++] = (int) appData.state;
        if (stateHistIdx >= NUMSTATEHIST) stateHistIdx=0;
        //char strSrc[6] = "";
        //itoa((int)appData.state,strSrc,10);
        //strcat(strSrc,";");        
        SYS_PRINT("%d;",appData.state);
        //SYS_CONSOLE_Message(0,stateStr[appData.state]);
    }
    appData.lastState = appData.state;

    //_APP_Button_Tasks();
    _APP_LED_Tasks();

    switch ( appData.state )
    {
        //======================================================================
        // Application's initial state. 
        //======================================================================
        case APP_STATE_INIT:
        {
            SYS_MESSAGE("x-INIT\r\n");
            tmrHandle = SYS_TIME_CallbackRegisterMS(_APP_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
                SYS_MESSAGE("x-nnnn\r\n");
                appData.state = APP_STATE_CODEC_OPEN;
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
#if 0
                    appData.muteEn   =false;
                    DRV_CODEC_MuteOff( appData.codecClientWriteRead.handle );
                    //DRV_CODEC_StereoMicSelect(appData.codecClientWriteRead.handle, 
                    //                          MIC1);  //G55 Audio X32 
                    DRV_CODEC_MicGainSet(appData.codecClientWriteRead.handle, 
                                         appData.micGain);

                    //Playback Volume
                    DRV_CODEC_VolumeSet(appData.codecClientWriteRead.handle, 
                                        DRV_CODEC_CHANNEL_LEFT_RIGHT, 200);
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
#endif //0
                }
                else
                {
                    SYS_DEBUG(0, "ERROR - Can't open CODEC\r\n");
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
            //Disable interrupts - critical section
            #if defined(__arm__)
            //See the following page:
            //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
            __disable_irq(); // Set PRIMASK
            #elif defined(__mips__)
            interruptStatus = __builtin_disable_interrupts(); 
            #endif

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

            //DRV_CODEC_StereoMicSelect(appData.codecClientWriteRead.handle, MIC1);
            DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle,
                                      appData.sampleFreq);
            appData.codecConfigured = true;
            
            appData.state = APP_STATE_USB_DEVICE_OPEN;


            //Enable Interrupts
            #if defined(__arm__)
            __enable_irq(); // Clear PRIMASK
            #elif defined(__mips__)
            __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
            #endif
        }
        break;
                       
        //======================================================================
        // Open USB Device Client
        // --Then APP_STATE_WAIT_FOR_CONFIGURATION
        //======================================================================
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

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                }
                else
                {
                //The Device Layer is not ready to be opened. 
                //--We should try again later.
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
                appData.ledState = LED_ON;
                SYS_MESSAGE("USB Configured\r\n");
                appData.state = APP_STATE_IDLE;
                _APP_Init_RWBufferQueue();
            }
        }
        break;

        //======================================================================
        // Wait for Speaker Active
        //======================================================================
        case APP_STATE_IDLE:
        {
#ifndef ONE_LED_VERSION
            appData.led2State = LED_OFF;
#endif
            /* Check if the device was configured */
            if(appData.isConfigured == false)
            {
                appData.playbackActive = false;
                appData.recordActive   = false;
                appData.ledState = LED_CONNECTED_BLINK;
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }

            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE) &&
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_NONE))
            {
                appData.ledState = LED_MUTE_BLINK;
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
            /* Check if the device was configured */
            if(appData.isConfigured == false)
            {
                /* If the device is configured then lets start reading */
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                break;
            }
#ifndef ONE_LED_VERSION
            appData.led2State = LED_CONNECTED_BLINK;
#endif

            _printCnt1 = 10;
            _printCnt2 = 10;

            ////Clear all codec status
            //_APP_Init_QueueCodecStatus();

            if ((appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_STEREO) || 
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
                appData.ledState = LED_ON; //Streaming

                SYS_MESSAGE("\r\n[QS]");
                //SYS_MESSAGE(paStr[appData.playbackActive?1:0]);
                //SYS_MESSAGE(maStr[appData.recordActive?1:0]);
                SYS_MESSAGE("\r\n");

                //DISABLE interrupts - Prevent USB RW Completes
                #if defined(__arm__)
                //See the following page:
                //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
                __disable_irq(); // Set PRIMASK
                #elif defined(__mips__)
                interruptStatus = __builtin_disable_interrupts(); 
#endif    
            
                    //NOTE:  Start from the head index, queue all buffers 
                    //       that have not complete USB xfers
                    //       --Codec_in_use buffers have been cleared.
                    //       --Completed USB buffers have been cleared.
                    //       --Queue all completed and unused buffers,i.e usb_in_use
                    //         is false for these buffers.
                    //       headIdx -> codec buffer completing or 
                    //                  next buffer to queue
                    errState = false;
                    for (i = 0;i < APP_QUEUE_SIZE;i++)
                    {
                        //USB Read to Head of i2sc0 Playback Buffer
                        headIdx = appWRQueue.headIdx; //Queue HEAD
                        currentQueueH     = &(appWRQueue.buffer[headIdx]);
                        currentPlayback  = (DRV_I2S_DATA16 *)
                                          (currentQueueH->playbackbuffer);
                        //currentRecord only used by codec
                        currentAecBuffer = (int16_t *) (currentQueueH->aecbuffer);

                        currentQueueH->bufferIdx = i;

                        if ((currentQueueH != NULL) && 
                            (currentQueueH->codecInUse == false) && 
                            (currentQueueH->usbInUse   == false)) 
                        {
                            //Buffer is ready to queue
                            currentQueueH->usbReadComplete  = false;
                            currentQueueH->usbWriteComplete = false;

                            //Read Stereo Playback buffer

                            //PLAYBACK
                            if (appData.playbackActive == true)
            {
                                audioErrRead = USB_DEVICE_AUDIO_Read(
                                        USB_DEVICE_INDEX_0, 
                                        &currentQueueH->usbReadHandle, 
                                        1, //Interface # 
                                        currentPlayback,
                                        appData.usbReadBufSize); 

                                if (audioErrRead != USB_DEVICE_AUDIO_RESULT_OK)
                                {
                                    currentQueueH->usbInUse = false;
                                    if (audioErrRead==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                                    {
                                        SYS_MESSAGE("{QROv}");
                                    }
                                    else 
                                    {
                                        //static char strPre[32] = "{QRErr(";
                                        //static char strPost[] = ")}";
                                        //DEBUG_PRINT1(strPre, audioErrRead, strPost);
                                        SYS_PRINT("{QRErr%d}",audioErrRead);
                                    }
                                    errState = true;
                                    break;
            }
                                else
                                {
                                    currentQueueH->usbInUse = true;
                                    appWRQueue.usbReadQueueCnt++;
                                    usbPlaybackQueueLevel++;
                                
                                    //static char strPre[32] = "{UR";
                                    //static char strPost[] = "}";
                                    //DEBUG_PRINT1(strPre, 
                                    //             usbPlaybackQueueLevel, 
                                    //             strPost);
                                    SYS_PRINT("{UR%d}",usbPlaybackQueueLevel);
                                }
                            }
                            else
                            {
                                //Allow Record Only by making buffer available
                                //NOTE:  The buffer should be 0 or only playback muted
                                memset(currentPlayback, 0, 
                                       appData.usbReadBufSize);
                                currentQueueH->usbReadComplete = true;
                            }

                            //RECORD 
                            if (appData.recordActive == true)
                            {
                                //NOTE:  Initially write a ZERO buffer since MIC 
                                //       has not been accessed yet.
                                memset(currentAecBuffer, 0, 
                                       appData.usbWriteBufSize);

                                audioErrWrite = USB_DEVICE_AUDIO_Write(
                                        USB_DEVICE_INDEX_0, 
                                        &currentQueueH->usbWriteHandle, 
                                        2,  //Interface #
                                        currentAecBuffer,  //Zeros
                                        appData.usbWriteBufSize);

                                if (audioErrWrite != USB_DEVICE_AUDIO_RESULT_OK)
                                {
                                    //USB Write Failed
                                    //Write Mono Record Buffer - AEC
                                    if (audioErrWrite==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                                    {
                                        SYS_MESSAGE("{QWOv}");
                                        //static char strPre[32] = "{QWOV(h=";
                                        //static char strPost[] = ")}";
                                        //DEBUG_PRINT1(strPre, appWRQueue.headIdx, strPost);
                                    }
                                    else 
                                    {
                                        //static char strPre[32] = "{QWErr(";
                                        //static char strPost[] = ")}";
                                        //DEBUG_PRINT1(strPre, audioErrWrite, strPost);
                                        SYS_PRINT("{QWErr%d}",audioErrWrite);
                                    }
                                    errState = true;
                                    break;
                                }
                                else
                                {
                                    //USB Write Queued
                                    currentQueueH->usbInUse = true;
                                    appWRQueue.usbWriteQueueCnt++;
                                    usbRecordQueueLevel++;

                                    //static char strPre[32] = "{UW";
                                    //static char strPost[] = "}";
                                    //DEBUG_PRINT1(strPre, usbRecordQueueLevel, strPost);
                                    SYS_PRINT("{UW%d}",usbRecordQueueLevel);
                                }
                            } //End recordActive 
                            else
                            {
                                //Allow Playback Only
                                currentQueueH->usbWriteComplete = true;
                            }

                            appWRQueue.headIdx = _APP_GetNextIdx(appWRQueue.headIdx);

                        } //End Buffer Available Check
                        else
                        {
                            //Buffer Queue Not Ready!!!
                            //char strPre[32] = "[NO BUFFER(";
                            //char strPost[] = ")";
                            //DEBUG_PRINT1(strPre, i, strPost);
                            SYS_PRINT("USB RW FAIL b%d\r\n",i);
                            currentQueueH->usbInUse = false;
                            appData.state = APP_STATE_IDLE;  //Wait for interface
                            break;
                        }
                    } //End USB RW Queing Loop

                    //Check for Error
                    if (errState == false)
                    {
                        _first = true;
                        appWRQueue.usbInitialBufsReady = false;  //Actually USB Init Q Read Complete
                        appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS;
                    }
                    else
                    {
                        //USB NOT Ready
                        if (audioErrWrite == 
                            USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                        {
                            //Queue Overflow - Continue
                            SYS_MESSAGE("\r\nQWov\r\n"); 
                            appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS; 
                        }
                        else if (audioErrWrite != USB_DEVICE_AUDIO_RESULT_OK)
                        {
                            //Queue Error - Idle
                            //static char strPre[32] = "\r\nQWErr(";
                            //static char strPost[] = ")";
                            //DEBUG_PRINT1(strPre, audioErrWrite, strPost);
                            SYS_PRINT("\r\nQWErr(%d)\r\n",audioErrWrite);
                            appData.state = APP_STATE_IDLE;
                        }  
                        
                        if (audioErrRead== 
                            USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                        {
                            //Queue Error - Idle
                            SYS_MESSAGE("\r\nQROv\r\n"); 
                            appData.state = APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS; 
                        }
                        else if (audioErrRead != USB_DEVICE_AUDIO_RESULT_OK)
                        {
                            //Queue Error - Idle
                            //static char strPre[32] = "\r\nQRErr(";
                            //static char strPost[] = ")";
                            //DEBUG_PRINT1(strPre, audioErrRead, strPost);
                            SYS_PRINT("\r\nQWErr(%d)\r\n",audioErrRead);
                            appData.state = APP_STATE_IDLE;
                        } 
                                           
                    }

                //ENABLE Interrupts - Allow USB RW Completes
                #if defined(__arm__)
                __enable_irq(); // Clear PRIMASK
                #elif defined(__mips__)
                __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
                #endif

            }//End Alternate Settings Playback/Record
            else
            {
                //Wait for the USB playback/record interface
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
                    APP_USB_SPEAKER_PLAYBACK_STEREO) ||
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_MONO))
            {
                if (appWRQueue.usbInitialBufsReady == true)
                {

                    static bool errState = false;
                    errState = false;
                    for (i = 0;
                         i < APP_QUEUE_SIZE/QUEUE_USB_INIT_PART; 
                         i++)
                    {
                        //int8_t headIdx = appWRQueue.headIdx; //Queue HEAD
                        tailIdx  = appWRQueue.tailIdx;   //Queue TAIL
                        currentQueueT    = &(appWRQueue.buffer[tailIdx]);
                        currentPlayback = (DRV_I2S_DATA16 *)
                                              (currentQueueT->playbackbuffer);
                        currentRecord   = (DRV_I2S_DATA16 *)
                                              (currentQueueT->recordbuffer);
                        currentQueueT->bufferIdx = tailIdx;

                        //Initiate write and read
                        if ((currentQueueT->usbReadComplete==true) && 
                            (currentQueueT->usbWriteComplete==true) && 
                            (currentQueueT->codecInUse == false))
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
                                    &(currentQueueT->codecWRHandle),
#ifdef DEBUG_TONE_CODEC
                                    sinBuffer,
#else
                                    currentPlayback,
#endif
                                    currentRecord, 
                                    appData.codecClientWriteRead.bufferSize); 

                            if (currentQueueT->codecWRHandle != 
                                DRV_CODEC_BUFFER_HANDLE_INVALID)
                            {
                                //++TAIL
                                currentQueueT->codecInUse       = true;
                                currentQueueT->usbReadComplete  = false;
                                currentQueueT->usbWriteComplete = false;
     
                                appWRQueue.codecWRQueueCnt++;

                                //SYS_PRINT("CQ%d %02x)\r\n",
                                //          i,currentQueueT->codecWRHandle);
     
                                //TAIL++  
                                appWRQueue.tailIdx = _APP_GetNextIdx(tailIdx);
                            }
                            else
                            {
                                currentQueueT->codecInUse = false;
                                errState                 = true;
                                break;
                            }
                        }
                        else
                        {

                            SYS_MESSAGE("[EUB-C]");
                            errNum   = ERR_USB_BUFFER;
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
                        appWRQueue.usbInitialBufsReady = false;
                        appData.state = APP_STATE_PROCESS_DATA;
                        
                    }
                    else
                    {
                        errNum = ERR_CODEC_BUFFER; 
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                } //End usbInitialBufsReady
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
        } //End case APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS
        break;


        //======================================================================
        // Process USB Read buffers to Codec Playback  and Codec read mic data to
        // USB Write buffers
        //
        // NOTE:  AEC is stubbed out
        //
        // Transition From:
        //    APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD
        //    --When the alt settings are not really changed from NONE

        //    APP_STATE_SUBMIT_INITIAL_CODEC_WRITEREAD_REQUEST:
        //    --When both alt setting is APP_USB_SPEAKER_PLAYBACK_STEREO
        //       && appWRQueue.usbInitialBufsReady
        // Transition To:
        //    APP_STATE_IDLE - When both interfaces are NONE
        //    APP_STATE_ERR - On error
        //======================================================================
        case APP_STATE_PROCESS_DATA:
        {
            if ((appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_STEREO) ||
                (appData.activeMicInterfaceAlternateSetting == 
                     APP_USB_MIC_RECORD_MONO))
            {
                tailIdx  = appWRQueue.tailIdx;  //Queue TAIL
                currentQueueT    = &(appWRQueue.buffer[tailIdx]);

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
                                      (currentQueueT->playbackbuffer);
#endif
                currentRecord   = (DRV_I2S_DATA16 *) 
                                      (currentQueueT->recordbuffer);
                currentQueueT->bufferIdx = tailIdx;

                //------------------------------------------------------------------ 
                //CODEC WR after USB RW Complete
                //--Playback USB Read Buffer
                //--Get new Codec Read Buffer (with echo)
                if ((currentQueueT->usbWriteComplete == true)  &&
                    (currentQueueT->usbReadComplete  == true)  &&
                    (currentQueueT->codecInUse       == false) &&
                    (currentQueueT->codecWRComplete  == false) &&
                    (currentQueueT->codecAecComplete == false))
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
                            &currentQueueT->codecWRHandle,
#ifdef DEBUG_TONE_CODEC
                                    sinBuffer,
#else
                                    currentPlayback,
#endif
                                    currentRecord, 
                                    appData.codecClientWriteRead.bufferSize);

                    if (currentQueueT->codecWRHandle != 
                        DRV_CODEC_BUFFER_HANDLE_INVALID)
                    {
                        
                        currentQueueT->codecInUse       = true;
                        //currentQueueT->usbReadComplete  = false;
                        //currentQueueT->usbWriteComplete = false;
                        currentQueueT->codecWRComplete  = false;
                        currentQueueT->codecAecComplete = false;

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
                        errNum = ERR_CODEC_BUFFER;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                } //End Codec WRITE/READ Bufffer Ready

                //--------------------------------------------------------------
                // USB Read and Write Request to HEAD
                headIdx = appWRQueue.headIdx;  //Queue HEAD
                //tailIdx  = appWRQueue.tailIdx;  //Queue TAIL
                currentQueueH    = &appWRQueue.buffer[headIdx];
                currentPlayback = (DRV_I2S_DATA16 *) 
                                  (currentQueueH->playbackbuffer);
                currentRecord   = (DRV_I2S_DATA16 *) 
                                  (currentQueueH->recordbuffer);
                currentAecBuffer  = (int16_t *) currentQueueH->aecbuffer;

                //Check Playback/Record QUEUE Buffers Ready 
                //--Playback and Record data buffers ready
                if ( (currentPlayback != NULL) && 
                     (currentRecord   != NULL) && 
                     (currentQueueH->codecWRComplete == true) )
                {

                    //USB Read (Playback stream) 
                    //NOTE:  Buffer size always at the maximum value (1ms @ 48000Hz)
#ifdef DEBUG_BUFFER_TIMING
#ifdef __CORE_CM7_H_GENERIC
                    //USB WR Toggle
                    TEST1_Toggle();
#endif //__CORE_CM7_H_GENERIC
#endif
                    if (appData.recordActive == true)
                    {
#undef DEBUG_USB_LOOPBACK
#ifdef DEBUG_USB_LOOPBACK
                        //BYPASS THE CODEC - LOOPBACK USB READ to USB WRITE
                        //--------------------------
                        //--Discard empty channel and convert to mono buffer
                        //--Acoustic Echo Cancellation (if enabled)
                        //Stereo Mic audio Input to Mono 
                        _aecSumStereoChannels(
                                        (DRV_I2S_DATA16 *) currentPlayback, 
                                        yInQ1d15,
                                        //48);
                                        appData.usbReadBufSize/
                                                APP_INT16_STEREO_SAMPLE_BYTES);
#else
                        //CODEC/Codec LOOPBACK The USB READ through the I2SC
                        //Convert Stereo Record Data to Mono
                        _aecSumStereoChannels(
                                        (DRV_I2S_DATA16 *) currentRecord, 
                                        yInQ1d15,
                                        //48); //# of stereo samples
                                        appData.usbReadBufSize/
                                                APP_INT16_STEREO_SAMPLE_BYTES);
#endif //DEBUG_USB_LOOPBACK

                        //NO ECHO CANCELLER
                        //TODO:  Acoustic Echo Cancelling. 
                        //       Pass-Thru echo for now
                        //Stereo Mic audio to Mono USB
                        //memcpy(currentAecBuffer, yInQ1d15, 96);
                        memcpy(currentAecBuffer, yInQ1d15, 
                               appData.usbWriteBufSize);

                        audioErrWrite = USB_DEVICE_AUDIO_Write(
                                            USB_DEVICE_INDEX_0, 
                                            &currentQueueH->usbWriteHandle, 
                                            2, currentAecBuffer,
                                            appData.usbWriteBufSize); //96
                    } //End recordActive block
                    else
                    {
                        //Allow Playback read without Record write
                        audioErrWrite = USB_DEVICE_AUDIO_RESULT_OK;
                    }

                    //USB Read - Playback
                    if (appData.playbackActive == true)
                    {
                        //USB Read to current codec playback buffer
                        audioErrRead  = USB_DEVICE_AUDIO_Read(
                                            USB_DEVICE_INDEX_0, 
                                            &currentQueueH->usbReadHandle, 
                                            1, currentPlayback,  
                                            appData.usbReadBufSize);
                    }
                    else
                    {
                        audioErrRead = USB_DEVICE_AUDIO_RESULT_OK;
                    }

                    if ((audioErrRead == USB_DEVICE_AUDIO_RESULT_OK) &&
                        (audioErrWrite== USB_DEVICE_AUDIO_RESULT_OK))
                    {
                        //Valid USB W or R Operations
                        currentQueueH->usbInUse         = true;
                        if (appData.playbackActive == true)
                        {
                            currentQueueH->usbReadComplete  = false;
                            appWRQueue.usbReadQueueCnt++; 
                            usbPlaybackQueueLevel++;
                            if (_printCnt1-- > 0) 
                            {
                                //static char strPre[32] = "{UR";
                                //static char strPost[] = "}";
                                //DEBUG_PRINT1(strPre, usbPlaybackQueueLevel, strPost);
                                SYS_PRINT("{UR%d}",usbPlaybackQueueLevel);
                            }
                        }
                        else
                        {
                            //Allow Record w/o Playback
                            currentQueueH->usbReadComplete  = true;
                        }
                        if (appData.recordActive == true)
                        {
                            currentQueueH->usbWriteComplete  = false;
                            appWRQueue.usbWriteQueueCnt++; 
                            usbRecordQueueLevel++;
                            
                            if (_printCnt1-- > 0) 
                            {
                                //static char strPre[32] = "{UW";
                                //static char strPost[] = "}";
                                //DEBUG_PRINT1(strPre, usbRecordQueueLevel, strPost);
                                SYS_PRINT("{UW%d}",usbRecordQueueLevel);
                            }
                        }
                        else
                        {
                            //Allow Playback w/o Record 
                            currentQueueH->usbWriteComplete  = true;
                        }
                        currentQueueH->codecInUse       = false;                
                        currentQueueH->codecWRComplete  = false;
                        currentQueueH->codecAecComplete = false;

                        //++HEAD (Next USB RW)

                        if ((appWRQueue.usbWriteQueueCnt) % REPORTINTVL == 0)
                        {
                            if (_first==true) {SYS_MESSAGE("\r\n"); _first=false;}
                            //static char strPre[32] = "UWQ ";
                            //static char strPost[] = "}\r\n";
                            //DEBUG_PRINT1(strPre, 
                            //             appWRQueue.usbWriteQueueCnt,
                            //             strPost);
                            SYS_PRINT("UWQ %d\r\n",(uint16_t)appWRQueue.usbWriteQueueCnt);
                        }
                        if ((appWRQueue.usbReadQueueCnt+1) % REPORTINTVL == 0)
                        {
                            if (_first==true) {SYS_MESSAGE("\r\n"); _first=false;}
                            //static char strPre[32] = "URQ ";
                            //static char strPost[] = "}\r\n";
                            //DEBUG_PRINT1(strPre, 
                            //             appWRQueue.usbReadQueueCnt,
                            //             strPost);
                            SYS_PRINT("URQ %d.",(uint16_t)appWRQueue.usbReadQueueCnt);
                        }

                        //Next Buffer to WR
                        appWRQueue.headIdx = _APP_GetNextIdx(appWRQueue.headIdx);
                    }
                    else
                    {
                        //ERROR
                        if (audioErrRead==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                        {
                            SYS_MESSAGE("{QROv}");
                        }
                        else 
                        {
                            //static char strPre[32] = "QRerr(";
                            //static char strPost[] = ")";
                            //DEBUG_PRINT1(strPre, audioErrRead, strPost);
                            SYS_PRINT("\r\n{QRErr(%d)}\r\n",audioErrRead); 
                        }

                        if (audioErrWrite==USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL)
                        {
                            SYS_MESSAGE("{QWOv}");
                        }
                        else 
                        {
                            //static char strPre[32] = "QWerr(";
                            //static char strPost[] = ")";
                            //DEBUG_PRINT1(strPre, audioErrWrite, strPost);
                            SYS_PRINT("\r\n{QWErr(%d)}\r\n",audioErrWrite);
                        }
                        //currentQueueH->usbInUse = false;
                        errNum = 3;
                        //appData.state = APP_STATE_ERROR;
                        //break;
                    }
                } //End codecAecComplete (USB Read/Write)
            } //End Record and Playback Alternate Setting
            else
            {
                //Not Ready for Playback/Record
                appData.state = APP_STATE_IDLE;
            }
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
                    appData.recordActive   = false;
                    appData.playbackActive = false;
                    //SYS_PRINT("APP: Mute Control ON\r\n");
                    appData.ledState = LED_MUTE_BLINK; 
                    appData.muteEn = true;
                    SYS_MESSAGE("[MOn]\r\n");
                    DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);

                    appData.state = APP_STATE_IDLE;
                    break;
                }

                if (appData.activeInterfaceAlternateSetting ==
                       APP_USB_SPEAKER_PLAYBACK_STEREO) 
                {
                    SYS_MESSAGE("[MOff]\r\n");
                    appData.muteEn = true;
                    DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                    //DRV_CODEC_MicGainSet(appData.codecClientWriteRead.handle, 
                    //             appData.micGain);
                    appData.playbackActive = true;
                    if (appData.activeMicInterfaceAlternateSetting == 
                       APP_USB_MIC_RECORD_MONO)
                    {
                        appData.recordActive   = true;
                    }
                }
                else
                {
                    appData.recordActive = false;
                }

                if (appData.activeMicInterfaceAlternateSetting == 
                         APP_USB_MIC_RECORD_MONO)
                {
                    appData.recordActive = true;
                    if (appData.activeInterfaceAlternateSetting == 
                       APP_USB_SPEAKER_PLAYBACK_STEREO)
                    {
                        appData.playbackActive   = true;
                    }
                }
                else
                {
                    appData.recordActive = false;
                }

                appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
        }
        break;

        //======================================================================
        //======================================================================
        case APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP:
        {
            //Changes sampling rate for Record and Playback (both stereo)
            SYS_MESSAGE("[HPFs]\r\n");
                    
            //NOTE:  Codec Master Mode only 
            //DRV_CODEC_SamplingRateSet(appData.codecClientWriteRead.handle, 
            //                                  appData.sampleFreq);

            hpInterfaceChanged = 1;
            appData.state = APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
        }
        break;


        //======================================================================
        //======================================================================
        case APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP:
        {
            //Changes sampling rate for Record and Playback (both stereo)
            SYS_MESSAGE("[MPFs]\r\n");

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
            CRLF();
#if 0
            //Disable interrupts - critical section
            #if defined(__arm__)
           //See the following page:
            //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
            __disable_irq(); // Set PRIMASK
            #elif defined(__mips__)
            interruptStatus = __builtin_disable_interrupts(); 
            #endif
#endif //0

            //SYS_PRINT("APP: ALt Setting RCVD: HP=%d MP=%d\r\n",
            //        hpInterfaceChanged, mpInterfaceChanged);
            _printCnt1 = 20;
            _printCnt2 = 20;
            _initQ = true;

            //Check for No Audio
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE)  &&
                (appData.activeMicInterfaceAlternateSetting == 
                    APP_USB_MIC_RECORD_NONE))
            {
                //No USB Audio
                appData.playbackActive = false;
                appData.recordActive   = false;
                usbPlaybackQueueLevel  = 0;
                usbRecordQueueLevel    = 0;
                usbPlaybackRestart     = false;
                usbRecordRestart       = false;

                hpInterfaceChanged     = false;
                mpInterfaceChanged     = false;
      
                if (appData.playbackActive == false && 
                    appData.recordActive == false)
                {
                    SYS_MESSAGE("[pima]");
                appData.ledState = LED_MUTE_BLINK; 
                    SYS_MESSAGE("[pimi]\r\n");
                    _APP_Init_RWBufferQueue();
                    appData.ledState = LED_MUTE_BLINK; //Streaming
                    appData.state = APP_STATE_IDLE;
                    break;
                }
            } //End !MP || !HP

            //Playback - Headphone interface ready
            //--playbackActive
            if (hpInterfaceChanged && 
                (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO))
            {

                //Interface changed:  PA/pi or Fs
                hpInterfaceChanged     = false;
                usbPlaybackQueueLevel  = 0;

                //Check if already active interface
                if ((appData.playbackActive == true)) //&&
                //    (usbPlaybackQueueLevel > 0))
                {
                    usbPlaybackRestart = true;   //let RQ go to 0 restart
                }
                else
                {
                    appData.playbackActive = true;
                    usbPlaybackRestart = false;   //let RQ go to 0 restart
                }

                SYS_STATUS codecStatus;
                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                        
                if (SYS_STATUS_READY == codecStatus)
                {
                    if (DRV_CODEC_ClientReady(0) == true)
                    {
                        SYS_MESSAGE("[MOff]\r\n");
                        DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                        //DRV_CODEC_MicGainSet(appData.codecClientWriteRead.handle, 
                        //             appData.micGain);
                        //Check if already active other interface
                        if ((appData.recordActive == false))
                        {
                            usbRecordQueueLevel = 0;
                            SYS_MESSAGE("[PAmi]\r\n");
                            appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
                        }
                        else
                        {
                            SYS_MESSAGE("[PAMA]\r\n");
                            usbRecordRestart = true;
                            appData.state = APP_STATE_USB_QUEUE_CLEAR;
                        }
                    }
                }
                else
                {
                    SYS_MESSAGE("APP:  LOOPBACK NOT Ready!\r\n");                   
                    errNum = ERR_CODEC;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            } //End HP USB Interface Alt Setting
            else if (hpInterfaceChanged == true)
            {
                hpInterfaceChanged     = false;
                appData.playbackActive = false;
                usbPlaybackQueueLevel    = 0;

                if   (appData.recordActive == true) 
                {
                    //Continue the Queing of completed USB Buffers
                    //--Record queue must clear before queueing
                    SYS_MESSAGE("[piMA]\r\n");
                    usbRecordRestart     = true;
                    //usbRecordRestart     = false;
                    //usbRecordQueueLevel  = 0;
                    appData.state = APP_STATE_USB_QUEUE_CLEAR;
                }
                else  
                {
                    //Reinit Q - no W/R Q clear
                    SYS_MESSAGE("[pimi]\r\n");
                    usbRecordRestart    = false;
                    usbRecordQueueLevel = 0;
                    //_APP_Init_RWBufferQueue();
                    appData.state = APP_STATE_IDLE;
                }
                //break;
            } //End of playbackActive block

            //Record - Mic Interface 
            //-- recordActive
            if ((mpInterfaceChanged == true) && 
                (appData.activeMicInterfaceAlternateSetting == 
                        APP_USB_MIC_RECORD_MONO))
            {
                mpInterfaceChanged     = false;
                appData.recordActive = true;
                usbRecordRestart  = false;
                usbRecordQueueLevel    = 0;
                appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;

                SYS_STATUS codecStatus;
                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
                    if (DRV_CODEC_ClientReady(0) == true)
                    {
                        //SYS_PRINT("APP: LOOPBACK Ready - MP Interface ON\r\n");
                        SYS_MESSAGE("[MOff]\r\n");
                        DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                        //DRV_CODEC_MicGainSet(appData.codecClientWriteRead.handle, 
                        //             appData.micGain);
                        //Check if other interface is already active
                        if (appData.playbackActive == false)
                        {
                            SYS_MESSAGE("[piMA]\r\n");
                            usbPlaybackRestart    = false;
                            usbPlaybackQueueLevel = 0;
                            appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
                        }
                        else
                        {
                            SYS_MESSAGE("[PAMA]\r\n");
                            usbPlaybackRestart = true;
                            appData.state = APP_STATE_USB_QUEUE_CLEAR;
                        }
                        //break;
                    }
                }
                else
                {
                    appData.state = APP_STATE_IDLE;
                    break;
                }
            } //End Mic USB Interface Alt Setting
            else if (mpInterfaceChanged == true) 
            {
                mpInterfaceChanged   = false;
                appData.recordActive = false;  
                usbRecordQueueLevel  = 0;

                if (appData.playbackActive == true) 
                {
                    SYS_MESSAGE("[PAmi]\r\n");
                    usbPlaybackRestart       = true;
                    appData.state = APP_STATE_USB_QUEUE_CLEAR;
                }
                else 
                {
                    SYS_MESSAGE("[pimi]\r\n");
                    //_APP_Init_RWBufferQueue();
                    appData.state = APP_STATE_IDLE;
                }
                //break;
            } //End recordActive block

#if 0
            //Enable Interrupts
            #if defined(__arm__)
            __enable_irq(); // Clear PRIMASK
            #elif defined(__mips__)
            __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
            #endif
#endif //0
        }
        break;

        //======================================================================
        //======================================================================
        case APP_STATE_USB_QUEUE_CLEAR:
        {
            static int irpQCalc   = 0;  //Calculated count 
            static int irpQActual = 0;  //Actual count 
            static int __attribute__((unused)) irpQCnt = 0;

            //Disable interrupts - critical section
            #if defined(__arm__)
            //See the following page:
            //https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html
            __disable_irq(); // Set PRIMASK
            #elif defined(__mips__)
            interruptStatus = __builtin_disable_interrupts(); 
            #endif

                irpQActual = IrpQueueCount(); 
                if (_initQ) 
                {
                    //NOTE: This just reports and check on what the actual
                    //      and the counted queue level are. 
                    //static char strPre1[] = "\r\nIRP(";
                    //static char strPost1[]  = ")";
                    //DEBUG_PRINT1(strPre1, irpQCnt, strPost1);
                    SYS_PRINT("\r\nIRP(%d)",irpQCnt);


                    //static char strPre[]   = "Q[P";
                    //static char strMid[] = "R";
                    //static char strPost[] = "]";
                    //DEBUG_PRINT2(strPre, usbPlaybackQueueLevel, 
                    //             strMid, usbRecordQueueLevel, strPost);
                    SYS_PRINT("\r\nQ[P%dR%d]",
                            usbPlaybackQueueLevel,usbRecordQueueLevel);

                    irpQCalc =  usbPlaybackQueueLevel + usbRecordQueueLevel;
                    if (irpQCalc != irpQActual) 
                    {
                        SYS_MESSAGE("\r\n!!QCOUNT ERROR!!\r\n");
                    }
                    _initQ = false;
                }

#define QUEUE_CLEAR_LEVEL  0
                if (irpQActual <= QUEUE_CLEAR_LEVEL)
                {
                    //Restart Audio Stream - with the active interfaces
                    usbPlaybackQueueLevel = 0;
                    usbRecordQueueLevel   = 0;
                    usbPlaybackRestart = false;
                    usbRecordRestart   = false;
                    SYS_MESSAGE("[Q00]\r\n");
                    _APP_Init_RWBufferQueue();
                    _initQ = true;  //Allow print
                    appData.state = APP_STATE_INITIAL_USB_READ_REQUESTS;
                }

            //Enable Interrupts
            #if defined(__arm__)
            __enable_irq(); // Clear PRIMASK
            #elif defined(__mips__)
            __builtin_mtc0(12,0,interruptStatus); //Enable Interrupts
            #endif
        }
        break;

        //======================================================================
        //======================================================================
        case APP_STATE_ERROR:
        {
            /* The default state should never be executed. */
            static int printErr = 1;
            appData.ledState = LED_ERROR_BLINK;
            if (printErr == 1)
            {
                printErr = 0;
                //SYS_PRINT("APP ERROR:  %s", ERROR_STR[errNum]);
                //SYS_DEBUG(0, "APP ERROR:  HALT!!!");
            }
        }
        break;

        //======================================================================
        //======================================================================
        default:
        {
            appData.ledState = LED_ERROR_BLINK;
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

        //LED1
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

#ifndef ONE_LED_VERSION
        //LED2
        switch(appData.led2State)
        {
            //Connection State
            case LED_ON:
                LED2_On();
            break;

            case LED_OFF:
                LED2_Off();
            break;

            case LED_CONNECTED_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED2_Toggle();
                    appData.blinkDelay = 1000;  
    }
            break;

            case LED_MUTE_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED2_Toggle();
                    appData.blinkDelay = 500;  
}
            break;

            case LED_ERROR_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED2_Toggle();
                    appData.blinkDelay = 250;  
                }
            break;

            default:
                LED2_Off();

        } //End switch(state)
#endif
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
                    SYS_MESSAGE("[MOn]\r\n");
                    DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
                }
                else
                {
                    appData.ledState = LED_ON;
                    appData.muteEn = false;
                    SYS_MESSAGE("[MOff]\r\n");
                    DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
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
                //    SYS_MESSAGE("[MOff]\r\n");
                //    DRV_CODEC_MuteOff(appData.codecClientWriteRead.handle);
                //}
                //else
                //{
                //    appData.ledState = MUTE_BLINK;
                //    appData.muteEn   =true;
                //    SYS_MESSAGE("[MOn]\r\n");
                //    DRV_CODEC_MuteOn(appData.codecClientWriteRead.handle);
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
                SYS_MESSAGE("[Q0]\r\n");
                appWRQueue.usbCompleteBufferLevel = 0;
                queueEmpty = true;
            }
        }
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
            errNum = ERR_CODEC_EVENT;
            appData.state = APP_STATE_ERROR;
        } 
        break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
            errNum = ERR_CODEC_EVENT;
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
static int _APP_SetUSBReadBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle,
                                      uint16_t frameLengthBytes)
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
                appWRQueue.buffer[i].frameLengthBytes = frameLengthBytes;
                appWRQueue.usbCompleteBufferLevel++;
            }
            return 1;
            //break;
        }
    }
    return 0;
}
//******************************************************************************
// _APP_SetUSBWriteBufferReady()
//Set the USB Write Buffer Data Ready 
//******************************************************************************
static int _APP_SetUSBWriteBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle,
                                       uint16_t frameLengthBytes)
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
                appWRQueue.buffer[i].usbInUse         = false;
                appWRQueue.buffer[i].frameLengthBytes = frameLengthBytes;
                appWRQueue.usbCompleteBufferLevel++;
            }
            return 1;
            //break;
        }
    }
    return 0;
}

//******************************************************************************
// _APP_USBReadAllBufferReady()
//    Check if USB Read Buffer Queue is Ready for CODEC Writes 
//******************************************************************************
static bool _APP_USBInitAllBufferReady()
{
    int i = 0;

    //Check to see if either Initial Read or Write Data transfers completed
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

    //setupNextInProcess = false;

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
    }
    appWRQueue.usbCompleteBufferLevel = 0;
    appWRQueue.codecWRQueueCnt        = 0;
    appWRQueue.codecWRCompleteCnt     = 0;
    appWRQueue.usbReadQueueCnt        = 0;
    appWRQueue.usbWriteQueueCnt       = 0;
    appWRQueue.usbReadCompleteCnt     = 0;
    appWRQueue.usbWriteCompleteCnt    = 0;

    appWRQueue.usbInitialBufsReady = false;


    queueFull  = false;
    queueEmpty = true;

    usbPlaybackQueueLevel    = 0;
    usbRecordQueueLevel      = 0;

    if (DRV_CODEC_ClientReady(0))
    {
        DRV_CODEC_ReadQueuePurge(appData.codecClientWriteRead.handle);
        DRV_CODEC_WriteQueuePurge(appData.codecClientWriteRead.handle);
    }
} //End _APP_Init_RWBufferQueue()


//******************************************************************************
//_APP_GetNextIdx()
//Increment the Head or Tail Index to Codec Playback Buffer Queue
//******************************************************************************
static int16_t _APP_GetNextIdx(int16_t index)
{
    int16_t newIndex; 
    newIndex = (index+1)%(APP_QUEUE_SIZE);
    return newIndex;
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


#if 0
//******************************************************************************
// _APP_Seti2sc0ReadComplete()
//Set the i2sc0 Read Buffer Complete 
//******************************************************************************
static void _APP_SetAecBufferReady(int idx)
{
    appWRQueue.buffer[idx].codecAecComplete = true;
}
#endif //0

#if 0
//******************************************************************************
//_APP_DupTheEmptyChannelFromMicrophone
//Create Full Stereo Buffer from One Channel stereo buffer (one empty channel)
//******************************************************************************
static void _APP_DupTheEmptyChannelFromMicrophone(DRV_I2S_DATA16 *input, 
                                                  DRV_I2S_DATA16 *output, 
                                                  int numSamples)
{
    int i;
    int j = 0;

    for(i = 0;i<numSamples; i+=2)
    {
        // AK4642 daughter card microphone has only one valid channel (AK4953 too).
        // but stereo microphone mode is set for synchronizing playback and 
        // microphone. 
        // Only one channel has valid data, the other one is zero, but we don't 
        // know which one is the 
        // "REAL" left channel and right channel, by adding this two together, 
        // we can get 
        // that one channel data, then send this output buffer to USB.
        output[j].leftData =  input[i].leftData + input[i].rightData;
        output[j].rightData = input[i+1].leftData + input[i+1].rightData;
        j++;
    }
    
}
#endif

#if 0
//******************************************************************************
// _APP_DiscardTheEmptyChannelFromMicrophone2()
// 
// Create Mono from Stereo Channel buffer 
//******************************************************************************
static void _APP_DiscardTheEmptyChannelFromMicrophone2(DRV_I2S_DATA16 *input, 
                                                      DRV_I2S_DATA16 *output, 
                                                      int numSamples)
{
    int i;
    int j = 0;

    for(i = 0;i<numSamples; i+=2)
    {
        // AK4642 daughter card microphone has only one valid channel 
        // (AK4953 too) but stereo microphone mode is set for synchronizing 
        // playback and microphone. 
        // Only one channel has valid data, the other one is zero, but we don't 
        // know which one is the "REAL" left channel and right channel, 
        // by adding this two together, we can get that one channel data, 
        // then send this output buffer to USB.
        output[j].leftData =  input[i].leftData + input[i].rightData;
        output[j].rightData = input[i+1].leftData + input[i+1].rightData;
        j++;
    }
    
}
#endif

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
        //Add with saturation
        output[i] = (q15) libq_q15_Add_q15_q15(input[i].leftData,
                                               input[i].rightData);
    }
}

void CRLF() { SYS_MESSAGE("\r\n"); }

int IrpQueueCount()
{
    int count=0;
    int i;

    for (i=0; i<USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED; i++)
    {
        if (gUSBDeviceAudioIRP[i].status > 
            USB_DEVICE_IRP_STATUS_COMPLETED) count++;
    }
    return count;
}

/*******************************************************************************
 End of File
 */
