"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

import os.path

# Capability of different peripherals that higher level layers depends on
peripherals = {
                "I2S_U2224"     : ["I2S"],
                "I2SC_11241"    : ["I2S"],
                "SPI_01329"     : ["I2S"],
                "SSC_6078"      : ["I2S"]
}

periphNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals")
modules = periphNode.getChildren()

for module in range (0, len(modules)):

    periphName = str(modules[module].getAttribute("name"))
    periphID = str(modules[module].getAttribute("id"))
    periphScript = periphName.lower() + "_" + periphID.lower() + \
                    "/config/" + periphName.lower() + ".py"

    # Check if peripheral has implementation
    if (os.path.isfile(Module.getPath()+periphScript)):

        instances = modules[module].getChildren()
        for instance in range (0, len(instances)):

            periphInstanceName = str(instances[instance].getAttribute("name"))
            if periphName=="SPI" and periphID=="01329":     # special case for MZ/MX where SPI and I2S are same peripheral
                periphInstanceNameLower = periphInstanceName.lower().replace("spi","i2s")
                periphInstanceNameUpper = periphInstanceName.upper().replace("SPI","I2S")
                periphNameUpper =  periphName.upper().replace("SPI","I2S")
            else:
                periphInstanceNameLower = periphInstanceName.lower()
                periphInstanceNameUpper = periphInstanceName.upper()
                periphNameUpper =  periphName.upper()

            print("Audio: create component: Peripheral " + periphInstanceNameUpper + " (ID = " + periphID + ")")

            periphComponent = Module.CreateComponent("a_" + periphInstanceNameLower, periphInstanceNameUpper, "/Audio/Peripherals/" + periphNameUpper + "/", "./" + periphScript)
            periphComponent.setDisplayType("Peripheral Library")

            key = periphName + "_" + periphID

            if key in peripherals:
                for capablity in peripherals[key]:
                    capablityId = periphInstanceName + "_" + capablity
                    periphComponent.addCapability(capablityId, capablity)