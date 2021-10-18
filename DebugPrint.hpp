
#ifndef VM_DebugPrint
#define VM_DebugPrint


#include <stdint.h>
#include "Pipeline.hpp"
#include <stdio.h>

//
// Print Sections:
//


void PrintSectionInstruction( Stage::Section_Instruction leSection )
{
    printf( "Instruction: %x\n", leSection.Get() );
}

void PrintSectionWBParams( Stage::Section_WBParams leSection )
{
    printf( "WB Parameters: DoWB = %d, Reg = %d, Val = %lx\n", (int) leSection.DoWB, (int) leSection.Reg, (uint64_t) leSection.Val );
}

void PrintSectionMEMParams( Stage::Section_MEMParams leSection )
{
    printf( "MEMParams: DoMemRead = %d, DoMemWrite = %d, Address = %lx, MEMSize = %x, SignExtend = %d, WriteVal = %lx\n", (int) leSection.DoMemRead, (int) leSection.DoMemWrite, (uint64_t) leSection.Address, (int) leSection.MemSize, (int) leSection.SignExtend, (uint64_t) leSection.WriteVal );
}

void PrintSectionPCResult( Stage::Section_PCResult leSection )
{
    printf( "PCResult: ReplacePC = %d, PCVal = %lx\n", (int) leSection.ReplacePC, (uint64_t) leSection.PCVal );
}

void PrintSectionWBOp( Stage::Section_WBOp leSection )
{
    printf( "WBOp: DoWB = %d, Reg = %d\n", (int) leSection.DoWB, (int) leSection.Reg );
}

void PrintSectionMEMParamsIDtoEX( Stage::Section_MEMParamsIDtoEX leSection )
{
    printf( "MEMParamsIDtoEX: DoMemRead = %d, DoMemWrite = %d, MemSize = %x, SignExtend = %d, WriteVal = %lx\n", (int) leSection.DoMemRead, (int) leSection.DoMemWrite, (int) leSection.MemSize, (int) leSection.SignExtend, (uint64_t) leSection.WriteVal );
}

void PrintSectionPCCalc( Stage::Section_PCCalc leSection )
{
    printf( "PCCalc: AddToPC = %d, ReplacePC = %d, IsConditional = %d, InvertALUOutput = %d, PCVal = %lx\n", (int) leSection.AddToPC, (int) leSection.ReplacePC, (int) leSection.IsConditional, (int) leSection.InvertALUOutput, (uint64_t) leSection.PCVal );
}

void PrintSectionALUInputs( Stage::Section_ALUInputs leSection )
{
    printf( "ALUInputs: DoALUOp = %d, ALUOp = %x, InvertALUOp = %d, UseImmNotReg2 = %d, ResultType (1-reg val, 0-address) = %d, UsePCRegister = %d, AddFourToResult = %d\n, RegVal1 = %lx, RegVal2 = %lx, Immediate = %lx, PCVal = %lx\n", (int) leSection.DoALUOp, (int) leSection.ALUOp, (int) leSection.InvertALUOp, (int) leSection.UseImmNotReg2, (int) leSection.ResultType, (int) leSection.UsePCRegister, (int) leSection.AddFourToResult, (uint64_t) leSection.RegVal1, (uint64_t) leSection.RegVal2, (uint64_t) leSection.Immediate, (uint64_t) leSection.PCVal );
}

void PrintSectionIDParams( Stage::Section_IDParams leSection )
{
    printf( "IDParams: DoGenConst = %d, ReadRegisters = %x, ReadReg1 = %x, ReadReg2 = %x\n", (int) leSection.DoGenConst, (int) leSection.ReadRegisters, (int) leSection.ReadReg1, (int) leSection.ReadReg2 );
}

void PrintSectionALUOp( Stage::Section_ALUOp leSection )
{
    printf( "ALUOp: DoALUOp = %d, ALUOp = %x, InvertALUOp = %d, UseImmNotReg2 = %d, ResultType (1-reg val, 0-address) = %d, UsePCRegister = %d, AddFourToResult = %d\n", (int) leSection.DoALUOp, (int) leSection.ALUOp, (int) leSection.InvertALUOp, (int) leSection.UseImmNotReg2, (int) leSection.ResultType, (int) leSection.UsePCRegister, (int) leSection.AddFourToResult );
}

void PrintSectionMEMParamsIFtoID( Stage::Section_MEMParamsIFtoID leSection )
{
    printf( "MEMParamsIFtoID: DoMemRead = %d, DoMemWrite = %d, MemSize = %x, SignExtend = %d\n", (int) leSection.DoMemRead, (int) leSection.DoMemWrite, (int) leSection.MemSize, (int) leSection.SignExtend );
}

//
// Print Pipelines:
//

void PrintPipeMEMtoWB( Stage::Pipeline_MEMtoWB lePipe )
{
    printf( "\nPrinting Pipeline MEM to WB\n" );
    PrintSectionInstruction( lePipe.Instruction );
    PrintSectionWBParams( lePipe.WBParams );
}

void PrintPipeEXtoMEM( Stage::Pipeline_EXtoMEM lePipe )
{
    printf( "\nPrinting Pipeline EX to MEM\n" );
    PrintSectionInstruction( lePipe.Instruction );
    PrintSectionMEMParams( lePipe.MEMParams );
    PrintSectionPCResult( lePipe.PCResult );
    PrintSectionWBParams( lePipe.WBParams );
}

void PrintPipeIDtoEX( Stage::Pipeline_IDtoEX lePipe )
{
    printf( "\nPrinting Pipeline ID to EX\n" );
    PrintSectionInstruction( lePipe.Instruction );
    PrintSectionALUInputs( lePipe.ALUInputs );
    PrintSectionPCCalc( lePipe.PCCalc );
    PrintSectionMEMParamsIDtoEX( lePipe.MEMParamsIDtoEX );
    PrintSectionWBOp( lePipe.WBOp );
}

void PrintPipeIFtoID( Stage::Pipeline_IFtoID lePipe )
{
    printf( "\nPrinting Pipeline IF to ID\n" );
    PrintSectionInstruction( lePipe.Instruction );
    PrintSectionIDParams( lePipe.IDParams );
    PrintSectionALUOp( lePipe.ALUOp );
    PrintSectionPCCalc( lePipe.PCCalc );
    PrintSectionMEMParamsIFtoID( lePipe.MEMParamsIFtoID );
    PrintSectionWBOp( lePipe.WBOp );
}

#endif
