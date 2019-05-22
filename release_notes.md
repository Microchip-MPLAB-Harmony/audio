# Microchip MPLAB Harmony 3 Release Notes
## Audio Release v3.3.0 (May, 2019)
### ADDITIONS AND UPDATES FOR 3.3.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects for the SAM E54:

| Application | Description |
| --- | --- |
| audio_player_basic | added project using WM8904 and SD card instead of USB, including FreeRTOS version |
| audio_tone | added bare metal and FreeRTOS projects using WM8904 |
| microphone_loopback | added bare metal and FreeRTOS projects using WM8904  |

The following table provides the list of new applications/projects for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | added graphics and FreeRTOS projects using WM8904|
| audio_tone | added project using AK4954 |
| microphone_loopback | added project using AK4954|
| usb_speaker | added FreeRTOS project |

- **Updated Applications/Projects**

The following table provides the list of updated applications/projects for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | all projects now use WAV library instead of inline decoding; fixed issue in WAV decoder where inSize argument was ignored and substituted with a fixed length |

- **New Drivers**

The following table provides the list of new audio drivers for the SAM E54:

| Driver | Description |
| --- | --- |
| Generic Codec| Generic Codec Driver|
| Encoder Library| Encoder Library with ADPCM and PCM encoders|
| Decoder Library|  Decoder Library with ADPCM and WAV decoders|

The following table provides the list of new audio drivers for the SAM E70/V71:

| Driver | Description |
| --- | --- |
| AK4954| AK4954 Codec Driver|
| Generic Codec| Generic Codec Driver|
| Encoder Library| Encoder Library with ADPCM and PCM encoders|
| Decoder Library| Decoder Library with ADPCM and WAV decoders|

- **New Audio Application Templates**

The following table provides the list new templates for the SAM E54:

| Template | Description |
| --- | --- |
| generic | Generic Codec with E54 Curiosity Ultra  |
| wm8904 | WM8904 Codec with E54 Curiosity Ultra  |

The following table provides the list new templates for the SAM E70:

| Template | Description |
| --- | --- |
| ak4954 | AK4954 Codec with E70 Xplained Ultra  |
| generic | Generic Codec with E70 Xplained Ultra  |

### TESTED WITH:

#### Software Dependencies

Before using MPLAB Harmony Audio, ensure that the following are installed:

- [MPLAB X IDE v5.20](https://www.microchip.com/mplab/mplab-x-ide) or later
- [MPLAB XC32 C/C++ Compiler v2.20](https://www.microchip.com/mplab/compilers) or later
- Harmony audio repository, 3.3.0

In order to regenerate source code for any of the applications, you will also need the following to be installed:

- MPLAB Harmony Configurator (MHC) v.3.3.0
- Harmony mplabx_plugin repository, 3.3.0
- Harmony bsp repository, 3.3.0
- Harmony csp repository, 3.3.0
- Harmony core repository, 3.3.0
- Harmony dev_packs repository, 3.3.0
- Harmony gfx repository, 3.3.0 (if building a project with graphics)
- Harmony usb repository, 3.2.2 (if building a project using USB)
- CMSIS-FreeRTOS repository, 10.2.0 if building a FreeRTOS project (from www.github.com/arm-software/cmsis-freertos)

#### Development Kit Support

This release supports applications for the following development kits

| Development Kits |
| --- |
| [SAM E54 Curiosity Ultra Evaluation Kit](https://www.microchip.com/developmenttools/listing/) |
| [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320113) |
| [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ATSAMV71-XULT) |

### KNOWN ISSUES

The current known issues are as follows:

* All code is compliant to MISRA C 2012 Mandatory guidelines, except applications using graphics (Rules R.9.1 and R.17.3)
* The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. When running demo projects using the V71, the ICD4 flex cable should be removed after programming to run the application.
* Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by the drivers and libraries is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the *Configuring the Library* section in the help documentation in the doc folder for the relevant Harmony 3 module instead.  Help is available in both CHM and PDF formats.
* audio_enc demonstration:
    - Audible artifacts may occur as a result of long period MSD writes.
    - Functionality deteriorates as higher sampling rates.
    - Audible artifacts may occur when using FreeRTOS
* audio_player_basic demonstration:
    - A clicking sound is heard before the playback of a
track.
    - Playback of only 16-bit data stereo samples are
currently supported.
    - Playback of files with more than 2 channels is not
supported. May result in garbled audio and longer
playback times.
    - Only CBR (Constant Bit Rate) encoding is supported.
    - Audible artifacts may occur when using FreeRTOS
* audio_signal_generator demonstration:
    - Not all frequencies selected can be output.  The nearest possible frequency to the one chosen will be used.
* audio_tone demonstration:
    - In both E54/WM8904 projects, only 32-bit audio has been tested.  
* audio_tone_linkeddma demonstration:
    - When changing the frequency the first time, the pushbutton
must be pressed twice.
* microphone_loopback demonstration:
    - In both E54/WM8904 projects, only 32-bit audio has been tested.
* usb_speaker demonstration:
    - Left-Right Channel switching can occur every 45 seconds and when changing track using the computer mouse control. 

### RELEASE CONTENTS

This topic lists the contents of this release and identifies each module.

#### Description

This table lists the contents of this release, including a brief description, and the release type (Alpha, Beta, Production, or Vendor).


| Folder | Description | Release Type |
| --- | --- | --- |
| audio\apps\audio_enc | audio encoder application | Beta |
| audio\apps\audio_player_basic | audio_player_basic application | Production |
| audio\apps\audio_signal_generator | audio signal generator application | Beta |
| audio\apps\audio_tone  | audio tone application | Production |
| audio\apps\audio_tone_linkeddma | audio tone using linked dma application | Production |
| audio\apps\microphone_loopback | microphone loopback application | Production |
| audio\apps\usb_speaker | USB speaker application | Beta |
| audio\driver\codec\AK4954 | AK4954 Codec Driver | Beta |
| audio\driver\codec\Generic | Generic Codec Driver | Beta |
| audio\driver\codec\WM8904 | WM8904 Codec Driver | Production |
| audio\templates\ak4954 | Audio application template | Beta |
| audio\templates\generic | Audio application template | Beta |
| audio\templates\wm8904 | Audio application template | Production |
| audio\decoder\audio_decoders\adpcm | Decoder Library | Beta |
| audio\decoder\audio_decoders\wav | Decoder Library | Beta |
| audio\encoder\audio_encoders\adpcm | Encoder Library | Beta |
| audio\encoder\audio_encoders\pcm | Encoder Library | Beta |

## Audio Release v3.2.0 (March, 2019)
### ADDITIONS AND UPDATES FOR  3.2.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects for the SAM E70:

| Application | Description |
| --- | --- |
| audio_enc |  records and encodes PCM audio with optional playback, including a FreeRTOS project |
| audio_signal_generator | generates audio sine, square, sawtooth and triangle waves using SSC with graphics display |
| usb_speaker | plays audio from a USB host such as a PC |

- **Updated Drivers**

The following table provides the list of new support for audio drivers for the SAM E70/V71:

| Driver | Description |
| --- |  --- |
| WM8904| Added microphone gain API|

- **Updated Applications**

The following table provides the list of updated applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | fixed a bug that caused slow playback of 96K sample rate WAVE files |
| microphone_loopback | added graphics and FreeRTOS projects |

## Audio Release v3.1.0 (January, 2019)
### ADDITIONS FOR 3.1.0:

- **New Drivers**

The following table provides the list of new audio drivers for the SAM E70/V71:

| Driver | Description |
| --- | --- |
| WM8904| WM8904 Codec Driver|

- **New Audio Application Templates**
 
The following table provides the list new templates for the SAM E70/V71:

| Template | Description |
| --- | --- |
| wm8904 | WM8904 Codec with E70 or V71 Xplained Ultra  |

- **New Applications**

The following table provides the list of new applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | plays PCM wave files from USB dongle using SSC|
| audio_tone |  sine wave audio tone generator using I2SC or SSC, including a FreeRTOS project|
| audio_tone_linkeddma | sine wave audio tone generator using linked DMA and I2SC or SSC |
| microphone_loopback | outputs audio from microphone after optional delay using I2SC |

The following table provides the list of new applications for the SAM V71:

| Application | Description |
| --- | --- |
| audio_tone |  sine wave audio tone generator using SSC |
| microphone_loopback | outputs audio from microphone after optional delay using SSC |




