#include "m6502.h"
using namespace m6502;

Word CPU::AddrZeroPage(s32& Cycles, const Mem& memory)
{
    Byte ZeroPageAddr = FetchByte(Cycles, memory);
    return ZeroPageAddr;
}
Word CPU::AddrZeroPageX(s32& Cycles, const Mem& memory)
{
    Byte ZeroPageAddr = FetchByte(Cycles, memory);
    ZeroPageAddr += X;
    Cycles--;
    return ZeroPageAddr;
}
Word CPU::AddrZeroPageY(s32& Cycles, const Mem& memory)
{
    Byte ZeroPageAddr = FetchByte(Cycles, memory);
    ZeroPageAddr += Y;
    Cycles--;
    return ZeroPageAddr;
}
Word CPU::AddrAbsolute(s32& Cycles, const Mem& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    return AbsAddress;
}
Word CPU::AddrAbsoluteX(s32& Cycles, const Mem& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word AbsAddressX = AbsAddress + X;
    if (AbsAddressX - AbsAddress >= 0xFF)
        Cycles--;
    return AbsAddressX;
}
Word CPU::AddrAbsoluteX_5(s32& Cycles, const Mem& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word AbsAddressX = AbsAddress + X;
    Cycles--;
    return AbsAddressX;
}
Word CPU::AddrAbsoluteY(s32& Cycles, const Mem& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word AbsAddressY = AbsAddress + Y;
    if (AbsAddressY - AbsAddress >= 0xFF)
        Cycles--;
    return AbsAddressY;
}
Word CPU::AddrAbsoluteY_5(s32& Cycles, const Mem& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word AbsAddressY = AbsAddress + Y;
    Cycles--;
    return AbsAddressY;
}
Word CPU::AddrIndirectX(s32& Cycles, const Mem& memory)
{
    Byte ZPAddress = FetchByte(Cycles, memory);
    ZPAddress += X;
    Cycles--;
    Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
    return EffectiveAddr;
}
Word CPU::AddrIndirectY(s32& Cycles, const Mem& memory)
{
    Byte ZPAddress = FetchByte(Cycles, memory);
    Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
    Word EffectiveAddrY = EffectiveAddr + Y;
    if (EffectiveAddrY - EffectiveAddr >= 0xFF)
        Cycles--;
    return EffectiveAddrY;
}

Word CPU::AddrIndirectY_6(s32& Cycles, const Mem& memory)
{
    Byte ZPAddress = FetchByte(Cycles, memory);
    Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
    Word EffectiveAddrY = EffectiveAddr + Y;
    Cycles--;
    return EffectiveAddrY;
}

//Return number of cycles used
s32 CPU::Execute(s32 Cycles, Mem& memory)
{
    //Load a register from a memory address
    auto LoadRegister =
            [&Cycles, &memory, this]
            (Word Address, Byte& Register)
    {
        Register = ReadByte(Cycles, Address, memory);
        LoadRegisterSetStatus(Register);
    };
    //AND the A register from a memory address
    auto And =
            [&Cycles, &memory, this]
            (Word Address)
    {
        A &= ReadByte(Cycles, Address, memory);
        LoadRegisterSetStatus(A);
    };
    //EOR the A register from a memory address
    auto Eor =
            [&Cycles, &memory, this]
            (Word Address)
    {
        A ^= ReadByte(Cycles, Address, memory);
        LoadRegisterSetStatus(A);
    };
    //ORA the A register from a memory address
    auto Ora =
            [&Cycles, &memory, this]
            (Word Address)
    {
        A |= ReadByte(Cycles, Address, memory);
        LoadRegisterSetStatus(A);
    };

    const s32 CyclesRequested = Cycles;
    while (Cycles > 0)
    {
        Byte Ins = FetchByte(Cycles, memory);
        switch(Ins)
        {
            case INS_BIT_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                Byte Value = ReadByte(Cycles, Address, memory);
                Flag.Z = !(A & Value);
                PS |= (Value & 0b11000000);
            }break;
            case INS_BIT_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                Byte Value = ReadByte(Cycles, Address, memory);
                Flag.Z = !(A & Value);
                PS |= (Value & 0b11000000);
            }break;
            case INS_AND_IM:
            {
                A &= FetchByte(Cycles, memory);
                LoadRegisterSetStatus(A);
            }break;
            case INS_EOR_IM:
            {
                A ^= FetchByte(Cycles, memory);
                LoadRegisterSetStatus(A);
            }break;
            case INS_ORA_IM:
            {
                A |= FetchByte(Cycles, memory);
                LoadRegisterSetStatus(A);
            }break;
            case INS_AND_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_ABSX:
            {
                Word Address = AddrAbsoluteX(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_ABSX:
            {
                Word Address = AddrAbsoluteX(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_ABSX:
            {
                Word Address = AddrAbsoluteX(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_ABSY:
            {
                Word Address = AddrAbsoluteY(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_ABSY:
            {
                Word Address = AddrAbsoluteY(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_ABSY:
            {
                Word Address = AddrAbsoluteY(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_INDX:
            {
                Word Address = AddrIndirectX(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_INDX:
            {
                Word Address = AddrIndirectX(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_INDX:
            {
                Word Address = AddrIndirectX(Cycles, memory);
                Ora(Address);
            }break;
            case INS_AND_INDY:
            {
                Word Address = AddrIndirectY(Cycles, memory);
                And(Address);
            }break;
            case INS_EOR_INDY:
            {
                Word Address = AddrIndirectY(Cycles, memory);
                Eor(Address);
            }break;
            case INS_ORA_INDY:
            {
                Word Address = AddrIndirectY(Cycles, memory);
                Ora(Address);
            }break;
            case INS_LDA_IM:
            {
                A = FetchByte(Cycles, memory);
                LoadRegisterSetStatus(A);
            } break;
            case INS_LDX_IM:
            {
                X = FetchByte(Cycles, memory);
                LoadRegisterSetStatus(X);
            } break;
            case INS_LDY_IM:
            {
                Y = FetchByte(Cycles, memory);
                LoadRegisterSetStatus(Y);
            } break;
            case INS_LDA_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                LoadRegister(Address, A);
            } break;
            case INS_LDX_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                LoadRegister(Address, X);
            } break;
            case INS_LDY_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                LoadRegister(Address, Y);
            } break;
            case INS_LDA_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                LoadRegister(Address, A);
            } break;
            case INS_LDY_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                LoadRegister(Address, Y);
            } break;
            case INS_LDX_ZPY:
            {
                Word Address = AddrZeroPageY(Cycles, memory);
                LoadRegister(Address, X);
            } break;
            case INS_LDA_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                LoadRegister(Address, A);
            }break;
            case INS_LDX_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                LoadRegister(Address, X);
            }break;
            case INS_LDY_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                LoadRegister(Address, Y);
            }break;
            case INS_LDA_ABSX:
            {
                Word Address = AddrAbsoluteX(Cycles, memory);
                LoadRegister(Address, A);
            }break;
            case INS_LDY_ABSX:
            {
                Word Address = AddrAbsoluteX(Cycles, memory);
                LoadRegister(Address, Y);
            }break;
            case INS_LDA_ABSY:
            {
                Word Address = AddrAbsoluteY(Cycles, memory);
                LoadRegister(Address, A);
            }break;
            case INS_LDX_ABSY:
            {
                Word Address = AddrAbsoluteY(Cycles, memory);
                LoadRegister(Address, X);
            }break;
            case INS_LDA_INDX:
            {
                Word Address = AddrIndirectX(Cycles, memory);
                LoadRegister(Address, A);
            }break;
            case INS_LDA_INDY:
            {
               Word Address = AddrIndirectY(Cycles, memory);
                LoadRegister(Address, A);
            }break;
            case INS_STA_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STX_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                WriteByte(X, Cycles, Address, memory);
            }break;
            case INS_STY_ZP:
            {
                Word Address = AddrZeroPage(Cycles, memory);
                WriteByte(Y, Cycles, Address, memory);
            }break;
            case INS_STA_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STX_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                WriteByte(X, Cycles, Address, memory);
            }break;
            case INS_STY_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                WriteByte(Y, Cycles, Address, memory);
            }break;
            case INS_STA_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STY_ZPX:
            {
                Word Address = AddrZeroPageX(Cycles, memory);
                WriteByte(Y, Cycles, Address, memory);
            }break;
            case INS_STX_ZPY:
            {
                Word Address = AddrZeroPageY(Cycles, memory);
                WriteByte(X, Cycles, Address, memory);
            }break;
            case INS_STA_ABSX:
            {
                Word Address = AddrAbsoluteX_5(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STA_ABSY:
            {
                Word Address = AddrAbsoluteY_5(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STA_INDX:
            {
                Word Address = AddrIndirectX(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_STA_INDY:
            {
                Word Address = AddrIndirectY_6(Cycles, memory);
                WriteByte(A, Cycles, Address, memory);
            }break;
            case INS_TSX:
            {
                X = SP;
                Cycles--;
                LoadRegisterSetStatus(X);
            }break;
            case INS_TXS:
            {
                SP = X;
                Cycles--;
            }break;
            case INS_PHA:
            {
                PushByteOntoStack(Cycles, A,  memory);
            }break;
            case INS_PLA:
            {
                A = PopByteFromStack(Cycles, memory);
                LoadRegisterSetStatus(A);
            }break;
            case INS_PHP:
            {
                PushByteOntoStack(Cycles, PS,  memory);
            }break;
            case INS_PLP:
            {
                PS = PopByteFromStack(Cycles, memory);
            }break;
            case INS_JSR:
            {
                Word SubAddr = FetchWord(Cycles, memory);
                PushPCToStack(Cycles, memory);
                PC = SubAddr;
                Cycles--;
            }break;
            case INS_RTS:
            {
                Word ReturnAddress = PopWordFromStack(Cycles, memory);
                PC = ReturnAddress + 1;
                Cycles -= 2;
            }break;
            case INS_JMP_ABS:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                PC = Address;
            }break;
//            NB:
//                An original 6502 has does not correctly fetch the target address if the indirect vector
//                falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). In this case
//                fetches the LSB from $xxFF as expected but takes the MSB from $xx00. This is fixed in some
//                later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end
//                of the page.
            case INS_JMP_IND:
            {
                Word Address = AddrAbsolute(Cycles, memory);
                Address = ReadWord(Cycles, Address, memory);
                PC = Address;
            }break;
            default:
            {
                printf("Instruction %d not handled\n", Ins);
                throw -1;
            } break;
        }
    }
    const s32 NumCyclesUsed = CyclesRequested - Cycles;
    return NumCyclesUsed;
}