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

i2sComponentIDList = ["a_drv_i2s", "drv_i2c", "sercom6", "a_i2s", "sys_time", "tc0"]		
i2sAutoConnectList = [["audio_codec_generic", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                      ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_i2s", "I2S_I2S"],
                      ["audio_codec_generic", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "sercom6", "SERCOM6_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
i2sPinConfigs =    [{"pin": 48, "name": "SERCOM6_PAD0", "type": "SERCOM6_PAD0", "direction": "", "latch": "", "abcd": "D"},    # PD09/SDA
                    {"pin": 47, "name": "SERCOM6_PAD1", "type": "SERCOM6_PAD1", "direction": "", "latch": "", "abcd": "D"},  # PD08/SCL
                    {"pin": 39, "name": "I2S_SDI", "type": "I2S_SDI", "direction": "", "latch": "", "abcd": "J"},              # PB10
                    {"pin": 67, "name": "GCLK_IO3", "type": "GCLK_IO3", "direction": "", "latch": "", "abcd": "M"},          # PA17
                    {"pin": 34, "name": "I2S_FS0", "type": "I2S_FS0", "direction": "", "latch": "", "abcd": "J"},              # PA09
                    {"pin": 35, "name": "I2S_SCK0", "type": "I2S_SCK0", "direction": "", "latch": "", "abcd": "J"},              # PA10
                    {"pin": 36, "name": "I2S_SDO", "type": "I2S_SDO", "direction": "", "latch": "", "abcd": "J"},              # PA11
                    {"pin": 72, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}]         # PC18
					
sam_e54_curiosity_ultra_SSC = bspSupportObj(i2sPinConfigs, i2sComponentIDList, None, i2sAutoConnectList, None)

addBSPSupport("BSP_SAM_E54_Curiosity_Ultra", "SSC", sam_e54_curiosity_ultra_SSC)