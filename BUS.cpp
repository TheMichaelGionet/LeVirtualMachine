#ifndef VM_Component_BUS
#define VM_Component_BUS

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Memory.cpp"

// NOTICE: ON HOLD until the processor is finished. The core is more important right now than the rest of the VM. 
// USE RAW MEMORY for now in the first version.

namespace Component
{
    class Peripheral : public MemoryAccess, public Component
    {
        private:
            uint64_t startLocation;
            uint64_t lengthLocation;

        protected:
            void SetRange( uint64_t start, uint64_t length )
            {
                startLocation = start;
                lengthLocation = length;
            }

        public:

        

    }

    class BUS : public Component, public MemoryAccess
    {
        private:
            

        public:


    };

};


#endif
