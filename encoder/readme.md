# Audio Libraries Help

## Encoder Library Help

This section provides descriptions of the software Encoder Libraries that are available in MPLAB Harmony.

### ADPCM Encoder Library

##### _Introduction_

Adaptive Differential Pulse Code Modulation (ADPCM) is a royalty-free audio codec. It is a lossy compression standard. It is a variant of Differential Pulse-Code Modulation (DPCM). The file format used to contain an ADPCM files, is the same as WAV.

Its popularity comes because it achieves compression of 4 to 1 but does not require high processing power.

Typical applications using ADPCM are for speech. This implementation currently supports 16 bit stereo.

ADPCM Ecncoder Library

**_Using the Library_**

#### _Library Interface_

**Functions**

| **Name** | **Description** |
| --- | --- |
| adpcm_encoder_init | This is function adpcm_encoder_init. |
| adpcm_encode_frame | This is function adpcm_encode_frame. |
| adpcm_encoder_config_byteorder | This is function adpcm_encoder_config_byteorder. |
| adpcm_encoder_free | This is function adpcm_encoder_free. |

**Description**

##### Functions

###### adpcm_encoder_init Function

This is function adpcm_encoder_init.

**bool** **adpcm_encoder_init**(void);

###### adpcm_encode_frame Function

This is function adpcm_encode_frame.

**bool** **adpcm_encode_frame**(**void** * **pin**, uint32_t **insize**, **void** * **pout**, uint32_t * **outsize**);

###### adpcm_encode_frame Function
 adpcm_encoder_config_byteorder Function

This is function adpcm_encoder_config_byteorder.

**void** **adpcm_encoder_config_byteorder**(**bool** **isLE**);

###### adpcm_encode_frame Function
 adpcm_encoder_free Function

This is function adpcm_encoder_free.

**bool** **adpcm_encoder_free**(void);

**Data Types and Constants**

#### Files

**Files**

| **Name** | **Description** |
| --- | --- |
| adpcm_enc.h | Contains the ADPCM encoder specific defintions and function prototypes. |

**Description**

##### adpcm_enc.h

adpcm_enc.h

**Summary**

Contains the ADPCM encoder specific defintions and function prototypes.

**Description**

ADPCM Decoder Header File

This file contains the ADPCM encoder specific defintions and function prototypes.

### PCM Encoder Library

##### _Introduction_

The PCM encoder is a royalty-free audio codec. It is a lossless standard and there is no compression.

It can typically be used to share speech; share music; create files to playback by older/slower processors. This implementation currently supports 16 bit stereo. The sample rate can vary widely.

**_Using the Library_**

#### _Library Interface_

**Functions**

| **Name** | **Description** |
| --- | --- |
| pcm_encoder_init | This is function pcm_encoder_init. |
| pcm_encoder_free | This is function pcm_encoder_free. |

**Description**

##### Functions

###### pcm_encoder_init Function

This is function pcm_encoder_init.

**bool** **pcm_encoder_init**(**int** **channel**, **int** **inputSampleRate**);

###### pcm_encode_frame Function

This is function pcm_encode_frame.

**bool** **pcm_encode_frame**(**void** * **pin**, uint32_t **insize**, **void** * **pout**, uint32_t * **outsize**);

###### pcm_encoder_free Function

This is function pcm_encoder_free.

**bool** **pcm_encoder_free**(void);

**Data Types and Constants**

#### Files

**Files**

| **Name** | **Description** |
| --- | --- |
| pcm_enc.h | WAV Encoder Header File |