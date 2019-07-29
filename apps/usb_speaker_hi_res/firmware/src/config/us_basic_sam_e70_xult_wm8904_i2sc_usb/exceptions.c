/*******************************************************************************
  System Exceptions File

  File Name:
    exceptions.c

  Summary:
    This file contains a function which overrides the default _weak_ exception
    handlers provided by the interrupt.c file.

  Description:
    This file redefines the default _weak_  exception handler with a more debug
    friendly one. If an unexpected exception occurs the code will stop in a
    while(1) loop.
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
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: Exception Handling Routine
// *****************************************************************************
// *****************************************************************************

/* Brief default interrupt handlers for core IRQs.*/

void NonMaskableInt_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
    __builtin_software_breakpoint();
#endif
    while (1)
    {
    }
}

void HardFault_Handler_C(uint32_t * hardfault_args, unsigned int lr_value)
{
   uint32_t stacked_r0;
   uint32_t stacked_r1;
   uint32_t stacked_r2;
   uint32_t stacked_r3;
   uint32_t stacked_r12;
   uint32_t stacked_lr;
   uint32_t stacked_pc;
   uint32_t stacked_psr;
   uint32_t cfsr;
   uint32_t bus_fault_address;
   uint32_t memmanage_fault_address;

   bus_fault_address       = SCB->BFAR;  // BusFault Address Register
   memmanage_fault_address = SCB->MMFAR; // MemManage Fault Address Register
   cfsr                    = SCB->CFSR;  // Configurable Fault Status Register

   stacked_r0  = hardfault_args[0];
   stacked_r1  = hardfault_args[1];
   stacked_r2  = hardfault_args[2];
   stacked_r3  = hardfault_args[3];
   stacked_r12 = hardfault_args[4];
   stacked_lr  = hardfault_args[5];  // Link Register
   stacked_pc  = hardfault_args[6];  // Program Counter
   stacked_psr = hardfault_args[7];  // Program Status Register

//KEEP THIS- SYS_PRINT for Debug
   SYS_PRINT("\r\n");
   SYS_PRINT("[HardFault]\r\n");
   SYS_PRINT("- Stack frame:\r\n");
   SYS_PRINT(" R0  = 0x%08lX\r\n", stacked_r0);
   SYS_PRINT(" R1  = 0x%08lX\r\n", stacked_r1);
   SYS_PRINT(" R2  = 0x%08lX\r\n", stacked_r2);
   SYS_PRINT(" R3  = 0x%08lX\r\n", stacked_r3);
   SYS_PRINT(" R12 = 0x%08lX\r\n", stacked_r12);
   SYS_PRINT(" LR  = 0x%08lX\r\n", stacked_lr);
   SYS_PRINT(" PC  = 0x%08lX\r\n", stacked_pc);
   SYS_PRINT(" PSR = 0x%08lX\r\n", stacked_psr);

   SYS_PRINT("- FSR/FAR:\r\n");
   SYS_PRINT(" CFSR = 0x%08lX\r\n", cfsr);
   SYS_PRINT(" HFSR = 0x%08lX\r\n", SCB->HFSR); // HardFault Status
   SYS_PRINT(" DFSR = 0x%08lX\r\n", SCB->DFSR); // Debug Fault Status
   SYS_PRINT(" AFSR = 0x%08lX\r\n", SCB->AFSR); // Auxiliary Fault Status
   if (cfsr & 0x0080) SYS_PRINT(" MMFAR = 0x%08lX\r\n", memmanage_fault_address);
   if (cfsr & 0x8000) SYS_PRINT(" BFAR = 0x%08lX\r\n", bus_fault_address);

   SYS_PRINT("- Misc\r\n");
   SYS_PRINT(" LR/EXC_RETURN = 0x%X, Bit 2: %d\r\n", lr_value, (lr_value & 0x4)>>2 );

  #if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
  __builtin_software_breakpoint();
  #endif

   while (1)
   {
       // Do Nothing
   }
}

void DebugMonitor_Handler_C(uint32_t * hardfault_args, unsigned int lr_value)
{
   uint32_t stacked_r0;
   uint32_t stacked_r1;
   uint32_t stacked_r2;
   uint32_t stacked_r3;
   uint32_t stacked_r12;
   uint32_t stacked_lr;
   uint32_t stacked_pc;
   uint32_t stacked_psr;
   uint32_t cfsr;

   cfsr = SCB->CFSR;  // Configurable Fault Status Register

   stacked_r0  = hardfault_args[0];
   stacked_r1  = hardfault_args[1];
   stacked_r2  = hardfault_args[2];
   stacked_r3  = hardfault_args[3];
   stacked_r12 = hardfault_args[4];
   stacked_lr  = hardfault_args[5];  // Link Register
   stacked_pc  = hardfault_args[6];  // Program Counter
   stacked_psr = hardfault_args[7];  // Program Status Register

   SYS_PRINT("\r\n");
   SYS_PRINT("[DebugFault]\r\n");
   SYS_PRINT("- Stack frame:\r\n");
   SYS_PRINT(" R0  = 0x%08lX\r\n", stacked_r0);
   SYS_PRINT(" R1  = 0x%08lX\r\n", stacked_r1);
   SYS_PRINT(" R2  = 0x%08lX\r\n", stacked_r2);
   SYS_PRINT(" R3  = 0x%08lX\r\n", stacked_r3);
   SYS_PRINT(" R12 = 0x%08lX\r\n", stacked_r12);
   SYS_PRINT(" LR  = 0x%08lX\r\n", stacked_lr);
   SYS_PRINT(" PC  = 0x%08lX\r\n", stacked_pc);
   SYS_PRINT(" PSR = 0x%08lX\r\n", stacked_psr);

   SYS_PRINT("- FSR/FAR:\r\n");
   SYS_PRINT(" CFSR = 0x%08lX\r\n", cfsr);
   SYS_PRINT(" HFSR = 0x%08lX\r\n", SCB->HFSR); // HardFault Status
   SYS_PRINT(" DFSR = 0x%08lX\r\n", SCB->DFSR); // Debug Fault Status
   SYS_PRINT(" AFSR = 0x%08lX\r\n", SCB->AFSR); // Auxiliary Fault Status

   SYS_PRINT("- Misc\r\n");
   SYS_PRINT(" LR/EXC_RETURN = 0x%X, Bit 2: %d\r\n", lr_value, (lr_value & 0x4)>>2 );

  #if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
  #endif
   while (1)
   {
       // Do Nothing
   }
}

void MemoryManagement_Handler_C(uint32_t * hardfault_args, unsigned int lr_value)
{
   uint32_t stacked_r0;
   uint32_t stacked_r1;
   uint32_t stacked_r2;
   uint32_t stacked_r3;
   uint32_t stacked_r12;
   uint32_t stacked_lr;
   uint32_t stacked_pc;
   uint32_t stacked_psr;
   uint32_t cfsr;
   uint8_t  mmfsr;
   uint32_t memmanage_fault_address;

   memmanage_fault_address = SCB->MMFAR;   // MemManage Fault Address Register
   cfsr                    = SCB->CFSR;    // Configurable Fault Status Register
   mmfsr = cfsr & 0xFF;                    // MemManage Fault Status

   stacked_r0  = hardfault_args[0];
   stacked_r1  = hardfault_args[1];
   stacked_r2  = hardfault_args[2];
   stacked_r3  = hardfault_args[3];
   stacked_r12 = hardfault_args[4];
   stacked_lr  = hardfault_args[5];  // Link Register
   stacked_pc  = hardfault_args[6];  // Program Counter
   stacked_psr = hardfault_args[7];  // Program Status Register

   SYS_PRINT("\r\n");
   SYS_PRINT("[MemoryManagement]\r\n");
   SYS_PRINT("- Stack frame:\r\n");
   SYS_PRINT(" R0  = 0x%08lX\r\n", stacked_r0);
   SYS_PRINT(" R1  = 0x%08lX\r\n", stacked_r1);
   SYS_PRINT(" R2  = 0x%08lX\r\n", stacked_r2);
   SYS_PRINT(" R3  = 0x%08lX\r\n", stacked_r3);
   SYS_PRINT(" R12 = 0x%08lX\r\n", stacked_r12);
   SYS_PRINT(" LR  = 0x%08lX\r\n", stacked_lr);
   SYS_PRINT(" PC  = 0x%08lX\r\n", stacked_pc);
   SYS_PRINT(" PSR = 0x%08lX\r\n", stacked_psr);

   SYS_PRINT("- FSR/FAR:\r\n");
   SYS_PRINT(" CFSR  = 0x%08lX\r\n", cfsr);
   SYS_PRINT(" MMFSR = 0x%02X\r\n", mmfsr);
   SYS_PRINT(" HFSR  = 0x%08lX\r\n", SCB->HFSR); // HardFault Status
   SYS_PRINT(" DFSR  = 0x%08lX\r\n", SCB->DFSR); // Debug Fault Status
   SYS_PRINT(" AFSR  = 0x%08lX\r\n", SCB->AFSR); // Auxiliary Fault Status
   if (cfsr & 0x0080) SYS_PRINT(" MMFAR = 0x%08lX\r\n", memmanage_fault_address);

   SYS_PRINT("- Misc\r\n");
   SYS_PRINT(" LR/EXC_RETURN = 0x%X, Bit 2: %d\r\n", lr_value, (lr_value & 0x4)>>2 );

  #if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
  #endif
   while (1)
   {
       // Do Nothing
   }
}

void BusFault_Handler_C(uint32_t * hardfault_args, unsigned int lr_value)
{
   uint32_t stacked_r0;
   uint32_t stacked_r1;
   uint32_t stacked_r2;
   uint32_t stacked_r3;
   uint32_t stacked_r12;
   uint32_t stacked_lr;
   uint32_t stacked_pc;
   uint32_t stacked_psr;
   uint32_t cfsr;
   uint8_t  bfsr;
   uint32_t bus_fault_address;

   bus_fault_address = SCB->BFAR;  // BusFault Address Register
   cfsr              = SCB->CFSR;  // Configurable Fault Status Register
   bfsr = ( cfsr & 0x0000FF00 ) >>8;  // Bus Fault Status

   stacked_r0  = hardfault_args[0];
   stacked_r1  = hardfault_args[1];
   stacked_r2  = hardfault_args[2];
   stacked_r3  = hardfault_args[3];
   stacked_r12 = hardfault_args[4];
   stacked_lr  = hardfault_args[5];  // Link Register
   stacked_pc  = hardfault_args[6];  // Program Counter
   stacked_psr = hardfault_args[7];  // Program Status Register

   SYS_PRINT("\r\n");
   SYS_PRINT("[BusFault]\r\n");
   SYS_PRINT("- Stack frame:\r\n");
   SYS_PRINT(" R0  = 0x%08lX\r\n", stacked_r0);
   SYS_PRINT(" R1  = 0x%08lX\r\n", stacked_r1);
   SYS_PRINT(" R2  = 0x%08lX\r\n", stacked_r2);
   SYS_PRINT(" R3  = 0x%08lX\r\n", stacked_r3);
   SYS_PRINT(" R12 = 0x%08lX\r\n", stacked_r12);
   SYS_PRINT(" LR  = 0x%08lX\r\n", stacked_lr);
   SYS_PRINT(" PC  = 0x%08lX\r\n", stacked_pc);
   SYS_PRINT(" PSR = 0x%08lX\r\n", stacked_psr);

   SYS_PRINT("- FSR/FAR:\r\n");
   SYS_PRINT(" CFSR  = 0x%08lX\r\n", cfsr);
   SYS_PRINT(" BFSR  = 0x%02X\r\n",  bfsr);
   SYS_PRINT(" HFSR  = 0x%08lX\r\n", SCB->HFSR); // HardFault Status
   SYS_PRINT(" DFSR  = 0x%08lX\r\n", SCB->DFSR); // Debug Fault Status
   SYS_PRINT(" AFSR  = 0x%08lX\r\n", SCB->AFSR); // Auxiliary Fault Status
   if (bfsr & 0x04) SYS_PRINT(" ABFSR = 0x%08lX\r\n", SCB->ABFSR);
   if (bfsr & 0x80) SYS_PRINT(" BFAR = 0x%08lX\r\n", bus_fault_address);

   SYS_PRINT("- Misc\r\n");
   SYS_PRINT(" LR/EXC_RETURN = 0x%X, Bit 2: %d\r\n", lr_value, (lr_value & 0x4)>>2 );

  #if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
  #endif
   while (1)
   {
       // Do Nothing
   }
}

void UsageFault_Handler_C(uint32_t * hardfault_args, unsigned int lr_value)
{
   uint32_t stacked_r0;
   uint32_t stacked_r1;
   uint32_t stacked_r2;
   uint32_t stacked_r3;
   uint32_t stacked_r12;
   uint32_t stacked_lr;
   uint32_t stacked_pc;
   uint32_t stacked_psr;
   uint32_t cfsr;
   uint16_t ufsr;

   cfsr = SCB->CFSR;  // Configurable Fault Status Register
   ufsr = cfsr >> 16; // Usage Fault Status

   stacked_r0  = hardfault_args[0];
   stacked_r1  = hardfault_args[1];
   stacked_r2  = hardfault_args[2];
   stacked_r3  = hardfault_args[3];
   stacked_r12 = hardfault_args[4];
   stacked_lr  = hardfault_args[5];  // Link Register
   stacked_pc  = hardfault_args[6];  // Program Counter
   stacked_psr = hardfault_args[7];  // Program Status Register

   SYS_PRINT("\r\n");
   SYS_PRINT("[UsageFault]\r\n");
   SYS_PRINT("- Stack frame:\r\n");
   SYS_PRINT(" R0  = 0x%08lX\r\n", stacked_r0);
   SYS_PRINT(" R1  = 0x%08lX\r\n", stacked_r1);
   SYS_PRINT(" R2  = 0x%08lX\r\n", stacked_r2);
   SYS_PRINT(" R3  = 0x%08lX\r\n", stacked_r3);
   SYS_PRINT(" R12 = 0x%08lX\r\n", stacked_r12);
   SYS_PRINT(" LR  = 0x%08lX\r\n", stacked_lr);
   SYS_PRINT(" PC  = 0x%08lX\r\n", stacked_pc);
   SYS_PRINT(" PSR = 0x%08lX\r\n", stacked_psr);

   SYS_PRINT("- FSR/FAR:\r\n");
   SYS_PRINT(" CFSR = 0x%08lX\r\n", cfsr);
   SYS_PRINT(" UFSR = 0x%04X\r\n",  ufsr);
   SYS_PRINT(" HFSR = 0x%08lX\r\n", SCB->HFSR); // HardFault Status
   SYS_PRINT(" DFSR = 0x%08lX\r\n", SCB->DFSR); // Debug Fault Status
   SYS_PRINT(" AFSR = 0x%08lX\r\n", SCB->AFSR); // Auxiliary Fault Status

   SYS_PRINT("- Misc\r\n");
   SYS_PRINT(" LR/EXC_RETURN = 0x%X, Bit 2: %d\r\n", lr_value, (lr_value & 0x4)>>2 );

  #if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
  #endif
   while (1)
   {
       // Do Nothing
   }
}
/*******************************************************************************
 End of File
 */

