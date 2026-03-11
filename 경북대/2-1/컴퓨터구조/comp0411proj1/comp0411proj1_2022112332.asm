# ------------------------------------------------------------------------------------
# [KNU COMP0411 Computer Architectures] Skeleton code for the 1st project (calculator)
# ------------------------------------------------------------------------------------
.data
myWord: .space 10
exitString: .string "press Q to exit, C to continue " 
char: .space 5
change: .string "    "
.text
# main
main:	
	jal x1, test #functionality test (please comment this line out if you want console test)
	
	addi x11 zero 0
	addi x12 zero 0
	addi x13 zero 0
	addi x24 zero 0
	addi x17 zero 0
	addi x28 zero 0
	addi x29 zero 0
	addi x30 zero 0
	addi x31 zero 0
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
	addi x25 zero 255
	addi x13 x13 10 #*10
	#x28 sum , x24 tmp
	firstOp:
		lb x24 0(a0)
		and x24 x24 x25
		blt x24 x23 operator
		addi x12 x28 0 #operand1 = sum
		addi sp sp -24
		sw x21 0(sp)
		sw x22 4(sp)
		sw x23 8(sp)
		sw x25 12(sp)
		sw a0 16(sp)
		sw x24 20(sp)
		addi x11 zero 2
		jal x1 calc
		addi x28 x10 0
		lw x24 20(sp)
		lw a0 16(sp)
		lw x25 12 (sp)
		lw x23 8 (sp)
		lw x22 4 (sp)
		lw x21 0(sp)
		addi sp sp 24
		addi x24 x24 -48
		add x28 x28 x24
		addi a0 a0 1
		beq zero zero firstOp
	operator:
		
		and x24 x24 x25
		addi sp sp -8
		sw x28 0(sp) #store firstOp
		sw x24 4(sp) #store operator
		addi a0 a0 1
		addi x12 zero 0
		addi x28 zero 0
		
	secondOp:
		lb x24 0(a0)
		#and x24 x24 x25
		beq x24 x13 print
		addi x12 x28 0 #operand1 = sum
		addi sp sp -24
		sw x21 0(sp)
		sw x22 4(sp)
		sw x23 8(sp)
		sw x25 12(sp)
		sw a0 16(sp)
		sw x24 20(sp)
		addi x11 zero 2
		jal x1 calc
		addi x28 x10 0
		lw x24 20(sp)
		lw a0 16(sp)
		lw x25 12 (sp)
		lw x23 8 (sp)
		lw x22 4 (sp)
		lw x21 0(sp)
		addi sp sp 24
		addi x24 x24 -48
		add x28 x28 x24
		addi a0 a0 1
		beq zero zero secondOp
		
	print:
		li a7 11
	
		la a0 myWord
		word:
			lb x24 0(a0)
			and x24 x24 x25
			beq x24 x13 calculate
			addi sp sp -4
			sw a0 0(sp)
			addi a0 x24 0
			ecall
			lw a0 0(sp)
			addi sp sp 4
			addi a0 a0 1
			beq zero zero word
			
		calculate:	
		addi x13 x28 0
		lw x11 4(sp)
		lw x12 0(sp)
		addi sp sp 8
		addi x21 zero 43#+
		addi x22 zero 45#-
		addi x23 zero 42#*
		addi x25 zero 47#/
		beq x11 x21 addit
		beq x11 x22 subtr
		beq x11 x23 mult
		beq x11 x25 divi
		addit:
		addi x11 zero 0
		beq zero zero nextstep
		subtr:
		addi x11 zero 1
		beq zero zero nextstep
		mult:
		addi x11 zero 2
		beq zero zero nextstep
		divi:
		addi x11 zero 3
		beq zero zero printdiv
		
		
		nextstep:
		jal x1 calc
		addi sp sp -4
		sw x10 0(sp)
		li a7 11
		addi a0 zero 61
		ecall
		li a7 1
		lw x24 0(sp)
		addi x10 x24 0
		addi sp sp 4
		ecall
		beq zero zero repeat
		printdiv:
		jal x1 calc
		addi sp sp -4
		sw x10 0(sp)
		li a7 11
		addi a0 zero 61
		ecall
		li a7 1
		lb x24 0(sp)
		addi x10 x24 0
		addi sp sp 4
		ecall
		li a7 11
		addi a0 zero 46
		ecall
		ecall
		ecall
		li a7 1
		add a0 zero x14
		ecall
		
	repeat:
	li a7 11
	addi a0 zero 10
	ecall
	li a7 4
	la a0 exitString
	ecall
	li a7 8
	la a0 char
	addi a1 zero 5
	ecall
	lb x24 0(a0)
	addi x21 zero 81
	addi x22 zero 67
	beq x24 x21 end
	beq x24 x22 main
	

	#----TODO-------------------------------------------------------
	#1. read a string from the console
	#2. perform arithmetic operations
	#3. print a string to the console to show the computation result
	#---------------------------------------------------------------	
	# Exit (93) with code 0
	end:

        li a0, 0
	li a7, 93
	ecall
	ebreak
#---------------------------------------------------------------------------------------------
#name: calc
#func: performs arithmetic operation
#x11(a1): arithmetic operation (0: addition, 1:  subtraction, 2:  multiplication, 3: division)
#x12(a2): the first operand
#x13(a3): the second operand
#x10(a0): return value
#x14(a4): return value (remainder for division operation)
#---------------------------------------------------------------------------------------------

calc:
	#TODO
	addi x24 zero 1
	addi x31 zero 2
	addi x30 zero 3
	beq x11 zero addition
	beq x11 x24 subtraction
	beq x11 x31 multiplication
	beq x11 x30 division
	
	addition: 
		add x10 x12 x13
		beq x0 x0 EXIT
	subtraction:
		addi x23 zero -1
		xor x13 x23 x13
		addi x13 x13 1
		add x10 x12 x13 #result
		xor x13 x23 x13 #negating second operand again
		addi x13 x13 1
		beq x0 x0 EXIT
		
	multiplication:
		add x21 x12 zero
		add x22 x13 zero
		add x10 zero zero
		LOOPm: 	
			and x23 x22 x24 #x24 == 1, find most right digit
			bne x23 x24 skip
			add x10 x10 x21
			skip:
			slli x21 x21 1
			srli x22 x22 1
			beq x22 zero EXIT
			beq x0 x0 LOOPm
	division:
		addi x22 x13 0 #divisor
		addi x30 zero 1
		slli x30 x30 31 
		add x14 zero zero #set remainder
		addi x23 zero -1 #x23 = 1111..
		add x10 x12 zero#quotient
		addi x29 zero 0#count
		addi x24 zero 33
		xor x22 x23 x22
		addi x22 x22 1 #x22=-divisor
		LOOPd:
			beq zero zero subt
			next:
			addi x29 x29 1
			blt x29 x24 LOOPd
			srli x14 x14 1
			beq zero zero EXIT
		subt:
			add x14 x14 x22		
			bge x14 zero big
			add x14 x14 x13
			and x28 x10 x30
			slli x14 x14 1 
			slli x10 x10 1
			beq x28 zero next
			addi x14 x14 1
			beq zero zero next
			big:
				and x28 x10 x30
				slli x14 x14 1 
				slli x10 x10 1
				addi x10 x10 1
				beq x28 zero next
				addi x14 x14 1
				beq zero zero next
		
	EXIT:
	
	jalr x0, 0(x1)


.include "common.asm"
