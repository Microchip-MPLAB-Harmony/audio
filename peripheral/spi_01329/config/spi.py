"""*****************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

################################################################################
#### Register Information ####
################################################################################
# SPICON Register
i2sValGrp_SPI1CON_MSTEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__MSTEN"]')
i2sBitField_SPI1CON_MSTEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="MSTEN"]')

i2sValGrp_SPI1CON_MSSEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__MSSEN"]')
i2sBitField_SPI1CON_MSSEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="MSSEN"]')

i2sValGrp_SPI1CON_MODE = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__MODE32"]')
i2sBitField_SPI1CON_MODE = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="MODE32"]')

i2sValGrp_SPI1CON_CKE = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__CKE"]')
i2sBitField_SPI1CON_CKE = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="CKE"]')

i2sValGrp_SPI1CON_CKP = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__CKP"]')
i2sBitField_SPI1CON_CKP = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="CKP"]')

i2sValGrp_SPI1CON_MSSEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__MSSEN"]')
i2sBitField_SPI1CON_MSSEN = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="MSSEN"]')

i2sValGrp_SPI1CON_MCLKSEL = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON__MCLKSEL"]')
i2sBitField_SPI1CON_MCLKSEL = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON"]/bitfield@[name="MCLKSEL"]')

i2sValGrp_SPI1CON2_AUDMOD = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON2__AUDMOD"]')
i2sBitField_SPI1CON2_AUDMOD = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON2"]/bitfield@[name="AUDMOD"]')

i2sValGrp_SPI1CON2_AUDMONO = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/value-group@[name="SPI1CON2__AUDMONO"]')
i2sBitField_SPI1CON2_AUDMONO = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="SPI1CON2"]/bitfield@[name="AUDMONO"]')

################################################################################
#### Global Variables ####
################################################################################

global interruptsChildren
interruptsChildren = ATDF.getNode('/avr-tools-device-file/devices/device/interrupts').getChildren()

################################################################################
#### Callbacks
################################################################################

################################################################################
#### Business Logic ####
################################################################################

def _get_enblReg_parms(vectorNumber):

    # This takes in vector index for interrupt, and returns the IECx register name as well as
    # mask and bit location within it for given interrupt
    index = int(vectorNumber / 32)
    regName = "IEC" + str(index)
    bitPosn = int(vectorNumber % 32)
    bitMask = hex(1 << bitPosn)

    return regName, str(bitPosn), str(bitMask)

def _get_statReg_parms(vectorNumber):

    # This takes in vector index for interrupt, and returns the IFSx register name as well as
    # mask and bit location within it for given interrupt
    index = int(vectorNumber / 32)
    regName = "IFS" + str(index)
    bitPosn = int(vectorNumber % 32)
    bitMask = hex(1 << bitPosn)

    return regName, str(bitPosn), str(bitMask)

def _get_bitfield_names(node, outputList):

    valueNodes = node.getChildren()

    for bitfield in valueNodes:   ##  do this for all <value > entries for this bitfield
        dict = {}
        if bitfield.getAttribute("caption").lower() != "reserved":  ##  skip (unused) reserved fields
            dict["desc"] = bitfield.getAttribute("caption")
            dict["key"] = bitfield.getAttribute("caption")

            ##  Get rid of leading '0x', and convert to int if was hex
            value = bitfield.getAttribute("value")

            if(value[:2] == "0x"):
                temp = value[2:]
                tempint = int(temp, 16)
            else:
                tempint = int(value)

            dict["value"] = str(tempint)
            outputList.append(dict)

def getIRQIndex(string):

    irq_index = "-1"

    for param in interruptsChildren:
        if "irq-index" in param.getAttributeList():
            name = str(param.getAttribute("irq-name"))
            if string == name:
                irq_index = str(param.getAttribute("irq-index"))
                break
        else:
            break

    return irq_index

def getVectorIndex(string):

    vector_index = "-1"

    for param in interruptsChildren:
        name = str(param.getAttribute("name"))
        if string == name:
            vector_index = str(param.getAttribute("index"))
            break

    return vector_index

global i2sCalculateBRGValue
def i2sCalculateBRGValue(ratio, baudRate):

    global i2sSym_BaudError_Comment
    t_brg = 0
    clkfreq = baudRate * ratio   

    if clkfreq != 0 and baudRate != 0:
        t_brg = ((int(clkfreq/baudRate) / 2) - 1)
        baudHigh = int (clkfreq / (2 * (t_brg + 1)))
        baudLow = int (clkfreq / (2 * (t_brg + 2)))
        errorHigh = baudHigh - baudRate
        errorLow = baudRate - baudLow

        if errorHigh > errorLow:
            t_brg +=1

    i2sSym_BaudError_Comment.setVisible(False)

    ## Baud rate register is a 9/13 bit register
    if t_brg < 0:
        t_brg = 0
        i2sSym_BaudError_Comment.setVisible(True)
    elif t_brg > i2sSymMaxBRG.getValue():
        t_brg = i2sSymMaxBRG.getValue()
        i2sSym_BaudError_Comment.setVisible(True)

    return int(t_brg)

def i2sSPIBRG_ValueUpdate(symbol, event):
    ratio = i2sSym_MCLK_BCLK_Ratio.getValue()
    baudRate = i2sSym_Baud_Rate.getValue()

    t_brg = i2sCalculateBRGValue(ratio, baudRate)
    i2sSym_SPIBRG_VALUE.setValue(t_brg)
   
def updateI2sClockWarningStatus(symbol, event):
    symbol.setVisible(not event["value"])
	
################################################################################
#### Instantiate Component ####
################################################################################

def instantiateComponent(i2sComponent):

    global i2sSym_BaudError_Comment
    global i2sSym_Baud_Rate
    global i2sSym_MCLK_BCLK_Ratio
    global spiInstanceName

    InterruptVector = []
    InterruptHandler = []
    InterruptHandlerLock = []
    InterruptVectorUpdate = []

    i2sID = i2sComponent.getID().replace("a_i2s","i2s")    # PLIBs in audio repo have an "a_" prefix

    # for MX/MZ processors, I2S masquerades as SPI
    i2sComponentName = i2sID.replace("i2s","spi")

    spiInstanceName = i2sComponent.createStringSymbol("SPI_INSTANCE_NAME", None)    # used for SPI registers
    spiInstanceName.setVisible(False)
    spiInstanceName.setDefaultValue(i2sComponentName.upper())               

    i2sInstanceName = i2sComponent.createStringSymbol("I2S_INSTANCE_NAME", None)
    i2sInstanceName.setVisible(False)
    i2sInstanceName.setDefaultValue(i2sID.upper())
    Log.writeInfoMessage("Running " + i2sInstanceName.getValue())   # prints "Running I2S1"

    i2sInstanceNum = i2sComponent.createStringSymbol("I2S_INSTANCE_NUM", None)
    i2sInstanceNum.setVisible(False)
    componentName = str(i2sComponentName)
    instanceNum = filter(str.isdigit,componentName)
    i2sInstanceNum.setDefaultValue(instanceNum)
    	
    #Clock enable
    Database.setSymbolValue("core", spiInstanceName.getValue() + "_CLOCK_ENABLE", True, 1)

    spiIrq = "SPI_" + i2sInstanceNum.getValue()
    spiVectorNum = getVectorIndex(spiIrq)

    if spiVectorNum != "-1":
        InterruptVector.append(spiIrq + "_INTERRUPT_ENABLE")
        InterruptHandler.append(spiIrq + "_INTERRUPT_HANDLER")
        InterruptHandlerLock.append(spiIrq + "_INTERRUPT_HANDLER_LOCK")
        InterruptVectorUpdate.append("core." + spiIrq + "_INTERRUPT_ENABLE_UPDATE")

        ## SPI Error IRQ
        spiIrqFault = spiInstanceName.getValue() + "_ERR"
        spiFaultVectorNum = int(getIRQIndex(spiIrqFault))

        ## SPI RX IRQ
        spiIrqrRx = spiInstanceName.getValue() + "_RX"
        spiRxVectorNum = int(getIRQIndex(spiIrqrRx))

        ## SPI TX IRQ
        spiIrqTx = spiInstanceName.getValue() + "_TX"
        spiTxVectorNum = int(getIRQIndex(spiIrqTx))
    else:
        ## SPI Fault Interrrupt
        spiIrqFault = spiInstanceName.getValue() + "_FAULT"
        spiFaultVectorNum = int(getVectorIndex(spiIrqFault))

        ## SPI RX Interrupt
        spiIrqrRx = spiInstanceName.getValue() + "_RX"
        InterruptVector.append(spiIrqrRx + "_INTERRUPT_ENABLE")
        InterruptHandler.append(spiIrqrRx + "_INTERRUPT_HANDLER")
        InterruptHandlerLock.append(spiIrqrRx + "_INTERRUPT_HANDLER_LOCK")
        InterruptVectorUpdate.append("core." + spiIrqrRx + "_INTERRUPT_ENABLE_UPDATE")
        spiRxVectorNum = int(getVectorIndex(spiIrqrRx))

        ## SPI TX Interrupt
        spiIrqTx = spiInstanceName.getValue() + "_TX"
        InterruptVector.append(spiIrqTx + "_INTERRUPT_ENABLE")
        InterruptHandler.append(spiIrqTx + "_INTERRUPT_HANDLER")
        InterruptHandlerLock.append(spiIrqTx + "_INTERRUPT_HANDLER_LOCK")
        InterruptVectorUpdate.append("core." + spiIrqTx + "_INTERRUPT_ENABLE_UPDATE")
        spiTxVectorNum = int(getVectorIndex(spiIrqTx))

    enblRegName, enblBitPosn, enblMask = _get_enblReg_parms(spiFaultVectorNum)
    statRegName, statBitPosn, statMask = _get_statReg_parms(spiFaultVectorNum)

    ## IEC REG
    spiIEC = i2sComponent.createStringSymbol("SPI_FLT_IEC_REG", None)
    spiIEC.setDefaultValue(enblRegName)
    spiIEC.setVisible(False)

    ## IEC REG MASK
    spiIECMask = i2sComponent.createStringSymbol("SPI_FLT_IEC_REG_MASK", None)
    spiIECMask.setDefaultValue(enblMask)
    spiIECMask.setVisible(False)

    ## IFS REG
    spiIFS = i2sComponent.createStringSymbol("SPI_FLT_IFS_REG", None)
    spiIFS.setDefaultValue(statRegName)
    spiIFS.setVisible(False)

    ## IFS REG MASK
    spiIFSMask = i2sComponent.createStringSymbol("SPI_FLT_IFS_REG_MASK", None)
    spiIFSMask.setDefaultValue(statMask)
    spiIFSMask.setVisible(False)

    enblRegName, enblBitPosn, enblMask = _get_enblReg_parms(spiRxVectorNum)
    statRegName, statBitPosn, statMask = _get_statReg_parms(spiRxVectorNum)

    ## IEC REG
    spiRXIEC = i2sComponent.createStringSymbol("SPI_RX_IEC_REG", None)
    spiRXIEC.setDefaultValue(enblRegName)
    spiRXIEC.setVisible(False)

    ## IEC REG MASK
    spiRXIECMask = i2sComponent.createStringSymbol("SPI_RX_IEC_REG_MASK", None)
    spiRXIECMask.setDefaultValue(enblMask)
    spiRXIECMask.setVisible(False)

    ## IFS REG
    spiRXIFS = i2sComponent.createStringSymbol("SPI_RX_IFS_REG", None)
    spiRXIFS.setDefaultValue(statRegName)
    spiRXIFS.setVisible(False)

    ## IFS REG MASK
    spiRXIFSMask = i2sComponent.createStringSymbol("SPI_RX_IFS_REG_MASK", None)
    spiRXIFSMask.setDefaultValue(statMask)
    spiRXIFSMask.setVisible(False)

    enblRegName, enblBitPosn, enblMask = _get_enblReg_parms(spiTxVectorNum)
    statRegName, statBitPosn, statMask = _get_statReg_parms(spiTxVectorNum)

    ## IEC REG
    spiTXIEC = i2sComponent.createStringSymbol("SPI_TX_IEC_REG", None)
    spiTXIEC.setDefaultValue(enblRegName)
    spiTXIEC.setVisible(False)

    ## IEC REG MASK
    spiTXIECMask = i2sComponent.createStringSymbol("SPI_TX_IEC_REG_MASK", None)
    spiTXIECMask.setDefaultValue(enblMask)
    spiTXIECMask.setVisible(False)

    ## IFS REG
    spiTXIFS = i2sComponent.createStringSymbol("SPI_TX_IFS_REG", None)
    spiTXIFS.setDefaultValue(statRegName)
    spiTXIFS.setVisible(False)

    ## IFS REG MASK
    spiTXIFSMask = i2sComponent.createStringSymbol("SPI_TX_IFS_REG_MASK", None)
    spiTXIFSMask.setDefaultValue(statMask)
    spiTXIFSMask.setVisible(False)

    ## MSTEN Selection Bit
    msten_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_MSTEN, msten_names)
    i2sSym_SPICON_MSTEN = i2sComponent.createKeyValueSetSymbol("I2S_MSTR_MODE_EN",None)
    i2sSym_SPICON_MSTEN.setLabel(i2sBitField_SPI1CON_MSTEN.getAttribute("caption"))
    i2sSym_SPICON_MSTEN.setDefaultValue(0)
    i2sSym_SPICON_MSTEN.setReadOnly(True)
    i2sSym_SPICON_MSTEN.setOutputMode( "Value" )
    i2sSym_SPICON_MSTEN.setDisplayMode( "Description" )
    for ii in msten_names:
        i2sSym_SPICON_MSTEN.addKey( ii['key'],ii['value'], ii['desc'] )

    ## CLock Polarity
    clkpol_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_CKP, clkpol_names)
    i2sSym_SPICON_CLKPOL = i2sComponent.createKeyValueSetSymbol( "I2S_SPICON_CLK_POL",None)
    i2sSym_SPICON_CLKPOL.setLabel(i2sBitField_SPI1CON_CKP.getAttribute("caption"))
    i2sSym_SPICON_CLKPOL.setDefaultValue(0)
    i2sSym_SPICON_CLKPOL.setOutputMode( "Value" )
    i2sSym_SPICON_CLKPOL.setDisplayMode( "Description" )
    for ii in clkpol_names:
        i2sSym_SPICON_CLKPOL.addKey( ii['key'],ii['value'], ii['desc'] )

    ## Clock Phase Bit
    clkph_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_CKE, clkph_names)
    i2sSym_SPICON_CLKPH = i2sComponent.createKeyValueSetSymbol( "I2S_SPICON_CLK_PH",None)
    i2sSym_SPICON_CLKPH.setLabel(i2sBitField_SPI1CON_CKE.getAttribute("caption"))
    i2sSym_SPICON_CLKPH.setDefaultValue(0)
    i2sSym_SPICON_CLKPH.setOutputMode( "Value" )
    i2sSym_SPICON_CLKPH.setDisplayMode( "Description" )
    for ii in clkph_names:
        i2sSym_SPICON_CLKPH.addKey( ii['key'],ii['value'], ii['desc'] )

    ## Slave select pin enable bit
    ssen_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_MSSEN, ssen_names)
    i2sSym_SPICON_MSSEN = i2sComponent.createKeyValueSetSymbol( "I2S_SPICON_MSSEN",None)
    i2sSym_SPICON_MSSEN.setLabel(i2sBitField_SPI1CON_MSSEN.getAttribute("caption"))
    i2sSym_SPICON_MSSEN.setDefaultValue(0)
    i2sSym_SPICON_MSSEN.setOutputMode( "Value" )
    i2sSym_SPICON_MSSEN.setVisible(False)
    i2sSym_SPICON_MSSEN.setReadOnly(True)           # value forced to 1 in ftl
    i2sSym_SPICON_MSSEN.setDisplayMode( "Description" )
    for ii in ssen_names:
        i2sSym_SPICON_MSSEN.addKey( ii['key'],ii['value'], ii['desc'] )

    ## SPI data width(Mode)
    mode_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_MODE, mode_names)
    i2sSym_SPICON_MODE = i2sComponent.createKeyValueSetSymbol( "I2S_SPICON_MODE",None)
    i2sSym_SPICON_MODE.setLabel(i2sBitField_SPI1CON_MODE.getAttribute("caption"))
    i2sSym_SPICON_MODE.setDefaultValue(2)
    i2sSym_SPICON_MODE.setOutputMode( "Value" )
    i2sSym_SPICON_MODE.setDisplayMode( "Description" )
    for ii in mode_names:
        i2sSym_SPICON_MODE.addKey( ii['key'],ii['value'], ii['desc'] )

    ## SPI Master clock
    msclk_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON_MCLKSEL, msclk_names)
    i2sSym_SPI1CON_MCLKSEL = i2sComponent.createKeyValueSetSymbol( "I2S_MASTER_CLOCK",None)
    i2sSym_SPI1CON_MCLKSEL.setLabel(i2sBitField_SPI1CON_MCLKSEL.getAttribute("caption"))
    i2sSym_SPI1CON_MCLKSEL.setDefaultValue(0)
    i2sSym_SPI1CON_MCLKSEL.setOutputMode( "Value" )
    i2sSym_SPI1CON_MCLKSEL.setReadOnly(True)           # value forced to 1 in ftl
    i2sSym_SPI1CON_MCLKSEL.setDisplayMode( "Description" )
    for ii in msclk_names:
        i2sSym_SPI1CON_MCLKSEL.addKey( ii['key'],ii['value'], ii['desc'] )

    i2sSym_Baud_Rate = i2sComponent.createIntegerSymbol("I2S_BAUD_RATE", None)
    i2sSym_Baud_Rate.setLabel("Baud Rate in Hz")
    i2sSym_Baud_Rate.setDefaultValue(48000)
    i2sSym_Baud_Rate.setMin(1)
    i2sSym_Baud_Rate.setMax(192000)
    i2sSym_Baud_Rate.setDependencies(i2sSPIBRG_ValueUpdate, ["I2S_BAUD_RATE"])

    i2sSym_MCLK_BCLK_Ratio = i2sComponent.createIntegerSymbol("I2S_MCLK_BCLK_RATIO", None)
    i2sSym_MCLK_BCLK_Ratio.setLabel("Master Clock/Bit Clock Ratio")
    i2sSym_MCLK_BCLK_Ratio.setDefaultValue(4)
    i2sSym_MCLK_BCLK_Ratio.setMin(1)
    i2sSym_MCLK_BCLK_Ratio.setMax(1024)
    i2sSym_MCLK_BCLK_Ratio.setDependencies(i2sSPIBRG_ValueUpdate, ["I2S_MCLK_BCLK_RATIO"])

    #SPI Baud Rate not supported comment
    global i2sSym_BaudError_Comment
    i2sSym_BaudError_Comment = i2sComponent.createCommentSymbol("I2S_BAUD_ERROR_COMMENT", None)
    i2sSym_BaudError_Comment.setLabel("********** WARNING!: Baud Rate is out of range **********")
    i2sSym_BaudError_Comment.setVisible(False)

    i2sxBRG = spiInstanceName.getValue() + "BRG"
    i2sxBRG_Bitfield = ATDF.getNode('/avr-tools-device-file/modules/module@[name="SPI"]/register-group@[name="SPI"]/register@[name="' + i2sxBRG + '"]/bitfield@[name="' + i2sxBRG + '"]')
    i2sMaxBRG = int(str(i2sxBRG_Bitfield.getAttribute("mask")), 0)

    global i2sSymMaxBRG
    i2sSymMaxBRG = i2sComponent.createIntegerSymbol("I2S_MAX_BRG", None)
    i2sSymMaxBRG.setDefaultValue(i2sMaxBRG)
    i2sSymMaxBRG.setVisible(False)

    # Baud Rate generation
    defaultSPIBR = i2sCalculateBRGValue(i2sSym_MCLK_BCLK_Ratio.getValue(), i2sSym_Baud_Rate.getValue())
    
    global i2sSym_SPIBRG_VALUE
    i2sSym_SPIBRG_VALUE = i2sComponent.createIntegerSymbol("I2S_BRG_VALUE", None)
    i2sSym_SPIBRG_VALUE.setVisible(False)
    i2sSym_SPIBRG_VALUE.setValue(defaultSPIBR, 1)

    ###################################################################################################
    ####################################### Driver Symbols ############################################
    ###################################################################################################

    #SPI 8-bit Character size Mask
    i2sSym_CHSIZE_8BIT = i2sComponent.createStringSymbol("I2S_CHARSIZE_BITS_8_BIT_MASK", None)
    i2sSym_CHSIZE_8BIT.setDefaultValue("0x00000000")
    i2sSym_CHSIZE_8BIT.setVisible(False)

    #SPI 16-bit Character size Mask
    i2sSym_CHSIZE_16BIT = i2sComponent.createStringSymbol("I2S_CHARSIZE_BITS_16_BIT_MASK", None)
    i2sSym_CHSIZE_16BIT.setDefaultValue("0x00000400")
    i2sSym_CHSIZE_16BIT.setVisible(False)

    #SPI 32-bit Character size Mask
    i2sSym_CHSIZE_32BIT = i2sComponent.createStringSymbol("I2S_CHARSIZE_BITS_32_BIT_MASK", None)
    i2sSym_CHSIZE_32BIT.setDefaultValue("0x00000800")
    i2sSym_CHSIZE_32BIT.setVisible(False)

    #SPI Clock Phase Leading Edge Mask
    i2sSym_CPHA_LE_Mask = i2sComponent.createStringSymbol("I2S_CLOCK_PHASE_LEADING_MASK", None)
    i2sSym_CPHA_LE_Mask.setDefaultValue("0x00000100")
    i2sSym_CPHA_LE_Mask.setVisible(False)

    #SPI Clock Phase Trailing Edge Mask
    i2sSym_CPHA_TE_Mask = i2sComponent.createStringSymbol("I2S_CLOCK_PHASE_TRAILING_MASK", None)
    i2sSym_CPHA_TE_Mask.setDefaultValue("0x00000000")
    i2sSym_CPHA_TE_Mask.setVisible(False)

    #SPI Clock Polarity Idle Low Mask
    i2sSym_CPOL_IL_Mask = i2sComponent.createStringSymbol("I2S_CLOCK_POLARITY_LOW_MASK", None)
    i2sSym_CPOL_IL_Mask.setDefaultValue("0x00000000")
    i2sSym_CPOL_IL_Mask.setVisible(False)

    #SPI Clock Polarity Idle High Mask
    i2sSym_CPOL_IH_Mask = i2sComponent.createStringSymbol("I2S_CLOCK_POLARITY_HIGH_MASK", None)
    i2sSym_CPOL_IH_Mask.setDefaultValue("0x00000040")
    i2sSym_CPOL_IH_Mask.setVisible(False)

    #SPI API Prefix
    i2sSym_API_Prefix = i2sComponent.createStringSymbol("I2S_PLIB_API_PREFIX", None)
    i2sSym_API_Prefix.setDefaultValue(spiInstanceName.getValue())
    i2sSym_API_Prefix.setVisible(False)

    #SPI Transmit data register
    transmitRegister = i2sComponent.createStringSymbol("TRANSMIT_DATA_REGISTER", None)
    transmitRegister.setDefaultValue("&("+spiInstanceName.getValue()+"BUF)")
    transmitRegister.setVisible(False)

    #SPI Receive data register
    receiveRegister = i2sComponent.createStringSymbol("RECEIVE_DATA_REGISTER", None)
    receiveRegister.setDefaultValue("&("+spiInstanceName.getValue()+"BUF)")
    receiveRegister.setVisible(False)

    #I2S mode specific
    audmod_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON2_AUDMOD, audmod_names)
    i2sSym_SPI_SPICON2_MODE = i2sComponent.createKeyValueSetSymbol("I2S_SPICON2_MODE",None)
    i2sSym_SPI_SPICON2_MODE.setLabel(i2sBitField_SPI1CON2_AUDMOD.getAttribute("caption"))
    i2sSym_SPI_SPICON2_MODE.setDefaultValue(3)
    i2sSym_SPI_SPICON2_MODE.setOutputMode("Value")
    i2sSym_SPI_SPICON2_MODE.setDisplayMode("Description")
    for ii in audmod_names:
        i2sSym_SPI_SPICON2_MODE.addKey( ii['key'],ii['value'], ii['desc'] )

    audmono_names = []
    _get_bitfield_names(i2sValGrp_SPI1CON2_AUDMONO, audmono_names)
    i2sSym_SPI_SPICON2_MONO = i2sComponent.createKeyValueSetSymbol("I2S_SPICON2_MONO",None)
    i2sSym_SPI_SPICON2_MONO.setLabel(i2sBitField_SPI1CON2_AUDMONO.getAttribute("caption"))
    i2sSym_SPI_SPICON2_MONO.setDefaultValue(1)
    i2sSym_SPI_SPICON2_MONO.setOutputMode("Value")
    i2sSym_SPI_SPICON2_MONO.setDisplayMode("Description")
    for ii in audmono_names:
        i2sSym_SPI_SPICON2_MONO.addKey( ii['key'],ii['value'], ii['desc'] )

    i2sLRCPin = i2sComponent.createKeyValueSetSymbol("I2S_LRCLK_PIN_DEFINE", None)     # used for I2Sx_LRCLK_Get() macro
    i2sLRCPin.setLabel("Frame Select Pin")
    for i2sLRCPort in ["A","B","C","D","E","F","G","H"]:
        for i2sLRCPinNum in range(0,32):
            i2sLRCPinDesc = i2sLRCPort + str(i2sLRCPinNum)
            i2sLRCPinDefine = "((PORT" + str(i2sLRCPort) + " >> " + str(i2sLRCPinNum) + ") & 0x1)"
            i2sLRCPin.addKey(i2sLRCPinDesc, i2sLRCPinDefine, i2sLRCPinDesc)
    i2sLRCPin.setDisplayMode("Description")
    i2sLRCPin.setOutputMode("Value")

    i2sLRCINV = i2sComponent.createKeyValueSetSymbol("I2S_LRCLK_INVERT", i2sLRCPin)
    i2sLRCINV.setLabel("Invert")
    i2sLRCINV.addKey("0", "0", "Normal LRCLK_Get Return Value")
    i2sLRCINV.addKey("1", "1", "Invert LRCLK_Get Return Value")
    i2sLRCINV.setDisplayMode("Description")
    i2sLRCINV.setOutputMode("Value")
    i2sLRCINV.setDefaultValue(0)

    i2sSym_SPI_SPICON_FRMSYNC = i2sComponent.createIntegerSymbol("I2S_SPICON_FRMSYNC", None)
    i2sSym_SPI_SPICON_FRMSYNC.setDefaultValue(0)
    i2sSym_SPI_SPICON_FRMSYNC.setVisible(False)

    i2sSym_SPI_SPICON_FRMPOL = i2sComponent.createIntegerSymbol("I2S_SPICON_FRMPOL", None)
    i2sSym_SPI_SPICON_FRMPOL.setDefaultValue(1)
    i2sSym_SPI_SPICON_FRMPOL.setVisible(False)

    i2sSym_SPI_SPICON_FRMCNT = i2sComponent.createIntegerSymbol("I2S_SPICON_FRMCNT", None)
    i2sSym_SPI_SPICON_FRMCNT.setDefaultValue(4)
    i2sSym_SPI_SPICON_FRMCNT.setVisible(False)

    i2sSym_SPI_SPICON_SPIFE = i2sComponent.createIntegerSymbol("I2S_SPICON_SPIFE", None)
    i2sSym_SPI_SPICON_SPIFE.setDefaultValue(0)
    i2sSym_SPI_SPICON_SPIFE.setVisible(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    i2sHeaderFile = i2sComponent.createFileSymbol("I2S_HEADER", None)
    i2sHeaderFile.setSourcePath("../peripheral/spi_01329/templates/plib_i2s.h.ftl")
    i2sHeaderFile.setOutputName("plib_" + i2sInstanceName.getValue().lower() + ".h")
    i2sHeaderFile.setDestPath("audio/peripheral/i2s/")
    i2sHeaderFile.setProjectPath("config/" + configName + "/audio/peripheral/i2s")
    i2sHeaderFile.setType("HEADER")
    i2sHeaderFile.setMarkup(True)

    i2sSourceFile = i2sComponent.createFileSymbol("SPI_I2S_SOURCE", None)
    i2sSourceFile.setSourcePath("../peripheral/spi_01329/templates/plib_i2s.c.ftl")
    i2sSourceFile.setOutputName("plib_" + i2sInstanceName.getValue().lower() + ".c")
    i2sSourceFile.setDestPath("audio/peripheral/i2s/")
    i2sSourceFile.setProjectPath("config/" + configName + "/audio/peripheral/i2s")
    i2sSourceFile.setType("SOURCE")
    i2sSourceFile.setMarkup(True)

    i2sSystemInitFile = i2sComponent.createFileSymbol("I2S_SYS_INIT", None)
    i2sSystemInitFile.setType("STRING")
    i2sSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    i2sSystemInitFile.setSourcePath("../peripheral/spi_01329/templates/system/initialization.c.ftl")
    i2sSystemInitFile.setMarkup(True)

    i2sSystemDefFile = i2sComponent.createFileSymbol("I2S_SYS_DEF", None)
    i2sSystemDefFile.setType("STRING")
    i2sSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    i2sSystemDefFile.setSourcePath("../peripheral/spi_01329/templates/system/definitions.h.ftl")
    i2sSystemDefFile.setMarkup(True)
