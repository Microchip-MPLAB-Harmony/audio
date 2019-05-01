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

#define LIBARIA_SCREEN_COUNT   2

// reference IDs for generated libaria screens
// screen "default"
#define default_ID    0

// screen "trigger"
#define trigger_ID    1



extern laScheme defaultScheme;
extern laScheme textFieldScheme;
extern laScheme titleScheme;
extern laScheme whiteText;
extern laScheme backgroundScheme;
extern laScheme defaultPaletteScheme;
extern laImageWidget* backgroundImage;
extern laLineWidget* LineWidget2;
extern laButtonWidget* SinewaveButton;
extern laButtonWidget* SquarewaveButton;
extern laButtonWidget* SawtoothButton;
extern laButtonWidget* TriangleButton;
extern laButtonWidget* HiddenSinewaveButton;
extern laButtonWidget* HiddenSquarewaveButton;
extern laButtonWidget* HiddenSawtoothButton;
extern laButtonWidget* HiddenTriangleButton;
extern laLineWidget* HorizLine;
extern laLineWidget* VertLine;
extern laLabelWidget* LabelWidget5;
extern laLabelWidget* EndFreqText;
extern laCircularSliderWidget* CircularVolume;
extern laLabelWidget* LabelWidget12;
extern laLabelWidget* LabelWidget14;
extern laCheckBoxWidget* SweepCheckBox;
extern laCheckBoxWidget* LoopCheckBox;
extern laButtonWidget* HiddenSweepButton;
extern laButtonWidget* HiddenLoopButton;
extern laButtonWidget* StartFreqPlusButton;
extern laButtonWidget* StartFreqMinusButton;
extern laButtonWidget* HiddenStartFreqPlusButton;
extern laButtonWidget* HiddenStartFreqMinusButton;
extern laButtonWidget* EndFreqPlusButton;
extern laButtonWidget* EndFreqMinusButton;
extern laButtonWidget* DurationPlusButton;
extern laButtonWidget* DurationMinusButton;
extern laButtonWidget* PlayPauseButton;
extern laTextFieldWidget* Title;
extern laTextFieldWidget* TitleWaveType;
extern laButtonWidget* OutputTriggerButton;
extern laButtonWidget* HiddenEndFreqPlusButton;
extern laButtonWidget* HiddenEndFreqMinusButton;
extern laButtonWidget* HiddenDurationPlusButton;
extern laButtonWidget* HiddenDurationMinusButton;
extern laButtonWidget* HiddenPlayPauseButton;
extern laLabelWidget* StartFreqLabel;
extern laLabelWidget* EndFreqLabel;
extern laLabelWidget* DurationLabel;
extern laLabelWidget* volumePercent;
extern laImageWidget* ImageWidget3;
extern laImageWidget* backgroundImage2;
extern laButtonWidget* SinewaveButton2;
extern laButtonWidget* SquarewaveButton2;
extern laButtonWidget* SawtoothButton2;
extern laButtonWidget* TriangleButton2;
extern laLineWidget* LineWidget36;
extern laLineWidget* LineWidget37;
extern laLabelWidget* LabelWidget54;
extern laLabelWidget* LabelWidget53;
extern laButtonWidget* DelayPlusButton;
extern laButtonWidget* DelayMinusButton;
extern laSliderWidget* DelaySlider;
extern laButtonWidget* SignalGeneratorButton;
extern laLabelWidget* DelayLabel;
extern laButtonWidget* HiddenDelayPlusButton;
extern laButtonWidget* HiddenDelayMinusButton;
extern laButtonWidget* PulseDurUpButton;
extern laButtonWidget* PulseDurDownButton;
extern laLabelWidget* PulseDurationLabel;
extern laButtonWidget* HiddenPulseDurUpButton;
extern laButtonWidget* HiddenPulseDurDownButton;
extern laButtonWidget* HiddenSinewaveButton2;
extern laButtonWidget* HiddenSquarewaveButton2;
extern laButtonWidget* HiddenSawtoothButton2;
extern laButtonWidget* HiddenTriangleButton2;
extern laTextFieldWidget* TextFieldWidget12;
extern laLineWidget* LineWidget1;
extern laImageWidget* ImageWidget4;





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
