# coding: utf-8
#
# math.py  - Creates the math component of MHC Version 3 
# 
# NOTES(s):
#   1)  Library component in audio/math
#   2)  Libraries: libq_c, libq (pic32mz), and libdsp (pic32mz/pic32mx)
#
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
import os
import sys

dbg = 0 #???

src_ext      = ('.c')
hdr_ext      = ('.h')
mipslib_ext  = ('.a')
samlib_ext   = ('.a')
mips_ext     = ('.S')

################################################################################
#### Lists of files to generate for each library
################################################################################
#Library Files to add to the stand alone project
libqc_Table  = [("LIB_", "libq_c/"   , "libq_c.h", "audio/math/libq_c"),
                #("LIB_", "libq_c/include/", "fract.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "fast_trig_functions.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "fract_fixedpoint.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "LUT__3Terms__nLUT_256.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "pixel_rotation.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "SinX_LUT_91.h", "audio/math/libq_c/include"),
                ("LIB_", "libq_c/include/", "XInDegrees_LUT_91.h", "audio/math/libq_c/include"),

                #Source Files
                ("LIB_", "libq_c/src/", "fract.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "fast_trig_functions.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "pixel_rotation.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_FxNorm.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_Abs_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_Add_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_DivisionWithSaturation_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_ExpAvg_q15_q15_q1d15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_ExtractH_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_MacR_q31_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_MsuR_q31_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_MultiplyR2_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_Negate_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_RoundL_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_ShiftLeft_q15_i16.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_ShiftRight_q15_i16.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q15_ShiftRightR_q15_i16.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q1d15_Sin_q10d6.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Abs_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Add_q31_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_DepositH_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Mac_q31_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Msu_q31_q15_q15.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Mult2_q15_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Multi_q15_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_Negate_q31.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_ShiftLeft_q31_i16.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_ShiftRight_q31_i16.c", "audio/math/libq_c/src"),
                ("LIB_", "libq_c/src/", "libq_q31_ShiftRightR_q31_i16.c", "audio/math/libq_c/src")]

libq_Table         = [("LIB_", "libq/", "libq.h", "audio/math/libq")]

libq_nfO3_Table    = [("LIB_", "libq/lib/", "libq_mips32_mz_ef_nfO3.a" ,"audio/math/libq")]

libq_nfOs_Table    = [("LIB_", "libq/lib/", "libq_mips32_mz_ef_nfOs.a" ,"audio/math/libq")]


libdsp_Table       = [("LIB_", "dsp/" , "dsp.h" , "audio/math/dsp"),
                      ("LIB_", "dsp/lib/" , "dsp_pic32mz_ef_nofpu.a", "audio/math/dsp")]

#Audio Component Templates
ftlTable     = [("LIB_", "templates/", "audio_math.h.ftl", "audio/math"),
                ("LIB_", "templates/", "audio_math_config.h.ftl", "audio/math")]

################################################################################
#### Functions that add files to the project using the file lists above
################################################################################

# Enable libq_c Function - file symbols 
def enableLibqcFiles(component, enable):
    #Log.writeInfoMessage("ENABLE LIBQC:")
    #Add each file - (symbol, path, filename 
    for fileSymbol, srcPath, filename, destPath in libqc_Table:
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        #Log.writeInfoMessage("    baseFileName:  " + baseFileName)
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

# Enable libq_c File symbols on depending on event
def enableLibqC(symbol, event):
    enableLibqcFiles(symbol.getComponent(), event["value"]==True)

# Enable libq optimized nfO3 files symbols function
def enableLibqFiles(component, enable):
    Log.writeInfoMessage("ENABLE LIBQ:")
    #Add each file - (symbol, path, filename 
    for fileSymbol, srcPath, filename, destPath in libq_Table:
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        elif ext in mips_ext:
            type = "SOURCE"
        elif ext in mipslib_ext:
            type = "SOURCE"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        print("    Enable SYMBOL: " + symbol)
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

# Enable PIC32 libq File symbols on depending on event
def enableLibq(symbol, event):
    enableLibqFiles(symbol.getComponent(), event["value"]==True)


# Enable libq optimized nfO3 files symbols function
def enableLibqNFO3Files(component, enable):
    Log.writeInfoMessage("ENABLE LIBQ NFO3:")
    #Add each file - (symbol, path, filename 
    for fileSymbol, srcPath, filename, destPath in libq_nfO3_Table:
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        elif ext in mips_ext:
            type = "SOURCE"
        elif ext in mipslib_ext:
            type = "SOURCE"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        print("    Enable SYMBOL: " + symbol)
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

# Enable PIC32 libq File symbols on depending on event
def enableLibqNFO3(symbol, event):
    enableLibqNFO3Files(symbol.getComponent(), event["value"]==True)

# Enable libq Function - file symbols 
def enableLibqNFOSFiles(component, enable):
    Log.writeInfoMessage("ENABLE LIBQ NFOS:")
    #Add each file - (symbol, path, filename 
    for fileSymbol, srcPath, filename, destPath in libq_nfOs_Table:
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        elif ext in mips_ext:
            type = "SOURCE"
        elif ext in mipslib_ext:
            type = "SOURCE"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        print("    Enable SYMBOL: " + symbol)
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

# Enable PIC32 libq File symbols on depending on event
def enableLibqNFOS(symbol, event):
    enableLibqNFOSFiles(symbol.getComponent(), event["value"]==True)

# Enable PIC32 libdsp Function - file symbols 
def enableLibdspFiles(component, enable):
    Log.writeInfoMessage("ENABLE DSP:")
    #Add each file - (symbol, path, filename 
    for fileSymbol, srcPath, filename, destPath in libdsp_Table:
        #Log.writeInfoMessage("    filename:  " + filename)
        #Log.writeInfoMessage("    srcPath:  " + srcPath)
        #Log.writeInfoMessage("    destPath:  " + destPath)
        #Log.writeInfoMessage("    fileSymbol:  " + fileSymbol)
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        #Log.writeInfoMessage("    baseFileName:  " + baseFileName)
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        elif ext in mips_ext:
            type = "SOURCE"
        elif ext in mipslib_ext:
            type = "SOURCE"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        print("    Enable SYMBOL: " + symbol)
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

# Enable libq_c File symbols on depending on event
def enableLibdsp(symbol, event):
    enableLibdspFiles(symbol.getComponent(), event["value"]==True)


################################################################################
############################################################################
#### Instantiation of the Math Component - dynamic     
#### Audio Math Library Component - with libq_c, libq, and libdsp libraries 
#### NOTE(s):  
####    1) MathComponent defined by module.py in math/config
############################################################################
################################################################################
def instantiateComponent(mathComponent):
    global CONFIG_USE_LIBQ_C_LIBRARY
    global CONFIG_USE_LIBQ_LIBRARY
    global CONFIG_USE_LIBQ_NFO3_LIBRARY
    global CONFIG_USE_LIBQ_NFOS_LIBRARY
    global CONFIG_USE_LIBDSP_LIBRARY
    global CONFIG_USE_LIBDSP_OPTIM_SPACE


    #Enable LIBQ_C - based on checkbox symbol CONFIG_USE_LIBQ_C_LIBRARY
    CONFIG_USE_LIBQ_C_LIBRARY = mathComponent.createBooleanSymbol("CONFIG_USE_LIBQ_C_LIBRARY", None)
    CONFIG_USE_LIBQ_C_LIBRARY.setVisible(True)
    CONFIG_USE_LIBQ_C_LIBRARY.setLabel("Enable LIBQ_C Library ")
    CONFIG_USE_LIBQ_C_LIBRARY.setDefaultValue(True)
    CONFIG_USE_LIBQ_C_LIBRARY.setDependencies(enableLibqC, ["CONFIG_USE_LIBQ_C_LIBRARY"])

    #LIBQ Library 
    CONFIG_LIBQ_ENABLE= mathComponent.createMenuSymbol("LIBQ_SELECT", None)
    CONFIG_LIBQ_ENABLE.setLabel("LIBQ Library")	
    CONFIG_LIBQ_ENABLE.setVisible(True)

    #Enable LIBQ - based on checkbox symbol CONFIG_USE_LIBQ_LIBRARY and PIC32MZ
    CONFIG_USE_LIBQ_LIBRARY = mathComponent.createBooleanSymbol("CONFIG_USE_LIBQ_LIBRARY", None)
    #if ("PIC32MZ" in Variables.get("__PROCESSOR")):
    CONFIG_USE_LIBQ_LIBRARY.setVisible(True)
    CONFIG_USE_LIBQ_LIBRARY.setLabel("Enable LIBQ O3 - No FPU Library")
    CONFIG_USE_LIBQ_LIBRARY.setDefaultValue(False)
    CONFIG_USE_LIBQ_LIBRARY.setDependencies(enableLibq, ["CONFIG_USE_LIBQ_LIBRARY"])

    #Enable LIBQ nfO3 Version - based on checkbox symbol CONFIG_USE_LIBQ_NFO3_LIBRARY and PIC32MZ
    #--under CONFIG_LIBQ_ENABLE Symbol
    CONFIG_USE_LIBQ_NFO3_LIBRARY = mathComponent.createBooleanSymbol("CONFIG_USE_LIBQ_NFO3_LIBRARY", CONFIG_LIBQ_ENABLE)
    CONFIG_USE_LIBQ_NFO3_LIBRARY.setVisible(True)
    CONFIG_USE_LIBQ_NFO3_LIBRARY.setLabel("Enable LIBQ Speed Optimized (O3) - No FPU Library")
    CONFIG_USE_LIBQ_NFO3_LIBRARY.setDefaultValue(False)
    CONFIG_USE_LIBQ_NFO3_LIBRARY.setDependencies(enableLibqNFO3, ["CONFIG_USE_LIBQ_NFO3_LIBRARY"])

    #Enable LIBQ nfOs Version - based on checkbox symbol CONFIG_USE_LIBQ_NFOS_LIBRARY and PIC32MZ
    #--under CONFIG_LIBQ_ENABLE Symbol
    CONFIG_USE_LIBQ_NFOS_LIBRARY = mathComponent.createBooleanSymbol("CONFIG_USE_LIBQ_NFOS_LIBRARY",
        CONFIG_LIBQ_ENABLE)
    CONFIG_USE_LIBQ_NFOS_LIBRARY.setVisible(True)
    CONFIG_USE_LIBQ_NFOS_LIBRARY.setLabel("Enable LIBQ Optimize for Space(Os) - No FPU Library")
    CONFIG_USE_LIBQ_NFOS_LIBRARY.setDefaultValue(False)
    CONFIG_USE_LIBQ_NFOS_LIBRARY.setDependencies(enableLibqNFOS, ["CONFIG_USE_LIBQ_NFOS_LIBRARY"])


    #Enable LIBDSP - based on checkbox symbol CONFIG_USE_LIBDSP_LIBRARY and PIC32MZ
    CONFIG_USE_LIBDSP_LIBRARY = mathComponent.createBooleanSymbol("CONFIG_USE_LIBDSP_LIBRARY", None)
    #if ("PIC32MZ" in Variables.get("__PROCESSOR")):
    CONFIG_USE_LIBDSP_LIBRARY.setVisible(True)
    CONFIG_USE_LIBDSP_LIBRARY.setLabel("Enable Pic32MZ DSP Library ")
    CONFIG_USE_LIBDSP_LIBRARY.setDefaultValue(False)
    CONFIG_USE_LIBDSP_LIBRARY.setDependencies(enableLibdsp, ["CONFIG_USE_LIBDSP_LIBRARY"])

    #====================================================
    # Start of Code Generation - Audio Math Component    
    #====================================================

    configName = Variables.get("__CONFIGURATION_NAME")  # e.g. "default"
    processor = Variables.get("__PROCESSOR")

    Log.writeInfoMessage("Audio Math instantiated:  " + processor)

    #LIBQ_C Code Generation
    Log.writeInfoMessage("LIBQ_C Code Generation")
    for fileSymbol, srcPath, filename, destPath in libqc_Table:
        #Log.writeInfoMessage("    filename:  " + filename)
        #Log.writeInfoMessage("    srcPath:  " + srcPath)
        #Log.writeInfoMessage("    destPath:  " + destPath)
        #Log.writeInfoMessage("    fileSymbol:  " + fileSymbol)
        # Set type
        baseFileName = os.path.splitext(filename)[0]
        ext = os.path.splitext(filename)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique filename symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        Log.writeInfoMessage("    Code SYMBOL: " + symbol )

        exec(symbol + " = mathComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + filename + "\")")
        exec(symbol + ".setOutputName(\"" + filename +           "\")")
        exec(symbol + ".setDestPath(  \"" + destPath +       "\")")
        if (type == "HEADER"):
            exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq_c\")")
        else:
            exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq_c\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_LIBQ_C_LIBRARY.getValue() == True)")

    #LIBQ NFO3 Code Generation
    if ("PIC32MZ" in Variables.get("__PROCESSOR")):
        Log.writeInfoMessage("LIBQ NFO3 Code Generation")
        for fileSymbol, srcPath, filename, destPath in libq_nfO3_Table:
            Log.writeInfoMessage("    filename:  " + filename)
            Log.writeInfoMessage("    srcPath:  " + srcPath)
            Log.writeInfoMessage("    destPath:  " + destPath)
            Log.writeInfoMessage("    fileSymbol:  " + fileSymbol)

            # Set type
            baseFileName = os.path.splitext(filename)[0]
            ext = os.path.splitext(filename)[-1].lower()
            if ext in src_ext:
                type = "SOURCE"
            elif ext in hdr_ext:
                type = "HEADER"
            elif ext in mips_ext:
                type = "SOURCE"
            elif ext in mipslib_ext:
                type = "SOURCE"
            else:
                type = "IMPORTANT"

            # Create unique filename symbol
            symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
            Log.writeInfoMessage("    code SYMBOL: " + symbol + "\n")

            exec(symbol + " = mathComponent.createFileSymbol(\"" + symbol + "\", None)")
            exec(symbol + ".setSourcePath(\"" + srcPath + filename + "\")")
            exec(symbol + ".setOutputName(\"" + filename +           "\")")
            exec(symbol + ".setDestPath(  \"" + destPath +       "\")")
            if (type == "HEADER"):
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq\")")
            else:
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq\")")
            exec(symbol + ".setType(\"" + type + "\")")
#????
            exec(symbol + ".setEnabled(CONFIG_USE_LIBQ_NFO3_LIBRARY.getValue() == True)")

    #LIBQ NFOS Code Generation
    if ("PIC32MZ" in Variables.get("__PROCESSOR")):
        Log.writeInfoMessage("LIBQ NFOS Code Generation")
        for fileSymbol, srcPath, filename, destPath in libq_nfOs_Table:
            Log.writeInfoMessage("    filename:  " + filename)
            Log.writeInfoMessage("    srcPath:  " + srcPath)
            Log.writeInfoMessage("    destPath:  " + destPath)
            Log.writeInfoMessage("    fileSymbol:  " + fileSymbol)

            # Set type
            baseFileName = os.path.splitext(filename)[0]
            ext = os.path.splitext(filename)[-1].lower()
            if ext in src_ext:
                type = "SOURCE"
            elif ext in hdr_ext:
                type = "HEADER"
            elif ext in mips_ext:
                type = "SOURCE"
            elif ext in mipslib_ext:
                type = "SOURCE"
            else:
                type = "IMPORTANT"

            # Create unique filename symbol
            symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
            Log.writeInfoMessage("    code SYMBOL: " + symbol + "\n")

            exec(symbol + " = mathComponent.createFileSymbol(\"" + symbol + "\", None)")
            exec(symbol + ".setSourcePath(\"" + srcPath + filename + "\")")
            exec(symbol + ".setOutputName(\"" + filename +           "\")")
            exec(symbol + ".setDestPath(  \"" + destPath +       "\")")
            if (type == "HEADER"):
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq\")")
            else:
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libq\")")
            exec(symbol + ".setType(\"" + type + "\")")
#????
            exec(symbol + ".setEnabled(CONFIG_USE_LIBQ_NFOS_LIBRARY.getValue() == True)")


    #LIBDSP Code Generation
    if ("PIC32MZ" in Variables.get("__PROCESSOR")):
        Log.writeInfoMessage("    DSP Code Generation:")
        for fileSymbol, srcPath, filename, destPath in libdsp_Table:
            Log.writeInfoMessage("    filename:  " + filename)
            Log.writeInfoMessage("    srcPath:  " + srcPath)
            Log.writeInfoMessage("    destPath:  " + destPath)
            Log.writeInfoMessage("    fileSymbol:  " + fileSymbol)
            # Set type
            baseFileName = os.path.splitext(filename)[0]
            ext = os.path.splitext(filename)[-1].lower()
            if ext in src_ext:
                type = "SOURCE"
            elif ext in hdr_ext:
                type = "HEADER"
            elif ext in mips_ext:
                type = "SOURCE"
            elif ext in mipslib_ext:
                type = "SOURCE"
            else:
                type = "IMPORTANT"

            # Create unique filename symbol
            symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
            Log.writeInfoMessage("    code SYMBOL: " + symbol)

            exec(symbol + " = mathComponent.createFileSymbol(\"" + symbol + "\", None)")
            exec(symbol + ".setSourcePath(\"" + srcPath + filename + "\")")
            exec(symbol + ".setOutputName(\"" + filename +           "\")")
            exec(symbol + ".setDestPath(  \"" + destPath +       "\")")
            if (type == "HEADER"):
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libdsp\")")
            else:
                exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math/libdsp\")")
            exec(symbol + ".setType(\"" + type + "\")")
            exec(symbol + ".setEnabled(CONFIG_USE_LIBDSP_LIBRARY.getValue() == True)")
            

    #Math Header Files 
    Log.writeInfoMessage("Audio Math Header Files")
    for fileSymbol, srcPath, filename, destPath in ftlTable:
        # Set type
        baseFileName1 = os.path.splitext(filename)[0]       # Strip the .ftl extension
        baseFileName = os.path.splitext(baseFileName1)[0]
        ext = os.path.splitext(baseFileName1)[-1].lower()
        Log.writeInfoMessage("fullname: " + baseFileName1 + ", name: " + baseFileName + ", ext: " + ext)
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique name symbol
        symbol = fileSymbol + srcPath.replace("/", "_").replace(".", "").upper() + baseFileName.upper() + "_" + type.upper()
        Log.writeInfoMessage("SYMBOL: " + symbol)

        exec(symbol + " = mathComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + filename + "\")")
        exec(symbol + ".setOutputName(\"" + baseFileName1 + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/math\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(True)")
        exec(symbol + ".setMarkup(True)")

    #System Template Files
    mathSymSystemDefObjFile = mathComponent.createFileSymbol("MATH_FILE_SYS_DEF", None)
    mathSymSystemDefObjFile.setType("STRING")
    mathSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    mathSymSystemDefObjFile.setSourcePath("/templates/system/system_definitions.h.ftl")
    mathSymSystemDefObjFile.setMarkup(True)

    if("PIC32MZ" in Variables.get("__PROCESSOR")):
       #Allow the selection
       CONFIG_USE_LIBQ_LIBRARY.setVisible(True)
       CONFIG_USE_LIBQ_NFO3_LIBRARY.setVisible(True)
       CONFIG_USE_LIBQ_NFOS_LIBRARY.setVisible(True)
       CONFIG_USE_LIBDSP_LIBRARY.setVisible(True)
       Log.writeInfoMessage("libq dsp visible")
    else:
       #Hide the selection
       CONFIG_USE_LIBQ_LIBRARY.setVisible(False)
       CONFIG_USE_LIBQ_NFO3_LIBRARY.setVisible(False)
       CONFIG_USE_LIBQ_NFOS_LIBRARY.setVisible(False)
       CONFIG_USE_LIBDSP_LIBRARY.setVisible(False)
       Log.writeInfoMessage("libq dsp hidden")

# TODO: Add check for using the compiled library
#   Check if .a files exist, if so setReadOnly(False)
#    if(os.path.is_file("../lib/math/libq_c):

# ADD files to Src
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	symbol.setProjectPath("config/" + configName1 + destPath)
	symbol.setSourcePath(srcPath + fileName)
	symbol.setOutputName(fileName)
	symbol.setDestPath(destPath)
	if fileName[-2:] == '.h':
		symbol.setType("HEADER")
	else:
		symbol.setType("SOURCE")
	symbol.setEnabled(enabled)
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])
