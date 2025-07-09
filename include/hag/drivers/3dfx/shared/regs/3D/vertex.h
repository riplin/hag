//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fp.h>
#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        Ax = 0x008,
        Ay = 0x00c,
        Bx = 0x010,
        By = 0x014,
        Cx = 0x018,
        Cy = 0x01c,
        FloatAx = 0x088,
        FloatAy = 0x08c,
        FloatBx = 0x090,
        FloatBy = 0x094,
        FloatCx = 0x098,
        FloatCy = 0x09c
    };
}

namespace Alias
{
    enum
    {
        Ax = 0x008,
        Ay = 0x00c,
        Bx = 0x010,
        By = 0x014,
        Cx = 0x018,
        Cy = 0x01c,
        FloatAx = 0x088,
        FloatAy = 0x08c,
        FloatBx = 0x090,
        FloatBy = 0x094,
        FloatCx = 0x098,
        FloatCy = 0x09c
    };
}

// vertex and fvertex Registers
// The vertexAx, vertexAy, vertexBx, vertexBy, vertexCx, vertexCy, fvertexAx, fvertexAy, fvertexBx,
// fvertexBy, fvertexCx, and fvertexCy registers specify the x and y coordinates of a triangle to be
// rendered. There are three vertices in an Banshee triangle, with the AB and BC edges defining the minor
// edge and the AC edge defining the major edge. The diagram below illustrates two typical triangles:

//                       (vertexAx, vertexAy)                   (vertexAx, vertexAy)
//                                *                                      *
//                              *  *                                    *  *
//                Major Edge  *     *  Minor Edge          Minor Edge  *     *  Major Edge
//                          *        * (vertexBx,          (vertexBx, *        *
//                        *       *     vertexBy)           vertexBy)    *       *
//                      *      *                                            *      *
//                    *     *  Minor Edge                         Minor Edge   *     *
//                  *    *                                                        *    *
//                *   *                                                              *   *
//              *  *                                                                    *  *
//            * *                                                                          * *
//          *                                                                                  *
// (vertexCx, vertexCy)                                                               (vertexCx, vertexCy)

// The fvertex registers are floating point equivalents of the vertex registers. Banshee automatically
// converts both the fvertex and vertex registers into an internal fixed point notation used for rendering.



typedef Fixed12p4_t Vertex_t;
namespace Vertex
{
    using namespace Fixed12p4;

    namespace Shift
    {
        using namespace Fixed12p4::Shift;
    }
}

}