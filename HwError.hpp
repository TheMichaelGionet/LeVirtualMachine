

#ifndef VM_HwError
#define VM_HwError


typedef enum
{
    HwError_NoError = 0,
    HwError_Undefined,
    HwError_InvalidAluOp,
    HwError_RegisterIndexOutOfRange,
    HwError_MemoryIndexOutOfRange,
    HwError_CannotAllocateSpace,
    HwError_UnknownInstructionType,

    HwError_RegisterFileMissing,
    HwError_MemoryAccessMissing,

    HwError_Pipeline_InvalidMemSize,
    HwError_CannotAccessMemory,

    HwError_BadOpcodeExtension,
    HwError_UnknownInstruction,
} HwError;

const char * DecodeHwError( HwError E )
{
    switch( E )
    {
        case HwError_NoError: return "No Error.";
        case HwError_Undefined: return "An unknown error has occurred.";
        case HwError_InvalidAluOp: return "Invalid ALU Operation has been given.";
        case HwError_RegisterIndexOutOfRange: return "The Register Index given is out of range.";
        case HwError_MemoryIndexOutOfRange: return "The Index given is larger than any memory location index.";
        case HwError_CannotAllocateSpace: return "Cannot allocate the specified amount of space for the raw memory.";
        case HwError_UnknownInstructionType: return "An Unknown instruction type is attempting to be parsed. ";
        case HwError_RegisterFileMissing: return "Register File is missing.";
        case HwError_MemoryAccessMissing: return "Memory Access is missing.";
        case HwError_Pipeline_InvalidMemSize: return "Invalid mem size.";
        case HwError_CannotAccessMemory: return "Cannot access memory.";
        case HwError_BadOpcodeExtension: return "Bad opcode extension.";
        case HwError_UnknownInstruction: return "Unknown instruction.";

        default: return "Error code not known.";
    }
}

const char * MoreInfoHwError( HwError E )
{
    switch( E )
    {

        case HwError_CannotAllocateSpace: return "If you are running the VM on an operating system, try closing some other programs first, or decrease the amount of memory the VM should use. If you are booting into the VM on raw hardware, check to see if you have the minimum amount of memory needed to run the VM.";
        case HwError_UnknownInstructionType: return "This is likely due to a programmer attempting to add an extra instruction type to the parser without adding support properly. Either that or possible memory corruption.";

        default: return "No more info.";
    }


}

















#endif
