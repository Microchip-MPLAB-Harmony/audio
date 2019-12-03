# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

pic32mzefSkI2sComponentIDList = ["a_drv_i2s", "drv_i2c", "i2c_bb", "a_i2s1", "tmr2", "sys_time", "core_timer"]		
pic32mzefSkI2sAutoConnectList = [["audio_codec_ak4953", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                      ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_i2s1", "SPI1_I2S"],
                      ["audio_codec_ak4953", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c_bb", "I2C"],
                      ["i2c_bb", "TMR", "tmr2", "TMR2_TMR"],
                      ["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"]]
pic32mzefSkI2sPinConfigs = [{"pin": 86, "name": "SDA2", "type": "GPIO", "direction": "in", "latch": "", "opendrain": "True", "abcd": ""},# RA15
                    {"pin": 85, "name": "SCL2", "type": "GPIO", "direction": "in", "latch": "", "opendrain": "True", "abcd": ""},      # RA14
                    {"pin": 69, "name": "SDI1", "type": "SDI1", "direction": "", "latch": "", "abcd": ""},           # RD14
                    {"pin": 70,  "name": "REFCLKO1", "type": "REFCLKO", "direction": "", "latch": "", "abcd": ""},    # RD15
                    {"pin": 58,  "name": "SS1 (out)", "type": "SS1 (out)", "direction": "", "latch": "", "abcd": ""}, # RF12
                    {"pin": 109, "name": "SCK1", "type": "SCK1", "direction": "", "latch": "", "abcd": ""},           # RD1
                    {"pin": 49,  "name": "SDO1", "type": "SDO1", "direction": "", "latch": "", "abcd": ""},           # RB10
                    {"pin": 46,  "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] # RH3
					
pic32mz_ef_starter_kit_I2S = bspSupportObj(pic32mzefSkI2sPinConfigs, pic32mzefSkI2sComponentIDList, None, pic32mzefSkI2sAutoConnectList, None)

addBSPSupport("BSP_PIC32MZ_EF_Starter_Kit", "PIC32MZ_EF_SK_I2S", pic32mz_ef_starter_kit_I2S)