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

componentsIDTable = ["HarmonyCore", "audio_codec_ak4953"]
#deactivateIDTable = ["FreeRTOS"]

execfile(Module.getPath() + "../common/pin_config.py")
execfile(Module.getPath() + "../common/bsp_utils.py")

#Add BSP support
execfile(Module.getPath() + "Support_BSP_PIC32MZ_EF_Starter_Kit.py")

def enablePIC32MZ_EF_SK_I2SPins(bspID, enable):
    pinConfigs= getBSPSupportNode(bspID, "PIC32MZ_EF_SK_I2S").getPinConfig()
    if pinConfigs:
        resetPinsPIC32M(pinConfigs)
        if (enable == True):
            configurePinsPIC32M(pinConfigs)

def enablePIC32MZ_EF_SK_I2SInterface(bspID, enable):
    if getBSPSupportNode(bspID, "PIC32MZ_EF_SK_I2S"):
        componentIDTable = getBSPSupportNode(bspID, "PIC32MZ_EF_SK_I2S").getComponentActivateList()
        autoConnectTable = getBSPSupportNode(bspID, "PIC32MZ_EF_SK_I2S").getComponentAutoConnectList()
        if (enable == True):
            res = Database.activateComponents(componentIDTable)
            res = Database.connectDependencies(autoConnectTable)
        elif (enable == False):
            res = Database.deactivateComponents(componentIDTable)
        enablePIC32MZ_EF_SK_I2SPins(bspID, enable)

def configureAK4953Interface(bspID, interface):
    print("Configuring for " + str(interface) + " Interface.")
    if bspID == None:
        print("No BSP used, will not configure")
    else:
        if (str(interface) == "PIC32MZ_EF_SK_I2S"):
            enablePIC32MZ_EF_SK_I2SInterface(bspID, True)

def instantiateComponent(bspComponent):
    global componentsIDTable
    global autoConnectTable
    global supportedBSPsIDList
	
    #Check if a supported BSP is loaded
    bspID = getSupportedBSP()
    if bspID == None:
        return

    res = Database.activateComponents(componentsIDTable)
    #res = Database.connectDependencies(autoConnectTable)
    #res = Database.deactivateComponents(deactivateIDTable);

    if getBSPSupportNode(bspID, "PIC32MZ_EF_SK_I2S"):
        configureAK4953Interface(bspID, "PIC32MZ_EF_SK_I2S")

