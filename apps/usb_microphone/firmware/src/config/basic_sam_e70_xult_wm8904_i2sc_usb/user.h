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

#include <stdio.h>

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
    
//KEEP THIS - APP Configuration
#define APP_QUEUING_DEPTH   2  //Ping/Pong
#define APP_MAKE_BUFFER_DMA_READY  __attribute__((aligned(16)))
#define APP_USB_STREAM_SAMPLE_RATE 16000

//If TEST<1-5> Implemented E70
#define DEBUG_TEST_POINTS

//    The WM8904 has 3 defined audio inputs, which for the standard WM8904 
//    Daughterboard are:
//       MIC1 - External Microphone
//       MIC2 - Line Input
//       MIC3 - Unused
#define MIC_SELECT MIC1
    
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
