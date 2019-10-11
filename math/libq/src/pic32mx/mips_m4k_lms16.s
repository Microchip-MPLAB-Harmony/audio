# ****************************Begin Copyright 1D**********************************
 # Unpublished work (c) MIPS Technologies, Inc.  All rights reserved.
 # Unpublished rights reserved under the copyright laws of the United States
 # of America and other countries.
 # 
 # This code is confidential and proprietary to MIPS Technologies, Inc. ("MIPS
 # Technologies") and  may be disclosed only as permitted in writing by MIPS
 # Technologies or an authorized third party.  Any copying, reproducing,
 # modifying, use or disclosure of this code (in whole or in part) that is not
 # expressly permitted in writing by MIPS Technologies or an authorized third
 # party is strictly prohibited.  At a minimum, this code is protected under
 # trade secret, unfair competition, and copyright laws.  Violations thereof
 # may result in criminal penalties and fines.
 # 
 # MIPS Technologies reserves the right to change this code to improve function,
 # design or otherwise.  MIPS Technologies does not assume any liability arising
 # out of the application or use of this code, or of any error or omission in
 # such code.  Any warranties, whether express, statutory, implied or otherwise,
 # including but not limited to the implied warranties of merchantability or 
 # fitness for a particular purpose, are excluded.  Except as expressly provided
 # in any written license agreement from MIPS Technologies or an authorized third
 # party, the furnishing of this code does not give recipient any license to any
 # intellectual property rights, including any patent rights, that cover this code.
 # 
 # This code shall not be exported or transferred for the purpose of reexporting
 # in violation of any U.S. or non-U.S. regulation, treaty, Executive Order, law,
 # statute, amendment or supplement thereto.
 # 
 # This code may only be disclosed to the United States government ("Government"),
 # or to Government users, with prior written consent from MIPS Technologies or an
 # authorized third party.  This code constitutes one or more of the following:
 # commercial computer software, commercial computer software documentation or
 # other commercial items.  If the user of this code, or any related documentation
 # of any kind, including related technical data or manuals, is an agency,
 # department, or other entity of the Government, the use, duplication,
 # reproduction, release, modification, disclosure, or transfer of this code, or
 # any related documentation of any kind, is restricted in accordance with Federal
 # Acquisition Regulation 12.212 for civilian agencies and Defense Federal
 # Acquisition Regulation Supplement 227.7202 for military agencies.  The use of
 # this code by the Government is further restricted in accordance with the terms
 # of the license agreement(s) and/or applicable contract terms and conditions
 # covering this code from MIPS Technologies or an authorized third party.
 # *******************************End Copyright************************************


	.text
	.set		noreorder
	.set		nomacro
	.set		noat

	.global		mips_lms16
	.ent		mips_lms16

# int16 mips_lms16(int16 in, int16 ref, int16 *coeffs, int16 *delayline,
#		   int16 *error, int16 K, int mu)
# $a0 - in
# $a1 - ref
# $a2 - coeffs
# $a3 - delayline
# 16($sp) - error from the last iteration
# 20($sp) - K = 2*k >= 4
# 24($sp) - mu (adaptation rate)

# $t8 - &coeffs[K-1]
# $t9 - adjust factor = *error * mu

# $v0 - accumulator in Q7.24 format
# $t3, $t7 - temp multiplication results

# accumulator guard bits are required to prevent overflow:

#define GBITS	 6
#define RND     (1 << (GBITS-1))

# optional rounding of multiplication results:

#define RNDMUL


mips_lms16:
	lw	$at, 16($sp)		# error pointer
	lw	$t9, 24($sp)		# mu

	li	$v0, RND		# accumulator rounding value

	lh	$t0, 0($at)		# *error
	lw	$t8, 20($sp)		# K

	mul	$t9, $t9, $t0		# adj = mu * error

	addiu	$t8, $t8, -2		# K-2
	sll	$t8, $t8, 1		# (K-2) * sizeof(int16)
	addu	$t8, $t8, $a2		# &coeffs[K-2]

	addiu	$t9, $t9, 0x4000	# rounding
	sra	$t9, $t9, 15

loop:
	lh	$t0, 2($a3)		# x1 = delayline[i+1]
	lh	$t4, 4($a3)		# x2 = delayline[i+2]
	
	mul	$t2, $t0, $t9		# x1 * adj
	mul	$t6, $t4, $t9		# x2 * adj
	
	lh	$t1, 0($a2)		# c1 = coeffs[i]
	lh	$t5, 2($a2)		# c2 = coeffs[i+1]

	addiu	$a3, $a3, 4		# 2 * sizeof(int16)
	addiu	$a2, $a2, 4		# 2 * sizeof(int16)

	addiu	$t2, $t2, 0x4000	# rounding
	sra	$t2, $t2, 15
	addu	$t1, $t1, $t2		# c1 += x1 * adj

	mul	$t3, $t0, $t1		# x1 * c1

	addiu	$t6, $t6, 0x4000	# rounding
	sra	$t6, $t6, 15
	addu	$t5, $t5, $t6		# c2 += x2 * adj

	mul	$t7, $t4, $t5		# x2 * c2

	sh	$t0, -4($a3)		# delayline[i] = x1
	sh	$t4, -2($a3)		# delayline[i+1] = x2

#ifdef RNDMUL
	addiu	$t3, $t3, RND		# round
#endif
	sra	$t3, $t3, GBITS		# scale to avoid acc overflow
	addu	$v0, $v0, $t3		# a += x1 * c1

#ifdef RNDMUL
	addiu	$t7, $t7, RND		# round
#endif
	sra	$t7, $t7, GBITS		# scale to avoid acc overflow
	addu	$v0, $v0, $t7		# a += x2 * c2

	sh	$t1, -4($a2)		# coeffs[i] = c1

	bne	$a2, $t8, loop
	sh	$t5, -2($a2)		# coeffs[i+1] = c2

	# 4 stall cycles - mispredicted branch

	lh	$t0, 2($a3)		# x = delayline[K-1]
	
	mul	$t6, $a0, $t9		# in * adj
	mul	$t2, $t0, $t9		# x * adj
	
	lh	$t1, 0($a2)		# c1 = coeffs[K-2]
	lh	$t5, 2($a2)		# c2 = coeffs[K-1]

	addiu	$t8, $zero, 0x7FFF	# load upper limit 0x00007FFF
	addiu	$t9, $zero, 0x8000	# load lower limit 0xFFFF8000 (sign extended)

	addiu	$t6, $t6, 0x4000	# rounding
	sra	$t6, $t6, 15
	addu	$t5, $t5, $t6		# c2 += in * adj

	mul	$t3, $a0, $t5		# in * c2

	addiu	$t2, $t2, 0x4000	# rounding
	sra	$t2, $t2, 15
	addu	$t1, $t1, $t2		# c1 += x * adj

	mul	$t7, $t0, $t1		# x * c1

	sh	$t0, 0($a3)		# delayline[K-2] = x
	sh	$a0, 2($a3)		# delayline[K-1] = in

#ifdef RNDMUL
	addiu	$t3, $t3, RND		# round
#endif
	sra	$t3, $t3, GBITS		# scale to avoid acc overflow
	addu	$v0, $v0, $t3		# a += in * c2

#ifdef RNDMUL
	addiu	$t7, $t7, RND		# round
#endif
	sra	$t7, $t7, GBITS		# scale to avoid acc overflow
	addu	$v0, $v0, $t7		# a += x * c1

	sh	$t1, 0($a2)		# coeffs[K-2] = c1
	sh	$t5, 2($a2)		# coeffs[K-1] = c2


	sra	$v0, $v0, (15-GBITS)	# scale the result
	
	slt	$t2, $t8, $v0		# set $t2 if result larger than 0x7FFF
	movn	$v0, $t8, $t2		# positive clipping to 0x7FFF if $t2 set
	slt	$t2, $v0, $t9		# set $t2 if result smaller than 0xFFFF8000
	movn	$v0, $t9, $t2		# negative clipping to 0xFFFF8000 if $t2 set

	subu	$v1, $a1, $v0		# error = ref - result
	
	jr	$ra
	sh	$v1, 0($at)		# *error


	.end		mips_lms16

