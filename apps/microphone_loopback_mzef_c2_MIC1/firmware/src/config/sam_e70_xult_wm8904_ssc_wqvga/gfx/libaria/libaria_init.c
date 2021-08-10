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
#include "gfx/libaria/inc/libaria_utils.h"

laScheme defaultScheme;
laScheme textFieldScheme;
laScheme titleScheme;
laScheme ButtonScheme;
laScheme whiteText;
laScheme backgroundScheme;
laScheme defaultPaletteScheme;
laImageWidget* backgroundImage;
laCircularSliderWidget* CircularVolume;
laTextFieldWidget* Title;
laLabelWidget* delayAmount;
laImageWidget* ImageWidget1;
laSliderWidget* SliderWidgetMicGain;
laLabelWidget* volumePercent;
laCircularSliderWidget* CircularSliderDelay;
laLabelWidget* LabelWidget6;
laLabelWidget* delayLabel;
laLabelWidget* LabelWidget8;
laLabelWidget* LabelWidget9;
laImageWidget* ImageWidget;
laLabelWidget* micGainPercent;
laButtonWidget* hiddenToggleOnButton;
laButtonWidget* hiddenToggleOffButton;


static void ScreenCreate_default(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    laScheme_Initialize(&defaultScheme, GFX_COLOR_MODE_RGB_565);
    defaultScheme.base = 0x0;
    defaultScheme.highlight = 0x0;
    defaultScheme.highlightLight = 0x30;
    defaultScheme.shadow = 0x2E;
    defaultScheme.shadowDark = 0x3;
    defaultScheme.foreground = 0x4;
    defaultScheme.foregroundInactive = 0x5;
    defaultScheme.foregroundDisabled = 0x2E;
    defaultScheme.background = 0x30;
    defaultScheme.backgroundInactive = 0x5;
    defaultScheme.backgroundDisabled = 0x0;
    defaultScheme.text = 0x4;
    defaultScheme.textHighlight = 0x6;
    defaultScheme.textHighlightText = 0x30;
    defaultScheme.textInactive = 0x5;
    defaultScheme.textDisabled = 0x7;

    laScheme_Initialize(&textFieldScheme, GFX_COLOR_MODE_RGB_565);
    textFieldScheme.base = 0x0;
    textFieldScheme.highlight = 0x0;
    textFieldScheme.highlightLight = 0x30;
    textFieldScheme.shadow = 0x2E;
    textFieldScheme.shadowDark = 0x3;
    textFieldScheme.foreground = 0x4;
    textFieldScheme.foregroundInactive = 0x5;
    textFieldScheme.foregroundDisabled = 0x2E;
    textFieldScheme.background = 0x30;
    textFieldScheme.backgroundInactive = 0x5;
    textFieldScheme.backgroundDisabled = 0x0;
    textFieldScheme.text = 0x4;
    textFieldScheme.textHighlight = 0x6;
    textFieldScheme.textHighlightText = 0x30;
    textFieldScheme.textInactive = 0x5;
    textFieldScheme.textDisabled = 0x7;

    laScheme_Initialize(&titleScheme, GFX_COLOR_MODE_RGB_565);
    titleScheme.base = 0x0;
    titleScheme.highlight = 0x0;
    titleScheme.highlightLight = 0x30;
    titleScheme.shadow = 0x2E;
    titleScheme.shadowDark = 0x3;
    titleScheme.foreground = 0x4;
    titleScheme.foregroundInactive = 0x5;
    titleScheme.foregroundDisabled = 0x2E;
    titleScheme.background = 0x8;   // KEEP THIS LINE until MH3-4858 fixed
    titleScheme.backgroundInactive = 0x5;
    titleScheme.backgroundDisabled = 0x0;
    titleScheme.text = 0x4;
    titleScheme.textHighlight = 0x6;
    titleScheme.textHighlightText = 0x30;
    titleScheme.textInactive = 0x5;
    titleScheme.textDisabled = 0x7;

    laScheme_Initialize(&ButtonScheme, GFX_COLOR_MODE_RGB_565);
    ButtonScheme.base = 0x0;
    ButtonScheme.highlight = 0x0;
    ButtonScheme.highlightLight = 0x30;
    ButtonScheme.shadow = 0x2E;
    ButtonScheme.shadowDark = 0x3;
    ButtonScheme.foreground = 0xB;   // KEEP THIS LINE until MH3-4858 fixed
    ButtonScheme.foregroundInactive = 0x5;
    ButtonScheme.foregroundDisabled = 0x2E;
    ButtonScheme.background = 0x30;
    ButtonScheme.backgroundInactive = 0x5;
    ButtonScheme.backgroundDisabled = 0x0;
    ButtonScheme.text = 0x4;
    ButtonScheme.textHighlight = 0x6;
    ButtonScheme.textHighlightText = 0x30;
    ButtonScheme.textInactive = 0x5;
    ButtonScheme.textDisabled = 0x7;

    laScheme_Initialize(&whiteText, GFX_COLOR_MODE_RGB_565);
    whiteText.base = 0x0;
    whiteText.highlight = 0x0;
    whiteText.highlightLight = 0x30;
    whiteText.shadow = 0x2E;
    whiteText.shadowDark = 0x3;
    whiteText.foreground = 0x4;
    whiteText.foregroundInactive = 0x5;
    whiteText.foregroundDisabled = 0x2E;
    whiteText.background = 0x30;
    whiteText.backgroundInactive = 0x5;
    whiteText.backgroundDisabled = 0x0;
    whiteText.text = 0x1;            // KEEP THIS LINE until MH3-4858 fixed
    whiteText.textHighlight = 0x6;
    whiteText.textHighlightText = 0x30;
    whiteText.textInactive = 0x5;
    whiteText.textDisabled = 0x7;

    laScheme_Initialize(&backgroundScheme, GFX_COLOR_MODE_RGB_565);
    backgroundScheme.base = 0x0;
    backgroundScheme.highlight = 0x0;
    backgroundScheme.highlightLight = 0x30;
    backgroundScheme.shadow = 0x2E;
    backgroundScheme.shadowDark = 0x3;
    backgroundScheme.foreground = 0x4;
    backgroundScheme.foregroundInactive = 0x5;
    backgroundScheme.foregroundDisabled = 0x2E;
    backgroundScheme.background = 0x30;
    backgroundScheme.backgroundInactive = 0x5;
    backgroundScheme.backgroundDisabled = 0x0;
    backgroundScheme.text = 0x4;
    backgroundScheme.textHighlight = 0x6;
    backgroundScheme.textHighlightText = 0x30;
    backgroundScheme.textInactive = 0x5;
    backgroundScheme.textDisabled = 0x7;

    laScheme_Initialize(&defaultPaletteScheme, GFX_COLOR_MODE_RGB_565);
    defaultPaletteScheme.base = 0x0;
    defaultPaletteScheme.highlight = 0x0;
    defaultPaletteScheme.highlightLight = 0x30;
    defaultPaletteScheme.shadow = 0x2E;
    defaultPaletteScheme.shadowDark = 0x3;
    defaultPaletteScheme.foreground = 0x4;
    defaultPaletteScheme.foregroundInactive = 0x5;
    defaultPaletteScheme.foregroundDisabled = 0x2E;
    defaultPaletteScheme.background = 0x30;
    defaultPaletteScheme.backgroundInactive = 0x5;
    defaultPaletteScheme.backgroundDisabled = 0x0;
    defaultPaletteScheme.text = 0x4;
    defaultPaletteScheme.textHighlight = 0x6;
    defaultPaletteScheme.textHighlightText = 0x30;
    defaultPaletteScheme.textInactive = 0x5;
    defaultPaletteScheme.textDisabled = 0x7;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCU);
    GFX_Set(GFXF_GLOBAL_PALETTE, globalColorPalette);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_TRUE, LA_TRUE, &ScreenCreate_default);
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

    CircularVolume = laCircularSliderWidget_New();
    laWidget_SetPosition((laWidget*)CircularVolume, 325, 80);
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
    laCircularSliderWidget_SetArcScheme(CircularVolume, CIRCLE_BUTTON, &ButtonScheme);
    laCircularSliderWidget_SetValueChangedEventCallback(CircularVolume, &CircularVolume_ValueChangedEvent);
    laCircularSliderWidget_SetPressedEventCallback(CircularVolume, &CircularVolume_PressedEvent);
    laCircularSliderWidget_SetReleasedEventCallback(CircularVolume, &CircularVolume_ReleasedEvent);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)CircularVolume);

    Title = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)Title, 141, 13);
    laWidget_SetSize((laWidget*)Title, 202, 25);
    laWidget_SetScheme((laWidget*)Title, &titleScheme);
    laWidget_SetBackgroundType((laWidget*)Title, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)Title, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(Title, laString_CreateFromID(string_Title));
    laTextFieldWidget_SetCursorEnabled(Title, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(Title, LA_FALSE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)Title);

    delayAmount = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)delayAmount, 220, 127);
    laWidget_SetSize((laWidget*)delayAmount, 48, 25);
    laWidget_SetScheme((laWidget*)delayAmount, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)delayAmount, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)delayAmount, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)delayAmount);

    ImageWidget1 = laImageWidget_New();
    laWidget_SetSize((laWidget*)ImageWidget1, 103, 24);
    laWidget_SetScheme((laWidget*)ImageWidget1, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)ImageWidget1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)ImageWidget1, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(ImageWidget1, &microchip_logo103x24);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageWidget1);

    SliderWidgetMicGain = laSliderWidget_New();
    laWidget_SetPosition((laWidget*)SliderWidgetMicGain, 35, 91);
    laWidget_SetSize((laWidget*)SliderWidgetMicGain, 33, 100);
    laWidget_SetScheme((laWidget*)SliderWidgetMicGain, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)SliderWidgetMicGain, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SliderWidgetMicGain, LA_WIDGET_BORDER_BEVEL);
    laSliderWidget_SetGripSize(SliderWidgetMicGain, 20);
    laSliderWidget_SetValueChangedEventCallback(SliderWidgetMicGain, &SliderWidgetMicGain_ValueChangedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SliderWidgetMicGain);

    volumePercent = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)volumePercent, 361, 128);
    laWidget_SetSize((laWidget*)volumePercent, 48, 25);
    laWidget_SetScheme((laWidget*)volumePercent, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)volumePercent, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)volumePercent, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)volumePercent);

    CircularSliderDelay = laCircularSliderWidget_New();
    laWidget_SetPosition((laWidget*)CircularSliderDelay, 184, 80);
    laWidget_SetSize((laWidget*)CircularSliderDelay, 120, 121);
    laWidget_SetScheme((laWidget*)CircularSliderDelay, &defaultScheme);
    laWidget_SetBackgroundType((laWidget*)CircularSliderDelay, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)CircularSliderDelay, LA_WIDGET_BORDER_NONE);
    laCircularSliderWidget_SetRadius(CircularSliderDelay, 60);
    laCircularSliderWidget_SetStartAngle(CircularSliderDelay, 270);
    laCircularSliderWidget_SetValue(CircularSliderDelay, 25);
    laCircularSliderWidget_SetStickyButton(CircularSliderDelay, LA_FALSE);
    laCircularSliderWidget_SetTouchOnButtonOnly(CircularSliderDelay, LA_FALSE);
    laCircularSliderWidget_SetDirection(CircularSliderDelay, CIRCULAR_SLIDER_DIR_CLOCKWISE);
    laCircularSliderWidget_SetArcThickness(CircularSliderDelay, ACTIVE_AREA, 20);
    laCircularSliderWidget_SetArcScheme(CircularSliderDelay, CIRCLE_BUTTON, &ButtonScheme);
    laCircularSliderWidget_SetValueChangedEventCallback(CircularSliderDelay, &CircularSliderDelay_ValueChangedEvent);
    laCircularSliderWidget_SetPressedEventCallback(CircularSliderDelay, &CircularSliderDelay_PressedEvent);
    laCircularSliderWidget_SetReleasedEventCallback(CircularSliderDelay, &CircularSliderDelay_ReleasedEvent);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)CircularSliderDelay);

    LabelWidget6 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget6, 352, 204);
    laWidget_SetSize((laWidget*)LabelWidget6, 59, 35);
    laWidget_SetScheme((laWidget*)LabelWidget6, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget6, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget6, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget6, laString_CreateFromID(string_OutputVolume));
    laLabelWidget_SetHAlignment(LabelWidget6, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget6);

    delayLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)delayLabel, 205, 208);
    laWidget_SetSize((laWidget*)delayLabel, 78, 25);
    laWidget_SetScheme((laWidget*)delayLabel, &whiteText);
    laWidget_SetBackgroundType((laWidget*)delayLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)delayLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(delayLabel, laString_CreateFromID(string_Delay));
    laLabelWidget_SetHAlignment(delayLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)delayLabel);

    LabelWidget8 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget8, 105, 204);
    laWidget_SetSize((laWidget*)LabelWidget8, 60, 35);
    laWidget_SetScheme((laWidget*)LabelWidget8, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget8, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget8, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget8, laString_CreateFromID(string_EnableDelay));
    laLabelWidget_SetHAlignment(LabelWidget8, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget8);

    LabelWidget9 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget9, 29, 204);
    laWidget_SetSize((laWidget*)LabelWidget9, 40, 35);
    laWidget_SetScheme((laWidget*)LabelWidget9, &whiteText);
    laWidget_SetBackgroundType((laWidget*)LabelWidget9, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget9, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget9, laString_CreateFromID(string_MicGain));
    laLabelWidget_SetHAlignment(LabelWidget9, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LabelWidget9);

    ImageWidget = laImageWidget_New();
    laWidget_SetPosition((laWidget*)ImageWidget, 125, 120);
    laWidget_SetSize((laWidget*)ImageWidget, 31, 61);
    laWidget_SetScheme((laWidget*)ImageWidget, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)ImageWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)ImageWidget, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(ImageWidget, &toggleon61x31);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageWidget);

    micGainPercent = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)micGainPercent, 28, 56);
    laWidget_SetSize((laWidget*)micGainPercent, 48, 25);
    laWidget_SetScheme((laWidget*)micGainPercent, &backgroundScheme);
    laWidget_SetBackgroundType((laWidget*)micGainPercent, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)micGainPercent, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)micGainPercent);

    hiddenToggleOnButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)hiddenToggleOnButton, 114, 106);
    laWidget_SetSize((laWidget*)hiddenToggleOnButton, 50, 50);
    laWidget_SetScheme((laWidget*)hiddenToggleOnButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)hiddenToggleOnButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)hiddenToggleOnButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)hiddenToggleOnButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)hiddenToggleOnButton, 0);
    laButtonWidget_SetPressedEventCallback(hiddenToggleOnButton, &hiddenToggleOnButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)hiddenToggleOnButton);

    hiddenToggleOffButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)hiddenToggleOffButton, 114, 155);
    laWidget_SetSize((laWidget*)hiddenToggleOffButton, 50, 50);
    laWidget_SetScheme((laWidget*)hiddenToggleOffButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)hiddenToggleOffButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)hiddenToggleOffButton, LA_WIDGET_BORDER_BEVEL);
    laWidget_SetAlphaEnable((laWidget*)hiddenToggleOffButton, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)hiddenToggleOffButton, 0);
    laButtonWidget_SetPressedEventCallback(hiddenToggleOffButton, &hiddenToggleOffButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)hiddenToggleOffButton);

}




