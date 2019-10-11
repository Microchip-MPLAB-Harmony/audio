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

	.global	mips_vec_sum_squares16
	.ent	mips_vec_sum_squares16

mips_vec_sum_squares16:

# int16 mips_vec_sum_squares16(int16 *indata, int N, int scale)
# $a0 - indata
# $a1 - N = 4*n >= 4
# $a2 - scale

	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll	$a1, $a1, 1		# N * sizeof(int16)
	addu	$a1, $a0, $a1		# final address in indata

	addiu	$t0, $zero, 0x4000	# accumulator rounding value
	sllv	$t0, $t0, $a2		# prescale the rounding value
	mthi	$zero
	mtlo	$t0

loop:
	lh	$t0, 0($a0)		# load indata[i]
	lh	$t1, 2($a0)		# load indata[i+1]
	lh	$t2, 4($a0)		# load indata[i+2]
	lh	$t3, 6($a0)		# load indata[i+3]

	addiu	$a0, $a0, 8		# 4 * sizeof(int16)
	
	madd	$t0, $t0		# acc += indata[i]^2
	madd	$t1, $t1		# acc += indata[i+1]^2
	madd	$t2, $t2		# acc += indata[i+2]^2

	bne	$a0, $a1, loop
	madd	$t3, $t3		# acc += indata[i+3]^2

	# 4 stall cycles - mispredicted branch

	mflo	$v0			# get 32 accumulator LSBs
	mfhi	$v1			# get 32 accumulator MSBs

	addiu	$t3, $zero, 0x7FFF	# load upper limit 0x00007FFF
	addiu	$t4, $zero, 0x8000	# load lower limit 0xFFFF8000 (sign extended)
	
	addiu	$a3, $a2, 15		# scale += 15
	li	$a2, 32
	subu	$a2, $a2, $a3		# 32 - (scale + 15)
	
	srlv	$v0, $v0, $a3		# scale the 32 LSBs of the result
	sllv	$t2, $v1, $a2		# isolate MSBs to be combined with the LSBs
	or	$v0, $v0, $t2		# combined scaled 32-bit result
	
	slt	$t2, $t3, $v0		# set $t2 if result larger than 0x7FFF
	movn	$v0, $t3, $t2		# positive clipping to 0x7FFF if $t2 set
	slt	$t2, $v0, $t4		# set $t2 if result smaller than 0xFFFF8000

	jr	$ra
	movn	$v0, $t4, $t2		# negative clipping to 0xFFFF8000 if $t2 set

	.end	mips_vec_sum_squares16



