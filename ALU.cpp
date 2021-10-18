
#ifndef VM_Component_ALU
#define VM_Component_ALU

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"

namespace Component
{

    class ALU : public Component
    {
        public:
            typedef enum
            {
                ALUOp_AddOrSub = 0b000,
                ALUOp_ShiftLeft = 0b001,
                ALUOp_SetLessThan = 0b010,
                ALUOp_SetLessThanUnsigned = 0b011,
                ALUOp_Xor = 0b100,
                ALUOp_ShiftRightOrArith = 0b101,
                ALUOp_Or = 0b110,
                ALUOp_And = 0b111
                
            } ALUOp;

            Component_ID GetID() override
            {
                return Component_ID_ALU;
            }

			uint64_t DoALUOp( uint8_t Op, uint64_t r1, uint64_t r2, bool invertOp )
			{
				return DoALUOp( (ALUOp) Op, r1, r2, invertOp );
			}

            uint64_t DoALUOp( ALUOp Op, uint64_t r1, uint64_t r2, bool invertOp )
            {
                static void* Pointers[] = { &&AddOrSub, &&ShiftLeft, &&SetLessThan, &&SetLessThanUnsigned, &&OP_XOR, &&ShiftRightOrArithmetic, &&OP_OR, &&OP_AND };
                SetLastHwError( HwError_NoError );

                uint8_t OpOffset = (uint8_t) Op;

                if( OpOffset <= 0b111 )
                    goto *Pointers[OpOffset];
                else
                    goto BADINSTRUCTION;

                // 0b000
                AddOrSub:
                {
                    if( invertOp )
                        r2 = -r2;
                    
                    return r1 + r2;
                }
                // 0b001
                ShiftLeft:
                {
                    return (r2 < 64) ? (r1 << r2) : 0;
                }
                // 0b010
                SetLessThan:
                {
                    int64_t sr1 = (int64_t) r1;
                    int64_t sr2 = (int64_t) r2;
                    return (sr1 < sr2) ? 1 : 0;
                }
                // 0b011
                SetLessThanUnsigned:
                {
                    return (r1 < r2) ? 1 : 0;
                }
                // 0b100
                OP_XOR:
                {
                    return r1 ^ r2;
                }
                // 0b101
                ShiftRightOrArithmetic:
                {
                    bool bit = ( r1 >> 63 & 1 ) != 0;
                    uint64_t OnlyOnes = -1;
                    uint64_t rawShift = r1 >> r2;

                    if( invertOp && bit )
                    {
                        return rawShift | (OnlyOnes << (64 - r2) );
                    }
                    else
                    {
                        return rawShift & (OnlyOnes >> r2);
                    }
                }
                // 0b110
                OP_OR:
                {
                    return r1 | r2;
                }
                // 0b111
                OP_AND:
                {
                    return r1 & r2;
                }

                BADINSTRUCTION:
                {
                    SetLastHwError( HwError_InvalidAluOp );

                    return -1;
                }

            }

    };
};





#endif
