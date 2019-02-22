# Microchip MPLAB Harmony 3 Release Notes
## Audio Release v3.2.0 (March, 2019)
### Software Requirements

Before using MPLAB Harmony Audio, ensure that the following are installed:

- [MPLAB X IDE v5.15](https://www.microchip.com/mplab/mplab-x-ide) or later
- [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers) or later
- MPLAB Harmony Configurator (MHC) v.3.2.0
- Harmony audio repository, 3.2.0
- Harmony bsp repository, 3.2.0
- Harmony csp repository, 3.2.0
- Harmony core repository, 3.2.0
- Harmony dev_packs repository, 3.2.0
- Harmony gfx repository, 3.2.0 (if building an app with graphics)
- Harmony usb repository, 3.2.0 (if building an app using USB)
- Harmony mplabx_plugin repository, 3.2.0
- CMSIS-FreeRTOS repository, 10.0.1 if building a FreeRTOS project (from www.github.com/arm-software/cmsis-freertos)

### Development Kit Support

- **Development kit support** - This release supports applications for the following development kits

| Development Kits |
| --- |
| [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAME70-XULT) |
| [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMV71-XULT) |

### Additions and Updates for 3.2.0:

- **New Applications**

The following table provides the list of new applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_enc |  records and encodes PCM audio with optional playback, including FreeRTOS project |
| audio_signal_generator | generates audio sine, square, sawtooth and triangle waves using SSC with graphics display |
| usb_microphone | acts as USB microphone for USB host such as a PC, including FreeRTOS project |
| usb_speaker | plays audio from USB host such as a PC, including graphics and FreeRTOS projects |


- **Updated Applications**

The following table provides the list of updated applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | added FreeRTOS project |
| microphone_loopback | added graphics and FreeRTOS projects |


### Additions for 3.1.0:

- **New driver support**

The following table provides the list of new support for audio drivers:

| Driver | Name | Feature |
| --- | --- | --- |
| WM8904| WM8904 | Added WM8904 Codec Driver|

- **New Audios Application Templates**
 
The following table provides the list templates for different product families:

| Template | Description |
| --- | --- |
| wm8904 | WM8904 Codec with E70 or V71 Xplained Ultra  |

- **New Applications**

The following table provides the list of new applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | plays wave files from USB dongle using SSC|
| audio_tone |  sine wave audio tone generator using I2SC or SSC, including one FreeRTOS project|
| audio_tone_linkeddma | sine wave audio tone generator using linked DMA and I2SC or SSC |
| microphone_loopback | outputs audio from microphone after optional delay using I2SC |

The following table provides the list of new applications for the SAM V71:

| Application | Description |
| --- | --- |
| audio_tone |  sine wave audio tone generator using SSC |
| microphone_loopback | outputs audio from microphone after optional delay using SSC |

### KNOWN ISSUES

The current known issues are as follows:

* The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. When running demo projects using the V71, the ICD4 flex cable should be removed after programming to run the application.
* Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the *Configuring the Library* section in the help documentation in the doc folder for this Harmony 3 module instead.  Help is available in both CHM and PDF formats.
* audio_player_basic demonstration:
    - A clicking sound is heard before the playback of a
track.
    - Playback of only 8 and 16 bit data samples are
currently supported. The app will skip any files with
different data sample sizes.
    - Playback of files with more than 2 channels is not
supported. May result in garbled audio and longer
playback times.
* audio_tone_linkeddma demonstration:
    - When changing the frequency the first time, SW1
must be pressed twice.

### RELEASE CONTENTS

This topic lists the contents of this release and identifies each module.

#### Description

This table lists the contents of this release, including a brief description, and the release type (Alpha, Beta, Production, or Vendor).


| Folder | Description | Release Type |
| --- | --- | --- |
| audio\apps\audio_enc | audio encoder application | Beta |
| audio\apps\audio_player_basic | audio_player_basic application | Beta |
| audio\apps\audio_signal_generator | audio signal generator application | Beta |
| audio\apps\audio_tone  | audio tone application | Beta |
| audio\apps\audio_tone_linkeddma | audio tone using linked dma application | Beta |
| audio\apps\microphone_loopback | microphone loopback application | Beta |
| audio\apps\usb_microphone | USB microphone application | Beta |
| audio\apps\usb_speaker | USB speaker application | Beta |
| audio\driver\codec\WM8904 | WM8904 Codec Driver | Beta |
| audio\templates\wm8904 | Audio application template for SAM E70/V71 Xplained Ultra | Beta |


