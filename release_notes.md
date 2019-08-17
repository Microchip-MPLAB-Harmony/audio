# Microchip MPLAB Harmony 3 Release Notes
## Audio Release v3.4.0 (August, 2019)
### ADDITIONS AND UPDATES FOR 3.4.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects:

| Application | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 |
| --- | --- | --- | --- | --- | --- | --- |
| audio_player_basic | added ADPCM decoder | | | | x | x |
| audio_tone | added BTADK and C2 variations using AK4954 | x | x | x | | |
| microphone_loopback | added BTADK variation using AK4954 | | x | | | |
| usb_microphone | new app, bare-metal and FreeRTOS versions | | | | | x |
| usb_speaker | added I2SC variation | | | | | x |
| usb_speaker_hi_res | new application | | | | | x |

- **Updated Applications/Projects**

The following table provides the list of updated applications/projects:

| Application | Description | E70 |
| --- | --- | --- |
| usb_speaker | Fixed buffer overflow/underrun problem | x |

- **New Drivers**

The following table provides the list of new audio drivers (moved from core repo):

| Driver | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 |
| --- | --- | --- | --- | --- | --- | --- |
| I2S | I2S Driver | x | x | x | x | x |


**NOTE:** The I2S driver listed above was previously in the core repo, and is now located in the audio repo instead.  All of the audio projects have been updated to use the new location.  Howevever if someone has copied an application from a previous version (e.g. 3.3) of the audio repo, it will need to be updated to reflect this change. After noting any special settings in the MHC dialog for the driver, the I2S driver should be removed from the project graph, and the same one selected again from the audio repo instead, and the appropriate connections restored.

Although the codec drivers were already in the audio repo, their location was changed bwteen versions 3.3 and 3.4, so they need to be removed and then added back as well. 

- **Updated Drivers**

The following table provides the list of new audio drivers (moved from core repo):

| Driver | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 |
| --- | --- | --- | --- | --- | --- | --- |
| I2S | Added audio data definitions | x | x | x | x | x |
| WM8904 | Added MIC port selection to WM8904 codec driver | | | | x | x |

- **New Peripheral Libraries (PLIBs)**

The following table provides the list of new audio peripheral libraries:

| Driver | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 |
| --- | --- | --- | --- | --- | --- | --- |
| i2s_u2224 | I2S Peripheral Library | | | | x | |
| i2sc_11241 | I2SC Peripheral Library | | | | | x |
| spi_01329 | I2S Peripheral Library | x | x | x | | |
| ssc_6078 |  SSC Peripheral Library | | | | | x |

**NOTE:** The audio I2S-related peripheral libraries listed above were previously in the csp repo, and are now located in the audio repo instead.  All of the audio projects have been updated to use the new location.  Howevever if someone has copied an application from a previous version (e.g. 3.3) of the audio repo, it will need to be updated to reflect this change. After noting any special settings in the MHC dialog for the peripheral, any I2S-related PLIB should be removed from the project graph, and the same one selected again from the audio repo instead, and the appropriate connections restored..

- **New Audio Application Templates**

The following table provides the list new templates:

| Template | Description | BTADK | BTADK/MZ | C2/MZ |
| --- | --- | --- | --- | --- | 
| ak4954 | AK4954 Codec with BTADK (MX) | x | | |
| ak4954 | AK4954 Codec with BTADK (MZ {PIM) | | x | |
| ak4954 | AK4954 Codec with Curiosity MZ 2.0  | | | x |
| generic | Generic Codec with BTADK (MX) | x | | |
| generic | Generic Codec with BTADK (MZ {PIM)| | x | |
| generic | Generic Codec with Curiosity MZ 2.0  | | | x |

### TESTED WITH:

#### Software Dependencies

Before using MPLAB Harmony Audio, ensure that the following are installed:

- [MPLAB X IDE v5.20](https://www.microchip.com/mplab/mplab-x-ide) or later
- [MPLAB XC32 C/C++ Compiler v2.20](https://www.microchip.com/mplab/compilers) or later
- [Harmony audio repository, 3.4.0](https://github.com/Microchip-MPLAB-Harmony/audio)

In order to regenerate source code for any of the applications, you will also need the following to be installed:

| Tool | Version |
| --- | --- |
| [MPLAB® Harmony Configurator (MHC)](https://github.com/Microchip-MPLAB-Harmony/mhc) | v3.3.0 |
| MPLAB® Harmony Configurator (MHC) plug-in   | v3.3.0.1 |
| [Harmony 3 BSP](https://github.com/Microchip-MPLAB-Harmony/bsp)   | v3.4.0 |
| [Harmony 3 CSP](https://github.com/Microchip-MPLAB-Harmony/csp)  | v3.4.0 |
| [Harmony 3 Core](https://github.com/Microchip-MPLAB-Harmony/core)  | v3.4.0 |
| [Harmony 3 Graphics ](https://github.com/Microchip-MPLAB-Harmony/gfx)   | v3.4.0 |
| [Harmony 3 Dev_Packs](https://github.com/Microchip-MPLAB-Harmony/dev_packs)   | v3.4.0 |
| [Harmony 3 USB](https://github.com/Microchip-MPLAB-Harmony/usb)   | v3.3.0 |
| [Harmony 3 CMSIS-FreeRTOS](https://github.com/ARM-software/CMSIS-FreeRTOS)   | v10.2.0 |

#### Development Kit Support

This release supports applications for the following development kits

| Development Kits |
| --- |
| [PIC32 Bluetooth Audio Development Kit/BTADK (DV320032)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DV320032) |
| [PIC32MZ EF Audio 144-pin PIM for BTADK (MA320018)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/MA320018)
| [Curiosity PIC32MZEF Dev Board 2.0 (DM320209)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320209) |
| [SAM E54 Curiosity Ultra Evaluation Kit (DM320210)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320210) |
| [SAM E70 Xplained Ultra Evaluation Kit (DM320113)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320113) |
| [SAM V71 Xplained Ultra Evaluation Kit (ATSAMV71-XULT)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ATSAMV71-XULT) |

### KNOWN ISSUES

The current known issues are as follows:

* Code is compliant to MISRA C 2012 Mandatory guidelines, except applications using graphics (Rule 9.1).
* The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. When running demo projects using the V71, the ICD4 flex cable should be removed after programming to run the application.
* Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by the drivers and libraries is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the *Configuring the Library* section in the help documentation in the doc folder for the relevant Harmony 3 module instead.  Help is available in both CHM and PDF formats.
* audio_enc demonstration:
    - Audible artifacts may occur as a result of long period MSD writes.
    - Functionality deteriorates as higher sampling rates.
    - Audible artifacts may occur when using FreeRTOS.
* audio_player_basic demonstration:
    - A clicking sound is heard before the playback of a
track.
    - Playback of files with more than 2 channels is not
supported. May result in garbled audio and longer
playback times.
    - Only CBR (Constant Bit Rate) encoding is supported.
    - Audible artifacts and/or L/R channel swapping may occur for some configurations.
* audio_signal_generator demonstration:
    - Not all frequencies selected can be output. The nearest possible frequency to the one chosen will be used.
    - Dropouts may occur when switching buffers. 
* audio_tone demonstration:
    - In both E54/WM8904 projects, only 32-bit audio has been tested.
    - Dropouts or ticking sound may occur when switching buffers. 
* audio_tone_linkeddma demonstration:
    - When changing the frequency the first time, the pushbutton must be pressed twice.
* microphone_loopback demonstration:
    - In both E54/WM8904 projects, only 32-bit audio has been tested.
* usb_speaker_hi_res demonstration:
    - Sometimes the PC audio volume level must to be lowered to prevent clipping of the codec data.  The playback sounds like a loud hiss or that the audio is popping when this happens.

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
| audio\apps\usb_speaker | USB speaker application | Production |
| audio\apps\usb_speaker_hi_res | USB speaker application (hi-res) | Beta |
| audio\driver\i2s | I2S Driver | Beta |
| audio\driver\codec\AK4954 | AK4954 Codec Driver | Beta |
| audio\driver\codec\Generic | Generic Codec Driver | Beta |
| audio\driver\codec\WM8904 | WM8904 Codec Driver | Production |
| audio\peripheral\i2s_u2224 | I2S Peripheral Library (E54) | Beta |
| audio\peripheral\i2sc_11241 | I2SC Peripheral Library (E70) | Beta |
| audio\peripheral\spi_01329 | I2S Peripheral Library (MX/MZ)| Beta |
| audio\peripheral\ssc_6078 | SSC Peripheral Library (E70/V71) | Beta |
| audio\templates\ak4954 | Audio application template | Beta |
| audio\templates\generic | Audio application template | Beta |
| audio\templates\wm8904 | Audio application template | Production |
| audio\decoder\audio_decoders\adpcm | Decoder Library | Beta |
| audio\decoder\audio_decoders\wav | Decoder Library | Beta |
| audio\encoder\audio_encoders\adpcm | Encoder Library | Beta |
| audio\encoder\audio_encoders\pcm | Encoder Library | Beta |

## Audio Release v3.3.0 (May, 2019)
### ADDITIONS AND UPDATES FOR 3.3.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- | --- |
| audio_player_basic | added project using WM8904 and SD card instead of USB, including FreeRTOS version | x | |
| audio_player_basic | added graphics and FreeRTOS projects using WM8904| | x |
| audio_tone | added bare metal and FreeRTOS projects using WM8904 | x | |
| audio_tone | added project using AK4954 | | x |
| microphone_loopback | added bare metal and FreeRTOS projects using WM8904 | x | |
| usb_speaker | added FreeRTOS project | | x |

- **Updated Applications/Projects**

The following table provides the list of updated applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- | --- |
| audio_player_basic | all projects now use WAV library instead of inline decoding; fixed issue in WAV decoder where inSize argument was ignored and substituted with a fixed length | | x |

- **New Drivers**

The following table provides the list of new audio drivers:

| Driver | Description | E54 | E70 |
| --- | --- | --- | --- |
| AK4954| AK4954 Codec Driver | | x |
| Generic Codec| Generic Codec Driver | x | x |
| Encoder Library| Encoder Library with ADPCM and PCM encoders | x | x |
| Decoder Library|  Decoder Library with ADPCM and WAV decoders | x |  x|

- **New Audio Application Templates**

The following table provides the list new templates:

| Template | Description | E54 | E70 |
| --- | --- | --- | --- |
| ak4954 | AK4954 Codec with E70 Xplained Ultra | | x |
| generic | Generic Codec with E54 Curiosity Ultra | x | |
| generic | Generic Codec with E70 Xplained Ultra | | x |
| wm8904 | WM8904 Codec with E54 Curiosity Ultra | x | |

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




