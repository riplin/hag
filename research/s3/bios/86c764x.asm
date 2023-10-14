;86C764x S3 Trio64 BIOS disassembly

;Resources:
;
;Original BIOS file was downloaded from here: http://chukaev.ru54.com/video_en.htm - 86c764x1.zip
;
;Interrupt list: http://www.ctyme.com/intr/int.htm
;S3 Trio32/Trio64 programming manual: http://www.bitsavers.org/components/s3/DB014-B_Trio32_Trio64_Graphics_Accelerators_Mar1995.pdf
;VGADOC: https://pdos.csail.mit.edu/6.828/2018/readings/hardware/vgadoc/S3.TXT
;Ports list: https://github.com/cirosantilli/ralf-brown-interrupt-list/blob/master/inter61d/PORTS.B
;Memory map: https://wiki.osdev.org/Memory_Map_(x86)
;BIOS Data Area: https://stanislavs.org/helppc/bios_data_area.html
;BIOS Data Area: http://www.bioscentral.com/misc/bda.htm
;Video Information tables: https://stanislavs.org/helppc/video_information.html
;PCI: https://wiki.osdev.org/PCI
;Display Data Channel:https://glenwing.github.io/docs/VESA-EDDC-1.2.pdf
;Display Data Channel:https://glenwing.github.io/docs/VESA-DDCCI-1.1.pdf
;VESA EDID:https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
;VESA VDIF: ???? need help here!
;
;Hex editor: https://hexed.it/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT 86c764x.asm
;Produces a .COM file. Just rename to .BIN

.MODEL tiny
.386
.CODE
ORG 0000h

;PCI vendor and device id.
VENDOR_ID                               EQU 5333h
DEVICE_ID                               EQU 8811h

;Video adapter registers
SetupOptionSelect                       EQU 0102h
CRTControllerIndexB                     EQU 03B4h
CRTControllerDataB                      EQU 03B5h
MDAModeControl                          EQU 03B8h
InputStatus1B                           EQU 03BAh
FeatureControlWB                        EQU 03BAh
AttributeControllerIndex                EQU 03C0h
AttributeControllerDataW                EQU 03C0h
AttributeControllerDataR                EQU 03C1h
InputStatus0                            EQU 03C2h
MiscellaneousWrite                      EQU 03C2h
VideoSubsystemEnable                    EQU 03C3h
SequenceIndex                           EQU 03C4h
SequenceData                            EQU 03C5h
DACMask                                 EQU 03C6h
DACReadIndex                            EQU 03C7h
DACStatus                               EQU 03C7h
DACWriteIndex                           EQU 03C8h
RAMDACData                              EQU 03C9h
FeatureControlR                         EQU 03CAh
MiscellaneousRead                       EQU 03CCh
GraphicsControllerIndex                 EQU 03CEh
GraphicsControllerData                  EQU 03CFh
CRTControllerIndexD                     EQU 03D4h
CRTControllerDataD                      EQU 03D5h
CGAModeControl                          EQU 03D8h
FeatureControlWD                        EQU 03DAh
InputStatus1D                           EQU 03DAh
SubsystemStatus                         EQU 42E8h
SubsystemControl                        EQU 42E8h
VideoSubsystemEnableWO                  EQU 46E8h
AdvancedFunctionControl                 EQU 4AE8h
CurrentYPosition                        EQU 82E8h
CurrentYPosition2                       EQU 82EAh
CurrentXPosition                        EQU 86E8h
CurrentXPosition2                       EQU 86EAh
DestinationYPositionAxialStepConstant   EQU 8AE8h
YCoordinate2AxialStepConstant2          EQU 8AEAh
DestinationXPositionDiagonalStepConstant EQU 8EE8h
XCoordinate2                            EQU 8EEAh
LineErrorTerm                           EQU 92E8h
LineErrorTerm2                          EQU 92EAh
MajorAxisPixelCount                     EQU 96E8h
MajorAxisPixelCount2                    EQU 96EAh
GraphicsProcessorStatus                 EQU 9AE8h
DrawingCommand                          EQU 9AE8h
DrawingCommand2                         EQU 9AEAh
ShortStrokeVectorTransfer               EQU 9EE8h
BackgroundColor                         EQU 0A2E8h
ForegroundColor                         EQU 0A6E8h
BitplaneWriteMask                       EQU 0AAE8h
BitplaneReadMask                        EQU 0AEE8h
ColorCompareRegister                    EQU 0B2E8h
BackgroundMix                           EQU 0B6E8h
ForegroundMix                           EQU 0BAE8h
ReadRegisterData                        EQU 0BEE8h
WriteRegisterData                       EQU 0BEE8h
PixelDataTransfer                       EQU 0E2E8h
PixelDataTransferExtension              EQU 0E2EAh
PatternY                                EQU 0EAE8h
PatternX                                EQU 0EAEAh

;BIOS Data Area
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
    BDA_EFBS_FeatureConnector0          EQU 40h
    BDA_EFBS_FeatureConnector1          EQU 80h
    BDA_EFBS_FeatureConnectorMask       EQU 0F0h
BDA_VideoDisplayDataArea                EQU 0489h;Byte 0x489
    BDA_VDDA_VGA                        EQU 01h
    BDA_VDDA_GrayScale                  EQU 02h
    BDA_VDDA_MonochromeMonitor          EQU 04h
    BDA_VDDA_DefaultPaletteDisabled     EQU 08h
    BDA_VDDA_DisplaySwitchingEnabled    EQU 40h
    BDA_VDDA_LineMode350                EQU 00h
    BDA_VDDA_LineMode400                EQU 10h
    BDA_VDDA_LineMode200                EQU 80h
    BDA_VDDA_LineModeMask               EQU 90h
BDA_DisplayCombinationCodeTableIndex    EQU 048ah;Byte 0x48a
BDA_VideoParameterControlBlockPointer   EQU 04a8h;Dword 0x4a8

INT10_00_SetVideoMode                   EQU 00h;mode in al
;These are currently a best guess based on information from http://www.ctyme.com/intr/rb-0069.htm
;Format:
;INT10_00_ Mode _ Text or Graphics _ Text XxY _ Character XxY _ Resolution XxY _ Number of Colors Mono, Grayscale, Color _ Pages _ Address
    INT10_00_xx_DontClearDisplay        EQU 80h
    INT10_00_00_T_40x25_9x16_360x400_16C_8_B800 EQU 00h
    INT10_00_01_T_40x25_9x16_360x400_16C_8_B800 EQU 01h
    INT10_00_02_T_80x25_9x16_720x400_16C_8_B800 EQU 02h
    INT10_00_03_T_80x25_9x16_720x400_16C_8_B800 EQU 03h
    INT10_00_04_G_40x25_8x8_320x200_4C_x_B800 EQU 04h
    INT10_00_05_G_40x25_8x8_320x200_4C_x_B800 EQU 05h
    INT10_00_06_G_80x25_8x8_640x200_2M_x_B800 EQU 06h
    INT10_00_07_T_80x25_9x16_720x400_M_x_B000 EQU 07h
    INT10_00_08_Unknown                 EQU 08h
    INT10_00_09_Unknown                 EQU 09h
    INT10_00_0A_Unknown                 EQU 0Ah
    INT10_00_0B_Reserved                EQU 0Bh
    INT10_00_0C_Reserved                EQU 0Ch
    INT10_00_0D_G_40x25_8x8_320x200_16C_8_A000 EQU 0Dh
    INT10_00_0E_G_80x25_8x8_640x200_16C_4_A000 EQU 0Eh
    INT10_00_0F_G_80x25_8x14_640x350_M_2_A000 EQU 0Fh
    INT10_00_10_G_80x25_8x14_640x350_4C_2_A000 EQU 10h
    INT10_00_11_G_80x30_8x16_640x480_M_x_A000 EQU 11h
    INT10_00_12_G_80x30_8x16_640x480_16C_x_A000 EQU 12h
    INT10_00_13_G_40x25_8x8_320x200_256C_x_A000 EQU 13h
    ;VESA MODES
    INT10_00_49_640x480x256C EQU 49h ;VESA Mode 0x201
    INT10_00_4A_800x600x16C EQU 4Ah ;VESA Mode 0x202
    INT10_00_4B_800x600x256C EQU 4Bh ;VESA Mode 0x203
    INT10_00_4C_1024x768x16C EQU 4Ch ;VESA Mode 0x204
    INT10_00_4D_1024x768x256C EQU 4Dh ;VESA Mode 0x205
    INT10_00_4E_1152x864x256C EQU 4Eh ;VESA Mode 0x207
    INT10_00_4F_1280x1024x16C EQU 4Fh ;VESA Mode 0x208
    INT10_00_52_640x400x16M EQU 52h ;VESA Mode 0x213
    INT10_00_54_T_132x43 EQU 54h ;VESA Mode 0x10A
    INT10_00_55_T_132x25 EQU 55h ;VESA Mode 0x109
    INT10_00_68_G_640x400x256C EQU 68h ;VESA Mode 0x100
    INT10_00_69_G_640x480x256C EQU 69h ;VESA Mode 0x101
    INT10_00_6A_G_800x600x16C EQU 6Ah ;VESA Mode 0x102
    INT10_00_6B_G_800x600x256C EQU 6Bh ;VESA Mode 0x103
    INT10_00_6C_1024x768x16C EQU 6Ch ;VESA Mode 0x104
    INT10_00_6D_1024x768x256C EQU 6Dh ;VESA Mode 0x105
    INT10_00_6E_1280x1024x16C EQU 6Eh ;VESA Mode 0x106
    INT10_00_6F_1280x1024x256C EQU 6Fh ;VESA Mode 0x107
    INT10_00_70_640x480x32K EQU 70h ;VESA Mode 0x110
    INT10_00_71_640x480x64K EQU 71h ;VESA Mode 0x111
    INT10_00_72_640x480x16M EQU 72h ;VESA Mode 0x112
    INT10_00_73_800x600x32K EQU 73h ;VESA Mode 0x113
    INT10_00_74_800x600x64K EQU 74h ;VESA Mode 0x114
    INT10_00_75_800x600x16M EQU 75h ;VESA Mode 0x115
    INT10_00_76_1024x768x32K EQU 76h ;VESA Mode 0x116
    INT10_00_77_1024x768x64K EQU 77h ;VESA Mode 0x117
    INT10_00_78_1024x768x16M EQU 78h ;VESA Mode 0x118
    INT10_00_79_1280x1024x32K EQU 79h ;VESA Mode 0x119
    INT10_00_7A_1280x1024x64K EQU 7Ah ;VESA Mode 0x11A
    INT10_00_7C_1600x1200x256 EQU 7Ch ;VESA Mode 0x120

INT10_01_SetTextModeCursorShape         EQU 01h
INT10_02_SetCursorPosition              EQU 02h
INT10_03_GetCursorPositionAndSize       EQU 03h
INT10_04_ForwardedToSystemBIOS          EQU 04h
INT10_05_GetCursorAddress               EQU 05h
INT10_06_ScrollUpWindow                 EQU 06h
INT10_07_ScrollDownWindow               EQU 07h
INT10_08_ReadCharAndAttrAtCursorPos     EQU 08h
INT10_09_WriteCharAndAttrAtCursorPos    EQU 09h
INT10_0A_WriteCharOnlyAtCursorPos       EQU 0Ah
INT10_0B_ColorFunctions                 EQU 0Bh;Subfunctions in bh
    INT10_0B_00_SetBGBorderColor        EQU 00h
    INT10_0B_01_SetPalette              EQU 01h
INT10_0C_WriteGraphicsPixel             EQU 0Ch
INT10_0D_ReadGraphicsPixel              EQU 0Dh
INT10_0E_TeletypeOutput                 EQU 0Eh
INT10_0F_GetCurrentVideoMode            EQU 0Fh
INT10_10_DACFunctions                   EQU 10h;Subfunctions in al
    INT10_10_10_SetDACRegister          EQU 10h
    INT10_10_12_SetDACRegisters         EQU 12h
    INT10_10_13_SelectDACColorPage      EQU 13h
    INT10_10_15_ReadDACRegister         EQU 15h
    INT10_10_17_ReadDACRegisters        EQU 17h
    INT10_10_18_SetPELMask              EQU 18h
    INT10_10_19_ReadPELMask             EQU 19h
    INT10_10_1A_GetDACColorPageState    EQU 1Ah
    INT10_10_1B_PerformGrayscaleSumming EQU 1Bh
INT10_11_TextModeCharFunctions          EQU 11h;Subfunctions in al
    INT10_11_00_LoadUserPatterns        EQU 00h
    INT10_11_01_LoadROMMonoPatterns     EQU 01h
    INT10_11_02_LoadROM8x8Patterns      EQU 02h
    INT10_11_03_SetBlockSpecifier       EQU 03h
    INT10_11_04_LoadRom8x16CharSet      EQU 04h
    INT10_11_20_SetUser8x8GfxChars      EQU 20h
    INT10_11_21_SetUserGfxChars         EQU 21h
    INT10_11_22_SetROM8x14GfxChars      EQU 22h
    INT10_11_23_SetROM8x8Chars          EQU 23h
    INT10_11_24_Load8x16GfxChars        EQU 24h
    INT10_11_30_GetFontInformation      EQU 30h
INT10_12_AlternateFunctions             EQU 12h;Subfunctions in bl
    INT10_12_10_GetEGAInfo              EQU 10h
    INT10_12_20_AlternatePRTSC          EQU 20h
    INT10_12_30_SelectVerticalRes       EQU 30h
    INT10_12_31_PaletteLoading          EQU 31h
    INT10_12_32_VideoAddressing         EQU 32h
    INT10_12_33_GrayScaleSumming        EQU 33h
    INT10_12_34_CursorEmulation         EQU 34h
    INT10_12_36_VideoRefreshControl     EQU 36h
INT10_13_WriteString                    EQU 13h
;INT 10h function 14h - 19h not implemented
INT10_1A_DisplayCombinationCodeFuncs    EQU 1Ah;Subfunctions in al
    INT10_1A_00_GetCombinationCode      EQU 00h
    INT10_1A_01_SetCombinationCode      EQU 01h
INT10_1B_FunctionalityAndStateInfo      EQU 1Bh
INT10_1C_SaveRestoreVideoState          EQU 1Ch
INT10_4F_VESASuperVGABIOSFunctions      EQU 4Fh

;   BIOS Flags
;
;   CR41 - Bios Flag register               CR52 - Extended Bios Flag 1 register    CR6B - Extended Bios Flag 3 register    CR6C - Extended Bios Flag 4 register
;   +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+
;   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |       | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |       | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |       | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
;   +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+
;    d2  d1  d0  c2  c1  c0  b2  b1          b0  a2  a1  a0   ?   ?   ?   ?          e7  e6  e5  e4  e3  e2  e1  e0          f7  f6  f5  f4  f3  f2  f1  f0
;
;   a2-0 : Relates to VESAVideoParameters640x480
;   b2-0 : Relates to VESAVideoParameters800x600
;   c2-0 : Relates to VESAVideoParameters1024x768
;   d2-0 : Relates to VESAVideoParameters1280x1024
;   e7-0 : Upper byte of PCI base address 0
;   f7-0 : Following byte of PCI base address 0
;


ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

    jmp short entry

ROMSignature            DB 37h, 34h, 30h, 30h  ;This has to be recalculated if this file is modified.

entry:
    jmp VideoBootstrap

ORG 18h
                        DW PCIHeader
                        DW 0000h               ;PnP header pointer
                        DW 0000h               ;Unknown
                        DB 'IBM VGA Compatible BIOS. ', 00h
                        DB 0BBh, 66h, 0E8h, 01h, 27h, 01h, 11h, 00h, 00h, 0FFh, 00H ;Unknown, Offset 0x38
Data0043                DB 03H
CopyrightString1        DB 'Phoenix S3 TRIO64 Enhanced VGA BIOS. Version 1.5-07 EDO', 0Dh, 0Ah ;Offset 0x44
                        DB 'Copyright 1987-1992 Phoenix Technologies Ltd.', 0Dh, 0Ah, 00h

ORG 0E4h
CopyrightString2        DB 'Copyright 1992-1993 S3 Incorporated', 0Dh, 0Ah
                        DB 'All Rights Reserved', 0DH, 0Ah
                        DB 'PG951215.01   ', 00h

ORG 184H
                        DB '09/27/95'
                        DB '12/14/95'
                        DB 01h                      ;Offset 0x194
                        DB 05h                      ;Offset 0x195
                        DB 07h                      ;Offset 0x196
Data0197                DB 09h                      ;Offset 0x197 - bit 7 - Save / Restore functionality supported
                        DB 02h                      ;Offset 0x198
Data0199                DB 80h                      ;Offset 0x199
PCISystemConfig         DB 0D0h                     ;Offset 0x19a
VLBSystemConfig         DB 0F0h                     ;Offset 0x19b
Data019C                DW 0780h                    ;Offset 0x19c
Data019E                DB 0Ch                      ;Offset 0x19e - This is the size of... something
                        DB 20h                      ;Unknown, Offset 0x19f
                        DB 02h                      ;Unknown, Offset 0x1a0
Data01A1                DW offset VideoOverrideTable1.VideoParameterTable.sgm
                        DW offset VideoOverrideTable1.SecondarySavePointerTable.sgm
                        DW offset SecondarySaveTable1.DisplayCombinationCodeTable.sgm
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DB 20h, 57h, 0FFh, 7Fh, 0A9h, 3Ah, 00h, 00h, 22h, 05h, 00h, 00h                   ;Unknown, Offset 0x1b1
CRTControllerInitPtr    DW offset CRTControllerInitData;Offset 0x1cc
                        DB 00h                      ;Unknown, Offset 0x1c0
                        DB 00h, 00h, 00h, 00h, 00h, 00h, 00h
                        DW 0871h                    ;Offset 0x1c7 Might point to offset 0x871
                        DB 00h
SequenceInitPtr         DW offset SequenceInitData

;CRT Controller data. First byte is count, then X words. Words are divided into low byte = index, high byte is register value.
CRTControllerInitData   DB 1Ah          ; 0x1A = 26 * 2 = 52
                        DW 4838h        ;CR38 - Register Lock 1 - 48h = unlock code
                        DW 0A539h       ;CR39 - Register Lock 2 - A5h = unlock code
                        DW 0531h        ;CR31 - Memory Configuration Register - bit 0 = 1 - Enable Base Address Offset, bit 2 = 1 - Enable 16-bit bus VGA memory read/writes
                        DW 0050h        ;CR50 - Extended System Control 1 - Resets everything to default. See documentation for specifics.
                        DW 0051h        ;CR51 - Extended System Control 2 - Resets everything to default. See documentation for specifics.
                        DW 0053h        ;CR53 - Extended Memory Control 1 - Resets everything to default. See documentation for specifics.
                        DW 3854h        ;CR54 - Extended Memory Control 2 - bits 7-3 = 7 - M parameter
                        DW 0055h        ;CR55 - Extended RAMDAC Control - Resets everything to default. See documentation for specifics.
                        DW 0358h        ;CR58 - Linear Address Window Control - Linear address windows size is set to 4MBytes
                        DW 005Dh        ;CR5D - Extended Horizontal Overflow - Resets everything to default. See documentation for specifics.
                        DW 005Eh        ;CR5E - Extended Vertical Overflow - Resets everything to default. See documentation for specifics.
                        DW 0760h        ;CR60 - Extended Memory Control 3 - value = 7 - Number of 4 or 8 byte fetches written to FIFO minus 1, so total = 8.
                        DW 8061h        ;CR61 - Unknown
                        DW 0A162h       ;CR62 - Unknown
                        DW 0067h        ;CR67 - Extended Miscellaneous Control 2 - Resets everything to default. See documentation for specifics.
                        DW 0069h        ;CR69 - Extended System Control 3 - Resets everything to default. See documentation for specifics.
                        DW 006Ah        ;CR6A - Extended System Control 4 - Resets everything to default. See documentation for specifics.
                        DW 4032h        ;CR32 - Backward Compatibility 1 - bit 6 = 1 - Use Standard VGA Memory Wrapping
                        DW 0033h        ;CR33 - Backward Compatibility 2 - Resets everything to default. See documentation for specifics.
                        DW 0034h        ;CR34 - Backward Compatibility 3 - Resets everything to default. See documentation for specifics.
                        DW 0035h        ;CR35 - CRT Register Lock - Unlock Vertical and Horizontal timing registers
                        DW 053Ah        ;CR3A - Miscellaneous 1 - bit 1-0 = 1 - Refresh Count 1, bit 2 = 1 - Enable Alternate Refresh Count Control
                        DW 5A3Bh        ;CR3B - Start Display FIFO - Fetch = 0x5A = 90
                        DW 103Ch        ;CR3C - Interlace Retrace Start - Start Positoin = 0x10 = 16
                        DW 0043h        ;CR43 - Extended Mode - Resets everything to default. See documentation for specifics.
                        DW 0045h        ;CR45 - Hardware Graphics Cursor Mode - Resets everything to default. See documentation for specifics.

;Sequence Controller Data. First byte is count, then X words. Words are divided into low byte = index, high byte is register value.
;There is some filtering going on in the loop that reads it. See code that references either SequenceInitData directly or SequenceInitPtr.
SequenceInitData        DB 03h
                        DW 0608h        ;SR8 - Unlock Extended Sequencer - 06h = Unlock code.
                        DW 000Bh        ;SRB - Miscellaneous Extended Sequencer - Resets everything to default. See documentation for specifics.
                        DW 0014h        ;SR14 - Clock Synthesis Control 1 - Resets everything to default. See documentation for specifics.

PCIHeader:
                        DB 'PCIR'                   ; PCI header magic
                        DW VENDOR_ID                ; Vendor ID
                        DW DEVICE_ID                ; Device ID
                        DW 00h                      ; Pointer to vital product data (0 = none)
                        DW 0018h                    ; PCI data structure length
                        DB 00h                      ; PCI data structure revision
                        DB 3h, 0h, 0h               ; PCI device class code
                        DW (ROMEnd - ROMStart)/512  ; Rom size in 512 byte blocks
                        DW 0000h                    ; Revision level of code
                        DB 00h                      ; Code type (0 = x86)
                        DB 80h                      ; Last image indicator
                        DW 0000h                    ; Reserved

                        DB 0B5h, 03h, 93h, 03h, 02h, 0FFh, 0FFh ;Unknown, Offset 0x220

;inputs:
;al = ?
;bl = character generator ram bank
Func0x227 PROC NEAR                     ;Offset 0x227
    push ax
    cmp  al, 55h
    jne  Label0x22f                     ;Offset 0x22f
    and  bl, 3fh                        ;if al == 55h, and off the top 2 bits from bl
Label0x22f:                             ;Offset 0x22f
    pop  ax
Func0x227 ENDP
;continue!
EmptyFunction1 PROC NEAR                 ;Offset 0x230
    ret
EmptyFunction1 ENDP

SetTextModeAndCheckMonitor PROC NEAR
    mov  ax, 0003h                      ;ah = 00h - Set Video Mode, al = 03h, 80x25 text mode
    int  10h                            ;Video BIOS interrupt
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    push dx                             ;port - 3?4h
    add  dl, 06h                        ;port - 3?ah
    in   al, dx                         ;port - 3?ah - Input Status 1 (Read)
    mov  dx, AttributeControllerIndex   ;port - 03c0h
    mov  al, 07h                        ;AR7 - Palette index 7
    out  dx, al                         ;
    mov  al, 00h                        ;Set color to black
    out  dx, al                         ;
    pop  dx                             ;Restore port - 3?4h
    mov  al, 11h                        ;CR11 - Vertical Retrace End register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 7fh                        ;And out top bit (LOCK CR0-CR7)
    out  dx, ax                         ;Unlock CRT registers
    mov  ax, 0ff02h                     ;CR2 - Start Horizontal Blank register 
    out  dx, ax                         ;Set to 0xFF
    mov  ax, 0ff15h                     ;CR15 - Start Vertical Blank register
    out  dx, ax                         ;Set to 0xFF
    call ClearPalette                   ;Offset 0x4813
    call EnablePaletteBasedVideo        ;Offset 0x479d
    call MonitorAndChipRevisionCheck    ;Offset 0x263
    ret
SetTextModeAndCheckMonitor ENDP

MonitorAndChipRevisionCheck PROC NEAR   ;Offset 0x263
    xor   cx, cx
    call  Func0x2c9                     ;Offset 0x2c9
    jne   Label0x27c                    ;Offset 0x27c
    mov   cx, 18h
    call  Func0x2c9                     ;Offset 0x2c9
    je    Label0x2bb                    ;Offset 0x2bb
    mov   cx, 1800h
    call  Func0x2c9                     ;Offset 0x2c9
    je    Label0x2a6                    ;Offset 0x2a6
    jmp   Label0x28c                    ;Offset 0x28c
Label0x27c:                             ;Offset 0x27c
    mov   cx, 18h
    call  Func0x2c9                     ;Offset 0x2c9
    jne   Label0x2b9                    ;Offset 0x2b9
    mov   cx, 1800h
    call  Func0x2c9                     ;Offset 0x2c9
    jne   Label0x2a6                    ;Offset 0x2a6
Label0x28c:
    call  ReadDeviceIDAndRevision       ;Offset 0xfc2
    cmp   al, 02h                       ;Test if it's a > 2 revision chip
    jg    Label0x29a                    ;Offset 0x29a
    or    byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0489h
    jmp   Label0x2bb                    ;Offset 0x2bb
Label0x29a:
    or    byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0487h
    or    byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_MonochromeMonitor OR BDA_VDDA_GrayScale;Offset 0489h
    jmp   Label0x2bb                    ;Offset 0x2bb
Label0x2a6:                             ;Offset 0x2a6
    call  ReadDeviceIDAndRevision       ;Offset 0xfc2
    cmp   al, 02h
    jg    Label0x2b4                    ;Offset 0x2b4
    or    byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0489h
    jmp   Label0x2b9                    ;Offset 0x2b9
Label0x2b4:
    or    byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_MonochromeMonitor OR BDA_VDDA_GrayScale;Offset 0489h
Label0x2b9:                             ;Offset 0x2b9
    xor   bx, bx                        ;Clear bx, set flags
Label0x2bb:                             ;Offset 0x2bb
    pushf                               ;Preserve flags
    xor   bx, bx                        ;Color index = 0
    xor   cx, cx                        ;clear green, blue
    xor   dx, dx                        ;clear red (dh)
    mov   ax, 1010h                     ;Set individual DAC register
    int   10h                           ;Video BIOS interrupt
    popf                                ;restore the flags
    ret   
MonitorAndChipRevisionCheck ENDP

;Some form of monitor probing?
;inputs:
;CH = Green
;CL = Blue
;outputs:
;zero flag
Func0x2c9 PROC NEAR                     ;Offset 0x2c9
    push   bx
    xor    bx, bx                       ;Color 0
    xor    dh, dh                       ;Red = 0, Green and Blue in CX
    mov    ax, 1010h                    ;Set Individual DAC register
    int    10h                          ;Video BIOS interrupt
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0463h Contains port - 0x3?4
    add    dl, 06h                      ;port - 0x3?a  - Input Status 1
    xor    cx, cx
Wait1:                                  ;Offset 0x2dc
    in     al, dx
    test   al, 08h                      ;Test Vertical Sync Active
    loope  Wait1                        ;Offset 0x2dc
    je     Failed                       ;Offset 0x2f2
    xor    cx, cx
Wait2:                                  ;Offset 0x2e5
    in     al, dx
    loopne Wait2                        ;Offset 0x2e5
    jne    Failed                       ;Offset 0x2f2
    mov    dx, 03c2h                    ;port 0x3c2 - Input Status 0
    in     al, dx                       ;
    test   al, 10h                      ;Monitor Sense status
    pop    bx                           ;zero flag = result
    ret
Failed:                                 ;Offset 0x2f2
    sub    al, al                       ;zero flag = 0
    pop    bx
    ret
Func0x2c9 ENDP

EnableOver256KAddressingAndSetAddressWindow PROC NEAR
    push ax
    mov  bl, al
    cmp  bl, 13h
    jbe  Label0x307                     ;Offset 0x307
    mov  al, 32h                        ;CR32 - Backward Compatibility 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0bfh                       ;set bit 6 to 0 - Memory accesses extending past a 256K boundary do not wrap
    out  dx, ax
Label0x307:                             ;Offset 0x307
    mov  al, 6bh                        ;CR6B - Extended BIOS Flag 3 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  al, 59h                        ;CR59 - Linear Address Window Position register MSB
    out  dx, ax
    mov  al, 6ch                        ;CR6C - Extended BIOS Flag 4 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 80h                        ;Only keep the top bit
    mov  al, 5ah                        ;CR5A - Linear Address Window Position register LSB
    out  dx, ax
    pop  ax
    ret  
EnableOver256KAddressingAndSetAddressWindow ENDP


;inputs:
;ah = index value 00h to 11h (Where 11h is a special case)
;This function is basically selecting a timing table based on chip revision, then adds the index to the timing table
;and sets up DCLK and perhaps MCLK. After that, based on the index it enables or disables interlacing.
SetupClocks PROC NEAR
    xor  bx, bx
    mov  bl, ah
    and  bl, 1fh
    push ax                             ;Store input
    mov  si, offset ClockData           ;Offset 0393h
    call ReadDeviceIDAndRevision        ;Offset 0xfc2
    cmp  al, 03h                        ;
    jne  NotRev3                        ;Offset 0x331
    mov  si, offset ClockDataRev3       ;Revision 3 uses offset 0x3b7 data instead.
NotRev3:                                ;Offset 0x331
    pop  ax                             ;Restore input
    shl  bl, 01h                        ;2 bytes per entry
    add  si, bx                         ;Add offset to base
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    call ConfigureDCLKAndMCLK           ;Offset 0x343
    and  ah, 20h                        ;If ah == 0x20 Interlaced = on
    mov  al, 42h                        ;CR42 - Mode Control Register
    out  dx, ax
    ret
SetupClocks ENDP

;inputs:
;bl = index
;si = pointer to timing data
;preserves ax and dx
;
;If bl is 22h, it ignores si and stomps it with a different pointer, 
;then it writes the si[1] to MCLK Low and si[0] to MCLK high and sets a hardcoded DCLK. Then it signals that both frequencies were set.
;
;If bl is not 22h, it writes si[1] to DCLK Low and si[0] to DCLK High. Then it signals that only the DCLK frequency should be refreshed.
ConfigureDCLKAndMCLK PROC NEAR
    push  ax
    push  dx
    mov   dx, SequenceIndex             ;port - 03c4h
    mov   ax, 0608h                     ;SR8 - Unlock Extended Sequencer register
    out   dx, ax                        ;Unlock
    mov   ax, 0012h                     ;SR12 - DCLK Value Low register
    cmp   bl, 22h                       ;22h is a special case...
    jne   Label0x364                    ;Offset 0x364
    mov   si, offset Data03b5           ;Offset 03b5h
    mov   ax, 001bh                     ;SR1B - Unknown
    out   dx, ax                        ;reset to 0
    mov   al, 1ah                       ;SR1A - Unknown
    mov   ah, byte ptr cs:[si + 01h]    ;Low value?
    out   dx, ax
    mov   al, 10h                       ;SR10 - MCLK Value Low register
Label0x364:                             ;Offset 0x364
    mov   ah, byte ptr cs:[si + 01h]    ;Low value
    out   dx, ax
    inc   al                            ;SR11 - MCLK Value High register or SR13 - DCLK Value High register
    mov   ah, byte ptr cs:[si]          ;High value
    out   dx, ax
    mov   cl, 02h                       ;2 = Enable new MCLK frequency load
    cmp   bl, 22h
    jne   Label0x380                    ;Offset 0x380
    mov   ax, 4912h                     ;DCLK Value Low register, PLL R = 4, PLL N = 10
    out   dx, ax
    mov   ax, 5513h                     ;DCLK Value High register, M = 86
    out   dx, ax
    mov   cl, 03h                       ;3 = Enable new MCLK and DCLK frequency load
Label0x380:                             ;Offset 0x380
    mov   ax, 0018h                     ;SR18 - RAMDAC/CLKSYN Control register, Reset all flags to default (00h)
    out   dx, ax
    mov   al, 15h                       ;SR15 - CLKSYN Control 2 register
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    and   ah, 0ech                      ;Clear MCLK frequency load, clear DCLK frequency load, don't divide DCLK by 2
    out   dx, ax
    or    ah, cl                        ;cl is either 3 or 2, which means either both MCLK and DCLK frequencies are set, or only MCLK
    out   dx, ax
    pop   dx
    pop   ax
    ret
ConfigureDCLKAndMCLK ENDP

;DCLK High/Low values
ClockData               DB 28h, 61h                 ;00h
                        DB 55h, 49h                 ;01h
                        DB 2Bh, 42h                 ;02h
                        DB 7Ch, 24h                 ;03h
                        DB 1Ah, 40h                 ;04h
                        DB 29h, 22h                 ;05h
                        DB 63h, 48h                 ;06h
                        DB 30h, 42h                 ;07h
                        DB 6Fh, 24h                 ;08h
                        DB 6Bh, 24h                 ;09h
                        DB 2Bh, 22h                 ;0Ah
                        DB 33h, 61h                 ;0Bh
                        DB 2Dh, 21h                 ;0Ch
                        DB 47h, 42h                 ;0Dh
                        DB 13h, 20h                 ;0Eh
                        DB 26h, 21h                 ;0Fh
                        DB 51h, 44h                 ;10h  <- Interlaced

;MCLK High/Low values
Data03b5                DB 79h, 46h

;DCLK High/Low values - revision 03h data
ClockDataRev3           DB 28h, 61h                 ;00h
                        DB 55h, 49h                 ;01h
                        DB 2Bh, 42h                 ;02h
                        DB 7Ch, 24h                 ;03h
                        DB 1Ah, 40h                 ;04h
                        DB 29h, 22h                 ;05h
                        DB 63h, 48h                 ;06h
                        DB 30h, 42h                 ;07h
                        DB 2Dh, 03h                 ;08h  <- Different from other table
                        DB 50h, 07h                 ;09h  <- Different from other table
                        DB 2Bh, 22h                 ;0Ah
                        DB 2Ah, 43h                 ;0Bh  <- Different from other table
                        DB 2Dh, 21h                 ;0Ch
                        DB 23h, 22h                 ;0Dh  <- Different from other table
                        DB 13h, 20h                 ;0Eh
                        DB 26h, 21h                 ;0Fh
                        DB 51h, 44h                 ;10h  <- Interlaced

;inputs:
;dx = CRTC register
;es:di = VESAResolutionVariant
ConfigureExtraVESAModeSettings PROC NEAR;Offset 0x3d9
    push di
    push ds
    mov  bl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push cs
    pop  ds
    lea  si, ds:[VESAModeData]          ;Offset 0x4d82
    call FindModeFromVESAModeData       ;Offset 0x4c87
    mov  si, word ptr ds:[si + 01h]     ;si = pointer to VESAModeInfo
    mov  bx, (VESAModeInfo ptr ds:[si]).BytesPerScanLine;bx = BytesPerScanLine
    cmp  (VESAModeInfo ptr ds:[si]).MemoryModelType, 00h;MemoryModelType = Text
    jne  NotText                        ;Offset 0x3f9
    shl  bx, 01h                        ;Two bytes per scan line for text
    jmp  Not16Color                     ;Offset 0x402
    nop  
NotText:                                ;Offset 0x3f9
    cmp  (VESAModeInfo ptr ds:[si]).MemoryModelType, 03h;16-color (EGA) graphics
    jne  Not16Color                     ;Offset 0x402
    shl  bx, 02h                        ;Four bytes per scan line for 16 color
Not16Color:                             ;Offset 0x402
    shr  bx, 03h                        ;Divide bytes per scan line by 8
    shl  bh, 04h                        ;move bits 3-0 to bits 7-4
    mov  al, 13h                        ;CR13 - Offset register
    mov  ah, bl                         ;
    out  dx, ax                         ;Write lower 8 bits of logical screen width
    mov  al, 51h                        ;CR51 - Extended System Control 2 register
                                        ;bits 5-4 - Logical screen width bits 9-8
    mov  ah, bh
    out  dx, ax                         ;
    mov  ax, 153ah                      ;CR3A - Miscellaneous register
                                        ;bits 1-0 = 01 - Refresh Count 1
                                        ;bit 2 = 1 - Enable Alternate Refresh Count Control
                                        ;bit 4 = 1 - Enable 8 Bits/Pixel or Greater Color Enhanced Mode
    cmp  (VESAModeInfo ptr ds:[si]).BitsPerPixel, 08h;
    jge  Over8bppOrEqual                ;Offset 0x41d
    mov  ah, 05h                        ;bit 4 = 0 - Attribute controller shift registers configured for 4-bit modes
Over8bppOrEqual:                        ;Offset 0x41d
    out  dx, ax
    pop  ds                             ;Restore ds
    pop  di                             ;restore di
    mov  al, 31h                        ;CR31 - Memory Configuration register
    mov  bh, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  bh, INT10_00_4A_800x600x16C    ;0x4a
    je   Is4bppMode                     ;Offset 0x43f
    cmp  bh, INT10_00_4C_1024x768x16C   ;0x4c
    je   Is4bppMode                     ;Offset 0x43f
    cmp  bh, INT10_00_4F_1280x1024x16C  ;0x4f
    je   Is4bppMode                     ;Offset 0x43f
    test ah, 10h                        ;Check if we enabled 8 Bits/Pixel or Greater Color Enhanced Mode
    jne  Not4bppMode                    ;Offset 0x444
    mov  ah, 05h                        ;bit 0 = 1 - Address offset bits 3-0 in CR35 and bits 3-2 in CR51 or the new address offset bits
                                        ;            (5-0 in CR6A) are enabled for specifying the 64KiB page of display memory.
                                        ;            Bits 5-0 of CR6A are used if this field contains a non-zero value. This allows access
                                        ;            up to 4MiB of display memory through a 64KiB window. (2MiB for the Trio32)
                                        ;bit 2 = 1 - Enable 16-bit bus VGA memory read/writes
    jmp  SetCR31                        ;Offset 0x446
    nop
Is4bppMode:                             ;Offset 0x43f
    mov  ah, 0fh                        ;bit 0 = 1 - See above
                                        ;bit 1 = 1 - Enable 2K x 1K x 4 map image screen for 1024 x 768 or 800 x 600 screen resolution,
                                        ;            or 2K x 512 x 8 map image screen for 640 x 480 screen resolution.
                                        ;bit 2 = 1 - See above
                                        ;bit 3 = 1 - Force Enhanced Mode mappings
                                        ;            Setting this bit to 1 overrides the settings of bit 6 of CR14 and bit 3 of CR17 and
                                        ;            causes the use of doubleword memory addressing mode. Also, the function of bits 3-2
                                        ;            of GR6 is overridden with a fixed 64K map at A0000h
    jmp  SetCR31                        ;Offset 0x446
    nop
Not4bppMode:                            ;Offset 0x444
    mov  ah, 09h                        ;bit 0 = 1 - See above
                                        ;bit 3 = 1 - See above
SetCR31:                                ;Offset 0x446
    out  dx, ax                         ;Write out CR31
    mov  al, 50h                        ;CR50 - Extended System Control 1 register
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedSystemControl1
    out  dx, ax
    test ah, 02h                        ;bit 1 - Unknown - Most likely just used as flag here.
    je   DontModifySyncPolarity         ;Offset 0x45e
    push dx
    mov  dx, MiscellaneousRead          ;port - 0x3cc Miscellaneous Output register (read)
    in   al, dx                         ;
    and  al, 3fh                        ;bits 7-6 = 00 - Positive horizontal and vertical sync pulse
    mov  dl, 0c2h                       ;port - 0x3c2 Miscellaneous Output register (write)
    out  dx, al
    pop  dx
DontModifySyncPolarity:                 ;Offset 0x45e
    mov  ax, 1034h                      ;CR34 - Backward Compatibility 3 register
                                        ;bit 4 = 1 - Start Display FIFO Fetch register (CR3B) enabled
    out  dx, ax
    mov  al, 54h                        ;CR54 - Extended Memory Control 2 register
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedMemoryControl2_1MiB
    mov  ch, (VESAResolutionVariant ptr es:[di]).ExtendedMemoryControl3_1MiB
    push dx                             ;store crtc port
    call GetInstalledMemorySizeIn4KBlocks;Offset 0x1457 dh holds value, dl = 0
    mov  bh, dh                         ;store size value in bh
    pop  dx                             ;crtc port restored
    cmp  bh, 01h                        ;1MiB
    je   Is1MiB                         ;Offset 0x480
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedMemoryControl2
    mov  ch, (VESAResolutionVariant ptr es:[di]).ExtendedMemoryControl3
Is1MiB:                                 ;Offset 0x480
    out  dx, ax
    mov  al, 60h                        ;CR60 - Extended Memory Control 3 register
    xchg ch, ah                         ;Store CR54 value in ch
    out  dx, ax                         ;
    mov  al, 5dh                        ;CR5D - Extended Horizontal Overflow register
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedHorizontalOverflow
    out  dx, ax                         ;
    mov  bh, ah                         ;Store CR5D value in bh
    xor  al, al                         ;CR0 - Horizontal Total register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah                         ;Store CR0 in bl
    and  bx, 01ffh                      ;Keep bottom 9 bits
    push bx                             ;Save BX
    shr  bx, 01h                        ;Divide by two (7-0)
    mov  al, 3ch                        ;CR3C - Interlace Retrace Start register
    mov  ah, bl                         ;
    out  dx, ax
    pop  bx                             ;Restore BX (8-0)
    sub  bx, 07h                        ;Subtract 7 
    mov  ah, bl
    mov  al, 3bh                        ;CR3B - Start Display FIFO register
    out  dx, ax
    mov  bh, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov  al, 40h                        ;CR40 - System Configuration register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0feh                       ;bit 0 = 0 - this is a useless operation?
    or   ah, 01h                        ;bit 0 = 1 - Enable enhanced register access (x2E8)
    out  dx, ax
    cmp  bh, INT10_00_55_T_132x25       ;0x55
    je   DontModifyHorizontalSync       ;Offset 0x4fa
    cmp  bh, INT10_00_54_T_132x43       ;0x54
    je   DontModifyHorizontalSync       ;Offset 0x4fa
    mov  al, 3ah                        ;CR3A - Miscellaneous 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    test ah, 10h                        ;bit 4 - Attribute controller shift register configured for 8-, 16- and 24/32-bit color Enhanced modes
    jne  DontModifyHorizontalSync       ;Offset 0x4fa
    mov  al, 11h                        ;CR11 - Vertical Retrace End register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    push ax                             ;Save CR11 index and data
    and  ah, 7fh                        ;bit 7 = 0 - Enable writing to all CRT Controller registers
    out  dx, ax
    mov  al, 04h                        ;CR4 - Start Horizontal Sync Position register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    add  ah, 02h                        ;Increase current position by 2
    out  dx, ax
    inc  al                             ;CR5 - End Horizontal Sync Position
    call ReadDataWithIndexRegister      ;Offset 0x4640
    add  ah, 02h                        ;Increase current position by 2
    out  dx, ax
    cmp  bh, INT10_00_4F_1280x1024x16C  ;0x4f
    jne  DontDecreaseHorizontalBlankRegister;Offset 0x4f8
    mov  al, 02h                        ;CR2 - Start Horizontal Blank register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    dec  ah                             ;Decrease current position by 1
    out  dx, ax
DontDecreaseHorizontalBlankRegister:    ;Offset 0x4f8
    pop  ax                             ;Restore CR11
    out  dx, ax
DontModifyHorizontalSync:               ;Offset 0x4fa
    mov  al, 5eh                        ;CR5E - Extended Vertical Overflow register
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedVerticalOverflow
    out  dx, ax
    mov  al, 67h                        ;CR67 - Extended Miscellaneous Control 2 register
    mov  ah, (VESAResolutionVariant ptr es:[di]).ExtendedMiscellaneousControl2
    out  dx, ax
    cmp  ah, 0d0h                       ;bits 7-4 = 1101 - Color Mode 13: 24-bit color, 1 pixel/VCLK
    jne  Exit                           ;Offset 0x521
    mov  al, 11h                        ;CR11 - Vertical Retrace End register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    push ax                             ;Save CR11 index and data
    and  ah, 7fh                        ;bit 7 = 0 - Writing to CRT Controller registers enabled
    out  dx, ax
    mov  al, 02h                        ;CR2 - Start Horizontal Blanking register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    inc  ah                             ;Increase by 1
    out  dx, ax
    pop  ax                             ;Restore CR11
    out  dx, ax
Exit:                                   ;Offset 0x521
    ret  
ConfigureExtraVESAModeSettings ENDP

VideoModeData STRUCT
    Mode BYTE ?
    OverrideTable WORD ?
    Flags BYTE ?    ;bit 0 - Window granularity and size: 0 = 32k, 1 = 64k
    AdvancedFunction BYTE ?
    ColorMode BYTE ?
    MemoryMode BYTE ?
    Value7 WORD ?
VideoModeData ENDS

;These are all VESA modes.
VESAVideoModeData       VideoModeData < INT10_00_54_T_132x43, offset VESAVideoParameters132x43, 003h, 002h, 000h, 002h, offset ModeData132x43x8 >
                        VideoModeData < INT10_00_55_T_132x25, offset VESAVideoParameters132x25, 033h, 002h, 000h, 002h, offset ModeData132x25x8 >
                        VideoModeData < INT10_00_68_G_640x400x256C, offset VESAVideoParameters640x400, 036h, 003h, 000h, 00Eh, offset ModeData640x400x8 >
                        VideoModeData < INT10_00_69_G_640x480x256C, offset VESAVideoParameters640x480, 036h, 003h, 000h, 00Eh, offset ModeData640x480x8 >
                        VideoModeData < INT10_00_6A_G_800x600x16C, offset VESAVideoParameters800x600, 002h, 006h, 000h, 006h, offset ModeData800x600x4 >
                        VideoModeData < INT10_00_6B_G_800x600x256C, offset VESAVideoParameters800x600, 006h, 003h, 000h, 00Eh, offset ModeData800x600x8 >
                        VideoModeData < INT10_00_6C_1024x768x16C, offset VESAVideoParameters1024x768, 032h, 006h, 000h, 006h, offset ModeData1024x768x4 >
                        VideoModeData < INT10_00_6D_1024x768x256C, offset VESAVideoParameters1024x768, 036h, 007h, 000h, 00Eh, offset ModeData1024x768x8 >
                        VideoModeData < INT10_00_6E_1280x1024x16C, offset VESAVideoParameters1280x1024, 032h, 006h, 000h, 006h, offset ModeData1280x1024x4 >
                        VideoModeData < INT10_00_6F_1280x1024x256C, offset VESAVideoParameters1280x1024, 036h, 003h, 005h, 00Eh, offset ModeData1280x1024x8 >
                        VideoModeData < INT10_00_70_640x480x32K, offset VESAVideoParameters640x480, 036h, 003h, 001h, 00Eh, offset ModeData640x480x16 >
                        VideoModeData < INT10_00_71_640x480x64K, offset VESAVideoParameters640x480, 036h, 003h, 002h, 00Eh, offset ModeData640x480x16 >
                        VideoModeData < INT10_00_72_640x480x16M, offset VESAVideoParameters640x480, 036h, 003h, 004h, 00Eh, offset ModeData640x480x32 >
                        VideoModeData < INT10_00_73_800x600x32K, offset VESAVideoParameters800x600, 006h, 003h, 001h, 00Eh, offset ModeData800x600x16 >
                        VideoModeData < INT10_00_74_800x600x64K, offset VESAVideoParameters800x600, 006h, 003h, 002h, 00Eh, offset ModeData800x600x16 >
                        VideoModeData < INT10_00_75_800x600x16M, offset VESAVideoParameters800x600, 006h, 003h, 004h, 00Eh, offset ModeData800x600x32 >
                        VideoModeData < INT10_00_76_1024x768x32K, offset VESAVideoParameters1024x768, 036h, 003h, 001h, 00Eh, offset ModeData1024x768x16 >
                        VideoModeData < INT10_00_77_1024x768x64K, offset VESAVideoParameters1024x768, 036h, 003h, 002h, 00Eh, offset ModeData1024x768x16 >
                        VideoModeData < INT10_00_78_1024x768x16M, offset VESAVideoParameters1024x768, 036h, 003h, 004h, 00Eh, offset ModeData1024x768x32 >
                        VideoModeData < INT10_00_79_1280x1024x32K, offset VESAVideoParameters1280x1024, 036h, 003h, 001h, 00Eh, offset ModeData1280x1024x16 >
                        VideoModeData < INT10_00_7A_1280x1024x64K, offset VESAVideoParameters1280x1024, 036h, 003h, 002h, 00Eh, offset ModeData1280x1024x16 >
                        VideoModeData < INT10_00_7C_1600x1200x256, offset VESAVideoParameters1600x1200, 036h, 003h, 005h, 00Eh, offset ModeData1600x1200x8 >
                        VideoModeData < INT10_00_49_640x480x256C, offset VESAVideoParameters640x480, 036h, 003h, 000h, 00Eh, offset ModeData640x480x8xOEM >
                        VideoModeData < INT10_00_4A_800x600x16C, offset VESAVideoParameters800x600, 002h, 007h, 000h, 00Eh, offset ModeData800x600x4xOEM >
                        VideoModeData < INT10_00_4B_800x600x256C, offset VESAVideoParameters800x600, 006h, 007h, 000h, 00Eh, offset ModeData800x600x8xOEM >
                        VideoModeData < INT10_00_4C_1024x768x16C, offset VESAVideoParameters1024x768, 032h, 007h, 000h, 00Eh, offset ModeData1024x768x4xOEM >
                        VideoModeData < INT10_00_4D_1024x768x256C, offset VESAVideoParameters1024x768, 036h, 007h, 000h, 00Eh, offset ModeData1024x768x8xOEM >
                        VideoModeData < INT10_00_4E_1152x864x256C, offset VESAVideoParameters1152x864, 036h, 007h, 000h, 00Eh, offset ModeData1152x864x8xOEM >
                        VideoModeData < INT10_00_4F_1280x1024x16C, offset VESAVideoParameters1280x1024, 032h, 007h, 005h, 00Eh, offset ModeData1280x1024x4xOEM >
                        VideoModeData < INT10_00_52_640x400x16M, offset VESAVideoParameters640x400, 036h, 003h, 004h, 00Eh, offset ModeData640x400x32xOEM >
                        DB 0FFh;Terminate

VideoParametersTable STRUCT ;Struct size = 64 bytes
    DisplayedCharacterColumns BYTE ?
    DisplayedRowsMinus1 BYTE ?
    CharacterMatrixHeightPoints BYTE ?
    VideoBufferSize WORD ?
    SequencerRegisters DWORD ?          ;SR1-SR4
    MiscOutputRegisterValues BYTE ?
    CRTCRegisters BYTE 25 DUP (?)       ;CR0-CR18
    AttributeControllerRegs BYTE 20 DUP (?)
    GraphicsControllerRegs BYTE 9 DUP (?) ;GR0-GR8
VideoParametersTable ENDS

VESAVideoParameters132x43 VideoParametersTable <>
VESAVideoParameters132x25 VideoParametersTable <>
VESAVideoParameters640x400 VideoParametersTable <>
VESAVideoParameters640x480 VideoParametersTable <>
VESAVideoParameters800x600 VideoParametersTable <>
VESAVideoParameters1024x768 VideoParametersTable <>
VESAVideoParameters1280x1024 VideoParametersTable <>
VESAVideoParameters1152x864 VideoParametersTable <>
VESAVideoParameters1600x1200 VideoParametersTable <>

ORG offset VESAVideoParameters132x43
                        DB 084h, 02Ah, 008h, 000h, 040h, 001h, 003h, 000h, 002h, 06Fh
Data063B                DB 09Ah, 083h, 084h, 09Ch, 088h, 01Ah, 0BFh, 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 083h, 085h, 058h, 042h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 090h, 00Eh, 000h, 0FFh

ORG offset VESAVideoParameters132x25
                        DB 084h, 018h, 010h, 000h, 020h, 001h, 003h, 000h, 002h, 06Fh
Data067B                DB 09Ah, 083h, 084h, 09Ch, 088h, 01Ah, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 042h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 090h, 00Eh, 000h, 0FFh

ORG offset VESAVideoParameters640x400
                        DB 050h, 018h, 010h, 000h, 010h, 001h, 00Fh, 000h, 00Eh, 0EFh
Data06BB                DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 0FFh, 000h, 09Ch, 08Eh, 08Fh, 050h, 040h, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters640x480
                        DB 050h, 01Dh, 010h, 000h, 010h, 001h, 00Fh, 000h, 00Eh, 0EFh
Data06FB                DB 05Fh, 04Fh, 050h, 002h, 053h, 09Fh, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0EAh, 08Ch, 0DFh, 080h, 060h, 0E7h, 004h, 0ABh, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters800x600
                        DB 064h, 04Ah, 008h, 000h, 0FAh, 001h, 00Fh, 000h, 00Eh, 0EFh
Data073B                DB 07Bh, 063h, 063h, 080h, 067h, 010h, 06Fh, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 0FFh, 000h, 058h, 08Ah, 057h, 080h, 000h, 057h, 000h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters1024x768
                        DB 080h, 02Fh, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 006h, 0EFh
Data077B                DB 09Ah, 07Fh, 07Fh, 09Dh, 081h, 017h, 097h, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 0FFh, 000h, 080h, 084h, 07Fh, 080h, 000h, 080h, 000h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters1280x1024
                        DB 0A0h, 03Fh, 010h, 000h, 001h, 001h, 00Fh, 000h, 006h, 0EFh
Data07BB                DB 0C0h, 09Fh, 0A0h, 083h, 0A4h, 019h, 018h, 0B2h, 000h, 060h, 000h, 000h, 000h, 000h, 0FFh, 000h, 001h, 085h, 0FFh, 080h, 000h, 000h, 018h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters1152x864
                        DB 090h, 036h, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 006h, 0EFh
Data07FB                DB 0B1h, 08Fh, 090h, 014h, 093h, 09Fh, 09Ah, 0FFh, 000h, 060h, 000h, 000h, 000h, 000h, 0FFh, 000h, 070h, 083h, 05Fh, 090h, 060h, 06Fh, 08Dh, 0EBh, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

ORG offset VESAVideoParameters1600x1200
                        DB 0C8h, 04Bh, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 006h, 02Fh
Data083B                DB 001h, 0C7h, 0C8h, 081h, 0D3h, 013h, 074h, 0E0h, 000h, 060h, 000h, 000h, 000h, 000h, 0FFh, 000h, 058h, 000h, 057h, 064h, 000h, 058h, 000h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x871
                        DB 0FFh
                        DB 042h, 050h, 054h, 054h, 060h, 060h, 05Dh, 05Eh, 067h, 0FFh

;00 pointer to CRTC registers
;02 upper bit is terminate flag, lower 3 bits are a filter. if it's higher than a 3 bit value from the Bios flags register, it's rejected.
;03 bits 4-0 = Clock configuration index. 0x11 is a special case value.
;04 CR50 - Extended System Control 1 register. bit 1 is used to indicate that hsync and vsync are positive sync (MiscellaneousRead/Write)
;05 CR54 - Extended Memory Control 2 register value for 1MiB size.
;06 CR54 - Extended Memory Control 2 register value for all other memory sizes.
;07 CR60 - Extended Memory Control 3 register value for 1MiB size.
;08 CR60 - Extended Memory Control 3 register value for all other memory sizes.
;09 CR5D - Extended Horizontal Overflow register
;0A CR5E - Extended Vertical Overflow register
;0B CR67 - Extended Miscellaneous Control 2 register

VESAResolutionVariant STRUCT
    CRTCRegisters DW ?                  ;0x00
    FlagsAndFilter DB ?                 ;0x02
    ClockConfigIndex DB ?               ;0x03
    ExtendedSystemControl1 DB ?         ;0x04
    ExtendedMemoryControl2_1MiB DB ?    ;0x05
    ExtendedMemoryControl2 DB ?         ;0x06
    ExtendedMemoryControl3_1MiB DB ?    ;0x07
    ExtendedMemoryControl3 DB ?         ;0x08
    ExtendedHorizontalOverflow DB ?     ;0x09
    ExtendedVerticalOverflow DB ?       ;0x0A
    ExtendedMiscellaneousControl2 DB ?  ;0x0B
VESAResolutionVariant ENDS

;The code searches backwards through this data for every resolution/bit depth. So keep the data sitting above the labled ones together.

ModeData132x43x8        VESAResolutionVariant <offset Data063B, 080h, 002h, 000h, 008h, 008h, 038h, 038h, 000h, 000h, 000h> ;Offset 0x87c

ModeData132x25x8        VESAResolutionVariant <offset Data067B, 080h, 002h, 000h, 008h, 008h, 038h, 038h, 000h, 000h, 000h> ;Offset 0x888

ModeData640x400x8       VESAResolutionVariant <offset Data06BB, 080h, 000h, 040h, 0F8h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x894

                        VESAResolutionVariant <offset Data06FB, 080h, 000h, 040h, 088h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x8a0
                        VESAResolutionVariant <offset Data0BD0, 001h, 00Bh, 040h, 088h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x8ac
ModeData640x480x8       VESAResolutionVariant <offset Data0BE9, 002h, 00Bh, 040h, 088h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x8b8

                        VESAResolutionVariant <offset Data073B, 080h, 006h, 000h, 080h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x8c4
                        VESAResolutionVariant <offset Data0C02, 001h, 002h, 002h, 070h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x8d0
                        VESAResolutionVariant <offset Data0C1B, 002h, 004h, 002h, 048h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x8dc
ModeData800x600x4       VESAResolutionVariant <offset Data0C34, 003h, 010h, 002h, 048h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x8e8

                        VESAResolutionVariant <offset Data073B, 080h, 006h, 080h, 080h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x8f4
                        VESAResolutionVariant <offset Data0C02, 001h, 002h, 082h, 070h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x900
                        VESAResolutionVariant <offset Data0C1B, 002h, 004h, 082h, 048h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x90c 
ModeData800x600x8       VESAResolutionVariant <offset Data0C34, 003h, 010h, 082h, 048h, 0F8h, 0FFh, 0FFh, 008h, 000h, 000h> ;Offset 0x918

                        VESAResolutionVariant <offset Data077B, 080h, 027h, 000h, 060h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x924
                        VESAResolutionVariant <offset Data0C4D, 002h, 00Dh, 000h, 030h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x930
                        VESAResolutionVariant <offset Data0C66, 003h, 00Eh, 000h, 010h, 0C8h, 0FFh, 0FFh, 000h, 000h, 001h> ;Offset 0x93c
ModeData1024x768x4      VESAResolutionVariant <offset Data0C7F, 004h, 00Ah, 002h, 010h, 0C0h, 0FFh, 0FFh, 000h, 000h, 001h> ;Offset 0x948

                        VESAResolutionVariant <offset Data077B, 080h, 027h, 000h, 060h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x954
                        VESAResolutionVariant <offset Data0C4D, 002h, 00Dh, 000h, 030h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x960
                        VESAResolutionVariant <offset Data0C66, 003h, 00Eh, 000h, 010h, 048h, 0FFh, 048h, 000h, 000h, 001h> ;Offset 0x96c
ModeData1024x768x8      VESAResolutionVariant <offset Data0C7F, 004h, 00Ah, 002h, 010h, 048h, 0FFh, 0FFh, 000h, 000h, 001h> ;Offset 0x978

ModeData1280x1024x4     VESAResolutionVariant <offset Data07BB, 082h, 02Eh, 000h, 038h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0x984

                        VESAResolutionVariant <offset Data07BB, 082h, 02Eh, 0C0h, 050h, 050h, 0FFh, 0FFh, 000h, 000h, 001h> ;Offset 0x990
                        VESAResolutionVariant <offset Data0C98, 004h, 00Ch, 0C0h, 028h, 028h, 0FFh, 0FFh, 000h, 055h, 010h> ;Offset 0x99c
                        VESAResolutionVariant <offset Data0CB1, 005h, 009h, 0C0h, 020h, 020h, 0FFh, 0FFh, 000h, 055h, 010h> ;Offset 0x9a8
ModeData1280x1024x8     VESAResolutionVariant <offset Data0CCA, 006h, 008h, 0C2h, 018h, 018h, 0FFh, 0FFh, 000h, 015h, 010h> ;Offset 0x9b4

                        VESAResolutionVariant <offset Data0CE3, 080h, 000h, 050h, 030h, 058h, 0FFh, 0FFh, 000h, 000h, 050h> ;Offset 0x9c0
                        VESAResolutionVariant <offset Data0CFC, 001h, 00Bh, 050h, 030h, 0F8h, 0FFh, 0FFh, 000h, 000h, 050h> ;Offset 0x9cc
ModeData640x480x16      VESAResolutionVariant <offset Data0D15, 002h, 00Bh, 050h, 028h, 0F8h, 0FFh, 0FFh, 000h, 000h, 050h> ;Offset 0x9d8

                        VESAResolutionVariant <offset Data06FB, 080h, 000h, 070h, 038h, 038h, 01Ch, 01Ch, 000h, 000h, 0D0h> ;Offset 0x9e4
                        VESAResolutionVariant <offset Data0BD0, 001h, 00Bh, 070h, 030h, 030h, 019h, 019h, 000h, 000h, 0D0h> ;Offset 0x9f0
ModeData640x480x32      VESAResolutionVariant <offset Data0BE9, 002h, 00Bh, 070h, 030h, 030h, 0FFh, 0FFh, 000h, 000h, 0D0h> ;Offset 0x9fc

                        VESAResolutionVariant <offset Data0D2E, 081h, 002h, 092h, 018h, 048h, 0FFh, 0FFh, 001h, 000h, 050h> ;Offset 0xa08
                        VESAResolutionVariant <offset Data0D47, 002h, 004h, 092h, 000h, 038h, 0FFh, 0FFh, 001h, 000h, 050h> ;Offset 0xa14
ModeData800x600x16      VESAResolutionVariant <offset Data0D60, 003h, 010h, 092h, 000h, 038h, 0FFh, 0FFh, 001h, 000h, 050h> ;Offset 0xa20

                        VESAResolutionVariant <offset Data0C02, 081h, 002h, 0B2h, 010h, 010h, 0FFh, 0FFh, 008h, 000h, 0D0h> ;Offset 0xa2c
                        VESAResolutionVariant <offset Data0C1B, 002h, 004h, 0B2h, 000h, 000h, 0FFh, 0FFh, 008h, 000h, 0D0h> ;Offset 0xa38
ModeData800x600x32      VESAResolutionVariant <offset Data0C34, 003h, 010h, 0B2h, 000h, 000h, 0FFh, 0FFh, 008h, 000h, 0D0h> ;Offset 0xa44

                        VESAResolutionVariant <offset Data0D79, 080h, 027h, 010h, 028h, 028h, 0FFh, 0FFh, 075h, 000h, 050h> ;Offset 0xa50
                        VESAResolutionVariant <offset Data0D92, 002h, 00Dh, 010h, 018h, 018h, 0FFh, 0FFh, 07Dh, 000h, 050h> ;Offset 0xa5c
                        VESAResolutionVariant <offset Data0DAB, 003h, 00Eh, 010h, 010h, 010h, 0FFh, 0FFh, 07Dh, 000h, 050h> ;Offset 0xa68
ModeData1024x768x16     VESAResolutionVariant <offset Data0DC4, 004h, 00Ah, 012h, 010h, 010h, 0FFh, 0FFh, 05Dh, 000h, 050h> ;Offset 0xa74

ModeData1024x768x32     VESAResolutionVariant <offset Data077B, 080h, 027h, 030h, 038h, 038h, 0FFh, 0FFh, 000h, 000h, 0D0h> ;Offset 0xa80

ModeData1280x1024x16    VESAResolutionVariant <offset Data0DDD, 082h, 02Ah, 0D0h, 010h, 010h, 0FFh, 0FFh, 07Fh, 000h, 050h> ;Offset 0xa8c

ModeData1600x1200x8     VESAResolutionVariant <offset Data083B, 080h, 029h, 081h, 000h, 000h, 0FFh, 0FFh, 001h, 000h, 010h> ;Offset 0xa98

                        VESAResolutionVariant <offset Data06FB, 080h, 000h, 000h, 088h, 088h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xaa4
                        VESAResolutionVariant <offset Data0BD0, 001h, 00Bh, 000h, 088h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xab0
ModeData640x480x8xOEM   VESAResolutionVariant <offset Data0BE9, 002h, 00Bh, 000h, 088h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xabc

                        VESAResolutionVariant <offset Data073B, 080h, 006h, 000h, 080h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xac8
                        VESAResolutionVariant <offset Data0C02, 001h, 002h, 002h, 070h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xad4
                        VESAResolutionVariant <offset Data0C1B, 002h, 004h, 002h, 048h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xae0
ModeData800x600x4xOEM   VESAResolutionVariant <offset Data0C34, 003h, 010h, 002h, 048h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xaec

                        VESAResolutionVariant <offset Data073B, 080h, 006h, 000h, 080h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xaf8
                        VESAResolutionVariant <offset Data0C02, 001h, 002h, 000h, 070h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb04
                        VESAResolutionVariant <offset Data0C1B, 002h, 004h, 000h, 048h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb10
ModeData800x600x8xOEM   VESAResolutionVariant <offset Data0C34, 003h, 010h, 000h, 048h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb1c

                        VESAResolutionVariant <offset Data077B, 080h, 027h, 000h, 060h, 0C0h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb28
                        VESAResolutionVariant <offset Data0C4D, 002h, 00Dh, 000h, 030h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb34
                        VESAResolutionVariant <offset Data0C66, 003h, 00Eh, 000h, 010h, 0C8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb40
ModeData1024x768x4xOEM  VESAResolutionVariant <offset Data0C7F, 004h, 00Ah, 002h, 010h, 0C0h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb4c

                        VESAResolutionVariant <offset Data077B, 080h, 027h, 000h, 060h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb58
                        VESAResolutionVariant <offset Data0C4D, 002h, 00Dh, 000h, 030h, 0F8h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb64
                        VESAResolutionVariant <offset Data0C66, 003h, 00Eh, 000h, 010h, 048h, 0FFh, 048h, 000h, 000h, 000h> ;Offset 0xb70
ModeData1024x768x8xOEM  VESAResolutionVariant <offset Data0C7F, 004h, 00Ah, 002h, 010h, 048h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb7c

ModeData1152x864x8xOEM  VESAResolutionVariant <offset Data07FB, 084h, 00Ah, 001h, 010h, 040h, 0FFh, 0FFh, 000h, 000h, 000h> ;Offset 0xb88

                        VESAResolutionVariant <offset Data07BB, 082h, 02Eh, 000h, 038h, 0F8h, 0FFh, 0FFh, 000h, 000h, 001h> ;Offset 0xb94
                        VESAResolutionVariant <offset Data0C98, 004h, 00Ch, 000h, 018h, 0C0h, 0FFh, 0FFh, 000h, 055h, 010h> ;Offset 0xba0
                        VESAResolutionVariant <offset Data0CB1, 045h, 009h, 000h, 018h, 080h, 0FFh, 0FFh, 000h, 055h, 010h> ;Offset 0xbac
ModeData1280x1024x4xOEM VESAResolutionVariant <offset Data0CCA, 046h, 008h, 002h, 018h, 080h, 0FFh, 0FFh, 000h, 015h, 010h> ;Offset 0xbb8

ModeData640x400x32xOEM  VESAResolutionVariant <Offset Data06BB, 081h, 000h, 070h, 008h, 008h, 0FFh, 0FFh, 000h, 000h, 0D0h> ;Offset 0xbc4

;-------------------CRT Controller data
Data0BD0                DB 063h, 04Fh, 04Fh, 000h, 053h, 018h, 006h, 03Eh, 000h, 040h, 000h, 000h ;Offset 0xbd0
                        DB 000h, 000h, 0FFh, 000h, 0E8h, 08Bh, 0DFh, 080h, 060h, 0DFh, 006h, 0ABh
                        DB 0FFh 
Data0BE9                DB 064h, 04Fh, 04Fh, 000h, 052h, 01Ah, 0F2h, 01Fh, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 0E0h, 083h, 0DFh, 080h, 060h, 0DFh, 000h, 0ABh
                        DB 0FFh
Data0C02                DB 07Fh, 063h, 063h, 080h, 069h, 019h, 072h, 0F0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 058h, 08Ch, 057h, 080h, 000h, 057h, 000h, 0E3h
                        DB 0FFh
Data0C1B                DB 07Dh, 063h, 063h, 080h, 06Bh, 01Ah, 099h, 0F0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 07Ch, 0A2h, 057h, 080h, 000h, 057h, 099h, 0E3h
                        DB 0FFh
Data0C34                DB 07Fh, 063h, 063h, 000h, 066h, 010h, 06Fh, 0E0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 058h, 08Bh, 057h, 080h, 000h, 057h, 000h, 0E3h
                        DB 0FFh
Data0C4D                DB 0A4h, 07Fh, 07Fh, 007h, 083h, 095h, 025h, 0F5h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 002h, 088h, 0FFh, 080h, 000h, 0FFh, 025h, 0E3h
                        DB 0FFh
Data0C66                DB 0A2h, 07Fh, 07Fh, 085h, 083h, 094h, 025h, 0F5h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 002h, 088h, 0FFh, 080h, 000h, 0FFh, 025h, 0E3h
                        DB 0FFh
Data0C7F                DB 0A2h, 07Fh, 07Fh, 083h, 082h, 08Eh, 01Fh, 0FDh, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 001h, 084h, 0FFh, 080h, 000h, 000h, 000h, 0EBh
                        DB 0FFh
Data0C98                DB 0D7h, 09Fh, 0A0h, 01Ah, 0A5h, 01Dh, 01Eh, 052h, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 003h, 086h, 0FFh, 080h, 000h, 000h, 000h, 0E3h
                        DB 0FFh
Data0CB1                DB 0CCh, 09Fh, 0A0h, 08Eh, 0A5h, 019h, 033h, 052h, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 005h, 08Ch, 0FFh, 080h, 000h, 000h, 031h, 0E3h
                        DB 0FFh
Data0CCA                DB 0CEh, 09Fh, 0A0h, 011h, 0A3h, 015h, 029h, 052h, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 001h, 084h, 0FFh, 080h, 000h, 000h, 000h, 0E3h
                        DB 0FFh
Data0CE3                DB 0C2h, 09Fh, 0A0h, 084h, 0A3h, 01Bh, 00Ch, 03Eh, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 0E9h, 08Bh, 0DFh, 000h, 060h, 0E7h, 004h, 0ABh
                        DB 0FFh
Data0CFC                DB 0CCh, 09Fh, 0A0h, 00Fh, 0A7h, 011h, 006h, 03Eh, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 0E8h, 08Bh, 0DFh, 080h, 060h, 0DFh, 006h, 0ABh
                        DB 0FFh
Data0D15                DB 0CFh, 09Fh, 0A0h, 012h, 0A5h, 015h, 0F1h, 01Fh, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 0E0h, 083h, 0DFh, 080h, 060h, 0DFh, 000h, 0ABh
                        DB 0FFh
Data0D2E                DB 003h, 0C7h, 0C8h, 006h, 0D3h, 013h, 072h, 0F0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 058h, 08Ch, 057h, 064h, 000h, 057h, 000h, 0A3h
                        DB 0FFh
Data0D47                DB 000h, 0C7h, 0C8h, 003h, 0D7h, 015h, 099h, 0F0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 07Ch, 082h, 057h, 064h, 000h, 057h, 099h, 0A3h
                        DB 0FFh
Data0D60                DB 004h, 0C7h, 0C8h, 007h, 0CDh, 001h, 06Fh, 0F0h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 058h, 08Bh, 057h, 080h, 000h, 057h, 000h, 0E3h
                        DB 0FFh
Data0D79                DB 038h, 0FFh, 000h, 000h, 003h, 00Fh, 097h, 01Fh, 000h, 040h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 080h, 084h, 07Fh, 000h, 000h, 080h, 000h, 0A3h
                        DB 0FFh
Data0D92                DB 04Dh, 0FFh, 000h, 00Dh, 007h, 009h, 025h, 0F5h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 002h, 088h, 0FFh, 080h, 000h, 0FFh, 025h, 0E3h
                        DB 0FFh
Data0DAB                DB 049h, 0FFh, 000h, 009h, 007h, 009h, 01Fh, 0F5h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 002h, 088h, 0FFh, 080h, 000h, 0FFh, 01Fh, 0E3h
                        DB 0FFh
Data0DC4                DB 048h, 0FFh, 000h, 008h, 005h, 01Dh, 01Fh, 0FDh, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 001h, 084h, 0FFh, 080h, 000h, 000h, 000h, 0EBh
                        DB 0FFh
Data0DDD                DB 0A1h, 03Fh, 040h, 00Dh, 049h, 003h, 018h, 0B2h, 000h, 060h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 001h, 085h, 0FFh, 080h, 000h, 000h, 018h, 0E3h
                        DB 0FFh
;inputs:
;al = video mode
SetColorMode PROC NEAR                  ;Offset 0xdf6
    push dx
    push bx
    call FindVideoModeData              ;Offset 0x103a
    cmp  al, 13h
    jbe  LegacyMode                     ;Offset 0xe18
    mov  al, (VideoModeData ptr es:[di + offset VESAVideoModeData]).ColorMode;Offset 0x527 - Translate video mode
    cmp  al, 05h
    je   ColorMode8                     ;Offset 0xe31
    cmp  al, 04h
    je   ColorMode13                    ;Offset 0xe1d
    cmp  al, 03h
    je   ColorMode12                    ;Offset 0xe22
    cmp  al, 02h
    je   ColorMode10                    ;Offset 0xe27
    cmp  al, 01h
    je   ColorMode9                     ;Offset 0xe2c
LegacyMode:                             ;Offset 0xe18
    xor  cx, cx                         ;Color Mode 0000 - Mode 0: 8-bit color, 1 pixel/VCLK
    jmp  ConfigureClocks                ;Offset 0xe33
    nop
ColorMode13:                            ;Offset 0xe1d
    mov  cl, 0d0h                       ;Color Mode 1101 - Mode 13: 24-bit color, 1 pixel/VCLK
    jmp  ConfigureClocks                ;Offset 0xe33
    nop
ColorMode12:                            ;Offset 0xe22
    mov  cl, 70h                        ;Color Mode 0111 - Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Trio 32 only)
    jmp  ConfigureClocks                ;Offset 0xe33
    nop
ColorMode10:                            ;Offset 0xe27
    mov  cl, 50h                        ;Color Mode 0101 - Mode 10: 16-bit color, 1 pixel/VCLK
    jmp  ConfigureClocks                ;Offset 0xe33
    nop
ColorMode9:                             ;Offset 0xe2c
    mov  cl, 30h                        ;Color Mode 0011 - Mode 9: 15-bit color, 1 pixel/VCLK
    jmp  ConfigureClocks                ;Offset 0xe33
    nop
ColorMode8:                             ;Offset 0xe31
    mov  cl, 10h                        ;Color Mode 0001 - Mode 8: 8-bit color, 2 pixels/VCLK
ConfigureClocks:                        ;Offset 0xe33
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  ax, 0608h                      ;SR8 - Unlock Extended Sequencer register 0x06 = unlock code.
    out  dx, ax
    cmp  cl, 10h                        ;Color Mode 0001 - Mode 8: 8-bit color, 2 pixels/VCLK
    jne  NotMode8                       ;Offset 0xe4f
    mov  ax, 0c018h                     ;SR18 - RAMDAC/CLKSYN Control register
                                        ;bit 6 = 1 - DCLK LUT write cycle
                                        ;but 7 = 1 - RAMDAC clock doubled mode (0001) enabled
    out  dx, ax
    mov  al, 15h                        ;SR15 - CLKSYN Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 10h                        ;bit 4 = 1 - Divide DCLK by 2
    out  dx, ax
    jmp  ApplyColorMode                 ;Offset 0xe5c
    nop
NotMode8:                               ;Offset 0xe4f
    mov  ax, 4018h                      ;SR18 - RAMDAC/CLKSYN Control register
                                        ;bit 6 - Invert DCLK - Either this bit or bit 4 of this register
                                        ;                      must be set to 1 for clock doubled RAMDAC
                                        ;                      Operation (mode 0001).
    out  dx, ax
    mov  al, 15h                        ;SR15 - CLKSYN Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0afh                       ;bit 6 = 0 - DCLK unchanged (not inverted)
                                        ;bit 4 = 0 - DCLK unchanged (not divided by 2)
    out  dx, ax
ApplyColorMode:                         ;Offset 0xe5c
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 67h                        ;CR67 - Extended Miscellaneous Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fh                        ;Clear out top 4 bits - Color Mode
    or   ah, cl                         ;Set Color Mode
    out  dx, ax
    pop  bx
    pop  dx
    ret
SetColorMode ENDP

InitCRTControllerAndSequencer PROC NEAR
    push si
    call EmptyFunction1                 ;Offset 0x230
    mov  si, word ptr cs:[CRTControllerInitPtr];Offset 0x1bd
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    call InitializeCRTControllerAndSequencer;Offset 0xfee
    pop  si
    ret
InitCRTControllerAndSequencer ENDP

;inputs:
;dx = CRT Control Index register
;outputs:
;bh = bus specific settings for CR40 - System Configuration
FetchBusSpecificSystemConfig PROC NEAR  ;Offset 0xe7e
    mov  al, 36h                        ;CR36 - Configuration 1
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 03h                        ;Isolate bits 1-0 - System Bus Select
    cmp  ah, 02h                        ;01h = VLB, 02h = PCI
    je   IsPCI                          ;Offset 0xe91
    mov  bh, byte ptr cs:[VLBSystemConfig];Offset 0x19b
    ret
IsPCI:                                  ;Offset 0xe91
    mov  bh, byte ptr cs:[PCISystemConfig];Offset 0x19a
    ret
FetchBusSpecificSystemConfig ENDP

ClearBIOSFlags PROC NEAR                ;Offset 0xe97
    call      UnlockExtendedCRTRegisters;Offsset 0xfa6
    call      GetCRTControllerIndexRegister;Offset 0xfdd
    mov       ax, 0041h                 ;CR41 - BIOS Flag register
    out       dx, ax                    ;00h - clear BIOS flags
    mov       ax, 0052h                 ;CR52 - Extended BIOS Flag 1 register
    out       dx, ax                    ;00h - clear extended BIOS flags
    call      LockExtendedCRTRegisters  ;Offset 0xfb4
    ret
ClearBIOSFlags ENDP

;inputs:
;cx = number of lines
;bh = bytes per line
;dx bytes to skip in destination
;ds:si = source
;es:di = destination
;preserves ax
CopyRect PROC NEAR                     ;Offset 0xea9
    push      ax                        ;store ax
LineLoop:
    push      cx                        ;Preserve line count
    mov       cl, bh                    ;set byte count
    xor       ch, ch                    ;zero out top of count
    rep movsb                           ;move cl bytes
    pop       cx                        ;restore line count
    add       di, dx                    ;add bytes to skip in destination
    loop      LineLoop                  ;Offset 0xeaa - loop cx times
    or        al, 0ffh                  ;Set flags?
    pop       ax                        ;restore ax
    ret
CopyRect ENDP

InitializeGraphicsEngine PROC NEAR
    pushf 
    cli
    mov   dx, VideoSubsystemEnable      ;port - 03c3h - Video Subsystem Enable register
    mov   al, 01h                       ;bit 0 is undocumented, bit3 = Video I/O and memory decoding are disabled.
    out   dx, al                        ;
    mov   dx, MiscellaneousRead         ;port - 0x3cc
    in    al, dx                        ;
    cmp   al, 00h                       ;
    je    Label0xedb
    mov   dx, 46e8h                     ;port - 46e8h - Video Subsystem Enable register
    mov   al, 10h                       ;Enable Trio32/Trio64 Setup
    out   dx, al
    push  dx
    mov   dx, 0102h                     ;port - 0102h - Setup Option Select register
    mov   al, 01h                       ;bit 0 = Disable Sleep Mode (1 = don't sleep)
    out   dx, al
    pop   dx                            ;port - 46e8h
    mov   al, 8                         ;bit 3 = 1 - Video I/O and memory decoders are enabled
    out   dx, al                        ;bit 4 = 0 - Trio32/Trio64 is in operational mode
Label0xedb:
    xor   ax, ax                        ;bit 0 = 0 - Enable VGA and VESA planar (4bits/pixel) modes
                                        ;bit 2 = 0 - 8 or more bits/pixel enhanded modes
                                        ;bit 4 = 0 - Disable linear addressing
                                        ;bit 5 = 0 - Disable MMIO
    mov   dx, 4ae8h                     ;port 4ae8h - Advanced Function Control register
    out   dx, al
    call  UnlockExtendedCRTRegisters
    mov   dx, SequenceIndex             ;port 0x3c4
    call  UnlockExtendedSequencerRegisters
    call  ReadDeviceIDAndRevision       ;
    cmp   ah, 10h                       ;10h Device ID = Trio32
    jne   IsTrio64                      ;Offset 0xef6
    mov   ax, 010dh                     ;SRD - Extended Sequencer Register
    out   dx, ax                        ;Write 0x01 - Disable Feature connector, HSYNC Control and VSYNC control 0 = Normal operation
IsTrio64:                               ;Offset 0xef6
    mov   ax, 0314h                     ;SR14 - Clock Synth Control 1 register
    out   dx, ax                        ;Write 0x03 - Power down the MCLK PLL and DCLK PLL
    call  Waitx3                        ;Wait
    mov   ax, 0014h                     ;SR14 - Clock Synth Control 1 register
    out   dx, ax                        ;Write 0x00 - Power on the MCLK PLL and DCLK PLL
    call  Waitx3                        ;Wait again
    call  GetCRTControllerIndexRegister
    mov   ax, 435ch                     ;CR5C - General Output Port register
    out   dx, ax                        ;Write 43h (?)
    mov   ax, 0066h                     ;CR66 - Extended Miscellaneous Control 1 register
    out   dx, ax                        ;Write 00h Bit 6 = Tri-State Off Pixel Address Bus (0 = Normal operation)
                                        ;Bit 7 = PCI bus disconnect enable (0 = PCI bus disconnect disabled)
    mov   al, 36h                       ;CR36 - Configuration 1 Register
    call  ReadDataWithIndexRegister
    and   ah, 03h                       ;And out the top 6 bits
    cmp   ah, 02h                       ;Check if it's 0x02 - System Bus Select - 2 = PCI Local Bus
    je    IsPCI                         ;Offset 0xf23
    mov   cx, word ptr cs:[Data019C]    ;Offset 0x19C
    jmp   IsVLB                         ;Offset 0xf4e
IsPCI:
    mov   ax, 0b102h                    ;PCI BIOS function - Find PCI Device
    mov   cx, DEVICE_ID                 ;0x8811 = Trio32 or Trio64
    mov   dx, VENDOR_ID                 ;0x5333 = S3
    xor   si, si                        ;Device index 0
    int   1ah                           ;PCI BIOS interrupt - On Success, BH = Bus number, BL = Device/function number
    cmp   ah, 86h                       ;86h = Device not found.
    je    DeviceNotFound                ;Offset 0xf5b
    mov   di, 4                         ;Register number 4 - Command register
    mov   ax, 0b109h                    ;PCI BIOS function - Read Configuration Word
    int   1ah                           ;PCI BIOS interrupt - On Success, CX = word read.
    and   cx, 0dfh                      ;VGA Palette Snoop turned off
    mov   ax, 0b10ch                    ;PCI BIOS function - Write Configuration Word
    int   1ah                           ;PCI BIOS interrupt
    mov   ax, 0b109h                    ;PCI BIOS function - Read Configuration Word
    mov   di, 12h                       ;Register number 12h - upper 2 bytes of base address 0
    int   1ah
IsVLB:                                  ;Offset 0xf4e
    call  GetCRTControllerIndexRegister ;
    mov   al, 6bh                       ;CR6B - Extended BIOS Flag 3 register
    mov   ah, ch                        ;Upper byte of base address 0 high word
    out   dx, ax                        ;
    inc   al                            ;CR6C - Extended BIOS Flag 4 register
    mov   ah, cl                        ;Lower byte of base address 0 high word
    out   dx, ax
DeviceNotFound:                         ;Offset 0xf5b
    mov   ah, 11h                       ;11h is special case value.
    call  SetupClocks                   ;Sets the DCLK and MCLK
    push  dx
    mov   dx, SequenceIndex             ;port 0x3c4
    mov   al, 0ah                       ;SRA - External Bus Request Control register
    call  ReadDataWithIndexRegister
    or    ah, 80h                       ;Set bit 7 - 2 MCLK memory writes (3 MCLK is 0)
    out   dx, ax                        ;
    pop   dx                            ;
    mov   al, 40h                       ;CR40 - System Configuration register
    call  ReadDataWithIndexRegister     ;
    or    ah, 31h                       ; bit 0 = 1 - Enable enhanced register access
                                        ; bit 4 = 1 - Ready Control (VL-Bus only) 1 wait state.
    out   dx, ax                        ; bit 5 = 1 - WDL DLAY = 1 (default)
    call  ResetGraphicsEngine           ;
    mov   al, 40h                       ;CR40 - System Configuration register
    call  ReadDataWithIndexRegister
    and   ah, 0feh                      ;Disable Enhanced register access.
    out   dx, ax
    popf
    ret
InitializeGraphicsEngine ENDP

Waitx3 PROC NEAR                        ;Offset 0xf85
    push  dx
    push  bx
    mov   al, 0                         ; 0 = Beeper is silent, we only wait.
    mov   cx, 3
    call  Beep
    pop   bx
    pop   dx
    ret
Waitx3 ENDP

UnlockExtendedSequencerRegisters PROC NEAR;Offset 0xf92 - Unlock extended sequencer registers
    push  dx
    mov   dx, SequenceIndex             ;port 0x3c4
    mov   ax, 0608h                     ;SR8 - Unlock Extended Sequencer - value 06h (unlock code)
    out   dx, ax                        ;      Gives access to SR9 - SR18
    pop   dx
    ret
UnlockExtendedSequencerRegisters ENDP

LockExtendedSequencerRegisters PROC NEAR;Offset 0xf9c - lock extended sequencer registers
    push dx
    mov  dx, SequenceIndex              ;port 0x3c4
    mov  ax, 0008h                      ;SR8 - Unlock Extended Sequencer - value 00h (not setting the significant bits means lock)
    out  dx, ax                         ;      Lockes access to SR9 - SR18
    pop  dx
    ret
LockExtendedSequencerRegisters ENDP

UnlockExtendedCRTRegisters PROC NEAR    ;Offset 0xfa6 - Unlock extended crt registers
    push dx
    call GetCRTControllerIndexRegister
    mov  ax, 4838h                      ;CR38 - Register Lock 1 - value 48h (unlock code)
    out  dx, ax                         ;       Gives access to CR30 - CR3F
    mov  ax, 0a539h                     ;CR39 - Register Lock 2 - value a5h (unlock code)
    out  dx, ax                         ;       Gives access to CR40 - CRFF
    pop  dx
    ret
UnlockExtendedCRTRegisters ENDP

LockExtendedCRTRegisters PROC NEAR      ;Offset 0xfb4 - Lock extended crt registers
    push dx
    call GetCRTControllerIndexRegister
    mov  ax, 0038h                      ;CR38 - Register Lock 1 - value 00h (not setting the significant bits means lock)
    out  dx, ax                         ;       Locks access to CR30 - CR3F
    mov  ax, 0039h                      ;CR39 - Register Lock 2 - value 00h (not setting the significant bits means lock)
    out  dx, ax                         ;       Locks access to CR40 - CRFF
    pop  dx
    ret
LockExtendedCRTRegisters ENDP

;Returns the Device ID in ah and Revision in al
ReadDeviceIDAndRevision PROC NEAR       ;Offset 0xfc2
    push dx
    push bx
    call UnlockExtendedCRTRegisters
    call GetCRTControllerIndexRegister
    mov  al, 2eh                        ;index 2eh - CR2E - Device ID Low
    call ReadDataWithIndexRegister
    mov  bh, ah
    mov  al, 2fh                        ;index 2fh - CR2F - Revision
    call ReadDataWithIndexRegister
    mov  al, ah
    mov  ah, bh
    pop  bx
    pop  dx
    ret
ReadDeviceIDAndRevision ENDP

;Returns the CRTControllerIndex register in dx
;This register lives in a different place depending on
;if the adapter is running in monochrome or color mode.
GetCRTControllerIndexRegister PROC NEAR ;Offset 0xfdd
    push  ax
    mov   dx, MiscellaneousRead         ;port 03cch
    in    al, dx
    mov   dx, CRTControllerIndexB       ;port 03b4h - Monochrome port
    test  al, 1                         ;Test if the IOAddressSelect bit is not set.
    je    IsMonochrome                  ;Jump if bit 0 of al = 0.
    mov   dx, CRTControllerIndexD       ;port 03d4h - Color port
IsMonochrome:
    pop   ax
    ret
GetCRTControllerIndexRegister ENDP

InitializeCRTControllerAndSequencer PROC NEAR
    push  bx
    push  cx
    lodsb cs:[si]                       ;Load count
    mov   cl, al
    xor   ch, ch
LoopCRTCData:                           ;Loop over CRT data and output.
    lodsw cs:[si]
    out   dx, ax
    loop  LoopCRTCData                  ;Offset 0xff6
    push  dx
    push  si
    mov   dx, SequenceIndex             ;port - 0x3c4
    mov   si, word ptr cs:[SequenceInitPtr];Offset 0x1ca
    lodsb cs:[si]
    mov   cl, al
    xor   ch, ch
LoopSequenceData:                       ;Loop over sequence data
    lodsw cs:[si]
    cmp   al, 0dh                       ;If register SRD, clear upper 4 bits
    jne   NotSRD                        ;Offset 0x1014
    and   ah, 0fh
NotSRD:
    out   dx, ax
    loop  LoopSequenceData              ;Offset 0x100b
    pop   si
    pop   dx
    call  FetchBusSpecificSystemConfig  ;Offset 0xe7e
    mov   al, 40h                       ;CR40 - System Configuration
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    and   ah, 0f0h                      ;drop lowest 4 bits
    or    ah, bh                        ;set lowest 4 bits with bus specific settings
    out   dx, ax                        ;
    mov   al, 42h                       ;CR42 Mode Control register
    out   dx, al
    inc   dx
    in    al, dx                        ;Read Mode Control register
    and   al, 0dfh                      ;bit 5, Interlaced mode, 0 = set to non-interlaced.
    out   dx, al
    dec   dx
    pop   cx
    pop   bx
    ret
InitializeCRTControllerAndSequencer ENDP

TextModeAndMonitorCheck PROC NEAR
    call  UnlockExtendedCRTRegisters    ;Offset 0xfa6
    call  SetTextModeAndCheckMonitor    ;Offset 0x231
    ret
TextModeAndMonitorCheck ENDP

;inputs:
;al = video mode > 13h
;outputs:
;di is offset into video mode info array
;zero flag = 0 - found, 1 - not found
;destroys:
;es
FindVideoModeData PROC NEAR             ;Offset 0x103a
    push ax
    mov  di, cs
    mov  es, di
    mov  di, offset VESAVideoModeData   ;Offset 0x522
FindModeData:                           ;Offset 0x1042
    cmp  (VideoModeData ptr es:[di]).Mode, 0ffh;Terminate
    je   Done                           ;Offset 0x1052
    cmp  (VideoModeData ptr es:[di]).Mode, al;al = video mode > 13h
    je   Done                           ;Offset 0x1052
    add  di, SIZEOF VideoModeData          ;0x9
    jmp  FindModeData                   ;Offset 0x1042
Done:                                   ;Offset 0x1052
    sub  di, offset VESAVideoModeData   ;Offset 0x522
    cmp  (VideoModeData ptr es:[di + offset VESAVideoModeData]).Mode, al;Offset 0x522
    pop  ax
    ret  
FindVideoModeData ENDP

;inputs:
;al = video mode
;
;outputs:
;al = flags
;zero flag 0 - found, 1 - found
GetVideoModeFlags PROC NEAR             ;Offset 0x105d
    push di
    push es
    call FindVideoModeData              ;Offset 0x103a
    jne  NotFound                       ;Offset 0x1069
    mov  al, (VideoModeData ptr cs:[di + offset VESAVideoModeData]).Flags;Offset 0x525
NotFound:                               ;Offset 0x1069
    pop  es
    pop  di
    ret  
GetVideoModeFlags ENDP

GetVESAVideoModeOverrideTable PROC NEAR ;Offset 0x106c
    push di
    push es
    call FindVideoModeData              ;Offset 0x103a
    mov  si, (VideoModeData ptr cs:[di + offset VESAVideoModeData]).OverrideTable;Offset 0x523
    pop  es
    pop  di
    ret  
GetVESAVideoModeOverrideTable ENDP

;inputs:
;al = video mode

ApplyVESAOverrideData PROC NEAR         ;Offset 0x1079
    push ax
    push bx
    push cx
    push es
    mov  bl, al
    call UnlockExtendedSequencerRegisters;Offset 0xf92
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 09h                        ;SR9 - Extended Sequencer register 9
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 7fh                        ;Set bit 7 to 0 - Turn off Memory IO only mode. Both programmed IO and memory IO are allowed.
    out  dx, ax
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 30h                        ;CR30 - Chip ID / Revision register
    mov  ah, 55h                        ;Unknown... We are writing to what the documentation says is a read-only register...
    out  dx, ax
    call ReadDataWithIndexRegister      ;Offset 0x4640
    cmp  ah, 55h                        ;Check if the write worked
    je   Label0x10b9                    ;Offset 0x10b9
    mov  al, 40h                        ;CR40 - System Configuration register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 01h                        ;Set bit 0 to 1 - Enhanced register (x2e8h) access enabled
    out  dx, ax
    push dx
    mov  dx, AdvancedFunctionControl    ;port - 0x4ae8
    mov  al, 02h                        ;Set bit 1 to 1 - Unknown
    out  dx, al
    pop  dx
    push si
    mov  si, word ptr cs:[CRTControllerInitPtr];Offset 0x1bd
    call InitializeCRTControllerAndSequencer;Offset 0xfee
    pop  si
Label0x10b9:                            ;Offset 0x10b9
    mov  al, bl
    call FindVideoModeData              ;Offset 0x103a
    je   Found                          ;Offset 0x10c3
    jmp  NotVESAMode                    ;Offset 0x115c
Found:                                  ;Offset 0x10c3
    call ClearMemory                    ;Offset 0x121e
    call EmptyFunction1                 ;Offset 0x230
    mov  al, 41h                        ;CR41 - BIOS Flag register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bh, ah                         ;
    mov  al, 52h                        ;CR52 - Extended BIOS Flag 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0f0h                       ;
    mov  bl, ah
    shr  bx, 04h                        ;
    mov  ax, offset VESAVideoParameters640x480;Offset 0x6f1
    mov  cx, 03h                        ;Loop 3 times. Every time the Override table doesn't match,
                                        ;we move up to a higher resolution. 800x600, 1024x768, 1280x1024
                                        ;While we do that, we drop 3 bits off the low end of bx.
FindOverrideTable:                      ;Offset 0x10e3
    cmp  (VideoModeData ptr es:[di + offset VESAVideoModeData]).OverrideTable, ax;Offset 0x523
    je   OverrideTableFound             ;Offset 0x10f2
    shr  bx, 03h
    DB 05h,40h,00h                      ;add  ax, 40h - masm encoding is different, but it's the same instruction
    loop FindOverrideTable              ;Offset 0x10e3
OverrideTableFound:                     ;Offset 0x10f2
    and  bx, 07h                        ;Whatever we found (or didn't find), we are using the bottom 3 bits of bx.
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 04h                        ;SR4 - MemoryModeControl
    mov  ah, (VideoModeData ptr es:[di + offset VESAVideoModeData]).MemoryMode;Offset 0x528
    out  dx, ax
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    push dx
    mov  al, 40h                        ;CR40 - System Configuration register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 01h                        ;Set bit 0 to 1 - Enable Enhanced (x2e8h) register access
    out  dx, ax
    mov  al, 11h                        ;CR11 - Vertical Retrace End register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 7fh                        ;Set bit 7 to 0 - Enable writes to CRT controller registers
    out  dx, ax
    mov  dx, AdvancedFunctionControl    ;port - 0x4ae8 - Advanced Function Control
    mov  al, (VideoModeData ptr es:[di + offset VESAVideoModeData]).AdvancedFunction;Offset 0x526
    out  dx, al
    pop  dx                             ;Restore CRTC register
    mov  ah, bl
    xor  bh, bh
    mov  bl, byte ptr cs:[Data019E]     ;offset 0x19e
    mov  di, (VideoModeData ptr es:[di + offset VESAVideoModeData]).Value7;Offset 0x529
DataSearch:                             ;Offset 0x112e
    mov  al, byte ptr es:[di + 02h]     ;Flag and 0..7 index
    test al, 80h                        ;If top bit set, exit. we're at the "end" (actually start)
    jne  DataFound                      ;Offset 0x1141
    and  ax, 0f0fh                      ;This and should be 0707h
    DB 3Ah, 0C4h                        ;cmp  al, ah    masm encoding difference
    jbe  DataFound                      ;Offset 0x1141
    sub  di, bx                         ;Move backwards
    jmp  DataSearch                     ;Offset 0x112e
DataFound:                              ;Offset 0x1141
    mov  ah, byte ptr es:[di + 03h]     ;Clock config index
    call SetupClocks                    ;Offset 0x31c
    mov  cx, 19h                        ;25 CRTC registers
    xor  al, al                         ;CR0
    mov  si, word ptr es:[di]           ;Load CRTC register data pointer
WriteCRTCRegisters:                     ;Offset 0x1150
    mov  ah, byte ptr es:[si]           ;
    out  dx, ax                         ;Write CR0 - CR18
    inc  al
    inc  si
    loop WriteCRTCRegisters             ;Offset 0x1150
    call ConfigureExtraVESAModeSettings ;Offset 0x3d9
NotVESAMode:                            ;Offset 0x115c
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call EnableOver256KAddressingAndSetAddressWindow;Offset 0x2f6
    call SetColorMode                   ;Offset 0xdf6
    call LockExtendedSequencerRegisters ;Offset 0xf9c
    pop  es
    pop  cx
    pop  bx
    pop  ax
    ret
ApplyVESAOverrideData ENDP

;These two functions do exactly the same, they just continue somewhere else.
;Optimized C code...
Func0x116d PROC NEAR                    ;Offset 0x116d
    cmp  byte ptr ds:[BDA_DisplayMode], 6ah;Offset 0x449
    jne  Func0x1181                     ;Offset 0x1181
    jmp  Func0x1f5d                     ;Offset 0x1f5d
Func0x116d ENDP

Func0x1177 PROC NEAR                    ;Offset 0x1177
    cmp  byte ptr ds:[BDA_DisplayMode], 6ah;Offset 0x449
    jne  Func0x1181                     ;Offset 0x1181
    jmp  Func0x1f0e                     ;Offset 0x1f0e
Func0x1177 ENDP

Func0x1181 PROC NEAR                    ;Offset 0x1181
    add  sp, 4
    ret
Func0x1181 ENDP

Func0x1185 PROC FAR                     ;Offset 0x1185
    xchg al, ah
    call GetVideoModeFlags              ;Offset 0x105d
    xchg al, ah
    jne  Label0x119a                    ;Offset 0x119a
    test ah, 01h
    jne  Label0x119a                    ;Offset 0x119a
    push si
    push bx
    push cx
    push dx
    jmp  Func0x2b82                     ;Offset 0x2b82
Label0x119a:                            ;Offset 0x119a
    mov  ah, 0dh
    pop  ds
    iret
Func0x1185 ENDP

Func0x119e PROC FAR                     ;Offset 0x119e
    xchg al, ah
    call GetVideoModeFlags              ;Offset 0x105d
    xchg al, ah
    jne  Label0x11b2                    ;Offset 0x11b2
    test ah, 01h
    jne  Label0x11b2                    ;Offset 0x11b2
    push bx
    push cx
    push dx
    jmp  Func0x2ac1                     ;Offset 0x2ac1
Label0x11b2:                            ;Offset 0x11b2
    mov  ah, 0ch
    pop  ds
    iret
Func0x119e ENDP

Func0x11b6 PROC NEAR                    ;Offset 0x11b6
    xchg al, ah
    call GetVideoModeFlags              ;Offset 0x105d
    xchg al, ah
    jne  Label0x11c6                    ;Offset 0x11c6
    test ah, 01h
    je   Func0x11dc                     ;Offset 0x11dc
    mov  ah, 03h
Label0x11c6:                            ;Offset 0x11c6
    jmp  Func0x2715                     ;Offset 0x2715
Func0x11b6 ENDP

Func0x11c9 PROC NEAR                    ;Offset 0x11c9
    xchg al, ah
    call GetVideoModeFlags              ;Offset 0x105d
    xchg al, ah
    jne  Func0x11d9                     ;Offset 0x11d9
    test ah, 01h
    je   Func0x11e6                     ;Offset 0x11e6
    mov  ah, 03h
Func0x11c9 ENDP

Func0x11d9 PROC NEAR                    ;Offset 0x11d9
    jmp  Func0x2694                     ;Offset 0x2694
Func0x11d9 ENDP

Func0x11dc PROC NEAR                    ;Offset 0x11dc
    cmp  byte ptr ds:[BDA_DisplayMode], 6ah;Offset 0x449
    jne  Func0x11f0                     ;Offset 0x11f0
    jmp  Func0x2712                     ;Offset 0x2712
Func0x11dc ENDP

Func0x11e6 PROC NEAR                    ;Offset 0x11e6
    cmp  byte ptr ds:[BDA_DisplayMode], 6ah;Offset 0x449
    jne  Func0x11f0                     ;Offset 0x11f0
    jmp  Func0x2683                     ;Offset 0x2683 
Func0x11e6 ENDP 

Func0x11f0 PROC NEAR                    ;Offset 0x11f0
    ret
Func0x11f0 ENDP

Func0x11f1 PROC NEAR                    ;Offset 0x11f1
    xchg al, ah
    call GetVideoModeFlags              ;Offset 0x105d
    xchg al, ah
    jne  Label0x1204                    ;Offset 0x1204
    test ah, 01h
    je   Label0x1204                    ;Offset 0x1204
    mov  ah, 03h
    jmp  Func0x240d                     ;Offset 0x240d
Label0x1204:                            ;Offset 0x1204
    jmp  Func0x240a                     ;Offset 0x240a
Func0x11f1 ENDP

SetCPUUpperMemoryAddressBits PROC NEAR  ;Offset 0x1207
    push ax
    push dx
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 31h                        ;CR31 - Memory Configuration register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 01h                        ;bit 0 = 1 - Enable Base Address Offset
                                        ;Address offset bits 3-0 of CR35 and bits 3-2 of CR51 or the new address offset
                                        ;bits (5-0 of CR6A) are enabled for specifying the 64K page of display memory.
                                        ;Bits 5-0 of CR6A are used if this field contains a non-zero value. 
                                        ;This allows access to up to 4 MBytes of display memory through a 64K window. (2 MBytes for the Tri032)
    out  dx, ax
    mov  al, 6ah                        ;CR6A - Extended System Control 4 register
    mov  ah, bl                         ;bits 5 - 0 = bl
                                        ;This field contains the upper 6 bits (19-14) of the CPU base address, allowing accessing 
                                        ;of up to 4 MBytes of display memory via 64K pages. When a non-zero value is programmed in this field,
                                        ;bits 3-0 of CR35 and 3-2 of CR51 (the old CPU base address bits) are ignored. 
                                        ;Bit 0 of CR31 must be set to 1 to enable this field.
                                        ;If linear addressing is enabled and a 64 KByte window is specified, these bits specify the 64K page
                                        ;to be accessed at the base address specified in CR59 and CR5A.
                                        ;Otherwise, the base address is normally at AOOOH.
                                        ;The Tri032 only supports 2 MBytes of memory, so the upper bit must always be O.
    out  dx, ax
    pop  dx
    pop  ax
    ret  
SetCPUUpperMemoryAddressBits ENDP

EmptyFunction2 PROC NEAR                ;Offset 0x121d
    ;Code was probably commented out
    ret
EmptyFunction2 ENDP

ClearMemory PROC NEAR                   ;Offset 0x121e
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487, 80h
    je   Label0x1226                    ;Offset 0x1226
    ret
Label0x1226:                            ;Offset 0x1226
    push dx
    push ax
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    call UnlockExtendedCRTRegisters     ;Offset 0xfa6
    mov  al, 58h                        ;CR58 - Linear Address Window Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0efh                       ;bit 4 = 0 - Disable linear addressing
    out  dx, ax                         ;Write Out
    mov  al, 50h                        ;CR50 - Extended System Control 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fh                        ;Clear out top 4 bits
    or   ah, 10h                        ;bit 4 = 1 - 16 bits / pixel
    out  dx, ax                         ;Write out
    mov  al, 43h                        ;CR43 - Extended Mode register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0f7h                       ;bit 3 = 0 - Unknown
    out  dx, ax                         ;
    mov  al, 40h                        ;CR40 - System Configuration register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 01h                        ;bit 0 = 1 - Enable enhanced register access
    out  dx, ax
    mov  dx, AdvancedFunctionControl    ;0x4ae8
    mov  al, 07h                        ;bit 0 = 1 - Enable Enhanced Functions
                                        ;bit 1 = 1 - Unknown
                                        ;bit 2 = 1 - Enhanced modes pixel length = 4 bits / pixel
    out  dx, al
    mov  dh, 0beh                       ;0xbee8 - WriteRegisterData
    xor  ax, ax
    mov  ah, 10h                        ;bits 15-12 = 0001 - Index = 0x1 - Top Scissors
    out  dx, ax                         ;bits 11-0 = 0x000
    mov  ah, 20h                        ;bits 15-12 = 0010 - Index = 0x2 - Left Scissors
    out  dx, ax                         ;bits 11-0 = 0x000
    mov  ax, 3fffh                      ;bits 15-12 = 0011 - Index 0x3 - Bottom Scissors
    out  dx, ax                         ;bits 11-0 = 0xfff
    mov  ah, 4fh                        ;bits 15-12 = 0100 - Index 0x4 - Right Scissors
    out  dx, ax                         ;bits 11-0 = 0xfff
    push ax                             ;store 0x4fff
    mov  ax, 0e040h                     ;bits 15-12 = 0111 - Index 0xe - Multifunction Control Miscellaneous
    out  dx, ax                         ;bit 6 = 1 - Slow Read/Modify/Write Cycle
    pop  ax                             ;0x4fff
    mov  dh, 0AAh                       ;0xaae8 - BitplaneWriteMask
    mov  ah, 0ffh                       ;0xffff
    out  dx, ax                         ;update all bitplanes lower 16 bits
    out  dx, ax                         ;update all bitplanes upper 16 bits
    mov  dh, 0a6h                       ;0xa6e8 - ForegroundColorRegister
    xor  ax, ax                         ;0
    out  dx, ax                         ;set foreground color to black, lower 16 bits
    out  dx, ax                         ;set foreground color to black, upper 16 bits
    mov  dh, 0bah                       ;0xbae8 - ForegroundMix
    mov  al, 27h                        ;0x0027
                                        ;bits 3-0 = 0111 = Mix Type: new
                                        ;bits 6-5 = 01 = Color Source: Foreground color
    out  dx, ax
    mov  dh, 0beh                       ;0xbee8 - WriteRegisterData
    mov  ax, 0a000h                     ;bits 15-12 = 1020 - Index 0xa - Pixel Control
    out  dx, ax                         ;bits 7-6 = 00 - Foreground Mix register is always selected
    mov  dh, 86h                        ;0x86e8 - CurrentXPosition
    xor  ax, ax                         ;0
    out  dx, ax
    mov  dh, 82h                        ;0x82e8 - CurrentYPosition
    out  dx, ax                         ;0
    mov  dh, 96h                        ;0x96e8 - MajorAxisPixelCount
    mov  ax, 0fffh                      ;0xfff = 4095
    out  dx, ax                         ;0b1000000101 10011
    mov  dh, 0beh                       ;0xbee8 - WriteRegisterData
    out  dx, ax                         ;bits 15-12 = 0 - Minor Axis Pixel Count, bits 11-0 = 4095 - Rectangle Height
    mov  dh, 09ah                       ;0x9ae8 - DrawingCommand
    mov  ax, 40b3h                      ;bit 1 = 1 - Multiple pixels transferred at a time (across the plane mode)
                                        ;bit 4 = 1 - Draw Pixels
                                        ;bits 7-5 = 101 - Select Drawing Direction 225 degrees, +Y,X maj, +X
                                        ;bits 15-13 = 010 - Rectangle fill
    out  dx, ax
    call WaitGraphicsEngineReady        ;Offset 0x12a6
    pop  ax
    pop  dx
    ret
ClearMemory ENDP

WaitGraphicsEngineReady PROC NEAR       ;Offset 0x12a6
    push dx
    push ax
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 40h                        ;CR40
    call ReadDataWithIndexRegister      ;Offset 0x4640
    test ah, 01h                        ;Check to see if Enhanced command registers are unlocked.
    je   Exit                           ;Offset 0x12c6 if locked, exit.
    mov  dx, GraphicsProcessorStatus    ;port - 0x9ae8
    in   ax, dx
    DB 03Dh, 0FFh, 0FFh                 ;cmp  ax, 0ffffh masm encoding difference
    je   Exit                           ;Offset 0x12c6 if all ones, we exit.
Label0x12be:                            ;Offset 0x12be
    jmp  Label0x12c0                    ;Offset 0x12c0
Label0x12c0:                            ;Offset 0x12c0
    in   ax, dx
    test ah, 02h                        ;Test Harware (Graphics Engine) Busy
    jne  Label0x12be                    ;Offset 0x12be if busy, try again.
Exit:                                   ;Offset 0x12c6
    pop  ax
    pop  dx
    ret  
WaitGraphicsEngineReady ENDP

ResetGraphicsEngine PROC NEAR           ;Offset 0x12c9
    push dx
    mov  dx, SubsystemControl           ;port - 0x42e8
    mov  ax, 9000h                      ;Setting bit 12 (Unknown), set Graphics Engine reset (bit 15,14 = 10)
    out  dx, ax
    call WaitGraphicsEngineReady        ;Offset 0x12a6
    mov  ah, 50h                        ;Set bits 14 and 8 (unknown) bit 15,14 = 01 = Graphics Engine Enabled.
    out  dx, ax
    call WaitGraphicsEngineReady        ;Offset 0x12a6
    pop  dx
    ret
ResetGraphicsEngine ENDP

ConfigureAndClearVideoMemory PROC NEAR  ;Offset 0x12dc
    call UnlockExtendedCRTRegisters     ;Offset 0xfa6
    call UnlockExtendedSequencerRegisters;Offset 0xf92
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 36h                        ;CR36 - Configuration 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 1fh                        ;And off the top 3 bits
                                        ;000 = 4MB memory
    out  dx, ax                         ;Write out
    test ah, 04h                        ;If bit 2 = 0 - Extended Data Out (EDO) Mode
    je   Trio32OrEDO                    ;Offset 0x1321
    call ReadDeviceIDAndRevision        ;Offset 0xfc2
    cmp  ah, 10h                        ;Trio32
    je   Trio32OrEDO                    ;Offset 0x1321
    push dx
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 0ah                        ;SRA - External Bus Request Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 40h                        ;Flip bit 6 to 1 - Pin 50 outputs ~RAS1 for FPM
    out  dx, ax                         ;Write out
    pop  dx                             ;Restore CRTControllerIndexRegister
    mov  bl, 3fh                        ;Set memory bits to 0x0fc000 - 0b000011111100000000000000 - 4MB
    call SetCPUUpperMemoryAddressBits   ;Offset 0x1207
    call TestMemory                     ;Offset 0x1359
    je   Found4MB                       ;Offset 0x1339
    push dx
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 0ah                        ;SRA - External Bus Request Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0bfh                       ;Flip bit 6 to 0 - Pin 50 outputs ~OE0 for FPM
    out  dx, ax                         ;Write out
    pop  dx                             ;Restore CRTControllerIndexRegister
Trio32OrEDO:                            ;Offset 0x1321
    mov  bl, 1fh                        ;Set memory bits to 0x07c000 - 0b000001111100000000000000 - 2MB
    call SetCPUUpperMemoryAddressBits   ;Offset 0x1207
    call TestMemory                     ;Offset 0x1359
    je   Found2MB                       ;Offset 0x133d
    mov  bl, 0fh                        ;;Set memory bits to 0x03c000 - 0b000000111100000000000000 - 1MB
    call SetCPUUpperMemoryAddressBits   ;Offset 0x1207
    call TestMemory                     ;Offset 0x1359
    je   Found1MB                       ;Offset 0x1341
    mov  bh, 0e0h                       ;CR36 bits 7-5 - 111 = 4MB
    jmp  Label0x1343                    ;Offset 0x1343
Found4MB:                               ;Offset 0x1339
    xor  bx, bx                         ;CR36 bits 7-5 - 000 = 4MB
    jmp  Label0x1343                    ;Offset 0x1343
Found2MB:                               ;Offset 0x133d
    mov  bh, 80h                        ;CR36 bits 7-5 - 100 = 2MB
    jmp  Label0x1343                    ;Offset 0x1343
Found1MB:                               ;Offset 0x1341
    mov  bh, 0c0h                       ;CR36 bits 7-5 - 110 = 1MB
Label0x1343:                            ;Offset 0x1343
    mov  al, 36h                        ;CR36 - Configuration 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 1fh                        ;And out upper 3 bits. 
    or   ah, bh                         ;Or in memory found
    out  dx, ax                         ;Write out
    mov  bl, 00h                        ;Reset upper memory bit address to 0
    call SetCPUUpperMemoryAddressBits   ;Offset 0x1207
    call ClearMemory                    ;Offset 0x121e
    xor  bx, bx
    ret  
ConfigureAndClearVideoMemory ENDP

;outputs:
;zero flag: 0 = success, 1 = fail
TestMemory PROC NEAR                    ;Offset 0x1359
    mov        al, 80h                  ;Test value
Label0x135b:                            ;Offset 0x135b
    xor        di, di                   ;destination offset
    mov        cx, 00ffh                ;test 255 bytes
    rep stosb                           ;store test value in memory
    xor        di, di                   ;reset offset
    mov        cx, 00ffh                ;test 255 bytes
    repe scasb                          ;if not equal, exit
    jne        Label0x1373              ;Offset 0x1373
    cmp        al, 00h                  ;if zero, done testing
    je         Label0x1373              ;Offset 0x1373
    shr        al, 01h                  ;move test bit down one bit.
    jmp        Label0x135b              ;Offset 0x135b
Label0x1373:                            ;Offset 0x1373
    ret        
TestMemory ENDP

UnlockExtendedCRTRegistersSafe PROC NEAR;Offset 0x1374
    push       ax
    push       bx
    push       dx
    call       UnlockExtendedCRTRegisters;Offset 0xfa6
    xor        al, al
    pop        dx
    pop        bx
    pop        ax
    ret        
UnlockExtendedCRTRegistersSafe ENDP

EmptyFunction3 PROC NEAR                ;Offset 0x1380
    call  EmptyFunction1                ;Offset 0x230
    ret
EmptyFunction3 ENDP

Func0x1384 PROC NEAR                    ;Offset 0x1384
    mov   dx, 03c4h                     ;port - 0x3c4
    call  UnlockExtendedSequencerRegisters;Offset 0xf92
    mov   cx, 13h
    mov   ah, 09h
Label0x138f:                            ;Offset 0x138f
    mov   al, ah
    out   dx, al
    inc   dx
    in    al, dx
    stosb byte ptr es:[di]
    dec   dx
    inc   ah
    loop  Label0x138f                   ;Offset 0x138f
    call  GetCRTControllerIndexRegister ;Offset 0xfdd
    call  UnlockExtendedCRTRegisters    ;Offset 0xfa6
    mov   cx, 40h
    mov   ah, 30h
Label0x13a5:                            ;Offset 0x13a5
    mov   al, ah
    out   dx, al
    inc   dx
    in    al, dx
    stosb byte ptr es:[di]
    dec   dx
    inc   ah
    cmp   ah, 4ah
    je    Label0x13b8                   ;Offset 0x13b8
    cmp   ah, 4bh
    jne   Label0x13c9                   ;Offset 0x13c9
Label0x13b8:                            ;Offset 0x13b8
    mov   al, 45h
    out   dx, al
    inc   dx
    in    al, dx
    dec   dx
    mov   al, ah
    out   dx, al
    inc   dx
    in    al, dx
    stosb byte ptr es:[di]
    in    al, dx
    stosb byte ptr es:[di]
    in    al, dx
    stosb byte ptr es:[di]
    dec   dx
Label0x13c9:                            ;Offset 0x13c9
    loop  Label0x13a5                   ;Offset 0x13a5
    push  es
    push  di
    xor   ax, ax
    xor   cx, cx
    mov   al, 6ch
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    mov   al, ah
    mov   cl, ah
    call  FindVideoModeData             ;Offset 0x103a
    mov   al, (VideoModeData ptr es:[di + offset VESAVideoModeData]).AdvancedFunction;Offset 0x526
    pop   di
    pop   es
    stosb byte ptr es:[di]
    ret
Func0x1384 ENDP

;inputs:
;es:di: pointer to 19 SR registers, 64 CR registers (68 bytes), 1 advanced function control byte
WriteSequencerCRTCAndAdvancedFunctionsRegisters PROC NEAR;Offset 0x13e6
    mov   dx, SequenceIndex             ;port - 0x3c4
    in    al, dx                        ;fetch index
    push  ax                            ;store index
    call  UnlockExtendedSequencerRegisters;Offset 0xf92
    mov   cx, 13h                       ;loop 19 times
    mov   ah, 09h                       ;SR9 - SR1B
LoadSequenceRegisters:                  ;Offset 0x13f3
    mov   al, ah
    out   dx, al
    inc   dx
    lodsb byte ptr es:[si]
    out   dx, al
    dec   dx
    inc   ah
    loop  LoadSequenceRegisters         ;Offset 0x13f3
    pop   ax                            ;restore index
    out   dx, al                        ;set index back to what it was
    call  GetCRTControllerIndexRegister ;Offset 0xfdd
    in    al, dx                        ;fetch index
    push  ax                            ;store index
    call  UnlockExtendedCRTRegisters    ;Offset 0xfa6
    mov   cx, 40h                       ;64
    mov   ah, 30h                       ;CR30 - CR6F
LoadCRTContollerRegisters:              ;Offset 0x140e
    mov   al, ah
    out   dx, al
    inc   dx
    lodsb byte ptr es:[si]
    out   dx, al
    dec   dx
    inc   ah
    cmp   ah, 4ah                       ;Handle CR4A differently
    je    HandleHWCursorColors          ;Offset 0x1422
    cmp   ah, 4bh                       ;Handle CR4B differently
    jne   ContinueCRTCLoop              ;
HandleHWCursorColors:                   ;Offset 0x1422
    mov   al, 45h                       ;CR45 - Hardware Graphics Cursor Mode register
    out   dx, al
    inc   dx
    in    al, dx                        ;Read hardware graphics cursor mode to reset CR4A/CR4B index
    dec   dx                            
    mov   al, ah                        ;CR4A/CR4B - Hardware Graphics Cursor Foreground/Background Color Stack register
    out   dx, al                        ;
    inc   dx
    lodsb byte ptr es:[si]              ;R
    out   dx, al
    lodsb byte ptr es:[si]              ;G
    out   dx, al
    lodsb byte ptr es:[si]              ;B
    out   dx, al
    dec   dx
ContinueCRTCLoop:                       ;Offset 0x1436
    loop  LoadCRTContollerRegisters     ;Offset 0x140e
    mov   al, 40h                       ;CR40 - System Configuration Register
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax                            ;Save CR40 content
    push  dx                            ;Save CRTC index register
    or    ah, 01h                       ;Enable enhanced features
    out   dx, ax
    lodsb byte ptr es:[si]              ;
    mov   dx, AdvancedFunctionControl   ;port - 0x4ae8
    out   dx, al
    mov   dx, BitplaneWriteMask         ;port - 0xaae8
    mov   ax, 0ffffh                    ;Set write plane mask to update all planes
    out   dx, ax
    out   dx, ax
    pop   dx                            ;Restore CRTC index register
    pop   ax
    out   dx, ax                        ;Restore CR40 lock back to what it was
    pop   ax
    out   dx, al                        ;Restore CRTC Index back to what it was
    ret
WriteSequencerCRTCAndAdvancedFunctionsRegisters ENDP

;inputs:
;none
;outputs:
;dx = Memory size in 4K blocks. 0.5MiB = 0 (why?)
GetInstalledMemorySizeIn4KBlocks PROC NEAR
    push ax
    call UnlockExtendedCRTRegisters     ;Offset 0xfa6
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 36h                        ;CR36 - Configuration 1 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    xor  dx, dx                         ;dx = 0
    and  ah, 0e0h                       ;And off everything but the upper 3 bits - Display memory size
    cmp  ah, 0e0h                       ;111 = 0.5MiB Trio32 only
    je   Exit                           ;Offset 0x1482
    inc  dx                             ;dx = 1
    cmp  ah, 0c0h                       ;110 = 1MiB
    je   Exit                           ;Offset 0x1482
    inc  dx                             ;dx = 2
    cmp  ah, 80h                        ;100 = 2MiB
    je   Exit                           ;Offset 0x1482
    inc  dx                             ;
    inc  dx                             ;dx = 4
    cmp  ah, 00h                        ;000 = 4MiB
    je   Exit                           ;Offset 0x1482
    mov  dl, 01h                        ;dx = 1 - Assume 1MiB
Exit:                                   ;Offset 0x1482
    xchg dh, dl                         ;dx = 0 - 256 - 512 - 1024
    pop  ax
    ret
GetInstalledMemorySizeIn4KBlocks ENDP

DSSegment               DW 0000h        ;Offset 0x1486
Data1488                DW 0000h        ;Offset 0x1488
Data148A                DW 0A000h       ;Offset 0x148a

VideoBootstrap PROC FAR                 ;Offset 0x148C
    cli
    pusha
    push   es
    push   ds
    sub    sp, 2
    cld
    mov    bp, sp
    mov    ds, word ptr cs:[DSSegment]  ;Offset 0x1486
    mov    ax, 1201h                    ;Alternate Function Select (VGA, MCGA) - al = 0x01 - Disable video addressing 
    mov    bl, 32h                      ;BL is the function (Video Addressing)
    int    10h                          ;Video BIOS interrupt (we haven't set up our interrupt handler yet, so this is the stock interrupt handler)
    call   InitializeGraphicsEngine
    call   InitCRTControllerAndSequencer
    mov    dx, MiscellaneousWrite       ;port 3c2h
    mov    al, 23h                      ;IOAddressSelect = 1 (Color), 
                                        ;EnableCPUDisplayMemoryAccess = 1 (Enabled), 
                                        ;SelectVideoClockFrequency = 00 (25.175 MHz DCLK for 640 horizontal pixels),
                                        ;SelectHigh64KPage = 1 (Select High Page),
                                        ;SelectNegativeHorizontalSyncPulse = 0 (Positive horizontal retrace sync pulse),
                                        ;SelectNegativeVerticalSyncPulse = 0 (Positive vertical retrace sync pulse)
    out    dx, al                       ;
    xor    al, al
    mov    byte ptr ds:[BDA_DisplayMode], al;Offset 0449h - Set display mode to 00h - BDA_DM_40x25_BW_Text
    mov    byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0487h, 0x60
    mov    byte ptr ds:[BDA_VideoDisplayDataArea], 00h;Offset 0489h - reset all flags
    call   SetupPointersAndInterruptHandlers
    call   ConfigureColorOrMonochrome
    mov    ax, (BDA_EFBS_CGAMono80x25_2 SHL 8) OR INT10_00_07_T_80x25_9x16_720x400_M_x_B000;80x25 text mode monochrome, 0xb07
    test   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    jne    IsMonochrome
    mov    ax, (BDA_EFBS_MDAHiResEnhanced_2 SHL 8) OR INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;80x25 text mode 16 color, 0x903
IsMonochrome:
    mov    byte ptr ds:[BDA_EGAFeatureBitSwitches], ah;Offset 0488h
    xor    ah, ah                       ;INT10_00_SetVideoMode
    int    10h
    call   PrepareAttributeControllerAndResetNonUsedModeControl
    or     byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0489h, 0x10
    test   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0489h, 0x01
    jne    VGAEnabled
    mov    ax, word ptr ds:[BDA_DetectedHardware];Offset 0410h
    push   ax
    mov    bl, INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;0x3
    mov    ax, (BDA_DH_80x25Monochrome SHL 8) OR INT10_00_07_T_80x25_9x16_720x400_M_x_B000;80x25 text mode monochrome, 0x3007
    test   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je     IsColor
    mov    ax, (BDA_DH_80x25Color SHL 8) OR INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;80x25 text mode 16 color, 0x2003
    mov    bl, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
IsColor:
    and    byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0410h, 0xcf
    or     byte ptr ds:[BDA_DetectedHardware], ah;Offset 0410h
    xor    ah, ah                       ;INT10_00_SetVideoMode
    int    42h                          ;System BIOS Video interrupt
    pop    ax
    mov    word ptr ds:[BDA_DetectedHardware], ax;Offset 0410h
    mov    al, bl
    xor    ah, ah
    int    10h
    call   PrepareAttributeControllerAndResetNonUsedModeControl
VGAEnabled:
    call   ReadEGAFeatureBitSwitches    ;Returns some mystery bits
    or     byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0448h
    test   byte ptr cs:[Data0199], 02h  ;Data0199 = 0x80
    jne    Label0x1539
    xor    al, al
    mov    byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], al;Offset 048ah
    call   TextModeAndMonitorCheck
    je     Label0x1539
    mov    al, 03h                      ;3 beeps
    call   PlayBeepPattern              ;Offset 0x468c
    mov    word ptr [bp + 4], 01h
Label0x1539:
    test   byte ptr cs:[Data0199], 04h  ;Data0199 = 0x80
    jne    Label0x158b
    mov    al, INT10_00_0E_G_80x25_8x8_640x200_16C_4_A000;0xe
    test   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je     IsColor2
    mov    al, INT10_00_0F_G_80x25_8x14_640x350_M_2_A000;0xf
IsColor2:
    call   GetVideoModeOverrideTable    ;es:si points to video mode override table entry based on al
    call   ApplyVideoParameters
    mov    ax, 0a000h                   ;Segment 0xa000
    mov    es, ax
    mov    ax, 0805h                    ;GR5 - Graphics Controller Mode register
                                        ;bit 4 = 1 - Enable Read Compare
    mov    dx, GraphicsControllerIndex  ;port - 0x3ce
    out    dx, ax
    mov    ax, 0f02h                    ;GR2 - Color Compare Register
                                        ;bits 3-0 = 1111 - Color compare value
    out    dx, ax
    mov    ax, 0f02h                    ;SR2 - Enable Write Plane register
                                        ;bits 3-0 = 1111 - Enable writing to all planes
    mov    dx, SequenceIndex            ;port - 0x3c4
    out    dx, ax
    call   ConfigureAndClearVideoMemory
    jne    MemoryFailure
    xor    dl, dl
    jmp    MemorySuccess
MemoryFailure:
    mov    dl, 03h
MemorySuccess:
    push   dx
    mov    al, INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;0x3
    test   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je     IsColor3
    mov    al, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
IsColor3:
    call   GetVideoModeOverrideTable
    call   ApplyVideoParameters
    pop    dx
    or     dl, dl
    jne    PlayMemoryErrorBeeps
Label0x158b:
    jmp    Label0x1597
PlayMemoryErrorBeeps:
    mov    al, 03h                      ;3 beeps
    call   PlayBeepPattern              ;Offset 0x468c
    mov    word ptr [bp + 04h], 01h
Label0x1597:
    call   SetStartupVideoMode
    cmp    byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], 00h;Offset 048ah
    jne    DisplayCombinationCodeAlreadySetUp
    call   ConfigureDisplayCombinationCode
DisplayCombinationCodeAlreadySetUp:
    call   ClearBIOSFlags               ;0xe97
    call   EmptyFunction1               ;Offset 0x230
    call   PrintBanner
    mov    sp, bp
    add    sp, 2
    pop    ds
    pop    es
    popa
    retf
VideoBootstrap ENDP

SetStartupVideoMode PROC NEAR
    test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0489h, 0x1
    jne  IsVGA                          ;Offset 0x15e6
    mov  ax, word ptr ds:[BDA_DetectedHardware];Offset 0410h
    push ax                             ;Save DetecedHardware
    mov  bl, INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;0x3
    mov  ax, (BDA_DH_80x25Monochrome SHL 8) OR 07h;0x3007
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je   IsColor                        ;Offset 0x15d2
    mov  ax, (BDA_DH_80x25Color SHL 8) OR 03h;0x2003
    mov  bl, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
IsColor:                                ;Offset 0x15d2
    and  byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0410h, 0xcf
    or   byte ptr ds:[BDA_DetectedHardware], ah;Offset 0410h
    xor  ah, ah                         ;INT10_00_SetVideoMode
    int  42h                            ;Call system int 10h handler
    pop  ax                             ;Restore DetectedHardware
    mov  word ptr ds:[BDA_DetectedHardware], ax;Offset 0410h
    jmp  Label0x1600                    ;Offset 0x1600
    nop  
IsVGA:                                  ;Offset 0x15e6
    test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_MonochromeMonitor;Offset 0489h, 0x4
    jne  IsMonochromeOrInactive         ;Offset 0x15fb
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0487h, 0x8
    jne  IsMonochromeOrInactive         ;Offset 0x15fb
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0487h, 0xfd
    jmp  Label0x1600                    ;Offset 0x1600
IsMonochromeOrInactive:                 ;Offset 0x15fb
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
Label0x1600:                            ;Offset 0x1600
    mov  ax, (BDA_DH_80x25Color SHL 8) OR BDA_EFBS_MDAHiResEnhanced_2;0x2009
    mov  bl, INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;0x3
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je   IsMonochrome2                  ;Offset 0x1611
    mov  ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_EFBS_CGAMono80x25_2;0x300b
    mov  bl, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
IsMonochrome2:                          ;Offset 0x1611
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0488h, 0xf0
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0488h
    and  byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0410h, 0xcf
    or   byte ptr ds:[BDA_DetectedHardware], ah;Offset 0410h
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0487h
    and  al, BDA_VMO_Inactive           ;0x8
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Inactive;Offset 0487h, 0xf7
    mov  ah, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0489h
    and  ah, BDA_VDDA_VGA               ;0x1
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0489h, 0xfe
    push ax                             ;Store inactive and vga bits
    xor  ah, ah                         ;INT10_00_SetVideoMode
    mov  al, bl                         ;Video mode 0x3 or 0x7
    int  10h
    pop  ax
    or   byte ptr ds:[BDA_VideoDisplayDataArea], ah;Offset 0489h or back in the vga bit
    test ah, BDA_VDDA_VGA               ;0x1
    jne  Exit                           ;Offset 0x167d
    test al, BDA_VMO_Inactive           ;0x8
    je   Exit                           ;Offset 0x167d
    push ax
    mov  ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_EFBS_MDAHiResEnhanced;3003h
    mov  bl, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0487h, 0x2
    je   IsMonochrome3                  ;Offset 0x1660
    mov  ax, (BDA_DH_80x25Color SHL 8) OR BDA_EFBS_CGAMono80x25;2005h
    mov  bl, INT10_00_03_T_80x25_9x16_720x400_16C_8_B800;0x3
IsMonochrome3:                          ;Offset 0x1660
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], 0f0h;Offset 0488h
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0488h
    and  byte ptr ds:[BDA_DetectedHardware], 0cfh;Offset 0410h
    or   byte ptr ds:[BDA_DetectedHardware], ah;Offset 0410h
    pop  ax
    or   byte ptr ds:[BDA_VideoModeOptions], al;Offset 0487h
    xor  ah, ah                         ;INT10_00_SetVideoMode
    mov  al, bl
    int  10h
Exit:                                   ;Offset 0x167d
    ret
SetStartupVideoMode ENDP

DisplayCombinationCode  DB 0Ch          ;00 - Color, Not VGA
                        DB 0Bh          ;01 - Color, VGA
                        DB 0Eh          ;10 - Mono, Not VGA
                        DB 0Dh          ;11 - Mono, VGA

ConfigureDisplayCombinationCode PROC NEAR;Offset 0x1682
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0487h
    and  al, BDA_VMO_Monochrome         ;0x2
    mov  ah, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0489h
    and  ah, BDA_VDDA_VGA               ;0x1
    or   al, ah
    xor  ah, ah
    mov  bx, ax
    mov  al, byte ptr cs:[bx + offset DisplayCombinationCode];Offset 0x167e
    mov  byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], al;Offset 048ah
    cmp  al, 0eh
    jne  Exit                           ;Offset 0x16c1
    push ds
    mov  ax, 0c600h                     ;Not sure what this memory refers to docs say PGA communication area, but that's probably wrong
    mov  ds, ax
    mov  ah, byte ptr ds:[03d4h]        ;Offset 03d4h
    mov  byte ptr ds:[03d4h], 28h       ;Offset 03d4h
    mov  dx, CRTControllerIndexD        ;port - 0x3d4
    in   al, dx
    mov  byte ptr ds:[03d4h], ah        ;Offset 03d4h
    pop  ds
    cmp  al, 28h                        ;Check if port 0x3d4 is memory mapped?
    jne  Exit                           ;Offset 0x16c1
    mov  byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], 0fh;Offset 048ah
Exit:                                   ;Offset 0x16c1
    ret  
ConfigureDisplayCombinationCode ENDP

;Looks to be testing the address range of the cursor?
;
TestCursorAddressWidth PROC NEAR        ;Offset 0x16c2
    mov  bl, 0aah
Label0x16c4:                            ;Offset 0x16c4
    mov  bh, bl
    and  bh, 0fh
    mov  al, 0eh                        ;CRE - Cursor Location High
    mov  ah, bl                         ;Write 0xaa, 0x55 or 0x00
    out  dx, ax                         ;
    inc  al                             ;CRF - Cursor Location Low
    out  dx, ax
    mov  al, 0eh                        ;CRE - Cursor Location High
    call ReadDataWithIndexRegisterNext  ;Offset 0x4649
    and  ah, 0fh
    DB 03Ah, 0E7h                       ;cmp  ah, bh - masm uses a different encoding.
    jne  Label0x16ee                    ;Offset 0x16ee
    call ReadDataWithIndexRegisterNext  ;Offset 0x4649
    and  ah, 0fh
    DB 03Ah, 0E7h                       ;cmp  ah, bh - masm uses a different encoding.
    jne  Label0x16ee                    ;Offset 0x16ee
    sub  bl, 55h
    jae  Label0x16c4                    ;Offset 0x16c4
    xor  bl, bl
Label0x16ee:                            ;Offset 0x16ee
    ret
TestCursorAddressWidth ENDP

ConfigureColorOrMonochrome PROC NEAR    ;Offset 0x16ef
    push ax
    mov  dx, CRTControllerIndexB        ;port - 0x3b4
    call TestCursorAddressWidth         ;Offset 0x16c2
    je   ColorConfigured                ;Offset 0x1737
    mov  dx, MiscellaneousWrite         ;port - 0x3c2
    mov  al, 0a6h                       ;bit 0 = 0 - Monochrome
                                        ;bit 1 = 1 - Enable CPU Display Memory Access
                                        ;bit 3-2 = 01 - Select 28.322 MHz Clock for 720 Horizontal pixels
                                        ;bit 5 = 1 - Select High 64k page
                                        ;bit 6 = 1 - Select negative horizontal retrace sync pulse
    out  dx, al
    mov  dx, CRTControllerIndexD        ;port - 0x3d4
    call TestCursorAddressWidth         ;Offset 0x16c2
    je   MonochromeConfigured           ;Offset 0x1724
    mov  dx, MiscellaneousWrite         ;port - 0x3c2
    mov  al, 23h                        ;bit 0 = 1 - Color
                                        ;bit 1 = 1 - Enable CPU Display Memory Access
                                        ;bit 3-2 = 00 - Select 25.175 MHz Clock for 640 Horizontal pixels
                                        ;bit 5 = 1 - Select High 64k page
                                        ;bit 6 = 0 - Select positive horizontal retrace sync pulse
    out  dx, al
    mov  dx, CRTControllerIndexD        ;port - 0x3d4
    mov  word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0463h
    and  byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0410h
    or   byte ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Color;Offset 0410h, 0x20
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0489h, 0x01
    pop  ax
    ret
MonochromeConfigured:                   ;Offset 0x1724
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    or   byte ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Monochrome;Offset 0410h, 0x30
    mov  dx, CRTControllerIndexB        ;port - 0x3b4
    mov  word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0463h
    pop  ax
    ret
ColorConfigured:                        ;Offset 0x1737
    and  byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0410h
    or   byte ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Color;Offset 0410h, 0x20
    mov  dx, CRTControllerIndexD        ;port - 0x3d4
    mov  word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0463h
    pop  ax
    ret
ConfigureColorOrMonochrome ENDP

SetupPointersAndInterruptHandlers PROC NEAR
    cli   
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    mov   bx, 0f000h                    ;System BIOS Segment
    mov   di, 0108h                     ;0x108 / 4 = 0x42 = VIDEO - RELOCATED DEFAULT INT 10 VIDEO SERVICES (EGA,VGA)
    mov   ax, 0f065h                    ;Pointer into System BIOS
    call  StoreFARPointer               ;Offset 0x17c4
    mov   bx, cs
    mov   di, 01b4h                     ;0x1b4 / 4 = 0x6d = VIDEO - many VGA - VIDEO BIOS ENTRY POINT
    mov   ax, InterruptHandlerEntrypoint;Offset 0x56c6
    call  StoreFARPointer               ;Offset 0x17c4
    mov   di, 40h                       ;0x40 / 4 = 0x10 = VIDEO BIOS Interrupt
    mov   ax, InterruptHandlerEntrypoint;Offset 0x56c6
    call  StoreFARPointer               ;Offset 0x17c4
    cli   
    mov   es, word ptr cs:[DSSegment]   ;Offset 0x1486
    mov   ax, cs
    mov   word ptr ds:[BDA_VideoParameterControlBlockPointer], offset VideoOverrideTable1;Offset 0x4a8, 0x3a09
    cmp   ax, 0c000h                    ;If we are in segment 0xC000, we can skip patching.
    je    LivingInROM                   ;Offset 0x17ac
    cmp   ax, 0e000h
    jne   LivingInMemory                ;Offset 0x1790
    mov   word ptr ds:[BDA_VideoParameterControlBlockPointer], offset VideoOverrideTable2;Offset 0x4a8, 0x3a25
    jmp   LivingInROM                   ;Offset 0x17ac
LivingInMemory:                         ;Offset 0x1790
    push  cx
    push  bx
    mov   cx, 0008h                     ;Patch code segment into potentially 8 pointers
    mov   bx, offset Data01A1           ;Offset 0x1a1
Label0x1798:                            ;Offset 0x1798
    push  bx
    mov   bx, word ptr cs:[bx]          ;Load pointer
    cmp   bx, 0000h                     ;If null, skip
    je    Done                          ;Offset 0x17a4
    mov   word ptr cs:[bx], cs          ;Store code segment at that address
Done:                                   ;Offset 0x17a4
    pop   bx
    add   bx, 0002h
    loop  Label0x1798                   ;Offset 0x1798
    pop   bx
    pop   cx
LivingInROM:                            ;Offset 0x17ac
    mov   word ptr ds:[04aah], cs       ;Offset 0x4aa
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    mov   di, 007ch                     ;0x7c / 4 = 0x1f - SYSTEM DATA - 8x8 GRAPHICS FONT
    mov   ax, offset UpperCharacters8x8 ;Offset 0x5b20
    call  StoreFARPointer               ;Offset 0x17c4
    mov   di, 010ch                     ;0x10c / 4 = 0x43 - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
    mov   ax, offset LowerCharacters8x8 ;Offset 0x5720
    ;No ret! We fall through to the next function
SetupPointersAndInterruptHandlers ENDP

;DON'T MOVE!
;inputs:
;es:di point to address where to store FAR pointer
;ax = offset
;bx = segment (is this the correct order?)
StoreFARPointer PROC NEAR               ;Offset 0x17c4
    stosw                               ;Store offset
    mov   ax, bx                        ;
    stosw                               ;Store segment
    ret
StoreFARPointer ENDP

ReadEGAFeatureBitSwitches PROC NEAR     ;Offset 0x17c9
    xor   ah, ah
    mov   cx, 0202h
    mov   dx, FeatureControlWD          ;port - 0x3da
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    je    Label0x17db                   ;Offset 0x17db
    mov   dx, FeatureControlWB          ;port - 0x3ba
Label0x17db:                            ;Offset 0x17db
    rol   ah, cl
    mov   al, ch                        ;Write bits 1-0 - Unknown.
    out   dx, al
    push  dx
    mov   dx, InputStatus0              ;port - 0x3c2
    in    al, dx
    pop   dx
    and   al, 60h                       ;Test bits 6-5 - Unknown.
    or    ah, al
    shr   ch, 01h
    jae   Label0x17db                   ;Offset 0x17db
    xor   al, al
    out   dx, al                        ;Enable normal vertical sync output to the monitor
    clc
    mov   al, ah
    ror   al, 01h
    and   al, 0f0h
    ret
ReadEGAFeatureBitSwitches ENDP

PrepareAttributeControllerAndResetNonUsedModeControl PROC NEAR;Offset 0x17f9
    call PrepareAttributeController     ;Offset 0x47ae
    push dx
    mov  dx, CGAModeControl             ;port - 0x3d8
    xor  al, al                         ;al = 0 - reset everything
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    jne  IsMonochrome                   ;Offset 0x180e
    mov  dx, MDAModeControl             ;0x3b8
    inc  al                             ;al = 1 - High resolution mode
IsMonochrome:                           ;Offset 0x180e
    out  dx, al
    pop  dx
    ret
PrepareAttributeControllerAndResetNonUsedModeControl ENDP

;Offset 0x1811
Mode0_7ControlRegValue  DB BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                                         ;2Ch ;Mode 0: 101100
                        DB BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                                                                 ;28h ;Mode 1; 101000
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                 ;2Dh ;Mode 2; 101101
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                                         ;29h ;Mode 3; 101001
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                                     ;2Ah ;Mode 4; 101010
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking             ;2Eh ;Mode 5; 101110
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_GraphicsOperation    ;1Eh ;Mode 6; 011110
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking                                         ;29h ;Mode 7; 101001

;inputs:
;al = requested video mode
SetVideoMode PROC NEAR                  ;Offset 0x1819
    call UnlockExtendedCRTRegistersSafe ;Offset 0x1374
    jne  Failure                        ;Offset 0x182b
    mov  ah, al                         ;al = requested video mode
    and  al, NOT INT10_00_xx_DontClearDisplay;0x7f
    cmp  al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
    jbe  ValidVideoMode                 ;Offset 0x182c
    call FindVideoModeData              ;Offset 0x103a
    je   ValidVideoMode                 ;Offset 0x182c
Failure:                                ;Offset 0x182b
    ret
ValidVideoMode:                         ;Offset 0x182c
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_DontClearDisplay;Offset 0x487, 0x7f
    and  ah, INT10_00_xx_DontClearDisplay;0x80
    or   byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    call ModeSetBDA                     ;Offset 0x18e8
    cmp  al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
    ja   IsVESAMode                     ;Offset 0x1850
    call Func0x1a4e                     ;Offset 0x1a4e - This function seems to destroy al?
    je   IsVESAMode                     ;Offset 0x1850
    and  al, 0dfh
    mov  byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    call EmptyFunction3                 ;Offset 0x1380
    call SetSystemAdapterVideoMode      ;Offset 0x1acc
    ret
IsVESAMode:                             ;Offset 0x1850
    push cx
    mov  byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    call SetTextModeBiosData            ;Offset 0x1b05
    and  byte ptr ds:[BDA_VideoModeOptions], 0f3h;Offset 0x487
    call SaveDynamicParameterData       ;Offset 0x1bac
    call ApplyVideoParameters           ;Offset 0x4829
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call ApplyVESAOverrideData          ;Offset 0x1079
    call Func0x4909                     ;Offset 0x4909
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  al, BDA_DM_320x200_4_Color_Graphics1;0x04
    jb   Label0x1885                    ;Offset 0x1885
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x07
    je   Label0x1885                    ;Offset 0x1885
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    jbe  Label0x18b5                    ;Offset 0x18b5
    call GetVideoModeFlags              ;Offset 0x105d
    test al, 01h
    je   Label0x18b5                    ;Offset 0x18b5
Label0x1885:                            ;Offset 0x1885
    call Func0x1c65                     ;Offset 0x1c65
    mov  ax, 000bh                      ;ax = sub index to mode list
    mov  bx, 0008h                      ;Fetch element 3 - Alphanumeric Character Set Override pointer
    call FetchCheckedVideoParameterBlockElement;Offset 0x1bc9
    jne  CharacterSetNotFound           ;Offset 0x1896
    call Func0x1bea                     ;Offset 0x1bea
CharacterSetNotFound:                   ;Offset 0x1896
    mov  bx, 0010h                      ;Secondary Save Pointer Table pointer (VGA)
    call GetVideoParameterBlockElement  ;Offset 0x1d95
    je   Label0x18c9                    ;Offset 0x18c9
    les  bx, es:[bx + 06h]              ;Pointer to Character font definition table
    mov  ax, es
    or   ax, bx
    je   Label0x18c9                    ;Offset 0x18c9
    mov  ax, 0007h
    call Func0x1bd1                     ;Offset 0x1bd1
    jne  Label0x18c9                    ;Offset 0x18c9
    call Func0x1c19                     ;Offset 0x1c19
    jmp  Label0x18c9                    ;Offset 0x18c9
Label0x18b5:                            ;Offset 0x18b5
    mov  word ptr ds:[BDA_CursorEndScanLine], 00h;Offset 0x460
    mov  ax, 0007h                      ;ax = sub index to mode list
    mov  bx, 000ch                      ;Fetch element 4 - Graphics Character Set Override pointer
    call FetchCheckedVideoParameterBlockElement;Offset 0x1bc9
    jne  Label0x18c9                    ;Offset 0x18c9
    call SetGraphicsCharacterFont       ;Offset 0x1c8e
Label0x18c9:                            ;Offset 0x18c9
    test byte ptr ds:[BDA_VideoModeOptions], 80h;Offset 0x487
    jne  Label0x18da                    ;Offset 0x18da
    mov  ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    or   ax, ax
    je   Label0x18da                    ;Offset 0x18da
    call Func0x1d47                     ;Offset 0x1d47
Label0x18da:                            ;Offset 0x18da
    call Func0x1cb4                     ;Offset 0x1cb4
    call EnablePaletteBasedVideo        ;Offset 0x479d
    call TurnOnScreen                   ;Offset 0x4800
    pop  cx
    call EmptyFunction3                 ;Offset 0x1380
    ret  
SetVideoMode ENDP

;inputs:
;al = video mode
ModeSetBDA PROC NEAR                    ;Offset 0x18e8
    mov  bl, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  bl, BDA_DH_InitialVideoModeMask;0x30
    test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489, 0x1
    je   Exit                           ;Offset 0x1944 - not in VGA mode.
    cmp  byte ptr ds:[BDA_DisplayMode], al;Offset 0x449 - Mode already set.
    je   Exit                           ;Offset 0x1944 - already in this video mode
    mov  ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and  ah, BDA_EFBS_AdapterTypeMask   ;0xf
    cmp  al, INT10_00_07_T_80x25_9x16_720x400_M_x_B000;0x7
    je   Label0x1983                    ;Offset 0x1983
    cmp  al, INT10_00_0F_G_80x25_8x14_640x350_M_2_A000;0xf
    je   Label0x1983                    ;Offset 0x1983
    mov  bh, al                         ;inputs: al = video mode, outputs: al = flags, zero = 0 success
    call GetVideoModeFlags              ;Offset 0x105d
    jne  NotFound                       ;Offset 0x1918
    test al, 02h                        ;test bit 1
    mov  al, bh                         ;al = video mode
    je   Label0x1983                    ;Offset 0x1983 - if not set, jump
NotFound:                               ;Offset 0x1918
    test byte ptr cs:[Data0197], 01h    ;Offset 0x197
    je   MonochromeMode                 ;Offset 0x1923
    jmp  Label0x19b7                    ;Offset 0x19b7
MonochromeMode:                         ;Offset 0x1923
    cmp  word ptr ds:[BDA_VideoBaseIOPort], CRTControllerIndexD;Offset 0x463, port - 0x3d4
    je   Exit                           ;Offset 0x1944 - We are operating in color mode. leave.
    cmp  bl, BDA_DH_80x25Monochrome     ;0x30
    je   Label0x198e                    ;Offset 0x198e
    cmp  ah, BDA_EFBS_CGAMono80x25      ;0x5
    jbe  Exit                           ;Offset 0x1944
    cmp  ah, BDA_EFBS_MDAHiRes80x25_2   ;0x8
    jbe  Label0x1949                    ;Offset 0x1949
    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
    je   Label0x194e                    ;Offset 0x194e
    cmp  ah, BDA_EFBS_CGAMono80x25_2    ;0xb
    jbe  Label0x1945                    ;Offset 0x1945
Exit:                                   ;Offset 0x1944
    ret
Label0x1945:                            ;Offset 0x1945
    mov  bl, BDA_EFBS_FeatureConnector0 OR BDA_EFBS_MDAHiRes80x25_2;0x48
    jmp  Label0x1950                    ;Offset 0x1950
Label0x1949:                            ;Offset 0x1949
    mov  bl, BDA_EFBS_FeatureConnector1 OR BDA_EFBS_CGAMono80x25_2;0x8b
    jmp  Label0x1950                    ;Offset 0x1950
    nop
Label0x194e:                            ;Offset 0x194e
    mov  bl, BDA_EFBS_CGAMono80x25_2    ;0xb
Label0x1950:                            ;Offset 0x1950
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487, 0xfd
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], 0f0h;Offset 0x488
    mov  ah, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    not  ah
    and  ah, BDA_VDDA_LineMode200;0x80
    rol  ah, 01h;rotate top bit in to first bit - BDA_EFBS_MDAColor80x25
    or   ah, bl
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], ah;Offset 0x488
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
    test byte ptr cs:[Data0197], 01h    ;Offset 0x197
    je   Exit                           ;Offset 0x1944
Label0x1978:                            ;Offset 0x1978
    and  byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410, 0x0cf
    or   word ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Color;Offset 0x410, 0x20
    ret
Label0x1983:                            ;Offset 0x1983 
    test byte ptr cs:[Data0197], 01h    ;Offset 0x197
    je   Label0x1992                    ;Offset 0x1992
    jmp  Label0x1a17                    ;Offset 0x1a17
Label0x198e:                            ;Offset 0x198e
    mov  al, 07h
    jmp  Exit                           ;Offset 0x1944
Label0x1992:                            ;Offset 0x1992
    cmp  word ptr ds:[BDA_VideoBaseIOPort], CRTControllerIndexB;Offset 0x463, port - 0x3b4
    je   Label0x1a16                    ;Offset 0x1a16
    cmp  bl, 30h
    jne  Label0x19d8                    ;Offset 0x19d8
    cmp  ah, BDA_EFBS_CGAMono80x25      ;0x5
    jbe  Label0x19dc                    ;Offset 0x19dc
    cmp  ah, BDA_EFBS_MDAHiRes80x25_2   ;0x8
    jbe  Label0x19e3                    ;Offset 0x19e3
    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
    je   Label0x1a01                    ;Offset 0x1a01
    cmp  ah, BDA_EFBS_CGAMono80x25_2    ;0xb
    ja   Label0x1a16                    ;Offset 0x1a16
    mov  bl, 08h
    jmp  Label0x1a03                    ;Offset 0x1a03
Label0x19b7:                            ;Offset 0x19b7
    cmp  word ptr ds:[BDA_VideoBaseIOPort], CRTControllerIndexD;Offset 0x463, port - 0x3d4
    je   Label0x1978                    ;Offset 0x1978
    cmp  ah, BDA_EFBS_CGAMono80x25      ;0x5
    jbe  Label0x19d3                    ;Offset 0x19d3
    cmp  ah, BDA_EFBS_MDAHiRes80x25_2   ;0x8
    jbe  Label0x19e3                    ;Offset 0x19e3
    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
    je   Label0x1a34                    ;Offset 0x1a34
    cmp  ah, BDA_EFBS_CGAMono80x25_2    ;0bh
    ja   Label0x1a16                    ;Offset 0x1a16
Label0x19d3:                            ;Offset 0x19d3
    mov  bl, 08h
    jmp  Label0x1950                    ;Offset 0x1950
Label0x19d8:                            ;Offset 0x19d8
    mov  al, 00h
    jmp  Label0x1a16                    ;Offset 0x1a16
Label0x19dc:                            ;Offset 0x19dc
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    jmp  Label0x1a16                    ;Offset 0x1a16
Label0x19e3:                            ;Offset 0x19e3
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x489, 0x80
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
    test byte ptr cs:[Data0197], 01h    ;Offset 0x197
    je   Label0x1a16                    ;Offset 0x1a16
    jmp  Label0x1a48                    ;Offset 0x1a48
Label0x1a01:                            ;Offset 0x1a01
    mov  bl, 0bh
Label0x1a03:                            ;Offset 0x1a03
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
Label0x1a16:                            ;Offset 0x1a16
    ret
Label0x1a17:                            ;Offset 0x1a17
    cmp  word ptr ds:[BDA_VideoBaseIOPort], CRTControllerIndexB;Offset 0x463, port - 0x3b4
    je   Label0x1a48                    ;Offset 0x1a48
    cmp  ah, BDA_EFBS_CGAMono80x25      ;0x5
    jbe  Label0x1a34                    ;Offset 0x1a34
    cmp  ah, BDA_EFBS_MDAHiRes80x25_2   ;0x8
    jbe  Label0x19e3                    ;Offset 0x19e3
    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
    je   Label0x1a34                    ;Offset 0x1a34
    cmp  ah, BDA_EFBS_CGAMono80x25_2    ;0xb
    jbe  Label0x19d3                    ;Offset 0x19d3
    ret
Label0x1a34:                            ;Offset 0x1a34
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
Label0x1a48:                            ;Offset 0x1a48
    or   word ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Monochrome;Offset 0x410, 0x30
    ret  
ModeSetBDA ENDP

;inputs:
;al = video mode
Func0x1a4e PROC NEAR                    ;Offset 0x1a4e
    mov  ah, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  ah, BDA_DH_InitialVideoModeMask;0x30
    cmp  ah, BDA_DH_80x25Monochrome     ;0x30
    jne  IsColor                        ;Offset 0x1a6a
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    jne  IsMonochrome2                  ;Offset 0x1aad
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487, 0x8
    mov  al, 0eh                        ;al = PointHeightOfCharacterMatrix 0xe = 14
    jmp  Label0x1a84                    ;Offset 0x1a84
IsColor:                                ;Offset 0x1a6a
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
    je   IsMonochrome1                  ;Offset 0x1a8f
    mov  ah, BDA_VMO_Inactive           ;0x8
    cmp  al, 02h
    jbe  Label0x1a7e                    ;Offset 0x1a7e
    cmp  al, 04h
    jae  Label0x1a7e                    ;Offset 0x1a7e
    or   ah, BDA_VMO_Unknown            ;0x4
Label0x1a7e:                            ;Offset 0x1a7e
    or   byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    mov  al, 08h                        ; al = PointHeightOfCharacterMatrix 0x8 = 8
Label0x1a84:                            ;Offset 0x1a84
    mov  byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484, 0x18 = 24
    mov  ah, 00h
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    ret
IsMonochrome1:                          ;Offset 0x1a8f
    cmp  al, 0fh
    je   Label0x1aa4                    ;Offset 0x1aa4
    cmp  al, 07h
    je   Label0x1aa4                    ;Offset 0x1aa4
    mov  bh, al
    call GetVideoModeFlags              ;Offset 0x105d
    jne  Exit                           ;Offset 0x1ac9
    test al, 02h
    mov  al, bh
    jne  Exit                           ;Offset 0x1ac9
Label0x1aa4:                            ;Offset 0x1aa4
    mov  al, 00h
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_DontClearDisplay;Offset 0x487, 0x7f
    jmp  Exit                           ;Offset 0x1ac9
IsMonochrome2:                          ;Offset 0x1aad
    cmp  al, 0fh
    je   Exit                           ;Offset 0x1ac9
    cmp  al, 07h
    je   Exit                           ;Offset 0x1ac9
    mov  bh, al
    call GetVideoModeFlags              ;Offset 0x105d
    jne  NotFound                       ;Offset 0x1ac2
    test al, 02h
    mov  al, bh
    je   Exit                           ;Offset 0x1ac9
NotFound:                               ;Offset 0x1ac2
    mov  al, 07h
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_DontClearDisplay;Offset 0x487, 0x7f
Exit:                                   ;Offset 0x1ac9
    DB 03Ah, 0C0h                       ;cmp  al, al - masm encoding difference
    ret
Func0x1a4e ENDP

SetSystemAdapterVideoMode PROC NEAR     ;Offset 0x1acc
    push  ds
    push  bp
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    cli
    mov   word ptr ds:[010ch], offset LowerCharacters8x8;Offset 0x10c, Offset 0x5720 - 0x10c / 4 = 0x43 - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
    mov   word ptr ds:[010eh], cs       ;Offset 0x10e
    sti
    mov   ds, word ptr ss:[bp]
    mov   ax, word ptr ss:[bp + 10h]
    mov   dx, word ptr ss:[bp + 0ah]
    mov   si, word ptr ss:[bp + 08h]
    mov   bp, word ptr ss:[bp + 04h]
    int   42h
    pop   bp
    pop   ds
    mov   word ptr ss:[bp + 10h], ax
    mov   al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and   al, BDA_DH_InitialVideoModeMask;0x30
    cmp   al, BDA_DH_80x25Monochrome    ;0x30
    jne   Exit                          ;Offset 0x1b04
    mov   word ptr ds:[BDA_CursorEndScanLine], 0b0ch;Offset 0x460 - set cursor start and end
Exit:                                   ;Offset 0x1b04
    ret
SetSystemAdapterVideoMode ENDP

SetTextModeBiosData PROC NEAR           ;Offset 0x1b05
    mov       es, word ptr cs:[Data1488];Offset 0x1488
    mov       di, 010ch                 ;int 43 - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
    cmp       al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
    jbe       LegacyMode                ;Offset 0x1b2d
    call      GetVideoModeFlags         ;Offset 0x105d
    jne       LegacyMode                ;Offset 0x1b2d
    mov       bx, offset LowerCharacters8x8;Offset 0x5720
    test      al, 01h
    jne       CharMapSelected           ;Offset 0x1b4b
    and       al, 70h
    je        CharMapSelected           ;Offset 0x1b4b
    mov       bx, offset Characters8x14 ;Offset 0x5f20
    cmp       al, 20h
    jbe       CharMapSelected           ;Offset 0x1b4b
    mov       bx, offset Characters8x16 ;Offset 0x6e30
    jmp       CharMapSelected           ;Offset 0x1b4b
LegacyMode:                             ;Offset 0x1b2d
    mov       bx, offset LowerCharacters8x8;Offset 0x5720
    cmp       al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
    je        CharMapSelected           ;Offset 0x1b4b
    cmp       al, INT10_00_08_Unknown   ;0x08
    jb        CharMapSelected           ;Offset 0x1b4b
    mov       bx, offset Characters8x16 ;Offset 0x6e30
    je        CharMapSelected           ;Offset 0x1b4b
    cmp       al, INT10_00_11_G_80x30_8x16_640x480_M_x_A000;0x11
    jae       CharMapSelected           ;Offset 0x1b4b
    mov       bx, offset Characters8x14 ;Offset 0x5f20
    cmp       al, INT10_00_0F_G_80x25_8x14_640x350_M_2_A000;0x0f
    jae       CharMapSelected           ;Offset 0x1b4b
    mov       bx, offset LowerCharacters8x8;Offset 0x5720
CharMapSelected:                        ;Offset 0x1b4b
    mov       ax, bx
    cli
    stosw     word ptr es:[di]
    mov       ax, cs
    stosw     word ptr es:[di]
    sti
    mov       ax, ds
    mov       es, ax
    mov       di, offset DBA_CursorPositionPage0;Offset 0x450
    mov       cx, 0008h
    xor       ax, ax
    rep stosw word ptr es:[di]          ;reset all cursor positions
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x07
    ja        NotCGA                    ;Offset 0x1b86
    xor       ah, ah                    ;reset ah
    mov       di, ax                    ;di is display mode
    mov       al, 3fh                   ;CGA color palette
                                        ;bit 5 = 1 - Mode 5 foreground colors - cyan/magenta/white
                                        ;bit 4 = 1 - Background color intensified
                                        ;bit 3 = 1 - Intensified border color (mode 2) and background color (mode 5)
                                        ;bit 2 = 1 - Red
                                        ;bit 1 = 1 - Green
                                        ;bit 0 = 1 - Blue
    cmp       di, BDA_DM_640x200_BW_Graphics;0x06
    je        IsMode6                   ;Offset 0x1b7b
    mov       al, 30h                   ;CGA color palette
                                        ;bit 5 = 1 - Mode 5 foreground colors - cyan/magenta/white
                                        ;bit 4 = 1 - Background color intensified
IsMode6:                                ;Offset 0x1b7b
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    mov       al, byte ptr cs:[di + Mode0_7ControlRegValue];Offset 0x1811
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
NotCGA:                                 ;Offset 0x1b86
    call      GetCurrentVideoModeOverrideTable;Offset 0x4a44
    push      si
    lodsb     byte ptr es:[si]
    xor       ah, ah
    mov       word ptr ds:[BDA_NumberOfScreenColumns], ax;Offset 0x44a
    lodsb     byte ptr es:[si]
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsb     byte ptr es:[si]
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw     word ptr es:[si]
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    add       si, 0fh
    lodsw     word ptr es:[si]
    xchg      al, ah
    mov       word ptr ds:[BDA_CursorEndScanLine], ax;Offset 0x460
    pop       si
    ret       
SetTextModeBiosData ENDP

SaveDynamicParameterData PROC NEAR      ;Offset 0x1bac
    push      ds
    push      es
    push      si
    push      es
    mov       bx, 04h                   ;Dynamic Parameter Save Area pointer (EGA, VGA)
    call      GetVideoParameterBlockElement;Offset 0x1d95
    pop       ds
    je        Exit                      ;Offset 0x1bc5
    add       si, 23h
    mov       di, bx
    mov       cx, 08h
    rep movsw word ptr es:[di], word ptr ds:[si]
    inc       si
    movsb     byte ptr es:[di], byte ptr ds:[si]
Exit:                                   ;Offset 0x1bc5
    pop       si
    pop       es
    pop       ds
    ret
SaveDynamicParameterData ENDP

;inputs:
;bx is index into video parameter table
;ax sub index into pulled table
;outputs:
;es:bx is new pointer - can be null
;al = 0xff if fail. unmodified otherwise.
;
;Fetches a pointer from the parameter block and then
;adds ax to it. after that, it runs through until 0xff or until current video mode found
;if found, it returns unmodified bx and al = video mode.
;if not found, it returns unmodified bx and al = 0xff
FetchCheckedVideoParameterBlockElement PROC NEAR;Offset 0x1bc9
    call GetVideoParameterBlockElement  ;Offset 0x1d95
    jne  Func0x1bd1                     ;Offset 0x1bd1
    or   al, 0ffh                       ;
    ret
FetchCheckedVideoParameterBlockElement ENDP

Func0x1bd1 PROC NEAR                    ;Offset 0x1bd1
    push bx
    add  bx, ax
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
Label0x1bd7:                            ;Offset 0x1bd7
    mov  ah, byte ptr es:[bx]
    inc  bx
    cmp  ah, 0ffh
    je   Label0x1be6                    ;Offset 0x1be6
    DB 03Ah, 0C4h                       ;cmp  ah, al - masm encoding difference
    jne  Label0x1bd7                    ;Offset 0x1bd7
    pop  bx
    ret
Label0x1be6:                            ;Offset 0x1be6
    or   al, 0ffh
    pop  bx
    ret
Func0x1bd1 ENDP

Func0x1bea PROC NEAR                    ;Offset 0x1bea
    mov  al, byte ptr es:[bx + 0ah]     ;number of character rows displayed
    push ax                             ;save that
    mov  cx, word ptr es:[bx + 02h]     ;number of characters defined
    mov  dx, word ptr es:[bx + 04h]     ;first character code in table
    mov  si, word ptr es:[bx + 06h]     ;offset to character font
    mov  ax, word ptr es:[bx + 08h]     ;segment to character font
    mov  bx, word ptr es:[bx]           ;bl = length of each character, bh = character generator ram bank
    xchg bl, bh                         ;bh = length of each character, bl = character generator ram bank
    and  bl, 3fh                        ;keep the lowest 6 bits
    mov  es, ax                         ;segment to character font
    mov  al, 10h                        ;
    call Func0x2f71                     ;Offset 0x2f71
    pop  ax
    cmp  al, 0ffh
    je   Label0x1c18                    ;Offset 0x1c18
    sub  al, 01h
    mov  byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x1c18:                            ;Offset 0x1c18
    ret
Func0x1bea ENDP

Func0x1c19 PROC NEAR                    ;Offset 0x1c19
    mov   al, byte ptr es:[bx]
    xor   ah, ah
    cmp   ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    jne   Label0x1c64                   ;Offset 0x1c64
    mov   cx, 100h
    xor   dx, dx
    mov   si, word ptr es:[bx + 03h]
    mov   ax, word ptr es:[bx + 05h]
    mov   bx, word ptr es:[bx]
    xchg  bl, bh
    and   bl, 3fh
    mov   es, ax
    mov   al, 00h
    push  bx
    call  Func0x2f71                    ;Offset 0x2f71
    mov   dx, 03c4h                     ;port - 0x3c4
    mov   al, 03h
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    and   ah, 13h
    pop   bx
    mov   bh, bl
    and   bl, 03h
    mov   cl, 02h
    shl   bl, cl
    and   bh, 04h
    inc   cl
    shl   bh, cl
    or    ah, bl
    or    ah, bh
    mov   al, 03h
    out   dx, ax
Label0x1c64:                            ;Offset 0x1c64
    ret
Func0x1c19 ENDP

Func0x1c65 PROC NEAR                    ;Offset 0x1c65
    xor   bl, bl
    mov   al, 01h
    cmp   word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 8x14
    je    Is8x14                        ;Offset 0x1c80
    mov   al, 02h
    cmp   word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485 8x8
    je    Label0x1c8a                   ;Offset 0x1c8a
    mov   al, 04h
    or    bl, 40h
    jmp   Label0x1c8a                   ;Offset 0x1c8a
Is8x14:                                 ;Offset 0x1c80
    cmp   byte ptr ds:[BDA_DisplayMode], 07h;Offset 0x449
    jne   Label0x1c8a                   ;Offset 0x1c8a
    or    bl, 80h
Label0x1c8a:                            ;Offset 0x1c8a
    call  Func0x2f71                    ;Offset 0x2f71
    ret   
Func0x1c65 ENDP

SetGraphicsCharacterFont PROC NEAR      ;Offset 0x1c8e
    mov   al, byte ptr es:[bx]          ;number of displayed character rows
    dec   al
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    mov   ax, word ptr es:[bx + 01h]    ;length of character definition in bytes
    mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    mov   ax, word ptr es:[bx + 03h]    ;offset of character font
    mov   bx, word ptr es:[bx + 05h]    ;segment of character font
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    mov   di, 010ch                     ;Offset 0x10c int 0x43 - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
    cli
    stosw word ptr es:[di]
    mov   ax, bx
    stosw word ptr es:[di]
    sti
    ret
SetGraphicsCharacterFont ENDP

Func0x1cb4 PROC NEAR                    ;Offset 0x1cb4
    mov   bx, 10h                       ;Secondary Save Pointer Table pointer (VGA)
    call  GetVideoParameterBlockElement ;Offset 0x1d95
    je    Label0x1cce                   ;Offset 0x1cce
    les   bx, es:[bx + 0ah]
    mov   ax, es
    or    ax, bx
    je    Label0x1cce                   ;Offset 0x1cce
    mov   ax, 14h
    call  Func0x1bd1                    ;Offset 0x1bd1
    je    Label0x1ccf                   ;Offset 0x1ccf
Label0x1cce:                            ;Offset 0x1cce
    ret
Label0x1ccf:                            ;Offset 0x1ccf
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 08h;Offset 0x489
    jne   Label0x1d2b                   ;Offset 0x1d2b
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dx, 06h
    in    al, dx
    push  ds
    push  bx
    mov   ax, word ptr es:[bx + 0eh]
    mov   ah, al
    lds   si, es:[bx + 10h]
    mov   bx, word ptr es:[bx + 0ch]
    or    bx, bx
    je    Label0x1d06                   ;Offset 0x1d06
    mov   dx, 03c8h                     ;port - 0x3c8
Label0x1cf5:                            ;Offset 0x1cf5
    mov   al, ah
    out   dx, al
    inc   dx
    mov   cx, 03h
Label0x1cfc:                            ;Offset 0x1cfc
    lodsb byte ptr ds:[si]
    out   dx, al
    loop  Label0x1cfc                   ;Offset 0x1cfc
    inc   ah
    dec   dx
    dec   bx
    jne   Label0x1cf5                   ;Offset 0x1cf5
Label0x1d06:                            ;Offset 0x1d06
    pop   bx
    mov   ax, word ptr es:[bx + 06h]
    mov   ah, al
    lds   si, es:[bx + 08h]
    mov   cx, word ptr es:[bx + 04h]
    jcxz  Label0x1d2a                   ;Offset 0x1d2a
    mov   dx, 03c0h                     ;port - 0x3c0
Label0x1d1a:                            ;Offset 0x1d1a
    mov   al, ah
    out   dx, al
    lodsb byte ptr ds:[si]
    out   dx, al
    inc   ah
    loop  Label0x1d1a                   ;Offset 0x1d1a
    inc   ah
    mov   al, ah
    out   dx, al
    lodsb byte ptr ds:[si]
    out   dx, al
Label0x1d2a:                            ;Offset 0x1d2a
    pop   ds
Label0x1d2b:                            ;Offset 0x1d2b
    mov   al, byte ptr es:[bx]
    or    al, al
    je    Label0x1d46                   ;Offset 0x1d46
    test  al, 80h
    mov   al, 1fh
    jne   Label0x1d3d                   ;Offset 0x1d3d
    mov   ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec   al
Label0x1d3d:                            ;Offset 0x1d3d
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov   ah, al
    mov   al, 14h
    out   dx, ax
Label0x1d46:                            ;Offset 0x1d46
    ret
Func0x1cb4 ENDP 

Func0x1d47 PROC NEAR
    mov       cx, 4000h
    mov       bl, byte ptr ds:[BDA_DisplayMode];Offset 0449h
    cmp       bl, 13h
    jbe       Label0x1d70
    mov       al, bl
    call      GetVideoModeFlags
    jne       Label0x1d70
    mov       bl, al
    mov       bh, 0b8h
    mov       ax, 0720h
    test      bl, 02h
    jne       Label0x1d68
    mov       bh, 0b0h
Label0x1d68:
    test      bl, 01h
    jne       Label0x1d8c
    jmp       EmptyFunction2            ;Offset 0x121d Tail call.
Label0x1d70:                            ;Offset 0x1d70
    mov       bh, 0b0h
    mov       ax, 0720h
    cmp       bl, 07h
    je        Label0x1d8c
    mov       bh, 0b8h
    cmp       bl, 03h
    jbe       Label0x1d8c
    xor       ax, ax
    cmp       bl, 06h
    jbe       Label0x1d8c
    mov       bh, 0a0h
    mov       ch, 80h
Label0x1d8c:
    xor       bl, bl
    mov       es, bx
    xor       di, di
    rep       stosw
    ret
Func0x1d47 ENDP

;input:
;bx = offset into Video Parameter Control Block
GetVideoParameterBlockElement PROC NEAR
    push di
    les  di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8 load segment:offset of video parameter control block into es:di
    les  bx, es:[bx + di]               ;Load segment:offset from video override table into es:bx
    mov  di, es                         ;
    or   di, bx                         ;or together segment and offset (not null test?)
    pop  di
    ret                                 ;return, es:bx is now offset to table.
GetVideoParameterBlockElement ENDP

SetTextModeCursorShape PROC NEAR        ;Offset 0x1da3
    mov  word ptr ds:[BDA_CursorEndScanLine], cx;Offset 0x460
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    call SetTextModeCursorShapeInternal ;Offset 0x1db2
    mov  word ptr ss:[bp + 10h], ax
    ret
SetTextModeCursorShape ENDP

SetTextModeCursorShapeInternal PROC NEAR;Offset 0x1db2
    test byte ptr ds:[BDA_VideoModeOptions], 08h;Offset 0x487
    jne  Label0x1e2c                    ;Offset 0x1e2c
    mov  ax, cx
    and  ah, 60h
    cmp  ah, 20h
    jne  Label0x1dc8                    ;Offset 0x1dc8
    mov  cx, 1e00h
    jmp  Label0x1e2c                    ;Offset 0x1e2c
Label0x1dc8:                            ;Offset 0x1dc8
    test byte ptr ds:[BDA_VideoModeOptions], 01h;Offset 0x487
    jne  Label0x1e2c                    ;Offset 0x1e2c
    mov  ax, cx
    and  ax, 0e0e0h
    jne  Label0x1e2c                    ;Offset 0x1e2c
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  al, 04h
    jb   Label0x1dea                    ;Offset 0x1dea
    cmp  al, 07h
    je   Label0x1dea                    ;Offset 0x1dea
    call GetVideoModeFlags              ;Offset 0x105d
    jne  Label0x1e2c                    ;Offset 0x1e2c
    test al, 01h
    je   Label0x1e2c                    ;Offset 0x1e2c
Label0x1dea:                            ;Offset 0x1dea
    DB 03Ah, 0CDh                       ;cmp  ch, cl - masm encoding difference
    jae  Label0x1dfc                    ;Offset 0x1dfc
    or   cl, cl
    je   Label0x1e2c                    ;Offset 0x1e2c
    mov  ch, cl
    mov  cl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec  cl
    jmp  Label0x1e2c                    ;Offset 0x1e2c
Label0x1dfc:                            ;Offset 0x1dfc
    mov  bl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov  bh, bl
    dec  bl
    mov  al, cl
    or   al, ch
    DB 03Ah, 0C7h                       ;cmp  bh, al - masm encoding difference
    jae  Label0x1e18                    ;Offset 0x1e18
    DB 03Ah, 0CBh                       ;cmp  cl, bl - masm encoding difference
    je   Label0x1e2c                    ;Offset 0x1e2c
    dec  bl
    DB 03Ah, 0EBh                       ;cmp  ch, bl - masm encoding difference
    je   Label0x1e2c                    ;Offset 0x1e2c
    inc  bl
Label0x1e18:                            ;Offset 0x1e18
    cmp  cl, 03h
    jbe  Label0x1e2c                    ;Offset 0x1e2c
    mov  al, ch
    add  al, 02h
    DB 03Ah, 0C1h                       ;cmp  al, cl - masm encoding difference
    jae  Label0x1e3f                    ;Offset 0x1e3f
    cmp  ch, 02h
    ja   Label0x1e37                    ;Offset 0x1e37
    mov  cl, bl
Label0x1e2c:                            ;Offset 0x1e2c
    mov  ah, ch
    mov  al, 0ah
    out  dx, ax
    mov  ah, cl
    mov  al, 0bh
    out  dx, ax
    ret
Label0x1e37:                            ;Offset 0x1e37
    mov  cl, bl
    mov  ch, bh
    shr  ch, 01h
    jmp  Label0x1e2c                    ;Offset 0x1e2c
Label0x1e3f:                            ;Offset 0x1e3f
    cmp  bh, 0eh
    jb   Label0x1e46                    ;Offset 0x1e46
    dec  bl
Label0x1e46:                            ;Offset 0x1e46
    sub  ch, cl
    add  ch, bl
    mov  cl, bl
    jmp  Label0x1e2c                    ;Offset 0x1e2c
SetTextModeCursorShapeInternal ENDP

SetCursorPosition PROC NEAR             ;Offset 0x1e4e
    mov  al, bh
    xchg bl, bh
    xor  bh, bh
    shl  bx, 01h
    mov  word ptr ds:[bx + DBA_CursorPositionPage0], dx;Offset 0x450
    cmp  byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    jne  EmptyFunction5                 ;Offset 0x1e80
SetCursorPosition ENDP
;DON'T MOVE! Rolling through to the function below!

;DON'T MOVE! The function above rolls in
SetCursorPositionInternal PROC NEAR     ;Offset 0x1e60
    mov  al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul  dh
    add  al, dl
    adc  ah, 00h
    mov  bx, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    shr  bx, 01h
    add  bx, ax
    mov  al, 0eh
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov  ah, bh
    out  dx, ax
    mov  ah, bl
    inc  al
    out  dx, ax
SetCursorPositionInternal ENDP
;DON'T MOVE! Rolling through to the function below!

;DON'T MOVE! The function above rolls in
EmptyFunction5 PROC NEAR                ;Offset 0x1e80
    ret
EmptyFunction5 ENDP

GetCursorPositionAndSize PROC NEAR      ;Offset 0x1e81
    xchg bl, bh
    xor  bh, bh
    add  bx, bx
    mov  dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    mov  cx, word ptr ds:[BDA_CursorEndScanLine];Offset 0x460
    mov  word ptr ss:[bp + 0ch], cx
    mov  word ptr ss:[bp + 0ah], dx
    ret
GetCursorPositionAndSize ENDP

Function4Forward PROC NEAR              ;Offset 0x1e96
    test byte ptr ds:[BDA_VideoModeOptions], 08h;Offset 0x487
    je   DoNothing                      ;Offset 0x1eac
    int  42h                            ;System int 10h handler
    mov  word ptr [bp + 0ah], dx
    mov  word ptr [bp + 0ch], cx
    mov  word ptr [bp + 0eh], bx
    mov  word ptr [bp + 10h], ax
    ret
DoNothing:                              ;Offset 0x1eac
    xor  ax, ax
    mov  word ptr [bp + 10h], ax
    ret
Function4Forward ENDP

SelectActiveDisplayPage PROC NEAR       ;Offset 0x1eb2
    xor  ah, ah
    and  al, 07h
    mov  di, ax
    mov  byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mul  word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov  word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    mov  bx, ax
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  al, 07h
    jbe  Label0x1ed8                    ;Offset 0x1ed8
    cmp  al, 13h
    jbe  Label0x1eda                    ;Offset 0x1eda
    call GetVideoModeFlags              ;Offset 0x105d
    jne  Label0x1eda                    ;Offset 0x1eda
    test al, 01h
    je   Label0x1eda                    ;Offset 0x1eda
Label0x1ed8:                            ;Offset 0x1ed8
    shr  bx, 01h
Label0x1eda:                            ;Offset 0x1eda
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov  al, 0ch
    mov  ah, bh
    out  dx, ax
    mov  ah, bl
    inc  al
    out  dx, ax
    shl  di, 01h
    mov  dx, word ptr ds:[di + DBA_CursorPositionPage0];Offset 0x450
    jmp  SetCursorPositionInternal      ;Offset 0x1e60
SelectActiveDisplayPage ENDP

ScrollUpWindow PROC NEAR                ;Offset 0x1ef1
    sub  sp, 04h
    mov  bp, sp
    mov  word ptr ss:[bp], ax
    mov  word ptr ss:[bp + 2], bx
    mov  ax, cx
    cmp  byte ptr ds:[BDA_DisplayMode], 0ch;Offset 0x449
    jbe  Func0x1f79                     ;Offset 0x1f79
    cmp  byte ptr ds:[BDA_DisplayMode], 13h;Offset 0x449
    je   Func0x1f11                     ;Offset 0x1f11
    ja   Func0x1f2d                     ;Offset 0x1f2d
ScrollUpWindow ENDP
;DON'T MOVE! Rolls in to function below!

;DON'T MOVE! Function above rolls in
Func0x1f0e PROC NEAR                    ;Offset 0x1f0e
    jmp  Func0x220b                     ;Offset 0x220b
Func0x1f0e ENDP

Func0x1f11 PROC NEAR                    ;Offset 0x1f11
    push dx
    mov  di, ax
    and  di, 0ffh
    mov  al, ah
    mul  byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul  word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add  di, ax
    shl  di, 03h
    pop  dx
    mov  ax, cx
    jmp  Func0x2339                     ;Offset 0x2339
Func0x1f11 ENDP

Func0x1f2d PROC NEAR                    ;Offset 0x1f2d
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      GetVideoModeFlags         ;Offset 0x105d
    jne       Func0x1f0e                ;Offset 0x1f0e
    test      al, 01h
    mov       bl, al
    mov       ax, cx
    jne       Func0x1f79                ;Offset 0x1f79
    jmp       Func0x1177                ;Offset 0x1177
Func0x1f2d ENDP

ScrollDownWindow PROC NEAR              ;Offset 0x1f40
    sub       sp, 04h
    mov       bp, sp
    mov       word ptr ss:[bp], ax
    mov       word ptr ss:[bp + 02h], bx
    mov       ax, cx
    cmp       byte ptr ds:[BDA_DisplayMode], 0ch;Offset 0x449
    jbe       Func0x1f76                ;Offset 0x1f76
    cmp       byte ptr ds:[BDA_DisplayMode], 13h;Offset 0x449
    je        Func0x1f60                ;Offset 0x1f60
    ja        Func0x1f63                ;Offset 0x1f63
ScrollDownWindow ENDP
;DON'T MOVE! Rolls in to function below!

;DON'T MOVE! Function above rolls in
Func0x1f5d PROC NEAR                    ;Offset 0x1f5d
   jmp       Func0x21d9                 ;Offset 0x21d9
Func0x1f5d ENDP
;DON'T MOVE! Rolls in to function below!

;DON'T MOVE! Function above rolls in
Func0x1f60 PROC NEAR                    ;Offset 0x1f60
    jmp       SetTextModeAndCheckMonitor0                ;Offset 0x2310
Func0x1f60 ENDP

Func0x1f63 PROC NEAR                    ;Offset 0x1f63
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      GetVideoModeFlags         ;Offset 0x105d
    jne       Func0x1f5d                ;Offset 0x1f5d
    test      al, 01h
    mov       bl, al
    mov       ax, cx
    jne       Func0x1f76                ;Offset 0x1f76
    jmp       Func0x116d                ;Offset 0x116d
Func0x1f63 ENDP

Func0x1f76 PROC NEAR                    ;Offset 0x1f76
    mov       cx, dx
    std
Func0x1f76 ENDP
;DON'T MOVE! Rolls in to function below!

;DON'T MOVE! Function above rolls in
Func0x1f79 PROC NEAR                    ;Offset 0x1f79
    mov       si, ax
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, 30h
    cmp       al, 30h
    mov       ax, 0b000h                ;Segment 0xb000
    je        Label0x1f89               ;Offset 0x1f89
    mov       ah, 0b8h                  ;Segment 0xb800
Label0x1f89:                            ;Offset 0x1f89
    mov       es, ax
    mov       ax, si
    sub       dl, al
    sub       dh, ah
    mov       al, byte ptr ss:[bp]
    inc       dl
    inc       dh
    mov       ah, dh
    or        al, al
    je        Label0x1fa2               ;Offset 0x1fa2
    sub       ah, al
    ja        Label0x1fa6               ;Offset 0x1fa6
Label0x1fa2:                            ;Offset 0x1fa2
    mov       al, dh
    xor       ah, ah
Label0x1fa6:                            ;Offset 0x1fa6
    push      ax
    mov       ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       ah, 03h
    jbe       Func0x1fbd                ;Offset 0x1fbd
    cmp       ah, 07h
    je        Func0x1fbd                ;Offset 0x1fbd
    cmp       ah, 13h
    ja        Func0x1fbd                ;Offset 0x1fbd
    jmp       Func0x209a                ;Offset 0x209a
Func0x1f79 ENDP

Func0x1fbd PROC NEAR                    ;Offset 0x1fbd
    mov       bl, al
    mov       al, ch
    mul       byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    add       al, cl
    adc       ah, 00h
    mov       di, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    shr       di, 01h
    add       di, ax
    mov       al, bl
    add       di, di
    mov       si, di
    mov       bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bl
    push      ax
    add       ax, ax
    add       bx, bx
    cmp       byte ptr ss:[bp + 01h], 06h
    je        Label0x1fed               ;Offset 0x1fed
    neg       ax
    neg       bx
Label0x1fed:                            ;Offset 0x1fed
    mov       cl, dl
    xor       ch, ch
    add       si, ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, 02h
    jb        Label0x2016               ;Offset 0x2016
    cmp       al, 03h
    ja        Label0x2016               ;Offset 0x2016
    test      byte ptr ds:[BDA_VideoModeOptions], 04h;Offset 0x487
    je        Label0x2016               ;Offset 0x2016
    mov       dx, 03dah                 ;port - 0x3da
Label0x2008:                            ;Offset 0x2008
    in        al, dx
    test      al, 08h
    je        Label0x2008               ;Offset 0x2008
    sub       dx, 02h
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    and       al, 0f7h
    out       dx, al
Label0x2016:                            ;Offset 0x2016
    cmp       cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       ax, es
    mov       ds, ax
    pop       ax
    pop       dx
    jne       Label0x2051               ;Offset 0x2051
    mov       bx, ax
    mov       al, cl
    mul       dh
    mov       cx, ax
    cmp       byte ptr ss:[bp + 01h], 07h
    jne       Label0x2034               ;Offset 0x2034
    dec       di
    dec       di
    dec       si
    dec       si
Label0x2034:                            ;Offset 0x2034
    shr       cx, 01h
    rep movsd dword ptr es:[di], dword ptr ds:[si]
    jae       Label0x203c               ;Offset 0x203c
    movsw     word ptr es:[di], word ptr ds:[si]
Label0x203c:                            ;Offset 0x203c
    cmp       byte ptr ss:[bp + 01h], 07h
    jne       Label0x2046               ;Offset 0x2046
    inc       di
    inc       di
    inc       si
    inc       si
Label0x2046:                            ;Offset 0x2046
    mov       cx, bx
    mov       al, 20h
    mov       ah, byte ptr ss:[bp + 03h]
    rep stosw word ptr es:[di]
    jmp       Label0x2078               ;Offset 0x2078
Label0x2051:                            ;Offset 0x2051
    or        dh, dh
    je        Label0x2067               ;Offset 0x2067
Label0x2055:                            ;Offset 0x2055
    push      cx
    push      si
    mov       ax, di
    rep movsw word ptr es:[di], word ptr ds:[si]
    mov       di, ax
    pop       si
    pop       cx
    add       si, bx
    add       di, bx
    dec       dh
    jne       Label0x2055               ;Offset 0x2055
Label0x2067:                            ;Offset 0x2067
    mov       al, 20h
    mov       ah, byte ptr ss:[bp + 03h]
Label0x206c:                            ;Offset 0x206c
    push      cx
    push      di
    rep stosw word ptr es:[di]
    pop       di
    pop       cx
    add       di, bx
    dec       dl
    jne       Label0x206c               ;Offset 0x206c
Label0x2078:                            ;Offset 0x2078
    mov       ds, word ptr cs:[DSSegment];Offset 0x1486
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, 02h
    jb        Exit                      ;Offset 0x2096
    cmp       al, 03h
    ja        Exit                      ;Offset 0x2096
    test      byte ptr ds:[BDA_VideoModeOptions], 04h;Offset 0x487
    je        Exit                      ;Offset 0x2096
    mov       dx, 03d8h                 ;port - 0x3d8
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    out       dx, al
Exit:                                   ;Offset 0x2096
    add       sp, 04h
    ret
Func0x1fbd ENDP

Func0x209a PROC NEAR                    ;Offset 0x209a
    cmp       ah, 06h
    je        Label0x20a3               ;Offset 0x20a3
    add       cl, cl
    add       dl, dl
Label0x20a3:                            ;Offset 0x20a3
    mov       si, dx
    mov       bl, al
    mov       al, ch
    xor       ah, ah
    mov       di, 140h
    mul       di
    add       al, cl
    adc       ah, 00h
    mov       di, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    add       di, ax
    mov       al, bl
    xor       ah, ah
    mov       dx, 140h
    mul       dx
    mov       dx, si
    push      ax
    mov       bx, 50h
    sub       bl, dl
    sbb       bh, 00h
    mov       ch, byte ptr [bp + 03h]
    cmp       byte ptr [bp + 01h], 07h
    mov       bp, 2000h
    je        Func0x2149                ;Offset 0x2149
    mov       cl, dl
    mov       si, di
    add       si, ax
    pop       ax
    pop       dx
    push      cx
    xor       ch, ch
    mov       ax, es
    mov       ds, ax
    or        dh, dh
    je        Label0x211d               ;Offset 0x211d
    shl       dh, 02h
    mov       ax, cx
Label0x20f3:                            ;Offset 0x20f3
    mov       cx, ax
    shr       cx, 01h
    rep movsw word ptr es:[di], word ptr ds:[si]
    rcl       cx, 01h
    rep movsb byte ptr es:[di], byte ptr ds:[si]
    sub       di, ax
    sub       si, ax
    add       si, bp
    add       di, bp
    mov       cx, ax
    shr       cx, 01h
    rep movsw word ptr es:[di], word ptr ds:[si]
    rcl       cx, 01h
    rep movsb byte ptr es:[di], byte ptr ds:[si]
    sub       si, bp
    sub       di, bp
    add       si, bx
    add       di, bx
    dec       dh
    jne       Label0x20f3               ;Offset 0x20f3
    mov       cx, ax
Label0x211d:                            ;Offset 0x211d
    pop       ax
    mov       al, ah
    shl       dl, 02h
    mov       si, cx
Label0x2125:                            ;Offset 0x2125
    mov       cx, si
    shr       cx, 01h
    rep stosw word ptr es:[di]
    rcl       cx, 01h
    rep stosb byte ptr es:[di]
    sub       di, si
    add       di, bp
    mov       cx, si
    shr       cx, 01h
    rep stosw word ptr es:[di]
    rcl       cx, 01h
    rep stosb byte ptr es:[di]
    sub       di, bp
    add       di, bx
    dec       dl
    jne       Label0x2125               ;Offset 0x2125
    add       sp, 04h
    ret       
Func0x209a ENDP

Func0x2149 PROC NEAR                    ;Offset 0x2149
    neg       ax
    neg       bx
    neg       bp
    add       di, 20f0h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449  06h
    je        Label0x215b               ;Offset 0x215b
    inc       di
Label0x215b:                            ;Offset 0x215b
    mov       cl, dl
    mov       si, di
    add       si, ax
    pop       ax
    pop       dx
    push      cx
    xor       ch, ch
    mov       ax, es
    mov       ds, ax
    or        dh, dh
    je        Label0x21a7               ;Offset 0x21a7
    shl       dh, 02h
    mov       ax, cx
Label0x2173:                            ;Offset 0x2173
    mov       cx, ax
    shr       cx, 01h
    jae       Label0x217a               ;Offset 0x217a
    movsb
Label0x217a:                            ;Offset 0x217a
    jcxz      Label0x2182               ;Offset 0x2182
    dec       si
    dec       di
    rep movsw
    inc       si
    inc       di
Label0x2182:                            ;Offset 0x2182
    add       si, ax
    add       di, ax
    add       si, bp
    add       di, bp
    mov       cx, ax
    shr       cx, 01h
    jae       Label0x2191               ;Offset 0x2191
    movsb
Label0x2191:                            ;Offset 0x2191
    jcxz      Label0x2199               ;Offset 0x2199
    dec       si
    dec       di
    rep movsw
    inc       si
    inc       di
Label0x2199:                            ;Offset 0x2199
    sub       si, bp
    sub       di, bp
    add       si, bx
    add       di, bx
    dec       dh
    jne       Label0x2173               ;Offset 0x2173
    mov       cx, ax
Label0x21a7:                            ;Offset 0x21a7
    pop       ax
    mov       al, ah
    shl       dl, 02h
    mov       si, cx
Label0x21af:                            ;Offset 0x21af
    mov       cx, si
    shr       cx, 01h
    jae       Label0x21b6               ;Offset 0x21b6
    stosb
Label0x21b6:                            ;Offset 0x21b6
    jcxz      Label0x21bc               ;Offset 0x21bc
    dec       di
    rep stosw
    inc       di
Label0x21bc:                            ;Offset 0x21bc
    add       di, si
    add       di, bp
    mov       cx, si
    shr       cx, 01h
    jae       Label0x21c7               ;Offset 0x21c7
    stosb
Label0x21c7:                            ;Offset 0x21c7
    jcxz      Label0x21cd               ;Offset 0x21cd
    dec       di
    rep stosw
    inc       di
Label0x21cd:                            ;Offset 0x21cd
    sub       di, bp
    add       di, bx
    dec       dl
    jne       Label0x21af               ;Offset 0x21af
    add       sp, 04h
    ret
Func0x2149 ENDP

Func0x21d9 PROC NEAR                    ;Offset 0x21d9
    std
    mov       ax, dx
    mov       si, dx
    mov       di, ax
    and       di, 0ffh
    mov       al, ah
    mul       byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add       di, ax
    xor       ah, ah
    mov       al, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add       di, ax
    mov       bx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       bx
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bx
    add       di, ax
    mov       dx, si
    jmp       Func0x222c                ;Offset 0x222c
Func0x21d9 ENDP

;inputs:
;ax = 
;cx = 
;dx = 
;outputs:
;di = 
;destroys:
;ax, si
Func0x220b PROC NEAR                    ;Offset 0x220b
    mov       si, dx                    ;save dx
    mov       di, ax
    and       di, 0ffh
    mov       al, ah
    mul       byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a   ax = al * number of screen columns
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485 dx:ax = ax * height of character
    add       di, ax    ;di = offset + al * number of screen columns * height of character
    xor       ah, ah
    mov       al, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c dx:ax = display number * video buffer size
    add       di, ax    ;di = display number * video buffer size + (offset + al * number of screen columns * height of character)
    mov       dx, si
Func0x220b ENDP

Func0x222c PROC NEAR                    ;Offset 0x222c
    sub       dx, cx
    inc       dh
    inc       dl
    mov       al, byte ptr ss:[bp]
    mov       ah, dh
    or        al, al
    je        Label0x223f               ;Offset 0x223f
    sub       ah, al
    ja        Label0x2243               ;Offset 0x2243
Label0x223f:                            ;Offset 0x223f
    mov       al, dh
    xor       ah, ah
Label0x2243:                            ;Offset 0x2243
    mov       cx, ax
    mov       si, dx
    mov       ax, 0105h
    mov       dx, 03ceh                 ;port - 0x3ce
    out       dx, ax
    mov       ax, 0f02h
    mov       dx, 03c4h                 ;port - 0x3c4
    out       dx, ax
    mov       ax, cx
    push      ax
    mov       bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bl
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       cx, ax
    mov       dx, si
    sub       bl, dl
    sbb       bh, 00h
    cmp       byte ptr ss:[bp + 01h], 07h
    jne       Label0x2275               ;Offset 0x2275
    neg       ax
    neg       bx
Label0x2275:                            ;Offset 0x2275
    mov       si, di
    add       si, ax
    mov       ax, cx
    mov       cl, dl
    xor       ch, ch
    pop       dx
    cmp       cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jne       Label0x22b9               ;Offset 0x22b9
    mov       bx, ax
    mov       al, cl
    mul       dh
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       cx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    rep movsb
    mov       cx, bx
    mov       dx, 03ceh                 ;port - 0x3ce
    mov       ax, 0005h
    out       dx, ax
    mov       ah, byte ptr ss:[bp + 03h]
    xor       al, al
    out       dx, ax
    inc       al
    out       dx, ax
    xor       ax, ax
    rep stosb
    out       dx, ax
    inc       al
    out       dx, ax
    add       sp, 04h
    ret
Label0x22b9:                            ;Offset 0x22b9
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       dh
    push      dx
    mov       dx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       es, ax
    or        dx, dx
    je        Label0x22d9               ;Offset 0x22d9
Label0x22cc:                            ;Offset 0x22cc
    mov       ax, cx
    rep movsb
    mov       cx, ax
    add       si, bx
    add       di, bx
    dec       dx
    jne       Label0x22cc               ;Offset 0x22cc
Label0x22d9:                            ;Offset 0x22d9
    mov       ds, word ptr cs:[DSSegment];Offset 0x1486
    pop       si
    mov       dx, 03ceh                 ;port - 0x3ce
    mov       ax, 05h
    out       dx, ax
    mov       ah, byte ptr ss:[bp + 03h]
    xor       al, al
    out       dx, ax
    inc       al
    out       dx, ax
    mov       dx, si
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       dl
    mov       dx, ax
    xor       ax, ax
Label0x22fa:                            ;Offset 0x22fa
    mov       si, cx
    rep stosb
    add       di, bx
    mov       cx, si
    dec       dx
    jne       Label0x22fa               ;Offset 0x22fa
    mov       dx, 03ceh                 ;port - 0x3ce
    out       dx, ax
    inc       al
    out       dx, ax
    add       sp, 04h
    ret
Func0x222c ENDP

SetTextModeAndCheckMonitor0 PROC NEAR                    ;Offset 0x2310
    std       
    mov       ax, dx
    push      dx
    mov       di, ax
    and       di, 0ffh
    mov       al, ah
    mul       byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add       di, ax
    mov       bx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       bx
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bx
    add       di, ax
    shl       di, 03h
    add       di, 06h
    pop       dx
SetTextModeAndCheckMonitor0 ENDP

Func0x2339 PROC NEAR                    ;Offset 0x2339
    sub       dx, cx
    inc       dh
    inc       dl
    mov       al, byte ptr ss:[bp]
    mov       ah, dh
    or        al, al
    je        Label0x234c               ;Offset 0x234c
    sub       ah, al
    ja        Label0x2350               ;Offset 0x2350
Label0x234c:                            ;Offset 0x234c
    mov       al, dh
    xor       ah, ah
Label0x2350:                            ;Offset 0x2350
    push      ax
    mov       si, dx
    mov       bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bl
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dx, si
    shl       ax, 03h
    push      ax
    sub       bl, dl
    sbb       bh, 00h
    cmp       byte ptr ss:[bp + 01h], 07h
    jne       Label0x2372               ;Offset 0x2372
    neg       ax
    neg       bx
Label0x2372:                            ;Offset 0x2372
    mov       cl, dl
    mov       ch, 00h
    mov       si, di
    add       si, ax
    pop       ax
    pop       dx
    cmp       cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jne       Label0x23a9               ;Offset 0x23a9
    mov       bx, ax
    mov       al, cl
    mul       dh
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       cx, ax
    shl       cx, 02h
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    rep movsw
    mov       cx, bx
    shr       cx, 01h
    mov       al, byte ptr ss:[bp + 03h]
    mov       ah, al
    rep stosw
    add       sp, 04h
    ret
Label0x23a9:                            ;Offset 0x23a9
    shl       bx, 03h
    shl       cx, 02h
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       dh
    push      dx
    mov       dx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       es, ax
    or        dx, dx
    je        Label0x23cf               ;Offset 0x23cf
Label0x23c2:                            ;Offset 0x23c2
    mov       ax, cx
    rep movsw
    mov       cx, ax
    add       si, bx
    add       di, bx
    dec       dx
    jne       Label0x23c2               ;Offset 0x23c2
Label0x23cf:                            ;Offset 0x23cf
    pop       dx
    mov       ds, word ptr cs:[DSSegment];Offset 0x1486
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       dl
    mov       dx, ax
    mov       al, byte ptr [bp + 03h]
    mov       ah, al
Label0x23e1:                            ;Offset 0x23e1
    mov       si, cx
    rep stosw
    add       di, bx
    mov       cx, si
    dec       dx
    jne       Label0x23e1               ;Offset 0x23e1
    add       sp, 04h
    ret
Func0x2339 ENDP

ReadCharAndAttrAtCursorPos PROC NEAR    ;Offset 0x23f0
    call ReadCharAndAttrAtCursorPosInternal;Offset 0x23f7
    mov  word ptr ss:[bp + 10h], ax
    ret
ReadCharAndAttrAtCursorPos ENDP

ReadCharAndAttrAtCursorPosInternal PROC NEAR;Offset 0x23f7
    mov  ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  ah, 0ch
    jbe  Func0x240d                     ;Offset 0x240d
    cmp  ah, 13h
    je   Func0x2433                     ;Offset 0x2433
    jb   Func0x240a                     ;Offset 0x240a
    jmp  Func0x11f1                     ;Offset 0x11f1
ReadCharAndAttrAtCursorPosInternal ENDP

Func0x240a PROC NEAR                    ;Offset 0x240a
    jmp  Func0x2509                     ;Offset 0x2509
Func0x240a ENDP

Func0x240d PROC NEAR                    ;Offset 0x240d
    mov  dx, ax
    mov  ax, word ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, 30h
    cmp  al, 30h
    mov  ax, 0b000h                     ;Segment 0xb000
    je   Label0x241d                    ;Offset 0x241d
    mov  ah, 0b8h                       ;Segment 0xb800
Label0x241d:                            ;Offset 0x241d
    mov  es, ax
    mov  ax, dx
    cmp  ah, 02h
    jb   Func0x2436                     ;Offset 0x2436
    cmp  ah, 04h
    jb   Func0x2454                     ;Offset 0x2454
    cmp  ah, 07h
    jae  Func0x2436                     ;Offset 0x2436
    jmp  Func0x247d                     ;Offset 0x247d
    nop
Func0x240d ENDP

Func0x2433 PROC NEAR                    ;Offset 0x2433
    jmp  Func0x258f                     ;Offset 0x258f
Func0x2433 ENDP

Func0x2436 PROC NEAR                    ;Offset 0x2436
    or   bh, bh
    jne  Label0x244d                    ;Offset 0x244d
    mov  bx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov  al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul  bh
    xor  bh, bh
    add  bx, ax
    shl  bx, 01h
    mov  ax, word ptr es:[bx]
    ret
Label0x244d:                            ;Offset 0x244d
    call Func0x264a                     ;Offset 0x264a
    mov  ax, word ptr es:[di]
    ret
Func0x2436 ENDP

Func0x2454 PROC NEAR                    ;Offset 0x2454
    test  byte ptr ds:[BDA_VideoModeOptions], 04h;Offset 0x487
    je    Func0x2436                    ;Offset 0x2436
    call  Func0x264a                    ;Offset 0x264a
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dx, 06h
    mov   ax, es
    mov   ds, ax
    mov   si, di
Label0x246b:                            ;Offset 0x246b
    in    al, dx
    and   al, 01h
    jne   Label0x246b                   ;Offset 0x246b
    cli
Label0x2471:                            ;Offset 0x2471
    in    al, dx
    and   al, 01h
    je    Label0x2471                   ;Offset 0x2471
    lodsw
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret
Func0x2454 ENDP

Func0x247d PROC NEAR                    ;Offset 0x247d
    mov   al, byte ptr ds:[DBA_CursorPositionPage0 + 01h];Offset 0x451
    mul   byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov   di, ax
    shl   di, 02h
    mov   al, byte ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    xor   ah, ah
    add   di, ax
    mov   ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp   ah, 06h
    je    Label0x249b                   ;Offset 0x249b
    shl   di, 01h
Label0x249b:                            ;Offset 0x249b
    add   di, 20f0h
    cmp   ah, 06h
    mov   si, di
    mov   ax, es
    mov   ds, ax
    mov   cx, 04h
    je    Label0x24c2                   ;Offset 0x24c2
Label0x24ad:                            ;Offset 0x24ad
    call  Func0x260d                    ;Offset 0x260d
    mov   bh, bl
    sub   si, 2002h
    call  Func0x260d                    ;Offset 0x260d
    add   si, 1faeh
    push  bx
    loop  Label0x24ad                   ;Offset 0x24ad
    jmp   Label0x24d1                   ;Offset 0x24d1
Label0x24c2:                            ;Offset 0x24c2
    lodsb
    mov   ah, al
    sub   si, 2001h
    lodsb
    add   si, 1fafh
    push  ax
    loop  Label0x24c2                   ;Offset 0x24c2
Label0x24d1:                            ;Offset 0x24d1
    mov   si, sp
    mov   ax, ss
    mov   ds, ax
    mov   ax, cs
    mov   es, ax
    mov   di, offset LowerCharacters8x8 ;Offset 0x5720
    mov   dx, 80h
    call  Func0x2622                    ;Offset 0x2622
    jne   Label0x2500                   ;Offset 0x2500
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    les   di, es:[007ch]                ;Offset 0x7c
    mov   ax, es
    or    ax, di
    je    Label0x2500                   ;Offset 0x2500
    mov   dx, 80h
    call  Func0x2622                    ;Offset 0x2622
    je    Label0x2500                   ;Offset 0x2500
    add   al, 80h
Label0x2500:                            ;Offset 0x2500
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    add   sp, 08h
    ret
Func0x247d ENDP

Func0x2509 PROC NEAR                    ;Offset 0x2509
    mov        ax, 0a000h               ;Segment 0xa000
    mov        es, ax
    mov        al, bh
    xor        ah, ah
    shl        ax, 01h
    mov        si, ax
    mov        ax, word ptr ds:[si + DBA_CursorPositionPage0];Offset 0x450
    mov        si, ax
    and        si, 0ffh
    mov        al, ah
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add        si, ax
    xor        ah, ah
    mov        al, bh
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add        si, ax
    mov        al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec        al
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    add        si, ax
    mov        ax, 0805h
    mov        dx, 03ceh                ;port - 0x3ce
    out        dx, ax
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    inc        bx
Label0x2551:                            ;Offset 0x2551
    lodsb      byte ptr es:[si]
    mov        ah, al
    not        ah
    push       ax
    inc        sp
    sub        si, bx
    loop       Label0x2551              ;Offset 0x2551
    mov        ax, 05h
    out        dx, ax
    mov        si, sp
    xor        al, al
    mov        es, word ptr cs:[Data1488];Offset 0x1488
    les        bx, es:[10ch]            ;Offset 0x10c
    mov        dx, si
Label0x2571:                            ;Offset 0x2571
    mov        di, bx
    cli
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    repe cmpsb byte ptr ss:[si], byte ptr es:[di]
    sti
    je         Label0x2588              ;Offset 0x2588
    add        bx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        si, dx
    inc        al
    jne        Label0x2571              ;Offset 0x2571
Label0x2588:                            ;Offset 0x2588
    xor        ah, ah
    add        sp, word ptr ds:[BDA_PointHeightOfCharacterMatrix]
    ret
Func0x2509 ENDP

Func0x258f PROC NEAR                    ;Offset 0x258f
    mov        ax, 0a000h               ;Offset 0xa000
    mov        es, ax
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov        di, ax
    and        di, 0ffh
    mov        al, ah
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add        di, ax
    shl        di, 03h
    mov        al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec        al
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        cl, 03h
    shl        ax, cl
    mov        si, di
    add        si, ax
    mov        dx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl        dx, cl
    add        dx, 08h
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
Label0x25ca:                            ;Offset 0x25ca
    mov        bl, 08h
    xor        ah, ah
Label0x25ce:                            ;Offset 0x25ce
    lodsb      byte ptr es:[si]
    cmp        al, 01h
    cmc
    rcl        ah, 01h
    dec        bl
    jne        Label0x25ce              ;Offset 0x25ce
    push       ax
    inc        sp
    sub        si, dx
    loop       Label0x25ca              ;Offset 0x25ca
    mov        si, sp
    xor        al, al
    mov        es, word ptr cs:[Data1488];Offset 0x1488
    les        bx, es:[010ch]           ;Offset 0x10c
    mov        dx, si
Label0x25ef:                            ;Offset 0x25ef
    mov        di, bx
    cli
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    repe cmpsb byte ptr ss:[si], byte ptr es:[di]
    sti
    je         Label0x2606              ;Offset 0x2606
    add        bx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        si, dx
    inc        al
    jne        Label0x25ef              ;Offset 0x25ef
Label0x2606:                            ;Offset 0x2606
    xor        ah, ah
    DB 03h, 26h, 85h, 04h               ;add        sp, word ptr [BDA_PointHeightOfCharacterMatrix];Offset 0x485 - masm encoding difference
    ret        
Func0x258f ENDP

Func0x260d PROC NEAR                    ;Offset 0x260d
    mov        dl, 08h
    xor        bl, bl
    lodsw
    xchg       al, ah
Label0x2614:                            ;Offset 0x2614
    shl        ax, 01h
    jns        Label0x2619              ;Offset 0x2619
    stc
Label0x2619:                            ;Offset 0x2619
    rcl        bl, 01h
    shl        ax, 01h
    dec        dl
    jne        Label0x2614              ;Offset 0x2614
    ret        
Func0x260d ENDP

Func0x2622 PROC NEAR                    ;Offset 0x2622
    mov        bx, di
    push       bp
Label0x2625:                            ;Offset 0x2625
    mov        ax, si
    mov        bp, di
    mov        cx, 04h
    repe cmpsw word ptr ds:[si], word ptr es:[di]
    mov        di, bp
    je         Label0x263e              ;Offset 0x263e
    add        di, 08h
    mov        si, ax
    dec        dx
    jne        Label0x2625              ;Offset 0x2625
    xor        ax, ax
    pop        bp
    ret
Label0x263e:                            ;Offset 0x263e
    sub        di, bx
    mov        cl, 03h
    shr        di, cl
    mov        ax, di
    or         cl, cl
    pop        bp
    ret        
Func0x2622 ENDP

Func0x264a PROC NEAR                    ;Offset 0x264a
    mov        ah, bl
    mov        di, ax
    mov        bl, bh
    xor        bh, bh
    mov        ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    shr        ax, 01h
    mul        bx
    mov        dx, ax
    shl        bx, 01h
    mov        bx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    mov        al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        bh
    add        ax, dx
    xor        bh, bh
    add        ax, bx
    shl        ax, 01h
    xchg       ax, di
    ret        
Func0x264a ENDP

WriteCharAndAttrAtCursorPos PROC NEAR   ;Offset 0x2670
    mov  ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  ah, BDA_DM_Reserved2           ;0ch
    jbe  Func0x2694
    cmp  ah, BDA_DM_320x200_256_Color_Graphics;13h
    je   Func0x2691                     ;Offset 0x2691
    jb   Func0x2683                     ;Offset 0x2683
    jmp  Func0x11c9                     ;Offset 0x11c9
WriteCharAndAttrAtCursorPos ENDP

Func0x2683 PROC NEAR                    ;Offset 0x2683
    cmp  ah, BDA_DM_640x480_BW_Graphics ;0x11
    jne  Label0x268e                    ;Offset 0x268e
    and  bl, 80h
    or   bl, 3fh
Label0x268e:                            ;Offset 0x268e
    jmp  Func0x2832
Func0x2683 ENDP

Func0x2691 PROC NEAR                    ;Offset 0x2691
    jmp  Func0x28e6                     ;Offset 0x28e6
Func0x2691 ENDP

Func0x2694 PROC NEAR
    mov       dl, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       dl, 30h
    cmp       dl, 30h
    mov       dx, 0b000h                ;Segment 0xb000
    je        Label0x26a5               ;Offset 0x26a5
    mov       dh, 0b8h                  ;Segment 0xb800
Label0x26a5:                            ;Offset 0x26a5
    mov       es, dx
    cmp       ah, 07h
    jae       Label0x26bd               ;Offset 0x26bd
    cmp       ah, 03h
    ja        Label0x26de               ;Offset 0x26de
    cmp       ah, 02h
    jb        Label0x26bd               ;Offset 0x26bd
    test      byte ptr ds:[BDA_VideoModeOptions], 04h;Offset 0x487
    jne       Label0x26e1               ;Offset 0x26e1
Label0x26bd:                            ;Offset 0x26bd
    or        bh, bh
    jne       Label0x26d8               ;Offset 0x26d8
    mov       ah, bl
    mov       di, ax
    mov       bx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       bh
    xor       bh, bh
    add       ax, bx
    shl       ax, 01h
    xchg      ax, di
    rep stosw word ptr es:[di]
    ret
Label0x26d8:                            ;Offset 0x26d8
    call      Func0x264a                ;Offset 0x264a
    rep stosw word ptr es:[di]
    ret
Label0x26de:                            ;Offset 0x26de
    jmp       Func0x2780                ;Offset 0x2780
Label0x26e1:                            ;Offset 0x26e1
    call      Func0x264a                ;Offset 0x264a
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dx, 06h
    mov       bl, al
Label0x26ed:                            ;Offset 0x26ed
    in        al, dx
    test      al, 01h
    jne       Label0x26ed               ;Offset 0x26ed
    cli
Label0x26f3:                            ;Offset 0x26f3
    in        al, dx
    test      al, 01h
    je        Label0x26f3               ;Offset 0x26f3
    mov       al, bl
    stosw     word ptr es:[di]
    sti
    loop      Label0x26ed               ;Offset 0x26ed
    ret       
Func0x2694 ENDP

WriteCharOnlyAtCursorPos PROC NEAR      ;Offset 0x26ff
    mov   ah, byte ptr ds:[BDA_DisplayMode]
    cmp   ah, BDA_DM_Reserved2          ;0xc
    jbe   Func0x2715                    ;Offset 0x2715
    cmp   ah, BDA_DM_320x200_256_Color_Graphics;0x13
    je    Func0x2691                    ;Offset 0x2691
    jb    Func0x2712                    ;Offset 0x2712
    jmp   Func0x11b6                    ;Offset 0x11b6
WriteCharOnlyAtCursorPos ENDP

Func0x2712 PROC NEAR                    ;Offset 0x2712
    jmp   Func0x2832                    ;Offset 0x2832
Func0x2712 ENDP

Func0x2715 PROC NEAR                    ;Offset 0x2715
    mov   dl, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and   dl, BDA_DH_InitialVideoModeMask;0x30
    cmp   dl, BDA_DH_80x25Monochrome    ;0x30
    mov   dx, 0b000h                    ;segment 0xb000
    je    Label0x2726                   ;Offset 0x2726
    mov   dh, 0b8h                      ;segment 0xb800
Label0x2726:                            ;Offset 0x2726
    mov   es, dx
    cmp   ah, 07h
    jae   Label0x273e                   ;Offset 0x273e
    cmp   ah, 03h
    ja    Func0x2780                    ;Offset 0x2780
    cmp   ah, 02h
    jb    Label0x273e                   ;Offset 0x273e
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487   04h
    jne   Func0x2761                    ;Offset 0x2761
Label0x273e:                            ;Offset 0x273e
    or    bh, bh
    jne   Func0x2759                    ;Offset 0x2759
    mov   di, ax
    mov   bx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov   al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul   bh
    xor   bh, bh
    add   ax, bx
    shl   ax, 01h
    xchg  ax, di
Label0x2754:                            ;Offset 0x2754
    stosb byte ptr es:[di]
    inc   di
    loop  Label0x2754                   ;Offset 0x2754
    ret
Func0x2715 ENDP

Func0x2759 PROC NEAR                    ;Offset 0x2759
    call  Func0x264a                    ;Offset 0x264a
Label0x275c:                            ;Offset 0x275c
    stosb byte ptr es:[di]
    inc   di
    loop  Label0x275c                   ;Offset 0x275c
    ret   
Func0x2759 ENDP

Func0x2761 PROC NEAR                    ;Offset 0x2761
    call  Func0x264a                    ;Offset 0x264a
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dx, 06h
    mov   ah, al
Label0x276d:                            ;Offset 0x276d
    in    al, dx
    test  al, 01h
    jne   Label0x276d                   ;Offset 0x276d
    cli
Label0x2773:                            ;Offset 0x2773
    in    al, dx
    test  al, 01h
    je    Label0x2773                   ;Offset 0x2773
    mov   al, ah
    stosb byte ptr es:[di]
    sti
    inc   di
    loop  Label0x276d                   ;Offset 0x276d
    ret   
Func0x2761 ENDP

Func0x2780 PROC NEAR                    ;Offset 0x2780
    mov   dx, ax
    mov   al, byte ptr ds:[DBA_CursorPositionPage0 + 01h];Offset 0x451
    mul   byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov   di, ax
    shl   di, 02h
    mov   al, byte ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    xor   ah, ah
    add   di, ax
    cmp   dh, 06h
    je    Label0x279c                   ;Offset 0x279c
    shl   di, 01h
Label0x279c:                            ;Offset 0x279c
    mov   al, dl
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    or    al, al
    jns   Label0x27af                   ;Offset 0x27af
    and   al, 7fh
    lds   si, ds:[7ch]                  ;Offset 0x7c
    jmp   Label0x27b3                   ;Offset 0x27b3
Label0x27af:                            ;Offset 0x27af
    lds   si, ds:[10ch]
Label0x27b3:                            ;Offset 0x27b3
    xor   ah, ah
    shl   ax, 03h
    add   si, ax
    cmp   dh, 06h
    jne   Label0x2807                   ;Offset 0x2807
Label0x27bf:                            ;Offset 0x27bf
    mov   dl, 04h
    or    bl, bl
    js    Label0x27e5                   ;Offset 0x27e5
Label0x27c5:                            ;Offset 0x27c5
    lodsw word ptr ds:[si]
    stosb byte ptr es:[di]
    add   di, 1fffh
    mov   al, ah
    stosb byte ptr es:[di]
    sub   di, 1fb1h
    dec   dl
    jne   Label0x27c5                   ;Offset 0x27c5
    sub   di, 013fh
    sub   si, 08h
    loop  Label0x27bf                   ;Offset 0x27bf
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret
Label0x27e5:                            ;Offset 0x27e5
    lodsw word ptr ds:[si]
    xor   byte ptr es:[di], al
    add   di, 2000h
    xor   byte ptr es:[di], ah
    sub   di, 1fb0h
    dec   dl
    jne   Label0x27e5                   ;Offset 0x27e5
    sub   di, 13fh
    sub   si, 08h
    loop  Label0x27bf                   ;Offset 0x27bf
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret
Label0x2807:                            ;Offset 0x2807
    mov   bh, bl
    and   bh, 03h
    mov   bp, cx
Label0x280e:                            ;Offset 0x280e
    mov   dh, 04h
Label0x2810:                            ;Offset 0x2810
    call  Func0x2942                    ;Offset 0x2942
    add   di, 1ffeh
    call  Func0x2942                    ;Offset 0x2942
    sub   di, 1fb2h
    dec   dh
    jne   Label0x2810                   ;Offset 0x2810
    sub   di, 013eh
    sub   si, 08h
    dec   bp
    jne   Label0x280e                   ;Offset 0x280e
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret   
Func0x2780 ENDP

Func0x2832 PROC NEAR                    ;Offset 0x2832
    mov   si, ax
    mov   bp, cx
    mov   es, word ptr cs:[Data148a]    ;Offset 0x148a
    mov   al, bh
    xor   ah, ah
    shl   ax, 01h
    mov   di, ax
    mov   ax, word ptr ds:[di + DBA_CursorPositionPage0];Offset 0x450
    mov   di, ax
    and   di, 0ffh
    mov   al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul   ah
    mul   word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add   di, ax
    xor   ah, ah
    mov   al, bh
    mul   word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add   di, ax
    mov   dl, bl
    mov   cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov   bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec   bx
    mov   ax, si
    mul   byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds   si, ds:[010ch]                ;Offset 0x10c
    add   si, ax
    mov   ah, dl
    xor   al, al
    mov   dx, 03ceh                     ;port - 0x3ce
    out   dx, ax
    inc   al
    not   ah
    out   dx, ax
    test  ah, 80h
    je    Label0x28b5                   ;Offset 0x28b5
    mov   dx, 03c4h                     ;Offset 0x3c4
    mov   ax, 0f02h
    out   dx, ax
Label0x2892:                            ;Offset 0x2892
    push  si
    push  di
    mov   dx, cx
Label0x2896:                            ;Offset 0x2896
    movsb byte ptr es:[di], byte ptr ds:[si]
    add   di, bx
    loop  Label0x2896                   ;Offset 0x2896
    mov   cx, dx
    pop   di
    pop   si
    inc   di
    dec   bp
    jne   Label0x2892                   ;Offset 0x2892
    mov   ax, 03h
    mov   dx, 03ceh                     ;port - 0x3ce
    out   dx, ax
    xor   ax, ax
    out   dx, ax
    inc   ax
    out   dx, ax
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret
Label0x28b5:                            ;Offset 0x28b5
    mov   ax, 1803h
    out   dx, ax
    mov   dx, 03c4h                     ;port - 0x3c4
    mov   ax, 0f02h
    out   dx, ax
Label0x28c0:                            ;Offset 0x28c0
    push  si
    push  di
    mov   dx, cx
Label0x28c4:                            ;Offset 0x28c4
    mov   al, byte ptr es:[di]
    movsb byte ptr es:[di], byte ptr ds:[si]
    add   di, bx
    loop  Label0x28c4                   ;Offset 0x28c4
    mov   cx, dx
    pop   di
    pop   si
    inc   di
    dec   bp
    jne   Label0x28c0                   ;Offset 0x28c0
    mov   ax, 03h
    mov   dx, 03ceh                     ;port - 0x3ce
    out   dx, ax
    xor   ax, ax
    out   dx, ax
    inc   ax
    out   dx, ax
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret   
Func0x2832 ENDP

Func0x28e6 PROC NEAR                    ;Offset 0x28e6
    mov   bp, cx
    mov   cx, ax
    mov   ax, 0a000h;Segment 0xa000
    mov   es, ax
    mov   ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov   di, ax
    and   di, 0ffh
    mov   al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul   ah
    mul   word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add   di, ax
    shl   di, 3
    mov   ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov   dx, ax
    mul   cl
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    lds   si, ds:[010ch]                ;Offset 0x10c
    add   si, ax
Label0x2918:                            ;Offset 0x2918
    push  di
    push  dx
Label0x291a:                            ;Offset 0x291a
    mov   cx, 08h
    mov   ah, byte ptr ds:[si]
    inc   si
Label0x2920:                            ;Offset 0x2920
    rcl   ah, 01h
    mov   al, bl
    jb    Label0x2928                   ;Offset 0x2928
    mov   al, bh
Label0x2928:                            ;Offset 0x2928
    stosb byte ptr es:[di]
    loop  Label0x2920                   ;Offset 0x2920
    add   di, 0138h
    dec   dx
    jne   Label0x291a                   ;Offset 0x291a
    pop   dx
    pop   di
    sub   si, dx
    add   di, 08h
    dec   bp
    jne   Label0x2918                   ;Offset 0x2918
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    ret   
Func0x28e6 ENDP

Func0x2942 PROC NEAR                    ;Offset 0x2942
    mov   dl, 08h
    xor   cx, cx
    lodsb byte ptr ds:[si]
Label0x2947:                            ;Offset 0x2947
    rcr   al, 01h
    jae   Label0x294d                   ;Offset 0x294d
    or    cl, bh
Label0x294d:                            ;Offset 0x294d
    ror   cx, 01h
    ror   cx, 01h
    dec   dl
    jne   Label0x2947                   ;Offset 0x2947
    mov   ax, cx
    xchg  al, ah
    or    bl, bl
    js    Label0x295f                   ;Offset 0x295f
    stosw word ptr es:[di]
    ret
Label0x295f:                            ;Offset 0x295f
    xor   word ptr es:[di], ax
    add   di, 02h
    ret
Func0x2942 ENDP

ColorFunctions PROC NEAR                ;Offset 0x2966
    cmp  word ptr ds:[BDA_VideoBaseIOPort], 03b4h;Offset 0x463, port - 0x3b4
    je   Label0x29c2                    ;Offset 0x29c2
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487, 08h
    je   Label0x2978                    ;Offset 0x2978
    int  42h
    ret
Label0x2978:                            ;Offset 0x2978
    cmp  bh, 00h
    je   Label0x29c3                    ;Offset 0x29c3
Label0x297d:                            ;Offset 0x297d
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  al, BDA_DM_320x200_4_Color_Graphics1;04h
    jb   Label0x29c2                    ;Offset 0x29c2
    cmp  al, BDA_DM_320x200_256_Color_Graphics;13h
    jbe  Label0x298f                    ;Offset 0x298f
    call GetVideoModeFlags              ;Offset 0x105d
    test al, 01h
    jne  Label0x29c2                    ;Offset 0x29c2
Label0x298f:                            ;Offset 0x298f
    mov  bh, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and  bh, 0dfh
    and  bl, 01h
    je   Label0x299e                    ;Offset 0x299e
    or   bh, 20h
Label0x299e:                            ;Offset 0x299e
    mov  byte ptr ds:[BDA_CGAColorPaletteMaskSetting], bh;Offset 0x466
    and  bh, 10h
    or   bh, bl
    or   bx, 0201h
Label0x29ab:                            ;Offset 0x29ab
    call Func0x2a31                     ;Offset 0x2a31
    call Func0x2a13                     ;Offset 0x2a13
    add  bh, 02h
    inc  bl
    cmp  bl, 03h
    jbe  Label0x29ab                    ;Offset 0x29ab
    mov  dx, 03d9h                      ;port - 0x3d9
    mov  al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    out  dx, al
Label0x29c2:                            ;Offset 0x29c2
    ret
Label0x29c3:                            ;Offset 0x29c3
    mov   ah, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and   ah, 0e0h
    mov   bh, bl
    and   bh, 1fh
    or    ah, bh
    mov   byte ptr ds:[BDA_CGAColorPaletteMaskSetting], ah;Offset 0x466
    mov   al, byte ptr ss:[bp + 0eh]
    and   al, 08h
    shl   al, 01h
    and   bh, 07h
    or    bh, al
    mov   bl, 11h
    call  Func0x2a31                    ;Offset 0x2a31
    mov   bl, 10h
    call  Func0x2a13                    ;Offset 0x2a13
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp   al, 03h
    jbe   Label0x2a05                   ;Offset 0x2a05
    cmp   al, 13h
    jbe   Label0x29fd                   ;Offset 0x29fd
    call  GetVideoModeFlags             ;Offset 0x105d
    test  al, 01h
    jne   Label0x2a05                   ;Offset 0x2a05
Label0x29fd:                            ;Offset 0x29fd
    xor   bl, bl
    call  Func0x2a31                    ;Offset 0x2a31
    call  Func0x2a13                    ;Offset 0x2a13
Label0x2a05:                            ;Offset 0x2a05
    mov   bl, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and   bl, 20h
    mov   cl, 05h
    shr   bl, cl
    jmp   Label0x297d                   ;Offset 0x297d
ColorFunctions ENDP

Func0x2a13 PROC NEAR                    ;Offset 0x2a13
    push  ax
    push  di
    push  es
    les   di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les   di, es:[di + 04h]
    mov   ax, es
    or    ax, di
    je    Label0x2a2d                   ;Offset 0x2a2d
    mov   al, bl
    mov   ah, 00h
    add   di, ax
    mov   al, bh
    stosb byte ptr es:[di]
Label0x2a2d:                            ;Offset 0x2a2d
    pop   es
    pop   di
    pop   ax
    ret   
Func0x2a13 ENDP

Func0x2a31 PROC NEAR                    ;Offset 0x2a31
    push  ax
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h
Label0x2a3a:                            ;Offset 0x2a3a
    in    al, dx
    and   al, 08h
    je    Label0x2a3a                   ;Offset 0x2a3a
    mov   dx, 03c0h                     ;port - 0x3c0
    mov   al, bl
    cli
    out   dx, al
    mov   al, bh
    out   dx, al
    mov   al, 20h
    out   dx, al
    sti
    pop   dx
    pop   ax
    ret
Func0x2a31 ENDP

Func0x2a50 PROC NEAR                    ;Offset 0x2a50
    ja    Label0x2a6c                   ;Offset 0x2a6c
    push  es
    push  di
    push  ax
    push  dx
    mov   di, ax
    mov   es, word ptr cs:[Data148a]    ;Offset 0x148a
    mov   ax, 0140h
    mul   dx
    add   ax, cx
    xchg  ax, di
    stosb byte ptr es:[di]
    pop   dx
    pop   ax
    pop   di
    pop   es
    pop   ds
    iret
Label0x2a6c:                            ;Offset 0x2a6c
    jmp   Func0x119e                    ;Offset 0x119e
Func0x2a50 ENDP

Func0x2a6f PROC NEAR                    ;Offset 0x2a6f
    cmp   ah, 04h
    jb    Label0x2aa3                   ;Offset 0x2aa3
    cmp   ah, 07h
    je    Label0x2aa3                   ;Offset 0x2aa3
    cmp   ah, 08h
    jae   Func0x2abe                    ;Offset 0x2abe
    push  es
    push  di
    push  bx
    push  cx
    push  dx
    mov   bx, ax
    call  Func0x2bc7                    ;Offset 0x2bc7
    mov   al, ah
    ror   al, cl
    not   al
    and   al, bl
    shl   al, cl
    or    bl, bl
    js    Label0x2aa5                   ;Offset 0x2aa5
    and   ah, byte ptr es:[di]
    or    al, ah
    stosb byte ptr es:[di]
Label0x2a9c:                            ;Offset 0x2a9c
    mov   ax, bx
    pop   dx
    pop   cx
    pop   bx
    pop   di
    pop   es
Label0x2aa3:                            ;Offset 0x2aa3
    pop   ds
    iret
Label0x2aa5:                            ;Offset 0x2aa5
    xor   byte ptr es:[di], al
    jmp   Label0x2a9c                   ;Offset 0x2a9c
Func0x2a6f ENDP

WriteGraphicsPixel PROC NEAR            ;Offset 0x2aaa
    push ds
    mov  ds, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  ah, BDA_DM_320x200_16_Color_Graphics;0dh
    jb   Func0x2a6f                     ;Offset 0x2a6f
    cmp  ah, BDA_DM_320x200_256_Color_Graphics;13h
    jae  Func0x2a50                     ;Offset 0x2a50
WriteGraphicsPixel ENDP

Func0x2abe PROC NEAR                    ;Offset 0x2abe
    push bx
    push cx
    push dx
Func0x2abe ENDP

Func0x2ac1 PROC NEAR                    ;Offset 0x2ac1
    xchg bx, cx
    mov  cl, bl
    shr  bx, 03h
    or   ch, ch
    jne  Label0x2b0b                    ;Offset 0x2b0b
Label0x2acc:                            ;Offset 0x2acc
    mov  ch, al
    mov  ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul  dx
    add  bx, ax
    mov  dx, 03ceh                      ;port -0x3ce
    xor  ax, ax
    out  dx, ax
    mov  ax, 0f01h
    out  dx, ax
    and  cl, 07h
    mov  ax, 8008h
    shr  ah, cl
    out  dx, ax
    mov  ax, 0a000h                     ;segment 0xa000
    mov  ds, ax
    or   ch, ch
    js   Label0x2b15                    ;Offset 0x2b15
    or   byte ptr ds:[bx], al
    mov  ah, ch
    out  dx, ax
    or   byte ptr ds:[bx], al
Label0x2af8:                            ;Offset 0x2af8
    mov  ax, 0ff08h
    out  dx, ax
    xor  ax, ax
    out  dx, ax
    inc  al
    out  dx, ax
    mov  al, ch
    mov  ah, 0ch
    pop  dx
    pop  cx
    pop  bx
    pop  ds
    iret
Label0x2b0b:                            ;Offset 0x2b0b
    add  bx, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec  ch
    jne  Label0x2b0b                    ;Offset 0x2b0b
    jmp  Label0x2acc                    ;Offset 0x2acc
Label0x2b15:                            ;Offset 0x2b15
    mov  ax, 1803h
    out  dx, ax
    mov  ah, ch
    xor  al, al
    out  dx, ax
    or   byte ptr ds:[bx], al
    mov  ax, 03h
    out  dx, ax
    jmp  Label0x2af8                    ;Offset 0x2af8
Func0x2ac1 ENDP

Func0x2b26 PROC NEAR                    ;Offset 0x2b26
    ja   Label0x2b40                    ;Offset 0x2b40
    push dx
    push di
    mov  ds, word ptr cs:[Data148a]     ;Offset 0x148a
    mov  ax, 140h
    mul  dx
    add  ax, cx
    mov  di, ax
    mov  al, byte ptr ds:[di]
    mov  ah, 0dh
    pop  di
    pop  dx
    pop  ds
    iret
Label0x2b40:                            ;Offset 0x2b40
    jmp  Func0x1185                     ;Offset 0x1185
Func0x2b26 ENDP

Func0x2b43 PROC NEAR                    ;Offset 0x2b43
    cmp  ah, 04h
    jb   Label0x2b68                    ;Offset 0x2b68
    cmp  ah, 07h
    je   Label0x2b68                    ;Offset 0x2b68
    cmp  ah, 08h
    jae  Func0x2b7e                     ;Offset 0x2b7e
    push es
    push di
    push cx
    push dx
    call Func0x2bc7                     ;Offset 0x2bc7
    not  ah
    and  ah, byte ptr es:[di]
    shr  ah, cl
    mov  al, ah
    mov  ah, 0dh
    pop  dx
    pop  cx
    pop  di
    pop  es
Label0x2b68:                            ;Offset 0x2b68
    pop  ds
    iret 
Func0x2b43 ENDP

ReadGraphicsPixel PROC NEAR             ;Offset 0x2b6a
    push ds
    mov  ds, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  ah, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  ah, 0dh
    jb   Func0x2b43                     ;Offset 0x2b43
    cmp  ah, 13h
    jae  Func0x2b26                     ;Offset 0x2b26
ReadGraphicsPixel ENDP
;DON'T MOVE! Rolls into next function!

;DON'T MOVE! Above function rolls into this one!
Func0x2b7e PROC NEAR                    ;Offset 0x2b7e
    push si
    push bx
    push cx
    push dx
Func0x2b7e ENDP
;DON'T MOVE! Rolls into next function!

;DON'T MOVE! Above function rolls into this one!
Func0x2b82 PROC NEAR                    ;Offset 0x2b82
    mov  si, cx
    shr  si, 03h
    or   bh, bh
    jne  Label0x2bbd                    ;Offset 0x2bbd
Label0x2b8b:                            ;Offset 0x2b8b
    mov  ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul  dx
    add  si, ax
    and  cl, 07h
    mov  bl, 80h
    shr  bl, cl
    mov  ds, word ptr cs:[Data148a]     ;Offset 0x148a
    mov  dx, GraphicsControllerIndex    ;port - 0x3ce
    xor  cl, cl
    mov  ax, 0304h                      ;GR4 - Read Plane Select Register - bits 1-0 = 11 - Plane 3
Label0x2ba6:                            ;Offset 0x2ba6
    out  dx, ax
    mov  ch, byte ptr ds:[si]
    and  ch, bl
    neg  ch
    rol  cx, 01h
    dec  ah
    jns  Label0x2ba6                    ;Offset 0x2ba6
    mov  al, cl
    mov  ah, 0dh
    pop  dx
    pop  cx
    pop  bx
    pop  si
    pop  ds
    iret
Label0x2bbd:                            ;Offset 0x2bbd
    add  si, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec  bh
    jne  Label0x2bbd                    ;Offset 0x2bbd
    jmp  Label0x2b8b                    ;Offset 0x2b8b
Func0x2b82 ENDP

;inputs:
;dl = 
;cx = 

Func0x2bc7 PROC NEAR                    ;Offset 0x2bc7
    mov  ax, 0b800h                     ;Offset 0xb800 - Text video memory
    mov  es, ax
    mov  al, 28h                        ;0x28 = 40
    mul  dl
    test al, 08h
    je   Label0x2bd7                    ;Offset 0x2bd7
    add  ax, 1fd8h                      ;0x1fd8 = 8152
Label0x2bd7:                            ;Offset 0x2bd7
    mov  di, ax
    mov  al, cl
    not  al
    cmp  byte ptr ds:[BDA_DisplayMode], 06h;Offset 0x449
    jb   Label0x2bec                    ;Offset 0x2bec
    shr  cx, 01h
    mov  ah, 0feh
    and  al, 07h
    jmp  Label0x2bf2                    ;Offset 0x2bf2
Label0x2bec:                            ;Offset 0x2bec
    mov  ah, 0fch
    shl  al, 01h
    and  al, 06h
Label0x2bf2:                            ;Offset 0x2bf2
    shr  cx, 02h
    add  di, cx
    mov  cl, al
    rol  ah, cl
    ret
Func0x2bc7 ENDP

Func0x2bfc PROC NEAR                    ;Offset 0x2bfc
    je     Label0x2c10                  ;Offset 0x2c10
    cmp    al, 0ah
    je     Func0x2c6a                   ;Offset 0x2c6a
    cmp    al, 08h
    je     Label0x2c15                  ;Offset 0x2c15
    cmp    al, 07h                      ;Bell character
    jne    Func0x2c53                   ;Offset 0x2c53
    call   BellCharacterBeep            ;Offset 0x2ce0
    jmp    Func0x2ccf                   ;Offset 0x2ccf
Label0x2c10:                            ;Offset 0x2c10
    xor    dl, dl
    jmp    NEAR PTR Func0x2cac          ;Offset 0x2cac
Label0x2c15:                            ;Offset 0x2c15
    dec    dl
    jns    Label0x2c1c                  ;Offset 0x2c1c
    jmp    Func0x2ccf                   ;Offset 0x2ccf
Label0x2c1c:                            ;Offset 0x2c1c
    jmp    NEAR PTR Func0x2cac          ;Offset 0x2cac
Func0x2bfc ENDP

Func0x2c1f PROC NEAR                    ;Offset 0x2c1f
    mov    cl, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    xor    ch, ch
    mov    di, cx
    shl    di, 01h
    mov    bh, cl
    mov    dx, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    cmp    al, 0dh
    jbe    Func0x2bfc                   ;Offset 0x2bfc
    jmp    Func0x2c53                   ;Offset 0x2c53
Func0x2c1f ENDP

TeletypeOutput PROC NEAR                ;Offset 0x2c35
    push   ax
    push   bx
    push   cx
    push   dx
    push   si
    push   di
    push   bp
    push   es
    push   ds
    mov    ds, word ptr cs:[Data1488]   ;Offset 0x1488
    mov    bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    or     bh, bh
    jne    Func0x2c1f                   ;Offset 0x2c1f
    mov    dx, word ptr ds:[DBA_CursorPositionPage0]
    cmp    al, 0dh
    jbe    Func0x2bfc                   ;Offset 0x2bfc
TeletypeOutput ENDP

Func0x2c53 PROC NEAR                    ;Offset 0x2c53
    push   dx
    xor    cx, cx
    inc    cl
    call   WriteCharOnlyAtCursorPos     ;Offset 0x26ff
    pop    dx
    inc    dl
    cmp    dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jne    Func0x2cac                   ;Offset 0x2cac
    xor    dl, dl
    mov    bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
Func0x2c53 ENDP

Func0x2c6a PROC NEAR                    ;Offset 0x2c6a
    inc    dh
    cmp    dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe    Func0x2cac                   ;Offset 0x2cac
    dec    dh
    push   dx
    call   ReadCharAndAttrAtCursorPosInternal;Offset 0x23f7
    mov    bh, ah
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp    al, BDA_DM_320x200_4_Color_Graphics1;04h
    jb     Label0x2c94                  ;Offset 0x2c94
    cmp    al, BDA_DM_80x25_Monochrome_Text;07h
    je     Label0x2c94                  ;Offset 0x2c94
    cmp    al, BDA_DM_320x200_256_Color_Graphics;13h
    jbe    Label0x2c92                  ;Offset 0x2c92
    call   GetVideoModeFlags            ;Offset 0x105d
    jne    Label0x2c92                  ;Offset 0x2c92
    test   al, 01h
    jne    Label0x2c94                  ;Offset 0x2c94
Label0x2c92:                            ;Offset 0x2c92
    xor    bh, bh
Label0x2c94:                            ;Offset 0x2c94
    mov    dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec    dl
    mov    dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    xor    cx, cx
    mov    ax, 0601h
    call   ScrollUpWindow               ;Offset 0x1ef1
    mov    ds, word ptr cs:[Data1488]   ;Offset 0x1488
    pop    dx
Func0x2c6a ENDP

Func0x2cac PROC NEAR                    ;Offset 0x2cac
    mov    al, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    or     al, al
    jne    Func0x2cd9                   ;Offset 0x2cd9
    mov    word ptr ds:[DBA_CursorPositionPage0], dx;Offset 0x450
    mov    al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul    dh
    add    al, dl
    adc    ah, 00h
    mov    bl, al
    mov    al, 0eh
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    out    dx, ax
    mov    ah, bl
    inc    al
    out    dx, ax
Func0x2cac ENDP

Func0x2ccf PROC NEAR                    ;Offset 0x2ccf
    pop    ds
    pop    es
    pop    bp
    pop    di
    pop    si
    pop    dx
    pop    cx
    pop    bx
    pop    ax
    iret
Func0x2ccf ENDP

Func0x2cd9 PROC NEAR                    ;Offset 0x2cd9
    mov    bh, al
    call   SetCursorPosition            ;Offset 0x1e4e
    jmp    Func0x2ccf                   ;Offset 0x2ccf
Func0x2cd9 ENDP

BellCharacterBeep PROC NEAR             ;Offset 0x2ce0
    push   ax
    push   bx
    push   cx
    push   dx
    mov    al, 02h
    mov    cx, 036ah
    call   Beep                         ;Offset 0x4654
    xor    al, al
    mov    cx, 19h
    call   Beep                         ;Offset 0x4654
    pop    dx
    pop    cx
    pop    bx
    pop    ax
    ret    
BellCharacterBeep ENDP

GetCurrentVideoMode PROC NEAR           ;Offset 0x2cf9
    mov    al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and    al, 80h
    or     al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov    byte ptr ss:[bp + 10h], al
    mov    al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov    byte ptr ss:[bp + 11h], al
    mov    al, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    mov    byte ptr ss:[bp + 0fh], al
    ret
GetCurrentVideoMode ENDP

DACFunctionTable        DW offset Func0x2d62;Offset 0x2D62
                        DW offset Func0x2d60;Offset 0x2D60
                        DW offset Func0x2d69;Offset 0x2D69
                        DW offset Func0x2d8a;Offset 0x2D8A
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2db6;Offset 0x2DB6
                        DW offset Func0x2db4;Offset 0x2DB4
                        DW offset Func0x2dbd;Offset 0x2DBD
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2dd7;Offset 0x2DD7
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2ddc;Offset 0x2DDC
                        DW offset Func0x2dfa;Offset 0x2DFA
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2e31;Offset 0x2E31
                        DW offset Func0x2d68;Offset 0x2D68
                        DW offset Func0x2e3e;Offset 0x2E3E
                        DW offset Func0x2e57;Offset 0x2E57
                        DW offset Func0x2e5e;Offset 0x2E5E
                        DW offset Func0x2e68;Offset 0x2E68
                        DW offset Func0x2e89;Offset 0x2E89

DACFunctions PROC NEAR                  ;Offset 0x2d4a
    cmp  al, 1bh;27
    ja   Func0x2d68                     ;Offset 0x2d68
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487, 08h
    jne  Func0x2d68                     ;Offset 0x2d68
    xor  ah, ah
    shl  ax, 01h
    mov  si, ax
    jmp  word ptr cs:[si + offset DACFunctionTable];Offset 0x2d12
DACFunctions ENDP

Func0x2d60 PROC NEAR                    ;Offset 0x2d60
    mov  bl, 11h
Func0x2d60 ENDP

Func0x2d62 PROC NEAR                    ;Offset 0x2d62
    call Func0x2a31                     ;Offset 0x2a31
    call Func0x2a13                     ;Offset 0x2a13
Func0x2d62 ENDP

Func0x2d68 PROC NEAR                    ;Offset 0x2d68
    ret;done. just a ret
Func0x2d68 ENDP

Func0x2d69 PROC NEAR                    ;Offset 0x2d69
    mov  cx, 10h
    xor  bl, bl
    mov  si, dx
Label0x2d70:                            ;Offset 0x2d70
    mov  bh, byte ptr es:[si]
    inc  si
    call Func0x2a31                     ;Offset 0x2a31
    call Func0x2a13                     ;Offset 0x2a13
    inc  bl
    loop Label0x2d70                    ;Offset 0x2d70
    mov  bh, byte ptr es:[si]
    call Func0x2a13                     ;Offset 0x2a13
    inc  bl
    call Func0x2a31                     ;Offset 0x2a31
    ret
Func0x2d69 ENDP

Func0x2d8a PROC NEAR                    ;Offset 0x2d8a
    mov  bl, 10h
    call Func0x2e9e                     ;Offset 0x2e9e
    cmp  byte ptr ss:[bp + 0eh], 00h
    je   Label0x2da5                    ;Offset 0x2da5
    cmp  byte ptr ss:[bp + 0eh], 01h
    jne  Label0x2db3                    ;Offset 0x2db3
    or   bh, 08h
    mov  al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    or   al, 20h
    jmp  Label0x2dad                    ;Offset 0x2dad
Label0x2da5:                            ;Offset 0x2da5
    and  bh, 0f7h
    mov  al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    and  al, 0dfh
Label0x2dad:                            ;Offset 0x2dad
    mov  byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    call Func0x2a31                     ;Offset 0x2a31
Label0x2db3:                            ;Offset 0x2db3
    ret  
Func0x2d8a ENDP

Func0x2db4 PROC NEAR                    ;Offset 0x2db4
    mov  bl, 11h
Func0x2db4 ENDP

Func0x2db6 PROC NEAR                    ;Offset 0x2db6
    call Func0x2e9e                     ;Offset 0x2e9e
    mov  byte ptr ss:[bp + 0fh], bh
    ret
Func0x2db6 ENDP

Func0x2dbd PROC NEAR                    ;Offset 0x2dbd
    mov   cx, 10h
    xor   bl, bl
    mov   di, dx
Label0x2dc4:                            ;Offset 0x2dc4
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   al, bh
    stosb byte ptr es:[di]
    inc   bl
    loop  Label0x2dc4                   ;Offset 0x2dc4
    inc   bl
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   al, bh
    stosb byte ptr es:[di]
    ret   
Func0x2dbd ENDP

Func0x2dd7 PROC NEAR                    ;Offset 0x2dd7
    mov   ah, dh
    jmp   Func0x2ec2                    ;Offset 0x2ec2
Func0x2dd7 ENDP

Func0x2ddc PROC NEAR                    ;Offset 0x2ddc
    call  TurnOffScreen                 ;Offset 0x47ed
    mov   si, dx
    mov   di, cx
NextColor:                              ;Offset 0x2de3
    lodsw word ptr es:[si]
    mov   ch, ah
    mov   ah, al
    lodsb byte ptr es:[si]
    mov   cl, al
    call  Func0x2eed                    ;Offset 0x2eed
    call  SetPaletteColor               ;Offset 0x47d8
    dec   di
    jne   NextColor                     ;Offset 0x2de3
    call  TurnOnScreen                  ;Offset 0x4800
    ret
Func0x2ddc ENDP

Func0x2dfa PROC NEAR                    ;Offset 0x2dfa
    or    bl, bl
    je    Label0x2e1a                   ;Offset 0x2e1a
    mov   bl, 10h
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   al, byte ptr ss:[bp + 0fh]
    and   al, 0fh
    test  bh, 80h
    jne   Label0x2e12                   ;Offset 0x2e12
    and   al, 03h
    shl   al, 02h
Label0x2e12:                            ;Offset 0x2e12
    mov   bl, 14h
    mov   bh, al
    call  Func0x2a31                    ;Offset 0x2a31
    ret
Label0x2e1a:                            ;Offset 0x2e1a
    mov   bl, 10h
    call  Func0x2e9e                    ;Offset 0x2e9e
    and   bh, 7fh
    cmp   byte ptr ss:[bp + 0fh], 00h
    je    Label0x2e2b                   ;Offset 0x2e2b
    or    bh, 80h
Label0x2e2b:                            ;Offset 0x2e2b
    mov   bl, 10h
    call  Func0x2a31                    ;Offset 0x2a31
    ret   
Func0x2dfa ENDP

Func0x2e31 PROC NEAR                    ;Offset 0x2e31
    call  Func0x2ed6                    ;Offset 0x2ed6
    mov   byte ptr ss:[bp + 0bh], ah
    mov   byte ptr ss:[bp + 0dh], ch
    mov   byte ptr ss:[bp + 0ch], cl
    ret   
Func0x2e31 ENDP

Func0x2e3e PROC NEAR                    ;Offset 0x2e3e
    call  TurnOffScreen                 ;Offset 0x47ed
    mov   di, dx
    mov   si, cx
NextColor:                              ;Offset 0x2e45
    call  GetPaletteColor               ;Offset 0x47bf
    mov   al, ah
    mov   ah, ch
    stosw word ptr es:[di]
    mov   al, cl
    stosb byte ptr es:[di]
    dec   si
    jne   NextColor                     ;Offset 0x2e45
    call  TurnOnScreen                  ;Offset 0x4800
    ret
Func0x2e3e ENDP

Func0x2e57 PROC NEAR                    ;Offset 0x2e57
    mov   dx, 03c6h                     ;port - 0x3c6
    mov   al, bl
    out   dx, al
    ret
Func0x2e57 ENDP

Func0x2e5e PROC NEAR                    ;Offset 0x2e5e
    mov   dx, 03c6h                     ;Offset 0x3c6
    in    al, dx
    xor   ah, ah
    mov   word ptr ss:[bp + 0eh], ax
    ret
Func0x2e5e ENDP

Func0x2e68 PROC NEAR                    ;Offset 0x2e68
    mov   bl, 14h
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   cl, bh
    and   cl, 0fh
    mov   bl, 10h
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   bl, 01h
    test  bh, 80h
    jne   Label0x2e83                   ;Offset 0x2e83
    mov   bl, 00h
    shr   cl, 02h
Label0x2e83:                            ;Offset 0x2e83
    mov   bh, cl
    mov   word ptr ss:[bp + 0eh], bx
    ret
Func0x2e68 ENDP

Func0x2e89 PROC NEAR                    ;Offset 0x2e89
    call  TurnOffScreen                 ;Offset 0x47ed
    mov   si, cx
NextColor:                              ;Offset 0x2e8e
    call  GetPaletteColor               ;Offset 0x47bf
    call  Func0x2ef4                    ;Offset 0x2ef4
    dec   bx
    call  SetPaletteColor               ;Offset 0x47d8
    dec   si
    jne   NextColor                     ;Offset 0x2e8e
    jmp   TurnOnScreen                  ;Offset 0x4800
Func0x2e89 ENDP

Func0x2e9e PROC NEAR                    ;Offset 0x2e9e
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h
Label0x2ea6:                            ;Offset 0x2ea6
    in    al, dx
    and   al, 08h
    je    Label0x2ea6                   ;Offset 0x2ea6
    push  dx
    mov   dx, 03c0h                     ;Offset 0x3c0
    mov   al, bl
    cli
    out   dx, al
    inc   dx
    in    al, dx
    pop   dx
    mov   bh, al
    in    al, dx
    mov   dx, 03c0h                     ;Offset 0x3c0
    mov   al, 20h
    out   dx, al
    sti
    pop   dx
    ret
Func0x2e9e ENDP

Func0x2ec2 PROC NEAR                    ;Offset 0x2ec2
    call  Func0x2eed                    ;Offset 0x2eed
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h
    pushf
    cli
Label0x2ece:                            ;Offset 0x2ece
    in    al, dx
    and   al, 08h
    je    Label0x2ece                   ;Offset 0x2ece
    jmp   SetPaletteColorInternal       ;Offset 0x47da
Func0x2ec2 ENDP

Func0x2ed6 PROC NEAR                    ;Offset 0x2ed6
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h
    pushf
    cli
Label0x2edf:                            ;Offset 0x2edf
    in    al, dx
    and   al, 08h
    je    Label0x2edf                   ;Offset 0x2edf
    jmp   GetPaletteColorInternal       ;Offset 0x47c1
Func0x2ed6 ENDP

Data2ee7                DW 2666h        ; 9,830
Data2ee9                DW 4B85h        ;19,333
Data2eeb                DW 0E14h        ; 3,604

Func0x2eed PROC NEAR                    ;offset 0x2eed
    test byte ptr ds:[0489h], 06h       ;Offset 0x489
    je   Func0x2f38                     ;Offset 0x2f38
Func0x2eed ENDP

Func0x2ef4 PROC NEAR                    ;Offset 0x2ef4
    push bx
    push dx
    and  ax, 3f00h
    xchg al, ah
    mul  word ptr cs:[Data2ee7]         ;Offset 0x2ee7
    push dx
    push ax
    mov  al, ch
    and  al, 3fh
    xor  ah, ah
    mul  word ptr cs:[Data2ee9]         ;Offset 0x2ee9
    push dx
    push ax
    mov  al, cl
    and  al, 3fh
    xor  ah, ah
    mul  word ptr cs:[Data2eeb]         ;Offset 0x2eeb
    pop  bx
    add  ax, bx
    pop  bx
    adc  dx, bx
    pop  bx
    add  ax, bx
    pop  bx
    adc  dx, bx
    add  ax, ax
    adc  dx, dx
    add  ax, 8000h
    adc  dx, 0
    mov  ah, dl
    mov  cl, dl
    mov  ch, dl
    pop  dx
    pop  bx
Func0x2ef4 ENDP
;continue!
Func0x2f38 PROC NEAR                    ;Offset 0x2f38
    ret;done
Func0x2f38 ENDP

TextModeCharCallTable   DW offset SetTextFontAndAddressingMode;Offset 0x2f8f
                        DW offset Func0x2f92;Offset 0x2f92
                        DW offset Func0x2fa3;Offset 0x2fa3
                        DW offset SelectCharacterFont;Offset 0x2fb4
                        DW offset Func0x2fbd;Offset 0x2fbd
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2fce;Offset 0x2fce
                        DW offset Func0x2fd4;Offset 0x2fd4
                        DW offset Func0x2fe8;Offset 0x2fe8
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2ffc;Offset 0x2ffc
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x30e1;Offset 0x30e1
                        DW offset Func0x3125;Offset 0x3125
                        DW offset Func0x30f5;Offset 0x30f5
                        DW offset Func0x3104;Offset 0x3104
                        DW offset Func0x3113;Offset 0x3113
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x2f8e;Offset 0x2f8e
                        DW offset Func0x3166;Offset 0x3166

TextModeCharFunctions PROC NEAR         ;Offset 0x2f6b
    mov  si, word ptr ss:[bp + 04h]
    and  bl, 3fh
TextModeCharFunctions ENDP
;continue!

;inputs:
;al = function flags
;bl = character generator ram bank
Func0x2f71 PROC NEAR                    ;Offset 0x2f71
    call Func0x227                      ;Offset 0x227 ands the top 2 bits off bl if al == 55
    mov  ah, al                         ;ah is now ?
    and  al, 0fh                        ;keep bits 3-0
    and  ah, 30h                        ;keep bits 5-4
    shr  ah, 01h                        ;now 4-3 
    or   al, ah                         ;mush together. Effectively, bit 4 and 3 overlap. If either is set, or neither is set.
    cmp  al, 19h                        ;if over 25, leave
    jae  Func0x2f8e                     ;Offset 0x2f8e
    mov  ah, 00h
    mov  di, ax
    shl  di, 01h
    jmp  word ptr cs:[di + TextModeCharCallTable];Offset 0x2f39
Func0x2f71 ENDP
;continue!
Func0x2f8e PROC NEAR                    ;Offset 0x2f8e
    ret
Func0x2f8e ENDP

SetTextFontAndAddressingMode PROC NEAR  ;Offset 0x2f8f
    jmp  SetTextFontAndAddressing       ;Offset 0x46c2
SetTextFontAndAddressingMode ENDP

Func0x2f92 PROC NEAR                    ;Offset 0x2f92
    mov  si, offset Characters8x14      ;Offset 0x5f20
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 0100h
    mov  bh, 0eh
    jmp  SetTextFontAndAddressing       ;Offset 0x46c2
Func0x2f92 ENDP

Func0x2fa3 PROC NEAR                    ;Offset 0x2fa3
    mov  si, offset LowerCharacters8x8  ;Offset 0x5720
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 100h
    mov  bh, 08h
    jmp  SetTextFontAndAddressing       ;Offset 0x46c2
Func0x2fa3 ENDP

SelectCharacterFont PROC NEAR           ;Offset 0x2fb4
    mov  al, 03h                        ;SR3 - Character Font Select register
    mov  ah, bl
    mov  dx, SequenceIndex              ;port - 0x3c4
    out  dx, ax
    ret  
SelectCharacterFont ENDP

Func0x2fbd PROC NEAR                    ;Offset 0x2fbd
    mov  si, offset Characters8x16      ;Offset 0x6e30
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 100h
    mov  bh, 10h
    jmp  SetTextFontAndAddressing       ;Offset 0x46c2
Func0x2fbd ENDP

Func0x2fce PROC NEAR                    ;Offset 0x2fce
    call SetTextFontAndAddressing       ;Offset 0x46c2
    jmp  Func0x3010                     ;Offset 0x3010
    nop
Func0x2fce ENDP

Func0x2fd4 PROC NEAR                    ;Offset 0x2fd4
    mov  si, offset Characters8x14      ;Offset 0x5f20
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 100h
    mov  bh, 0eh
    call SetTextFontAndAddressing       ;Offset 0x46c2
    jmp  Func0x3010                     ;Offset 0x3010
    nop
Func0x2fd4 ENDP

Func0x2fe8 PROC NEAR                    ;Offset 0x2fe8
    mov  si, offset LowerCharacters8x8  ;Offset 0x5720
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 100h
    mov  bh, 08h
    call SetTextFontAndAddressing       ;Offset 0x46c2
    jmp  Func0x3010                     ;Offset 0x3010
    nop
Func0x2fe8 ENDP

Func0x2ffc PROC NEAR                    ;Offset 0x2ffc
    mov  si, offset Characters8x16      ;Offset 0x6e30
    mov  ax, cs
    mov  es, ax
    xor  dx, dx
    mov  cx, 100h
    mov  bh, 10h
    call SetTextFontAndAddressing       ;Offset 0x46c2
    jmp  Func0x3010                     ;Offset 0x3010
    nop  
Func0x2ffc ENDP

Func0x3010 PROC NEAR                    ;Offset 0x3010
    mov  byte ptr ds:[BDA_PointHeightOfCharacterMatrix], bh;Offset 0x485
    mov  bx, 0190h
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp  al, BDA_DM_640x480_BW_Graphics ;11h
    je   Label0x306c                    ;Offset 0x306c
    cmp  al, BDA_DM_640x480_16_Color_Graphics;12h
    je   Label0x306c                    ;Offset 0x306c
    mov  bx, 0c8h
    cmp  al, BDA_DM_320x200_256_Color_Graphics;13h
    je   Label0x306c                    ;Offset 0x306c
    cmp  al, BDA_DM_320x200_4_Color_Graphics1;04h
    jb   Label0x3040                    ;Offset 0x3040
    cmp  al, BDA_DM_640x200_BW_Graphics ;06h
    jbe  Label0x306c                    ;Offset 0x306c
    cmp  al, BDA_DM_Unknown2            ;09h
    jb   Label0x3040                    ;Offset 0x3040
    cmp  al, BDA_DM_640x200_16_Color_Graphics;0eh
    jbe  Label0x306c                    ;Offset 0x306c
    mov  bx, 015eh
    cmp  al, BDA_DM_640x350_16_Color_Graphics;10h
    jbe  Label0x306c                    ;Offset 0x306c
Label0x3040:                            ;Offset 0x3040
    mov  bx, 190h
    test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
    jne  Label0x306c                    ;Offset 0x306c
    mov  bx, 015eh
    test byte ptr ds:[BDA_VideoModeOptions], BDA_VDDA_GrayScale;Offset 0x487, 0x2
    jne  Label0x306c                    ;Offset 0x306c
    mov  ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and  ah, 0fh
    cmp  ah, 03h
    je   Label0x306c                    ;Offset 0x306c
    cmp  ah, 09h
    je   Label0x306c                    ;Offset 0x306c
    cmp  al, BDA_DM_80x25_Monochrome_Text;07h
    je   Label0x306c                    ;Offset 0x306c
    mov  bx, 0c8h
Label0x306c:                            ;Offset 0x306c
    mov  ax, bx
    xor  dx, dx
    div  word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec  al
    mov  byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    inc  al
    mov  cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl  cx, 01h
    xor  ah, ah
    mul  cx
    add  ax, 100h
    mov  word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov  ah, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec  ah
    cmp  byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449, 0x7
    jne  Label0x309f                    ;Offset 0x309f
    mov  al, 14h
    out  dx, ax
Label0x309f:                            ;Offset 0x309f
    mov  ch, ah
    mov  cl, ah
    mov  al, 09h
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0e0h
    or   ah, ch
    out  dx, ax
    dec  ch
    mov  ah, cl
    cmp  ah, 0ch
    jbe  Label0x30bb                    ;Offset 0x30bb
    sub  cx, 0101h
Label0x30bb:                            ;Offset 0x30bb
    mov  word ptr ds:[BDA_CursorEndScanLine], cx;Offset 0x460
    mov  al, 0ah
    mov  ah, ch
    out  dx, ax
    inc  al
    mov  ah, cl
    out  dx, ax
    mov  al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    inc  al
    mul  byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp  bx, 0c8h
    jne  Label0x30da                    ;Offset 0x30da
    shl  ax, 01h
Label0x30da:                            ;Offset 0x30da
    dec  ax
    mov  ah, al
    mov  al, 12h
    out  dx, ax
    ret  
Func0x3010 ENDP

Func0x30e1 PROC NEAR                    ;Offset 0x30e1
    mov  di, es
    mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
    cli
    mov  word ptr es:[7ch], si          ;Offset 0x7c
    mov  word ptr es:[7eh], di          ;Offset 0x7e
    sti
    ret
Func0x30e1 ENDP

Func0x30f5 PROC NEAR                    ;Offset 0x30f5
    mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  si, offset Characters8x14      ;Offset 0x5f20
    mov  di, cs
    mov  cx, 0eh
    jmp  Func0x312c                     ;Offset 0x312c
Func0x30f5 ENDP

Func0x3104 PROC NEAR                    ;Offset 0x3104
    mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  si, offset LowerCharacters8x8  ;Offset 0x5720
    mov  di, cs
    mov  cx, 08h
    jmp  Func0x312c                     ;Offset 0x312c
Func0x3104 ENDP

Func0x3113 PROC NEAR                    ;Offset 0x3113
    mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  si, offset Characters8x16      ;Offset 0x6e30
    mov  di, cs
    mov  cx, 10h
    jmp  Func0x312c                     ;Offset 0x312c
Func0x3113 ENDP

Data3122 DB 0Dh, 18h, 2Ah

Func0x3125 PROC NEAR                    ;Offset 0x3125
    mov  di, es
    mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
Func0x3125 ENDP
;continue!
Func0x312c PROC NEAR                    ;Offset 0x312c
    cli
    mov  word ptr es:[010ch], si        ;Offset 0x10c
    mov  word ptr es:[010eh], di        ;Offset 0x10e
    sti
    cmp  bl, 04h
    jb   Label0x313f                    ;Offset 0x313f
    mov  bl, 02h
Label0x313f:                            ;Offset 0x313f
    dec  dl
    or   bl, bl
    je   Label0x314e                    ;Offset 0x314e
    dec  bl
    mov  bh, 00h
    mov  dl, byte ptr cs:[bx + offset Data3122]
Label0x314e:                            ;Offset 0x314e
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
    mov  byte ptr [bp + 10h], dl
    ret
Func0x312c ENDP

;Offset 0x315a
Data315a                DW offset Characters8x14;Offset 0x05f20
                        DW offset LowerCharacters8x8;Offset 0x05720
                        DW offset UpperCharacters8x8;Offset 0x05b20
                        DW offset Patch8x14;Offset 0x06d20
                        DW offset Characters8x16;Offset 0x06e30
                        DW offset Patch8x16;Offset 0x07e30

Func0x3166 PROC NEAR                    ;Offset 0x3166
    mov es, word ptr cs:[Data1488]      ;Offset 0x1488
    or  bh, bh
    jne Label0x3176                     ;Offset 0x3176
    les bx, es:[7ch]
    jmp Label0x3197                     ;Offset 0x3197
Label0x3176:                            ;Offset 0x3176
    dec bh
    jne Label0x3181                     ;Offset 0x3181
    les bx, es:[10ch]
    jmp Label0x3197                     ;Offset 0x3197
Label0x3181:                            ;Offset 0x3181
    dec bh
    cmp bh, 05h
    ja  Exit                            ;Offset 0x31ae
    mov ax, cs
    mov es, ax
    mov bl, bh
    mov bh, 00h
    add bx, bx
    mov bx, word ptr cs:[bx + offset Data315a];Offset 0x315a
Label0x3197:                            ;Offset 0x3197
    mov word ptr ss:[bp + 04h], bx
    mov word ptr ss:[bp + 02h], es
    mov ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov word ptr ss:[bp + 0ch], ax
    mov al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov byte ptr ss:[bp + 0ah], al
    mov word ptr ss:[bp + 10h], 0000h
Exit:                                   ;Offset 0x31ae
    ret
Func0x3166 ENDP

;Offset 0x31af
AltFuncCallTable        DW offset Func0x3224;Offset 0x3224
                        DW offset Func0x32d3;Offset 0x32d3
                        DW offset Func0x32e7;Offset 0x32e7
                        DW offset Func0x32fe;Offset 0x32fe
                        DW offset Func0x3317;Offset 0x3317
                        DW offset Func0x3333;Offset 0x3333
                        DW offset Func0x33b8;Offset 0x33b8

AlternateFunctions PROC NEAR            ;Offset 0x31bd
    sub bl, 10h
    je  Label0x31df                     ;Offset 0x31df
    sub bl, 10h
    je  Exit2                           ;Offset 0x320b
    sub bl, 10h
    jb  Exit1                           ;Offset 0x31de
    cmp bl, 06h
    ja  Exit1                           ;Offset 0x31de
    xor bh, bh
    shl bx, 01h
    mov byte ptr ss:[bp + 10h], 12h
    jmp word ptr cs:[bx + offset AltFuncCallTable];Offset 0x31af
Exit1:                                  ;Offset 0x31de
    ret
Label0x31df:                            ;Offset 0x31df
    mov ch, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    mov cl, 04h
    mov al, ch
    shr ch, cl
    mov cl, al
    and cl, 0fh
    mov word ptr ss:[bp + 0ch], cx
    mov al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shr al, 01h
    and al, 01h
    mov byte ptr ss:[bp + 0fh], al
    mov al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and al, 7fh
    mov cl, 05h
    shr al, cl
    mov byte ptr ss:[bp + 0eh], al
    mov byte ptr ss:[bp + 10h], al
    ret
Exit2:                                  ;Offset 0x320b
    ret
AlternateFunctions ENDP

;Offset 0x320c
Func0x3224CallTable     DW offset Func0x325a;Offset 0x325a
                        DW offset Func0x3255;Offset 0x3255
                        DW offset Func0x3250;Offset 0x3250
                        DW offset Func0x326a;Offset 0x326a
                        DW offset Func0x3284;Offset 0x3284
                        DW offset Func0x327f;Offset 0x327f
                        DW offset Func0x3299;Offset 0x3299
                        DW offset Func0x3294;Offset 0x3294
                        DW offset Func0x32ae;Offset 0x32ae
                        DW offset Func0x32a9;Offset 0x32a9
                        DW offset Func0x32c3;Offset 0x32c3
                        DW offset Func0x32be;Offset 0x32be

Func0x3224 PROC NEAR                    ;Offset 0x3224
    cmp  al, 02h
    ja   Func0x3250                     ;Offset 0x3250
    test byte ptr ds:[BDA_VideoModeOptions], 08h;Offset 0x487
    jne  Func0x3250                     ;Offset 0x3250
    mov  bl, al
    xor  bh, bh
    shl  bx, 03h
    mov  si, bx
    mov  bl, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and  bl, BDA_VMO_Monochrome         ;02h
    mov  al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and  al, BDA_VDDA_VGA               ;01h
    or   bl, al
    and  bl, 03h
    shl  bx, 01h
    jmp  word ptr cs:[bx + si + offset Func0x3224CallTable];Offset 0x320c
Func0x3224 ENDP

Func0x3250 PROC NEAR                    ;Offset 0x3250
    mov  byte ptr ss:[bp + 10h], 00h
    ret
Func0x3250 ENDP

Func0x3255 PROC NEAR                    ;Offset 0x3255
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x489, 0x80
Func0x3255 ENDP

Func0x325a PROC NEAR                    ;Offset 0x325a
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode400;Offset 0x489, 0xef
    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_MDAHiRes80x25_2;Offset 0x488, 0x8
    ret
Func0x325a ENDP

Func0x326a PROC NEAR                    ;Offset 0x326a
    or  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x489, 0x80
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode400;Offset 0x489, 0xef
    and byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
    ret
Func0x326a ENDP

Func0x327f PROC NEAR                    ;Offset 0x327f
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489. 0x7f
Func0x327f ENDP
;continue!
Func0x3284 PROC NEAR                    ;Offset 0x3284
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode400;Offset 0x489, 0xef
    and byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_MDAHiResEnhanced_2;Offset 0x488, 0x9
    ret
Func0x3284 ENDP

Func0x3294 PROC NEAR                    ;Offset 0x3294
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
Func0x3294 ENDP
;continue!
Func0x3299 PROC NEAR                    ;Offset 0x3299
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode400;Offset 0x489, 0xef
    and byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
    ret
Func0x3299 ENDP

Func0x32a9 PROC NEAR                    ;Offset 0x32a9
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
Func0x32a9 ENDP
;continue!
Func0x32ae PROC NEAR                    ;Offset 0x32ae
    or  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
    and byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_MDAHiResEnhanced_2;Offset 0x488, 0x9
    ret
Func0x32ae ENDP

Func0x32be PROC NEAR                    ;Offset 0x32be
    and byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
Func0x32be ENDP
;continue!
Func0x32c3 PROC NEAR                    ;Offset 0x32c3
    or  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
    and byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
    or  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
    ret
Func0x32c3 ENDP

Func0x32d3 PROC NEAR                    ;Offset 0x32d3
    cmp   al, BDA_VDDA_VGA
    ja    Func0x3312                    ;Offset 0x3312
    mov   al, 00h
    jne   Label0x32dd                   ;Offset 0x32dd
    mov   al, BDA_VDDA_DefaultPaletteDisabled
Label0x32dd:                            ;Offset 0x32dd
    and   byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_DefaultPaletteDisabled;Offset 0x489, 0xf7
    or    byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    ret   
Func0x32d3 ENDP

Func0x32e7 PROC NEAR                    ;Offset 0x32e7
    cmp   al, 01h
    ja    Func0x3312                    ;Offset 0x3312
    push  ax
    mov   al, 00h
    je    Label0x32f2                   ;Offset 0x32f2
    mov   al, 0eh
Label0x32f2:                            ;Offset 0x32f2
    mov   dx, VideoSubsystemEnableWO    ;port - 0x46e8
    out   dx, al
    pop   ax
    mov   dx, VideoSubsystemEnable      ;port - 0x3c3
    xor   al, 01h
    out   dx, al
    ret   
Func0x32e7 ENDP

Func0x32fe PROC NEAR                    ;Offset 0x32fe
    cmp   al, BDA_VDDA_VGA              ;01h
    ja    Func0x3312                    ;Offset 0x3312
    mov   al, 00h
    je    Label0x3308                   ;Offset 0x3308
    mov   al, BDA_VDDA_GrayScale        ;02h
Label0x3308:                            ;Offset 0x3308
    and   byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489, 0xfd
    or    byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    ret
Func0x32fe ENDP

Func0x3312 PROC NEAR                    ;Offset 0x3312
    mov   byte ptr ss:[bp + 10h], 00h
    ret   
Func0x3312 ENDP

Func0x3317 PROC NEAR                    ;Offset 0x3317
    cmp   al, BDA_VMO_CursorEmulationEnabled
    ja    Func0x3312                    ;Offset 0x3312
    mov   al, 00h
    jne   Label0x3321                   ;Offset 0x3321
    mov   al, BDA_VMO_CursorEmulationEnabled
Label0x3321:                            ;Offset 0x3321
    and   byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487
    or    byte ptr ds:[BDA_VideoModeOptions], al;Offset 0x487
    ret   
Func0x3317 ENDP

CallTable332b           DW offset Func0x334e;Offset 0x334e
                        DW offset Func0x334d;Offset 0x334d
                        DW offset Func0x3346;Offset 0x3346
                        DW offset Func0x338a;Offset 0x338a

Func0x3333 PROC NEAR                    ;Offset 0x3333
    mov   byte ptr ss:[bp + 10h], 00h
    cmp   al, 03h
    ja    Func0x334d                    ;Offset 0x334d
    mov   bl, al
    xor   bh, bh
    shl   bx, 01h
    jmp   word ptr cs:[bx + offset CallTable332b];Offset 0x332b
Func0x3333 ENDP

Func0x3346 PROC NEAR                    ;Offset 0x3346
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489, 0x40
    je    Func0x3363                    ;Offset 0x3363
Func0x3346 ENDP
;continue!
Func0x334d PROC NEAR                    ;Offset 0x334d
    ret
Func0x334d ENDP

Func0x334e PROC NEAR                    ;Offset 0x334e
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489, 0x40
    jne   Func0x334d                    ;Offset 0x334d
    mov   ax, 1280h
    mov   bl, 35h
    int   42h
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489, 0x40
    je    Func0x334d                    ;Offset 0x334d
Func0x334e ENDP
;continue!
Func0x3363 PROC NEAR                    ;Offset 0x3363
    mov   di, dx
    call  Func0x3976                    ;Offset 0x3976
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    mov   di, 1b4h                      ;Offset 0x1b4
    mov   si, 108h                      ;Offset 0x108
    movsw
    movsw
    mov   di, 108h                      ;Offset 0x108
    mov   ax, offset InterruptHandlerEntrypoint;Offset 0x56c6
    stosw
    mov   ax, cs
    stosw
    mov   dx, VideoSubsystemEnableWO    ;port - 0x46e8
    mov   al, 06h
    out   dx, al
    mov   byte ptr ss:[bp + 10h], 12h
    ret
Func0x3363 ENDP

Func0x338a PROC NEAR                    ;Offset 0x338a
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489, 0x40
    je    Func0x334d                    ;Offset 0x334d
    mov   di, dx
    call  Func0x39a6                    ;Offset 0x39a6
    mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    mov   di, 108h                      ;Offset 0x108
    mov   si, 1b4h                      ;Offset 0x1b4
    movsw
    movsw
    mov   di, 1b4h                      ;Offset 0x1b4
    mov   ax, offset InterruptHandlerEntrypoint;Offset 0x56c6
    stosw
    mov   ax, cs
    stosw
    mov   dx, VideoSubsystemEnableWO    ;port - 0x46e8
    mov   al, 0eh
    out   dx, al
    mov   byte ptr [bp + 10h], 12h
    ret
Func0x338a ENDP

Func0x33b8 PROC NEAR                    ;Offset 0x33b8
    cmp   al, 01h
    ja    Label0x33d3                   ;Offset 0x33d3
    mov   cl, 05h
    shl   al, cl
    mov   bl, al
    mov   dx, SequenceIndex             ;port - 0x3c4
    mov   al, 01h
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    and   ah, 0dfh
    or    ah, bl
    mov   al, 01h
    out   dx, ax
    ret
Label0x33d3:                            ;Offset 0x33d3
    mov   byte ptr [bp + 10h], 00h
    ret   
Func0x33b8 ENDP

EmptyFunction6 PROC NEAR                ;Offset 0x33d8
    ret   
EmptyFunction6 ENDP

WriteString PROC NEAR                   ;Offset 0x33d9
    jcxz  EmptyFunction6                ;Offset 0x33d8
    cmp   al, 3
    ja    EmptyFunction6                ;Offset 0x33d8
    mov   ah, bh
    xchg  byte ptr ds:[BDA_ActiveDisplayNumber], bh;Offset 0x462
    push  bx
    mov   bl, ah
    xor   bh, bh
    shl   bx, 01h
    test  al, 01h
    jne   Label0x33f4                   ;Offset 0x33f4
    push  word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
Label0x33f4:                            ;Offset 0x33f4
    mov   si, word ptr ss:[bp + 04h]
    mov   word ptr ds:[bx + DBA_CursorPositionPage0], dx;Offset 
    or    dx, dx
    jne   Label0x341d                   ;Offset 0x341d
    push  dx
    mov   dx, GraphicsControllerIndex   ;port - 0x3ce
    mov   al, 06h
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    pop   dx
    test  ah, 01h
    jne   Label0x341d                   ;Offset 0x341d
    or    bx, bx
    jne   Label0x341d                   ;Offset 0x341d
    cmp   byte ptr ss:[bp + 10h], 00h
    jne   Label0x341d                   ;Offset 0x341d
    call  Func0x34a1                    ;Offset 0x34a1
    je    Label0x346e                   ;Offset 0x346e
Label0x341d:                            ;Offset 0x341d
    push  cx
    mov   es, word ptr ss:[bp + 02h]
    lodsb byte ptr es:[si]
    cmp   al, 0dh
    jbe   Label0x347e                   ;Offset 0x347e
Label0x3427:                            ;Offset 0x3427
    mov   bx, word ptr ss:[bp + 0eh]
    test  byte ptr ss:[bp + 10h], 02h
    je    Label0x3434                   ;Offset 0x3434
    mov   bl, byte ptr es:[si]
    inc   si
Label0x3434:                            ;Offset 0x3434
    push  si
    push  dx
    push  bp
    mov   cx, 01h
    call  WriteCharAndAttrAtCursorPos   ;Offset 0x2670
    mov   ds, word ptr cs:[Data1488]    ;Offset 0x1488
    pop   bp
    pop   dx
    inc   dl
    cmp   dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb    Label0x345f                   ;Offset 0x345f
    inc   dh
    xor   dl, dl
    cmp   dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe   Label0x345f                   ;Offset 0x345f
    mov   al, 0ah
    pushf
    push  cs
    call  TeletypeOutput                ;Offset 0x2c35
    dec   dh
Label0x345f:                            ;Offset 0x345f
    mov   bl, byte ptr ss:[bp + 0fh]
    xor   bh, bh
    shl   bx, 01h
    mov   word ptr ds:[bx + DBA_CursorPositionPage0], dx;Offset 0x450
Label0x346a:                            ;Offset 0x346a
    pop   si
    pop   cx
    loop  Label0x341d                   ;Offset 0x341d
Label0x346e:                            ;Offset 0x346e
    test  byte ptr ss:[bp + 10h], 01h
    jne   Label0x3475                   ;Offset 0x3475
    pop   dx
Label0x3475:                            ;Offset 0x3475
    pop   bx
    xchg  byte ptr ds:[BDA_ActiveDisplayNumber], bh;Offset 0x462
    call  SetCursorPosition             ;Offset 0x1e4e
    ret
Label0x347e:                            ;Offset 0x347e
    cmp   al, 0dh
    je    Label0x348e                   ;Offset 0x348e
    cmp   al, 0ah
    je    Label0x348e                   ;Offset 0x348e
    cmp   al, 07h
    je    Label0x348e                   ;Offset 0x348e
    cmp   al, 08h
    jne   Label0x3427                   ;Offset 0x3427
Label0x348e:                            ;Offset 0x348e
    push  si
    pushf
    push  cs
    call  TeletypeOutput                ;Offset 0x2c35
    mov   bl, byte ptr ss:[bp + 0fh]
    xor   bh, bh
    shl   bx, 01h
    mov   dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    jmp   Label0x346a                   ;Offset 0x346a
WriteString ENDP

Func0x34a1 PROC NEAR                    ;Offset 0x34a1
    push  dx
    push  ds
    mov   dl, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and   dl, BDA_DH_InitialVideoModeMask;0x30
    cmp   dl, BDA_DH_80x25Monochrome    ;0x30
    mov   dx, 0b000h                    ;Segment 0xb000
    je    Label0x34b4                   ;Offset 0x34b4
    mov   dh, 0b8h                      ;Segment 0xb800
Label0x34b4:                            ;Offset 0x34b4
    mov   es, dx
    mov   ds, word ptr ss:[bp + 02h]
    xor   di, di
    mov   ah, byte ptr ss:[bp + 0eh]
Label0x34be:                            ;Offset 0x34be
    mov   al, byte ptr ds:[si]
    cmp   al, 0dh
    jbe   Label0x34cd                   ;Offset 0x34cd
Label0x34c4:                            ;Offset 0x34c4
    stosw
    inc   si
    loop  Label0x34be                   ;Offset 0x34be
    xor   al, al
    pop   ds
    pop   dx
    ret
Label0x34cd:                            ;Offset 0x34cd
    cmp  al, 0dh
    je   Label0x34dd                    ;Offset 0x34dd
    cmp  al, 0ah
    je   Label0x34dd                    ;Offset 0x34dd
    cmp  al, 07h
    je   Label0x34dd                    ;Offset 0x34dd
    cmp  al, 08h
    jne  Label0x34c4                    ;Offset 0x34c4
Label0x34dd:                            ;Offset 0x34dd
    or   al, 0ffh
    pop  ds
    pop  dx
    ret
Func0x34a1 ENDP

DisplayCombinationCodeFuncs PROC NEAR   ;Offset 0x34e2
    or          al, al
    jne         Label0x350f             ;Offset 0x350f
    call        Func0x3556              ;Offset 0x3556
    je          Label0x3506             ;Offset 0x3506
    or          cl, cl
    je          Label0x3504             ;Offset 0x3504
    mov         al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and         al, BDA_DH_InitialVideoModeMask;0x30
    cmp         al, BDA_DH_80x25Monochrome;0x30
    je          Label0x34ff             ;Offset 0x34ff
    test        cl, 01h
    jne         Label0x3504             ;Offset 0x3504
    jmp         Label0x3506             ;Offset 0x3506
Label0x34ff:                            ;Offset 0x34ff
    test        cl, 01h
    jne         Label0x3506             ;Offset 0x3506
Label0x3504:                            ;Offset 0x3504
    xchg        cl, ch
Label0x3506:                            ;Offset 0x3506
    mov         word ptr ss:[bp + 0eh], cx
    mov         word ptr ss:[bp + 10h], 1ah
    ret
Label0x350f:                            ;Offset 0x350f
    dec         al
    jne         Label0x3555             ;Offset 0x3555
    mov         dl, 0ffh
    mov         bx, 0010h               ;Secondary Save Pointer Table pointer (VGA)
    call        GetVideoParameterBlockElement;Offset 0x1d95
    je          Label0x354c             ;Offset 0x354c
    les         bx, es:[bx + 02h]
    mov         ax, es
    or          ax, bx
    je          Label0x354c             ;Offset 0x354c
    mov         cl, byte ptr es:[bx]
    xor         ch, ch
    jcxz        Label0x354c             ;Offset 0x354c
    mov         di, bx
    add         di, 0004h
    mov         ax, word ptr ss:[bp + 0eh]
    repne scasw word ptr es:[di]
    je          Label0x3548             ;Offset 0x3548
    mov         cl, byte ptr es:[bx]
    mov         di, bx
    add         di, 04h
    xchg        al, ah
    repne scasw word ptr es:[di]
    jne         Label0x354c             ;Offset 0x354c
Label0x3548:                            ;Offset 0x3548
    mov         dl, 0fh
    sub         dl, cl
Label0x354c:                            ;Offset 0x354c
    mov         byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], dl;Offset 0x48a
    mov         word ptr ss:[bp + 10h], 1ah
Label0x3555:                            ;Offset 0x3555
    ret
DisplayCombinationCodeFuncs ENDP

Func0x3556 PROC NEAR                    ;Offset 0x3556
    push        es
    mov         cx, 0ffffh
    mov         bx, 10h                 ;Secondary Save Pointer Table pointer (VGA)
    call        GetVideoParameterBlockElement;Offset 0x1d95
    je          Label0x3580             ;Offset 0x3580
    les         bx, es:[bx + 02h]
    mov         ax, es
    or          ax, bx
    je          Label0x3580             ;Offset 0x3580
    mov         al, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    cmp         al, byte ptr es:[bx]
    jae         Label0x3580             ;Offset 0x3580
    xor         ah, ah
    shl         ax, 01h
    DB 05h, 04h, 00h                    ;add         ax, 04h;masm encoding difference?
    mov         si, ax
    mov         cx, word ptr es:[bx + si]
Label0x3580:                            ;Offset 0x3580
    cmp         cx, 0ffffh
    pop         es
    ret
Func0x3556 ENDP

;Offset 0x3585
Data3585 DB 0Fh, 0Fh, 0Fh, 0Fh, 03h, 03h, 01h, 00h, 00h, 00h, 00h, 00h, 00h, 0Fh, 0Fh, 00h, 0Fh, 01h, 0Fh, 0FFh

;Offset 0x3599
Data3599 DB 08h, 08h, 08h, 08h, 01h, 01h, 01h, 08h, 00h, 00h, 00h, 00h, 00h, 08h, 04h, 02h, 02h, 01h, 01h, 01h

FunctionalityAndStateInfo PROC NEAR     ;Offset 0x35ad
    or        bx, bx
    je        Label0x35b6               ;Offset 0x35b6
    mov       byte ptr ss:[bp + 10h], 00h
    ret
Label0x35b6:                            ;Offset 0x35b6
    mov       byte ptr ss:[bp + 10h], 1bh
    mov       ax, offset Data3a99       ;Offset 0x3a99
    stosw
    mov       ax, cs
    stosw
    mov       cx, 0fh
    mov       si, BDA_DisplayMode       ;Offset 0x449
    rep movsw
    mov       al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    inc       al
    stosb
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    stosw
    call      Func0x3556                ;Offset 0x3556
    mov       ax, cx
    xchg      al, ah
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    xor       ah, ah
    mov       bx, ax
    mov       al, byte ptr cs:[bx + offset Data3585];Offset 0x3585
    cmp       al, 00h
    je        Label0x35ec               ;Offset 0x35ec
    inc       ax
Label0x35ec:                            ;Offset 0x35ec
    stosw
    mov       al, byte ptr cs:[bx + offset Data3599];Offset 0x3599
    stosb
    mov       bl, 03h
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_640x480_BW_Graphics;11h
    je        Label0x3645               ;Offset 0x3645
    cmp       al, BDA_DM_640x480_16_Color_Graphics;12h
    je        Label0x3645               ;Offset 0x3645
    mov       bl, 00h
    cmp       al, BDA_DM_320x200_256_Color_Graphics;13h
    je        Label0x3645               ;Offset 0x3645
    cmp       al, BDA_DM_320x200_4_Color_Graphics1;04h
    jb        Label0x361c               ;Offset 0x361c
    cmp       al, BDA_DM_640x200_BW_Graphics;06h
    jbe       Label0x3645               ;Offset 0x3645
    cmp       al, BDA_DM_Unknown2;09h
    jb        Label0x361c               ;Offset 0x361c
    cmp       al, BDA_DM_640x200_16_Color_Graphics;0eh
    jbe       Label0x3645               ;Offset 0x3645
    mov       bl, 01h
    cmp       al, BDA_DM_640x350_16_Color_Graphics;10h
    jbe       Label0x3645               ;Offset 0x3645
Label0x361c:                            ;Offset 0x361c
    mov       bl, 02h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
    jne       Label0x3645               ;Offset 0x3645
    mov       bl, 01h
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x02
    jne       Label0x3645               ;Offset 0x3645
    mov       ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       ah, BDA_EFBS_AdapterTypeMask;0fh
    cmp       ah, BDA_EFBS_MDAHiResEnhanced;03h
    je        Label0x3645               ;Offset 0x3645
    cmp       ah, BDA_EFBS_MDAHiResEnhanced_2;09h
    je        Label0x3645               ;Offset 0x3645
    cmp       al, BDA_EFBS_MDAColor80x25_2;07h
    je        Label0x3645               ;Offset 0x3645
    mov       bl, 00h
Label0x3645:                            ;Offset 0x3645
    mov       al, bl
    stosb
    mov       dx, SequenceIndex         ;port - 0x3c4
    mov       al, 03h
    call      ReadDataWithIndexRegisterNext;Offset 0x4649
    mov       al, ah
    and       al, 03h
    test      ah, 10h
    je        Label0x365b               ;Offset 0x365b
    or        al, 04h
Label0x365b:                            ;Offset 0x365b
    stosb
    mov       al, ah
    and       al, 0ch
    shr       al, 02h
    test      ah, 20h
    je        Label0x366a               ;Offset 0x366a
    or        al, 04h
Label0x366a:                            ;Offset 0x366a
    stosb
    mov       bl, 10h
    call      Func0x2e9e                ;Offset 0x2e9e
    mov       al, bh
    and       al, 08h
    shl       al, 02h
    mov       ah, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       ah, 0fh
    or        ah, al
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, 01h
    shl       al, 04h
    xor       al, 10h
    or        al, ah
    stosb
    xor       ax, ax
    stosw
    stosb
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shr       al, 05h
    and       al, 03h
    stosb
    xor       cl, cl
    push      es
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    mov       ax, word ptr es:[bx + 04h]
    or        ax, word ptr es:[bx + 06h]
    je        Label0x36ae               ;Offset 0x36ae
    or        cl, 02h
Label0x36ae:                            ;Offset 0x36ae
    mov       ax, word ptr es:[bx + 08h]
    or        ax, word ptr es:[bx + 0ah]
    je        Label0x36bb               ;Offset 0x36bb
    or        cl, 04h
Label0x36bb:                            ;Offset 0x36bb
    mov       ax, word ptr es:[bx + 0ch]
    or        ax, word ptr es:[bx + 0eh]
    je        Label0x36c8               ;Offset 0x36c8
    or        cl, 08h
Label0x36c8:                            ;Offset 0x36c8
    les       bx, es:[bx + 10h]
    mov       ax, es
    or        ax, bx
    je        Label0x36fb               ;Offset 0x36fb
    mov       ax, word ptr es:[bx + 06h]
    or        ax, word ptr es:[bx + 08h]
    je        Label0x36df               ;Offset 0x36df
    or        cl, 01h
Label0x36df:                            ;Offset 0x36df
    mov       ax, word ptr es:[bx + 0ah]
    or        ax, word ptr es:[bx + 0ch]
    je        Label0x36ec               ;Offset 0x36ec
    or        cl, 10h
Label0x36ec:                            ;Offset 0x36ec
    les       bx, es:[bx + 02h]
    mov       ax, es
    mov       bx, cs
    cmp       ax, bx
    je        Label0x36fb               ;Offset 0x36fb
    or        cl, 20h
Label0x36fb:                            ;Offset 0x36fb
    mov       al, cl
    pop       es
    stosb
    mov       cx, 0dh
    xor       al, al
    rep stosb
    ret
FunctionalityAndStateInfo ENDP

;Offset 0x3707
SaveRestoreCallTable    DW offset FetchStateBufferSize;Offset 0x373d
                        DW offset SaveVideoState;Offset 0x3755
                        DW offset RestoreVideoState;Offset 0x3887

SaveRestoreVideoState PROC NEAR         ;Offset 0x370d
    mov  byte ptr [bp + 10h], 00h
    test byte ptr cs:[Data0197], 08h    ;Offset 0x197 - Save / Restore functionality supported
    je   SaveRestoreNotSupported        ;Offset 0x371d
    mov  byte ptr [bp + 10h], 1ch
SaveRestoreNotSupported:                ;Offset 0x371d
    cmp  al, 02h                        ;0 - 2 valid values
    ja   EmptyFunction7                 ;Offset 0x372c
    xor  ah, ah
    shl  ax, 01h
    mov  si, ax
    jmp  word ptr cs:[si + offset SaveRestoreCallTable];Offset 0x3707
SaveRestoreVideoState ENDP

EmptyFunction7 PROC NEAR                ;Offset 0x372c
    ret
EmptyFunction7 ENDP

RequestedStateSizesIn64ByteBlocks DB 00h, 02h, 02h, 03h, 0Dh, 0Eh, 0Eh, 0Fh, 02h, 03h, 03h, 04h, 0Eh, 0Fh, 0Fh, 10h

;inputs:
;cl = requested states
;bit 0 - video hardware (see #00049)
;bit 1 - BIOS data areas
;bit 2 - color registers and DAC state (see #00050)
;bit 3 - S3 extended video hardware

FetchStateBufferSize PROC NEAR          ;Offset 0x373d
    and   cl, 0fh                       ;0x00 - 0xf bound check
    je    EmptyFunction7                ;Offset 0x372c - this jump is stupid when there's a perfectly good ret in this function already.
    mov   byte ptr [bp + 10h], 1ch      ;Function supported
    mov   bl, cl
    xor   bh, bh
    xor   ah, ah
    mov   al, byte ptr cs:[bx + offset RequestedStateSizesIn64ByteBlocks];Offset 0x372d
    mov   word ptr [bp + 0eh], ax
    ret
FetchStateBufferSize ENDP

SaveVideoState PROC NEAR                ;Offset 0x3755
    mov   di, bx
    add   di, 20h
    test  cl, 01h
    jne   Label0x3762                   ;Offset 0x3762
    jmp   Label0x3836                   ;Offset 0x3836
Label0x3762:                            ;Offset 0x3762
    mov   word ptr es:[bx], di
    mov   dx, SequenceIndex             ;port - 0x3c4
    in    al, dx
    stosb
    mov   dx, CRTControllerIndexD       ;port - 0x3d4
    in    al, dx
    stosb
    mov   dx, GraphicsControllerIndex   ;port - 0x3ce
    in    al, dx
    stosb
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dx, 06h
    in    al, dx
    mov   dx, AttributeControllerIndex  ;port - 0x3c0
    in    al, dx
    stosb
    mov   dx, FeatureControlR           ;port - 0x3ca
    in    al, dx
    stosb
    mov   dx, SequenceIndex             ;port - 0x3c4
    mov   cx, 04h
    mov   al, 01h
Label0x378e:                            ;Offset 0x378e
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    xchg  al, ah
    stosb
    xchg  al, ah
    loop  Label0x378e                   ;Offset 0x378e
    mov   dx, MiscellaneousRead         ;port - 0x3cc
    in    al, dx
    stosb
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov   cx, 19h
    mov   al, 00h
Label0x37a6:                            ;Offset 0x37a6
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    xchg  al, ah
    stosb
    xchg  al, ah
    loop  Label0x37a6                   ;Offset 0x37a6
    push  dx
    add   dx, 06h
    in    al, dx
    mov   dx, AttributeControllerIndex  ;port - 0x3c0
    mov   cx, 14h
    push  bx
    mov   bl, 00h
Label0x37be:                            ;Offset 0x37be
    call  Func0x2e9e                    ;Offset 0x2e9e
    mov   al, bh
    stosb
    inc   bl
    loop  Label0x37be                   ;Offset 0x37be
    pop   bx
    mov   dx, GraphicsControllerIndex   ;port - 0x3ce
    mov   cx, 09h
    mov   al, 00h
Label0x37d1:                            ;Offset 0x37d1
    call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    xchg  al, ah
    stosb
    xchg  al, ah
    loop  Label0x37d1                   ;Offset 0x37d1
    pop   ax
    stosw
    push  ds
    mov   ax, 0a000h                    ;Segment 0xa000
    mov   ds, ax
    mov   dx, SequenceIndex             ;port - 0x3c4
    mov   al, 02h
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax
    mov   ah, 0fh
    out   dx, ax
    mov   al, 04h
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax
    mov   ah, 07h
    out   dx, ax
    mov   dx, GraphicsControllerIndex   ;port - 0x3ce
    mov   al, 04h
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax
    mov   al, 05h
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax
    mov   ah, 01h
    out   dx, ax
    mov   al, 06h
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    push  ax
    mov   ah, 04h
    out   dx, ax
    mov   byte ptr ds:[0ffffh], 00h     ;Offset 0xffff
    mov   cx, 04h
    mov   ah, 00h
Label0x381d:                            ;Offset 0x381d
    mov   al, 04h
    out   dx, ax
    inc   ah
    mov   al, byte ptr ds:[0ffffh]      ;Offset 0xffff
    stosb
    loop  Label0x381d                   ;Offset 0x381d
    pop   ax
    out   dx, ax
    pop   ax
    out   dx, ax
    pop   ax
    out   dx, ax
    mov   dx, SequenceIndex             ;port - 0x3c4
    pop   ax
    out   dx, ax
    pop   ax
    out   dx, ax
    pop   ds
Label0x3836:                            ;Offset 0x3836
    test  byte ptr ss:[bp + 0ch], 02h
    je    Label0x3843                   ;Offset 0x3843
    mov   word ptr es:[bx + 02h], di
    call  Func0x3976                    ;Offset 0x3976
Label0x3843:                            ;Offset 0x3843
    test  byte ptr ss:[bp + 0ch], 04h
    je    Label0x3875                   ;Offset 0x3875
    mov   word ptr es:[bx + 4], di
    mov   dx, DACReadIndex              ;port - 0x3c7
    in    al, dx
    inc   dx
    and   al, 01h
    stosb
    in    al, dx
    je    Label0x385a                   ;Offset 0x385a
    dec   al
Label0x385a:                            ;Offset 0x385a
    stosb
    mov   dx, DACMask                   ;port - 0x3c6
    in    al, dx
    stosb
    push  bx
    xor   bx, bx
    mov   si, 100h
Label0x3866:                            ;Offset 0x3866
    call  GetPaletteColor               ;Offset 0x47bf
    mov   al, ah
    mov   ah, ch
    stosw
    mov   al, cl
    stosb
    dec   si
    jne   Label0x3866                   ;Offset 0x3866
    pop   bx
Label0x3875:                            ;Offset 0x3875
    test  byte ptr ss:[bp + 0ch], 08h
    je    Label0x3886                   ;Offset 0x3886
    mov   word ptr es:[bx + 06h], di
    mov   byte ptr ss:[bp + 10h], 1ch
    call  Func0x1384                    ;Offset 0x1384
Label0x3886:                            ;Offset 0x3886
    ret   
SaveVideoState ENDP

RestoreVideoState PROC NEAR             ;Offset 0x3887
    test      cl, 01h
    jne       Label0x388f               ;Offset 0x388f
    jmp       Label0x3914               ;Offset 0x3914
Label0x388f:                            ;Offset 0x388f
    mov       si, word ptr es:[bx]
    push      ds
    mov       dx, GraphicsControllerIndex;port - 0x3ce
    mov       ax, 0005h
    out       dx, ax
    mov       ax, 0406h
    out       dx, ax
    mov       dx, SequenceIndex         ;port - 0x3c4
    mov       ax, 0704h
    out       dx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       di, 0ffffh
    mov       ch, 04h
    mov       cl, 01h
    add       si, 42h
Label0x38b4:                            ;Offset 0x38b4
    mov       al, 02h
    mov       ah, cl
    out       dx, ax
    lodsb     byte ptr es:[si]
    mov       byte ptr ds:[di], al
    shl       cl, 01h
    dec       ch
    jne       Label0x38b4               ;Offset 0x38b4
    mov       al, 02h
    mov       ah, 0fh
    out       dx, ax
    mov       al, byte ptr ds:[di]
    pop       ds
    mov       si, word ptr es:[bx]
    mov       al, 00h
    xchg      byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    push      ax
    push      word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    call      ApplyVideoParameters      ;Offset 0x4829
    pop       word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    pop       ax
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    mov       si, word ptr es:[bx]
    mov       dx, SequenceIndex         ;port - 0x3c4
    add       si, 05h
    lodsb     byte ptr es:[si]
    mov       ah, 01h
    xchg      ah, al
    out       dx, ax
    mov       si, word ptr es:[bx]
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dx, CRTControllerIndexD   ;port - 0x3d4
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dx, GraphicsControllerIndex;port - 0x3ce
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dx, word ptr es:[si + 3dh]
    add       dx, 06h
    in        al, dx
    mov       dx, AttributeControllerIndex;port - 0x3c0
    lodsb     byte ptr es:[si]
    out       dx, al
Label0x3914:                            ;Offset 0x3914
    test      byte ptr ss:[bp + 0ch], 02h
    je        Label0x3921               ;Offset 0x3921
    mov       si, word ptr es:[bx + 02h]
    call      Func0x39a6                ;Offset 0x39a6
Label0x3921:                            ;Offset 0x3921
    test      byte ptr ss:[bp + 0ch], 04h
    je        Label0x3968               ;Offset 0x3968
    mov       si, word ptr es:[bx + 4]
    mov       byte ptr ss:[bp + 10h], 1ch
    mov       dx, DACMask               ;port - 0x3c6
    add       si, 02h
    lodsb     byte ptr es:[si]
    out       dx, al
    call      TurnOffScreen             ;Offset 0x47ed
    push      bx
    xor       bx, bx
    mov       dx, 100h
Label0x3941:                            ;Offset 0x3941
    push      dx
    lodsw     word ptr es:[si]
    mov       ch, ah
    mov       ah, al
    lodsb     byte ptr es:[si]
    mov       cl, al
    call      SetPaletteColor           ;Offset 0x47d8
    pop       dx
    dec       dx
    jne       Label0x3941               ;Offset 0x3941
    call      TurnOnScreen              ;Offset 0x4800
    pop       bx
    mov       si, word ptr es:[bx + 04h]
    mov       dx, DACStatus             ;Offset 0x3c7
    lodsb     byte ptr es:[si]
    or        al, al
    jne       Label0x3965               ;Offset 0x3965
    inc       dx
Label0x3965:                            ;Offset 0x3965
    lodsb     byte ptr es:[si]
    out       dx, al
Label0x3968:                            ;Offset 0x3968
    test      byte ptr ss:[bp + 0ch], 08h
    je        Label0x3975               ;Offset 0x3975
    mov       si, word ptr es:[bx + 06h]
    call      WriteSequencerCRTCAndAdvancedFunctionsRegisters;Offset 0x13e6
Label0x3975:                            ;Offset 0x3975
    ret       
RestoreVideoState ENDP

Func0x3976 PROC NEAR                    ;Offset 0x3976
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    mov       si, BDA_DisplayMode       ;Offset 0x449
    mov       cx, 0fh
    rep movsw
    mov       si, BDA_RowsOnScreen      ;Offset 0x484
    mov       cx, 07h
    rep movsb
    mov       si, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
    movsw
    movsw
    mov       si, 14h                   ;Offset 0x14
    movsw
    movsw
    mov       si, 74h                   ;Offset 0x74
    movsw
    movsw
    mov       si, 7ch                   ;Offset 0x7c
    movsw
    movsw
    mov       si, 10ch                  ;Offset 0x10c
    movsw
    movsw
    ret
Func0x3976 ENDP

Func0x39a6 PROC NEAR                    ;Offset 0x39a6
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410, 0xcf
    lodsb     byte ptr es:[si]
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    push      es
    push      ds
    mov       ax, ds
    mov       di, es
    xchg      ax, di
    mov       ds, ax
    mov       es, di
    mov       di, BDA_DisplayMode       ;Offset 0x449
    mov       cx, 0fh
    rep movsw
    mov       di, BDA_RowsOnScreen      ;Offset 0x484
    mov       cx, 07h
    rep movsb
    pop       ds
    pop       es
    lodsw     word ptr es:[si]
    mov       word ptr ds:[BDA_VideoParameterControlBlockPointer], ax;Offset 0x4a8
    lodsw     word ptr es:[si]
    mov       word ptr ds:[BDA_VideoParameterControlBlockPointer + 02h], ax;Offset 0x4aa
    mov       di, 14h                   ;Offset 0x14
    lodsw     word ptr es:[si]
    mov       word ptr [di], ax
    lodsw     word ptr es:[si]
    mov       word ptr [di + 02h], ax
    mov       di, 74h                   ;Offset 0x74
    lodsw     word ptr es:[si]
    mov       word ptr [di], ax
    lodsw     word ptr es:[si]
    mov       word ptr [di + 2], ax
    mov       di, 7ch                   ;Offset 0x7c
    lodsw     word ptr es:[si]
    mov       word ptr [di], ax
    lodsw     word ptr es:[si]
    mov       word ptr [di + 02h], ax
    mov       di, 10ch                  ;Offset 0x10c
    lodsw     word ptr es:[si]
    mov       word ptr [di], ax
    lodsw     word ptr es:[si]
    mov       word ptr [di + 02h], ax
    ret
Func0x39a6 ENDP

FARPointer STRUCT
    off WORD ?
    sgm WORD ?
FARPointer ENDS

VideoOverridePointerTable STRUCT
    VideoParameterTable FARPointer <?,?>
    DynamicParameterSaveArea FARPointer <?,?>
    AplhaNumericCharacterSetOverride FARPointer <?,?>
    GraphicsCharacterSetOverride FARPointer <?,?>
    SecondarySavePointerTable FARPointer <?,?>
    Reserved1 FARPointer <?,?>
    Reserved2 FARPointer <?,?>
VideoOverridePointerTable ENDS

;Alphanumeric Character Set Override
;	00   byte    length of each character definition in bytes
;	01   byte    character generator RAM bank
;	02   word    count of characters defined
;	04   word    first character code in table
;	06   dword   pointer to character font definition table
;	0A   byte    number of character rows displayed
;	0B   nbytes  array of applicable video modes
;	0B+n byte    FFh end of mode list marker


;Graphics Character Set Override
;	00   byte    count of displayed character rows
;	01   word    length of each character definition in bytes
;	03   dword   pointer to character font definition table
;	07   nbytes  array of applicable video modes
;	07+n byte    FFh end of mode list marker

;Offset 0x3a09
VideoOverrideTable1 VideoOverridePointerTable < <offset VideoParameters, 0C000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable1, 0C000h>, <00000h, 00000h>, <00000h, 00000h> >

;Offset 0x3a25
VideoOverrideTable2 VideoOverridePointerTable < <offset VideoParameters, 0E000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable2, 0E000h>, <00000h, 00000h>, <00000h, 00000h> >

SecondarySavePointerTable STRUCT
    LengthInBytes WORD ?
    DisplayCombinationCodeTable FARPointer <?,?>
    SecondaryAplhanumericCharSetOverride FARPointer <?,?>
    UserPaletteProfileTable FARPointer <?,?>
    Reserved1 FARPointer <?,?>
    Reserved2 FARPointer <?,?>
    Reserved3 FARPointer <?,?>
SecondarySavePointerTable ENDS

;Offset 0x3a41
SecondarySaveTable1 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0C000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >
;Offset 0x3a5b
SecondarySaveTable2 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0E000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >

DisplayCombinationCodeTable STRUCT
    TableEntryCount BYTE ?
    TableVersionNumber BYTE ?
    MaximumDisplayTypeCode BYTE ?
    Reserved BYTE ?
DisplayCombinationCodeTable ENDS

;Offset 0x3a75
DisplayCominationCodes DisplayCombinationCodeTable < 010h, 001h, 008h, 000h >
DB 000h, 000h;Table entries
DB 000h, 001h
DB 000h, 002h
DB 002h, 001h
DB 000h, 004h
DB 004h, 001h
DB 000h, 005h
DB 002h, 005h
DB 000h, 006h
DB 001h, 006h
DB 005h, 006h
DB 000h, 008h
DB 001h, 008h
DB 000h, 007h
DB 002h, 007h
DB 006h, 007h

;Offset 0x3a99 - what is this?
Data3a99 DB 0FFh, 0E0h, 00Fh, 000h, 000h, 000h, 000h, 007h, 002h, 008h, 0FFh, 00Eh, 000h, 000h, 03Fh, 000h

;The assembler doesn't like all that data on one line, so I am just overlapping these to make struct accesses work.
;If anyone knows a cleaner way, please let me know
VideoParameters VideoParametersTable 27 dup(<>);Offset 0x3aa9

ORG offset VideoParameters
;Offset 0x3aa9
DB 028h
DB 018h
DB 008h
DW 00800h
DD 002000309h
DB 063h
DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x3ae9
DB 028h
DB 018h
DB 008h
DW 00800h
DD 002000309h
DB 063h
DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 00FFh

;Offset 0x3b29
DB 050h
DB 018h
DB 008h
DW 01000h
DD 002000301h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x3b69
DB 050h
DB 018h
DB 008h
DW 01000h
DD 002000301h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x3ba9
DB 028h
DB 018h
DB 008h
DW 04000h
DD 002000309h
DB 063h
DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh 

;Offset 0x3be9
DB 028h
DB 018h
DB 008h
DW 04000h
DD 002000309h
DB 063h
DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh 

;Offset 0x3c29
DB 050h
DB 018h
DB 008h
DW 04000h
DD 006000101h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0C2h, 0FFh
DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh 

;Offset 0x3c69
DB 050h
DB 018h
DB 00Eh
DW 01000h
DD 003000300h
DB 0A6h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h, 0BAh, 0A3h, 0FFh
DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh 

;Offset 0x3ca9
DB 050h
DB 018h
DB 010h
DW 07D00h
DD 006000F21h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh 

;Offset 0x3ce9
DB 000h
DB 000h
DB 000h
DW 00000h
DD 000000000h
DB 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h 

;Offset 0x3d29
DB 028h
DB 018h
DB 008h
DW 04000h
DD 003000000h
DB 023h
DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x3d69
DB 050h
DB 000h
DB 000h
DW 00000h
DD 006000F29h
DB 062h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh 

;Offset 0x3da9
DB 050h
DB 000h
DB 000h
DW 00000h
DD 006000F29h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh 

;Offset 0x3de9
DB 028h
DB 018h
DB 008h
DW 02000h
DD 006000F09h
DB 063h
DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0E3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh 

;Offset 0x3e29
DB 050h
DB 018h
DB 008h
DW 04000h
DD 006000F01h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0E3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh 

;Offset 0x3e69
DB 050h
DB 018h
DB 00Eh
DW 08000h
DD 000000F05h
DB 0A2h
DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 000h, 05Eh, 06Eh, 08Bh, 0FFh
DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh 

;Offset 0x3ea9
DB 050h
DB 018h
DB 00Eh
DW 08000h
DD 000000F05h
DB 0A7h
DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh 

;Offset 0x3ee9
DB 050h
DB 018h
DB 00Eh
DW 08000h
DD 006000F01h
DB 0A2h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh 

;Offset 0x3f29
DB 050h
DB 018h
DB 00Eh
DW 08000h
DD 006000F01h
DB 0A3h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh 

;Offset 0x3f69
DB 028h
DB 018h
DB 00Eh
DW 00800h
DD 002000309h
DB 0A3h
DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x3fa9
DB 028h
DB 018h
DB 00Eh
DW 00800h
DD 002000309h
DB 0A3h
DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x3fe9
DB 050h
DB 018h
DB 00Eh
DW 01000h
DD 002000301h
DB 0A3h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x4029
DB 050h
DB 018h
DB 00Eh
DW 01000h
DD 002000301h
DB 0A3h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x4069
DB 028h
DB 018h
DB 010h
DW 00800h
DD 002000308h
DB 067h
DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x40a9
DB 050h
DB 018h
DB 010h
DW 01000h
DD 002000300h
DB 067h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh 

;Offset 0x40e9
DB 050h
DB 018h
DB 010h
DW 01000h
DD 002000300h
DB 066h
DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh 

;Offset 0x4129
DB 050h
DB 01Dh
DB 010h
DW 0A000h
DD 006000F01h
DB 0E3h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0C3h, 0FFh
DB 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh 

;Offset 0x4169
DB 050h
DB 01Dh
DB 010h
DW 0A000h
DD 006000F01h
DB 0E3h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0E3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh 

;Offset 0x41a9
DB 028h
DB 018h
DB 008h
DW 02000h
DD 00E000F01h
DB 063h
DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h, 0B9h, 0A3h, 0FFh
DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h
DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh 

;Offset 0x41e9
Data41e9                DB 080h, 040h, 02Ah, 0C0h, 02Ah, 000h, 06Ah
                        DB 02Ah, 040h, 0EAh, 000h, 02Ah, 015h, 000h, 0AAh, 080h, 040h, 02Ah, 0C0h, 02Ah, 000h, 06Ah, 02Ah
                        DB 040h, 0EAh, 000h, 02Ah, 015h, 000h, 0AAh, 095h, 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h
                        DB 0FFh, 015h, 07Fh, 015h, 0BFh, 095h, 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h, 0FFh, 015h
                        DB 07Fh, 015h, 0BFh, 080h, 040h, 02Ah, 0C0h, 02Ah, 000h, 06Ah, 02Ah, 040h, 0EAh, 000h, 02Ah, 015h
                        DB 000h, 0AAh, 080h, 040h, 02Ah, 0C0h, 02Ah, 000h, 06Ah, 02Ah, 040h, 0EAh, 000h, 02Ah, 015h, 000h
                        DB 0AAh, 095h, 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h, 0FFh, 015h, 07Fh, 015h, 0BFh, 095h
                        DB 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h, 0FFh, 015h, 07Fh, 015h, 0BFh
;Offset 0x425d
Data425d                DB 080h, 085h, 091h
                        DB 09Ch, 088h, 08Bh, 094h, 0A8h, 080h, 085h, 091h, 09Ch, 088h, 08Bh, 094h, 0A8h, 08Eh, 098h, 0ADh
                        DB 0B2h, 0A0h, 0A4h, 0B8h, 0BFh, 08Eh, 098h, 0ADh, 0B2h, 0A0h, 0A4h, 0B8h, 0BFh, 080h, 085h, 091h
                        DB 09Ch, 088h, 08Bh, 094h, 0A8h, 080h, 085h, 091h, 09Ch, 088h, 08Bh, 094h, 0A8h, 08Eh, 098h, 0ADh
                        DB 0B2h, 0A0h, 0A4h, 0B8h, 0BFh, 08Eh, 098h, 0ADh, 0B2h, 0A0h, 0A4h, 0B8h, 0BFh
;Offset 0x429d
Data429d                DB 080h, 040h, 02Ah
                        DB 0C0h, 02Ah, 000h, 06Ah, 02Ah, 040h, 0EAh, 000h, 06Ah, 000h, 0AAh, 040h, 015h, 040h, 03Fh, 000h
                        DB 02Ah, 015h, 000h, 02Ah, 03Fh, 02Ah, 000h, 015h, 02Ah, 000h, 03Fh, 06Ah, 015h, 06Ah, 03Fh, 0C0h
                        DB 015h, 000h, 015h, 02Ah, 0C0h, 03Fh, 000h, 03Fh, 02Ah, 02Ah, 015h, 000h, 0EAh, 015h, 02Ah, 03Fh
                        DB 000h, 0EAh, 03Fh, 000h, 055h, 000h, 015h, 03Fh, 000h, 03Fh, 015h, 000h, 07Fh, 02Ah, 055h, 02Ah
                        DB 015h, 03Fh, 02Ah, 03Fh, 015h, 02Ah, 07Fh, 015h, 040h, 015h, 000h, 02Ah, 015h, 02Ah, 000h, 015h
                        DB 06Ah, 03Fh, 040h, 03Fh, 000h, 02Ah, 03Fh, 02Ah, 000h, 03Fh, 06Ah, 0D5h, 000h, 015h, 000h, 03Fh
                        DB 0D5h, 02Ah, 015h, 02Ah, 03Fh, 03Fh, 000h, 015h, 0FFh, 000h, 03Fh, 02Ah, 015h, 0FFh, 02Ah, 055h
                        DB 000h, 055h, 02Ah, 015h, 03Fh, 000h, 015h, 03Fh, 02Ah, 03Fh, 015h, 000h, 03Fh, 015h, 02Ah, 07Fh
                        DB 000h, 07Fh, 02Ah, 095h, 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h, 0FFh, 015h, 07Fh, 015h
                        DB 0BFh
;Offset 0x4331
Data4331                DB 080h, 085h, 091h, 09Ch, 088h, 08Bh, 0A5h, 0A8h, 082h, 087h, 09Bh, 0A0h, 08Fh, 094h, 0A8h
                        DB 0ACh, 08Ch, 091h, 0A5h, 0AAh, 094h, 09Eh, 0B2h, 0B6h, 08Fh, 093h, 0A7h, 0ACh, 09Bh, 0A0h, 0B4h
                        DB 0B9h, 086h, 08Bh, 09Fh, 0A4h, 093h, 098h, 0ACh, 0B0h, 089h, 08Dh, 0A1h, 0A6h, 095h, 09Ah, 0AEh
                        DB 0B3h, 093h, 097h, 0ABh, 0B0h, 09Fh, 0A4h, 0B8h, 0BDh, 08Eh, 098h, 0ADh, 0B2h, 0A0h, 0A4h, 0B8h
                        DB 0BFh
;Offset 0x4371
Data4371                DB 080h, 080h, 080h, 080h, 080h, 080h, 080h, 080h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh
                        DB 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh
                        DB 0BFh, 080h, 080h, 080h, 080h, 080h, 080h, 080h, 080h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh
                        DB 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh, 0BFh
                        DB 0BFh
;Offset 0x43b1
Data43b1                DB 080h, 040h, 02Ah, 0C0h, 02Ah, 000h, 06Ah, 02Ah, 040h, 0EAh, 000h, 02Ah, 015h, 000h, 0AAh
                        DB 095h, 055h, 03Fh, 0D5h, 03Fh, 015h, 07Fh, 03Fh, 055h, 0FFh, 015h, 07Fh, 015h, 0BFh
;Offset 0x43ce
Data43ce                DB 080h, 085h
                        DB 091h, 09Ch, 088h, 08Bh, 094h, 0A8h, 08Eh, 098h, 0ADh, 0B2h, 0A0h, 0A4h, 0B8h, 0BFh
;Offset 0x43de
Data43de                DB 080h, 085h
                        DB 088h, 08Bh, 08Eh, 091h, 094h, 098h, 09Ch, 0A0h, 0A4h, 0A8h, 0ADh, 0B2h, 0B8h, 0BFh
;Offset 0x43ee - how long is this one?
Data43ee                DB 040h, 03Fh
                        DB 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 0FFh, 000h, 03Fh, 000h, 02Fh, 03Fh, 000h
                        DB 01Fh, 03Fh, 000h, 010h, 03Fh, 040h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 07Fh
                        DB 000h, 02Fh, 03Fh, 000h, 01Fh, 03Fh, 000h, 010h, 03Fh, 000h, 0C0h, 03Fh, 000h, 03Fh, 010h, 000h
                        DB 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 07Fh, 000h, 02Fh, 03Fh, 000h, 01Fh, 03Fh, 000h, 010h, 03Fh
                        DB 05Fh, 03Fh, 027h, 01Fh, 03Fh, 02Fh, 01Fh, 03Fh, 037h, 01Fh, 03Fh, 0FFh, 01Fh, 03Fh, 01Fh, 037h
                        DB 03Fh, 01Fh, 02Fh, 03Fh, 01Fh, 027h, 03Fh, 05Fh, 03Fh, 027h, 01Fh, 03Fh, 02Fh, 01Fh, 03Fh, 037h
                        DB 01Fh, 07Fh, 01Fh, 037h, 03Fh, 01Fh, 02Fh, 03Fh, 01Fh, 027h, 03Fh, 01Fh, 0DFh, 03Fh, 01Fh, 03Fh
                        DB 027h, 01Fh, 03Fh, 02Fh, 01Fh, 03Fh, 037h, 01Fh, 07Fh, 01Fh, 037h, 03Fh, 01Fh, 02Fh, 03Fh, 01Fh
                        DB 027h, 03Fh, 06Dh, 03Fh, 031h, 02Dh, 03Fh, 036h, 02Dh, 03Fh, 03Ah, 02Dh, 03Fh, 0FFh, 02Dh, 03Fh
                        DB 02Dh, 03Ah, 03Fh, 02Dh, 036h, 03Fh, 02Dh, 031h, 03Fh, 06Dh, 03Fh, 031h, 02Dh, 03Fh, 036h, 02Dh
                        DB 03Fh, 03Ah, 02Dh, 07Fh, 02Dh, 03Ah, 03Fh, 02Dh, 036h, 03Fh, 02Dh, 031h, 03Fh, 02Dh, 0EDh, 03Fh
                        DB 02Dh, 03Fh, 031h, 02Dh, 03Fh, 036h, 02Dh, 03Fh, 03Ah, 02Dh, 07Fh, 02Dh, 03Ah, 03Fh, 02Dh, 036h
                        DB 03Fh, 02Dh, 031h, 03Fh, 040h, 01Ch, 007h, 000h, 01Ch, 00Eh, 000h, 01Ch, 015h, 000h, 01Ch, 0DCh
                        DB 000h, 01Ch, 000h, 015h, 01Ch, 000h, 00Eh, 01Ch, 000h, 007h, 01Ch, 040h, 01Ch, 007h, 000h, 01Ch
                        DB 00Eh, 000h, 01Ch, 015h, 000h, 05Ch, 000h, 015h, 01Ch, 000h, 00Eh, 01Ch, 000h, 007h, 01Ch, 000h
                        DB 0C0h, 01Ch, 000h, 01Ch, 007h, 000h, 01Ch, 00Eh, 000h, 01Ch, 015h, 000h, 05Ch, 000h, 015h, 01Ch
                        DB 000h, 00Eh, 01Ch, 000h, 007h, 01Ch, 04Eh, 01Ch, 011h, 00Eh, 01Ch, 015h, 00Eh, 01Ch, 018h, 00Eh
                        DB 01Ch, 0DCh, 00Eh, 01Ch, 00Eh, 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h, 01Ch, 04Eh, 01Ch, 011h
                        DB 00Eh, 01Ch, 015h, 00Eh, 01Ch, 018h, 00Eh, 05Ch, 00Eh, 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h
                        DB 01Ch, 00Eh, 0CEh, 01Ch, 00Eh, 01Ch, 011h, 00Eh, 01Ch, 015h, 00Eh, 01Ch, 018h, 00Eh, 05Ch, 00Eh
                        DB 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h, 01Ch, 054h, 01Ch, 016h, 014h, 01Ch, 018h, 014h, 01Ch
                        DB 01Ah, 014h, 01Ch, 0DCh, 014h, 01Ch, 014h, 01Ah, 01Ch, 014h, 018h, 01Ch, 014h, 016h, 01Ch, 054h
                        DB 01Ch, 016h, 014h, 01Ch, 018h, 014h, 01Ch, 01Ah, 014h, 05Ch, 014h, 01Ah, 01Ch, 014h, 018h, 01Ch
                        DB 014h, 016h, 01Ch, 014h, 0D4h, 01Ch, 014h, 01Ch, 016h, 014h, 01Ch, 018h, 014h, 01Ch, 01Ah, 014h
                        DB 05Ch, 014h, 01Ah, 01Ch, 014h, 018h, 01Ch, 014h, 016h, 01Ch, 040h, 010h, 004h, 000h, 010h, 008h
                        DB 000h, 010h, 00Ch, 000h, 010h, 0D0h, 000h, 010h, 000h, 00Ch, 010h, 000h, 008h, 010h, 000h, 004h
                        DB 010h, 040h, 010h, 004h, 000h, 010h, 008h, 000h, 010h, 00Ch, 000h, 050h, 000h, 00Ch, 010h, 000h
                        DB 008h, 010h, 000h, 004h, 010h, 000h, 0C0h, 010h, 000h, 010h, 004h, 000h, 010h, 008h, 000h, 010h
                        DB 00Ch, 000h, 050h, 000h, 00Ch, 010h, 000h, 008h, 010h, 000h, 004h, 010h, 048h, 010h, 00Ah, 008h
                        DB 010h, 00Ch, 008h, 010h, 00Eh, 008h, 010h, 0D0h, 008h, 010h, 008h, 00Eh, 010h, 008h, 00Ch, 010h
                        DB 008h, 00Ah, 010h, 048h, 010h, 00Ah, 008h, 010h, 00Ch, 008h, 010h, 00Eh, 008h, 050h, 008h, 00Eh
                        DB 010h, 008h, 00Ch, 010h, 008h, 00Ah, 010h, 008h, 0C8h, 010h, 008h, 010h, 00Ah, 008h, 010h, 00Ch
                        DB 008h, 010h, 00Eh, 008h, 050h, 008h, 00Eh, 010h, 008h, 00Ch, 010h, 008h, 00Ah, 010h, 04Bh, 010h
                        DB 00Ch, 00Bh, 010h, 00Dh, 00Bh, 010h, 00Fh, 00Bh, 010h, 0D0h, 00Bh, 010h, 00Bh, 00Fh, 010h, 00Bh
                        DB 00Dh, 010h, 00Bh, 00Ch, 010h, 04Bh, 010h, 00Ch, 00Bh, 010h, 00Dh, 00Bh, 010h, 00Fh, 00Bh, 050h
                        DB 00Bh, 00Fh, 010h, 00Bh, 00Dh, 010h, 00Bh, 00Ch, 010h, 00Bh, 0CBh, 010h, 00Bh, 010h, 00Ch, 00Bh
                        DB 010h, 00Dh, 00Bh, 010h, 00Fh, 00Bh, 050h, 00Bh, 00Fh, 010h, 00Bh, 00Dh, 010h, 00Bh, 00Ch, 010h


;This function pokes an index value into an index register
;and then reads the result from the adjacent data register and puts it in ah.
;inputs:
;dx = index port
;al = index
;returns:
;al = index
;ah = value at index
;dx is unchanged
ReadDataWithIndexRegister PROC NEAR     ;Offset 0x4640
    out  dx, al
    inc  dx
    mov  ah, al
    in   al, dx
    dec  dx
    xchg al, ah
    ret
ReadDataWithIndexRegister ENDP

;This function pokes an index value into an index register
;and then reads the result from the adjacent data register and puts it in ah.
;It also increments the index value in al.
;inputs:
;dx = index port
;al = index
;returns:
;al = nex index
;ah = value at index
;dx is unchanged
ReadDataWithIndexRegisterNext PROC NEAR ;Offset 0x4649
    out  dx, al
    inc  dx
    inc  al
    mov  ah, al
    in   al, dx
    dec  dx
    xchg al, ah
    ret
ReadDataWithIndexRegisterNext ENDP

;inputs:
;al = passing 0x02 enables the speaker 0 = silent
;cx = wait count times 0x0550
Beep PROC NEAR                          ;Offset 0x4654
    mov  bl, al                         ;Save input in bl
    in   al, 61h                        ;Read Keyboard controller port B
    mov  bh, al                         ;Save original value in bh
    and  al, 0fch                       ;Mask out the speaker data status and timer 2 gate to speaker status
    or   al, bl                         ;Or in the input 
    or   al, 1                          ;Or in 1 to enable timer 2 gate to speaker
    out  61h, al                        ;Write out to Keyboard controller port B
    mov  al, 0b6h                       ;1011 0110b - 10 = Select counter 2, 11 = write counter bits 0-7 first, 001 = square wave generator,  0 = binary counter is 16 bits
    out  43h, al                        ;
    mov  al, 50h                        ;
    out  42h, al                        ;Write lower count byte to counter 2
    mov  al, 05h                        ;0550h wait time
    out  42h, al                        ;Write higher count byte to counter 2
    mov  dx, 0ffffh
CountDown:
    mov  al, 80h                        ;Counter 2 select
    out  43h, al
    in   al, 42h                        ;Read Counter lower byte
    mov  ah, al
    in   al, 42h                        ;Read Counter higher byte
    xchg al, ah
    cmp  ax, dx                         ;Check if time has run out
    mov  dx, ax
    jb   CountDown
    loop CountDown                      ;Loop cx times
    in   al, 61h                        ;Read Keyboard controller port B
    and  al, bh                         ;And out anything that wasn't set before
    out  61h, al                        ;Write Keyboard controller port B
    ret  
Beep ENDP

;input:
;al = number of beeps
PlayBeepPattern PROC NEAR               ;Offset 0x468c
    push ax
    push bx
    push cx
    push dx
    push ax
    mov  cx, 06cch                      ;Long beep
    mov  al, 03h                        ;Beeper on
    call Beep                           ;Offset 0x4654
    pop  ax
CountDown:                              ;offset 0x469a
    push ax
    mov  cx, 012ch                      ;Short pause
    mov  al, 00h                        ;Beeper off
    call Beep                           ;Offset 0x4654
    mov  cx, 0244h                      ;Medium beep
    mov  al, 02h                        ;Beeper on
    call Beep                           ;Offset 0x4654
    pop  ax
    dec  al
    jne  CountDown                      ;Offset 0x469a
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    ret  
PlayBeepPattern ENDP

Func0x46b5 PROC NEAR
    push ax
    push si
    push es
    call GetVideoModeOverrideTable      ;Offset 0x4a47
    call ApplyVideoParameters           ;Offset 0x4829
    pop  es
    pop  si
    pop  ax
    ret  
Func0x46b5 ENDP

;inputs:
;es:si pointer to character font
;dx = offset into ?
;cx = Number of characters
;bh = character height
;bl = character generator ram bank
SetTextFontAndAddressing PROC NEAR      ;Offset 0x46c2
    push      ax
    push      bx
    push      cx
    push      dx
    push      di
    push      si
    push      ds
    push      es
    call      PrepareAttributeController;Offset 0x47ae
    push      dx                        ;Store dx
    mov       dx, SequenceIndex         ;port - 0x3c4 - Sequence Index register
    mov       ax, 0402h                 ;SR2 - Enable Write Plane Register
    out       dx, ax                    ;bit 2 = 1 - Enable plane 3 (counting from 1)
    mov       ax, 0704h                 ;SR4 - Memory Mode Control register
    out       dx, ax                    ;bit 0 = 1 - Unknown, bit 1 = 1 - 256kb memory access, bit 2 = 1 - Sequential Addressing Mode
    mov       dx, GraphicsControllerIndex;port - 0x3ce - Graphics Controller Index register
    mov       ax, 0204h                 ;GR4 - Read Plane Select Register
    out       dx, ax                    ;bit 1 = 1 - Enable plane 2 (counting from 1) 
    mov       ax, 0005h                 ;GR5 - Graphics Controller Mode register
    out       dx, ax                    ;bit 1-0 = 00 - Write Mode 0. Each of four video memory planes is written with the CPU data
                                        ;               rotated by the number of counts in the rotate register. If the Set/Reset
                                        ;               register is enabled for any of four planes, the corresponding plane is written
                                        ;               with the data stored in the set/reset register. Raster operations and bit mask 
                                        ;               registers are effective
                                        ;bit 3 = 0 - The CPU reads data from the video memory planes. The plane is selected by the
                                        ;            Read Plane Select register. This is called read mode 0
                                        ;bit 4 = 0 - Standard addressing.
                                        ;bit 5 = 0 - Normal shift mode
                                        ;bit 6 = 0 - Bit 5 in this register controls operation ofthe video shift registers
                                        ;
    mov       ax, 0406h                 ;GR6 - Memory Map Mode Control register
    out       dx, ax                    ;bit 0 = 0 - Text mode display addressing selected
                                        ;bit 1 = 0 - AO address bit unchanged
                                        ;bit 3-2 = 01 - AOOOOH to AFFFFH (64 KBytes)
    pop       dx                        ;Restore dx
    mov       ax, es
    mov       ds, ax
    mov       ax, 0a000h                ;Video memory
    mov       es, ax
    push      bx                        ;store bh = character height, bl = character generator ram bank
    and       bl, 07h                   ;0x00           ;0x01           ;0x02           ;0x03           ;0x04           ;0x05           ;0x06           ;0x07
    ror       bl, 01h                   ;0x00 - C = 0   ;0x80 - C = 1   ;0x01 - C = 0   ;0x81 - C = 1   ;0x02 - C = 0   ;0x82 - C = 1   ;0x03 - C = 0   ;0x83 - C = 1
    ror       bl, 01h                   ;0x00 - C = 0   ;0x40 - C = 0   ;0x80 - C = 1   ;0xC0 - C = 1   ;0x01 - C = 0   ;0x41 - C = 0   ;0x81 - C = 1   ;0xC1 - C = 1
    rcr       bl, 01h                   ;0x00 - C = 0   ;0x20 - C = 0   ;0x40 - C = 0   ;0x60 - C = 0   ;0x00 - C = 1   ;0x20 - C = 1   ;0xD0 - C = 1   ;0xE0 - C = 1
    jae       NoCarry                   ;Offset 0x4701 - same as jnc = jump no carry
    add       bl, 10h                   ;0x10           ;0x30           ;0x50           ;0x70           ;               ;               ;               ;
NoCarry:                                ;Offset 0x4701
    shl       bl, 01h                   ;0x20           ;0x60           ;0xA0           ;0xE0           ;0x00           ;0x40           ;0x80           ;0xC0
    mov       ah, bl
    mov       al, 00h
    mov       di, ax                    ;di = 0x0000(4), 0x2000(0), 0x4000(5), 0x6000(1), 0x8000(6), 0xA000(2), 0xC000(7), 0xE000(3)
    or        dx, dx
    je        NoOffset                  ;Offset 0x4714
    mov       ax, 20h
    mul       dx
    add       di, ax
NoOffset:                               ;Offset 0x4714
    mov       dx, 20h
    sub       dl, bh                    ;32 - character height
    jcxz      CopyDone                  ;Offset 0x472c - if no characters, bail
    call      CopyRect                  ;Offset 0xea9
    jne       CopyDone                  ;Offset 0x472c
Label0x4720:                            ;Offset 0x4720
    push      cx
    mov       cl, bh
    mov       ch, 0
    rep movsb
    add       di, dx
    pop       cx
    loop      Label0x4720               ;Offset 0x4720
CopyDone:                               ;Offset 0x472c
    pop       bx                        ;restore bh = character height, bl = character generator ram bank
    pop       es                        ;
    pop       ds                        ;
    call      PatchCharacterSet         ;Offset 0x4760
    mov       dx, SequenceIndex         ;port - 0x3c4
    mov       ax, 0302h                 ;SR2 - Enable Write Plane register
                                        ;bits 3-0 = 0011 - Enable writing to plane 0 and 1
    out       dx, ax
    mov       ax, 0304h                 ;SR4 - Memory Mode Control register
                                        ;bit 0 = 1 - Unknown
                                        ;bit 1 = 1 - Allows complete memory access to 256KiB
                                        ;bit 2 = 0 - Sequential Addressing Mode.
                                        ;            Enables the odd/even addressing mode.
                                        ;            Even addresses access planes 0 and 2.
                                        ;            Odd addresses access planes 1 and 3.
    out       dx, ax
    mov       dx, GraphicsControllerIndex;port - 0x3ce
    mov       ax, 0004h                 ;GR4 - Read Plane Select Register
                                        ;bits 1-0 = 00 - Plane 0
    out       dx, ax
    mov       ax, 1005h                 ;GR5 - Graphics Controller Mode register
                                        ;bit 4 = 1 - Odd/Even addressing mode. Even CPU addresses access plane 0 and 2,
                                        ;            while odd CPU addresses access plane 1 and 3. This option is useful
                                        ;            for emulating the CGA compatible mode. The value of this bit should be
                                        ;            the inverted value programmed in bit 2 of the Sequencer Memory Mode register
                                        ;            SR4. This bit affects reading of display memory by the CPU.
    out       dx, ax
    mov       ax, 0a06h                 ;GR6 - Memory Map Mode Control register
                                        ;bit 0 = 0 - Text mode display addressing selected
                                        ;bit 1 = 1 - Chain odd / even planes
                                        ;bits 3-2 = 10 - B0000 - B7FFF (32KiB)
    cmp       word ptr ds:[BDA_VideoBaseIOPort], 03b4h;Offset 0463h
    je        Label0x4755               ;Offset 0x4755
    mov       ah, 0eh                   ;GR6 - Memory Map Mode Control register
                                        ;bit 0 = 0 - Text mode display addressing selected
                                        ;bit 1 = 1 - Chain odd / even planes
                                        ;bits 3-2 = 11 - B8000 - BFFFF (32KiB)
Label0x4755:                            ;Offset 0x4755
    out       dx, ax
    call      EnablePaletteBasedVideo   ;Offset 0x479d
    pop       si
    pop       di
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
SetTextFontAndAddressing ENDP

;inputs:
;bl = flags 0x80 = 8 lines, 0x40 = 7 lines
;
PatchCharacterSet PROC NEAR             ;Offset 0x4760
    push      ax
    push      cx
    push      di
    push      si
    push      es
    mov       cx, 0a000h
    mov       es, cx                    ;es points to video segment
    test      bl, 0c0h                  ;bl & 0c0h == 0 -> exit
    je        Exit                      ;Offset 0x4797
    mov       cx, 0007h                 ;7 * 2 pixels
    mov       si, offset Patch8x14      ;Offset 0x6d20
    test      bl, 80h                   ;bit 7 set -> Start copy of 7 lines
    jne       NextCharacter             ;Offset 0x477f
    mov       si, offset Patch8x16      ;Offset 0x7e30
    mov       cl, 08h                   ;8 * 2 pixels
NextCharacter:                          ;Offset 0x477f
    mov       ah, byte ptr cs:[si]      ;read byte
    inc       si                        ;point to next one
    or        ah, ah                    ;is ah zero? -> Exit
    je        Exit                      ;Offset 0x4797
    xor       al, al                    ;empty al
    shr       ax, 03h                   ;ax = ax / 8
    mov       di, ax                    ;new destination
    push      cx                        ;store count
    cli                                 ;clear interrupts
    rep movsw es:[di], cs:[si]          ;move cx words
    sti                                 ;restore interrupts
    pop       cx                        ;restore count
    jmp       NextCharacter             ;Offset 0x477f
Exit:                                   ;Offset 0x4797
    pop       es
    pop       si
    pop       di
    pop       cx
    pop       ax
    ret       
PatchCharacterSet ENDP

EnablePaletteBasedVideo PROC NEAR       ;Offset 0x479d
    push      dx                        ;preserve dx
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0463h = port - 0x3?4
    add       dl, 06h                   ;port - 0x3?A - Input Status 1
    in        al, dx                    ;
    mov       dx, 03c0h                 ;port 0x3c0 - Attribute Controller Index
    mov       al, 20h                   ;Enable Video Display = 1, Display video using the palette registers enabled (normal display operation).
    out       dx, al
    pop       dx                        ;restore dx
    ret       
EnablePaletteBasedVideo ENDP

PrepareAttributeController PROC NEAR    ;Offset 0x47ae
    push  dx                            ;store dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0463h = port - 0x3?4
    add   dx, 06h                       ;port - 0x3?a - Input Status 1
    in    al, dx                        ;pull Input Status 1
    xor   al, al                        ;Throw it away
    mov   dx, AttributeControllerIndex  ;port - 0x3c0 - Attribute Controller Index
    out   dx, al                        ;Set index 00h
    pop   dx                            ;restore dx
    ret
PrepareAttributeController ENDP


;inputs:
;bl = color index
;outputs:
;ah = red
;ch = green
;cl = blue
;bx = bx + 1
;destroys dx
GetPaletteColor PROC NEAR               ;Offset 0x47bf
    pushf
    cli
GetPaletteColor ENDP
;continues!
GetPaletteColorInternal PROC NEAR       ;Offset 0x47c1
    mov   dx, DACWriteIndex             ;port - 03c8h - DACWriteIndex
    in    al, dx
    dec   dx                            ;port - 03c7h - DACReadIndex
    mov   al, bl
    out   dx, al                        ;write color index
    add   dx, 0002h                     ;port - 03c9h - RAMDACData
    in    al, dx                        ;read red
    mov   ah, al
    in    al, dx                        ;read green
    mov   ch, al
    in    al, dx                        ;read blue
    mov   cl, al
    inc   bx
    popf
    ret
GetPaletteColorInternal ENDP

;inputs:
;bl = color index
;ah = red
;ch = green
;cl = blue
;outputs:
;bx = bx + 1
;destroys dx
SetPaletteColor PROC NEAR               ;Offset 0x47d8
    pushf
    cli
SetPaletteColor ENDP
;continues!
SetPaletteColorInternal PROC NEAR       ;Offset 0x47da
    mov   dx, DACWriteIndex             ;port - 03c8h
    mov   al, bl
    out   dx, al                        ;Write color index
    inc   dx                            ;port - 03c9h - RAMDACData
    mov   al, ah
    out   dx, al                        ;Write red
    mov   al, ch
    out   dx, al                        ;Write green
    mov   al, cl
    out   dx, al                        ;Write blue
    inc   bx
    popf
    ret
SetPaletteColorInternal ENDP

TurnOffScreen PROC NEAR                 ;Offset 0x47ed
    push  dx
    mov   dx, SequenceIndex             ;port - 03c4h
    in    al, dx                        ;Read current index number
    push  ax                            ;Store it
    mov   al, 01h                       ;SR1 - Clocking Mode register
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    or    ah, 20h                       ;Turn off screen
    out   dx, ax                        ;Write
    pop   ax                            ;Load back original value of Index
    out   dx, al                        ;Write back original index
    pop   dx
    ret   
TurnOffScreen ENDP

TurnOnScreen PROC NEAR                  ;Offset 0x4800
    push  dx
    mov   dx, SequenceIndex             ;port - 03c4h
    in    al, dx                        ;Read current index number
    push  ax                            ;Store it
    mov   al, 01h                       ;SR1 - Clocking Mode register
    call  ReadDataWithIndexRegister     ;Offset 0x4640
    and   ah, 0dfh                      ;Turn on screen
    out   dx, ax                        ;Write
    pop   ax                            ;Load back original value of index
    out   dx, al                        ;Write back original index
    pop   dx
    ret
TurnOnScreen ENDP

ClearPalette PROC NEAR                  ;Offset 0x4813
    call  TurnOffScreen                 ;Offset 0x47ed
    mov   si, 0100h                     ;256 colors
    xor   bx, bx                        ;zero out index
    xor   ah, ah                        ;zero out red
    xor   cx, cx                        ;zero out green and blue
ClearColor:                             ;Offset 0x481f
    call  SetPaletteColor               ;Offset 0x47d8
    dec   si                            ;Count down
    jne   ClearColor                    ;Offset 0x481f
    call  TurnOnScreen                  ;Offset 0x4800
    ret
ClearPalette ENDP

;inputs:
;es:si video mode table pointer
ApplyVideoParameters PROC NEAR          ;Offset 0x4829
    push  cx
    push  dx
    mov   cx, 0019h
    mov   ax, 0000h
    push  cx
    push  ax
    call  PrepareAttributeController    ;Offset 0x47ae
    mov   cx, 0005h
    add   si, cx                        ;point to byte 5 in video mode table entry - SequencerRegisters
    mov   ax, 0100h                     ;SR0 - Asynchronous reset (should serve no function on Trio32/Trio64)
    mov   dx, SequenceIndex             ;port - 0x3c4
    cli
    out   dx, ax
    inc   al                            ;SR1 - Clock Mode Register
    mov   ah, 20h                       ;bit 5 = 1 - Turn screen off
    out   dx, ax
    or    ah, byte ptr es:[si]          ;Write Clock mode register again.
    out   dx, ax
    mov   al, byte ptr es:[si + 04h]    ;Load MiscOutputRegisterValues
    mov   dx, MiscellaneousWrite        ;port - 0x3c2
    out   dx, al
    mov   ax, 0300h                     ;CR0 - Asynchronous and Synchronous reset (should server no function on Trio32/Trio64)
    mov   dx, SequenceIndex             ;port - 0x3c4
    out   dx, ax
    sti
    mov   cx, 03h                       ;loop remaining 3 bytes
    mov   al, 02h                       ;SR2 - Enable Write Plane register
    inc   si
WriteSequenceRegisters:                 ;Offset 0x4862
    mov   ah, byte ptr es:[si]          ;
    out   dx, ax
    inc   al
    inc   si
    loop  WriteSequenceRegisters        ;Offset 0x4862
                                        ;si no points at MiscOutputRegisterValues
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0463h = port 0x3?4
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_Unknown1;Offset 0449h, 0x8
    jb    Label0x487d                   ;Offset 0x487d
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_Reserved2;Offset 0449h, 0xc
    jbe   IsMonochrome                  ;Offset 0x488a
Label0x487d:                            ;Offset 0x487d
    mov   dx, CRTControllerIndexB       ;port - 0x3b4
    mov   al, byte ptr es:[si]
    test  al, 01h                       ;bit 0 - 0 = Monochrome emulation, 1 = Color emulation
    je    IsMonochrome                  ;Offset 0x488a
    mov   dx, CRTControllerIndexD       ;port - 0x3d4
IsMonochrome:                           ;Offset 0x488a
    mov   word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0463h Store CRTControllerIndex in BDA
    inc   si                            ;Point to CRTCRegisters
    mov   ax, 11h                       ;CR11 - Vertical Retrace End register
    out   dx, ax                        ;Clear to 0
                                        ;bits 3-0 = 000 - Vertical retrace end set to 0 scanlines
                                        ;bit 4 = 0 - Clear vertical retrace interrupt
                                        ;bit 5 = 0 - Enable vertical retrace interrupt
                                        ;bit 6 = 0 - 3 dram refresh cycles generated per horizontal line
                                        ;bit 7 = 0 - Writing to all CRT controller registers enabled
    pop   ax                            ;0x0000
    pop   cx                            ;0x0019 - 25
    add   si, ax
WriteCRTCRegisters:                     ;Offset 0x4897
    mov   ah, byte ptr es:[si]          ;Write CR0-CR18
    inc   si
    out   dx, ax
    inc   al
    loop  WriteCRTCRegisters            ;Offset 0x4897
    add   dx, 06h                       ;port - 0x3?a - InputStatus1
    in    al, dx                        ;Reset Attribute Controller port 0x3c0 to point to index register
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_Unknown2;Offset 0449h, 0x9
    je    Label0x48b2                   ;Offset 0x48b2
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_Reserved1;Offset 0449h, 0xb
    jne   Label0x48b6                   ;Offset 0x48b6
Label0x48b2:                            ;Offset 0x48b2
    sub   dl, 20h                       ;port - 0x3ba - InputStatus1
    in    al, dx                        ;Reset Attribute Controller port 0x3c0 to point to index register
Label0x48b6:                            ;Offset 0x48b6
    xor   ah, ah                        ;0
    mov   cx, 0010h                     ;16
    mov   dx, AttributeControllerIndex  ;port - 0x3c0
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DefaultPaletteDisabled;Offset 0489h, 0x8
    jne   DefaultPaletteDisabled        ;Offset 0x48cf
WritePalette:                           ;Offset 0x48c5
    mov   al, ah                        ;
    out   dx, al                        ;Index
    inc   ah                            ;counts up to 0x11?
    lodsb es:[si]
    out   dx, al
    loop  WritePalette                  ;Offset 0x48c5
DefaultPaletteDisabled:                 ;Offset 0x48cf
    add   ah, cl                        ;Skip over palette data, ah = 0x10
    add   si, cx                        ;Skip over palette data
    mov   cx, 0005h                     ;
WriteAttributeControllerRegs:           ;Offset 0x48d6
    cmp   ah, 11h                       ;
    jne   Skip17                        ;Offset 0x48e4
    inc   si
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DefaultPaletteDisabled;Offset 0489h, 0x8
    jne   DefaultPaletteDisabled2       ;Offset 0x48f1
    dec   si
Skip17:                                 ;Offset 0x48e4
    mov   al, ah
    out   dx, al                        ;write 0x10 or 0x11
    xor   al, al                        ;0
    cmp   ah, 14h                       ;Don't load 20th byte
    je    Skip20                        ;Offset 0x48f0
    lodsb es:[si]
Skip20:                                 ;Offset 0x48f0
    out   dx, al                        ;Write out 0 or the loaded value
DefaultPaletteDisabled2:                ;Offset 0x48f1
    inc   ah
    loop  WriteAttributeControllerRegs  ;Offset 0x48d6 loop 5 bytes
    xor   al, al                        ;0
    mov   cx, 0009h                     ;count 9, si points to GraphicsControllerRegs
    mov   dx, GraphicsControllerIndex   ;port - 0x3ce
WriteGraphicsControllerRegs:            ;Offset 0x48fd
    mov   ah, byte ptr es:[si]
    inc   si
    out   dx, ax
    inc   al
    loop  WriteGraphicsControllerRegs   ;Offset 0x48fd write out GR0-GR8
    pop   dx
    pop   cx
    ret
ApplyVideoParameters ENDP


;
Func0x4909 PROC NEAR                    ;Offset 0x4909
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 08h;Offset 0489h
    je    Label0x4911                   ;Offset 0x4911
    ret
Label0x4911:                            ;Offset 0x4911
    mov   dx, 03c6h
    in    al, dx
    inc   al
    je    Label0x491c                   ;Offset 0x491c
    mov   al, 0ffh
    out   dx, al
Label0x491c:                            ;Offset 0x491c
    mov   di, 0040h
    xor   bx, bx
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0449h
    mov   si, 4371h                     ;Offset 0x4371
    cmp   al, 07h
    je    Func0x498e                    ;Offset 0x498e
    cmp   al, 0fh
    je    Func0x498e                    ;Offset 0x498e
    cmp   al, 13h
    je    Func0x4998                    ;Offset 0x4998
    jb    Label0x4948                   ;Offset 0x4948
    mov   ah, al
    call  GetVideoModeFlags             ;Offset 0x105d
    xchg  ah, al
    test  ah, 02h
    je    Func0x498e                    ;Offset 0x498e
    test  ah, 04h
    je    Label0x4981                   ;Offset 0x4981
    jmp   Func0x4998                    ;Offset 0x4998
Label0x4948:                            ;Offset 0x4948
    cmp   al, 04h
    jb    Label0x495a                   ;Offset 0x495a
    cmp   al, 06h
    jbe   Label0x4972                   ;Offset 0x4972
    cmp   al, 08h
    je    Label0x4981                   ;Offset 0x4981
    cmp   al, 0eh
    jbe   Label0x4972                   ;Offset 0x4972
    jmp   Label0x4981                   ;Offset 0x4981
Label0x495a:                            ;Offset 0x495a
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 10h;Offset 0489h
    jne   Label0x4981                   ;Offset 0x4981
    mov   ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0448h
    and   ah, 0fh
    cmp   ah, 03h
    je    Label0x4981                   ;Offset 0x4981
    cmp   ah, 09h
    je    Label0x4981                   ;Offset 0x4981
Label0x4972:                            ;Offset 0x4972
    mov   si, 41e9h                     ;Offset 0x41e9
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 06h;Offset 0489h
    je    Func0x498e                    ;Offset 0x498e
    mov   si, 425dh                     ;Offset 0x425d
    jmp   Func0x498e                    ;Offset 0x498e
Label0x4981:                            ;Offset 0x4981
    mov   si, 429dh                     ;Offset 0x429d
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 06h;Offset 0489h
    je    Func0x498e                    ;Offset 0x498e
    mov   si, 4331h                     ;Offset 0x4331
Func0x4909 ENDP
;continue!
Func0x498e PROC NEAR                    ;Offset 0x498e
    call  Func0x49d4                    ;Offset 0x49d4
    call  SetPaletteColor               ;Offset 0x47d8
    dec   di
    jne   Func0x498e                    ;Offset 0x498e
    ret
Func0x498e ENDP

Func0x4998 PROC NEAR                    ;Offset 0x4998
    mov   di, 0010h
    mov   si, 43b1h                     ;Offset 0x43b1
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 06h;Offset 0489h
    je    Label0x49a8                   ;Offset 0x49a8
    mov   si, 43ceh                     ;Offset 0x43ce
Label0x49a8:                            ;Offset 0x49a8
    call  Func0x498e                    ;Offset 0x498e
    mov   di, 0010h
    mov   bx, 0010h
    mov   si, 43deh                     ;Offset 0x43de
    call  Func0x498e                    ;Offset 0x498e
    mov   di, 00d8h
    mov   bx, 0020h
    mov   si, 43eeh                     ;Offset 0x43ee
    test  byte ptr ds:[BDA_VideoDisplayDataArea], 06h;Offset 0489h
    je    Func0x498e                    ;Offset 0x498e
Label0x49c7:                            ;Offset 0x49c7
    call  Func0x49d4                    ;Offset 0x49d4
    call  Func0x2eed                    ;Offset 0x2eed
    call  SetPaletteColor               ;Offset 0x47d8
    dec   di
    jne   Label0x49c7                   ;Offset 0x49c7
    ret
Func0x4998 ENDP

Func0x49d4 PROC NEAR                    ;Offset 0x49d4
    lodsb cs:[si]
    mov   ah, al
    and   ah, 03fh
    and   al, 0c0h
    je    Label0x49f3                   ;Offset 0x49f3
    cmp   al, 40h
    je    Label0x4a01                   ;Offset 0x4a01
    cmp   al, 80h
    je    Label0x49ee                   ;Offset 0x49ee
    mov   cl, ah
    lodsb cs:[si]
    mov   ch, al
    ret
Label0x49ee:                            ;Offset 0x49ee
    mov   ch, ah
    mov   cl, ah
    ret
Label0x49f3:                            ;Offset 0x49f3
    lodsb cs:[si]
    test  al, 40h
    mov   ch, al
    je    Label0x4a03                   ;Offset 0x4a03
    and   ch, 0bfh
    mov   cl, ch
    ret
Label0x4a01:                            ;Offset 0x4a01
    mov   ch, ah
Label0x4a03:                            ;Offset 0x4a03
    lodsb cs:[si]
    mov   cl, al
    ret
Func0x49d4 ENDP

VideoModeOverrideTranslationTable1 DB 17h, 17h, 18h, 18h, 04h, 05h, 06h, 19h, 08h, 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh, 11h, 12h, 1Ah, 1Bh, 1Ch
VideoModeOverrideTranslationTable2 DB 13h, 14h, 15h, 16h, 04h, 05h, 06h, 07h, 08h, 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh, 11h, 12h, 1Ah, 1Bh, 1Ch
VideoModeOverrideTranslationTable3 DB 00h, 01h, 02h, 03h, 04h, 05h, 06h, 07h, 08h, 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh, 11h, 12h, 1Ah, 1Bh, 1Ch

GetCurrentVideoModeOverrideTable PROC NEAR;Offset 0x4a44
    mov al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
GetCurrentVideoModeOverrideTable ENDP
;continue!
;inputs:
;al = mode
;outputs:
;es:si into video override table based on mode passed in in al
GetVideoModeOverrideTable PROC NEAR     ;Offset 0x4a47
    push ax
    push bx
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    jbe  NotVESA                        ;Offset 0x4a62 if less than or equal, regular video mode
    xor  si, si
    mov  es, si                         ;
    call GetVESAVideoModeOverrideTable  ;Offset 0x106c
    jne  NotVESA                        ;Offset 0x4a62 ; not found, this should never happen...
    mov  ax, es
    or   ax, ax
    jne  Exit                           ;Offset 0x4a9b
    mov  ax, cs
    mov  es, ax
    jmp  Exit                           ;Offset 0x4a9b
NotVESA:                                ;Offset 0x4a62
    mov  si, offset VideoModeOverrideTranslationTable1;Offset 0x4a08
    test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
    jne  Done                           ;Offset 0x4a87
    mov  si, offset VideoModeOverrideTranslationTable2;Offset 0x4a1c
    mov  ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and  ah, BDA_EFBS_AdapterTypeMask   ;0xf
    cmp  ah, BDA_EFBS_MDAHiResEnhanced  ;0x3
    je   Done                           ;Offset 0x4a87
    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
    je   Done                           ;Offset 0x4a87
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x7
    je   Done                           ;Offset 0x4a87
    mov  si, offset VideoModeOverrideTranslationTable3;Offset 0x4a30
Done:                                   ;Offset 0x4a87
    mov  bl, al                         ;bl is video mode
    xor  bh, bh                         ;
    mov  al, byte ptr cs:[bx + si]      ;Look up the mode in one of three tables above
    mov  ah, 40h                        ;al
    mul  ah                             ;multiply by 64
    mov  si, ax                         ;Index of 64 x Mode
    xor  bx, bx                         ;Video Parameter Table pointer
    call GetVideoParameterBlockElement  ;Offset 0x1d95
    add  si, bx                         ;es:si is segment:offset into video override table based on mode
Exit:                                   ;Offset 0x4a9b
    pop  bx
    pop  ax
    ret
GetVideoModeOverrideTable ENDP

PrintBanner PROC NEAR                   ;Offset 0x4a9e
    xor    cl, cl                       ;Clear cl
    mov    bl, byte ptr cs:[Data0043]   ;Load bits to see which strings to print
    test   bl, 01h                      ;Test if bit 0 is cleared
    je     DontPrint1                   ;Offset 0x4ab0
    mov    si, offset CopyrightString1  ;Offset 0x0044
    call   PrintString
DontPrint1:
    test   bl, 02h                      ;Test if bit 1 is cleared
    je     DontPrint2                   ;Offset 0x4abb
    mov    si, offset CopyrightString2  ;Offset 00e4h
    call   PrintString
DontPrint2:                             ;Offset 0x4abb
    test   bl, 07h
    je     Exit                         ;Offset 0x4ad9
    test   bl, 08h
    jne    Exit                         ;Offset 0x4ad9
    mov    cx, 1800h                    ;Long pause
    mov    al, 00h                      ;Mute beep
    call   Beep                         ;Offset 0x4654
    call   Func0x1d47                   ;Offset 0x1d47
    mov    bh, 00h                      ;Page number 0
    mov    dx, 0000h                    ;Cursor position dl = 0, dh = 0
    mov    ah, 02h                      ;Set Cursor Position
    int    10h                          ;Video BIOS interrupt
Exit:                                   ;Offset 0x4ad9
    ret
PrintBanner ENDP

PrintString PROC NEAR                   ;Offset 0x4ada
    xor    bh, bh                       ;Set Page number to 0
NextCharacter:                          ;Offset 0x4adc
    mov    ah, 0eh                      ;Teletype output
    lodsb  cs:[si]                      ;Load character
    shr    al, cl                       ;Shift right by cl bits
    test   al, al                       ;Check if al equals zero
    je     Exit                         ;Offset 0x4aea
    int    10h                          ;Video BIOS interrupt
    jmp    NextCharacter                ;Offset 0x4adc
Exit:                                   ;Offset 0x4aea
    ret
PrintString ENDP


VESAHandlerCallTable    DW offset GetSuperVGAInfo;Offset 0x5637;00h;GET SuperVGA INFORMATION
                        DW offset GetSuperVGAModeInfo;Offset 0x4b37;01h;GET SuperVGA MODE INFORMATION
                        DW offset SetSuperVGAMode;Offset 0x4bfa;02h;SET SuperVGA VIDEO MODE
                        DW offset GetCurrentSuperVGAMode;Offset 0x4c60;03h;GET CURRENT VIDEO MODE
                        DW offset SaveRestoreSuperVGAVideoState;Offset 0x4c9c;04h;SAVE/RESTORE SuperVGA VIDEO STATE
                        DW offset CPUVideoMemoryControl;Offset 0x4cb3;05h;CPU VIDEO MEMORY CONTROL
                        DW offset GetSetLogicalScanLineLength;Offset 0x4cd1;06h;GET/SET LOGICAL SCAN LINE LENGTH (PIXELS)
                        DW offset GetSetDisplayStart;Offset 0x4cf6;07h;GET/SET DISPLAY START
                        DW offset GetSetDACPaletteControl;Offset 0x559b;08h;GET/SET DAC PALETTE CONTROL
                        DW offset VESAPowerManagement;Offset 0x55b8;10h;VESA VBE/PM (Power Management)
                        DW offset VESADisplayDataChannel;Offset 0x55ff;15h;VESA VBE/DC (Display Data Channel)

VESAHandler PROC NEAR                   ;Offset 0x4b01
    mov  ds, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  si, ax
    xor  ah, ah
    cmp  al, 15h
    je   Label0x4b18                    ;Offset 0x4b18
    cmp  al, 10h
    je   Label0x4b1d                    ;Offset 0x4b1d
    cmp  al, 08h
    ja   Label0x4b31                    ;Offset 0x4b31
    jmp  Label0x4b1f                    ;Offset 0x4b1f
Label0x4b18:                            ;Offset 0x4b18
    mov  al, 0ah
    jmp  Label0x4b1f                    ;Offset 0x4b1f
    nop
Label0x4b1d:                            ;Offset 0x4b1d
    mov  al, 09h
Label0x4b1f:                            ;Offset 0x4b1f
    mov  byte ptr ss:[bp + 10h], 4fh
    push ax
    call UnlockExtendedCRTRegisters     ;Offset 0xfa6
    pop  ax
    shl  ax, 01h
    xchg ax, si
    call word ptr cs:[si + offset VESAHandlerCallTable];Offset 0x4aeb
Label0x4b30:                            ;Offset 0x4b30
    ret
Label0x4b31:                            ;Offset 0x4b31
    mov       byte ptr [bp + 10h], 00h
    jmp       Label0x4b30               ;Offset 0x4b30
VESAHandler ENDP

;Info from: http://www.ctyme.com/intr/rb-0274.htm
;
;inputs:
;cx = 
;Bit(s)  Description     (Table 04082)
;15     preserve display memory on mode change
;14     (VBE v2.0+) use linear (flat) frame buffer
;13     (VBE/AF 1.0P) VBE/AF initializes accelerator hardware
;12     reserved for VBE/AF
;11     (VBE v3.0) user user-specified CRTC refresh rate values
;10-9   reserved for future expansion
;8-0    video mode number (0xxh are non-VESA modes, 1xxh are VESA-defined)
;
;es:di points to 256 byte  buffer
;
;Offset  Size    Description     (Table 00079)
;00h    WORD    mode attributes (see #00080)
;02h    BYTE    window attributes, window A (see #00081)
;03h    BYTE    window attributes, window B (see #00081)
;04h    WORD    window granularity in KB
;06h    WORD    window size in KB
;08h    WORD    start segment of window A (0000h if not supported)
;0Ah    WORD    start segment of window B (0000h if not supported)
;0Ch    DWORD   -> FAR window positioning function (equivalent to AX=4F05h)
;10h    WORD    bytes per scan line
;---remainder is optional for VESA modes in v1.0/1.1, needed for OEM modes---
;12h    WORD    width in pixels (graphics) or characters (text)
;14h    WORD    height in pixels (graphics) or characters (text)
;16h    BYTE    width of character cell in pixels
;17h    BYTE    height of character cell in pixels
;18h    BYTE    number of memory planes
;19h    BYTE    number of bits per pixel
;1Ah    BYTE    number of banks
;1Bh    BYTE    memory model type (see #00082)
;1Ch    BYTE    size of bank in KB
;1Dh    BYTE    number of image pages (less one) that will fit in video RAM
;1Eh    BYTE    reserved (00h for VBE 1.0-2.0, 01h for VBE 3.0)
;---VBE v1.2+ ---
;1Fh    BYTE    red mask size
;20h    BYTE    red field position
;21h    BYTE    green mask size
;22h    BYTE    green field size
;23h    BYTE    blue mask size
;24h    BYTE    blue field size
;25h    BYTE    reserved mask size
;26h    BYTE    reserved mask position
;27h    BYTE    direct color mode info
;
;outputs:
;AL = 4Fh if function supported
;AH = status, 00h successful and ES:DI buffer filled. AH = 01h failed

GetSuperVGAModeInfo PROC NEAR           ;Offset 0x4b37
    mov       bx, cx
    mov       byte ptr ss:[bp + 11h], 00h;ah status = success
    xor       ax, ax
    mov       si, di
    mov       cx, 80h                   ;0x80 = 128
    rep stosw                           ;clear 256 bytes
    mov       di, si
    push      cs
    pop       ds
    test      bx, 7f00h                 ;Check if VESA mode
    jne       NotLegacy                 ;Offset 0x4b55
Failed:                                 ;Offset 0x4b50
    mov       byte ptr ss:[bp + 11h], 01h;ah status = failed
    ret
NotLegacy:                              ;Offset 0x4b55
    mov       dx, di                    ;preserve output offset
    xor       di, di
    and       bx, 7fffh                 ;And off preserve display memory on mode change
    lea       si, ds:[VESAVideoModeIds] ;Offset 0x4d28
    call      VESAFindModeData          ;Offset 0x4c4a
    jne       Found                     ;Offset 0x4b71
    add       si, 02h
    add       di, 03h
    call      VESAFindModeData          ;Offset 0x4c4a
    je        Failed                    ;Offset 0x4b50
Found:                                  ;Offset 0x4b71
    lea       si, ds:[VESAModeData]     ;Offset 0x4d82
    add       si, di                    ;point si to mode data
    mov       bl, byte ptr ds:[si]
    xor       bh, bh
    mov       di, dx                    ;restore output offset
    push      di                        ;preserve output offset
    mov       si, word ptr ds:[si + 01h];
    movsb                               ;0x00 = mode attributes
    inc       di                        ;0x01 = 0x00 (no higher mode attributes)
    push      si
    lea       si, ds:[VESAWindowAttributes];Offset 0x4de0
    movsw                               ;0x02 = Window attributes (Window A and Window B)
    mov       al, bl
    call      GetVideoModeFlags         ;Offset 0x105d
    test      al, 01h
    je        WindowSize64k             ;Offset 0x4b9c
    mov       ax, 0020h                 ;Window granularity and size in KB 0x20 -> 32k
    stosw                               ;0x04 = Window Granularity in KB
    stosw                               ;0x06 = Window Size in KB
    mov       ax, 0b800h                ;Segment 0xb800
    jmp       WindowSize32k             ;Offset 0x4ba4
WindowSize64k:                          ;Offset 0x4b9c
    mov       ax, 0040h                 ;Window granularity and size in KB 0x40 -> 64k
    stosw                               ;0x04 = Window granularity in KB
    stosw                               ;0x06 = Window size in KB
    mov       ax, 0a000h                ;Segment 0xa000
WindowSize32k:                          ;Offset 0x4ba4
    stosw                               ;0x08 = Start segment of Window A
    stosw                               ;0x0A = Start segment of Window B
    push      di                        ;Save ouput pointer
    add       di, 10h                   ;0x1C
    movsd                               ;si is pointing at VESAWindowAttributes + 2, which is DB 000h, 000h, 001h, 000h
                                        ;1Ch    BYTE    size of bank in KB (zero, no planar VESA modes supported)
                                        ;1Dh    BYTE    number of image pages (less one) that will fit in video RAM (updated later)
                                        ;1Eh    BYTE    reserved (00h for VBE 1.0-2.0, 01h for VBE 3.0)
                                        ;---VBE v1.2+ ---
                                        ;1Fh    BYTE    red mask size (this one is overwritten late)
    pop       di                        ;restore 0x0c
    pop       si                        ;restore pointer to that data pointed to by current VESAModeData
    lea       ax, ds:[DirectCallableCPUVideoMemoryControl];Offset 0x4cc5
    stosw                               ;0x0C = FAR Window positioning function 
    mov       ax, ds
    stosw                               ;0x0E = segment (our code segment)
    mov       cx, 03h                   ;
                                        ;0x10 = bytes per scan line
                                        ;---remainder is optional for VESA modes in v1.0/1.1, needed for OEM modes---
                                        ;0x12 = width in pixels (graphics) or characters (text)
                                        ;0x14 = height in pixels (graphics) or characters (text)
    rep movsw
    mov       al, 08h
    stosb                               ;0x16 = Width of character cell in pixels
    movsb                               ;0x17 = Height of character cell in pixels
    movsd                               ;0x18 = number of memory planes
                                        ;0x19 = number of bits per pixel
                                        ;0x1A = number of banks
                                        ;0x1B = memory model type
    push      si
    dec       si
    cmp       byte ptr ds:[si], 06h     ;Point to 0x1B - Memory model type
    jne       Done                      ;Offset 0x4bf4
    sub       si, 02h                   ;Point to 0x19 - Number of bits per pixel
    cmp       byte ptr ds:[si], 0fh     ;15bpp
    jne       Not15bpp                  ;Offset 0x4bd5
    mov       si, offset PixelData15bpp ;Offset 0x4f4e
    jmp       Label0x4bec               ;Offset 0x4bec
Not15bpp:                               ;Offset 0x4bd5
    cmp       byte ptr ds:[si], 10h     ;16bpp
    jne       Label0x4bdf               ;Offset 0x4bdf
    mov       si, offset PixelData16bpp ;Offset 0x4f57
    jmp       Label0x4bec               ;Offset 0x4bec
Label0x4bdf:                            ;Offset 0x4bdf
    cmp       byte ptr ds:[si], 18h     ;24bpp
    jne       Label0x4be9               ;Offset 0x4be9
    mov       si, offset PixelData24bpp ;Offset 0x4f60
    jmp       Label0x4bec               ;Offset 0x4bec
Label0x4be9:                            ;Offset 0x4be9
    mov       si, offset PixelData32bpp ;Offset 0x4f69
Label0x4bec:                            ;Offset 0x4bec
    add       di, 03h                   ;Point to 0x1F - Red mask size
    mov       cx, 09h                   ;Copy 9 bytes
    rep movsb                           ;---VBE v1.2+ ---
                                        ;0x1F = red mask size
                                        ;0x20 = red field position
                                        ;0x21 = green mask size
                                        ;0x22 = green field size
                                        ;0x23 = blue mask size
                                        ;0x24 = blue field size
                                        ;0x25 = reserved mask size
                                        ;0x26 = reserved mask position
                                        ;0x27 = direct color mode info
Done:                                   ;Offset 0x4bf4
    pop       si                        ;restore pointer into that data pointed to by current VESAModeData, after memory model type
                                        ;not used.
    pop       di                        ;Restored to point at start of buffer
    call      SetNumberOfImagePlanesAndModeAvailability;Offset 0x4fed
    ret
GetSuperVGAModeInfo ENDP

SetSuperVGAMode PROC NEAR               ;Offset 0x4bfa
    mov    cx, bx
    mov    byte ptr ss:[bp + 11h], 00h
    test   bx, 7f00h                    ;Test if it's a legacy mode
    jne    ConvertVESAToLegacy          ;Offset 0x4c22
SetVESAMode:                            ;Offset 0x4c06
    xor    ah, ah                       ;Legacy function 0x00
    mov    al, bl                       ;Mode comes in in bl, but SetVideoMode expects it in al
    test   cx, 8000h                    ;Test if screen should be cleared
    je     ClearScreen                  ;Offset 0x4c12
    or     al, 80h
ClearScreen:                            ;Offset 0x4c12
    pusha
    mov    ds, word ptr cs:[Data1488]   ;Offset 0x1488
    call   SetVideoMode                 ;Offset 0x1819
    popa
    ret
Failure:                                ;Ofsset 0x4c1d
    mov    byte ptr ss:[bp + 11h], 01h
    ret
ConvertVESAToLegacy:                    ;Offset 0x4c22
    xor    di, di                       ;Set index to zero
    push   cs
    pop    ds                           ;
    and    bx, 7fffh                    ;And off top bit
    lea    si, ds:[VESAVideoModeIds]    ;Offset 0x4d28
    call   VESAFindModeData             ;Offset 0x4c4a
    jne    Found                        ;Offset 0x4c3e
    add    si, 02h
    add    di, 03h
    call   VESAFindModeData             ;Offset 0x4c4a
    je     Failure                      ;Ofsset 0x4c1d
Found:                                  ;Offset 0x4c3e
    lea    si, ds:[VESAModeData]        ;Offset 0x4d82
    add    si, di                       ;Add the found offset to the data pointer to point to the found mode data
    mov    bl, byte ptr ds:[si]         ;Load the converted legacy mode 0x49 - 0x7c These are internal ID's that don't 
                                        ;adhere to any standard. They are used to have a single mode set function.
    xor    bh, bh
    jmp    SetVESAMode                  ;Offset 0x4c06
SetSuperVGAMode ENDP

VESAFindModeData PROC NEAR              ;Offset 0x4c4a
    DB 081h, 03Ch, 0FFh, 0FFh           ;cmp    word ptr ds:[si], 0ffffh - masm encoding difference
    je     Failure                      ;Offset 0x4c5f
    cmp    word ptr ds:[si], bx
    je     Label0x4c5c                  ;Offset 0x4c5c
    add    di, 03h
    add    si, 02h
    jmp    VESAFindModeData             ;Offset 0x4c4a
Label0x4c5c:                            ;Offset 0x4c5c
    cmp    bl, 0ffh
Failure:                                ;Offset 0x4c5f
    ret
VESAFindModeData ENDP

GetCurrentSuperVGAMode PROC NEAR        ;Offset 0x4c60
    mov    byte ptr ss:[bp + 11h], 00h
    xor    di, di
    xor    ax, ax
    mov    ds, ax
    xor    bh, bh
    mov    bl, byte ptr ds:[BDA_DisplayMode];Offset 0x449 Legacy Mode id
    push   cs
    pop    ds
    lea    si, ds:[VESAModeData]        ;Offset 0x4d82
    call   FindModeFromVESAModeData     ;Offset 0x4c87
    je     NotFound                     ;Offset 0x4c83
    lea    si, ds:[VESAVideoModeIds]    ;Offset 0x4d28
    add    si, di                       ;di is offset into VESAVideoModeIds
    mov    bx, word ptr ds:[si]         ;Gets the VESA mode ID
NotFound:                               ;Offset 0x4c83
    mov    word ptr ss:[bp + 0eh], bx
    ret
GetCurrentSuperVGAMode ENDP

;inputs:
;bl = legacy mode
;ds:si points to VESAModeData
;
;outputs:
;di will be incremented by N*2 loops over VESAModeData until mode is found, this can point into the VESAVideoModeIds table
;si will point to found VESAModeData entry.
FindModeFromVESAModeData PROC NEAR      ;Offset 0x4c87
    cmp    byte ptr ds:[si], 0ffh       ;Terminate?
    je     NotFound                     ;Offset 0x4c9b
    cmp    byte ptr ds:[si], bl         ;Found?
    je     Finished                     ;Offset 0x4c98
    add    di, 02h
    add    si, 03h
    jmp    FindModeFromVESAModeData     ;Offset 0x4c87
Finished:                               ;Offset 0x4c98
    cmp    bl, 0ffh
NotFound:                               ;Offset 0x4c9b
    ret
FindModeFromVESAModeData ENDP

SaveRestoreSuperVGAVideoState PROC NEAR ;Offset 0x4c9c
    mov  byte ptr ss:[bp + 11h], 01h    ;ah status - failure
    cmp  dl, 02h                        ;subfunction 0x2 (the highest)
    ja   UnknownSubfunction             ;Offset 0x4cb2
    mov  byte ptr ss:[bp + 11h], 00h    ;ah status - success
    mov  ah, INT10_1C_SaveRestoreVideoState;0x1c
    mov  al, dl
    int  10h
    mov  word ptr ss:[bp + 0eh], bx     
UnknownSubfunction:                     ;Offset 0x4cb2
    ret
SaveRestoreSuperVGAVideoState ENDP

;inputs:
;bh = subfunction
;   0x00 - Select Video memory window
;   0x01 - Get Video memory window
;outputs:
;dx = window address in video memory
;bl = window index
;   0x00 = Window A
;   0x01 = Window B
;ah = status
;   0x00 = success
;   0x01 = fail
CPUVideoMemoryControl PROC NEAR         ;Offset 0x4cb3
    call SetVideoMemoryWindow           ;Offset 0x5123
    cmp  bh, 01h
    jne  NotGetSubfunction              ;Offset 0x4cbe
    call GetVideoMemoryWindow           ;Offset 0x5148
NotGetSubfunction:                      ;Offset 0x4cbe
    mov  byte ptr ss:[bp + 11h], ah
    mov  word ptr ss:[bp + 0ah], dx
    ret
CPUVideoMemoryControl ENDP

;Same as above, minus the stack return values
DirectCallableCPUVideoMemoryControl PROC FAR;Offset 0x4cc5
    call SetVideoMemoryWindow           ;Offset 0x5123
    cmp  bh, 01h
    jne  NotGetSubfunction              ;Offset 0x4cd0
    call GetVideoMemoryWindow           ;Offset 0x5148
NotGetSubfunction:                      ;Offset 0x4cd0
    retf
DirectCallableCPUVideoMemoryControl ENDP

;inputs:
;bl = subfunction 0x00 = set logical scan line length, 0x01 = get logical scan line length
;cx = desired logical scan line length in pixels (func 0)
;
;outputs:
;ah = status 0x00 = success, 0x01 = fail
;bx = bytes per scan line
;cx = pixels per scan line
;dx = maximum number of scan lines
;
;If the desired width is not achievable, the next larger width will be set.
;The scan line may be wider than the visible area of the screen. 
;This function is valid in text modes, provided that pixel values are multiplied by the character cell width/height.

GetSetLogicalScanLineLength PROC NEAR   ;Offset 0x4cd1
    mov  byte ptr ss:[bp + 11h], 01h    ;status = 0x01 - failure
    cmp  bl, 01h                        ;subfunction 0x01 is highest valid subfunction. 
    ja   Exit                           ;Offset 0x4cf5
    xor  ax, ax
    mov  ds, ax
    mov  bh, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push cs
    pop  ds
    xchg bh, bl                         ;bl = current mode, bh = subfunction
    lea  si, [VESAModeData]             ;Offset 0x4d82
    call FindModeFromVESAModeData       ;Offset 0x4c87
    je   Exit                           ;Offset 0x4cf5 mode not found
    mov  si, (VESAMode ptr ds:[si]).ModeInfo
    call GetSetLogicalScanLineLengthInternal;Offset 0x504a
Exit:                                   ;Offset 0x4cf5
    ret
GetSetLogicalScanLineLength ENDP

;
;info from: http://www.ctyme.com/intr/rb-0282.htm
;
;AX = 4F07h
;BL = subfunction
;00h,80h set display start
;01h get display start
;
;set inputs:
;CX = leftmost displayed pixel in scan line
;DX = first displayed scan line
;
;ouputs:
;AL = 4Fh if function supported
;AH = status - 0x00 = success, 0x01 = failure
;
;other ouputs(get):
;BH = 00h
;CX = leftmost displayed pixel in scan line
;DX = first displayed scan line.
;
;This function is valid in text modes, provided that values are multiplied by the character cell width/height.
;Subfunction 80h waits until vertical retrace before changing the display start address (VBE v2.0+)
GetSetDisplayStart PROC NEAR            ;Offset 0x4cf6
    call  SetDisplayStart               ;Offset 0x5177
    cmp   ah, 01h
    jne   Exit                          ;Offset 0x4d0c
    call  GetDisplayStart               ;Offset 0x5203
    cmp   ah, 00h
    jne   Exit                          ;Offset 0x4d0c
    mov   word ptr ss:[bp + 0ch], cx    ;
    mov   word ptr ss:[bp + 0ah], dx    ;
Exit:                                   ;Offset 0x4d0c
    mov   byte ptr ss:[bp + 11h], ah    ;Store result 0x00 = success, 0x01 = failure
    ret
GetSetDisplayStart ENDP

;Offset 0x4d10
OEMName                 DB 'S3 Incorporated. Trio64', 000h

;Offset 0x4d28
;This list maps on to the VESAModeData list below.
VESAVideoModeIds        DW 00100h;0x68
                        DW 00101h;0x69
                        DW 00102h;0x6A
                        DW 00103h;0x6B
                        DW 00104h;0x6C
                        DW 00105h;0x6D
                        DW 00106h;0x6E
                        DW 00107h;0x6F
                        DW 00109h;0x55
                        DW 0010Ah;0x54
                        DW 00110h;0x70
                        DW 00111h;0x71
                        DW 00112h;0x72
                        DW 00113h;0x73
                        DW 00114h;0x74
                        DW 00115h;0x75
                        DW 00116h;0x76
                        DW 00117h;0x77
                        DW 00118h;0x78
                        DW 00119h;0x79
                        DW 0011Ah;0x7A
                        DW 00120h;0x7C
                        DW 0FFFFh
                        ;
                        DW 00201h;0x49
                        DW 00202h;0x4A
                        DW 00203h;0x4B
                        DW 00204h;0x4C
                        DW 00205h;0x4D
                        DW 00207h;0x4E
                        DW 00208h;0x4F
                        DW 00213h;0x52
                        ;These last numbers will never be hit.
                        DW 00107h
                        DW 00110h
                        DW 00111h
                        DW 00112h
                        DW 00113h
                        DW 00114h
                        DW 00115h
                        DW 00116h
                        DW 00117h
                        DW 00118h
                        DW 00119h
                        DW 0011Ah
                        DW 00120h
                        DW 0FFFFh;Except this one. Terminate

VESAMode STRUCT
    ModeId BYTE ?
    ModeInfo WORD ?
VESAMode ENDS

;Offset 0x4d82
VESAModeData            VESAMode < INT10_00_68_G_640x400x256C, offset VESAModeInfo_68_G_640x400x256C >
                        VESAMode < INT10_00_69_G_640x480x256C, offset VESAModeInfo_69_G_640x480x256C >
                        VESAMode < INT10_00_6A_G_800x600x16C, offset VESAModeInfo_6A_G_800x600x16C >
                        VESAMode < INT10_00_6B_G_800x600x256C, offset VESAModeInfo_6B_G_800x600x256C >
                        VESAMode < INT10_00_6C_1024x768x16C, offset VESAModeInfo_6C_1024x768x16C >
                        VESAMode < INT10_00_6D_1024x768x256C, offset VESAModeInfo_6D_1024x768x256C >
                        VESAMode < INT10_00_6E_1280x1024x16C, offset VESAModeInfo_6E_1280x1024x16C >
                        VESAMode < INT10_00_6F_1280x1024x256C, offset VESAModeInfo_6F_1280x1024x256C >
                        VESAMode < INT10_00_55_T_132x25, offset VESAModeInfo_55_T_132x25 >
                        VESAMode < INT10_00_54_T_132x43, offset VESAModeInfo_54_T_132x43 >
                        VESAMode < INT10_00_70_640x480x32K, offset VESAModeInfo_70_640x480x32K >
                        VESAMode < INT10_00_71_640x480x64K, offset VESAModeInfo_71_640x480x64K >
                        VESAMode < INT10_00_72_640x480x16M, offset VESAModeInfo_72_640x480x16M >
                        VESAMode < INT10_00_73_800x600x32K, offset VESAModeInfo_73_800x600x32K >
                        VESAMode < INT10_00_74_800x600x64K, offset VESAModeInfo_74_800x600x64K >
                        VESAMode < INT10_00_75_800x600x16M, offset VESAModeInfo_75_800x600x16M >
                        VESAMode < INT10_00_76_1024x768x32K, offset VESAModeInfo_76_1024x768x32K >
                        VESAMode < INT10_00_77_1024x768x64K, offset VESAModeInfo_77_1024x768x64K >
                        VESAMode < INT10_00_78_1024x768x16M, offset VESAModeInfo_78_1024x768x16M >
                        VESAMode < INT10_00_79_1280x1024x32K, offset VESAModeInfo_79_1280x1024x32K >
                        VESAMode < INT10_00_7A_1280x1024x64K, offset VESAModeInfo_7A_1280x1024x64K >
                        VESAMode < INT10_00_7C_1600x1200x256, offset VESAModeInfo_7C_1600x1200x256 >
                        VESAMode < 080h, 00000h >;maps on to 0xffff above
                        VESAMode < INT10_00_49_640x480x256C, offset VESAModeInfo_49_640x480x256C >
                        VESAMode < INT10_00_4A_800x600x16C, offset VESAModeInfo_4A_800x600x16C >
                        VESAMode < INT10_00_4B_800x600x256C, offset VESAModeInfo_4B_800x600x256C >
                        VESAMode < INT10_00_4C_1024x768x16C, offset VESAModeInfo_4C_1024x768x16C >
                        VESAMode < INT10_00_4D_1024x768x256C, offset VESAModeInfo_4D_1024x768x256C >
                        VESAMode < INT10_00_4E_1152x864x256C, offset VESAModeInfo_4E_1152x864x256C >
                        VESAMode < INT10_00_4F_1280x1024x16C, offset VESAModeInfo_4F_1280x1024x16C >
                        VESAMode < INT10_00_52_640x400x16M, offset VESAModeInfo_52_640x400x16M >
                        DB 0FFh;Terminate

;Bit(s)  Description     (Table 00081)
;0      exists
;1      readable
;2      writable
;3-7    reserved
VESAWindowAttributes    DW 00007h       ;Offset 0x4de0

;The following 4 bytes are copied into these fiels in the GetSuperVGAModeInfo function
;1Ch    BYTE    size of bank in KB
;1Dh    BYTE    number of image pages (less one) that will fit in video RAM
;1Eh    BYTE    reserved (00h for VBE 1.0-2.0, 01h for VBE 3.0)
;---VBE v1.2+ ---
;1Fh    BYTE    red mask size
                        DB 000h, 000h, 001h, 000h


;Offset
;00 = Byte - Mode attributes
;     bit 0 = Mode supported by present hardware configuration
;     bit 1 = Optional information available (must be =1 for VBE 1.2+)
;     bit 2 = BIOS output supported
;     bit 3 = Set if color, clear if monochrome
;     bit 4 = Set if graphics mode, clear if text mode
;
;01 = Word - Bytes per scanline
;03 = Word - Width in pixels (graphics) or characters (text)
;05 = Word - Height in pixels (graphics) or characters (text)
;07 = Byte - Height of character cell in pixels
;08 = Byte - Number of memory planes
;09 = Byte - Number of bits per pixel
;0A = Byte - Number of banks
;0B = Byte - Memory model type
;       00h    text
;       01h    CGA graphics
;       02h    HGC graphics
;       03h    16-color (EGA) graphics
;       04h    packed pixel graphics
;       05h    "sequ 256" (non-chain 4) graphics
;       06h    direct color (HiColor, 24-bit color)
;       07h    YUV (luminance-chrominance, also called YIQ)
;       08h-0Fh reserved for VESA
;       10h-FFh OEM memory models

VESAModeInfo STRUCT
    ModeAttributes BYTE ?
    BytesPerScanline WORD ?
    WidthInPixels WORD ?
    HeightInPixels WORD ?
    HeightOfCharacterCellInPixels BYTE ?
    MemoryPlanes BYTE ?
    BitsPerPixel BYTE ?
    Banks BYTE ?
    MemoryModelType BYTE ?
VESAModeInfo ENDS

VESAModeInfo_68_G_640x400x256C VESAModeInfo < 01Bh, 00280h, 00280h, 00190h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_69_G_640x480x256C VESAModeInfo < 01Bh, 00280h, 00280h, 001E0h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_6A_G_800x600x16C VESAModeInfo < 01Fh, 00064h, 00320h, 00258h, 008h, 004h, 004h, 001h, 003h >
VESAModeInfo_6B_G_800x600x256C VESAModeInfo < 01Bh, 00320h, 00320h, 00258h, 008h, 001h, 008h, 001h, 004h >
VESAModeInfo_6C_1024x768x16C VESAModeInfo < 01Bh, 00080h, 00400h, 00300h, 010h, 004h, 004h, 001h, 003h >
VESAModeInfo_6D_1024x768x256C VESAModeInfo < 01Bh, 00400h, 00400h, 00300h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_6E_1280x1024x16C VESAModeInfo < 01Bh, 000A0h, 00500h, 00400h, 010h, 004h, 004h, 001h, 003h >
VESAModeInfo_6F_1280x1024x256C VESAModeInfo < 01Bh, 00500h, 00500h, 00400h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_55_T_132x25 VESAModeInfo < 00Fh, 00108h, 00084h, 00019h, 010h, 004h, 004h, 001h, 000h >
VESAModeInfo_54_T_132x43 VESAModeInfo < 00Fh, 00108h, 00084h, 0002Bh, 008h, 004h, 004h, 001h, 000h >
VESAModeInfo_70_640x480x32K VESAModeInfo < 01Bh, 00500h, 00280h, 001E0h, 010h, 001h, 00Fh, 001h, 006h >
VESAModeInfo_71_640x480x64K VESAModeInfo < 01Bh, 00500h, 00280h, 001E0h, 010h, 001h, 010h, 001h, 006h >
VESAModeInfo_72_640x480x16M VESAModeInfo < 01Bh, 00A00h, 00280h, 001E0h, 010h, 001h, 020h, 001h, 006h >
VESAModeInfo_73_800x600x32K VESAModeInfo < 01Bh, 00640h, 00320h, 00258h, 008h, 001h, 00Fh, 001h, 006h >
VESAModeInfo_74_800x600x64K VESAModeInfo < 01Bh, 00640h, 00320h, 00258h, 008h, 001h, 010h, 001h, 006h >
VESAModeInfo_75_800x600x16M VESAModeInfo < 01Bh, 00C80h, 00320h, 00258h, 008h, 001h, 020h, 001h, 006h >
VESAModeInfo_76_1024x768x32K VESAModeInfo < 01Bh, 00800h, 00400h, 00300h, 010h, 001h, 00Fh, 001h, 006h >
VESAModeInfo_77_1024x768x64K VESAModeInfo < 01Bh, 00800h, 00400h, 00300h, 010h, 001h, 010h, 001h, 006h >
VESAModeInfo_78_1024x768x16M VESAModeInfo < 01Bh, 01000h, 00400h, 00300h, 010h, 001h, 020h, 001h, 006h >
VESAModeInfo_79_1280x1024x32K VESAModeInfo < 01Bh, 00A00h, 00500h, 00400h, 010h, 001h, 00Fh, 001h, 006h >
VESAModeInfo_7A_1280x1024x64K VESAModeInfo < 01Bh, 00A00h, 00500h, 00400h, 010h, 001h, 010h, 001h, 006h >
VESAModeInfo_7C_1600x1200x256 VESAModeInfo < 01Bh, 00640h, 00640h, 004B0h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_49_640x480x256C VESAModeInfo < 01Bh, 00400h, 00280h, 001E0h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_4A_800x600x16C VESAModeInfo < 01Bh, 00400h, 00320h, 00258h, 008h, 001h, 004h, 001h, 004h >
VESAModeInfo_4B_800x600x256C VESAModeInfo < 01Bh, 00400h, 00320h, 00258h, 008h, 001h, 008h, 001h, 004h >
VESAModeInfo_4C_1024x768x16C VESAModeInfo < 01Bh, 00400h, 00400h, 00300h, 010h, 001h, 004h, 001h, 004h >
VESAModeInfo_4D_1024x768x256C VESAModeInfo < 01Bh, 00400h, 00400h, 00300h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_4E_1152x864x256C VESAModeInfo < 01Bh, 00480h, 00480h, 00360h, 010h, 001h, 008h, 001h, 004h >
VESAModeInfo_4F_1280x1024x16C VESAModeInfo < 01Bh, 00400h, 00500h, 00400h, 010h, 001h, 004h, 001h, 004h >
VESAModeInfo_52_640x400x16M VESAModeInfo < 01Bh, 00A00h, 00280h, 00190h, 010h, 001h, 020h, 001h, 006h >

;0 = red mask size
;1 = red field position
;2 = green mask size
;3 = green field size
;4 = blue mask size
;5 = blue field size
;6 = reserved mask size
;7 = reserved mask position
;8 = direct color mode info
PixelData15bpp          DB 005h, 00Ah, 005h, 005h, 005h, 000h, 001h, 00Fh, 000h;Offset 0x4f4e
PixelData16bpp          DB 005h, 00Bh, 006h, 005h, 005h, 000h, 000h, 000h, 000h;Offset 0x4f57
PixelData24bpp          DB 008h, 010h, 008h, 008h, 008h, 000h, 000h, 000h, 000h;Offset 0x4f60
PixelData32bpp          DB 008h, 010h, 008h, 008h, 008h, 000h, 008h, 018h, 000h;Offset 0x4f69

VESASetDisplayPowerState PROC NEAR      ;Offset 0x4f72
    call TurnOffScreen                  ;Offset 0x47ed
    mov  al, 18h                        ;SR18 - RAMDAC/CLKSYN Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 20h                        ;bit 5 = 1 - Power down RAMDAC
    out  dx, ax
    mov  al, 0dh                        ;SRD - Extended Sequencer register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fh                        ;Clear out HSync and VSync control bits
    cmp  bh, 04h                        ;Display off
    je   DisplayOff                     ;Offset 0x4fab
    cmp  bh, 02h                        ;Display suspend
    je   DisplaySuspend                 ;Offset 0x4fb3
    cmp  bh, 01h                        ;Display standby
    je   DisplayStandby                 ;Offset 0x4fbb
    cmp  bh, 00h
    je   DisplayOn                      ;Offset 0x4f9c
    jmp  Exit                           ;Offset 0x4fc1
DisplayOn:                              ;Offset 0x4f9c
    out  dx, ax                         ;ax still loaded with cleared out HSync and VSync control bits and SRD
    mov  al, 18h                        ;SR18 - RAMDAC/CLKSYN Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0dfh                       ;bit 5 = 0 - Power up RAMDAC
    out  dx, ax
    call TurnOnScreen                   ;Offset 0x4800
    jmp  Exit                           ;Offset 0x4fc1
DisplayOff:                             ;Offset 0x4fab
    or   ah, 50h                        ;HSync and VSync both output 0 and nothing else
    out  dx, ax
    xor  ah, ah
    jmp  Exit                           ;Offset 0x4fc1
DisplaySuspend:                         ;Offset 0x4fb3
    or   ah, 40h                        ;VSync outputs 0 and nothing else (HSync normal operation)
    out  dx, ax
    xor  ah, ah
    jmp  Exit                           ;Offset 0x4fc1
DisplayStandby:                         ;Offset 0x4fbb
    or   ah, 10h                        ;HSync outputs 0 and nothing else (VSync normal operation)
    out  dx, ax
    xor  ah, ah
Exit:                                   ;Offset 0x4fc1
    ret
VESASetDisplayPowerState ENDP

VESAGetDisplayPowerState PROC NEAR      ;Offset 0x4fc2
    mov  al, 0dh                        ;SRD - Extended Sequencer register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0f0h                       ;Keep VSync and HSync control
    cmp  ah, 00h                        ;both VSync and HSync are 00 - Normal operation
    je   DisplayOn                      ;Offset 0x4fde
    cmp  ah, 10h                        ;HSync = 01 - HSync outputs 0 (nothing else), VSync normal operation
    je   DisplayStandby                 ;Offset 0x4fe2
    cmp  ah, 40h                        ;VSync = 01 - VSync outputs 0 (nothing else), HSync normal operation
    je   DisplaySuspend                 ;Offset 0x4fe6
    cmp  ah, 50h                        ;VSync and HSync = 01 both output 0 (nothing else)
    je   DisplayOff                     ;Offset 0x4fea
DisplayOn:                              ;Offset 0x4fde
    xor  bh, bh
    jmp  Exit                           ;Offset 0x4fec
DisplayStandby:                         ;Offset 0x4fe2
    mov  bh, 01h
    jmp  Exit                           ;Offset 0x4fec
DisplaySuspend:                         ;Offset 0x4fe6
    mov  bh, 02h
    jmp  Exit                           ;Offset 0x4fec
DisplayOff:                             ;Offset 0x4fea
    mov  bh, 04h
Exit:                                   ;Offset 0x4fec
    ret
VESAGetDisplayPowerState ENDP

;inputs:
;es:di points to VESA mode information buffer
SetNumberOfImagePlanesAndModeAvailability PROC NEAR;Offset 0x4fed
    push eax
    push ebx
    push edx
    mov  cx, word ptr es:[di + 10h]     ;0x10 = Bytes per scan line
    cmp  byte ptr es:[di + 1bh], 03h    ;0x1B = Memory model type 0x3 = 16 color (EGA) graphics
    jne  Not16Color1                    ;Offset 0x5001
    shl  cx, 02h                        ;bytes per scan line * 4
Not16Color1:                            ;Offset 0x5001
    mov  ax, word ptr es:[di + 14h]     ;0x14 = Height in pixels (graphics) or characters (text)
    mul  cx                             ;heigt * bytes per scan line (* 4 for 16 color), result in dx:ax
    mov  bx, dx                         ;
    shl  ebx, 10h
    mov  bx, ax                         ;ebx is now full size of screen
    mov  cl, dl
    call GetInstalledMemorySizeIn4KBlocks;Offset 0x1457
    shr  dx, 04h                        ;Memory is now 4k * 16 = 64KiB blocks
    cmp  dl, 00h                        ;If 0.5MiB
    jne  Not05MiB                       ;Offset 0x501e
    mov  dl, 08h                        ;8 * 64k = 512KiB
Not05MiB:                               ;Offset 0x501e
    DB 03Ah, 0CAh                       ;cmp  cl, dl - masm encoding difference
    jle  ScreenCanFitInMemory           ;Offset 0x5028
    and  word ptr es:[di], 0fffeh       ;Mode Attributes - bit 0 = 0 - Mode NOT supported by present hardware configuration
    jmp  Done                           ;Offset 0x5043
ScreenCanFitInMemory:                   ;Offset 0x5028
    mov  ax, dx                         ;ax = memory in 64KiB blocks
    cmp  byte ptr es:[di + 1bh], 03h    ;0x1B = Memory model type 0x3 = 16 color (EGA) graphics
    jne  Not16Color2                    ;Offset 0x5034
    mov  ax, 10h                        ;10 64KiB blocks - regardless of actual memory size
Not16Color2:                            ;Offset 0x5034
    shl  eax, 10h                       ;Multiply by 65536 - full size in bytes
    xor  edx, edx                       ;clear out edx as it's part of the division
    div  ebx                            ;Divide eax by ebx - result in eax
    dec  ax                             ;Number of screens minus one
    mov  byte ptr es:[di + 1dh], al     ;Number of image planes
Done:                                   ;Offset 0x5043
    pop  edx
    pop  ebx
    pop  eax
    ret
SetNumberOfImagePlanesAndModeAvailability ENDP

;inputs:
;si = pointer to VESA Mode Info struct
;bl = current mode
;bh = subfunction
;cx = desired logical scan line length in pixels
;
;outputs:
;bx = bytes per scan line
;cx = pixels per scan line
;dx = maximum number of scan lines
;

VESAModeInfo STRUCT
    ModeAttributes BYTE ?                   ;0x00
    BytesPerScanline WORD ?                 ;0x01
    WidthInPixels WORD ?                    ;0x03
    HeightInPixels WORD ?                   ;0x05
    HeightOfCharacterCellInPixels BYTE ?    ;0x07
    MemoryPlanes BYTE ?                     ;0x08
    BitsPerPixel BYTE ?                     ;0x09
    Banks BYTE ?                            ;0x0A
    MemoryModelType BYTE ?                  ;0x0B
VESAModeInfo ENDS

GetSetLogicalScanLineLengthInternal PROC NEAR;Offset 0x504a
    xor  dx, dx                         ;clear dx
    mov  ax, cx                         ;preserve requested logical scan line length in pixels in ax
    cmp  bh, 01h                        ;subfunction 0x01 = get
    je   GetScanLines                   ;Offset 0x50af
    push cx                             ;Store requested logical scan line length
    xor  eax, eax                       ;
    xor  edx, edx                       ;
    xor  ecx, ecx                       ;
    pop  cx                             ;Restore requested logical scan line length
    mov  al, (VESAModeInfo ptr ds:[si]).BitsPerPixel
    cmp  al, 0fh                        ;15bpp
    jne  Not15bpp1                      ;Offset 0x5067
    inc  al
Not15bpp1:                              ;Offset 0x5067
    mul  ecx                            ;eax = bits per pixel * requested scan line size in pixels
    shr  eax, 03h                       ;divide by 8 to give requested scan line size in bytes
    mov  dx, ax                         ;store requested bytes in dx
    shr  eax, 10h                       ;eax = 0?
    xchg ax, dx                         ;dx = 0?, ax = scan line size in bytes
    cmp  ax, (VESAModeInfo ptr ds:[si]).BytesPerScanline
    jae  GreaterThanModeBytesPerScanline;Offset 0x507d - Can't request the bytes per scan line to be less than the mode requires
    mov  ax, (VESAModeInfo ptr ds:[si]).BytesPerScanline
GreaterThanModeBytesPerScanline:        ;Offset 0x507d
    mov  bx, 08h                        ;
    div  bx                             ;ax is result / 8, dx = remainder (which stomps the value calculated above?)
    cmp  dx, 00h                        ;check if remainder == 0x00
    je   NoRemainder                    ;Offset 0x5088
    inc  ax                             ;ax++
NoRemainder:                            ;Offset 0x5088
    cmp  ah, 03h                        ;if requested bytes per scan line is larger than 3 * 256 * 8 = 6,144
    jbe  RequestedSizeWithinBounds      ;Offset 0x5090
    jmp  Failure                        ;Offset 0x50fd - failure
    nop
RequestedSizeWithinBounds:              ;Offset 0x5090
    push ax                             ;Store requested scan line size in 8 byte blocks
    mov  bx, ax                         ;also store in bx
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 13h                        ;CR13 - Offset register
    mov  ah, bl                         ;
    out  dx, ax                         ;10-bit Value = quantity that is multiplied by:
                                        ;               2 (word mode)
                                        ;               4 (doubleword mode)
                                        ;               8 (quadword mode)
                                        ;               to specify the difference between the starting byte addresses
                                        ;               of two consecutive scan lines.
                                        ;               This register contains the least significant 8 bits of this value.
                                        ;               The addressing mode is specified by bit 6 of CR14 and bit 3 of CR17.
                                        ;               Setting bit 3 of CR31 to 1 forces doubleword mode.
    mov  al, 51h                        ;CR 51 - Extended System Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    shl  bh, 04h                        ;shift up the two bottom bits of bh to bits 5-4
    and  ah, 0cfh                       ;clear out bits 5-4 (Logical screen width bits 9-8)
    or   ah, bh                         ;set bits 5-4 (Logical screen width bits 9-8)
    out  dx, ax                         ;
    pop  ax                             ;restore scan line size in 8 byte blocks
    shl  ax, 03h                        ;multiply by 8 - scan line size in bytes
    jmp  ComputeResults                 ;Offset 0x50cb
GetScanLines:                           ;Offset 0x50af
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 13h
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah
    mov  al, 51h
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 30h
    shr  ah, 04h
    mov  al, bl
    shl  ax, 03h
    jmp  ComputeResults                 ;Offset 0x50cb
ComputeResults:                         ;Offset 0x50cb
    push ax
    cmp  (VESAModeInfo ptr ds:[si]).MemoryModelType, 00h; 0x00 = text
    je   IsText                         ;Offset 0x50db
    xor  bx, bx
    xor  dx, dx
    mov  bl, (VESAModeInfo ptr ds:[si]).MemoryPlanes
    div  bx                             ;bytes per scan line / memory planes
IsText:                                 ;Offset 0x50db
    mov  word ptr ss:[bp + 0eh], ax     ;store result
    pop  ax                             ;restore scan line size in bytes
    xor  bx, bx                         ;
    mov  bl, (VESAModeInfo ptr ds:[si]).BitsPerPixel
    cmp  bl, 0fh                        ;15bpp
    jne  Not15bpp2                      ;Offset 0x50eb
    inc  bl                             ;16bpp
Not15bpp2:                              ;Offset 0x50eb
    xor  dx, dx                         ;
    div  bx                             ;divide by bits per pixel
    shl  ax, 03h                        ;multiply by 8
    mov  word ptr ss:[bp + 0ch], ax     ;Store result
    call CalculateMaximumNumberOfScanLines;Offset 0x5102
    mov  byte ptr ss:[bp + 11h], 00h    ;0x00 = success
    ret
Failure:                                ;Offset 0x50fd
    mov  byte ptr ss:[bp + 11h], 01h    ;0x01 = failure
    ret
GetSetLogicalScanLineLengthInternal ENDP

CalculateMaximumNumberOfScanLines PROC NEAR;Offset 0x5102
    call GetInstalledMemorySizeIn4KBlocks;Offset 0x1457
    shr  dx, 04h                        ;size in 64KiB blocks
    cmp  dx, 00h                        ;
    jne  IsNot05MiB                     ;Offset 0x510f
    mov  dl, 08h                        ;8 * 64KiB = 512KiB
IsNot05MiB:                             ;Offset 0x510f
    xor  ax, ax                         ;dx:ax = memory in MiB
    mov  bx, word ptr ss:[bp + 0eh]     ;Fetch result
    cmp  (VESAModeInfo ptr ds:[si]).MemoryModelType, 03h; 0x03 = 16 color graphics
    jne  MemoryModel3                   ;Offset 0x511d
    shl  bx, 02h                        ;multiply by 4 - number of planes?
MemoryModel3:                           ;Offset 0x511d
    div  bx                             ;ax = dx:ax / bx
    mov  word ptr ss:[bp + 0ah], ax     ;maximum number of scan lines
    ret  
CalculateMaximumNumberOfScanLines ENDP

SetVideoMemoryWindow PROC NEAR          ;Offset 0x5123
    push dx
    push bx
    mov  ax, 014fh                      ;failure return value
    cmp  bh, 00h                        ;Subfunction 0x00 - Set Video memory window
    jne  WrongSubfunction               ;Offset 0x5145
    mov  bx, dx
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 04h                        ;SR4 - Memory Mode Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    test ah, 08h                        ;Test if bit 3 = 1 - Chain 4 Mode. 
                                        ;               This bit selects modulo 4 addressing for CPU access to display memory.
                                        ;               A logical 1 directs the two lower order bits of the CPU address used 
                                        ;               to select the plane in video memory to be accessed as follows:
                                        ;A1 AO Plane Selected
                                        ; 0  0              0
                                        ; 0  1              1
                                        ; 1  0              2
                                        ; 1  1              3
    jne  Planar                         ;Offset 0x513f
    shl  bx, 02h                        ;Address * 4
Planar:                                 ;Offset 0x513f
    call SetCPUUpperMemoryAddressBits   ;Offset 0x1207
    mov  ax, 004fh                      ;Success
WrongSubfunction:                       ;Offset 0x5145
    pop  bx
    pop  dx
    ret
SetVideoMemoryWindow ENDP

GetVideoMemoryWindow PROC NEAR          ;Offset 0x5148
    push bx
    mov  ax, 014fh                      ;failure return value
    cmp  bh, 01h                        ;Not subfunction 0x01
    jne  Exit                           ;Offset 0x5175
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 6ah                        ;CR6A - Extended System Control 4 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah                         ;Contains upper 6 bits (19-14 of the CPU base address.)
    and  bl, 7fh                        ;And off top bit
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 04h                        ;SR4 - Memory Mode Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    xor  dx, dx                         ;
    mov  dl, bl                         ;Store top 6 bits of CPU base address
    test ah, 08h                        ;Test if bit 3 is set - Chain 4 mode (See SetVideoMemoryWindow for info)
    mov  ax, 004fh                      ;Success return value
    jne  Exit                           ;Offset 0x5175
    shr  dl, 02h                        ;Divide by 4 if not chained
Exit:                                   ;Offset 0x5175
    pop  bx
    ret
GetVideoMemoryWindow ENDP

;BL = subfunction
;00h,80h set display start
;
;set inputs:
;CX = leftmost displayed pixel in scan line
;DX = first displayed scan line
SetDisplayStart PROC NEAR               ;Offset 0x5177
    push bx                             ;save subfunction
    mov  ax, 014fh                      ;result 0x01 = failure
    and  bl, 7fh                        ;And off top bit (wait for vsync, we don't do that)
    cmp  bl, 00h                        ;set subfunction
    je   DoSetDisplayStart              ;Offset 0x5186
    jmp  Exit                           ;Offset 0x5201
    nop
DoSetDisplayStart:                      ;Offset 0x5186
    push dx                             ;Store first displayed scan line
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 13h                        ;CR13 - Offset register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah                         ;store current value in bl
    mov  al, 51h                        ;CR51 - Extended System Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 30h                        ;keep bits 5-4 the msb's of the offset value
    shr  ah, 04h                        ;shift down to 1-0
    mov  al, bl                         ;ax = offset value
    shl  ax, 01h                        ;* 2
    push ax                             ;store
    mov  bx, cx                         ;bx = left most displayed pixel
    mov  al, 67h                        ;CR67 - Extended Miscellaneous Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0f0h                       ;Keep Color Mode bits
                                        ;0000 = Mode 0: 8-bit color, 1 pixel/VCLK
                                        ;0001 = Mode 8: 8-bit color, 2 pixels/VCLK
                                        ;0011 = Mode 9: 15-bit color, 1 pixel/VCLK
                                        ;0101 = Mode 10: 16-bit color, 1 pixel/VCLK
                                        ;0111 = Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Trio32 only)
                                        ;1101 = Mode 13: 24-bit color, 1 pixel/VCLK
    mov  cl, 02h                        ;divisor shr 2
    cmp  ah, 10h                        ;Check if Mode 8
    jbe  ModeIs8bpp                     ;Offset 0x51c4
    mov  cl, 01h                        ;divisor shr 1
    cmp  ah, 50h                        ;Check if Mode 10
    jbe  DivisorSet                     ;Offset 0x51d3
    mov  cl, 00h                        ;divisor shr 0
    cmp  ah, 70h                        ;Mode 12
    je   DivisorSet                     ;Offset 0x51d3
    mov  cl, 00h                        ;divisor shr 0
    jmp  DivisorSet                     ;Offset 0x51d3
ModeIs8bpp:                             ;Offset 0x51c4
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 04h                        ;SR4 - Memory Mode Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    test ah, 08h                        ;test bit 3 = 1 - Chain 4 mode
    jne  DivisorSet                     ;Offset 0x51d3
    mov  cl, 03h                        ;divisor shr 3
DivisorSet:                             ;Offset 0x51d3
    shr  bx, cl                         ;bx = left most displayed pixel >> cl
    pop  ax                             ;restore fetched offset * 2
    pop  dx                             ;restore first displayed scan line
    mul  dx                             ;dx:ax = ax * dx
    cmp  dx, 0fh                        ;check if result less or equal to 0xf???? - Seems to me that dx:ax + bx can still overflow this
    jbe  InBounds                       ;Offset 0x51e3
    mov  ax, 014fh                      ;0x01 = failure
    jmp  Exit                           ;Offset 0x5201
InBounds:                               ;Offset 0x51e3
    add  ax, bx                         ;start pixel to first displayed scan line * offset * 2
    mov  bx, dx                         ;bx is upper half
    jae  NoOverflow                     ;Offset 0x51ea
    inc  bx                             ;increment if carry?
NoOverflow:                             ;Offset 0x51ea
    push ax                             ;store bottom half
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 69h                        ;CR69 - Extended System Control 3 register
    mov  ah, bl                         ;set upper bit (19-16) of display start address
    out  dx, ax
    pop  bx                             ;restore bottom half
    mov  al, 0ch                        ;CRC - Start Address High register
    mov  ah, bh                         ;Write out bits (15-8) of display start address
    out  dx, ax
    mov  al, 0dh                        ;CRD - Start Address Low register
    mov  ah, bl                         ;Write out bits (7-0) of display start address
    out  dx, ax
    mov  ax, 004fh                      ;0x00 = success
Exit:                                   ;Offset 0x5201
    pop  bx
    ret  
SetDisplayStart ENDP

;input:
;bl = subfunction 0x01 = get
;
;outputs:
;CX = leftmost displayed pixel in scan line
;DX = first displayed scan line.
GetDisplayStart PROC NEAR               ;Offset 0x5203
    push bx
    mov  ax, 014fh                      ;failure
    cmp  bl, 01h                        ;subfunction 0x01
    jne  Exit                           ;Offset 0x5285
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    xor  bx, bx
    mov  al, 69h                        ;CR69 - Extended System Control 3 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 1fh                        ;keep bottom 5 bits (documentation says only 4 bits are valid?)
    mov  bl, ah                         ;keep in bl
    push bx                             ;store
    mov  al, 0ch                        ;CRC - Start Address High register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bh, ah                         ;Save in bh
    mov  al, 0dh                        ;CRD - Start Address Low register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah                         ;save in bl
    push bx                             ;store on stack
    mov  al, 13h                        ;CR13 - Offset register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    mov  bl, ah                         ;store in bl
    mov  al, 51h                        ;CR51 - Extended System Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 30h                        ;keep bits 5-4 - Logical Screen Width bits 9-8
    shr  ah, 04h                        ;move to 1-0
    mov  al, bl                         ;set bottom 8 bits of offset in al
    shl  ax, 01h                        ;multiply by 2
    mov  bx, ax                         ;store in bx
    pop  ax                             ;Start address bottom
    pop  dx                             ;start address top
    div  bx                             ;ax = dx:ax / bx, dx = remainder
    push ax                             ;store divison
    push dx                             ;store remainder
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 67h                        ;CR67 - Extended Miscellaneous Control 2 register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0f0h                       ;Keep top 4 bits - Color Mode
                                        ;0000 = Mode 0: 8-bit color, 1 pixel/VCLK
                                        ;0001 = Mode 8: 8-bit color, 2 pixels/VCLK
                                        ;0011 = Mode 9: 15-bit color, 1 pixel/VCLK
                                        ;0101 = Mode 10: 16-bit color, 1 pixel/VCLK
                                        ;0111 = Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Trio32 only)
                                        ;1101 = Mode 13: 24-bit color, 1 pixel/VCLK
    mov  cl, 02h                        ;multiplier shl 2
    cmp  ah, 10h                        ;Mode 8
    jbe  ModeIs8bpp                     ;Offset 0x526d
    mov  cl, 01h                        ;multiplier shl 1
    cmp  ah, 50h                        ;Mode 10
    jbe  MultiplierSet                  ;Offset 0x527c
    mov  cl, 00h                        ;multiplier shl 0
    cmp  ah, 70h                        ;Mode 12
    je   MultiplierSet                  ;Offset 0x527c
    mov  cl, 00h                        ;multiplier shl 0
    jmp  MultiplierSet                  ;Offset 0x527c
ModeIs8bpp:                             ;Offset 0x526d
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 04h                        ;SR4 - Memory Mode Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    test ah, 08h                        ;test bit 3 - Chain 4 Mode
    jne  MultiplierSet                  ;Offset 0x527c
    mov  cl, 03h                        ;multiplier shl 3
MultiplierSet:                          ;Offset 0x527c
    pop  ax                             ;restore remainder
    shl  ax, cl                         ;ax = ax << cl
    mov  cx, ax                         ;save result in cx
    pop  dx                             ;save division in dx
    mov  ax, 4fh                        ;0x00 = success
Exit:                                   ;Offset 0x5285
    pop  bx
    ret  
GetDisplayStart ENDP

VESADDCReadEDID PROC NEAR               ;Offset 0x5287
    push bx
    push cx
    call VESADDCInstallationCheck       ;Offset 0x5342 returns bx = 0x0102 if DDC2, 0x0105 if DDC1, 0x0000 if not detected
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 55h                        ;CR55 - Extended RAMDAC Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 04h                        ;bit 2 = 1 - Enable General Input Port Read
    out  dx, ax
    test bl, 02h                        ;test if DDC2
    jne  DDC2Found                      ;Offset 0x52ba
    test bl, 01h                        ;0x0105 test if DDC1
    je   Done                           ;Offset 0x52c0
    call TurnOffScreen                  ;Offset 0x47ed
    mov  bx, 0ff06h
    call DDC1FindHeader                 ;Offset 0x5386
    jne  DDC1HeaderFound                ;Offset 0x52b5
    mov  bx, 0007h
    call DDC1FindHeader                 ;Offset 0x5386
    je   Done                           ;Offset 0x52c0
DDC1HeaderFound:                        ;Offset 0x52b5
    call DDC1ReadData                   ;Offset 0x5452
    jmp  Done                           ;Offset 0x52c0
DDC2Found:                              ;Offset 0x52ba
    mov  bx, 0080h                      ;Read from word 00, flags 0x80, bit 0 = 0 - read response into es:di
    call DDCSendCommand                 ;Offset 0x5519
Done:                                   ;Offset 0x52c0
    mov  dx, SequenceIndex              ;port - 0x3c4
    mov  al, 0dh                        ;SRD - Extended Sequencer register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fh                        ;set VSYNC and HSYNC to 00 - Normal operation
    out  dx, ax
    call TurnOnScreen                   ;Offset 0x4800
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 55h                        ;CR55 - Extended RAMDAC Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fbh                       ;bit 2 = 0 - Disable General Input Port Read (RAMDAC reads enabled)
    out  dx, ax
    xor  bh, 01h                        ;Toggle lower bit
    pop  cx
    pop  bx
    ret  
VESADDCReadEDID ENDP

;I can't find a format specification for VDIF. If anyone knows where I can find one, please let me know!
VESADDCReadVDIF PROC NEAR               ;Offset 0x52e1
    push bx
    push cx
    push dx
    call VESADDCInstallationCheck       ;Offset 0x5342 returns bx = 0x0102 if DDC2, 0x0105 if DDC1, 0x0000 if not detected
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 55h                        ;CR55 - Extended RAMDAC Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    or   ah, 04h                        ;bit 2 = 1 - Enable General Input Port Read
    out  dx, ax
    pop  dx
    xor  bh, bh
    test bl, 02h                        ;Test if DDC
    je   Done                           ;Offset 0x5330
    and  dx, 0ffh                       ;This is confusing. and off the top byte
    shl  dl, 06h                        ;shift up so only 2 bits remain
    add  dl, 80h                        ;add 0x80 to toggle top bit?
    mov  bh, dl                         ;bh is start word
    mov  bl, 42h                        ;flags?
    push bx                             ;Store confusing value
    mov  bx, 8001h                      ;Send command, don't read result, word offset 0x80
    call DDCSendCommand                 ;Offset 0x5519
    call DDCReceiveByte                 ;Offset 0x5584
    cmp  bh, 56h
    jne  HeaderDataMismatch             ;Offset 0x532c
    call DDCSendAck                     ;Offset 0x5593
    call DDCReceiveByte                 ;Offset 0x5584
    cmp  bh, 44h
    jne  HeaderDataMismatch             ;Offset 0x532c
    call DDCSendEnd                     ;Offset 0x54b2
    pop  bx                             ;Restore confusing value
    call DDCSendCommand                 ;Offset 0x5519
    jmp  Done                           ;Offset 0x5330
HeaderDataMismatch:                     ;Offset 0x532c
    pop  bx
    call DDCSendEnd                     ;Offset 0x54b2
Done:                                   ;Offset 0x5330
    call GetCRTControllerIndexRegister  ;Offset 0xfdd
    mov  al, 55h                        ;CR55 - Extended RAMDAC Control register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    and  ah, 0fbh                       ;bit 2 = 0 - Disable General Input Port Read (RAMDAC reads enabled)
    out  dx, ax
    xor  bh, 01h
    pop  cx
    pop  bx
    ret  
VESADDCReadVDIF ENDP

;outputs:
;bx = 
;     0x0102 = DDC2
;     0x0105 = DDC1
;     0x0000 = Not found
VESADDCInstallationCheck PROC NEAR      ;Offset 0x5342
    call      GetCRTControllerIndexRegister;Offset 0xfdd
    mov       al, 55h                   ;CR55 - Extended RAMDAC Control register
    call      ReadDataWithIndexRegister ;Offset 0x4640
    or        ah, 04h                   ;bit 2 = 1 - Enable General Input Port Read
    out       dx, ax
    mov       bx, 0001h                 ;word offset 0x00, flag bit 0 = don't read response.
    call      DDCSendCommand            ;Offset 0x5519
    call      DDCReceiveByte            ;Offset 0x5584
    cmp       bh, 00h                   ;Expect 0x00 response, fixed header pattern is 00 FF FF FF FF FF FF 00
    jne       HeaderDataMismatch        ;Offset 0x536f
    call      DDCSendAck                ;Offset 0x5593
    call      DDCReceiveByte            ;Offset 0x5584
    cmp       bh, 0ffh
    jne       HeaderDataMismatch        ;Offset 0x536f
    call      DDCSendEnd                ;Offset 0x54b2
    mov       bx, 0102h
    jmp       Done                      ;Offset 0x537a
HeaderDataMismatch:                     ;Offset 0x536f
    mov       bx, 0000h
    call      TestIfDDC1                ;Offset 0x5480
    je        Done                      ;Offset 0x537a - if no bit change was detected, there's nothing sending data
    mov       bx, 0105h
Done:                                   ;Offset 0x537a
    mov       al, 55h                   ;CR55 - Extended RAMDAC Control register
    call      ReadDataWithIndexRegister ;Offset 0x4640
    and       ah, 0fbh                  ;bit 2 = 0 - Disable General Input Port Read (RAMDAC reads enabled)
    out       dx, ax
    xor       dx, dx                    ;This resets the flags...
    ret       
VESADDCInstallationCheck ENDP

;inputs:
;bh = expected value
;bl = count of FF bytes
DDC1FindHeader PROC NEAR                ;Offset 0x5386
    mov       dx, DACWriteIndex         ;port - 0x3c8
    call      DDC1ReadByte              ;Offset 0x5421
    mov       cx, 0480h                 ;Loop for 1152 bits (144 bytes)
FindByte:                               ;Offset 0x538f
    DB 03Ah, 0E7h                       ;cmp       ah, bh - masm encoding difference
    je        Found                     ;Offset 0x53a4
    shl       ah, 01h                   ;shift out upper bit
    call      SendVSyncPulse            ;Offset 0x5439
    in        al, dx                    ;Read a bit
    and       al, 01h                   ;mask off top 7 bits
    or        ah, al                    ;set lowest bit
    loop      FindByte                  ;Offset 0x538f
    mov       bh, 01h                   ;set bh to 0x1
    jmp       NotFound                  ;Offset 0x53e4 - byte wasn't found
    nop
Found:                                  ;Offset 0x53a4
    push      bx
FindFFByte:                             ;Offset 0x53a5
    dec       bl
    je        FoundFFBytes              ;Offset 0x53b7
    call      SendVSyncPulse            ;Offset 0x5439
    call      DDC1ReadByte              ;Offset 0x5421
    cmp       ah, 0ffh
    je        FindFFByte                ;Offset 0x53a5
    pop       bx
    jmp       FindByte                  ;Offset 0x538f
FoundFFBytes:                           ;Offset 0x53b7
    pop       bx
    call      SendVSyncPulse            ;Offset 0x5439
    call      DDC1ReadByte              ;Offset 0x5421
    cmp       ah, 00h
    jne       FindByte                  ;Offset 0x538f
    cmp       bh, 00h
    je        FoundHeader               ;Offset 0x53e2
    push      cx
    mov       cx, 0438h                 ;0x438 = 1080 bits = 135 bytes
SkipBits:                               ;Offset 0x53cc
    call      SendVSyncPulse            ;Offset 0x5439
    loop      SkipBits                  ;Offset 0x53cc
    pop       cx
    call      DDC1ReadByte              ;Offset 0x5421
    cmp       ah, 00h                   ;
    jne       FindByte                  ;Offset 0x538f didn't find the byte we were looking for
    mov       cx, 0040h                 ;skip 0x40 = 64 bits = 8 bytes
SkipMoreBits:                           ;Offset 0x53dd
    call      SendVSyncPulse            ;Offset 0x5439
    loop      SkipMoreBits              ;Offset 0x53dd
FoundHeader:                            ;Offset 0x53e2
    mov       bh, 00h
NotFound:                               ;Offset 0x53e4
    xor       bh, 01h                   ;xor-ing 0x1 = 0, 0x0 = 1
    ret
DDC1FindHeader ENDP

WaitForVSync PROC NEAR                  ;Offset 0x53e8
    push      dx
    push      cx
    push      ax
    mov       dx, InputStatus1D         ;port - 0x3da
    mov       cx, 0ffffh
Label0x53f1:                            ;Offset 0x53f1
    in        al, dx
    and       al, 08h                   ;bit 3 = 0 - Vertical Sync is in the Display mode
    jcxz      Fail                      ;Offset 0x53fd
    loope     Label0x53f1               ;Offset 0x53f1
Label0x53f8:                            ;Offset 0x53f8
    in        al, dx
    and       al, 08h                   ;bit 3 = 1 - Vertical Sync is in the vertical retrace mode
    loopne    Label0x53f8               ;Offset 0x53f8
Fail:                                   ;Offset 0x53fd
    pop       ax
    pop       cx
    pop       dx
    ret
WaitForVSync ENDP

;inputs:
;bh = 0x01 - VSYNC = 1, 0x00 = VSYNC = 0
SetVSYNC PROC NEAR                    ;Offset 0x5401
    push      dx
    push      ax
    mov       dx, SequenceIndex         ;port - 0x3c4
    mov       al, 0dh                   ;SRD - Extended Sequencer register
    call      ReadDataWithIndexRegister ;Offset 0x4640
    and       ah, 3fh                   ;drop top 2 bits.
    cmp       bh, 01h                   ;
    jne       Vsync0                    ;Offset 0x541a
    or        ah, 80h                   ;bits 7-6 = 10 - VSYNC = 1
    out       dx, ax
    jmp       Done                      ;Offset 0x541e
    nop
Vsync0:                                 ;Offset 0x541a
    or        ah, 40h                   ;bits 7-6 = 01 - VSYNC = 0
    out       dx, ax
Done:                                   ;Offset 0x541e
    pop       ax
    pop       dx
    ret
SetVSYNC ENDP

;inputs:
;dx = RAMDAC port
;outputs:
;ah = byte read
DDC1ReadByte PROC NEAR                  ;Offset 0x5421
    push      cx
    xor       ah, ah
    mov       cl, 08h
ReadBit:                                ;Offset 0x5426
    dec       cl
    call      SendVSyncPulse            ;Offset 0x5439
    in        al, dx
    and       al, 01h                   ;mask off top 7 bits
    shl       al, cl                    ;shift by which bit we're reading - 1
    or        ah, al                    ;add to result
    cmp       cl, 00h
    jne       ReadBit                   ;Offset 0x5426
    pop       cx
    ret       
DDC1ReadByte ENDP

SendVSyncPulse PROC NEAR                ;Offset 0x5439
    push      cx
    push      bx
    mov       bh, 01h
    call      SetVSYNC                  ;Offset 0x5401
    mov       cx, 0113h
Wait1:                                  ;Offset 0x5443 - time sensitive loop!
    loop      Wait1                     ;Offset 0x5443
    mov       bh, 00h
    call      SetVSYNC                  ;Offset 0x5401
    mov       cx, 0113h
Wait2:                                  ;Offset 0x544d - time sensitive loop!
    loop      Wait2                     ;Offset 0x544d
    pop       bx
    pop       cx
    ret       
SendVSyncPulse ENDP

;inputs:
;es:di points to output buffer
;outputs:
;bh = 1 - success, 0 - failure
DDC1ReadData PROC NEAR                    ;Offset 0x5452
    pushf
    cld
    push      di
    xor       al, al
    stosb                               ;Recreate 00 FF FF FF FF FF FF 00 header pattern
    not       al
    mov       cx, 06h
    rep stosb
    xor       al, al
    stosb
    mov       bh, 0fah                  ;0xfa = 250
    mov       cx, 078h                  ;0x78 = 120
ReadBytes:                              ;Offset 0x5467
    call      SendVSyncPulse            ;Offset 0x5439
    call      DDC1ReadByte              ;Offset 0x5421
    mov       al, ah
    add       bh, ah
    stosb
    loop      ReadBytes                 ;Offset 0x5467
    pop       di
    popf      
    cmp       bh, 00h
    mov       bh, 00h
    jne       Done                      ;Offset 0x547f
    inc       bh
Done:                                   ;Offset 0x547f
    ret       
DDC1ReadData ENDP

;Tests to see if there's continuous data being sent
TestIfDDC1 PROC NEAR                    ;Offset 0x5480
    push      dx
    mov       dx, DACWriteIndex         ;port - 0x3c8
    call      WaitForVSync              ;Offset 0x53e8
    in        al, dx                    ;read general io port
    and       al, 01h                   ;keep the lowest bit (data)
    mov       ah, al                    ;store in ah
    mov       cx, 32h                   ;for 50 frames, loop
ReadLoop:                               ;Offset 0x548f
    call      WaitForVSync              ;Offset 0x53e8
    in        al, dx
    and       al, 01h                   ;read bit again
    DB 03Ah, 0C4h                       ;cmp       al, ah - masm encoding difference
    jne       Done                      ;Offset 0x549b if they are different, we're done
    loop      ReadLoop                  ;Offset 0x548f
Done:                                   ;Offset 0x549b
    pop       dx
    ret
TestIfDDC1 ENDP

DDCSendBegin PROC NEAR                  ;Offset 0x549d
    mov       bl, 01h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 03h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 02h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 00h
    call      DDCSendData               ;Offset 0x54cc
    ret
DDCSendBegin ENDP

DDCSendEnd PROC NEAR                    ;Offset 0x54b2
    mov       bl, 00h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 02h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 03h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 01h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 03h
    call      DDCSendData               ;Offset 0x54cc
    ret
DDCSendEnd ENDP

DDCSendData PROC NEAR                   ;Offset 0x54cc
    push      dx
    push      cx
    push      ax
    call      GetCRTControllerIndexRegister;Offset 0xfdd
    mov       al, 5ch                   ;CR5C - General Output Port register
    call      ReadDataWithIndexRegister ;Offset 0x4640
    and       ah, 0fch                  ;And out lowest two bits.
    or        ah, bl                    ;send lowest two bits.
    out       dx, ax
    mov       cx, 0113h
WaitLoop:                               ;Offset 0x54e0 - This feels like a time sensitive loop!
    loop      WaitLoop                  ;Offset 0x54e0
    pop       ax
    pop       cx
    pop       dx
    ret
DDCSendData ENDP

DDCReceiveBit PROC NEAR                 ;Offset 0x54e6
    push      dx
    push      ax
    mov       bl, 01h
    call      DDCSendData               ;Offset 0x54cc
    mov       bl, 03h
    call      DDCSendData               ;Offset 0x54cc
    mov       dx, DACWriteIndex         ;port - 0x3c8 - DACWriteIndex, but now it behaves as General IO Port 
    in        al, dx
    and       al, 01h                   ;mask off everything but the bottom bit
    or        bh, al                    ;or into bh
    mov       bl, 01h
    call      DDCSendData               ;Offset 0x54cc
    pop       ax
    pop       dx
    ret
DDCReceiveBit ENDP

DDCSendBit PROC NEAR                    ;Offset 0x5502
    xor       bl, bl
    or        bl, bh
    and       bl, 01h                   ;Send 2 bits: 0x maybe the top bit is a clock signal?
    call      DDCSendData               ;Offset 0x54cc
    or        bl, 02h                   ;Send 2 bits: 1x
    call      DDCSendData               ;Offset 0x54cc
    and       bl, 01h                   ;Send 2 bits: 0x
    call      DDCSendData               ;Offset 0x54cc
    ret
DDCSendBit ENDP

;inputs:
;bl = flags. bit 0 = 1 - read response into es:di, 0 = exit after send command
;            bit 1 = 1 - perform accumulation test (total must be 0), 0 = ignore
;            bits 7-2 = number of bytes to read * 4
;bh = word offset
;returns:
;bh = 0 - fail
;bh = 1 - success
DDCSendCommand PROC NEAR                ;Offset 0x5519
    mov       cx, bx                    ;store word offset and flags
    call      DDCSendBegin              ;Offset 0x549d
    mov       bh, 0a0h                  ;Send device ID A0h - write operation
    call      DDCSendByte               ;Offset 0x5570
    mov       bx, cx                    ;Send upper byte of bx - word offset
    call      DDCSendByte               ;Offset 0x5570
    call      DDCSendBegin              ;Offset 0x549d
    mov       bh, 0a1h                  ;Send device ID A1h - read operation
    call      DDCSendByte               ;Offset 0x5570
    test      cx, 01h                   ;if lower bit set, don't read response.
    jne       Exit                      ;Offset 0x556f
    push      cx                        ;store word offset and flags
    push      di                        ;store destination index
    pushf                               ;store flags
    cld                                 ;clear direction flag
    and       cx, 0fch                  ;And off word offset and bottom two flags
    dec       cx                        ;number of bytes to read minus one (since that's read after the loop)
    xor       ah, ah
ReceiveBytes:                           ;Offset 0x5541
    call      DDCReceiveByte            ;Offset 0x5584
    call      DDCSendAck                ;Offset 0x5593
    mov       al, bh
    stosb
    add       ah, bh
    loop      ReceiveBytes              ;Offset 0x5541
    call      DDCReceiveByte            ;Offset 0x5584
    mov       al, bh
    stosb
    add       bh, ah
    call      DDCSendEnd                ;Offset 0x54b2
    popf                                ;restore direction flag
    pop       di                        ;restore index
    pop       cx                        ;
    test      cx, 0002h                 ;test bit 1
    je        Label0x5566               ;Offset 0x5566
    mov       bh, 01h
    jmp       Exit                      ;Offset 0x556f
Label0x5566:                            ;Offset 0x5566
    cmp       bh, 00h
    mov       bh, 00h
    jne       Exit                      ;Offset 0x556f
    inc       bh
Exit:                                   ;Offset 0x556f
    ret
DDCSendCommand ENDP

;inputs:
;bh = byte to send
;outputs:
;none
DDCSendByte PROC NEAR                   ;Offset 0x5570
    push      cx
    mov       cx, 08h
SendBit:                                ;Offset 0x5574
    rol       bh, 01h
    call      DDCSendBit                ;Offset 0x5502
    loop      SendBit                   ;Offset 0x5574
    push      bx
    mov       bh, 01h                   ;Stop bit?
    call      DDCSendBit                ;Offset 0x5502
    pop       bx
    pop       cx
    ret
DDCSendByte ENDP

;inputs:
;none
;outputs:
;bh = read byte
DDCReceiveByte PROC NEAR                ;Offset 0x5584
    push      cx
    xor       bh, bh
    mov       cx, 08h
ReceiveBit:                             ;Offset 0x558a
    rol       bh, 01h
    call      DDCReceiveBit             ;Offset 0x54e6
    loop      ReceiveBit                ;Offset 0x558a
    pop       cx
    ret
DDCReceiveByte ENDP

DDCSendAck PROC NEAR                    ;Offset 0x5593
    push      bx
    xor       bh, bh
    call      DDCSendBit                ;Offset 0x5502
    pop       bx
    ret
DDCSendAck ENDP

;AX = 4F09h
;BL = subfunction
;00h set (primary) palette
;01h get (primary) palette
;02h set secondary palette data
;03h get secondary palette data
;80h set palette during vertical retrace
;CX = number of entries to change
;DX = starting palette index
;ES:DI -> palette buffer, array of DAC entries (see #00086)

;Return:
;AL = 4Fh if function supported
;AH = status
;00h successful
;01h failed
;It's clear that this function does not do what it advertises
GetSetDACPaletteControl PROC NEAR       ;Offset 0x559b
    mov  byte ptr ss:[bp + 11h], 01h    ;Failure
    cmp  bl, 01h                        ;
    ja   Failure                        ;Offset 0x55b7
    mov  byte ptr ss:[bp + 0fh], 06h
    or   bh, bh                         ;function 0x00 - set primary palette
    jne  GoDoSomething                  ;Offset 0x55b5
    mov  byte ptr ss:[bp + 0fh], 06h
Success:                                ;Offset 0x55b0
    mov  byte ptr ss:[bp + 11h], 00h    ;Success
    ret
GoDoSomething:                          ;Offset 0x55b5
    jmp  Success                        ;Offset 0x55b0 - We did it!
Failure:                                ;Offset 0x55b7
    ret
GetSetDACPaletteControl ENDP

VESAPowerManagement PROC NEAR           ;Offset 0x55b8
    mov  dx, SequenceIndex              ;port - 0x3c4
    in   al, dx                         ;Save state
    push ax
    mov  al, 08h                        ;SR8 - Unlock Extended Sequencer register
    call ReadDataWithIndexRegister      ;Offset 0x4640
    push ax                             ;Save state
    mov  ah, 06h                        ;Unlock Extended Sequencer registers
    out  dx, ax
    mov  byte ptr ss:[bp + 11h], 01h    ;failure return value
    cmp  bl, 02h                        ;Highest subfunction
    ja   Exit                           ;Offset 0x55fa
    cmp  bl, 00h
    je   GetPMCapabilities              ;Offset 0x55f1
    cmp  bl, 01h
    je   SetDisplayPowerState           ;Offset 0x55e9
    cmp  bl, 02h
    je   GetDisplayPowerState           ;Offset 0x55de - Unnecessary jump
GetDisplayPowerState:                   ;Offset 0x55de
    call VESAGetDisplayPowerState       ;Offset 0x4fc2
    mov  byte ptr ss:[bp + 0fh], bh
    je   Success                        ;Offset 0x55f6
    jmp  Exit                           ;Offset 0x55fa
    nop
SetDisplayPowerState:                   ;Offset 0x55e9
    call VESASetDisplayPowerState       ;Offset 0x4f72
    je   Success                        ;Offset 0x55f6
    jmp  Exit                           ;Offset 0x55fa
    nop
GetPMCapabilities:                      ;Offset 0x55f1
    mov  word ptr ss:[bp + 0eh], 0710h  ;bits 3-0 Minor Version = 0x00
                                        ;bits 7-4 Major Version = 0x01
                                        ;bit 8 = 1 - Standby supported
                                        ;bit 9 = 1 - Suspend supported
                                        ;bit 10 = 1 - Off supported
                                        ;bit 11 = 0 - Reduced on not supported
Success:                                ;Offset 0x55f6
    mov  byte ptr ss:[bp + 11h], 00h    ;success return value
Exit:                                   ;Offset 0x55fa
    pop  ax
    out  dx, ax
    pop  ax
    out  dx, al
    ret  
VESAPowerManagement ENDP

VESADisplayDataChannel PROC NEAR        ;Offset 0x55ff
    mov       byte ptr ss:[bp + 11h], 01h;ah return value 0x01 = failed
    cmp       bl, 02h                   ;0x00 - 0x02 are valid values.
    ja        Exit                      ;Offset 0x5636
    cmp       bl, 00h
    je        InstallationCheck         ;Offset 0x5617
    cmp       bl, 01h
    je        ReadEDID                  ;Offset 0x5622
    cmp       bl, 02h
    je        ReadVDIF                  ;Offset 0x562a
InstallationCheck:                      ;Offset 0x5617
    call      VESADDCInstallationCheck  ;Offset 0x5342
    mov       word ptr ss:[bp + 0eh], bx
    je        Success                   ;Offset 0x5632
    jmp       Exit                      ;Offset 0x5636
    nop
ReadEDID:                               ;Offset 0x5622
    call      VESADDCReadEDID           ;Offset 0x5287
    je        Success                   ;Offset 0x5632
    jmp       Exit                      ;Offset 0x5636
    nop
ReadVDIF:                               ;Offset 0x562a
    call      VESADDCReadVDIF           ;Offset 0x52e1
    je        Success                   ;Offset 0x5632
    jmp       Exit                      ;Offset 0x5636
    nop
Success:                                ;Offset 0x5632
    mov       byte ptr [bp + 11h], 00h  ;ah return value 0x00 = success
Exit:                                   ;Offset 0x5636
    ret
VESADisplayDataChannel ENDP

;inputs:
;es:di points to 128 byte buffer
;
;outputs:
;ah = status 0 - success, 1 - failure
;es:di buffer filled with info:
;
;Info comes from: http://www.ctyme.com/intr/rb-0273.htm
;
;Offset  Size    Description     (Table 00077)
;00h  4 BYTEs   (ret) signature ("VESA")
;(call) VESA 2.0 request signature ("VBE2"), required to receive
;version 2.0 info
;04h    WORD    VESA version number (one-digit minor version -- 0102h = v1.2)
;06h    DWORD   pointer to OEM name
;"761295520" for ATI
;0Ah    DWORD   capabilities flags (see #00078)
;0Eh    DWORD   pointer to list of supported VESA and OEM video modes
;(list of words terminated with FFFFh)
;12h    WORD    total amount of video memory in 64K blocks
;---VBE v1.x ---
;14h 236 BYTEs  reserved

GetSuperVGAInfo PROC NEAR               ;Offset 0x5637
    mov       byte ptr ss:[bp + 11h], 00h;return value - success
    xor       ax, ax
    push      di
    mov       cx, 0080h                 ;clear 128 bytes
    rep stosw
    pop       di
    ;mov       dword ptr es:[di], 41534556h; "VESA" - masm encoding difference
    DB 026h, 066h, 0C7h, 005h, 056h, 045h, 053h, 041h;0x00 = "VESA"
    add       di, 04h
    mov       ax, 0102h                 ;0x04 = VESA version number. 1.2 in this case.
    stosw
    mov       ax, offset OEMName        ;Offset 0x4d10
    stosw                               ;0x06 = OEM name
    mov       ax, cs                    ;0x08 = segment for OEM name FAR pointer
    stosw                               ;
    add       di, 04h                   ;Skip capabilities flags
    lea       ax, ds:[VESAVideoModeIds] ;Offset 0x4d28
    stosw                               ;0x0E = Pointer to list of supported VESA and OEM video modes
    mov       ax, cs                    ;0x10 = segment for VESA mode list
    stosw
    call      GetInstalledMemorySizeIn4KBlocks;Offset 0x1457
    mov       ax, 08h                   ;8 * 64KiB = 0.5MiB
    cmp       dh, 00h
    je        Done                      ;Offset 0x5688
    shl       ax, 01h                   ;16 * 64KiB = 1MiB
    cmp       dh, 01h
    je        Done                      ;Offset 0x5688
    shl       ax, 01h                   ;32 * 64KiB = 2MiB
    cmp       dh, 02h
    je        Done                      ;Offset 0x5688
    shl       ax, 01h                   ;64 * 64KiB = 4MiB
    cmp       dh, 04h
    je        Done                      ;Offset 0x5688
    mov       ax, 10h                   ;Default to 1MiB if unknown
Done:                                   ;Offset 0x5688
    mov       word ptr es:[di], ax      ;0x12 = total memory in 64k blocks
    ret
GetSuperVGAInfo ENDP

IntHandlerCallTable     DW SetVideoMode ;Offset 0x1819              ;00h
                        DW SetTextModeCursorShape;Offset 0x1da3     ;01h
                        DW SetCursorPosition;Offset 0x1e4e          ;02h
                        DW GetCursorPositionAndSize;Offset 0x1e81   ;03h
                        DW Function4Forward;Offset 0x1e96           ;04h
                        DW SelectActiveDisplayPage;Offset 0x1eb2    ;05h
                        DW ScrollUpWindow;Offset 0x1ef1             ;06h
                        DW ScrollDownWindow;Offset 0x1f40           ;07h
                        DW ReadCharAndAttrAtCursorPos;Offset 0x23f0 ;08h
                        DW WriteCharAndAttrAtCursorPos;Offset 0x2670;09h
                        DW WriteCharOnlyAtCursorPos;Offset 0x26ff   ;0Ah
                        DW ColorFunctions;Offset 0x2966             ;0Bh
                        DW WriteGraphicsPixel;Offset 0x2aaa         ;0Ch
                        DW ReadGraphicsPixel;Offset 0x2b6a          ;0Dh
                        DW TeletypeOutput;Offset 0x2c35             ;0Eh
                        DW GetCurrentVideoMode;Offset 0x2cf9        ;0Fh
                        DW DACFunctions ;Offset 0x2d4a              ;10h
                        DW TextModeCharFunctions;Offset 0x2f6b      ;11h
                        DW AlternateFunctions;Offset 0x31bd         ;12h
                        DW WriteString  ;Offset 0x33d9              ;13h
                        DW EmptyFunction4;Offset 0x5718             ;14h
                        DW EmptyFunction4;Offset 0x5718             ;15h
                        DW EmptyFunction4;Offset 0x5718             ;16h
                        DW EmptyFunction4;Offset 0x5718             ;17h
                        DW EmptyFunction4;Offset 0x5718             ;18h
                        DW EmptyFunction4;Offset 0x5718             ;19h
                        DW DisplayCombinationCodeFuncs;Offset 0x34e2;1Ah
                        DW FunctionalityAndStateInfo;Offset 0x35ad  ;1Bh
                        DW SaveRestoreVideoState;Offset 0x370d      ;1Ch

InterruptHandlerEntrypoint PROC FAR     ;Offset 0x56c6
    sti
    cld
    cmp  ah, INT10_0E_TeletypeOutput    ;function 0xe
    je   GotoTeletypeHandler            ;Offset 0x5703
    cmp  ah, INT10_0C_WriteGraphicsPixel;function 0xc
    je   GotoWriteGraphicsPixelHandler  ;Offset 0x5706
    cmp  ah, INT10_0D_ReadGraphicsPixel ;function 0xd
    je   GotoReadGraphicsPixelHandler   ;Offset 0x5709
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push bp
    push es
    push ds
    mov  bp, sp
    mov  ds, word ptr cs:[Data1488]     ;Offset 0x1488
    mov  si, ax
    mov  al, ah
    xor  ah, ah
    cmp  al, 1dh                        ;0x1c is last supported function, except vesa
    jae  GotoVESAHandler                ;Offset 0x570c
    shl  ax, 01h
    xchg ax, si
    call word ptr cs:[si + offset IntHandlerCallTable];Offset 0x568c
Exit:                                   ;Offset 0x56f9
    pop  ds
    pop  es
    pop  bp
    pop  di
    pop  si
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    iret
GotoTeletypeHandler:                    ;Offset 0x5703
    jmp  TeletypeOutput                 ;Offset 0x2c35
GotoWriteGraphicsPixelHandler:          ;Offset 0x5706
    jmp  WriteGraphicsPixel             ;Offset 0x2aaa
GotoReadGraphicsPixelHandler:           ;Offset 0x5709
    jmp  ReadGraphicsPixel              ;Offset 0x2b6a
GotoVESAHandler:                        ;Offset 0x570c
    mov  ax, si
    cmp  ah, INT10_4F_VESASuperVGABIOSFunctions;VESA functions, 0x4f
    jne  Exit                           ;Offset 0x56f9
    call VESAHandler                    ;Offset 0x4b01
    jmp  Exit                           ;Offset 0x56f9
InterruptHandlerEntrypoint ENDP

EmptyFunction4 PROC NEAR                ;Offset 0x5718
    ret
EmptyFunction4 ENDP

ORG 5720h
LowerCharacters8x8      DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 081h, 0A5h, 081h, 0BDh, 099h, 081h, 07Eh;Offset 0x5720
                        DB 07Eh, 0FFh, 0DBh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 06Ch, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h
                        DB 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 038h, 07Ch, 038h, 0FEh, 0FEh, 092h, 010h, 07Ch
                        DB 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 07Ch, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h
                        DB 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h
                        DB 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 00Fh, 007h, 00Fh, 07Dh, 0CCh, 0CCh, 0CCh, 078h
                        DB 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 03Fh, 033h, 03Fh, 030h, 030h, 070h, 0F0h, 0E0h
                        DB 07Fh, 063h, 07Fh, 063h, 063h, 067h, 0E6h, 0C0h, 099h, 05Ah, 03Ch, 0E7h, 0E7h, 03Ch, 05Ah, 099h
                        DB 080h, 0E0h, 0F8h, 0FEh, 0F8h, 0E0h, 080h, 000h, 002h, 00Eh, 03Eh, 0FEh, 03Eh, 00Eh, 002h, 000h
                        DB 018h, 03Ch, 07Eh, 018h, 018h, 07Eh, 03Ch, 018h, 066h, 066h, 066h, 066h, 066h, 000h, 066h, 000h
                        DB 07Fh, 0DBh, 0DBh, 07Bh, 01Bh, 01Bh, 01Bh, 000h, 03Eh, 063h, 038h, 06Ch, 06Ch, 038h, 086h, 0FCh
                        DB 000h, 000h, 000h, 000h, 07Eh, 07Eh, 07Eh, 000h, 018h, 03Ch, 07Eh, 018h, 07Eh, 03Ch, 018h, 0FFh
                        DB 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h
                        DB 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 030h, 060h, 0FEh, 060h, 030h, 000h, 000h
                        DB 000h, 000h, 0C0h, 0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h
                        DB 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 000h, 000h, 000h, 0FFh, 0FFh, 07Eh, 03Ch, 018h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 018h, 000h, 018h, 000h
                        DB 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 06Ch, 06Ch, 0FEh, 06Ch, 0FEh, 06Ch, 06Ch, 000h
                        DB 018h, 07Eh, 0C0h, 07Ch, 006h, 0FCh, 018h, 000h, 000h, 0C6h, 0CCh, 018h, 030h, 066h, 0C6h, 000h
                        DB 038h, 06Ch, 038h, 076h, 0DCh, 0CCh, 076h, 000h, 030h, 030h, 060h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 030h, 060h, 060h, 060h, 030h, 018h, 000h, 060h, 030h, 018h, 018h, 018h, 030h, 060h, 000h
                        DB 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h
                        DB 07Ch, 0CEh, 0DEh, 0F6h, 0E6h, 0C6h, 07Ch, 000h, 030h, 070h, 030h, 030h, 030h, 030h, 0FCh, 000h
                        DB 078h, 0CCh, 00Ch, 038h, 060h, 0CCh, 0FCh, 000h, 078h, 0CCh, 00Ch, 038h, 00Ch, 0CCh, 078h, 000h
                        DB 01Ch, 03Ch, 06Ch, 0CCh, 0FEh, 00Ch, 01Eh, 000h, 0FCh, 0C0h, 0F8h, 00Ch, 00Ch, 0CCh, 078h, 000h
                        DB 038h, 060h, 0C0h, 0F8h, 0CCh, 0CCh, 078h, 000h, 0FCh, 0CCh, 00Ch, 018h, 030h, 030h, 030h, 000h
                        DB 078h, 0CCh, 0CCh, 078h, 0CCh, 0CCh, 078h, 000h, 078h, 0CCh, 0CCh, 07Ch, 00Ch, 018h, 070h, 000h
                        DB 000h, 018h, 018h, 000h, 000h, 018h, 018h, 000h, 000h, 018h, 018h, 000h, 000h, 018h, 018h, 030h
                        DB 018h, 030h, 060h, 0C0h, 060h, 030h, 018h, 000h, 000h, 000h, 07Eh, 000h, 07Eh, 000h, 000h, 000h
                        DB 060h, 030h, 018h, 00Ch, 018h, 030h, 060h, 000h, 03Ch, 066h, 00Ch, 018h, 018h, 000h, 018h, 000h
                        DB 07Ch, 0C6h, 0DEh, 0DEh, 0DCh, 0C0h, 07Ch, 000h, 030h, 078h, 0CCh, 0CCh, 0FCh, 0CCh, 0CCh, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 066h, 066h, 0FCh, 000h, 03Ch, 066h, 0C0h, 0C0h, 0C0h, 066h, 03Ch, 000h
                        DB 0F8h, 06Ch, 066h, 066h, 066h, 06Ch, 0F8h, 000h, 0FEh, 062h, 068h, 078h, 068h, 062h, 0FEh, 000h
                        DB 0FEh, 062h, 068h, 078h, 068h, 060h, 0F0h, 000h, 03Ch, 066h, 0C0h, 0C0h, 0CEh, 066h, 03Ah, 000h
                        DB 0CCh, 0CCh, 0CCh, 0FCh, 0CCh, 0CCh, 0CCh, 000h, 078h, 030h, 030h, 030h, 030h, 030h, 078h, 000h
                        DB 01Eh, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 000h, 0E6h, 066h, 06Ch, 078h, 06Ch, 066h, 0E6h, 000h
                        DB 0F0h, 060h, 060h, 060h, 062h, 066h, 0FEh, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h, 0C6h, 000h
                        DB 0C6h, 0E6h, 0F6h, 0DEh, 0CEh, 0C6h, 0C6h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0D6h, 07Ch, 00Eh, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 06Ch, 066h, 0E6h, 000h, 07Ch, 0C6h, 0E0h, 078h, 00Eh, 0C6h, 07Ch, 000h
                        DB 0FCh, 0B4h, 030h, 030h, 030h, 030h, 078h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0FCh, 000h
                        DB 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 030h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0FEh, 06Ch, 000h
                        DB 0C6h, 0C6h, 06Ch, 038h, 06Ch, 0C6h, 0C6h, 000h, 0CCh, 0CCh, 0CCh, 078h, 030h, 030h, 078h, 000h
                        DB 0FEh, 0C6h, 08Ch, 018h, 032h, 066h, 0FEh, 000h, 078h, 060h, 060h, 060h, 060h, 060h, 078h, 000h
                        DB 0C0h, 060h, 030h, 018h, 00Ch, 006h, 002h, 000h, 078h, 018h, 018h, 018h, 018h, 018h, 078h, 000h
                        DB 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
                        DB 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h
                        DB 0E0h, 060h, 060h, 07Ch, 066h, 066h, 0DCh, 000h, 000h, 000h, 078h, 0CCh, 0C0h, 0CCh, 078h, 000h
                        DB 01Ch, 00Ch, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h
                        DB 038h, 06Ch, 064h, 0F0h, 060h, 060h, 0F0h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h
                        DB 0E0h, 060h, 06Ch, 076h, 066h, 066h, 0E6h, 000h, 030h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 00Ch, 000h, 01Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 0E0h, 060h, 066h, 06Ch, 078h, 06Ch, 0E6h, 000h
                        DB 070h, 030h, 030h, 030h, 030h, 030h, 078h, 000h, 000h, 000h, 0CCh, 0FEh, 0FEh, 0D6h, 0D6h, 000h
                        DB 000h, 000h, 0B8h, 0CCh, 0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 078h, 0CCh, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 000h, 0DCh, 066h, 066h, 07Ch, 060h, 0F0h, 000h, 000h, 076h, 0CCh, 0CCh, 07Ch, 00Ch, 01Eh
                        DB 000h, 000h, 0DCh, 076h, 062h, 060h, 0F0h, 000h, 000h, 000h, 07Ch, 0C0h, 070h, 01Ch, 0F8h, 000h
                        DB 010h, 030h, 0FCh, 030h, 030h, 034h, 018h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 0CCh, 078h, 030h, 000h, 000h, 000h, 0C6h, 0C6h, 0D6h, 0FEh, 06Ch, 000h
                        DB 000h, 000h, 0C6h, 06Ch, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h
                        DB 000h, 000h, 0FCh, 098h, 030h, 064h, 0FCh, 000h, 01Ch, 030h, 030h, 0E0h, 030h, 030h, 01Ch, 000h
                        DB 018h, 018h, 018h, 000h, 018h, 018h, 018h, 000h, 0E0h, 030h, 030h, 01Ch, 030h, 030h, 0E0h, 000h
                        DB 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 000h

UpperCharacters8x8      DB 07Ch, 0C6h, 0C0h, 0C6h, 07Ch, 00Ch, 006h, 07Ch, 000h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 076h, 000h;Offset 0x5b20
                        DB 01Ch, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h, 07Eh, 081h, 03Ch, 006h, 03Eh, 066h, 03Bh, 000h
                        DB 0CCh, 000h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h, 0E0h, 000h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h
                        DB 030h, 030h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h, 000h, 000h, 07Ch, 0C6h, 0C0h, 078h, 00Ch, 038h
                        DB 07Eh, 081h, 03Ch, 066h, 07Eh, 060h, 03Ch, 000h, 0CCh, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h
                        DB 0E0h, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h, 0CCh, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 07Ch, 082h, 038h, 018h, 018h, 018h, 03Ch, 000h, 0E0h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 0C6h, 010h, 07Ch, 0C6h, 0FEh, 0C6h, 0C6h, 000h, 030h, 030h, 000h, 078h, 0CCh, 0FCh, 0CCh, 000h
                        DB 01Ch, 000h, 0FCh, 060h, 078h, 060h, 0FCh, 000h, 000h, 000h, 07Fh, 00Ch, 07Fh, 0CCh, 07Fh, 000h
                        DB 03Eh, 06Ch, 0CCh, 0FEh, 0CCh, 0CCh, 0CEh, 000h, 078h, 084h, 000h, 078h, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 0CCh, 000h, 078h, 0CCh, 0CCh, 078h, 000h, 000h, 0E0h, 000h, 078h, 0CCh, 0CCh, 078h, 000h
                        DB 078h, 084h, 000h, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 0E0h, 000h, 0CCh, 0CCh, 0CCh, 076h, 000h
                        DB 000h, 0CCh, 000h, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h, 0C3h, 018h, 03Ch, 066h, 066h, 03Ch, 018h, 000h
                        DB 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 000h, 018h, 018h, 07Eh, 0C0h, 0C0h, 07Eh, 018h, 018h
                        DB 038h, 06Ch, 064h, 0F0h, 060h, 0E6h, 0FCh, 000h, 0CCh, 0CCh, 078h, 030h, 0FCh, 030h, 0FCh, 030h
                        DB 0F8h, 0CCh, 0CCh, 0FAh, 0C6h, 0CFh, 0C6h, 0C3h, 00Eh, 01Bh, 018h, 03Ch, 018h, 018h, 0D8h, 070h
                        DB 01Ch, 000h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h, 038h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 000h, 01Ch, 000h, 078h, 0CCh, 0CCh, 078h, 000h, 000h, 01Ch, 000h, 0CCh, 0CCh, 0CCh, 076h, 000h
                        DB 000h, 0F8h, 000h, 0B8h, 0CCh, 0CCh, 0CCh, 000h, 0FCh, 000h, 0CCh, 0ECh, 0FCh, 0DCh, 0CCh, 000h
                        DB 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h
                        DB 018h, 000h, 018h, 018h, 030h, 066h, 03Ch, 000h, 000h, 000h, 000h, 0FCh, 0C0h, 0C0h, 000h, 000h
                        DB 000h, 000h, 000h, 0FCh, 00Ch, 00Ch, 000h, 000h, 0C6h, 0CCh, 0D8h, 036h, 06Bh, 0C2h, 084h, 00Fh
                        DB 0C3h, 0C6h, 0CCh, 0DBh, 037h, 06Dh, 0CFh, 003h, 018h, 000h, 018h, 018h, 03Ch, 03Ch, 018h, 000h
                        DB 000h, 033h, 066h, 0CCh, 066h, 033h, 000h, 000h, 000h, 0CCh, 066h, 033h, 066h, 0CCh, 000h, 000h
                        DB 022h, 088h, 022h, 088h, 022h, 088h, 022h, 088h, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh
                        DB 0DBh, 0F6h, 0DBh, 06Fh, 0DBh, 07Eh, 0D7h, 0EDh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 0F6h, 036h, 036h, 036h, 000h, 000h, 000h, 000h, 0FEh, 036h, 036h, 036h
                        DB 000h, 000h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 036h, 036h, 0F6h, 006h, 0F6h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 000h, 000h, 0FEh, 006h, 0F6h, 036h, 036h, 036h
                        DB 036h, 036h, 0F6h, 006h, 0FEh, 000h, 000h, 000h, 036h, 036h, 036h, 036h, 0FEh, 000h, 000h, 000h
                        DB 018h, 018h, 0F8h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 01Fh, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 0FFh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h
                        DB 018h, 018h, 01Fh, 018h, 01Fh, 018h, 018h, 018h, 036h, 036h, 036h, 036h, 037h, 036h, 036h, 036h
                        DB 036h, 036h, 037h, 030h, 03Fh, 000h, 000h, 000h, 000h, 000h, 03Fh, 030h, 037h, 036h, 036h, 036h
                        DB 036h, 036h, 0F7h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0F7h, 036h, 036h, 036h
                        DB 036h, 036h, 037h, 030h, 037h, 036h, 036h, 036h, 000h, 000h, 0FFh, 000h, 0FFh, 000h, 000h, 000h
                        DB 036h, 036h, 0F7h, 000h, 0F7h, 036h, 036h, 036h, 018h, 018h, 0FFh, 000h, 0FFh, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 0FFh, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 03Fh, 000h, 000h, 000h
                        DB 018h, 018h, 01Fh, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 01Fh, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 03Fh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 036h, 036h, 036h
                        DB 018h, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 01Fh, 018h, 018h, 018h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h
                        DB 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 0C8h, 0DCh, 076h, 000h, 000h, 078h, 0CCh, 0F8h, 0CCh, 0F8h, 0C0h, 0C0h
                        DB 000h, 0FCh, 0CCh, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 000h
                        DB 0FCh, 0CCh, 060h, 030h, 060h, 0CCh, 0FCh, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 070h, 000h
                        DB 000h, 066h, 066h, 066h, 066h, 07Ch, 060h, 0C0h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 000h
                        DB 0FCh, 030h, 078h, 0CCh, 0CCh, 078h, 030h, 0FCh, 038h, 06Ch, 0C6h, 0FEh, 0C6h, 06Ch, 038h, 000h
                        DB 038h, 06Ch, 0C6h, 0C6h, 06Ch, 06Ch, 0EEh, 000h, 01Ch, 030h, 018h, 07Ch, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 000h, 07Eh, 0DBh, 0DBh, 07Eh, 000h, 000h, 006h, 00Ch, 07Eh, 0DBh, 0DBh, 07Eh, 060h, 0C0h
                        DB 038h, 060h, 0C0h, 0F8h, 0C0h, 060h, 038h, 000h, 078h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 000h
                        DB 000h, 07Eh, 000h, 07Eh, 000h, 07Eh, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 07Eh, 000h
                        DB 060h, 030h, 018h, 030h, 060h, 000h, 0FCh, 000h, 018h, 030h, 060h, 030h, 018h, 000h, 0FCh, 000h
                        DB 00Eh, 01Bh, 01Bh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0D8h, 0D8h, 070h
                        DB 018h, 018h, 000h, 07Eh, 000h, 018h, 018h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h
                        DB 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 00Fh, 00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 03Ch, 01Ch
                        DB 058h, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 070h, 098h, 030h, 060h, 0F8h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 03Ch, 03Ch, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

Characters8x14          DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h;Offset 0x5f20
                        DB 000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 06Ch, 0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h, 099h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 07Eh, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 01Eh, 00Eh, 01Ah, 032h, 078h, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 03Fh, 033h, 03Fh, 030h, 030h, 030h, 070h, 0F0h, 0E0h, 000h, 000h, 000h
                        DB 000h, 000h, 07Fh, 063h, 07Fh, 063h, 063h, 063h, 067h, 0E7h, 0E6h, 0C0h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 0DBh, 03Ch, 0E7h, 03Ch, 0DBh, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 080h, 0C0h, 0E0h, 0F8h, 0FEh, 0F8h, 0E0h, 0C0h, 080h, 000h, 000h, 000h
                        DB 000h, 000h, 002h, 006h, 00Eh, 03Eh, 0FEh, 03Eh, 00Eh, 006h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 066h, 066h, 000h, 000h, 000h
                        DB 000h, 000h, 07Fh, 0DBh, 0DBh, 0DBh, 07Bh, 01Bh, 01Bh, 01Bh, 01Bh, 000h, 000h, 000h
                        DB 000h, 07Ch, 0C6h, 060h, 038h, 06Ch, 0C6h, 0C6h, 06Ch, 038h, 00Ch, 0C6h, 07Ch, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 07Eh, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 030h, 060h, 0FEh, 060h, 030h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C0h, 0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 028h, 06Ch, 0FEh, 06Ch, 028h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 010h, 038h, 038h, 07Ch, 07Ch, 0FEh, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 0FEh, 0FEh, 07Ch, 07Ch, 038h, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 03Ch, 03Ch, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 066h, 066h, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h
                        DB 018h, 018h, 07Ch, 0C6h, 0C2h, 0C0h, 07Ch, 006h, 086h, 0C6h, 07Ch, 018h, 018h, 000h
                        DB 000h, 000h, 000h, 000h, 0C2h, 0C6h, 00Ch, 018h, 030h, 066h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 06Ch, 038h, 076h, 0DCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 030h, 030h, 030h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Ch, 018h, 030h, 030h, 030h, 030h, 030h, 018h, 00Ch, 000h, 000h, 000h
                        DB 000h, 000h, 030h, 018h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 018h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 030h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 002h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0CEh, 0DEh, 0F6h, 0E6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 038h, 078h, 018h, 018h, 018h, 018h, 018h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 006h, 00Ch, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 006h, 006h, 03Ch, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 00Ch, 01Ch, 03Ch, 06Ch, 0CCh, 0FEh, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C0h, 0C0h, 0FCh, 00Eh, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 060h, 0C0h, 0C0h, 0FCh, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 006h, 00Ch, 018h, 030h, 030h, 030h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 006h, 00Ch, 078h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 006h, 00Ch, 018h, 030h, 060h, 030h, 018h, 00Ch, 006h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 060h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 060h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 00Ch, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0DEh, 0DEh, 0DEh, 0DCh, 0C0h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 066h, 066h, 066h, 0FCh, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0F8h, 06Ch, 066h, 066h, 066h, 066h, 066h, 06Ch, 0F8h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 062h, 066h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 060h, 060h, 0F0h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0DEh, 0C6h, 066h, 03Ah, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 000h, 000h, 000h
                        DB 000h, 000h, 0E6h, 066h, 06Ch, 06Ch, 078h, 06Ch, 06Ch, 066h, 0E6h, 000h, 000h, 000h
                        DB 000h, 000h, 0F0h, 060h, 060h, 060h, 060h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0DEh, 07Ch, 00Ch, 00Eh, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 06Ch, 066h, 066h, 0E6h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 060h, 038h, 00Ch, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 07Eh, 05Ah, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 010h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 07Ch, 06Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 06Ch, 038h, 038h, 038h, 06Ch, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 08Ch, 018h, 030h, 060h, 0C2h, 0C6h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 080h, 0C0h, 0E0h, 070h, 038h, 01Ch, 00Eh, 006h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 03Ch, 000h, 000h, 000h
                        DB 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h
                        DB 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 0DCh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 01Ch, 00Ch, 00Ch, 03Ch, 06Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0CCh, 078h, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 06Ch, 076h, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 006h, 006h, 000h, 00Eh, 006h, 006h, 006h, 006h, 066h, 066h, 03Ch, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 066h, 06Ch, 078h, 06Ch, 066h, 0E6h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h, 0D6h, 0D6h, 0D6h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 076h, 062h, 060h, 060h, 0F0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 070h, 01Ch, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 010h, 030h, 030h, 0FCh, 030h, 030h, 030h, 036h, 01Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 06Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 06Ch, 038h, 038h, 06Ch, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 0F8h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 0CCh, 018h, 030h, 066h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 00Eh, 018h, 018h, 018h, 070h, 018h, 018h, 018h, 00Eh, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 070h, 018h, 018h, 018h, 00Eh, 018h, 018h, 018h, 070h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 00Ch, 006h, 07Ch, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 00Ch, 018h, 030h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 038h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 03Ch, 066h, 060h, 066h, 03Ch, 00Ch, 006h, 03Ch, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 018h, 03Ch, 066h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 0C6h, 0C6h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 038h, 06Ch, 038h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 018h, 030h, 060h, 000h, 0FEh, 066h, 060h, 07Ch, 060h, 066h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0CCh, 076h, 036h, 07Eh, 0D8h, 0D8h, 06Eh, 000h, 000h, 000h
                        DB 000h, 000h, 03Eh, 06Ch, 0CCh, 0CCh, 0FEh, 0CCh, 0CCh, 0CCh, 0CEh, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 030h, 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 078h, 000h
                        DB 000h, 0C6h, 0C6h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h
                        DB 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 018h, 018h, 07Ch, 0C6h, 0C0h, 0C0h, 0C6h, 07Ch, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h, 0E6h, 0FCh, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 0F8h, 0CCh, 0CCh, 0F8h, 0C4h, 0CCh, 0DEh, 0CCh, 0CCh, 0C6h, 000h, 000h, 000h
                        DB 000h, 00Eh, 01Bh, 018h, 018h, 018h, 07Eh, 018h, 018h, 018h, 018h, 0D8h, 070h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 00Ch, 018h, 030h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h
                        DB 076h, 0DCh, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 030h, 030h, 000h, 030h, 030h, 060h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 006h, 006h, 006h, 000h, 000h, 000h, 000h
                        DB 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h, 030h, 060h, 0CEh, 093h, 006h, 00Ch, 01Fh, 000h
                        DB 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h, 030h, 066h, 0CEh, 09Ah, 03Fh, 006h, 00Fh, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 018h, 018h, 03Ch, 03Ch, 03Ch, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 033h, 066h, 0CCh, 066h, 033h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0CCh, 066h, 033h, 066h, 0CCh, 000h, 000h, 000h, 000h, 000h
                        DB 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h
                        DB 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh
                        DB 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 037h, 030h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 03Fh, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 037h, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h
                        DB 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 0D8h, 0D8h, 0DCh, 076h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FCh, 0C6h, 0C6h, 0FCh, 0C0h, 0C0h, 0C0h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 060h, 030h, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 0D8h, 070h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0C0h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 030h, 018h, 00Ch, 03Eh, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 003h, 006h, 07Eh, 0CFh, 0DBh, 0F3h, 07Eh, 060h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 01Ch, 030h, 060h, 060h, 07Ch, 060h, 060h, 030h, 01Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 000h
                        DB 000h, 000h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 000h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 00Ch, 018h, 030h, 060h, 030h, 018h, 00Ch, 000h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 00Eh, 01Bh, 01Bh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0D8h, 0D8h, 070h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 000h, 07Eh, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 00Fh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 03Ch, 01Ch, 000h, 000h, 000h
                        DB 000h, 0D8h, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 070h, 098h, 030h, 060h, 0C8h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

Patch8x14               DB 01Dh, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h;Offset 0x6d20
                        DB 022h, 000h, 063h, 063h, 063h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 02Bh, 000h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 04Dh, 000h, 000h, 0C3h, 0E7h, 0FFh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h
                        DB 054h, 000h, 000h, 0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 057h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h
                        DB 058h, 000h, 000h, 0C3h, 0C3h, 066h, 03Ch, 018h, 03Ch, 066h, 0C3h, 0C3h, 000h, 000h, 000h
                        DB 05Ah, 000h, 000h, 0FFh, 0C3h, 086h, 00Ch, 018h, 030h, 061h, 0C3h, 0FFh, 000h, 000h, 000h
                        DB 05Bh, 000h, 000h, 03Eh, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 03Eh, 000h, 000h, 000h
                        DB 05Dh, 000h, 000h, 03Eh, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 03Eh, 000h, 000h, 000h
                        DB 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh, 0DBh, 0DBh, 0DBh, 000h, 000h, 000h
                        DB 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 000h, 000h, 000h
                        DB 091h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h, 000h, 000h, 000h
                        DB 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 07Eh, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h
                        DB 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 0F3h, 000h, 000h, 000h
                        DB 0F1h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 0FFh, 000h, 000h, 000h
                        DB 0F6h, 000h, 000h, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h ;Terminate

Characters8x16          DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h;Offset 0x6e30
                        DB 000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 081h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 0FFh, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 06Ch, 0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h, 099h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 07Eh, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 01Eh, 00Eh, 01Ah, 032h, 078h, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Fh, 033h, 03Fh, 030h, 030h, 030h, 030h, 070h, 0F0h, 0E0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Fh, 063h, 07Fh, 063h, 063h, 063h, 063h, 067h, 0E7h, 0E6h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 0DBh, 03Ch, 0E7h, 03Ch, 0DBh, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 080h, 0C0h, 0E0h, 0F0h, 0F8h, 0FEh, 0F8h, 0F0h, 0E0h, 0C0h, 080h, 000h, 000h, 000h, 000h
                        DB 000h, 002h, 006h, 00Eh, 01Eh, 03Eh, 0FEh, 03Eh, 01Eh, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Fh, 0DBh, 0DBh, 0DBh, 07Bh, 01Bh, 01Bh, 01Bh, 01Bh, 01Bh, 000h, 000h, 000h, 000h
                        DB 000h, 07Ch, 0C6h, 060h, 038h, 06Ch, 0C6h, 0C6h, 06Ch, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh, 0FEh, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 060h, 0FEh, 060h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C0h, 0C0h, 0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 028h, 06Ch, 0FEh, 06Ch, 028h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 038h, 07Ch, 07Ch, 0FEh, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FEh, 0FEh, 07Ch, 07Ch, 038h, 038h, 010h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 03Ch, 03Ch, 018h, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 066h, 066h, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 07Ch, 0C6h, 0C2h, 0C0h, 07Ch, 006h, 086h, 0C6h, 07Ch, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0C2h, 0C6h, 00Ch, 018h, 030h, 060h, 0C6h, 086h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 06Ch, 038h, 076h, 0DCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 030h, 030h, 030h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Ch, 018h, 030h, 030h, 030h, 030h, 030h, 030h, 018h, 00Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 030h, 018h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 018h, 030h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 002h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0CEh, 0D6h, 0D6h, 0E6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 038h, 078h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 006h, 006h, 03Ch, 006h, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Ch, 01Ch, 03Ch, 06Ch, 0CCh, 0FEh, 00Ch, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 0FCh, 006h, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 060h, 0C0h, 0C0h, 0FCh, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 006h, 006h, 00Ch, 018h, 030h, 030h, 030h, 030h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 006h, 006h, 00Ch, 078h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 006h, 00Ch, 018h, 030h, 060h, 030h, 018h, 00Ch, 006h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 060h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 060h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 00Ch, 018h, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0DEh, 0DEh, 0DEh, 0DCh, 0C0h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 066h, 066h, 066h, 066h, 0FCh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0F8h, 06Ch, 066h, 066h, 066h, 066h, 066h, 066h, 06Ch, 0F8h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0DEh, 0C6h, 0C6h, 066h, 03Ah, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0E6h, 066h, 06Ch, 06Ch, 078h, 078h, 06Ch, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0F0h, 060h, 060h, 060h, 060h, 060h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0DEh, 07Ch, 00Ch, 00Eh, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 06Ch, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 060h, 038h, 00Ch, 006h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 07Eh, 05Ah, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 06Ch, 06Ch, 038h, 038h, 06Ch, 06Ch, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 086h, 00Ch, 018h, 030h, 060h, 0C2h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 080h, 0C0h, 0E0h, 070h, 038h, 01Ch, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 03Ch, 000h, 000h, 000h, 000h
                        DB 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h
                        DB 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 066h, 0DCh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C0h, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 01Ch, 00Ch, 00Ch, 03Ch, 06Ch, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0CCh, 078h, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 06Ch, 076h, 066h, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 006h, 006h, 000h, 00Eh, 006h, 006h, 006h, 006h, 006h, 006h, 066h, 066h, 03Ch, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 066h, 06Ch, 078h, 078h, 06Ch, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h, 0D6h, 0D6h, 0D6h, 0D6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 076h, 062h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 060h, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 010h, 030h, 030h, 0FCh, 030h, 030h, 030h, 030h, 036h, 01Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 066h, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 06Ch, 038h, 038h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 0F8h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 0CCh, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Eh, 018h, 018h, 018h, 070h, 018h, 018h, 018h, 018h, 00Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 070h, 018h, 018h, 018h, 00Eh, 018h, 018h, 018h, 018h, 070h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 00Ch, 006h, 07Ch, 000h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 00Ch, 018h, 030h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 038h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 03Ch, 066h, 060h, 060h, 066h, 03Ch, 00Ch, 006h, 03Ch, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 018h, 03Ch, 066h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 0C6h, 0C6h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 038h, 06Ch, 038h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 018h, 030h, 060h, 000h, 0FEh, 066h, 060h, 07Ch, 060h, 060h, 066h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 076h, 036h, 07Eh, 0D8h, 0D8h, 06Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Eh, 06Ch, 0CCh, 0CCh, 0FEh, 0CCh, 0CCh, 0CCh, 0CCh, 0CEh, 000h, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 030h, 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 078h, 000h
                        DB 000h, 0C6h, 0C6h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h
                        DB 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 018h, 018h, 03Ch, 066h, 060h, 060h, 060h, 066h, 03Ch, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h, 060h, 0E6h, 0FCh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 07Eh, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 0F8h, 0CCh, 0CCh, 0F8h, 0C4h, 0CCh, 0DEh, 0CCh, 0CCh, 0CCh, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 00Eh, 01Bh, 018h, 018h, 018h, 07Eh, 018h, 018h, 018h, 018h, 018h, 0D8h, 070h, 000h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 00Ch, 018h, 030h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 018h, 030h, 060h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 076h, 0DCh, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 030h, 030h, 000h, 030h, 030h, 060h, 0C0h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 006h, 006h, 006h, 006h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 060h, 0CEh, 093h, 006h, 00Ch, 01Fh, 000h, 000h
                        DB 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 066h, 0CEh, 09Ah, 03Fh, 006h, 00Fh, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 018h, 018h, 018h, 03Ch, 03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 033h, 066h, 0CCh, 066h, 033h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 066h, 033h, 066h, 0CCh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h
                        DB 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh
                        DB 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 037h, 030h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 03Fh, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 037h, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
                        DB 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h
                        DB 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 0D8h, 0D8h, 0D8h, 0DCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FCh, 0C6h, 0FCh, 0C6h, 0C6h, 0FCh, 0C0h, 0C0h, 0C0h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 080h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 0FEh, 0C6h, 060h, 030h, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 030h, 018h, 00Ch, 03Eh, 066h, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0DBh, 0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 003h, 006h, 07Eh, 0CFh, 0DBh, 0F3h, 07Eh, 060h, 0C0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 01Ch, 030h, 060h, 060h, 07Ch, 060h, 060h, 060h, 030h, 01Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 000h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 00Ch, 018h, 030h, 060h, 030h, 018h, 00Ch, 000h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Eh, 01Bh, 01Bh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0D8h, 0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 018h, 000h, 07Eh, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 00Fh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 06Ch, 03Ch, 01Ch, 000h, 000h, 000h, 000h
                        DB 000h, 0D8h, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 070h, 098h, 030h, 060h, 0C8h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

Patch8x16               DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h;Offset 0x7e30
                        DB 022h, 000h, 063h, 063h, 063h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 02Bh, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 0FFh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 04Dh, 000h, 000h, 0C3h, 0E7h, 0FFh, 0DBh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h, 000h
                        DB 054h, 000h, 000h, 0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 057h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 05Ah, 000h, 000h, 0FFh, 0C3h, 083h, 006h, 00Ch, 018h, 030h, 061h, 0C3h, 0FFh, 000h, 000h, 000h, 000h
                        DB 05Bh, 000h, 000h, 03Eh, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 03Eh, 000h, 000h, 000h, 000h
                        DB 05Dh, 000h, 000h, 03Eh, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 03Eh, 000h, 000h, 000h, 000h
                        DB 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh, 0DBh, 0DBh, 0DBh, 0DBh, 000h, 000h, 000h, 000h
                        DB 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 000h, 000h, 000h, 000h
                        DB 091h, 000h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h, 000h, 000h, 000h, 000h
                        DB 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 07Eh, 018h, 07Eh, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 066h, 0F3h, 000h, 000h, 000h, 000h
                        DB 0F1h, 000h, 000h, 000h, 000h, 018h, 018h, 0FFh, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h
                        DB 0F6h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 0FFh, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h ;Terminate

ORG 7fffh
DB 3Eh
ROMEnd:
END