#ifndef VM_Stage_IF
#define VM_Stage_IF

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"
#include "ControlUnit.hpp"

namespace Stage
{

    class IF : public Component::Component_t
    {
        private:

            uint64_t PCReg;

        public:

            IF()
            {
                PCReg = 0;
            }

    };

};

#endif
