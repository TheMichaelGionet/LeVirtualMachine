#ifndef VM_VirtualMachine
#define VM_VirtualMachine

#include <stdint.h>
#include <stdio.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "Memory.cpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"
#include "ControlUnit.cpp"
#include "BUS.cpp"
#include "MachineControls.cpp"
#include "StageIF.cpp"
#include "StageID.cpp"
#include "StageEX.cpp"
#include "StageMEM.cpp"
#include "StageWB.cpp"

// Specs are:
// 1MiB of ram (= 2^20 Bytes)
// memory begins at 0x00 and ends at 0xffffffff
// printer begins at 0x100000000 and ends at 0x100000011
// power begins at 0x100000020 and ends at 0x100000020


class VirtualMachine : public Component::Component_t
{
    private:
        Stage::IF IF;
        Stage::ID ID;
        Stage::EX EX;
        Stage::MEM MEM;
        Stage::WB WB;

        Component::MachineMemory Memory;
        Component::Printer Printer;
        Component::MachinePower Power;

        Component::BUS BUS;

        const uint64_t MemBaseAddress = 0;
        const uint64_t MemSize = 0x100000000;
        const uint64_t PrinterBaseAddress = 0x100000000;
        const uint64_t PrinterSize = 0x12;
        const uint64_t PowerBaseAddress = 0x100000020;
        const uint64_t PowerSize = 0x1;

    public:

        VirtualMachine()
        {
            Memory.SetNewMemorySize( MemSize );

            Component::IntegerRegisterFile * RegF = ID.GetRegFForWB();
            WB.SetRegF( RegF );

            Memory.SetRange( MemBaseAddress, MemSize );
            Printer.SetRange( PrinterBaseAddress, PrinterSize );
            Power.SetRange( PowerBaseAddress, PowerSize );

            BUS.AddPeripheral( &Memory, 0 );
            BUS.AddPeripheral( &Printer, 1 );
            BUS.AddPeripheral( &Power, 2 );

        }

        Component::Component_ID GetID() override
        {
            return Component::Component_ID_VirtualMachine;
        }

        HwError Setup() override
        {
            HwError result = HwError_NoError;

            result = Memory.Setup();
            if( result != HwError_NoError )
                return result;

            result = Printer.Setup();
            if( result != HwError_NoError )
                return result;

            result = Power.Setup();
            if( result != HwError_NoError )
                return result;

            result = BUS.Setup();
            if( result != HwError_NoError )
                return result;

            result = IF.Setup();
            if( result != HwError_NoError )
                return result;

            result = ID.Setup();
            if( result != HwError_NoError )
                return result;

            result = EX.Setup();
            if( result != HwError_NoError )
                return result;

            result = MEM.Setup();
            if( result != HwError_NoError )
                return result;

            result = WB.Setup();
            if( result != HwError_NoError )
                return result;

            return result;
        }

        Component::MemoryAccess * GetMem()
        {
            return &BUS;
        }

        uint64_t GetPCVal()
        {
            return IF.GetPCReg();
        }

        void SetPCVal( uint64_t val )
        {
            IF.ReplacePC( val );
        }
};

#endif
