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

	.global	mips_vec_sum_squares32
	.ent	mips_vec_sum_squares32

mips_vec_sum_squares32:

# int32 mips_vec_sum_squares32(int32 *indata, int N, int scale)
# $a0 - indata
# $a1 - N = 4*n >= 4
# $a2 - scale

	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll	$a1, $a1, 2		# N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata

	lui	$t0, 0x4000		# accumulator rounding value
	li	$t1, 32
	subu	$t1, $t1, $a2		# 32 - scale
	srlv	$t1, $t0, $t1		# rounding >> (32 - scale)
	sllv	$t0, $t0, $a2		# rounding << scale
	mthi	$t1
	mtlo	$t0

loop:
	lw	$t0,  0($a0)		# load indata[i]
	lw	$t1,  4($a0)		# load indata[i+1]
	lw	$t2,  8($a0)		# load indata[i+2]
	lw	$t3, 12($a0)		# load indata[i+3]

	addiu	$a0, $a0, 16		# 4 * sizeof(int32)
	
	madd	$t0, $t0		# acc += indata[i]^2
	madd	$t1, $t1		# acc += indata[i+1]^2
	madd	$t2, $t2		# acc += indata[i+2]^2

	bne	$a0, $a1, loop
	madd	$t3, $t3		# acc += indata[i+3]^2

	# 4 stall cycles - mispredicted branch

	mflo	$v0			# get 32 accumulator LSBs
	mfhi	$v1			# get 32 accumulator MSBs

	sll	$v1, $v1, 1		# shift left the MSBs
	srl	$v0, $v0, 31		# isolate most-sginificant bit from the LSBs
	or	$v0, $v0, $v1		# combined result
	
	jr	$ra
	srav	$v0, $v0, $a2		# final scaled result

	.end	mips_vec_sum_squares32



