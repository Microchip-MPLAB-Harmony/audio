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

	.global	mips_vec_add16
	.ent	mips_vec_add16

mips_vec_add16:

# void mips_vec_add16(int16 *outdata, int16 *indata1, int16 *indata2, int N)
# $a0 - outdata
# $a1 - indata1
# $a2 - indata2
# $a3 - N = 4*n >= 4

	andi	$a3, $a3, 0xFFFC	# make sure N is multiple of 4
	sll	$a3, $a3, 1		# N * sizeof(int16)
	addu	$a3, $a1, $a3		# final address in indata1
	
loop:
	lh	$t0, 0($a1)		# load indata1[i]
	lh	$t1, 2($a1)		# load indata1[i+1]
	lh	$t2, 4($a1)		# load indata1[i+2]
	lh	$t3, 6($a1)		# load indata1[i+3]

	lh	$t4, 0($a2)		# load indata2[i]
	lh	$t5, 2($a2)		# load indata2[i+1]
	lh	$t6, 4($a2)		# load indata2[i+2]
	lh	$t7, 6($a2)		# load indata2[i+3]

	addu	$t0, $t0, $t4		# outdata[i] = indata1[i] + indata2[i]
	addu	$t1, $t1, $t5		# outdata[i+1] = indata1[i+1] + indata2[i+1]
	addu	$t2, $t2, $t6		# outdata[i+2] = indata1[i+2] + indata2[i+2]
	addu	$t3, $t3, $t7		# outdata[i+3] = indata1[i+3] + indata2[i+3]

	sh	$t0, 0($a0)		# store outdata[i]
	sh	$t1, 2($a0)		# store outdata[i+1]
	sh	$t2, 4($a0)		# store outdata[i+2]
	sh	$t3, 6($a0)		# store outdata[i+3]

	addiu	$a1, $a1, 8		# 4 * sizeof(int16)
	addiu	$a2, $a2, 8		# 4 * sizeof(int16)
	bne	$a1, $a3, loop
	addiu	$a0, $a0, 8		# 4 * sizeof(int16)

	# 4 stall cycles - mispredicted branch

	jr	$ra
	ssnop

	.end	mips_vec_add16

