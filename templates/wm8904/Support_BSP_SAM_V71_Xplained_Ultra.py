sscComponentIDList = ["drv_i2s", "drv_i2c", "twihs0", "ssc", "sys_time", "tc0"]		
sscAutoConnectList = [["audio_codec_wm8904", "DRV_I2S", "drv_i2s_0", "drv_i2s"],
                      ["drv_i2s_0", "drv_i2s_I2S_dependency", "ssc", "SSC_I2S"],
                      ["audio_codec_wm8904", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
					
sam_v71_xplained_ultra_SSC = bspSupportObj(None, sscComponentIDList, None, sscAutoConnectList, None)

addBSPSupport("BSP_SAM_V71_Xplained_Ultra", "SSC", sam_v71_xplained_ultra_SSC)