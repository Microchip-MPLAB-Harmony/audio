"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

def monomodeUpdate(source, event):
    source.getComponent().getSymbolByID("MonoMode").setVisible(event["value"])
    if event["value"]==True:   
        print"Mono Enabled!!"
    else:
        print"Mono disabled!!"
        
def norvalUpdate(source, event):
    nov = source.getComponent().getSymbolByID("NOVRVAL")
    if event["value"]== 2 or event["value"]== 3:  
        nov.setVisible(True)
        print"Non Overlap mode enabled."
    else:
        nov.setVisible(False)
        print"Non Overlap mode disabled."

def instantiateComponent(classdComponent):
   
    frame = classdComponent.createKeyValueSetSymbol("frame", None)
    frame.setLabel("Incoming Data Sampling Frequency")
    frame.addKey("FRAME_8k", "0", "8 kHz.")
    frame.addKey("FRAME_16k", "1", "16 kHz.")
    frame.addKey("FRAME_32k", "2", "32 kHz.")
    frame.addKey("FRAME_48k", "3", "48 kHz.")
    frame.addKey("FRAME_96k", "4", "96 kHz.")
    frame.addKey("FRAME_22k", "5", "22.05 kHz.")
    frame.addKey("FRAME_44k", "6", "44.1 kHz.")
    frame.addKey("FRAME_88k", "7", "88.2 kHz.")
    frame.setDisplayMode("Key")
    frame.setOutputMode("Key")
    frame.setDefaultValue(3)
    frame.setDescription("Configures the Incoming Data Sampling Frequency.")
    
    output = classdComponent.createKeyValueSetSymbol("output", None)
    output.setLabel("CLASSD Output Mode")
    output.addKey("SINGLE_ENDED", "0", "Single ended.")
    output.addKey("DIFFERENTIAL", "1", "Differential.")
    output.addKey("HALF_BRIDGE", "2", "Half bridge.")
    output.addKey("FULL_BRIDGE", "3", "Full bridge.")
    output.setDisplayMode("Key")
    output.setOutputMode("Key")
    output.setDefaultValue(3)
    output.setDescription("Sets the CLASSD Outout mode.")
    output.setDependencies(norvalUpdate, ["output"])
    
    
    NOVRVAL = classdComponent.createKeyValueSetSymbol("NOVRVAL", None)
    NOVRVAL.setLabel("Non-overlapping time")
    NOVRVAL.addKey("5NS", "0", "5 Ns.")
    NOVRVAL.addKey("10NS", "1", "10 Ns.")
    NOVRVAL.addKey("15NS", "2", "15 Ns.")
    NOVRVAL.addKey("20NS", "3", "20 Ns.")
    NOVRVAL.setDisplayMode("Key")
    NOVRVAL.setOutputMode("Key")
    NOVRVAL.setDefaultValue(1)
    NOVRVAL.setVisible(True)
    NOVRVAL.setDescription("Sets the NonOverlaping time if Non Overlap bit is set.")
    
    
    tr = classdComponent.createKeyValueSetSymbol("tr", None)
    tr.setLabel("CLASSD Transfer Mode")
    tr.addKey("poll", "0", "Polling method for transfer.")
    tr.addKey("dma", "1", "DMA method for transfer.")
    tr.setDisplayMode("Key")
    tr.setOutputMode("Key")
    tr.setDefaultValue(0)
    tr.setDescription("Configures the ClassD transfer method (polling/DMA).")
    
    Mono = classdComponent.createBooleanSymbol("Mono", None)
    Mono.setLabel("Mono Enable")
    Mono.setDefaultValue(True)
    Mono.setDescription("Full mode allows the preview and codec paths to operate simultaneously.")
    Mono.setDependencies(monomodeUpdate, ["Mono"])
    
    
    MonoMode = classdComponent.createKeyValueSetSymbol("MonoMode", None)
    MonoMode.setLabel("Mono Mode")
    MonoMode.addKey("MONOMIX", "0", "(left+right)/2 is sent to both channels.")
    MonoMode.addKey("MONOSAT", "1", "(left+right)/2 is sent to both channels.")
    MonoMode.addKey("MONOLEFT", "2", "(left+right)/2 is sent to both channels.")
    MonoMode.addKey("MONORIGHT", "3", "(left+right)/2 is sent to both channels.")
    MonoMode.setVisible(True)
    MonoMode.setDisplayMode("Key")
    MonoMode.setOutputMode("Key")
    MonoMode.setDefaultValue(0)
    MonoMode.setDescription("Defines which signal is sent to both channels when MONO mode is selected.")
    
    eqcfg = classdComponent.createKeyValueSetSymbol("eqcfg", None)
    eqcfg.setLabel("Equaization Selection")
    eqcfg.addKey("FLAT", "0", "8 kHz.")
    eqcfg.addKey("BBOOST12", "1", "16 kHz.")
    eqcfg.addKey("BBOOST6", "2", "32 kHz.")
    eqcfg.addKey("BCUT12", "3", "48 kHz.")
    eqcfg.addKey("BCUT6", "4", "96 kHz.")
    eqcfg.addKey("MBOOST3", "5", "22.05 kHz.")
    eqcfg.addKey("MBOOST8", "6", "44.1 kHz.")
    eqcfg.addKey("MCUT3", "7", "88.2 kHz.")
    eqcfg.addKey("MCUT8", "8", "48 kHz.")
    eqcfg.addKey("TBOOST12", "9", "96 kHz.")
    eqcfg.addKey("TBOOST6", "10", "22.05 kHz.")
    eqcfg.addKey("TCUT12", "11", "44.1 kHz.")
    eqcfg.addKey("TCUT6", "12", "88.2 kHz.")
    eqcfg.setDisplayMode("Key")
    eqcfg.setOutputMode("Key")
    eqcfg.setDefaultValue(0)
    eqcfg.setDescription("Selects the desired Equalization.")
    
    ######################################################################
    
    configName = Variables.get("__CONFIGURATION_NAME")
    
    classdHeader1File = classdComponent.createFileSymbol("CLASSD_HEADER", None)
    classdHeader1File.setSourcePath("../peripheral/classd_11283/templates/plib_classd.h.ftl")
    classdHeader1File.setOutputName("plib_classd" + ".h")
    classdHeader1File.setDestPath("audio/peripheral/classd/")
    classdHeader1File.setProjectPath("config/" + configName +"/audio/peripheral/classd/")
    classdHeader1File.setType("HEADER")
    classdHeader1File.setMarkup(True)
    
    classdSource1File = classdComponent.createFileSymbol("CLASSD_SOURCE", None)
    classdSource1File.setSourcePath("../peripheral/classd_11283/templates/plib_classd.c.ftl")
    classdSource1File.setOutputName("plib_classd" + ".c")
    classdSource1File.setDestPath("audio/peripheral/classd/")
    classdSource1File.setProjectPath("config/" + configName +"/audio/peripheral/classd/")
    classdSource1File.setType("SOURCE")
    classdSource1File.setMarkup(True)
    
    
    classdSystemInitFile = classdComponent.createFileSymbol("CLASSD_INIT", None)
    classdSystemInitFile.setType("STRING")
    classdSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    classdSystemInitFile.setSourcePath("../peripheral/classd_11283/templates/system/initialization.c.ftl")
    classdSystemInitFile.setMarkup(True)
    
    classdSystemDefFile = classdComponent.createFileSymbol("CLASSD_DEF", None)
    classdSystemDefFile.setType("STRING")
    classdSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    classdSystemDefFile.setSourcePath("../peripheral/classd_11283/templates/system/definitions.h.ftl")
    classdSystemDefFile.setMarkup(True)
    
