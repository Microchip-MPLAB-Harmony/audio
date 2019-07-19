# coding: utf-8
##############################################################################
# Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

def instantiateComponent(genericcodecComponent):
    Log.writeInfoMessage("GENERICCODEC instantiated")

    custom = False
    genericcodecIndex = genericcodecComponent.createIntegerSymbol("GENERICCODEC_INDEX", None)
    genericcodecIndex.setVisible(False)
    genericcodecIndex.setDefaultValue(0)

    genericcodecUse = genericcodecComponent.createBooleanSymbol("USE_DRV_CODEC_GENERIC", None)
    genericcodecUse.setVisible(False)
    genericcodecUse.setDefaultValue(True)

    genericcodecIdx0 = genericcodecComponent.createBooleanSymbol("DRV_CODEC_GENERIC_INST_IDX0", None)
    genericcodecIdx0.setVisible(False)
    genericcodecIdx0.setDefaultValue(True)

    genericcodecI2CDriver = genericcodecComponent.createStringSymbol("DRV_GENERICCODEC_I2C", None)
    genericcodecI2CDriver.setVisible(True)
    genericcodecI2CDriver.setLabel("I2C Driver used")
    genericcodecI2CDriver.setReadOnly(True)
    genericcodecI2CDriver.setDefaultValue("I2C")

    genericcodecI2SDriver = genericcodecComponent.createStringSymbol("DRV_GENERICCODEC_I2S", None)
    genericcodecI2SDriver.setVisible(True)
    genericcodecI2SDriver.setLabel("I2S Driver used")
    genericcodecI2SDriver.setReadOnly(True)
    genericcodecI2SDriver.setDefaultValue("I2S")

    genericcodecClients = genericcodecComponent.createIntegerSymbol("DRV_GENERICCODEC_CLIENTS_NUMBER", None)
    genericcodecClients.setVisible(True)
    genericcodecClients.setLabel("Number of GENERICCODEC Driver Clients")
    genericcodecClients.setReadOnly(True)
    genericcodecClients.setDefaultValue(1)

    genericcodecBaud = genericcodecComponent.createIntegerSymbol("DRV_GENERICCODEC_BAUD_RATE", None)
    genericcodecBaud.setVisible(True)
    genericcodecBaud.setLabel("Sampling Rate")
    genericcodecBaud.setDefaultValue(48000)

    genericcodecVolume = genericcodecComponent.createIntegerSymbol("DRV_GENERICCODEC_VOLUME", None)
    genericcodecVolume.setVisible(True)
    genericcodecVolume.setLabel("Volume for the Codec in range 0(Min) to 255(Max)")
    genericcodecVolume.setDefaultValue(200)

    genericcodecFormat = genericcodecComponent.createKeyValueSetSymbol("DRV_GENERICCODEC_AUDIO_DATA_FORMAT", None)
    genericcodecFormat.setVisible(True)
    genericcodecFormat.setLabel("Audio Data Format")
    genericcodecFormat.addKey("DATA_16_BIT_LEFT_JUSTIFIED", "0", "16-bit Left Justified")
    genericcodecFormat.addKey("DATA_16_BIT_I2S", "1", "16-bit I2S")
    genericcodecFormat.addKey("DATA_32_BIT_LEFT_JUSTIFIED", "2", "32-bit Left Justified")
    genericcodecFormat.addKey("DATA_32_BIT_I2S", "3", "32-bit I2S")
    genericcodecFormat.setDisplayMode("Description")
    genericcodecFormat.setOutputMode("Key")
    genericcodecFormat.setDefaultValue(1) 

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
    
    genericcodecSymHeaderFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_HEADER", None)
    genericcodecSymHeaderFile.setSourcePath("codec/Generic/drv_genericcodec.h")
    genericcodecSymHeaderFile.setOutputName("drv_genericcodec.h")
    genericcodecSymHeaderFile.setDestPath("audio/driver/codec/genericcodec/")
    genericcodecSymHeaderFile.setProjectPath("config/" + configName + "/audio/driver/codec/genericcodec/")
    genericcodecSymHeaderFile.setType("HEADER")
    genericcodecSymHeaderFile.setOverwrite(True)
    
    genericcodecSymSourceFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SOURCE", None)
    genericcodecSymSourceFile.setSourcePath("codec/Generic/src/drv_genericcodec.c")
    genericcodecSymSourceFile.setOutputName("drv_genericcodec.c")
    genericcodecSymSourceFile.setDestPath("audio/driver/codec/genericcodec/")
    genericcodecSymSourceFile.setProjectPath("config/" + configName + "/audio/driver/codec/genericcodec/")
    genericcodecSymSourceFile.setType("SOURCE")
    genericcodecSymSourceFile.setOverwrite(True)

    genericcodecSymHeaderLocalFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_HEADER_LOCAL", None)
    genericcodecSymHeaderLocalFile.setSourcePath("codec/Generic/src/drv_genericcodec_local.h")
    genericcodecSymHeaderLocalFile.setOutputName("drv_genericcodec_local.h")
    genericcodecSymHeaderLocalFile.setDestPath("audio/driver/codec/genericcodec/")
    genericcodecSymHeaderLocalFile.setProjectPath("config/" + configName + "/audio/driver/codec/genericcodec/")
    genericcodecSymHeaderLocalFile.setType("SOURCE")
    genericcodecSymHeaderLocalFile.setOverwrite(True)
    
    genericcodecSymSystemDefIncFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SYSTEM_DEF", None)
    genericcodecSymSystemDefIncFile.setType("STRING")
    genericcodecSymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    genericcodecSymSystemDefIncFile.setSourcePath("codec/Generic/templates/system/system_definitions.h.ftl")
    genericcodecSymSystemDefIncFile.setMarkup(True)
    
    genericcodecSymSystemDefObjFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SYSTEM_DEF_OBJECT", None)
    genericcodecSymSystemDefObjFile.setType("STRING")
    genericcodecSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    genericcodecSymSystemDefObjFile.setSourcePath("codec/Generic/templates/system/system_definitions_objects.h.ftl")
    genericcodecSymSystemDefObjFile.setMarkup(True)

    genericcodecSymSystemConfigFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SYSTEM_CONFIG", None)
    genericcodecSymSystemConfigFile.setType("STRING")
    genericcodecSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    genericcodecSymSystemConfigFile.setSourcePath("codec/Generic/templates/system/system_config.h.ftl")
    genericcodecSymSystemConfigFile.setMarkup(True)

    genericcodecSymSystemInitDataFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_INIT_DATA", None)
    genericcodecSymSystemInitDataFile.setType("STRING")
    genericcodecSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    genericcodecSymSystemInitDataFile.setSourcePath("codec/Generic/templates/system/system_initialize_data.c.ftl")
    genericcodecSymSystemInitDataFile.setMarkup(True)

    genericcodecSymSystemInitFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SYS_INIT", None)
    genericcodecSymSystemInitFile.setType("STRING")
    genericcodecSymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")  
    genericcodecSymSystemInitFile.setSourcePath("codec/Generic/templates/system/system_initialize.c.ftl")
    genericcodecSymSystemInitFile.setMarkup(True)

    genericcodecSystemTaskFile = genericcodecComponent.createFileSymbol("DRV_GENERICCODEC_SYSTEM_TASKS_C", None)
    genericcodecSystemTaskFile.setType("STRING")
    genericcodecSystemTaskFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    genericcodecSystemTaskFile.setSourcePath("codec/Generic/templates/system/system_tasks.c.ftl")
    genericcodecSystemTaskFile.setMarkup(True)

# this callback occurs when user connects I2C or I2S driver to GENERICCODEC driver block in Project Graph    
def onDependencyConnected(info):
    global i2sPlibId
    if info["dependencyID"] == "DRV_I2S":
        plibUsed = info["localComponent"].getSymbolByID("DRV_GENERICCODEC_I2S")
        i2sOri2cId = info["remoteComponent"].getID().upper()
        i2sOri2cId = i2sOri2cId.replace("A_","")    # I2S driver in audio repo have an "a_" prefix
    elif info["dependencyID"] == "DRV_I2C":
        plibUsed = info["localComponent"].getSymbolByID("DRV_GENERICCODEC_I2C")
        i2sOri2cId = info["remoteComponent"].getID().upper()
    plibUsed.setValue(i2sOri2cId, 1)
