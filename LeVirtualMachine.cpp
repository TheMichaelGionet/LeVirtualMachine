
// Top level file that loads a program and then runs it in the VM.

#include <stdint.h>
#include <stdio.h>
#include "VirtualMachine.cpp"



bool LoadProgramFromFile( char * filename, Component::MemoryAccess * leMem, uint64_t offset )
{
    FILE * f = fopen( filename, "rb" );

    if( f == NULL )
    {
        printf( "Could not open '%s'\n", filename );
        return false;
    }

    uint64_t iterator;
    bool ContinueReading = true;
    for( iterator = 0; ContinueReading; iterator += 4 )
    {
        uint32_t buffer;
        uint64_t NumInstructionsRead = fread( &buffer, sizeof(uint32_t), 1, f );
        if( NumInstructionsRead )
        {
            leMem->SetWord( iterator + offset, buffer );
        }
        else ContinueReading = false;
    }

    fclose(f);

    return true;
}



int main( int argc, char * argv[] )
{
    if( argc != 2 )
    {
        printf( "Usage: ./LeVirtualMachine <file_to_execute>\n" );
        return 0;
    }

    VirtualMachine leVM;

    HwError resultSetup = leVM.Setup();

    if( resultSetup != HwError_NoError )
    {
        printf( "Error setting up: %s\n", DecodeHwError(resultSetup) );
        return -1;
    }

    Component::MemoryAccess * leMem = leVM.GetMem();

    if( !LoadProgramFromFile( argv[1], leMem, 0 ) )
        return -1;

    leMem->SetByte( leVM.PrinterBaseAddress, 0 );

    leVM.Perform();

    HwError resultRunning = leVM.LastHwError();

    if( resultRunning != HwError_NoError )
    {
        printf( "Error during execution: %s\n", DecodeHwError(resultRunning) );
        return -1;
    }

    return 0;

}
