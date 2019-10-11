/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ32:

      int32 DSP_FilterIIRBQ32(int32 Xin, PARM_EQUAL_FILTER_32 *pFilter)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq32.S

  Summary:
    Performs a single-pass IIR Biquad Filter.

  Description:
    Calculates a single pass IIR biquad filter on Xin, and delivers the result 
    as a 16-bit output.  All math is performed using 32 bit instructions, whih
    results truncated to 32-bits for the output.  The delay register is stored
    as a 32-bit value for subsequent functions.
    All values are fractional Q31, see data structure for specifics.

Notes:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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
/*
    created    2013 July 26    C15637
 */

	.text
	.set		noreorder
	.set		nomacro

	.global		DSP_FilterIIRBQ32
	.ent		DSP_FilterIIRBQ32

DSP_FilterIIRBQ32:

#notes: global gain output not in function, may be required
#  measured 256 passes, each consistent at 42 M4K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int32 DSP_FilterIIRBQ32(int32 Xin, PARM_EQUAL_FILTER_32 *pFilter)
#
# $a0 - Xin
# $a1 - pFilter pointer
# $a2 - 
# $a3 -

# $t0 - z1 delay value (input)
# $t1 - reserved (fracGain)
# $t2 - reserved (expGain)
# $t3 - reserved (log2alpha)
# $t4 - z1 output value
# $t5 - intermediate mult value
# $t6 - intermediate mult values
# $t7 - z2 output value
# $t8 - intermediate mult value
# $t9 - intermediate mult value

# $s0 - 
# $s1 - b0 coef (feed forward)
# $s2 - b1 coef
# $s3 - b2 coef
# $s4 - a1 coef (feed back)
# $s5 - a2 coef
# $s6 - 
# $s7 - z2 delay value (input)

# $v0 - function 32-bit output
# $v1 - 
# Assumption: Delay line array is initialized with 0


#	lh		$t1, 0($a1)         # fracGain -- unused
#   lh      $t2, 2($a1)         # expGain, binary shift output gain -- unused
#   lw      $t3, 4($a1)         # log2alpha, binary gain in feedback -- unused
    lw      $s1, 8($a1)         # b0 coef
    lw      $t0, 28($a1)        # z1
    lw      $s2, 12($a1)        # b1 coef
    lw      $s4, 20($a1)        # a1 coef
    lw      $s7, 32($a1)        # z2
    lw      $s3, 16($a1)        # b2 coef
    lw      $s5, 24($a1)        # a2 coef

# calc Y
    mtlo    $zero
    mthi    $t0                 # z1
    madd    $a0, $s1            # (z1) + Xin * b0
    mfhi    $v0
#   sll     $v0, $v1, $t3       ### need to add log2alpha later (additional gain)
    sll     $v0, 2             # 2 bits gain, one for log2alpha, one for 31 bit math
    
# calc z1 & z2
    mtlo    $zero
    mthi    $s7                 # z2
    madd    $a0, $s2            # (z2) + Xin * b1
    mfhi    $t5
    mult    $v0, $s4            # Y * a1
    mfhi    $t6
    mult    $a0, $s3            # Xin * b2
    mfhi    $t8
    mult    $v0, $s5            # Y * a2
    mfhi    $t9

    subu    $t4, $t5, $t6       # [z2 + (Xin * b1)] - (Y * a1)
    sw      $t4, 28($a1)        # store z1

    subu    $t7, $t8, $t9       # (Xin * b2) - (Y * a2)
    

####

	jr		$ra                 # return
    sw      $t7, 32($a1)        # store z2

	.end		DSP_FilterIIRBQ32

