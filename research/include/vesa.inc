;VESA Data structures

;Interrupt list: http://www.ctyme.com/intr/int.htm

VESA_INFO_Signature                     EQU 000h;0x00
VESA_INFO_Version                       EQU 004h;0x04
VESA_INFO_OEMNamePointer                EQU 006h;0x06
VESA_INFO_OEMNamePointerOfs             EQU 006h;0x06
VESA_INFO_OEMNamePointerSeg             EQU 008h;0x08
VESA_INFO_Capabilities                  EQU 00ah;0x0a
VESA_INFO_SupportedVideoModesPointer    EQU 00eh;0x0e
VESA_INFO_SupportedVideoModesPointerOfs EQU 00eh;0x0e
VESA_INFO_SupportedVideoModesPointerSeg EQU 010h;0x10
VESA_INFO_MemoryIn64KBBlocks            EQU 012h;0x12
VESA_INFO_OEMSoftwareVersion            EQU 014h;0x14
VESA_INFO_VendorNamePointer             EQU 016h;0x16
VESA_INFO_VendorNamePointerOfs          EQU 016h;0x16
VESA_INFO_VendorNamePointerSeg          EQU 018h;0x18
VESA_INFO_ProductNamePointer            EQU 01ah;0x1a
VESA_INFO_ProductNamePointerOfs         EQU 01ah;0x1a
VESA_INFO_ProductNamePointerSeg         EQU 01ch;0x1c
VESA_INFO_RevisionStringPointer         EQU 01eh;0x1e
VESA_INFO_RevisionStringPointerOfs      EQU 01eh;0x1e
VESA_INFO_RevisionStringPointerSeg      EQU 020h;0x20

;         Display |   Simultaneous display colors
;      resolution |   16      256     32K     64K     16M
;-----------------+----------------------------------------------
;    640 x 400    |   --      100      --      --      --
;    640 x 480    | (VGA)     101     110     111     112
;    800 x 600    |  102      103     113     114     115
;   1024 x 768  * |   --      105     116     117     118
;   1280 x 1024 * |   --      107     119     11A     11B
;   1600 x 1200 * |   --      11C     11D     11E      --

VESA_MODE_640x400x256                   EQU 0100h;0x100
VESA_MODE_640x480x256                   EQU 0101h;0x101
VESA_MODE_800x600x16                    EQU 0102h;0x102
VESA_MODE_800x600x256                   EQU 0103h;0x103
VESA_MODE_1024x768x256                  EQU 0105h;0x105
VESA_MODE_1280x1024x256                 EQU 0107h;0x107
VESA_MODE_80x60xText                    EQU 0108h;0x108
VESA_MODE_132x25xText                   EQU 0109h;0x109
VESA_MODE_132x43xText                   EQU 010ah;0x10a
VESA_MODE_132x50xText                   EQU 010bh;0x10b
VESA_MODE_132x60xText                   EQU 010ch;0x10c
VESA_MODE_640x480x32K                   EQU 0110h;0x110
VESA_MODE_640x480x64K                   EQU 0111h;0x111
VESA_MODE_640x480x16M                   EQU 0112h;0x112
VESA_MODE_800x600x32K                   EQU 0113h;0x113
VESA_MODE_800x600x64K                   EQU 0114h;0x114
VESA_MODE_800x600x16M                   EQU 0115h;0x115
VESA_MODE_1024x768x32K                  EQU 0116h;0x116
VESA_MODE_1024x768x64K                  EQU 0117h;0x117
VESA_MODE_1024x768x16M                  EQU 0118h;0x118
VESA_MODE_1280x1024x32K                 EQU 0119h;0x119
VESA_MODE_1280x1024x64K                 EQU 011ah;0x11a
VESA_MODE_1280x1024x16M                 EQU 011bh;0x11b
VESA_MODE_1600x1200x256                 EQU 011ch;0x11c
VESA_MODE_1600x1200x32K                 EQU 011dh;0x11d
VESA_MODE_1600x1200x64K                 EQU 011eh;0x11e
VESA_MODE_1600x1200x16M                 EQU 011fh;0x11f

VESA_MODE_INFO_Attributes               EQU 00h;0x00 Word
VESA_MODE_INFO_AttributesWindowA        EQU 02h;0x02 Byte
VESA_MODE_INFO_AttributesWindowB        EQU 03h;0x03 Byte
VESA_MODE_INFO_WindowGranularityInKB    EQU 04h;0x04 Word
VESA_MODE_INFO_WindowSizeInKB           EQU 06h;0x06 Word
VESA_MODE_INFO_StartSegmentWindowA      EQU 08h;0x08 Word
VESA_MODE_INFO_StartSegmentWindowB      EQU 0ah;0x0a Word
VESA_MODE_INFO_FARWindowPositioningFunc EQU 0ch;0x0c DWord
VESA_MODE_INFO_FARWindowPositioningOfs  EQU 0ch;0x0c Word
VESA_MODE_INFO_FARWindowPositioningSeg  EQU 0eh;0x0e Word
VESA_MODE_INFO_BytesPerScanLine         EQU 10h;0x10 Word
;-------1.0/1.1 optional----------------------------
VESA_MODE_INFO_WidthInPixelsOrChars     EQU 12h;0x12 Word
VESA_MODE_INFO_HeightInPixelsOrChars    EQU 14h;0x14 Word
VESA_MODE_INFO_CharCellWidth            EQU 16h;0x16 Byte
VESA_MODE_INFO_CharCellHeight           EQU 17h;0x17 Byte
VESA_MODE_INFO_NumberOfMemoryPlanes     EQU 18h;0x18 Byte
VESA_MODE_INFO_NumberOfBitsPerPixel     EQU 19h;0x19 Byte
VESA_MODE_INFO_NumberOfBanks            EQU 1ah;0x1a Byte
VESA_MODE_INFO_MemoryModelType          EQU 1bh;0x1b Byte
VESA_MODE_INFO_SizeOfBankInKB           EQU 1ch;0x1c Byte
VESA_MODE_INFO_NumberOfImgPagesMinusOne EQU 1dh;0x1d Byte
VESA_MODE_INFO_Reserved1E               EQU 1eh;0x1e Byte
;-------1.2+----------------------------------------
VESA_MODE_INFO_RedMaskSize              EQU 1fh;0x1f Byte
VESA_MODE_INFO_RedFieldPosition         EQU 20h;0x20 Byte
VESA_MODE_INFO_GreenMaskSize            EQU 21h;0x21 Byte
VESA_MODE_INFO_GreenFieldPosition       EQU 22h;0x22 Byte
VESA_MODE_INFO_BlueMaskSize             EQU 23h;0x23 Byte
VESA_MODE_INFO_BlueFieldPosition        EQU 24h;0x24 Byte
VESA_MODE_INFO_ReservedMaskSize         EQU 25h;0x25 Byte
VESA_MODE_INFO_ReservedFieldPosition    EQU 26h;0x26 Byte
VESA_MODE_INFO_DirectColorModeInfo      EQU 27h;0x27 Byte
;-------2.0+----------------------------------------
VESA_MODE_INFO_LinearFrameBufferAddress EQU 28h;0x28 DWord
VESA_MODE_INFO_OffScreenMemoryAddress   EQU 2ch;0x2c DWord
VESA_MODE_INFO_OffScreenMemorySizeInKB  EQU 30h;0x30 Word
;-------3.0-----------------------------------------
VESA_MODE_INFO_BytesPerScanLineLinear   EQU 32h;0x32 Word
VESA_MODE_INFO_ImageCountMinusOneBanked EQU 34h;0x34 Byte
VESA_MODE_INFO_ImageCountMinusOneLinear EQU 35h;0x35 Byte
VESA_MODE_INFO_LinearRedMaskSizeBits    EQU 36h;0x36 Byte
VESA_MODE_INFO_LinearPosRedMaskLSB      EQU 37h;0x37 Byte
VESA_MODE_INFO_LinearGreenMaskSizeBits  EQU 38h;0x38 Byte
VESA_MODE_INFO_LinearPosGreenMaskLSB    EQU 39h;0x39 Byte
VESA_MODE_INFO_LinearBlueMaskSizeBits   EQU 3ah;0x3a Byte
VESA_MODE_INFO_LinearPosBlueMaskLSB     EQU 3bh;0x3b Byte
VESA_MODE_INFO_LinearResrvdMaskSizeBits EQU 3ch;0x3c Byte
VESA_MODE_INFO_LinearPosResrvdMaskLSB   EQU 3dh;0x3d Byte
VESA_MODE_INFO_MaxPixelClockHz          EQU 3eh;0x3e Byte


VESA_MODE_ATTR_NotSupported             EQU 0000h;0x0000
VESA_MODE_ATTR_Supported                EQU 0001h;0x0001
VESA_MODE_ATTR_Support_MASK             EQU 0001h;0x0001
VESA_MODE_ATTR_OptionalInfoNotAvailable EQU 0000h;0x0000
VESA_MODE_ATTR_OptionalInfoAvailable    EQU 0002h;0x0002
VESA_MODE_ATTR_OptionalInfo_MASK        EQU 0002h;0x0002
VESA_MODE_ATTR_BIOSOutputNotSupported   EQU 0000h;0x0000
VESA_MODE_ATTR_BIOSOutputSupported      EQU 0004h;0x0004
VESA_MODE_ATTR_BIOSOutput_MASK          EQU 0004h;0x0004
VESA_MODE_ATTR_Monochrome               EQU 0000h;0x0000
VESA_MODE_ATTR_Color                    EQU 0008h;0x0008
VESA_MODE_ATTR_Color_MASK               EQU 0008h;0x0008
VESA_MODE_ATTR_ModeText                 EQU 0000h;0x0000
VESA_MODE_ATTR_ModeGraphics             EQU 0010h;0x0010
VESA_MODE_ATTR_Mode_MASK                EQU 0010h;0x0010
VESA_MODE_ATTR_VGAIncompatible          EQU 0000h;0x0000
VESA_MODE_ATTR_VGACompatible            EQU 0020h;0x0020
VESA_MODE_ATTR_VGA_MASK                 EQU 0020h;0x0020
VESA_MODE_ATTR_BankSwitchNotSupported   EQU 0000h;0x0000
VESA_MODE_ATTR_BankSwitchSupported      EQU 0040h;0x0040
VESA_MODE_ATTR_BankSwitch_MASK          EQU 0040h;0x0040
VESA_MODE_ATTR_LinearNotSupported       EQU 0000h;0x0000
VESA_MODE_ATTR_LinearSupported          EQU 0080h;0x0080
VESA_MODE_ATTR_Linear_MASK              EQU 0080h;0x0080
VESA_MODE_ATTR_DoubleScanNotSupported   EQU 0000h;0x0000
VESA_MODE_ATTR_DoubleScanSupported      EQU 0100h;0x0100
VESA_MODE_ATTR_DoubleScan_MASK          EQU 0100h;0x0100
VESA_MODE_ATTR_InterlaceNotSupported    EQU 0000h;0x0000
VESA_MODE_ATTR_InterlaceSupported       EQU 0200h;0x0200
VESA_MODE_ATTR_Interlace_MASK           EQU 0200h;0x0200
VESA_MODE_ATTR_TripleBufNotSupported    EQU 0000h;0x0000
VESA_MODE_ATTR_TripleBufSupported       EQU 0400h;0x0400
VESA_MODE_ATTR_TripleBuffer_MASK        EQU 0400h;0x0400
VESA_MODE_ATTR_StereoscopicNotSupported EQU 0000h;0x0000
VESA_MODE_ATTR_StereoscopicSupported    EQU 0800h;0x0800
VESA_MODE_ATTR_Stereoscopic_MASK        EQU 0800h;0x0800
VESA_MODE_ATTR_DualDisplayNotSupported  EQU 0000h;0x0000
VESA_MODE_ATTR_DualDisplaySupported     EQU 1000h;0x1000
VESA_MODE_ATTR_DualDisplay_MASK         EQU 1000h;0x1000

VESA_WND_ATTR_DoesntExist               EQU 00h;0x00
VESA_WND_ATTR_Exists                    EQU 01h;0x01
VESA_WND_ATTR_Existence_MASK            EQU 01h;0x01
VESA_WND_ATTR_NotReadable               EQU 00h;0x00
VESA_WND_ATTR_Readable                  EQU 02h;0x02
VESA_WND_ATTR_Read_MASK                 EQU 02h;0x02
VESA_WND_ATTR_NotWriteable              EQU 00h;0x00
VESA_WND_ATTR_Writeable                 EQU 04h;0x04
VESA_WND_ATTR_Write_MASK                EQU 04h;0x04

VESA_MEM_MODEL_Text                     EQU 00h;0x00
VESA_MEM_MODEL_CGA_Graphics             EQU 01h;0x01
VESA_MEM_MODEL_HGC_Graphics             EQU 02h;0x02
VESA_MEM_MODEL_EGA_Graphics             EQU 03h;0x03
VESA_MEM_MODEL_PackedPixelGraphics      EQU 04h;0x04
VESA_MEM_MODEL_SEQU256Graphics          EQU 05h;0x05
VESA_MEM_MODEL_DirectColorGraphics      EQU 06h;0x06
VESA_MEM_MODEL_YUV_Graphics             EQU 07h;0x07

