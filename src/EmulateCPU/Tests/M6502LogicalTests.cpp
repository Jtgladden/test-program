//
// Created by jt_gladden on 5/21/2021.
//
#include <gtest/gtest.h>
#include "../m6502.h"
using namespace m6502;

static void VerifyUnmodifiedFlagsFromLogical(
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

class M6502LogicalTests : public testing::Test
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
    enum class ELogicalOp
    {
        And, Eor, Or
    };

    Byte DoLogicalOp(Byte A, Byte B, ELogicalOp LogicalOp)
    {
        switch ( LogicalOp) {
            case ELogicalOp::And:
                return A & B;
                break;
            case ELogicalOp::Or:
                return A | B;
                break;
            case ELogicalOp::Eor:
                return A ^ B;
                break;
            default:
                throw -1; //invalid
                break;
        }
    }

    void TestLogicalImmediate(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_IM;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_IM;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_IM;
                break;
        }
        mem[0xFFFD] = 0x84;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 2;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x84, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical( cpu, CPUCopy );
    }

    void TestLogicalZeroPage(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ZP;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ZP;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ZP;
                break;
        }
        mem[0xFFFD] = 0x42;
        mem[0x0042] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 3;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalZeroPageX(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.X = 5;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ZPX;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ZPX;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ZPX;
                break;
        }
        mem[0xFFFD] = 0x42;
        mem[0x0047] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 4;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalAbsolute(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ABS;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ABS;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ABS;
                break;
        }
        mem[0xFFFD] = 0x80;
        mem[0xFFFE] = 0x44;
        mem[0x4480] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 4;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalAbsoluteX(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.X = 1;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ABSX;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ABSX;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ABSX;
                break;
        }
        mem[0xFFFD] = 0x80;
        mem[0xFFFE] = 0x44;
        mem[0x4481] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 4;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );;
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalAbsoluteXWhenCrossingPage( ELogicalOp LogicalOp )
    {
        // given:
        cpu.A = 0xCC;
        cpu.X = 0xFF;
        switch ( LogicalOp )
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ABSX;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ABSX;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ABSX;
                break;
        }
        mem[0xFFFD] = 0x02;
        mem[0xFFFE] = 0x44;	//0x4402
        mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;
        CPU CPUCopy = cpu;

        //when:
        s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical( cpu, CPUCopy );
    }

    void TestLogicalAbsoluteY(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.Y = 1;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ABSY;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ABSY;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ABSY;
                break;
        }
        mem[0xFFFD] = 0x80;
        mem[0xFFFE] = 0x44;
        mem[0x4481] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 4;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalAbsoluteYWhenCrossingPage( ELogicalOp LogicalOp )
    {
        // given:
        cpu.A = 0xCC;
        cpu.Y = 0xFF;
        switch ( LogicalOp )
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ABSY;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ABSY;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ABSY;
                break;
        }
        mem[0xFFFD] = 0x02;
        mem[0xFFFE] = 0x44;	//0x4402
        mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;
        CPU CPUCopy = cpu;

        //when:
        s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical( cpu, CPUCopy );
    }

    void TestLogicalIndirectX(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.X = 0x04;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_INDX;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_INDX;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_INDX;
                break;
        }
        mem[0xFFFD] = 0x02;
        mem[0x0006] = 0x00;
        mem[0x0007] = 0x80;
        mem[0x8000] = 0x37;


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 6;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalIndirectY(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.Y = 0x04;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_INDY;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_INDY;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_INDY;
                break;
        }
        mem[0xFFFD] = 0x02;
        mem[0x0002] = 0x00;
        mem[0x0003] = 0x80; //0x80 + 0x00 = 0x8000
        mem[0x8004] = 0x37; //0x8000 + 0x04


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 5;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalIndirectYWhenPageBoundaryIsCrossed(
            ELogicalOp LogicalOp
    )
    {
        cpu.A = 0xCC;
        cpu.Y = 0xFF;
        switch ( LogicalOp)
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_INDY;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_INDY;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_INDY;
                break;
        }
        mem[0xFFFD] = 0x02;
        mem[0x0002] = 0x02;
        mem[0x0003] = 0x80; //0x80 + 0x02 = 0x8002
        mem[0x8101] = 0x37; //0x8002 + 0xFF


        CPU CPUCopy = cpu;
        constexpr s32 NUM_CYCLES = 6;

        //when:
        s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, NUM_CYCLES );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical(cpu, CPUCopy);
    }

    void TestLogicalZeroPageXWhenItWraps
            ( ELogicalOp LogicalOp )
    {
        // given:
        using namespace m6502;
        cpu.A = 0xCC;
        cpu.X = 0xFF;
        switch ( LogicalOp )
        {
            case ELogicalOp::And:
                mem[0xFFFC] = CPU::INS_AND_ZPX;
                break;
            case ELogicalOp::Or:
                mem[0xFFFC] = CPU::INS_ORA_ZPX;
                break;
            case ELogicalOp::Eor:
                mem[0xFFFC] = CPU::INS_EOR_ZPX;
                break;
        }
        mem[0xFFFD] = 0x80;
        mem[0x007F] = 0x37;

        //when:
        CPU CPUCopy = cpu;
        s32 CyclesUsed = cpu.Execute( 4, mem );

        //then:
        const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
        const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
        EXPECT_EQ( cpu.A, ExpectedResult );
        EXPECT_EQ( CyclesUsed, 4 );
        EXPECT_FALSE( cpu.Flag.Z );
        EXPECT_EQ( cpu.Flag.N, ExpectedNegative );
        VerifyUnmodifiedFlagsFromLogical( cpu, CPUCopy );
    }


};

TEST_F(M6502LogicalTests, TestLogicalOpAndImmediateOnARegister)
{
    TestLogicalImmediate(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrImmediateOnARegister)
{
    TestLogicalImmediate(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorImmediateOnARegister)
{
    TestLogicalImmediate(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndZeroPageOnARegister)
{
    TestLogicalZeroPage(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrZeroPageOnARegister)
{
    TestLogicalZeroPage(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorZeroPageOnARegister)
{
    TestLogicalZeroPage(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndZeroPageXOnARegister)
{
    TestLogicalZeroPageX(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrZeroPageXOnARegister)
{
    TestLogicalZeroPageX(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorZeroPageXOnARegister)
{
    TestLogicalZeroPageX(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndZeroPageXOnARegisterWhenItWraps)
{
    TestLogicalZeroPageXWhenItWraps(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrZeroPageXOnARegisterWhenItWraps)
{
    TestLogicalZeroPageXWhenItWraps(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorZeroPageXOnARegisterWhenItWraps)
{
    TestLogicalZeroPageXWhenItWraps(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndAbsoluteOnARegister)
{
    TestLogicalAbsolute(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrAbsoluteOnARegister)
{
    TestLogicalAbsolute(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorAbsoluteOnARegister)
{
    TestLogicalAbsolute(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndAbsoluteXOnARegister)
{
    TestLogicalAbsoluteX(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrAbsoluteXOnARegister)
{
    TestLogicalAbsoluteX(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorAbsoluteXOnARegister)
{
    TestLogicalAbsoluteX(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndAbsoluteXOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalAbsoluteXWhenCrossingPage(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrAbsoluteXOnARegisteWhenPageBoundaryIsCrossedr)
{
    TestLogicalAbsoluteXWhenCrossingPage(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorAbsoluteXOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalAbsoluteXWhenCrossingPage(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndAbsoluteYOnARegister)
{
    TestLogicalAbsoluteY(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrAbsoluteYOnARegister)
{
    TestLogicalAbsoluteY(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorAbsoluteYOnARegister)
{
    TestLogicalAbsoluteY(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndAbsoluteYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalAbsoluteYWhenCrossingPage(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrAbsoluteYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalAbsoluteYWhenCrossingPage(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorAbsoluteYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalAbsoluteYWhenCrossingPage(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndIndirectXOnARegister)
{
    TestLogicalIndirectX(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrIndirectXOnARegister)
{
    TestLogicalIndirectX(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorIndirectXOnARegister)
{
    TestLogicalIndirectX(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndIndirectYOnARegister)
{
    TestLogicalIndirectY(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrIndirectYOnARegister)
{
    TestLogicalIndirectY(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorIndirectYOnARegister)
{
    TestLogicalIndirectY(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestLogicalOpAndIndirectYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalIndirectYWhenPageBoundaryIsCrossed(M6502LogicalTests::ELogicalOp::And);
}
TEST_F(M6502LogicalTests, TestLogicalOpOrIndirectYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalIndirectYWhenPageBoundaryIsCrossed(M6502LogicalTests::ELogicalOp::Or);
}
TEST_F(M6502LogicalTests, TestLogicalOpEorIndirectYOnARegisterWhenPageBoundaryIsCrossed)
{
    TestLogicalIndirectYWhenPageBoundaryIsCrossed(M6502LogicalTests::ELogicalOp::Eor);
}
TEST_F(M6502LogicalTests, TestBitZeroPage)
{
    //given
    cpu.A = 0xCC;
    mem[0xFFFC] = CPU::INS_BIT_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0xCC;


    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 3;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_EQ( cpu.A, 0xCC );
    EXPECT_FALSE( cpu.Flag.Z );
    EXPECT_TRUE( cpu.Flag.V );
    EXPECT_TRUE( cpu.Flag.N );
}
TEST_F(M6502LogicalTests, TestBitZeroPageResultZero)
{
    //given
    cpu.A = 0xCC;
    mem[0xFFFC] = CPU::INS_BIT_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x33;


    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 3;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_EQ( cpu.A, 0xCC );
    EXPECT_TRUE( cpu.Flag.Z );
    EXPECT_FALSE( cpu.Flag.V );
    EXPECT_FALSE( cpu.Flag.N );
}
TEST_F( M6502LogicalTests, TestBitZeroPageResultZeroBits6And7Zero )
{
    // given:
    using namespace m6502;
    cpu.Flag.V = cpu.Flag.N = false;
    cpu.A = 0x33;
    mem[0xFFFC] = CPU::INS_BIT_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0xCC;
    CPU CPUCopy = cpu;
    constexpr s32 EXPECTED_CYCLES = 3;

    //when:
    s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

    //then:
    EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
    EXPECT_EQ( cpu.A, 0x33 );
    EXPECT_TRUE( cpu.Flag.Z );
    EXPECT_TRUE( cpu.Flag.V );
    EXPECT_TRUE( cpu.Flag.N );
}TEST_F( M6502LogicalTests, TestBitZeroPageResultZeroBits6And7Mixed )
{
    // given:
    mem[0xFFFC] = CPU::INS_BIT_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0b01000000;
    constexpr s32 EXPECTED_CYCLES = 3;

    //when:
    cpu.Execute( EXPECTED_CYCLES, mem );

    //then:
    EXPECT_TRUE( cpu.Flag.V );
    EXPECT_FALSE( cpu.Flag.N );
}
TEST_F(M6502LogicalTests, TestBitAbsolute)
{
    //given
    cpu.A = 0xCC;
    mem[0xFFFC] = CPU::INS_BIT_ABS;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44;
    mem[0x4480] = 0xCC;


    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_EQ( cpu.A, 0xCC );
    EXPECT_FALSE( cpu.Flag.Z );
    EXPECT_TRUE( cpu.Flag.V );
    EXPECT_TRUE( cpu.Flag.N );
}
TEST_F(M6502LogicalTests, TestBitAbsoluteResultZero)
{
    //given
    cpu.A = 0xCC;
    mem[0xFFFC] = CPU::INS_BIT_ABS;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44;
    mem[0x4480] = 0x33;


    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
    EXPECT_EQ( cpu.A, 0xCC );
    EXPECT_TRUE( cpu.Flag.Z );
    EXPECT_FALSE( cpu.Flag.V );
    EXPECT_FALSE( cpu.Flag.N );
}
TEST_F( M6502LogicalTests, TestBitAbsoluteResultZeroBit6And7Zero )
{
    // given:
    using namespace m6502;
    cpu.Flag.V = cpu.Flag.N = false;
    cpu.A = 0x33;
    mem[0xFFFC] = CPU::INS_BIT_ABS;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8000] = 0xCC;
    CPU CPUCopy = cpu;
    constexpr s32 EXPECTED_CYCLES = 4;

    //when:
    s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

    //then:
    EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
    EXPECT_EQ( cpu.A, 0x33 );
    EXPECT_TRUE( cpu.Flag.Z );
    EXPECT_TRUE( cpu.Flag.V );
    EXPECT_TRUE( cpu.Flag.N );
}
TEST_F( M6502LogicalTests, TestBitAbsoluteResultZeroBit6And7Mixed )
{
    // given:
    mem[0xFFFC] = CPU::INS_BIT_ABS;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8000] = 0b10000000;
    CPU CPUCopy = cpu;
    constexpr s32 EXPECTED_CYCLES = 4;

    //when:
    cpu.Execute( EXPECTED_CYCLES, mem );

    //then:
    EXPECT_FALSE( cpu.Flag.V );
    EXPECT_TRUE( cpu.Flag.N );
}
