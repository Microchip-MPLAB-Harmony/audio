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
Copyright (c) 2017-2018 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_MAX_TIMERS                  10



// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/*** Codec Driver Configuration ***/

#define DRV_WM8904_CLIENTS_NUMBER                           1
#define DRV_WM8904_INSTANCES_NUMBER                         1

#define DRV_WM8904_AUDIO_SAMPLING_RATE                      48000
#define DRV_WM8904_VOLUME	                      	        200
#define DRV_WM8904_AUDIO_DATA_FORMAT_MACRO             	    DATA_16_BIT_I2S
#define DRV_WM8904_ENABLE_MIC_INPUT             	    	false

#define DRV_WM8904_I2S_DRIVER_MODULE_INDEX_IDX0             DRV_I2S_INDEX_0
#define DRV_WM8904_I2C_DRIVER_MODULE_INDEX_IDX0             DRV_I2C_INDEX_0
/* CODEC Driver Abstraction definition */

#define DRV_CODEC_INDEX_0                                   DRV_WM8904_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvwm8904Codec0
#define DRV_CODEC_BUFFER_HANDLE                             DRV_WM8904_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_WM8904_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_WM8904_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_WM8904_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_WM8904_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_WM8904_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_WM8904_BUFFER_EVENT_ABORT
#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_WM8904_COMMAND_EVENT_HANDLER

#define DRV_CODEC_CHANNEL_LEFT                              DRV_WM8904_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_WM8904_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_WM8904_CHANNEL_LEFT_RIGHT

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
#define DRV_CODEC_SamplingRateSet                           DRV_WM8904_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_WM8904_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_WM8904_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_WM8904_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_WM8904_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_WM8904_MuteOff

/* I2C Driver Common Configuration Options */
#define DRV_I2C_INSTANCES_NUMBER              1


/* I2C Driver Instance 0 Configuration Options */
#define DRV_I2C_INDEX_0                       0
#define DRV_I2C_CLIENTS_NUMBER_IDX0           1
#define DRV_I2C_INT_SRC_IDX0                  TWIHS0_IRQn
#define DRV_I2C_QUEUE_SIZE_IDX0               2
#define DRV_I2C_CLOCK_SPEED_IDX0              400000

/* I2S Driver Common Configuration Options */
#define DRV_I2S_INSTANCES_NUMBER              2

/* I2S Driver Instance 0 Configuration Options */
#define DRV_I2S_INDEX_0                       0
#define DRV_I2S_CLIENTS_NUMBER_IDX0           1
#define DRV_I2S_QUEUE_DEPTH_COMBINED          8
#define DRV_I2S_QUEUE_SIZE_IDX0               8
#define DRV_I2S_DATA_LENGTH_IDX0              16
#define DRV_I2S_INT_SRC_IDX0                  SSC_IRQn
#define DRV_SSC_XMIT_DMA_CH_IDX0              DMA_CHANNEL_1
#define DRV_SSC_RCV_DMA_CH_IDX0               DMA_CHANNEL_0



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************


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
