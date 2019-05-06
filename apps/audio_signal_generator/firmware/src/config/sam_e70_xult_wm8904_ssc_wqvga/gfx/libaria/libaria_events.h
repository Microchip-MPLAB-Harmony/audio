/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_events.h

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

#ifndef _LIBARIA_EVENTS_H
#define _LIBARIA_EVENTS_H

#include "gfx/libaria/libaria.h"
#include "gfx/libaria/libaria_init.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// Generated Event Handler - Origin: HiddenSinewaveButton, Event: PressedEvent
void HiddenSinewaveButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenSquarewaveButton, Event: PressedEvent
void HiddenSquarewaveButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenSawtoothButton, Event: PressedEvent
void HiddenSawtoothButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenTriangleButton, Event: PressedEvent
void HiddenTriangleButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: CircularVolume, Event: ValueChangedEvent
void CircularVolume_ValueChangedEvent(laCircularSliderWidget * slider, int32_t value);

// Generated Event Handler - Origin: CircularVolume, Event: PressedEvent
void CircularVolume_PressedEvent(laCircularSliderWidget * slider, int32_t value);

// Generated Event Handler - Origin: CircularVolume, Event: ReleasedEvent
void CircularVolume_ReleasedEvent(laCircularSliderWidget * slider, int32_t value);

// Generated Event Handler - Origin: HiddenSweepButton, Event: PressedEvent
void HiddenSweepButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenLoopButton, Event: PressedEvent
void HiddenLoopButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenStartFreqPlusButton, Event: PressedEvent
void HiddenStartFreqPlusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenStartFreqPlusButton, Event: ReleasedEvent
void HiddenStartFreqPlusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenStartFreqMinusButton, Event: PressedEvent
void HiddenStartFreqMinusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenStartFreqMinusButton, Event: ReleasedEvent
void HiddenStartFreqMinusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: OutputTriggerButton, Event: PressedEvent
void OutputTriggerButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: OutputTriggerButton, Event: ReleasedEvent
void OutputTriggerButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenEndFreqPlusButton, Event: PressedEvent
void HiddenEndFreqPlusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenEndFreqPlusButton, Event: ReleasedEvent
void HiddenEndFreqPlusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenEndFreqMinusButton, Event: PressedEvent
void HiddenEndFreqMinusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenEndFreqMinusButton, Event: ReleasedEvent
void HiddenEndFreqMinusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDurationPlusButton, Event: PressedEvent
void HiddenDurationPlusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDurationPlusButton, Event: ReleasedEvent
void HiddenDurationPlusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDurationMinusButton, Event: PressedEvent
void HiddenDurationMinusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDurationMinusButton, Event: ReleasedEvent
void HiddenDurationMinusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPlayPauseButton, Event: PressedEvent
void HiddenPlayPauseButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPlayPauseButton, Event: ReleasedEvent
void HiddenPlayPauseButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: DelaySlider, Event: ValueChangedEvent
void DelaySlider_ValueChangedEvent(laSliderWidget* sld);

// Generated Event Handler - Origin: SignalGeneratorButton, Event: PressedEvent
void SignalGeneratorButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SignalGeneratorButton, Event: ReleasedEvent
void SignalGeneratorButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDelayPlusButton, Event: PressedEvent
void HiddenDelayPlusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDelayPlusButton, Event: ReleasedEvent
void HiddenDelayPlusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDelayMinusButton, Event: PressedEvent
void HiddenDelayMinusButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenDelayMinusButton, Event: ReleasedEvent
void HiddenDelayMinusButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPulseDurUpButton, Event: PressedEvent
void HiddenPulseDurUpButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPulseDurUpButton, Event: ReleasedEvent
void HiddenPulseDurUpButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPulseDurDownButton, Event: PressedEvent
void HiddenPulseDurDownButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenPulseDurDownButton, Event: ReleasedEvent
void HiddenPulseDurDownButton_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenSinewaveButton2, Event: PressedEvent
void HiddenSinewaveButton2_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenSquarewaveButton2, Event: PressedEvent
void HiddenSquarewaveButton2_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenSawtoothButton2, Event: PressedEvent
void HiddenSawtoothButton2_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: HiddenTriangleButton2, Event: PressedEvent
void HiddenTriangleButton2_PressedEvent(laButtonWidget* btn);

// KEEP THESE LINES:
#define LIBARIA_EVENTS_DEFINED  1       // if file is regenerated via Bamboo, this will remain undefined
void InitLabels( void );      // routine in libaria_events.c called from app.c
void KeyRepeatTask( void );   // routine in libaria_events.c called from app.c


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _LIBARIA_EVENTS_H
/*******************************************************************************
 End of File
*/
