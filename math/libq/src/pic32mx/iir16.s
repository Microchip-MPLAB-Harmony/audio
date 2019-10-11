
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

	.global		mips_iir16
	.ent		mips_iir16

mips_iir16:

# int16 mips_iir16(int16 in, int16 *coeffs, int16 *delayline, int B, int scale)
# $a0 - in
# $a1 - coeffs
# $a2 - delayline
# $a3 - B = 2*b >= 2
# 16($sp) - scale


	lw		$t8, 16($sp)		# S
	andi		$a3, $a3, 0xFFFE	# make sure B is multiple of 2
	sll		$a3, $a3, 3		# B * (4*sizeof(int16))
	addu		$a3, $a1, $a3		# final address in coeffs

	move		$v0, $a0		# y = x
	
	.align	2

loop:
	shrav.ph	$v1, $v0, $t8		# x = y >> S
	lwp		$t0, 0($a2)		# d11|d21, d12|d22
	ins		$v1, $a0, 16, 16	# x|(x>>S)
	srl		$t9, $t0, 16		# 0|d11
	addq_s.ph	$v1, $v1, $t9		# x1|x2 = x|(x>>S) + 0|d1
	shrav.ph	$v1, $v1, $t8		# x1|x2 >>= S

	lwp		$t2, 0($a1)		# a11|a21, a12|a22
	
	addq_s.ph	$v0, $v1, $t0		# y1|y2 = x1|x2 + d11|d21

	lwp		$t4, 8($a1)		# b11|b21, b12|b22

	mulq_rs.ph	$t2, $t2, $v0		# a11|a21 * y1|y2
	mulq_rs.ph	$t4, $t4, $v1		# b11|b21 * x1|x2

	addiu		$a1, $a1, 16		# 2 * (4*sizeof(int16))

	mulq_rs.ph	$t3, $t3, $v0		# a12|a22 * y1|y2
	mulq_rs.ph	$t5, $t5, $v1		# b12|b22 * x1|x2

	addq_s.ph	$t0, $t2, $t4		# d11|d21

	addq_s.ph	$t0, $t0, $t1		# d11|d21 += d12|d22
	addq_s.ph	$t1, $t3, $t5		# d12|d22

	swp		$t0, 0($a2)		# d11|d21, d12|d22

	bne		$a1, $a3, loop
	addiu		$a2, $a2, 8		# 2 * (2*sizeof(int16))


	jr		$ra
	seh		$v0, $v0


	.end		mips_iir16
