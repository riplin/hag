//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace S3 { namespace Shared
{

typedef uint16_t ChipId_t;

namespace ChipId
{
    
    enum
    {
        Unknown = 0x00,
        Vision864,      //Vision864     //86C864        //4MB                //Note: I'm not sure how many of these (non)-P variants exist.
        Vision864P,     //Vision864P    //86C864-P      //4MB
        Vision964,      //Vision964     //86C964        //8MB
        Vision964P,     //Vision964P    //86C964-P      //8MB
        Vision866,      //Vision866     //86C866        //?                 //I can't even find a picture of this card/chip.
        Vision868,      //Vision868     //86C868        //4MB
        Vision868P,     //Vision868P    //86C868-P      //4MB
        Vision968,      //Vision968     //86C968        //8MB
        Vision968P,     //Vision968P    //86C968-P      //8MB
        Trio32,         //Trio32        //86C732        //2MB
        Trio64,         //Trio64        //86C764        //4MB
        Trio64x,        //Trio64        //86C764X       //4MB
        Trio64Vp,       //Trio64V+      //86C765        //4MB
        Aurora64Vp,     //Aurora64V+    //86CM65        //4MB
        Trio64UVp,      //Trio64UV+     //86C767        //2MB
        TrioV2DX,       //Trio64V2/DX   //86C775        //4MB
        TrioV2GX,       //Trio64V2/GX   //86C785        //4MB
        Trio3D,         //Trio 3D       //86C365        //4MB
        Trio3D2X,       //Trio 3D/2X    //86C362        //8MB
        Trio3D2Xx,      //Trio 3D/2X    //86C368        //8MB
        ViRGEMX,        //ViRGE MX      //86C260        //4MB
        ViRGEMXp,       //ViRGE MX+     //86C280        //4MB
        ViRGE,          //ViRGE         //86C325        //4MB
        ViRGEGX2,       //ViRGE GX2     //86C357        //4MB
        ViRGEDXGX,      //ViRGE DX/GX   //86C375/86C385 //4MB   These chips cannot be distinguished from software. :(
        ViRGEVX,        //ViRGE VX      //86C988        //8MB
        Savage3D,       //Savage3D      //86C390        //8MB
        Savage3Dx,      //Savage3D      //86C391        //8MB
        Savage4LT,      //Savage4 LT    //86C394        //8MB
        Savage4GT,      //Savage4 GT    //86C395        //16MB
        Savage4PRO,     //Savage4 PRO   //86C397        //32MB
        SavageMX,       //Savage4 MX    //86C290        //16MB
        SavageIX,       //Savage/IX     //86C298        //16MB
        Savage2000,     //Savage2000    //86C410        //64MB


        UNSET = 0xFFFF
    };

    const char* ToString(ChipId_t value);
    const char* GetChipCode(ChipId_t value);

}

}}}
