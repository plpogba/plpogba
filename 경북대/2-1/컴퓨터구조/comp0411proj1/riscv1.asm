# ------------------------------------------------------------------------------------
# [KNU COMP0411 Computer Architectures] Skeleton code for the 1st project (calculator)
# ------------------------------------------------------------------------------------
.data
myWord: .space 10
equal: .string "="
num: .space 2

.text
# main
main:
	li a7 11
	li a0 62 #print >
	ecall
	
	li a7 8
	li a1 10
	la a0 myWord #ReadString
	ecall
	addi x21 zero 10
	addi x22 zero 42
	addi x23 zero 48
	firstOp:
		addi a0 a0 1
		lb x24 0(a0)
		beq zero zero print
	
	secondOp:
		add a0 zero x24
		li a7 1
		ecall
	print:
		li a7 11
		add a0 zero x24
		ecall
	
		
	#jal x1, test #functionality test (please comment this line out if you want console test)

	#----TODO-------------------------------------------------------
	#1. read a string from the console
	#2. perform arithmetic operations
	#3. print a string to the console to show the computation result
	#---------------------------------------------------------------	
	# Exit (93) with code 0
        li a0, 0
	li a7, 93
	ecall
	ebreak
