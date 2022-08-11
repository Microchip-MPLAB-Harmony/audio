/*******************************************************************************
  CLASS D PLIB Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    plib_classd.c

  Summary:
    Function implementations for the CLASS D PLIB.

  Description:
    This PLIB provides a simple interface to configure the Audio CLASS D 
    Amplifier.

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************
#include "plib_classd.h"
<#if tr == "dma">
#include "../src/config/${__CONFIGURATION_NAME}/peripheral/xdmac/plib_xdmac.h"

typedef struct
{
    CLASSD_DMA_CALLBACK classd_app_callback;
    uintptr_t              context;
} CLASSD_DMA_OBJECT ;

CLASSD_DMA_OBJECT classDdmaObj;
</#if>
// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************
<#if tr == "dma">
void classD_dma_complete(XDMAC_TRANSFER_EVENT status, uintptr_t context)
{
    CLASSD_DMA_TRANSFER_EVENT classdStatus = CLASSD_DMA_TRANSFER_NONE;
    if( status==XDMAC_TRANSFER_COMPLETE )
        classdStatus = CLASSD_DMA_TRANSFER_COMPLETE;
    else
        classdStatus = CLASSD_DMA_TRANSFER_ERROR;
    classDdmaObj.classd_app_callback(classdStatus, 0);
}
</#if>
static void setCLASSD_Frame(void)
{
<#if frame == "FRAME_8k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_12M288 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_8K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_16k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_12M288 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_16K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_32k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_12M288 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_32K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_48k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_12M288 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_48K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_96k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_12M288 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_96K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_22k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_11M2896 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_22K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#elseif frame == "FRAME_44k">
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_11M2896 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_44K;   //CLASSD_INTPMR_FRAME_FRAME_48K
<#else>
    CLASSD_REGS->CLASSD_INTPMR |= 
            CLASSD_INTPMR_DSPCLKFREQ_11M2896 //CLASSD_INTPMR_DSPCLKFREQ_12M288
          | CLASSD_INTPMR_FRAME_FRAME_88K;   //CLASSD_INTPMR_FRAME_FRAME_48K
</#if>
}

static void setCLASSD_OutputMode(void)
{
    uint32_t mr = 0;
<#if output == "FULL_BRIDGE">
    mr |= CLASSD_MR_PWMTYP_Msk|CLASSD_MR_NON_OVERLAP_Msk;
<#elseif output == "HALF_BRIDGE"> 
    mr |= CLASSD_MR_NON_OVERLAP_Msk;
<#elseif output == "DIFFERENTIAL">    
    mr |= CLASSD_MR_PWMTYP_Msk;
</#if>

<#if output == "FULL_BRIDGE" || output == "HALF_BRIDGE">
    /* configure non-overlapping time */
<#if NOVRVAL == "5NS">	
    mr |= CLASSD_MR_NOVRVAL_5NS;
<#elseif NOVRVAL == "10NS"> 
    mr |= CLASSD_MR_NOVRVAL_10NS;
<#elseif NOVRVAL == "15NS"> 
    mr |= CLASSD_MR_NOVRVAL_15NS;
<#else>
    mr |= CLASSD_MR_NOVRVAL_20NS;
</#if>   
</#if>  
    CLASSD_REGS->CLASSD_MR |= mr;
}
<#if Mono == true>  
static void setCLASSD_MonoMode(void)
{
<#if MonoMode == "MONOMIX">
    CLASSD_REGS->CLASSD_INTPMR |= CLASSD_INTPMR_MONO_ENABLED| CLASSD_INTPMR_MONOMODE_MONOMIX;
<#elseif MonoMode == "MONOSAT">
	CLASSD_REGS->CLASSD_INTPMR |= CLASSD_INTPMR_MONO_ENABLED| CLASSD_INTPMR_MONOMODE_MONOSAT;
<#elseif MonoMode == "MONOLEFT">
	CLASSD_REGS->CLASSD_INTPMR |= CLASSD_INTPMR_MONO_ENABLED| CLASSD_INTPMR_MONOMODE_MONOLEFT;
<#else>
	CLASSD_REGS->CLASSD_INTPMR |= CLASSD_INTPMR_MONO_ENABLED| CLASSD_INTPMR_MONOMODE_MONORIGHT;
</#if>
}
</#if>
static void setCLASSD_EQCFG(void)
{
<#if eqcfg == "FLAT">
    uint32_t bits = CLASSD_INTPMR_EQCFG_FLAT;
<#elseif eqcfg == "BBOOST12">
    uint32_t bits = CLASSD_INTPMR_EQCFG_BBOOST12;
<#elseif eqcfg == "BBOOST6">
    uint32_t bits = CLASSD_INTPMR_EQCFG_BBOOST6;
<#elseif eqcfg == "BCUT12">
    uint32_t bits = CLASSD_INTPMR_EQCFG_BCUT12;
<#elseif eqcfg == "BCUT6">
    uint32_t bits = CLASSD_INTPMR_EQCFG_BCUT6;
<#elseif eqcfg == "MBOOST3">
    uint32_t bits = CLASSD_INTPMR_EQCFG_MBOOST3;
<#elseif eqcfg == "MBOOST8">
    uint32_t bits = CLASSD_INTPMR_EQCFG_MBOOST8;
<#elseif eqcfg == "MCUT3">
    uint32_t bits = CLASSD_INTPMR_EQCFG_MCUT3;
<#elseif eqcfg == "MCUT8">
    uint32_t bits = CLASSD_INTPMR_EQCFG_MCUT8;
<#elseif eqcfg == "TBOOST12">
    uint32_t bits = CLASSD_INTPMR_EQCFG_TBOOST12;
<#elseif eqcfg == "TBOOST6">
    uint32_t bits = CLASSD_INTPMR_EQCFG_TBOOST6;
<#elseif eqcfg == "TCUT12">
    uint32_t bits = CLASSD_INTPMR_EQCFG_TCUT12;	
<#else>
    uint32_t bits = CLASSD_INTPMR_EQCFG_TCUT6;
</#if>
    CLASSD_REGS->CLASSD_INTPMR |= (CLASSD_REGS->CLASSD_INTPMR & ~CLASSD_INTPMR_EQCFG_Msk) | bits;
}


// *****************************************************************************
// *****************************************************************************
// Section: CLASS D Implementation
// *****************************************************************************
// *****************************************************************************
/******************************************************************************
  Function:
    void CLASSD_Transfer(uint8_t* data, uint32_t length)

  Summary:
    Transmits the raw music data to CLASSD_THR register.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_Transfer(uint8_t* data, uint32_t length)
{   
<#if tr == "dma">
    XDMAC_ChannelTransfer(XDMAC_CHANNEL_0, (uint32_t*)data, (uint32_t*)&CLASSD_REGS->CLASSD_THR, length);     
<#else>                
    uint16_t* start = (uint16_t*)data;
    uint16_t* end = start + length;
	uint16_t* current = start;
    
    while (1) 
    {
		if (CLASSD_REGS->CLASSD_ISR & CLASSD_ISR_DATRDY_Msk) 
		{
			uint16_t left = 0;
			left = *current++;
			CLASSD_REGS->CLASSD_THR = CLASSD_THR_LDATA(left);
		}
		if (current >= end)
			break;
	}
</#if>
}

/******************************************************************************
  Function:
    CLASSD_ChannelDisable(  bool left, bool right )

  Summary:
    Enables the left/right channel.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_ChannelDisable(  bool left, bool right )
{   
    uint32_t channel=0;
    if(left)
        channel |= CLASSD_MR_LEN_Msk;
    if(right)
        channel |= CLASSD_MR_REN_Msk;
    CLASSD_REGS->CLASSD_MR |=  ~channel;
}

/******************************************************************************
  Function:
    CLASSD_ChannelEnable(  bool left, bool right )

  Summary:
    Enables the left/right channel.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_ChannelEnable(  bool left, bool right )
{   
    uint32_t channel=0;
    if(left)
        channel |= CLASSD_MR_LEN_Msk;
    if(right)
        channel |= CLASSD_MR_REN_Msk;
    CLASSD_REGS->CLASSD_MR |=  channel;                                             
}

/******************************************************************************
  Function:
    CLASSD_AttenuationSet( uint8_t chnl, uint8_t attn )

  Summary:
    Sets the attenuation for  left/right channel.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_AttenuationSet( CLASSD_CHANNEL chnl, uint8_t attn )
{  
    uint32_t intpmr=0;
    if (attn > (CLASSD_INTPMR_ATTL_Msk >> CLASSD_INTPMR_ATTL_Pos))
		attn = (CLASSD_INTPMR_ATTL_Msk >> CLASSD_INTPMR_ATTL_Pos);
    
    if(chnl == CLASSD_LEFT)
    {
        intpmr = CLASSD_REGS->CLASSD_INTPMR & ~CLASSD_INTPMR_ATTL_Msk;
        CLASSD_REGS->CLASSD_INTPMR = intpmr | CLASSD_INTPMR_ATTL(attn);
    }
    else
    {
        intpmr = CLASSD_REGS->CLASSD_INTPMR & ~CLASSD_INTPMR_ATTR_Msk;
        CLASSD_REGS->CLASSD_INTPMR = intpmr | CLASSD_INTPMR_ATTR(attn);
    }                                      
}

/******************************************************************************
  Function:
    CLASSD_UnMute(  bool left, bool right )

  Summary:
    Sets the channel to unmute.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_UnMute(  bool left, bool right )
{   
    uint32_t mute=0;
    if(left)
        mute |= CLASSD_MR_LMUTE_Msk;
    else
        mute |= CLASSD_MR_RMUTE_Msk;
    CLASSD_REGS->CLASSD_MR |=  ~mute;                                             
}

/******************************************************************************
  Function:
    CLASSD_Mute(  bool left, bool right )

  Summary:
    Sets the channel to mute.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_Mute(  bool left, bool right )
{   
    uint32_t mute=0;
    if(left)
        mute |= CLASSD_MR_LMUTE_Msk;
    if(right)
        mute |= CLASSD_MR_RMUTE_Msk;
    CLASSD_REGS->CLASSD_MR |=  mute;                                             
}
<#if tr == "dma">
/******************************************************************************
  Function:
    CLASSD_CallbackRegister ( const XDMAC_CHANNEL_CALLBACK eventHandler, const uintptr_t contextHandle )

  Summary:
    Allows application to register a callback when DMA is used for CLASS D.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_CallbackRegister( const CLASSD_DMA_CALLBACK eventHandler, const uintptr_t contextHandle )
{
    classDdmaObj.classd_app_callback = eventHandler;
    classDdmaObj.context = contextHandle;

    return;
}
</#if>
/******************************************************************************
  Function:
    CLASSD_Initialize ( void )

  Summary:
    Configures the Audio Class D Amplifier.

  Remarks:
    See plib_classd.h for more details.
*/
void CLASSD_Initialize( void )
{ 
<#if tr == "dma">
    //using DMA for transfer
    uint32_t xdmac_cfg= 0;
</#if>
    /* perform soft reset */
    CLASSD_REGS->CLASSD_CR = CLASSD_CR_SWRST_Msk;
    CLASSD_REGS->CLASSD_IDR = CLASSD_IDR_DATRDY_Msk;
    CLASSD_REGS->CLASSD_INTPMR = 0;
    CLASSD_REGS->CLASSD_MR = 0;
    
    /* Set incoming data sampling frequency & DSP Clock Freq*/
    setCLASSD_Frame();
    
    /* Configure output mode */
    setCLASSD_OutputMode();
	
<#if Mono == true>
    /* Set Mono mode */
    setCLASSD_MonoMode();
</#if>    

    /* Enable channel but set attenuation to max*/
    CLASSD_REGS->CLASSD_MR |= CLASSD_MR_LEN_Msk;
    CLASSD_REGS->CLASSD_INTPMR |= CLASSD_INTPMR_ATTL(CLASSD_INTPMR_ATTL_Msk);
                 
    /* Select Equalization configuration */
    setCLASSD_EQCFG();                       
<#if tr == "dma">	        
    xdmac_cfg = XDMAC_ChannelSettingsGet(0);
    xdmac_cfg |= XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DWIDTH_HALFWORD ;
    XDMAC_ChannelSettingsSet(XDMAC_CHANNEL_0, xdmac_cfg);
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_0, classD_dma_complete, 0);
</#if>
}
