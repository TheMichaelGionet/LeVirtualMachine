#ifndef VM_Component_RegisterFile
#define VM_Component_RegisterFile

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"

namespace Component
{

    // All register files here have the convention that r0 = (T) 0.
    // Assume that T can cast 0 to T.
    template <typename T, unsigned int Num>
    class RegisterFile : public Component_t
    {
        protected:
            T Registers[ Num ];

        public:

            HwError Setup() override
            {
                SetLastHwError( HwError_NoError );
                unsigned int iterator;
                for( iterator = 0; iterator < Num; iterator++ )
                {
                    Registers[iterator] = (T) 0;
                }
                return LastHwError();
            }

            T GetVal( unsigned int index )
            {
                SetLastHwError( HwError_NoError );

                if( index > 0 && index < Num )
                    return Registers[index];
                else
                {
                    if( index == 0 )
                        return (T) 0;

                    SetLastHwError( HwError_RegisterIndexOutOfRange );
                    return (T) 0;
                }
            }

            void SetVal( unsigned int index, T val )
            {
                SetLastHwError( HwError_NoError );

                if( index > 0 && index < Num )
                    Registers[index] = val;
                else if( index > 0 )
                    SetLastHwError( HwError_RegisterIndexOutOfRange );

            }

            unsigned int Length()
            {
                return Num;
            }
    };


    class IntegerRegisterFile : public RegisterFile< uint64_t, 32 >
    {
        public:
            Component_ID GetID() override
            {
                return Component_ID_IntRegisterFile;
            }
    };

    class FloatRegisterFile : public RegisterFile< double, 32 >
    {
        public:
            Component_ID GetID() override
            {
                return Component_ID_FloatRegisterFile;
            }
    };


};


#endif
