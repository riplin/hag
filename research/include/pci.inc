;PCI registers

;Ports list: https://github.com/cirosantilli/ralf-brown-interrupt-list/blob/master/inter61d/PORTS.B
;PCI: https://wiki.osdev.org/PCI

PCI_M1_ConfigAddress                    EQU 0cf8h;0xcf8
PCI_M1_ConfigAddress_lowbyte            EQU 0f8h;0xf8
    PCI_M1_ConfigAddres_Enable          EQU 80000000h;0x80000000
PCI_M1_ConfigData                       EQU 0cfch;0xcfc
PCI_M1_ConfigData_lowbyte               EQU 0fch;0xfc
PCI_M1_ConfigData_B0                    EQU 0cfch;0xcfc
PCI_M1_ConfigData_W0_lowbyte            EQU 0fch;0xfc
PCI_M1_ConfigData_B0_lowbyte            EQU 0fch;0xfc
PCI_M1_ConfigData_B1                    EQU 0cfdh;0xcfd
PCI_M1_ConfigData_B1_lowbyte            EQU 0fdh;0xfd
PCI_M1_ConfigData_B2                    EQU 0cfeh;0xcfe
PCI_M1_ConfigData_W1_lowbyte            EQU 0feh;0xfe
PCI_M1_ConfigData_B2_lowbyte            EQU 0feh;0xfe
PCI_M1_ConfigData_B3                    EQU 0cffh;0xcff
PCI_M1_ConfigData_B3_lowbyte            EQU 0ffh;0xff

PCI_M2_ConfigSpaceEnable                EQU 0cf8h;0xcf8
PCI_M2_ForwardRegister                  EQU 0cfah;0xcfa
PCI_M2_ForwardRegister_lowbyte          EQU 0fah;0xfa
PCI_M2_DataBase                         EQU 0c000h;0xc000

PCI_Header_Word_VendorID                EQU 000h;0x00
PCI_Header_Word_DeviceID                EQU 002h;0x02
PCI_Header_Word_Command                 EQU 004h;0x04
    PCI_Cmd_IOSpace                     EQU 001h;0x01
    PCI_Cmd_MemSpace                    EQU 002h;0x02
    PCI_Cmd_BusMaster                   EQU 004h;0x04
    PCI_Cmd_SpecialCycles               EQU 008h;0x08
    PCI_Cmd_MemWriteAndInvalidateEnbl   EQU 010h;0x10
    PCI_Cmd_VGAPaletteSnoop             EQU 020h;0x20
    PCI_Cmd_ParityErrorResponse         EQU 040h;0x40
    PCI_Cmd_SERREnable                  EQU 100h;0x100
    PCI_Cmd_FastBackToBackEnable        EQU 200h;0x200
    PCI_Cmd_InterruptDisable            EQU 400h;0x400
PCI_Header_Word_Status                  EQU 006h;0x06
PCI_Header_Byte_RevisionID              EQU 008h;0x08
PCI_Header_Byte_ProgrammingInterface    EQU 009h;0x09
PCI_Header_Byte_SubClassCode            EQU 00ah;0x0a
PCI_Header_Byte_ClassCode               EQU 00bh;0x0b
PCI_Header_Byte_CacheLineSize           EQU 00ch;0x0c
PCI_Header_Byte_LatchTimer              EQU 00dh;0x0d
PCI_Header_Byte_HeaderType              EQU 00eh;0x0e
PCI_Header_Byte_BuiltInSelfTest         EQU 00fh;0x0f

PCI_H0_DWord_BaseAddress0               EQU 010h;0x10
PCI_H0_DWord_BaseAddress1               EQU 014h;0x14
PCI_H0_DWord_BaseAddress2               EQU 018h;0x18
PCI_H0_DWord_BaseAddress3               EQU 01ch;0x1c
PCI_H0_DWord_BaseAddress4               EQU 020h;0x20
PCI_H0_DWord_BaseAddress5               EQU 024h;0x24
PCI_H0_DWord_CardBusCISPointer          EQU 028h;0x28
PCI_H0_Word_SubsystemVendorID           EQU 02ch;0x2c
PCI_H0_Word_SubsystemID                 EQU 02eh;0x2e
PCI_H0_DWord_ExpansionROMBaseAddress    EQU 030h;0x30
PCI_H0_Byte_CapabilitiesPointer         EQU 034h;0x34
PCI_H0_Byte_InterruptLine               EQU 03ch;0x3c
PCI_H0_Byte_InterruptPIN                EQU 03dh;0x3d
PCI_H0_Byte_MinGrant                    EQU 03eh;0x3e
PCI_H0_Byte_MaxLatency                  EQU 03fh;0x3f
