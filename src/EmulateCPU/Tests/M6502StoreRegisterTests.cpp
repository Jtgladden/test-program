//
// Created by jt_gladden on 5/19/2021.
//
#include <gtest/gtest.h>
#include "../m6502.h"
using namespace m6502;

static void VerifyUnmodifiedFlagsFromStoreRegister(
        const m6502::CPU& cpu,
        const m6502::CPU& CPUCopy
)
{
    EXPECT_EQ(cpu.Flag.C, CPUCopy.Flag.C);
    EXPECT_EQ(cpu.Flag.I, CPUCopy.Flag.I);
    EXPECT_EQ(cpu.Flag.D, CPUCopy.Flag.D);
    EXPECT_EQ(cpu.Flag.B, CPUCopy.Flag.B);
    EXPECT_EQ(cpu.Flag.V, CPUCopy.Flag.V);
    EXPECT_EQ(cpu.Flag.Z, CPUCopy.Flag.Z);
    EXPECT_EQ(cpu.Flag.N, CPUCopy.Flag.N);
}

class M6502StoreRegisterTests : public testing::Test
{
public:
    m6502::Mem mem;
    m6502::CPU cpu;

    virtual void SetUp()
    {
        cpu.reset(mem);
    }

    virtual void TearDown()
    {
    }

    void TestStoreRegisterZeroPage(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x80;
        mem[0x0080] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 3;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x0080], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterZeroPageX(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.X = 0x0F;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x80;
        mem[0x008F] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x008F], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }
    void TestStoreRegisterZeroPageY(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.Y = 0x0F;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x80;
        mem[0x008F] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x008F], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterAbsolute(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x00;
        mem[0xFFFE] = 0x80;
        mem[0x8000] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x8000], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterAbsoluteX(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.X = 0x01;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x00;
        mem[0xFFFE] = 0x80;
        mem[0x8001] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 5;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x8001], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterAbsoluteY(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.Y = 0x01;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x00;
        mem[0xFFFE] = 0x80;
        mem[0x8001] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 5;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x8001], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterIndirectX(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.X = 0x04;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x02;
        mem[0x0006] = 0x80;
        mem[0x0007] = 0x00;
        mem[0x8000] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 6;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x0080], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }

    void TestStoreRegisterIndirectY(
            Byte OpcodeToTest,
            Byte CPU::*Register
    )
    {
        //given:
        cpu.*Register = 0x2F;
        cpu.Y = 0x04;
        mem[0xFFFC] = OpcodeToTest;
        mem[0xFFFD] = 0x02;
        mem[0x0002] = 0x80;
        mem[0x0003] = 0x00;
        mem[0x8004] = 0x00;
        constexpr s32 EXPECTED_CYCLES = 6;
        CPU CPUCopy = cpu;

        //When:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        //Then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(mem[0x0084], cpu.*Register);
        VerifyUnmodifiedFlagsFromStoreRegister(cpu, CPUCopy);
    }


};

TEST_F(M6502StoreRegisterTests, STAZeroPageCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STA_ZP, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STXZeroPageCanStoreTheXRegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STX_ZP, &CPU::X);
}
TEST_F(M6502StoreRegisterTests, STYZeroPageCanStoreTheYRegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STY_ZP, &CPU::Y);
}
TEST_F(M6502StoreRegisterTests, STAAbsoluteCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STA_ABS, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STXAbsoluteCanStoreTheXRegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STX_ABS, &CPU::X);
}
TEST_F(M6502StoreRegisterTests, STYAbsoluteCanStoreTheYRegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STY_ABS, &CPU::Y);
}
TEST_F(M6502StoreRegisterTests, STAZeroPageXCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterZeroPageX(CPU::INS_STA_ZPX, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STYZeroPageXCanStoreTheYRegisterIntoMemory)
{
    TestStoreRegisterZeroPageX(CPU::INS_STY_ZPX, &CPU::Y);
}
TEST_F(M6502StoreRegisterTests, STXZeroPageYCanStoreTheXRegisterIntoMemory)
{
    TestStoreRegisterZeroPageY(CPU::INS_STX_ZPY, &CPU::X);
}
TEST_F(M6502StoreRegisterTests, STAAbsoluteXCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterAbsoluteX(CPU::INS_STA_ABSX, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STAAbsoluteYCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterAbsoluteY(CPU::INS_STA_ABSY, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STAIndirectXCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterIndirectX(CPU::INS_STA_INDX, &CPU::A);
}
TEST_F(M6502StoreRegisterTests, STAIndirectYCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterIndirectY(CPU::INS_STA_INDY, &CPU::A);
}