
#include <stdint.h>
#include <stdio.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"
#include "DebugPrint.hpp"

//#include "StageIF.cpp"
//#include "StageID.cpp"
//#include "StageEX.cpp"

#include "Memory.cpp"
#include "StageMEM.cpp"

#include "RegisterFile.cpp"
#include "StageWB.cpp"

void TestIF();
void TestID();
void TestEX();
void TestMEM();
void TestWB();


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


void TestIF()
{

}

void TestID()
{

}

void TestEX()
{

}

void TestMEM()
{

    const uint64_t MemSize = 0xffffff + 1;
    Component::RawMemoryOnOS leRawMem( MemSize );

    printf( "=================================================================\n" );
    printf( "\nTesting MEM\n\n" );

    printf( "Allocation Test:\n" );

    HwError leError;
    leError = leRawMem.Setup();
    if( leError == HwError_NoError )
        printf( "Allocated %ld bytes of space.\n", MemSize );
    else
    {
        printf( "%s", DecodeHwError( leError ) );
        return;
    }

    Stage::MEM leMEM( &leRawMem );

    leRawMem.SetByte( 0x00ff, 0x81 );
    leRawMem.SetByte( 0x0fff, 0x7f );
    leRawMem.SetHWord( 0xdead, 0xfac3 );
    leRawMem.SetWord( 0xc0ffee, 0xdeadbeef );
    leRawMem.SetDWord( 0xc0de, 0xff00ff00ff00ff00 );

    Stage::Section_Instruction DefaultInstruction = { 0x00 };
    Stage::Section_PCResult DefaultPCResult = { 0, 0x00 };
    Stage::Section_WBParams DefaultWBParams = { 0, 8, 0xdee5 };
    

    Stage::Pipeline_EXtoMEM testPipeline1 = { DefaultInstruction, { 0, 0, 0, 0, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline2 = { DefaultInstruction, { 1, 0, 0xff, 0, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline3 = { DefaultInstruction, { 1, 0, 0xff, 0, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline4 = { DefaultInstruction, { 1, 0, 0xdead, 1, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline5 = { DefaultInstruction, { 1, 0, 0xdead, 1, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline6 = { DefaultInstruction, { 1, 0, 0xc0ffee, 2, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline7 = { DefaultInstruction, { 1, 0, 0xc0ffee, 2, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline8 = { DefaultInstruction, { 1, 0, 0xc0de, 3, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline9 = { DefaultInstruction, { 1, 0, 0xc0de, 3, 1, 0 }, DefaultPCResult, DefaultWBParams };

    Stage::Pipeline_EXtoMEM testPipeline10 = { DefaultInstruction, { 1, 0, 0xfff, 0, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline11 = { DefaultInstruction, { 1, 0, 0xfff, 0, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline12 = { DefaultInstruction, { 1, 0, 0xfff, 1, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline13 = { DefaultInstruction, { 1, 0, 0xfff, 1, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline14 = { DefaultInstruction, { 1, 0, 0xfff, 2, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline15 = { DefaultInstruction, { 1, 0, 0xfff, 2, 1, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline16 = { DefaultInstruction, { 1, 0, 0xfff, 3, 0, 0 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline17 = { DefaultInstruction, { 1, 0, 0xfff, 3, 1, 0 }, DefaultPCResult, DefaultWBParams };

    Stage::Pipeline_EXtoMEM testPipeline18 = { DefaultInstruction, { 1, 0, MemSize - 2, 3, 0, 0 }, DefaultPCResult, DefaultWBParams };
    
    Stage::Pipeline_EXtoMEM testPipelinesRead[] = { testPipeline1, testPipeline2, testPipeline3, testPipeline4, testPipeline5, testPipeline6, testPipeline7, testPipeline8, testPipeline9, testPipeline10, testPipeline11, testPipeline12, testPipeline13, testPipeline14, testPipeline15, testPipeline16, testPipeline17, testPipeline18 };

    printf( "\nRead Tests:\n\n" );

    unsigned int NumReadPipelines = sizeof( testPipelinesRead ) / sizeof( Stage::Pipeline_EXtoMEM );
    unsigned int readIterator;
    for( readIterator = 0; readIterator < NumReadPipelines; readIterator++ )
    {
        Stage::Pipeline_MEMtoWB result;

        PrintPipeEXtoMEM( testPipelinesRead[readIterator] );

        result = leMEM.Perform( testPipelinesRead[readIterator] );

        HwError LastHwError = leMEM.LastHwError();
        if( LastHwError != HwError_NoError )
        {
            printf( "Result was an error: %s\n", DecodeHwError( LastHwError ) );
            // keep going
        }

        PrintPipeMEMtoWB( result );

        printf( "\n" );
    }

    Stage::Pipeline_EXtoMEM testPipeline50 = { DefaultInstruction, { 0, 1, 0x700, 0, 0, 0xfe }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline51 = { DefaultInstruction, { 0, 1, 0x710, 1, 0, 0xac }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline52 = { DefaultInstruction, { 0, 1, 0x720, 2, 0, 0x25 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline53 = { DefaultInstruction, { 0, 1, 0x730, 3, 0, 0x65 }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline54 = { DefaultInstruction, { 0, 1, 0x740, 0, 0, 0xffffffffffffffff }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline55 = { DefaultInstruction, { 0, 1, 0x750, 1, 0, 0xffffffffffffffff }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline56 = { DefaultInstruction, { 0, 1, 0x760, 2, 0, 0xffffffffffffffff }, DefaultPCResult, DefaultWBParams };
    Stage::Pipeline_EXtoMEM testPipeline57 = { DefaultInstruction, { 0, 1, 0x770, 3, 0, 0xffffffffffffffff }, DefaultPCResult, DefaultWBParams };

    Stage::Pipeline_EXtoMEM testPipeline58 = { DefaultInstruction, { 0, 1, MemSize, 3, 0, 0xffffffffffffffff }, DefaultPCResult, DefaultWBParams };

    Stage::Pipeline_EXtoMEM testPipelinesWrite[] = { testPipeline50, testPipeline51, testPipeline52, testPipeline53, testPipeline54, testPipeline55, testPipeline56, testPipeline57, testPipeline58 };

    printf( "\nWrite Test:\n\n" );

    unsigned int NumWritePipelines = sizeof( testPipelinesWrite ) / sizeof( Stage::Pipeline_EXtoMEM );
    unsigned int writeIterator;
    for( writeIterator = 0; writeIterator < NumWritePipelines; writeIterator++ )
    {
        Stage::Pipeline_MEMtoWB result;

        PrintPipeEXtoMEM( testPipelinesWrite[writeIterator] );

        result = leMEM.Perform( testPipelinesWrite[writeIterator] );

        HwError LastHwError = leMEM.LastHwError();
        if( LastHwError != HwError_NoError )
        {
            printf( "Result was an error: %s\n", DecodeHwError( LastHwError ) );
            // keep going
        }
        else
        {
            uint64_t Address = testPipelinesWrite[writeIterator].MEMParams.Address;

            uint64_t Value = leRawMem.GetDWord( Address );

            printf( "\nWrote %lx\n\n", Value );
        }

        PrintPipeMEMtoWB( result );
    }

}

void TestWB()
{
    Component::IntegerRegisterFile RegF;

    RegF.Setup();

    Stage::Pipeline_MEMtoWB testPipeline1 = { { 0x00 }, { 1, 31, 0xdeadbeef } };
    Stage::Pipeline_MEMtoWB testPipeline2 = { { 0x00 }, { 1, 0, 0xdeadbeef } };
    Stage::Pipeline_MEMtoWB testPipeline3 = { { 0x00 }, { 0, 30, 0xc0ffee } };
    Stage::Pipeline_MEMtoWB testPipeline4 = { { 0x00 }, { 0, 0, 0xc0ffee } };
    Stage::Pipeline_MEMtoWB testPipeline5 = { { 0x00 }, { 1, 31, 0xff00ff00 } };
    Stage::Pipeline_MEMtoWB testPipeline6 = { { 0x00 }, { 1, 15, 0xc0ffee } };
    Stage::Pipeline_MEMtoWB testPipeline7 = { { 0x00 }, { 1, 7, 0xffffffffffffffff } };

    Stage::Pipeline_MEMtoWB Pipelines[] = { testPipeline1, testPipeline2, testPipeline3, testPipeline4, testPipeline5, testPipeline6, testPipeline7 };

    unsigned int NumTests = sizeof( Pipelines ) / sizeof( Stage::Pipeline_MEMtoWB );

    printf( "=================================================================\n" );
    printf( "\nTesting WB\n\n" );

    Stage::WB leWB( &RegF );

    PrintRegisterFile( &RegF );

    unsigned int pipeIterator;
    for( pipeIterator = 0; pipeIterator < NumTests; pipeIterator++ )
    {
        PrintPipeMEMtoWB( Pipelines[pipeIterator] );

        leWB.Perform( Pipelines[pipeIterator] );

        PrintRegisterFile( &RegF );
    }

}


int main()
{

    TestIF();

    TestID();

    TestEX();

    TestMEM();

    TestWB();


    return 0;
}
