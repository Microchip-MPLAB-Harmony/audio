# Audio Libraries Help

## Decoder Library Help

This section provides descriptions of the software Decoder Libraries that are available in MPLAB Harmony.

### ADPCM Decoder Library

##### _Introduction_

Adaptive Differential Pulse Code Modulation (ADPCM) is a royalty-free audio codec. It is a lossy compression standard. It is a variant of Differential Pulse-Code Modulation (DPCM). The file format used to contain an ADPCM files, is the same as WAV. Decoding is fast and requires little processing power.

Typical applications using ADPCM are for speech. This implementation currently supports 16 bit stereo.

#### _Library Interface_

**Functions**

| **Name** | **Description** |
| --- | --- |
| ADPCM_Initialize | This is function ADPCM_Initialize. |
| ADPCM_Decoder | Decode the ADPCM stream of inSize from input pointer into output pointer. |
| ADPCM_GetChannels | This is function ADPCM_GetChannels. |
| ADPCM_Decoder_ConfigByteOrder | Configure input ADPCM stream byte order. |
| ADPCM_HdrGetFormat | This is function ADPCM_HdrGetFormat. |
| ADPCM_HdrGetBitsPerSample | This is function ADPCM_HdrGetBitsPerSample. |
| ADPCM_HdrGetDataLen | This is function ADPCM_HdrGetDataLen. |
| ADPCM_HdrGetFileSize | This is function ADPCM_HdrGetFileSize. |
| ADPCM_HdrGetBlockAlign | This is function ADPCM_HdrGetBlockAlign. |
| ADPCM_HdrGetBytesPerSec | This is function ADPCM_HdrGetBytesPerSec. |
| ADPCM_HdrGetNumOfChan | This is function ADPCM_HdrGetNumOfChan. |
| ADPCM_HdrGetSamplesPerSec | This is function ADPCM_HdrGetSamplesPerSec. |

**Data Types and Constants**

| **Name** | **Description** |
| --- | --- |
| ADPCMHEADER | ADPCM WAV File Header |
| ADPCM_HEADER_SIZE | WAV Header Size |

**Description**

This section describes the Application Programming Interface (API) functions of the ADPCM Decoder Library.

##### Functions

###### ADPCM_Initialize Function

This is function ADPCM_Initialize.

**void** **ADPCM_Initialize**(uint8_t * **input**);

###### ADPCM_Decoder Function

bool ADPCM_Decoder

(uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written);

**Summary**

Decode the ADPCM stream of inSize from input pointer into output pointer.

**Description**

Decode the ADPCM stream of inSize from input pointer into output pointer.

**Parameters**

| **Parameters** | **Description** |
| --- | --- |
| inSize | Size of ADPCM input stream in bytes. |
| read | Size of ADPCM decoder has decoded after this function complete |

**Returns**

Pointer to the decoded data. written - Size of decoded data.

If successful, returns true, otherwise, returns false.

**bool** **ADPCM_Decoder**(uint8_t * **input**, uint16_t **inSize**, uint16_t * **read**, int16_t * **output**, uint16_t * **written**);

###### ADPCM_GetChannels Function

This is function ADPCM_GetChannels.

uint8_t **ADPCM_GetChannels**(void);

###### ADPCM_Decoder_ConfigByteOrder Function

void ADPCM_Decoder_ConfigByteOrder (bool isLE);

**Summary**

Configure input ADPCM stream byte order.

**Description**

Configure input ADPCM stream byte order.

**Parameters**

| **Parameters** | **Description** |
| --- | --- |
| isLE | True if ADPCM stream is in little endian format, otherwise, false. |

**Returns**

None.

**void** **ADPCM_Decoder_ConfigByteOrder**(**bool** **isLE**);

###### ADPCM_HdrGetFormat Function

This is function ADPCM_HdrGetFormat.

**int** **ADPCM_HdrGetFormat**(void);

###### ADPCM_HdrGetBitsPerSample Function

This is function ADPCM_HdrGetBitsPerSample.

**int** **ADPCM_HdrGetBitsPerSample**(void);

###### ADPCM_HdrGetDataLen Function

This is function ADPCM_HdrGetDataLen.

**int** **ADPCM_HdrGetDataLen**(void);

###### ADPCM_HdrGetFileSize Function

This is function ADPCM_HdrGetFileSize.

**unsigned** **int** **ADPCM_HdrGetFileSize**(void);

###### ADPCM_HdrGetBlockAlign Function

This is function ADPCM_HdrGetBlockAlign.

**int** **ADPCM_HdrGetBlockAlign**(void);

###### ADPCM_HdrGetBytesPerSec Function

This is function ADPCM_HdrGetBytesPerSec.

**int** **ADPCM_HdrGetBytesPerSec**(void);

###### ADPCM_HdrGetNumOfChan Function

This is function ADPCM_HdrGetNumOfChan.

**int** **ADPCM_HdrGetNumOfChan**(void);

###### ADPCM_HdrGetSamplesPerSec Function

This is function ADPCM_HdrGetSamplesPerSec.

**int** **ADPCM_HdrGetSamplesPerSec**(void);

##### Data Types and Constants

###### ADPCMHEADER Type

ADPCM WAV File Header

**Description**

ADPCM Audio File Header Structure

This structure is header structure of wav container.

**typedef** **struct** ADPCMHEADER@1 **ADPCMHEADER**;

###### ADPCM_HEADER_SIZE Macro

ADPCM WAV Header Size

**Description**

ADPCM WAV File Header Size 
**#define** **ADPCM_HEADER_SIZE** 44

### MP3 Decoder Library

##### _Introduction_

MP3 is a lossy compression standard. MP3 compression works by reducing (or approximating) the accuracy of certain components of sound that are considered (by psychoacoustic analysis) to be beyond the hearing capabilities of most humans. This method is commonly referred to as perceptual coding or as psychoacoustic modeling.[8] The remaining audio information is then recorded in a space-efficient manner, using MDCT and FFT algorithms. Compared to CD-quality digital audio, MP3 compression can commonly achieve a 75 to 95% reduction in size.

In Harmony 3, the MP3 decoder is limited to 16-bit audio, typically recorded at either CD quality (44100 samples/second) or DVD quality (48000 samples/second).

In addition to audio, an MP3 file may contain metadata, called ID3 tags, which include the name of the song, name of the artist, name of the album, and other information.

An MP3 file is made up of MP3 frames, which consist of a header and a data block. This sequence of frames is called an elementary stream. Due to the "bit reservoir", frames are not independent items and cannot usually be extracted on arbitrary frame boundaries. The MP3 Data blocks contain the (compressed) audio information in terms of frequencies and amplitudes. The MP3 Header consists of a sync word, which is used to identify the beginning of a valid frame. This is followed by a bit indicating that this is the MPEG standard and two bits that indicate that layer 3 is used; hence MPEG-1 Audio Layer 3 or MP3.

MP3 was originally patented, but the last patent expired on April 16, 2017.

In Harmony 3, an open-source MP3 decoder called Helix MP3 is used. The main portion of the code is located in a separate repository called helix_mp3\. The MP3 Decoder Library in the audio repository is just a wrapper that calls functions in the main library (which is brought in automatically when you generate code in MHC, with the MP3 Decoder Library selected).

##### _Building the Library_

This section lists the files that are available in the MP3 Decoder Library.

**Description**

This section lists the files that are available in the src folder of the MP3 Decoder Library. It lists which files need to be included in the build based on a configuration option selected by the system.

The following three tables list and describe the header (.h) and source (.c) files that implement this library. The parent folder for these files is src/config/<config_dir>/audio/decoder/

###### Interface File(s)

This table lists and describes the header files that must be included (i.e., using #include) by any code that uses this library.

| **Source File Name** | **Description** |
| --- | --- |
| audio_decoders.h | Header file that includes the lower-level include files (e.g. mp3_dec.h) |
| audio_decoders_config.h | Header file that contains #defines for each enabled decoder MP3_DECODER_ENABLED) |

###### Required File(s)

![](graphics/MHC_logo.jpg) **_All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use._**

This table lists and describes the header and source files that must _always_ be included in the MPLAB X IDE project to build this library.

| **Source File Name** | **Description** |
| --- | --- |
| audio_decoders/mp3/mp3_dec.c,.h | This file contains implementation of the API for the MP3 Decoder |
| audio_decoders/mp3/id3/id3.c,.h | This file contains implementation of the API for parsing ID3 tags |
| audio_decoders/mp3/id3/utils.c,.h | These files contain utility functions for parsing ID3 tags |
| audio_decoders/mp3/fixpnt/*.* | Files from the Helix MP3 Decoder repository (see below) |

The MP3 Decoder Library files in the apps repository only contain a wrapper around the Helix open-source MP3 Decoder, located in the helix_mp3 repository. Files from the fixpnt folder in that repo are copied into the fixpnt folder listed above when code is generated using MHC.

![](graphics/important.jpg) **_If using the MP3 Decoder Library from the audio repository, the helix_mp3 repository must also be downloaded._**

#### Library Interface
This section describes the Application Programming Interface (API) functions of the MP3 Decoder Library.

**Functions**

| **Name** | **Description** |
| --- | --- |
| isMP3decoder_enabled | This is function isMP3decoder_enabled. |
| MP3_Decoder | This is function MP3_Decoder. |
| MP3_FillReadBuffer | This is function MP3_FillReadBuffer. |
| MP3_FindSyncWord | This is function MP3_FindSyncWord. |
| MP3_GetAudioSize | This is function MP3_GetAudioSize. |
| MP3_GetBitRate | This is function MP3_GetBitRate. |
| MP3_GetBitsPerSample | This is function MP3_GetBitsPerSample. |
| MP3_GetChannels | This is function MP3_GetChannels. |
| MP3_GetDuration | This is function MP3_GetDuration. |
| MP3_GetSampleRate | This is function MP3_GetSampleRate. |
| MP3_HdrGetBytesPerSec | This is function MP3_HdrGetBytesPerSec. |
| MP3_HdrGetDataLen | This is function MP3_HdrGetDataLen. |
| MP3_HdrGetFileSize | This is function MP3_HdrGetFileSize. |
| MP3_HdrGetSamplesPerSec | This is function MP3_HdrGetSamplesPerSec. |
| MP3_Initialize | This is function MP3_Initialize. |
| MP3_Initialize_N | This is function MP3_Initialize_N. |
| MP3_UpdatePlaytime | This is function MP3_UpdatePlaytime. |
| read_ID3Tags | This is function read_ID3Tags. |

**Description**

This section describes the Application Programming Interface (API) functions of the MP3 Decoder Library.

##### Functions

###### _isMP3decoder_enabled Function_

This is function isMP3decoder_enabled.

**bool** **isMP3decoder_enabled**(void);

###### _MP3_Decoder Function_

This is function MP3_Decoder.

**bool** **MP3_Decoder**(uint8_t * **input**, uint16_t **inSize**, uint16_t * **read**, int16_t * **output**, uint16_t

* **written**);

###### _MP3_FillReadBuffer Function_

This is function MP3_FillReadBuffer.

**int** **MP3_FillReadBuffer**(**unsigned** **char** * **readBuf**, **unsigned** **char** * **readPtr**, **int** **bufSize**, **int** **bytesLeft**, SYS_FS_HANDLE **fileHandle**);

###### _MP3_FindSyncWord Function_

This is function MP3_FindSyncWord.

int32_t **MP3_FindSyncWord**(HMP3Decoder **hMP3Decoder**, **unsigned** **char** * **readPtr**, int32_t **bytesLeft**);

###### _MP3_GetAudioSize Function_

This is function MP3_GetAudioSize.

uint32_t **MP3_GetAudioSize**(void);

###### _MP3_GetBitRate Function_

This is function MP3_GetBitRate.

uint32_t **MP3_GetBitRate**(SYS_FS_HANDLE **fileHandle**, uint32_t **firstframe**, uint8_t * **heap**);

###### _MP3_GetBitsPerSample Function_

This is function MP3_GetBitsPerSample.

**int** **MP3_GetBitsPerSample**(void);

###### _MP3_GetChannels Function_

This is function MP3_GetChannels.

uint8_t **MP3_GetChannels**(void);

###### _MP3_GetDuration Function_

This is function MP3_GetDuration.

uint32_t **MP3_GetDuration**(void);

###### _MP3_GetSampleRate Function_

This is function MP3_GetSampleRate.

uint32_t **MP3_GetSampleRate**(void);

###### _MP3_HdrGetBytesPerSec Function_

This is function MP3_HdrGetBytesPerSec.

**int** **MP3_HdrGetBytesPerSec**(void);

###### _MP3_HdrGetDataLen Function_

This is function MP3_HdrGetDataLen.

**int** **MP3_HdrGetDataLen**(void);

###### _MP3_HdrGetFileSize Function_

This is function MP3_HdrGetFileSize.

**unsigned** **int** **MP3_HdrGetFileSize**(void);

###### _MP3_HdrGetSamplesPerSec Function_

This is function MP3_HdrGetSamplesPerSec.

**int** **MP3_HdrGetSamplesPerSec**(void);

###### _MP3_Initialize Function_

This is function MP3_Initialize.

**void** **MP3_Initialize**(uint8_t * **input**);

###### _MP3_Initialize_N Function_

This is function MP3_Initialize_N.

**void** **MP3_Initialize_N**(uint8_t * **input**, SYS_FS_HANDLE **wavFilehandle**);

###### _MP3_UpdatePlaytime Function_

This is function MP3_UpdatePlaytime.

uint32_t **MP3_UpdatePlaytime**(void);

###### _read_ID3Tags Function_

This is function read_ID3Tags.

**void** **read_ID3Tags**(SYS_FS_HANDLE **fileHandle**, uint8_t * **heap**, ID3_EVENT_HANDLER **eventHandler**);

**Data Types and Constants**

### WAV Decoder Library

##### _Introduction_

#### _Library Interface_

**Functions**

| **Name** | **Description** |
| --- | --- |
| WAV_Initialize_N | This is function WAV_Initialize_N. |
| WAV_Decoder | This is function WAV_Decoder. |
| WAV_GetChannels | This is function WAV_GetChannels. |
| WAV_UpdatePlaytime | This is function WAV_UpdatePlaytime. |
| WAV_GetDuration | This is function WAV_GetDuration. |
| WAV_HdrGetDataLen | This is function WAV_HdrGetDataLen. |
| WAV_HdrGetFileSize | This is function WAV_HdrGetFileSize. |
| WAV_GetSampleRate | This is function WAV_GetSampleRate. |
| WAV_GetBitRate | This is function WAV_GetBitRate. |
| WAV_HdrGetBytesPerSec | This is function WAV_HdrGetBytesPerSec. |
| WAV_HdrGetSamplesPerSec | This is function WAV_HdrGetSamplesPerSec. |
| WAV_GetAudioSize | This is function WAV_GetAudioSize. |
| isWAVdecoder_enabled | This is function isWAVdecoder_enabled. |
| WAV_Initialize | This is function WAV_Initialize. |

**Data Types and Constants**

| **Name** | **Description** |
| --- | --- |
| WAV_HEADER_SIZE | This is macro WAV_HEADER_SIZE. |
| WAV_HEAP_SIZE | This is macro WAV_HEAP_SIZE. |
| WAVE_FORMAT_ALAW | This is macro WAVE_FORMAT_ALAW. |
| WAVE_FORMAT_MULAW | This is macro WAVE_FORMAT_MULAW. |
| WAVE_FORMAT_PCM | This is macro WAVE_FORMAT_PCM. |
| WAV_INPUT_BUFFER_SIZE | This is macro WAV_INPUT_BUFFER_SIZE. |
| WAV_OUTPUT_BUFFER_SIZE | This is macro WAV_OUTPUT_BUFFER_SIZE. |
| dWAVHEADER | This is type dWAVHEADER. |
| WAV_DEC | This is type WAV_DEC. |

**Description**

This section describes the Application Programming Interface (API) functions of the WAV Decoder Library.

##### Functions

###### WAV_Initialize_N Function

This is function WAV_Initialize_N.

**void** **WAV_Initialize_N**(uint8_t * **input**, SYS_FS_HANDLE **wavFilehandle**);

###### WAV_Decoder Function

This is function WAV_Decoder.

**bool** **WAV_Decoder**(uint8_t * **input**, uint16_t **inSize**, uint16_t * **read**, int16_t * **output**, uint16_t * **written**);

###### WAV_GetChannels Function

This is function WAV_GetChannels.

uint8_t **WAV_GetChannels**(void);

###### WAV_UpdatePlaytime Function

This is function WAV_UpdatePlaytime.

uint32_t **WAV_UpdatePlaytime**(void);

###### WAV_GetDuration Function

This is function WAV_GetDuration.

uint32_t **WAV_GetDuration**(void);

###### WAV_HdrGetDataLen Function

This is function WAV_HdrGetDataLen.

**int** **WAV_HdrGetDataLen**(void);

###### WAV_HdrGetFileSize Function

This is function WAV_HdrGetFileSize.

**unsigned** **int** **WAV_HdrGetFileSize**(void);

###### WAV_GetSampleRate Function

This is function WAV_GetSampleRate.

uint32_t **WAV_GetSampleRate**(void);

###### WAV_GetBitRate Function

This is function WAV_GetBitRate.

uint32_t **WAV_GetBitRate**(void);

###### WAV_HdrGetBytesPerSec Function

This is function WAV_HdrGetBytesPerSec.

**int** **WAV_HdrGetBytesPerSec**(void);

###### WAV_HdrGetSamplesPerSec Function

This is function WAV_HdrGetSamplesPerSec.

**int** **WAV_HdrGetSamplesPerSec**(void);

###### WAV_GetAudioSize Function

This is function WAV_GetAudioSize.

uint32_t **WAV_GetAudioSize**(void);

###### isWAVdecoder_enabled Function

This is function isWAVdecoder_enabled.

**bool** **isWAVdecoder_enabled**(void);

###### WAV_Initialize Function

This is function WAV_Initialize.

**void** **WAV_Initialize**(uint8_t * **input**);

##### Data Types and Constants

###### WAV_HEADER_SIZE Macro

This is macro WAV_HEADER_SIZE.

**#define** **WAV_HEADER_SIZE** 44

###### WAV_HEAP_SIZE Macro

This is macro WAV_HEAP_SIZE.

**#define** **WAV_HEAP_SIZE** 1024

###### WAVE_FORMAT_ALAW Macro

This is macro WAVE_FORMAT_ALAW.

**#define** **WAVE_FORMAT_ALAW** 0x0006

###### WAVE_FORMAT_MULAW Macro

This is macro WAVE_FORMAT_MULAW.

**#define** **WAVE_FORMAT_MULAW** 0x0007

###### WAVE_FORMAT_PCM Macro

This is macro WAVE_FORMAT_PCM.

ADPCM Ecncoder Library

**#define** **WAVE_FORMAT_PCM** 0x0001

###### _WAV_INPUT_BUFFER_SIZE Macro_

This is macro WAV_INPUT_BUFFER_SIZE.

**#define** **WAV_INPUT_BUFFER_SIZE**

(DECODER_MAX_OUTPUT_BUFFER_SIZE>DECODER_MAX_INPUT_BUFFER_SIZE?DECODER_MAX_INPUT_BUFFER_SIZE:DECO

DER_MAX_OUTPUT_BUFFER_SIZE)

###### _WAV_OUTPUT_BUFFER_SIZE Macro_

This is macro WAV_OUTPUT_BUFFER_SIZE.

**#define** **WAV_OUTPUT_BUFFER_SIZE** (WAV_INPUT_BUFFER_SIZE)

###### dWAVHEADER Type

This is type dWAVHEADER.

**typedef** **struct** dWAVHEADER@1 **dWAVHEADER**;

###### WAV_DEC Type

This is type WAV_DEC.

**typedef** **struct** WAV_DEC@1 **WAV_DEC**;

### FLAC Decoder Library

##### _Introduction_

FLAC stands for Free Lossless Audio Codec, an audio format similar to MP3, but lossless, meaning that audio is compressed in FLAC without any loss in quality. FLAC stands out as the fastest and most widely supported lossless audio codec, and the only one that at once is non-proprietary, is unencumbered by patents, has an open-source reference implementation, has a well documented format and API, and has several other independent implementations.

In Harmony 3, the FLAC decoder is limited to 16-bit, 2 channel audio, typically recorded at either CD quality (44100 samples/second) or DVD quality (48000 samples/second).

In Harmony 3, an open-source FLAC decoder called XIPH FLAC is used. The main portion of the code is located in a separate repository called xiph_flac\. The FLAC Decoder Library in the audio repository is just a wrapper that calls functions in the main library (which is brought in automatically when you generate code in MHC, if the the FLAC Decoder Library is selected). Note that this library uses heap.

##### _Building the Library_

This section lists the files that are available in the FLAC Decoder Library.

**Description**

This section lists the files that are available in the src folder of the FLAC Decoder Library. It lists which files need to be included in the build based on a configuration option selected by the system.

The following three tables list and describe the header (.h) and source (.c) files that implement this library. The parent folder for these files is src/config/<config_dir>/audio/decoder/

###### Interface File(s)

This table lists and describes the header files that must be included (i.e., using #include) by any code that uses this library.

| **Source File Name** | **Description** |
| --- | --- |
| audio_decoders.h | Header file that includes the lower-level include files (e.g. flac_dec.h) |
| audio_decoders_config.h | Header file that contains #defines for each enabled decoder FLAC_DECODER_ENABLED) |

###### Required File(s)

![](graphics/MHC_logo.jpg) **_All of the required files listed in the following table are automatically added into the MPLAB X IDE project by the MHC when the library is selected for use._**

This table lists and describes the header and source files that must _always_ be included in the MPLAB X IDE project to build this library.

| **Source File Name** | **Description** |
| --- | --- |
| audio_decoders/flac/flac_dec.c,.h | These are public APIs for using the FLAC Decoder |
| audio_decoders/flac/flac_master/include/FLAC/format.h | This file configures decoding logic and format |

The FLAC Decoder Library files in the apps repository only contain a wrapper around the Xiph open-source FLAC Decoder, located in the xiph_flac repository. Files from the Harmony framework's xiph repo are copied into the local flac folder of the application when code is generated using MHC.

![](graphics/important.jpg) **_If using the FLAC Decoder Library from the audio repository, the xiph_flac repository must also be downloaded._**

#### Library Interface
This section describes the Application Programming Interface (API) functions of the FLAC Decoder Library, in this version of the release.

**Functions**

| **Name** | **Description** |
| --- | --- |
| FLAC_isDecoder_enabled | Function to check if FLAC decoding is enabled. |
| FLAC_Initialize | Function to initialize FLAC library. |
| FLAC_RegisterDecoderEventHandlerCallback | Registers application defined Decoder Callbacks. |
| FLAC_DecodeSingleFrame | This function decodes one FLAC frame and returns the decoded data. |
| FLAC_GetChannels | This function returns number of channels. |
| FLAC_GetBitRate | This function returns the bit rate. |
| FLAC_GetSamplingRate | This function returns the sample rate. |
| FLAC_GetBlockSize | This Function returns the number of samples in a sub block. |
| FLAC_GetBitdepth | This function returns the resolution of the sound data. |
| FLAC_GetDuration | This function returns the track length of the FLAC audio file. |
| FLAC_Cleanup | This is the function for deallocating memory resources of a FLAC decoder. |
| FLAC_DecoderIsFileValid | Function to check if a given FLAC file is valid or not. |

**Data Types and Constants**

| **Name** | **Description** |
| --- | --- |
| DecoderEventHandlerCB | The Callback function template for the application defined callback Functions. |
| FLAC_DEC_APPLICATIONCB_SET | The set of callbacks an application would be interested for callback. |
| FLAC_DEC_WRITE_EVT_CONTINUE | Write status Success upon decoder writecallback. |
| FLAC_DEC_WRITE_EVT_ERR | Write status Error upon decoder writecallback. |

**Description**

This section describes the Application Programming Interface (API) functions of the FLAC Decoder Library.

##### Functions

###### FLAC_isDecoder_enabled Function

Function to check if FLAC decoding is enabled.

**bool** **FLAC_isDecoder_enabled**(void);

###### FLAC_Initialize Function

Function to initialize FLAC library.

**bool** **FLAC_Initialize**(SYS_FS_HANDLE **fhandle**, void * **input_buffer**)

###### FLAC_RegisterDecoderEventHandlerCallback Function

FLAC_RegisterDecoderEventHandlerCallback

void **FLAC_RegisterDecoderEventHandlerCallback**(FLAC_DEC_APPLICATIONCB_SET * **flac_decoder_appCB_set**, void * **ctxt**);

###### FLAC_DecodeSingleFrame Function

This function decodes one FLAC frame and returns the decoded data.

**bool** **FLAC_DecodeSingleFrame**(uint8_t * **output**);

###### FLAC_GetChannels Function

This function returns number of channels.

uint8_t **FLAC_GetChannels**(void);

###### FLAC_GetBitRate Function

This function returns the bit rate.

int32_t **FLAC_GetBitRate**(void);

###### FLAC_GetSamplingRate Function

This function returns the sample rate.

int32_t **FLAC_GetSamplingRate**(void);

###### FLAC_GetBlockSize Function

This function returns the number of samples in a sub block.

int32_t **FLAC_GetBlockSize**(void);

###### FLAC_GetBitdepth Function

This function returns the resolution of the sound data.

uint8_t **FLAC_GetBitdepth**(void);

###### FLAC_GetDuration Function

This is function returns the track length of the FLAC audio file.

uint32_t **FLAC_GetDuration**(void);

###### FLAC_Cleanup Function 

This is the function for deallocating memory resources of a FLAC decoder.

void **FLAC_Cleanup**(void);
 
###### FLAC_DecoderIsFileValid Function

Function to check if a given FLAC file is valid or not.

**bool** **FLAC_DecoderIsFileValid**(void);

##### Data Types and Constants

###### DecoderEventHandlerCB Type

The Callback function template for the application defined callback Functions.

**typedef** void (* **DecoderEventHandlerCB**)(uint32_t **event**, void * **cb_param**, void * **ctxt**);

###### FLAC_DEC_APPLICATIONCB_SET Type

The set of callbacks an application would be interested for callback. 

**typedef** void (* **DecoderEventHandlerCB**)(uint32_t **event**, void * **cb_param**, void * **ctxt**);

###### FLAC_DEC_APPLICATIONCB_SET Type

Write status Success upon decoder writecallback.

###### FLAC_DEC_WRITE_EVT_ERR Type

Write status Error upon decoder writecallback.
