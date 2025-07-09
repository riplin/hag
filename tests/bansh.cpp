//Copyright 2025-Present riplin

#include <functional>
#include <sys/nearptr.h>
#include <support/allocatr.h>

#include <hag/color.h>
#include <hag/testing/log.h>
#include <hag/math/math.h>
#include <hag/drivers/3dfx/banshee/banshee.h>

// #include <xmmintrin.h>
// #ifdef __SSE__
// #error blah
// #endif

// __m128 a = { 0.0f, 1.0f, 2.0f, 3.0f };
// __m128 b = { 4.0f, 5.0f, 6.0f, 7.0f };

// __m128 c = _mm_add_ps(a, b);

// LOG("test", "c: %0.4f, %0.4f, %0.4f, %0.4f", c[0], c[1], c[2], c[3]);


extern Hag::Math::v4 icoVecs[12];
extern uint32_t icoTri[20][3];
extern Hag::Math::v4 icoNorm[20];

void setupNormals();
void colorLerp(Hag::Color32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB);

typedef std::function<void(Hag::Color32_t color, const Hag::Math::v4* triangle)> TriangleRender_t;
void RenderModel(const Hag::Math::v3& val,
                 uint16_t screenCenterX,
                 uint16_t screenCenterY,
                 const Hag::Math::v4& light,
                 const Hag::Math::v3& modelTranslate,
                 const Hag::Math::m44& scale,
                 const Hag::Math::m44& proj,
                 const Hag::Math::v4* vectors,
                 Hag::Math::v4* tempVectors,
                 uint32_t vectorCount,
                 const uint32_t(*indices)[3],
                 const Hag::Math::v4* normals,
                 Hag::Math::v4* tempNormals,
                 uint32_t indexCount,
                const TriangleRender_t& triangleRender);

void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void FillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void DrawTriangle(const Hag::Math::v4* vectors);

uint32_t colors[256];

int main(void)
{
    using namespace Hag;
    using namespace Hag::Math;
    using namespace Hag::TDfx;
    using namespace Hag::Color;

    __djgpp_nearptr_enable();

    LOG_CONFIGURE("bansh.txt");

    uint16_t width = 800;
    uint16_t height = 600;

    colorLerp(colors, 192, Bpp32::Black, Bpp32::RoyalPurple);
    colorLerp(colors + 192, 63, Bpp32::RoyalPurple, Bpp32::Cream);
    colors[255] = Bpp32::Cream;

    v3 val(0);
    v3 inc(Pi / 100, Pi / 75, Pi / 55);

    v4 icor[12];
    v4 icon[20];
    setupNormals();

    m44 scale = m44::Scale(v3(1.5f, 1.5f, 1.5f));
    m44 proj = m44::Projection(width, height, ToRad(90), One, 250);
    float scrX = width >> 1;
    float scrY = height >> 1;

    v4 light = v4(v3(1.0f).Normalize(), 0);
    
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

        RenderModel(val,
                    scrX,
                    scrY,
                    light,
                    v3(0, 0, 90),
                    scale,
                    proj,
                    icoVecs,
                    icor,
                    sizeof(icoVecs) / sizeof(v4),
                    icoTri,
                    icoNorm,
                    icon,
                    sizeof(icoTri) / sizeof(uint32_t[3]),
                    [](Color32_t color, const v4* triangle)
            {
                Banshee::Fifo::Deferred::TwoD::SetForegroundColor(color);
                DrawTriangle(triangle);
            });

        val = (val + inc) % TwoPi;        

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

void DrawTriangle(const Hag::Math::v4* vectors)
{
    using namespace Hag;
    using namespace Hag::TDfx;
    LOG("main", "DrawTriangle");

    uint32_t vecs[4] =
    {
        ((uint32_t(vectors[0].x()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[0].y()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
        ((uint32_t(vectors[1].x()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[1].y()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
        ((uint32_t(vectors[2].x()) << Banshee::TwoD::XY::Shift::X) & Banshee::TwoD::XY::X) |
        ((uint32_t(vectors[2].y()) << Banshee::TwoD::XY::Shift::Y) & Banshee::TwoD::XY::Y),
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
        int16_t d01x = ((vecs[1] & Banshee::TwoD::XY::X) >> Banshee::TwoD::XY::Shift::X) -
                       ((vecs[0] & Banshee::TwoD::XY::X) >> Banshee::TwoD::XY::Shift::X);
        int16_t d01y = ((vecs[1] & Banshee::TwoD::XY::Y) >> Banshee::TwoD::XY::Shift::Y) -
                       ((vecs[0] & Banshee::TwoD::XY::Y) >> Banshee::TwoD::XY::Shift::Y);
        int16_t d02x = ((vecs[2] & Banshee::TwoD::XY::X) >> Banshee::TwoD::XY::Shift::X) -
                       ((vecs[0] & Banshee::TwoD::XY::X) >> Banshee::TwoD::XY::Shift::X);
        int16_t d02y = ((vecs[2] & Banshee::TwoD::XY::Y) >> Banshee::TwoD::XY::Shift::Y) -
                       ((vecs[0] & Banshee::TwoD::XY::Y) >> Banshee::TwoD::XY::Shift::Y);
        if (d01x * d02y - d01y * d02x >= 0)
        {
            uint32_t tmp = vecs[1];
            vecs[1] = vecs[2];
            vecs[2] = tmp;
        }

        Banshee::Fifo::Deferred::TwoD::SetCommand(
            Banshee::TwoD::Command::CommandPolygonFill |
            Banshee::TwoD::Command::InitiateCommand,
            Banshee::TwoD::Rop::Source);

        Banshee::Fifo::Deferred::TwoD::Commit(0);

        Banshee::Fifo::WriteLaunchArea<0>(vecs + 1, 3);
    }
}

float pX = 52.5731112119133606f;
float pZ = 85.0650808352039932f;
float nX = -pX;
float nZ = -pZ;

Hag::Math::v4 icoVecs[12] =
{
    Hag::Math::v4(nX,  0, pZ, 1), Hag::Math::v4(pX,  0, pZ, 1), Hag::Math::v4(nX,  0, nZ, 1), Hag::Math::v4(pX,  0, nZ, 1),
    Hag::Math::v4( 0, pZ, pX, 1), Hag::Math::v4( 0, pZ, nX, 1), Hag::Math::v4( 0, nZ, pX, 1), Hag::Math::v4( 0, nZ, nX, 1),
    Hag::Math::v4(pZ, pX,  0, 1), Hag::Math::v4(nZ, pX,  0, 1), Hag::Math::v4(pZ, nX,  0, 1), Hag::Math::v4(nZ, nX,  0, 1),
};

uint32_t icoTri[20][3] =
{
    {0,  1,  4}, {0,  4, 9}, {9, 4,  5}, { 4, 8, 5}, {4,  1, 8},
    {8,  1, 10}, {8, 10, 3}, {5, 8,  3}, { 5, 3, 2}, {2,  3, 7},
    {7,  3, 10}, {7, 10, 6}, {7, 6, 11}, {11, 6, 0}, {0,  6, 1},
    {6, 10,  1}, {9, 11, 0}, {9, 2, 11}, { 9, 5, 2}, {7, 11, 2},
};

Hag::Math::v4 icoNorm[20];

void setupNormals()
{
    using namespace Hag::Math::flt;

    for (uint32_t i = 0; i < 20; ++i)
    {
        v3 vx = (icoVecs[icoTri[i][1]] - icoVecs[icoTri[i][0]]).xyz().Normalize();
        v3 vy = (icoVecs[icoTri[i][2]] - icoVecs[icoTri[i][0]]).xyz().Normalize();
        icoNorm[i] = v4(vy.Cross(vx).Normalize(), 0);
    }
}

bool isBackFace(const Hag::Math::v4& vec0, const Hag::Math::v4& vec1, const Hag::Math::v4& vec2)
{
    using namespace Hag::Math::flt;
    float d01x = (vec1.x() - vec0.x());
    float d01y = (vec1.y() - vec0.y());
    float d02x = (vec2.x() - vec0.x());
    float d02y = (vec2.y() - vec0.y());
    return d01x * d02y - d01y * d02x >= 0;
}

void RenderModel(const Hag::Math::v3& val,
                 uint16_t screenCenterX,
                 uint16_t screenCenterY,
                 const Hag::Math::v4& light,
                 const Hag::Math::v3& modelTranslate,
                 const Hag::Math::m44& scale,
                 const Hag::Math::m44& proj,
                 const Hag::Math::v4* vectors,
                 Hag::Math::v4* tempVectors,
                 uint32_t vectorCount,
                 const uint32_t(*indices)[3],
                 const Hag::Math::v4* normals,
                 Hag::Math::v4* tempNormals,
                 uint32_t indexCount,
                 const TriangleRender_t& triangleRender)
{
    using namespace Hag;
    using namespace Hag::Math;
    using namespace Hag::TDfx;

    LOG("main", "RenderModel");

    m44 rot = m44::RotateZ(val.z()) *
                    m44::RotateY(val.y()) *
                    m44::RotateX(val.x());

    m44 trans = proj * m44::Translate(modelTranslate) * rot * scale;

    for (uint32_t i = 0; i < vectorCount; ++i)
    {
        v4 t = trans * vectors[i];
        float invZ = One / t.z();
        tempVectors[i] = v4(t.x() * invZ + screenCenterX, t.y() * invZ + screenCenterY, t.z(), t.w());
    }

    for (uint32_t i = 0; i < indexCount; ++i)
    {
        tempNormals[i] = rot * normals[i];
    }

    v4 tri[3];

    for (uint32_t i = 0; i < indexCount; ++i)
    {
        tri[0] = tempVectors[indices[i][0]];
        tri[1] = tempVectors[indices[i][1]];
        tri[2] = tempVectors[indices[i][2]];
        if(isBackFace(tri[0], tri[1], tri[2]))
            continue;

        int16_t colIdx = max<int16_t>(min<int16_t>(int16_t((tempNormals[i].Dot(light) + 1) * 128), 255), 0);

        uint32_t color = colors[colIdx];
        triangleRender(color, tri);
    }
}

void colorLerp(Hag::Color32_t* colors, uint32_t steps, uint32_t colorA, uint32_t colorB)
{
    using namespace Hag;
    using namespace Hag::Math;

    for (uint32_t i = 0; i < steps; ++i)
    {
        float fromCol = float(steps - i) / float(steps);
        float toCol = float(i) / float(steps);
        float fromRed = (colorA >> 16) & 0xFF;
        float fromGreen = (colorA >> 8) & 0xFF;
        float fromBlue = colorA & 0xFF;
        float toRed = (colorB >> 16) & 0xFF;
        float toGreen = (colorB >> 8) & 0xFF;
        float toBlue = colorB & 0xFF;
        int16_t red = min<int16_t>(max<int16_t>(int16_t(fromRed * fromCol + toRed * toCol), 0), 255);
        int16_t green = min<int16_t>(max<int16_t>(int16_t(fromGreen * fromCol + toGreen * toCol), 0), 255);
        int16_t blue = min<int16_t>(max<int16_t>(int16_t(fromBlue * fromCol + toBlue * toCol), 0), 255);
        colors[i] = 0xFF000000 | (Color32_t(red) << 16) | (Color32_t(green) << 8) | Color32_t(blue);                    
    }
}