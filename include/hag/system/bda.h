//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/testing/mock.h>

//This API handles the BIOS Data Area

#pragma pack(push, 1);

namespace Hag { namespace System { namespace BDA
{
    namespace Offset
    {
        enum
        {
            DetectedHardware                    = 0x10, //Word 0x410
            DisplayMode                         = 0x49, //Byte 0x449
            NumberOfScreenColumns               = 0x4A, //Word 0x44a
            VideoBufferSize                     = 0x4C, //Word 0x44c
            VideoBufferOffset                   = 0x4E, //Word 0x44e
            CursorPositionPage0                 = 0x50, //Word 0x450
            CursorPositionPage1                 = 0x52, //Word 0x452
            CursorPositionPage2                 = 0x54, //Word 0x454
            CursorPositionPage3                 = 0x56, //Word 0x456
            CursorPositionPage4                 = 0x58, //Word 0x458
            CursorPositionPage5                 = 0x5A, //Word 0x45a
            CursorPositionPage6                 = 0x5C, //Word 0x45c
            CursorPositionPage7                 = 0x5E, //Word 0x45e
            CursorEndScanLine                   = 0x60, //Byte 0x460
            CursorStartScanLine                 = 0x61, //Byte 0x461
            ActiveDisplayNumber                 = 0x62, //Byte 0x462
            VideoBaseIOPort                     = 0x63, //Word 0x463
            CRTModeControlRegValue              = 0x65, //Byte 0x465
            CGAColorPaletteMaskSetting          = 0x66, //Byte 0x466
            RowsOnScreen                        = 0x84, //Byte 0x484
            PointHeightOfCharacterMatrix        = 0x85, //Word 0x485
            VideoModeOptions                    = 0x87, //Byte 0x487
            EGAFeatureBitSwitches               = 0x88, //Byte 0x488
            VideoDisplayDataArea                = 0x89, //Byte 0x489
            DisplayCombinationCodeTableIndex    = 0x8a, //Byte 0x48a
            VideoParameterControlBlockPointer   = 0xa8 //Dword 0x4a8
        };
    }

#ifndef MOCK
    template<typename T, uint16_t Offs> T& GetVariable(uint16_t count = 1) { (void)count; return *(T*)(0x400 + Offs); }
#else
    template<typename T, uint16_t Offs> T& GetVariable(uint16_t count = 1) { return Testing::Mock::BDA::RefAs<T, Offs>(count); }
#endif
    
    typedef uint8_t DetectedHardware_t;
    namespace DetectedHardware
    {
        enum
        {
            InitialVideoModeMask         = 0x30,
            Color40x25                   = 0x10,
            Color80x25                   = 0x20,
            Monochrome80x25              = 0x30
        };

        inline DetectedHardware_t& Get()
        {
            return GetVariable<DetectedHardware_t, Offset::DetectedHardware>();
        }
    }

    namespace DisplayMode
    {
        inline VGA::VideoMode_t& Get()
        {
            return GetVariable<VGA::VideoMode_t, Offset::DisplayMode>();
        }
    }

    typedef uint16_t NumberOfScreenColumns_t;
    namespace NumberOfScreenColumns
    {
        inline NumberOfScreenColumns_t& Get()
        {
            return GetVariable<NumberOfScreenColumns_t, Offset::NumberOfScreenColumns>();
        }
    }

    typedef uint16_t VideoBufferSize_t;
    namespace VideoBufferSize
    {
        inline VideoBufferSize_t& Get()
        {
            return GetVariable<VideoBufferSize_t, Offset::VideoBufferSize>();
        }
    }

    typedef uint16_t VideoBufferOffset_t;
    namespace VideoBufferOffset
    {
        inline VideoBufferOffset_t& Get()
        {
            return GetVariable<VideoBufferOffset_t, Offset::VideoBufferOffset>();
        }
    }

    struct Position
    {
        uint8_t Column;
        uint8_t Row;
    };

    typedef Position CursorPosition_t;
    namespace CursorPositions
    {
        //There are 8 cursor positions.
        inline CursorPosition_t* Get()
        {
            return &GetVariable<CursorPosition_t, Offset::CursorPositionPage0>(8);
        }
    }

    struct EndStart
    {
        inline EndStart() : End(0), Start(0) {}
        inline EndStart(uint8_t end, uint8_t start) : End(end), Start(start) {}

        uint8_t End;
        uint8_t Start;
    };

    typedef EndStart CursorScanLines_t;
    namespace CursorScanLines
    {
        inline CursorScanLines_t& Get()
        {
            return GetVariable<CursorScanLines_t, Offset::CursorEndScanLine>();
        }
    }

    typedef uint8_t ActiveDisplayNumber_t;
    namespace ActiveDisplayNumber
    {
        inline ActiveDisplayNumber_t& Get()
        {
            return GetVariable<ActiveDisplayNumber_t, Offset::ActiveDisplayNumber>();
        }
    }
    
    typedef uint16_t VideoBaseIOPort_t;
    namespace VideoBaseIOPort
    {
        inline VideoBaseIOPort_t& Get()
        {
            return GetVariable<VideoBaseIOPort_t, Offset::VideoBaseIOPort>();
        }
    }

    typedef uint8_t CRTModeControlRegValue_t;
    namespace CRTModeControlRegValue
    {
        enum
        {
            Mode2Or3Text              = 0x01,
            Mode4Or5Graphics          = 0x02,
            Monochrome                = 0x04,
            VideoEnabled              = 0x08,
            GraphicsOperation         = 0x10,
            Blinking                  = 0x20 //Off = Intensity
        };

        inline CRTModeControlRegValue_t& Get()
        {
            return GetVariable<CRTModeControlRegValue_t, Offset::CRTModeControlRegValue>();
        }
    }
    
    typedef uint8_t CGAColorPaletteMaskSetting_t;
    namespace CGAColorPaletteMaskSetting
    {
        enum
        {
            Blue = 0x01,
            Green = 0x02,
            Red = 0x04,
            BorderAndBackgroundColorIntensity = 0x08, //intensified border color (mode 2) and background color (mode 5)
            BackgroundColor = 0x10, //0 = normal bg color, 1 = intensified bg color
            ForgroundColorSelect = 0x20, //0 = Green/Red/Yellow, 1 = Cyan/Magenta/White
        };

        inline CGAColorPaletteMaskSetting_t& Get()
        {
            return GetVariable<CGAColorPaletteMaskSetting_t, Offset::CGAColorPaletteMaskSetting>();
        }
    }

    typedef uint8_t RowsOnScreen_t;
    namespace RowsOnScreen
    {
        inline RowsOnScreen_t& Get()
        {
            return GetVariable<RowsOnScreen_t, Offset::RowsOnScreen>();
        }
    }

    typedef uint8_t PointHeightOfCharacterMatrix_t;
    namespace PointHeightOfCharacterMatrix
    {
        inline PointHeightOfCharacterMatrix_t& Get()
        {
            return GetVariable<PointHeightOfCharacterMatrix_t, Offset::PointHeightOfCharacterMatrix>();
        }
    }

    typedef uint8_t VideoModeOptions_t;
    namespace VideoModeOptions
    {
        enum
        {
            CursorEmulationEnabled      = 0x01,
            Monochrome                  = 0x02,
            Unknown                     = 0x04,
            Inactive                    = 0x08,
            Memory64k                   = 0x00,
            Memory128k                  = 0x20,
            Memory192k                  = 0x40,
            Memory256k                  = 0x60,
            MemoryMask                  = 0x60,
            DontClearDisplay            = 0x80
        };

        inline VideoModeOptions_t& Get()
        {
            return GetVariable<VideoModeOptions_t, Offset::VideoModeOptions>();
        }
    }

    typedef uint8_t EGAFeatureBitSwitches_t;
    namespace EGAFeatureBitSwitches
    {
        enum
        {
            MDAColor40x25              = 0x00,
            MDAColor80x25              = 0x01,
            MDAHiRes80x25              = 0x02,
            MDAHiResEnhanced           = 0x03,
            CGAMono40x25               = 0x04,
            CGAMono80x25               = 0x05,
            MDAColor40x25_2            = 0x06,
            MDAColor80x25_2            = 0x07,
            MDAHiRes80x25_2            = 0x08,
            MDAHiResEnhanced_2         = 0x09,
            CGAMono40x25_2             = 0x0A,
            CGAMono80x25_2             = 0x0B,
            AdapterTypeMask            = 0x0F,
            FeatureConnector0          = 0x40,
            FeatureConnector1          = 0x80,
            FeatureConnectorMask       = 0xF0
        };

        inline EGAFeatureBitSwitches_t& Get()
        {
            return GetVariable<EGAFeatureBitSwitches_t, Offset::EGAFeatureBitSwitches>();
        }
    }

    typedef uint8_t VideoDisplayDataArea_t;
    namespace VideoDisplayDataArea
    {
        enum
        {
            VGA                        = 0x01,
            GrayScale                  = 0x02,
            MonochromeMonitor          = 0x04,
            PaletteLoadingEnabled      = 0x08,
            DisplaySwitchingEnabled    = 0x40,
            LineMode350                = 0x00,
            LineMode400                = 0x10,
            LineMode200                = 0x80,
            LineModeMask               = 0x90
        };

        inline VideoDisplayDataArea_t& Get()
        {
            return GetVariable<VideoDisplayDataArea_t, Offset::VideoDisplayDataArea>();
        }
    }

    typedef uint8_t DisplayCombinationCodeTableIndex_t;
    namespace DisplayCombinationCodeTableIndex
    {
        inline DisplayCombinationCodeTableIndex_t& Get()
        {
            return GetVariable<DisplayCombinationCodeTableIndex_t, Offset::DisplayCombinationCodeTableIndex>();
        }
    }
    
    struct VideoParameterTable
    {
        uint8_t NumCharacterColumns;
        uint8_t NumScreenRowsMinus1;
        uint8_t CharacterMatrixHeightInPoints;
        uint16_t VideoBufferSize;
        uint8_t SequencerRegisters[4];
        uint8_t MiscellaneousOutputRegister;
        uint8_t CRTCRegisters[25];
        uint8_t AttributeControllerRegisters[20];
        uint8_t GraphicsControllerRegisters[9];
    };

    struct DynamicParameterSaveArea
    {
        uint8_t PaletteRegisters[16];
        uint8_t GraphicsControllerOverscanRegister;
        uint8_t Reserved[239];
    };

    struct AlphaNumericCharacterSetOverride
    {
        uint8_t CharacterLength;
        uint8_t CharacterGeneratorRAMBank;
        uint16_t NumberOfCharacters;
        uint16_t FirstCharacterCodeInTable;
        FARPointer CharacterFontDefinitionTable;
        uint8_t NumberOfCharacterRowsDisplayed;
        uint8_t ApplicableVideoModes[];//Terminated by 0xFF
    };

    struct GraphicsCharacterSetOverride
    {
        uint8_t NumberOfCharacterRowsDisplayed;
        uint8_t CharacterLength;
        FARPointer CharacterFontDefinitionTable;
        uint8_t ApplicableVideoModes[];//Terminated by 0xFF
    };

    struct SecondarySavePointerTable
    {
        uint16_t LengthOfTableInBytes;
        FARPointer DisplayCombinationCodeTable;
        FARPointer SecondaryAlphanumericCharacterSetOverride;
        FARPointer UserPaletteProfileTable;
        FARPointer Reserved1;
        FARPointer Reserved2;
        FARPointer Reserved3;
    };

    struct DisplayCombinationCodeTable
    {
        uint8_t NumTableEntries;
        uint8_t DCCTableVersionNumber;
        uint8_t MaximumDisplayTypeCode;
        uint8_t Reserved;
        uint16_t DisplayCombinations[];
        /*
        0,0 entry  0  no display
        0,1 entry  1  MDPA
        0,2 entry  2  CGA
        2,1 entry  3  MDPA + CGA
        0,4 entry  4  EGA
        4,1 entry  5  EGA + MDPA
        0,5 entry  6  MEGA
        2,5 entry  7  MEGA + CGA
        0,6 entry  8  PGC
        1,6 entry  9  PGC + MDPA
        5,6 entry 10  PGC + MEGA
        0,8 entry 11  CVGA
        1,8 entry 12  CVGA + MDPA
        0,7 entry 13  MVGA
        2,7 entry 14  MVGA + CGA
        2,6 entry 15  MVGA + PGC
        */
    };


/*



Secondary Alpha Mode Auxillary Character Generator Table

00   byte    bytes per character
01   byte    block to load
02   byte    reserved
03   dword   font table pointer
07   nbytes  array of mode values for this font
07+n byte    FFh end of mode list marker


Palette Profile Table (VGA only)

00   byte    1 - enable underlining in all alphanumeric modes
            0 - enable underlining in monochrome alpha modes
        -1 - disable underlining in all alpha modes
01   byte   reserved
02   word   reserved
04   word   count of attribute controller regs in table
06   word   first attribute controller register number
08   dword  pointer to attribute controller reg table
0C   word   count of video DAC color registers in table
0E   word   first video DAC color register number
10   dword  video DAC color register table pointer
14   nbytes array of applicable video modes for this font
14+n byte   FFh end of video mode list marker


*/


    typedef FARPointer VideoParameterControlBlockPointer_t;
    namespace VideoParameterControlBlockPointer
    {
        inline VideoParameterControlBlockPointer_t& Get()
        {
            return GetVariable<VideoParameterControlBlockPointer_t, Offset::VideoParameterControlBlockPointer>();
        }

    }

}}}

#pragma pack(pop);
