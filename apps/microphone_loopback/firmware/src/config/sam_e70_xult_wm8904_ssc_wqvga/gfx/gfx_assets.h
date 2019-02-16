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
 * Mode:   GS_8
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset blue_bg480x272;
	
/*********************************
 * GFX Image Asset
 * Name:   microchip_logo103x24
 * Size:   103x24 pixels
 * Mode:   INDEX_4
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset microchip_logo103x24;
	
/*********************************
 * GFX Image Asset
 * Name:   toggleon61x31
 * Size:   31x61 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset toggleon61x31;
	
/*********************************
 * GFX Image Asset
 * Name:   toggleoff61x31
 * Size:   31x61 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset toggleoff61x31;
	
/*********************************
 * GFX Image Asset
 * Name:   titlebg
 * Size:   42x30 pixels
 * Mode:   RGB_888
 * Format: RAW
 ***********************************/
extern GFXU_ImageAsset titlebg;
	
/*****************************************************************************
 * MPLAB Harmony Graphics Palette Assets
 *****************************************************************************/
/*********************************
 * GFX Palette Asset
 * Name:   microchip_logo103x24_palette
 * Colors: 6
 * Format: RGB_888
 ***********************************/
extern GFXU_PaletteAsset microchip_logo103x24_palette;
	
/*****************************************************************************
 * MPLAB Harmony Graphics Font Assets
 *****************************************************************************/
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans11
 * Height:       14
 * Style:        Plain
 * Glyph Count:  0
 * Range Count:  0
 ***********************************/
extern GFXU_FontAsset LiberationSans11;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans18
 * Height:       22
 * Style:        Plain
 * Glyph Count:  20
 * Range Count:  7
 * Glyph Ranges: 0x20
			     0x25
			     0x2B-0x2E
			     0x30-0x39
			     0x4E
			     0x65
			     0x6E-0x6F
 ***********************************/
extern GFXU_FontAsset LiberationSans18;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans14Bold
 * Height:       17
 * Style:        Bold
 * Glyph Count:  24
 * Range Count:  14
 * Glyph Ranges: 0x20
			     0x28-0x29
			     0x44-0x45
			     0x47
			     0x4D
			     0x4F
			     0x56
			     0x5C
			     0x61-0x63
			     0x65
			     0x69
			     0x6C-0x70
			     0x73-0x75
			     0x79
 ***********************************/
extern GFXU_FontAsset LiberationSans14Bold;
	
/*********************************
 * GFX Font Asset
 * Name:         LiberationSans18Bold
 * Height:       22
 * Style:        Bold
 * Glyph Count:  54
 * Range Count:  4
 * Glyph Ranges: 0x20
			     0x2D
			     0x41-0x5A
			     0x61-0x7A
 ***********************************/
extern GFXU_FontAsset LiberationSans18Bold;
	
/*****************************************************************************
 * MPLAB Harmony Graphics String Table
 *****************************************************************************/
/*********************************
 * GFX String Table
 * Name:         stringTable
 * Encoding:     ASCII
 * Languages:    default
 * String Count: 10
 ***********************************/
// language IDs
#define language_default    0

// string IDs
#define string_Delay    0
#define string_EnableDelay    1
#define string_MicGain    2
#define string_None    3
#define string_NumPunc18    4
#define string_NumTest    5
#define string_OutputVolume    6
#define string_StriAlphaNumPunc18Bold    7
#define string_Title    8
#define string_zero    9
 
extern GFXU_StringTableAsset stringTable;

/*****************************************************************************
 * MPLAB Harmony Graphics Global Palette
 *****************************************************************************/

extern uint16_t globalColorPalette[256];
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* GFX_ASSETS_H */

