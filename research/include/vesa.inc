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
