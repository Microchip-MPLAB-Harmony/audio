/*******************************************************************************
	ADPCM Decoder Header File

	Company:
		Microchip Technology Inc.

	File Name:
		adpcm_enc.h

	Summary:
		Contains the ADPCM encoder specific defintions and function prototypes.

	Description:
		This file contains the ADPCM encoder specific defintions and function
		prototypes.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2012 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END 

#ifndef _ADPCM_ENC_H    /* Guard against multiple inclusion */
#define _ADPCM_ENC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include<stdbool.h>
#include<stdint.h>
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    bool adpcm_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize);
    bool adpcm_encoder_init();
    bool adpcm_encoder_free();
	void adpcm_encoder_config_byteorder(bool isLE);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ADPCM_ENC_H */

/* *****************************************************************************
 End of File
 */
