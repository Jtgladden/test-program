//
// Created by jt_gladden on 5/13/2021.
//

#ifndef EMULATECPU_MAIN_6502_H

#include <stdio.h>
#include <stdlib.h>

namespace m6502
{
    using SByte = char;
    using Byte = unsigned char;
    using Word = unsigned short;

    using u32 = unsigned int;
    using s32 = signed int;

    struct Mem;
    struct CPU;
    struct StatusFlags;
}

struct m6502::Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
            Data[i] = 0;
    }

    //read 1 byte
    Byte operator[](u32 Address) const
    {
        //Assert here Address is < MAX_MEM
        return Data[Address];
    }

    //write 1 byte
    Byte& operator[](u32 Address)
    {
        //Assert here Address is < MAX_MEM
        return Data[Address];
    }

};

struct m6502::StatusFlags
{
    Byte C : 1;	//0: Carry Flag
    Byte Z : 1;	//1: Zero Flag
    Byte I : 1; //2: Interrupt disable
    Byte D : 1; //3: Decimal mode
    Byte B : 1; //4: Break
    Byte Unused : 1; //5: Unused
    Byte V : 1; //6: Overflow
    Byte N : 1; //7: Negative
};

struct m6502::CPU
{
    Word PC; //program counter
    Byte SP; //stack pointer

    Byte A, X, Y; // registers

    union // processor status
    {
        Byte PS;
        StatusFlags Flag;
    };

    void Reset( Word ResetVector, Mem& memory ) {
        PC = ResetVector;
        SP = 0xFF;
        Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.V =Flag. N = 0;
        A = X = Y = 0;
        memory.Initialize();
    }

    void reset(Mem& memory) {
        PC = 0xFFFC;
        SP = 0xFF;
        Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.V =Flag. N = 0;
        A = X = Y = 0;
        memory.Initialize();
    }

        Byte FetchByte(s32& Cycles, const Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }
    Word FetchWord(s32& Cycles, const Mem& memory)
    {
        //6502 is little endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8);
        PC++;

        Cycles -= 2;
        return Data;
    }

    Byte ReadByte(
            s32& Cycles,
            Word Address,
            const Mem& memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    Word ReadWord(
            s32& Cycles,
            Word Address,
            const Mem& memory)
    {
        Byte LoByte = ReadByte(Cycles, Address, memory);
        Byte HiByte = ReadByte(Cycles, Address + 1, memory);
        return LoByte | (HiByte << 8);
    }
    //Write 1 byte to memory
    void WriteByte(Byte Value, s32& Cycles, Word Address, Mem& memory)
    {
        memory[Address] = Value;
        Cycles--;
    }

    //Write 2 bytes to memory
    void WriteWord(Word Value, s32& Cycles, Word Address, Mem& memory)
    {
        memory[Address] = Value & 0xFF;
        memory[Address + 1] = (Value >> 8);
        Cycles -= 2;
    }
    //Return the Stack Pointer as a full 16-bit address in the first page
    Word SPToAddress() const
    {
        return 0x100 | SP;
    }
    //Push the Program Counter - 1 onto the stack
    void PushPCToStack(s32& Cycles, Mem& memory)
    {
        WriteWord(PC - 1, Cycles, SPToAddress() - 1, memory);
        SP -= 2;
    }
    void PushByteOntoStack(s32& Cycles, Byte Value, Mem &memory)
    {
        const Word SPWord = SPToAddress();
        memory[SPWord] = Value;
        Cycles--;
        SP--;
        Cycles--;
    }
    //Pop the Program Counter - 1 onto the stack
    Word PopWordFromStack(s32& Cycles, Mem& memory)
    {
        Word ValueFromStack = ReadWord(Cycles, SPToAddress() + 1, memory);
        SP += 2;
        Cycles--;
        return ValueFromStack;
    }

    Byte PopByteFromStack(s32& Cycles, Mem& memory)
    {
        SP++;
        const Word SPWord = SPToAddress();
        Byte Value = memory[SPWord];
        Cycles -= 3;

        return Value;
    }

    //opcodes
    static constexpr Byte
            //LDA
            INS_LDA_IM = 0xA9,
            INS_LDA_ZP = 0xA5,
            INS_LDA_ZPX = 0xB5,
            INS_LDA_ABS = 0xAD,
            INS_LDA_ABSX = 0xBD,
            INS_LDA_ABSY = 0xB9,
            INS_LDA_INDX = 0xA1,
            INS_LDA_INDY = 0xB1,
            //LDX
            INS_LDX_IM = 0xA2,
            INS_LDX_ZP = 0xA6,
            INS_LDX_ZPY = 0xB6,
            INS_LDX_ABS = 0xAE,
            INS_LDX_ABSY = 0xBE,
            //LDY
            INS_LDY_IM = 0xA0,
            INS_LDY_ZP = 0xA4,
            INS_LDY_ZPX = 0xB4,
            INS_LDY_ABS = 0xAC,
            INS_LDY_ABSX = 0xBC,
            //STA
            INS_STA_ZP = 0x85,
            INS_STA_ZPX = 0x95,
            INS_STA_ABS = 0x8D,
            INS_STA_ABSX = 0x9D,
            INS_STA_ABSY = 0x99,
            INS_STA_INDX = 0x81,
            INS_STA_INDY = 0x91,
            //STX
            INS_STX_ZP = 0x86,
            INS_STX_ZPY = 0x96,
            INS_STX_ABS = 0x8E,
            //STY
            INS_STY_ZP = 0x84,
            INS_STY_ZPX = 0x94,
            INS_STY_ABS = 0x8C,
            //Stack Operations
            INS_TSX = 0xBA,
            INS_TXS = 0x9A,
            INS_PHA = 0x48,
            INS_PLA = 0x68,
            INS_PHP = 0x08,
            INS_PLP = 0x28,
            //Logical AND
            INS_AND_IM = 0x29,
            INS_AND_ZP = 0x25,
            INS_AND_ZPX = 0x35,
            INS_AND_ABS = 0x2D,
            INS_AND_ABSX = 0x3D,
            INS_AND_ABSY = 0x39,
            INS_AND_INDX = 0x21,
            INS_AND_INDY = 0x31,
            //Logical EOR
            INS_EOR_IM = 0x49,
            INS_EOR_ZP = 0x45,
            INS_EOR_ZPX = 0x55,
            INS_EOR_ABS = 0x4D,
            INS_EOR_ABSX = 0x5D,
            INS_EOR_ABSY = 0x59,
            INS_EOR_INDX = 0x41,
            INS_EOR_INDY = 0x51,
            //Logical ORA
            INS_ORA_IM = 0x09,
            INS_ORA_ZP = 0x05,
            INS_ORA_ZPX = 0x15,
            INS_ORA_ABS = 0x0D,
            INS_ORA_ABSX = 0x1D,
            INS_ORA_ABSY = 0x19,
            INS_ORA_INDX = 0x01,
            INS_ORA_INDY = 0x11,
            //Logical BIT
            INS_BIT_ZP = 0x24,
            INS_BIT_ABS = 0x2C,

            //Jumps and Calls
            INS_JSR = 0x20,
            INS_RTS = 0x60,
            INS_JMP_ABS = 0x4C,
            INS_JMP_IND = 0x6C;

    void LoadRegisterSetStatus(Byte Register)
    {
        Flag.Z = (Register==0);
        Flag.N = (Register & 0b10000000) > 0;
    }


    s32 Execute( s32 Cycles, Mem& memory );

    /**Addressing Mode - Zero page */
    Word AddrZeroPage(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Zero page X */
    Word AddrZeroPageX(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Zero page Y */
    Word AddrZeroPageY(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Absolute */
    Word AddrAbsolute(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Absolute X */
    Word AddrAbsoluteX(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Absolute X _ 5 cycles */
    Word AddrAbsoluteX_5(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Absolute Y */
    Word AddrAbsoluteY(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Absolute Y _ 5 cycles */
    Word AddrAbsoluteY_5(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Indirect X */
    Word AddrIndirectX(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Indirect Y */
    Word AddrIndirectY(s32& Cycles, const Mem& memory);
    /**Addressing Mode - Indirect Y _ 6 cycles */
    Word AddrIndirectY_6(s32& Cycles, const Mem& memory);

};

#define EMULATECPU_MAIN_6502_H

#endif //EMULATECPU_MAIN_6502_H
