//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        sSetupMode = 0x260
    };
}

// sSetupMode Register
// The sSetupMode register provides a way for the CPU to only setup required parameters. When a Bit is
// set, that parameter will be calculated in the setup process, otherwise the value is not passed down to the
// triangle, and the previous value will be used. Also the definition of the triangle strip is defined in bits
// 21:16, where bit 16 defines fan. Culling is enabled by seting bit 17 to a value of “1”, whereas bit 18
// defines the culling sign. Bit 19 disables the ping pong sign inversion that happens during triangle strips.

// Triangle Setup Vertex Registers
// The sVx, sVy registers specify the x and y coordinates of a triangle strip to be rendered. A triangle strip,
// once the initial triangle has been defined, only requires a new X and Y to render consecutive triangles.
// The diagram below illustrates how triangle strips are sent over to Banshee.

// <see image in document>

// Triangle strips and triangle fans are implemented in Banshee by common vertex information and 2
// triangle commands. Vertex information is written to Banshee for a current vertex and are followed by a
// write to either the sBeginTriCMD or the sDrawTriCMD . For example, to render the triangle strip in the
// above figure, parameters X, Y, ARGB, W0, S/W, T/W for vertex R would be written followed by a write
// to sBeginTriCMD. Vertex D1’s parameters would next be written followed by a write to the
// sDrawTriCMD. After D2’s data has been sent, and the 2nd write to sDrawTriCMD has been completed
// Banshee will begin to render triangle 1. As triangle 1 is being rendered, data for vertex D3 will be sent
// down followed by another write to sDrawTriCMD, thus launching another triangle. Triangle fans are
// very similar to triangle strips. Instead of changing all three vertices, only the last 2 get modified.
// Triangle fans start with a sBeginTriCMD just as the triangle strip did, and send down sDrawTriCMD for
// every new vertex. To select triangle fan or triangle strip, you must write bit 0 of the triangle setup mode
// register.

typedef uint32_t sSetupMode_t;
namespace sSetupMode
{
    enum
    {
        RGB =                   0x00000001, // 0 Setup Red, Green, and Blue
        Alpha =                 0x00000002, // 1 Setup Alpha
        Z =                     0x00000004, // 2 Setup Z
        Wb =                    0x00000008, // 3 Setup Wb
        W0 =                    0x00000010, // 4 Setup W0
        S0T0 =                  0x00000020, // 5 Setup S0 and T0
        W1 =                    0x00000040, // 6 Setup W1
        S1T1 =                  0x00000080, // 7 Setup S1 and T1
        Type =                  0x00010000, // 16 Strip mode (0=strip, 1=fan)
        TypeStrip =             0x00000000,
        TypeFan =               0x00010000,
        Culling =               0x00020000, // 17 Enable Culling (0=disable, 1=enable)
        CullingDisable =        0x00000000,
        CullingEnable =         0x00020000,
        CullingSign =           0x00040000, // 18 Culling Sign (0=positive sign, 1=negative sign)
        CullingPositive =       0x00000000,
        CullingNegative =       0x00040000,
        PingPong =              0x00080000, // 19 Disable ping pong sign correction during triangle strips (0=normal, 1=disable)
        PingPongEnable =        0x00000000,
        PingPongDisable =       0x00080000
    };

    namespace Shift
    {
        enum
        {
            RGB =                   0x00,
            Alpha =                 0x01,
            Z =                     0x02,
            Wb =                    0x03,
            W0 =                    0x04,
            S0T0 =                  0x05,
            W1 =                    0x06,
            S1T1 =                  0x07,
            Type =                  0x10,
            Culling =               0x11,
            CullingSign =           0x12,
            PingPong =              0x13
        };
    }
}

}