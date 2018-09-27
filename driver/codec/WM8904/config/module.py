def loadModule():
    component = Module.CreateComponent("audio_codec_wm8904", "WM8904", "/Audio/Driver/Codec/", "config/wm8904.py")
    component.setDisplayType("Driver")

    component.addDependency("DRV_I2C", "DRV_I2C", False)
    component.addDependency("DRV_I2S", "DRV_I2S", False)
    component.addCapability("audio_codec", "Audio Codec", False)
