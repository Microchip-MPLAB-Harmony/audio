/*** Codec Driver Configuration ***/

<#if USE_DRV_CODEC_AK4953 ==  true>
#define DRV_AK4953_CLIENTS_NUMBER                           ${DRV_AK4953_CLIENTS_NUMBER}
#define DRV_AK4953_INSTANCES_NUMBER                         1
<#if SYS_CLK_REFCLK_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK_ENABLE == true>
#define DRV_AK4953_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL}
</#if></#if>
<#if SYS_CLK_REFCLK0_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK0_ENABLE == true>
#define DRV_AK4953_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL0}
</#if></#if>
</#if>

<#if DRV_AK4953_MASTER_MODE == "MASTER">
#define DRV_AK4953_I2S_MASTER_MODE                              true
<#else>
#define DRV_AK4953_I2S_MASTER_MODE                              false
</#if>
#define DRV_AK4953_AUDIO_SAMPLING_RATE                      ${DRV_AK4953_BAUD_RATE}
#define DRV_AK4953_VOLUME	                      	        ${DRV_AK4953_VOLUME}
#define DRV_AK4953_AUDIO_DATA_FORMAT_MACRO             	    ${DRV_AK4953_AUDIO_DATA_FORMAT}
#define DRV_AK4953_WHICH_MIC_INPUT                          ${DRV_AK4953_WHICH_MIC_INPUT}
#define DRV_AK4953_ENABLE_MIC_BIAS                          ${DRV_AK4953_ENABLE_MIC_BIAS?c}
#define DRV_AK4953_MIC_GAIN	                      	        ${DRV_AK4953_MIC_GAIN}
#define DRV_AK4953_MCLK_SAMPLE_FREQ_MULTPLIER               256
#define DRV_AK4953_BCLK_BIT_CLK_DIVISOR                     4
#define DRV_AK4953_DELAY_INITIALIZATION                     ${DRV_AK4953_DELAY_INITIALIZATION?c}

<#-- Instance 0 -->
<#if DRV_CODEC_AK4953_INST_IDX0 == true>
#define DRV_AK4953_I2S_DRIVER_MODULE_INDEX_IDX0             ${DRV_AK4953_I2S_INDEX}
#define DRV_AK4953_I2C_DRIVER_MODULE_INDEX_IDX0             ${DRV_AK4953_I2C_INDEX}
</#if> 
/* CODEC Driver Abstraction definition */

#define DRV_CODEC_INDEX_0                                   DRV_AK4953_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvak4953Codec0
#define DRV_CODEC_I2S_MASTER_MODE                           DRV_AK4953_I2S_MASTER_MODE
#define DRV_CODEC_BUFFER_HANDLE                             DRV_AK4953_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_AK4953_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_AK4953_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_AK4953_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_AK4953_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_AK4953_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_AK4953_BUFFER_EVENT_ABORT
#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_AK4953_COMMAND_EVENT_HANDLER

#define DRV_CODEC_CHANNEL_LEFT                              DRV_AK4953_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_AK4953_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_AK4953_CHANNEL_LEFT_RIGHT
#define DRV_CODEC_AUDIO_SAMPLING_RATE                       DRV_AK4953_AUDIO_SAMPLING_RATE
#define DRV_CODEC_WHICH_MIC_INPUT                          DRV_AK4953_WHICH_MIC_INPUT

#define DRV_CODEC_Initialize                                DRV_AK4953_Initialize
#define DRV_CODEC_Deinitialize                              DRV_AK4953_Deinitialize
#define DRV_CODEC_Status                                    DRV_AK4953_Status
#define DRV_CODEC_Tasks                                     DRV_AK4953_Tasks
#define DRV_CODEC_Open                                      DRV_AK4953_Open
#define DRV_CODEC_Close                                     DRV_AK4953_Close
//#define DRV_CODEC_ClientReady                               DRV_AK4953_ClientReady
#define DRV_CODEC_BufferEventHandlerSet                     DRV_AK4953_BufferEventHandlerSet
#define DRV_CODEC_CommandEventHandlerSet                    DRV_AK4953_CommandEventHandlerSet
#define DRV_CODEC_BufferAddWrite                            DRV_AK4953_BufferAddWrite
#define DRV_CODEC_BufferAddRead                             DRV_AK4953_BufferAddRead
#define DRV_CODEC_BufferAddWriteRead                        DRV_AK4953_BufferAddWriteRead
#define DRV_CODEC_WriteQueuePurge                           DRV_AK4953_WriteQueuePurge
#define DRV_CODEC_ReadQueuePurge                            DRV_AK4953_ReadQueuePurge
#define DRV_CODEC_SamplingRateSet                           DRV_AK4953_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_AK4953_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_AK4953_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_AK4953_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_AK4953_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_AK4953_MuteOff
#define DRV_CODEC_MicGainSet                                DRV_AK4953_MicGainSet
#define DRV_CODEC_MicGainGet                                DRV_AK4953_MicGainGet
#define DRV_CODEC_MicMuteOn                                 DRV_AK4953_MicMuteOn
#define DRV_CODEC_MicMuteOff                                DRV_AK4953_MicMuteOff
#define DRV_CODEC_GetI2SDriver                              DRV_AK4953_GetI2SDriver
#define DRV_CODEC_LRCLK_Sync                                DRV_AK4953_LRCLK_Sync 
