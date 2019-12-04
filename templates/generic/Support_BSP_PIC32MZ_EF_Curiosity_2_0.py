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

pic32mzCuriosity20I2sComponentIDList = ["a_drv_i2s", "drv_i2c", "i2c1", "a_i2s2", "sys_time", "core_timer"]		
pic32mzCuriosity20I2sAutoConnectList = [["audio_codec_generic", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                      ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_i2s2", "SPI2_I2S"],
                      ["audio_codec_generic", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c1", "I2C1_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"]]
pic32mzCuriosity20I2sPinConfigs = [{"pin": 96, "name": "SDA1", "type": "SDA1", "direction": "", "latch": "", "abcd": ""},  						# RA15
                    {"pin": 95, "name": "SCL1", "type": "SCL1", "direction": "", "latch": "", "abcd": ""},           							# RA14
                    {"pin": 15, "name": "SDI2", "type": "SDI2", "direction": "", "latch": "", "opendrain": "", "abcd": ""},                     # RG7
                    {"pin": 70, "name": "REFCLKO1", "type": "REFCLKO", "direction": "", "latch": "", "opendrain": "", "abcd": ""},              # RD15
                    {"pin": 11, "name": "SS2 (out)", "type": "SS2 (out)", "direction": "", "latch": "", "opendrain": "", "abcd": ""},           # RC2
                    {"pin": 14, "name": "SCK2", "type": "SCK2", "direction": "", "latch": "", "opendrain": "", "abcd": ""},                     # RG6
                    {"pin": 16, "name": "SDO2", "type": "SDO2", "direction": "", "latch": "", "opendrain": "", "abcd": ""},                     # RG8
                    {"pin": 10, "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "opendrain": "", "abcd": ""}]           # RJ10
					
pic32mz_ef_curiosity20_I2S = bspSupportObj(pic32mzCuriosity20I2sPinConfigs, pic32mzCuriosity20I2sComponentIDList, None, pic32mzCuriosity20I2sAutoConnectList, None)

addBSPSupport("BSP_PIC32MZ_EF_Curiosity_2.0", "PIC32MZ_CURIOSITY20_I2S", pic32mz_ef_curiosity20_I2S)