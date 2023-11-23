//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/bgcolor.h>
#include <hag/drivers/s3/bgmix.h>
#include <hag/drivers/s3/bitplnrm.h>
#include <hag/drivers/s3/bitplnwm.h>
#include <hag/drivers/s3/colcomp.h>
#include <hag/drivers/s3/curxpos.h>
#include <hag/drivers/s3/curxpos2.h>
#include <hag/drivers/s3/curypos.h>
#include <hag/drivers/s3/curypos2.h>
#include <hag/drivers/s3/drawcmd.h>
#include <hag/drivers/s3/drawcmd2.h>
#include <hag/drivers/s3/dxpdsc.h>
#include <hag/drivers/s3/dypasc.h>
#include <hag/drivers/s3/fgcolor.h>
#include <hag/drivers/s3/fgmix.h>
#include <hag/drivers/s3/gfxprocs.h>
#include <hag/drivers/s3/linerrtr.h>
#include <hag/drivers/s3/linerrt2.h>
#include <hag/drivers/s3/majapcnt.h>
#include <hag/drivers/s3/majapcn2.h>
#include <hag/drivers/s3/patx.h>
#include <hag/drivers/s3/paty.h>
#include <hag/drivers/s3/pixdtrf.h>
#include <hag/drivers/s3/shrstrvt.h>
#include <hag/drivers/s3/wregdata.h>
#include <hag/drivers/s3/xcoord2.h>
#include <hag/drivers/s3/yc2asc2.h>

namespace Hag { namespace S3
{

namespace MMIO
{

    namespace CurrentYPosition
    {
        inline CurrentYPosition_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentYPosition).
                ToPointer<CurrentYPosition_t>(sizeof(CurrentYPosition_t));
        }

        inline void Write(CurrentYPosition_t value)
        {
            *FARPointer(0xA000, Register::CurrentYPosition).
                ToPointer<CurrentYPosition_t>(sizeof(CurrentYPosition_t)) = value;
        }
    }

    namespace CurrentYPosition2
    {
        inline CurrentYPosition2_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentYPosition2).
                ToPointer<CurrentYPosition2_t>(sizeof(CurrentYPosition2_t));
        }

        inline void Write(CurrentYPosition2_t value)
        {
            *FARPointer(0xA000, Register::CurrentYPosition2).
                ToPointer<CurrentYPosition2_t>(sizeof(CurrentYPosition2_t)) = value;
        }
    }

    namespace CurrentXPosition
    {
        inline CurrentXPosition_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentXPosition).
                ToPointer<CurrentXPosition_t>(sizeof(CurrentXPosition_t));
        }

        inline void Write(CurrentXPosition_t value)
        {
            *FARPointer(0xA000, Register::CurrentXPosition).
                ToPointer<CurrentXPosition_t>(sizeof(CurrentXPosition_t)) = value;
        }
    }

    namespace CurrentXPosition2
    {
        inline CurrentXPosition2_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentXPosition2).
                ToPointer<CurrentXPosition2_t>(sizeof(CurrentXPosition2_t));
        }

        inline void Write(CurrentXPosition2_t value)
        {
            *FARPointer(0xA000, Register::CurrentXPosition2).
                ToPointer<CurrentXPosition2_t>(sizeof(CurrentXPosition2_t)) = value;
        }
    }

    namespace DestinationYPositionAxialStepConstant
    {
        inline DestinationYPositionAxialStepConstant_t Read()
        {
            return *FARPointer(0xA000, Register::DestinationYPositionAxialStepConstant).
                ToPointer<DestinationYPositionAxialStepConstant_t>(sizeof(DestinationYPositionAxialStepConstant_t));
        }

        inline void Write(DestinationYPositionAxialStepConstant_t value)
        {
            *FARPointer(0xA000, Register::DestinationYPositionAxialStepConstant).
                ToPointer<DestinationYPositionAxialStepConstant_t>(sizeof(DestinationYPositionAxialStepConstant_t)) = value;
        }
    }

    namespace YCoordinate2AxialStepConstant2
    {
        inline YCoordinate2AxialStepConstant2_t Read()
        {
            return *FARPointer(0xA000, Register::YCoordinate2AxialStepConstant2).
                ToPointer<YCoordinate2AxialStepConstant2_t>(sizeof(YCoordinate2AxialStepConstant2_t));
        }

        inline void Write(YCoordinate2AxialStepConstant2_t value)
        {
            *FARPointer(0xA000, Register::YCoordinate2AxialStepConstant2).
                ToPointer<YCoordinate2AxialStepConstant2_t>(sizeof(YCoordinate2AxialStepConstant2_t)) = value;
        }
    }

    namespace DestinationXPositionDiagonalStepConstant
    {
        inline DestinationXPositionDiagonalStepConstant_t Read()
        {
            return *FARPointer(0xA000, Register::DestinationXPositionDiagonalStepConstant).
                ToPointer<DestinationXPositionDiagonalStepConstant_t>(sizeof(DestinationXPositionDiagonalStepConstant_t));
        }

        inline void Write(CurrentXPosition_t value)
        {
            *FARPointer(0xA000, Register::DestinationXPositionDiagonalStepConstant).
                ToPointer<DestinationXPositionDiagonalStepConstant_t>(sizeof(DestinationXPositionDiagonalStepConstant_t)) = value;
        }
    }

    namespace XCoordinate2
    {
        inline XCoordinate2_t Read()
        {
            return *FARPointer(0xA000, Register::XCoordinate2).
                ToPointer<XCoordinate2_t>(sizeof(XCoordinate2_t));
        }

        inline void Write(XCoordinate2_t value)
        {
            *FARPointer(0xA000, Register::XCoordinate2).
                ToPointer<XCoordinate2_t>(sizeof(XCoordinate2_t)) = value;
        }
    }

    namespace LineErrorTerm
    {
        inline LineErrorTerm_t Read()
        {
            return *FARPointer(0xA000, Register::LineErrorTerm).
                ToPointer<LineErrorTerm_t>(sizeof(LineErrorTerm_t));
        }

        inline void Write(LineErrorTerm_t value)
        {
            *FARPointer(0xA000, Register::LineErrorTerm).
                ToPointer<LineErrorTerm_t>(sizeof(LineErrorTerm_t)) = value;
        }
    }

    namespace LineErrorTerm2
    {
        inline LineErrorTerm2_t Read()
        {
            return *FARPointer(0xA000, Register::LineErrorTerm2).
                ToPointer<LineErrorTerm2_t>(sizeof(LineErrorTerm2_t));
        }

        inline void Write(LineErrorTerm2_t value)
        {
            *FARPointer(0xA000, Register::LineErrorTerm2).
                ToPointer<LineErrorTerm2_t>(sizeof(LineErrorTerm2_t)) = value;
        }
    }

    namespace MajorAxisPixelCount
    {
        inline MajorAxisPixelCount_t Read()
        {
            return *FARPointer(0xA000, Register::MajorAxisPixelCount).
                ToPointer<MajorAxisPixelCount_t>(sizeof(MajorAxisPixelCount_t));
        }

        inline void Write(MajorAxisPixelCount_t value)
        {
            *FARPointer(0xA000, Register::MajorAxisPixelCount).
                ToPointer<MajorAxisPixelCount_t>(sizeof(MajorAxisPixelCount_t)) = value;
        }
    }

    namespace MajorAxisPixelCount2
    {
        inline MajorAxisPixelCount2_t Read()
        {
            return *FARPointer(0xA000, Register::MajorAxisPixelCount2).
                ToPointer<MajorAxisPixelCount2_t>(sizeof(MajorAxisPixelCount2_t));
        }

        inline void Write(MajorAxisPixelCount2_t value)
        {
            *FARPointer(0xA000, Register::MajorAxisPixelCount2).
                ToPointer<MajorAxisPixelCount2_t>(sizeof(MajorAxisPixelCount2_t)) = value;
        }
    }

    namespace GraphicsProcessorStatus
    {
        inline GraphicsProcessorStatus_t Read()
        {
            return *FARPointer(0xA000, Register::GraphicsProcessorStatus).
                ToPointer<GraphicsProcessorStatus_t>(sizeof(GraphicsProcessorStatus_t));
        }
    }

    namespace DrawingCommand
    {
        inline void Write(DrawingCommand_t value)
        {
            *FARPointer(0xA000, Register::DrawingCommand).
                ToPointer<DrawingCommand_t>(sizeof(DrawingCommand_t)) = value;
        }
    }

    namespace DrawingCommand2
    {
        inline void Write(DrawingCommand2_t value)
        {
            *FARPointer(0xA000, Register::DrawingCommand2).
                ToPointer<DrawingCommand2_t>(sizeof(DrawingCommand2_t)) = value;
        }
    }

    namespace ShortStrokeVectorTransfer
    {
        inline ShortStrokeVectorTransfer_t Read()
        {
            return *FARPointer(0xA000, Register::ShortStrokeVectorTransfer).
                ToPointer<ShortStrokeVectorTransfer_t>(sizeof(ShortStrokeVectorTransfer_t));
        }

        inline void Write(ShortStrokeVectorTransfer_t value)
        {
            *FARPointer(0xA000, Register::ShortStrokeVectorTransfer).
                ToPointer<ShortStrokeVectorTransfer_t>(sizeof(ShortStrokeVectorTransfer_t)) = value;
        }
    }

    namespace BackgroundColor
    {
        inline BackgroundColor_t Read()
        {
            BackgroundColor_t lower = *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t));
            BackgroundColor_t upper = *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t));
            return (upper << 16) | lower;
        }

        inline void Write(BackgroundColor_t value)
        {
            *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t)) = uint16_t(value >> 16);
        }
    }

    namespace ForegroundColor
    {
        inline ForegroundColor_t Read()
        {
            ForegroundColor_t lower = *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t));
            ForegroundColor_t upper = *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t));
            return (upper << 16) | lower;
        }

        inline void Write(ForegroundColor_t value)
        {
            *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t)) = uint16_t(value >> 16);
        }
    }

    namespace BitplaneWriteMask
    {
        inline BitplaneWriteMask_t Read()
        {
            BitplaneWriteMask_t lower = *FARPointer(0xA000, Register::BitplaneWriteMask).
                ToPointer<BitplaneWriteMask_t>(sizeof(BitplaneWriteMask_t));
            BitplaneWriteMask_t upper = *FARPointer(0xA000, Register::BitplaneWriteMask).
                ToPointer<BitplaneWriteMask_t>(sizeof(BitplaneWriteMask_t));
            return (upper << 16) | lower;
        }

        inline void Write(BitplaneWriteMask_t value)
        {
            *FARPointer(0xA000, Register::BitplaneWriteMask).
                ToPointer<BitplaneWriteMask_t>(sizeof(BitplaneWriteMask_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::BitplaneWriteMask).
                ToPointer<BitplaneWriteMask_t>(sizeof(BitplaneWriteMask_t)) = uint16_t(value >> 16);
        }
    }

    namespace BitplaneReadMask
    {
        inline BitplaneReadMask_t Read()
        {
            BitplaneReadMask_t lower = *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t));
            BitplaneReadMask_t upper = *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t));
            return (upper << 16) | lower;
        }

        inline void Write(BitplaneReadMask_t value)
        {
            *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t)) = uint16_t(value >> 16);
        }
    }

    namespace ColorCompare
    {
        inline ColorCompare_t Read()
        {
            ColorCompare_t lower = *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t));
            BitplaneReadMask_t upper = *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t));
            return (upper << 16) | lower;
        }

        inline void Write(ColorCompare_t value)
        {
            *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t)) = uint16_t(value >> 16);
        }
    }

    namespace BackgroundMix
    {
        inline BackgroundMix_t Read()
        {
            return *FARPointer(0xA000, Register::BackgroundMix).
                ToPointer<BackgroundMix_t>(sizeof(BackgroundMix_t));
        }

        inline void Write(BackgroundMix_t value)
        {
            *FARPointer(0xA000, Register::BackgroundMix).
                ToPointer<BackgroundMix_t>(sizeof(BackgroundMix_t)) = value;
        }
    }

    namespace ForegroundMix
    {
        inline ForegroundMix_t Read()
        {
            return *FARPointer(0xA000, Register::ForegroundMix).
                ToPointer<ForegroundMix_t>(sizeof(ForegroundMix_t));
        }

        inline void Write(ForegroundMix_t value)
        {
            *FARPointer(0xA000, Register::ForegroundMix).
                ToPointer<ForegroundMix_t>(sizeof(ForegroundMix_t)) = value;
        }
    }

    namespace PixelDataTransfer
    {
        inline PixelDataTransfer16_t Read16()
        {
            return *FARPointer(0xA000, Register::PixelDataTransfer).
                ToPointer<PixelDataTransfer16_t>(sizeof(PixelDataTransfer16_t));
        }

        inline PixelDataTransfer32_t Read32()
        {
            return *FARPointer(0xA000, Register::PixelDataTransfer).
                ToPointer<PixelDataTransfer32_t>(sizeof(PixelDataTransfer32_t));
        }

        inline void Write16(PixelDataTransfer16_t value)
        {
            *FARPointer(0xA000, Register::PixelDataTransfer).
                ToPointer<PixelDataTransfer16_t>(sizeof(PixelDataTransfer16_t)) = value;
        }

        inline void Write32(PixelDataTransfer32_t value)
        {
            *FARPointer(0xA000, Register::PixelDataTransfer).
                ToPointer<PixelDataTransfer32_t>(sizeof(PixelDataTransfer32_t)) = value;
        }
    }

    namespace PatternY
    {
        inline PatternY_t Read()
        {
            return *FARPointer(0xA000, Register::PatternY).
                ToPointer<PatternY_t>(sizeof(PatternY_t));
        }

        inline void Write(PatternY_t value)
        {
            *FARPointer(0xA000, Register::PatternY).
                ToPointer<PatternY_t>(sizeof(PatternY_t)) = value;
        }
    }

    namespace PatternX
    {
        inline PatternX_t Read()
        {
            return *FARPointer(0xA000, Register::PatternX).
                ToPointer<PatternX_t>(sizeof(PatternX_t));
        }

        inline void Write(PatternX_t value)
        {
            *FARPointer(0xA000, Register::PatternX).
                ToPointer<PatternX_t>(sizeof(PatternX_t)) = value;
        }
    }

    namespace Packed
    {

        namespace CurrentXYPosition
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::CurrentXYPosition).ToPointer<uint32_t>();
            }

            inline void Write(CurrentXPosition_t xPosition, CurrentYPosition_t yPosition)
            {
                 Get() = (uint32_t(xPosition) << 16) | yPosition;
            }
        }

        namespace CurrentXYPosition2
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::CurrentXYPosition2).ToPointer<uint32_t>();
            }

            inline void Write(CurrentXPosition2_t xPosition2, CurrentYPosition2_t yPosition2)
            {
                 Get() = (uint32_t(xPosition2) << 16) | yPosition2;
            }
        }

        namespace DestinationXYPositionStepConstant
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::DestinationXYPositionStepConstant).ToPointer<uint32_t>();
            }

            inline void Write(DestinationXPositionDiagonalStepConstant_t destinationX, DestinationYPositionAxialStepConstant_t destinationY)
            {
                 Get() = (uint32_t(destinationX) << 16) | destinationY;
            }
        }

        namespace XYCoordinate2
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::XYCoordinate2).ToPointer<uint32_t>();
            }

            inline void Write(XCoordinate2_t coordinateX, YCoordinate2AxialStepConstant2_t coordinateY)
            {
                 Get() = (uint32_t(coordinateX) << 16) | coordinateY;
            }
        }

        namespace LinearErrorTerms
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::LinearErrorTerms).ToPointer<uint32_t>();
            }

            inline void Write(LineErrorTerm2_t errorTerm2, LineErrorTerm_t errorTerm)
            {
                 Get() = (uint32_t(errorTerm2) << 16) | errorTerm;
            }
        }

        namespace DrawingCommands
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::DrawingCommands).ToPointer<uint32_t>();
            }

            inline void Write(DrawingCommand2_t drawingCommand2, DrawingCommand_t drawingCommand)
            {
                 Get() = (uint32_t(drawingCommand2) << 16) | drawingCommand;
            }
        }

        namespace ShortStrokeVectorTransfer
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::ShortStrokeVectorTransfer).ToPointer<uint32_t>();
            }

            inline void Write(ShortStrokeVectorTransfer_t shortStrokeVector)
            {
                 Get() = shortStrokeVector;
            }
        }

        namespace BackgroundColor
        {
            inline BackgroundColor_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::BackgroundColor).ToPointer<BackgroundColor_t>();
            }

            inline void Write(BackgroundColor_t backgroundColor)
            {
                 Get() = backgroundColor;
            }
        }

        namespace ForegroundColor
        {
            inline ForegroundColor_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::ForegroundColor).ToPointer<ForegroundColor_t>();
            }

            inline void Write(ForegroundColor_t foregroundColor)
            {
                 Get() = foregroundColor;
            }
        }

        namespace BitplaneWriteMask
        {
            inline BitplaneWriteMask_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::BitplaneWriteMask).ToPointer<BitplaneWriteMask_t>();
            }

            inline void Write(BitplaneWriteMask_t writeMask)
            {
                 Get() = writeMask;
            }
        }

        namespace BitplaneReadMask
        {
            inline BitplaneReadMask_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::BitplaneReadMask).ToPointer<BitplaneReadMask_t>();
            }

            inline void Write(BitplaneReadMask_t readMask)
            {
                 Get() = readMask;
            }
        }

        namespace ColorCompare
        {
            inline ColorCompare_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::ColorCompare).ToPointer<ColorCompare_t>();
            }

            inline void Write(ColorCompare_t colorCompare)
            {
                 Get() = colorCompare;
            }
        }

        namespace Mixers
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::Mixers).ToPointer<uint32_t>();
            }

            inline void Write(ForegroundMix_t foregroundMix, BackgroundMix_t backgroundMix)
            {
                 Get() = (uint32_t(foregroundMix) << 16) | backgroundMix;
            }
        }

        namespace ScissorsTopLeft
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::ScissorsTopLeft).ToPointer<uint32_t>();
            }

            inline void Write(LeftScissors_t leftScissors, TopScissors_t topScissors)
            {
                 Get() = (uint32_t(leftScissors) << 16) | topScissors;
            }
        }

        namespace ScissorsBottomRight
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::ScissorsBottomRight).ToPointer<uint32_t>();
            }

            inline void Write(RightScissors_t rightScissors, BottomScissors_t bottomScissors)
            {
                 Get() = (uint32_t(rightScissors) << 16) | bottomScissors;
            }
        }

        namespace PixelControlAndMultiFunctionMisc2
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::PixelControlAndMultiFunctionMisc2).ToPointer<uint32_t>();
            }

            inline void Write(MultifunctionControlMiscellaneous2_t multiFunctionControlMisc2, PixelControl_t pixelControl)
            {
                 Get() = (uint32_t(multiFunctionControlMisc2) << 16) | pixelControl;
            }
        }

        namespace MultiFunctionAndReadSelect
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::MultiFunctionAndReadSelect).ToPointer<uint32_t>();
            }

            inline void Write(ReadSelect_t readSelect, MultifunctionControlMiscellaneous_t multiFunctionControlMisc)
            {
                 Get() = (uint32_t(readSelect) << 16) | multiFunctionControlMisc;
            }
        }

        namespace MinorMajorAxisPixelCounts
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::MinorMajorAxisPixelCounts).ToPointer<uint32_t>();
            }

            inline void Write(MajorAxisPixelCount_t majorAxisPixelCount, MinorAxisPixelCount_t minorAxisPixelCount)
            {
                 Get() = (uint32_t(majorAxisPixelCount) << 16) | minorAxisPixelCount;
            }
        }

        namespace MajorAxisPixelCount2
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::MajorAxisPixelCount2).ToPointer<uint32_t>();
            }

            inline void Write(MajorAxisPixelCount2_t majorAxisPixelCount2)
            {
                 Get() = majorAxisPixelCount2;
            }
        }

        namespace PixelDataTransfer
        {
            inline uint32_t* Get()
            {
                return FARPointer(0xA000, Register::Packed::PatternXY).ToPointer<uint32_t>(0x7FFF);
            }
        }

        namespace PatternXY
        {
            inline uint32_t& Get()
            {
                return *FARPointer(0xA000, Register::Packed::PatternXY).ToPointer<uint32_t>();
            }

            inline void Write(PatternX_t patternX, PatternY_t patternY)
            {
                 Get() = (uint32_t(patternX) << 16) | patternY;
            }
        }
    }

}

}}
