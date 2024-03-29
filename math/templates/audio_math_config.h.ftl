<#--
/*******************************************************************************
  Audio Math Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    audio_math_config.h.ftl

  Summary:
    Audio math Freemarker Template File

  Description:

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

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

/*** Audio Math Configuration ***/
//NOTE:  This is not really used for anything
<#if CONFIG_USE_LIBQ_C_LIBRARY == true>
#define AUDIO_MATH_LIBQ_C_ENABLED
</#if>

<#if CONFIG_USE_LIBQ_NFO3_LIBRARY == true || CONFIG_USE_LIBQ_NFO3_LIBRARY == true >
#define AUDIO_MATH_LIBQ_ENABLED
</#if>

<#if CONFIG_USE_DSP_NFO3_LIBRARY == true || CONFIG_USE_DSP_NFOS_LIBRARY == true>
#define AUDIO_MATH_DSP_ENABLED
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
