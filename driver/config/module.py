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

def loadModule():
    #check if I2S driver should be created
    if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70", "SAMD5", "SAME5", "PIC32MX", "PIC32MZ", "SAMD21"]):
        Name        = "i2s"
        print("Audio: create component: " + Name.upper() + " Driver")

        actualPath  = "driver/"
        displayPath = "/Audio/Drivers/"

        Component = Module.CreateGeneratorComponent("a_drv_" + Name, "I2S", "/Audio/Drivers/", Name + "/config/drv_" + Name + "_common.py", Name + "/config/drv_" + Name + ".py")

        Component.addCapability("DRV_I2S".lower(), "DRV_I2S")
        Component.addDependency("drv_" + Name + "_" + "I2S" + "_dependency", "I2S", False, True)

        Component.setDisplayType("Driver")

        # Add Generic Dependency on Core Service
        Component.addDependency("drv_" + Name + "_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

    component = Module.CreateComponent("audio_codec_ak4953", "AK4953", "/Audio/Drivers/Codec/", "codec/AK4953/config/ak4953.py")
    component.setDisplayType("Driver")
    component.addDependency("DRV_I2C", "DRV_I2C", False)
    component.addDependency("DRV_I2S", "DRV_I2S", False)
    component.addCapability("audio_codec", "Audio Codec", False)

    component = Module.CreateComponent("audio_codec_ak4954", "AK4954", "/Audio/Drivers/Codec/", "codec/AK4954/config/ak4954.py")
    component.setDisplayType("Driver")
    component.addDependency("DRV_I2C", "DRV_I2C", False)
    component.addDependency("DRV_I2S", "DRV_I2S", False)
    component.addCapability("audio_codec", "Audio Codec", False)

    component = Module.CreateComponent("audio_codec_generic", "Generic Codec", "/Audio/Drivers/Codec/", "codec/Generic/config/genericcodec.py")
    component.setDisplayType("Driver")
    component.addDependency("DRV_I2C", "DRV_I2C", False)
    component.addDependency("DRV_I2S", "DRV_I2S", False)
    component.addCapability("audio_codec", "Audio Codec", False)

    component = Module.CreateComponent("audio_codec_wm8904", "WM8904", "/Audio/Drivers/Codec/", "codec/WM8904/config/wm8904.py")
    component.setDisplayType("Driver")
    component.addDependency("DRV_I2C", "DRV_I2C", False)
    component.addDependency("DRV_I2S", "DRV_I2S", False)
    component.addCapability("audio_codec", "Audio Codec", False)
