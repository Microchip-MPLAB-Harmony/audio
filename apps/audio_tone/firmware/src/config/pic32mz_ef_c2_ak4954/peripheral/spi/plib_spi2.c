/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi2.c

  Summary:
    SPI2 Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_spi2.h"

// *****************************************************************************
// *****************************************************************************
// Section: SPI2 Implementation
// *****************************************************************************
// *****************************************************************************

/* Global object to save SPI Exchange related data */
SPI_OBJECT spi2Obj;

//#define SPI2_CON_MSTEN                      (1 << _SPI2CON_MSTEN_POSITION)
//#define SPI2_CON_CKP                        (0 << _SPI2CON_CKP_POSITION)
//#define SPI2_CON_CKE                        (1 << _SPI2CON_CKE_POSITION)
//#define SPI2_CON_MODE_32_MODE_16            (0 << _SPI2CON_MODE16_POSITION)
//#define SPI2_CON_ENHBUF                     (1 << _SPI2CON_ENHBUF_POSITION)
//#define SPI2_CON_MCLKSEL                    (0 << _SPI2CON_MCLKSEL_POSITION)
//#define SPI2_CON_MSSEN                      (1 << _SPI2CON_MSSEN_POSITION)

#define SPI2_CON_MSTEN                      (1 << _SPI2CON_MSTEN_POSITION)
#define SPI2_CON_CKP                        (1 << _SPI2CON_CKP_POSITION)
#define SPI2_CON_CKE                        (0 << _SPI2CON_CKE_POSITION)
#define SPI2_CON_MODE_32_MODE_16            (1 << _SPI2CON_MODE16_POSITION)
#define SPI2_CON_ENHBUF                     (0 << _SPI2CON_ENHBUF_POSITION)
#define SPI2_CON_MCLKSEL                    (1 << _SPI2CON_MCLKSEL_POSITION)
#define SPI2_CON_MSSEN                      (1 << _SPI2CON_MSSEN_POSITION)

#define SPI2_CON_FRMEN                      (1 << _SPI2CON_FRMEN_POSITION)
#define SPI2_CON_FRMSYNC                    (0 << _SPI2CON_FRMSYNC_POSITION)
#define SPI2_CON_FRMPOL                     (1 << _SPI2CON_FRMPOL_POSITION)
#define SPI2_CON_FRMSYPW                    (0 << _SPI2CON_FRMSYPW_POSITION)
#define SPI2_CON_FRMCNT                     (4 << _SPI2CON_FRMCNT_POSITION)
#define SPI2_CON_SPIFE                      (0 << _SPI2CON_SPIFE_POSITION)

#define SPI2_CON2_IGNROV                    (1 << _SPI2CON2_IGNROV_POSITION)
#define SPI2_CON2_IGNTUR                    (1 << _SPI2CON2_IGNTUR_POSITION)
#define SPI2_CON2_AUDEN                     (1 << _SPI2CON2_AUDEN_POSITION)
#define SPI2_CON2_AUDMONO                   (0 << _SPI2CON2_AUDMONO_POSITION)
#define SPI2_CON2_AUDMOD                    (2 << _SPI2CON2_AUDMOD_POSITION)

void SPI2_Initialize ( void )
{
    uint32_t rdata;

    /*Disable SPI2_FAULT Interrupt, */
    /*Disable SPI2_RX Interrupt, */
    /*Disable SPI2_TX Interrupt */
    IEC4CLR = 0x4000;
    IEC4CLR = 0x8000;
    IEC4CLR = 0x10000;

    /* STOP and Reset the SPI*/
    SPI2CON = 0;
    SPI2CON2 = 0;

    /*Clear the Receiver buffer */
    rdata = SPI2BUF;
    rdata = rdata;

    /* Clear SPI2_FAULT Interrupt flag */
    /* Clear SPI2_RX Interrupt flag */
    /* Clear SPI2_TX Interrupt flag */
    IFS4CLR = 0x4000;
    IFS4CLR = 0x8000;
    IFS4CLR = 0x10000;

    /* BAUD Rate register Setup */
    SPI2BRG = 1;

    /* CLear the Overflow */
    SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

    /*
    MSTEN = 1
    CKP = 0
    CKE = 1
    MODE<32,16> = 0
    ENHBUF = 1
    MSSEN = 1
    MCLKSEL = 0
    */
    SPI2CONSET = SPI2_CON_MSSEN | SPI2_CON_MCLKSEL | SPI2_CON_ENHBUF | SPI2_CON_MODE_32_MODE_16 | SPI2_CON_CKE | SPI2_CON_CKP | SPI2_CON_MSTEN |
        SPI2_CON_FRMEN | SPI2_CON_FRMPOL | SPI2_CON_FRMSYNC | SPI2_CON_FRMSYPW | SPI2_CON_FRMCNT | SPI2_CON_SPIFE;

    SPI2CON2SET = SPI2_CON2_IGNROV | SPI2_CON2_IGNTUR | SPI2_CON2_AUDEN | SPI2_CON2_AUDMONO | SPI2_CON2_AUDMOD;
    
    /* Enable transmit interrupt when transmit buffer is completely empty (STXISEL = '01') */
    /* Enable receive interrupt when the receive buffer is not empty (SRXISEL = '01') */
    //!!SPI2CONSET = 0x00000005;

    /* Initialize global variables */
    spi2Obj.transferIsBusy = false;
    spi2Obj.callback = NULL;

    /* Enable SPI2 */
    SPI2CONSET= _SPI2CON_ON_MASK;

    return;
}

bool SPI2_TransferSetup (SPI_TRANSFER_SETUP* setup, uint32_t spiSourceClock )
{
    uint32_t t_brg;
    uint32_t baudHigh;
    uint32_t baudLow;
    uint32_t errorHigh;
    uint32_t errorLow;

    if ((setup == NULL) || (setup->clockFrequency == 0))
    {
        return false;
    }

    if(spiSourceClock == 0)
    {
        // Use Master Clock Frequency set in GUI
        spiSourceClock = 99000000;
    }

    t_brg = (((spiSourceClock / (setup->clockFrequency)) / 2u) - 1u);
    baudHigh = spiSourceClock / (2u * (t_brg + 1u));
    baudLow = spiSourceClock / (2u * (t_brg + 2u));
    errorHigh = baudHigh - setup->clockFrequency;
    errorLow = setup->clockFrequency - baudLow;

    if (errorHigh > errorLow)
    {
        t_brg++;
    }

    if(t_brg > 8191)
    {
        return false;
    }

    SPI2BRG = t_brg;
    SPI2CON = (SPI2CON & (~(_SPI2CON_MODE16_MASK | _SPI2CON_MODE32_MASK | _SPI2CON_CKP_MASK | _SPI2CON_CKE_MASK))) |
                            (setup->clockPolarity | setup->clockPhase | setup->dataBits);

    return true;
}

bool SPI2_Write(void* pTransmitData, size_t txSize)
{
    return(SPI2_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool SPI2_Read(void* pReceiveData, size_t rxSize)
{
    return(SPI2_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool SPI2_WriteRead (void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    bool isRequestAccepted = false;
    uint32_t dummyData;

    /* Verify the request */
    if((((txSize > 0) && (pTransmitData != NULL)) || ((rxSize > 0) && (pReceiveData != NULL))) && (spi2Obj.transferIsBusy == false))
    {
        isRequestAccepted = true;
        spi2Obj.txBuffer = pTransmitData;
        spi2Obj.rxBuffer = pReceiveData;
        spi2Obj.rxCount = 0;
        spi2Obj.txCount = 0;
        spi2Obj.dummySize = 0;

        if (pTransmitData != NULL)
        {
            spi2Obj.txSize = txSize;
        }
        else
        {
            spi2Obj.txSize = 0;
        }

        if (pReceiveData != NULL)
        {
            spi2Obj.rxSize = rxSize;
        }
        else
        {
            spi2Obj.rxSize = 0;
        }

        spi2Obj.transferIsBusy = true;

        if (spi2Obj.rxSize > spi2Obj.txSize)
        {
            spi2Obj.dummySize = spi2Obj.rxSize - spi2Obj.txSize;
        }

        /* Clear the receive overflow error if any */
        SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

        /* Make sure there is no data pending in the RX FIFO */
        /* Depending on 8/16/32 bit mode, there may be 16/8/4 bytes in the FIFO */
        while ((bool)(SPI2STAT & _SPI2STAT_SPIRBE_MASK) == false)
        {
            dummyData = SPI2BUF;
            (void)dummyData;
        }

        /* Configure SPI to generate receive interrupt when receive buffer is empty (SRXISEL = '01') */
        SPI2CONSET = 0x00000001;

        /* Configure SPI to generate transmit interrupt when the transmit shift register is empty (STXISEL = '00')*/
        SPI2CONCLR = _SPI2CON_STXISEL_MASK;

        /* Clear the receive interrupt flag */
        IFS4CLR = 0x8000;

        /* Clear the transmit interrupt flag */
        IFS4CLR = 0x10000;

        /* Disable the receive interrupt */
        IEC4CLR = 0x8000;

        /* Disable the transmit interrupt */
        IEC4CLR = 0x10000;

        /* Start the first write here itself, rest will happen in ISR context */
        if((_SPI2CON_MODE32_MASK) == (SPI2CON & (_SPI2CON_MODE32_MASK)))
        {
            spi2Obj.txSize >>= 2;
            spi2Obj.dummySize >>= 2;
            spi2Obj.rxSize >>= 2;

            if(spi2Obj.txCount < spi2Obj.txSize)
            {
                SPI2BUF= *((uint32_t*)spi2Obj.txBuffer);
                spi2Obj.txCount++;
            }
            else if (spi2Obj.dummySize > 0)
            {
                SPI2BUF= (uint32_t)(0xff);
                spi2Obj.dummySize--;
            }
        }
        else if((_SPI2CON_MODE16_MASK) == (SPI2CON & (_SPI2CON_MODE16_MASK)))
        {
            spi2Obj.txSize >>= 1;
            spi2Obj.dummySize >>= 1;
            spi2Obj.rxSize >>= 1;

            if (spi2Obj.txCount < spi2Obj.txSize)
            {
                SPI2BUF= *((uint16_t*)spi2Obj.txBuffer);
                spi2Obj.txCount++;
            }
            else if (spi2Obj.dummySize > 0)
            {
                SPI2BUF= (uint16_t)(0xff);
                spi2Obj.dummySize--;
            }
        }
        else
        {
            if (spi2Obj.txCount < spi2Obj.txSize)
            {
                SPI2BUF= *((uint8_t*)spi2Obj.txBuffer);
                spi2Obj.txCount++;
            }
            else if (spi2Obj.dummySize > 0)
            {
                SPI2BUF= (uint8_t)(0xff);
                spi2Obj.dummySize--;
            }
        }

        if (rxSize > 0)
        {
            /* Enable receive interrupt to complete the transfer in ISR context.
             * Keep the transmit interrupt disabled. Transmit interrupt will be
             * enabled later if txCount < txSize, when rxCount = rxSize.
             */
            IEC4SET = 0x8000;
        }
        else
        {
            if (spi2Obj.txCount != spi2Obj.txSize)
            {
                /* Configure SPI to generate transmit buffer empty interrupt only if more than
                 * data is pending (STXISEL = '01')  */
                SPI2CONSET = 0x00000004;
            }
            /* Enable transmit interrupt to complete the transfer in ISR context */
            IEC4SET = 0x10000;
        }
    }

    return isRequestAccepted;
}

bool SPI2_IsBusy (void)
{
    return spi2Obj.transferIsBusy;
}

void SPI2_CallbackRegister (SPI_CALLBACK callback, uintptr_t context)
{
    spi2Obj.callback = callback;

    spi2Obj.context = context;
}

void SPI2_RX_InterruptHandler (void)
{
    uint32_t receivedData = 0;

    /* Check if the receive buffer is empty or not */
    if ((bool)(SPI2STAT & _SPI2STAT_SPIRBE_MASK) == false)
    {
        /* Receive buffer is not empty. Read the received data. */
        receivedData = SPI2BUF;

        if (spi2Obj.rxCount < spi2Obj.rxSize)
        {
            /* Copy the received data to the user buffer */
            if((_SPI2CON_MODE32_MASK) == (SPI2CON & (_SPI2CON_MODE32_MASK)))
            {
                ((uint32_t*)spi2Obj.rxBuffer)[spi2Obj.rxCount++] = receivedData;
            }
            else if((_SPI2CON_MODE16_MASK) == (SPI2CON & (_SPI2CON_MODE16_MASK)))
            {
                ((uint16_t*)spi2Obj.rxBuffer)[spi2Obj.rxCount++] = receivedData;
            }
            else
            {
                ((uint8_t*)spi2Obj.rxBuffer)[spi2Obj.rxCount++] = receivedData;
            }
            if ((spi2Obj.rxCount == spi2Obj.rxSize) && (spi2Obj.txCount < spi2Obj.txSize))
            {
                /* Reception of all bytes is complete. However, there are few more
                 * bytes to be transmitted as txCount != txSize. Finish the
                 * transmission of the remaining bytes from the transmit interrupt. */

                /* Disable the receive interrupt */
                IEC4CLR = 0x8000;

                /* Enable the transmit interrupt. Callback will be given from the
                 * transmit interrupt, when all bytes are shifted out. */
                IEC4SET = 0x10000;
            }
        }
        if (spi2Obj.rxCount < spi2Obj.rxSize)
        {
            /* More bytes pending to be received .. */
            if((_SPI2CON_MODE32_MASK) == (SPI2CON & (_SPI2CON_MODE32_MASK)))
            {
                if (spi2Obj.txCount < spi2Obj.txSize)
                {
                    SPI2BUF = ((uint32_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
                }
                else if (spi2Obj.dummySize > 0)
                {
                    SPI2BUF = (uint32_t)(0xff);
                    spi2Obj.dummySize--;
                }
            }
            else if((_SPI2CON_MODE16_MASK) == (SPI2CON & (_SPI2CON_MODE16_MASK)))
            {
                if (spi2Obj.txCount < spi2Obj.txSize)
                {
                    SPI2BUF = ((uint16_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
                }
                else if (spi2Obj.dummySize > 0)
                {
                    SPI2BUF = (uint16_t)(0xff);
                    spi2Obj.dummySize--;
                }
            }
            else
            {
                if (spi2Obj.txCount < spi2Obj.txSize)
                {
                    SPI2BUF = ((uint8_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
                }
                else if (spi2Obj.dummySize > 0)
                {
                    SPI2BUF = (uint8_t)(0xff);
                    spi2Obj.dummySize--;
                }
            }
        }
        else
        {
            if((spi2Obj.rxCount == spi2Obj.rxSize) && (spi2Obj.txCount == spi2Obj.txSize))
            {
                /* Clear receiver overflow error if any */
                SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

                /* Disable receive interrupt */
                IEC4CLR = 0x8000;

                /* Transfer complete. Give a callback */
                spi2Obj.transferIsBusy = false;

                if(spi2Obj.callback != NULL)
                {
                    spi2Obj.callback(spi2Obj.context);
                }
            }
        }
    }

    /* Clear SPI2 RX Interrupt flag */
    /* This flag should cleared only after reading buffer */
    IFS4CLR = 0x8000;
}

void SPI2_TX_InterruptHandler (void)
{
    /* If there are more words to be transmitted, then transmit them here and keep track of the count */
    if((SPI2STAT & _SPI2STAT_SPITBE_MASK) == _SPI2STAT_SPITBE_MASK)
    {
        if (spi2Obj.txCount < spi2Obj.txSize)
        {
            if((_SPI2CON_MODE32_MASK) == (SPI2CON & (_SPI2CON_MODE32_MASK)))
            {
                SPI2BUF = ((uint32_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
            }
            else if((_SPI2CON_MODE16_MASK) == (SPI2CON & (_SPI2CON_MODE16_MASK)))
            {
                SPI2BUF = ((uint16_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
            }
            else
            {
                SPI2BUF = ((uint8_t*)spi2Obj.txBuffer)[spi2Obj.txCount++];
            }

            if (spi2Obj.txCount == spi2Obj.txSize)
            {
                /* All bytes are submitted to the SPI module. Now, enable transmit
                 * interrupt when the shift register is empty (STXISEL = '00')*/
                SPI2CONCLR = _SPI2CON_STXISEL_MASK;
            }
        }
        else if ((spi2Obj.txCount == spi2Obj.txSize) && (SPI2STAT & _SPI2STAT_SRMT_MASK))
        {
            /* This part of code is executed when the shift register is empty. */

            /* Clear receiver overflow error if any */
            SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

            /* Disable transmit interrupt */
            IEC4CLR = 0x10000;

            /* Transfer complete. Give a callback */
            spi2Obj.transferIsBusy = false;

            if(spi2Obj.callback != NULL)
            {
                spi2Obj.callback(spi2Obj.context);
            }
        }
    }
    /* Clear the transmit interrupt flag */
    IFS4CLR = 0x10000;
}
