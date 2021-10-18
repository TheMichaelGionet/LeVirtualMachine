#ifndef VM_Stage_ID
#define VM_Stage_ID

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"
#include "RegisterFile.cpp"
#include "ConstantGenerator.hpp"

namespace Stage
{

    class ID : public Component::Component
    {
        private:
    
            Component::IntegerRegisterFile RegF;
            Component::ConstantGenerator Gen;
    
        public:
    
            Component::Component_ID GetID() override
            {
                return Component::Component_ID_StageID;
            }
    
            HwError Setup() override
            {
                HwError result = RegF.Setup();
                if( result != HwError_NoError )
                    return result;

                result = Gen.Setup();
                return result;
            }

            Pipeline_IDtoEX Perform( Pipeline_IFtoID pipe, Instruction::ParsedInstruction instr )
            {
                SetLastHwError( HwError_NoError );

                Pipeline_IDtoEX result;

                // Copy over the unused stuff;

                result.Instruction = pipe.Instruction;

                result.DoALUOp = pipe.DoALUOp;
                result.ALUOp = pipe.ALUOp;
                result.InvertALUOp = pipe.InvertALUOp;
                result.UseImmNotReg2 = pipe.UseImmNotReg2;
                result.ResultType = pipe.ResultType;

                result.AddToPC = pipe.AddToPC;
                result.ReplacePC = pipe.ReplacePC;
                result.PCVal = pipe.PCVal;

                result.DoMemRead = pipe.DoMemRead;
                result.DoMemWrite = pipe.DoMemWrite;
                result.MemSize = pipe.MemSize;
                
                result.DoWB = pipe.DoWB;
                result.Reg = pipe.Reg;

                // Generate constant

                uint64_t leConst = Gen.GenerateConst( instr );
                result.Immediate = leConst;

                SetLastHwError( Gen.LastHwError() );
                if( LastHwError() != HwError_NoError )
                {
                    result.RegVal1 = 0;
                    result.RegVal2 = 0;
                    result.WriteVal = 0;

                    return result;
                }

                // Read Registers

                if( pipe.ReadRegisters & 0b01 )
                {
                    result.RegVal1 = RegF.GetVal( pipe.ReadReg1 );
                    SetLastHwError( RegF.LastHwError() );
                    if( LastHwError() != HwError_NoError )
                    {
                        result.RegVal2 = 0;
                        result.WriteVal = 0;

                        return result;
                    }
                }
                else
                    result.RegVal1 = 0;

                if( pipe.ReadRegisters & 0b10 )
                {
                    uint64_t RegVal2 = RegF.GetVal( pipe.ReadReg2 );
                    result.RegVal2 = RegVal2;
                    result.WriteVal = RegVal2;
                    SetLastHwError( RegF.LastHwError() );
                    if( LastHwError() != HwError_NoError )
                        return result;
                }

                return result;
            }
    
    
    };


};

#endif
