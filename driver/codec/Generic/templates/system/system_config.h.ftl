/*** Codec Driver Configuration ***/

<#if USE_DRV_CODEC_GENERIC ==  true>
#define DRV_GENERICCODEC_CLIENTS_NUMBER                           ${DRV_GENERICCODEC_CLIENTS_NUMBER}
#define DRV_GENERICCODEC_INSTANCES_NUMBER                         1
<#if SYS_CLK_REFCLK_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK_ENABLE == true>
#define DRV_GENERICCODEC_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL}
</#if></#if>
<#if SYS_CLK_REFCLK0_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK0_ENABLE == true>
#define DRV_GGENERICCODEC_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL0}
</#if></#if>
</#if>

#define DRV_GENERICCODEC_AUDIO_SAMPLING_RATE                        ${DRV_GENERICCODEC_BAUD_RATE}
#define DRV_GENERICCODEC_VOLUME	                      	            ${DRV_GENERICCODEC_VOLUME}
#define DRV_GENERICCODEC_AUDIO_DATA_FORMAT_MACRO                    ${DRV_GENERICCODEC_AUDIO_DATA_FORMAT}

<#-- Instance 0 -->
<#if DRV_CODEC_GENERIC_INST_IDX0 == true>
#define DRV_GENERICCODEC_I2S_DRIVER_MODULE_INDEX_IDX0             DRV_I2S_INDEX_0
#define DRV_GENERICCODEC_I2C_DRIVER_MODULE_INDEX_IDX0             DRV_I2C_INDEX_0
</#if> 
/* CODEC Driver Abstraction definition */

#define DRV_CODEC_INDEX_0                                   DRV_GENERICCODEC_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvgegerniccodecCodec0
#define DRV_CODEC_BUFFER_HANDLE                             DRV_GENERICCODEC_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_GENERICCODEC_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_GENERICCODEC_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_GENERICCODEC_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_GENERICCODEC_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_GENERICCODEC_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_GENERICCODEC_BUFFER_EVENT_ABORT
#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_GENERICCODEC_COMMAND_EVENT_HANDLER

#define DRV_CODEC_CHANNEL_LEFT                              DRV_GENERICCODEC_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_GENERICCODEC_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_GENERICCODEC_CHANNEL_LEFT_RIGHT

#define DRV_CODEC_Initialize                                DRV_GENERICCODEC_Initialize
#define DRV_CODEC_Deinitialize                              DRV_GENERICCODEC_Deinitialize
#define DRV_CODEC_Status                                    DRV_GENERICCODEC_Status
#define DRV_CODEC_Tasks                                     DRV_GENERICCODEC_Tasks
#define DRV_CODEC_Open                                      DRV_GENERICCODEC_Open
#define DRV_CODEC_Close                                     DRV_GENERICCODEC_Close
#define DRV_CODEC_BufferEventHandlerSet                     DRV_GENERICCODEC_BufferEventHandlerSet
#define DRV_CODEC_CommandEventHandlerSet                    DRV_GENERICCODEC_CommandEventHandlerSet
#define DRV_CODEC_BufferAddWrite                            DRV_GENERICCODEC_BufferAddWrite
#define DRV_CODEC_BufferAddRead                             DRV_GENERICCODEC_BufferAddRead
#define DRV_CODEC_BufferAddWriteRead                        DRV_GENERICCODEC_BufferAddWriteRead
#define DRV_CODEC_WriteQueuePurge                           DRV_GENERICCODEC_WriteQueuePurge
#define DRV_CODEC_ReadQueuePurge                            DRV_GENERICCODEC_ReadQueuePurge
#define DRV_CODEC_SamplingRateSet                           DRV_GENERICCODEC_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_GENERICCODEC_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_GENERICCODEC_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_GENERICCODEC_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_GENERICCODEC_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_GENERICCODEC_MuteOff
#define DRV_CODEC_MicGainSet                                DRV_GENERICCODEC_MicGainSet
#define DRV_CODEC_MicGainGet                                DRV_GENERICCODEC_MicGainGet
#define DRV_CODEC_MicMuteOn                                 DRV_GENERICCODEC_MicMuteOn
#define DRV_CODEC_MicMuteOff                                DRV_GENERICCODEC_MicMuteOff
#define DRV_CODEC_GetI2SDriver                              DRV_GENERICCODEC_GetI2SDriver
#define DRV_CODEC_LRCLK_Sync                                DRV_GENERICCODEC_LRCLK_Sync 
