//Copyright 2025-Present riplin

#pragma once

#include <hag/testing/log.h>
#include <hag/drivers/3dfx/shared/fifo/pck2.h>
#include <hag/drivers/3dfx/shared/fifo/pck4.h>

// Things to keep in mind:
//
// Writing the same register multiple times before committing means only the last value in that register is committed.
// Any values that were written before a direct API call and committed after will stomp the direct API value.

namespace Hag::TDfx::Shared::Fifo::Deferred::TwoD
 {

    //Register cache
    extern Command_t s_Registers[Packet2::Index::Command + 1];

    //Number of written registers
    extern uint8_t s_Count;

    //Bitfield describing which registers contain a value
    extern uint32_t s_Mask;

    //Submit packet to fifo
    extern void Commit(uint8_t fifo);// 0 = fifo 0, anything else = fifo 1

    //Resets the content of the cache
    inline void Reset()
    {
        s_Mask = 0;
        s_Count = 0;
    }

    inline void WriteReg(uint8_t index, Command_t value)
    {
        if ((s_Mask & (1 << index)) == 0)
        {
            s_Mask |= uint32_t(1) << index;
            ++s_Count;
        }
        s_Registers[index] = value;
    }

    inline void SetClipRect0(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY)
    {
        LOG("Fifo::Deferred::TwoD", "SetClipRect0: minX: %i, minY: %i, maxX: %i, maxY: %i", minX, minY, maxX, maxY);

        WriteReg(Packet2::Index::Clip0Min, (Command_t(minX) << Shared::TwoD::Clip::Shift::X) | (Command_t(minY) << Shared::TwoD::Clip::Shift::Y));
        WriteReg(Packet2::Index::Clip0Max, (Command_t(maxX) << Shared::TwoD::Clip::Shift::X) | (Command_t(maxY) << Shared::TwoD::Clip::Shift::Y));
    }
    
    inline void SetClipRect1(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY)
    {
        LOG("Fifo::Deferred::TwoD", "SetClipRect1: minX: %i, minY: %i, maxX: %i, maxY: %i", minX, minY, maxX, maxY);

        WriteReg(Packet2::Index::Clip1Min, (Command_t(minX) << Shared::TwoD::Clip::Shift::X) | (Command_t(minY) << Shared::TwoD::Clip::Shift::Y));
        WriteReg(Packet2::Index::Clip1Max, (Command_t(maxX) << Shared::TwoD::Clip::Shift::X) | (Command_t(maxY) << Shared::TwoD::Clip::Shift::Y));
    }
    
    inline void SetLineStippleAndStyle(Shared::TwoD::LineStipple_t stipple, Shared::TwoD::LineStyle_t style)
    {
        LOG("Fifo::Deferred::TwoD", "SetLineStippleAndStyle: stipple: 0x%08lX, style: 0x%08lX", stipple, style);

        WriteReg(Packet2::Index::LineStipple, stipple);
        WriteReg(Packet2::Index::LineStyle, style);
    }

    inline void SetColors(Shared::TwoD::Color_t foregroundColor, Shared::TwoD::Color_t backgroundColor)
    {
        LOG("Fifo::Deferred::TwoD", "SetColors: foregroundColor: 0x%08lX, backgroundColor: 0x%08lX", foregroundColor, backgroundColor);

        WriteReg(Packet2::Index::BackgroundColor, backgroundColor);
        WriteReg(Packet2::Index::ForegroundColor, foregroundColor);
    }
    
    inline void SetForegroundColor(Shared::TwoD::Color_t color)
    {
        LOG("Fifo::Deferred::TwoD", "SetForegroundColor: color: 0x%08lX", color);

        WriteReg(Packet2::Index::ForegroundColor, color);
    }
    
    inline void SetBackgroundColor(Shared::TwoD::Color_t color)
    {
        LOG("Fifo::Deferred::TwoD", "SetBackgroundColor: color: 0x%08lX", color);

        WriteReg(Packet2::Index::BackgroundColor, color);
    }
    
    inline void SetSourceColorKey(Shared::TwoD::ColorKey_t colorKeyMin, Shared::TwoD::ColorKey_t colorKeyMax)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceColorKey: colorKeyMin: 0x%08lX, colorKeyMax: 0x%08lX", colorKeyMin, colorKeyMax);

        WriteReg(Packet2::Index::SourceColorKeyMin, colorKeyMin);
        WriteReg(Packet2::Index::SourceColorKeyMax, colorKeyMax);
    }
    
    inline void SetSourceBaseAddress(Shared::TwoD::BaseAddress_t baseAddress)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceBaseAddress: baseAddress: 0x%08lX", baseAddress);

        WriteReg(Packet2::Index::SourceBaseAddress, baseAddress);
    }
    
    inline void SetSourceBaseAddressSizeStrideAndFormat(Shared::TwoD::BaseAddress_t baseAddress, uint16_t stride, Shared::TwoD::SourceFormat_t bpp, Shared::TwoD::SourceFormat_t packing)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceBaseAddressSizeStrideAndFormat: baseAddress: 0x%08lX, stride: %i, bpp: 0x%08lX, packing: 0x%08lX", baseAddress, stride, bpp, packing);

        WriteReg(Packet2::Index::SourceBaseAddress, baseAddress);
        WriteReg(Packet2::Index::SourceFormat, (stride << Shared::TwoD::SourceFormat::Shift::Stride) | bpp | packing);
    }

    inline void SetSourceXY(uint16_t x, uint16_t y)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceXY: x: %i, y: %i", x, y);

        WriteReg(Packet2::Index::SourceXY, (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y));
    }

    inline void SetSourceXY(uint32_t xy)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceXY: xy: 0x%08lX", xy);

        WriteReg(Packet2::Index::SourceXY, xy);
    }
    
    inline void SetSourceRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        LOG("Fifo::Deferred::TwoD", "SetSourceRect: x: %i, y: %i, width: %i, height: %i", x, y, width, height);

        WriteReg(Packet2::Index::SourceSize, (width << Shared::TwoD::Size::Shift::Width) | (height << Shared::TwoD::Size::Shift::Height));
        WriteReg(Packet2::Index::SourceXY, (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y));
    }
    
    inline void SetDestinationColorKey(Shared::TwoD::ColorKey_t colorKeyMin, Shared::TwoD::ColorKey_t colorKeyMax)
    {
        LOG("Fifo::Deferred::TwoD", "SetDestinationColorKey: colorKeyMin: 0x%08lX, colorKeyMax: 0x%08lX", colorKeyMin, colorKeyMax);

        WriteReg(Packet2::Index::DestinationColorKeyMin, colorKeyMin);
        WriteReg(Packet2::Index::DestinationColorKeyMax, colorKeyMax);
    }
    
    inline void SetDestinationBaseAddress(Shared::TwoD::BaseAddress_t baseAddress)
    {
        LOG("Fifo::Deferred::TwoD", "SetDestinationBaseAddress: baseAddress: 0x%08lX", baseAddress);

        WriteReg(Packet2::Index::DestinationBaseAddress, baseAddress);
    }

    inline void SetDestinationXY(uint16_t x, uint16_t y)
    {
        LOG("Fifo::Deferred::TwoD", "SetDestinationXY: x: %i, y: %i", x, y);

        WriteReg(Packet2::Index::DestinationXY, (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y));
    }

    inline void SetDestinationXY(uint32_t xy)
    {
        LOG("Fifo::Deferred::TwoD", "SetDestinationXY: xy: 0x%08lX", xy);

        WriteReg(Packet2::Index::DestinationXY, xy);
    }

    inline void SetDestinationRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        LOG("Fifo::Deferred::TwoD", "SetDestinationRect: x: %i, y: %i, width: %i, height: %i", x, y, width, height);

        WriteReg(Packet2::Index::DestinationSize, (width << Shared::TwoD::Size::Shift::Width) | (height << Shared::TwoD::Size::Shift::Height));
        WriteReg(Packet2::Index::DestinationXY, (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y));
    }
    
    inline void SetBresenhamError0(Shared::TwoD::BresenhamError_t bresenhamError)
    {
        LOG("Fifo::Deferred::TwoD", "SetBresenhamError0: destinationBaseAddress: %lu", bresenhamError);

        WriteReg(Packet2::Index::BresenhamError0, bresenhamError);
    }
    
    inline void SetBresenhamError1(Shared::TwoD::BresenhamError_t bresenhamError)
    {
        LOG("Fifo::Deferred::TwoD", "SetBresenhamError1: destinationBaseAddress: %lu", bresenhamError);

        WriteReg(Packet2::Index::BresenhamError1, bresenhamError);
    }
    
    inline void SetBresenhamErrors(Shared::TwoD::BresenhamError_t bresenhamError0, Shared::TwoD::BresenhamError_t bresenhamError1)
    {
        LOG("Fifo::Deferred::TwoD", "SetBresenhamErrors: bresenhamError0: %lu, bresenhamError1: %lu", bresenhamError0, bresenhamError1);

        WriteReg(Packet2::Index::BresenhamError0, bresenhamError0);
        WriteReg(Packet2::Index::BresenhamError1, bresenhamError1);
    }
    
    inline void SetRasterOperations(Shared::TwoD::Rop_t rop1, Shared::TwoD::Rop_t rop2, Shared::TwoD::Rop_t rop3)
    {
        LOG("Fifo::Deferred::TwoD", "SetRasterOperations: rop1: 0x%02X, rop2: 0x%02X, rop3: 0x%02X", rop1, rop2, rop3);

        WriteReg(Packet2::Index::RasterOperation,
            (Shared::TwoD::RasterOperation_t(rop1) << Shared::TwoD::RasterOperation::Shift::RasterOperation1) |
            (Shared::TwoD::RasterOperation_t(rop2) << Shared::TwoD::RasterOperation::Shift::RasterOperation2) |
            (Shared::TwoD::RasterOperation_t(rop3) << Shared::TwoD::RasterOperation::Shift::RasterOperation3));
    }
    
    inline void SetPattern0Alias(Shared::TwoD::Pattern_t patternAlias)
    {
        LOG("Fifo::Deferred::TwoD", "SetPattern0Alias: patternAlias: 0x%08lX", patternAlias);

        WriteReg(Packet2::Index::Pattern0Alias, patternAlias);
    }
    
    inline void SetPattern1Alias(Shared::TwoD::Pattern_t patternAlias)
    {
        LOG("Fifo::Deferred::TwoD", "SetPattern1Alias: patternAlias: 0x%08lX", patternAlias);

        WriteReg(Packet2::Index::Pattern1Alias, patternAlias);
    }
    
    inline void SetPatternAliases(Shared::TwoD::Pattern_t pattern0Alias, Shared::TwoD::Pattern_t pattern1Alias)
    {
        LOG("Fifo::Deferred::TwoD", "SetPatternAliases: pattern0Alias: 0x%08lX, pattern1Alias: 0x%08lX", pattern0Alias, pattern1Alias);

        WriteReg(Packet2::Index::Pattern0Alias, pattern0Alias);
        WriteReg(Packet2::Index::Pattern1Alias, pattern1Alias);
    }

    inline void SetCommandAndExtra(Shared::TwoD::Command_t command, Shared::TwoD::Rop_t rop, Shared::TwoD::CommandExtra_t commandExtra)
    {
        LOG("Fifo::Deferred::TwoD", "SetCommandAndExtra: command: 0x%08lX, rop: 0x%02X, commandExtra: 0x%08lX", command, rop, commandExtra);

        WriteReg(Packet2::Index::CommandExtra, commandExtra);
        WriteReg(Packet2::Index::Command, (rop << Shared::TwoD::Command::Shift::RasterOperation0) | command);
    }
    
    inline void SetCommandExtra(Shared::TwoD::CommandExtra_t commandExtra)
    {
        LOG("Fifo::Deferred::TwoD", "SetCommandExtra: commandExtra: 0x%08lX", commandExtra);

        WriteReg(Packet2::Index::CommandExtra, commandExtra);
    }
    
    inline void SetCommand(Shared::TwoD::Command_t command, Shared::TwoD::Rop_t rop)
    {
        LOG("Fifo::Deferred::TwoD", "SetCommand: command: 0x%08lX, rop: 0x%02X", command, rop);

        WriteReg(Packet2::Index::Command, (rop << Shared::TwoD::Command::Shift::RasterOperation0) | command);
    }

}