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

#ifndef CHAR_CODE_H
#define CHAR_CODE_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

//#include "app.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"


#define ENCODING_ASCII          0x00
#define ENCODING_UTF16          0x01
#define ENCODING_UTF16_BE       0x02
#define ENCODING_UTF8           0x03

#define ENCODING_UNKNOWN_CODE   '?'

uint16_t ENCODING_FormatASCII ( uint8_t encoding, uint8_t* source, uint16_t sourceSize, int8_t* dest, uint16_t destSize );

#ifdef __cplusplus
}
#endif

#endif
