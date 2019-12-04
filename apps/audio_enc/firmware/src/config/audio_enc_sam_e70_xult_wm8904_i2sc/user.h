/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END
#include "audio/decoder/audio_decoders_config.h"
#include "include/audio_encoder_config.h"
#include "audio_decoders.h"
    
// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************
#ifdef WAV_CONTAINER
#include "include/wav_format_container.h"
#endif
#ifdef WAV_STREAMING_ENABLED
#include "wav/wav_dec.h"
#endif
#ifdef PCM_ENCODER
#include "pcm/pcm_enc.h"
#endif
    
#define NUM_SAMPLES                     256*4
#define BUFFER_SIZE                     4*NUM_SAMPLES  

#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000
    
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
