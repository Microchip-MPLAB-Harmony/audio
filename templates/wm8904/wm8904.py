componentsIDTable = ["HarmonyCore", "audio_codec_wm8904"]
deactivateIDTable = ["FreeRTOS"]

execfile(Module.getPath() + "../common/pin_config.py")
execfile(Module.getPath() + "../common/bsp_utils.py")

#Add BSP support
execfile(Module.getPath() + "Support_BSP_SAM_E70_Xplained_Ultra.py")

def enableI2SCPins(bspID, enable):
    I2SCPinConfigs = getBSPSupportNode(bspID, "I2SC").getPinConfig()
    resetPins(I2SCPinConfigs)
    if (enable == True):
		configurePins(I2SCPinConfigs)

def enableSSCPins(bspID, enable):
    SSCPinConfigs=  getBSPSupportNode(bspID, "SSC").getPinConfig()
    resetPins(SSCPinConfigs)
    if (enable == True):
        configurePins(SSCPinConfigs)

def enableI2SCInterface(bspID, enable):
    componentIDTable = getBSPSupportNode(bspID, "I2SC").getComponentActivateList()
    autoConnectTable = getBSPSupportNode(bspID, "I2SC").getComponentAutoConnectList()
    if (enable == True):
        res = Database.activateComponents(componentIDTable)
        res = Database.connectDependencies(autoConnectTable)
    elif (enable == False):
        res = Database.deactivateComponents(componentIDTable)
    enableI2SCPins(bspID, enable)
	
def enableSSCInterface(bspID, enable):
    componentIDTable = getBSPSupportNode(bspID, "SSC").getComponentActivateList()
    autoConnectTable = getBSPSupportNode(bspID, "SSC").getComponentAutoConnectList()
    if (enable == True):
        res = Database.activateComponents(componentIDTable)
        res = Database.connectDependencies(autoConnectTable)
    elif (enable == False):
        res = Database.deactivateComponents(componentIDTable)
    enableSSCPins(bspID, enable)

def configureWM8904Interface(bspID, interface):
    print("Configuring for " + str(interface) + " Interface.")
    if (bspID == None):
        print("No BSP used, will not configure")
    else:
        if (str(interface) == "SSC"):
            enableI2SCInterface(bspID, False)
            enableSSCInterface(bspID, True)
        elif (str(interface) == "I2SC"):
            enableSSCInterface(bspID, False)
            enableI2SCInterface(bspID, True)

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

    res = Database.activateComponents(componentsIDTable)
    #res = Database.connectDependencies(autoConnectTable)
    res = Database.deactivateComponents(deactivateIDTable);
	
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
	
    if (bspID != None):
        configureWM8904Interface(bspID, str(currWM8904Interface.getValue()))
    else:
        print("No BSP used, only software components are configured. Please add board-specific components.")
