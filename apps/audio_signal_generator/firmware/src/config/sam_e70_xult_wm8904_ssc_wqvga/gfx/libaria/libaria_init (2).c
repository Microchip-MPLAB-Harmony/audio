/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_init.c

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

#include "gfx/libaria/libaria_init.h"

laScheme defaultScheme;
laScheme titleScheme;
laScheme textFieldScheme;
laScheme whiteText;
laScheme backgroundScheme;
laScheme defaultPaletteScheme;
laImageWidget* backgroundImage;
laLineWidget* LineWidget2;
laButtonWidget* SinewaveButton;
laButtonWidget* SquarewaveButton;
laButtonWidget* SawtoothButton;
laButtonWidget* TriangleButton;
laButtonWidget* HiddenSinewaveButton;
laButtonWidget* HiddenSquarewaveButton;
laButtonWidget* HiddenSawtoothButton;
laButtonWidget* HiddenTriangleButton;
laLineWidget* HorizLine;
laLineWidget* VertLine;
laLabelWidget* LabelWidget5;
laLabelWidget* EndFreqText;
laCircularSliderWidget* CircularVolume;
laLabelWidget* LabelWidget12;
laLabelWidget* LabelWidget14;
laCheckBoxWidget* SweepCheckBox;
laCheckBoxWidget* LoopCheckBox;
laButtonWidget* HiddenSweepButton;
laButtonWidget* HiddenLoopButton;
laButtonWidget* StartFreqPlusButton;
laButtonWidget* StartFreqMinusButton;
laButtonWidget* HiddenStartFreqPlusButton;
laButtonWidget* HiddenStartFreqMinusButton;
laButtonWidget* EndFreqPlusButton;
laButtonWidget* EndFreqMinusButton;
laButtonWidget* DurationPlusButton;
laButtonWidget* DurationMinusButton;
laButtonWidget* PlayPauseButton;
laTextFieldWidget* Title;
laTextFieldWidget* TitleWaveType;
laButtonWidget* OutputTriggerButton;
laButtonWidget* HiddenEndFreqPlusButton;
laButtonWidget* HiddenEndFreqMinusButton;
laButtonWidget* HiddenDurationPlusButton;
laButtonWidget* HiddenDurationMinusButton;
laButtonWidget* HiddenPlayPauseButton;
laLabelWidget* StartFreqLabel;
laLabelWidget* EndFreqLabel;
laLabelWidget* DurationLabel;
laLabelWidget* volumePercent;
laImageWidget* ImageWidget3;
laImageWidget* backgroundImage2;
laButtonWidget* SinewaveButton2;
laButtonWidget* SquarewaveButton2;
laButtonWidget* SawtoothButton2;
laButtonWidget* TriangleButton2;
laLineWidget* LineWidget36;
laLineWidget* LineWidget37;
laLabelWidget* LabelWidget54;
laLabelWidget* LabelWidget53;
laButtonWidget* DelayPlusButton;
laButtonWidget* DelayMinusButton;
laSliderWidget* DelaySlider;
laButtonWidget* SignalGeneratorButton;
laLabelWidget* DelayLabel;
laButtonWidget* HiddenDelayPlusButton;
laButtonWidget* HiddenDelayMinusButton;
laButtonWidget* PulseDurUpButton;
laButtonWidget* PulseDurDownButton;
laLabelWidget* PulseDurationLabel;
laButtonWidget* HiddenPulseDurUpButton;
laButtonWidget* HiddenPulseDurDownButton;
laButtonWidget* HiddenSinewaveButton2;
laButtonWidget* HiddenSquarewaveButton2;
laButtonWidget* HiddenSawtoothButton2;
laButtonWidget* HiddenTriangleButton2;
laTextFieldWidget* TextFieldWidget12;
laLineWidget* LineWidget1;
laImageWidget* ImageWidget4;


static void ScreenCreate_default(laScreen* screen);
static void ScreenCreate_trigger(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    laScheme_Initialize(&defaultScheme, GFX_COLOR_MODE_RGB_565);
    defaultScheme.base = 0x0;
    defaultScheme.highlight = 0x0;
    defaultScheme.highlightLight = 0x57;
    defaultScheme.shadow = 0x84;
    defaultScheme.shadowDark = 0x3;
    defaultScheme.foreground = 0x4;
    defaultScheme.foregroundInactive = 0x5;
    defaultScheme.foregroundDisabled = 0x84;
    defaultScheme.background = 0x57;
    defaultScheme.backgroundInactive = 0x5;
    defaultScheme.backgroundDisabled = 0x0;
    defaultScheme.text = 0x4;
    defaultScheme.textHighlight = 0x6;
    defaultScheme.textHighlightText = 0x57;
    defaultScheme.textInactive = 0x5;
    defaultScheme.textDisabled = 0x8B;

    laScheme_Initialize(&titleScheme, GFX_COLOR_MODE_RGB_565);
    titleScheme.base = 0x0;
    titleScheme.highlight = 0x0;
    titleScheme.highlightLight = 0x57;
    titleScheme.shadow = 0x84;
    titleScheme.shadowDark = 0x3;
    titleScheme.foreground = 0x4;
    titleScheme.foregroundInactive = 0x5;
    titleScheme.foregroundDisabled = 0x84;
    titleScheme.background = 0x57;
    titleScheme.backgroundInactive = 0x5;
    titleScheme.backgroundDisabled = 0x0;
    titleScheme.text = 0x4;
    titleScheme.textHighlight = 0x6;
    titleScheme.textHighlightText = 0x57;
    titleScheme.textInactive = 0x5;
    titleScheme.textDisabled = 0x8B;

    laScheme_Initialize(&textFieldScheme, GFX_COLOR_MODE_RGB_565);
    textFieldScheme.base = 0x0;
    textFieldScheme.highlight = 0x0;
    textFieldScheme.highlightLight = 0x57;
    textFieldScheme.shadow = 0x84;
    textFieldScheme.shadowDark = 0x3;
    textFieldScheme.foreground = 0x4;
    textFieldScheme.foregroundInactive = 0x5;
    textFieldScheme.foregroundDisabled = 0x84;
    textFieldScheme.background = 0x57;
    textFieldScheme.backgroundInactive = 0x5;
    textFieldScheme.backgroundDisabled = 0x0;
    textFieldScheme.text = 0x4;
    textFieldScheme.textHighlight = 0x6;
    textFieldScheme.textHighlightText = 0x57;
    textFieldScheme.textInactive = 0x5;
    textFieldScheme.textDisabled = 0x8B;

    laScheme_Initialize(&whiteText, GFX_COLOR_MODE_RGB_565);
    whiteText.base = 0x0;
    whiteText.highlight = 0x0;
    whiteText.highlightLight = 0x57;
    whiteText.shadow = 0x84;
    whiteText.shadowDark = 0x1;
    whiteText.foreground = 0x4;
    whiteText.foregroundInactive = 0x5;
    whiteText.foregroundDisabled = 0x84;
    whiteText.background = 0x57;
    whiteText.backgroundInactive = 0x5;
    whiteText.backgroundDisabled = 0x0;
    whiteText.text = 0x1;
    whiteText.textHighlight = 0x6;
    whiteText.textHighlightText = 0x57;
    whiteText.textInactive = 0x5;
    whiteText.textDisabled = 0x8B;

    laScheme_Initialize(&backgroundScheme, GFX_COLOR_MODE_RGB_565);
    backgroundScheme.base = 0x0;
    backgroundScheme.highlight = 0x0;
    backgroundScheme.highlightLight = 0x57;
    backgroundScheme.shadow = 0x84;
    backgroundScheme.shadowDark = 0x3;
    backgroundScheme.foreground = 0x4;
    backgroundScheme.foregroundInactive = 0x5;
    backgroundScheme.foregroundDisabled = 0x84;
    backgroundScheme.background = 0x57;
    backgroundScheme.backgroundInactive = 0x5;
    backgroundScheme.backgroundDisabled = 0x0;
    backgroundScheme.text = 0x4;
    backgroundScheme.textHighlight = 0x6;
    backgroundScheme.textHighlightText = 0x57;
    backgroundScheme.textInactive = 0x5;
    backgroundScheme.textDisabled = 0x8B;

    laScheme_Initialize(&defaultPaletteScheme, GFX_COLOR_MODE_RGB_565);
    defaultPaletteScheme.base = 0x0;
    defaultPaletteScheme.highlight = 0x0;
    defaultPaletteScheme.highlightLight = 0x57;
    defaultPaletteScheme.shadow = 0x84;
    defaultPaletteScheme.shadowDark = 0x3;
    defaultPaletteScheme.foreground = 0x4;
    defaultPaletteScheme.foregroundInactive = 0x5;
    defaultPaletteScheme.foregroundDisabled = 0x84;
    defaultPaletteScheme.background = 0x57;
    defaultPaletteScheme.backgroundInactive = 0x5;
    defaultPaletteScheme.backgroundDisabled = 0x0;
    defaultPaletteScheme.text = 0x4;
    defaultPaletteScheme.textHighlight = 0x6;
    defaultPaletteScheme.textHighlightText = 0x57;
    defaultPaletteScheme.textInactive = 0x5;
    defaultPaletteScheme.textDisabled = 0x8B;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCU);
    GFX_Set(GFXF_GLOBAL_PALETTE, globalColorPalette);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_TRUE, LA_TRUE, &ScreenCreate_default);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_TRUE, LA_TRUE, &ScreenCreate_trigger);
    laContext_AddScreen(screen);

    laContext_SetPreemptionLevel(LA_PREEMPTION_LEVEL_2);
    laContext_SetActiveScreen(0);

	return 0;
}

static void ScreenCreate_default(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &defaultScheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    backgroundImage = laImageWidget_New();
    laWidget_SetSize((laWidget*)backgroundImage, 480, 272);
    laWidget_SetScheme((laWidget*)backgroundImage, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)backgroundImage, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)backgroundImage, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(backgroundImage, &blue_bg480x272);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)backgroundImage);

    LineWidget2 = laLineWidget_New();
    laWidget_SetPosition((laWidget*)LineWidget2, 0, 24);
    laWidget_SetSize((laWidget*)LineWidget2, 106, 2);
    laWidget_SetScheme((laWidget*)LineWidget2, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LineWidget2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LineWidget2, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(LineWidget2, 0, 0);
    laLineWidget_SetEndPoint(LineWidget2, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LineWidget2);

    SinewaveButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SinewaveButton, 30, 35);
    laWidget_SetSize((laWidget*)SinewaveButton, 50, 30);
    laWidget_SetScheme((laWidget*)SinewaveButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SinewaveButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SinewaveButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SinewaveButton, LA_TRUE);
    laButtonWidget_SetPressed(SinewaveButton, LA_TRUE);
    laButtonWidget_SetPressedImage(SinewaveButton, &sinewave_lit);
    laButtonWidget_SetReleasedImage(SinewaveButton, &sinewave_small);
    laButtonWidget_SetPressedOffset(SinewaveButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SinewaveButton);

    SquarewaveButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SquarewaveButton, 30, 80);
    laWidget_SetSize((laWidget*)SquarewaveButton, 50, 30);
    laWidget_SetScheme((laWidget*)SquarewaveButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SquarewaveButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SquarewaveButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SquarewaveButton, LA_TRUE);
    laButtonWidget_SetPressedImage(SquarewaveButton, &squarewave_lit);
    laButtonWidget_SetReleasedImage(SquarewaveButton, &squarewave_small);
    laButtonWidget_SetPressedOffset(SquarewaveButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SquarewaveButton);

    SawtoothButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SawtoothButton, 30, 125);
    laWidget_SetSize((laWidget*)SawtoothButton, 50, 30);
    laWidget_SetScheme((laWidget*)SawtoothButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SawtoothButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SawtoothButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SawtoothButton, LA_TRUE);
    laButtonWidget_SetPressedImage(SawtoothButton, &sawtoothwave_lit);
    laButtonWidget_SetReleasedImage(SawtoothButton, &sawtoothwave_small);
    laButtonWidget_SetPressedOffset(SawtoothButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SawtoothButton);

    TriangleButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)TriangleButton, 30, 170);
    laWidget_SetSize((laWidget*)TriangleButton, 50, 30);
    laWidget_SetScheme((laWidget*)TriangleButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)TriangleButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)TriangleButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(TriangleButton, LA_TRUE);
    laButtonWidget_SetPressedImage(TriangleButton, &triangle_lit);
    laButtonWidget_SetReleasedImage(TriangleButton, &triangle_small);
    laButtonWidget_SetPressedOffset(TriangleButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TriangleButton);

    HiddenSinewaveButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSinewaveButton, 9, 27);
    laWidget_SetSize((laWidget*)HiddenSinewaveButton, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSinewaveButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSinewaveButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSinewaveButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSinewaveButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSinewaveButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSinewaveButton, &HiddenSinewaveButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSinewaveButton);

    HiddenSquarewaveButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSquarewaveButton, 9, 71);
    laWidget_SetSize((laWidget*)HiddenSquarewaveButton, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSquarewaveButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSquarewaveButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSquarewaveButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSquarewaveButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSquarewaveButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSquarewaveButton, &HiddenSquarewaveButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSquarewaveButton);

    HiddenSawtoothButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSawtoothButton, 9, 117);
    laWidget_SetSize((laWidget*)HiddenSawtoothButton, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSawtoothButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSawtoothButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSawtoothButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSawtoothButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSawtoothButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSawtoothButton, &HiddenSawtoothButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSawtoothButton);

    HiddenTriangleButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenTriangleButton, 9, 162);
    laWidget_SetSize((laWidget*)HiddenTriangleButton, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenTriangleButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenTriangleButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenTriangleButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenTriangleButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenTriangleButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenTriangleButton, &HiddenTriangleButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenTriangleButton);

    HorizLine = laLineWidget_New();
    laWidget_SetPosition((laWidget*)HorizLine, 0, 213);
    laWidget_SetSize((laWidget*)HorizLine, 106, 2);
    laWidget_SetScheme((laWidget*)HorizLine, &whiteText);
    laWidget_SetBackgroundType((laWidget*)HorizLine, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HorizLine, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(HorizLine, 0, 0);
    laLineWidget_SetEndPoint(HorizLine, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)HorizLine);

    VertLine = laLineWidget_New();
    laWidget_SetPosition((laWidget*)VertLine, 104, 0);
    laWidget_SetSize((laWidget*)VertLine, 2, 270);
    laWidget_SetScheme((laWidget*)VertLine, &whiteText);
    laWidget_SetBackgroundType((laWidget*)VertLine, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)VertLine, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(VertLine, 0, 0);
    laLineWidget_SetEndPoint(VertLine, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)VertLine);

    LabelWidget5 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget5, 104, 87);
    laWidget_SetSize((laWidget*)LabelWidget5, 80, 40);
    laWidget_SetScheme((laWidget*)LabelWidget5, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget5, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget5, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget5, laString_CreateFromID(string_StartFreq));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget5);

    EndFreqText = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)EndFreqText, 347, 86);
    laWidget_SetSize((laWidget*)EndFreqText, 60, 44);
    laWidget_SetScheme((laWidget*)EndFreqText, &whiteText);
    laWidget_SetBackgroundType((laWidget*)EndFreqText, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)EndFreqText, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(EndFreqText, laString_CreateFromID(string_EndFreq));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)EndFreqText);

    CircularVolume = laCircularSliderWidget_New();
    laWidget_SetPosition((laWidget*)CircularVolume, 212, 125);
    laWidget_SetSize((laWidget*)CircularVolume, 120, 121);
    laWidget_SetScheme((laWidget*)CircularVolume, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)CircularVolume, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)CircularVolume, LA_WIDGET_BORDER_NONE);
    laCircularSliderWidget_SetRadius(CircularVolume, 60);
    laCircularSliderWidget_SetStartAngle(CircularVolume, 270);
    laCircularSliderWidget_SetValue(CircularVolume, 25);
    laCircularSliderWidget_SetStickyButton(CircularVolume, LA_FALSE);
    laCircularSliderWidget_SetTouchOnButtonOnly(CircularVolume, LA_FALSE);
    laCircularSliderWidget_SetDirection(CircularVolume, CIRCULAR_SLIDER_DIR_CLOCKWISE);
    laCircularSliderWidget_SetArcThickness(CircularVolume, ACTIVE_AREA, 20);
    laCircularSliderWidget_SetArcScheme(CircularVolume, CIRCLE_BUTTON, &backgroundScheme);
    laCircularSliderWidget_SetValueChangedEventCallback(CircularVolume, &CircularVolume_ValueChangedEvent);
    laCircularSliderWidget_SetPressedEventCallback(CircularVolume, &CircularVolume_PressedEvent);
    laCircularSliderWidget_SetReleasedEventCallback(CircularVolume, &CircularVolume_ReleasedEvent);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)CircularVolume);

    LabelWidget12 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget12, 241, 246);
    laWidget_SetSize((laWidget*)LabelWidget12, 64, 25);
    laWidget_SetScheme((laWidget*)LabelWidget12, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget12, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget12, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget12, laString_CreateFromID(string_Volume));
    laLabelWidget_SetHAlignment(LabelWidget12, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget12);

    LabelWidget14 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget14, 347, 212);
    laWidget_SetSize((laWidget*)LabelWidget14, 49, 34);
    laWidget_SetScheme((laWidget*)LabelWidget14, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget14, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget14, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget14, laString_CreateFromID(string_Duration));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget14);

    SweepCheckBox = laCheckBoxWidget_New();
    laWidget_SetPosition((laWidget*)SweepCheckBox, 261, 46);
    laWidget_SetSize((laWidget*)SweepCheckBox, 69, 25);
    laWidget_SetScheme((laWidget*)SweepCheckBox, &whiteText);
    laWidget_SetBackgroundType((laWidget*)SweepCheckBox, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SweepCheckBox, LA_WIDGET_BORDER_NONE);
    laCheckBoxWidget_SetText(SweepCheckBox, laString_CreateFromID(string_Sweep));
    laCheckBoxWidget_SetImageMargin(SweepCheckBox, 5);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SweepCheckBox);

    LoopCheckBox = laCheckBoxWidget_New();
    laWidget_SetPosition((laWidget*)LoopCheckBox, 261, 92);
    laWidget_SetSize((laWidget*)LoopCheckBox, 58, 25);
    laWidget_SetScheme((laWidget*)LoopCheckBox, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LoopCheckBox, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LoopCheckBox, LA_WIDGET_BORDER_NONE);
    laCheckBoxWidget_SetChecked(LoopCheckBox, LA_TRUE);
    laCheckBoxWidget_SetText(LoopCheckBox, laString_CreateFromID(string_Loop));
    laCheckBoxWidget_SetImageMargin(LoopCheckBox, 5);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LoopCheckBox);

    HiddenSweepButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSweepButton, 255, 38);
    laWidget_SetSize((laWidget*)HiddenSweepButton, 75, 40);
    laWidget_SetScheme((laWidget*)HiddenSweepButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSweepButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSweepButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSweepButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSweepButton, 0);
    laButtonWidget_SetImageMargin(HiddenSweepButton, 5);
    laButtonWidget_SetPressedEventCallback(HiddenSweepButton, &HiddenSweepButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSweepButton);

    HiddenLoopButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenLoopButton, 255, 84);
    laWidget_SetSize((laWidget*)HiddenLoopButton, 75, 40);
    laWidget_SetScheme((laWidget*)HiddenLoopButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenLoopButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenLoopButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenLoopButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenLoopButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenLoopButton, &HiddenLoopButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenLoopButton);

    StartFreqPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)StartFreqPlusButton, 190, 40);
    laWidget_SetSize((laWidget*)StartFreqPlusButton, 40, 40);
    laWidget_SetScheme((laWidget*)StartFreqPlusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)StartFreqPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)StartFreqPlusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(StartFreqPlusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(StartFreqPlusButton, &Plus2);
    laButtonWidget_SetReleasedImage(StartFreqPlusButton, &Plus2);
    laButtonWidget_SetPressedOffset(StartFreqPlusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)StartFreqPlusButton);

    StartFreqMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)StartFreqMinusButton, 190, 95);
    laWidget_SetSize((laWidget*)StartFreqMinusButton, 40, 40);
    laWidget_SetScheme((laWidget*)StartFreqMinusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)StartFreqMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)StartFreqMinusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(StartFreqMinusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(StartFreqMinusButton, &Minus2);
    laButtonWidget_SetReleasedImage(StartFreqMinusButton, &Minus2);
    laButtonWidget_SetPressedOffset(StartFreqMinusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)StartFreqMinusButton);

    HiddenStartFreqPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenStartFreqPlusButton, 175, 28);
    laWidget_SetSize((laWidget*)HiddenStartFreqPlusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenStartFreqPlusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenStartFreqPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenStartFreqPlusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenStartFreqPlusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenStartFreqPlusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenStartFreqPlusButton, &HiddenStartFreqPlusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenStartFreqPlusButton, &HiddenStartFreqPlusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenStartFreqPlusButton);

    HiddenStartFreqMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenStartFreqMinusButton, 175, 87);
    laWidget_SetSize((laWidget*)HiddenStartFreqMinusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenStartFreqMinusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenStartFreqMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenStartFreqMinusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenStartFreqMinusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenStartFreqMinusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenStartFreqMinusButton, &HiddenStartFreqMinusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenStartFreqMinusButton, &HiddenStartFreqMinusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenStartFreqMinusButton);

    EndFreqPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)EndFreqPlusButton, 420, 40);
    laWidget_SetSize((laWidget*)EndFreqPlusButton, 40, 40);
    laWidget_SetScheme((laWidget*)EndFreqPlusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)EndFreqPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)EndFreqPlusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(EndFreqPlusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(EndFreqPlusButton, &Plus2);
    laButtonWidget_SetReleasedImage(EndFreqPlusButton, &Plus2);
    laButtonWidget_SetPressedOffset(EndFreqPlusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)EndFreqPlusButton);

    EndFreqMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)EndFreqMinusButton, 420, 95);
    laWidget_SetSize((laWidget*)EndFreqMinusButton, 40, 40);
    laWidget_SetScheme((laWidget*)EndFreqMinusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)EndFreqMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)EndFreqMinusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(EndFreqMinusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(EndFreqMinusButton, &Minus2);
    laButtonWidget_SetReleasedImage(EndFreqMinusButton, &Minus2);
    laButtonWidget_SetPressedOffset(EndFreqMinusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)EndFreqMinusButton);

    DurationPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)DurationPlusButton, 420, 157);
    laWidget_SetSize((laWidget*)DurationPlusButton, 40, 40);
    laWidget_SetScheme((laWidget*)DurationPlusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)DurationPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DurationPlusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(DurationPlusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(DurationPlusButton, &Plus2);
    laButtonWidget_SetReleasedImage(DurationPlusButton, &Plus2);
    laButtonWidget_SetPressedOffset(DurationPlusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DurationPlusButton);

    DurationMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)DurationMinusButton, 420, 212);
    laWidget_SetSize((laWidget*)DurationMinusButton, 40, 40);
    laWidget_SetScheme((laWidget*)DurationMinusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)DurationMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DurationMinusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(DurationMinusButton, LA_TRUE);
    laButtonWidget_SetPressedImage(DurationMinusButton, &Minus2);
    laButtonWidget_SetReleasedImage(DurationMinusButton, &Minus2);
    laButtonWidget_SetPressedOffset(DurationMinusButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DurationMinusButton);

    PlayPauseButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)PlayPauseButton, 130, 162);
    laWidget_SetSize((laWidget*)PlayPauseButton, 68, 56);
    laWidget_SetScheme((laWidget*)PlayPauseButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)PlayPauseButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)PlayPauseButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(PlayPauseButton, LA_TRUE);
    laButtonWidget_SetPressedImage(PlayPauseButton, &AudioPlay);
    laButtonWidget_SetReleasedImage(PlayPauseButton, &AudioPlay);
    laButtonWidget_SetPressedOffset(PlayPauseButton, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)PlayPauseButton);

    Title = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)Title, 109, 6);
    laWidget_SetSize((laWidget*)Title, 221, 25);
    laWidget_SetScheme((laWidget*)Title, &titleScheme);
    laWidget_SetBackgroundType((laWidget*)Title, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)Title, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(Title, laString_CreateFromID(string_Title));
    laTextFieldWidget_SetCursorEnabled(Title, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)Title);

    TitleWaveType = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)TitleWaveType, 317, 6);
    laWidget_SetSize((laWidget*)TitleWaveType, 155, 25);
    laWidget_SetScheme((laWidget*)TitleWaveType, &titleScheme);
    laWidget_SetBackgroundType((laWidget*)TitleWaveType, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)TitleWaveType, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetCursorEnabled(TitleWaveType, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TitleWaveType);

    OutputTriggerButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)OutputTriggerButton, 20, 226);
    laWidget_SetSize((laWidget*)OutputTriggerButton, 75, 35);
    laWidget_SetScheme((laWidget*)OutputTriggerButton, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)OutputTriggerButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)OutputTriggerButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(OutputTriggerButton, laString_CreateFromID(string_OutputTrigger));
    laButtonWidget_SetPressedEventCallback(OutputTriggerButton, &OutputTriggerButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(OutputTriggerButton, &OutputTriggerButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)OutputTriggerButton);

    HiddenEndFreqPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenEndFreqPlusButton, 403, 27);
    laWidget_SetSize((laWidget*)HiddenEndFreqPlusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenEndFreqPlusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenEndFreqPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenEndFreqPlusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenEndFreqPlusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenEndFreqPlusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenEndFreqPlusButton, &HiddenEndFreqPlusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenEndFreqPlusButton, &HiddenEndFreqPlusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenEndFreqPlusButton);

    HiddenEndFreqMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenEndFreqMinusButton, 403, 86);
    laWidget_SetSize((laWidget*)HiddenEndFreqMinusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenEndFreqMinusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenEndFreqMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenEndFreqMinusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenEndFreqMinusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenEndFreqMinusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenEndFreqMinusButton, &HiddenEndFreqMinusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenEndFreqMinusButton, &HiddenEndFreqMinusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenEndFreqMinusButton);

    HiddenDurationPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenDurationPlusButton, 403, 145);
    laWidget_SetSize((laWidget*)HiddenDurationPlusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenDurationPlusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenDurationPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenDurationPlusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenDurationPlusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenDurationPlusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenDurationPlusButton, &HiddenDurationPlusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenDurationPlusButton, &HiddenDurationPlusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenDurationPlusButton);

    HiddenDurationMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenDurationMinusButton, 403, 203);
    laWidget_SetSize((laWidget*)HiddenDurationMinusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenDurationMinusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenDurationMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenDurationMinusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenDurationMinusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenDurationMinusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenDurationMinusButton, &HiddenDurationMinusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenDurationMinusButton, &HiddenDurationMinusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenDurationMinusButton);

    HiddenPlayPauseButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenPlayPauseButton, 112, 150);
    laWidget_SetSize((laWidget*)HiddenPlayPauseButton, 100, 80);
    laWidget_SetScheme((laWidget*)HiddenPlayPauseButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenPlayPauseButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenPlayPauseButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenPlayPauseButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenPlayPauseButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenPlayPauseButton, &HiddenPlayPauseButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenPlayPauseButton, &HiddenPlayPauseButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenPlayPauseButton);

    StartFreqLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)StartFreqLabel, 121, 59);
    laWidget_SetSize((laWidget*)StartFreqLabel, 50, 25);
    laWidget_SetScheme((laWidget*)StartFreqLabel, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)StartFreqLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)StartFreqLabel, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)StartFreqLabel);

    EndFreqLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)EndFreqLabel, 354, 59);
    laWidget_SetSize((laWidget*)EndFreqLabel, 50, 25);
    laWidget_SetScheme((laWidget*)EndFreqLabel, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)EndFreqLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)EndFreqLabel, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)EndFreqLabel);

    DurationLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)DurationLabel, 339, 188);
    laWidget_SetSize((laWidget*)DurationLabel, 65, 25);
    laWidget_SetScheme((laWidget*)DurationLabel, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)DurationLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DurationLabel, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DurationLabel);

    volumePercent = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)volumePercent, 248, 173);
    laWidget_SetSize((laWidget*)volumePercent, 48, 25);
    laWidget_SetScheme((laWidget*)volumePercent, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)volumePercent, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)volumePercent, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)volumePercent);

    ImageWidget3 = laImageWidget_New();
    laWidget_SetSize((laWidget*)ImageWidget3, 103, 23);
    laWidget_SetScheme((laWidget*)ImageWidget3, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)ImageWidget3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)ImageWidget3, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(ImageWidget3, &microchip_logo103x24);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageWidget3);

}

static void ScreenCreate_trigger(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &backgroundScheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    backgroundImage2 = laImageWidget_New();
    laWidget_SetSize((laWidget*)backgroundImage2, 480, 272);
    laWidget_SetScheme((laWidget*)backgroundImage2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)backgroundImage2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)backgroundImage2, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(backgroundImage2, &blue_bg480x272);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)backgroundImage2);

    SinewaveButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SinewaveButton2, 30, 35);
    laWidget_SetSize((laWidget*)SinewaveButton2, 50, 30);
    laWidget_SetScheme((laWidget*)SinewaveButton2, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SinewaveButton2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SinewaveButton2, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SinewaveButton2, LA_TRUE);
    laButtonWidget_SetPressed(SinewaveButton2, LA_TRUE);
    laButtonWidget_SetPressedImage(SinewaveButton2, &sinewave_lit);
    laButtonWidget_SetReleasedImage(SinewaveButton2, &sinewave_small);
    laButtonWidget_SetPressedOffset(SinewaveButton2, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SinewaveButton2);

    SquarewaveButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SquarewaveButton2, 30, 80);
    laWidget_SetSize((laWidget*)SquarewaveButton2, 50, 30);
    laWidget_SetScheme((laWidget*)SquarewaveButton2, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SquarewaveButton2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SquarewaveButton2, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SquarewaveButton2, LA_TRUE);
    laButtonWidget_SetPressedImage(SquarewaveButton2, &squarewave_lit);
    laButtonWidget_SetReleasedImage(SquarewaveButton2, &squarewave_small);
    laButtonWidget_SetPressedOffset(SquarewaveButton2, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SquarewaveButton2);

    SawtoothButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SawtoothButton2, 30, 125);
    laWidget_SetSize((laWidget*)SawtoothButton2, 50, 30);
    laWidget_SetScheme((laWidget*)SawtoothButton2, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)SawtoothButton2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SawtoothButton2, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(SawtoothButton2, LA_TRUE);
    laButtonWidget_SetPressedImage(SawtoothButton2, &sawtoothwave_lit);
    laButtonWidget_SetReleasedImage(SawtoothButton2, &sawtoothwave_small);
    laButtonWidget_SetPressedOffset(SawtoothButton2, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SawtoothButton2);

    TriangleButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)TriangleButton2, 30, 170);
    laWidget_SetSize((laWidget*)TriangleButton2, 50, 30);
    laWidget_SetScheme((laWidget*)TriangleButton2, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)TriangleButton2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)TriangleButton2, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetToggleable(TriangleButton2, LA_TRUE);
    laButtonWidget_SetPressedImage(TriangleButton2, &triangle_lit);
    laButtonWidget_SetReleasedImage(TriangleButton2, &triangle_small);
    laButtonWidget_SetPressedOffset(TriangleButton2, 2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TriangleButton2);

    LineWidget36 = laLineWidget_New();
    laWidget_SetPosition((laWidget*)LineWidget36, 104, 0);
    laWidget_SetSize((laWidget*)LineWidget36, 2, 270);
    laWidget_SetScheme((laWidget*)LineWidget36, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LineWidget36, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LineWidget36, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(LineWidget36, 0, 0);
    laLineWidget_SetEndPoint(LineWidget36, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LineWidget36);

    LineWidget37 = laLineWidget_New();
    laWidget_SetPosition((laWidget*)LineWidget37, 0, 213);
    laWidget_SetSize((laWidget*)LineWidget37, 106, 2);
    laWidget_SetScheme((laWidget*)LineWidget37, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LineWidget37, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LineWidget37, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(LineWidget37, 0, 0);
    laLineWidget_SetEndPoint(LineWidget37, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LineWidget37);

    LabelWidget54 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget54, 246, 219);
    laWidget_SetSize((laWidget*)LabelWidget54, 49, 39);
    laWidget_SetScheme((laWidget*)LabelWidget54, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget54, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget54, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget54, laString_CreateFromID(string_PulseDuration));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget54);

    LabelWidget53 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget53, 222, 78);
    laWidget_SetSize((laWidget*)LabelWidget53, 88, 34);
    laWidget_SetScheme((laWidget*)LabelWidget53, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget53, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget53, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget53, laString_CreateFromID(string_Delayed));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget53);

    DelayPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)DelayPlusButton, 319, 40);
    laWidget_SetSize((laWidget*)DelayPlusButton, 40, 40);
    laWidget_SetScheme((laWidget*)DelayPlusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)DelayPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DelayPlusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedImage(DelayPlusButton, &Plus2);
    laButtonWidget_SetReleasedImage(DelayPlusButton, &Plus2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DelayPlusButton);

    DelayMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)DelayMinusButton, 319, 95);
    laWidget_SetSize((laWidget*)DelayMinusButton, 40, 40);
    laWidget_SetScheme((laWidget*)DelayMinusButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)DelayMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DelayMinusButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedImage(DelayMinusButton, &Minus2);
    laButtonWidget_SetReleasedImage(DelayMinusButton, &Minus2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DelayMinusButton);

    DelaySlider = laSliderWidget_New();
    laWidget_SetPosition((laWidget*)DelaySlider, 386, 35);
    laWidget_SetSize((laWidget*)DelaySlider, 30, 100);
    laWidget_SetScheme((laWidget*)DelaySlider, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)DelaySlider, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DelaySlider, LA_WIDGET_BORDER_BEVEL);
    laSliderWidget_SetGripSize(DelaySlider, 20);
    laSliderWidget_SetValueChangedEventCallback(DelaySlider, &DelaySlider_ValueChangedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)DelaySlider);

    SignalGeneratorButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SignalGeneratorButton, 20, 226);
    laWidget_SetSize((laWidget*)SignalGeneratorButton, 75, 35);
    laWidget_SetScheme((laWidget*)SignalGeneratorButton, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)SignalGeneratorButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SignalGeneratorButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(SignalGeneratorButton, laString_CreateFromID(string_SignalGenerator));
    laButtonWidget_SetPressedEventCallback(SignalGeneratorButton, &SignalGeneratorButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(SignalGeneratorButton, &SignalGeneratorButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SignalGeneratorButton);

    DelayLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)DelayLabel, 237, 57);
    laWidget_SetSize((laWidget*)DelayLabel, 63, 25);
    laWidget_SetScheme((laWidget*)DelayLabel, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)DelayLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)DelayLabel, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)DelayLabel);

    HiddenDelayPlusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenDelayPlusButton, 303, 28);
    laWidget_SetSize((laWidget*)HiddenDelayPlusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenDelayPlusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenDelayPlusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenDelayPlusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenDelayPlusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenDelayPlusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenDelayPlusButton, &HiddenDelayPlusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenDelayPlusButton, &HiddenDelayPlusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenDelayPlusButton);

    HiddenDelayMinusButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenDelayMinusButton, 303, 88);
    laWidget_SetSize((laWidget*)HiddenDelayMinusButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenDelayMinusButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenDelayMinusButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenDelayMinusButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenDelayMinusButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenDelayMinusButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenDelayMinusButton, &HiddenDelayMinusButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenDelayMinusButton, &HiddenDelayMinusButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenDelayMinusButton);

    PulseDurUpButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)PulseDurUpButton, 319, 162);
    laWidget_SetSize((laWidget*)PulseDurUpButton, 40, 40);
    laWidget_SetScheme((laWidget*)PulseDurUpButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)PulseDurUpButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)PulseDurUpButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedImage(PulseDurUpButton, &uparrow2);
    laButtonWidget_SetReleasedImage(PulseDurUpButton, &uparrow2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)PulseDurUpButton);

    PulseDurDownButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)PulseDurDownButton, 319, 217);
    laWidget_SetSize((laWidget*)PulseDurDownButton, 40, 40);
    laWidget_SetScheme((laWidget*)PulseDurDownButton, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)PulseDurDownButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)PulseDurDownButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedImage(PulseDurDownButton, &downarrow2);
    laButtonWidget_SetReleasedImage(PulseDurDownButton, &downarrow2);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)PulseDurDownButton);

    PulseDurationLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)PulseDurationLabel, 245, 195);
    laWidget_SetSize((laWidget*)PulseDurationLabel, 50, 25);
    laWidget_SetScheme((laWidget*)PulseDurationLabel, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)PulseDurationLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)PulseDurationLabel, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)PulseDurationLabel);

    HiddenPulseDurUpButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenPulseDurUpButton, 303, 150);
    laWidget_SetSize((laWidget*)HiddenPulseDurUpButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenPulseDurUpButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenPulseDurUpButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenPulseDurUpButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenPulseDurUpButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenPulseDurUpButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenPulseDurUpButton, &HiddenPulseDurUpButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenPulseDurUpButton, &HiddenPulseDurUpButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenPulseDurUpButton);

    HiddenPulseDurDownButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenPulseDurDownButton, 303, 210);
    laWidget_SetSize((laWidget*)HiddenPulseDurDownButton, 70, 60);
    laWidget_SetScheme((laWidget*)HiddenPulseDurDownButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenPulseDurDownButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenPulseDurDownButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenPulseDurDownButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenPulseDurDownButton, 0);
    laButtonWidget_SetPressedEventCallback(HiddenPulseDurDownButton, &HiddenPulseDurDownButton_PressedEvent);
    laButtonWidget_SetReleasedEventCallback(HiddenPulseDurDownButton, &HiddenPulseDurDownButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenPulseDurDownButton);

    HiddenSinewaveButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSinewaveButton2, 9, 27);
    laWidget_SetSize((laWidget*)HiddenSinewaveButton2, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSinewaveButton2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSinewaveButton2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSinewaveButton2, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSinewaveButton2, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSinewaveButton2, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSinewaveButton2, &HiddenSinewaveButton2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSinewaveButton2);

    HiddenSquarewaveButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSquarewaveButton2, 9, 71);
    laWidget_SetSize((laWidget*)HiddenSquarewaveButton2, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSquarewaveButton2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSquarewaveButton2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSquarewaveButton2, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSquarewaveButton2, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSquarewaveButton2, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSquarewaveButton2, &HiddenSquarewaveButton2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSquarewaveButton2);

    HiddenSawtoothButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenSawtoothButton2, 9, 117);
    laWidget_SetSize((laWidget*)HiddenSawtoothButton2, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenSawtoothButton2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenSawtoothButton2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenSawtoothButton2, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenSawtoothButton2, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenSawtoothButton2, 0);
    laButtonWidget_SetPressedEventCallback(HiddenSawtoothButton2, &HiddenSawtoothButton2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenSawtoothButton2);

    HiddenTriangleButton2 = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenTriangleButton2, 9, 162);
    laWidget_SetSize((laWidget*)HiddenTriangleButton2, 90, 45);
    laWidget_SetScheme((laWidget*)HiddenTriangleButton2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)HiddenTriangleButton2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)HiddenTriangleButton2, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)HiddenTriangleButton2, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenTriangleButton2, 0);
    laButtonWidget_SetPressedEventCallback(HiddenTriangleButton2, &HiddenTriangleButton2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenTriangleButton2);

    TextFieldWidget12 = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)TextFieldWidget12, 127, 6);
    laWidget_SetSize((laWidget*)TextFieldWidget12, 317, 25);
    laWidget_SetScheme((laWidget*)TextFieldWidget12, &titleScheme);
    laWidget_SetBackgroundType((laWidget*)TextFieldWidget12, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)TextFieldWidget12, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(TextFieldWidget12, laString_CreateFromID(string_Title2));
    laTextFieldWidget_SetCursorEnabled(TextFieldWidget12, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TextFieldWidget12);

    LineWidget1 = laLineWidget_New();
    laWidget_SetPosition((laWidget*)LineWidget1, 0, 24);
    laWidget_SetSize((laWidget*)LineWidget1, 106, 2);
    laWidget_SetScheme((laWidget*)LineWidget1, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LineWidget1, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LineWidget1, LA_WIDGET_BORDER_LINE);
    laLineWidget_SetStartPoint(LineWidget1, 0, 0);
    laLineWidget_SetEndPoint(LineWidget1, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LineWidget1);

    ImageWidget4 = laImageWidget_New();
    laWidget_SetSize((laWidget*)ImageWidget4, 103, 23);
    laWidget_SetScheme((laWidget*)ImageWidget4, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)ImageWidget4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)ImageWidget4, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(ImageWidget4, &microchip_logo103x24);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageWidget4);

}



