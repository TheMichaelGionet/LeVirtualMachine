
#include <memory.h>
#include <stdint.h>
#include "Pipeline.hpp"
#include <stdio.h>
#include "DebugPrint.hpp"

void TestPipelines()
{
    Stage::Pipeline_IFtoID IFID;
    Stage::Pipeline_IDtoEX IDEX;
    Stage::Pipeline_EXtoMEM EXMEM;
    Stage::Pipeline_MEMtoWB MEMWB;

    memset( &IFID, 0xff, sizeof(Stage::Pipeline_IFtoID) );
    memset( &IDEX, 0xff, sizeof(Stage::Pipeline_IDtoEX) );
    memset( &EXMEM, 0xff, sizeof(Stage::Pipeline_EXtoMEM) );
    memset( &MEMWB, 0xff, sizeof(Stage::Pipeline_MEMtoWB) );

    IFID.Reset();
    IDEX.Reset();
    EXMEM.Reset();
    MEMWB.Reset();

    printf( "\nTesting Reset Pipes\n\n" );

    PrintPipeIFtoID( IFID );
    PrintPipeIDtoEX( IDEX );
    PrintPipeEXtoMEM( EXMEM );
    PrintPipeMEMtoWB( MEMWB );

    Stage::Pipeline_IFtoID IFID_ForCopy;
    Stage::Pipeline_IDtoEX IDEX_ForCopy;
    Stage::Pipeline_EXtoMEM EXMEM_ForCopy;
    Stage::Pipeline_MEMtoWB MEMWB_ForCopy;

    memset( &IFID_ForCopy, 0xff, sizeof(Stage::Pipeline_IFtoID) );
    memset( &IDEX_ForCopy, 0xff, sizeof(Stage::Pipeline_IDtoEX) );
    memset( &EXMEM_ForCopy, 0xff, sizeof(Stage::Pipeline_EXtoMEM) );
    memset( &MEMWB_ForCopy, 0xff, sizeof(Stage::Pipeline_MEMtoWB) );

    printf( "\nTesting ID to EX pipe\n\n" );

    printf( "\nCopy From Last Test:\n" );
    IDEX.CopyFromLast( IFID_ForCopy );
    PrintPipeIDtoEX( IDEX );

    printf( "\nSet ALU Inputs Test:\n" );
    IDEX.SetALUInputs( ~(uint64_t)0, ~(uint64_t)0, ~(uint64_t)0, ~(uint64_t)0 );
    PrintPipeIDtoEX( IDEX );

    printf( "\nSet Mem Param test:\n" );
    IDEX.SetMemParam( ~(uint64_t)0 );
    PrintPipeIDtoEX( IDEX );

    printf( "\nTesting EX to MEM pipe\n\n" );

    printf( "\nCopy From Last Test:\n" );
    EXMEM.CopyFromLast( IDEX_ForCopy );
    PrintPipeEXtoMEM( EXMEM );

    printf( "\nSet Memory Address:\n" );
    EXMEM.SetMemAddress( ~(uint64_t) 0 );
    PrintPipeEXtoMEM( EXMEM );

    printf( "\nSet Write Back Value:\n" );
    EXMEM.SetWBVal( ~(uint64_t) 0 );
    PrintPipeEXtoMEM( EXMEM );

    printf( "\nSet PC Value and Replace:\n" );
    EXMEM.SetPCVal( 0xff, ~(uint64_t) 0 );
    PrintPipeEXtoMEM( EXMEM );

    printf( "\nTesting MEM to WB pipe\n\n" );

    printf( "\nCopy From Last:\n" );
    MEMWB.CopyFromLast( EXMEM_ForCopy );
    PrintPipeMEMtoWB( MEMWB );

    printf( "\nSet Write Back Value\n" );
    MEMWB.SetWBVal( 0xff, ~(uint64_t) 0 );
    PrintPipeMEMtoWB( MEMWB );


}


int main()
{

    TestPipelines();

    

    return 0;
}
