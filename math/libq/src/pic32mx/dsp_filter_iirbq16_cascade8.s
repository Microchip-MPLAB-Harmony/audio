/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ16_cascade8

      int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq16_cascade8.S

  Summary:
    Performs a single IIR Biquad Filter as a cascade of 8 series filters.

  Description: 
    Calculates a single pass IIR biquad cascade filter on Xin, and delivers the
    result as a 16-bit output.  The cascade of filters is 8 unique biquad filters
    arranged in series such that the output of one is provided as the input to
    the next.  A unique filter coefficient set is provided to each, and 32 bit
    delay lines are maintained for each.  All math is performed using 32 bit
    instructions, which results truncated to 16-bits for the output.  The delay
    register is stored as a 32-bit value for subsequent functions.
    All values are fractional Q15 and Q31, see data structure for specifics.

Notes:
    All delay registers values should be initialized to zero prior to the first
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
    created    2013 August 1    C15637
 */

	.text
	.set		noreorder
	.set		nomacro

	.global		DSP_FilterIIRBQ16_cascade8
	.ent		DSP_FilterIIRBQ16_cascade8

DSP_FilterIIRBQ16_cascade8:

#notes: global gain output not in function, may be required
#  measured 256 passes, each consistent at 234 M4K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array)
#
# $a0 - Xin
# $a1 - pFilter pointer, increments with loops
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
# $s6 - loop end comparison, $a1 + (loops * filter length)
# $s7 - z2 delay value (input)

# $v0 - function 16-bit output
# $v1 - intermediate Y16 value
# Assumption: Delay line array is initialized with 0


#	lh		$t1, 0($a1)         # fracGain -- unused
#   lh      $t2, 2($a1)         # expGain, binary shift output gain -- unused
#   lh      $t3, 4($a1)         # log2alpha, binary gain in feedback -- unused
    addu    $s6, $a1, 192       # end address is 8*24 (len of filt stucture)


loop:
    lh      $s1, 6($a1)         # b0 coef
    lw      $t0, 16($a1)        # z1
    lh      $s2, 8($a1)         # b1 coef
    lh      $s4, 12($a1)        # a1 coef
    lw      $s7, 20($a1)        # z2
    lh      $s3, 10($a1)        # b2 coef
    lh      $s5, 14($a1)        # a2 coef
    

# calc Y
    mul     $t6, $a0, $s1       # Xin * b0
    add     $v1, $t6, $t0       # Y = (Xin * b0) + z1
#   sll     $v1, $v1, $t3       ### need to add log2alpha later (additional gain)
    sra     $v1, 14             # truncate (Y16), 2 bits gain, one for log2alpha, one for 15 bit math
    
# calc z1 & z2
    mul     $t5, $a0, $s2       # Xin * b1
    mul     $t6, $v1, $s4       # Y * a1
    mul     $t8, $a0, $s3       # Xin * b2
    mul     $t9, $v1, $s5       # Y * a2

    subu    $t4, $t5, $t6       # (Xin * b1) - (Y * a1)
    add     $t4, $t4, $s7       # (((Xin*b1)) - (Y*a1)) + z2
    sw      $t4, 16($a1)        # store z1

    subu    $t7, $t8, $t9       # (Xin * b2) - (Y * a2)
    sw      $t7, 20($a1)        # store z2
    add     $a1, $a1, 24        # increment length of filter structure
    
    bne     $a1, $s6, loop      # loop if not at end address
    move    $a0, $v1            # assign output to the input for each loop pass
    
####

	jr		$ra                 # return
	seh		$v0, $v1            # end value return, int 16

	.end		DSP_FilterIIRBQ16_cascade8

