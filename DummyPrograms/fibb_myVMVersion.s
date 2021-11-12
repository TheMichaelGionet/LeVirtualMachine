Main:
    # sp = 0x70000000
    lui x2, 0x70000
    
    addi x31, x0, 0x2
    slli x31, x31, 31
    
    # stack frame:
    # uint64_t n
    # 
    addi x2, x2, 8
	
    #for n = 0; n < 100; n++
    addi x5, x0, 0
    jal x0, Main_For_TestCond
	Main_For_Code:

	    sd x5, -8, x2
        
        add x12, x5, x0
        jal x1, Proc_Fibb
        
        #print Fibb(n)

		sd x10, 0x2, x31

        addi x11, x0, '\n'
        sb x11, 0x0, x31
        
        ld x5, -8, x2

    Main_For_Iteration:

        addi x5, x5, 1

    Main_For_TestCond:
    
        addi x6, x0, 100
        bltu x5, x6, Main_For_Code
    
    Main_For_End:
    
    
Main_Return:
sb x0, 0x20, x31




Proc_Fibb: # uint64_t n : x12, uint64_t sp : x2, uint64_t ra : x1
    # stack frame:
    # uint64_t RA;
    # uint64_t n;
    # uint64_t FibbResult1;
    addi x2, x2, 32
    sd x1, -8, x2
    
    
    addi x5, x0, 2
    bltu x12, x5, Proc_Fibb_Else
    Proc_Fibb_If: #if n > 1
        # FibbResult1 = Fibb( n-1 );
        sd x12, -16, x2
        addi x12, x12, -1
        jalr x1, x0, Proc_Fibb
        sd x10, -24, x2
        
        # return FibbResult1 + Fibb(n-2);
        ld x12, -16, x2
        addi x12, x12, -2
        jalr x1, x0, Proc_Fibb
        
        ld x5, -24, x2
        
        add x10, x5, x10
        jal x0, Proc_Fibb_Return
        
    Proc_Fibb_Else:
        addi x10, x0, 1
    Proc_Fibb_EndIf:
    
Proc_Fibb_Return:
ld x1, -8, x2
addi x2, x2, -32
jalr x0, x1, 0

