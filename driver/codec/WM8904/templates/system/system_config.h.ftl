/*** Codec Driver Configuration ***/

<#if USE_DRV_CODEC_WM8904 ==  true>
#define DRV_WM8904_CLIENTS_NUMBER                           ${DRV_WM8904_CLIENTS_NUMBER}
#define DRV_WM8904_INSTANCES_NUMBER                         1
<#if SYS_CLK_REFCLK_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK_ENABLE == true>
#define DRV_WM8904_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL}
</#if></#if>
<#if SYS_CLK_REFCLK0_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK0_ENABLE == true>
#define DRV_WM8904_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL0}
</#if></#if>
</#if>

<#if DRV_WM8904_MASTER_MODE == "MASTER">
#define DRV_WM8904_I2S_MASTER_MODE                          true
<#else>
#define DRV_WM8904_I2S_MASTER_MODE                          false
</#if>
#define DRV_WM8904_AUDIO_SAMPLING_RATE                      ${DRV_WM8904_BAUD_RATE}
#define DRV_WM8904_VOLUME	                      	        ${DRV_WM8904_VOLUME}
#define DRV_WM8904_AUDIO_DATA_FORMAT_MACRO             	    ${DRV_WM8904_AUDIO_DATA_FORMAT}
#define DRV_WM8904_ENABLE_MIC_INPUT                         ${DRV_WM8904_ENABLE_MIC_INPUT?c}
#define DRV_WM8904_ENABLE_MIC_BIAS                          ${DRV_WM8904_ENABLE_MIC_BIAS?c}
#define DRV_WM8904_MIC_GAIN	                      	        ${DRV_WM8904_MIC_GAIN}

<#-- Instance 0 -->
<#if DRV_CODEC_WM8904_INST_IDX0 == true>
#define DRV_WM8904_I2S_DRIVER_MODULE_INDEX_IDX0             ${DRV_WM8904_I2S?replace("DRV_I2S_", "DRV_I2S_INDEX_")}
#define DRV_WM8904_I2C_DRIVER_MODULE_INDEX_IDX0             ${DRV_WM8904_I2C?replace("DRV_I2C_", "DRV_I2C_INDEX_")}
</#if> 
/* CODEC Driver Abstraction definition */

//Codec Driver Instance
#define DRV_CODEC_INDEX_0                                   DRV_WM8904_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvwm8904Codec0
#define DRV_CODEC_I2S_MASTER_MODE                           DRV_WM8904_I2S_MASTER_MODE
#define DRV_CODEC_BUFFER_HANDLE                             DRV_WM8904_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_WM8904_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_WM8904_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_WM8904_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_WM8904_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_WM8904_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_WM8904_BUFFER_EVENT_ABORT
#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_WM8904_COMMAND_EVENT_HANDLER

//Codec Driver API Configuration Parameters
#define DRV_CODEC_CHANNEL_LEFT                              DRV_WM8904_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_WM8904_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_WM8904_CHANNEL_LEFT_RIGHT
#define DRV_CODEC_AUDIO_SAMPLING_RATE                       DRV_WM8904_AUDIO_SAMPLING_RATE
#define DRV_CODEC_WHICH_MIC_INPUT                           DRV_WM8904_WHICH_MIC_INPUT
#define DRV_CODEC_VOLUME	                        	    DRV_WM8904_VOLUME
#define DRV_CODEC_AUDIO_DATA_FORMAT_MACRO                 	DRV_WM8904_AUDIO_DATA_FORMAT_MACRO
#define DRV_CODEC_ENABLE_MIC_INPUT                          DRV_WM8904_ENABLE_MIC_INPUT
#define DRV_CODEC_ENABLE_MIC_BIAS                           DRV_WM8904_ENABLE_MIC_BIAS
#define DRV_CODEC_MIC_GAIN                                  DRV_WM8904_MIC_GAIN

//Codec Driver API Functions
#define DRV_CODEC_Initialize                                DRV_WM8904_Initialize
#define DRV_CODEC_Deinitialize                              DRV_WM8904_Deinitialize
#define DRV_CODEC_Status                                    DRV_WM8904_Status
#define DRV_CODEC_ClientReady                               DRV_WM8904_ClientReady
#define DRV_CODEC_Tasks                                     DRV_WM8904_Tasks
#define DRV_CODEC_Open                                      DRV_WM8904_Open
#define DRV_CODEC_Close                                     DRV_WM8904_Close
#define DRV_CODEC_BufferEventHandlerSet                     DRV_WM8904_BufferEventHandlerSet
#define DRV_CODEC_CommandEventHandlerSet                    DRV_WM8904_CommandEventHandlerSet
#define DRV_CODEC_BufferAddWrite                            DRV_WM8904_BufferAddWrite
#define DRV_CODEC_BufferAddRead                             DRV_WM8904_BufferAddRead
#define DRV_CODEC_BufferAddWriteRead                        DRV_WM8904_BufferAddWriteRead
#define DRV_CODEC_WriteQueuePurge                           DRV_WM8904_WriteQueuePurge
#define DRV_CODEC_ReadQueuePurge                            DRV_WM8904_ReadQueuePurge
#define DRV_CODEC_SamplingRateSet                           DRV_WM8904_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_WM8904_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_WM8904_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_WM8904_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_WM8904_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_WM8904_MuteOff
#define DRV_CODEC_StereoMicSelect                           DRV_WM8904_StereoMicSelect
#define DRV_CODEC_MicGainSet                                DRV_WM8904_MicGainSet
#define DRV_CODEC_MicGainGet                                DRV_WM8904_MicGainGet
#define DRV_CODEC_MicMuteOn                                 DRV_WM8904_MicMuteOn
#define DRV_CODEC_MicMuteOff                                DRV_WM8904_MicMuteOff
#define DRV_CODEC_GetI2SDriver                              DRV_WM8904_GetI2SDriver
#define DRV_CODEC_LRCLK_Sync                                DRV_WM8904_LRCLK_Sync 
