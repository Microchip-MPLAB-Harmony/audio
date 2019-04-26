/*** CODEC Driver Initialization Data ***/
<#-- Instance 0 -->
<#if DRV_CODEC_GENERIC_INST_IDX0 == true>
const DRV_GENERICCODEC_INIT drvgenericCodec0InitData =
{
<#if DRV_GENERICCODEC_I2S_DRIVER_MODULE_INDEX_IDX0?has_content>
    .i2sDriverModuleIndex = DRV_GENERICCODEC_I2S_DRIVER_MODULE_INDEX_IDX0,
</#if>
<#if DRV_GENERICCODEC_I2C_DRIVER_MODULE_INDEX_IDX0?has_content>
    .i2cDriverModuleIndex = DRV_GENERIC_I2C_DRIVER_MODULE_INDEX_IDX0,
</#if>
    .samplingRate = DRV_GENERICCODEC_AUDIO_SAMPLING_RATE,
    .volume = DRV_GENERICCODEC_VOLUME,
    .audioDataFormat = DRV_GENERICCODEC_AUDIO_DATA_FORMAT_MACRO,
};
</#if>
