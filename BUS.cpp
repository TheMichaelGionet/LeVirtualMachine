#ifndef VM_Component_BUS
#define VM_Component_BUS

#ifndef MODE
#define Mode_Release 1
#define Mode_Debug 2

#define MODE Mode_Release

#endif

#if MODE == Mode_Debug
#include <stdio.h>
#endif

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Memory.cpp"

namespace Component
{
    const uint64_t MaxNumPeripherals = 256;

    class Peripheral : public MemoryAccess
    {
        protected:
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

    };

    class BUS : public MemoryAccess
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

            BUS()
            {
                ZeroPeripherals();
            }

            Component_ID GetID() override
            {
                return Component_ID_Bus;
            }

            void AddPeripheral( Peripheral * newPeripheral, uint64_t slot )
            {
                this->SetLastHwError( HwError_NoError );
                if( slot < MaxNumPeripherals )
                    lePeripherals[slot] = newPeripheral;
                else
                    this->SetLastHwError( HwError_PeripheralOutOfRange );
            }

            uint8_t GetByte( uint64_t index ) override
            {
                this->SetLastHwError( HwError_NoError );

                uint64_t iterator;
                for( iterator = 0; iterator < MaxNumPeripherals; iterator++ )
                {
                    if( (lePeripherals[iterator] != NULL) && (lePeripherals[iterator]->CheckAccessibility( index, sizeof(uint8_t) ) ) )
                    {
                        #if MODE == Mode_Debug
                        printf( "Accessed peripheral %ld for get byte at location %ld\n", iterator, index );
                        #endif
                        uint8_t result = lePeripherals[iterator]->GetByte(index);
                        this->SetLastHwError( lePeripherals[iterator]->LastHwError() );
                        return result;
                    }
                }

                SetLastHwError( HwError_UnknownAddress );
                return 0;
            }

            void SetByte( uint64_t index, uint8_t val ) override
            {
                this->SetLastHwError( HwError_NoError );

                uint64_t iterator;
                for( iterator = 0; iterator < MaxNumPeripherals; iterator++ )
                {
                    if( lePeripherals[iterator] != NULL )
                    if( lePeripherals[iterator]->CheckAccessibility( index, sizeof(uint8_t) ) )
                    {
                        #if MODE == Mode_Debug
                        printf( "Accessed peripheral %ld for set byte at location %ld\n", iterator, index );
                        #endif
                        lePeripherals[iterator]->SetByte( index, val );
                        this->SetLastHwError( lePeripherals[iterator]->LastHwError() );
                        return;
                    }
                }

                SetLastHwError( HwError_UnknownAddress );
            }

            // In this case, accessibility requires that the whole space is owned by a single peripheral.
            bool CheckAccessibility( uint64_t index, uint64_t size ) override
            {
                this->SetLastHwError( HwError_NoError );

                uint64_t iterator;
                for( iterator = 0; iterator < MaxNumPeripherals; iterator++ )
                {
                    if( lePeripherals[iterator] != nullptr )
                    if( lePeripherals[iterator]->CheckAccessibility( index, size ) )
                        return true;
                }

                return false;
            }

    };


};


#endif
