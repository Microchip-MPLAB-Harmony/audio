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

	.global	mips_vec_abs16
	.ent	mips_vec_abs16

mips_vec_abs16:

# void mips_vec_abs16(int16 *outdata, int16 *indata, int N)
# $a0 - outdata
# $a1 - indata
# $a2 - N = 4*n >= 4

	andi	$a2, $a2, 0xFFFC	# make sure N is multiple of 4
	sll	$a2, $a2, 1		# N * sizeof(int16)
	addu	$a2, $a1, $a2		# final address in indata
	
	li	$t5, 32767		# max 16-bit value used for saturation

loop:
	lh	$t0, 0($a1)		# load data
	lh	$t1, 2($a1)
	lh	$t2, 4($a1)
	lh	$t3, 6($a1)

	sra	$t4, $t0, 31		# 32 sign bits
	xor	$t0, $t0, $t4		# invert the bits of negative numbers
	subu	$t0, $t0, $t4		# complete the negation of negative numbers
	slt	$t6, $t5, $t0		# test if result larger than max
	movn	$t0, $t5, $t6		# clip the result to max value
	sh	$t0, 0($a0)		# store final result

	sra	$t4, $t1, 31
	xor	$t1, $t1, $t4
	subu	$t1, $t1, $t4
	slt	$t6, $t5, $t1
	movn	$t1, $t5, $t6
	sh	$t1, 2($a0)

	sra	$t4, $t2, 31
	xor	$t2, $t2, $t4
	subu	$t2, $t2, $t4
	slt	$t6, $t5, $t2
	movn	$t2, $t5, $t6
	sh	$t2, 4($a0)

	sra	$t4, $t3, 31
	xor	$t3, $t3, $t4
	subu	$t3, $t3, $t4
	slt	$t6, $t5, $t3
	movn	$t3, $t5, $t6
	sh	$t3, 6($a0)

	addiu	$a1, $a1, 8		# 4 * sizeof(int16)
	bne	$a1, $a2, loop
	addiu	$a0, $a0, 8		# 4 * sizeof(int16)

	# 4 stall cycles - mispredicted branch

	jr	$ra
	ssnop

	.end	mips_vec_abs16

