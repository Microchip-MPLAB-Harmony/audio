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

#include "bsp/bsp.h"


#include "user.h"


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

#ifdef PIC32MZEFC2
//LIBQ/DSP
#else //E70
//CMSIS-DSP - not included in definitions.h
#include "arm_math.h"
#endif //PIC32MZEFC2


//#include "display.h"

//DEBUG 
#define INCLUDE_BASS_BOOST
#undef  DEBUG_TONE_CODEC_TX
#undef  DEBUG_TONE_SQUARE
//static volatile bool __attribute__((tcm,unused)) 
static volatile bool 
    testVal1 = false;
//static volatile bool __attribute__((tcm,unused)) 
static volatile bool 
    testVal2 = false;

#ifdef PIC32MZEFC2
const char * ledStr[] =
{   "LED_STATE_NONE",
    "LED_NO_CONNECT_BLINK",    //Red Blink
    "LED_CONFIGURED",          //Red
    "LED_CONNECT_BLINK",       //Green Blink 
    "LED_AUDIO_PLAY",          //Green
    "LED_AUDIO_MUTE_BLINK",    //Green Blink
    "LED_BOOST_PLAY",          //Blue
    "LED_BOOST_MUTE_BLINK",    //Blue Blink
    "LED_ERROR_BLINK",         //White Blink
};

const char * buttonStr[] = 
{ "B_NONE",
  "B_IDLE",
  "B_PRESS",   //Any button pressed
  "B1_PRESSED",   
  "B2_PRESSED",   
  "B3_PRESSED",   
  "B4_PRESSED",   
  "B1_RELEASE",    
  "B2_RELEASE",    
  "B3_RELEASE",    
  "NUM_BUTTON_STATES"
};
#endif //PIC32MZEFC2


bool queueFull;
bool queueEmpty;

int usbReadScheduleCnt = 0; 
int usbReadFailureCnt = 0;
int usbReadCompleteCnt = 0; 
int codecWriteScheduleCnt = 0; 
int codecWriteFailureCnt = 0;
int codecWriteCompleteCnt = 0; 
int usbNextCompleteIdx = 0;
int codecNextCompleteIdx = 0;
int usbReadCompleteQLvl = 0;

uint16_t volumeLevels[VOLUME_STEPS] =
{
#ifdef CODEC_AK4954
    0 /* off */, 192, 208, 224          // if using AK4954, use narrower range  
#else 
    //CODEC_WM8904
    0 /* off */, 128, 192, 255
#endif         
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

#ifdef DEBUG_TONE_CODEC_TX

/* PCM16 samples for 1Khz Sine Wave at 48Khz Sample Rate */
static const DRV_I2S_DATA16 
    //__attribute__((aligned(32))) __attribute__((tcm)) audioSamples[48] =  {
    __attribute__((aligned(32))) audioSamples[48] =  {
//Sin Tone 1Khz @ 48Khz Sampling Rate - .5 Amplitude - 1 Cycle
#ifdef DEBUG_TONE_SQUARE
#include "sq_1Khz_p5_48Ksps.dat"
#else //DEBUG_TONE_SQUARE
#include "sin_1Khz_p5_48Ksps.dat"
#endif //DEBUG_TONE_SQUARE
};

//DEBUG_TONE Queue Buffers
static DRV_I2S_DATA16 
    //__attribute__((aligned(32))) __attribute__((tcm)) 
    __attribute__((aligned(32))) 
        cpBuffer16[APP_QUEUING_DEPTH][48];
#endif //DEBUG_TONE_CODEC_TX

//Button Timer
DRV_HANDLE tmrHandle;

APP_PLAYBACK_BUFFER * current;
DRV_I2S_DATA16 * currentBuffer;
int8_t codecWriteIdx; 

//uint32_t timerStart, timerEnd;
//uint32_t cycles = 0;
volatile bool usbReadCompleteFlag = false;

//NOTE: APP_PLAYBACK BUFFER is what is transferred using DMA--> it is padded
// and placed at the beginning of the struct and the struct attribute is the
// allocated with the COHERENT and aligned(16) attributes so that it is 
// placed at the correct page boundary.
//static __attribute__((aligned(32))) __attribute__((tcm))
#ifdef PIC32MZEFC2
static __attribute__((aligned(32))) __attribute((coherent))
    APP_PLAYBACK_BUFFER_QUEUE appPlaybackBuffer;
#else //E70
static __attribute__((aligned(32)))
    APP_PLAYBACK_BUFFER_QUEUE appPlaybackBuffer;
#endif //PIC32MZEFC2

//==============================================================================
//BASS BOOST
#ifdef INCLUDE_BASS_BOOST
//------------------------------------------------------------------------------
#ifdef PIC32MZEFC2
//-- Using DSP_FilterIIRQB16_fast(X, PARM_EQUAL_FILTER_16 *)
// NOTE: All coefs are half value of original design, gain handled in algorithm
// note subtract is handled in algorithm, so a coefs go in at actual transfer
// funciton value.
PARM_EQUAL_FILTER_16 * ptrFilterEQL = NULL;
PARM_EQUAL_FILTER_16 * ptrFilterEQR = NULL;
int16_t dataInQ15;

//4Db Bass Boost
PARM_EQUAL_FILTER_16  bb4DbCoeffsLeftQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x401F, 0x80F5, 0x3f13},  //b0,b1,b2,a1,a2
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

PARM_EQUAL_FILTER_16  bb4DbCoeffsRightQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x401F, 0x80F5, 0x3f13},  //b0,b1,b2,a1,a2
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

//6Db Bass Boost
//['0x4032', '0x80fb', '0x3f05', '0x4000', '0x80ee', '0x3f2b']
PARM_EQUAL_FILTER_16  bb6DbCoeffsLeftQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4032,0x80Fb, 0x3f05}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};
PARM_EQUAL_FILTER_16  bb6DbCoeffsRightQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4032,0x80Fb, 0x3f05}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

//8Db Bass Boost
//['0x4048', '0x8101', '0x3ef6', '0x4000', '0x80ee', '0x3f2b']
PARM_EQUAL_FILTER_16  bb8DbCoeffsLeftQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4048,0x8101, 0x3ef6}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};
PARM_EQUAL_FILTER_16  bb8DbCoeffsRightQ15  =
{
    //{ 0x401F, 0x0000, 0x80F5, 0x3F13, 0x7F12, 0xC0D5};
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4048,0x8101, 0x3ef6}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

//10Db Bass Boost
//['0x4060', '0x8109', '0x3ee6', '0x4000', '0x80ee', '0x3f2b']
PARM_EQUAL_FILTER_16  bb10DbCoeffsLeftQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4060,0x8195, 0x3ee6}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};
PARM_EQUAL_FILTER_16  bb10DbCoeffsRightQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x4060,0x8195, 0x3ee6}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

//12Db Bass Boost
//['0x407d', '0x8114', '0x3ed4', '0x4000', '0x80ee', '0x3f2b']
PARM_EQUAL_FILTER_16  bb12DbCoeffsLeftQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x407d,0x8114, 0x33d4}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};
PARM_EQUAL_FILTER_16  bb12DbCoeffsRightQ15  =
{
    //.G = 0x7FFF,
    .log2Alpha = 1,
    .b = { 0x407d,0x8114, 0x33d4}, 
    .a = { 0x80ee, 0x3f2b},
    .Z = { 0, 0},
};

#define NUMBBLEVELS 4
_Sat _Fract gainFactorFract;
_Sat _Fract dataValFract;
PARM_EQUAL_FILTER_16 *ptrBbCoeffsLeftQ15[NUMBBLEVELS] = 
{ 
    &bb4DbCoeffsLeftQ15, 
    &bb6DbCoeffsLeftQ15, 
    &bb8DbCoeffsLeftQ15, 
    &bb10DbCoeffsLeftQ15, 
    //&bb12DbCoeffsLeftQ15, 
};
PARM_EQUAL_FILTER_16 *ptrBbCoeffsRightQ15[NUMBBLEVELS] = 
{ 
    &bb4DbCoeffsRightQ15, 
    &bb6DbCoeffsRightQ15, 
    &bb8DbCoeffsRightQ15, 
    &bb10DbCoeffsRightQ15, 
    //&bb12DbCoeffsRightQ15, 
};
_Sat _Fract gainFactorLevelsQ15[NUMBBLEVELS] =
{
    .95, .95, .95, .95,
};

#else //E70
//------------------------------------------------------------------------------
//CMSIS_LIBRARY
#define BASS_BOOST_10DB
//Peaking Filter used as Bass Band shelving filter
//Add 0 after b0, for ARM SIMD calculation
//Use Negated denominator(a) coefficients for CMSIS 
//--Scaled by >>1
#if defined(BASS_BOOST_4DB)
//4dB Peaking Filter used as Bass Band shelving filter
#define BBGAINDB  4.00    
q15_t bbCoeffsQ15[5] = { 0x401F, 0x80F5, 0x3F13, 0x80ee, 0x3f2b };
q15_t bbCoeffsArmQ15[6]   = { 0x401F, 0x0000, 0x80F5, 0x3F13, 0x80EE, 0x3F2B };
q15_t bbCoeffsMaArmQ15[6] = { 0x401F, 0x0000, 0x80F5, 0x3F13, 0x7F12, 0xC0D5};

#elif defined(BASS_BOOST_6DB)
//6dB Peaking Filter used as Bass Band shelving filter
#define BBGAINDB       6.00    
q15_t bbCoeffsQ15[5] = { 0x4032, 0x80FB, 0x3F05, 0x80ee, 0x3f2b };
q15_t bbCoeffsArmQ15[6]   = { 0x4032, 0x0000, 0x80FB, 0x3F05, 0x80EE, 0x3F2B };
q15_t bbCoeffsMaArmQ15[6] = { 0x4032, 0x0000, 0x80FB, 0x3F05, 0x7F12, 0xC0D5};

#elif defined(BASS_BOOST_10DB)
//10dB Peaking Filter used as Bass Band shelving filter
#define BBGAINDB       10.00    
q15_t bbCoeffsQ15[5] = { 0x4060, 0x8109, 0x3EE6, 0x80ee, 0x3f2b };
q15_t bbCoeffsArmQ15[6]   = { 0x4060, 0x0000, 0x8109, 0x3EE6, 0x80EE, 0x3F2B };
q15_t bbCoeffsMaArmQ15[6] = { 0x4060, 0x0000, 0x8109, 0x3EE6, 0x7F12, 0xC0D5};

#elif defined(BASS_BOOST_12DB)
//12dB Peaking Filter used as Bass Band shelving filter
#define BBGAINDB       12.00    
q15_t bbCoeffsQ15[5] = { 0x407D, 0x8114, 0x3ED4, 0x80ee, 0x3f2b };
q15_t bbCoeffsArmQ15[6]   = { 0x407D, 0x0000, 0x8114, 0x3ED4, 0x80EE, 0x3F2B };
q15_t bbCoeffsMaArmQ15[6] = { 0x407D, 0x0000, 0x8114, 0x3ED4, 0x7F12, 0xC0D5};
#endif //BASS_BOOST_xxDB
#define BBCOEFSHIFTQ15    1       //1
#define BBNUMSTAGES       1       //Biquad Stages

//Unity Gain Filter
q15_t testCoeffsArmQ15[6] = { 0x4000, 0x0000, 0x0000, 0x0000, 0x0000 };
#define TESTGAINDB  0.00    
#define TESTGAINREDUCQ15 0x7FFF
#define TESTCOEFSHIFTQ15    1 //1
#define TESTNUMSTAGES       1   //Biquad Stages

q15_t bbLeftStateQ15[4*BBNUMSTAGES]   = {0};
q15_t bbRightStateQ15[4*BBNUMSTAGES]  = {0};

arm_biquad_casd_df1_inst_q15 bbLeftBiquadQ15 =
{
    .numStages = 1,
    .pState    = bbLeftStateQ15,
    .pCoeffs   = bbCoeffsMaArmQ15,
    .postShift = BBCOEFSHIFTQ15
};  //arm_biquad_casd_df1_inst_q15

arm_biquad_casd_df1_inst_q15 bbRightBiquadQ15 =
{
    .numStages = 1,
    .pState    = bbRightStateQ15,
    .pCoeffs   = bbCoeffsMaArmQ15,
    .postShift = BBCOEFSHIFTQ15
};  //arm_biquad_casd_df1_inst_q15

static q15_t __attribute__((aligned(32))) __attribute((unused))
        leftInQ15[48];
static q15_t __attribute__((aligned(32))) __attribute((unused))
        rightInQ15[48];
static q15_t __attribute__((aligned(32))) __attribute((unused))
        leftOutQ15[48];
static q15_t __attribute__((aligned(32))) __attribute((unused))
        rightOutQ15[48];

#endif //PIC32MZEFC2/SAME70CULT
#endif //INCLUDE_BASS_BOOST

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
void LedOnStateClear(void);

static bool volatile hpInterfaceChanged = false;
static USB_DEVICE_AUDIO_RESULT volatile audioErr1;

//Application Class Data
APP_DATA appData =
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
#ifdef SAME70CULT
    .ledState = LED_OFF,
    .blinkDelay = 0,
#elif defined(PIC32MZEFC2)
    .buttonDelay = 0,
    .buttonState = BUTTON_STATE_IDLE,
    .lastButtonState = BUTTON_STATE_NONE,
    .blinkDelay = 0,
    .ledState = LED_NO_CONNECT_BLINK,
    .lastLedState = LED_STATE_NONE,
#endif //SAME70CULT/PIC32MZEFC2
    
    .sampleFreq = 0xBB80,    //48000 Hz
    
    .usbReadBufSize = 192,   //APP_MAX_NO_BYTES_IN_USB_BUFFER,   
                             //48000Hz  1ms = 48*4 bytes
    
    //CODEC Driver Write only Client
    .codecClientWrite.context = (uintptr_t)0,
    .codecClientWrite.bufferHandler = (DRV_CODEC_BUFFER_EVENT_HANDLER) 
                                          APP_CODECBufferEventHandler,
    .codecClientWrite.bufferSize = 192,   //APP_MAX_NO_BYTES_IN_USB_BUFFER,
    
    /* No Audio control in progress.*/
    .currentAudioControl = APP_USB_CONTROL_NONE,

    .volumeIndex = 1,
    .volume = 255,

    .lrSync = true,
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
#if defined(SAME70CULT)
                appData.ledState = CONNECTED_BLINK;
                appData.blinkDelay = 0; 
#elif defined(PIC32MZEFC2)
                appData.ledState = LED_CONFIGURED;
                appData.blinkDelay = 0; 
#endif //SAME70CULT/PIC32MZEFC2
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            /* Switch on green and orange, switch off red */
#if defined(SAME70CULT)
            appData.ledState = CONNECTED_BLINK;
            appData.blinkDelay = 0; 
#elif defined(PIC32MZEFC2)
            appData.ledState = LED_CONFIGURED;
            appData.blinkDelay=0;
            
#endif //SAME70CULT/PIC32MZEFC2
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS was removed. Notify USB stack about the event*/
            USB_DEVICE_Detach (appData.usbDevHandle);
#if defined(SAME70CULT)
            appData.ledState = LED_OFF;
#elif defined(PIC32MZEFC2)
            LedOnStateClear();  //Detached
            appData.ledState = LED_NO_CONNECT_BLINK;
            appData.blinkDelay = 0;
#endif //SAME70CULT/PIC32MZEFC2
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
                        appData.activeInterfaceAlternateSetting = interfaceInfo->interfaceAlternateSetting;
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
                //DEBUG
                usbReadCompleteCnt++;
#ifdef SAME70CULT 
                TEST4_Toggle();
#endif //SAME70CULT

                //We have received an audio frame from the Host.
                //Now send this audio frame to Audio Codec for Playback.
                readEventData = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *)pData;

                //Make USB Read Buffer Queue Ready for Data (Clear data flags)
                int __attribute__((unused)) usbBufferIdx = 
                    _APP_SetUSBReadBufferReady(readEventData->handle);

                appPlaybackBuffer.usbReadCompleteBufferLevel++;
                appPlaybackBuffer.usbReadCompleteCnt++;
                appPlaybackBuffer.usbReadQueueCnt--;  //Not completed level

                queueEmpty = false;

                //Check if the this is the initial loading of the playback
                //queue with USB Rx Data 
                if(appData.state == APP_STATE_INITIAL_CODEC_WRITE_REQUEST)
                {
                    if(_APP_USBReadAllBufferReady())
                    {
                        usbReadCompleteFlag = true;
//                        SYS_PRINT("USB INIT READ COMPLETEs:  Qlevel %d - Ridx %d - Widx %d",
//                            appPlaybackBuffer.usbReadCompleteBufferLevel,
//                            appPlaybackBuffer.usbReadIdx,
//                            appPlaybackBuffer.codecWriteIdx);
//                        SYS_PRINT("USB INIT READ COMPLETEs: Qlevel %d RQCnt %d WQCnt %d",
//                            appPlaybackBuffer.usbReadCompleteBufferLevel,
//                            appPlaybackBuffer.usbReadQueueCnt,
//                            appPlaybackBuffer.codecWriteQueueCnt);
                        usbReadCompleteFlag = true;
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
                    //DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                    //                          appData.sampleFreq);
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
    SYS_DEBUG_PRINT(0,"----------------------------------------\r\n");
    SYS_DEBUG_PRINT(0,"- Starting:\r\n");
    SYS_DEBUG_PRINT(0,"----------------------------------------\r\n");

    appData.state = APP_STATE_INIT;
    
    appData.volumeIndex = 1;
    appData.volume = volumeLevels[appData.volumeIndex];    
    
    appData.buttonDelay = 0;
    appData.buttonState = BUTTON_STATE_IDLE;
    appData.blinkDelay = 0;
#if defined(SAME70CULT)
    appData.ledState= LED_OFF;
#elif defined(PIC32MZEFC2)
    appData.lastButtonState = BUTTON_STATE_NONE;
    appData.ledState = LED_NO_CONNECT_BLINK;
    appData.lastLedState = LED_STATE_NONE;
    LedOnStateClear();
#endif //SAME70CULT/PIC32MZEFC2

    appData.lrSync = true;

#ifdef DEBUG_TONE_CODEC_TX
    int i,j;
    for (i=0; i<APP_QUEUING_DEPTH; i++ )
    {
        for (j=0; j<48; j++)
        {
            cpBuffer16[i][j].leftData  = audioSamples[j].leftData;
            cpBuffer16[i][j].rightData = audioSamples[j].rightData;
        }
    }
#endif //DEBUG_TONE_CODEC_TX

#if defined(USE_DISPLAY)
    display_init(&DISPLAY_STATS);
#endif

#ifdef SAME70CULT 
TEST1_Set();
TEST2_Set();
TEST3_Set();
TEST4_Set();
TEST5_Set();
#endif //SAME70CULT

#ifdef INCLUDE_BASS_BOOST
#ifdef PIC32MZEFC2
ptrFilterEQL = &bb4DbCoeffsLeftQ15;
ptrFilterEQR = &bb4DbCoeffsRightQ15;
#else  //E70
arm_biquad_cascade_df1_init_q15(
    &bbLeftBiquadQ15,  BBNUMSTAGES, bbCoeffsMaArmQ15, bbLeftStateQ15,  BBCOEFSHIFTQ15);
arm_biquad_cascade_df1_init_q15(
    &bbRightBiquadQ15, BBNUMSTAGES, bbCoeffsMaArmQ15, bbRightStateQ15, BBCOEFSHIFTQ15);
#endif //PIC32MZEFC2 / E70
#endif //INCLUDE_BASS_BOOST

} //End APP_Initialize()


//******************************************************************************
// APP_Tasks()
//
// Application tasks routine. This function implements the
// application state machine.  //******************************************************************************
//void __attribute__((tcm)) APP_Tasks() 
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
                       
                    //DRV_CODEC_SamplingRateSet(appData.codecClient.handle, 48000); 
                if (appData.codecClientWrite.handle != DRV_HANDLE_INVALID) 
                {
                    DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                        DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                        appData.volume);
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                    //DRV_CODEC_SamplingRateSet(appData.codecClient.handle, 48000); 
                }
                else
                {
                    SYS_DEBUG(0, "Find out whats wrong \r\n");
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
            DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                                      appData.sampleFreq);

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
                APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                //Clear the frame counts
                usbReadScheduleCnt = 0;
                usbReadFailureCnt = 0;
                usbReadCompleteCnt = 0;
                usbReadCompleteQLvl = 0;  

                usbNextCompleteIdx = 0;

#if defined(SAME70CULT)
                appData.ledState = CONNECTED_BLINK;
#elif defined(PIC32MZEFC2)
                appData.ledState   = LED_CONNECT_BLINK;
                appData.blinkDelay = 0;
#endif //SAME70CULT/PIC32MZEFC2

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
        //              --16Khz playback alternate setting
        //         APP_MUTE_AUDIO_PLAYBACK 
        //              --16Khz playback alternate setting
        //
        // Wait for 16Khz alt playback 
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
                     APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                //Initialize/Purge playback queue
                _APP_Init_PlaybackBufferQueue();
                queueFull = false;
                queueEmpty = true;
#if defined(SAME70CULT)
                appData.ledState = LED_ON;
#elif defined(PIC32MZEFC2)
                appData.ledState = LED_AUDIO_PLAY;
#endif //SAME70CULT/PIC32MZEFC2
                appData.blinkDelay = 0;
                DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                    DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                    appData.volume);

                //To check alignment
                //int value1 = sizeof(APP_PLAYBACK_BUFFER)%16; 

                //Fill the queue with USB Read
                //--Flag to wait for Initial USB Read Q to fill. 
                usbReadCompleteFlag = false;  

                for (i = 0; i < APP_QUEUING_DEPTH; i++)
                {
                    //USB Read to Head of Codec Playback Buffer
                    APP_PLAYBACK_BUFFER* usbReadBuffer = 
                            &appPlaybackBuffer.playbackBuffer[i];

                    if (usbReadBuffer != NULL && 
                        !usbReadBuffer->codecInUse && 
                        !usbReadBuffer->usbInUse)
                    {

                        usbReadBuffer->usbReadCompleted = false;
                        usbReadBuffer->usbInUse = true;

                        //Read to Queue HEAD
#ifdef SAME70CULT 
                        TEST3_Toggle();
#endif //SAME70CULT
                        audioErr1 = USB_DEVICE_AUDIO_Read(
                                        USB_DEVICE_INDEX_0, 
                                        &usbReadBuffer->usbReadHandle, 
                                        1, usbReadBuffer->buffer, 
                                        appData.usbReadBufSize); //48 Samples

                        if(audioErr1 != USB_DEVICE_AUDIO_RESULT_OK)
                        {
                            //USB Read Queue Full and does not match the APP 
                            //read queue size--ERROR.
                            usbReadBuffer->usbInUse = false;
                            break;

                            //DEBUG
                            usbReadScheduleCnt++;
                        }
                        else
                        {
                            appPlaybackBuffer.usbReadQueueCnt++;
                            //SYS_PRINT("***INIT USB READ: RQCnt %d", 
                            //    appPlaybackBuffer.usbReadQueueCnt);
                            //Next Codec Write Index (HEAD Index)
                            appPlaybackBuffer.usbReadIdx = _APP_GetNextIdx(appPlaybackBuffer.usbReadIdx);
                        }
                    }
                } //End USB Audio Read Queue loop


//                SYS_PRINT("***INIT USB READ Finished: RQCnt %d", 
//                         appPlaybackBuffer.usbReadQueueCnt);
                appData.state = APP_STATE_INITIAL_CODEC_WRITE_REQUEST;

            } //activeInterfaceAlternatedSetting 
            else
            {
#if defined(SAME70CULT)
                appData.ledState = CONNECTED_BLINK;
#elif defined(PIC32MZEFC2)
                appData.ledState   = LED_AUDIO_MUTE_BLINK;
#endif //SAME70CULT/PIC32MZEFC2
                appData.blinkDelay = 0;
                appData.volumeMuteOn = true;
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
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                _APP_Init_PlaybackBufferQueue();
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE; 
            }
            else if  (usbReadCompleteFlag == true)
            {
                //Set the volume
                DRV_CODEC_LRCLK_Sync(appData.codecClientWrite.handle);
                //DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                //                    DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                //                    appData.volume);
                //Initialize the Codec Write Queue
                for (i = 0;
                     i < APP_QUEUING_DEPTH/QUEUE_USB_INIT_PART; i++)
                {

                        codecWriteIdx = appPlaybackBuffer.codecWriteIdx;
                        current = &appPlaybackBuffer.playbackBuffer[codecWriteIdx];


                    //CODEC Playback 
                    if (current->usbReadCompleted == true && 
                        current->usbInUse   == false &&
                        current->codecInUse == false)
                    {
                        current->codecInUse = true;

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
                        //Write stereo tone to output instead of USB data.
                        DRV_CODEC_BufferAddWrite(
                                    appData.codecClientWrite.handle, 
                                    &current->codecWriteHandle,
                                    &cpBuffer16[0],
                                    //sinBuffer16, 
                                    192); //1ms 16bit stereo buffer @ 48Khz 
#else //DEBUG_TONE_CODEC_TX
                        currentBuffer = (DRV_I2S_DATA16 *) current->buffer;

                        DRV_CODEC_BufferAddWrite(
                                appData.codecClientWrite.handle, 
                                &current->codecWriteHandle,
                                currentBuffer, 
                                appData.codecClientWrite.bufferSize); //48*4 bytes
#endif //DEBUG_TONE_CODEC_TX 
                        if(current->codecWriteHandle != DRV_CODEC_BUFFER_HANDLE_INVALID)
                        {
#ifdef SAME70CULT 
                            TEST1_Toggle();
#endif //SAME70CULT
                            appPlaybackBuffer.codecWriteQueueCnt++;
                            appPlaybackBuffer.codecWriteIdx = 
                                    _APP_GetNextIdx(codecWriteIdx);

                            //DEBUG
                            codecWriteScheduleCnt++;
                        }
                        else
                        {
                            current->codecInUse = false;
                            // CODEC doesn't have enough write buffers
                            // should never happen
                            asm("NOP");
                        }
                    } //Buffer Ready
                } //Queue Loop
//                SYS_PRINT("USB INIT READ QUEUE: Qlevel %d RQ %d WQ %d WC %d",
//                         appPlaybackBuffer.usbReadCompleteBufferLevel,
//                         appPlaybackBuffer.usbReadQueueCnt,
//                         appPlaybackBuffer.codecWriteQueueCnt,
//                         appPlaybackBuffer.codecWriteCompleteCnt);
                appData.state = APP_PROCESS_DATA;
                usbReadCompleteFlag = false;
            } //Wait for initial USB Reads
        } //End case APP_STATE_INITIAL_CODEC_WRITE_REQUEST
        break;

        //---------------------------------------------------------------------
        // Process USB Read buffers to Codec Playback  and Codec read mic data to
        // USB Write buffers
        //
        // Transition From:
        //    APP_STATE_INITIAL_CODEC_WRITE_REQUEST:
        //    --When usbReadCompleteFlag is true
        //---------------------------------------------------------------------
        case APP_PROCESS_DATA:
        {

            if (appData.activeInterfaceAlternateSetting == 
                APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                //----------------------------
                //USB read to usbReadIdx (Next Queue HEAD index)
                APP_PLAYBACK_BUFFER * usbReadBuffer = 
                     &appPlaybackBuffer.
                        playbackBuffer[appPlaybackBuffer.usbReadIdx];

                if (appPlaybackBuffer.usbReadCompleteBufferLevel == 
                    APP_QUEUING_DEPTH)
                {
                    queueFull = true;
//                    SYS_PRINT("****QUEUE Full****: RBLevel %d Ridx %d - Widx %d",
//                                appPlaybackBuffer.usbReadCompleteBufferLevel,
//                                appPlaybackBuffer.usbReadIdx,
//                                appPlaybackBuffer.codecWriteIdx);
//                    SYS_PRINT("*** QUEUE FULL****: Qlevel %d RQ %d WQ %d WC %d",
//                                appPlaybackBuffer.usbReadCompleteBufferLevel,
//                                appPlaybackBuffer.usbReadQueueCnt,
//                                appPlaybackBuffer.codecWriteQueueCnt,
//                                appPlaybackBuffer.codecWriteCompleteCnt);
                }

                if ( usbReadBuffer != NULL && 
                     !usbReadBuffer->usbInUse && 
                     !usbReadBuffer->codecInUse)
                {
                    usbReadBuffer->usbReadCompleted = false;
                    
#ifdef SAME70CULT 
                    TEST3_Toggle();
#endif //SAME70CULT
                    audioErr1 = USB_DEVICE_AUDIO_Read(USB_DEVICE_INDEX_0, 
                                          &usbReadBuffer->usbReadHandle, 
                                          1, usbReadBuffer->buffer,  
                                          appData.usbReadBufSize);//48*4 samples

                    if(audioErr1 == USB_DEVICE_AUDIO_RESULT_OK)
                    {
                        usbReadBuffer->usbInUse = true;

                        appPlaybackBuffer.usbReadQueueCnt++;
                        appPlaybackBuffer.usbReadIdx = 
                                _APP_GetNextIdx(appPlaybackBuffer.usbReadIdx);
                        
                        //DEBUG
                        usbReadScheduleCnt++;
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
                    codecWriteIdx = appPlaybackBuffer.codecWriteIdx;
                    current = &appPlaybackBuffer.playbackBuffer[codecWriteIdx];


                    if(current->usbReadCompleted && !current->codecInUse)
                    {
                        current->codecInUse = true;

#ifdef DEBUG_TONE_CODEC_TX
                        //Write stereo tone to output instead of USB data.
                        DRV_CODEC_BufferAddWrite(
                                    appData.codecClientWrite.handle, 
                                    &current->codecWriteHandle,
                                    &cpBuffer16[0],
                                    //sinBuffer16, 
                                    192); //1ms 16bit stereo buffer @ 48Khz 
#else //DEBUG_TONE_CODEC_TX
                        currentBuffer = (DRV_I2S_DATA16 *) current->buffer;

#ifdef INCLUDE_BASS_BOOST
                        if (appData.bassBoostEn == true)
                        {
                            int j;
                            //Bass Boost Processing
                            //--Separate the channels
                            //--Bass Boost each channel
                            //--Buffer the channels
#if defined(SAME70CULT)
                            for (j=0; j<48; j++)
                            {
                                leftInQ15[j]   = currentBuffer[j].leftData; 
                                rightInQ15[j]  = currentBuffer[j].rightData; 
                            }
                            arm_biquad_cascade_df1_q15(
                                    &bbLeftBiquadQ15, //const arm_biquad_casd_df1_inst_q15* 
                                    leftInQ15,  //const q15_t *pSrc,
                                    leftOutQ15, //q15_t *pDst,
                                    48);        //uint32_t blockSize
                            arm_biquad_cascade_df1_q15(
                                    &bbRightBiquadQ15, //const arm_biquad_casd_df1_inst_q15* 
                                    rightInQ15,  //const q15_t *pSrc,
                                    rightOutQ15, //q15_t *pDst,
                                    48);        //uint32_t blockSize
                            for (j=0; j<48; j++)
                            {
                                currentBuffer[j].leftData  = leftOutQ15[j];
                                currentBuffer[j].rightData = rightOutQ15[j];
                            }
#elif defined(PIC32MZEFC2)
                            //Buffer Filter Loop
                            for (j=0; j<48; j++)
                            {
                                dataInQ15 = currentBuffer[j].leftData;
                                dataValFract = gainFactorFract * (_Fract) dataInQ15;
                                currentBuffer[j].leftData = 
                                    DSP_FilterIIRBQ16_fast(
                                        dataInQ15, 
                                        ptrFilterEQL);

                                dataInQ15 = currentBuffer[j].rightData;
                                currentBuffer[j].rightData = 
                                    DSP_FilterIIRBQ16_fast(
                                        dataInQ15, 
                                        ptrFilterEQR);
                            } //End Filter Loop;
#endif //SAME70CULT/PIC32MZEFC2
                        } //End Bass Boost
#endif //INCLUDE_BASS_BOOST
#endif //DEBUG_TONE_CODEC

                        DRV_CODEC_BufferAddWrite(
                                appData.codecClientWrite.handle, 
                                &current->codecWriteHandle,
                                currentBuffer, 
                                appData.codecClientWrite.bufferSize); //48*4 bytes
                        if (current->codecWriteHandle != DRV_CODEC_BUFFER_HANDLE_INVALID)
                        {
#ifdef SAME70CULT 
                            TEST1_Toggle();
#endif //SAME70CULT
                            current->codecInUse = true;
                            
                            appPlaybackBuffer.codecWriteQueueCnt++;
                            appPlaybackBuffer.codecWriteIdx = _APP_GetNextIdx(codecWriteIdx);

                            //Check for QUEUE Empty 
                            if (appPlaybackBuffer.codecWriteIdx == appPlaybackBuffer.usbReadIdx)
                            {
                               //QUEUE is empty do not execute USB Read command;
                               queueEmpty = true;
                            }
                            else
                            {
                               queueEmpty = false;
                            }
                           
                            //DEBUG
                            codecWriteScheduleCnt++; 
                        }
                        else
                        {
                            // CODEC doesn't have enough write buffers
                            // --Wait till one becomes available
                            current->codecInUse = false;
                        }
                    } //USB Read Buffer Playback
                    else
                    {
                        asm("NOP");
                    }
                } //Check for read buffer available.
            } //End APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ
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
                //MUTE - On
#if defined(SAME70CULT)
                appData.ledState   = CONNECTED_BLINK;
#elif defined(PIC32MZEFC2)
                if (appData.bassBoostEn)
                {
                    appData.ledState = LED_BOOST_MUTE_BLINK;
                }
                else
                {
                    appData.ledState = LED_AUDIO_MUTE_BLINK;
                }
#endif //SAME70CULT/PIC32MZEFC2
                appData.blinkDelay = 0;
                appData.volumeMuteOn = true;
                DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                appData.state = APP_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting == 1)
            {
                //MUTE - Off
#if defined(SAME70CULT)
                appData.ledState = LED_ON;
#elif defined(PIC32MZEFC2)
                if (appData.bassBoostEn)
                {
                    appData.ledState = LED_BOOST_PLAY;
                }
                else
                {
                    appData.ledState = LED_AUDIO_PLAY;
                }
                appData.bassBoostEn = false;
#endif //SAME70CULT/PIC32MZEFC2
                appData.blinkDelay = 0;
                appData.volumeMuteOn = false;
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
            DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                                              appData.sampleFreq);
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
#ifdef SAME70CULT 
                TEST1_Set();
                TEST2_Set();
                TEST3_Set();
                TEST4_Set();
                TEST5_Set();
#endif //SAME70CULT
                _APP_Init_PlaybackBufferQueue();
                queueFull = false;
                queueEmpty = true;

                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
                    appData.volumeMuteOn = true;
                    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                    appData.state = APP_IDLE;
                }

#if defined(SAME70CULT)
                appData.ledState = MUTE_BLINK;
#elif defined(PIC32MZEFC2)
                if (appData.bassBoostEn)
                {
                    appData.ledState   = LED_BOOST_MUTE_BLINK;
                }
                else
                {
                    appData.ledState   = LED_AUDIO_MUTE_BLINK;
                }
#endif //SAME70CULT/PIC32MZEFC2
                appData.blinkDelay = 0;

                appData.state = APP_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting == 
                     APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);
                if (SYS_STATUS_READY == codecStatus)
                {
#if defined(SAME70CULT)
                        appData.ledState   = LED_ON;
#elif defined(PIC32MZEFC2)
                    if (appData.bassBoostEn)
                    {
                        appData.ledState   = LED_BOOST_PLAY;
                    }
                    else
                    {
                        appData.ledState   = LED_AUDIO_PLAY;
                    }
#endif //SAME70CULT/PIC32MZEFC2
                    appData.blinkDelay = 0;

                    appData.volumeMuteOn = false;
                    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                    _APP_Init_PlaybackBufferQueue();
                    appData.state =  APP_STATE_INITIAL_USB_READ_REQUEST;
                } //End codestatus
            } //End activeInterfaceAlternatesetting - Playback
        } //End case APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        break;

        //---------------------------------------------------------------------
        // APP_IDLE
        //---------------------------------------------------------------------
        case APP_IDLE:
        {
            if ((appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE))
            {
#if defined(SAME70CULT)
                //TODO:
#elif defined(PIC32MZEFC2)
                //TODO:
#endif //SAME70CULT/PIC32MZEFC2
            }
        }
        break;

        //---------------------------------------------------------------------
        // APP_STATE_ERROR - default
        //---------------------------------------------------------------------
        case APP_STATE_ERROR:
        default:
        {
#if defined(SAME70CULT)
            appData.ledState   = LED_ERROR_BLINK;
#elif defined(PIC32MZEFC2)
            appData.ledState   = LED_ERROR_BLINK;
#endif //SAME70CULT/PIC32MZEFC2
            appData.blinkDelay = 0;
            break;
        }

    } //End switch(appData.state))

    //Save state for debug of transitions
    appData.lastState = appData.state;

} //End APP_Tasks()


//******************************************************************************
// APP_Button_Tasks()
//******************************************************************************
void _APP_Button_Tasks()
{
    if (appData.buttonState != appData.lastButtonState)    
    {
        //if ((appData.buttonState != BUTTON_STATE_PRESSED) &&
        //    (appData.buttonState != BUTTON_STATE_IDLE))
        {
            SYS_DEBUG_PRINT(0,
                "%s vol=%d %s(%d) d=%d\r\n",
                buttonStr[appData.buttonState], appData.volumeIndex,
                    appData.bassBoostEn?"BOOst(":"NOboost-",
                    appData.bassBoostIndex,
                    appData.buttonDelay);
        }

        appData.lastButtonState = appData.buttonState;
    }

    //BUTTON PROCESSING
    /* Check the buttons' current state. */      
    switch ( appData.buttonState )
    {
        //Wait for Button Press
        case BUTTON_STATE_IDLE:
        {
            if ( (appData.buttonDelay==0) &&
#if defined(SAME70CULT)
                 ((SWITCH_Get()==SWITCH_STATE_PRESSED)) )
#elif defined(PIC32MZEFC2)
                 ((SWITCH_Get()==SWITCH_STATE_PRESSED) ||                
                  (SWITCH2_Get()==SWITCH2_STATE_PRESSED) ||                
                  (SWITCH3_Get()==SWITCH3_STATE_PRESSED)) )                
#endif //SAME70CULT/PIC32MZEFC2
            {
                //Pressed debounce
                appData.buttonDelay=BUTTON_DEBOUNCE;       
                appData.buttonState=BUTTON_STATE_PRESSED;               
            }
        }
        break;
        
        //Button Pressed - Debounce
        case BUTTON_STATE_PRESSED:
        { 
            //Debounce Delay
            if (appData.buttonDelay>0)
            {
                break;      //Still debouncing
            }
            
            //Still Pressed after Debounce
            if(SWITCH_Get()==SWITCH_STATE_PRESSED)  //Volume 
            {                
                //Volume Up
                appData.buttonDelay=LONG_BUTTON_PRESS;   // 1 sec is long press
                appData.buttonState=BUTTON_STATE_BUTTON1_PRESSED;                  
            }
#if defined(PIC32MZEFC2)
            else  if(SWITCH2_Get()==SWITCH_STATE_PRESSED) 
            {                
                //Bass Boost Up 
                appData.buttonDelay=LONG_BUTTON_PRESS;   // 1 sec is long press
                appData.buttonState=BUTTON_STATE_BUTTON2_PRESSED;                  
            }
            else if (SWITCH3_Get()==SWITCH_STATE_PRESSED) 
            {                
                //Not Used
                appData.buttonDelay=LONG_BUTTON_PRESS;   // 1 sec is long press
                appData.buttonState=BUTTON_STATE_BUTTON3_PRESSED;                  
            }
#endif //SAME70CULT/PIC32MZEFC2
            else
            {
                appData.buttonDelay=BUTTON_DEBOUNCE;       
                appData.buttonState = BUTTON_STATE_IDLE;
            }
        }
        break;
          
        //Short Press - change volume in 4 levels starting and 
        //              returning back to /mute on/off after level 3
        //Long Press - volume/mute mode toggle
        case BUTTON_STATE_BUTTON1_PRESSED:
        {
            // SW01 pressed and released < 1 sec after debounce
            if ((appData.buttonDelay>0) &&
                (SWITCH_Get()!=SWITCH_STATE_PRESSED) &&  
                (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ))
            {
                //SHORT PRESS - SW1
                //Update Volume
                appData.volumeIndex++;
                if (appData.volumeIndex >= VOLUME_STEPS)
                {
                    appData.volumeIndex = 0;    
                }
                appData.volume = volumeLevels[appData.volumeIndex];
                
                //VOLUME Control LED
                if (appData.volume > 0)
                {
                    appData.volumeMuteOn = false;
                    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                    DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                       DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                       appData.volume);

                    //Audio Play - LED State
                    if (appData.bassBoostEn)
                    {
#if defined(SAME70CULT)
                        appData.ledState   = LED_ON;
#elif defined(PIC32MZEFC2)
                        appData.ledState   = LED_BOOST_PLAY;
#endif //SAME70CULT/PIC32MZEFC2
                        appData.blinkDelay = 0; 
                    }
                    else
                    {
                        //Button 1 - Volume Audio No Boost
#if defined(SAME70CULT)
                        appData.ledState   = LED_ON;
#elif defined(PIC32MZEFC2)
                        appData.ledState   = LED_AUDIO_PLAY;
#endif //SAME70CULT/PIC32MZEFC2
                        appData.blinkDelay = 0; 
                    }

                }
                else
                {
                    //AUDIO MUTE LED State
                    if (appData.bassBoostEn)
                    {
#ifdef SAME70CULT
                        appData.ledState   = BOOST_BLINK;
#elif defined(PIC32MZEFC2)
                        appData.ledState   = LED_BOOST_MUTE_BLINK;
#endif //SAME70CULT/PIC32MZEFC2
                        appData.blinkDelay = 0; 
                    } //End bassBoostEn Mute
                    else
                    {
#if defined(SAME70CULT)
                        appData.ledState   = MUTE_BLINK;
#elif defined(PIC32MZEFC2)
                        appData.ledState   = LED_AUDIO_MUTE_BLINK;
#endif //SAME70CULT/PIC32MZEFC2
                        appData.blinkDelay = 0; 
                    } //End bassBoostEn Audio 
                    appData.volumeMuteOn = true;
                    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);
                } //Volume Control 

                //Set the debounce value
                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              

            } //End Button Pressed - After Debounce
            else if ((appData.buttonDelay==0) &&
                     (SWITCH_Get()==SWITCH_STATE_PRESSED) &&  
                      (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ))
            {
#if defined(SAME70CULT)
                //LONG PRESS - BOOST Enable/Disable
                appData.bassBoostEn = !appData.bassBoostEn;
                if (appData.bassBoostEn && appData.volumeMuteOn)
                {
                    appData.ledState   = MUTE_BOOST_BLINK;
                }
                else if (appData.bassBoostEn)
                {
                    appData.ledState   = BOOST_BLINK;
                }
                else if (appData.volumeMuteOn)
                {
                    appData.ledState   = MUTE_BLINK;
                }
                else
                {
                    appData.ledState   = LED_ON;
                }
#elif defined(PIC32MZEFC2)
                //Button 1 - LONG PRESS - Mute/UnMute
                appData.volumeMuteOn = !appData.volumeMuteOn;
                if (appData.volumeMuteOn)
                {
                    DRV_CODEC_MuteOn(appData.codecClientWrite.handle);

                    if (appData.bassBoostEn)
                    {
                        appData.ledState   = LED_BOOST_MUTE_BLINK;
                    }
                    else
                    {
                        appData.ledState   = LED_AUDIO_MUTE_BLINK;
                    }
                } //End Button 1 LP - Mute On
                else //volMuteOn
                {
                    //Audio - No Mute
                    DRV_CODEC_MuteOff(appData.codecClientWrite.handle);
                    DRV_CODEC_VolumeSet(appData.codecClientWrite.handle, 
                                    DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                    appData.volume);
                    if (appData.bassBoostEn)
                    {
                        appData.ledState = LED_BOOST_PLAY;
                    }
                    else
                    {
                        appData.ledState = LED_AUDIO_PLAY;
                    }
                } //End Button 1 LP - volume Mute
                appData.blinkDelay = 0; 
#endif //PIC32MZEF
                appData.buttonState=BUTTON1_STATE_WAIT_FOR_RELEASE;                
            } //End Button 1 LONG PRESS
        } //End case BUTTON_STATE_BUTTON1_PRESSED:
        break;

#ifdef PIC32MZEFC2
        //Short Press - Boost, initially at level 0, 
        //              and increasing after each short press,
        //              returning back to level 0 after level 3. 
        //Long Press  - Boost Enable Toggle
        case BUTTON_STATE_BUTTON2_PRESSED:
        {
            int __attribute__((unused))  sw2Val;
            sw2Val = SWITCH2_Get();
            if ((appData.buttonDelay>0) &&
                (SWITCH2_Get()!=SWITCH2_STATE_PRESSED) &&
                (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ))
            {
                if (appData.bassBoostEn)
                {
                    //Increase Bass Boost (in a 4 level cycle)
                    appData.bassBoostIndex++;
                    if (appData.bassBoostIndex >= NUMBBLEVELS)
                    {
                        appData.bassBoostIndex = 0;    
                    }
                    ptrFilterEQL = ptrBbCoeffsLeftQ15[appData.bassBoostIndex]; 
                    ptrFilterEQR = ptrBbCoeffsRightQ15[appData.bassBoostIndex]; 
                    gainFactorFract = gainFactorLevelsQ15[appData.bassBoostIndex];
                } //End bassBoostEn
                else
                {
                    //Enable Bass Boost if not enabled
                    appData.bassBoostEn = true;
                    appData.bassBoostIndex = 0;
                }

                //--Only LP can disable BB
                if (appData.volumeMuteOn)
                {
                    appData.ledState   = LED_BOOST_MUTE_BLINK;
                }
                else
                {
                    appData.ledState   = LED_BOOST_PLAY;
                }
                appData.blinkDelay = 0; 

                //Set the debounce value
                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              
            }
            else if ((appData.buttonDelay==0) &&
                     (SWITCH2_Get()==SWITCH2_STATE_PRESSED) && 
                     (appData.activeInterfaceAlternateSetting == 
                        APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ))
            {
                // LONG PRESS - SW2 still pressed after 1 sec
                // --> Toggle Button Mode - Bass Boost
                appData.bassBoostEn = !appData.bassBoostEn;
                if (appData.bassBoostEn)
                {
                    //Boost Enabled
                    if (appData.volumeMuteOn)
                    {
                        appData.ledState   = LED_BOOST_MUTE_BLINK;
                    }
                    else
                    {
                        appData.ledState   = LED_BOOST_PLAY;
                    }
                }
                else 
                {
                    //NO Boost
                    appData.bassBoostIndex = 0;
                    if (appData.volumeMuteOn)
                    {
                        appData.ledState   = LED_AUDIO_MUTE_BLINK;
                    }
                    else
                    {
                        appData.ledState   = LED_AUDIO_PLAY;
                    }
                }
                appData.blinkDelay = 0; 
                appData.buttonState=BUTTON2_STATE_WAIT_FOR_RELEASE;                
            }
            SYS_DEBUG_PRINT(0,"  B2end -  %s=%d %s(%d) sw-%d d=%x(%x)\r\n",
                        appData.volumeMuteOn?"Mute":"Audio-",
                        appData.volumeIndex,
                        appData.bassBoostEn?"BOOst":"NOboost",
                        appData.bassBoostIndex,
                        sw2Val, appData.buttonDelay,SWITCH2_STATE_PRESSED);
            SYS_DEBUG_PRINT(0,
                "  NS%s d=%d\r\n",
                buttonStr[appData.buttonState], appData.buttonDelay);
        } //End case BUTTON_STATE_BUTTON2_PRESSED:
        break;

        case BUTTON_STATE_BUTTON3_PRESSED:
        {
            //Do Nothing
            appData.buttonState=BUTTON3_STATE_WAIT_FOR_RELEASE;                
        }
        break;
#endif //PIC32MZEFC2

        case BUTTON1_STATE_WAIT_FOR_RELEASE:
        {
            if (SWITCH_Get()!=SWITCH_STATE_PRESSED)
            {
                //Release Debounce
                appData.buttonDelay=BUTTON_DEBOUNCE;
                appData.buttonState=BUTTON_STATE_IDLE;
            }
        }
        break;

#if defined(PIC32MZEFC2)
        case BUTTON2_STATE_WAIT_FOR_RELEASE:
        {
            if (SWITCH2_Get()!=SWITCH2_STATE_PRESSED)
            {
                //Release Debounce
                appData.buttonDelay=BUTTON_DEBOUNCE;
                appData.buttonState=BUTTON_STATE_IDLE;
            }
        }
        break;

        case BUTTON3_STATE_WAIT_FOR_RELEASE:
        {
            if (SWITCH3_Get()!=SWITCH3_STATE_PRESSED)
            {
                //Release Debounce
                appData.buttonDelay=BUTTON_DEBOUNCE;
                appData.buttonState=BUTTON_STATE_IDLE;
            }
        }
        break;
#endif //SAME70CULT/PIC32MZEFC2

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }

} //End APP_Button_Tasks()


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

            //DEBUG
            codecWriteCompleteCnt++;
            appPlaybackBuffer.codecWriteCompleteCnt++;

#ifdef SAME70CULT 
            TEST2_Toggle();
#endif //SAME70CULT
            if (appPlaybackBuffer.codecWriteIdx != appPlaybackBuffer.usbReadIdx)
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
            appPlaybackBuffer.codecWriteQueueCnt--;
            appPlaybackBuffer.usbReadCompleteBufferLevel--;

            if (appPlaybackBuffer.usbReadCompleteBufferLevel == 0)
            {
                //USB Read needs to complete before next Codec Write.
                queueEmpty = true;
//                SYS_PRINT("*** UNDERFLOW ***: QLevel %d - Ridx %d - Widx %d",
//                        appPlaybackBuffer.usbReadCompleteBufferLevel,
//                        appPlaybackBuffer.usbReadIdx,
//                        appPlaybackBuffer.codecWriteIdx);
//                SYS_PRINT("*** underFLOW ***: Qlevel %d RQ %d WQ %d WC %d",
//                        appPlaybackBuffer.usbReadCompleteBufferLevel,
//                        appPlaybackBuffer.usbReadQueueCnt,
//                        appPlaybackBuffer.codecWriteQueueCnt,
//                        appPlaybackBuffer.codecWriteCompleteCnt);
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
        if (appPlaybackBuffer.playbackBuffer[i].usbReadHandle == handle)
        {
            appPlaybackBuffer.playbackBuffer[i].usbReadCompleted = true;
            //appPlaybackBuffer.playbackBuffer[i].usbInUse = false;
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
        if(appPlaybackBuffer.playbackBuffer[i].usbReadCompleted != true)
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
    appPlaybackBuffer.codecWriteIdx = 0;
    appPlaybackBuffer.usbReadIdx = 0;

    for(i=0;i<APP_QUEUING_DEPTH;i++)
    {
        appPlaybackBuffer.playbackBuffer[i].codecInUse = false;
        appPlaybackBuffer.playbackBuffer[i].usbInUse = false;
        appPlaybackBuffer.playbackBuffer[i].usbReadCompleted = false;
    }
    appPlaybackBuffer.usbReadCompleteBufferLevel = 0;
    appPlaybackBuffer.codecWriteCompleteCnt      = 0;
    appPlaybackBuffer.usbReadCompleteCnt         = 0;
    appPlaybackBuffer.usbReadQueueCnt            = 0;
    appPlaybackBuffer.codecWriteQueueCnt         = 0;

    DRV_CODEC_WriteQueuePurge(appData.codecClientWrite.handle);

    //DEBUG
    usbNextCompleteIdx = 0;
    usbReadScheduleCnt = 0; 
    usbReadFailureCnt = 0;
    usbReadCompleteCnt = 0; 
    codecWriteScheduleCnt = 0; 
    codecWriteFailureCnt = 0;
    codecWriteCompleteCnt = 0; 
    usbNextCompleteIdx = 0;
    codecNextCompleteIdx = 0;

    //Synchronize playback to the I2S L/R channel slots.
    appData.lrSync = true;

} //End  _APP_Init_PlaybackBufferQueue()

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
        if(appPlaybackBuffer.playbackBuffer[i].codecWriteHandle == handle)
        {
            appPlaybackBuffer.playbackBuffer[i].codecWriteHandle = 
                    DRV_CODEC_BUFFER_HANDLE_INVALID;
            appPlaybackBuffer.playbackBuffer[i].codecInUse = false;
            appPlaybackBuffer.playbackBuffer[i].usbInUse = false;
            appPlaybackBuffer.playbackBuffer[i].usbReadCompleted = false;
            appPlaybackBuffer.playbackBuffer[i].usbReadHandle = 
                    USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;

            return i;
        }
    }
    
    return -1; //ERROR
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


#if defined(SAME70CULT) 
//******************************************************************************
//  APP_LED_Tasks() - ONE_BUTTON_VERSION
//******************************************************************************
void _APP_LED_Tasks()
{
    //LED1 
    if(appData.blinkDelay==0)
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
                LED1_Toggle();
                appData.blinkDelay = 1000;  
            break;

            case MUTE_BLINK:
            case MUTE_BOOST_BLINK:
                LED1_Toggle();
                appData.blinkDelay = 500;  
            break;

            case BOOST_BLINK:
                LED1_Toggle();
                appData.blinkDelay = 200;  
            break;

            case LED_ERROR_BLINK:
                LED1_Toggle();
                appData.blinkDelay = 250;  
            break;

            default:
                LED1_Off();
        } //End switch(state)
    } //End Blink Delay
} //End _APP_LED_Tasks()
#elif defined(PIC32MZEFC2)
//******************************************************************************
//  APP_LED_Tasks() - ONE_BUTTON_VERSION
//    LED_STATE_NONE = 0,
//    LED_NO_CONNECT_BLINK,    //Red Blink
//    LED_CONFIGURED,          //Red
//    LED_CONNECT_BLINK,       //Green Blink 
//    LED_AUDIO_PLAY,          //Green
//    LED_AUDIO_MUTE_BLINK,    //Green Blink
//    LED_BOOST_PLAY,          //Blue
//    LED_BOOST_MUTE_BLINK,    //Blue Blink
//    LED_ERROR_BLINK,         //White Blink
//    NUM_LED_STATES
//******************************************************************************
void _APP_LED_Tasks()
{
    if (appData.lastLedState != appData.ledState)
    {
        //New State
        SYS_DEBUG_PRINT(0,"\r\n%s v=%d %s(%d)\r\n",
                ledStr[appData.ledState], 
                appData.volumeIndex,
                appData.bassBoostEn?"BOOst":"NOboost",
                appData.bassBoostIndex);

        appData.lastLedState = appData.ledState;

    }
    else if (appData.blinkDelay != 0)
    {
        //Only enter state when it needs to change.
        return;
    }

    //Change LED State
    switch(appData.ledState)
    {
        //No USB Connection State
        case LED_NO_CONNECT_BLINK:
            LedOnStateClear();
            RGB_LED_R_Toggle();
            RGB_LED_G_Off();
            RGB_LED_B_Off();
            appData.blinkDelay = LED_CONNECT_BLINK_MS;  
        break;

        //USB Connected - No Audio stream 
        case LED_CONFIGURED:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE)
            {
                RGB_LED_R_On();
                RGB_LED_G_Off();
                RGB_LED_B_Off();
                appData.blinkDelay = 0;  
            }
        break;

        //USB Connected - No Audio stream 
        case LED_CONNECT_BLINK:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_NONE)
            {
                LED1_Toggle();
                RGB_LED_R_Off();
                RGB_LED_G_Toggle();
                RGB_LED_B_Off();
                appData.blinkDelay = LED_CONNECT_BLINK_MS;  
            }
        break;

        //Audio Streaming
        case LED_AUDIO_PLAY:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                LED1_On();
                RGB_LED_R_Off();
                RGB_LED_G_On();
                RGB_LED_B_Off();
                appData.blinkDelay = 0;  
            }
        break;

        //Audio Streaming Muted
        case LED_AUDIO_MUTE_BLINK:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                RGB_LED_R_Off();
                RGB_LED_G_Toggle();
                RGB_LED_B_Off();
                appData.blinkDelay = LED_MUTE_BLINK_MS;  
            }
        break;

        //Audio Streaming with Boost
        case LED_BOOST_PLAY:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                LED2_On();
                RGB_LED_R_Off();
                RGB_LED_G_Off();
                RGB_LED_B_On();
                appData.blinkDelay = 0;  
            }
        break;

        //Audio Streaming with Boost - Muted
        case LED_BOOST_MUTE_BLINK:
            if (appData.activeInterfaceAlternateSetting == 
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                LED1_Toggle();
                LED2_Toggle();
                RGB_LED_R_Off();
                RGB_LED_G_Off();
                RGB_LED_B_Toggle();
                appData.blinkDelay = LED_MUTE_BLINK_MS;  
            }
        break;

        case LED_ERROR_BLINK:
            LED1_Toggle();
            LED2_Toggle();
            LED3_Toggle();
            RGB_LED_R_Toggle();
            RGB_LED_G_Toggle();
            RGB_LED_B_Toggle();
            appData.blinkDelay = LED_ERROR_BLINK_MS;  
        break;

        default:
            LED1_Off();
            LED2_On();
            LED3_Off();
            RGB_LED_R_On();
            RGB_LED_G_On();
            RGB_LED_B_On();
            appData.blinkDelay = LED_ERROR_BLINK_MS;  
    } //End ledState 

} //End _APP_LED_Tasks()
#endif //PIC32MZEFC2/SAME70CULT

#ifdef PIC32MZEFC2
void pic32_parmEqFilter_init_q15(
        PARM_EQUAL_FILTER * ptrFiltEQ15,  
        int16_t * bbCoeffsPic32Q15)
{
    //Bass Boost filter coefficients
    //{   b0      b1     b2       a1      a2   }
    //{ 0x1008, 0xe03d, 0x0fc5, 0x80ee, 0x3f2b };
    // note all coefs are half value of original design, gain handled in algorithm
    // note subtract is handled in algorithm, so coefs go in at actual value
    ptrFiltEQ15->b[0]=bbCoeffsPic32Q15[0]; //feedforward b0
    ptrFiltEQ15->b[1]=bbCoeffsPic32Q15[1]; //feedforward b1
    ptrFiltEQ15->b[2]=bbCoeffsPic32Q15[2]; //feedforward b2
    ptrFiltEQ15->a[0]=bbCoeffsPic32Q15[3]; //feedback a1 coef
    ptrFiltEQ15->a[1]=bbCoeffsPic32Q15[4]; //feedback a2 coef

    //Filter State ???? feed forward delay
    ptrFiltEQ15->Z[0]=0;
    ptrFiltEQ15->Z[1]=0;

    ptrFiltEQ15->G.fracGain = 0x4000;   // Filter max gain multiplier
    ptrFiltEQ15->G.expGain  = 1;        // Filter max gain multiplier
    ptrFiltEQ15->log2Alpha  = 1;        // coefficient scaling bit shift value
}
#endif //PIC32MZEFC2


#if defined(PIC32MZEFC2)
void LedOnStateClear()
{
	RGB_LED_R_Off();
	RGB_LED_G_Off();
	RGB_LED_B_Off();
	LED3_Off();
	LED2_Off();
	LED1_Off();
    appData.blinkDelay = 0;
}
#endif //SAME70CULT/PIC32MZEFC2

/*******************************************************************************
 End of File
 */
