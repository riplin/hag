//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace VGA { namespace CRTControllerRegister
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
};

}}}
