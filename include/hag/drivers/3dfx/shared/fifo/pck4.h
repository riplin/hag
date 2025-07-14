//Copyright 2025-Present riplin

#pragma once

#include <initializer_list>
#include <has/testing/log.h>
#include <hag/drivers/3dfx/shared/funcs/system.h>
#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

#include <hag/drivers/3dfx/shared/regs/2D/baseaddr.h>
#include <hag/drivers/3dfx/shared/regs/2D/breserr.h>
#include <hag/drivers/3dfx/shared/regs/2D/clip.h>
#include <hag/drivers/3dfx/shared/regs/2D/cmd.h>
#include <hag/drivers/3dfx/shared/regs/2D/cmdextra.h>
#include <hag/drivers/3dfx/shared/regs/2D/colkey.h>
#include <hag/drivers/3dfx/shared/regs/2D/color.h>
#include <hag/drivers/3dfx/shared/regs/2D/dstfmt.h>
#include <hag/drivers/3dfx/shared/regs/2D/lncharea.h>
#include <hag/drivers/3dfx/shared/regs/2D/lnstl.h>
#include <hag/drivers/3dfx/shared/regs/2D/lnstp.h>
#include <hag/drivers/3dfx/shared/regs/2D/pattern.h>
#include <hag/drivers/3dfx/shared/regs/2D/rop.h>
#include <hag/drivers/3dfx/shared/regs/2D/size.h>
#include <hag/drivers/3dfx/shared/regs/2D/srcfmt.h>
#include <hag/drivers/3dfx/shared/regs/2D/xy.h>

#include <hag/drivers/3dfx/shared/regs/3D/alpham.h>
#include <hag/drivers/3dfx/shared/regs/3D/buffaddr.h>
#include <hag/drivers/3dfx/shared/regs/3D/buffstrd.h>
#include <hag/drivers/3dfx/shared/regs/3D/chrkey.h>
#include <hag/drivers/3dfx/shared/regs/3D/chrrng.h>
#include <hag/drivers/3dfx/shared/regs/3D/clip.h>
#include <hag/drivers/3dfx/shared/regs/3D/cmd.h>
#include <hag/drivers/3dfx/shared/regs/3D/color.h>
#include <hag/drivers/3dfx/shared/regs/3D/colpth.h>
#include <hag/drivers/3dfx/shared/regs/3D/delta.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbiaff.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbichrf.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbipi.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbipo.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbiswph.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbitrio.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbizff.h>
#include <hag/drivers/3dfx/shared/regs/3D/fbzm.h>
#include <hag/drivers/3dfx/shared/regs/3D/fogcol.h>
#include <hag/drivers/3dfx/shared/regs/3D/fogm.h>
#include <hag/drivers/3dfx/shared/regs/3D/fogtable.h>
#include <hag/drivers/3dfx/shared/regs/3D/lfbm.h>
#include <hag/drivers/3dfx/shared/regs/3D/ncctbl.h>
#include <hag/drivers/3dfx/shared/regs/3D/ovlbuf.h>
#include <hag/drivers/3dfx/shared/regs/3D/ssetupm.h>
#include <hag/drivers/3dfx/shared/regs/3D/start.h>
#include <hag/drivers/3dfx/shared/regs/3D/stipple.h>
#include <hag/drivers/3dfx/shared/regs/3D/svertex.h>
#include <hag/drivers/3dfx/shared/regs/3D/swpbfpnd.h>
#include <hag/drivers/3dfx/shared/regs/3D/tbaseadr.h>
#include <hag/drivers/3dfx/shared/regs/3D/tdetail.h>
#include <hag/drivers/3dfx/shared/regs/3D/tlod.h>
#include <hag/drivers/3dfx/shared/regs/3D/trxinit1.h>
#include <hag/drivers/3dfx/shared/regs/3D/txtrm.h>
#include <hag/drivers/3dfx/shared/regs/3D/vertex.h>
#include <hag/drivers/3dfx/shared/regs/3D/zacol.h>

// CMDFIFO Packet Type 4
// CMDFIFO Packet Type 4 is a variable length packet, requiring a minimum of 2 32-bit words, and a
// maximum of 22 32-bit words for the complete packet. The first 3 bits 31:29 of word 0 define the number
// of pad words that follow the packet type 4 data. The next 14 bits of the header 28:15 define the register
// write mask, followed by the register base field, described later in this section. From LSB to MSB of the
// mask, a “1” enables the write and a “0” disables the write. The sequence of up to 22 32-bit data words
// following the mask modify addresses equal to the implied base address plus N where mask[N] equals “1”
// as N ranges from 0 to 16. The total number of 32-bit data words following the mask is equal to the
// number of “1”s in the mask. As a requirement, the general register mask must have a non zero value.

// CMDFIFO Packet Type 4
//          +--------+-------------------------+----------------------------+-----+
//          | 31  29 | 28                   15 | 14                       3 | 2 0 |
//          +--------+-------------------------+----------------------------+-----+
// word 0   | Number | General Register Mask   | Register Base (See below)  | 100 |
//          +----------------------------------+----------------------------+-----+
// word 1   |                              Data                                   |
//          +---------------------------------------------------------------------+
// word N   |                         Optional Data N                             |
//          +---------------------------------------------------------------------+

// Register base:
//                  Banshee
// +--------------------------------------------+
// |   14  |    13    | 12 11 | 10            3 |
// +--------------------------------------------+
// | 2D/3D | Reserved | Chip  | Register Number |
// +--------------------------------------------+
// If bit 11 is set, then the register base is a 2D register, otherwise it is a 3D register.


//                  CVG
// +--------------------------------------------+
// |  14                   11 | 10            3 |
// +--------------------------------------------+
// | Chip field               | Register Number |
// +--------------------------------------------+


namespace Hag::TDfx::Shared::Fifo::Packet4
{

    enum
    {
        PacketType =            0x00000004,
        RegisterBase =          0x00007ff8,
        RegisterType =          0x00004000,
        Type2D =                0x00004000,
        Type3D =                0x00000000,
        Chip =                  0x00001800,
        Register =              0x000007f8,
        GeneralRegisterMask =   0x1fff8000,
        Padding =               0xe0000000
    };

    namespace Shift
    {
        enum
        {
            PacketType = 0x00,
            RegisterBase = 0x03,
            RegisterType = 0x0e,
            Chip = 0x0b,
            GeneralRegisterMask = 0xf,
            Padding = 0x1d
        };
    }

    namespace TwoD::Index
    {
        enum
        {
            Clip0Min = (int(Shared::TwoD::Register::Clip0Min) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Clip0Max = (int(Shared::TwoD::Register::Clip0Max) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationBaseAddress = (int(Shared::TwoD::Register::DestinationBaseAddress) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationFormat = (int(Shared::TwoD::Register::DestinationFormat) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceColorKeyMin = (int(Shared::TwoD::Register::SourceColorKeyMin) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceColorKeyMax = (int(Shared::TwoD::Register::SourceColorKeyMax) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationColorKeyMin = (int(Shared::TwoD::Register::DestinationColorKeyMin) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationColorKeyMax = (int(Shared::TwoD::Register::DestinationColorKeyMax) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            BresenhamError0 = (int(Shared::TwoD::Register::BresenhamError0) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            BresenhamError1 = (int(Shared::TwoD::Register::BresenhamError1) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            RasterOperation = (int(Shared::TwoD::Register::RasterOperation) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceBaseAddress = (int(Shared::TwoD::Register::SourceBaseAddress) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            CommandExtra = (int(Shared::TwoD::Register::CommandExtra) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            LineStipple = (int(Shared::TwoD::Register::LineStipple) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            LineStyle = (int(Shared::TwoD::Register::LineStyle) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Pattern0Alias = (int(Shared::TwoD::Register::Pattern0Alias) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Pattern1Alias = (int(Shared::TwoD::Register::Pattern1Alias) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Clip1Min = (int(Shared::TwoD::Register::Clip1Min) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Clip1Max = (int(Shared::TwoD::Register::Clip1Max) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceFormat = (int(Shared::TwoD::Register::SourceFormat) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceSize = (int(Shared::TwoD::Register::SourceSize) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            SourceXY = (int(Shared::TwoD::Register::SourceXY) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            BackgroundColor = (int(Shared::TwoD::Register::BackgroundColor) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            ForegroundColor = (int(Shared::TwoD::Register::ForegroundColor) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationSize = (int(Shared::TwoD::Register::DestinationSize) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            DestinationXY = (int(Shared::TwoD::Register::DestinationXY) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
            Command = (int(Shared::TwoD::Register::Command) - int(Shared::TwoD::Register::Clip0Min)) >> 2,
        };
    }

    namespace ThreeD::Index
    {
        enum
        {
            Ax = (int(Shared::ThreeD::Register::Ax) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Ay = (int(Shared::ThreeD::Register::Ay) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Bx = (int(Shared::ThreeD::Register::Bx) - int(Shared::ThreeD::Register::Ax)) >> 2,
            By = (int(Shared::ThreeD::Register::By) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Cx = (int(Shared::ThreeD::Register::Cx) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Cy = (int(Shared::ThreeD::Register::Cy) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartR = (int(Shared::ThreeD::Register::StartR) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartG = (int(Shared::ThreeD::Register::StartG) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartB = (int(Shared::ThreeD::Register::StartB) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartZ = (int(Shared::ThreeD::Register::StartZ) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartA = (int(Shared::ThreeD::Register::StartA) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartS = (int(Shared::ThreeD::Register::StartS) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartT = (int(Shared::ThreeD::Register::StartT) - int(Shared::ThreeD::Register::Ax)) >> 2,
            StartW = (int(Shared::ThreeD::Register::StartW) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaRDeltaX = (int(Shared::ThreeD::Register::DeltaRDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaGDeltaX = (int(Shared::ThreeD::Register::DeltaGDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaBDeltaX = (int(Shared::ThreeD::Register::DeltaBDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaZDeltaX = (int(Shared::ThreeD::Register::DeltaZDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaADeltaX = (int(Shared::ThreeD::Register::DeltaADeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaSDeltaX = (int(Shared::ThreeD::Register::DeltaSDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaTDeltaX = (int(Shared::ThreeD::Register::DeltaTDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaWDeltaX = (int(Shared::ThreeD::Register::DeltaWDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaRDeltaY = (int(Shared::ThreeD::Register::DeltaRDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaGDeltaY = (int(Shared::ThreeD::Register::DeltaGDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaBDeltaY = (int(Shared::ThreeD::Register::DeltaBDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaZDeltaY = (int(Shared::ThreeD::Register::DeltaZDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaADeltaY = (int(Shared::ThreeD::Register::DeltaADeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaSDeltaY = (int(Shared::ThreeD::Register::DeltaSDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaTDeltaY = (int(Shared::ThreeD::Register::DeltaTDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DeltaWDeltaY = (int(Shared::ThreeD::Register::DeltaWDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TriangleCommand = (int(Shared::ThreeD::Register::TriangleCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatAx = (int(Shared::ThreeD::Register::FloatAx) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatAy = (int(Shared::ThreeD::Register::FloatAy) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatBx = (int(Shared::ThreeD::Register::FloatBx) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatBy = (int(Shared::ThreeD::Register::FloatBy) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatCx = (int(Shared::ThreeD::Register::FloatCx) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatCy = (int(Shared::ThreeD::Register::FloatCy) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartR = (int(Shared::ThreeD::Register::FloatStartR) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartG = (int(Shared::ThreeD::Register::FloatStartG) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartB = (int(Shared::ThreeD::Register::FloatStartB) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartZ = (int(Shared::ThreeD::Register::FloatStartZ) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartA = (int(Shared::ThreeD::Register::FloatStartA) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartS = (int(Shared::ThreeD::Register::FloatStartS) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartT = (int(Shared::ThreeD::Register::FloatStartT) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatStartW = (int(Shared::ThreeD::Register::FloatStartW) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaRDeltaX = (int(Shared::ThreeD::Register::FloatDeltaRDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaGDeltaX = (int(Shared::ThreeD::Register::FloatDeltaGDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaBDeltaX = (int(Shared::ThreeD::Register::FloatDeltaBDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaZDeltaX = (int(Shared::ThreeD::Register::FloatDeltaZDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaADeltaX = (int(Shared::ThreeD::Register::FloatDeltaADeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaSDeltaX = (int(Shared::ThreeD::Register::FloatDeltaSDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaTDeltaX = (int(Shared::ThreeD::Register::FloatDeltaTDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaWDeltaX = (int(Shared::ThreeD::Register::FloatDeltaWDeltaX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaRDeltaY = (int(Shared::ThreeD::Register::FloatDeltaRDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaGDeltaY = (int(Shared::ThreeD::Register::FloatDeltaGDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaBDeltaY = (int(Shared::ThreeD::Register::FloatDeltaBDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaZDeltaY = (int(Shared::ThreeD::Register::FloatDeltaZDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaADeltaY = (int(Shared::ThreeD::Register::FloatDeltaADeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaSDeltaY = (int(Shared::ThreeD::Register::FloatDeltaSDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaTDeltaY = (int(Shared::ThreeD::Register::FloatDeltaTDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatDeltaWDeltaY = (int(Shared::ThreeD::Register::FloatDeltaWDeltaY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FloatTriangleCommand = (int(Shared::ThreeD::Register::FloatTriangleCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FbzColorPath = (int(Shared::ThreeD::Register::FbzColorPath) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FogMode = (int(Shared::ThreeD::Register::FogMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            AlphaMode = (int(Shared::ThreeD::Register::AlphaMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            RGBDepthMode = (int(Shared::ThreeD::Register::RGBDepthMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            LinearFrameBufferMode = (int(Shared::ThreeD::Register::LinearFrameBufferMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ClipLeftRight = (int(Shared::ThreeD::Register::ClipLeftRight) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ClipTopBottom = (int(Shared::ThreeD::Register::ClipTopBottom) - int(Shared::ThreeD::Register::Ax)) >> 2,
            NopCommand = (int(Shared::ThreeD::Register::NopCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FastFillCommand = (int(Shared::ThreeD::Register::FastFillCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            SwapBufferCommand = (int(Shared::ThreeD::Register::SwapBufferCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FogColor = (int(Shared::ThreeD::Register::FogColor) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DepthAlphaConstant = (int(Shared::ThreeD::Register::DepthAlphaConstant) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ChromaKey = (int(Shared::ThreeD::Register::ChromaKey) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ChromaRange = (int(Shared::ThreeD::Register::ChromaRange) - int(Shared::ThreeD::Register::Ax)) >> 2,
            UserInterruptCommand = (int(Shared::ThreeD::Register::UserInterruptCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Stipple = (int(Shared::ThreeD::Register::Stipple) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Color0 = (int(Shared::ThreeD::Register::Color0) - int(Shared::ThreeD::Register::Ax)) >> 2,
            Color1 = (int(Shared::ThreeD::Register::Color1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            FogTable = (int(Shared::ThreeD::Register::FogTable) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ColorBufferAddress = (int(Shared::ThreeD::Register::ColorBufferAddress) - int(Shared::ThreeD::Register::Ax)) >> 2,
            AuxBufferAddress = (int(Shared::ThreeD::Register::AuxBufferAddress) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ColorBufferStride = (int(Shared::ThreeD::Register::ColorBufferStride) - int(Shared::ThreeD::Register::Ax)) >> 2,
            AuxBufferStride = (int(Shared::ThreeD::Register::AuxBufferStride) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ClipLeftRight1 = (int(Shared::ThreeD::Register::ClipLeftRight1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            ClipTopBottom1 = (int(Shared::ThreeD::Register::ClipTopBottom1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            SwapBufferPending = (int(Shared::ThreeD::Register::SwapBufferPending) - int(Shared::ThreeD::Register::Ax)) >> 2,
            LeftOverlayBufferAddress = (int(Shared::ThreeD::Register::LeftOverlayBufferAddress) - int(Shared::ThreeD::Register::Ax)) >> 2,
            RightOverlayBufferAddress = (int(Shared::ThreeD::Register::RightOverlayBufferAddress) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sSetupMode = (int(Shared::ThreeD::Register::sSetupMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexX = (int(Shared::ThreeD::Register::sVertexX) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexY = (int(Shared::ThreeD::Register::sVertexY) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexARGB = (int(Shared::ThreeD::Register::sVertexARGB) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexRed = (int(Shared::ThreeD::Register::sVertexRed) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexGreen = (int(Shared::ThreeD::Register::sVertexGreen) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexBlue = (int(Shared::ThreeD::Register::sVertexBlue) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexAlpha = (int(Shared::ThreeD::Register::sVertexAlpha) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexZ = (int(Shared::ThreeD::Register::sVertexZ) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexW = (int(Shared::ThreeD::Register::sVertexW) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexWTmu0 = (int(Shared::ThreeD::Register::sVertexWTmu0) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexSdW0 = (int(Shared::ThreeD::Register::sVertexSdW0) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexTdW0 = (int(Shared::ThreeD::Register::sVertexTdW0) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexWTmu1 = (int(Shared::ThreeD::Register::sVertexWTmu1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexSdWTmu1 = (int(Shared::ThreeD::Register::sVertexSdWTmu1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            sVertexTdWTmu1 = (int(Shared::ThreeD::Register::sVertexTdWTmu1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            DrawTriangleCommand = (int(Shared::ThreeD::Register::DrawTriangleCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            BeginTriangleCommand = (int(Shared::ThreeD::Register::BeginTriangleCommand) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureMode = (int(Shared::ThreeD::Register::TextureMode) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureLevelOfDetail = (int(Shared::ThreeD::Register::TextureLevelOfDetail) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureDetail = (int(Shared::ThreeD::Register::TextureDetail) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureBaseAddress = (int(Shared::ThreeD::Register::TextureBaseAddress) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureBaseAddress1 = (int(Shared::ThreeD::Register::TextureBaseAddress1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureBaseAddress2 = (int(Shared::ThreeD::Register::TextureBaseAddress2) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TextureBaseAddress3_8 = (int(Shared::ThreeD::Register::TextureBaseAddress3_8) - int(Shared::ThreeD::Register::Ax)) >> 2,
            TrexInit1 = (int(Shared::ThreeD::Register::TrexInit1) - int(Shared::ThreeD::Register::Ax)) >> 2,
            NccTable0 = (int(Shared::ThreeD::Register::NccTable0) - int(Shared::ThreeD::Register::Ax)) >> 2,
            NccTable1 = (int(Shared::ThreeD::Register::NccTable1) - int(Shared::ThreeD::Register::Ax)) >> 2,
        };
    }

    inline Command_t Word0_2D(uint16_t regBase, uint16_t regMask, uint8_t padding)
    {
        return PacketType |
               Type2D |
               (Command_t(regBase) << Shift::RegisterBase) |
               (Command_t(regMask) << Shift::GeneralRegisterMask) |
               (Command_t(padding) << Shift::Padding);
    }
    
    inline Command_t Word0_3D(uint16_t regBase, uint16_t regMask, uint8_t padding)
    {
        return PacketType |
               Type3D |
               (Command_t(regBase) << Shift::RegisterBase) |
               (Command_t(regMask) << Shift::GeneralRegisterMask) |
               (Command_t(padding) << Shift::Padding);
    }

    template <uint16_t BaseRegister, auto... Regs>
    struct Registers
    {
        static consteval unsigned int base() { return BaseRegister; }
        static consteval std::size_t length() { return sizeof...(Regs); }

        static consteval int numbits(int val)
        {
            int ret = 0;
            while (val != 0)
            {
                ret += val & 1;
                val >>= 1;
            }
            return ret;
        }

        static consteval bool valid()
        {
            bool v = true;
            for (auto reg : {Regs...})
            {
                v &= (reg >= BaseRegister) && (reg < (BaseRegister + numbits(GeneralRegisterMask)));
            }
            return v;
        }

        static consteval Command_t bits()
        {
            Command_t ret = 0;
            for (auto reg : {Regs...})
            {
                ret |= 1 << (reg - BaseRegister);
            }
            return ret;
        }
    };

    template <int F, Command_t CoreTarget, typename Regs, typename... Vals>
    forceinline void Emit(uint8_t padding, Vals... vals)
    {
        static_assert(Regs::length() == sizeof...(Vals), "Number of registers must match number of arguments");
        static_assert(sizeof...(Vals) > 0, "Must write at least one register");
        static_assert(Regs::valid(), "Registers are out of bound");

        Command_t volatile* ptr = nullptr;

        if constexpr (F == 0)
            ptr = Function::System::Allocate0(1 + Regs::length());
        else
            ptr = Function::System::Allocate1(1 + Regs::length());

        int index = 0;
        ptr[index] = (Regs::bits() << Shift::GeneralRegisterMask) |
                     (Regs::base() << Shift::RegisterBase) |
                     (Command_t(padding) << Shift::Padding) |
                     CoreTarget |
                     PacketType;
        LOG("Fifo", "Packet4: header: 0x%08lX", ptr[index]);

        ++index;
        for (auto val : {vals...})
        {
            LOG("Fifo", "Packet4: register: 0x%08lX", val);

            ptr[index] = val;
            ++index;
        }

        if constexpr (F == 0)
            Function::System::CommitFifoEntries0();
        else
            Function::System::CommitFifoEntries1();
    }

    template <int F, typename Regs, typename... Vals>
    forceinline void Emit2D(uint8_t padding, Vals... vals)
    {
        Emit<F, Type2D, Regs, Vals...>(padding, vals...);
    }

    template <int F, typename Regs, typename... Vals>
    forceinline void Emit3D(uint8_t padding, Vals... vals)
    {
        Emit<F, Type3D, Regs, Vals...>(padding, vals...);
    }

}