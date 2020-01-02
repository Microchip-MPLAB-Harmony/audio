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
laScheme whiteText;
laScheme NewScheme;
laScheme ProgressBar;
laImageWidget* BackgroundImageWidget;
laImageWidget* MicrochipImageWidget;
laLabelWidget* AppTitleWidget;
laLabelWidget* lblSampleRate;
laLabelWidget* lblChannels;
laLabelWidget* lblBitDepth;
laCircularSliderWidget* VolumeWidget;
laLabelWidget* lblVolume;
laImageWidget* HeadPhoneEnableImg;
laLabelWidget* HPEnableLabelWidget;
laButtonWidget* HPEnableBtn;
laButtonWidget* HPDisableBtnWidget;
laLabelWidget* txtSampleRate;
laLabelWidget* txtChannels;
laLabelWidget* txtBitDepth;
laLabelWidget* txtVolumePercent;
laButtonWidget* RewindBtn;
laButtonWidget* NextTrackBtn;
laProgressBarWidget* ProgressBarWidget;
laLabelWidget* txtEndPosition;
laLabelWidget* txtCurrentPosition;
laButtonWidget* HiddenPlayStopBtn;
laImageWidget* PlayStopBtn;
laLabelWidget* lblFilename;
laLabelWidget* lblArtist;
laLabelWidget* lblTrackTitle;
laLabelWidget* lblAlbumTitle;
laLabelWidget* txtArtist;
laLabelWidget* txtFilename;
laLabelWidget* txtAlbumTitle;
laLabelWidget* txtTrackTitle;


static void ScreenCreate_GainLableWidget(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    laScheme_Initialize(&defaultScheme, GFX_COLOR_MODE_RGB_565);
    defaultScheme.base = 0xC67A;
    defaultScheme.highlight = 0xC67A;
    defaultScheme.highlightLight = 0xFFFF;
    defaultScheme.shadow = 0x8410;
    defaultScheme.shadowDark = 0x4208;
    defaultScheme.foreground = 0x0;
    defaultScheme.foregroundInactive = 0xD71C;
    defaultScheme.foregroundDisabled = 0x8410;
    defaultScheme.background = 0xFFFF;
    defaultScheme.backgroundInactive = 0xD71C;
    defaultScheme.backgroundDisabled = 0xC67A;
    defaultScheme.text = 0x0;
    defaultScheme.textHighlight = 0x1F;
    defaultScheme.textHighlightText = 0xFFFF;
    defaultScheme.textInactive = 0xD71C;
    defaultScheme.textDisabled = 0x8C92;

    laScheme_Initialize(&whiteText, GFX_COLOR_MODE_RGB_565);
    whiteText.base = 0xC67A;
    whiteText.highlight = 0xC67A;
    whiteText.highlightLight = 0xFFFF;
    whiteText.shadow = 0x8410;
    whiteText.shadowDark = 0x4208;
    whiteText.foreground = 0x0;
    whiteText.foregroundInactive = 0xD71C;
    whiteText.foregroundDisabled = 0x8410;
    whiteText.background = 0xFFFF;
    whiteText.backgroundInactive = 0xD71C;
    whiteText.backgroundDisabled = 0xC67A;
    whiteText.text = 0xFFFF;
    whiteText.textHighlight = 0x1F;
    whiteText.textHighlightText = 0xFFFF;
    whiteText.textInactive = 0xD71C;
    whiteText.textDisabled = 0x8C92;

    laScheme_Initialize(&NewScheme, GFX_COLOR_MODE_RGB_565);
    NewScheme.base = 0xC67A;
    NewScheme.highlight = 0xC67A;
    NewScheme.highlightLight = 0xFFFF;
    NewScheme.shadow = 0x8410;
    NewScheme.shadowDark = 0x4208;
    NewScheme.foreground = 0x0;
    NewScheme.foregroundInactive = 0xD71C;
    NewScheme.foregroundDisabled = 0x8410;
    NewScheme.background = 0xFFFF;
    NewScheme.backgroundInactive = 0xD71C;
    NewScheme.backgroundDisabled = 0xC67A;
    NewScheme.text = 0x0;
    NewScheme.textHighlight = 0x1F;
    NewScheme.textHighlightText = 0xFFFF;
    NewScheme.textInactive = 0xD71C;
    NewScheme.textDisabled = 0x8C92;

    laScheme_Initialize(&ProgressBar, GFX_COLOR_MODE_RGB_565);
    ProgressBar.base = 0xC67A;
    ProgressBar.highlight = 0xC67A;
    ProgressBar.highlightLight = 0xFFFF;
    ProgressBar.shadow = 0x8410;
    ProgressBar.shadowDark = 0x4208;
    ProgressBar.foreground = 0x0;
    ProgressBar.foregroundInactive = 0xD71C;
    ProgressBar.foregroundDisabled = 0x8410;
    ProgressBar.background = 0xFFFF;
    ProgressBar.backgroundInactive = 0xD71C;
    ProgressBar.backgroundDisabled = 0xC67A;
    ProgressBar.text = 0x0;
    ProgressBar.textHighlight = 0x5E0;
    ProgressBar.textHighlightText = 0xFFFF;
    ProgressBar.textInactive = 0xD71C;
    ProgressBar.textDisabled = 0x8C92;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCUGPU);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_GainLableWidget);
    laContext_AddScreen(screen);

    laContext_SetPreemptionLevel(LA_PREEMPTION_LEVEL_2);
    laContext_SetActiveScreen(0);

	return 0;
}

static void ScreenCreate_GainLableWidget(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_NONE);
    laLayer_SetBufferCount(layer0, 1);

    laScreen_SetLayer(screen, 0, layer0);

    BackgroundImageWidget = laImageWidget_New();
    laWidget_SetPosition((laWidget*)BackgroundImageWidget, 1, 1);
    laWidget_SetSize((laWidget*)BackgroundImageWidget, 480, 272);
    laWidget_SetBackgroundType((laWidget*)BackgroundImageWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)BackgroundImageWidget, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(BackgroundImageWidget, &blue_bg480x272);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)BackgroundImageWidget);

    MicrochipImageWidget = laImageWidget_New();
    laWidget_SetPosition((laWidget*)MicrochipImageWidget, 10, 13);
    laWidget_SetSize((laWidget*)MicrochipImageWidget, 103, 24);
    laWidget_SetBackgroundType((laWidget*)MicrochipImageWidget, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)MicrochipImageWidget, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(MicrochipImageWidget, &microchip_logo103x24);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)MicrochipImageWidget);

    AppTitleWidget = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)AppTitleWidget, 153, 10);
    laWidget_SetSize((laWidget*)AppTitleWidget, 189, 25);
    laWidget_SetBackgroundType((laWidget*)AppTitleWidget, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)AppTitleWidget, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(AppTitleWidget, laString_CreateFromID(string_strAudioPlayerDecoderTitleLabel));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)AppTitleWidget);

    lblSampleRate = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblSampleRate, 20, 65);
    laWidget_SetSize((laWidget*)lblSampleRate, 70, 20);
    laWidget_SetScheme((laWidget*)lblSampleRate, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblSampleRate, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblSampleRate, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblSampleRate, laString_CreateFromID(string_strSampleRateLabel));
    laLabelWidget_SetHAlignment(lblSampleRate, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblSampleRate);

    lblChannels = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblChannels, 20, 85);
    laWidget_SetSize((laWidget*)lblChannels, 55, 20);
    laWidget_SetScheme((laWidget*)lblChannels, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblChannels, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblChannels, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblChannels, laString_CreateFromID(string_strNoOfChannelsLabel));
    laLabelWidget_SetHAlignment(lblChannels, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblChannels);

    lblBitDepth = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblBitDepth, 20, 105);
    laWidget_SetSize((laWidget*)lblBitDepth, 50, 20);
    laWidget_SetScheme((laWidget*)lblBitDepth, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblBitDepth, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblBitDepth, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblBitDepth, laString_CreateFromID(string_strBitDepthLabel));
    laLabelWidget_SetHAlignment(lblBitDepth, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblBitDepth);

    VolumeWidget = laCircularSliderWidget_New();
    laWidget_SetPosition((laWidget*)VolumeWidget, 10, 125);
    laWidget_SetSize((laWidget*)VolumeWidget, 100, 107);
    laWidget_SetBackgroundType((laWidget*)VolumeWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)VolumeWidget, LA_WIDGET_BORDER_NONE);
    laCircularSliderWidget_SetStartAngle(VolumeWidget, 270);
    laCircularSliderWidget_SetValue(VolumeWidget, 75);
    laCircularSliderWidget_SetDirection(VolumeWidget, CIRCULAR_SLIDER_DIR_CLOCKWISE);
    laCircularSliderWidget_SetArcThickness(VolumeWidget, ACTIVE_AREA, 20);
    laCircularSliderWidget_SetValueChangedEventCallback(VolumeWidget, &VolumeWidget_ValueChangedEvent);
    laCircularSliderWidget_SetReleasedEventCallback(VolumeWidget, &VolumeWidget_ReleasedEvent);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)VolumeWidget);

    lblVolume = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblVolume, 35, 235);
    laWidget_SetSize((laWidget*)lblVolume, 50, 25);
    laWidget_SetScheme((laWidget*)lblVolume, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblVolume, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblVolume, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblVolume, laString_CreateFromID(string_strVolumeCntlLabel));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblVolume);

    HeadPhoneEnableImg = laImageWidget_New();
    laWidget_SetPosition((laWidget*)HeadPhoneEnableImg, 166, 167);
    laWidget_SetSize((laWidget*)HeadPhoneEnableImg, 33, 62);
    laWidget_SetBackgroundType((laWidget*)HeadPhoneEnableImg, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)HeadPhoneEnableImg, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(HeadPhoneEnableImg, &toggleon61x31);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)HeadPhoneEnableImg);

    HPEnableLabelWidget = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)HPEnableLabelWidget, 152, 227);
    laWidget_SetSize((laWidget*)HPEnableLabelWidget, 60, 30);
    laWidget_SetScheme((laWidget*)HPEnableLabelWidget, &whiteText);
    laWidget_SetBackgroundType((laWidget*)HPEnableLabelWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)HPEnableLabelWidget, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(HPEnableLabelWidget, laString_CreateFromID(string_strHPEnableLabel));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)HPEnableLabelWidget);

    HPEnableBtn = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HPEnableBtn, 150, 136);
    laWidget_SetSize((laWidget*)HPEnableBtn, 67, 61);
    laWidget_SetBackgroundType((laWidget*)HPEnableBtn, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)HPEnableBtn, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedEventCallback(HPEnableBtn, &HPEnableBtn_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HPEnableBtn);

    HPDisableBtnWidget = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HPDisableBtnWidget, 150, 198);
    laWidget_SetSize((laWidget*)HPDisableBtnWidget, 67, 61);
    laWidget_SetBackgroundType((laWidget*)HPDisableBtnWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)HPDisableBtnWidget, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedEventCallback(HPDisableBtnWidget, &HPDisableBtnWidget_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HPDisableBtnWidget);

    txtSampleRate = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtSampleRate, 90, 65);
    laWidget_SetSize((laWidget*)txtSampleRate, 50, 20);
    laWidget_SetScheme((laWidget*)txtSampleRate, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtSampleRate, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtSampleRate, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtSampleRate, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtSampleRate);

    txtChannels = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtChannels, 76, 85);
    laWidget_SetSize((laWidget*)txtChannels, 50, 20);
    laWidget_SetScheme((laWidget*)txtChannels, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtChannels, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtChannels, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtChannels, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtChannels);

    txtBitDepth = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtBitDepth, 71, 105);
    laWidget_SetSize((laWidget*)txtBitDepth, 25, 20);
    laWidget_SetScheme((laWidget*)txtBitDepth, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtBitDepth, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtBitDepth, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtBitDepth, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtBitDepth);

    txtVolumePercent = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtVolumePercent, 45, 168);
    laWidget_SetSize((laWidget*)txtVolumePercent, 30, 25);
    laWidget_SetScheme((laWidget*)txtVolumePercent, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtVolumePercent, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtVolumePercent, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(txtVolumePercent, laString_CreateFromID(string_strVolumePercentage));
    laLabelWidget_SetHAlignment(txtVolumePercent, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtVolumePercent);

    RewindBtn = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)RewindBtn, 235, 190);
    laWidget_SetSize((laWidget*)RewindBtn, 56, 60);
    laWidget_SetBackgroundType((laWidget*)RewindBtn, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)RewindBtn, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetToggleable(RewindBtn, LA_TRUE);
    laButtonWidget_SetReleasedImage(RewindBtn, &rwdBtn30x30);
    laButtonWidget_SetPressedEventCallback(RewindBtn, &RewindBtn_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)RewindBtn);

    NextTrackBtn = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)NextTrackBtn, 390, 190);
    laWidget_SetSize((laWidget*)NextTrackBtn, 55, 60);
    laWidget_SetBackgroundType((laWidget*)NextTrackBtn, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)NextTrackBtn, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetReleasedImage(NextTrackBtn, &nexttrack30x30);
    laButtonWidget_SetPressedEventCallback(NextTrackBtn, &NextTrackBtn_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)NextTrackBtn);

    ProgressBarWidget = laProgressBarWidget_New();
    laWidget_SetPosition((laWidget*)ProgressBarWidget, 235, 137);
    laWidget_SetSize((laWidget*)ProgressBarWidget, 200, 15);
    laWidget_SetScheme((laWidget*)ProgressBarWidget, &ProgressBar);
    laWidget_SetBackgroundType((laWidget*)ProgressBarWidget, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)ProgressBarWidget, LA_WIDGET_BORDER_BEVEL);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ProgressBarWidget);

    txtEndPosition = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtEndPosition, 438, 134);
    laWidget_SetSize((laWidget*)txtEndPosition, 32, 20);
    laWidget_SetScheme((laWidget*)txtEndPosition, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtEndPosition, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtEndPosition, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(txtEndPosition, laString_CreateFromID(string_strTotalTime));
    laLabelWidget_SetHAlignment(txtEndPosition, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtEndPosition);

    txtCurrentPosition = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtCurrentPosition, 199, 134);
    laWidget_SetSize((laWidget*)txtCurrentPosition, 32, 20);
    laWidget_SetScheme((laWidget*)txtCurrentPosition, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtCurrentPosition, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtCurrentPosition, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(txtCurrentPosition, laString_CreateFromID(string_strStartTime));
    laLabelWidget_SetHAlignment(txtCurrentPosition, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtCurrentPosition);

    HiddenPlayStopBtn = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)HiddenPlayStopBtn, 302, 190);
    laWidget_SetSize((laWidget*)HiddenPlayStopBtn, 76, 60);
    laWidget_SetBackgroundType((laWidget*)HiddenPlayStopBtn, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)HiddenPlayStopBtn, LA_WIDGET_BORDER_NONE);
    laWidget_SetAlphaEnable((laWidget*)HiddenPlayStopBtn, GFX_TRUE);
    laWidget_SetAlphaAmount((laWidget*)HiddenPlayStopBtn, 255);
    laButtonWidget_SetPressedOffset(HiddenPlayStopBtn, 5);
    laButtonWidget_SetPressedEventCallback(HiddenPlayStopBtn, &HiddenPlayStopBtn_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)HiddenPlayStopBtn);

    PlayStopBtn = laImageWidget_New();
    laWidget_SetPosition((laWidget*)PlayStopBtn, 27, 14);
    laWidget_SetSize((laWidget*)PlayStopBtn, 30, 30);
    laWidget_SetBackgroundType((laWidget*)PlayStopBtn, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)PlayStopBtn, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(PlayStopBtn, &pauseBtn);
    laWidget_AddChild((laWidget*)HiddenPlayStopBtn, (laWidget*)PlayStopBtn);

    lblFilename = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblFilename, 20, 45);
    laWidget_SetSize((laWidget*)lblFilename, 70, 20);
    laWidget_SetScheme((laWidget*)lblFilename, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblFilename, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblFilename, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblFilename, laString_CreateFromID(string_strFilenameLabel));
    laLabelWidget_SetHAlignment(lblFilename, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblFilename);

    lblArtist = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblArtist, 151, 65);
    laWidget_SetSize((laWidget*)lblArtist, 46, 20);
    laWidget_SetScheme((laWidget*)lblArtist, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblArtist, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblArtist, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblArtist, laString_CreateFromID(string_strArtistLabel));
    laLabelWidget_SetHAlignment(lblArtist, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblArtist);

    lblTrackTitle = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblTrackTitle, 151, 85);
    laWidget_SetSize((laWidget*)lblTrackTitle, 46, 20);
    laWidget_SetScheme((laWidget*)lblTrackTitle, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblTrackTitle, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblTrackTitle, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblTrackTitle, laString_CreateFromID(string_strTrackLabel));
    laLabelWidget_SetHAlignment(lblTrackTitle, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblTrackTitle);

    lblAlbumTitle = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lblAlbumTitle, 151, 105);
    laWidget_SetSize((laWidget*)lblAlbumTitle, 46, 20);
    laWidget_SetScheme((laWidget*)lblAlbumTitle, &whiteText);
    laWidget_SetBackgroundType((laWidget*)lblAlbumTitle, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lblAlbumTitle, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lblAlbumTitle, laString_CreateFromID(string_strAlbumLabel));
    laLabelWidget_SetHAlignment(lblAlbumTitle, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lblAlbumTitle);

    txtArtist = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtArtist, 203, 65);
    laWidget_SetSize((laWidget*)txtArtist, 231, 20);
    laWidget_SetScheme((laWidget*)txtArtist, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtArtist, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtArtist, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtArtist, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtArtist);

    txtFilename = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtFilename, 72, 45);
    laWidget_SetSize((laWidget*)txtFilename, 100, 20);
    laWidget_SetScheme((laWidget*)txtFilename, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtFilename, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtFilename, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtFilename, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtFilename);

    txtAlbumTitle = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtAlbumTitle, 201, 104);
    laWidget_SetSize((laWidget*)txtAlbumTitle, 231, 20);
    laWidget_SetScheme((laWidget*)txtAlbumTitle, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtAlbumTitle, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtAlbumTitle, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtAlbumTitle, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtAlbumTitle);

    txtTrackTitle = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)txtTrackTitle, 203, 84);
    laWidget_SetSize((laWidget*)txtTrackTitle, 231, 20);
    laWidget_SetScheme((laWidget*)txtTrackTitle, &whiteText);
    laWidget_SetBackgroundType((laWidget*)txtTrackTitle, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtTrackTitle, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(txtTrackTitle, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtTrackTitle);

}




