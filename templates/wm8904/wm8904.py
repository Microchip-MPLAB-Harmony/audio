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

componentsIDTable = ["HarmonyCore", "audio_codec_wm8904"]
#deactivateIDTable = ["FreeRTOS"]

execfile(Module.getPath() + "../common/pin_config.py")
execfile(Module.getPath() + "../common/bsp_utils.py")

#Add BSP support
execfile(Module.getPath() + "Support_BSP_SAM_E54_Curiosity_Ultra.py")
execfile(Module.getPath() + "Support_BSP_SAM_E70_Xplained_Ultra.py")
execfile(Module.getPath() + "Support_BSP_SAM_V71_Xplained_Ultra.py")

def enableI2SCPins(bspID, enable):
    I2SCPinConfigs = getBSPSupportNode(bspID, "I2SC").getPinConfig()
    if I2SCPinConfigs:
        resetPins(I2SCPinConfigs)
        if (enable == True):
    		configurePins(I2SCPinConfigs)

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

def enableI2SCInterface(bspID, enable):
    if getBSPSupportNode(bspID, "I2SC"):
        componentIDTable = getBSPSupportNode(bspID, "I2SC").getComponentActivateList()
        autoConnectTable = getBSPSupportNode(bspID, "I2SC").getComponentAutoConnectList()
        if (enable == True):
            res = Database.activateComponents(componentIDTable)
            res = Database.connectDependencies(autoConnectTable)
        elif (enable == False):
            res = Database.deactivateComponents(componentIDTable)
        enableI2SCPins(bspID, enable)
	
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

def configureWM8904Interface(bspID, interface):
    print("Configuring for " + str(interface) + " Interface.")
    if bspID == None:
        print("No BSP used, will not configure")
    else:
        if (str(interface) == "SSC"):
            enableI2SCInterface(bspID, False)
            enableSSCInterface(bspID, True)
            enableI2SInterface(bspID, False)
        elif (str(interface) == "I2SC"):
            enableSSCInterface(bspID, False)
            enableI2SCInterface(bspID, True)
            enableI2SInterface(bspID, False)
        elif (str(interface) == "I2S"):
            enableSSCInterface(bspID, False)
            enableI2SCInterface(bspID, False)
            enableI2SInterface(bspID, True)

def onWM8904InterfaceSelected(interfaceSelected, event):
    bspID = getSupportedBSP()
    newWM8904Interface= interfaceSelected.getComponent().getSymbolByID("WM8904Interface").getValue()
    currWM8904Interface = interfaceSelected.getComponent().getSymbolByID("currWM8904Interface").getValue()
    interfaceSelected.getComponent().getSymbolByID("currWM8904Interface").setValue(event["value"], 1)
    configureWM8904Interface(bspID, str(newWM8904Interface))

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
	
    if not getBSPSupportNode(bspID, "I2S"):
        if getBSPSupportNode(bspID, "SSC") and getBSPSupportNode(bspID, "I2SC"):        # if both are defined
            WM8904Interface = bspComponent.createComboSymbol("WM8904Interface", None, ["SSC", "I2SC"])
            WM8904Interface.setLabel("WM8904 Interface")
            WM8904Interface.setDescription("Configures the interface to the WM8904 codec.")
            WM8904Interface.setDefaultValue("SSC")
            WM8904Interface.setDependencies(onWM8904InterfaceSelected, ["WM8904Interface"])
            WM8904Interface.setVisible(True)
        	
        # Shadow display interface symbol
        currWM8904Interface = bspComponent.createComboSymbol("currWM8904Interface", None, ["SSC", "I2SC"])
        currWM8904Interface.setDefaultValue("SSC")
        currWM8904Interface.setVisible(False)

        configureWM8904Interface(bspID, str(currWM8904Interface.getValue()))
    else:
        configureWM8904Interface(bspID, "I2S")

