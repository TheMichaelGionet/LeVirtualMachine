#ifndef VM_Component_MemoryAccess
#define VM_Component_MemoryAccess

//
// Include this file whenever we want to create a BUS (and peripherals), so that it can use the MemoryAccess class
//

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"


#include <exception>

namespace Component
{

    // The way this works is fucky because it should work regardless of the endianness of the host machine.
    // The faster variant for little endian systems is trivial.

    // WARNING: With the current system, if the index is about to overflow, then it will read/write without setting LastHwError, so it will succeed in spite of it trying to access non-memory indicies.
    //          A similar thing happens when the index is about to go out of bounds. It will read/write successfully and then display an error.
    //
    //          A potential fix is to check to make sure the index is <= Size - SizeOfElement, since this would guarantee that only memory elements can be accessed.
    //
    // Update: Added the check to see if the spot in memory is even accessible. This should be called before any access.
    class MemoryAccess
    {
        public:
            
            virtual bool CheckAccessibility( uint64_t index, uint64_t size ) = 0;

            virtual uint8_t GetByte( uint64_t index ) = 0;
            uint16_t GetHWord( uint64_t index )
            {
                uint16_t bottomHalf = GetByte( index );
                uint16_t upperHalf = GetByte( index + 1 );
                return bottomHalf | ( upperHalf << 8 );
            }
            uint32_t GetWord( uint64_t index )
            {
                uint32_t bottomHalf = GetHWord( index );
                uint32_t upperHalf = GetHWord( index + 2 );
                return bottomHalf | ( upperHalf << 16 );
            }
            uint64_t GetDWord( uint64_t index )
            {
                uint64_t bottomHalf = GetWord( index );
                uint64_t upperHalf = GetWord( index + 4 );
                return bottomHalf | ( upperHalf << 32 );
            }

            virtual void SetByte( uint64_t index, uint8_t val ) = 0;
            void SetHWord( uint64_t index, uint16_t val )
            {
                uint16_t bottomHalf = val & 0xff;
                uint16_t upperHalf = (val >> 8) & 0xff;
                SetByte( index, (uint8_t) bottomHalf );
                SetByte( index + 1, (uint8_t) upperHalf );
            }
            void SetWord( uint64_t index, uint32_t val )
            {
                uint32_t bottomHalf = val & 0xffff;
                uint32_t upperHalf = (val >> 16) & 0xffff;
                SetHWord( index, (uint16_t) bottomHalf );
                SetHWord( index + 2, (uint16_t) upperHalf );
            }
            void SetDWord( uint64_t index, uint64_t val )
            {
                uint64_t bottomHalf = val & 0xffffffff;
                uint64_t upperHalf = (val >> 32) & 0xffffffff;
                SetWord( index, (uint32_t) bottomHalf );
                SetWord( index + 4, (uint32_t) upperHalf );
            }
    };

    // ========================================================================================================
    //
    //
    // Raw Memory and related.
    //
    //
    // ========================================================================================================


    // Setup() depends on whether this is running on the raw hardware or not.
    // Setup, the constructor and destructor all depend on this.
    class RawMemoryBase : public Component, public MemoryAccess
    {
        protected:
            uint8_t * Memory;
            uint64_t MemorySize;

        public:

            Component_ID GetID() override
            {
                return Component_ID_RawMem;
            }

            uint8_t GetByte( uint64_t index ) override
            {
                SetLastHwError( HwError_NoError );
                if( index < MemorySize )
                    return Memory[index];
                SetLastHwError( HwError_MemoryIndexOutOfRange );
                return 0;
            }

            void SetByte( uint64_t index, uint8_t val ) override
            {
                SetLastHwError( HwError_NoError );
                if( index < MemorySize )
                    Memory[index] = val;
                else
                    SetLastHwError( HwError_MemoryIndexOutOfRange );
            }

            bool CheckAccessibility( uint64_t index, uint64_t size ) override
            {
                // Check for right hand and then left hand spills. Return true if good.
                return (index + size <= MemorySize) && ( index < MemorySize );
            }

    };

    // ==================================================================================
    //
    // Raw Memory Instances
    //
    // ==================================================================================

    // Use when there is an underlying OS that can handle allocation.
    class RawMemoryOnOS : public RawMemoryBase
    {
        private:
            uint64_t _newMemorySize;

        public:
            RawMemoryOnOS( uint64_t newMemorySize )
            {
                _newMemorySize = newMemorySize;
                // Done this way so that allocations happen on the setup instead of on the construction.
                this->Memory = nullptr;
                this->MemorySize = 0;
            }

            HwError Setup() override
            {
                SetLastHwError( HwError_NoError );

                try
                {
                    this->Memory = new uint8_t[_newMemorySize];
                    this->MemorySize = _newMemorySize;
                }
                catch( std::bad_alloc& ba )
                {
                    SetLastHwError( HwError_CannotAllocateSpace );
                }

                return LastHwError();

            }

            ~RawMemoryOnOS()
            {
                if( this->Memory != nullptr )
                {
                    delete[] this->Memory;
                    this->Memory = nullptr;
                }
                this->MemorySize = 0;
            }

    };





};






#endif
