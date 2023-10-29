//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace S3 { namespace CRTControllerRegister
{
    
    enum
    {
        HorizontalTotal = 0x00,                                 //H_TOTAL CR0
        HorizontalDisplayEnd = 0x01,                            //H_D_END CR1
        StartHorizontalBlank = 0x02,                            //S_H_BLNK CR2
        EndHorizontalBlank = 0x03,                              //E_H_BLNK CR3
        StartHorizontalSyncPosition = 0x04,                     //S_H_SY_P CR4
        EndHorizontalSyncPosition = 0x05,                       //E_H_SY_P CR5
        VerticalTotal = 0x06,                                   //V_TOTAL CR6
        CRTCOverflow = 0x07,                                    //OVFL_REG CR7
        PresetRowScan = 0x08,                                   //P_R_SCAN CR8
        MaximumScanLine = 0x09,                                 //MAX_S_LN CR9
        CursorStartScanLine = 0x0A,                             //CSSL CRA
        CursorEndScanLine = 0x0B,                               //CESL CRB
        StartAddressHigh = 0x0C,                                //STA(H) CRC
        StartAddressLow = 0x0D,                                 //STA(L) CRD
        CursorLocationAddressHigh = 0x0E,                       //CLA(H) CRE
        CursorLocationAddressLow = 0x0F,                        //CLA(L) CRF
        VerticalRetraceStart = 0x10,                            //VRS CR10
        VerticalRetraceEnd = 0x11,                              //VRE CR11
        VerticalDisplayEnd = 0x12,                              //VDE CR12
        ScreenOffset = 0x13,                                    //SCREEN-OFFSET CR13
        UnderlineLocation = 0x14,                               //ULL CR14
        StartVerticalBlank = 0x15,                              //SVB CR15
        EndVerticalBlank = 0x16,                                //EVB CR16
        CRTCModeControl = 0x17,                                 //CRT_MD CR17
        LineCompare = 0x18,                                     //LCM CR18
        CPULatchData = 0x22,                                    //GCCL CR22
        AttributeIndexF = 0x24,                                 //ATC_F CR24 //Same as AttributeIndexI
        AttributeIndexI = 0x26,                                 //ATC_I CR26 //Same as AttributeIndexF

        DeviceIDHigh = 0x2D,                                    //CR2D
        DeviceIDLow = 0x2E,                                     //CR2E
        Revision = 0x2F,                                        //CR2F
        ChipIDRevision = 0x30,                                  //CHIP-ID/REV CR30
        MemoryConfiguration = 0x31,                             //MEM_CNFG CR31
        BackwardCompatibility1 = 0x32,                          //BKWD_1 CR32
        BackwardCompatibility2 = 0x33,                          //BKWD_2 CR33
        BackwardCompatibility3 = 0x34,                          //BKWD_3 CR34
        CRTRegisterLock = 0x35,                                 //CRTR_LOCK CR35
        Configuration1 = 0x36,                                  //CONFG_REG1 CR36
        Configuration2 = 0x37,                                  //CONFG_REG2 CR37
        RegisterLock1 = 0x38,                                   //REG_LOCK1 CR38
        RegisterLock2 = 0x39,                                   //REG_LOCK2 CR39
        Miscellaneous1 = 0x3A,                                  //MISC_1 CR3A
        StartDisplayFIFO = 0x3B,                                //DT_EX_POS
        InterlaceRetraceStart = 0x3C,                           //IL_RTSTART CR3C

        SystemConfiguration = 0x40,                             //SYS_CNFG CR40
        BIOSFlag = 0x41,                                        //BIOS_FLAG CR41
        ModeControl = 0x42,                                     //MODE_CTL CR42
        ExtendedMode = 0x43,                                    //EXT_MODE CR43
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
        ExtendedSystemControl2 = 0x51,                          //EX_SCTL_2 CR51
        ExtendedBIOSFlag1 = 0x52,                               //EXT_BBFLG1 CR52
        ExtendedMemoryControl1 = 0x53,                          //EXT_MCTL_1 CR53
        ExtendedMemoryControl2 = 0x54,                          //EXT_MCTL_2 CR54
        ExtendedRAMDACControl = 0x55,                           //EX_DAC_CT CR55
        ExternalSyncControl1 = 0x56,                            //EX_SYNC_1 CR56
        ExternalSyncControl2 = 0x57,                            //EX_SYNC_2 CR57
        LinearAddressWindowControl = 0x58,                      //LAW_CTL CR58
        LinearAddressWindowPositionH = 0x59,                    //LAW_POS_H CR59
        LinearAddressWindowPositionL = 0x5A,                    //LAW_POS_L CR5A
        GeneralOutputPort = 0x5C,                               //GOUT_PORT CR5C
        ExtendedHorizontalOverflow = 0x5D,                      //EXT_H_OVF CR5D
        ExtendedVerticalOverflow = 0x5E,                        //EXT_V_OVF CR5E
        ExtendedMemoryControl3 = 0x60,                          //EXT-MCTL-3 CR60
        ExternalSyncControl3 = 0x63,                            //EX-SYNC-3 CR63
        ExtendedMiscellaneousControl = 0x65,                    //EXT-MISC-CTL CR65
        ExtendedMiscellaneousControl1 = 0x66,                   //EXT-MISC-1 CR66
        ExtendedMiscellaneousControl2 = 0x67,                   //EXT-MISC-2 CR67
        Configuration3 = 0x68,                                  //CNFG-REG-3 CR68
        ExtendedSystemControl3 = 0x69,                          //EXT-SCTL-3 CR69
        ExtendedSystemControl4 = 0x6A,                          //EXT-SCTL-4 CR6A
        ExtendedBIOSFlag3 = 0x6B,                               //EBIOS-FLG3 CR6B
        ExtendedBIOSFlag4 = 0x6C,                               //EBIOS-FLG4 CR6C
    };
}}}
