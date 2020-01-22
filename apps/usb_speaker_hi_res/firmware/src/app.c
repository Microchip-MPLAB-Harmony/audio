/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c for H3 usb_speaker

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
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


//*****************************************************************************
//*****************************************************************************
// Section: Included Files
//*****************************************************************************
//*****************************************************************************

#include "app.h"
#include "atsame70q21b.h"
#include "component/pmc.h"

//#include "display.h"

//DEBUG 
#undef DEBUG_TONE_CODEC_TX
#undef DEBUG_CLOCK_TUNING
#define DEBUG_BUFFER_TIMING

//=============================================================================
//CLOCK CONTROL and TUNING
//=============================================================================
static uint32_t __attribute__((unused)) dpl, mpl, dg, dp;

#define CLOCK_MISMATCH_COMPENSATION
#ifdef CLOCK_MISMATCH_COMPENSATION
#define CKUPDATERED 2   //Clock Update Delay 
static int ckUpdateCnt = CKUPDATERED; 
static const int CLOCK_STABLE_DELAY_MS = 100;

#ifdef APP_CODEC_MASTER
static const int APP_CLKDEL = 200;
#else  //APP_CODEC_MASTER
#undef FS48KHZ
#ifdef FS48KHZ
static uint32_t _clockAdjUp[9][3] = 
{
    {1, 41, 40}, //492000000, 12300000,  48046,  0.096%, 
    {2, 41, 20}, //246000000, 12300000,  48046,  0.096%, 
    {3, 40, 13}, //160000000, 12307692,  48076,  0.158%, 
    {1, 40, 39}, //480000000, 12307692,  48076,  0.158%, 
    {1, 39, 38}, //468000000, 12315789,  48108,  0.225%, 
    {2, 39, 19}, //234000000, 12315789,  48108,  0.225%, 
    {1, 38, 37}, //456000000, 12324324,  48141,  0.294%, 
    {1, 37, 36}, //444000000, 12333333,  48177,  0.369%, 
    {2, 37, 18}, //222000000, 12333333,  48177,  0.369%, 
};

static uint32_t _clockAdjDwn[8][3] = 
{
    {3, 43, 14}, //172000000, 12285714,  47991, -0.019%, 
    {2, 43, 21}, //258000000, 12285714,  47991, -0.019%, 
    {2, 45, 22}, //270000000, 12272727,  47940, -0.125%, 
    {3, 46, 15}, //184000000, 12266666,  47916, -0.175%, 
    {2, 47, 23}, //282000000, 12260869,  47894, -0.221%, 
    {2, 49, 24}, //294000000, 12250000,  47851, -0.310%, 
    {3, 49, 16}, //196000000, 12250000,  47851, -0.310%, 
    {2, 51, 25}, //306000000, 12240000,  47812, -0.392%, 
};
#endif //FS48KHZ

//d, m, and d2 values for E70 96Khz/64 bit stereo frame
static uint32_t d,m,d2;
#define CLOCKUP_IND   0   
#define CLOCKDWN_IND  0
static uint32_t _clockAdjUp[3][3] = 
{
{2,41,10}, //246000000,24600000,96093,0.097%,
{1,39,19}, //468000000,24631578,96217,0.226%,
{2,37,9},  //222000000,24666666,96354,0.369%,
};

static uint32_t _clockAdjDwn[3][3] = 
{
{3,43,7},  //172000000,24571428,95982,-0.019%,
{2,45,11}, //270000000,24545454,95880,-0.125%,
{2,49,12}, //294000000,24500000,95703,-0.309%,
};
#endif //APP_CODEC_MASTER
#endif //CLOCK_MISMATCH_COMPENSATION 

bool queueFull;
bool queueEmpty;

//24 to 32 bit Unpacking 
uint32_t sa;
uint32_t sb;
uint32_t sc;

uint16_t volumeLevels[VOLUME_STEPS] =
{
    0 /* off */, 64, 128, 255
};

//NOTE: Cache Coherency, 16 byte alignment and N*16 length buffers 
//      required for MZ processor,
//      -->as the cache line for PIC32MZ is 16 bytes.
//      16 byte alignment for E70 processor (dma transfers are 16 bit)
//
//      You don't want to run into an issue where linker allocates the data 
//      structure in the same page as another data structure and then a line 
//      flush causes coherency issues in  other structures.
//USB Tx Buffer (Ping-Pong)
//DRV_I2S_DATA16   __attribute__((aligned(32))) __attribute__((tcm))
//96Khz Test Data
//Test buffers (sine tone)
//-->96X4 = 2*192  bytes
//NOTE:  DRV_I2S_DATA24 is equivalent to DRV_I2S_DATA32
DRV_I2S_DATA32 __attribute__((aligned(32)))
        testBuffer2[96] =
{
#include "tone_1000Hz_32bit_96Khz_p125.txt"
#include "packs/ATSAME70Q21B_DFP/atsame70q21b.h" 
};

//Pack/Unpack 24bit USB Data to 32 bit word.
//ARM Packed byte structure (unaligned)
#pragma pack(push, 1)  //Current pragma state is 1 byte packed
typedef struct _UInt24  { uint8_t byte[3]; } UInt24;
typedef struct  _BITS24 { int8_t bytes[3]; } BITS24;
#pragma pack(pop) //Return to the non packed pragma state.

int32_t _Convert24to32bit(BITS24 val);
void _Unpack24to32bitBuffer(uint8_t *src, uint8_t*dst, int numUsb24BitSamples);
inline uint32_t unpack(UInt24 x);
inline UInt24 pack(uint32_t x);



APP_PLAYBACK_BUFFER* currentPlayBuffer;
int8_t codecWriteIdx; 
//uint32_t *src; 
UInt24   *src;
uint32_t *dst;

//Button Timer
DRV_HANDLE tmrHandle;

//uint32_t timerStart, timerEnd;
//uint32_t cycles = 0;
volatile bool usbInitialReadsComplete = false;

//NOTE: APP_PLAYBACK BUFFER is what is transferred using DMA--> it is padded
// and placed at the beginning of the struct and the struct attribute is the
// allocated with the COHERENT and aligned(16) attributes so that it is 
// placed at the correct page boundary.
//static __attribute__((aligned(32))) __attribute__((tcm))
#ifdef APP_ALLOCATE_TCM
static APP_PLAYBACK_BUFFER_QUEUE 
       __attribute__((aligned(32))) __attribute__((tcm))
           appBufferQueue;
#else
static APP_PLAYBACK_BUFFER_QUEUE __attribute__((aligned(32)))
           appBufferQueue;
#endif

//==============================================================================
// Application Playback Buffer Queue
//==============================================================================
static int     _APP_SetUSBReadBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle);
static void    _APP_Init_PlaybackBufferQueue(void);
static int     _APP_ClearCodecReturnBuffer(DRV_CODEC_BUFFER_HANDLE handle);
static uint8_t _APP_GetNextIdx(uint8_t index);
static bool    _APP_USBReadAllBufferReady(void);
//static bool   _APP_CodecBufferInUse(void);

void _APP_Button_Tasks(void);
void _APP_LED_Tasks(void);
static void _APP_TimerCallback( uintptr_t context);

static bool volatile hpInterfaceChanged = false;
static USB_DEVICE_AUDIO_RESULT volatile audioErr1;

//Application Class Data
//APP_DATA __attribute__((tcm)) appData =
#ifdef APP_ALLOCATE_TCM
APP_DATA __attribute__((aligned(32))) appData =
#else
APP_DATA __attribute__((aligned(32))) __attribute__((tcm)) appData =
#endif //APP_ALLOCATE_TCM
{
    /* Device Layer Handle  */
    .usbDevHandle = -1,

    /* USB Audio Instance index for this app object 0*/
    .audioInstance = 0,

     /* app state */
    .state = APP_STATE_CODEC_OPEN,

    /* device configured status */
    .isConfigured = false,

    .codecConfigured = false,

    /* Initialize active interface setting to 0. */
    .activeInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE,
    
    /* DAC is not muted initially */
    .dacMute = false,        //USB Mute command
    .ledState = LED_OFF,
    .blinkDelay = 0,
    .muteEn = false,
    
    .sampleFreq = 0x17700,    //96000 Hz
    
    .usbReadBufSize = APP_MAX_NO_BYTES_IN_USB_BUFFER, //96000Hz 1ms (96*6) = 576  
    //CODEC Driver Write only Client
    .codecClientWrite.context = (uintptr_t)0,
    .codecClientWrite.bufferHandler = (DRV_CODEC_BUFFER_EVENT_HANDLER) 
                                          APP_CODECBufferEventHandler,
    .codecClientWrite.bufferSize = APP_MAX_NO_BYTES_IN_CODEC_BUFFER, // 96*8 = 768 
    
    /* No Audio control in progress.*/
    .currentAudioControl = APP_USB_CONTROL_NONE,

    .volumeIndex = 1,
    .volume = 255,

    .lrSync = true,

#ifdef CLOCK_MISMATCH_COMPENSATION
    .clockStableDelayFlag = true,
#else
    .clockStableDelayFlag = false,
#endif
}; //End appData initialization


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

    switch( event )
    {
        case USB_DEVICE_EVENT_RESET:
            SYS_PRINT("USB: Device RESET\r\n");
            break;

        case USB_DEVICE_EVENT_DECONFIGURED:
            // USB device is reset or device is de-configured.
            // This means that USB device layer is about to de-initialize
            // all function drivers. So close handles to previously opened
            // function drivers.

            // Also turn ON LEDs to indicate reset/de-configured state.
            /* Switch on red and orange, switch off green */
            SYS_PRINT("USB: DeConfigured\r\n");

            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            LED1_Off();
            /* check the configuration */
            configuredEventData = 
                     (USB_DEVICE_EVENT_DATA_CONFIGURED *)pEventData;
            if(configuredEventData->configurationValue == 1)
            {
                /* the device is in configured state */
                /* Switch on green and switch off red and orange */
                USB_DEVICE_AUDIO_EventHandlerSet(0,
                                                 APP_USBDeviceAudioEventHandler ,
                                                 (uintptr_t)NULL);

                /* mark that set configuration is complete */
                appData.isConfigured = true;
                appData.ledState = CONNECTED_BLINK;
                SYS_PRINT("USB:CONFIGURED\r\n");
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            /* Switch on green and orange, switch off red */
            appData.ledState = LED_OFF;
            SYS_PRINT("USB: Suspended\r\n");
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            SYS_PRINT("USB: ATTACHED\r\n");
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS was removed. Notify USB stack about the event*/
            USB_DEVICE_Detach (appData.usbDevHandle);
            SYS_PRINT("USB: DETACH\r\n");
            appData.ledState = LED_OFF;
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
} //End APP_USBDeviceEventHandler()


//******************************************************************************
// APP_USBDeviceAudioEventHandler()
//******************************************************************************
//void __attribute__((tcm)) 
//     APP_USBDeviceAudioEventHandler(USB_DEVICE_AUDIO_INDEX iAudio,
void APP_USBDeviceAudioEventHandler(USB_DEVICE_AUDIO_INDEX iAudio,
                               USB_DEVICE_AUDIO_EVENT event ,
                               void * pData,
                               uintptr_t context)
{
    volatile USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED *interfaceInfo;
    volatile USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *readEventData;

    uint8_t entityID;
    uint8_t controlSelector;

    if ( iAudio == 0 )
    {
        switch (event)
        {
            case USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED:
            {
                /* We have received a request from USB host to change the Interface-
                   Alternate setting.*/
                interfaceInfo = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED *)
                   pData;

                //if(interfaceInfo->interfaceNumber == APP_PLAYBACK_INTERFACE)
                //{
                //    if(appData.activeInterfaceAlternateSetting != interfaceInfo->interfaceAlternateSetting){
                        hpInterfaceChanged = true;
                        SYS_PRINT("USB: HP Changed to %d(%d)\r\n",
                                interfaceInfo->interfaceAlternateSetting,
                                interfaceInfo->interfaceNumber);
                        appData.activeInterfaceAlternateSetting = 
                                interfaceInfo->interfaceAlternateSetting;
                        appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                //    }
                //    
                //}
                /* We have received a request from USB host to change the Interface-
                   Alternate setting.*/
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
            {
                TEST2_Toggle();
                //We have received an audio frame from the Host.
                //Now send this audio frame to Audio Codec for Playback.
                readEventData = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *)pData;

                //Make USB Read Buffer Queue Ready for Data (Clear data flags)
                int __attribute__((unused)) usbBufferIdx = 
                    _APP_SetUSBReadBufferReady(readEventData->handle);

                appBufferQueue.usbReadCompleteBufferLevel++;
                appBufferQueue.usbReadCompleteCnt++;
                appBufferQueue.usbReadQueueCnt--;  //Not completed level

                queueEmpty = false;

                //Check if the this is the initial loading of the playback
                //queue with USB Rx Data 
                if(appData.state == APP_STATE_INITIAL_CODEC_WRITE_REQUEST)
                {
                    if(_APP_USBReadAllBufferReady())
                    {
                        usbInitialReadsComplete = true;
                        appBufferQueue.previousBufferLevel = 
                                appBufferQueue.usbReadCompleteBufferLevel;
                        SYS_PRINT(
                            "USB: INIT READS:  Qlevel %d - Ridx %d - Widx %d\r\n",
                            appBufferQueue.usbReadCompleteBufferLevel,
                            appBufferQueue.usbReadIdx,
                            appBufferQueue.codecWriteIdx);
//                        SYS_PRINT("USB INIT READ COMPLETEs: Qlevel %d RQCnt %d WQCnt %d\r\n",
//                            appBufferQueue.usbReadCompleteBufferLevel,
//                            appBufferQueue.usbReadQueueCnt,
//                            appBufferQueue.codecWriteQueueCnt);
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
            {
            }
            break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR:
            {
                //if(((USB_SETUP_PACKET*)pData)->Recipient == 
                //     USB_SETUP_REQUEST_RECIPIENT_INTERFACE)
                //{
                    entityID = 
                       ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)pData)->entityID;
                    if ((entityID == APP_ID_FEATURE_UNIT))
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
                           appData.currentAudioControl = APP_USB_AUDIO_MUTE_CONTROL;
                       }
                       
                    }
                    //else if (entityID == APP_ID_MIXER_UNIT)
                    //{
                    //    USB_DEVICE_ControlStatus(appData.usbDevHandle, 
                    //                             USB_DEVICE_CONTROL_STATUS_ERROR);
                    //}
                //}
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_CUR:
            {
                entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)
                           pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT)
                {
                   controlSelector = ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)pData)->controlSelector;

                   if (controlSelector == USB_AUDIO_MUTE_CONTROL)
                   {
                       /*Handle Get request*/
                       USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                              (void *)&(appData.dacMute), 
                                              1);
                   }
                }
                //if (((USB_SETUP_PACKET*)pData)->Recipient == 
                //     USB_SETUP_REQUEST_RECIPIENT_INTERFACE)
                //{
                //    if ((entityID == APP_ID_FEATURE_UNIT) || 
                //         (entityID == APP_ID_FEATURE_UNIT_SIDE_TONING))
                //    {
                //       controlSelector = 
                //              ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                //                            pData)->controlSelector;
                //       if (controlSelector == USB_AUDIO_MUTE_CONTROL)
                //       {
                //           /*Handle Get request*/
                //           USB_DEVICE_ControlSend(appData.usbDevHandle, 
                //                                 (void *)&(appData.dacMute), 
                //                                 1);
                //       }
                //    }
                    
                //    else if (entityID == APP_ID_MIXER_UNIT)
                //    {
                //        USB_DEVICE_ControlStatus (appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
                //    }
                //}
                //else if (((USB_SETUP_PACKET*)pData)->Recipient == 
                //           USB_SETUP_REQUEST_RECIPIENT_ENDPOINT)
                //{
                //    controlSelector = ((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                //                         pData)->controlSelector;
                //    if (controlSelector == USB_AUDIO_SAMPLING_FREQ_CONTROL)
                //    {
                //        if (((USB_AUDIO_ENDPOINT_CONTROL_REQUEST*)
                //                       pData)->endpointNumber == HEADPHONE_EP)
                //        {
                //           //A control write transfer received from Host. Now receive data from Host.
                //           USB_DEVICE_ControlSend(appData.usbDevHandle, (void *)&(appData.sampleFreq), 3 );
                //        }
                //    }
                //}
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
                USB_DEVICE_ControlStatus (appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
            

            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            {
                USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK );

                if (appData.currentAudioControl == APP_USB_AUDIO_MUTE_CONTROL)
                {
                    appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                    //Handle Mute Control Here.
                }
                
                //Needed for multiple frequency playback settings.
                //NOTE:  Maximum buffer size is 48 samples (192 bytes)
                if (appData.currentAudioControl == APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP)
                {
                    if (appData.sampleFreq == SAMPLING_RATE_48000)
                    {
                        appData.codecClientWrite.bufferSize = 192;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_32000)
                    {
                        appData.codecClientWrite.bufferSize = 128;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_24000)
                    {
                        appData.codecClientWrite.bufferSize = 96;
                    }
                    else if (appData.sampleFreq == SAMPLING_RATE_16000)
                    {
                        appData.codecClientWrite.bufferSize = 64;
                    }
                    //appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                  
                    //NOTE:  Change Sampling Frequency then Reinit Playback Queue
                    //appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    appData.state = APP_SAMPLING_FREQUENCY_CHANGE; //RTOS Version
                }
            }
            break;

            case  USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

            default:
                SYS_ASSERT ( false , "Invalid callback" );
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
      SYS_PRINT("----------------------------------------\r\n");
      SYS_PRINT("- Starting:\r\n");
      SYS_PRINT("----------------------------------------\r\n");

    appData.state = APP_STATE_INIT;
    appData.ledState = LED_OFF; 
    
    appData.volumeIndex = 1;
    appData.volume = volumeLevels[appData.volumeIndex];    
    
    appData.buttonDelay = 0;
    appData.ledState= LED_OFF;
    appData.blinkDelay = 0;

    appData.lrSync = true;

#if defined(DEBUG_CLOCK_TUNING) || defined(DEBUG_BUFFER_TIMING)
    TEST1_Clear();
    TEST2_Clear();
    TEST3_Clear();
    TEST4_Clear();
#endif

#if defined(USE_DISPLAY)
    display_init(&DISPLAY_STATS);
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
    
    switch(appData.state)
    {
        //---------------------------------------------------------------------
        // Configure CODEC
        // Transition from: APP_STATE_WAIT_FOR_CONFIGURATION
        // Wait until: SYS_STATUS_READY 
        // Transition To: APP_STATE_CODEC_SET_BUFFER_HANDLER
        //---------------------------------------------------------------------
        case APP_STATE_INIT:
        {
            // open the timer Driver
            tmrHandle = SYS_TIME_CallbackRegisterMS(_APP_TimerCallback, 
                    (uintptr_t)0, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID != tmrHandle )
            {
               appData.state = APP_STATE_CODEC_OPEN;
            }            
        }
        break;

        case APP_STATE_CODEC_OPEN:
        {
            SYS_STATUS codecStatus;
            codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);

            if (SYS_STATUS_READY == codecStatus)
            {
                // This means the driver can now be be opened.
                /* A client opens the driver object to get an Handle */
                appData.codecClientWrite.handle = 
                        DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                                       DRV_IO_INTENT_WRITE);
                       
                if (appData.codecClientWrite.handle != DRV_HANDLE_INVALID) 
                {

                    //Initial Volume
                    DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                        DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                        appData.volume);
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                    SYS_PRINT("APP: CODEC Open - volume %d\r\n",
                            appData.volume);
                }
                else
                {
                    SYS_PRINT(0, "Find out whats wrong \r\n");
                    appData.state = APP_STATE_ERROR;
                }
            }
            else
            {
                /* Wait for CODEC to Initialize */
                ;
            }
        }
        break;
        
        //---------------------------------------------------------------------
        // Set a handler for the audio buffer completion event 
        // --Then APP_STATE_USB_DEVICE_OPEN
        //---------------------------------------------------------------------
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {
            DRV_CODEC_BufferEventHandlerSet(
                        appData.codecClientWrite.handle,
                        appData.codecClientWrite.bufferHandler,
                        appData.codecClientWrite.context);

            //Default Value -- Never changes for usb_speaker.
            if (DRV_CODEC_I2S_MASTER_MODE == true)
            {
#if defined(APP_CODEC_MASTER)
                //CODEC is Master
                DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                                          appData.sampleFreq);
                SYS_PRINT("APP: CODEC Master Buffer Handler Fs = %d Hz",
                        appData.sampleFreq);
#else
#ifdef CLOCK_MISMATCH_COMPENSATION
                //CODEC is Slave
                static DRV_WM8904_CLIENT_OBJ *clientObj;
                static DRV_WM8904_OBJ *drvObj;
                static DRV_HANDLE * handle;

                handle = (DRV_HANDLE *) appData.codecClientWrite.handle;
                clientObj = (DRV_WM8904_CLIENT_OBJ *) handle;
                drvObj = (DRV_WM8904_OBJ *) clientObj->hDriver;
                //Set the PLLA/GCLK/PCK2
                static uint32_t plla, gclk, pck2;
                plla = PMC_REGS->CKGR_PLLAR;
                PMC_REGS->PMC_PCR =  PMC_PCR_PID(70);
                gclk = PMC_REGS->PMC_PCR;
                pck2  = PMC_REGS->PMC_PCK[2];

                //Register Values
                dpl = (plla & 0xff); 
                mpl = (plla>>16 & 0x3ff) + 1; 
                dg  = (gclk>>20 & 0xff) + 1; 
                dp  = (pck2>>4 & 0xff) + 1; 
                SYS_PRINT("I: PLLA %08x\r\n",plla);
                SYS_PRINT("I: GCLK %08x\r\n",gclk);
                SYS_PRINT("I: PCK  %08x\r\n",pck2);
                SYS_PRINT("I: d= %x m= %x d2 = %x(%x)\r\n",dpl,mpl,dg,dp);

                //Clock Initialize
                const int d  = _clockAdjUp[CLOCKUP_IND][0];
                const int m  = _clockAdjUp[CLOCKUP_IND][1];
                const int d2 = _clockAdjUp[CLOCKUP_IND][2];
                DRV_I2S_ClockGenerationSet(drvObj->i2sDriverHandle, d, m, d2);
                DRV_I2S_ProgrammableClockSet(drvObj->i2sDriverHandle, 2, d2);

#ifdef DEBUG_CLOCK_TUNING
                TEST1_Clear();
                TEST2_Clear();
#endif //DEBUG_CLOCK_TUNING

                plla = PMC_REGS->CKGR_PLLAR;
                PMC_REGS->PMC_PCR =  PMC_PCR_PID(70);
                gclk = PMC_REGS->PMC_PCR;
                pck2  = PMC_REGS->PMC_PCK[2];
                dpl = (plla & 0xff); 
                mpl = (plla>>16 & 0x3ff) + 1; 
                dg  = (gclk>>20 & 0xff) + 1; 
                dp  = (pck2>>4 & 0xff) + 1; 
                SYS_PRINT("PLLA %08x\r\n",plla);
                SYS_PRINT("GCLK %08x\r\n",gclk);
                SYS_PRINT("PCK  %08x\r\n",pck2);
                SYS_PRINT("d= %x m= %x d2 = %x(%x)\r\n",dpl,mpl,dg,dp);
                SYS_PRINT("APP: Codec Slave Buffer Handler Fs = %d Hz",
                          appData.sampleFreq);
#endif //CLOCK_MISMATCH_COMPENSATION
#endif //APP_CODEC_MASTER
            }
            else
            {
                SYS_PRINT("APP: Codec Slave Buffer Handler Fs = ?? Hz");
                //TODO:  Change I2SC1 slave sample rate
            }

            appData.codecConfigured = true;
            appData.state = APP_STATE_USB_DEVICE_OPEN;
        }
        break;

        //---------------------------------------------------------------------
        // Open USB Device Client
        // --Then APP_STATE_WAIT_FOR_CONFIGURATION
        //---------------------------------------------------------------------
        case APP_STATE_USB_DEVICE_OPEN:
        {    
            _APP_Init_PlaybackBufferQueue();

            queueFull = false;
            queueEmpty = true;
            LED1_Off();

            /* Open the device layer */

            //Open USB for Read/Write //WRITE??? only
            appData.usbDevHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0,
                                                   DRV_IO_INTENT_READWRITE); 

            if (appData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get 
                 * event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.usbDevHandle, 
                                           APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }
        }
        //Fall Through

        //---------------------------------------------------------------------
        // Wait for USB Connection to be configured
        // --then APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE
        //---------------------------------------------------------------------
        case APP_STATE_WAIT_FOR_CONFIGURATION:
        {
            //Check if Host has configured the Device.
            if (appData.isConfigured == true)
            {
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE;
            }
        }
        break;

        //---------------------------------------------------------------------
        // Wait for the USB Audio Stream to start
        // (NEW FOR H3)
        // --then APP_STATE_INITIAL_USB_READ_REQUEST
        //---------------------------------------------------------------------
        case APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE:
        {
            if (appData.activeInterfaceAlternateSetting == 
                APP_USB_SPEAKER_PLAYBACK_STEREO)
            {
                //Clear the frame counts
                appData.ledState = CONNECTED_BLINK;
                appData.state = APP_STATE_INITIAL_USB_READ_REQUEST; 
            }
        }
        break; 

        //---------------------------------------------------------------------
        // Initial USB Read Request
        //
        // Transition From:
        //         APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE
        //         APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        //              --playback alternate setting
        //         APP_MUTE_AUDIO_PLAYBACK 
        //              --playback alternate setting
        //
        // Wait for alt playback 
        // -->Initiate USB Rx to all playback queue buffers
        //
        // Transition To: 
        //     APP_STATE_INITIAL_CODEC_WRITE_REQUEST
        //---------------------------------------------------------------------
        case APP_STATE_INITIAL_USB_READ_REQUEST:
        {

            if (appData.codecConfigured == false)
            {
                break;
            }
            else if (appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_STEREO)
            {

#ifdef CLOCK_MISMATCH_COMPENSATION
            //Start with decreased clock rate
            //--and let clock stabilize
            ckUpdateCnt = 0;  
            appData.clockStableDelayFlag = false;  //Clock Stable Delay not complete
            appData.clockStableDelayMs= CLOCK_STABLE_DELAY_MS;
#else  //CLOCK_MISMATCH_COMPENSATION
            appData.clockStableDelayFlag = true;  //No Delay
#endif //CLOCK_MISMATCH_COMPENSATION

                //Initialize/Purge playback queue
                _APP_Init_PlaybackBufferQueue();
                queueFull = false;
                queueEmpty = true;
                appData.ledState = LED_ON;
                appData.muteEn = false;
                DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                    DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                    appData.volume);

                //To check alignment
                //int value1 = sizeof(APP_PLAYBACK_BUFFER)%16; 

                //Fill the queue with USB Read
                //--Flag to wait for Initial USB Read Q to fill. 
                usbInitialReadsComplete = false;  

                for (i = 0; i < APP_QUEUING_DEPTH; i++)
                {
                    //USB Read to Head of Codec Playback Buffer
                    APP_PLAYBACK_BUFFER* usbReadBuffer = 
                            &appBufferQueue.playbackBuffer[i];

                    if (usbReadBuffer != NULL && 
                        !usbReadBuffer->codecInUse && 
                        !usbReadBuffer->usbInUse)
                    {

                        usbReadBuffer->usbReadCompleted = false;
                        usbReadBuffer->usbInUse = true;

                        //Read to Queue HEAD
                        TEST1_Toggle();
                        audioErr1 = USB_DEVICE_AUDIO_Read(
                                        USB_DEVICE_INDEX_0, 
                                        &usbReadBuffer->usbReadHandle, 
#ifdef APP_BSUBFRAMESIZE_3
                                        1, usbReadBuffer->buffer,  //24 bit buffer 
#else
                                        1, usbReadBuffer->buffer32, 
#endif //APP_BSUBFRAMESIZE_3
                                        appData.usbReadBufSize); //96 Samples

                        if(audioErr1 == USB_DEVICE_AUDIO_RESULT_OK)
                        {
                            //SYS_PRINT("***INIT USB READ: RQCnt %d", 
                            //    appBufferQueue.usbReadQueueCnt);
                            //Next Codec Write Index (HEAD Index)
                            appBufferQueue.usbReadIdx = _APP_GetNextIdx(appBufferQueue.usbReadIdx);
                            appBufferQueue.usbReadQueueCnt++;
                            
                        }
                        else
                        {
                            //ERROR - USB Read Queue Full and does not match the APP 
                            //read queue size--ERROR.
                            usbReadBuffer->usbInUse = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }

                    }
                } //End USB Audio Read Queue loop

                SYS_PRINT("***INIT USB READ Finished: RQCnt %d\r\n", 
                         appBufferQueue.usbReadQueueCnt);
                appData.state = APP_STATE_INITIAL_CODEC_WRITE_REQUEST;

            } //activeInterfaceAlternatedSetting 
            else
            {
                //_APP_Init_PlaybackBufferQueue();

                //queueFull = false;
                //queueEmpty = false;

                appData.ledState = CONNECTED_BLINK;
                appData.muteEn = true;
                DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
            }//No activeInterfaceAlternateSetting

        } //End APP_SUBMIT_INITIAL_USB_READ_REQUEST:
        break;


        //---------------------------------------------------------------------
        // Initial Codec Write Request
        //
        // Transition From:
        //     APP_STATE_INITIAL_USB_READ_REQUEST 
        //       -- initiates USB reads to playback queue buffers.
        //          and waits for a number of them to complete
        //     USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
        //       -- USB read completion
        //
        // Transition To: 
        //----------------------------------------------------------------------
        case APP_STATE_INITIAL_CODEC_WRITE_REQUEST:
        {    
            if (appData.activeInterfaceAlternateSetting != 
                    APP_USB_SPEAKER_PLAYBACK_STEREO)
            {
                _APP_Init_PlaybackBufferQueue();
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE; 
            }
            else if  (usbInitialReadsComplete == true)
            {
                //appData.ledState = LED_ON;
                //Set the volume
                DRV_CODEC_LRCLK_Sync(appData.codecClientWrite.handle);

                //Initialize the Codec Write Queue
                for (i = 0;
                     i < APP_INIT_QUEUE_LEVEL; i++)
                {

                    codecWriteIdx = appBufferQueue.codecWriteIdx;
                    currentPlayBuffer = 
                            &appBufferQueue.playbackBuffer[codecWriteIdx];


                    //CODEC Playback 
                    if (currentPlayBuffer->usbReadCompleted == true && 
                        currentPlayBuffer->usbInUse   == false &&
                        currentPlayBuffer->codecInUse == false)
                    {
                        //Initial CODEC Write
                        currentPlayBuffer->codecInUse = true;
                        
                        //L/R I2S Channel Sync 
                        if(appData.lrSync)
                        {
                            appData.lrSync = 
                                DRV_CODEC_LRCLK_Sync(appData.codecClientWrite.
                                                                 handle);
                            if (appData.lrSync == false)
                            {
                                appData.state = APP_STATE_ERROR;
                                break;  //FAIL
                            }
                            appData.lrSync = false;
                        }


                        #ifdef DEBUG_TONE_CODEC_TX
                        //TONE Data

                        //Write stereo tone to output instead of USB data.
                        DRV_CODEC_BufferAddWrite(appData.codecClientWrite.handle, 
                                                 &currentPlayBuffer->codecWriteHandle,
                                                 testBuffer2, sizeof(testBuffer2));
                        #else
                        //USB Data

                        #ifdef APP_BSUBFRAMESIZE_3
                        //Unpack the 24 bit buffer to the 32 bit buffer
                        // 4bytes x 4words = 12 bytes -> 4x24 bit samples
                        //int i;
                        int j;
                        int numUsb24BitSamples = appData.usbReadBufSize/3;

                        src = (UInt24 *) currentPlayBuffer->buffer;
                        dst = (uint32_t *) currentPlayBuffer->buffer32; 
                        for (j=0; j<numUsb24BitSamples; j++) 
                        {
                            dst[j] = unpack(src[j]);
                        }
#if 0
                        src = (uint32_t *) currentPlayBuffer->buffer;
                        dst = (uint32_t *) currentPlayBuffer->buffer32; 
                        for (i=0, j=0; j<numUsb24BitSamples; i+=3, j+=4) 
                        {
                            //Every 3 32 bit words converted to 4 32 bit samples
                            sa = src[i+0]; //LS
                            sb = src[i+1];
                            sc = src[i+2]; //MS

#define UPPER
#ifdef UPPER
                            //Q23 --> Q31
                            //Little ENDIAN - shifted right MS bytes first
                            dst[j+0] = sa & 0xFFFFFF00;
                            dst[j+1] = ((sa<<24) | (sb>>8 & 0x00FFFF00);
                            dst[j+2] = ((sb<<16) | (sc>>16 & 0xFFFFFF00);
                            dst[j+3] = sc<<8;
#else //LOWER
                            //Q23 --> Q31
                            //Little ENDIAN - MS bytes first
                            //dst[j+0] = sa<<8; 
                            dst[j+0] = sa>>8 & 0x00FFFFFF; 
                            //dst[j+1] = (sa>>16)) | ((sb<<16) & 0xFFFFFF00;
                            dst[j+1] = (sa<<16)) | (sb<<16 & 0xFFFF00);
                            //dst[j+2] = ((sb>>8) | (sc<<24)) & 0xFFFFFF00;
                            dst[j+2] = ((sb>>8) | (sc<<24)) & 0xFFFFFF00;
                            //dst[j+3] = sc & 0xFFFFFF00;
                            dst[j+3] = sc & 0xFFFFFF00;
#endif //UPPER?LOWER
           
                        }
#endif //0

                        //Initial CODEC Write
                        currentPlayBuffer->codecInUse = true;
                        #endif //APP_BSUBFRAMESIZE_3

                        //Write stereo tone to output instead of USB data.
                        TEST3_Toggle();
                        DRV_CODEC_BufferAddWrite(
                                    appData.codecClientWrite.handle, 
                                    &currentPlayBuffer->codecWriteHandle,
                                    currentPlayBuffer->buffer32,
                                    sizeof(currentPlayBuffer->buffer32)); 

                        #endif //DEBUG_TONE_CODEC_TX

                        if (currentPlayBuffer->codecWriteHandle != 
                            DRV_CODEC_BUFFER_HANDLE_INVALID)
                        {
                            appBufferQueue.codecWriteQueueCnt++;
                            appBufferQueue.codecWriteIdx = 
                                    _APP_GetNextIdx(codecWriteIdx);
                        }
                        else
                        {
                            currentPlayBuffer->codecInUse = false;
                            // CODEC doesn't have enough write buffers
                            // should never happen
                            appData.state = APP_STATE_ERROR;
                        }
                    } //Buffer Ready
                } //Queue Loop
                appData.state = APP_PROCESS_DATA;
            } //Wait for initial USB Reads
        } //End case APP_STATE_INITIAL_CODEC_WRITE_REQUEST
        break;

        //---------------------------------------------------------------------
        // Process USB Read buffers to Codec Playback  and Codec read mic data to
        // USB Write buffers
        //
        // Transition From:
        //    APP_STATE_INITIAL_CODEC_WRITE_REQUEST:
        //    --When usbInitialReadsComplete is true
        //---------------------------------------------------------------------
        case APP_PROCESS_DATA:
        {

            if (appData.activeInterfaceAlternateSetting == 
                APP_USB_SPEAKER_PLAYBACK_STEREO)
            {

#ifdef CLOCK_MISMATCH_COMPENSATION
                //TODO:  This should be part of the I2S API
                //       --For now it is in the APP
                //USB-CODEC Datarate Compensation
                //bufferLevel = abs(usbNextBufferRead - i2sNextBuffer);
                if (appBufferQueue.usbReadCompleteBufferLevel != 
                    appBufferQueue.previousBufferLevel)
                {
                    if (appBufferQueue.usbReadCompleteBufferLevel < APP_INIT_QUEUE_LEVEL+2)
                    {    
                        //Let the clock stabilize to a converged value by counting
                        //down the updates
                        //Each packet is 1 ms of data
                        if (ckUpdateCnt-- == 0)
                        {
#ifdef APP_CODEC_MASTER
                            DRV_CODEC_SamplingRateSet(
                                    appData.codecClientWrite.handle, 
                                    appData.sampleFreq+APP_CLKDEL);
#else
                            //USB is running slow. Decrease I2S clock.  
                            //- Prevent underflow (queueEmpty)
                            //Increase Clock Rate
                            d  = _clockAdjUp[CLOCKUP_IND][0];
                            m  = _clockAdjUp[CLOCKUP_IND][1];
                            d2 = _clockAdjUp[CLOCKUP_IND][2];
                            //d  = _clockAdjDwn[CLOCKDWN_IND][0];
                            //m  = _clockAdjDwn[CLOCKDWN_IND][1];
                            //d2 = _clockAdjDwn[CLOCKDWN_IND][2];
#ifdef DEBUG_CLOCK_TUNING
                            TEST1_Set();
                            TEST2_Clear();
#endif //DEBUG_CLOCK_TUNING
#endif //APP_CODEC_MASTER
                        }
                    }
                    else 
                    {
                        if (appBufferQueue.usbReadCompleteBufferLevel == APP_INIT_QUEUE_LEVEL+2)
                        {
                            // USB is at same rate as CODEC 
                            //Keep Current Clock Rate
                        }
                        else
                        {
#ifdef  APP_CODEC_MASTER
                            DRV_CODEC_SamplingRateSet(
                                    appData.codecClientWrite.handle, 
                                    appData.sampleFreq-APP_CLKDEL);
#else
                            // USB is running faster. 
                            //Decrease Clock Rate
                            //-prevent Codec data overflow
                            d  = _clockAdjDwn[CLOCKDWN_IND][0];
                            m  = _clockAdjDwn[CLOCKDWN_IND][1];
                            d2 = _clockAdjDwn[CLOCKDWN_IND][2];
#ifdef DEBUG_CLOCK_TUNING
                            TEST1_Clear();
                            TEST2_Set();
#endif //DEBUG_CLOCK_TUNING
#endif //APP_CODEC_MASTER
                        }
                    }
#ifdef APP_CODEC_MASTER
                    static DRV_WM8904_CLIENT_OBJ *clientObj;
                    static DRV_WM8904_OBJ *drvObj;
                    static DRV_HANDLE * handle;

                    handle = (DRV_HANDLE *) appData.codecClientWrite.handle;
                    clientObj = (DRV_WM8904_CLIENT_OBJ *) handle;
                    drvObj = (DRV_WM8904_OBJ *) clientObj->hDriver;
                    //Set the PLLA/GCLK/PCK2
                    DRV_I2S_ProgrammableClockSet(drvObj->i2sDriverHandle, 2, d2);
                    DRV_I2S_ClockGenerationSet(drvObj->i2sDriverHandle, d, m, d2);
#endif //APP_CODEC_MASTER

                    appBufferQueue.previousBufferLevel = 
                         appBufferQueue.usbReadCompleteBufferLevel; 
                } //End Clock Compensation
#endif //CLOCK_MISMATCH_COMPENSATION

                //----------------------------
                //USB read
                //--Read to usbReadIdx (Next Queue HEAD index)
                //--Returns NULL pointer if FULL
                APP_PLAYBACK_BUFFER * usbReadBuffer = 
                     &appBufferQueue.
                        playbackBuffer[appBufferQueue.usbReadIdx];

                if (appBufferQueue.usbReadCompleteBufferLevel == 
                    APP_QUEUING_DEPTH)
                {
                      queueFull = true;
//                      SYS_PRINT("****QUEUE Full****: RBLevel %d Ridx %d - Widx %d\r\n",
//                                  appBufferQueue.usbReadCompleteBufferLevel,
//                                  appBufferQueue.usbReadIdx,
//                                  appBufferQueue.codecWriteIdx);
//                    SYS_PRINT("*** QUEUE FULL****: Qlevel %d RQ %d WQ %d WC %d\r\n",
//                                appBufferQueue.usbReadCompleteBufferLevel,
//                                appBufferQueue.usbReadQueueCnt,
//                                appBufferQueue.codecWriteQueueCnt,
                }

                if ( usbReadBuffer != NULL && 
                     !usbReadBuffer->usbInUse && 
                     !usbReadBuffer->codecInUse)
                {
                    usbReadBuffer->usbReadCompleted = false;

                    TEST1_Toggle();
                    audioErr1 = USB_DEVICE_AUDIO_Read(USB_DEVICE_INDEX_0, 
                                          &usbReadBuffer->usbReadHandle, 
#ifdef APP_BSUBFRAMESIZE_3
                                          1, usbReadBuffer->buffer,  
#else
                                          1, usbReadBuffer->buffer32,  
#endif //APP_BSUBFRAMESIZE_3
                                          appData.usbReadBufSize);//96*3 bytes

                    if(audioErr1 == USB_DEVICE_AUDIO_RESULT_OK)
                    {
                        //USB Buffer Read OK
                        usbReadBuffer->usbInUse = true;

                        appBufferQueue.usbReadQueueCnt++;
                        appBufferQueue.usbReadIdx = 
                                _APP_GetNextIdx(appBufferQueue.usbReadIdx);
                    }
                    else
                    {
                        //USB Read Fail to queue this buffer
                        //--> Wait for queue to empty
                        usbReadBuffer->usbInUse = false;
                    }
                } //End USB Queue Read
                
                //------------------------- 
                //CODEC ADD WRITE
                //if (_APP_BufferInCodec())
                {
                    //Playback Codec is Ready (not in use)

                    //CODEC Write to codecWriteIdx (Next Queue TAILIndex)
                    codecWriteIdx = appBufferQueue.codecWriteIdx;
                    currentPlayBuffer = 
                            &appBufferQueue.playbackBuffer[codecWriteIdx];

                    if (currentPlayBuffer->usbReadCompleted==true && 
                        currentPlayBuffer->codecInUse==false)
                    {
                        #ifdef DEBUG_TONE_CODEC_TX
                        //Write stereo tone to output instead of USB data.
                        DRV_CODEC_BufferAddWrite(appData.codecClientWrite.handle, 
                                                 &currentPlayBuffer->codecWriteHandle,
                                                 testBuffer2, sizeof(testBuffer2));
                        #else //DEBUG_TONE_CODEC_TX
                        //Unpack the 24 bit buffer to the 32 bit buffer
                        //int i;
                        int j;
                        int numUsb24BitSamples = appData.usbReadBufSize/3;

                        src = (UInt24 *) currentPlayBuffer->buffer;
                        dst = (uint32_t *) currentPlayBuffer->buffer32; 
                        for (j=0; j<numUsb24BitSamples; j++) 
                        {
                            dst[j] = unpack(src[j]);
                        }
#if 0//
#ifdef APP_BSUBFRAMESIZE_3

                        src = (uint32_t *) currentPlayBuffer->buffer;
                        dst = (uint32_t *) currentPlayBuffer->buffer32; 

                        //_Unpack24to32bitBuffer(*src, *dst,  numUsb24BitSamples);
                        for (i=0, j=0; j<numUsb24BitSamples; i+=3, j+=4) 
                        {
                            //Every 3 32 bit words converted to 4 32 bit samples
                            sa = src[i+0]; //LS
                            sb = src[i+1];
                            sc = src[i+2]; //MS

                            //Little ENDIAN - MS bytes first
                            //dst[j+0] = sa & 0xFFFFFF00;
                            dst[j+0] = sa<<8; 
                            //dst[j+1] = ((sa<<24) | (sb>>8)) & 0xFFFFFF00;
                            dst[j+1] = ((sb<<16) | (sa>>16)) & 0xFFFFFF00;
                            //dst[j+2] = ((sb<<16) | (sc>>16)) & 0xFFFFFF00;
                            dst[j+2] = ((sb>>8) | (sc<<24)) & 0xFFFFFF00;
                            //dst[j+3] = sc<<8;
                            dst[j+3] = sc & 0xFFFFFF00;
                        }
#endif //APP_BSUBFRAMESIZE_3
#endif //0


                        TEST3_Toggle();
                        DRV_CODEC_BufferAddWrite(
                                    appData.codecClientWrite.handle, 
                                    &currentPlayBuffer->codecWriteHandle,
                                    currentPlayBuffer->buffer32, 
                                    appData.codecClientWrite.bufferSize); //96*8=768

                        #endif //DEBUG_TONE_CODEC_TX

                        if (currentPlayBuffer->codecWriteHandle != DRV_CODEC_BUFFER_HANDLE_INVALID)
                        {
                            currentPlayBuffer->codecInUse = true;
                            
                            appBufferQueue.codecWriteQueueCnt++;
                            appBufferQueue.codecWriteIdx = _APP_GetNextIdx(codecWriteIdx);

                            //Check for QUEUE Empty 
                            if (appBufferQueue.codecWriteIdx == appBufferQueue.usbReadIdx)
                            {
                               //QUEUE is empty do not execute USB Read command;
                               queueEmpty = true;
                            }
                            else
                            {
                               queueEmpty = false;
                            }
                        }
                        else
                        {
                            // CODEC doesn't have enough write buffers
                            // --Wait till one becomes available
                            currentPlayBuffer->codecInUse = false;
                        }
                    } //USB Read Buffer Playback
                    else
                    {
                        asm("NOP");
                    }
                } //Check for read buffer available.
            } //End APP_USB_SPEAKER_PLAYBACK_STEREO
        }
        break; //End case APP_PROCESS_DATA:

        //---------------------------------------------------------------------
        // APP_MUTE_AUDIO_PLAYBACK
        //---------------------------------------------------------------------
        case APP_MUTE_AUDIO_PLAYBACK:
        {
            if (appData.activeInterfaceAlternateSetting == 0 && 
                   APP_USB_SPEAKER_PLAYBACK_NONE)
            {
                appData.ledState = CONNECTED_BLINK;
                appData.muteEn = true;
                DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                appData.state = APP_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO)
            {
                appData.ledState = LED_ON;
                appData.muteEn = false;
                DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                appData.state = APP_STATE_INITIAL_USB_READ_REQUEST;
            }
        }
        break;

        //---------------------------------------------------------------------
        // APP_SAMPLING_FREQUENCY_CHANGE - RTOS Version
        //---------------------------------------------------------------------
        case APP_SAMPLING_FREQUENCY_CHANGE:
        {
            //Changes sampling rate for Record and Playback (both stereo)
            if(DRV_CODEC_I2S_MASTER_MODE == true)
            {
                DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                                          appData.sampleFreq);
            }
            else
            {
                //Slave Mode Codec I2S
                //--LRCK set by MCU
            }
            appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
        }
        //break;
        
        //---------------------------------------------------------------------
        // APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD
        //---------------------------------------------------------------------
        case APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        {
            SYS_STATUS codecStatus;

            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE)
            {
                SYS_PRINT("APP: AS 0 - Mute\r\n");
                _APP_Init_PlaybackBufferQueue();
                queueFull = false;
                queueEmpty = true;

                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
                    appData.ledState = MUTE_BLINK;
                    appData.muteEn = true;
                    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                    appData.state = APP_IDLE;
                }
                appData.state = APP_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_STEREO)
            {
                SYS_PRINT("APP: AS 1 - PLAYBACK\r\n");
                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
                    appData.ledState = LED_ON;
                    appData.muteEn = false;
                    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                    _APP_Init_PlaybackBufferQueue();
                    appData.state =  APP_STATE_INITIAL_USB_READ_REQUEST;
                }
            }
        }
        break;

        //---------------------------------------------------------------------
        // APP_IDLE
        //---------------------------------------------------------------------
        case APP_IDLE:
        {
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE))
            {
                asm("Nop");
                //appData.ledState = MUTE_BLINK;
                //appData.muteEn = true;
                //DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
            }
        }
        break;

        //---------------------------------------------------------------------
        // APP_STATE_ERROR - default
        //---------------------------------------------------------------------
        case APP_STATE_ERROR:
        default:
        {
            /* TODO: Handle error in application's state machine. */
            appData.ledState = LED_ERROR_BLINK;
            break;
        }

    } //End switch(appData.state))

    //Save state for debug of transitions
    appData.lastState = appData.state;

} //End APP_Tasks()


//******************************************************************************
// APP_CODECBufferEventHandler()
//
// Application CODEC Write buffer Event handler.
// This function is called back by the CODEC driver when
// a CODEC data buffer TX completes.
//******************************************************************************
//void __attribute__((tcm)) 
void 
APP_CODECBufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
                            DRV_CODEC_BUFFER_HANDLE handle, 
                            uintptr_t context )
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
        {
            TEST4_Toggle();

            //DEBUG
            appBufferQueue.codecWriteCompleteCnt++;

            if (appBufferQueue.codecWriteIdx != appBufferQueue.usbReadIdx)
            {
                //QUEUE is not full or empty
                asm("NOP");
            }

            //This buffer is ready for USB Write
            int codecCompleteIndex = _APP_ClearCodecReturnBuffer(handle);

            if (codecCompleteIndex < 0)
            {
                asm("NOP");
            }

            //Remove 1 buffer from USB complete read queue
            appBufferQueue.codecWriteQueueCnt--;
            appBufferQueue.usbReadCompleteBufferLevel--;

            if (appBufferQueue.usbReadCompleteBufferLevel == 0)
            {
                //USB Read needs to complete before next Codec Write.
                queueEmpty = true;
//                SYS_PRINT("*** UNDERFLOW ***: QLevel %d - Ridx %d - Widx %d\r\n",
//                        appBufferQueue.usbReadCompleteBufferLevel,
//                        appBufferQueue.usbReadIdx,
//                        appBufferQueue.codecWriteIdx);
//                SYS_PRINT("*** underFLOW ***: Qlevel %d RQ %d WQ %d WC %d\r\n",
//                        appBufferQueue.usbReadCompleteBufferLevel,
//                        appBufferQueue.usbReadQueueCnt,
//                        appBufferQueue.codecWriteQueueCnt,
//                        appBufferQueue.codecWriteCompleteCnt);
            }
        }
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
        } break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
        } break;
    }
    ;
} //End APP_CODECBufferEventHandler()


//******************************************************************************
// _APP_SetUSBReadBufferReady()
//--Set the USB Read Buffer Data Ready for CODEC Write 
//******************************************************************************
static int _APP_SetUSBReadBufferReady(USB_DEVICE_AUDIO_TRANSFER_HANDLE handle)
{
    int i=0;
    for (i=0; i<APP_QUEUING_DEPTH; i++)
    {
        if (appBufferQueue.playbackBuffer[i].usbReadHandle == handle)
        {
            appBufferQueue.playbackBuffer[i].usbReadCompleted = true;
            appBufferQueue.playbackBuffer[i].usbInUse = false;
            return i;
        }
    }
    return -1;
}

//******************************************************************************
// _APP_USBReadAllBufferReady()
//    Check if USB Read Buffer Queue is Ready for CODEC Writes 
//******************************************************************************
static bool _APP_USBReadAllBufferReady()
{
    int i = 0;

    //Fill the queue with data before writing
    //for (i=0; i<APP_QUEUING_DEPTH; i++)
    for (i=0; i<APP_QUEUING_DEPTH/QUEUE_USB_INIT_PART; i++)
    {
        if(appBufferQueue.playbackBuffer[i].usbReadCompleted != true)
        {
            return false;
        }
    }
    return true;
}

//******************************************************************************
// _APP_Init_PlaybackBufferQueue()
//Initialize Codec Playback Buffer Queue
//******************************************************************************
static void _APP_Init_PlaybackBufferQueue()
{
    int i=0;
    appBufferQueue.codecWriteIdx = 0;
    appBufferQueue.usbReadIdx = 0;

    for(i=0;i<APP_QUEUING_DEPTH;i++)
    {
        appBufferQueue.playbackBuffer[i].codecInUse = false;
        appBufferQueue.playbackBuffer[i].usbInUse = false;
        appBufferQueue.playbackBuffer[i].usbReadCompleted = false;
    }
    appBufferQueue.usbReadCompleteBufferLevel = 0;
    appBufferQueue.codecWriteCompleteCnt      = 0;
    appBufferQueue.usbReadCompleteCnt         = 0;
    appBufferQueue.usbReadQueueCnt            = 0;
    appBufferQueue.codecWriteQueueCnt         = 0;

    DRV_CODEC_WriteQueuePurge(appData.codecClientWrite.handle);

    //Synchronize playback to the I2S L/R channel slots.
    appData.lrSync = true;
}

//******************************************************************************
//_APP_GetNextIdx()
//Increment the Head or Tail Index to Codec Playback Buffer Queue
//******************************************************************************
static uint8_t _APP_GetNextIdx(uint8_t index)
{
    if (index == 32)
    {
        asm("NOP");
    }
    return (index+1)%APP_QUEUING_DEPTH;
}

//******************************************************************************
// _APP_ClearCodecReturnBuffer()
//--Clear the Codec Playback Buffer to allow for  USB Reads 
//******************************************************************************
static int _APP_ClearCodecReturnBuffer(DRV_CODEC_BUFFER_HANDLE handle)
{
    int i = 0;
    for(i=0;i<APP_QUEUING_DEPTH;i++)
    {
        if(appBufferQueue.playbackBuffer[i].codecWriteHandle == handle)
        {
            appBufferQueue.playbackBuffer[i].codecWriteHandle = 
                    DRV_CODEC_BUFFER_HANDLE_INVALID;
            appBufferQueue.playbackBuffer[i].codecInUse = false;
            appBufferQueue.playbackBuffer[i].usbInUse = false;
            appBufferQueue.playbackBuffer[i].usbReadCompleted = false;
            appBufferQueue.playbackBuffer[i].usbReadHandle = 
                    USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;

            return i;
        }
    }
    
    return -1; //ERROR
}

//******************************************************************************
// Convert the packed 24 bit byte array value to 32 bit word value
// NOTE:  This is an 24Bit integer unpack (not Q23 to Q31)
//******************************************************************************
inline int32_t _Convert24to32bit(BITS24 val)
{
        if ( val.bytes[2] & 0x80 ) 
        {
            //negative -->  24 to 32 bit sign extend.
            return ( 0xff << 24)         | 
                    (val.bytes[2] << 16) | //LS first 
                    (val.bytes[1] << 8)  | 
                    (val.bytes[0] << 0);
        }
        else
        {
            return ( val.bytes[2] << 16) |
                    (val.bytes[1] << 8)  | 
                    (val.bytes[0] << 0);
        }
}


//******************************************************************************
// Convert the Unpacked 32 bit byte array value to 24 bit word value
// NOTE:  This is an 32Bit integer unpack (not Q23) without saturation
//        (assumes 0 in the 32 bit LS byte)
//******************************************************************************
inline void _Unpack24to32bitBuffer(uint8_t *src, uint8_t*dst, int numUsb24BitSamples)
{
    int i,j;
    uint32_t saa,sbb,scc;

    for (i=0, j=0; j<numUsb24BitSamples; i+=3, j+=4) 
    {
        //Every 3 32 bit words converted to 4 32 bit samples
        saa = src[i+0]; //LS
        sbb = src[i+1];
        scc = src[i+2]; //MS

        //Little ENDIAN - MS bytes first
        //dst[j+0] = saa & 0xFFFFFF00;
        dst[j+0] = saa<<8;  //Shift off the MS byte leaving 24 bits 
        //dst[j+1] = ((saa<<24) | (sbb>>8)) & 0xFFFFFF00;
        dst[j+1] = ((sbb<<16) | (saa>>16)) & 0xFFFFFF00; 
        //dst[j+2] = ((sbb<<16) | (scc>>16)) & 0xFFFFFF00;
        dst[j+2] = ((sbb>>8) | (scc<<24)) & 0xFFFFFF00;
        //dst[j+3] = scc<<8;
        dst[j+3] = scc & 0xFFFFFF00;
    }
}

//3byte Q24 to 4byte Q31
//If the byte index gives the addressing sequence, then this is Big Endian
//byte order.
inline uint32_t unpack(UInt24 x) 
{
    uint32_t retVal = x.byte[2];
    retVal = retVal << 8 | x.byte[1];
    retVal = retVal << 8 | x.byte[0];
    return retVal;
}

//4byte Q31 to 3byte Q24
//If the byte index gives the addressing sequence, then this is Big Endian
//byte order.
inline UInt24 pack(uint32_t x) 
{
    UInt24 retVal;
    retVal.byte[2] = (x >> 16) & 0xff;
    retVal.byte[1] = (x >> 8) & 0xff;
    retVal.byte[0] = x & 0xff;
    return retVal;
}

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
} 

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
                    appData.ledState = MUTE_BLINK;
                    appData.muteEn = true;
                    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                }
                else
                {
                    appData.ledState = LED_ON;
                    appData.muteEn = false;
                    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                    DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                       DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                       appData.volume);
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
                //    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                //}
                //else
                //{
                //    appData.ledState = MUTE_BLINK;
                //    appData.muteEn   =true;
                //    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
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

            case CONNECTED_BLINK:
                if (appData.blinkDelay == 0)
                {
                    LED1_Toggle();
                    appData.blinkDelay = 1000;  
                }
            break;

            case MUTE_BLINK:
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
/*******************************************************************************
 End of File
 */
