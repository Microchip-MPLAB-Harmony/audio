/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _XINDEGREES_LUT_91_     // Guards against multiple inclusion
#define _XINDEGREES_LUT_91_

static const fract_q15_t _1stQuadrantX_LUT[] =  //1x91 LUT, Degrees in Q9.6
{ 0x0000, 0x0040, 0x0080, 0x00C0, 0x0100, 0x0140, 0x0180, 0x01C0, 0x0200,
  0x0240, 0x0280, 0x02C0, 0x0300, 0x0340, 0x0380, 0x03C0, 0x0400, 0x0440,
  0x0480, 0x04C0, 0x0500, 0x0540, 0x0580, 0x05C0, 0x0600, 0x0640, 0x0680,
  0x06C0, 0x0700, 0x0740, 0x0780, 0x07C0, 0x0800, 0x0840, 0x0880, 0x08C0,
  0x0900, 0x0940, 0x0980, 0x09C0, 0x0A00, 0x0A40, 0x0A80, 0x0AC0, 0x0B00,
  0x0B40, 0x0B80, 0x0BC0, 0x0C00, 0x0C40, 0x0C80, 0x0CC0, 0x0D00, 0x0D40,
  0x0D80, 0x0DC0, 0x0E00, 0x0E40, 0x0E80, 0x0EC0, 0x0F00, 0x0F40, 0x0F80,
  0x0FC0, 0x1000, 0x1040, 0x1080, 0x10C0, 0x1100, 0x1140, 0x1180, 0x11C0,
  0x1200, 0x1240, 0x1280, 0x12C0, 0x1300, 0x1340, 0x1380, 0x13C0, 0x1400,
  0x1440, 0x1480, 0x14C0, 0x1500, 0x1540, 0x1580, 0x15C0, 0x1600, 0x1640,
  0x1680 };

 #endif//def _XINDEGREES_LUT_91_
