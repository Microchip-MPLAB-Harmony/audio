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

def customUpdate(ak4953WhichMic, event):
    global ak4953EnableBias 
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

    ak4953MicGain.setVisible(custom)
    ak4953EnableBias.setVisible(custom2)

def instantiateComponent(ak4953Component):
    global custom
    global custom2
    global ak4953EnableBias
    global ak4953MicGain 

    Log.writeInfoMessage("AK4953 instantiated")

    custom = False
    custom2 = False
    ak4953Index = ak4953Component.createIntegerSymbol("WM8904_INDEX", None)
    ak4953Index.setVisible(False)
    ak4953Index.setDefaultValue(0)

    ak4953Use = ak4953Component.createBooleanSymbol("USE_DRV_CODEC_AK4953", None)
    ak4953Use.setVisible(False)
    ak4953Use.setDefaultValue(True)

    ak4953Idx0 = ak4953Component.createBooleanSymbol("DRV_CODEC_AK4953_INST_IDX0", None)
    ak4953Idx0.setVisible(False)
    ak4953Idx0.setDefaultValue(True)

    ak4953I2CDriver = ak4953Component.createStringSymbol("DRV_AK4953_I2C", None)
    ak4953I2CDriver.setVisible(True)
    ak4953I2CDriver.setLabel("I2C Driver used")
    ak4953I2CDriver.setReadOnly(True)
    ak4953I2CDriver.setDefaultValue("I2C")

    ak4953I2SDriver = ak4953Component.createStringSymbol("DRV_AK4953_I2S", None)
    ak4953I2SDriver.setVisible(True)
    ak4953I2SDriver.setLabel("I2S Driver used")
    ak4953I2SDriver.setReadOnly(True)
    ak4953I2SDriver.setDefaultValue("I2S")

    ak4953I2SDriverIndex = ak4953Component.createStringSymbol("DRV_AK4953_I2S_INDEX", None)
    ak4953I2SDriverIndex.setVisible(False)
    ak4953I2SDriverIndex.setReadOnly(True)
    ak4953I2SDriverIndex.setDefaultValue("DRV_I2S_INDEX")

    ak4953I2CDriverIndex = ak4953Component.createStringSymbol("DRV_AK4953_I2C_INDEX", None)
    ak4953I2CDriverIndex.setVisible(False)
    ak4953I2CDriverIndex.setReadOnly(True)
    ak4953I2CDriverIndex.setDefaultValue("DRV_I2C_INDEX")

    ak4953Mode = ak4953Component.createKeyValueSetSymbol("DRV_AK4953_MASTER_MODE", None)
    ak4953Mode.setVisible(True)
    ak4953Mode.setLabel("Usage Mode")
    ak4953Mode.addKey("SLAVE", "0", "Slave")
    ak4953Mode.addKey("MASTER", "1", "Master")
    ak4953Mode.setDisplayMode("Description")
    ak4953Mode.setOutputMode("Key")
    ak4953Mode.setDefaultValue(0)
    ak4953Mode.setReadOnly(True)        # for this version, only slave mode is supported

    ak4953FormatComment = ak4953Component.createCommentSymbol("DRV_AK4953_MASTER_MODE_COMMENT", None)
    ak4953FormatComment.setVisible(True)
    ak4953FormatComment.setLabel("I2SC/SSC/I2S PLIB should be placed in Master Mode") 

    ak4953Clients = ak4953Component.createIntegerSymbol("DRV_AK4953_CLIENTS_NUMBER", None)
    ak4953Clients.setVisible(True)
    ak4953Clients.setLabel("Number of AK4953 Driver Clients")
    ak4953Clients.setReadOnly(True)
    ak4953Clients.setDefaultValue(1)

    ak4953Baud = ak4953Component.createIntegerSymbol("DRV_AK4953_BAUD_RATE", None)
    ak4953Baud.setVisible(True)
    ak4953Baud.setLabel("Sampling Rate")
    ak4953Baud.setDefaultValue(48000)

    ak4953Volume = ak4953Component.createIntegerSymbol("DRV_AK4953_VOLUME", None)
    ak4953Volume.setVisible(True)
    ak4953Volume.setLabel("Volume for the Codec in range 0(Min) to 255(Max)")
    ak4953Volume.setDefaultValue(200)

    ak4953DelayInit = ak4953Component.createBooleanSymbol("DRV_AK4953_DELAY_INITIALIZATION", None)
    ak4953DelayInit.setVisible(False)
    ak4953DelayInit.setDefaultValue(False)
    ak4953DelayInit.setLabel("Delay driver initialization (due to shared RESET pin)")

    ak4953Format = ak4953Component.createKeyValueSetSymbol("DRV_AK4953_AUDIO_DATA_FORMAT", None)
    ak4953Format.setVisible(True)
    ak4953Format.setLabel("Audio Data Format")
    ak4953Format.addKey("DRV_AK4953_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_24BIT_LSB_SDTI", "0", "24-bit Left-justified(ADC), 24-bit Right-justified(DAC)")
    ak4953Format.addKey("DRV_AK4953_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_16BIT_LSB_SDTI", "1", "24-bit Left-justified(ADC), 16--bit Right-justified(DAC)")
    ak4953Format.addKey("DRV_AK4953_AUDIO_DATA_FORMAT_24BIT_MSB_SDTO_24BIT_MSB_SDTI", "2", "24-bit Left-justified(ADC), 24-bit Left-justified(DAC)")
    ak4953Format.addKey("DRV_AK4953_AUDIO_DATA_FORMAT_I2S_16BIT_24BIT", "3", "24/16-bit I2S")
    ak4953Format.setDisplayMode("Description")
    ak4953Format.setOutputMode("Key")
    ak4953Format.setDefaultValue(3)

    ak4953FormatComment = ak4953Component.createCommentSymbol("AUDIO_DATA_FORMAT_COMMENT", None)
    ak4953FormatComment.setVisible(True)
    ak4953FormatComment.setLabel("Must match Audio Protocol and Data Length field in I2SC/SSC/I2S PLIB") 

    ak4953WhichMic = ak4953Component.createKeyValueSetSymbol("DRV_AK4953_WHICH_MIC_INPUT", None)
    ak4953WhichMic.setVisible(True)
    ak4953WhichMic.setLabel("Microphone/Line Input")
    ak4953WhichMic.addKey("MIC1", "0", "Internal Mic (on board)")
    ak4953WhichMic.addKey("MIC2", "1", "External Mic Input")
    ak4953WhichMic.addKey("MIC3", "2", "Line Input")
    ak4953WhichMic.setDisplayMode("Description")
    ak4953WhichMic.setOutputMode("Key")
    ak4953WhichMic.setDefaultValue(0)
    ak4953WhichMic.setDependencies(customUpdate, ["DRV_AK4953_WHICH_MIC_INPUT"])

    ak4953MicGain = ak4953Component.createIntegerSymbol("DRV_AK4953_MIC_GAIN", None)
    ak4953MicGain.setVisible(custom)
    ak4953MicGain.setLabel("Ext Mic Gain in dB range 0(Min) to 31(Max)")
    ak4953MicGain.setDefaultValue(20)

    ak4953EnableBias = ak4953Component.createBooleanSymbol("DRV_AK4953_ENABLE_MIC_BIAS", None)
    ak4953EnableBias.setVisible(custom2)
    ak4953EnableBias.setLabel("Enable Microphone Bias for electret microphones")
    ak4953EnableBias.setDefaultValue(True)

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
    
    ak4953SymHeaderFile = ak4953Component.createFileSymbol("DRV_AK4953_HEADER", None)
    ak4953SymHeaderFile.setSourcePath("codec/AK4953/drv_ak4953.h")
    ak4953SymHeaderFile.setOutputName("drv_ak4953.h")
    ak4953SymHeaderFile.setDestPath("audio/driver/codec/ak4953/")
    ak4953SymHeaderFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4953/")
    ak4953SymHeaderFile.setType("HEADER")
    ak4953SymHeaderFile.setOverwrite(True)
    
    ak4953SymSourceFile = ak4953Component.createFileSymbol("DRV_AK4953_SOURCE", None)
    ak4953SymSourceFile.setMarkup(True) 
    ak4953SymSourceFile.setSourcePath("codec/AK4953/templates/drv_ak4953.c.ftl")
    ak4953SymSourceFile.setOutputName("drv_ak4953.c")
    ak4953SymSourceFile.setDestPath("audio/driver/codec/ak4953/")
    ak4953SymSourceFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4953/")
    ak4953SymSourceFile.setType("SOURCE")
    ak4953SymSourceFile.setOverwrite(True)

    ak4953SymHeaderLocalFile = ak4953Component.createFileSymbol("DRV_AK4953_HEADER_LOCAL", None)
    ak4953SymHeaderLocalFile.setSourcePath("codec/AK4953/src/drv_ak4953_local.h")
    ak4953SymHeaderLocalFile.setOutputName("drv_ak4953_local.h")
    ak4953SymHeaderLocalFile.setDestPath("audio/driver/codec/ak4953/")
    ak4953SymHeaderLocalFile.setProjectPath("config/" + configName + "/audio/driver/codec/ak4953/")
    ak4953SymHeaderLocalFile.setType("SOURCE")
    ak4953SymHeaderLocalFile.setOverwrite(True)
    
    ak4953SymSystemDefIncFile = ak4953Component.createFileSymbol("DRV_AK4953_SYSTEM_DEF", None)
    ak4953SymSystemDefIncFile.setType("STRING")
    ak4953SymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    ak4953SymSystemDefIncFile.setSourcePath("codec/AK4953/templates/system/system_definitions.h.ftl")
    ak4953SymSystemDefIncFile.setMarkup(True)
    
    ak4953SymSystemDefObjFile = ak4953Component.createFileSymbol("DRV_AK4953_SYSTEM_DEF_OBJECT", None)
    ak4953SymSystemDefObjFile.setType("STRING")
    ak4953SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    ak4953SymSystemDefObjFile.setSourcePath("codec/AK4953/templates/system/system_definitions_objects.h.ftl")
    ak4953SymSystemDefObjFile.setMarkup(True)

    ak4953SymSystemConfigFile = ak4953Component.createFileSymbol("DRV_AK4953_SYSTEM_CONFIG", None)
    ak4953SymSystemConfigFile.setType("STRING")
    ak4953SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    ak4953SymSystemConfigFile.setSourcePath("codec/AK4953/templates/system/system_config.h.ftl")
    ak4953SymSystemConfigFile.setMarkup(True)

    ak4953SymSystemInitDataFile = ak4953Component.createFileSymbol("DRV_AK4953_INIT_DATA", None)
    ak4953SymSystemInitDataFile.setType("STRING")
    ak4953SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    ak4953SymSystemInitDataFile.setSourcePath("codec/AK4953/templates/system/system_initialize_data.c.ftl")
    ak4953SymSystemInitDataFile.setMarkup(True)

    ak4953SymSystemInitFile = ak4953Component.createFileSymbol("DRV_AK4953_SYS_INIT", None)
    ak4953SymSystemInitFile.setType("STRING")
    ak4953SymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")  
    ak4953SymSystemInitFile.setSourcePath("codec/AK4953/templates/system/system_initialize.c.ftl")
    ak4953SymSystemInitFile.setMarkup(True)

    ak4953SystemTaskFile = ak4953Component.createFileSymbol("DRV_AK4953_SYSTEM_TASKS_C", None)
    ak4953SystemTaskFile.setType("STRING")
    ak4953SystemTaskFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    ak4953SystemTaskFile.setSourcePath("codec/AK4953/templates/system/system_tasks.c.ftl")
    ak4953SystemTaskFile.setMarkup(True)

# this callback occurs when user connects I2C or I2S driver to AK4953 driver block in Project Graph    
def onDependencyConnected(info):
    if info["dependencyID"] == "DRV_I2S":
        drvUsed = info["localComponent"].getSymbolByID("DRV_AK4953_I2S")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        i2sOri2cId = i2sOri2cId.replace("A_","")    # I2S driver in audio repo have an "a_" prefix
        drvIndexUsed = info["localComponent"].getSymbolByID("DRV_AK4953_I2S_INDEX")
        i2sOri2cIndex = i2sOri2cId.replace("I2S_","I2S_INDEX_")    # DRV_I2S_1 => DRV_I2S_INDEX_1
    elif info["dependencyID"] == "DRV_I2C":
        drvUsed = info["localComponent"].getSymbolByID("DRV_AK4953_I2C")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        drvIndexUsed = info["localComponent"].getSymbolByID("DRV_AK4953_I2C_INDEX")
        i2sOri2cIndex = i2sOri2cId.replace("I2C_","I2C_INDEX_")    # DRV_I2C_0 => DRV_I2C_INDEX_0   
    drvUsed.setValue(i2sOri2cId, 1)
    drvIndexUsed.setValue(i2sOri2cIndex, 1)
