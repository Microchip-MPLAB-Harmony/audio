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

pic32mxBtadkI2sComponentIDList = ["a_drv_i2s", "drv_i2c", "i2c1", "a_i2s1", "sys_time", "core_timer"]		
pic32mxBtadkI2sAutoConnectList = [["audio_codec_ak4954", "DRV_I2S", "a_drv_i2s_0", "drv_i2s"],
                      ["a_drv_i2s_0", "drv_i2s_I2S_dependency", "a_i2s1", "SPI1_I2S"],
                      ["audio_codec_ak4954", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c1", "I2C1_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"]]
pic32mxBtadkI2sPinConfigs = [{"pin": 67, "name": "SDA1", "type": "SDA1", "direction": "", "latch": "", "abcd": ""},  # RA15
                    {"pin": 66, "name": "SCL1", "type": "SCL1", "direction": "", "latch": "", "abcd": ""},           # RA14
                    {"pin": 73, "name": "SDI1", "type": "SDI1", "direction": "", "latch": "", "abcd": ""},           # RC13
                    {"pin": 53, "name": "REFCLKO", "type": "REFCLKO", "direction": "", "latch": "", "abcd": ""},     # RF8
                    {"pin": 69, "name": "SS1 (out)", "type": "SS1 (out)", "direction": "", "latch": "", "abcd": ""}, # RD9
                    {"pin": 70, "name": "SCK1", "type": "SCK1", "direction": "", "latch": "", "abcd": ""},           # RD10
                    {"pin": 72, "name": "SDO1", "type": "SDO1", "direction": "", "latch": "", "abcd": ""},           # RD0
                    {"pin": 1,  "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] # RG15
					
pic32mx_bluetooth_audio_dev_kit_I2S = bspSupportObj(pic32mxBtadkI2sPinConfigs, pic32mxBtadkI2sComponentIDList, None, pic32mxBtadkI2sAutoConnectList, None)

addBSPSupport("BSP_PIC32MX_Bluetooth_Audio_Development_Kit", "PIC32MX_BTADK_I2S", pic32mx_bluetooth_audio_dev_kit_I2S)