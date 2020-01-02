
/*** Audio Decoders Configuration ***/
#define MP3_DECODER_ENABLED
#define WAV_STREAMING_ENABLED
#define ADPCM_STREAMING_ENABLED

/* Customize buffer size depends on device SRAM */
#define DECODER_MAX_INPUT_BUFFER_SIZE (1024*7)
#define DECODER_MAX_OUTPUT_BUFFER_SIZE (1024*7)

