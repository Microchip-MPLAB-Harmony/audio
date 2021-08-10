/*******************************************************************************
  Serial Communication Interface Inter-Integrated Circuit (SERCOM I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_sercom0_i2c.c

  Summary:
    SERCOM I2C PLIB Implementation file

  Description:
    This file defines the interface to the SERCOM I2C peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "plib_sercom0_i2c.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* SERCOM0 I2C baud value for 100 Khz baud rate */
#define SERCOM0_I2CM_BAUD_VALUE			(59880U)

static SERCOM_I2C_OBJ sercom0I2CObj;

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM0 I2C Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SERCOM0_I2C_Initialize(void)

  Summary:
    Initializes the instance of the SERCOM peripheral operating in I2C mode.

  Description:
    This function initializes the given instance of the SERCOM I2C peripheral as
    configured by the user from the MHC.

  Remarks:
    Refer plib_sercom0_i2c.h for more information.
*/

void SERCOM0_I2C_Initialize(void)
{
    /* Enable smart mode enable */
    SERCOM0_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN_Msk;

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

    /* Baud rate - Master Baud Rate*/
    SERCOM0_REGS->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(SERCOM0_I2CM_BAUD_VALUE);

    /* Set Operation Mode (Master), SDA Hold time, run in stand by and i2c master enable */
    SERCOM0_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SDAHOLD_75NS | SERCOM_I2CM_CTRLA_SPEED_STANDARD_AND_FAST_MODE | SERCOM_I2CM_CTRLA_ENABLE_Msk ;

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

    /* Initial Bus State: IDLE */
    SERCOM0_REGS->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_BUSSTATE(0x01);

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

    /* Initialize the SERCOM0 PLib Object */
    sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;
    sercom0I2CObj.state = SERCOM_I2C_STATE_IDLE;

    /* Enable all Interrupts */
    SERCOM0_REGS->I2CM.SERCOM_INTENSET = SERCOM_I2CM_INTENSET_Msk;
}

// *****************************************************************************
/* Function:
    void SERCOM0_I2C_InitiateRead(uint16_t address)

  Summary:
    Intiates I2C Read

  Description:

  Remarks:
    Refer plib_sercom0_i2c.h for more information.
*/

static void SERCOM0_I2C_InitiateRead(uint16_t address)
{
    sercom0I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;

    SERCOM0_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_READ;

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);
}

// *****************************************************************************
/* Function:
    void SERCOM0_I2C_CallbackRegister(SERCOM_I2C_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given SERCOM I2C's transfer events occur.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the SERCOM_I2C_CALLBACK data type.

    context - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

   Returns:
    None.
*/

void SERCOM0_I2C_CallbackRegister(SERCOM_I2C_CALLBACK callback, uintptr_t contextHandle)
{
    sercom0I2CObj.callback = callback;

    sercom0I2CObj.context  = contextHandle;
}

/// *****************************************************************************
/* Function:
    void SERCOM0_2C_InitiateTransfer(uint16_t address, bool type)

  Summary:
    Send the 7-bit or 10-bit slave address.

  Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
     address - 7-bit / 10-bit slave address.
     type - Read / Write

  Remarks:
    None.
*/

static void SERCOM0_I2C_InitiateTransfer(uint16_t address, bool type)
{
    sercom0I2CObj.writeCount = 0;
    sercom0I2CObj.readCount = 0;

    /* Clear all flags */
    SERCOM0_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;

    /* Smart mode enabled - ACK is set to send while receiving the data */
    SERCOM0_REGS->I2CM.SERCOM_CTRLB &= ~SERCOM_I2CM_CTRLB_ACKACT_Msk;

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

    /* Reset Error Information */
    sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;

    if(type)
    {
        sercom0I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;

        /* Write 7bit address with direction (ADDR.ADDR[0]) equal to 1*/
        SERCOM0_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_READ;
    }
    else
    {
        sercom0I2CObj.state = SERCOM_I2C_STATE_TRANSFER_WRITE;

        /* Write 7bit address with direction (ADDR.ADDR[0]) equal to 0*/
        SERCOM0_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_WRITE;
    }

    /* Wait for synchronization */
    while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);
}

// *****************************************************************************
/* Function:
    bool SERCOM0_I2C_Read(uint16_t address, uint8_t *pdata,
                                                                 uint32_t length)

   Summary:
    Reads data from the slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to destination data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM0_I2C_Read(uint16_t address, uint8_t *pdata, uint32_t length)
{
    /* Check for ongoing transfer */
    if(sercom0I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom0I2CObj.address = address;
    sercom0I2CObj.readBuffer = pdata;
    sercom0I2CObj.readSize = length;
    sercom0I2CObj.writeBuffer = NULL;
    sercom0I2CObj.writeSize = 0;
    sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM0_I2C_InitiateTransfer(address, true);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM0_I2C_Write(uint16_t address, uint8_t *pdata,
                                                                 uint32_t length)

   Summary:
    Writes data onto the slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to source data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM0_I2C_Write(uint16_t address, uint8_t *pdata, uint32_t length)
{
    /* Check for ongoing transfer */
    if(sercom0I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom0I2CObj.address = address;
    sercom0I2CObj.readBuffer = NULL;
    sercom0I2CObj.readSize = 0;
    sercom0I2CObj.writeBuffer = pdata;
    sercom0I2CObj.writeSize = length;
    sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM0_I2C_InitiateTransfer(address, false);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM0_I2C_WriteRead(uint16_t address, uint8_t *wdata,
                               uint32_t wlength, uint8_t *rdata, uint32_t rlength)

   Summary:
    Write and Read data from Slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    wdata   - pointer to write data buffer
    wlength - write data length in bytes.
    rdata   - pointer to read data buffer.
    rlength - read data length in bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM0_I2C_WriteRead(uint16_t address, uint8_t *wdata, uint32_t wlength, uint8_t *rdata, uint32_t rlength)
{
    /* Check for ongoing transfer */
    if(sercom0I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom0I2CObj.address = address;
    sercom0I2CObj.readBuffer = rdata;
    sercom0I2CObj.readSize = rlength;
    sercom0I2CObj.writeBuffer = wdata;
    sercom0I2CObj.writeSize = wlength;
    sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM0_I2C_InitiateTransfer(address, false);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM0_I2C_IsBusy(void)

   Summary:
    Returns the Peripheral busy status.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    None.

   Returns:
    true - Busy.
    false - Not busy.
*/

bool SERCOM0_I2C_IsBusy(void)
{
    if(sercom0I2CObj.state == SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// *****************************************************************************
/* Function:
    SERCOM_I2C_ERROR SERCOM0_I2C_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/

SERCOM_I2C_ERROR SERCOM0_I2C_ErrorGet(void)
{
    return sercom0I2CObj.error;
}

// *****************************************************************************
/* Function:
    void SERCOM0_I2C_InterruptHandler(void)

  Summary:
    SERCOM0 I2C Peripheral Interrupt Handler.

  Description:
    This function is SERCOM0 I2C Peripheral Interrupt Handler and will
    called on every SERCOM0 I2C interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/

void SERCOM0_I2C_InterruptHandler(void)
{
    if(SERCOM0_REGS->I2CM.SERCOM_INTENSET != 0)
    {
        /* Checks if the arbitration lost in multi-master scenario */
        if((SERCOM0_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_ARBLOST_Msk) == SERCOM_I2CM_STATUS_ARBLOST_Msk)
        {
            /*
             * Re-initiate the transfer if arbitration is lost
             * in between of the transfer
             */
            sercom0I2CObj.state = SERCOM_I2C_REINITIATE_TRANSFER;

        }
        /* Check for Bus Error during transmission */
        else if((SERCOM0_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSERR_Msk) == SERCOM_I2CM_STATUS_BUSERR_Msk)
        {
            /* Set Error status */
            sercom0I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom0I2CObj.error = SERCOM_I2C_ERROR_BUS;
        }
        /* Checks slave acknowledge for address or data*/
        else if((SERCOM0_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_RXNACK_Msk) == SERCOM_I2CM_STATUS_RXNACK_Msk)
        {
            sercom0I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom0I2CObj.error = SERCOM_I2C_ERROR_NAK;
        }
        else
        {
            switch(sercom0I2CObj.state)
            {
                case SERCOM_I2C_REINITIATE_TRANSFER:
                {
                    if (sercom0I2CObj.writeSize != 0)
                    {
                        /* Initiate Write transfer */
                        SERCOM0_I2C_InitiateTransfer(sercom0I2CObj.address, false);
                    }
                    else
                    {
                        /* Initiate Read transfer */
                        SERCOM0_I2C_InitiateTransfer(sercom0I2CObj.address, true);
                    }

                    break;
                }

                case SERCOM_I2C_STATE_IDLE:
                {
                    break;
                }
                case SERCOM_I2C_STATE_TRANSFER_WRITE:
                {
                    if (sercom0I2CObj.writeCount == (sercom0I2CObj.writeSize))
                    {
                        if(sercom0I2CObj.readSize != 0)
                        {
                            SERCOM0_I2C_InitiateRead(sercom0I2CObj.address);
                        }
                        else
                        {
                            SERCOM0_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(3);

                            /* Wait for synchronization */
                            while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

                            sercom0I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                        }
                    }
                    /* Write next byte */
                    else
                    {
                        SERCOM0_REGS->I2CM.SERCOM_DATA = sercom0I2CObj.writeBuffer[sercom0I2CObj.writeCount++];
                    }

                    break;
                }
                case SERCOM_I2C_STATE_TRANSFER_READ:
                {
                    if(sercom0I2CObj.readCount == (sercom0I2CObj.readSize - 1))
                    {
                        /* Set NACK and send stop condition to the slave from master */
                        SERCOM0_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_ACKACT_Msk | SERCOM_I2CM_CTRLB_CMD(3);

                        /* Wait for synchronization */
                        while(SERCOM0_REGS->I2CM.SERCOM_SYNCBUSY);

                        sercom0I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                    }

                    /* Read the received data */
                    sercom0I2CObj.readBuffer[sercom0I2CObj.readCount++] = SERCOM0_REGS->I2CM.SERCOM_DATA;

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        /* Error Status */
        if(sercom0I2CObj.state == SERCOM_I2C_STATE_ERROR)
        {
            /* Reset the PLib objects and Interrupts */
            sercom0I2CObj.state = SERCOM_I2C_STATE_IDLE;

            SERCOM0_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;

            if (sercom0I2CObj.callback != NULL)
            {
                sercom0I2CObj.callback(sercom0I2CObj.context);
            }
        }
        /* Transfer Complete */
        else if(sercom0I2CObj.state == SERCOM_I2C_STATE_TRANSFER_DONE)
        {
            /* Reset the PLib objects and interrupts */
            sercom0I2CObj.state = SERCOM_I2C_STATE_IDLE;
            sercom0I2CObj.error = SERCOM_I2C_ERROR_NONE;

            SERCOM0_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;

            if(sercom0I2CObj.callback != NULL)
            {
                sercom0I2CObj.callback(sercom0I2CObj.context);
            }

        }
    }

    return;
}