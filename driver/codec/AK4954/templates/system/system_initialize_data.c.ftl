/*** CODEC Driver Initialization Data ***/
<#-- Instance 0 -->
<#if DRV_CODEC_AK4954_INST_IDX0 == true>
const DRV_AK4954_INIT drvak4954Codec0InitData =
{
<#if DRV_AK4954_I2S != "I2S">
    .i2sDriverModuleIndex = DRV_AK4954_I2S_DRIVER_MODULE_INDEX_IDX0,
</#if>
    .i2cDriverModuleIndex = DRV_AK4954_I2C_DRIVER_MODULE_INDEX_IDX0,
    .masterMode           = DRV_AK4954_I2S_MASTER_MODE,
    .samplingRate         = DRV_AK4954_AUDIO_SAMPLING_RATE,
    .volume               = DRV_AK4954_VOLUME,
    .audioDataFormat      = DRV_AK4954_AUDIO_DATA_FORMAT_MACRO,
    .whichMicInput        = DRV_AK4954_WHICH_MIC_INPUT,
    .enableMicBias        = DRV_AK4954_ENABLE_MIC_BIAS,
    .micGain              = DRV_AK4954_MIC_GAIN,
    .delayDriverInitialization = DRV_AK4954_DELAY_INITIALIZATION,
};
</#if>
