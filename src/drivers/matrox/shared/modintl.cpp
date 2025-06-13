//Copyright 2025-Present riplin

#include <stdio.h>
#include <string.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include "modintl.h"

namespace Hag { namespace Matrox { namespace Shared { namespace Function { namespace ModeSetting
{

extern Scanlines_t GetNumberOfActiveScanlines(const ModeDescriptor& descriptor);

VideoParameters& ModeDescriptor::GetParameters() const
{
    return ((Flags & Flags::ParameterCount) == Flags::MultiParameter) ? 
        *Parameters[GetNumberOfActiveScanlines(*this)] :
        *Parameters[0];
}

uint16_t ModeDescriptor::CalculateOffset() const
{
    VideoParameters& parameters = GetParameters();

    if ((parameters.Timings.FrequencyKHz != 0) &&
        ((Flags & Flags::Mode) != Flags::Text) &&
        (Bpp >= BitsPerPixel::Bpp8))
    {
        BitsPerPixel_t bpp = Bpp;
        if (bpp == BitsPerPixel::Bpp15)
            bpp = BitsPerPixel::Bpp16;
            
        return uint16_t((parameters.HorizontalDisplayEnableEndPixels() * bpp) / 128);
    }
    else
    {
        uint8_t everySecond = parameters.Config.Sequencer[0] & VGA::Sequencer::ClockingMode::LoadSerializersEverySecondCharacterClock;
        uint8_t everyFourth = parameters.Config.Sequencer[0] & VGA::Sequencer::ClockingMode::LoadSerializersEveryFourthCaracterClock;
        uint8_t charWidth = (everyFourth != 0) ? 4 : ((everySecond != 0) ? 2 : 1);

        return parameters.HorizontalDisplayEnableEndChars() / (charWidth * 2);
    }
}

bool VideoParameters::IsExtendedMode()
{
    return Timings.FrequencyKHz != 0;
}

uint8_t VideoParameters::CharacterClockInPixels()
{
    //Dot clock select bit is in bit 0.
    uint8_t dotClockSelect = Config.Sequencer[VGA::Sequencer::Register::ClockingMode - 1] & VGA::Sequencer::ClockingMode::DotClockSelect;
    //dotClockSelect == 0 -> 9 pixels, 1 -> 8 pixels.
    return dotClockSelect == 0 ? 9 : 8;
}

uint8_t VideoParameters::ScanlineDouble()//Returns 0 if no doubling, 1 if there is.
{
    return (Config.CRTCModeControl & VGA::CRTController::CRTCModeControl::VerticalTotalDouble) >> 2;
}

uint32_t VideoParameters::HorizontalTotalChars()
{
    uint32_t horizontalTotal = Timings.Horizontal.Total;
    if (IsExtendedMode())
    {
        //Extension bit 8 is in bit 0.
        uint32_t horizontalTotalBit8 = Timings.HorizontalCounterExtensions & CRTCExtension::HorizontalCounterExtensions::HorizontalTotal8;
        //Move to bit 8.
        horizontalTotalBit8 <<= 8;
        //Move bit in to place.
        horizontalTotal |= horizontalTotalBit8;
    }
    return horizontalTotal + 5;
}

uint32_t VideoParameters::HorizontalTotalPixels()
{
    return HorizontalTotalChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::HorizontalDisplayEnableEndChars()
{
    return Timings.Horizontal.DisplayEnd + 1;
}

uint32_t VideoParameters::HorizontalDisplayEnableEndPixels()
{
    return HorizontalDisplayEnableEndChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::HorizontalBlankStartChars()
{
    uint32_t startHorizontalBlank = Timings.Horizontal.StartBlankLow;
    if (IsExtendedMode())
    {
        //Extension bit 8 is in bit 1.
        uint32_t startHorizontalBlankBit8 = Timings.HorizontalCounterExtensions & CRTCExtension::HorizontalCounterExtensions::HorizontalBlankingStart8;
        //Move to bit 8.
        startHorizontalBlankBit8 <<= 7;
        //Move bit in to place.
        startHorizontalBlank|= startHorizontalBlankBit8;
    }
    return startHorizontalBlank + 1;//+1 added by me.
}

uint32_t VideoParameters::HorizontalBlankStartPixels()
{
    return HorizontalBlankStartChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::HorizontalBlankEndChars()
{
    //5 bits.
    uint32_t endHorizontalBlank = Timings.Horizontal.EndBlank & VGA::CRTController::EndHorizontalBlank::EndHorizontalBlankLow;
    //Extension bit 5 is in bit 7.
    uint32_t endHorizontalBlankBit5 = Timings.Horizontal.EndSyncPosition & VGA::CRTController::EndHorizontalSyncPosition::EndHorizontalBlankHigh;
    //Move to bit 5.
    endHorizontalBlankBit5 >>= 2;
    //Move bit in to place.
    endHorizontalBlank |= endHorizontalBlankBit5;
    if (IsExtendedMode())
    {
        //Extension bit 6 is in bit 6.
        uint32_t endHorizontalBlankBit6 = Timings.HorizontalCounterExtensions & CRTCExtension::HorizontalCounterExtensions::HorizontalBlankingEnd6;
        //Move bit in to place.
        endHorizontalBlank |= endHorizontalBlankBit6;
        //Fetch start horizontal blank.
        uint32_t startHorizontalBlank = HorizontalBlankStartChars();
        //Recover top bits from startHorizontal Blank.
        uint32_t startHorizontalBlankTopBits = startHorizontalBlank & ~0x7F;//And off the irrelevant bits.
        //Move bits in to place.
        endHorizontalBlank |= startHorizontalBlankTopBits;
        //If start is less than end, add an extra top bit to compensate.
        if (startHorizontalBlank >= endHorizontalBlank) endHorizontalBlank += 0x80;
    }
    else
    {
        //Fetch start horizontal blank.
        uint32_t startHorizontalBlank = HorizontalBlankStartChars();
        //Recover top bits from startHorizontal Blank.
        uint32_t startHorizontalBlankTopBits = startHorizontalBlank & ~0x3F;//And off the irrelevant bits.
        //Move bits in to place.
        endHorizontalBlank |= startHorizontalBlankTopBits;
        //If start is less than end, add an extra top bit to compensate.
        if (startHorizontalBlank >= endHorizontalBlank) endHorizontalBlank += 0x40;
    }

    return endHorizontalBlank + 1; //+1 added by me.
}

uint32_t VideoParameters::HorizontalBlankEndPixels()
{
    return HorizontalBlankEndChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::HorizontalSyncStartChars()
{
    uint16_t startHorizontalSync = Timings.Horizontal.StartSyncPosition;
    if (IsExtendedMode())
    {
        //Extension bit 8 is in bit 2.
        uint32_t startHorizontalSyncBit8 = Timings.HorizontalCounterExtensions & CRTCExtension::HorizontalCounterExtensions::HorizontalRetraceStart8;
        //Move to bit 8.
        startHorizontalSyncBit8 <<= 6;
        //Move bit in to place.
        startHorizontalSync |= startHorizontalSyncBit8;
    }

    return startHorizontalSync;
}

uint32_t VideoParameters::HorizontalSyncStartPixels()
{
    return HorizontalSyncStartChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::HorizontalSyncEndChars()
{
    //5 bits.
    uint32_t endHorizontalSync = Timings.Horizontal.EndSyncPosition & VGA::CRTController::EndHorizontalSyncPosition::EndHorizontalSyncPositionLow;
    //Fetch start horizontal sync start.
    uint32_t startHorizontalSync = HorizontalSyncStartChars();
    //Recover top bits from start horizontal sync;
    uint32_t startHorizontalSyncTopBits = startHorizontalSync & 0xFFFFFFE0;
    //Move bits in to place.
    endHorizontalSync |= startHorizontalSyncTopBits;
    //If start is less than end, add an extra top bit to compensate.
    if (startHorizontalSync >= endHorizontalSync) endHorizontalSync += 0x20;
    
    return endHorizontalSync;
}

uint32_t VideoParameters::HorizontalSyncEndPixels()
{
    return HorizontalSyncEndChars() * CharacterClockInPixels();
}

uint32_t VideoParameters::VerticalDisplayEnableEndLines()
{
    uint32_t verticalDisplayEnd = Timings.Vertical.DisplayEnd;
    //Extension bit 8 is in bit 1.
    uint32_t verticalDisplayEndBit8 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalDisplayEndHigh1;
    //Move to bit 8.
    verticalDisplayEndBit8 <<= 7;
    //Move bit in to place.
    verticalDisplayEnd |= verticalDisplayEndBit8;
    //Extension bit 9 is in bit 6.
    uint32_t verticalDisplayEndBit9 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalDisplayEndHigh2;
    //Move to bit 9.
    verticalDisplayEndBit9 <<= 3;
    //Move bit in to place.
    verticalDisplayEnd |= verticalDisplayEndBit9;
    if (IsExtendedMode())
    {
        //Extension bit 10 is in bit 2.
        uint32_t verticalDisplayEndBit10 = Timings.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VeticalDisplayEnableEnd10;
        //Move to bit 10.
        verticalDisplayEndBit10 <<= 8;
        //Move bit in to place.
        verticalDisplayEnd |= verticalDisplayEndBit10;
    }
    return (verticalDisplayEnd + 1) << ScanlineDouble();
}

uint32_t VideoParameters::VerticalTotalLines()
{
    uint32_t verticalTotal = Timings.Vertical.Total;
    //Extension bit 8 is in bit 0.
    uint32_t verticalTotalBit8 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalTotalHigh1;
    //Move to bit 8.
    verticalTotalBit8 <<= 8;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit8;
    //Extension bit 9 is in bit 5.
    uint32_t verticalTotalBit9 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalTotalHigh2;
    //Move to bit 9.
    verticalTotalBit9 <<= 4;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit9;
    if (IsExtendedMode())
    {
        //Extension bits 11 and 10 are in bits 1 and 0.
        uint32_t verticalTotalBits11And10 = Timings.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VerticalTotal11_10;
        //Move to bits 11 and 10.
        verticalTotalBits11And10 <<= 10;
        //Move bits in to place.
        verticalTotal |= verticalTotalBits11And10;
    }
    return (verticalTotal + 2) << ScanlineDouble();
}

uint32_t VideoParameters::VerticalBlankStartLines()
{
    uint32_t verticalBlankStart = Timings.Vertical.StartBlank;
    //Extension bit 8 is in bit 3.
    uint32_t verticalBlankStartBit8 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::StartVerticalBlankHigh;
    //Move to bit 8.
    verticalBlankStartBit8 <<= 5;
    //Move bit in to place.
    verticalBlankStart |= verticalBlankStartBit8;
    //Extension bit 9 is in bit 5.
    uint32_t verticalBlankStartBit9 = Timings.Vertical.VBlank;
    //Move to bit 9.
    verticalBlankStartBit9 <<= 4;
    //Move bit in to place.
    verticalBlankStart |= verticalBlankStartBit9;
    if (IsExtendedMode())
    {
        //Extension bits 11 and 10 are in bits 3 and 4.
        uint32_t verticalBlankStartBits11And10 = Timings.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VerticalBlankingStart11_10;
        //Move to bit 11 and 10.
        verticalBlankStartBits11And10 <<= 7;
        //Move bits in to place.
        verticalBlankStart |= verticalBlankStartBits11And10;
    }
    return (verticalBlankStart + 1) << ScanlineDouble();
}

uint32_t VideoParameters::VerticalBlankEndLines()
{
    uint32_t verticalBlankEnd = Timings.Vertical.EndBlank;
    //Fetch vertical blank start lines.
    uint32_t verticalBlankStart = VerticalBlankStartLines();
    //Recover top bits from vertical blank start.
    uint32_t verticalBlankStartTopBits = verticalBlankStart & 0xFFFFFF00;
    //Move bits in to place.
    verticalBlankEnd |= verticalBlankStartTopBits;
    //If start is less than end, add an extra top bit to compensate.
    if (verticalBlankStart >= verticalBlankEnd) verticalBlankEnd += 0x100;
    
    //+1 add by me
    if (IsExtendedMode())
    {
        verticalBlankEnd += 1;
    }

    return verticalBlankEnd << ScanlineDouble();
}

uint32_t VideoParameters::VerticalSyncStartLines()
{
    uint32_t verticalSyncStart = Timings.Vertical.RetraceStart;
    //Extension bit 8 is in bit 2.
    uint32_t verticalSyncStartBit8 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalRetraceStartHigh1;
    //Move to bit 8.
    verticalSyncStartBit8 <<= 6;
    //Move bit in to place.
    verticalSyncStart |= verticalSyncStartBit8;
    //Extension bit 8 is in bit 7.
    uint32_t verticalSyncStartBit9 = Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalRetraceStartHigh2;
    //Move to bit 9.
    verticalSyncStartBit9 <<= 2;
    //Move bit in to place.
    verticalSyncStart |= verticalSyncStartBit9;
    if (IsExtendedMode())
    {
        //Extension bits 11 and 10 are in bits 6 and 5.
        uint32_t verticalSyncStartBits11And10 = Timings.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VerticalRetraceStart11_10;
        //Move to bits 11 and 10.
        verticalSyncStartBits11And10 <<= 5;
        //Move bits in to place.
        verticalSyncStart |= verticalSyncStartBits11And10;
    }

    return verticalSyncStart << ScanlineDouble();
}

uint32_t VideoParameters::VerticalSyncEndLines()
{
    //4 bits.
    uint32_t verticalSyncEnd = Timings.Vertical.RetraceEnd & VGA::CRTController::VerticalRetraceEnd::VerticalRetraceEndCount;
    //Fetch vertical sync start lines.
    uint32_t verticalSyncStart = VerticalSyncStartLines();
    //Recover top bits from sync start lines.
    uint32_t verticalSyncStartTopBits = verticalSyncStart & 0xFFFFFFF0;
    //Move bits in to place.
    verticalSyncEnd |= verticalSyncStartTopBits;
    //If start is less than end, add an extra top bit to compensate.
    if (verticalSyncStart >= verticalSyncEnd) verticalSyncEnd += 0x10;

    return verticalSyncEnd << ScanlineDouble();
}

bool VideoParameters::IsInterlaced()
{
    return false;//No way to even set this currently.
}

bool VideoParameters::HorizontalSyncPolarityPositive()
{
    using namespace Hag::VGA;
    return (Timings.Polarity & MiscellaneousOutput::SelectNegativeHorizontalSyncPulse) == 0;
}

bool VideoParameters::VerticalSyncPolarityPositive()
{
    using namespace Hag::VGA;
    return (Timings.Polarity & MiscellaneousOutput::SelectNegativeVerticalSyncPulse) == 0;
}

float VideoParameters::RefreshRateHz()
{
    uint32_t horizontalTotalPixels = HorizontalTotalPixels();
    uint32_t verticalTotalLines = VerticalTotalLines();

    float refreshRate = (float(Timings.FrequencyKHz) / (horizontalTotalPixels * verticalTotalLines)) * 1000.0f;

    return refreshRate;
}

void PrintTimings(VideoParameters& videoMode)
{
    uint32_t horizontalAddressPixels = videoMode.HorizontalDisplayEnableEndPixels();
    uint32_t verticalAddressLines = videoMode.VerticalDisplayEnableEndLines();
    float refreshRateHz = videoMode.RefreshRateHz();

    printf("\n\nTiming Name         = %lu x %lu @ %.fHz;\n\n",
        horizontalAddressPixels, verticalAddressLines, refreshRateHz);

    printf("Hor Pixels          = %4lu;         // Pixels\n", horizontalAddressPixels);
    printf("Ver Pixels          = %4lu;         // Pixels\n\n", verticalAddressLines);

    uint32_t horizontalTotalPixels = videoMode.HorizontalTotalPixels();
    float horizontalFrequencyKhz = float(videoMode.Timings.FrequencyKHz) / horizontalTotalPixels;
    float horizontalTotalTimeUsec = 1000.0f / horizontalFrequencyKhz;

    printf("Hor Frequency       = %.3f;       // kHz      =    %.1f usec / line\n",
        horizontalFrequencyKhz, horizontalTotalTimeUsec);

    float verticalTotalTimeMsec = 1000.0f / refreshRateHz;
    printf("Ver Frequency       = %.3f;       // Hz       =    %.1f msec / frame\n\n",
        refreshRateHz, verticalTotalTimeMsec);

    float pixelClockMHz = float(videoMode.Timings.FrequencyKHz) / 1000.0f;
    float pixelTime = 1000.0f / pixelClockMHz;

    printf("Pixel Clock         = %.3f;       // MHz      =    %.1f nsec ± 0.5%%\n",
        pixelClockMHz, pixelTime);

    uint32_t characterWidth = videoMode.CharacterClockInPixels();
    float characterTime = (characterWidth * 1000.0f) / pixelClockMHz;

    printf("Character Width     = %lu;            // Pixels   =   %.1f nsec\n",
        characterWidth, characterTime);

    printf("Scan Type           = %s;                // H Phase  =   ? %%\n",
        videoMode.IsInterlaced() ? "INTERLACED" : "NONINTERLACED");

    uint32_t horizontalBlankDurationPixels = videoMode.HorizontalBlankEndPixels() - videoMode.HorizontalBlankStartPixels();
    float hblankPercentage = 100.0f * (float(horizontalBlankDurationPixels) / horizontalTotalPixels);

    printf("Hor Sync Polarity   = %s;     // HBlank   =   %.1f%% of HTotal\n",
        videoMode.HorizontalSyncPolarityPositive() ? "POSITIVE" : "NEGATIVE", hblankPercentage);

    uint32_t verticalBlankDurationLines = videoMode.VerticalBlankEndLines() - videoMode.VerticalBlankStartLines();
    uint32_t verticalTotalLines = videoMode.VerticalTotalLines();
    float vblankPercentage = 100.0f * (float(verticalBlankDurationLines) / verticalTotalLines);

    printf("Ver Sync Polarity   = %s;     // VBlank   =   %.1f%% of VTotal\n\n",
        videoMode.VerticalSyncPolarityPositive() ? "POSITIVE" : "NEGATIVE", vblankPercentage);

    float horizontalTotalTime = float(horizontalTotalPixels) / pixelClockMHz;
    uint32_t horizontalTotalChars = videoMode.HorizontalTotalChars();

    printf("Hor Total Time      = %2.3f;       // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalTotalTime, horizontalTotalChars, horizontalTotalPixels);

    float horizontalAddressTime = float(horizontalAddressPixels) / pixelClockMHz;
    uint32_t horizontalAddressChars = videoMode.HorizontalDisplayEnableEndChars();

    printf("Hor Addr Time       = %2.3f;       // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalAddressTime, horizontalAddressChars, horizontalAddressPixels);

    uint32_t horizontalBlankStartPixels = videoMode.HorizontalBlankStartPixels();
    uint32_t horizontalBlankStartChars = videoMode.HorizontalBlankStartChars();
    float horizontalBlankStartTime = float(horizontalBlankStartPixels) / pixelClockMHz;

    printf("Hor Blank Start     = %2.3f;       // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalBlankStartTime, horizontalBlankStartChars, horizontalBlankStartPixels);

    uint32_t horizontalBlankDurationChars = videoMode.HorizontalBlankEndChars() - videoMode.HorizontalBlankStartChars();
    float horizontalBlankDurationTime = float(horizontalBlankDurationPixels) / pixelClockMHz;

    printf("Hor Blank Time      =  %1.3f;       // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalBlankDurationTime, horizontalBlankDurationChars, horizontalBlankDurationPixels);

    uint32_t horizontalSyncStartChars = videoMode.HorizontalSyncStartChars();
    uint32_t horizontalSyncStartPixels = videoMode.HorizontalSyncStartPixels();
    float horizontalSyncStartTime = float(horizontalSyncStartPixels) / pixelClockMHz;

    printf("Hor Sync Start      = %2.3f;       // (usec)   =  %4lu chars =    %4lu Pixels\n\n",
        horizontalSyncStartTime, horizontalSyncStartChars, horizontalSyncStartPixels);

    uint32_t horizontalRightBorderChars = videoMode.HorizontalBlankStartChars() - videoMode.HorizontalDisplayEnableEndChars();
    uint32_t horizontalRightBorderPixels = videoMode.HorizontalBlankStartPixels() - videoMode.HorizontalDisplayEnableEndPixels();
    float horizontalRightBorderTime = float(horizontalRightBorderPixels) / pixelClockMHz;

    printf("// H Right Border   = %1.3f;        // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalRightBorderTime, horizontalRightBorderChars, horizontalRightBorderPixels);

    uint32_t horizontalFrontPorchChars = videoMode.HorizontalSyncStartChars() - videoMode.HorizontalBlankStartChars();
    uint32_t horizontalFrontPorchPixels = videoMode.HorizontalSyncStartPixels() - videoMode.HorizontalBlankStartPixels();
    float horizontalFrontPorchTime = float(horizontalFrontPorchPixels) / pixelClockMHz;

    printf("// H Front Porch    = %1.3f;        // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalFrontPorchTime, horizontalFrontPorchChars, horizontalFrontPorchPixels);

    uint32_t horizontalSyncDurationChars = videoMode.HorizontalSyncEndChars() - videoMode.HorizontalSyncStartChars();
    uint32_t horizontalSyncDurationPixels = videoMode.HorizontalSyncEndPixels() - videoMode.HorizontalSyncStartPixels();
    float horizontalSyncDurationTime = float(horizontalSyncDurationPixels) / pixelClockMHz;

    printf("Hor Sync Time       = %1.3f;        // (usec)   =  %4lu chars =    %4lu Pixels\n",
    horizontalSyncDurationTime, horizontalSyncDurationChars, horizontalSyncDurationPixels);

    uint32_t horizontalBackPorchChars = videoMode.HorizontalBlankEndChars() - videoMode.HorizontalSyncEndChars();
    uint32_t horizontalBackPorchPixels = videoMode.HorizontalBlankEndPixels() - videoMode.HorizontalSyncEndPixels();
    float horizontalBackPorchTime = float(horizontalBackPorchPixels) / pixelClockMHz;

    printf("// H Back Porch     = %1.3f;        // (usec)   =  %4lu chars =    %4lu Pixels\n",
        horizontalBackPorchTime, horizontalBackPorchChars, horizontalBackPorchPixels);

    uint32_t horizontalLeftBorderChars = videoMode.HorizontalTotalChars() - videoMode.HorizontalBlankEndChars();
    uint32_t horizontalLeftBorderPixels = videoMode.HorizontalTotalPixels() - videoMode.HorizontalBlankEndPixels();
    float horizontalLeftBorderTime = float(horizontalLeftBorderPixels) / pixelClockMHz;

    printf("// H Left Border    = %1.3f;        // (usec)   =  %4lu chars =    %4lu Pixels\n\n",
        horizontalLeftBorderTime, horizontalLeftBorderChars, horizontalLeftBorderPixels);

    float verticalTotalTime = 1000.0f / refreshRateHz;

    printf("Ver Total Time      = %2.3f;       // (msec)   =  %4lu lines     HT - (1.06xHA)\n",
        verticalTotalTime, verticalTotalLines);

    float verticalAddressTime = (1000.0f * (float(verticalAddressLines) / verticalTotalLines)) / refreshRateHz;
    float extraCalculation = horizontalTotalTime - (1.06f * horizontalAddressTime);

    printf("Ver Addr Time       = %2.3f;       // (msec)   =  %4lu lines         = %1.2f\n",
        verticalAddressTime, verticalAddressLines, extraCalculation);

    uint32_t verticalBlankStartLines = videoMode.VerticalBlankStartLines();
    float verticalBlankStartTime = (1000.0f * (float(verticalBlankStartLines) / verticalTotalLines)) / refreshRateHz;

    printf("Ver Blank Start     = %2.3f;       // (msec)   =  %4lu lines\n",
        verticalBlankStartTime, verticalBlankStartLines);

    float verticalBlankDurationTime = (1000.0f * (float(verticalBlankDurationLines) / verticalTotalLines)) / refreshRateHz;

    printf("Ver Blank Time      =  %1.3f;       // (msec)   =  %4lu lines\n",
        verticalBlankDurationTime, verticalBlankDurationLines);

    uint32_t verticalSyncStartLines = videoMode.VerticalSyncStartLines();
    float verticalSyncStartTime = (1000.0f * (float(verticalSyncStartLines) / verticalTotalLines)) / refreshRateHz;

    printf("Ver Sync Start      = %2.3f;       // (msec)   =  %4lu lines\n\n",
        verticalSyncStartTime, verticalSyncStartLines);

    uint32_t verticalBottomBorderLines = videoMode.VerticalBlankStartLines() - videoMode.VerticalDisplayEnableEndLines();
    float verticalBottomBorderTime = (1000.0f * (float(verticalBottomBorderLines) / verticalTotalLines)) / refreshRateHz;

    printf("// V Bottom Border  = %1.3f;        // (msec)   =  %4lu lines\n",
        verticalBottomBorderTime, verticalBottomBorderLines);

    uint32_t verticalFrontPorchLines = videoMode.VerticalSyncStartLines() - videoMode.VerticalBlankStartLines();
    float verticalFrontPorchTime = (1000.0f * (float(verticalFrontPorchLines) / verticalTotalLines)) / refreshRateHz;

    printf("// V Front Porch    = %1.3f;        // (msec)   =  %4lu lines\n",
        verticalFrontPorchTime, verticalFrontPorchLines);
    
    uint32_t verticalSyncDurationLines = videoMode.VerticalSyncEndLines() - videoMode.VerticalSyncStartLines();
    float verticalSyncDurationTime = (1000.0f * (float(verticalSyncDurationLines) / verticalTotalLines)) / refreshRateHz;

    printf("Ver Sync Time       = %1.3f;        // (msec)   =  %4lu lines\n",
        verticalSyncDurationTime, verticalSyncDurationLines);

    uint32_t verticalBackPorchLines = videoMode.VerticalBlankEndLines() - videoMode.VerticalSyncEndLines();
    float verticalBackPorchTime = (1000.0f * (float(verticalBackPorchLines) / verticalTotalLines)) / refreshRateHz;

    printf("// V Back Porch     = %1.3f;        // (msec)   =  %4lu lines\n",
        verticalBackPorchTime, verticalBackPorchLines);

    uint32_t verticalTopBorderLines = videoMode.VerticalTotalLines() - videoMode.VerticalBlankEndLines();
    float verticalTopBorderTime = (1000.0f * (float(verticalTopBorderLines) / verticalTotalLines)) / refreshRateHz;

    printf("// V Top Border     = %1.3f;        // (msec)   =  %4lu lines\n\n",
        verticalTopBorderTime, verticalTopBorderLines);
}

}}}}}