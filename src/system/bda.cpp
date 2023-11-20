//Copyright 2023-Present riplin

#include <hag/system/bda.h>

namespace Hag { namespace System { namespace BDA
{

bool GetVideoParameterBlockElement(uint16_t index, uint8_t*& returnPointer, uint16_t size)
{
    bool ret = false;
    returnPointer = NULL;

    if (!VideoParameterControlBlockPointer::Get().IsNull())
    {
        FARPointer* realControlBlockPointer = VideoParameterControlBlockPointer::Get().ToPointer<FARPointer>(0x1C);
        if (!realControlBlockPointer[index].IsNull())
        {
            ret = true;
            returnPointer = realControlBlockPointer[index].ToPointer<uint8_t>(size);
        }
    }
    return ret;
}

bool CheckValidInCurrentMode(uint8_t* ptr)
{
    uint8_t displayMode = DisplayMode::Get();
    do
    {
        if(*ptr == 0xFF)
            return false;

        if (*ptr == displayMode)
            return true;

        ++ptr;
    } while (true);
}

void SetGraphicsCharacterFont(GraphicsCharacterSetOverride* graphicsCharacterFontDefinition)
{
    RowsOnScreen::Get() = graphicsCharacterFontDefinition->NumberOfCharacterRowsDisplayed - 1;
    PointHeightOfCharacterMatrix::Get() = graphicsCharacterFontDefinition->CharacterLength;
    
    //This would normally also set the int 43h vector to point to the font data. We don't do that.
}


}}}
