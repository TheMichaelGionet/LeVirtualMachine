#ifndef VM_Stage_MEM
#define VM_Stage_MEM

#include <stdint.h>
#include "Common.hpp"
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"

#include "Memory.cpp"

namespace Stage
{

    class MEM : public Component::Component_t
    {
        private:
            Component::MemoryAccess * Mem = nullptr;

        public:

            MEM( Component::MemoryAccess * mem )
            {
                Mem = mem;
            }

            Component::Component_ID GetID()
            {
                return Component::Component_ID_StageMEM;
            }

            HwError Setup() override
            {
                return HwError_NoError;
            }

            Pipeline_MEMtoWB Perform( Pipeline_EXtoMEM pipe )
            {
                Pipeline_MEMtoWB Result;
                Result.Reset();
                Result.CopyFromLast( pipe );

                SetLastHwError( HwError_NoError );

                if( pipe.MEMParams.DoMemRead )
                {
                    static void * DispatchTable[] = { &&Read_Byte, &&Read_HWord, &&Read_Word, &&Read_DWord };
                    if( Mem == nullptr )
                    {
                        SetLastHwError( HwError_MemoryAccessMissing );
                        return Result;
                    }

                    uint64_t Address = pipe.MEMParams.Address;
                    bool SignExtend = pipe.MEMParams.SignExtend != 0;
                    uint8_t MemSize = pipe.MEMParams.MemSize;

                    if( MemSize < 4 )
                        goto *DispatchTable[MemSize];
                    else
                    {
                        Result.SetWBVal( 1, 0 );
                        SetLastHwError( HwError_Pipeline_InvalidMemSize );
                    }

                    Read_Byte:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 1 );
                        if( MemIsGood )
                        {
                            uint8_t Val = Mem->GetByte( Address );
                            Result.SetWBVal( 1, ExtendByte( Val, SignExtend ) );
                        }
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }
                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastRead;
                    };

                    Read_HWord:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 2 );
                        if( MemIsGood )
                        {
                            uint16_t Val = Mem->GetHWord( Address );
                            Result.SetWBVal( 1, ExtendHWord( Val, SignExtend ) );
                        }
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }
                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastRead;
                    };

                    Read_Word:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 4 );
                        if( MemIsGood )
                        {
                            uint32_t Val = Mem->GetWord( Address );
                            Result.SetWBVal( 1, ExtendWord( Val, SignExtend ) );
                        }
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }
                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastRead;
                    };

                    Read_DWord:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 8 );
                        if( MemIsGood )
                        {
                            uint64_t Val = Mem->GetDWord( Address );
                            Result.SetWBVal( 1, Val );
                        }
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }
                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastRead;
                    };
                }

                BreakPastRead:

                if( pipe.MEMParams.DoMemWrite )
                {
                    static void * DispatchTable[] = { &&Write_Byte, &&Write_HWord, &&Write_Word, &&Write_DWord };

                    if( Mem == nullptr )
                    {
                        SetLastHwError( HwError_MemoryAccessMissing );
                        return Result;
                    }

                    uint64_t Address = pipe.MEMParams.Address;
                    uint8_t MemSize = pipe.MEMParams.MemSize;
                    uint64_t WriteVal = pipe.MEMParams.WriteVal;

                    if( MemSize < 4 )
                        goto *DispatchTable[MemSize];
                    else
                    {
                        SetLastHwError( HwError_Pipeline_InvalidMemSize );
                    }

                    Write_Byte:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 1 );

                        if( MemIsGood )
                            Mem->SetByte( Address, (uint8_t) (WriteVal & 0xff) );
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }

                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastWrite;
                    }

                    Write_HWord:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 2 );

                        if( MemIsGood )
                            Mem->SetHWord( Address, (uint16_t) (WriteVal & 0xffff) );
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }

                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastWrite;
                    }

                    Write_Word:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 4 );

                        if( MemIsGood )
                            Mem->SetWord( Address, (uint32_t) (WriteVal & 0xffffffff) );
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }

                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastWrite;
                    }

                    Write_DWord:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( Address, 8 );

                        if( MemIsGood )
                            Mem->SetDWord( Address, WriteVal );
                        else
                        {
                            SetLastHwError( HwError_CannotAccessMemory );
                            return Result;
                        }

                        SetLastHwError( Mem->LastHwError() );

                        goto BreakPastWrite;
                    }

                }

                BreakPastWrite:

                return Result;
            }

    };

};


#endif
