;Matrox Mystique PCI BIOS v1.8 b37 disassembly

;Resources:
;
;BIOS files: https://ftp.matrox.com/pub/mga/bios/setup351.exe (can be unzipped)
;
;DOS video card tests: https://gona.mactar.hu/DOS_TESTS/
;
;Display Data Channel:https://glenwing.github.io/docs/VESA-EDDC-1.2.pdf
;Display Data Channel:https://glenwing.github.io/docs/VESA-DDCCI-1.1.pdf
;VESA EDID:https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
;
;Matrox Mystique Specification: https://bitsavers.trailing-edge.com/components/matrox/_dataSheets/MGA-1064sg_199702.pdf
;
;ALI M1541 Specification: https://bitsavers.org/components/ali/M1541_M1542_Aladdin_V_Socket_7_North_Bridge.pdf
;
;I don't have the specification for the IBM 27 82351 PCI to PCI Bridge. 
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT 858-8.asm
;Produces a .COM file. Just rename to .BIN
;
;There are a number of commented out lines of code.
;These aren't wrong instructions, they are just encoded differently by whatever compiler / assembler Matrox used.
;I commented them out because I wanted the output to be 100% binary compatible.
;
;Having said that, there are two instances of jump instructions seemingly jumping to invalid addresses.
;Not sure what's going on there.
;

include ..\..\include\bda.inc
include ..\..\include\keyboard.inc
include ..\..\include\pci.inc
include ..\..\include\pit.inc
include ..\..\include\vesa.inc
include ..\..\include\vga.inc
include myst.inc

;Offset in calltables. Not sure on the names yet.
MGA_DDCPin0Send1                        EQU 00h;0x00
MGA_DDCPin1Send1                        EQU 02h;0x02
MGA_DDCPin0Send0                        EQU 04h;0x04
MGA_DDCPin1Send0                        EQU 06h;0x06
MGA_DDCPin0Read                         EQU 08h;0x08
MGA_DDCPin1Read                         EQU 0ah;0x0a

PCI_VENDOR_Intel                        EQU 8086h;0x8086
PCI_DEVICE_Triton_430FX                 EQU 122dh;0x122d
PCI_VENDOR_IBM                          EQU 1014h;0x1014
PCI_DEVICE_IBM27_82351                  EQU 0022h;0x0022    PCI to PCI bridge
PCI_VENDOR_Matrox                       EQU 102bh;0x102b
PCI_DEVICE_MystiquePCI                  EQU 051ah;0x051a
PCI_DEVICE_MillenniumIIPCI              EQU 051bh;0x051b

MGA_Mystique_Rev3                       EQU 03h;0x03

PCI_ACCESS_ReadByte                     EQU 08h;0x08
PCI_ACCESS_ReadWord                     EQU 09h;0x09
PCI_ACCESS_ReadDWord                    EQU 0ah;0x0a
PCI_ACCESS_WriteByte                    EQU 0bh;0x0b
PCI_ACCESS_WriteWord                    EQU 0ch;0x0c
PCI_ACCESS_WriteDWord                   EQU 0dh;0x0d

;ConfigurePixelClocks cx:
MGA_CPC_PLLSetA                         EQU 000h;0x00
MGA_CPC_PLLSetB                         EQU 001h;0x01
MGA_CPC_PLLSetC                         EQU 002h;0x02
MGA_CPC_ClockPCI                        EQU 000h;0x00
MGA_CPC_ClockPLL                        EQU 004h;0x04
MGA_CPC_ClockVDOCLK                     EQU 008h;0x08

.MODEL tiny
.386
.CODE
ORG 0000h

ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

jmp entry                               ;Offset 0x80

;Offset 0x5
                        DB 018h         ;BIOS Version (1.8)
                        DB 0D0h, 003h, 0E0h, 004h, 0E0h, 011h, 00Eh, 037h, 01Ah, 012h
                        DB 024h, 012h, 0FFh, 0FFh, 000h, 000h, 000h, 000h

;Offset 0x18
                        DW offset PCIHeader;0x60
                        DW 0000h        ;PnP header pointer
                        DW 2000h        ;Unknown
                        DB 'IBM COMPATIBLE MATROX/MYSTIQUE  VGA/VBE BIOS (V1.8 )', 000h

;Offset 0x53
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    nop

;Offset 0x60
PCIHeader               DB 'PCIR'       ;PCI header magic
                        DW PCI_VENDOR_Matrox;Vendor ID
                        DW PCI_DEVICE_MystiquePCI;Device ID
                        DW 0000h        ;Pointer to vital product data (0 = none)
                        DW 0018h        ;PCI data structure length
                        DB 00h          ;PCI data structure revision
                        DB 00h, 00h, 03h;PCI device class code
                        DW (ROMEnd - ROMStart)/512;Rom size in 512 byte blocks
                        DW 2518h        ;Revision level of code
                        DB 00h          ;Code type (0 = x86)
                        DB 80h          ;Last image indicator
                        DW 0000h        ;Reserved

;Offset 0x78
BuildVersion            DB '858-8', 000h
                        DB 0FFh, 0FFh

entry:                                  ;Offset 0x80
    call      Func0x5530                ;Offset 0x5530
    mov       bx, ax
    call      ReadSubClassCode          ;Offset 0x55d5
    je        Label0xac                 ;Offset 0xac
    mov       ax, cs
    cmp       ax, 0c000h                ;Segment 0xc000
    je        Label0xac                 ;Offset 0xac
    mov       al, 04h
    mov       byte ptr cs:[ROMSize], al ;Offset 0x2 Resize ROM to 2KB
    xor       ax, ax
    xor       si, si
    mov       cx, 07ffh
Label0x9e:                              ;Offset 0x9e
    add       al, byte ptr cs:[si]
    inc       si
    loop      Label0x9e                 ;Offset 0x9e
    neg       al
    mov       byte ptr cs:[si], al
    xor       ax, ax
    retf
Label0xac:                              ;Offset 0xac
    push      bp
    xor       ax, ax
    mov       ds, ax
    mov       es, ax
    call      FindMystique              ;Offset 0x57c6
    test      byte ptr cs:[Data0x387], 02h;Offset 0x387
    je        Label0x102                ;Offset 0x102
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop       
    mov       di, PCI_Header_Byte_RevisionID;0x8
    call      AccessPCIRegister         ;Offset 0x5734
    cmp       cl, MGA_Mystique_Rev3     ;0x3
    jae       Label0x102                ;Offset 0x102
    mov       byte ptr cs:[Data0x780b], 32h;Offset 0x780b
    mov       byte ptr cs:[Data0x780c], 0ffh;Offset 0x780c
    mov       byte ptr cs:[Data0x780d], 0ffh;Offset 0x780d
    mov       byte ptr cs:[Data0x780e], 0ffh;Offset 0x780e
    mov       byte ptr cs:[Data0x780f], 0ffh;Offset 0x780f
    xor       ax, ax
    xor       si, si
Label0xee:                              ;Offset 0xee
    add       al, byte ptr cs:[si + PINS];Offset 0x77e0
    inc       si
    cmp       si, 003fh
    jne       Label0xee                 ;Offset 0xee
    xor       al, 0ffh
    inc       al
    mov       byte ptr cs:[si + PINS], al;Offset 0x77e0
Label0x102:                             ;Offset 0x102
    call      FindMystique              ;Offset 0x57c6
    call      TurnScreenOff             ;Offset 0x3109
    mov       cx, 0058h
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x5c6d
    call      ConfigureAndSelectPLLSetB ;Offset 0x6190
    push      bx
    mov       al, 40h
    call      SetupInterruptsTextModeAndBDA;Offset 0x147
    pop       bx
    call      SetMemoryTimingsAndRefresh;Offset 0x5d85
    xor       cx, cx
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x5c6d
    cld
    mov       dx, VGA_CRTControllerIndexD;Port 0x3d4
    call      Func0x2ce3                ;Offset 0x2ce3
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       di, 2000h
    mov       cx, 0c000h
    xor       ax, ax
    rep stosb
    mov       es, ax
    call      Func0x2ce9                ;Offset 0x2ce9
    mov       ax, 0003h
    int       10h
    call      PrintStartupSplash        ;Offset 0x388
    pop       bp
    xor       ax, ax
    retf

SetupInterruptsTextModeAndBDA PROC NEAR ;Offset 0x147
    cli       
    mov       word ptr ds:[INT_42_HandlerOfs], SYS_INT_10h_HandlerOfs;Offset 0x108 Offset 0xf065
    mov       word ptr ds:[INT_42_HandlerSeg], SYS_INT_10h_HandlerSeg;Offset 0x10a Segment 0xf000
    mov       word ptr ds:[INT_10_HandlerOfs], offset Int10Handler;Offset 0x40 Offset 0x1240
    mov       word ptr ds:[INT_10_HandlerSeg], cs;Offset 0x42
    mov       word ptr ds:[INT_6D_HandlerOfs], offset Int6DHandler;Offset 0x1b4 Offset 0x1250
    mov       word ptr ds:[INT_6D_HandlerSeg], cs;Offset 0x1b6
    sti
    mov       word ptr ds:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x3b20
    mov       word ptr ds:[INT_1F_HandlerSeg], cs;Offset 0x7e
    mov       word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3720
    mov       word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    mov       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0x487 0x60
    mov       ch, BDA_VDDA_VGA OR BDA_VDDA_LineMode400;0x11
    test      al, 40h
    jne       IsColor                   ;Offset 0x18b
    or        ch, BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;0x6
IsColor:                                ;Offset 0x18b
    lea       bx, [VideoOverrideTable1] ;Offset 0x3d0
    call      SetupVideoParameterControlBlock;Offset 0x1a3
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], ch;Offset 0x489
    call      SetCardAddressOrFallbackToSystemAdapter;Offset 0x1c0
    call      SetupEGAFeatureBitSwitches;Offset 0x224
    call      SetTextMode               ;Offset 0x24f
    call      SetDisplayCombinationCode ;Offset 0x29c
    ret
SetupInterruptsTextModeAndBDA ENDP

SetupVideoParameterControlBlock PROC NEAR;Offset 0x1a3
    mov       ax, cs
    cmp       ax, word ptr cs:[VideoOverrideTable2.VideoParameterTable.sgm];Offset 0x424
    jne       IsShadowed                ;Offset 0x1b0
    ;add       bx, (offset VideoOverrideTable2) - (offset VideoOverrideTable1);0x52
    DB 081h, 0C3h, 052h, 000h           ;!!!!!!!!!!!!!! Assembles to different variant.
IsShadowed:                             ;Offset 0x1b0
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], bx;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], ax;Offset 0x4aa
    ret
SetupVideoParameterControlBlock ENDP

;Offset 0x1b8
InitMDA                 DB VGA_Misc_IOAddressSel_MDA, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Color, BDA_DM_80x25_16_Color_Text;0x0, 0xD4, 0x20, 0x03

;Offset 0x1bc
InitCGA                 DB VGA_Misc_IOAddressSel_CGA, VGA_CRTControllerIndexB_lowbyte, BDA_DH_80x25Monochrome, BDA_DM_80x25_Monochrome_Text;0x1, 0xB4, 0x30, 0x07

SetCardAddressOrFallbackToSystemAdapter PROC NEAR;Offset 0x1c0
    lea       si, [InitMDA]             ;Offset 0x1b8
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    call      SetCardMode               ;Offset 0x1db   Attempt to set card to Monochrome
    jae       Success                   ;Offset 0x1da
    lea       si, [InitCGA]             ;Offset 0x1bc
    and       byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487 0xfd
    call      SetCardMode               ;Offset 0x1db   Set card to Color
Success:                                ;Offset 0x1da
    ret
SetCardAddressOrFallbackToSystemAdapter ENDP

SetCardMode PROC NEAR                   ;Offset 0x1db
    mov       dx, VGA_MiscellaneousWrite;Port 0x3c2
    lodsb     byte ptr cs:[si]          ;Load MDA / CGA config bit
    out       dx, al                    ;Select MDA or CGA
    lodsb     byte ptr cs:[si]          ;Load CRTController Index port lower byte
    mov       dl, al                    ;dl = 0xd4 or 0xb4
    mov       ax, dx                    ;ax = 0x3d4 or 0x3b4
    xor       al, 60h                   ;al = 0xd4 xor 0x60 = 0xb4 or 0xb4 xor 0x60 = 0xd4
    mov       word ptr ds:[BDA_VideoBaseIOPort], ax;Offset 0x463
    mov       al, VGA_CRTCIdx_CursorLocLow;0xf  Not sure if this is correct
    out       dx, al
    inc       dx                        ;0x3?5 - CRTC Data Register
    mov       ah, 0e5h
    call      TestRegister              ;Offset 0x214 - If non-selected CRTC register is still writeable, it's a VGA card
    jne       IsVGA                     ;Offset 0x213
    mov       ah, 1ah
    call      TestRegister              ;Offset 0x214 - Make sure
    jne       IsVGA                     ;Offset 0x213
    lodsb     byte ptr cs:[si]          ;Card is not VGA.
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    lodsb     byte ptr cs:[si]          ;Load fallback mode
    cbw                                 ;zero out ah
    int       42h                       ;Call System interrupt handler to set video mode.
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
IsVGA:                                  ;Offset 0x213
    ret
SetCardMode ENDP

;input:
;   dx = Port
;   ah = Test value
;
;output:
;   zero flag = 0 - unequal, 1 - equal
;   carry flag = 1
;
;destroys:
; al, bl, flags
;
TestRegister PROC NEAR                  ;Offset 0x214
    in        al, dx                    ;Read register
    mov       bl, al                    ;Store result in bl
    mov       al, ah                    ;Move test value to al
    out       dx, al                    ;Write test value to register
    jmp       Delay                     ;Offset 0x21c   !!!!!!Speed sensitive!
Delay:                                  ;Offset 0x21c
    in        al, dx                    ;Read back test value
    ;cmp       al, ah                    ;Compare read test value to original test value
    DB 03Ah, 0C4h                       ;!!!!!! Misassembled
    mov       al, bl                    ;Move original register value to al
    out       dx, al                    ;Restore original value back to register
    stc                                 ;Set carry flag
    ret
TestRegister ENDP

SetupEGAFeatureBitSwitches PROC NEAR    ;Offset 0x224
    mov       al, VGA_FC_Bit0           ;0x1
    call      ReadFeatureInputBits      ;Offset 0x240
    shr       al, 01h
    mov       ah, al
    mov       al, VGA_FC_Bit1           ;0x2
    call      ReadFeatureInputBits      ;Offset 0x240
    shl       al, 01h
    or        al, ah
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], NOT BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0x488
    ret
SetupEGAFeatureBitSwitches ENDP

ReadFeatureInputBits PROC NEAR          ;Offset 0x240
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a - VGA_FeatureControlW
    out       dx, al
    mov       dx, VGA_InputStatus0      ;Port 0x3c2
    in        al, dx
    and       al, VGA_IS_FeatureInputs01;0x60
    ret
ReadFeatureInputBits ENDP

SetTextMode PROC NEAR                   ;Offset 0x24f
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    xor       al, al                    ;zero al
    out       dx, al                    ;select color 0
    inc       dx                        ;Port 0x3c9
    mov       cx, 300h                  ;768 values
LoopColors:                             ;Offset 0x259
    out       dx, al                    ;Zero all colors
    loop      LoopColors                ;Offset 0x259
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    cmp       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    mov       bl, al                    ;store modified vdda
    je        SetMonochrome             ;Offset 0x273
    mov       ax, (BDA_DH_80x25Color SHL 8) OR BDA_DM_80x25_16_Color_Text;0x2003
    mov       bh, BDA_EFBS_MDAHiResEnhanced_2;0x9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        SetColor                  ;Offset 0x27d
SetMonochrome:                          ;Offset 0x273
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    mov       ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_DM_80x25_Monochrome_Text;0x3007
    mov       bh, BDA_EFBS_CGAMono80x25_2;0xb
SetColor:                               ;Offset 0x27d
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bh;Offset 0x488
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], ah;Offset 0x410
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
    cbw                                 ;zero out ah
    int       10h                       ;set video mode
    or        byte ptr ds:[BDA_VideoDisplayDataArea], bl;Offset 0x489
    ret
SetTextMode ENDP

SetDisplayCombinationCode PROC NEAR     ;Offset 0x29c
    mov   ax, BDA_DCC_CVGA SHL 8        ;0x800
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsColor                       ;Offset 0x2a8
    mov   ah, BDA_DCC_MVGA              ;0x7
IsColor:                                ;Offset 0x2a8
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    jne   IsVGA                         ;Offset 0x2bf
    inc   ax                            ;BDA_DCC_MDPA
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsVGA                         ;Offset 0x2bf
    inc   ax                            ;BDA_DCC_CGA
    call  CheckRegistersMemoryMapped    ;Offset 0x2c5
    jne   IsVGA                         ;Offset 0x2bf
    mov   al, BDA_DCC_PGC               ;0x6
IsVGA:                                  ;Offset 0x2bf
    mov   bx, ax
    call  SetDisplayCombinationCodeIndex;Offset 0x3120
    ret
SetDisplayCombinationCode ENDP

;
;input:
;   -
;output
;   zero flag = 0 = failed, 1 = success
;destroys:
;   dx, si
CheckRegistersMemoryMapped PROC NEAR    ;Offset 0x2c5
    push  ax
    push  ds
    mov   ax, 0c600h                    ;Segment 0xc600
    mov   ds, ax
    mov   si, VGA_CRTControllerIndexD   ;Offset / Port 0x3d4
    mov   ah, 28h
    xchg  byte ptr [si], ah
    mov   dx, si
    in    al, dx
    mov   byte ptr [si], ah
    cmp   al, 28h
    pop   ds
    pop   ax
    ret
CheckRegistersMemoryMapped ENDP

;Offset 0x2df
StartupSplash           DB 00Dh, 00Ah
                        DB 'MATROX POWER GRAPHICS ACCELERATOR', 00Dh, 00Ah
                        DB 'MGA Series', 00Dh, 00Ah
                        DB 'VGA/VBE BIOS, Version V1.8 ', 00Dh, 00Ah
                        DB 'Copyright (C) 1998, Matrox Graphics Inc.', 00Dh, 00Ah
                        DB 'Copyright (C) LSI Logic Corporation 1990-1991', 00Dh, 00Ah
                        DB 00Dh, 00Ah, 000h

;Offset 0x387
Data0x387               DB 000h

PrintStartupSplash PROC NEAR            ;Offset 0x388
    test  byte ptr cs:[Data0x387], 01h  ;Offset 0x387
    je    Return                        ;Offset 0x3ad
    cmp   word ptr ds:[BDA_SoftResetFlag], 1234h;Offset 0x472
    je    Return                        ;Offset 0x3ad
    lea   si, [StartupSplash]           ;Offset 0x2dd
NextCharacter:                          ;Offset 0x39c
    lodsb byte ptr cs:[si]
    or    al, al
    je    EndOfString                   ;Offset 0x3a8
    mov   ah, 0eh                       ;Teletype Output
    int   10h
    jmp   NextCharacter                 ;Offset 0x39c
EndOfString:                            ;Offset 0x3a8
    mov   al, 0fah
    call  Sleep2                        ;Offset 0x31f0
Return:                                 ;Offset 0x3ad
    ret
PrintStartupSplash ENDP

;
;inputs:
;   al = index
;
;outputs:
;   al = value
;
;destroys:
;   -
;
;note: If register 0x7 (Requester Control) is read, value is or-ed with 0xf
;
MGAReadCRTCExtensionRegister PROC NEAR  ;Offset 0x3ae
    push  dx
    push  bx
    mov   bl, al
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, al
    inc   dx
    in    al, dx
    pushf
    cmp   bl, 07h                       ;MGA_CRTCExt_RequesterControl doesn't exist on the Mystique?
    jne   NotRequesterControl           ;Offset 0x3c0
    or    al, 0fh
NotRequesterControl:                    ;Offset 0x3c0
    popf
    pop   bx
    pop   dx
    ret
MGAReadCRTCExtensionRegister ENDP

;
;inputs:
;   al = value
;   ah = index
;
;outputs:
;   -
;
;destroys:
;   -
;
MGAWriteCRTCExtensionRegister PROC NEAR ;Offset 0x3c4
    push  dx
    xchg  al, ah
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, ax
    xchg  al, ah
    pop   dx
    ret
MGAWriteCRTCExtensionRegister ENDP

    nop                                 ;hello

;Offset 0x3d0
VideoOverrideTable1 VideoOverridePointerTable < <offset VideoParameters, 0C000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable1, 0C000h>, <00000h, 00000h>, <00000h, 00000h> >

;Offset 0x3ec
                        DW 00C20h, 0C000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h  

;Offset 0x408
SecondarySaveTable1 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0C000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >

;Offset 0x422
VideoOverrideTable2 VideoOverridePointerTable < <offset VideoParameters, 0E000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable2, 0E000h>, <00000h, 00000h>, <00000h, 00000h> >

;Offset 0x43e
                        DW 00C20h, 0E000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h
                        DW 00000h, 00000h

;Offset 0x45a
SecondarySaveTable2 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0E000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >

;Offset 0x474
DisplayCominationCodes DisplayCombinationCodeTable < 010h, 001h, 008h, 000h >
                        DB BDA_DCC_NoDisplay, BDA_DCC_NoDisplay ;  0) No displays
                        DB BDA_DCC_NoDisplay, BDA_DCC_MDPA      ;  1) Monochrome display and printer adapter (MDPA)
                        DB BDA_DCC_NoDisplay, BDA_DCC_CGA       ;  2) Color/graphics monitor adapter (CGA)
                        DB BDA_DCC_CGA, BDA_DCC_MDPA            ;  3) MDPA + CGA
                        DB BDA_DCC_NoDisplay, BDA_DCC_EGA       ;  4) Enhanced Graphics Adapter (EGA)
                        DB BDA_DCC_EGA, BDA_DCC_MDPA            ;  5) EGA + MDPA
                        DB BDA_DCC_NoDisplay, BDA_DCC_MEGA      ;  6) EGA with monochrome display (MEGA)
                        DB BDA_DCC_CGA, BDA_DCC_MEGA            ;  7) MEGA + CGA
                        DB BDA_DCC_NoDisplay, BDA_DCC_PGC       ;  8) Professional graphics controller (PGC)
                        DB BDA_DCC_MDPA, BDA_DCC_PGC            ;  9) PGC + MDPA
                        DB BDA_DCC_MEGA, BDA_DCC_PGC            ; 10) PGC + MEGA
                        DB BDA_DCC_NoDisplay, BDA_DCC_CVGA      ; 11) Video graphics Array (VGA) based with color display (CVGA)
                        DB BDA_DCC_MDPA, BDA_DCC_CVGA           ; 12) CVGA + MDPA
                        DB BDA_DCC_NoDisplay, BDA_DCC_MVGA      ; 13) VGA based with monochrome display (MVGA)
                        DB BDA_DCC_CGA, BDA_DCC_MVGA            ; 14) MVGA + CGA
                        DB BDA_DCC_PGC, BDA_DCC_MVGA            ; 15) MVGA + PGC

;Offset 0x498
StaticFunctionalityTable DB 0FFh        ;Modes supported 1: bit 0 to bit 7 = 1 modes 0,1,2,3,4,5,6,7 supported
                        DB 0E0h         ;Modes supported 2: bit 0 to bit 7 = 1 modes 0Dh,0Eh,0Fh supported - 8,9,0Ah,0Bh,0Ch not supported
                        DB 00Fh         ;Modes supported 3: bit 0 to bit 3 = 1 modes 10h,11h,12h,13h supported - bit 4 to bit 7 unused (0)
                        DB 000h         ;(IBM) Reserved
                        DB 000h         ;(IBM) Reserved
                        DB 000h         ;(IBM) Reserved
                        DB 000h         ;(IBM) Reserved
                        DB 007h         ;Scan lines supported: bit 0 to bit 2 = 1 if scan lines 200,350,400 supported - bits 3-7 = unused (0)
                        DB 002h         ;Total number of character blocks available in text modes
                        DB 008h         ;Maximum number of active character blocks in text modes
                        DW 00EFFh       ;Miscellaneous function support flags:
                                        ;bit 0      All modes on all displays function supported
                                        ;bit 1      Gray summing function supported
                                        ;bit 2      Character font loading function supported
                                        ;bit 3      Default palette loading enable/disable supported
                                        ;bit 4      Cursor emulation function supported
                                        ;bit 5      EGA palette present
                                        ;bit 6      Color palette present
                                        ;bit 7      Color-register paging function supported
                                        ;bit 8      - Light pen not supported
                                        ;bit 9      Save/restore state function 1Ch supported
                                        ;bit 10     Intensity/blinking function supported (see AX=1003h)
                                        ;bit 11     Display Combination Code supported (see #00039)
                                        ;bits 12-15 Unused (0)
                        DW 00000h       ;Reserved
                        DB 03Fh         ;Save pointer function flags:
                                        ;bit 0      512 character set supported
                                        ;bit 1      Dynamic save area supported
                                        ;bit 2      Alpha font override supported
                                        ;bit 3      Graphics font override supported
                                        ;bit 4      Palette override supported
                                        ;bit 5      DCC extension supported
                                        ;bits 6-7   Unused (0)
                        DB 000h         ;Reserved

;Offset 0x4a8
Data0x4a8               DB BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2c
                        DB BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x28
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2d
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2a
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2e
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_GraphicsOperation;0x1e
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29

;Offset 0x4b0
                        DB 006h, 000h
                        DB 006h, 000h
                        DB 007h, 000h
                        DB 007h, 000h
                        DB 005h, 000h
                        DB 005h, 000h
                        DB 004h, 000h
                        DB 005h, 000h
                        DB 000h, 000h
                        DB 000h, 000h
                        DB 000h, 000h
                        DB 000h, 000h
                        DB 000h, 000h
                        DB 005h, 000h
                        DB 006h, 000h
                        DB 004h, 000h
                        DB 004h, 000h
                        DB 000h, 000h
                        DB 000h, 000h
                        DB 000h, 000h

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

;The assembler doesn't like all that data on one line, so I am just overlapping these to make struct accesses work.
;If anyone knows a cleaner way, please let me know
VideoParameters VideoParametersTable 29 dup(<>);Offset 0x4c0

ORG offset VideoParameters
;Offset 0xc4e0

                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x520
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x560
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x5a0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x5e0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x620
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x660
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000101h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0C2h, 0FFh
                        DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

;Offset 0x6a0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000300h
                        DB 0A6h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0x6e0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 07D00h
                        DD 006000F21h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x720
                        DB 000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 000000000h
                        DB 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x760
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x7a0
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 062h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x7e0
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x820
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 006000F09h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x860
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x8a0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 000h, 05Eh, 06Eh, 08Bh, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x8e0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
                        DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x920
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A2h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh

;Offset 0x960
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x9a0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x9e0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa20
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa60
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xaa0
                        DB 028h
                        DB 018h
                        DB 010h
                        DW 00800h
                        DD 002000308h
                        DB 067h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xae0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 067h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xb20
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 066h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0xb60
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0C3h, 0FFh
                        DB 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh

;Offset 0xba0
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xbe0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 00E000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0xc20
Data0xc20               DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 00300030Bh
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xc60
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 00300030Bh
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xca0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 003000301h
                        DB 023h
                        DB 070h, 04Fh, 05Ch, 02Fh, 05Fh, 007h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 028h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xce0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 003000301h
                        DB 023h
                        DB 070h, 04Fh, 05Ch, 02Fh, 05Fh, 007h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 028h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xd20
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 00200030Bh
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 030h, 014h, 004h, 011h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 000h, 0E0h, 0F0h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0xd60
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 00200030Bh
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 030h, 014h, 004h, 011h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 000h, 0E0h, 0F0h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0xda0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000101h
                        DB 023h
                        DB 070h, 04Fh, 059h, 02Dh, 05Eh, 006h, 004h, 011h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 0E0h, 023h, 0C7h, 028h, 000h, 0DFh, 0EFh, 0C2h, 0FFh
                        DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

;Offset 0xde0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000300h
                        DB 0A6h
                        DB 060h, 04Fh, 056h, 03Ah, 051h, 060h, 070h, 01Fh, 000h, 00Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 028h, 00Dh, 05Eh, 06Eh, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0xe20
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xe60
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xea0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xee0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 007000401h
                        DB 023h
                        DB 070h, 04Fh, 05Ch, 02Fh, 05Fh, 007h, 004h, 011h, 000h, 007h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 028h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 0FFh

;Offset 0xf20
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 007000400h
                        DB 0A6h
                        DB 060h, 04Fh, 056h, 03Ah, 051h, 060h, 070h, 01Fh, 000h, 00Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 028h, 00Dh, 05Eh, 06Eh, 0A3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 0FFh

;Offset 0xf60
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 006000F0Bh
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 030h, 014h, 004h, 011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 000h, 0E0h, 0F0h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xfa0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000F01h
                        DB 023h
                        DB 070h, 04Fh, 059h, 02Dh, 05Eh, 006h, 004h, 011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0E0h, 023h, 0C7h, 028h, 000h, 0DFh, 0EFh, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xfe0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 00Dh, 05Eh, 06Eh, 08Bh, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x1020
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
                        DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x1060
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 03Ah, 050h, 060h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 028h, 00Dh, 05Eh, 06Eh, 0E3h, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x10a0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 037h, 052h, 000h, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 028h, 00Fh, 05Fh, 00Ah, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x10e0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 00300030Bh
                        DB 0A7h
                        DB 02Dh, 027h, 02Bh, 02Dh, 028h, 06Dh, 06Ch, 01Fh, 000h, 00Dh, 006h, 007h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Eh, 00Ah, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x1120
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 00300030Bh
                        DB 0A7h
                        DB 02Dh, 027h, 02Bh, 02Dh, 028h, 06Dh, 06Ch, 01Fh, 000h, 00Dh, 006h, 007h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Eh, 00Ah, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x1160
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000301h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 037h, 051h, 05Bh, 06Ch, 01Fh, 000h, 00Dh, 006h, 007h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 028h, 00Fh, 05Eh, 00Ah, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x11a0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000301h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 037h, 051h, 05Bh, 06Ch, 01Fh, 000h, 00Dh, 006h, 007h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 028h, 00Fh, 05Eh, 00Ah, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x11e0
Int10CallTable          DW SetVideoMode;Offset 0x12f0                                       ;  0
                        DW SetTextModeCursorShape;Offset 0x1684                             ;  1
                        DW SetCursorPosition;Offset 0x1700                                  ;  2
                        DW GetCursorPositionAndSize;Offset 0x1734                           ;  3
                        DW ReadLightPenPosition_SystemForward;Offset 0x1744                 ;  4
                        DW SelectActiveDisplayPage;Offset 0x1751                            ;  5
                        DW ScrollUpWindow;Offset 0x1787                                     ;  6
                        DW ScrollDownWindow;Offset 0x19ac                                   ;  7
                        DW ReadCharacterAndAttributeAtCursorPosition;Offset 0x1be9          ;  8
                        DW WriteCharacterAndAttributeAtCursorPosition;Offset 0x1db6         ;  9
                        DW WriteCharacterOnlyAtCursorPosition;Offset 0x20bf                 ;  a
                        DW SetBorderColorOrPalette;Offset 0x2120                            ;  b
                        DW WriteGraphicsPixel;Offset 0x21c9                                 ;  c
                        DW ReadGraphicsPixel;Offset 0x22a6                                  ;  d
                        DW TeletypeOutput;Offset 0x235c                                     ;  e
                        DW GetCurrentVideoMode;Offset 0x23e3                                ;  f
                        DW RAMDACFunctions;Offset 0x23f5                                    ; 10
                        DW TextFunctions;Offset 0x25e5                                      ; 11
                        DW AlternateFunctions;Offset 0x26e9                                 ; 12
                        DW WriteString  ;Offset 0x2876                                      ; 13
                        DW NotImplemented;Offset 0x2902                                     ; 14
                        DW NotImplemented;Offset 0x2902                                     ; 15
                        DW NotImplemented;Offset 0x2902                                     ; 16
                        DW NotImplemented;Offset 0x2902                                     ; 17
                        DW NotImplemented;Offset 0x2902                                     ; 18
                        DW NotImplemented;Offset 0x2902                                     ; 19
                        DW DisplayCombinationCodeFunctions;Offset 0x2903                    ; 1a
                        DW FunctionalityAndStateInfo;Offset 0x291b                          ; 1b
                        DW SaveRestoreVideoState;Offset 0x2a28                              ; 1c

;Offset 0x121a
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 050h, 012h, 000h, 000h, 0FFh, 0FFh, 074h, 004h, 098h, 004h, 000h, 02Ah, 014h, 02Ah, 004h, 030h
                        DB 080h, 030h, 0EEh, 032h, 000h, 000h

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

Int10Handler:                           ;Offset 0x1240
    cmp  ax, 4f10h
    jne  NotPowerManagement             ;Offset 0x124a
    sti
    cld
    jmp  VESAPowerManagement            ;Offset 0x5082
NotPowerManagement:                     ;Offset 0x124a
    int  6dh
    iret

    xchg bx, bx                         ;hello
    nop

Int6DHandler:                           ;Offset 0x1250
    sti
    cld
    cmp  ax, 4f10h
    jne  NotVESAPowerManagement         ;Offset 0x125a
    jmp  VESAPowerManagement            ;Offset 0x5082
NotVESAPowerManagement:                 ;Offset 0x125a
    cmp  ax, 4f15h
    jne  NotVESADDC                     ;Offset 0x1262
    jmp  VESADDC                        ;Offset 0x514e
NotVESADDC:                             ;Offset 0x1262
    cmp  ah, 1ch
    ja   Label0x12ba                    ;Offset 0x12ba
    push ds
    push si
    push di
    xor  si, si
    mov  ds, si
    mov  si, ax
    mov  al, ah
    cbw
    shl  ax, 01h
    xchg ax, si
    cmp  byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    jl   Label0x12b1                    ;Offset 0x12b1
    cmp  ah, 0eh
    je   Label0x128c                    ;Offset 0x128c
    cmp  ah, 06h
    jl   Label0x12b1                    ;Offset 0x12b1
    cmp  ah, 0ah
    jg   Label0x12b1                    ;Offset 0x12b1
Label0x128c:                            ;Offset 0x128c
    push ax
    push dx
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    mov  al, VGA_SEQIdx_MemoryMode      ;0x4
    out  dx, al
    xchg al, ah
    inc  dx
    in   al, dx
    xchg al, ah
    mov  di, ax
    pop  dx
    pop  ax
    push di
    call word ptr cs:[si + Int10CallTable];Offset 0x11e0
    pop  di
    push ax
    push dx
    mov  ax, di
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    out  dx, ax
    pop  dx
    pop  ax
    jmp  Label0x12b6                    ;Offset 0x12b6
Label0x12b1:                            ;Offset 0x12b1
    call word ptr cs:[si + Int10CallTable];Offset 0x11e0
Label0x12b6:                            ;Offset 0x12b6
    pop  di
    pop  si
    pop  ds
    iret
Label0x12ba:                            ;Offset 0x12ba
    cmp  ah, 4fh
    jne  Label0x12c2                    ;Offset 0x12c2
    jmp  VESAHandler                    ;Offset 0x707e
Label0x12c2:                            ;Offset 0x12c2
    int  42h
    iret

Func0x12c5 PROC NEAR                    ;Offset 0x12c5
    push ax
    push bx
    mov  ax, cs
    mov  bx, ds
    cmp  ax, bx
    pop  bx
    pop  ax
    jne  Label0x12ee                    ;Offset 0x12ee
    cmp  cl, 0eh
    jne  Label0x12ee                    ;Offset 0x12ee
    cmp  al, 67h
    je   Label0x12ec                    ;Offset 0x12ec
    cmp  al, 6ah
    je   Label0x12ec                    ;Offset 0x12ec
    cmp  al, 79h
    je   Label0x12ec                    ;Offset 0x12ec
    cmp  al, 71h
    je   Label0x12ec                    ;Offset 0x12ec
    cmp  al, 70h
    je   Label0x12ec                    ;Offset 0x12ec
    cmp  al, 98h
Label0x12ec:                            ;Offset 0x12ec
    clc
    ret
Label0x12ee:                            ;Offset 0x12ee
    stc
    ret
Func0x12c5 ENDP

;inputs:
;   al = video mode
;
SetVideoMode PROC NEAR                  ;Offset 0x12f0
    push ax
    mov  ah, al
    mov  al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and  al, BDA_VDDA_Reserved          ;0x20
    je   Label0x12fd                    ;Offset 0x12fd
    or   ah, BDA_DM_DONT_CLEAR_SCREEN   ;0x80
Label0x12fd:                            ;Offset 0x12fd
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shl  ax, 01h                        ;Don't clear screen bit is now in carry flag
    xchg al, ah                         ;al = mode, ah = video mode options
    rcr  ax, 01h                        ;al no longer has clear screen bit, ah dont clear screen bit replaced.
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    ja   Label0x136a                    ;Offset 0x136a
    push ax
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x7
    je   Label0x131c                    ;Offset 0x131c
    cmp  al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je   Label0x131c                    ;Offset 0x131c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Color          ;0x20
    jmp  Label0x1323                    ;Offset 0x1323
Label0x131c:                            ;Offset 0x131c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Monochrome     ;0x30
Label0x1323:                            ;Offset 0x1323
    mov  byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    pop  ax
    call Func0x149c                     ;Offset 0x149c
    mov  word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3720
    mov  word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    and  ah, 0f3h
    test byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je   Label0x1340                    ;Offset 0x1340
    jp   Func0x13ba                     ;Offset 0x13ba
Label0x1340:                            ;Offset 0x1340
    test ah, 02h
    jne  Label0x1354                    ;Offset 0x1354
    mov  si, 3d4h
    cmp  al, 07h
    je   Label0x1350                    ;Offset 0x1350
    cmp  al, 0fh
    jne  Func0x13cf                     ;Offset 0x13cf
Label0x1350:                            ;Offset 0x1350
    mov  al, 03h
    jmp  Func0x13cc                     ;Offset 0x13cc
Label0x1354:                            ;Offset 0x1354
    cmp  al, 02h
    jb   Label0x135f                    ;Offset 0x135f
    cmp  al, 03h
    ja   Label0x135f                    ;Offset 0x135f
    or   ah, 04h
Label0x135f:                            ;Offset 0x135f
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485 0x8
    call Func0x13aa                     ;Offset 0x13aa
Label0x1368:                            ;Offset 0x1368
    pop  ax
    ret
Label0x136a:                            ;Offset 0x136a
    cmp  al, 6ah
    je   Label0x138d                    ;Offset 0x138d
    mov  ah, al
    and  ah, 0c0h
    or   ah, 01h
    and  al, 3fh
    cmp  al, 20h
    jb   Label0x1368                    ;Offset 0x1368
    cmp  al, 3fh
    ja   Label0x1368                    ;Offset 0x1368
    sub  al, 20h
    push bx
    mov  bx, ax
    mov  ax, 04f02h
    int  10h
    pop  bx
    pop  ax
    ret
Label0x138d:                            ;Offset 0x138d
    pop  ax
    push bx
    mov  ax, 4f02h
    mov  bx, 0102h
    int  10h
    pop  bx
    ret
SetVideoMode ENDP

Func0x1399 PROC NEAR                    ;Offset 0x1399
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 0xe
    call Func0x13aa                     ;Offset 0x13aa
    mov  word ptr ds:[BDA_CursorEndStartScanLine], 0b0ch;Offset 0x460
    pop  ax
    ret
Func0x1399 ENDP

Func0x13aa PROC NEAR                    ;Offset 0x13aa
    mov  byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484 0x18
    or   ah, BDA_VMO_Inactive           ;0x8
    mov  byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    cbw
    int  42h
    ret
Func0x13aa ENDP

;
;inputs:
;   al = display mode
;   ah = video mode options
;   si = video base io port
;
;
Func0x13ba PROC NEAR                    ;Offset 0x13ba
    test      ah, BDA_VMO_Monochrome    ;0x2
    je        Func0x1399                ;Ofset 0x1399
    mov       si, VGA_CRTControllerIndexB;Port 0x3b4
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Func0x13cf                ;Offset 0x13cf
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Func0x13cf                ;Offset 0x13cf
    mov       al, BDA_DM_80x25_Monochrome_Text;0x7
Func0x13ba ENDP
;continue!
Func0x13cc PROC NEAR                    ;Offset 0x13cc
    and       ah, NOT BDA_VMO_DontClearDisplay;0x7f
Func0x13cc ENDP
;continue!
Func0x13cf PROC NEAR                    ;Offset 0x13cf
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    push      ax
    call      TurnScreenOff             ;Offset 0x3109
    mov       dx, ax
    pop       ax
    push      dx
    mov       byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov       byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    mov       word ptr ds:[BDA_VideoBaseIOPort], si;Offset 0x463
    push      ax
    push      dx
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, al
    inc       dx
    in        al, dx
    or        al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    out       dx, al
    pop       dx
    pop       ax
    call      Func0x1499                ;Offset 0x1499
    xor       ax, ax
    mov       es, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    lea       di, ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       cx, 0008h
    rep stosw
    call      Func0x2fe4                ;Offset 0x2fe4
    xor       ax, ax
    lodsb     byte ptr es:[si]
    mov       word ptr ds:[BDA_NumberOfScreenColumns], ax;Offset 0x44a
    lodsb     byte ptr es:[si]
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsb     byte ptr es:[si]
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw     word ptr es:[si]
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    mov       ax, word ptr es:[si + 0fh]
    xchg      al, ah
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call      Func0x2e39                ;Offset 0x2e39
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
    mov       al, VGA_ATTR_ColorSelect  ;0x14
    out       dx, al
    xor       al, al
    out       dx, al
    push      ax
    push      dx
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, al
    inc       dx
    in        al, dx
    and       al, NOT (MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff);0xcf
    out       dx, al
    pop       dx
    pop       ax
    call      Func0x152a                ;Offset 0x152a
    call      ClearScreen               ;Offset 0x2ca5
    lea       si, [Func0x1549]          ;Offset 0x1549
    call      Func0x30a2                ;Offset 0x30a2
    je        Label0x145c               ;Offset 0x145c
    lea       si, [Func0x1574]          ;Offset 0x1574
Label0x145c:                            ;Offset 0x145c
    call      si
    call      Func0x1597                ;Offset 0x1597
    call      Func0x30c9                ;Offset 0x30c9
    pop       ax
    call      TurnScreenOn              ;Offset 0x3105
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x1486               ;Offset 0x1486
    mov       ah, al
    lea       bx, [Data0x4a8]           ;Offset 0x4a8
    xlatb     cs:[bx]
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    mov       al, 30h
    cmp       ah, BDA_DM_640x200_BW_Graphics;0x6
    jne       Label0x1483               ;Offset 0x1483
    mov       al, 3fh
Label0x1483:                            ;Offset 0x1483
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
Label0x1486:                            ;Offset 0x1486
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_Reserved     ;0x20
    je        Label0x1492               ;Offset 0x1492
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_Reserved;Offset 0x489 0xdf
Label0x1492:                            ;Offset 0x1492
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0x13cf ENDP

Func0x1499 PROC NEAR                    ;Offset 0x1499
    call      Func0x6257                ;Offset 0x6257
Func0x1499 ENDP
;continue!
Func0x149c PROC NEAR                    ;Offset 0x149c
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    je        Label0x151f               ;Offset 0x151f
    cmp       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je        Label0x151f               ;Offset 0x151f
    push      bx
    push      dx
    lea       si, [Data0x1520]          ;Offset 0x1520
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0x14bb               ;Offset 0x14bb
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0x14bb               ;Offset 0x14bb
    ;add       si, 0005h
    DB 81h, 0C6h, 05h, 00h              ;!!!!!!!!!!!!! misassembly
Label0x14bb:                            ;Offset 0x14bb
    xchg      ax, bx
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       al, byte ptr cs:[si + 01h]
    je        Label0x151c               ;Offset 0x151c
    mov       ah, bl
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    mov       bl, BDA_DM_80x25_Monochrome_Text;0x7
    cmp       al, byte ptr cs:[si + 02h]
    je        Label0x151c               ;Offset 0x151c
    mov       bl, BDA_DM_80x25_16_Color_Text;0x3
    cmp       al, byte ptr cs:[si + 03h]
    js        Label0x151c               ;Offset 0x151c
    mov       bl, ah
    and       bh, 0fdh
    or        bh, byte ptr cs:[si + 04h]
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_AdapterTypeMask;0xf
    sub       al, BDA_EFBS_MDAColor40x25_2;0x6
    cmp       al, BDA_EFBS_CGAMono80x25 ;0x5
    ja        Label0x151c               ;Offset 0x151c
    sub       al, BDA_EFBS_MDAHiResEnhanced;0x3
    cbw
    mov       al, BDA_EFBS_CGAMono80x25_2;0xb
    jbe       Label0x14f9               ;Offset 0x14f9
    mov       al, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0x14f9:                            ;Offset 0x14f9
    mov       dl, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    mov       dh, dl
    and       dx, 807fh
    rol       dh, 01h
    and       dh, byte ptr cs:[si]
    sub       al, dh
    and       ah, byte ptr cs:[si]
    or        dl, ah
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0x488
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], dl;Offset 0x489
Label0x151c:                            ;Offset 0x151c
    xchg      ax, bx
    pop       dx
    pop       bx
Label0x151f:                            ;Offset 0x151f
    ret
Func0x149c ENDP

;Offset 0x1520
Data0x1520              DB 080h, VGA_CRTControllerIndexB_lowbyte, 0FFh, BDA_DH_80x25Monochrome, 002h;0x80 0xB4 0xFF 0x30 0x02

;Offset 0x1525
Data0x1525              DB 001h, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Monochrome, 000h, 000h;0x01 0xD4 0x30 0x00 0x00

Func0x152a PROC NEAR                    ;Offset 0x152a
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   PaletteLoadingDisabled        ;Offset 0x1548
    push  si
    xor   ax, ax
    mov   cx, 0010h
    call  Func0x2eb9                    ;Offset 0x2eb9
    inc   ax
    inc   si
    mov   cl, 01h
    call  Func0x2eb9                    ;Offset 0x2eb9
    pop   si
    call  Func0x2eff                    ;Offset 0x2eff
    call  Func0x2f14                    ;Offset 0x2f14
PaletteLoadingDisabled:                 ;Offset 0x1548
    ret
Func0x152a ENDP

Func0x1549 PROC NEAR                    ;Offset 0x1549
    call  Func0x2ce3                    ;Offset 0x2ce3
    call  GetNumberOfActiveScanlines    ;Offset 0x3054
    dec   al
    mov   ax, 0002h
    js    Label0x1565                   ;Offset 0x1565
    mov   al, 03h
    jne   Label0x1563                   ;Offset 0x1563
    mov   al, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne   Label0x1565                   ;Offset 0x1565
Label0x1563:                            ;Offset 0x1563
    mov   ah, 80h
Label0x1565:                            ;Offset 0x1565
    mov   bl, ah
    call  Func0x2d24                    ;Offset 0x2d24
    call  Func0x1612                    ;Offset 0x1612
    call  Func0x1645                    ;Offset 0x1645
    call  Func0x2ce9                    ;Offset 0x2ce9
    ret
Func0x1549 ENDP

Func0x1574 PROC NEAR                    ;Offset 0x1574
    call  GetNumberOfActiveScanlines    ;Offset 0x3054
    dec   al
    js    Label0x158e                   ;Offset 0x158e
    lea   ax, [Font8x16]                ;Offset 0x3f20
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    mov   ax, cs
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
    jne   Label0x158e                   ;Offset 0x158e
    mov   ax, 0bd70h
    int   6dh
Label0x158e:                            ;Offset 0x158e
    xor   ax, ax
    mov   word ptr ds:[BDA_CursorEndScanLine], ax;Offset 0x460
    call  Func0x15ee                    ;Offset 0x15ee
    ret
Func0x1574 ENDP

Func0x1597 PROC NEAR                    ;Offset 0x1597
    mov   al, BDA_VPCB_PaletterProfileTable;0xa
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    Label0x15ed                   ;Offset 0x15ed
    mov   al, 14h
    call  Func0x30b5                    ;Offset 0x30b5
    jne   Label0x15ed                   ;Offset 0x15ed
    lodsb byte ptr es:[si]
    or    al, al
    je    Label0x15bb                   ;Offset 0x15bb
    mov   ax, 1420h
    js    Label0x15b3                   ;Offset 0x15b3
    mov   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
Label0x15b3:                            ;Offset 0x15b3
    dec   ax
    xchg  al, ah
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    out   dx, ax
Label0x15bb:                            ;Offset 0x15bb
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   Label0x15ed                   ;Offset 0x15ed
    push  es
    push  si
    add   si, 0003h
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0x15d6                   ;Offset 0x15d6
    mov   ah, 10h
    call  Func0x2ebb                    ;Offset 0x2ebb
Label0x15d6:                            ;Offset 0x15d6
    pop   si
    pop   es
    add   si, 000bh
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0x15ed                   ;Offset 0x15ed
    mov   dx, VGA_DACWriteIndex         ;Port 0x3c8
    out   dx, al
    inc   dx
    call  Func0x2f93                    ;Offset 0x2f93
Label0x15ed:                            ;Offset 0x15ed
    ret
Func0x1597 ENDP

Func0x15ee PROC NEAR                    ;Offset 0x15ee
    mov   al, BDA_VPCB_GrahicsCharSetOverride;0xc
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    Label0x1611                   ;Offset 0x1611
    mov   al, 07h
    call  Func0x30b5                    ;Offset 0x30b5
    jne   Label0x1611                   ;Offset 0x1611
    lodsb byte ptr es:[si]
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsw word ptr es:[si]
    mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
Label0x1611:                            ;Offset 0x1611
    ret
Func0x15ee ENDP

Func0x1612 PROC NEAR                    ;Offset 0x1612
    mov   al, BDA_VPCB_AlphaNumCharSetOverride;0x8
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    Label0x1644                   ;Offset 0x1644
    mov   al, 0bh
    call  Func0x30b5                    ;Offset 0x30b5
    jne   Label0x1644                   ;Offset 0x1644
    lodsw word ptr es:[si]
    xchg  al, ah
    xchg  ax, bx
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    xchg  ax, dx
    lodsw word ptr es:[si]
    xchg  ax, bp
    lodsw word ptr es:[si]
    push  word ptr es:[si]
    mov   es, ax
    call  Func0x2d47                    ;Offset 0x2d47
    call  Func0x2dc6                    ;Offset 0x2dc6
    pop   ax
    cmp   al, 0ffh
    je    Label0x1644                   ;Offset 0x1644
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x1644:                            ;Offset 0x1644
    ret
Func0x1612 ENDP

Func0x1645 PROC NEAR                    ;Offset 0x1645
    mov   al, BDA_VPCB_SecondaryCharSetOverride;0x6
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    Label0x1683                   ;Offset 0x1683
    mov   al, 07h
    call  Func0x30b5                    ;Offset 0x30b5
    jne   Label0x1683                   ;Offset 0x1683
    lodsw word ptr es:[si]
    cmp   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    jne   Label0x1683                   ;Offset 0x1683
    push  ax
    xchg  al, ah
    and   al, 7fh
    xchg  ax, bx
    inc   si
    les   bp, es:[si]
    xor   dx, dx
    mov   cx, 0100h
    call  Func0x2d47                    ;Offset 0x2d47
    pop   ax
    shl   ah, 01h
    shl   ah, 01h
    add   ah, 10h
    mov   dx, VGA_SequenceIndex         ;Port 0x3c4
    mov   al, VGA_SEQIdx_CharacterMapSelect;0x3
    out   dx, al
    inc   dx
    in    al, dx
    and   ax, 2c13h
    or    al, ah
    out   dx, al
Label0x1683:                            ;Offset 0x1683
    ret
Func0x1645 ENDP

SetTextModeCursorShape PROC NEAR        ;Offset 0x1684
    push      dx
    mov       ax, cx
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x16ad               ;Offset 0x16ad
    and       ah, 60h
    cmp       ah, 20h
    mov       ax, 1e00h
    je        Label0x16ad               ;Offset 0x16ad
    mov       ax, cx
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_CursorEmulationEnabled;Offset 0x487 0x8
    jne       Label0x16ad               ;Offset 0x16ad
    call      Func0x16b2                ;Offset 0x16b2
    call      SetCursorEndStart         ;Offset 0x2c96
    pop       dx
    ret
Label0x16ad:                            ;Offset 0x16ad
    call      SetCursorEndStart         ;Offset 0x2c96
    pop       dx
    ret
SetTextModeCursorShape ENDP

Func0x16b2 PROC NEAR                    ;Offset 0x16b2
    call      Func0x30a2                ;Offset 0x30a2
    jne       Label0x16ff               ;Offset 0x16ff
    test      ax, 0e0e0h
    jne       Label0x16ff               ;Offset 0x16ff
    mov       dl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       dx
    ;cmp       ah, al
    DB 3Ah, 0E0h                        ;!!!!!!!! Misassembly
    ja        Label0x16f8               ;Offset 0x16f8
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    ja        Label0x16d5               ;Offset 0x16d5
    ;cmp       al, dl
    DB 3Ah, 0C2h                        ;!!!!!!!! Misassembly
    ja        Label0x16d5               ;Offset 0x16d5
    je        Label0x16ff               ;Offset 0x16ff
    dec       dx
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    je        Label0x16ff               ;Offset 0x16ff
    inc       dx
Label0x16d5:                            ;Offset 0x16d5
    cmp       al, 03h
    jbe       Label0x16ff               ;Offset 0x16ff
    sub       al, ah
    cmp       al, 02h
    mov       al, dl
    jbe       Label0x16ec               ;Offset 0x16ec
    cmp       ah, 02h
    jbe       Label0x16ff               ;Offset 0x16ff
    inc       dx
    shr       dl, 01h
    mov       ah, dl
    ret
Label0x16ec:                            ;Offset 0x16ec
    add       ah, dl
    sub       ah, cl
    cmp       al, 0dh
    jb        Label0x16ff               ;Offset 0x16ff
    sub       ax, 0101h
    ret
Label0x16f8:                            ;Offset 0x16f8
    cmp       al, 00h
    je        Label0x16ff               ;Offset 0x16ff
    mov       dh, al
    xchg      ax, dx
Label0x16ff:                            ;Offset 0x16ff
    ret
Func0x16b2 ENDP

SetCursorPosition PROC NEAR             ;Offset 0x1700
    push      ax
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       word ptr [si + DBA_CursorPositionPage0], dx;Offset 0x450
    cmp       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    jne       Func0x1732                ;Offset 0x1732
    push      dx
    mov       ax, dx
    jmp       Func0x171a                ;Offset 0x171a
SetCursorPosition ENDP

Func0x1717 PROC NEAR                    ;Offset 0x1717
    pop       dx
    push      ax
    push      dx
Func0x1717 ENDP
;continue!
Func0x171a PROC NEAR                    ;Offset 0x171a
    call      Func0x2c4e                ;Offset 0x2c4e
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    shr       ax, 01h
    push      ax
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       al, VGA_CRTCIdx_CursorLocHigh;0xe
    out       dx, ax
    pop       ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_CursorLocLow;0xf
    out       dx, ax
    pop       dx
Func0x171a ENDP
;continue!
Func0x1732 PROC NEAR                    ;Offset 0x1732
    pop       ax
    ret
Func0x1732 ENDP

GetCursorPositionAndSize PROC NEAR      ;Offset 0x1734
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    mov       cx, word ptr ds:[BDA_CursorEndStartScanLine];Offset 0x460
    ret
GetCursorPositionAndSize ENDP

ReadLightPenPosition_SystemForward PROC NEAR;Offset 0x1744
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x174e               ;Offset 0x174e
    xor       ah, ah
    ret
Label0x174e:                            ;Offset 0x174e
    int       42h
    ret
ReadLightPenPosition_SystemForward ENDP

SelectActiveDisplayPage PROC NEAR       ;Offset 0x1751
    push      dx
    cbw
    mov       si, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, 07h
    pop       ax
    ja        Label0x176d               ;Offset 0x176d
    shr       ax, 01h
Label0x176d:                            ;Offset 0x176d
    call      SetStartAddressVGA        ;Offset 0x1778
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Func0x1717                ;Offset 0x1717
SelectActiveDisplayPage ENDP

SetStartAddressVGA PROC NEAR            ;Offset 0x1778
    push      ax
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       al, VGA_CRTCIdx_StartAddrHigh;0xc
    out       dx, ax
    pop       ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_StartAddrLow;0xd
    out       dx, ax
    ret
SetStartAddressVGA ENDP

ScrollUpWindow PROC NEAR                ;Offset 0x1787
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    mov       bl, al
    dec       ax
    sub       al, dh
    add       al, ch
    cbw
    and       bl, ah
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x17ba               ;Offset 0x17ba
    ja        Label0x17af               ;Offset 0x17af
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x17c3               ;Offset 0x17c3
Label0x17a9:                            ;Offset 0x17a9
    jmp       Label0x1841               ;Offset 0x1841
Label0x17ac:                            ;Offset 0x17ac
    jmp       Label0x192f               ;Offset 0x192f
Label0x17af:                            ;Offset 0x17af
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x1824               ;Offset 0x1824
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x17ac               ;Offset 0x17ac
    jmp       Label0x18b4               ;Offset 0x18b4
Label0x17ba:                            ;Offset 0x17ba
    mov       si, 0b800h                ;Offset 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x17a9               ;Offset 0x17a9
Label0x17c3:                            ;Offset 0x17c3
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2c4e                ;Offset 0x2c4e
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       si, ax
    mov       di, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bp, ax
    mul       bl
    shl       ax, 01h
    add       si, ax
    mov       al, ch
    sub       al, bl
    xor       ah, ah
    mov       dl, cl
    sub       bp, dx
    shl       bp, 01h
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x182e               ;Offset 0x182e
Label0x17f4:                            ;Offset 0x17f4
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x182a               ;Offset 0x182a
Label0x17fc:                            ;Offset 0x17fc
    mov       cx, dx
    rep movsw
    add       si, bp
    add       di, bp
    dec       ax
    jne       Label0x17fc               ;Offset 0x17fc
Label0x1807:                            ;Offset 0x1807
    xchg      ah, bh
    mov       al, 20h
Label0x180b:                            ;Offset 0x180b
    mov       cx, dx
    rep stosw
    add       di, bp
    dec       bx
    jne       Label0x180b               ;Offset 0x180b
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x1824               ;Offset 0x1824
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1824:                            ;Offset 0x1824
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x182a:                            ;Offset 0x182a
    mov       bl, al
    jmp       Label0x1807               ;Offset 0x1807
Label0x182e:                            ;Offset 0x182e
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x1833:                            ;Offset 0x1833
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x1833               ;Offset 0x1833
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x17f4               ;Offset 0x17f4
Label0x1841:                            ;Offset 0x1841
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    shl       ah, 01h
    shl       ah, 01h
    xchg      ax, cx
    call      Func0x2c5c                ;Offset 0x2c5c
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x185b               ;Offset 0x185b
    shl       ax, 01h
    shl       cl, 01h
Label0x185b:                            ;Offset 0x185b
    mov       di, ax
    mov       si, ax
    shl       bl, 01h
    shl       bl, 01h
    mov       al, 50h
    mul       bl
    add       si, ax
    mov       al, ch
    sub       al, bl
    xor       ah, ah
    mov       dl, cl
    mov       bp, dx
    neg       bp
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x18b0               ;Offset 0x18b0
Label0x187d:                            ;Offset 0x187d
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 1fb0h]     ;Offset 0x1fb0
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       ax
    jne       Label0x187d               ;Offset 0x187d
Label0x1898:                            ;Offset 0x1898
    xchg      al, bh
Label0x189a:                            ;Offset 0x189a
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       bx
    jne       Label0x189a               ;Offset 0x189a
    jmp       Label0x1824               ;Offset 0x1824
Label0x18b0:                            ;Offset 0x18b0
    xchg      bl, al
    jmp       Label0x1898               ;Offset 0x1898
Label0x18b4:                            ;Offset 0x18b4
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2c6c                ;Offset 0x2c6c
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       di, ax
    mov       si, ax
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_CPUDirect SHL 8) OR VGA_GCTLIdx_GraphicMode;0x105
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bp, ax
    mul       bl
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    add       si, ax
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    push      ax
    sub       ch, bl
    mul       ch
    mov       dl, cl
    sub       bp, dx
    or        bl, bl
    push      bx
    mov       bx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    je        Label0x192a               ;Offset 0x192a
Label0x18fb:                            ;Offset 0x18fb
    mov       cx, dx
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x18fb               ;Offset 0x18fb
    pop       bx
Label0x1907:                            ;Offset 0x1907
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x1917:                            ;Offset 0x1917
    mov       cx, si
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x1917               ;Offset 0x1917
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1824               ;Offset 0x1824
Label0x192a:                            ;Offset 0x192a
    pop       bx
    mov       bl, ch
    jmp       Label0x1907               ;Offset 0x1907
Label0x192f:                            ;Offset 0x192f
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2c7e                ;Offset 0x2c7e
    xchg      ax, di
    mov       si, di
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, ((VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4) SHL 8) OR VGA_SEQIdx_MemoryMode;0xe04
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       ax, (VGA_GCTL5_256ColorMode SHL 8) OR VGA_GCTLIdx_GraphicMode;0x4005
    out       dx, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       ax, 01h
    shl       ax, 01h
    shl       ax, 01h
    xchg      ax, bp
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       bl
    mul       bp
    add       si, ax
    mov       dl, cl
    shl       dx, 01h
    mov       ax, dx
    shl       ax, 01h
    shl       ax, 01h
    sub       bp, ax
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    push      ax
    sub       ch, bl
    mul       ch
    or        bl, bl
    push      bx
    xchg      ax, bx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    je        Label0x19a6               ;Offset 0x19a6
Label0x197e:                            ;Offset 0x197e
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x197e               ;Offset 0x197e
    pop       bx
Label0x198e:                            ;Offset 0x198e
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x1996:                            ;Offset 0x1996
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x1996               ;Offset 0x1996
    jmp       Label0x1824               ;Offset 0x1824
Label0x19a6:                            ;Offset 0x19a6
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x198e               ;Offset 0x198e
ScrollUpWindow ENDP

ScrollDownWindow PROC NEAR              ;Offset 0x19ac
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    std
    mov       bl, al
    dec       ax
    sub       al, dh
    add       al, ch
    cbw
    and       bl, ah
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x19e0               ;Offset 0x19e0
    ja        Label0x19d5               ;Offset 0x19d5
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x19e9               ;Offset 0x19e9
Label0x19cf:                            ;Offset 0x19cf
    jmp       Label0x1a69               ;Offset 0x1a69
Label0x19d2:                            ;Offset 0x19d2
    jmp       Label0x1b63               ;Offset 0x1b63
Label0x19d5:                            ;Offset 0x19d5
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x1a4c               ;Offset 0x1a4c
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x19d2               ;Offset 0x19d2
    jmp       Label0x1ae0               ;Offset 0x1ae0
Label0x19e0:                            ;Offset 0x19e0
    mov       si, 0b800h                ;Offset 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x19cf               ;Offset 0x19cf
Label0x19e9:                            ;Offset 0x19e9
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    call      Func0x2c4e                ;Offset 0x2c4e
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       si, ax
    mov       di, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bp, ax
    mul       bl
    shl       ax, 01h
    sub       si, ax
    mov       al, ch
    sub       al, bl
    xor       ah, ah
    mov       dl, cl
    sub       bp, dx
    shl       bp, 01h
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x1a56               ;Offset 0x1a56
Label0x1a1c:                            ;Offset 0x1a1c
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1a52               ;Offset 0x1a52
Label0x1a24:                            ;Offset 0x1a24
    mov       cx, dx
    rep movsw
    sub       si, bp
    sub       di, bp
    dec       ax
    jne       Label0x1a24               ;Offset 0x1a24
Label0x1a2f:                            ;Offset 0x1a2f
    xchg      ah, bh
    mov       al, 20h
Label0x1a33:                            ;Offset 0x1a33
    mov       cx, dx
    rep stosw
    sub       di, bp
    dec       bx
    jne       Label0x1a33               ;Offset 0x1a33
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x1a4c               ;Offset 0x1a4c
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1a4c:                            ;Offset 0x1a4c
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x1a52:                            ;Offset 0x1a52
    mov       bl, al
    jmp       Label0x1a2f               ;Offset 0x1a2f
Label0x1a56:                            ;Offset 0x1a56
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x1a5b:                            ;Offset 0x1a5b
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x1a5b               ;Offset 0x1a5b
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x1a1c               ;Offset 0x1a1c
Label0x1a69:                            ;Offset 0x1a69
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    shl       dh, 01h
    shl       dh, 01h
    mov       cx, dx
    call      Func0x2c5c                ;Offset 0x2c5c
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x1a86               ;Offset 0x1a86
    shl       ax, 01h
    inc       ax
    shl       cl, 01h
Label0x1a86:                            ;Offset 0x1a86
    add       ax, 00f0h
    mov       di, ax
    mov       si, ax
    shl       bl, 01h
    shl       bl, 01h
    mov       al, 50h
    mul       bl
    sub       si, ax
    mov       al, ch
    sub       al, bl
    xor       ah, ah
    mov       dl, cl
    mov       bp, dx
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1adc               ;Offset 0x1adc
Label0x1aa9:                            ;Offset 0x1aa9
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 2050h]     ;Offset 0x2050
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       ax
    jne       Label0x1aa9               ;Offset 0x1aa9
Label0x1ac4:                            ;Offset 0x1ac4
    xchg      al, bh
Label0x1ac6:                            ;Offset 0x1ac6
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       bx
    jne       Label0x1ac6               ;Offset 0x1ac6
    jmp       Label0x1a4c               ;Offset 0x1a4c
Label0x1adc:                            ;Offset 0x1adc
    xchg      bl, al
    jmp       Label0x1ac4               ;Offset 0x1ac4
Label0x1ae0:                            ;Offset 0x1ae0
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x2c6c                ;Offset 0x2c6c
    sub       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       di, ax
    mov       si, ax
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_CPUDirect SHL 8) OR VGA_GCTLIdx_GraphicMode;0x105
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bp, ax
    mul       bl
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       si, ax
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    push      ax
    sub       ch, bl
    mul       ch
    mov       dl, cl
    sub       bp, dx
    or        bl, bl
    push      bx
    mov       bx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    je        Label0x1b5e               ;Offset 0x1b5e
Label0x1b2f:                            ;Offset 0x1b2f
    mov       cx, dx
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x1b2f               ;Offset 0x1b2f
    pop       bx
Label0x1b3b:                            ;Offset 0x1b3b
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x1b4b:                            ;Offset 0x1b4b
    mov       cx, si
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x1b4b               ;Offset 0x1b4b
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1a4c               ;Offset 0x1a4c
Label0x1b5e:                            ;Offset 0x1b5e
    pop       bx
    mov       bl, ch
    jmp       Label0x1b3b               ;Offset 0x1b3b
Label0x1b63:                            ;Offset 0x1b63
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x2c7e                ;Offset 0x2c7e
    ;add       ax, 07h
    DB 05h, 07h, 00h                    ;!!!!!!!!! misassembly
    xchg      ax, di
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, ((VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4) SHL 8) OR VGA_SEQIdx_MemoryMode;0xe04
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       ax, (VGA_GCTL5_256ColorMode SHL 8) OR VGA_GCTLIdx_GraphicMode;0x4005
    out       dx, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       ax, 01h
    shl       ax, 01h
    shl       ax, 01h
    sub       di, ax
    xchg      ax, bp
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       bl
    mul       bp
    mov       si, di
    sub       si, ax
    mov       dl, cl
    shl       dx, 01h
    mov       ax, dx
    shl       ax, 01h
    shl       ax, 01h
    sub       bp, ax
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    push      ax
    sub       ch, bl
    mul       ch
    or        bl, bl
    push      bx
    xchg      ax, bx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       ds, ax
    je        Label0x1be3               ;Offset 0x1be3
Label0x1bbb:                            ;Offset 0x1bbb
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x1bbb               ;Offset 0x1bbb
    pop       bx
Label0x1bcb:                            ;Offset 0x1bcb
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x1bd3:                            ;Offset 0x1bd3
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x1bd3               ;Offset 0x1bd3
    jmp       Label0x1a4c               ;Offset 0x1a4c
Label0x1be3:                            ;Offset 0x1be3
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x1bcb               ;Offset 0x1bcb
ScrollDownWindow ENDP

ReadCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x1be9
    push       dx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x64cd               ;Offset 0x64cd
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x1bfd              ;Offset 0x1bfd
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Label0x1c36              ;Offset 0x1c36
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x1bfd:                            ;Offset 0x1bfd
    push       dx
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c4e               ;Offset 0x2c4e
    add        si, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x1c1f              ;Offset 0x1c1f
    pop        ds
    lodsw
    pop        dx
    ret
Label0x1c1f:                            ;Offset 0x1c1f
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a VGA Input Status
    pop        ds
Label0x1c27:                            ;Offset 0x1c27
    in         al, dx
    shr        al, 01h                  ;check HSync
    jb         Label0x1c27              ;Offset 0x1c27
    cli
Label0x1c2d:                            ;Offset 0x1c2d
    in         al, dx
    shr        al, 01h                  ;check HSync
    jae        Label0x1c2d              ;Offset 0x1c2d
    lodsw
    sti
    pop        dx
    ret
Label0x1c36:                            ;Offset 0x1c36
    push       cx
    push       es
    ja         Label0x1ca5              ;Offset 0x1ca5
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c5c               ;Offset 0x2c5c
    mov        si, ax
    sub        sp, 0008h
    mov        di, sp
    mov        ax, ss
    mov        es, ax
    mov        ax, 0b800h               ;Segment 0xb800
    mov        cx, 0004h
    cmp        byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov        ds, ax
    je         Label0x1c69              ;Offset 0x1c69
    shl        si, 01h
Label0x1c5c:                            ;Offset 0x1c5c
    call       Func0x1d9a               ;Offset 0x1d9a
    call       Func0x1d9a               ;Offset 0x1d9a
    add        si, 0050h
    loop       Label0x1c5c              ;Offset 0x1c5c
    jmp        Label0x1c76              ;Offset 0x1c76
Label0x1c69:                            ;Offset 0x1c69
    mov        al, byte ptr [si]
    stosb
    mov        al, byte ptr [si + 2000h];Offset 0x2000
    stosb
    add        si, 0050h
    loop       Label0x1c69              ;Offset 0x1c69
Label0x1c76:                            ;Offset 0x1c76
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, 0008h
    xor        ax, ax
Label0x1c81:                            ;Offset 0x1c81
    push       si
    push       di
    mov        cl, 04h
    repe cmpsw
    pop        di
    pop        si
    je         Label0x1c9e              ;Offset 0x1c9e
    add        si, 0008h
    inc        al
    je         Label0x1c9e              ;Offset 0x1c9e
    jno        Label0x1c81              ;Offset 0x1c81
    xor        cx, cx
    mov        ds, cx
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    jmp        Label0x1c81              ;Offset 0x1c81
Label0x1c9e:                            ;Offset 0x1c9e
    add        sp, 0008h
    pop        es
    pop        cx
    pop        dx
    ret
Label0x1ca5:                            ;Offset 0x1ca5
    push       bx
    mov        dh, 0a0h                 ;Segment 0xa000
    push       dx
    cmp        al, 13h
    jne        Label0x1cb0              ;Offset 0x1cb0
    jmp        Label0x1d37              ;Offset 0x1d37
Label0x1cb0:                            ;Offset 0x1cb0
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c6c               ;Offset 0x2c6c
    add        si, ax
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, (VGA_GCTL5_ReadColCmp SHL 8) OR VGA_GCTLIdx_GraphicMode;0x805
    out        dx, ax
    mov        bx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        dx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    pop        ds
    sub        sp, bx
    mov        di, sp
    mov        ax, ss
    mov        es, ax
    mov        cx, bx
Label0x1ce0:                            ;Offset 0x1ce0
    mov        al, byte ptr [si]
    not        al
    stosb
    add        si, dx
    loop       Label0x1ce0              ;Offset 0x1ce0
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out        dx, ax
Label0x1cf0:                            ;Offset 0x1cf0
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, bx
    xor        ax, ax
Label0x1cfa:                            ;Offset 0x1cfa
    push       si
    push       di
    mov        cx, bx
    call       Func0x12c5               ;Offset 0x12c5
    jb         Label0x1d1a              ;Offset 0x1d1a
    jne        Label0x1d1a              ;Offset 0x1d1a
    mov        cl, 08h
    repe cmpsb
    je         Label0x1d0f              ;Offset 0x1d0f
    pop        di
    pop        si
    jmp        Label0x1d20              ;Offset 0x1d20
Label0x1d0f:                            ;Offset 0x1d0f
    inc        si
    mov        cl, 06h
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1d30              ;Offset 0x1d30
    jmp        Label0x1d20              ;Offset 0x1d20
Label0x1d1a:                            ;Offset 0x1d1a
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1d30              ;Offset 0x1d30
Label0x1d20:                            ;Offset 0x1d20
    mov        cx, bx
    call       Func0x12c5               ;Offset 0x12c5
    jb         Label0x1d2a              ;Offset 0x1d2a
    add        si, 0002h
Label0x1d2a:                            ;Offset 0x1d2a
    add        si, bx
    inc        al
    jne        Label0x1cfa              ;Offset 0x1cfa
Label0x1d30:                            ;Offset 0x1d30
    add        sp, bx
    pop        bx
    pop        es
    pop        cx
    pop        dx
    ret
Label0x1d37:                            ;Offset 0x1d37
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c7e               ;Offset 0x2c7e
    xchg       ax, si
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, ((VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x604
    out        dx, ax
    mov        dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    out        dx, al
    mov        dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov        al, MGA_CRTCIdx_CPUReadLatch;0x22
    out        dx, al
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl        ax, 01h
    shl        ax, 01h
    shl        ax, 01h
    pop        ds
    sub        sp, cx
    mov        di, sp
    push       ss
    pop        es
    push       cx
    push       bp
    xchg       ax, bp
Label0x1d64:                            ;Offset 0x1d64
    call       Func0x1d76               ;Offset 0x1d76
    stosb
    add        si, bp
    loop       Label0x1d64              ;Offset 0x1d64
    pop        bp
    pop        bx
    mov        dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov        al, VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4;0xe
    out        dx, al
    jmp        Label0x1cf0              ;Offset 0x1cf0
ReadCharacterAndAttributeAtCursorPosition ENDP

Func0x1d76 PROC NEAR                    ;Offset 0x1d76
    mov        bx, 0100h
    mov        al, byte ptr [si + 04h]
    call       Func0x1d81               ;Offset 0x1d81
    mov        al, byte ptr [si]
Func0x1d76 ENDP
;continue!
Func0x1d81 PROC NEAR                    ;Offset 0x1d81
    mov        ah, 03h
Label0x1d83:                            ;Offset 0x1d83
    mov        dl, VGA_GraphicsControllerData_lowbyte;Port 0x3cf
    mov        al, ah
    out        dx, al
    mov        dl, VGA_CRTControllerDataD_lowbyte;Port 0x3d5
    in         al, dx
    or         al, al
    je         Label0x1d91              ;Offset 0x1d91
    or         bl, bh
Label0x1d91:                            ;Offset 0x1d91
    rol        bh, 01h
    dec        ah
    jns        Label0x1d83              ;Offset 0x1d83
    mov        al, bl
    ret
Func0x1d81 ENDP

Func0x1d9a PROC NEAR                    ;Offset 0x1d9a
    push       cx
    mov        dx, word ptr [si]
    xchg       dl, dh
    mov        cx, 0008h
Label0x1da2:                            ;Offset 0x1da2
    shl        dx, 01h
    jb         Label0x1da9              ;Offset 0x1da9
    jns        Label0x1da9              ;Offset 0x1da9
    stc
Label0x1da9:                            ;Offset 0x1da9
    rcl        al, 01h
    shl        dx, 01h
    loop       Label0x1da2              ;Offset 0x1da2
    stosb
    xor        si, 2000h
    pop        cx
    ret
Func0x1d9a ENDP

WriteCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x1db6
    push       cx
    push       dx
    push       es
    mov        ah, bl
    push       ax
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x64cd               ;Offset 0x64cd
    mov        dx, 0b800h               ;Offset 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x1dd3              ;Offset 0x1dd3
    cmp        al, BDA_DM_640x480_BW_Graphics;0x11
    je         Label0x1e16              ;Offset 0x1e16
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Func0x1e1c               ;Offset 0x1e1c
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x1dd3:                            ;Offset 0x1dd3
    mov        es, dx
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c4e               ;Offset 0x2c4e
    add        di, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x1df9              ;Offset 0x1df9
    pop        ax
    rep stosw
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1df9:                            ;Offset 0x1df9
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a
    pop        si
Label0x1e01:                            ;Offset 0x1e01
    in         al, dx
    shr        al, 01h
    jb         Label0x1e01              ;Offset 0x1e01
    cli
Label0x1e07:                            ;Offset 0x1e07
    in         al, dx
    shr        al, 01h
    jae        Label0x1e07              ;Offset 0x1e07
    mov        ax, si
    stosw
    sti
    loop       Label0x1e01              ;Offset 0x1e01
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1e16:                            ;Offset 0x1e16
    and        bl, 80h
    or         bl, 0fh
WriteCharacterAndAttributeAtCursorPosition ENDP
;continue!
Func0x1e1c PROC NEAR                    ;Offset 0x1e1c
    ja         Label0x1e5b              ;Offset 0x1e5b
    mov        es, dx
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c5c               ;Offset 0x2c5c
    mov        di, ax
    mov        dl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop        ax
    or         al, al
    jns        Label0x1e39              ;Offset 0x1e39
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    and        al, 7fh
    jmp        Label0x1e3d              ;Offset 0x1e3d
Label0x1e39:                            ;Offset 0x1e39
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
Label0x1e3d:                            ;Offset 0x1e3d
    cbw
    shl        ax, 01h
    shl        ax, 01h
    shl        ax, 01h
    add        si, ax
    cmp        dl, BDA_DM_640x200_BW_Graphics;0x6
    je         Label0x1e54              ;Offset 0x1e54
    shl        di, 01h
    push       bx
    call       Func0x2031               ;Offset 0x2031
    pop        bx
    jmp        Label0x1e57              ;Offset 0x1e57
Label0x1e54:                            ;Offset 0x1e54
    call       Func0x1fcd               ;Offset 0x1fcd
Label0x1e57:                            ;Offset 0x1e57
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1e5b:                            ;Offset 0x1e5b
    mov        dh, 0a0h                 ;Segment 0xa000
    mov        es, dx
    cmp        al, 13h
    jne        Label0x1e66              ;Offset 0x1e66
    jmp        Label0x1f67              ;Offset 0x1f67
Label0x1e66:                            ;Offset  0x1e66
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c6c               ;Offset 0x2c6c
    add        di, ax
    pop        ax
    push       bp
    push       cx
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    xor        ch, ch
    call       Func0x12c5               ;Offset 0x12c5
    jb         Label0x1e9f              ;Offset 0x1e9f
    jne        Label0x1e97              ;Offset 0x1e97
    or         ch, 02h
Label0x1e97:                            ;Offset 0x1e97
    mov        cl, 10h
    mul        cl
    mov        cl, 0eh
    jmp        Label0x1ea1              ;Offset 0x1ea1
Label0x1e9f:                            ;Offset 0x1e9f
    mul        cl
Label0x1ea1:                            ;Offset 0x1ea1
    add        si, ax
    or         bl, bl
    js         Label0x1f17              ;Offset 0x1f17
    mov        al, 52h
    push       cx
    push       cx
    xor        ch, ch
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out        dx, ax
    xor        ax, ax
    push       di
Label0x1eb7:                            ;Offset 0x1eb7
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1eb7              ;Offset 0x1eb7
    pop        di
    mov        al, VGA_SEQIdx_MapMask   ;0x2
    mov        ah, bl
    out        dx, ax
    mov        dx, di
    pop        cx
    and        ch, 02h
    je         Label0x1ee6              ;Offset 0x1ee6
    xor        ch, ch
    mov        cl, 08h
Label0x1ed0:                            ;Offset 0x1ed0
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1ed0              ;Offset 0x1ed0
    mov        cl, 07h
    inc        si
Label0x1edb:                            ;Offset 0x1edb
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1edb              ;Offset 0x1edb
    inc        di
    jmp        Label0x1eee              ;Offset 0x1eee
Label0x1ee6:                            ;Offset 0x1ee6
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1ee6              ;Offset 0x1ee6
Label0x1eee:                            ;Offset 0x1eee
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x1ef9              ;Offset 0x1ef9
    mov        cx, 0010h
Label0x1ef9:                            ;Offset 0x1ef9
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    ;jne        Label0x1ea8              ;Offset 0x1ea8 !!!!!!!!!!!
    DB 075h, 0A4h                       ;Generates a broken jump (target = middle of instruction)
Label0x1f04:                            ;Offset 0x1f04
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, VGA_GCTLIdx_DataRotate;0x3
    out        dx, ax
    mov        dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov        ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out        dx, ax
    pop        bp
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1f16:                            ;Offset 0x1f16
    push       dx
Label0x1f17:                            ;Offset 0x1f17
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, (VGA_GCTL3_FuncSourceXORLatched SHL 8) OR VGA_GCTLIdx_DataRotate;0x1803
    out        dx, ax
    mov        dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov        al, (VGA_SEQ2_MaskNone SHL 8) OR VGA_SEQIdx_MapMask;0x2
    mov        ah, bl
    out        dx, ax
    push       cx
    mov        dx, di
    and        ch, 02h
    je         Label0x1f47              ;Offset 0x1f47
    xor        ch, ch
    mov        cl, 08h
Label0x1f31:                            ;Offset 0x1f31
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1f31              ;Offset 0x1f31
    mov        cl, 07h
    inc        si
Label0x1f3c:                            ;Offset 0x1f3c
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1f3c              ;Offset 0x1f3c
    inc        di
    jmp        Label0x1f4f              ;Offset 0x1f4f
Label0x1f47:                            ;Offset 0x1f47
    lodsb
    xchg       byte ptr es:[di], al
    add        di, bp
    loop       Label0x1f47              ;Offset 0x1f47
Label0x1f4f:                            ;Offset 0x1f4f
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x1f5a              ;Offset 0x1f5a
    mov        cx, 0010h
Label0x1f5a:                            ;Offset 0x1f5a
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    jne        Label0x1f16              ;Offset 0x1f16
    jmp        Label0x1f04              ;Offset 0x1f04
Label0x1f67:                            ;Offset 0x1f67
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2c7e               ;Offset 0x2c7e
    xchg       ax, di
    pop        ax
    push       bp
    push       bx
    push       cx
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl        bp, 01h
    shl        bp, 01h
    shl        bp, 01h
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    mul        cl
    add        si, ax
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, ((VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4) SHL 8) OR VGA_SEQIdx_MemoryMode;0xe04
    out        dx, ax
    mov        dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov        ax, (VGA_GCTL5_256ColorMode SHL 8) OR VGA_GCTLIdx_GraphicMode;0x4005
    out        dx, ax
    mov        al, 52h
    push       cx
    mov        dx, di
Label0x1f9a:                            ;Offset 0x1f9a
    lodsb
    push       di
    push       cx
    mov        cx, 0008h
Label0x1fa0:                            ;Offset 0x1fa0
    rcl        al, 01h
    jae        Label0x1fa9              ;Offset 0x1fa9
    mov        byte ptr es:[di], bl
    jmp        Label0x1fac              ;Offset 0x1fac
Label0x1fa9:                            ;Offset 0x1fa9
    mov        byte ptr es:[di], bh
Label0x1fac:                            ;Offset 0x1fac
    inc        di
    loop       Label0x1fa0              ;Offset 0x1fa0
    pop        cx
    pop        di
    add        di, bp
    loop       Label0x1f9a              ;Offset 0x1f9a
    pop        cx
    sub        si, cx
    mov        di, dx
    add        di, 0008h
    pop        dx
    dec        dx
    ;jne        Label0x1f96              ;Offset 0x1f96 !!!!!!!!!!!!!!
    DB 075h, 0D5h                       ;Generates a broken jump (target = middle of instruction)
    pop        bx
    pop        bp
    pop        es
    pop        dx
    pop        cx
    ret
    pop        bx
    pop        bp
    pop        es
    pop        dx
    pop        cx
    ret
Func0x1e1c ENDP

Func0x1fcd PROC NEAR                    ;Offset 0x1fcd
    or        bl, bl
    js        Label0x2001               ;Offset 0x2001
Label0x1fd1:                            ;Offset 0x1fd1
    lodsw
    mov       byte ptr es:[di], al
    mov       byte ptr es:[di + 2000h], ah;Offset 0x2000
    lodsw
    mov       byte ptr es:[di + 50h], al;Offset 0x50
    mov       byte ptr es:[di + 2050h], ah;Offset 0x2050
    lodsw
    mov       byte ptr es:[di + 0a0h], al;Offset 0xa0
    mov       byte ptr es:[di + 20a0h], ah;Offset 0x20a0
    lodsw
    mov       byte ptr es:[di + 0f0h], al;Offset 0xf0
    mov       byte ptr es:[di + 20f0h], ah;Offset 0x20f0
    sub       si, 0008h
    inc       di
    loop      Label0x1fd1               ;Offset 0x1fd1
    ret
Label0x2001:                            ;Offset 0x2001
    lodsw
    xor       byte ptr es:[di], al
    xor       byte ptr es:[di + 2000h], ah;Offset 0x2000
    lodsw
    xor       byte ptr es:[di + 50h], al;Offset 0x50
    xor       byte ptr es:[di + 2050h], ah;Offset 0x2050
    lodsw
    xor       byte ptr es:[di + 0a0h], al;Offset 0xa0
    xor       byte ptr es:[di + 20a0h], ah;Offset 0x20a0
    lodsw
    xor       byte ptr es:[di + 0f0h], al;Offset 0xf0
    xor       byte ptr es:[di + 20f0h], ah;Offset 0x20f0
    sub       si, 0008h
    inc       di
    loop      Label0x2001               ;Offset 0x2001
    ret
Func0x1fcd ENDP

Func0x2031 PROC NEAR                    ;Offset 0x2031
    mov       dl, bl
    and       dl, 03h
    mov       dh, dl
    shl       dl, 01h
    shl       dl, 01h
    or        dl, dh
    shl       dl, 01h
    shl       dl, 01h
    or        dl, dh
    shl       dl, 01h
    shl       dl, 01h
    or        dl, dh
    mov       dh, dl
    or        bl, bl
    js        Label0x2073               ;Offset 0x2073
Label0x2050:                            ;Offset 0x2050
    mov       bh, 04h
Label0x2052:                            ;Offset 0x2052
    lodsb
    call      Func0x2096                ;Offset 0x2096
    mov       word ptr es:[di], ax
    lodsb
    call      Func0x2096                ;Offset 0x2096
    mov       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x2052               ;Offset 0x2052
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x2050               ;Offset 0x2050
    ret
Label0x2073:                            ;Offset 0x2073
    mov       bh, 04h
Label0x2075:                            ;Offset 0x2075
    lodsb
    call      Func0x2096                ;Offset 0x2096
    xor       word ptr es:[di], ax
    lodsb
    call      Func0x2096                ;Offset 0x2096
    xor       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x2075               ;Offset 0x2075
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x2073               ;Offset 0x2073
    ret
Func0x2031 ENDP

Func0x2096 PROC NEAR                    ;Offset 0x2096
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    mov       bl, ah
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    ror       ax, 01h
    sar       ah, 01h
    mov       al, ah
    mov       ah, bl
    and       ax, dx
    ret
Func0x2096 ENDP

WriteCharacterOnlyAtCursorPosition PROC NEAR;Offset 0x20bf
    push      cx
    push      dx
    push      es
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    mov       dx, 0b800h                ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x20d6               ;Offset 0x20d6
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x211d               ;Offset 0x211d
    mov       dh, 0b0h                  ;Segment 0xb000
Label0x20d6:                            ;Offset 0x20d6
    mov       es, dx
    mov       al, bh
    cbw
    mov       si, ax
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       di, ax
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call      Func0x2c4e                ;Offset 0x2c4e
    add       di, ax
    pop       ax
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x20fe               ;Offset 0x20fe
Label0x20f6:                            ;Offset 0x20f6
    stosb
    inc       di
    loop      Label0x20f6               ;Offset 0x20f6
    pop       es
    pop       dx
    pop       cx
    ret
Label0x20fe:                            ;Offset 0x20fe
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a VGA_InputStatus1
    mov       ah, al
Label0x2107:                            ;Offset 0x2107
    in        al, dx
    shr       al, 01h
    jb        Label0x2107               ;Offset 0x2107
    cli
Label0x210d:                            ;Offset 0x210d
    in        al, dx
    shr       al, 01h
    jae       Label0x210d               ;Offset 0x210d
    mov       al, ah
    stosb
    sti
    inc       di
    loop      Label0x2107               ;Offset 0x2107
    pop       es
    pop       dx
    pop       cx
    ret
Label0x211d:                            ;Offset 0x211d
    jmp       Func0x1e1c                ;Offset 0x1e1c
WriteCharacterOnlyAtCursorPosition ENDP

SetBorderColorOrPalette PROC NEAR       ;Offset 0x2120
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexB_lowbyte;Offset 0x463 0xb4
    je        Label0x2130               ;Offset 0x2130
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x2131               ;Offset 0x2131
    int       42h
Label0x2130:                            ;Offset 0x2130
    ret
Label0x2131:                            ;Offset 0x2131
    push      bx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    xchg      ax, si
    or        bh, bh
    jne       Label0x218a               ;Offset 0x218a
    mov       al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    mov       ah, bl
    and       ax, 1fe0h
    or        al, ah
    mov       bh, al
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    and       ah, 08h
    shl       ah, 01h
    and       bl, 07h
    or        bl, ah
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x2170               ;Offset 0x2170
    mov       al, bl
    xor       ah, ah
    call      Func0x30d9                ;Offset 0x30d9
    or        si, si
    je        Label0x2170               ;Offset 0x2170
    mov       byte ptr es:[di], al
    jmp       Label0x2170               ;Offset 0x2170
Label0x2170:                            ;Offset 0x2170
    mov       al, bl
    mov       ah, 11h
    call      Func0x30d9                ;Offset 0x30d9
    or        si, si
    je        Label0x217f               ;Offset 0x217f
    mov       byte ptr es:[di + 10h], al
Label0x217f:                            ;Offset 0x217f
    mov       bl, bh
    and       bl, 20h
    shl       bl, 01h
    shl       bl, 01h
    rol       bl, 01h
Label0x218a:                            ;Offset 0x218a
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x21b4               ;Offset 0x21b4
    mov       al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       al, 0dfh
    and       bl, 01h
    je        Label0x21a0               ;Offset 0x21a0
    or        al, 20h
Label0x21a0:                            ;Offset 0x21a0
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    and       al, 10h
    or        al, 02h
    or        al, bl
    mov       ah, 01h
    call      Func0x21ba                ;Offset 0x21ba
    call      Func0x21ba                ;Offset 0x21ba
    call      Func0x21ba                ;Offset 0x21ba
Label0x21b4:                            ;Offset 0x21b4
    call      Func0x30c9                ;Offset 0x30c9
    pop       es
    pop       bx
    ret
SetBorderColorOrPalette ENDP

Func0x21ba PROC NEAR                    ;Offset 0x21ba
    call      Func0x30d9                ;Offset 0x30d9
    or        si, si
    je        Label0x21c5               ;Offset 0x21c5
    inc       di
    mov       byte ptr es:[di], al
Label0x21c5:                            ;Offset 0x21c5
    add       ax, 0102h
    ret
Func0x21ba ENDP

WriteGraphicsPixel PROC NEAR            ;Offset 0x21c9
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x2231               ;Offset 0x2231
    push      cx
    push      dx
    push      ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x2248               ;Offset 0x2248
    mov       dh, al
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x21ec               ;Offset 0x21ec
    or        ah, 20h
Label0x21ec:                            ;Offset 0x21ec
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    mov       ah, dh
    jb        Label0x2208               ;Offset 0x2208
    shr       si, 01h
    mov       al, 80h
    jmp       Label0x220e               ;Offset 0x220e
Label0x2208:                            ;Offset 0x2208
    shl       cl, 01h
    mov       al, 0c0h
    ror       ah, 01h
Label0x220e:                            ;Offset 0x220e
    add       di, si
    ror       ah, 01h
    and       cl, 07h
    and       ah, al
    shr       ax, cl
    mov       cl, byte ptr [di]
    shl       dh, 01h
    jb        Label0x2229               ;Offset 0x2229
    not       al
    and       cl, al
    or        cl, ah
    mov       byte ptr [di], cl
    jmp       Label0x222d               ;Offset 0x222d
Label0x2229:                            ;Offset 0x2229
    xor       cl, ah
    mov       byte ptr [di], cl
Label0x222d:                            ;Offset 0x222d
    pop       ax
    pop       dx
    pop       cx
    ret
Label0x2231:                            ;Offset 0x2231
    xchg      ax, di
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       dl
    shl       ax, 01h
    shl       ax, 01h
    shl       ax, 01h
    add       ax, cx
    xchg      ax, di
    mov       si, 0a000h                ;Segment 0xa000
    mov       ds, si
    mov       byte ptr [di], al
    ret
Label0x2248:                            ;Offset 0x2248
    push      ax
    mov       ax, dx
    mul       word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       di, cx
    shr       di, 01h
    shr       di, 01h
    shr       di, 01h
    add       di, ax
    mov       al, bh
    cbw
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add       di, ax
    mov       ah, 80h
    and       cl, 07h
    shr       ah, cl
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_BitMask   ;0x8
    out       dx, ax
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    pop       cx
    or        cl, cl
    jns       Label0x2281               ;Offset 0x2281
    mov       ax, (VGA_GCTL3_FuncSourceXORLatched SHL 8) OR VGA_GCTLIdx_DataRotate;0x1803
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    jmp       Label0x228b               ;Offset 0x228b
Label0x2281:                            ;Offset 0x2281
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    xor       al, al
    xchg      byte ptr [di], al
Label0x228b:                            ;Offset 0x228b
    mov       al, VGA_SEQIdx_MapMask    ;0x2
    mov       ah, cl
    out       dx, ax
    mov       al, 0ffh
    xchg      byte ptr [di], al
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       ax, (VGA_GCTL3_Rotate0 SHL 8) OR VGA_GCTLIdx_DataRotate;0x3
    out       dx, ax
    mov       ax, (0ffh SHL 8) OR VGA_GCTLIdx_BitMask;0xff08
    out       dx, ax
    pop       ax
    pop       dx
    pop       cx
    ret
WriteGraphicsPixel ENDP

ReadGraphicsPixel PROC NEAR             ;Offset 0x22a6
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x22ff               ;Offset 0x22ff
    push      cx
    push      dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x2315               ;Offset 0x2315
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x22c6               ;Offset 0x22c6
    or        ah, 20h
Label0x22c6:                            ;Offset 0x22c6
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    jb        Label0x22eb               ;Offset 0x22eb
    shr       si, 01h
    add       di, si
    and       cl, 07h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    and       al, 01h
    jmp       Label0x22fc               ;Offset 0x22fc
Label0x22eb:                            ;Offset 0x22eb
    add       di, si
    and       cl, 03h
    shl       cl, 01h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    rol       al, 01h
    and       al, 03h
Label0x22fc:                            ;Offset 0x22fc
    pop       dx
    pop       cx
    ret
Label0x22ff:                            ;Offset 0x22ff
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       dl
    shl       ax, 01h
    shl       ax, 01h
    shl       ax, 01h
    add       ax, cx
    xchg      ax, di
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       al, byte ptr [di]
    ret
Label0x2315:                            ;Offset 0x2315
    mov       ax, dx
    mul       word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       di, cx
    shr       di, 01h
    shr       di, 01h
    shr       di, 01h
    add       di, ax
    mov       al, bh
    cbw
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add       di, ax
    mov       ch, 80h
    and       cl, 07h
    shr       ch, cl
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       si, bx
    xor       bl, bl
    mov       ah, 03h
Label0x2343:                            ;Offset 0x2343
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, ax
    mov       al, byte ptr [di]
    and       al, ch
    rol       bl, 01h
    or        bl, al
    dec       ah
    jns       Label0x2343               ;Offset 0x2343
    inc       cx
    rol       bl, cl
    mov       al, bl
    mov       bx, si
    pop       dx
    pop       cx
    ret
ReadGraphicsPixel ENDP

TeletypeOutput PROC NEAR                ;Offset 0x235c
    push      ax
    push      bx
    push      cx
    push      dx
    xchg      ax, di
    mov       al, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    mov       bh, al
    cbw
    xchg      ax, si
    shl       si, 01h
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    xchg      ax, di
    cmp       al, 0dh
    jbe       Func0x2390                ;Offset 0x2390
TeletypeOutput ENDP
;continue!
Func0x2373 PROC NEAR                    ;Offset 0x2373
    mov       cx, 0001h
    push      ds
    call      WriteCharacterOnlyAtCursorPosition;Offset 0x20bf
    pop       ds
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    je        Func0x23b0                ;Offset 0x23b0
Func0x2373 ENDP
;continue!
Func0x2382 PROC NEAR                    ;Offset 0x2382
    call      SetCursorPosition         ;Offset 0x1700
Func0x2382 ENDP
;continue!
Func0x2385 PROC NEAR                    ;Offset 0x2385
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0x2385 ENDP

Func0x238a PROC NEAR                    ;Offset 0x238a
    push      ax
    push      bx
    push      cx
    push      dx
    cmp       al, 0dh
Func0x238a ENDP
;continue!
Func0x2390 PROC NEAR                    ;Offset 0x2390
    je        Label0x23a5               ;Offset 0x23a5
    cmp       al, 0ah
    je        Func0x23b2                ;Offset 0x23b2
    cmp       al, 08h
    je        Label0x23a9               ;Offset 0x23a9
    cmp       al, 07h
    jne       Func0x2373                ;Offset 0x2373
    mov       al, 25h
    call      Beep                      ;Offset 0x31de
    jmp       Func0x2385                ;Offset 0x2385
Label0x23a5:                            ;Offset 0x23a5
    xor       dl, dl
    jmp       Func0x2382                ;Offset 0x2382
Label0x23a9:                            ;Offset 0x23a9
    or        dl, dl
    je        Func0x2382                ;Offset 0x2382
    dec       dx
    jmp       Func0x2382                ;Offset 0x2382
Func0x2390 ENDP

Func0x23b0 PROC NEAR                    ;Offset 0x23b0
    xor       dl, dl
Func0x23b0 ENDP
;continue!
Func0x23b2 PROC NEAR                    ;Offset 0x23b2
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Func0x2382                ;Offset 0x2382
    dec       dh
    call      SetCursorPosition         ;Offset 0x1700
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x23ce               ;Offset 0x23ce
    cbw
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x23d3               ;Offset 0x23d3
Label0x23ce:                            ;Offset 0x23ce
    push      ds
    call      ReadCharacterAndAttributeAtCursorPosition;Offset 0x1be9
    pop       ds
Label0x23d3:                            ;Offset 0x23d3
    mov       bh, ah
    xor       cx, cx
    mov       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec       dx
    mov       al, 01h
    call      ScrollUpWindow            ;Offset 0x1787
    jmp       Func0x2385                ;Offset 0x2385
Func0x23b2 ENDP

GetCurrentVideoMode PROC NEAR           ;Offset 0x23e3
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, BDA_VMO_DontClearDisplay;0x80
    or        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       ah, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    ret
GetCurrentVideoMode ENDP

RAMDACFunctions PROC NEAR               ;Offset 0x23f5
    mov       ah, 1bh
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;misassemble
    ja        EmptyFunction             ;Offset 0x2409
    cbw
    mov       si, ax
    shl       si, 01h
    push      es
    push      bx
    call      word ptr cs:[si + RAMDACCallTable];Offset 0x2410
    pop       bx
    pop       es
RAMDACFunctions ENDP

EmptyFunction PROC NEAR                 ;Offset 0x2409
    ret
EmptyFunction ENDP

Func0x240a PROC NEAR                    ;Offset 0x240a
    add       sp, 0004h
    pop       es
    ret
Func0x240a ENDP

    nop                                 ;hello

;Offset 0x2410
RAMDACCallTable         DW SetSinglePaletteRegister;Offset 0x2448       ;0x00
                        DW SetBorderColor;Offset 0x2465                 ;0x01
                        DW SetAllPaletteRegisters;Offset 0x2469         ;0x02
                        DW ToggleIntensityBlinkingBit;Offset 0x249b     ;0x03
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW GetIndividualPaletteRegister;Offset 0x24bd   ;0x07
                        DW GetBorderColor;Offset 0x24c7                 ;0x08
                        DW GetAllPaletteRegisters;Offset 0x24cb         ;0x09
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW SetIndividualDACRegister;Offset 0x24d1       ;0x10
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW SetBlockOfDACRegisters;Offset 0x24fe         ;0x12
                        DW SelectVideoDACColorPage;Offset 0x2517        ;0x13
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW GetIndividualDACRegister;Offset 0x253d       ;0x15
                        DW EmptyFunction;Offset 0x2409                  ;
                        DW GetBlockOfDACRegisters;Offset 0x255c         ;0x17
                        DW SetPELMask   ;Offset 0x2576                  ;0x18
                        DW GetPELMask   ;Offset 0x257e                  ;0x19
                        DW GetVideoDACColorPageState;Offset 0x258b      ;0x1a
                        DW PerformGrayScaleSumming;Offset 0x25a8        ;0x1b

SetSinglePaletteRegister PROC NEAR      ;Offset 0x2448
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Func0x2464                ;Offset 0x2464
SetSinglePaletteRegister ENDP
;continue!
Func0x244f PROC NEAR                    ;Offset 0x244f
    mov       al, bh
    mov       ah, bl
    call      Func0x30d9                ;Offset 0x30d9
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    je        Func0x2464                ;Offset 0x2464
    mov       al, bh
    xor       bh, bh
    mov       byte ptr es:[bx + di], al
Func0x244f ENDP
;continue!
Func0x2464 PROC NEAR                    ;Offset 0x2464
    ret
Func0x2464 ENDP

SetBorderColor PROC NEAR                ;Offset 0x2465
    mov       bl, 11h
    jmp       Func0x244f                ;Offset 0x244f
SetBorderColor ENDP

SetAllPaletteRegisters PROC NEAR        ;Offset 0x2469
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x249a               ;Offset 0x249a
    push      cx
    push      dx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    je        Label0x2485               ;Offset 0x2485
    pop       ds
    push      ds
    mov       si, dx
    mov       cx, 0011h
    rep movsb
    mov       ds, cx
Label0x2485:                            ;Offset 0x2485
    pop       es
    mov       si, dx
    mov       ax, 1000h
    mov       cx, 0011h
    call      Func0x31be                ;Offset 0x31be
    call      Func0x2ebb                ;Offset 0x2ebb
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
Label0x249a:                            ;Offset 0x249a
    ret
SetAllPaletteRegisters ENDP

ToggleIntensityBlinkingBit PROC NEAR    ;Offset 0x249b
    mov       ah, 10h
    call      Func0x30ed                ;Offset 0x30ed
    cmp       bl, 01h
    ja        Label0x24b7               ;Offset 0x24b7
    and       al, 0f7h
    and       byte ptr ds:[BDA_CRTModeControlRegValue], NOT BDA_CMCRV_Blinking;Offset 0x465 0xdf
    or        bl, bl
    je        Label0x24b7               ;Offset 0x24b7
    or        al, 08h
    or        byte ptr ds:[BDA_CRTModeControlRegValue], BDA_CMCRV_Blinking;Offset 0x465 0x20
Label0x24b7:                            ;Offset 0x24b7
    mov       ah, 10h
    call      Func0x30d9                ;Offset 0x30d9
    ret
ToggleIntensityBlinkingBit ENDP

GetIndividualPaletteRegister PROC NEAR  ;Offset 0x24bd
    mov       ah, bl
GetIndividualPaletteRegister ENDP
;continue!
Func0x24bf PROC NEAR                    ;Offset 0x24bf
    call      Func0x30ed                ;Offset 0x30ed
    mov       bh, al
    jmp       Func0x240a                ;Offset 0x240a
Func0x24bf ENDP

GetBorderColor PROC NEAR                ;Offset 0x24c7
    mov       ah, 11h
    jmp       Func0x24bf                ;Offset 0x24bf
GetBorderColor ENDP

GetAllPaletteRegisters PROC NEAR        ;Offset 0x24cb
    mov       di, dx
    call      Func0x2ed1                ;Offset 0x2ed1
    ret
GetAllPaletteRegisters ENDP

SetIndividualDACRegister PROC NEAR      ;Offset 0x24d1
    push      cx
    push      dx
    mov       ah, dh
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    je        Label0x24e1               ;Offset 0x24e1
    call      Func0x2fcc                ;Offset 0x2fcc
    mov       cx, ax
Label0x24e1:                            ;Offset 0x24e1
    call      Func0x31cc                ;Offset 0x31cc
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    jmp       Label0x24ec               ;Offset 0x24ec Speed sensitive!
Label0x24ec:                            ;Offset 0x24ec
    inc       dx
    mov       al, ah
    out       dx, al
    jmp       Label0x24f2               ;Offset 0x24f2 Speed sensitive!
Label0x24f2:                            ;Offset 0x24f2
    mov       al, ch
    out       dx, al
    jmp       Label0x24f7               ;Offset 0x24f7 Speed sensitive!
Label0x24f7:                            ;Offset 0x24f7
    mov       al, cl
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
SetIndividualDACRegister ENDP

SetBlockOfDACRegisters PROC NEAR        ;Offset 0x24fe
    push      cx
    push      dx
    mov       si, dx
    call      TurnScreenOff             ;Offset 0x3109
    push      ax
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    inc       dx
    call      Func0x2f93                ;Offset 0x2f93
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x310b
    pop       dx
    pop       cx
    ret
SetBlockOfDACRegisters ENDP

SelectVideoDACColorPage PROC NEAR       ;Offset 0x2517
    mov       ah, 10h
    call      Func0x30ed                ;Offset 0x30ed
    or        bl, bl
    jne       Label0x252b               ;Offset 0x252b
    shl       al, 01h
    add       bh, 0ffh
    rcr       al, 01h
    call      Func0x30d9                ;Offset 0x30d9
    ret
Label0x252b:                            ;Offset 0x252b
    or        al, al
    mov       al, bh
    js        Label0x2535               ;Offset 0x2535
    shl       al, 01h
    shl       al, 01h
Label0x2535:                            ;Offset 0x2535
    and       al, 0fh
    mov       ah, 14h
    call      Func0x30d9                ;Offset 0x30d9
    ret
SelectVideoDACColorPage ENDP

GetIndividualDACRegister PROC NEAR      ;Offset 0x253d
    mov       al, bl
    mov       bl, dl
    call      Func0x31cc                ;Offset 0x31cc
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    out       dx, al
    jmp       Label0x254a               ;Offset 0x254a
Label0x254a:                            ;Offset 0x254a Speed sensitive!
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x254f               ;Offset 0x254f Speed sensitive!
Label0x254f:                            ;Offset 0x254f
    mov       bh, al
    in        al, dx
    jmp       Label0x2554               ;Offset 0x2554 Speed sensitive!
Label0x2554:                            ;Offset 0x2554
    mov       ah, al
    in        al, dx
    sti
    xchg      ax, cx
    mov       dx, bx
    ret
GetIndividualDACRegister ENDP

GetBlockOfDACRegisters PROC NEAR        ;Offset 0x255c
    push      cx
    push      dx
    mov       di, dx
    call      TurnScreenOff             ;Offset 0x3109
    push      ax
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    call      Func0x2fc3                ;Offset 0x2fc3
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x310b
    pop       dx
    pop       cx
    ret
GetBlockOfDACRegisters ENDP

SetPELMask PROC NEAR                    ;Offset 0x2576
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    xchg      ax, bx
    out       dx, al
    pop       dx
    ret
SetPELMask ENDP

GetPELMask PROC NEAR                    ;Offset 0x257e
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    in        al, dx
    pop       dx
    xor       ah, ah
    xchg      ax, bx
    jmp       Func0x240a                ;Offset 0x240a
    ret
GetPELMask ENDP

GetVideoDACColorPageState PROC NEAR     ;Offset 0x258b
    mov       ah, 14h
    call      Func0x30ed                ;Offset 0x30ed
    and       al, 0fh
    mov       bh, al
    mov       ah, 10h
    call      Func0x30ed                ;Offset 0x30ed
    rol       al, 01h
    and       al, 01h
    mov       bl, al
    jne       Label0x25a5               ;Offset 0x25a5
    shr       bh, 01h
    shr       bh, 01h
Label0x25a5:                            ;Offset 0x25a5
    jmp       Func0x240a                ;Offset 0x240a
GetVideoDACColorPageState ENDP

PerformGrayScaleSumming PROC NEAR       ;Offset 0x25a8
    push      cx
    push      dx
    call      TurnScreenOff             ;Offset 0x3109
    push      ax
Label0x25ae:                            ;Offset 0x25ae
    push      cx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    jmp       Label0x25b7               ;Offset 0x25b7 Speed sensitive!
Label0x25b7:                            ;Offset 0x25b7
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x25bc               ;Offset 0x25bc Speed sensitive!
Label0x25bc:                            ;Offset 0x25bc
    mov       bh, al
    in        al, dx
    jmp       Label0x25c1               ;Offset 0x25c1 Speed sensitive!
Label0x25c1:                            ;Offset 0x25c1
    mov       ah, al
    in        al, dx
    jmp       Label0x25c6               ;Offset 0x25c6 Speed sensitive!
Label0x25c6:                            ;Offset 0x25c6
    xchg      ax, cx
    dec       dx
    mov       al, bl
    out       dx, al
    mov       dh, bh
    call      Func0x2fcc                ;Offset 0x2fcc
    mov       dx, VGA_RAMDACData        ;Port 0x3c9
    out       dx, al
    jmp       Label0x25d6               ;Offset 0x25d6 Speed sensitive!
Label0x25d6:                            ;Offset 0x25d6
    out       dx, al
    jmp       Label0x25d9               ;Offset 0x25d9 Speed sensitive!
Label0x25d9:                            ;Offset 0x25d9
    out       dx, al
    inc       bx
    pop       cx
    loop      Label0x25ae               ;Offset 0x25ae
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x310b
    pop       dx
    pop       cx
    ret
PerformGrayScaleSumming ENDP

TextFunctions PROC NEAR                 ;Offset 0x25e5
    lea       si, [TextFunctions0X]     ;Offset 0x2612
    cmp       al, 10h
    jb        Label0x2603               ;Offset 0x2603
    lea       si, [TextFunctions1X]     ;Offset 0x2633
    cmp       al, 20h
    jb        Label0x2603               ;Offset 0x2603
    lea       si, [TextFunctions2X]     ;Offset 0x264e
    cmp       al, 30h
    jb        Label0x2603               ;Offset 0x2603
    jne       Label0x2611               ;Offset 0x2611
    call      GetFontInformation        ;Offset 0x26b2
    ret
Label0x2603:                            ;Offset 0x2603
    push      bx
    push      cx
    push      dx
    push      es
    push      bp
    and       al, 0fh
    call      si
    pop       bp
    pop       es
    pop       dx
    pop       cx
    pop       bx
Label0x2611:                            ;Offset 0x2611
    ret
TextFunctions ENDP

TextFunctions0X PROC NEAR               ;Offset 0x2612
    cmp       al, 03h
    je        SelectCharacterMap        ;Offset 0x262a
    cmp       al, 04h
    ja        Return                    ;Offset 0x2632
    and       bl, 7fh
    call      Func0x2ce3                ;Offset 0x2ce3
    call      Func0x2d24                ;Offset 0x2d24
    call      Func0x2ce9                ;Offset 0x2ce9
    call      Func0x30c9                ;Offset 0x30c9
    ret
SelectCharacterMap:                     ;Offset 0x262a
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    mov       ah, bl
    out       dx, ax
Return:                                 ;Offset 0x2632
    ret
TextFunctions0X ENDP

TextFunctions1X PROC NEAR               ;Offset 0x2633
    cmp       al, 03h
    je        Label0x264d               ;Offset 0x264d
    cmp       al, 04h
    ja        Label0x264d               ;Offset 0x264d
    and       bl, 7fh
    call      Func0x2ce3                ;Offset 0x2ce3
    call      Func0x2d24                ;Offset 0x2d24
    call      Func0x2dc6                ;Offset 0x2dc6
    call      Func0x2ce9                ;Offset 0x2ce9
    call      Func0x30c9                ;Offset 0x30c9
Label0x264d:                            ;Offset 0x264d
    ret
TextFunctions1X ENDP

TextFunctions2X PROC NEAR               ;Offset 0x264e
    cmp       al, 04h
    ja        Label0x26b1               ;Offset 0x26b1
    cbw
    push      ax
    dec       ax
    je        Label0x267e               ;Offset 0x267e
    jns       Label0x2663               ;Offset 0x2663
    pop       ax
    mov       word ptr ds:[INT_1F_HandlerOfs], bp;Offset 0x7c
    mov       word ptr ds:[INT_1F_HandlerSeg], es;Offset 0x7e
    ret
Label0x2663:                            ;Offset 0x2663
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3f20
    mov       cx, 000eh
    dec       ax
    je        Label0x267e               ;Offset 0x267e
    lea       bp, [Font8x8]             ;Offset 0x3720
    mov       cl, 08h
    dec       ax
    je        Label0x267e               ;Offset 0x267e
    lea       bp, [Font8x16]            ;Offset 0x3f20
    mov       cl, 10h
Label0x267e:                            ;Offset 0x267e
    mov       word ptr ds:[INT_43_HandlerOfs], bp;Offset 0x10c
    mov       word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    pop       ax
    cmp       al, 01h
    je        Label0x2695               ;Offset 0x2695
    cmp       cl, 0eh
    jne       Label0x2695               ;Offset 0x2695
    mov       ax, 0bd70h
    int       6dh
Label0x2695:                            ;Offset 0x2695
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    mov       al, dl
    and       bx, 00ffh
    je        Label0x26ad               ;Offset 0x26ad
    mov       al, 19h
    cmp       bl, 03h
    ja        Label0x26ad               ;Offset 0x26ad
    mov       al, byte ptr cs:[bx + RowsOnScreen];Offset 0x370a
Label0x26ad:                            ;Offset 0x26ad
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x26b1:                            ;Offset 0x26b1
    ret
TextFunctions2X ENDP

GetFontInformation PROC NEAR            ;Offset 0x26b2
    mov       cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov       al, bh
    sub       al, 02h
    cbw
    jb        Label0x26dc               ;Offset 0x26dc
    cmp       al, 05h
    ja        Label0x26e8               ;Offset 0x26e8
    shl       ax, 01h
    mov       si, ax
    mov       bp, word ptr cs:[si + Data0x370e];Offset 0x370e
    push      cs
    pop       es
    or        ax, ax
    jne       Label0x26db               ;Offset 0x26db
    mov       ax, 0bd71h
    int       6dh
    xor       ax, ax
Label0x26db:                            ;Offset 0x26db
    ret
Label0x26dc:                            ;Offset 0x26dc
    inc       ax
    js        Label0x26e4               ;Offset 0x26e4
    les       bp, ds:[INT_43_HandlerOfs];Offset 0x10c
    ret
Label0x26e4:                            ;Offset 0x26e4
    les       bp, ds:[INT_1F_HandlerOfs];Offset 0x7c
Label0x26e8:                            ;Offset 0x26e8
    ret
GetFontInformation ENDP

AlternateFunctions PROC NEAR            ;Offset 0x26e9
    cmp       bl, 20h
    je        InstallAlternatePrintScreenHandler;Offset 0x271d
    cmp       bl, 10h
    je        GetEGAInfo                ;Offset 0x26f5
    jmp       Label0x272a               ;Offset 0x272a
GetEGAInfo:                             ;Offset 0x26f5
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shr       al, 01h
    mov       bh, al
    and       bh, 01h
    shr       al, 01h
    shr       al, 01h
    shr       al, 01h
    shr       al, 01h
    and       al, 03h
    mov       bl, al
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    mov       ch, al
    shr       ch, 01h
    shr       ch, 01h
    shr       ch, 01h
    shr       ch, 01h
    and       al, 0fh
    mov       cl, al
    ret
InstallAlternatePrintScreenHandler:     ;Offset 0x271d
    cli
    mov       word ptr ds:[INT_5_HandlerOfs], offset PrintScreenHandler;Offset 0x14 Offset 0x3240
    mov       word ptr ds:[INT_5_HandlerSeg], cs;Offset 0x16
    sti
    ret
Label0x272a:                            ;Offset 0x272a
    push      bx
    push      cx
    push      dx
    mov       cl, 03h
    xor       bh, bh
    sub       bl, 30h
    shl       bx, 01h
    je        Label0x2743               ;Offset 0x2743
    cmp       bl, 0ah
    je        Label0x2744               ;Offset 0x2744
    cmp       bl, 0ch
    ja        Label0x2755               ;Offset 0x2755
    dec       cx
Label0x2743:                            ;Offset 0x2743
    dec       cx
Label0x2744:                            ;Offset 0x2744
    ;cmp       al, cl
    DB 3Ah, 0C1h                        ;misassembly
    ja        Label0x2755               ;Offset 0x2755
    call      word ptr cs:[bx + AlternateFunctionsTable];Offset 0x275a
    jb        Label0x2755               ;Offset 0x2755
    mov       al, 12h
Label0x2751:                            ;Offset 0x2751
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x2755:                            ;Offset 0x2755
    mov       al, 00h
    jmp       Label0x2751               ;Offset 0x2751
AlternateFunctions ENDP

    nop                                 ;hello

;Offset 0x275a
AlternateFunctionsTable DW SelectVerticalResolution;Offset 0x27de
                        DW SetPaletteLoading;Offset 0x2768
                        DW SetVGAMemoryIOMapping;Offset 0x2776
                        DW SetGrayscaleSumming;Offset 0x27b4
                        DW SetCursorEmulation;Offset 0x27c2
                        DW DisplaySwitchInterface;Offset 0x2822
                        DW TurnScreenOnOff;Offset 0x27d0

SetPaletteLoading PROC NEAR             ;Offset 0x2768
    je   Enable                         ;Offset 0x2770
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0xf7
    ret
Enable:                                 ;Offset 0x2770
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    ret
SetPaletteLoading ENDP

;
;inputs:
;   zero flag = 1 = disable VGA IO Memory Map, 0 = enable VGA IO Memory Map
;
;outputs:
;   carry flag = 0
;
;destroys:
;   dx, di, bx, cl
;
SetVGAMemoryIOMapping PROC NEAR         ;Offset 0x2776
    mov  dl, 00h
    je   Disable                        ;Offset 0x277c
    dec  dl
Disable:                                ;Offset 0x277c
    call FindMystique                   ;Offset 0x57c6
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_Header_Word_Command    ;0x4
    call AccessPCIRegister              ;Offset 0x5734
    and  cl, NOT PCI_Cmd_MemSpace       ;0xfd
    and  dl, PCI_Cmd_IOSpace OR PCI_Cmd_MemSpace;0x3
    je   DisableMemSpace                ;Offset 0x2792
    or   cl, dl
DisableMemSpace:                        ;Offset 0x2792
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x5734
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_MGA_Option + 01h       ;0x41
    nop
    call AccessPCIRegister              ;Offset 0x5734
    and  cl, NOT (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0xfe
    and  dl, (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0x1
    je   DoWrite                        ;Offset 0x27ac
    or   cl, dl                         ;OR in the new value (one)
DoWrite:                                ;Offset 0x27ac
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x5734
    clc  
    ret  
SetVGAMemoryIOMapping ENDP

SetGrayscaleSumming PROC NEAR           ;Offset 0x27b4
    je   Disable                        ;Offset 0x27bc
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0x489 0x2
    ret
Disable:                                ;Offset 0x27bc
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489 0xfd
    ret
SetGrayscaleSumming ENDP

SetCursorEmulation PROC NEAR            ;Offset 0x27c2
    je   Enable                         ;Offset 0x27ca
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487 0xfe
    ret
Enable:                                 ;Offset 0x27ca
    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_CursorEmulationEnabled;Offset 0x487 0x1
    ret
SetCursorEmulation ENDP

;
;inputs:
;   zero flag: 0 = Off, 1 = On
;
;outputs:
;   -
;
TurnScreenOnOff PROC NEAR               ;Offset 0x27d0
    lea  dx, [TurnScreenOff]            ;Offset 0x3109
    je   TurnOff                        ;Offset 0x27da
    lea  dx, [TurnScreenOn]             ;Offset 0x3105
TurnOff:                                ;Offset 0x27da
    call dx
    clc
    ret
TurnScreenOnOff ENDP

SelectVerticalResolution PROC NEAR      ;Offset 0x27de
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x2803               ;Offset 0x2803
    cbw
    xchg      ax, si
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, NOT BDA_VDDA_LineMode400;0xef
    test      al, BDA_VDDA_VGA          ;0x1
    je        Label0x27f2               ;Offset 0x27f2
    and       al, NOT BDA_VDDA_LineMode200;0x7f
Label0x27f2:                            ;Offset 0x27f2
    mov       bl, BDA_EFBS_MDAHiResEnhanced_2;0x9
    dec       si
    jns       Label0x2807               ;Offset 0x2807
    dec       bx
    test      al, BDA_VDDA_VGA          ;0x1
    jne       Label0x2805               ;Offset 0x2805
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x2810               ;Offset 0x2810
Label0x2803:                            ;Offset 0x2803
    stc
    ret
Label0x2805:                            ;Offset 0x2805
    or        al, BDA_VDDA_LineMode200  ;0x80
Label0x2807:                            ;Offset 0x2807
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x2810               ;Offset 0x2810
    mov       bl, BDA_EFBS_CGAMono80x25_2;0xb
Label0x2810:                            ;Offset 0x2810
    dec       si
    js        Label0x2815               ;Offset 0x2815
    or        al, BDA_VDDA_LineMode400  ;0x10
Label0x2815:                            ;Offset 0x2815
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    ret
SelectVerticalResolution ENDP

DisplaySwitchInterface PROC NEAR        ;Offset 0x2822
    cbw
    xchg      ax, si
    dec       si
    je        Label0x2874               ;Offset 0x2874
    js        Label0x2840               ;Offset 0x2840
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2874               ;Offset 0x2874
    dec       si
    je        Label0x2853               ;Offset 0x2853
    mov       si, dx
    call      RestoreBiosDataAreaVideoSections;Offset 0x2bfa
    mov       al, 0eh
Label0x283a:                            ;Offset 0x283a
    mov       dx, 46e8h                 ;Port 0x46e8 Some kind of video subsystem enable port?
    out       dx, al
    clc
    ret
Label0x2840:                            ;Offset 0x2840
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    jne       Label0x2874               ;Offset 0x2874
    mov       ax, 1280h
    int       42h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2874               ;Offset 0x2874
Label0x2853:                            ;Offset 0x2853
    mov       di, dx
    call      BackupBiosDataAreaVideoSections;Offset 0x2b17
    push      es
    les       bx, ds:[INT_42_HandlerOfs];Offset 0x108
    mov       word ptr ds:[INT_6D_HandlerOfs], bx;Offset 0x1b4
    mov       word ptr ds:[INT_6D_HandlerSeg], es;Offset 0x1b6
    pop       es
    mov       word ptr ds:[INT_42_HandlerOfs], Int6DHandler;Offset 0x108 Offset 0xb70
    mov       word ptr ds:[INT_42_HandlerSeg], cs;Offset 0x10a
    mov       al, 06h
    jmp       Label0x283a               ;Offset 0x283a
Label0x2874:                            ;Offset 0x2874
    stc
    ret
DisplaySwitchInterface ENDP

WriteString PROC NEAR                   ;Offset 0x2876
    push      bx
    push      cx
    push      dx
    push      bp
    cmp       al, 03h
    ja        Label0x28bc               ;Offset 0x28bc
    jcxz      Label0x28bc               ;Offset 0x28bc
    cbw
    mov       di, ax
    dec       di
    mov       al, bh
    mov       si, ax
    shl       si, 1
    push      word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    push      si
    call      SetCursorPosition         ;Offset 0x1700
Label0x2892:                            ;Offset 0x2892
    mov       al, byte ptr es:[bp]
    inc       bp
    cmp       al, 0dh
    jbe       Label0x28c1               ;Offset 0x28c1
Label0x289b:                            ;Offset 0x289b
    or        di, di
    jle       Label0x28a4               ;Offset 0x28a4
    mov       bl, byte ptr es:[bp]
    inc       bp
Label0x28a4:                            ;Offset 0x28a4
    call      Func0x28ed                ;Offset 0x28ed
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae       Label0x28da               ;Offset 0x28da
Label0x28ae:                            ;Offset 0x28ae
    call      SetCursorPosition         ;Offset 0x1700
Label0x28b1:                            ;Offset 0x28b1
    loop      Label0x2892               ;Offset 0x2892
    pop       si
    pop       dx
    shr       di, 01h
    jae       Label0x28bc               ;Offset 0x28bc
    call      SetCursorPosition         ;Offset 0x1700
Label0x28bc:                            ;Offset 0x28bc
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x28c1:                            ;Offset 0x28c1
    je        Label0x28cf               ;Offset 0x28cf
    cmp       al, 0ah
    je        Label0x28cf               ;Offset 0x28cf
    cmp       al, 08h
    je        Label0x28cf               ;Offset 0x28cf
    cmp       al, 07h
    jne       Label0x289b               ;Offset 0x289b
Label0x28cf:                            ;Offset 0x28cf
    call      Func0x28fa                ;Offset 0x28fa
    pop       si
    push      si
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x28b1               ;Offset 0x28b1
Label0x28da:                            ;Offset 0x28da
    xor       dl, dl
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Label0x28ae               ;Offset 0x28ae
    dec       dh
    mov       al, 0ah
    call      Func0x28fa                ;Offset 0x28fa
    jmp       Label0x28ae               ;Offset 0x28ae
WriteString ENDP

Func0x28ed PROC NEAR                    ;Offset 0x28ed
    push      cx
    push      di
    push      ds
    mov       cx, 0001h
    call      WriteCharacterAndAttributeAtCursorPosition;Offset 0x1db6
    pop       ds
    pop       di
    pop       cx
    ret
Func0x28ed ENDP

Func0x28fa PROC NEAR                    ;Offset 0x28fa
    push      di
    push      ds
    call      Func0x238a                ;Offset 0x238a
    pop       ds
    pop       di
    ret
Func0x28fa ENDP

NotImplemented PROC NEAR                ;Offset 0x2902
    ret
NotImplemented ENDP

DisplayCombinationCodeFunctions PROC NEAR;Offset 0x2903
    push      cx
    push      dx
    push      es
    xor       ah, ah
    dec       ax
    js        IsGet                     ;Offset 0x2912
    jne       NotSupported              ;Offset 0x2917
    call      SetDisplayCombinationCodeIndex;Offset 0x3120
    jmp       Done                      ;Offset 0x2915
IsGet:                                  ;Offset 0x2912
    call      GetDisplayCombinationCode ;Offset 0x3147
Done:                                   ;Offset 0x2915
    mov       al, 1ah
NotSupported:                           ;Offset 0x2917
    pop       es
    pop       dx
    pop       cx
    ret
DisplayCombinationCodeFunctions ENDP

FunctionalityAndStateInfo PROC NEAR     ;Offset 0x291b
    or        bx, bx
    je        Label0x2922               ;Offset 0x2922
    xor       al, al
    ret
Label0x2922:                            ;Offset 0x2922
    push      bx
    push      cx
    push      dx
    lea       ax, [StaticFunctionalityTable];Offset 0x476
    stosw
    mov       ax, cs
    stosw
    lea       si, ds:[BDA_DisplayMode]  ;Offset 0x449
    mov       cx, 001eh
    rep movsb
    lea       si, ds:[BDA_RowsOnScreen] ;Offset 0x484
    lodsb
    inc       ax
    stosb
    movsw
    call      GetDisplayCombinationCode ;Offset 0x3147
    xchg      ax, bx
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    push      ax
    lea       bx, [ColorsPerModePlusOne];Offset 0x2a00
    xlatb     cs:[bx]
    dec       ax
    jns       ColorsUnder256            ;Offset 0x2954
    mov       ax, 0100h                 ;Set colors to 256 (mode 0x13)
ColorsUnder256:                         ;Offset 0x2954
    stosw
    pop       ax
    lea       bx, [NumberOfPagesPerMode];Offset 0x2a14
    xlatb     cs:[bx]
    stosb
    call      GetNumberOfActiveScanlines;Offset 0x3054
    stosb
    mov       cl, 02h
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    out       dx, al
    inc       dx
    in        al, dx
    mov       ah, al
    mov       bl, al
    shr       ax, cl
    mov       bh, ah
    shr       bh, 01h
    and       bx, 0403h
    and       ax, 0304h
    or        ax, bx
    stosw
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    in        al, dx
    mov       dl, VGA_AttributeControllerDataW_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource OR VGA_ATTR_ModeControl;0x30
    out       dx, al
    inc       dx
    in        al, dx
    mov       ah, al
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    mov       bl, al
    inc       ax
    shl       ax, cl
    shl       al, cl
    and       ax, 2010h
    or        ah, al
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_SettingLowerMask;0xf
    or        al, ah
    stosb
    xor       ax, ax
    stosw
    stosb
    xchg      ax, bx
    inc       cx
    rol       al, cl
    and       al, 03h
    stosb
    push      es
    push      di
    xor       dx, dx
    mov       al, 02h
    call      Func0x29ee                ;Offset 0x29ee
    mov       al, 0ah
    call      Func0x29e5                ;Offset 0x29e5
    mov       al, 0ch
    call      Func0x29e5                ;Offset 0x29e5
    mov       al, 08h
    call      Func0x29e5                ;Offset 0x29e5
    mov       al, 04h
    call      Func0x29e5                ;Offset 0x29e5
    mov       al, 06h
    call      Func0x29e5                ;Offset 0x29e5
    xchg      ax, dx
    pop       di
    pop       es
    stosb
    xor       ax, ax
    mov       cl, 06h
    rep stosw
    stosb
    mov       al, 1bh
    pop       dx
    pop       cx
    pop       bx
    ret
FunctionalityAndStateInfo ENDP

Func0x29e5 PROC NEAR                    ;Offset 0x29e5
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    je        Label0x29ed               ;Offset 0x29ed
    inc       dx
Label0x29ed:                            ;Offset 0x29ed
    ret
Func0x29e5 ENDP

Func0x29ee PROC NEAR                    ;Offset 0x29ee
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    je        Label0x29fe               ;Offset 0x29fe
    mov       ax, cs
    mov       bx, es
    cmp       ax, bx
    je        Label0x29fe               ;Offset 0x29fe
    inc       dx
Label0x29fe:                            ;Offset 0x29fe
    ret
Func0x29ee ENDP

    nop                                 ;hello

;Offset 0x2a00
ColorsPerModePlusOne    DB 011h, 011h, 011h, 011h, 005h, 005h, 003h, 001h, 001h, 001h
                        DB 001h, 001h, 001h, 011h, 011h, 001h, 011h, 003h, 011h, 000h

;Offset 0x2a14
NumberOfPagesPerMode    DB 008h, 008h, 008h, 008h, 001h, 001h, 001h, 008h, 000h, 000h
                        DB 000h, 000h, 000h, 008h, 004h, 002h, 002h, 001h, 001h, 001h 

SaveRestoreVideoState PROC NEAR         ;Offset 0x2a28
    push      cx
    test      cl, 07h
    je        Label0x2a39               ;Offset 0x2a39
    cbw
    inc       ax
    dec       ax
    je        Label0x2a3d               ;Offset 0x2a3d
    dec       ax
    je        Label0x2a63               ;Offset 0x2a63
    dec       ax
    je        Label0x2a7f               ;Offset 0x2a7f
Label0x2a39:                            ;Offset 0x2a39
    mov       al, 00h
    pop       cx
    ret
Label0x2a3d:                            ;Offset 0x2a3d
    xor       ax, ax
    shr       cl, 01h
    jae       Label0x2a45               ;Offset 0x2a45
    add       al, 46h
Label0x2a45:                            ;Offset 0x2a45
    shr       cl, 01h
    jae       Label0x2a4b               ;Offset 0x2a4b
    add       al, 3ah
Label0x2a4b:                            ;Offset 0x2a4b
    shr       cl, 01h
    jae       Label0x2a52               ;Offset 0x2a52
    add       ax, 0303h
Label0x2a52:                            ;Offset 0x2a52
    add       al, 20h
    mov       cl, 06h
    ror       ax, cl
    add       ah, 0ffh
    adc       al, 00h
    cbw
    xchg      ax, bx
Label0x2a5f:                            ;Offset 0x2a5f
    mov       al, 1ch
    pop       cx
    ret
Label0x2a63:                            ;Offset 0x2a63
    push      dx
    lea       di, [bx + 20h]
    shr       cl, 01h
    jae       Label0x2a6e               ;Offset 0x2a6e
    call      Func0x2a98                ;Offset 0x2a98
Label0x2a6e:                            ;Offset 0x2a6e
    shr       cl, 01h
    jae       Label0x2a75               ;Offset 0x2a75
    call      Func0x2b13                ;Offset 0x2b13
Label0x2a75:                            ;Offset 0x2a75
    shr       cl, 01h
    jae       Label0x2a7c               ;Offset 0x2a7c
    call      Func0x2b50                ;Offset 0x2b50
Label0x2a7c:                            ;Offset 0x2a7c
    pop       dx
    jmp       Label0x2a5f               ;Offset 0x2a5f
Label0x2a7f:                            ;Offset 0x2a7f
    push      dx
    shr       cl, 01h
    jae       Label0x2a87               ;Offset 0x2a87
    call      Func0x2b75                ;Offset 0x2b75
Label0x2a87:                            ;Offset 0x2a87
    shr       cl, 01h
    jae       Label0x2a8e               ;Offset 0x2a8e
    call      Func0x2bf6                ;Offset 0x2bf6
Label0x2a8e:                            ;Offset 0x2a8e
    shr       cl, 01h
    jae       Label0x2a95               ;Offset 0x2a95
    call      Func0x2c24                ;Offset 0x2c24
Label0x2a95:                            ;Offset 0x2a95
    pop       dx
    jmp       Label0x2a5f               ;Offset 0x2a5f
SaveRestoreVideoState ENDP

Func0x2a98 PROC NEAR                    ;Offset 0x2a98
    push      cx
    mov       word ptr es:[bx], di
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    in        al, dx
    stosb
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       si, dx
    in        al, dx
    stosb
    add       dl, 06h                   ;Port 0x3?a
    in        al, dx
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    in        al, dx
    stosb
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    in        al, dx
    stosb
    mov       dl, VGA_FeatureControlR_lowbyte;Port 0x3ca
    in        al, dx
    stosb
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       cx, 0004h
    mov       al, 01h
    call      Func0x2eae                ;Offset 0x2eae
    mov       dl, VGA_MiscellaneousRead_lowbyte;Port 0x3cc
    in        al, dx
    stosb
    mov       dx, si
    mov       cl, 19h
    call      Func0x2eac                ;Offset 0x2eac
    mov       ah, 00h
    mov       cl, 14h
Label0x2ad2:                            ;Offset 0x2ad2
    call      Func0x30ed                ;Offset 0x30ed
    stosb
    inc       ah
    loop      Label0x2ad2               ;Offset 0x2ad2
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    call      Func0x2eac                ;Offset 0x2eac
    mov       ax, si
    stosw
    xchg      ax, dx
    mov       al, 22h
    out       dx, al
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ4_MemSize256k SHL 8) OR VGA_SEQIdx_MemoryMode;0x204
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, al
    inc       dx
    inc       si
    mov       al, 00h
Label0x2af7:                            ;Offset 0x2af7
    out       dx, al
    push      ax
    xchg      dx, si
    in        al, dx
    stosb
    xchg      dx, si
    pop       ax
    inc       ax
    cmp       al, 03h
    jbe       Label0x2af7               ;Offset 0x2af7
    mov       al, byte ptr es:[di - 0bh]
    out       dx, al
    mov       dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov       al, byte ptr es:[di - 3eh]
    out       dx, al
    pop       cx
    ret
Func0x2a98 ENDP

Func0x2b13 PROC NEAR                    ;Offset 0x2b13
    mov       word ptr es:[bx + 02h], di
Func0x2b13 ENDP
;continue!
BackupBiosDataAreaVideoSections PROC NEAR;Offset 0x2b17
    push      cx
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    lea       si, [BiosDataAreaVideoSections];Offset 0x2b32
ProcessNextSection:                     ;Offset 0x2b22
    lodsw     word ptr cs:[si]          ;Count
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2b2f
    lodsw     word ptr cs:[si]          ;Source address
    xchg      ax, si
    rep movsb
    xchg      ax, si
    jmp       ProcessNextSection        ;Offset 0x2b22
Return:                                 ;Offset 0x2b2f
    pop       cx
    ret
BackupBiosDataAreaVideoSections ENDP

    nop                                 ;https://i.imgur.com/NGkg1Ip.jpeg

;Offset 0x2b32
BiosDataAreaVideoSections   DW 001eh, BDA_DisplayMode;Offset 0x449
                            DW 0007h, BDA_RowsOnScreen;Offset 0x484
                            DW 0004h, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
                            DW 0004h, INT_5_HandlerOfs;Offset 0x014
                            DW 0004h, INT_1D_HandlerOfs;Offset 0x074
                            DW 0004h, INT_1F_HandlerOfs;Offset 0x07c
                            DW 0004h, INT_43_HandlerOfs;Offset 0x10c
                            DW 0000h

Func0x2b50 PROC NEAR                    ;Offset 0x2b50
    push      cx
    mov       word ptr es:[bx + 04h], di
    mov       dx, VGA_DACStatus         ;Port 0x3c7
    in        al, dx
    and       al, 01h
    stosb
    inc       dx
    shr       al, 01h
    in        al, dx
    sbb       al, 00h
    stosb
    mov       dl, VGA_DACMask_lowbyte   ;Port 0x3c6
    in        al, dx
    stosb
    inc       dx
    mov       al, 00h
    out       dx, al
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    mov       cx, 0100h
    call      Func0x2fc3                ;Offset 0x2fc3
    pop       cx
    ret
Func0x2b50 ENDP

Func0x2b75 PROC NEAR                    ;Offset 0x2b75
    push      cx
    mov       si, word ptr es:[bx]
    add       si, 0042h
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    mov       ax, (VGA_GCTL6_Mem_A0000_AFFFF SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x406
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x704
    out       dx, ax
    mov       al, VGA_SEQIdx_MapMask    ;0x2
    out       dx, al
    inc       dx
    push      ds
    mov       ax, 0a000h                ;Segment 0xa000
    mov       ds, ax
    mov       di, 0ffffh
    mov       al, VGA_SEQ2_Mask0        ;0x1
    out       dx, al
    lodsb     byte ptr es:[si]
    mov       byte ptr [di], al
    mov       al, VGA_SEQ2_Mask1        ;0x2
    out       dx, al
    lodsb     byte ptr es:[si]
    mov       byte ptr [di], al
    mov       al, VGA_SEQ2_Mask2        ;0x4
    out       dx, al
    lodsb     byte ptr es:[si]
    mov       byte ptr [di], al
    mov       al, VGA_SEQ2_Mask3        ;0x8
    out       dx, al
    lodsb     byte ptr es:[si]
    mov       byte ptr [di], al
    mov       al, VGA_SEQ2_MaskAll      ;0xf
    out       dx, al
    mov       al, byte ptr [di]
    pop       ds
    sub       si, 0041h
    mov       dx, word ptr es:[si + 3bh]
    call      Func0x2e35                ;Offset 0x2e35
    add       dl, 06h
    mov       al, byte ptr es:[si - 1fh]
    out       dx, al
    xor       ax, ax
    mov       cx, 0010h
    call      Func0x2eb9                ;Offset 0x2eb9
    inc       ax
    inc       si
    mov       cl, 01h
    call      Func0x2eb9                ;Offset 0x2eb9
    sub       si, 0035h
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dx, di
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dl, VGA_AttributeControllerDataW_lowbyte;Port 0x3c0
    lodsb     byte ptr es:[si]
    out       dx, al
    pop       cx
    ret
Func0x2b75 ENDP

Func0x2bf6 PROC NEAR                    ;Offset 0x2bf6
    mov       si, word ptr es:[bx + 02h]
Func0x2bf6 ENDP
;continue!
RestoreBiosDataAreaVideoSections PROC NEAR;Offset 0x2bfa
    push      cx
    push      ds
    push      es
    lodsb     byte ptr es:[si]
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    push      ds
    push      es
    pop       ds
    pop       es
    lea       dx, [BiosDataAreaVideoSections];Offset 0x2b32
ProcessNextSection:                     ;Offset 0x2c10
    xchg      si, dx
    lodsw     word ptr cs:[si]
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2c20
    lodsw     word ptr cs:[si]
    xchg      ax, di
    xchg      si, dx
    rep movsb
    jmp       ProcessNextSection        ;Offset 0x2c10
Return:                                 ;Offset 0x2c20
    pop       es
    pop       ds
    pop       cx
    ret
RestoreBiosDataAreaVideoSections ENDP

Func0x2c24 PROC NEAR                    ;Offset 0x2c24
    push      cx
    mov       si, word ptr es:[bx + 04h]
    add       si, 0002h
    mov       dx, VGA_DACMask           ;Port 0x3c6
    lodsb     byte ptr es:[si]
    out       dx, al
    mov       dl, VGA_DACWriteIndex_lowbyte;Port 0x3c8
    mov       al, 00h
    out       dx, al
    inc       dx
    mov       cx, 0100h
    call      Func0x2f93                ;Offset 0x2f93
    sub       si, 0303h
    lodsw     word ptr es:[si]
    xchg      al, ah
    shr       ah, 01h
    sbb       dl, 01h
    out       dx, al
    pop       cx
    ret
Func0x2c24 ENDP

Func0x2c4e PROC NEAR                    ;Offset 0x2c4e
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    xor       dh, dh
    add       ax, dx
    shl       ax, 01h
    ret
Func0x2c4e ENDP

Func0x2c5c PROC NEAR                    ;Offset 0x2c5c
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    shl       ax, 01h
    shl       ax, 01h
    xor       dh, dh
    add       ax, dx
    ret
Func0x2c5c ENDP

Func0x2c6c PROC NEAR                    ;Offset 0x2c6c
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    pop       dx
    xor       dh, dh
    add       ax, dx
    ret
Func0x2c6c ENDP

Func0x2c7e PROC NEAR                    ;Offset 0x2c7e
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    pop       dx
    xor       dh, dh
    add       ax, dx
    shl       ax, 01h
    shl       ax, 01h
    shl       ax, 01h
    ret
Func0x2c7e ENDP

;
;inputs:
;   al = Cursor End
;   ah = Cursor Start
;
;outputs:
;   -
;
;destroys:
;   ax, dx
;
SetCursorEndStart PROC NEAR             ;Offset 0x2c96
    push      ax
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       al, VGA_CRTCIdx_CursorStart;0xa
    out       dx, ax
    pop       ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_CursorEnd ;0xb
    out       dx, ax
    ret
SetCursorEndStart ENDP

ClearScreen PROC NEAR                   ;Offset 0x2ca5
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487 0x80
    jne       Label0x2ce2               ;Offset 0x2ce2
    cmp       word ptr ds:[BDA_VideoBufferSize], 00h;Offset 0x44c
    je        Label0x2ce2               ;Offset 0x2ce2
    push      bx
    push      cx
    push      dx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x64cd                ;Offset 0x64cd
    mov       bx, 0a000h                ;Segment 0xa000
    mov       cx, 8000h
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x2cd5               ;Offset 0x2cd5
    mov       bh, 0b0h                  ;Segment 0xb000
    mov       ch, 40h
    mov       dx, 0720h                 ;Attribute + space
    je        Label0x2cd7               ;Offset 0x2cd7
    mov       bh, 0b8h                  ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x2cd7               ;Offset 0x2cd7
Label0x2cd5:                            ;Offset 0x2cd5
    xor       dx, dx
Label0x2cd7:                            ;Offset 0x2cd7
    mov       es, bx
    mov       ax, dx
    xor       di, di
    rep stosw
    pop       dx
    pop       cx
    pop       bx
Label0x2ce2:                            ;Offset 0x2ce2
    ret
ClearScreen ENDP

Func0x2ce3 PROC NEAR                    ;Offset 0x2ce3
    lea       si, [Data0x2d0e]          ;Offset 0x2d0e
    jmp       Func0x2ced                ;Offset 0x2ced
Func0x2ce3 ENDP

Func0x2ce9 PROC NEAR                    ;Offset 0x2ce9
    lea       si, [Data0x2d18]          ;Offset 0x2d18
Func0x2ce9 ENDP
;continue!
Func0x2ced PROC NEAR                    ;Offset 0x2ced
    push      ax
    push      dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    lodsw     word ptr cs:[si]
    out       dx, ax
    lodsw     word ptr cs:[si]
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    lodsw     word ptr cs:[si]
    out       dx, ax
    lodsw     word ptr cs:[si]
    out       dx, ax
Label0x2d00:                            ;Offset 0x2d00
    lodsw     word ptr cs:[si]
    out       dx, ax
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       ax, 0eb4h
    je        Label0x2d00               ;Offset 0x2d00
    pop       dx
    pop       ax
    ret
Func0x2ced ENDP

;Offset 0x2d0e
Data0x2d0e              DW (VGA_SEQ2_Mask2 SHL 8) OR VGA_SEQIdx_MapMask;0x0402
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0704
                        DW (VGA_GCTL4_Map2 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0204
                        DW (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x0005
                        DW (VGA_GCTL6_Mem_A0000_AFFFF SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0406

;Offset 0x2d18
Data0x2d18              DW ((VGA_SEQ2_Mask0 OR VGA_SEQ2_Mask1) SHL 8)OR VGA_SEQIdx_MapMask;0x0302
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0304
                        DW (VGA_GCTL4_Map0 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0004
                        DW (VGA_GCTL5_OddEven SHL 8) OR VGA_GCTLIdx_GraphicMode;0x1005
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B8000_BFFFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0e06
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B0000_B7FFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0a06

Func0x2d24 PROC NEAR                    ;Offset 0x2d24
    cbw
    dec       ax
    js        Func0x2d47                ;Offset 0x2d47
    mov       cx, 0100h
    xor       dx, dx
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3f20
    mov       bh, 00h
    dec       ax
    js        Func0x2d47                ;Offset 0x2d47
    lea       bp, [Font8x8]             ;Offset 0x3720
    mov       bh, 08h
    dec       ax
    js        Func0x2d47                ;Offset 0x2d47
    lea       bp, [Font8x16]            ;Offset 0x3f20
    mov       bh, 10h
Func0x2d24 ENDP
;continue!
Func0x2d47 PROC NEAR                    ;Offset 0x2d47
    mov       si, bp
    push      es
    pop       ds
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    jcxz      Label0x2da6               ;Offset 0x2da6
Label0x2d52:                            ;Offset 0x2d52
    push      cx
    mov       cl, 05h
    shl       dx, cl
    push      bx
    and       bx, 0007h
    add       dh, byte ptr cs:[bx + Data0x2dbe];Offset 0x2dbe
    pop       bx
    mov       di, dx
    mov       dl, bh
    xor       dh, dh
    or        dl, dl
    jne       Label0x2d6d               ;Offset 0x2d6d
    mov       dl, 10h
Label0x2d6d:                            ;Offset 0x2d6d
    pop       ax
Label0x2d6e:                            ;Offset 0x2d6e
    or        bh, bh
    jne       Label0x2d9a               ;Offset 0x2d9a
    cmp       al, 99h
    je        Label0x2d8c               ;Offset 0x2d8c
    cmp       al, 96h
    je        Label0x2d8c               ;Offset 0x2d8c
    cmp       al, 87h
    je        Label0x2d8c               ;Offset 0x2d8c
    cmp       al, 90h
    je        Label0x2d8c               ;Offset 0x2d8c
    cmp       al, 8fh
    je        Label0x2d8c               ;Offset 0x2d8c
    cmp       al, 68h
    je        Label0x2d8c               ;Offset 0x2d8c
    jmp       Label0x2d9a               ;Offset 0x2d9a
Label0x2d8c:                            ;Offset 0x2d8c
    mov       cx, 0008h
    rep movsb
    inc       si
    mov       cx, 0007h
    rep movsb
    inc       di
    jmp       Label0x2d9e               ;Offset 0x2d9e
Label0x2d9a:                            ;Offset 0x2d9a
    mov       cx, dx
    rep movsb
Label0x2d9e:                            ;Offset 0x2d9e
    sub       di, dx
    add       di, 0020h
    dec       ax
    jne       Label0x2d6e               ;Offset 0x2d6e
Label0x2da6:                            ;Offset 0x2da6
    or        bl, bl
    jns       Label0x2db3               ;Offset 0x2db3
    mov       dl, byte ptr [si]
    inc       si
    inc       cx
    or        dl, dl
    jne       Label0x2d52               ;Offset 0x2d52
    dec       cx
Label0x2db3:                            ;Offset 0x2db3
    or        bh, bh
    jne       Label0x2db9               ;Offset 0x2db9
    mov       bh, 0eh
Label0x2db9:                            ;Offset 0x2db9
    push      ds
    pop       es
    mov       ds, cx
    ret
Func0x2d47 ENDP

;Offset 0x2dbe
Data0x2dbe              DB 000h, 040h, 080h, 0C0h, 020h, 060h, 0A0h, 0E0h

Func0x2dc6 PROC NEAR                    ;Offset 0x2dc6
    mov       al, bh
    cbw
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    dec       ax
    mov       ah, al
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne       Label0x2ddd               ;Offset 0x2ddd
    mov       al, VGA_CRTCIdx_UnderlineLoc;0x14
    out       dx, ax
Label0x2ddd:                            ;Offset 0x2ddd
    mov       al, VGA_CRTCIdx_MaxScanLine;0x9
    out       dx, al
    inc       dx
    in        al, dx
    and       al, NOT VGA_CRTC9_MaxScanMask;0xe0
    or        al, ah
    out       dx, al
    mov       al, ah
    dec       ah
    cmp       al, 0dh
    jb        Label0x2df2               ;Offset 0x2df2
    sub       ax, 0101h
Label0x2df2:                            ;Offset 0x2df2
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call      SetCursorEndStart         ;Offset 0x2c96
    call      GetNumberOfActiveScanlines;Offset 0x3054
    dec       al
    mov       ax, 00c8h
    js        Label0x2e0a               ;Offset 0x2e0a
    mov       ax, 015eh
    je        Label0x2e0a               ;Offset 0x2e0a
    mov       ax, 0190h
Label0x2e0a:                            ;Offset 0x2e0a
    div       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    inc       ax
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp       ax, 00c8h
    ja        Label0x2e1e               ;Offset 0x2e1e
    shl       ax, 01h
Label0x2e1e:                            ;Offset 0x2e1e
    dec       ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_VertDispEnableEnd;0x12
    out       dx, ax
    mov       al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    inc       ax
    mul       byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       ax, 01h
    add       ax, 0100h
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    ret
Func0x2dc6 ENDP

Func0x2e35 PROC NEAR                    ;Offset 0x2e35
    push      cx
    push      dx
    jmp       Func0x2e45                ;Offset 0x2e45
Func0x2e35 ENDP

Func0x2e39 PROC NEAR                    ;Offset 0x2e39
    push      cx
    push      dx
    call      Func0x2fe4                ;Offset 0x2fe4
    add       si, 0005h
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
Func0x2e39 ENDP
;continue!
Func0x2e45 PROC NEAR                    ;Offset 0x2e45
    mov       di, dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       cx, 0004h
    mov       ax, (VGA_SEQ0_AsyncReset SHL 8) OR VGA_SEQIdx_Reset;0x100
    cli
    out       dx, ax
    inc       ax
    call      Func0x2ea3                ;Offset 0x2ea3
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    lodsb     byte ptr es:[si]
    out       dx, al
    push      cx
    mov       cx, 8000h
Label0x2e5f:                            ;Offset 0x2e5f
    loop      Label0x2e5f               ;Offset 0x2e5f  Speed sensitive!
    pop       cx
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8) OR VGA_SEQIdx_Reset;0x300
    out       dx, ax
    sti
    call      TurnScreenOff             ;Offset 0x3109
    push      ax
    mov       dx, di
    mov       ax, VGA_CRTCIdx_VertRetraceEnd;0x11
    out       dx, ax
    mov       cl, 19h
    xor       ax, ax
    call      Func0x2ea3                ;Offset 0x2ea3
    push      si
    add       dl, 06h                   ;Port 0x3ca
    in        al, dx
    xor       ax, ax
    out       dx, al
    mov       al, 10h
    add       si, ax
    mov       cl, 01h
    call      Func0x2eb9                ;Offset 0x2eb9
    inc       ax
    inc       si
    mov       cl, 02h
    call      Func0x2eb9                ;Offset 0x2eb9
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    xor       ax, ax
    call      Func0x2ea3                ;Offset 0x2ea3
    pop       si
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x310b
    pop       dx
    pop       cx
    ret
Func0x2e45 ENDP

Func0x2ea3 PROC NEAR                    ;Offset 0x2ea3
Label0x2ea3:                            ;Offset 0x2ea3
    mov       ah, byte ptr es:[si]
    inc       si
    out       dx, ax
    inc       ax
    loop      Label0x2ea3               ;Offset 0x2ea3
    ret
Func0x2ea3 ENDP

Func0x2eac PROC NEAR                    ;Offset 0x2eac
    mov       al, 00h
Func0x2eac ENDP
;continue!
Func0x2eae PROC NEAR                    ;Offset 0x2eae
Label0x2eae:                            ;Offset 0x2eae
    out       dx, al
    push      ax
    inc       dx
    in        al, dx
    dec       dx
    stosb
    pop       ax
    inc       ax
    loop      Label0x2eae               ;Offset 0x2eae
    ret
Func0x2eae ENDP

Func0x2eb9 PROC NEAR                    ;Offset 0x2eb9
    mov       ah, 0ffh
Func0x2eb9 ENDP
;continue!
Func0x2ebb PROC NEAR                    ;Offset 0x2ebb
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
Label0x2ebe:                            ;Offset 0x2ebe
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;Misassemble!
    jb        Label0x2ec7               ;Offset 0x2ec7
    ja        Label0x2ed0               ;Offset 0x2ed0
    inc       ax
    mov       cl, 01h
Label0x2ec7:                            ;Offset 0x2ec7
    out       dx, al
    push      ax
    lodsb     byte ptr es:[si]
    out       dx, al
    pop       ax
    inc       ax
    loop      Label0x2ebe               ;Offset 0x2ebe
Label0x2ed0:                            ;Offset 0x2ed0
    ret
Func0x2ebb ENDP

Func0x2ed1 PROC NEAR                    ;Offset 0x2ed1
    mov       ah, 0ffh
    push      cx
    push      dx
    mov       al, 00h
    mov       cx, 0010h
    call      Func0x31cc                ;Offset 0x31cc
Label0x2edd:                            ;Offset 0x2edd
    push      ax
    push      dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0x3c0
    out       dx, al
    inc       dx
    in        al, dx
    stosb
    pop       dx
    in        al, dx
    pop       ax
    inc       ax
    dec       cx
    js        Label0x2ef6               ;Offset 0x2ef6
    jne       Label0x2edd               ;Offset 0x2edd
    add       ax, 0101h
    jb        Label0x2edd               ;Offset 0x2edd
    inc       di
    jmp       Label0x2edd               ;Offset 0x2edd
Label0x2ef6:                            ;Offset 0x2ef6
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
Func0x2ed1 ENDP

Func0x2eff PROC NEAR                    ;Offset 0x2eff
    push      ds
    push      es
    push      si
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    pop       si
    pop       ds
    je        Label0x2f12               ;Offset 0x2f12
    mov       cx, 0008h
    rep movsw
    inc       si
    movsb
Label0x2f12:                            ;Offset 0x2f12
    pop       ds
    ret
Func0x2eff ENDP

Func0x2f14 PROC NEAR                    ;Offset 0x2f14
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, 0ffh
    out       dx, al
    push      cs
    pop       es
    call      GetNumberOfActiveScanlines;Offset 0x3054
    mov       ah, al
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    ;cmp       ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 3Dh, 13h, 00h                    ;Misassemble!
    je        Label0x2f43               ;Offset 0x2f43
    lea       si, [Data0x337a]          ;Offset 0x337a
    jb        Label0x2f3f               ;Offset 0x2f3f
    lea       si, [Data0x3402]          ;Offset 0x3402
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0x2f3f               ;Offset 0x2f3f
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0x2f3f               ;Offset 0x2f3f
    lea       si, [Data0x32f2]          ;Offset 0x32f2
Label0x2f3f:                            ;Offset 0x2f3f
    call      Func0x2f5d                ;Offset 0x2f5d
    ret
Label0x2f43:                            ;Offset 0x2f43
    lea       si, [Data0x3446]          ;Offset 0x3446
    call      Func0x2f5d                ;Offset 0x2f5d
    lea       si, [Data0x346e]          ;Offset 0x346e
    mov       al, 10h
    call      Func0x2f5f                ;Offset 0x2f5f
    lea       si, [Data0x3482]          ;Offset 0x3482
    mov       al, 20h
    call      Func0x2f5f                ;Offset 0x2f5f
    ret
Func0x2f14 ENDP

Func0x2f5d PROC NEAR                    ;Offset 0x2f5d
    xor       al, al
Func0x2f5d ENDP
;continue!
Func0x2f5f PROC NEAR                    ;Offset 0x2f5f
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    inc       dx
    mov       ax, word ptr es:[si - 04h]
    mov       cx, word ptr es:[si - 02h]
    test      byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    je        Label0x2f7b               ;Offset 0x2f7b
    add       si, cx
    add       si, 0004h
    mov       ax, word ptr es:[si - 04h]
Label0x2f7b:                            ;Offset 0x2f7b
    or        ah, ah
    je        Label0x2f8d               ;Offset 0x2f8d
    jns       Func0x2f93                ;Offset 0x2f93
Label0x2f81:                            ;Offset 0x2f81
    lodsb     byte ptr es:[si]
    out       dx, al
    jmp       Label0x2f86               ;Offset 0x2f86 Speed sensitive!
Label0x2f86:                            ;Offset 0x2f86
    out       dx, al
    jmp       Label0x2f89               ;Offset 0x2f89 Speed sensitive!
Label0x2f89:                            ;Offset 0x2f89
    out       dx, al
    loop      Label0x2f81               ;Offset 0x2f81
    ret
Label0x2f8d:                            ;Offset 0x2f8d
    push      bx
    call      Func0x319b                ;Offset 0x319b
    pop       bx
    ret
Func0x2f5f ENDP

Func0x2f93 PROC NEAR                    ;Offset 0x2f93
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    jne       Label0x2fa6               ;Offset 0x2fa6
Label0x2f9a:                            ;Offset 0x2f9a
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    loop      Label0x2f9a               ;Offset 0x2f9a
    ret
Label0x2fa6:                            ;Offset 0x2fa6
    push      cx
    push      dx
    mov       dh, byte ptr es:[si]
    inc       si
    mov       ch, byte ptr es:[si]
    inc       si
    mov       cl, byte ptr es:[si]
    inc       si
    call      Func0x2fcc                ;Offset 0x2fcc
    pop       dx
    pop       cx
    out       dx, al
    jmp       Label0x2fbc               ;Offset 0x2fbc Speed sensitive!
Label0x2fbc:                            ;Offset 0x2fbc
    out       dx, al
    jmp       Label0x2fbf               ;Offset 0x2fbf Speed sensitive!
Label0x2fbf:                            ;Offset 0x2fbf
    out       dx, al
    loop      Label0x2fa6               ;Offset 0x2fa6
    ret
Func0x2f93 ENDP

Func0x2fc3 PROC NEAR                    ;Offset 0x2fc3
Label0x2fc3:                            ;Offset 0x2fc3
    in        al, dx
    stosb
    in        al, dx
    stosb
    in        al, dx
    stosb
    loop      Label0x2fc3               ;Offset 0x2fc3
    ret
Func0x2fc3 ENDP

Func0x2fcc PROC NEAR                    ;Offset 0x2fcc
    mov       al, 4dh
    mul       dh
    mov       dx, ax
    mov       al, 97h
    mul       ch
    add       dx, ax
    mov       al, 1ch
    mul       cl
    add       ax, dx
    add       ax, 0080h
    mov       al, ah
    ret
Func0x2fcc ENDP

Func0x2fe4 PROC NEAR                    ;Offset 0x2fe4
    mov       al, BDA_VPCB_VideoParameterTable;0x0
    call      LookupVideoParameterControlBlockPointer;Offset 0x317d
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    shl       ax, 01h
    shl       ax, 01h
    mov       di, ax
    call      GetNumberOfActiveScanlines;Offset 0x3054
    add       di, ax
    mov       al, byte ptr cs:[di + Data0x3004];Offset 0x3004
    mov       ah, 40h
    mul       ah
    add       si, ax
    ret
Func0x2fe4 ENDP

;
;                           +------------------------ 200 Scan lines
;                           |
;                           |     +------------------ 350 Scan lines
;                           |     |
;                           |     |     +------------ 400 Scan lines
;                           |     |     |
;                           |     |     |     +------ 480 Scan lines
;                           |     |     |     |
;Offset 0x3004              |     |     |     |       Display mode:
Data0x3004              DB 000h, 013h, 017h, 000h   ; 0x00
                        DB 001h, 014h, 017h, 000h   ; 0x01
                        DB 002h, 015h, 018h, 000h   ; 0x02
                        DB 003h, 016h, 018h, 000h   ; 0x03
                        DB 004h, 000h, 000h, 000h   ; 0x04
                        DB 005h, 000h, 000h, 000h   ; 0x05
                        DB 006h, 000h, 000h, 000h   ; 0x06
                        DB 000h, 007h, 019h, 000h   ; 0x07
                        DB 000h, 000h, 008h, 000h   ; 0x08
                        DB 009h, 000h, 000h, 000h   ; 0x09
                        DB 00Ah, 000h, 000h, 000h   ; 0x0a
                        DB 00Bh, 000h, 000h, 000h   ; 0x0b
                        DB 00Ch, 000h, 000h, 000h   ; 0x0c
                        DB 00Dh, 000h, 000h, 000h   ; 0x0d
                        DB 00Eh, 000h, 000h, 000h   ; 0x0e
                        DB 000h, 011h, 000h, 000h   ; 0x0f
                        DB 000h, 012h, 000h, 000h   ; 0x10
                        DB 000h, 000h, 000h, 01Ah   ; 0x11
                        DB 000h, 000h, 000h, 01Bh   ; 0x12
                        DB 01Ch, 000h, 000h, 000h   ; 0x13

;
;outputs:
;   al = number of active scan lines
;        0 = 200
;        1 = 350
;        2 = 400
;        3 = 480
;
GetNumberOfActiveScanlines PROC NEAR    ;Offset 0x3054
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push  bx
    lea   bx, [NumberOfActiveScanlinesPerModePlusOne];Offset 0x3080
    cmp   al, BDA_DM_320x200_256_Color_Graphics;0x13
    xlatb cs:[bx]
    ja    Label0x3066                   ;Offset 0x3066
    dec   al                            ;Subtract one to see if it turns negative (and thus, not set)
    jns   Label0x307d                   ;Offset 0x307d
Label0x3066:                            ;Offset 0x3066
    mov   al, 02h                       ;400 Scan lines
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    jne   Label0x307d                   ;Offset 0x307d
    dec   ax                            ;350 Scan lines
    call  IsMDAHiResEnhanced            ;Offset 0x3094
    je    Label0x307d                   ;Offset 0x307d
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je    Label0x307d                   ;Offset 0x307d
    dec   ax                            ;200 Scan lines
Label0x307d:                            ;Offset 0x307d
    pop   bx
    ret   
GetNumberOfActiveScanlines ENDP

    nop                                 ;hello   

;
;
;                                               000h means it's determined another way
;                                               All others are decremented by one and returned.
;Offset 0x3080                                    Display mode:
NumberOfActiveScanlinesPerModePlusOne DB 000h   ; 0x00
                                      DB 000h   ; 0x01
                                      DB 000h   ; 0x02
                                      DB 000h   ; 0x03
                                      DB 001h   ; 0x04
                                      DB 001h   ; 0x05
                                      DB 001h   ; 0x06
                                      DB 000h   ; 0x07
                                      DB 003h   ; 0x08
                                      DB 001h   ; 0x09
                                      DB 001h   ; 0x0a
                                      DB 001h   ; 0x0b
                                      DB 001h   ; 0x0c
                                      DB 001h   ; 0x0d
                                      DB 001h   ; 0x0e
                                      DB 002h   ; 0x0f
                                      DB 002h   ; 0x10
                                      DB 004h   ; 0x11
                                      DB 004h   ; 0x12
                                      DB 001h   ; 0x13

IsMDAHiResEnhanced PROC NEAR            ;Offset 0x3094
    push  ax
    mov   al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and   al, BDA_EFBS_AdapterTypeMask  ;0xf
    cmp   al, BDA_EFBS_MDAHiResEnhanced ;0x3
    je    Found                         ;Offset 0x30a0
    cmp   al, BDA_EFBS_MDAHiResEnhanced_2;0x9 Also check the other one
Found:                                  ;Offset 0x30a0
    pop   ax
    ret
IsMDAHiResEnhanced ENDP

Func0x30a2 PROC NEAR                    ;Offset 0x30a2
    push  ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call  Func0x64cd                    ;Offset 0x64cd
    cmp   al, BDA_DM_80x25_Monochrome_Text;0x7
    je    Label0x30b3                   ;Offset 0x30b3
    cmp   al, BDA_DM_80x25_16_Color_Text;0x3
    ja    Label0x30b3                   ;Offset 0x30b3
    xor   al, al
Label0x30b3:                            ;Offset 0x30b3
    pop   ax
    ret
Func0x30a2 ENDP

Func0x30b5 PROC NEAR                    ;Offset 0x30b5
    push  si
    cbw
    xchg  ax, si
    add   si, di
Label0x30ba:                            ;Offset 0x30ba
    lodsb byte ptr es:[si]
    cmp   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je    Label0x30c7                   ;Offset 0x30c7
    inc   al
    jne   Label0x30ba                   ;Offset 0x30ba
    inc   ax
Label0x30c7:                            ;Offset 0x30c7
    pop   si
    ret
Func0x30b5 ENDP

Func0x30c9 PROC NEAR                    ;Offset 0x30c9
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
    in    al, dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    pop   dx
    ret
Func0x30c9 ENDP

Func0x30d9 PROC NEAR                    ;Offset 0x30d9
    push  ax
    push  dx
    call  Func0x31cc                    ;Offset 0x31cc
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    xchg  al, ah
    out   dx, al
    mov   al, ah
    out   dx, al
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    sti
    pop   dx
    pop   ax
    ret
Func0x30d9 ENDP

Func0x30ed PROC NEAR                    ;Offset 0x30ed
    push  dx
    call  Func0x31cc                    ;Offset 0x31cc
    push  dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, ah
    out   dx, al
    inc   dx
    in    al, dx
    pop   dx
    push  ax
    in    al, dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    sti
    pop   ax
    pop   dx
    ret
Func0x30ed ENDP

TurnScreenOn PROC NEAR                  ;Offset 0x3105
    mov   ah, 00h
    jmp   ToggleScreenOnOff             ;Offset 0x310b
TurnScreenOn ENDP

TurnScreenOff PROC NEAR                 ;Offset 0x3109
    mov   ah, 20h
TurnScreenOff ENDP
;continue!
;
;inputs:
;   ah = 0x00 = screen on, ah = 0x20 = screen off
;
;outputs:
;   ah = original clocking mode register value
;
;destroys:
;   dx, al
;
ToggleScreenOnOff PROC NEAR             ;Offset 0x310b
    mov   dx, VGA_SequenceIndex         ;Port 0x3c4
    mov   al, VGA_SEQIdx_ClockingMode   ;0x1
    out   dx, al                        ;Write Index
    inc   dx                            ;Move to sequence data port
    in    al, dx                        ;read data port
    mov   dh, al                        ;store original value
    and   ax, 20dfh                     ;Filter ah to only VGA_SEQ1_ScreenOff, and filter out screen off in original value
    or    al, ah                        ;set screen off bit in original value
    mov   ah, dh                        ;ah = original register value
    mov   dh, 03h                       ;Restore port high byte
    out   dx, al                        ;write updated register value
    ret
ToggleScreenOnOff ENDP

;
;inputs:
;   bx = display combination code
;
;outputs:
;   none
;
;destroys:
;   ax, cx, dx, es, si, di
;
SetDisplayCombinationCodeIndex PROC NEAR;Offset 0x3120
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    NotFound                      ;Offset 0x3140
    xor   dx, dx
    xor   cx, cx
    mov   cl, byte ptr es:[si]
    add   si, sizeof DisplayCombinationCodeTable;0x4 Skip display combination code header
Find:                                   ;Offset 0x3131
    lodsw word ptr es:[si]
    cmp   ax, bx
    je    Found                         ;Offset 0x3142
    xchg  al, ah
    cmp   ax, bx
    je    Found                         ;Offset 0x3142
    inc   dx
    loop  Find                          ;Offset 0x3131
NotFound:                               ;Offset 0x3140
    mov   dl, 0ffh                      ;Entry not found set index to -1
Found:                                  ;Offset 0x3142
    mov   byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], dl;Offset 0x48a
    ret
SetDisplayCombinationCodeIndex ENDP

GetDisplayCombinationCode PROC NEAR     ;Offset 0x3147
    push  di
    push  es
    mov   bx, 0ffffh
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x317d
    je    Label0x317a                   ;Offset 0x317a
    mov   al, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    cmp   al, byte ptr es:[si]
    jae   Label0x317a                   ;Offset 0x317a
    cbw
    shl   ax, 01h
    xchg  ax, bx
    mov   ax, word ptr es:[bx + si + 04h]
    mov   bx, ax
    cmp   al, 00h
    je    Label0x3178                   ;Offset 0x3178
    test  byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je    Label0x3172                   ;Offset 0x3172
    jp    Label0x3174                   ;Offset 0x3174
Label0x3172:                            ;Offset 0x3172
    xor   al, 01h
Label0x3174:                            ;Offset 0x3174
    test  al, 01h
    jne   Label0x317a                   ;Offset 0x317a
Label0x3178:                            ;Offset 0x3178
    xchg  bl, bh
Label0x317a:                            ;Offset 0x317a
    pop   es
    pop   di
    ret
GetDisplayCombinationCode ENDP

LookupVideoParameterControlBlockPointer PROC NEAR;Offset 0x317d
    cbw
    les   si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8    es:si = Video Save/Override Pointer table
    test  al, 02h                       ;Test if al bit 2 is set.
    je    DereferencePointerWithOffset  ;Offset 0x318f  if not set, then lookup in primary table
    push  ax                            ;else save ax
    mov   al, BDA_VPCB_SecondarySavePointerTbl;0x10 load offset 0x10 instead (secondary save pointer)
    call  DereferencePointerWithOffset  ;Offset 0x318f
    pop   ax                            ;restore ax
    je    Func0x319a                    ;Offset 0x319a  if previous call yielded null, return else call with original value
                                        ;with secondary save pointer
LookupVideoParameterControlBlockPointer ENDP
;continue!
;
;inputs:
;   es:si = far pointer to table
;   ax    = offset into table
;
;outputs:
;   es:di/si = output far pointer
;   flags = zero = 0 = es:di is set, 1 = es:di is null
;
;destroys:
;   ax
;
DereferencePointerWithOffset PROC NEAR  ;Offset 0x318f
    add   si, ax                        ;add ax offset to video save/override pointer
    les   si, es:[si]                   ;load es:si from table
    mov   di, si                        ;store si in di
    mov   ax, es                        ;store es in ax
    or    ax, si                        ;or ax and si (set flags)
DereferencePointerWithOffset ENDP
;continue!
Func0x319a PROC NEAR                    ;Offset 0x319a
    ret
Func0x319a ENDP

Func0x319b PROC NEAR                    ;Offset 0x319b
Label0x319b:                            ;Offset 0x319b
    lodsb byte ptr es:[si]
    mov   ah, al
    mov   bx, 0003h
Label0x31a2:                            ;Offset 0x31a2
    shl   ah, 01h
    shl   ah, 01h
    xor   al, al
    test  ah, 0c0h
    je    Label0x31b7                   ;Offset 0x31b7
    jns   Label0x31b5                   ;Offset 0x31b5
    jnp   Label0x31b3                   ;Offset 0x31b3
    add   al, 15h
Label0x31b3:                            ;Offset 0x31b3
    add   al, 15h
Label0x31b5:                            ;Offset 0x31b5
    add   al, 15h
Label0x31b7:                            ;Offset 0x31b7
    out   dx, al
    dec   bx
    jne   Label0x31a2                   ;Offset 0x31a2
    loop  Label0x319b                   ;Offset 0x319b
    ret
Func0x319b ENDP

Func0x31be PROC NEAR                    ;Offset 0x31be
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x31c6:                            ;Offset 0x31c6
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne   Label0x31c6                   ;Offset 0x31c6
    pop   ax
Func0x31be ENDP
;continue!
Func0x31cc PROC NEAR                    ;Offset 0x31cc
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x31d4:                            ;Offset 0x31d4
    sti
    nop
    cli
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x31d4                   ;Offset 0x31d4
    pop   ax
    ret
Func0x31cc ENDP

;
;inputs:
;   al * 8 * 1193 ticks time
;
Beep PROC NEAR                          ;Offset 0x31de
    mov   ah, al
    in    al, KB_PortB                  ;Port 0x61
    push  ax
    or    al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out   KB_PortB, al                  ;Port 0x61
    mov   al, ah
    call  Sleep2                        ;Offset 0x31f0
    pop   ax
    out   KB_PortB, al                  ;Port 0x61
    ret
Beep ENDP

;
;inputs:
;   al * 8 * 1193 ticks time
;outputs:
;   -
;
Sleep2 PROC NEAR                        ;Offset 0x31f0
    call  SetupSquareWave               ;Offset 0x3230
    push  ax
    push  cx
    push  dx
    push  ds
    mov   dx, SYS_Segment               ;Segment 0xf000
    mov   ds, dx
    mov   dx, KB_CommandPort_XT         ;0x62
    cmp   byte ptr ds:[SYS_MachineID], SYS_MachineID_IBM_AT;Offset 0xfffe 0xfc
    jne   NotAT                         ;Offset 0x3207
    dec   dx
NotAT:                                  ;Offset 0x3207
    pop   ds
    mov   cl, al
    xor   ch, ch
    shl   cx, 01h
    shl   cx, 01h
    shl   cx, 01h
    jcxz  NoSleep                       ;Offset 0x322c
    in    al, KB_PortB                  ;Port 0x61
    mov   ah, al                        ;store off original value
    or    al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out   KB_PortB, al                  ;Port 0x61
WaitTimeExpired:                        ;Offset 0x321c
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne   WaitTimeExpired               ;Offset 0x321c
WaitAgain:                              ;Offset 0x3221
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    je    WaitAgain                     ;Offset 0x3221
    loop  WaitTimeExpired               ;Offset 0x321c
    mov   al, ah                        ;restore original value
    out   KB_PortB, al                  ;Port 0x61
NoSleep:                                ;Offset 0x322c
    pop   dx
    pop   cx
    pop   ax
    ret
Sleep2 ENDP

SetupSquareWave PROC NEAR               ;Offset 0x3230
    push ax
    mov  al, PIT_MC_OpMode_SquareWaveGenerator OR PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2;0xb6
    out  PIT_ModeCommand, al            ;Port 0x43
    mov  ax, 04a9h                      ;1193 ticks
    out  PIT_Channel2Data, al           ;Port 0x42
    mov  al, ah
    out  PIT_Channel2Data, al           ;Port 0x42
    pop  ax
    ret
SetupSquareWave ENDP

PrintScreenHandler:                     ;Offset 0x3240
    sti
    push ax
    push bx
    push cx
    push dx
    push ds
    xor  ax, ax
    mov  ds, ax
    inc  ax
    xchg byte ptr ds:[BDA_PrintScreenStatus], al ;Offset 0x500
    dec  ax
    je   Label0x3294                    ;Offset 0x3294
    mov  ah, 0fh
    int  10h
    mov  bl, ah
    mov  ah, 03h
    int  10h
    push dx
    call PrintCRLF                      ;Offset 0x329a
    mov  cl, 0ffh
    xor  dx, dx
Label0x3264:                            ;Offset 0x3264
    mov  ah, 02h
    int  10h
    mov  ah, 08h
    int  10h
    or   al, al
    jne  Label0x3272                    ;Offset 0x3272
    mov  al, 20h
Label0x3272:                            ;Offset 0x3272
    call PrintCharacter                 ;Offset 0x32a1
    jne  Label0x328b                    ;Offset 0x328b
    inc  dx
    ;cmp  dl, bl
    DB 3Ah, 0D3h                        ;Misassemble!
    jb   Label0x3264                    ;Offset 0x3264
    call PrintCRLF                      ;Offset 0x329a
    xor  dl, dl
    inc  dh
    cmp  dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe  Label0x3264                    ;Offset 0x3264
    xor  cl, cl
Label0x328b:                            ;Offset 0x328b
    pop  dx
    mov  ah, 02h
    int  10h
    mov  byte ptr ds:[BDA_PrintScreenStatus], cl;Offset 0x500
Label0x3294:                            ;Offset 0x3294
    pop  ds
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    iret

PrintCRLF PROC NEAR                     ;Offset 0x329a
    mov  al, 0dh
    call PrintCharacter                 ;Offset 0x32a1
    mov  al, 0ah
PrintCRLF ENDP
;continue!
PrintCharacter PROC NEAR                ;Offset 0x32a1
    push dx
    xor  dx, dx
    xor  ah, ah
    int  17h
    test ah, 29h
    pop  dx
    ret
PrintCharacter ENDP

    nop                                 ;hello

;Offset 0x32ae
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x32ee
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x32f2
Data0x32f2              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 028h, 02Ah, 001h, 003h, 009h, 00Bh, 021h, 023h, 029h, 02Bh
                        DB 004h, 006h, 00Ch, 00Eh, 024h, 026h, 02Ch, 02Eh, 005h, 007h, 00Dh, 00Fh, 025h, 027h, 02Dh, 02Fh
                        DB 010h, 012h, 018h, 01Ah, 030h, 032h, 038h, 03Ah, 011h, 013h, 019h, 01Bh, 031h, 033h, 039h, 03Bh
                        DB 014h, 016h, 01Ch, 01Eh, 034h, 036h, 03Ch, 03Eh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x3232
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x3236
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 025h, 028h, 002h, 007h, 01Bh, 020h, 00Fh, 014h, 028h, 02Ch
                        DB 00Ch, 011h, 025h, 02Ah, 014h, 01Eh, 032h, 036h, 00Fh, 013h, 027h, 02Ch, 01Bh, 020h, 034h, 039h
                        DB 006h, 00Bh, 01Fh, 024h, 013h, 018h, 02Ch, 030h, 009h, 00Dh, 021h, 026h, 015h, 01Ah, 02Eh, 033h
                        DB 013h, 017h, 02Bh, 030h, 01Fh, 024h, 038h, 03Dh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x3276
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x337a
Data0x337a              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x32ba
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x32be
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x32fe
                        DW 0000h        ;BDA_VideoDisplayDataArea 0 = terminate
                        DW 0040h        ;count (below)
;Offset 0x3402
Data0x3402              DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
;Offset 0x3442
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x3446
Data0x3446              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x3456
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x345a
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x346a
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x346e
Data0x346e              DB 000h, 005h, 008h, 00Bh, 00Eh, 011h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Dh, 032h, 038h, 03Fh
;Offset 0x347e
                        DW 4000h        ;BDA_VideoDisplayDataArea
                        DW 00D8h        ;count (below) RGB, 0xd8 * 3
;Offset 0x3482
Data0x3482              DB 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 03Fh, 03Fh
                        DB 000h, 02Fh, 03Fh, 000h, 01Fh, 03Fh, 000h, 010h, 03Fh, 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh
                        DB 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 02Fh, 03Fh, 000h, 01Fh, 03Fh, 000h, 010h, 03Fh, 000h
                        DB 000h, 03Fh, 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h
                        DB 02Fh, 03Fh, 000h, 01Fh, 03Fh, 000h, 010h, 03Fh, 01Fh, 01Fh, 03Fh, 027h, 01Fh, 03Fh, 02Fh, 01Fh
                        DB 03Fh, 037h, 01Fh, 03Fh, 03Fh, 01Fh, 03Fh, 03Fh, 01Fh, 037h, 03Fh, 01Fh, 02Fh, 03Fh, 01Fh, 027h
                        DB 03Fh, 01Fh, 01Fh, 03Fh, 027h, 01Fh, 03Fh, 02Fh, 01Fh, 03Fh, 037h, 01Fh, 03Fh, 03Fh, 01Fh, 037h
                        DB 03Fh, 01Fh, 02Fh, 03Fh, 01Fh, 027h, 03Fh, 01Fh, 01Fh, 03Fh, 01Fh, 01Fh, 03Fh, 027h, 01Fh, 03Fh
                        DB 02Fh, 01Fh, 03Fh, 037h, 01Fh, 03Fh, 03Fh, 01Fh, 037h, 03Fh, 01Fh, 02Fh, 03Fh, 01Fh, 027h, 03Fh
                        DB 02Dh, 02Dh, 03Fh, 031h, 02Dh, 03Fh, 036h, 02Dh, 03Fh, 03Ah, 02Dh, 03Fh, 03Fh, 02Dh, 03Fh, 03Fh
                        DB 02Dh, 03Ah, 03Fh, 02Dh, 036h, 03Fh, 02Dh, 031h, 03Fh, 02Dh, 02Dh, 03Fh, 031h, 02Dh, 03Fh, 036h
                        DB 02Dh, 03Fh, 03Ah, 02Dh, 03Fh, 03Fh, 02Dh, 03Ah, 03Fh, 02Dh, 036h, 03Fh, 02Dh, 031h, 03Fh, 02Dh
                        DB 02Dh, 03Fh, 02Dh, 02Dh, 03Fh, 031h, 02Dh, 03Fh, 036h, 02Dh, 03Fh, 03Ah, 02Dh, 03Fh, 03Fh, 02Dh
                        DB 03Ah, 03Fh, 02Dh, 036h, 03Fh, 02Dh, 031h, 03Fh, 000h, 000h, 01Ch, 007h, 000h, 01Ch, 00Eh, 000h
                        DB 01Ch, 015h, 000h, 01Ch, 01Ch, 000h, 01Ch, 01Ch, 000h, 015h, 01Ch, 000h, 00Eh, 01Ch, 000h, 007h
                        DB 01Ch, 000h, 000h, 01Ch, 007h, 000h, 01Ch, 00Eh, 000h, 01Ch, 015h, 000h, 01Ch, 01Ch, 000h, 015h
                        DB 01Ch, 000h, 00Eh, 01Ch, 000h, 007h, 01Ch, 000h, 000h, 01Ch, 000h, 000h, 01Ch, 007h, 000h, 01Ch
                        DB 00Eh, 000h, 01Ch, 015h, 000h, 01Ch, 01Ch, 000h, 015h, 01Ch, 000h, 00Eh, 01Ch, 000h, 007h, 01Ch
                        DB 00Eh, 00Eh, 01Ch, 011h, 00Eh, 01Ch, 015h, 00Eh, 01Ch, 018h, 00Eh, 01Ch, 01Ch, 00Eh, 01Ch, 01Ch
                        DB 00Eh, 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h, 01Ch, 00Eh, 00Eh, 01Ch, 011h, 00Eh, 01Ch, 015h
                        DB 00Eh, 01Ch, 018h, 00Eh, 01Ch, 01Ch, 00Eh, 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h, 01Ch, 00Eh
                        DB 00Eh, 01Ch, 00Eh, 00Eh, 01Ch, 011h, 00Eh, 01Ch, 015h, 00Eh, 01Ch, 018h, 00Eh, 01Ch, 01Ch, 00Eh
                        DB 018h, 01Ch, 00Eh, 015h, 01Ch, 00Eh, 011h, 01Ch, 014h, 014h, 01Ch, 016h, 014h, 01Ch, 018h, 014h
                        DB 01Ch, 01Ah, 014h, 01Ch, 01Ch, 014h, 01Ch, 01Ch, 014h, 01Ah, 01Ch, 014h, 018h, 01Ch, 014h, 016h
                        DB 01Ch, 014h, 014h, 01Ch, 016h, 014h, 01Ch, 018h, 014h, 01Ch, 01Ah, 014h, 01Ch, 01Ch, 014h, 01Ah
                        DB 01Ch, 014h, 018h, 01Ch, 014h, 016h, 01Ch, 014h, 014h, 01Ch, 014h, 014h, 01Ch, 016h, 014h, 01Ch
                        DB 018h, 014h, 01Ch, 01Ah, 014h, 01Ch, 01Ch, 014h, 01Ah, 01Ch, 014h, 018h, 01Ch, 014h, 016h, 01Ch
                        DB 000h, 000h, 010h, 004h, 000h, 010h, 008h, 000h, 010h, 00Ch, 000h, 010h, 010h, 000h, 010h, 010h
                        DB 000h, 00Ch, 010h, 000h, 008h, 010h, 000h, 004h, 010h, 000h, 000h, 010h, 004h, 000h, 010h, 008h
                        DB 000h, 010h, 00Ch, 000h, 010h, 010h, 000h, 00Ch, 010h, 000h, 008h, 010h, 000h, 004h, 010h, 000h
                        DB 000h, 010h, 000h, 000h, 010h, 004h, 000h, 010h, 008h, 000h, 010h, 00Ch, 000h, 010h, 010h, 000h
                        DB 00Ch, 010h, 000h, 008h, 010h, 000h, 004h, 010h, 008h, 008h, 010h, 00Ah, 008h, 010h, 00Ch, 008h
                        DB 010h, 00Eh, 008h, 010h, 010h, 008h, 010h, 010h, 008h, 00Eh, 010h, 008h, 00Ch, 010h, 008h, 00Ah
                        DB 010h, 008h, 008h, 010h, 00Ah, 008h, 010h, 00Ch, 008h, 010h, 00Eh, 008h, 010h, 010h, 008h, 00Eh
                        DB 010h, 008h, 00Ch, 010h, 008h, 00Ah, 010h, 008h, 008h, 010h, 008h, 008h, 010h, 00Ah, 008h, 010h
                        DB 00Ch, 008h, 010h, 00Eh, 008h, 010h, 010h, 008h, 00Eh, 010h, 008h, 00Ch, 010h, 008h, 00Ah, 010h
                        DB 00Bh, 00Bh, 010h, 00Ch, 00Bh, 010h, 00Dh, 00Bh, 010h, 00Fh, 00Bh, 010h, 010h, 00Bh, 010h, 010h
                        DB 00Bh, 00Fh, 010h, 00Bh, 00Dh, 010h, 00Bh, 00Ch, 010h, 00Bh, 00Bh, 010h, 00Ch, 00Bh, 010h, 00Dh
                        DB 00Bh, 010h, 00Fh, 00Bh, 010h, 010h, 00Bh, 00Fh, 010h, 00Bh, 00Dh, 010h, 00Bh, 00Ch, 010h, 00Bh
                        DB 00Bh, 010h, 00Bh, 00Bh, 010h, 00Ch, 00Bh, 010h, 00Dh, 00Bh, 010h, 00Fh, 00Bh, 010h, 010h, 00Bh
                        DB 00Fh, 010h, 00Bh, 00Dh, 010h, 00Bh, 00Ch, 010h

;Offset 0x370a
RowsOnScreen            DB 000h, 00Eh, 019h, 02Bh

;Offset 0x370e
Data0x370e              DW offset Font8x16;Offset 0x3f20
                        DW offset Font8x8;Offset 0x3720
                        DW offset GraphicsFont8x8;Offset 0x3b20
                        DW offset Data0x4f20;Offset 0x4f20
                        DW offset Font8x16;Offset 0x3f20
                        DW offset Data0x4f20;Offset 0x4f20

;Offset 0x371a
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx

;Offset 0x3720
Font8x8                 DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 081h, 0A5h, 081h, 0BDh, 099h, 081h, 07Eh
                        DB 07Eh, 0FFh, 0DBh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 06Ch, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h
                        DB 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 038h, 07Ch, 038h, 0FEh, 0FEh, 07Ch, 038h, 07Ch
                        DB 010h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 07Ch, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h
                        DB 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h
                        DB 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 00Fh, 007h, 00Fh, 07Dh, 0CCh, 0CCh, 0CCh, 078h
                        DB 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 018h, 03Fh, 033h, 03Fh, 030h, 030h, 070h, 0F0h, 0E0h
                        DB 07Fh, 063h, 07Fh, 063h, 063h, 067h, 0E6h, 0C0h, 099h, 05Ah, 03Ch, 0E7h, 0E7h, 03Ch, 05Ah, 099h
                        DB 080h, 0E0h, 0F8h, 0FEh, 0F8h, 0E0h, 080h, 000h, 002h, 00Eh, 03Eh, 0FEh, 03Eh, 00Eh, 002h, 000h
                        DB 018h, 03Ch, 07Eh, 018h, 018h, 07Eh, 03Ch, 018h, 066h, 066h, 066h, 066h, 066h, 000h, 066h, 000h
                        DB 07Fh, 0DBh, 0DBh, 07Bh, 01Bh, 01Bh, 01Bh, 000h, 03Eh, 063h, 038h, 06Ch, 06Ch, 038h, 0CCh, 078h
                        DB 000h, 000h, 000h, 000h, 07Eh, 07Eh, 07Eh, 000h, 018h, 03Ch, 07Eh, 018h, 07Eh, 03Ch, 018h, 0FFh
                        DB 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h
                        DB 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 030h, 060h, 0FEh, 060h, 030h, 000h, 000h
                        DB 000h, 000h, 0C0h, 0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h
                        DB 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 000h, 000h, 000h, 0FFh, 0FFh, 07Eh, 03Ch, 018h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 078h, 078h, 030h, 030h, 000h, 030h, 000h
                        DB 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 06Ch, 06Ch, 0FEh, 06Ch, 0FEh, 06Ch, 06Ch, 000h
                        DB 030h, 07Ch, 0C0h, 078h, 00Ch, 0F8h, 030h, 000h, 000h, 0C6h, 0CCh, 018h, 030h, 066h, 0C6h, 000h
                        DB 038h, 06Ch, 038h, 076h, 0DCh, 0CCh, 076h, 000h, 060h, 060h, 0C0h, 000h, 000h, 000h, 000h, 000h
                        DB 018h, 030h, 060h, 060h, 060h, 030h, 018h, 000h, 060h, 030h, 018h, 018h, 018h, 030h, 060h, 000h
                        DB 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 030h, 030h, 0FCh, 030h, 030h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 030h, 060h, 000h, 000h, 000h, 0FCh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 030h, 000h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h
                        DB 07Ch, 0C6h, 0CEh, 0DEh, 0F6h, 0E6h, 07Ch, 000h, 030h, 070h, 030h, 030h, 030h, 030h, 0FCh, 000h
                        DB 078h, 0CCh, 00Ch, 038h, 060h, 0CCh, 0FCh, 000h, 078h, 0CCh, 00Ch, 038h, 00Ch, 0CCh, 078h, 000h
                        DB 01Ch, 03Ch, 06Ch, 0CCh, 0FEh, 00Ch, 01Eh, 000h, 0FCh, 0C0h, 0F8h, 00Ch, 00Ch, 0CCh, 078h, 000h
                        DB 038h, 060h, 0C0h, 0F8h, 0CCh, 0CCh, 078h, 000h, 0FCh, 0CCh, 00Ch, 018h, 030h, 030h, 030h, 000h
                        DB 078h, 0CCh, 0CCh, 078h, 0CCh, 0CCh, 078h, 000h, 078h, 0CCh, 0CCh, 07Ch, 00Ch, 018h, 070h, 000h
                        DB 000h, 030h, 030h, 000h, 000h, 030h, 030h, 000h, 000h, 030h, 030h, 000h, 000h, 030h, 030h, 060h
                        DB 018h, 030h, 060h, 0C0h, 060h, 030h, 018h, 000h, 000h, 000h, 0FCh, 000h, 000h, 0FCh, 000h, 000h
                        DB 060h, 030h, 018h, 00Ch, 018h, 030h, 060h, 000h, 078h, 0CCh, 00Ch, 018h, 030h, 000h, 030h, 000h
                        DB 07Ch, 0C6h, 0DEh, 0DEh, 0DEh, 0C0h, 078h, 000h, 030h, 078h, 0CCh, 0CCh, 0FCh, 0CCh, 0CCh, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 066h, 066h, 0FCh, 000h, 03Ch, 066h, 0C0h, 0C0h, 0C0h, 066h, 03Ch, 000h
                        DB 0F8h, 06Ch, 066h, 066h, 066h, 06Ch, 0F8h, 000h, 0FEh, 062h, 068h, 078h, 068h, 062h, 0FEh, 000h
                        DB 0FEh, 062h, 068h, 078h, 068h, 060h, 0F0h, 000h, 03Ch, 066h, 0C0h, 0C0h, 0CEh, 066h, 03Eh, 000h
                        DB 0CCh, 0CCh, 0CCh, 0FCh, 0CCh, 0CCh, 0CCh, 000h, 078h, 030h, 030h, 030h, 030h, 030h, 078h, 000h
                        DB 01Eh, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 000h, 0E6h, 066h, 06Ch, 078h, 06Ch, 066h, 0E6h, 000h
                        DB 0F0h, 060h, 060h, 060h, 062h, 066h, 0FEh, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h, 0C6h, 000h
                        DB 0C6h, 0E6h, 0F6h, 0DEh, 0CEh, 0C6h, 0C6h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h, 078h, 0CCh, 0CCh, 0CCh, 0DCh, 078h, 01Ch, 000h
                        DB 0FCh, 066h, 066h, 07Ch, 06Ch, 066h, 0E6h, 000h, 078h, 0CCh, 0E0h, 070h, 01Ch, 0CCh, 078h, 000h
                        DB 0FCh, 0B4h, 030h, 030h, 030h, 030h, 078h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0FCh, 000h
                        DB 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 030h, 000h, 0C6h, 0C6h, 0C6h, 0D6h, 0FEh, 0EEh, 0C6h, 000h
                        DB 0C6h, 0C6h, 06Ch, 038h, 038h, 06Ch, 0C6h, 000h, 0CCh, 0CCh, 0CCh, 078h, 030h, 030h, 078h, 000h
                        DB 0FEh, 0C6h, 08Ch, 018h, 032h, 066h, 0FEh, 000h, 078h, 060h, 060h, 060h, 060h, 060h, 078h, 000h
                        DB 0C0h, 060h, 030h, 018h, 00Ch, 006h, 002h, 000h, 078h, 018h, 018h, 018h, 018h, 018h, 078h, 000h
                        DB 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
                        DB 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 076h, 000h
                        DB 0E0h, 060h, 060h, 07Ch, 066h, 066h, 0DCh, 000h, 000h, 000h, 078h, 0CCh, 0C0h, 0CCh, 078h, 000h
                        DB 01Ch, 00Ch, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h
                        DB 038h, 06Ch, 060h, 0F0h, 060h, 060h, 0F0h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h
                        DB 0E0h, 060h, 06Ch, 076h, 066h, 066h, 0E6h, 000h, 030h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 00Ch, 000h, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 0E0h, 060h, 066h, 06Ch, 078h, 06Ch, 0E6h, 000h
                        DB 070h, 030h, 030h, 030h, 030h, 030h, 078h, 000h, 000h, 000h, 0CCh, 0FEh, 0FEh, 0D6h, 0C6h, 000h
                        DB 000h, 000h, 0F8h, 0CCh, 0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 078h, 0CCh, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 000h, 0DCh, 066h, 066h, 07Ch, 060h, 0F0h, 000h, 000h, 076h, 0CCh, 0CCh, 07Ch, 00Ch, 01Eh
                        DB 000h, 000h, 0DCh, 076h, 066h, 060h, 0F0h, 000h, 000h, 000h, 07Ch, 0C0h, 078h, 00Ch, 0F8h, 000h
                        DB 010h, 030h, 07Ch, 030h, 030h, 034h, 018h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h
                        DB 000h, 000h, 0CCh, 0CCh, 0CCh, 078h, 030h, 000h, 000h, 000h, 0C6h, 0D6h, 0FEh, 0FEh, 06Ch, 000h
                        DB 000h, 000h, 0C6h, 06Ch, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h
                        DB 000h, 000h, 0FCh, 098h, 030h, 064h, 0FCh, 000h, 01Ch, 030h, 030h, 0E0h, 030h, 030h, 01Ch, 000h
                        DB 018h, 018h, 018h, 000h, 018h, 018h, 018h, 000h, 0E0h, 030h, 030h, 01Ch, 030h, 030h, 0E0h, 000h
                        DB 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 000h

;Offset 0x3b20
GraphicsFont8x8         DB 078h, 0CCh, 0C0h, 0CCh, 078h, 018h, 00Ch, 078h, 000h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 07Eh, 000h
                        DB 01Ch, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h, 07Eh, 0C3h, 03Ch, 006h, 03Eh, 066h, 03Fh, 000h
                        DB 0CCh, 000h, 078h, 00Ch, 07Ch, 0CCh, 07Eh, 000h, 0E0h, 000h, 078h, 00Ch, 07Ch, 0CCh, 07Eh, 000h
                        DB 030h, 030h, 078h, 00Ch, 07Ch, 0CCh, 07Eh, 000h, 000h, 000h, 078h, 0C0h, 0C0h, 078h, 00Ch, 038h
                        DB 07Eh, 0C3h, 03Ch, 066h, 07Eh, 060h, 03Ch, 000h, 0CCh, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h
                        DB 0E0h, 000h, 078h, 0CCh, 0FCh, 0C0h, 078h, 000h, 0CCh, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 07Ch, 0C6h, 038h, 018h, 018h, 018h, 03Ch, 000h, 0E0h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 0C6h, 038h, 06Ch, 0C6h, 0FEh, 0C6h, 0C6h, 000h, 030h, 030h, 000h, 078h, 0CCh, 0FCh, 0CCh, 000h
                        DB 01Ch, 000h, 0FCh, 060h, 078h, 060h, 0FCh, 000h, 000h, 000h, 07Fh, 00Ch, 07Fh, 0CCh, 07Fh, 000h
                        DB 03Eh, 06Ch, 0CCh, 0FEh, 0CCh, 0CCh, 0CEh, 000h, 078h, 0CCh, 000h, 078h, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 0CCh, 000h, 078h, 0CCh, 0CCh, 078h, 000h, 000h, 0E0h, 000h, 078h, 0CCh, 0CCh, 078h, 000h
                        DB 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 07Eh, 000h, 000h, 0E0h, 000h, 0CCh, 0CCh, 0CCh, 07Eh, 000h
                        DB 000h, 0CCh, 000h, 0CCh, 0CCh, 07Ch, 00Ch, 0F8h, 0C3h, 018h, 03Ch, 066h, 066h, 03Ch, 018h, 000h
                        DB 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 000h, 018h, 018h, 07Eh, 0C0h, 0C0h, 07Eh, 018h, 018h
                        DB 038h, 06Ch, 064h, 0F0h, 060h, 0E6h, 0FCh, 000h, 0CCh, 0CCh, 078h, 0FCh, 030h, 0FCh, 030h, 030h
                        DB 0F8h, 0CCh, 0CCh, 0FAh, 0C6h, 0CFh, 0C6h, 0C7h, 00Eh, 01Bh, 018h, 03Ch, 018h, 018h, 0D8h, 070h
                        DB 01Ch, 000h, 078h, 00Ch, 07Ch, 0CCh, 07Eh, 000h, 038h, 000h, 070h, 030h, 030h, 030h, 078h, 000h
                        DB 000h, 01Ch, 000h, 078h, 0CCh, 0CCh, 078h, 000h, 000h, 01Ch, 000h, 0CCh, 0CCh, 0CCh, 07Eh, 000h
                        DB 000h, 0F8h, 000h, 0F8h, 0CCh, 0CCh, 0CCh, 000h, 0FCh, 000h, 0CCh, 0ECh, 0FCh, 0DCh, 0CCh, 000h
                        DB 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h
                        DB 030h, 000h, 030h, 060h, 0C0h, 0CCh, 078h, 000h, 000h, 000h, 000h, 0FCh, 0C0h, 0C0h, 000h, 000h
                        DB 000h, 000h, 000h, 0FCh, 00Ch, 00Ch, 000h, 000h, 0C3h, 0C6h, 0CCh, 0DEh, 033h, 066h, 0CCh, 00Fh
                        DB 0C3h, 0C6h, 0CCh, 0DBh, 037h, 06Fh, 0CFh, 003h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 000h
                        DB 000h, 033h, 066h, 0CCh, 066h, 033h, 000h, 000h, 000h, 0CCh, 066h, 033h, 066h, 0CCh, 000h, 000h
                        DB 022h, 088h, 022h, 088h, 022h, 088h, 022h, 088h, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh
                        DB 0DBh, 077h, 0DBh, 0EEh, 0DBh, 077h, 0DBh, 0EEh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
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
                        DB 000h, 0FCh, 0CCh, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h
                        DB 0FCh, 0CCh, 060h, 030h, 060h, 0CCh, 0FCh, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 070h, 000h
                        DB 000h, 066h, 066h, 066h, 066h, 07Ch, 060h, 0C0h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 000h
                        DB 0FCh, 030h, 078h, 0CCh, 0CCh, 078h, 030h, 0FCh, 038h, 06Ch, 0C6h, 0FEh, 0C6h, 06Ch, 038h, 000h
                        DB 038h, 06Ch, 0C6h, 0C6h, 06Ch, 06Ch, 0EEh, 000h, 01Ch, 030h, 018h, 07Ch, 0CCh, 0CCh, 078h, 000h
                        DB 000h, 000h, 07Eh, 0DBh, 0DBh, 07Eh, 000h, 000h, 006h, 00Ch, 07Eh, 0DBh, 0DBh, 07Eh, 060h, 0C0h
                        DB 038h, 060h, 0C0h, 0F8h, 0C0h, 060h, 038h, 000h, 078h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 000h
                        DB 000h, 0FCh, 000h, 0FCh, 000h, 0FCh, 000h, 000h, 030h, 030h, 0FCh, 030h, 030h, 000h, 0FCh, 000h
                        DB 060h, 030h, 018h, 030h, 060h, 000h, 0FCh, 000h, 018h, 030h, 060h, 030h, 018h, 000h, 0FCh, 000h
                        DB 00Eh, 01Bh, 01Bh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0D8h, 0D8h, 070h
                        DB 030h, 030h, 000h, 0FCh, 000h, 030h, 030h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h
                        DB 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 00Fh, 00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 03Ch, 01Ch
                        DB 078h, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 070h, 018h, 030h, 060h, 078h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 03Ch, 03Ch, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x3f20
Font8x16                DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 081h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 0FFh, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 06Ch, 0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Fh, 0DBh, 0DBh, 0DBh, 07Bh, 01Bh, 01Bh, 01Bh, 01Bh, 01Bh, 000h, 000h, 000h, 000h
                        DB 000h, 07Ch, 0C6h, 060h, 038h, 06Ch, 0C6h, 0C6h, 06Ch, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh, 0FEh, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 060h, 0FEh, 060h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 028h, 06Ch, 0FEh, 06Ch, 028h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 038h, 07Ch, 07Ch, 0FEh, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FEh, 0FEh, 07Ch, 07Ch, 038h, 038h, 010h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 03Ch, 03Ch, 018h, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 066h, 066h, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h, 000h
                        DB 018h, 018h, 07Ch, 0C6h, 0C2h, 0C0h, 07Ch, 006h, 006h, 086h, 0C6h, 07Ch, 018h, 018h, 000h, 000h
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
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0D6h, 0D6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 000h, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 0E6h, 066h, 066h, 06Ch, 078h, 078h, 06Ch, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0F0h, 060h, 060h, 060h, 060h, 060h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0DEh, 07Ch, 00Ch, 00Eh, 000h, 000h
                        DB 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 06Ch, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 060h, 038h, 00Ch, 006h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 07Eh, 05Ah, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0D6h, 0FEh, 0EEh, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 0C6h, 06Ch, 07Ch, 038h, 038h, 07Ch, 06Ch, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 086h, 00Ch, 018h, 030h, 060h, 0C2h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 080h, 0C0h, 0E0h, 070h, 038h, 01Ch, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 03Ch, 000h, 000h, 000h, 000h
                        DB 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h
                        DB 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 066h, 07Ch, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h, 0D6h, 0D6h, 0D6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 076h, 066h, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 060h, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 010h, 030h, 030h, 0FCh, 030h, 030h, 030h, 030h, 036h, 01Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 066h, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0D6h, 0D6h, 0D6h, 0FEh, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 06Ch, 038h, 038h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 0F8h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0FEh, 0CCh, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 00Eh, 018h, 018h, 018h, 070h, 018h, 018h, 018h, 018h, 00Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 018h, 018h, 000h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 070h, 018h, 018h, 018h, 00Eh, 018h, 018h, 018h, 018h, 070h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 00Ch, 006h, 07Ch, 000h, 000h
                        DB 000h, 000h, 0CCh, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 00Ch, 018h, 030h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0CCh, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 038h, 06Ch, 038h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 03Ch, 066h, 060h, 060h, 066h, 03Ch, 00Ch, 006h, 03Ch, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 000h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 066h, 000h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 018h, 03Ch, 066h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 0C6h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 038h, 06Ch, 038h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h
                        DB 018h, 030h, 060h, 000h, 0FEh, 066h, 060h, 07Ch, 060h, 060h, 066h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0CCh, 076h, 036h, 07Eh, 0D8h, 0D8h, 06Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 03Eh, 06Ch, 0CCh, 0CCh, 0FEh, 0CCh, 0CCh, 0CCh, 0CCh, 0CEh, 000h, 000h, 000h, 000h
                        DB 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 030h, 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 060h, 030h, 018h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0C6h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 078h, 000h
                        DB 000h, 0C6h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h
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
                        DB 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 060h, 0DCh, 086h, 00Ch, 018h, 03Eh, 000h, 000h
                        DB 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 066h, 0CEh, 09Eh, 03Eh, 006h, 006h, 000h, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 018h, 018h, 018h, 03Ch, 03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 036h, 06Ch, 0D8h, 06Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0D8h, 06Ch, 036h, 06Ch, 0D8h, 000h, 000h, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 078h, 0CCh, 0CCh, 0CCh, 0D8h, 0CCh, 0C6h, 0C6h, 0C6h, 0CCh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 0FEh, 0C6h, 060h, 030h, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 030h, 018h, 00Ch, 03Eh, 066h, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0DBh, 0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 003h, 006h, 07Eh, 0DBh, 0DBh, 0F3h, 07Eh, 060h, 0C0h, 000h, 000h, 000h, 000h
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
                        DB 000h, 070h, 0D8h, 030h, 060h, 0C8h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x4f20
Data0x4f20              DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 030h, 000h, 000h, 03Ch, 066h, 0C3h, 0C3h, 0DBh, 0DBh, 0C3h, 0C3h, 066h, 03Ch, 000h, 000h, 000h, 000h
                        DB 04Dh, 000h, 000h, 0C3h, 0E7h, 0FFh, 0FFh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h, 000h
                        DB 054h, 000h, 000h, 0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 056h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 057h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h, 000h
                        DB 058h, 000h, 000h, 0C3h, 0C3h, 066h, 03Ch, 018h, 018h, 03Ch, 066h, 0C3h, 0C3h, 000h, 000h, 000h, 000h
                        DB 059h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 05Ah, 000h, 000h, 0FFh, 0C3h, 086h, 00Ch, 018h, 030h, 060h, 0C1h, 0C3h, 0FFh, 000h, 000h, 000h, 000h
                        DB 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh, 0DBh, 0DBh, 0DBh, 0DBh, 000h, 000h, 000h, 000h
                        DB 076h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h, 000h
                        DB 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 000h, 000h, 000h, 000h
                        DB 078h, 000h, 000h, 000h, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 03Ch, 066h, 0C3h, 000h, 000h, 000h, 000h
                        DB 091h, 000h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h, 000h, 000h, 000h, 000h
                        DB 09Bh, 000h, 018h, 018h, 07Eh, 0C3h, 0C0h, 0C0h, 0C0h, 0C3h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 066h, 0F3h, 000h, 000h, 000h, 000h
                        DB 0ABh, 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 060h, 0CEh, 09Bh, 006h, 00Ch, 01Fh, 000h, 000h
                        DB 0ACh, 000h, 0C0h, 0C0h, 0C2h, 0C6h, 0CCh, 018h, 030h, 066h, 0CEh, 096h, 03Eh, 006h, 006h, 000h, 000h
                        DB 000h

;Offset 0x5064
                        DB 'DATE: 04/28/98', 000h
;Offset 0x5073
                        DB 'Revision: 0.37', 000h

VESAPowerManagement:                    ;Offset 0x5082
    cmp  bl, 00h
    jne  VPMNot0                        ;Offset 0x5089
    jmp  VPMGetCaps                     ;Offset 0x509b
VPMNot0:                                ;Offset 0x5089
    cmp  bl, 01h
    jne  VPMNot1                        ;Offset 0x5090
    jmp  VPMSetDisplayPowerState        ;Offset 0x50c6
VPMNot1:                                ;Offset 0x5090
    cmp  bl, 02h
    jne  VPMNot2                        ;Offset 0x5097
    jmp  VPMGetDisplayPowerState        ;Offset 0x50a3
VPMNot2:                                ;Offset 0x5097
    mov  ax, 014fh
    iret
VPMGetCaps:                             ;Offset 0x509b
    mov  ax, 004fh
    mov  bh, 07h
    mov  bl, 10h
    iret
VPMGetDisplayPowerState:                ;Offset 0x50a3
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    xor  bh, bh                         ;0 = Display is on
    and  al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    je   Label0x50c1                    ;Offset 0x50c1
    inc  bh                             ;1 = Display in standby
    cmp  al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    je   Label0x50c1                    ;Offset 0x50c1
    inc  bh                             ;2 = Display is suspended
    cmp  al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    je   Label0x50c1                    ;Offset 0x50c1
    shl  bh, 01h                        ;4 = Display is off
Label0x50c1:                            ;Offset 0x50c1
    mov  ax, 004fh
    pop  dx
    iret
VPMSetDisplayPowerState:                ;Offset 0x50c6
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    mov  al, bh
    and  bh, 0fh
    je   Label0x50db                    ;Offset 0x50db
    and  bl, 07h
    je   Label0x510d                    ;Offset 0x510d
Label0x50db:                            ;Offset 0x50db
    in   al, dx
    and  al, NOT (MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff);0xcf
    or   bh, bh                         ;0 = Turn display on
    jne  Label0x50eb                    ;Offset 0x50eb
    out  dx, al
    call ScreenOn2                      ;Offset 0x5125
    mov  ax, 004fh
    jmp  Label0x5110                    ;Offset 0x5110
Label0x50eb:                            ;Offset 0x50eb
    cmp  bh, 01h                        ;1 = Set display to standby
    jne  Label0x50f4                    ;Offset 0x50f4
    or   al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    jmp  Label0x5104                    ;Offset 0x5104
Label0x50f4:                            ;Offset 0x50f4
    cmp  bh, 02h                        ;2 = Suspend display
    jne  Label0x50fd                    ;Offset 0x50fd
    or   al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    jmp  Label0x5104                    ;Offset 0x5104
Label0x50fd:                            ;Offset 0x50fd
    cmp  bh, 04h                        ;4 = Turn display off
    jne  Label0x510d                    ;Offset 0x510d
    or   al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
Label0x5104:                            ;Offset 0x5104
    out  dx, al
    call ScreenOff2                     ;Offset 0x5112
    mov  ax, 004fh
    jmp  Label0x5110                    ;Offset 0x5110
Label0x510d:                            ;Offset 0x510d
    mov  ax, 014fh
Label0x5110:                            ;Offset 0x5110
    pop  dx
    iret

ScreenOff2 PROC NEAR                    ;Offset 0x5112
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    in   al, dx
    xchg al, ah
    mov  al, VGA_SEQIdx_ClockingMode    ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    or   al, VGA_SEQ1_ScreenOff         ;0x20
    out  dx, al
    xchg al, ah
    dec  dx
    out  dx, al
    ret
ScreenOff2 ENDP

ScreenOn2 PROC NEAR                     ;Offset 0x5125
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    in   al, dx
    xchg al, ah
    mov  al, VGA_SEQIdx_ClockingMode    ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    and  al, NOT VGA_SEQ1_ScreenOff     ;0xdf
    out  dx, al
    xchg al, ah
    dec  dx
    out  dx, al
    ret
ScreenOn2 ENDP

;Offset 0x5138
EDIDHeader              DB 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
                        DB 000h, 000h

;Offset 0x5142
DDCFuncs                DW offset DDCPin1Send1      ;Offset 0x51b7  ;0
                        DW offset DDCPin3Send1      ;Offset 0x519b  ;2
                        DW offset DDCPin1Send0      ;Offset 0x5202  ;4
                        DW offset DDCPin3Send0      ;Offset 0x51d3  ;6
                        DW offset DDCPin1Read       ;Offset 0x5231  ;8
                        DW offset DDCPin3Read       ;Offset 0x5257  ;a

VESADDC:                                ;Offset 0x514e
    cmp  bl, 00h
    je   DDCInstallationCheck           ;Offset 0x5165
    cmp  bl, 01h
    je   DDCReadEDID                    ;Offset 0x5434
    cmp  bl, 02h
    je   DDCReadVDIF                    ;Offset 0x542f
    mov  ax, 014fh
    iret
DDCInstallationCheck:                   ;Offset 0x5165
    push ds
    push es
    push ecx
    push edx
    push edi
    cli
    mov  ax, ss
    mov  es, ax
    sub  sp, 0080h
    mov  di, sp
    call Func0x544f                     ;Offset 0x544f
    or   ah, ah
    jne  Label0x5185                    ;Offset 0x5185
    mov  bh, 01h
    mov  bl, 02h
    jmp  Label0x5189                    ;Offset 0x5189
Label0x5185:                            ;Offset 0x5185
    mov  bh, 00h
    mov  bl, 00h
Label0x5189:                            ;Offset 0x5189
    mov  ah, 00h
    mov  al, 4fh
    sti
    add  sp, 0080h
    pop  edi
    pop  edx
    pop  ecx
    pop  es
    pop  ds
    iret

DDCPin3Send1 PROC NEAR                  ;Offset 0x519b
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  edx
    pop  ecx
    ret
DDCPin3Send1 ENDP

DDCPin1Send1 PROC NEAR                  ;Offset 0x51b7
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  edx
    pop  ecx
    ret
DDCPin1Send1 ENDP

DDCPin3Send0 PROC NEAR                  ;Offset 0x51d3
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC3Data ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    or   cl, MGA_GENIOCTRL_DDC3Enable   ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  edx
    pop  ecx
    ret
DDCPin3Send0 ENDP

DDCPin1Send0 PROC NEAR                  ;Offset 0x5202
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC1Data ;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    or   cl, MGA_GENIOCTRL_DDC1Enable   ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  edx
    pop  ecx
    ret
DDCPin1Send0 ENDP

DDCPin1Read PROC NEAR                   ;Offset 0x5231
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    shl  cl, 07h
    pop  edx
    pop  ecx
    ret
DDCPin1Read ENDP

DDCPin3Read PROC NEAR                   ;Offset 0x5257
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x5bab
    shl  cl, 05h
    pop  edx
    pop  ecx
    ret
DDCPin3Read ENDP

Sleep PROC NEAR                         ;Offset 0x527d
    push bx
    push dx
    mov  ax, 0077h
    ;or   ax, 000fh
    DB 00Dh, 00Fh, 000h
    mov  bx, ax
    mov  al, PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2 OR PIT_MC_OpMode_RateGenerator;0xb4
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, KB_PortB                   ;Port 0x61
    and  al, NOT (KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable);0xfc
    or   al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out  KB_PortB, al                   ;Port 0x61
    mov  ax, bx
    out  PIT_Channel2Data, al           ;Port 0x42
    mov  al, ah
    out  PIT_Channel2Data, al           ;Port 0x42
    mov  al, PIT_MC_ChanSel_2           ;0x80
    out  PIT_ModeCommand, al            ;Port 0x43
    mov  ax, bx
    mov  bx, 8000h
Label0x52a4:                            ;Offset 0x52a4
    shl  ax, 01h
    jb   Label0x52ac                    ;Offset 0x52ac
    shr  bx, 01h
    jmp  Label0x52a4                    ;Offset 0x52a4
Label0x52ac:                            ;Offset 0x52ac
    mov  dl, PIT_MC_ChanSel_2           ;0x80
Label0x52ae:                            ;Offset 0x52ae
    mov  al, dl
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, PIT_Channel2Data           ;Port 0x42
    mov  ah, al
    in   al, PIT_Channel2Data           ;Port 0x42
    xchg al, ah
    and  ax, bx
    je   Label0x52ae                    ;0x52ae
Label0x52be:                            ;Offset 0x52be
    mov  al, dl
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, PIT_Channel2Data           ;Port 0x42
    mov  ah, al
    in   al, PIT_Channel2Data           ;Port 0x42
    xchg al, ah
    and  ax, bx
    jne  Label0x52be                    ;Offset 0x52be
    in   al, KB_PortB                   ;Port 0x61
    and  al, NOT (KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable);0xfc
    out  KB_PortB, al                   ;Port 0x61
    mov  al, PIT_MC_ChanSel_2 OR PIT_MC_AccMode_LoByteHiByte OR PIT_MC_OpMode_SquareWaveGenerator;0xb6
    out  PIT_ModeCommand, al            ;Port 0x43
    pop  dx
    pop  bx
    ret
Sleep ENDP

;Pin1: 0
;
Func0x52db PROC NEAR                    ;Offset 0x52db
    push ecx
    call word ptr cs:[si + MGA_DDCPin1Send0];0x6
    call Sleep                          ;Offset 0x527d
    pop  ecx
    ret
Func0x52db ENDP

;
;Pin0: 11r...00
;Pin1: x1r...x0
;
;Note: Reads until both Pin0 and Pin1 are 1
SendStartBit PROC NEAR                  ;Offset 0x52e7
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 14h
WaitFor1:                               ;Offset 0x52f5
    call Sleep                          ;Offset 0x527d
    dec  cl
    je   TimeOut                        ;Offset 0x5313
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  WaitFor1                       ;Offset 0x52f5
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jae  WaitFor1                       ;Offset 0x52f5
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
TimeOut:                                ;Offset 0x5313
    mov  al, cl
    pop  ecx
    ret
SendStartBit ENDP

;
;Pin0: 0001
;Pin1: x011
;
SendStopBit PROC NEAR                   ;Offset 0x5318
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call Sleep                          ;Offset 0x527d
    pop  ecx
    ret
SendStopBit ENDP

;
;Pin0: 1r...xxxxxx
;Pin1: xxxxx1r...0
;
WaitAck PROC NEAR                       ;Offset 0x5338
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    mov  cl, 14h
Wait0:                                  ;Offset 0x533f
    dec  cl
    je   TimeOut                        ;Offset 0x5366
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jb   Wait0                          ;Offset 0x533f
    mov  cl, 14h
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x5352
    dec  cl
    je   TimeOut                        ;Offset 0x5366
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x5352
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call Sleep                          ;Offset 0x527d
TimeOut:                                ;Offset 0x5366
    mov  al, cl
    pop  ecx
    ret
WaitAck ENDP

;
;Pin0: 0000001
;Pin1: x1r...0
;
SendAck PROC NEAR                      ;Offset 0x536b
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 14h
Wait1:                                  ;Offset 0x537a
    dec  cl
    je   TimeOut                        ;Offset 0x5391
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x537a
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call Sleep                          ;Offset 0x527d
TimeOut:                                ;Offset 0x5391
    mov  al, cl
    pop  ecx
    ret
SendAck ENDP

;
;Pin0: 1111111
;Pin1: x1r...0
;
Func0x5396 PROC NEAR                    ;Offset 0x5396
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call Sleep                          ;Offset 0x527d
    mov  cl, 14h
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x53a4
    dec  cl
    je   TimeOut                        ;Offset 0x53bb
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x53a4
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call Sleep                          ;Offset 0x527d
TimeOut:                                ;Offset 0x53bb
    mov  al, cl
    pop  ecx
    ret
Func0x5396 ENDP

;input:
;   al = data
;
;Data in: abcdefgh
;
;Pin0: aaaaaabbbbbbb
;Pin1: x1r...01r...0 etc
;
;output:
;   al = 0 = success, not 0 = failure
I2CSendByte PROC NEAR                   ;Offset 0x53c0
    push ecx
    push edx
    mov  cl, 08h
    mov  ch, al
NextBit:                                ;Offset 0x53c8
    shl  ch, 01h
    jb   Send1                          ;Offset 0x53d2
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    jmp  Sent0                          ;Offset 0x53d5
Send1:                                  ;Offset 0x53d2
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
Sent0:                                  ;Offset 0x53d5
    mov  dl, 14h
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x53de
    dec  dl
    je   TimeOut                        ;Offset 0x53f3
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x53de
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    dec  cl
    jne  NextBit                        ;Offset 0x53c8
TimeOut:                                ;Offset 0x53f3
    mov  al, dl
    pop  edx
    pop  ecx
    ret
I2CSendByte ENDP

;
;Pin0: xxxxxaxxxxxb
;Pin1: 1r...01r...0 etc
;
;Data out: abcdefgh
;
;ah = 0 = success, not 0 = time out
;al = data
I2CReadByte PROC NEAR                   ;Offset 0x53fa
    push ecx
    push edx
    xor  ch, ch
    mov  cl, 08h
NextBit:                                ;Offset 0x5402
    mov  dl, 14h
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x5408
    dec  dl
    je   TimeOut                        ;Offset 0x5428
    call Sleep                          ;Offset 0x527d
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x5408
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    rcl  ch, 01h                        ;Carry bit is read data
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call Sleep                          ;Offset 0x527d
    dec  cl
    jne  NextBit                        ;Offset 0x5402
    mov  al, ch
TimeOut:                                ;Offset 0x5428
    mov  ah, dl
    pop  edx
    pop  ecx
    ret
I2CReadByte ENDP

DDCReadVDIF:                            ;Offset 0x542f
    mov  al, 4fh
    mov  ah, 01h
    iret

DDCReadEDID:                            ;Offset 0x5434
    push es
    push ecx
    push edi
    call Func0x544f                     ;Offset 0x544f
    or   ah, ah
    jne  Label0x5447                    ;Offset 0x5447
    xor  bl, bl
    or   bl, 02h
    jmp  Label0x5449                    ;Offset 0x5449
Label0x5447:                            ;Offset 0x5447
    xor  bl, bl
Label0x5449:                            ;Offset 0x5449
    pop  edi
    pop  ecx
    pop  es
    iret

Func0x544f PROC NEAR                    ;Offset 0x544f
    push es
    push ebx
    push ecx
    push edx
    push esi
    push edi
    call FindMystique                   ;Offset 0x57c6
    mov  si, offset DDCFuncs            ;Offset 0x5142
    call Func0x52db                     ;Offset 0x52db
    mov  cl, 0fh
Label0x5465:                            ;Offset 0x5465
    call SendStartBit                   ;Offset 0x52e7
    or   al, al
    je   Label0x5501                    ;Offset 0x5501
    mov  al, 0a0h
    call I2CSendByte                    ;Offset 0x53c0
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    call WaitAck                        ;Offset 0x5338
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    mov  al, 00h
    call I2CSendByte                    ;Offset 0x53c0
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    call WaitAck                        ;Offset 0x5338
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    mov  dl, 05h
Label0x5490:                            ;Offset 0x5490
    call Sleep                          ;Offset 0x527d
    dec  dl
    jne  Label0x5490                    ;Offset 0x5490
    call SendStartBit                   ;Offset 0x52e7
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    mov  al, 0a1h
    call I2CSendByte                    ;Offset 0x53c0
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    call WaitAck                        ;Offset 0x5338
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    mov  cl, 7fh
Label0x54b0:                            ;Offset 0x54b0
    call I2CReadByte                    ;Offset 0x53fa
    or   ah, ah
    je   Label0x54ef                    ;Offset 0x54ef
    mov  byte ptr es:[di], al
    call SendAck                        ;Offset 0x536b
    or   al, al
    je   Label0x54ef                    ;Offset 0x54ef
    inc  di
    dec  cl
    jne  Label0x54b0                    ;Offset 0x54b0
    call I2CReadByte                    ;Offset 0x53fa
    or   ah, ah
    je   Label0x54ef                    ;Offset 0x54ef
    mov  byte ptr es:[di], al
    call Func0x5396                     ;Offset 0x5396
    call Sleep                          ;Offset 0x527d
    call SendStopBit                    ;Offset 0x5318
    mov  dl, 05h
Label0x54db:                            ;Offset 0x54db
    call Sleep                          ;Offset 0x527d
    dec  dl
    jne  Label0x54db                    ;Offset 0x54db
    call CheckEDIDHeader                ;Offset 0x5517
    or   al, al
    jne  Label0x5507                    ;Offset 0x5507
    mov  ah, 00h
    mov  al, 4fh
    jmp  Label0x550b                    ;Offset 0x550b
Label0x54ef:                            ;Offset 0x54ef
    call Func0x5396                     ;Offset 0x5396
    call Sleep                          ;Offset 0x527d
    call SendStopBit                    ;Offset 0x5318
    mov  dl, 05h
Label0x54fa:                            ;Offset 0x54fa
    call Sleep                          ;Offset 0x527d
    dec  dl
    jne  Label0x54fa                    ;Offset 0x54fa
Label0x5501:                            ;Offset 0x5501
    dec  cl
    jne  Label0x5465                    ;Offset 0x5465
Label0x5507:                            ;Offset 0x5507
    mov  ah, 01h
    mov  al, 4fh
Label0x550b:                            ;Offset 0x550b
    pop  edi
    pop  esi
    pop  edx
    pop  ecx
    pop  ebx
    pop  es
    ret
Func0x544f ENDP

CheckEDIDHeader PROC NEAR               ;Offset 0x5517
    push       ds
    push       cx
    push       si
    push       di
    cld
    push       cs
    pop        ds
    mov        si, offset EDIDHeader    ;Offset 0x5138
    sub        di, 007fh
    mov        cx, 0008h
    repe cmpsb
    mov        al, cl
    pop        di
    pop        si
    pop        cx
    pop        ds
    ret
CheckEDIDHeader ENDP

Func0x5530 PROC NEAR                    ;Offset 0x5530
    push ax
    push si
    push di
    xor  si, si
Label0x5535:                            ;Offset 0x5535
    mov  ax, 0b102h                     ;Find PCI Device
    mov  cx, PCI_DEVICE_MystiquePCI     ;0x51a
    mov  dx, PCI_VENDOR_Matrox          ;0x102b
    int  1ah
    or   ah, ah
    jne  Label0x5572                    ;Offset 0x5572
    mov  ax, 0b10ch                     ;Write Configuration Word
    mov  cx, MGA_MemAddr_PaletteRamWriteAddress;0x3c00
    mov  di, PCI_MGA_IndirectAccessIndex;0x44
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_Header_Byte_SubClassCode;0xa
    int  1ah
    and  cl, cl
    jne  Label0x55c9                    ;Offset 0x55c9
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_MGA_Option + 03h       ;0x43
    int  1ah
    and  cl, PCI_MGA_Opt_ProductIdMask SHR 24;0x1f
    cmp  cl, 02h
    je   Label0x5574                    ;Offset 0x5574
    cmp  cl, 03h
    jne  Label0x55c9                    ;Offset 0x55c9
    jmp  Label0x5574                    ;Offset 0x5574
Label0x5572:                            ;Offset 0x5572
    jmp  Label0x55d1                    ;Offset 0x55d1
Label0x5574:                            ;Offset 0x5574
    push si
    push bx
    xor  si, si
Label0x5578:                            ;Offset 0x5578
    mov  ax, 0b102h                     ;Find PCI Device
    mov  cx, PCI_DEVICE_IBM27_82351     ;0x22
    mov  dx, PCI_VENDOR_IBM             ;0x1014
    int  1ah
    or   ah, ah
    jne  Label0x55c7                    ;Offset 0x55c7
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_H1_Byte_SecondaryBusNumber;0x19
    int  1ah
    or   ah, ah
    jne  Label0x55c1                    ;Offset 0x55c1
    pop  ax
    push ax
    ;cmp  ah, cl
    DB 03Ah, 0E1h
    jne  Label0x55c1                    ;Offset 0x55c1
    mov  ax, 0b10bh                     ;Write Configuration Byte
    mov  cl, PCI_Cmd_SERRDisable OR PCI_Cmd_FastBackToBackDisable OR PCI_Cmd_InterruptEnable;0x0
    mov  di, PCI_Header_Word_Command + 1;0x5
    int  1ah
    mov  ax, 0b10bh                     ;Write Configuration Byte
    mov  cl, 0f4h                       ;Unknown value
    mov  di, 0041h                      ;Unknown register
    int  1ah
    mov  ax, 0b10bh                     ;Write Configuration Byte
    mov  cl, 0f0h
    mov  di, PCI_H1_Byte_IOBase         ;0x1c
    int  1ah
    mov  ax, 0b10bh                     ;Write Configuration Byte
    mov  cl, 00h
    mov  di, PCI_H1_Byte_IOLimit        ;0x1d
    int  1ah
Label0x55c1:                            ;Offset 0x55c1
    inc  si
    cmp  si, 0020h
    jl   Label0x5578                    ;Offset 0x5578
Label0x55c7:                            ;Offset 0x55c7
    pop  bx
    pop  si
Label0x55c9:                            ;Offset 0x55c9
    inc  si
    cmp  si, 0020h
    jl   Label0x5535                    ;Offset 0x5535
Label0x55d1:                            ;Offset 0x55d1
    pop  di
    pop  si
    pop  ax
    ret
Func0x5530 ENDP

ReadSubClassCode PROC NEAR              ;Offset 0x55d5
    push di
    mov  ax, 0b109h                     ;Read Configuration Word
    mov  di, PCI_Header_Word_VendorID   ;0x0
    int  1ah
    cmp  cx, PCI_VENDOR_Matrox          ;0x102b
    jne  NotFound                       ;Offset 0x55f9
    mov  ax, 0b10ch                     ;Write Configuration Word
    mov  di, PCI_MGA_IndirectAccessIndex;0x44
    mov  cx, MGA_MemAddr_PaletteRamWriteAddress;0x3c00
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_Header_Byte_SubClassCode;0xa
    int  1ah
    and  cl, cl
NotFound:                               ;Offset 0x55f9
    pop  di
    ret
ReadSubClassCode ENDP

Func0x55fb PROC NEAR                    ;Offset 0x55fb
    push cx
    mov  cl, MGA_INDD_Reserved3f        ;0x3f ???
    call ReadIndexedRegister            ;Offset 0x5bab
    cmp  cl, 30h
    pop  cx
    ret
Func0x55fb ENDP

IsExtensionReg7Writeable PROC NEAR      ;Offset 0x5606
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  al, MGA_CRTCExt_Reserved7      ;0x7
    out  dx, al
    in   ax, dx
    push ax
    mov  ax, 0a007h                     ;Unknown values.
    out  dx, ax                         ;It seems to be checking if extension reg 7 is writeable.
    in   ax, dx
    cmp  ah, 0a0h
    pop  ax
    out  dx, ax
    pop  dx
    ret
IsExtensionReg7Writeable ENDP

;PCI Configuration Mechanism 2
FindMystiqueMechanism2 PROC NEAR        ;Offset 0x
    push   ax
    push   cx
    push   dx
    xor    bx, bx
    mov    al, PCI_Header_Word_VendorID ;0x0
    mov    dx, PCI_M2_ForwardRegister   ;Port 0xcfa
    out    dx, al
    mov    al, 80h
    mov    dx, PCI_M2_ConfigSpaceEnable ;Port 0xcf8
    out    dx, al
    mov    dx, PCI_M2_DataBase          ;Port 0xc000
Label0x562f:                            ;Offset 0x562f
    xor    dl, dl
    in     ax, dx
    cmp    ax, PCI_VENDOR_Matrox        ;0x102b
    jne    Label0x5651                  ;Offset 0x5651
    mov    dl, PCI_Header_Word_DeviceID ;0x2
    in     ax, dx
    cmp    ax, PCI_DEVICE_MystiquePCI   ;0x51a
    jne    Label0x5651                  ;Offset 0x5651
    mov    dl, PCI_Header_Word_Command  ;0x4
    in     al, dx
    and    al, PCI_Cmd_IOSpace          ;0x1
    je     Label0x5651                  ;Offset 0x5651
    mov    dl, PCI_Header_Byte_SubClassCode;0xa
    in     al, dx
    or     al, al
    jne    Label0x5651                  ;Offset 0x5651
    mov    bh, dh
    jmp    Label0x566e                  ;Offset 0x566e
Label0x5651:                            ;Offset 0x5651
    xor    dl, dl
    inc    dh
    cmp    dh, 0d0h
    jne    Label0x562f                  ;Offset 0x562f
    mov    dh, 0c0h
    inc    bx
    mov    al, bl
    push   dx
    mov    dx, PCI_M2_ForwardRegister   ;Port 0xcfa
    out    dx, al
    pop    dx
    cmp    bx, 0100h
    jne    Label0x562f                  ;Offset 0x562f
    mov    bx, 0ffffh
Label0x566e:                            ;Offset 0x566e
    xor    al, al
    mov    dx, PCI_M2_ConfigSpaceEnable ;Port 0xcf8
    out    dx, al
    pop    dx
    pop    cx
    pop    ax
    ret
FindMystiqueMechanism2 ENDP

;
;inputs:
;   -
;
;outputs:
;   bx = Bus, Device ID and function ID
;
;destroys:
;   flags
;
FindMystiqueMechanism1 PROC NEAR        ;Offset 0x5678
    push  eax
    push  ebx
    push  ecx
    push  edx
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch                       ;PCI M1 port high byte
LoopBus:                                ;Offset 0x568a
    mov   cl, 20h                       ;Check 32 devices on this bus
LoopDevices:                            ;Offset 0x568c
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax                       ;Address for Header field 0
    mov   dl, PCI_M1_ConfigData_lowbyte ;Port 0xcfc
    in    eax, dx                       ;Read Device + Vendor ID's
    cmp   eax, (PCI_DEVICE_MystiquePCI SHL 16) OR PCI_VENDOR_Matrox;0x51a102b
    jne   NextDevice                    ;Offset 0x56c5
    mov   eax, ebx
    mov   al, PCI_Header_Word_Command   ;0x4
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B0_lowbyte;Port 0xcfc
    in    al, dx
    and   al, PCI_Cmd_IOSpace           ;0x1 Check if not IO space
    je    NextDevice                    ;Offset 0x56c5
    mov   eax, ebx
    mov   al, PCI_Header_Byte_RevisionID;0x8
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B2_lowbyte;Port 0xcfe
    in    al, dx                        ;PCI_Header_Byte_SubClassCode
    or    al, al                        ;Check if not 0
    jne   NextDevice                    ;Offset 0x56c5
    shr   ebx, 08h                      ;bx = Bus, Device ID and function ID
    jmp   DeviceFound                   ;Offset 0x56e4
NextDevice:                             ;Offset 0x56c5
    dec   cl
    je    NextBus                       ;Offset 0x56d2
    add   ebx, 00000800h                ;Next device
    jmp   LoopDevices                   ;Offset 0x568c
NextBus:                                ;Offset 0x56d2
    and   bx, 07ffh                     ;Drop device index
    add   ebx, 00010000h                ;Next Bus
    dec   ch
    jne   LoopBus                       ;Offset 0x568a
    mov   bx, 0ffffh                    ;Device not found, set Bus, device ID and function ID to invalid
DeviceFound:                            ;Offset 0x56e4
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    xor   eax, eax
    out   dx, eax
    mov   ax, bx
    pop   edx
    pop   ecx
    pop   ebx
    mov   bx, ax
    pop   eax
    ret
FindMystiqueMechanism1 ENDP

;
;inputs:
;   -
;
;outputs:
;   eax = 0 = not found, eax = device ID, Vendor ID = found
;
;destroys:
;   cx, dx, flags
;
FindIntelTriton430FXChipset PROC NEAR   ;Offset 0x56f8
    push  ebx
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch
    mov   cl, 10h
CheckNextDevice:                        ;Offset 0x5706
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_lowbyte;Port 0xcfc
    in    eax, dx
    cmp   eax, (PCI_DEVICE_Triton_430FX SHL 16) OR PCI_VENDOR_Intel;Intel Triton 430FX chipset 0x122d8086
    je    FoundDevice                   ;Offset 0x5729
    dec   cl
    je    SearchDone                    ;Offset 0x5726
    add   ebx, 00000800h
    jmp   CheckNextDevice               ;Offset 0x5706
SearchDone:                             ;Offset 0x5726
    xor   eax, eax
FoundDevice:                            ;Offset 0x5729
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    xchg  eax, ebx
    out   dx, eax                       ;Select device for changes.
    xchg  eax, ebx
    pop   ebx
    ret
FindIntelTriton430FXChipset ENDP

;
;inputs:
;   al = action
;       0x8 = Read Byte
;       0x9 = Read Word
;       0xa = Read DWord
;       0xb = Write Byte
;       0xc = Write Word
;       0xd = Write DWord
;   bx = Bus, Device ID and Function ID
;   di = register
;  ecx,cx,cl = value (optional)
;
;outputs:
; ecx,cx,cl = value (optional)
;
;destroys:
;   -
;
AccessPCIRegister PROC NEAR             ;Offset 0x5734
    push  eax
    push  bx
    push  dx
    push  di
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    cmp   bh, 0c0h
    jb    Mechanism1                    ;Offset 0x5755
    push  ax
    push  dx
    mov   dl, PCI_M2_ForwardRegister_lowbyte;Port 0xcfa
    mov   al, bl
    out   dx, al
    xor   bl, bl
    pop   dx
    mov   al, 80h
    out   dx, al
    mov   dx, bx
    or    dx, di
    pop   ax
    jmp   PerformAccess                 ;Offset 0x5771
Mechanism1:                             ;Offset 0x5755
    push  ax
    mov   eax, PCI_M1_ConfigAddres_Enable SHR 8;0x800000
    mov   ax, bx
    shl   eax, 08h
    or    ax, di
    and   al, 0fch
    out   dx, eax
    mov   dx, PCI_M1_ConfigData         ;Port 0xcfc
    and   di, 0003h
    add   dx, di                        ;Sub-register offset
    pop   ax
PerformAccess:                          ;Offset 0x5771
    cmp   al, PCI_ACCESS_ReadByte       ;0x8
    je    ReadByte                      ;Offset 0x57a8
    cmp   al, PCI_ACCESS_ReadWord       ;0x9
    je    ReadWord                      ;Offset 0x57a3
    cmp   al, PCI_ACCESS_ReadDWord      ;0xa
    je    ReadDWord                     ;Offset 0x579c
    cmp   al, PCI_ACCESS_WriteByte      ;0xb
    je    WriteByte                     ;Offset 0x5797
    cmp   al, PCI_ACCESS_WriteWord      ;0xc
    je    WriteWord                     ;Offset 0x5792
    cmp   al, PCI_ACCESS_WriteDWord     ;0xd
    je    WriteDWord                    ;Offset 0x578b
    jmp   Done                          ;Offset 0x57ab
WriteDWord:                             ;Offset 0x578b
    mov   eax, ecx
    out   dx, eax
    jmp   Done                          ;Offset 0x57ab
WriteWord:                              ;Offset 0x5792
    mov   ax, cx
    out   dx, ax
    jmp   Done                          ;Offset 0x57ab
WriteByte:                              ;Offset 0x5797
    mov   al, cl
    out   dx, al
    jmp   Done                          ;Offset 0x57ab
ReadDWord:                              ;Offset 0x579c
    in    eax, dx
    mov   ecx, eax
    jmp   Done                          ;Offset 0x57ab
ReadWord:                               ;Offset 0x57a3
    in    ax, dx
    mov   cx, ax
    jmp   Done                          ;Offset 0x57ab
ReadByte:                               ;Offset 0x57a8
    in    al, dx
    mov   cl, al
Done:                                   ;Offset 0x57ab
    cmp   dh, 0c0h
    jl    IsMechanism1                  ;Offset 0x57b8
    xor   al, al
    mov   dx, PCI_M2_ConfigSpaceEnable  ;Port 0xcf8
    out   dx, al
    jmp   Return                        ;Offset 0x57c0
IsMechanism1:                           ;Offset 0x57b8
    xor   eax, eax
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    out   dx, eax
Return:                                 ;Offset 0x57c0
    pop   di
    pop   dx
    pop   bx
    pop   eax
    ret
AccessPCIRegister ENDP

;
;outputs:
;   bx = Bus, Device ID and Function ID
;
FindMystique PROC NEAR                  ;Offset 0x57c6
    call  FindMystiqueMechanism1        ;Offset 0x5678
    inc   bx
    jne   Found                         ;Offset 0x57d0
    call  FindMystiqueMechanism2        ;Offset 0x561b
    inc   bx
Found:                                  ;Offset 0x57d0
    dec   bx
    ret
FindMystique ENDP

ReadPCIBase0AndBase1 PROC NEAR          ;Offset 0x57d2
    push  di
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 + 2;0x12
    call  AccessPCIRegister             ;Offset 0x5734
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 ;0x10
    call  AccessPCIRegister             ;Offset 0x5734
    and   ecx, 0fffffff0h
    mov   eax, ecx
    push  eax
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 + 2;0x16
    call  AccessPCIRegister             ;Offset 0x5734
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 ;0x14
    call  AccessPCIRegister             ;Offset 0x5734
    and   ecx, 0fffffff0h
    push  ecx
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_Header_Byte_RevisionID;0x8
    call  AccessPCIRegister             ;Offset 0x5734
    cmp   cl, MGA_Mystique_Rev3         ;0x3
    jb    Label0x581d                   ;Offset 0x581d
    pop   eax
    pop   ecx
    jmp   Label0x5821                   ;Offset 0x5821
Label0x581d:                            ;Offset 0x581d
    pop   ecx
    pop   eax
Label0x5821:                            ;Offset 0x5821
    pop   di
    ret
ReadPCIBase0AndBase1 ENDP

ReadPCIDeviceID PROC NEAR               ;Offset 0x5823
    push  di
    push  cx
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_Header_Word_DeviceID  ;0x2
    call  AccessPCIRegister             ;Offset 0x5734
    mov   ax, cx
    pop   cx
    pop   di
    ret
ReadPCIDeviceID ENDP

CapAXTo0x40 PROC NEAR                   ;Offset 0x5832
    ;cmp   ax, 40h
    DB 080h, 0FCh, 040h
    ;jbe   IsLess                        ;Offset 0x583b
    DB 00Fh, 086h, 002h, 000h
    mov  ah, 40h
IsLess:                                 ;Offset 0x583b
    cmp  ah, 00h
    ;jne  NotZero                        ;Offset 0x5845
    DB 00Fh, 085h, 003h, 000h
    mov  ax, 4000h
NotZero:                                ;Offset 0x5845
    ret
CapAXTo0x40 ENDP

GetMemoryIn64KBlocks PROC NEAR          ;Offset 0x5846
    push  di
    call  GetMemoryExternal             ;Offset 0x5855
    mov   ax, di
    ;je    Label0x5853                   ;Offset 0x5853
    DB 00Fh, 084h, 003h, 000h
    call  GetMemorySize                 ;Offset 0x5866
Label0x5853:                            ;Offset 0x5853
    pop   di
    ret
GetMemoryIn64KBlocks ENDP

GetMemoryExternal PROC NEAR             ;Offset 0x5855
    push  dx
    mov   ax, 0bd50h
    xor   dx, dx
    int   6dh
    shl   dx, 0ch
    mov   di, dx
    pop   dx
    or    ah, ah
    ret
GetMemoryExternal ENDP

;inputs:
;   -
;
;outputs:
;   ah = Size of memory in 64KB blocks
;
GetMemorySize PROC NEAR                 ;Offset 0x5866
    push  bx
    push  cx
    push  dx
    push  ds
    push  di
    push  si
    mov   dx, VGA_SequenceIndex         ;Port 0x3c4
    in    al, dx
    push  ax
    mov   al, VGA_SEQIdx_ClockingMode   ;0x1
    out   dx, al
    in    ax, dx
    push  ax
    or    ah, VGA_SEQ1_ScreenOff        ;0x20
    out   dx, ax
    mov   cx, 04b0h
Label0x587d:                            ;Offset 0x587d
    loop  Label0x587d                   ;Offset 0x587d Speed sensitive!
    mov   dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov   al, VGA_GCTLIdx_Miscellaneous ;0x6
    out   dx, al
    in    ax, dx
    push  ax
    and   ah, VGA_GCTL6_GfxMode OR VGA_GCTL6_ChainEven;0x3
    or    ah, VGA_GCTL6_Mem_A0000_AFFFF ;0x4h
    out   dx, ax
    mov   dx, VGA_InputStatus1D         ;Port 0x3da
    xor   cx, cx
Label0x5893:                            ;Offset 0x5893
    dec   cx
    je    Label0x58a3                   ;Offset 0x58a3
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x5893                   ;Offset 0x5893
Label0x589b:                            ;Offset 0x589b
    dec   cx
    je    Label0x58a3                   ;Offset 0x58a3
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x589b                   ;Offset 0x589b
Label0x58a3:                            ;Offset 0x58a3
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    mov   al, MGA_CRTCExt_Misc          ;0x3
    out   dx, al
    in    ax, dx
    push  ax
    mov   ax, ((MGA_CRTCEXT3_ScaleDiv2 OR MGA_CRTCEXT3_MGAModeEnable) SHL 8) OR MGA_CRTCExt_Misc;0x8103
    out   dx, ax
    mov   al, MGA_CRTCExt_MemoryPage    ;0x4
    out   dx, al
    in    ax, dx
    push  ax
    mov   bx, 0a000h                    ;Segment 0xa000
    mov   ds, bx
    xor   si, si
    dec   si
    dec   si
    call  FindMystique                  ;Offset 0x57c6
    call  ReadPCIDeviceID               ;Offset 0x5823
    mov   di, 2000h
    mov   bx, 8000h                     ;8MB max
    cmp   ax, PCI_DEVICE_MillenniumIIPCI;0x051b
    ;jb    Label0x58d2                   ;Offset 0x58d2
    DB 00Fh, 082h, 002h, 000h
    xor   bx, bx
Label0x58d2:                            ;Offset 0x58d2
    mov   ax, (20h SHL 8)OR MGA_CRTCExt_MemoryPage;0x2004 Start at 2MB
Label0x58d5:                            ;Offset 0x58d5
    out   dx, ax
    mov   ch, byte ptr [si]
    mov   cl, 0a5h
    mov   byte ptr [si], cl
    push  di
    push  cx
    not   cx
    mov   di, 0000h
    mov   ch, byte ptr [di]
    mov   byte ptr [di], cl
    mov   byte ptr [di], ch
    pop   cx
    pop   di
    cmp   cl, byte ptr [si]
    mov   byte ptr [si], ch
    jne   Label0x58fc                   ;Offset 0x58fc
    add   di, 1000h
    add   ah, 10h
    cmp   bx, di
    jne   Label0x58d5                   ;Offset 0x58d5
Label0x58fc:                            ;Offset 0x58fc
    pop   ax
    out   dx, ax
    pop   ax
    out   dx, ax
    pop   ax
    mov   dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    out   dx, ax
    pop   ax
    mov   dl, VGA_SequenceIndex_lowbyte ;Port 0x3c4
    out   dx, ax
    pop   ax
    out   dx, al
    or    di, di
    ;jne   Label0x5913                   ;Offset 0x5913
    DB 00Fh, 085h, 003h, 000h
    mov   di, 0001h
Label0x5913:                            ;Offset 0x5913
    mov   ax, di
    pop   si
    pop   di
    pop   ds
    pop   dx
    pop   cx
    pop   bx
    ret
GetMemorySize ENDP

;
;inputs:
;   si = Indirect Access Register
;   cl = value
;
;outputs:
;   -
;
;destroys:
;   al
;
IndirectRegisterWriteByte PROC NEAR     ;Offset 0x591c
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xch
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    cx
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterWriteByte ENDP

;
;inputs:
;   si = index
;   cx = value
;
;outputs
;   -
;
;destroys:
;   al
;
IndirectRegisterWriteWord PROC NEAR     ;Offset 0x593a
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterWriteWord ENDP

;
;inputs:
;   si  = index
;   ecx = value
;
;outputs
;   -
;
;destroys:
;   al
;
IndirectRegisterWriteDWord PROC NEAR    ;Offset 0x5958
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    cx
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterWriteDWord ENDP

;
;inputs:
;   si = index
;
;outputs
;   cl = value
;
;destroys:
;   al
;
IndirectRegisterReadByte PROC NEAR      ;Offset 0x5971
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterReadByte ENDP

;
;inputs:
;   si = index
;
;outputs
;   cx = value
;
;destroys:
;   al
;
IndirectRegisterReadWord PROC NEAR      ;Offset 0x598d
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, PCI_ACCESS_ReadWord      ;0x9
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterReadWord ENDP

;
;inputs:
;   si = index
;
;outputs
;   ecx = value
;
;destroys:
;   al
;
IndirectRegisterReadDWord PROC NEAR     ;Offset 0x59a9
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    si
    ret
IndirectRegisterReadDWord ENDP

Func0x59c0 PROC NEAR                    ;Offset 0x59c0
    push  ds
    xor   ax, ax
    mov   ds, ax
    mov   ax, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je    IsMode13                      ;Offset 0x59d2
    mov   ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
IsMode13:                               ;Offset 0x59d2
    and   al, 0fh
    pop   ds
    ret
Func0x59c0 ENDP

Func0x59d6 PROC NEAR                    ;Offset 0x59d6
    push  ds
    push  0000h
    pop   ds
    or    al, al
    mov   ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    je    Label0x59eb                   ;Offset 0x59eb
    shr   al, 04h
    and   cl, 0f0h
    or    cl, al
    jmp   Label0x59f7                   ;Offset 0x59f7
Label0x59eb:                            ;Offset 0x59eb
    push  cx
    shl   cl, 04h
    and   al, 0fh
    or    al, cl
    mov   word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    pop   cx
Label0x59f7:                            ;Offset 0x59f7
    pop   ds
    ret
Func0x59d6 ENDP

GetStartAddress PROC NEAR               ;Offset 0x59f9
    push  dx
    push  ds
    xor   eax, eax
    mov   ds, ax
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    mov   al, MGA_CRTCExt_AddrGeneratorExt;0x0
    out   dx, al
    inc   dx
    in    al, dx
    and   al, MGA_CRTEXT0_StartAddress19_16;0xf
    shl   eax, 10h
    mov   dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov   al, VGA_CRTCIdx_StartAddrHigh ;0xc
    out   dx, al
    inc   dx
    in    al, dx
    xchg  al, ah
    dec   dx
    mov   al, VGA_CRTCIdx_StartAddrLow  ;0xd
    out   dx, al
    inc   dx
    in    al, dx
    call  Func0x6416                    ;Offset 0x6416
    je    Label0x5a25                   ;Offset 0x5a25
    shl   eax, 01h
Label0x5a25:                            ;Offset 0x5a25
    pop   ds
    pop   dx
    ret
GetStartAddress ENDP

SetStartAddress PROC NEAR               ;Offset 0x5a28
    push  dx
    push  ds
    xor   dx, dx
    mov   ds, dx
    call  Func0x6416                    ;Offset 0x6416
    je    Label0x5a36                   ;Offset 0x5a36
    shr   eax, 01h
Label0x5a36:                            ;Offset 0x5a36
    pop   ds
    push  eax
    test  bl, 80h
    ;je    Label0x5a48                   ;Offset 0x5a48
    DB 00Fh, 084h, 008h, 000h
    mov   dx, VGA_InputStatus1D         ;Port 0x3da
Label0x5a43:                            ;Offset 0x5a43
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x5a43                   ;Offset 0x5a43
Label0x5a48:                            ;Offset 0x5a48
    mov   dx, VGA_CRTControllerIndexD   ;Port 0x3d4
    mov   al, VGA_CRTCIdx_StartAddrLow  ;0xd
    out   dx, al
    inc   dx
    pop   ax
    out   dx, al
    dec   dx
    mov   al, VGA_CRTCIdx_StartAddrHigh ;0xc
    out   dx, al
    inc   dx
    xchg  al, ah
    out   dx, al
    mov   dl, MGA_CRTCExtensionIndex_lowbyte;Port 0x3de
    xor   al, al                        ;MGA_CRTCExt_AddrGeneratorExt
    out   dx, al
    inc   dx
    pop   ax
    xchg  al, ah
    in    al, dx
    and   al, NOT MGA_CRTEXT0_StartAddress19_16;0xf0
    or    al, ah
    out   dx, al
    pop   dx
    ret
SetStartAddress ENDP

Func0x5a6a PROC NEAR                    ;Offset 0x5a6a
    push      ebx
    push      edx
    xor       ebx, ebx
    call      Func0x5ae1                ;Offset 0x5ae1
    shr       bx, 03h
    call      GetStartAddress           ;Offset 0x59f9
    xor       edx, edx
    div       ebx
    mov       cx, dx
    push      ax
    call      Func0x59c0                ;Offset 0x59c0
    and       al, 03h
    je        Label0x5a94               ;Offset 0x5a94
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shl       ax, cl
    xchg      ax, cx
Label0x5a94:                            ;Offset 0x5a94
    pop       ax
    pop       edx
    pop       ebx
    mov       dx, ax
    mov       al, 01h
    call      Func0x59d6                ;Offset 0x59d6
    ret
Func0x5a6a ENDP

Func0x5aa1 PROC NEAR                    ;Offset 0x5aa1
    push      ebx
    push      ecx
    push      edx
    xor       al, al
    call      Func0x59d6                ;Offset 0x59d6
    push      bx
    push      cx
    push      dx
    call      Func0x5ae1                ;Offset 0x5ae1
    shr       bx, 03h
    mov       ax, bx
    pop       dx
    mul       dx
    push      dx
    push      ax
    pop       eax
    xor       ecx, ecx
    pop       cx
    pop       bx
    push      ax
    call      Func0x59c0                ;Offset 0x59c0
    and       al, 03h
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shr       ax, cl
    xchg      ax, cx
    pop       ax
    add       eax, ecx
    call      SetStartAddress           ;Offset 0x5a28
    pop       edx
    pop       ecx
    pop       ebx
    ret
Func0x5aa1 ENDP

Func0x5ae1 PROC NEAR                    ;Offset 0x5ae1
    push      ds
    xor       ax, ax
    mov       ds, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    out       dx, al
    inc       dx
    in        al, dx
    shr       al, 04h
    xchg      al, ah
    and       ah, 03h
    mov       dl, VGA_CRTControllerIndexD_lowbyte;0xd4
    mov       al, VGA_CRTCIdx_Offset    ;0x13
    out       dx, al
    inc       dx
    in        al, dx
    call      Func0x6416                ;Offset 0x6416
    je        Label0x5b02               ;Offset 0x5b02
    shl       ax, 01h
Label0x5b02:                            ;Offset 0x5b02
    mov   bx, ax
    xor   ax, ax
    mov   dx, ax
    call  Func0x59c0                    ;Offset 0x59c0
    mov   dl, ah
    mov   cl, al
    and   cl, 03h
    shl   bx, 04h
    dec   cl
    mov   ax, bx
    shr   ax, cl
    mov   cx, ax
    xor   ax, ax
    div   bx
    mov   dx, ax
    pop   ds
    ret
Func0x5ae1 ENDP

Func0x5b25 PROC NEAR                    ;Offset 0x5b25
    push      ds
    push      cx
    xor       cx, cx
    mov       ds, cx
    call      Func0x6416                    ;Offset 0x6416
    je        Label0x5b32                   ;Offset 0x5b32
    shr       ax, 01h
Label0x5b32:                            ;Offset 0x5b32
    mov      ch, byte ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    and      ch, 03h
    dec      ch
    mov      cl, 04h
    sub      cl, ch
    shr      ax, cl
    mov      bx, ax
    shl      bx, 03h
    push     ax
    shl      ah, 04h
    mov      dx, MGA_CRTCExtensionIndex ;Port 0x3de
    xor      al, al                    ;MGA_CRTCExt_AddrGeneratorExt
    out      dx, al
    inc      dx
    in       al, dx
    and      al, NOT MGA_CRTEXT0_Offset9_8;0xcf
    or       al, ah
    out      dx, al
    mov      dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov      al, VGA_CRTCIdx_Offset     ;0x13
    out      dx, al
    inc      dx
    pop      ax
    out      dx, al
    mov      dl, byte ptr ds:[BDA_VideoBufferSize + 01h];Offset 0x44d
    xor      dh, dh
    xor      ax, ax
    div      bx
    mov      dx, ax
    pop      cx
    pop      ds
    ret
Func0x5b25 ENDP

Func0x5b6e PROC NEAR                    ;Offset 0x5b6e
    mov       bx, ax
    call      Func0x59c0                ;Offset 0x59c0
    and       al, 03h
    je        Label0x5b8f               ;Offset 0x5b8f
    mov       cl, al
    dec       cl
    mov       ax, 0010h
    shr       ax, cl
    xor       dx, dx
    xchg      ax, bx
    push      ax
    div       bx
    pop       ax
    or        dx, dx
    je        Label0x5b8f               ;Offset 0x5b8f
    sub       bx, dx
    add       ax, bx
Label0x5b8f:                            ;Offset 0x5b8f
    ret
Func0x5b6e ENDP

;
;inputs:
;   cl = RAMDAC write index
;   ch = Value
;
;destroys:
;   ax
;
WriteIndexedRegister PROC NEAR          ;Offset 0x5b90
    push   si
    push   dx
    mov    al, cl
    and    al, 80h
    jne    Label0x5b9e                  ;Offset 0x5b9e
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
Label0x5b9e:                            ;Offset 0x5b9e
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterWriteByte    ;Offset 0x591c
    xchg   cl, ch
    pop    dx
    pop    si
    ret
WriteIndexedRegister ENDP

;
;inputs:
;   cl = RAMDAC read index
;
;outputs:
;   cl = Value
;
;destroys:
;   ax
;
ReadIndexedRegister PROC NEAR           ;Offset 0x5bab
    push   si
    push   dx
    mov    al, cl
    and    al, 80h
    jne    Label0x5bb9                  ;Offset 0x5bb9
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
Label0x5bb9:                            ;Offset 0x5bb9
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterReadByte     ;Offset 0x5971
    pop    dx
    pop    si
    ret
ReadIndexedRegister ENDP

;Unused function
WriteIndexedRegisters PROC NEAR         ;Offset 0x5bc4
    push cx
    push si
Label0x5bc6:                            ;Offset 0x5bc6
    mov  cx, word ptr [si]
    cmp  cl, 0ffh
    je   Label0x5bd4                    ;Offset 0x5bd4
    call WriteIndexedRegister           ;Offset 0x5b90
    inc  si
    inc  si
    jmp  Label0x5bc6                    ;Offset 0x5bc6
Label0x5bd4:                            ;Offset 0x5bd4
    xor  ax, ax
    pop  si
    pop  cx
    ret
WriteIndexedRegisters ENDP

FetchDefaultSystemClockSpeed PROC NEAR  ;Offset 0x5bd9
    push ax
    push bx
    call GetMemorySize                  ;Offset 0x5866
    xchg ah, al
    xor  dx, dx
    xor  bx, bx
    dec  dl
    dec  bl
    ;cmp  ax, 0040h
    DB 03Dh, 040h, 000h
    ;jbe  Label0x5bfe                    ;Offset 0x5bfe
    DB 00Fh, 086h, 00Fh, 000h
    mov  dl, byte ptr cs:[Data0x780e]   ;Offset 0x780e
    cmp  dl, 0ffh
    ;je   Label0x5bfe                    ;Offset 0x5bfe
    DB 00Fh, 084h, 003h, 000h
    jmp  Label0x5c34                    ;Offset 0x5c34
    nop
Label0x5bfe:                            ;Offset 0x5bfe
    ;cmp  ax, 0020h
    DB 03Dh, 020h, 000h
    ;jbe  Label0x5c14                    ;Offset 0x5c14
    DB 00Fh, 086h, 00Fh, 000h
    mov  dl, byte ptr cs:[Data0x780d]   ;Offset 0x780d
    cmp  dl, 0ffh
    ;je   Label0x5c14                    ;Offset 0x5c14
    DB 00Fh, 084h, 003h, 000h
    jmp  Label0x5c34                    ;Offset 0x5c34
    nop
Label0x5c14:                            ;Offset 0x5c14
    mov  dl, byte ptr cs:[Data0x780b]   ;Offset 0x780b
    cmp  dl, 0ffh
    ;je   Label0x5c23                    ;Offset 0x5c23
    DB 00Fh, 084h, 003h, 000h
    jmp  Label0x5c34                    ;Offset 0x5c34
    nop
Label0x5c23:                            ;Offset 0x5c23
    mov  bl, byte ptr cs:[Data0x780c]   ;Offset 0x780c
    cmp  bl, 0ffh
    ;je   Label0x5c32                    ;Offset 0x5c32
    DB 00Fh, 084h, 003h, 000h
    jmp  Label0x5c34                    ;Offset 0x5c34
    nop
Label0x5c32:                            ;Offset 0x5c32
    mov  dl, 2ch
Label0x5c34:                            ;Offset 0x5c34
    mov  al, byte ptr cs:[Data0x780c]   ;Offset 0x780c
    cmp  byte ptr cs:[Data0x780b], al   ;Offset 0x780b
    ;jne  Label0x5c4f                    ;Offset 0x5c4f
    DB 00Fh, 085h, 00Eh, 000h
    cmp  byte ptr cs:[Data0x780b], 0ffh ;Offset 0x780b
    ;je   Label0x5c4f                    ;Offset 0x5c4f
    DB 00Fh, 084h, 004h, 000h
    mov  cl, 0dh
    jmp  Label0x5c6a                    ;Offset 0x5c6a
Label0x5c4f:                            ;Offset 0x5c4f
    cmp  dl, 0ffh
    ;je   Label0x5c62                    ;Offset 0x5c62
    DB 00Fh, 084h, 00Ch, 000h
    mov  ax, dx
    xor  dx, dx
    mov  cx, 03h
    mul  cx
    jmp  Label0x5c66                    ;Offset 0x5c66
    nop
Label0x5c62:                            ;Offset 0x5c62
    mov  ax, bx
    shl  ax, 01h
Label0x5c66:                            ;Offset 0x5c66
    mov  dx, ax
    mov  cl, 01h
Label0x5c6a:                            ;Offset 0x5c6a
    pop  bx
    pop  ax
    ret
FetchDefaultSystemClockSpeed ENDP

PowerUpPLLsLUTsAndConfigure PROC NEAR   ;Offset 0x5c6d
    push  ax
    push  bx
    push  cx
    push  dx
    push  di
    push  cx
    call  FindMystique                  ;Offset 0x57c6
    call  FindIntelTriton430FXChipset   ;Offset 0x56f8
    or    ax, ax
    je    NoIntelTriton430FX            ;Offset 0x5c8d
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_MGA_Option + 03h      ;0x43
    call  AccessPCIRegister             ;Offset 0x5734
    or    cl, (PCI_MGA_Opt_NoRetry SHR 24);0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5734
NoIntelTriton430FX:                     ;Offset 0x5c8d
    mov   cl, MGA_INDD_VoltageReferenceControl;0x18
    mov   ch, MGA_VREFCTRL_SysPLLVoltRefBlkPwrUp OR MGA_VREFCTRL_SysPLLVoltRefPLLRefBlk;0x3
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   al, 0fh
    call  Sleep2                        ;Offset 0x31f0
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5734
    or    cl, PCI_MGA_Opt_SysPLLPowerUp ;0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5734
    mov   al, 01h
    call  Sleep2                        ;Offset 0x31f0
    xor   dx, dx
PLLNotFrequencyLocked:                  ;Offset 0x5cb0
    dec   dx
    je    PLLFreqLockTimeout            ;Offset 0x5cbd
    mov   cl, MGA_INDD_SYSPLL_Status    ;0x2f
    call  ReadIndexedRegister           ;Offset 0x5bab
    and   cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je    PLLNotFrequencyLocked         ;Offset 0x5cb0
PLLFreqLockTimeout:                     ;Offset 0x5cbd
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x5bab
    or    cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   al, 01h
    call  Sleep2                        ;Offset 0x31f0
    xor   dx, dx
PIXNotFrequencyLocked:                  ;Offset 0x5cd3
    dec   dx
    je    PIXFreqLockTimeout            ;Offset 0x5ce0
    mov   cl, MGA_INDD_PIXPLL_Status    ;0x4f
    call  ReadIndexedRegister           ;Offset 0x5bab
    and   cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je    PIXNotFrequencyLocked         ;Offset 0x5cd3
PIXFreqLockTimeout:                     ;Offset 0x5ce0
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x5bab
    or    cl, MGA_MISCCTRL_LUT_Enable   ;0x10
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x5bab
    or    cl, MGA_MISCCTRL_DAC_Enable   ;0x1
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5734
    or    cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5734
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5734
    and   cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or    cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5734
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5734
    and   cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5734
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x5bab
    or    cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x5bab
    and   cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or    cl, MGA_PIXCLKCTRL_SelPLL     ;0x1
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x5bab
    and   cl, NOT MGA_PIXCLKCTRL_ClockDisable;0xfb
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5b90
    pop   ax
    call  FetchDefaultSystemClockSpeed  ;Offset 0x5bd9
    or    ax, ax
    ;je    Label0x5d6d                   ;Offset 0x5d6d
    DB 00Fh, 084h, 002h, 000h
    mov   dx, ax
Label0x5d6d:                            ;Offset 0x5d6d
    mov   ax, dx                        ;ax = frequency in KHz
    xor   dx, dx
    mov   bx, 03e8h                     ;1,000
    mul   bx                            ;dx:ax is frequency in Hz
    shl   edx, 10h
    mov   dx, ax                        ;edx = frequency in Hz
    call  ConfigureSystemClock                    ;Offset 0x5f91
    pop   di
    pop   dx
    pop   cx
    pop   bx
    pop   ax
    ret
PowerUpPLLsLUTsAndConfigure ENDP

SetMemoryTimingsAndRefresh PROC NEAR    ;Offset 0x5d85
    push   ax
    push   bx
    push   cx
    push   dx
    push   di
    call   FindMystique                 ;Offset 0x57c6
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out    dx, ax
    mov    si, MGA_MemAddr_MemoryControlWaitState;0x1c08
    mov    dl, byte ptr cs:[MemoryControlWaitState];Offset 0x7813
    mov    cl, dl
    and    cl, MGA_MA_MCWS_RASMinActive_MASK SHR 14;0xc
    shl    ecx, 0eh
    mov    cl, dl
    and    cl, MGA_MA_MCWS_CASLatency_MASK;0x1
    shr    dl, 01h
    mov    ch, dl
    and    ch, MGA_MA_MCWS_RAS2CASDelay_MASK SHR 8;0x1
    call   IndirectRegisterWriteDWord   ;Offset 0x5958
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT (PCI_MGA_Opt_MemCfg_MASK SHR 8);0xef
    or     cl, PCI_MGA_Opt_FrameBufMask8MB SHR 8;0xe
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, 01h
    call   Sleep2                       ;Offset 0x31f0
    mov    si, MGA_MemAddr_MemoryAccess + 01h;0x1c05
    mov    cl, MGA_MA_MA_MemoryActive SHR 8;0x0
    call   IndirectRegisterWriteByte    ;Offset 0x591c
    mov    cl, MGA_MA_MA_MemoryReset SHR 8;0x80
    call   IndirectRegisterWriteByte    ;Offset 0x591c
    mov    al, 01h
    call   Sleep2                       ;Offset 0x31f0
    mov    cl, (MGA_MA_MA_MemSeq8RefresRegRest OR MGA_MA_MA_MemoryActive) SHR 8;0x40
    call   IndirectRegisterWriteByte    ;Offset 0x591c
    mov    cl, MGA_MA_MA_MemoryReset SHR 8;0x80
    call   IndirectRegisterWriteByte    ;Offset 0x591c
    push   bx
    xor    ax, ax
    call   FetchDefaultSystemClockSpeed ;Offset 0x5bd9
    mov    ax, dx
    shr    ax, 01h
    cmp    al, 0ffh
    jne    Label0x5df8                  ;Offset 0x5df8
    mov    al, 42h
Label0x5df8:                            ;Offset 0x5df8
    xor    dx, dx
    mov    bx, 03e8h
    mul    bx
    mov    bx, 0040h
    div    bx
    inc    ax
    mov    bx, 0010h
Label0x5e08:                            ;Offset 0x5e08
    dec    bx
    mov    dx, bx
    and    dx, 0eh
    shl    dx, 07h
    mov    cx, bx
    and    cx, 01h
    shl    cx, 06h
    add    dx, cx
    inc    dx
    cmp    ax, dx
    jl     Label0x5e08                  ;Offset 0x5e08
    mov    dl, bl
    pop    bx
    mov    di, PCI_MGA_Option + 02h     ;0x42
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT (PCI_MGA_Opt_RefreshCounter_MASK SHR 16);0xf0
    or     cl, dl
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    or     cl, PCA_MGA_Opt_HWPlaneWriteMaskOn SHR 8;0x40
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    pop    di
    pop    dx
    pop    cx
    pop    bx
    pop    ax
    ret
SetMemoryTimingsAndRefresh ENDP

;
;Calculate the M, N, P and S values for the PLL's given an input frequency in KHz
;
;inputs:
;   eax = requested frequency in KHz
;
;outputs:
;   ebx = M,N,P and S values
;         bl holds N in lower 7 bits
;         bh holds M in lower 5 bits
;         bh holds P in upper 3 bits
;         S 2 bits sit above bh
;         00000000 000000SS PPPMMMMM xNNNNNNN
;
CalculatePLL_MNPS PROC NEAR             ;Offset 0x5e4b
    push   eax
    push   ecx
    push   edx
    push   esi
    push   edi
    cmp    eax, 00001876h               ;6,262
    jge    Label0x5e63                  ;Offset 0x5e63
    mov    eax, 00001876h               ;6,262
Label0x5e63:                            ;Offset 0x5e63
    cmp    eax, 000356f0h               ;218,864
    jle    Label0x5e71                  ;Offset 0x5e71
    mov    eax, 000356f0h               ;218,864
Label0x5e71:                            ;Offset 0x5e71
    mov    esi, eax
    mov    edi, 000356f0h               ;218,864
    mov    edx, 00035b60h               ;220,000
    mov    cl, 64h                      ;100
    xor    ebx, ebx
Label0x5e85:                            ;Offset 0x5e85
    cmp    cl, 80h                      ;128
    je     Label0x5f86                  ;Offset 0x5f86
    mov    ch, 00h
Label0x5e8e:                            ;Offset 0x5e8e
    cmp    ch, 20h                      ;32
    je     Label0x5f81                  ;Offset 0x5f81
    push   ebx
    push   edx
    xor    edx, edx
    mov    eax, 000037eeh               ;14,318
    xor    ebx, ebx
    mov    bl, cl
    inc    ebx
    mul    ebx
    xor    ebx, ebx
    mov    bl, ch
    inc    ebx
    div    ebx
    pop    edx
    pop    ebx
    cmp    eax, 0000c350h               ;50,000
    jl     Label0x5f7c                  ;Offset 0x5f7c
    cmp    eax, 00035b60h               ;220,000
    jg     Label0x5f7c                  ;Offset 0x5f7c
    shl    ecx, 10h
    mov    cl, 00h
Label0x5ed4:                            ;Offset 0x5ed4
    cmp    cl, 04h
    jne    Label0x5ee0                  ;Offset 0x5ee0
    shr    ecx, 10h
    jmp    Label0x5f7c                  ;Offset 0x5f7c
Label0x5ee0:                            ;Offset 0x5ee0
    push   eax
    push   ebx
    push   edx
    xor    edx, edx
    mov    ebx, 00000001h
    shl    ebx, cl
    div    ebx
    mov    edx, esi
    cmp    eax, edx
    jge    Label0x5eff                  ;Offset 0x5eff
    xchg   eax, edx
Label0x5eff:                            ;Offset 0x5eff
    sub    eax, edx
    pop    edx
    pop    ebx
    cmp    eax, edi
    jg     Label0x5f75                  ;Offset 0x5f75
    cmp    eax, edi
    mov    edi, eax
    jne    Label0x5f1c                  ;Offset 0x5f1c
    pop    eax
    push   eax
    cmp    eax, edx
    jge    Label0x5f75                  ;Offset 0x5f75
Label0x5f1c:                            ;Offset 0x5f1c
    pop    eax
    push   eax
    mov    edx, eax
    xor    ebx, ebx
    mov    eax, ecx
    shr    eax, 10h
    mov    bx, ax
    and    bx, 1fffh
    mov    ax, 0001h
    shl    ax, cl
    dec    ax
    shl    ax, 0dh
    or     bx, ax
    xor    eax, eax
    cmp    edx, 000186a0h               ;100,000
    jg     Label0x5f4f                  ;Offset 0x5f4f
    mov    ax, 0000h
    jmp    Label0x5f6e                  ;Offset 0x5f6e
Label0x5f4f:                            ;Offset 0x5f4f
    cmp    edx, 000222e0h               ;140,000
    jg     Label0x5f5d                  ;Offset 0x5f5d
    mov    ax, 0001h
    jmp    Label0x5f6e                  ;Offset 0x5f6e
Label0x5f5d:                            ;Offset 0x5f5d
    cmp    edx, 0002bf20h               ;180,000
    jg     Label0x5f6b                  ;Offset 0x5f6b
    mov    ax, 0002h
    jmp    Label0x5f6e                  ;Offset 0x5f6e
Label0x5f6b:                            ;Offset 0x5f6b
    mov    ax, 0003h
Label0x5f6e:                            ;Offset 0x5f6e
    shl    eax, 10h
    or     ebx, eax
Label0x5f75:                            ;Offset 0x5f75
    pop    eax
    inc    cl
    jmp    Label0x5ed4                  ;Offset 0x5ed4
Label0x5f7c:                            ;Offset 0x5f7c
    inc    ch
    jmp    Label0x5e8e                  ;Offset 0x5e8e
Label0x5f81:                            ;Offset 0x5f81
    inc    cl
    jmp    Label0x5e85                  ;Offset 0x5e85
Label0x5f86:                            ;Offset 0x5f86
    pop    edi
    pop    esi
    pop    edx
    pop    ecx
    pop    eax
    ret
CalculatePLL_MNPS ENDP

;
;inputs:
;  edx = Requested frequency in Hz
;   cx = bits 1-0 = sysclksl
;        bit    2 = gclkdiv
;        bit    3 = mclkdiv
;        bit    5 = nogclkdiv
;        bit    6 = nomclkdiv
ConfigureSystemClock PROC NEAR          ;Offset 0x5f91
    push   eax
    push   ebx
    push   ecx
    push   edx
    push   esi
    push   cx
    and    cl, 01h                      ;bit 0
    jne    NotPLL                       ;Offset 0x5fde
    call   FindMystique                 ;Offset 0x57c6
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, dl
    and    al, PCI_MGA_Opt_SysClockSelect_MASK;bits 1,0
    jne    Label0x5fcb                  ;Offset 0x5fcb
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    jmp    Label0x6063                  ;Offset 0x6063
Label0x5fcb:                            ;Offset 0x5fcb
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectMCLK;0x2
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    jmp    Label0x6063                  ;Offset 0x6063
NotPLL:                                 ;Offset 0x5fde
    mov    eax, edx
    call   CalculatePLL_MNPS            ;Offset 0x5e4b
    push   ebx
    call   FindMystique                 ;Offset 0x57c6
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT PCI_MGA_Opt_SysClock_MASK;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    pop    edx
    mov    cl, MGA_INDD_SYSPLL_M_Value  ;0x2c
    mov    ch, dh
    and    ch, MGA_SYSPLLM_Mask         ;0x1f
    call   WriteIndexedRegister         ;Offset 0x5b90
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x5b90
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x5b90
    mov    al, 01h
    call   Sleep2                       ;Offset 0x31f0
    xor    dx, dx
WaitForFrequencyLock:                   ;Offset 0x6038
    dec    dx
    je     TimeOut                      ;Offset 0x6045
    mov    cl, MGA_INDD_SYSPLL_Status   ;0x2f
    call   ReadIndexedRegister          ;Offset 0x5bab
    and    cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je     WaitForFrequencyLock         ;Offset 0x6038
TimeOut:                                ;Offset 0x6045
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
Label0x6063:                            ;Offset 0x6063
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    mov    al, dl
    and    al, 0ch                      ;gclkdiv & mclkdiv
    shl    al, 01h                      ;...
    or     cl, al
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5734
    and    cl, NOT PCI_MGA_Opt_SysClock_MASK;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5734
    pop    esi
    pop    edx
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigureSystemClock ENDP

;
;inputs:
;   ebx = M,N,P and S values
;         bl holds N in lower 7 bits
;         bh holds M in lower 5 bits
;         bh holds P in upper 3 bits
;         S 2 bits sit above bh
;         00000000 000000SS PPPMMMMM xNNNNNNN
;    cl = bits 1-0 = PLL Set A = 0, B = 1, C = 2
;         bits 7-2 = Clock select: 0 = PCI, 1 = PLL, 2 = VDOCLK
;
ConfigurePixelClocks PROC NEAR          ;Offset 0x6090
    push   eax
    push   ebx
    push   ecx
    push   edx
    mov    edx, ebx
    push   edx
    push   cx
    call   FindMystique                 ;Offset 0x57c6
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    out    dx, al
    inc    dx
    in     al, dx
    or     al, VGA_SEQ1_ScreenOff       ;0x20
    out    dx, al
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x5bab
    or     cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5b90
    pop    cx
    mov    al, cl
    and    al, 03h
    cmp    al, 00h
    jne    Label0x60c8                  ;Offset 0x60c8
    mov    al, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jmp    Label0x60d2                  ;Offset 0x60d2
Label0x60c8:                            ;Offset 0x60c8
    cmp    al, 01h
    jne    Label0x60d0                  ;Offset 0x60d0
    mov    al, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jmp    Label0x60d2                  ;Offset 0x60d2
Label0x60d0:                            ;Offset 0x60d0
    mov    al, MGA_INDD_PIXPLL_M_ValueSetC;0x4c
Label0x60d2:                            ;Offset 0x60d2
    mov    ah, cl
    shr    ah, 02h
    cmp    ah, 01h
    jne    Label0x60e0                  ;Offset 0x60e0
    mov    ah, MGA_PIXCLKCTRL_SelPLL    ;0x01
    jmp    Label0x60eb                  ;Offset 0x60eb
Label0x60e0:                            ;Offset 0x60e0
    cmp    ah, 00h
    jne    Label0x60e9                  ;Offset 0x60e9
    mov    ah, MGA_PIXCLKCTRL_SelPCI    ;0x0
    jmp    Label0x60eb                  ;Offset 0x60eb
Label0x60e9:                            ;Offset 0x60e9
    mov    ah, MGA_PIXCLKCTRL_SelVDOCLK ;0x2
Label0x60eb:                            ;Offset 0x60eb
    pop    edx
    push   ax
    mov    ch, dh
    and    ch, MGA_PIXPLL_M_MASK        ;0x1f
    mov    cl, al
    call   WriteIndexedRegister         ;Offset 0x5b90
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x5b90
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x5b90
    mov    al, 01h
    cli
    call   Sleep2                       ;Offset 0x31f0
    sti
    xor    dx, dx
Label0x6113:                            ;Offset 0x6113
    dec    dx
    je     Label0x6120                  ;Offset 0x6120
    mov    cl, MGA_INDD_PIXPLL_Status   ;0x4f
    call   ReadIndexedRegister          ;Offset 0x5bab
    and    cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je     Label0x6113                  ;Offset 0x6113
Label0x6120:                            ;Offset 0x6120
    pop    dx
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x5bab
    and    cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or     cl, dh
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5b90
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x5bab
    and    cl, NOT MGA_PIXCLKCTRL_ClockMASK;0xfb
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5b90
    cmp    dh, MGA_PIXCLKCTRL_SelPCI    ;0x0
    jne    Label0x614a                  ;Offset 0x614a
    mov    bl, VGA_MISC_Clock25p175MHz SHR 2;0x0
    jmp    Label0x6155                  ;Offset 0x6155
Label0x614a:                            ;Offset 0x614a
    cmp    dh, MGA_PIXCLKCTRL_SelPLL    ;0x1
    jne    Label0x6153                  ;Offset 0x6153
    mov    bl, VGA_MISC_Clock28p322MHz SHR 2;0x1
    jmp    Label0x6155                  ;Offset 0x6155
Label0x6153:                            ;Offset 0x6153
    mov    bl, MGA_MISC_MGAPixelClock SHR 2;0x3
Label0x6155:                            ;Offset 0x6155
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    shl    bl, 02h
    and    al, NOT VGA_MISC_ClockSelectMask;0xf3
    or     al, bl
    mov    dx, VGA_MiscellaneousWrite   ;Port 0x3c2
    out    dx, al
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    out    dx, al
    inc    dx
    in     al, dx
    and    al, NOT VGA_SEQ1_ScreenOff   ;0xdf
    out    dx, al
    pop    edx
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigurePixelClocks ENDP

;inputs:
;   edx = requested frequency in KHz
;
;outputs:
;   -
;
;destroys:
;   -
;
ConfigureAndSelectPLLSetC PROC NEAR     ;Offset 0x6178
    push eax
    push ebx
    push ecx
    mov  cl, MGA_CPC_PLLSetC OR MGA_CPC_ClockPLL;0x6
    mov  eax, edx
    call CalculatePLL_MNPS              ;Offset 0x5e4b
    call ConfigurePixelClocks           ;Offset 0x6090
    pop  ecx
    pop  ebx
    pop  eax
    ret
ConfigureAndSelectPLLSetC ENDP

ConfigureAndSelectPLLSetB PROC NEAR     ;Offset 0x6190
    push eax
    push ebx
    push ecx
    mov  cl, MGA_CPC_PLLSetB OR MGA_CPC_ClockPLL;0x5
    mov  ebx, 00016c66h                 ;93,286
    call ConfigurePixelClocks           ;Offset 0x6090
    pop  ecx
    pop  ebx
    pop  eax
    ret
ConfigureAndSelectPLLSetB ENDP

;Offset 0x61a8
IndexedRegisters        DB MGA_INDD_CursorBaseAddressLow            ;0x04
                        DB MGA_INDD_CursorBaseAddressHigh           ;0x05
                        DB MGA_INDD_CursorControl                   ;0x06
                        DB MGA_INDD_CursorColor0Red                 ;0x08
                        DB MGA_INDD_CursorColor0Green               ;0x09
                        DB MGA_INDD_CursorColor0Blue                ;0x0A
                        DB MGA_INDD_CursorColor1Red                 ;0x0C
                        DB MGA_INDD_CursorColor1Green               ;0x0D
                        DB MGA_INDD_CursorColor1Blue                ;0x0E
                        DB MGA_INDD_CursorColor2Red                 ;0x10
                        DB MGA_INDD_CursorColor2Green               ;0x11
                        DB MGA_INDD_CursorColor2Blue                ;0x12
                        DB MGA_INDD_MultiplexControl                ;0x19
                        DB MGA_INDD_PixelClockControl               ;0x1A
                        DB MGA_INDD_GeneralControl                  ;0x1D
                        DB MGA_INDD_MiscellaneousControl            ;0x1E
                        DB MGA_INDD_GeneralPurposeIOControl         ;0x2A
                        DB MGA_INDD_GeneralPurposeIOData            ;0x2B
                        DB MGA_INDD_SYSPLL_M_Value                  ;0x2C
                        DB MGA_INDD_SYSPLL_N_Value                  ;0x2D
                        DB MGA_INDD_SYSPLL_P_Value                  ;0x2E
                        DB MGA_INDD_SYSPLL_Status                   ;0x2F
                        DB MGA_INDD_ZoomControl                     ;0x38
                        DB MGA_INDD_ColorKeyMask                    ;0x40
                        DB MGA_INDD_Reserved41                      ;0x41
                        DB MGA_INDD_ColorKey                        ;0x42
                        DB MGA_INDD_Reserved43                      ;0x43
                        DB MGA_INDD_PIXPLL_M_ValueSetA              ;0x44
                        DB MGA_INDD_PIXPLL_N_ValueSetA              ;0x45
                        DB MGA_INDD_PIXPLL_P_ValueSetA              ;0x46
                        DB MGA_INDD_PIXPLL_M_ValueSetB              ;0x48
                        DB MGA_INDD_PIXPLL_N_ValueSetB              ;0x49
                        DB MGA_INDD_PIXPLL_P_ValueSetB              ;0x4A
                        DB MGA_INDD_PIXPLL_M_ValueSetC              ;0x4C
                        DB MGA_INDD_PIXPLL_N_ValueSetC              ;0x4D
                        DB MGA_INDD_PIXPLL_P_ValueSetC              ;0x4E
                        DB MGA_INDD_PIXPLL_Status                   ;0x4F
                        DB 080h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

Func0x61d7 PROC NEAR                    ;Offset 0x61d7
    push dx
    push cx
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, MGA_MULCTRL_DepthMask      ;0x7
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    xor  al, al
    cmp  cl, MGA_MULCTRL_Depth32Pal     ;0x7
    je   Label0x61f2                    ;Offset 0x61f2
    cmp  cl, MGA_MULCTRL_Depth16        ;0x2
    je   Label0x61fe                    ;Offset 0x61fe
    jmp  Label0x621b                    ;Offset 0x621b
Label0x61f2:                            ;Offset 0x61f2
    out  dx, al
    inc  dx
    out  dx, al
    out  dx, al
    out  dx, al
    dec  dx
    inc  al
    jne  Label0x61f2                    ;Offset 0x61f2
    jmp  Label0x622c                    ;Offset 0x622c
Label0x61fe:                            ;Offset 0x61fe
    out  dx, al
    inc  dx
    mov  cl, al
    shl  al, 03h
    out  dx, al
    mov  al, cl
    shl  al, 02h
    out  dx, al
    mov  al, cl
    shl  al, 03h
    out  dx, al
    dec  dx
    mov  al, cl
    inc  al
    jne  Label0x61fe                    ;Offset 0x61fe
    jmp  Label0x622c                    ;Offset 0x622c
Label0x621b:                            ;Offset 0x621b
    out  dx, al
    inc  dx
    mov  cl, al
    shl  al, 03h
    out  dx, al
    out  dx, al
    out  dx, al
    dec  dx
    mov  al, cl
    inc  al
    jne  Label0x621b                    ;Offset 0x621b
Label0x622c:                            ;Offset 0x622c
    pop  cx
    pop  dx
    ret
Func0x61d7 ENDP

Func0x622f PROC NEAR                    ;Offset 0x622f
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  ax, ax                         ;MGA_CRTCExt_AddrGeneratorExt
    out  dx, al
    inc  dx
    in   al, dx
    shl  ax, 04h
    mov  dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov  al, VGA_CRTCIdx_Offset         ;0x13
    out  dx, al
    inc  dx
    in   al, dx
    or   cl, cl
    jne  Label0x624a                    ;Offset 0x624a
    shr  ax, 01h
    jmp  Label0x624c                    ;Offset 0x624c
Label0x624a:                            ;Offset 0x624a
    shl  ax, cl
Label0x624c:                            ;Offset 0x624c
    out  dx, al
    mov  dl, MGA_CRTCExtensionData_lowbyte;Port 0x3df
    mov  al, ah
    shl  al, 04h
    out  dx, al
    pop  dx
    ret
Func0x622f ENDP

Func0x6257 PROC NEAR                    ;Offset 0x6257
    push bx
    push cx
    push dx
    push si
    push di
    call IsExtensionReg7Writeable       ;Offset 0x5606
    ;je   Label0x62db                    ;Offset 0x62db
    DB 00Fh, 084h, 078h, 000h
    push es
    push ds
    push cs
    pop  ds
    call FindMystique                   ;Offset 0x57c6
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_Bit_MASK);0xf1
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    xor  ch, ch                         ;MGA_MULCTRL_Depth8
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENCTRL_PedestalControl_MASK;0xef
    mov  al, byte ptr cs:[Data0x7814]   ;Offset 0x7814
    and  al, 01h
    shl  al, 04h                        ;Pedestal Control
    or   cl, al
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_GENCTRL_GreenSyncOut_MASK;0xdf
    mov  al, byte ptr cs:[Data0x7814]   ;Offset 0x7814
    and  al, MGA_GENCTRL_GreenSyncOut_MASK;0x20
    or   cl, al
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, MGA_GENCTRL_PedestalControl_MASK OR MGA_GENCTRL_GreenSyncOut_MASK;0x30
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_CursorControl     ;0x6
    mov  ch, MGA_CURCTRL_CursorDisabled ;0x0
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_PIXCLKCTRL_SelMASK ;0xfc
    or   cl, MGA_PIXCLKCTRL_SelPLL      ;0x1
    mov  ch, cl
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  ds
    pop  es
Label0x62db:                            ;Offset 0x62db
    mov  ax, MGA_CRTCExt_AddrGeneratorExt SHL 8;0x0
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    mov  ax, (MGA_CRTCExt_HorCounterExt SHL 8) OR MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x130
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    mov  ax, MGA_CRTCExt_VertCounterExt SHL 8;0x200
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, byte ptr cs:[Data0x7814]   ;Offset 0x7814
    and  ah, MGA_CRTCEXT3_CompSyncEnable SHR 2;10h
    shl  ah, 02h                        ;MGA_CRTCEXT3_CompSyncEnable
    mov  al, MGA_CRTCExt_Misc           ;0x3
    out  dx, ax
    mov  ax, MGA_CRTCExt_MemoryPage SHL 8;0x400
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    mov  ax, MGA_CRTCExt_HorVidHalfCount SHL 8;0x500
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    pop  di
    pop  si
    pop  dx
    pop  cx
    pop  bx
    ret
Func0x6257 ENDP

Func0x6310 PROC NEAR                    ;Offset 0x6310
    push ds
    push es
    push si
    push cx
    push bx
    push dx
    xor  ax, ax
    mov  es, ax
    call GetMemoryIn64KBlocks           ;Offset 0x5846
    call CapAXTo0x40                    ;Offset 0x5832
    mov  byte ptr es:[BDA_VideoBufferSize + 01h], ah;Offset 0x44d
    mov  byte ptr es:[BDA_VideoBufferSize], 00h;Offset 0x44c
    mov  al, 03h
    and  al, byte ptr [si + 0bh]
    jne  Label0x6400                    ;Offset 0x6400
    call FindMystique                   ;Offset 0x57c6
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    mov  di, PCI_MGA_Option + 01h       ;0x41
    call AccessPCIRegister              ;Offset 0x5734
    and  cl, NOT (PCI_MGA_Opt_SplitModeEnable SHR 8);0xdf
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    call AccessPCIRegister              ;Offset 0x5734
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_MemoryPage     ;0x4
    out  dx, ax
    test byte ptr [si + 0bh], 10h
    je   Label0x63b8                    ;Offset 0x63b8
    test byte ptr [si + 0bh], 20h
    je   Label0x6385                    ;Offset 0x6385
    mov  byte ptr es:[BDA_VideoBufferSize], 03h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    mov  ch, MGA_MULCTRL_Depth32Pal     ;0x7
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv4) SHL 8) OR MGA_CRTCExt_Misc;0x8303
    out  dx, ax
    call Func0x61d7                     ;Offset 0x61d7
    mov  cx, 0002h
    jmp  Label0x63de                    ;Offset 0x63de
Label0x6385:                            ;Offset 0x6385
    mov  byte ptr es:[BDA_VideoBufferSize], 02h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    mov  ch, 02h
    test byte ptr [si + 0bh], 40h
    je   Label0x63a6                    ;Offset 0x63a6
    mov  ch, 01h
Label0x63a6:                            ;Offset 0x63a6
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv2) SHL 8) OR MGA_CRTCExt_Misc;0x8103
    out  dx, ax
    call Func0x61d7                     ;Offset 0x61d7
    mov  cx, 0000h
    jmp  Label0x63de                    ;Offset 0x63de
Label0x63b8:                            ;Offset 0x63b8
    mov  byte ptr es:[BDA_VideoBufferSize], 01h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_MISCCTRL_DAC_8Bit  ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    xor  ch, ch
    call WriteIndexedRegister           ;Offset 0x5b90
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv1) SHL 8) OR MGA_CRTCExt_Misc;0x8003
    out  dx, ax
    mov  cx, 0001h
Label0x63de:                            ;Offset 0x63de
    push cx
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    or   cl, MGA_MISCCTRL_MAFC_Disable  ;0x6
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    pop  cx
    mov  ax, (MGA_CRTCExt_HorVidHalfCount SHL 8) OR 01h;0x501
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    or   cl, cl
    je   Label0x640f                    ;Offset 0x640f
    dec  cl
    call Func0x622f                     ;Offset 0x622f
    jmp  Label0x640f                    ;Offset 0x640f
Label0x6400:                            ;Offset 0x6400
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    and  cl, NOT MGA_MISCCTRL_MAFC_MASK;0xf9
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
Label0x640f:                            ;Offset 0x640f
    pop  dx
    pop  bx
    pop  cx
    pop  si
    pop  es
    pop  ds
    ret
Func0x6310 ENDP

;lol
Func0x6416 PROC NEAR                    ;Offset 0x6416
    push ax
    xor  ax, ax
    pop  ax
    ret
Func0x6416 ENDP

SaveMGAState PROC NEAR                  ;Offset 0x641b
    push ds
    push si
    push di
    push dx
    push cx
    push ax
    mov  ax, cs
    mov  ds, ax
    mov  di, bx
    call FindMystique                   ;Offset 0x57c6
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x642c
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x61a8
    cmp  cl, 80h
    je   EndOfIndexedRegisterData       ;Offset 0x6440
    call ReadIndexedRegister            ;Offset 0x5bab
    mov  byte ptr es:[di], cl
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x642c
EndOfIndexedRegisterData:               ;Offset 0x6440
    mov  byte ptr es:[di], 80h
    inc  di
    push di
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    mov  di, PCI_MGA_Option             ;0x40
    call AccessPCIRegister              ;Offset 0x5734
    pop  di
    ;mov  dword ptr es:[di], ecx
    DB 026h, 066h, 089h, 00Dh
    add  di, 0004h
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x645c
    mov  al, cl
    out  dx, al
    inc  dx
    in   al, dx
    dec  dx
    mov  byte ptr es:[di], al
    inc  di
    inc  cl
    cmp  cl, 06h
    jl   LoopExtensionRegisters         ;Offset 0x645c
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  di
    pop  si
    pop  ds
    ret
SaveMGAState ENDP

RestoreMGAState PROC NEAR               ;Offset 0x6476
    push ds
    push di
    push si
    push dx
    push cx
    push ax
    mov  ax, cs
    mov  ds, ax
    mov  di, bx
    call FindMystique                   ;Offset 0x57c6
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x6487
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x61a8
    cmp  cl, 80h
    je   EndOfIndexedRegisterData       ;Offset 0x649b
    mov  ch, byte ptr es:[di]
    call WriteIndexedRegister           ;Offset 0x5b90
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x6487
EndOfIndexedRegisterData:               ;Offset 0x649b
    inc  di
    push di
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    ;mov  ecx, dword ptr es:[di]
    DB 026h, 066h, 08Bh, 00Dh
    mov  di, PCI_MGA_Option             ;0x40
    call AccessPCIRegister              ;Offset 0x5734
    pop  di
    add  di, 0004h
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x64b3
    mov  al, cl
    out  dx, al
    inc  dx
    mov  al, byte ptr es:[di]
    out  dx, al
    dec  dx
    inc  di
    inc  cl
    cmp  cl, 06h
    jl   LoopExtensionRegisters         ;Offset 0x64b3
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  si
    pop  di
    pop  ds
    ret
RestoreMGAState ENDP

Func0x64cd PROC NEAR                    ;Offset 0x64cd
    cmp  al, 20h                        ;Under VESA 0x100 640x400x256
    jb   Label0x64e5                    ;Offset 0x64e5
    cmp  al, 22h                        ;VESA 0x102 VESA_MODE_800x600x16
    je   Label0x64df                    ;Offset 0x64df
    cmp  al, 28h                        ;Under VESA 0x108 VESA_MODE_80x60xText
    jb   Label0x64e3                    ;Offset 0x64e3
    cmp  al, 2dh                        ;Under VESA 0x10d (320x200x32K)
    mov  al, 03h                        ;VGA 80x25 16 color text
    jb   Label0x64e5                    ;Offset 0x64e5
Label0x64df:                            ;Offset 0x64df
    mov  al, 12h                        ;640x480   16 color
    jmp  Label0x64e5                    ;Offset 0x64e5
Label0x64e3:                            ;Offset 0x64e3
    mov  al, 13h                        ;320x200 256 color
Label0x64e5:                            ;Offset 0x64e5
    ret
Func0x64cd ENDP

Func0x64e6 PROC NEAR                    ;Offset 0x64e6
    push      es
    push      di
    push      dx
    push      ecx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    call      Func0x59c0                ;Offset 0x59c0
    mov       al, 04h
Label0x64f8:                            ;Offset 0x64f8
    dec       ah
    jb        Label0x6512               ;Offset 0x6512
    out       dx, ax
    push      ax
    xor       eax, eax
    mov       di, 0000h
    mov       ecx, 00004000h
    rep stosd
    pop       ax
    or        ah, ah
    jne       Label0x64f8               ;Offset 0x64f8
Label0x6512:                            ;Offset 0x6512
    pop       ecx
    pop       dx
    pop       di
    pop       es
    ret
Func0x64e6 ENDP

Func0x6518 PROC NEAR                    ;Offset 0x6518
    push      bp
    mov       bp, sp
    push      es
    push      ebx
    sub       sp, 002ch
    push      ss
    pop       es
    mov       di, sp
    mov       bx, ax
    shl       ebx, 01h
    xor       dx, dx
    mov       ds, dx
    lds       si, ds:[BDA_VideoParameterControlBlockPtrOfs];Offset 0x4a8
    push      ds
    push      si
    push      di
    mov       cx, 000eh
    rep movsw
    pop       di
    lds       si, [bp + 04h]
    push      ax
    push      dx
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, al
    inc       dx
    in        al, dx
    or        al, MGA_CRTCEXT1_VerSyncOff OR MGA_CRTCEXT1_HorSyncOff;0x30
    out       dx, al
    pop       dx
    pop       ax
    mov       dx, VGA_MiscellaneousWrite
    mov       al, NOT VGA_MISC_VideoDisable;0xef
    out       dx, al
    mov       edx, dword ptr [si]
    mov       cl, 2dh
    call      ConfigureAndSelectPLLSetC ;Offset 0x6178
    xor       dx, dx
    xor       eax, eax
    mov       ax, word ptr [si + 0ch]
    mov       bx, ds
    mov       cx, bx
    or        cx, ax
    je        Label0x6589               ;Offset 0x6589
    push      ax
    mov       cx, 0004h
    shr       ax, cl
    add       bx, ax
    mov       al, byte ptr [si + 12h]
    xor       ah, ah
    shl       ax, 01h
    shl       ax, 01h
    sub       bx, ax
    pop       ax
    ;and       ax, 000fh
    DB 025h, 00Fh, 000h
    mov       word ptr es:[di], ax
    mov       word ptr es:[di + 02h], bx
Label0x6589:                            ;Offset 0x6589
    mov       word ptr es:[di + 08h], dx
    mov       word ptr es:[di + 0ah], dx
    shr       ebx, 01h
    and       bh, 80h
    mov       dl, byte ptr [si + 13h]
    or        dl, bh
    shl       ebx, 01h
    xor       ax, ax
    mov       ds, ax
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], di;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], es;Offset 0x4aa
    mov       ax, dx
    int       6dh
    call      TurnScreenOff             ;Offset 0x3109
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs];Offset 0x4a8
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg];Offset 0x4aa
    lds       si, [bp + 04h]
    test      byte ptr [si + 0bh], 80h
    je        Label0x65cf               ;Offset 0x65cf
    push      si
    push      ds
    mov       al, 02h
    mov       bx, 0000h
    call      TextFunctions             ;Offset 0x25e5
    pop       ds
    pop       si
Label0x65cf:                            ;Offset 0x65cf
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       ah, byte ptr [si + 10h]
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    mov       ah, byte ptr [si + 11h]
    mov       al, MGA_CRTCExt_VertCounterExt;0x2
    out       dx, ax
    call      Func0x6310                ;Offset 0x6310
    shr       ebx, 01h
    and       bh, 80h
    jne       Label0x65f2               ;Offset 0x65f2
    test      byte ptr [si + 0bh], 02h
    jne       Label0x65f2               ;Offset 0x65f2
    call      Func0x64e6                ;Offset 0x64e6
Label0x65f2:                            ;Offset 0x65f2
    xor       eax, eax
    call      SetStartAddress           ;Offset 0x5a28
    call      TurnScreenOn              ;Offset 0x3105
    add       sp, 002ch
    xor       ax, ax
    pop       ebx
    pop       es
    pop       bp
    ret
Func0x6518 ENDP

;Offset 0x6605
VESACallTable           DW offset GetSuperVGAInformation;Offset 0x712b          ;0
                        DW offset GetSuperVGAModeInformation;Offset 0x7209      ;1
                        DW offset SetSuperVGAVideoMode;Offset 0x7356            ;2
                        DW offset SuperVGAGetCurrentVideoMode;Offset 0x73ec     ;3
                        DW offset SaveRestoreSuperVGAVideoState;Offset 0x7414   ;4
                        DW offset CPUVideoMemoryControl;Offset 0x747f           ;5
                        DW offset LogicalScanLineLengthHandling;Offset 0x7488   ;6
                        DW offset DisplayStartHandling;Offset 0x74ff            ;7
                        DW offset GetSetDACPaletteControl;Offset 0x7599         ;8
                        DW offset GetSetPaletteEntries;Offset 0x75dc            ;9
                        DW offset GetProtectedModeInterface;Offset 0x7644       ;a

;Offset 0x661b
VESAResponseTemplate    DB 'VESA'       ;00h  4 BYTEs   (ret) signature ("VESA")
                        DW 0200h        ;04h    WORD    VESA version number (one-digit minor version -- 0200h = v2.0)
                        DD 00000000h    ;06h    DWORD   pointer to OEM name
                        DD 00000001h    ;0Ah    DWORD   capabilities flags
                        DD 00000000h    ;0Eh    DWORD   pointer to list of supported VESA and OEM video modes
                        DW 0020h        ;12h    WORD    total amount of video memory in 64K blocks
                        DW 0101h        ;14h    WORD    OEM software version (BCD, high byte = major, low byte = minor)
                        DD 00000000h    ;16h    DWORD   pointer to vendor name
                        DD 00000000h    ;1Ah    DWORD   pointer to product name
                        DD 00000000h    ;1Eh    DWORD   pointer to product revision string

;Offset 0x663d
VBEGfxModeInfoTemplate  DW VESA_MODE_ATTR_ModeGraphics OR VESA_MODE_ATTR_Color OR VESA_MODE_ATTR_BIOSOutputNotSupported OR VESA_MODE_ATTR_OptionalInfoAvailable OR VESA_MODE_ATTR_Supported;0x1B
                        DB VESA_WND_ATTR_Writeable OR VESA_WND_ATTR_Readable OR VESA_WND_ATTR_Exists;0x7
                        DB VESA_WND_ATTR_NotWriteable OR VESA_WND_ATTR_NotReadable OR VESA_WND_ATTR_DoesntExist;0x0
                        DW 00040h       ;VESA_MODE_INFO_WindowGranularityInKB
                        DW 00040h       ;VESA_MODE_INFO_WindowSizeInKB
                        DW 0A000h       ;VESA_MODE_INFO_StartSegmentWindowA
                        DW 00000h       ;VESA_MODE_INFO_StartSegmentWindowB
                        DD 000000000h   ;VESA_MODE_INFO_FARWindowPositioningFunc

;Offset 0x664d
VBETextModeInfoTemplate DW VESA_MODE_ATTR_ModeText OR VESA_MODE_ATTR_Color OR VESA_MODE_ATTR_BIOSOutputSupported OR VESA_MODE_ATTR_OptionalInfoAvailable OR VESA_MODE_ATTR_Supported;0xF
                        DB VESA_WND_ATTR_Writeable OR VESA_WND_ATTR_Readable OR VESA_WND_ATTR_Exists;0x7
                        DB VESA_WND_ATTR_NotWriteable OR VESA_WND_ATTR_NotReadable OR VESA_WND_ATTR_DoesntExist;0x0
                        DW 00020h       ;VESA_MODE_INFO_WindowGranularityInKB
                        DW 00020h       ;VESA_MODE_INFO_WindowSizeInKB
                        DW 0B800h       ;VESA_MODE_INFO_StartSegmentWindowA
                        DW 0B800h       ;VESA_MODE_INFO_StartSegmentWindowB
                        DD 000000000h   ;VESA_MODE_INFO_FARWindowPositioningFunc

;Offset 0x665d
SupportedVESAVideoModes DW VESA_MODE_640x400x256    ;0x100      1MB
                        DW VESA_MODE_640x480x256    ;0x101
                        DW VESA_MODE_800x600x16     ;0x102
                        DW VESA_MODE_800x600x256    ;0x103
                        DW VESA_MODE_1024x768x256   ;0x105
                        DW VESA_MODE_1280x1024x256  ;0x107
                        DW VESA_MODE_80x60xText     ;0x108
                        DW VESA_MODE_132x25xText    ;0x109
                        DW VESA_MODE_132x43xText    ;0x10a
                        DW VESA_MODE_132x50xText    ;0x10b
                        DW VESA_MODE_132x60xText    ;0x10c
                        DW 0FFFFh

;Offset 0x6675
                        DW VESA_MODE_640x480x32K    ;0x110      2MB
                        DW VESA_MODE_640x480x64K    ;0x111
                        DW VESA_MODE_640x480x16M    ;0x112
                        DW VESA_MODE_800x600x32K    ;0x113
                        DW VESA_MODE_800x600x64K    ;0x114
                        DW VESA_MODE_800x600x16M    ;0x115
                        DW VESA_MODE_1024x768x32K   ;0x116
                        DW VESA_MODE_1024x768x64K   ;0x117
                        DW VESA_MODE_1600x1200x256  ;0x11c
                        DW 0FFFFh

;Offset 0x6689
                        DW VESA_MODE_1024x768x16M   ;0x118      4MB
                        DW VESA_MODE_1280x1024x32K  ;0x119
                        DW VESA_MODE_1280x1024x64K  ;0x11a
                        DW VESA_MODE_1600x1200x32K  ;0x11d
                        DW VESA_MODE_1600x1200x64K  ;0x11e
                        DW 0FFFFh

;Offset 0x6695
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x66d1
VESAModeInfo640x400x256     DW 0280h        ;VESA_MODE_INFO_BytesPerScanLine
                            DW 0280h        ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 0190h        ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 007h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x66f3
VESAModeInfo640x480x256     DW 00280h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00280h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 001E0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 005h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6715
VESAModeInfo800x600x16      DW 00064h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00320h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00258h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 00Eh         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_EGA_Graphics;0x3
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6737
VESAModeInfo800x600x256     DW 00320h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00320h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00258h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 002h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6759
VESAModeInfo1024x768x256    DW 00400h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00400h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00300h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 001h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x677b
VESAModeInfo1280x1024x256   DW 00500h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00500h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00400h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x679d
VESAModeInfo80x60xText      DW 00050h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00050h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 0003Ch       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 008h         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_Text;0x0
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x67bf
VESAModeInfo132x25xText     DW 00084h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00084h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00019h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_Text;0x0
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x67e1
VESAModeInfo132x43xText     DW 00084h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00084h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 0002Bh       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 008h         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_Text;0x0
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6803
VESAModeInfo132x50xText     DW 00084h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00084h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00032h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 008h         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_Text;0x0
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6825
VESAModeInfo132x60xText     DW 00084h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00084h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 0003Ch       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 008h         ;VESA_MODE_INFO_CharCellHeight
                            DB 004h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 004h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_Text;0x0
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6847
VESAModeInfo640x480x32K     DW 00500h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00280h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 001E0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 002h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Ah         ;VESA_MODE_INFO_RedFieldPosition
                            DB 005h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 001h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 00Fh         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6869
VESAModeInfo640x480x64K     DW 00500h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00280h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 001E0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 002h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Bh         ;VESA_MODE_INFO_RedFieldPosition
                            DB 006h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x688b
VESAModeInfo640x480x16M     DW 00A00h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00280h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 001E0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 020h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 008h         ;VESA_MODE_INFO_RedMaskSize
                            DB 010h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 008h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 008h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 008h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 008h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 018h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x68ad
VESAModeInfo800x600x32K     DW 00640h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00320h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00258h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Ah         ;VESA_MODE_INFO_RedFieldPosition
                            DB 005h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 001h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 00Fh         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x68cf
VESAModeInfo800x600x64K     DW 00640h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00320h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00258h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Bh         ;VESA_MODE_INFO_RedFieldPosition
                            DB 006h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x68f1
VESAModeInfo800x600x16M     DW 00C80h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00320h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00258h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 020h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 008h         ;VESA_MODE_INFO_RedMaskSize
                            DB 010h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 008h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 008h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 008h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 008h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 018h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6913
VESAModeInfo1024x768x32K    DW 00800h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00400h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00300h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Ah         ;VESA_MODE_INFO_RedFieldPosition
                            DB 005h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 001h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 00Fh         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6935
VESAModeInfo1024x768x64K    DW 00800h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00400h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00300h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Bh         ;VESA_MODE_INFO_RedFieldPosition
                            DB 006h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6957
VESAModeInfo1024x768x16M    DW 01000h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00400h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00300h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 020h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 008h         ;VESA_MODE_INFO_RedMaskSize
                            DB 010h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 008h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 008h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 008h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 008h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 018h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6979
VESAModeInfo1280x1024x32K   DW 00A00h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00500h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00400h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Ah         ;VESA_MODE_INFO_RedFieldPosition
                            DB 005h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 001h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 00Fh         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x699b
VESAModeInfo1280x1024x64K   DW 00A00h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00500h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00400h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Bh         ;VESA_MODE_INFO_RedFieldPosition
                            DB 006h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x69bd
VESAModeInfo1280x1024x16M   DW 01400h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00500h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00400h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 020h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 008h         ;VESA_MODE_INFO_RedMaskSize
                            DB 010h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 008h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 008h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 008h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 008h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 018h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x69df
VESAModeInfo1600x1200x256   DW 00640h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00640h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 004B0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6a01
VESAModeInfo1600x1200x32K   DW 00C80h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00640h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 004B0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Ah         ;VESA_MODE_INFO_RedFieldPosition
                            DB 005h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 001h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 00Fh         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6a23
VESAModeInfo1600x1200x64K   DW 00C80h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00640h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 004B0h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 010h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_DirectColorGraphics;0x6
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 005h         ;VESA_MODE_INFO_RedMaskSize
                            DB 00Bh         ;VESA_MODE_INFO_RedFieldPosition
                            DB 006h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 005h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 005h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6a45
VESAModeInfoUnknown         DW 00000h       ;VESA_MODE_INFO_BytesPerScanLine
                            DW 00000h       ;VESA_MODE_INFO_WidthInPixelsOrChars
                            DW 00000h       ;VESA_MODE_INFO_HeightInPixelsOrChars
                            DB 008h         ;VESA_MODE_INFO_CharCellWidth
                            DB 010h         ;VESA_MODE_INFO_CharCellHeight
                            DB 001h         ;VESA_MODE_INFO_NumberOfMemoryPlanes
                            DB 008h         ;VESA_MODE_INFO_NumberOfBitsPerPixel
                            DB 001h         ;VESA_MODE_INFO_NumberOfBanks
                            DB VESA_MEM_MODEL_PackedPixelGraphics;0x4
                            DB 000h         ;VESA_MODE_INFO_SizeOfBankInKB
                            DB 000h         ;VESA_MODE_INFO_NumberOfImgPagesMinusOne
                            DB 001h         ;VESA_MODE_INFO_Reserved1E
                            DB 000h         ;VESA_MODE_INFO_RedMaskSize
                            DB 000h         ;VESA_MODE_INFO_RedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_GreenMaskSize
                            DB 000h         ;VESA_MODE_INFO_GreenFieldPosition
                            DB 000h         ;VESA_MODE_INFO_BlueMaskSize
                            DB 000h         ;VESA_MODE_INFO_BlueFieldPosition
                            DB 000h         ;VESA_MODE_INFO_ReservedMaskSize
                            DB 000h         ;VESA_MODE_INFO_ReservedFieldPosition
                            DB 000h         ;VESA_MODE_INFO_DirectColorModeInfo
                            DD 000000000h   ;VESA_MODE_INFO_LinearFrameBufferAddress
                            DD 000000000h   ;VESA_MODE_INFO_OffScreenMemoryAddress
                            DW 00000h       ;VESA_MODE_INFO_OffScreenMemorySizeInKB

;Offset 0x6a67
VESAModeInfoPointers    DW offset VESAModeInfo640x400x256   ;Offset 0x66d1
                        DW offset VESAModeInfo640x480x256   ;Offset 0x66f3
                        DW offset VESAModeInfo800x600x16    ;Offset 0x6715
                        DW offset VESAModeInfo800x600x256   ;Offset 0x6737
                        DW 0000h
                        DW offset VESAModeInfo1024x768x256  ;Offset 0x6759
                        DW 0000h
                        DW offset VESAModeInfo1280x1024x256 ;Offset 0x677b
                        DW offset VESAModeInfo80x60xText    ;Offset 0x679d
                        DW offset VESAModeInfo132x25xText   ;Offset 0x67bf
                        DW offset VESAModeInfo132x43xText   ;Offset 0x67e1
                        DW offset VESAModeInfo132x50xText   ;Offset 0x6803
                        DW offset VESAModeInfo132x60xText   ;Offset 0x6825
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW offset VESAModeInfo640x480x32K   ;Offset 0x6847
                        DW offset VESAModeInfo640x480x64K   ;Offset 0x6869
                        DW offset VESAModeInfo640x480x16M   ;Offset 0x688b
                        DW offset VESAModeInfo800x600x32K   ;Offset 0x68ad
                        DW offset VESAModeInfo800x600x64K   ;Offset 0x68cf
                        DW offset VESAModeInfo800x600x16M   ;Offset 0x68f1
                        DW offset VESAModeInfo1024x768x32K  ;Offset 0x6913
                        DW offset VESAModeInfo1024x768x64K  ;Offset 0x6935
                        DW offset VESAModeInfo1024x768x16M  ;Offset 0x6957
                        DW offset VESAModeInfo1280x1024x32K ;Offset 0x6979
                        DW offset VESAModeInfo1280x1024x64K ;Offset 0x699b
                        DW offset VESAModeInfo1280x1024x16M ;Offset 0x69bd
                        DW offset VESAModeInfo1600x1200x256 ;Offset 0x69df
                        DW offset VESAModeInfo1600x1200x32K ;Offset 0x6a01
                        DW offset VESAModeInfo1600x1200x64K ;Offset 0x6a23
                        DW offset VESAModeInfoUnknown       ;Offset 0x6a45

;Offset 0x6aa7
Data0x6aa7              DW Data0x6de3   ;Offset 0x6de3 VESA_MODE_640x400x256
                        DW Data0x6df9   ;Offset 0x6df9 VESA_MODE_640x480x256
                        DW Data0x6e51   ;Offset 0x6e51 VESA_MODE_800x600x16
                        DW Data0x6e69   ;Offset 0x6e69 VESA_MODE_800x600x256
                        DW 0000h
                        DW Data0x6ec1   ;Offset 0x6ec1 VESA_MODE_1024x768x256
                        DW 0000h
                        DW Data0x6f19   ;Offset 0x6f19 VESA_MODE_1280x1024x256
                        DW Data0x6fc9   ;Offset 0x6fc9 VESA_MODE_80x60xText
                        DW Data0x6fdf   ;Offset 0x6fdf VESA_MODE_132x25xText
                        DW Data0x6ff5   ;Offset 0x6ff5 VESA_MODE_132x43xText
                        DW Data0x700b   ;Offset 0x700b VESA_MODE_132x50xText
                        DW Data0x7021   ;Offset 0x7021 VESA_MODE_132x60xText
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW Data0x6e0f   ;Offset 0x6e0f VESA_MODE_640x480x32K
                        DW Data0x6e25   ;Offset 0x6e25 VESA_MODE_640x480x64K
                        DW Data0x6e3b   ;Offset 0x6e3b VESA_MODE_640x480x16M
                        DW Data0x6e7f   ;Offset 0x6e7f VESA_MODE_800x600x32K
                        DW Data0x6e95   ;Offset 0x6e95 VESA_MODE_800x600x64K
                        DW Data0x6eab   ;Offset 0x6eab VESA_MODE_800x600x16M
                        DW Data0x6ed7   ;Offset 0x6ed7 VESA_MODE_1024x768x32K
                        DW Data0x6eed   ;Offset 0x6eed VESA_MODE_1024x768x64K
                        DW Data0x6f03   ;Offset 0x6f03 VESA_MODE_1024x768x16M
                        DW Data0x6f2f   ;Offset 0x6f2f VESA_MODE_1280x1024x32K
                        DW Data0x6f45   ;Offset 0x6f45 VESA_MODE_1280x1024x64K
                        DW Data0x6f5b   ;Offset 0x6f5b VESA_MODE_1280x1024x16M
                        DW Data0x6f71   ;Offset 0x6f71 VESA_MODE_1600x1200x256
                        DW Data0x6f87   ;Offset 0x6f87 VESA_MODE_1600x1200x32K
                        DW Data0x6f9d   ;Offset 0x6f9d VESA_MODE_1600x1200x64K

;Offset 0x6ae5
Data0x6ae5              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 06Fh, 067h, 04Fh, 04Fh, 00Bh, 053h
                        DB 09Fh, 0C0h, 01Fh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 097h, 08Dh, 08Fh, 050h, 000h
                        DB 08Fh, 0C1h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6b25
Data0x6b25              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 05Fh, 04Fh, 04Fh, 003h, 051h
                        DB 09Dh, 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0E9h, 02Bh, 0DFh, 050h, 000h
                        DB 0DFh, 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6b65
Data0x6b65              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 07Fh, 063h, 063h, 003h, 068h
                        DB 018h, 072h, 0F0h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 058h, 02Ch, 057h, 064h, 000h
                        DB 057h, 073h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6ba5
Data0x6ba5              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 0A3h, 07Fh, 07Fh, 007h, 082h
                        DB 093h, 024h, 0F5h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 002h, 028h, 0FFh, 080h, 000h
                        DB 0FFh, 025h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6be5
Data0x6be5              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 0CEh, 09Fh, 09Fh, 012h, 0A5h
                        DB 013h, 028h, 05Ah, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 023h, 0FFh, 0A0h, 000h
                        DB 0FFh, 029h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x6c24
Data0x6c24              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 009h, 0C7h, 0C7h, 00Dh, 0CFh, 007h
                        DB 0E0h, 000h, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0B0h, 023h, 0AFh, 0C8h, 000h, 0AFh
                        DB 0E1h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x6c63
Data0x6c63              DB 064h
                        DB 024h, 010h, 000h, 0F0h, 001h, 00Fh, 000h, 006h, 02Fh, 07Fh, 063h, 063h, 082h, 06Bh, 01Bh, 072h
                        DB 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 058h, 08Ch, 057h, 032h, 000h, 057h, 073h
                        DB 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x6ca3
Data0x6ca3              DB 050h
                        DB 03Bh, 008h, 080h, 025h, 001h, 003h, 000h, 002h, 0EFh, 060h, 04Fh, 050h, 083h, 052h, 09Eh, 00Bh
                        DB 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EBh, 08Ch, 0DFh, 028h, 01Fh, 0E6h, 006h
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6ce3
Data0x6ce3              DB 084h
                        DB 018h, 010h, 0D0h, 019h, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 0C0h
                        DB 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Eh, 086h, 08Fh, 042h, 01Fh, 096h, 0BBh
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6d23
Data0x6d23              DB 084h
                        DB 02Ah, 008h, 058h, 02Ch, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 088h
                        DB 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 066h, 084h, 057h, 042h, 01Fh, 05Eh, 083h
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6d63
Data0x6d63              DB 084h
                        DB 031h, 008h, 000h, 034h, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 0C0h
                        DB 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 09Eh, 086h, 08Fh, 042h, 01Fh, 096h, 0BBh
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6da3
Data0x6da3              DB 084h
                        Db 03Bh, 008h, 000h, 03Eh, 001h, 003h, 000h, 002h, 0EFh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 011h
                        Db 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EBh, 08Eh, 0DFh, 042h, 01Fh, 0E6h, 00Ch
                        Db 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        Db 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh, 

;Offset 0x6de3      ;VESA_MODE_640x400x256
Data0x6de3              DB 086h, 06Ah, 000h, 000h
                        DW 00280h
                        DW 00190h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6ae5
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 000h

;Offset 0x6df9      ;VESA_MODE_640x480x256
Data0x6df9              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6b25
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6e0f      ;VESA_MODE_640x480x32K
Data0x6e0f              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 00Fh, 050h
                        DW offset Data0x6b25
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6e25      ;VESA_MODE_640x480x64K
Data0x6e25              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 010h, 010h
                        DW offset Data0x6b25
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6e3b      ;VESA_MODE_640x480x16M
Data0x6e3b              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 020h, 030h
                        DW offset Data0x6b25
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6e51      ;VESA_MODE_800x600x16
Data0x6e51              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 010h, 000h, 001h, 009h
                        DW offset Data0x6c63
                        DB 000h, 000h, 000h, 000h, 01Bh, 012h, 000h, 000h, 007h, 016h

;Offset 0x6e69      ;VESA_MODE_800x600x256
Data0x6e69              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6b65
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 007h, 016h 

;Offset 0x6e7f      ;VESA_MODE_800x600x32K
Data0x6e7f              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6b65
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6e95      ;VESA_MODE_800x600x64K
Data0x6e95              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6b65
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6eab      ;VESA_MODE_800x600x16M
Data0x6eab              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6b65
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6ec1      ;VESA_MODE_1024x768x256-------------
Data0x6ec1              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6ba5
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6ed7      ;VESA_MODE_1024x768x32K
Data0x6ed7              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6ba5
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6eed      ;VESA_MODE_1024x768x64K
Data0x6eed              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6ba5
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f03      ;VESA_MODE_1024x768x16M
Data0x6f03              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6ba5
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f19      ;VESA_MODE_1280x1024x256
Data0x6f19              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6be5
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f2f      ;VESA_MODE_1280x1024x32K
Data0x6f2f              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6be5
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f45      ;VESA_MODE_1280x1024x64K
Data0x6f45              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6be5
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f5b      ;VESA_MODE_1280x1024x16M
Data0x6f5b              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6be5
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f71      ;VESA_MODE_1600x1200x256
Data0x6f71              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6c24
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f87      ;VESA_MODE_1600x1200x32K
Data0x6f87              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6c24
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6f9d      ;VESA_MODE_1600x1200x64K
Data0x6f9d              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6c24
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6fb3      ;VESA_MODE_1600x1200x16M (unreferenced)
                        DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6c24
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6fc9      ;VESA_MODE_80x60xText
Data0x6fc9              DB 070h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6ca3
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6fdf      ;VESA_MODE_132x25xText
Data0x6fdf              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00190h
                        DB 010h, 000h, 001h, 002h
                        DW offset Data0x6ce3
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6ff5      ;VESA_MODE_132x43xText
Data0x6ff5              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00158h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6d23
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x700b      ;VESA_MODE_132x50xText
Data0x700b              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00190h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6d63
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x7021      ;VESA_MODE_132x60xText
Data0x7021              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 001E0h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6da3
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x7037
OEMName                 DB 'Matrox Graphics Inc.', 000h

;Offset 0x704c
VendorName              DB 'Matrox', 000h

;Offset 0x7053
ProductName             DB 'MYSTIQUE', 000h

;Offset 0x705c
RevisionString          DB '00', 000h

;Offset 0x705f
VBEMGATemplate          DB 'VBE/MGA'
                        DB 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h
                        DW 00108h   ;BIOS Version (1.8)

;Offset 0x7072
MatroxOEMExtensionTable DW offset InstallationCheckGetCaps;Offset 0x76d0
                        DW offset AccessMGAIndexedDataRegister;Offset 0x7757
                        DW offset AccessMGAIndirectRegister;Offset 0x776d
                        DW offset Label0x77aa;Offset 0x77aa
                        DW offset Label0x77b9;Offset 0x77b9
                        DW offset Label0x77d4;Offset 0x77d4

VESAHandler:                            ;Offset 0x707e
    cmp       al, 14h                   ;Matrox OEM extension
    je        Label0x7092               ;Offset 0x7092
    cmp       al, 0ah
    ja        Label0x70a4               ;Offset 0x70a4
    sti
    cld
    xor       ah, ah
    shl       ax, 01h
    xchg      ax, bx
    jmp       word ptr cs:[bx + VESACallTable];Offset 0x6605
Label0x7092:                            ;Offset 0x7092
    cmp       bl, 05h
    ja        Label0x70a4               ;Offset 0x70a4
    mov       ax, bx
    and       bx, 00ffh
    shl       bx, 01h
    jmp       word ptr cs:[bx + MatroxOEMExtensionTable];Offset 0x7072
Label0x70a4:                            ;Offset 0x70a4
    mov       ax, 0ffffh
    iret

Func0x70a8 PROC NEAR                    ;Offset 0x70a8
    push cx
    cmp  bx, 81ffh
    jne  Label0x70b5                    ;Offset 0x70b5
    mov  bx, 011fh
    jmp  Label0x70ca                    ;Offset 0x70ca
    nop
Label0x70b5:                            ;Offset 0x70b5
    mov  cx, bx
    and  cx, 01ffh
    cmp  cx, 006ah
    ;jne  Label0x70ca                    ;Offset 0x70ca
    DB 00Fh, 085h, 008h, 000h
    and  bx, 0fe00h
    or   bx, 0102h
Label0x70ca:                            ;Offset 0x70ca
    pop  cx
    ret
Func0x70a8 ENDP

Func0x70cc PROC NEAR                    ;Offset 0x70cc
    push  ax
    push  bx
    push  si
    push  ds
    push  cs
    pop   ds
    and   bx, 01ffh
    mov   si, offset SupportedVESAVideoModes;Offset 0x665d
Label0x70d9:                            ;Offset 0x70d9
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x70e9                   ;Offset 0x70e9
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x7124                   ;Offset 0x7124
    DB 00Fh, 084h, 03Dh, 000h
    jmp   Label0x70d9                   ;Offset 0x70d9
Label0x70e9:                            ;Offset 0x70e9
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x70f9                   ;Offset 0x70f9
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x7124                   ;Offset 0x7124
    DB 00Fh, 084h, 02Dh, 000h
    jmp   Label0x70e9                   ;Offset 0x70e9
Label0x70f9:                            ;Offset 0x70f9
    call  GetMemoryIn64KBlocks          ;Offset 0x5846
    call  CapAXTo0x40                   ;Offset 0x5832
    xchg  al, ah
    ;cmp   ax, 0020h
    DB 03Dh, 020h, 000h
    ;jne   Label0x710e                   ;Offset 0x710e
    DB 00Fh, 085h, 006h, 000h
    ;or    ax, 0001h
    DB 00Dh, 001h, 000h
    jmp   Label0x7126                   ;Offset 0x7126
    nop
Label0x710e:                            ;Offset 0x710e
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x711e                   ;Offset 0x711e
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x7124                   ;Offset 0x7124
    DB 00Fh, 084h, 008h, 000h
    jmp   Label0x710e                   ;Offset 0x710e
Label0x711e:                            ;Offset 0x711e
    ;or    ax, 0001h
    DB 00Dh, 001h, 000h
    jmp   Label0x7126                   ;Offset 0x7126
    nop
Label0x7124:                            ;Offset 0x7124
    xor   ax, ax
Label0x7126:                            ;Offset 0x7126
    pop   ds
    pop   si
    pop   bx
    pop   ax
    ret
Func0x70cc ENDP

GetSuperVGAInformation:                 ;Offset 0x712b
    mov       bx, ax
    push      ds
    push      si
    push      cx
    push      di
    push      cs
    pop       ds
    mov       ax, 4256h                 ;'VB'
    cmp       ax, word ptr es:[di + VESA_INFO_Signature];0x0
    jne       Label0x715b               ;Offset 0x715b
    mov       ax, 3245h                 ;'E2'
    cmp       ax, word ptr es:[di + VESA_INFO_Signature + 02h];0x2
    jne       Label0x715b               ;Offset 0x715b
    push      di
    add       di, 0100h
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    mov       cx, 0013h
    mov       si, offset VendorName     ;Offset 0x704c
    rep movsb
    pop       di
Label0x715b:                            ;Offset 0x715b
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       cx, 0022h
    mov       si, offset VESAResponseTemplate;Offset 0x661b
    rep movsb
    pop       di
    mov       al, 18h                   ;OEM Version 1.8
    shl       ax, 04h
    mov       al, 18h                   ;OEM Version 1.8
    and       al, 0fh
    mov       word ptr es:[di + VESA_INFO_OEMSoftwareVersion], ax;0x14
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerSeg], ax;0x8
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerSeg], ax;0x18
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerSeg], ax;0x1c
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerSeg], ax;0x20
    mov       ax, offset OEMName        ;Offset 0x7037
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerOfs], ax;0x6
    mov       ax, offset VendorName     ;Offset 0x704c
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerOfs], ax;0x16
    mov       ax, offset ProductName    ;Offset 0x7053
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerOfs], ax;0x1a
    mov       ax, offset RevisionString ;Offset 0x705c
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerOfs], ax;0x1e
    call      GetMemoryIn64KBlocks      ;Offset 0x5846
    xchg      ah, al
    mov       word ptr es:[di + VESA_INFO_MemoryIn64KBBlocks], ax;0x12
    xchg      ah, al
    call      CapAXTo0x40               ;Offset 0x5832
    mov       cl, ah
    mov       ax, es
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerSeg], es;0x10
    mov       ax, di
    ;add       ax, 0022h
    DB 005h, 022h, 000h
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerOfs], ax;0xe
    push      bx
    push      di
    mov       di, ax
    mov       si, offset SupportedVESAVideoModes;Offset 0x665d
Label0x71ce:                            ;Offset 0x71ce
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x71d7               ;Offset 0x71d7
    stosw
    jmp       Label0x71ce               ;Offset 0x71ce
Label0x71d7:                            ;Offset 0x71d7
    call      FindMystique              ;Offset 0x57c6 - not used lol
Label0x71da:                            ;Offset 0x71da
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x71e3               ;Offset 0x71e3
    stosw
    jmp       Label0x71da               ;Offset 0x71da
Label0x71e3:                            ;Offset 0x71e3
    cmp       cl, 20h
    je        Label0x71ff               ;Offset 0x71ff
Label0x71e8:                            ;Offset 0x71e8
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x71f1               ;Offset 0x71f1
    stosw
    jmp       Label0x71e8               ;Offset 0x71e8
Label0x71f1:                            ;Offset 0x71f1
    cmp       cl, 40h
    je        Label0x71ff               ;Offset 0x71ff
Label0x71f6:                            ;Offset 0x71f6
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x71ff               ;Offset 0x71ff
    stosw
    jmp       Label0x71f6               ;Offset 0x71f6
Label0x71ff:                            ;Offset 0x71ff
    stosw
    pop       di
    pop       bx
    mov       ax, 004fh
    pop       cx
    pop       si
    pop       ds
    iret

GetSuperVGAModeInformation:             ;Offset 0x7209
    push      ax
    push      ds
    push      di
    push      si
    push      ecx
    push      edx
    mov       bx, cx
    call      Func0x70a8                ;Offset 0x70a8
    cmp       bx, 011fh
    ;je        Label0x722f               ;Offset 0x722f
    DB 00Fh, 084h, 011h, 000h
    call      Func0x70cc                ;Offset 0x70cc
    ;jne       Label0x722a               ;Offset 0x722a
    DB 00Fh, 085h, 005h, 000h
    and       bx, 007fh
    jmp       Label0x722f               ;Offset 0x722f
Label0x722a:                            ;Offset 0x722a
    mov       ah, 01h
    jmp       Label0x734b               ;Offset 0x734b
Label0x722f:                            ;Offset 0x722f
    push      eax
    push      cx
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       ax, cs
    mov       ds, ax
    mov       cx, 0010h
    ;mov       si, VBEGfxModeInfoTemplate;Offset 0x663d
    DB 0BEh, 03Dh, 066h
    cmp       bx, VESA_MODE_80x60xText AND 7fh;0x8
    jb        IsGraphicsMode            ;Offset 0x7256
    cmp       bx, VESA_MODE_132x60xText AND 7fh;0xc
    ja        IsGraphicsMode            ;Offset 0x7256
    mov       cx, 0010h
    ;mov       si, VBETextModeInfoTemplate;Offset 0x664d
    DB 0BEh, 04Dh, 066h
IsGraphicsMode:                         ;Offset 0x7256
    cld
    rep movsb
    shl       bx, 01h
    mov       si, word ptr cs:[bx + VESAModeInfoPointers];Offset 0x6a67
    push      si
    mov       cx, 0022h
    rep movsb
    pop       si
    pop       di
    push      ebx
    push      edx
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType], VESA_MEM_MODEL_EGA_Graphics;0x1b 0x3
    ;je        Label0x72b8               ;Offset 0x72b8
    DB 00Fh, 084h, 043h, 000h
    call      GetMemoryIn64KBlocks      ;Offset 0x5846
    call      CapAXTo0x40               ;Offset 0x5832
    xchg      al, ah
    ;cmp       ax, 0020h
    DB 03Dh, 020h, 000h
    ;je        Label0x72b8               ;Offset 0x72b8
    DB 00Fh, 084h, 034h, 000h
    xor       dx, dx
    mov       bx, word ptr [si + VESA_MODE_INFO_BytesPerScanLine - 10h];0x0
    mov       ax, word ptr [si + VESA_MODE_INFO_HeightInPixelsOrChars - 10h];0x04
    mul       bx
    mov       bx, 0ffffh
    div       bx
    or        dx, dx
    ;je        Label0x7299               ;Offset 0x7299
    DB 00Fh, 084h, 001h, 000h
    inc       ax
Label0x7299:                            ;Offset 0x7299
    xor       dx, dx
    mul       bx
    shl       edx, 10h
    mov       dx, ax
    mov       ebx, edx
    xor       edx, edx
    mov       eax, 00400000h
    div       ebx
    dec       al
    mov       byte ptr es:[di + VESA_MODE_INFO_NumberOfImgPagesMinusOne], al  ;0x1d
Label0x72b8:                            ;Offset 0x72b8
    pop       edx
    pop       ebx
    mov       al, byte ptr [si + VESA_MODE_INFO_MemoryModelType - 010h]   ;0xb
    cmp       al, VESA_MEM_MODEL_Text   ;0x0
    jne       Label0x72cd               ;Offset 0x72cd
    mov       ax, VESA_MODE_ATTR_ModeGraphics;0x10
    not       al
    and       word ptr es:[di + VESA_MODE_INFO_Attributes], ax
    jmp       Label0x72d7               ;Offset 0x72d7
Label0x72cd:                            ;Offset 0x72cd
    cmp       al, VESA_MEM_MODEL_EGA_Graphics;0x3
    je        Label0x72d7               ;Offset 0x72d7
    mov       ax, VESA_MODE_ATTR_LinearSupported;0x80
    or        word ptr es:[di + VESA_MODE_INFO_Attributes], ax
Label0x72d7:                            ;Offset 0x72d7
    mov       ax, cs
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningSeg], ax;0xe
    pop       bx
    mov       ax, offset AccessMemoryPageFAR;Offset 0x7461
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningOfs], ax;0xc
    add       di, 0010h
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType - 10h], VESA_MEM_MODEL_EGA_Graphics;0xb 0x3
    ;je        Label0x7347               ;Offset 0x7347
    DB 00Fh, 084h, 056h, 000h
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType - 10h], VESA_MEM_MODEL_Text;0xb 0x0
    ;je        Label0x7347               ;Offset 0x7347
    DB 00Fh, 084h, 04Dh, 000h
    call      FindMystique              ;Offset 0x57c6
    call      ReadPCIBase0AndBase1      ;Offset 0x57d2
    ;mov       dword ptr es:[di + VESA_MODE_INFO_LinearFrameBufferAddress - 10h], ecx;0x18
    DB 026h, 066h, 089h, 04Dh, 018h
    xor       dx, dx
    mov       ax, word ptr es:[di + VESA_MODE_INFO_HeightInPixelsOrChars - 10h];0x4
    mul       word ptr es:[di + VESA_MODE_INFO_BytesPerScanLine - 10h];0x0
    shl       edx, 10h
    mov       dx, ax
    mov       ecx, edx
    and       ecx, 000003ffh
    ;je        Label0x7329               ;Offset 0x7329
    DB 00Fh, 084h, 007h, 000h
    add       edx, 00000400h
Label0x7329:                            ;Offset 0x7329
    and       edx, 0fffffc00h
    ;mov       dword ptr es:[di + VESA_MODE_INFO_OffScreenMemoryAddress - 10h], edx;0x1c
    DB 026h, 066h, 089h, 055h, 01Ch
    shr       edx, 0ah
    call      GetMemoryIn64KBlocks      ;Offset 0x5846
    xchg      ah, al
    shl       ax, 06h
    sub       ax, dx
    mov       word ptr es:[di + VESA_MODE_INFO_OffScreenMemorySizeInKB - 10h], ax;0x20
Label0x7347:                            ;Offset 0x7347
    pop       eax
    mov       ah, 00h
Label0x734b:                            ;Offset 0x734b
    mov       al, 4fh
    pop       edx
    pop       ecx
    pop       si
    pop       di
    pop       ds
    pop       bx
    iret

SetSuperVGAVideoMode:                   ;Offset 0x7356
    push  ax
    push  ds
    push  di
    push  si
    push  cx
    push  dx
    push  cs
    pop   ds
    mov   bx, ax
    call  IsExtensionReg7Writeable      ;Offset 0x5606
    ;je    Label0x73e1                   ;Offset 0x73e1
    DB 00Fh, 084h, 07Ah, 000h
    push  bx
    call  Func0x70a8                    ;Offset 0x70a8
    mov   ax, bx
    cmp   bl, 1fh
    jne   Label0x7375                   ;Offset 0x7375
    mov   bx, VESA_MODE_640x400x256     ;0x100
Label0x7375:                            ;Offset 0x7375
    call  Func0x70cc                    ;Offset 0x70cc
    jne   Label0x73a4                   ;Offset 0x73a4
    and   bx, 00ffh
    shl   bx, 01h
    ;add   bx, Data0x6aa7                ;Offset 0x6aa7
    DB 081h, 0C3h, 0A7h, 06Ah
    mov   si, word ptr cs:[bx]
    push  ax
    mov   ax, 0bd60h
    int   6dh
    pop   ax
    push  ds
    push  si
    call  Func0x6518                    ;Offset 0x6518
    pop   ax
    pop   ax
    pop   bx
    cmp   bl, 1fh
    jne   Label0x73a2                   ;Offset 0x73a2
    mov   dx, VGA_SequenceIndex         ;Port 0x3c4
    mov   ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out   dx, ax
Label0x73a2:                            ;Offset 0x73a2
    jmp   Label0x73bf                   ;Offset 0x73bf
Label0x73a4:                            ;Offset 0x73a4
    pop   bx
    mov   ax, bx
    ;cmp   ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 03Dh, 013h, 000h
    jle   Label0x73b1                   ;Offset 0x73b1
    mov   ax, 014fh
    jmp   Label0x73e1                   ;Offset 0x73e1
Label0x73b1:                            ;Offset 0x73b1
    mov   ah, 00h
    mov   al, bl
    int   10h                           ;Regular int 10h handler for standard modes
    inc   ax
    jne   Label0x73bf                   ;Offset 0x73bf
    mov   ax, 014fh
    jmp   Label0x73e1                   ;Offset 0x73e1
Label0x73bf:                            ;Offset 0x73bf
    push  bx
    and   bx, 7f7fh
    cmp   bx, VESA_MODE_640x400x256     ;0x100
    jl    Label0x73cd                   ;Offset 0x73cd
    add   bl, 20h                       ;VESA mode is (mode & 0x7f) + 0x20
Label0x73cd:                            ;Offset 0x73cd
    xor   ax, ax
    mov   ds, ax
    mov   al, bl
    pop   bx
    mov   ah, bh
    and   ah, 0c0h                      ;Top 2 bits are preserved
    or    al, ah                        ;And stored along with the mode
    mov   byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov   ax, 004fh
Label0x73e1:                            ;Offset 0x73e1
    pop   dx
    pop   cx
    pop   si
    pop   di
    pop   ds
    pop   bx
    or    ah, ah
    jne   Label0x73eb                   ;Offset 0x73eb   If not, then still.
Label0x73eb:                            ;Offset 0x73eb
    iret

SuperVGAGetCurrentVideoMode:            ;Offset 0x73ec
    mov  ax, 0f00h
    int  10h
    xor  bx, bx
    mov  ah, al
    and  ah, 0c0h
    and  al, 3fh
    cmp  al, 20h
    jl   Label0x740e                    ;Offset 0x740e
    cmp  al, 40h
    jge  Label0x740e                    ;Offset 0x740e
    sub  al, 20h
    inc  bh
    or   bh, ah
    cmp  al, 1fh
    jne  Label0x740e                    ;Offset 0x740e
    mov  al, 0ffh
Label0x740e:                            ;Offset 0x740e
    mov  bl, al
    mov  ax, 004fh
    iret

SaveRestoreSuperVGAVideoState:          ;Offset 0x7414
    mov  bx, ax
    mov  ah, 1ch
    mov  al, dl
    push ax
    push cx
    test cx, 0008h
    je   Label0x744c                    ;Offset 0x744c
    cmp  al, 00h
    jne  Label0x7432                    ;Offset 0x7432
    cmp  cx, 0008h
    jne  Label0x744c                    ;Offset 0x744c
    mov  bx, 0001h
    pop  cx
    pop  ax
    jmp  Label0x745d                    ;Offset 0x745d
Label0x7432:                            ;Offset 0x7432
    cmp  al, 01h
    jne  Label0x743f                    ;Offset 0x743f
    push bx
    call SaveMGAState                   ;Offset 0x641b
    int  10h
    pop  bx
    jmp  Label0x744e                    ;Offset 0x744e
Label0x743f:                            ;Offset 0x743f
    cmp  al, 02h
    jne  Label0x744c                    ;Offset 0x744c
    push bx
    call RestoreMGAState                ;Offset 0x6476
    int  10h
    pop  bx
    jmp  Label0x744e                    ;Offset 0x744e
Label0x744c:                            ;Offset 0x744c
    int  10h
Label0x744e:                            ;Offset 0x744e
    pop  cx
    pop  ax
    test cx, 0008h
    je   Label0x745d                    ;Offset 0x745d
    cmp  al, 00h
    jne  Label0x745d                    ;Offset 0x745d
    add  bx, 0001h
Label0x745d:                            ;Offset 0x745d
    mov  ax, 004fh
    iret

AccessMemoryPageFAR PROC FAR            ;Offset 0x7461
    call AccessMemoryPage               ;Offset 0x7465
    retf
AccessMemoryPageFAR ENDP

AccessMemoryPage PROC NEAR              ;Offset 0x7465
    or   bh, bh
    jne  ReadPage                       ;Offset 0x7475
    push dx
    push si
    mov  ah, MGA_CRTCExt_MemoryPage     ;0x4
    mov  al, dl
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3c4
    pop  si
    pop  dx
    ret
ReadPage:                               ;Offset 0x7475
    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
    call MGAReadCRTCExtensionRegister   ;Offset 0x3ae
    mov  dl, al
    xor  dh, dh
    ret
AccessMemoryPage ENDP

CPUVideoMemoryControl:                  ;Offset 0x747f
    mov  bx, ax
    call AccessMemoryPage               ;Offset 0x7465
    mov  ax, 004fh
    iret

LogicalScanLineLengthHandling:          ;Offset 0x7488
    mov  bx, ax
    call Func0x59c0                     ;Offset 0x59c0
    and  al, 03h
    je   Label0x74db                    ;Offset 0x74db
    push si
    cmp  bl, 01h
    je   Label0x74d3                    ;Offset 0x74d3
    call Func0x76a6                     ;Offset 0x76a6
    cmp  bl, 03h
    jne  Label0x74a6                    ;Offset 0x74a6
    call Func0x7664                     ;Offset 0x7664
    mov  cx, ax
    jmp  Label0x74d6                    ;Offset 0x74d6
Label0x74a6:                            ;Offset 0x74a6
    cmp  bl, 02h
    jne  Label0x74b3                    ;Offset 0x74b3
Label0x74ab:                            ;Offset 0x74ab
    dec  al
    je   Label0x74b3                    ;Offset 0x74b3
    shr  cx, 01h
    jmp  Label0x74ab                    ;Offset 0x74ab
Label0x74b3:                            ;Offset 0x74b3
    mov  ax, word ptr cs:[si + 04h]
    cmp  ax, cx
    jb   Label0x74bd                    ;Offset 0x74bd
    mov  cx, ax
Label0x74bd:                            ;Offset 0x74bd
    mov  ax, cx
    call Func0x5b6e                     ;Offset 0x5b6e
    mov  cx, ax
    call Func0x7664                     ;Offset 0x7664
    cmp  ax, cx
    mov  ax, 024fh
    jb   Label0x74d9                    ;Offset 0x74d9
    mov  ax, cx
    call Func0x5b25                     ;Offset 0x5b25
Label0x74d3:                            ;Offset 0x74d3
    call Func0x5ae1                     ;Offset 0x5ae1
Label0x74d6:                            ;Offset 0x74d6
    mov  ax, 004fh
Label0x74d9:                            ;Offset 0x74d9
    pop  si
    iret
Label0x74db:                            ;Offset 0x74db
    cmp  bl, 03h
    jne  Label0x74e5                    ;Offset 0x74e5
    mov  cx, 0320h
    jmp  Label0x74fb                    ;Offset 0x74fb
Label0x74e5:                            ;Offset 0x74e5
    mov  dx, VGA_CRTControllerIndexD    ;Port 0x3d4
    mov  al, VGA_CRTCIdx_HorDispEnableEnd;0x1
    out  dx, al
    inc  dx
    in   al, dx
    inc  al
    xor  ah, ah
    mov  cx, ax
    shl  cx, 03h
    mov  bx, ax
    mov  dx, 027eh
Label0x74fb:                            ;Offset 0x74fb
    mov  ax, 004fh
    iret

DisplayStartHandling:                   ;Offset 0x74ff
    mov  bx, ax
    call Func0x59c0                     ;Offset 0x59c0
    and  al, 03h
    je   Label0x7524                    ;Offset 0x7524
    cmp  bl, 01h
    je   Label0x751d                    ;Offset 0x751d
    or   bl, bl
    je   Label0x7516                    ;Offset 0x7516
    cmp  bl, 80h
    jne  Label0x7593                    ;Offset 0x7593
Label0x7516:                            ;Offset 0x7516
    call Func0x5aa1                     ;Offset 0x5aa1
    mov  ax, 004fh
    iret
Label0x751d:                            ;Offset 0x751d
    call Func0x5a6a                     ;Offset 0x5a6a
    mov  ax, 004fh
    iret
Label0x7524:                            ;Offset 0x7524
    push si
    push ds
    push 0000h
    pop  ds
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop  ds
    mov  si, 0064h
    cmp  al, 22h
    ;je   Label0x7542                    ;Offset 0x7542
    DB 00Fh, 084h, 00Ch, 000h
    mov  si, 0050h
    cmp  al, 28h
    ;je   Label0x7542                    ;Offset 0x7542
    DB 00Fh, 084h, 003h, 000h
    mov  si, 0084h
Label0x7542:                            ;Offset 0x7542
    and  bl, 01h
    jne  Label0x756e                    ;Offset 0x756e
    or   bl, bl
    je   Label0x7550                    ;Offset 0x7550
    cmp  bl, 80h
    jne  Label0x7593                    ;Offset 0x7593
Label0x7550:                            ;Offset 0x7550
    push cx
    push dx
    xor  al, al
    call Func0x59d6                     ;Offset 0x59d6
    mov  ax, si
    mul  dx
    push dx
    push ax
    pop  eax
    shr  cx, 03h
    add  ax, cx
    call SetStartAddress                ;Offset 0x5a28
    pop  dx
    pop  cx
    pop  si
    mov  ax, 004fh
    iret
Label0x756e:                            ;Offset 0x756e
    call GetStartAddress                ;Offset 0x59f9
    xor  cx, cx
    xor  dx, dx
    or   eax, eax
    je   Label0x7589                    ;Offset 0x7589
    push eax
    pop  ax
    pop  dx
    mov  bx, si
    div  bx
    mov  cx, dx
    shl  cx, 03h
    mov  dx, ax
Label0x7589:                            ;Offset 0x7589
    mov  al, 01h
    call Func0x59d6                     ;Offset 0x59d6
    pop  si
    mov  ax, 004fh
    iret
Label0x7593:                            ;Offset 0x7593
    pop  si
    mov  ah, 01h
    mov  al, 4fh
    iret

GetSetDACPaletteControl:                ;Offset 0x7599
    push cx
    push dx
    mov  dx, ax
    call FindMystique                   ;Offset 0x57c6
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x5bab
    mov  ah, 01h
    or   dl, dl
    je   Label0x75bd                    ;Offset 0x75bd
    cmp  dl, 01h
    jne  Label0x75d5                    ;Offset 0x75d5
    xor  ah, ah
    mov  dh, 06h
    and  cl, MGA_MISCCTRL_DAC_Bit_MASK  ;0x8
    je   Label0x75d5                    ;Offset 0x75d5
    mov  dh, 08h
    jmp  Label0x75d5                    ;Offset 0x75d5
Label0x75bd:                            ;Offset 0x75bd
    and  cl, NOT MGA_MISCCTRL_DAC_Bit_MASK;0xf7
    cmp  dh, 08h
    mov  dh, 06h
    jb   Label0x75cc                    ;Offset 0x75cc
    mov  dh, 08h
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
Label0x75cc:                            ;Offset 0x75cc
    xchg cl, ch
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5b90
    xor  ah, ah
Label0x75d5:                            ;Offset 0x75d5
    mov  bx, dx
    pop  dx
    pop  cx
    mov  al, 4fh
    iret

GetSetPaletteEntries:                   ;Offset 0x75dc
    mov  bx, ax
    cmp  bl, 02h
    je   Label0x7640                    ;Offset 0x7640
    cmp  bl, 03h
    je   Label0x7640                    ;Offset 0x7640
    push bx
    push cx
    push dx
    push di
    mov  al, dl
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    out  dx, al
    mov  al, bl
    cmp  al, 80h
    jne  Label0x7604                    ;Offset 0x7604
    mov  dl, VGA_InputStatus1D_lowbyte  ;Port 0x3da
Label0x75fa:                            ;Offset 0x75fa
    in   al, dx
    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne  Label0x75fa                    ;Offset 0x75fa
Label0x75ff:                            ;Offset 0x75ff
    in   al, dx
    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je   Label0x75ff                    ;Offset 0x75ff
Label0x7604:                            ;Offset 0x7604
    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
    mov  al, bl
    and  al, 01h
    je   Label0x7623                    ;Offset 0x7623
Label0x760c:                            ;Offset 0x760c
    in   al, dx
    mov  byte ptr es:[di + 02h], al
    in   al, dx
    mov  byte ptr es:[di + 01h], al
    in   al, dx
    mov  byte ptr es:[di], al
    inc  di
    inc  di
    inc  di
    inc  di
    dec  cx
    jne  Label0x760c                    ;Offset 0x760c
    jmp  Label0x7638                    ;Offset 0x7638
Label0x7623:                            ;Offset 0x7623
    mov  al, byte ptr es:[di + 02h]
    out  dx, al
    mov  al, byte ptr es:[di + 01h]
    out  dx, al
    mov  al, byte ptr es:[di]
    out  dx, al
    inc  di
    inc  di
    inc  di
    inc  di
    dec  cx
    jne  Label0x7623                    ;Offset 0x7623
Label0x7638:                            ;Offset 0x7638
    pop  di
    pop  dx
    pop  cx
    pop  bx
    mov  ax, 004fh
    iret
Label0x7640:                            ;Offset 0x7640
    mov  ax, 024fh
    iret

GetProtectedModeInterface:              ;Offset 0x7644
    mov  bx, ax
    mov  ah, 01h
    or   bl, bl
    jne  Label0x7661                    ;Offset 0x7661
    mov  ax, cs
    mov  es, ax
    mov  di, offset PINS                ;Offset 0x77e0
    xor  ax, ax
    mov  al, byte ptr es:[di + 02h]
    add  di, ax
    mov  cx, word ptr es:[di + 08h]
    xor  ah, ah
Label0x7661:                            ;Offset 0x7661
    mov  al, 4fh
    iret

Func0x7664 PROC NEAR                    ;Offset 0x7664
    push dx
    push cx
    call Func0x59c0                     ;Offset 0x59c0
    xor  dx, dx
    mov  dl, ah
    mov  cl, al
    and  cl, 03h
    or   cl, cl
    jne  Label0x767c                    ;Offset 0x767c
    mov  cx, word ptr cs:[si + 04h]
    jmp  Label0x76a0                    ;Offset 0x76a0
Label0x767c:                            ;Offset 0x767c
    dec  cl
    push bx
    push cx
    mov  ax, word ptr cs:[si + 06h]
    shl  ax, cl
    mov  cx, ax
    xor  ax, ax
    div  cx
    mov  bx, 0010h
    pop  cx
    shr  bx, cl
    not  bx
    inc  bx
    and  al, bl
    pop  bx
    cmp  ax, 1f80h
    jbe  Label0x76a0                    ;Offset 0x76a0
    mov  ax, 1f80h
Label0x76a0:                            ;Offset 0x76a0
    call Func0x5b6e                     ;Offset 0x5b6e
    pop  cx
    pop  dx
    ret
Func0x7664 ENDP

Func0x76a6 PROC NEAR                    ;Offset 0x76a6
    push ds
    push di
    xor  ax, ax
    mov  ds, ax
    ;mov  si, offset Data0x6aa7          ;Offset 0x6aa7
    DB 0BEh, 0A7h, 06Ah
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    and  al, 3fh
    cmp  al, 20h
    jl   Label0x76cb                    ;Offset 0x76cb
    cmp  al, 40h
    jge  Label0x76cb                    ;Offset 0x76cb
    sub  al, 20h
    shl  ax, 01h
    add  si, ax
    shr  ax, 01h
    inc  ah
    mov  si, word ptr cs:[si]
    jmp  Label0x76cd                    ;Offset 0x76cd
Label0x76cb:                            ;Offset 0x76cb
    xor  si, si
Label0x76cd:                            ;Offset 0x76cd
    pop  di
    pop  ds
    ret
Func0x76a6 ENDP

InstallationCheckGetCaps:               ;Offset 0x76d0
    mov       bx, ax
    push      ds
    push      cx
    push      dx
    push      si
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       ax, cs
    mov       ds, ax
    mov       cx, 0013h
    mov       si, offset VBEMGATemplate ;Offset 0x705f
    rep movsb
    pop       di
    mov       ax, cs
    mov       word ptr es:[di + VBE_MGA_VendorNameSeg], ax;0x15
    mov       word ptr es:[di + VBE_MGA_ProductNameSeg], ax;0x19
    mov       word ptr es:[di + VBE_MGA_RevisionSeg], ax;0x1d
    mov       word ptr es:[di + VBE_MGA_OEMNameSeg], ax;0x21
    mov       word ptr es:[di + VBE_MGA_BuildVersionSeg], ax;0x25
    mov       word ptr es:[di + VBE_MGA_PINSSeg], ax;0x29
    mov       ax, offset VendorName     ;Offset 0x704c
    mov       word ptr es:[di + VBE_MGA_VendorNameOfs], ax;0x13
    mov       ax, offset ProductName    ;Offset 0x7053
    mov       word ptr es:[di + VBE_MGA_ProductNameOfs], ax;0x17
    mov       ax, offset RevisionString ;Offset 0x705c
    mov       word ptr es:[di + VBE_MGA_RevisionOfs], ax;0x1b
    mov       ax, offset OEMName        ;Offset 0x7037
    mov       word ptr es:[di + VBE_MGA_OEMNameOfs], ax;0x1f
    mov       ax, offset BuildVersion   ;Offset 0x78
    mov       word ptr es:[di + VBE_MGA_BuildVersionOfs], ax;0x23
    mov       ax, word ptr cs:[PINSPointer];Offset 0x7ffc
    mov       word ptr es:[di + VBE_MGA_PINSOfs], ax;0x27
    call      FindMystique              ;Offset 0x57c6
    mov       word ptr es:[di + VBE_MGA_PCIBusDeviceIdFunctionId], bx;0x35
    call      ReadPCIBase0AndBase1      ;Offset 0x57d2
    ;mov       dword ptr es:[di + VBE_MGA_LinearFrameBuffer], eax;0x2d
    DB 026h, 066h, 089h, 045h, 02Dh
    ;mov       dword ptr es:[di + VBE_MGA_LinearControlAperture], ecx;0x31
    DB 026h, 066h, 089h, 04Dh, 031h
    call      GetMemoryIn64KBlocks      ;Offset 0x5846
    xchg      ah, al
    mov       word ptr es:[di + VBE_MGA_MemoryIn64KBBlocks], ax;0x2b
    mov       ax, 004fh
    pop       si
    pop       dx
    pop       cx
    pop       ds
    iret

AccessMGAIndexedDataRegister:           ;Offset 0x7757
    mov  bx, ax
    xchg bx, dx
    or   dh, dh
    jne  Label0x7764                    ;Offset 0x7764
    call WriteIndexedRegister           ;Offset 0x5b90
    jmp  Label0x7767                    ;Offset 0x7767
Label0x7764:                            ;Offset 0x7764
    call ReadIndexedRegister            ;Offset 0x5bab
Label0x7767:                            ;Offset 0x7767
    xchg bx, dx
    mov  ax, 004fh
    iret

AccessMGAIndirectRegister:              ;Offset 0x776d
    mov  bx, ax
    xchg bx, dx
    and  ah, 10h
    je   Label0x778e                    ;Offset 0x778e
    and  dh, 03h
    je   Label0x7789                    ;Offset 0x7789
    dec  dl
    je   Label0x7784                    ;Offset 0x7784
    call IndirectRegisterReadDWord      ;Offset 0x59a9
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x7784:                            ;Offset 0x7784
    call IndirectRegisterReadWord       ;Offset 0x598d
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x7789:                            ;Offset 0x7789
    call IndirectRegisterReadByte       ;Offset 0x5971
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x778e:                            ;Offset 0x778e
    and  dh, 03h
    je   Label0x77a1                    ;Offset 0x77a1
    dec  dl
    je   Label0x779c                    ;Offset 0x779c
    call IndirectRegisterWriteDWord     ;Offset 0x5958
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x779c:                            ;Offset 0x779c
    call IndirectRegisterWriteWord      ;Offset 0x593a
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x77a1:                            ;Offset 0x77a1
    call IndirectRegisterWriteByte      ;Offset 0x591c
    jmp  Label0x77a6                    ;Offset 0x77a6
Label0x77a6:                            ;Offset 0x77a6
    mov  ax, 004fh
    iret 

Label0x77aa:                            ;Offset 0x77aa
    push si
    mov  bx, ax
    add  si, offset PINS                ;Offset 0x77e0
    mov  cl, byte ptr cs:[si]
    mov  ax, 004fh
    pop  si
    iret

Label0x77b9:                            ;Offset 0x77b9
    or   ax, ax
    mov  ah, 01h
    je   Label0x77d1                    ;Offset 0x77d1
    mov  ax, 000ah
    mul  dx
    shl  edx, 10h
    mov  dx, ax
    mov  cl, 2dh
    call ConfigureAndSelectPLLSetC      ;Offset 0x6178
    xor  ah, ah
Label0x77d1:                            ;Offset 0x77d1
    mov  al, 4fh
    iret

Label0x77d4:                            ;Offset 0x77d4
    mov  bx, ax
    mov  cl, byte ptr cs:[si]
    mov  ax, 004fh
    iret

    xchg      bx, bx                    ;hello
    nop

;Offset 0x77e0
PINS                    DB 02Eh, 041h   ;signature ".A"
                        DB 040h         ;Size. Also used as offset from 0x77e0 to the protected mode table: 0x77e0 + 0x40 = 0x7820
                        DB 0FFh         ;??
                        DW 00200h       ;Structure version (2.0)
                        DW 0FFFFh       ;Date last programmed
                        DW 0FFFFh       ;Number of times programmed
                        DW 00000h       ;Flags
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh   ;ASCII Serial number, 3 letters + 5 digits
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh   ;8 bytes ???
                        DB 0FFh, 0FFh, 0FFh, 0FFh   ;4 bytes ASCII Card Version
                        DB 0FFh, 0FFh, 0FFh, 0FFh   ;DWORD PCB Revision
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x780b
Data0x780b              DB 0FFh

;Offset 0x780c
Data0x780c              DB 0FFh

;Offset 0x780d
Data0x780d              DB 0FFh

;Offset 0x780e
Data0x780e              DB 0FFh

;Offset 0x780f
Data0x780f              DB 0FFh

;Offset 0x7810
                        DB 0FFh, 0FFh, 0FFh
;Offset 0x7813
MemoryControlWaitState  DB 0FFh

;Offset 0x7814
Data0x7814              DB 0EEh

;Offset 0x7815
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 098h         ;Checksum

;Offset 0x7820
                        DW 00014h       ;00h    WORD    offset (within table) of protected-mode code for Function 5 (Set Window) : Points to 0x7834
                        DW 0003Bh       ;02h    WORD    offset of protected-mode code for Function 7 (Set Disp Start) : Points to 0x785b
                        DW 00099h       ;04h    WORD    offset of protected-mode code for Function 9 (Set Primary Palette) : Points to 0x78b9
                        DW 0000Ch       ;06h    WORD    offset (within table) of list of I/O ports and memory locations
                                        ;               for which I/O privilege may be required in protected mode (0000h if no list) : Points to 0x756c
                                        ;08h    var     code and optional port/memory list
                        DW 0010Ah       ;Size
                        DW 00000h       ;?

;Offset 0x782c
                        DW 003DEh       ;Port list
                        DW 003DFh
                        DW 0FFFFh
                        DW 0FFFFh       ;Memory list

;Can't have 16-bit tiny code and 32-bit flat code in the same file, so here is the data with code as comment:

                                        ;SetWindow32bit PROC NEAR                ;Offset 0x7834
DB 066h, 052h                           ;    push dx
DB 00Ah, 0FFh                           ;    or   bh, bh
DB 075h, 00Ch                           ;    jne  Label0x7846                    ;Offset 0x7846
DB 08Ah, 0E2h                           ;    mov  ah, dl
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 066h, 0EFh                           ;    out  dx, ax
DB 0EBh, 00Eh                           ;    jmp  Label0x7854                    ;Offset 0x7854
                                        ;Label0x7846:                            ;Offset 0x7846
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 08Ah, 0D0h                           ;    mov  dl, al
DB 032h, 0F6h                           ;    xor  dh, dh
                                        ;Label0x7854:                            ;Offset 0x7854
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004f
DB 0C3h                                 ;    ret
                                        ;SetWindow32bit ENDP

                                        ;SetDisplayStart32bit PROC NEAR          ;Offset 0x785b
DB 050h                                 ;    push eax
DB 066h, 052h                           ;    push dx
DB 066h, 08Bh, 0C2h                     ;    mov  ax, dx
DB 0C1h, 0C0h, 010h                     ;    rol  eax, 10h
DB 066h, 08Bh, 0C1h                     ;    mov  ax, cx
DB 066h, 050h                           ;    push ax
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 005h                           ;    mov  al, MGA_CRTCExt_HorVidHalfCount;0x5
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 00Ah, 0C0h                           ;    or   al, al
DB 066h, 058h                           ;    pop  ax
DB 074h, 002h                           ;    je   Label0x787b                    ;Offset 0x787b
DB 0D1h, 0E8h                           ;    shr  eax, 01h
                                        ;Label0x787b:                            ;Offset 0x787b
DB 050h                                 ;    push eax
DB 0F6h, 0C3h, 080h                     ;    test bl, 80h
DB 00Fh, 084h, 009h, 000h, 000h, 000h   ;    je   Label0x788e                    ;Offset 0x788e
DB 066h, 0BAh, 0DAh, 003h               ;    mov  dx, VGA_InputStatus1D          ;Port 0x3da
                                        ;Label0x7889:                            ;Offset 0x7889
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0FBh                           ;    je   Label0x7889                    ;Offset 0x7889
                                        ;Label0x788e:                            ;Offset 0x788e
DB 066h, 0BAh, 0D4h, 003h               ;    mov  dx, VGA_CRTControllerIndexD    ;Port 0x3d4
DB 0B0h, 00Dh                           ;    mov  al, VGA_CRTCIdx_StartAddrLow   ;0xd
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 066h, 058h                           ;    pop  ax
DB 0EEh                                 ;    out  dx, al
DB 066h, 04Ah                           ;    dec  dx
DB 0B0h, 00Ch                           ;    mov  al, VGA_CRTCIdx_StartAddrHigh  ;0xc
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 086h, 0C4h                           ;    xchg al, ah
DB 0EEh                                 ;    out  dx, al
DB 0B2h, 0DEh                           ;    mov  dl, MGA_CRTCExtensionIndex_lowbyte;Port 0x3de
DB 032h, 0C0h                           ;    xor  al, al                         ;MGA_CRTCExt_AddrGeneratorExt
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 066h, 058h                           ;    pop  ax
DB 086h, 0C4h                           ;    xchg al, ah
DB 0ECh                                 ;    in   al, dx
DB 024h, 0F0h                           ;    and  al, NOT MGA_CRTEXT0_StartAddress19_16;0xf0
DB 00Ah, 0C4h                           ;    or   al, ah
DB 0EEh                                 ;    out  dx, al
DB 066h, 05Ah                           ;    pop  dx
DB 058h                                 ;    pop  eax
DB 0C3h                                 ;    ret
                                        ;SetDisplayStart32bit ENDP

                                        ;SetPrimaryPalette32bit PROC NEAR        ;Offset 0x78b9
DB 080h, 0FBh, 002h                     ;    cmp  bl, 02h
DB 074h, 067h                           ;    je   Label0x7925                    ;Offset 0x7925
DB 080h, 0FBh, 003h                     ;    cmp  bl, 03h
DB 074h, 062h                           ;    je   Label0x7925                    ;Offset 0x7925
DB 066h, 053h                           ;    push bx
DB 066h, 051h                           ;    push cx
DB 066h, 052h                           ;    push dx
DB 057h                                 ;    push edi
DB 08Ah, 0C2h                           ;    mov  al, dl
DB 066h, 0BAh, 0C8h, 003h               ;    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
DB 0EEh                                 ;    out  dx, al
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 03Ch, 080h                           ;    cmp  al, 80h
DB 075h, 00Ch                           ;    jne  Label0x78e3                    ;Offset 0x78e3
DB 0B2h, 0DAh                           ;    mov  dl, VGA_FeatureControlWD_lowbyte;Port 0x3da
                                        ;Label0x78d9:                            ;Offset 0x78d9
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0FBh                           ;    jne  Label0x78d9                    ;Offset 0x78d9
                                        ;Label0x78de:                            ;Offset 0x78de
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0FBh                           ;    je   Label0x78de                    ;Offset 0x78de
                                        ;Label0x78e3:                            ;Offset 0x78e3
DB 0B2h, 0C9h                           ;    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 024h, 001h                           ;    and  al, 01h
DB 074h, 018h                           ;    je   Label0x7903                    ;Offset 0x7903
                                        ;Label0x78eb:                            ;Offset 0x78eb
DB 0ECh                                 ;    in   al, dx
DB 026h, 088h, 047h, 002h               ;    mov  byte ptr es:[edi + 02h], al
DB 0ECh                                 ;    in   al, dx
DB 026h, 088h, 047h, 001h               ;    mov  byte ptr es:[edi + 01h], al
DB 0ECh                                 ;    in   al, dx
DB 026h, 088h, 007h                     ;    mov  byte ptr es:[edi], al
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 066h, 049h                           ;    dec  cx
DB 075h, 0EAh                           ;    jne  Label0x78eb                    ;Offset 0x78eb
DB 0EBh, 016h                           ;    jmp  Label0x7919                    ;Offset 0x7919
                                        ;Label0x7903:                            ;Offset 0x7903
DB 026h, 08Ah, 047h, 002h               ;    mov  al, byte ptr es:[edi + 02h]
DB 0EEh                                 ;    out  dx, al
DB 026h, 08Ah, 047h, 001h               ;    mov  al, byte ptr es:[edi + 01h]
DB 0EEh                                 ;    out  dx, al
DB 026h, 08Ah, 007h                     ;    mov  al, byte ptr es:[edi]
DB 0EEh                                 ;    out  dx, al
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 047h                                 ;    inc  edi
DB 066h, 049h                           ;    dec  cx
DB 075h, 0EAh                           ;    jne  Label0x7903                    ;Offset 0x7903
                                        ;Label0x7919:                            ;Offset 0x7919
DB 05Fh                                 ;    pop  edi
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 059h                           ;    pop  cx
DB 066h, 05Bh                           ;    pop  bx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;Label0x7925:                            ;Offset 0x7925
DB 066h, 0B8h, 04Fh, 002h               ;    mov  ax, 024fh
DB 0C3h                                 ;    ret
                                        ;SetPrimaryPalette32bit ENDP
DB 000h

;Offset 0x792b
DB 6cdh dup (0FFh)

;Offset 0x7ff8
DB 000h, 000h                           ;Subsystem Vendor ID

;Offset 0x7ffa
DB 000h, 000h                           ;Subsystem ID

;Offset 0x7ffc
PINSPointer             DW offset PINS  ;Offset 0x7520

DB 0FFh

DB 01Bh
ROMEnd:
END
