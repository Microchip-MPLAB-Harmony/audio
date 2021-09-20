// <editor-fold defaultstate="collapsed" desc="DRV_I2S Instance ${INDEX?string} Initialization Data">

/* I2S PLIB Interface Initialization */
DRV_I2S_PLIB_INTERFACE drvI2S${INDEX?string}PlibAPI =
{
    .I2S_LRCLK_Get = (DRV_I2S_LRCLK_GET)${DRV_I2S_PLIB}_LRCLK_Get,  
<#if __PROCESSOR?matches("ATSAME70.*") == true && DRV_I2S_PLIB?matches("I2SC.*") >

/************************ Start of code specific to SAME70 ********************/
    .I2S_PLLA_CLOCK_SET = (DRV_I2S_PLLA_CLOCK_SET) ${DRV_I2S_PLIB}_PLLAClockSet,
    .I2S_GCLK_SET       = (DRV_I2S_GCLK_SET) ${DRV_I2S_PLIB}_GenericClockSet,
    .I2S_PCLK_SET       = (DRV_I2S_PCLK_SET) ${DRV_I2S_PLIB}_ProgrammableClockSet,
/************************ End of SAM E70 specific code ***********************/
</#if>
<#if __PROCESSOR?matches("PIC32M.*") == true>
    .I2S_RefClockSet = (DRV_I2S_REFCLOCKSET) ${DRV_I2S_PLIB}_RefClockSet,
    .I2S_BaudRateSet = (DRV_I2S_BAUDRATESET) ${DRV_I2S_PLIB}_BaudRateSet,
</#if>
};

/* I2S Driver Initialization Data */
DRV_I2S_INIT drvI2S${INDEX?string}InitData =
{
    /* I2S PLIB API */
    .i2sPlib = &drvI2S${INDEX?string}PlibAPI,

    /* I2S IRQ */
    .interruptI2S = DRV_I2S_INT_SRC_IDX${INDEX?string},

    /* I2S Number of clients */
    .numClients = DRV_I2S_CLIENTS_NUMBER_IDX${INDEX?string},

    /* I2S Queue Size */
    .queueSize = DRV_I2S_QUEUE_SIZE_IDX0,  

<#if DRV_I2S_TX_RX_DMA == true>
    .dmaChannelTransmit = DRV_I2S_XMIT_DMA_CH_IDX${INDEX?string},
    .dmaChannelReceive  = DRV_I2S_RCV_DMA_CH_IDX${INDEX?string},
    .i2sTransmitAddress = (void *)${.vars["a_${DRV_I2S_PLIB?lower_case}"].TRANSMIT_DATA_REGISTER},
    .i2sReceiveAddress = (void *)${.vars["a_${DRV_I2S_PLIB?lower_case}"].RECEIVE_DATA_REGISTER},
<#else>
    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,
    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,
</#if>

<#if __PROCESSOR?matches("PIC32M.*") == true>
    /************ code specific to PIC32M. ********************/
    <#if DRV_I2S_TX_DMA_CHANNEL?has_content>
    .interruptTxDMA = _DMA${DRV_I2S_TX_DMA_CHANNEL}_VECTOR,
    </#if>
    <#if DRV_I2S_RX_DMA_CHANNEL?has_content>
    .interruptRxDMA = _DMA${DRV_I2S_RX_DMA_CHANNEL}_VECTOR,
    </#if>
    /************ end of PIC32M. specific code ********************/
<#elseif __PROCESSOR?matches("ATSAME54.*") == true >
    /************ code specific to SAM E54 ********************/
    <#if DRV_I2S_TX_DMA_CHANNEL?has_content>
    .interruptTxDMA = DMAC_${DRV_I2S_TX_DMA_CHANNEL}_IRQn,
    </#if>
    <#if DRV_I2S_RX_DMA_CHANNEL?has_content>
    .interruptTxDMA = DMAC_${DRV_I2S_RX_DMA_CHANNEL}_IRQn,
    </#if>
    /************ end of E54 specific code ********************/
<#else>
    <#if core.DMA_INSTANCE_NAME?has_content>
    /************ code specific to SAM E70 ********************/
    .interruptDMA = ${core.DMA_INSTANCE_NAME}_IRQn,
    /************ code specific to SAM E70 ********************/
    </#if>
</#if>
    .dmaDataLength = DRV_I2S_DATA_LENGTH_IDX${INDEX?string},
};

// </editor-fold>
