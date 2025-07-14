//Copyright 2023-Present riplin

#pragma once

#include <string.h>
#include <has/types.h>
#include <has/farptr.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/testing/mock.h>

//This API handles the BIOS Data Area

namespace Hag::System::BDA
{
    #pragma pack(push, 1)

    struct Position
    {
        inline Position() : Column(0), Row(0) {}
        inline Position(uint8_t column, uint8_t row) : Column(column), Row(row) {}
        
        uint8_t Column;
        uint8_t Row;
    };

    struct EndStart
    {
        inline EndStart() : End(0), Start(0) {}
        inline EndStart(uint8_t end, uint8_t start) : End(end), Start(start) {}

        uint8_t End;
        uint8_t Start;
    };

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

    struct GraphicsCharacterSet
    {
        uint8_t NumberOfCharacterRowsDisplayed;
        uint16_t CharacterLength;
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

    struct AlphanumericCharSet
    {
        uint8_t CharacterHeight;                //00   byte    length of each character definition in bytes
        uint8_t RamBank;                        //01   byte    character generator RAM bank
        uint16_t NumCharacters;                 //02   word    count of characters defined
        uint16_t FirstCharacter;                //04   word    first character code in table
        FARPointer FontData;                    //06   dword   pointer to character font definition table
        uint8_t Rows;                           //0A   byte    number of character rows displayed
        VGA::VideoMode_t ApplicableModes[1];    //0B   nbytes  array of applicable video modes
                                                //0B+n byte    FFh end of mode list marker
    };

    struct SecondaryAlphaModeAuxillaryCharacterGeneratorTable
    {
        uint8_t CharacterHeight;                //00   byte    bytes per character
        uint8_t RamBank;                        //01   byte    block to load
        uint8_t Unknown;                        //02   byte    reserved
        FARPointer FontData;                    //03   dword   font table pointer
        VGA::VideoMode_t ApplicableModes[1];    //07   nbytes  array of mode values for this font
                                                //07+n byte    FFh end of mode list marker
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

    struct PaletteProfile
    {
        int8_t Underlining;                     //00   byte    1 - enable underlining in all alphanumeric modes
                                                //             0 - enable underlining in monochrome alpha modes
                                                //            -1 - disable underlining in all alpha modes
        uint8_t Reserved1;                      //01   byte   reserved
        uint16_t Reserved2;                     //02   word   reserved
        uint16_t AttributeRegisterCount;        //04   word   count of attribute controller regs in table
        uint16_t AttributeRegisterStartIndex;   //06   word   first attribute controller register number
        FARPointer AttributeRegisterTable;      //08   dword  pointer to attribute controller reg table
        uint16_t DACRegisterCount;              //0C   word   count of video DAC color registers in table
        uint16_t DACRegisterStartIndex;         //0E   word   first video DAC color register number
        FARPointer DACRegisterTable;            //10   dword  video DAC color register table pointer
        VGA::VideoMode_t ApplicableModes[1];    //14   nbytes array of applicable video modes for this font
                                                //14+n byte   FFh end of video mode list marker
    };

    struct VideoParameterControlBlock
    {
        FARPointer VideoParameters;                 //VideoParameterTable
        FARPointer DynamicParamSaveArea;            //DynamicParameterSaveArea
        FARPointer AlphanumericCharsetOverride;     //AlphanumericCharSet
        FARPointer GraphicsCharacterSetOverride;    //GraphicsCharacterSet
        FARPointer SecondarySavePointer;            //SecondarySavePointerTable
        FARPointer Reserved1;
        FARPointer Reserved2;
    };

    typedef uint8_t DetectedHardware_t;
    typedef uint16_t NumberOfScreenColumns_t;
    typedef uint16_t VideoBufferSize_t;
    typedef uint16_t VideoBufferOffset_t;
    typedef Position CursorPosition_t;
    typedef EndStart CursorScanLines_t;
    typedef uint8_t ActiveDisplayNumber_t;
    typedef uint8_t CRTModeControlRegValue_t;
    typedef uint8_t CGAColorPaletteMaskSetting_t;
    typedef uint8_t RowsOnScreen_t;
    typedef uint16_t PointHeightOfCharacterMatrix_t;
    typedef uint8_t VideoModeOptions_t;
    typedef uint8_t EGAFeatureBitSwitches_t;
    typedef uint8_t VideoDisplayDataArea_t;
    typedef uint8_t DisplayCombinationCodeTableIndex_t;
    typedef FARPointer VideoParameterControlBlockPointer_t;


    namespace DetectedHardware
    {
        enum
        {
            InitialVideoModeMask         = 0x30,
            Color40x25                   = 0x10,
            Color80x25                   = 0x20,
            Monochrome80x25              = 0x30
        };
    }

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
    }
    
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
    }

    namespace VideoModeOptions
    {
        enum
        {
            CursorEmulationEnabled      = 0x01,
            Monochrome                  = 0x02,
            Color                       = 0x00,
            Unknown                     = 0x04,
            Inactive                    = 0x08,
            Memory64k                   = 0x00,
            Memory128k                  = 0x20,
            Memory192k                  = 0x40,
            Memory256k                  = 0x60,
            MemoryMask                  = 0x60,
            DontClearDisplay            = 0x80
        };
    }

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
    }

    namespace VideoDisplayDataArea
    {
        enum
        {
            VGA                        = 0x01,
            GrayScale                  = 0x02,
            MonochromeMonitor          = 0x04,
            PaletteLoadingDisabled     = 0x08,
            Reserved                   = 0x20,
            DisplaySwitchingEnabled    = 0x40,
            LineMode350                = 0x00,
            LineMode400                = 0x10,
            LineMode200                = 0x80,
            LineModeMask               = 0x90
        };
    }

    struct Instance
    {
        uint16_t Com1PortAddress;                                                   // 40:00
        uint16_t Com2PortAddress;                                                   // 40:02
        uint16_t Com3PortAddress;                                                   // 40:04
        uint16_t Com4PortAddress;                                                   // 40:06
        uint16_t Lpt1PortAddress;                                                   // 40:08
        uint16_t Lpt2PortAddress;                                                   // 40:0a
        uint16_t Lpt3PortAddress;                                                   // 40:0c
        uint16_t Lpt4PortAddress;                                                   // 40:0e
        DetectedHardware_t DetectedHardware;                                        // 40:10
        uint8_t MoreDetectedHardware;                                               // 40:11
        uint8_t  PcJrInfraKeyboardLinkRedErrorCount;                                // 40:12
        uint16_t MemorySizeInKiloByte;                                              // 40:13
        uint8_t  Reserved15;                                                        // 40:15
        uint8_t  Ps2BIOSControlFlags;                                               // 40:16
        uint8_t  KeyboardFlagByte0;                                                 // 40:17
        uint8_t  KeyboardFlagByte1;                                                 // 40:18
        uint8_t  AlternateKeypadEntry;                                              // 40:19
        uint16_t KeyboardBufferOffsetHead;                                          // 40:1a
        uint16_t KeyboardBufferOffsetTail;                                          // 40:1c
        uint8_t  KeyboardBuffer[32];                                                // 40:1e
        uint8_t  DriveRecalibrationStatus;                                          // 40:3e
        uint8_t  DisketteMotorStatus;                                               // 40:3f
        uint8_t  MotorShutoffCounter;                                               // 40:40
        uint8_t  LastDisketteOperationStatus;                                       // 40:41
        uint8_t  NECDisketteControllerStatus[7];                                    // 40:42
        VGA::VideoMode_t DisplayMode;                                               // 40:49
        NumberOfScreenColumns_t NumberOfScreenColumns;                              // 40:4a
        VideoBufferSize_t VideoBufferSize;                                          // 40:4c
        VideoBufferOffset_t VideoBufferOffset;                                      // 40:4e
        CursorPosition_t CursorPositions[8];                                        // 40:50
        CursorScanLines_t CursorScanLines;                                          // 40:60
        ActiveDisplayNumber_t ActiveDisplayNumber;                                  // 40:62
        VGA::Register_t VideoBaseIOPort;                                            // 40:63
        CRTModeControlRegValue_t CRTModeControlRegValue;                            // 40:65
        CGAColorPaletteMaskSetting_t CGAColorPaletteMaskSetting;                    // 40:66
        uint32_t DayCounter;                                                        // 40:67
        uint8_t  Reserved6b;                                                        // 40:6b
        uint32_t DailyTimer;                                                        // 40:6c
        uint8_t  ClockRolloverFlag;                                                 // 40:70
        uint8_t  BreakFlag;                                                         // 40:71
        uint16_t SoftResetFlag;                                                     // 40:72
        uint8_t  HardDiskLastOperationStatus;                                       // 40:74
        uint8_t  AttachedHarddiskCount;                                             // 40:75
        uint8_t  XTFixedDriveControl;                                               // 40:76
        uint8_t  PortOffsetToCurrentFixedDiskAdapter;                               // 40:77
        uint8_t  LptTimeOutValue[4];                                                // 40:78
        uint8_t  ComTimeOutValue[4];                                                // 40:7c
        uint16_t KeyboardBufferStartOffset;                                         // 40:80
        uint16_t KeyboardBufferEndOffset;                                           // 40:82
        RowsOnScreen_t RowsOnScreen;                                                // 40:84
        PointHeightOfCharacterMatrix_t PointHeightOfCharacterMatrix;                // 40:85
        VideoModeOptions_t VideoModeOptions;                                        // 40:87
        EGAFeatureBitSwitches_t EGAFeatureBitSwitches;                              // 40:88
        VideoDisplayDataArea_t VideoDisplayDataArea;                                // 40:89
        DisplayCombinationCodeTableIndex_t DisplayCombinationCodeTableIndex;        // 40:8a
        uint8_t  LastDisketteDataRateSelected;                                      // 40:8b
        uint8_t  HardDiskStatusFromController;                                      // 40:8c
        uint8_t  HardDiskErrorFromController;                                       // 40:8d
        uint8_t  HardDiskInterruptControlFlag;                                      // 40:8e
        uint8_t  HardAndFLoppyComboCard;                                            // 40:8f
        uint8_t  DriveMediaState[4];                                                // 40:90
        uint8_t  DriveCurrentTrack[2];                                              // 40:94
        uint8_t  KeyboardModeAndType;                                               // 40:96
        uint8_t  KeyboardLedFlags;                                                  // 40:97
        uint32_t UserWaitCompleteFlagPointer;                                       // 40:98
        uint32_t UserWaitTimeOutInMicroSeconds;                                     // 40:9c
        uint8_t  RTCWaitFunctionFlag;                                               // 40:a0
        uint8_t  LANADMAChannelFlags;                                               // 40:a1
        uint8_t  LanaStatus[2];                                                     // 40:a2
        uint32_t SavedHardDiskInterruptVector;                                      // 40:a4
        VideoParameterControlBlockPointer_t VideoParameterControlBlockPointer;      // 40:a8
    };
    
    #pragma pack(pop)

    inline Instance& SystemBDA()
    {
        return *FARPointer(0x0000, 0x0400).ToPointer<Instance>();
    }

    Instance& CurrentInstance();

    void SetInstance(Instance& instance);

    inline void CloneSystemBDA(Instance& instance)
    {
        memcpy(&instance, &SystemBDA(), sizeof(Instance));
    }

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
    
    namespace DetectedHardware
    {
        inline DetectedHardware_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().DetectedHardware;
            #else
            return Testing::Mock::BDA::RefAs<DetectedHardware_t, Offset::DetectedHardware>();
            #endif
        }
    }

    namespace DisplayMode
    {
        inline VGA::VideoMode_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().DisplayMode;
            #else
            return Testing::Mock::BDA::RefAs<VGA::VideoMode_t, Offset::DisplayMode>();
            #endif
        }
    }

    namespace NumberOfScreenColumns
    {
        inline NumberOfScreenColumns_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().NumberOfScreenColumns;
            #else
            return Testing::Mock::BDA::RefAs<NumberOfScreenColumns_t, Offset::NumberOfScreenColumns>();
            #endif
        }
    }

    namespace VideoBufferSize
    {
        inline VideoBufferSize_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoBufferSize;
            #else
            return Testing::Mock::BDA::RefAs<VideoBufferSize_t, Offset::VideoBufferSize>();
            #endif
        }
    }

    namespace VideoBufferOffset
    {
        inline VideoBufferOffset_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoBufferOffset;
            #else
            return Testing::Mock::BDA::RefAs<VideoBufferOffset_t, Offset::VideoBufferOffset>();
            #endif
        }
    }

    namespace CursorPositions
    {
        //There are 8 cursor positions.
        inline CursorPosition_t* Get()
        {
            #ifndef MOCK
            return CurrentInstance().CursorPositions;
            #else
            return &Testing::Mock::BDA::RefAs<CursorPosition_t, Offset::CursorPositionPage0>(8);
            #endif
        }
        inline void Clear()
        {
            Get()[0] = 
            Get()[1] = 
            Get()[2] = 
            Get()[3] = 
            Get()[4] = 
            Get()[5] = 
            Get()[6] = 
            Get()[7] = Position();
        }
    }

    namespace CursorScanLines
    {
        inline CursorScanLines_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().CursorScanLines;
            #else
            return Testing::Mock::BDA::RefAs<CursorScanLines_t, Offset::CursorEndScanLine>();
            #endif
        }
    }

    namespace ActiveDisplayNumber
    {
        inline ActiveDisplayNumber_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().ActiveDisplayNumber;
            #else
            return Testing::Mock::BDA::RefAs<ActiveDisplayNumber_t, Offset::ActiveDisplayNumber>();
            #endif
        }
    }
    
    namespace VideoBaseIOPort
    {
        inline VGA::Register_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoBaseIOPort;
            #else
            return Testing::Mock::BDA::RefAs<VGA::Register_t, Offset::VideoBaseIOPort>();
            #endif
        }
    }

    namespace CRTModeControlRegValue
    {
        inline CRTModeControlRegValue_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().CRTModeControlRegValue;
            #else
            return Testing::Mock::BDA::RefAs<CRTModeControlRegValue_t, Offset::CRTModeControlRegValue>();
            #endif
        }
    }
    
    namespace CGAColorPaletteMaskSetting
    {
        inline CGAColorPaletteMaskSetting_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().CGAColorPaletteMaskSetting;
            #else
            return Testing::Mock::BDA::RefAs<CGAColorPaletteMaskSetting_t, Offset::CGAColorPaletteMaskSetting>();
            #endif
        }
    }

    namespace RowsOnScreen
    {
        inline RowsOnScreen_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().RowsOnScreen;
            #else
            return Testing::Mock::BDA::RefAs<RowsOnScreen_t, Offset::RowsOnScreen>();
            #endif
        }
    }

    namespace PointHeightOfCharacterMatrix
    {
        inline PointHeightOfCharacterMatrix_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().PointHeightOfCharacterMatrix;
            #else
            return Testing::Mock::BDA::RefAs<PointHeightOfCharacterMatrix_t, Offset::PointHeightOfCharacterMatrix>();
            #endif
        }
    }

    namespace VideoModeOptions
    {
        inline VideoModeOptions_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoModeOptions;
            #else
            return Testing::Mock::BDA::RefAs<VideoModeOptions_t, Offset::VideoModeOptions>();
            #endif
        }
    }

    namespace EGAFeatureBitSwitches
    {
        inline EGAFeatureBitSwitches_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().EGAFeatureBitSwitches;
            #else
            return Testing::Mock::BDA::RefAs<EGAFeatureBitSwitches_t, Offset::EGAFeatureBitSwitches>();
            #endif
        }
    }

    namespace VideoDisplayDataArea
    {
        inline VideoDisplayDataArea_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoDisplayDataArea;
            #else
            return Testing::Mock::BDA::RefAs<VideoDisplayDataArea_t, Offset::VideoDisplayDataArea>();
            #endif
        }
    }

    namespace DisplayCombinationCodeTableIndex
    {
        inline DisplayCombinationCodeTableIndex_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().DisplayCombinationCodeTableIndex;
            #else
            return Testing::Mock::BDA::RefAs<DisplayCombinationCodeTableIndex_t, Offset::DisplayCombinationCodeTableIndex>();
            #endif
        }
    }

    namespace VideoParameterControlBlockPointer
    {
        inline VideoParameterControlBlockPointer_t& Get()
        {
            #ifndef MOCK
            return CurrentInstance().VideoParameterControlBlockPointer;
            #else
            return Testing::Mock::BDA::RefAs<VideoParameterControlBlockPointer_t, Offset::VideoParameterControlBlockPointer>();
            #endif
        }
    }

}