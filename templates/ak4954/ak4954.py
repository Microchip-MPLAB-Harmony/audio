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

componentsIDTable = ["HarmonyCore", "audio_codec_ak4954"]
#deactivateIDTable = ["FreeRTOS"]

execfile(Module.getPath() + "../common/pin_config.py")
execfile(Module.getPath() + "../common/bsp_utils.py")

#Add BSP support
#execfile(Module.getPath() + "Support_BSP_SAM_E54_Curiosity_Ultra.py")
execfile(Module.getPath() + "Support_BSP_SAM_E70_Xplained_Ultra.py")

def enableSSCPins(bspID, enable):
    SSCPinConfigs = getBSPSupportNode(bspID, "SSC").getPinConfig()
    if SSCPinConfigs:
        resetPins(SSCPinConfigs)
        if (enable == True):
            configurePins(SSCPinConfigs)

def enableI2SPins(bspID, enable):
    I2SPinConfigs = getBSPSupportNode(bspID, "I2S").getPinConfig()
    if I2SPinConfigs:
        resetPins(I2SPinConfigs)
        if (enable == True):
            configurePins(I2SPinConfigs)

def enableSSCInterface(bspID, enable):
    if getBSPSupportNode(bspID, "SSC"):
        componentIDTable = getBSPSupportNode(bspID, "SSC").getComponentActivateList()
        autoConnectTable = getBSPSupportNode(bspID, "SSC").getComponentAutoConnectList()
        if (enable == True):
            res = Database.activateComponents(componentIDTable)
            res = Database.connectDependencies(autoConnectTable)
        elif (enable == False):
            res = Database.deactivateComponents(componentIDTable)
        enableSSCPins(bspID, enable)

def enableI2SInterface(bspID, enable):
    if getBSPSupportNode(bspID, "I2S"):
        componentIDTable = getBSPSupportNode(bspID, "I2S").getComponentActivateList()
        autoConnectTable = getBSPSupportNode(bspID, "I2S").getComponentAutoConnectList()
        if (enable == True):
            res = Database.activateComponents(componentIDTable)
            res = Database.connectDependencies(autoConnectTable)
        elif (enable == False):
            res = Database.deactivateComponents(componentIDTable)
        enableI2SPins(bspID, enable)

def configureAK4954Interface(bspID, interface):
    print("Configuring for " + str(interface) + " Interface.")
    if bspID == None:
        print("No BSP used, will not configure")
    else:
        if (str(interface) == "SSC"):
            enableSSCInterface(bspID, True)
            enableI2SInterface(bspID, False)
        elif (str(interface) == "I2S"):
            enableSSCInterface(bspID, False)
            enableI2SInterface(bspID, True)

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

    if getBSPSupportNode(bspID, "SSC"):
        configureAK4954Interface(bspID, "SSC")	
    elif getBSPSupportNode(bspID, "I2S"):
        configureAK4954Interface(bspID, "I2S")

