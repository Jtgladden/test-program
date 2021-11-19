//
// Created by jt_gladden on 5/20/2021.
//
#include <gtest/gtest.h>
#include "../m6502.h"
using namespace m6502;

static void VerifyUnmodifiedFlagsFromJumpsAndCalls(
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

class M6502JumpsAndCallsTests: public testing::Test {
public:
    m6502::Mem mem;
    m6502::CPU cpu;

    virtual void SetUp() {
        cpu.reset(mem);
    }

    virtual void TearDown() {
    }
};

TEST_F(M6502JumpsAndCallsTests, CanJumpToASubroutineAndJumpBackAgain)
{
    //given:
    cpu.Reset(0xFF00, mem);
    mem[0xFF00] = CPU::INS_JSR;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x80;
    mem[0x8000] = CPU::INS_RTS;
    mem[0xFF03] = CPU::INS_LDA_IM;
    mem[0xFF04] = 0x42;
    constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x42);
    EXPECT_EQ(cpu.SP, CPUCopy.SP);
}
TEST_F(M6502JumpsAndCallsTests, JSRDoesNotEffectTheProgramStatusFlags)
{
    //given:
    cpu.Reset(0xFF00, mem);
    mem[0xFF00] = CPU::INS_JSR;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x80;
    constexpr s32 EXPECTED_CYCLES = 6;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
    EXPECT_NE(cpu.SP, CPUCopy.SP);
    EXPECT_EQ(cpu.PC, 0x8000);
    VerifyUnmodifiedFlagsFromJumpsAndCalls(cpu, CPUCopy);
}
TEST_F(M6502JumpsAndCallsTests, RTSDoesNotEffectTheProgramStatusFlags)
{
    //given:
    cpu.Reset(0xFF00, mem);
    mem[0xFF00] = CPU::INS_JSR;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x80;
    mem[0x8000] = CPU::INS_RTS;
    constexpr s32 EXPECTED_CYCLES = 6 + 6;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
    EXPECT_EQ(cpu.PC, 0xFF03);
    VerifyUnmodifiedFlagsFromJumpsAndCalls(cpu, CPUCopy);
}
TEST_F(M6502JumpsAndCallsTests, JMPAbsoluteCanJumpToANewLocationInTheProgram)
{
    //given:
    cpu.Reset(0xFF00, mem);
    mem[0xFF00] = CPU::INS_JMP_ABS;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x80;
    constexpr s32 EXPECTED_CYCLES = 3;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
    EXPECT_EQ(cpu.SP, CPUCopy.SP);
    EXPECT_EQ(cpu.PC, 0x8000);
    VerifyUnmodifiedFlagsFromJumpsAndCalls(cpu, CPUCopy);
}
TEST_F(M6502JumpsAndCallsTests, JMPIndirectCanJumpToANewLocationInTheProgram)
{
    //given:
    cpu.Reset(0xFF00, mem);
    mem[0xFF00] = CPU::INS_JMP_IND;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x80;
    mem[0x8000] = 0x00;
    mem[0x8001] = 0x90;
    constexpr s32 EXPECTED_CYCLES = 5;
    CPU CPUCopy = cpu;

    //When:
    const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

    //Then:
    EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, CPUCopy.PS);
    EXPECT_EQ(cpu.SP, CPUCopy.SP);
    EXPECT_EQ(cpu.PC, 0x9000);
    VerifyUnmodifiedFlagsFromJumpsAndCalls(cpu, CPUCopy);
}