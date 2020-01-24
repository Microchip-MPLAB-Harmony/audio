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
// Board Definition
#define PIC32MZEFC2
#define CODEC_AK4954
#undef SAME70CULT
#undef CODEC_WM8904

#undef ONE_BUTTON_VERSION

#ifdef SYS_DEBUG_USE_CONSOLE

#ifndef SYS_MESSAGE
    #define SYS_MESSAGE(message)                SYS_DEBUG_Message(message)
#endif

#ifndef SYS_DEBUG_MESSAGE
    #define SYS_DEBUG_MESSAGE(level, message)   _SYS_DEBUG_MESSAGE(level, message)
#endif

#ifndef SYS_PRINT
    #define SYS_PRINT(fmt, ...)                 SYS_DEBUG_Print(fmt, ##__VA_ARGS__)
#endif

#ifndef SYS_DEBUG_PRINT
    #define SYS_DEBUG_PRINT(level, fmt, ...)    _SYS_DEBUG_PRINT(level, fmt, ##__VA_ARGS__)
#endif
#endif //SYS_DEBUG_USE_CONSOLE

#define SYS_PRINT(fmt, ...)    _SYS_DEBUG_PRINT(0, fmt, ##__VA_ARGS__)

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
