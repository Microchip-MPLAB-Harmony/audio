/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef APP_H
#define APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"
#include "peripheral/clk/plib_clk.h"
#include "peripheral/nvic/plib_nvic.h"
#include "peripheral/pio/plib_pio.h"
#include "bsp/bsp.h"
#include "system/int/sys_int.h"
#include "system/ports/sys_ports.h"
#include "osal/osal.h"
#include "driver/i2s/drv_i2s.h"
#include "driver/i2c/drv_i2c.h"
#include "peripheral/twihs/plib_twihs0.h"
#include "system/time/sys_time.h"

#include "audio/driver/wm8904/drv_wm8904_local.h"
#include "audio/driver/wm8904/drv_wm8904.h"

#include "disk.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

#define WAV_STREAMING_ENABLED
#undef  USE_CLEAR_BUFFER
    
#define NUM_SAMPLES             256*4
#define BUFFER_SIZE             4*NUM_SAMPLES  

#if 0    
#define LED1_On()               LED_AL_PA5_On()
#define LED1_Off()              LED_AL_PA5_Off()
#define LED1_Toggle()           LED_AL_PA5_Toggle()
#endif
    

       
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct 
{
    /* Left channel data */
    int16_t leftData;

    /* Right channel data */
    int16_t rightData;

} DRV_I2S_DATA16;

extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer1[NUM_SAMPLES];
extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Buffer2[NUM_SAMPLES];
extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Single_Chnl_Buffer[NUM_SAMPLES/2];
#ifdef USE_CLEAR_BUFFER
extern DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Audio_Output_Clear_Buffer[NUM_SAMPLES];
#endif

// *****************************************************************************
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
            
    /* TODO: Define states used by the application state machine. */
    APP_STATE_CODEC_OPEN,
    APP_STATE_CODEC_CLEAR,
    APP_STATE_CODEC_SET_BUFFER_HANDLER,
    APP_STATE_CODEC_ADD_BUFFER,
    APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE, 

	// USB stuff
	APP_STATE_BUS_ENABLE,
    APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
    APP_STATE_WAIT_FOR_DEVICE_ATTACH,
    APP_STATE_DEVICE_CONNECTED,
    APP_STATE_MOUNT_DISK,
    APP_STATE_UNMOUNT_DISK,
    APP_STATE_OPEN_FILE,
    APP_STATE_READ_FILE,
    APP_STATE_CLOSE_FILE,
    APP_STATE_IDLE,
    APP_STATE_HALT,
    APP_STATE_NO_MEDIA,
    APP_STATE_NO_FILE,
    APP_STATE_WAITING,
    APP_STATE_RUNNING,
    APP_STATE_READY_TO_SCAN,
    APP_STATE_SCANNING,
    APP_STATE_SEND_AUDIO_DATA,
    APP_STATE_DECODE_DATA,
    APP_STATE_TRACK_CHANGE,
    APP_STATE_WAIT_BUFFERS_COMPLETED,
    APP_STATE_ERROR
} APP_STATES;

typedef enum
{
    BUTTON_STATE_IDLE=0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_BUTTON0_PRESSED,                   
    BUTTON_STATE_WAIT_FOR_RELEASE    
} BUTTON_STATES;

// *****************************************************************************
// CODEC Data
//
//  Summary:
//    Holds codec data
//
//  Description:
//    This structure holds the codec's data.
//
//  Remarks:
// *****************************************************************************
typedef struct
{
    DRV_HANDLE handle;
    DRV_CODEC_BUFFER_HANDLE writeBufHandle1, writeBufHandle2;
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    uintptr_t context;
    uint8_t *txbufferObject1;
    uint8_t *txbufferObject2;
    size_t bufferSize1, bufferSize2;
} AUDIO_CODEC_DATA;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */


typedef enum
{
    PLAYER_CMD_NONE,
    PLAYER_CMD_STOP,
    PLAYER_CMD_PLAY,
    PLAYER_CMD_PAUSE,
    PLAYER_CMD_PLAY_PAUSE,
    PLAYER_CMD_NEXT_FILE,
    PLAYER_CMD_PREV_FILE,
    PLAYER_CMD_FAST_FORWARD,
    PLAYER_CMD_REWIND,
}PLAYER_COMMAND;

typedef enum
{
    PLAYER_EVENT_DISK_REMOVED,
    PLAYER_EVENT_DISK_INSERTED,
    PLAYER_EVENT_SCANNING_STARTED,
    PLAYER_EVENT_SCANNING_FOUND,
    PLAYER_EVENT_SCANNING_FINISHED,
    PLAYER_EVENT_NO_FILES_FOUND,
    PLAYER_EVENT_READY,
    PLAYER_EVENT_TAG_ARTIST,
    PLAYER_EVENT_TAG_ALBUM,
    PLAYER_EVENT_TAG_TITLE,
    PLAYER_EVENT_TRACK_CHANGED,
    PLAYER_EVENT_TIME_CHANGED,
    PLAYER_EVENT_SAMPLERATE,
    PLAYER_EVENT_BITRATE,
    PLAYER_EVENT_TRACK_TOTAL_TIME,
    PLAYER_EVENT_TRACK_PLAY_TIME,
    PLAYER_EVENT_VOLUME_CHANGE,
    PLAYER_EVENT_ERROR_MESSAGE,
    PLAYER_EVENT_GOTO_EXPLORER_WINDOW,
    PLAYER_EVENT_GOTO_PLAYER_WINDOW,
}
PLAYER_EVENT;

typedef enum{
    APP_STREAM_MP3,
    APP_STREAM_WMA,
    APP_STREAM_WAV,
    APP_STREAM_AAC,
    APP_STREAM_SPEEX,
    APP_STREAM_ADPCM,
    APP_STREAM_OPUS,
    APP_STREAM_FLAC,
}PLAYER_STREAM_TYPE;

typedef enum{
    APP_FILE_TYPE_WAV,
    APP_FILE_TYPE_MP3,
    APP_FILE_TYPE_WMA,
    APP_FILE_TYPE_AAC,
    APP_FILE_TYPE_MP4,
    APP_FILE_TYPE_UNKNOWN,
}APP_FILE_TYPE;

typedef enum{
    APP_DECODER_MP3,
    APP_DECODER_AAC,
    APP_DECODER_ADPCM,
    APP_DECODER_WMA,
    APP_DECODER_WAV,
    APP_DECODER_SPEEX,
    APP_DECODER_OPUS,
    APP_DECODER_FLAC,
    APP_DECODER_UNKNOWN,
}APP_DECODER_TYPE;

typedef enum{
    VOL_LVL_MUTE,
    VOL_LVL_LOW,
    VOL_LVL_MED,
    VOL_LVL_HIGH,
    VOL_LVLS_MAX,
}VOL_LEVEL;

typedef enum{
    VOL_ADJUST,
    LINEAR_TRACK_CHANGE,
//    RANDOM_TRACK_CHANGE,
    MAX_MODES,
}PLAYER_MODE;

typedef struct{
    char chunkID[4];        // "RIFF"
    uint32_t chunkSize;
    char format[4];         // "WAVE"
    char subChunkId1[4];    // "fmt "
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subChunk2Id[4];    // "data"
    uint32_t subChunk2Size;
}WAV_FILE_HEADER;
typedef struct{
    bool bStereoMode;
}APP_RUNDCPT; // application layer descriptor 

#define AUDIO_QUEUEBUFFER_NUMBER 2

typedef struct{
//    int8_t buffer[DECODER_MAX_OUTPUT_BUFFER_SIZE];
    bool inUse;
    bool decoded;
    uint32_t offset;
    DRV_CODEC_BUFFER_HANDLE writeHandler;
    size_t bufferSize;
}AUDIO_QUEUEBUFFER;



typedef struct {
    /* SYS_FS File handle for 1st file */
    SYS_FS_HANDLE fileHandle;
    SYS_FS_FSTAT fileStatus;

    /* handle to opened timer */
    DRV_HANDLE tmrHandle;
    
    /* Application's current state */
    APP_STATES state;

    PLAYER_MODE playerBtnMode;
    VOL_LEVEL volLevel;
    /*Unfolding audio files*/
    uint16_t currentSongIdx;
    uint16_t nextSongIdx;
    uint16_t previousSongIdx;
    uint16_t totalAudioFiles;

    /* Number of bytes read */
    uint32_t nBytesRead;
    uint32_t diskCurrentFile;
    int32_t  fileSize;
    int32_t readBytes;
    uint32_t current_filesize;
    
    int8_t readIdx;
    uint8_t writeIdx;
    uint16_t playerDecodeDataSize;
    
    bool nextTrack;
    bool readbyte_flag;

    uint32_t playbackDelay;
    uint32_t buttonDelay;
    uint32_t led1Delay;
    uint32_t led2Delay;
    
    BUTTON_STATES buttonState;

    uint8_t volume;
       
    uint8_t pingPong;
       
    AUDIO_CODEC_DATA codecData;
    
    bool buttonMode;
    
    uint8_t volumeIndex;   
    
    bool deviceIsConnected;
    bool buf1Clear;
    bool buf2Clear;
    bool lrSync;

    //    UpdatePlaytimeFuncPtr updatePlaytimeFunc;
    
    PLAYER_COMMAND pressCMD;
    PLAYER_STREAM_TYPE currentStreamType;
    /* repeat timer handle */
    DRV_HANDLE repeatTmrHandle;
    AUDIO_QUEUEBUFFER  audioBuffer[AUDIO_QUEUEBUFFER_NUMBER];
    bool trackPlayed[DISK_MAX_FILES];
    char fileName[64];
}APP_DATA ;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
//APP: API to CODEC Event Handler
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
    DRV_CODEC_BUFFER_HANDLE handle, uintptr_t context);
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void                APP_Tasks( void );

void                APP_Button_Tasks( void );
void                APP_LED_Tasks( void );

void                DISK_Tasks( void );

bool                APP_GetSpiAudioMode( void );
APP_DATA *          APP_GetAppDataInstance( void );
int32_t             APP_GetReadBytesInAppData( void );
void                APP_SetSpiAudioMode( bool mode );
void                APP_SetReadBytesInAppData( int32_t val );
void                APP_SetReadFlagInAppData( bool b );
void                APP_SetReadBytesReadFlag( int32_t val, bool b );
bool                APP_IsSupportedAudioFile( char *name );
bool                APP_PlayerEventHandler ( PLAYER_EVENT event, uint32_t data );
void                APP_Initialize( void );
bool                APP_PlayerDecode( uint8_t *ptr, int16_t* out );
APP_DECODER_TYPE    APP_GetCurrentFileType ( char *ext );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

