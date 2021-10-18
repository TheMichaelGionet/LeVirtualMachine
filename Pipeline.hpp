#ifndef VM_Stage_Pipelines
#define VM_Stage_Pipelines

#include <stdint.h>

namespace Stage
{
    // Pipeline sections

    class __attribute__((packed)) Section_Instruction;
    class __attribute__((packed)) Section_WBOp;
    class __attribute__((packed)) Section_WBParams;
    class __attribute__((packed)) Section_MEMParams;
    class __attribute__((packed)) Section_MEMParamsIDtoEX;
    class __attribute__((packed)) Section_MEMParamsIFtoID;
    class __attribute__((packed)) Section_PCResult;
    class __attribute__((packed)) Section_PCCalc;
    class __attribute__((packed)) Section_ALUInputs;
    class __attribute__((packed)) Section_ALUOp;
    class __attribute__((packed)) Section_IDParams;


    class __attribute__((packed)) Section_Instruction
    {
        public:

        uint32_t Instruction : 32;

        void Copy( Section_Instruction copy )
        {
            Instruction = copy.Instruction;
        }

        uint32_t Get()
        {
            return Instruction;
        }

        void Reset()
        {
            Instruction = 0;
        }
    };

    class __attribute__((packed)) Section_WBOp
    {
        public:

        uint8_t DoWB : 1;
        uint8_t Reg : 5;

        void Copy( Section_WBOp copy )
        {
            DoWB = copy.DoWB;
            Reg = copy.Reg;
        }

        void Reset()
        {
            DoWB = 0;
            Reg = 0;
        }

    };

    class __attribute__((packed)) Section_WBParams
    {
        public:

        uint8_t DoWB : 1;
        uint8_t Reg : 5;
        uint64_t Val : 64;

        void Copy( Section_WBParams copy )
        {
            DoWB = copy.DoWB;
            Reg = copy.Reg;
            Val = copy.Val;
        }

        void Reset()
        {
            DoWB = 0;
            Reg = 0;
            Val = 0;
        }

        void SetFromLast( Section_WBOp leSection )
        {
            DoWB = leSection.DoWB;
            Reg = leSection.Reg;
        }

        void SetParams( uint64_t nVal )
        {
            Val = nVal;
        }
    };

    class __attribute__((packed)) Section_MEMParamsIFtoID
    {
        public:

        uint8_t DoMemRead : 1;
        uint8_t DoMemWrite : 1;
        uint8_t MemSize : 2;
        uint8_t SignExtend : 1; // Should we sign extend whatever is read from memory?

        void Reset()
        {
            DoMemRead = 0;
            DoMemWrite = 0;
            MemSize = 0;
            SignExtend = 0;
        }

        void Copy( Section_MEMParamsIFtoID copy )
        {
            DoMemRead = copy.DoMemRead;
            DoMemWrite = copy.DoMemWrite;
            MemSize = copy.MemSize;
            SignExtend = copy.SignExtend;
        }

    };

    class __attribute__((packed)) Section_MEMParamsIDtoEX
    {
        public:

        uint8_t DoMemRead : 1;
        uint8_t DoMemWrite : 1;
        uint8_t MemSize : 2;
        uint8_t SignExtend : 1;
        uint64_t WriteVal : 64; // Set in ID

        void Reset()
        {
            DoMemRead = 0;
            DoMemWrite = 0;
            MemSize = 0;
            SignExtend = 0;
            WriteVal = 0;
        }

        void Copy( Section_MEMParamsIDtoEX copy )
        {
            DoMemRead = copy.DoMemRead;
            DoMemWrite = copy.DoMemWrite;
            MemSize = copy.MemSize;
            SignExtend = copy.SignExtend;
            WriteVal = copy.WriteVal;
        }

        void SetFromLast( Section_MEMParamsIFtoID leSection )
        {
            DoMemRead = leSection.DoMemRead;
            DoMemWrite = leSection.DoMemWrite;
            MemSize = leSection.MemSize;
            SignExtend = leSection.SignExtend;
        }

        void SetParams( uint64_t nWriteVal )
        {
            WriteVal = nWriteVal;
        }
    };

    class __attribute__((packed)) Section_MEMParams
    {
        public:

        uint8_t DoMemRead : 1;
        uint8_t DoMemWrite : 1;
        uint64_t Address : 64; // Set in EX.
        uint8_t MemSize : 2; // byte - 0, hword - 1, word - 2, dword - 3
        uint8_t SignExtend : 1;
        uint64_t WriteVal : 64; // lower number of bytes indicated by MemSize. Set in EX or ID.

        void Reset()
        {
            DoMemRead = 0;
            DoMemWrite = 0;
            Address = 0;
            MemSize = 0;
            SignExtend = 0;
            WriteVal = 0;
        }

        void Copy( Section_MEMParams copy )
        {
            DoMemRead = copy.DoMemRead;
            DoMemWrite = copy.DoMemWrite;
            Address = copy.Address;
            MemSize = copy.MemSize;
            SignExtend = copy.SignExtend;
            WriteVal = copy.WriteVal;
        }

        void SetFromLast( Section_MEMParamsIDtoEX leSection )
        {
            DoMemRead = leSection.DoMemRead;
            DoMemWrite = leSection.DoMemWrite;
            MemSize = leSection.MemSize;
            SignExtend = leSection.SignExtend;
            WriteVal = leSection.WriteVal;
        }

        void SetParams( uint64_t nAddress )
        {
            Address = nAddress;
        }

    };

    class __attribute__((packed)) Section_PCCalc
    {
        public:
        uint8_t AddToPC : 1; // add the immediate to PC
        uint8_t ReplacePC : 1; // replace PC with RegVal1 + Immediate
        uint8_t IsConditional : 1; // ReplacePC will be whatever is output by the ALU
        uint8_t InvertALUOutput : 1; // Since the ALU only uses strict less thans, we may need do NOT(ALU(...)) to get the desired jump condition.
        uint64_t PCVal : 64; // PC value extracted from PC register at the time of instruction fetching.

        void Reset()
        {
            AddToPC = 0;
            ReplacePC = 0;
            IsConditional = 0;
            InvertALUOutput = 0;
            PCVal = 0;
        }

        void Copy( Section_PCCalc copy )
        {
            AddToPC = copy.AddToPC;
            ReplacePC = copy.ReplacePC;
            IsConditional = copy.IsConditional;
            InvertALUOutput = copy.InvertALUOutput;
            PCVal = copy.PCVal;
        }
    };

    class __attribute__((packed)) Section_PCResult
    {
        public:
        uint8_t ReplacePC : 1; // Replace PC with PCVal (different from ReplacePC from bellow)
        uint64_t PCVal : 64; // PCVal is whatever is determined in EX

        void Reset()
        {
            ReplacePC = 0;
            PCVal = 0;
        }

        void Copy( Section_PCResult copy )
        {
            ReplacePC = copy.ReplacePC;
            PCVal = copy.PCVal;
        }
    };

    class __attribute__((packed)) Section_ALUOp
    {
        public:

        uint8_t DoALUOp : 1;
        uint8_t ALUOp : 3;
        uint8_t InvertALUOp : 1;
        uint8_t UseImmNotReg2 : 1;
        uint8_t ResultType : 1;
        uint8_t UsePCRegister : 1; // Use PC instead of a general purpose register
        uint8_t AddFourToResult : 1; // Add 4 to the ALU output.

        void Copy( Section_ALUOp copy )
        {
            DoALUOp = copy.DoALUOp;
            ALUOp = copy.ALUOp;
            InvertALUOp = copy.InvertALUOp;
            UseImmNotReg2 = copy.UseImmNotReg2;
            ResultType = copy.ResultType;
            UsePCRegister = copy.UsePCRegister;
            AddFourToResult = copy.AddFourToResult;
        }

        void Reset()
        {
            DoALUOp = 0;
            ALUOp = 0;
            InvertALUOp = 0;
            UseImmNotReg2 = 0;
            ResultType = 0;
            UsePCRegister = 0;
            AddFourToResult = 0;
        }
    };

    class __attribute__((packed)) Section_ALUInputs
    {
        public:

        uint8_t DoALUOp : 1;
        uint8_t ALUOp : 3;
        uint8_t InvertALUOp : 1;
        uint8_t UseImmNotReg2 : 1; // 0 - use RegVal2, 1 - use Immediate
        uint8_t ResultType : 1; // 0 - address, 1 - value for register
        uint8_t UsePCRegister : 1;
        uint8_t AddFourToResult : 1;

        uint64_t RegVal1 : 64; // Set in ID
        uint64_t RegVal2 : 64; // Set in ID
        uint64_t Immediate : 64; // Set in ID
        uint64_t PCVal : 64; // Set in ID

        void Reset()
        {
            DoALUOp = 0;
            ALUOp = 0;
            InvertALUOp = 0;
            UseImmNotReg2 = 0;
            ResultType = 0;
            UsePCRegister = 0; // instead of RegVal1
            AddFourToResult = 0;

            RegVal1 = 0;
            RegVal2 = 0;
            Immediate = 0;
            PCVal = 0;
        }

        void Copy( Section_ALUInputs copy )
        {
            DoALUOp = copy.DoALUOp;
            ALUOp = copy.ALUOp;
            InvertALUOp = copy.InvertALUOp;
            UseImmNotReg2 = copy.UseImmNotReg2;
            ResultType = copy.ResultType;
            UsePCRegister = copy.UsePCRegister;
            AddFourToResult = copy.AddFourToResult;

            RegVal1 = copy.RegVal1;
            RegVal2 = copy.RegVal2;
            Immediate = copy.Immediate;
            PCVal = copy.PCVal;
        }

        void SetFromLast( Section_ALUOp leSection )
        {
            DoALUOp = leSection.DoALUOp;
            ALUOp = leSection.ALUOp;
            InvertALUOp = leSection.InvertALUOp;
            UseImmNotReg2 = leSection.UseImmNotReg2;
            ResultType = leSection.ResultType;
            UsePCRegister = leSection.UsePCRegister;
            AddFourToResult = leSection.AddFourToResult;
        }

        void SetParams( uint64_t nRegVal1, uint64_t nRegVal2, uint64_t nImmediate, uint64_t nPCVal )
        {
            RegVal1 = nRegVal1;
            RegVal2 = nRegVal2;
            Immediate = nImmediate;
            PCVal = nPCVal;
        }

    };

    class __attribute__((packed)) Section_IDParams
    {
        public:

        uint8_t DoGenConst : 1;
        uint8_t ReadRegisters : 2; // 00 - no read, 01 - read reg1, 10 - read reg2, 11 - read both.
        uint8_t ReadReg1 : 5;
        uint8_t ReadReg2 : 5;

        void Copy( Section_IDParams copy )
        {
            DoGenConst = copy.DoGenConst;
            ReadRegisters = copy.ReadRegisters;
            ReadReg1 = copy.ReadReg1;
            ReadReg2 = copy.ReadReg2;
        }

        void Reset()
        {
            DoGenConst = 0;
            ReadRegisters = 0;
            ReadReg1 = 0;
            ReadReg2 = 0;
        }

    };



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Pipeline construction
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class __attribute__((packed)) Pipeline_MEMtoWB;
    class __attribute__((packed)) Pipeline_EXtoMEM;
    class __attribute__((packed)) Pipeline_IDtoEX;
    class __attribute__((packed)) Pipeline_IFtoID;

    class __attribute__((packed)) Pipeline_IFtoID
    {
        public:

        Section_Instruction Instruction;
        Section_IDParams IDParams;
        Section_ALUOp ALUOp;
        Section_PCCalc PCCalc;
        Section_MEMParamsIFtoID MEMParamsIFtoID;
        Section_WBOp WBOp;

        void Reset()
        {
            Instruction.Reset();
            IDParams.Reset();
            ALUOp.Reset();
            PCCalc.Reset();
            MEMParamsIFtoID.Reset();
            WBOp.Reset();
        }

    };

    class __attribute__((packed)) Pipeline_IDtoEX
    {
        public:

        Section_Instruction Instruction;
        Section_ALUInputs ALUInputs;
        Section_PCCalc PCCalc;
        Section_MEMParamsIDtoEX MEMParamsIDtoEX;
        Section_WBOp WBOp;

        void Reset()
        {
            Instruction.Reset();
            ALUInputs.Reset();
            PCCalc.Reset();
            MEMParamsIDtoEX.Reset();
            WBOp.Reset();
        }

        void CopyFromLast( Pipeline_IFtoID lePipeline )
        {
            Instruction.Copy( lePipeline.Instruction );
            ALUInputs.SetFromLast( lePipeline.ALUOp );
            PCCalc.Copy( lePipeline.PCCalc );
            MEMParamsIDtoEX.SetFromLast( lePipeline.MEMParamsIFtoID );
            WBOp.Copy( lePipeline.WBOp );
        }

        void SetALUInputs( uint64_t RegVal1, uint64_t RegVal2, uint64_t Imm, uint64_t PCVal )
        {
            ALUInputs.RegVal1 = RegVal1;
            ALUInputs.RegVal2 = RegVal2;
            ALUInputs.Immediate = Imm;
            ALUInputs.PCVal = PCVal;
        }

        void SetMemParam( uint64_t WriteVal )
        {
            MEMParamsIDtoEX.WriteVal = WriteVal;
        }

    };

    class __attribute__((packed)) Pipeline_EXtoMEM
    {
        public:
        Section_Instruction Instruction;
        Section_MEMParams MEMParams;
        Section_PCResult PCResult;
        Section_WBParams WBParams;

        void Reset()
        {
            Instruction.Reset();
            MEMParams.Reset();
            PCResult.Reset();
            WBParams.Reset();
        }

        void CopyFromLast( Pipeline_IDtoEX lePipeline )
        {
            Instruction.Copy( lePipeline.Instruction );
            MEMParams.SetFromLast( lePipeline.MEMParamsIDtoEX );
            WBParams.SetFromLast( lePipeline.WBOp );
        }

        void SetMemAddress( uint64_t Address )
        {
            MEMParams.Address = Address;
        }

        void SetWBVal( uint64_t WBVal )
        {
            WBParams.Val = WBVal;
        }

        void SetPCVal( uint8_t ReplacePC, uint64_t PCVal )
        {
            PCResult.ReplacePC = ReplacePC & 0b01;
            PCResult.PCVal = PCVal;
        }


    };

    class __attribute__((packed)) Pipeline_MEMtoWB
    {
        public:

        Section_Instruction Instruction;
        Section_WBParams WBParams;
        
        void Reset()
        {
            Instruction.Reset();
            WBParams.Reset();
        }

        void CopyFromLast( Pipeline_EXtoMEM lePipeline )
        {
            Instruction.Copy( lePipeline.Instruction );
            WBParams.Copy( lePipeline.WBParams );
        }

        void SetWBVal( uint8_t DoWB, uint64_t Val )
        {
            WBParams.DoWB = DoWB & 0b01;
            WBParams.Val = Val;
        }
    };




};




#endif
