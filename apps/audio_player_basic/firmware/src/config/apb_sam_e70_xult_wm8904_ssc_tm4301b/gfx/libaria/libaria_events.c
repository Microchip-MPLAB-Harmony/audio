/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_events.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
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
*******************************************************************************/
// DOM-IGNORE-END

#include "gfx/libaria/libaria_events.h"
#include "app.h"

// PlayStopBtn - PressedEvent
void PlayStopBtn_PressedEvent(laButtonWidget* btn)
{
    // Start playback
    appData.pause = false;
}

// VolumeWidget - ValueChangedEvent
void VolumeWidget_ValueChangedEvent(laCircularSliderWidget * slider, int32_t value)
{
}

// VolumeWidget - ReleasedEvent
void VolumeWidget_ReleasedEvent(laCircularSliderWidget * slider, int32_t value)
{
}

// HPEnableBtn - PressedEvent
void HPEnableBtn_PressedEvent(laButtonWidget* btn)
{
    // Set Press State (HPDisableBtnWidget) - Set Press State - HPDisableBtnWidget
    laButtonWidget_SetPressed((laButtonWidget*)HPDisableBtnWidget, LA_FALSE);

    // Set Image (HeadPhoneEnableImg) - Set Image - HeadPhoneEnableImg
    laImageWidget_SetImage((laImageWidget*)HeadPhoneEnableImg, &toggleon61x31);

    // Enable Head Phone Output
    if( !appData.headphone_out )
    {
    	appData.headphone_out = true;
    	DRV_CODEC_MuteOff( appData.codecData.handle );
    }
}

// HPDisableBtnWidget - PressedEvent
void HPDisableBtnWidget_PressedEvent(laButtonWidget* btn)
{
    // Set Press State (HPEnableBtnWidget) - Set Press State - HPEnableBtn
    laButtonWidget_SetPressed((laButtonWidget*)HPEnableBtn, LA_FALSE);

    // Set Image (HeadPhoneEnableImg) - Set Image - HeadPhoneEnableImg
    laImageWidget_SetImage((laImageWidget*)HeadPhoneEnableImg, &toggleoff61x31);

    // Disable Head Phone Output
    if( appData.headphone_out )
    {
    	appData.headphone_out = false;
    	DRV_CODEC_MuteOn( appData.codecData.handle );
    }
}

// RewindBtn - PressedEvent
void RewindBtn_PressedEvent(laButtonWidget* btn)
{
    // Seek to start of file
    SYS_FS_FileSeek(appData.fileHandle, sizeof(WAV_FILE_HEADER), SYS_FS_SEEK_SET);
}

// NextTrackBtn - PressedEvent
void NextTrackBtn_PressedEvent(laButtonWidget* btn)
{
    // Seek to end and close the file
    SYS_FS_FileSeek( appData.fileHandle, 0, SYS_FS_SEEK_END);
}

// PauseBtn - PressedEvent
void PauseBtn_PressedEvent(laButtonWidget* btn)
{
    // Pause playback
    appData.pause = true;
}





