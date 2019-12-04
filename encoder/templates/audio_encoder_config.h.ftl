<#--
/*******************************************************************************
  Audio Decoder Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    audio_encoders_config.h.ftl

  Summary:
    Audio Encoders Freemarker Template File

  Description:

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->

/*** Audio Encoders Configuration ***/
<#if CONFIG_USE_PCM_ENCODER>
#define PCM_ENCODER
</#if>
<#if CONFIG_USE_ADPCM_ENCODER>
#define ADPCM_ENCODER
</#if>
<#if CONFIG_USE_OPUS_ENCODER>
#define OPUS_ENCODER
</#if>
<#if CONFIG_USE_SPEEX_ENCODER>
#define SPEEX_ENCODER
</#if>
<#if CONFIG_USE_WAV_CONTAINER>
#define WAV_CONTAINER
</#if>
<#if CONFIG_USE_OGG_CONTAINER>
#define OGG_CONTAINER
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
