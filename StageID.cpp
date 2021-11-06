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

    class ID : public Component::Component_t
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

                result.CopyFromLast( pipe );

                uint64_t leConst = 0;
                uint64_t RegVal1 = 0;
                uint64_t RegVal2 = 0;
                uint64_t PCVal = pipe.PCCalc.PCVal;

                // Generate constant

                if( pipe.IDParams.DoGenConst )
                    leConst = Gen.GenerateConst( instr, pipe.IDParams.SignExtend );

                SetLastHwError( Gen.LastHwError() );
                if( LastHwError() != HwError_NoError )
                {
                    result.SetALUInputs( RegVal1, RegVal2, leConst, PCVal );
                    result.SetMemParam( RegVal2 );

                    return result;
                }

                // Read Registers

                if( pipe.IDParams.ReadRegisters & 0b01 )
                {
                    RegVal1 = RegF.GetVal( pipe.IDParams.ReadReg1 );
                    SetLastHwError( RegF.LastHwError() );
                    if( LastHwError() != HwError_NoError )
                    {
                        result.SetALUInputs( RegVal1, RegVal2, leConst, PCVal );
                        result.SetMemParam( RegVal2 );
                        return result;
                    }
                }
                else
                    RegVal1 = 0;

                if( pipe.IDParams.ReadRegisters & 0b10 )
                {
                    RegVal2 = RegF.GetVal( pipe.IDParams.ReadReg2 );
                    SetLastHwError( RegF.LastHwError() );
                    if( LastHwError() != HwError_NoError )
                    {
                        result.SetALUInputs( RegVal1, RegVal2, leConst, PCVal );
                        result.SetMemParam( RegVal2 );
                        return result;
                    }
                }

                result.SetALUInputs( RegVal1, RegVal2, leConst, PCVal );
                result.SetMemParam( RegVal2 );

                return result;
            }


        Component::IntegerRegisterFile * GetRegFForWB()
        {
            return &RegF;
        }
    
    };


};

#endif
