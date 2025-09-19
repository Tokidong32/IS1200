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
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################
	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string, delay, display_string

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
	li	a0, 2		# ms
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

time2string:	#a0 = addressen d�r str�ngen ska ligga( hh:mm[NUL] ) 6 tecken
		#a1 = inneh�ller 16 bitar tidsinfo. Form = xxxx xxxx xxxx xxxx hhhh hhhh mmmm mmmm. x=dontcare h = tim m = min
	PUSH s0
	PUSH s1
	PUSH s2
	PUSH s3
	addi s3, a0, 0 # sparar addressen till senare
	addi s2, a1, 0 # S2 = hela tidstalet.  
	addi s1, zero, 0 # S1 = 0 countern
	addi s0, zero, 4 
	
	PUSH ra
	readerLoop:
	bge s1,s0, readerLoopEnd
		andi a0, s2, 0xf # a0 inneh�ller fyra lsb
		jal hex2asc
		PUSH a0 #kommer var en byte ASCIIkod 
		srli s2, s2, 4 # shiftar bort de fyra lsb i tidinfon
		addi s1, s1, 0x01 # �kar countern med 1
		j readerLoop
	readerLoopEnd:
	
	# dags att bygga str�ngen mm:ss\0
	addi t0, zero, 0x3a
	POP a0
	sb a0, 0(s3)
	POP a0
	sb a0, 1(s3)
	sb t0, 2(s3)
	POP a0
	sb a0, 3(s3)
	POP a0
	sb a0, 4(s3)
	sb zero, 5(s3)
	
	POP ra
	
	POP s3
	POP s2
	POP s1
	POP s0
		
	jr ra
	
hex2asc: # a0 ska inneh�lla talet som ska konverteras return a0 �r sifran i ASCIIkod
	addi    sp,sp,-4	# inkremerar satcken  PUSH macrot
	sw      ra,0(sp)	# l�gger in addressen i sp till ra
	
	jal	hexasc		# call hexasc

	lw      ra,0(sp) 	#POP mackrot
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  hexasc:
	andi t0, a0, 0xF #t0 inneh�ller talet vi ska representera 
	addi t1, zero, 10 #t1 = 10 eller 0xA
	bge  t0,t1, latter #if a0 >= 10 
	
	addi a0, t0, 0x30 #l�gger till f�ljande bitar f�r att representera en siffra i ASCII
	jr ra
	latter:
	addi a0, t0, 0x37 #l�gger till f�ljande bitar f�r att representera en bokstav i ASCII
	jr ra

delay:	# f�rdr�j med en X antal ms
	# a0 ligger antalet X ms  a0 = ms
	# t0 = i
	# t1 = 4711 = 0x1267 "konstanten f�r att f�rdr�ja en ms"
	
		lui t1,0x3		# 
		ori t1, t1, 0x000 	# t1 = 4711
		#addi t1, zero, 700	# t1 = 2 g�r att min dator v�ntar en ca en secund per delay call
	while:
		bltz a0, whileEnd	# while(ms > 0)
		addi a0, a0, -1		# ms = ms - 1
		addi t0, zero, 0	# int i = 0
		for:
			bge t0, t1, while # for( i = 0; i < 4711; i = i + 1)
			addi t0, t0, 1
			j for
	whileEnd:
	
	jr ra
