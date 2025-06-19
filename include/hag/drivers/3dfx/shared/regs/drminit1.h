//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        DRAMInit1 = 0x1c
    };
}

typedef uint32_t DRAMInit1_t;
namespace DRAMInit1
{
    enum //Mask
    {
        SGRAMRefreshControl             = 0x00000001, // SGRAM Refresh Control
        SGRAMRefreshDisable             = 0x00000000,
        SGRAMRefreshEnable              = 0x00000001,
        SGRAMRefreshLoad                = 0x000003fe, // 9:1 Refresh_load Value. (Internal 14-bit counter 5 LSBs are 0x0) Default is 0x100.
        VideoArbitration                = 0x00000400, // 10 Video arbitration priority. (0=normal, 1=aggressive) Default is 0
        VideoArbitrationNormal          = 0x00000000,
        VideoArbitrationAggressive      = 0x00000400,
        Buffering                       = 0x00000800,
        DoubleBuffering                 = 0x00000000,
        TripleBuffering                 = 0x00000800,
        Dithering                       = 0x00001000,
        DitherDisable                   = 0x00001000,
        DitherEnable                    = 0x00000000,
        SGRAMReadDataSamplingControl    = 0x00002000,
        SGRAMDelay                      = 0x00000000,
        SGRAMDelayBypass                = 0x00002000, // bypass the delay element
        SGRAMInvertedSample             = 0x00004000,
        SGRAMInvertedSampleDisable      = 0x00000000,
        SGRAMInvertedSampleEnable       = 0x00004000, // resample the flopped sgram data with another negative-edge flop
                                                      // before flopping data with mclk.
        SGRAMDelayClockInvert           = 0x00008000,
        SGRAMDelayClockInvertDisable    = 0x00000000,        
        SGRAMDelayClockInvertEnable     = 0x00008000, // invert delayed clock before using it.
        SGRAMClockAdjustment            = 0x000f0000, // delay value for sgram read data sample clock
                                                      // (2-62 NAND gates of delay, in steps of 4)
        SGRAMOutputDelayValue           = 0x00f00000, // Delay amount for clock out to SGRAMs.
        SGRAMOutputNoDelay              = 0x01000000, // forces clock out to SGRAMs to have minimum delay
        MemControlPowerOn               = 0x02000000,
        MemControlLongPowerOn           = 0x00000000,
        MemControlShortPowerOn          = 0x02000000, // Power on in 128 cycles.
        RowScanActivation               = 0x04000000,
        RowScanAggressive               = 0x00000000,
        RowScanNormal                   = 0x04000000,
        MemPageBreak                    = 0x08000000,
        MemPageBreakOff                 = 0x00000000,
        MemPageBreakOn                  = 0x08000000,
        MemDataOutputs                  = 0x10000000,
        MemDataLow                      = 0x00000000,
        MemDataTristate                 = 0x10000000,
        MemVin                          = 0x20000000,
        MemVinLockBus                   = 0x00000000,
        MemVinDontLockBus               = 0x20000000,
        MemType                         = 0x40000000,
        MemTypeSGRAM                    = 0x00000000,
        MemTypeSDRAM                    = 0x40000000
    };

    namespace Shift
    {
        enum
        {
            SGRAMRefreshControl             = 0x00,
            SGRAMRefreshLoad                = 0x01,
            VideoArbitration                = 0x0a,
            Buffering                       = 0x0b,
            Dithering                       = 0x0c,
            SGRAMReadDataSamplingControl    = 0x0d,
            SGRAMInvertedSample             = 0x0e,
            SGRAMDelayClockInvert           = 0x0f,
            SGRAMClockAdjustment            = 0x10,
            SGRAMOutputDelayValue           = 0x14,
            SGRAMOutputNoDelay              = 0x18,
            MemControlPowerOn               = 0x19,
            RowScanActivation               = 0x1a,
            MemPageBreak                    = 0x1b,
            MemDataOutputs                  = 0x1c,
            MemVin                          = 0x1d,
            MemType                         = 0x1e
        };
    }
}

}