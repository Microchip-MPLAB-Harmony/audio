/*******************************************************************************
  System Initialization File

  File Name:
    usb_device_init_data.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
	const USB_DEVICE_AUDIO_INIT audioInit0 =
{
	.queueSizeRead = 8,
	.queueSizeWrite = 8
};


/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{
    
	/* Audio Function 0 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .interfaceNumber = 0,       /* First interfaceNumber of this function */ 
        .speed = USB_SPEED_HIGH|USB_SPEED_FULL,    /* Function Speed */ 
        .numberOfInterfaces = 3,    /* Number of interfaces */
        .funcDriverIndex = 0,  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,    /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit0    /* Function driver init data */
    },


};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/

const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                                  // DEVICE descriptor type
    0x0200,                                                 // USB Spec Release Number in BCD format
    0x00,         // Class Code
    0x00,         // Subclass code
    0x00,         // Protocol code


    USB_DEVICE_EP0_BUFFER_SIZE,                             // Max packet size for EP0, see configuration.h
    0x04D8,                                                 // Vendor ID
    0x0065,                                                 // Product ID				
    0x0100,                                                 // Device release number in BCD format
    0x01,                                                   // Manufacturer string index
    0x02,                                                   // Product string index
	0x00,                                                   // Device serial number string index
    0x01                                                    // Number of possible configurations
};


/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
	/* Configuration Descriptor */

    0x09,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                           // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(234),                      //(234 Bytes)Size of the Configuration descriptor
    3,                                                      // Number of interfaces in this configuration
    0x01,                                                   // Index value of this configuration
    0x00,                                                   // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,
	

/* Descriptor for Function 1 - Audio     */ 
    
    /* USB Headset Standard Audio Control Interface Descriptor          */
    0x09,                            // Size of this descriptor in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    0,
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)				 
    USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
    USB_AUDIO_PR_PROTOCOL_UNDEFINED, // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Headset Class-specific AC Interface Descriptor  */
    0x0A,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype      (bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */
									 
    0x64,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */

    2,                              /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    1,                              /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/
    2,                              /* AudioStreaming interface 2 belongs to this
                                     * AudioControl interface. (baInterfaceNr(2))*/

    /* USB Headset Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,                // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)
    
    /* USB Headset Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,                // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x02,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // Two Channels (bNrChannels)
    0x04,0x00,                      // (wChannelConfig) 0x0004
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Feature Unit ID8 Descriptor */
    0x0D,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_ID,                // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_MIXER_UNIT_ID,                  // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,0x00,                          // (bmaControls(2)) Controls for Channel 2
    0x00,                               //  iFeature
    
    /* USB Headset Feature Unit ID5 Descriptor */
    0x0B,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_MICROPHONE_ID,     // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,   // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,                               //  iFeature
    
    /* USB Headset Feature Unit ID7 Descriptor */
    0x0B,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_SIDE_TONING_ID,    // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,   // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,                               //  iFeature
    
    /* USB Headset Mixer Unit ID8 Descriptor */
    0x0D,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_MIXER_UNIT,               // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_MIXER_UNIT_ID,                  // ID of this Unit ( bUnitID  ).
    2,                                  // Number of input pins
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_ID,              // sourceID 1
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_SIDE_TONING_ID,    // sourceID 2
    0x02,                               // number of channels
    0x03,0x00,                          // channel config
    0x00,                               // iChannelNames
    0x00,                               // bmControls
    0x00,                               // iMixer
    
    /* USB Headset Output Terminal Descriptor */
    0x09,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,          // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_OUTPUT_TERMINAL_ID,             // ID of this Terminal.(bTerminalID)
    0x02,0x03,                          // (wTerminalType)See USB Audio Terminal Types.
    0x00,                               // No association (bAssocTerminal)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_ID,                // (bSourceID)
    0x00,                               // Unused. (iTerminal)

    /* USB Headset Output Terminal Microphone Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_OUTPUT_TERMINAL_MICROPHONE_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType)See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_FEATURE_UNIT_MICROPHONE_ID,             // (bSourceID)
    0x00,                            // Unused. (iTerminal)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,        // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,                              // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Headset Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    // GENERAL subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_INPUT_TERMINAL_ID,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Headset Type 1 Format Type Descriptor */
    0x11,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x02,                            // two channel.(bNrChannels)
//    0x01,
    0x02,                            // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x03,                            // two frequency supported. (bSamFreqType)
    
    0x80,0x3E,0x00,                  // 16000
//    0xC0,0x5D,0x00,                  // 24000
    0x00,0x7D,0x00,                  // 32000
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Headset Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    1 | USB_EP_DIRECTION_OUT,                            // Endpoint1:OUT  (bEndpointAddress)
    0x09,                            /* ?(bmAttributes) Isochronous,
                                      * asynchronous, data endpoint */
    0xC0,0x00,                      // ?(wMaxPacketSize) //48 * 4
//    0x40, 0x00,                     // 16*4
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Speaker Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x01,                            /* Turn on sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00,                        // Unused. (wLockDelay)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    0x02,                              // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    0x02,                           // Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                               // Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,                  // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,                     // GENERAL subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX0_DESCRIPTOR_OUTPUT_TERMINAL_MICROPHONE_ID,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x11,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,                   // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x01,                            // one channel.(bNrChannels)
    0x02,                            // 2 bytes per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x03,                            // One frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                  // Sampling Frequency = 16000 Hz(tSamFreq)
//    0xC0,0x5D,0x00,                  // 24000
    0x00,0x7D,0x00,                  // 32000
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    2 | USB_EP_DIRECTION_IN,                            // Endpoint2:IN (bEndpointAddress)
    0x0D,                            /* ?(bmAttributes) Isochronous,
                                      * asynchronous, data endpoint */
    0x60, 0x00,                      // ?(wMaxPacketSize) //48 * 2
//    0x40, 0x00,
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x01,                            /* Turn on sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00,                        // Unused. (wLockDelay)
	


};

/*******************************************
 * Array of Full speed Configuration 
 * descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/
 /*******************************************
 *  Language code string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),                                      // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,                              // STRING descriptor type
        {0x0409}                                            // Language ID
    };
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;                                    // Size of this descriptor in bytes
        uint8_t bDscType;                                   // STRING descriptor type
        uint16_t string[25];                                // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
		
    };

/*******************************************
 *  Product string descriptor
 *******************************************/
	const struct
    {
        uint8_t bLength;                                    // Size of this descriptor in bytes
        uint8_t bDscType;                                   // STRING descriptor type
        uint16_t string[37];                                // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		{'H','a','r','m','o','n','y',' ','U','S','B',' ','H','e','a','d','s','e','t',' ','P','I','C','3','2','M','Z','E','F',' ','E','x','a','m','p','l','e'}
    }; 

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002,
};

/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
 
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,                                      // Full speed descriptor
    1,                                                      // Total number of full speed configurations available
    fullSpeedConfigDescSet,                                 // Pointer to array of full speed configurations descriptors
	NULL, 
	0,
	NULL,
	3,  													// Total number of string descriptors available.
    stringDescriptors,                                      // Pointer to array of string descriptors.
	NULL, 
	NULL
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/

const USB_DEVICE_INIT usbDevInitData =
{
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 1,
	
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
	.deviceSpeed =  USB_SPEED_FULL,
	
	/* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBHS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBHS_DEVICE_INTERFACE,
	
};
// </editor-fold>
