/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_macros.h

  Summary:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated definitions header based on output by the MPLAB Harmony
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

#ifndef _LIBARIA_INIT_H
#define _LIBARIA_INIT_H

#include "gfx/libaria/libaria.h"
#include "gfx/libaria/libaria_events.h"

#include "gfx/gfx_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

#define LIBARIA_SCREEN_COUNT   1

// reference IDs for generated libaria screens
// screen "GainLableWidget"
#define GainLableWidget_ID    0



extern laScheme defaultScheme;
extern laScheme whiteText;
extern laScheme NewScheme;
extern laScheme ProgressBar;
extern laImageWidget* BackgroundImageWidget;
extern laImageWidget* MicrochipImageWidget;
extern laLabelWidget* AppTitleWidget;
extern laLabelWidget* lblSampleRate;
extern laLabelWidget* lblChannels;
extern laLabelWidget* lblBitDepth;
extern laCircularSliderWidget* VolumeWidget;
extern laLabelWidget* lblVolume;
extern laImageWidget* HeadPhoneEnableImg;
extern laLabelWidget* HPEnableLabelWidget;
extern laButtonWidget* HPEnableBtn;
extern laButtonWidget* HPDisableBtnWidget;
extern laLabelWidget* txtSampleRate;
extern laLabelWidget* txtChannels;
extern laLabelWidget* txtBitDepth;
extern laLabelWidget* txtVolumePercent;
extern laButtonWidget* RewindBtn;
extern laButtonWidget* NextTrackBtn;
extern laProgressBarWidget* ProgressBarWidget;
extern laLabelWidget* txtEndPosition;
extern laLabelWidget* txtCurrentPosition;
extern laButtonWidget* HiddenPlayStopBtn;
extern laImageWidget* PlayStopBtn;
extern laLabelWidget* lblFilename;
extern laLabelWidget* lblArtist;
extern laLabelWidget* lblTrackTitle;
extern laLabelWidget* lblAlbumTitle;
extern laLabelWidget* txtArtist;
extern laLabelWidget* txtFilename;
extern laLabelWidget* txtAlbumTitle;
extern laLabelWidget* txtTrackTitle;





int32_t libaria_initialize(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _LIBARIA_INIT_H
/*******************************************************************************
 End of File
*/
