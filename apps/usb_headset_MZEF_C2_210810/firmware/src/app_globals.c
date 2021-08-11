#include "app_globals.h"

bool       errState     = false;
ERROR_NUM  errNum       = ERR_NONE;
SYS_STATUS codecStatus;
int16_t    tailIdx;
int16_t    headIdx;

ERROR_NUM errorNum = 0;
char * ERROR_STR[] = 
    { 
      "No Error",          //0
      "Generic Error",     //1
      "Timer Not Ready",   //2 
      "Codec Not Ready",   //3 
      "Codec Event Error", //4 
      "Codec Buffer Error",//5 
      "USB Not Ready",     //6
      "USB Event Error",   //7
      "USB Buffer Error",  //8
    };

/* Application's state machine's initial state. */
/*
typedef enum
{
    APP_STATE_INIT = 0,
    APP_STATE_USB_DEVICE_OPEN                        =1,
    APP_STATE_CODEC_OPEN                             =2,
    APP_STATE_CODEC_SET_BUFFER_HANDLER               =3,
    //APP_STATE_I2SC0_OPEN                             =2,
    //APP_STATE_I2SC0_SET_BUFFER_HANDLER               =3,
    APP_STATE_INITIAL_USB_READ_REQUESTS              =4,
    APP_STATE_INITIAL_CODEC_WRITEREAD_REQUESTS       =5,
    APP_STATE_MUTE_AUDIO_PLAYBACK                    =6,
    APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD   =7,
    APP_STATE_SAMPLING_FREQUENCY_CHANGE_HP           =8,
    APP_STATE_SAMPLING_FREQUENCY_CHANGE_MP           =9,
    //APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD_HP, //headphone
    //APP_STATE_USB_INTERFACE_ALTERNATE_SETTING_RCVD_MP, //microphone
    APP_STATE_WAIT_FOR_CONFIGURATION                 =10,
    APP_STATE_PROCESS_DATA                           =11,
    APP_STATE_IDLE                                   =12,
    APP_STATE_USB_QUEUE_CLEAR                        =13,
    APP_STATE_ERROR                                  =14,
} APP_STATES;
*/


char * stateStr[NUMSTATES] =
{
    "_INIT",                                 //0
    "_USB_DEVICE_OPEN",                      //1
    "_CODEC_OPEN",                           //2
    "_CODEC_SET_BUFFER_HANDLER",             //3
    "_INITIAL_USB_READ_REQUESTS",            //4
    "_INITIAL_CODEC_WRITEREAD_REQUESTS",     //5
    "_MUTE_AUDIO_PLAYBACK",                  //6
    "_USB_INTERFACE_ALTERNATE_SETTING_RCVD", //7
    "_SAMPLING_FREQUENCY_CHANGE_HP",         //8
    "_SAMPLING_FREQUENCY_CHANGE_MP",         //9
    "_WAIT_FOR_CONFIGURATION",               //10
    "_PROCESS_DATA",                         //11
    "_IDLE",                                 //12
    "_USB_QUEUE_CLEAR",                      //13
    "_ERROR",                                //14
};

char * usbDevEventStr[] =
{
    "unknown 0",
    "USB_DEVICE:_ERROR",          //1
    "USB_DEVICE:_RESET",          //2 
    "USB_DEVICE:_RESUMED",        //3
    "USB_DEVICE:_SUSPENDED",      //4 
    "unknown 5",
    "unknown 6",
    "USB_DEVICE:_SOF",            //7 
    "USB_DEVICE:_POWER_DETECTED", //8 
    "USB_DEVICE:_POWER_REMOVED",  //9
    "USB_DEVICE:_CONFIGURED",
    "USB_DEVICE:_DECONFIGURED",
    "USB_DEVICE:_CONTROL_TRANSFER_ABORTED",
    "USB_DEVICE:_CONTROL_TRANSFER_DATA_RECEIVED",
    "USB_DEVICE:_CONTROL_TRANSFER_SETUP_REQUEST",
    "USB_DEVICE:_CONTROL_TRANSFER_DATA_SENT",
    "USB_DEVICE:_ENDPOINT_READ_COMPLETE",
    "USB_DEVICE:_ENDPOINT_WRITE_COMPLETE",
    "USB_DEVICE:_SET_DESCRIPTOR",
    "USB_DEVICE:_SYNCH_FRAME",
};

char * usbAudioEventStr[] =
{
    "USB_AUDIO:_WRITE_COMPLETE ",
    "USB_AUDIO:_READ_COMPLETE ",
    "USB_AUDIO:_STATUS_SEND_COMPLETE ", 
    "USB_AUDIO:_INTERFACE_SETTING_CHANGED",
    "USB_AUDIO:_CONTROL_TRANSFER_DATA_RECEIVED",
    "USB_AUDIO:_CONTROL_TRANSFER_DATA_SENT",
    "USB_AUDIO:_CONTROL_TRANSFER_UNKNOWN",
    "USB_AUDIO:_CONTROL_SET_CUR",
    "USB_AUDIO:_CONTROL_SET_MIN",
    "USB_AUDIO:_CONTROL_SET_MAX",
    "USB_AUDIO:_CONTROL_SET_RES",
    "USB_AUDIO:_ENTITY_SET_MEM",
    "USB_AUDIO:_CONTROL_GET_CUR",
    "USB_AUDIO:_CONTROL_GET_MIN",
    "USB_AUDIO:_CONTROL_GET_MAX",
    "USB_AUDIO:_CONTROL_GET_RES",
    "USB_AUDIO:_ENTITY_GET_MEM",
    "USB_AUDIO:_ENTITY_GET_STAT",
};