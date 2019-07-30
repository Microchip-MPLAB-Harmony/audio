/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "toolchain_specifics.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                     0
#define SYS_TIME_MAX_TIMERS                  5
#define SYS_TIME_HW_COUNTER_WIDTH            16
#define SYS_TIME_HW_COUNTER_PERIOD           65535U
#define SYS_TIME_HW_COUNTER_HALF_PERIOD		 (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY         240000000
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES      (900)



// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Instance 0 Configuration Options */
#define DRV_I2C_INDEX_0                       0
#define DRV_I2C_CLIENTS_NUMBER_IDX0           1
#define DRV_I2C_QUEUE_SIZE_IDX0               2
#define DRV_I2C_CLOCK_SPEED_IDX0              400000

/* I2S Driver Instance 0 Configuration Options */
#define DRV_I2S_INDEX_0                       0
#define DRV_I2S_CLIENTS_NUMBER_IDX0           1
#define DRV_I2S_QUEUE_DEPTH_COMBINED          8
#define DRV_I2S_QUEUE_SIZE_IDX0               8
#define DRV_I2S_DATA_LENGTH_IDX0              16
#define DRV_I2S_INT_SRC_IDX0                  I2SC1_IRQn
#define DRV_I2S_XMIT_DMA_CH_IDX0              SYS_DMA_CHANNEL_0
#define DRV_I2S_RCV_DMA_CH_IDX0               SYS_DMA_CHANNEL_1


/* I2C Driver Common Configuration Options */
#define DRV_I2C_INSTANCES_NUMBER              1


/* I2S Driver Common Configuration Options */
#define DRV_I2S_INSTANCES_NUMBER              1

/*** Codec Driver Configuration ***/

#define DRV_WM8904_CLIENTS_NUMBER                           1
#define DRV_WM8904_INSTANCES_NUMBER                         1

#define DRV_WM8904_MASTER_MODE                              false
#define DRV_WM8904_AUDIO_SAMPLING_RATE                      16000
#define DRV_WM8904_VOLUME	                      	        200
#define DRV_WM8904_AUDIO_DATA_FORMAT_MACRO             	    DATA_16_BIT_I2S
#define DRV_WM8904_ENABLE_MIC_INPUT                         true
#define DRV_WM8904_ENABLE_MIC_BIAS                          true
#define DRV_WM8904_MIC_GAIN	                      	        28

#define DRV_WM8904_I2S_DRIVER_MODULE_INDEX_IDX0             DRV_I2S_INDEX_0
#define DRV_WM8904_I2C_DRIVER_MODULE_INDEX_IDX0             DRV_I2C_INDEX_0
/* CODEC Driver Abstraction definition */

//Codec Driver Instance
#define DRV_CODEC_INDEX_0                                   DRV_WM8904_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvwm8904Codec0
#define DRV_CODEC_I2S_MASTER_MODE                               DRV_WM8904_MASTER_MODE
#define DRV_CODEC_BUFFER_HANDLE                             DRV_WM8904_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_WM8904_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_WM8904_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_WM8904_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_WM8904_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_WM8904_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_WM8904_BUFFER_EVENT_ABORT
#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_WM8904_COMMAND_EVENT_HANDLER

//Codec Driver API Configuration Parameters
#define DRV_CODEC_CHANNEL_LEFT                              DRV_WM8904_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_WM8904_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_WM8904_CHANNEL_LEFT_RIGHT
#define DRV_CODEC_AUDIO_SAMPLING_RATE                       DRV_WM8904_AUDIO_SAMPLING_RATE
#define DRV_CODEC_VOLUME	                        	    DRV_WM8904_VOLUME
#define DRV_CODEC_AUDIO_DATA_FORMAT_MACRO                 	DRV_WM8904_AUDIO_DATA_FORMAT_MACRO
#define DRV_CODEC_ENABLE_MIC_INPUT                          DRV_WM8904_ENABLE_MIC_INPUT
#define DRV_CODEC_ENABLE_MIC_BIAS                           DRV_WM8904_ENABLE_MIC_BIAS
#define DRV_CODEC_MIC_GAIN                                  DRV_WM8904_MIC_GAIN

//Codec Driver API Functions
#define DRV_CODEC_Initialize                                DRV_WM8904_Initialize
#define DRV_CODEC_Deinitialize                              DRV_WM8904_Deinitialize
#define DRV_CODEC_Status                                    DRV_WM8904_Status
#define DRV_CODEC_Tasks                                     DRV_WM8904_Tasks
#define DRV_CODEC_Open                                      DRV_WM8904_Open
#define DRV_CODEC_Close                                     DRV_WM8904_Close
#define DRV_CODEC_BufferEventHandlerSet                     DRV_WM8904_BufferEventHandlerSet
#define DRV_CODEC_CommandEventHandlerSet                    DRV_WM8904_CommandEventHandlerSet
#define DRV_CODEC_BufferAddWrite                            DRV_WM8904_BufferAddWrite
#define DRV_CODEC_BufferAddRead                             DRV_WM8904_BufferAddRead
#define DRV_CODEC_BufferAddWriteRead                        DRV_WM8904_BufferAddWriteRead
#define DRV_CODEC_WriteQueuePurge                           DRV_WM8904_WriteQueuePurge
#define DRV_CODEC_ReadQueuePurge                            DRV_WM8904_ReadQueuePurge
#define DRV_CODEC_SamplingRateSet                           DRV_WM8904_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_WM8904_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_WM8904_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_WM8904_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_WM8904_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_WM8904_MuteOff
#define DRV_CODEC_StereoMicSelect                           DRV_WM8904_StereoMicSelect
#define DRV_CODEC_MicGainSet                                DRV_WM8904_MicGainSet
#define DRV_CODEC_MicGainGet                                DRV_WM8904_MicGainGet
#define DRV_CODEC_MicMuteOn                                 DRV_WM8904_MicMuteOn
#define DRV_CODEC_MicMuteOff                                DRV_WM8904_MicMuteOff
#define DRV_CODEC_GetI2SDriver                              DRV_WM8904_GetI2SDriver
#define DRV_CODEC_LRCLK_Sync                                DRV_WM8904_LRCLK_Sync 



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
/* Number of Endpoints used */
#define DRV_USBHSV1_ENDPOINTS_NUMBER                        3

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER                         1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64



	


/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBHSV1_INSTANCES_NUMBER                        1

/* Interrupt mode enabled */
#define DRV_USBHSV1_INTERRUPT_MODE                          true

/* Enables Device Support */
#define DRV_USBHSV1_DEVICE_SUPPORT                          true

/* Disable Host Support */
#define DRV_USBHSV1_HOST_SUPPORT                            false

/* Alignment for buffers that are submitted to USB Driver*/ 
#define USB_ALIGN  CACHE_ALIGN

/* Maximum instances of Audio function driver */
#define USB_DEVICE_AUDIO_INSTANCES_NUMBER    1 

/* Audio Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
//KEEP THIS- not generated correctly after the first time.
// 8+8
#define USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED 16


/* No of Audio streaming interfaces */
#define USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES   1

/* No of alternate settings */
#define USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING      2




// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
