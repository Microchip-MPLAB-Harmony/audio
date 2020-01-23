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

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************
#define USE_SDMMC
#define DATA32_ENABLED
#define NUM_SAMPLES                     256*4*4
#define BUFFER_SIZE                     4*NUM_SAMPLES  
#define SWITCH_Get()                    SWITCH1_Get()
#define SWITCH_STATE_PRESSED            SWITCH1_STATE_PRESSED
#define SWITCH_STATE_RELEASED           SWITCH1_STATE_RELEASED

#define BUTTON_DEBOUNCE 400
#define LONG_BUTTON_PRESS 8000


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
