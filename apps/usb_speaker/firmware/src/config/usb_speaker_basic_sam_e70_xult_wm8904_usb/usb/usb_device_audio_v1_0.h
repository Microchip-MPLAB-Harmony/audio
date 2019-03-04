/*******************************************************************************
  USB Device Audio Function Driver Interface

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_audio.h

  Summary:
    USB Device Audio function Driver Interface 

  Description:
    This file describes the USB Device Audio Function Driver interface. This
    file should be included by the application if it needs to use the Audio
    Function Driver API.
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

#ifndef _USB_DEVICE_AUDIO_H
#define _USB_DEVICE_AUDIO_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "usb/src/usb_external_dependencies.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "usb/src/usb_device_function_driver.h"
#include "usb/usb_audio_v1_0.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Device Audio Function Driver Index

  Summary:
    USB Device Audio function driver index.

  Description:
    This definition uniquely identifies a Audio Function Driver instance.

  Remarks:
    None.
*/

typedef uintptr_t USB_DEVICE_AUDIO_INDEX;

// *****************************************************************************
/* USB Device Audio Function Driver Transfer Handle Definition

  Summary:
    USB Device Audio Function Driver transfer handle definition.

  Description:
    This definition defines a USB Device Audio Function Driver Transfer Handle.
    A Transfer Handle is owned by the application but its value is modified by
    the USB_DEVICE_AUDIO_Write, USB_DEVICE_AUDIO_Read functions. The
    transfer handle is valid for the life time of the transfer and expires when
    the transfer related event had occurred.

  Remarks:
    None.
*/

typedef uintptr_t USB_DEVICE_AUDIO_TRANSFER_HANDLE;

// *****************************************************************************
/* USB Device Audio Function Driver Invalid Transfer Handle Definition
 
  Summary:
    USB Device Audio Function Driver invalid transfer handle definition.

  Description:
    This definition defines a Invalid USB Device Audio Function Driver Transfer
    Handle.  A Invalid Transfer Handle is returned by the
    USB_DEVICE_Audio_Write , USB_DEVICE_Audio_Read, functions when the
    request was not successful.

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID  /*DOM-IGNORE-BEGIN*/((USB_DEVICE_AUDIO_TRANSFER_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Device Audio Function Driver Events

  Summary:
    USB Device Audio Function Driver events.

  Description:
    These events are specific to a USB Device Audio Function Driver instance.
    An event may have some data associated with it. This is provided to the
    event handling function. Each event description contains details about this
    event data (pData) and other parameters passed along with the event, to the
    event handler. 
    
    Events associated with the Audio Function Driver Specific Control Transfers
    require application response. The application should respond to
    these events by using the USB_DEVICE_ControlReceive,
    USB_DEVICE_ControlSend and USB_DEVICE_ControlStatus functions.
    
    Calling the USB_DEVICE_ControlStatus function with a
    USB_DEVICE_CONTROL_STATUS_ERROR will stall the control transfer request.
    The application would do this if the control transfer request is not
    supported. Calling the USB_DEVICE_ControlStatus function with a
    USB_DEVICE_CONTROL_STATUS_OK will complete the status stage of the
    control transfer request. The application would do this if the control
    transfer request is supported. 
    
    The following code shows an example of a possible event handling scheme.
    
    <code>
    
    // This code example shows all USB Audio Function Driver possible events and
    // a possible scheme for handling these events. In this case event responses
    // are not deferred.
    
    void APP_USBDeviceAudioEventHandler
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex ,
        USB_DEVICE_AUDIO_EVENT event ,
        void * pData,
        uintptr_t context
    )
    {
        switch (event)
        {
            case USB_DEVICE_AUDIO_EVENT_READ_COMPLETE:

                // This event indicates that a Audio Read Transfer request
                // has completed. pData should be interpreted as a 
                // USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE pointer type.
                // This contains the transfer handle of the read transfer
                // that completed and amount of data that was read.

                break;
            
            case USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE:
 
                // This event indicates that a Audio Write Transfer request
                // has completed. pData should be interpreted as a 
                // USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE pointer type.
                // This contains the transfer handle of the write transfer
                // that completed and amount of data that was written.

                break;

            case USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE:
                // This event indicates that a Audio Status Write Transfer
                // request on the interrupt Endpoint has been completed. pData 
                // should be interpreted as a 
                // USB_DEVICE_AUDIO_EVENT_DATA_STATUS_SEND_COMPLETE pointer type.
                // This contains the transfer handle of the transfer.

                break;


            case USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED:

                // This event occurs when the host sends Set Interface request
                // to the Audio USB Device. pData will be a pointer to a
                // USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED. This
                // contains the interface number whose setting was
                // changed and the index of the alternate setting.
                // The application should typically enable the audio function
                // if the interfaceAlternateSettting member of pData is greater
                // than 0.

                break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_UNKNOWN:
             
                // This event indicates that the Audio function driver has
                // received a control transfer which it cannot decode. pData
                // will be a pointer to USB_SETUP_PACKET type pointer. The
                // application should decode the packet and take the required
                // action using the USB_DEVICE_ControlStatus(),
                // USB_DEVICE_ControlSend() and USB_DEVICE_ControlReceive()
                // functions.

                break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT:
                
                // This event indicates the data send request associated with
                // the latest USB_DEVICE_ControlSend() function was
                // completed. pData will be NULL. 

            case USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

                // This event indicates the data receive request associated with
                // the latest USB_DEVICE_ControlReceive() function was
                // completed. pData will be NULL. The application can either
                // acknowledge the received data or reject it by calling the 
                // USB_DEVICE_ControlStatus() function. 

                break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR:
                
                // This event indicates that the host is trying to set the
                // current setting attribute of a control. The data type will be
                // USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR type.  The
                // application should identify the entity type based on the
                // entity ID. This mapping is application specific. The
                // following example assumes entity type to be a Feature Unit. 

                if(APP_EntityIdentify(((USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR *)pData)->entityID) 
                        == APP_AUDIO_ENTITY_FEATURE_UNIT)
                {
                    // The entity type is a feature unit. Type cast pData as
                    // a USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST type and find
                    // identify the control selector. This example shows the 
                    // handling for VOLUME control 

                    switch(((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST *)pData)->controlSelector)
                    {
                        case USB_AUDIO_VOLUME_CONTROL:
                            // This means the host is trying to set the volume.
                            // Use the USB_DEVICE_ControlReceive() function to
                            // receive the volume settings for each channel.

                            USB_DEVICE_ControlReceive(usbDeviceHandle, volumeSetting,
                                    ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST *)pData)->wLength);
                        default:
                            // Only volume control is supported in this example.
                            // So everything else is stalled.
                            USB_DEVICE_ControlStatus(usbDeviceHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
                    }
                }
                break;

            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_CUR:
                
                // This event indicates that the host is trying to get the
                // current setting attribute of a control. The data type will be
                // USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR type.  The
                // application should identify the entity type based on the
                // entity ID. This mapping is application specific. The
                // following example assumes entity type to be a Feature Unit. 

                if(APP_EntityIdentify(((USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR *)pData)->entityID) 
                        == APP_AUDIO_ENTITY_FEATURE_UNIT)
                {
                    // The entity type is a feature unit. Type cast pData as
                    // a USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST type and find
                    // identify the control selector. This example shows the 
                    // handling for VOLUME control 

                    switch(((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST *)pData)->controlSelector)
                    {
                        case USB_AUDIO_VOLUME_CONTROL:
                            // This means the host is trying to get the volume.
                            // Use the USB_DEVICE_ControlReceive() function to
                            // receive the volume settings for each channel.

                            USB_DEVICE_ControlSend(usbDeviceHandle, volumeSetting,
                                    ((USB_AUDIO_FEATURE_UNIT_CONTROL_REQUEST *)pData)->wLength);
                        default:
                            // Only volume control is supported in this example.
                            // So everything else is stalled.
                            USB_DEVICE_ControlStatus(usbDeviceHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
                    }
                }
                break;
            
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MAX:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MIN:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_RES:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MEM:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MAX:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MIN:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_RES:
            case USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MEM:
                // In this example these request are not supported and so are
                // stalled.
                USB_DEVICE_ControlStatus(usbDeviceHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
                break;
            
            default:
                break;
        }
        
        return(USB_DEVICE_AUDIO_EVENT_RESPONSE_NONE);
    }
    
    </code>

  Remarks:
    The application can defer responses to events triggered by control
    transfers. In that, the application can respond to the control transfer
    event after exiting the event handler. This allows the application some time
    to obtain the response data rather than having to respond to the event
    immediately. Note that a USB host will typically wait for an event
    response for a finite time duration before timing out and canceling the
    event and associated transactions.  Even when deferring response, the
    application must respond promptly if such time-out have to be avoided.                                                                                         
*/

typedef enum
{
    /* This event occurs when a write operation scheduled by calling the
       USB_DEVICE_AUDIO_Write() function has completed. The pData member in
       the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE type. */

    USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE ,

    /* This event occurs when a read operation scheduled by calling  the
       USB_DEVICE_AUDIO_Read() function has completed. The pData member in
       the event handler will point to USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE
       type.  */

    USB_DEVICE_AUDIO_EVENT_READ_COMPLETE ,

    /* This event occurs when a status write opearttion is complete which was 
       scheduled using the USB_DEVICE_AUDIO_StatusSend() function. The pData 
       parameter in the event handler will point to the 
       USB_DEVICE_AUDIO_EVENT_DATA_STATUS_SEND_COMPLETE type.*/ 

    USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE , 

    /* This event occurs when the Host requests the Audio USB
       device to set an alternate setting on an interface present in this audio
       function. An Audio USB Device will typically feature a default interface
       setting and one or more alternate interface settings. The pData member in
       the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED type. This contains
       the index of the interface whose setting must be changed and the index of
       the alternate setting. The application may enable or disable audio
       functions based on the interface setting. */

    USB_DEVICE_AUDIO_EVENT_INTERFACE_SETTING_CHANGED,

    /* This event occurs when the data stage of a control write transfer has
       completed. This would occur after the application would respond with a
       USB_DEVICE_ControlReceive function, which may possibly have been called
       in response to a USB_DEVICE_AUDIO_EVENT_ENTITY_SETTINGS_RECEIVED event
       This event notifies the application that the data is received from Host
       and is available at the location passed by the
       USB_DEVICE_ControlReceive function. If the received data is acceptable
       to the application, it should acknowledge the data by calling the
       USB_DEVICE_ControlStatus function with a USB_DEVICE_CONTROL_STATUS_OK
       flag.The application can reject the received data by calling the
       USB_DEVICE_ControlStatus function with the
       USB_DEVICE_CONTROL_STATUS_ERROR flag. The pData parameter will be
       NULL. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_RECEIVED,

    /* This event occurs when the data stage of a control read transfer has
       completed. This would occur when the application has called the
       USB_DEVICE_ControlSend function to complete the data stage of a control
       transfer. The event indicates that the data has been transmitted to the
       host. The pData parameter will be NULL.
       */

    USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_DATA_SENT,

    /* This event occurs when the Audio function driver receives a control
       transfer request that could not be decoded by Audio Function driver.The
       pData parameter will point to a USB_SETUP_PACKET type containing the
       SETUP packet.  The application must analyze this SETUP packet and use the
       USB_DEVICE_ControlSend or USB_DEVICE_ControlReceive or the
       USB_DEVICE_ControlStatus functions to advance the control transfer or
       complete it.*/

    USB_DEVICE_AUDIO_EVENT_CONTROL_TRANSFER_UNKNOWN,

    /* This event occurs when the Host sends an Audio Control specific Set
       Current Setting Attribute Control Transfer request to an Audio Device
       Control. The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlReceive functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_SET_CUR,

    /* This event occurs when the Host sends an Audio Control specific Set
       Minimum Setting Attribute Control Transfer request to an Audio Device
       Control.  The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MIN type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlReceive functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MIN,

    /* This event occurs when the Host sends an Audio Control specific Set
       Maximum Setting Attribute Control Transfer request to an Audio Device
       Control. The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MAX type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlReceive functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_SET_MAX,
 
    /* This event occurs when the Host sends an Audio Control specific Set
       Resolution Attribute Control Transfer request to an Audio Device Control.
       The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_RES type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus
       USB_DEVICE_ControlSend and/or USB_DEVICE_ControlReceive functions. */
    
    USB_DEVICE_AUDIO_EVENT_CONTROL_SET_RES,

    /* This event occurs when the Host sends an Audio Entity specific Set
       Memory Space Attribute Control Transfer request to an Audio Device
       Entity.  The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MEM type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus
       USB_DEVICE_ControlSend and/or USB_DEVICE_ControlReceive functions. */

    USB_DEVICE_AUDIO_EVENT_ENTITY_SET_MEM,

    /* This event occurs when the Host sends an Audio Control specific Get
       Current Setting Attribute Control Transfer request to an Audio Device
       Control. The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_CUR type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlSend functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_GET_CUR,

    /* This event occurs when the Host sends an Audio Control specific Get
       Minimum Setting Attribute Control Transfer request to an Audio Device
       Control.  The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_MIN type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlSend functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MIN,

    /* This event occurs when the Host sends an Audio Control specific Get
       Maximum Setting Attribute Control Transfer request to an Audio Device
       Control.  The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_MAX type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlSend functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_GET_MAX,

    /* This event occurs when the Host sends an Audio Control specific Get
       Resolution Setting Attribute Control Transfer request to an Audio Device
       Control. The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_RES type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus and
       USB_DEVICE_ControlSend functions. */

    USB_DEVICE_AUDIO_EVENT_CONTROL_GET_RES,

    /* This event occurs when the Host sends an Audio Entity specific Get
       Memory Space Attribute Control Transfer request to an Audio Device
       Entity.  The pData member in the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MEM type. The application must
       use the entityID, interface, endpoint and the wValue field in the event
       data to determine the entity and control type and then respond to the
       control transfer with a USB_DEVICE_ControlStatus or
       USB_DEVICE_ControlSend functions. */
    USB_DEVICE_AUDIO_EVENT_ENTITY_GET_MEM,
            
    /* This event occurs when the Host sends a Audio Entity specific Get Status
       Control Transfer request to an Audio Device Entity. The pData member in
       the event handler will point to
       USB_DEVICE_AUDIO_EVENT_DATA_ENTITY_GET_STAT type. The application mus use
       the entityID, interface, endpoint and the wValue field in the event data
       to determine the entity and control type and then respond to the control
       transfer with a USB_DEVICE_ControlSend and or USB_DEVICE_ControlStatus
       functions. */
    USB_DEVICE_AUDIO_EVENT_ENTITY_GET_STAT,

} USB_DEVICE_AUDIO_EVENT;

// *****************************************************************************
/* USB Device Audio Function Driver Event Handler Response Type

  Summary:
    USB Device Audio Function Driver event callback response type.

  Description:
    This is the return type of the Audio Function Driver event handler.

  Remarks:
    None.
*/

typedef void USB_DEVICE_AUDIO_EVENT_RESPONSE;

// *****************************************************************************
/* USB Device Audio Function Driver Event Handler Response None  

  Summary:
    USB Device Audio Function Driver event handler response type none.

  Description:
    This is the definition of the Audio Function Driver event handler response
    type none.

  Remarks:
    Intentionally defined to be empty.
*/

#define USB_DEVICE_AUDIO_EVENT_RESPONSE_NONE

// *****************************************************************************
/* USB Device Audio Event Handler Function Pointer Type.

  Summary:
    USB Device Audio event handler function pointer type.

  Description:
    This data type defines the required function signature USB Device Audio
    Function Driver event handling callback function. The application must
    register a pointer to an Audio Function Driver events handling function whose
    function signature (parameter and return value types) match the types
    specified by this function pointer in order to receive event call backs from
    the Audio Function Driver. The function driver will invoke this function with
    event relevant parameters. The description of the event handler function
    parameters is given here.

    instanceIndex           - Instance index of the Audio Function Driver that
                              generated the event.

    event                   - Type of event generated.

    pData                   - This parameter should be typecast to an event 
                              specific pointer type based on the event that 
                              has occurred. Refer to the 
                              USB_DEVICE_AUDIO_EVENT enumeration description for
                              more details.

    context                 - Value identifying the context of the application 
                              that registered the event handling function.

  Remarks:
    The event handler function executes in the USB interrupt context when the
    USB Device Stack is configured for interrupt based operation. It is not
    advisable to call blocking functions or computationally intensive functions
    in the event handler. Where the response to a control transfer related event
    requires extended processing, the response to the control transfer should be
    deferred and the event handler should be allowed to complete execution.
*/

typedef USB_DEVICE_AUDIO_EVENT_RESPONSE (*USB_DEVICE_AUDIO_EVENT_HANDLER )
(
    USB_DEVICE_AUDIO_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_EVENT event ,
    void * pData,
    uintptr_t context
);

// *****************************************************************************
/* USB Device Audio Function Driver USB Device Audio Result enumeration.

  Summary:
    USB Device Audio Function Driver USB Device Audio result enumeration.

  Description:
    This enumeration lists the possible USB Device Audio Function Driver
    operation results. 

  Remarks:
    None.
 */

typedef enum
{
    /* The operation was successful */
    USB_DEVICE_AUDIO_RESULT_OK 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_NONE /* DOM-IGNORE-END */,

    /* The transfer queue is full and no new transfers can be scheduled */
    USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_IRP_QUEUE_FULL /* DOM-IGNORE-END */,

    /* The specified instance is not provisioned in the system */
    USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_DEVICE_FUNCTION_INSTANCE_INVALID /* DOM-IGNORE-END */,

    /* The specified instance is not configured yet */
    USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_ENDPOINT_NOT_CONFIGURED /* DOM-IGNORE-END */,

    /* The event handler provided is NULL */
    USB_DEVICE_AUDIO_RESULT_ERROR_PARAMETER_INVALID  
        /* DOM-IGNORE-BEGIN */ =  USB_ERROR_PARAMETER_INVALID /* DOM-IGNORE-END */,

    /* Interface number passed to the read or write function is invalid. */
    USB_DEVICE_AUDIO_RESULT_ERROR_INVALID_INTERFACE_ID,

    /* A NULL buffer was specified in the read or write function */
    USB_DEVICE_AUDIO_RESULT_ERROR_INVALID_BUFFER,
    
    /* Transfer terminated because host halted the endpoint */
    USB_DEVICE_AUDIO_RESULT_ERROR_ENDPOINT_HALTED
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_ENDPOINT_HALTED /* DOM-IGNORE-END */,

    /* Transfer terminated by host because of a stall clear */
    USB_DEVICE_AUDIO_RESULT_ERROR_TERMINATED_BY_HOST
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_TRANSFER_TERMINATED_BY_HOST /* DOM-IGNORE-END */,

    /* General Error */
    USB_DEVICE_AUDIO_RESULT_ERROR

} USB_DEVICE_AUDIO_RESULT;

// *****************************************************************************
/* USB Device Audio Function Driver Read and Write Complete Event Data.

  Summary:
    USB Device Audio Function Driver audio read and write complete event data.

  Description:
    This data type defines the data structure returned by the driver along with
    USB_DEVICE_AUDIO_EVENT_READ_COMPLETE, USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE,
    events.

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle associated with this
     * read or write request */
    USB_DEVICE_AUDIO_TRANSFER_HANDLE handle;

    /* Indicates the amount of data (in bytes) that was
     * read or written */
    uint16_t length;

    /* Interface Number */
    uint8_t interfaceNum;
    
    /* Completion status of the transfer */
    USB_DEVICE_AUDIO_RESULT status;
}
USB_DEVICE_AUDIO_EVENT_DATA_WRITE_COMPLETE,
USB_DEVICE_AUDIO_EVENT_DATA_READ_COMPLETE;

// *****************************************************************************
/* USB Device Audio Function Driver Status Send Complete Event Data.

  Summary:
    USB Device Audio Function Driver Status Send complete event data.

  Description:
    This data type defines the data structure returned by the driver along with
    USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE event.

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle associated with Status Send request */
    USB_DEVICE_AUDIO_TRANSFER_HANDLE handle;
    
    /* Completion status of the transfer */
    USB_DEVICE_AUDIO_RESULT status;
}
USB_DEVICE_AUDIO_EVENT_DATA_STATUS_SEND_COMPLETE; 

// *****************************************************************************
/* USB Device Audio Function Driver Set and Get request data.

  Summary:
    USB Device Audio Function Driver set and get request data.

  Description:
    This data type defines the data structure returned by the driver along with the
    USB_DEVICE_AUDIO_EVENT_CONTROL_SET_XXX,
    USB_DEVICE_AUDIO_EVENT_ENTITY_SET_MEM,
    USB_DEVICE_AUDIO_EVENT_CONTROL_GET_XXX and
    USB_DEVICE_AUDIO_EVENT_ENTITY_GET_MEM events.

  Remarks:
    None.
*/

typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_CUR;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MIN;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MAX;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_RES;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_SET_MEM;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_CUR;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_MIN;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_MAX;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_RES;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_CONTROL_GET_MEM;
typedef USB_AUDIO_CONTROL_INTERFACE_REQUEST USB_DEVICE_AUDIO_EVENT_DATA_ENTITY_GET_STAT;

// *****************************************************************************
/* USB Device Audio Function Driver Alternate Interface Setting Event Data.

  Summary:
    USB Device Audio Function Driver alternate interface setting event data.

  Description:
    This data type defines the data structure returned by the driver along with
    USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED.

  Remarks:
    None.
*/

typedef struct
{
    /* Interface number of the interface who setting is to be changed */
    uint8_t interfaceNumber;

    /* Alternate setting number */
    uint8_t interfaceAlternateSetting;

} USB_DEVICE_AUDIO_EVENT_DATA_INTERFACE_SETTING_CHANGED;

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Audio Interface Function Definitions
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_EventHandlerSet
    (
        USB_DEVICE_AUDIO_INDEX instance ,
        USB_DEVICE_AUDIO_EVENT_HANDLER eventHandler ,
        uintptr_t context
    );
    
  Summary:
    This function registers an event handler for the specified Audio function
    driver instance. 

  Description:
    This function registers a event handler for the specified Audio function
    driver instance. This function should be called by the application when it
    receives a SET CONFIGURATION event from the device layer. The application
    must register an event handler with the function driver in order to receive
    and respond to function driver specific events and control transfers. If the
    event handler is not registered, the device layer will stall function driver
    specific commands and the USB device may not function.
   
  Precondition:
    This function should be called when the function driver has been initialized
    as a result of a set configuration.
    
  Parameters:
    instance  - Instance of the Audio Function Driver.
    eventHandler - A pointer to event handler function.
    context - Application specific context that is returned in the event handler.
  
  Returns:
    - USB_DEVICE_AUDIO_RESULT_OK - The operation was successful
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID - The specified instance does 
      not exist.
    - USB_DEVICE_AUDIO_RESULT_ERROR_PARAMETER_INVALID - The eventHandler parameter is 
      NULL
    
  Example:
    <code>
    // The following code shows an example for registering an event handler. The
    // application specifies the context parameter as a pointer to an
    // application object (appObject) that should be associated with this 
    // instance of the Audio function driver.
    
    USB_DEVICE_AUDIO_RESULT result;
    
    USB_DEVICE_AUDIO_EVENT_RESPONSE APP_USBDeviceAUDIOEventHandler 
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex ,
        USB_DEVICE_AUDIO_EVENT event ,
        void* pData,
        uintptr_t context
    )
    {
        // Event Handling comes here

        switch(event) 
        {
            ...
        }

        return(USB_DEVICE_AUDIO_EVENT_RESPONSE_NONE);
    }

    result = USB_DEVICE_AUDIO_EventHandlerSet ( USB_DEVICE_AUDIO_INSTANCE_0 ,
                &APP_USBDeviceAUDIOEventHandler, (uintptr_t) &appObject);

    if(USB_DEVICE_AUDIO_RESULT_OK != result)
    {
        // Do error handling here
    }
  
    </code>

  Remarks:
    None.
*/

USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_EventHandlerSet
(
    USB_DEVICE_AUDIO_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_EVENT_HANDLER eventHandler ,
    uintptr_t context
);

//**************************************************************************
/*
  Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Write
    (
        USB_DEVICE_AUDIO_INDEX instance ,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
        uint8_t interfaceNum ,
        void * data ,
        size_t size
    );
 
  Summary:
    This function requests a data write to the USB Device Audio Function
    Driver Layer.
  
  Description:
    This function requests a data write to the USB Device Audio Function
    Driver Layer. The function places a requests with driver, the request
    will get serviced as data is requested by the USB Host. A handle to the
    request is returned in the transferHandle parameter. The termination of
    the request is indicated by the USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE
    event. The amount of data written and the transfer handle associated
    with the request is returned along with the event in writeCompleteData 
    member of the pData parameter in the event handler. 
    
    The transfer handle expires when event handler for the
    USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE exits. If the write request could not
    be accepted, the function returns an error code and transferHandle will
    contain the value USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID.
 
  Precondition:
    The function driver should have been configured.
  
  Parameters:
    instance -        USB Device Audio Function Driver instance.
    transferHandle -  Pointer to a USB_DEVICE_AUDIO_TRANSFER_HANDLE type of
                      variable. This variable will contain the transfer
                      handle in case the write request was successful.
    interfaceNum -    The USB Audio streaming interface number on which the
                      write request is to placed.
    data  -           pointer to the data buffer contains the data to be written. 
                      In case of PIC32MZ device, this buffer should be located in 
                      coherent memory and should be aligned a 16 byte boundary.
    size -            Size of the data buffer.

  Return:
    - USB_DEVICE_AUDIO_RESULT_OK - The read request was successful. transferHandle
    contains a valid transfer handle.
    - USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL - internal request queue
    is full. The write request could not be added.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED - The specified
    instance is not configured yet.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID - The specified instance
    was not provisioned in the application and is invalid.
  
  Example:
    <code>

    // Shows an example of how to write audio data to the audio streaming
    // interface . This assumes that device is configured and the audio
    // streaming interface is 1.

    USB_DEVICE_AUDIO_INDEX instanceIndex;
    USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle;
    unit8_t interfaceNumber;
    unit8_t txBuffer[192]; // Use this attribute for PIC32MZ __attribute__((coherent, aligned(16)))
    USB_DEVICE_AUDIO_RESULT writeRequestResult;
    
    instanceIndex = 0; //specify the Audio Function driver instance number.
    interfaceNumber = 1; //Specify the Audio Streaming interface number.
    
    writeRequestResult = USB_DEVICE_AUDIO_Write ( instanceIndex, &transferHandle,
                                interfaceNumber, &txBuffer, 192);

    if(USB_DEVICE_AUDIO_RESULT_OK != writeRequestResult)
    {
        //Do Error handling here
    }

    // The completion of the write request will be indicated by the
    // USB_DEVICE_AUDIO_EVENT_WRITE_COMPLETE event. The transfer handle
    // and transfer size is provided along with this event.

    </code>

  Remarks:
    While the using the Audio Function Driver with the PIC32MZ USB module, the
    audio buffer provided to the USB_DEVICE_AUDIO_Write function should be
    placed in coherent memory and aligned at a 16 byte boundary.  This can be
    done by declaring the buffer using the  __attribute__((coherent,
    aligned(16))) attribute. An example is shown here                                                                   

    <code>
    uint8_t data[256] __attribute__((coherent, aligned(16)));
    </code>                                                                  
*/

USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Write
(
    USB_DEVICE_AUDIO_INDEX instanceIndex,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE * transferHandle,
    uint8_t interfaceNumber,
    void * data,
    size_t size
);

//***************************************************************************
/*
  Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Read
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex ,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
        uint8_t interfaceNum ,
        void * data ,
        size_t size
    );
  
  Summary:
    This function requests a data read from the USB Device Audio Function
    Driver Layer.
  
  Description:
    This function requests a data read from the USB Device Audio Function
    Driver Layer. The function places a requests with driver, the request
    will get serviced as data is made available by the USB Host. A handle
    to the request is returned in the transferHandle parameter. The
    termination of the request is indicated by the
    USB_DEVICE_AUDIO_EVENT_READ_COMPLETE event. The amount of data read and
    the transfer handle associated with the request is returned along with
    the event. The transfer handle expires when event handler for the
    USB_DEVICE_AUDIO_EVENT_READ_COMPLETE exits. If the read request could
    not be accepted, the function returns an error code and transferHandle
    will contain the value USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID.

  Conditions:
    The function driver should have been configured.
  
  Parameters:
    instance -        USB Device Audio Function Driver instance.
    transferHandle -  Pointer to a USB_DEVICE_AUDIO_TRANSFER_HANDLE type of
                      variable. This variable will contain the transfer
                      handle in case the read request was successful.
    interfaceNum -    The USB Audio streaming interface number on which read
                      request is to placed.
    data  -           pointer to the data buffer where read data will be stored. 
                      In case of PIC32MZ device, this buffer should be located in 
                      coherent memory and should be aligned a 16 byte boundary.
    size -            Size of the data buffer. Refer to the description section
                      for more details on how the size affects the transfer.

  Return:
    - USB_DEVICE_AUDIO_RESULT_OK - The read request was successful. transferHandle
    contains a valid transfer handle.
    - USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL - internal request queue
    is full. The read request could not be added.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED - The specified
    instance is not configured yet.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID - The specified instance
    was not provisioned in the application and is invalid.

  Example:
    <code>
    
    // Shows an example of how to read. This assumes that
    // device had been configured. The example attempts to read
    // data from interface 1.
   
    USB_DEVICE_AUDIO_INDEX instanceIndex;
    USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle;
    unit8_t interfaceNumber;
    unit8_t rxBuffer[192]; // Use this attribute for PIC32MZ __attribute__((coherent, aligned(16)))
    USB_DEVICE_AUDIO_RESULT readRequestResult; 

    instanceIndex = 0; //specify the Audio Function driver instance number.
    interfaceNumber = 1; //Specify the Audio Streaming interface number.
    
    readRequestResult = USB_DEVICE_AUDIO_Read ( instanceIndex, &transferHandle,
                            interfaceNumber, &rxBuffer, 192);

    if(USB_DEVICE_AUDIO_RESULT_OK != readRequestResult)
    {
        //Do Error handling here
    }

    // The completion of the read request will be indicated by the
    // USB_DEVICE_AUDIO_EVENT_READ_COMPLETE event. The transfer handle
    // and the amount of data read will be returned along with the 
    // event.

    </code>

  Remarks:
    While the using the Audio Function Driver with PIC32MZ USB module, the audio
    buffer provided to the USB_DEVICE_AUDIO_Read function should be placed in
    coherent memory and aligned at a 16 byte boundary.  This can be done by
    declaring the buffer using the  __attribute__((coherent, aligned(16)))
    attribute. An example is shown here                                                                   

    <code>
    uint8_t data[256] __attribute__((coherent, aligned(16)));
    </code>
*/

USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Read
(
    USB_DEVICE_AUDIO_INDEX instanceIndex,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
    uint8_t interfaceNumber,
    void * data,
    size_t size
);

//******************************************************************************
/* Function:
   USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_StatusSend 
   (
        USB_DEVICE_AUDIO_INDEX instanceIndex,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
        USB_AUDIO_INTERRUPT_STATUS_WORD* status
    );

  Summary:
   This function requests a Status write to the USB Device Audio Function Driver 
   Layer.

  Description:
    This function requests a status write to the USB Device Audio Function
    Driver Layer. The function places a requests with driver to arm the status 
    interrupt Endpoint with the status provided, the request will get serviced 
    as data is requested by the USB Host. A handle to the request is returned in 
    the transferHandle parameter. The termination of the request is indicated by
    the USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE event.
    
    The transfer handle expires when event handler for the
    USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE exits. If the Status Send 
    request could not be accepted, the function returns an error code and 
    transferHandle will contain the value 
    USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID.

  Precondition:
    The USB Device should be in a configured state. The USB Configuration 
    descriptor must contain Status interrupt Endpoint descriptor.  

  Parameters:
    instance -        USB Device Audio Function Driver instance.
    transferHandle -  Pointer to a USB_DEVICE_AUDIO_TRANSFER_HANDLE type of
                      variable. This variable will contain the transfer
                      handle in case the write request was successful.
    status  -         pointer to the data buffer contains the Status. 
                      In case of PIC32MZ device, this buffer should be located in 
                      coherent memory and should be aligned a 16 byte boundary.

  Returns:

    - USB_DEVICE_AUDIO_RESULT_OK    - The Status send request was successful. 
           transferHandle contains a valid transfer handle.
    - USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL - internal request queue
           is full. The status send request could not be added.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED - The specified
           instance is not configured yet.
    - USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID - The specified instance
           was not provisioned in the application and is invalid.

  Example:
   <code>
    
    // Shows an example of how to Submit Status Send request to Host. This assumes 
    // that device has been configured. 
   
    USB_DEVICE_AUDIO_INDEX instanceIndex;
    USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle;
    // Following must have __attribute__((coherent, aligned(16))) for PIC32MZ 
    USB_AUDIO_INTERRUPT_STATUS_WORD statusWord; 
    USB_DEVICE_AUDIO_RESULT statusSendResult; 

    //specify the Audio Function driver instance number.
    instanceIndex = 0; 

    // Fill in Status Word 
    statusWord.bOriginator = 0x01; //ID of the terminal  
    statusWord.originator =  0x00; //Audio Control interface 
    statusWord.memoryContentsChanged = 1; //Memory contents changed 
    statusWord.interruptPending = 1; //Interrupt is pending 

    statusSendResult = USB_DEVICE_AUDIO_StatusSend ( instanceIndex, &transferHandle,
                            &statusWord);

    if(USB_DEVICE_AUDIO_RESULT_OK != statusSendResult)
    {
        //Do Error handling here
    }

    // The completion of the read request will be indicated by the
    // USB_DEVICE_AUDIO_EVENT_STATUS_SEND event. The transfer handle
    // will be returned along with the event.

    </code>

  Remarks:
    While the using the Audio Function Driver with PIC32MZ USB module, the audio
    buffer provided to the USB_DEVICE_AUDIO_StatusSend function should be placed in
    coherent memory and aligned at a 16 byte boundary.  This can be done by
    declaring the buffer using the  __attribute__((coherent, aligned(16)))
    attribute. An example is shown here                                                                   

    <code>
    USB_AUDIO_INTERRUPT_STATUS_WORD statusWord __attribute__((coherent, aligned(16)));
    </code>
*/
USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_StatusSend 
(
    USB_DEVICE_AUDIO_INDEX instanceIndex,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
    USB_AUDIO_INTERRUPT_STATUS_WORD* status
);
//******************************************************************************
/* Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_TransferCancel
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle
    );

  Summary:
    This function cancels a scheduled Audio Device data transfer.

  Description:
    This function cancels a scheduled Audio Device data transfer. The transfer
    could have been scheduled  using the USB_DEVICE_AUDIO_Read,
    USB_DEVICE_AUDIO_Write, or the USB_DEVICE_AUDIO_SerialStateNotificationSend 
    functions. If a transfer is still in the queue and its processing has not 
    started, the transfer is canceled completely. A transfer that is in progress 
    may or may not get canceled depending on the transaction that is presently in 
    progress. If the last transaction of the transfer is in progress, the transfer 
    will not be canceled.  If it is not the last transaction in progress, the in-progress 
    will be allowed to complete. Pending transactions will be canceled. The first 
    transaction of an in progress transfer cannot be canceled.

  Precondition:
    The USB Device should be in a configured state.

  Parameters:
    instanceIndex  - AUDIO Function Driver instance index.
    transferHandle - Transfer handle of the transfer to be canceled.

  Returns:
    - USB_DEVICE_AUDIO_RESULT_OK    - The transfer will be canceled completely or
                                      partially.
    - USB_DEVICE_AUDIO_RESULT_ERROR - The transfer could not be canceled because it has
                                      either completed, the transfer handle is invalid
                                      or the last transaction is in progress.

  Example:
    <code>

    // The following code snippet cancels a AUDIO transfer.

    USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle;
    USB_DEVICE_AUDIO_RESULT result;

    result = USB_DEVICE_AUDIO_TransferCancel(instanceIndex, transferHandle);

    if(USB_DEVICE_AUDIO_RESULT_OK == result)
    {
        // The transfer cancellation was either completely or
        // partially successful.
    }

    </code>

  Remarks:
    None.
*/

USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_TransferCancel
(
    USB_DEVICE_AUDIO_INDEX instanceIndex,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE transferHandle
);


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types. This section is specific to PIC32 implementation
//          of the USB Device Audio Function Driver
// *****************************************************************************
// *****************************************************************************

/*DOM-IGNORE-BEGIN*/extern const USB_DEVICE_FUNCTION_DRIVER audioFunctionDriver;/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Device Audio Function Driver Function Pointer

  Summary:
    USB Device Audio Function Driver function pointer.

  Description:
    This is the USB Device Audio Function Driver function pointer. This should
    registered with the device layer in the function driver registration table.

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_FUNCTION_DRIVER /*DOM-IGNORE-BEGIN*/&audioFunctionDriver/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Device Audio Function Driver Initialization Data Structure

  Summary:
    USB Device Audio Function Driver initialization data structure.

  Description:
    This data structure must be defined for every instance of the Audio Function 
    Driver. It is passed to the Audio function driver, by the Device Layer,
    at the time of initialization. The funcDriverInit member of the 
    Device Layer Function Driver registration table entry must point to this
    data structure for an instance of the Audio function driver. 

  Remarks:
    The queue sizes that are specified in this data structure are also affected
    by the USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED configuration macro.
*/

typedef struct 
{
    /* Size of the read queue for this instance
     * of the Audio function driver */
    size_t queueSizeRead;
    
    /* Size of the write queue for this instance
     * of the Audio function driver */
    size_t queueSizeWrite;

    /* Queue Size for the Status send request for 
       this instance of the Audio function driver */ 
    size_t queueSizeStatusSend;    

} USB_DEVICE_AUDIO_INIT;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _USB_DEVICE_AUDIO_H
