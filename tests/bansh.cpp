//Copyright 2025-Present riplin

#include <sys/nearptr.h>
#include <support/allocatr.h>

#include <hag/color.h>
#include <hag/testing/log.h>
#include <hag/math/fp/fpmath.h>
#include <hag/drivers/3dfx/banshee/banshee.h>

extern Hag::Math::v4 icoVecs[12];
extern int32_t icoTri[20][3];
extern Hag::Math::v4 icoNorm[20];

void setupNormals();
void colorLerp(Hag::Color32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB);
bool isBackFace(Hag::Math::v4& vec0, Hag::Math::v4& vec1, Hag::Math::v4& vec2);

void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void FillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void DrawTriangle(Hag::Math::v4* vectors);

uint32_t colors[256];

int main(void)
{
    using namespace Hag;
    using namespace Hag::Math;
    using namespace Hag::TDfx;
    using namespace Hag::Color;

    __djgpp_nearptr_enable();

    LOG_CONFIGURE("bansh.txt");

    uint16_t width = 640;
    uint16_t height = 480;

    colorLerp(colors, 192, Bpp32::Black, Bpp32::RoyalPurple);
    colorLerp(colors + 192, 63, Bpp32::RoyalPurple, Bpp32::Cream);
    colors[255] = Bpp32::Cream;

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
    
    Support::Allocator allocator;
    Banshee::Initialize(allocator);

    Banshee::SetVideoMode(width, height, Banshee::BitsPerPixel::Bpp24, Banshee::Flags::LinearFramebuffer);
    Banshee::SetupBuffers(Banshee::Buffers::Depth16Bpp | Banshee::Buffers::DoubleBuffer);

    do
    {
        Banshee::Fifo::Deferred::TwoD::SetForegroundColor(Color::Bpp32::Black);
        FillRectangle(0, 0, width, height);

        Banshee::Fifo::Deferred::TwoD::SetForegroundColor(Color::Bpp32::BabyBlue);
        DrawRectangle(0, 0, width - 1, height - 1);

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
/*
        tri[0] = v4(fp(0), fp(-50), 0, 1);
        tri[1] = v4(fp(-40), fp(50), 0, 1);
        tri[2] = v4(fp(40), fp(50), 0, 1);

        tri[0] = m44::RotateZ(val.z()) * tri[0];
        tri[0] += v4(scrX, scrY, 0, 0);
        tri[1] = m44::RotateZ(val.z()) * tri[1];
        tri[1] += v4(scrX, scrY, 0, 0);
        tri[2] = m44::RotateZ(val.z()) * tri[2];
        tri[2] += v4(scrX, scrY, 0, 0);

        DrawTriangle(tri);
*/

        for (int32_t i1 = 0; i1 < 20; ++i1)
        {
            tri[0] = icor[icoTri[i1][0]];
            tri[2] = icor[icoTri[i1][1]];
            tri[1] = icor[icoTri[i1][2]];
            if(isBackFace(tri[0], tri[2], tri[1]))
                continue;

            int16_t colIdx = max<int16_t>(min<int16_t>(((icon[i1].Dot(light) + 1) * 128).RawFloor(), 255), 0);

            uint32_t color = colors[colIdx];
            Banshee::Fifo::Deferred::TwoD::SetForegroundColor(color);
            DrawTriangle(tri);
            //DrawLine(tri[0].x().RawFloor(), tri[0].y().RawFloor(), tri[1].x().RawFloor(), tri[1].y().RawFloor());
            //DrawLine(tri[1].x().RawFloor(), tri[1].y().RawFloor(), tri[2].x().RawFloor(), tri[2].y().RawFloor());
            //DrawLine(tri[2].x().RawFloor(), tri[2].y().RawFloor(), tri[0].x().RawFloor(), tri[0].y().RawFloor());
        }

        Banshee::Fifo::Deferred::TwoD::SetForegroundColor(Color::Bpp32::LightGreen);
/*
        tri[0] = v4(fp(50), fp(50), 0, 1);
        tri[1] = v4(fp(70), fp(50), 0, 1);
        tri[2] = v4(fp(60), fp(70), 0, 1);
        DrawTriangle(tri);
        
        tri[0] = v4(fp(90), fp(50), 0, 1);
        tri[1] = v4(fp(80), fp(70), 0, 1);
        tri[2] = v4(fp(100), fp(70), 0, 1);
        DrawTriangle(tri);

        tri[0] = v4(fp(110), fp(50), 0, 1);
        tri[1] = v4(fp(130), fp(60), 0, 1);
        tri[2] = v4(fp(110), fp(70), 0, 1);
        DrawTriangle(tri);
        

        tri[0] = v4(fp(160), fp(50), 0, 1);
        tri[1] = v4(fp(140), fp(60), 0, 1);
        tri[2] = v4(fp(160), fp(70), 0, 1);
        DrawTriangle(tri);
*/
        val = (val + inc) % fp::TwoPi;        

        Banshee::SwapScreen2D(true);
    } while (!kbhit());

    Banshee::SetVideoMode(80, 25, Banshee::BitsPerPixel::Bpp4, Banshee::Flags::Text);

    Banshee::Shutdown();

	__djgpp_nearptr_disable();
    return 0;
}

void FillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    using namespace Hag;
    using namespace Hag::TDfx;
    LOG("main", "FillRectangle");

    uint32_t left = min(x0, x1);
    uint32_t top = min(y0, y1);
    uint32_t width = (max(x0, x1) - left);
    uint32_t height = (max(y0, y1) - top);

    Banshee::Fifo::Deferred::TwoD::SetDestinationRect(left, top, width, height);
    Banshee::Fifo::Deferred::TwoD::SetCommand(
        Banshee::TwoD::Command::CommandRectangleFill | Banshee::TwoD::Command::InitiateCommand,
        Banshee::TwoD::Rop::Source);
    
    Banshee::Fifo::Deferred::TwoD::Commit(0);
}

void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    using namespace Hag;
    using namespace Hag::TDfx;
    LOG("main", "DrawRectangle");

    uint32_t left = min(x0, x1);
    uint32_t top = min(y0, y1);
    uint32_t right = max(x0, x1);
    uint32_t bottom = max(y0, y1);

    Banshee::Fifo::Deferred::TwoD::SetSourceXY(left, top);
    Banshee::Fifo::Deferred::TwoD::SetCommand(
        Banshee::TwoD::Command::CommandPolyLine,
        Banshee::TwoD::Rop::Source);

    Banshee::Fifo::Deferred::TwoD::Commit(0);

    Banshee::Fifo::Command_t points[4] =
    {
        (right << Banshee::TwoD::PolygonLaunch::Shift::X) | (top << Banshee::TwoD::PolygonLaunch::Shift::Y),
        (right << Banshee::TwoD::PolygonLaunch::Shift::X) | (bottom << Banshee::TwoD::PolygonLaunch::Shift::Y),
        (left << Banshee::TwoD::PolygonLaunch::Shift::X) | (bottom << Banshee::TwoD::PolygonLaunch::Shift::Y),
        (left << Banshee::TwoD::PolygonLaunch::Shift::X) | (top << Banshee::TwoD::PolygonLaunch::Shift::Y)
    };

    Banshee::Fifo::WriteLaunchArea<0>(points, sizeof(points) / sizeof(Banshee::Fifo::Command_t));
}

void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    using namespace Hag::TDfx;
    LOG("main", "DrawLine");

    Shared::Fifo::Deferred::TwoD::SetSourceXY(x0, y0);
    Shared::Fifo::Deferred::TwoD::SetDestinationXY(x1, y1);
    Shared::Fifo::Deferred::TwoD::SetCommand(
                                   Shared::TwoD::Command::CommandPolyLine
                                   | Shared::TwoD::Command::InitiateCommand,
                                    Shared::TwoD::Rop::Source);
    
    Shared::Fifo::Deferred::TwoD::Commit(0);
}

void DrawTriangle(Hag::Math::v4* vectors)
{
    using namespace Hag;
    using namespace Hag::TDfx;
    LOG("main", "DrawTriangle");

    LOG("main", "DrawTriangle vector[0] = %i, %i", vectors[0].x().RawFloor(), vectors[0].y().RawFloor());
    LOG("main", "DrawTriangle vector[1] = %i, %i", vectors[1].x().RawFloor(), vectors[1].y().RawFloor());
    LOG("main", "DrawTriangle vector[2] = %i, %i", vectors[2].x().RawFloor(), vectors[2].y().RawFloor());

    uint32_t vecs[4] =
    {
        ((uint32_t(vectors[0].x().RawFloor()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[0].y().RawFloor()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
        ((uint32_t(vectors[1].x().RawFloor()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[1].y().RawFloor()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
        ((uint32_t(vectors[2].x().RawFloor()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[2].y().RawFloor()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
        0
    };

    std::sort(vecs, vecs + 3);
    vecs[3] = vecs[2];

    Banshee::Fifo::Deferred::TwoD::SetSourceXY(vecs[0]);

    if ((vecs[0] & Banshee::TwoD::XY::Y) == (vecs[1] & Banshee::TwoD::XY::Y))
    {
        Banshee::Fifo::Deferred::TwoD::SetDestinationXY(vecs[1]);

        Banshee::Fifo::Deferred::TwoD::SetCommand(
            Banshee::TwoD::Command::CommandPolygonFill,
            Banshee::TwoD::Rop::Source);

        Banshee::Fifo::Deferred::TwoD::Commit(0);

        Banshee::Fifo::WriteLaunchArea<0>(vecs + 2, 2);
    }
    else
    {
        if ((vecs[2] & Banshee::TwoD::XY::X) < (vecs[1] & Banshee::TwoD::XY::X))
        {
            uint32_t tmp = vecs[2];
            vecs[2] = vecs[1];
            vecs[1] = tmp;
        }

        Banshee::Fifo::Deferred::TwoD::SetCommand(
            Banshee::TwoD::Command::CommandPolygonFill |
            Banshee::TwoD::Command::InitiateCommand,
            Banshee::TwoD::Rop::Source);

        Banshee::Fifo::Deferred::TwoD::Commit(0);

        Banshee::Fifo::WriteLaunchArea<0>(vecs + 1, 3);
    }
}

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

void colorLerp(Hag::Color32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB)
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
        colors[i] = 0xFF000000 | (Color32_t(red) << 16) | (Color32_t(green) << 8) | Color32_t(blue);                    
    }
}