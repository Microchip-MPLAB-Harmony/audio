/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017-2018 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/*** CODEC Driver Initialization Data ***/
const DRV_WM8904_INIT drvwm8904Codec0InitData =
{
    .samplingRate = DRV_WM8904_AUDIO_SAMPLING_RATE,
    .volume = DRV_WM8904_VOLUME,
    .audioDataFormat = DRV_WM8904_AUDIO_DATA_FORMAT_MACRO,
    .enableMicInput = DRV_WM8904_ENABLE_MIC_INPUT
};

// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 0 Initialization Data">

/* I2C Client Objects Pool */
DRV_I2C_CLIENT_OBJ drvI2C0ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX0] = {0};

/* I2C Transfer Objects Pool */
DRV_I2C_TRANSFER_OBJ drvI2C0TransferObj[DRV_I2C_QUEUE_SIZE_IDX0] = {0};

/* I2C PLib Interface Initialization */
DRV_I2C_PLIB_INTERFACE drvI2C0PLibAPI = {
    
    /* I2C PLib Transfer Setup */
    .transferSetup = (DRV_I2C_TRANSFER_SETUP_CALLBACK)TWIHS0_TransferSetup,
    
    /* I2C PLib Transfer Read Add function */
    .read = (DRV_I2C_READ_CALLBACK)TWIHS0_Read,
    
    /* I2C PLib Transfer Write Add function */
    .write = (DRV_I2C_WRITE_CALLBACK)TWIHS0_Write,
    
    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_WRITE_READ_CALLBACK)TWIHS0_WriteRead,
    
    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_ERROR_GET_CALLBACK)TWIHS0_ErrorGet,
    
    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_CALLBACK_REGISTER_CALLBACK)TWIHS0_CallbackRegister,
};

/* I2C Driver Initialization Data */
DRV_I2C_INIT drvI2C0InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C0PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX0,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C0ClientObjPool[0],

    /* I2C IRQ */
    .interruptI2C = DRV_I2C_INT_SRC_IDX0,
    
    /* I2C TWI Queue Size */
    .queueSize = DRV_I2C_QUEUE_SIZE_IDX0,
    
    /* I2C Transfer Objects */
    .transferObj = (uintptr_t)&drvI2C0TransferObj[0],

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX0,
};

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_I2S Instance 0 Initialization Data">

/* I2S PLIB Interface Initialization */
DRV_I2S_PLIB_INTERFACE drvI2S0PlibAPI =
{  
    /* I2S PLIB Set Baud */
    .setBaud = (DRV_BAUDSET)SSC0_BaudSet
};

/* I2S Driver Initialization Data */
DRV_I2S_INIT drvI2S0InitData =
{
    /* I2S PLIB API */
    .i2sPlib = &drvI2S0PlibAPI,

    /* I2S IRQ */
    .interruptI2S = DRV_I2S_INT_SRC_IDX0,
    
    /* I2S Number of clients */
    .numClients = DRV_I2S_CLIENTS_NUMBER_IDX0,

    /* I2S Queue Size */
    .queueSize = DRV_I2S_QUEUE_SIZE_IDX0,  
    
    .dmaChannelTransmit = DRV_SSC_XMIT_DMA_CH_IDX0,
    .dmaChannelReceive  = DRV_SSC_RCV_DMA_CH_IDX0,
    .i2sTransmitAddress = (void *)SSC_TRANSMIT_ADDRESS,
    .i2sReceiveAddress = (void *)SSC_RECEIVE_ADDRESS,
    .interruptDMA = XDMAC_IRQn,

    .dmaDataLength = DRV_I2S_DATA_LENGTH_IDX0,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

TIME_PLIB_API sysTimePlibAPI = {
    .timerCallbackSet = (TIME_CallbackSet)TC0_CH0_TimerCallbackRegister,
    .timerCounterGet = (TIME_CounterGet)TC0_CH0_TimerCounterGet,
    .timerPeriodSet = (TIME_PeriodSet)TC0_CH0_TimerPeriodSet,
    .timerStart = (TIME_Start)TC0_CH0_TimerStart,
    .timerStop = (TIME_Stop)TC0_CH0_TimerStop
};

SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .timeInterrupt = TC0_CH0_IRQn,
    .timeFrequency = TC0_CH0_TimerFrequencyGet()
};

// </editor-fold>


/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    CLK_Initialize();
	PIO_Initialize();

    NVIC_Initialize();
    XDMAC_Initialize();
	RSWDT_REGS->RSWDT_MR|= RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 
	WDT_REGS->WDT_MR|= WDT_MR_WDDIS_Msk; 		// Disable WDT 
	BSP_Initialize();
	SSC0_Initialize();
 
    TC0_CH0_TimerInitialize(); 
     
    
	TWIHS0_Initialize();

    sysObj.drvwm8904Codec0 = DRV_WM8904_Initialize(DRV_WM8904_INDEX_0, (SYS_MODULE_INIT *)&drvwm8904Codec0InitData);

    /* Initialize I2C0 Driver Instance */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);
    /* Initialize I2S0 Driver Instance */
    sysObj.drvI2S0 = DRV_I2S_Initialize(DRV_I2S_INDEX_0, (SYS_MODULE_INIT *)&drvI2S0InitData);


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);



    APP_Initialize();


}


/*******************************************************************************
 End of File
*/

