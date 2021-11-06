
#ifndef VM_Component
#define VM_Component

#include "HwError.hpp"

namespace Component
{
    typedef enum
    {
        Component_ID_BaseComponent = 0,
        Component_ID_ALU,
        Component_ID_FPU,
        Component_ID_IntRegisterFile,
        Component_ID_FloatRegisterFile,
        Component_ID_RawMem,
        Component_ID_Bus,
        Component_ID_InstructionDecoder,
        Component_ID_InstructionExecuter,
        Component_ID_ConstantGenerator,

        Component_ID_StageIF,
        Component_ID_StageID,
        Component_ID_StageEX,
        Component_ID_StageMEM,
        Component_ID_StageWB,

        Component_ID_Printer,
        Component_ID_Power,
        Component_ID_MemoryUnit,

    } Component_ID;


    class Component_t
    {
        private:
            HwError lastHwError = HwError_NoError;

        protected:
            void SetLastHwError( HwError newError )
            {
                lastHwError = newError;
            }

        public:
            virtual Component_ID GetID()
            {
                return Component_ID_BaseComponent;
            }

            virtual HwError Setup()
            {
                SetLastHwError( HwError_NoError );
                return HwError_NoError;
            }

            HwError LastHwError()
            {
                return lastHwError;
            }

    };

};



#endif
