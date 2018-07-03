/*******************************************************************************
  TWIHS Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    twihs.c

  Summary
    TWIHS peripheral library interface.

  Description

  Remarks:
    
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_twihs0.h"

// *****************************************************************************
// *****************************************************************************
// Local Data Type Definitions
// *****************************************************************************
// *****************************************************************************

#define TWIHS_MASTER_MAX_BAUDRATE        (400000U)
#define TWIHS_LOW_LEVEL_TIME_LIMIT       (384000U)
#define TWIHS_CLK_DIVIDER                     (2U)
#define TWIHS_CLK_CALC_ARGU                   (3U)
#define TWIHS_CLK_DIV_MAX                  (0xFFU)
#define TWIHS_CLK_DIV_MIN                     (7U)

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

static TWIHS_OBJ twihs0Obj;
static TWIHS_TRANSACTION_REQUEST_BLOCK twihs0TRBsList[2];
static twihs_registers_t *TWIHS0_Module = TWIHS0_REGS;

// *****************************************************************************
// *****************************************************************************
// TWIHS0 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void TWIHS0_Initialize(void)

   Summary:
    Initializes given instance of the TWIHS peripheral.

   Precondition:
    None.
	
   Parameters:
    None.
	
   Returns:
    None
*/

void TWIHS0_Initialize(void)
{    
    // Reset the i2c Module
    TWIHS0_Module->TWIHS_CR = TWIHS_CR_SWRST_Msk;
        
    // Disable the I2C Master/Slave Mode
    TWIHS0_Module->TWIHS_CR = TWIHS_CR_MSDIS_Msk | 
	                                      TWIHS_CR_SVDIS_Msk;
	
    // Set Baud rate 
	TWIHS0_Module->TWIHS_CWGR = ( TWIHS_CWGR_HOLD_Msk & TWIHS0_Module->TWIHS_CWGR) |
											( TWIHS_CWGR_CLDIV(192) | 
											  TWIHS_CWGR_CHDIV(177) | 
											  TWIHS_CWGR_CKDIV(0) );
	
	// Starts the transfer by clearing the transmit hold register 
    TWIHS0_Module->TWIHS_CR = TWIHS_CR_THRCLR_Msk;
	
	// Enables interrupt on nack and arbitration lost 
    TWIHS0_Module->TWIHS_IER = TWIHS_IER_NACK_Msk | 
	                                       TWIHS_IER_ARBLST_Msk;
	
	// Enable Master Mode 
    TWIHS0_Module->TWIHS_CR = TWIHS_CR_MSEN_Msk;
	
	// Initialize the twihs PLib Object
	twihs0Obj.error   = TWIHS_ERROR_NONE;
	twihs0Obj.state   = TWIHS_STATE_IDLE;
	twihs0Obj.numTRBs = 0;		
}

// *****************************************************************************
/* Function:
    void TWIHS0_CallbackRegister(TWIHS_CALLBACK callback, uintptr_t contextHandle)
	
   Summary:
    Sets the pointer to the function (and it's context) to be called when the 
    given TWIHS's transfer events occur.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined 
	by the TWIHS_CALLBACK data type.

    context - A value (usually a pointer) passed (unused) into the function 
	identified by the callback parameter.
  
   Returns:
    None.
*/

void TWIHS0_CallbackRegister(TWIHS_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
	{
	    return;
	}
	
	twihs0Obj.callback = callback;
	twihs0Obj.context = contextHandle;
}

// *****************************************************************************
/* Function:
    void TWIHS0_TransferSetup(TWIHS_TRANSFER_SETUP setup, uint32_t srcClkFreq)

   Summary:
    Dynamic setup of TWIHS Peripheral.

   Precondition:
    None.
	
   Parameters:
    None.
	
   Returns:
    None
*/

bool TWIHS0_TransferSetup( TWIHS_TRANSFER_SETUP * setup, uint32_t srcClkFreq )
{
    uint32_t clockSrcFreq;
    uint32_t twihsClkSpeed;
    uint32_t ckdiv = 0;
    uint32_t cldiv = 0; 
    uint32_t chdiv = 0;
	uint32_t c_lh_div = 0;
    
    // Check for ongoing transfer
	if( twihs0Obj.state != TWIHS_STATE_IDLE )
	{
	    return false;
	}
    
    if( srcClkFreq )
    {
        clockSrcFreq = srcClkFreq;
    }
    else
    {
        clockSrcFreq = 150000000;
    }
    
    twihsClkSpeed = setup->clkSpeed;
    
    /* Set Clock */
    if( TWIHS_MASTER_MAX_BAUDRATE < twihsClkSpeed  )
    {
        return (false);
    }
    
	/* Low level time not less than 1.3us of I2C Fast Mode. */
	if ( twihsClkSpeed > TWIHS_LOW_LEVEL_TIME_LIMIT ) 
    {
		/* Low level of time fixed for 1.3us. */
		cldiv = clockSrcFreq / ( TWIHS_LOW_LEVEL_TIME_LIMIT * 
                                    TWIHS_CLK_DIVIDER ) - 
                                    TWIHS_CLK_CALC_ARGU;
        
		chdiv = clockSrcFreq / (( twihsClkSpeed + 
                            ( twihsClkSpeed - TWIHS_LOW_LEVEL_TIME_LIMIT)) * 
                              TWIHS_CLK_DIVIDER ) - 
                              TWIHS_CLK_CALC_ARGU;
		
		/* cldiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while (( cldiv > TWIHS_CLK_DIV_MAX ) && 
               ( ckdiv < TWIHS_CLK_DIV_MIN )) 
        {
			/* Increase clock divider */
			ckdiv++;
            
			/* Divide cldiv value */
			cldiv /= TWIHS_CLK_DIVIDER;
		}
        
		/* chdiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while (( chdiv > TWIHS_CLK_DIV_MAX ) && 
               ( ckdiv < TWIHS_CLK_DIV_MIN )) 
        {
			/* Increase clock divider */
			ckdiv++;
            
			/* Divide cldiv value */
			chdiv /= TWIHS_CLK_DIVIDER;
		}

		/* set clock waveform generator register */
        TWIHS0_Module->TWIHS_CWGR = ( TWIHS_CWGR_HOLD_Msk & TWIHS0_Module->TWIHS_CWGR) |
                                  ( TWIHS_CWGR_CLDIV(cldiv) | 
                                    TWIHS_CWGR_CHDIV(chdiv) |
                                    TWIHS_CWGR_CKDIV(ckdiv) );
	} 
    else 
    {
		c_lh_div = clockSrcFreq / ( twihsClkSpeed * TWIHS_CLK_DIVIDER ) - 
                   TWIHS_CLK_CALC_ARGU;

		/* cldiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while (( c_lh_div > TWIHS_CLK_DIV_MAX ) && 
               ( ckdiv < TWIHS_CLK_DIV_MIN )) 
        {
			/* Increase clock divider */
			ckdiv++;
            
			/* Divide cldiv value */
			c_lh_div /= TWIHS_CLK_DIVIDER;
		}

		/* set clock waveform generator register */
        TWIHS0_Module->TWIHS_CWGR = ( TWIHS_CWGR_HOLD_Msk & TWIHS0_Module->TWIHS_CWGR) |
                                  ( TWIHS_CWGR_CLDIV(c_lh_div) | 
                                    TWIHS_CWGR_CHDIV(c_lh_div) |
                                    TWIHS_CWGR_CKDIV(ckdiv) )  ;
	}
    
    return (true);

}

// *****************************************************************************
/* Function:
    bool TWIHS0_IsBusy(void)
	
   Summary:
    Returns the Peripheral busy status.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.

   Parameters:
    None.
	
   Returns:
    true - Busy.
    false - Not busy.
*/

bool TWIHS0_IsBusy(void)
{
    if( twihs0Obj.state == TWIHS_STATE_IDLE )
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
    bool TWIHS0_TRBBuildRead(uint16_t address, uint8_t *pdata, uint8_t length)
	
   Summary:
    Allocates and Builds the Read Transaction Request Block.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy.

   Parameters:
    address - 7-bit / 10-bit slave address.
	pdata   - pointer to destination data buffer
	length  - length of data buffer in number of bytes.
  
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_TRBBuildRead(uint16_t address, uint8_t *pdata, uint8_t length)
{
	TWIHS_TRANSACTION_REQUEST_BLOCK * trb = NULL;
	
	// current TRB index cannot be more than available TRB's
    if( twihs0Obj.numTRBs >= 2)
	{
	    return false;
	}
	
	// Check for ongoing transfer
	if( twihs0Obj.state != TWIHS_STATE_IDLE )
	{
	    return false;
	}
	
	trb = &twihs0TRBsList[twihs0Obj.numTRBs];
	
	// Fill the TRB
	trb->address = address;
	trb->read    = true;
	trb->length  = length;
	trb->pbuffer = pdata;
	
	// Increment current TRB index
	twihs0Obj.numTRBs++;
	
	return true;
}

// *****************************************************************************
/* Function:
    bool TWIHS0_TRBBuildWrite(uint16_t address, uint8_t *pdata, uint8_t length)
	
   Summary:
    Allocates and Builds the Read Transaction Request Block.
	
   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy.

   Parameters:
    address - 7-bit / 10-bit slave address.
	pdata   - pointer to source data buffer
	length  - length of data buffer in number of bytes.
  
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_TRBBuildWrite(uint16_t address, uint8_t *pdata, uint8_t length)
{
    TWIHS_TRANSACTION_REQUEST_BLOCK * trb = NULL;
	
	// current TRB index cannot be more than available TRB's
    if( twihs0Obj.numTRBs >= 2)
	{
	    return false;
	}
	
	// Check for ongoing transfer
	if( twihs0Obj.state != TWIHS_STATE_IDLE )
	{
	    return false;
	}
	
	trb = &twihs0TRBsList[twihs0Obj.numTRBs];
	
	// Fill the TRB
	trb->address = address;
	trb->read    = false;
	trb->length  = length;
	trb->pbuffer = pdata;
	
	// Increment current TRB index
	twihs0Obj.numTRBs++;
	
	return true;
}

// *****************************************************************************
/* Function:
    bool TWIHS0_TRBTransfer(void)
	
   Summary:
    Submits all TRB's build for processing. 
	
   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy before calling TWIHSx_TRBTransfer.

   Parameters:
    None.
  
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_TRBTransfer(void)
{
    // Minimum one filled TRB must be available
    if(  twihs0Obj.numTRBs == 0 )
	{
	    return false;
	}
	
	// Check for ongoing transfer
    if( twihs0Obj.state != TWIHS_STATE_IDLE )
	{
	    return false;
	}
	
	// Initiate Transfer
	twihs0Obj.state = TWIHS_STATE_ADDR_SEND;
    twihs0Obj.error = TWIHS_ERROR_NONE;
	
	TWIHS0_Module->TWIHS_IER = TWIHS_IER_TXCOMP_Msk | 
	                                       TWIHS_IER_TXRDY_Msk  |
                                           TWIHS_IER_RXRDY_Msk;
						   
	return true;
}

// *****************************************************************************
/* Function:
    bool TWIHS0_Read(uint16_t address, uint8_t *pdata, uint8_t length)
	
   Summary:
    Reads data from the slave.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy before calling TWIHSx_TRBTransfer.
	Minimum one TRB should be available.

   Parameters:
    address - 7-bit / 10-bit slave address.
	pdata   - pointer to destination data buffer
	length  - length of data buffer in number of bytes.
  
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_Read(uint16_t address, uint8_t *pdata, uint8_t length)
{
    // Build Read TRB
    if ( !TWIHS0_TRBBuildRead( address, pdata, length ) )
	{
	    return false;
	}
	
	// Initiate transfer
	if ( !TWIHS0_TRBTransfer( ) )
	{
	    return false;
	}
	
	return true;
}

// *****************************************************************************
/* Function:
    bool TWIHS0_Write(uint16_t address, uint8_t *pdata, uint8_t length)
	
   Summary:
    Writes data onto the slave.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy before calling TWIHSx_TRBTransfer.
	Minimum one TRB should be available.

   Parameters:
    address - 7-bit / 10-bit slave address.
	pdata   - pointer to source data buffer
	length  - length of data buffer in number of bytes.
	
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_Write(uint16_t address, uint8_t *pdata, uint8_t length)
{
    // Build Write TRB
    if ( !TWIHS0_TRBBuildWrite( address, pdata, length ) )
	{
	    return false;
	}
	
	// Initiate transfer
	if ( !TWIHS0_TRBTransfer( ) )
	{
	    return false;
	}
	
	return true;
}

// *****************************************************************************
/* Function:
    bool TWIHS0_WriteRead(uint16_t address, uint8_t *wdata, uint8_t wlength, uint8_t *rdata, uint8_t rlength)
	
   Summary:
    Write and Read data from Slave.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.
	The transfer status should not be busy before calling TWIHSx_TRBTransfer.
	Minimum two TRB's should be available.

   Parameters:
    address - 7-bit / 10-bit slave address.
	wdata   - pointer to write data buffer
	wlength - write data length in bytes.
	rdata   - pointer to read data buffer.
	rlength - read data length in bytes.
  
   Returns:
    true  - TRB submitted Successfully.
	false - Failure while submitting TRB.
*/

bool TWIHS0_WriteRead(uint16_t address, uint8_t *wdata, uint8_t wlength, uint8_t *rdata, uint8_t rlength)
{
    // Build Write TRB
    if ( !TWIHS0_TRBBuildWrite( address, wdata, wlength ) )
	{
	    return false;
	}
	
	// Build Read TRB
	if ( !TWIHS0_TRBBuildRead( address, rdata, rlength ) )
	{
	    return false;
	}
	
	// Initiate transfer
	if ( !TWIHS0_TRBTransfer( ) )
	{
	    return false;
	}
	
	return true;
}

// *****************************************************************************
/* Function:
    TWIHS_ERROR TWIHS0_ErrorGet(void)
	
   Summary:
    Returns the error during transfer.

   Precondition:
    TWIHSx_Initialize must have been called for the associated TWIHS instance.

   Parameters:
    None.
	
   Returns:
    Error during transfer.
*/

TWIHS_ERROR TWIHS0_ErrorGet(void)
{
    TWIHS_ERROR error;

    error = twihs0Obj.error;
    twihs0Obj.error = TWIHS_ERROR_NONE;

    return error;
}

// *****************************************************************************
/* Function:
    void TWIHS0_InterruptHandler(void)

   Summary:
    TWIHS0 Peripheral Interrupt Handler.

   Description:
    This function is TWIHS0 Peripheral Interrupt Handler and will
    called on every TWIHS0 interrupt.

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

void TWIHS0_InterruptHandler(void)
{
    uint32_t status;
	static uint32_t currentTRB = 0;
	TWIHS_TRANSACTION_REQUEST_BLOCK * trb;
	
	// check if transfer is initiated
    if ( twihs0Obj.state == TWIHS_STATE_IDLE )
	{
	    return;
	}
	
	// Minimum one filled TRB must be available
	if ( twihs0Obj.numTRBs == 0 )
	{
	    return;
	}
	
	// Read the peripheral status
	status = TWIHS0_Module->TWIHS_SR;
	
	/* checks if Slave has Nacked */
    if( status & TWIHS_SR_NACK_Msk ) 
    {
        twihs0Obj.state = TWIHS_STATE_ERROR;
        twihs0Obj.error = TWIHS_ERROR_NACK;
    }

    /* checks if the arbitration is lost in multi-master scenario */
    if( status & TWIHS_SR_ARBLST_Msk )
    {
        /* Re-initiate the transfer if arbitration is lost in 
         * between of the transfer 
         */
        twihs0Obj.state = TWIHS_STATE_ADDR_SEND;
    }
	
	trb = &twihs0TRBsList[currentTRB];
	
    switch( twihs0Obj.state )
	{	
		case TWIHS_STATE_TRANSFER_READ:
		{
		    /* checks if master has received the data */
            if( status & TWIHS_SR_RXRDY_Msk )
            {
			    trb->length --;
				
                if( 1 == trb->length )
                {
				    // check for last TRB
                    if( currentTRB == (twihs0Obj.numTRBs - 1) )
                    {
                        // Send Stop before last byte is received
                        TWIHS0_Module->TWIHS_CR = TWIHS_CR_STOP_Msk;
                    }
					// not the last TRB
                    else
                    {
                        // Send a Repeated Start
                        TWIHS0_Module->TWIHS_CR = TWIHS_CR_START_Msk;
                    }
                }

                /* read the received data */
                *(trb->pbuffer++) = (uint8_t)(TWIHS0_Module->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);
                
                /* checks if transmission has reached at the end */
                if( 0 == trb->length )
                {
				    // check for last TRB
                    if( currentTRB == (twihs0Obj.numTRBs - 1) )
                    {
                        /* read the module status again to know if 
                           tranmission complete flag is set */
                        status = TWIHS0_Module->TWIHS_SR;
                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;					
                        }
                        else
                        {
                            twihs0Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;    
                        }
                    }
					// not the last TRB
                    else
                    {
                        twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                    }
                    
                    /* Disable the RXRDY interrupt*/
                    TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_RXRDY_Msk;
                }
			}
		    break;
		}
		
		case TWIHS_STATE_TRANSFER_WRITE:
		{
		    /* checks if master is ready to transmit */
            if( status & TWIHS_SR_TXRDY_Msk )
            {
                /* byte by byte data transmit */
                TWIHS0_Module->TWIHS_THR = TWIHS_THR_TXDATA(*(trb->pbuffer++));
                trb->length--;
                
                if( 0 == trb->length )
				{
				    // check for last TRB
                    if( currentTRB == (twihs0Obj.numTRBs - 1))
                    {
                        // Sends a stop bit
                        TWIHS0_Module->TWIHS_CR = TWIHS_CR_STOP_Msk;

                        /* read the module status again to know if 
                           tranmission complete flag is set */
                        status = TWIHS0_Module->TWIHS_SR;
                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs0Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;    
                        }
                    }
					// not the last TRB
                    else
                    {
                        twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                    }
                    
                    // Disable the RXRDY interrupt
                    TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
				}
			}

		    break;
		}
		
		case TWIHS_STATE_WAIT_FOR_TXCOMP:
		{
		    if( status & TWIHS_SR_TXCOMP_Msk )
            {
                twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;					
            }
		    break;
		}
		
		default:
		{
		    break;
		}
	}
	
	/* Check for error during transmission */
    if( TWIHS_STATE_ERROR == twihs0Obj.state )
    {	
		// Reset the PLib objects and Interrupts
		twihs0Obj.numTRBs = 0;
		currentTRB = 0;
        twihs0Obj.state = TWIHS_STATE_IDLE;
        TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | 
                                 TWIHS_IDR_TXRDY_Msk  |
                                 TWIHS_IDR_RXRDY_Msk;
        
        if ( twihs0Obj.callback != NULL )
		{
		    twihs0Obj.callback( twihs0Obj.context );
		}
	}
	
	// check for completion of transfer
	if( TWIHS_STATE_TRANSFER_DONE  == twihs0Obj.state )
	{
	    // Call callback on completion of last TRB.
	    if ( currentTRB == (twihs0Obj.numTRBs - 1) )
		{
		    twihs0Obj.error = TWIHS_ERROR_NONE;
			
			// Reset the PLib objects and Interrupts
			twihs0Obj.numTRBs = 0;
            currentTRB = 0;
            twihs0Obj.state = TWIHS_STATE_IDLE;
            TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | 
                                     TWIHS_IDR_TXRDY_Msk  |
                                     TWIHS_IDR_RXRDY_Msk;
            
            if ( twihs0Obj.callback != NULL )
		    {
		        twihs0Obj.callback( twihs0Obj.context );
		    }

			return;
		}
		
        currentTRB ++;
		twihs0Obj.state = TWIHS_STATE_ADDR_SEND;   
	}
    
    if( TWIHS_STATE_ADDR_SEND  == twihs0Obj.state )
    {
        trb = &twihs0TRBsList[currentTRB];
        
        TWIHS0_Module->TWIHS_MMR = 0;
        
        // 10-bit Slave Address
        if( trb->address > 0x007F)
        {
            TWIHS0_Module->TWIHS_MMR = TWIHS_MMR_DADR((trb->address & 0x00007F00) >> 8) | 
                                                   TWIHS_MMR_IADRSZ(1);

            // Set internal address
            TWIHS0_Module->TWIHS_IADR = TWIHS_IADR_IADR( trb->address & 0x000000FF );
        }
        // 7-bit Slave Address
        else
        {
            TWIHS0_Module->TWIHS_MMR = TWIHS_MMR_DADR(trb->address) | 
                                                   TWIHS_MMR_IADRSZ(0);
        }

        if( trb->read )
        {
            TWIHS0_Module->TWIHS_MMR|= TWIHS_MMR_MREAD_Msk; 

            // Send a START Condition
            TWIHS0_Module->TWIHS_CR = TWIHS_CR_START_Msk;

            if( 1 == trb->length )
            {
                if( currentTRB == (twihs0Obj.numTRBs - 1) )
                {
                    /* Send Stop at the same time as start 
                     * if size of data to be received is 1*/
                    TWIHS0_Module->TWIHS_CR = TWIHS_CR_STOP_Msk;
                }
                else
                {
                    // Send a Repeated Start
                    TWIHS0_Module->TWIHS_CR = TWIHS_CR_START_Msk;
                }
            }

            twihs0Obj.state = TWIHS_STATE_TRANSFER_READ;
            TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
            TWIHS0_Module->TWIHS_IER = TWIHS_IER_RXRDY_Msk;
        }
        else
        {
            if( currentTRB > 0 )
            {
                // Send a Repeated Start
                TWIHS0_Module->TWIHS_CR = TWIHS_CR_START_Msk;
            }
            
            twihs0Obj.state = TWIHS_STATE_TRANSFER_WRITE;
            TWIHS0_Module->TWIHS_IDR = TWIHS_IDR_RXRDY_Msk;
            TWIHS0_Module->TWIHS_IER = TWIHS_IER_TXRDY_Msk;
        }
    }
	
	return;
}	

/*******************************************************************************
 End of File
*/
