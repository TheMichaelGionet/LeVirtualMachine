
#include <stdint.h>
#include <stdio.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"
#include "DebugPrint.hpp"

//#include "StageIF.cpp"
//#include "StageID.cpp"
//#include "StageEX.cpp"
//#include "StageMEM.cpp"

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
