#ifndef VM_Component_ConstantGenerator
#define VM_Component_ConstantGenerator

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"

namespace Component
{

    class ConstantGenerator : public Component_t
    {

        private:

            uint64_t GenerateConst( Instruction::ParsedIType_Null instruction, bool SignExtend )
            {
                return instruction.garb;
            }

            uint64_t GenerateConst( Instruction::ParsedIType_R instruction, bool SignExtend )
            {
                // No constant to generate
                return 0;
            }

            uint64_t GenerateConst( Instruction::ParsedIType_I instruction, bool SignExtend )
            {
                uint8_t signBit = instruction.imm0to11 >> 11;

                uint64_t result = instruction.imm0to11;

                if( SignExtend && (signBit != 0) )
                {
                    result |= ~(uint64_t)0xfff;
                }

                return result;
            }

            uint64_t GenerateConst( Instruction::ParsedIType_S instruction, bool SignExtend )
            {
                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm0to4 : 5;
                    uint8_t imm5to11 : 7;
                    uint64_t imm12to63 : 52;
                } intermS;

                intermS interm = { instruction.imm0to4, instruction.imm5to11, 0 };
                uint64_t result = *(uint64_t*) &interm;

                uint8_t signBit = (uint8_t) (result >> 11);

                if( SignExtend && (signBit != 0) )
                {
                    result |= ~(uint64_t)0xfff;
                }

                return result;
            }

            uint64_t GenerateConst( Instruction::ParsedIType_SB instruction, bool SignExtend )
            {
                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm11 : 1;
                    uint8_t imm1to4 : 4;
                } intermSBdecomp1;

                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm5to10 : 6;
                    uint8_t imm12 : 1;
                } intermSBdecomp2;

                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm0 : 1;
                    uint8_t imm1to4 : 4;
                    uint8_t imm5to10 : 6;
                    uint8_t imm11 : 1;
                    uint8_t imm12 : 1;
                    uint64_t imm13to63 : 51;
                } intermSBtogether;

                uint8_t imm11and1to4 = instruction.imm11and1to4;
                uint8_t imm5to10and12 = instruction.imm5to10and12;

                intermSBdecomp1 dec1 = *( intermSBdecomp1* ) &imm11and1to4;
                intermSBdecomp2 dec2 = *( intermSBdecomp2* ) &imm5to10and12;

                intermSBtogether result = { 0, dec1.imm1to4, dec2.imm5to10, dec1.imm11, dec2.imm12, 0 };

                uint64_t signExtendedResult = *(uint64_t*) &result;

                uint8_t signBit = signExtendedResult >> 12;

                if( SignExtend && (signBit != 0) )
                {
                    signExtendedResult |= ~(uint64_t)0xfff;
                }

                return signExtendedResult;
            }

            uint64_t GenerateConst( Instruction::ParsedIType_U instruction, bool SignExtend )
            {
                uint64_t result = ((uint64_t) instruction.imm12to31) << 12;

                uint8_t signBit = (uint8_t) (result >> 31);

                if( SignExtend && (signBit != 0) )
                {
                    result |= ~(uint64_t)0xffffffff;
                }

                return result;

            }

            uint64_t GenerateConst( Instruction::ParsedIType_UJ instruction, bool SignExtend )
            {
                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm12to19 : 8;
                    uint8_t imm11 : 1;
                    uint16_t imm1to10 : 10;
                    uint8_t imm20 : 1;
                    uint16_t imm21to32 : 12; // 0 padding
                } intermUJdecomp;

                typedef struct __attribute__ ((__packed__))
                {
                    uint8_t imm0 : 1;
                    uint16_t imm1to10 : 10;
                    uint8_t imm11 : 1;
                    uint8_t imm12to19 : 8;
                    uint8_t imm20 : 1;
                    uint64_t imm21to63 : 43; // 0 padding
                } intermUJtogether;

                uint32_t imm12to19and11and1to10and20 = instruction.imm12to19and11and1to10and20;
                

                intermUJdecomp decomp = *(intermUJdecomp*) &imm12to19and11and1to10and20;
                intermUJtogether result = { 0, decomp.imm1to10, decomp.imm11, decomp.imm12to19, decomp.imm20 };

                uint64_t signExtendedResult = *(uint64_t*) &result;

                uint8_t signBit = signExtendedResult >> 20;

                if( SignExtend && (signBit != 0) )
                {
                    signExtendedResult |= ~(uint64_t)0xfffff;
                }

                return signExtendedResult;
            }

        public:
            Component_ID GetID() override
            {
                return Component_ID_ConstantGenerator;
            }

            uint64_t GenerateConst( Instruction::ParsedInstruction instr, bool SignExtend )
            {
                static void * DispatchTable[] = { &&Case_Null, &&Case_R, &&Case_I, &&Case_S, &&Case_SB, &&Case_U, &&Case_UJ };

                SetLastHwError( HwError_NoError );

                uint8_t instrTypeOffset = (uint8_t) instr.Type;

                if( instrTypeOffset < Instruction::NumInstructionTypes )
                    goto *DispatchTable[instrTypeOffset];
                else
                    goto Case_Bad;

                Case_Null:
                {
                    return GenerateConst( instr.ParsedInst.Null, SignExtend );
                };

                Case_R:
                {
                    return GenerateConst( instr.ParsedInst.R, SignExtend );
                };

                Case_I:
                {
                    return GenerateConst( instr.ParsedInst.I, SignExtend );
                };

                Case_S:
                {
                    return GenerateConst( instr.ParsedInst.S, SignExtend );
                };

                Case_SB:
                {
                    return GenerateConst( instr.ParsedInst.SB, SignExtend );
                };

                Case_U:
                {
                    return GenerateConst( instr.ParsedInst.U, SignExtend );
                };

                Case_UJ:
                {
                    return GenerateConst( instr.ParsedInst.UJ, SignExtend );
                };

                Case_Bad:
                {
                    SetLastHwError( HwError_UnknownInstructionType );
                    return 0;
                };

            }

    };


};




#endif
