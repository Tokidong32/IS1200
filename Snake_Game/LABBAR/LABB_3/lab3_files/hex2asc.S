  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 15		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
hexasc:	addi  t0,zero, 10
	bge   a0,t0, af
	addi  a0, a0, 0x30
	addi  t0,zero, 0x7F  
	and   a0, a0, t0 #ensures that every bit above seven from lsb is zero
	jr ra
af:	addi  a0,a0,0x37
	addi  t0, zero, 0x7F 
	and   a0, a0, t0 #ensures that every bit above seven from lsb is zero
	jr ra
