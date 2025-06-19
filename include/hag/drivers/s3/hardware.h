//Copyright 2023-Present riplin

/*
 Reading material: 
 Trio32/Trio64 Graphics Accelerators: DB014-B_Trio32_Trio64_Graphics_Accelerators_Mar1995.pdf
*/

/*
Values pulled off of video cards:

Chip        Name            ID & Revision   Device Low      Device High     Revision    Detected    Conf1   Conf2

86C864-P    Vision864       C0              C0              C0              C0          *
86C964-P    Vision964       D1              D1              D1              D1          *
86C775      Trio64V2/DX     E1              01              89              16          *
86C375      ViRGE/DX        E1              01              8A              01          ?           
86C385      ViRGE/GX        E1              01              8A              01          ?           
86C365      Trio 3D         E1              04              89              01          *
86C732      Trio32          E1              10              88              11          *
86C357      ViRGE/GX2       E1              10              8A              06          *
86C764X     Trio64          E1              11              88              03          *
86C764X     Trio64          E1              11              88              20          *
86C764-P    Trio64          E1              11              88              10          *
86C765      Trio64V+        E1              11              88              54          *
86CM65      Aurora64V+      E1              12              88              02          *
86C362      Trio 3D/2X      E1              13              8A              01          *
86C368      Trio 3D/2X      E1              13              8A              02          *
86C767      TrioUV+         E1              14              88              52          *
86C325      ViRGE           E1              31              56              05          *
86C988      ViRGE/VX        E1              3D              88              02          *
86C868-P    Vision868       E1              90              88              03          *
86C968-P    Vision968       E1              F0              88              03          *

They look the same to me.

DX  Conf1   Conf2       Conf1         Conf2
    12      FB          0 00 1 11 11    1 11 1 1 0 1 1
    1E      FB          0 00 1 11 10    1 11 1 1 0 1 1

GX  1A      FB          0 00 1 10 10    1 11 1 1 0 1 1
    12      FB          0 00 1 11 11    1 11 1 1 0 1 1
    16      FB          0 00 1 01 10    1 11 1 1 0 1 1
*/

#pragma once

#include <hag/drivers/s3/shared/chipid.h>
#include <hag/drivers/vga/miscout.h>

namespace Hag::S3
{

namespace Hardware
{
    typedef uint16_t Driver_t;
    enum
    {
        Unsupported = 0x00,
        Trio = 0x01,
        ViRGE = 0x02,
        Trio3D = 0x03,
    };

    Shared::ChipId_t IdentifyChip();
    Driver_t DriverToUse(Shared::ChipId_t chipId);
    inline bool InColorMode() { return (VGA::MiscellaneousOutput::Read() & VGA::MiscellaneousOutput::IOAddressSelect) != 0; }
}

/*
TODO:

        Configuration3 = 0x68,                                  //CNFG-REG-3 CR68
        ExtendedSystemControl4 = 0x6A,                          //EXT-SCTL-4 CR6A

*/    
/*
class Trio
{
private:
    Register_t m_FeatureControlW;
    Register_t m_InputStatus1;
    Register_t m_CRTControllerIndex;
    Register_t m_CRTControllerData;
public:
    inline Trio()
    {
        if (ColorSupport())
        {
            m_FeatureControlW = Register::FeatureControlWD;
            m_InputStatus1 = Register::InputStatus1D;
            m_CRTControllerIndex = Register::CRTControllerIndexD;
            m_CRTControllerData = Register::CRTControllerDataD;
        }
        else
        {
            m_FeatureControlW = Register::FeatureControlWB;
            m_InputStatus1 = Register::InputStatus1B;
            m_CRTControllerIndex = Register::CRTControllerIndexB;
            m_CRTControllerData = Register::CRTControllerDataB;
        }
        UnlockRegisters();
    }

    inline ~Trio()
    {
        LockRegisters();
    }


    //Called by the constructor / destructor.
    void UnlockRegisters();
    void LockRegisters();

    void TestModeSet();

    uint16_t GetDisplayMemoryInKiB();
    uint16_t GetOffScreenMemoryInKiB();
    uint16_t GetMaxMemorySizeInMiB(ChipId_t chipId);

    static inline bool ColorSupport() { return MiscellaneousOutput::GetIOAddressSelect(MiscellaneousOutput::Read()) != 0; }
    static ChipId_t IdentifyChip();
};
*/

}
