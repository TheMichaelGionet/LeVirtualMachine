#ifndef VM_Stage_WB
#define VM_Stage_WB

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"
#include "RegisterFile.cpp"

namespace Stage
{

    class WB : public Component::Component
    {
        private:
            Component::IntegerRegisterFile * RegF = nullptr; // this lives in the ID stage, but this is the only way to access it from here.

        public:
            WB( IntegerRegisterFile * regf )
            {
                RegF = regf;
            }

            Component::Component_ID GetID()
            {
                return Component::Component_ID_StageWB;
            }

            void Perform( Pipeline_MEMtoWB pipe )
            {
                SetLastHwError( HwError_NoError );
                if( pipe.DoWB == 1 )
                {
                    if( RegF != nullptr )
                    {
                        RegF->SetVal( pipe.Reg, pipe.Val );
                        SetLastHwError( RegF->LastHwError() );
                    }
                    else
                        SetLastHwError( HwError_RegisterFileMissing );
                }
            }

    };

};





#endif
