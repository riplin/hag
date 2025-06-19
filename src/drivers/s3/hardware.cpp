//Copyright 2023-Present riplin

#include <stdio.h>

#include <hag/drivers/s3/hardware.h>
#include <hag/drivers/s3/shared/crtc/chipidrv.h>
#include <hag/drivers/s3/shared/crtc/devidlo.h>
#include <hag/drivers/s3/shared/crtc/devidhi.h>
#include <hag/drivers/s3/shared/crtc/reglock1.h>
#include <hag/drivers/s3/shared/crtc/revision.h>

namespace Hag::S3::Hardware
{

/*
Memory Sizes:

000 = 4MBytes
010 = 3MBytes
100 = 2MBytes
101 = 1.5MBytes
110 = 1MByte
111 = 0.5MByte

TODO: Check to see which ViRGE (and Trio3D) models use this table! (Standard Virge doesn't, but what about DX / GX, etc?)
ViRGE VX has a different table:
00 = 2MBytes
01 = 4MBytes
10 = 6MBytes
11 = 8MBytes

DRAM table (off-screen memory)
00 = Reserved
01 = 4MBytes
10 = 2MBytes
11 = 0MBytes


*/

/*
struct MaxMemorySizeByChip_t
{
    ChipId_t chipId;
    uint16_t maxMemorySizeInMiB;
};

static MaxMemorySizeByChip_t s_MaxMemorySize[] =
{
    //Don't assume that this table is in order (no direct indexing)!
    { ChipId::Vision864, 4 },
    { ChipId::Vision864P, 4 },
    { ChipId::Vision964, 8 },
    { ChipId::Vision964P, 8 },
    { ChipId::Vision866, 4 },
    { ChipId::Vision868, 4 },
    { ChipId::Vision868P, 4 },
    { ChipId::Vision968, 8 },
    { ChipId::Vision968P, 8 },
    { ChipId::Trio32, 2 },
    { ChipId::Trio64, 4 },
    { ChipId::Trio64x, 4 },
    { ChipId::Trio64Vp, 4 },
    { ChipId::Aurora64Vp, 4 },
    { ChipId::Trio64UVp, 2 },
    { ChipId::TrioV2DX, 4 },
    { ChipId::TrioV2GX, 4 },
    { ChipId::Trio3D, 4 },
    { ChipId::Trio3D2X, 8 },
    { ChipId::Trio3D2Xx, 8 },
    { ChipId::ViRGEMX, 4 },
    { ChipId::ViRGEMXp, 4 },
    { ChipId::ViRGE, 4 },
    { ChipId::ViRGEGX2, 4 },
    { ChipId::ViRGEDX, 4 },
    { ChipId::ViRGEGX, 4 },
    { ChipId::ViRGEVX, 4 },
};

uint16_t Trio::GetMaxMemorySizeInMiB(ChipId_t chipId)
{
    uint16_t maxMemorySizeInMiB = 0;
    for (int i = 0; i < sizeof(s_MaxMemorySize) / sizeof(MaxMemorySizeByChip_t); ++i)
    {
        if (s_MaxMemorySize[i].chipId == chipId)
        {
            maxMemorySizeInMiB = s_MaxMemorySize[i].maxMemorySizeInMiB;
            break;
        }
    }
    return maxMemorySizeInMiB;
}

uint16_t Trio::GetDisplayMemoryInKiB()
{
    static uint16_t memorySizeInKB = 0xFFFF;

    //Early out.
    if (memorySizeInKB != 0xFFFF)
        return memorySizeInKB;

    memorySizeInKB = 0;

    ChipId_t chipId = IdentifyChip();
    uint16_t maxMemorySizeInMB = GetMaxMemorySizeInMiB(chipId);

    if (maxMemorySizeInMB == 0)//unknown chip.
        return memorySizeInKB;

    //For now I have no better way to determine how to know which version of the Configuration1 register to use.
    if (maxMemorySizeInMB <= 4)
    {
        memorySizeInKB = CRTController::Configuration1::GetDisplayMemorySizeInKiB(m_CRTControllerIndex);
    }
    else
    {
        //The DRAM is not usable as display memory, but is added to the total memory size. We have to subtract it out.
        memorySizeInKB = CRTController::Configuration1V2::GetMemorySizeInKiB(m_CRTControllerIndex) -
                         CRTController::Configuration2V2::GetDRAMSizeInKiB(m_CRTControllerIndex);
    }

    return memorySizeInKB;
}


uint16_t Trio::GetOffScreenMemoryInKiB()
{
    static uint16_t memorySizeInKB = 0xFFFF;

    //Early out.
    if (memorySizeInKB != 0xFFFF)
        return memorySizeInKB;

    memorySizeInKB = 0;

    ChipId_t chipId = IdentifyChip();
    uint16_t maxMemorySizeInMB = GetMaxMemorySizeInMiB(chipId);

    if (maxMemorySizeInMB == 0)//unknown chip.
        return memorySizeInKB;

    //For now I have no better way to determine how to know which version of the Configuration1 register to use.
    if (maxMemorySizeInMB <= 4)
    {
        memorySizeInKB = 0;
    }
    else
    {
        memorySizeInKB = CRTController::Configuration2V2::GetDRAMSizeInKiB(m_CRTControllerIndex);
    }

    return memorySizeInKB;
}

void Trio::UnlockRegisters()
{
    //Write code to CR38 to provide access to the S3 VGA registers (CR30-CR3F)
    CRTController::RegisterLock1::Unlock(m_CRTControllerIndex);
    
    //Write code to CR39 to provide access to the System Control and System Extension registers (CR40-CRFF)
    CRTController::RegisterLock2::Unlock(m_CRTControllerIndex);

    //Set bit 0 in CR40 to enable access to the Enhanced Commands registers.
    CRTController::SystemConfiguration::Unlock(m_CRTControllerIndex);

    //Enable write access to bits 1 and 6 of CR7
    //Enable access to RAMDAC register
    //Enable access to Palette/Overscan registers
    CRTController::BackwardCompatibility2::UnlockAll(m_CRTControllerIndex);

    //Enable write access to CR0-CR6, CR7 (bits 7,5,3,2,0), CR9 (bit5), CR10, CR11 (bits 3-0), CR15-CR16, CR17 (bit 2)
    CRTController::CRTRegisterLock::UnlockAll(m_CRTControllerIndex);

    //Enable write access to CR0-CR7
    CRTController::VerticalRetraceEnd::Unlock(m_CRTControllerIndex);

    //write code to SR8 to provide access to SR9-SR18.
    Sequence::UnlockExtendedSequencer::Unlock();

}

void Trio::LockRegisters()
{
    //Wait for the graphics engine to be done.
    GraphicsProcessorStatus::Wait();

    Sequence::UnlockExtendedSequencer::Lock();

    CRTController::VerticalRetraceEnd::Lock(m_CRTControllerIndex);
    CRTController::CRTRegisterLock::LockAll(m_CRTControllerIndex);
    CRTController::BackwardCompatibility2::LockAll(m_CRTControllerIndex);
    CRTController::SystemConfiguration::Lock(m_CRTControllerIndex);
    CRTController::RegisterLock2::Lock(m_CRTControllerIndex);
    CRTController::RegisterLock1::Lock(m_CRTControllerIndex);
}
*/

    Shared::ChipId_t IdentifyChip()
    {
        static Shared::ChipId_t chipId = Shared::ChipId::UNSET;

        //Early out. we don't early out in the mock test since we'll be running multiple times.
#ifndef MOCK
        if (chipId != Shared::ChipId::UNSET)
            return chipId;
#endif

        chipId = Shared::ChipId::Unknown;

        //Figure out what the crtc index register is.
        VGA::Register_t crtcIndex = InColorMode() ? VGA::Register::CRTControllerIndexD : VGA::Register::CRTControllerIndexB;
        
        //Save the current lock state, we don't want to end up accidentally locking it if it was unlocked.
        Shared::CRTController::RegisterLock1_t reglock = Shared::CRTController::RegisterLock1::Read(crtcIndex);

        //Unlock needed registers.
        Shared::CRTController::RegisterLock1::Unlock(crtcIndex);

        //Old ChipID / revision is stored in CR30.
        Shared::CRTController::ChipIDRevision_t chipIDRevision = Shared::CRTController::ChipIDRevision::Read(crtcIndex);
        
        //Isolate the Chip ID.
        uint4_t oldChipID = (chipIDRevision & Shared::CRTController::ChipIDRevision::ChipID) >> Shared::CRTController::ChipIDRevision::Shift::ChipID;

        if (oldChipID != 0x0E) //Old chip ID method or unknown.
        {
            switch(chipIDRevision)
            {
            //case 0x81://  86c911
            //case 0x82://  86c911A/924
            //case 0x90://  86c928 original
            //case 0x91://  86c928 C-step
            //case 0x94://  86c928 D-step
            //case 0x95://  86c928 E-step
            //case 0xA0://  86c801/805 A or B-step
            //case 0xA2://  86c801/805 C-step
            //case 0xA5://  86c801/805 D-step
            //case 0xA6://  86c801/805 P-step
            //case 0xA8://  86c801/805 I-step
            //case 0xB0://  86c928PCI
            case 0xC0:
                chipId = Shared::ChipId::Vision864;
                break;
            case 0xC1:
                chipId = Shared::ChipId::Vision864P;
                break;
            case 0xD0:
                chipId = Shared::ChipId::Vision964;
                break;
            case 0xD1:
                chipId = Shared::ChipId::Vision964P;
            }
        }
        else //New chip ID method.
        {
            Shared::CRTController::DeviceIDLow_t deviceIdLow = Shared::CRTController::DeviceIDLow::Read(crtcIndex);
            Shared::CRTController::DeviceIDHigh_t deviceIdHigh = Shared::CRTController::DeviceIDHigh::Read(crtcIndex);
            Shared::CRTController::Revision_t revision = Shared::CRTController::Revision::Read(crtcIndex);

            switch (deviceIdLow)
            {
            case 0x01:
                if (deviceIdHigh == 89)
                {
                    chipId = Shared::ChipId::TrioV2DX;
                }
                else
                {
                    //Have to check memory type?
                    chipId = Shared::ChipId::ViRGEDXGX;
                }
                break;
            case 0x04:
                chipId = Shared::ChipId::Trio3D;
                break;
            case 0x10:
                if (deviceIdHigh == 0x88)
                {
                    chipId = Shared::ChipId::Trio32;
                }
                else
                {
                    chipId = Shared::ChipId::ViRGEGX2;
                }
                break;
            case 0x11:
                if ((revision & 0x40) == 0x40)
                {
                    chipId = Shared::ChipId::Trio64Vp;
                }
                else if (revision == 10)
                {
                    chipId = Shared::ChipId::Trio64;
                }
                else
                {
                    chipId = Shared::ChipId::Trio64x;
                }
                break;
            case 0x12:
                chipId = Shared::ChipId::Aurora64Vp;
                break;
            case 0x13:
                if (revision == 1)
                {
                    chipId = Shared::ChipId::Trio3D2X;
                }
                else
                {
                    chipId = Shared::ChipId::Trio3D2Xx;
                }
                break;
            case 0x14:
                chipId = Shared::ChipId::Trio64UVp;
                break;
            case 0x31:
                chipId = Shared::ChipId::ViRGE;
                break;
            case 0x3D:
                chipId = Shared::ChipId::ViRGEVX;
                break;
            case 0x80:
                chipId = Shared::ChipId::Vision866;
                break;
            case 0x90:
                chipId = Shared::ChipId::Vision868;
                break;
            case 0xB0:
                chipId = Shared::ChipId::Vision968;
                break;
            case 0xF0:
                chipId = Shared::ChipId::Vision968P;
                break;
            //case 0x??:
                //chipId = ChipId::Savage3D;
                //break;
            //case 0x??:
                //chipId = ChipId::Savage3Dx;
                //break;
            //case 0x??:
                //chipId = ChipId::Savage4LT;
                //break;
            //case 0x??:
                //chipId = ChipId::Savage4GT;
                //break;
            //case 0x??:
                //chipId = ChipId::Savage4PRO;
                //break;
            //case 0x??:
                //chipId = ChipId::SavageMX;
                //break;
            //case 0x??:
                //chipId = ChipId::SavageIX;
                //break;
            //case 0x??:
                //chipId = ChipId::Savage2000;
                //break;
            //case 0x??:
                //chipId = ChipId::TrioV2GX;
                //break;
            //case 0x??:
                //chipId = ChipId::ViRGEMX;
                //break;
            //case 0x??:
                //chipId = ChipId::ViRGEMXp;
                //break;
            //case 0x??: //   86c868P
                //chipId = ChipId::Vision868P;
                //break;
            }
        }

        //Restore the previous lock state.
        Shared::CRTController::RegisterLock1::Write(crtcIndex, reglock);

        return chipId;
    }

}

namespace Shared::ChipId
{
    const char* ToString(ChipId_t value)
    {
        switch(value)
        {
        case Vision864:
            return "Vision864";
        case Vision864P:
        return "Vision864P";
        case Vision964:
            return "Vision964";
        case Vision964P:
            return "Vision964P";
        case Vision866:
            return "Vision866";
        case Vision868:
            return "Vision868";
        case Vision868P:
            return "Vision868P";
        case Vision968:
            return "Vision968";
        case Vision968P:
            return "Vision968P";
        case Trio32:
            return "Trio32";
        case Trio64:
            return "Trio64";
        case Trio64x:
            return "Trio64";
        case Trio64Vp:
            return "Trio64V+";
        case Aurora64Vp:
            return "Aurora64V+";
        case Trio64UVp:
            return "Trio64UV+";
        case TrioV2DX:
            return "Trio64V2/DX";
        case TrioV2GX:
            return "Trio64V2/GX";
        case Trio3D:
            return "Trio 3D";
        case Trio3D2X:
            return "Trio 3D/2X";
        case Trio3D2Xx:
            return "Trio 3D/2X";
        case ViRGE:
            return "ViRGE";
        case ViRGEDXGX:
            return "ViRGE DX/GX";
        case ViRGEVX:
            return "ViRGE VX";
        case ViRGEGX2:
            return "ViRGE GX2";
        case ViRGEMX:
            return "ViRGE MX";
        case ViRGEMXp:
            return "ViRGE MX+";
        case Savage3D:
            return "Savage3D";
        case Savage3Dx:
            return "Savage3D";
        case Savage4LT:
            return "Savage4 LT";
        case Savage4GT:
            return "Savage4 GT";
        case Savage4PRO:
            return "Savage4 PRO";
        case SavageMX:
            return "Savage4 MX";
        case SavageIX:
            return "Savage/IX";
        case Savage2000:
            return "Savage2000";
        default:
            return "Unknown";
        }
    }

    const char* GetChipCode(ChipId_t value)
    {
        switch(value)
        {
        case Vision864:
            return "86C864";
        case Vision864P:
            return "86C864-P";
        case Vision964:
            return "86C964";
        case Vision964P:
            return "86C964-P";
        case Vision866:
            return "86C866";
        case Vision868:
            return "86C868";
        case Vision868P:
            return "86C868-P";
        case Vision968:
            return "86C968";
        case Vision968P:
            return "86C968-P";
        case Trio32:
            return "86C732";
        case Trio64:
            return "86C764";
        case Trio64x:
            return "86C764X";
        case Trio64Vp:
            return "86C765";
        case Aurora64Vp:
            return "86CM65";
        case Trio64UVp:
            return "86C767";
        case TrioV2DX:
            return "86C775";
        case TrioV2GX:
            return "86C785";
        case Trio3D:
            return "86C365";
        case Trio3D2X:
            return "86C362";
        case Trio3D2Xx:
            return "86C368";
        case ViRGE:
            return "86C325";
        case ViRGEDXGX:
            return "86C375/86C385";
        case ViRGEVX:
            return "86C988";
        case ViRGEGX2:
            return "86C357";
        case ViRGEMX:
            return "86C260";
        case ViRGEMXp:
            return "86C280";
        case Savage3D:
            return "86C390";
        case Savage3Dx:
            return "86C391";
        case Savage4LT:
            return "86C394";
        case Savage4GT:
            return "86C395";
        case Savage4PRO:
            return "86C397";
        case SavageMX:
            return "86C290";
        case SavageIX:
            return "86C298";
        case Savage2000:
            return "86C410";
        default:
            return "Unknown";
        }
    }

}
