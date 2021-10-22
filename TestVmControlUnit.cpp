
#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"
#include "ControlUnit.cpp"
#include "DebugPrint.hpp"
#include <stdio.h>

uint32_t InstructionList[] = {
    0x000000EF, // jal x1 0
    0x00808113, // addi x2 x1 8
    0x004101E7, // jalr x3 x2 4
    0x001141B3, // xor x3 x2 x1
    0x00000017, // auipc x0 0
    0x02303E23, // sd x3 60(x0)
    0x04C03203, // ld x4 76(x0)
    0x04C02283, // lw x5 76(x0)
    0x0BEEF317, // auipc x6 0xbeef
    0xDEADB3B7, // lui x7 0xdeadb
    0x5EF38393, // addi x7 x7 0x5ef
    0x00100073, // ebreak
    0x00000073, // ecall
    0xFC30C6E3, // blt x1 x3 -52
    0xFC30D4E3, // bge x1 x3 -56
    0xFC3082E3, // beq x1 x3 -60
    0xFC3090E3, // bne x1 x3 -64
    0xFA30EEE3, // bltu x1 x3 -68
    0xFA30FCE3, // bgeu x1 x3 -72
    0x00000000, // not a real instruction.
};

const char * AssemblyList[] = {
    "jal x1 0",
    "addi x2 x1 8",
    "jalr x3 x2 4",
    "xor x3 x2 x1",
    "auipc x0 0",
    "sd x3 60(x0)",
    "ld x4 76(x0)",
    "lw x5 76(x0)",
    "auipc x6 0xbeef",
    "lui x7 0xdeadb",
    "addi x7 x7 0x5ef",
    "ebreak",
    "ecall",
    "blt x1 x3 -52",
    "bge x1 x3 -56",
    "beq x1 x3 -60",
    "bne x1 x3 -64",
    "bltu x1 x3 -68",
    "bgeu x1 x3 -72",
    "not a real instruction (just 0s)",
};

const uint64_t InitPCVal = 0xc0ffee;

void TestControlUnit()
{
    printf( "\nTest Control Unit\n\n" );

    Component::ControlUnit leSignals;

    if( leSignals.Setup() != HwError_NoError )
    {
        printf( "Something went wrong in the setup.\n" );
        return;
    }
    else
        printf( "Setup without error.\n" );

    uint64_t leSize = sizeof(InstructionList) / sizeof(uint32_t);

    uint64_t iterator;
    for( iterator = 0; iterator < leSize; iterator++ )
    {
        uint32_t lInstruction = InstructionList[iterator];

        uint64_t DummyPCVal = InitPCVal + sizeof(uint32_t)*iterator;

        printf( "\n=======================================================================================\n" );
        printf( "Instr = %x, PCVal = %lx, ASM = %s\n", lInstruction, DummyPCVal, AssemblyList[iterator] );

        Stage::Pipeline_IFtoID result = leSignals.Perform( lInstruction, DummyPCVal );

        HwError leLast = HwError_NoError;
        if( (leLast = leSignals.LastHwError()) != HwError_NoError )
        {
            printf( "An Error Occurred in obtaining control signals: %s\n", DecodeHwError( leLast ) );
            return;
        }

        PrintPipeIFtoID( result );
    }
}








int main()
{

    TestControlUnit();


    return 0;
}

