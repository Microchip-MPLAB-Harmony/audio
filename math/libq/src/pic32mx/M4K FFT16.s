#
#
# PROPRIETARY / SECRET CONFIDENTIAL INFORMATION OF MIPS TECHNOLOGIES,
# INC. FOR EVALUATION ONLY. 
#
# Unpublished work (c) MIPS Technologies, Inc. �All rights reserved.
# Unpublished rights reserved under the copyright laws of the United
# States of America and other countries.
# 
# This code is confidential and proprietary to MIPS Technologies,
# Inc. ("MIPS Technologies") and may be disclosed only as permitted
# in writing by MIPS Technologies or an authorized third party. �Any
# copying, reproducing, modifying, use or disclosure of this code (in
# whole or in part) that is not expressly permitted in writing by
# MIPS Technologies or an authorized third party is strictly
# prohibited. �At a minimum, this code is protected under trade
# secret, unfair competition, and copyright laws. �Violations thereof
# may result in criminal penalties and fines.
# 
# MIPS Technologies reserves the right to change this code to improve
# function, design or otherwise. �MIPS Technologies does not assume
# any liability arising out of the application or use of this code,
# or of any error or omission in such code. �Any warranties, whether
# express, statutory, implied or otherwise, including but not limited
# to the implied warranties of merchantability or fitness for a
# particular purpose, are excluded. �Except as expressly provided in
# any written license agreement from MIPS Technologies or an
# authorized third party, the furnishing of this code does not give
# recipient any license to any intellectual property rights,
# including any patent rights, that cover this code.
# 
# This code shall not be exported or transferred for the purpose of
# reexporting in violation of any U.S. or non-U.S. regulation,
# treaty, Executive Order, law, statute, amendment or supplement
# thereto.
# 
# This code may only be disclosed to the United States government
# ("Government"), or to Government users, with prior written consent
# from MIPS Technologies or an authorized third party. �This code
# constitutes one or more of the following: commercial computer
# software, commercial computer software documentation or other
# commercial items. �If the user of this code, or any related
# documentation of any kind, including related technical data or
# manuals, is an agency, department, or other entity of the
# Government, the use, duplication, reproduction, release,
# modification, disclosure, or transfer of this code, or any related
# documentation of any kind, is restricted in accordance with Federal
# Acquisition Regulation 12.212 for civilian agencies and Defense
# Federal Acquisition Regulation Supplement 227.7202 for military
# agencies. �The use of this code by the Government is further
# restricted in accordance with the terms of the license agreement(s)
# and/or applicable contract terms and conditions covering this code
# from MIPS Technologies or an authorized third party.
#
#

		.text
		.set	noreorder
		.set	noat

		.global	mips_fft16
		.ent	mips_fft16

# fft16: 16-bit FFT function tuned for MIPS M4K processor cores.
# All input/output parameters are equivalent to the ones used by the
# mips_fft16() function from the MIPS DSP Library.
#
# void mips_fft16(int16c *dout, int16c *din, const int16c *twiddles,
#				  int16c *scratch, int log2N);

mips_fft16:
		addiu	$sp,$sp,-32
		sw		$s0,28($sp)
		sw		$s1,24($sp)
		sw		$s2,20($sp)
		sw		$s3,16($sp)
		sw		$s4,12($sp)
		sw		$s5, 8($sp)
		sw		$s6, 4($sp)
		sw		$s7, 0($sp)

		lw		$v0,48($sp)
		move	$at,$a0
		andi	$t1,$v0,0x1
		movz	$a0,$a3,$t1
		movz	$a3,$at,$t1

		li		$v1,2
		sllv	$v1,$v1,$v0
		addu	$s0,$a1,$v1
		addu	$s1,$a0,$v1
		move	$s2,$s0

# First stage loop - no multiplications, unrolled 2x

loopS1:
		lh		$t2,0($s0)
		lh		$t3,2($s0)
		lh		$t8,0($a1)
		lh		$t9,2($a1)

		subu	$t4,$t8,$t2
		subu	$t5,$t9,$t3
		sra		$t4,$t4,0x1
		sra		$t5,$t5,0x1
		addu	$t8,$t8,$t2
		addu	$t9,$t9,$t3
		sra		$t8,$t8,0x1
		sra		$t9,$t9,0x1

		sh		$t4,0($s1)
		sh		$t5,2($s1)
		sh		$t8,0($a0)
		sh		$t9,2($a0)

########################################

		lh		$t2,4($s0)
		lh		$t3,6($s0)
		lh		$t8,4($a1)
		lh		$t9,6($a1)

		subu	$t4,$t8,$t2
		subu	$t5,$t9,$t3
		sra		$t4,$t4,0x1
		sra		$t5,$t5,0x1
		addu	$t8,$t8,$t2
		addu	$t9,$t9,$t3
		sra		$t8,$t8,0x1
		sra		$t9,$t9,0x1

		sh		$t4,4($s1)
		sh		$t5,6($s1)
		sh		$t8,4($a0)
		sh		$t9,6($a0)

		addiu	$s0,$s0,8
		addiu	$a1,$a1,8

		addiu	$a0,$a0,8
		bne		$a1,$s2,loopS1
		addiu	$s1,$s1,8
		
		move	$a1,$v1
		li		$v0,4
		srl		$v1,$v1,0x1
		subu	$a0,$a0,$a1

		li		$t4, 0x7FFFFFFF

# Loop for stages 2 to N-1
		
loopS:
		move	$at,$a2
		sll		$s3,$a1,0x1
		addu	$s3,$a0,$s3

# Group loop - load new coefficients for each group

loopGR:
		addu	$s0,$a0,$v1
		addu	$s1,$a3,$a1
		move	$s2,$s0

		lh		$t0,0($at)
		lh		$t1,2($at)

		sll		$t0,$t0,16
		sll		$t1,$t1,16

# Buttefly loop - all butterflies in a group share the
# same coefficients, unrolled 2x.

loopBF:
		lh		$t2,0($s0)
		lh		$t3,2($s0)
		lh		$t8,0($a0)
		lh		$t9,2($a0)

		mult	$t0,$t2
		msub	$t1,$t3
		madd	$t4,$t8
		mfhi	$t6

		mult	$t1,$t2
		madd	$t0,$t3
		madd	$t4,$t9
		mfhi	$t7

		subu	$s4,$t8,$t6
		subu	$s5,$t9,$t7

		sh		$s4,0($s1)
		sh		$s5,2($s1)
		sh		$t6,0($a3)
		sh		$t7,2($a3)

########################################
	
		lh		$t2,4($s0)
		lh		$t3,6($s0)
		lh		$t8,4($a0)
		lh		$t9,6($a0)

		mult	$t0,$t2
		msub	$t1,$t3
		madd	$t4,$t8
		mfhi	$t6

		mult	$t1,$t2
		madd	$t0,$t3
		madd	$t4,$t9
		mfhi	$t7

		subu	$s4,$t8,$t6
		subu	$s5,$t9,$t7

		sh		$s4,4($s1)
		sh		$s5,6($s1)
		sh		$t6,4($a3)
		sh		$t7,6($a3)

		addiu	$s0,$s0,8
		addiu	$a0,$a0,8

		addiu	$a3,$a3,8
		bne		$a0,$s2,loopBF
		addiu	$s1,$s1,8

		addu	$a0,$a0,$v1
		bne		$a0,$s3,loopGR
		addu	$at,$at,$v1

		srl		$v1,$v1,0x1
		subu	$a0,$a3,$a1
		subu	$a3,$s0,$a1
		bne		$v1,$v0,loopS
		subu	$a3,$a3,$a1

		move	$at,$a2
		sll		$s3,$a1,0x1
		addu	$s3,$a0,$s3
		addu	$s1,$a3,$a1

# Last stage loop - new coefficients for each butterfly,
# unrolled 2x.

loopSL2N:
		lh		$t0,0($at)
		lh		$t1,2($at)

		lh		$t8,0($a0)
		lh		$t9,2($a0)
		lh		$t2,4($a0)
		lh		$t3,6($a0)

		sll		$t0,$t0,16
		sll		$t1,$t1,16

		mult	$t0,$t2
		msub	$t1,$t3
		madd	$t4,$t8
		mfhi	$t6

		mult	$t1,$t2
		madd	$t0,$t3
		madd	$t4,$t9
		mfhi	$t7

		subu	$s4,$t8,$t6
		subu	$s5,$t9,$t7

		sh		$s4,0($s1)
		sh		$s5,2($s1)
		sh		$t6,0($a3)
		sh		$t7,2($a3)

########################################

		lh		$t0,4($at)
		lh		$t1,6($at)

		lh		$t8, 8($a0)
		lh		$t9,10($a0)
		lh		$t2,12($a0)
		lh		$t3,14($a0)

		sll		$t0,$t0,16
		sll		$t1,$t1,16

		mult	$t0,$t2
		msub	$t1,$t3
		madd	$t4,$t8
		mfhi	$t6

		mult	$t1,$t2
		madd	$t0,$t3
		madd	$t4,$t9
		mfhi	$t7

		subu	$s4,$t8,$t6
		subu	$s5,$t9,$t7

		sh		$s4,4($s1)
		sh		$s5,6($s1)
		sh		$t6,4($a3)
		sh		$t7,6($a3)

		addiu	$at,$at,8
		addiu	$a0,$a0,16

		addiu	$a3,$a3,8
		bne		$a0,$s3,loopSL2N
		addiu	$s1,$s1,8

		lw		$s0,28($sp)
		lw		$s1,24($sp)
		lw		$s2,20($sp)
		lw		$s3,16($sp)
		lw		$s4,12($sp)
		lw		$s5, 8($sp)
		lw		$s6, 4($sp)
		lw		$s7, 0($sp)

		jr		$ra
		addiu	$sp,$sp,32

		.end	mips_fft16

