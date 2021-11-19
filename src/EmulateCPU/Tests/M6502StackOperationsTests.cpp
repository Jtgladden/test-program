//
// Created by jt_gladden on 5/21/2021.
//
#include <gtest/gtest.h>
#include "../m6502.h"
using namespace m6502;

class M6502StackOperationsTests: public testing::Test {
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
};

TEST_F(M6502StackOperationsTests, TSXCanTransferTheStackPointerToXRegister)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.Flag.Z = cpu.Flag.N = true;
    cpu.SP = 0x01;
    cpu.X = 0x00;
    mem[0xFF00] = CPU::INS_TSX;
    constexpr s32 EXPECTED_CYCLES = 2;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0x01);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(M6502StackOperationsTests, TSXCanTransferAZeroStackPointerToXRegister)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.Flag.Z = cpu.Flag.N = true;
    cpu.SP = 0x00;
    cpu.X = 0x00;
    mem[0xFF00] = CPU::INS_TSX;
    constexpr s32 EXPECTED_CYCLES = 2;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_TRUE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(M6502StackOperationsTests, TSXCanTransferANegativeStackPointerToXRegister)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.Flag.Z = cpu.Flag.N = false;
    cpu.SP = 0b10000000;
    cpu.X = 0x00;
    mem[0xFF00] = CPU::INS_TSX;
    constexpr s32 EXPECTED_CYCLES = 2;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0b10000000);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_TRUE(cpu.Flag.N);
}
TEST_F(M6502StackOperationsTests, TXSCanTransferTheXRegisterToStackPointer)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.SP = 0x00;
    cpu.X = 0xFF;
    mem[0xFF00] = CPU::INS_TXS;
    constexpr s32 EXPECTED_CYCLES = 2;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.SP, 0xFF);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
}
TEST_F(M6502StackOperationsTests, PHACanPushARegisterOntoTheStack)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.A = 0x42;
    mem[0xFF00] = CPU::INS_PHA;
    constexpr s32 EXPECTED_CYCLES = 3;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(mem[cpu.SPToAddress() + 1], cpu.A);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
}
TEST_F(M6502StackOperationsTests, PLACanPullARegisterFromTheStack) {
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.A = 0x00;
    cpu.SP = 0xFE;
    mem[0x01FF] = 0x42;
    mem[0xFF00] = CPU::INS_PLA;
    constexpr s32 EXPECTED_CYCLES = 4;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x42);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
};
TEST_F(M6502StackOperationsTests, PLACanPullZeroValueIntoTheARegisterFromTheStack)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.Flag.Z = false;
    cpu.A = 0x42;
    cpu.SP = 0xFE;
    mem[0x01FF] = 0x00;
    mem[0xFF00] = CPU::INS_PLA;
    constexpr s32 EXPECTED_CYCLES = 4;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
}
TEST_F(M6502StackOperationsTests, PLACanPullNegativeValueIntoTheARegisterFromTheStack)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.Flag.N = false;
    cpu.A = 0x42;
    cpu.SP = 0xFE;
    mem[0x01FF] = 0b10000000;
    mem[0xFF00] = CPU::INS_PLA;
    constexpr s32 EXPECTED_CYCLES = 4;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0b10000000);
    EXPECT_TRUE(cpu.Flag.N);
    EXPECT_FALSE(cpu.Flag.Z);
}
TEST_F(M6502StackOperationsTests, PHPCanPushStatusFlagsOntoTheStack)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.PS = 0xCC;
    mem[0xFF00] = CPU::INS_PHP;
    constexpr s32 EXPECTED_CYCLES = 3;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(mem[cpu.SPToAddress() + 1], cpu.PS);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
    EXPECT_EQ(cpu.SP, 0xFE);
}
TEST_F(M6502StackOperationsTests, PLPCanPullStatusFlagsFromTheStack)
{
    //given:
    cpu.Reset(0xFF00, mem);
    cpu.PS = 0xFF;
    cpu.SP = 0xFE;
    mem[0x01FF] = 0xCC;
    mem[0xFF00] = CPU::INS_PLP;
    constexpr s32 EXPECTED_CYCLES = 4;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, 0xCC);
    EXPECT_EQ(cpu.SP, 0xFF);
}
