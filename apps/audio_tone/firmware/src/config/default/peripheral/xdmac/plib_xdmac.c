/*******************************************************************************
  XDMAC PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_xdmac.c

  Summary:
    XDMAC PLIB Implementation File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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

#include "device.h"
#include "plib_xdmac.h"

/* Macro for limiting XDMAC objects to highest channel enabled */
#define XDMAC_ACTIVE_CHANNELS_MAX 2

typedef struct
{
    uint8_t                inUse;
    XDMAC_CHANNEL_CALLBACK callback;
    uintptr_t              context;

} XDMAC_CH_OBJECT ;

XDMAC_CH_OBJECT xdmacChannelObj[XDMAC_ACTIVE_CHANNELS_MAX];

// *****************************************************************************
// *****************************************************************************
// Section: XDMAC Implementation
// *****************************************************************************
// *****************************************************************************

void XDMAC_InterruptHandler( void )
{
    XDMAC_CH_OBJECT *xdmacChObj = (XDMAC_CH_OBJECT *)&xdmacChannelObj[0];
    uint8_t channel = 0;
    volatile uint32_t chanIntStatus = 0;

    /* Iterate all channels */
    for (channel = 0; channel < XDMAC_ACTIVE_CHANNELS_MAX; channel++)
    {
        /* Process events only on active channels */
        if (1 == xdmacChObj->inUse)
        {
            /* Read the interrupt status for the active DMA channel */
            chanIntStatus = XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CIS;

            if (chanIntStatus & ( XDMAC_CIS_RBEIS_Msk | XDMAC_CIS_WBEIS_Msk | XDMAC_CIS_ROIS_Msk))
            {
                /* It's an error interrupt */
                if (NULL != xdmacChObj->callback)
                {
                    xdmacChObj->callback(XDMAC_TRANSFER_ERROR, xdmacChObj->context);
                }
            }
            else if (chanIntStatus & XDMAC_CIS_BIS_Msk)
            {
                /* It's a block transfer complete interrupt */
                if (NULL != xdmacChObj->callback)
                {
                    xdmacChObj->callback(XDMAC_TRANSFER_COMPLETE, xdmacChObj->context);
                }
            }
        }

        /* Point to next channel object */
        xdmacChObj += 1;
    }
}

void XDMAC_Initialize( void )
{
    XDMAC_CH_OBJECT *xdmacChObj = (XDMAC_CH_OBJECT *)&xdmacChannelObj[0];
    uint8_t channel = 0;

    /* Initialize channel objects */
    for(channel = 0; channel < XDMAC_ACTIVE_CHANNELS_MAX; channel++)
    {
        xdmacChObj->inUse = 0;
        xdmacChObj->callback = NULL;
        xdmacChObj->context = 0;

        /* Point to next channel object */
        xdmacChObj += 1;
    }

    /* Configure Channel 0 */
    XDMAC_REGS->XDMAC_CHID[0].XDMAC_CC= (XDMAC_CC_TYPE_PER_TRAN | XDMAC_CC_PERID(33) | XDMAC_CC_DSYNC_PER2MEM | XDMAC_CC_SWREQ_HWR_CONNECTED | XDMAC_CC_DAM_INCREMENTED_AM | XDMAC_CC_SAM_FIXED_AM | XDMAC_CC_SIF_AHB_IF1 | XDMAC_CC_DIF_AHB_IF0 | XDMAC_CC_DWIDTH_HALFWORD | XDMAC_CC_CSIZE_CHK_1 | XDMAC_CC_MBSIZE_SINGLE);
    XDMAC_REGS->XDMAC_CHID[0].XDMAC_CIE= (XDMAC_CIE_BIE_Msk | XDMAC_CIE_RBIE_Msk | XDMAC_CIE_WBIE_Msk | XDMAC_CIE_ROIE_Msk);
    XDMAC_REGS->XDMAC_GIE= (XDMAC_GIE_IE0_Msk << 0);
    xdmacChannelObj[0].inUse = 1;

    /* Configure Channel 1 */
    XDMAC_REGS->XDMAC_CHID[1].XDMAC_CC= (XDMAC_CC_TYPE_PER_TRAN | XDMAC_CC_PERID(32) | XDMAC_CC_DSYNC_MEM2PER | XDMAC_CC_SWREQ_HWR_CONNECTED | XDMAC_CC_DAM_FIXED_AM | XDMAC_CC_SAM_INCREMENTED_AM | XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF1 | XDMAC_CC_DWIDTH_HALFWORD | XDMAC_CC_CSIZE_CHK_1 | XDMAC_CC_MBSIZE_SINGLE);
    XDMAC_REGS->XDMAC_CHID[1].XDMAC_CIE= (XDMAC_CIE_BIE_Msk | XDMAC_CIE_RBIE_Msk | XDMAC_CIE_WBIE_Msk | XDMAC_CIE_ROIE_Msk);
    XDMAC_REGS->XDMAC_GIE= (XDMAC_GIE_IE0_Msk << 1);
    xdmacChannelObj[1].inUse = 1;

    return;
}

void XDMAC_ChannelCallbackRegister( XDMAC_CHANNEL channel, const XDMAC_CHANNEL_CALLBACK eventHandler, const uintptr_t contextHandle )
{
    xdmacChannelObj[channel].callback = eventHandler;
    xdmacChannelObj[channel].context = contextHandle;

    return;
}

void XDMAC_ChannelTransfer( XDMAC_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize )
{
    volatile uint32_t status = 0;

    /* Clear channel level status before adding transfer parameters */
    status = XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CIS;
    (void)status;

    /*Set source address */
    XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CSA= (uint32_t)srcAddr;

    /* Set destination address */
    XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CDA= (uint32_t)destAddr;

    /* Set block size */
    XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CUBC= XDMAC_CUBC_UBLEN(blockSize);

    /* Enable the channel */
    XDMAC_REGS->XDMAC_GE= (XDMAC_GE_EN0_Msk << channel);

    return;
}

bool XDMAC_ChannelIsBusy (XDMAC_CHANNEL channel)
{
    bool status = false;

    if( (XDMAC_CC_WRIP_Msk == (XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CC& XDMAC_CC_WRIP_Msk)) || (XDMAC_CC_RDIP_Msk == (XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CC& XDMAC_CC_RDIP_Msk)) )
    {
        status = true;
    }

    return status;
}

void XDMAC_ChannelDisable (XDMAC_CHANNEL channel)
{
    /* Disable the channel */
    XDMAC_REGS->XDMAC_GD = (XDMAC_GD_DI0_Msk << channel);

    return;
}

XDMAC_CHANNEL_CONFIG XDMAC_ChannelSettingsGet (XDMAC_CHANNEL channel)
{
    return (XDMAC_CHANNEL_CONFIG)XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CC;
}

bool XDMAC_ChannelSettingsSet (XDMAC_CHANNEL channel, XDMAC_CHANNEL_CONFIG setting)
{
    /* Disable the channel */
    XDMAC_REGS->XDMAC_GD= (XDMAC_GD_DI0_Msk << channel);

    /* Set the new settings */
    XDMAC_REGS->XDMAC_CHID[channel].XDMAC_CC= setting;

    return true;
}
