#ifndef Test_VMComponents
#define Test_VMComponents

// Debug is for debugging new features. More error checking is used.
// Release is for customers. Things that are expected to work are not checked.
// None is just remove all error checking. May be used if we need a speed advantage.
#define ErrorMode_Debug 1
#define ErrorMode_Release 2
#define ErrorMode_None 3

// This is the variable that is used to change compilation.
#define ErrorMode ErrorMode_Debug


#include <stdint.h>

#include "HwError.hpp"
#include "Component.hpp"
#include "ALU.cpp"
#include "RegisterFile.cpp"
#include "Memory.cpp"
#include "InstructionType.hpp"
#include "ConstantGenerator.hpp"

#include <stdio.h>
#include <string>

void TestALU();
void TestRegisterFile();
void TestFloatRegisterFile();
void TestRawMemory();
void TestAnInstruction( Instruction::InstructionParser * leParser, uint32_t instruction );
void TestInstructionParse();
void TestConstantGenerator();
int main( int argc, char * argv[] );



void TestALU()
{
    Component::ALU Alu;
    Alu.Setup();

    uint8_t index;

    bool setFunct7 = true;
    bool unsetFunct7 = false;

    // test Add/Sub

    printf( "\nTest Add\n\n" );

    uint64_t val1;
    uint64_t val2;
    
    val1 = 0;
    val2 = 0;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 0;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    val1 = 0x10;
    val2 = 0x01;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    val1 = 0xa0a210426df;
    val2 = 0xaaab3153264;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    val1 = (uint64_t) -1;
    val2 = (uint64_t) 1;
    printf( "%lx + %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, unsetFunct7 ) );

    printf( "\nTest Sub\n\n" );

    val1 = 0;
    val2 = 0;
    printf( "%lx - %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, setFunct7 ) );

    val1 = 1;
    val2 = 0;
    printf( "%lx - %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, setFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx - %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, setFunct7 ) );

    val1 = -1;
    val2 = -1;
    printf( "%lx - %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, setFunct7 ) );

    val1 = 0xaaaaaaaaaaaa;
    val2 = 0x0aaaaaaaaaaa;
    printf( "%lx - %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b000, val1, val2, setFunct7 ) );


    // test shift left

    printf( "\nTest Left Shift\n\n" );
    index = 0;
    for( index = 0; index < 128; index++ )
    {
        val1 = 1;
        val2 = index;
        printf( "%lx << %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b001, val1, val2, unsetFunct7 ) );
    }

    // test set less than signed

    int32_t sindex;
    
    printf( "\nTest Signed less Than\n\n" );
    for( sindex = -1000; sindex < 1000; sindex++ )
    {
        val1 = 400;
        val2 = (uint64_t) sindex;
        printf( "%lx SLT %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b010, val1, val2, unsetFunct7 ) );
    }
    

    // test set less than unsigned

    
    printf( "\nTest Unsigned Less Than\n\n" );
    for( sindex = -1000; sindex < 1000; sindex++ )
    {
        val1 = 400;
        val2 = (uint64_t) sindex;
        printf( "%lx SLTU %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b011, val1, val2, unsetFunct7 ) );
    }
    

    // test XOR

    printf( "\nTest XOR\n\n" );
    val1 = 0;
    val2 = 0;
    printf( "%lx ^ %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b100, val1, val2, unsetFunct7 ) );

    val1 =-1;
    val2 = 0;
    printf( "%lx ^ %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b100, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = -1;
    printf( "%lx ^ %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b100, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 0;
    printf( "%lx ^ %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b100, val1, val2, unsetFunct7 ) );

    val1 = 0xa1849bf420;
    val2 = 0xa52ba4ef22;
    printf( "%lx ^ %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b100, val1, val2, unsetFunct7 ) );

    // test right shift ordinary

    printf( "\nRight shift ordinary\n\n" );
    val1 = 1;
    val2 = 0;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 2;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    val1 = 0xc0ffee;
    val2 = 5;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = 8;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    val1 = -1 * (int64_t) 0x10ffeec0ffeec0ff;
    val2 = 32;
    printf( "%lx >> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, unsetFunct7 ) );

    // test right shift arithmetic

    printf( "\nTest right shift arithmetic\n\n" );
    val1 = 1;
    val2 = 0;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    val1 = 1;
    val2 = 2;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    val1 = 0xc0ffee;
    val2 = 5;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    val1 = -1;
    val2 = 8;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    val1 = -1 * (int64_t) 0x10ffeec0ffeec0ff;
    val2 = 32;
    printf( "%lx >>> %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b101, val1, val2, setFunct7 ) );

    // OR

    printf( "\nTest OR\n\n" );
    val1 = 0;
    val2 = 0;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = 0;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = -1;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 0;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    val1 = 0xc0ffee000000;
    val2 = 0x0c0ffee00000;
    printf( "%lx | %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b110, val1, val2, unsetFunct7 ) );

    // AND

    printf( "\nTest And\n\n" );
    val1 = 0;
    val2 = 0;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = 0;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );

    val1 = -1;
    val2 = -1;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 0;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );

    val1 = 1;
    val2 = 1;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );

    val1 = 0xc0ffee000000;
    val2 = 0x0c0ffee00000;
    printf( "%lx & %lx = %lx\n", val1, val2, Alu.DoALUOp( 0b111, val1, val2, unsetFunct7 ) );
}

void TestRegisterFile()
{
    
    Component::IntegerRegisterFile RegF;
    printf( "\nTesting Integer Register File.\n\n" );
    RegF.Setup();

    unsigned int iterator;
    for( iterator = 0; iterator <  RegF.Length(); iterator++ )
    {
        printf( "Regs[%d] = %lx\n", iterator, RegF.GetVal( iterator ) );
    }

    for( iterator = 0; iterator < RegF.Length(); iterator++ )
    {
        RegF.SetVal( iterator, (iterator | ( iterator << 8 )) + 1 );
    }

    for( iterator = 0; iterator < RegF.Length(); iterator++ )
    {
        printf( "Regs[%d] = %lx\n", iterator, RegF.GetVal( iterator ) );
    }

}

void TestFloatRegisterFile()
{
    Component::FloatRegisterFile RegF;
    printf( "\nTesting Float Register File.\n\n" );
    RegF.Setup();

    unsigned int iterator;
    for( iterator = 0; iterator <  RegF.Length(); iterator++ )
    {
        printf( "Regs[%d] = %f\n", iterator, RegF.GetVal( iterator ) );
    }

    for( iterator = 0; iterator < RegF.Length(); iterator++ )
    {
        RegF.SetVal( iterator, 0.1 * iterator + 0.3 );
    }

    for( iterator = 0; iterator < RegF.Length(); iterator++ )
    {
        printf( "Regs[%d] = %f\n", iterator, RegF.GetVal( iterator ) );
    }

}

void TestRawMemory()
{
    const uint64_t MemSizeInBytes = 64;
    Component::RawMemoryOnOS Mem( MemSizeInBytes );
    HwError result = Mem.Setup();
    if( result != HwError_NoError )
    {
        printf( "%s\n", DecodeHwError( result ) );
        return;
    }

    uint64_t iterator;


    for( iterator = 0; iterator < MemSizeInBytes; iterator++ )
    {
        Mem.SetByte( iterator, 0 );
    }

    for( iterator = 0; iterator < MemSizeInBytes; iterator++ )
    {
        printf( "Mem[%ld] = %x\n", iterator, Mem.GetByte(iterator) );
    }


    for( iterator = 0; iterator < MemSizeInBytes; iterator++ )
    {
        Mem.SetByte( iterator, (uint8_t) ( (iterator + 1) & 0xff ) );
    }

    for( iterator = 0; iterator < MemSizeInBytes; iterator++ )
    {
        printf( "Mem[%ld] = %x\n", iterator, Mem.GetByte(iterator) );
    }

    
    for( iterator = 0; iterator < MemSizeInBytes/2; iterator++ )
    {
        Mem.SetHWord( 2*iterator, (uint16_t) ( ( iterator << 8 ) | (iterator+1) ) );
    }

    for( iterator = 0; iterator < MemSizeInBytes/2; iterator++ )
    {
        printf( "Memh[%ld] = %x\n", iterator, Mem.GetHWord( 2*iterator ) );
        printf( "Mem[%ld], Mem[%ld+1] = %x, %x\n", iterator, iterator, Mem.GetByte( 2*iterator ), Mem.GetByte( 2*iterator+1 ) );
    }


    for( iterator = 0; iterator < MemSizeInBytes/4; iterator++ )
    {
        uint32_t val = (uint32_t) (iterator << 24) | ( (iterator+1) << 16 ) | ( (iterator+2) << 8 ) | ( iterator+3 );
        Mem.SetWord( 4*iterator, val );
    }

    for( iterator = 0; iterator < MemSizeInBytes/4; iterator++ )
    {
        printf( "Memw[%ld] = %x\n", iterator, Mem.GetWord( 4*iterator ) );
        printf( "Mem[%ld], Mem[%ld+1], Mem[%ld+2], Mem[%ld+3] = %x, %x, %x, %x\n", iterator, iterator, iterator, iterator, Mem.GetByte( 4*iterator ), Mem.GetByte( 4*iterator+1 ), Mem.GetByte( 4*iterator+2 ), Mem.GetByte( 4*iterator+3 ) );
    }


    for( iterator = 0; iterator < MemSizeInBytes/8; iterator++ )
    {
        uint64_t val = (iterator << 24) | ( (iterator+1) << 16 ) | ( (iterator+2) << 8 ) | ( iterator+3 );
        val = val | (~(val << 32) ^ 0xffffffff );
        Mem.SetWord( 8*iterator, val );
    }

    for( iterator = 0; iterator < MemSizeInBytes/8; iterator++ )
    {
        printf( "Memd[%ld] = %lx\n", iterator, Mem.GetDWord( 8*iterator ) );
        printf( "Mem[%ld ... += 8] = [%x %x %x %x %x %x %x %x]\n", iterator, Mem.GetByte( 8*iterator ), Mem.GetByte( 8*iterator+1 ), Mem.GetByte( 8*iterator+2 ), Mem.GetByte( 8*iterator+3 ), Mem.GetByte( 8*iterator+4 ), Mem.GetByte( 8*iterator+5 ), Mem.GetByte( 8*iterator+6 ), Mem.GetByte( 8*iterator+7 ) );
    }

    result = Mem.LastHwError();
    printf( "%s\n", DecodeHwError( result ) );

    printf( "\nAttempt to access an out of bounds element:\n\n" );

    printf( "Mem[%ld] = %x\n", MemSizeInBytes, Mem.GetByte( MemSizeInBytes ) );

    result = Mem.LastHwError();
    printf( "%s\n", DecodeHwError( result ) );

}


void TestAnInstruction( Instruction::InstructionParser * leParser, uint32_t instruction )
{
    Instruction::InstructionType leType;
    Instruction::ParsedIType Parsed;

    Instruction::ParsedInstruction P = leParser->ParseInstruction( instruction );

    leType = P.Type;
    Parsed = P.ParsedInst;
    printf( "Instruction %x type is %d: ", instruction, leType );
    switch( leType )
    {
        case Instruction::InstructionType_Null:
        {
            printf( "opcode = %x, ", Parsed.Null.opcode );
            printf( "garb = %x\n", Parsed.Null.garb );
            break;
        }
        case Instruction::InstructionType_R:
        {
            printf( "opcode = %x, ", Parsed.R.opcode );
            printf( "rd = %x, ", Parsed.R.rd );
            printf( "funct3 = %x, ", Parsed.R.funct3 );
            printf( "rs1 = %x, ", Parsed.R.rs1 );
            printf( "rs2 = %x, ", Parsed.R.rs2 );
            printf( "funct7 = %x\n", Parsed.R.funct7 );
            break;
        }
        case Instruction::InstructionType_I:
        {
            printf( "opcode = %x, ", Parsed.I.opcode );
            printf( "rd = %x, ", Parsed.I.rd );
            printf( "funct3 = %x, ", Parsed.I.funct3 );
            printf( "rs1 = %x, ", Parsed.I.rs1 );
            printf( "imm0to11 = %x\n", Parsed.I.imm0to11 );
            break;
        }
        case Instruction::InstructionType_S:
        {
            printf( "opcode = %x, ", Parsed.S.opcode );
            printf( "imm0to4 = %x, ", Parsed.S.imm0to4 );
            printf( "funct3 = %x, ", Parsed.S.funct3 );
            printf( "rs1 = %x, ", Parsed.S.rs1 );
            printf( "rs2 = %x, ", Parsed.S.rs2 );
            printf( "imm5to11 = %x\n", Parsed.S.imm5to11 );
            break;
        }
        case Instruction::InstructionType_SB:
        {
            printf( "opcode = %x, ", Parsed.SB.opcode );
            printf( "imm11and1to4 = %x, ", Parsed.SB.imm11and1to4 );
            printf( "funct3 = %x, ", Parsed.SB.funct3 );
            printf( "rs1 = %x, ", Parsed.SB.rs1 );
            printf( "rs2 = %x, ", Parsed.SB.rs2 );
            printf( "imm5to10and12 = %x\n", Parsed.SB.imm5to10and12 );
            break;
        }
        case Instruction::InstructionType_U:
        {
            printf( "opcode = %x, ", Parsed.U.opcode );
            printf( "rd = %x, ", Parsed.U.rd );
            printf( "imm12to31 = %x\n", Parsed.U.imm12to31 );
            break;
        }
        case Instruction::InstructionType_UJ:
        {
            printf( "opcode = %x, ", Parsed.UJ.opcode );
            printf( "rd = %x, ", Parsed.UJ.rd );
            printf( "imm12to19and11and1to10and20 = %x\n", Parsed.UJ.imm12to19and11and1to10and20 );
            break;
        }
    }
}

void TestInstructionParse()
{
    uint32_t instr1 = 0x03808083; // lb x1, label2
    uint32_t instr2 = 0x7ee00093; // addi x1, x0, 0x7ee
    uint32_t instr3 = 0x0beef217; // auipc x4, 0xbeef
    uint32_t instr4 = 0x7ff00093; // addi x1, x0, 0x7ff
    uint32_t instr5 = 0x02008223; // sb x0, label2, x1
    uint32_t instr6 = 0x00108133; // add x1, x1, x1
    uint32_t instr7 = 0x0c0011b7; // lui x3, 0xc001
    uint32_t instr8 = 0x00110133; // add x2, x2, x1
    uint32_t instr9 = 0x00000863; // beq x0, x0, label2
    uint32_t instr10 = 0x011000e7; // jalr x1, x0, 0x11
    uint32_t instr11 = 0x008001ef; // jal x1, x0, label2
    uint32_t instr12 = 0x00000073; // ecall

    Instruction::InstructionParser leParser;

    printf( "\nTesting Instruction Parser\n\n" );

    TestAnInstruction( &leParser, instr1 );
    TestAnInstruction( &leParser, instr2 );
    TestAnInstruction( &leParser, instr3 );
    TestAnInstruction( &leParser, instr4 );
    TestAnInstruction( &leParser, instr5 );
    TestAnInstruction( &leParser, instr6 );
    TestAnInstruction( &leParser, instr7 );
    TestAnInstruction( &leParser, instr8 );
    TestAnInstruction( &leParser, instr9 );
    TestAnInstruction( &leParser, instr10 );
    TestAnInstruction( &leParser, instr11 );
    TestAnInstruction( &leParser, instr12 );

    printf( "sizeof(ParsedIType_R) = %ld\n", sizeof(Instruction::ParsedIType_R) );

}

void TestConstantGenerator()
{

    uint32_t instr1 = 0x03808083;  //  I: lb x1, label2
    uint32_t instr2 = 0x7ee00093;  //  I: addi x1, x0, 0x7ee
    uint32_t instr3 = 0x0beef217;  //  U: auipc x4, 0xbeef
    uint32_t instr4 = 0x7ff00093;  //  I: addi x1, x0, 0x7ff
    uint32_t instr5 = 0x02008223;  //  S: sb x0, label2, x1
    uint32_t instr6 = 0x00108133;  //  R: add x1, x1, x1
    uint32_t instr7 = 0x0c0011b7;  //  U: lui x3, 0xc001
    uint32_t instr8 = 0x00110133;  //  R: add x2, x2, x1
    uint32_t instr9 = 0x00000863;  // SB: beq x0, x0, label2
    uint32_t instr10 = 0x011000e7; //  U: jalr x1, x0, 0x11
    uint32_t instr11 = 0x008001ef; // UJ: jal x1, x0, label2
    uint32_t instr12 = 0x00000073; //  I: ecall

    // Expected outputs for label2 = { 0x20, 0x18, 0x0c, 0x04 }
    uint32_t instr13 = 0x02008083; // lb x1, label2
    uint32_t instr14 = 0x00100c23; // sb x1, label2, x0
    uint32_t instr15 = 0x00c000ef; // jal x1, label2
    uint32_t instr16 = 0x00000263; // beq x0, x0, label2

    uint32_t instr17 = 0xFFF00093; // addi x1, x0, -1
    uint32_t instr18 = 0x7FF00093; // addi x1, x0, 0x7ff

    uint32_t instr19 = 0xFFDFF0EF; // jal x1 -4
    uint32_t instr20 = 0x004000EF; // jal x1 4

    uint32_t instr21 = 0xFFF000E7; // jalr x1 x0 -1
    uint32_t instr22 = 0x7FF000E7; // jalr x1 x0 0x7ff

    uint32_t instr23 = 0xFE0008E3; // beq x0 x0 -16
    uint32_t instr24 = 0x7E0008E3; // beq x0 x0 0x7E0 ?

    uint32_t instr25 = 0xFFFFF0B7; // lui x1 0xfffff
    uint32_t instr26 = 0x7FFFF0B7; // lui x1 0x7ffff

    uint32_t instr27 = 0xFC102C23; // sw x1 -40 x0
    uint32_t instr28 = 0x7C102C23; // sq x1 something x0

    uint32_t instructions[] = { instr1, instr2, instr3, instr4, instr5, instr6, instr7, instr8, instr9, instr10, instr11, instr12, instr13, instr14, instr15, instr16, instr17, instr18, instr19, instr20, instr21, instr22, instr23, instr24, instr25, instr26, instr27, instr28 };
    uint8_t numInstructions = sizeof( instructions ) / sizeof( uint32_t );

    Instruction::InstructionParser leParser;

    Component::ConstantGenerator generator;

    printf( "\nTesting Constant Generator\n\n" );

    HwError result = generator.Setup();
    if( result != HwError_NoError )
    {
        printf( "Error: %s\n.", DecodeHwError( result ) );
        return;
    }

    unsigned int iterator;
    for( iterator = 0; iterator < numInstructions; iterator++ )
    {
        Instruction::ParsedInstruction lInstruction = leParser.ParseInstruction( instructions[iterator] );
        uint64_t leConst = generator.GenerateConst( lInstruction, true );
        printf( "Instruction %x generated %lx (sign extended)\n", instructions[iterator], leConst );
        leConst = generator.GenerateConst( lInstruction, false );
        printf( "Instruction %x generated %lx (not sign extended)\n", instructions[iterator], leConst );
    }
    
}




int main( int argc, char * argv[] )
{

    bool DoTestALU = true;
    bool DoTestRegisterFile = true;
    bool DoTestFloatRegisterFile = true;
    bool DoTestRawMemory = true;
    bool DoInstructionParseTest = true;
    bool DoConstGenTest = true;

    // Untested section.
    if( argc > 0 )
    {
        int iterator;
        for( iterator = 0; iterator < argc; iterator++ )
        {
            char * leStringAsCharStar = argv[iterator];
            
            printf( "%s\n", argv[iterator] );
            std::string leString( leStringAsCharStar );
            if( leString.find( "-TestALU" ) == 0 )
            {
                std::size_t equalSignLocation = leString.find( '=' );
                if( equalSignLocation != std::string::npos )
                {
                    std::string val( leString.substr( equalSignLocation+1 ) );
                    if( val == "0" )
                        DoTestALU = false;
                    else if( val.length() > 0 )
                        DoTestALU = true;
                    else
                    {
                        printf( "Error in input. Expected \"-TestALU=[1|0]\" as syntax.\n" );
                        return 0;
                    }    
                }
                else
                {
                    printf( "Error in input. Expected \"-TestALU=[1|0]\" as syntax.\n" );
                    return 0;
                }
            }
        }
    }

    if( DoTestALU )
        TestALU();

    if( DoTestRegisterFile )
        TestRegisterFile();

    if( DoTestFloatRegisterFile )
        TestFloatRegisterFile();

    if( DoTestRawMemory )
        TestRawMemory();

    if( DoInstructionParseTest )
        TestInstructionParse();

    if( DoConstGenTest )
        TestConstantGenerator();

    return 0;
}











#endif
