/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c for usb_microphone

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
Copyright (c) 2013-2019 released Microchip Technology Inc.  All rights reserved.
 ******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app.h"

//#include "display.h"

//=============================================================================
//DEBUG
//
//DEBUG_TONE_INJECT:  Send tone buffer to USB and to CODEC Playback, using
//                    queue timing. 
#undef DEBUG_TONE_INJECT

#if defined(DEBUG_TONE_INJECT)
uint32_t Debug_var;

//Stereo sine test tone 
//NOTE:  USB requires 32 sample(stereo) frame buffers. 

//uint16_t scaleQ1d15 = 0x4000; //Scale multiplier .5
uint16_t scaleQ1d15 = 0x2000; //Scale multiplier .25

//Sin Tone Table
//NOTE:  DRV_I2S_DATA16 has .leftData and .rightData 
//       members, each uint16_t.
//       Format of initializer:
//           { { valLeft0, valRight0 }, { ... }, ... {valLeftN, valRightN}, }
//       Only using 1 channel of the tone data.
//NOTE:  USB using 192 byte (96 word) buffers
DRV_I2S_DATA16 sinToneQ1d15[]=
{
//2 cycle of 1Khz tone stereo @ 16Khz sampling
//-- 16*2 = 32*2 samples
//#include "tone_1000Hz_2Cy_SQ_16bit_16Khz.dat" //2cycles @ 16Khz-> 32 samples    
#include "tone_1000Hz_16bit_16Khz.dat" 
};

uint16_t sinSampleCount = sizeof(sinToneQ1d15)/sizeof(DRV_I2S_DATA16); //32
#define TONE_SAMPLES_PER_CYCLE 16  //Samples are 16bit stereo
#define TONE_NUMBER_OF_CYCLES   1
#define TONE_BUFFER_SIZE_BYTES  (TONE_SAMPLES_PER_CYCLE*4)
//Make toneBuffer same size as rxBuffer (16)
int16_t toneBuffer[TONE_SAMPLES_PER_CYCLE];
#endif //DEBUG_TONE_INJECT
//END DEBUG
//=============================================================================


//CODEC read data (mic data) buffer. 
//I2S Clock Settings:   LRCK(16Khz) BCLK(1.024Mhz) MCLK(4.096Mhz) 
//--ping-pong (2) buffers/32 16bit stereo samples {leftData, rightData}
//--Q1.15 data/2channels
//
// DRV_I2S_DATA16 { .leftData .rightData } uint16_t each channel
//
// NOTE: CODEC set as mono --> despite SPI_AUDIO_TRANSMIT_STEREO MHC selection
//       Data received as pseudo stereo at 16Khz
//
//  16*4 = 96 bytes (CODEC)      16*2=32 bytes (USB)
#define USB_FRAME_SIZE_MS   1000
#define APP_CODEC_FRAMESIZE_SAMPLES  APP_USB_STREAM_SAMPLE_RATE/USB_FRAME_SIZE_MS  
#define APP_CODEC_BYTES_PER_SAMPLE 4              //Stereo samples 
#define APP_CODEC_BUFFER_SIZE_BYTES  APP_CODEC_FRAMESIZE_SAMPLES*APP_CODEC_BYTES_PER_SAMPLE
#define APP_USB_FRAMESIZE_SAMPLES APP_CODEC_FRAMESIZE_SAMPLES
#define APP_USB_BYTES_PER_SAMPLE 2
#define APP_USB_FRAMESIZE_BYTES   APP_USB_FRAMESIZE_SAMPLES*APP_USB_BYTES_PER_SAMPLE 

//Codec Rx Buffer - pseudo stereo
// NOTE: Cache coherency and 16 byte alignment required for MZ processor,
//       -->as the cache page size on PIC32MZ is 16 bytes.
//       You don?t want to run into an issue where linker allocates the data 
//       structure in the same page as another data structure and then a line 
//       flush causes coherency issues.
DRV_I2S_DATA16 APP_MAKE_BUFFER_DMA_READY 
    rxBuffer[APP_QUEUING_DEPTH][APP_CODEC_FRAMESIZE_SAMPLES]; //16 pseudo stereo samples
DRV_I2S_DATA16 APP_MAKE_BUFFER_DMA_READY 
    zeroBuffer[APP_CODEC_FRAMESIZE_SAMPLES] = {0}; //16 0 samples

//USB Tx Buffer
//1 channel mic Buffer
int16_t _micBuffer[APP_USB_FRAMESIZE_SAMPLES];  //16 mono samples

//Button Timer
DRV_HANDLE tmrHandle;

//USB Tx Buffer for Mono Upsampled Data 
//--32 Q1.15 values
//
//NOTE:  Make the APP_USB_FRAMESIZE_BYTES the same as the TONE_BUFFER_SIZE_BYTES
int16_t APP_MAKE_BUFFER_DMA_READY   \
    usbTxBuffer1[APP_USB_FRAMESIZE_SAMPLES];  //16
int16_t APP_MAKE_BUFFER_DMA_READY   \
    usbTxBuffer2[APP_USB_FRAMESIZE_SAMPLES];  //16

uint32_t usbWriteError=0;
uint8_t  muteonflag = 0;

//NOTE:  Mute/Max gain levels
const uint8_t micGainLevels[GAIN_STEPS] =
{
    0 /* off */, 255
};

void __attribute__((unused)) _APP_DiscardChannelFromMicrophone(DRV_I2S_DATA16 *input, 
                                              int16_t *output, 
                                              int numSamples);
void __attribute__((unused)) _APP_CombineChannelsFromI2S(DRV_I2S_DATA16 *input, 
                                              int16_t *output, 
                                              int numSamples);

void _APP_Button_Tasks(void);
void _APP_LED_Tasks(void);
void _APP_TimerCallback(uintptr_t context);


APP_DATA appData =
{
    // Device Layer Handle
    .usbDevHandle = -1,

    // USB Audio Instance index for this app object 0
    .audioInstance = 0,

     /* app state */
    .state = APP_STATE_CODEC_OPEN,

    /* device configured status */
    .isConfigured = false,

    .codecConfigured = false,

    /* Initialize active interface setting to 0. */
    .activeInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE,

    // DAC is not muted initially
    .dacMute = false,     //USB Mute command
    .ledState = LED_OFF,
    
    .blinkDelay = 200,

    .sampleFreq = APP_USB_STREAM_SAMPLE_RATE,    
    
    //Initialize Mic Settings
    .midCtrl  = 0x0F,
    .midMax   = 0x3F,
    .midMin   = 0xF0,
    .midRes   = 0x04,
    .autoGain = true,
    .micGainIndex = 1,
    .micGain      = 200,

    .codecClient.context = (uintptr_t)0,
    .codecClient.bufferHandler = (DRV_CODEC_BUFFER_EVENT_HANDLER) APP_CODECBufferEventHandler,
    .codecClient.bufferSize = APP_CODEC_BUFFER_SIZE_BYTES, //sizeof(rxBuffer[0]),
    .codecClient.rxBufIdx = 0,
    .codecClient.txBufIdx = 1,
    .codecClient.isCodecReadComplete = false,

    // This is to indicate which Audio Control Transfer is currently
    // in progress. */
    // --No Audio control in progress.
    .currentAudioControl = APP_USB_CONTROL_NONE,

    //CODEC Read Buffer Available after USB write
    .codecClient.isReadBufHandleAvailable[0] = true,
    .codecClient.isReadBufHandleAvailable[1] = true,
            
    //KEEP THIS
    //CODEC Read Buffer Handle
    .codecClient.readBufHandle[0] = DRV_CODEC_BUFFER_HANDLE_INVALID,
    .codecClient.readBufHandle[1] = DRV_CODEC_BUFFER_HANDLE_INVALID,

    .lrSync       = true,

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
            break;

        case USB_DEVICE_EVENT_DECONFIGURED:
            // USB device is reset or device is de-configured.
            // This means that USB device layer is about to de-initialize
            // all function drivers. So close handles to previously opened
            // function drivers.


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
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            appData.ledState = LED_OFF;
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS was removed. Notify USB stack about the event*/
            USB_DEVICE_Detach (appData.usbDevHandle);
            appData.ledState = LED_OFF;
        case USB_DEVICE_EVENT_ERROR:
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
    //volatile USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *readEventData;
    USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE *writeEventData;
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

                appData.activeInterface = interfaceInfo->interfaceNumber;
                if(appData.activeInterface == 1)
                {
                    appData.activeInterfaceAlternateSettingMic = 
                            interfaceInfo->interfaceAlternateSetting;
                    appData.state = 
                            APP_STATE_MIC_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
            {
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
            {
                writeEventData = 
                        (USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE *)pData;
                //Audio frame was written to the Host.
                //Now get next frame from Audio Codec.
                if (writeEventData->handle == 
                        appData.writeTransferHandle[0])
                    

                {
#ifdef DEBUG_TEST_POINTS
                    TEST4_Set();
#endif
                    appData.isUsbWriteComplete[0] = true;
                    appData.codecClient.isReadBufHandleAvailable[0] = true;
                    //appData.state = APP_STATE_PROCESS_DATA;
                    
                }
                else if(writeEventData->handle == 
                        appData.writeTransferHandle[1])
                {
#ifdef DEBUG_TEST_POINTS
                    TEST4_Clear();
#endif
                    appData.isUsbWriteComplete[1] = true;
                    appData.codecClient.isReadBufHandleAvailable[1] = true;
                    //appData.state = APP_STATE_PROCESS_DATA;
                }
                else
                {
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR:
            {
                entityID = 
                    ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT)
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
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_CUR:
            {
                entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)
                                pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT)
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
                    }
                }

                if (entityID == APP_ID_FEATURE_UNIT_MIC)
                {
                    controlSelector = 
                        ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)pData)->controlSelector;
                    if (controlSelector == USB_AUDIO_MID_CONTROL)
                    {
                       /*Handle Get request*/
                       USB_DEVICE_ControlSend(appData.usbDevHandle, 
                            (void *)&(appData.midCtrl), 1 );
                    }
                    if (controlSelector == USB_AUDIO_AUTOMATIC_GAIN_CONTROL)
                    {
                       /*Handle Get request*/
                       USB_DEVICE_ControlSend(appData.usbDevHandle,
                           (void *)&(appData.autoGain), 1 );
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MIN:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MIN:
            {
                entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT_MIC)
                {
                    controlSelector = 
                        ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                            pData)->controlSelector;
                    if (controlSelector == USB_AUDIO_MID_CONTROL)
                    {
                        /*Handle Get request*/
                        USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                              (void *)&(appData.midMin), 1 );
                    }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MAX:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MAX:
                entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT_MIC)
                {
                   controlSelector = 
                        ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                            pData)->controlSelector;
                   if (controlSelector == USB_AUDIO_MID_CONTROL)
                   {
                       /*Handle Get request*/
                       USB_DEVICE_ControlSend(appData.usbDevHandle, 
                            (void *)&(appData.midMax), 1 );
                   }
                }
                break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_RES:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_RES:
            {
                entityID = ((USB_AUDIO_CONTROL_INTERFACE_REQUEST*)
                                pData)->entityID;
                if (entityID == APP_ID_FEATURE_UNIT_MIC)
                {
                   controlSelector = 
                        ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST*)
                            pData)->controlSelector;
                   if (controlSelector == USB_AUDIO_MID_CONTROL)
                   {
                       /*Handle Get request*/
                       USB_DEVICE_ControlSend(appData.usbDevHandle, 
                                              (void *)&(appData.midRes), 1 );
                   }
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_ENTITY_GET_MEM:
            {
                /* Stall request */
                USB_DEVICE_ControlStatus(
                        appData.usbDevHandle, 
                        USB_DEVICE_CONTROL_STATUS_ERROR);
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            {
                USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK );
                if (appData.currentAudioControl == APP_USB_AUDIO_MUTE_CONTROL)
                {
                    appData.state = APP_STATE_MUTE_AUDIO_PLAYBACK;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                    //Handle Mute Control Here.
                }
            }
            break;

            case  USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

            default:
                assert(0);
            break;

        } //End switch (event)
    }//end of if  if ( iAudio == 0 )
} // APP_USBDeviceAudioEventHandler()


//******************************************************************************
// APP_Initialize() - USB Playback and Record
//
// APP Initialize. It is last item called from the MH 
// generated SYS_Initialize() function (called main() before the polling loop)
//******************************************************************************
void APP_Initialize()
{
    printf("----------------------------------------");
    printf("- Starting:");
    printf("----------------------------------------");

    
    //TX Buffer USB write completed
    appData.isUsbWriteComplete[0] = false;  
    appData.isUsbWriteComplete[1] = false;

    /* Write transfer handle for each buffer*/
    appData.writeTransferHandle[0] = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID,
    appData.writeTransferHandle[1] = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID,
            
    //appData.activeInterfaceAlternateSettingMic = APP_USB_MIC_RECORD_NONE,
    
    //Stereo Codec Buffer
    appData.codecClient.rxbufferObject      = (uint8_t *) rxBuffer[0];
    appData.codecClient.txbufferObject      = (uint8_t *) rxBuffer[1];
    appData.codecClient.bufferSize          = APP_CODEC_BUFFER_SIZE_BYTES;
    appData.codecClient.isCodecReadComplete = false; 

    //RX buffer available for Codec Read
    appData.codecClient.isReadBufHandleAvailable[0] = true; 
    appData.codecClient.isReadBufHandleAvailable[1] = true;

    appData.state = APP_STATE_INIT;
    appData.ledState = LED_OFF,
    appData.buttonDelay = 0;
    appData.blinkDelay = 0;

    appData.lrSync = true;

    //DEBUG
#if defined(DEBUG_TONE_INJECT)

    //Tone Buffer for mono usb output 
    int i;
    int16_t valueQ1d15;

    assert(TONE_SAMPLES_PER_CYCLE==APP_CODEC_FRAMESIZE_SAMPLES);

    for (i=0; i<(TONE_SAMPLES_PER_CYCLE); i++)
    {
         //Scaled stereo values
        valueQ1d15 = sinToneQ1d15[i].leftData;
        toneBuffer[i]  = (valueQ1d15 ); 
    }

    //Tone buffer for stereo USB Tx
    for (i=0; i<TONE_SAMPLES_PER_CYCLE; i++)
    {
        //Scaled Mono values
         usbTxBuffer1[i] = sinToneQ1d15[i].rightData;
         usbTxBuffer2[i] = sinToneQ1d15[TONE_SAMPLES_PER_CYCLE+i].rightData;
    }

#endif //DEBUG_TONE_INJECT
#if defined(USE_DISPLAY)
    //display_init(&DISPLAY_STATS);
#endif


} //End APP_Initialize()


//******************************************************************************
// APP_Tasks()
// Application tasks routine.  Last item executed in MH generated SYS_Tasks() 
// in the main() polling loop.
//******************************************************************************
void APP_Tasks()
{
    USB_DEVICE_AUDIO_RESULT audioErr;

    _APP_Button_Tasks();
    _APP_LED_Tasks();
    
    switch(appData.state)
    {
        //---------------------------------------------------------------------
        // Open Timer
        // Open CODEC and set buffer handler
        // --Wait until: SYS_STATUS_READY 
        // Open USB and wait for the device to configure 
        // --Transition from: APP_STATE_WAIT_FOR_CONFIGURATION
        //   to Idle State
        // Idle State
        // --Transition to Initial Codec Reads from Alternate Setting 1
        // Initial Codec Reads
        // USB Writes/Codec Reads
        // --Transiton back to Idls on Alternate Setting 0
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

        //----------------------------------------------------------------------
        // Open CODEC for Mic Data Read
        //----------------------------------------------------------------------
        case APP_STATE_CODEC_OPEN:
        {
            //Open the codec client
            SYS_STATUS codecStatus;
            codecStatus = DRV_CODEC_Status(sysObjdrvCodec0);

            if (SYS_STATUS_READY == codecStatus)
            {
                // This means the driver can now be be opened.
                /* A client opens the driver object to get an Handle */
                printf("APP_STATE:  CODEC Open");
                appData.codecClient.handle = 
                                    DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                                                   DRV_IO_INTENT_READWRITE | 
                                                   DRV_IO_INTENT_EXCLUSIVE); 

                //TODO:  Determine the current USB TX rate/mode (16Khz mono read),
                //       i.e. starting in record mode.  Switch to playback mode
                //       after recording and determine the playback rate for the 
                //       CODEC. 
                if(appData.codecClient.handle != DRV_HANDLE_INVALID)
                {
                    printf("APP_STATE:  CODEC Opened");
                    DRV_CODEC_MuteOff( appData.codecClient.handle );
                    DRV_CODEC_MicGainSet(appData.codecClient.handle, 
                                         appData.micGain);

                    //Playback Volume
                    DRV_CODEC_VolumeSet(appData.codecClient.handle, 
                                        DRV_CODEC_CHANNEL_LEFT_RIGHT, 200);

                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                }
                else
                {
                    printf("Codec Client Handle Invalid\r\n");
                }
            }
            else
            {
                /* Wait for CODEC to Initialize */
                ;
            }
        } //ENDcase APP_STATE_CODEC_OPEN:
        break;

        //---------------------------------------------------------------------
        // Set a handler for the audio buffer completion event 
        // --Then APP_STATE_USB_DEVICE_OPEN
        //---------------------------------------------------------------------
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {
            DRV_CODEC_BufferEventHandlerSet(
                        appData.codecClient.handle,
                        appData.codecClient.bufferHandler,
                        appData.codecClient.context);

            //REQUIRED:  MHC desired sampling rate is not the same as the rate
            //           set by the I2S clock rate settings, unless sampling 
            //           rate is set to change the I2S clock.

            //Fixed Sample Rate
            DRV_CODEC_SamplingRateSet(appData.codecClient.handle,
                                      appData.sampleFreq);

            //NOTE: MIC IN 
            //DRV_CODEC_StereoMicSelect(appData.codecClient.handle, MIC_SELECT);
            //NOTE: LINE-IN
            //DRV_CODEC_StereoMicSelect(appData.codecClient.handle, MIC2);

            appData.codecConfigured = true;

            printf("CODEC:  Configured Set Buffer and Sampling Rate(%d)", 
                    DRV_CODEC_AUDIO_SAMPLING_RATE);

            appData.state = APP_STATE_USB_DEVICE_OPEN;
        } //END APP_STATE_CODEC_SET_BUFFER_HANDLER
        break;

        //---------------------------------------------------------------------
        // Open USB Device Client
        // --Then APP_STATE_WAIT_FOR_CONFIGURATION
        //---------------------------------------------------------------------
        case APP_STATE_USB_DEVICE_OPEN:
        {
            /* Open the USB device layer  as R/W */
            appData.usbDevHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0,
                                                    DRV_IO_INTENT_READWRITE );

            if(appData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event 
                 * notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.usbDevHandle, 
                                           APP_USBDeviceEventHandler, 0);

                printf("APP_STATE: USB Device Opened");
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                //GFX_MENU_DRAW();
            }
            else
            {
                // The Device Layer is not ready to be opened. We should try
                // again later.
                appData.state = APP_STATE_USB_DEVICE_OPEN;
            }
        } //End APP_STATE_USB_DEVICE_OPEN
        break;
            
        //---------------------------------------------------------------------
        // Wait for USB Connection to be configured
        // --then APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE
        //---------------------------------------------------------------------
        case APP_STATE_WAIT_FOR_CONFIGURATION:
        {
            //Check if Host has configured the Device.
            if (appData.isConfigured == true)  //USB Configured
            {
                appData.ledState = CONNECTED_BLINK;
                printf("APP_STATE: USB Configured");
                //appData.state = APP_STATE_CODEC_OPEN;
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE;
            }
        } //End APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE
        break;

        //---------------------------------------------------------------------
        // Wait for the USB Audio Stream to start
        // (NEW FOR H3)
        // --then APP_STATE_INITIAL_READ_REQUEST
        //---------------------------------------------------------------------
        case APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE:
        {
            if (appData.activeInterfaceAlternateSettingMic == 
                APP_USB_MIC_RECORD_MONO)
            {
                appData.ledState = CONNECTED_BLINK;
                //NOTE: CODEC may not have been opened before USB sets interface
                if (appData.codecConfigured)
                {
                    appData.state = APP_STATE_INITIALIZE_CODEC_QUEUE;
                }
            }
        }
        break; 

        //----------------------------------------------------------------------
        //  Initialize Codec Read/Write Queue 
        //----------------------------------------------------------------------
        case APP_STATE_INITIALIZE_CODEC_QUEUE:
        {
#ifdef DEBUG_TEST_POINTS
            TEST4_Clear();
            TEST3_Clear();
            TEST2_Clear();
            TEST1_Clear();
#endif //DEBUG_TEST_POINTS
            printf("APP_STATE_INITIAL_READ_REQUEST");
            if (appData.activeInterfaceAlternateSettingMic == 
                APP_USB_MIC_RECORD_MONO)
            {
                //Flush Queue
                DRV_CODEC_WriteQueuePurge(appData.codecClient.handle);
                DRV_CODEC_ReadQueuePurge(appData.codecClient.handle);

                appData.lrSync = false; //I2S not synced to LR Clock

                //No current codec read data to write to USB
                appData.codecClient.isCodecReadComplete = false; 


                appData.state = APP_STATE_CODEC_READ;

            } //End APP_USB_RECORD_MONO Block
            else
            {
                //Wait for Stream IO
                appData.state = APP_STATE_IDLE;
            }
        } //END case APP_STATE_INITIAL_READ_REQUEST
        break;


        //----------------------------------------------------------------------
        //  Codec Read/USB Write
        //  --Codec Loopback Write
        //----------------------------------------------------------------------
        case APP_STATE_CODEC_READ:
        {
            if (appData.activeInterfaceAlternateSettingMic == 
                APP_USB_MIC_RECORD_MONO)
                //      Fix Random LR channel swaps.
            {

#ifdef DEBUG_TEST_POINTS
                TEST2_Set();
#endif //DEBUG_TEST_POINTS

                //STUB: Check if USB Write pending on current RX buffer

                //L/R I2S Channel Sync 
                if(appData.lrSync == true)
                {
                    appData.lrSync = 
                        DRV_CODEC_LRCLK_Sync(
                                appData.codecClient.handle);
                    if (appData.lrSync == false)
                    {
                        //Did not sync to I2S channel clock
                        appData.state = APP_STATE_ERROR;
                        break;  //FAIL
                    }
                //TODO: Reset the I2SC1 port to initiate the initial transfer
                //      on the correct clock polarity.
                }

                //Loopback Codec Write/Read
                DRV_CODEC_BufferAddWriteRead(
                        appData.codecClient.handle, 
                        &appData.codecClient.
                            readBufHandle[appData.codecClient.rxBufIdx],
#ifdef DEBUG_TONE_INJECT
                        sinToneQ1d15,
#else
                        appData.codecClient.txbufferObject, //TX
#endif
                        appData.codecClient.rxbufferObject, //RX
                        appData.codecClient.bufferSize);

                if (appData.codecClient.
                        readBufHandle[appData.codecClient.rxBufIdx] != 
                    DRV_CODEC_BUFFER_HANDLE_INVALID)
                {

                    //----------------------------------
                    //USB TX Buffer Write Queueing
                    //--Check if TX buffer ready to write
                    //-->Write txBuffer to USB
                    //-->Initial buffer is set to 0
                    //-->Overflow/Underflow Check
                    
                    if (appData.codecClient.isCodecReadComplete == true) 
                    {
                        //Read(Mic) Buffer 1 Complete
                        //Buffer 1 Codec read has completed
                        appData.codecClient.isCodecReadComplete = false; 

                        //USB TX MIC DATA - MONO 16Khz
                        //Remove duplicate microphone channel
                        _APP_CombineChannelsFromI2S(
                                (DRV_I2S_DATA16 *)
                                    appData.codecClient.txbufferObject,
                                _micBuffer, 
                                APP_CODEC_FRAMESIZE_SAMPLES);

                        /* Write CODEC data to USB from rxBuffer[0]*/
#ifdef DEBUG_TEST_POINTS
                        TEST1_Set();
#endif //DEBUG_TEST_POINTS
                        audioErr = 0;
                        audioErr = USB_DEVICE_AUDIO_Write(
                                       USB_DEVICE_INDEX_0, 
                                       &appData.
                                           writeTransferHandle
                                               [appData.codecClient.txBufIdx], 
#ifdef DEBUG_TONE_INJECT
                                       1 , toneBuffer,  //16Khz Mono
#else
                                       1 , _micBuffer,  //16Khz Mono
#endif
                                       APP_USB_FRAMESIZE_BYTES);

                        if (audioErr != USB_DEVICE_AUDIO_RESULT_OK)
                        {
                            //usbWriteError++;
                            appData.state = APP_STATE_ERROR;
                            break;  //FAIL
                        }
                        //Buffer being used by USB
                        //--USB Write pending on buffer
                        appData.codecClient.
                            isReadBufHandleAvailable
                                [appData.codecClient.txBufIdx] = 
                                   false;
                        appData.isUsbWriteComplete
                                [appData.codecClient.txBufIdx] = 
                                   false;

                    //} //End USB Write TX Buffer 
                    } //End Codec Read Complete

                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                } //End Codec WR Handle Valid
                else
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            } //End APP_USB_RECORD_MONO block
            else
            {
                //Wait for Stream IO
                appData.state = APP_STATE_IDLE;
            }
        } //End case APP_STATE_CODEC_READ
        break;


        //----------------------------------------------------------------------
        // Wait for Codec Buffer Complete
        //----------------------------------------------------------------------
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
        {
            if (appData.activeInterfaceAlternateSettingMic == 
                APP_USB_MIC_RECORD_MONO)
            {
                //Wait for read complete after Codec WriteRead
                if ( appData.codecClient.isCodecReadComplete)
                {
                    appData.state = APP_STATE_CODEC_READ;
                }
            }
            else
            {
                //Wait for Stream IO
                appData.state = APP_STATE_IDLE;
            }
        }
        break;


        //----------------------------------------------------------------------
        // Mute the Audio Codec which USB interface changes.
        //
        // Initiated from :
        //   APP_USBDeviceAudioEventHandler())
        //   --Event: USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
        // 
        // Goes to:
        //    APP_STATE_INITIAL_READ_REQUEST while changing the codec interface.
        //    APP_STATE_IDLE when complete
        //----------------------------------------------------------------------
        case APP_STATE_MUTE_AUDIO_PLAYBACK:
        {
            printf("APP_STATE_MUTE_AUDIO_PLAYBACK");
            if (appData.activeInterfaceAlternateSetting == 0)
            {
                //NOTE: CODEC may not be ready before USB 
                DRV_HANDLE handle = appData.codecClient.handle;
                if(handle == DRV_HANDLE_INVALID || (DRV_HANDLE)NULL == handle)
                {
                //Mute until USB interface set.
                DRV_CODEC_MuteOn(appData.codecClient.handle);
                }
                appData.ledState = MUTE_BLINK; 
                appData.state = APP_STATE_IDLE;
            }
            else if (appData.activeInterfaceAlternateSetting == 1)
            {
                //NOTE: CODEC may not be ready before USB sets the interface 
                DRV_HANDLE handle = appData.codecClient.handle;
                if(handle == DRV_HANDLE_INVALID || (DRV_HANDLE)NULL == handle)
                {
                    
                    if (appData.codecConfigured)
                    {
                        DRV_CODEC_MuteOff(appData.codecClient.handle);
                        appData.ledState = LED_ON;
                        appData.state = APP_STATE_INITIALIZE_CODEC_QUEUE;
                    }
                    else
                    {
                       printf("Codec Not Configured to Mute\r\n");
                    }
                }
                printf("USB: aiasetting=1 MUTE Off");
            }
        } //End APP_STATE_MUTE_AUDIO_PLAYBACK
        break;

        //----------------------------------------------------------------------
        // Set the Mic interface Record configuraton APP_USB_RECORD_MONO_16KHZ
        // 
        //
        // Initiated by:
        //    APP_USBDeviceAudioEventHandler())
        //    --Event: USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED:
        //
        // Goes to:
        //    APP_STATE_INITIAL_READ_REQUEST - start mic recording
        //    APP_STATE_IDLE when complete
        //----------------------------------------------------------------------
        case APP_STATE_MIC_USB_INTERFACE_ALTERNATE_SETTING_RCVD:
        {
           if (appData.activeInterfaceAlternateSettingMic == APP_USB_MIC_RECORD_NONE)
            {
                //printf("APP_STATE_MIC_USB...RCVD: APP_USB_MIC_NONE");
            //Initialize and idle
                //NOTE: CODEC may not be ready before USB 
                DRV_HANDLE handle = appData.codecClient.handle;
                if(handle == DRV_HANDLE_INVALID || (DRV_HANDLE)NULL == handle)
                {
                    DRV_CODEC_MuteOn(appData.codecClient.handle);
                }
                
                appData.codecClient.isCodecReadComplete = false;

                appData.isUsbWriteComplete[0] = false;
                appData.isUsbWriteComplete[1] = false;

                //RX Buffer Ready for Read
                appData.codecClient.isReadBufHandleAvailable[0] = true;
                appData.codecClient.isReadBufHandleAvailable[1] = true;
                
                //CODEC should already be configured for 16Khz/16bit pseudostereo
                appData.ledState = CONNECTED_BLINK; //Connected, No Stream
                appData.state = APP_STATE_IDLE;
            }
            else if(appData.activeInterfaceAlternateSettingMic == 
                    APP_USB_MIC_RECORD_MONO)
            {
                if (appData.codecConfigured)
                {
                    printf("RCVD: APP_USB_RECORD_MONO_16Khz");

                    //NOTE: CODEC may not be ready before USB 
                    DRV_HANDLE handle = appData.codecClient.handle;

                    if (handle == DRV_HANDLE_INVALID || 
                        (DRV_HANDLE)NULL == handle)
                    {
                        //??????
                        
                        //DRV_CODEC_MicrophoneTypeSet(
                        //    appData.codecClient.handle, 
                        //    DRV_CODEC_MICROPHONE_TYPE_EXTERNAL);
                        //Check the schematics to see which MIC 
                        //input channel is available
                        //
                        //DRV_CODEC_MicrophoneSoundSet(
                        //    appData.codecClient.handle, 
                        //    MONO_RIGHT_CHANNEL);
                        //appData.state =  APP_STATE_INITIAL_READ_REQUEST;

                        DRV_CODEC_MuteOff(appData.codecClient.handle);
                        //DRV_CODEC_MicGainSet(appData.codecClient.handle, 
                        //                     appData.micGain);
                        DRV_CODEC_VolumeSet(appData.codecClient.handle, 
                                            DRV_CODEC_CHANNEL_LEFT_RIGHT, 200);
                    }
                    else
                    {
                        appData.state =  APP_STATE_ERROR;
                    }

                    appData.ledState = LED_ON;  //Connected with Stream
                    appData.state =  APP_STATE_INITIALIZE_CODEC_QUEUE;
                }
                else
                {
                   printf("Codec Not Configured-Mute!!!\r\n");
                }
            }
        } //END APP_STATE_MIC_USB_INTERFACE_ALTERNATE_SETTING_RCVD
        break;

        //----------------------------------------------------------------------
        //  Do Nothing.
        //----------------------------------------------------------------------
        case APP_STATE_IDLE:
        {
            if (appData.activeInterfaceAlternateSettingMic == APP_USB_MIC_RECORD_NONE)
            {
                if(muteonflag==0)
                {
                    //NOTE: CODEC may not be ready before USB 
                    DRV_HANDLE handle = appData.codecClient.handle;
                    if(handle == DRV_HANDLE_INVALID || (DRV_HANDLE)NULL == handle)
                    {
                    DRV_CODEC_MuteOn(appData.codecClient.handle);
                    muteonflag = 1;
                    appData.ledState = MUTE_BLINK;
                    }
                }
            }
        } //END APP_STATE_IDLE
        break;

        //----------------------------------------------------------------------
        //  Error/Fault/Default
        //----------------------------------------------------------------------
        case APP_STATE_ERROR:
        default:
        {
            appData.ledState = LED_ERROR_BLINK;
        }

    } //End switch(appData.state)

} //End APP_Tasks()

//******************************************************************************
// APP_Button_Tasks()
//******************************************************************************
void _APP_Button_Tasks()
{
   //BUTTON PROCESSING
    /* Check the buttons' current state. */      
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
          
        //Short Press - change mute on/off
        case BUTTON_STATE_BUTTON0_PRESSED:
        {
            if ((appData.buttonDelay>0)&&
                (SWITCH_Get()!=SWITCH_STATE_PRESSED))     // SW01 pressed and released < 1 sec
            {
                //New Mic Gain
                appData.micGainIndex++;  //Next Gain Value
                if (appData.micGainIndex >= GAIN_STEPS)
                {
                    appData.micGainIndex = 0;    
                }
                appData.micGain= micGainLevels[appData.micGainIndex];
                
                if (0==appData.micGain)
                {
                    appData.ledState = MUTE_BLINK;
                    DRV_CODEC_MicGainSet(appData.codecClient.handle, 
                                         0);
                }
                else
                {
                    appData.ledState = LED_ON;
                    DRV_CODEC_MicGainSet(appData.codecClient.handle, 
                                         appData.micGain);
                }

                //Set the debounce value
                appData.buttonDelay=BUTTON_DEBOUNCE;                
                appData.buttonState=BUTTON_STATE_IDLE;              
            }
            else if ((appData.buttonDelay==0)&&
                     (SWITCH_Get()==SWITCH_STATE_PRESSED))  
            {
                appData.buttonState=BUTTON_STATE_WAIT_FOR_RELEASE;                
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
    } //End switch(buttonState)

} //End APP_Button_Tasks()


// ****************************************************************************
// APP_TimerCallback()
// --Callback for the periodic timer interrupt
//******************************************************************************
void _APP_TimerCallback( uintptr_t context)
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
//  APP_LED_Tasks())
//******************************************************************************
void _APP_LED_Tasks()
{
    //LED 
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
    } //End LED
} //End _APP_LED_Tasks()

//=============================================================================
// APP_CODECBufferEventHandler()
//
// Application CODEC buffer Event handler.  This function is called back by 
// the CODEC driver when a CODEC mic data buffer is received.
//
//=============================================================================
void APP_CODECBufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
        DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context )
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
        {
            //uint8_t index;

            if (appData.codecClient.
                    readBufHandle[appData.codecClient.rxBufIdx] == handle)
            {
#ifdef DEBUG_TEST_POINTS
                TEST3_Set();
#endif
                //Ready to TX USB buffer 1
                //--buffer 1 not available until USB TX completes
                //  (appData.codecClient.isReadBufHandleAvailable1) = false
                appData.codecClient.isCodecReadComplete = true;

                //Swap RX and TX buffers for the next Codec WriteRead
                // -- TX Buffer for Codec/USB playback
                // -- RX Buffer for next codec read complete
                appData.codecClient.rxBufIdx = 
                        1 - appData.codecClient.rxBufIdx;
                appData.codecClient.txBufIdx = 
                        1 - appData.codecClient.rxBufIdx;
                appData.codecClient.rxbufferObject = 
                    (uint8_t *) rxBuffer[appData.codecClient.rxBufIdx];
                appData.codecClient.txbufferObject = 
                    (uint8_t *) rxBuffer[appData.codecClient.txBufIdx];

                //printf("CODEC(1): Read Complete");
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
} //End APP_CODECBufferEventHandler()


//******************************************************************************
//_APP_DiscardChannelFromMicrophone()
//Create Mono from Duplicate Stereo Channel buffer 
//******************************************************************************
void _APP_DiscardChannelFromMicrophone(DRV_I2S_DATA16 *input, 
                                              int16_t *output, 
                                              int numSamples)
{
    int i;

    for (i = 0; i<numSamples; i++)
    {
        output[i] =  input[i].leftData;
    }
}

//******************************************************************************
//_APP_DiscardChannelFromMicrophone()
//Create Mono from Duplicate Stereo Channel buffer 
//******************************************************************************
void _APP_CombineChannelsFromI2S(DRV_I2S_DATA16 *input, 
                                 int16_t *output, 
                                 int numSamples)
{
    int i;

    for (i = 0; i<numSamples; i++)
    {
        output[i] =  (input[i].leftData/2 + input[i].rightData/2);
    }
}
/*******************************************************************************
 End of File
 */
