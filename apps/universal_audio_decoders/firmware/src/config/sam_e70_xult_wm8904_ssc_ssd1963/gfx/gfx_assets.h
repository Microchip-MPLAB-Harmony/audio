/*******************************************************************************
  MPLAB Harmony Graphics Asset Header File

  File Name:
    gfx_assets.h

  Summary:
    Header file containing a list of asset specifications for use with the
	MPLAB Harmony Graphics Stack.

  Description:
    Header file containing a list of asset specifications for use with the
	MPLAB Harmony Graphics Stack.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef GFX_ASSETS_H
#define GFX_ASSETS_H

#include "gfx/utils/gfx_utils.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END 

/*** Generated Asset Descriptors ***/
/*****************************************************************************
 * MPLAB Harmony Graphics Asset Location IDs
 *****************************************************************************/
#define GFXU_ASSET_LOCATION_ID_INTERNAL    0
 
/*****************************************************************************
 * MPLAB Harmony Graphics Image Assets
 *****************************************************************************/
/*********************************
 * GFX Image Asset
 * Name:   blue_bg480x272
 * Size:   480x272 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset blue_bg480x272;
	
/*********************************
 * GFX Image Asset
 * Name:   microchip_logo103x24
 * Size:   103x24 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset microchip_logo103x24;
	
/*********************************
 * GFX Image Asset
 * Name:   toggleoff61x31
 * Size:   31x61 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset toggleoff61x31;
	
/*********************************
 * GFX Image Asset
 * Name:   toggleon61x31
 * Size:   31x61 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset toggleon61x31;
	
/*********************************
 * GFX Image Asset
 * Name:   PauseBtn30x30
 * Size:   30x30 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset PauseBtn30x30;
	
/*********************************
 * GFX Image Asset
 * Name:   PlaybackBtn30x30
 * Size:   30x30 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset PlaybackBtn30x30;
	
/*********************************
 * GFX Image Asset
 * Name:   StopBtn30x30
 * Size:   30x30 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset StopBtn30x30;
	
/*********************************
 * GFX Image Asset
 * Name:   nexttrack30x30
 * Size:   30x30 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset nexttrack30x30;
	
/*********************************
 * GFX Image Asset
 * Name:   rwdBtn30x30
 * Size:   30x30 pixels
 * Mode:   RGB_565
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset rwdBtn30x30;
	
/*********************************
 * GFX Image Asset
 * Name:   pauseBtn
 * Size:   30x30 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset pauseBtn;
	
/*********************************
 * GFX Image Asset
 * Name:   pauseBtnPressed
 * Size:   30x30 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset pauseBtnPressed;
	
/*********************************
 * GFX Image Asset
 * Name:   playBtn
 * Size:   30x32 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset playBtn;
	
/*********************************
 * GFX Image Asset
 * Name:   playBtnPressed
 * Size:   30x32 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset playBtnPressed;
	
/*****************************************************************************
 * MPLAB Harmony Graphics Font Assets
 *****************************************************************************/
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans_Bold
 * Height:       14
 * Style:        Plain
 * Glyph Count:  16
 * Range Count:  11
 * Glyph Ranges: 0x20
			     0x41
			     0x44
			     0x55
			     0x61
			     0x63-0x65
			     0x69
			     0x6C
			     0x6E-0x6F
			     0x72-0x73
			     0x75-0x76
 ***********************************/
extern GFXU_FontAsset LiberationSans_Bold;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans_BoldItalic
 * Height:       14
 * Style:        Plain
 * Glyph Count:  0
 * Range Count:  0
 ***********************************/
extern GFXU_FontAsset LiberationSans_BoldItalic;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans_Italic
 * Height:       14
 * Style:        Plain
 * Glyph Count:  0
 * Range Count:  0
 ***********************************/
extern GFXU_FontAsset LiberationSans_Italic;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans_Regular
 * Height:       14
 * Style:        Plain
 * Glyph Count:  95
 * Range Count:  1
 * Glyph Ranges: 0x20-0x7E
 ***********************************/
extern GFXU_FontAsset LiberationSans_Regular;
	
/*****************************************************************************
 * MPLAB Harmony Graphics String Table
 *****************************************************************************/
/*********************************
 * GFX String Table
 * Name:         stringTable
 * Encoding:     ASCII
 * Languages:    default
 * String Count: 16
 ***********************************/
// language IDs
#define language_default    0

// string IDs
#define string_StringChars    0
#define string_strAlbumLabel    1
#define string_strArtistLabel    2
#define string_strAudioPlayerDecoderTitleLabel    3
#define string_strBitDepthLabel    4
#define string_strContainerTypeLabel    5
#define string_strFilenameLabel    6
#define string_strHPEnableLabel    7
#define string_strNoOfChannelsLabel    8
#define string_strSampleRateLabel    9
#define string_strStartTime    10
#define string_strTitleLabel    11
#define string_strTotalTime    12
#define string_strTrackLabel    13
#define string_strVolumeCntlLabel    14
#define string_strVolumePercentage    15
 
extern GFXU_StringTableAsset stringTable;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* GFX_ASSETS_H */

