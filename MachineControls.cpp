#ifndef VM_Machine_Controls
#define VM_Machine_Controls

#include <stdint.h>
#include "Component.hpp"
#include "Memory.cpp"
#include "BUS.cpp"
#include <stdio.h>


namespace Component
{

    class Printer : public Peripheral
    {
        private:

            uint64_t LastIntInput;
            uint64_t LastHexInput;

            bool IsInRange( uint64_t x, uint64_t start, uint64_t length )
            {
                return ( x >= start ) && ( x - start < length );
            }

        public:

            Printer()
            {
                LastIntInput = 0;
                LastHexInput = 0;
            }

            Component_ID GetID() override
            {
                return Component_ID_Printer;
            }

            void PrintStartupMessage()
            {
                // TODO: Biggify my name a lot
                printf( "This Virtual Machine was made by Michael Gionet!\n" );
            }

            uint8_t GetByte( uint64_t index ) override
            {
                SetLastHwError( HwError_UnknownAddress );
                return 0;
            }

            void SetByte( uint64_t index, uint8_t val ) override
            {
                SetLastHwError( HwError_NoError );
                uint64_t offset = index - this->startLocation;

                // print startup message
                if( offset == 0 )
                {
                    PrintStartupMessage();
                    return;
                }
                // print character ( offset == 1 )
                else if( offset == 1 )
                {
                    printf( "%c", (char)val );
                }
                // print integer value (offset == 2)
                else if( IsInRange( offset, 2, sizeof(uint64_t) ) )
                {
                    uint64_t offset2 = offset - 2;

                    ((uint8_t*)&LastIntInput)[offset2] = val;

                    if( offset2 == 0 )
                    {
                        printf( "%ld", LastIntInput );
                        LastIntInput = 0;
                    }
                }
                // print hex value (offset == 10)
                else if( IsInRange( offset, 10, sizeof(uint64_t) ) )
                {
                    uint64_t offset2 = offset - 10;

                    ((uint8_t*)&LastHexInput)[offset2] = val;

                    if( offset2 == 0 )
                    {
                        printf( "%lx", LastHexInput );
                        LastHexInput = 0;
                    }
                }
            }

            uint64_t RequiredAddressSpace()
            {
                return 18;
            }

            bool CheckAccessibility( uint64_t index, uint64_t size ) override
            {
                uint64_t offset = index - this->startLocation;

                if( IsInRange( offset, 0, RequiredAddressSpace() ) && IsInRange( offset + size-1, 0, RequiredAddressSpace() ) )
                    return true;
                else
                    return false;
            }
    };

    class MachinePower : public Peripheral
    {
        public:
        uint8_t state; // 0 - off, 1 - on

        Component_ID GetID() override
        {
            return Component_ID_Power;
        }

        MachinePower()
        {
            state = 1;
        }

        bool CheckAccessibility( uint64_t index, uint64_t size ) override
        {
            uint64_t offset = index - this->startLocation;

            return ( offset == 0 ) && ( size == 1 );
        }

        uint8_t GetByte( uint64_t index ) override
        {
            SetLastHwError( HwError_NoError );
            if( CheckAccessibility( index, 1 ) )
                return state;
            else
            {
                SetLastHwError( HwError_UnknownAddress );
                return 0;
            }
        }

        void SetByte( uint64_t index, uint8_t val ) override
        {
            SetLastHwError( HwError_NoError );
            if( CheckAccessibility( index, 1 ) )
            {
                switch( val )
                {
                    case 0:
                    {
                        state = 0;
                        break;
                    }
                    case 1:
                    {
                        state = 1;
                        break;
                    }
                    default:
                    {
                        SetLastHwError( HwError_UnknownOption );
                        break;
                    }
                }
            }
            else
            {
                SetLastHwError( HwError_UnknownAddress );
                return;
            }
        }

    };

    class MachineMemory : public Peripheral
    {
        private:
            uint64_t MemSize;
            RawMemoryOnOS leMem;

        public:
            MachineMemory( uint64_t mem )
            {
                SetLastHwError( HwError_NoError );
                leMem.SetNewMemorySize( mem );
                MemSize = mem;
                SetLastHwError( leMem.LastHwError() );
            }

            MachineMemory(){}

            void SetNewMemorySize( uint64_t mem )
            {
                SetLastHwError( HwError_NoError );
                leMem.SetNewMemorySize( mem );
                MemSize = mem;
                SetLastHwError( leMem.LastHwError() );
            }

            HwError Setup() override
            {
                return leMem.Setup();
            }

            Component_ID GetID() override
            {
                return Component_ID_MemoryUnit;
            }

            uint8_t GetByte( uint64_t index ) override
            {
                SetLastHwError( HwError_NoError );
                uint8_t result = leMem.GetByte( index - this->startLocation );
                SetLastHwError( leMem.LastHwError() );
                return result;
            }

            void SetByte( uint64_t index, uint8_t val ) override
            {
                SetLastHwError( HwError_NoError );
                leMem.SetByte( index - this->startLocation, val );
                SetLastHwError( leMem.LastHwError() );
            }

            bool CheckAccessibility( uint64_t index, uint64_t size ) override
            {
                SetLastHwError( HwError_NoError );
                bool access = leMem.CheckAccessibility( index - this->startLocation, size );
                SetLastHwError( leMem.LastHwError() );
                return access;
            }

    };
};

#endif
