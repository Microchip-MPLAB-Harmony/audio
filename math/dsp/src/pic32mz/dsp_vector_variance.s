/*******************************************************************************
Microchip DSP Library
  DSP_VectorVariance:

      int32_t   DSP_VectorVariance(int32_t *inVector, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_variance.S

  Summary: 
    Computes the variance of a vector.

  Description: 
    Calculates the variance on the first N elements of inVector and returns the
    32-bit scalar result.  The variance is a measure of the delta from mean values,
    and the mean value of the vector is computed in the process.  The function has
    the form -

    var = sum[0..N]((x(i) - M(N))^2) / (N-1)
        where
    N is the number of vector elements
    x(i) is a single element in the vector
    M(N) is the mean of the N elements of the vector

    Input values of the vector and output scalar value is Q31 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Notes:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
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
/****************************************************************************
Change log

new content     C15637          2013-9-11
modified        C15637          2013-10-9
  - removed references to $Sx registers, improved speed & stability

****************************************************************************/

	.text
	.set	noreorder
	.set	nomacro
    .set    micromips

	.global	DSP_VectorVariance
	.ent	DSP_VectorVariance

DSP_VectorVariance:

# int32_t DSP_VectorVariance(int32_t *inVector, int N)
#
#  426 cycles for 4 element vector
#  464 cycles for 8 element vector
#
# $a0 - input data vector pointer
# $a1 - Number of array elements, final address
# $a2 -
# $a3 -

# $t0 - intermedite values
# $t1 - intermedite values
# $t2 - temp constant, intermedite values
# $t3 - intermedite values
# $t4 - 1 / N
# $t5 - temp constant
# $t6 - 1 / (N-1)
# $t7 - group of 4 sum
# $t8 - inVector address pointer
# $t9 - input N trap, function accumulator

# $s0 - 
# $s1 -
# $s2 -
# $s3 -
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $v0 - function 32-bit output
# $v1 - mean average
# Assumption: N must be a multiple of 4, and greater than 4

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done
	move    $t4, $a1            # number of elements(N)

    li      $t5, 0x80000000     # unsigned 1.0
    divu    $t5, $t4            # div unsigned, 1.0/N

    addi    $t6, $a1, -1        # N - 1
               
    MFLO    $t4                 # quotient in $t4  # scale factor 1/N
    li      $t2, 0x80000000     # another 1.0, first one consumed in div
    divu    $t2, $t6            # div 1.0/(N-1)

    andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll     $a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata
    move    $t8, $a0            # store vector starting address

    mflo    $t6, $ac0           # quotient 1/(N-1)
    MTHI    $zero, $ac0         # clear the accumulator
    MTLO    $zero, $ac0
    mthi    $zero, $ac1
    mtlo    $zero, $ac1

loop_mean:
	lwp	$t0,  0($a0)            # load indata[i, i+1]
	lwp	$t2,  8($a0)            # load indata[i+2, i+3]
	
    DPAQ_SA.L.W $ac0,$t0,$t4    # accum, indata[i] * 1/N
   	DPAQ_SA.L.W $ac0,$t1,$t4
   	DPAQ_SA.L.W $ac0,$t2,$t4
    DPAQ_SA.L.W $ac0,$t3,$t4

    addiu	$a0, $a0, 16		# 4 * sizeof(int32), next i*4
	bne     $a1, $a0, loop_mean
	MFHI    $v1                 # mean value (M)

# between loops
    move    $a0, $t8            # reset vector pointer for next loop
    mthi    $zero, $ac0           # clear accumulator
    mtlo    $zero, $ac0

loop_sum:
    lwp	$t0,  0($a0)            # load indata[i, i+1]
	lwp	$t2,  8($a0)            # load indata[i+2, i+3]

    subq_s.w  $t0, $t0, $v1     # x(i) - M
    subq_s.w  $t1, $t1, $v1     # x(i+1) - M
    subq_s.w  $t2, $t2, $v1     # x(i+2) - M
    subq_s.w  $t3, $t3, $v1     # x(i+3) - M

    dpaq_sa.l.w $ac0, $t0, $t0  # sum += [x(i) - M] ^2
    dpaq_sa.l.w $ac0, $t1, $t1  # sum += [x(i+1) - M] ^2
    dpaq_sa.l.w $ac0, $t2, $t2  # sum += [x(i+2) - M] ^2
    dpaq_sa.l.w $ac0, $t3, $t3  # sum += [x(i+3) - M] ^2

    addiu   $a0, $a0, 16        # next 4 inputs

    mfhi    $t7, $ac0           # get sum
    dpaq_sa.l.w  $ac1, $t7, $t6 # total += sum * 1/(N-1)

    mthi    $zero, $ac0         # clear (sum) accumulator
    mtlo    $zero, $ac0

    bne     $a1, $a0, loop_sum
    mfhi    $t9, $ac1           # get total

# outside of loop

done:
   	jr	$ra                     # exit
	move $v0, $t9               # output = result total

	.end	DSP_VectorVariance
