//Copyright 2025-Present riplin

#pragma once

#include <assert.h>
#include <has/testing/log.h>
#include <hag/drivers/3dfx/shared/fifo/pck2.h>
#include <hag/drivers/3dfx/shared/fifo/pck4.h>

// Things to keep in mind:
//
// The direct API will immediately emit a fifo packet with the data provided.
// The direct API is completely unaware of the deferred API!
// Any deferred values that get committed after a direct API call will override that direct value!

namespace Hag::TDfx::Shared::Fifo::Direct::ThreeD
{
    template <int F>
    void FastFill(bool dithering = false)
    {
        Packet4::Emit3D<F, Packet4::Registers<Packet4::ThreeD::Index::FastFillCommand, Packet4::ThreeD::Index::FastFillCommand>>(0, dithering ? Shared::ThreeD::Command::FastFill::NoDithering : 0);
    }
}

namespace Hag::TDfx::Shared::Fifo::Direct::TwoD
{

    template <int F>
    void SetClipRect0(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY)
    {
        LOG("Fifo::Direct::TwoD", "SetClipRect0: minX: %i, minY: %i, maxX: %i, maxY: %i", minX, minY, maxX, maxY);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Clip0Min, Packet2::Bit::Clip0Max>>
            (
                (Command_t(minX) << Shared::TwoD::Clip::Shift::X) | (Command_t(minY) << Shared::TwoD::Clip::Shift::Y),
                (Command_t(maxX) << Shared::TwoD::Clip::Shift::X) | (Command_t(maxY) << Shared::TwoD::Clip::Shift::Y)
            );
    }

    template <int F>
    void SetClipRect1(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY)
    {
        LOG("Fifo::Direct::TwoD", "SetClipRect1: minX: %i, minY: %i, maxX: %i, maxY: %i", minX, minY, maxX, maxY);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Clip1Min, Packet2::Bit::Clip1Max>>
            (
                (Command_t(minX) << Shared::TwoD::Clip::Shift::X) | (Command_t(minY) << Shared::TwoD::Clip::Shift::Y),
                (Command_t(maxX) << Shared::TwoD::Clip::Shift::X) | (Command_t(maxY) << Shared::TwoD::Clip::Shift::Y)
            );
    }

    template <int F>
    void SetLineStippleAndStyle(Shared::TwoD::LineStipple_t stipple, Shared::TwoD::LineStyle_t style)
    {
        LOG("Fifo::Direct::TwoD", "SetLineStippleAndStyle: stipple: 0x%08lX, style: 0x%08lX", stipple, style);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::LineStipple, Packet2::Bit::LineStyle>>
            (
                stipple,
                style
            );
    }

    template <int F>
    void SetColors(Shared::TwoD::Color_t foregroundColor, Shared::TwoD::Color_t backgroundColor)
    {
        LOG("Fifo::Direct::TwoD", "SetColors: foregroundColor: 0x%08lX, backgroundColor: 0x%08lX", foregroundColor, backgroundColor);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::BackgroundColor, Packet2::Bit::ForegroundColor>>
            (
                backgroundColor,
                foregroundColor
            );
    }

    template <int F>
    void SetForegroundColor(Shared::TwoD::Color_t color)
    {
        LOG("Fifo::Direct::TwoD", "SetForegroundColor: color: 0x%08lX", color);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::ForegroundColor>>
            (
                color
            );
    }

    template <int F>
    void SetBackgroundColor(Shared::TwoD::Color_t color)
    {
        LOG("Fifo::Direct::TwoD", "SetBackgroundColor: color: 0x%08lX", color);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::BackgroundColor>>
            (
                color
            );
    }

    template <int F>
    void SetSourceColorKey(Shared::TwoD::ColorKey_t colorKeyMin, Shared::TwoD::ColorKey_t colorKeyMax)
    {
        LOG("Fifo::Direct::TwoD", "SetSourceColorKey: colorKeyMin: 0x%08lX, colorKeyMax: 0x%08lX", colorKeyMin, colorKeyMax);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::SourceColorKeyMin, Packet2::Bit::SourceColorKeyMax>>
            (
                colorKeyMin,
                colorKeyMax
            );
    }

    template <int F>
    void SetSourceBaseAddress(Shared::TwoD::BaseAddress_t baseAddress)
    {
        LOG("Fifo::Direct::TwoD", "SetSourceBaseAddress: baseAddress: 0x%08lX", baseAddress);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::SourceBaseAddress>>
            (
                baseAddress
            );
    }

    template <int F>
    void SetSourceBaseAddressSizeStrideAndFormat(Shared::TwoD::BaseAddress_t baseAddress, uint16_t stride, Shared::TwoD::SourceFormat_t bpp, Shared::TwoD::SourceFormat_t packing)
    {
        LOG("Fifo::Direct::TwoD", "SetSourceBaseAddressSizeStrideAndFormat: baseAddress: 0x%08lX, stride: %i, bpp: 0x%08lX, packing: 0x%08lX", baseAddress, stride, bpp, packing);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::SourceBaseAddress, Packet2::Bit::SourceFormat>>
            (
                baseAddress,
                (stride << Shared::TwoD::SourceFormat::Shift::Stride) | bpp | packing
            );
    }

    template <int F>
    void SetSourceXY(uint16_t x, uint16_t y)
    {
        LOG("Fifo::Direct::TwoD", "SetSourceXY: x: %i, y: %i", x, y);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::SourceXY>>
            (
                (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y)
            );
    }

    template <int F>
    void SetSourceRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        LOG("Fifo::Direct::TwoD", "SetSourceRect: x: %i, y: %i, width: %i, height: %i", x, y, width, height);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::SourceXY, Packet2::Bit::SourceSize>>
            (
                (width << Shared::TwoD::Size::Shift::Width) | (height << Shared::TwoD::Size::Shift::Height),
                (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y)
            );
    }

    template <int F>
    void SetDestinationColorKey(Shared::TwoD::ColorKey_t colorKeyMin, Shared::TwoD::ColorKey_t colorKeyMax)
    {
        LOG("Fifo::Direct::TwoD", "SetDestinationColorKey: colorKeyMin: 0x%08lX, colorKeyMax: 0x%08lX", colorKeyMin, colorKeyMax);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::DestinationColorKeyMin, Packet2::Bit::DestinationColorKeyMax>>
            (
                colorKeyMin,
                colorKeyMax
            );
    }

    template <int F>
    void SetDestinationBaseAddress(Shared::TwoD::BaseAddress_t baseAddress)
    {
        LOG("Fifo::Direct::TwoD", "SetDestinationBaseAddress: baseAddress: 0x%08lX", baseAddress);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::DestinationBaseAddress>>
            (
                baseAddress
            );
    }

    template <int F>
    void SetDestinationXY(uint16_t x, uint16_t y)
    {
        LOG("Fifo::Direct::TwoD", "SetDestinationXY: x: %i, y: %i", x, y);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::DestinationXY>>
            (
                (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y)
            );
    }

    template <int F>
    void SetDestinationRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        LOG("Fifo::Direct::TwoD", "SetDestinationRect: x: %i, y: %i, width: %i, height: %i", x, y, width, height);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::DestinationXY, Packet2::Bit::DestinationSize>>
            (
                (width << Shared::TwoD::Size::Shift::Width) | (height << Shared::TwoD::Size::Shift::Height),
                (x << Shared::TwoD::XY::Shift::X) | (y << Shared::TwoD::XY::Shift::Y)
            );
    }

    template <int F>
    void SetBresenhamError0(Shared::TwoD::BresenhamError_t bresenhamError)
    {
        LOG("Fifo::Direct::TwoD", "SetBresenhamError0: destinationBaseAddress: %lu", bresenhamError);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::BresenhamError0>>
            (
                bresenhamError
            );
    }

    template <int F>
    void SetBresenhamError1(Shared::TwoD::BresenhamError_t bresenhamError)
    {
        LOG("Fifo::Direct::TwoD", "SetBresenhamError1: destinationBaseAddress: %lu", bresenhamError);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::BresenhamError1>>
            (
                bresenhamError
            );
    }

    template <int F>
    void SetBresenhamErrors(Shared::TwoD::BresenhamError_t bresenhamError0, Shared::TwoD::BresenhamError_t bresenhamError1)
    {
        LOG("Fifo::Direct::TwoD", "SetBresenhamErrors: bresenhamError0: %lu, bresenhamError1: %lu", bresenhamError0, bresenhamError1);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::BresenhamError0, Packet2::Bit::BresenhamError1>>
            (
                bresenhamError0,
                bresenhamError1
            );
    }

    template <int F>
    void SetRasterOperations(Shared::TwoD::Rop_t rop1, Shared::TwoD::Rop_t rop2, Shared::TwoD::Rop_t rop3)
    {
        LOG("Fifo::Direct::TwoD", "SetRasterOperations: rop1: 0x%02X, rop2: 0x%02X, rop3: 0x%02X", rop1, rop2, rop3);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::RasterOperation>>
            (
                (Shared::TwoD::RasterOperation_t(rop1) << Shared::TwoD::RasterOperation::Shift::RasterOperation1) |
                (Shared::TwoD::RasterOperation_t(rop2) << Shared::TwoD::RasterOperation::Shift::RasterOperation2) |
                (Shared::TwoD::RasterOperation_t(rop3) << Shared::TwoD::RasterOperation::Shift::RasterOperation3)
            );
    }

    template <int F>
    void SetPattern0Alias(Shared::TwoD::Pattern_t patternAlias)
    {
        LOG("Fifo::Direct::TwoD", "SetPattern0Alias: patternAlias: 0x%08lX", patternAlias);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Pattern0Alias>>
            (
                patternAlias
            );
    }

    template <int F>
    void SetPattern1Alias(Shared::TwoD::Pattern_t patternAlias)
    {
        LOG("Fifo::Direct::TwoD", "SetPattern1Alias: patternAlias: 0x%08lX", patternAlias);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Pattern1Alias>>
            (
                patternAlias
            );
    }

    template <int F>
    void SetPatternAliases(Shared::TwoD::Pattern_t pattern0Alias, Shared::TwoD::Pattern_t pattern1Alias)
    {
        LOG("Fifo::Direct::TwoD", "SetPatternAliases: pattern0Alias: 0x%08lX, pattern1Alias: 0x%08lX", pattern0Alias, pattern1Alias);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Pattern0Alias, Packet2::Bit::Pattern1Alias>>
            (
                pattern0Alias,
                pattern1Alias
            );
    }

    template <int F>
    void SetCommandAndExtra(Shared::TwoD::Command_t command, Shared::TwoD::Rop_t rop, Shared::TwoD::CommandExtra_t commandExtra)
    {
        LOG("Fifo::Direct::TwoD", "SetCommandAndExtra: command: 0x%08lX, rop: 0x%02X, commandExtra: 0x%08lX", command, rop, commandExtra);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::CommandExtra, Packet2::Bit::Command>>
            (
                commandExtra,
                (rop << Shared::TwoD::Command::Shift::RasterOperation0) | command
            );
    }

    template <int F>
    void SetCommandExtra(Shared::TwoD::CommandExtra_t commandExtra)
    {
        LOG("Fifo::Direct::TwoD", "SetCommandExtra: commandExtra: 0x%08lX", commandExtra);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::CommandExtra>>
            (
                commandExtra
            );
    }

    template <int F>
    void SetCommand(Shared::TwoD::Command_t command, Shared::TwoD::Rop_t rop)
    {
        LOG("Fifo::Direct::TwoD", "SetCommand: command: 0x%08lX, rop: 0x%02X", command, rop);

        Packet2::Emit<F, Packet2::Registers<Packet2::Bit::Command>>
            (
                (rop << Shared::TwoD::Command::Shift::RasterOperation0) | command
            );
    }

}
