addi x31, x0, 2
slli x31, x31, 31
add x1, x0, x0
Loop:
addi x1, x1, 1

	addi x3, x0, 11
    bge x1, x3, Loop_MoreThan10
    addi x3, x0, 6
    bge x1, x3, Loop_Is6orMore
    addi x3, x0, 5
    beq x1, x3, Loop_Equals5
    addi x3, x0, 4
    beq x1, x3, Loop_Equals4
    addi x3, x0, 3

	Loop_LessThan3:
    addi x2, x0, 'L'
	sb x2, 0x1, x31
    jalr x0, x0, Loop
    
    Loop_Equals4:
    addi x2, x0, 'E'
    sb x2, 0x1, x31
    jalr x0, x0, Loop
    
    Loop_Equals5:
    addi x2, x0, '5'
    sb x2, 0x1, x31
    jalr x0, x0, Loop
    
    Loop_Is6orMore:
    addi x2, x0, 'G'
    sb x2, 0x1, x31
    jalr x0, x0, Loop

	Loop_MoreThan10:
    sb x0, 0x20, x31
