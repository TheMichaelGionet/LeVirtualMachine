#ifndef VM_Stage_EX
#define VM_Stage_EX

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"
#include "ALU.cpp"

namespace Stage
{

    class EX : public Component::Component_t
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
                return Component::Component_ID_StageEX;
            }

            Pipeline_EXtoMEM Perform( Pipeline_IDtoEX pipe )
            {
                SetLastHwError( HwError_NoError );

                Pipeline_EXtoMEM Result;

                Result.CopyFromLast( pipe );

                // Handle ALU

                uint64_t ALUResult = 0;

                Result.SetMemAddress(0);
                if( pipe.ALUInputs.DoALUOp )
                {
                    uint64_t Source2 = ( pipe.ALUInputs.UseImmNotReg2 ? pipe.ALUInputs.Immediate : pipe.ALUInputs.RegVal2 );
                    uint64_t Source1 = ( pipe.ALUInputs.UsePCRegister ? pipe.ALUInputs.PCVal : pipe.ALUInputs.RegVal1 );
                    ALUResult = Alu.DoALUOp( pipe.ALUInputs.ALUOp, Source1, Source2, (bool) pipe.ALUInputs.InvertALUOp );

                    if( pipe.ALUInputs.AddFourToResult )
                        ALUResult += 4;

                    if( pipe.ALUInputs.ResultType )
                    {
                        Result.SetWBVal( ALUResult );
                    }
                    else
                    {
                        Result.SetMemAddress( ALUResult );
                    }
                    SetLastHwError( Alu.LastHwError() );
                }

                // Handle PC.

                bool UpdatePC = !pipe.PCCalc.IsConditional;

                if( pipe.PCCalc.IsConditional )
                {
                    UpdatePC = ALUResult != 0;
                    if( pipe.PCCalc.InvertALUOutput )
                        UpdatePC = !UpdatePC;
                }

                if( UpdatePC )
                {
                    if( pipe.PCCalc.AddToPC )
                    {
                        Result.SetPCVal( 1, pipe.PCCalc.PCVal + pipe.ALUInputs.Immediate );
                    }
                    else if( pipe.PCCalc.ReplacePC )
                    {
                        Result.SetPCVal( 1, pipe.ALUInputs.RegVal1 + pipe.ALUInputs.Immediate );
                    }
                    else
                    {
                        Result.SetPCVal( 0, pipe.ALUInputs.PCVal ); // This can be omitted.
                    }
                }
                else
                    Result.SetPCVal( 0, pipe.ALUInputs.PCVal );

                return Result;
            }


    };

};




#endif
