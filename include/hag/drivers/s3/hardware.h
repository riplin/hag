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

#include <hag/drivers/s3/chipid.h>
#include <hag/drivers/vga/miscout.h>

namespace Hag { namespace S3
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

    ChipId_t IdentifyChip();
    Driver_t DriverToUse(ChipId_t chipId);
    inline bool InColorMode() { return (VGA::MiscellaneousOutput::Read() & VGA::MiscellaneousOutput::IOAddressSelect) != 0; }
}

/*
TODO:

        CPULatchData = 0x22,                                    //GCCL CR22
        AttributeIndexF = 0x24,                                 //ATC_F CR24 //Same as AttributeIndexI
        AttributeIndexI = 0x26,                                 //ATC_I CR26 //Same as AttributeIndexF
        MemoryConfiguration = 0x31,                             //MEM_CNFG CR31
        BackwardCompatibility1 = 0x32,                          //BKWD_1 CR32
        BackwardCompatibility3 = 0x34,                          //BKWD_3 CR34
        Configuration2 = 0x37,                                  //CONFG_REG2 CR37
        Miscellaneous1 = 0x3A,                                  //MISC_1 CR3A
        StartDisplayFIFO = 0x3B,                                //DT_EX_POS
        InterlaceRetraceStart = 0x3C,                           //IL_RTSTART CR3C

        BIOSFlag = 0x41,                                        //BIOS_FLAG CR41
        ModeControl = 0x42,                                     //MODE_CTL CR42
        HardwareGraphicsCursorMode = 0x45,                      //HGC_MODE CR45
        HardwareGraphicsCursorOriginXH = 0x46,                  //HWGC_ORGXH CR46
        HardwareGraphicsCursorOriginXL = 0x47,                  //HWGC_ORGXL CR47
        HardwareGraphicsCursorOriginYH = 0x48,                  //HWGC_ORGYH CR48
        HardwareGraphicsCursorOriginYL = 0x49,                  //HWGC_ORGYL CR49
        HardwareGraphicsCursorForegroundColorStack = 0x4A,      //HWGC_FGSTK CR4A
        HardwareGraphicsCursorBackgroundColorStack = 0x4B,      //HWGC_BGSTK CR4B
        HardwareGraphicsCursorStorageStartAddressH = 0x4C,      //HWGC_STAH CR4C
        HardwareGraphicsCursorStorageStartAddressL = 0x4D,      //HWGC_STAL CR4D
        HardwareGraphicsCursorPatternDisplayStartX = 0x4E,      //HWGC_DX CR4E
        HardwareGraphicsCursorPatternDisplayStartY = 0x4F,      //HWGC_DY CR4F

        ExtendedSystemControl1 = 0x50,                          //EX_SCTL_1 CR50
        ExtendedBIOSFlag1 = 0x52,                               //EXT_BBFLG1 CR52
        ExtendedMemoryControl1 = 0x53,                          //EXT_MCTL_1 CR53
        ExtendedMemoryControl2 = 0x54,                          //EXT_MCTL_2 CR54
        ExternalSyncControl1 = 0x56,                            //EX_SYNC_1 CR56
        ExternalSyncControl2 = 0x57,                            //EX_SYNC_2 CR57
        LinearAddressWindowControl = 0x58,                      //LAW_CTL CR58
        LinearAddressWindowPositionH = 0x59,                    //LAW_POS_H CR59
        LinearAddressWindowPositionL = 0x5A,                    //LAW_POS_L CR5A
        GeneralOutputPort = 0x5C,                               //GOUT_PORT CR5C
        ExtendedMemoryControl3 = 0x60,                          //EXT-MCTL-3 CR60
        ExternalSyncControl3 = 0x63,                            //EX-SYNC-3 CR63
        ExtendedMiscellaneousControl = 0x65,                    //EXT-MISC-CTL CR65
        ExtendedMiscellaneousControl1 = 0x66,                   //EXT-MISC-1 CR66
        ExtendedMiscellaneousControl2 = 0x67,                   //EXT-MISC-2 CR67
        Configuration3 = 0x68,                                  //CNFG-REG-3 CR68
        ExtendedSystemControl4 = 0x6A,                          //EXT-SCTL-4 CR6A
        ExtendedBIOSFlag3 = 0x6B,                               //EBIOS-FLG3 CR6B
        ExtendedBIOSFlag4 = 0x6C,                               //EBIOS-FLG4 CR6C

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

}}
