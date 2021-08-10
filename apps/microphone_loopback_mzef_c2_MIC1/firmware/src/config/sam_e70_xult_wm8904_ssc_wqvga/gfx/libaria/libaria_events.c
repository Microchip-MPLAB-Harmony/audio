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
int32_t _msSinceDelayPressed;
int32_t _msSinceMicGainChanged;
uint32_t* _repeatValue;
uint32_t _repeatIncOrDec;
uint32_t _repeatMinOrMaxValue;
laLabelWidget* _repeatLabel;
bool _repeatPercent;
int32_t latestVolume;
int32_t latestDelay;
int32_t latestMicGain;
int32_t lastMicGain;
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

void _setDelayVisible()
{
    laWidget_SetVisible((laWidget*)CircularSliderDelay, appData.delayEnabled);
    laWidget_SetVisible((laWidget*)delayAmount, appData.delayEnabled);            
    laWidget_SetVisible((laWidget*)delayLabel, appData.delayEnabled); 
}

void InitGUI( void )
{   
    _msSinceKeyPressed = -1;
    _msSinceVolumePressed = -1;   
    _msSinceDelayPressed = -1; 
    _msSinceMicGainChanged = -1;
    _SetNumericLabel(INITIAL_VOLUME_PERCENT, volumePercent, true); 
    _SetNumericLabel(INITIAL_DELAY, delayAmount, false);
    _SetNumericLabel(INITIAL_MIC_GAIN_PERENT, micGainPercent, true);   

    bool oldDelayEnabled = appData.delayEnabled;
    appData.delayEnabled = false;       // reset delayEnabled so mic gain change event doesn't act on it
    laCircularSliderWidget_SetValue(CircularSliderDelay, INITIAL_DELAY/10);     // assumes 1000 ms maximum
    laCircularSliderWidget_SetValue(CircularVolume, INITIAL_VOLUME_PERCENT);
    laSliderWidget_SetSliderPercentage(SliderWidgetMicGain, INITIAL_MIC_GAIN_PERENT);
    appData.delayEnabled = oldDelayEnabled;

    lastMicGain = -1;
    
    laImageWidget_SetImage(ImageWidget, &toggleon61x31);     
    _setDelayVisible();    
}

void KeyRepeatTask_sub( void )        // called every ms from gui.c
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
    if (_msSinceDelayPressed >= 0)
    {
        _msSinceDelayPressed++;        
        
        if ((_msSinceDelayPressed % 100)==0)       // update 10 times per sec
        {          
            appData.newDelay = latestDelay;
            _SetNumericLabel(appData.newDelay, delayAmount, false);            
        }
    }    
    if (_msSinceMicGainChanged >= 0)
    {
        _msSinceMicGainChanged++;        
        
        if ((_msSinceMicGainChanged % 100)==0)       // update 10 times per sec
        {
            if (lastMicGain != latestMicGain)
            {
                appData.newMicGain = latestMicGain;
                _SetNumericLabel(appData.newMicGain, micGainPercent, false);            
            }
            else
            {
                _msSinceMicGainChanged = -1;
            }
			lastMicGain = latestMicGain;
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
}

void _resetKeyPressed()
{
    _msSinceKeyPressed = -1;
}


// CircularVolume - ValueChangedEvent
void CircularVolume_ValueChangedEvent(laCircularSliderWidget * slider, int32_t value)
{   
    latestVolume = value;    
}

// CircularVolume - PressedEvent
void CircularVolume_PressedEvent(laCircularSliderWidget * slider, int32_t value)
{
    _msSinceVolumePressed = 0;
    appData.newVolume = latestVolume = value;
    _SetNumericLabel(appData.newVolume, volumePercent, true);
    
    laImageWidget_SetImage(ImageWidget, &toggleoff61x31);     
    appData.delayEnabled = false;
    _setDelayVisible();    
}

// CircularVolume - ReleasedEvent
void CircularVolume_ReleasedEvent(laCircularSliderWidget * slider, int32_t value)
{
    appData.newVolume = value;
    _msSinceVolumePressed = -1;
    _SetNumericLabel(appData.newVolume, volumePercent, true);    
}

// SliderWidgetMicGain - ValueChangedEvent
void SliderWidgetMicGain_ValueChangedEvent(laSliderWidget* sld)
{
    _msSinceMicGainChanged = 0;
    
    latestMicGain = laSliderWidget_GetSliderPercentage(sld);    
    _SetNumericLabel(latestMicGain, micGainPercent, true);

    if (appData.delayEnabled)
    {
        laImageWidget_SetImage(ImageWidget, &toggleoff61x31);     
        appData.delayEnabled = false;
        _setDelayVisible();    
    }
}

// CircularSliderDelay - ValueChangedEvent
void CircularSliderDelay_ValueChangedEvent(laCircularSliderWidget * slider, int32_t value)
{
    latestDelay = 10*value;    
}

// CircularSliderDelay - PressedEvent
void CircularSliderDelay_PressedEvent(laCircularSliderWidget * slider, int32_t value)
{
    _msSinceDelayPressed = 0;
    appData.newDelay = latestDelay = 10*value;
    _SetNumericLabel(appData.newDelay, delayAmount, false);     
}

// CircularSliderDelay - ReleasedEvent
void CircularSliderDelay_ReleasedEvent(laCircularSliderWidget * slider, int32_t value)
{
    appData.newDelay = 10*value;
    _msSinceDelayPressed = -1;
    _SetNumericLabel(appData.newDelay, delayAmount, false);     
}

// hiddenToggleOnButton - PressedEvent
void hiddenToggleOnButton_PressedEvent(laButtonWidget* btn)
{
    if (appData.delayEnabled)
    {
        laImageWidget_SetImage(ImageWidget, &toggleoff61x31);     
        appData.delayEnabled = false;
        _setDelayVisible();
    }
    else
    {
        laImageWidget_SetImage(ImageWidget, &toggleon61x31);
        appData.delayEnabled = true;
        _setDelayVisible();        
    }
}

// hiddenToggleOffButton - PressedEvent
void hiddenToggleOffButton_PressedEvent(laButtonWidget* btn)
{
    if (appData.delayEnabled)
    {
        laImageWidget_SetImage(ImageWidget, &toggleoff61x31);     
        appData.delayEnabled = false;
        _setDelayVisible();
    }
    else
    {
        laImageWidget_SetImage(ImageWidget, &toggleon61x31);
        appData.delayEnabled = true;
        _setDelayVisible();        
    }
}





