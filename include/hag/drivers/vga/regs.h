//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace VGA
{

typedef uint16_t Register_t;

namespace Register
{
    
    enum
    {
        CRTControllerIndexB = 0x3B4,                            //CRX //Mono
        CRTControllerDataB = 0x3B5,                             //CRTC_DATA CRT //Mono
        InputStatus1B = 0x3BA,                                  //STATUS_1 //Mono
        FeatureControlWB = 0x3BA,                               //FCR_WT //Mono
        AttributeControllerIndex = 0x3C0,                       //ATR_AD
        AttributeControllerDataW = 0x3C0,                       //ATR_DATA
        AttributeControllerDataR = 0x3C1,                       //ATR_DATA
        InputStatus0 = 0x3C2,                                   //STATUS_0
        MiscellaneousW = 0x3C2,                                 //MISC
        VideoSubsystemEnable = 0x3C3,                           //SETUP_MD  //This register is reserved in standard VGA
        SequencerIndex = 0x3C4,                                 //SEQX
        SequencerData = 0x3C5,                                  //SEQ_DATA
        DACMask = 0x3C6,                                        //DAC_AD_MK
        DACReadIndex = 0x3C7,                                   //DAC_RD_AD
        DACStatus = 0x3C7,                                      //DAC_STS
        DACWriteIndex = 0x3C8,                                  //DAC_WR_AD
        RAMDACData = 0x3C9,                                     //DAC_DATA
        FeatureControlR = 0x3CA,                                //FCR_AD
        MiscellaneousR = 0x3CC,                                 //MISC
        GraphicsControllerIndex = 0x3CE,                        //GRC_ADR
        GraphicsControllerData = 0x3CF,                         //GRC_DATA
        CRTControllerIndexD = 0x3D4,                            //CRX //Color
        CRTControllerDataD = 0x3D5,                             //CRTC_DATA CRT //Color
        InputStatus1D = 0x3DA,                                  //STATUS_1 //Color
        FeatureControlWD = 0x3DA,                               //FCR_WT //Color
    };

}

}}
