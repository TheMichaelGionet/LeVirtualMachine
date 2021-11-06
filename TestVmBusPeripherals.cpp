
#include <stdint.h>
#include "Component.hpp"
#include "Memory.cpp"
#include "BUS.cpp"
#include "MachineControls.cpp"

#include <stdio.h>


void TestMachineControls()
{
    printf( "\n==============================================\n\n" );
    printf( "Testing Machine Controls.\n" );

    Component::Printer lePrinter;

    lePrinter.SetRange( 5, lePrinter.RequiredAddressSpace() );

    printf( "Startup Message:\n" );

    lePrinter.SetByte( 5, 0 );

    printf( "printing \"hello world!\\n\":\n" );

    uint8_t * Message = (uint8_t*)"hello world!\n";

    int iterator;
    for( iterator = 0; Message[iterator]; iterator++ )
    {
        lePrinter.SetByte( 6, Message[iterator] );
    }

    printf( "Printing numbers 1, 2, 3, 14, 69, 420, 1201849113219032:\n" );

    lePrinter.SetByte( 5 + 2, 1 ); printf( "\n" );
    lePrinter.SetByte( 5 + 2, 2 ); printf( "\n" );
    lePrinter.SetByte( 5 + 2, 3 ); printf( "\n" );
    lePrinter.SetByte( 5 + 2, 14 ); printf( "\n" );
    lePrinter.SetByte( 5 + 2, 69 ); printf( "\n" );
    lePrinter.SetHWord( 5 + 2, 420 ); printf( "\n" );
    lePrinter.SetDWord( 5 + 2, 1201849113219032 ); printf( "\n" );

    printf( "\n" );
    printf( "Printing numbers 0x1, 0x2, 0x69, 0x420, 0xffffffffff:\n" );

    lePrinter.SetByte( 5 + 10, 0x1 ); printf( "\n" );
    lePrinter.SetByte( 5 + 10, 0x2 ); printf( "\n" );
    lePrinter.SetByte( 5 + 10, 0x3 ); printf( "\n" );
    lePrinter.SetByte( 5 + 10, 0x69 ); printf( "\n" );
    lePrinter.SetHWord( 5 + 10, 0x420 ); printf( "\n" );
    lePrinter.SetDWord( 5 + 10, 0xffffffffff ); printf( "\n" );

}

void TestPower()
{

    printf( "\n==============================================\n\n" );
    printf( "Testing Power.\n" );
    Component::MachinePower lePower;
    lePower.SetRange( 100, 1 );

    printf( "State = %d\n", lePower.state );

    lePower.SetByte( 100, 0 );
    printf( "State = %d\n", lePower.state );

    lePower.SetByte( 100, 1 );
    printf( "State = %d\n", lePower.state );

}

void TestMachineMemory()
{
    printf( "\n==============================================\n\n" );
    printf( "Testing Memory Unit.\n" );

    uint64_t MemSize = 0xff + 1;

    printf( "Spawning memory with size %ld\n", MemSize );

    Component::MachineMemory leMem( MemSize );

    HwError leError = leMem.LastHwError();
    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    leMem.SetRange( 0x100, MemSize );

    printf( "Testing Instantiation:\n" );

    leError = leMem.Setup();

    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    printf( "Testing Byte read/write:\n" );

    int iterator;
    for( iterator = 0xff; iterator <= MemSize + 0x100; iterator++ )
    {
        bool accessible = leMem.CheckAccessibility( iterator, 1 );
        leMem.SetByte( iterator, 0xff );
        printf( "%x ", (int) leMem.GetByte( iterator ) );
        if( !(iterator % 0x10) )
            printf( "\n" );
    }




}


void TestBusWithPeripherals()
{

    printf( "\n==============================================\n\n" );
    printf( "Testing Bus with the other units.\n" );

    uint64_t MemSize = 0xff + 1;

    uint64_t MemoryBaseAddress = 0;

    uint64_t PrinterBaseAddress = 0x200;
    uint64_t PowerBaseAddress = 0x300;

    Component::Printer lePrinter;
    Component::MachinePower lePower;
    Component::MachineMemory leMem( MemSize );

    printf( "Setting up mem.\n" );

    HwError leError = leMem.Setup();

    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    Component::BUS leBus;

    printf( "Setting Peripheral Ranges\n" );

    leMem.SetRange( MemoryBaseAddress, MemSize );

    lePrinter.SetRange( PrinterBaseAddress, lePrinter.RequiredAddressSpace() );

    lePower.SetRange( PowerBaseAddress, 1 );

    printf( "Adding mem as a peripheral\n" );

    leBus.AddPeripheral( &leMem, 0 );
    leError = leBus.LastHwError();

    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    printf( "Adding printer peripheral\n" );

    leBus.AddPeripheral( &lePrinter, 1 );
    leError = leBus.LastHwError();

    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    printf( "Adding power peripheral\n" );

    leBus.AddPeripheral( &lePower, 2 );
    leError = leBus.LastHwError();

    if( leError != HwError_NoError )
    {
        printf( "Error: %s\n", DecodeHwError( leError ) );
        return;
    }

    printf( "Accessing memory now.\n" );

    uint64_t iterator;
    for( iterator = MemoryBaseAddress; iterator < (MemSize + MemoryBaseAddress); iterator++ )
    {
        leBus.SetByte( iterator, 0xff );
        printf( "%x ", leBus.GetByte( iterator ) );
        if( !(iterator % 0x100) )
            printf( "\n" );
    }

    printf( "Accessing printer now.\n" );

    iterator = PrinterBaseAddress;
    leBus.SetByte( iterator, 0 );
    leBus.SetByte( iterator + 1, 'E' );
    leBus.SetByte( iterator + 1, '\n' );
    leBus.SetDWord( iterator + 2, 0xdeadbeefc0defee7 );
    leBus.SetByte( iterator + 1, '\n' );
    leBus.SetDWord( iterator + 10, 0xdeadbeefc0defee7 );
    leBus.SetByte( iterator + 1, '\n' );

    printf( "Accessing power now.\n" );
    iterator = PowerBaseAddress;
    printf( "state = %d\n", leBus.GetByte( iterator ) );
    leBus.SetByte( iterator, 0 );
    printf( "state = %d\n", leBus.GetByte( iterator ) );
    leBus.SetByte( iterator, 1 );
    printf( "state = %d\n", leBus.GetByte( iterator ) );

}



int main()
{
    TestMachineControls();

    TestPower();

    TestMachineMemory();

    TestBusWithPeripherals();

    return 0;
}

