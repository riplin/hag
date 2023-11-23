//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/regs.h>

namespace Hag { namespace S3
{

typedef VGA::Register_t Register_t;

namespace Register
{

enum
{
    SetupOptionSelect = 0x102,                              //SETUP_MD
    VideoSubsystemEnableS3 = 0x46E8,                        //SETUP_MD // Original VGA this is 0x3C3
    SubsystemStatus = 0x42E8,                               //SUBSYS_STAT
    SubsystemControl = 0x42E8,                              //SUBSYS_CNTL
    AdvancedFunctionControl = 0x4AE8,                       //ADVFUNC_CNTL
    CurrentYPosition = 0x82E8,                              //CUR_Y
    CurrentYPosition2 = 0x82EA,                             //CUR_Y2
    CurrentXPosition = 0x86E8,                              //CUR_X
    CurrentXPosition2 = 0x86EA,                             //CUR_X2
    DestinationYPositionAxialStepConstant = 0x8AE8,         //DESTY_AXSTP
    YCoordinate2AxialStepConstant2 = 0x8AEA,                //Y2_AXSTP2
    DestinationXPositionDiagonalStepConstant = 0x8EE8,      //DESTX_DIASTP
    XCoordinate2 = 0x8EEA,                                  //X2_DIASTP2
    LineErrorTerm = 0x92E8,                                 //ERR_TERM
    LineErrorTerm2 = 0x92EA,                                //ERR_TERM2
    MajorAxisPixelCount = 0x96E8,                           //MAJ_AXIS_PCNT
    MajorAxisPixelCount2 = 0x96EA,                          //MAJ_AXIS_PCNT2
    GraphicsProcessorStatus = 0x9AE8,                       //GP_STAT
    DrawingCommand = 0x9AE8,                                //CMD
    DrawingCommand2 = 0x9AEA,                               //CMD2
    ShortStrokeVectorTransfer = 0x9EE8,                     //SHORT_STROKE
    BackgroundColor = 0xA2E8,                               //BKGD_COLOR
    ForegroundColor = 0xA6E8,                               //FRGD_COLOR
    BitplaneWriteMask = 0xAAE8,                             //WRT_MASK
    BitplaneReadMask = 0xAEE8,                              //RD_MASK
    ColorCompare = 0xB2E8,                                  //COLOR_CMP
    BackgroundMix = 0xB6E8,                                 //BKGD_MIX
    ForegroundMix = 0xBAE8,                                 //FRGD_MIX
    ReadRegisterData = 0xBEE8,                              //RD_REG_DT
    WriteRegisterData = 0xBEE8,                             //WR_REG_DT
    PixelDataTransfer = 0xE2E8,                             //PIX_TRANS
    PixelDataTransferExtension = 0xE2EA,                    //PIX_TRANS_EXT
    PatternY = 0xEAE8,                                      //PAT_Y
    PatternX = 0xEAEA                                       //PAT_X
};

namespace Packed
{
    enum
    {
        CurrentXYPosition = 0x8100,
        CurrentXYPosition2 = 0x8104,
        DestinationXYPositionStepConstant = 0x8108,
        XYCoordinate2 = 0x810C,
        LinearErrorTerms = 0x8110,
        DrawingCommands = 0x8118,
        ShortStrokeVectorTransfer = 0x811C,
        BackgroundColor = 0x8120,
        ForegroundColor = 0x8124,
        BitplaneWriteMask = 0x8128,
        BitplaneReadMask = 0x812C,
        ColorCompare = 0x8130,
        Mixers = 0x8134,
        ScissorsTopLeft = 0x8138,
        ScissorsBottomRight = 0x813C,
        PixelControlAndMultiFunctionMisc2 = 0x8140,
        MultiFunctionAndReadSelect = 0x8144,
        MinorMajorAxisPixelCounts = 0x8148,
        MajorAxisPixelCount2 = 0x814C,
        PatternXY = 0x8168
    };
}

}

}}
