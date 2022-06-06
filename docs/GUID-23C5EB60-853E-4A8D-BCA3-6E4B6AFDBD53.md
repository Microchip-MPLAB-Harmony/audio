# MPLAB Harmony Audio Driver and Peripherals Help

![](GUID-C743039C-0895-4E95-811A-FC2F250F7168-low.png) MPLAB Harmony Integrated Software Framework

**Parent topic:**[MPLAB® Harmony 3 Audio, Support Package](GUID-546D2D8B-EC03-4BF5-B23F-D618F41955FB.md)

## Driver Libraries Help

This section provides descriptions of the Driver libraries that are available in audio repo.

For additional information on Harmony 3 driver architecture, refer to the documentation in the core repository.

**Codec Libraries Help**

**AK4953 CODEC Driver Library Help**

This topic describes the AK4953 Codec Driver Library.

## *Introduction*

This topic describes the basic architecture of the AK4953 Codec Driver Library and provides information and examples on its use.

**Description**

### Interface Header File: drv\_AK4953.h

The interface to the AK4953 Codec Driver library is defined in the audio/driver/codec/AK4953/drv\_AK4953.h header file. Any C language source \(.c\) file that uses the AK4953 Codec Driver library should include this header.

**Library Source Files:**

The AK4953 Codec Driver library source files are provided in the audio/driver/codec/AK4953/src directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features and to **Building the Library** for instructions on how to build the library.

***Using the Library***

## Abstraction Model

This library provides a low-level abstraction of the AK4954 Codec Driver Library on the Microchip family microcontrollers with a convenient C language interface. This topic describes how that abstraction is modeled in software and introduces the library's interface.

**Description**

The abstraction model shown in the following diagram depicts how the AK4953 Codec Driver is positioned in the MPLAB Harmony framework. The AK4953 Codec Driver uses the I2C and I2S drivers for control and audio data transfers to the AK4953 module.

### AK4953 Driver Abstraction Model

![](GUID-A1402B4E-C779-4E72-B06A-3076BD3B42F4-low.png)

## Library Overview

Refer to the Driver Library Overview section for information on how the driver operates in a system.

The AK4953 Codec Driver Library provides an API interface to transfer control commands and digital audio data to the serially interfaced AK4953 Codec module. The library interface routines are divided into various sub-sections, which address one of the blocks or the overall operation of the AK4953 Codec Driver Library.

|**Library Interface Section**|**Description**|
|-----------------------------|---------------|
|System Functions|Provides system module interfaces, device initialization, deinitialization, reinitialization, tasks and status functions.|
|Client Setup Functions|Provides open and close functions.|
|Data Transfer Functions|Provides data transfer functions, such as Buffer Read and Write.|
|Settings Functions|Provides driver specific functions for settings, such as volume control and sampling rate.|
|Other Functions|Miscellaneous functions, such as getting the driver’s version number and syncing to the LRCLK signal.|
|Data Types and Constants|These data types and constants are required while interacting and setting up the AK4953 Codec Driver Library.|

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** All functions and constants in this section are named with the format DRV\_ AK4953\_xxx, where 'xxx' is a function name or constant. These names are redefined in the appropriate configuration’s configuration.h file to the format DRV\_CODEC\_xxx using \#defines so that code in the application that references the library can be written as generically as possible \(e.g., by writing DRV\_CODEC\_Open instead of DRV\_ AK4953\_Open etc.\). This allows the codec type to be changed in the MHC without having to modify the application’s source code.

## How the Library Works

How the Library Works

The library provides interfaces to support:

-   System Functionality

-   Client Functionality


**Description**

### Setup \(Initialization\)

This topic describes system initialization, implementations, and includes a system access code example.

**Description**

The system performs the initialization of the device driver with settings that affect only the instance of the device that is being initialized. During system initialization in the system\_init.c file, each instance of the AK4953 module would be initialized with the following configuration settings \(either passed dynamically at run time using DRV\_AK4953\_INIT or by using Initialization Overrides\) that are supported by the specific AK4953 device hardware:

-   Device requested power state: one of the System Module Power States. For specific details please refer to Data Types and Constants in the Library Interface section.

-   I2C driver module index. The module index should be same as the one used in initializing the I2C Driver

-   I2S driver module index. The module index should be same as the one used in initializing the I2S Driver

-   Sampling rate

-   Volume

-   Audio data format. The audio data format should match with the audio data format settings done in I2S driver initialization

-   Determines whether or not the microphone input is enabled


The DRV\_AK4953\_Initialize API returns an object handle of the type SYS\_MODULE\_OBJ. The object handle returned by the

Initialize interface would be used by the other system interfaces such as DRV\_ AK4953\_Deinitialize, DRV\_ AK4953\_Status and

DRV\_I2S\_Tasks

### Client Access

This topic describes driver initialization and provides a code example.

**Description**

For the application to start using an instance of the module, it must call the DRV\_AK4953\_Open function. The

DRV\_AK4953\_Open function provides a driver handle to the AK4953 Codec Driver instance for operations. If the driver is deinitialized using the function DRV\_AK4953\_Deinitialize, the application must call the DRV\_AK4953\_Open function again to set up the instance of the driver.

For the various options available for IO\_INTENT, please refer to Data Types and Constants in the Library Interface section.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is necessary to check the status of driver initialization before opening a driver instance. The status of the AK4953 Codec Driver can be known by calling DRV\_ AK4953\_Status.

**Example:**

DRV\_HANDLE handle; SYS\_STATUS ak4953Status;

ak4953Status Status = DRV\_AK4953\_Status\(sysObjects.ak4953Status DevObject\); if \(SYS\_STATUS\_READY == ak4953Status\)

\{

// The driver can now be opened.

appData.ak4953Client.handle = DRV\_AK4953\_Open

\(DRV\_AK4953\_INDEX\_0, DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_EXCLUSIVE\); if\(appData.ak4953Client.handle != DRV\_HANDLE\_INVALID\)

\{

appData.state = APP\_STATE\_AK4953\_SET\_BUFFER\_HANDLER;

\}

else

\{

SYS\_DEBUG\(0, "Find out what's wrong \\r\\n"\);

\}

\}

else

\{

/\* AK4953 Driver Is not ready \*/

### Client Operations

This topic provides information on client operations.

**Description**

Client operations provide the API interface for control command and audio data transfer to the AK4953 Codec.

The following AK4953 Codec specific control command functions are provided:

-   DRV\_AK4953\_SamplingRateSet

-   DRV\_AK4953\_SamplingRateGet

-   DRV\_AK4953\_VolumeSet

-   DRV\_AK4953\_VolumeGet

-   DRV\_AK4953\_MuteOn

-   DRV\_AK4953\_MuteOff


These functions schedule a non-blocking control command transfer operation. These functions submit the control command request to the AK4953 Codec. These functions submit the control command request to I2C Driver transmit queue, the request is processed immediately if it is the first request, or processed when the previous request is complete.

DRV\_AK4953\_BufferAddWrite, DRV\_AK4953\_BufferAddRead, and DRV\_AK4953\_BufferAddWriteRead are buffered data operation functions. These functions schedule non-blocking audio data transfer operations. These functions add the request to I2S Driver transmit or receive buffer queue depends on the request type, and are executed immediately if it is the first buffer, or executed later when the previous buffer is complete. The driver notifies the client with

DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE, DRV\_AK4953\_BUFFER\_EVENT\_ERROR, or DRV\_AK4953\_BUFFER\_EVENT\_ABORT events.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is not necessary to close and reopen the client between multiple transfers.

### Configuring the Library

The configuration of the I2S Driver Library is based on the file configurations.h, as generated by the MHC.

This header file contains the configuration selection for the I2S Driver Library. Based on the selections made, the I2S Driver Library may support the selected features. These configuration settings will apply to all instances of the I2S Driver Library.

This header can be placed anywhere; however, the path of this header needs to be present in the include search path for a successful build. Refer to the Applications Help section for more details.

## System Configuration

### Configuring MHC

Provides examples on how to configure the MPLAB Harmony Configurator \(MHC\) for a specific driver.

**Description**

When building a new application, start by creating a 32-bit MPLAB Harmony 3 project in MPLAB X IDE by selecting *File \> New Project*. Chose the Configuration name the based on the BSP, and select the appropriate processor.

In the MHC, under Available Components select the appropriate BSP. Under *Audio\>Templates*, double-click on a codec template such as AK4953. Answer Yes to all questions.

You should end up with a project graph that looks like this, after rearranging the boxes: ![](GUID-C3554075-908C-444B-BDDF-6DB178E075E1-low.png)

Click on the AK4953 Driver component \(not AK4953 Codec\) and the following menu will be displayed in the Configurations Options: ![](GUID-E61F7652-A17E-450F-9462-6E561EC13E19-low.png)

-   **I2C Driver Used** will display the driver instance used for the I2C interface.

-   **I2S Driver Used** will display the driver instance used for the I2S interface.

-   **Usage Mode** indicates whether the AK4953 is a Master \(supplies I2S clocks\) or a Slave \(MCU supplies I2S clocks\).

-   **Number of AK4953 Clients** indicates the maximum number of clients that can be connected to the AK4953 Driver.

-   **Sampling Rate** indicates the number of samples per second per channel, 8000 to 96,000.

-   **Volume** indicates the volume in a linear scale from 0-255.


#### • Audio Data Format is either

-   24-bit Left Justified \(ADC\), 24-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 16-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 24-bit Left-justified\(DAC\)

-   24/16-bit I2S


It must match the audio protocol and data length set up in either the SSC or I2S PLIB.

-   **Microphone/Line Input** selects which microphone or line input is selected, either:

-   Internal Mic \(mounted on the AK4953 daughterboard\)

-   External Mic Input

-   Line Input


If External Mic input or Line Input is selected, then the following option is provided:

#### • Ext Mic Gain in dB range 0\(min\) to 31\(max\)

If External Mic input is selected, then the following option is provided:

• **Enable Microphone Bias** should be checked if using an electret microphone.

You can also bring in the AK4953 Driver by itself, by double clicking AK4953 under Audio\_-\>Driver-\>Codec\_ in the Available Components list. You will then need to add any additional needed components manually and connect them together.

Note that the AK4953 requires the TCx Peripheral Library and TIME System Service in order to perform some of its internal timing sequences.

### Bulding the Library

This section lists the files that are available in the AK4953 Codec Driver Library.

**Description**

This section lists the files that are available in the src folder of the AK4953 Codec Driver. It lists which files need to be included in the build based on either a hardware feature present on the board or configuration option selected by the system.

The following three tables list and describe the header \(.h\) and source \(.c\) files that implement this library. The parent folder for these files is audio/driver/codec/AK4953\\. **Interface File\(s\)**

This table lists and describes the header files that must be included \(i.e., using \#include\) by any code that uses this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|drv\_ak4953.h|Header file that exports the driver API.|

#### Required File\(s\)

![](GUID-B924D07A-0D74-453B-BCDE-8190A195C55D-low.png) ***All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use.***

This table lists and describes the source and header files that must *always* be included in the MPLAB X IDE project to build this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|/src/drv\_ak4953.c|This file contains implementation of the AK4953 Codec Driver.|

#### Optional File\(s\)

This table lists and describes the source and header files that may optionally be included if required for the desired implementation.

|**Source File Name**|**Description**|
|--------------------|---------------|
|N/A|No optional files are available for this library.|

#### Module Dependencies

The AK4953 Codec Driver Library depends on the following modules:

-   I2S Driver Library

-   I2C Driver Library


### Library Interface

**Client Setup Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4953\_Close|Closes an opened-instance of the AK4953 driver|
||DRV\_AK4953\_CommandEventHandlerSet|This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.|
||DRV\_AK4953\_Open|Opens the specified AK4953 driver instance and returns a handle to it.|
||DRV\_AK4953\_BufferEventHandlerSet|This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.|

**Data Transfer Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4953\_BufferAddRead|Schedule a non-blocking driver read operation.|
||DRV\_AK4953\_BufferAddWrite|Schedule a non-blocking driver write operation.|
||DRV\_AK4953\_BufferAddWriteRead|Schedule a non-blocking driver write-read operation.|
||DRV\_AK4953\_ReadQueuePurge|Removes all buffer requests from the read queue.|
||DRV\_AK4953\_WriteQueuePurge|Removes all buffer requests from the write queue.|

**Data Types and Constants**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_AK4953\_AUDIO\_DATA\_FORMAT|Identifies the Serial Audio data interface format.|
|-|DRV\_AK4953\_BUFFER\_EVENT|Identifies the possible events that can result from a buffer add request.|
|-|DRV\_AK4953\_BUFFER\_EVENT\_HANDLER|Pointer to a AK4953 Driver Buffer Event handler function|
|-|DRV\_AK4953\_BUFFER\_HANDLE|Handle identifying a write buffer passed to the driver.|
|-|DRV\_AK4953\_CHANNEL|Identifies Left/Right Audio channel|
|-|DRV\_AK4953\_COMMAND\_EVENT\_HANDLER|Pointer to a AK4953 Driver Command Event Handler Function|
|-|DRV\_AK4953\_DIGITAL\_BLOCK\_CONTROL|Identifies Bass-Boost Control function|
|-|DRV\_AK4953\_INIT|Defines the data required to initialize or reinitialize the AK4953 driver|
|-|DRV\_AK4953\_INT\_EXT\_MIC|Identifies the Mic input source.|
|-|DRV\_AK4953\_MIC|This is type DRV\_AK4953\_MIC.|
|-|DRV\_AK4953\_MONO\_STEREO\_MIC|Identifies the Mic input as Mono / Stereo.|

**Other Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4953\_GetI2SDriver|Get the handle to the I2S driver for this codec instance.|
||DRV\_AK4953\_LRCLK\_Sync|Synchronize to the start of the I2S LRCLK \(left/right clock\) signal|
||DRV\_AK4953\_VersionGet|This function returns the version of AK4953 driver.|
||DRV\_AK4953\_VersionStrGet|This function returns the version of AK4953 driver in string format.|

**Settings Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4953\_IntExtMicSet|This function sets up the codec for the X32 DB internal or the external microphone use.|
||DRV\_AK4953\_MicGainGet|This function gets the microphone gain for the AK4953 Codec.|
||DRV\_AK4953\_MicGainSet|This function sets the microphone gain for the AK4953 CODEC.|
||DRV\_AK4953\_MicMuteOff|Umutes th AK4953's microphone input.|
||DRV\_AK4953\_MicMuteOn|Mutes the AK4953's microphone input|
||DRV\_AK4953\_MicSet|This function sets up the codec for the internal or the AK4953 Mic1 or Mic2 input.|
||DRV\_AK4953\_MonoStereoMicSet|This function sets up the codec for the Mono or Stereo microphone mode.|
||DRV\_AK4953\_MuteOff|This function disables AK4953 output for soft mute.|
||DRV\_AK4953\_MuteOn|This function allows AK4953 output for soft mute on.|
||DRV\_AK4953\_SamplingRateGet|This function gets the sampling rate set on the DAC AK4953.|
||DRV\_AK4953\_SamplingRateSet|This function sets the sampling rate of the media stream.|
||DRV\_AK4953\_VolumeGet|This function gets the volume for AK4953 Codec.|
||DRV\_AK4953\_VolumeSet|This function sets the volume for AK4953 Codec.|

**System Interaction Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4953\_Deinitialize|Deinitializes the specified instance of the AK4953 driver module.|
||DRV\_AK4953\_EnableInitialization|Enable delayed initialization of the driver.|
||DRV\_AK4953\_Initialize|Initializes hardware and data for the instance of the AK4953 Codec module.|
||DRV\_AK4953\_IsInitializationDelayed|Checks if delayed initialization of the driver has been requested.|
||DRV\_AK4953\_Status|Gets the current status of the AK4953 driver module.|
||DRV\_AK4953\_Tasks|Maintains the driver's control and data interface state machine.|

**Description**

## System Interaction Functions

### DRV\_AK4953\_Deinitialize Function

void DRV\_AK4953\_Deinitialize\( SYS\_MODULE\_OBJ object\)

**Summary**

Deinitializes the specified instance of the AK4953 driver module.

**Description**

Deinitializes the specified instance of the AK4953 driver module, disabling its operation \(and any hardware\). Invalidates all the internal data.

**Preconditions**

Function DRV\_AK4953\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_AK4953\_Initialize routine|

**Returns**

None.

**Remarks**

Once the Initialize operation has been called, the De-initialize operation must be called before the Initialize operation can be called again. This routine will NEVER block waiting for hardware.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4953\_Initialize*

SYS\_STATUS status;

DRV\_AK4953\_Deinitialize\(object\); status = DRV\_AK4953\_Status\(object\); **if** \(SYS\_MODULE\_DEINITIALIZED != status\) \{

*// Check again later if you need to know* *// when the driver is deinitialized.*

\}

**C**

**void** **DRV\_AK4953\_Deinitialize**\(SYS\_MODULE\_OBJ **object**\);

### DRV\_AK4953\_EnableInitialization Function

void DRV\_AK4953\_EnableInitialization\(SYS\_MODULE\_OBJ object\);

**Summary**

Enable delayed initialization of the driver.

**Description**

If the AK4953 codec is sharing a RESET line with another peripheral, such as a Bluetooth module with its own driver, then the codec driver initialization has to be delayed until after the Bluetooth module has toggled its RESET pin. Once this has been accomplished, this function should be called to kick-start the codec driver initialization.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4953\_Initialize\)|

**Returns**

None.

**Remarks**

This is not needed for audio-only applications without a Bluetooth module.

**C**

**void** **DRV\_AK4953\_EnableInitialization**\(SYS\_MODULE\_OBJ **object**\);

### DRV\_AK4953\_Initialize Function

SYS\_MODULE\_OBJ DRV\_AK4953\_Initialize

\(

const SYS\_MODULE\_INDEX drvIndex, const SYS\_MODULE\_INIT \*const init

\);

**Summary**

Initializes hardware and data for the instance of the AK4953 Codec module.

**Description**

This routine initializes the AK4953 driver instance for the specified driver index, making it ready for clients to open and use it. The initialization data is specified by the init parameter. The initialization may fail if the number of driver objects allocated are insufficient or if the specified driver instance is already initialized.

**Preconditions**

DRV\_I2S\_Initialize must be called before calling this function to initialize the data interface of this Codec driver. DRV\_I2C\_Initialize must be called if SPI driver is used for handling the control interface of this Codec driver.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the driver instance to be initialized|
|init|Pointer to the data structure containing any data necessary to initialize the hardware. This pointer may be null if no data is required and default initialization is to be used.|

**Returns**

If successful, returns a valid handle to a driver instance object. Otherwise, it returns SYS\_MODULE\_OBJ\_INVALID.

**Remarks**

This routine must be called before any other AK4953 routine is called.

This routine should only be called once during system initialization unless DRV\_AK4953\_Deinitialize is called to deinitialize the driver instance. This routine will NEVER block for hardware access.

**Example**

DRV\_AK4953\_INIT init;

SYS\_MODULE\_OBJ objectHandle; init-\>inUse = **true**; init-\>status = SYS\_STATUS\_BUSY; init-\>numClients = 0; init-\>i2sDriverModuleIndex = ak4953Init-\>i2sDriverModuleIndex; init-\>i2cDriverModuleIndex = ak4953Init-\>i2cDriverModuleIndex; init-\>samplingRate = DRV\_AK4953\_AUDIO\_SAMPLING\_RATE; init-\>audioDataFormat = DRV\_AK4953\_AUDIO\_DATA\_FORMAT\_MACRO; **for**\(index=0; index < DRV\_AK4953\_NUMBER\_OF\_CHANNELS; index++\) \{

init-\>volume = ak4953Init-\>volume; \} init-\>isInInterruptContext = **false**; init-\>commandCompleteCallback = \(DRV\_AK4953\_COMMAND\_EVENT\_HANDLER\)0; init-\>commandContextData = 0; init-\>mclk\_multiplier = DRV\_AK4953\_MCLK\_SAMPLE\_FREQ\_MULTPLIER; objectHandle = DRV\_AK4953\_Initialize\(DRV\_AK4953\_0, \(SYS\_MODULE\_INIT\*\)init\); **if** \(SYS\_MODULE\_OBJ\_INVALID == objectHandle\) \{

*// Handle error*

\}

**C**

SYS\_MODULE\_OBJ **DRV\_AK4953\_Initialize**\(**const** SYS\_MODULE\_INDEX **drvIndex**, **const** SYS\_MODULE\_INIT \* **const** **init**\);

### DRV\_AK4953\_IsInitializationDelayed Function

bool DRV\_AK4953\_IsInitializationDelayed\(SYS\_MODULE\_OBJ object\);

**Summary**

Checks if delayed initialization of the driver has been requested.

**Description**

If the AK4953 codec is sharing a RESET line with another peripheral, such as a Bluetooth module with its own driver, then the codec driver initialization has to be delayed until after the Bluetooth module has toggled its RESET pin. This function returns true if that option has been selected in MHC in the checkbox: "Delay driver initialization \(due to shared RESET pin\)"

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4953\_Initialize\)|

**Returns**

true if the delayed initialization option has been enabled

**Remarks**

This is not needed for audio-only applications without a Bluetooth module.

**C**

**bool** **DRV\_AK4953\_IsInitializationDelayed**\(SYS\_MODULE\_OBJ **object**\);

### DRV\_AK4953\_Status Function

SYS\_STATUS DRV\_AK4953\_Status\( SYS\_MODULE\_OBJ object\)

**Summary**

Gets the current status of the AK4953 driver module.

**Description**

This routine provides the current status of the AK4953 driver module.

**Preconditions**

Function DRV\_AK4953\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_AK4953\_Initialize routine|

**Returns**

SYS\_STATUS\_DEINITIALIZED - Indicates that the driver has been deinitialized

SYS\_STATUS\_READY - Indicates that any previous module operation for the specified module has completed

SYS\_STATUS\_BUSY - Indicates that a previous module operation for the specified module has not yet completed

SYS\_STATUS\_ERROR - Indicates that the specified module is in an error state

**Remarks**

A driver can opened only when its status is SYS\_STATUS\_READY.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4953\_Initialize*

SYS\_STATUS AK4953Status;

AK4953Status = DRV\_AK4953\_Status\(object\); **if** \(SYS\_STATUS\_READY == AK4953Status\) \{

*// This means the driver can be opened using the* *// DRV\_AK4953\_Open\(\) function.*

\}

**C**

SYS\_STATUS **DRV\_AK4953\_Status**\(SYS\_MODULE\_OBJ **object**\);

### DRV\_AK4953\_Tasks Function

void DRV\_AK4953\_Tasks\(SYS\_MODULE\_OBJ object\);

**Summary**

Maintains the driver's control and data interface state machine.

**Description**

This routine is used to maintain the driver's internal control and data interface state machine and implement its control and data interface implementations. This function should be called from the SYS\_Tasks\(\) function.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4953\_Initialize\)|

**Returns**

None.

**Remarks**

This routine is normally not called directly by an application. It is called by the system's Tasks routine \(SYS\_Tasks\).

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4953\_Initialize* **while** \(**true**\) \{

DRV\_AK4953\_Tasks \(object\);

*// Do other tasks*

\}

**C**

**void** **DRV\_AK4953\_Tasks**\(SYS\_MODULE\_OBJ **object**\);

## Client Setup Functions

### DRV\_AK4953\_Close Function

void DRV\_AK4953\_Close\( DRV\_Handle handle \)

**Summary**

Closes an opened-instance of the AK4953 driver

**Description**

This routine closes an opened-instance of the AK4953 driver, invalidating the handle. Any buffers in the driver queue that were submitted by this client will be removed. After calling this routine, the handle passed in "handle" must not be used with any of the remaining driver routines. A new handle must be obtained by calling DRV\_AK4953\_Open before the caller may use the driver again

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

Usually there is no need for the driver client to verify that the Close operation has completed. The driver will abort any ongoing operations when this routine is called.

**Example**

DRV\_HANDLE handle; *// Returned from DRV\_AK4953\_Open*

DRV\_AK4953\_Close\(handle\);

**C**

**void** **DRV\_AK4953\_Close**\(**const** DRV\_HANDLE **handle**\);

### DRV\_AK4953\_CommandEventHandlerSet Function

void DRV\_AK4953\_CommandEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_AK4953\_COMMAND\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

**Description**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

When a client calls DRV\_AK4953\_BufferAddWrite function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "AK4953 CODEC Specific Client Routines" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the command has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4953\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4953\_CommandEventHandlerSet\(myAK4953Handle,

APP\_AK4953CommandEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4953\_DeEmphasisFilterSet\(myAK4953Handle, DRV\_AK4953\_DEEMPHASIS\_FILTER\_44\_1KHZ\)

*// Event is received when // the buffer is processed.* **void** APP\_AK4953CommandEventHandler\(uintptr\_t contextHandle\) \{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ *// Last Submitted command is completed.*

*// Perform further processing here*

\}

\}

**C**

**void** **DRV\_AK4953\_CommandEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_AK4953\_COMMAND\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

### DRV\_AK4953\_Open Function

DRV\_HANDLE DRV\_AK4953\_Open

\(

const SYS\_MODULE\_INDEX drvIndex, const DRV\_IO\_INTENT ioIntent

\)

**Summary**

Opens the specified AK4953 driver instance and returns a handle to it.

**Description**

This routine opens the specified AK4953 driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver. The ioIntent parameter defines how the client interacts with this driver instance.

The DRV\_IO\_INTENT\_BLOCKING and DRV\_IO\_INTENT\_NONBLOCKING ioIntent options are not relevant to this driver. All the data transfer functions of this driver are non blocking.

AK4953 can be opened with DRV\_IO\_INTENT\_WRITE, or DRV\_IO\_INTENT\_READ or DRV\_IO\_INTENT\_WRITEREAD io\_intent option. This decides whether the driver is used for headphone output, or microphone input or both modes simultaneously.

Specifying a DRV\_IO\_INTENT\_EXCLUSIVE will cause the driver to provide exclusive access to this client. The driver cannot be opened by any other client.

**Preconditions**

Function DRV\_AK4953\_Initialize must have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the object instance to be opened|
|ioIntent|Zero or more of the values from the enumeration DRV\_IO\_INTENT "ORed" together to indicate the intended use of the driver. See function description for details.|

**Returns**

If successful, the routine returns a valid open-instance handle \(a number identifying both the caller and the module instance\).

If an error occurs, the return value is DRV\_HANDLE\_INVALID. Error can occur

-   if the number of client objects allocated via DRV\_AK4953\_CLIENTS\_NUMBER is insufficient.

-   if the client is trying to open the driver but driver has been opened exclusively by another client.

-   if the driver hardware instance being opened is not initialized or is invalid.

-   if the ioIntent options passed are not relevant to this driver.


**Remarks**

The handle returned is valid until the DRV\_AK4953\_Close routine is called. This routine will NEVER block waiting for hardware.If the requested intent flags are not supported, the routine will return DRV\_HANDLE\_INVALID. This function is thread safe in a RTOS application. It should not be called in an ISR.

**Example**

DRV\_HANDLE handle; handle = DRV\_AK4953\_Open\(DRV\_AK4953\_INDEX\_0, DRV\_IO\_INTENT\_WRITEREAD \| DRV\_IO\_INTENT\_EXCLUSIVE\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver*

*// May be the driver is not initialized or the initialization* *// is not complete.*

\}

**C**

DRV\_HANDLE **DRV\_AK4953\_Open**\(**const** SYS\_MODULE\_INDEX **iDriver**, **const** DRV\_IO\_INTENT **ioIntent**\);

### DRV\_AK4953\_BufferEventHandlerSet Function

void DRV\_AK4953\_BufferEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_AK4953\_BUFFER\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.

**Description**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished. When a client calls DRV\_AK4953\_BufferAddRead function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "buffer add" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the queued buffer transfer has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4953\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4953\_BufferEventHandlerSet\(myAK4953Handle,

APP\_AK4953BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4953\_BufferAddRead\(myAK4953handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_AK4953\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4953BufferEventHandler\(DRV\_AK4953\_BUFFER\_EVENT event, DRV\_AK4953\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_AK4953\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4953\_BufferEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_AK4953\_BUFFER\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

## Data Transfer Functions

### DRV\_AK4953\_BufferAddRead Function

void DRV\_AK4953\_BufferAddRead

\(

const DRV\_HANDLE handle,

DRV\_AK4953\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver read operation.

**Description**

This function schedules a non-blocking read operation. The function returns with a valid buffer handle in the bufferHandle argument if the read request was scheduled successfully. The function adds the request to the hardware instance receive queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4953\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4953\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 device instance and the DRV\_AK4953\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle. DRV\_IO\_INTENT\_READ must have been specified in the DRV\_AK4953\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4953 instance as return by the DRV\_AK4953\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4953\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4953 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4953 driver instance. It should not otherwise be called directly in an ISR.

**C**

**void** **DRV\_AK4953\_BufferAddRead**\(**const** DRV\_HANDLE **handle**, DRV\_AK4953\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

### DRV\_AK4953\_BufferAddWrite Function

void DRV\_AK4953\_BufferAddWrite

\(

const DRV\_HANDLE handle,

DRV\_AK4953\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write operation.

**Description**

This function schedules a non-blocking write operation. The function returns with a valid buffer handle in the bufferHandle argument if the write request was scheduled successfully. The function adds the request to the hardware instance transmit queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4953\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4953\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 device instance and the DRV\_AK4953\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_WRITE must have been specified in the DRV\_AK4953\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4953 instance as return by the DRV\_AK4953\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4953\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4953 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4953 driver instance. It should not otherwise be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4953\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4953\_BufferEventHandlerSet\(myAK4953Handle,

APP\_AK4953BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4953\_BufferAddWrite\(myAK4953handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_AK4953\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{ *// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4953BufferEventHandler\(DRV\_AK4953\_BUFFER\_EVENT event, DRV\_AK4953\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.*

**break**; **case** DRV\_AK4953\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

**break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4953\_BufferAddWrite**\(**const** DRV\_HANDLE **handle**, DRV\_AK4953\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

### DRV\_AK4953\_BufferAddWriteRead Function

void DRV\_AK4953\_BufferAddWriteRead

\(

const DRV\_HANDLE handle,

DRV\_AK4953\_BUFFER\_HANDLE \*bufferHandle,

void \*transmitBuffer, void \*receiveBuffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write-read operation.

**Description**

This function schedules a non-blocking write-read operation. The function returns with a valid buffer handle in the bufferHandle argument if the write-read request was scheduled successfully. The function adds the request to the hardware instance queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read only or write only

-   if the buffer size is 0

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4953\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 device instance and the DRV\_AK4953\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_READWRITE must have been specified in the DRV\_AK4953\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4953 instance as returned by the DRV\_AK4953\_Open function|
|bufferHandle|Pointer to an argument that will contain the return buffer handle|
|transmitBuffer|The buffer where the transmit data will be stored|
|receiveBuffer|The buffer where the received data will be stored|
|size|Buffer size in bytes|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4953\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4953 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4953 driver instance. It should not otherwise be called directly in an ISR.

This function is useful when there is valid read expected for every AK4953 write. The transmit and receive size must be same.

**Example**

MY\_APP\_OBJ myAppObj; uint8\_t mybufferTx; uint8\_t mybufferRx; DRV\_AK4953\_BUFFER\_HANDLE bufferHandle;

*// myak4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4953\_BufferEventHandlerSet\(myak4953Handle,

APP\_AK4953BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4953\_BufferAddWriteRead\(myak4953handle, &bufferHandle, mybufferTx,mybufferRx,MY\_BUFFER\_SIZE\); **if**\(DRV\_AK4953\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4953BufferEventHandler\(DRV\_AK4953\_BUFFER\_EVENT event, DRV\_AK4953\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.*

**break**; **case** DRV\_AK4953\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

**break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4953\_BufferAddWriteRead**\(**const** DRV\_HANDLE **handle**, DRV\_AK4953\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **transmitBuffer**, **void** \* **receiveBuffer**, size\_t **size**\);

### DRV\_AK4953\_ReadQueuePurge Function

bool DRV\_AK4953\_ReadQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the read queue.

**Description**

This function removes all the buffer requests from the read queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_AK4953\_Open function.|

**Returns**

True - Read queue purge is successful. False - Read queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_AK4953\_Open function.*

*// Use DRV\_AK4953\_BufferAddRead to queue read requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_AK4953\_ReadQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the read queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_AK4953\_ReadQueuePurge**\(**const** DRV\_HANDLE **handle**\);

### DRV\_AK4953\_WriteQueuePurge Function

bool DRV\_AK4953\_WriteQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the write queue.

**Description**

This function removes all the buffer requests from the write queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_AK4953\_Open function.|

**Returns**

True - Write queue purge is successful. False - Write queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_AK4953\_Open function.*

*// Use DRV\_AK4953\_BufferAddWrite to queue write requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_AK4953\_WriteQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the write queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_AK4953\_WriteQueuePurge**\(**const** DRV\_HANDLE **handle**\);

## Settings Functions

### DRV\_AK4953\_IntExtMicSet Function

void DRV\_AK4953\_IntExtMicSet

**Summary**

This function sets up the codec for the X32 DB internal or the external microphone use.

**Description**

This function sets up the codec for the internal or the external microphone use.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|micInput|Internal vs External mic input|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4953\_IntExtMicSet**\(DRV\_HANDLE **handle**, DRV\_AK4953\_INT\_EXT\_MIC **micInput**\);

### DRV\_AK4953\_MicGainGet Function

uint8\_t DRV\_AK4953\_MicGainGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the microphone gain for the AK4953 Codec.

**Description**

This functions gets the current microphone gain programmed to the Codec AK4953.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

Microphone gain, in range 0-31.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t gain;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

gain = DRV\_AK4953\_MicGainGet\(myAK4953Handle\);

**C**

uint8\_t **DRV\_AK4953\_MicGainGet**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_MicGainSet Function

void DRV\_AK4953\_MicGainSet\(DRV\_HANDLE handle, uint8\_t gain\)

**Summary**

This function sets the microphone gain for the AK4953 CODEC.

**Description**

This functions sets the microphone gain value from 0-31 which can range from -1.5 to 28.3 dB

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|gain|Gain value, in range 0-31|

**Returns**

None.

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_MicGainSet\(myAK4953Handle, 15\); *//AK4953 mic gain set to 15*

**Remarks**

None.

**C**

**void** **DRV\_AK4953\_MicGainSet**\(DRV\_HANDLE **handle**, uint8\_t **gain**\);

### DRV\_AK4953\_MicMuteOff Function

void DRV\_AK4953\_MicMuteOff\(DRV\_HANDLE handle\)

**Summary**

Umutes th AK4953's microphone input.

**Description**

This function unmutes the AK4953's microphone input.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_MicMuteOff\(myAK4953Handle\); *//AK4953 microphone unmuted*

**C**

**void** **DRV\_AK4953\_MicMuteOff**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_MicMuteOn Function

void DRV\_AK4953\_MicMuteOn\(DRV\_HANDLE handle\);

**Summary**

Mutes the AK4953's microphone input

**Description**

This function mutes the AK4953's microphone input

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_MicMuteOn\(myAK4953Handle\); *//AK4953 microphone muted*

**C**

**void** **DRV\_AK4953\_MicMuteOn**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_MicSet Function

void DRV\_AK4953\_IntMic12Set

**Summary**

This function sets up the codec for the internal or the AK4953 Mic1 or Mic2 input.

**Description**

This function sets up the codec.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|micInput|Internal vs External mic input|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4953\_MicSet**\(DRV\_HANDLE **handle**, DRV\_AK4953\_MIC **micInput**\);

### DRV\_AK4953\_MonoStereoMicSet Function

void DRV\_AK4953\_MonoStereoMicSet\(DRV\_HANDLE handle\);

**Summary**

This function sets up the codec for the Mono or Stereo microphone mode.

**Description**

This function sets up the codec for the Mono or Stereo microphone mode.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4953\_MonoStereoMicSet**\(DRV\_HANDLE **handle**, DRV\_AK4953\_MONO\_STEREO\_MIC **mono\_stereo\_mic**\);

### DRV\_AK4953\_MuteOff Function

void DRV\_AK4953\_MuteOff\(DRV\_HANDLE handle\)

**Summary**

This function disables AK4953 output for soft mute.

**Description**

This function disables AK4953 output for soft mute.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_MuteOff\(myAK4953Handle\); *//AK4953 output soft mute disabled*

**C**

**void** **DRV\_AK4953\_MuteOff**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_MuteOn Function

void DRV\_AK4953\_MuteOn\(DRV\_HANDLE handle\);

**Summary**

This function allows AK4953 output for soft mute on.

**Description**

This function Enables AK4953 output for soft mute.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_MuteOn\(myAK4953Handle\); *//AK4953 output soft muted*

**C**

**void** **DRV\_AK4953\_MuteOn**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_SamplingRateGet Function

uint32\_t DRV\_AK4953\_SamplingRateGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the sampling rate set on the DAC AK4953.

**Description**

This function gets the sampling rate set on the DAC AK4953.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

uint32\_t baudRate;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

baudRate = DRV\_AK4953\_SamplingRateGet\(myAK4953Handle\);

**C**

uint32\_t **DRV\_AK4953\_SamplingRateGet**\(DRV\_HANDLE **handle**\);

### DRV\_AK4953\_SamplingRateSet Function

void DRV\_AK4953\_SamplingRateSet\(DRV\_HANDLE handle, uint32\_t samplingRate\)

**Summary**

This function sets the sampling rate of the media stream.

**Description**

This function sets the media sampling rate for the client handle.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_SamplingRateSet\(myAK4953Handle, 48000\); *//Sets 48000 media sampling rate*

**C**

**void** **DRV\_AK4953\_SamplingRateSet**\(DRV\_HANDLE **handle**, uint32\_t **samplingRate**\);

### DRV\_AK4953\_VolumeGet Function

uint8\_t DRV\_AK4953\_VolumeGet\(DRV\_HANDLE handle, DRV\_AK4953\_CHANNEL chan\)

**Summary**

This function gets the volume for AK4953 Codec.

**Description**

This functions gets the current volume programmed to the CODEC AK4953.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|chan|Audio channel volume to be set|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t volume;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

volume = DRV\_AK4953\_VolumeGet\(myAK4953Handle,DRV\_AK4953\_CHANNEL\_LEFT\);

**C**

uint8\_t **DRV\_AK4953\_VolumeGet**\(DRV\_HANDLE **handle**, DRV\_AK4953\_CHANNEL **chan**\);

### DRV\_AK4953\_VolumeSet Function

void DRV\_AK4953\_VolumeSet\(DRV\_HANDLE handle, DRV\_AK4953\_CHANNEL channel, uint8\_t volume\);

**Summary**

This function sets the volume for AK4953 Codec.

**Description**

This functions sets the volume value from 0-255. The codec has DAC value to volume range mapping as :- 00 H : +12dB FF H : -115dB In order to make the volume value to dB mapping monotonically increasing from 00 to FF, re-mapping is introduced which reverses the volume value to dB mapping as well as normalizes the volume range to a more audible dB range. The current driver implementation assumes that all dB values under -60 dB are inaudible to the human ear. Re-Mapped values 00 H : -60 dB FF H : +12 dB

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|chan|Audio channel volume to be set|
|volume|volume value specified in the range 0-255 \(0x00 to 0xFF\)|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4953Handle is the handle returned // by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_VolumeSet\(myAK4953Handle, DRV\_AK4953\_CHANNEL\_LEFT, 120\);

**C**

**void** **DRV\_AK4953\_VolumeSet**\(DRV\_HANDLE **handle**, DRV\_AK4953\_CHANNEL **channel**, uint8\_t **volume**\);

## Other Functions

### DRV\_AK4953\_GetI2SDriver Function

DRV\_HANDLE DRV\_AK4953\_GetI2SDriver\(DRV\_HANDLE codecHandle\)

**Summary**

Get the handle to the I2S driver for this codec instance.

**Description**

Returns the appropriate handle to the I2S based on the ioIent member of the codec object.

**Preconditions**

The DRV\_AK4953\_Initialize routine must have been called for the specified AK4953 driver instance.

DRV\_AK4953\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

-   A handle to the I2S driver for this codec instance


**Remarks**

This allows the caller to directly access portions of the I2S driver that might not be available via the codec API.

**C**

DRV\_HANDLE **DRV\_AK4953\_GetI2SDriver**\(DRV\_HANDLE **codecHandle**\);

### DRV\_AK4953\_LRCLK\_Sync Function

uint32\_t DRV\_AK4953\_LRCLK\_Sync \(const DRV\_HANDLE handle\);

**Summary**

Synchronize to the start of the I2S LRCLK \(left/right clock\) signal

**Description**

This function waits until low-to high transition of the I2S LRCLK \(left/right clock\) signal \(high-low if Left-Justified format, this is determined by the PLIB\). In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize calls to the DMA with the LRCLK signal so the left/right channel association is valid.

**Preconditions**

None.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myAK4953Handle is the handle returned* *// by the DRV\_AK4953\_Open function.*

DRV\_AK4953\_LRCLK\_Sync\(myAK4953Handle\);

**C**

**bool** **DRV\_AK4953\_LRCLK\_Sync**\(**const** DRV\_HANDLE **handle**\);

### DRV\_AK4953\_VersionGet Function

uint32\_t DRV\_AK4953\_VersionGet\( void \)

**Summary**

This function returns the version of AK4953 driver.

**Description**

The version number returned from the DRV\_AK4953\_VersionGet function is an unsigned integer in the following decimal format. \* 10000 + \* 100 + Where the numbers are represented in decimal and the meaning is the same as above. Note that there is no numerical representation of release type.

**Preconditions**

None.

**Returns**

returns the version of AK4953 driver.

**Remarks**

None.

**Example 1**

For version "0.03a", return: 0 \* 10000 + 3 \* 100 + 0 For version "1.00", return: 1 \* 100000 + 0 \* 100 + 0

**Example 2**

uint32\_t AK4953version; AK4953version = DRV\_AK4953\_VersionGet\(\);

**C**

uint32\_t **DRV\_AK4953\_VersionGet**\(\);

### DRV\_AK4953\_VersionStrGet Function

int8\_t\* DRV\_AK4953\_VersionStrGet\(void\)

**Summary**

This function returns the version of AK4953 driver in string format.

**Description**

The DRV\_AK4953\_VersionStrGet function returns a string in the format: "." Where: is the AK4953 driver's version number. is the AK4953 driver's version number. is an optional "patch" or "dot" release number \(which is not included in the string if it equals "00"\). is an optional release type \("a" for alpha, "b" for beta ? not the entire word spelled out\) that is not included if the release is a production version \(I.e. Not an alpha or beta\).

The String does not contain any spaces.

**Preconditions**

None.

**Returns**

returns a string containing the version of AK4953 driver.

**Remarks**

None.

**Example 1**

"0.03a" "1.00"

**Example 2**

int8\_t \*AK4953string; AK4953string = DRV\_AK4953\_VersionStrGet\(\);

**C**

int8\_t\* **DRV\_AK4953\_VersionStrGet**\(\);

**Data Types and Constants**

## *DRV\_AK4953\_AUDIO\_DATA\_FORMAT Type*

Identifies the Serial Audio data interface format.

**Description**

AK4953 Audio data format

This enumeration identifies Serial Audio data interface format.

**C**

**typedef** **enum** DRV\_AK4953\_AUDIO\_DATA\_FORMAT@1 **DRV\_AK4953\_AUDIO\_DATA\_FORMAT**;

## *DRV\_AK4953\_BUFFER\_EVENT Type*

Identifies the possible events that can result from a buffer add request.

**Description**

AK4953 Driver Events

This enumeration identifies the possible events that can result from a buffer add request caused by the client calling either the DRV\_AK4953\_BufferAddWrite\(\) or the DRV\_AK4953\_BufferAddRead\(\) function.

**Remarks**

One of these values is passed in the "event" parameter of the event handling callback function that the client registered with the driver by calling the DRV\_AK4953\_BufferEventHandlerSet function when a buffer transfer request is completed.

**C**

**typedef** **enum** DRV\_AK4953\_BUFFER\_EVENT@1 **DRV\_AK4953\_BUFFER\_EVENT**;

## *DRV\_AK4953\_BUFFER\_EVENT\_HANDLER Type*

Pointer to a AK4953 Driver Buffer Event handler function

**Description**

AK4953 Driver Buffer Event Handler Function

This data type defines the required function signature for the AK4953 driver buffer event handling callback function. A client must register a pointer to a buffer event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive buffer related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|event|Identifies the type of event|
|bufferHandle|Handle identifying the buffer to which the event relates|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

If the event is DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE, this means that the data was transferred successfully.

If the event is DRV\_AK4953\_BUFFER\_EVENT\_ERROR, this means that the data was not transferred successfully. The bufferHandle parameter contains the buffer handle of the buffer that failed. The DRV\_AK4953\_BufferProcessedSizeGet\(\) function can be called to find out how many bytes were processed.

The bufferHandle parameter contains the buffer handle of the buffer that associated with the event.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_AK4953\_BufferEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The buffer handle in bufferHandle expires after this event handler exits. In that the buffer object that was allocated is deallocated by the driver after the event handler exits.

The event handler function executes in the data driver\(i2S\) peripheral's interrupt context when the driver is configured for interrupt mode operation. It is recommended of the application to not perform process intensive or blocking operations with in this function.

DRV\_AK4953\_BufferAddWrite function can be called in the event handler to add a buffer to the driver queue.

**Example**

**void** APP\_MyBufferEventHandler\( DRV\_AK4953\_BUFFER\_EVENT event, DRV\_AK4953\_BUFFER\_HANDLE bufferHandle, uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context; **switch**\(event\) \{ **case** DRV\_AK4953\_BUFFER\_EVENT\_COMPLETE: *// Handle the completed buffer.*

**break**; **case** DRV\_AK4953\_BUFFER\_EVENT\_ERROR:

**default**: *// Handle error.*

**break**; \}

\}

**C**

**typedef** **void** \(\* **DRV\_AK4953\_BUFFER\_EVENT\_HANDLER**\)\(DRV\_AK4953\_BUFFER\_EVENT event, DRV\_AK4953\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\);

## *DRV\_AK4953\_BUFFER\_HANDLE Type*

Handle identifying a write buffer passed to the driver.

**Description**

AK4953 Driver Buffer Handle

A buffer handle value is returned by a call to the DRV\_AK4953\_BufferAddWrite\(\) or DRV\_AK4953\_BufferAddRead\(\) function. This handle is associated with the buffer passed into the function and it allows the application to track the completion of the data from \(or into\) that buffer.

The buffer handle value returned from the "buffer add" function is returned back to the client by the "event handler callback" function registered with the driver.

The buffer handle assigned to a client request expires when the client has been notified of the completion of the buffer transfer \(after event handler function that notifies the client returns\) or after the buffer has been retired by the driver if no event handler callback was set.

**Remarks**

None

**C**

**typedef** uintptr\_t **DRV\_AK4953\_BUFFER\_HANDLE**;

## *DRV\_AK4953\_CHANNEL Type*

Identifies Left/Right Audio channel

**Description**

AK4953 Audio Channel

This enumeration identifies Left/Right Audio channel

**Remarks**

None.

**C**

**typedef** **enum** DRV\_AK4953\_CHANNEL@1 **DRV\_AK4953\_CHANNEL**;

## *DRV\_AK4953\_COMMAND\_EVENT\_HANDLER Type*

Pointer to a AK4953 Driver Command Event Handler Function

**Description**

AK4953 Driver Command Event Handler Function

This data type defines the required function signature for the AK4953 driver command event handling callback function.

A command is a control instruction to the AK4953 Codec. Example Mute ON/OFF, Zero Detect Enable/Disable etc.

A client must register a pointer to a command event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive command related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

The occurrence of this call back means that the last control command was transferred successfully.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_AK4953\_CommandEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The event handler function executes in the control data driver interrupt context. It is recommended of the application to not perform process intensive or blocking operations with in this function.

**Example**

**void** APP\_AK4953CommandEventHandler\( uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context;

*// Last Submitted command is completed.*

*// Perform further processing here*

\}

**C**

**typedef** **void** \(\* **DRV\_AK4953\_COMMAND\_EVENT\_HANDLER**\)\(uintptr\_t contextHandle\);

## *DRV\_AK4953\_DIGITAL\_BLOCK\_CONTROL Type*

Identifies Bass-Boost Control function

**Description**

AK4953 Bass-Boost Control

This enumeration identifies the settings for Bass-Boost Control function.

**Remarks**

None.

**C**

**typedef** **enum** DRV\_AK4953\_DIGITAL\_BLOCK\_CONTROL@1 **DRV\_AK4953\_DIGITAL\_BLOCK\_CONTROL**;

### DRV\_AK4953\_INIT Type

Defines the data required to initialize or reinitialize the AK4953 driver

**Description**

AK4953 Driver Initialization Data

This data type defines the data required to initialize or reinitialize the AK4953 Codec driver.

**Remarks**

None.

**C**

**typedef** **struct** DRV\_AK4953\_INIT@1 **DRV\_AK4953\_INIT**;

## *DRV\_AK4953\_INT\_EXT\_MIC Type*

Identifies the Mic input source.

**Description**

AK4953 Mic Internal / External Input

This enumeration identifies the Mic input source.

**C**

**typedef** **enum** DRV\_AK4953\_INT\_EXT\_MIC@1 **DRV\_AK4953\_INT\_EXT\_MIC**;

### DRV\_AK4953\_MIC Type

This is type DRV\_AK4953\_MIC.

**C**

**typedef** **enum** DRV\_AK4953\_MIC@1 **DRV\_AK4953\_MIC**;

## *DRV\_AK4953\_MONO\_STEREO\_MIC Type*

Identifies the Mic input as Mono / Stereo.

**Description**

AK4953 Mic Mono / Stereo Input

This enumeration identifies the Mic input as Mono / Stereo.

**C**

**typedef** **enum** DRV\_AK4953\_MONO\_STEREO\_MIC@1 **DRV\_AK4953\_MONO\_STEREO\_MIC**;

***Files***

**AK4954 CODEC Driver Library Help**

This topic describes the AK4954 Codec Driver Library.

## *Introduction*

This topic describes the basic architecture of the AK4954 Codec Driver Library and provides information and examples on its use.

**Description**

### Interface Header File: drv\_AK4954.h

The interface to the AK4954 Codec Driver library is defined in the audio/driver/codec/AK4954/drv\_AK4954.h header file.

Any C language source \(.c\) file that uses the AK4954 Codec Driver library should include this header.

**Library Source Files:**

The AK4954 Codec Driver library source files are provided in the audio/driver/codec/AK4954/src directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features and to **Building the Library** for instructions on how to build the library.

**Example Applications:**

This library is used by the following applications:

-   audio/apps/audio\_tone

-   audio/apps/microphone\_loopback


***Using the Library***

## Abstration Model

This library provides a low-level abstraction of the AK4954 Codec Driver Library on the Microchip family microcontrollers with a convenient C language interface. This topic describes how that abstraction is modeled in software and introduces the library's interface.

**Description**

The abstraction model shown in the following diagram depicts how the AK4954 Codec Driver is positioned in the MPLAB Harmony framework. The AK4954 Codec Driver uses the I2C and I2S drivers for control and audio data transfers to the AK4954 module. **AK4954 Driver Abstraction Model** ![](GUID-A1402B4E-C779-4E72-B06A-3076BD3B42F4-low.png)

## Library Overview

Refer to the Driver Library Overview section for information on how the driver operates in a system.

The AK4954 Codec Driver Library provides an API interface to transfer control commands and digital audio data to the serially interfaced AK4954 Codec module. The library interface routines are divided into various sub-sections, which address one of the blocks or the overall operation of the AK4954 Codec Driver Library.

|**Library Interface Section**|**Description**|
|-----------------------------|---------------|
|System Functions|Provides system module interfaces, device initialization, deinitialization, reinitialization, tasks and status functions.|
|Client Setup Functions|Provides open and close functions.|
|Data Transfer Functions|Provides data transfer functions, such as Buffer Read and Write.|
|Settings Functions|Provides driver specific functions for settings, such as volume control and sampling rate.|
|Other Functions|Miscellaneous functions, such as getting the driver’s version number and syncing to the LRCLK signal.|
|Data Types and Constants|These data types and constants are required while interacting and setting up the AK4954 Codec Driver Library.|

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** All functions and constants in this section are named with the format DRV\_ AK4954\_xxx, where 'xxx' is a function name or constant. These names are redefined in the appropriate configuration’s configuration.h file to the format DRV\_CODEC\_xxx using \#defines so that code in the application that references the library can be written as generically as possible \(e.g., by writing DRV\_CODEC\_Open instead of DRV\_ AK4954\_Open etc.\). This allows the codec type to be changed in the MHC without having to modify the application’s source code.

## How the Library Works

How the Library Works

The library provides interfaces to support:

-   System Functionality

-   Client Functionality


### Setup \(Initialization\)

This topic describes system initialization, implementations, and includes a system access code example.

**Description**

## System Initialization

The system performs the initialization of the device driver with settings that affect only the instance of the device that is being initialized. During system initialization in the system\_init.c file, each instance of the AK4954 module would be initialized with the following configuration settings \(either passed dynamically at run time using DRV\_AK4954\_INIT or by using Initialization Overrides\) that are supported by the specific AK4954 device hardware:

-   Device requested power state: one of the System Module Power States. For specific details please refer to Data Types and Constants in the Library Interface section.

-   I2C driver module index. The module index should be same as the one used in initializing the I2C Driver

-   I2S driver module index. The module index should be same as the one used in initializing the I2S Driver

-   Sampling rate

-   Volume

-   Audio data format. The audio data format should match with the audio data format settings done in I2S driver initialization

-   Determines whether or not the microphone input is enabled


The DRV\_AK4954\_Initialize API returns an object handle of the type SYS\_MODULE\_OBJ. The object handle returned by the

Initialize interface would be used by the other system interfaces such as DRV\_ AK4954\_Deinitialize, DRV\_ AK4954\_Status and

DRV\_I2S\_Tasks

### Client Access

This topic describes driver initialization and provides a code example.

**Description**

For the application to start using an instance of the module, it must call the DRV\_AK4954\_Open function. The

DRV\_AK4954\_Open function provides a driver handle to the AK4954 Codec Driver instance for operations. If the driver is deinitialized using the function DRV\_AK4954\_Deinitialize, the application must call the DRV\_AK4954\_Open function again to set up the instance of the driver.

For the various options available for IO\_INTENT, please refer to Data Types and Constants in the Library Interface section.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is necessary to check the status of driver initialization before opening a driver instance. The status of the AK4954 Codec Driver can be known by calling DRV\_ AK4954\_Status.

**Example:**

DRV\_HANDLE handle; SYS\_STATUS ak4954Status;

ak4954Status Status = DRV\_AK4954\_Status\(sysObjects.ak4954Status DevObject\); if \(SYS\_STATUS\_READY == ak4954Status\) \{ // The driver can now be opened. appData.ak4954Client.handle = DRV\_AK4954\_Open

\( DRV\_AK4954\_INDEX\_0, DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_EXCLUSIVE\); if\(appData.ak4954Client.handle != DRV\_HANDLE\_INVALID\) \{

appData.state = APP\_STATE\_AK4954\_SET\_BUFFER\_HANDLER; \} else \{ SYS\_DEBUG\(0, "Find out what's wrong \\r\\n"\);

\} \} else \{ /\* AK4954 Driver Is not ready \*/

\}

### Client Operations

This topic provides information on client operations.

**Description**

Client operations provide the API interface for control command and audio data transfer to the AK4954 Codec.

The following AK4954 Codec specific control command functions are provided:

-   DRV\_AK4954\_SamplingRateSet

-   DRV\_AK4954\_SamplingRateGet

-   DRV\_AK4954\_VolumeSet

-   DRV\_AK4954\_VolumeGet

-   DRV\_AK4954\_MuteOn

-   DRV\_AK4954\_MuteOff


These functions schedule a non-blocking control command transfer operation. These functions submit the control command request to the AK4954 Codec. These functions submit the control command request to I2C Driver transmit queue, the request is processed immediately if it is the first request, or processed when the previous request is complete.

DRV\_AK4954\_BufferAddWrite, DRV\_AK4954\_BufferAddRead, and DRV\_AK4954\_BufferAddWriteRead are buffered data operation functions. These functions schedule non-blocking audio data transfer operations. These functions add the request to I2S Driver transmit or receive buffer queue depends on the request type, and are executed immediately if it is the first buffer, or executed later when the previous buffer is complete. The driver notifies the client with

DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE, DRV\_AK4954\_BUFFER\_EVENT\_ERROR, or DRV\_AK4954\_BUFFER\_EVENT\_ABORT events.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is not necessary to close and reopen the client between multiple transfers.

### Configuring the Library

The configuration of the I2S Driver Library is based on the file configurations.h, as generated by the MHC.

This header file contains the configuration selection for the I2S Driver Library. Based on the selections made, the I2S Driver Library may support the selected features. These configuration settings will apply to all instances of the I2S Driver Library.

This header can be placed anywhere; however, the path of this header needs to be present in the include search path for a successful build. Refer to the Applications Help section for more details.

### System Configuration

**Macros**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_MACRO|Specifies the audio data format for the codec.|
|-|DRV\_AK4954\_AUDIO\_SAMPLING\_RATE|Specifies the initial baud rate for the codec.|
|-|DRV\_AK4954\_CLIENTS\_NUMBER|Sets up the maximum number of clients that can be connected to any hardware instance.|
|-|DRV\_AK4954\_ENABLE\_MIC\_BIAS|Specifies whether to enable the microphone bias.|
|-|DRV\_AK4954\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2C interface.|
|-|DRV\_AK4954\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2S interface.|
|-|DRV\_AK4954\_INSTANCES\_NUMBER|Sets up the maximum number of hardware instances that can be supported|
|-|DRV\_AK4954\_MASTER\_MODE|Specifies if codec is in Master or Slave mode.|
|-|DRV\_AK4954\_MIC\_GAIN|Specifies the gain of the microphone \(external or line input only\)|
|-|DRV\_AK4954\_VOLUME|Specifies the initial volume level.|
|-|DRV\_AK4954\_WHICH\_MIC\_INPUT|Specifies whether to enable the microphone input.|

**Description**

## *DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_MACRO Macro*

Specifies the audio data format for the codec.

**Description**

AK4954 Audio Data Format

Sets up the length of each sample plus the format \(I2S or left-justified\) for the audio.

Valid choices are: "DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_24BIT\_MSB\_SDTO\_24BIT\_LSB\_SDTI"

"DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_24BIT\_MSB\_SDTO\_16BIT\_LSB\_SDTI"

"DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_24BIT\_MSB\_SDTO\_24BIT\_MSB\_SDTI" "DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S\_16BIT\_24BIT"

"DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_32BIT\_MSB\_SDTO\_32BIT\_MSB\_SDTI"

"DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S\_32BIT" where SDTO is input line \(ADC\) and STDI is output line \(DAC\)

**Remarks**

If 24-bit audio is needed, it should be sent, left-justified, in a 32-bit format.

**C**

**\#define** **DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_MACRO**

## *DRV\_AK4954\_AUDIO\_SAMPLING\_RATE Macro*

Specifies the initial baud rate for the codec.

**Description**

AK4954 Baud Rate

Sets the initial baud rate \(sampling rate\) for the codec. Typical values are 8000, 16000, 44100, 48000, 88200 and 96000.

**C**

**\#define** **DRV\_AK4954\_AUDIO\_SAMPLING\_RATE**

## *DRV\_AK4954\_CLIENTS\_NUMBER Macro*

Sets up the maximum number of clients that can be connected to any hardware instance.

**Description**

AK4954 Client Count Configuration

Sets up the maximum number of clients that can be connected to any hardware instance. Typically only one client could be connected to one hardware instance. This value represents the total number of clients to be supported across all hardware instances.

**C**

**\#define** **DRV\_AK4954\_CLIENTS\_NUMBER**

## *DRV\_AK4954\_ENABLE\_MIC\_BIAS Macro*

Specifies whether to enable the microphone bias.

**Description**

AK4954 Microphone Enable

Indicates whether the bias voltage needed for electret microphones should be enabled.

**C**

**\#define** **DRV\_AK4954\_ENABLE\_MIC\_BIAS**

## *DRV\_AK4954\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2C interface.

**Description**

AK4954 I2C instance number

Specifies the instance number of the I2C interface being used by the MCU to send commands and receive status to and from the AK4954. enabled.

**C**

**\#define** **DRV\_AK4954\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_AK4954\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2S interface.

**Description**

AK4954 I2S instance number

Specifies the instance number of the I2S interface being used by the MCU to send and receive audio data to and from the AK4954. enabled.

**C**

**\#define** **DRV\_AK4954\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_AK4954\_INSTANCES\_NUMBER Macro*

Sets up the maximum number of hardware instances that can be supported

**Description**

AK4954 driver objects configuration

Sets up the maximum number of hardware instances that can be supported. It is recommended that this number be set exactly equal to the number of AK4954 Codec modules that are needed by an application, namely one.

**C**

**\#define** **DRV\_AK4954\_INSTANCES\_NUMBER**

### DRV\_AK4954\_MASTER\_MODE Macro

Specifies if codec is in Master or Slave mode.

**Description**

AK4954 Codec Master/Slave Mode

Indicates whether the codec is to be operating in a Master mode \(generating word and bit clock as outputs\) or Slave mode receiving word and bit clock as inputs\).

**C**

**\#define** **DRV\_AK4954\_MASTER\_MODE**

### DRV\_AK4954\_MIC\_GAIN Macro

Specifies the gain of the microphone \(external or line input only\)

**Description**

AK4954 Microphone Gain

Specifies the gain of the microphone \(external or line input only\), on a scale of 0-31

**C**

**\#define** **DRV\_AK4954\_MIC\_GAIN**

### DRV\_AK4954\_VOLUME Macro

Specifies the initial volume level.

**Description**

AK4954 Volume

Sets the initial volume level, in the range 0-255.

**Remarks**

The value is mapped to an internal AK4954 volume level in the range 0-192 using a logarithmic table so the input scale appears linear \(128 is half volume\).

**C**

**\#define** **DRV\_AK4954\_VOLUME**

## *DRV\_AK4954\_WHICH\_MIC\_INPUT Macro*

Specifies whether to enable the microphone input.

**Description**

AK4954 Which Microphone

Indicates which microphone \(or line input\) is chosen

Valid choices are: "MIC1" \(Internal Mic on board\) "MIC2" \(External Mic Input\) "MIC3" \(Line Input\)

**C**

**\#define** **DRV\_AK4954\_WHICH\_MIC\_INPUT**

### Configuring MHC

Provides examples on how to configure the MPLAB Harmony Configurator \(MHC\) for a specific driver.

**Description**

When building a new application, start by creating a 32-bit MPLAB Harmony 3 project in MPLAB X IDE by selecting *File \> New Project*. Chose the Configuration name the based on the BSP, and select the appropriate processor \(such as ATSAME70Q21B\).

In the MHC, under Available Components select the appropriate BSP, such as SAM E70 Xplained Ultra. Under *Audio\>Templates*, double-click on a codec template such as AK4954. Answer Yes to all questions.

You should end up with a project graph that looks like this, after rearranging the boxes: ![](GUID-6E925773-504F-4F0F-B05D-449BCACE0494-low.png)

Click on the AK4954 Driver component \(not AK4954 Codec\) and the following menu will be displayed in the Configurations Options: ![](GUID-4AF50211-0514-4851-A9DE-E0E042250333-low.png)

-   **I2C Driver Used** will display the driver instance used for the I2C interface.

-   **I2S Driver Used** will display the driver instance used for the I2S interface.

-   **Usage Mode** indicates whether the AK4954 is a Master \(supplies I2S clocks\) or a Slave \(MCU supplies I2S clocks\).

-   **Number of AK4954 Clients** indicates the maximum number of clients that can be connected to the AK4954 Driver.

-   **Sampling Rate** indicates the number of samples per second per channel, 8000 to 96,000.

-   **Volume** indicates the volume in a linear scale from 0-255.


#### • Audio Data Format is either

-   24-bit Left Justified \(ADC\), 24-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 16-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 24-bit Left-justified\(DAC\)

-   24/16-bit I2S

-   32-bit Left Justified \(ADC\), 32-bit Left-justified\(DAC\)

-   32-bit I2S


It must match the audio protocol and data length set up in either the SSC or I2S PLIB.

-   **Microphone/Line Input** selects which microphone or line input is selected, either:

-   Internal Mic \(mounted on the AK4954 daughterboard\)

-   External Mic Input

-   Line Input


If External Mic input or Line Input is selected, then the following option is provided:

#### • Ext Mic Gain in dB range 0\(min\) to 31\(max\)

If External Mic input is selected, then the following option is provided:

• **Enable Microphone Bias** should be checked if using an electret microphone.

You can also bring in the AK4954 Driver by itself, by double clicking AK4954 under Audio\_-\>Driver-\>Codec\_ in the Available Components list. You will then need to add any additional needed components manually and connect them together.

Note that the AK4954 requires the TCx Peripheral Library and TIME System Service in order to perform some of its internal timing sequences.

### Building the Library

This section lists the files that are available in the AK4954 Codec Driver Library.

**Description**

This section lists the files that are available in the src folder of the AK4954 Codec Driver. It lists which files need to be included in the build based on either a hardware feature present on the board or configuration option selected by the system.

The following three tables list and describe the header \(.h\) and source \(.c\) files that implement this library. The parent folder for these files is audio/driver/codec/AK4954.

### Interface File\(s\)

This table lists and describes the header files that must be included \(i.e., using \#include\) by any code that uses this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|drv\_ak4954.h|Header file that exports the driver API.|

### Required File\(s\)

![](GUID-B924D07A-0D74-453B-BCDE-8190A195C55D-low.png) ***All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use.***

This table lists and describes the source and header files that must *always* be included in the MPLAB X IDE project to build this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|/src/drv\_ak4954.c|This file contains implementation of the AK4954 Codec Driver.|

### Optional File\(s\)

This table lists and describes the source and header files that may optionally be included if required for the desired implementation.

|**Source File Name**|**Description**|
|--------------------|---------------|
|N/A|No optional files are available for this library.|

### Module Dependencies

The AK4954 Codec Driver Library depends on the following modules:

-   I2S Driver Library

-   I2C Driver Library


#### *Library Interface*

**Client Setup Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4954\_Open|Opens the specified AK4954 driver instance and returns a handle to it.|
||DRV\_AK4954\_Close|Closes an opened-instance of the AK4954 driver|
||DRV\_AK4954\_BufferEventHandlerSet|This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.|
||DRV\_AK4954\_CommandEventHandlerSet|This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.|

**Data Transfer Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4954\_BufferAddRead|Schedule a non-blocking driver read operation.|
||DRV\_AK4954\_BufferAddWrite|Schedule a non-blocking driver write operation.|
||DRV\_AK4954\_BufferAddWriteRead|Schedule a non-blocking driver write-read operation.|
||DRV\_AK4954\_ReadQueuePurge|Removes all buffer requests from the read queue.|
||DRV\_AK4954\_WriteQueuePurge|Removes all buffer requests from the write queue.|

**Data Types and Constants**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_AK4954\_AUDIO\_DATA\_FORMAT|Identifies the Serial Audio data interface format.|
|-|DRV\_AK4954\_BUFFER\_EVENT|Identifies the possible events that can result from a buffer add request.|
|-|DRV\_AK4954\_BUFFER\_EVENT\_HANDLER|Pointer to a AK4954 Driver Buffer Event handler function|
|-|DRV\_AK4954\_BUFFER\_HANDLE|Handle identifying a write buffer passed to the driver.|
|-|DRV\_AK4954\_CHANNEL|Identifies Left/Right Audio channel|
|-|DRV\_AK4954\_COMMAND\_EVENT\_HANDLER|Pointer to a AK4954 Driver Command Event Handler Function|
|-|DRV\_AK4954\_DIGITAL\_BLOCK\_CONTROL|Identifies Bass-Boost Control function|
|-|DRV\_AK4954\_INIT|Defines the data required to initialize or reinitialize the AK4954 driver|
|-|DRV\_AK4954\_INT\_EXT\_MIC|Identifies the Mic input source.|
|-|DRV\_AK4954\_MIC|This is type DRV\_AK4954\_MIC.|
|-|DRV\_AK4954\_MONO\_STEREO\_MIC|Identifies the Mic input as Mono / Stereo.|
|-|SAMPLE\_LENGTH|in bits|
|-|DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S|for compatability with old code|
|-|DRV\_AK4954\_BUFFER\_HANDLE\_INVALID|Definition of an invalid buffer handle.|
|-|DRV\_AK4954\_COUNT|Number of valid AK4954 driver indices|
|-|DRV\_AK4954\_INDEX\_0|AK4954 driver index definitions|
|-|DRV\_AK4954\_INDEX\_1|This is macro DRV\_AK4954\_INDEX\_1.|
|-|DRV\_AK4954\_INDEX\_2|This is macro DRV\_AK4954\_INDEX\_2.|
|-|DRV\_AK4954\_INDEX\_3|This is macro DRV\_AK4954\_INDEX\_3.|
|-|DRV\_AK4954\_INDEX\_4|This is macro DRV\_AK4954\_INDEX\_4.|
|-|DRV\_AK4954\_INDEX\_5|This is macro DRV\_AK4954\_INDEX\_5.|

**Other Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4954\_LRCLK\_Sync|Synchronize to the start of the I2S LRCLK \(left/right clock\) signal|
||DRV\_AK4954\_GetI2SDriver|Get the handle to the I2S driver for this codec instance.|
||DRV\_AK4954\_VersionStrGet|This function returns the version of AK4954 driver in string format.|
||DRV\_AK4954\_VersionGet|This function returns the version of AK4954 driver.|

**Settings Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4954\_MicGainGet|This function gets the microphone gain for the AK4954 Codec.|
||DRV\_AK4954\_MicGainSet|This function sets the microphone gain for the AK4954 CODEC.|
||DRV\_AK4954\_MicMuteOff|Umutes th AK4954's microphone input.|
||DRV\_AK4954\_MicMuteOn|Mutes the AK4954's microphone input|
||DRV\_AK4954\_MicSet|This function sets up the codec for the internal or the AK4954 Mic1 or Mic2 input.|
||DRV\_AK4954\_MonoStereoMicSet|This function sets up the codec for the Mono or Stereo microphone mode.|
||DRV\_AK4954\_MuteOff|This function disables AK4954 output for soft mute.|
||DRV\_AK4954\_MuteOn|This function allows AK4954 output for soft mute on.|
||DRV\_AK4954\_IntExtMicSet|This function sets up the codec for the X32 DB internal or the external microphone use.|
||DRV\_AK4954\_SamplingRateGet|This function gets the sampling rate set on the DAC AK4954.|
||DRV\_AK4954\_SamplingRateSet|This function sets the sampling rate of the media stream.|
||DRV\_AK4954\_VolumeGet|This function gets the volume for AK4954 Codec.|
||DRV\_AK4954\_VolumeSet|This function sets the volume for AK4954 Codec.|

**System Interaction Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_AK4954\_Initialize|Initializes hardware and data for the instance of the AK4954 Codec module.|
||DRV\_AK4954\_EnableInitialization|Enable delayed initialization of the driver.|
||DRV\_AK4954\_IsInitializationDelayed|Checks if delayed initialization of the driver has been requested.|
||DRV\_AK4954\_Deinitialize|Deinitializes the specified instance of the AK4954 driver module.|
||DRV\_AK4954\_Status|Gets the current status of the AK4954 driver module.|
||DRV\_AK4954\_Tasks|Maintains the driver's control and data interface state machine.|

**Description**

#### System Interaction Functions

##### DRV\_AK4954\_Initialize Function

SYS\_MODULE\_OBJ DRV\_AK4954\_Initialize

\(

const SYS\_MODULE\_INDEX drvIndex, const SYS\_MODULE\_INIT \*const init

\);

**Summary**

Initializes hardware and data for the instance of the AK4954 Codec module.

**Description**

This routine initializes the AK4954 driver instance for the specified driver index, making it ready for clients to open and use it. The initialization data is specified by the init parameter. The initialization may fail if the number of driver objects allocated are insufficient or if the specified driver instance is already initialized.

**Preconditions**

DRV\_I2S\_Initialize must be called before calling this function to initialize the data interface of this Codec driver. DRV\_I2C\_Initialize must be called if SPI driver is used for handling the control interface of this Codec driver.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the driver instance to be initialized|
|init|Pointer to the data structure containing any data necessary to initialize the hardware. This pointer may be null if no data is required and default initialization is to be used.|

**Returns**

If successful, returns a valid handle to a driver instance object. Otherwise, it returns SYS\_MODULE\_OBJ\_INVALID.

**Remarks**

This routine must be called before any other AK4954 routine is called.

This routine should only be called once during system initialization unless DRV\_AK4954\_Deinitialize is called to deinitialize the driver instance. This routine will NEVER block for hardware access.

**Example**

DRV\_AK4954\_INIT init;

SYS\_MODULE\_OBJ objectHandle; init-\>inUse = **true**; init-\>status = SYS\_STATUS\_BUSY; init-\>numClients = 0; init-\>i2sDriverModuleIndex = ak4954Init-\>i2sDriverModuleIndex; init-\>i2cDriverModuleIndex = ak4954Init-\>i2cDriverModuleIndex; init-\>samplingRate = DRV\_AK4954\_AUDIO\_SAMPLING\_RATE; init-\>audioDataFormat = DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_MACRO; **for**\(index=0; index < DRV\_AK4954\_NUMBER\_OF\_CHANNELS; index++\) \{

init-\>volume = ak4954Init-\>volume; \} init-\>isInInterruptContext = **false**; init-\>commandCompleteCallback = \(DRV\_AK4954\_COMMAND\_EVENT\_HANDLER\)0; init-\>commandContextData = 0; init-\>mclk\_multiplier = DRV\_AK4954\_MCLK\_SAMPLE\_FREQ\_MULTPLIER; objectHandle = DRV\_AK4954\_Initialize\(DRV\_AK4954\_0, \(SYS\_MODULE\_INIT\*\)init\); **if** \(SYS\_MODULE\_OBJ\_INVALID == objectHandle\) \{

*// Handle error*

\}

**C**

SYS\_MODULE\_OBJ **DRV\_AK4954\_Initialize**\(**const** SYS\_MODULE\_INDEX **drvIndex**, **const** SYS\_MODULE\_INIT \* **const** **init**\);

##### DRV\_AK4954\_EnableInitialization Function

void DRV\_AK4954\_EnableInitialization\(SYS\_MODULE\_OBJ object\);

**Summary**

Enable delayed initialization of the driver.

**Description**

If the AK4954 codec is sharing a RESET line with another peripheral, such as a Bluetooth module with its own driver, then the codec driver initialization has to be delayed until after the Bluetooth module has toggled its RESET pin. Once this has been accomplished, this function should be called to kick-start the codec driver initialization.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4954\_Initialize\)|

**Returns**

None.

**Remarks**

This is not needed for audio-only applications without a Bluetooth module.

**C**

**void** **DRV\_AK4954\_EnableInitialization**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_AK4954\_IsInitializationDelayed Function

bool DRV\_AK4954\_IsInitializationDelayed\(SYS\_MODULE\_OBJ object\);

**Summary**

Checks if delayed initialization of the driver has been requested.

**Description**

If the AK4954 codec is sharing a RESET line with another peripheral, such as a Bluetooth module with its own driver, then the codec driver initialization has to be delayed until after the Bluetooth module has toggled its RESET pin. This function returns true if that option has been selected in MHC in the checkbox: "Delay driver initialization \(due to shared RESET pin\)"

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4954\_Initialize\)|

**Returns**

true if the delayed initialization option has been enabled

**Remarks**

This is not needed for audio-only applications without a Bluetooth module.

**C**

**bool** **DRV\_AK4954\_IsInitializationDelayed**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_AK4954\_Deinitialize Function

void DRV\_AK4954\_Deinitialize\( SYS\_MODULE\_OBJ object\)

**Summary**

Deinitializes the specified instance of the AK4954 driver module.

**Description**

Deinitializes the specified instance of the AK4954 driver module, disabling its operation \(and any hardware\). Invalidates all the internal data.

**Preconditions**

Function DRV\_AK4954\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_AK4954\_Initialize routine|

**Returns**

None.

**Remarks**

Once the Initialize operation has been called, the De-initialize operation must be called before the Initialize operation can be called again. This routine will NEVER block waiting for hardware.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4954\_Initialize*

SYS\_STATUS status;

DRV\_AK4954\_Deinitialize\(object\);

status = DRV\_AK4954\_Status\(object\); **if** \(SYS\_MODULE\_DEINITIALIZED != status\) \{

*// Check again later if you need to know* *// when the driver is deinitialized.*

\}

**C**

**void** **DRV\_AK4954\_Deinitialize**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_AK4954\_Status Function

SYS\_STATUS DRV\_AK4954\_Status\( SYS\_MODULE\_OBJ object\)

**Summary**

Gets the current status of the AK4954 driver module.

**Description**

This routine provides the current status of the AK4954 driver module.

**Preconditions**

Function DRV\_AK4954\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_AK4954\_Initialize routine|

**Returns**

SYS\_STATUS\_DEINITIALIZED - Indicates that the driver has been deinitialized

SYS\_STATUS\_READY - Indicates that any previous module operation for the specified module has completed

SYS\_STATUS\_BUSY - Indicates that a previous module operation for the specified module has not yet completed

SYS\_STATUS\_ERROR - Indicates that the specified module is in an error state

**Remarks**

A driver can opened only when its status is SYS\_STATUS\_READY.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4954\_Initialize*

SYS\_STATUS AK4954Status;

AK4954Status = DRV\_AK4954\_Status\(object\); **if** \(SYS\_STATUS\_READY == AK4954Status\) \{

*// This means the driver can be opened using the* *// DRV\_AK4954\_Open\(\) function.*

\}

**C**

SYS\_STATUS **DRV\_AK4954\_Status**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_AK4954\_Tasks Function

void DRV\_AK4954\_Tasks\(SYS\_MODULE\_OBJ object\);

**Summary**

Maintains the driver's control and data interface state machine.

**Description**

This routine is used to maintain the driver's internal control and data interface state machine and implement its control and data interface implementations. This function should be called from the SYS\_Tasks\(\) function.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_AK4954\_Initialize\)|

**Returns**

None.

**Remarks**

This routine is normally not called directly by an application. It is called by the system's Tasks routine \(SYS\_Tasks\).

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_AK4954\_Initialize* **while** \(**true**\) \{

DRV\_AK4954\_Tasks \(object\);

*// Do other tasks*

\}

**C**

**void** **DRV\_AK4954\_Tasks**\(SYS\_MODULE\_OBJ **object**\);

#### Client Setup Functions

##### DRV\_AK4954\_Open Function

DRV\_HANDLE DRV\_AK4954\_Open

\(

const SYS\_MODULE\_INDEX drvIndex, const DRV\_IO\_INTENT ioIntent

\)

**Summary**

Opens the specified AK4954 driver instance and returns a handle to it.

**Description**

This routine opens the specified AK4954 driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver. The ioIntent parameter defines how the client interacts with this driver instance.

The DRV\_IO\_INTENT\_BLOCKING and DRV\_IO\_INTENT\_NONBLOCKING ioIntent options are not relevant to this driver. All the data transfer functions of this driver are non blocking.

AK4954 can be opened with DRV\_IO\_INTENT\_WRITE, or DRV\_IO\_INTENT\_READ or DRV\_IO\_INTENT\_WRITEREAD io\_intent option. This decides whether the driver is used for headphone output, or microphone input or both modes simultaneously.

Specifying a DRV\_IO\_INTENT\_EXCLUSIVE will cause the driver to provide exclusive access to this client. The driver cannot be opened by any other client.

**Preconditions**

Function DRV\_AK4954\_Initialize must have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the object instance to be opened|
|ioIntent|Zero or more of the values from the enumeration DRV\_IO\_INTENT "ORed" together to indicate the intended use of the driver. See function description for details.|

**Returns**

If successful, the routine returns a valid open-instance handle \(a number identifying both the caller and the module instance\).

If an error occurs, the return value is DRV\_HANDLE\_INVALID. Error can occur

-   if the number of client objects allocated via DRV\_AK4954\_CLIENTS\_NUMBER is insufficient.

-   if the client is trying to open the driver but driver has been opened exclusively by another client.

-   if the driver hardware instance being opened is not initialized or is invalid.

-   if the ioIntent options passed are not relevant to this driver.


**Remarks**

The handle returned is valid until the DRV\_AK4954\_Close routine is called. This routine will NEVER block waiting for hardware.If the requested intent flags are not supported, the routine will return DRV\_HANDLE\_INVALID. This function is thread safe in a RTOS application. It should not be called in an ISR.

**Example**

DRV\_HANDLE handle; handle = DRV\_AK4954\_Open\(DRV\_AK4954\_INDEX\_0, DRV\_IO\_INTENT\_WRITEREAD \| DRV\_IO\_INTENT\_EXCLUSIVE\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver*

*// May be the driver is not initialized or the initialization* *// is not complete.*

\}

**C**

DRV\_HANDLE **DRV\_AK4954\_Open**\(**const** SYS\_MODULE\_INDEX **iDriver**, **const** DRV\_IO\_INTENT **ioIntent**\);

##### DRV\_AK4954\_Close Function

void DRV\_AK4954\_Close\( DRV\_Handle handle \)

**Summary**

Closes an opened-instance of the AK4954 driver

**Description**

This routine closes an opened-instance of the AK4954 driver, invalidating the handle. Any buffers in the driver queue that were submitted by this client will be removed. After calling this routine, the handle passed in "handle" must not be used with any of the remaining driver routines. A new handle must be obtained by calling DRV\_AK4954\_Open before the caller may use the driver again

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

Usually there is no need for the driver client to verify that the Close operation has completed. The driver will abort any ongoing operations when this routine is called.

**Example**

DRV\_HANDLE handle; *// Returned from DRV\_AK4954\_Open*

DRV\_AK4954\_Close\(handle\);

**C**

**void** **DRV\_AK4954\_Close**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_BufferEventHandlerSet Function

void DRV\_AK4954\_BufferEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_AK4954\_BUFFER\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.

**Description**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished. When a client calls DRV\_AK4954\_BufferAddRead function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "buffer add" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the queued buffer transfer has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4954\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4954\_BufferEventHandlerSet\(myAK4954Handle,

APP\_AK4954BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4954\_BufferAddRead\(myAK4954handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_AK4954\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4954BufferEventHandler\(DRV\_AK4954\_BUFFER\_EVENT event, DRV\_AK4954\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_AK4954\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4954\_BufferEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_AK4954\_BUFFER\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

##### DRV\_AK4954\_CommandEventHandlerSet Function

void DRV\_AK4954\_CommandEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_AK4954\_COMMAND\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

**Description**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

When a client calls DRV\_AK4954\_BufferAddWrite function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "AK4954 CODEC Specific Client Routines" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the command has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4954\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4954\_CommandEventHandlerSet\(myAK4954Handle,

APP\_AK4954CommandEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4954\_DeEmphasisFilterSet\(myAK4954Handle, DRV\_AK4954\_DEEMPHASIS\_FILTER\_44\_1KHZ\)

*// Event is received when // the buffer is processed.*

**void** APP\_AK4954CommandEventHandler\(uintptr\_t contextHandle\) \{ *// contextHandle points to myAppObj.*

**switch**\(event\) \{ *// Last Submitted command is completed.* *// Perform further processing here*

\}

\}

**C**

**void** **DRV\_AK4954\_CommandEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_AK4954\_COMMAND\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

#### Data Transfer Functions

##### DRV\_AK4954\_BufferAddRead Function

void DRV\_AK4954\_BufferAddRead

\(

const DRV\_HANDLE handle,

DRV\_AK4954\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver read operation.

**Description**

This function schedules a non-blocking read operation. The function returns with a valid buffer handle in the bufferHandle argument if the read request was scheduled successfully. The function adds the request to the hardware instance receive queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4954\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4954\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 device instance and the DRV\_AK4954\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle. DRV\_IO\_INTENT\_READ must have been specified in the DRV\_AK4954\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4954 instance as return by the DRV\_AK4954\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4954\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4954 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4954 driver instance. It should not otherwise be called directly in an ISR.

**C**

**void** **DRV\_AK4954\_BufferAddRead**\(**const** DRV\_HANDLE **handle**, DRV\_AK4954\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_AK4954\_BufferAddWrite Function

void DRV\_AK4954\_BufferAddWrite

\(

const DRV\_HANDLE handle,

DRV\_AK4954\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write operation.

**Description**

This function schedules a non-blocking write operation. The function returns with a valid buffer handle in the bufferHandle argument if the write request was scheduled successfully. The function adds the request to the hardware instance transmit queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4954\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4954\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 device instance and the DRV\_AK4954\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_WRITE must have been specified in the DRV\_AK4954\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4954 instance as return by the DRV\_AK4954\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4954\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4954 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4954 driver instance. It should not otherwise be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_AK4954\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4954\_BufferEventHandlerSet\(myAK4954Handle,

APP\_AK4954BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4954\_BufferAddWrite\(myAK4954handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_AK4954\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4954BufferEventHandler\(DRV\_AK4954\_BUFFER\_EVENT event, DRV\_AK4954\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_AK4954\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4954\_BufferAddWrite**\(**const** DRV\_HANDLE **handle**, DRV\_AK4954\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_AK4954\_BufferAddWriteRead Function

void DRV\_AK4954\_BufferAddWriteRead

\(

const DRV\_HANDLE handle,

DRV\_AK4954\_BUFFER\_HANDLE \*bufferHandle,

void \*transmitBuffer, void \*receiveBuffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write-read operation.

**Description**

This function schedules a non-blocking write-read operation. The function returns with a valid buffer handle in the bufferHandle argument if the write-read request was scheduled successfully. The function adds the request to the hardware instance queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read only or write only

-   if the buffer size is 0

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_AK4954\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 device instance and the DRV\_AK4954\_Status must have returned SYS\_STATUS\_READY.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_READWRITE must have been specified in the DRV\_AK4954\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the AK4954 instance as returned by the DRV\_AK4954\_Open function|
|bufferHandle|Pointer to an argument that will contain the return buffer handle|
|transmitBuffer|The buffer where the transmit data will be stored|
|receiveBuffer|The buffer where the received data will be stored|
|size|Buffer size in bytes|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_AK4954\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the AK4954 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another AK4954 driver instance. It should not otherwise be called directly in an ISR.

This function is useful when there is valid read expected for every AK4954 write. The transmit and receive size must be same.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybufferTx; uint8\_t mybufferRx; DRV\_AK4954\_BUFFER\_HANDLE bufferHandle;

*// myak4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

*// Client registers an event handler with driver*

DRV\_AK4954\_BufferEventHandlerSet\(myak4954Handle,

APP\_AK4954BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_AK4954\_BufferAddWriteRead\(myak4954handle, &bufferHandle,

mybufferTx,mybufferRx,MY\_BUFFER\_SIZE\);

**if**\(DRV\_AK4954\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_AK4954BufferEventHandler\(DRV\_AK4954\_BUFFER\_EVENT event, DRV\_AK4954\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_AK4954\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_AK4954\_BufferAddWriteRead**\(**const** DRV\_HANDLE **handle**, DRV\_AK4954\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **transmitBuffer**, **void** \* **receiveBuffer**, size\_t **size**\);

##### DRV\_AK4954\_ReadQueuePurge Function

bool DRV\_AK4954\_ReadQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the read queue.

**Description**

This function removes all the buffer requests from the read queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_AK4954\_Open function.|

**Returns**

True - Read queue purge is successful. False - Read queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_AK4954\_Open function.*

*// Use DRV\_AK4954\_BufferAddRead to queue read requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_AK4954\_ReadQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the read queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_AK4954\_ReadQueuePurge**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_WriteQueuePurge Function

bool DRV\_AK4954\_WriteQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the write queue.

**Description**

This function removes all the buffer requests from the write queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_AK4954\_Open function.|

**Returns**

True - Write queue purge is successful. False - Write queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_AK4954\_Open function.*

*// Use DRV\_AK4954\_BufferAddWrite to queue write requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_AK4954\_WriteQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the write queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_AK4954\_WriteQueuePurge**\(**const** DRV\_HANDLE **handle**\);

#### Settings Functions

##### DRV\_AK4954\_MicGainGet Function

uint8\_t DRV\_AK4954\_MicGainGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the microphone gain for the AK4954 Codec.

**Description**

This functions gets the current microphone gain programmed to the Codec AK4954.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

Microphone gain, in range 0-31.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t gain;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

gain = DRV\_AK4954\_MicGainGet\(myAK4954Handle\);

**C**

uint8\_t **DRV\_AK4954\_MicGainGet**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_MicGainSet Function

void DRV\_AK4954\_MicGainSet\(DRV\_HANDLE handle, uint8\_t gain\)

**Summary**

This function sets the microphone gain for the AK4954 CODEC.

**Description**

This functions sets the microphone gain value from 0-31 which can range from -1.5 to 28.3 dB

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|gain|Gain value, in range 0-31|

**Returns**

None.

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_MicGainSet\(myAK4954Handle, 15\); *//AK4954 mic gain set to 15*

**Remarks**

None.

**C**

**void** **DRV\_AK4954\_MicGainSet**\(DRV\_HANDLE **handle**, uint8\_t **gain**\);

##### DRV\_AK4954\_MicMuteOff Function

void DRV\_AK4954\_MicMuteOff\(DRV\_HANDLE handle\)

**Summary**

Umutes th AK4954's microphone input.

**Description**

This function unmutes the AK4954's microphone input.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_MicMuteOff\(myAK4954Handle\); *//AK4954 microphone unmuted*

**C**

**void** **DRV\_AK4954\_MicMuteOff**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_MicMuteOn Function

void DRV\_AK4954\_MicMuteOn\(DRV\_HANDLE handle\);

**Summary**

Mutes the AK4954's microphone input

**Description**

This function mutes the AK4954's microphone input

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_MicMuteOn\(myAK4954Handle\); *//AK4954 microphone muted*

**C**

**void** **DRV\_AK4954\_MicMuteOn**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_MicSet Function

void DRV\_AK4954\_IntMic12Set

**Summary**

This function sets up the codec for the internal or the AK4954 Mic1 or Mic2 input.

**Description**

This function sets up the codec.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|micInput|Internal vs External mic input|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4954\_MicSet**\(DRV\_HANDLE **handle**, DRV\_AK4954\_MIC **micInput**\);

##### DRV\_AK4954\_MonoStereoMicSet Function

void DRV\_AK4954\_MonoStereoMicSet\(DRV\_HANDLE handle\);

**Summary**

This function sets up the codec for the Mono or Stereo microphone mode.

**Description**

This function sets up the codec for the Mono or Stereo microphone mode.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4954\_MonoStereoMicSet**\(DRV\_HANDLE **handle**, DRV\_AK4954\_MONO\_STEREO\_MIC **mono\_stereo\_mic**\);

##### DRV\_AK4954\_MuteOff Function

void DRV\_AK4954\_MuteOff\(DRV\_HANDLE handle\)

**Summary**

This function disables AK4954 output for soft mute.

**Description**

This function disables AK4954 output for soft mute.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_MuteOff\(myAK4954Handle\); *//AK4954 output soft mute disabled*

**C**

**void** **DRV\_AK4954\_MuteOff**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_MuteOn Function

void DRV\_AK4954\_MuteOn\(DRV\_HANDLE handle\);

**Summary**

This function allows AK4954 output for soft mute on.

**Description**

This function Enables AK4954 output for soft mute.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_MuteOn\(myAK4954Handle\); *//AK4954 output soft muted*

**C**

**void** **DRV\_AK4954\_MuteOn**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_IntExtMicSet Function

void DRV\_AK4954\_IntExtMicSet

**Summary**

This function sets up the codec for the X32 DB internal or the external microphone use.

**Description**

This function sets up the codec for the internal or the external microphone use.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|micInput|Internal vs External mic input|

**Returns**

None

**Remarks**

None.

**C**

**void** **DRV\_AK4954\_IntExtMicSet**\(DRV\_HANDLE **handle**, DRV\_AK4954\_INT\_EXT\_MIC **micInput**\);

##### DRV\_AK4954\_SamplingRateGet Function

uint32\_t DRV\_AK4954\_SamplingRateGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the sampling rate set on the DAC AK4954.

**Description**

This function gets the sampling rate set on the DAC AK4954.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

uint32\_t baudRate;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

baudRate = DRV\_AK4954\_SamplingRateGet\(myAK4954Handle\);

**C**

uint32\_t **DRV\_AK4954\_SamplingRateGet**\(DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_SamplingRateSet Function

void DRV\_AK4954\_SamplingRateSet\(DRV\_HANDLE handle, uint32\_t samplingRate\)

**Summary**

This function sets the sampling rate of the media stream.

**Description**

This function sets the media sampling rate for the client handle.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_SamplingRateSet\(myAK4954Handle, 48000\); *//Sets 48000 media sampling rate*

**C**

**void** **DRV\_AK4954\_SamplingRateSet**\(DRV\_HANDLE **handle**, uint32\_t **samplingRate**\);

##### DRV\_AK4954\_VolumeGet Function

uint8\_t DRV\_AK4954\_VolumeGet\(DRV\_HANDLE handle, DRV\_AK4954\_CHANNEL chan\)

**Summary**

This function gets the volume for AK4954 Codec.

**Description**

This functions gets the current volume programmed to the CODEC AK4954.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|chan|Audio channel volume to be set|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t volume;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

volume = DRV\_AK4954\_VolumeGet\(myAK4954Handle,DRV\_AK4954\_CHANNEL\_LEFT\);

**C**

uint8\_t **DRV\_AK4954\_VolumeGet**\(DRV\_HANDLE **handle**, DRV\_AK4954\_CHANNEL **chan**\);

##### DRV\_AK4954\_VolumeSet Function

void DRV\_AK4954\_VolumeSet\(DRV\_HANDLE handle, DRV\_AK4954\_CHANNEL channel, uint8\_t volume\);

**Summary**

This function sets the volume for AK4954 Codec.

**Description**

This functions sets the volume value from 0-255. The codec has DAC value to volume range mapping as :- 00 H : +12dB FF H : -115dB In order to make the volume value to dB mapping monotonically increasing from 00 to FF, re-mapping is introduced which reverses the volume value to dB mapping as well as normalizes the volume range to a more audible dB range. The current driver implementation assumes that all dB values under -60 dB are inaudible to the human ear. Re-Mapped values 00 H : -60 dB FF H : +12 dB

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|chan|Audio channel volume to be set|
|volume|volume value specified in the range 0-255 \(0x00 to 0xFF\)|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myAK4954Handle is the handle returned // by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_VolumeSet\(myAK4954Handle, DRV\_AK4954\_CHANNEL\_LEFT, 120\);

**C**

**void** **DRV\_AK4954\_VolumeSet**\(DRV\_HANDLE **handle**, DRV\_AK4954\_CHANNEL **channel**, uint8\_t **volume**\);

#### Other Functions

##### DRV\_AK4954\_LRCLK\_Sync Function

uint32\_t DRV\_AK4954\_LRCLK\_Sync \(const DRV\_HANDLE handle\);

**Summary**

Synchronize to the start of the I2S LRCLK \(left/right clock\) signal

**Description**

This function waits until low-to high transition of the I2S LRCLK \(left/right clock\) signal \(high-low if Left-Justified format, this is determined by the PLIB\). In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize calls to the DMA with the LRCLK signal so the left/right channel association is valid.

**Preconditions**

None.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myAK4954Handle is the handle returned* *// by the DRV\_AK4954\_Open function.*

DRV\_AK4954\_LRCLK\_Sync\(myAK4954Handle\);

**C**

**bool** **DRV\_AK4954\_LRCLK\_Sync**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_AK4954\_GetI2SDriver Function

DRV\_HANDLE DRV\_AK4954\_GetI2SDriver\(DRV\_HANDLE codecHandle\)

**Summary**

Get the handle to the I2S driver for this codec instance.

**Description**

Returns the appropriate handle to the I2S based on the ioIent member of the codec object.

**Preconditions**

The DRV\_AK4954\_Initialize routine must have been called for the specified AK4954 driver instance.

DRV\_AK4954\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

-   A handle to the I2S driver for this codec instance


**Remarks**

This allows the caller to directly access portions of the I2S driver that might not be available via the codec API.

**C**

DRV\_HANDLE **DRV\_AK4954\_GetI2SDriver**\(DRV\_HANDLE **codecHandle**\);

##### DRV\_AK4954\_VersionStrGet Function

int8\_t\* DRV\_AK4954\_VersionStrGet\(void\)

**Summary**

This function returns the version of AK4954 driver in string format.

**Description**

The DRV\_AK4954\_VersionStrGet function returns a string in the format: "." Where: is the AK4954 driver's version number. is the AK4954 driver's version number. is an optional "patch" or "dot" release number \(which is not included in the string if it equals "00"\). is an optional release type \("a" for alpha, "b" for beta ? not the entire word spelled out\) that is not included if the release is a production version \(I.e. Not an alpha or beta\).

The String does not contain any spaces.

**Preconditions**

None.

**Returns**

returns a string containing the version of AK4954 driver.

**Remarks**

None.

**Example 1**

"0.03a" "1.00"

**Example 2**

int8\_t \*AK4954string; AK4954string = DRV\_AK4954\_VersionStrGet\(\);

**C**

int8\_t\* **DRV\_AK4954\_VersionStrGet**\(\);

##### DRV\_AK4954\_VersionGet Function

uint32\_t DRV\_AK4954\_VersionGet\( void \)

**Summary**

This function returns the version of AK4954 driver.

**Description**

The version number returned from the DRV\_AK4954\_VersionGet function is an unsigned integer in the following decimal format. \* 10000 + \* 100 + Where the numbers are represented in decimal and the meaning is the same as above. Note that there is no numerical representation of release type.

**Preconditions**

None.

**Returns**

returns the version of AK4954 driver.

**Remarks**

None.

**Example 1**

For version "0.03a", return: 0 \* 10000 + 3 \* 100 + 0 For version "1.00", return: 1 \* 100000 + 0 \* 100 + 0

**Example 2**

uint32\_t AK4954version; AK4954version = DRV\_AK4954\_VersionGet\(\);

**C**

uint32\_t **DRV\_AK4954\_VersionGet**\(\);

**Data Types and Constants**

## *DRV\_AK4954\_AUDIO\_DATA\_FORMAT Type*

Identifies the Serial Audio data interface format.

**Description**

AK4954 Audio data format

This enumeration identifies Serial Audio data interface format.

**C**

**typedef** **enum** DRV\_AK4954\_AUDIO\_DATA\_FORMAT@1 **DRV\_AK4954\_AUDIO\_DATA\_FORMAT**;

## *DRV\_AK4954\_BUFFER\_EVENT Type*

Identifies the possible events that can result from a buffer add request.

**Description**

AK4954 Driver Events

This enumeration identifies the possible events that can result from a buffer add request caused by the client calling either the DRV\_AK4954\_BufferAddWrite\(\) or the DRV\_AK4954\_BufferAddRead\(\) function.

**Remarks**

One of these values is passed in the "event" parameter of the event handling callback function that the client registered with the driver by calling the DRV\_AK4954\_BufferEventHandlerSet function when a buffer transfer request is completed.

**C**

**typedef** **enum** DRV\_AK4954\_BUFFER\_EVENT@1 **DRV\_AK4954\_BUFFER\_EVENT**;

## *DRV\_AK4954\_BUFFER\_EVENT\_HANDLER Type*

Pointer to a AK4954 Driver Buffer Event handler function

**Description**

AK4954 Driver Buffer Event Handler Function

This data type defines the required function signature for the AK4954 driver buffer event handling callback function. A client must register a pointer to a buffer event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive buffer related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|event|Identifies the type of event|
|bufferHandle|Handle identifying the buffer to which the event relates|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

If the event is DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE, this means that the data was transferred successfully.

If the event is DRV\_AK4954\_BUFFER\_EVENT\_ERROR, this means that the data was not transferred successfully. The bufferHandle parameter contains the buffer handle of the buffer that failed. The DRV\_AK4954\_BufferProcessedSizeGet\(\) function can be called to find out how many bytes were processed.

The bufferHandle parameter contains the buffer handle of the buffer that associated with the event.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_AK4954\_BufferEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The buffer handle in bufferHandle expires after this event handler exits. In that the buffer object that was allocated is deallocated by the driver after the event handler exits.

The event handler function executes in the data driver\(i2S\) peripheral's interrupt context when the driver is configured for interrupt mode operation. It is recommended of the application to not perform process intensive or blocking operations with in this function.

DRV\_AK4954\_BufferAddWrite function can be called in the event handler to add a buffer to the driver queue.

**Example**

**void** APP\_MyBufferEventHandler\( DRV\_AK4954\_BUFFER\_EVENT event, DRV\_AK4954\_BUFFER\_HANDLE bufferHandle, uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context; **switch**\(event\) \{ **case** DRV\_AK4954\_BUFFER\_EVENT\_COMPLETE: *// Handle the completed buffer.* **break**; **case** DRV\_AK4954\_BUFFER\_EVENT\_ERROR: **default**: *// Handle error.* **break**; \}

\}

**C**

**typedef** **void** \(\* **DRV\_AK4954\_BUFFER\_EVENT\_HANDLER**\)\(DRV\_AK4954\_BUFFER\_EVENT event, DRV\_AK4954\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\);

## *DRV\_AK4954\_BUFFER\_HANDLE Type*

Handle identifying a write buffer passed to the driver.

**Description**

AK4954 Driver Buffer Handle

A buffer handle value is returned by a call to the DRV\_AK4954\_BufferAddWrite\(\) or DRV\_AK4954\_BufferAddRead\(\) function. This handle is associated with the buffer passed into the function and it allows the application to track the completion of the data from \(or into\) that buffer.

The buffer handle value returned from the "buffer add" function is returned back to the client by the "event handler callback" function registered with the driver.

The buffer handle assigned to a client request expires when the client has been notified of the completion of the buffer transfer \(after event handler function that notifies the client returns\) or after the buffer has been retired by the driver if no event handler callback was set.

**Remarks**

None

**C**

**typedef** uintptr\_t **DRV\_AK4954\_BUFFER\_HANDLE**;

## *DRV\_AK4954\_CHANNEL Type*

Identifies Left/Right Audio channel

**Description**

AK4954 Audio Channel

This enumeration identifies Left/Right Audio channel

**Remarks**

None.

DRV\_AK4954\_CHANNEL@1 **DRV\_AK4954\_CHANNEL**;

## *DRV\_AK4954\_COMMAND\_EVENT\_HANDLER Type*

Pointer to a AK4954 Driver Command Event Handler Function

**Description**

AK4954 Driver Command Event Handler Function

This data type defines the required function signature for the AK4954 driver command event handling callback function.

A command is a control instruction to the AK4954 Codec. Example Mute ON/OFF, Zero Detect Enable/Disable etc.

A client must register a pointer to a command event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive command related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

The occurrence of this call back means that the last control command was transferred successfully.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_AK4954\_CommandEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The event handler function executes in the control data driver interrupt context. It is recommended of the application to not perform process intensive or blocking operations with in this function.

**Example**

**void** APP\_AK4954CommandEventHandler\( uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context;

*// Last Submitted command is completed.* *// Perform further processing here*

\}

**C**

**typedef** **void** \(\* **DRV\_AK4954\_COMMAND\_EVENT\_HANDLER**\)\(uintptr\_t contextHandle\);

## *DRV\_AK4954\_DIGITAL\_BLOCK\_CONTROL Type*

Identifies Bass-Boost Control function

**Description**

AK4954 Bass-Boost Control

This enumeration identifies the settings for Bass-Boost Control function.

**Remarks**

None.

DRV\_AK4954\_DIGITAL\_BLOCK\_CONTROL@1 **DRV\_AK4954\_DIGITAL\_BLOCK\_CONTROL**;

### DRV\_AK4954\_INIT Type

Defines the data required to initialize or reinitialize the AK4954 driver

**Description**

AK4954 Driver Initialization Data

This data type defines the data required to initialize or reinitialize the AK4954 Codec driver.

**Remarks**

None.

**C**

**typedef** **struct** DRV\_AK4954\_INIT@1 **DRV\_AK4954\_INIT**;

## *DRV\_AK4954\_INT\_EXT\_MIC Type*

Identifies the Mic input source.

**Description**

AK4954 Mic Internal / External Input

This enumeration identifies the Mic input source.

**C**

**typedef** **enum** DRV\_AK4954\_INT\_EXT\_MIC@1 **DRV\_AK4954\_INT\_EXT\_MIC**;

### DRV\_AK4954\_MIC Type

This is type DRV\_AK4954\_MIC.

**C**

**typedef** **enum** DRV\_AK4954\_MIC@1 **DRV\_AK4954\_MIC**;

## *DRV\_AK4954\_MONO\_STEREO\_MIC Type*

Identifies the Mic input as Mono / Stereo.

**Description**

AK4954 Mic Mono / Stereo Input

This enumeration identifies the Mic input as Mono / Stereo.

**C**

**typedef** **enum** DRV\_AK4954\_MONO\_STEREO\_MIC@1 **DRV\_AK4954\_MONO\_STEREO\_MIC**;

## *SAMPLE\_LENGTH Type*

in bits

SAMPLE\_LENGTH@2 **SAMPLE\_LENGTH**;

## *DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S Macro*

for compatability with old code

**C**

**\#define** **DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S** DRV\_AK4954\_AUDIO\_DATA\_FORMAT\_I2S\_16BIT\_24BIT *// for compatability with old code*

## *DRV\_AK4954\_BUFFER\_HANDLE\_INVALID Macro*

Definition of an invalid buffer handle.

**Description**

AK4954 Driver Invalid Buffer Handle

This is the definition of an invalid buffer handle. An invalid buffer handle is returned by DRV\_AK4954\_BufferAddWrite\(\) and the DRV\_AK4954\_BufferAddRead\(\) function if the buffer add request was not successful.

**Remarks**

None

**C**

**\#define** **DRV\_AK4954\_BUFFER\_HANDLE\_INVALID** \(\(DRV\_AK4954\_BUFFER\_HANDLE\)\(-1\)\)

### DRV\_AK4954\_COUNT Macro

Number of valid AK4954 driver indices

**Description**

AK4954 Driver Module Count

This constant identifies the maximum number of AK4954 Driver instances that should be defined by the application. Defining more instances than this constant will waste RAM memory space.

This constant can also be used by the application to identify the number of AK4954 instances on this microcontroller.

**Remarks**

This value is part-specific.

**C**

**\#define** **DRV\_AK4954\_COUNT**

### DRV\_AK4954\_INDEX\_0 Macro

AK4954 driver index definitions

**Description**

Driver AK4954 Module Index

These constants provide AK4954 driver index definition.

**Remarks**

These constants should be used in place of hard-coded numeric literals. These values should be passed into the

DRV\_AK4954\_Initialize and DRV\_AK4954\_Open routines to identify the driver instance in use.

**C**

**\#define** **DRV\_AK4954\_INDEX\_0** 0

### DRV\_AK4954\_INDEX\_1 Macro

This is macro DRV\_AK4954\_INDEX\_1.

**C**

**\#define** **DRV\_AK4954\_INDEX\_1** 1

### DRV\_AK4954\_INDEX\_2 Macro

This is macro DRV\_AK4954\_INDEX\_2.

**C**

**\#define** **DRV\_AK4954\_INDEX\_2** 2

### DRV\_AK4954\_INDEX\_3 Macro

This is macro DRV\_AK4954\_INDEX\_3.

**C**

**\#define** **DRV\_AK4954\_INDEX\_3** 3

### DRV\_AK4954\_INDEX\_4 Macro

This is macro DRV\_AK4954\_INDEX\_4.

**C**

**\#define** **DRV\_AK4954\_INDEX\_4** 4

### DRV\_AK4954\_INDEX\_5 Macro

This is macro DRV\_AK4954\_INDEX\_5.

**C**

**\#define** **DRV\_AK4954\_INDEX\_5** 5

### Files

**Files**

|**Name**|**Description**|
|--------|---------------|
|drv\_ak4954.h|AK4954 Codec Driver Interface header file|
|drv\_ak4954\_config\_template.h|AK4954 Codec Driver Configuration Template.|

**Description**

### drv\_ak4954.h

drv\_ak4954.h

**Summary**

AK4954 Codec Driver Interface header file

**Description**

AK4954 Codec Driver Interface

The AK4954 Codec device driver interface provides a simple interface to manage the AK4954 16/24/32-Bit Codec that can be interfaced to a Microchip microcontroller. This file provides the public interface definitions for the AK4954 Codec device driver.

### drv\_ak4954\_config\_template.h

drv\_ak4954\_config\_template.h

**Summary**

AK4954 Codec Driver Configuration Template.

**Description**

AK4954 Codec Driver Configuration Template

These file provides the list of all the configurations that can be used with the driver. This file should not be included in the driver.

**GENERIC AUDIO DRIVER CODEC Library**

This topic describes the Generic Codec Driver Library.

## *Introduction*

### Summary

This library provides an Applications Programming Interface \(API\) to manage the Generic Codec that is serially interfaced to the I2C and I2S peripherals of a Microchip microcontroller for the purpose of providing audio solutions.

**Description**

### Description

This file contains the implementation of the Generic Codec driver, which provides a simple interface to manage a codec that can be interfaced to Microchip microcontroller. The user will need to modify it to match the requirements of their codec. Areas where code needs to be added or changed are marked with TO-DO!!

Note: this module assumes the codec is controlled over an I2C interface. The I2C Driver will need to be enabled in the MHC Project Graph. If another type of interface is used, the user will need to modify the code to suit. This module makes use of SYS\_TIME. It will need to be enabled in the Project Graph.

This module assumes the an I2S interface is used for audio data output \(to headphones or line-out\) and input \(from microphone or line-in\).

The Generic Codec can be configured as either an I2S clock slave \(receives all clocks from the host\), or I2S clock master \(generates I2S clocks from a master clock input MCLK\).

A typical interface of Generic Codec to a Microchip microcontroller using an I2C and SSC interface \(configured as I2S\), with the Generic Codec set up as the I2S clock slave, is provided in the following diagram: ![](GUID-98432D6B-B7BB-41C7-81E9-59C3894319CD-low.png)

## *Using the Library*

This topic describes the basic architecture of the Generic Codec Driver Library and provides information and examples on its use.

**Description**

### Interface Header File: drv\_Generic.h

The interface to the Generic Codec Driver library is defined in the audio/driver/codec/Generic/drv\_Generic.h header file. Any C language source \(.c\) file that uses the Generic Codec Driver library should include this header.

**Library Source Files:**

The Generic Codec Driver library source files are provided in the audio/driver/codec/Generic/src directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features and to **Building the Library** for instructions on how to build the library.

**Example Applications:**

This codec is not used directly by any demonstration applications. However the following applications could be looked at, to see how a codec such as the WM8904 or AK4954 is used:

-   audio/apps/audio\_tone

-   audio/apps/microphone\_loopback


## Abstraction Model

This library provides a low-level abstraction of the Generic Codec Driver Library on the Microchip family microcontrollers with a convenient C language interface. This topic describes how that abstraction is modeled in software and introduces the library's interface.

**Description**

The abstraction model shown in the following diagram depicts how the Generic Codec Driver is positioned in the MPLAB Harmony framework. The Generic Codec Driver uses the I2C and I2S drivers for control and audio data transfers to the Generic module.

### Generic Driver Abstraction Model

![](GUID-4461FAD4-1090-4853-AA68-0119968102AD-low.png)

## Library Overview

Refer to the Driver Library Overview section for information on how the driver operates in a system.

The Generic Codec Driver Library provides an API interface to transfer control commands and digital audio data to the serially interfaced Generic Codec module. The library interface routines are divided into various sub-sections, which address one of the blocks or the overall operation of the Generic Codec Driver Library.

|**Library Interface Section**|**Description**|
|-----------------------------|---------------|
|System Functions|Provides system module interfaces, device initialization, deinitialization, reinitialization, tasks and status functions.|
|Client Setup Functions|Provides open and close functions.|
|Data Transfer Functions|Provides data transfer functions, such as Buffer Read and Write.|
|Settings Functions|Provides driver specific functions for settings, such as volume control and sampling rate.|
|Other Functions|Miscellaneous functions, such as getting the driver’s version number and syncing to the LRCLK signal.|
|Data Types and Constants|These data types and constants are required while interacting and setting up the Generic Codec Driver Library.|

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** All functions and constants in this section are named with the format DRV\_ Generic\_xxx, where 'xxx' is a function name or constant. These names are redefined in the appropriate configuration’s configuration.h file to the format DRV\_CODEC\_xxx using \#defines so that code in the application that references the library can be written as generically as possible \(e.g., by writing DRV\_CODEC\_Open instead of DRV\_ Generic\_Open etc.\). This allows the codec type to be changed in the MHC without having to modify the application’s source code.

## How the Library Works

How the Library Works

The library provides interfaces to support:

-   System Functionality

-   Client Functionality


### Setup \(Initialization\)

This topic describes system initialization, implementations, and includes a system access code example.

**Description**

## System Initialization

The system performs the initialization of the device driver with settings that affect only the instance of the device that is being initialized. During system initialization in the system\_init.c file, each instance of the Generic module would be initialized with the following configuration settings \(either passed dynamically at run time using DRV\_Generic\_INIT or by using Initialization Overrides\) that are supported by the specific Generic device hardware:

-   Device requested power state: one of the System Module Power States. For specific details please refer to Data Types and Constants in the Library Interface section.

-   I2C driver module index. The module index should be same as the one used in initializing the I2C Driver

-   I2S driver module index. The module index should be same as the one used in initializing the I2S Driver

-   Sampling rate

-   Volume

-   Audio data format. The audio data format should match with the audio data format settings done in I2S driver initialization

-   Determines whether or not the microphone input is enabled


The DRV\_Generic\_Initialize API returns an object handle of the type SYS\_MODULE\_OBJ. The object handle returned by the Initialize interface would be used by the other system interfaces such as DRV\_ Generic\_Deinitialize, DRV\_ Generic\_Status and DRV\_I2S\_Tasks.

### Client Access

This topic describes driver initialization and provides a code example.

**Description**

For the application to start using an instance of the module, it must call the DRV\_Generic\_Open function. The

DRV\_Generic\_Open function provides a driver handle to the Generic Codec Driver instance for operations. If the driver is deinitialized using the function DRV\_Generic\_Deinitialize, the application must call the DRV\_Generic\_Open function again to set up the instance of the driver.

For the various options available for IO\_INTENT, please refer to Data Types and Constants in the Library Interface section.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is necessary to check the status of driver initialization before opening a driver instance. The status of the Generic Codec Driver can be known by calling DRV\_ Generic\_Status.

**Example:**

DRV\_HANDLE handle; SYS\_STATUS genericStatus;

genericStatus Status = DRV\_Generic\_Status\(sysObjects.genericStatus DevObject\); if \(SYS\_STATUS\_READY == genericStatus\) \{ // The driver can now be opened.

appData.genericClient.handle = DRV\_Generic\_Open

\(DRV\_Generic\_INDEX\_0, DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_EXCLUSIVE\); if\(appData.genericClient.handle != DRV\_HANDLE\_INVALID\) \{

appData.state = APP\_STATE\_Generic\_SET\_BUFFER\_HANDLER; \} else \{ SYS\_DEBUG\(0, "Find out what's wrong \\r\\n"\);

\} \} else \{ /\* Generic Driver Is not ready \*/

\}

### Client Operations

This topic provides information on client operations.

**Description**

Client operations provide the API interface for control command and audio data transfer to the Generic Codec.

The following Generic Codec specific control command functions are provided:

-   DRV\_Generic\_SamplingRateSet

-   DRV\_Generic\_SamplingRateGet

-   DRV\_Generic\_VolumeSet

-   DRV\_Generic\_VolumeGet

-   DRV\_Generic\_MuteOn

-   DRV\_Generic\_MuteOff


These functions schedule a non-blocking control command transfer operation. These functions submit the control command request to the Generic Codec. These functions submit the control command request to I2C Driver transmit queue, the request is processed immediately if it is the first request, or processed when the previous request is complete.

DRV\_Generic\_BufferAddWrite, DRV\_Generic\_BufferAddRead, and DRV\_Generic\_BufferAddWriteRead are buffered data operation functions. These functions schedule non-blocking audio data transfer operations. These functions add the request to I2S Driver transmit or receive buffer queue depends on the request type, and are executed immediately if it is the first buffer, or executed later when the previous buffer is complete. The driver notifies the client with

DRV\_Generic\_BUFFER\_EVENT\_COMPLETE, DRV\_Generic\_BUFFER\_EVENT\_ERROR, or DRV\_Generic\_BUFFER\_EVENT\_ABORT events.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is not necessary to close and reopen the client between multiple transfers.

### Configuring the Library

The configuration of the I2S Driver Library is based on the file configurations.h, as generated by the MHC.

This header file contains the configuration selection for the I2S Driver Library. Based on the selections made, the I2S Driver Library may support the selected features. These configuration settings will apply to all instances of the I2S Driver Library.

This header can be placed anywhere; however, the path of this header needs to be present in the include search path for a successful build. Refer to the Applications Help section for more details.

### System Configuration

**Macros**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT\_MACRO|Specifies the audio data format for the codec.|
|-|DRV\_GENERICCODEC\_AUDIO\_SAMPLING\_RATE|Specifies the initial baud rate for the codec.|
|-|DRV\_GENERICCODEC\_CLIENTS\_NUMBER|Sets up the maximum number of clients that can be connected to any hardware instance.|
|-|DRV\_GENERICCODEC\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2C interface.|
|-|DRV\_GENERICCODEC\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2S interface.|
|-|DRV\_GENERICCODEC\_INSTANCES\_NUMBER|Sets up the maximum number of hardware instances that can be supported|
|-|DRV\_GENERICCODEC\_VOLUME|Specifies the initial volume level.|

**Description**

## *DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT\_MACRO Macro*

Specifies the audio data format for the codec.

**Description**

Generic Codec Audio Data Format

Sets up the length of each sample plus the format \(I2S or left-justified\) for the audio.

Valid choices are: "DATA\_16\_BIT\_LEFT\_JUSTIFIED", 16-bit Left Justified "DATA\_16\_BIT\_I2S", 16-bit I2S"

"DATA\_32\_BIT\_LEFT\_JUSTIFIED", 32-bit Left Justified "DATA\_32\_BIT\_I2S", 32-bit I2S"

**C**

**\#define** **DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT\_MACRO**

## *DRV\_GENERICCODEC\_AUDIO\_SAMPLING\_RATE Macro*

Specifies the initial baud rate for the codec.

**Description**

Generic Codec Baud Rate

Sets the initial baud rate \(sampling rate\) for the codec. Typical values are 8000, 16000, 44100, 48000, 88200 and 96000.

**C**

**\#define** **DRV\_GENERICCODEC\_AUDIO\_SAMPLING\_RATE**

## *DRV\_GENERICCODEC\_CLIENTS\_NUMBER Macro*

Sets up the maximum number of clients that can be connected to any hardware instance.

**Description**

Generic Codec Client Count Configuration

Sets up the maximum number of clients that can be connected to any hardware instance. Typically only one client could be connected to one hardware instance. This value represents the total number of clients to be supported across all hardware instances.

**C**

**\#define** **DRV\_GENERICCODEC\_CLIENTS\_NUMBER**

## *DRV\_GENERICCODEC\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2C interface.

**Description**

Generic Codec I2C instance number

Specifies the instance number of the I2C interface being used by the MCU to send commands and receive status to and from the Generic Codec. enabled.

**C**

**\#define** **DRV\_GENERICCODEC\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_GENERICCODEC\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2S interface.

**Description**

Generic Codec I2S instance number

Specifies the instance number of the I2S interface being used by the MCU to send and receive audio data to and from the Generic Codec. enabled.

**C**

**\#define** **DRV\_GENERICCODEC\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_GENERICCODEC\_INSTANCES\_NUMBER Macro*

Sets up the maximum number of hardware instances that can be supported

**Description**

Generic Codec driver objects configuration

Sets up the maximum number of hardware instances that can be supported. It is recommended that this number be set exactly equal to the number of Generic Codec modules that are needed by an application, namely one.

**C**

**\#define** **DRV\_GENERICCODEC\_INSTANCES\_NUMBER**

## *DRV\_GENERICCODEC\_VOLUME Macro*

Specifies the initial volume level.

**Description**

Generic Codec Volume

Sets the initial volume level, in the range 0-255.

**Remarks**

The value is mapped to an internal Generic Codec volume level in the range 0-192 using a logarithmic table so the input scale appears linear \(128 is half volume\).

**C**

**\#define** **DRV\_GENERICCODEC\_VOLUME**

### Confuguring MHC

Provides examples on how to configure the MPLAB Harmony Configurator \(MHC\) for a specific driver.

**Description**

When building a new application, start by creating a 32-bit MPLAB Harmony 3 project in MPLAB X IDE by selecting *File \> New Project*. Chose the Configuration name the based on the BSP, and select the appropriate processor \(such as ATSAME70Q21B\).

In the MHC, under Available Components select the appropriate BSP, such as SAM E70 Xplained Ultra. Under *Audio\>Templates*, double-click on a codec template such as Generic. Answer Yes to all questions.

You should end up with a project graph that looks like this, after rearranging the boxes: ![](GUID-FFCFDEBB-A94E-4375-A7AC-AD7C7AF3BB37-low.png)

Click on the Generic Driver component \(not Generic Codec\) and the following menu will be displayed in the Configurations Options: ![](GUID-D90F9968-8569-4729-BDA2-B18F10381A33-low.png)

-   **I2C Driver Used** will display the driver instance used for the I2C interface.

-   **I2S Driver Used** will display the driver instance used for the I2S interface.

-   **Number of Generic Clients** indicates the maximum number of clients that can be connected to the Generic Driver.

-   **Sampling Rate** indicates the number of samples per second per channel, 8000 to 96,000.

-   **Volume** indicates the volume in a linear scale from 0-255.


#### • Audio Data Format is either

-   24-bit Left Justified \(ADC\), 24-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 16-bit Right-justified\(DAC\)

-   24-bit Left Justified \(ADC\), 24-bit Left-justified\(DAC\)

-   24/16-bit I2S

-   32-bit Left Justified \(ADC\), 32-bit Left-justified\(DAC\)

-   32-bit I2S


It must match the audio protocol and data length set up in either the SSC or I2S PLIB..

You can also bring in the Generic Driver by itself, by double clicking Generic under Audio\_-\>Driver-\>Codec\_ in the Available Components list. You will then need to add any additional needed components manually and connect them together.

Note that the Generic requires the TCx Peripheral Library and TIME System Service in order to perform some of its internal timing sequences.

### Building the Library

This section lists the files that are available in the Generic Codec Driver Library.

**Description**

This section lists the files that are available in the src folder of the Generic Codec Driver. It lists which files need to be included in the build based on either a hardware feature present on the board or configuration option selected by the system.

The following three tables list and describe the header \(.h\) and source \(.c\) files that implement this library. The parent folder for these files is audio/driver/codec/Generic.

### Interface File\(s\)

This table lists and describes the header files that must be included \(i.e., using \#include\) by any code that uses this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|drv\_genericcodec.h|Header file that exports the driver API.|

### Required File\(s\)

![](GUID-B924D07A-0D74-453B-BCDE-8190A195C55D-low.png) ***All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use.***

This table lists and describes the source and header files that must *always* be included in the MPLAB X IDE project to build this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|/src/drv\_genericcodec.c|This file contains implementation of the Generic Codec Driver.|

### Optional File\(s\)

This table lists and describes the source and header files that may optionally be included if required for the desired implementation.

|**Source File Name**|**Description**|
|--------------------|---------------|
|N/A|No optional files are available for this library.|

### Module Dependencies

The Generic Codec Driver Library depends on the following modules:

-   I2S Driver Library

-   I2C Driver Library


#### *Library Interface*

**Client Setup Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_GENERICCODEC\_Open|Opens the specified Generic Codec driver instance and returns a handle to it|
||DRV\_GENERICCODEC\_Close|Closes an opened-instance of the Generic Codec driver|
||DRV\_GENERICCODEC\_BufferEventHandlerSet|This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.|
||DRV\_GENERICCODEC\_CommandEventHandlerSet|This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.|

**Dara Types and Constants**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_GENERICCODEC\_BUFFER\_EVENT|Identifies the possible events that can result from a buffer add request.|
|-|DRV\_GENERICCODEC\_BUFFER\_EVENT\_HANDLER|Pointer to a Generic Codec Driver Buffer Event handler function|
|-|DRV\_GENERICCODEC\_BUFFER\_HANDLE|Handle identifying a write buffer passed to the driver.|
|-|DRV\_GENERICCODEC\_CHANNEL|Identifies Left/Right Audio channel|
|-|DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER|Pointer to a Generic Codec Driver Command Event Handler Function|
|-|DRV\_GENERICCODEC\_INIT|Defines the data required to initialize or reinitialize the Generic Codec driver|
|-|DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID|Definition of an invalid buffer handle.|
|-|DRV\_GENERICCODEC\_COUNT|Number of valid Generic Codec driver indices|
|-|DRV\_GENERICCODEC\_INDEX\_0|Generic Codec driver index definitions|
|-|DRV\_GENERICCODEC\_INDEX\_1|This is macro DRV\_GENERICCODEC\_INDEX\_1.|
|-|DRV\_GENERICCODEC\_INDEX\_2|This is macro DRV\_GENERICCODEC\_INDEX\_2.|
|-|DRV\_GENERICCODEC\_INDEX\_3|This is macro DRV\_GENERICCODEC\_INDEX\_3.|
|-|DRV\_GENERICCODEC\_INDEX\_4|This is macro DRV\_GENERICCODEC\_INDEX\_4.|
|-|DRV\_GENERICCODEC\_INDEX\_5|This is macro DRV\_GENERICCODEC\_INDEX\_5.|
|-|DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT|Identifies the Serial Audio data interface format.|

**Data Transfer Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_GENERICCODEC\_BufferAddRead|Schedule a non-blocking driver read operation.|
||DRV\_GENERICCODEC\_BufferAddWrite|Schedule a non-blocking driver write operation.|
||DRV\_GENERICCODEC\_BufferAddWriteRead|Schedule a non-blocking driver write-read operation.|
||DRV\_GENERICCODEC\_ReadQueuePurge|Removes all buffer requests from the read queue.|
||DRV\_GENERICCODEC\_WriteQueuePurge|Removes all buffer requests from the write queue.|

**Other Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_GENERICCODEC\_GetI2SDriver|Get the handle to the I2S driver for this codec instance.|
||DRV\_GENERICCODEC\_LRCLK\_Sync|Synchronize to the start of the I2S LRCLK \(left/right clock\) signal|

**Settings Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_GENERICCODEC\_MicGainGet|This function gets the microphone gain for the Generic Codec.|
||DRV\_GENERICCODEC\_MicGainSet|This function sets the microphone gain for the Generic Codec CODEC.|
||DRV\_GENERICCODEC\_MicMuteOff|Umutes th Generic Codec's microphone input.|
||DRV\_GENERICCODEC\_MicMuteOn|Mutes the Generic Codec's microphone input|
||DRV\_GENERICCODEC\_MuteOff|This function disables Generic Codec output for soft mute.|
||DRV\_GENERICCODEC\_MuteOn|This function allows Generic Codec output for soft mute on.|
||DRV\_GENERICCODEC\_SamplingRateGet|This function gets the sampling rate set on the Generic Codec.|
||DRV\_GENERICCODEC\_SamplingRateSet|This function sets the sampling rate of the media stream.|
||DRV\_GENERICCODEC\_VersionGet|This function returns the version of Generic Codec driver|
||DRV\_GENERICCODEC\_VersionStrGet|This function returns the version of Generic Codec driver in string format.|
||DRV\_GENERICCODEC\_VolumeGet|This function gets the volume for Generic Codec.|
||DRV\_GENERICCODEC\_VolumeSet|This function sets the volume for Generic Codec.|

**System Interaction Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_GENERICCODEC\_Initialize|Initializes hardware and data for the instance of the Generic Codec module|
||DRV\_GENERICCODEC\_Deinitialize|Deinitializes the specified instance of the Generic Codec driver module|
||DRV\_GENERICCODEC\_Status|Gets the current status of the Generic Codec driver module.|
||DRV\_GENERICCODEC\_Tasks|Maintains the driver's control and data interface state machine.|

**Description**

#### System Interaction Functions

##### DRV\_GENERICCODEC\_Initialize Function

SYS\_MODULE\_OBJ DRV\_GENERICCODEC\_Initialize

\(

const SYS\_MODULE\_INDEX drvIndex, const SYS\_MODULE\_INIT \*const init

\);

**Summary**

Initializes hardware and data for the instance of the Generic Codec module

**Description**

This routine initializes the Generic Codec driver instance for the specified driver index, making it ready for clients to open and use it. The initialization data is specified by the init parameter. The initialization may fail if the number of driver objects allocated are insufficient or if the specified driver instance is already initialized.

**Preconditions**

DRV\_I2S\_Initialize must be called before calling this function to initialize the data interface of this Codec driver. DRV\_I2C\_Initialize must be called if SPI driver is used for handling the control interface of this Codec driver.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the driver instance to be initialized|
|init|Pointer to the data structure containing any data necessary to initialize the hardware. This pointer may be null if no data is required and default initialization is to be used.|

**Returns**

If successful, returns a valid handle to a driver instance object. Otherwise, it returns SYS\_MODULE\_OBJ\_INVALID.

**Remarks**

This routine must be called before any other Generic Codec routine is called.

This routine should only be called once during system initialization unless DRV\_GENERICCODEC\_Deinitialize is called to deinitialize the driver instance. This routine will NEVER block for hardware access.

**Example**

DRV\_GENERICCODEC\_INIT init;

SYS\_MODULE\_OBJ objectHandle; init-\>inUse = **true**; init-\>status = SYS\_STATUS\_BUSY; init-\>numClients = 0; init-\>i2sDriverModuleIndex = genercicodecInit-\>i2sDriverModuleIndex; init-\>i2cDriverModuleIndex = genercicodecInit-\>i2cDriverModuleIndex; init-\>samplingRate = DRV\_GENERICCODEC\_AUDIO\_SAMPLING\_RATE; init-\>audioDataFormat = DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT\_MACRO; init-\>isInInterruptContext = **false**; init-\>commandCompleteCallback = \(DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER\)0; init-\>commandContextData = 0; init-\>mclk\_multiplier = DRV\_GENERICCODEC\_MCLK\_SAMPLE\_FREQ\_MULTPLIER; objectHandle = DRV\_GENERICCODEC\_Initialize\(DRV\_GENERICCODEC\_0, \(SYS\_MODULE\_INIT\*\)init\); **if** \(SYS\_MODULE\_OBJ\_INVALID == objectHandle\) \{

*// Handle error*

\}

**C**

SYS\_MODULE\_OBJ **DRV\_GENERICCODEC\_Initialize**\(**const** SYS\_MODULE\_INDEX **drvIndex**, **const**

SYS\_MODULE\_INIT \* **const** **init**\);

##### DRV\_GENERICCODEC\_Deinitialize Function

void DRV\_GENERICCODEC\_Deinitialize\( SYS\_MODULE\_OBJ object\)

**Summary**

Deinitializes the specified instance of the Generic Codec driver module

**Description**

Deinitializes the specified instance of the Generic Codec driver module, disabling its operation \(and any hardware\). Invalidates all the internal data.

**Preconditions**

Function DRV\_GENERICCODEC\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_GENERICCODEC\_Initialize routine|

**Returns**

None.

**Remarks**

Once the Initialize operation has been called, the De-initialize operation must be called before the Initialize operation can be called again. This routine will NEVER block waiting for hardware.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_GENERICCODEC\_Initialize*

SYS\_STATUS status;

DRV\_GENERICCODEC\_Deinitialize\(object\); status = DRV\_GENERICCODEC\_Status\(object\); **if** \(SYS\_MODULE\_DEINITIALIZED != status\) \{

*// Check again later if you need to know* *// when the driver is deinitialized.*

\}

**C**

**void** **DRV\_GENERICCODEC\_Deinitialize**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_GENERICCODEC\_Status Function

SYS\_STATUS DRV\_GENERICCODEC\_Status\( SYS\_MODULE\_OBJ object\)

**Summary**

Gets the current status of the Generic Codec driver module.

**Description**

This routine provides the current status of the Generic Codec driver module.

**Preconditions**

Function DRV\_GENERICCODEC\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_GENERICCODEC\_Initialize routine|

**Returns**

SYS\_STATUS\_DEINITIALIZED - Indicates that the driver has been deinitialized

SYS\_STATUS\_READY - Indicates that any previous module operation for the specified module has completed

SYS\_STATUS\_BUSY - Indicates that a previous module operation for the specified module has not yet completed

SYS\_STATUS\_ERROR - Indicates that the specified module is in an error state

**Remarks**

A driver can opened only when its status is SYS\_STATUS\_READY.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_GENERICCODEC\_Initialize*

SYS\_STATUS GENERICCODECStatus;

GENERICCODECStatus = DRV\_GENERICCODEC\_Status\(object\); **if** \(SYS\_STATUS\_READY == GENERICCODECStatus\) \{

*// This means the driver can be opened using the* *// DRV\_GENERICCODEC\_Open\(\) function.*

\}

**C**

SYS\_STATUS **DRV\_GENERICCODEC\_Status**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_GENERICCODEC\_Tasks Function

void DRV\_GENERICCODEC\_Tasks\(SYS\_MODULE\_OBJ object\);

**Summary**

Maintains the driver's control and data interface state machine.

**Description**

This routine is used to maintain the driver's internal control and data interface state machine and implement its control and data interface implementations. This function should be called from the SYS\_Tasks\(\) function.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_GENERICCODEC\_Initialize\)|

**Returns**

None.

**Remarks**

This routine is normally not called directly by an application. It is called by the system's Tasks routine \(SYS\_Tasks\).

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_GENERICCODEC\_Initialize* **while** \(**true**\) \{

DRV\_GENERICCODEC\_Tasks \(object\);

*// Do other tasks*

\}

**C**

**void** **DRV\_GENERICCODEC\_Tasks**\(SYS\_MODULE\_OBJ **object**\);

#### Client Setup Functions

##### DRV\_GENERICCODEC\_Open Function

DRV\_HANDLE DRV\_GENERICCODEC\_Open

\(

const SYS\_MODULE\_INDEX drvIndex, const DRV\_IO\_INTENT ioIntent

\)

**Summary**

Opens the specified Generic Codec driver instance and returns a handle to it

**Description**

This routine opens the specified Generic Codec driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver. The ioIntent parameter defines how the client interacts with this driver instance.

The DRV\_IO\_INTENT\_BLOCKING and DRV\_IO\_INTENT\_NONBLOCKING ioIntent options are not relevant to this driver. All the data transfer functions of this driver are non blocking.

Generic Codec can be opened with DRV\_IO\_INTENT\_WRITE, or DRV\_IO\_INTENT\_READ or DRV\_IO\_INTENT\_WRITEREAD

io\_intent option. This decides whether the driver is used for headphone output, or microphone input or both modes simultaneously.

Specifying a DRV\_IO\_INTENT\_EXCLUSIVE will cause the driver to provide exclusive access to this client. The driver cannot be opened by any other client.

**Preconditions**

Function DRV\_GENERICCODEC\_Initialize must have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the object instance to be opened|
|ioIntent|Zero or more of the values from the enumeration DRV\_IO\_INTENT "ORed" together to indicate the intended use of the driver. See function description for details.|

**Returns**

If successful, the routine returns a valid open-instance handle \(a number identifying both the caller and the module instance\).

If an error occurs, the return value is DRV\_HANDLE\_INVALID. Error can occur

-   if the number of client objects allocated via DRV\_GENERICCODEC\_CLIENTS\_NUMBER is insufficient.

-   if the client is trying to open the driver but driver has been opened exclusively by another client.

-   if the driver hardware instance being opened is not initialized or is invalid.

-   if the ioIntent options passed are not relevant to this driver.


**Remarks**

The handle returned is valid until the DRV\_GENERICCODEC\_Close routine is called. This routine will NEVER block waiting for hardware.If the requested intent flags are not supported, the routine will return DRV\_HANDLE\_INVALID. This function is thread safe in a RTOS application. It should not be called in an ISR.

**Example**

DRV\_HANDLE handle;

handle = DRV\_GENERICCODEC\_Open\(DRV\_GENERICCODEC\_INDEX\_0, DRV\_IO\_INTENT\_WRITEREAD \| DRV\_IO\_INTENT\_EXCLUSIVE\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver* *// May be the driver is not initialized or the initialization* *// is not complete.*

\}

**C**

DRV\_HANDLE **DRV\_GENERICCODEC\_Open**\(**const** SYS\_MODULE\_INDEX **iDriver**, **const** DRV\_IO\_INTENT **ioIntent**\);

##### DRV\_GENERICCODEC\_Close Function

void DRV\_GENERICCODEC\_Close\( DRV\_Handle handle \)

**Summary**

Closes an opened-instance of the Generic Codec driver

**Description**

This routine closes an opened-instance of the Generic Codec driver, invalidating the handle. Any buffers in the driver queue that were submitted by this client will be removed. After calling this routine, the handle passed in "handle" must not be used with any of the remaining driver routines. A new handle must be obtained by calling DRV\_GENERICCODEC\_Open before the caller may use the driver again

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

• None

**Remarks**

Usually there is no need for the driver client to verify that the Close operation has completed. The driver will abort any ongoing operations when this routine is called.

**Example**

DRV\_HANDLE handle; *// Returned from DRV\_GENERICCODEC\_Open*

DRV\_GENERICCODEC\_Close\(handle\);

**C**

**void** **DRV\_GENERICCODEC\_Close**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_BufferEventHandlerSet Function

void DRV\_GENERICCODEC\_BufferEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_GENERICCODEC\_BUFFER\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.

**Description**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished. When a client calls DRV\_GENERICCODEC\_BufferAddWrite function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "buffer add" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the queued buffer transfer has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

*// Client registers an event handler with driver*

DRV\_GENERICCODEC\_BufferEventHandlerSet\(myGENERICCODECHandle,

DRV\_GENERICCODECBufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_GENERICCODEC\_BufferAddWrite\(myGENERICCODEChandle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{ *// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** DRV\_GENERICCODECBufferEventHandler\(DRV\_GENERICCODEC\_BUFFER\_EVENT event, DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.*

**switch**\(event\) \{ **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.*

**break**; **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

**break**;

**default**: **break**; \}

\}

**C**

**void** **DRV\_GENERICCODEC\_BufferEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_GENERICCODEC\_BUFFER\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

##### DRV\_GENERICCODEC\_CommandEventHandlerSet Function

void DRV\_GENERICCODEC\_CommandEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

**Description**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

The event handler should be set before the client performs any "Generic Codec Specific Client Routines" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the command has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

*// Client registers an event handler with driver*

DRV\_GENERICCODEC\_CommandEventHandlerSet\(myGENERICCODECHandle,

DRV\_GENERICCODECCommandEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_GENERICCODEC\_DeEmphasisFilterSet\(myGENERICCODECHandle,

DRV\_GENERICCODEC\_DEEMPHASIS\_FILTER\_44\_1KHZ\)

*// Event is received when // the buffer is processed.* **void** DRV\_GENERICCODECCommandEventHandler\(uintptr\_t contextHandle\) \{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ *// Last Submitted command is completed.*

*// Perform further processing here*

\}

\}

**C**

**void** **DRV\_GENERICCODEC\_CommandEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

#### Data Transfer Functions

##### DRV\_GENERICCODEC\_BufferAddRead Function

void DRV\_GENERICCODEC\_BufferAddRead

\(

const DRV\_HANDLE handle,

DRV\_GENERICCODEC\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver read operation.

**Description**

This function schedules a non-blocking read operation. The function returns with a valid buffer handle in the bufferHandle argument if the read request was scheduled successfully. The function adds the request to the hardware instance receive queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec device instance and the DRV\_GENERICCODEC\_Status must have returned SYS\_STATUS\_READY.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle. DRV\_IO\_INTENT\_READ must have been specified in the DRV\_GENERICCODEC\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the Generic Codec instance as return by the DRV\_GENERICCODEC\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be

DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the Generic Codec Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another Generic Codec driver instance. It should not otherwise be called directly in an ISR.

**C**

**void** **DRV\_GENERICCODEC\_BufferAddRead**\(**const** DRV\_HANDLE **handle**, DRV\_GENERICCODEC\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_GENERICCODEC\_BufferAddWrite Function

void DRV\_GENERICCODEC\_BufferAddWrite

\(

const DRV\_HANDLE handle,

DRV\_GENERICCODEC\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write operation.

**Description**

This function schedules a non-blocking write operation. The function returns with a valid buffer handle in the bufferHandle argument if the write request was scheduled successfully. The function adds the request to the hardware instance transmit queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec device instance and the DRV\_GENERICCODEC\_Status must have returned SYS\_STATUS\_READY.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_WRITE must have been specified in the DRV\_GENERICCODEC\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the Generic Codec instance as return by the DRV\_GENERICCODEC\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be

DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the Generic Codec Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another Generic Codec driver instance. It should not otherwise be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

*// Client registers an event handler with driver*

DRV\_GENERICCODEC\_BufferEventHandlerSet\(myGENERICCODECHandle,

DRV\_GENERICCODECBufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_GENERICCODEC\_BufferAddWrite\(myGENERICCODEChandle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** DRV\_GENERICCODECBufferEventHandler\(DRV\_GENERICCODEC\_BUFFER\_EVENT event, DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

**break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_GENERICCODEC\_BufferAddWrite**\(**const** DRV\_HANDLE **handle**, DRV\_GENERICCODEC\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_GENERICCODEC\_BufferAddWriteRead Function

void DRV\_GENERICCODEC\_BufferAddWriteRead

\(

const DRV\_HANDLE handle,

DRV\_GENERICCODEC\_BUFFER\_HANDLE \*bufferHandle,

void \*transmitBuffer, void \*receiveBuffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write-read operation.

**Description**

This function schedules a non-blocking write-read operation. The function returns with a valid buffer handle in the bufferHandle argument if the write-read request was scheduled successfully. The function adds the request to the hardware instance queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified.

The function returns DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read only or write only

-   if the buffer size is 0

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec device instance and the DRV\_GENERICCODEC\_Status must have returned SYS\_STATUS\_READY.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_READWRITE must have been specified in the DRV\_GENERICCODEC\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the Generic Codec instance as returned by the DRV\_GENERICCODEC\_Open function|
|bufferHandle|Pointer to an argument that will contain the return buffer handle|
|transmitBuffer|The buffer where the transmit data will be stored|
|receiveBuffer|The buffer where the received data will be stored|
|size|Buffer size in bytes|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be

DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the Generic Codec Driver Buffer Event Handler that

is registered by this client. It should not be called in the event handler associated with another Generic Codec driver instance. It should not otherwise be called directly in an ISR.

This function is useful when there is valid read expected for every Generic Codec write. The transmit and receive size must be same.

**Example**

MY\_APP\_OBJ myAppObj; uint8\_t mybufferTx; uint8\_t mybufferRx; DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle;

*// mygenericcodecHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

*// Client registers an event handler with driver*

DRV\_GENERICCODEC\_BufferEventHandlerSet\(mygenericcodecHandle,

DRV\_GENERICCODECBufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_GENERICCODEC\_BufferAddWriteRead\(mygenericcodecHandle, &bufferHandle, mybufferTx,mybufferRx,MY\_BUFFER\_SIZE\); **if**\(DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** DRV\_GENERICCODECBufferEventHandler\(DRV\_GENERICCODEC\_BUFFER\_EVENT event, DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

**break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_GENERICCODEC\_BufferAddWriteRead**\(**const** DRV\_HANDLE **handle**, DRV\_GENERICCODEC\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **transmitBuffer**, **void** \* **receiveBuffer**, size\_t **size**\);

##### DRV\_GENERICCODEC\_ReadQueuePurge Function

bool DRV\_GENERICCODEC\_ReadQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the read queue.

**Description**

This function removes all the buffer requests from the read queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_GENERICCODEC\_Open function.|

**Returns**

True - Read queue purge is successful. False - Read queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_GENERICCODEC\_Open function.*

*// Use DRV\_GENERICCODEC\_BufferAddRead to queue read requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_GENERICCODEC\_ReadQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the read queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_GENERICCODEC\_ReadQueuePurge**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_WriteQueuePurge Function

bool DRV\_GENERICCODEC\_WriteQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the write queue.

**Description**

This function removes all the buffer requests from the write queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_GENERICCODEC\_Open function.|

**Returns**

True - Write queue purge is successful. False - Write queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_GENERICCODEC\_Open function.*

*// Use DRV\_GENERICCODEC\_BufferAddWrite to queue write requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_GENERICCODEC\_WriteQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the write queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_GENERICCODEC\_WriteQueuePurge**\(**const** DRV\_HANDLE **handle**\);

#### Settings Functions

##### DRV\_GENERICCODEC\_MicGainGet Function

uint8\_t DRV\_GENERICCODEC\_MicGainGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the microphone gain for the Generic Codec.

**Description**

This functions gets the current microphone gain programmed to the Generic Codec.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

Microphone gain, in range 0-31.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t gain;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

gain = DRV\_GENERICCODEC\_MicGainGet\(myGENERICCODECHandle\);

**C**

uint8\_t **DRV\_GENERICCODEC\_MicGainGet**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_MicGainSet Function

void DRV\_GENERICCODEC\_MicGainSet\(DRV\_HANDLE handle, uint8\_t gain\)

**Summary**

This function sets the microphone gain for the Generic Codec CODEC.

**Description**

This functions sets the microphone gain value from 0-31

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|gain|Gain value, in range 0-31|

**Returns**

None.

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_MicGainSet\(myGENERICCODECHandle, 15\); *//GENERICCODEC mic gain set to 15* **Remarks**

None.

**C**

**void** **DRV\_GENERICCODEC\_MicGainSet**\(DRV\_HANDLE **handle**, uint8\_t **gain**\);

##### DRV\_GENERICCODEC\_MicMuteOff Function

void DRV\_GENERICCODEC\_MicMuteOff\(DRV\_HANDLE handle\)

**Summary**

Umutes th Generic Codec's microphone input.

**Description**

This function unmutes the Generic Codec's microphone input.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_MicMuteOff\(myGENERICCODECHandle\); *//Generic Codec microphone unmuted*

**C**

**void** **DRV\_GENERICCODEC\_MicMuteOff**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_MicMuteOn Function

void DRV\_GENERICCODEC\_MicMuteOn\(DRV\_HANDLE handle\);

**Summary**

Mutes the Generic Codec's microphone input

**Description**

This function mutes the Generic Codec's microphone input

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_MicMuteOn\(myGENERICCODECHandle\); *//Generic Codec microphone muted*

**C**

**void** **DRV\_GENERICCODEC\_MicMuteOn**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_MuteOff Function

void DRV\_GENERICCODEC\_MuteOff\(DRV\_HANDLE handle\)

**Summary**

This function disables Generic Codec output for soft mute.

**Description**

This function disables Generic Codec output for soft mute.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_MuteOff\(myGENERICCODECHandle\); *//Generic Codec output soft mute disabled*

**C**

**void** **DRV\_GENERICCODEC\_MuteOff**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_MuteOn Function

void DRV\_GENERICCODEC\_MuteOn\(DRV\_HANDLE handle\);

**Summary**

This function allows Generic Codec output for soft mute on.

**Description**

This function Enables Generic Codec output for soft mute.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_MuteOn\(myGENERICCODECHandle\); *//GENERICCODEC output soft muted*

**C**

**void** **DRV\_GENERICCODEC\_MuteOn**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_SamplingRateGet Function

uint32\_t DRV\_GENERICCODEC\_SamplingRateGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the sampling rate set on the Generic Codec.

**Description**

This function gets the sampling rate set on the DAC Generic Codec.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Remarks**

None.

**Example**

uint32\_t baudRate;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

baudRate = DRV\_GENERICCODEC\_SamplingRateGet\(myGENERICCODECHandle\);

**C**

uint32\_t **DRV\_GENERICCODEC\_SamplingRateGet**\(DRV\_HANDLE **handle**\);

##### DRV\_GENERICCODEC\_SamplingRateSet Function

void DRV\_GENERICCODEC\_SamplingRateSet\(DRV\_HANDLE handle, uint32\_t samplingRate\)

**Summary**

This function sets the sampling rate of the media stream.

**Description**

This function sets the media sampling rate for the client handle.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|samplingRate|Sampling frequency in Hz|

**Returns**

None.

**Remarks**

None.

**Example**

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_SamplingRateSet\(myGENERICCODECHandle, 48000\); *//Sets 48000 media sampling rate*

**C**

**void** **DRV\_GENERICCODEC\_SamplingRateSet**\(DRV\_HANDLE **handle**, uint32\_t **samplingRate**\);

##### DRV\_GENERICCODEC\_VersionGet Function

uint32\_t DRV\_GENERICCODEC\_VersionGet\( void \)

**Summary**

This function returns the version of Generic Codec driver

**Description**

The version number returned from the DRV\_GENERICCODEC\_VersionGet function is an unsigned integer in the following decimal format. \* 10000 + \* 100 + Where the numbers are represented in decimal and the meaning is the same as above. Note that there is no numerical representation of release type.

**Preconditions**

None.

**Returns**

returns the version of Generic Codec driver.

**Remarks**

None.

**Example 1**

For version "0.03a", return: 0 \* 10000 + 3 \* 100 + 0 For version "1.00", return: 1 \* 100000 + 0 \* 100 + 0

**Example 2** uint32\_t GENERICCODECversion; GENERICCODECversion = DRV\_GENERICCODEC\_VersionGet\(\);

**C**

uint32\_t **DRV\_GENERICCODEC\_VersionGet**\(\);

##### DRV\_GENERICCODEC\_VersionStrGet Function

int8\_t\* DRV\_GENERICCODEC\_VersionStrGet\(void\)

**Summary**

This function returns the version of Generic Codec driver in string format.

**Description**

The DRV\_GENERICCODEC\_VersionStrGet function returns a string in the format: "." Where: is the Generic Codec driver's version number. is the Generic Codec driver's version number. is an optional "patch" or "dot" release number \(which is not included in the string if it equals "00"\). is an optional release type \("a" for alpha, "b" for beta ? not the entire word spelled out\) that is not included if the release is a production version \(I.e. Not an alpha or beta\).

The String does not contain any spaces. For example, "0.03a" "1.00"

**Preconditions**

None.

**Returns**

returns a string containing the version of Generic Codec driver.

**Remarks**

None

**Example**

int8\_t \*GENERICCODECstring; GENERICCODECstring = DRV\_GENERICCODEC\_VersionStrGet\(\);

**C**

int8\_t\* **DRV\_GENERICCODEC\_VersionStrGet**\(\);

##### DRV\_GENERICCODEC\_VolumeGet Function

uint8\_t DRV\_GENERICCODEC\_VolumeGet\(DRV\_HANDLE handle, DRV\_GENERICCODEC\_CHANNEL channel\)

**Summary**

This function gets the volume for Generic Codec.

**Description**

This functions gets the current volume programmed to the Generic Codec.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|channel|argument indicating Left or Right or Both channel volume to be modified|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t volume;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

volume = DRV\_GENERICCODEC\_VolumeGet\(myGENERICCODECHandle, DRV\_GENERICCODEC\_CHANNEL\_LEFT\);

**C**

uint8\_t **DRV\_GENERICCODEC\_VolumeGet**\(DRV\_HANDLE **handle**, DRV\_GENERICCODEC\_CHANNEL **channel**\);

##### DRV\_GENERICCODEC\_VolumeSet Function

void DRV\_GENERICCODEC\_VolumeSet\(DRV\_HANDLE handle, DRV\_GENERICCODEC\_CHANNEL channel, uint8\_t volume\);

**Summary**

This function sets the volume for Generic Codec.

**Description**

This functions sets the volume value from 0-255.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|channel|argument indicating Left or Right or Both channel volume to be modified|
|volume|volume value specified in the range 0-255 \(0x00 to 0xFF\)|

**Returns**

None

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myGENERICCODECHandle is the handle returned // by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_VolumeSet\(myGENERICCODECHandle,DRV\_GENERICCODEC\_CHANNEL\_LEFT, 120\);

**C**

**void** **DRV\_GENERICCODEC\_VolumeSet**\(DRV\_HANDLE **handle**, DRV\_GENERICCODEC\_CHANNEL **channel**, uint8\_t **volume**\);

#### Other Functions

##### DRV\_GENERICCODEC\_GetI2SDriver Function

DRV\_HANDLE DRV\_GENERICCODEC\_GetI2SDriver\(DRV\_HANDLE codecHandle\)

**Summary**

Get the handle to the I2S driver for this codec instance.

**Description**

Returns the appropriate handle to the I2S based on the ioIent member of the codec object.

**Preconditions**

The DRV\_GENERICCODEC\_Initialize routine must have been called for the specified Generic Codec driver instance.

DRV\_GENERICCODEC\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

-   A handle to the I2S driver for this codec instance


**Remarks**

This allows the caller to directly access portions of the I2S driver that might not be available via the codec API.

**C**

DRV\_HANDLE **DRV\_GENERICCODEC\_GetI2SDriver**\(DRV\_HANDLE **codecHandle**\);

##### DRV\_GENERICCODEC\_LRCLK\_Sync Function

uint32\_t DRV\_GENERICCODEC\_LRCLK\_Sync \(const DRV\_HANDLE handle\);

**Summary**

Synchronize to the start of the I2S LRCLK \(left/right clock\) signal

**Description**

This function waits until low-to high transition of the I2S LRCLK \(left/right clock\) signal \(high-low if Left-Justified format, this is determined by the PLIB\). In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize calls to the DMA with the LRCLK signal so the left/right channel association is valid.

**Preconditions**

None.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myGENERICCODECHandle is the handle returned* *// by the DRV\_GENERICCODEC\_Open function.*

DRV\_GENERICCODEC\_LRCLK\_Sync\(myGENERICCODECHandle\);

**C**

**bool** **DRV\_GENERICCODEC\_LRCLK\_Sync**\(**const** DRV\_HANDLE **handle**\);

**Dara Types and Constants**

## *DRV\_GENERICCODEC\_BUFFER\_EVENT Type*

Identifies the possible events that can result from a buffer add request.

**Description**

Generic Codec Driver Events

This enumeration identifies the possible events that can result from a buffer add request caused by the client calling either the DRV\_GENERICCODEC\_BufferAddWrite\(\) or the DRV\_GENERICCODEC\_BufferAddRead\(\) function.

**Remarks**

One of these values is passed in the "event" parameter of the event handling callback function that the client registered with the driver by calling the DRV\_GENERICCODEC\_BufferEventHandlerSet function when a buffer transfer request is completed.

**C**

**typedef** **enum** DRV\_GENERICCODEC\_BUFFER\_EVENT@1 **DRV\_GENERICCODEC\_BUFFER\_EVENT**;

## *DRV\_GENERICCODEC\_BUFFER\_EVENT\_HANDLER Type*

Pointer to a Generic Codec Driver Buffer Event handler function

**Description**

Generic Codec Driver Buffer Event Handler Function

This data type defines the required function signature for the Generic Codec driver buffer event handling callback function. A client must register a pointer to a buffer event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive buffer related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|event|Identifies the type of event|
|bufferHandle|Handle identifying the buffer to which the event relates|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

If the event is DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE, this means that the data was transferred successfully.

If the event is DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR, this means that the data was not transferred successfully.

The bufferHandle parameter contains the buffer handle of the buffer that failed. The

DRV\_GENERICCODEC\_BufferProcessedSizeGet\(\) function can be called to find out how many bytes were processed.

The bufferHandle parameter contains the buffer handle of the buffer that associated with the event.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_GENERICCODEC\_BufferEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The buffer handle in bufferHandle expires after this event handler exits. In that the buffer object that was allocated is deallocated by the driver after the event handler exits.

The event handler function executes in the data driver\(i2S\) peripheral's interrupt context when the driver is configured for interrupt mode operation. It is recommended of the application to not perform process intensive or blocking operations with in this function.

DRV\_GENERICCODEC\_BufferAddWrite function can be called in the event handler to add a buffer to the driver queue.

**Example**

**void** DRV\_GENERICCODECBufferEventHandler\( DRV\_GENERICCODEC\_BUFFER\_EVENT event, DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle, uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context; **switch**\(event\) \{ **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_COMPLETE: *// Handle the completed buffer.* **break**; **case** DRV\_GENERICCODEC\_BUFFER\_EVENT\_ERROR: **default**: *// Handle error.* **break**; \}

\}

**C**

**typedef** **void** \(\* **DRV\_GENERICCODEC\_BUFFER\_EVENT\_HANDLER**\)\(DRV\_GENERICCODEC\_BUFFER\_EVENT event, DRV\_GENERICCODEC\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\);

## *DRV\_GENERICCODEC\_BUFFER\_HANDLE Type*

Handle identifying a write buffer passed to the driver.

**Description**

Generic Codec Driver Buffer Handle

A buffer handle value is returned by a call to the DRV\_GENERICCODEC\_BufferAddWrite\(\) or

DRV\_GENERICCODEC\_BufferAddRead\(\) function. This handle is associated with the buffer passed into the function and it allows the application to track the completion of the data from \(or into\) that buffer.

The buffer handle value returned from the "buffer add" function is returned back to the client by the "event handler callback" function registered with the driver.

The buffer handle assigned to a client request expires when the client has been notified of the completion of the buffer transfer \(after event handler function that notifies the client returns\) or after the buffer has been retired by the driver if no event handler callback was set.

**Remarks**

None

**C**

**typedef** uintptr\_t **DRV\_GENERICCODEC\_BUFFER\_HANDLE**;

## *DRV\_GENERICCODEC\_CHANNEL Type*

Identifies Left/Right Audio channel

**Description**

Generic Codec Audio Channel

This enumeration identifies Left/Right Audio channel

**Remarks**

None.

**C**

**typedef** **enum** DRV\_GENERICCODEC\_CHANNEL@1 **DRV\_GENERICCODEC\_CHANNEL**;

## *DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER Type*

Pointer to a Generic Codec Driver Command Event Handler Function

**Description**

Generic Codec Driver Command Event Handler Function

This data type defines the required function signature for the Generic Codec driver command event handling callback function.

A command is a control instruction to the Generic Codec. Example Mute ON/OFF, Zero Detect Enable/Disable etc.

A client must register a pointer to a command event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive command related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

The occurrence of this call back means that the last control command was transferred successfully.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_GENERICCODEC\_CommandEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The event handler function executes in the control data driver interrupt context. It is recommended of the application to not perform process intensive or blocking operations with in this function.

**Example**

**void** DRV\_GENERICCODECCommandEventHandler\( uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context;

*// Last Submitted command is completed.*

*// Perform further processing here*

\}

**C**

**typedef** **void** \(\* **DRV\_GENERICCODEC\_COMMAND\_EVENT\_HANDLER**\)\(uintptr\_t contextHandle\);

## *DRV\_GENERICCODEC\_INIT Type*

Defines the data required to initialize or reinitialize the Generic Codec driver

**Description**

Generic Codec Driver Initialization Data

This data type defines the data required to initialize or reinitialize the Generic Codec driver.

**Remarks**

None.

**C**

**typedef** **struct** DRV\_GENERICCODEC\_INIT@1 **DRV\_GENERICCODEC\_INIT**;

## *DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID Macro*

Definition of an invalid buffer handle.

**Description**

Generic Codec Driver Invalid Buffer Handle

This is the definition of an invalid buffer handle. An invalid buffer handle is returned by DRV\_GENERICCODEC\_BufferAddWrite\(\) and the DRV\_GENERICCODEC\_BufferAddRead\(\) function if the buffer add request was not successful.

**Remarks**

None.

**C**

**\#define** **DRV\_GENERICCODEC\_BUFFER\_HANDLE\_INVALID** \(\(DRV\_GENERICCODEC\_BUFFER\_HANDLE\)\(-1\)\)

## *DRV\_GENERICCODEC\_COUNT Macro*

Number of valid Generic Codec driver indices

**Description**

Generic Codec Driver Module Count

This constant identifies the maximum number of Generic Codec Driver instances that should be defined by the application. Defining more instances than this constant will waste RAM memory space.

This constant can also be used by the application to identify the number of Generic Codec instances on this microcontroller.

**Remarks**

This value is part-specific.

**C**

**\#define** **DRV\_GENERICCODEC\_COUNT**

### DRV\_GENERICCODEC\_INDEX\_0 Macro

Generic Codec driver index definitions

**Description**

Driver Generic Codec Module Index

These constants provide Generic Codec driver index definition.

**Remarks**

These constants should be used in place of hard-coded numeric literals. These values should be passed into the DRV\_GENERICCODEC\_Initialize and DRV\_GENERICCODEC\_Open routines to identify the driver instance in use.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_0** 0

## *DRV\_GENERICCODEC\_INDEX\_1 Macro*

This is macro DRV\_GENERICCODEC\_INDEX\_1.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_1** 1

## *DRV\_GENERICCODEC\_INDEX\_2 Macro*

This is macro DRV\_GENERICCODEC\_INDEX\_2.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_2** 2

## *DRV\_GENERICCODEC\_INDEX\_3 Macro*

This is macro DRV\_GENERICCODEC\_INDEX\_3.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_3** 3

## *DRV\_GENERICCODEC\_INDEX\_4 Macro*

This is macro DRV\_GENERICCODEC\_INDEX\_4.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_4** 4

## *DRV\_GENERICCODEC\_INDEX\_5 Macro*

This is macro DRV\_GENERICCODEC\_INDEX\_5.

**C**

**\#define** **DRV\_GENERICCODEC\_INDEX\_5** 5

### DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT Type

Identifies the Serial Audio data interface format.

**Description**

Generic Codec Audio data format

This enumeration identifies Serial Audio data interface format.

**C**

**typedef** **enum** DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT@1 **DRV\_GENERICCODEC\_AUDIO\_DATA\_FORMAT**;

### Files

**Files**

|**Name**|**Description**|
|--------|---------------|
|drv\_genericcodec.h|Generic Codec Driver Interface header file|
|drv\_genericcodec\_config\_template.h|Generic Codec Driver Configuration Template.|

**Description**

### drv\_genericcodec.h

drv\_genericcodec.h

**Summary**

Generic Codec Driver Interface header file

**Description**

Generic Codec Driver Interface

The Generic Codec device driver interface provides a simple interface to manage a codec that can be interfaced to a Microchip microcontroller. This file provides the public interface definitions for the Generic Codec device driver.

### drv\_genericcodec\_config\_template.h

drv\_genericcodec\_config\_template.h

**Summary**

Generic Codec Driver Configuration Template.

**Description**

Generic Codec Driver Configuration Template

These file provides the list of all the configurations that can be used with the driver. This file should not be included in the driver.

**WM8904 CODEC Driver Library Help**

This topic describes the WM8904 Codec Driver Library.

## *Introduction*

This library provides an Applications Programming Interface \(API\) to manage the WM8904 Codec that is serially interfaced to the I2C and I2S peripherals of a Microchip microcontroller for the purpose of providing audio solutions.

**Description**

The WM8904 module is 24-bit Audio Codec from Cirrus Logic, which can operate in 16-, 20-, 24-, and 32-bit audio modes. The WM8904 can be interfaced to Microchip microcontrollers through I2C and I2S serial interfaces. The I2C interface is used to send commands and receive status, and the I2S interface is used for audio data output \(to headphones or line-out\) and input \(from microphone or line-in\).

The WM8904 can be configured as either an I2S clock slave \(receives all clocks from the host\), or I2S clock master \(generates I2S clocks from a master clock input MCLK\). Currently the driver only supports master mode with headphone output and \(optionally\) microphone input.

A typical interface of WM8904 to a Microchip microcontroller using an I2C and SSC interface \(configured as I2S\), with the WM8904 set up as the I2S clock master, is provided in the following diagram: ![](GUID-2299F263-3B83-46C3-8B94-8535A4E6B104-low.png)

The WM8904 Codec supports the following features:

-   Audio Interface Format: 16-/20-/24-/32-bit interface, LSB justified or I2S format \(only 16 and 32-bit interfaces supported in the driver\)

-   Sampling Frequency Range: 8 kHz to 96 kHz

-   Digital Volume Control: -71.625 to 0 dB in 192 steps \(converted to a linear scale 0-255 in the driver\)

-   Soft mute capability


## *Using the Library*

This topic describes the basic architecture of the WM8904 Codec Driver Library and provides information and examples on its use.

**Description**

### Interface Header File: drv\_WM8904.h

The interface to the WM8904 Codec Driver library is defined in the audio/driver/codec/WM8904/drv\_WM8904.h header file.

Any C language source \(.c\) file that uses the WM8904 Codec Driver library should include this header.

**Library Source Files:**

The WM8904 Codec Driver library source files are provided in the audio/driver/codec/WM8904/src directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features and to **Building the Library** for instructions on how to build the library.

**Example Applications:**

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/audio\_tone\_linkeddma

-   audio/apps/microphone\_loopback


## Abstraction Model

This library provides a low-level abstraction of the WM8904 Codec Driver Library on the Microchip family microcontrollers with a convenient C language interface. This topic describes how that abstraction is modeled in software and introduces the library's interface.

**Description**

The abstraction model shown in the following diagram depicts how the WM8904 Codec Driver is positioned in the MPLAB Harmony framework. The WM8904 Codec Driver uses the I2C and I2S drivers for control and audio data transfers to the WM8904 module.

### WM8904 Driver Abstraction Model

![](GUID-FB7447CC-3095-4365-B789-3FF239D5A9CF-low.png)

## Library Overview

Refer to the Driver Library Overview section for information on how the driver operates in a system.

The WM8904 Codec Driver Library provides an API interface to transfer control commands and digital audio data to the serially interfaced WM8904 Codec module. The library interface routines are divided into various sub-sections, which address one of the blocks or the overall operation of the WM8904 Codec Driver Library.

|**Library Interface Section**|**Description**|
|-----------------------------|---------------|
|System Functions|Provides system module interfaces, device initialization, deinitialization, reinitialization, tasks and status functions.|
|Client Setup Functions|Provides open and close functions.|
|Data Transfer Functions|Provides data transfer functions, such as Buffer Read and Write.|
|Settings Functions|Provides driver specific functions for settings, such as volume control and sampling rate.|
|Other Functions|Miscellaneous functions, such as getting the driver’s version number and syncing to the LRCLK signal.|
|Data Types and Constants|These data types and constants are required while interacting and setting up the WM8904 Codec Driver Library.|

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** All functions and constants in this section are named with the format DRV\_ WM8904\_xxx, where 'xxx' is a function name or constant. These names are redefined in the appropriate configuration’s configuration.h file to the format DRV\_CODEC\_xxx using \#defines so that code in the application that references the library can be written as generically as possible \(e.g., by writing DRV\_CODEC\_Open instead of DRV\_ WM8904\_Open etc.\). This allows the codec type to be changed in the MHC without having to modify the application’s source code.

## How the Library Works

How the Library Works

The library provides interfaces to support:

-   System Functionality

-   Client Functionality


### Setup \(Initialization\)

This topic describes system initialization, implementations, and includes a system access code example.

**Description**

## System Initialization

The system performs the initialization of the device driver with settings that affect only the instance of the device that is being initialized. During system initialization in the system\_init.c file, each instance of the WM8904 module would be initialized with the following configuration settings \(either passed dynamically at run time using DRV\_WM8904\_INIT or by using Initialization Overrides\) that are supported by the specific WM8904 device hardware:

-   Device requested power state: one of the System Module Power States. For specific details please refer to Data Types and Constants in the Library Interface section.

-   I2C driver module index. The module index should be same as the one used in initializing the I2C Driver

-   I2S driver module index. The module index should be same as the one used in initializing the I2S Driver

-   Sampling rate

-   Volume

-   Audio data format. The audio data format should match with the audio data format settings done in I2S driver initialization

-   Determines whether or not the microphone input is enabled


The DRV\_WM8904\_Initialize API returns an object handle of the type SYS\_MODULE\_OBJ. The object handle returned by the Initialize interface would be used by the other system interfaces such as DRV\_ WM8904\_Deinitialize, DRV\_ WM8904\_Status and DRV\_I2S\_Tasks.

### Client Access

This topic describes driver initialization and provides a code example.

**Description**

For the application to start using an instance of the module, it must call the DRV\_WM8904\_Open function. The

DRV\_WM8904\_Open function provides a driver handle to the WM8904 Codec Driver instance for operations. If the driver is deinitialized using the function DRV\_WM8904\_Deinitialize, the application must call the DRV\_WM8904\_Open function again to set up the instance of the driver.

For the various options available for IO\_INTENT, please refer to Data Types and Constants in the Library Interface section.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is necessary to check the status of driver initialization before opening a driver instance. The status of the WM8904 Codec Driver can be known by calling DRV\_ WM8904\_Status.

**Example:**

DRV\_HANDLE handle; SYS\_STATUS wm8904Status;

wm8904Status Status = DRV\_WM8904\_Status\(sysObjects.wm8904Status DevObject\); if \(SYS\_STATUS\_READY == wm8904Status\) \{ // The driver can now be opened. appData.wm8904Client.handle = DRV\_WM8904\_Open

\( DRV\_WM8904\_INDEX\_0, DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_EXCLUSIVE\); if\(appData.wm8904Client.handle != DRV\_HANDLE\_INVALID\) \{

appData.state = APP\_STATE\_WM8904\_SET\_BUFFER\_HANDLER; \} else \{ SYS\_DEBUG\(0, "Find out what's wrong \\r\\n"\);

\} \} else \{ /\* WM8904 Driver Is not ready \*/

\}

### Client Operations

This topic provides information on client operations.

**Description**

Client operations provide the API interface for control command and audio data transfer to the WM8904 Codec.

The following WM8904 Codec specific control command functions are provided:

-   DRV\_WM8904\_SamplingRateSet

-   DRV\_WM8904\_SamplingRateGet

-   DRV\_WM8904\_VolumeSet

-   DRV\_WM8904\_VolumeGet

-   DRV\_WM8904\_MuteOn

-   DRV\_WM8904\_MuteOff


These functions schedule a non-blocking control command transfer operation. These functions submit the control command request to the WM8904 Codec. These functions submit the control command request to I2C Driver transmit queue, the request is processed immediately if it is the first request, or processed when the previous request is complete.

DRV\_WM8904\_BufferAddWrite, DRV\_WM8904\_BufferAddRead, and DRV\_WM8904\_BufferAddWriteRead are buffered data operation functions. These functions schedule non-blocking audio data transfer operations. These functions add the request to I2S Driver transmit or receive buffer queue depends on the request type, and are executed immediately if it is the first buffer, or executed later when the previous buffer is complete. The driver notifies the client with

DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE, DRV\_WM8904\_BUFFER\_EVENT\_ERROR, or DRV\_WM8904\_BUFFER\_EVENT\_ABORT events.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is not necessary to close and reopen the client between multiple transfers.

### Configuring the Library

The configuration of the I2S Driver Library is based on the file configurations.h, as generated by the MHC.

This header file contains the configuration selection for the I2S Driver Library. Based on the selections made, the I2S Driver Library may support the selected features. These configuration settings will apply to all instances of the I2S Driver Library.

This header can be placed anywhere; however, the path of this header needs to be present in the include search path for a successful build. Refer to the Applications Help section for more details.

### System Configuration

**Macros**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_WM8904\_AUDIO\_DATA\_FORMAT\_MACRO|Specifies the audio data format for the codec.|
|-|DRV\_WM8904\_AUDIO\_SAMPLING\_RATE|Specifies the initial baud rate for the codec.|
|-|DRV\_WM8904\_CLIENTS\_NUMBER|Sets up the maximum number of clients that can be connected to any hardware instance.|
|-|DRV\_WM8904\_ENABLE\_MIC\_BIAS|Specifies whether to enable the microphone bias.|
|-|DRV\_WM8904\_ENABLE\_MIC\_INPUT|Specifies whether to enable the microphone input.|
|-|DRV\_WM8904\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2C interface.|
|-|DRV\_WM8904\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx|Specifies the instance number of the I2S interface.|
|-|DRV\_WM8904\_INSTANCES\_NUMBER|Sets up the maximum number of hardware instances that can be supported|
|-|DRV\_WM8904\_MASTER\_MODE|Specifies if codec is in Master or Slave mode.|
|-|DRV\_WM8904\_VOLUME|Specifies the initial volume level.|

**Description**

## *DRV\_WM8904\_AUDIO\_DATA\_FORMAT\_MACRO Macro*

Specifies the audio data format for the codec.

**Description**

WM8904 Audio Data Format

Sets up the length of each sample plus the format \(I2S or left-justified\) for the audio.

Valid choices are: "DATA\_16\_BIT\_LEFT\_JUSTIFIED" "DATA\_16\_BIT\_I2S" "DATA\_32\_BIT\_LEFT\_JUSTIFIED" "DATA\_32\_BIT\_I2S"

**Remarks**

If 24-bit audio is needed, it should be sent, left-justified, in a 32-bit format.

**C**

**\#define** **DRV\_WM8904\_AUDIO\_DATA\_FORMAT\_MACRO**

## *DRV\_WM8904\_AUDIO\_SAMPLING\_RATE Macro*

Specifies the initial baud rate for the codec.

**Description**

WM8904 Baud Rate

Sets the initial baud rate \(sampling rate\) for the codec. Typical values are 8000, 16000, 44100, 48000, 88200 and 96000.

**C**

**\#define** **DRV\_WM8904\_AUDIO\_SAMPLING\_RATE**

## *DRV\_WM8904\_CLIENTS\_NUMBER Macro*

Sets up the maximum number of clients that can be connected to any hardware instance.

**Description**

WM8904 Client Count Configuration

Sets up the maximum number of clients that can be connected to any hardware instance. Typically only one client could be connected to one hardware instance. This value represents the total number of clients to be supported across all hardware instances.

**C**

**\#define** **DRV\_WM8904\_CLIENTS\_NUMBER**

## *DRV\_WM8904\_ENABLE\_MIC\_BIAS Macro*

Specifies whether to enable the microphone bias.

**Description**

WM8904 Microphone Enable

Indicates whether the bias voltage needed for electret microphones should be enabled.

**C**

**\#define** **DRV\_WM8904\_ENABLE\_MIC\_BIAS**

## *DRV\_WM8904\_ENABLE\_MIC\_INPUT Macro*

Specifies whether to enable the microphone input.

**Description**

WM8904 Microphone Enable

Indicates whether the ADC inputs for the two microphone channels \(L-R\) should be enabled.

**C**

**\#define** **DRV\_WM8904\_ENABLE\_MIC\_INPUT**

## *DRV\_WM8904\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2C interface.

**Description**

WM8904 I2C instance number

Specifies the instance number of the I2C interface being used by the MCU to send commands and receive status to and from the WM8904. enabled.

**C**

**\#define** **DRV\_WM8904\_I2C\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_WM8904\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx Macro*

Specifies the instance number of the I2S interface.

**Description**

WM8904 I2S instance number

Specifies the instance number of the I2S interface being used by the MCU to send and receive audio data to and from the WM8904. enabled.

**C**

**\#define** **DRV\_WM8904\_I2S\_DRIVER\_MODULE\_INDEX\_IDXx**

## *DRV\_WM8904\_INSTANCES\_NUMBER Macro*

Sets up the maximum number of hardware instances that can be supported

**Description**

WM8904 driver objects configuration

Sets up the maximum number of hardware instances that can be supported. It is recommended that this number be set exactly equal to the number of WM8904 Codec modules that are needed by an application, namely one.

**C**

**\#define** **DRV\_WM8904\_INSTANCES\_NUMBER**

### DRV\_WM8904\_MASTER\_MODE Macro

Specifies if codec is in Master or Slave mode.

**Description**

WM8904 Codec Master/Slave Mode

Indicates whether the codec is to be operating in a Master mode \(generating word and bit clock as outputs\) or Slave mode receiving word and bit clock as inputs\).

**C**

**\#define** **DRV\_WM8904\_MASTER\_MODE**

### DRV\_WM8904\_VOLUME Macro

Specifies the initial volume level.

**Description**

WM8904 Volume

Sets the initial volume level, in the range 0-255.

**Remarks**

The value is mapped to an internal WM8904 volume level in the range 0-192 using a logarithmic table so the input scale appears linear \(128 is half volume\).

**C**

**\#define** **DRV\_WM8904\_VOLUME**

### Configuring MHC

Provides examples on how to configure the MPLAB Harmony Configurator \(MHC\) for a specific driver.

**Description**

When building a new application, start by creating a 32-bit MPLAB Harmony 3 project in MPLAB X IDE by selecting *File \> New Project*. Chose the Configuration name the based on the BSP, and select the appropriate processor \(such as ATSAME70Q21B\).

In the MHC, under Available Components select the appropriate BSP, such as SAM E70 Xplained Ultra. Under *Audio\>Templates*, double-click on a codec template such as WM8904. Answer Yes to all questions.

You should end up with a project graph that looks like this, after rearranging the boxes: ![](GUID-FDF866A7-5FBB-4DA9-AA7E-5F6135618110-low.png)

Click on the WM8904 Driver component \(not WM8904 Codec\) and the following menu will be displayed in the Configurations Options: ![](GUID-940559E6-52E1-49CF-BE6D-284BA3AE560C-low.png)

-   **I2C Driver Used** will display the driver instance used for the I2C interface.

-   **I2S Driver Used** will display the driver instance used for the I2S interface.

-   **Usage Mode** indicates whether the WM8904 is a Master \(supplies I2S clocks\) or a Slave \(MCU supplies I2S clocks\).

-   **Number of WM8904 Clients** indicates the maximum number of clients that can be connected to the WM8904 Driver.

-   **Sampling Rate** indicates the number of samples per second per channel, 8000 to 96,000.

-   **Volume** indicates the volume in a linear scale from 0-255.

-   **Audio Data Format** is either 16-bit Left Justified, 16-bit I2S, 32-bit Left Justified, or 32-bit I2S. It must match the audio protocol and data length set up in either the SSC or I2S PLIB.

-   **Sampling Rate** indicates the number of samples per second per channel, 8000 to 96,000.

-   **Enable Microphone Input** should be checked a microphone is being used. If checked, another option,

-   **Enable Microphone Bias** should be checked if using an electret microphone.


You can also bring in the WM8904 Driver by itself, by double clicking WM8904 under Audio\_-\>Driver-\>Codec\_ in the Available Components list. You will then need to add any additional needed components manually and connect them together.

Note that the WM8904 requires the TCx Peripheral Library and TIME System Service in order to perform some of its internal timing sequences.

### Building the Library

This section lists the files that are available in the WM8904 Codec Driver Library.

**Description**

This section lists the files that are available in the src folder of the WM8904 Codec Driver. It lists which files need to be included in the build based on either a hardware feature present on the board or configuration option selected by the system.

The following three tables list and describe the header \(.h\) and source \(.c\) files that implement this library. The parent folder for these files is audio/driver/codec/WM8904.

### Interface File\(s\)

This table lists and describes the header files that must be included \(i.e., using \#include\) by any code that uses this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|drv\_wm8904.h|Header file that exports the driver API.|

### Required File\(s\)

![](GUID-B924D07A-0D74-453B-BCDE-8190A195C55D-low.png) ***All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use.***

This table lists and describes the source and header files that must *always* be included in the MPLAB X IDE project to build this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|/src/drv\_wm8904.c|This file contains implementation of the WM8904 Codec Driver.|

### Optional File\(s\)

This table lists and describes the source and header files that may optionally be included if required for the desired implementation.

|**Source File Name**|**Description**|
|--------------------|---------------|
|N/A|No optional files are available for this library.|

### Module Dependencies

The WM8904 Codec Driver Library depends on the following modules:

-   I2S Driver Library

-   I2C Driver Library


#### *Library Interface*

**Client Setup Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_WM8904\_Open|Opens the specified WM8904 driver instance and returns a handle to it|
||DRV\_WM8904\_Close|Closes an opened-instance of the WM8904 driver|
||DRV\_WM8904\_BufferEventHandlerSet|This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.|
||DRV\_WM8904\_CommandEventHandlerSet|This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.|

**Data Transfer Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_WM8904\_BufferAddRead|Schedule a non-blocking driver read operation.|
||DRV\_WM8904\_BufferAddWrite|Schedule a non-blocking driver write operation.|
||DRV\_WM8904\_BufferAddWriteRead|Schedule a non-blocking driver write-read operation.|
||DRV\_WM8904\_ReadQueuePurge|Removes all buffer requests from the read queue.|
||DRV\_WM8904\_WriteQueuePurge|Removes all buffer requests from the write queue.|

**Data Types and Constants**

|-|**Name**|**Description**|
|--|--------|---------------|
|-|DATA\_LENGTH|in bits|
|-|DRV\_WM8904\_AUDIO\_DATA\_FORMAT|Identifies the Serial Audio data interface format.|
|-|DRV\_WM8904\_BUFFER\_EVENT|Identifies the possible events that can result from a buffer add request.|
|-|DRV\_WM8904\_BUFFER\_EVENT\_HANDLER|Pointer to a WM8904 Driver Buffer Event handler function|
|-|DRV\_WM8904\_BUFFER\_HANDLE|Handle identifying a write buffer passed to the driver.|
|-|DRV\_WM8904\_CHANNEL|Identifies Left/Right Audio channel|
|-|DRV\_WM8904\_COMMAND\_EVENT\_HANDLER|Pointer to a WM8904 Driver Command Event Handler Function|
|-|DRV\_WM8904\_INIT|Defines the data required to initialize or reinitialize the WM8904 driver|
|-|DRV\_WM8904\_BUFFER\_HANDLE\_INVALID|Definition of an invalid buffer handle.|
|-|DRV\_WM8904\_COUNT|Number of valid WM8904 driver indices|
|-|DRV\_WM8904\_INDEX\_0|WM8904 driver index definitions|
|-|DRV\_I2C\_INDEX|This is macro DRV\_I2C\_INDEX.|

**Other Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_WM8904\_GetI2SDriver|Get the handle to the I2S driver for this codec instance.|
||DRV\_WM8904\_VersionGet|This function returns the version of WM8904 driver|
||DRV\_WM8904\_VersionStrGet|This function returns the version of WM8904 driver in string format.|
||DRV\_WM8904\_LRCLK\_Sync|Synchronize to the start of the I2S LRCLK \(left/right clock\) signal|

**Settings Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_WM8904\_MuteOff|This function disables WM8904 output for soft mute.|
||DRV\_WM8904\_MuteOn|This function allows WM8904 output for soft mute on.|
||DRV\_WM8904\_SamplingRateGet|This function gets the sampling rate set on the WM8904.|
||DRV\_WM8904\_SamplingRateSet|This function sets the sampling rate of the media stream.|
||DRV\_WM8904\_VolumeGet|This function gets the volume for WM8904 Codec.|
||DRV\_WM8904\_VolumeSet|This function sets the volume for WM8904 Codec.|

**System Interaction Functions**

|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_WM8904\_Initialize|Initializes hardware and data for the instance of the WM8904 DAC module|
||DRV\_WM8904\_Deinitialize|Deinitializes the specified instance of the WM8904 driver module|
||DRV\_WM8904\_Status|Gets the current status of the WM8904 driver module.|
||DRV\_WM8904\_Tasks|Maintains the driver's control and data interface state machine.|

#### System Interaction Functions

##### DRV\_WM8904\_Initialize Function

SYS\_MODULE\_OBJ DRV\_WM8904\_Initialize

\(

const SYS\_MODULE\_INDEX drvIndex, const SYS\_MODULE\_INIT \*const init

\);

**Summary**

Initializes hardware and data for the instance of the WM8904 DAC module

**Description**

This routine initializes the WM8904 driver instance for the specified driver index, making it ready for clients to open and use it. The initialization data is specified by the init parameter. The initialization may fail if the number of driver objects allocated are insufficient or if the specified driver instance is already initialized.

**Preconditions**

DRV\_I2S\_Initialize must be called before calling this function to initialize the data interface of this Codec driver. DRV\_I2C\_Initialize must be called if SPI driver is used for handling the control interface of this Codec driver.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the driver instance to be initialized|
|init|Pointer to the data structure containing any data necessary to initialize the hardware. This pointer may be null if no data is required and default initialization is to be used.|

**Returns**

If successful, returns a valid handle to a driver instance object. Otherwise, it returns SYS\_MODULE\_OBJ\_INVALID.

**Remarks**

This routine must be called before any other WM8904 routine is called.

This routine should only be called once during system initialization unless DRV\_WM8904\_Deinitialize is called to deinitialize the driver instance. This routine will NEVER block for hardware access.

**Example**

DRV\_WM8904\_INIT init; SYS\_MODULE\_OBJ objectHandle;

init-\>inUse = **true**; init-\>status = SYS\_STATUS\_BUSY; init-\>numClients = 0;

init-\>i2sDriverModuleIndex = wm8904Init-\>i2sDriverModuleIndex; init-\>i2cDriverModuleIndex = wm8904Init-\>i2cDriverModuleIndex; init-\>samplingRate = DRV\_WM8904\_AUDIO\_SAMPLING\_RATE; init-\>audioDataFormat = DRV\_WM8904\_AUDIO\_DATA\_FORMAT\_MACRO;

init-\>isInInterruptContext = **false**; init-\>commandCompleteCallback = \(DRV\_WM8904\_COMMAND\_EVENT\_HANDLER\)0; init-\>commandContextData = 0; init-\>mclk\_multiplier = DRV\_WM8904\_MCLK\_SAMPLE\_FREQ\_MULTPLIER; objectHandle = DRV\_WM8904\_Initialize\(DRV\_WM8904\_0, \(SYS\_MODULE\_INIT\*\)init\); **if** \(SYS\_MODULE\_OBJ\_INVALID == objectHandle\) \{

*// Handle error*

\}

**C**

SYS\_MODULE\_OBJ **DRV\_WM8904\_Initialize**\(**const** SYS\_MODULE\_INDEX **drvIndex**, **const** SYS\_MODULE\_INIT \* **const** **init**\);

##### DRV\_WM8904\_Deinitialize Function

void DRV\_WM8904\_Deinitialize\( SYS\_MODULE\_OBJ object\)

**Summary**

Deinitializes the specified instance of the WM8904 driver module

**Description**

Deinitializes the specified instance of the WM8904 driver module, disabling its operation \(and any hardware\). Invalidates all the internal data.

**Preconditions**

Function DRV\_WM8904\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_WM8904\_Initialize routine|

**Returns**

None.

**Remarks**

Once the Initialize operation has been called, the De-initialize operation must be called before the Initialize operation can be called again. This routine will NEVER block waiting for hardware.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_WM8904\_Initialize*

SYS\_STATUS status;

DRV\_WM8904\_Deinitialize\(object\); status = DRV\_WM8904\_Status\(object\); **if** \(SYS\_MODULE\_DEINITIALIZED != status\) \{

*// Check again later if you need to know* *// when the driver is deinitialized.*

\}

**C**

**void** **DRV\_WM8904\_Deinitialize**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_WM8904\_Status Function

SYS\_STATUS DRV\_WM8904\_Status\( SYS\_MODULE\_OBJ object\)

**Summary**

Gets the current status of the WM8904 driver module.

**Description**

This routine provides the current status of the WM8904 driver module.

**Preconditions**

Function DRV\_WM8904\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_WM8904\_Initialize routine|

**Returns**

SYS\_STATUS\_DEINITIALIZED - Indicates that the driver has been deinitialized

SYS\_STATUS\_READY - Indicates that any previous module operation for the specified module has completed

SYS\_STATUS\_BUSY - Indicates that a previous module operation for the specified module has not yet completed

SYS\_STATUS\_ERROR - Indicates that the specified module is in an error state

**Remarks**

A driver can opened only when its status is SYS\_STATUS\_READY.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_WM8904\_Initialize*

SYS\_STATUS WM8904Status;

WM8904Status = DRV\_WM8904\_Status\(object\); **if** \(SYS\_STATUS\_READY == WM8904Status\) \{

*// This means the driver can be opened using the* *// DRV\_WM8904\_Open\(\) function.*

\}

**C**

SYS\_STATUS **DRV\_WM8904\_Status**\(SYS\_MODULE\_OBJ **object**\);

##### DRV\_WM8904\_Tasks Function

void DRV\_WM8904\_Tasks\(SYS\_MODULE\_OBJ object\);

**Summary**

Maintains the driver's control and data interface state machine.

**Description**

This routine is used to maintain the driver's internal control and data interface state machine and implement its control and data interface implementations. This function should be called from the SYS\_Tasks\(\) function.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Object handle for the specified driver instance \(returned from DRV\_WM8904\_Initialize\)|

**Returns**

None.

**Remarks**

This routine is normally not called directly by an application. It is called by the system's Tasks routine \(SYS\_Tasks\).

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_WM8904\_Initialize* **while** \(**true**\) \{

DRV\_WM8904\_Tasks \(object\);

*// Do other tasks*

\}

**C**

**void** **DRV\_WM8904\_Tasks**\(SYS\_MODULE\_OBJ **object**\);

#### Client Setup Functions

##### DRV\_WM8904\_Open Function

DRV\_HANDLE DRV\_WM8904\_Open

\(

const SYS\_MODULE\_INDEX drvIndex, const DRV\_IO\_INTENT ioIntent

\)

**Summary**

Opens the specified WM8904 driver instance and returns a handle to it

**Description**

This routine opens the specified WM8904 driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver. The ioIntent parameter defines how the client interacts with this driver instance.

The DRV\_IO\_INTENT\_BLOCKING and DRV\_IO\_INTENT\_NONBLOCKING ioIntent options are not relevant to this driver. All the data transfer functions of this driver are non blocking.

The WM8904 can be opened with DRV\_IO\_INTENT\_WRITE, or DRV\_IO\_INTENT\_READ or DRV\_IO\_INTENT\_WRITEREAD

io\_intent option. This decides whether the driver is used for headphone output, or microphone input or both modes simultaneously.

Specifying a DRV\_IO\_INTENT\_EXCLUSIVE will cause the driver to provide exclusive access to this client. The driver cannot be opened by any other client.

**Preconditions**

Function DRV\_WM8904\_Initialize must have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|drvIndex|Identifier for the object instance to be opened|
|ioIntent|Zero or more of the values from the enumeration DRV\_IO\_INTENT "ORed" together to indicate the intended use of the driver. See function description for details.|

**Returns**

If successful, the routine returns a valid open-instance handle \(a number identifying both the caller and the module instance\).

If an error occurs, the return value is DRV\_HANDLE\_INVALID. Error can occur:

-   if the number of client objects allocated via DRV\_WM8904\_CLIENTS\_NUMBER is insufficient.

-   if the client is trying to open the driver but driver has been opened exclusively by another client.

-   if the driver hardware instance being opened is not initialized or is invalid.

-   if the ioIntent options passed are not relevant to this driver.


**Remarks**

The handle returned is valid until the DRV\_WM8904\_Close routine is called. This routine will NEVER block waiting for hardware.If the requested intent flags are not supported, the routine will return DRV\_HANDLE\_INVALID. This function is thread safe in a RTOS application. It should not be called in an ISR.

**Example**

DRV\_HANDLE handle; handle = DRV\_WM8904\_Open\(DRV\_WM8904\_INDEX\_0, DRV\_IO\_INTENT\_WRITEREAD \| DRV\_IO\_INTENT\_EXCLUSIVE\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver*

*// May be the driver is not initialized or the initialization* *// is not complete.*

\}

**C**

DRV\_HANDLE **DRV\_WM8904\_Open**\(**const** SYS\_MODULE\_INDEX **iDriver**, **const** DRV\_IO\_INTENT **ioIntent**\);

##### DRV\_WM8904\_Close Function

void DRV\_WM8904\_Close\( DRV\_Handle handle \)

**Summary**

Closes an opened-instance of the WM8904 driver

**Description**

This routine closes an opened-instance of the WM8904 driver, invalidating the handle. Any buffers in the driver queue that were submitted by this client will be removed. After calling this routine, the handle passed in "handle" must not be used with any of the remaining driver routines. A new handle must be obtained by calling DRV\_WM8904\_Open before the caller may use the driver again

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

• None

**Remarks**

Usually there is no need for the driver client to verify that the Close operation has completed. The driver will abort any ongoing operations when this routine is called.

**Example**

DRV\_HANDLE handle; *// Returned from DRV\_WM8904\_Open*

DRV\_WM8904\_Close\(handle\);

**C**

**void** **DRV\_WM8904\_Close**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_WM8904\_BufferEventHandlerSet Function

void DRV\_WM8904\_BufferEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_WM8904\_BUFFER\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.

**Description**

When a client calls DRV\_WM8904\_BufferAddWrite function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "buffer add" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the queued buffer transfer has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_WM8904\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

*// Client registers an event handler with driver*

DRV\_WM8904\_BufferEventHandlerSet\(myWM8904Handle,

APP\_WM8904BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_WM8904\_BufferAddWrite\(myWM8904handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_WM8904\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_WM8904BufferEventHandler\(DRV\_WM8904\_BUFFER\_EVENT event, DRV\_WM8904\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_WM8904\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_WM8904\_BufferEventHandlerSet**\(DRV\_HANDLE **handle**, **const** DRV\_WM8904\_BUFFER\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

##### DRV\_WM8904\_CommandEventHandlerSet Function

void DRV\_WM8904\_CommandEventHandlerSet

\(

DRV\_HANDLE handle,

const DRV\_WM8904\_COMMAND\_EVENT\_HANDLER eventHandler, const uintptr\_t contextHandle

\)

**Summary**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

**Description**

This function allows a client to identify a command event handling function for the driver to call back when the last submitted command have finished.

The event handler should be set before the client performs any "WM8904 Codec Specific Client Routines" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the command has completed, it does not need to register a callback.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_WM8904\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

*// Client registers an event handler with driver*

DRV\_WM8904\_CommandEventHandlerSet\(myWM8904Handle,

APP\_WM8904CommandEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_WM8904\_DeEmphasisFilterSet\(myWM8904Handle, DRV\_WM8904\_DEEMPHASIS\_FILTER\_44\_1KHZ\)

*// Event is received when // the buffer is processed.* **void** APP\_WM8904CommandEventHandler\(uintptr\_t contextHandle\) \{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ *// Last Submitted command is completed.* *// Perform further processing here*

\}

\}

**C**

**void** **DRV\_WM8904\_CommandEventHandlerSet**\(DRV\_HANDLE **handle**, **const**

DRV\_WM8904\_COMMAND\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **contextHandle**\);

#### Data Transfer Functions

##### DRV\_WM8904\_BufferAddRead Function

void DRV\_WM8904\_BufferAddRead

\(

const DRV\_HANDLE handle,

DRV\_WM8904\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver read operation.

**Description**

This function schedules a non-blocking read operation. The function returns with a valid buffer handle in the bufferHandle argument if the read request was scheduled successfully. The function adds the request to the hardware instance receive queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_WM8904\_BUFFER\_HANDLE\_INVALID

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_WM8904\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 device instance and the DRV\_WM8904\_Status must have returned SYS\_STATUS\_READY.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle. DRV\_IO\_INTENT\_READ must have been specified in the DRV\_WM8904\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the WM8904 instance as return by the DRV\_WM8904\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_WM8904\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the WM8904 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another WM8904 driver instance. It should not otherwise be called directly in an ISR.

**C**

**void** **DRV\_WM8904\_BufferAddRead**\(**const** DRV\_HANDLE **handle**, DRV\_WM8904\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_WM8904\_BufferAddWrite Function

void DRV\_WM8904\_BufferAddWrite

\(

const DRV\_HANDLE handle,

DRV\_WM8904\_BUFFER\_HANDLE \*bufferHandle,

void \*buffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write operation.

**Description**

This function schedules a non-blocking write operation. The function returns with a valid buffer handle in the bufferHandle argument if the write request was scheduled successfully. The function adds the request to the hardware instance transmit queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_WM8904\_BUFFER\_HANDLE\_INVALID:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0.

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_WM8904\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 device instance and the DRV\_WM8904\_Status must have returned SYS\_STATUS\_READY.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_WRITE must have been specified in the DRV\_WM8904\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the WM8904 instance as return by the DRV\_WM8904\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_WM8904\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the WM8904 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another WM8904 driver instance. It should not otherwise be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_WM8904\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned*

*// by the DRV\_WM8904\_Open function.*

*// Client registers an event handler with driver*

DRV\_WM8904\_BufferEventHandlerSet\(myWM8904Handle,

APP\_WM8904BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_WM8904\_BufferAddWrite\(myWM8904handle, &bufferHandle myBuffer, MY\_BUFFER\_SIZE\); **if**\(DRV\_WM8904\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**void** APP\_WM8904BufferEventHandler\(DRV\_WM8904\_BUFFER\_EVENT event, DRV\_WM8904\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_WM8904\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_WM8904\_BufferAddWrite**\(**const** DRV\_HANDLE **handle**, DRV\_WM8904\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **buffer**, size\_t **size**\);

##### DRV\_WM8904\_BufferAddWriteRead Function

void DRV\_WM8904\_BufferAddWriteRead

\(

const DRV\_HANDLE handle,

DRV\_WM8904\_BUFFER\_HANDLE \*bufferHandle,

void \*transmitBuffer, void \*receiveBuffer, size\_t size

\)

**Summary**

Schedule a non-blocking driver write-read operation.

**Description**

This function schedules a non-blocking write-read operation. The function returns with a valid buffer handle in the bufferHandle argument if the write-read request was scheduled successfully. The function adds the request to the hardware instance queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read only or write only

-   if the buffer size is 0

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_WM8904\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 device instance and the DRV\_WM8904\_Status must have returned SYS\_STATUS\_READY.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

DRV\_IO\_INTENT\_READWRITE must have been specified in the DRV\_WM8904\_Open call.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the WM8904 instance as returned by the DRV\_WM8904\_Open function|
|bufferHandle|Pointer to an argument that will contain the return buffer handle|
|transmitBuffer|The buffer where the transmit data will be stored|
|receiveBuffer|The buffer where the received data will be stored|
|size|Buffer size in bytes|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_WM8904\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the WM8904 Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another WM8904 driver instance. It should not otherwise be called directly in an ISR.

This function is useful when there is valid read expected for every WM8904 write. The transmit and receive size must be same.

**Example**

MY\_APP\_OBJ myAppObj; uint8\_t mybufferTx; uint8\_t mybufferRx; DRV\_WM8904\_BUFFER\_HANDLE bufferHandle;

*// mywm8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

*// Client registers an event handler with driver*

DRV\_WM8904\_BufferEventHandlerSet\(mywm8904Handle, APP\_WM8904BufferEventHandler, \(uintptr\_t\)&myAppObj\);

DRV\_WM8904\_BufferAddWriteRead\(mywm8904handle, &bufferHandle,

mybufferTx,mybufferRx,MY\_BUFFER\_SIZE\);

**if**\(DRV\_WM8904\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{ *// Error handling here*

\}

*// Event is received when // the buffer is processed.* **void** APP\_WM8904BufferEventHandler\(DRV\_WM8904\_BUFFER\_EVENT event, DRV\_WM8904\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\)

\{ *// contextHandle points to myAppObj.* **switch**\(event\) \{ **case** DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_WM8904\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_WM8904\_BufferAddWriteRead**\(**const** DRV\_HANDLE **handle**, DRV\_WM8904\_BUFFER\_HANDLE \* **bufferHandle**, **void** \* **transmitBuffer**, **void** \* **receiveBuffer**, size\_t **size**\);

##### DRV\_WM8904\_ReadQueuePurge Function

bool DRV\_WM8904\_ReadQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the read queue.

**Description**

This function removes all the buffer requests from the read queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_WM8904\_Open function.|

**Returns**

True - Read queue purge is successful. False - Read queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_WM8904\_Open function.*

*// Use DRV\_WM8904\_BufferAddRead to queue read requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{

**if**\(**false** == DRV\_WM8904\_ReadQueuePurge\(myCodecHandle\)\)

\{

*//Couldn't purge the read queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_WM8904\_ReadQueuePurge**\(**const** DRV\_HANDLE **handle**\);

##### DRV\_WM8904\_WriteQueuePurge Function

bool DRV\_WM8904\_WriteQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the write queue.

**Description**

This function removes all the buffer requests from the write queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_WM8904\_Open function.|

**Returns**

True - Write queue purge is successful. False - Write queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myCodecHandle is the handle returned by the DRV\_WM8904\_Open function.*

*// Use DRV\_WM8904\_BufferAddWrite to queue write requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_WM8904\_WriteQueuePurge\(myCodecHandle\)\) \{ *//Couldn't purge the write queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_WM8904\_WriteQueuePurge**\(**const** DRV\_HANDLE **handle**\);

#### Settings Functions

##### DRV\_WM8904\_MuteOff Function

void DRV\_WM8904\_MuteOff\(DRV\_HANDLE handle\)

**Summary**

This function disables WM8904 output for soft mute.

**Description**

This function disables WM8904 output for soft mute.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

DRV\_WM8904\_MuteOff\(myWM8904Handle\); *//WM8904 output soft mute disabled*

**C**

**void** **DRV\_WM8904\_MuteOff**\(DRV\_HANDLE **handle**\);

##### DRV\_WM8904\_MuteOn Function

void DRV\_WM8904\_MuteOn\(DRV\_HANDLE handle\);

**Summary**

This function allows WM8904 output for soft mute on.

**Description**

This function Enables WM8904 output for soft mute.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

DRV\_WM8904\_MuteOn\(myWM8904Handle\); *//WM8904 output soft muted*

**C**

**void** **DRV\_WM8904\_MuteOn**\(DRV\_HANDLE **handle**\);

##### DRV\_WM8904\_SamplingRateGet Function

uint32\_t DRV\_WM8904\_SamplingRateGet\(DRV\_HANDLE handle\)

**Summary**

This function gets the sampling rate set on the WM8904.

**Description**

This function gets the sampling rate set on the DAC WM8904.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Remarks**

None.

**Example**

uint32\_t baudRate;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

baudRate = DRV\_WM8904\_SamplingRateGet\(myWM8904Handle\);

**C**

uint32\_t **DRV\_WM8904\_SamplingRateGet**\(DRV\_HANDLE **handle**\);

##### DRV\_WM8904\_SamplingRateSet Function

void DRV\_WM8904\_SamplingRateSet\(DRV\_HANDLE handle, uint32\_t samplingRate\)

**Summary**

This function sets the sampling rate of the media stream.

**Description**

This function sets the media sampling rate for the client handle.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|samplingRate|Sampling frequency in Hz|

**Returns**

None.

**Remarks**

None.

**Example**

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

DRV\_WM8904\_SamplingRateSet\(myWM8904Handle, 48000\); *//Sets 48000 media sampling rate*

**C**

**void** **DRV\_WM8904\_SamplingRateSet**\(DRV\_HANDLE **handle**, uint32\_t **samplingRate**\);

##### DRV\_WM8904\_VolumeGet Function

uint8\_t DRV\_WM8904\_VolumeGet\(DRV\_HANDLE handle, DRV\_WM8904\_CHANNEL channel\)

**Summary**

This function gets the volume for WM8904 Codec.

**Description**

This functions gets the current volume programmed to the Codec WM8904.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|channel|argument indicating Left or Right or Both channel volume to be modified|

**Returns**

None.

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj; uint8\_t volume;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

volume = DRV\_WM8904\_VolumeGet\(myWM8904Handle, DRV\_WM8904\_CHANNEL\_LEFT\);

**C**

uint8\_t **DRV\_WM8904\_VolumeGet**\(DRV\_HANDLE **handle**, DRV\_WM8904\_CHANNEL **channel**\);

##### DRV\_WM8904\_VolumeSet Function

void DRV\_WM8904\_VolumeSet\(DRV\_HANDLE handle, DRV\_WM8904\_CHANNEL channel, uint8\_t volume\);

**Summary**

This function sets the volume for WM8904 Codec.

**Description**

This function sets the volume value from 0-255. The codec has DAC value to volume range mapping as :- 00 H : +12dB FF H : -115dB In order to make the volume value to dB mapping monotonically increasing from 00 to FF, re-mapping is introduced which reverses the volume value to dB mapping as well as normalizes the volume range to a more audible dB range. The current driver implementation assumes that all dB values under -60 dB are inaudible to the human ear. Re-Mapped values 00 H : -60 dB FF H : +12 dB

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|channel|argument indicating Left or Right or Both channel volume to be modified|
|volume|volume value specified in the range 0-255 \(0x00 to 0xFF\)|

**Returns**

None

**Remarks**

None.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_BUFFER\_HANDLE bufferHandle;

*// myWM8904Handle is the handle returned // by the DRV\_WM8904\_Open function.*

DRV\_WM8904\_VolumeSet\(myWM8904Handle,DRV\_WM8904\_CHANNEL\_LEFT, 120\);

**C**

**void** **DRV\_WM8904\_VolumeSet**\(DRV\_HANDLE **handle**, DRV\_WM8904\_CHANNEL **channel**, uint8\_t **volume**\);

#### Other Functions

##### DRV\_WM8904\_GetI2SDriver Function

DRV\_HANDLE DRV\_WM8904\_GetI2SDriver\(DRV\_HANDLE codecHandle\)

**Summary**

Get the handle to the I2S driver for this codec instance.

**Description**

Returns the appropriate handle to the I2S based on the ioIent member of the codec object.

**Preconditions**

The DRV\_WM8904\_Initialize routine must have been called for the specified WM8904 driver instance.

DRV\_WM8904\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

A handle to the I2S driver for this codec instance

**Remarks**

This allows the caller to directly access portions of the I2S driver that might not be available via the codec API.

**C**

DRV\_HANDLE **DRV\_WM8904\_GetI2SDriver**\(DRV\_HANDLE **codecHandle**\);

##### DRV\_WM8904\_VersionGet Function

uint32\_t DRV\_WM8904\_VersionGet\( void \)

**Summary**

This function returns the version of WM8904 driver

**Description**

The version number returned from the DRV\_WM8904\_VersionGet function is an unsigned integer in the following decimal format. \* 10000 + \* 100 + Where the numbers are represented in decimal and the meaning is the same as above. Note that there is no numerical representation of release type.

**Preconditions**

None.

**Returns**

returns the version of WM8904 driver.

**Remarks**

None.

**Example 1**

For version "0.03a", return: 0 \* 10000 + 3 \* 100 + 0 For version "1.00", return: 1 \* 100000 + 0 \* 100 + 0

**Example 2**

uint32\_t WM8904version; WM8904version = DRV\_WM8904\_VersionGet\(\);

**C**

uint32\_t **DRV\_WM8904\_VersionGet**\(\);

##### DRV\_WM8904\_VersionStrGet Function

int8\_t\* DRV\_WM8904\_VersionStrGet\(void\)

**Summary**

This function returns the version of WM8904 driver in string format.

**Description**

The DRV\_WM8904\_VersionStrGet function returns a string in the format: "." Where: is the WM8904 driver's version number. is the WM8904 driver's version number. is an optional "patch" or "dot" release number \(which is not included in the string if it equals "00"\). is an optional release type \("a" for alpha, "b" for beta ? not the entire word spelled out\) that is not included if the release is a production version \(I.e. Not an alpha or beta\).

The String does not contain any spaces. For example, "0.03a" "1.00"

**Preconditions**

None.

**Returns**

returns a string containing the version of WM8904 driver.

**Remarks**

None

**Example**

int8\_t \*WM8904string; WM8904string = DRV\_WM8904\_VersionStrGet\(\);

**C**

int8\_t\* **DRV\_WM8904\_VersionStrGet**\(\);

##### DRV\_WM8904\_LRCLK\_Sync Function

uint32\_t DRV\_WM8904\_LRCLK\_Sync \(const DRV\_HANDLE handle\);

**Summary**

Synchronize to the start of the I2S LRCLK \(left/right clock\) signal

**Description**

This function waits until low-to high transition of the I2S LRCLK \(left/right clock\) signal \(high-low if Left-Justified format, this is determined by the PLIB\). In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize calls to the DMA with the LRCLK signal so the left/right channel association is valid.

**Preconditions**

None.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myWM8904Handle is the handle returned* *// by the DRV\_WM8904\_Open function.*

DRV\_WM8904\_LRCLK\_Sync\(myWM8904Handle\);

**C**

**bool** **DRV\_WM8904\_LRCLK\_Sync**\(**const** DRV\_HANDLE **handle**\);

#### Data Types and Constants

##### DATA\_LENGTH Type

in bits

**C**

**typedef** **enum** DATA\_LENGTH@4 **DATA\_LENGTH**;

##### DRV\_WM8904\_AUDIO\_DATA\_FORMAT Enumeration

Identifies the Serial Audio data interface format.

**Description**

WM8904 Audio data format

This enumeration identifies Serial Audio data interface format.

**C**

**typedef** **enum** \{ **DATA\_16\_BIT\_LEFT\_JUSTIFIED**,

**DATA\_16\_BIT\_I2S**, **DATA\_32\_BIT\_LEFT\_JUSTIFIED**,

**DATA\_32\_BIT\_I2S**

\} DRV\_WM8904\_AUDIO\_DATA\_FORMAT;

##### DRV\_WM8904\_BUFFER\_EVENT Enumeration

Identifies the possible events that can result from a buffer add request.

**Description**

WM8904 Driver Events

This enumeration identifies the possible events that can result from a buffer add request caused by the client calling either the DRV\_WM8904\_BufferAddWrite\(\) or the DRV\_WM8904\_BufferAddRead\(\) function.

**Remarks**

One of these values is passed in the "event" parameter of the event handling callback function that the client registered with the driver by calling the DRV\_WM8904\_BufferEventHandlerSet function when a buffer transfer request is completed.

**C**

**typedef** **enum** \{ **DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE**,

**DRV\_WM8904\_BUFFER\_EVENT\_ERROR**,

**DRV\_WM8904\_BUFFER\_EVENT\_ABORT**

\} DRV\_WM8904\_BUFFER\_EVENT;

## *DRV\_WM8904\_BUFFER\_EVENT\_HANDLER Type*

Pointer to a WM8904 Driver Buffer Event handler function

**Description**

WM8904 Driver Buffer Event Handler Function

This data type defines the required function signature for the WM8904 driver buffer event handling callback function. A client must register a pointer to a buffer event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive buffer related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|event|Identifies the type of event|
|bufferHandle|Handle identifying the buffer to which the event relates|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

If the event is DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE, this means that the data was transferred successfully.

If the event is DRV\_WM8904\_BUFFER\_EVENT\_ERROR, this means that the data was not transferred successfully. The bufferHandle parameter contains the buffer handle of the buffer that failed. The DRV\_WM8904\_BufferProcessedSizeGet\(\) function can be called to find out how many bytes were processed.

The bufferHandle parameter contains the buffer handle of the buffer that associated with the event.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_WM8904\_BufferEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The buffer handle in bufferHandle expires after this event handler exits. In that the buffer object that was allocated is deallocated by the driver after the event handler exits.

The event handler function executes in the data driver\(i2S\) peripheral's interrupt context when the driver is configured for interrupt mode operation. It is recommended of the application to not perform process intensive or blocking operations with in this function.

DRV\_WM8904\_BufferAddWrite function can be called in the event handler to add a buffer to the driver queue.

**Example**

**void** APP\_MyBufferEventHandler\( DRV\_WM8904\_BUFFER\_EVENT event, DRV\_WM8904\_BUFFER\_HANDLE bufferHandle, uintptr\_t context \) \{ MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context; **switch**\(event\)

\{ **case** DRV\_WM8904\_BUFFER\_EVENT\_COMPLETE: *// Handle the completed buffer.* **break**; **case** DRV\_WM8904\_BUFFER\_EVENT\_ERROR: **default**: *// Handle error.* **break**; \}

\}

**C**

**typedef** **void** \(\* **DRV\_WM8904\_BUFFER\_EVENT\_HANDLER**\)\(DRV\_WM8904\_BUFFER\_EVENT event, DRV\_WM8904\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\);

## *DRV\_WM8904\_BUFFER\_HANDLE Type*

Handle identifying a write buffer passed to the driver.

**Description**

WM8904 Driver Buffer Handle

A buffer handle value is returned by a call to the DRV\_WM8904\_BufferAddWrite\(\) or DRV\_WM8904\_BufferAddRead\(\) function. This handle is associated with the buffer passed into the function and it allows the application to track the completion of the data from \(or into\) that buffer.

The buffer handle value returned from the "buffer add" function is returned back to the client by the "event handler callback" function registered with the driver.

The buffer handle assigned to a client request expires when the client has been notified of the completion of the buffer transfer \(after event handler function that notifies the client returns\) or after the buffer has been retired by the driver if no event handler callback was set.

**Remarks**

None

**C**

**typedef** uintptr\_t **DRV\_WM8904\_BUFFER\_HANDLE**;

### DRV\_WM8904\_CHANNEL Enumeration

Identifies Left/Right Audio channel

**Description**

WM8904 Audio Channel

This enumeration identifies Left/Right Audio channel

**Remarks**

None.

**C**

**typedef** **enum** \{ **DRV\_WM8904\_CHANNEL\_LEFT**,

**DRV\_WM8904\_CHANNEL\_RIGHT**,

**DRV\_WM8904\_CHANNEL\_LEFT\_RIGHT**,

**DRV\_WM8904\_NUMBER\_OF\_CHANNELS**

\} DRV\_WM8904\_CHANNEL;

## *DRV\_WM8904\_COMMAND\_EVENT\_HANDLER Type*

Pointer to a WM8904 Driver Command Event Handler Function

**Description**

WM8904 Driver Command Event Handler Function

This data type defines the required function signature for the WM8904 driver command event handling callback function.

A command is a control instruction to the WM8904 Codec. Example Mute ON/OFF, Zero Detect Enable/Disable etc.

A client must register a pointer to a command event handling function who's function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive command related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

The occurrence of this call back means that the last control command was transferred successfully.

The context parameter contains a handle to the client context, provided at the time the event handling function was registered using the DRV\_WM8904\_CommandEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The event handler function executes in the control data driver interrupt context. It is recommended of the application to not perform process intensive or blocking operations with in this function.

**Example**

**void** APP\_WM8904CommandEventHandler\( uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context;

*// Last Submitted command is completed.*

*// Perform further processing here*

\}

**C**

**typedef** **void** \(\* **DRV\_WM8904\_COMMAND\_EVENT\_HANDLER**\)\(uintptr\_t contextHandle\);

### DRV\_WM8904\_INIT Structure

Defines the data required to initialize or reinitialize the WM8904 driver

**Description**

WM8904 Driver Initialization Data

This data type defines the data required to initialize or reinitialize the WM8904 Codec driver.

**Remarks**

None.

**C**

**typedef** **struct** \{

SYS\_MODULE\_INIT **moduleInit**;

SYS\_MODULE\_INDEX **i2sDriverModuleIndex**; SYS\_MODULE\_INDEX **i2cDriverModuleIndex**; **bool** **masterMode**; uint32\_t **samplingRate**; uint8\_t **volume**; DRV\_WM8904\_AUDIO\_DATA\_FORMAT **audioDataFormat**; **bool** **enableMicInput**; **bool** **enableMicBias**; \} DRV\_WM8904\_INIT;

## *DRV\_WM8904\_BUFFER\_HANDLE\_INVALID Macro*

Definition of an invalid buffer handle.

**Description**

WM8904 Driver Invalid Buffer Handle

This is the definition of an invalid buffer handle. An invalid buffer handle is returned by DRV\_WM8904\_BufferAddWrite\(\) and the DRV\_WM8904\_BufferAddRead\(\) function if the buffer add request was not successful.

**Remarks**

None.

**C**

**\#define** **DRV\_WM8904\_BUFFER\_HANDLE\_INVALID** \(\(DRV\_WM8904\_BUFFER\_HANDLE\)\(-1\)\)

### DRV\_WM8904\_COUNT Macro

Number of valid WM8904 driver indices

**Description**

WM8904 Driver Module Count

This constant identifies the maximum number of WM8904 Driver instances that should be defined by the application. Defining more instances than this constant will waste RAM memory space.

This constant can also be used by the application to identify the number of WM8904 instances on this microcontroller.

**Remarks**

This value is part-specific.

**C**

**\#define** **DRV\_WM8904\_COUNT**

### DRV\_WM8904\_INDEX\_0 Macro

WM8904 driver index definitions

**Description**

Driver WM8904 Module Index

These constants provide WM8904 driver index definition.

**Remarks**

These constants should be used in place of hard-coded numeric literals. These values should be passed into the DRV\_WM8904\_Initialize and DRV\_WM8904\_Open routines to identify the driver instance in use.

Introduction

**C**

**\#define** **DRV\_WM8904\_INDEX\_0** 0

### DRV\_I2C\_INDEX Macro

This is macro DRV\_I2C\_INDEX.

**C**

**\#define** **DRV\_I2C\_INDEX** DRV\_WM8904\_I2C\_INSTANCES\_NUMBER

### Files

**Files**

|**Name**|**Description**|
|--------|---------------|
|drv\_wm8904.h|WM8904 Codec Driver Interface header file|
|drv\_wm8904\_config\_template.h|WM8904 Codec Driver Configuration Template.|

**Description**

This section will list only the library's interface header file\(s\).

### drv\_wm8904.h

drv\_wm8904.h

**Summary**

WM8904 Codec Driver Interface header file

**Description**

WM8904 Codec Driver Interface

The WM8904 Codec device driver interface provides a simple interface to manage the WM8904 16/24/32-Bit Codec that can be interfaced to a Microchip microcontroller. This file provides the public interface definitions for the WM8904 Codec device driver.

### drv\_wm8904\_config\_template.h

drv\_wm8904\_config\_template.h

**Summary**

WM8904 Codec Driver Configuration Template.

**Description**

WM8904 Codec Driver Configuration Template

These file provides the list of all the configurations that can be used with the driver. This file should not be included in the driver.

**I2S Driver Library Help**

This section describes the I2S Driver Library.

**Introduction**

This library provides an interface to manage the I2S Audio Protocol Interface Modes.

**Description**

The I2S Driver is connected to a hardware module that provides the actual I2S stream, on some MCUs this is a Serial Peripheral Interface \(SPI\), on others it may be an I2S Controller \(I2SC\), or Serial Synchronous Controller \(SSC\).

The I2S hardware peripheral is then interfaced to various devices such as codecs and Bluetooth modules to provide microcontroller-based audio solutions.

**Using the Library**

This topic describes the basic architecture of the I2S Driver Library and provides information and examples on its use.

**Description**

## Interface Header File: drv\_i2s.h

The interface to the I2S Driver Library is defined in the drv\_i2s.h header file. Any C language source \(.c\) file that uses the I2S Driver Library should include drv\_i2s.h.

Please refer to the What is MPLAB Harmony? section for how the driver interacts with the framework.

**Example Applications:**

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/audio\_tone\_linkeddma

-   audio/apps/microphone\_loopback


## *Abstraction Model*

The I2S Driver provides a high level abstraction of the lower level \(SPI/I2SC/SSC\) I2S modules with a convenient C language interface. This topic describes how that abstraction is modeled in the software and introduces the I2S Driver Library interface.

**Description**

Different types of I2S capable PLIBs are available on various Microchip microcontrollers. Some have an internal buffer mechanism and some do not. The buffer depth varies across part families. The I2S Driver Library abstracts out these differences and provides a unified model for audio data transfer across different types of I2S modules.

Both the transmitter and receiver provide a buffer in the driver, which transmits and receives data to/from the hardware. The I2S Driver Library provides a set of interfaces to perform the read and the write. The following diagrams illustrate the abstraction model used by the I2S Driver Library. The I2SC Peripheral is used as an example of an I2S-capable PLIB. **I2S Driver Abstraction Model** ![](GUID-116B9958-209C-4BB0-A9F7-7685360BDD59-low.png)

The PLIBs currently provided, such as SSC and I2SC, only support an interrupt/DMA mode of operation. Polled mode of operation is not supported.

## *Library Overview*

Refer to the Driver Library Overwiew section for information on how the driver operates in a system.

The I2S driver library provides an API interface to transfer/receive digital audio data using supported Audio protocols. The library interface routines are divided into various sub-sections, which address one of the blocks or the overall operation of the I2S Driver Library.

|**Library Interface Section**|**Description**|
|-----------------------------|---------------|
|System Interaction Functions|Provides device initialization and status functions.|
|Client Setup Functions|Provides open and close functions.|
|Data Transfer Functions|Provides data transfer functions.|
|Miscellaneous Functions|Provides driver miscellaneous functions such as get error functions, L/R clock sync, etc.|
|Data Types and Constants|These data types and constants are required while interacting and setting up the I2S Driver Library.|

## *How the Library Works*

The library provides interfaces to support:

-   System Functionality

-   Client Functionality


![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** Not all modes are available on all devices. Please refer to the specific device data sheet to determine the supported modes.

## System Access

This section provides information on system access.

**Description**

## System Initialization

The system performs the initialization of the device driver with settings that affect only the instance of the device that is being initialized. During system initialization, each instance of the I2S module would be initialized with the following configuration settings \(either passed dynamically at run time using DRV\_I2S\_INIT or by using Initialization Overrides\) that are supported by the specific I2S device hardware:

-   Device requested power state: one of the System Module Power States. For specific details please refer to **Data Types and Constants** in the Library Interface section.

-   The actual peripheral ID enumerated as the PLIB level module ID \(e.g., SPI\_ID\_2\)

-   Defining the respective interrupt sources for TX, RX, DMA TX Channel, DMA RX Channel and Error Interrupt


The DRV\_I2S\_Initialize API returns an object handle of the type SYS\_MODULE\_OBJ. The object handle returned by the Initialize interface would be used by the other system interfaces such as DRV\_I2S\_Deinitialize, DRV\_I2S\_Status, DRV\_I2S\_Tasks, and DRV\_I2S\_TasksError.

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Notes:** 1. The system initialization setting only effect the instance of the peripheral that is being initialized.

2. Configuration of the dynamic driver for DMA mode\(uses DMA channel for data transfer\) or Non DMA mode can be performed by appropriately setting the 'dmaChannelTransmit' and 'dmaChannelReceive' variables of the DRV\_I2S\_INIT structure. For example the TX will be in DMA mode when 'dmaChannelTransmit' is initialized to a valid supported channel number from the enum DMA\_CHANNEL. TX will be in Non DMA mode when 'dmaChannelTransmit' is initialized to 'DMA\_CHANNEL\_NONE'.

**Example:**

DRV\_I2S\_INIT init;

SYS\_MODULE\_OBJ objectHandle;

*/\* I2S Driver Initialization Data \*/*

DRV\_I2S\_INIT drvI2S0InitData =

\{

.i2sPlib = &drvI2S0PlibAPI,

.interruptI2S = DRV\_I2S\_INT\_SRC\_IDX0,

.numClients = DRV\_I2S\_CLIENTS\_NUMBER\_IDX0,

.queueSize = DRV\_I2S\_QUEUE\_SIZE\_IDX0,

.dmaChannelTransmit = DRV\_I2S\_XMIT\_DMA\_CH\_IDX0,

.dmaChannelReceive = DRV\_I2S\_RCV\_DMA\_CH\_IDX0,

.i2sTransmitAddress = \(**void** \*\)&\(SSC\_REGS-\>SSC\_THR\),

.i2sReceiveAddress = \(**void** \*\)&\(SSC\_REGS-\>SSC\_RHR\),

.interruptDMA = XDMAC\_IRQn,

.dmaDataLength = DRV\_I2S\_DATA\_LENGTH\_IDX0,

\};

sysObj.drvI2S0 = DRV\_I2S\_Initialize\(DRV\_I2S\_INDEX\_0, \(SYS\_MODULE\_INIT \*\)&drvI2S0InitData\); **Task Routine**

There is no task routine, since polled mode is not currently supported.

### Client Access

This section provides information on general client operation.

**Description**

## General Client Operation

For the application to start using an instance of the module, it must call the DRV\_I2S\_Open function. This provides the settings required to open the I2S instance for operation.

For the various options available for IO\_INTENT, please refer to **Data Types and Constants** in the Library Interface section.

**Example:**

DRV\_HANDLE handle; handle = DRV\_I2S\_Open\(drvObj-\>i2sDriverModuleIndex, \(DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_NONBLOCKING\)\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver* *// May be the driver is not initialized or the initialization* *// is not complete.*

\}

### Client Operations - Buffered

This section provides information on buffered client operations.

**Description**

#### Client Operations - Buffered

Client buffered operations provide a the typical audio interface. The functions DRV\_I2S\_BufferAddRead,

DRV\_I2S\_BufferAddWrite, and DRV\_I2S\_BufferAddWriteRead are the buffered data operation functions. The buffered functions schedules non-blocking operations. The function adds the request to the hardware instance queues and returns a buffer handle.

The requesting client also registers a callback event with the driver. The driver notifies the client with

DRV\_I2S\_BUFFER\_EVENT\_COMPLETE, DRV\_I2S\_BUFFER\_EVENT\_ERROR or DRV\_I2S\_BUFFER\_EVENT\_ABORT events.

The buffer add requests are processed from the I2S channel ISR in interrupt mode.

The following diagram illustrates the buffered data operations ![](GUID-2033FC7F-0724-4949-94D9-990AF9CEAB56-low.png)

![](GUID-9DDF134C-2114-431B-9555-E4D0BCA3939D-low.png) **Note:** It is not necessary to close and reopen the client between multiple transfers.

An application using the buffered functionality needs to perform the following steps:

1.  The system should have completed necessary setup and initializations.

2.  If DMA mode is desired, the DMA should be initialized by calling SYS\_DMA\_Initialize.

3.  The necessary ports setup and remapping must be done for I2S lines: ADCDAT, DACDAT, BCLK, LRCK and MCLK \(if required\).

4.  The driver object should have been initialized by calling DRV\_I2S\_Initialize. If DMA mode is desired, related attributes in the init structure must be set.

5.  Open the driver using DRV\_I2S\_Open with the necessary ioIntent to get a client handle.

6.  The necessary BCLK, LRCK, and MCLK should be set up so as to generate the required media bit rate.

7.  The necessary Baud rate value should be set up by calling DRV\_I2S\_BaudrateSet.

8.  The Register and event handler for the client handle should be set up by calling DRV\_I2S\_BufferEventHandlerSet.

9.  Add a buffer to initiate the data transfer by calling


DRV\_I2S\_BufferAddWrite/DRV\_I2S\_BufferAddRead/DRV\_I2S\_BufferAddWriteRead.

1.  When the DMA Channel has finished, the callback function registered in step 8 will be called.

2.  Repeat step 9 through step 10 to handle multiple buffer transmission and reception.

3.  When the client is done it can use DRV\_I2S\_Close to close the client handle.


**Example:**

*// The following is an example for interrupt mode buffered transmit*

**\#define** SYS\_I2S\_DRIVER\_INDEX DRV\_I2S\_1 *// I2S Uses SPI Hardware*

**\#define** BUFFER\_SIZE 1000 *// I2S initialization structure.*

*// This should be populated with necessary settings.*

*// attributes dmaChannelTransmit/dmaChannelReceive*

*// and dmaInterruptTransmitSource/dmaInterruptReceiveSource // must be set if DMA mode of operation is desired.*

DRV\_I2S\_INIT i2sInit;

SYS\_MODULE\_OBJ sysObj; *//I2S module object* DRV\_HANDLE handle; *//Client handle* uint32\_t i2sClock; *//BCLK frequency* uint32\_t baudrate; *//baudrate*

uint16\_t myAudioBuffer; *//Audio buffer to be transmitted* DRV\_I2S\_BUFFER\_HANDLE bufferHandle; APP\_DATA\_S state; *//Application specific state* uintptr\_t contextHandle; **void** SYS\_Initialize \( **void**\* data \) \{ *// The system should have completed necessary setup and initializations.*

*// Necessary ports setup and remapping must be done for I2S lines ADCDAT,*

*// DACDAT, BCLK, LRCK and MCLK* sysObj = DRV\_I2S\_Initialize\(SYS\_I2S\_DRIVER\_INDEX, \(SYS\_MODULE\_INIT\*\)&i2sInit\); **if** \(SYS\_MODULE\_OBJ\_INVALID == sysObj\) \{

*// Handle error*

\}

\}

**void** App\_Task\(**void**\) \{ **switch**\(state\) \{ **case** APP\_STATE\_INIT: \{

handle = DRV\_I2S\_Open\(SYS\_I2S\_DRIVER\_INDEX, \(DRV\_IO\_INTENT\_WRITE \| DRV\_IO\_INTENT\_NONBLOCKING\)\); **if**\(handle != DRV\_HANDLE\_INVALID \) \{ */\* Update the state \*/* state = APP\_STATE\_WAIT\_FOR\_READY; \} \} **break**; **case** APP\_STATE\_WAIT\_FOR\_READY: \{

*// Necessary clock settings must be done to generate*

*// required MCLK, BCLK and LRCK*

DRV\_I2S\_BaudrateSet\(handle, i2sClock, baudrate\);

*/\* Set the Event handler \*/* DRV\_I2S\_BufferEventHandlerSet\(handle,App\_BufferEventHandler, contextHandle\);

*/\* Add a buffer to write\*/* DRV\_I2S\_WriteBufferAdd\(handle, myAudioBuffer, BUFFER\_SIZE, &bufferHandle\);

**if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\)

\{

*// Error handling here* \} state = APP\_STATE\_IDLE; \} **break**; **case** APP\_STATE\_WAIT\_FOR\_DONE: state = APP\_STATE\_DONE; **break**; **case** APP\_STATE\_DONE: \{

*// Close done*

DRV\_I2S\_Close\(handle\); \} **break**; **case** APP\_STATE\_IDLE: *// Do nothing* **break**; **default**: **break**; \}

\}

**void** App\_BufferEventHandler\(DRV\_I2S\_BUFFER\_EVENT event, DRV\_I2S\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle\) \{ uint8\_t temp; **if**\(DRV\_I2S\_BUFFER\_EVENT\_COMPLETE == event\) \{

*// Can set state = APP\_STATE\_WAIT\_FOR\_DONE;*

*// Take Action as needed* \} **else** **if**\(DRV\_I2S\_BUFFER\_EVENT\_ERROR == event\) \{

*// Take Action as needed* \} **else** **if**\(DRV\_I2S\_BUFFER\_EVENT\_ABORT == event\) \{ *// Take Action as needed* \} **else** \{ *// Do nothing*

\}

\}

**void** SYS\_Tasks \( **void** \) \{

*/\* Call the application's tasks routine \*/*

APP\_Tasks \( \);

\}

#### *Configuring the Library*

The configuration of the I2S Driver Library is based on the file configurations.h.

This header file contains the configuration selection for the I2S Driver Library. Based on the selections made, the I2S Driver Library may support the selected features. These configuration settings will apply to all instances of the I2S Driver Library.

This header can be placed anywhere; however, the path of this header needs to be present in the include search path for a successful build. Refer to the Applications Help section for more details.

### System Configuration

Configurations for driver instances, polled/interrupt mode, etc.

#### *Configuring MHC*

Provides examples on how to configure the MPLAB Harmony Configurator \(MHC\) for a specific driver.

**Description**

When building a new application, start by creating a 32-bit MPLAB Harmony 3 project in MPLAB X IDE by selecting *File \> New Project*. Chose the Configuration name the based on the BSP, and select the appropriate processor \(such as ATSAME70Q21B\).

In MHC, under Available Components select the appropriate BSP, such as SAM E70 Xplained Ultra. Under Audio\_-\>Templates\_, double-click on a codec template such as WM8904. Answer Yes to all questions.

You should end up with a project graph that looks like this, after rearranging the boxes: ![](GUID-773CF642-1091-455F-9FF2-FB072398BC6F-low.png)

Click on the I2S Driver component, Instance 0, and the following menu will be displayed in the Configurations Options: ![](GUID-404447BF-96F0-4991-9CA0-F9CBE04E4890-low.png)

**PLIB Used** will display the hardware peripheral instance connected to the I2S Driver, such as SPI0, SSC, or I2SC1.

**Number of Clients** indicates the maximum number of clients that can be connected to the I2S Driver.

**Transfer Queue Size** indicates number of buffers, of each transfer queue \(transmit/receive\).

**I2S Data Length** is the number of bits for one channel of audio \(left or right\). It must match the size of the PLIB.

**Use DMA for Transmit and Receive** should always be checked if using DMA, which is currently the only supported mode.

**Use DMA for Transmit** should be checked if sending data to a codec or Bluetooth module.

**DMA Channel for Transmit** indicates the DMA channel \# assigned \(done automatically when you connect the PLIB\).

**Use DMA for Receive** should be checked if receiving data from a codec or Bluetooth module.However if you are only writing to the I2S stream, leaving this checked won't harm anything.

**DMA Channel for Receive** indicates the DMA channel \# assigned \(done automatically when you connect the PLIB\).

**Included Linked List DMA Functions** should be checked if using the Linked DMA feature of some MCUs.

You can also bring in the I2S Driver by itself, by double clicking I2S under Harmony\_-\>Drivers\_ in the Available Components list. You will then need to add any additional needed components manually and connect them together.

**Building the Library**

This section lists the files that are available in the I2S Driver Library.

**Description**

The following three tables list and describe the header \(.h\) and source \(.c\) files that implement this library. The parent folder for these files is core/driver/i2s.

## Interface File\(s\)

This table lists and describes the header files that must be included \(i.e., using \#include\) by any code that uses this library.

|**Source File Name**|**Description**|-| |-| |-| |
|--------------------|---------------|--|--|--|--|--|--|
|drv\_i2s.h|This file provides the interface definitions of audio/driver/i2s/templates/drv\_i2s.h.ftl\)|the|I2S|driver|\(generated|via|template|

## Required File\(s\)

![](GUID-B924D07A-0D74-453B-BCDE-8190A195C55D-low.png) ***All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use.***

This table lists and describes the source and header files that must *always* be included in the MPLAB X IDE project to build this library.

|**Source File Name**|**Description**|
|--------------------|---------------|
|/src/drv\_i2s.c|This file contains the core implementation of the I2S driver with DMA support \(generated via template audio/driver/i2s/templates/drv\_i2s.c.ftl\)|

## Optional File\(s\)

This table lists and describes the source and header files that may optionally be included if required for the desired implementation.

|**Source File Name**|**Description**|
|--------------------|---------------|
|N/A|-|

## Module Dependencies

The I2S Driver Library depends on the following modules:

-   I2S Peripheral Library, *or*

-   SSC Peripheral Library

-   I2SC Peripheral Library


**Library Interface**

1.  **System Interaction Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_I2S\_Initialize|Initializes the I2S instance for the specified driver index.|
||DRV\_I2S\_Status|Gets the current status of the I2S driver module.|

1.  **Client Setup Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_I2S\_Open|Opens the specified I2S driver instance and returns a handle to it.|
||DRV\_I2S\_Close|Closes an opened-instance of the I2S driver.|

1.  **Data Transfer Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_I2S\_ReadBufferAdd|Queues a read operation.|
||DRV\_I2S\_WriteBufferAdd|Queues a write operation.|
||DRV\_I2S\_WriteReadBufferAdd|Queues a write/read operation.|
||DRV\_I2S\_BufferEventHandlerSet|Allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.|
||DRV\_I2S\_ReadQueuePurge|Removes all buffer requests from the read queue.|
||DRV\_I2S\_WriteQueuePurge|Removes all buffer requests from the write queue.|
||DRV\_I2S\_BufferStatusGet|Returns the transmit/receive request status.|
||DRV\_I2S\_BufferCompletedBytesGet|Returns the number of bytes that have been processed for the specified buffer request.|

1.  **Miscellaneous Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||DRV\_I2S\_ErrorGet|Gets the I2S hardware errors associated with the client.|
||DRV\_I2S\_LRCLK\_Sync|Synchronize to the start of the I2S LRCLK \(left/right clock\) signal|
||DRV\_I2S\_SerialSetup|Sets the I2S serial communication settings dynamically.|
||DRV\_I2S\_ClockGenerationSet|Set the clock\(PLLA and I2SC GCLK clock\) generation values|
||DRV\_I2S\_ProgrammableClockSet|Set the Programmable Clock|

1.  **Data Types and Constants**


|-|**Name**|**Description**|
|--|--------|---------------|
|-|DRV\_I2S\_BUFFER\_EVENT|Identifies the possible events that can result from a buffer add request.|
|-|DRV\_I2S\_BUFFER\_EVENT\_HANDLER|Pointer to a I2S Driver Buffer Event handler function|
|-|DRV\_I2S\_BUFFER\_HANDLE|Handle identifying a read or write buffer passed to the driver.|
|-|DRV\_I2S\_SERIAL\_SETUP|Defines the data required to dynamically set the serial settings.|
|-|DRV\_I2S\_BUFFER\_HANDLE\_INVALID|Definition of an invalid buffer handle.|

**Description**

This section describes the Application Programming Interface \(API\) functions of the I2S Driver Library.

Refer to each section for a detailed description.

***a\) System Interaction Functions***

## DRV\_I2S\_Initialize Function

SYS\_MODULE\_OBJ DRV\_I2S\_Initialize

\(

const SYS\_MODULE\_INDEX index, const SYS\_MODULE\_INIT \* const init

\)

**Summary**

Initializes the I2S instance for the specified driver index.

**Description**

This routine initializes the I2S driver instance for the specified driver index, making it ready for clients to open and use it. The initialization data is specified by the init parameter. The initialization may fail if the number of driver objects allocated are insufficient or if the specified driver instance is already initialized. The driver instance index is independent of the I2S module ID. For example, driver instance 0 can be assigned to I2S2.

**Preconditions**

None.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|index|Identifier for the instance to be initialized|
|init|Pointer to the init data structure containing any data necessary to initialize the driver.|

**Returns**

If successful, returns a valid handle to a driver instance object. Otherwise, returns SYS\_MODULE\_OBJ\_INVALID.

**Remarks**

This routine must be called before any other I2S routine is called.

This routine should only be called once during system initialization. This routine will NEVER block for hardware access.

**Example**

*// The following code snippet shows an example I2S driver initialization.*

SYS\_MODULE\_OBJ objectHandle;

I2S\_PLIB\_API drvUsart0PlibAPI = \{

\{ .readCallbackRegister = I2S1\_ReadCallbackRegister,

.read = I2S1\_Read,

.readIsBusy = I2S1\_ReadIsBusy,

.readCountGet = I2S1\_ReadCountGet, .writeCallbackRegister = I2S1\_WriteCallbackRegister,

.write = I2S1\_Write,

.writeIsBusy = I2S1\_WriteIsBusy,

.writeCountGet = I2S1\_WriteCountGet,

.errorGet = I2S1\_ErrorGet

\}

\};

DRV\_I2S\_INIT drvUsart0InitData =

\{

.i2sPlib = &drvUsart0PlibAPI,

.interruptI2S = I2S1\_IRQn,

.queueSizeTransmit = DRV\_I2S\_XMIT\_QUEUE\_SIZE\_IDX0,

.queueSizeReceive = DRV\_I2S\_RCV\_QUEUE\_SIZE\_IDX0,

.dmaChannelTransmit = SYS\_DMA\_CHANNEL\_NONE,

.dmaChannelReceive = SYS\_DMA\_CHANNEL\_NONE,

.i2sTransmitAddress = I2S1\_TRANSMIT\_ADDRESS,

.i2sReceiveAddress = I2S1\_RECEIVE\_ADDRESS,

.interruptDMA = XDMAC\_IRQn

\}; objectHandle = DRV\_I2S\_Initialize\(DRV\_I2S\_INDEX\_1, \(SYS\_MODULE\_INIT\*\)&drvUsart0InitData\); **if** \(SYS\_MODULE\_OBJ\_INVALID == objectHandle\) \{

*// Handle error*

\}

**C**

SYS\_MODULE\_OBJ **DRV\_I2S\_Initialize**\(**const** SYS\_MODULE\_INDEX **index**, **const** SYS\_MODULE\_INIT \* **const** **init**\);

## DRV\_I2S\_Status Function

SYS\_STATUS DRV\_I2S\_Status\( SYS\_MODULE\_OBJ object \)

**Summary**

Gets the current status of the I2S driver module.

**Description**

This routine provides the current status of the I2S driver module.

**Preconditions**

Function DRV\_I2S\_Initialize should have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|object|Driver object handle, returned from the DRV\_I2S\_Initialize routine|

**Returns**

SYS\_STATUS\_READY - Initialization have succeeded and the I2S is ready for additional operations

SYS\_STATUS\_DEINITIALIZED - Indicates that the driver has been deinitialized

**Remarks**

A driver can opened only when its status is SYS\_STATUS\_READY.

**Example**

SYS\_MODULE\_OBJ object; *// Returned from DRV\_I2S\_Initialize*

SYS\_STATUS i2sStatus; i2sStatus = DRV\_I2S \_Status\(object\); **if** \(SYS\_STATUS\_READY == i2sStatus\) \{

*// This means the driver can be opened using the* *// DRV\_I2S\_Open\(\) function.*

\}

**C**

SYS\_STATUS **DRV\_I2S\_Status**\(SYS\_MODULE\_OBJ **object**\);

***b\) Client Setup Functions***

## DRV\_I2S\_Open Function

DRV\_HANDLE DRV\_I2S\_Open

\(

const SYS\_MODULE\_INDEX index, const DRV\_IO\_INTENT ioIntent

\)

**Summary**

Opens the specified I2S driver instance and returns a handle to it.

**Description**

This routine opens the specified I2S driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver. The ioIntent parameter defines how the client interacts with this driver instance.

Specifying a DRV\_IO\_INTENT\_EXCLUSIVE will cause the driver to provide exclusive access to this client. The driver cannot be opened by any other client.

**Preconditions**

Function DRV\_I2S\_Initialize must have been called before calling this function.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|index|Identifier for the object instance to be opened|
|intent|Zero or more of the values from the enumeration DRV\_IO\_INTENT "ORed" together to indicate the intended useof the driver. See function description for details.|

**Returns**

If successful, the routine returns a valid open-instance handle \(a number identifying both the caller and the module instance\).

If an error occurs, the return value is DRV\_HANDLE\_INVALID. Error can occur

-   if the number of client objects allocated via DRV\_I2S\_CLIENTS\_NUMBER is insufficient.

-   if the client is trying to open the driver but driver has been opened exclusively by another client.

-   if the driver peripheral instance being opened is not initialized or is invalid.

-   if the client is trying to open the driver exclusively, but has already been opened in a non exclusive mode by another client.

-   if the driver is not ready to be opened, typically when the initialize routine has not completed execution.


**Remarks**

The handle returned is valid until the DRV\_I2S\_Close routine is called. This routine will NEVER block waiting for hardware. If the requested intent flags are not supported, the routine will return DRV\_HANDLE\_INVALID. This function is thread safe in a RTOS application.

**Example**

DRV\_HANDLE handle; handle = DRV\_I2S\_Open\(DRV\_I2S\_INDEX\_0, DRV\_IO\_INTENT\_EXCLUSIVE\); **if** \(DRV\_HANDLE\_INVALID == handle\) \{

*// Unable to open the driver*

*// May be the driver is not initialized or the initialization* *// is not complete.*

\}

**C**

DRV\_HANDLE **DRV\_I2S\_Open**\(**const** SYS\_MODULE\_INDEX **index**, **const** DRV\_IO\_INTENT **ioIntent**\);

## DRV\_I2S\_Close Function

void DRV\_I2S\_Close\( DRV\_Handle handle \)

**Summary**

Closes an opened-instance of the I2S driver.

**Description**

This routine closes an opened-instance of the I2S driver, invalidating the handle. Any buffers in the driver queue that were submitted by this client will be removed. A new handle must be obtained by calling DRV\_I2S\_Open before the caller may use the driver again.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

None.

**Remarks**

None.

**Example**

*// 'handle', returned from the DRV\_I2S\_Open*

DRV\_I2S\_Close\(handle\);

**C**

**void** **DRV\_I2S\_Close**\(**const** DRV\_HANDLE **handle**\);

***c\) Data Transfer Functions***

## DRV\_I2S\_ReadBufferAdd Function

void DRV\_I2S\_ReadBufferAdd

\(

const DRV\_HANDLE handle,

void \* buffer, const size\_t size,

DRV\_I2S\_BUFFER\_HANDLE \* bufferHandle

\)

**Summary**

Queues a read operation.

**Description**

This function schedules a non-blocking read operation. The function returns with a valid buffer handle in the bufferHandle argument if the read request was scheduled successfully. The function adds the request to the hardware instance receive queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_I2S\_BUFFER\_HANDLE\_INVALID in the bufferHandle argument:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the buffer size is 0

-   if the read queue size is full or queue depth is insufficient.

-   if the driver handle is invalid


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_I2S\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_I2S\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_I2S\_Open function.|
|buffer|Buffer where the received data will be stored.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The buffer handle is returned in the bufferHandle argument. This is DRV\_I2S\_BUFFER\_HANDLE\_INVALID if the request was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the I2S Driver Buffer Event Handler that is registered by the client. It should not be called in the event handler associated with another I2S driver instance. It should not be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_I2S\_BUFFER\_HANDLE bufferHandle;

*// myI2SHandle is the handle returned // by the DRV\_I2S\_Open function.*

DRV\_I2S\_ReadBufferAdd\(myI2Shandle, myBuffer, MY\_BUFFER\_SIZE,

&bufferHandle\);

**if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{ *// Error handling here*

\}

*// Event is received when the buffer is processed.*

**C**

**void** **DRV\_I2S\_ReadBufferAdd**\(**const** DRV\_HANDLE **handle**, **void** \* **buffer**, **const** size\_t **size**, DRV\_I2S\_BUFFER\_HANDLE \* **const** **bufferHandle**\);

## DRV\_I2S\_WriteBufferAdd Function

void DRV\_I2S\_WriteBufferAdd

\(

const DRV\_HANDLE handle,

void \* buffer, size\_t size,

DRV\_I2S\_BUFFER\_HANDLE \* bufferHandle

\);

**Summary**

Queues a write operation.

**Description**

This function schedules a non-blocking write operation. The function returns with a valid buffer handle in the bufferHandle argument if the write request was scheduled successfully. The function adds the request to the driver instance transmit queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. On returning, the bufferHandle parameter may be DRV\_I2S\_BUFFER\_HANDLE\_INVALID for the following reasons:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read-only

-   if the buffer size is 0

-   if the transmit queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_I2S\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully or a DRV\_I2S\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully. **Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as return by the DRV\_I2S\_Open function.|
|buffer|Data to be transmitted.|
|size|Buffer size in bytes.|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_I2S\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the I2S Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another I2S driver instance. It should not otherwise be called directly in an ISR.

**Example**

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_I2S\_BUFFER\_HANDLE bufferHandle;

*// myI2SHandle is the handle returned // by the DRV\_I2S\_Open function.*

DRV\_I2S\_WriteBufferAdd\(myI2Shandle, myBuffer, MY\_BUFFER\_SIZE,

&bufferHandle\);

**if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when // the buffer is processed.*

**C**

**void** **DRV\_I2S\_WriteBufferAdd**\(**const** DRV\_HANDLE **handle**, **void** \* **buffer**, **const** size\_t **size**, DRV\_I2S\_BUFFER\_HANDLE \* **bufferHandle**\);

## DRV\_I2S\_WriteReadBufferAdd Function

void DRV\_I2S\_BufferAddWriteRead\(const DRV\_HANDLE handle, void \*transmitBuffer, void \*receiveBuffer,

size\_t size, DRV\_I2S\_BUFFER\_HANDLE \*bufferHandle\)

**Summary**

Queues a write/read operation.

**Description**

This function schedules a non-blocking write-read operation. The function returns with a valid buffer handle in the bufferHandle argument if the write-read request was scheduled successfully. The function adds the request to the hardware instance queue and returns immediately. While the request is in the queue, the application buffer is owned by the driver and should not be modified. The function returns DRV\_I2S\_BUFFER\_HANDLE\_INVALID:

-   if a buffer could not be allocated to the request

-   if the input buffer pointer is NULL

-   if the client opened the driver for read only or write only

-   if the buffer size is 0

-   if the queue is full or the queue depth is insufficient


If the requesting client registered an event callback with the driver, the driver will issue a

DRV\_I2S\_BUFFER\_EVENT\_COMPLETE event if the buffer was processed successfully of DRV\_I2S\_BUFFER\_EVENT\_ERROR event if the buffer was not processed successfully.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as return by the DRV\_I2S\_Open function.|
|transmitBuffer|Data to be transmitted.|
|receiveBuffer|Will hold data that is received.|
|size|Buffer size in bytes \(same for both buffers\)|
|bufferHandle|Pointer to an argument that will contain the return buffer handle.|

**Returns**

The bufferHandle parameter will contain the return buffer handle. This will be DRV\_I2S\_BUFFER\_HANDLE\_INVALID if the function was not successful.

**Remarks**

This function is thread safe in a RTOS application. It can be called from within the I2S Driver Buffer Event Handler that is registered by this client. It should not be called in the event handler associated with another I2S driver instance. It should not otherwise be called directly in an ISR.

This function is useful when there is valid read expected for every I2S write. The transmit and receive size must be same.

**C**

**void** **DRV\_I2S\_WriteReadBufferAdd**\(**const** DRV\_HANDLE **handle**, **void** \* **transmitBuffer**, **void** \* **receiveBuffer**, size\_t **size**, DRV\_I2S\_BUFFER\_HANDLE \* **bufferHandle**\);

## DRV\_I2S\_BufferEventHandlerSet Function

void DRV\_I2S\_BufferEventHandlerSet

\(

const DRV\_HANDLE handle,

const DRV\_I2S\_BUFFER\_EVENT\_HANDLER eventHandler, const uintptr\_t context

\)

**Summary**

Allows a client to identify a buffer event handling function for the driver to call back when queued buffer transfers have finished.

**Description**

This function allows a client to register a buffer event handling function with the driver to call back when queued buffer transfers have finished. When a client calls either the DRV\_I2S\_ReadBufferAdd or DRV\_I2S\_WriteBufferAdd function, it is provided with a handle identifying the buffer that was added to the driver's buffer queue. The driver will pass this handle back to the client by calling "eventHandler" function when the buffer transfer has completed.

The event handler should be set before the client performs any "buffer add" operations that could generate events. The event handler once set, persists until the client closes the driver or sets another event handler \(which could be a "NULL" pointer to indicate no callback\).

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine.|
|eventHandler|Pointer to the event handler function.|
|context|The value of parameter will be passed back to the client unchanged, when the eventHandler function is called. It can be used to identify any client specific data object that identifies the instance of the client module \(for example, it may be a pointer to the client module's state structure\).|

**Returns**

None.

**Remarks**

If the client does not want to be notified when the queued buffer transfer has completed, it does not need to register a callback. This function is thread safe when called in a RTOS application.

**Example**

*// myAppObj is an application specific state data object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_I2S\_BUFFER\_HANDLE bufferHandle;

*// myI2SHandle is the handle returned // by the DRV\_I2S\_Open function.*

*// Client registers an event handler with driver. This is done once*

DRV\_I2S\_BufferEventHandlerSet\( myI2SHandle, APP\_I2SBufferEventHandler,

\(uintptr\_t\)&myAppObj \);

DRV\_I2S\_ReadBufferAdd\(myI2Shandle, myBuffer, MY\_BUFFER\_SIZE,

&bufferHandle\);

**if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*// Event is received when the buffer is processed.*

**void** APP\_I2SBufferEventHandler\(DRV\_I2S\_BUFFER\_EVENT event, DRV\_I2S\_BUFFER\_HANDLE handle, uintptr\_t context\)

\{

*// The context handle was set to an application specific* *// object. It is now retrievable easily in the event handler.*

MY\_APP\_OBJ myAppObj = \(MY\_APP\_OBJ \*\) context; **switch**\(event\) \{ **case** DRV\_I2S\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_I2S\_BUFFER\_EVENT\_ERROR: *// Error handling here.* **break**; **default**: **break**; \}

\}

**C**

**void** **DRV\_I2S\_BufferEventHandlerSet**\(**const** DRV\_HANDLE **handle**, **const** DRV\_I2S\_BUFFER\_EVENT\_HANDLER **eventHandler**, **const** uintptr\_t **context**\);

## DRV\_I2S\_ReadQueuePurge Function

bool DRV\_I2S\_ReadQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the read queue.

**Description**

This function removes all the buffer requests from the read queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_I2S\_Open function.|

**Returns**

True - Read queue purge is successful. False - Read queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myI2SHandle is the handle returned by the DRV\_I2S\_Open function.*

*// Use DRV\_I2S\_ReadBufferAdd to queue read requests*

*// Application timeout function, where remove queued buffers.* **void** APP\_TimeOut\(**void**\) \{ **if**\(**false** == DRV\_I2S\_ReadQueuePurge\(myI2Shandle\)\) \{ *//Couldn't purge the read queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_I2S\_ReadQueuePurge**\(**const** DRV\_HANDLE **handle**\);

## DRV\_I2S\_WriteQueuePurge Function

bool DRV\_I2S\_WriteQueuePurge\( const DRV\_HANDLE handle \)

**Summary**

Removes all buffer requests from the write queue.

**Description**

This function removes all the buffer requests from the write queue. The client can use this function to purge the queue on timeout or to remove unwanted stalled buffer requests or in any other use case.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|Handle of the communication channel as returned by the DRV\_I2S\_Open function.|

**Returns**

True - Write queue purge is successful. False - Write queue purge has failed.

**Remarks**

This function is thread safe when used in an RTOS environment. Avoid this function call from within the callback.

**Example**

*// myI2SHandle is the handle returned by the DRV\_I2S\_Open function.*

*// Use DRV\_I2S\_WriteBufferAdd to queue write requests*

*// Application timeout function, where remove queued buffers.*

**void** APP\_TimeOut\(**void**\) \{

**if**\(**false** == DRV\_I2S\_WriteQueuePurge\(myI2Shandle\)\) \{ *//Couldn't purge the write queue, try again.* \} **else** \{ *//Queue purge successful.*

\}

\}

**C**

**bool** **DRV\_I2S\_WriteQueuePurge**\(**const** DRV\_HANDLE **handle**\);

## DRV\_I2S\_BufferStatusGet Function

DRV\_I2S\_BUFFER\_EVENT DRV\_I2S\_BufferStatusGet

\(

const DRV\_I2S\_BUFFER\_HANDLE bufferHandle

\)

**Summary**

Returns the transmit/receive request status.

**Description**

This function can be used to poll the status of the queued buffer request if the application doesn't prefer to use the event handler \(callback\) function to get notified.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

Either the DRV\_I2S\_ReadBufferAdd or DRV\_I2S\_WriteBufferAdd function must have been called and a valid buffer handle returned.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|bufferhandle|Handle for the buffer of which the processed number of bytes to be obtained.|

**Returns**

The success or error event of the buffer.

**Remarks**

This function returns error event if the buffer handle is invalid.

**Example**

*// myAppObj is an application specific object.*

MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_I2S\_BUFFER\_HANDLE bufferHandle;

DRV\_I2S\_BUFFER\_EVENT event;

*// myI2SHandle is the handle returned* *// by the DRV\_I2S\_Open function.*

*// Client registers an event handler with driver. This is done once*

DRV\_I2S\_BufferEventHandlerSet\( myI2SHandle, APP\_I2SBufferEventHandle,

\(uintptr\_t\)&myAppObj \);

DRV\_I2S\_ReadBufferAdd\( myI2Shandle, myBuffer, MY\_BUFFER\_SIZE, bufferHandle\); **if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{

*// Error handling here*

\}

*//Check the status of the buffer* *//This call can be used to wait until the buffer is processed.*

event = DRV\_I2S\_BufferStatusGet\(bufferHandle\);

**C**

DRV\_I2S\_BUFFER\_EVENT **DRV\_I2S\_BufferStatusGet**\(**const** DRV\_I2S\_BUFFER\_HANDLE **bufferHandle**\);

## DRV\_I2S\_BufferCompletedBytesGet Function

size\_t DRV\_I2S\_BufferCompletedBytesGet

\(

DRV\_I2S\_BUFFER\_HANDLE bufferHandle

\);

**Summary**

Returns the number of bytes that have been processed for the specified buffer request.

**Description**

The client can use this function, in a case where the buffer is terminated due to an error, to obtain the number of bytes that have been processed. Or in any other use case. This function can be used for non-DMA buffer transfers only. It cannot be used when the I2S driver is configured to use DMA.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

Either the DRV\_I2S\_ReadBufferAdd or DRV\_I2S\_WriteBufferAdd function must have been called and a valid buffer handle returned.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|bufferhandle|Handle for the buffer of which the processed number of bytes to be obtained.|

**Returns**

Returns the number of bytes that have been processed for this buffer.

Returns DRV\_I2S\_BUFFER\_HANDLE\_INVALID for an invalid or an expired buffer handle.

**Remarks**

This function is expected to work in non-DMA mode only. This function is thread safe when used in a RTOS application. If called from the callback, it must not call an OSAL mutex or critical section.

**Example**

*// myAppObj is an application specific object.* MY\_APP\_OBJ myAppObj;

uint8\_t mybuffer; DRV\_I2S\_BUFFER\_HANDLE bufferHandle;

*// myI2SHandle is the handle returned // by the DRV\_I2S\_Open function.*

*// Client registers an event handler with driver. This is done once*

DRV\_I2S\_BufferEventHandlerSet\( myI2SHandle, APP\_I2SBufferEventHandle,

\(uintptr\_t\)&myAppObj \);

DRV\_I2S\_ReadBufferAdd\( myI2Shandle, myBuffer, MY\_BUFFER\_SIZE, bufferHandle\); **if**\(DRV\_I2S\_BUFFER\_HANDLE\_INVALID == bufferHandle\) \{ *// Error handling here*

\}

*// Event Processing Technique. Event is received when // the buffer is processed.*

**void** APP\_I2SBufferEventHandler\( DRV\_I2S\_BUFFER\_EVENT event, DRV\_I2S\_BUFFER\_HANDLE bufferHandle, uintptr\_t contextHandle \)

\{

*// The context handle was set to an application specific* *// object. It is now retrievable easily in the event handler.* MY\_APP\_OBJ myAppObj = \(MY\_APP\_OBJ \*\) contextHandle; size\_t processedBytes; **switch**\(event\) \{ **case** DRV\_I2S\_BUFFER\_EVENT\_COMPLETE: *// This means the data was transferred.* **break**; **case** DRV\_I2S\_BUFFER\_EVENT\_ERROR: *// Error handling here.*

*// We can find out how many bytes have been processed in this* *// buffer request prior to the error.*

processedBytes= DRV\_I2S\_BufferCompletedBytesGet\(bufferHandle\); **break**; **default**: **break**; \}

\}

**C**

size\_t **DRV\_I2S\_BufferCompletedBytesGet**\(DRV\_I2S\_BUFFER\_HANDLE **bufferHandle**\);

***d\) Miscellaneous Functions***

## DRV\_I2S\_ErrorGet Function

DRV\_I2S\_ERROR DRV\_I2S\_ErrorGet\( const DRV\_HANDLE handle \)

**Summary**

Gets the I2S hardware errors associated with the client.

**Description**

This function returns the errors associated with the given client. The call to this function also clears all the associated error flags.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|

**Returns**

Errors occurred as listed by DRV\_I2S\_ERROR. This function reports multiple I2S errors if occurred.

**Remarks**

I2S errors are normally associated with the receiver. The driver clears all the errors internally and only returns the occurred error information for the client.

**Example**

*// 'handle', returned from the DRV\_I2S\_Open* **if** \(DRV\_I2S\_ERROR\_OVERRUN & DRV\_I2S\_ErrorGet\(handle\)\) \{

*//Errors are cleared by the driver, take respective action* *//for the overrun error case.*

\}

**C**

DRV\_I2S\_ERROR **DRV\_I2S\_ErrorGet**\(**const** DRV\_HANDLE **handle**\);

## DRV\_I2S\_LRCLK\_Sync Function

uint32\_t DRV\_I2S\_LRCLK\_Sync \(const DRV\_HANDLE handle, const uint32\_t sample\_rate\);

**Summary**

Synchronize to the start of the I2S LRCLK \(left/right clock\) signal

**Description**

This function waits until low-tohigh transition of the I2S LRCLK \(left/right clock\) signal \(high-low if Left-Jusitified format, this is determined by the PLIB\). In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize calls to the DMA with the LRCLK signal so the left/right channel association is valid.

**Preconditions**

None.

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myI2SHandle is the handle returned* *// by the DRV\_I2S\_Open function.*

DRV\_I2S\_LRCLK\_Sync\(myI2SHandle, 48000\);

**C**

**bool** **DRV\_I2S\_LRCLK\_Sync**\(**const** DRV\_HANDLE **handle**, **const** uint32\_t **sample\_rate**\);

## DRV\_I2S\_SerialSetup Function

bool DRV\_I2S\_SerialSetup\(const DRV\_HANDLE handle,

DRV\_I2S\_SERIAL\_SETUP \* setup\)

**Summary**

Sets the I2S serial communication settings dynamically.

**Description**

This function sets the I2S serial communication settings dynamically.

**Preconditions**

DRV\_I2S\_Open must have been called to obtain a valid opened device handle. The I2S transmit or receive transfer status should not be busy.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|handle|A valid open-instance handle, returned from the driver's open routine|
|setup|Pointer to the structure containing the serial setup.|

**Returns**

true - Serial setup was updated successfully. false - Failure while updating serial setup.

**Remarks**

None.

**Example**

*// 'handle', returned from the DRV\_I2S\_Open*

DRV\_I2S\_SERIAL\_SETUP setup = \{

115200,

DRV\_I2S\_DATA\_8\_BIT,

DRV\_I2S\_PARITY\_ODD,

DRV\_I2S\_STOP\_1\_BIT

\};

DRV\_I2S\_SerialSetup\(handle, &setup\);

**C**

**bool** **DRV\_I2S\_SerialSetup**\(**const** DRV\_HANDLE **handle**, DRV\_I2S\_SERIAL\_SETUP \* **setup**\);

## DRV\_I2S\_ClockGenerationSet Function

bool DRV\_I2S\_ClockGenerationSet\(DRV\_HANDLE handle, uint8\_t div, uint8\_t mul, uint8\_t div2\);

**Summary**

Set the clock\(PLLA and I2SC GCLK clock\) generation values

**Description**

Set the clock\(PLLA and I2SC GCLK clock\) generation values

**Preconditions**

None.

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myI2SHandle is the handle returned* *// by the DRV\_I2S\_Open function.*

DRV\_I2S\_ClockGenerationSet\(myI2SHandle, 2, 40, 5\);

**C**

**bool** **DRV\_I2S\_ClockGenerationSet**\(DRV\_HANDLE **handle**, uint8\_t **div**, uint8\_t **mul**, uint8\_t **div2**\);

## DRV\_I2S\_ProgrammableClockSet Function

bool DRV\_I2S\_ProgrammableClockSet\(DRV\_HANDLE handle, uint8\_t pClkNum, uint8\_t div2\);

**Summary**

Set the Programmable Clock

**Description**

Set the Programmable Clock ignoring glitch control

**Preconditions**

None.

**Returns**

true if the function was successful, false if a timeout occurred \(no transitions seen\)

**Remarks**

None.

**Example**

*// myI2SHandle is the handle returned* *// by the DRV\_I2S\_Open function.*

DRV\_I2S\_ProgrammableClockSet\(myI2SHandle, 2, 7\);

**C**

**bool** **DRV\_I2S\_ProgrammableClockSet**\(DRV\_HANDLE **handle**, uint8\_t **pClkNum**, uint8\_t **div2**\);

***e\) Data Types and Constants***

## DRV\_I2S\_BUFFER\_EVENT Enumeration

Identifies the possible events that can result from a buffer add request.

**Description**

I2S Driver Buffer Events

This enumeration identifies the possible events that can result from a buffer add request caused by the client calling either the DRV\_I2S\_ReadBufferAdd or DRV\_I2S\_WriteBufferAdd functions.

**Remarks**

One of these values is passed in the "event" parameter of the event handling callback function that the client registered with the driver by calling the DRV\_I2S\_BufferEventHandlerSet function when a buffer transfer request is completed.

**C**

**typedef** **enum** \{ **DRV\_I2S\_BUFFER\_EVENT\_COMPLETE**,

**DRV\_I2S\_BUFFER\_EVENT\_ERROR**,

**DRV\_I2S\_BUFFER\_EVENT\_ABORT**

\} DRV\_I2S\_BUFFER\_EVENT;

## DRV\_I2S\_BUFFER\_EVENT\_HANDLER Type

Pointer to a I2S Driver Buffer Event handler function

**Description**

I2S Driver Buffer Event Handler Function Pointer

This data type defines the required function signature for the I2S driver buffer event handling callback function. A client must register a pointer using the buffer event handling function whose function signature \(parameter and return value types\) match the types specified by this function pointer in order to receive buffer related event calls back from the driver.

The parameters and return values are described here and a partial example implementation is provided.

**Parameters**

|**Parameters**|**Description**|
|--------------|---------------|
|event|Identifies the type of event|
|bufferHandle|Handle identifying the buffer to which the vent relates|
|context|Value identifying the context of the application that registered the event handling function.|

**Returns**

None.

**Remarks**

If the event is DRV\_I2S\_BUFFER\_EVENT\_COMPLETE, it means that the data was transferred successfully.

If the event is DRV\_I2S\_BUFFER\_EVENT\_ERROR, it means that the data was not transferred successfully. The DRV\_I2S\_BufferCompletedBytesGet function can be called to find out how many bytes were processed.

The bufferHandle parameter contains the buffer handle of the buffer that associated with the event. And bufferHandle will be valid while the buffer request is in the queue and during callback, unless an error occurred. After callback returns, the driver will retire the buffer handle.

The context parameter contains the a handle to the client context, provided at the time the event handling function was registered using the DRV\_I2S\_BufferEventHandlerSet function. This context handle value is passed back to the client as the "context" parameter. It can be any value necessary to identify the client context or instance \(such as a pointer to the client's data\) instance of the client that made the buffer add request.

The event handler function executes in the peripheral's interrupt context when the driver is configured for interrupt mode operation. It is recommended of the application to not perform process intensive or blocking operations with in this function.

The DRV\_I2S\_ReadBufferAdd and DRV\_I2S\_WriteBufferAdd functions can be called in the event handler to add a buffer to the driver queue. These functions can only be called to add buffers to the driver whose event handler is running. For example, I2S2 driver buffers cannot be added in I2S1 driver event handler.

**Example**

**void** APP\_MyBufferEventHandler\( DRV\_I2S\_BUFFER\_EVENT event, DRV\_I2S\_BUFFER\_HANDLE bufferHandle, uintptr\_t context \) \{

MY\_APP\_DATA\_STRUCT pAppData = \(MY\_APP\_DATA\_STRUCT\) context; **switch**\(event\) \{ **case** DRV\_I2S\_BUFFER\_EVENT\_COMPLETE: *// Handle the completed buffer.*

**break**; **case** DRV\_I2S\_BUFFER\_EVENT\_ERROR:

**default**: *// Handle error.*

**break**;

Files

\}

\}

**C**

**typedef** **void** \(\* **DRV\_I2S\_BUFFER\_EVENT\_HANDLER**\)\(DRV\_I2S\_BUFFER\_EVENT event, DRV\_I2S\_BUFFER\_HANDLE bufferHandle, uintptr\_t context\);

## DRV\_I2S\_BUFFER\_HANDLE Type

Handle identifying a read or write buffer passed to the driver.

**Description**

I2S Driver Buffer Handle

A buffer handle value is returned by a call to the DRV\_I2S\_ReadBufferAdd or DRV\_I2S\_WriteBufferAdd functions. This handle is associated with the buffer passed into the function and it allows the application to track the completion of the data from \(or into\) that buffer. The buffer handle value returned from the "buffer add" function is returned back to the client by the "event handler callback" function registered with the driver.

The buffer handle assigned to a client request expires when the client has been notified of the completion of the buffer transfer \(after event handler function that notifies the client returns\) or after the buffer has been retired by the driver if no event handler callback was set.

**Remarks**

None

**C**

**typedef** uintptr\_t **DRV\_I2S\_BUFFER\_HANDLE**;

## DRV\_I2S\_SERIAL\_SETUP Type

Defines the data required to dynamically set the serial settings.

**Description**

I2S Driver Serial Setup Data

This data type defines the data required to dynamically set the serial settings for the specific I2S driver instance.

**Remarks**

This structure is implementation specific. It is fully defined in drv\_i2s\_definitions.h.

**C**

**typedef** **struct** \_DRV\_I2S\_SERIAL\_SETUP **DRV\_I2S\_SERIAL\_SETUP**;

## DRV\_I2S\_BUFFER\_HANDLE\_INVALID Macro

Definition of an invalid buffer handle.

**Description**

I2S Driver Invalid Buffer Handle

This is the definition of an invalid buffer handle. An invalid buffer handle is returned by DRV\_I2S\_ReadBufferAdd and DRV\_I2S\_WriteBufferAdd functions if the buffer add request was not successful.

**Remarks**

None

**C**

**\#define** **DRV\_I2S\_BUFFER\_HANDLE\_INVALID**

Files

**Files**

**Files**

|**Name**|**Description**|
|--------|---------------|
|drv\_i2s.h|I2S Driver Interface Header File|
|drv\_i2s\_config\_template.h|I2S Driver Configuration Template.|

**Description**

## *drv\_i2s.h*

drv\_i2s.h

**Summary**

I2S Driver Interface Header File

**Description**

I2S Driver Interface Header File

The I2S device driver provides a simple interface to manage the I2S or SSC modules on Microchip PIC32 microcontrollers. This file provides the interface definition for the I2S driver.

## *drv\_i2s\_config\_template.h*

drv\_i2s\_config\_template.h

**Summary**

I2S Driver Configuration Template.

**Description**

I2S Driver Configuration Template

These file provides the list of all the configurations that can be used with the driver. This file should not be included in the driver.

Introduction

**Peripheral Libraries Help**

This topic provides help for the peripheral libraries that are available in the audio repo.

For additional information on Harmony 3 peripheral libraries \(PLIBs\), refer to the documentation in the csp repository.

**Description**

The MPLAB Harmony Configurator \(MHC\) is a Graphical User Interface \(GUI\) plug-in tool for MPLAB X IDE to configures a rich set of peripherals and functions specific to your application and generate the corresponding peripheral library code. The generated code directly accesses the Peripheral registers without any abstraction layers and it is easy to understand.

**I2S Peripheral Library Help**

This section provides an interface to use the I2S peripheral.

**Introduction**

This library provides a brief overview of the I2S peripheral.

**Description**

The I2S module implements an I2S \(Inter-IC Sound\) interface, for connection between an MCU and an audio peripheral such as a codec or Bluetooth module.

The I2S module provides support to the audio protocol functionality via four standard I/O pins. The four pins that make up the audio protocol interface modes are:

-   **SDI:** Serial Data Input for receiving sample digital audio data \(ADCDAT as output from the codec\)

-   **SDO:** Serial Data Output for transmitting digital audio data \(DACDAT as input to the codec\)

-   **SCKn:** Serial Clock, also known as bit clock \(BCLK\)

-   **FSn:** Frame Select, also known as Word Select or Left/Right Channel Clock \(LRCK\)


In addition, there is a fifth line, called **MCKn** \(Master Clock\), which can be used to drive the codec.

There are two clock units in the I2S module, so **n**=1 or 2 depending on which is used. However there are only two data lines \(SDO/SDI\), one for each serializer \(transmit/receive\).

The SCK provides the clock required to drive the data out or into the module, while FS provides the synchronization of the frame based on the protocol mode selected. In I2S mode, the leading edge of audio data is driven out one SCK period of starting the frame.

In Master mode, the module generates both the SCK and FS.

In Slave mode, the peripheral generates the BCLK and LRCLK signals, driving the SCK and FS pins of the I2S module. When in Slave mode, the I2S cannot generate a master clock \(MCK\), so a generic clock \(GCLKn\) must be used.

## Master Mode

### Master Generating its Own Clock – Output BCLK and LRCK

![](GUID-0EAF3BD6-20B2-4761-A19A-D9E8C8DD14D9-low.png)

## Slave Mode

### Codec Device as Master Derives MCLK from MCU Reference Clock Out

Configuring the Library ![](GUID-DB11C543-871D-405A-9F39-15BC9DDB4514-low.png)

**Configuring the Library**

This section describes how to configure the peripheral library using the MHC.

**Description**

The library is configured for the supported processor when the processor is chosen in MPLAB X IDE, using the Microchip Harmony Configurator \(MHC\).

Choose the I2S peripheral by clicking on the appropriate instance under *Peripherals-\>I2S* in the Available Components section of MHC. \(The I2S component may also be added automatically as a result of using a BSP Template, such as one for a codec or Bluetooth module\).

When the I2S peripheral is clicked on in the Project Graph, the following menu is displayed in the Configurations Options: ![](GUID-EB4BD32D-6DBB-4C6E-9B43-BC917603F923-low.png)

Typical values are shown for working in Slave mode, with a codec such as the WM8904 in Master mode. **DMA** and **Interrupt Mode** are always enabled.

For each of the two Clock Units \(0 and 1, only the first is shown\):

**Clock Unit n Enable --** if checked, the clock unit is enabled

**Master/Slave Mode** can be either Master -- the I2S peripheral supplies the I2S clocks, or Slave -- the peripheral such as a codec or Bluetooth module supplies the I2S clocks.

Using the Library

**Master Clock Output Divisor** -- the Generic I2S Clock \(GCLK\_I2S\_n\) is divided by this number to generate the Master Clock \(MCKn\)

**Master Clock Division Factor** -- the Master Clock \(MCKn\) is divided by this number to generate a serial clock SCKn

**Frame Sync Output Invert** -- the FSn signal is output with or without inversion

**Frame Sync Input Invert** -- the FSn signal is used with or without inversion on input

**Data Delay from Frame Sync** -- either 1 bit for I2S format, or 0 for left/right adjusted format

**Slot Size** selects the number of bits per slot: 8, 16, 24 or 32

For each of the Serializers \(TX/RX\):

**Stereo/Mono** selects stereo or mono \(left channel is duplicated to the right\)

**Data Word Formatting Adjust** selects left or right adjustment of data samples within the word

**Data Word Size** selects the number of bits per sample: 8, 16, 18, 20, 24 or 32 \(or 8-bit compact stereo or 16-bit compact stereo\)

**Data Slot Formatting Adjust** selects left or right adjustment of data samples within the slot

**Clock Unit Selection** selects which clock unit to use for this serializer, 0 or 1

**Frame Select Pin** specifies which pin is used as the FSn output as selected in the Pin Diagram

**Invert** selects whether the output of the I2S\_LRCLK\_Get function returns the true value or inverted one.

**Using the Library**

This topic describes the basic architecture of the I2S Peripheral Library and provides information and examples on how to use it.

**Description**

## Abstraction Model

The I2S module sits between the I2S Driver, and the actual hardware. ![](GUID-116B9958-209C-4BB0-A9F7-7685360BDD59-low.png)

### Interface Header File: plib\_i2sc.h

The interface to the I2S Peripheral Library is defined in the plib\_i2sc.h header file. Any C language source \(.c\) file that uses the I2S Peripheral Library should include plib\_i2sc.h.

**Library Source Files:**

The I2S Peripheral Library library source files are provided in the audio/peripheral/i2s\_xxxx/src or audio/peripheral/i2s\_xxxx/templates directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features.

Introduction

## Usage Model

The only usage model for the I2S Peripheral Library is to use a Interrupt/DMA model. Therefore the remaining functions normally associated with a PLIB like this \(handling write or read requests, or returning transfer status\), will be accomplished by the I2S driver directly communicating with the appropriate DMA functions, which is why they are not provided here.

The one function provided besides initialization is one for synchronizing with the left/right clock \(LRCLK\) for the I2S stream.

**Example Applications:**

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/audio\_tone\_linkeddma

-   audio/apps/microphone\_loopback


**Library Interface**

This section describes the Application Programming Interface \(API\) functions of the Peripheral Library.

Refer to each section for a detailed description.

1.  ***Initialization Function***

2.  ***Transaction Functions***


**Files**

**Files**

|**Name**|**Description**|
|--------|---------------|
|plib\_i2s\_u2224.h|I2S PLIB Header File for documentation|

**Description**

## *plib\_i2s\_u2224.h*

plib\_i2s\_u2224.h

**Summary**

I2S PLIB Header File for documentation

**Description**

I2SC PLIB

This library provides documentation of all the interfaces which can be used to control and interact with an instance of an Inter-IC Sound Controller \(I2S\). This file must not be included in any MPLAB Project.

**I2SC Peripheral Library Help**

This section provides an interface to use the Inter-IC Sound Controller \(I2SC\) peripheral.

Configuring the Library

**Introduction**

This library provides a brief overview of the I2SC peripheral.

**Description**

The I2SC module implements an I2S \(Inter-IC Sound\) interface, for connection between an MCU and an audio peripheral such as a codec or Bluetooth module.

The I2SC module provides support to the audio protocol functionality via four standard I/O pins. The four pins that make up the audio protocol interface modes are:

-   **DI:** Serial Data Input for receiving sample digital audio data \(ADCDAT\)

-   **DO:** Serial Data Output for transmitting digital audio data \(DACDAT\)

-   **CK:** Serial Clock, also known as bit clock \(BCLK\)

-   **WS:** Word Select, also known as Left/Right Channel Clock \(LRCK\)


The BCLK provides the clock required to drive the data out or into the module, while the LRCK provides the synchronization of the frame based on the protocol mode selected.

In Master mode, the module generates both the BCLK on the CK pin and the LRCK on the WS pin. In certain devices, while in Slave mode, the module receives these two clocks from its I2S partner, which is operating in Master mode.

When configured in Master mode, the leading edge of CK and the LRCK are driven out within one SCK period of starting the audio protocol. Serial data is shifted in or out with timings determined by the protocol mode set.

In Slave mode, the peripheral drives zeros out DO, but does not transmit the contents of the transmit FIFO until it sees the leading edge of the LRCK, after which time it starts receiving data.

## Master Mode

### Master Generating its Own Clock – Output BCLK and LRCK

![](GUID-CE2DE0EB-4569-46ED-8C1E-BB30E8C319B9-low.png)

## Slave Mode

### Codec Device as Master Derives MCLK from MCU Reference Clock Out

![](GUID-BF9BC1C9-632E-415D-AD19-4D18FEF2DEC3-low.png)

### Audio Formats

The I2SC Module supports just one audio format, I2S. Left Justified format is not available.

Using the Library

**Configuring the Library**

This section describes how to configure the peripheral library using the MHC.

**Description**

The library is configured for the supported processor when the processor is chosen in MPLAB X IDE, using the Microchip Harmony Configurator \(MHC\).

Choose the I2SC peripheral by clicking on the appropriate instance under *Peripherals-\>I2SC* in the Available Components section of MHC. \(The I2SC component may also be added automatically as a result of using a BSP Template, such as one for a codec or Bluetooth module\).

When the I2SCx peripheral is clicked on in the Project Graph, the following menu is displayed in the Configurations Options \(example shown for instance I2SC1\): ![](GUID-0C25CC5D-A012-4E12-98B8-09D800BD1068-low.png)

Default values are shown. The **DMA** and **Interrupt Mode** are always enabled.

**Master/Slave Mode** can be either Master -- the I2SC peripheral supplies the I2S clocks, or Slave -- the peripheral such as a codec or Bluetooth module supplies the I2S clocks.

**Data Word Length** can be selected from 8 to 32-bits.

**Data Format** is currently always I2S.

**Receiver Stereo/Mono** can be either Stereo or Mono \(left channel duplicated to right\).

**\# of DMA Channels for Receiver** can either Single or Multiple \(1/channel\).

**Loopback Test Mode** can be Normal or Loop mode.

**Transmitter Stereo/Mono** can be either Stereo or Mono \(left channel duplicated to right\).

**Transmit Data When Underrun** can be Transmit 0 or Transmit previous.

**Slot Width** can be 24 or 32-bits wide.

The following options are only shown if the Master/Slave mode is Master:

• **Selected Clock for IMCK Ratio** is selectable as any integer

## • Master Clock to Sample Rate Ratio can be set from 32 to 2048

• **Master Clock Mode** can be either Master Clock Generated or not

Library Interface

**Using the Library**

This topic describes the basic architecture of the I2SC Peripheral Library and provides information and examples on how to use it.

**Description**

## Abstraction Model

The I2SC module sits between the I2S Driver, and the actual hardware. ![](GUID-7FFA68F8-05F4-4557-A74E-B01D2423AA60-low.png)

### Interface Header File: plib\_i2sc.h

The interface to the I2SC Peripheral Library is defined in the plib\_i2sc.h header file. Any C language source \(.c\) file that uses the I2SC Peripheral Library should include plib\_i2sc.h.

**Library Source Files:**

The I2SC Peripheral Library library source files are provided in the csp/peripheral/i2sc\_xxxx/src or csp/peripheral/i2sc\_xxxx/templates directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features.

## Usage Model

The only usage model for the I2SC Peripheral Library is to use a Interrupt/DMA model. Therefore the remaining functions normally associated with a PLIB like this \(handling write or read requests, or returning transfer status\), will be accomplished by the I2S driver directly communicating with the appropriate DMA functions, which is why they are not provided here.

The one function provided besides initialization is one for synchronizing with the left/right clock \(LRCLK\) for the I2S stream.

**Example Applications:**

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/audio\_tone\_linkeddma

-   audio/apps/microphone\_loopback


Library Interface

**Library Interface**

This section describes the Application Programming Interface \(API\) functions of the Peripheral Library.

Refer to each section for a detailed description.

1.  **Initialization Function**


|-|**Name**|**Description**|
|--|--------|---------------|
||I2SCx\_Initialize|Initializes I2SCx module of the device|

1.  **Transaction Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||I2SCx\_LRCLK\_Get|Get the level of the I2S LRCLK \(left/right clock\) signal|

***a\) Initialization Function***

## I2SCx\_Initialize Function

void I2SCx\_Initialize \(void\);

**Summary**

Initializes I2SCx module of the device

**Description**

This function initializes I2SCx module of the device with the values configured in MHC GUI. Once the peripheral is initialized, transfer APIs can be used to transfer the data.

**Preconditions**

MHC GUI should be configured with the right values.

**Returns**

None.

**Remarks**

This function must be called only once and before any other I2SC function is called.

**Example**

I2SC0\_Initialize\(\);

**C**

**void** **I2SCx\_Initialize**\(\);

***b\) Transaction Functions***

## I2SCx\_LRCLK\_Get Function

uint32\_t I2SCx\_LRCLK\_Get\(void\);

Introduction

**Summary**

Get the level of the I2S LRCLK \(left/right clock\) signal

**Description**

This function returns the state of the I2S LRCLK \(left/right clock\) signal. In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize itself to the LRCLK signal.

**Preconditions**

None.

**Returns**

State of the LRCLK pin for the I2SCx module -- 1 if high, 0 if low

**Remarks**

None.

**Example**

I2SC1\_LRCLK\_Get\(\);

**C**

uint32\_t **I2SCx\_LRCLK\_Get**\(\);

***c\) Data Types and Constants***

**Files**

**Files**

|**Name**|**Description**|
|--------|---------------|
|plib\_i2sc\_11241.h|I2SC PLIB Header File for documentation|

**Description**

## *plib\_i2sc\_11241.h*

plib\_i2sc\_11241.h

**Summary**

I2SC PLIB Header File for documentation

**Description**

I2SC PLIB

This library provides documentation of all the interfaces which can be used to control and interact with an instance of an Inter-IC Sound Controller \(I2SC\). This file must not be included in any MPLAB Project.

**SPI-I2S Peripheral Library Help**

This section provides an interface to use the Inter-IC Sound \(I2S\) peripheral.

Configuring the Library

**Introduction**

This library provides a brief overview of the I2S peripheral. On PIC32MX/MZ processors, this peripheral is shared with the SPI \(Serial Peripheral Interface\) hardware.

**Description**

The I2S module implements an I2S \(Inter-IC Sound\) interface, for connection between an MCU and an audio peripheral such as a codec or Bluetooth module.

The I2S module provides support to the audio protocol functionality via four standard I/O pins. The four pins that make up the audio protocol interface modes are:

-   **SDI:** Serial Data Input for receiving sample digital audio data \(ADCDAT as output from the codec\)

-   **SDO:** Serial Data Output for transmitting digital audio data \(DACDAT as input to the codec\)

-   **SCK:** Serial Clock, also known as bit clock \(BCLK\)

-   **FS:** Frame Select, also known as Word Select or Left/Right Channel Clock \(LRCK\)


In addition, there is a fifth line, called REFCLKO \(Reference CLock Output, or Master Clock\), which can be used to drive the codec.

The SCK provides the clock required to drive the data out or into the module, while FS provides the synchronization of the frame based on the protocol mode selected. In I2S mode, the leading edge of audio data is driven out one SCK period of starting the frame.

In Master mode, the module generates both the SCK and FS.

In Slave mode, the peripheral generates the BCLK and LRCLK signals, driving the SCK and FS pins of the I2S module.

## Master Mode Master Generating its Own Clock – Output BCLK and LRCK

![](GUID-F0A6B110-F34A-42BF-A2A0-819FEFCF049E-low.png)

## Slave Mode Codec Device as Master Derives MCLK from MCU Reference Clock Out

![](GUID-84A5D851-9E27-43F3-8708-45BFDC894028-low.png)

**Configuring the Library**

This section describes how to configure the peripheral library using the MHC.

**Description**

The library is configured for the supported processor when the processor is chosen in MPLAB X IDE, using the Microchip

Library Interface

Harmony Configurator \(MHC\).

Choose the I2S peripheral by clicking on the appropriate instance under *Peripherals-\>I2S* in the Available Components section of MHC. \(The I2S component may also be added automatically as a result of using a BSP Template, such as one for a codec or Bluetooth module\).

When the I2S peripheral is clicked on in the Project Graph, the following menu is displayed in the Configurations Options: ![](GUID-4EFBB190-7FC4-45B2-B197-85DEAB8F0055-low.png)

Note: DMA and Interrupt Mode are always enabled.

**Master Mode Enable Bit --** only Master Mode is currently supported.

**Clock Polarity Select Bit** -- determines whether idle state of clock is high \(default\) or vice versa.

**SPI Clock Edge Select Bit** -- selects whether output chamges on transistion from active to idle \(default\) or vice versa.

**32/16 Bit Communication Select Bits** -- selects the data, FIFO, channel, and frame sizes.

**Master CLock Enable Bit** -- always selects REFCLK to be used by the baud rate generator.

**Audio Protocol Mode** -- I2S, left or right justified, or PCM.

**Transmit Audio Data Format** -- stereo or mono.

**Frame Select Pin** -- specifies which pin is used for the frame select \(aka L/R clock or word select\) **Invert** -- indicates whether the output of the LRCLK\_Get function is inverted or not

**Using the Library**

This section explains how to use the peripheral library.

**Description**

Library Source Files: The I2S Peripheral Library library source files are provided in the

audio/peripheral/i2s\_xxxx/src or audio/peripheral/i2s\_xxxx/templates directory. This folder may contain optional files and alternate implementations. Please refer to Configuring the Library **for** instructions on how to select optional features.

Usage Model The only usage model **for** the I2S Peripheral Library is to use a Interrupt/DMA model. Therefore the remaining functions normally associated with a PLIB like **this** \(handling write or read requests, or returning transfer status\), will be accomplished by the I2S driver directly communicating with the appropriate DMA functions, which is why they are not provided here. The one function provided besides initialization is one **for** synchronizing with the left/right clock \(LRCLK\) **for** the I2S stream. Example Applications:

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/microphone\_loopback


Library Interface

**Library Interface**

This section describes the Application Programming Interface \(API\) functions of the Peripheral Library.

Refer to each section for a detailed description.

1.  **Initialization Function**


|-|**Name**|**Description**|
|--|--------|---------------|
||I2Sx\_Initialize|Initializes I2S x module of the device|

1.  **Status Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||I2Sx\_LRCLK\_Get|Get LRCLK state|

***a\) Initialization Function***

## I2Sx\_Initialize Function

void I2Sx\_Initialize \(void\);

**Summary**

Initializes I2S x module of the device

**Description**

This function initializes I2S x module of the device with the values configured in MHC GUI. Once the peripheral is initialized, transfer APIs can be used to transfer the data.

**Preconditions**

MHC GUI should be configured with the right values.

**Returns**

None.

**Remarks**

This function must be called only once and before any other SPI function is called.

**Example**

I2S1\_Initialize\(\);

**C**

**void** **I2Sx\_Initialize**\(\);

***b\) Status Functions***

## I2Sx\_LRCLK\_Get Function

uint32\_t I2Sx\_LRCLK\_Get \(void\);

Introduction

**Summary**

Get LRCLK state **Description**

This function returns the state of the LRCLK \(left/right clock, aka word select line\) of the I2S peipheral.

**Preconditions**

MHC GUI should be configured with the right values.

**Returns**

None.

**Example** lrclkState = I2Sx\_LRCLK\_Get\(\);

**C**

uint32\_t **I2Sx\_LRCLK\_Get**\(\);

**Files**

**Files**

|**Name**|**Description**|
|--------|---------------|
|plib\_spi\_01329.h|SPI PLIB Header File for documentation|

**Description**

## *plib\_spi\_01329.h*

plib\_spi\_01329.h

**Summary**

SPI PLIB Header File for documentation

**Description**

SPI-I2S PLIB

This library provides documentation of all the interfaces which can be used to control and interact with an instance of a Serial Peripheral Interface \(SPI\) controller used for I2S. This file must not be included in any MPLAB Project.

**SSC Peripheral Library Help**

This section provides an interface to use the Serial Synchronous Controller \(SSC\) peripheral.

**Introduction**

This section provides a brief overview of the SSC peripheral.

**Description**

The SSC module implements an I2S \(Inter-IC Sound\) interface, for connection between an MCU and an audio peripheral such as a codec or Bluetooth module. The SSC hardware also provides for other protocols, which can be configured manually.

The SSC module provides support to the audio protocol functionality via four standard I/O pins. The four pins that make up the audio protocol interface modes are:

Configuring the Library

-   **RD:** Serial Data Input for receiving sample digital audio data \(ADCDAT\)

-   **TD:** Serial Data Output for transmitting digital audio data \(DACDAT\)

-   **TK/RK:** Transmit/Receive Serial Clock, also known as bit clock \(BCLK\)

-   **TF/RF:** Transmit/Receive Frame Clock, also known as Left/Right Channel Clock \(LRCK\)


BCLK provides the clock required to drive the data out or into the module, while LRCK provides the synchronization of the frame based on the protocol mode selected.

In Master mode, the module generates both the BCLK on the TK pin and the LRCK on the TF pin. In certain devices, while in Slave mode, the module receives these two clocks from its I2S partner, which is operating in Master mode.

When configured in Master mode, the leading edge of BCLK and the LRCK are driven out within one SCK period of starting the audio protocol. Serial data is shifted in or out with timings determined by the protocol mode set.

In Slave mode, the peripheral drives zeros out TD, but does not transmit the contents of the transmit FIFO until it sees the leading edge of the LRCK, after which time it starts receiving data.

## Master Mode

### Master Generating its Own Clock – Output BCLK and LRCK

![](GUID-79AC79AC-C2CA-4A50-9E4F-504E63A728B4-low.png)

## Slave Mode

### Codec Device as Master Derives MCLK from MCU Reference Clock Out

![](GUID-DF62A4EF-7C79-4EC1-B67D-C7E42E2EAB8E-low.png)

Default values are shown. The **DMA** and **Interrupt Mode** are always enabled.

**Usage Mode** can be either Master -- the SSC peripheral supplies the I2S clocks, or Slave -- the peripheral such as a codec or Bluetooth module supplies the I2S clocks.

**Audio Protocol Mode** can be either I2S, Left Justified, or Custom. In the latter case, a number of other options are then available, which allows the SSC peripheral to be used for a number of protocols \(not currently supported by existing drivers\).

**Data Length** is set to the number of data bits per channel.

**Using the Library**

This topic describes the basic architecture of the SSC Peripheral Library and provides information and examples on how to use it.

**Description**

## Abstraction Model

The SSC module sits between the I2S Driver, and the actual hardware. ![](GUID-9D52156A-2B78-409E-A603-C95968F1D8C0-low.png)

Note: Some devices may have only one instance of an SSC interface \(SSC0 only\).

### Interface Header File: plib\_ssc.h

The interface to the SSC Peripheral Library is defined in the plib\_ssc.h header file. Any C language source \(.c\) file that uses the SSC Peripheral Library should include plib\_ssc.h.

**Library Source Files:**

The SSC Peripheral Library library source files are provided in the audio/peripheral/ssc\_xxxx/src or audio/peripheral/ssc\_xxxx/templates directory. This folder may contain optional files and alternate implementations. Please refer to **Configuring the Library** for instructions on how to select optional features.

## Usage Model

The only usage model for the SSC Peripheral Library is to use a Interrupt/DMA model. Therefore many functions normally associated with a PLIB like this \(handling write or read requests, or returning transfer status\), will be accomplished by the I2S driver directly communicating with the appropriate DMA functions, which is why they are not provided here.

Library Interface

**Example Applications:**

This library is used by the following applications, among others:

-   audio/apps/audio\_tone

-   audio/apps/audio\_tone\_linkeddma


**Library Interface**

This section describes the Application Programming Interface \(API\) functions of the Peripheral Library.

Refer to each section for a detailed description.

1.  **Initialization Function**


|-|**Name**|**Description**|
|--|--------|---------------|
||SSC\_Initialize|Initializes SSC module of the device|

1.  **Transaction Functions**


|-|**Name**|**Description**|
|--|--------|---------------|
||SSC\_BaudSet|Changes the baud rate \(samples/second\) of the interface.|
||SSC\_LRCLK\_Get|Get the level of the I2S LRCLK \(left/right clock\) signal|

***a\) Initialization Function***

## SSC\_Initialize Function

void SSC\_Initialize \(void\);

**Summary**

Initializes SSC module of the device

**Description**

This function initializes SSC module of the device with the values configured in MHC GUI. Once the peripheral is initialized, transfer APIs can be used to transfer the data.

**Preconditions**

MHC GUI should be configured with the right values.

**Returns**

None.

**Remarks**

This function must be called only once and before any other SSC function is called.

**Example**

SSC\_Initialize\(\);

**C**

**void** **SSC\_Initialize**\(\);

***b\) Transaction Functions***

Files

## SSC\_BaudSet Function

void SSC\_BaudSet \(const uint32\_t baud\);

**Summary**

Changes the baud rate \(samples/second\) of the interface.

**Description**

This function Cchanges the baud rate, or samples/second of the interface.

**Preconditions**

None.

**Returns**

None.

**Remarks**

None.

**Example**

SSC\_BaudSet\(44100\);

**C**

**void** **SSC\_BaudSet**\(**const** uint32\_t **baud**\);

## SSC\_LRCLK\_Get Function

uint32\_t SSC\_LRCLK\_Get\(void\);

**Summary**

Get the level of the I2S LRCLK \(left/right clock\) signal

**Description**

This function returns the state of the I2S LRCLK \(left/right clock\) signal. In the case where this signal is generated from a codec or other external source, this allows the caller to synchronize itself to the LRCLK signal.

**Preconditions**

None.

**Returns**

State of the LRCLK pin for the SSC module -- 1 if high, 0 if low if the audio format is I2S, and 0 if high, 1 if low if the format is Left Jusutifed

**Remarks**

None.

**Example**

SSC\_LRCLK\_Get\(\);

**C**

uint32\_t **SSC\_LRCLK\_Get**\(\);

Files

**Files**

**Files**

|**Name**|**Description**|
|--------|---------------|
|plib\_ssc\_6078.h|SSC PLIB Header File for documentation|

**Description**

## *plib\_ssc\_6078.h*

plib\_ssc\_6078.h

**Summary**

SSC PLIB Header File for documentation

**Description**

SSC PLIB

This library provides documentation of all the interfaces which can be used to control and interact with an instance of a Serial Synchronous Controller \(SSC\). This file must not be included in any MPLAB Project.

