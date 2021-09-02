/*** CODEC Driver Initialization Data ***/
<#-- Instance 0 -->
<#if DRV_CODEC_WM8904_INST_IDX0 == true>
const DRV_WM8904_INIT drvwm8904Codec0InitData =
{
<#if DRV_WM8904_I2S?has_content>
    .i2sDriverModuleIndex = DRV_WM8904_I2S_DRIVER_MODULE_INDEX_IDX0,
</#if>
<#if DRV_WM8904_I2C?has_content>
    .i2cDriverModuleIndex = DRV_WM8904_I2C_DRIVER_MODULE_INDEX_IDX0,
</#if>
    .masterMode = DRV_WM8904_I2S_MASTER_MODE,
    .samplingRate = DRV_WM8904_AUDIO_SAMPLING_RATE,
    .volume = DRV_WM8904_VOLUME,
    .audioDataFormat = DRV_WM8904_AUDIO_DATA_FORMAT_MACRO,
    .enableMicInput = DRV_WM8904_ENABLE_MIC_INPUT,
    .enableMicBias = DRV_WM8904_ENABLE_MIC_BIAS,
    .micGain = DRV_WM8904_MIC_GAIN
};
</#if>
