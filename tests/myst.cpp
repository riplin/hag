//Copyright 2025-Present riplin

#include <stdio.h>
#include <conio.h>

#include <hag/color.h>
#include <hag/system/bda.h>
#include <hag/math/fp/FPMATH.H>
#include <hag/drivers/matrox/mystique/mystique.h>

extern Hag::Math::v4 icoVecs[12];
extern int32_t icoTri[20][3];
extern Hag::Math::v4 icoNorm[20];

void SetStartY(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t startY);
void SetPitch(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t pitch);
void SetClipping(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t left, int16_t top, int16_t right, int16_t bottom);
void GlobalInit(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t width, uint32_t height);
void DrawLine(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t fgcolor);
void DrawRectangle(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t fgcolor);
void DrawTrapezoid(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t xLt, int16_t xLb, int16_t xRt, int16_t xRb, int16_t yT, int16_t yB, uint32_t fgcolor);
void DrawTriangle(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, Hag::Math::v4* vectors, uint32_t fgcolor);
void FlipFrame(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint16_t width, uint16_t height);
bool isBackFace(Hag::Math::v4& vec0, Hag::Math::v4& vec1, Hag::Math::v4& vec2);
void setupNormals();
void colorLerp(uint32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB);

uint32_t colors[256];

int main(void)
{
    using namespace Hag;
    using namespace Hag::Math;
    using namespace Hag::Color;
    using namespace Hag::Matrox;

    if (!Mystique::Initialize())
    {
        printf("Could not initialize Matrox Mystique.\n");
        return 1;
    }
    colorLerp(colors, 192, Bpp32::Black, Bpp32::RoyalPurple);
    colorLerp(colors + 192, 63, Bpp32::RoyalPurple, Bpp32::Cream);
    colors[255] = Bpp32::Cream;
    uint16_t width = 640;
    uint16_t height = 480;

    Mystique::SetVideoError_t result = Mystique::SetVideoMode(width, height, Mystique::BitsPerPixel::Bpp32, Mystique::Flags::LinearFramebuffer);

    if (result != Mystique::SetVideoError::Success)
    {
        Mystique::Shutdown();
        printf("An error occurred.\n");
        return result;
    }

    //TODO:: figure out another way to feed the device to the functions that need it.
    System::PCI::Device_t device;
    if (!System::PCI::FindDevice(0x102B, 0x051A, device))
    {
        printf("Unable to find PCI device.\n");
        Mystique::Shutdown();
        return 1;
    }

    uint32_t* frameBuffer = Mystique::PCI::FrameBufferAperture::GetAddress<uint32_t>(device);
    Mystique::PCI::ControlAperture_t controlAperture = Mystique::PCI::ControlAperture::GetAddress(device);

    GlobalInit(controlAperture, width, height);
    //Setup drawing for next frame.
    SetStartY(controlAperture, height);
    SetClipping(controlAperture, 0, 0, width - 1, height);//The clip window does not migrate after a back buffer change.

    int16_t hwidth = width >> 1;
    int16_t hheight = height >> 1;

    v3 val(0);
    v3 inc(fp::Pi / 100, fp::Pi / 75, fp::Pi / 55);

    v4 icor[12];
    v4 icon[20];
    setupNormals();

    m44 scale = m44::Scale(v3(3, 3, 3));
    m44 proj = m44::Projection(width, height, fp(90).ToRad(), fp::One, fp(250));
    fp scrX = width >> 1;
    fp scrY = height >> 1;

    v4 light = v4(v3(1, 1, 1).Normalize(), 0);
    
    v4 tri0(fp(0), fp(-100), 0, 1);
    v4 tri1(fp(100), fp(100), 0, 1);
    v4 tri2(fp(-100), fp(100), 0, 1);

    bool evenOdd = false;
    while (!kbhit())
    {
        DrawRectangle(controlAperture, 0, 0, width, height, Bpp32::Black);
        
        DrawLine(controlAperture, 0, 0, width - 1, 0, Bpp32::BabyBlue);
        DrawLine(controlAperture, width - 1, 0, width - 1, height - 1, Bpp32::BabyBlue);
        DrawLine(controlAperture, width - 1, height - 1, 0, height - 1, Bpp32::BabyBlue);
        DrawLine(controlAperture, 0, height - 1, 0, 0, Bpp32::BabyBlue);

        m44 rot = m44::RotateZ(val.z()) *
                    m44::RotateY(val.y()) * m44::RotateX(val.x());

        m44 icoTrans = proj * m44::Translate(v3(0, 0, 90)) * rot * scale;

        for (int32_t ij = 0; ij < 12; ++ij)
        {
            v4 t = icoTrans * icoVecs[ij];
            fp invZ = fp::One / t.z();
            icor[ij] = v4(t.x() * invZ + scrX, t.y() * invZ + scrY, t.z(), t.w());
        }

        for (int32_t ij = 0; ij < 20; ++ij)
        {
            icon[ij] = rot * icoNorm[ij];
        }

        v4 tri[3];

        for (int32_t i1 = 0; i1 < 20; ++i1)
        {
            tri[0] = icor[icoTri[i1][0]];
            tri[2] = icor[icoTri[i1][1]];
            tri[1] = icor[icoTri[i1][2]];
            if(isBackFace(tri[0], tri[2], tri[1]))
                continue;

            int16_t colIdx = max<int16_t>(min<int16_t>(((icon[i1].Dot(light) + 1) * 128).RawFloor(), 255), 0);

            uint32_t color = colors[colIdx];
            DrawTriangle(controlAperture, tri, color);
        }

        val = (val + inc) % fp::TwoPi;
        FlipFrame(controlAperture, width, height);
        evenOdd = !evenOdd;
    }

    Mystique::SetVideoMode(80, 25, Mystique::BitsPerPixel::Bpp4, Mystique::Flags::Text);
    Mystique::Shutdown();

    return 0;
}

void SetStartAddress(uint32_t address)
{
    using namespace Hag;
    using namespace Hag::Matrox;

    uint32_t adjustedStartAddress = address >> 1;//TODO fix for other bitdepths!

    VGA::CRTController::StartAddressLow::Write(VGA::Register::CRTControllerIndexD, uint8_t(adjustedStartAddress));
    VGA::CRTController::StartAddressHigh::Write(VGA::Register::CRTControllerIndexD, uint8_t(adjustedStartAddress >> 8));
    Shared::CRTCExtension::AddressGeneratorExtensions::Write(
        (Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
        ~Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16) |
        (uint8_t(adjustedStartAddress >> 16) & Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16));
}

void WaitVerticalSync()
{
    using namespace Hag;
    using namespace Hag::System;

    while ((VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06) & VGA::InputStatus1::VerticalSync) == VGA::InputStatus1::VerticalSyncActive);
    while ((VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06) & VGA::InputStatus1::VerticalSync) == VGA::InputStatus1::VerticalSyncInactive);
}

//TODO: This needs to be stored in a device instance.
uint32_t s_Pitch = 0;
uint32_t s_YOffset = 0;

void SetStartY(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t startY)
{
    using namespace Hag::Matrox::Shared;
    s_YOffset = startY * s_Pitch;

    MMIO::MemoryOrigin::Write(controlAperture, s_YOffset);
}

void SetPitch(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t pitch)
{
    using namespace Hag::Matrox::Shared;
    s_Pitch = pitch;
    MMIO::MemoryPitch::Write(controlAperture, MemoryPitch::YNoLinearization | s_Pitch);
}

void SetClipping(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t left, int16_t top, int16_t right, int16_t bottom)
{
    using namespace Hag::Matrox::Shared;
    MMIO::Clipper::WriteYTop(controlAperture, top * s_Pitch + s_YOffset);
    MMIO::Clipper::WriteYBottom(controlAperture, bottom * s_Pitch + s_YOffset);
    MMIO::Clipper::WriteXLeft(controlAperture, left);
    MMIO::Clipper::WriteXRight(controlAperture, right);
}

void GlobalInit(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint32_t width, uint32_t height)
{
    using namespace Hag::Matrox::Shared;

    //5.5.3 Global Initialization
    MMIO::MemoryAccess::Write(controlAperture, MemoryAccess::Width32bpp | MemoryAccess::DitheringDisabled);

    SetPitch(controlAperture, width);
    SetStartY(controlAperture, 0);
    SetClipping(controlAperture, 0, 0, width - 1, height);

    MMIO::PlaneWriteMask::Write(controlAperture, 0xFFFFFFFF);

    uint32_t zbuffer = Function::System::GetMemorySize() - (s_Pitch * height * 2);
    MMIO::ZDepthOrigin::Write(controlAperture, zbuffer);
}

// AR0      2b (1)
// AR1      Error term: 2b - a - sdy
// AR2      Minor axis increment: 2b - 2a
// SGN      Vector quadrant (2)
// XDST     The x start position
// YDSTLEN  The y start position and vector length      Can use YDST and LEN instead; must use
//                                                      YDST and LEN when destination address is
//                                                      linear (i.e.. ylin = 1, see PITCH)
// (1) Definitions: a = max (|dY|, |dX|), b = min (|dY|, |dX|).
// (2) Sets major or minor axis and positive or negative direction for x and y.

void DrawLine(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t fgcolor)
{
    using namespace Hag;
    using namespace Hag::Matrox::Shared;

    int32_t dX = x1 - x0;
    int32_t dY = y1 - y0;
    int32_t absdX = uint32_t(abs(dX));
    int32_t absdY = uint32_t(abs(dY));
    int32_t a = max(absdY, absdX);
    
    if (a != 0)
    {
        int32_t b = min(absdY, absdX);
        uint32_t sdX = (uint32_t(dX) >> 31) & 1;
        uint32_t sdY = (uint32_t(dY) >> 31) & 1;
        uint32_t sdYdX = (uint32_t((absdY - absdX)) >> 31) & 1;

        uint32_t AR0 = uint32_t(b << 1);
        uint32_t AR2 = uint32_t((b - a) << 1);
        uint32_t AR1 = uint32_t(((b - (a - sdY)) << 1));
        Sign_t sign = (sdYdX << Sign::Shift::SignDeltaYMinusDeltaX) |
                    (sdX << Sign::Shift::SignOfDeltaX) |
                    (sdY << Sign::Shift::SignOfDeltaY);

        XDestination_t xdst = x0;
        YDestination_t ydst = y0 * (s_Pitch >> 5);
        Length_t len = a;
        DrawingControl_t dwgctl = DrawingControl::LineOpen |
                                  DrawingControl::AccessReplace |
                                  DrawingControl::ZAlways |
                                  DrawingControl::Solid |
                                  DrawingControl::ShiftZero |
                                  DrawingControl::BoolSrc |
                                  DrawingControl::BlitFormattedColor;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
        MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
        MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
        MMIO::Sign::Write(controlAperture, sign);
        MMIO::XYAddress::WriteXDestination(controlAperture, xdst);
        MMIO::XYAddress::WriteYDestination(controlAperture, ydst);
        MMIO::Length::Write(controlAperture, len);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
    }
}

void DrawRectangle(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t fgcolor)
{
    using namespace Hag;
    using namespace Hag::Matrox::Shared;

    int32_t dX = x1 - x0;
    int32_t dY = y1 - y0;
    if ((dX > 1) && (dY > 1))
    {
        uint32_t left = min<uint32_t>(x0, x1);
        uint32_t right = max<uint32_t>(x0, x1);
        uint32_t top = min<uint32_t>(y0, y1);
        uint32_t bottom = max<uint32_t>(y0, y1);

        DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                  DrawingControl::AccessReplace |
                                  DrawingControl::ZAlways |
                                  DrawingControl::Solid |
                                  DrawingControl::ARZero |
                                  DrawingControl::SignZero |
                                  DrawingControl::ShiftZero |
                                  DrawingControl::BoolSrc;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::XYAddress::WriteLeft(controlAperture, left);
        MMIO::XYAddress::WriteRight(controlAperture, right);
        MMIO::XYAddress::WriteYDestination(controlAperture, top * (s_Pitch >> 5));
        MMIO::Length::Write(controlAperture, bottom - top);
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
    }
}

// AR0      Left edge major axis increment: dYl         yl_end - yl_start
// AR1      Left edge error term: errl                  (sdxl == XL_NEG) ? dXl + dYl - 1 : - dXl
// AR2      Left edge minor axis increment: -|dXl|      -|xl_end - xl_start|
// AR4      Right edge error term: errr                 (sdxr == XR_NEG) ? dXr + dYr - 1 : - dXr
// AR5      Right edge minor axis increment: -|dXr|     -|xr_end - xr_start|
// AR6      Right edge major axis increment: dYr        yr_end - yr_start
// SGN      Vector quadrant
// FXBNDRY  Filled object x left and right coordinates Can use FXRIGHT and FXLEFT
// YDSTLEN  The y start position and number of lines Can use YDST and LEN instead;
//          must use YDST and LEN when destination address is linear
//          (i.e.. ylin = 1, see PITCH)

void DrawTrapezoid(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, int16_t xLt, int16_t xLb, int16_t xRt, int16_t xRb, int16_t yT, int16_t yB, uint32_t fgcolor)
{
    using namespace Hag;
    using namespace Hag::Matrox::Shared;

    int32_t dY = yB - yT;
    if ((dY > 1) && (((xRt - xLt) > 1) || ((xRb - xLb) > 1)))
    {
        int32_t dXL = xLb - xLt;
        int32_t dXR = xRb - xRt;
        int32_t absdXL = abs(dXL);
        int32_t absdXR = abs(dXR);
        uint32_t sdXL = (uint32_t(dXL) >> 31) & 1;
        uint32_t sdXR = (uint32_t(dXR) >> 31) & 1;

        uint32_t AR0 = dY;
        uint32_t AR1 = (sdXL != 0) ? (dXL + dY) - 1 : -dXL;
        uint32_t AR2 = -absdXL;
        uint32_t AR4 = (sdXR != 0) ? (dXR + dY) - 1 : -dXR;
        uint32_t AR5 = -absdXR;
        uint32_t AR6 = dY;
        uint32_t sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                        (sdXR << Sign::Shift::SignOfDeltaXRight);
        YDestination_t ydst = yT * (s_Pitch >> 5);
        DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                    DrawingControl::AccessReplace |
                                    DrawingControl::ZAlways |
                                    DrawingControl::Solid |
                                    DrawingControl::ShiftZero |
                                    DrawingControl::BoolSrc;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::Sign::Write(controlAperture, sign);
        MMIO::XYAddress::WriteLeft(controlAperture, xLt);
        MMIO::XYAddress::WriteRight(controlAperture, xRt);
        MMIO::XYAddress::WriteYDestination(controlAperture, ydst);
        MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
        MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
        MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
        MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
        MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
        MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
        MMIO::Length::Write(controlAperture, dY);
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
    }
}

//Takes an array of 3 vectors. Assumes clockwise ordering.
void DrawTriangle(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, Hag::Math::v4* vectors, uint32_t fgcolor)
{
    using namespace Hag;
    using namespace Hag::Matrox::Shared;

    uint16_t vec0 = 0;
    uint16_t vec1 = 1;
    uint16_t vec2 = 2;

    if (vectors[0].y() > vectors[1].y())
    {
        vec0 = 1;
        vec1 = 2;
        vec2 = 0;
    }

    if (vectors[vec0].y() > vectors[2].y())
    {
        vec0 = 2;
        vec1 = 0;
        vec2 = 1;
    }

    /*
    We now have either:

              0      0
            / |      | \    top trapezoid
          /   |  or: |   \
         2----|      |----1
          \   |      |   /  bottom trapezoid
            \ 1      2 /
    */

    if ((vectors[vec0].y().Floor() != vectors[vec1].y().Floor()) && (vectors[vec0].y().Floor() != vectors[vec2].y().Floor())) // Flat top check
    {
        int32_t dXL = vectors[vec2].x().RawFloor() - vectors[vec0].x().RawFloor();
        int32_t dYL = vectors[vec2].y().RawFloor() - vectors[vec0].y().RawFloor();
        int32_t dXR = vectors[vec1].x().RawFloor() - vectors[vec0].x().RawFloor();
        int32_t dYR = vectors[vec1].y().RawFloor() - vectors[vec0].y().RawFloor();
        int32_t absdXL = abs(dXL);
        int32_t absdXR = abs(dXR);
        uint32_t sdXL = (uint32_t(dXL) >> 31) & 1;
        uint32_t sdXR = (uint32_t(dXR) >> 31) & 1;

        uint32_t AR0 = dYL;
        uint32_t AR1 = (sdXL != 0) ? (dXL + dYL) - 1 : -dXL;
        uint32_t AR2 = -absdXL;
        uint32_t AR4 = (sdXR != 0) ? (dXR + dYR) - 1 : -dXR;
        uint32_t AR5 = -absdXR;
        uint32_t AR6 = dYR;
        uint32_t sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                        (sdXR << Sign::Shift::SignOfDeltaXRight);
        YDestination_t ydst = vectors[vec0].y().RawFloor() * (s_Pitch >> 5);
        DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                    DrawingControl::AccessReplace |
                                    DrawingControl::ZAlways |
                                    DrawingControl::Solid |
                                    DrawingControl::ShiftZero |
                                    DrawingControl::BoolSrc;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::Sign::Write(controlAperture, sign);
        MMIO::XYAddress::WriteLeft(controlAperture, vectors[vec0].x().RawFloor());
        MMIO::XYAddress::WriteRight(controlAperture, vectors[vec0].x().RawFloor());
        MMIO::XYAddress::WriteYDestination(controlAperture, ydst);
        MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
        MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
        MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
        MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
        MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
        MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
        MMIO::Length::Write(controlAperture, min(dYL, dYR));
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);

        //Filter out dYL == dYR
        if (dYL > dYR)
        {
            dXR = vectors[vec2].x().RawFloor() - vectors[vec1].x().RawFloor();
            dYR = vectors[vec2].y().RawFloor() - vectors[vec1].y().RawFloor();
            absdXR = abs(dXR);
            sdXR = (uint32_t(dXR) >> 31) & 1;

            AR4 = (sdXR != 0) ? (dXR + dYR) - 1 : -dXR;
            AR5 = -absdXR;
            AR6 = dYR;
            sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                   (sdXR << Sign::Shift::SignOfDeltaXRight);
            dwgctl = DrawingControl::Trapezoid |
                     DrawingControl::AccessReplace |
                     DrawingControl::ZAlways |
                     DrawingControl::Solid |
                     DrawingControl::ShiftZero |
                     DrawingControl::BoolSrc;

            MMIO::Status::WaitDrawingEngineReady(controlAperture);
            MMIO::Sign::Write(controlAperture, sign);
            MMIO::XYAddress::WriteRight(controlAperture, vectors[vec1].x().RawFloor());
            MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
            MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
            MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
            MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
            MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
            MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
            MMIO::Length::Write(controlAperture, dYR);
            MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
        } else if (dYL < dYR)
        {
            dXL = vectors[vec1].x().RawFloor() - vectors[vec2].x().RawFloor();
            dYL = vectors[vec1].y().RawFloor() - vectors[vec2].y().RawFloor();
            absdXL = abs(dXL);
            sdXL = (uint32_t(dXL) >> 31) & 1;

            AR0 = dYL;
            AR1 = (sdXL != 0) ? (dXL + dYL) - 1 : -dXL;
            AR2 = -absdXL;
            sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                            (sdXR << Sign::Shift::SignOfDeltaXRight);
            DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                        DrawingControl::AccessReplace |
                                        DrawingControl::ZAlways |
                                        DrawingControl::Solid |
                                        DrawingControl::ShiftZero |
                                        DrawingControl::BoolSrc;

            MMIO::Status::WaitDrawingEngineReady(controlAperture);
            MMIO::Sign::Write(controlAperture, sign);
            MMIO::XYAddress::WriteLeft(controlAperture, vectors[vec2].x().RawFloor());
            MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
            MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
            MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
            MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
            MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
            MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
            MMIO::Length::Write(controlAperture, dYL);
            MMIO::ForegroundColor::Write(controlAperture, fgcolor);
            MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
        }
    }
    else if (vectors[vec0].y().Floor() == vectors[vec1].y().Floor())
    {
        int32_t dXL = vectors[vec2].x().RawFloor() - vectors[vec0].x().RawFloor();
        int32_t dYL = vectors[vec2].y().RawFloor() - vectors[vec0].y().RawFloor();
        int32_t dXR = vectors[vec2].x().RawFloor() - vectors[vec1].x().RawFloor();
        int32_t dYR = vectors[vec2].y().RawFloor() - vectors[vec1].y().RawFloor();
        int32_t absdXL = abs(dXL);
        int32_t absdXR = abs(dXR);
        uint32_t sdXL = (uint32_t(dXL) >> 31) & 1;
        uint32_t sdXR = (uint32_t(dXR) >> 31) & 1;

        uint32_t AR0 = dYL;
        uint32_t AR1 = (sdXL != 0) ? (dXL + dYL) - 1 : -dXL;
        uint32_t AR2 = -absdXL;
        uint32_t AR4 = (sdXR != 0) ? (dXR + dYR) - 1 : -dXR;
        uint32_t AR5 = -absdXR;
        uint32_t AR6 = dYR;
        uint32_t sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                        (sdXR << Sign::Shift::SignOfDeltaXRight);
        YDestination_t ydst = vectors[vec0].y().RawFloor() * (s_Pitch >> 5);
        DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                    DrawingControl::AccessReplace |
                                    DrawingControl::ZAlways |
                                    DrawingControl::Solid |
                                    DrawingControl::ShiftZero |
                                    DrawingControl::BoolSrc;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::Sign::Write(controlAperture, sign);
        MMIO::XYAddress::WriteLeft(controlAperture, vectors[vec0].x().RawFloor());
        MMIO::XYAddress::WriteRight(controlAperture, vectors[vec1].x().RawFloor());
        MMIO::XYAddress::WriteYDestination(controlAperture, ydst);
        MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
        MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
        MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
        MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
        MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
        MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
        MMIO::Length::Write(controlAperture, min(dYL, dYR));
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
    }
    else if (vectors[vec2].y().Floor() == vectors[vec0].y().Floor())
    {
        int32_t dXL = vectors[vec1].x().RawFloor() - vectors[vec2].x().RawFloor();
        int32_t dYL = vectors[vec1].y().RawFloor() - vectors[vec2].y().RawFloor();
        int32_t dXR = vectors[vec1].x().RawFloor() - vectors[vec0].x().RawFloor();
        int32_t dYR = vectors[vec1].y().RawFloor() - vectors[vec0].y().RawFloor();
        int32_t absdXL = abs(dXL);
        int32_t absdXR = abs(dXR);
        uint32_t sdXL = (uint32_t(dXL) >> 31) & 1;
        uint32_t sdXR = (uint32_t(dXR) >> 31) & 1;

        uint32_t AR0 = dYL;
        uint32_t AR1 = (sdXL != 0) ? (dXL + dYL) - 1 : -dXL;
        uint32_t AR2 = -absdXL;
        uint32_t AR4 = (sdXR != 0) ? (dXR + dYR) - 1 : -dXR;
        uint32_t AR5 = -absdXR;
        uint32_t AR6 = dYR;
        uint32_t sign = (sdXL << Sign::Shift::SignOfDeltaX) |
                        (sdXR << Sign::Shift::SignOfDeltaXRight);
        YDestination_t ydst = vectors[vec2].y().RawFloor() * (s_Pitch >> 5);
        DrawingControl_t dwgctl = DrawingControl::Trapezoid |
                                    DrawingControl::AccessReplace |
                                    DrawingControl::ZAlways |
                                    DrawingControl::Solid |
                                    DrawingControl::ShiftZero |
                                    DrawingControl::BoolSrc;

        MMIO::Status::WaitDrawingEngineReady(controlAperture);
        MMIO::Sign::Write(controlAperture, sign);
        MMIO::XYAddress::WriteLeft(controlAperture, vectors[vec2].x().RawFloor());
        MMIO::XYAddress::WriteRight(controlAperture, vectors[vec0].x().RawFloor());
        MMIO::XYAddress::WriteYDestination(controlAperture, ydst);
        MMIO::MultiPurposeAddress::Write0(controlAperture, AR0 & MultiPurposeAddress0::Value);
        MMIO::MultiPurposeAddress::Write1(controlAperture, AR1 & MultiPurposeAddress1::Value);
        MMIO::MultiPurposeAddress::Write2(controlAperture, AR2 & MultiPurposeAddress2::Value);
        MMIO::MultiPurposeAddress::Write4(controlAperture, AR4 & MultiPurposeAddress4::Value);
        MMIO::MultiPurposeAddress::Write5(controlAperture, AR5 & MultiPurposeAddress5::Value);
        MMIO::MultiPurposeAddress::Write6(controlAperture, AR6 & MultiPurposeAddress6::Value);
        MMIO::Length::Write(controlAperture, min(dYL, dYR));
        MMIO::ForegroundColor::Write(controlAperture, fgcolor);
        MMIO::DrawingControl::WriteCommit(controlAperture, dwgctl);
    }
}

void FlipFrame(Hag::Matrox::Shared::PCI::ControlAperture_t controlAperture, uint16_t width, uint16_t height)
{
    static bool even = false;
    WaitVerticalSync();
    SetStartAddress(even ? 0 : (width * height));
    SetStartY(controlAperture, even ? height : 0);
    SetClipping(controlAperture, 0, 0, width - 1, height);
    even = !even;
}


/////////////////////

Hag::Math::fp pX = Hag::Math::fp::Divide(5257311, 200000);
Hag::Math::fp pZ = Hag::Math::fp::Divide(8506508, 200000);
Hag::Math::fp nX = pX.Neg();
Hag::Math::fp nZ = pZ.Neg();

Hag::Math::v4 icoVecs[12] =
{
    Hag::Math::v4(nX,  0, pZ, 1), Hag::Math::v4(pX,  0, pZ, 1), Hag::Math::v4(nX,  0, nZ, 1), Hag::Math::v4(pX,  0, nZ, 1),
    Hag::Math::v4( 0, pZ, pX, 1), Hag::Math::v4( 0, pZ, nX, 1), Hag::Math::v4( 0, nZ, pX, 1), Hag::Math::v4( 0, nZ, nX, 1),
    Hag::Math::v4(pZ, pX,  0, 1), Hag::Math::v4(nZ, pX,  0, 1), Hag::Math::v4(pZ, nX,  0, 1), Hag::Math::v4(nZ, nX,  0, 1),
};

int32_t icoTri[20][3] =
{
    {0,  1,  4}, {0,  4, 9}, {9, 4,  5}, { 4, 8, 5}, {4,  1, 8},
    {8,  1, 10}, {8, 10, 3}, {5, 8,  3}, { 5, 3, 2}, {2,  3, 7},
    {7,  3, 10}, {7, 10, 6}, {7, 6, 11}, {11, 6, 0}, {0,  6, 1},
    {6, 10,  1}, {9, 11, 0}, {9, 2, 11}, { 9, 5, 2}, {7, 11, 2},
};

Hag::Math::v4 icoNorm[20];

void setupNormals()
{
    using namespace Hag::Math;

    for (uint32_t i = 0; i < 20; ++i)
    {
        v3 vx = (icoVecs[icoTri[i][1]] - icoVecs[icoTri[i][0]]).xyz().Normalize();
        v3 vy = (icoVecs[icoTri[i][2]] - icoVecs[icoTri[i][0]]).xyz().Normalize();
        icoNorm[i] = v4(vy.Cross(vx).Normalize(), 0);
    }
}

bool isBackFace(Hag::Math::v4& vec0, Hag::Math::v4& vec1, Hag::Math::v4& vec2)
{
    using namespace Hag::Math;
    static fp oneTenth = fp(1) / fp(10); //We were exceeding 32k and that makes the triangle blink
    fp d01x = (vec1.x() - vec0.x()) * oneTenth;
    fp d01y = (vec1.y() - vec0.y()) * oneTenth;
    fp d02x = (vec2.x() - vec0.x()) * oneTenth;
    fp d02y = (vec2.y() - vec0.y()) * oneTenth;
    return d01x * d02y - d01y * d02x >= 0;
}

void colorLerp(uint32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB)
{
    using namespace Hag;
    using namespace Hag::Math;

    for (uint32_t i = 0; i < steps; ++i)
    {
        fp fromCol = fp(steps - i) / fp(steps);
        fp toCol = fp(i) / fp(steps);
        fp fromRed = (colorA >> 16) & 0xFF;
        fp fromGreen = (colorA >> 8) & 0xFF;
        fp fromBlue = colorA & 0xFF;
        fp toRed = (colorB >> 16) & 0xFF;
        fp toGreen = (colorB >> 8) & 0xFF;
        fp toBlue = colorB & 0xFF;
        int16_t red = min<int16_t>(max<int16_t>((fromRed * fromCol + toRed * toCol).RawFloor(), 0), 255);
        int16_t green = min<int16_t>(max<int16_t>((fromGreen * fromCol + toGreen * toCol).RawFloor(), 0), 255);
        int16_t blue = min<int16_t>(max<int16_t>((fromBlue * fromCol + toBlue * toCol).RawFloor(), 0), 255);
        colors[i] = 0xFF000000 | (uint32_t(red) << 16) | (uint32_t(green) << 8) | uint32_t(blue);                    
    }
}
