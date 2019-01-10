/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c for usb_speaker

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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"        // also beings in app_tone_lookup_table.h


#undef DEBUG_CODEC_TONE
#undef DEBUG_USB_ONLY

int usbReadScheduleCnt = 0; 
int usbReadFailureCnt = 0;
int usbReadCompleteCnt = 0; 
int usbReadCompleteQLvl = 0;    //Write completed
int codecWriteScheduleCnt = 0; 
int codecWritePendingQLvl = 0;
int codecWriteFailureCnt = 0;
int codecWriteCompleteCnt = 0; 
int usbNextCompleteIdx = 0;
int codecNextCompleteIdx = 0;
int nonZeroPacketCnt = 0;
extern int i2sAddCnt;
extern int dmaCnt;
int i,j;

volatile uint32_t codecNextWriteIdx;
volatile uint32_t usbNextReadIdx;

/* PCM16 samples for 1Khz Sine Wave at 48Khz Sample Rate */
static const DRV_I2S_DATA16 __attribute__((aligned(16))) audioSamples[48] =  {
//Sin Tone 1Khz @ 48Khz Sampling Rate - .5 Amplitude - 1 Cycle
#include "sin_1Khz_p5_48Ksps.dat"
};

static DRV_I2S_DATA16 __attribute__((aligned(16))) 
            cpBuffer16[APP_QUEUING_DEPTH][48];  

uint8_t  __attribute__((aligned(16))) __attribute__((tcm))
            readBuffer[APP_QUEUING_DEPTH][192]; 

void APP_USBDeviceAudioEventHandler(USB_DEVICE_AUDIO_INDEX iAudio,
                                    USB_DEVICE_AUDIO_EVENT event ,
                                    void * pData,
                                    uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint16_t __attribute__((aligned(16))) audioSamples2[96];

// table designed for 48 kHz or 96 kHz samples/sec only
uint16_t samples[] =
{
     192, // 200 Hz at 48000 samples/sec
      96, // 400
      48, // 1000
      24, // 2 kHz at 48000 samples/sec
};

uint16_t volumeLevels[] =
{
    0 /* off */, 128, 192, 255
};


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA __attribute__((tcm)) appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
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
            //APP_LED5_OFF();

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* check the configuration */
            configuredEventData = 
                     (USB_DEVICE_EVENT_DATA_CONFIGURED *)pEventData;

            if(configuredEventData->configurationValue == 1)
            {
                /* the device is in configured state */
                /* Switch on green and switch off red and orange */
                //APP_LED5_ON();

                USB_DEVICE_AUDIO_EventHandlerSet(0,
                                                 APP_USBDeviceAudioEventHandler ,
                                                 (uintptr_t)NULL);
                /* mark that set configuration is complete */
                appData.isConfigured = true;
                printf("USB Device Configured\n");
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            /* Switch on green and orange, switch off red */
            //APP_LED5_ON();

            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. Notify USB stack about the event */
            USB_DEVICE_Attach (appData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS was removed. Notify USB stack about the event*/
            USB_DEVICE_Detach (appData.usbDevHandle);
            appData.isConfigured = false; 
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
    volatile USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE *readEventData;
    volatile uint8_t __attribute__((unused)) status;

    uint8_t entityID;
    uint8_t controlSelector;
    int count; 
    
    if ( iAudio == 0 )
    {
        switch (event)
        {
            case USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED:
            {
                interfaceInfo = 
                   (USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED *)
                   pData;
                appData.activeInterfaceAlternateSetting = 
                        interfaceInfo->interfaceAlternateSetting;
                if(appData.activeInterfaceAlternateSetting == 
                   APP_USB_SPEAKER_PLAYBACK_NONE) 
                {
                   LED1_Off();
                }
                else
                {
                   LED1_On();
                }
            }
            break;

            case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:
            {
                /* This means the Read request completed. We can
                    * find out if the request was successful. */
                usbReadCompleteCnt++;
                usbReadCompleteQLvl++;

                //DEBUG:  Signal Read Complete
                TEST2_Hi();
                TEST2_Lo();

#if 0
                //TEST4_Lo();  //Queue Empty
                if (usbReadCompleteQLvl >= APP_QUEUING_DEPTH)
                {
                    TEST3_Hi();  //Queue Full
                }
                else
                {
                    TEST3_Lo();
                }
#endif

                usbNextCompleteIdx++;
                if(usbNextCompleteIdx >= APP_QUEUING_DEPTH)
                {
                    usbNextCompleteIdx = 0;
                }

                readEventData = 
                        (USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE*)pData;
                USB_DEVICE_AUDIO_RESULT status = readEventData->status;

                if (status != USB_DEVICE_AUDIO_RESULT_OK && 
                    status != USB_DEVICE_AUDIO_RESULT_OK)
                {
                    asm("NOP");
                }

                //Find buffer
                for (count = 0; count< APP_QUEUING_DEPTH; count++)
                {

                    if (appData.usbReadClient.readTransferHandle[count] == 
                        readEventData->handle )
                    {
                        //Current USB Data Frame
                        appData.usbReadClient.usbReadPending[count] = false;
                        appData.usbReadClient.usbDataAvail[count] = true;
                        appData.usbReadClient.bufferSize[count] = 
                                                 readEventData->length;
                        appData.usbReadClient.readTransferHandle[count] = 
                                    USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;
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
                USB_DEVICE_ControlStatus (appData.usbDevHandle, 
                                          USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            {
                USB_DEVICE_ControlStatus(appData.usbDevHandle, 
                                         USB_DEVICE_CONTROL_STATUS_OK );

                if (appData.currentAudioControl == APP_USB_AUDIO_MUTE_CONTROL)
                {
                    //appData.state = APP_MUTE_AUDIO_PLAYBACK;
                    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                    //Handle Mute Control Here.
                }
                
                //Needed for multiple frequency playback settings.
                //NOTE:  Maximum buffer size is 48 samples (192 bytes)
                //if (appData.currentAudioControl == APP_USB_AUDIO_SAMPLING_FREQ_CONTROL_HP)
                //{
                //    //DRV_CODEC_SamplingRateSet(appData.codecClientWrite.handle, 
                //    //                          appData.sampleFreq);
                //    if (appData.sampleFreq == SAMPLING_RATE_48000)
                //    {
                //        appData.codecClientWrite.bufferSize = 192;
                //    }
                //    else if (appData.sampleFreq == SAMPLING_RATE_32000)
                //    {
                //        appData.codecClientWrite.bufferSize = 128;
                //    }
                //    else if (appData.sampleFreq == SAMPLING_RATE_24000)
                //    {
                //        appData.codecClientWrite.bufferSize = 96;
                //    }
                //    else if (appData.sampleFreq == SAMPLING_RATE_16000)
                //    {
                //        appData.codecClientWrite.bufferSize = 64;
                //    }
                //    //appData.state = APP_USB_INTERFACE_ALTERNATE_SETTING_RCVD;
                //    appData.currentAudioControl = APP_USB_CONTROL_NONE;
                //  
                //    //NOTE:  Change Sampling Frequency then Reinit Playback Queue
                //    //       appData.state = APP_MUTE_AUDIO_PLAYBACK;
                //    appData.state = APP_SAMPLING_FREQUENCY_CHANGE; //RTOS Version
                //}
            }
            break;

            case  USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
                asm("NOP");
            break;

            default:
                SYS_ASSERT ( false , "Invalid callback" );
            break;

//NOTE:   
        } //End switch (event)
    }//end of if  if ( iAudio == 0 )

}//End APP_AudioEventCallback()

//******************************************************************************
//
// Audio_Codec_BufferEventHandler()
//
// Description
//   Application CODEC buffer Event handler called back by the CODEC driver 
//   when a CODEC data buffer TX completes. Indicates when head buffer
//   has been completed transmission.
//
//   NOTE: Required callback routine by CODEC driver.
//
// Arguments: 
//     event   - Action completed
//     handle  - driver instance handle
//     context - operation context
//
//******************************************************************************
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
                                  DRV_CODEC_BUFFER_HANDLE handle, 
                                  uintptr_t context)
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
        {
            int i;


            //Identify the buffer - Mark it read for data
            for (i=0; i< APP_QUEUING_DEPTH; i++)
            {
                if (handle == appData.codecClient.writeBufHandle[i])
                {
                    //Allow USB Read to buffer
                    appData.usbReadClient.codecBusy[i] = false;   
                    appData.usbReadClient.usbDataAvail[i] = false;   
                    appData.usbReadClient.usbReadPending[i] = false;

                    //Assume correct buffer handle
                    codecWriteCompleteCnt++; 
                    codecWritePendingQLvl--; //Actual data scheduled to writte
                    usbReadCompleteQLvl--;   //Pending data to write
                    TEST3_Lo();  //Queue Full
#if 0
                    if (usbReadCompleteQLvl == 0)
                    {
                        TEST4_Hi();  //Queue Empty
                    }
                    else
                    {
                        TEST4_Lo();
                    }
#endif
                    codecNextCompleteIdx++;
                    if(codecNextCompleteIdx >= APP_QUEUING_DEPTH)
                    {
                        codecNextCompleteIdx = 0;
                    }

                    //DEBUG
                    if (codecWriteCompleteCnt == 0x0040)
                    { 
                        asm("NOP");
                        asm("NOP");
                    }
                    break; 
                }
            } //End Complete Write Buffer Handle Loop
            TEST4_Lo();  //Buffer Write Complete
        }        
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
        } 
        break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
            asm("NOP");
        } 
        break;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

void fillInNumSamplesTable(uint8_t bufferNum)
{
    uint16_t i;
    
    if (bufferNum==1)
    {
        appData.numNumSamples1 = MAX_AUDIO_NUM_SAMPLES / samples[appData.sampleTableIndex];
        for (i=0; i < appData.numNumSamples1; i++)
        {
            appData.numSamples1[i] = samples[appData.sampleTableIndex];
        }
    }
    else
    {
        appData.numNumSamples2 = MAX_AUDIO_NUM_SAMPLES / samples[appData.sampleTableIndex]; 
        for (i=0; i < appData.numNumSamples2; i++)
        {
            appData.numSamples2[i] = samples[appData.sampleTableIndex];
        }        
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void _APP_USB_Reset()
{
    appData.isConfigured = false; 
    appData.isReadComplete = false; 
    appData.activeInterfaceAlternateSetting = APP_USB_SPEAKER_PLAYBACK_NONE; 
    TEST1_Lo();  //DMA 
    TEST2_Lo();
    TEST3_Lo();
    TEST4_Lo();
}

/*******************************************************************************
  Function:
    void APP_AudioDataSetDefault (void)

  Remarks:
    See prototype in app.h.
 */
void APP_AudioDataSetDefault()
{
    uint32_t loopCount = 0;
    
    usbNextReadIdx = 0;
    usbNextCompleteIdx = 0;
    codecNextWriteIdx = 0;
    codecNextCompleteIdx = 0;

    //DEBUG
    dmaCnt = 0;
    i2sAddCnt = 0;

    for (loopCount = 0; loopCount < APP_QUEUING_DEPTH; loopCount++)
    {
        appData.codecClient.writeBufHandle[loopCount] = DRV_HANDLE_INVALID;
        appData.usbReadClient.usbDataAvail[loopCount] = false; 
        appData.usbReadClient.codecBusy[loopCount] = false; 
        appData.usbReadClient.usbReadPending[loopCount] = false; 
        appData.usbReadClient.readTransferHandle[loopCount] = 
                                 USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;
        appData.usbReadClient.pReadBuffer[loopCount] = readBuffer[loopCount];
    } 
} //End APP_AudioDataSetDefault()


/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */   
    appData.state = APP_STATE_INIT;
    
    appData.volumeIndex = 1;
    appData.volume = volumeLevels[appData.volumeIndex];    
    
    appData.buttonDelay = 0;
    appData.buttonMode = false;    

    appData.sampleTableIndex = INIT_SAMPLE_INDEX;   

    appData.pingPong = 1;
    
    appData.isCodecWriteComplete = true; 
 
    //fillInNumSamplesTable(1);
    //fillInNumSamplesTable(2);  

    for (i=0; i<APP_QUEUING_DEPTH; i++ )
    {
        for (j=0; j<48; j++)
        {
            cpBuffer16[i][j].leftData  = audioSamples[j].leftData;
            cpBuffer16[i][j].rightData = audioSamples[j].rightData;
        }
    }
    
    _APP_USB_Reset();
    APP_AudioDataSetDefault();  //Set the buffer queue flags 
    SYS_DEBUG_MESSAGE(0,"\r\nAPP:  Initializing...");
    TEST1_Lo();
    TEST2_Lo();
    TEST3_Lo();
    TEST4_Lo();

} //End APP_Initialize())

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
DRV_HANDLE tmrHandle;
int count; 

void APP_Tasks ( void )
{   
    USB_DEVICE_AUDIO_RESULT audioResult;  
    uint32_t count; 
    bool firstUSBReadsCompleted = false; 
    bool isFailed = false;

    //APP_Button_Tasks();
    
    /* Check the application's current state. */
    switch ( appData.state )
    {       
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SYS_STATUS status;
            
            /* See if codec is done initializing */ 
            status = DRV_CODEC_Status(sysObjdrvCodec0);     
            if (SYS_STATUS_READY == status)
            {
                // This means the driver can now be be opened.
                /* A client opens the driver object to get an Handle */
                appData.codecClient.handle = DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                                                       DRV_IO_INTENT_WRITE | 
                                                       DRV_IO_INTENT_EXCLUSIVE);       
                if(appData.codecClient.handle != DRV_HANDLE_INVALID)
                {                                 
                    DRV_CODEC_VolumeSet(appData.codecClient.handle, 
                                        DRV_CODEC_CHANNEL_LEFT_RIGHT, 
                                        appData.volume);
                    //DRV_CODEC_SamplingRateSet(appData.codecClient.handle, 48000); 
                    DRV_CODEC_BufferEventHandlerSet(appData.codecClient.handle, 
                                    Audio_Codec_BufferEventHandler, 
                                    0);                                    
            
                    appData.state = APP_USB_DEVICE_OPEN;    
                }
            }                 
        }
        break;
       
        case APP_USB_DEVICE_OPEN:
            //Open USB for Read/Write 
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
            break; 
        
        case APP_STATE_WAIT_FOR_CONFIGURATION:
            
            if(appData.isConfigured == true)
            {
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE;
            }
                
            break; 
            
        case APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE:
            if (appData.activeInterfaceAlternateSetting == 
                APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                //Clear the frame counts
                usbReadScheduleCnt = 0;
                usbReadFailureCnt = 0;
                usbReadCompleteCnt = 0;
                usbReadCompleteQLvl = 0;  
                TEST3_Lo(); //Queue Full
                //TEST4_Hi(); //Queue Empty

                usbNextCompleteIdx = 0;

                appData.state = APP_STATE_SUBMIT_FIRST_USB_AUDIO_READ; 
            }
            break; 
            
        case APP_STATE_SUBMIT_FIRST_USB_AUDIO_READ:
             
            APP_AudioDataSetDefault(); 
            DRV_CODEC_WriteQueuePurge(appData.codecClient.handle);
            if(appData.isConfigured == false)
            {
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else if (appData.activeInterfaceAlternateSetting != 
                     APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE; 
            }
            else
            {
                codecNextWriteIdx = 0;
                codecNextCompleteIdx = 0;
                usbNextCompleteIdx = 0;
                usbNextReadIdx = 0;

                /* Start the USB Audio Reads */
                int count;
                for (count = 0; count < APP_QUEUING_DEPTH; count ++)
                {
                    appData.usbReadClient.
                                usbReadPending[count] = true; 
                    audioResult = USB_DEVICE_AUDIO_Read(
                                    0,
                                    &appData.usbReadClient.
                                    readTransferHandle[count],
                                    1,
                                    &readBuffer[count][0],
                                    192); 

                    if (audioResult != USB_DEVICE_AUDIO_RESULT_OK )
                    {
                        appData.usbReadClient.
                                    usbReadPending[count] = false; 
                        usbReadFailureCnt++;
                        isFailed = true;
                        break; //Try again
                    }
                    else
                    {
                        //USB Read Scheduled
                        usbNextReadIdx++;
                        if (usbNextReadIdx >= APP_QUEUING_DEPTH)
                        {
                            usbNextReadIdx = 0; 
                        }
                        usbReadScheduleCnt++; 
                    }
                } //Initial USB Reads Loop

                if (isFailed == true)
                {
                    appData.state = APP_STATE_SUBMIT_FIRST_USB_AUDIO_READ; 
                }
                else
                {
                    appData.state = APP_STATE_WAIT_FOR_INITIAL_READS; 
                }  
            }
            break; 
            
        case APP_STATE_WAIT_FOR_INITIAL_READS:
            if(appData.isConfigured == false)
            {
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else if (appData.activeInterfaceAlternateSetting != 
                     APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE; 
                APP_AudioDataSetDefault();
            }
            else
            {
                /* Wait for initial USB reads to complete */
                firstUSBReadsCompleted = true; 
                for (count =0; count < APP_FIRST_USB_READS_NUMBER; count++)
                {
                    if (appData.usbReadClient.usbDataAvail[count] == false)
                    {
                        firstUSBReadsCompleted = false; 
                        break; 
                    }
                }

                if (firstUSBReadsCompleted == true)
                {
                    /* Advance application state */
                    appData.state = APP_STATE_AUDIO_PLAYBACK; 
                    codecWriteCompleteCnt  = 0;
                    codecWriteScheduleCnt= 0;
                    codecWritePendingQLvl = 0;
                    codecWriteFailureCnt = 0;
                    codecNextWriteIdx = 0;
                    codecNextCompleteIdx = 0;
                }
                else
                {
                    appData.state = APP_STATE_WAIT_FOR_INITIAL_READS; 
                }
            }
            
            break; 
         
        case APP_STATE_AUDIO_PLAYBACK:
        {
            if (appData.isConfigured == false)
            {
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else if (appData.activeInterfaceAlternateSetting != 
                    APP_USB_SPEAKER_PLAYBACK_STEREO_48KHZ)
            {
                appData.state = APP_STATE_WAIT_FOR_USB_AUDIO_STREAM_ENABLE; 
            }
            else
            {
                //CODEC - Playback
                /* Submit Code write if USB Read is complete  */
                static volatile bool __attribute__((tcm,unused)) testVal1 = false;
                static volatile bool __attribute__((tcm,unused)) testVal2 = false;
                testVal1 =  appData.usbReadClient.usbDataAvail[codecNextWriteIdx];
                if ((appData.usbReadClient.usbDataAvail[codecNextWriteIdx] == true)
                     && (appData.usbReadClient.codecBusy[codecNextWriteIdx] == false)
                     && (appData.usbReadClient.usbReadPending[codecNextWriteIdx] == false))
                {      
#ifndef DEBUG_USB_ONLY
#ifdef DEBUG_CODEC_TONE
                    //Copy Sin Data to buffer
                    DRV_CODEC_BufferAddWrite(
                                appData.codecClient.handle, 
                                &appData.codecClient.writeBufHandle[codecNextWriteIdx],
                                cpBuffer16[codecNextWriteIdx],
                                //sinBuffer16, 
                                192); 
#else //DEBUG_CODEC_TONE
                    testVal2 =  appData.usbReadClient.usbDataAvail[codecNextWriteIdx];
                    if (testVal2 == false) 
                    {
                        asm("NOP");
                    }
                    TEST4_Hi();  //AddWrite
                    DRV_CODEC_BufferAddWrite(appData.codecClient.handle, 
                                             &appData.codecClient.
                                             writeBufHandle[codecNextWriteIdx],
                                             readBuffer[codecNextWriteIdx],
                                             192); 
#endif //DEBUG_CODEC_TONE
                    if (appData.codecClient.writeBufHandle[codecNextWriteIdx] != 
                        DRV_HANDLE_INVALID)
                    {
                        //Buffer is now codecBusy
                        appData.usbReadClient.codecBusy[codecNextWriteIdx] = true; 

                        //Prevent next USB Read from this buffer
                        //TODO:  Redundant
                        //appData.usbReadClient.
                        //            usbDataAvail[codecNextWriteIdx] = true;
                        //appData.usbReadClient.
                        //            usbReadPending[codecNextWriteIdx] = false;
                        appData.usbReadClient.codecBusy[codecNextWriteIdx] = true;   
                        codecWriteScheduleCnt++; 
                        codecWritePendingQLvl++;

                        //Next buffer
                        codecNextWriteIdx++;
                        if (codecNextWriteIdx >= APP_QUEUING_DEPTH)
                        {
                            codecNextWriteIdx = 0; 
                        }
                    } //End AddWrite
                    else
                    {
                        codecWriteFailureCnt++; 
                    }
#else //DEBUG_USB_ONLY
                    //Make the buffer ready for the next USB Read
                    appData.usbReadClient.usbDataAvail[codecNextWriteIdx] = false;   
                    appData.usbReadClient.codecBusy[codecNextWriteIdx] = false;   
                    codecNextWriteIdx++;
                    if (codecNextWriteIdx >= APP_QUEUING_DEPTH)
                    {
                        codecNextWriteIdx = 0; 
                    }

                    //Increment to next buffer for write
                    //--NO codec Writes Scheduled
                    codecWriteCompleteCnt++; 
                    codecNextCompleteIdx++;
                    if(codecNextCompleteIdx >= APP_QUEUING_DEPTH)
                    {
                        codecNextCompleteIdx = 0;
                    }
                    break; 
#endif //DEBUG_USB_ONLY
                } //End CODEC Playback

                //USB Audio Read
                /* Submit USB Reads if there is any empty slots in the queue */
                if ((appData.usbReadClient.codecBusy[usbNextReadIdx] == false)
                        &&(appData.usbReadClient.
                                     usbDataAvail[usbNextReadIdx] == false)
                        && (appData.usbReadClient.
                                     usbReadPending[usbNextReadIdx] == false))
                { 
                    audioResult = USB_DEVICE_AUDIO_Read(
                                      0,  //Device Audio Index
                                      &appData.usbReadClient.
                                           readTransferHandle[usbNextReadIdx],
                                      1,
                                      &readBuffer[usbNextReadIdx][0],
                                      192); 

                    if (audioResult == USB_DEVICE_AUDIO_RESULT_OK)
                    { 
                        usbReadScheduleCnt++;
                        appData.usbReadClient.
                                    usbReadPending[usbNextReadIdx] = true; 

                        usbNextReadIdx++; 
                        if (usbNextReadIdx >= APP_QUEUING_DEPTH)
                        {
                            usbNextReadIdx = 0; 
                        }
                    }
                    else
                    {
                        appData.usbReadClient.
                                    usbReadPending[usbNextReadIdx] = false; 
                        usbReadFailureCnt++; 
                    }
                }  //End USB Audio Read
            } //End Playback
        }
        break; //End case: APP_STATE_AUDIO_PLAYBACK 
        
        default:
        {
            /* TODO: Handle error in application's state machine. */
        }
        break;             
    } 
}

#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000

#define VOLUME_STEPS    4 
#define SAMPLE_STEPS    4

//void APP_Button_Tasks()
//{
//   //BUTTON PROCESSING
//    /* Check the buttons' current state. */      
//    switch ( appData.buttonState )
//    {
//        case BUTTON_STATE_IDLE:
//        {
//            if ( (appData.buttonDelay==0)&&
//                 (SWITCH_Get()==SWITCH_STATE_PRESSED))                
//            {
//                appData.buttonDelay=BUTTON_DEBOUNCE;       
//                appData.buttonState=BUTTON_STATE_PRESSED;               
//            }
//        }
//        break;
//        
//        case BUTTON_STATE_PRESSED:
//        { 
//            if (appData.buttonDelay>0)
//            {
//                break;      // still debouncing
//            }
//            
//            if(SWITCH_Get()==SWITCH_STATE_PRESSED) 
//            {                
//                appData.buttonDelay=LONG_BUTTON_PRESS;          // 1 sec is long press
//                appData.buttonState=BUTTON_STATE_BUTTON0_PRESSED;                  
//            }
//        }
//        break;
//          
//        case BUTTON_STATE_BUTTON0_PRESSED:
//        {
//            if ((appData.buttonDelay>0)&&
//                (SWITCH_Get()!=SWITCH_STATE_PRESSED))     // SW01 pressed and released < 1 sec
//            {
//                if (appData.buttonMode)     // if modifying frequency
//                {
//                    appData.sampleTableIndex++;
//                    if (appData.sampleTableIndex >= SAMPLE_STEPS)
//                    {
//                        appData.sampleTableIndex = 0;    
//                    }
//                                     
//                    if (0==appData.volume)
//                    {
//                        // after changing freq, if volume 0 make sure we can hear it
//                        appData.volumeIndex++;
//                        appData.volume = volumeLevels[appData.volumeIndex];                        
//                        Audio_Codec_VolumeSet(&appData.codecData, appData.volume);                       
//                        Audio_Codec_MuteOff(&appData.codecData);                        
//                    }                                                
//                }
//                else
//                {
//                    uint8_t oldVolumeIndex;
//                    
//                    oldVolumeIndex = appData.volumeIndex;
//                    appData.volumeIndex++;
//                    if (appData.volumeIndex >= VOLUME_STEPS)
//                    {
//                        appData.volumeIndex = 0;    
//                    }
//                    
//                    appData.volume = volumeLevels[appData.volumeIndex];
//                    
//                    if (0==appData.volume)
//                    {
//                        Audio_Codec_MuteOn(&appData.codecData);                        
//                    }
//                    else
//                    {
//                        Audio_Codec_VolumeSet(&appData.codecData, appData.volume);
//                        if (0==volumeLevels[oldVolumeIndex])
//                        {
//                            // if volume was 0, unmute codec
//                            Audio_Codec_MuteOff(&appData.codecData);                        
//                        }
//                    }                             
//                }
//                appData.buttonDelay=BUTTON_DEBOUNCE;                
//                appData.buttonState=BUTTON_STATE_IDLE;              
//            }
//            else if ((appData.buttonDelay==0)&&
//                     (SWITCH_Get()==SWITCH_STATE_PRESSED))  // SW0 still pressed after 1 sec
//            {
//                appData.buttonMode = !appData.buttonMode;
//                if (appData.buttonMode)
//                {
//                    LED1_On();
//                }
//                else
//                {
//                    LED1_Off();
//                }               
//                
//                appData.buttonState=BUTTON_STATE_WAIT_FOR_RELEASE;                
//            }                          
//        } 
//        break;
//
//        case BUTTON_STATE_WAIT_FOR_RELEASE:
//        {
//            if (SWITCH_Get()!=SWITCH_STATE_PRESSED)
//            {
//                appData.buttonDelay=BUTTON_DEBOUNCE;
//                appData.buttonState=BUTTON_STATE_IDLE;
//            }
//        }
//
//        /* The default state should never be executed. */
//        default:
//        {
//            /* TODO: Handle error in application's state machine. */
//            break;
//        }
//    }
//}

//******************************************************************************
// 
// Audio_Codec_TxBufferComplete() - Set APP_Tasks Next state to buffer complete.
//
// NOTE: Called from Audio_Codec_BufferEventHandler() (CODEC API)
//
// TODO: Put the appData instance pointer in the AUDIO_CODEC_DATA instance on
//       initialization of codecData and let the audio_codec instance change
//       the codec data and state. CAL
//
//******************************************************************************
void Audio_Codec_TxBufferComplete()
{
    //Next State -- after the buffer complete interrupt.
}

/*******************************************************************************
 End of File
 */
