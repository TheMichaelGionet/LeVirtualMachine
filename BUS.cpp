#ifndef VM_Component_BUS
#define VM_Component_BUS

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Memory.cpp"

// NOTICE: ON HOLD until the processor is finished. The core is more important right now than the rest of the VM. 
// USE RAW MEMORY for now in the first version.

// Update: No! I'm using memory mapped IO for the console and machine controls.

namespace Component
{
    const uint64_t MaxNumPeripherals = 256;

    class Peripheral : public MemoryAccess, public Component_t
    {
        private:
            uint64_t startLocation;
            uint64_t lengthLocation;

            bool CheckAddressInRange( uint64_t index )
            {
                return ( index >= startLocation && ( index - startLocation < lengthLocation ) );
            }

            uint64_t TranslateAddress( uint64_t Address )
            {
                return Address - startLocation;
            }

        public:

            bool CheckAccessibility( uint64_t index, uint64_t size ) override
            {
                return CheckAddressInRange( index ) && CheckAddressInRange( index + size - 1 );
            }

            void SetRange( uint64_t start, uint64_t length )
            {
                startLocation = start;
                lengthLocation = length;
            }

            

            

        

            

    }

    class BUS : public Component_t, public MemoryAccess
    {
        private:
            Peripheral * lePeripherals[MaxNumPeripherals];

            void ZeroPeripherals()
            {
                uint64_t iterator;
                for( iterator = 0; iterator < MaxNumPeripherals; iterator++ )
                {
                    lePeripherals[iterator] = NULL;
                }
            }

        public:

            HwError AddPeripheral( Peripheral * newPeripheral,  )
            {
                
            }

    };


};


#endif
