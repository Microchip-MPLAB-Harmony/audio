/*** CODEC Driver Initialization Data ***/
<#-- Instance 0 -->
<#if DRV_CODEC_AK4953_INST_IDX0 == true>
const DRV_AK4953_INIT drvak4953Codec0InitData =
{
    .i2sDriverModuleIndex = DRV_AK4953_I2S_DRIVER_MODULE_INDEX_IDX0,
    .i2cDriverModuleIndex = DRV_AK4953_I2C_DRIVER_MODULE_INDEX_IDX0,
    .masterMode = DRV_AK4953_MASTER_MODE,
    .samplingRate = DRV_AK4953_AUDIO_SAMPLING_RATE,
    .volume = DRV_AK4953_VOLUME,
    .audioDataFormat = DRV_AK4953_AUDIO_DATA_FORMAT_MACRO,
    .whichMicInput = DRV_AK4953_WHICH_MIC_INPUT,
    .enableMicBias = DRV_AK4953_ENABLE_MIC_BIAS,
    .micGain = DRV_AK4953_MIC_GAIN,
    .delayDriverInitialization = DRV_AK4953_DELAY_INITIALIZATION
};
</#if>
