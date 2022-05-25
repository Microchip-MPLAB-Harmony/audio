# Microchip MPLAB Harmony 3 Release Notes

## Audio Release v3.6.0 (May, 2022)
### Additions and Updates for 3.6.0:

Added SDL support.
**As of audio release v3.6.0, applications that were previously in the audio repository have been moved to the new audio_apps repository:**
[Harmony audio apps repository, 3.1.0](https://github.com/Microchip-MPLAB-Harmony/audio_apps)

- **Updated Drivers**

The following table provides the list of updated audio drivers:

| Driver | Description | MZ EF/C2 | E54 | E70 |
| --- | --- | --- | --- | --- | 
| AK4954 | Fix mic gain and missing function | x | x |  x |
| AK4954 | Fix race condition | x | x |  x |
| I2S | Fix macro problem | x | x |  x |
| WM8904 | Fix missing break statements | x | x |  x |

where:

| |  Development Kit |
| --- | --- | 
|**MZ EF/C2**| [Curiosity PIC32MZEF Dev Board 2.0](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320209) |
|**E54**| [SAM E54 Curiosity Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320210) |
|**E70**| [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320113) |


- **Updated Peripheral Libraries (PLIBs)**

The following table provides the list of updated audio peripheral libraries:

| PLIB | Description | MZ EF/C2 | E54 | E70 |
| --- | --- | --- | --- | --- | 
| spi_01329 | Supply local min function not provided by Linux | x |  |  |


- **Updated Libraries**

The following table provides the list of updated libraries:

| Library | Description | MZ EF/C2 | E54 | E70 |
| --- | --- | --- | --- | --- | 
| Decoder Library | Added FLAC wrapper for Xiph FLAC Decoder Library | x | | x |


### TESTED WITH:

#### Software Dependencies

Before using MPLAB Harmony Audio, ensure that the following are installed:

- [MPLAB X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide) or later
- [MPLAB XC32 C/C++ Compiler v4.10](https://www.microchip.com/mplab/compilers) or later
- [Harmony audio repository, 3.6.0](https://github.com/Microchip-MPLAB-Harmony/audio)

To build applications, you now need to also have the separate audio apps repository:

- [Harmony audio apps repository, 3.1.0](https://github.com/Microchip-MPLAB-Harmony/audio_apps)

## Audio Release v3.5.1 (June, 2021)
### Updates for 3.5.1:

- **Updated Drivers**

The following table provides the list of updated audio drivers:

| Driver | Description | BTADK | BTADK/MZ | MZ EF/C2 | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| All | Fixes for new compiler switches and XC32 3.0 compiler | x | x | x | x | x | x |
| AK4954 | Remove 32-bit data formats | x | x |  x | x | x | |
| WM8904 | Always tries to open I2C Driver instance 0 | x | x |  x | x | x | x |

- **Updated Peripheral Libraries (PLIBs)**

The following table provides the list of updated audio peripheral libraries:

| PLIB | Description | BTADK | BTADK/MZ | MZ EF/C2 | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| All | Fixes for new compiler switches and XC32 3.0 compiler | x | x | x | x | x | x |
| i2sc 11241 | Fix incorrect names being created for some functions | | | | | x | |
| i2sc 11241 | Remove unused clock-related functions | | | | | x | |
| i2s u2224 | Remove references to deleted TXCTRL bits | | | | x | | |

## Audio Release v3.5.0 (February, 2020)
### Additions and Updates for 3.4.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects:

| Application | Description | MZ EF/C2 | E70 |
| --- | --- | --- | --- |
| universal_audio_decoders | New application, both with and w/o display, works with WAV, ADPCM, and MP3 files | x | x |
| usb_speaker | Added MZEF C2 variation | x | |
| usb_speaker_bass_boost | New application, with variable bass boost | x | x |
| usb_speaker_hi_res | Added FreeRTOS variation | | x |


- **Updated Applications/Projects**

The following table provides the list of updated applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- |--- |
| audio_player_basic | Modified so it now works only with WAV files, without library | x | x |

- **New Drivers**

The following table provides the list of new audio drivers:

| Driver | Description | MZ EF |
| --- | --- | --- |
| AK4953 | AK4953 | x |

- **Updated Drivers**

The following table provides the list of updated audio drivers:

| Driver | Description | BTADK | BTADK/MZ | MZ EF/C2 | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| I2S | Added functions to set master clock | x | x | x | | | |
| I2S | Fixed two issues with DMA | x | x | x | x | x | x |
| AK4954 | Added delayed driver initialization | x | x |  x| x | x | |

where:

| |  Development Kit |
| --- | --- | 
|**BTADK**| [PIC32 Bluetooth Audio Development Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DV320032) |
|**BTADK/MZ**| [BTADK with PIC32MZ EF Audio 144-pin PIM](https://www.microchip.com/developmenttools/ProductDetails/PartNO/MA320018)
|**MZ EF/C2**| [Curiosity PIC32MZEF Dev Board 2.0](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320209) |
|**E54**| [SAM E54 Curiosity Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320210) |
|**E70**| [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/DM320113) |
|**V71**| [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ATSAMV71-XULT) |


- **Updated Peripheral Libraries (PLIBs)**

The following table provides the list of updated audio peripheral libraries:

| PLIB | Description | MZ EF/C2 |
| --- | --- | --- |
| spi_01329 | Added functions to set master clock | x |

- **New Libraries**

The following table provides the list of new libraries:

| Library | Description | BTADK | BTADK/MZ | MZ EF/C2 | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Math/DSP Libraries | Added LIBQ_C, LIBQ and DSP libraries | x | x | x | x | x | x |

- **Updated Libraries**

The following table provides the list of updated libraries:

| Library | Description | BTADK | BTADK/MZ | MZ EF/C2 | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Decoder Library | Added MP3 wrapper for Helix MP3 Decoder Library and ID3 tag functions | x | x | x | x | x | x |

- **Updated Audio Application Templates**

The following table provides the list of updated templates:

| Template | Description | BTADK/MZ | MZ EF/C2 |
| --- | --- | --- | --- | 
| ak4954 | Specify bit-banged I2C driver for MZ EF | x | |
| ak4954 | Specify I2C driver for MZ EF/C2 | | x |

## Audio Release v3.4.0 (August, 2019)
### ADDITIONS AND UPDATES FOR 3.4.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects:

| Application | Description | BTADK | BTADK/MZ | C2/MZ | E70 |
| --- | --- | --- | --- | --- | --- |
| audio_tone | Added BTADK and C2 variations using AK4954 | x | x | x | |
| microphone_loopback | Added BTADK variation using AK4954 | | x | | |
| usb_microphone | New application, bare-metal and FreeRTOS versions | | | | x |
| usb_speaker | Added I2SC variation | | | | x |
| usb_speaker_hi_res | New application; plays 96K audio from a USB host such as a PC | | | | x |

- **Updated Applications/Projects**

The following table provides the list of updated applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- |--- |
| audio_player_basic | Added ADPCM decoder | x | x |
| usb_speaker | Fixed buffer overflow/underrun problem || x |

- **New Drivers**

The following table provides the list of new audio drivers:

| Driver | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| I2S | I2S Driver | x | x | x | x | x | x |

- **Updated Drivers**

The following table provides the list of updated audio drivers:

| Driver | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| I2S | Added audio data definitions | x | x | x | x | x | x |
| WM8904 | Added MIC port selection to WM8904 codec driver | | | | x | x | x |

- **New Peripheral Libraries (PLIBs)**

The following table provides the list of new audio peripheral libraries:

| PLIB | Description | BTADK | BTADK/MZ | C2/MZ | E54 | E70 | V71 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| i2s_u2224 | I2S Peripheral Library | | | | x | | |
| i2sc_11241 | I2SC Peripheral Library | | | | | x | x |
| spi_01329 | I2S Peripheral Library | x | x | x | | | |
| ssc_6078 |  SSC Peripheral Library | | | | | x | x |

- **New Audio Application Templates**

The following table provides the list of new audio templates:

| Template | Description | BTADK | BTADK/MZ | C2/MZ |
| --- | --- | --- | --- | --- | 
| ak4954 | AK4954 Codec with BTADK (MX) | x | | |
| ak4954 | AK4954 Codec with BTADK (MZ/PIM) | | x | |
| ak4954 | AK4954 Codec with Curiosity MZ 2.0  | | | x |
| generic | Generic Codec with BTADK (MX) | x | | |
| generic | Generic Codec with BTADK (MZ/PIM)| | x | |
| generic | Generic Codec with Curiosity MZ 2.0  | | | x |

## Audio Release v3.3.0 (May, 2019)
### ADDITIONS AND UPDATES FOR 3.3.0:

- **New Applications/Projects**

The following table provides the list of new applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- | --- |
| audio_player_basic | Added project using WM8904 and SD card instead of USB, including FreeRTOS version | x | |
| audio_player_basic | Added graphics and FreeRTOS projects using WM8904| | x |
| audio_tone | Added bare metal and FreeRTOS projects using WM8904 | x | |
| audio_tone | Added project using AK4954 | | x |
| microphone_loopback | Added bare metal and FreeRTOS projects using WM8904 | x | |
| usb_speaker | Added FreeRTOS project | | x |

- **Updated Applications/Projects**

The following table provides the list of updated applications/projects:

| Application | Description | E54 | E70 |
| --- | --- | --- | --- |
| audio_player_basic | All projects now use WAV library instead of inline decoding; fixed issue in WAV decoder where inSize argument was ignored and substituted with a fixed length | | x |

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
| audio_enc |  Records and encodes PCM audio with optional playback, including a FreeRTOS project |
| audio_signal_generator | Generates audio sine, square, sawtooth and triangle waves using SSC with graphics display |
| usb_speaker | Plays audio from a USB host such as a PC |

- **Updated Drivers**

The following table provides the list of new support for audio drivers for the SAM E70/V71:

| Driver | Description |
| --- |  --- |
| WM8904| Added microphone gain API|

- **Updated Applications**

The following table provides the list of updated applications for the SAM E70:

| Application | Description |
| --- | --- |
| audio_player_basic | Fixed a bug that caused slow playback of 96K sample rate WAVE files |
| microphone_loopback | Added graphics and FreeRTOS projects |

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
| audio_player_basic | Plays PCM wave files from USB dongle using SSC|
| audio_tone |  Sine wave audio tone generator using I2SC or SSC, including a FreeRTOS project|
| audio_tone_linkeddma | Sine wave audio tone generator using linked DMA and I2SC or SSC |
| microphone_loopback | Outputs audio from microphone after optional delay using I2SC |

The following table provides the list of new applications for the SAM V71:

| Application | Description |
| --- | --- |
| audio_tone |  Sine wave audio tone generator using SSC |
| microphone_loopback | Outputs audio from microphone after optional delay using SSC |




