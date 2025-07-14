//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    PixelDataTransfer = 0xE2E8,                             //PIX_TRANS
    PixelDataTransferExtension = 0xE2EA,                    //PIX_TRANS_EXT
};

}

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

namespace MMIO
{

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

}

}
