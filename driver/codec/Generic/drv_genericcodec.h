/*******************************************************************************
  Generic Codec Driver Interface

  Company:
    Microchip Technology Inc.

  File Name:
    drv_genericcodec.h

  Summary:
    Generic Codec Driver Interface header file

  Description:
    The Generic Codec device driver interface provides a simple interface to
    manage a codec that can be interfaced to a Microchip microcontroller. 
    This file provides the public interface definitions for the Generic Codec 
    device driver.
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

#ifndef _DRV_GENERICCODEC_H
#define _DRV_GENERICCODEC_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "system/system_module.h"
#include "system/int/sys_int.h"
#include "driver/driver_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

#define DRV_I2C_INDEX DRV_GENERICCODEC_I2C_INSTANCES_NUMBER

/* Driver Generic Codec Module Index

  Summary:
    Generic Codec driver index definitions

  Description:
    These constants provide Generic Codec driver index definition.

  Remarks:
    These constants should be used in place of hard-coded numeric literals.
    These values should be passed into the DRV_GENERICCODEC_Initialize and
    DRV_GENERICCODEC_Open routines to identify the driver instance in use.
 */
 
#define      DRV_GENERICCODEC_INDEX_0      0
#define      DRV_GENERICCODEC_INDEX_1      1
#define      DRV_GENERICCODEC_INDEX_2      2
#define      DRV_GENERICCODEC_INDEX_3      3
#define      DRV_GENERICCODEC_INDEX_4      4
#define      DRV_GENERICCODEC_INDEX_5      5


// *****************************************************************************
/* Generic Codec Driver Module Count

  Summary:
    Number of valid Generic Codec driver indices

  Description:
    This constant identifies the maximum number of Generic Codec Driver instances that
    should be defined by the application. Defining more instances than this
    constant will waste RAM memory space.

    This constant can also be used by the application to identify the number of
    Generic Codec instances on this microcontroller.

  Remarks:
    This value is part-specific.

 */
#define DRV_GENERICCODEC_COUNT /*DOM-IGNORE-BEGIN*/ DRV_GENERICCODEC_INSTANCES_NUMBER /*DOM-IGNORE-END*/


// *****************************************************************************
/* Generic Codec Driver Buffer Handle

  Summary:
    Handle identifying a write buffer passed to the driver.

  Description:
    A buffer handle value is returned by a call to the DRV_GENERICCODEC_BufferAddWrite()
    or DRV_GENERICCODEC_BufferAddRead() function. This handle is associated with the 
    buffer passed into the function and it allows the application to track the 
    completion of the data from (or into) that buffer.
 
    The buffer handle value returned from the "buffer add" function is returned 
    back to the client by the "event handler callback" function registered with
    the driver.

    The buffer handle assigned to a client request expires when the client has
    been notified of the completion of the buffer transfer (after event handler
    function that notifies the client returns) or after the buffer has been
    retired by the driver if no event handler callback was set.

  Remarks:
    None
 */
typedef uintptr_t DRV_GENERICCODEC_BUFFER_HANDLE;


// *****************************************************************************
/* Generic Codec Driver Invalid Buffer Handle

  Summary:
    Definition of an invalid buffer handle.

  Description:
    This is the definition of an invalid buffer handle. An invalid buffer handle
    is returned by DRV_GENERICCODEC_BufferAddWrite() and the DRV_GENERICCODEC_BufferAddRead()
    function if the buffer add request was not successful.

  Remarks:
    None.
 */
#define DRV_GENERICCODEC_BUFFER_HANDLE_INVALID ((DRV_GENERICCODEC_BUFFER_HANDLE)(-1))

// *****************************************************************************
/* Generic Codec Driver Events

   Summary:
    Identifies the possible events that can result from a buffer add request.

   Description:
    This enumeration identifies the possible events that can result from a
    buffer add request caused by the client calling either the
    DRV_GENERICCODEC_BufferAddWrite() or the DRV_GENERICCODEC_BufferAddRead() function.

   Remarks:
    One of these values is passed in the "event" parameter of the event
    handling callback function that the client registered with the driver by
    calling the DRV_GENERICCODEC_BufferEventHandlerSet function when a buffer
    transfer request is completed.

 */
typedef enum {
    /* Data was transferred successfully. */
    DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE,

    /* Error while processing the request */
    DRV_GENERICCODEC_BUFFER_EVENT_ERROR,

    /* Data transfer aborted (Applicable in DMA mode) */
    DRV_GENERICCODEC_BUFFER_EVENT_ABORT

} DRV_GENERICCODEC_BUFFER_EVENT;


typedef enum
{
    DATA_LENGTH_16,
    DATA_LENGTH_24,
    DATA_LENGTH_32,
}DATA_LENGTH; // in bits

// *****************************************************************************
/* Generic Codec Driver Buffer Event Handler Function
  Summary:
    Pointer to a Generic Codec Driver Buffer Event handler function

  Description:
    This data type defines the required function signature for the Generic Codec driver
    buffer event handling callback function. A client must register a pointer
    to a buffer event handling function who's function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive buffer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    event           - Identifies the type of event

    bufferHandle    - Handle identifying the buffer to which the event relates

    context         - Value identifying the context of the application that registered
                      the event handling function.

  Returns:
    None.

  Example:
    <code>
    void DRV_GENERICCODECBufferEventHandler( DRV_GENERICCODEC_BUFFER_EVENT event,
                                   DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle,
                                   uintptr_t context )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;

        switch(event)
        {
            case DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE:
                // Handle the completed buffer.
            break;

            case DRV_GENERICCODEC_BUFFER_EVENT_ERROR:
            default:
                // Handle error.
            break;
        }
    }
    </code>

   Remarks:
    If the event is DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE, this means that the data
    was transferred successfully.

    If the event is DRV_GENERICCODEC_BUFFER_EVENT_ERROR, this means that the data was
    not transferred successfully. The bufferHandle parameter contains the buffer
    handle of the buffer that failed. The DRV_GENERICCODEC_BufferProcessedSizeGet()
    function can be called to find out how many bytes were processed.

    The bufferHandle parameter contains the buffer handle of the buffer that
    associated with the event.

    The context parameter contains a handle to the client context,
    provided at the time the event handling function was  registered using the
    DRV_GENERICCODEC_BufferEventHandlerSet function.  This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the buffer add request.

    The buffer handle in bufferHandle expires after this event handler exits. In
    that the buffer object that was allocated is deallocated by the driver
    after the event handler exits.

    The event handler function executes in the data driver(i2S) peripheral's
    interrupt context when the driver is configured for interrupt mode operation.
    It is recommended of the application to not perform process intensive or
    blocking operations with in this function.

    DRV_GENERICCODEC_BufferAddWrite function can be called in the event handler
    to add a buffer to the driver queue.

 */
typedef void (*DRV_GENERICCODEC_BUFFER_EVENT_HANDLER) (DRV_GENERICCODEC_BUFFER_EVENT event,
        DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle);


// *****************************************************************************
/* Generic Codec Driver Command Event Handler Function

  Summary:
    Pointer to a Generic Codec Driver Command Event Handler Function

  Description:
    This data type defines the required function signature for the Generic Codec driver
    command event handling callback function.

    A command is a control instruction to the Generic Codec.
    Example Mute ON/OFF, Zero Detect Enable/Disable etc.

    A client must register a pointer to a command event handling function who's
    function signature (parameter and return value types) match the types
    specified by this function pointer in order to receive command related event
    calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    context         - Value identifying the context of the application that 
                      registered the event handling function.

  Returns:
    None.

  Example:
    <code>
    void DRV_GENERICCODECCommandEventHandler( uintptr_t context )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;

        // Last Submitted command is completed.
        // Perform further processing here
    }
    </code>

   Remarks:

    The occurrence of this call back means that the last control command was
    transferred successfully.

    The context parameter contains a handle to the client context,
    provided at the time the event handling function was  registered using the
    DRV_GENERICCODEC_CommandEventHandlerSet function.  This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the buffer add request.

    The event handler function executes in the control data driver
    interrupt context. It is recommended of the application to not perform
    process intensive or blocking operations with in this function.

 */
typedef void (*DRV_GENERICCODEC_COMMAND_EVENT_HANDLER) (uintptr_t contextHandle);


// *****************************************************************************
/* Generic Codec Audio data format

  Summary:
    Identifies the Serial Audio data interface format.

  Description:
    This enumeration identifies Serial Audio data interface format.

  Remarks:
 
 */
typedef enum {
    DATA_16_BIT_LEFT_JUSTIFIED,
    DATA_16_BIT_I2S,
    DATA_32_BIT_LEFT_JUSTIFIED,
    DATA_32_BIT_I2S

} DRV_GENERICCODEC_AUDIO_DATA_FORMAT;

// *****************************************************************************
/* Generic Codec Audio Channel

  Summary:
    Identifies Left/Right Audio channel
  Description:
    This enumeration identifies Left/Right Audio channel

  Remarks:
    None.
 */
typedef enum {
    DRV_GENERICCODEC_CHANNEL_LEFT,

    DRV_GENERICCODEC_CHANNEL_RIGHT,

    DRV_GENERICCODEC_CHANNEL_LEFT_RIGHT,

    DRV_GENERICCODEC_NUMBER_OF_CHANNELS

} DRV_GENERICCODEC_CHANNEL;

// *****************************************************************************
/* Generic Codec Driver Initialization Data

  Summary:
    Defines the data required to initialize or reinitialize the Generic Codec driver

  Description:
    This data type defines the data required to initialize or reinitialize the
    Generic Codec driver.

  Remarks:
    None.
 */
typedef struct {
    /* System module initialization */
    SYS_MODULE_INIT moduleInit;

    /* Identifies data module(I2S) driver ID for
       data interface of Codec */
    SYS_MODULE_INDEX i2sDriverModuleIndex;

    /* Identifies data module(I2C) driver ID for
    control interface of Codec */
    SYS_MODULE_INDEX i2cDriverModuleIndex;

    /* Master or slave mode */
    bool masterMode;

    /* Sampling rate */
    uint32_t samplingRate;

    /* Volume */
    uint8_t volume;
    
    /* Identifies the Audio data format */
    DRV_GENERICCODEC_AUDIO_DATA_FORMAT audioDataFormat;

} DRV_GENERICCODEC_INIT;


// *****************************************************************************
// *****************************************************************************
// Section: Generic Codec Driver Module Interface Routines
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
    Initializes hardware and data for the instance of the Generic Codec DAC module

  Description:
    This routine initializes the Generic Codec driver instance for the specified driver
    index, making it ready for clients to open and use it. The initialization
    data is specified by the init parameter. The initialization may fail if the
    number of driver objects allocated are insufficient or if the specified
    driver instance is already initialized.

  Precondition:
    DRV_I2S_Initialize must be called before calling this function to initialize
    the data interface of this Codec driver.
    DRV_I2C_Initialize must be called if SPI driver is used for handling
    the control interface of this Codec driver.

  Parameters:
    drvIndex        - Identifier for the driver instance to be initialized
    init            - Pointer to the data structure containing any data
                      necessary to initialize the hardware. This pointer may
                      be null if no data is required and default
                      initialization is to be used.

  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, it returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    DRV_GENERICCODEC_INIT              	init;
    SYS_MODULE_OBJ              	objectHandle;

    init->inUse                           = true;
    init->status                          = SYS_STATUS_BUSY;
    init->numClients                      = 0;
    init->i2sDriverModuleIndex            = genercicodecInit->i2sDriverModuleIndex;
    init->i2cDriverModuleIndex            = genercicodecInit->i2cDriverModuleIndex;
    init->samplingRate                    = DRV_GENERICCODEC_AUDIO_SAMPLING_RATE;
    init->audioDataFormat                 = DRV_GENERICCODEC_AUDIO_DATA_FORMAT_MACRO;
    
    init->isInInterruptContext            = false;

    init->commandCompleteCallback = (DRV_GENERICCODEC_COMMAND_EVENT_HANDLER)0;
    init->commandContextData = 0;
    init->mclk_multiplier = DRV_GENERICCODEC_MCLK_SAMPLE_FREQ_MULTPLIER;


    objectHandle = DRV_GENERICCODEC_Initialize(DRV_GENERICCODEC_0, (SYS_MODULE_INIT*)init);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        // Handle error
    }
    </code>

  Remarks:
    This routine must be called before any other Generic Codec routine is called.

    This routine should only be called once during system initialization
    unless DRV_GENERICCODEC_Deinitialize is called to deinitialize the driver
    instance. This routine will NEVER block for hardware access.

 */
SYS_MODULE_OBJ DRV_GENERICCODEC_Initialize
(
        const SYS_MODULE_INDEX drvIndex,
        const SYS_MODULE_INIT * const init
);

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

  Example:
    <code>
    SYS_MODULE_OBJ      object;     //  Returned from DRV_GENERICCODEC_Initialize
    SYS_STATUS          status;


    DRV_GENERICCODEC_Deinitialize(object);

    status = DRV_GENERICCODEC_Status(object);
    if (SYS_MODULE_DEINITIALIZED != status)
    {
        // Check again later if you need to know
        // when the driver is deinitialized.
    }
    </code>

  Remarks:
    Once the Initialize operation has been called, the De-initialize operation
    must be called before the Initialize operation can be called again. This
    routine will NEVER block waiting for hardware.
 */
void DRV_GENERICCODEC_Deinitialize(SYS_MODULE_OBJ object);


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
                                deinitialized

    SYS_STATUS_READY          - Indicates that any previous module operation
                                for the specified module has completed

    SYS_STATUS_BUSY           - Indicates that a previous module operation for
                                the specified module has not yet completed

    SYS_STATUS_ERROR          - Indicates that the specified module is in an
                                error state

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_GENERICCODEC_Initialize
    SYS_STATUS          GENERICCODECStatus;

    GENERICCODECStatus = DRV_GENERICCODEC_Status(object);
    if (SYS_STATUS_READY == GENERICCODECStatus)
    {
        // This means the driver can be opened using the
        // DRV_GENERICCODEC_Open() function.
    }
    </code>

  Remarks:
    A driver can opened only when its status is SYS_STATUS_READY.
 */
SYS_STATUS DRV_GENERICCODEC_Status(SYS_MODULE_OBJ object);


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

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_GENERICCODEC_Initialize

    while (true)
    {
        DRV_GENERICCODEC_Tasks (object);

        // Do other tasks
    }
    </code>

  Remarks:
    This routine is normally not called directly by an application.  It is
    called by the system's Tasks routine (SYS_Tasks).

 */
void DRV_GENERICCODEC_Tasks(SYS_MODULE_OBJ object);


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
        const DRV_IO_INTENT ioIntent
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

    Generic Codec can be opened with DRV_IO_INTENT_WRITE, or DRV_IO_INTENT_READ or
    DRV_IO_INTENT_WRITEREAD io_intent option.  This decides whether the driver
    is used for headphone output, or microphone input or both modes simultaneously.

    Specifying a DRV_IO_INTENT_EXCLUSIVE will cause the driver to provide
    exclusive access to this client. The driver cannot be opened by any
    other client.

  Precondition:
    Function DRV_GENERICCODEC_Initialize must have been called before calling this
    function.

  Parameters:
    drvIndex    - Identifier for the object instance to be opened

    ioIntent    - Zero or more of the values from the enumeration
                  DRV_IO_INTENT "ORed" together to indicate the intended use
                  of the driver. See function description for details.

  Returns:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance).

    If an error occurs, the return value is DRV_HANDLE_INVALID. Error can occur
    - if the number of client objects allocated via DRV_GENERICCODEC_CLIENTS_NUMBER is
      insufficient.
    - if the client is trying to open the driver but driver has been opened 
      exclusively by another client.
    - if the driver hardware instance being opened is not initialized or is invalid.
    - if the ioIntent options passed are not relevant to this driver.

  Example:
    <code>
    DRV_HANDLE handle;

    handle = DRV_GENERICCODEC_Open(DRV_GENERICCODEC_INDEX_0, DRV_IO_INTENT_WRITEREAD | DRV_IO_INTENT_EXCLUSIVE);
    if (DRV_HANDLE_INVALID == handle)
    {
        // Unable to open the driver
        // May be the driver is not initialized or the initialization
        // is not complete.
    }
    </code>

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
);

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

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
                 - None

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_GENERICCODEC_Open

    DRV_GENERICCODEC_Close(handle);

    </code>

  Remarks:

    Usually there is no need for the driver client to verify that the Close
    operation has completed.  The driver will abort any ongoing operations
    when this routine is called.
 */
void DRV_GENERICCODEC_Close(const DRV_HANDLE handle);


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
    modified.  The function returns DRV_GENERICCODEC_BUFFER_HANDLE_INVALID:
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

    DRV_IO_INTENT_WRITE must have been specified in the DRV_GENERICCODEC_Open call.

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

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    // Client registers an event handler with driver

    DRV_GENERICCODEC_BufferEventHandlerSet(myGENERICCODECHandle,
                    DRV_GENERICCODECBufferEventHandler, (uintptr_t)&myAppObj);

    DRV_GENERICCODEC_BufferAddWrite(myGENERICCODEChandle, &bufferHandle
                                        myBuffer, MY_BUFFER_SIZE);

    if(DRV_GENERICCODEC_BUFFER_HANDLE_INVALID == bufferHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.

    void DRV_GENERICCODECBufferEventHandler(DRV_GENERICCODEC_BUFFER_EVENT event,
            DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE:

                // This means the data was transferred.
                break;

            case DRV_GENERICCODEC_BUFFER_EVENT_ERROR:

                // Error handling here.
                break;

            default:
                break;
        }
    }

    </code>

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
);

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
    was processed successfully of DRV_GENERICCODEC_BUFFER_EVENT_ERROR event if the
    buffer was not processed successfully.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec device instance and the DRV_GENERICCODEC_Status must have returned
    SYS_STATUS_READY.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

    DRV_IO_INTENT_READWRITE must have been specified in the DRV_GENERICCODEC_Open call.

  Parameters:
    handle         - Handle of the Generic Codec instance as returned by the
                     DRV_GENERICCODEC_Open function
    bufferHandle   - Pointer to an argument that will contain the
                     return buffer handle
    transmitBuffer - The buffer where the transmit data will be stored
    receiveBuffer  - The buffer where the received data will be stored
    size           - Buffer size in bytes

  Returns:
    The bufferHandle parameter will contain the return buffer handle. This will be
    DRV_GENERICCODEC_BUFFER_HANDLE_INVALID if the function was not successful.

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t mybufferTx[MY_BUFFER_SIZE];
    uint8_t mybufferRx[MY_BUFFER_SIZE];
    DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle;

    // myak4642Handle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    // Client registers an event handler with driver

    DRV_GENERICCODEC_BufferEventHandlerSet(myak4642Handle,
                    DRV_GENERICCODECBufferEventHandler, (uintptr_t)&myAppObj);

    DRV_GENERICCODEC_BufferAddWriteRead(myak4642handle, &bufferHandle,
                                        mybufferTx,mybufferRx,MY_BUFFER_SIZE);

    if(DRV_GENERICCODEC_BUFFER_HANDLE_INVALID == bufferHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.

    void DRV_GENERICCODECBufferEventHandler(DRV_GENERICCODEC_BUFFER_EVENT event,
            DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE:

                // This means the data was transferred.
                break;

            case DRV_GENERICCODEC_BUFFER_EVENT_ERROR:

                // Error handling here.
                break;

            default:
                break;
        }
    }

    </code>

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
    DRV_GENERICCODEC_BUFFER_HANDLE	*bufferHandle,
    void *transmitBuffer, void *receiveBuffer,
    size_t size
);

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
    Schedule a non-blocking driver read operation.

  Description:
    This function schedules a non-blocking read operation. The function returns
    with a valid buffer handle in the bufferHandle argument if the read request
    was scheduled successfully. The function adds the request to the hardware
    instance receive queue and returns immediately. While the request is in the
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

    DRV_IO_INTENT_READ must have been specified in the DRV_GENERICCODEC_Open call.

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
);

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

  Example:
    <code>
    // myCodecHandle is the handle returned by the DRV_GENERICCODEC_Open function.
    // Use DRV_GENERICCODEC_BufferAddWrite to queue write requests

    // Application timeout function, where remove queued buffers.
    void APP_TimeOut(void)
    {
        if(false == DRV_GENERICCODEC_WriteQueuePurge(myCodecHandle))
        {
            //Couldn't purge the write queue, try again.
        }
        else
        {
            //Queue purge successful.
        }
    }
    </code>

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
*/

bool DRV_GENERICCODEC_WriteQueuePurge(const DRV_HANDLE handle);

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

  Example:
    <code>
    // myCodecHandle is the handle returned by the DRV_GENERICCODEC_Open function.
    // Use DRV_GENERICCODEC_BufferAddRead to queue read requests

    // Application timeout function, where remove queued buffers.
    void APP_TimeOut(void)
    {
        if(false == DRV_GENERICCODEC_ReadQueuePurge(myCodecHandle))
        {
            //Couldn't purge the read queue, try again.
        }
        else
        {
            //Queue purge successful.
        }
    }
    </code>

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
*/

bool DRV_GENERICCODEC_ReadQueuePurge(const DRV_HANDLE handle);

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

  Example:
    <code>
    MY_APP_OBJ myAppObj;
    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    // Client registers an event handler with driver

    DRV_GENERICCODEC_BufferEventHandlerSet(myGENERICCODECHandle,
                    DRV_GENERICCODECBufferEventHandler, (uintptr_t)&myAppObj);

    DRV_GENERICCODEC_BufferAddWrite(myGENERICCODEChandle, &bufferHandle
                                        myBuffer, MY_BUFFER_SIZE);

    if(DRV_GENERICCODEC_BUFFER_HANDLE_INVALID == bufferHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.

    void DRV_GENERICCODECBufferEventHandler(DRV_GENERICCODEC_BUFFER_EVENT event,
            DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE:

                // This means the data was transferred.
                break;

            case DRV_GENERICCODEC_BUFFER_EVENT_ERROR:

                // Error handling here.
                break;

            default:
                break;
        }
    }
    </code>

  Remarks:
    If the client does not want to be notified when the queued buffer transfer
    has completed, it does not need to register a callback.
 */
void DRV_GENERICCODEC_BufferEventHandlerSet
(
    DRV_HANDLE handle,
    const DRV_GENERICCODEC_BUFFER_EVENT_HANDLER eventHandler,
    const uintptr_t contextHandle
);

// *****************************************************************************
// *****************************************************************************
// Section: Generic Codec Specific Client Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_SamplingRateSet(DRV_HANDLE handle, uint32_t samplingRate)

  Summary:
    This function sets the sampling rate of the media stream.

  Description:
    This function sets the media sampling rate for the client handle.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    samplingRate - Sampling frequency in Hz
  Returns:
    None.

  Example:
    <code>
    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    DRV_GENERICCODEC_SamplingRateSet(myGENERICCODECHandle, 48000);	//Sets 48000 media sampling rate

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_SamplingRateSet(DRV_HANDLE handle, uint32_t samplingRate);

// *****************************************************************************
/*
  Function:
    uint32_t DRV_GENERICCODEC_SamplingRateGet(DRV_HANDLE handle)

  Summary:
    This function gets the sampling rate set on the Generic Codec.

  Description:
    This function gets the sampling rate set on the DAC Generic Codec.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
  Example:
    <code>
   uint32_t baudRate;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    baudRate = DRV_GENERICCODEC_SamplingRateGet(myGENERICCODECHandle);

    </code>

  Remarks:
    None.
 */
uint32_t DRV_GENERICCODEC_SamplingRateGet(DRV_HANDLE handle);

// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_VolumeSet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL channel, uint8_t volume);

  Summary:
    This function sets the volume for Generic Codec.

  Description:
    This functions sets the volume value from 0-255.
    
  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    channel      - argument indicating Left or Right or Both channel volume to be modified
    volume       - volume value specified in the range 0-255 (0x00 to 0xFF)

  Returns:
    None

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

        DRV_GENERICCODEC_VolumeSet(myGENERICCODECHandle,DRV_GENERICCODEC_CHANNEL_LEFT, 120);	

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_VolumeSet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL channel, uint8_t volume);

// *****************************************************************************
/*
  Function:
    uint8_t DRV_GENERICCODEC_VolumeGet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL channel)

  Summary:
    This function gets the volume for Generic Codec.

  Description:
    This functions gets the current volume programmed to the Generic Codec.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
    channel      - argument indicating Left or Right or Both channel volume to be modified

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;
    uint8_t volume;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

      volume = DRV_GENERICCODEC_VolumeGet(myGENERICCODECHandle, DRV_GENERICCODEC_CHANNEL_LEFT);
    </code>

  Remarks:
    None.
 */
uint8_t DRV_GENERICCODEC_VolumeGet(DRV_HANDLE handle, DRV_GENERICCODEC_CHANNEL channel);


// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MuteOn(DRV_HANDLE handle);

  Summary:
    This function allows Generic Codec output for soft mute on.

  Description:
    This function Enables Generic Codec output for soft mute.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    DRV_GENERICCODEC_MuteOn(myGENERICCODECHandle);	//GENERICCODEC output soft muted

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_MuteOn(DRV_HANDLE handle);


// *****************************************************************************
/*
  Function:
        void DRV_GENERICCODEC_MuteOff(DRV_HANDLE handle)

  Summary:
    This function disables Generic Codec output for soft mute.

  Description:
    This function disables Generic Codec output for soft mute.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

        DRV_GENERICCODEC_MuteOff(myGENERICCODECHandle);	//Generic Codec output soft mute disabled

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_MuteOff(DRV_HANDLE handle);


// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MicGainSet(DRV_HANDLE handle, uint8_t gain)

  Summary:
    This function sets the microphone gain for the Generic Codec CODEC.

  Description:
    This functions sets the microphone gain value from 0-31

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

    gain         - Gain value, in range 0-31

  Returns:
    None.

    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

        DRV_GENERICCODEC_MicGainSet(myGENERICCODECHandle, 15);	//GENERICCODEC mic gain set to 15

    </code>

  Remarks:
    None.
*/
void DRV_GENERICCODEC_MicGainSet(DRV_HANDLE handle, uint8_t gain);


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
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    Microphone gain, in range 0-31.

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
uint8_t DRV_GENERICCODEC_MicGainGet(DRV_HANDLE handle);


// *****************************************************************************
/*
  Function:
    void DRV_GENERICCODEC_MicMuteOn(DRV_HANDLE handle);

  Summary:
    Mutes the Generic Codec's microphone input

  Description:
    This function mutes the Generic Codec's microphone input

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    DRV_GENERICCODEC_MicMuteOn(myGENERICCODECHandle);	//Generic Codec microphone muted

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_MicMuteOn(DRV_HANDLE handle);


// *****************************************************************************
/*
  Function:
        void DRV_GENERICCODEC_MicMuteOff(DRV_HANDLE handle)

  Summary:
    Umutes th Generic Codec's microphone input.

  Description:
    This function unmutes the Generic Codec's microphone input.

  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

        DRV_GENERICCODEC_MicMuteOff(myGENERICCODECHandle);	//Generic Codec microphone unmuted

    </code>

  Remarks:
    None.
 */
void DRV_GENERICCODEC_MicMuteOff(DRV_HANDLE handle);


// *****************************************************************************
/*
  Function:
    DRV_HANDLE DRV_GENERICCODEC_GetI2SDriver(DRV_HANDLE codecHandle)

  Summary:
    Get the handle to the I2S driver for this codec instance.

  Description:
    Returns the appropriate handle to the I2S based on the ioIent member
    of the codec object.
 
  Precondition:
    The DRV_GENERICCODEC_Initialize routine must have been called for the specified
    Generic Codec driver instance.

    DRV_GENERICCODEC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:       - A handle to the I2S driver for this codec instance
    
  Remarks:
    This allows the caller to directly access portions of the I2S driver that
    might not be available via the codec API.
*/
DRV_HANDLE DRV_GENERICCODEC_GetI2SDriver(DRV_HANDLE codecHandle);

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
  
  Precondition:
    None.
  
  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine
  
  Returns:
    true if the function was successful, false if a timeout occurred (no transitions seen)
    
  Example:
    <code>
        // myGENERICCODECHandle is the handle returned
        // by the DRV_GENERICCODEC_Open function.

        DRV_GENERICCODEC_LRCLK_Sync(myGENERICCODECHandle);
    </code>
    
  Remarks:
    None.                                            
*/
bool DRV_GENERICCODEC_LRCLK_Sync (const DRV_HANDLE handle);

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

    The event handler should be set before the client performs any "Generic Codec
    Specific Client Routines" operations that could generate events.
    The event handler once set, persists until the client closes the driver or
    sets another event handler (which could be a "NULL" pointer to indicate no
    callback).

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

  Example:
    <code>
    MY_APP_OBJ myAppObj;
    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_GENERICCODEC_BUFFER_HANDLE bufferHandle;

    // myGENERICCODECHandle is the handle returned
    // by the DRV_GENERICCODEC_Open function.

    // Client registers an event handler with driver

    DRV_GENERICCODEC_CommandEventHandlerSet(myGENERICCODECHandle,
                    DRV_GENERICCODECCommandEventHandler, (uintptr_t)&myAppObj);

    DRV_GENERICCODEC_DeEmphasisFilterSet(myGENERICCODECHandle, DRV_GENERICCODEC_DEEMPHASIS_FILTER_44_1KHZ)

    // Event is received when
    // the buffer is processed.

    void DRV_GENERICCODECCommandEventHandler(uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
                // Last Submitted command is completed.
                // Perform further processing here
        }
    }
    </code>

  Remarks:
    If the client does not want to be notified when the command
    has completed, it does not need to register a callback.
 */
void DRV_GENERICCODEC_CommandEventHandlerSet
(
    DRV_HANDLE handle,
    const DRV_GENERICCODEC_COMMAND_EVENT_HANDLER eventHandler,
    const uintptr_t contextHandle
);

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

        The String does not contain any spaces. For example, 
		"0.03a"
		"1.00"
    
  Precondition:
    None.

  Parameters:
    None.

  Returns: returns a string containing the version of Generic Codec driver.

  Example:
    <code>
        int8_t *GENERICCODECstring;
        GENERICCODECstring = DRV_GENERICCODEC_VersionStrGet();
    </code>

  Remarks:
    None
 */
int8_t* DRV_GENERICCODEC_VersionStrGet(void);


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
    None.

  Parameters:
    None.

  Returns: returns the version of Generic Codec driver.

  Example:
    <code>
        uint32_t GENERICCODECversion;
        GENERICCODECversion = DRV_GENERICCODEC_VersionGet();
    </code>

  Remarks:
    None.
 */
uint32_t DRV_GENERICCODEC_VersionGet(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif // #ifndef _DRV_GENERICCODEC_H
/*******************************************************************************
 End of File
 */

