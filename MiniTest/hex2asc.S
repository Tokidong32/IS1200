  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 4		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
  hexasc:
	andi t0, a0, 0xF #t0 innehåller talet vi ska representera 
	addi t1, zero, 10 #t1 = 10 eller 0xA
	bge  t0,t1, latter #if a0 >= 10 
	
	addi a0, t0, 0x30 #lägger till följande bitar för att representera en siffra i ASCII
	jr ra
	latter:
	addi a0, t0, 0x37 #lägger till följande bitar för att representera en bokstav i ASCII
	jr ra