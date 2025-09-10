# main.S
# Written by W Szczerek
# Copyright abandonded - this file is in the public domain.

######################################################################################
# This file is only a "hub" for your code to run in. You can reference the functions #
# that you wrote in the other files here.                                            #
######################################################################################

.text
.globl main, handle_interrupt


	
# You can select which one of the assignments to run by uncommenting the apporpriate
# call.
main:
    # Uncomment below for assignment 1
    #jal analyze
	#li a0, 10
	#li a7,11
	#ecall
    # Uncomment below for assignment 2
    #jal hex2asc
	#li a0, 10
	#li a7,11
	#ecall
    #U Uncomment below for assignment 3 & 6
    jal timetemplate
loop: j loop

# Below is unused in lab1
handle_interrupt:
    jr ra
	
