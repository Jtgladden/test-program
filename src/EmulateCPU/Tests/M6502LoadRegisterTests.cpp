//
// Created by jt_gladden on 5/13/2021.
//
#include <gtest/gtest.h>
#include "../m6502.h"
using namespace m6502;

class M6502LoadRegisterTests : public testing::Test
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

    void TestLoadRegisterImmediate(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterZeroPage(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterZeroPageX(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterZeroPageXWhenWrapping(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterZeroPageYWhenWrapping(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterZeroPageY(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterAbsolute(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterAbsoluteX(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterAbsoluteXWhenCrossingPageBoundary(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterAbsoluteY(
            Byte OpcodeToTest,
            Byte CPU::*Register);

    void TestLoadRegisterAbsoluteYWhenCrossingPageBoundary(
            Byte OpcodeToTest,
            Byte CPU::*Register);
};

static void VerifyUnmodifiedFlagsFromLoadRegister(
        const m6502::CPU& cpu,
        const m6502::CPU& CPUCopy
        )
{
    EXPECT_EQ(cpu.Flag.C, CPUCopy.Flag.C);
    EXPECT_EQ(cpu.Flag.I, CPUCopy.Flag.I);
    EXPECT_EQ(cpu.Flag.D, CPUCopy.Flag.D);
    EXPECT_EQ(cpu.Flag.B, CPUCopy.Flag.B);
    EXPECT_EQ(cpu.Flag.V, CPUCopy.Flag.V);
}

void M6502LoadRegisterTests::TestLoadRegisterImmediate(
        Byte OpcodeToTest,
        Byte CPU::*Register
        )
{
    //given:
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x84;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 2;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x84);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_TRUE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPage(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x42;
    mem[0x00042] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 3;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageX(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.X = 5;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x42;
    mem[0x00047] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageXWhenWrapping(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.X = 0xFF;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x80;
    mem[0x0007F] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageYWhenWrapping(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.Y = 0xFF;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x80;
    mem[0x0007F] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageY(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.Y = 5;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x42;
    mem[0x00047] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterAbsolute(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44; //0x4480
    mem[0x4480] = 0x37;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteX(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.X = 1;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44; //0x4480
    mem[0x4481] = 0x37; // 0x4480 + 0x01
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteXWhenCrossingPageBoundary(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.X = 0xFF;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x02;
    mem[0xFFFE] = 0x44; //0x4402
    mem[0x4501] = 0x37; // 0x4402 + 0xFF crosses page boundary
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 5;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteY(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.Y = 1;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44; //0x4480
    mem[0x4481] = 0x37; // 0x4480 + 0x01
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteYWhenCrossingPageBoundary(
        Byte OpcodeToTest,
        Byte CPU::*Register
)
{
    //given:
    cpu.Flag.Z = cpu.Flag.N = 1;
    cpu.Y = 0xFF;
    mem[0xFFFC] = OpcodeToTest;
    mem[0xFFFD] = 0x02;
    mem[0xFFFE] = 0x44; //0x4402
    mem[0x4501] = 0x37; // 0x4402 + 0xFF crosses page boundary
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 5;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.*Register, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}


TEST_F(M6502LoadRegisterTests, TheCPUDoesNothingWhenWeExecuteZeroCycles)
{
    //given:
    constexpr s32 NUM_CYCLES = 0;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, 0);
}

TEST_F(M6502LoadRegisterTests, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction)
{
    //given:
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 1;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, 2);
}

TEST_F(M6502LoadRegisterTests, LDAImmediateCanAffectTheZeroFlag)
{
    //given:
    cpu.A = 0x44;
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x00;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 2;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_TRUE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}


TEST_F(M6502LoadRegisterTests, LDAImmediateCanLoadAValueIntoTheARegister)
{
    //given:
    TestLoadRegisterImmediate(CPU::INS_LDA_IM, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDXImmediateCanLoadAValueIntoTheXRegister)
{
    //given:
    TestLoadRegisterImmediate(CPU::INS_LDX_IM, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDYImmediateCanLoadAValueIntoTheYRegister)
{
    //given:
    TestLoadRegisterImmediate(CPU::INS_LDY_IM, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAZeroPageCanLoadAValueIntoTheARegister)
{
    //given:
    TestLoadRegisterZeroPage(CPU::INS_LDA_ZP, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDXZeroPageCanLoadAValueIntoTheXRegister)
{
    //given:
    TestLoadRegisterZeroPage(CPU::INS_LDX_ZP, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDYZeroPageCanLoadAValueIntoTheXRegister)
{
    //given:
    TestLoadRegisterZeroPage(CPU::INS_LDY_ZP, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegister)
{
    //given:
    TestLoadRegisterZeroPageX(CPU::INS_LDA_ZPX, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDYZeroPageXCanLoadAValueIntoTheYRegister)
{
    //given:
    TestLoadRegisterZeroPageX(CPU::INS_LDY_ZPX, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDXZeroPageYCanLoadAValueIntoTheXRegister)
{
    //given:
    TestLoadRegisterZeroPageY(CPU::INS_LDX_ZPY, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDXZeroPageYCanLoadAValueIntoTheXRegisterWhenItWraps)
{
    //given:
    TestLoadRegisterZeroPageYWhenWrapping(CPU::INS_LDX_ZPY, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps)
{
    //given:
    TestLoadRegisterZeroPageXWhenWrapping(CPU::INS_LDA_ZPX, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDYZeroPageXCanLoadAValueIntoTheYRegisterWhenItWraps)
{
    //given:
    TestLoadRegisterZeroPageXWhenWrapping(CPU::INS_LDY_ZPX, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAAbsoluteCanLoadAValueIntoTheARegister)
{
    //given:
    TestLoadRegisterAbsolute(CPU::INS_LDA_ABS, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDXAbsoluteCanLoadAValueIntoTheXRegister)
{
    //given:
    TestLoadRegisterAbsolute(CPU::INS_LDX_ABS, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDYAbsoluteCanLoadAValueIntoTheYRegister)
{
    //given:
    TestLoadRegisterAbsolute(CPU::INS_LDY_ABS, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary)
{
    //given:
    TestLoadRegisterAbsoluteXWhenCrossingPageBoundary(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWhenItCrossesAPageBoundary)
{
    //given:
    TestLoadRegisterAbsoluteXWhenCrossingPageBoundary(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(M6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary)
{
    //given:
    TestLoadRegisterAbsoluteYWhenCrossingPageBoundary(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(M6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWhenItCrossesAPageBoundary)
{
    //given:
    TestLoadRegisterAbsoluteYWhenCrossingPageBoundary(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(M6502LoadRegisterTests, LDAIndirectXCanLoadAValueIntoTheARegister)
{
    //given:
    cpu.X = 0x04;
    mem[0xFFFC] = CPU::INS_LDA_INDX;
    mem[0xFFFD] = 0x02;
    mem[0x0006] = 0x00; //0x02 + 0x04 = 0x0006
    mem[0x0007] = 0x80;
    mem[0x8000] = 0x37; //0x80 + 0x00 = 0x8000
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 6;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

TEST_F(M6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegister)
{
    //given:
    cpu.Y = 0x04;
    mem[0xFFFC] = CPU::INS_LDA_INDY;
    mem[0xFFFD] = 0x02;
    mem[0x0002] = 0x00;
    mem[0x0003] = 0x80; //0x80 + 0x00 = 0x8000
    mem[0x8004] = 0x37; //0x8000 + 0x04
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 5;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

TEST_F(M6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary)
{
    //given:
    cpu.Y = 0xFF;
    mem[0xFFFC] = CPU::INS_LDA_INDY;
    mem[0xFFFD] = 0x02;
    mem[0x0002] = 0x02;
    mem[0x0003] = 0x80; //0x80 + 0x02 = 0x8002
    mem[0x8101] = 0x37; //0x8002 + 0xFF
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 6;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_FALSE(cpu.Flag.Z);
    EXPECT_FALSE(cpu.Flag.N);
    VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}


