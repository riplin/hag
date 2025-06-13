;BIOS Data Area

;Memory map: https://wiki.osdev.org/Memory_Map_(x86)
;BIOS Data Area: https://stanislavs.org/helppc/bios_data_area.html
;BIOS Data Area: http://www.bioscentral.com/misc/bda.htm
;BDA/Display combination codes: http://www.mcamafia.de/pdf/ps2bios3.pdf
;Video Information tables: https://stanislavs.org/helppc/video_information.html

BDA_DetectedHardware                    EQU 0410h;Word 0x410
    BDA_DH_InitialVideoModeMask         EQU 30h
    BDA_DH_40x25Color                   EQU 10h
    BDA_DH_80x25Color                   EQU 20h
    BDA_DH_80x25Monochrome              EQU 30h
BDA_DisplayMode                         EQU 0449h;Byte 0x449
    BDA_DM_40x25_BW_Text                EQU 00h
    BDA_DM_40x25_16_Color_Text          EQU 01h
    BDA_DM_80x25_16_Gray_Text           EQU 02h
    BDA_DM_80x25_16_Color_Text          EQU 03h
    BDA_DM_320x200_4_Color_Graphics1    EQU 04h
    BDA_DM_320x200_4_Color_Graphics2    EQU 05h;?
    BDA_DM_640x200_BW_Graphics          EQU 06h
    BDA_DM_80x25_Monochrome_Text        EQU 07h
    BDA_DM_Unknown1                     EQU 08h
    BDA_DM_Unknown2                     EQU 09h
    BDA_DM_Unknown3                     EQU 0Ah
    BDA_DM_Reserved1                    EQU 0Bh
    BDA_DM_Reserved2                    EQU 0Ch
    BDA_DM_320x200_16_Color_Graphics    EQU 0Dh
    BDA_DM_640x200_16_Color_Graphics    EQU 0Eh
    BDA_DM_640x350_Monochrome_Graphics  EQU 0Fh
    BDA_DM_640x350_16_Color_Graphics    EQU 10h
    BDA_DM_640x480_BW_Graphics          EQU 11h
    BDA_DM_640x480_16_Color_Graphics    EQU 12h
    BDA_DM_320x200_256_Color_Graphics   EQU 13h
    BDA_DM_DONT_CLEAR_SCREEN            EQU 80h
BDA_NumberOfScreenColumns               EQU 044Ah;Word 0x44a
BDA_VideoBufferSize                     EQU 044Ch;Word 0x44c
BDA_VideoBufferOffset                   EQU 044Eh;Word 0x44e
DBA_CursorPositionPage0                 EQU 0450h;Word 0x450
DBA_CursorPositionPage1                 EQU 0452h;Word 0x452
DBA_CursorPositionPage2                 EQU 0454h;Word 0x454
DBA_CursorPositionPage3                 EQU 0456h;Word 0x456
DBA_CursorPositionPage4                 EQU 0458h;Word 0x458
DBA_CursorPositionPage5                 EQU 045Ah;Word 0x45a
DBA_CursorPositionPage6                 EQU 045Ch;Word 0x45c
DBA_CursorPositionPage7                 EQU 045Eh;Word 0x45e
BDA_CursorEndStartScanLine              EQU 0460h;Word 0x460
BDA_CursorEndScanLine                   EQU 0460h;Byte 0x460
BDA_CursorStartScanLine                 EQU 0461h;Byte 0x461
BDA_ActiveDisplayNumber                 EQU 0462h;Byte 0x462
BDA_VideoBaseIOPort                     EQU 0463h;Word 0x463
BDA_CRTModeControlRegValue              EQU 0465h;Byte 0x465
    BDA_CMCRV_Mode23Text                EQU 01h
    BDA_CMCRV_Mode45Graphics            EQU 02h
    BDA_CMCRV_Monochrome                EQU 04h
    BDA_CMCRV_VideoEnabled              EQU 08h
    BDA_CMCRV_GraphicsOperation         EQU 10h
    BDA_CMCRV_Blinking                  EQU 20h
BDA_CGAColorPaletteMaskSetting          EQU 0466h;Byte 0x466
BDA_SoftResetFlag                       EQU 0472h;Word 0x472
BDA_SoftResetFlagHighByte               EQU 0473h;Byte 0x473
BDA_RowsOnScreen                        EQU 0484h;Byte 0x484
BDA_PointHeightOfCharacterMatrix        EQU 0485h;Word 0x485
BDA_VideoModeOptions                    EQU 0487h;Byte 0x487
    BDA_VMO_CursorEmulationEnabled      EQU 01h
    BDA_VMO_Monochrome                  EQU 02h
    BDA_VMO_Unknown                     EQU 04h
    BDA_VMO_Inactive                    EQU 08h
    BDA_VMO_Memory64k                   EQU 00h
    BDA_VMO_Memory128k                  EQU 20h
    BDA_VMO_Memory192k                  EQU 40h
    BDA_VMO_Memory256k                  EQU 60h
    BDA_VMO_MemoryMask                  EQU 60h
    BDA_VMO_DontClearDisplay            EQU 80h
BDA_EGAFeatureBitSwitches               EQU 0488h;Byte 0x488
    BDA_EFBS_MDAColor40x25              EQU 00h
    BDA_EFBS_MDAColor80x25              EQU 01h
    BDA_EFBS_MDAHiRes80x25              EQU 02h
    BDA_EFBS_MDAHiResEnhanced           EQU 03h
    BDA_EFBS_CGAMono40x25               EQU 04h
    BDA_EFBS_CGAMono80x25               EQU 05h
    BDA_EFBS_MDAColor40x25_2            EQU 06h
    BDA_EFBS_MDAColor80x25_2            EQU 07h
    BDA_EFBS_MDAHiRes80x25_2            EQU 08h
    BDA_EFBS_MDAHiResEnhanced_2         EQU 09h
    BDA_EFBS_CGAMono40x25_2             EQU 0Ah
    BDA_EFBS_CGAMono80x25_2             EQU 0Bh
    BDA_EFBS_AdapterTypeMask            EQU 0Fh
    BDA_EFBS_FeatureConnector0_0        EQU 10h
    BDA_EFBS_FeatureConnector0_1        EQU 20h
    BDA_EFBS_FeatureConnector1_0        EQU 40h
    BDA_EFBS_FeatureConnector1_1        EQU 80h
    BDA_EFBS_FeatureConnectorMask       EQU 0F0h
BDA_VideoDisplayDataArea                EQU 0489h;Byte 0x489
    BDA_VDDA_VGA                        EQU 01h
    BDA_VDDA_GrayScale                  EQU 02h
    BDA_VDDA_MonochromeMonitor          EQU 04h
    BDA_VDDA_PaletteLoadingEnabled      EQU 08h
    BDA_VDDA_SettingLowerMask           EQU 0Fh
    BDA_VDDA_DisplaySwitchingEnabled    EQU 40h
    BDA_VDDA_LineMode350                EQU 00h
    BDA_VDDA_LineMode400                EQU 10h
    BDA_VDDA_Reserved                   EQU 20h
    BDA_VDDA_LineMode200                EQU 80h
    BDA_VDDA_LineModeMask               EQU 90h
BDA_DisplayCombinationCodeTableIndex    EQU 048ah;Byte 0x48a
BDA_VideoParameterControlBlockPointer   EQU 04a8h;Dword 0x4a8
BDA_VideoParameterControlBlockPtrOfs    EQU 04a8h;word 0x4a8
BDA_VideoParameterControlBlockPtrSeg    EQU 04aah;word 0x4aa
    BDA_VPCB_VideoParameterTable        EQU 00h;dword 0x00
    BDA_VPCB_DynamicParamSaveArea       EQU 04h;dword 0x04
    BDA_VPCB_AlphaNumCharSetOverride    EQU 08h;dword 0x08
        BDA_ACSO_ChardDefLength         EQU 00h;0x00   byte    length of each character definition in bytes
        BDA_ACSO_RamBankIndex           EQU 01h;0x01   byte    character generator RAM bank
        BDA_ACSO_NumCharacters          EQU 02h;0x02   word    count of characters defined
        BDA_ACSO_StartIndex             EQU 04h;0x04   word    first character code in table
        BDA_ACSO_FontData               EQU 06h;0x06   dword   pointer to character font definition table
        BDA_ACSO_CharacterRows          EQU 0ah;0x0A   byte    number of character rows displayed
        BDA_ACSO_ApplicableModeList     EQU 0bh;0x0B   nbytes  array of applicable video modes
                                               ;9x0B+n byte    FFh end of mode list marker
    BDA_VPCB_GrahicsCharSetOverride     EQU 0ch;dword 0x0c
        BDA_GCSO_NumCharacters          EQU 00h;0x00   byte    count of displayed character rows
        BDA_GCSO_CharDefLength          EQU 01h;0x01   word    length of each character definition in bytes
        BDA_GCSO_FontData               EQU 03h;0x03   dword   pointer to character font definition table
        BDA_GCSO_ApplicableModeList     EQU 07h;0x07   nbytes  array of applicable video modes
                                               ;07+n byte    FFh end of mode list marker
    BDA_VPCB_SecondarySavePointerTbl    EQU 10h;dword 0x10
        BDA_SSP_Length                  EQU 00h;0x00  word   length of table in bytes
        BDA_SSP_DisplayCombinationCode  EQU 02h;0x02  dword  pointer to display combination code table
        BDA_SSP_SecondaryAlphaOverr     EQU 06h;0x06  dword  pointer to secondary alphanumeric char set override
            BDA_SACSO_CharDefLength     EQU 00h;0x00   byte    bytes per character
            BDA_SACSO_RamBankIndex      EQU 01h;0x01   byte    block to load
            BDA_SACSO_Reserved2         EQU 02h;0x02   byte    reserved
            BDA_SACSO_FontData          EQU 03h;0x03   dword   font table pointer
            BDA_SACSO_ApplicableModeLst EQU 07h;07   nbytes  array of mode values for this font
                                               ;07+n byte    FFh end of mode list marker

        BDA_SSP_UserPaletteProfileTbl   EQU 0ah;0x0a  dword  pointer to user palette profile table  (VGA)    
    BDA_VPCB_DisplayCombinationCodePtr  EQU 02h;dword 0x02
        BDA_DCC_NoDisplay               EQU 000h;0x00   No display
        BDA_DCC_MDPA                    EQU 001h;0x01   Monochrome display and printer adapter (MDPA)
        BDA_DCC_CGA                     EQU 002h;0x02   Color/graphics monitor adapter (CGA)
        BDA_DCC_EGA                     EQU 004h;0x04   Enhanced Graphics Adapter (EGA)
        BDA_DCC_MEGA                    EQU 005h;0x05   EGA with monochrome display (MEGA)
        BDA_DCC_PGC                     EQU 006h;0x06   Professional graphics controller (PGC)
        BDA_DCC_MVGA                    EQU 007h;0x07   VGA based with monochrome display (MVGA)
        BDA_DCC_CVGA                    EQU 008h;0x08   Video Graphics Array (VGA) based with color display (CVGA)
    BDA_VPCB_SecondaryCharSetOverride   EQU 006h;dword 0x06
    BDA_VPCB_PaletterProfileTable       EQU 00ah;dword 0x0a
        BDA_PPT_Underlining             EQU 000h;0x00 byte    1 - enable underlining in all alphanumeric modes
                                                ;             0 - enable underlining in monochrome alpha modes
                                                ;            -1 - disable underlining in all alpha modes
        BDA_PPT_Reserved1               EQU 001h;0x01 byte   reserved
        BDA_PPT_Reserved2               EQU 002h;0x02 word   reserved
        BDA_PPT_AttributeRegCount       EQU 004h;0x04 word   count of attribute controller regs in table
        BDA_PPT_AttributeRegStartIndex  EQU 006h;0x06 word   first attribute controller register number
        BDA_PPT_AttributeRegsPtr        EQU 008h;0x08 dword  pointer to attribute controller reg table
        BDA_PPT_DACRegCount             EQU 00ch;0x0c word   count of video DAC color registers in table
        BDA_PPT_DACRegStartIndex        EQU 00eh;0x0e word   first video DAC color register number
        BDA_PPT_DacRegsPtr              EQU 010h;0x10 dword  video DAC color register table pointer
        BDA_PPT_ApplicableModeList      EQU 014h;0x14 nbytes array of applicable video modes for this font
                                                ;9x14+n byte FFh end of video mode list marker
BDA_PrintScreenStatus                   EQU 0500h;Byte 0x500
    BDA_PSS_NotActive                   EQU 00h
    BDA_PSS_InProgress                  EQU 01h
    BDA_PSS_Error                       EQU 0FFh

INT_5_Handler                           EQU 0014h   ;Word 0x14
INT_5_HandlerOfs                        EQU 0014h   ;Word 0x14
INT_5_HandlerSeg                        EQU 0016h   ;Word 0x16
INT_10_Handler                          EQU 0040h   ;Word 0x40
INT_10_HandlerOfs                       EQU 0040h   ;Word 0x40
INT_10_HandlerSeg                       EQU 0042h   ;Word 0x42
INT_1D_Handler                          EQU 0074h   ;Word 0x74
INT_1D_HandlerOfs                       EQU 0074h   ;Word 0x74
INT_1D_HandlerSeg                       EQU 0076h   ;Word 0x76
INT_1F_Handler                          EQU 007ch   ;Word 0x7c
INT_1F_HandlerOfs                       EQU 007ch   ;Word 0x7c
INT_1F_HandlerSeg                       EQU 007eh   ;Word 0x7e
INT_42_Handler                          EQU 0108h   ;Word 0x108
INT_42_HandlerOfs                       EQU 0108h   ;Word 0x108
INT_42_HandlerSeg                       EQU 010ah   ;Word 0x10a
INT_43_Handler                          EQU 010ch   ;Word 0x10c
INT_43_HandlerOfs                       EQU 010ch   ;Word 0x10c
INT_43_HandlerSeg                       EQU 010eh   ;Word 0x10e
INT_6D_Handler                          EQU 01b4h   ;Word 0x1b4
INT_6D_HandlerOfs                       EQU 01b4h   ;Word 0x1b4
INT_6D_HandlerSeg                       EQU 01b6h   ;Word 0x1b6
SYS_INT_10h_HandlerOfs                  EQU 0f065h  ;Word 0xf065
SYS_INT_10h_HandlerSeg                  EQU 0f000h  ;Word 0xf000

SYS_Segment                             EQU 0f000h  ;0xf000
SYS_MachineID                           EQU 0fffeh  ;Byte 0xfffe
SYS_MachineID_IBM_AT                    EQU 0fch    ;0xfc
