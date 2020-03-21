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

def customUpdate(ak4954WhichMic, event):
    global ak4954EnableBias 
    global custom
    global custom2


    if event["value"]>=1:   # Enable Mic Gain for external or line in
        custom = True
    else:
        custom = False

    print(event["value"])
    if event["value"]==1:   # Enable Mic Bias for external only
        custom2 = True
    else:
        custom2 = False

    ak4954MicGain.setVisible(custom)
    ak4954EnableBias.setVisible(custom2)

def instantiateComponent(ak4954Component):
    global custom
    global custom2
    global ak4954EnableBias
    global ak4954MicGain 

    Log.writeInfoMessage("AK4954 instantiated")

    custom = False
    custom2 = False
    ak4954Index = ak4954Component.createIntegerSymbol("WM8904_INDEX", None)
    ak4954Index.setVisible(False)
    ak4954Index.setDefaultValue(0)

    ak4954Use = ak4954Component.createBooleanSymbol("USE_DRV_CODEC_AK4954", None)
    ak4954Use.setVisible(False)
    ak4954Use.setDefaultValue(True)

    ak4954Idx0 = ak4954Component.createBooleanSymbol("DRV_CODEC_AK4954_INST_IDX0", None)
    ak4954Idx0.setVisible(False)
    ak4954Idx0.setDefaultValue(True)

    ak4954I2CDriver = ak4954Component.createStringSymbol("DRV_AK4954_I2C", None)
    ak4954I2CDriver.setVisible(True)
    ak4954I2CDriver.setLabel("I2C Driver used")
    ak4954I2CDriver.setReadOnly(True)
    ak4954I2CDriver.setDefaultValue("I2C")

    ak4954I2SDriver = ak4954Component.createStringSymbol("DRV_AK4954_I2S", None)
    ak4954I2SDriver.setVisible(True)
    ak4954I2SDriver.setLabel("I2S Driver used")
    ak4954I2SDriver.setReadOnly(True)
    ak4954I2SDriver.setDefaultValue("I2S")

    ak4954I2SDriverIndex = ak4954Component.createStringSymbol("DRV_AK4954_I2S_INDEX", None)
    ak4954I2SDriverIndex.setVisible(False)
    ak4954I2SDriverIndex.setReadOnly(True)
    ak4954I2SDriverIndex.setDefaultValue("DRV_I2S_INDEX")

    ak4954I2CDriverIndex = ak4954Component.createStringSymbol("DRV_AK4954_I2C_INDEX", None)
    ak4954I2CDriverIndex.setVisible(False)
    ak4954I2CDriverIndex.setReadOnly(True)
    ak4954I2CDriverIndex.setDefaultValue("DRV_I2C_INDEX")

    ak4954Mode = ak4954Component.createKeyValueSetSymbol("DRV_AK4954_MASTER_MODE", None)
    ak4954Mode.setVisible(True)
    ak4954Mode.setLabel("Usage Mode")
    ak4954Mode.addKey("SLAVE", "0", "Slave")
    ak4954Mode.addKey("MASTER", "1", "Master")
    ak4954Mode.setDisplayMode("Description")
    ak4954Mode.setOutputMode("Key")
    ak4954Mode.setDefaultValue(0)
    ak4954Mode.setReadOnly(True)        # for this version, only slave mode is supported

    ak4954FormatComment = ak4954Component.createCommentSymbol("DRV_AK4954_MASTER_MODE_COMMENT", None)
    ak4954FormatComment.setVisible(True)
    ak4954FormatComment.setLabel("I2SC/SSC/I2S PLIB should be placed in Master Mode") 

    ak4954Clients = ak4954Component.createIntegerSymbol("DRV_AK4954_CLIENTS_NUMBER", None)
    ak4954Clients.setVisible(True)
    ak4954Clients.setLabel("Number of AK4954 Driver Clients")
    ak4954Clients.setReadOnly(True)
    ak4954Clients.setDefaultValue(1)

    ak4954Baud = ak4954Component.createIntegerSymbol("DRV_AK4954_BAUD_RATE", None)
    ak4954Baud.setVisible(True)
    ak4954Baud.setLabel("Sampling Rate")
    ak4954Baud.setDefaultValue(48000)

    ak4954Volume = ak4954Component.createIntegerSymbol("DRV_AK4954_VOLUME", None)
    ak4954Volume.setVisible(True)
    ak4954Volume.setLabel("Volume for the Codec in range 0(Min) to 255(Max)")
    ak4954Volume.setDefaultValue(200)

    ak4954DelayInit = ak4954Component.createBooleanSymbol("DRV_AK4954_DELAY_INITIALIZATION", None)
    ak4954DelayInit.setVisible(True)
    ak4954DelayInit.setDefaultValue(False)
    ak4954DelayInit.setLabel("Delay driver initialization (due to shared RESET pin)")

    ak4954Format = ak4954Component.createKeyValueSetSymbol("DRV_AK4954_AUDIO_DATA_FORMAT", None)
    ak4954Format.setVisible(True)
    ak4954Format.setLabel("Audio Data Format")
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_24BIT_LSB_SDTI", "0", "24-bit Left-justified(ADC), 24-bit Right-justified(DAC)")
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_16BIT_LSB_SDTI", "1", "24-bit Left-justified(ADC), 16--bit Right-justified(DAC)")
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_24BIT_MSB_SDTI", "2", "24-bit Left-justified(ADC), 24-bit Left-justified(DAC)")
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_I2S_16BIT_24BIT", "3", "24/16-bit I2S")          
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_32BIT_MSB_SDTO_32BIT_MSB_SDTI", "6", "32-bit Left-justified(ADC), 32-bit Left-justified(DAC)")   
    ak4954Format.addKey("DRV_AK4954_AUDIO_DATA_FORMAT_I2S_32BIT", "7", "32-bit I2S")   
    ak4954Format.setDisplayMode("Description")
    ak4954Format.setOutputMode("Key")
    ak4954Format.setDefaultValue(3)

    ak4954FormatComment = ak4954Component.createCommentSymbol("AUDIO_DATA_FORMAT_COMMENT", None)
    ak4954FormatComment.setVisible(True)
    ak4954FormatComment.setLabel("Must match Audio Protocol and Data Length field in I2SC/SSC/I2S PLIB") 

    ak4954WhichMic = ak4954Component.createKeyValueSetSymbol("DRV_AK4954_WHICH_MIC_INPUT", None)
    ak4954WhichMic.setVisible(True)
    ak4954WhichMic.setLabel("Microphone/Line Input")
    ak4954WhichMic.addKey("MIC1", "0", "Internal Mic (on board)")
    ak4954WhichMic.addKey("MIC2", "1", "External Mic Input")
    ak4954WhichMic.addKey("MIC3", "2", "Line Input")
    ak4954WhichMic.setDisplayMode("Description")
    ak4954WhichMic.setOutputMode("Key")
    ak4954WhichMic.setDefaultValue(0)
    ak4954WhichMic.setDependencies(customUpdate, ["DRV_AK4954_WHICH_MIC_INPUT"])

    ak4954MicGain = ak4954Component.createIntegerSymbol("DRV_AK4954_MIC_GAIN", None)
    ak4954MicGain.setVisible(custom)
    ak4954MicGain.setLabel("Ext Mic Gain in dB range 0(Min) to 31(Max)")
    ak4954MicGain.setDefaultValue(20)

    ak4954EnableBias = ak4954Component.createBooleanSymbol("DRV_AK4954_ENABLE_MIC_BIAS", None)
    ak4954EnableBias.setVisible(custom2)
    ak4954EnableBias.setLabel("Enable Microphone Bias for electret microphones")
    ak4954EnableBias.setDefaultValue(True)

    # Enable "Generate Harmony Application Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_APP_FILE", True, 1)

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 1)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 1)

    # Enable "Enable System Interrupt" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_INT", True, 1)

    # Enable "Enable System Ports" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_PORTS", True, 1)

    # Enable "Enable System DMA" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_DMA", True, 1)

    # Enable "Enable OSAL" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_OSAL", True, 1)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")  # e.g. "default"
    
    ak4954SymHeaderFile = ak4954Component.createFileSymbol("DRV_AK4954_HEADER", None)
    ak4954SymHeaderFile.setSourcePath("codec/AK4954/drv_ak4954.h")
    ak4954SymHeaderFile.setOutputName("drv_ak4954.h")
    ak4954SymHeaderFile.setDestPath("audio/driver/codec/ak4954/")
    ak4954SymHeaderFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4954/")
    ak4954SymHeaderFile.setType("HEADER")
    ak4954SymHeaderFile.setOverwrite(True)
    
    ak4954SymSourceFile = ak4954Component.createFileSymbol("DRV_AK4954_SOURCE", None)
    ak4954SymSourceFile.setMarkup(True) 
    ak4954SymSourceFile.setSourcePath("codec/AK4954/templates/drv_ak4954.c.ftl")
    ak4954SymSourceFile.setOutputName("drv_ak4954.c")
    ak4954SymSourceFile.setDestPath("audio/driver/codec/ak4954/")
    ak4954SymSourceFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4954/")
    ak4954SymSourceFile.setType("SOURCE")
    ak4954SymSourceFile.setOverwrite(True)

    ak4954SymHeaderLocalFile = ak4954Component.createFileSymbol("DRV_AK4954_HEADER_LOCAL", None)
    ak4954SymHeaderLocalFile.setMarkup(True) 
    ak4954SymHeaderLocalFile.setSourcePath("codec/AK4954/templates/drv_ak4954_local.h.ftl")
    ak4954SymHeaderLocalFile.setOutputName("drv_ak4954_local.h")
    ak4954SymHeaderLocalFile.setDestPath("audio/driver/codec/ak4954/")
    ak4954SymHeaderLocalFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4954/")
    ak4954SymHeaderLocalFile.setType("SOURCE")
    ak4954SymHeaderLocalFile.setOverwrite(True)
    
    ak4954SymSystemDefIncFile = ak4954Component.createFileSymbol("DRV_AK4954_SYSTEM_DEF", None)
    ak4954SymSystemDefIncFile.setType("STRING")
    ak4954SymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    ak4954SymSystemDefIncFile.setSourcePath("codec/AK4954/templates/system/system_definitions.h.ftl")
    ak4954SymSystemDefIncFile.setMarkup(True)
    
    ak4954SymSystemDefObjFile = ak4954Component.createFileSymbol("DRV_AK4954_SYSTEM_DEF_OBJECT", None)
    ak4954SymSystemDefObjFile.setType("STRING")
    ak4954SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    ak4954SymSystemDefObjFile.setSourcePath("codec/AK4954/templates/system/system_definitions_objects.h.ftl")
    ak4954SymSystemDefObjFile.setMarkup(True)

    ak4954SymSystemConfigFile = ak4954Component.createFileSymbol("DRV_AK4954_SYSTEM_CONFIG", None)
    ak4954SymSystemConfigFile.setType("STRING")
    ak4954SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    ak4954SymSystemConfigFile.setSourcePath("codec/AK4954/templates/system/system_config.h.ftl")
    ak4954SymSystemConfigFile.setMarkup(True)

    ak4954SymSystemInitDataFile = ak4954Component.createFileSymbol("DRV_AK4954_INIT_DATA", None)
    ak4954SymSystemInitDataFile.setType("STRING")
    ak4954SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    ak4954SymSystemInitDataFile.setSourcePath("codec/AK4954/templates/system/system_initialize_data.c.ftl")
    ak4954SymSystemInitDataFile.setMarkup(True)

    ak4954SymSystemInitFile = ak4954Component.createFileSymbol("DRV_AK4954_SYS_INIT", None)
    ak4954SymSystemInitFile.setType("STRING")
    ak4954SymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")  
    ak4954SymSystemInitFile.setSourcePath("codec/AK4954/templates/system/system_initialize.c.ftl")
    ak4954SymSystemInitFile.setMarkup(True)

    ak4954SystemTaskFile = ak4954Component.createFileSymbol("DRV_AK4954_SYSTEM_TASKS_C", None)
    ak4954SystemTaskFile.setType("STRING")
    ak4954SystemTaskFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    ak4954SystemTaskFile.setSourcePath("codec/AK4954/templates/system/system_tasks.c.ftl")
    ak4954SystemTaskFile.setMarkup(True)

# this callback occurs when user connects I2C or I2S driver to AK4954 driver block in Project Graph    
def onDependencyConnected(info):
    if info["dependencyID"] == "DRV_I2S":
        drvUsed = info["localComponent"].getSymbolByID("DRV_AK4954_I2S")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        i2sOri2cId = i2sOri2cId.replace("A_","")    # I2S driver in audio repo have an "a_" prefix
        drvIndexUsed = info["localComponent"].getSymbolByID("DRV_AK4954_I2S_INDEX")
        i2sOri2cIndex = i2sOri2cId.replace("I2S_","I2S_INDEX_")    # DRV_I2S_1 => DRV_I2S_INDEX_1
    elif info["dependencyID"] == "DRV_I2C":
        drvUsed = info["localComponent"].getSymbolByID("DRV_AK4954_I2C")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        drvIndexUsed = info["localComponent"].getSymbolByID("DRV_AK4954_I2C_INDEX")
        i2sOri2cIndex = i2sOri2cId.replace("I2C_","I2C_INDEX_")    # DRV_I2C_0 => DRV_I2C_INDEX_0   
    drvUsed.setValue(i2sOri2cId, 1)
    drvIndexUsed.setValue(i2sOri2cIndex, 1)
