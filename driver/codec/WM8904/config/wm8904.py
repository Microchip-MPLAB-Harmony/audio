def customUpdate(wm8904Mode, event):
    global wm8904Baud 
    global custom

    if event["value"]==1:   # Master
        custom = True
    else:
        custom = False

    wm8904Baud.setVisible(custom) 

def instantiateComponent(wm8904Component):
    global custom
    global wm8904Baud 

    Log.writeInfoMessage("WM8904 instantiated")

    custom = True
    wm8904Index = wm8904Component.createIntegerSymbol("WM8904_INDEX", None)
    wm8904Index.setVisible(False)
    wm8904Index.setDefaultValue(0)

    wm8904Use = wm8904Component.createBooleanSymbol("USE_DRV_CODEC_WM8904", None)
    wm8904Use.setVisible(False)
    wm8904Use.setDefaultValue(True)

    wm8904Idx0 = wm8904Component.createBooleanSymbol("DRV_CODEC_WM8904_INST_IDX0", None)
    wm8904Idx0.setVisible(False)
    wm8904Idx0.setDefaultValue(True)

    wm8904I2CDriver = wm8904Component.createStringSymbol("DRV_WM8904_I2C", None)
    wm8904I2CDriver.setVisible(True)
    wm8904I2CDriver.setLabel("I2C Driver used")
    wm8904I2CDriver.setReadOnly(True)
    wm8904I2CDriver.setDefaultValue("I2C")

    wm8904I2SDriver = wm8904Component.createStringSymbol("DRV_WM8904_I2S", None)
    wm8904I2SDriver.setVisible(True)
    wm8904I2SDriver.setLabel("I2S Driver used")
    wm8904I2SDriver.setReadOnly(True)
    wm8904I2SDriver.setDefaultValue("I2S")

    wm8904Mode = wm8904Component.createKeyValueSetSymbol("DRV_WM8904_MASTER_MODE", None)
    wm8904Mode.setVisible(True)
    wm8904Mode.setLabel("Usage Mode")
    wm8904Mode.addKey("SLAVE", "0", "Slave")
    wm8904Mode.addKey("MASTER", "1", "Master")
    wm8904Mode.setDisplayMode("Description")
    wm8904Mode.setOutputMode("Key")
    wm8904Mode.setDefaultValue(1)
    wm8904Mode.setDependencies(customUpdate, ["DRV_WM8904_MASTER_MODE"])

    wm8904Clients = wm8904Component.createIntegerSymbol("DRV_WM8904_CLIENTS_NUMBER", None)
    wm8904Clients.setVisible(True)
    wm8904Clients.setLabel("Number of WM8904 Driver Clients")
    wm8904Clients.setReadOnly(True)
    wm8904Clients.setDefaultValue(1)

    wm8904Baud = wm8904Component.createIntegerSymbol("DRV_WM8904_BAUD_RATE", None)
    wm8904Baud.setVisible(custom)
    wm8904Baud.setLabel("Sampling Rate")
    wm8904Baud.setDefaultValue(48000)

    wm8904Volume = wm8904Component.createIntegerSymbol("DRV_WM8904_VOLUME", None)
    wm8904Volume.setVisible(False)
    wm8904Volume.setLabel("Volume for the Codec in range 0(Min) to 255(Max)")
    wm8904Volume.setDefaultValue(200)

    wm8904Format = wm8904Component.createKeyValueSetSymbol("DRV_WM8904_AUDIO_DATA_FORMAT", None)
    wm8904Format.setVisible(True)
    wm8904Format.setLabel("Audio Data Format")
    wm8904Format.addKey("DATA_16_BIT_LEFT_JUSTIFIED", "0", "16-bit Left Justified")
    wm8904Format.addKey("DATA_16_BIT_I2S", "1", "16-bit I2S")
    wm8904Format.addKey("DATA_32_BIT_LEFT_JUSTIFIED", "2", "32-bit Left Justified")
    wm8904Format.addKey("DATA_32_BIT_I2S", "3", "32-bit I2S")
    wm8904Format.setDisplayMode("Description")
    wm8904Format.setOutputMode("Key")
    wm8904Format.setDefaultValue(1)

    wm8904FormatComment = wm8904Component.createCommentSymbol("AUDIO_DATA_FORMAT_COMMENT", None)
    wm8904FormatComment.setVisible(True)
    wm8904FormatComment.setLabel("Must match Audio Protocol and Data Length field in I2SC/SSC PLIB")") 

    wm8904EnableMic = wm8904Component.createBooleanSymbol("DRV_WM8904_ENABLE_MIC_INPUT", None)
    wm8904EnableMic.setVisible(True)
    wm8904EnableMic.setLabel("Enable Microphone Input")
    wm8904EnableMic.setDefaultValue(False)

    # Enable "Generate Harmony Application Files" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_APP_FILE", True, 1)

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_DRV_COMMON", True, 1)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_SYS_COMMON", True, 1)

    # Enable "Enable System Interrupt" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_SYS_INT", True, 1)

    # Enable "Enable System Ports" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_SYS_PORTS", True, 1)

    # Enable "Enable System DMA" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_SYS_DMA", True, 1)

    # Enable "Enable OSAL" option in MHC
    Database.setSymbolValue("Harmony", "ENABLE_OSAL", True, 1)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")  # e.g. "default"
    
    wm8904SymHeaderFile = wm8904Component.createFileSymbol("DRV_WM8904_HEADER", None)
    wm8904SymHeaderFile.setSourcePath("drv_wm8904.h")
    wm8904SymHeaderFile.setOutputName("drv_wm8904.h")
    wm8904SymHeaderFile.setDestPath("driver/wm8904/")
    wm8904SymHeaderFile.setProjectPath("config/" + configName + "/driver/wm8904/")
    wm8904SymHeaderFile.setType("HEADER")
    wm8904SymHeaderFile.setOverwrite(True)
    
#    wm8904SymHeaderDefFile = wm8904Component.createFileSymbol("DRV_WM8904_DEF", None)
#    wm8904SymHeaderDefFile.setSourcePath("drv_wm8904_definitions.h")
#    wm8904SymHeaderDefFile.setOutputName("drv_wm8904_definitions.h")
#    wm8904SymHeaderDefFile.setDestPath("")
#    wm8904SymHeaderDefFile.setProjectPath("config/" + configName + "/")
#    wm8904SymHeaderDefFile.setType("HEADER")
#    wm8904SymHeaderDefFile.setOverwrite(True)

    wm8904SymSourceFile = wm8904Component.createFileSymbol("DRV_WM8904_SOURCE", None)
    wm8904SymSourceFile.setSourcePath("src/drv_wm8904.c")
    wm8904SymSourceFile.setOutputName("drv_wm8904.c")
    wm8904SymSourceFile.setDestPath("driver/wm8904/")
    wm8904SymSourceFile.setProjectPath("config/" + configName + "/driver/wm8904/")
    wm8904SymSourceFile.setType("SOURCE")
    wm8904SymSourceFile.setOverwrite(True)

    wm8904SymHeaderLocalFile = wm8904Component.createFileSymbol("DRV_WM8904_HEADER_LOCAL", None)
    wm8904SymHeaderLocalFile.setSourcePath("src/drv_wm8904_local.h")
    wm8904SymHeaderLocalFile.setOutputName("drv_wm8904_local.h")
    wm8904SymHeaderLocalFile.setDestPath("driver/wm8904/")
    wm8904SymHeaderLocalFile.setProjectPath("config/" + configName + "/driver/wm8904/")
    wm8904SymHeaderLocalFile.setType("SOURCE")
    wm8904SymHeaderLocalFile.setOverwrite(True)
    
    wm8904SymSystemDefIncFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_DEF", None)
    wm8904SymSystemDefIncFile.setType("STRING")
    wm8904SymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    wm8904SymSystemDefIncFile.setSourcePath("templates/system/system_definitions.h.ftl")
    wm8904SymSystemDefIncFile.setMarkup(True)
    
    wm8904SymSystemDefObjFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_DEF_OBJECT", None)
    wm8904SymSystemDefObjFile.setType("STRING")
    wm8904SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    wm8904SymSystemDefObjFile.setSourcePath("templates/system/system_definitions_objects.h.ftl")
    wm8904SymSystemDefObjFile.setMarkup(True)

    wm8904SymSystemConfigFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_CONFIG", None)
    wm8904SymSystemConfigFile.setType("STRING")
    wm8904SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    wm8904SymSystemConfigFile.setSourcePath("templates/system/system_config.h.ftl")
    wm8904SymSystemConfigFile.setMarkup(True)

    wm8904SymSystemInitDataFile = wm8904Component.createFileSymbol("DRV_WM8904_INIT_DATA", None)
    wm8904SymSystemInitDataFile.setType("STRING")
    wm8904SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    wm8904SymSystemInitDataFile.setSourcePath("templates/system/system_initialize_data.c.ftl")
    wm8904SymSystemInitDataFile.setMarkup(True)

    wm8904SymSystemInitFile = wm8904Component.createFileSymbol("DRV_WM8904_SYS_INIT", None)
    wm8904SymSystemInitFile.setType("STRING")
    wm8904SymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")  
    wm8904SymSystemInitFile.setSourcePath("templates/system/system_initialize.c.ftl")
    wm8904SymSystemInitFile.setMarkup(True)

    wm8904SystemTaskFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_TASKS_C", None)
    wm8904SystemTaskFile.setType("STRING")
    wm8904SystemTaskFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    wm8904SystemTaskFile.setSourcePath("templates/system/system_tasks.c.ftl")
    wm8904SystemTaskFile.setMarkup(True)

# this callback occurs when user connects I2C or I2S driver to WM8904 driver block in Project Graph    
def onDependencyConnected(info):
    global i2sPlibId
    if info["dependencyID"] == "I2S driver":
        plibUsed = info["localComponent"].getSymbolByID("DRV_WM8904_I2S")
    elif info["dependencyID"] == "I2C driver":
        plibUsed = info["localComponent"].getSymbolByID("DRV_WM8904_I2C")
    i2sOri2cId = info["remoteComponent"].getID().upper()
    plibUsed.setValue(i2sOri2cId, 1)