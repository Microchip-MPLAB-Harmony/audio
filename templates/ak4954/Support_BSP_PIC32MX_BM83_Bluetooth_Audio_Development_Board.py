# coding: utf-8
##############################################################################
# Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

pic32mxBm83BtadbComponentIDList = ["drv_i2c", "i2c1", "sys_time", "core_timer"]		
pic32mxBm83BtadbAutoConnectList = [["audio_codec_ak4954", "DRV_I2C", "drv_i2c_0", "drv_i2c"],
                      ["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c1", "I2C1_I2C"],
                      ["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"]]
pic32mxBm83BtadbPinConfigs = [{"pin": 67, "name": "SDA1", "type": "SDA1", "direction": "", "latch": "", "opendrain": "", "abcd": ""},  # RA15
                    {"pin": 66, "name": "SCL1", "type": "SCL1", "direction": "", "latch": "", "opendrain": "", "abcd": ""},            # RA14
                    {"pin": 91,  "name": "STBYRST", "type": "GPIO", "direction": "Out", "latch": "High", "opendrain": "true", "pullup": "true", "abcd": ""}] # RA6
					
pic32mx_bm83_bluetooth_audio_dev_bd = bspSupportObj(pic32mxBm83BtadbPinConfigs, pic32mxBm83BtadbComponentIDList, None, pic32mxBm83BtadbAutoConnectList, None)

addBSPSupport("BSP_PIC32MX_BM83_Bluetooth_Audio_Development_Board", "PIC32MX_BM83_BTADB", pic32mx_bm83_bluetooth_audio_dev_bd)
