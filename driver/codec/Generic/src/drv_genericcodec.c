/*******************************************************************************
  Generic Codec Driver implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_genericcodec.c

  Summary:
    Generic Codec Driver implementation.

  Description:
    This file contains the implementation of the Generic Codec driver, which
    provides a simple interface to manage a codec that can be interfaced to a 
    Microchip microcontroller. 

    The user will need to modify it to match the requirments of their codec.
    Areas where code needs to be added or changed are marked with TO-DO!!

    Note: this module assumes the codec is controlled over an I2C interface.
    The I2C Driver will need to be enabled in the MHC Project Graph.  If another 
    type of interface is used, the user will need to modify the code to suit.
    This module also makes use of SYS_TIME.  It will need to be enabled in the
    Project Graph.

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

/*************************************************************
 * Include files.
 ************************************************************/

#include "drv_genericcodec_local.h"

static bool add_I2C_Command(DRV_GENERICCODEC_OBJ *drvObj, GENERICCODEC_I2C_COMMAND_BUFFER I2C_CommandPtr,
        bool end);
// TO-DO!! - uncomment the following line when this function needs to be referenced
//static void start_I2C_Commands(DRV_GENERICCODEC_OBJ *drvObj);
static void _DRV_GENERICCODEC_ControlTasks(DRV_GENERICCODEC_OBJ *drvObj);
static void GENERICCODEC_TimerCallback( uintptr_t context );
static void _DRV_GENERICCODEC_I2SBufferEventHandler(DRV_I2S_BUFFER_EVENT event,
    DRV_I2S_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle);
static void _samplingRateSet(DRV_GENERICCODEC_OBJ *drvObj, uint32_t sampleRate, bool standalone);
static void _volumeSet(DRV_GENERICCODEC_OBJ *drvObj, DRV_GENERICCODEC_CHANNEL chan, uint8_t volume, bool standalone);
static void _micGainSet(DRV_GENERICCODEC_OBJ *drvObj, uint8_t volume, bool standalone);

// *****************************************************************************
/* Driver Hardware instance objects.

  Summary:
    Defines the hardware instances objects for the Generic Codec

  Description:
    This data type defines the hardware instance objects that are available for
    Generic Codec, so as to capture the hardware state of the instance.

  Remarks:
    Not all modes are available on all micro-controllers.
 */
DRV_GENERICCODEC_OBJ gDrvgenericcodecObj[DRV_GENERICCODEC_INSTANCES_NUMBER];

// *****************************************************************************
/* Driver Client instance objects.

  Summary:
    Defines the client instances objects

  Description:
    This data type defines the client instance objects that are available on
    Generic Codec, so as to capture the client state of the instance.
    It uses the configuration of maximum number of clients which can get
    registered per hardware instance.

  Remarks:
    Not all modes are available on all micro-controllers.
 */
DRV_GENERICCODEC_CLIENT_OBJ gDrvgenericcodecClientObj[DRV_GENERICCODEC_CLIENTS_NUMBER];


// *****************************************************************************
/* Driver common data object

  Summary:
    Defines the common data object

  Description:
    This object maintains data that is required by all Generic Codec
   driver instances

  Remarks:
    None
 */
DRV_GENERICCODEC_COMMON_DATA_OBJ gDrvgenericcodecCommonDataObj;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

/// *****************************************************************************
 /*
  Function:
        static void GENERICCODEC_I2CEventHandler
        (
            DRV_I2C_TRANSFER_EVENT event
            DRV_I2C_TRANSFER_HANDLE transferHandle, 
            uintptr_t context
        )

  Summary:
    Implements the handler for I2C  callback function.

  Description:
    Called when I2c transfer completes.

  Parameters:
    event       -
    transferHandle - 
    context     - Contains index into driver's instance object array

  Remarks:
    None
*/
static void GENERICCODEC_I2CEventHandler ( DRV_I2C_TRANSFER_EVENT event,
                           DRV_I2C_TRANSFER_HANDLE transferHandle, 
                           uintptr_t context )
{
    DRV_GENERICCODEC_OBJ *drvObj = (DRV_GENERICCODEC_OBJ*)context;
    
    /* Checks for valid buffer handle */
    if( transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID )
    {
        return;
    }
       
    if( transferHandle == drvObj->hWriteTransfer &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        /* Codec write operation is complete */
        drvObj->writeComplete = true;
    }
           
    if( transferHandle == drvObj->hReadTransfer &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        /* Codec read is complete */
        drvObj->readComplete = true;
    }   
}

/// *****************************************************************************
 /*
  Function:
        static void GENERICCODEC_TimerCallback
        (
            uintptr_t context
        )

  Summary:
    Implements the handler for timer callback function.

  Description:
    Called every 1 ms by timer services.  It then decrements GENERICCODECDelay if
    non-zero.

  Parameters:
    context      - Contains index into driver's instance object array

  Remarks:
    None
*/
static void GENERICCODEC_TimerCallback( uintptr_t context )
{
    DRV_GENERICCODEC_OBJ *drvObj = (DRV_GENERICCODEC_OBJ*)context;    
    if (drvObj->GENERICCODECDelay)
    {      
        drvObj->GENERICCODECDelay--;      
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Generic Codec Driver System Routine Implementations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/*   Function:
	SYS_MODULE_OBJ  DRV_GENERICCODEC_Initialize
	(
		const SYS_MODULE_INDEX drvIndex,
        const SYS_MODULE_INIT *const init
	);

  Summary:
    Initializes hardware and data for the instance of the Generic Codec module

  Description:
    This routine initializes the Generic Codec driver instance for the specified driver
    index, making it ready for clients to open and use it. The initialization
    data is specified by the init parameter. The initialization may fail if the
    number of driver objects allocated are insufficient or if the specified
    driver instance is already initialized.

  Precondition:
    None

  Remarks:
    This routine must be called before any other Generic Codec routine is called.

    This routine should only be called once during system initialization
    unless DRV_GENERICCODEC_Deinitialize is called to de-initialize the driver
    instance. This routine will NEVER block for hardware access.

 */
SYS_MODULE_OBJ  DRV_GENERICCODEC_Initialize
(
	const SYS_MODULE_INDEX drvIndex,
	const SYS_MODULE_INIT *const init
)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_INIT *genericcodecInit;
    uint8_t index;


    /* Validate the driver index */
    if (drvIndex >= DRV_GENERICCODEC_INSTANCES_NUMBER)
    {
        SYS_DEBUG(0, "Invalid driver index \r\n");
        return SYS_MODULE_OBJ_INVALID;
    }

    if (true == gDrvgenericcodecObj[drvIndex].inUse)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG(0, "Instance already in use \r\n");
        return SYS_MODULE_OBJ_INVALID;
    }

    genericcodecInit = (DRV_GENERICCODEC_INIT *) init;
    drvObj = (DRV_GENERICCODEC_OBJ *)&gDrvgenericcodecObj[drvIndex];

    /* Populate the driver object with the required data */
    drvObj->inUse                           = true;
    drvObj->status                          = SYS_STATUS_UNINITIALIZED;
    drvObj->numClients                      = 0;
    drvObj->i2sDriverModuleIndex            = genericcodecInit->i2sDriverModuleIndex;
    drvObj->samplingRate                    = genericcodecInit->samplingRate;
    drvObj->audioDataFormat                 = genericcodecInit->audioDataFormat;

    drvObj->currentState    = DRV_GENERICCODEC_STATE_INIT;
    drvObj->tmrHandle       = DRV_HANDLE_INVALID;     
    drvObj->drvI2CHandle    = DRV_HANDLE_INVALID;
    drvObj->hWriteTransfer  = DRV_I2C_TRANSFER_HANDLE_INVALID;
    drvObj->hReadTransfer   = DRV_I2C_TRANSFER_HANDLE_INVALID;
    drvObj->drvIndex        = drvIndex;
    drvObj->GENERICCODECDelay = 0;
    drvObj->I2C_endLoop     = 0;
    drvObj->I2C_lastCommand = 0;
    
    drvObj->readComplete = false;
    drvObj->writeComplete = false;
    
    /*Assigning the init volume to all supported audio channels*/
    for(index=0; index < DRV_GENERICCODEC_NUMBER_OF_CHANNELS; index++)
    {
        drvObj->volume[index] = genericcodecInit->volume;
    }

    drvObj->commandCompleteCallback = (DRV_GENERICCODEC_COMMAND_EVENT_HANDLER)0;    
    drvObj->commandContextData = -1;

    /* Create the hardware instance mutex. */
     if(OSAL_MUTEX_Create(&(drvObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
     {
        return SYS_MODULE_OBJ_INVALID;
     }

    /* Check if the global mutexes have been created. If not
       then create these. */
     if(!gDrvgenericcodecCommonDataObj.membersAreInitialized)
     {
         /* This means that mutexes where not created. Create them. */
         if((OSAL_MUTEX_Create(&(gDrvgenericcodecCommonDataObj.mutexClientObjects)) != OSAL_RESULT_TRUE))
         {
            return SYS_MODULE_OBJ_INVALID;
         }
         /* Set this flag so that global mutexes get allocated only once */
         gDrvgenericcodecCommonDataObj.membersAreInitialized = true;
     }

    drvObj->status = SYS_STATUS_BUSY;

    /* Return the object structure */
    return ((SYS_MODULE_OBJ) drvObj);

} /* DRV_GENERICCODEC_Initialize */


// *****************************************************************************
/* Function:
    void DRV_GENERICCODEC_Deinitialize( SYS_MODULE_OBJ object)

  Summary:
    Deinitializes the specified instance of the Generic Codec driver module

  Description:
    Deinitializes the specified instance of the Generic Codec driver module, disabling
    its operation (and any hardware).  Invalidates all the internal data.

  Precondition:
    Function DRV_GENERICCODEC_Initialize should have been called before calling this
    function.

  Parameters:
    object          - Driver object handle, returned from the
                      DRV_GENERICCODEC_Initialize routine

  Returns:
    None.

  Remarks:
    Once the Initialize operation has been called, the De-initialize operation
    must be called before the Initialize operation can be called again. This
    routine will NEVER block waiting for hardware.
*/
void DRV_GENERICCODEC_Deinitialize( SYS_MODULE_OBJ object)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    

    if (object == SYS_MODULE_OBJ_INVALID || object == (SYS_MODULE_OBJ)NULL )
    {
        /* Invalid object */
        SYS_DEBUG(0, "Invalid object \r\n");
        return;
    }

	drvObj = (DRV_GENERICCODEC_OBJ *) object;

    if (false == drvObj->inUse)
    {
        /* Cannot deinitialize an object that is
         * not already in use. */
        SYS_DEBUG(0, "Instance not in use \r\n");
        return;
    }

    /* Deallocate all the mutexes */
     if((OSAL_MUTEX_Delete(&(drvObj->mutexDriverInstance)) != OSAL_RESULT_TRUE))
     {
        SYS_DEBUG(0, "Unable to delete client handle mutex \r\n");
        return;
     }

    /* Indicate that this object is not is use */
    drvObj->inUse = false;

    /* Set number of clients to zero */
    drvObj->numClients = 0;
    drvObj->status = SYS_STATUS_UNINITIALIZED;
}


// *****************************************************************************
/* Function:
    SYS_STATUS DRV_GENERICCODEC_Status( SYS_MODULE_OBJ object)

  Summary:
    Gets the current status of the Generic Codec driver module.

  Description:
    This routine provides the current status of the Generic Codec driver module.

  Precondition:
    Function DRV_GENERICCODEC_Initialize should have been called before calling this
    function.

  Parameters:
    object          - Driver object handle, returned from the
                      DRV_GENERICCODEC_Initialize routine

  Returns:
    SYS_STATUS_DEINITIALIZED  - Indicates that the driver has been
                                de-initialized

    SYS_STATUS_READY          - Indicates that any previous module operation
                                for the specified module has completed

    SYS_STATUS_BUSY           - Indicates that a previous module operation for
                                the specified module has not yet completed

    SYS_STATUS_ERROR          - Indicates that the specified module is in an
                                error state

  Remarks:
    A driver can opened only when its status is SYS_STATUS_READY.
*/
SYS_STATUS DRV_GENERICCODEC_Status( SYS_MODULE_OBJ object)
{
    DRV_GENERICCODEC_OBJ *drvObj;

    if (object == SYS_MODULE_OBJ_INVALID ||
        object < DRV_GENERICCODEC_INSTANCES_NUMBER)
    {
        SYS_DEBUG(0, "System Module Object is invalid \r\n");
        return SYS_STATUS_ERROR;
    }
    drvObj = (DRV_GENERICCODEC_OBJ *)object;

    /* Return the status of the driver object */
    return drvObj->status;
} /* DRV_GENERICCODEC_Status */


// *****************************************************************************
/* Function:
    void  DRV_GENERICCODEC_Tasks(SYS_MODULE_OBJ object);

  Summary:
    Maintains the driver's control and data interface state machine.

  Description:
    This routine is used to maintain the driver's internal control and data
    interface state machine and implement its control and data interface
    implementations.
    This function should be called from the SYS_Tasks() function.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

  Parameters:
    object      - Object handle for the specified driver instance (returned from
                  DRV_GENERICCODEC_Initialize)

  Returns:
    None.

  Remarks:
    This routine is normally not called directly by an application.  It is
    called by the system's Tasks routine (SYS_Tasks).

*/
void DRV_GENERICCODEC_Tasks(SYS_MODULE_OBJ object)
{

    DRV_GENERICCODEC_OBJ *drvObj;

    drvObj = (DRV_GENERICCODEC_OBJ *)object;

    if((false == drvObj->inUse))
    {
        // This instance of the driver is not initialized. Don't do anything
        return;
    }

    _DRV_GENERICCODEC_ControlTasks(drvObj);
}

// *****************************************************************************
// *****************************************************************************
// Section: Generic Codec Driver Client Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_GENERICCODEC_Open
    (
		const SYS_MODULE_INDEX drvIndex,
		const DRV_IO_INTENT    ioIntent
	)

  Summary:
    Opens the specified Generic Codec driver instance and returns a handle to it

  Description:
    This routine opens the specified Generic Codec driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. The ioIntent
    parameter defines how the client interacts with this driver instance.

    The DRV_IO_INTENT_BLOCKING and DRV_IO_INTENT_NONBLOCKING ioIntent
    options are not relevant to this driver. All the data transfer functions
    of this driver are non blocking.

    Only DRV_IO_INTENT_WRITE is a valid ioIntent option as Generic Codec is DAC only.

    Specifying a DRV_IO_INTENT_EXCLUSIVE will cause the driver to provide
    exclusive access to this client. The driver cannot be opened by any
    other client.

  Precondition:
    Function DRV_GENERICCODEC_Initialize must have been called before calling this
    function.

  Parameters:
    drvIndex    - Identifier for the object instance to be opened

    ioIntent    - Zero or more of the values from the enumeration
                  DRV_IO_INTENT "or'd" together to indicate the intended use
                  of the driver. See function description for details.

  Returns:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance).

    If an error occurs, the return value is DRV_HANDLE_INVALID. Error can occur
    - if the number of client objects allocated via DRV_GENERICCODEC_CLIENTS_NUMBER is insufficient.
    - if the client is trying to open the driver but driver has been opened exclusively by another client.
    - if the driver hardware instance being opened is not initialized or is invalid.
    - if the ioIntent options passed are not relevant to this driver.

  Remarks:
    The handle returned is valid until the DRV_GENERICCODEC_Close routine is called.
    This routine will NEVER block waiting for hardware.If the requested intent
    flags are not supported, the routine will return DRV_HANDLE_INVALID.  This
    function is thread safe in a RTOS application. It should not be called in an
    ISR.
*/
DRV_HANDLE DRV_GENERICCODEC_Open
(
	const SYS_MODULE_INDEX iDriver,
	const DRV_IO_INTENT ioIntent
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *hClient;
    DRV_GENERICCODEC_OBJ *drvObj;
    uint32_t iClient;

    /* The iDriver value should be valid. It should be
     * less the number of driver object instances.
     */
    if (iDriver >= DRV_GENERICCODEC_INSTANCES_NUMBER)
    {
        SYS_DEBUG(0, "Bad Driver Index \r\n");
        return DRV_HANDLE_INVALID;
    }
    drvObj = (DRV_GENERICCODEC_OBJ *)&gDrvgenericcodecObj[iDriver];
    if (drvObj->status != SYS_STATUS_READY)
    {
        /* The GENERICCODEC  module should be ready */
        SYS_DEBUG(0, "Was the driver initialized? \r\n");
        return DRV_HANDLE_INVALID;
    }

    if ((drvObj->numClients > 0) && (true == drvObj->isExclusive))
    {
        /* Driver already opened in exclusive mode. Cannot open a new client. */
        SYS_DEBUG(0, "Cannot open a new client in exclusive mode \r\n");
        return DRV_HANDLE_INVALID;
    }

    if ((drvObj->numClients > 0) &&
        (DRV_IO_INTENT_EXCLUSIVE == (ioIntent & DRV_IO_INTENT_EXCLUSIVE)))
    {
        /*  A client Instance of driver is open.
            Cannot open the new client in exclusive mode */
            SYS_DEBUG(0, "Cannot open a new client in exclusive mode \r\n");
            return DRV_HANDLE_INVALID;
    }

    iClient = 0;
    hClient = (DRV_GENERICCODEC_CLIENT_OBJ *)&gDrvgenericcodecClientObj[iClient];

    /* Grab client object mutex here */
    if(OSAL_MUTEX_Lock(&(gDrvgenericcodecCommonDataObj.mutexClientObjects), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* Setup client operations */
        /* Find available slot in array of client objects */
        for (; iClient < DRV_GENERICCODEC_CLIENTS_NUMBER; iClient++)
        {
            if (false == hClient->inUse)
            {
                /* Set the exclusive mode for the driver instance */
                if (DRV_IO_INTENT_EXCLUSIVE == (ioIntent & DRV_IO_INTENT_EXCLUSIVE))
                {
                    drvObj->isExclusive = true;
                }

                if(DRV_IO_INTENT_READWRITE == (ioIntent & DRV_IO_INTENT_READWRITE))
                {
                    hClient->ioIntent = DRV_IO_INTENT_READWRITE;
                    drvObj->i2sDriverHandle = DRV_I2S_Open(drvObj->i2sDriverModuleIndex,
                        (DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING));
                }
                else if(DRV_IO_INTENT_WRITE == (ioIntent & DRV_IO_INTENT_READWRITE))
                {
                    hClient->ioIntent = DRV_IO_INTENT_WRITE;
                    drvObj->i2sDriverClientHandleWrite = DRV_I2S_Open(drvObj->i2sDriverModuleIndex,
                        (DRV_IO_INTENT_WRITE | DRV_IO_INTENT_NONBLOCKING));
                    drvObj->i2sDriverHandle = drvObj->i2sDriverClientHandleWrite;
                }
                else if(DRV_IO_INTENT_READ == (ioIntent & DRV_IO_INTENT_READWRITE))
                {
                    hClient->ioIntent = DRV_IO_INTENT_READ;
                    drvObj->i2sDriverClientHandleRead = DRV_I2S_Open(drvObj->i2sDriverModuleIndex,
                        (DRV_IO_INTENT_READ | DRV_IO_INTENT_NONBLOCKING));
                    drvObj->i2sDriverHandle = drvObj->i2sDriverClientHandleRead;
                }
                else
                {
                    SYS_DEBUG(0, "i2s DRV_I2S_Open Error");
                }

                hClient->ioIntent |= DRV_IO_INTENT_NONBLOCKING;

                /* Remember which Generic Codec driver instance owns me */
                hClient->inUse  = true;
                hClient->hDriver = drvObj;
                hClient->pEventCallBack = NULL;
                //hClient->pReadEventCallBack = NULL;
                drvObj->numClients++;
                /* We have found a client object
                 * Release the mutex and return with
                 * the driver handle */
                /* An operation mode is needed */
                if((OSAL_MUTEX_Unlock(&(gDrvgenericcodecCommonDataObj.mutexClientObjects))) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG(0, "Unable to unlock open routine mutex \r\n");
                    return DRV_HANDLE_INVALID;
                }
                /* Return the client object */
                return (DRV_HANDLE) hClient;
            }
            hClient++;
        }
        /* Could not find a client object. Release the mutex and
         * return with an invalid handle. */
        if((OSAL_MUTEX_Unlock(&(gDrvgenericcodecCommonDataObj.mutexClientObjects))) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG(0, "Unable to unlock open routine mutex \r\n");
        }
    }    
    return DRV_HANDLE_INVALID;
}

// *****************************************************************************
/* Function:
    void DRV_GENERICCODEC_Close( DRV_Handle handle )

  Summary:
    Closes an opened-instance of the Generic Codec driver

  Description:
    This routine closes an opened-instance of the Generic Codec driver, invalidating the
    handle. Any buffers in the driver queue that were submitted by this client
    will be removed.  After calling this routine, the handle passed in "handle"
    must not be used with any of the remaining driver routines.  A new handle must
    be obtained by calling DRV_GENERICCODEC_Open before the caller may use the driver
    again

  Remarks:

    Usually there is no need for the driver client to verify that the Close
    operation has completed.  The driver will abort any ongoing operations
    when this routine is called.
*/
void DRV_GENERICCODEC_Close( const DRV_HANDLE handle)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    if(handle == DRV_HANDLE_INVALID || (DRV_HANDLE)NULL == handle)
    {
        SYS_DEBUG(0, "Invalid Driver Handle \r\n");
        return;
    }

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    if (false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid Driver Handle \r\n");
        return;
    }

    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    if(DRV_IO_INTENT_READ == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        DRV_I2S_Close (drvObj->i2sDriverClientHandleRead);
    }
    else if(DRV_IO_INTENT_WRITE == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        DRV_I2S_Close (drvObj->i2sDriverClientHandleWrite);

    }
    else if(DRV_IO_INTENT_READWRITE == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        DRV_I2S_Close (drvObj->i2sDriverHandle);
    }

    /* De-allocate the object */
    clientObj->inUse = false;
    /* Reduce the number of clients */
    drvObj->numClients--;
} /* DRV_GENERICCODEC_Close */

// *****************************************************************************
/*
Function:
	void DRV_GENERICCODEC_BufferAddWrite
	(
		const DRV_HANDLE handle,
		DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
		void *buffer, size_t size
	)

  Summary:
    Schedule a non-blocking driver write operation.

  Description:
    This function schedules a non-blocking write operation. The function returns
    with a valid buffer handle in the bufferHandle argument if the write request
    was scheduled successfully. The function adds the request to the hardware
    instance transmit queue and returns immediately. While the request is in the
    queue, the application buffer is owned by the driver and should not be
    modified.  The function returns DRV_GENERICCODEC_BUFFER_HANDLE_INVALID
    - if a buffer could not be allocated to the request
    - if the input buffer pointer is NULL
    - if the buffer size is 0.
    - if the queue is full or the queue depth is insufficient
    If the requesting client registered an event callback with the driver,
    the driver will issue a DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE event if the buffer
    was processed successfully of DRV_GENERICCODEC_BUFFER_EVENT_ERROR event if the
    buffer was not processed successfully.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec device instance and the DRV_GENERICCODEC_Status must have returned
    SYS_STATUS_READY.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

    DRV_IO_INTENT_WRITE ust have been specified in the DRV_GENERICCODEC_Open call.

  Parameters:
    handle       - Handle of the Generic Codec instance as return by the
                   DRV_GENERICCODEC_Open function.
    buffer       - Data to be transmitted.
    size         - Buffer size in bytes.
    bufferHandle - Pointer to an argument that will contain the
                   return buffer handle.

  Returns:
    The bufferHandle parameter will contain the return buffer handle. This will be
    DRV_GENERICCODEC_BUFFER_HANDLE_INVALID if the function was not successful.

   Remarks:
    This function is thread safe in a RTOS application. It can be called from
    within the Generic Codec Driver Buffer Event Handler that is registered by this
    client. It should not be called in the event handler associated with another
    Generic Codec driver instance. It should not otherwise be called directly in an ISR.

*/
void DRV_GENERICCODEC_BufferAddWrite
(
	const DRV_HANDLE handle,
	DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
	void *buffer, size_t size
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    /* The Client and driver objects from the handle */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    /* We first check the arguments and initialize the
     * buffer handle */
    if(bufferHandle != NULL)
    {
        *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
    }

    /* See if the handle is still valid */
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid Driver Handle \r\n");
        return;
    }

    /* Grab a mutex. */
    if (OSAL_MUTEX_Lock(&(drvObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        ;
    }
    else
    {
        /* The mutex acquisition timed out. Return with an
         * invalid handle. This code will not execute
         * if there is no RTOS. */
        return;
    }
    {
        DRV_I2S_BUFFER_HANDLE i2sBufferHandle = DRV_I2S_BUFFER_HANDLE_INVALID;
        DRV_I2S_WriteBufferAdd(drvObj->i2sDriverClientHandleWrite,
                (uint8_t *) buffer, size, &i2sBufferHandle);              

        if(i2sBufferHandle != DRV_I2S_BUFFER_HANDLE_INVALID)
        {
            *bufferHandle = (DRV_GENERICCODEC_BUFFER_HANDLE)i2sBufferHandle;
        }
        else
        {
            *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
        }
    }

    /* Release mutex */
    if((OSAL_MUTEX_Unlock(&(drvObj->mutexDriverInstance))) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "Unable to DriverInstance mutex \r\n");
    }
} /* DRV_GENERICCODEC_BufferAddWrite */

// *****************************************************************************
/* Function:
	void DRV_GENERICCODEC_BufferAddWriteRead
        (
                const DRV_HANDLE handle,
                DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
                void *transmitBuffer,
                void *receiveBuffer,
                size_t size
        )

  Summary:
    Schedule a non-blocking driver write-read operation.

  Description:
    This function schedules a non-blocking write-read operation. The function
    returns with a valid buffer handle in the bufferHandle argument if the
    write-read request was scheduled successfully. The function adds the request
    to the hardware instance queue and returns immediately. While the request is
    in the queue, the application buffer is owned by the driver and should not
    be modified. The function returns DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE:
    - if a buffer could not be allocated to the request
    - if the input buffer pointer is NULL
    - if the client opened the driver for read only or write only
    - if the buffer size is 0
    - if the queue is full or the queue depth is insufficient
    If the requesting client registered an event callback with the driver,
    the driver will issue a DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE event if the buffer
    was processed successfully of DDRV_GENERICCODEC_BUFFER_EVENT_ERROR event if the
    buffer was not processed successfully.

  Remarks:
    This function is thread safe in a RTOS application. It can be called from
    within the Generic Codec Driver Buffer Event Handler that is registered by this
    client. It should not be called in the event handler associated with another
    Generic Codec driver instance. It should not otherwise be called directly in an ISR.

    This function is useful when there is valid read expected for every
    Generic Codec write. The transmit and receive size must be same.

*/
void DRV_GENERICCODEC_BufferAddWriteRead
(
    const DRV_HANDLE handle,
    DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
    void *transmitBuffer, void *receiveBuffer,
    size_t size
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    /* The Client and driver objects from the handle */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    /* We first check the arguments and initialize the
     * buffer handle */
    if(bufferHandle != NULL)
    {
        *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
    }

    /* See if the handle is still valid */
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid Driver Handle \r\n");
        return;
    }

    /* Grab a mutex. */
    if (OSAL_MUTEX_Lock(&(drvObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        ;
    }
    else
    {
        /* The mutex acquisition timed out. Return with an
         * invalid handle. This code will not execute
         * if there is no RTOS. */
        return;
    }
    {
        DRV_I2S_BUFFER_HANDLE i2sBufferHandle = DRV_I2S_BUFFER_HANDLE_INVALID;
        DRV_I2S_WriteReadBufferAdd(drvObj->i2sDriverHandle,
                (uint8_t *) transmitBuffer, (uint8_t *) receiveBuffer, size, &i2sBufferHandle);          

        if(i2sBufferHandle != DRV_I2S_BUFFER_HANDLE_INVALID)
        {
            *bufferHandle = (DRV_GENERICCODEC_BUFFER_HANDLE)i2sBufferHandle;
        }
        else
        {
            *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
        }
    }

    /* Release mutex */
    if((OSAL_MUTEX_Unlock(&(drvObj->mutexDriverInstance))) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "Unable to DriverInstance mutex \r\n");
    }
} /* DRV_GENERICCODEC_BufferAddWriteRead */

// *****************************************************************************
/*
Function:
	void DRV_GENERICCODEC_BufferAddRead
	(
		const DRV_HANDLE handle,
		DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
		void *buffer, size_t size
	)

  Summary:
    Schedule a non-blocking driver write operation.

  Description:
    This function schedules a non-blocking write operation. The function returns
    with a valid buffer handle in the bufferHandle argument if the write request
    was scheduled successfully. The function adds the request to the hardware
    instance transmit queue and returns immediately. While the request is in the
    queue, the application buffer is owned by the driver and should not be
    modified.  The function returns DRV_GENERICCODEC_BUFFER_HANDLE_INVALID
    - if a buffer could not be allocated to the request
    - if the input buffer pointer is NULL
    - if the buffer size is 0.
    - if the queue is full or the queue depth is insufficient
    If the requesting client registered an event callback with the driver,
    the driver will issue a DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE event if the buffer
    was processed successfully of DRV_GENERICCODEC_BUFFER_EVENT_ERROR event if the
    buffer was not processed successfully.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec device instance and the DRV_GENERICCODEC_Status must have returned
    SYS_STATUS_READY.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

    DRV_IO_INTENT_WRITE ust have been specified in the DRV_GENERICCODEC_Open call.

  Parameters:
    handle       - Handle of the Generic Codec instance as return by the
                   DRV_GENERICCODEC_Open function.
    buffer       - Data to be transmitted.
    size         - Buffer size in bytes.
    bufferHandle - Pointer to an argument that will contain the
                   return buffer handle.

  Returns:
    The bufferHandle parameter will contain the return buffer handle. This will be
    DRV_GENERICCODEC_BUFFER_HANDLE_INVALID if the function was not successful.

   Remarks:
    This function is thread safe in a RTOS application. It can be called from
    within the Generic Codec Driver Buffer Event Handler that is registered by this
    client. It should not be called in the event handler associated with another
    Generic Codec driver instance. It should not otherwise be called directly in an ISR.

*/
void DRV_GENERICCODEC_BufferAddRead
(
	const DRV_HANDLE handle,
	DRV_GENERICCODEC_BUFFER_HANDLE *bufferHandle,
	void *buffer, size_t size
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    /* The Client and driver objects from the handle */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    /* We first check the arguments and initialize the
     * buffer handle */
    if(bufferHandle != NULL)
    {
        *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
    }

    /* See if the handle is still valid */
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid Driver Handle \r\n");
        return;
    }

    {
        DRV_I2S_BUFFER_HANDLE i2sBufferHandle = DRV_I2S_BUFFER_HANDLE_INVALID;
        DRV_I2S_ReadBufferAdd(drvObj->i2sDriverClientHandleRead,
                (uint8_t *) buffer, size, &i2sBufferHandle);        
            
        if(i2sBufferHandle != DRV_I2S_BUFFER_HANDLE_INVALID)
        {
            *bufferHandle = (DRV_GENERICCODEC_BUFFER_HANDLE)i2sBufferHandle;
        }
        else
        {
            *bufferHandle = DRV_GENERICCODEC_BUFFER_HANDLE_INVALID;
        }
    }
} /* DRV_GENERICCODEC_BufferAddRead */

// *****************************************************************************
/* Function:
    bool DRV_GENERICCODEC_WriteQueuePurge( const DRV_HANDLE handle )

  Summary:
    Removes all buffer requests from the write queue.

  Description:
    This function removes all the buffer requests from the write queue.
    The client can use this function to purge the queue on timeout or to remove
    unwanted stalled buffer requests or in any other use case.

  Precondition:
    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as returned by the
    DRV_GENERICCODEC_Open function.

  Returns:
    True - Write queue purge is successful.
    False - Write queue purge has failed.

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
*/

bool DRV_GENERICCODEC_WriteQueuePurge(const DRV_HANDLE handle)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    /* The Client and driver objects from the handle */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    return DRV_I2S_WriteQueuePurge(drvObj->i2sDriverClientHandleWrite);
}

// *****************************************************************************
/* Function:
    bool DRV_GENERICCODEC_ReadQueuePurge( const DRV_HANDLE handle )

  Summary:
    Removes all buffer requests from the read queue.

  Description:
    This function removes all the buffer requests from the read queue.
    The client can use this function to purge the queue on timeout or to remove
    unwanted stalled buffer requests or in any other use case.

  Precondition:
    DRV_I2S_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as returned by the
    DRV_GENERICCODEC_Open function.

  Returns:
    True - Read queue purge is successful.
    False - Read queue purge has failed.

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
*/

bool DRV_GENERICCODEC_ReadQueuePurge(const DRV_HANDLE handle)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    /* The Client and driver objects from the handle */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *) clientObj->hDriver;

    return DRV_I2S_ReadQueuePurge(drvObj->i2sDriverClientHandleRead);
}

// *****************************************************************************
/*
  Function:
	void DRV_GENERICCODEC_BufferEventHandlerSet
	(
		DRV_HANDLE handle,
		const DRV_GENERICCODEC_BUFFER_EVENT_HANDLER eventHandler,
		const uintptr_t contextHandle
	)

  Summary:
    This function allows a client to identify a buffer event handling function
    for the driver to call back when queued buffer transfers have finished.

  Description:
    This function allows a client to identify a buffer event handling function
    for the driver to call back when queued buffer transfers have finished.
    When a client calls DRV_GENERICCODEC_BufferAddWrite function, it is provided with
    a handle identifying  the buffer that was added to the driver's buffer queue.
    The driver will pass this handle back to the client by calling "eventHandler"
    function when the buffer transfer has completed.

    The event handler should be set before the client performs any "buffer add"
    operations that could generate events. The event handler once set, persists
    until the client closes the driver or sets another event handler (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    eventHandler - Pointer to the event handler function.
    context      - The value of parameter will be passed back to the client
                   unchanged, when the eventHandler function is called.  It can
                   be used to identify any client specific data object that
                   identifies the instance of the client module (for example,
                   it may be a pointer to the client module's state structure).

  Returns:
    None.

  Remarks:
    If the client does not want to be notified when the queued buffer transfer
    has completed, it does not need to register a callback.
*/
void DRV_GENERICCODEC_BufferEventHandlerSet
(
	DRV_HANDLE handle,
	const DRV_GENERICCODEC_BUFFER_EVENT_HANDLER eventHandler,
	const uintptr_t contextHandle
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    if((DRV_HANDLE_INVALID == handle) || (0 == handle))
    {
        /* This means the handle is invalid */
        SYS_DEBUG(0, "Handle is invalid \r\n");
        return;
    }

    /* Assing the event handler and the context */
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid driver handle \r\n");
        return;
    }
    
    drvObj = clientObj->hDriver;
    /* Set the Event Handler and context */
    clientObj->pEventCallBack = eventHandler;
    clientObj->hClientArg = contextHandle;

    if((clientObj->ioIntent & DRV_IO_INTENT_READWRITE) == DRV_IO_INTENT_READWRITE)
    {
        DRV_I2S_BufferEventHandlerSet(drvObj->i2sDriverHandle,
        (DRV_I2S_BUFFER_EVENT_HANDLER) _DRV_GENERICCODEC_I2SBufferEventHandler,
        (uintptr_t)(clientObj));
    }
    else if((clientObj->ioIntent & DRV_IO_INTENT_READWRITE) == DRV_IO_INTENT_WRITE)
    {
        DRV_I2S_BufferEventHandlerSet(drvObj->i2sDriverClientHandleWrite,
        (DRV_I2S_BUFFER_EVENT_HANDLER) _DRV_GENERICCODEC_I2SBufferEventHandler,
        (uintptr_t)(clientObj));
    }
    else if((clientObj->ioIntent & DRV_IO_INTENT_READWRITE) == DRV_IO_INTENT_READ)
    {
        DRV_I2S_BufferEventHandlerSet(drvObj->i2sDriverClientHandleRead,
        (DRV_I2S_BUFFER_EVENT_HANDLER) _DRV_GENERICCODEC_I2SBufferEventHandler,
        (uintptr_t)(clientObj));
    }
} /* DRV_GENERICCODEC_BufferEventHandlerSet */

void DRV_GENERICCODEC_I2SBufferHandlerSet
(
	DRV_HANDLE handle,
        DRV_I2S_BUFFER_EVENT_HANDLER I2SBufferEventHandler
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    if((DRV_HANDLE_INVALID == handle) || (0 == handle))
    {
        /* This means the handle is invalid */
        SYS_DEBUG(0, "Handle is invalid \r\n");
        return;
    }

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid driver handle \r\n");
        return;
    }
    drvObj = clientObj->hDriver;

    DRV_I2S_BufferEventHandlerSet(drvObj->i2sDriverHandle,
        (DRV_I2S_BUFFER_EVENT_HANDLER) I2SBufferEventHandler,
        (uintptr_t)(drvObj));
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_SetAudioCommunicationMode
(
    DRV_HANDLE handle, 
    const DATA_LENGTH dl, 
    const SAMPLE_LENGTH sl
)

  Summary:
    This function provides a run time audio format configuration

  Description:
    This function sets up audio mode in I2S protocol

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    dl           - Data length for I2S audio interface
    sl           - Left/Right Sample Length for I2S audio interface
  Returns:
    None

  Remarks:
    None.
*/

#define GENERICCODEC_NUMBER_AUDIOFORMAT_COMMANDS   1

// TO-DO!! - remove the #if 0 -- #endif when this function needs to be used
#if 0
static void _setAudioCommunicationFormat(DRV_GENERICCODEC_OBJ *drvObj, DRV_GENERICCODEC_AUDIO_DATA_FORMAT audioDataFormat, bool standalone)
{

    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_AUDIOFORMAT_COMMANDS];  

    // add commands to set audio format         

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
}
#endif

// *****************************************************************************
/*
  Function:
	void DRV_GENERICCODEC_CommandEventHandlerSet
	(
		DRV_HANDLE handle,
		const DRV_GENERICCODEC_COMMAND_EVENT_HANDLER eventHandler,
		const uintptr_t contextHandle
	)

  Summary:
    This function allows a client to identify a command event handling function
    for the driver to call back when the last submitted command have finished.

  Description:
    This function allows a client to identify a command event handling function
    for the driver to call back when the last submitted command have finished.

    When a client calls DRV_GENERICCODEC_BufferAddWrite function, it is provided with
    a handle identifying  the buffer that was added to the driver's buffer queue.
    The driver will pass this handle back to the client by calling "eventHandler"
    function when the buffer transfer has completed.

    The event handler should be set before the client performs any "Generic Codec
    Specific Client Routines" operations that could generate events.
    The event handler once set, persists until the client closes the driver or
    sets another event handler (which could be a "NULL" pointer to indicate no callback).

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    eventHandler - Pointer to the event handler function.
    context      - The value of parameter will be passed back to the client
                   unchanged, when the eventHandler function is called.  It can
                   be used to identify any client specific data object that
                   identifies the instance of the client module (for example,
                   it may be a pointer to the client module's state structure).

  Returns:
    None.

  Remarks:
    If the client does not want to be notified when the command
    has completed, it does not need to register a callback.
*/
void DRV_GENERICCODEC_CommandEventHandlerSet
(
	DRV_HANDLE handle,
	const DRV_GENERICCODEC_COMMAND_EVENT_HANDLER eventHandler,
	const uintptr_t contextHandle
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    DRV_GENERICCODEC_OBJ *drvObj;

    if((DRV_HANDLE_INVALID == handle) || (0 == handle))
    {
        /* This means the handle is invalid */
        SYS_DEBUG(0, "Handle is invalid \r\n");
        return;
    }

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    /* assigning the event handler and the context */
    if(false == clientObj->inUse)
    {
        SYS_DEBUG(0, "Invalid driver handle \r\n");
        return;
    }

    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;
    drvObj->commandCompleteCallback = eventHandler;
    drvObj->commandContextData = contextHandle;
}

// *****************************************************************************
// *****************************************************************************
// Section: Generic Codec Version Information Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/*
  Function:
    int8_t* DRV_GENERICCODEC_VersionStrGet(void)

  Summary:
    This function returns the version of Generic Codec driver in string format.

  Description:
    The DRV_GENERICCODEC_VersionStrGet function returns a string in the format:
    "<major>.<minor>[.<patch>][<type>]"
    Where:
        <major> is the Generic Codec driver's version number.
        <minor> is the Generic Codec driver's version number.
        <patch> is an optional "patch" or "dot" release number (which is not
        included in the string if it equals "00").
        <type> is an optional release type ("a" for alpha, "b" for beta ?
        not the entire word spelled out) that is not included if the release
        is a production version (I.e. Not an alpha or beta).

        The String does not contain any spaces.

        Example:
        "0.03a"
        "1.00"

  Precondition:
    None

  Parameters:
    None

  Returns: returns a string containing the version of Generic Codec driver.

  Example:
    <code>
        int8_t *GENERICCODECstring;
        GENERICCODECstring = DRV_GENERICCODEC_VersionStrGet();
    </code>

  Remarks:
    None
 */
int8_t* DRV_GENERICCODEC_VersionStrGet(void)
{
    return (int8_t*) _DRV_GENERICCODEC_VERSION_STR;
}


// *****************************************************************************
/*
  Function:
    uint32_t DRV_GENERICCODEC_VersionGet( void )

  Summary:
    This function returns the version of Generic Codec driver

  Description:
    The version number returned from the DRV_GENERICCODEC_VersionGet function is an
    unsigned integer in the following decimal format.
    <major> * 10000 + <minor> * 100 + <patch>

    Where the numbers are represented in decimal and the meaning is the same as
    above.  Note that there is no numerical representation of release type.

    Example:
    For version "0.03a", return:  0 * 10000 + 3 * 100 + 0
    For version "1.00", return:  1 * 100000 + 0 * 100 + 0


  Precondition:
    None

  Parameters:
    None

  Returns: returns the version of Generic Codec driver.

  Example:
    <code>
        uint32_t GENERICCODECversion;
        GENERICCODECversion = DRV_GENERICCODEC_VersionGet();
    </code>

  Remarks:
    None
 */
uint32_t DRV_GENERICCODEC_VersionGet(void)
{
    return (_DRV_GENERICCODEC_VERSION_MAJOR * 10000 +  \
            _DRV_GENERICCODEC_VERSION_MINOR * 100 + \
            _DRV_GENERICCODEC_VERSION_PATCH);
}

// *****************************************************************************
// *****************************************************************************
// Section: GENERICCODEC Codec Specific Client Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_SamplingRateSet(DRV_HANDLE handle, uint32_t samplingRate)

  Summary:
    This function sets the sampling rate of the media stream.

  Description:
    This function sets the media sampling rate for the client handle.  Only used
    Generic Codec in master mode.

  Remarks:
    None.
*/   
#define GENERICCODEC_NUMBER_SAMPLERATE_COMMANDS   1     // modify as needed

void _samplingRateSet(DRV_GENERICCODEC_OBJ *drvObj, uint32_t sampleRate, bool standalone)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0
    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_SAMPLERATE_COMMANDS];                                      

    // add commands to set sample rate, if any            

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif
}

void DRV_GENERICCODEC_SamplingRateSet(DRV_HANDLE handle, uint32_t sampleRate)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;
    
    _samplingRateSet(drvObj, sampleRate, true);
}

// *****************************************************************************
/*
  Function:
    uint32_t DRV_GENERICCODEC_SamplingRateGet(DRV_HANDLE handle)

  Summary:
    This function gets the sampling rate set on the DAC Generic Codec

  Description:
    This function gets the sampling rate set on the DAC Generic Codec.

  Remarks:
    None.
 */
uint32_t DRV_GENERICCODEC_SamplingRateGet(DRV_HANDLE handle)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    /* Return the sampling rate */
    return drvObj->samplingRate;
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_VolumeSet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL chan, uint8_t volume)

  Summary:
    This function sets the volume for Generic Codec CODEC.

  Description:
    This functions sets the volume value from 0-255

  Remarks:
    None.
*/
#define GENERICCODEC_NUMBER_VOLUME_COMMANDS   1     // modify as needed

static void _volumeSet(DRV_GENERICCODEC_OBJ *drvObj, DRV_GENERICCODEC_CHANNEL chan, uint8_t volume, bool standalone)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0
    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_VOLUME_COMMANDS];
    
    //uint8_t indexedVolume = volumeSteps[volume];   // convert external value to internal if needed

    // add commands to set volume, if any            

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif 
}

void DRV_GENERICCODEC_VolumeSet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL chan, uint8_t volume)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    _volumeSet(drvObj, chan, volume, true);
}

// *****************************************************************************
/*
  Function:
    uint8_t DRV_GENERICCODEC_VolumeGet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL chan)

  Summary:
    This function gets the volume for Generic Codec CODEC.

  Description:
    This functions gets the current volume programmed to the DAC Generic Codec.

  Remarks:
    None.
 */
uint8_t DRV_GENERICCODEC_VolumeGet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL chan)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;    

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    /* Return the volume */
    return drvObj->volume[chan];
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MuteOn(DRV_HANDLE handle);

  Summary:
    This function allows Generic Codec output for soft mute on.

  Description:
    This function Enables Generic Codec output for soft mute.

  Remarks:
    None.
*/
#define GENERICCODEC_NUMBER_MUTEON_COMMANDS   1  // modify as needed

void DRV_GENERICCODEC_MuteOn(DRV_HANDLE handle)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0 
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;
   
    // add commands to mute output, if any            

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value =        // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif   
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MuteOff(DRV_HANDLE handle)

  Summary:
    This function disables Generic Codec output for soft mute.

  Description:
    This function disables Generic Codec output for soft mute.

  Remarks:
    None.
*/
#define GENERICCODEC_NUMBER_MUTEOFF_COMMANDS   1  // modify as needed

void DRV_GENERICCODEC_MuteOff(DRV_HANDLE handle)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0 
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    clientObj       = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj          = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_MUTEOFF_COMMANDS];  
    
    // add commands to unmute output, if any            

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif 
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MicGainSet(DRV_HANDLE handle, uint8_t gain)

  Summary:
    This function sets the microphone gain for the Generic Codec CODEC.

  Description:
    This functions sets the microphone gain value from 0-31

  Remarks:
    None.
*/
#define GENERICCODEC_NUMBER_MICGAIN_COMMANDS   1    // modify as needed

static void _micGainSet(DRV_GENERICCODEC_OBJ *drvObj, uint8_t gain, bool standalone)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0
    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_MICGAIN_COMMANDS];

    // add commands to set mic gain, if any            

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif
}

void DRV_GENERICCODEC_MicGainSet(DRV_HANDLE handle, uint8_t gain)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;
    
    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    _micGainSet(drvObj, gain, true);
}

// *****************************************************************************
/*
  Function:
    uint8_t DRV_GENERICCODEC_MicGainGet(DRV_HANDLE handle)

  Summary:
    This function gets the microphone gain for the Generic Codec.

  Description:
    This functions gets the current microphone gain programmed to the Generic Codec.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    GENERICCODEC driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    Microphone gain, in range 0-31

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;
    uint8_t gain;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

      gain = DRV_GENERICCODEC_MicGainGet(myGENERICCODECHandle);
    </code>

  Remarks:
    None.
 */
uint8_t DRV_GENERICCODEC_MicGainGet(DRV_HANDLE handle)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;    

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    /* Return the gain */
    return drvObj->micGain;
}

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MicMuteOn(DRV_HANDLE handle);

  Summary:
    Mutes the GENERICCODEC's microphone input

  Description:
    This function mutes the Generic Codec's microphone input

  Remarks:
    None.
 */
#define GENERICCODEC_NUMBER_MICMUTEON_COMMANDS   1      // modify as needed

void DRV_GENERICCODEC_MicMuteOn(DRV_HANDLE handle)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0 
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;    

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_MICMUTEON_COMMANDS];

    // add commands to unmute mic, if any

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif
}

// *****************************************************************************
/*
  Function:
        void DRV_GENERICCODEC_MicMuteOff(DRV_HANDLE handle)

  Summary:
    Umutes th Generic Codec's microphone input.

  Description:
    This function unmutes the Generic Codec's microphone input.

  Remarks:
    None.
 */
#define GENERICCODEC_NUMBER_MICMUTEON_COMMANDS   1  // modify as needed

void DRV_GENERICCODEC_MicMuteOff(DRV_HANDLE handle)
{
// TO-DO!! - remove the #if 0 -- #endif and modify the code as needed for your codec
#if 0 
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;    

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_Commands[GENERICCODEC_NUMBER_MICMUTEON_COMMANDS];

    // add commands to mute mic, if any

    uint8_t loopSize;

    GENERICCODEC_I2C_Commands[0].reg_addr = 0;  // reg # goes here
    GENERICCODEC_I2C_Commands[0].value = 0;     // reg value goes here
    loopSize = 1;  // modify as needed      

    uint8_t i;                                      
    for (i=0; i < loopSize; i++)
    {
        GENERICCODEC_I2C_Commands[i].delay = (i==loopSize-1) ? 5 : 2;             
        add_I2C_Command(drvObj, GENERICCODEC_I2C_Commands[i], (i==loopSize-1));   // add commands to buffer
    }

    start_I2C_Commands(drvObj);   
#endif
}

// *****************************************************************************
/*
  Function:
    DRV_HANDLE DRV_GENERICCODEC_GetI2SDriver(DRV_HANDLE codecHandle)

  Summary:
    Get the handle to the I2S driver for this codec instance.

  Description:
    Returns the appropriate handle to the I2S based on the ioIent member
    of the codec object.

  Remarks:
    This allows the caller to directly access portions of the I2S driver that
    might not be available via the codec API.
*/
DRV_HANDLE DRV_GENERICCODEC_GetI2SDriver(DRV_HANDLE codecHandle)
{
    DRV_GENERICCODEC_CLIENT_OBJ* clientObj = (DRV_GENERICCODEC_CLIENT_OBJ*)codecHandle; 
    DRV_GENERICCODEC_OBJ* drvObj = (DRV_GENERICCODEC_OBJ*)clientObj->hDriver;

    if(DRV_IO_INTENT_READ == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        return drvObj->i2sDriverClientHandleRead;
    }
    else if(DRV_IO_INTENT_WRITE == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        return drvObj->i2sDriverClientHandleWrite;

    }
    else if(DRV_IO_INTENT_READWRITE == (clientObj->ioIntent & DRV_IO_INTENT_READWRITE))
    {
        return drvObj->i2sDriverHandle;
    }
    return (DRV_HANDLE)NULL;
} 

// *****************************************************************************
/* Function:
    uint32_t DRV_GENERICCODEC_LRCLK_Sync (const DRV_HANDLE handle);
    
  Summary:
    Synchronize to the start of the I2S LRCLK (left/right clock) signal
    
  Description:
    This function waits until low-to high transition of the I2S LRCLK (left/right clock)
    signal (high-low if Left-Justified format, this is determined by the PLIB).
    In the case where this signal is generated from a codec or other external
    source, this allows the caller to synchronize calls to the DMA with the LRCLK signal
    so the left/right channel association is valid.
    
  Remarks:
    None.                                            
*/
bool DRV_GENERICCODEC_LRCLK_Sync (const DRV_HANDLE handle)
{
    DRV_GENERICCODEC_OBJ *drvObj;
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *) handle;
    drvObj = (DRV_GENERICCODEC_OBJ *)clientObj->hDriver;

    // if no sampling rate set, pass something reasonable to calculate delay times
    uint32_t samplingRate = (drvObj->samplingRate) ? drvObj->samplingRate : 48000;

    return DRV_I2S_LRCLK_Sync (drvObj->i2sDriverHandle, samplingRate);
}

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// register contents for initialization of Generic Codec
static const GENERICCODEC_I2C_COMMAND_BUFFER GENERICCODEC_I2C_InitializationCommands1[] =
{
    // format is register name, value, delay in ms after command
    // define registers and values in drv_genericcodec_local.h

    { 0/*GENERICCODEC_SW_RESET*/, 0, 100 },
};

// add more as needed

// *****************************************************************************
 /*
  Function:
    static void _DRV_GENERICCODEC_ControlTasks(DRV_GENERICCODEC_OBJ *drvObj)

  Summary:
    Implements the state machine for the Audio control interface of Generic Codec

  Description:
    Implements the state machine for the Audio control interface of Generic Codec

  Remarks:
    None
*/

static void _DRV_GENERICCODEC_ControlTasks (DRV_GENERICCODEC_OBJ *drvObj)
{
    /* Check the application's current state. */
    switch ( drvObj->currentState )
    {
        /* Application's initial state. */
        case DRV_GENERICCODEC_STATE_INIT:
        {
            /* Open the timer Driver */
            drvObj->tmrHandle = SYS_TIME_CallbackRegisterMS(GENERICCODEC_TimerCallback, 
                    (uintptr_t)drvObj, 1/*ms*/, SYS_TIME_PERIODIC);

            if ( SYS_TIME_HANDLE_INVALID == drvObj->tmrHandle )
            {
                return;
            }
    
            drvObj->currentState = DRV_GENERICCODEC_STATE_OPEN;
        }
        break;
        
        /* Application's initial state. */
        case DRV_GENERICCODEC_STATE_OPEN:
        {
            /* Open the I2C Driver */
            drvObj->drvI2CHandle = DRV_I2C_Open( DRV_I2C_INDEX_0,
                    DRV_IO_INTENT_READWRITE );
            
            if ( DRV_HANDLE_INVALID == drvObj->drvI2CHandle )
            {
                return;
            }
            
            /* Register the I2C Driver event Handler */
            DRV_I2C_TransferEventHandlerSet( drvObj->drvI2CHandle, 
                                             GENERICCODEC_I2CEventHandler, (uintptr_t)drvObj);
            
            drvObj->currentState = DRV_GENERICCODEC_STATE_DATA_SETUP;
        }
        break;                      

        case DRV_GENERICCODEC_STATE_DATA_SETUP:
        {
/*  TO-DO!!
  This the state where commands are sent to initialize the codec.  They are setup in one or more arrays defined earlier like:

static const GENERICCODEC_I2C_InitializationCommands5[] =
{     
	{ WM8904_ANALOGUE_OUT1_LEFT, WM8904_HPOUTL_VOL(0x39), 2 },
	{ WM8904_ANALOGUE_OUT1_RIGHT, WM8904_HPOUTR_VOL(0x39), 2 }, 
	{ WM8904_ANALOGUE_OUT1_LEFT, WM8904_HPOUT_VU | WM8904_HPOUTL_VOL(0x39), 2 },
	{ WM8904_ANALOGUE_OUT1_RIGHT, WM8904_HPOUT_VU | WM8904_HPOUTR_VOL(0x39), 100 }     
};

where the first item in a line is the register, the next is a value, and the last is a delay in ms to wait after the command
is sent.  The registers and value-setting macros whould be defined as #defines in drv_genericcode_locl.h.
*/
            uint8_t i, loopSize;                     
            // get # of entries in table                     
            loopSize = sizeof(GENERICCODEC_I2C_InitializationCommands1) / sizeof(GENERICCODEC_I2C_InitializationCommands1[0]);

            // add more as needed, e.g.
            // loopSize += sizeof(GENERICCODEC_I2C_InitializationCommands2) / sizeof(GENERICCODEC_I2C_InitializationCommands2[0]);

            // if local routines are called, add the length of their out as well:
            // loopSize += GENERICCODEC_NUMBER_AUDIOFORMAT_COMMANDS;        
            // loopSize += GENERICCODEC_NUMBER_VOLUME_COMMANDS;
            // loopSize += GENERICCODEC_NUMBER_SAMPLERATE_COMMANDS;

            if (loopSize <= GENERICCODEC_MAX_I2C_COMMANDS)
            {
                loopSize = sizeof(GENERICCODEC_I2C_InitializationCommands1) / sizeof(GENERICCODEC_I2C_InitializationCommands1[0]);                        
                for (i=0; i < loopSize; i++)
                {
                    add_I2C_Command(drvObj, GENERICCODEC_I2C_InitializationCommands1[i], false);   // add commands to buffer

                }
        
                // add more as needed, e.g.
                //loopSize = sizeof(GENERICCODEC_I2C_InitializationCommands2) / sizeof(GENERICCODEC_I2C_InitializationCommands2[0]);                        
                //for (i=0; i < loopSize; i++)
                //{
                //    add_I2C_Command(drvObj, GENERICCODEC_I2C_InitializationCommands2[i], false);   // add commands to buffer
                //}

                // and/or calls to private functions as needed
                // _volumeSet(drvObj, DRV_GENERICCODEC_CHANNEL_LEFT_RIGHT,
                //            drvObj->volume[DRV_GENERICCODEC_CHANNEL_LEFT_RIGHT], false);
                // _setAudioCommunicationFormat(drvObj, drvObj->audioDataFormat, false);                       
                // _samplingRateSet(drvObj, drvObj->samplingRate, false); 

                drvObj->I2C_loopCount = 0;                      
                drvObj->currentState = DRV_GENERICCODEC_STATE_DATA_TRANSMIT;      // go transmit stored commands                                                                         
            }
            else
            {
                drvObj->currentState = DRV_GENERICCODEC_STATE_ERROR;
            }                   
         }
        break;
        
        case DRV_GENERICCODEC_STATE_DATA_TRANSMIT:
        {           
            if(drvObj->I2C_loopCount >= drvObj->I2C_endLoop)
            {
                drvObj->I2C_loopCount = drvObj->I2C_endLoop = 0;      // for next sequence of commands
                drvObj->currentState = DRV_GENERICCODEC_STATE_DONE;
                
                // will also be redundantly set at end of any new command sequences
                drvObj->status = SYS_STATUS_READY;
                return;
            }
            else               
            {                                
                for (; drvObj->I2C_loopCount < drvObj->I2C_endLoop;) 
                {
                   if (drvObj->I2C_Commands[drvObj->I2C_loopCount].reg_addr <= 255)                                                        
                   {                           
                        uint16_t dataVar16;
                        dataVar16 = drvObj->I2C_Commands[drvObj->I2C_loopCount].value;
                        drvObj->tmpData[0] = drvObj->I2C_Commands[drvObj->I2C_loopCount].reg_addr;                     
                        drvObj->tmpData[1] = (dataVar16 & 0xff00) >> 8;
                        drvObj->tmpData[2] = dataVar16 & 0xff;

                        DRV_I2C_WriteTransferAdd( drvObj->drvI2CHandle,
                                                  APP_CODEC_GENERICCODEC_DEVICE_ADDR,
                                                  (uint8_t *)&drvObj->tmpData,
                                                  APP_WRITE_DATA_LENGTH,
                                                  &drvObj->hWriteTransfer ); 

                        if( drvObj->hWriteTransfer == DRV_I2C_TRANSFER_HANDLE_INVALID )
                        {
                            drvObj->currentState = DRV_GENERICCODEC_STATE_ERROR;
                            break;
                        }
                        else
                        {
                            drvObj->GENERICCODECDelay = drvObj->I2C_Commands[drvObj->I2C_loopCount].delay;
                            if (drvObj->GENERICCODECDelay==0)
                            {
                                drvObj->GENERICCODECDelay = 2;      // always delay at least 2 ms    
                            }
                            drvObj->I2C_loopCount++;
                            drvObj->I2C_lastCommand = drvObj->I2C_loopCount;    // 1 ahead so 0 can be used as flag
                            drvObj->currentState = DRV_GENERICCODEC_STATE_UPDATE;
                            break;
                        }                                    
                    }
                    else
                    {
                        drvObj->currentState = DRV_GENERICCODEC_STATE_ERROR;
                        break;
                    }
                }              
            }                        
        }
        break;
                       
        case DRV_GENERICCODEC_STATE_UPDATE:
        {
            if (drvObj->GENERICCODECDelay)
            {
                break;
            } 
            
            if( drvObj->writeComplete )
            {
                /* Write is complete, verify if Codec is busy*/
                if (drvObj->I2C_loopCount == 0)
                {
                    drvObj->currentState = DRV_GENERICCODEC_STATE_DONE;
                }
                else
                {
                    if (drvObj->I2C_lastCommand)
                    {
                        if (drvObj->I2C_Commands[drvObj->I2C_lastCommand-1].end)
                        {                    
                            if (drvObj->commandCompleteCallback != (DRV_GENERICCODEC_COMMAND_EVENT_HANDLER) 0)
                            {
                                drvObj->commandCompleteCallback(drvObj->commandContextData);
                            }
                        }
                        drvObj->I2C_lastCommand = 0;    // mark processed                
                    }
                    // ready to execute next command in sequence (if any))                  
                    drvObj->currentState = DRV_GENERICCODEC_STATE_DATA_TRANSMIT;    
                }
                drvObj->writeComplete = false;
            }
            else if( drvObj->readComplete )
            {
                /* read is complete -- not currently used*/
                drvObj->currentState = DRV_GENERICCODEC_STATE_DONE;
                drvObj->readComplete = false;
            }    
        }
        break;               
        
        case DRV_GENERICCODEC_STATE_ERROR:
        {
        }
        break;
        
        case DRV_GENERICCODEC_STATE_DONE:
        {
        }
        break;        
                
        default:
        {
        }
        break;
    }
    
    return;
}

static bool add_I2C_Command(DRV_GENERICCODEC_OBJ *drvObj, GENERICCODEC_I2C_COMMAND_BUFFER I2C_CommandPtr, bool end)
{
    if (drvObj->I2C_endLoop < GENERICCODEC_MAX_I2C_COMMANDS)
    {
        drvObj->I2C_Commands[drvObj->I2C_endLoop] = I2C_CommandPtr;     // structure copy
        drvObj->I2C_Commands[drvObj->I2C_endLoop].end = end;            // mark last command in series
        drvObj->I2C_endLoop++;        
        return true;
    }
    return false;
}

// TO-DO!! - remove the #if 0 -- #endif when this function is to be used to send I2C commands to your codec
#if 0
static void start_I2C_Commands(DRV_GENERICCODEC_OBJ *drvObj)
{
    if (drvObj->currentState == DRV_GENERICCODEC_STATE_DONE)
    {
        drvObj->currentState = DRV_GENERICCODEC_STATE_DATA_TRANSMIT;     // restart machine
    }
    // otherwise commands have been added to end of current queue and will be executed
}
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

/// *****************************************************************************
 /*
  Function:
        static void _DRV_GENERICCODEC_I2SBufferEventHandler
        (
            DRV_I2S_BUFFER_EVENT event,
            DRV_I2S_BUFFER_HANDLE bufferHandle,
            uintptr_t contextHandle
        )

  Summary:
    Implements the handler for i2s buffer request completion.

  Description:
    Implements the handler for i2s buffer request completion.

  Remarks:
    None
*/
static void _DRV_GENERICCODEC_I2SBufferEventHandler
(
    DRV_I2S_BUFFER_EVENT event,
    DRV_I2S_BUFFER_HANDLE bufferHandle,
    uintptr_t contextHandle
)
{
    DRV_GENERICCODEC_CLIENT_OBJ *clientObj;

    if(DRV_I2S_BUFFER_HANDLE_INVALID == bufferHandle ||
            0 == contextHandle )
    {
        /* This means the handle is invalid */
        SYS_DEBUG(0, "Handle is invalid \r\n");
        return;
    }

    clientObj = (DRV_GENERICCODEC_CLIENT_OBJ *)contextHandle;
    if(DRV_I2S_BUFFER_EVENT_COMPLETE == event)
    {
        clientObj->pEventCallBack(DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE,
            (DRV_GENERICCODEC_BUFFER_HANDLE) bufferHandle, clientObj->hClientArg);
    }
    else if(DRV_I2S_BUFFER_EVENT_ABORT == event)
    {
        clientObj->pEventCallBack(DRV_GENERICCODEC_BUFFER_EVENT_ABORT,
            (DRV_GENERICCODEC_BUFFER_HANDLE) bufferHandle, clientObj->hClientArg);
    }
    else if(DRV_I2S_BUFFER_EVENT_ERROR == event)
    {
        clientObj->pEventCallBack(DRV_GENERICCODEC_BUFFER_EVENT_ERROR,
            (DRV_GENERICCODEC_BUFFER_HANDLE) bufferHandle, clientObj->hClientArg);
    }
    else
    {
        ;
    }
    return;
}


