#ifndef VM_Stage_EX
#define VM_Stage_EX

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"
#include "ALU.cpp"

namespace Stage
{

    class EX : public Component::Component
    {
        private:

            Component::ALU Alu;

        public:

            HwError Setup() override
            {
                SetLastHwError( HwError_NoError );

                HwError result = Alu.Setup();

                return result;
            }

            Component::Component_ID GetID() override
            {
                return Component_ID_StageEX;
            }

            Pipeline_EXtoMEM Perform( Pipeline_IDtoEX pipe )
            {
                SetLastHwError( HwError_NoError );

                Pipeline_EXtoMEM result;
                result.Instruction = pipe.Instruction;

                result.DoMemRead = pipe.DoMemRead;
                result.DoMemWrite = pipe.DoMemWrite;
                result.MemSize = pipe.MemSize;
                result.WriteVal = pipe.WriteVal; 

                result.DoWB = pipe.DoWB;
                result.Reg = pipe.Reg;
                result.Val = pipe.Val;

                // Handle PC.

                if( pipe.AddToPC )
                {
                    result.PCVal = pipe.PCVal + pipe.Immediate;
                    result.ReplacePC = 1;
                }
                else if( pipe.ReplacePC )
                {
                    result.PCVal = pipe.RegVal1 + pipe.Immediate;
                    result.ReplacePC = 1;
                }
                else
                {
                    result.PCVal = pipe.PCVal; // Could omit this line since nothing would be done with it.
                    result.ReplacePC = 0;
                }

                // Handle ALU

                result.Address = 0;
                if( pipe.DoALUOp )
                {
                    uint64_t Source2 = ( pipe.UseImmNotReg2 ? pipe.Immediate : pipe.RegVal2 );
                    uint64_t ALUResult = Alu.DoALUOp( pipe.ALUOp, pipe.RegVal1, Source2, (bool) pipe.InvertALUOp );
                    if( pipe.ResultType )
                    {
                        result.Val = ALUResult;
                    }
                    else
                    {
                        result.Address = ALUResult;
                    }
                    SetLastHwError( Alu.LastHwError() );
                }

                return result;
            }



    };

};




#endif
