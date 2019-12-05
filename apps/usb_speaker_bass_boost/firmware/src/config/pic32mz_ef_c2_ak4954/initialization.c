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
#include "configuration.h"
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_LEVEL_3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_2
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_66
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_12MHZ

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0x0





// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 0 Initialization Data">

/* I2C Client Objects Pool */
static DRV_I2C_CLIENT_OBJ drvI2C0ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX0];

/* I2C Transfer Objects Pool */
static DRV_I2C_TRANSFER_OBJ drvI2C0TransferObj[DRV_I2C_QUEUE_SIZE_IDX0];

/* I2C PLib Interface Initialization */
const DRV_I2C_PLIB_INTERFACE drvI2C0PLibAPI = {

    /* I2C PLib Transfer Read Add function */
    .read = (DRV_I2C_PLIB_READ)I2C_BB_Read,

    /* I2C PLib Transfer Write Add function */
    .write = (DRV_I2C_PLIB_WRITE)I2C_BB_Write,


    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_PLIB_WRITE_READ)I2C_BB_WriteRead,

    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_PLIB_ERROR_GET)I2C_BB_ErrorGet,

    /* I2C PLib Transfer Setup function */
    .transferSetup = (DRV_I2C_PLIB_TRANSFER_SETUP)I2C_BB_TransferSetup,

    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_PLIB_CALLBACK_REGISTER)I2C_BB_CallbackRegister,
};


const DRV_I2C_INTERRUPT_SOURCES drvI2C0InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.i2cInterrupt             = I2C_BB_IRQn,
};

/* I2C Driver Initialization Data */
const DRV_I2C_INIT drvI2C0InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C0PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX0,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C0ClientObjPool[0],

    /* I2C TWI Queue Size */
    .transferObjPoolSize = DRV_I2C_QUEUE_SIZE_IDX0,

    /* I2C Transfer Objects */
    .transferObjPool = (uintptr_t)&drvI2C0TransferObj[0],

    /* I2C interrupt sources */
    .interruptSources = &drvI2C0InterruptSources,

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX0,
};

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_I2S Instance 0 Initialization Data">

/* I2S PLIB Interface Initialization */
DRV_I2S_PLIB_INTERFACE drvI2S0PlibAPI =
{
    .I2S_LRCLK_Get = (DRV_I2S_LRCLK_GET)I2S2_LRCLK_Get,  
    .I2S_RefClockSet = (DRV_I2S_REFCLOCKSET) I2S2_RefClockSet,
    .I2S_BaudRateSet = (DRV_I2S_BAUDRATESET) I2S2_BaudRateSet,
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

    .dmaChannelTransmit = DRV_I2S_XMIT_DMA_CH_IDX0,
    .dmaChannelReceive  = DRV_I2S_RCV_DMA_CH_IDX0,
    .i2sTransmitAddress = (void *)&(SPI2BUF),
    .i2sReceiveAddress = (void *)&(SPI2BUF),
    .interruptDMA = _DMA0_VECTOR,

    .dmaDataLength = DRV_I2S_DATA_LENGTH_IDX0,
};

// </editor-fold>

/*** CODEC Driver Initialization Data ***/
const DRV_AK4954_INIT drvak4954Codec0InitData =
{
    .i2sDriverModuleIndex = DRV_AK4954_I2S_DRIVER_MODULE_INDEX_IDX0,
    .i2cDriverModuleIndex = DRV_AK4954_I2C_DRIVER_MODULE_INDEX_IDX0,
    .masterMode = DRV_AK4954_MASTER_MODE,
    .samplingRate = DRV_AK4954_AUDIO_SAMPLING_RATE,
    .volume = DRV_AK4954_VOLUME,
    .audioDataFormat = DRV_AK4954_AUDIO_DATA_FORMAT_MACRO,
    .whichMicInput = DRV_AK4954_WHICH_MIC_INPUT,
    .enableMicBias = DRV_AK4954_ENABLE_MIC_BIAS,
    .micGain = DRV_AK4954_MIC_GAIN,
    .delayDriverInitialization = DRV_AK4954_DELAY_INITIALIZATION,
};



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
/******************************************************
 * USB Driver Initialization
 ******************************************************/

const DRV_USBHS_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB,

	/* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_DMA,
	
    /* System module initialization */
    .moduleInit = {0},

    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBHS_OPMODE_DEVICE,

	/* Enable Full Speed Operation */
	.operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .stopInIdle = true,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USBHS_ID_0,
	
};




// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)CORETIMER_CallbackSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)CORETIMER_CounterGet,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)CORETIMER_FrequencyGet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)CORETIMER_CompareSet,
    .timerStart = (SYS_TIME_PLIB_START)CORETIMER_Start,
    .timerStop = (SYS_TIME_PLIB_STOP)CORETIMER_Stop 
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = 0,
};

// </editor-fold>

const SYS_DEBUG_INIT debugInit =
{
    .moduleInit = {0},
    .errorLevel = SYS_DEBUG_GLOBAL_ERROR_LEVEL,
    .consoleIndex = 0,
};


// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Instance 0 Initialization Data">

static QElement sysConsole0UARTRdQueueElements[SYS_CONSOLE_UART_RD_QUEUE_DEPTH_IDX0];
static QElement sysConsole0UARTWrQueueElements[SYS_CONSOLE_UART_WR_QUEUE_DEPTH_IDX0];

/* Declared in console device implementation (sys_console_uart.c) */
extern const SYS_CONSOLE_DEV_DESC sysConsoleUARTDevDesc;

const SYS_CONSOLE_UART_PLIB_INTERFACE sysConsole0UARTPlibAPI =
{
    .read = (SYS_CONSOLE_UART_PLIB_READ)UART1_Read,
    .write = (SYS_CONSOLE_UART_PLIB_WRITE)UART1_Write,
    .readCallbackRegister = (SYS_CONSOLE_UART_PLIB_REGISTER_CALLBACK_READ)UART1_ReadCallbackRegister,
    .writeCallbackRegister = (SYS_CONSOLE_UART_PLIB_REGISTER_CALLBACK_WRITE)UART1_WriteCallbackRegister,
    .errorGet = (SYS_CONSOLE_UART_PLIB_ERROR_GET)UART1_ErrorGet,
};


const SYS_CONSOLE_UART_INTERRUPT_SOURCES sysConsole0UARTInterruptSources =
{
    /* Peripheral has more than one interrupt vector */
    .isSingleIntSrc                        = false,

    /* Peripheral interrupt lines */
    .intSources.multi.usartTxCompleteInt   = _UART1_TX_VECTOR,
    .intSources.multi.usartTxReadyInt      = -1,
    .intSources.multi.usartRxCompleteInt   = _UART1_RX_VECTOR,
    .intSources.multi.usartErrorInt        = _UART1_FAULT_VECTOR,
};

const SYS_CONSOLE_UART_INIT_DATA sysConsole0UARTInitData =
{
    .uartPLIB = &sysConsole0UARTPlibAPI,
    .readQueueElementsArr = sysConsole0UARTRdQueueElements,
    .writeQueueElementsArr = sysConsole0UARTWrQueueElements,
    .readQueueDepth = SYS_CONSOLE_UART_RD_QUEUE_DEPTH_IDX0,
    .writeQueueDepth = SYS_CONSOLE_UART_WR_QUEUE_DEPTH_IDX0,
    .interruptSources = &sysConsole0UARTInterruptSources,
};

const SYS_CONSOLE_INIT sysConsole0Init =
{
    .deviceInitData = (const void*)&sysConsole0UARTInitData,
    .consDevDesc = &sysConsoleUARTDevDesc,
    .deviceIndex = 0,
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
    /* Start out with interrupts disabled before configuring any modules */
    __builtin_disable_interrupts();

  
    CLK_Initialize();
	GPIO_Initialize();
    
    /* Configure Prefetch, Wait States and ECC */
    PRECONbits.PREFEN = 3;
    PRECONbits.PFMWS = 2;
    CFGCONbits.ECCCON = 3;


    DMAC_Initialize();

	BSP_Initialize();
    CORETIMER_Initialize();
	UART1_Initialize();

    TMR2_Initialize();

	I2S2_Initialize();


    /* Initialize I2C0 Driver Instance */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);
    /* Initialize I2S0 Driver Instance */
    sysObj.drvI2S0 = DRV_I2S_Initialize(DRV_I2S_INDEX_0, (SYS_MODULE_INIT *)&drvI2S0InitData);


    I2C_BB_Initialize();
    sysObj.drvak4954Codec0 = DRV_AK4954_Initialize(DRV_AK4954_INDEX_0, (SYS_MODULE_INIT *)&drvak4954Codec0InitData);


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);
    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*)&debugInit);


    sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&sysConsole0Init);



	 /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);
	
	

	/* Initialize USB Driver */ 
    sysObj.drvUSBHSObject = DRV_USBHS_Initialize(DRV_USBHS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);	


    APP_Initialize();


    EVIC_Initialize();

	/* Enable global interrupts */
    __builtin_enable_interrupts();


}


/*******************************************************************************
 End of File
*/
