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
#include "audio/decoder/audio_decoders.h"

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************
#define NUM_SAMPLES                     1152
#define BUFFER_SIZE                     4*NUM_SAMPLES  
    
#define BUTTON_DEBOUNCE 50
#define LONG_BUTTON_PRESS 1000

#define SWITCH_Get()            SWITCH1_Get()
#define SWITCH_STATE_PRESSED    SWITCH1_STATE_PRESSED
    
#define GFX_ENABLED 1
    
#define SWAPCHANNELS 1    
    
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
