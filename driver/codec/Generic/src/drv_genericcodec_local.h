/*******************************************************************************
  Generic Codec Driver Interface

  Company:
    Microchip Technology Inc.

  File Name:
    drv_genericcodec_local.h

  Summary:
    Generic Codec driver local declarations and definitions

  Description:
    This file contains the Generic Codec driver's local declarations and definitions.

    The user will need to modify it to match the requirments of their codec.
    Areas where code needs to be added or changed are marked with TO-DO!!

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_GENERICCODEC_LOCAL_H
#define _DRV_GENERICCODEC_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "drv_genericcodec.h"
#include "definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Constants and Macros
// *****************************************************************************
// *****************************************************************************

/* Generic Codec Driver Version Macros

  Summary:
    Generic Codec driver version.

  Description:
    These constants provide Generic Codec driver version information. The driver
    version is:
    DRV_GENERICCODEC_VERSION_MAJOR.DRV_GENERICCODEC_VERSION_MINOR[.<DRV_GENERICCODEC_VERSION_PATCH>][DRV_GENERICCODEC_VERSION_TYPE]
    It is represented in DRV_I2S_VERSION as:
    MAJOR *10000 + MINOR * 100 + PATCH, so as to allow comparisons.
    It is also represented in string format in DRV_GENERICCODEC_VERSION_STR.
    DRV_GENERICCODEC_VERSION_TYPE provides the type of the release when the
    release is a(alpha) or b(beta). The interfaces DRV_GENERICCODEC_VersionGet
    and DRV_GENERICCODEC_VersionStrGet provide interfaces to the access the
    version and the version string.

  Remarks:
    Modify the return value of DRV_I2S_VersionStrGet and the
    DRV_GENERICCODEC_VERSION_MAJOR, DRV_GENERICCODEC_VERSION_MINOR,
    DRV_GENERICCODEC_VERSION_PATCH, and DRV_GENERICCODEC_VERSION_TYPE.
 */

#define _DRV_GENERICCODEC_VERSION_MAJOR         0
#define _DRV_GENERICCODEC_VERSION_MINOR         1
#define _DRV_GENERICCODEC_VERSION_PATCH         0
#define _DRV_GENERICCODEC_VERSION_TYPE          a
#define _DRV_GENERICCODEC_VERSION_STR           "0.1.0a"

// *****************************************************************************
// *****************************************************************************
// Section: Register Definitions for Generic Codec
// 
// Used in the GENERICCODEC_I2C_InitializationCommands arrays(s)
// *****************************************************************************
// *****************************************************************************
    
// TO-DO!!
// add defines for any registers inthe codec, e.g.

// Register index.

//#define WM8904_SW_RESET_AND_ID                  0x00
//#define WM8904_BIAS_CONTROL_0                   0x04
// etc.

// also define bit defintions for each field in the registers, e.g.

// Field Definitions.

// R4 (0x04) - Bias Control 0

//#define WM8904_ISEL_Pos                         2  /* ISEL - [3:2] */
//#define WM8904_ISEL_Msk                         0x000C
//#define WM8904_ISEL(Value)                      (WM8904_ISEL_Msk & ((Value) << WM8904_ISEL_Pos))
// etc.

// *****************************************************************************
/* I2C Address for the on-board Generic Codec

  Summary:
    Defines the on-board Generic Codec's I2C Address.

  Description:
    This #define defines the on-board Generic Codec's I2C Address. It is used
    by the I2C Driver API's to transmit and receive data.
*/
    
#define DRV_GENERICCODEC_DEVICE_ADDR           0x34     // 0011 0100   In Harmony Driver format.

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* I2C Address for the on-board Generic Codec

  Summary:
    Defines the on-board Generic Codec's I2C Address.

  Description:
    This #define defines the on-board Generic Codec's I2C Address. It is used
    by the I2C Driver API's to transmit and receive data.
*/
    
// TO-DO!!  change to the address of your codec
#define APP_CODEC_GENERICCODEC_DEVICE_ADDR           0x00      // must be in Harmony 3 Driver 7-bit format.

#define APP_WRITE_DATA_LENGTH            3

/**********************************************
 * Driver Client Obj
 **********************************************/
typedef struct _DRV_GENERICCODEC_CLIENT_OBJ_STRUCT
{
    /* Indicates that this object is in use */
    bool inUse;

    /* Indicate whether the client is open in
     * read,write or read/write mode */
    DRV_IO_INTENT ioIntent;

    /* Indicate the possible events that can 
     * result from a buffer add request */

    /* Call back function for this client */
    DRV_GENERICCODEC_BUFFER_EVENT_HANDLER  pEventCallBack;

    /* Client data(Event Context) that will be
     * returned at callback */
    uintptr_t hClientArg;

    /* pointer to the driver that own this object */
    void* hDriver;

} DRV_GENERICCODEC_CLIENT_OBJ;

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	DRV_GENERICCODEC_STATE_INIT=0,

    DRV_GENERICCODEC_STATE_OPEN,            
            
    DRV_GENERICCODEC_STATE_READY,           
            
    DRV_GENERICCODEC_STATE_DATA_SETUP,

	DRV_GENERICCODEC_STATE_DATA_TRANSMIT,           
            
    DRV_GENERICCODEC_STATE_UPDATE,

    DRV_GENERICCODEC_STATE_ERROR,            
            
    DRV_GENERICCODEC_STATE_DONE            

} DRV_GENERICCODEC_STATE;

typedef struct {
	uint8_t reg_addr;    
	uint16_t value;
    uint16_t delay;
    bool end;    
} GENERICCODEC_I2C_COMMAND_BUFFER;

#define GENERICCODEC_MAX_I2C_COMMANDS  60     // current max at one time is about 40
// it's assumed we don't need a circular buffer, so any commands that are added while
// in the process of sending are just added to the end, and the loop ptrs reset only
// when completely idle

/***********************************************
 * Driver object structure. One object per
 * hardware instance
 **********************************************/

typedef struct _DRV_GENERICCODEC_OBJ_STRUCT
{

    /* Status of this driver instance */
    SYS_STATUS status;

    /* Indicates this object is in use */
    bool inUse;

    /* Flag to indicate that the hardware instance is used
     *  in exclusive access mode */
    bool isExclusive;

    /* Number of clients possible with the hardware instance */
    uint8_t numClients;

    /* Identifies data module(I2S) driver ID for
     * data interface of Codec */
    SYS_MODULE_INDEX i2sDriverModuleIndex;

    /* Identifies data module(I2C) driver ID for
     * control interface of Codec */
    SYS_MODULE_INDEX i2cDriverModuleIndex;
    
    /* Identifies data module(I2S) driver open handle */
    DRV_HANDLE i2sDriverHandle;

    DRV_HANDLE i2sDriverClientHandleRead;

    DRV_HANDLE i2sDriverClientHandleWrite;

    /* Identifies control module timer ID for
     * control interface of Codec */
    SYS_MODULE_INDEX tmrDriverModuleIndex;

    /* Identifies control module(Timer) driver open handle */    
    DRV_HANDLE tmrDriverHandle;

    /* Sampling rate */
    uint32_t samplingRate;

    uint8_t micGain;
    
    /* Identifies the Audio data format */
    DRV_GENERICCODEC_AUDIO_DATA_FORMAT audioDataFormat;

    /* Hardware instance mutex */
    OSAL_MUTEX_DECLARE(mutexDriverInstance);

    /* ----------------------------------------------------*/
    /* Control interface specific Implementation variables */
    /* ----------------------------------------------------*/

    /* Volume for volume command */
    uint8_t volume[DRV_GENERICCODEC_NUMBER_OF_CHANNELS];

    /* Command complete callback function */
    DRV_GENERICCODEC_COMMAND_EVENT_HANDLER commandCompleteCallback;

    /* command complete event context */
    uintptr_t commandContextData;
    
    SYS_MODULE_INDEX drvIndex;
    
    /* The application's current state */
    DRV_GENERICCODEC_STATE currentState;

    /* I2C Driver Handle */
    DRV_HANDLE drvI2CHandle;
    
    /* Write transfer handle */
    DRV_I2C_TRANSFER_HANDLE hWriteTransfer;

    /* Read transfer handle */
    DRV_I2C_TRANSFER_HANDLE hReadTransfer;   

    DRV_HANDLE tmrHandle;     

    uint32_t GENERICCODECDelay;        // decremented in TimerCallback every ms
    
    GENERICCODEC_I2C_COMMAND_BUFFER I2C_Commands[GENERICCODEC_MAX_I2C_COMMANDS];
    
    uint8_t tmpData[10];
    
    uint8_t I2C_loopCount, I2C_endLoop, I2C_lastCommand;
    
    bool writeComplete;

    bool readComplete;
   
} DRV_GENERICCODEC_OBJ;

// *****************************************************************************
/* Generic Codec Driver Global Instances Object

  Summary:
    Object used to keep track of data that is common to all instances of the
    Generic Codec driver.

  Description:
    This object is used to keep track of any data that is common to all
    instances of the Generic Codec driver.

  Remarks:
    None.
*/

typedef struct
{
    /* Set to true if all members of this structure
       have been initialized once */
    bool membersAreInitialized;

    /* Mutex to protect client object pool */
    OSAL_MUTEX_DECLARE(mutexClientObjects);

} DRV_GENERICCODEC_COMMON_DATA_OBJ;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef _DRV_GENERICCODEC_LOCAL
/*******************************************************************************
 End of File
*/
