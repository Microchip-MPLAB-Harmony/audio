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

// KEEP ALL LINES BELOW
// locals
int32_t _msSinceKeyPressed;
int32_t _msSinceVolumePressed;
uint32_t* _repeatValue;
uint32_t _repeatIncOrDec;
uint32_t _repeatMinOrMaxValue;
laLabelWidget* _repeatLabel;
bool _repeatPercent;
int32_t latestVolume;
laSliderWidget* _repeatSlider;

void _SetNumericLabel(uint32_t value, laLabelWidget* label, bool percent)
{
    char valueStrBuf[5];     // enough for 100%
    laString tempStr;
    
    if (percent)
    {
        sprintf(valueStrBuf,"%4d%%",(int)value);
    }
    else
    {
        sprintf(valueStrBuf,"%4d",(int)value);
    }
    tempStr = laString_CreateFromCharBuffer((char *) valueStrBuf, &LiberationSans18);
    laLabelWidget_SetText(label, tempStr);
    laString_Destroy(&tempStr); 
}

void _setEndFreqVisible()
{
    laWidget_SetVisible((laWidget*)EndFreqPlusButton, appData.sweepEnabled);
    laWidget_SetVisible((laWidget*)EndFreqMinusButton, appData.sweepEnabled);
    laWidget_SetVisible((laWidget*)EndFreqLabel, appData.sweepEnabled);
    laWidget_SetVisible((laWidget*)EndFreqText, appData.sweepEnabled);            
}

void _SetTitleType(char* titleType)
{
    char titleStrBuf[20];
    strcpy(titleStrBuf,"- ");
    strcat(titleStrBuf,titleType);
    laString tempStr;
    tempStr = laString_CreateFromCharBuffer((char *) titleStrBuf, &LiberationSans18Bold);
    laTextFieldWidget_SetText(TitleWaveType, tempStr);
    laString_Destroy(&tempStr);      
}

void InitLabels( void )
{
    _SetTitleType("Sine Wave");
    
    _msSinceKeyPressed = -1;
    _msSinceVolumePressed = -1;   
    appData.startFrequency = 1000;
    _SetNumericLabel(appData.startFrequency, StartFreqLabel, false);
    appData.endFrequency = 3000;
    _SetNumericLabel(appData.endFrequency, EndFreqLabel, false);
    _SetNumericLabel(25, volumePercent, true);
    appData.duration = 1200;    
    _SetNumericLabel(appData.duration, DurationLabel, false);
    appData.pulseDelay = 0;
    _SetNumericLabel(appData.pulseDelay, DelayLabel, true);    
    appData.pulseDurIdx = 0;    
    _SetNumericLabel(pulseDurations[appData.pulseDurIdx], PulseDurationLabel, false);
    
    laCheckBoxWidget_SetChecked(SweepCheckBox, appData.sweepEnabled);
    laCheckBoxWidget_SetChecked(LoopCheckBox, appData.loopEnabled);   
    laButtonWidget_SetReleasedImage(PlayPauseButton, &AudioPlay);    
    _setEndFreqVisible();
}

void KeyRepeatTask( void )        // called every ms from app.c
{   
    if (_msSinceKeyPressed >= 0)
    {
        _msSinceKeyPressed++;
        
        if ((_msSinceKeyPressed % 100)==0)       // auto inc/dec 10 times per sec
        {        
            int incDecAmt = 0;
            //if (_msSinceKeyPressed > 2000)       // after 3 1/2 sec = inc/dec by 1000
            //{
            //    incDecAmt = 1000;
            //}
            //else
            if (_msSinceKeyPressed > 1000)  // after 2 1/2 sec = inc/dec by 100
            {
                incDecAmt = 100;    
            }
            else if (_msSinceKeyPressed > 200)  // after 200 ms = inc/dec by 10
            {
                incDecAmt = 10;
            }

            if (incDecAmt)
            {
                if (_repeatIncOrDec)
                {
                    if (*_repeatValue < (_repeatMinOrMaxValue-incDecAmt))
                    {
                        *_repeatValue += incDecAmt;
                        *_repeatValue = incDecAmt*(*_repeatValue/incDecAmt);
                    }
                    else
                    {
                        *_repeatValue = _repeatMinOrMaxValue;
                    }
                    _SetNumericLabel(*_repeatValue, _repeatLabel, _repeatPercent); 
                }
                else
                {
                    if (*_repeatValue > (_repeatMinOrMaxValue+incDecAmt))
                    {
                        if ((*_repeatValue%incDecAmt)!=0)
                        {
                            *_repeatValue = incDecAmt*(*_repeatValue/incDecAmt);                        
                        }
                        else
                        {
                            *_repeatValue -= incDecAmt;
                        }                        
                    }
                    else
                    {
                        *_repeatValue = _repeatMinOrMaxValue;
                    }
                    _SetNumericLabel(*_repeatValue, _repeatLabel, _repeatPercent); 
                }
                if (_repeatSlider != NULL)
                {
                    laSliderWidget_SetSliderPercentage(_repeatSlider, *_repeatValue);                    
                }
            }
        }
    }
    if (_msSinceVolumePressed >= 0)
    {
        _msSinceVolumePressed++;        
        
        if ((_msSinceVolumePressed % 100)==0)       // update 10 times per sec
        {          
            appData.newVolume = latestVolume;
            _SetNumericLabel(appData.newVolume, volumePercent, true);            
        }
    }        
}

    
void _processKeyPressed(uint32_t* value, bool incOrDec, uint32_t minMaxValue, laLabelWidget* label, bool percent, laSliderWidget* slider)
{
    _msSinceKeyPressed = 0;
    _repeatValue = value;
    _repeatIncOrDec = incOrDec;
    _repeatMinOrMaxValue = minMaxValue;
    _repeatLabel = label;
    _repeatPercent = percent;
    _repeatSlider = slider;
    
    if (incOrDec)
    {
        if (*value < minMaxValue)
        {
            *value += 10;
            _SetNumericLabel(*value, label, percent); 
        }
    }
    else
    {
        if (*value > minMaxValue)
        {
            *value -= 10;
            _SetNumericLabel(*value, label, percent); 
        }    
    }
    if (slider != NULL)
    {
        laSliderWidget_SetSliderPercentage(slider,*value);                    
    }    
}

void _resetKeyPressed()
{
    _msSinceKeyPressed = -1;
}

// in order for buttons on both screens to be pressed, the screens must be checked
// as Persistent in the Graphics Composer

void _SetWaveButtons(laBool sinewave, laBool squarewave, laBool sawtooth, laBool triangle)
{
    laButtonWidget_SetPressed(SinewaveButton, sinewave);    
    laButtonWidget_SetPressed(SquarewaveButton, squarewave);
    laButtonWidget_SetPressed(SawtoothButton, sawtooth);
    laButtonWidget_SetPressed(TriangleButton, triangle);
    
    laButtonWidget_SetPressed(SinewaveButton2, sinewave);    
    laButtonWidget_SetPressed(SquarewaveButton2, squarewave);
    laButtonWidget_SetPressed(SawtoothButton2, sawtooth);
    laButtonWidget_SetPressed(TriangleButton2, triangle);    
}

// HiddenSinewaveButton - PressedEvent
void HiddenSinewaveButton_PressedEvent(laButtonWidget* btn)
{
    _SetWaveButtons(LA_TRUE,LA_FALSE,LA_FALSE,LA_FALSE); 
    _SetTitleType("Sine Wave");   
    appData.wave_mode = SINE_WAVE;
    appData.freqChange = true;
}

// HiddenSquarewaveButton - PressedEvent
void HiddenSquarewaveButton_PressedEvent(laButtonWidget* btn)
{
    _SetWaveButtons(LA_FALSE,LA_TRUE,LA_FALSE,LA_FALSE);
    _SetTitleType("Square Wave");   
    appData.wave_mode = SQUARE_WAVE;
    appData.freqChange = true;
}

// HiddenSawtoothButton - PressedEvent
void HiddenSawtoothButton_PressedEvent(laButtonWidget* btn)
{
    _SetWaveButtons(LA_FALSE,LA_FALSE,LA_TRUE,LA_FALSE);
    _SetTitleType("Sawtooth Wave");   
    appData.wave_mode = SAWTOOTH_WAVE;
    appData.freqChange = true;
}

// HiddenTriangleButton - PressedEvent
void HiddenTriangleButton_PressedEvent(laButtonWidget* btn)
{
    _SetWaveButtons(LA_FALSE,LA_FALSE,LA_FALSE,LA_TRUE);
    _SetTitleType("Triangle Wave"); 
    appData.wave_mode = TRIANGLE_WAVE;
    appData.freqChange = true;
}

// CircularVolume - ValueChangedEvent
void CircularVolume_ValueChangedEvent(laCircularSliderWidget * slider, int32_t value)
{
    _msSinceVolumePressed = 0;    
    latestVolume = value;
}

// CircularVolume - PressedEvent
void CircularVolume_PressedEvent(laCircularSliderWidget * slider, int32_t value)
{
    _msSinceVolumePressed = 0;
    appData.newVolume = latestVolume = value;
    _SetNumericLabel(appData.newVolume, volumePercent, true);    
}

// CircularVolume - ReleasedEvent
void CircularVolume_ReleasedEvent(laCircularSliderWidget * slider, int32_t value)
{
    appData.newVolume = value;
    _msSinceVolumePressed = -1;
    _SetNumericLabel(appData.newVolume, volumePercent, true);    
}

// HiddenSweepButton - PressedEvent
void HiddenSweepButton_PressedEvent(laButtonWidget* btn)
{
    appData.sweepEnabled = !appData.sweepEnabled;
    laCheckBoxWidget_SetChecked(SweepCheckBox, appData.sweepEnabled);
    _setEndFreqVisible();
}

// HiddenLoopButton - PressedEvent
void HiddenLoopButton_PressedEvent(laButtonWidget* btn)
{
    appData.loopEnabled = !appData.loopEnabled;
    laCheckBoxWidget_SetChecked(LoopCheckBox, appData.loopEnabled);        
}

// HiddenStartFreqPlusButton - PressedEvent
void HiddenStartFreqPlusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(StartFreqPlusButton, LA_TRUE);
    _processKeyPressed(&appData.startFrequency,true,6000,StartFreqLabel,false,NULL);
}

// HiddenStartFreqPlusButton - ReleasedEvent
void HiddenStartFreqPlusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(StartFreqPlusButton, LA_FALSE);
}

// HiddenStartFreqMinusButton - PressedEvent
void HiddenStartFreqMinusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(StartFreqMinusButton, LA_TRUE);    
    _processKeyPressed(&appData.startFrequency,false,50,StartFreqLabel,false,NULL);    
}

// HiddenStartFreqMinusButton - ReleasedEvent
void HiddenStartFreqMinusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(StartFreqMinusButton, LA_FALSE);    
}

// OutputTriggerButton - PressedEvent
void OutputTriggerButton_PressedEvent(laButtonWidget* btn)
{
}

// OutputTriggerButton - ReleasedEvent
void OutputTriggerButton_ReleasedEvent(laButtonWidget* btn)
{
    laContext_SetActiveScreen(trigger_ID); 
}

// HiddenEndFreqPlusButton - PressedEvent
void HiddenEndFreqPlusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(EndFreqPlusButton, LA_TRUE);    
    _processKeyPressed(&appData.endFrequency,true,6000,EndFreqLabel,false,NULL);     
}

// HiddenEndFreqPlusButton - ReleasedEvent
void HiddenEndFreqPlusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(EndFreqPlusButton, LA_FALSE);     
}

// HiddenEndFreqMinusButton - PressedEvent
void HiddenEndFreqMinusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(EndFreqMinusButton, LA_TRUE);     
    _processKeyPressed(&appData.endFrequency,false,50,EndFreqLabel,false,NULL);     
}

// HiddenEndFreqMinusButton - ReleasedEvent
void HiddenEndFreqMinusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(EndFreqMinusButton, LA_FALSE);     
}

// HiddenDurationPlusButton - PressedEvent
void HiddenDurationPlusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(DurationPlusButton, LA_TRUE);     
    _processKeyPressed(&appData.duration,true,20000,DurationLabel,false,NULL);    
}

// HiddenDurationPlusButton - ReleasedEvent
void HiddenDurationPlusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(DurationPlusButton, LA_FALSE);     
}

// HiddenDurationMinusButton - PressedEvent
void HiddenDurationMinusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(DurationMinusButton, LA_TRUE);     
    _processKeyPressed(&appData.duration,false,0,DurationLabel,false,NULL);   
}

// HiddenDurationMinusButton - ReleasedEvent
void HiddenDurationMinusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(DurationMinusButton, LA_FALSE);      
}

// HiddenPlayPauseButton - PressedEvent
void HiddenPlayPauseButton_PressedEvent(laButtonWidget* btn)
{
    if (appData.playOrPaused)
    {
        laButtonWidget_SetPressedImage(PlayPauseButton, &AudioMutePressed);        
    }
    else
    {
        laButtonWidget_SetPressedImage(PlayPauseButton, &AudioPlayPressed);
    }
    laButtonWidget_SetPressed(PlayPauseButton, LA_TRUE);
}

// HiddenPlayPauseButton - ReleasedEvent
void HiddenPlayPauseButton_ReleasedEvent(laButtonWidget* btn)
{
    // don't use btn argument, passed as NULL in app.c call
    
    appData.playOrPaused = !appData.playOrPaused;
    if (appData.playOrPaused)
    {
        laButtonWidget_SetReleasedImage(PlayPauseButton, &AudioMute);        
    }
    else
    {
        laButtonWidget_SetReleasedImage(PlayPauseButton, &AudioPlay);
    }    
    laButtonWidget_SetPressed(PlayPauseButton, LA_FALSE);    
}

// DelaySlider - ValueChangedEvent
void DelaySlider_ValueChangedEvent(laSliderWidget* sld)
{
    appData.pulseDelay = laSliderWidget_GetSliderPercentage(sld);
    _SetNumericLabel(appData.pulseDelay, DelayLabel, true);    
}

// SignalGeneratorButton - PressedEvent
void SignalGeneratorButton_PressedEvent(laButtonWidget* btn)
{
}

// SignalGeneratorButton - ReleasedEvent
void SignalGeneratorButton_ReleasedEvent(laButtonWidget* btn)
{
    laContext_SetActiveScreen(default_ID); 
}

// HiddenDelayPlusButton - PressedEvent
void HiddenDelayPlusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(DelayPlusButton, LA_TRUE);     
    _processKeyPressed(&appData.pulseDelay,true,100,DelayLabel,true,DelaySlider);    
}

// HiddenDelayPlusButton - ReleasedEvent
void HiddenDelayPlusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(DelayPlusButton, LA_FALSE);   
}

// HiddenDelayMinusButton - PressedEvent
void HiddenDelayMinusButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(DelayMinusButton, LA_TRUE);     
    _processKeyPressed(&appData.pulseDelay,false,0,DelayLabel,true,DelaySlider);     
}

// HiddenDelayMinusButton - ReleasedEvent
void HiddenDelayMinusButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(DelayMinusButton, LA_FALSE);    
}

// HiddenPulseDurUpButton - PressedEvent
void HiddenPulseDurUpButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(PulseDurUpButton, LA_TRUE);     
    if (appData.pulseDurIdx < 2)
    {
        appData.pulseDurIdx++;    
        _SetNumericLabel(pulseDurations[appData.pulseDurIdx], PulseDurationLabel, false);
    }    
}

// HiddenPulseDurUpButton - ReleasedEvent
void HiddenPulseDurUpButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(PulseDurUpButton, LA_FALSE);    
}

// HiddenPulseDurDownButton - PressedEvent
void HiddenPulseDurDownButton_PressedEvent(laButtonWidget* btn)
{
    laButtonWidget_SetPressed(PulseDurDownButton, LA_TRUE);    
    if (appData.pulseDurIdx > 0)
    {
        appData.pulseDurIdx--;    
        _SetNumericLabel(pulseDurations[appData.pulseDurIdx], PulseDurationLabel, false);
    }    
}

// HiddenPulseDurDownButton - ReleasedEvent
void HiddenPulseDurDownButton_ReleasedEvent(laButtonWidget* btn)
{
    _resetKeyPressed();
    laButtonWidget_SetPressed(PulseDurDownButton, LA_FALSE);    
}

// HiddenSinewaveButton2 - PressedEvent
void HiddenSinewaveButton2_PressedEvent(laButtonWidget* btn)
{
    HiddenSinewaveButton_PressedEvent(btn);
}

// HiddenSquarewaveButton2 - PressedEvent
void HiddenSquarewaveButton2_PressedEvent(laButtonWidget* btn)
{
    HiddenSquarewaveButton_PressedEvent(btn);    
}

// HiddenSawtoothButton2 - PressedEvent
void HiddenSawtoothButton2_PressedEvent(laButtonWidget* btn)
{
    HiddenSawtoothButton_PressedEvent(btn);       
}

// HiddenTriangleButton2 - PressedEvent
void HiddenTriangleButton2_PressedEvent(laButtonWidget* btn)
{
    HiddenTriangleButton_PressedEvent(btn);
}





