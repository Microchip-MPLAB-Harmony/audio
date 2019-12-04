/*******************************************************************************
  MPLAB Harmony Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    adpcm_enc.c

  Summary:
    adpcm encoder source file.

  Description:
    adpcm encoder source file.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "adpcm_enc.h"
#include "encoder.h"
#include<string.h>
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* ADPCM Decoder State Structure
	Summary:
		APDCM encoder state structure
	Description:
		This structure holds current adpcm encoder state and byte order.
*/
typedef struct 
{
	int			prevsample;		/* Predicted adpcm sample */
	int			previndex;		/* Index into step size table */
	bool		isLE;			/* ADPCM stream in little endian format*/
}ADPCMState;

/*
* Local ADPCM state instance
*/
static ADPCMState _adpcm_state;

/* Table of index changes */
static const int indexTable[16] = 
{
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};

/* Quantizer step size lookup table */
static const int stepSizeTable[89] = 
{
   7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
   19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
   50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
   130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
   337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
   876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
   2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
   5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
   15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*
Local ADPCM decoder function declarition
*/
static unsigned char adpcm_encode_sample( short sample, ADPCMState *state);

/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

bool adpcm_encode_frame(void *pin, uint32_t insize, void *pout, uint32_t *outsize)
{    
    unsigned char	adPCMLo;	// the encoded value
	unsigned char	adPCMHi;	// the encoded value
	unsigned char* dstBuff=(unsigned char*)pout;
	short* srcBuff= (short*)pin;
	int	nSamples=insize/2;
    *outsize = nSamples/2;

	if (_adpcm_state.isLE)
	{	
		while(nSamples/2)
		{
            // two shorts into one char
			adPCMHi=adpcm_encode_sample( *srcBuff++, &_adpcm_state);
			adPCMLo=adpcm_encode_sample( *srcBuff++, &_adpcm_state);
	   		*dstBuff++ = adPCMHi|(adPCMLo<<4);	// pack the ADPCM sample
			nSamples-=2;
		}
	}
	else
	{
		while(nSamples/2)
		{
			adPCMHi=adpcm_encode_sample( *srcBuff++, &_adpcm_state);
			adPCMLo=adpcm_encode_sample( *srcBuff++, &_adpcm_state);
			*dstBuff++=adPCMLo|(adPCMHi<<4);
			nSamples-=2;
		}
	}
    return true;
}
/* init wav encoder structure */
bool adpcm_encoder_init(int channel, int inputSampleRate)
{
    memset((void*)&_adpcm_state,0, sizeof(ADPCMState));
    return true;
}

bool adpcm_encoder_free()
{
    return true;
}

void adpcm_encoder_config_byteorder(bool isLE)
{
	_adpcm_state.isLE = isLE;
}

/****************************************************************************
 * Function:        ADPCMEncodeSample
 *
 * PreCondition:    adpcm State has to configured
 *
 * Input:           short sample - 16-bit signed speech sample  
 *
 * Output:          char - 8-bit number containing the 4-bit ADPCM code 
 *
 * Side Effects:    None
 *
 * Overview:        This function encode the 16-bit speech sample into
 *                  8-bit code containing 4-bit ADPCM value. Refer the
 *                  the comments given inline for more details of operation.
 *
 * Note:            None
 *****************************************************************************/

static unsigned char adpcm_encode_sample( short sample, ADPCMState *state)
{

   int step;		/* Quantizer step size */
   int predsample;		/* Output of ADPCM predictor */
   int diffq,diff;		/* Dequantized predicted difference */
   int code;
   int index;		/* Index into step size table */

	// Restore previous values of predicted sample and quantizer step
	// size index
	predsample = state->prevsample;
	index = state->previndex;
	step = stepSizeTable[index];

	// Compute the difference between the acutal sample (sample) and the
	// the predicted sample (predsample)
	diff = sample - predsample;
	if(diff >= 0)
	{
		code = 0;
	}
	else
	{
		code = 8;
		diff = -diff;
	}

	// Quantize the difference into the 4-bit ADPCM code using the
	// the quantizer step size
	// Inverse quantize the ADPCM code into a predicted difference
	// using the quantizer step size
	diffq = step >> 3;
	if( diff >= step )
	{
		code |= 4;
		diff -= step;
		diffq += step;
	}
	step >>= 1;
	if( diff >= step )
	{
		code |= 2;
		diff -= step;
		diffq += step;
	}
	step >>= 1;
	if( diff >= step )
	{
		code |= 1;
		diffq += step;
	}

	// Fixed predictor computes new predicted sample by adding the
	// old predicted sample to predicted difference
	if( code & 8 )
	{
		predsample -= diffq;
	}
	else
	{
		predsample += diffq;
	}


	if(predsample<-32768)
	{
		predsample=-32768;
	}
	else if(predsample>32767)
	{
		predsample=32767;
	}
	
	// Find new quantizer stepsize index by adding the old index
	// to a table lookup using the ADPCM code
	index += indexTable[code];

	// Check for overflow of the new quantizer step size index
	if( index < 0 )
	{
		index = 0;
	}
	if( index > 88 )
	{
		index = 88;
	}

	// Save the predicted sample and quantizer step size index for
	// next iteration
	state->prevsample = predsample;
	state->previndex = index;

	// Return the new ADPCM code
	return ( code & 0x0f );
}


const HAR_ENCODER adpcmEncoderInst ={
	ADPCM,
	adpcm_encoder_init,
	adpcm_encoder_free,
	adpcm_encode_frame
};
/* *****************************************************************************
 End of File
 */
