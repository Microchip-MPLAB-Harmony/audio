/* 
 * File:   app_globals.h
 * Author: C16825
 *
 * Created on April 16, 2020, 3:45 PM
 */

#ifndef APP_GLOBALS_H
#define	APP_GLOBALS_H

#include <stdbool.h>
#include <stdint.h>
#include "system/system.h" 

#ifdef	__cplusplus
extern "C" {
#endif

#define NUMSTATES 15

typedef enum _ERROR_NUM 
    {ERR_NONE, ERR_GENERIC, ERR_TIMER, ERR_CODEC, ERR_CODEC_EVENT,
     ERR_CODEC_BUFFER,ERR_USB, ERR_USB_EVENT, ERR_USB_BUFFER
    } ERROR_NUM;

//#define NUMSTATES 12
extern char *     stateStr[NUMSTATES];
extern char *     usbDevEventStr[];
extern char *     usbAudioEventStr[];
extern char *     ERROR_STR[]; 
extern bool       errState;
extern ERROR_NUM  errNum;
extern ERROR_NUM  errorNum;
extern int16_t    tailIdx;
extern int16_t    headIdx;
extern SYS_STATUS codecStatus;

#ifdef	__cplusplus
}
#endif

#endif	/* APP_GLOBALS_H */

