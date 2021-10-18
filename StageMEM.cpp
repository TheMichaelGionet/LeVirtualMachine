#ifndef VM_Stage_MEM
#define VM_Stage_MEM

#include <stdint.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Pipeline.hpp"

#include "Memory.cpp"

namespace Stage
{

    class MEM : public Component::Component
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
                HwError result = Mem->Setup();
                return result;
            }

            Pipeline_MEMtoWB Perform( Pipeline_EXtoMEM pipe )
            {
                Pipeline_MEMtoWB result;
                
                result.Instruction = pipe.Instruction;
                result.DoWB = pipe.DoWB;
                result.Reg = pipe.Reg;
                result.Val = pipe.Val; // If we do not read, this just carries over from the EX stage.

                SetLastHwError( HwError_NoError );

                if( pipe.DoMemRead )
                {
                    static void * DispatchTable[] = { &&Read_Byte, &&Read_HWord, &&Read_Word, &&Read_Dword };
                    if( Mem == nullptr )
                    {
                        SetLastHwError( HwError_MemoryAccessMissing );
                        return result;
                    }

                    if( pipe.MemSize < 4 )
                        goto *DispatchTable[pipe.MemSize];
                    else
                    {
                        result.Val = 0;
                        SetLastHwError( HwError_Pipeline_InvalidMemSize );
                    }

                    Read_Byte:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 1 );
                        if( MemIsGood )
                            result.Val = (uint64_t) Mem->GetByte( pipe.Address );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 2 );
                        if( MemIsGood )
                            result.Val = (uint64_t) Mem->GetHWord( pipe.Address );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 4 );
                        if( MemIsGood )
                            result.Val = (uint64_t) Mem->GetWord( pipe.Address );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 8 );
                        if( MemIsGood )
                            result.Val = (uint64_t) Mem->GetDWord( pipe.Address );
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

                if( pipe.DoMemWrite )
                {
                    static void * DispatchTable[] = { &&Write_Byte, &&Write_HWord, &&Write_Word, &&Write_DWord };

                    if( Mem == nullptr )
                    {
                        SetLastHwError( HwError_MemoryAccessMissing );
                        return result;
                    }

                    if( pipe.MemSize < 4 )
                        goto *DispatchTable[pipe.MemSize];
                    else
                    {
                        result.Val = 0;
                        SetLastHwError( HwError_Pipeline_InvalidMemSize );
                    }

                    Write_Byte:
                    {
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 1 );

                        if( MemIsGood )
                            Mem->SetByte( pipe.Address, (uint8_t) (pipe.WriteVal & 0xff) );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 2 );

                        if( MemIsGood )
                            Mem->SetHWord( pipe.Address, (uint16_t) (pipe.WriteVal & 0xffff) );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 4 );

                        if( MemIsGood )
                            Mem->SetWord( pipe.Address, (uint32_t) (pipe.WriteVal & 0xffffffff) );
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
                        bool MemIsGood = Mem->CheckAccessibility( pipe.Address, 8 );

                        if( MemIsGood )
                            Mem->SetDWord( pipe.Address, pipe.WriteVal );
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

                return result;
            }

    };

};


#endif
