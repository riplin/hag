;Matrox G200 Registers (only includes the ones used by the BIOS)

include gshared.inc

;MGA_CRTCExtensionIndex
    MGA_CRTCExt_RequesterControl        EQU 007h;0x07

MGA_MemAddr_MemoryReadBack              EQU 1e44h;0x1e44
    MGA_MA_MRB_ModeRegSetCMDOpcode_MASK EQU 3e000000h;0x3e000000

