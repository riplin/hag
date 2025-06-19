//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        DRAMInit0 = 0x18
    };
}

typedef uint32_t DRAMInit0_t;
namespace DRAMInit0
{
    enum //Mask
    {
        RowActiveToRowActive =              0x00000003, // tRRD - row active to row active (1-4 clks). Default is 0x1 (2 clks).
        RowActiveToRowActive1Cycle =        0x00000000,
        RowActiveToRowActive2Cycles =       0x00000001,
        RowActiveToRowActive3Cycles =       0x00000002,
        RowActiveToRowActive4Cycles =       0x00000003,
        RASToCASDelay =                     0x0000000c, // tRCD - RAS to CAS delay (1-4 clks). Default is 0x2 (3 clks).
        RASToCASDelay1Cycle =               0x00000000,
        RASToCASDelay2Cycles =              0x00000004,
        RASToCASDelay3Cycles =              0x00000008,
        RASToCASDelay4Cycles =              0x0000000c,
        RowPreCharge =                      0x00000030, // tRP - row precharge (1-4 clks). Default is 0x2 (3 clks).
        RowPreCharge1Cycle =                0x00000000,
        RowPreCharge2Cycles =               0x00000010,
        RowPreCharge3Cycles =               0x00000020,
        RowPreCharge4Cycles =               0x00000030,
        RASMinimumActiveTime =              0x000003c0, // tRAS - minimum active time (1-16 clks). Default is 0x4 (5 clks).
        RASMinimumActive1Cycle =            0x00000000,
        RASMinimumActive2Cycles =           0x00000040,
        RASMinimumActive3Cycles =           0x00000080,
        RASMinimumActive4Cycles =           0x000000c0,
        RASMinimumActive5Cycles =           0x00000100,
        RASMinimumActive6Cycles =           0x00000140,
        RASMinimumActive7Cycles =           0x00000180,
        RASMinimumActive8Cycles =           0x000001c0,
        RASMinimumActive9Cycles =           0x00000200,
        RASMinimumActive10Cycles =          0x00000240,
        RASMinimumActive11Cycles =          0x00000280,
        RASMinimumActive12Cycles =          0x000002c0,
        RASMinimumActive13Cycles =          0x00000300,
        RASMinimumActive14Cycles =          0x00000340,
        RASMinimumActive15Cycles =          0x00000380,
        RASMinimumActive16Cycles =          0x000003c0,
        MinimumRowCycleTime =               0x00003c00, // tRC - minimum row cycle time (1-16 clks). Default is 0x7 (8 clks).
        MinimumRowCyle1Cycle =              0x00000000,
        MinimumRowCyle2Cycles =             0x00000400,
        MinimumRowCyle3Cycles =             0x00000800,
        MinimumRowCyle4Cycles =             0x00000c00,
        MinimumRowCyle5Cycles =             0x00001000,
        MinimumRowCyle6Cycles =             0x00001400,
        MinimumRowCyle7Cycles =             0x00001800,
        MinimumRowCyle8Cycles =             0x00001c00,
        MinimumRowCyle9Cycles =             0x00002000,
        MinimumRowCyle10Cycles =            0x00002400,
        MinimumRowCyle11Cycles =            0x00002800,
        MinimumRowCyle12Cycles =            0x00002c00,
        MinimumRowCyle13Cycles =            0x00003000,
        MinimumRowCyle14Cycles =            0x00003400,
        MinimumRowCyle15Cycles =            0x00003800,
        MinimumRowCyle16Cycles =            0x00003c00,
        CASLatency =                        0x0000c000, // tCAS latency (1-4 clks). Default is 0x2 (3 clks).
        CASLatency1Cycle =                  0x00000000,
        CASLatency2Cycles =                 0x00004000,
        CASLatency3Cycles =                 0x00008000,
        CASLatency4Cycles =                 0x0000c000,
        ModeAndSpecialModeRegCycleTime =    0x00010000, // tMRS mode and special mode register cycle time (1-2 clks).. Default is 0x1 (2 clks)
        ModeAndSpecialModeReg1Cycle =       0x00000000,
        ModeAndSpecialModeReg2Cycles =      0x00010000,
        RdToDQMAssertionDelay =             0x00020000, // tDQR Rd to DQM assertion delay (0-1 clks). Default is 0x1 (1 clk)
        RdToDQMAssertion0Cycles =           0x00000000,
        RdToDQMAssertion1Cycle =            0x00020000,
        BlockWriteCycleTime =               0x00040000, // tBWC Block write cycle time (1-2 clks). Default is 0x1 (2 clks)
        BlockWrite1Cycle =                  0x00000000,
        BlockWrite2Cycles =                 0x00040000,
        WRToPre =                           0x00080000, // tWL WR to pre (1-2 clks). Default is 0x0 (1 clk)
        WRToPre1Cycle =                     0x00000000,
        WRToPre2Cycles =                    0x00080000,
        BKWRToPre =                         0x00300000, // tBWL BKWR to Pre (1-4 clks). Default is 0x1 (2 clks)
        BKWRToPre1Cycle =                   0x00000000,
        BKWRToPre2Cycles =                  0x00100000,
        BKWRToPre3Cycles =                  0x00200000,
        BKWRToPre4Cycles =                  0x00300000,
        RDToPre =                           0x00400000, // tRL RD to PRE (1-2 clks). Default is 0x1 (2 clks)
        RDToPre1Cycle =                     0x00000000,
        RDToPre2Cycles =                    0x00400000,
        WriteTerminateOrBST =               0x00800000, // dont allow WR/BKWR to terminate RD, use BST. Default is 0x0 (allow wr-term)
        AllowWRToTerminateRD =              0x00000000,
        UseBST =                            0x00800000,
        DeadBusCycle =                      0x01000000, // Disable the dead bus cycle btw. RD and WR (0=enable, 1=disable). Default is 0x0
        DeadBusCycleEnable =                0x00000000,
        DeadBusCycleDisable =               0x01000000,
        SGRAMWriteBit =                     0x02000000, // SGRAM write per bit enable (0=disable, 1=enable). Default is 0x0
        SGRAMWriteBitDisable =              0x00000000,
        SGRAMWriteBitEnable =               0x02000000,
        SGRAMChipSetCount =                 0x04000000, // Number of Sgram chipsets (0=1, 1=2). Default is 0x0. (power on strap = VMI_DATA_5)
        SGRAMChipSets1 =                    0x00000000,
        SGRAMChipSets2 =                    0x04000000,
        SGRAMChipSize =                     0x08000000, // Sgram type (0=8Mbit, 1=16Mbit). Default is 0x0. (power on strap = VMI_DATA_6)
        SGRAMChipSize8MBit =                0x00000000,
        SGRAMChipSize16MBit =               0x08000000
    };

    namespace Shift
    {
        enum
        {
            RowActiveToRowActive =              0x00,
            RASToCASDelay =                     0x02,
            RowPreCharge =                      0x04,
            RASMinimumActiveTime =              0x06,
            MinimumRowCycleTime =               0x0a,
            CASLatency =                        0x0e,
            ModeAndSpecialModeRegCycleTime =    0x10,
            RdToDQMAssertionDelay =             0x11,
            BlockWriteCycleTime =               0x12,
            WRToPre =                           0x13,
            BKWRToPre =                         0x14,
            RDToPre =                           0x16,
            WriteTerminateOrBST =               0x17,
            DeadBusCycle =                      0x18,
            SGRAMWriteBit =                     0x19,
            SGRAMChipSetCount =                 0x1a,
            SGRAMChipSize =                     0x1b
        };
    }
}

}