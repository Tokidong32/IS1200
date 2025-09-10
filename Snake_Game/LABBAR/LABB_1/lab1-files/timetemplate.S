# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
.macro	PUSH (%reg)
	addi	sp,sp,-4
	sw	%reg,0(sp) 
.end_macro

.macro	POP (%reg)
	lw	%reg,0(sp)
	addi	sp,sp,4
.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp) 
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

hexasc:	addi  t0,zero, 10
	andi  a0, a0, 0xf
	bge   a0,t0, af
	addi  a0, a0, 0x30
	jr ra
	
af:	addi  a0,a0,0x37
	jr ra


delay:	
	li t0, 1315  #for loop delay constant
	whiledelay:
		   ble a0, zero, delaydone
		   addi a0, a0, -1
		   addi t1, zero, 0
		   fordelay: bge t1,t0, whiledelay
			     addi t1, t1, 1
			     j fordelay 
	delaydone:
		  jr ra
		  
printHour: 
	POP(ra)
	addi t0, zero, 0x48 #H
	sb t0, 0(a0)
	
	addi t0, zero, 0x4f #O
	sb t0, 1(a0)
	
	addi t0, zero, 0x55 #U 
	sb t0, 2(a0)
	
	addi t0, zero, 0x52 #R
	sb t0, 3(a0)
	
	sb zero,4(a0)
    jr ra
    
time2string:
	PUSH(s0)
	PUSH(s1)
	#a1 stores mytime
	mv s0,a0  #save adress
	addi s1,zero, 4 #COUNTER
	
	PUSH(ra)
	li t1, 0xffff
	and a1, a1, t1
	beqz a1, printHour
	
	whilehex: beq s1,zero, addtomem 
		  andi t0, a1, 0xf #Get first 4 bites
		  mv a0, t0   	

		  jal hexasc

		  PUSH(a0)
		  srai a1, a1, 4 #shift for 4 next bites
		  addi s1, s1,-1
		  j whilehex
	addtomem:
	
	POP(a0)
	
	sb a0, 0(s0)
	POP(a0)
	sb a0, 1(s0)	
	
	addi t0, zero, 0x3A
	sb t0, 2(s0)
	
	POP(a0)
	sb a0, 3(s0)
	POP(a0)
	sb a0, 4(s0)
	
	sb zero,5(s0)
	
	POP(ra)
	POP(s1)	
	POP(s0)

	jr ra	
	

