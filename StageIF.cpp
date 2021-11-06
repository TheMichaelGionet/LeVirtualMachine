#ifndef VM_Stage_IF
#define VM_Stage_IF

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"
#include "Memory.cpp"
#include "ControlUnit.cpp"

namespace Stage
{

    class IF : public Component::Component_t
    {
        private:

            uint64_t PCReg;

            Component::MemoryAccess * leMem;
            Component::ControlUnit leControl;

        public:

            IF( Component::MemoryAccess * newMem )
            {
                PCReg = 0;
                leMem = newMem;
            }

            HwError Setup() override
            {
                HwError result = HwError_NoError;

                result = leControl.Setup();

                return result;
            }

            void ReplacePC( uint64_t NewVal )
            {
                PCReg = NewVal;
            }

            Stage::Pipeline_IFtoID Perform( bool DoReplacePC, uint64_t NewVal )
            {
                SetLastHwError( HwError_NoError );

                if( DoReplacePC )
                    PCReg = NewVal;

                uint32_t NextInstruction = leMem->GetWord( PCReg );;

                Stage::Pipeline_IFtoID result = leControl.Perform( NextInstruction, PCReg );

                HwError leError = leControl.LastHwError();

                SetLastHwError( leError );

                PCReg += 4;

                return result;
            }

            uint64_t GetPCReg()
            {
                return PCReg;
            }

    };

};

#endif
