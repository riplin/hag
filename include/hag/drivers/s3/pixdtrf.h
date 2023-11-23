//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t PixelDataTransfer16_t;
typedef uint32_t PixelDataTransfer32_t;

namespace PixelDataTransfer
{

    inline PixelDataTransfer16_t Read16()
    {
        return PixelDataTransfer16_t(SYS_ReadPortShort(Register::PixelDataTransfer));
    }

    inline PixelDataTransfer32_t Read16x2()
    {
        return PixelDataTransfer32_t(SYS_ReadPortShort(Register::PixelDataTransfer)) |
               (PixelDataTransfer32_t(SYS_ReadPortShort(Register::PixelDataTransferExtension)) << 16);
    }

    inline PixelDataTransfer16_t Read32()
    {
        return PixelDataTransfer32_t(SYS_ReadPortShort(Register::PixelDataTransfer));
    }

    inline void Write16(PixelDataTransfer16_t value)
    {
        SYS_WritePortShort(Register::PixelDataTransfer, value);
    }

    inline void Write16x2(PixelDataTransfer16_t value)
    {
        SYS_WritePortShort(Register::PixelDataTransfer, value);
    }

    inline void Write16x2(PixelDataTransfer32_t value)
    {
        SYS_WritePortShort(Register::PixelDataTransfer, PixelDataTransfer16_t(value));
        SYS_WritePortShort(Register::PixelDataTransferExtension, PixelDataTransfer16_t(value >> 16));
    }

    inline void Write32(PixelDataTransfer32_t value)
    {
        SYS_WritePortDouble(Register::PixelDataTransfer, value);
    }

}

}}
