//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace VGA { namespace GraphicsControllerRegister
{

enum
{
    SetResetData = 0x00,                    //SET/RST_DT GR0
    EnableSetResetData = 0x01,              //EN_S/R_DT GR1
    ColorCompare = 0x02,                    //COLOR-CMP GR2
    RasterOperationRotateCount = 0x03,      //WT_ROP/RTC GR3
    ReadPlaneSelect = 0x04,                 //RD_PL_SL GR4
    GraphicsControllerMode = 0x05,          //GRP_MODE GR5
    MemoryMapModeControl = 0x06,            //MISC_GM GR6
    ColorDontCare = 0x07,                   //CMP_DNTC GR7
    BitMask = 0x08,                         //BIT_MASK GR8
};

}}}
