//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/data.h>
#include <hag/drivers/matrox/shared/crtcext/regtype.h>

namespace Hag { namespace Matrox { namespace Shared { namespace CRTCExtension
{

namespace Register
{

enum
{
    AddressGeneratorExtensions = 0x00,                          //CER0
};

}

typedef Shared::CRTCExtensionData_t AddressGeneratorExtensions_t;

namespace AddressGeneratorExtensions
{
    
    enum //Mask
    {
        StartAddress19_16 = 0x0F,       //Start Address bits 19-16
        Offset9_8 = 0x30,               //Offset bits 9-8
        StartAddress20 = 0x40,          //Start Address bit 20
        InterlaceMode = 0x80            //Interlace Mode 0 = off, 1 = enabled
    };

    namespace Shift
    {
        enum
        {
            StartAddress19_16 = 0x00,
            Offset9_8 = 0x04,
            StartAddress20 = 0x06,
            InterlaceMode = 0x07
        };
    }

    inline AddressGeneratorExtensions_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::AddressGeneratorExtensions);
        return AddressGeneratorExtensions_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(AddressGeneratorExtensions_t value)
    {
        Shared::CRTCExtensionData::Write(Register::AddressGeneratorExtensions, Shared::CRTCExtensionData_t(value));
    }

}

}}}}