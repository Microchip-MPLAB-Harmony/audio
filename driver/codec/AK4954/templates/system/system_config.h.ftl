/*** Codec Driver Configuration ***/

<#if USE_DRV_CODEC_AK4954 ==  true>
#define DRV_AK4954_CLIENTS_NUMBER                           ${DRV_AK4954_CLIENTS_NUMBER}
#define DRV_AK4954_INSTANCES_NUMBER                         1
<#if SYS_CLK_REFCLK_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK_ENABLE == true>
#define DRV_AK4954_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL}
</#if></#if>
<#if SYS_CLK_REFCLK0_ENABLE?has_content><#if CONFIG_SYS_CLK_REFCLK0_ENABLE == true>
#define DRV_AK4954_INPUT_REFCLOCK    	                	${SYS_CLK_REFCLK_ROSEL0}
</#if></#if>
</#if>

<#if DRV_AK4954_MASTER_MODE == "MASTER">
#define DRV_AK4954_I2S_MASTER_MODE                              true
<#else>
#define DRV_AK4954_I2S_MASTER_MODE                              false
</#if>
#define DRV_AK4954_AUDIO_SAMPLING_RATE                      ${DRV_AK4954_BAUD_RATE}
#define DRV_AK4954_VOLUME	                      	        ${DRV_AK4954_VOLUME}
#define DRV_AK4954_AUDIO_DATA_FORMAT_MACRO             	    ${DRV_AK4954_AUDIO_DATA_FORMAT}
#define DRV_AK4954_WHICH_MIC_INPUT                          ${DRV_AK4954_WHICH_MIC_INPUT}
#define DRV_AK4954_ENABLE_MIC_BIAS                          ${DRV_AK4954_ENABLE_MIC_BIAS?c}
#define DRV_AK4954_MIC_GAIN	                      	        ${DRV_AK4954_MIC_GAIN}
#define DRV_AK4954_MCLK_SAMPLE_FREQ_MULTPLIER               256
#define DRV_AK4954_BCLK_BIT_CLK_DIVISOR                     4
#define DRV_AK4954_DELAY_INITIALIZATION                     ${DRV_AK4954_DELAY_INITIALIZATION?c}

<#-- Instance 0 -->
<#if DRV_CODEC_AK4954_INST_IDX0 == true>
<#if DRV_AK4954_I2S != "I2S">
#define DRV_AK4954_I2S_DRIVER_MODULE_INDEX_IDX0             ${DRV_AK4954_I2S_INDEX}
</#if>
#define DRV_AK4954_I2C_DRIVER_MODULE_INDEX_IDX0             ${DRV_AK4954_I2C_INDEX}
</#if> 
/* CODEC Driver Abstraction definition */

#define DRV_CODEC_INDEX_0                                   DRV_AK4954_INDEX_0
#define sysObjdrvCodec0                                     sysObj.drvak4954Codec0
#define DRV_CODEC_I2S_MASTER_MODE                           DRV_AK4954_I2S_MASTER_MODE

<#if DRV_AK4954_I2S != "I2S">
#define DRV_CODEC_BUFFER_HANDLE                             DRV_AK4954_BUFFER_HANDLE
#define DRV_CODEC_BUFFER_HANDLE_INVALID                     DRV_AK4954_BUFFER_HANDLE_INVALID
#define DRV_CODEC_BUFFER_EVENT_HANDLER                      DRV_AK4954_BUFFER_EVENT_HANDLER
#define DRV_CODEC_BUFFER_EVENT                              DRV_AK4954_BUFFER_EVENT
#define DRV_CODEC_BUFFER_EVENT_COMPLETE                     DRV_AK4954_BUFFER_EVENT_COMPLETE
#define DRV_CODEC_BUFFER_EVENT_ERROR                        DRV_AK4954_BUFFER_EVENT_ERROR
#define DRV_CODEC_BUFFER_EVENT_ABORT                        DRV_AK4954_BUFFER_EVENT_ABORT
</#if>

#define DRV_CODEC_COMMAND_EVENT_HANDLER                     DRV_AK4954_COMMAND_EVENT_HANDLER
#define DRV_CODEC_AUDIO_SAMPLING_RATE                       DRV_AK4954_AUDIO_SAMPLING_RATE
#define DRV_CODEC_WHICH_MIC_INPUT                           DRV_AK4954_WHICH_MIC_INPUT
#define DRV_CODEC_MIC_GAIN                                  DRV_AK4954_MIC_GAIN
#define DRV_CODEC_CHANNEL_LEFT                              DRV_AK4954_CHANNEL_LEFT
#define DRV_CODEC_CHANNEL_RIGHT                             DRV_AK4954_CHANNEL_RIGHT
#define DRV_CODEC_CHANNEL_LEFT_RIGHT                        DRV_AK4954_CHANNEL_LEFT_RIGHT

#define DRV_CODEC_Initialize                                DRV_AK4954_Initialize
#define DRV_CODEC_Deinitialize                              DRV_AK4954_Deinitialize
#define DRV_CODEC_Status                                    DRV_AK4954_Status
#define DRV_CODEC_ClientReady                               DRV_AK4954_ClientReady
#define DRV_CODEC_Tasks                                     DRV_AK4954_Tasks
#define DRV_CODEC_Open                                      DRV_AK4954_Open
#define DRV_CODEC_Close                                     DRV_AK4954_Close
#define DRV_CODEC_CommandEventHandlerSet                    DRV_AK4954_CommandEventHandlerSet
<#if DRV_AK4954_I2S != "I2S">
#define DRV_CODEC_BufferEventHandlerSet                     DRV_AK4954_BufferEventHandlerSet
#define DRV_CODEC_BufferAddWrite                            DRV_AK4954_BufferAddWrite
#define DRV_CODEC_BufferAddRead                             DRV_AK4954_BufferAddRead
#define DRV_CODEC_BufferAddWriteRead                        DRV_AK4954_BufferAddWriteRead
#define DRV_CODEC_WriteQueuePurge                           DRV_AK4954_WriteQueuePurge
#define DRV_CODEC_ReadQueuePurge                            DRV_AK4954_ReadQueuePurge
</#if>
#define DRV_CODEC_SamplingRateSet                           DRV_AK4954_SamplingRateSet
#define DRV_CODEC_SamplingRateGet                           DRV_AK4954_SamplingRateGet
#define DRV_CODEC_VolumeSet                                 DRV_AK4954_VolumeSet
#define DRV_CODEC_VolumeGet                                 DRV_AK4954_VolumeGet
#define DRV_CODEC_MuteOn                                    DRV_AK4954_MuteOn
#define DRV_CODEC_MuteOff                                   DRV_AK4954_MuteOff
#define DRV_CODEC_MicGainSet                                DRV_AK4954_MicGainSet
#define DRV_CODEC_MicGainGet                                DRV_AK4954_MicGainGet
#define DRV_CODEC_MicMuteOn                                 DRV_AK4954_MicMuteOn
#define DRV_CODEC_MicMuteOff                                DRV_AK4954_MicMuteOff
<#if DRV_AK4954_I2S != "I2S">
#define DRV_CODEC_GetI2SDriver                              DRV_AK4954_GetI2SDriver
#define DRV_CODEC_LRCLK_Sync                                DRV_AK4954_LRCLK_Sync
</#if>
#define DRV_CODEC_EnableInitialization                      DRV_AK4954_EnableInitialization    
#define DRV_CODEC_IsInitializationDelayed                   DRV_AK4954_IsInitializationDelayed         
