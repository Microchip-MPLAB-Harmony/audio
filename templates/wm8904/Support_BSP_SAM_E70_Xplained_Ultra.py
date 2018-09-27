i2scComponentIDList = ["drv_i2s", "drv_i2c", "twihs0", "i2sc1", "sys_time", "tc0"]
i2scAutoConnectList = [["audio_codec_wm8904", "DRV_I2S", "drv_i2s_0", "drv_i2s"],
                       ["drv_i2s_0", "drv_i2s_I2S_dependency", "i2sc1", "I2SC1_I2S"],
                       ["audio_codec_wm8904", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                       ["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
                       ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
i2scPinConfigs =   [{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": ""},    # PA3
                    {"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": ""},  # PA4
                    {"pin": 24, "name": "PMC_PCK2", "type": "PMC_PCK2", "direction": "", "latch": ""},          # PA18
                    {"pin": 23, "name": "I2SC1_MCK", "type": "I2SC1_MCK", "direction": "", "latch": ""},        # PA19
                    {"pin": 22, "name": "I2SC1_CK", "type": "I2SC1_CK", "direction": "", "latch": ""},          # PA20
                    {"pin":  4, "name": "I2SC1_WS", "type": "I2SC1_WS", "direction": "", "latch": ""},          # PE0
                    {"pin":  6, "name": "I2SC1_DO0", "type": "I2SC1_DO0", "direction": "", "latch": ""},        # PE1
                    {"pin":  7, "name": "I2SC1_DI0", "type": "I2SC1_DI0", "direction": "", "latch": ""},        # PE2
                    {"pin": 98, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High"}]        # PD11

sscComponentIDList = ["drv_i2s", "drv_i2c", "twihs0", "ssc", "sys_time", "tc0"]		
sscAutoConnectList = [["audio_codec_wm8904", "DRV_I2S", "drv_i2s_0", "drv_i2s"],
                      ["drv_i2s_0", "drv_i2s_I2S_dependency", "ssc", "SSC_I2S"],
                      ["audio_codec_wm8904", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
sscPinConfigs =    [{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": ""},    # PA3
                    {"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": ""},  # PA4
                    {"pin": 66, "name": "SSC_RD", "type": "SSC_RD", "direction": "", "latch": ""},              # PA10
                    {"pin": 24, "name": "PMC_PCK2", "type": "PMC_PCK2", "direction": "", "latch": ""},          # PA18
                    {"pin": 21, "name": "SSC_TF", "type": "SSC_TF", "direction": "", "latch": ""},              # PB0
                    {"pin": 20, "name": "SSC_TK", "type": "SSC_TK", "direction": "", "latch": ""},              # PB1
                    {"pin": 53, "name": "SSC_TD", "type": "SSC_TD", "direction": "", "latch": ""},              # PD26
                    {"pin": 98, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High"}]        # PD11
					
sam_e70_xplained_ultra_SSC = bspSupportObj(sscPinConfigs, sscComponentIDList, None, sscAutoConnectList, None)
sam_e70_xplained_ultra_I2SC = bspSupportObj(i2scPinConfigs, i2scComponentIDList, None, i2scAutoConnectList, None)

addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "SSC", sam_e70_xplained_ultra_SSC)
addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "I2SC", sam_e70_xplained_ultra_I2SC)