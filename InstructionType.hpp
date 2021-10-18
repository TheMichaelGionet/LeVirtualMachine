#ifndef VM_Instruction_InstructionType
#define VM_Instruction_InstructionType

#include <stdint.h>
#include "HwError.hpp"

namespace Instruction
{

    const unsigned int NumInstructionTypes = 7;
    const unsigned int NumInstructions = 1<<7;

    typedef enum
    {
        InstructionType_Null = 0,
        InstructionType_R,
        InstructionType_I,
        InstructionType_S,
        InstructionType_SB,
        InstructionType_U,
        InstructionType_UJ
    } InstructionType;


    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint32_t garb : 25;
    } ParsedIType_Null;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t rd : 5;
        uint8_t funct3 : 3;
        uint8_t rs1 : 5;
        uint8_t rs2 : 5;
        uint8_t funct7 : 7;
    } ParsedIType_R;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t rd : 5;
        uint8_t funct3 : 3;
        uint8_t rs1 : 5;
        uint16_t imm0to11 : 12;
    } ParsedIType_I;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t imm0to4 : 5;
        uint8_t funct3 : 3;
        uint8_t rs1 : 5;
        uint8_t rs2 : 5;
        uint8_t imm5to11 : 7;
    } ParsedIType_S;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t imm11and1to4 : 5;
        uint8_t funct3 : 3;
        uint8_t rs1 : 5;
        uint8_t rs2 : 5;
        uint8_t imm5to10and12 : 7;
    } ParsedIType_SB;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t rd : 5;
        uint32_t imm12to31 : 20;
    } ParsedIType_U;

    typedef struct __attribute__ ((__packed__))
    {
        uint8_t opcode : 7;
        uint8_t rd : 5;
        uint32_t imm12to19and11and1to10and20 : 20;
    } ParsedIType_UJ;

    typedef union
    {
        ParsedIType_Null Null;
        ParsedIType_R R;
        ParsedIType_I I;
        ParsedIType_S S;
        ParsedIType_SB SB;
        ParsedIType_U U;
        ParsedIType_UJ UJ;
    } ParsedIType;

    typedef struct __attribute__ ((__packed__))
    {
        ParsedIType ParsedInst;
        InstructionType Type;
    } ParsedInstruction;

    class InstructionParser
    {
        private:
            HwError LastHwError;
            InstructionType ITypeArray[NumInstructions];

            ParsedIType_Null ParseInstructionNull( uint32_t instruction )
            {
                ParsedIType_Null result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_R ParseInstructionR( uint32_t instruction )
            {
                ParsedIType_R result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_I ParseInstructionI( uint32_t instruction )
            {
                ParsedIType_I result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_S ParseInstructionS( uint32_t instruction )
            {
                ParsedIType_S result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_SB ParseInstructionSB( uint32_t instruction )
            {
                ParsedIType_SB result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_U ParseInstructionU( uint32_t instruction )
            {
                ParsedIType_U result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

            ParsedIType_UJ ParseInstructionUJ( uint32_t instruction )
            {
                ParsedIType_UJ result;
                *(uint32_t*)(&result) = instruction;
                return result;
            }

        public:
            InstructionParser()
            {
                uint8_t iterator;
                for( iterator = 0; iterator < NumInstructions; iterator++ )
                {
                    ITypeArray[iterator] = InstructionType_Null;
                }

                // loads
                ITypeArray[ 0b0000011 ] = InstructionType_I;

                // imm ALU op 64 bit.
                ITypeArray[ 0b0010011 ] = InstructionType_I;

                // AUIPC
                ITypeArray[ 0b0010111 ] = InstructionType_U;

                // imm ALU op 32 bit.
                ITypeArray[ 0b0011011 ] = InstructionType_I;

                // stores
                ITypeArray[ 0b0100011 ] = InstructionType_S;

                // reg ALU op 64 bit.
                ITypeArray[ 0b0110011 ] = InstructionType_R;

                // LUI
                ITypeArray[ 0b0110111 ] = InstructionType_U;

                // reg ALU op 32 bit.
                ITypeArray[ 0b0111011 ] = InstructionType_R;

                // branch
                ITypeArray[ 0b1100011 ] = InstructionType_SB;

                // jalr
                ITypeArray[ 0b1100111 ] = InstructionType_I;

                // jal
                ITypeArray[ 0b1101111 ] = InstructionType_UJ;

                // environment, CSR
                ITypeArray[ 0b1110011 ] = InstructionType_I;
            }

            HwError GetLastHwError()
            {
                return LastHwError;
            }

            InstructionType GetType( uint32_t instruction )
            {
                LastHwError = HwError_NoError;
                uint8_t OpCode = instruction & 0b01111111;
                return ITypeArray[OpCode];
            }

            ParsedInstruction ParseInstruction( uint32_t instruction )
            {
                static void* DispatchTable[NumInstructionTypes] = 
                {
                    &&NullInstruction, &&RInstruction, &&IInstruction, &&SInstruction, &&SBInstruction, &&UInstruction, &&UJInstruction
                };
                LastHwError = HwError_NoError;

                ParsedInstruction result;
                result.Type = GetType( instruction );

                uint8_t jumpOffset = (uint8_t) result.Type;

                if( jumpOffset < NumInstructionTypes )
                    goto *DispatchTable[jumpOffset];
                else
                    goto BadInstruction;

                NullInstruction:
                {
                    result.ParsedInst.Null = ParseInstructionNull( instruction );
                    return result;
                };

                RInstruction:
                {
                    result.ParsedInst.R = ParseInstructionR( instruction );
                    return result;
                };

                IInstruction:
                {
                    result.ParsedInst.I = ParseInstructionI( instruction );
                    return result;
                };

                SInstruction:
                {
                    result.ParsedInst.S = ParseInstructionS( instruction );
                    return result;
                };

                SBInstruction:
                {
                    result.ParsedInst.SB = ParseInstructionSB( instruction );
                    return result;
                };

                UInstruction:
                {
                    result.ParsedInst.U = ParseInstructionU( instruction );
                    return result;
                };

                UJInstruction:
                {
                    result.ParsedInst.UJ = ParseInstructionUJ( instruction );
                    return result;
                };

                BadInstruction:
                {
                    result.ParsedInst.Null = ParseInstructionNull( instruction );
                    LastHwError = HwError_UnknownInstructionType;
                    return result;
                };

            }

    };

};




#endif
