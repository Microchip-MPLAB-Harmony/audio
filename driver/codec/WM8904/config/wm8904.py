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

def customUpdate(wm8904EnableMic, event):
    global wm8904EnableBias 
    global wm8904MicGain
    global custom

    if event["value"]==True:   # Enable Mic Input
        custom = True
    else:
        custom = False

    wm8904EnableBias.setVisible(custom) 
    wm8904MicGain.setVisible(custom)

def instantiateComponent(wm8904Component):
    global custom
    global wm8904EnableBias
    global wm8904MicGain 

    Log.writeInfoMessage("WM8904 instantiated")

    custom = False
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

    wm8904Clients = wm8904Component.createIntegerSymbol("DRV_WM8904_CLIENTS_NUMBER", None)
    wm8904Clients.setVisible(True)
    wm8904Clients.setLabel("Number of WM8904 Driver Clients")
    wm8904Clients.setReadOnly(True)
    wm8904Clients.setDefaultValue(1)

    wm8904Baud = wm8904Component.createIntegerSymbol("DRV_WM8904_BAUD_RATE", None)
    wm8904Baud.setVisible(True)
    wm8904Baud.setLabel("Sampling Rate")
    wm8904Baud.setDefaultValue(48000)

    wm8904Volume = wm8904Component.createIntegerSymbol("DRV_WM8904_VOLUME", None)
    wm8904Volume.setVisible(True)
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
    wm8904FormatComment.setLabel("Must match Audio Protocol and Data Length field in I2SC/SSC/I2S PLIB") 

    wm8904EnableMic = wm8904Component.createBooleanSymbol("DRV_WM8904_ENABLE_MIC_INPUT", None)
    wm8904EnableMic.setVisible(True)
    wm8904EnableMic.setLabel("Enable Microphone Input")
    wm8904EnableMic.setDefaultValue(False)
    wm8904EnableMic.setDependencies(customUpdate, ["DRV_WM8904_ENABLE_MIC_INPUT"])

    wm8904EnableBias = wm8904Component.createBooleanSymbol("DRV_WM8904_ENABLE_MIC_BIAS", None)
    wm8904EnableBias.setVisible(custom)
    wm8904EnableBias.setLabel("Enable Microphone Bias for electret microphones")
    wm8904EnableBias.setDefaultValue(False)

    wm8904MicGain = wm8904Component.createIntegerSymbol("DRV_WM8904_MIC_GAIN", None)
    wm8904MicGain.setVisible(custom)
    wm8904MicGain.setLabel("Microphone Gain in range 0(Min) to 31(Max)")
    wm8904MicGain.setDefaultValue(28)

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
    
    wm8904SymHeaderFile = wm8904Component.createFileSymbol("DRV_WM8904_HEADER", None)
    wm8904SymHeaderFile.setSourcePath("codec/WM8904/drv_wm8904.h")
    wm8904SymHeaderFile.setOutputName("drv_wm8904.h")
    wm8904SymHeaderFile.setDestPath("audio/driver/codec/wm8904/")
    wm8904SymHeaderFile.setProjectPath("config/" + configName + "/audio/driver/codec/wm8904/")
    wm8904SymHeaderFile.setType("HEADER")
    wm8904SymHeaderFile.setOverwrite(True)
    
    wm8904SymSourceFile = wm8904Component.createFileSymbol("DRV_WM8904_SOURCE", None)
    wm8904SymSourceFile.setSourcePath("codec/WM8904/src/drv_wm8904.c")
    wm8904SymSourceFile.setOutputName("drv_wm8904.c")
    wm8904SymSourceFile.setDestPath("audio/driver/codec/wm8904/")
    wm8904SymSourceFile.setProjectPath("config/" + configName + "/audio/driver/codec/wm8904/")
    wm8904SymSourceFile.setType("SOURCE")
    wm8904SymSourceFile.setOverwrite(True)

    wm8904SymHeaderLocalFile = wm8904Component.createFileSymbol("DRV_WM8904_HEADER_LOCAL", None)
    wm8904SymHeaderLocalFile.setSourcePath("codec/WM8904/src/drv_wm8904_local.h")
    wm8904SymHeaderLocalFile.setOutputName("drv_wm8904_local.h")
    wm8904SymHeaderLocalFile.setDestPath("audio/driver/codec/wm8904/")
    wm8904SymHeaderLocalFile.setProjectPath("config/" + configName + "/audio/driver/codec/wm8904/")
    wm8904SymHeaderLocalFile.setType("SOURCE")
    wm8904SymHeaderLocalFile.setOverwrite(True)
    
    wm8904SymSystemDefIncFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_DEF", None)
    wm8904SymSystemDefIncFile.setType("STRING")
    wm8904SymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    wm8904SymSystemDefIncFile.setSourcePath("codec/WM8904/templates/system/system_definitions.h.ftl")
    wm8904SymSystemDefIncFile.setMarkup(True)
    
    wm8904SymSystemDefObjFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_DEF_OBJECT", None)
    wm8904SymSystemDefObjFile.setType("STRING")
    wm8904SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    wm8904SymSystemDefObjFile.setSourcePath("codec/WM8904/templates/system/system_definitions_objects.h.ftl")
    wm8904SymSystemDefObjFile.setMarkup(True)

    wm8904SymSystemConfigFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_CONFIG", None)
    wm8904SymSystemConfigFile.setType("STRING")
    wm8904SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    wm8904SymSystemConfigFile.setSourcePath("codec/WM8904/templates/system/system_config.h.ftl")
    wm8904SymSystemConfigFile.setMarkup(True)

    wm8904SymSystemInitDataFile = wm8904Component.createFileSymbol("DRV_WM8904_INIT_DATA", None)
    wm8904SymSystemInitDataFile.setType("STRING")
    wm8904SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    wm8904SymSystemInitDataFile.setSourcePath("codec/WM8904/templates/system/system_initialize_data.c.ftl")
    wm8904SymSystemInitDataFile.setMarkup(True)

    wm8904SymSystemInitFile = wm8904Component.createFileSymbol("DRV_WM8904_SYS_INIT", None)
    wm8904SymSystemInitFile.setType("STRING")
    wm8904SymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")  
    wm8904SymSystemInitFile.setSourcePath("codec/WM8904/templates/system/system_initialize.c.ftl")
    wm8904SymSystemInitFile.setMarkup(True)

    wm8904SystemTaskFile = wm8904Component.createFileSymbol("DRV_WM8904_SYSTEM_TASKS_C", None)
    wm8904SystemTaskFile.setType("STRING")
    wm8904SystemTaskFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    wm8904SystemTaskFile.setSourcePath("codec/WM8904/templates/system/system_tasks.c.ftl")
    wm8904SystemTaskFile.setMarkup(True)

# this callback occurs when user connects I2C or I2S driver to WM8904 driver block in Project Graph    
def onDependencyConnected(info):
    global i2sPlibId
    if info["dependencyID"] == "DRV_I2S":
        plibUsed = info["localComponent"].getSymbolByID("DRV_WM8904_I2S")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        i2sOri2cId = i2sOri2cId.replace("A_","")    # I2S driver in audio repo have an "a_" prefix
        Log.writeInfoMessage("I2S, Instance Chosen: " + i2sOri2cId)
    elif info["dependencyID"] == "DRV_I2C":
        plibUsed = info["localComponent"].getSymbolByID("DRV_WM8904_I2C")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        i2sOri2cId = i2sOri2cId.replace("A_","")    # I2C driver in audio repo have an "a_" prefix
        Log.writeInfoMessage("I2C, Instance Chosen: " + i2sOri2cId)
    plibUsed.setValue(i2sOri2cId, 1)
