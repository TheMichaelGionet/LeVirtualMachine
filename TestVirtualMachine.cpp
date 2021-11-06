#include <stdio.h>
#include <memory.h>

#include "VirtualMachine.cpp"









int main( int argc, char * argv[] )
{

    printf( "##########################################################################\n" );
    printf( "Testing Virtual Machine with a fibbonachi program. (finds Fibb(n) recursively in O(2^n) time).\n" );

    uint32_t FibbonachiTestProgram[] = {
        0x70000137, // correct
        0x00200F93, // correct
        0x01FF9F93, // correct
        0x00810113, // correct
        0x00000293, // correct
        0x0240006F, // correct
        0xFE513C23, // correct
        0x00028633, // correct
        0x024000EF, // correct
        0x00AFB123, // correct
        0x00A00593, // correct
        0x00BF8023, // correct
        0xFF813283, // correct
        0x00128293, // correct
        0x01E00313, // correct
        0xFC62EEE3, // correct
        0x020F8023, // correct
        0x02010113, // correct
        0xFE113C23, // correct
        0x00200293, // correct
        0x02566663, // correct
        0xFEC13823, // correct
        0xFFF60613, // correct
        0x044000E7, // correct
        0xFEA13423, // correct
        0xFF013603, // correct
        0xFFE60613, // correct
        0x044000E7, // correct
        0xFE813283, // correct
        0x00A28533, // correct
        0x0080006F, // correct
        0x00100513, // correct
        0xFF813083, // correct
        0xFE010113, // correct
        0x00008067, // correct
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
        alive = (int) leVM.PerformExecutionRound();
        printf( "Round %ld has occurred\n", iterator );
    }

    result = leVM.LastHwError();

    printf( "We are done with: %s\n", DecodeHwError( result ) );

    return 0;
}
