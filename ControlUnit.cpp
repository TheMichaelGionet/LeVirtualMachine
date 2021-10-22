#ifndef VM_Component_InstructionDecoder
#define VM_Component_InstructionDecoder

#include <stdint.h>
#include <memory.h>
#include "HwError.hpp"
#include "Component.hpp"
#include "InstructionType.hpp"
#include "Pipeline.hpp"


namespace Component
{

    // Note:
    //
    // This Control Unit simply takes an input of an instruction and the current PC value and sets a bunch of control signals based on it.
    //

    class ControlUnit : public Component_t
    {
        private:

            Instruction::InstructionParser Parser;

            Stage::Pipeline_IFtoID DefaultValues[ Instruction::NumInstructions ];

            void SetupDefaultValues()
            {
                memset( DefaultValues, 0, sizeof(Stage::Pipeline_IFtoID) * Instruction::NumInstructions );

                // Convention: in hex when set by instruction, in decimal or binary for a hard default.

                // 0b0000011 = LOAD
                DefaultValues[ 0b0000011 ].Instruction = {0x00};
                DefaultValues[ 0b0000011 ].IDParams = {1, 0b01, 0x00, 0};
                DefaultValues[ 0b0000011 ].ALUOp = {1, 0b000, 0, 1, 0, 0, 0};
                DefaultValues[ 0b0000011 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0000011 ].MEMParamsIFtoID = {1, 0, 0x00, 0x00 };
                DefaultValues[ 0b0000011 ].WBOp = {1, 0x00};

                // 0b0010011 = ALU with IMM value
                DefaultValues[ 0b0010011 ].Instruction = {0x00};
                DefaultValues[ 0b0010011 ].IDParams = {1, 0b01, 0x00, 0};
                DefaultValues[ 0b0010011 ].ALUOp = {1, 0x00, 0, 1, 1, 0, 0};
                DefaultValues[ 0b0010011 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0010011 ].MEMParamsIFtoID = {0, 0, 0, 0 };
                DefaultValues[ 0b0010011 ].WBOp = {1, 0x00};

                // 0b0010111 = AUIPC
                DefaultValues[ 0b0010111 ].Instruction = {0x00};
                DefaultValues[ 0b0010111 ].IDParams = {1, 0b00, 0, 0};
                DefaultValues[ 0b0010111 ].ALUOp = {1, 0b000, 0, 1, 1, 1, 0};
                DefaultValues[ 0b0010111 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0010111 ].MEMParamsIFtoID = {0, 0, 0, 0 };
                DefaultValues[ 0b0010111 ].WBOp = {1, 0x00};

                // 0b0011011 = 32 bit ALU with IMM value (ignore, this impl. uses 64 bits. Someone can fork this if they have a need for it.)

                // 0b0100011 = STORE
                DefaultValues[ 0b0100011 ].Instruction = {0x00};
                DefaultValues[ 0b0100011 ].IDParams = {1, 0b11, 0x00, 0x00};
                DefaultValues[ 0b0100011 ].ALUOp = {1, 0b000, 0, 1, 0, 0, 0};
                DefaultValues[ 0b0100011 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0100011 ].MEMParamsIFtoID = {0, 1, 0x00, 0 };
                DefaultValues[ 0b0100011 ].WBOp = {0, 0};

                // 0b0110011 = ALU with Reg value
                DefaultValues[ 0b0110011 ].Instruction = {0x00};
                DefaultValues[ 0b0110011 ].IDParams = {0, 0b11, 0x00, 0x00};
                DefaultValues[ 0b0110011 ].ALUOp = {1, 0x00, 0x00, 0, 1, 0, 0};
                DefaultValues[ 0b0110011 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0110011 ].MEMParamsIFtoID = {0, 0, 0, 0 };
                DefaultValues[ 0b0110011 ].WBOp = {1, 0x00};

                // 0b0110111 = LUI
                DefaultValues[ 0b0110111 ].Instruction = {0x00};
                DefaultValues[ 0b0110111 ].IDParams = {1, 0b00, 0, 0};
                DefaultValues[ 0b0110111 ].ALUOp = {0, 0, 0, 0, 0, 0, 0};
                DefaultValues[ 0b0110111 ].PCCalc = {0, 0, 0, 0, 0x00};
                DefaultValues[ 0b0110111 ].MEMParamsIFtoID = {0, 0, 0, 0};
                DefaultValues[ 0b0110111 ].WBOp = {1, 0x00};

                // 0b0111011 = 32 bit ALU with Reg value (ignore...) 

                // 0b1100011 = Branch
                DefaultValues[ 0b1100011 ].Instruction = {0x00};
                DefaultValues[ 0b1100011 ].IDParams = {1, 0b11, 0x00, 0x00};
                DefaultValues[ 0b1100011 ].ALUOp = {1, 0x00, 0, 1, 1, 0, 0};
                DefaultValues[ 0b1100011 ].PCCalc = {1, 0, 1, 0x00, 0x00};
                DefaultValues[ 0b1100011 ].MEMParamsIFtoID = {0, 0, 0, 0};
                DefaultValues[ 0b1100011 ].WBOp = {0, 0};

                // 0b1100011 = JALR
                DefaultValues[ 0b1100111 ].Instruction = {0x00};
                DefaultValues[ 0b1100111 ].IDParams = {1, 0b01, 0x00, 0};
                DefaultValues[ 0b1100111 ].ALUOp = {1, 0b000, 0, 1, 1, 1, 1};
                DefaultValues[ 0b1100111 ].PCCalc = {0, 1, 0, 0, 0x00};
                DefaultValues[ 0b1100111 ].MEMParamsIFtoID = {0, 0, 0, 0};
                DefaultValues[ 0b1100111 ].WBOp = {1, 0x00};

                // 0b1101111 = JAL
                DefaultValues[ 0b1101111 ].Instruction = {0x00};
                DefaultValues[ 0b1101111 ].IDParams = {1, 0b00, 0, 0};
                DefaultValues[ 0b1101111 ].ALUOp = {1, 0b000, 0, 1, 1, 1, 1};
                DefaultValues[ 0b1101111 ].PCCalc = {1, 0, 0, 0, 0x00};
                DefaultValues[ 0b1101111 ].MEMParamsIFtoID = {0, 0, 0, 0};
                DefaultValues[ 0b1101111 ].WBOp = {1, 0x00};

                // 0b1110011 = ENVIRONMENT, CSR
                // CSR not implemented yet, so this will be ignored for now.
                // Simply look for the ECALL and EBREAK instructions exactly for now.

            }

            // All instructions that begin with 0b11 are parsed here.
            Stage::Pipeline_IFtoID DecodeStandard( uint32_t instruction, uint64_t PCVal )
            {

                static void * StdOpDispatch[32] = 
                {
                    &&Load, &&Unknown, &&Unknown, &&Unknown,
                    &&ALU64Imm, &&AUIPC, &&ALU32Imm, &&Unknown,
                    &&Store, &&Unknown, &&Unknown, &&Unknown, 
                    &&ALU64Reg, &&LUI, &&ALU32Reg, &&Unknown,
                    &&Unknown, &&Unknown, &&Unknown, &&Unknown,
                    &&Unknown, &&Unknown, &&Unknown, &&Unknown,
                    &&Branch, &&Jalr, &&Unknown, &&Jal, 
                    &&StatusEnvironment, &&Unknown, &&Unknown, &&Unknown
                };

                uint8_t opcode = instruction & 0b01111111;
                uint8_t stdop = opcode >> 2; // 5 bits

                Stage::Pipeline_IFtoID result = DefaultValues[opcode];
                result.Instruction.Instruction = instruction;
                result.PCCalc.PCVal = PCVal;
                Instruction::ParsedInstruction lInstruction = Parser.ParseInstruction( instruction );

                if( stdop < 32 )
                    goto *StdOpDispatch[stdop];

                Load:
                {

                    uint8_t funct3 = lInstruction.ParsedInst.I.funct3;
                    uint8_t RegDest = lInstruction.ParsedInst.I.rd;
                    uint8_t RegSource1 = lInstruction.ParsedInst.I.rs1;

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.MEMParamsIFtoID.MemSize = funct3 & 0b11;
                    result.MEMParamsIFtoID.SignExtend = !( funct3 >> 2 ); // the 2nd bit (starting at 0) is "keep unsigned = 1, sign extend = 0"
                    result.WBOp.Reg = RegDest & 0b11111;

                    return result;
                };

                ALU64Imm:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.I.funct3;
                    uint8_t RegDest = lInstruction.ParsedInst.I.rd;
                    uint8_t RegSource1 = lInstruction.ParsedInst.I.rs1;

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.ALUOp.ALUOp = funct3 & 0b111;
                    result.WBOp.Reg = RegDest & 0b11111;

                    return result;
                };

                AUIPC:
                {
                    uint8_t RegDest = lInstruction.ParsedInst.U.rd;

                    result.WBOp.Reg = RegDest & 0b11111;

                    return result;
                };

                ALU32Imm:
                {
                    // Not Used, make a fork if you want this.

                    return result;
                };

                Store:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.S.funct3;
                    uint8_t RegSource1 = lInstruction.ParsedInst.S.rs1;
                    uint8_t RegSource2 = lInstruction.ParsedInst.S.rs2;

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.IDParams.ReadReg2 = RegSource2 & 0b11111;
                    result.MEMParamsIFtoID.MemSize = funct3 & 0b11;

                    return result;
                };

                ALU64Reg:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.R.funct3;
                    uint8_t RegSource1 = lInstruction.ParsedInst.R.rs1;
                    uint8_t RegSource2 = lInstruction.ParsedInst.R.rs2;
                    uint8_t RegDest = lInstruction.ParsedInst.R.rd;
                    uint8_t funct7 = lInstruction.ParsedInst.R.funct7;

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.IDParams.ReadReg2 = RegSource2 & 0b11111;
                    result.WBOp.Reg = RegDest & 0b11111;
                    result.ALUOp.ALUOp = funct3 & 0b111;
                    result.ALUOp.InvertALUOp = (funct7 >> 5) & 0b1;

                    return result;
                };

                LUI:
                {
                    uint8_t RegDest = lInstruction.ParsedInst.U.rd;
                    
                    result.WBOp.Reg = RegDest & 0b11111;

                    return result;
                };

                ALU32Reg:
                {
                    // not used, fork if you want it

                    return result;
                };

                Branch:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.SB.funct3;
                    uint8_t RegSource1 = lInstruction.ParsedInst.SB.rs1;
                    uint8_t RegSource2 = lInstruction.ParsedInst.SB.rs2;

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.IDParams.ReadReg2 = RegSource2 & 0b11111;

                    // funct3    cc    OpName    ALUOp    Invert Output
                    // 0b000     eq    XOR       0b100    1
                    // 0b001     ne    XOR       0b100    0
                    // 0b010     --------------------------------------
                    // 0b011     --------------------------------------
                    // 0b100     lt    SLT       0b010    0
                    // 0b101     ge    SLT       0b010    1
                    // 0b110     ltu   SLTU      0b011    0
                    // 0b111     geu   SLTU      0b011    1

                    // technically, 2, 3 are don't cares, but I set them to something that would make this not break.
                    static uint8_t ALUOpLookup[8] = {0b100, 0b100, 0b110, 0b110, 0b010, 0b010, 0b11, 0b11};
                    static uint8_t InvertOpLookup[8] = {1, 0, 0, 1, 0, 1, 0, 1};

                    result.ALUOp.ALUOp = ALUOpLookup[funct3];
                    result.PCCalc.InvertALUOutput = InvertOpLookup[funct3];

                    return result;
                };

                Jalr:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.I.funct3;
                    uint8_t RegSource1 = lInstruction.ParsedInst.I.rs1;
                    uint8_t RegDest = lInstruction.ParsedInst.I.rd;

                    // funct3 should always be 0b000

                    result.IDParams.ReadReg1 = RegSource1 & 0b11111;
                    result.WBOp.Reg = RegDest & 0b11111;

                    return result;
                };

                Jal:
                {
                    uint8_t RegDest = lInstruction.ParsedInst.UJ.rd;

                    result.WBOp.Reg = RegDest;

                    return result;
                };

                StatusEnvironment:
                {
                    uint8_t funct3 = lInstruction.ParsedInst.I.funct3;

                    static void * DispatchStatus[8] = { &&StatusEnvironment_Env, &&StatusEnvironment_CSRRW, &&StatusEnvironment_CSRRS, &&StatusEnvironment_CSRRC, &&StatusEnvironment_Unknown, &&StatusEnvironment_CSRRWI, &&StatusEnvironment_CSRRSI, &&StatusEnvironment_CSRRCI };

                    if( funct3 < 8 )
                        goto *DispatchStatus[funct3];
                    else
                        goto StatusEnvironment_Unknown;

                    StatusEnvironment_Env:
                    {
                        // Do nothing for now. Will probably implement procedures later to print characters to the screen, and test for keypresses.

                        return result;
                    };

                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    // Yes, it is intended to fall through each to the unknown option, until we have a CSR register we can do things with.
                    //
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    StatusEnvironment_CSRRW:
                    {

                        //return result;
                    };

                    StatusEnvironment_CSRRS:
                    {
                        
                        //return result;
                    };

                    StatusEnvironment_CSRRC:
                    {

                        //return result;
                    };

                    StatusEnvironment_CSRRWI:
                    {

                        //return result;
                    };

                    StatusEnvironment_CSRRSI:
                    {
                        
                        //return result;
                    };

                    StatusEnvironment_CSRRCI:
                    {

                        //return result;
                    };

                    StatusEnvironment_Unknown:
                    {
                        SetLastHwError( HwError_UnknownInstruction );
                        return result;
                    };
                };

                Unknown:
                {
                    SetLastHwError( HwError_UnknownInstruction );
                    return result;
                };

            }

            Stage::Pipeline_IFtoID MissingExtension()
            {
                Stage::Pipeline_IFtoID result = {0};

                return result;
            }

        public:

            Component_ID GetID() override
            {
                return Component_ID_InstructionDecoder;
            }

            HwError Setup() override
            {
                HwError result = HwError_NoError;
                SetupDefaultValues();

                return result;
            }

            Instruction::ParsedInstruction Parse( uint32_t instruction )
            {
                return Parser.ParseInstruction( instruction );
            }

            Stage::Pipeline_IFtoID Perform( uint32_t instruction, uint64_t PCVal )
            {

                static void* ExtensionDispatch[4] = { &&Ext00, &&Ext01, &&Ext10, &&ExtStd };

                SetLastHwError( HwError_NoError );

                uint8_t opcode = instruction & 0b01111111;
                uint8_t extension = opcode & 0b11;

                if( extension < 4 )
                    goto *ExtensionDispatch[extension];
                else
                    goto BadExt;

                Ext00:
                {
                    SetLastHwError( HwError_UnknownInstruction );
                    return MissingExtension();
                };

                Ext01:
                {
                    SetLastHwError( HwError_UnknownInstruction );
                    return MissingExtension();
                };

                Ext10:
                {
                    SetLastHwError( HwError_UnknownInstruction );
                    return MissingExtension();
                };

                ExtStd:
                {
                    return DecodeStandard( instruction, PCVal );
                };
                
                BadExt:
                {
                    SetLastHwError( HwError_BadOpcodeExtension );
                    Stage::Pipeline_IFtoID result = {0};
                    return result;
                };
            }

    };

};

#endif
