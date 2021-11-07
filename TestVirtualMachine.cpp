#define Mode_Release 1
#define Mode_Debug 2

#define MODE Mode_Debug

#include <stdio.h>
#include <memory.h>

#include "VirtualMachine.cpp"
#include <unistd.h>

void PrintRegisterFile( Component::IntegerRegisterFile * RegF )
{
    printf( "\nPrinting Register File Content:\n\n" );

    unsigned int iterator;
    for( iterator = 0; iterator < 32; iterator++ )
    {
        printf( "RegF[%d] = %lx", iterator, RegF->GetVal(iterator) );
        if( (iterator + 1) % 8 )
            printf( ", " );
        else
            printf( "\n" );
    }
}






int main( int argc, char * argv[] )
{

    printf( "##########################################################################\n" );
    printf( "Testing Virtual Machine with a fibbonachi program. (finds Fibb(n) recursively in O(2^n) time).\n" );

    uint32_t FibbonachiTestProgram[] = {
        0x70000137, // Main: lui x2 0x70000
        0x00200F93, // addi x31 x0, 0x2
        0x01FF9F93, // slli x31, x31, 31
        0x00810113, // addi x2, x2, 8
        0x00000293, // addi x5, x0, 0
        0x0240006F, // jal x0, Main_For_TestCond
        0xFE513C23, // Main_For_Code: sd x5, -8, x2
        0x00028633, // add x12, x5, x0
        0x024000EF, // jal x1, Proc_Fibb
        0x00AFB123, // sd x10, 0x2, x31
        0x00A00593, // addi x11, x0, '\n'
        0x00BF8023, // sb x11, 0x0, x31
        0xFF813283, // ld x5, -8, x2
        0x00128293, // Main_For_Iteration: addi x5, x5, 1
        0x01E00313, // Main_For_TestCond: addi x6, x0, 10
        0xFC62EEE3, // bltu x5, x6, Main_For_Code
        0x020F8023, // Main_For_End: Main_Return: sb x0, 0x20, x31
        0x02010113, // Proc_Fibb: addi x2, x2, 32
        0xFE113C23, // sd x1, -8, x2
        0x00200293, // addi x5, x0, 2
        0x02566663, // bltu x12, x5, Proc_Fibb_Else
        0xFEC13823, // Proc_Fibb_If: sd x12, -16, x2
        0xFFF60613, // addi x12, x12, -1
        0x044000E7, // jalr x1, x0, Proc_Fibb
        0xFEA13423, // sd x10, -24, x2
        0xFF013603, // ld x12, -16, x2
        0xFFE60613, // addi x12, x12, -2
        0x044000E7, // jalr x1, x0, Proc_Fibb
        0xFE813283, // ld x5, -24, x2
        0x00A28533, // add x10, x5, x10
        0x0080006F, // jal x0, Proc_Fibb_Return
        0x00100513, // Proc_Fibb_Else: addi x10, x0, 1
        0xFF813083, // Proc_Fibb_EndIf: Proc_Fibb_Return: ld x1, -8, x2
        0xFE010113, // addi x2, x2, -32
        0x00008067, // jalr x0, x1, 0
    };

    uint64_t programLength = sizeof( FibbonachiTestProgram ) / sizeof(uint32_t);

    uint64_t iterator;
    for( iterator = 0; iterator < programLength; iterator++ )
    {
        printf( "%x\n", FibbonachiTestProgram[iterator] );
    }
    
    printf( "\nSetting up the Virtual Machine:\n\n" );

    VirtualMachine leVM;

    HwError result = leVM.Setup();

    printf( "%s\n", DecodeHwError( result ) );

    if( result != HwError_NoError )
        return 0;

    printf( "Writing the program to the virtual memory:\n" );

    Component::MemoryAccess * Mem = leVM.GetMem();
    Component::IntegerRegisterFile * RegF = leVM.RegF();

    for( iterator = 0; iterator < programLength; iterator++ )
    {
        Mem->SetWord( 4*iterator, FibbonachiTestProgram[iterator] );
        result = Mem->LastHwError();
        if( result != HwError_NoError )
        {
            printf( "%s\n", DecodeHwError( result ) );
            return 0;
        }
    }

    printf( "Running...:\n" );

    iterator = 0;
    int alive = 1;
    while( alive )
    {
        printf( "\n=============================================================================\n\n" );
        printf( "Round %ld:\n", iterator++ );
        printf( "Registers Before Next Execution:\n" );
        PrintRegisterFile( RegF );
        printf( "\n-----------------------------------------------------------------------------\n\n" );
        alive = (int) leVM.PerformExecutionRound();
        sleep(1);
    }

    result = leVM.LastHwError();

    printf( "We are done with: %s\n", DecodeHwError( result ) );

    return 0;
}
