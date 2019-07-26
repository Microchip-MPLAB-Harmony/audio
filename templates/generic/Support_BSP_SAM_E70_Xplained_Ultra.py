# coding: utf-8
##############################################################################
# Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

i2scComponentIDList = ["a_drv_i2s", "drv_i2c", "twihs0", "a_i2sc1", "sys_time", "tc0"]
i2scAutoConnectList = [["audio_codec_generic", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                       ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_i2sc1", "I2SC1_I2S"],
                       ["audio_codec_generic", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                       ["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
                       ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
i2scPinConfigs =   [{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"},    # PA3
                    {"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": "", "abcd": "A"},  # PA4
                    {"pin": 24, "name": "PMC_PCK2", "type": "PMC_PCK2", "direction": "", "latch": "", "abcd": "B"},          # PA18
                    {"pin": 23, "name": "I2SC1_MCK", "type": "I2SC1_MCK", "direction": "", "latch": "", "abcd": "D"},        # PA19
                    {"pin": 22, "name": "I2SC1_CK", "type": "I2SC1_CK", "direction": "", "latch": "", "abcd": "D"},          # PA20
                    {"pin":  4, "name": "I2SC1_WS", "type": "I2SC1_WS", "direction": "", "latch": "", "abcd": "C"},          # PE0
                    {"pin":  6, "name": "I2SC1_DO0", "type": "I2SC1_DO0", "direction": "", "latch": "", "abcd": "C"},        # PE1
                    {"pin":  7, "name": "I2SC1_DI0", "type": "I2SC1_DI0", "direction": "", "latch": "", "abcd": "C"},        # PE2
                    {"pin": 98, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}]         # PD11

sscComponentIDList = ["a_drv_i2s", "drv_i2c", "twihs0", "a_ssc", "sys_time", "tc0"]		
sscAutoConnectList = [["audio_codec_generic", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                      ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_ssc", "SSC_I2S"],
                      ["audio_codec_generic", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
sscPinConfigs =    [{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"},    # PA3
                    {"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": "", "abcd": "A"},  # PA4
                    {"pin": 66, "name": "SSC_RD", "type": "SSC_RD", "direction": "", "latch": "", "abcd": "C"},              # PA10
                    {"pin": 24, "name": "PMC_PCK2", "type": "PMC_PCK2", "direction": "", "latch": "", "abcd": "B"},          # PA18
                    {"pin": 21, "name": "SSC_TF", "type": "SSC_TF", "direction": "", "latch": "", "abcd": "D"},              # PB0
                    {"pin": 20, "name": "SSC_TK", "type": "SSC_TK", "direction": "", "latch": "", "abcd": "D"},              # PB1
                    {"pin": 53, "name": "SSC_TD", "type": "SSC_TD", "direction": "", "latch": "", "abcd": "B"},              # PD26
                    {"pin": 98, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}]         # PD11
					
sam_e70_xplained_ultra_SSC = bspSupportObj(sscPinConfigs, sscComponentIDList, None, sscAutoConnectList, None)
sam_e70_xplained_ultra_I2SC = bspSupportObj(i2scPinConfigs, i2scComponentIDList, None, i2scAutoConnectList, None)

addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "SSC", sam_e70_xplained_ultra_SSC)
addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "I2SC", sam_e70_xplained_ultra_I2SC)