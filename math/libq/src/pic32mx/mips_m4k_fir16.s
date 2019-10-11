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
	.set	noreorder
	.set	nomacro

	.global	mips_fir16
	.ent	mips_fir16

mips_fir16:

# void mips_fir16(int16 *outdata, int16 *indata, int16 *coeffs2x, int16 *delayline,
#				int N, int K, int scale)
# $a0 - outdata
# $a1 - indata
# $a2 - coeffs2x, &coeffs2x[K-dlp]
# $a3 - delayline
# 16($sp) - N
# 20($sp) - K = 4*k >= 4
# 24($sp) - scale
#
# $t4 - 0x00007FFF
# $t5 - 0xFFFF8000
# $t6 - 15 + scale
# $t7 - 32 - (15 + scale)
# $t8 - dlp * sizeof(int16)
# $t9 - &delayline[dlp]
#
# $s0 - 0x4000
# $s1 - &outdata[N]
# $s2 - K * sizeof(int16)
# $s3 - &coeffs2x[K]
#
# $v0 - &delayline[K]


	addiu	$sp, $sp, -16		# reserve stack space for s0-s3

	sw	$s0, 12($sp)
	sw	$s1,  8($sp)
	sw	$s2,  4($sp)
	sw	$s3,  0($sp)

	lhu	$t8, 0($a3)		# dlp = delayline[0]

	addiu	$t4, $zero, 0x7FFF	# load upper limit 0x00007FFF
	addiu	$t5, $zero, 0x8000	# load lower limit 0xFFFF8000 (sign extended)

	sll	$t8, $t8, 1		# dlp * sizeof(int16)
	addu	$t9, $a3, $t8		# address of delayline[dlp]

	lw	$s1, 16+16($sp)		# load N
	lw	$s2, 16+20($sp)		# load K
	lw	$t6, 16+24($sp)		# load scale
	
	li	$s0, 0x4000		# accumulator rounding value
	sllv	$s0, $s0, $t6		# rounding value << scale

	lh	$t0, 0($t9)		# delayline[dlp]

	sll	$s1, $s1, 1		# N * sizeof(int16)
	addu	$s1, $a0, $s1		# &outdata[N]

	sll	$s2, $s2, 1		# K * sizeof(int16)
	addu	$s3, $a2, $s2		# &coeffs2x[K]
	addu	$v0, $a3, $s2		# &delayline[K]

	sh	$t0, 0($a3)		# delayline[0] = delayline[dlp]

	li	$t7, 32
	addiu	$t6, $t6, 15		# 15 + scale
	subu	$t7, $t7, $t6		# 32 - (15 + scale)
	
loopN:
	mtlo	$s0			# accumulator rounding value
	mthi	$zero

	lh	$t0, 0($a1)		# load indata[i]
	addiu	$a1, $a1, 2		# sizeof(int16)
	sh	$t0, 0($t9)		# delayline[dlp] = indata[i]

	subu	$a2, $s3, $t8		# &coeffs2x[K-dlp]

loopK:
	lh	$t0, 0($a3)		# load delayline[j]
	lh	$t1, 2($a3)		# load delayline[j+1]

	lh	$t2, 0($a2)		# load coeffs2x[k]
	lh	$t3, 2($a2)		# load coeffs2x[k+1]

	madd	$t0, $t2		# acc += delayline[j] * coeffs2x[k]
	madd	$t1, $t3		# acc += delayline[j+1] * coeffs2x[k+1]

	lh	$t0, 4($a3)		# load delayline[j+2]
	lh	$t1, 6($a3)		# load delayline[j+3]
	
	lh	$t2, 4($a2)		# load coeffs2x[k+2]
	lh	$t3, 6($a2)		# load coeffs2x[k+3]

	madd	$t0, $t2		# acc += delayline[j+2] * coeffs2x[k+2]
	madd	$t1, $t3		# acc += delayline[j+3] * coeffs2x[k+3]

	addiu	$a3, $a3, 8		# 4 * sizeof(int16)

	bne	$a3, $v0, loopK
	addiu	$a2, $a2, 8		# 4 * sizeof(int16)

	# 4 stall cycles - mispredicted branch

	mflo	$t0			# get 32 accumulator LSBs
	mfhi	$t1			# get 32 accumulator MSBs
	
	subu	$a3, $a3, $s2		# restore delay line pointer
	movz	$t8, $s2, $t8		# if (dlp == 0) dlp = K
	addiu	$t8, $t8, -2		# decrement dlp

	srlv	$t0, $t0, $t6		# scale the 32 LSBs of the result
	sllv	$t2, $t1, $t7		# isolate MSBs to be combined with the LSBs
	or	$t0, $t0, $t2		# combined scaled 32-bit result
	
	slt	$t2, $t4, $t0		# set $t2 if result larger than 0x7FFF
	movn	$t0, $t4, $t2		# positive clipping to 0x7FFF if $t2 set
	slt	$t2, $t0, $t5		# set $t2 if result smaller than 0xFFFF8000
	movn	$t0, $t5, $t2		# negative clipping to 0xFFFF8000 if $t2 set

	sh	$t0, 0($a0)		# store outdata[i]
	addiu	$a0, $a0, 2		# sizeof(int16)

	bne	$a0, $s1, loopN
	addu	$t9, $a3, $t8		# address of delayline[dlp]

	# 4 stall cycles - mispredicted branch

	lh	$t0, 0($a3)		# delayline[0]
	srl	$t8, $t8, 1		# dlp / sizeof(int16)
	sh	$t0, 0($t9)		# delayline[dlp] = delayline[0]
	sh	$t8, 0($a3)		# delayline[0] = dlp

	lw	$s0, 12($sp)
	lw	$s1,  8($sp)
	lw	$s2,  4($sp)
	lw	$s3,  0($sp)

	jr	$ra
	addiu	$sp, $sp, 16

	.end	mips_fir16



