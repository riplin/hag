;Matrox G400 AGP BIOS v2.1 b35 disassembly

;Resources:
;
;BIOS files: https://ftp.matrox.com/pub/mga/archive/bios/2003/setup257.exe (can be unzipped)
;
;DOS video card tests: https://gona.mactar.hu/DOS_TESTS/
;
;Display Data Channel:https://glenwing.github.io/docs/VESA-EDDC-1.2.pdf
;Display Data Channel:https://glenwing.github.io/docs/VESA-DDCCI-1.1.pdf
;VESA EDID:https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
;
;Matrox G400 Specification: https://bitsavers.org/pdf/matrox/G400SPEC_Jun1999.PDF
;
;ALI M1541 Specification: https://bitsavers.org/components/ali/M1541_M1542_Aladdin_V_Socket_7_North_Bridge.pdf
;
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT 897-21.asm
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
include g400.inc

;Offset in calltables. Not sure on the names yet.
MGA_DDCDisable0                         EQU 00h;0x00
MGA_DDCDisable1                         EQU 02h;0x02
MGA_DDCSet0                             EQU 04h;0x04
MGA_DDCSet1                             EQU 06h;0x06
MGA_DDCRead0                            EQU 08h;0x08
MGA_DDCRead1                            EQU 0ah;0x0a

PCI_VENDOR_Intel                        EQU 8086h;0x8086
PCI_DEVICE_Triton_430FX                 EQU 122dh;0x122d
PCI_VENDOR_ALI                          EQU 10b9h;0x10b9
PCI_DEVICE_M1541_Aladdin                EQU 1541h;0x1541
PCI_DEVICE_M5243_PCItoAGP_Controller    EQU 5243h;0x5243
PCI_VENDOR_Matrox                       EQU 102bh;0x102b
PCI_DEVICE_G400AGP                      EQU 0525h;0x0525

PCI_ACCESS_ReadByte                     EQU 08h;0x08
PCI_ACCESS_ReadWord                     EQU 09h;0x09
PCI_ACCESS_ReadDWord                    EQU 0ah;0x0a
PCI_ACCESS_WriteByte                    EQU 0bh;0x0b
PCI_ACCESS_WriteWord                    EQU 0ch;0x0c
PCI_ACCESS_WriteDWord                   EQU 0dh;0x0d

MGA_G400_Rev2                           EQU 02h;0x02
MGA_G400_Rev3                           EQU 03h;0x03

ALI_M1541_CPUtoPCIWriteBufferOption     EQU 086h;0x86
    ALI_M1541_CPUPCIWBO_PrgFrameBuffer  EQU 001h;0x01
ALI_M5243_CPUtoPCI66WriteBufferOption   EQU 086h;0x86
    ALI_M5243_CPUPCIWBO_VGAFixedFB      EQU 001h;0x01
ALI_M5243_PCI66ToMainMemPCI66ArbiterOpt EQU 088h;0x88
    ALI_M5243_ForcePCI66GATMode         EQU 008h;0x08

.MODEL tiny
.386
.CODE
ORG 0000h

ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

jmp entry                               ;Offset 0x80

;Offset 0x5
                        DB 021h         ;BIOS Version (2.1)
                        DB 034h, 003h, 010h, 004h, 050h, 00Bh, 0EEh, 030h, 08Ah, 00Bh
                        DB 094h, 00Bh, 0FFh, 0FFh, 000h, 000h, 000h, 000h

;Offset 0x18
                        DW offset PCIHeader
                        DW 0000h        ;PnP header pointer
                        DW 2000h        ;Unknown
                        DB 'IBM COMPATIBLE MATROX/Matrox G400 VGA/VBE BIOS (V2.1) b35 ', 000h

;Offset 0x59
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    nop

;Offset 0x60
PCIHeader               DB 'PCIR'       ;PCI header magic
                        DW PCI_VENDOR_Matrox;Vendor ID
                        DW PCI_DEVICE_G400AGP;Device ID
                        DW 0000h        ;Pointer to vital product data (0 = none)
                        DW 0018h        ;PCI data structure length
                        DB 00h          ;PCI data structure revision
                        DB 00h, 00h, 03h;PCI device class code
                        DW (ROMEnd - ROMStart)/512;Rom size in 512 byte blocks
                        DW 2321h        ;Revision level of code
                        DB 00h          ;Code type (0 = x86)
                        DB 80h          ;Last image indicator
                        DW 0000h        ;Reserved
                        DB '897-21', 000h
                        DB 0FFh

entry:                                  ;Offset 0x80
    push      bp
    xor       ax, ax
    mov       ds, ax
    mov       es, ax
    call      FindMGAG400               ;Offset 0x7a63
    mov       word ptr cs:[PCIBusDeviceIDFunctionID], bx;Offset 0x7ff2
    mov       di, PCI_Header_Byte_RevisionID;0x8
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    mov       byte ptr cs:[DeviceRevision], cl;Offset 0x7ff4
    call      RebootMGAG400             ;Offset 0x77c5
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call      Func0x783e                ;Offset 0x783e
    call      TurnScreenOff             ;Offset 0x2b1f
    mov       di, PCI_MGA_Option + 03h  ;0x43
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    or        cl, PCI_MGA_OPTION_MFIFONoHiPrioReqDsbl SHR 24;0x10
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    mov       di, PCI_Header_Byte_RevisionID;0x8
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    cmp       cl, MGA_G400_Rev2         ;0x2
    jne       Label0xdf                 ;Offset 0xdf
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    or        cl, 40h                   ;Undocumented flag
    mov       ch, cl
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6b6e
Label0xdf:                              ;Offset 0xdf
    call      Func0x733f                ;Offset 0x733f
    cmp       byte ptr cs:[Data0x7ff5], 00h;Offset 0x7ff5
    je        Label0x10c                ;Offset 0x10c
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    jne       Label0x100                ;Offset 0x100
    call      Func0x73a1                ;Offset 0x73a1
    or        byte ptr cs:[Data0x7ff1], ah;Offset 0x7ff1
    test      ah, 80h
    nop
    je        Label0x10c                ;Offset 0x10c
Label0x100:                             ;Offset 0x100
    call      Func0x7186                ;Offset 0x7186
    mov       cl, MGA_INDD_PanelMode    ;0x1f
    nop
    nop
    mov       ch, MGA_PANELMODE_ClientPanelLinkMode OR MGA_PANELMODE_HorPolarityActiveLow OR MGA_PANELMODE_VerPolarityActiveLow;0xc1
    call      WriteIndexedRegister      ;Offset 0x6b6e
Label0x10c:                             ;Offset 0x10c
    test      byte ptr cs:[Data0x7ff1], 81h;Offset 0x7ff1
    je        Label0x11b                ;Offset 0x11b
    mov       word ptr cs:[Data0x549b], 0ffffh;Offset 0x549b
Label0x11b:                             ;Offset 0x11b
    test      byte ptr cs:[Data0x7d15], 04h;Offset 0x7d15
    je        Label0x136                ;Offset 0x136
    mov       di, PCI_MGA_Option        ;0x40
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    or        cl, PCI_MGA_Opt_PLLSelP1SysP2Pix;0x40
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    nop
    call      AccessPCIRegister         ;Offset 0x69da
Label0x136:                             ;Offset 0x136
    call      Func0x7b6a                ;Offset 0x7b6a
    call      Func0x7a6f                ;Offset 0x7a6f
    push      bx
    mov       al, 40h
    call      SetupInterruptsTextModeAndBDA;Offset 0x1aa
    pop       bx
    call      CacheMemorySize           ;Offset 0x7c88
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x15b                ;Offset 0x15b
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x15b                ;Offset 0x15b
    call      Func0x6ffc                ;Offset 0x6ffc
    jmp       Label0x15e                ;Offset 0x15e
Label0x15b:                             ;Offset 0x15b
    call      Func0x6f9d                ;Offset 0x6f9d
Label0x15e:                             ;Offset 0x15e
    mov       di, PCI_MGA_Option2 + 03h ;0x53
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    or        cl, PCI_MGA_OPTION2_CPDECClockSelPLL SHR 24;0x1
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    nop
    call      AccessPCIRegister         ;Offset 0x69da
    cld
    mov       dx, VGA_CRTControllerIndexD;Port 0x3d4
    call      Func0x26ef                ;Offset 0x26ef
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       di, 2000h                 ;Offset 0x2000
    mov       cx, 0c000h
    xor       ax, ax
    rep stosb                           ;clear off-screen video memory
    mov       es, ax
    call      Func0x26f5                ;Offset 0x26f5
    mov       ax, BDA_DM_80x25_16_Color_Text;0x3 Set Text Mode
    int       10h
    call      Func0x6b4d                ;Offset 0x6b4d
    xor       ax, ax
    xor       si, si
    mov       cx, 7fffh                 ;Rom size - 1
ReadROMByteLoop:                        ;Offset 0x19b
    add       al, byte ptr cs:[si]      ;Recalculate the check byte at end of ROM
    inc       si
    loop      ReadROMByteLoop           ;Offset 0x19b
    neg       al
    mov       byte ptr cs:[si], al      ;Store new check byte
    pop       bp
    xor       ax, ax
    retf                                ;All set up. Return control to system.

SetupInterruptsTextModeAndBDA PROC NEAR ;Offset 0x1aa
    cli
    mov       word ptr ds:[INT_42_HandlerOfs], SYS_INT_10h_HandlerOfs;Offset 0x108 Offset 0xf065
    mov       word ptr ds:[INT_42_HandlerSeg], SYS_INT_10h_HandlerSeg;Offset 0x10a Segment 0xf000
    mov       word ptr ds:[INT_10_HandlerOfs], offset Int10Handler;Offset 0x40 Offset 0xbb0
    mov       word ptr ds:[INT_10_HandlerSeg], cs;Offset 0x42
    mov       word ptr ds:[INT_6D_HandlerOfs], offset Int6DHandler;Offset 0x1b4 Offset 0xbc0
    mov       word ptr ds:[INT_6D_HandlerSeg], cs;Offset 0x1b6
    sti       
    mov       word ptr ds:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x3500
    mov       word ptr ds:[INT_1F_HandlerSeg], cs;Offset 0x7e
    mov       word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3100
    mov       word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    mov       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0x487 0x60
    mov       ch, BDA_VDDA_VGA OR BDA_VDDA_LineMode400;0x11
    test      al, 40h
    jne       IsColor                   ;Offset 0x1ee
    or        ch, BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;0x6
IsColor:                                ;Offset 0x1ee
    lea       bx, [VideoOverrideTable1] ;Offset 0x334
    call      SetupVideoParameterControlBlock;Offset 0x203
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], ch;Offset 0x489
    call      SetCardAddressOrFallbackToSystemAdapter;Offset 0x220
    call      SetTextMode               ;Offset 0x284
    call      SetDisplayCombinationCode ;Offset 0x2d1
    ret
SetupInterruptsTextModeAndBDA ENDP

SetupVideoParameterControlBlock PROC NEAR;Offset 0x203
    mov       ax, cs
    cmp       ax, word ptr cs:[VideoOverrideTable2.VideoParameterTable.sgm];Offset 0x36c
    jne       IsShadowed                ;Offset 0x210
    ;add       bx, (SIZEOF VideoOverridePointerTable) + (SIZEOF SecondarySavePointerTable);0x36
    DB 081h, 0C3h, 036h, 000h           ;!!!!!!!!!!!!!! Assembles to different variant.
IsShadowed:                             ;Offset 0x210
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], bx;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], ax;Offset 0x4aa
    ret
SetupVideoParameterControlBlock ENDP

;Offset 0x218
InitMDA                 DB VGA_Misc_IOAddressSel_MDA, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Color, BDA_DM_80x25_16_Color_Text;0x0, 0xD4, 0x20, 0x03

;Offset 0x21c
InitCGA                 DB VGA_Misc_IOAddressSel_CGA, VGA_CRTControllerIndexB_lowbyte, BDA_DH_80x25Monochrome, BDA_DM_80x25_Monochrome_Text;0x1, 0xB4, 0x30, 0x07

SetCardAddressOrFallbackToSystemAdapter PROC NEAR;Offset 0x220
    lea       si, [InitMDA]             ;Offset 0x218
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    call      SetCardMode               ;Offset 0x23b   Attempt to set card to Monochrome
    jae       Success                   ;Offset 0x23a
    lea       si, [InitCGA]             ;Offset 0x21c
    and       byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487 0xfd
    call      SetCardMode               ;Offset 0x23b   Set card to Color
Success:                                ;Offset 0x23a
    ret
SetCardAddressOrFallbackToSystemAdapter ENDP

SetCardMode PROC NEAR                   ;Offset 0x23b
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
    call      TestRegister              ;Offset 0x274 - If non-selected CRTC register is still writeable, it's a VGA card
    jne       IsVGA                     ;Offset 0x273
    mov       ah, 1ah
    call      TestRegister              ;Offset 0x274 - Make sure
    jne       IsVGA                     ;Offset 0x273
    lodsb     byte ptr cs:[si]          ;Card is not VGA.
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    lodsb     byte ptr cs:[si]          ;Load fallback mode
    cbw                                 ;zero out ah
    int       42h                       ;Call System interrupt handler to set video mode.
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
IsVGA:                                  ;Offset 0x273
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
TestRegister PROC NEAR                  ;Offset 0x274
    in        al, dx                    ;Read register
    mov       bl, al                    ;Store result in bl
    mov       al, ah                    ;Move test value to al
    out       dx, al                    ;Write test value to register
    jmp       Delay                     ;Offset 0x27c   !!!!!!Speed sensitive!
Delay:                                  ;Offset 0x27c
    in        al, dx                    ;Read back test value
    ;cmp       al, ah                    ;Compare read test value to original test value
    DB 03Ah, 0C4h                       ;!!!!!! Misassembled
    mov       al, bl                    ;Move original register value to al
    out       dx, al                    ;Restore original value back to register
    stc                                 ;Set carry flag
    ret
TestRegister ENDP

SetTextMode PROC NEAR                   ;Offset 0x284
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    xor       al, al                    ;zero al
    out       dx, al                    ;select color 0
    inc       dx                        ;Port 0x3c9
    mov       cx, 300h                  ;768 values
LoopColors:                             ;Offset 0x28e
    out       dx, al                    ;Zero all colors
    loop      LoopColors                ;Offset 0x28e
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    cmp       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    mov       bl, al                    ;store modified vdda
    je        SetMonochrome             ;Offset 0x2a8
    mov       ax, (BDA_DH_80x25Color SHL 8) OR BDA_DM_80x25_16_Color_Text;0x2003
    mov       bh, BDA_EFBS_MDAHiResEnhanced_2;0x9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        SetColor                  ;Offset 0x2b2
SetMonochrome:                          ;Offset 0x2a8
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    mov       ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_DM_80x25_Monochrome_Text;0x3007
    mov       bh, BDA_EFBS_CGAMono80x25_2;0xb
SetColor:                               ;Offset 0x2b2
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

SetDisplayCombinationCode PROC NEAR     ;Offset 0x2d1
    mov   ax, BDA_DCC_CVGA SHL 8        ;0x800
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsColor                       ;Offset 0x2dd
    mov   ah, BDA_DCC_MVGA              ;0x7
IsColor:                                ;Offset 0x2dd
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    jne   IsVGA                         ;Offset 0x2f4
    inc   ax                            ;BDA_DCC_MDPA
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsVGA                         ;Offset 0x2f4
    inc   ax                            ;BDA_DCC_CGA
    call  CheckRegistersMemoryMapped    ;Offset 0x2fa
    jne   IsVGA                         ;Offset 0x2f4
    mov   al, BDA_DCC_PGC               ;0x6
IsVGA:                                  ;Offset 0x2f4
    mov   bx, ax
    call  SetDisplayCombinationCodeIndex;Offset 0x2b36
    ret
SetDisplayCombinationCode ENDP

;
;input:
;   -
;output
;   zero flag = 0 = failed, 1 = success
;destroys:
;   dx, si
CheckRegistersMemoryMapped PROC NEAR    ;Offset 0x2fa
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
MGAReadCRTCExtensionRegister PROC NEAR  ;Offset 0x312
    push  dx
    push  bx
    mov   bl, al
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, al
    inc   dx
    in    al, dx
    pushf
    cmp   bl, MGA_CRTCExt_RequesterControl;0x7
    jne   NotRequesterControl           ;Offset 0x324
    or    al, 0fh
NotRequesterControl:                    ;Offset 0x324
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
MGAWriteCRTCExtensionRegister PROC NEAR ;Offset 0x328
    push  dx
    xchg  al, ah
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, ax
    xchg  al, ah
    pop   dx
    ret
MGAWriteCRTCExtensionRegister ENDP

    nop                                 ;hello

;Offset 0x334
VideoOverrideTable1 VideoOverridePointerTable < <offset VideoParameters, 0C000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable1, 0C000h>, <00000h, 00000h>, <00000h, 00000h> >
SecondarySaveTable1 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0C000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >

VideoOverrideTable2 VideoOverridePointerTable < <offset VideoParameters, 0E000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable2, 0E000h>, <00000h, 00000h>, <00000h, 00000h> >
SecondarySaveTable2 SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0E000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h> >

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

;Offset 0x3c4
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

;Offset 0x3d4
Data0x3d4               DB BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2c
                        DB BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x28
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2d
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2a
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2e
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_GraphicsOperation;0x1e
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29

;Offset 0x3dc
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
    xchg bx, bx
    xchg bx, bx

;The assembler doesn't like all that data on one line, so I am just overlapping these to make struct accesses work.
;If anyone knows a cleaner way, please let me know
VideoParameters VideoParametersTable 29 dup(<>);Offset 0x3c0

ORG offset VideoParameters

;Offset 0x410
Data0x410               DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x450
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x490
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x4d0
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x510
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x550
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x590
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000101h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0C2h, 0FFh
                        DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

;Offset 0x5d0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000300h
                        DB 0A6h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0x610
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 07D00h
                        DD 006000F21h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x650
                        DB 000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 000000000h
                        DB 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x690
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6d0
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 062h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x710
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x750
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 006000F09h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x790
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x7d0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 000h, 05Eh, 06Eh, 08Bh, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x810
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
                        DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x850
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A2h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh

;Offset 0x890
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x8d0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x910
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x950
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x990
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x9d0
                        DB 028h
                        DB 018h
                        DB 010h
                        DW 00800h
                        DD 002000308h
                        DB 067h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa10
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 067h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa50
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 066h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0xa90
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0C3h, 0FFh
                        DB 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh

;Offset 0xad0
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xb10
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 00E000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0xb50
Int10CallTable          DW SetVideoMode;Offset 0x0c80                                       ;  0
                        DW SetTextModeCursorShape;Offset 0x1066                             ;  1
                        DW SetCursorPosition;Offset 0x10e2                                  ;  2
                        DW GetCursorPositionAndSize;Offset 0x1116                           ;  3
                        DW ReadLightPenPosition_SystemForward;Offset 0x1128                 ;  4
                        DW SelectActiveDisplayPage;Offset 0x1135                            ;  5
                        DW ScrollUpWindow;Offset 0x116b                                     ;  6
                        DW ScrollDownWindow;Offset 0x1390                                   ;  7
                        DW ReadCharacterAndAttributeAtCursorPosition;Offset 0x15cd          ;  8
                        DW WriteCharacterAndAttributeAtCursorPosition;Offset 0x179a         ;  9
                        DW WriteCharacterOnlyAtCursorPosition;Offset 0x1aa3                 ;  a
                        DW SetBorderColorOrPalette;Offset 0x1b04                            ;  b
                        DW WriteGraphicsPixel;Offset 0x1bad                                 ;  c
                        DW ReadGraphicsPixel;Offset 0x1c8a                                  ;  d
                        DW TeletypeOutput;Offset 0x1d40                                     ;  e
                        DW GetCurrentVideoMode;Offset 0x1dc7                                ;  f
                        DW RAMDACFunctions;Offset 0x1dd9                                    ; 10
                        DW TextFunctions;Offset 0x1fc9                                      ; 11
                        DW AlternateFunctions;Offset 0x20cd                                 ; 12
                        DW WriteString  ;Offset 0x2254                                      ; 13
                        DW NotImplemented ;Offset 0x22e0                                    ; 14
                        DW NotImplemented ;Offset 0x22e0                                    ; 15
                        DW NotImplemented ;Offset 0x22e0                                    ; 16
                        DW NotImplemented ;Offset 0x22e0                                    ; 17
                        DW NotImplemented ;Offset 0x22e0                                    ; 18
                        DW NotImplemented ;Offset 0x22e0                                    ; 19
                        DW DisplayCombinationCodeFunctions;Offset 0x22e1                    ; 1a
                        DW FunctionalityAndStateInfo;Offset 0x2328                          ; 1b
                        DW SaveRestoreVideoState;Offset 0x2434                              ; 1c

;Offset 0xb8a
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0C0h, 00Bh, 000h, 000h, 0FFh, 0FFh, 0A0h, 003h, 0C4h, 003h, 00Ch, 024h, 020h, 024h, 01Ah, 02Ah
                        DB 096h, 02Ah, 0CEh, 02Ch, 000h, 000h

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

Int10Handler:                           ;Offset 0xbb0
    cmp  ax, 4f10h
    jne  Label0xbba                     ;Offset 0xbba
    sti
    cld
    jmp  VESAPowerManagement            ;Offset 0x4b0c
Label0xbba:                             ;Offset 0xbba
    int  6dh
    iret

    xchg bx, bx                         ;hello
    nop

Int6DHandler:                           ;Offset 0xbc0
    sti
    cld
    cmp  ax, 4f10h
    jne  Label0xbca                     ;Offset 0xbca
    jmp  VESAPowerManagement            ;Offset 0x4b0c
Label0xbca:                             ;Offset 0xbca
    cmp  ax, 4f15h
    jne  Label0xbd2                     ;Offset 0xbd2
    jmp  VESADDC                        ;Offset 0x4c21
Label0xbd2:                             ;Offset 0xbd2
    cmp  ah, 0afh
    je   Label0xbdc                     ;Offset 0xbdc
    cmp  ah, 0bdh
    jne  Label0xbdd                     ;Offset 0xbdd
Label0xbdc:                             ;Offset 0xbdc
    iret
Label0xbdd:                             ;Offset 0xbdd
    cmp  ax, 0bf00h
    jne  Label0xbf2                     ;Offset 0xbf2
    mov  bx, cs
    mov  cx, 5d1dh
    mov  ax, 00bfh
    mov  ah, byte ptr cs:[Data0x7ff1]   ;Offset 0x7ff1
    mov  al, 0bfh
    iret
Label0xbf2:                             ;Offset 0xbf2
    cmp  ah, 01ch
    ja   Label0xc4a                     ;Offset 0xc4a
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
    jl   Label0xc41                     ;Offset 0xc41
    cmp  ah, 0eh
    je   Label0xc1c                     ;Offset 0xc1c
    cmp  ah, 06h
    jl   Label0xc41                     ;Offset 0xc41
    cmp  ah, 0ah
    jg   Label0xc41                     ;Offset 0xc41
Label0xc1c:                             ;Offset 0xc1c
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
    call word ptr cs:[si + Int10CallTable];Offset 0xb50
    pop  di
    push ax
    push dx
    mov  ax, di
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    out  dx, ax
    pop  dx
    pop  ax
    jmp  Label0xc46                     ;Offset 0xc46
Label0xc41:                             ;Offset 0xc41
    call word ptr cs:[si + Int10CallTable];Offset 0xb50
Label0xc46:                             ;Offset 0xc46
    pop  di
    pop  si
    pop  ds
    iret
Label0xc4a:                             ;Offset 0xc4a
    cmp  ah, 04fh
    jne  Label0xc52                     ;Offset 0xc52
    jmp  VESAHandler                    ;Offset 0x57eb
Label0xc52:                             ;Offset 0xc52
    int  42h
    iret

Func0xc55 PROC NEAR                     ;Offset 0xc55
    push ax
    push bx
    mov  ax, cs
    mov  bx, ds
    cmp  ax, bx
    pop  bx
    pop  ax
    jne  Label0xc7e                     ;Offset 0xc7e
    cmp  cl, 0eh
    jne  Label0xc7e                     ;Offset 0xc7e
    cmp  al, 67h
    je   Label0xc7c                     ;Offset 0xc7c
    cmp  al, 6ah
    je   Label0xc7c                     ;Offset 0xc7c
    cmp  al, 79h
    je   Label0xc7c                     ;Offset 0xc7c
    cmp  al, 71h
    je   Label0xc7c                     ;Offset 0xc7c
    cmp  al, 70h
    je   Label0xc7c                     ;Offset 0xc7c
    cmp  al, 98h
Label0xc7c:                             ;Offset 0xc7c
    clc
    ret
Label0xc7e:                             ;Offset 0xc7e
    stc
    ret
Func0xc55 ENDP

SetVideoMode PROC NEAR                  ;Offset 0x0c80
    push ax
    mov  ah, al
    mov  al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and  al, BDA_VDDA_Reserved          ;0x20
    je   Label0xc8d                     ;Offset 0xc8d
    or   ah, BDA_DM_DONT_CLEAR_SCREEN   ;0x80
Label0xc8d:                             ;Offset 0xc8d
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shl  ax, 01h
    xchg al, ah
    rcr  ax, 01h
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    ja   Label0xcfa                     ;Offset 0xcfa
    push ax
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x7
    je   Label0xcac                     ;Offset 0xcac
    cmp  al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je   Label0xcac                     ;Offset 0xcac
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Color          ;0x20
    jmp  Label0xcb3                     ;Offset 0xcb3
Label0xcac:                             ;Offset 0xcac
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Monochrome     ;0x30
Label0xcb3:                             ;Offset 0xcb3
    mov  byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    pop  ax
    call Func0xe7e                      ;Offset 0xe7e
    mov  word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3100
    mov  word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    and  ah, 0f3h
    test byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je   Label0xcd0                     ;Offset 0xcd0
    jp   Func0xd43                      ;Offset 0xd43
Label0xcd0:                             ;Offset 0xcd0
    test ah, 02h
    jne  Label0xce4                     ;Offset 0xce4
    mov  si, 03d4h                      ;Port 0x3d4
    cmp  al, 07h
    je   Label0xce0                     ;Offset 0xce0
    cmp  al, 0fh
    jne  Func0xd58                      ;Offset 0xd58
Label0xce0:                             ;Offset 0xce0
    mov  al, 03h
    jmp  Func0xd55                      ;Offset 0xd55
Label0xce4:                             ;Offset 0xce4
    cmp  al, 02h
    jb   Label0xcef                     ;Offset 0xcef
    cmp  al, 03h
    ja   Label0xcef                     ;Offset 0xcef
    or   ah, 04h
Label0xcef:                             ;Offset 0xcef
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485
    call Func0xd33                      ;Offset 0xd33
Label0xcf8:                             ;Offset 0xcf8
    pop  ax
    ret
Label0xcfa:                             ;Offset 0xcfa
    cmp  al, 6ah
    jne  Label0xd03                     ;Offset 0xd03
    mov  ax, 0102h
    jmp  Label0xd17                     ;Offset 0xd17
Label0xd03:                             ;Offset 0xd03
    mov  ah, al
    and  ah, 0c0h
    or   ah, 01h
    and  al, 3fh
    cmp  al, 20h
    jb   Label0xcf8                     ;Offset 0xcf8
    cmp  al, 3fh
    ja   Label0xcf8                     ;Offset 0xcf8
    sub  al, 20h
Label0xd17:                             ;Offset 0xd17
    push bx
    mov  bx, ax
    mov  ax, 4f02h
    int  10h
    pop  bx
    pop  ax
    ret
SetVideoMode ENDP

Func0xd22 PROC NEAR                     ;Offset 0xd22
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 0xe
    call Func0xd33                      ;Offset 0xd33
    mov  word ptr ds:[BDA_CursorEndStartScanLine], 0b0ch;Offset 0x460
    pop  ax
    ret
Func0xd22 ENDP

Func0xd33 PROC NEAR                     ;Offset 0xd33
    mov  byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484
    or   ah, BDA_VMO_Inactive           ;0x8
    mov  byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    cbw
    int   42h
    ret
Func0xd33 ENDP

Func0xd43 PROC NEAR                     ;Offset 0xd43
    test      ah, 02h
    je        Func0xd22                 ;Offset 0xd22
    mov       si, 03b4h                 ;Port/Offset 0x3b4
    cmp       al, 07h
    je        Func0xd58                 ;Offset 0xd58
    cmp       al, 0fh
    je        Func0xd58                 ;Offset 0xd58
    mov       al, 07h
Func0xd43 ENDP
;continue!
Func0xd55 PROC NEAR                     ;Offset 0xd55
    and       ah, 7fh
Func0xd55 ENDP
;continue!
Func0xd58 PROC NEAR                     ;Offset 0xd58
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    push      ax
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x66f6
    je        Label0xd69                ;Offset 0xd69
    mov       ax, 0000h
    call      Func0x5427                ;Offset 0x5427
Label0xd69:                             ;Offset 0xd69
    pop       ax
    push      ax
    call      TurnScreenOff             ;Offset 0x2b1f
    mov       dx, ax
    pop       ax
    push      dx
    mov       byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov       byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    mov       word ptr ds:[BDA_VideoBaseIOPort], si;Offset 0x463
    push      ax
    push      dx
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_Misc      ;0x3
    out       dx, al
    in        al, dx
    mov       ah, 1eh
    test      al, MGA_CRTCEXT3_MGAModeEnable;0x80
    jne       Label0xd8e                ;Offset 0xd8e
    mov       ah, 00h
Label0xd8e:                             ;Offset 0xd8e
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    pop       dx
    pop       ax
    call      Func0xe7b                 ;Offset 0xe7b
    xor       ax, ax
    mov       es, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    lea       di, ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       cx, 0008h
    rep stosw
    call      Func0x29fa                ;Offset 0x29fa
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
    mov       word ptr ds:[BDA_CursorEndScanLine], ax;Offset 0x460
    call      Func0x284f                ;Offset 0x284f
    mov       dx, VGA_AttributeControllerIndex;0x3c0
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
    call      Func0xf0c                 ;Offset 0xf0c
    call      ClearScreen               ;Offset 0x26b1
    lea       si, [Func0xf2b]           ;Offset 0xf2b
    call      Func0x2ab8                ;Offset 0x2ab8
    je        Label0xdf9                ;Offset 0xdf9
    lea       si, [Func0xf56]           ;Offset 0xf56
Label0xdf9:                             ;Offset 0xdf9
    call      si
    call      Func0xf79                 ;Offset 0xf79
    call      Func0x2adf                ;Offset 0x2adf
    pop       ax
    call      TurnScreenOn              ;Offset 0x2b1b
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0xe23                ;Offset 0xe23
    mov       ah, al
    lea       bx, [Data0x3d4]           ;Offset 0x3d4
    xlatb     cs:[bx]
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    mov       al, 30h
    cmp       ah, BDA_DM_640x200_BW_Graphics;0x6
    jne       Label0xe20                ;Offset 0xe20
    mov       al, 3fh
Label0xe20:                             ;Offset 0xe20
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
Label0xe23:                             ;Offset 0xe23
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_Reserved     ;0x20
    je        Label0xe2f                ;Offset 0xe2f
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_Reserved;Offset 0x489 0xdf
Label0xe2f:                             ;Offset 0xe2f
    push      ax
    push      bx
    push      cx
    push      edx
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x66f6
    je        Label0xe6f                ;Offset 0xe6f
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0xe6f                ;Offset 0xe6f
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, NOT MGA_MISCCTRL_MAFC_MASK;0xf9
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0xe5b                ;Offset 0xe5b
    and       cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_Enable);0xf8
Label0xe5b:                             ;Offset 0xe5b
    or        cl, MGA_MISCCTRL_MAFC_PanelLinkMode;0x4
    mov       ch, cl
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       al, 01h
    call      Func0x5427                ;Offset 0x5427
    call      Func0x6b4d                ;Offset 0x6b4d
Label0xe6f:                             ;Offset 0xe6f
    pop       edx
    pop       cx
    pop       bx
    pop       ax
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0xd58 ENDP

Func0xe7b PROC NEAR                     ;Offset 0xe7b
    call      Func0x6256                ;Offset 0x6256
Func0xe7b ENDP
;continue!
Func0xe7e PROC NEAR                     ;Offset 0xe7e
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    je        Label0xf01                ;Offset 0xf01
    cmp       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je        Label0xf01                ;Offset 0xf01
    push      bx
    push      dx
    lea       si, [Data0xf02]           ;Offset 0xf02
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0xe9d                ;Offset 0xe9d
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0xe9d                ;Offset 0xe9d
    ;add       si, 0005h
    DB 81h, 0C6h, 05h, 00h              ;!!!!!!!!!!!!! misassembly
Label0xe9d:                             ;Offset 0xe9d
    xchg      ax, bx
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       al, byte ptr cs:[si + 01h]
    je        Label0xefe                ;Offset 0xefe
    mov       ah, bl
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    mov       bl, BDA_DM_80x25_Monochrome_Text;0x7
    cmp       al, byte ptr cs:[si + 02h]
    je        Label0xefe                ;Offset 0xefe
    mov       bl, BDA_DM_80x25_16_Color_Text;0x3
    cmp       al, byte ptr cs:[si + 03h]
    js        Label0xefe                ;Offset 0xefe
    mov       bl, ah
    and       bh, 0fdh
    or        bh, byte ptr cs:[si + 04h]
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_AdapterTypeMask;0xf
    sub       al, BDA_EFBS_MDAColor40x25_2;0x6
    cmp       al, BDA_EFBS_CGAMono80x25 ;0x5
    ja        Label0xefe                ;Offset 0xefe
    sub       al, BDA_EFBS_MDAHiResEnhanced;0x3
    cbw
    mov       al, BDA_EFBS_CGAMono80x25_2;0xb
    jbe       Label0xedb                ;Offset 0xedb
    mov       al, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0xedb:                             ;Offset 0xedb
    mov       dl, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    mov       dh, dl
    and       dx, 807fh
    rol       dh, 01h
    and       dh, byte ptr cs:[si]
    sub       al, dh
    and       ah, byte ptr cs:[si]
    or        dl, ah
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0x488
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], dl;Offset 0x489
Label0xefe:                             ;Offset 0xefe
    xchg      ax, bx
    pop       dx
    pop       bx
Label0xf01:                             ;Offset 0xf01
    ret
Func0xe7e ENDP

;Offset 0xf02
Data0xf02               DB 080h, VGA_CRTControllerIndexB_lowbyte, 0FFh, BDA_DH_80x25Monochrome, 002h;0x80 0xB4 0xFF 0x30 0x02

;Offset 0xf07
Data0xf07               DB 001h, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Monochrome, 000h, 000h;0x01 0xD4 0x30 0x00 0x00

Func0xf0c PROC NEAR                     ;Offset 0xf0c
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   PaletteLoadingDisabled        ;Offset 0xf2a
    push  si
    xor   ax, ax
    mov   cx, 0010h
    call  Func0x28cf                    ;Offset 0x28cf
    inc   ax
    inc   si
    mov   cl, 0001h
    call  Func0x28cf                    ;Offset 0x28cf
    pop   si
    call  Func0x2915                    ;Offset 0x2915
    call  Func0x292a                    ;Offset 0x292a
PaletteLoadingDisabled:                 ;Offset 0xf2a
    ret
Func0xf0c ENDP

Func0xf2b PROC NEAR                     ;Offset 0xf2b
    call  Func0x26ef                    ;Offset 0x26ef
    call  GetNumberOfActiveScanlines    ;Offset 0x2a6a
    dec   al
    mov   ax, 0002h
    js    Label0xf47                    ;Offset 0xf47
    mov   al, 03h
    jne   Label0xf45                    ;Offset 0xf45
    mov   al, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne   Label0xf47                    ;Offset 0xf47
Label0xf45:                             ;Offset 0xf45
    mov   ah, 80h
Label0xf47:                             ;Offset 0xf47
    mov   bl, ah
    call  Func0x2730                    ;Offset 0x2730
    call  Func0xff4                     ;Offset 0xff4
    call  Func0x1027                    ;Offset 0x1027
    call  Func0x26f5                    ;Offset 0x26f5
    ret
Func0xf2b ENDP

Func0xf56 PROC NEAR                     ;Offset 0xf56
    call  GetNumberOfActiveScanlines    ;Offset 0x2a6a
    dec   al
    js    Label0xf70                    ;Offset 0xf70
    lea   ax, [Font8x16]                ;Offset 0x3900
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    mov   ax, cs
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
    jne   Label0xf70                    ;Offset 0xf70
    mov   ax, 0bd70h
    int   6dh
Label0xf70:                             ;Offset 0xf70
    xor   ax, ax
    mov   word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call  Func0xfd0                     ;Offset 0xfd0
    ret
Func0xf56 ENDP

Func0xf79 PROC NEAR                     ;Offset 0xf79
    mov   al, BDA_VPCB_PaletterProfileTable;0xa
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    Label0xfcf                    ;Offset 0xfcf
    mov   al, 14h
    call  Func0x2acb                    ;Offset 0x2acb
    jne   Label0xfcf                    ;Offset 0xfcf
    lodsb byte ptr es:[si]
    or    al, al
    je    Label0xf9d                    ;Offset 0xf9d
    mov   ax, 1420h
    js    Label0xf95                    ;Offset 0xf95
    mov   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
Label0xf95:                             ;Offset 0xf95
    dec   ax
    xchg  al, ah
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    out   dx, ax
Label0xf9d:                             ;Offset 0xf9d
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   Label0xfcf                    ;Offset 0xfcf
    push  es
    push  si
    add   si, 0003h
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0xfb8                    ;Offset 0xfb8
    mov   ah, 10h
    call  Func0x28d1                    ;Offset 0x28d1
Label0xfb8:                             ;Offset 0xfb8
    pop   si
    pop   es
    add   si, 000bh
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0xfcf                    ;Offset 0xfcf
    mov   dx, VGA_DACWriteIndex         ;Port 0x3c8
    out   dx, al
    inc   dx
    call  Func0x29a9                    ;Offset 0x29a9
Label0xfcf:                             ;Offset 0xfcf
    ret
Func0xf79 ENDP

Func0xfd0 PROC NEAR                     ;Offset 0xfd0
    mov   al, 0ch
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    Label0xff3                    ;Offset 0xff3
    mov   al, 07h
    call  Func0x2acb                    ;Offset 0x2acb
    jne   Label0xff3                    ;Offset 0xff3
    lodsb byte ptr es:[si]
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsw word ptr es:[si]
    mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offest 0x10e
Label0xff3:                             ;Offset 0xff3
    ret
Func0xfd0 ENDP

Func0xff4 PROC NEAR                     ;Offset 0xff4
    mov   al, 08h
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    Label0x1026                   ;Offset 0x1026
    mov   al, 0bh
    call  Func0x2acb                    ;Offset 0x2acb
    jne   Label0x1026                   ;Offset 0x1026
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
    call  Func0x2753                    ;Offset 0x2753
    call  Func0x27d2                    ;Offset 0x27d2
    pop   ax
    cmp   al, 0ffh
    je    Label0x1026                   ;Offset 0x1026
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x1026:                            ;Offset 0x1026
    ret
Func0xff4 ENDP

Func0x1027 PROC NEAR                    ;Offset 0x1027
    mov   al, BDA_VPCB_SecondaryCharSetOverride;0x6
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    Label0x1065                   ;Offset 0x1065
    mov   al, 07h
    call  Func0x2acb                    ;Offset 0x2acb
    jne   Label0x1065                   ;Offset 0x1065
    lodsw word ptr es:[si]
    cmp   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    jne   Label0x1065                   ;Offset 0x1065
    push  ax
    xchg  al, ah
    and   al, 7fh
    xchg  ax, bx
    inc   si
    les   bp, es:[si]
    xor   dx, dx
    mov   cx, 0100h
    call  Func0x2753                    ;Offset 0x2753
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
Label0x1065:                            ;Offset 0x1065
    ret
Func0x1027 ENDP

SetTextModeCursorShape PROC NEAR        ;Offset 0x1066
    push      dx
    mov       ax, cx
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x108f               ;Offset 0x108f
    and       ah, 60h
    cmp       ah, 20h
    mov       ax, 1e00h
    je        Label0x108f               ;Offset 0x108f
    mov       ax, cx
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_CursorEmulationEnabled;Offset 0x487 0x1
    jne       Label0x108f               ;Offset 0x108f
    call      Func0x1094                ;Offset 0x1094
    call      SetCursorEndStart         ;Offset 0x26a2
    pop       dx
    ret
Label0x108f:                            ;Offset 0x108f
    call      SetCursorEndStart         ;Offset 0x26a2
    pop       dx
    ret
SetTextModeCursorShape ENDP

Func0x1094 PROC NEAR                    ;Offset 0x1094
    call      Func0x2ab8                ;Offset 0x2ab8
    jne       Label0x10e1               ;Offset 0x10e1
    test      ax, 0e0e0h
    jne       Label0x10e1               ;Offset 0x10e1
    mov       dl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       dx
    ;cmp       ah, al
    DB 3Ah, 0E0h                        ;!!!!!!!! Misassembly
    ja        Label0x10da               ;Offset 0x10da
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    ja        Label0x10b7               ;Offset 0x10b7
    ;cmp       al, dl
    DB 3Ah, 0C2h                        ;!!!!!!!! Misassembly
    ja        Label0x10b7               ;Offset 0x10b7
    je        Label0x10e1               ;Offset 0x10e1
    dec       dx
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    je        Label0x10e1               ;Offset 0x10e1
    inc       dx
Label0x10b7:                            ;Offset 0x10b7
    cmp       al, 03h
    jbe       Label0x10e1               ;Offset 0x10e1
    sub       al, ah
    cmp       al, 02h
    mov       al, dl
    jbe       Label0x10ce               ;Offset 0x10ce
    cmp       ah, 02h
    jbe       Label0x10e1               ;Offset 0x10e1
    inc       dx
    shr       dl, 01h
    mov       ah, dl
    ret
Label0x10ce:                            ;Offset 0x10ce
    add       ah, dl
    sub       ah, cl
    cmp       al, 0dh
    jb        Label0x10e1               ;Offset 0x10e1
    sub       ax, 0101h
    ret
Label0x10da:                            ;Offset 0x10da
    cmp       al, 00h
    je        Label0x10e1               ;Offset 0x10e1
    mov       dh, al
    xchg      ax, dx
Label0x10e1:                            ;Offset 0x10e1
    ret
Func0x1094 ENDP

SetCursorPosition PROC NEAR             ;Offset 0x10e2
    push      ax
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       word ptr [si + DBA_CursorPositionPage0], dx;Offset 0x450
    cmp       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    jne       Func0x1114                ;Offset 0x1114
    push      dx
    mov       ax, dx
    jmp       Func0x10fc                ;Offset 0x10fc
SetCursorPosition ENDP

Func0x10f9 PROC NEAR                    ;Offset 0x10f9
    pop       dx
    push      ax
    push      dx
Func0x10f9 ENDP
;continue!
Func0x10fc PROC NEAR                    ;Offset 0x10fc
    call      Func0x265a                ;Offset 0x265a
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
Func0x10fc ENDP
;continue!
Func0x1114 PROC NEAR                    ;Offset 0x1114
    pop       ax
    ret
Func0x1114 ENDP

GetCursorPositionAndSize PROC NEAR      ;Offset 0x1116
    push      ax
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    mov       cx, word ptr ds:[BDA_CursorEndStartScanLine];Offset 0x460
    pop       ax
    ret
GetCursorPositionAndSize ENDP

ReadLightPenPosition_SystemForward PROC NEAR;Offset 0x1128
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x1132               ;Offset 0x1132
    xor       ah, ah
    ret
Label0x1132:                            ;Offset 0x1132
    int       42h
    ret
ReadLightPenPosition_SystemForward ENDP

SelectActiveDisplayPage PROC NEAR       ;Offset 0x1135
    push      dx
    cbw
    mov       si, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, 07h
    pop       ax
    ja        Label0x1151               ;Offset 0x1151
    shr       ax, 01h
Label0x1151:                            ;Offset 0x1151
    call      SetStartAddress           ;Offset 0x115c
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Func0x10f9                ;Offset 0x10f9
SelectActiveDisplayPage ENDP

SetStartAddress PROC NEAR               ;Offset 0x115c
    push      ax
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       al, VGA_CRTCIdx_StartAddrHigh;0xc
    out       dx, ax
    pop       ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_StartAddrLow;0xd
    out       dx, ax
    ret
SetStartAddress ENDP

ScrollUpWindow PROC NEAR                ;Offset 0x116b
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
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x119e               ;Offset 0x119e
    ja        Label0x1193               ;Offset 0x1193
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x11a7               ;Offset 0x11a7
Label0x118d:                            ;Offset 0x118d
    jmp       Label0x1225               ;Offset 0x1225
Label0x1190:                            ;Offset 0x1190
    jmp       Label0x1313               ;Offset 0x1313
Label0x1193:                            ;Offset 0x1193
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x1208               ;Offset 0x1208
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x1190               ;Offset 0x1190
    jmp       Label0x1298               ;Offset 0x1298
Label0x119e:                            ;Offset 0x119e
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x118d               ;Offset 0x118d
Label0x11a7:                            ;Offset 0x11a7
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x265a                ;Offset 0x265a
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
    jne       Label0x1212               ;Offset 0x1212
Label0x11d8:                            ;Offset 0x11d8
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x120e               ;Offset 0x120e
Label0x11e0:                            ;Offset 0x11e0
    mov       cx, dx
    rep movsw
    add       si, bp
    add       di, bp
    dec       ax
    jne       Label0x11e0               ;Offset 0x11e0
Label0x11eb:                            ;Offset 0x11eb
    xchg      ah, bh
    mov       al, 20h
Label0x11ef:                            ;Offset 0x11ef
    mov       cx, dx
    rep stosw
    add       di, bp
    dec       bx
    jne       Label0x11ef               ;Offset 0x11ef
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x1208               ;Offset 0x1208
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1208:                            ;Offset 0x1208
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x120e:                            ;Offset 0x120e
    mov       bl, al
    jmp       Label0x11eb               ;Offset 0x11eb
Label0x1212:                            ;Offset 0x1212
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x1217:                            ;Offset 0x1217
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x1217               ;Offset 0x1217
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x11d8               ;Offset 0x11d8
Label0x1225:                            ;Offset 0x1225
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    shl       ah, 01h
    shl       ah, 01h
    xchg      ax, cx
    call      Func0x2668                ;Offset 0x2668
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x123f               ;Offset 0x123f
    shl       ax, 01h
    shl       cl, 01h
Label0x123f:
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
    je        Label0x1294               ;Offset 0x1294
Label0x1261:                            ;Offset 0x1261
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 1fb0h]     ;Offset 0x1fb0
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       ax
    jne       Label0x1261               ;Offset 0x1261
Label0x127c:                            ;Offset 0x127c
    xchg      al, bh
Label0x127e:                            ;Offset 0x127e
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       bx
    jne       Label0x127e               ;Offset 0x127e
    jmp       Label0x1208               ;Offset 0x1208
Label0x1294:                            ;Offset 0x1294
    xchg      bl, al
    jmp       Label0x127c               ;Offset 0x127c
Label0x1298:                            ;Offset 0x1298
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2678                ;Offset 0x2678
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
    je        Label0x130e               ;Offset 0x130e
Label0x12df:                            ;Offset 0x12df
    mov       cx, dx
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x12df               ;Offset 0x12df
    pop       bx
Label0x12eb:                            ;Offset 0x12eb
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x12fb:                            ;Offset 0x12fb
    mov       cx, si
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x12fb               ;Offset 0x12fb
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1208               ;Offset 0x1208
Label0x130e:                            ;Offset 0x130e
    pop       bx
    mov       bl, ch
    jmp       Label0x12eb               ;Offset 0x12eb
Label0x1313:                            ;Offset 0x1313
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x268a                ;Offset 0x268a
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
    je        Label0x138a               ;Offset 0x138a
Label0x1362:                            ;Offset 0x1362
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x1362               ;Offset 0x1362
    pop       bx
Label0x1372:                            ;Offset 0x1372
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x137a:                            ;Offset 0x137a
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x137a               ;Offset 0x137a
    jmp       Label0x1208               ;Offset 0x1208
Label0x138a:                            ;Offset 0x138a
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x1372               ;Offset 0x1372
ScrollUpWindow ENDP

ScrollDownWindow PROC NEAR              ;Offset 0x1390
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
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, 07h
    jb        Label0x13c4               ;Offset 0x13c4
    ja        Label0x13b9               ;Offset 0x13b9
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x13cd               ;Offset 0x13cd
Label0x13b3:                            ;Offset 0x13b3
    jmp       Label0x144d               ;Offset 0x144d
Label0x13b6:                            ;Offset 0x13b6
    jmp       Label0x1547               ;Offset 0x1547
Label0x13b9:                            ;Offset 0x13b9
    cmp       al, 0dh
    jb        Label0x1430               ;Offset 0x1430
    cmp       al, 13h
    je        Label0x13b6               ;Offset 0x13b6
    jmp       Label0x14c4               ;Offset 0x14c4
Label0x13c4:                            ;Offset 0x13c4
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, 03h
    ja        Label0x13b3               ;Offset 0x13b3
Label0x13cd:                            ;Offset 0x13cd
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    call      Func0x265a                ;Offset 0x265a
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
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_EFBS_CGAMono40x25;Offset 0x487 0x4
    jne       Label0x143a               ;Offset 0x143a
Label0x1400:                            ;Offset 0x1400
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1436               ;Offset 0x1436
Label0x1408:                            ;Offset 0x1408
    mov       cx, dx
    rep movsw
    sub       si, bp
    sub       di, bp
    dec       ax
    jne       Label0x1408               ;Offset 0x1408
Label0x1413:                            ;Offset 0x1413
    xchg      ah, bh
    mov       al, 20h
Label0x1417:                            ;Offset 0x1417
    mov       cx, dx
    rep stosw
    sub       di, bp
    dec       bx
    jne       Label0x1417               ;Offset 0x1417
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449
    je        Label0x1430               ;Offset 0x1430
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1430:                            ;Offset 0x1430
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x1436:
    mov       bl, al
    jmp       Label0x1413               ;Offset 0x1413
Label0x143a:                            ;Offset 0x143a
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x143f:                            ;Offset 0x143f
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x143f               ;Offset 0x143f
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x1400               ;Offset 0x1400
Label0x144d:                            ;Offset 0x144d
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    shl       dh, 01h
    shl       dh, 01h
    mov       cx, dx
    call      Func0x2668                ;Offset 0x2668
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x146a               ;Offset 0x146a
    shl       ax, 01h
    inc       ax
    shl       cl, 01h
Label0x146a:                            ;Offset 0x146a
    add       ax, 0f0h
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
    je        Label0x14c0               ;Offset 0x14c0
Label0x148d:                            ;Offset 0x148d
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 2050h]     ;Offset 0x2050
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       ax
    jne       Label0x148d               ;Offset 0x148d
Label0x14a8:                            ;Offset 0x14a8
    xchg      al, bh
Label0x14aa:                            ;Offset 0x14aa
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       bx
    jne       Label0x14aa               ;Offset 0x14aa
    jmp       Label0x1430               ;Offset 0x1430
Label0x14c0:                            ;Offset 0x14c0
    xchg      bl, al
    jmp       Label0x14a8               ;Offset 0x14a8
Label0x14c4:                            ;Offset 0x14c4
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x2678                ;Offset 0x2678
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
    je        Label0x1542               ;Offset 0x1542
Label0x1513:                            ;Offset 0x1513
    mov       cx, dx
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x1513               ;Offset 0x1513
    pop       bx
Label0x151f:                            ;Offset 0x151f
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x152f:                            ;Offset 0x152f
    mov       cx, si
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x152f               ;Offset 0x152f
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1430               ;Offset 0x1430
Label0x1542:                            ;Offset 0x1542
    pop       bx
    mov       bl, ch
    jmp       Label0x151f               ;Offset 0x151f
Label0x1547:                            ;Offset 0x1547
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x268a                ;Offset 0x268a
    ;add       ax, 7
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
    je        Label0x15c7               ;Offset 0x15c7
Label0x159f:                            ;Offset 0x159f
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x159f               ;Offset 0x159f
    pop       bx
Label0x15af:                            ;Offset 0x15af
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x15b7:                            ;Offset 0x15b7
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x15b7               ;Offset 0x15b7
    jmp       Label0x1430               ;Offset 0x1430
Label0x15c7:                            ;Offset 0x15c7
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x15af               ;Offset 0x15af
ScrollDownWindow ENDP

ReadCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x15cd
    push       dx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x60a3               ;Offset 0x60a3
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x15e1              ;Offset 0x15e1
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Label0x161a              ;Offset 0x161a
    mov        dh, 0b0h                 ;Segment 0xb0
Label0x15e1:                            ;Offset 0x15e1
    push       dx
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x265a               ;Offset 0x265a
    add        si, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x1603              ;Offset 0x1603
    pop        ds
    lodsw
    pop        dx
    ret
Label0x1603:                            ;Offset 0x1603
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a VGA Input Status
    pop        ds
Label0x160b:                            ;Offset 0x160b
    in         al, dx
    shr        al, 01h                  ;check HSync
    jb         Label0x160b              ;Offset 0x160b
    cli
Label0x1611:                            ;Offset 0x1611
    in         al, dx
    shr        al, 01h                  ;check HSync
    jae        Label0x1611              ;Offset 0x1611
    lodsw
    sti
    pop        dx
    ret
Label0x161a:                            ;Offset 0x161a
    push       cx
    push       es
    ja         Label0x1689              ;Offset 0x1689
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2668               ;Offset  0x2668
    mov        si, ax
    sub        sp, 08h
    mov        di, sp
    mov        ax, ss
    mov        es, ax
    mov        ax, 0b800h               ;Segment 0xb800
    mov        cx, 04h
    cmp        byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov        ds, ax
    je         Label0x164d              ;Offset 0x164d
    shl        si, 01h
Label0x1640:                            ;Offset 0x1640
    call       Func0x177e               ;Offset 0x177e
    call       Func0x177e               ;Offset 0x177e
    add        si, 0050h
    loop       Label0x1640              ;Offset 0x1640
    jmp        Label0x165a              ;Offset 0x165a
Label0x164d:                            ;Offset 0x164d
    mov        al, byte ptr [si]
    stosb
    mov        al, byte ptr [si + 2000h];Offset 0x2000
    stosb
    add        si, 0050h
    loop       Label0x164d              ;Offset 0x164d
Label0x165a:                            ;Offset 0x165a
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, 0008h
    xor        ax, ax
Label0x1665:                            ;Offset 0x1665
    push       si
    push       di
    mov        cl, 04h
    repe cmpsw
    pop        di
    pop        si
    je         Label0x1682              ;Offset 0x1682
    add        si, 0008h
    inc        al
    je         Label0x1682              ;Offset 0x1682
    jno        Label0x1665              ;Offset 0x1665
    xor        cx, cx
    mov        ds, cx
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    jmp        Label0x1665              ;Offset 0x1665
Label0x1682:                            ;Offset 0x1682
    add        sp, 0008h
    pop        es
    pop        cx
    pop        dx
    ret
Label0x1689:                            ;Offset 0x1689
    push       bx
    mov        dh, 0a0h                 ;Segment 0xa000
    push       dx
    cmp        al, 13h
    jne        Label0x1694              ;Offset 0x1694
    jmp        Label0x171b              ;Offset 0x171b
Label0x1694:                            ;Offset 0x1694
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2678               ;Offset 0x2678
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
Label0x16c4:                            ;Offset 0x16c4
    mov        al, byte ptr [si]
    not        al
    stosb
    add        si, dx
    loop       Label0x16c4              ;Offset 0x16c4
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, VGA_GCTLIdx_GraphicMode;0x5
    out        dx, ax
Label0x16d4:                            ;Offset 0x16d4
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, bx
    xor        ax, ax
Label0x16de:                            ;Offset 0x16de
    push       si
    push       di
    mov        cx, bx
    call       Func0xc55                ;Offset 0xc55
    jb         Label0x16fe              ;Offset 0x16fe
    jne        Label0x16fe              ;Offset 0x16fe
    mov        cl, 08h
    repe cmpsb
    je         Label0x16f3              ;Offset 0x16f3
    pop        di
    pop        si
    jmp        Label0x1704              ;Offset 0x1704
Label0x16f3:                            ;Offset 0x16f3
    inc        si
    mov        cl, 06h
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1714              ;Offset 0x1714
    jmp        Label0x1704              ;Offset 0x1704
Label0x16fe:                            ;Offset 0x16fe
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1714              ;Offset 0x1714
Label0x1704:                            ;Offset 0x1704
    mov        cx, bx
    call       Func0xc55                ;Offset 0xc55
    jb         Label0x170e              ;Offset 0x170e
    add        si, 0002h
Label0x170e:                            ;Offset 0x170e
    add        si, bx
    inc        al
    jne        Label0x16de              ;Offset 0x16de
Label0x1714:                            ;Offset 0x1714
    add        sp, bx
    pop        bx
    pop        es
    pop        cx
    pop        dx
    ret
Label0x171b:                            ;Offset 0x171b
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x268a               ;Offset 0x268a
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
Label0x1748:                            ;Offset 0x1748
    call       Func0x175a               ;Offset 0x175a
    stosb
    add        si, bp
    loop       Label0x1748              ;Offset 0x1748
    pop        bp
    pop        bx
    mov        dl, VGA_SequenceData_lowbyte;0x3c5
    mov        al, VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4;0xe
    out        dx, al
    jmp        Label0x16d4              ;Offset 0x16d4
ReadCharacterAndAttributeAtCursorPosition ENDP

Func0x175a PROC NEAR                    ;Offset 0x175a
    mov        bx, 0100h
    mov        al, byte ptr [si + 04h]
    call       Func0x1765               ;Offset 0x1765
    mov        al, byte ptr [si]
Func0x175a ENDP
;continue!
Func0x1765 PROC NEAR                    ;Offset 0x1765
    mov        ah, 03h
Label0x1767:                            ;Offset 0x1767
    mov        dl, VGA_GraphicsControllerData_lowbyte;Port 0x3cf
    mov        al, ah
    out        dx, al
    mov        dl, VGA_CRTControllerDataD_lowbyte;Port 0x3d5
    in         al, dx
    or         al, al
    je         Label0x1775              ;Offset 0x1775
    or         bl, bh
Label0x1775:                            ;Offset 0x1775
    rol        bh, 01h
    dec        ah
    jns        Label0x1767              ;Offset 0x1767
    mov        al, bl
    ret
Func0x1765 ENDP

Func0x177e PROC NEAR                    ;Offset 0x177e
    push       cx
    mov        dx, word ptr [si]
    xchg       dl, dh
    mov        cx, 0008h
Label0x1786:                            ;Offset 0x1786
    shl        dx, 01h
    jb         Label0x178d              ;Offset 0x178d
    jns        Label0x178d              ;Offset 0x178d
    stc
Label0x178d:                            ;Offset 0x178d
    rcl        al, 01h
    shl        dx, 01h
    loop       Label0x1786              ;Offset 0x1786
    stosb
    xor        si, 2000h
    pop        cx
    ret
Func0x177e ENDP

WriteCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x179a
    push       cx
    push       dx
    push       es
    mov        ah, bl
    push       ax
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x60a3               ;Offset 0x60a3
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x17b7              ;Offset 0x17b7
    cmp        al, BDA_DM_640x480_BW_Graphics;0x11
    je         Label0x17fa              ;Offset 0x17fa
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Func0x1800               ;Offset 0x1800
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x17b7:                            ;Offset 0x17b7
    mov        es, dx
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x265a               ;Offset 0x265a
    add        di, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x17dd              ;Offset 0x17dd
    pop        ax
    rep stosw
    pop        es
    pop        dx
    pop        cx
    ret
Label0x17dd:                            ;Offset 0x17dd
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a
    pop        si
Label0x17e5:                            ;Offset 0x17e5
    in         al, dx
    shr        al, 01h
    jb         Label0x17e5              ;Offset 0x17e5
    cli
Label0x17eb:                            ;Offset 0x17eb
    in         al, dx
    shr        al, 01h
    jae        Label0x17eb              ;Offset 0x17eb
    mov        ax, si
    stosw
    sti
    loop       Label0x17e5              ;Offset 0x17e5
    pop        es
    pop        dx
    pop        cx
    ret
Label0x17fa:                            ;Offset 0x17fa
    and        bl, 80h
    or         bl, 0fh
WriteCharacterAndAttributeAtCursorPosition ENDP
;continue!
Func0x1800 PROC NEAR                    ;Offset 0x1800
    ja         Label0x183f              ;Offset 0x183f
    mov        es, dx
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2668               ;Offset 0x2668
    mov        di, ax
    mov        dl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop        ax
    or         al, al
    jns        Label0x181d              ;Offset 0x181d
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    and        al, 7fh
    jmp        Label0x1821              ;Offset 0x1821
Label0x181d:                            ;Offset 0x181d
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
Label0x1821:                            ;Offset 0x1821
    cbw
    shl        ax, 01h
    shl        ax, 01h
    shl        ax, 01h
    add        si, ax
    cmp        dl, BDA_DM_640x200_BW_Graphics;0x6
    je         Label0x1838              ;Offset 0x1838
    shl        di, 01h
    push       bx
    call       Func0x1a15               ;Offset 0x1a15
    pop        bx
    jmp        Label0x183b              ;Offset 0x183b
Label0x1838:                            ;Offset 0x1838
    call       Func0x19b1               ;Offset 0x19b1
Label0x183b:                            ;Offset 0x183b
    pop        es
    pop        dx
    pop        cx
    ret
Label0x183f:                            ;Offset 0x183f
    mov        dh, 0a0h                 ;Segment 0xa000
    mov        es, dx
    cmp        al, 13h
    jne        Label0x184a              ;Offset 0x184a
    jmp        Label0x194b              ;Offset 0x194b
Label0x184a:                            ;Offset 0x184a
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2678               ;Offset 0x2678
    add        di, ax
    pop        ax
    push       bp
    push       cx
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    xor        ch, ch
    call       Func0xc55                ;Offset 0xc55
    jb         Label0x1883              ;Offset 0x1883
    jne        Label0x187b              ;Offset 0x187b
    or         ch, 02h
Label0x187b:                            ;Offset 0x187b
    mov        cl, 10h
    mul        cl
    mov        cl, 0eh
    jmp        Label0x1885              ;Offset 0x1885
Label0x1883:                            ;Offset 0x1883
    mul        cl
Label0x1885:                            ;Offset 0x1885
    add        si, ax
    or         bl, bl
    js         Label0x18fb              ;Offset 0x18fb
    mov        al, 52h
    push       cx
    push       cx
    xor        ch, ch
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out        dx, ax
    xor        ax, ax
    push       di
Label0x189b:                            ;Offset 0x189b
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x189b              ;Offset 0x189b
    pop        di
    mov        al, VGA_SEQIdx_MapMask    ;0x2
    mov        ah, bl
    out        dx, ax
    mov        dx, di
    pop        cx
    and        ch, 02h
    je         Label0x18ca              ;Offset 0x18ca
    xor        ch, ch
    mov        cl, 08h
Label0x18b4:                            ;Offset 0x18b4
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18b4              ;Offset 0x18b4
    mov        cl, 07h
    inc        si
Label0x18bf:                            ;Offset 0x18bf
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18bf              ;Offset 0x18bf
    inc        di
    jmp        Label0x18d2              ;Offset 0x18d2
Label0x18ca:                            ;Offset 0x18ca
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18ca              ;Offset 0x18ca
Label0x18d2:                            ;Offset 0x18d2
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x18dd              ;Offset 0x18dd
    mov        cx, 0010h
Label0x18dd:                            ;Offset 0x18dd
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    ;jne        Label0x188c              ;Offset 0x188c !!!!!!!!!!!
    DB 075h, 0A4h                       ;Generates a broken jump (target = middle of instruction)
Label0x18e8:                            ;Offset 0x18e8
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
Label0x18fa:                            ;Offset 0x18fa
    push       dx
Label0x18fb:                            ;Offset 0x18fb
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
    je         Label0x192b              ;Offset 0x192b
    xor        ch, ch
    mov        cl, 08h
Label0x1915:                            ;Offset 0x1915
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1915              ;Offset 0x1915
    mov        cl, 07h
    inc        si
Label0x1920:                            ;Offset 0x1920
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1920              ;Offset 0x1920
    inc        di
    jmp        Label0x1933              ;Offset 0x1933
Label0x192b:                            ;Offset 0x192b
    lodsb
    xchg       byte ptr es:[di], al
    add        di, bp
    loop       Label0x192b              ;Offset 0x192b
Label0x1933:                            ;Offset 0x1933
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x193e              ;Offset 0x193e
    mov        cx, 0010h
Label0x193e:                            ;Offset 0x193e
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    jne        Label0x18fa              ;Offset 0x18fa
    jmp        Label0x18e8              ;Offset 0x18e8
Label0x194b:                            ;Offset 0x194b
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x268a               ;Offset 0x268a
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
Label0x197e:                            ;Offset 0x197e
    lodsb
    push       di
    push       cx
    mov        cx, 0008h
Label0x1984:                            ;Offset 0x1984
    rcl        al, 01h
    jae        Label0x198d              ;Offset 0x198d
    mov        byte ptr es:[di], bl
    jmp        Label0x1990              ;Offset 0x1990
Label0x198d:                            ;Offset 0x198d
    mov        byte ptr es:[di], bh
Label0x1990:                            ;Offset 0x1990
    inc        di
    loop       Label0x1984              ;Offset 0x1984
    pop        cx
    pop        di
    add        di, bp
    loop       Label0x197e              ;Offset 0x197e
    pop        cx
    sub        si, cx
    mov        di, dx
    add        di, 0008h
    pop        dx
    dec        dx
    ;jne        Label0x197a              ;Offset 0x197a !!!!!!!!!!!!!!
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
Func0x1800 ENDP

Func0x19b1 PROC NEAR                    ;Offset 0x19b1
    or        bl, bl
    js        Label0x19e5               ;Offset 0x19e5
Label0x19b5:                            ;Offset 0x19b5
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
    loop      Label0x19b5               ;Offset 0x19b5
    ret
Label0x19e5:                            ;Offset 0x19e5
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
    loop      Label0x19e5               ;Offset 0x19e5
    ret
Func0x19b1 ENDP

Func0x1a15 PROC NEAR                    ;Offset 0x1a15
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
    js        Label0x1a57               ;Offset 0x1a57
Label0x1a34:                            ;Offset 0x1a34
    mov       bh, 04h
Label0x1a36:                            ;Offset 0x1a36
    lodsb
    call      Func0x1a7a                ;Offset 0x1a7a
    mov       word ptr es:[di], ax
    lodsb
    call      Func0x1a7a                ;Offset 0x1a7a
    mov       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a36               ;Offset 0x1a36
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a34               ;Offset 0x1a34
    ret
Label0x1a57:                            ;Offset 0x1a57
    mov       bh, 04h
Label0x1a59:                            ;Offset 0x1a59
    lodsb
    call      Func0x1a7a                ;Offset 0x1a7a
    xor       word ptr es:[di], ax
    lodsb
    call      Func0x1a7a                ;Offset 0x1a7a
    xor       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a59               ;Offset 0x1a59
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a57               ;Offset 0x1a57
    ret
Func0x1a15 ENDP

Func0x1a7a PROC NEAR                    ;Offset 0x1a7a
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
Func0x1a7a ENDP

WriteCharacterOnlyAtCursorPosition PROC NEAR;Offset 0x1aa3
    push      cx
    push      dx
    push      es
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x60a3                ;Offset 0x60a3
    mov       dx, 0b800h                ;Segment 0xb800
    cmp       al, 03h
    jbe       Label0x1aba               ;Offset 0x1aba
    cmp       al, 07h
    jne       Label0x1b01               ;Offset 0x1b01
    mov       dh, 0b0h                  ;Segment 0xb000
Label0x1aba:                            ;Offset 0x1aba
    mov       es, dx
    mov       al, bh
    cbw
    mov       si, ax
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       di, ax
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call      Func0x265a                ;Offset 0x265a
    add       di, ax
    pop       ax
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x1ae2               ;Offset 0x1ae2
Label0x1ada:                            ;Offset 0x1ada
    stosb
    inc       di
    loop      Label0x1ada               ;Offset 0x1ada
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1ae2:                            ;Offset 0x1ae2
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a VGA_InputStatus1
    mov       ah, al
Label0x1aeb:                            ;Offset 0x1aeb
    in        al, dx
    shr       al, 01h
    jb        Label0x1aeb               ;Offset 0x1aeb
    cli
Label0x1af1:                            ;Offset 0x1af1
    in        al, dx
    shr       al, 01h
    jae       Label0x1af1               ;Offset 0x1af1
    mov       al, ah
    stosb
    sti
    inc       di
    loop      Label0x1aeb               ;Offset 0x1aeb
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1b01:                            ;Offset 0x1b01
    jmp       Func0x1800                ;Offset 0x1800
WriteCharacterOnlyAtCursorPosition ENDP

SetBorderColorOrPalette PROC NEAR       ;Offset 0x1b04
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexB_lowbyte;Offset 0x463 0xb4
    je        Label0x1b14               ;Offset 0x1b14
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x1b15               ;Offset 0x1b15
    int       42h
Label0x1b14:                            ;Offset 0x1b14
    ret
Label0x1b15:                            ;Offset 0x1b15
    push      bx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    xchg      ax, si
    or        bh, bh
    jne       Label0x1b6e               ;Offset 0x1b6e
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
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1b54               ;Offset 0x1b54
    mov       al, bl
    xor       ah, ah
    call      Func0x2aef                ;Offset 0x2aef
    or        si, si
    je        Label0x1b54               ;Offset 0x1b54
    mov       byte ptr es:[di], al
    jmp       Label0x1b54               ;Offset 0x1b54
Label0x1b54:                            ;Offset 0x1b54
    mov       al, bl
    mov       ah, 11h
    call      Func0x2aef                ;Offset 0x2aef
    or        si, si
    je        Label0x1b63               ;Offset 0x1b63
    mov       byte ptr es:[di + 10h], al
Label0x1b63:                            ;Offset 0x1b63
    mov       bl, bh
    and       bl, 20h
    shl       bl, 01h
    shl       bl, 01h
    rol       bl, 01h
Label0x1b6e:                            ;Offset 0x1b6e
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1b98               ;Offset 0x1b98
    mov       al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       al, 0dfh
    and       bl, 01h
    je        Label0x1b84               ;Offset 0x1b84
    or        al, 20h
Label0x1b84:                            ;Offset 0x1b84
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    and       al, 10h
    or        al, 02h
    or        al, bl
    mov       ah, 01h
    call      Func0x1b9e                ;Offset 0x1b9e
    call      Func0x1b9e                ;Offset 0x1b9e
    call      Func0x1b9e                ;Offset 0x1b9e
Label0x1b98:                            ;Offset 0x1b98
    call      Func0x2adf                ;Offset 0x2adf
    pop       es
    pop       bx
    ret
SetBorderColorOrPalette ENDP

Func0x1b9e PROC NEAR                    ;Offset 0x1b9e
    call      Func0x2aef                ;Offset 0x2aef
    or        si, si
    je        Label0x1ba9               ;Offset 0x1ba9
    inc       di
    mov       byte ptr es:[di], al
Label0x1ba9:                            ;Offset 0x1ba9
    add       ax, 0102h
    ret
Func0x1b9e ENDP

WriteGraphicsPixel PROC NEAR            ;Offset 0x1bad
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1c15               ;Offset 0x1c15
    push      cx
    push      dx
    push      ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1c2c               ;Offset 0x1c2c
    mov       dh, al
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1bd0               ;Offset 0x1bd0
    or        ah, 20h
Label0x1bd0:                            ;Offset 0x1bd0
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    mov       ah, dh
    jb        Label0x1bec               ;Offset 0x1bec
    shr       si, 01h
    mov       al, 80h
    jmp       Label0x1bf2               ;Offset 0x1bf2
Label0x1bec:                            ;Offset 0x1bec
    shl       cl, 01h
    mov       al, 0c0h
    ror       ah, 01h
Label0x1bf2:                            ;Offset 0x1bf2
    add       di, si
    ror       ah, 01h
    and       cl, 07h
    and       ah, al
    shr       ax, cl
    mov       cl, byte ptr [di]
    shl       dh, 01h
    jb        Label0x1c0d               ;Offset 0x1c0d
    not       al
    and       cl, al
    or        cl, ah
    mov       byte ptr [di], cl
    jmp       Label0x1c11               ;Offset 0x1c11
Label0x1c0d:                            ;Offset 0x1c0d
    xor       cl, ah
    mov       byte ptr [di], cl
Label0x1c11:                            ;Offset 0x1c11
    pop       ax
    pop       dx
    pop       cx
    ret
Label0x1c15:                            ;Offset 0x1c15
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
Label0x1c2c:                            ;Offset 0x1c2c
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
    jns       Label0x1c65               ;Offset 0x1c65
    mov       ax, (VGA_GCTL3_FuncSourceXORLatched SHL 8) OR VGA_GCTLIdx_DataRotate;0x1803
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    jmp       Label0x1c6f               ;Offset 0x1c6f
Label0x1c65:                            ;Offset 0x1c65
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    xor       al, al
    xchg      byte ptr [di], al
Label0x1c6f:                            ;Offset 0x1c6f
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

ReadGraphicsPixel PROC NEAR             ;Offset 0x1c8a
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1ce3               ;Offset 0x1ce3
    push      cx
    push      dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1cf9               ;Offset 0x1cf9
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1caa               ;Offset 0x1caa
    or        ah, 20h
Label0x1caa:                            ;Offset 0x1caa
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    jb        Label0x1ccf               ;Offset 0x1ccf
    shr       si, 01h
    add       di, si
    and       cl, 07h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    and       al, 01h
    jmp       Label0x1ce0               ;Offset 0x1ce0
Label0x1ccf:                            ;Offset 0x1ccf
    add       di, si
    and       cl, 03h
    shl       cl, 01h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    rol       al, 01h
    and       al, 03h
Label0x1ce0:                            ;Offset 0x1ce0
    pop       dx
    pop       cx
    ret
Label0x1ce3:                            ;Offset 0x1ce3
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
Label0x1cf9:                            ;Offset 0x1cf9
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
Label0x1d27:                            ;Offset 0x1d27
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, ax
    mov       al, byte ptr [di]
    and       al, ch
    rol       bl, 01h
    or        bl, al
    dec       ah
    jns       Label0x1d27               ;Offset 0x1d27
    inc       cx
    rol       bl, cl
    mov       al, bl
    mov       bx, si
    pop       dx
    pop       cx
    ret
ReadGraphicsPixel ENDP

TeletypeOutput PROC NEAR                ;Offset 0x1d40
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
    jbe       Func0x1d74                ;Offset 0x1d74
TeletypeOutput ENDP

Func0x1d57 PROC NEAR                    ;Offset 0x1d57
    mov       cx, 0001h
    push      ds
    call      WriteCharacterOnlyAtCursorPosition;Offset 0x1aa3
    pop       ds
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    je        Func0x1d94                ;Offset 0x1d94
Func0x1d57 ENDP
;continue!
Func0x1d66 PROC NEAR                    ;Offset 0x1d66
    call      SetCursorPosition         ;Offset 0x10e2
Func0x1d66 ENDP
;continue!
Func0x1d69 PROC NEAR                    ;Offset 0x1d69
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret       
Func0x1d69 ENDP

Func0x1d6e PROC NEAR                    ;Offset 0x1d6e
    push      ax
    push      bx
    push      cx
    push      dx
    cmp       al, 0dh
Func0x1d6e ENDP
;continue!
Func0x1d74 PROC NEAR                    ;Offset 0x1d74
    je        Label0x1d89               ;Offset 0x1d89
    cmp       al, 0ah
    je        Func0x1d96                ;Offset 0x1d96
    cmp       al, 08h
    je        Label0x1d8d               ;Offset 0x1d8d
    cmp       al, 07h
    jne       Func0x1d57                ;Offset 0x1d57
    mov       al, 25h
    call      Beep                      ;Offset 0x2bf4
    jmp       Func0x1d69                ;Offset 0x1d69
Label0x1d89:                            ;Offset 0x1d89
    xor       dl, dl
    jmp       Func0x1d66                ;Offset 0x1d66
Label0x1d8d:                            ;Offset 0x1d8d
    or        dl, dl
    je        Func0x1d66                ;Offset 0x1d66
    dec       dx
    jmp       Func0x1d66                ;Offset 0x1d66
Func0x1d74 ENDP

Func0x1d94 PROC NEAR                    ;Offset 0x1d94
    xor       dl, dl
Func0x1d94 ENDP
;continue!
Func0x1d96 PROC NEAR                    ;Offset 0x1d96
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Func0x1d66                ;Offset 0x1d66
    dec       dh
    call      SetCursorPosition         ;Offset 0x10e2
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x60a3                ;Offset 0x60a3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1db2               ;Offset 0x1db2
    cbw
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x1db7               ;Offset 0x1db7
Label0x1db2:                            ;Offset 0x1db2
    push      ds
    call      ReadCharacterAndAttributeAtCursorPosition;Offset 0x15cd
    pop       ds
Label0x1db7:                            ;Offset 0x1db7
    mov       bh, ah
    xor       cx, cx
    mov       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec       dx
    mov       al, 01h
    call      ScrollUpWindow            ;Offset 0x116b
    jmp       Func0x1d69                ;Offset 0x1d69
Func0x1d96 ENDP

GetCurrentVideoMode PROC NEAR           ;Offset 0x1dc7
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, BDA_VMO_DontClearDisplay;0x80
    or        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       ah, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    ret
GetCurrentVideoMode ENDP

RAMDACFunctions PROC NEAR               ;Offset 0x1dd9
    mov       ah, 1bh
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;misassemble
    ja        EmptyFunction             ;Offset 0x1ded
    cbw
    mov       si, ax
    shl       si, 01h
    push      es
    push      bx
    call      word ptr cs:[si + RAMDACCallTable];Offset 0x1df4
    pop       bx
    pop       es
RAMDACFunctions ENDP

EmptyFunction PROC NEAR                 ;Offset 0x1ded
    ret       
EmptyFunction ENDP

Func0x1dee PROC NEAR                    ;Offset 0x1dee
    add       sp, 04h
    pop       es
    ret       
Func0x1dee ENDP

    nop                                 ;hello

;Offset 0x1df4
RAMDACCallTable         DW SetSinglePaletteRegister;Offset 0x1e2c       ;0x00
                        DW SetBorderColor;Offset 0x1e49                 ;0x01
                        DW SetAllPaletteRegisters;Offset 0x1e4d         ;0x02
                        DW ToggleIntensityBlinkingBit;Offset 0x1e7f     ;0x03
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW GetIndividualPaletteRegister;Offset 0x1ea1   ;0x07
                        DW GetBorderColor;Offset 0x1eab                 ;0x08
                        DW GetAllPaletteRegisters;Offset 0x1eaf         ;0x09
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW SetIndividualDACRegister;Offset 0x1eb5       ;0x10
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW SetBlockOfDACRegisters;Offset 0x1ee2         ;0x12
                        DW SelectVideoDACColorPage;Offset 0x1efb        ;0x13
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW GetIndividualDACRegister;Offset 0x1f21       ;0x15
                        DW EmptyFunction;Offset 0x1ded                  ;
                        DW GetBlockOfDACRegisters;Offset 0x1f40         ;0x17
                        DW SetPELMask   ;Offset 0x1f5a                  ;0x18
                        DW GetPELMask   ;Offset 0x1f62                  ;0x19
                        DW GetVideoDACColorPageState;Offset 0x1f6f      ;0x1a
                        DW PerformGrayScaleSumming;Offset 0x1f8c        ;0x1b

SetSinglePaletteRegister PROC NEAR      ;Offset 0x1e2c
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Func0x1e48                ;Offset 0x1e48
SetSinglePaletteRegister ENDP
;continue!
Func0x1e33 PROC NEAR                    ;Offset 0x1e33
    mov       al, bh
    mov       ah, bl
    call      Func0x2aef                ;Offset 0x2aef
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je        Func0x1e48                ;Offset 0x1e48
    mov       al, bh
    xor       bh, bh
    mov       byte ptr es:[bx + di], al
Func0x1e33 ENDP
;continue!
Func0x1e48 PROC NEAR                    ;Offset 0x1e48
    ret
Func0x1e48 ENDP

SetBorderColor PROC NEAR                ;Offset 0x1e49
    mov       bl, 11h
    jmp       Func0x1e33                ;Offset 0x1e33
SetBorderColor ENDP

SetAllPaletteRegisters PROC NEAR        ;Offset 0x1e4d
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1e7e               ;Offset 0x1e7e
    push      cx
    push      dx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je        Label0x1e69               ;Offset 0x1e69
    pop       ds
    push      ds
    mov       si, dx
    mov       cx, 0011h
    rep movsb
    mov       ds, cx
Label0x1e69:                            ;Offset 0x1e69
    pop       es
    mov       si, dx
    mov       ax, 1000h
    mov       cx, 0011h
    call      Func0x2bd4                ;Offset 0x2bd4
    call      Func0x28d1                ;Offset 0x28d1
    mov       al, 20h
    out       dx, al
    sti
    pop       dx
    pop       cx
Label0x1e7e:                            ;Offset 0x1e7e
    ret
SetAllPaletteRegisters ENDP

ToggleIntensityBlinkingBit PROC NEAR    ;Offset 0x1e7f
    mov       ah, 10h
    call      Func0x2b03                ;Offset 0x2b03
    cmp       bl, 01h
    ja        Label0x1e9b               ;Offset 0x1e9b
    and       al, 0f7h
    and       byte ptr ds:[BDA_CRTModeControlRegValue], NOT BDA_CMCRV_Blinking;Offset 0x465 0xdf
    or        bl, bl
    je        Label0x1e9b               ;Offset 0x1e9b
    or        al, 08h
    or        byte ptr ds:[BDA_CRTModeControlRegValue], BDA_CMCRV_Blinking;Offset 0x465 0x20
Label0x1e9b:                            ;Offset 0x1e9b
    mov       ah, 10h
    call      Func0x2aef                ;Offset 0x2aef
    ret
ToggleIntensityBlinkingBit ENDP

GetIndividualPaletteRegister PROC NEAR  ;Offset 0x1ea1
    mov       ah, bl
GetIndividualPaletteRegister ENDP
;continue!
Func0x1ea3 PROC NEAR                    ;Offset 0x1ea3
    call      Func0x2b03                ;Offset 0x2b03
    mov       bh, al
    jmp       Func0x1dee                ;Offset 0x1dee
Func0x1ea3 ENDP

GetBorderColor PROC NEAR                ;Offset 0x1eab
    mov       ah, 11h
    jmp       Func0x1ea3                ;Offset 0x1ea3
GetBorderColor ENDP

GetAllPaletteRegisters PROC NEAR        ;Offset 0x1eaf
    mov       di, dx
    call      Func0x28e7                ;Offset 0x28e7
    ret       
GetAllPaletteRegisters ENDP

SetIndividualDACRegister PROC NEAR      ;Offset 0x1eb5
    push      cx
    push      dx
    mov       ah, dh
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    je        Label0x1ec5               ;Offset 0x1ec5
    call      Func0x29e2                ;Offset 0x29e2
    mov       cx, ax
Label0x1ec5:                            ;Offset 0x1ec5
    call      Func0x2be2                ;Offset 0x2be2
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    jmp       Label0x1ed0               ;Offset 0x1ed0 Speed sensitive!
Label0x1ed0:                            ;Offset 0x1ed0
    inc       dx
    mov       al, ah
    out       dx, al
    jmp       Label0x1ed6               ;Offset 0x1ed6 Speed sensitive!
Label0x1ed6:                            ;Offset 0x1ed6
    mov       al, ch
    out       dx, al
    jmp       Label0x1edb               ;Offset 0x1edb Speed sensitive!
Label0x1edb:                            ;Offset 0x1edb
    mov       al, cl
    out       dx, al
    sti       
    pop       dx
    pop       cx
    ret       
SetIndividualDACRegister ENDP

SetBlockOfDACRegisters PROC NEAR        ;Offset 0x1ee2
    push      cx
    push      dx
    mov       si, dx
    call      TurnScreenOff             ;Offset 0x2b1f
    push      ax
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    inc       dx
    call      Func0x29a9                ;Offset 0x29a9
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b21
    pop       dx
    pop       cx
    ret
SetBlockOfDACRegisters ENDP

SelectVideoDACColorPage PROC NEAR       ;Offset 0x1efb
    mov       ah, 10h
    call      Func0x2b03                ;Offset 0x2b03
    or        bl, bl
    jne       Label0x1f0f               ;Offset 0x1f0f
    shl       al, 01h
    add       bh, 0ffh
    rcr       al, 01h
    call      Func0x2aef                ;Offset 0x2aef
    ret
Label0x1f0f:                            ;Offset 0x1f0f
    or        al, al
    mov       al, bh
    js        Label0x1f19               ;Offset 0x1f19
    shl       al, 01h
    shl       al, 01h
Label0x1f19:                            ;Offset 0x1f19
    and       al, 0fh
    mov       ah, 14h
    call      Func0x2aef                ;Offset 0x2aef
    ret
SelectVideoDACColorPage ENDP

GetIndividualDACRegister PROC NEAR      ;Offset 0x1f21
    mov       al, bl
    mov       bl, dl
    call      Func0x2be2                ;Offset 0x2be2
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    out       dx, al
    jmp       Label0x1f2e               ;Offset 0x1f2e Speed sensitive!
Label0x1f2e:                            ;Offset 0x1f2e
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1f33               ;Offset 0x1f33 Speed sensitive!
Label0x1f33:                            ;Offset 0x1f33
    mov       bh, al
    in        al, dx
    jmp       Label0x1f38               ;Offset 0x1f38 Speed sensitive!
Label0x1f38:                            ;Offset 0x1f38
    mov       ah, al
    in        al, dx
    sti
    xchg      ax, cx
    mov       dx, bx
    ret
GetIndividualDACRegister ENDP

GetBlockOfDACRegisters PROC NEAR        ;Offset 0x1f40
    push      cx
    push      dx
    mov       di, dx
    call      TurnScreenOff             ;Offset 0x2b1f
    push      ax
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    mov       dl, VGA_RAMDACData_lowbyte;0x3c9
    call      Func0x29d9                ;Offset 0x29d9
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b21
    pop       dx
    pop       cx
    ret       
GetBlockOfDACRegisters ENDP

SetPELMask PROC NEAR                    ;Offset 0x1f5a
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    xchg      ax, bx
    out       dx, al
    pop       dx
    ret       
SetPELMask ENDP

GetPELMask PROC NEAR                    ;Offset 0x1f62
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    in        al, dx
    pop       dx
    xor       ah, ah
    xchg      ax, bx
    jmp       Func0x1dee                ;Offset 0x1dee
    ret       
GetPELMask ENDP

GetVideoDACColorPageState PROC NEAR     ;Offset 0x1f6f
    mov       ah, 14h
    call      Func0x2b03                ;Offset 0x2b03
    and       al, 0fh
    mov       bh, al
    mov       ah, 10h
    call      Func0x2b03                ;Offset 0x2b03
    rol       al, 01h
    and       al, 01h
    mov       bl, al
    jne       Label0x1f89               ;Offset 0x1f89
    shr       bh, 01h
    shr       bh, 01h
Label0x1f89:                            ;Offset 0x1f89
    jmp       Func0x1dee                ;Offset 0x1dee
GetVideoDACColorPageState ENDP

PerformGrayScaleSumming PROC NEAR       ;Offset 0x1f8c
    push      cx
    push      dx
    call      TurnScreenOff             ;Offset 0x2b1f
    push      ax
Label0x1f92:                            ;Offset 0x1f92
    push      cx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    jmp       Label0x1f9b               ;Offset 0x1f9b Speed sensitive!
Label0x1f9b:                            ;Offset 0x1f9b
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1fa0               ;Offset 0x1fa0 Speed sensitive!
Label0x1fa0:                            ;Offset 0x1fa0
    mov       bh, al
    in        al, dx
    jmp       Label0x1fa5               ;Offset 0x1fa5 Speed sensitive!
Label0x1fa5:                            ;Offset 0x1fa5
    mov       ah, al
    in        al, dx
    jmp       Label0x1faa               ;Offset 0x1faa Speed sensitive!
Label0x1faa:                            ;Offset 0x1faa
    xchg      ax, cx
    dec       dx
    mov       al, bl
    out       dx, al
    mov       dh, bh
    call      Func0x29e2                ;Offset 0x29e2
    mov       dx, VGA_RAMDACData        ;Port 0x3c9
    out       dx, al
    jmp       Label0x1fba               ;Offset 0x1fba Speed sensitive!
Label0x1fba:                            ;Offset 0x1fba
    out       dx, al
    jmp       Label0x1fbd               ;Offset 0x1fbd Speed sensitive!
Label0x1fbd:                            ;Offset 0x1fbd
    out       dx, al
    inc       bx
    pop       cx
    loop      Label0x1f92               ;Offset 0x1f92
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b21
    pop       dx
    pop       cx
    ret
PerformGrayScaleSumming ENDP

TextFunctions PROC NEAR                 ;Offset 0x1fc9
    lea       si, [Func0x1ff6]          ;Offset 0x1ff6
    cmp       al, 10h
    jb        Label0x1fe7               ;Offset 0x1fe7
    lea       si, [Func0x2017]          ;Offset 0x2017
    cmp       al, 20h
    jb        Label0x1fe7               ;Offset 0x1fe7
    lea       si, [Func0x2032]          ;Offset 0x2032
    cmp       al, 30h
    jb        Label0x1fe7               ;Offset 0x1fe7
    jne       Label0x1ff5               ;Offset 0x1ff5
    call      Func0x2096                ;Offset 0x2096
    ret
Label0x1fe7:                            ;Offset 0x1fe7
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
Label0x1ff5:                            ;Offset 0x1ff5
    ret       
TextFunctions ENDP

Func0x1ff6 PROC NEAR                    ;Offset 0x1ff6
    cmp       al, 03h
    je        Label0x200e               ;Offset 0x200e
    cmp       al, 04h
    ja        Label0x2016               ;Offset 0x2016
    and       bl, 7fh
    call      Func0x26ef                ;Offset 0x26ef
    call      Func0x2730                ;Offset 0x2730
    call      Func0x26f5                ;Offset 0x26f5
    call      Func0x2adf                ;Offset 0x2adf
    ret
Label0x200e:                            ;Offset 0x200e
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    mov       ah, bl
    out       dx, ax
Label0x2016:                            ;Offset 0x2016
    ret
Func0x1ff6 ENDP

Func0x2017 PROC NEAR                    ;Offset 0x2017
    cmp       al, 03h
    je        Label0x2031               ;Offset 0x2031
    cmp       al, 04h
    ja        Label0x2031               ;Offset 0x2031
    and       bl, 7fh
    call      Func0x26ef                ;Offset 0x26ef
    call      Func0x2730                ;Offset 0x2730
    call      Func0x27d2                ;Offset 0x27d2
    call      Func0x26f5                ;Offset 0x26f5
    call      Func0x2adf                ;Offset 0x2adf
Label0x2031:                            ;Offset 0x2031
    ret
Func0x2017 ENDP

Func0x2032 PROC NEAR                    ;Offset 0x2032
    cmp       al, 04h
    ja        Label0x2095               ;Offset 0x2095
    cbw
    push      ax
    dec       ax
    je        Label0x2062               ;Offset 0x2062
    jns       Label0x2047               ;Offset 0x2047
    pop       ax
    mov       word ptr ds:[INT_1F_HandlerOfs], bp;Offset 0x7c
    mov       word ptr ds:[INT_1F_HandlerSeg], es;Offset 0x7e
    ret
Label0x2047:                            ;Offset 0x2047
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3900
    mov       cx, 000eh
    dec       ax
    je        Label0x2062               ;Offset 0x2062
    lea       bp, [Font8x8]             ;Offset 0x3100
    mov       cl, 08h
    dec       ax
    je        Label0x2062               ;Offset 0x2062
    lea       bp, [Font8x16]            ;Offset 0x3900
    mov       cl, 10h
Label0x2062:                            ;Offset 0x2062
    mov       word ptr ds:[INT_43_HandlerOfs], bp;Offset 0x10c
    mov       word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    pop       ax
    cmp       al, 01h
    je        Label0x2079               ;Offset 0x2079
    cmp       cl, 0eh
    jne       Label0x2079               ;Offset 0x2079
    mov       ax, 0bd70h
    int       6dh
Label0x2079:                            ;Offset 0x2079
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    mov       al, dl
    and       bx, 00ffh
    je        Label0x2091               ;Offset 0x2091
    mov       al, 19h
    cmp       bl, 03h
    ja        Label0x2091               ;Offset 0x2091
    mov       al, byte ptr cs:[bx + RowsOnScreen];Offset 0x30ea
Label0x2091:                            ;Offset 0x2091
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x2095:                            ;Offset 0x2095
    ret
Func0x2032 ENDP

Func0x2096 PROC NEAR                    ;Offset 0x2096
    mov       cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov       al, bh
    sub       al, 02h
    cbw
    jb        Label0x20c0               ;Offset 0x20c0
    cmp       al, 05h
    ja        Label0x20cc               ;Offset 0x20cc
    shl       ax, 01h
    mov       si, ax
    mov       bp, word ptr cs:[si + Data0x30ee];Offset 0x30ee
    push      cs
    pop       es
    or        ax, ax
    jne       Label0x20bf               ;Offset 0x20bf
    mov       ax, 0bd71h
    int       6dh
    xor       ax, ax
Label0x20bf:                            ;Offset 0x20bf
    ret
Label0x20c0:                            ;Offset 0x20c0
    inc       ax
    js        Label0x20c8               ;Offset 0x20c8
    les       bp, ds:[INT_43_HandlerOfs];Offset 0x10c
    ret
Label0x20c8:                            ;Offset 0x20c8
    les       bp, ds:[INT_1F_HandlerOfs];Offset 0x7c
Label0x20cc:                            ;Offset 0x20cc
    ret       
Func0x2096 ENDP

AlternateFunctions PROC NEAR            ;Offset 0x20cd
    cmp       bl, 20h
    je        Label0x20f9               ;Offset 0x20f9
    cmp       bl, 10h
    je        Label0x20d9               ;Offset 0x20d9
    jmp       Label0x2106               ;Offset 0x2106
Label0x20d9:                            ;Offset 0x20d9
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
    mov       ch, 0fh
    and       al, BDA_EFBS_AdapterTypeMask;0xf
    mov       cl, al
    ret
Label0x20f9:                            ;Offset 0x20f9
    cli
    mov       word ptr ds:[INT_5_HandlerOfs], offset PrintScreenHandler;Offset 0x14 Offset 0x2c60
    mov       word ptr ds:[INT_5_HandlerSeg], cs;Offset 0x16
    sti
    ret
Label0x2106:                            ;Offset 0x2106
    push      bx
    push      cx
    push      dx
    mov       cl, 03h
    xor       bh, bh
    sub       bl, 30h
    shl       bx, 01h
    je        Label0x211f               ;Offset 0x211f
    cmp       bl, 0ah
    je        Label0x2120               ;Offset 0x2120
    cmp       bl, 0ch
    ja        Label0x2131               ;Offset 0x2131
    dec       cx
Label0x211f:                            ;Offset 0x211f
    dec       cx
Label0x2120:                            ;Offset 0x2120
    ;cmp       al, cl
    DB 3Ah, 0C1h                        ;misassembly
    ja        Label0x2131               ;Offset 0x2131
    call      word ptr cs:[bx + AlternateFunctionsTable];Offset 0x2136
    jb        Label0x2131               ;Offset 0x2131
    mov       al, 12h
Label0x212d:                            ;Offset 0x212d
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x2131:                            ;Offset 0x2131
    mov       al, 00h
    jmp       Label0x212d               ;Offset 0x212d
AlternateFunctions ENDP

    nop                                 ;hello

;Offset 0x2136
AlternateFunctionsTable DW SelectVerticalResolution;Offset 0x21bc
                        DW SetPaletteLoading;Offset 0x2144
                        DW SetVGAMemoryIOMapping;Offset 0x2152
                        DW SetGrayscaleSumming;Offset 0x2192
                        DW SetCursorEmulation;Offset 0x21a0
                        DW DisplaySwitchInterface;Offset 0x2200
                        DW TurnScreenOnOff;Offset 0x21ae

SetPaletteLoading PROC NEAR             ;Offset 0x2144
    je   Enabled                        ;Offset 0x214c
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0xf7
    ret
Enabled:                                ;Offset 0x214c
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
SetVGAMemoryIOMapping PROC NEAR         ;Offset 0x2152
    mov  dl, 00h
    je   Disable                        ;Offset 0x2158
    dec  dl
Disable:                                ;Offset 0x2158
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_Header_Word_Command    ;0x4
    call AccessPCIRegister              ;Offset 0x69da
    and  cl, NOT PCI_Cmd_MemSpace       ;0xfd
    and  dl, PCI_Cmd_IOSpace OR PCI_Cmd_MemSpace;0x3
    je   DisableMemSpace                ;Offset 0x2170
    or   cl, dl
DisableMemSpace:                        ;Offset 0x2170
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x69da
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_MGA_Option + 01h       ;0x41
    nop
    call AccessPCIRegister              ;Offset 0x69da
    and  cl, NOT (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0xfe
    and  dl, (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0x1
    je   DoWrite                        ;Offset 0x218a
    or   cl, dl                         ;OR in the new value (one)
DoWrite:                                ;Offset 0x218a
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x69da
    clc
    ret
SetVGAMemoryIOMapping ENDP

SetGrayscaleSumming PROC NEAR           ;Offset 0x2192
    je   Disable                        ;Offset 0x219a
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0x489 0x2
    ret
Disable:                                ;Offset 0x219a
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489 0xfd
    ret       
SetGrayscaleSumming ENDP

SetCursorEmulation PROC NEAR            ;Offset 0x21a0
    je   Enable                         ;Offset 0x21a8
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487 0xfe
    ret
Enable:                                 ;Offset 0x21a8
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
TurnScreenOnOff PROC NEAR               ;Offset 0x21ae
    lea  dx, [TurnScreenOff]            ;Offset 0x2b1f
    je   TurnOff                        ;Offset 0x21b8
    lea  dx, [TurnScreenOn]             ;Offset 0x2b1b
TurnOff:                                ;Offset 0x21b8
    call dx
    clc       
    ret       
TurnScreenOnOff ENDP

SelectVerticalResolution PROC NEAR      ;Offset 0x21bc
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x21e1               ;Offset 0x21e1
    cbw
    xchg      ax, si
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, NOT BDA_VDDA_LineMode400;0xef
    test      al, BDA_VDDA_VGA          ;0x1
    je        Label0x21d0               ;Offset 0x21d0
    and       al, NOT BDA_VDDA_LineMode200;0x7f
Label0x21d0:                            ;Offset 0x21d0
    mov       bl, BDA_EFBS_MDAHiResEnhanced_2;0x9
    dec       si
    jns       Label0x21e5               ;Offset 0x21e5
    dec       bx
    test      al, BDA_VDDA_VGA          ;0x1
    jne       Label0x21e3               ;Offset 0x21e3
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x21ee               ;Offset 0x21ee
Label0x21e1:                            ;Offset 0x21e1
    stc
    ret
Label0x21e3:                            ;Offset 0x21e3
    or        al, BDA_VDDA_LineMode200  ;0x80
Label0x21e5:                            ;Offset 0x21e5
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x21ee               ;Offset 0x21ee
    mov       bl, BDA_EFBS_CGAMono80x25_2;0xb
Label0x21ee:                            ;Offset 0x21ee
    dec       si
    js        Label0x21f3               ;Offset 0x21f3
    or        al, BDA_VDDA_LineMode400  ;0x10
Label0x21f3:                            ;Offset 0x21f3
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    ret
SelectVerticalResolution ENDP

DisplaySwitchInterface PROC NEAR        ;Offset 0x2200
    cbw
    xchg      ax, si
    dec       si
    je        Label0x2252               ;Offset 0x2252
    js        Label0x221e               ;Offset 0x221e
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2252               ;Offset 0x2252
    dec       si
    je        Label0x2231               ;Offset 0x2231
    mov       si, dx
    call      RestoreBiosDataAreaVideoSections;Offset 0x2606
    mov       al, 0eh
Label0x2218:                            ;Offset 0x2218
    mov       dx, 46e8h                 ;Port 0x46e8 Some kind of video subsystem enable port?
    out       dx, al
    clc
    ret
Label0x221e:                            ;Offset 0x221e
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    jne       Label0x2252               ;Offset 0x2252
    mov       ax, 1280h
    int       42h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2252               ;Offset 0x2252
Label0x2231:                            ;Offset 0x2231
    mov       di, dx
    call      BackupBiosDataAreaVideoSections;Offset 0x2523
    push      es
    les       bx, ds:[INT_42_HandlerOfs];Offset 0x108
    mov       word ptr ds:[INT_6D_HandlerOfs], bx;Offset 0x1b4
    mov       word ptr ds:[INT_6D_HandlerSeg], es;Offset 0x1b6
    pop       es
    mov       word ptr ds:[INT_42_HandlerOfs], Int6DHandler;Offset 0x108 Offset 0xb70
    mov       word ptr ds:[INT_42_HandlerSeg], cs;Offset 0x10a
    mov       al, 06h
    jmp       Label0x2218               ;Offset 0x2218
Label0x2252:                            ;Offset 0x2252
    stc       
    ret       
DisplaySwitchInterface ENDP

WriteString PROC NEAR                   ;Offset 0x2254
    push      bx
    push      cx
    push      dx
    push      bp
    cmp       al, 03h
    ja        Label0x229a               ;Offset 0x229a
    jcxz      Label0x229a               ;Offset 0x229a
    cbw
    mov       di, ax
    dec       di
    mov       al, bh
    mov       si, ax
    shl       si, 01h
    push      word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    push      si
    call      SetCursorPosition         ;Offset 0x10e2
Label0x2270:                            ;Offset 0x2270
    mov       al, byte ptr es:[bp]
    inc       bp
    cmp       al, 0dh
    jbe       Label0x229f               ;Offset 0x229f
Label0x2279:                            ;Offset 0x2279
    or        di, di
    jle       Label0x2282               ;Offset 0x2282
    mov       bl, byte ptr es:[bp]
    inc       bp
Label0x2282:                            ;Offset 0x2282
    call      Func0x22cb                ;Offset 0x22cb
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae       Label0x22b8               ;Offset 0x22b8
Label0x228c:                            ;Offset 0x228c
    call      SetCursorPosition         ;Offset 0x10e2
Label0x228f:                            ;Offset 0x228f
    loop      Label0x2270               ;Offset 0x2270
    pop       si
    pop       dx
    shr       di, 01h
    jae       Label0x229a               ;Offset 0x229a
    call      SetCursorPosition         ;Offset 0x10e2
Label0x229a:                            ;Offset 0x229a
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x229f:                            ;Offset 0x229f
    je        Label0x22ad               ;Offset 0x22ad
    cmp       al, 0ah
    je        Label0x22ad               ;Offset 0x22ad
    cmp       al, 08h
    je        Label0x22ad               ;Offset 0x22ad
    cmp       al, 07h
    jne       Label0x2279               ;Offset 0x2279
Label0x22ad:                            ;Offset 0x22ad
    call      Func0x22d8                ;Offset 0x22d8
    pop       si
    push      si
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x228f               ;Offset 0x228f
Label0x22b8:                            ;Offset 0x22b8
    xor       dl, dl
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Label0x228c               ;Offset 0x228c
    dec       dh
    mov       al, 0ah
    call      Func0x22d8                ;Offset 0x22d8
    jmp       Label0x228c               ;Offset 0x228c
WriteString ENDP

Func0x22cb PROC NEAR                    ;Offset 0x22cb
    push      cx
    push      di
    push      ds
    mov       cx, 0001h
    call      WriteCharacterAndAttributeAtCursorPosition;Offset 0x179a
    pop       ds
    pop       di
    pop       cx
    ret
Func0x22cb ENDP

Func0x22d8 PROC NEAR                    ;Offset 0x22d8
    push      di
    push      ds
    call      Func0x1d6e                ;Offset 0x1d6e
    pop       ds
    pop       di
    ret
Func0x22d8 ENDP

NotImplemented PROC NEAR                ;Offset 0x22e0
    ret
NotImplemented ENDP

DisplayCombinationCodeFunctions PROC NEAR;Offset 0x22e1
    push      cx
    push      dx
    push      es
    push      ax
    or        al, al
    jne       Label0x2312               ;Offset 0x2312
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x66f6
    je        Label0x2312               ;Offset 0x2312
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_Misc      ;0x3
    out       dx, al
    inc       dx
    in        al, dx
    and       al, MGA_CRTCEXT3_MGAModeEnable;0x80
    je        Label0x2312               ;Offset 0x2312
    push      ds
    xor       ax, ax
    mov       ds, ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    pop       ds
    ja        Label0x2312               ;Offset 0x2312
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x2312               ;Offset 0x2312
    pop       ax
    jmp       Label0x2324               ;Offset 0x2324
Label0x2312:                            ;Offset 0x2312
    pop       ax
    xor       ah, ah
    dec       ax
    js        Label0x231f               ;Offset 0x231f
    jne       Label0x2324               ;Offset 0x2324
    call      SetDisplayCombinationCodeIndex;Offset 0x2b36
    jmp       Label0x2322               ;Offset 0x2322
Label0x231f:                            ;Offset 0x231f
    call      Func0x2b5d                ;Offset 0x2b5d
Label0x2322:                            ;Offset 0x2322
    mov       al, 1ah
Label0x2324:                            ;Offset 0x2324
    pop       es
    pop       dx
    pop       cx
    ret       
DisplayCombinationCodeFunctions ENDP

FunctionalityAndStateInfo PROC NEAR     ;Offset 0x2328
    or        bx, bx
    je        Label0x232f               ;Offset 0x232f
    xor       al, al
    ret
Label0x232f:                            ;Offset 0x232f
    push      bx
    push      cx
    push      dx
    lea       ax, [StaticFunctionalityTable];Offset 0x3c4
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
    call      Func0x2b5d                ;Offset 0x2b5d
    xchg      ax, bx
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    push      ax
    lea       bx, [ColorsPerModePlusOne];Offset 0x240c
    xlatb     cs:[bx]
    dec       ax
    jns       ColorsUnder256            ;Offset 0x2361
    mov       ax, 0100h                 ;Set colors to 256 (mode 0x13)
ColorsUnder256:                         ;Offset 0x2361
    stosw
    pop       ax
    lea       bx, [NumberOfPagesPerMode];Offset 0x2420
    xlatb     cs:[bx]
    stosb
    call      GetNumberOfActiveScanlines;Offset 0x2a6a
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
    call      Func0x23fb                ;Offset 0x23fb
    mov       al, 0ah
    call      Func0x23f2                ;Offset 0x23f2
    mov       al, 0ch
    call      Func0x23f2                ;Offset 0x23f2
    mov       al, 08h
    call      Func0x23f2                ;Offset 0x23f2
    mov       al, 04h
    call      Func0x23f2                ;Offset 0x23f2
    mov       al, 06h
    call      Func0x23f2                ;Offset 0x23f2
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

Func0x23f2 PROC NEAR                    ;Offset 0x23f2
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je        Label0x23fa               ;Offset 0x23fa
    inc       dx
Label0x23fa:                            ;Offset 0x23fa
    ret
Func0x23f2 ENDP

Func0x23fb PROC NEAR                    ;Offset 0x23fb
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je        Label0x240b               ;Offset 0x240b
    mov       ax, cs
    mov       bx, es
    cmp       ax, bx
    je        Label0x240b               ;Offset 0x240b
    inc       dx
Label0x240b:                            ;Offset 0x240b
    ret
Func0x23fb ENDP

;Offset 0x240c
ColorsPerModePlusOne    DB 011h, 011h, 011h, 011h, 005h, 005h, 003h, 001h, 001h, 001h
                        DB 001h, 001h, 001h, 011h, 011h, 001h, 011h, 003h, 011h, 000h

;Offset 0x2420
NumberOfPagesPerMode    DB 008h, 008h, 008h, 008h, 001h, 001h, 001h, 008h, 000h, 000h
                        DB 000h, 000h, 000h, 008h, 004h, 002h, 002h, 001h, 001h, 001h

SaveRestoreVideoState PROC NEAR         ;Offset 0x2434
    push      cx
    test      cl, 07h
    je        Label0x2445               ;Offset 0x2445
    cbw
    inc       ax
    dec       ax
    je        Label0x2449               ;Offset 0x2449
    dec       ax
    je        Label0x246f               ;Offset 0x246f
    dec       ax
    je        Label0x248b               ;Offset 0x248b
Label0x2445:                            ;Offset 0x2445
    mov       al, 00h
    pop       cx
    ret
Label0x2449:                            ;Offset 0x2449
    xor       ax, ax
    shr       cl, 01h
    jae       Label0x2451               ;Offset 0x2451
    add       al, 46h
Label0x2451:                            ;Offset 0x2451
    shr       cl, 01h
    jae       Label0x2457               ;Offset 0x2457
    add       al, 3ah
Label0x2457:                            ;Offset 0x2457
    shr       cl, 01h
    jae       Label0x245e               ;Offset 0x245e
    add       ax, 0303h
Label0x245e:                            ;Offset 0x245e
    add       al, 20h
    mov       cl, 06h
    ror       ax, cl
    add       ah, 0ffh
    adc       al, 00h
    cbw
    xchg      ax, bx
Label0x246b:                            ;Offset 0x246b
    mov       al, 1ch
    pop       cx
    ret
Label0x246f:                            ;Offset 0x246f
    push      dx
    lea       di, [bx + 20h]
    shr       cl, 01h
    jae       Label0x247a               ;Offset 0x247a
    call      Func0x24a4                ;Offset 0x24a4
Label0x247a:                            ;Offset 0x247a
    shr       cl, 01h
    jae       Label0x2481               ;Offset 0x2481
    call      Func0x251f                ;Offset 0x251f
Label0x2481:                            ;Offset 0x2481
    shr       cl, 01h
    jae       Label0x2488               ;Offset 0x2488
    call      Func0x255c                ;Offset 0x255c
Label0x2488:                            ;Offset 0x2488
    pop       dx
    jmp       Label0x246b               ;Offset 0x246b
Label0x248b:                            ;Offset 0x248b
    push      dx
    shr       cl, 01h
    jae       Label0x2493               ;Offset 0x2493
    call      Func0x2581                ;Offset 0x2581
Label0x2493:                            ;Offset 0x2493
    shr       cl, 01h
    jae       Label0x249a               ;Offset 0x249a
    call      Func0x2602                ;Offset 0x2602
Label0x249a:                            ;Offset 0x249a
    shr       cl, 01h
    jae       Label0x24a1               ;Offset 0x24a1
    call      Func0x2630                ;Offset 0x2630
Label0x24a1:                            ;Offset 0x24a1
    pop       dx
    jmp       Label0x246b               ;Offset 0x246b
SaveRestoreVideoState ENDP

Func0x24a4 PROC NEAR                    ;Offset 0x24a4
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
    call      Func0x28c4                ;Offset 0x28c4
    mov       dl, VGA_MiscellaneousRead_lowbyte;Port 0x3cc
    in        al, dx
    stosb
    mov       dx, si
    mov       cl, 19h
    call      Func0x28c2                ;Offset 0x28c2
    mov       ah, 00h
    mov       cl, 14h
Label0x24de:                            ;Offset 0x24de
    call      Func0x2b03                ;Offset 0x2b03
    stosb
    inc       ah
    loop      Label0x24de               ;Offset 0x24de
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    call      Func0x28c2                ;Offset 0x28c2
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
Label0x2503:                            ;Offset 0x2503
    out       dx, al
    push      ax
    xchg      dx, si
    in        al, dx
    stosb
    xchg      dx, si
    pop       ax
    inc       ax
    cmp       al, 03h
    jbe       Label0x2503               ;Offset 0x2503
    mov       al, byte ptr es:[di - 0bh]
    out       dx, al
    mov       dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov       al, byte ptr es:[di - 3eh]
    out       dx, al
    pop       cx
    ret
Func0x24a4 ENDP

Func0x251f PROC NEAR                    ;Offset 0x251f
    mov       word ptr es:[bx + 02h], di
Func0x251f ENDP
;continue!
BackupBiosDataAreaVideoSections PROC NEAR;Offset 0x2523
    push      cx
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    lea       si, [BiosDataAreaVideoSections];Offset 0x253e
ProcessNextSection:                     ;Offset 0x252e
    lodsw     word ptr cs:[si]          ;Count
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x253b
    lodsw     word ptr cs:[si]          ;Source address
    xchg      ax, si
    rep movsb
    xchg      ax, si
    jmp       ProcessNextSection        ;Offset 0x252e
Return:                                 ;Offset 0x253b
    pop       cx
    ret
BackupBiosDataAreaVideoSections ENDP

    nop                                 ;https://i.imgur.com/NGkg1Ip.jpeg

;Offset 0x253e
BiosDataAreaVideoSections   DW 001eh, BDA_DisplayMode;Offset 0x449
                            DW 0007h, BDA_RowsOnScreen;Offset 0x484
                            DW 0004h, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
                            DW 0004h, INT_5_HandlerOfs;Offset 0x014
                            DW 0004h, INT_1D_HandlerOfs;Offset 0x074
                            DW 0004h, INT_1F_HandlerOfs;Offset 0x07c
                            DW 0004h, INT_43_HandlerOfs;Offset 0x10c
                            DW 0000h

Func0x255c PROC NEAR                    ;Offset 0x255c
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
    call      Func0x29d9                ;Offset 0x29d9
    pop       cx
    ret
Func0x255c ENDP

Func0x2581 PROC NEAR                    ;Offset 0x2581
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
    call      Func0x284b                ;Offset 0x284b
    add       dl, 0006h
    mov       al, byte ptr es:[si - 1fh]
    out       dx, al
    xor       ax, ax
    mov       cx, 0010h
    call      Func0x28cf                ;Offset 0x28cf
    inc       ax
    inc       si
    mov       cl, 01h
    call      Func0x28cf                ;Offset 0x28cf
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
Func0x2581 ENDP

Func0x2602 PROC NEAR                    ;Offset 0x2602
    mov       si, word ptr es:[bx + 02h]
Func0x2602 ENDP
;continue!
RestoreBiosDataAreaVideoSections PROC NEAR;Offset 0x2606
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
    lea       dx, [BiosDataAreaVideoSections];Offset 0x253e
ProcessNextSection:                     ;Offset 0x261c
    xchg      si, dx
    lodsw     word ptr cs:[si]
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x262c
    lodsw     word ptr cs:[si]
    xchg      ax, di
    xchg      si, dx
    rep movsb
    jmp       ProcessNextSection        ;Offset 0x261c
Return:                                 ;Offset 0x262c
    pop       es
    pop       ds
    pop       cx
    ret       
RestoreBiosDataAreaVideoSections ENDP

Func0x2630 PROC NEAR                    ;Offset 0x2630
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
    call      Func0x29a9                ;Offset 0x29a9
    sub       si, 0303h
    lodsw     word ptr es:[si]
    xchg      al, ah
    shr       ah, 01h
    sbb       dl, 01h
    out       dx, al
    pop       cx
    ret
Func0x2630 ENDP

Func0x265a PROC NEAR                    ;Offset 0x265a
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    xor       dh, dh
    add       ax, dx
    shl       ax, 01h
    ret
Func0x265a ENDP

Func0x2668 PROC NEAR                    ;Offset 0x2668
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    shl       ax, 01h
    shl       ax, 01h
    xor       dh, dh
    add       ax, dx
    ret
Func0x2668 ENDP

Func0x2678 PROC NEAR                    ;Offset 0x2678
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    pop       dx
    xor       dh, dh
    add       ax, dx
    ret
Func0x2678 ENDP

Func0x268a PROC NEAR                    ;Offset 0x268a
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
Func0x268a ENDP

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
SetCursorEndStart PROC NEAR             ;Offset 0x26a2
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

ClearScreen PROC NEAR                   ;Offset 0x26b1
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487 0x80
    jne       Label0x26ee               ;Offset 0x26ee
    cmp       word ptr ds:[BDA_VideoBufferSize], 00h;Offset 0x44c
    je        Label0x26ee               ;Offset 0x26ee
    push      bx
    push      cx
    push      dx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x60a3                ;Offset 0x60a3
    mov       bx, 0a000h                ;Segment 0xa000
    mov       cx, 8000h
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x26e1               ;Offset 0x26e1
    mov       bh, 0b0h                  ;Segment 0xb000
    mov       ch, 40h
    mov       dx, 0720h                 ;Attribute + space
    je        Label0x26e3               ;Offset 0x26e3
    mov       bh, 0b8h                  ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x26e3               ;Offset 0x26e3
Label0x26e1:                            ;Offset 0x26e1
    xor       dx, dx
Label0x26e3:                            ;Offset 0x26e3
    mov       es, bx
    mov       ax, dx
    xor       di, di
    rep stosw
    pop       dx
    pop       cx
    pop       bx
Label0x26ee:                            ;Offset 0x26ee
    ret
ClearScreen ENDP

Func0x26ef PROC NEAR                    ;Offset 0x26ef
    lea       si, [Data0x271a]          ;Offset 0x271a
    jmp       Func0x26f9                ;Offset 0x26f9
Func0x26ef ENDP

Func0x26f5 PROC NEAR                    ;Offset 0x26f5
    lea       si, [Data0x2724]          ;Offset 0x2724
Func0x26f5 ENDP
;continue!
Func0x26f9 PROC NEAR                    ;Offset 0x26f9
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
Label0x270c:                            ;Offset 0x270c
    lodsw     word ptr cs:[si]
    out       dx, ax
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       ax, 0eb4h
    je        Label0x270c               ;Offset 0x270c
    pop       dx
    pop       ax
    ret
Func0x26f9 ENDP

;Offset 0x271a
Data0x271a              DW (VGA_SEQ2_Mask2 SHL 8) OR VGA_SEQIdx_MapMask;0x0402
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0704
                        DW (VGA_GCTL4_Map2 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0204
                        DW (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x0005
                        DW (VGA_GCTL6_Mem_A0000_AFFFF SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0406

;Offset 0x2724
Data0x2724              DW ((VGA_SEQ2_Mask0 OR VGA_SEQ2_Mask1) SHL 8)OR VGA_SEQIdx_MapMask;0x0302
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0304
                        DW (VGA_GCTL4_Map0 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0004
                        DW (VGA_GCTL5_OddEven SHL 8) OR VGA_GCTLIdx_GraphicMode;0x1005
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B8000_BFFFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0e06
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B0000_B7FFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0a06

Func0x2730 PROC NEAR                    ;Offset 0x2730
    cbw
    dec       ax
    js        Func0x2753                ;Offset 0x2753
    mov       cx, 0100h
    xor       dx, dx
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3900
    mov       bh, 00h
    dec       ax
    js        Func0x2753                ;Offset 0x2753
    lea       bp, [Font8x8]             ;Offset 0x3100
    mov       bh, 08h
    dec       ax
    js        Func0x2753                ;Offset 0x2753
    lea       bp, [Font8x16]            ;Offset 0x3900
    mov       bh, 10h
Func0x2730 ENDP
;continue!
Func0x2753 PROC NEAR                    ;Offset 0x2753
    mov       si, bp
    push      es
    pop       ds
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    jcxz      Label0x27b2               ;Offset 0x27b2
Label0x275e:                            ;Offset 0x275e
    push      cx
    mov       cl, 05h
    shl       dx, cl
    push      bx
    and       bx, 0007h
    add       dh, byte ptr cs:[bx + Data0x27ca];Offset 0x27ca
    pop       bx
    mov       di, dx
    mov       dl, bh
    xor       dh, dh
    or        dl, dl
    jne       Label0x2779               ;Offset 0x2779
    mov       dl, 10h
Label0x2779:                            ;Offset 0x2779
    pop       ax
Label0x277a:                            ;Offset 0x277a
    or        bh, bh
    jne       Label0x27a6               ;Offset 0x27a6
    cmp       al, 99h
    je        Label0x2798               ;Offset 0x2798
    cmp       al, 96h
    je        Label0x2798               ;Offset 0x2798
    cmp       al, 87h
    je        Label0x2798               ;Offset 0x2798
    cmp       al, 90h
    je        Label0x2798               ;Offset 0x2798
    cmp       al, 8fh
    je        Label0x2798               ;Offset 0x2798
    cmp       al, 68h
    je        Label0x2798               ;Offset 0x2798
    jmp       Label0x27a6               ;Offset 0x27a6
Label0x2798:                            ;Offset 0x2798
    mov       cx, 0008h
    rep movsb
    inc       si
    mov       cx, 0007h
    rep movsb
    inc       di
    jmp       Label0x27aa               ;Offset 0x27aa
Label0x27a6:                            ;Offset 0x27a6
    mov       cx, dx
    rep movsb
Label0x27aa:                            ;Offset 0x27aa
    sub       di, dx
    add       di, 20h
    dec       ax
    jne       Label0x277a               ;Offset 0x277a
Label0x27b2:                            ;Offset 0x27b2
    or        bl, bl
    jns       Label0x27bf               ;Offset 0x27bf
    mov       dl, byte ptr [si]
    inc       si
    inc       cx
    or        dl, dl
    jne       Label0x275e               ;Offset 0x275e
    dec       cx
Label0x27bf:                            ;Offset 0x27bf
    or        bh, bh
    jne       Label0x27c5               ;Offset 0x27c5
    mov       bh, 0eh
Label0x27c5:                            ;Offset 0x27c5
    push      ds
    pop       es
    mov       ds, cx
    ret
Func0x2753 ENDP

;Offset 0x27ca
Data0x27ca              DB 000h, 040h, 080h, 0C0h, 020h, 060h, 0A0h, 0E0h

Func0x27d2 PROC NEAR                    ;Offset 0x27d2
    mov       al, bh
    cbw
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    dec       ax
    mov       ah, al
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne       Label0x27e9               ;Offset 0x27e9
    mov       al, VGA_CRTCIdx_UnderlineLoc;0x14
    out       dx, ax
Label0x27e9:                            ;Offset 0x27e9
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
    jb        Label0x27fe               ;Offset 0x27fe
    sub       ax, 0101h
Label0x27fe:                            ;Offset 0x27fe
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call      SetCursorEndStart         ;Offset 0x26a2
    call      GetNumberOfActiveScanlines;Offset 0x2a6a
    dec       al
    mov       ax, 00c8h
    js        Label0x2816               ;Offset 0x2816
    mov       ax, 015eh
    je        Label0x2816               ;Offset 0x2816
    mov       ax, 0190h
Label0x2816:                            ;Offset 0x2816
    div       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    inc       ax
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp       ax, 00c8h
    ja        Label0x282a               ;Offset 0x282a
    shl       ax, 01h
Label0x282a:                            ;Offset 0x282a
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    je        Label0x2834               ;Offset 0x2834
    shr       ax, 01h
Label0x2834:                            ;Offset 0x2834
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
Func0x27d2 ENDP

Func0x284b PROC NEAR                    ;Offset 0x284b
    push      cx
    push      dx
    jmp       Func0x285b                ;Offset 0x285b
Func0x284b ENDP

Func0x284f PROC NEAR                    ;Offset 0x284f
    push      cx
    push      dx
    call      Func0x29fa                ;Offset 0x29fa
    add       si, 0005h
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
Func0x284f ENDP
;continue!
Func0x285b PROC NEAR                    ;Offset 0x285b
    mov       di, dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       cx, 0004h
    mov       ax, (VGA_SEQ0_AsyncReset SHL 8) OR VGA_SEQIdx_Reset;0x100
    cli
    out       dx, ax
    inc       ax
    call      Func0x28b9                ;Offset 0x28b9
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    lodsb     byte ptr es:[si]
    out       dx, al
    push      cx
    mov       cx, 8000h
Label0x2875:                            ;Offset 0x2875
    loop      Label0x2875               ;Offset 0x2875  Speed sensitive!
    pop       cx
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8) OR VGA_SEQIdx_Reset;0x300
    out       dx, ax
    sti
    call      TurnScreenOff             ;Offset 0x2b1f
    push      ax
    mov       dx, di
    mov       ax, VGA_CRTCIdx_VertRetraceEnd;0x11
    out       dx, ax
    mov       cl, 19h
    xor       ax, ax
    call      Func0x28b9                ;Offset 0x28b9
    push      si
    add       dl, 06h                   ;Port 0x3?a
    in        al, dx
    xor       ax, ax
    out       dx, al
    mov       al, 10h
    add       si, ax
    mov       cl, 01h
    call      Func0x28cf                ;Offset 0x28cf
    inc       ax
    inc       si
    mov       cl, 02h
    call      Func0x28cf                ;Offset 0x28cf
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    xor       ax, ax
    call      Func0x28b9                ;Offset 0x28b9
    pop       si
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b21
    pop       dx
    pop       cx
    ret
Func0x285b ENDP

Func0x28b9 PROC NEAR                    ;Offset 0x28b9
Label0x28b9:                            ;Offset 0x28b9
    mov       ah, byte ptr es:[si]
    inc       si
    out       dx, ax
    inc       ax
    loop      Label0x28b9               ;Offset 0x28b9
    ret
Func0x28b9 ENDP

Func0x28c2 PROC NEAR                    ;Offset 0x28c2
    mov       al, 00h
Func0x28c2 ENDP
;continue!
Func0x28c4 PROC NEAR                    ;Offset 0x28c4
Label0x28c4:                            ;Offset 0x28c4
    out       dx, al
    push      ax
    inc       dx
    in        al, dx
    dec       dx
    stosb
    pop       ax
    inc       ax
    loop      Label0x28c4               ;Offset 0x28c4
    ret
Func0x28c4 ENDP

Func0x28cf PROC NEAR                    ;Offset 0x28cf
    mov       ah, 0ffh
Func0x28cf ENDP
;continue!
Func0x28d1 PROC NEAR                    ;Offset 0x28d1
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
Label0x28d4:                            ;Offset 0x28d4
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;Misassemble!
    jb        Label0x28dd               ;Offset 0x28dd
    ja        Label0x28e6               ;Offset 0x28e6
    inc       ax
    mov       cl, 01h
Label0x28dd:                            ;Offset 0x28dd
    out       dx, al
    push      ax
    lodsb     byte ptr es:[si]
    out       dx, al
    pop       ax
    inc       ax
    loop      Label0x28d4               ;Offset 0x28d4
Label0x28e6:                            ;Offset 0x28e6
    ret       
Func0x28d1 ENDP

Func0x28e7 PROC NEAR                    ;Offset 0x28e7
    mov       ah, 0ffh
    push      cx
    push      dx
    mov       al, 00h
    mov       cx, 0010h
    call      Func0x2be2                ;Offset 0x2be2
Label0x28f3:                            ;Offset 0x28f3
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
    js        Label0x290c               ;Offset 0x290c
    jne       Label0x28f3               ;Offset 0x28f3
    add       ax, 0101h
    jb        Label0x28f3               ;Offset 0x28f3
    inc       di
    jmp       Label0x28f3               ;Offset 0x28f3
Label0x290c:                            ;Offset 0x290c
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
Func0x28e7 ENDP

Func0x2915 PROC NEAR                    ;Offset 0x2915
    push      ds
    push      es
    push      si
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    pop       si
    pop       ds
    je        Label0x2928               ;Offset 0x2928
    mov       cx, 0008h
    rep movsw
    inc       si
    movsb
Label0x2928:                            ;Offset 0x2928
    pop       ds
    ret
Func0x2915 ENDP

Func0x292a PROC NEAR                    ;Offset 0x292a
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, 0ffh
    out       dx, al
    push      cs
    pop       es
    call      GetNumberOfActiveScanlines;Offset 0x2a6a
    mov       ah, al
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    ;cmp       ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 3Dh, 13h, 00h                    ;Misassemble!
    je        Label0x2959               ;Offset 0x2959
    lea       si, [Data0x2d5a]          ;Offset 0x2d5a
    jb        Label0x2955               ;Offset 0x2955
    lea       si, [Data0x2de2]          ;Offset 0x2de2
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0x2955               ;Offset 0x2955
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0x2955               ;Offset 0x2955
    lea       si, [Data0x2cd2]          ;Offset 0x2cd2
Label0x2955:                            ;Offset 0x2955
    call      Func0x2973                ;Offset 0x2973
    ret
Label0x2959:                            ;Offset 0x2959
    lea       si, [Data0x2e26]          ;Offset 0x2e26
    call      Func0x2973                ;Offset 0x2973
    lea       si, [Data0x2e4e]          ;Offset 0x2e4e
    mov       al, 10h
    call      Func0x2975                ;Offset 0x2975
    lea       si, [Data0x2e62]          ;Offset 0x2e62
    mov       al, 20h
    call      Func0x2975                ;Offset 0x2975
    ret       
Func0x292a ENDP

Func0x2973 PROC NEAR                    ;Offset 0x2973
    xor       al, al
Func0x2973 ENDP
;continue!
Func0x2975 PROC NEAR                    ;Offset 0x2975
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    inc       dx
    mov       ax, word ptr es:[si - 04h]
    mov       cx, word ptr es:[si - 02h]
    test      byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    je        Label0x2991               ;Offset 0x2991
    add       si, cx
    add       si, 0004h
    mov       ax, word ptr es:[si - 04h]
Label0x2991:                            ;Offset 0x2991
    or        ah, ah
    je        Label0x29a3               ;Offset 0x29a3
    jns       Func0x29a9                ;Offset 0x29a9
Label0x2997:                            ;Offset 0x2997
    lodsb     byte ptr es:[si]
    out       dx, al
    jmp       Label0x299c               ;Offset 0x299c Speed sensitive!
Label0x299c:                            ;Offset 0x299c
    out       dx, al
    jmp       Label0x299f               ;Offset 0x299f Speed Sensitive!
Label0x299f:                            ;Offset 0x299f
    out       dx, al
    loop      Label0x2997               ;Offset 0x2997
    ret
Label0x29a3:                            ;Offset 0x29a3
    push      bx
    call      Func0x2bb1                ;Offset 0x2bb1
    pop       bx
    ret
Func0x2975 ENDP

Func0x29a9 PROC NEAR                    ;Offset 0x29a9
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    jne       Label0x29bc               ;Offset 0x29bc
Label0x29b0:                            ;Offset 0x29b0
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    loop      Label0x29b0               ;Offset 0x29b0
    ret
Label0x29bc:                            ;Offset 0x29bc
    push      cx
    push      dx
    mov       dh, byte ptr es:[si]
    inc       si
    mov       ch, byte ptr es:[si]
    inc       si
    mov       cl, byte ptr es:[si]
    inc       si
    call      Func0x29e2                ;Offset 0x29e2
    pop       dx
    pop       cx
    out       dx, al
    jmp       Label0x29d2               ;Offset 0x29d2 Speed sensitive!
Label0x29d2:                            ;Offset 0x29d2
    out       dx, al
    jmp       Label0x29d5               ;Offset 0x29d5 Speed sensitive!
Label0x29d5:                            ;Offset 0x29d5
    out       dx, al
    loop      Label0x29bc               ;Offset 0x29bc
    ret
Func0x29a9 ENDP

Func0x29d9 PROC NEAR                    ;Offset 0x29d9
Label0x29d9:                            ;Offset 0x29d9
    in        al, dx
    stosb
    in        al, dx
    stosb
    in        al, dx
    stosb
    loop      Label0x29d9               ;Offset 0x29d9
    ret
Func0x29d9 ENDP

Func0x29e2 PROC NEAR                    ;Offset 0x29e2
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
Func0x29e2 ENDP

Func0x29fa PROC NEAR                    ;Offset 0x29fa
    mov       al, BDA_VPCB_VideoParameterTable;0x0
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b93
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    shl       ax, 01h
    shl       ax, 01h
    mov       di, ax
    call      GetNumberOfActiveScanlines;Offset 0x2a6a
    add       di, ax
    mov       al, byte ptr cs:[di + Data0x2a1a];Offset 0x2a1a
    mov       ah, 40h
    mul       ah
    add       si, ax
    ret
Func0x29fa ENDP

;
;                           +------------------------ 200 Scan lines
;                           |
;                           |     +------------------ 350 Scan lines
;                           |     |
;                           |     |     +------------ 400 Scan lines
;                           |     |     |
;                           |     |     |     +------ 480 Scan lines
;                           |     |     |     |
;Offset 0x2a1a              |     |     |     |       Display mode:
Data0x2a1a              DB 000h, 013h, 017h, 000h   ; 0x00
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
;   al = number of active scanlines
;        0 = 200
;        1 = 350
;        2 = 400
;        3 = 480
;
GetNumberOfActiveScanlines PROC NEAR       ;Offset 0x2a6a
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push  bx
    lea   bx, [NumberOfActiveScanlinesPerModePlusOne];Offset 0x2a96
    cmp   al, BDA_DM_320x200_256_Color_Graphics;0x13
    xlatb cs:[bx]
    ja    Label0x2a7c                   ;Offset 0x2a7c
    dec   al                            ;Subtract one to see if it turns negative (and thus, not set)
    jns   Label0x2a93                   ;Offset 0x2a93
Label0x2a7c:                            ;Offset 0x2a7c
    mov   al, 02h                       ;400 Scan lines
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    jne   Label0x2a93                   ;Offset 0x2a93
    dec   ax                            ;350 Scan lines
    call  IsMDAHiResEnhanced            ;Offset 0x2aaa
    je    Label0x2a93                   ;Offset 0x2a93
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je    Label0x2a93                   ;Offset 0x2a93
    dec   ax                            ;200 Scan lines
Label0x2a93:                            ;Offset 0x2a93
    pop   bx
    ret   
GetNumberOfActiveScanlines ENDP

    nop                                 ;hello

;
;
;                                               000h means it's determined another way
;                                               All others are decremented by one and returned.
;Offset 0x2a96                                    Display mode:
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

IsMDAHiResEnhanced PROC NEAR            ;Offset 0x2aaa
    push  ax
    mov   al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and   al, BDA_EFBS_AdapterTypeMask  ;0xf
    cmp   al, BDA_EFBS_MDAHiResEnhanced ;0x3
    je    Label0x2ab6                   ;Offset 0x2ab6
    cmp   al, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0x2ab6:                            ;Offset 0x2ab6
    pop   ax
    ret
IsMDAHiResEnhanced ENDP

Func0x2ab8 PROC NEAR                    ;Offset 0x2ab8
    push  ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call  Func0x60a3                    ;Offset 0x60a3
    cmp   al, BDA_DM_80x25_Monochrome_Text;0x7
    je    Label0x2ac9                   ;Offset 0x2ac9
    cmp   al, BDA_DM_80x25_16_Color_Text;0x3
    ja    Label0x2ac9                   ;Offset 0x2ac9
    xor   al, al
Label0x2ac9:                            ;Offset 0x2ac9
    pop   ax
    ret   
Func0x2ab8 ENDP

Func0x2acb PROC NEAR                    ;Offset 0x2acb
    push  si
    cbw
    xchg  ax, si
    add   si, di
Label0x2ad0:                            ;Offset 0x2ad0
    lodsb byte ptr es:[si]
    cmp   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je    Label0x2add                   ;Offset 0x2add
    inc   al
    jne   Label0x2ad0                   ;Offset 0x2ad0
    inc   ax
Label0x2add:                            ;Offset 0x2add
    pop   si
    ret   
Func0x2acb ENDP

Func0x2adf PROC NEAR                    ;Offset 0x2adf
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
    in    al, dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    pop   dx
    ret
Func0x2adf ENDP

Func0x2aef PROC NEAR                    ;Offset 0x2aef
    push  ax
    push  dx
    call  Func0x2be2                    ;Offset 0x2be2
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
Func0x2aef ENDP

Func0x2b03 PROC NEAR                    ;Offset 0x2b03
    push  dx
    call  Func0x2be2                    ;Offset 0x2be2
    push  dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, ah
    out   dx, al
    inc   dx
    in    al, dx
    pop   dx
    push  ax
    in    al, dx
    mov   dl, VGA_AttributeControllerDataW_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    sti
    pop   ax
    pop   dx
    ret
Func0x2b03 ENDP

TurnScreenOn PROC NEAR                  ;Offset 0x2b1b
    mov   ah, 00h
    jmp   ToggleScreenOnOff             ;Offset 0x2b21
TurnScreenOn ENDP

TurnScreenOff PROC NEAR                 ;Offset 0x2b1f
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
ToggleScreenOnOff PROC NEAR             ;Offset 0x2b21
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
SetDisplayCombinationCodeIndex PROC NEAR;Offset 0x2b36
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    NotFound                      ;Offset 0x2b56
    xor   dx, dx
    xor   cx, cx
    mov   cl, byte ptr es:[si]
    add   si, sizeof DisplayCombinationCodeTable;0x4 Skip display combination code header
Find:                                   ;Offset 0x2b47
    lodsw word ptr es:[si]
    cmp   ax, bx
    je    Found                         ;Offset 0x2b58
    xchg  al, ah
    cmp   ax, bx
    je    Found                         ;Offset 0x2b58
    inc   dx
    loop  Find                          ;Offset 0x2b47
NotFound:                               ;Offset 0x2b56
    mov   dl, 0ffh                      ;Entry not found set index to -1
Found:                                  ;Offset 0x2b58
    mov   byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], dl;Offset 0x48a
    ret
SetDisplayCombinationCodeIndex ENDP

Func0x2b5d PROC NEAR                    ;Offset 0x2b5d
    push  di
    push  es
    mov   bx, 0ffffh
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b93
    je    Label0x2b90                   ;Offset 0x2b90
    mov   al, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    cmp   al, byte ptr es:[si]
    jae   Label0x2b90                   ;Offset 0x2b90
    cbw
    shl   ax, 01h
    xchg  ax, bx
    mov   ax, word ptr es:[bx + si + 04h]
    mov   bx, ax
    cmp   al, 00h
    je    Label0x2b8e                   ;Offset 0x2b8e
    test  byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je    Label0x2b88                   ;Offset 0x2b88
    jp    Label0x2b8a                   ;Offset 0x2b8a
Label0x2b88:                            ;Offset 0x2b88
    xor   al, 01h
Label0x2b8a:                            ;Offset 0x2b8a
    test  al, 01h
    jne   Label0x2b90                   ;Offset 0x2b90
Label0x2b8e:                            ;Offset 0x2b8e
    xchg  bl, bh
Label0x2b90:                            ;Offset 0x2b90
    pop   es
    pop   di
    ret   
Func0x2b5d ENDP

LookupVideoParameterControlBlockPointer PROC NEAR;Offset 0x2b93
    cbw
    les   si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8    es:si = Video Save/Override Pointer table
    test  al, 02h                       ;Test if al bit 2 is set.
    je    DereferencePointerWithOffset  ;Offset 0x2ba5  if not set, then lookup in primary table
    push  ax                            ;else save ax
    mov   al, BDA_VPCB_SecondarySavePointerTbl;0x10 load offset 0x10 instead (secondary save pointer)
    call  DereferencePointerWithOffset  ;Offset 0x2ba5
    pop   ax                            ;restore ax
    je    Func0x2bb0                    ;Offset 0x2bb0  if previous call yielded null, return else call with original value
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
DereferencePointerWithOffset PROC NEAR  ;Offset 0x2ba5
    add   si, ax                        ;add ax offset to video save/override pointer
    les   si, es:[si]                   ;load es:si from table
    mov   di, si                        ;store si in di
    mov   ax, es                        ;store es in ax
    or    ax, si                        ;or ax and si (set flags)
DereferencePointerWithOffset ENDP
;continue!
Func0x2bb0 PROC NEAR                    ;Offset 0x2bb0
    ret
Func0x2bb0 ENDP

Func0x2bb1 PROC NEAR                    ;Offset 0x2bb1
Label0x2bb1:                            ;Offset 0x2bb1
    lodsb byte ptr es:[si]
    mov   ah, al
    mov   bx, 0003h
Label0x2bb8:                            ;Offset 0x2bb8
    shl   ah, 01h
    shl   ah, 01h
    xor   al, al
    test  ah, 0c0h
    je    Label0x2bcd                   ;Offset 0x2bcd
    jns   Label0x2bcb                   ;Offset 0x2bcb
    jnp   Label0x2bc9                   ;Offset 0x2bc9
    add   al, 15h
Label0x2bc9:                            ;Offset 0x2bc9
    add   al, 15h
Label0x2bcb:                            ;Offset 0x2bcb
    add   al, 15h
Label0x2bcd:                            ;Offset 0x2bcd
    out   dx, al
    dec   bx
    jne   Label0x2bb8                   ;Offset 0x2bb8
    loop  Label0x2bb1                   ;Offset 0x2bb1
    ret
Func0x2bb1 ENDP

Func0x2bd4 PROC NEAR                    ;Offset 0x2bd4
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2bdc:                            ;Offset 0x2bdc
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne   Label0x2bdc                   ;Offset 0x2bdc
    pop   ax
Func0x2bd4 ENDP
;continue!
Func0x2be2 PROC NEAR                    ;Offset 0x2be2
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2bea:                            ;Offset 0x2bea
    sti
    nop
    cli
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x2bea                   ;Offset 0x2bea
    pop   ax
    ret
Func0x2be2 ENDP

;
;inputs:
;   al * 8 * 1193 ticks time
;
Beep PROC NEAR                          ;Offset 0x2bf4
    mov   ah, al
    in    al, KB_PortB                  ;Port 0x61
    push  ax
    or    al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out   KB_PortB, al                  ;Port 0x61
    mov   al, ah
    call  Sleep2                        ;Offset 0x2c06
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
Sleep2 PROC NEAR                        ;Offset 0x2c06
    call  SetupSquareWave               ;Offset 0x2c46
    push  ax
    push  cx
    push  dx
    push  ds
    mov   dx, SYS_Segment               ;Segment 0xf000
    mov   ds, dx
    mov   dx, KB_CommandPort_XT         ;0x62
    cmp   byte ptr ds:[SYS_MachineID], SYS_MachineID_IBM_AT;Offset 0xfffe 0xfc
    jne   NotAT                         ;Offset 0x2c1d
    dec   dx
NotAT:                                  ;Offset 0x2c1d
    pop   ds
    mov   cl, al
    xor   ch, ch
    shl   cx, 01h
    shl   cx, 01h
    shl   cx, 01h
    jcxz  NoSleep                       ;Offset 0x2c42
    in    al, KB_PortB                  ;Port 0x61
    mov   ah, al                        ;store off original value
    or    al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out   KB_PortB, al                  ;Port 0x61
WaitTimeExpired:                        ;Offset 0x2c32
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne   WaitTimeExpired               ;Offset 0x2c32
WaitAgain:                              ;Offset 0x2c37
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    je    WaitAgain                     ;Offset 0x2c37
    loop  WaitTimeExpired               ;Offset 0x2c32
    mov   al, ah                        ;restore original value
    out   KB_PortB, al                  ;Port 0x61
NoSleep:                                ;Offset 0x2c42
    pop   dx
    pop   cx
    pop   ax
    ret
Sleep2 ENDP

SetupSquareWave PROC NEAR               ;Offset 0x2c46
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

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

PrintScreenHandler:                     ;Offset 0x2c60
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
    je   Label0x2cb4                    ;Offset 0x2cb4
    mov  ah, 0fh
    int  10h
    mov  bl, ah
    mov  ah, 03h
    int  10h
    push dx
    call PrintCRLF                      ;Offset 0x2cba
    mov  cl, 0ffh
    xor  dx, dx
Label0x2c84:                            ;Offset 0x2c84
    mov  ah, 02h
    int  10h
    mov  ah, 08h
    int  10h
    or   al, al
    jne  Label0x2c92                    ;Offset 0x2c92
    mov  al, 20h
Label0x2c92:                            ;Offset 0x2c92
    call PrintCharacter                 ;Offset 0x2cc1
    jne  Label0x2cab                    ;Offset 0x2cab
    inc  dx
    ;cmp  dl, bl
    DB 3Ah, 0D3h                        ;Misassemble!
    jb   Label0x2c84                    ;Offset 0x2c84
    call PrintCRLF                      ;Offset 0x2cba
    xor  dl, dl
    inc  dh
    cmp  dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe  Label0x2c84                    ;Offset 0x2c84
    xor  cl, cl
Label0x2cab:                            ;Offset 0x2cab
    pop  dx
    mov  ah, 02h
    int  10h
    mov  byte ptr ds:[BDA_PrintScreenStatus], cl;Offset 0x500
Label0x2cb4:                            ;Offset 0x2cb4
    pop  ds
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    iret

PrintCRLF PROC NEAR                     ;Offset 0x2cba
    mov  al, 0dh
    call PrintCharacter                 ;Offset 0x2cc1
    mov  al, 0ah
PrintCRLF ENDP
;continue!
PrintCharacter PROC NEAR                ;Offset 0x2cc1
    push dx
    xor  dx, dx
    xor  ah, ah
    int  17h
    test ah, 29h
    pop  dx
    ret
PrintCharacter ENDP

    nop                                 ;hello

;Offset 0x2cce
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2cd2
Data0x2cd2              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 028h, 02Ah, 001h, 003h, 009h, 00Bh, 021h, 023h, 029h, 02Bh
                        DB 004h, 006h, 00Ch, 00Eh, 024h, 026h, 02Ch, 02Eh, 005h, 007h, 00Dh, 00Fh, 025h, 027h, 02Dh, 02Fh
                        DB 010h, 012h, 018h, 01Ah, 030h, 032h, 038h, 03Ah, 011h, 013h, 019h, 01Bh, 031h, 033h, 039h, 03Bh
                        DB 014h, 016h, 01Ch, 01Eh, 034h, 036h, 03Ch, 03Eh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d12
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d16
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 025h, 028h, 002h, 007h, 01Bh, 020h, 00Fh, 014h, 028h, 02Ch
                        DB 00Ch, 011h, 025h, 02Ah, 014h, 01Eh, 032h, 036h, 00Fh, 013h, 027h, 02Ch, 01Bh, 020h, 034h, 039h
                        DB 006h, 00Bh, 01Fh, 024h, 013h, 018h, 02Ch, 030h, 009h, 00Dh, 021h, 026h, 015h, 01Ah, 02Eh, 033h
                        DB 013h, 017h, 02Bh, 030h, 01Fh, 024h, 038h, 03Dh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2d56
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d5a
Data0x2d5a              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d9a
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d9e
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2dde
                        DW 0000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2de2
Data0x2de2              DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
;Offset 0x2e22
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e26
Data0x2E26              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2e36
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e3a
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2e4a
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e4e
Data0x2e4e              DB 000h, 005h, 008h, 00Bh, 00Eh, 011h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Dh, 032h, 038h, 03Fh
;Offset 0x2e5e
                        DW 4000h        ;BDA_VideoDisplayDataArea
                        DW 00D8h        ;count (below) RGB, 0xd8 * 3
;Offset 0x2e62
Data0x2e62              DB 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 03Fh, 03Fh
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

;Offset 0x30ea
RowsOnScreen            DB 000h, 00Eh, 019h, 02Bh

;Offset 0x30ee
Data0x30ee              DW offset Font8x16;Offset 0x3900
                        DW offset Font8x8;Offset 0x3100
                        DW offset GraphicsFont8x8;Offset 0x3500
                        DW offset Data0x4900;Offset 0x4900
                        DW offset Font8x16;Offset 0x3900
                        DW offset Data0x4900;Offset 0x4900

;Offset 0x30fa
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx

;Offset 0x3100
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

;Offset 0x3500
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

;Offset 0x3900
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

;Offset 0x4900
Data0x4900              DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h
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

;Offset 0x4a44
                        DB 00Dh, 00Ah
                        DB 'MATROX POWER GRAPHICS ACCELERATOR', 00Dh, 00Ah
                        DB 'MGA Series', 00Dh, 00Ah
                        DB 'VGA/VBE BIOS, Version V2.1', 00Dh, 00Ah
                        DB 'Copyright (C) 1998, Matrox Graphics Inc.', 00Dh, 00Ah
                        DB 'Copyright (C) LSI Logic Corporation 1990-1991', 00Dh, 00Ah
                        DB 00Dh, 00Ah, 000h
;Offset 0x4aed
                        DB 'DATE: 11/21/00', 000h
;Offset 0x4afc
                        DB 'Revision: 0.35 ', 000h

VESAPowerManagement:                    ;Offset 0x4b0c
    cmp  bl, 00h
    jne  VPMNot0                        ;Offset 0x4b13
    jmp  VPMGetCaps                     ;Offset 0x4b25
VPMNot0:                                ;Offset 0x4b13
    cmp  bl, 01h
    jne  VPMNot1                        ;Offset 0x4b1a
    jmp  VPMSetDisplayPowerState        ;Offset 0x4b50
VPMNot1:                                ;Offset 0x4b1a
    cmp  bl, 02h
    jne  VPMNot2                        ;Offset 0x4b21
    jmp  VPMGetDisplayPowerState        ;Offset 0x4b2d
VPMNot2:                                ;Offset 0x4b21
    mov  ax, 014fh
    iret
VPMGetCaps:                             ;Offset 0x4b25
    mov  ax, 004fh
    mov  bh, 07h
    mov  bl, 10h
    iret
VPMGetDisplayPowerState:                ;Offset 0x4b2d
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    xor  bh, bh                         ;0 = Display is on
    and  al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    je   Label0x4b4b                    ;Offset 0x4b4b
    inc  bh                             ;1 = Display in standby
    cmp  al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    je   Label0x4b4b                    ;Offset 0x4b4b
    inc  bh                             ;2 = Display is suspended
    cmp  al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    je   Label0x4b4b                    ;Offset 0x4b4b
    shl  bh, 01h                        ;4 = Display is off
Label0x4b4b:                            ;Offset 0x4b4b
    mov  ax, 004fh
    pop  dx
    iret
VPMSetDisplayPowerState:                ;Offset 0x4b50
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop  
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    mov  al, bh
    and  bh, 0fh
    je   Label0x4b64                    ;Offset 0x4b64
    and  al, 07h
    je   Label0x4bb0                    ;Offset 0x4bb0
Label0x4b64:                            ;Offset 0x4b64
    in   al, dx
    and  al, NOT (MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff);0xcf
    or   bh, bh                         ;0 = Turn display on
    jne  Label0x4b81                    ;Offset 0x4b81
    out  dx, al
    call ScreenOn2                      ;Offset 0x4bc8
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    je   Label0x4b7c                    ;Offset 0x4b7c
    mov  al, 01h
    call Func0x5427                     ;Offset 0x5427
Label0x4b7c:                            ;Offset 0x4b7c
    mov  ax, 004fh
    jmp  Label0x4bb3                    ;Offset 0x4bb3
Label0x4b81:                            ;Offset 0x4b81
    cmp  bh, 01h                        ;1 = Set display to standby
    jne  Label0x4b8a                    ;Offset 0x4b8a
    or   al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    jmp  Label0x4b9a                    ;Offset 0x4b9a
Label0x4b8a:                            ;Offset 0x4b8a
    cmp  bh, 02h                        ;2 = Suspend display
    jne  Label0x4b93                    ;Offset 0x4b93
    or   al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    jmp  Label0x4b9a                    ;Offset 0x4b9a
Label0x4b93:                            ;Offset 0x4b93
    cmp  bh, 04h                        ;4 = Turn display off
    jne  Label0x4bb0                    ;Offset 0x4bb0
    or   al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
Label0x4b9a:                            ;Offset 0x4b9a
    out  dx, al
    call ScreenOff2                     ;Offset 0x4bb5
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    je   Label0x4bab                    ;Offset 0x4bab
    mov  al, 00h
    call Func0x5427                     ;Offset 0x5427
Label0x4bab:                            ;Offset 0x4bab
    mov  ax, 004fh
    jmp  Label0x4bb3                    ;Offset 0x4bb3
Label0x4bb0:                            ;Offset 0x4bb0
    mov  ax, 014fh
Label0x4bb3:                            ;Offset 0x4bb3
    pop  dx
    iret

ScreenOff2 PROC NEAR                    ;Offset 0x4bb5
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

ScreenOn2 PROC NEAR                     ;Offset 0x4bc8
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

;Offset 0x4bdb
                        DB 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
                        DB 000h, 000h

;These five tables are order dependent (There's a pointer compare in one of the functions)

;Offset 0x4be5
DDCFuncsNewer           DW offset DDCDisablePin1    ;Offset 0x4d42  ;0
                        DW offset DDCDisablePin3    ;Offset 0x4d26  ;2
                        DW offset DDCPin1SetOutput0 ;Offset 0x4d8d  ;4
                        DW offset DDCPin3SetOutput0 ;Offset 0x4d5e  ;6
                        DW offset DDCPins01Read     ;Offset 0x4e08  ;8
                        DW offset DDCPins23Read     ;Offset 0x4e2e  ;a

;Offset 0x4bf1
DDCFuncs                DW offset DDCDisablePin1    ;Offset 0x4d42  ;0
                        DW offset DDCDisablePin3    ;Offset 0x4d26  ;2
                        DW offset DDCPin1SetOutput0 ;Offset 0x4d8d  ;4
                        DW offset DDCPin3SetOutput0 ;Offset 0x4d5e  ;6
                        DW offset DDCPin1Read       ;Offset 0x4dbc  ;8
                        DW offset DDCPin3Read       ;Offset 0x4de2  ;a

;Offset 0x4bfd
Data0x4bfd              DW offset DDCDisablePin0    ;Offset 0x4e54  ;0
                        DW 0000h                    ;Offset 0x0000  ;2
                        DW offset DDCPin0SetOutput0 ;Offset 0x4e70  ;4
                        DW 0000h                    ;Offset 0x0000  ;6
                        DW offset DDCPin0Read       ;Offset 0x4e9f  ;8
                        DW offset DDCPin2Read       ;Offset 0x4ec5  ;a

;Offset 0x4c09
MISCFuncs               DW offset MISCDisablePin0   ;Offset 0x4f07  ;0
                        DW offset MISCDisablePin1   ;Offset 0x4eeb  ;2
                        DW offset MISCPin0SetOutput0;Offset 0x4f52  ;4
                        DW offset MISCPin1SetOutput0;Offset 0x4f23  ;6
                        DW offset MISCPin0Read      ;Offset 0x4f81  ;8
                        DW offset MISCPin1Read      ;Offset 0x4fa7  ;a

;Offset 0x4c15
Data0x4c15              DW offset Func0x500d        ;Offset 0x500d  ;0
                        DW offset Func0x4fcd        ;Offset 0x4fcd  ;2
                        DW offset Func0x5092        ;Offset 0x5092  ;4
                        DW offset Func0x504d        ;Offset 0x504d  ;6
                        DW offset Func0x50d9        ;Offset 0x50d9  ;8
                        DW offset Func0x50d7        ;Offset 0x50d7  ;a


VESADDC:                                ;Offset 0x4c21
    cmp  bl, 00h
    je   DDCInstallationCheck           ;Offset 0x4c48
    cmp  bl, 01h
    jne  DDCUnknownFunction             ;Offset 0x4c2f
    call DDCReadEDID                    ;Offset 0x4c75
    iret
DDCUnknownFunction:                     ;Offset 0x4c2f
    mov  ax, 0100h
    iret 

Func0x4c33 PROC NEAR                    ;Offset 0x4c33
    push cx
    mov  cx, 0000h
    call Func0x4cf7                     ;Offset 0x4cf7
    call Func0x517b                     ;Offset 0x517b
    mov  cx, 0001h
    call Func0x4cf7                     ;Offset 0x4cf7
    call Func0x517b                     ;Offset 0x517b
    pop  cx
    ret
Func0x4c33 ENDP

DDCInstallationCheck:                   ;Offset 0x4c48
    push es
    push di
    cli
    push ss
    pop  es
    push bp
    mov  bp, sp
    sub  sp, 0080h
    mov  di, sp
    xor  bx, bx
    call DDCReadEDID                    ;Offset 0x4c75
    or   ah, ah
    jne  Label0x4c62                    ;Offset 0x4c62
    or   bl, 02h
Label0x4c62:                            ;Offset 0x4c62
    mov  bh, 01h
    or   bl, bl
    jne  Label0x4c6a                    ;Offset 0x4c6a
    mov  bh, 00h
Label0x4c6a:                            ;Offset 0x4c6a
    mov  ah, 00h
    mov  al, 4fh
    mov  sp, bp
    pop  bp
    sti
    pop  di
    pop  es
    iret

DDCReadEDID PROC NEAR                   ;Offset 0x4c75
    push ds
    push es
    push bx
    push si
    push ecx
    push edx
    push edi
    mov  ax, 014fh
    test cx, 0fffeh
    jne  Label0x4cec                    ;Offset 0x4cec
    call Func0x4cf7                     ;Offset 0x4cf7
    cli
    push 0000h
    pop  ds
    push word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    push ds
    mov  word ptr ds:[BDA_SoftResetFlag], 0ffefh;Offset 0x472
    mov  ax, di
    shl  edi, 10h
    mov  di, ax
    push es
    pop  ds
    mov  word ptr [di], 0000h
    test dl, 01h
    je   Label0x4cb1                    ;Offset 0x4cb1
    mov  word ptr [di], 0080h
Label0x4cb1:                            ;Offset 0x4cb1
    shr  dx, 01h
    mov  cx, dx
    shl  ecx, 10h
    mov  dx, 0080h
    shl  edx, 10h
    mov  dx, 0001h
    mov  cl, 04h
    mov  ch, 0a0h
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
Label0x4ccc:                            ;Offset 0x4ccc
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    je   Label0x4cdf                    ;Offset 0x4cdf
    call Func0x5407                     ;Offset 0x5407
    or   ah, ah
    jne  Label0x4cdf                    ;Offset 0x4cdf
    mov  ax, 004fh
    jmp  Label0x4ce6                    ;Offset 0x4ce6
Label0x4cdf:                            ;Offset 0x4cdf
    dec  cl
    jne  Label0x4ccc                    ;Offset 0x4ccc
    mov  ax, 014fh
Label0x4ce6:                            ;Offset 0x4ce6
    pop  ds
    pop  word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    sti
Label0x4cec:                            ;Offset 0x4cec
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  bx
    pop  es
    pop  ds
    ret
DDCReadEDID ENDP

Func0x4cf7 PROC NEAR                    ;Offset 0x4cf7
    mov  al, 00h
    test byte ptr cs:[Data0x7d3b], 40h  ;Offset 0x7d3b
    je   Label0x4d0d                    ;Offset 0x4d0d
    mov  al, 01h
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    jne  Label0x4d0d                    ;Offset 0x4d0d
    mov  al, 00h
Label0x4d0d:                            ;Offset 0x4d0d
    lea  si, [Data0x4c15]               ;Offset 0x4c15
    xor  al, cl
    jne  Label0x4d25                    ;Offset 0x4d25
    lea  si, [DDCFuncs]                 ;Offset 0x4bf1
    test byte ptr cs:[Data0x7d3d], 08h  ;Offset 0x7d3d
    jne  Label0x4d25                    ;Offset 0x4d25
    lea  si, [DDCFuncsNewer]            ;Offset 0x4be5
Label0x4d25:                            ;Offset 0x4d25
    ret
Func0x4cf7 ENDP

DDCDisablePin3 PROC NEAR                ;Offset 0x4d26
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCDisablePin3 ENDP

DDCDisablePin1 PROC NEAR                ;Offset 0x4d42
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCDisablePin1 ENDP

DDCPin3SetOutput0 PROC NEAR             ;Offset 0x4d5e
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC3Data ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_GENIOCTRL_DDC3Enable   ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCPin3SetOutput0 ENDP

DDCPin1SetOutput0 PROC NEAR             ;Offset 0x4d8d
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC1Data ;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_GENIOCTRL_DDC1Enable   ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCPin1SetOutput0 ENDP

DDCPin1Read PROC NEAR                   ;Offset 0x4dbc
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 07h
    pop  edx
    pop  ecx
    ret
DDCPin1Read ENDP

DDCPin3Read PROC NEAR                   ;Offset 0x4de2
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 05h
    pop  edx
    pop  ecx
    ret
DDCPin3Read ENDP

DDCPins01Read PROC NEAR                 ;Offset 0x4e08
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT (MGA_GENIOCTRL_DDC0Enable OR MGA_GENIOCTRL_DDC1Enable);0xfc
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 08h
    pop  edx
    pop  ecx
    ret
DDCPins01Read ENDP

DDCPins23Read PROC NEAR                 ;Offset 0x4e2e
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT (MGA_GENIOCTRL_DDC2Enable OR MGA_GENIOCTRL_DDC3Enable);0xf3
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 06h
    pop  edx
    pop  ecx
    ret
DDCPins23Read ENDP

DDCDisablePin0 PROC NEAR                ;Offset 0x4e54
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC0Enable;0xfe
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCDisablePin0 ENDP

DDCPin0SetOutput0 PROC NEAR             ;Offset 0x4e70
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC0Data ;0xfe
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_GENIOCTRL_DDC0Enable   ;0x1
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
DDCPin0SetOutput0 ENDP

DDCPin0Read PROC NEAR                   ;Offset 0x4e9f
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC0Enable;0xfe
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 08h
    pop  edx
    pop  ecx
    ret
DDCPin0Read ENDP

DDCPin2Read PROC NEAR                   ;Offset 0x4ec5
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_DDC2Enable;0xfb
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 06h
    pop  edx
    pop  ecx
    ret
DDCPin2Read ENDP

MISCDisablePin1 PROC NEAR               ;Offset 0x4eeb
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
MISCDisablePin1 ENDP

MISCDisablePin0 PROC NEAR               ;Offset 0x4f07
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
MISCDisablePin0 ENDP

MISCPin1SetOutput0 PROC NEAR            ;Offset 0x4f23
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc1Data;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_GENIOCTRL_Misc1Enable  ;20h
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
MISCPin1SetOutput0 ENDP

MISCPin0SetOutput0 PROC NEAR            ;Offset 0x4f52
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc0Data;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_GENIOCTRL_Misc0Enable  ;0x10
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  edx
    pop  ecx
    ret
MISCPin0SetOutput0 ENDP

MISCPin0Read PROC NEAR                  ;Offset 0x4f81
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 04h
    pop  edx
    pop  ecx
    ret
MISCPin0Read ENDP

MISCPin1Read PROC NEAR                    ;Offset 0x4fa7
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    shl  cl, 03h
    pop  edx
    pop  ecx
    ret
MISCPin1Read ENDP

Func0x4fcd PROC NEAR                    ;Offset 0x4fcd
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4c09
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 41h;Offset 0x472 0x41
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x4ff7:                            ;Offset 0x4ff7
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    ;jne  Label0x5004                    ;Offset 0x5004
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x4ff7                    ;Offset 0x4ff7
Label0x5004:                            ;Offset 0x5004
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x4fcd ENDP

Func0x500d PROC NEAR                    ;Offset 0x500d
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4c09
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 42h;Offset 0x472 0x42
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x5037:                            ;Offset 0x5037
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    ;jne  Label0x5044                    ;Offset 0x5044
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x5037                    ;Offset 0x5037
Label0x5044:                            ;Offset 0x5044
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x500d ENDP

Func0x504d PROC NEAR                    ;Offset 0x504d
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4c09
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    and  byte ptr ds:[BDA_SoftResetFlag], 0feh ;Offset 0x472 0xfe
    or   byte ptr ds:[BDA_SoftResetFlag], 40h ;Offset 0x472 0x40
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x507c:                            ;Offset 0x507c
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    ;jne  Label0x5089                    ;Offset 0x5089
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x507c                    ;Offset 0x507c
Label0x5089:                            ;Offset 0x5089
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x504d ENDP

Func0x5092 PROC NEAR                    ;Offset 0x5092
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4c09
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    and  byte ptr ds:[BDA_SoftResetFlag], 0fdh;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 40h;Offset 0x472
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x50c1:                            ;Offset 0x50c1
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    ;jne  Label0x50ce                    ;Offset 0x50ce
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x50c1                    ;Offset 0x50c1
Label0x50ce:                            ;Offset 0x50ce
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x5092 ENDP

Func0x50d7 PROC NEAR                    ;Offset 0x50d7
    stc
    ret
Func0x50d7 ENDP

Func0x50d9 PROC NEAR                    ;Offset 0x50d9
    push ds
    push es
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  es
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4c09
    mov  di, BDA_SoftResetFlagHighByte  ;Offset 0x473
    shl  edi, 10h
    mov  edx, 00010000h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x5103:                            ;Offset 0x5103
    call Func0x52fa                     ;Offset 0x52fa
    or   ax, ax
    ;jne  Label0x5110                    ;Offset 0x5110
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x5103                    ;Offset 0x5103
Label0x5110:                            ;Offset 0x5110
    mov  al, byte ptr es:[BDA_SoftResetFlagHighByte];Offset 0x473
    shl  al, 07h
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  es
    pop  ds
    ret
Func0x50d9 ENDP

;
;inputs:
;   si = pointer to data
;
;outputs:
;   -
;
;destroys:
;   -
;
SleepIfNeeded PROC NEAR                 ;Offset 0x5121
    cmp  si, offset DDCFuncs            ;Offset 0x4bf1
    ja   DontSleep                      ;Offset 0x512f
    push dx
    mov  dx, 0d2h
    call Sleep                          ;Offset 0x5130
    pop  dx
DontSleep:                              ;Offset 0x512f
    ret
SleepIfNeeded ENDP

;
;input:
;   dx = ticks
;
;output:
;   -
;
;destroys:
;   -
;
Sleep PROC NEAR                         ;Offset 0x5130
    push ax
    push bx
    push cx
    push edx
    mov  al, PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2;0xb0
    out  PIT_ModeCommand, al            ;Port 0x43
    jmp  Delay1                         ;Offset 0x513b Speed sensitive!
Delay1:                                 ;Offset 0x513b
    mov  ax, dx
    out  PIT_Channel2Data, al           ;Port 0x42
    jmp  Delay2                         ;Offset 0x5141 Speed sensitive!
Delay2:                                 ;Offset 0x5141
    mov  al, ah
    out  PIT_Channel2Data, al           ;Port 0x42
    jmp  Delay3                         ;Offset 0x5147 Speed sensitive!
Delay3:                                 ;Offset 0x5147
    in   al, KB_PortB                   ;Port 0x61
    mov  cl, al
    and  cl, KB_PBR_LowerNibbleMask     ;0xf
    and  al, KB_PBR_Timer2ClkGateSpeakerStatus OR KB_PBR_NMIParityCheckStatus OR KB_PBR_NMIIOChanCheckStatus;0xd
    or   al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out  KB_PortB, al                   ;Port 0x61
    jmp  Delay4                         ;Offset 0x5156 Speed sensitive!
Delay4:                                 ;Offset 0x5156
    mov  edx, 004c4b40h                 ;5,000,000 time out
WaitLoop:                               ;Offset 0x515c
    in   al, KB_PortB                   ;0x61
    test al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne  TimeOut                        ;Offset 0x5166
    dec  edx
    jne  WaitLoop                       ;Offset 0x515c
TimeOut:                                ;Offset 0x5166
    mov  al, cl
    out  KB_PortB, al                   ;Port 0x61
    mov  al, PIT_MC_OpMode_SquareWaveGenerator OR PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2;0b6h
    out  PIT_ModeCommand, al            ;Port 0x43
    mov  ax, 0000h
    out  PIT_Channel2Data, al           ;Port 0x42
    out  PIT_Channel2Data, al           ;Port 0x42
    pop  edx
    pop  cx
    pop  bx
    pop  ax
    ret
Sleep ENDP

Func0x517b PROC NEAR                    ;Offset 0x517b
    push cx
    push ds
    push 0000h
    pop  ds
    push word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    push ds
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
    mov  cx, 0009h
Label0x518e:
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable1];0x02
    call SleepIfNeeded                  ;Offset 0x5121
    loop Label0x518e                    ;Offset 0x518e
    pop  ds
    pop  word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    pop  ds
    pop  cx
    ret
Func0x517b ENDP

Func0x51a6 PROC NEAR                    ;Offset 0x51a6
    push ecx
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call SleepIfNeeded                  ;Offset 0x5121
    pop  ecx
    ret
Func0x51a6 ENDP

Func0x51b2 PROC NEAR                    ;Offset 0x51b2
    push ecx
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable1];0x02
    mov  cl, 0ah
Label0x51c0:                            ;Offset 0x51c0
    call SleepIfNeeded                  ;Offset 0x5121
    dec  cl
    je   TimeOut                        ;Offset 0x51de
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x51c0                    ;Offset 0x51c0
    call word ptr cs:[si + MGA_DDCRead0];0x08
    jae  Label0x51c0                    ;Offset 0x51c0
    call word ptr cs:[si + MGA_DDCSet0] ;0x04
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
TimeOut:                                ;Offset 0x51de
    mov  al, cl
    pop  ecx
    ret
Func0x51b2 ENDP

TurnOffOutput PROC NEAR                 ;Offset 0x51e3
    push ecx
    call word ptr cs:[si + MGA_DDCSet0] ;0x04
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable1];0x02
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
    pop  ecx
    ret
TurnOffOutput ENDP

Func0x5203 PROC NEAR                    ;Offset 0x5203
    push ecx
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    mov  cl, 0ah
Label0x520a:                            ;Offset 0x520a
    dec  cl
    je   Label0x5231                    ;Offset 0x5231
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead0];0x08
    jb   Label0x520a                    ;Offset 0x520a
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCDisable1];0x02
Label0x521d:                            ;Offset 0x521d
    dec  cl
    je   Label0x5231                    ;Offset 0x5231
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x521d                    ;Offset 0x521d
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call SleepIfNeeded                  ;Offset 0x5121
Label0x5231:                            ;Offset 0x5231
    mov  al, cl
    pop  ecx
    ret
Func0x5203 ENDP

Func0x5236 PROC NEAR                    ;Offset 0x5236
    push ecx
    call word ptr cs:[si + MGA_DDCSet0] ;0x04
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable1];0x02
    mov  cl, 0ah
Label0x5245:                            ;Offset 0x5245
    dec  cl
    je   Label0x525c                    ;Offset 0x525c
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x5245                    ;Offset 0x5245
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
Label0x525c:                            ;Offset 0x525c
    mov  al, cl
    pop  ecx
    ret
Func0x5236 ENDP

Func0x5261 PROC NEAR                    ;Offset 0x5261
    push ecx
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCDisable1];0x02
Label0x526f:                            ;Offset 0x526f
    dec  cl
    je   Label0x5286                    ;Offset 0x5286
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x526f                    ;Offset 0x526f
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call word ptr cs:[si + MGA_DDCDisable0];0x00
    call SleepIfNeeded                  ;Offset 0x5121
Label0x5286:                            ;Offset 0x5286
    mov  al, cl
    pop  ecx
    ret
Func0x5261 ENDP

Func0x528b PROC NEAR                    ;Offset 0x528b
    push ecx
    push edx
    mov  cl, 08h
    mov  ch, al
Label0x5293:                            ;Offset 0x5293
    shl  ch, 01h
    jb   Label0x529d                    ;Offset 0x529d
    call word ptr cs:[si + MGA_DDCSet0] ;0x04
    jmp  Label0x52a0                    ;Offset 0x52a0
Label0x529d:                            ;Offset 0x529d
    call word ptr cs:[si + MGA_DDCDisable0];0x00
Label0x52a0:                            ;Offseet 0x52a0
    mov  dl, 0ah
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCDisable1];0x02
Label0x52a9:                            ;Offset 0x52a9
    dec  dl
    je   Label0x52be                    ;Offset 0x52be
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x52a9                    ;Offset 0x52a9
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    dec  cl
    jne  Label0x5293                    ;Offset 0x5293
Label0x52be:                            ;Offset 0x52be
    mov  al, dl
    pop  edx
    pop  ecx
    ret
Func0x528b ENDP

Func0x52c5 PROC NEAR                    ;Offset 0x52c5
    push ecx
    push edx
    xor  ch, ch
    mov  cl, 08h
Label0x52cd:                            ;Offset 0x52cd
    mov  dl, 0ah
    call word ptr cs:[si + MGA_DDCDisable1];0x02
Label0x52d3:                            ;Offset 0x52d3
    dec  dl
    je   Label0x52f3                    ;Offset 0x52f3
    call SleepIfNeeded                  ;Offset 0x5121
    call word ptr cs:[si + MGA_DDCRead1];0x0a
    jae  Label0x52d3                    ;Offset 0x52d3
    call word ptr cs:[si + MGA_DDCRead0];0x08
    rcl  ch, 01h
    call word ptr cs:[si + MGA_DDCSet1] ;0x06
    call SleepIfNeeded                  ;Offset 0x5121
    dec  cl
    jne  Label0x52cd                    ;Offset 0x52cd
    mov  al, ch
Label0x52f3:                            ;Offset 0x52f3
    mov  ah, dl
    pop  edx
    pop  ecx
    ret
Func0x52c5 ENDP

Func0x52fa PROC NEAR                    ;Offset 0x52fa
    push     ecx
    shr      ecx, 08h
    xchg     ch, cl
    and      ecx, 0000ffffh
    cmp      ch, 0a0h
    jne      Label0x5311                ;Offset 0x5311
    call     Func0x51a6                 ;Offset 0x51a6
Label0x5311:                            ;Offset 0x5311
    cmp      cl, 00h
    je       Label0x533b                ;Offset 0x533b
    call     Func0x51b2                 ;Offset 0x51b2
    or       al, al
    je       NotDetected                ;Offset 0x53f1
    mov      al, 60h
    call     Func0x528b                 ;Offset 0x528b
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    call     Func0x5203                 ;Offset 0x5203
    mov      al, cl
    call     Func0x528b                 ;Offset 0x528b
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    call     Func0x5203                 ;Offset 0x5203
Label0x533b:                            ;Offset 0x533b
    mov      cl, 01h
Label0x533d:                            ;Offset 0x533d
    push     edx
    push     edi
    call     Func0x51b2                 ;Offset 0x51b2
    or       al, al
    je       NotDetected                ;Offset 0x53f1
    or       dx, dx
    ;je       Label0x5398                ;Offset 0x5398
    DB 00Fh, 084h, 048h, 000h           ;Weird misassemble
    mov      al, ch
    and      al, 0feh
    call     Func0x528b                 ;Offset 0x528b
    or       al, al
    ;je       Label0x53e2                ;Offset 0x53e2
    DB 00Fh, 084h, 085h, 000h           ;Weird misassemble
    call     Func0x5203                 ;Offset 0x5203
    or       al, al
    ;je       Label0x53e2                ;Offset 0x53e2
    DB 00Fh, 084h, 07Ch, 000h           ;Weird misassemble
Label0x5366:                            ;Offset 0x5366
    mov      al, byte ptr [di]
    call     Func0x528b                 ;Offset 0x528b
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    call     Func0x5203                 ;Offset 0x5203
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    inc      di
    dec      dx
    jne      Label0x5366                ;Offset 0x5366
    or       edx, edx
    ;je       Label0x53d9                ;Offset 0x53d9
    DB 00Fh, 084h, 058h, 000h           ;Weird misassemble
    and      ecx, 0000ffffh
    mov      al, 05h
Label0x538a:                            ;Offset 0x538a
    call     SleepIfNeeded              ;Offset 0x5121
    dec      al
    jne      Label0x538a                ;Offset 0x538a
    call     Func0x51b2                 ;Offset 0x51b2
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
Label0x5398:                            ;Offset 0x5398
    mov      al, ch
    or       al, 01h
    call     Func0x528b                 ;Offset 0x528b
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    call     Func0x5203                 ;Offset 0x5203
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    shr      edi, 10h
    shr      edx, 10h
    dec      dx
    ;je       Label0x53cc                ;Offset 0x53cc
    DB 00Fh, 084h, 015h, 000h           ;Weird misassemble
Label0x53b7:                            ;Offset 0x53b7
    call     Func0x52c5                 ;Offset 0x52c5
    or       ah, ah
    je       Label0x53e2                ;Offset 0x53e2
    mov      byte ptr es:[di], al
    call     Func0x5236                 ;Offset 0x5236
    or       al, al
    je       Label0x53e2                ;Offset 0x53e2
    inc      di
    dec      dx
    jne      Label0x53b7                ;Offset 0x53b7
Label0x53cc:                            ;Offset 0x53cc
    call     Func0x52c5                 ;Offset 0x52c5
    or       ah, ah
    je       Label0x53e2                ;Offset 0x53e2
    mov      byte ptr es:[di], al
    call     Func0x5261                 ;Offset 0x5261
Label0x53d9:                            ;Offset 0x53d9
    mov      cl, 01h
    or       ecx, 00010000h
Label0x53e2:                            ;Offset 0x53e2
    call     SleepIfNeeded              ;Offset 0x5121
    call     TurnOffOutput              ;Offset 0x51e3
    mov      al, 05h
Delay:                                  ;Offset 0x53ea
    call     SleepIfNeeded              ;Offset 0x5121
    dec      al
    jne      Delay                      ;Offset 0x53ea
NotDetected:                            ;Offset 0x53f1
    pop      edi
    pop      edx
    dec      cl
    jne      Label0x533d                ;Offset 0x533d
    shr      ecx, 10h
    mov      ax, cx
    call     Func0x6b4d                 ;Offset 0x6b4d
    pop      ecx
    ret
Func0x52fa ENDP

Func0x5407 PROC NEAR                    ;Offset 0x5407
    push     ds
    push     edx
    push     esi
    push     es
    pop      ds
    xor      ax, ax
    mov      esi, edi
    shr      esi, 10h
    shr      edx, 10h
Label0x541b:                            ;Offset 0x541b
    lodsb
    add      ah, al
    dec      dx
    jne      Label0x541b                ;Offset 0x541b
    pop      esi
    pop      edx
    pop      ds
    ret      
Func0x5407 ENDP

Func0x5427 PROC NEAR                    ;Offset 0x5427
    push     ds
    push     bx
    push     si
    push     di
    push     ecx
    push     edx
    cli
    push     0000h
    pop      ds
    push     word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    mov      word ptr ds:[BDA_SoftResetFlag], 00ffh;Offset 0x472
    or       al, al
    ;je       Label0x5449                ;Offset 0x5449
    DB 00Fh, 084h, 006h, 000h           ;Misassemble
    mov      word ptr ds:[BDA_SoftResetFlag], 0efh;Offset 0x472
Label0x5449:                            ;Offset 0x5449
    mov      bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    and      ecx, 0000ffffh
    mov      ch, byte ptr cs:[Data0x7ff5];Offset 0x7ff5
    mov      si, offset MISCFuncs       ;Offset 0x4c09
    mov      di, BDA_SoftResetFlag      ;Offset 0x472
    mov      edx, 00000001h
    call     Func0x52fa                 ;Offset 0x52fa
    pop      word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    sti
    pop      edx
    pop      ecx
    pop      di
    pop      si
    pop      bx
    pop      ds
    ret
Func0x5427 ENDP

;Offset 0x5477
SupportedVESAVideoModes DW 0101h        ;640x480x256
                        DW 0110h        ;640x480x32K
                        DW 0111h        ;640x480x64K
                        DW 0112h        ;640x480x16M

;Offset 0x547f
Data0x547f              DW 0100h        ;640x400x256

;Offset 0x5481
Data0x5481              DW 0102h        ;800x600x16
                        DW 0103h        ;800x600x256
                        DW 0113h        ;800x600x32K
                        DW 0114h        ;800x600x64K
                        DW 0115h        ;800x600x16M

;Offset 0x548b
Data0x548b              DW 0105h        ;1024x768x256
                        DW 0116h        ;1024x768x32K
                        DW 0117h        ;1024x768x64K
                        DW 0118h        ;1024x768x16M

;Offset 0x5493                        
Data0x5493              DW 0107h        ;1280x1024x256
                        DW 0119h        ;1280x1024x32K
                        DW 011ah        ;1280x1024x64K
                        DW 011bh        ;1280x1024x16M

;Offset 0x549b
Data0x549b              DW 010ah        ;132x43 text
                        DW 0FFFFh

;Offset 0x549f
Data0x549f              DW offset Data0x56f1, 0010h     ;Offset 0x56f1  0x100   640x400x256
                        DW offset Data0x56ff, 0010h     ;Offset 0x56ff  0x101   640x480x256
                        DW offset Data0x5737, 0000h     ;Offset 0x5737  0x102   800x600x16
                        DW offset Data0x570d, 0010h     ;Offset 0x570d  0x103   800x600x256
                        DW 0000h, 0000h
                        DW offset Data0x571b, 0010h     ;Offset 0x571b  0x105   1024x768x256
                        DW 0000h, 0000h
                        DW offset Data0x5729, 0010h     ;Offset 0x5729  0x107   1280x1024x256
                        DW 0000h, 00c0h
                        DW 0000h, 0040h
                        DW offset Data0x5745, 00c0h     ;Offset 0x5745  0x10a   132x43 text
                        DW 0000h, 00c0h
                        DW 0000h, 00c0h
                        DW 0000h, 0000h
                        DW 0000h, 0000h
                        DW 0000h, 0000h
                        DW offset Data0x56ff, 0020h     ;Offset 0x56ff  0x110   640x480x32K
                        DW offset Data0x56ff, 0060h     ;Offset 0x56ff  0x111   640x480x64K
                        DW offset Data0x56ff, 0030h     ;Offset 0x56ff  0x112   640x480x16M
                        DW offset Data0x570d, 0020h     ;Offset 0x570d  0x113   800x600x32K
                        DW offset Data0x570d, 0060h     ;Offset 0x570d  0x114   800x600x64K
                        DW offset Data0x570d, 0030h     ;Offset 0x570d  0x115   800x600x16M
                        DW offset Data0x571b, 0020h     ;Offset 0x571b  0x116   1024x768x32K
                        DW offset Data0x571b, 0060h     ;Offset 0x571b  0x117   1024x768x64K
                        DW offset Data0x571b, 0030h     ;Offset 0x571b  0x118   1024x768x16M
                        DW offset Data0x5729, 0020h     ;Offset 0x5729  0x119   1280x1024x32K
                        DW offset Data0x5729, 0060h     ;Offset 0x5729  0x11a   1280x1024x64K
                        DW offset Data0x5729, 0030h     ;Offset 0x5729  0x11b   1280x1024x16M
                        DW offset Data0x56f1, 0010h     ;Offset 0x56f1  0x11c   1600x1200x256

;Offset 0x5513
Data0x5513              DB 008h, 010h, 001h, 008h, 001h, 004h, 000h

;Offset 0x551a
Data0x551a              DB 005h, 00Ah, 005h, 005h, 005h, 000h, 001h, 00Fh

;Offset 0x5522
Data0x5522              DB 005h, 00Bh, 006h, 005h, 005h, 000h, 000h, 000h

;Offset 0x552a
Data0x552a              DB 008h, 010h, 008h, 008h, 008h, 000h, 008h, 018h

;Offset 0x5532
Data0x5532              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 06Fh, 05Fh, 04Fh, 04Fh, 003h, 051h, 09Dh
                        DB 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0C1h, 023h, 08Fh, 050h, 000h, 08Fh
                        DB 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x5572
Data0x5572              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 05Fh, 04Fh, 04Fh, 003h, 051h, 09Dh
                        DB 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0E9h, 0ABh, 0DFh, 050h, 000h, 0DFh
                        DB 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x55b2
Data0x55b2              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 07Fh, 063h, 063h, 003h, 068h, 018h
                        DB 072h, 0F0h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 064h, 000h, 057h
                        DB 073h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x55f2
Data0x55f2              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 0A3h, 07Fh, 07Fh, 007h, 082h, 093h
                        DB 024h, 0F5h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 002h, 0A8h, 0FFh, 080h, 000h, 0FFh
                        DB 025h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x5632
Data0x5632              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 0CEh, 09Fh, 09Fh, 012h, 0A5h, 013h
                        DB 028h, 05Ah, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 0A3h, 0FFh, 0A0h, 000h, 0FFh
                        DB 029h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x5671
Data0x5671              DB 064h, 024h, 010h, 000h, 0F0h, 001h, 00Fh, 000h, 006h, 02Fh

;Offset 0x567b
Data0x567b              DB 07Ah, 063h, 063h, 080h, 06Bh, 01Bh
                        DB 072h, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 032h, 000h, 057h
                        DB 000h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x56b1
Data0x56b1              DB 084h, 02Ah, 008h, 058h, 02Ch, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh
                        DB 0CBh, 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 09Bh, 0ADh, 057h, 042h, 01Fh, 05Bh
                        DB 0C8h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x56f1
Data0x56f1              DB 057h, 062h, 000h, 000h
                        DW 0280h        ;640
                        DW 0190h        ;400
                        DB 01Ch, 013h
                        DW offset Data0x5532;Offset 0x5532
                        DB 040h, 000h

;Offset 0x56ff
Data0x56ff              DB 057h, 062h, 000h, 000h
                        DW 0280h        ;640
                        DW 01E0h        ;480
                        DB 01Ch, 013h
                        DW offset Data0x5572;Offset 0x5572
                        DB 040h, 000h
                        
;Offset 0x570d
Data0x570d              DB 040h, 09Ch, 000h, 000h
                        DW 0320h        ;800
                        DW 0258h        ;600
                        DB 01Ch, 013h
                        DW offset Data0x55b2;Offset 0x55b2
                        DB 000h, 000h

;Offset 0x571b
Data0x571b              DB 0E8h, 0FDh, 000h, 000h
                        DW 0400h        ;1024
                        DW 0300h        ;768
                        DB 01Ch, 013h
                        DW offset Data0x55f2;Offset 0x55f2
                        DB 000h, 000h
                        
;Offset 0x5729
Data0x5729              DB 0E0h, 0A5h, 001h, 000h
                        DW 0500h        ;1280
                        DW 0400h        ;1024
                        DB 01Ch, 013h
                        DW offset Data0x5632;Offset 0x5632
                        DB 040h, 021h

;Offset 0x5737
Data0x5737              DB 058h, 098h, 000h, 000h
                        DW 0320h        ;800
                        DW 0258h        ;600
                        DB 01Bh, 012h
                        DW offset Data0x5671;Offset 0x5671
                        DB 000h, 000h

;Offset 0x5745
Data0x5745              DB 078h, 0A0h, 000h, 000h
                        DW 0420h        ;1056
                        DW 0158h        ;344
                        DB 018h, 003h
                        DW offset Data0x56b1;Offset 0x56b1
                        DB 000h, 000h

;Offset 0x5753
VESACallTable           DW offset GetSuperVGAInformation;Offset 0x584f          ;0
                        DW offset GetSuperVGAModeInformation;Offset 0x58ef      ;1
                        DW offset SetSuperVGAVideoMode;Offset 0x5b0e            ;2
                        DW offset SuperVGAGetCurrentVideoMode;Offset 0x5ca3     ;3
                        DW offset SaveRestoreSuperVGAVideoState;Offset 0x5ccb   ;4
                        DW offset CPUVideoMemoryControl;Offset 0x5d4d           ;5
                        DW offset LogicalScanLineLengthHandling;Offset 0x5d55   ;6
                        DW offset DisplayStartHandling;Offset 0x5dcd            ;7
                        DW offset GetSetDACPaletteControl;Offset 0x5e63         ;8
                        DW offset GetSetPaletteEntries;Offset 0x5eaf            ;9
                        DW offset GetProtectedModeInterface;Offset 0x5f21       ;a
                        DW offset GetNearestPixelClock;Offset 0x5f41            ;b

;Offset 0x576b
VESAResponseTemplate    DB 'VESA'       ;00h  4 BYTEs   (ret) signature ("VESA")
                        DW 0300h        ;04h    WORD    VESA version number (one-digit minor version -- 0300h = v3.0)
                        DD 00000000h    ;06h    DWORD   pointer to OEM name
                        DD 00000001h    ;0Ah    DWORD   capabilities flags
                        DD 00000000h    ;0Eh    DWORD   pointer to list of supported VESA and OEM video modes
                        DW 0000h        ;12h    WORD    total amount of video memory in 64K blocks
                        DW 0000h        ;14h    WORD    OEM software version (BCD, high byte = major, low byte = minor)
                        DD 00000000h    ;16h    DWORD   pointer to vendor name
                        DD 00000000h    ;1Ah    DWORD   pointer to product name
                        DD 00000000h    ;1Eh    DWORD   pointer to product revision string

;Offset 0x578d
Data0x578d              DB 01Bh, 000h, 007h, 000h, 040h, 000h, 040h, 000h
                        DB 000h, 0A0h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x579d
Data0x579d              DB 00Fh, 000h, 007h, 000h, 020h, 000h, 020h, 000h
                        DB 000h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x57ad
OEMName                 DB 'Matrox Graphics Inc.', 000h

;Offset 0x57c2
VendorName              DB 'Matrox', 000h

;Offset 0x57c9
ProductName             DB 'Matrox G400', 000h

;Offset 0x57d5
RevisionString          DB '00', 000h

;Offset 0x57d8
                        DB 'VBE/MGA'
                        DB 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh
                        DW 00201h       ;BIOS Version (2.1)

VESAHandler:                            ;Offset 0x57eb
    cmp       al, 14h                   ;Matrox OEM extension (bl=00h = installation check / get caps) https://www.ctyme.com/intr/rb-0306.htm
    je        Label0x57ff               ;Offset 0x57ff
    cmp       al, 0bh
    ja        Label0x5810               ;Offset 0x5810
    sti
    cld
    xor       ah, ah
    shl       ax, 01h
    xchg      ax, bx
    jmp       word ptr cs:[bx + VESACallTable];Offset 0x5753
Label0x57ff:                            ;Offset 0x57ff
    cmp       bl, 03h
    je        Label0x5f7e               ;Offset 0x5f7e
    cmp       bl, 05h
    je        Label0x5f8d               ;Offset 0x5f8d
    jmp       Label0x5810               ;Offset 0x5810
    nop
Label0x5810:                            ;Offset 0x5810
    mov       ax, 0ffffh
    iret

Func0x5814 PROC NEAR                    ;Offset 0x5814
    push      cx
    mov       cx, bx
    and       cx, 01ffh
    cmp       cx, 006ah
    je        Label0x5826               ;Offset 0x5826
    cmp       cx, 0102h
    jne       Label0x582e               ;Offset 0x582e
Label0x5826:                            ;Offset 0x5826
    and       bx, 0f600h
    or        bx, 0102h
Label0x582e:                            ;Offset 0x582e
    pop       cx
    ret
Func0x5814 ENDP

Func0x5830 PROC NEAR                    ;Offset 0x5830
    push      ax
    push      bx
    push      si
    push      ds
    push      cs
    pop       ds
    and       bx, 01ffh
    lea       si, [SupportedVESAVideoModes];Offset 0x5477
Label0x583e:                            ;Offset 0x583e
    lodsw
    cmp       bx, ax
    je        Label0x584a               ;Offset 0x584a
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    jne       Label0x583e               ;Offset 0x583e
    or        ax, ax
Label0x584a:                            ;Offset 0x584a
    pop       ds
    pop       si
    pop       bx
    pop       ax
    ret
Func0x5830 ENDP

GetSuperVGAInformation:                 ;Offset 0x584f
    mov       bx, ax
    push      ds
    push      di
    push      si
    push      cx
    push      dx
    push      cs
    pop       ds
    mov       dx, 0000h
    ;cmp       dword ptr es:[di], 32454256h ;VBE2
    DB 026h, 066h, 081h, 03Dh, 056h, 042h, 045h, 032h
    jne       NoVBE2Requested           ;Offset 0x5868
    mov       dx, 0001h
NoVBE2Requested:                        ;Offset 0x5868
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       cx, 0022h
    mov       si, offset VESAResponseTemplate;Offset 0x576b
    rep movsb
    pop       di
    call      GetMemoryIn64KBlocks      ;Offset 0x673e
    mov       word ptr es:[di + VESA_INFO_MemoryIn64KBBlocks], ax;0x12
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerSeg], cs;0x10
    mov       ax, offset SupportedVESAVideoModes;Offset 0x5477
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerOfs], ax;0xe
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerSeg], cs;0x8
    mov       ax, offset OEMName        ;Offset 0x57ad
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerOfs], ax;0x6
    cmp       dx, 0001h
    jne       DontFillOutVBE2Info       ;Offset 0x58e6
    mov       ax, 0021h                 ;OEM Version 2.1
    shl       ax, 04h
    mov       al, 0021h                 ;OEM Version 2.1
    and       al, 0fh
    mov       word ptr es:[di + VESA_INFO_OEMSoftwareVersion], ax;0x14
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerSeg], ax;0x18
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerSeg], ax;0x1c
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerSeg], ax;0x20
    mov       ax, offset VendorName     ;Offset 0x57c2
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerOfs], ax;0x16
    mov       ax, offset ProductName    ;Offset 0x57c9
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerOfs], ax;0x1a
    mov       ax, offset RevisionString ;Offset 0x57d5
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerOfs], ax;0x1e
    add       di, 0100h
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    mov       cx, 0016h
    lea       si, [VendorName]          ;Offset 0x57c2
    rep movsb
DontFillOutVBE2Info:                    ;Offset 0x58e6
    mov       ax, 004fh
    pop       dx
    pop       cx
    pop       si
    pop       di
    pop       ds
    iret

GetSuperVGAModeInformation:             ;Offset 0x58ef
    push      ax
    push      ds
    push      di
    push      si
    push      ebx
    push      ecx
    push      edx
    mov       bx, cx
    mov       dx, 0000h
    call      Func0x5814                ;Offset 0x5814
    cmp       bl, 0ffh
    jne       Label0x590b               ;Offset 0x590b
    mov       bx, 0116h
    jmp       Label0x5921               ;Offset 0x5921
Label0x590b:                            ;Offset 0x590b
    mov       ah, 01h
    call      Func0x5830                ;Offset 0x5830
    je        Label0x5921               ;Offset 0x5921
    mov       ax, 0af01h
    int       6dh
    cmp       ax, 0af01h
    je        Label0x5b01               ;Offset 0x5b01
    mov       dx, 0001h
Label0x5921:                            ;Offset 0x5921
    push      eax
    push      cx
    cld
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      ds
    push      si
    push      cs
    pop       ds
    mov       cx, 0010h
    lea       si, [Data0x578d]          ;Offset 0x578d
    cmp       bl, 08h
    jb        Label0x5947               ;Offset 0x5947
    cmp       bl, 0ch
    ja        Label0x5947               ;Offset 0x5947
    lea       si, [Data0x579d]          ;Offset 0x579d
Label0x5947:                            ;Offset 0x5947
    rep movsb
    pop       si
    pop       ds
    or        dx, dx
    jne       Label0x595d               ;Offset 0x595d
    push      cs
    pop       ds
    lea       si, [Data0x549f]          ;Offset 0x549f
    and       bx, 001fh
    shl       bx, 02h
    add       si, bx
Label0x595d:                            ;Offset 0x595d
    mov       bx, word ptr [si + 02h]
    mov       si, word ptr [si]
    mov       ax, cs
    mov       word ptr es:[di - 02h], ax
    lea       ax, [Func0x5d1d]          ;Offset 0x5d1d
    mov       word ptr es:[di - 04h], ax
    push      ds
    push      si
    push      di
    push      cs
    pop       ds
    lea       si, [Data0x5513]          ;Offset 0x5513
    lea       di, [di + 06h]
    mov       cx, 0007h
    rep movsb
    pop       di
    pop       si
    pop       ds
    ;mov       dx, word ptr [si + 06h]
    DB 08Bh, 094h, 006h, 000h
    mov       word ptr es:[di + 04h], dx
    ;mov       dx, word ptr [si + 04h]
    DB 08Bh, 094h, 004h, 000h
    mov       word ptr es:[di + 02h], dx
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x59b2               ;Offset 0x59b2
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x59b2               ;Offset 0x59b2
    mov       dx, 0500h
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x59b2               ;Offset 0x59b2
    mov       dx, 0400h
Label0x59b2:                            ;Offset 0x59b2
    mov       byte ptr es:[di + 0eh], 01h
    mov       ax, bx
    ;and       ax, 30h
    DB 025h, 030h, 000h
    je        Label0x5a1b               ;Offset 0x5a1b
    shr       ax, 04h
    dec       ax
    mov       cx, ax
    shl       dx, cl
    mov       word ptr es:[di], dx
    or        word ptr es:[di - 10h], 80h
    mov       word ptr es:[di + 22h], dx
    mov       dx, 0008h
    shl       dx, cl
    mov       byte ptr es:[di + 09h], dl
    cmp       bx, 10h
    je        Label0x5a57               ;Offset 0x5a57
    mov       byte ptr es:[di + 0bh], 06h
    push      cs
    pop       ds
    lea       si, [Data0x551a]          ;Offset 0x551a
    cmp       bx, 0020h
    je        Label0x59fe               ;Offset 0x59fe
    lea       si, [Data0x5522]          ;Offset 0x5522
    cmp       bx, 0060h
    je        Label0x59fe               ;Offset 0x59fe
    lea       si, [Data0x552a]          ;Offset 0x552a
Label0x59fe:                            ;Offset 0x59fe
    push      si
    push      di
    lea       di, [di + 0fh]
    mov       cx, 0008h
    rep movsb
    pop       di
    pop       si
    mov       byte ptr es:[di + 17h], 01h
    push      di
    lea       di, [di + 26h]
    mov       cx, 0008h
    rep movsb
    pop       di
    jmp       Label0x5a57               ;Offset 0x5a57
Label0x5a1b:                            ;Offset 0x5a1b
    shr       dx, 03h
    mov       word ptr es:[di], dx
    mov       byte ptr es:[di + 08h], 04h
    mov       byte ptr es:[di + 09h], 04h
    test      bx, 40h
    je        Label0x5a4d               ;Offset 0x5a4d
    shr       word ptr es:[di + 02h], 03h
    shr       word ptr es:[di + 04h], 03h
    test      bx, 80h
    je        Label0x5a46               ;Offset 0x5a46
    mov       byte ptr es:[di + 07h], 08h
Label0x5a46:                            ;Offset 0x5a46
    mov       byte ptr es:[di + 0bh], 00h
    jmp       Label0x5a57               ;Offset 0x5a57
Label0x5a4d:                            ;Offset 0x5a4d
    mov       byte ptr es:[di + 07h], 0eh
    mov       byte ptr es:[di + 0bh], 03h
Label0x5a57:                            ;Offset 0x5a57
    mov       si, bx
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call      ReadPCIBase0AndBase1      ;Offset 0x6a6f
    pop       dx
    push      es
    push      0000h
    pop       es
    push      word ptr es:[BDA_DisplayMode];Offset 0x449
    add       dl, 20h
    mov       byte ptr es:[BDA_DisplayMode], dl;Offset 0x449
    call      Func0x694c                ;Offset 0x694c
    pop       word ptr es:[BDA_DisplayMode];Offset 0x449
    pop       es
    mov       eax, ebx
    and       eax, 000000ffh
    mov       edx, 00010000h
    mul       edx
    add       ecx, eax
    test      si, 0030h
    je        Label0x5a9c               ;Offset 0x5a9c
    ;mov       dword ptr es:[di + 18h], ecx
    DB 026h, 066h, 089h, 04Dh, 018h
Label0x5a9c:                            ;Offset 0x5a9c
    mov       ecx, eax
    test      si, 0030h
    je        Label0x5af4               ;Offset 0x5af4
    mov       bx, word ptr es:[di]
    mov       ax, word ptr es:[di + 04h]
    or        bx, bx
    je        Label0x5af4               ;Offset 0x5af4
    xor       dx, dx
    mul       bx
    mov       bx, 0ffffh
    div       bx
    or        dx, dx
    je        Label0x5abe               ;Offset 0x5abe
    inc       ax
Label0x5abe:                            ;Offset 0x5abe
    xor       dx, dx
    mul       bx
    shl       edx, 10h
    mov       dx, ax
    mov       ebx, edx
    xor       edx, edx
    call      GetMemoryIn64KBlocks      ;Offset 0x673e
    cmp       ax, 0100h
    jbe       Label0x5ad9               ;Offset 0x5ad9
    mov       ax, 0100h
Label0x5ad9:                            ;Offset 0x5ad9
    shl       eax, 10h
    shl       ecx, 01h
    sub       eax, ecx
    div       ebx
    dec       al
    mov       byte ptr es:[di + 0dh], al
    mov       byte ptr es:[di + 24h], al
    mov       byte ptr es:[di + 25h], al
Label0x5af4:                            ;Offset 0x5af4
    ;mov       dword ptr es:[di + 2eh], 0cfe6a80h
    DB 026h, 066h, 0C7h, 045h, 02Eh, 080h, 06Ah, 0FEh, 00Ch
    pop       eax
    mov       ah, 00h
Label0x5b01:                            ;Offset 0x5b01
    mov       al, 4fh
    pop       edx
    pop       ecx
    pop       ebx
    pop       si
    pop       di
    pop       ds
    pop       bx
    iret

;
;inputs:
;   ax = requested mode
;
;
;
SetSuperVGAVideoMode:                   ;Offset 0x5b0e
    push      ax
    push      ds
    push      di
    push      si
    push      cx
    push      cs
    pop       ds
    mov       bx, ax
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x66f6
    je        Exit                      ;Offset 0x5c9d
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x5b2a               ;Offset 0x5b2a
    and       bx, 0f7ffh
Label0x5b2a:                            ;Offset 0x5b2a
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x5b35               ;Offset 0x5b35
    and       bh, 7fh
Label0x5b35:                            ;Offset 0x5b35
    push      bx
    call      Func0x5814                ;Offset 0x5814
    cmp       bl, 0ffh
    jne       Label0x5b41               ;Offset 0x5b41
    mov       bx, 0100h
Label0x5b41:                            ;Offset 0x5b41
    mov       ax, bx
    push      ax
    mov       ax, 0af02h
    int       6dh
    cmp       ax, 0af02h
    pop       ax
    mov       cx, si
    jne       Label0x5b5b               ;Offset 0x5b5b
    mov       cx, 0000h
    call      Func0x5830                ;Offset 0x5830
    jne       Label0x5c86               ;Offset 0x5c86
Label0x5b5b:                            ;Offset 0x5b5b
    lea       si, [Data0x549f]          ;Offset 0x549f
    and       bx, 001fh
    shl       bx, 02h
    add       si, bx
    push      es
    push      0000h
    pop       es
    mov       bx, word ptr cs:[si + 02h]
    and       byte ptr es:[BDA_EGAFeatureBitSwitches], 0fh;Offset 0x488
    or        byte ptr es:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    pop       es
    or        cx, cx
    jne       Label0x5b82               ;Offset 0x5b82
    mov       cx, word ptr cs:[si]
Label0x5b82:                            ;Offset 0x5b82
    push      bp
    mov       bp, sp
    test      ax, 0800h
    je        Label0x5c6b               ;Offset 0x5c6b
    ;sub       sp, 0066h
    DB 081h, 0ECh, 066h, 000h
    cld
    push      es
    push      ax
    push      ebx
    push      edx
    mov       si, cx
    push      es
    push      di
    xor       edx, edx
    ;mov       eax, dword ptr es:[di + 0dh]
    DB 026h, 066h, 08Bh, 045h, 00Dh
    mov       ebx, 000003e8h
    div       ebx
    mov       ebx, eax
    ;mov       dx, word ptr [si + 06h]
    DB 08Bh, 094h, 006h, 000h
    shl       edx, 10h
    ;mov       dx, word ptr [si + 04h]
    DB 08Bh, 094h, 004h, 000h
    push      ss
    pop       es
    ;lea       di, [bp - 66h]
    DB 08Dh, 0BEh, 09Ah, 0FFh
    mov       si, cx
    mov       cx, 000eh
    nop
    push      si
    rep movsb
    pop       si
    ;mov       si, word ptr [si + 0ah]
    DB 08Bh, 0B4h, 00Ah, 000h
    mov       cx, 0010h
    rep movsd
    pop       si
    pop       ds
    mov       ax, dx
    shr       ax, 03h
    mov       word ptr es:[di], ax
    mov       word ptr es:[di + 02h], ax
    mov       ax, word ptr [si + 02h]
    shr       ax, 03h
    mov       word ptr es:[di + 04h], ax
    mov       ax, word ptr [si + 04h]
    shr       ax, 03h
    mov       word ptr es:[di + 06h], ax
    mov       ax, word ptr [si]
    shr       ax, 03h
    mov       word ptr es:[di + 08h], ax
    mov       word ptr es:[di + 0ah], ax
    shr       edx, 10h
    mov       ax, dx
    mov       word ptr es:[di + 0ch], ax
    mov       word ptr es:[di + 0eh], ax
    mov       ax, word ptr [si + 08h]
    mov       word ptr es:[di + 10h], ax
    mov       ax, word ptr [si + 0ah]
    mov       word ptr es:[di + 12h], ax
    mov       ax, word ptr [si + 06h]
    mov       word ptr es:[di + 14h], ax
    mov       word ptr es:[di + 16h], ax
    mov       dl, byte ptr [si + 0ch]
    mov       ax, ss
    mov       ds, ax
    mov       es, ax
    mov       si, di
    ;lea       di, [bp - 66h]
    DB 08Dh, 0BEh, 09Ah, 0FFh
    ;mov       dword ptr [di], ebx
    DB 066h, 089h, 09Dh, 000h, 000h,
    ;add       di, 000eh
    DB 081h, 0C7h, 00Eh, 000h
    ;mov       word ptr [bp - 5ch], di
    DB 089h, 0BEh, 0A4h, 0FFh
    mov       ax, 0001h
    call      Func0x6793                ;Offset 0x6793
    ;mov       word ptr [bp - 5ah], ax
    DB 089h, 086h, 0A6h, 0FFh
    mov       ax, 002fh
    and       dx, 000ch
    shl       dx, 04h
    or        al, dl
    ;mov       byte ptr es:[di + 09h], al
    DB 026h, 088h, 085h, 009h, 000h
    ;lea       cx, [bp - 66h]
    DB 08Dh, 08Eh, 09Ah, 0FFh
    pop       edx
    pop       ebx
    pop       ax
    pop       es
Label0x5c6b:                            ;Offset 0x5c6b
    push      ds
    push      cx
    call      Func0x6380                ;Offset 0x6380
    pop       ax
    pop       ax
    mov       sp, bp
    pop       bp
    pop       bx
    cmp       bl, 0ffh
    jne       Label0x5c9a               ;Offset 0x5c9a
    push      dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out       dx, ax
    pop       dx
    jmp       Label0x5c9a               ;Offset 0x5c9a
Label0x5c86:                            ;Offset 0x5c86
    pop       bx
    mov       ax, bx
    and       al, 7fh
    ;cmp       ax, 0013h
    DB 03Dh, 013h, 000h
    jle       Label0x5c95               ;Offset 0x5c95
    mov       ax, 014fh
    jmp       Exit                      ;Offset 0x5c9d
Label0x5c95:                            ;Offset 0x5c95
    movzx     ax, bl
    int       10h
Label0x5c9a:                            ;Offset 0x5c9a
    mov       ax, 004fh
Exit:                                   ;Offset 0x5c9d
    pop       cx
    pop       si
    pop       di
    pop       ds
    pop       bx
    iret      

SuperVGAGetCurrentVideoMode:            ;Offset 0x5ca3
    mov       ax, 0f00h
    int       10h
    xor       bx, bx
    mov       ah, al
    and       ah, 0c0h
    and       al, 3fh
    cmp       al, 20h
    jl        Label0x5cc5               ;Offset 0x5cc5
    cmp       al, 40h
    jge       Label0x5cc5               ;Offset 0x5cc5
    sub       al, 20h
    inc       bh
    or        bh, ah
    cmp       al, 16h
    jne       Label0x5cc5               ;Offset 0x5cc5
    mov       al, 0ffh
Label0x5cc5:                            ;Offset 0x5cc5
    mov       bl, al
    mov       ax, 004fh
    iret

SaveRestoreSuperVGAVideoState:          ;Offset 0x5ccb
    mov       bx, ax
    mov       ah, 1ch
    mov       al, dl
    push      ax
    push      cx
    test      cx, 0008h
    je        Label0x5d04               ;Offset 0x5d04
    cmp       al, 00h
    jne       Label0x5cea               ;Offset 0x5cea
    cmp       cx, 0008h
    jne       Label0x5d04               ;Offset 0x5d04
    mov       bx, 0001h
    nop
    pop       cx
    pop       ax
    jmp       Label0x5d16               ;Offset 0x5d16
Label0x5cea:                            ;Offset 0x5cea
    cmp       al, 01h
    jne       Label0x5cf7               ;Offset 0x5cf7
    push      bx
    call      SaveMGAState              ;Offset 0x5fce
    int       10h
    pop       bx
    jmp       Label0x5d06               ;Offset 0x5d06
Label0x5cf7:                            ;Offset 0x5cf7
    cmp       al, 02h
    jne       Label0x5d04               ;Offset 0x5d04
    push      bx
    call      RestoreMGAState           ;Offset 0x603b
    int       10h
    pop       bx
    jmp       Label0x5d06               ;Offset 0x5d06
Label0x5d04:                            ;Offset 0x5d04
    int       10h
Label0x5d06:                            ;Offset 0x5d06
    pop       cx
    pop       ax
    test      cx, 0008h
    je        Label0x5d16               ;Offset 0x5d16
    cmp       al, 00h
    jne       Label0x5d16               ;Offset 0x5d16
    ;add       bx, 0001h
    DB 081h, 0C3h, 001h, 000h
Label0x5d16:                            ;Offset 0x5d16
    call      Func0x6b4d                ;Offset 0x6b4d
    mov       ax, 004fh
    iret

Func0x5d1d PROC FAR                     ;Offset 0x5d1d
    call      Func0x5d21                ;Offset 0x5d21
    retf
Func0x5d1d ENDP

Func0x5d21 PROC NEAR                    ;Offset 0x5d21
    push      ebx
    mov       ah, 02h
    or        bl, bl
    jne       Label0x5d4a               ;Offset 0x5d4a
    mov       ah, bh
    call      Func0x694c                ;Offset 0x694c
    or        ah, ah
    jne       Label0x5d3e               ;Offset 0x5d3e
    mov       ah, MGA_CRTCExt_MemoryPage;0x4
    mov       al, dl
    add       al, bl
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    jmp       Label0x5d48               ;Offset 0x5d48
    nop
Label0x5d3e:                            ;Offset 0x5d3e
    mov       al, MGA_CRTCExt_MemoryPage;0x4
    call      MGAReadCRTCExtensionRegister;Offset 0x312
    sub       al, bl
    movzx     dx, al
Label0x5d48:                            ;Offset 0x5d48
    mov       ah, 00h
Label0x5d4a:                            ;Offset 0x5d4a
    pop       ebx
    ret
Func0x5d21 ENDP

CPUVideoMemoryControl:                  ;Offset 0x5d4d
    mov       bx, ax
    call      Func0x5d21                ;Offset 0x5d21
    mov       al, 4fh
    iret

LogicalScanLineLengthHandling:          ;Offset 0x5d55
    mov       bx, ax
    call      Func0x64b6                ;Offset 0x64b6
    je        Label0x5da8               ;Offset 0x5da8
    push      ds
    push      si
    cmp       bl, 01h
    je        Label0x5d9f               ;Offset 0x5d9f
    call      Func0x6759                ;Offset 0x6759
    cmp       bl, 03h
    jne       Label0x5d72               ;Offset 0x5d72
    call      Func0x66a0                ;Offset 0x66a0
    mov       cx, ax
    jmp       Label0x5da2               ;Offset 0x5da2
Label0x5d72:                            ;Offset 0x5d72
    cmp       bl, 02h
    jne       Label0x5d7f               ;Offset 0x5d7f
Label0x5d77:                            ;Offset 0x5d77
    dec       al
    je        Label0x5d7f               ;Offset 0x5d7f
    shr       cx, 01h
    jmp       Label0x5d77               ;Offset 0x5d77
Label0x5d7f:                            ;Offset 0x5d7f
    ;mov       ax, word ptr [si + 04h]
    DB 08Bh, 084h, 004h, 000h
    cmp       ax, cx
    jb        Label0x5d89               ;Offset 0x5d89
    mov       cx, ax
Label0x5d89:                            ;Offset 0x5d89
    mov       ax, cx
    call      Func0x6680                ;Offset 0x6680
    mov       cx, ax
    call      Func0x66a0                ;Offset 0x66a0
    cmp       ax, cx
    mov       ax, 024fh
    jb        Label0x5da5               ;Offset 0x5da5
    mov       ax, cx
    call      Func0x664b                ;Offset 0x664b
Label0x5d9f:                            ;Offset 0x5d9f
    call      Func0x6605                ;Offset 0x6605
Label0x5da2:                            ;Offset 0x5da2
    mov       ax, 004fh
Label0x5da5:                            ;Offset 0x5da5
    pop       si
    pop       ds
    iret
Label0x5da8:                            ;Offset 0x5da8
    cmp       bl, 03h
    jne       Label0x5db2               ;Offset 0x5db2
    mov       cx, 0320h
    jmp       Label0x5dc9               ;Offset 0x5dc9
Label0x5db2:                            ;Offset 0x5db2
    mov       dx, VGA_CRTControllerIndexD;Port 0x3d4
    nop   
    mov       al, VGA_CRTCIdx_HorDispEnableEnd;0x1
    out       dx, al
    inc       dx
    in        al, dx
    inc       al
    xor       ah, ah
    mov       cx, ax
    shl       cx, 03h
    mov       bx, ax
    mov       dx, 027eh
Label0x5dc9:                            ;Offset 0x5dc9
    mov       ax, 004fh
    iret

DisplayStartHandling:                   ;Offset 0x5dcd
    mov       bx, ax
    call      Func0x64b6                ;Offset 0x64b6
    je        Label0x5df2               ;Offset 0x5df2
    cmp       bl, 01h
    je        Label0x5deb               ;Offset 0x5deb
    or        bl, bl
    je        Label0x5de4               ;Offset 0x5de4
    cmp       bl, 80h
    ;jne       Label0x5e5d               ;Offset 0x5e5d
    DB 00Fh, 085h, 079h, 000h
Label0x5de4:                            ;Offset 0x5de4
    call      Func0x65b2                ;Offset 0x65b2
    mov       ax, 004fh
    iret
Label0x5deb:                            ;Offset 0x5deb
    call      Func0x6570                ;Offset 0x6570
    mov       ax, 004fh
    iret
Label0x5df2:                            ;Offset 0x5df2
    push      di
    push      ds
    push      0000h
    pop       ds
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop       ds
    mov       di, 0064h
    cmp       al, 22h
    je        Label0x5e0c               ;Offset 0x5e0c
    mov       di, 0050h
    cmp       al, 28h
    je        Label0x5e0c               ;Offset 0x5e0c
    mov       di, 0084h
Label0x5e0c:                            ;Offset 0x5e0c
    and       bl, 01h
    jne       Label0x5e38               ;Offset 0x5e38
    or        bl, bl
    je        Label0x5e1a               ;Offset 0x5e1a
    cmp       bl, 80h
    jne       Label0x5e5d               ;Offset 0x5e5d
Label0x5e1a:                            ;Offset 0x5e1a
    push      cx
    push      dx
    xor       al, al
    call      Func0x64d0                ;Offset 0x64d0
    mov       ax, di
    mul       dx
    push      dx
    push      ax
    pop       eax
    shr       cx, 03h
    add       ax, cx
    call      Func0x6526                ;Offset 0x6526
    pop       dx
    pop       cx
    pop       di
    mov       ax, 004fh
    iret
Label0x5e38:                            ;Offset 0x5e38
    call      Func0x64f3                ;Offset 0x64f3
    xor       cx, cx
    xor       dx, dx
    or        eax, eax
    je        Label0x5e53               ;Offset 0x5e53
    push      eax
    pop       ax
    pop       dx
    mov       bx, di
    div       bx
    mov       cx, dx
    shl       cx, 03h
    mov       dx, ax
Label0x5e53:                            ;Offset 0x5e53
    mov       al, 01h
    call      Func0x64d0                ;Offset 0x64d0
    pop       di
    mov       ax, 004fh
    iret
Label0x5e5d:                            ;Offset 0x5e5d
    pop       di
    mov       ah, 01h
    mov       al, 004fh
    iret

GetSetDACPaletteControl:                ;Offset 0x5e63
    push      cx
    push      dx
    mov       dx, ax
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, 1eh
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    mov       ah, 01h
    or        dl, dl
    je        Label0x5e8b               ;Offset 0x5e8b
    cmp       dl, 01h
    jne       Label0x5ea5               ;Offset 0x5ea5
    xor       ah, ah
    mov       dh, 06h
    and       cl, MGA_MISCCTRL_DAC_8Bit ;08h
    je        Label0x5ea5               ;Offset 0x5ea5
    mov       dh, 08h
    jmp       Label0x5ea5               ;Offset 0x5ea5
Label0x5e8b:                            ;Offset 0x5e8b
    and       cl, NOT MGA_MISCCTRL_DAC_8Bit;0xf7
    cmp       dh, 08h
    mov       dh, 06h
    jb        Label0x5e9a               ;Offset 0x5e9a
    mov       dh, 08h
    or        cl, MGA_MISCCTRL_DAC_8Bit ;0x8
Label0x5e9a:                            ;Offset 0x5e9a
    xchg      cl, ch
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6b6e
    xor       ah, ah
Label0x5ea5:                            ;Offset 0x5ea5
    call      Func0x6b4d                ;Offset 0x6b4d
    mov       bx, dx
    pop       dx
    pop       cx
    mov       al, 004fh
    iret

GetSetPaletteEntries:                   ;Offset 0x5eaf
    mov       bx, ax
    cmp       bl, 02h
    je        Label0x5f1d               ;Offset 0x5f1d
    cmp       bl, 03h
    je        Label0x5f1d               ;Offset 0x5f1d
    push      bx
    push      cx
    push      dx
    push      di
    mov       al, dl
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    push      cx
    mov       al, bl
    cmp       al, 80h
    jne       Label0x5ee0               ;Offset 0x5ee0
    mov       dl, 0dah
    xor       cx, cx
Label0x5ed0:                            ;Offset 0x5ed0
    dec       cx
    je        Label0x5ee0               ;Offset 0x5ee0
    in        al, dx
    and       al, 08h
    jne       Label0x5ed0               ;Offset 0x5ed0
Label0x5ed8:                            ;Offset 0x5ed8
    dec       cx
    je        Label0x5ee0               ;Offset 0x5ee0
    in        al, dx
    and       al, 08h
    je        Label0x5ed8               ;Offset 0x5ed8
Label0x5ee0:                            ;Offset 0x5ee0
    pop       cx
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    mov       al, bl
    and       al, 01h
    je        Label0x5f00               ;Offset 0x5f00
Label0x5ee9:                            ;Offset 0x5ee9
    in        al, dx
    mov       byte ptr es:[di + 02h], al
    in        al, dx
    mov       byte ptr es:[di + 01h], al
    in        al, dx
    mov       byte ptr es:[di], al
    inc       di
    inc       di
    inc       di
    inc       di
    dec       cx
    jne       Label0x5ee9               ;Offset 0x5ee9
    jmp       Label0x5f15               ;Offset 0x5f15
Label0x5f00:                            ;Offset 0x5f00
    mov       al, byte ptr es:[di + 02h]
    out       dx, al
    mov       al, byte ptr es:[di + 01h]
    out       dx, al
    mov       al, byte ptr es:[di]
    out       dx, al
    inc       di
    inc       di
    inc       di
    inc       di
    dec       cx
    jne       Label0x5f00               ;Offset 0x5f00
Label0x5f15:                            ;Offset 0x5f15
    pop       di
    pop       dx
    pop       cx
    pop       bx
    mov       ax, 004fh
    iret
Label0x5f1d:                            ;Offset 0x5f1d
    mov       ax, 024fh
    iret

GetProtectedModeInterface:              ;Offset 0x5f21
    mov       bx, ax
    mov       ah, 01h
    or        bl, bl
    jne       Label0x5f3e               ;Offset 0x5f3e
    mov       ax, cs
    mov       es, ax
    mov       di, offset PINS           ;Offset 0x7ce0
    xor       ax, ax
    mov       al, byte ptr es:[di + 02h]
    add       di, ax
    mov       cx, word ptr es:[di + 08h]
    xor       ah, ah
Label0x5f3e:                            ;Offset 0x5f3e
    mov       al, 004fh
    iret

GetNearestPixelClock:                   ;Offset 0x5f41
    push      ax
    push      si
    push      edx
    push      eax
    push      ebx
    mov       si, ax
    xor       edx, edx
    mov       eax, ecx
    mov       ecx, 000003e8h
    div       ecx
    mov       bx, 0002h
    nop
    call      Func0x6bf0                ;Offset 0x6bf0
    mul       ecx
    mov       ecx, eax
    and       si, 00ffh
    je        Label0x5f72               ;Offset 0x5f72
    mov       cl, 06h
    call      Func0x6e6b                ;Offset 0x6e6b
Label0x5f72:                            ;Offset 0x5f72
    pop       ebx
    pop       eax
    mov       ax, 004fh
    pop       edx
    pop       si
    pop       bx
    iret

Label0x5f7e:                            ;Offset 0x5f7e
    push      si
    mov       bx, ax
    add       si, offset PINS           ;Offset 0x7ce0
    mov       cl, byte ptr cs:[si]
    mov       ax, 004fh
    pop       si
    iret

Label0x5f8d:                            ;Offset 0x5f8d
    mov       bx, ax
    mov       cl, byte ptr cs:[si]
    mov       ax, 004fh
    iret

;Offset 0x5f96
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
                        DB MGA_INDD_PanelMode                       ;0x1F
                        DB MGA_INDD_MAFCDelay                       ;0x20
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
                        DB 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x5fce
SaveMGAState PROC NEAR                  ;Offset 0x5fce
    push ds
    push si
    push di
    push dx
    push cx
    push ax
    push cs
    pop  ds
    mov  di, bx
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x5fdf
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5f96
    cmp  cl, 0ffh
    je   EndOfIndexedRegisterData       ;Offset 0x5ff3
    call ReadIndexedRegister            ;Offset 0x6b83
    mov  byte ptr es:[di], cl
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x5fdf
EndOfIndexedRegisterData:               ;Offset 0x5ff3
    mov  byte ptr es:[di], 0ffh
    inc  di
    mov  si, di
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    nop
    mov  di, PCI_MGA_Option             ;0x40
    nop
    call AccessPCIRegister              ;Offset 0x69da
    ;mov  dword ptr es:[si], ecx
    DB 026h, 066h, 089h, 00Ch
    add  si, 0004h
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    nop
    mov  di, PCI_MGA_OPTION3            ;0x54
    nop
    call AccessPCIRegister              ;Offset 0x69da
    ;mov  dword ptr es:[si], ecx
    DB 026h, 066h, 089h, 00Ch
    add  si, 0004h
    mov  di, si
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x6023
    mov  al, cl
    out  dx, al
    in   ax, dx
    mov  byte ptr es:[di], ah
    inc  di
    inc  cl
    cmp  cl, 08h
    jbe  LoopExtensionRegisters         ;Offset 0x6023
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  di
    pop  si
    pop  ds
    ret
SaveMGAState ENDP

RestoreMGAState PROC NEAR               ;Offset 0x603b
    push ds
    push di
    push si
    push dx
    push cx
    push ax
    push cs
    pop  ds
    mov  di, bx
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x604c
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5f96
    cmp  cl, 0ffh
    je   EndOfIndexedRegisterData       ;Offset 0x6060
    mov  ch, byte ptr es:[di]
    call WriteIndexedRegister           ;Offset 0x6b6e
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x604c
EndOfIndexedRegisterData:               ;Offset 0x6060
    inc  di
    mov  si, di
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    nop
    ;mov  ecx, dword ptr es:[si]
    DB 026h, 066h, 08Bh, 00Ch
    mov  di, PCI_MGA_Option             ;0x40
    nop
    call AccessPCIRegister              ;Offset 0x69da
    add  si, 0004h
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    nop
    ;mov  ecx, dword ptr es:[si]
    DB 026h, 066h, 08Bh, 00Ch
    mov  di, PCI_MGA_OPTION3            ;0x54
    nop  
    call AccessPCIRegister              ;Offset 0x69da
    add  si, 0004h
    mov  di, si
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x608c
    mov  al, cl
    mov  ah, byte ptr es:[di]
    out  dx, ax
    inc  di
    inc  cl
    cmp  cl, 08h
    jbe  LoopExtensionRegisters         ;Offset 0x608c
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  si
    pop  di
    pop  ds
    ret
RestoreMGAState ENDP

Func0x60a3 PROC NEAR                    ;Offset 0x60a3
    cmp  al, 20h
    jb   Label0x60bb                    ;Offset 0x60bb
    cmp  al, 22h
    je   Label0x60b5                    ;Offset 0x60b5
    cmp  al, 28h
    jb   Label0x60b9                    ;Offset 0x60b9
    cmp  al, 2dh
    mov  al, 03h
    jb   Label0x60bb                    ;Offset 0x60bb
Label0x60b5:                            ;Offset 0x60b5
    mov  al, 12h
    jmp  Label0x60bb                    ;Offset 0x60bb
Label0x60b9:                            ;Offset 0x60b9
    mov  al, 13h
Label0x60bb:                            ;Offset 0x60bb
    ret  
Func0x60a3 ENDP

Func0x60bc PROC NEAR                    ;Offset 0x60bc
    push dx
    push cx
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, MGA_MULCTRL_DepthMask      ;0x7
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    xor  al, al
    out  dx, al
    inc  dx
    cmp  cl, MGA_MULCTRL_Depth32Pal     ;0x7
    je   Label0x60db                    ;Offset 0x60db
    cmp  cl, MGA_MULCTRL_Depth16        ;0x2
    je   Label0x60e4                    ;Offset 0x60e4
    jmp  Label0x60fe                    ;Offset 0x60fe
Label0x60db:                            ;Offset 0x60db
    out  dx, al
    out  dx, al
    out  dx, al
    inc  al
    jne  Label0x60db                    ;Offset 0x60db
    jmp  Label0x610c                    ;Offset 0x610c
Label0x60e4:                            ;Offset 0x60e4
    mov  cl, al
    shl  al, 03h
    out  dx, al
    mov  al, cl
    shl  al, 02h
    out  dx, al
    mov  al, cl
    shl  al, 03h
    out  dx, al
    mov  al, cl
    inc  al
    jne  Label0x60e4                    ;Offset 0x60e4
    jmp  Label0x610c                    ;Offset 0x610c
Label0x60fe:                            ;Offset 0x60fe
    mov  cl, al
    shl  al, 03h
    out  dx, al
    out  dx, al
    out  dx, al
    mov  al, cl
    inc  al
    jne  Label0x60fe                    ;Offset 0x60fe
Label0x610c:                            ;Offset 0x610c
    pop  cx
    pop  dx
    ret
Func0x60bc ENDP

Func0x610f PROC NEAR                    ;Offset 0x610f
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  ax, ax
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
    jne  Label0x612a                    ;Offset 0x612a
    shr  ax, 01h
    jmp  Label0x612c                    ;Offset 0x612c
Label0x612a:                            ;Offset 0x612a
    shl  ax, cl
Label0x612c:                            ;Offset 0x612c
    out  dx, al
    mov  dl, MGA_CRTCExtensionData_lowbyte;Port 0x3df
    mov  al, ah
    shl  al, 04h
    out  dx, al
    pop  dx
    ret
Func0x610f ENDP

Func0x6137 PROC NEAR                    ;Offset 0x6137
    push ds
    push es
    push si
    push cx
    push bx
    push dx
    xor  ax, ax
    mov  es, ax
    call GetMemoryIn64KBlocks           ;Offset 0x673e
    cmp  ax, 0080h
    jbe  Label0x614c                    ;Offset 0x614c
    mov  ax, 0080h
Label0x614c:                            ;Offset 0x614c
    mov  byte ptr es:[BDA_VideoBufferSize + 1], al;Offset 0x44d
    mov  byte ptr es:[BDA_VideoBufferSize], 00h;Offset 0x44c
    mov  al, byte ptr es:[BDA_EGAFeatureBitSwitches];Offset 0x488
    shr  al, 04h
    and  al, 03h
    je   Label0x624f                    ;Offset 0x624f
    push ax
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_AddressExtension;0x8 Select lower 16MB
    out  dx, al
    in   ax, dx
    and  ah, NOT MGA_CRTCEXT8_PageRegisterBit8;0xef
    out  dx, ax
    mov  ax, MGA_CRTCExt_MemoryPage     ;0x4 Select memory page 0
    out  dx, ax
    pop  ax
    sub  al, 02h
    js   Label0x61ec                    ;Offset 0x61ec
    je   Label0x61b1                    ;Offset 0x61b1
    mov  byte ptr es:[BDA_VideoBufferSize], 03h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    mov  ch, MGA_MULCTRL_Depth32Pal     ;0x7
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_ScaleDiv4 OR MGA_CRTCEXT3_MGAModeEnable) SHL 8);0x8303
    out  dx, ax
    call Func0x60bc                     ;Offset 0x60bc
    mov  cx, 0002h
    jmp  Label0x6218                    ;Offset 0x6218
Label0x61b1:                            ;Offset 0x61b1
    mov  byte ptr es:[BDA_VideoBufferSize], 02h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    mov  ch, MGA_MULCTRL_Depth16        ;0x2
    test byte ptr es:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnector1_0;Offset 0x488 0x40
    jne  Label0x61da                    ;Offset 0x61da
    mov  ch, MGA_MULCTRL_Depth15        ;0x1
Label0x61da:                            ;Offset 0x61da
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv2) SHL 8);0x8103
    out  dx, ax
    call Func0x60bc                     ;Offset 0x60bc
    mov  cx, 0000h
    jmp  Label0x6218                    ;Offset 0x6218
Label0x61ec:                            ;Offset 0x61ec
    mov  byte ptr es:[BDA_VideoBufferSize], 01h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT MGA_MISCCTRL_DAC_8Bit  ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    xor  ch, ch
    call WriteIndexedRegister           ;Offset 0x6b6e
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv1) SHL 8);8003h
    out  dx, ax
    mov  cx, 0001h
Label0x6218:                            ;Offset 0x6218
    push cx
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6b83
    and  cl, NOT (MGA_MISCCTRL_DAC_Enable OR MGA_MISCCTRL_MAFC_MASK);0xf8
    test byte ptr cs:[Data0x7ff1], 20h  ;Offset 0x7ff1
    je   Label0x622e                    ;Offset 0x622e
    or   cl, MGA_MISCCTRL_DAC_Enable    ;0x1
Label0x622e:                            ;Offset 0x622e
    or   cl, MGA_MISCCTRL_MAFC_PanelLinkMode;0x4
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    jne  Label0x623c                    ;Offset 0x623c
    or   cl, MGA_MISCCTRL_DAC_Enable OR MGA_MISCCTRL_MAFC_Disable;0x7
Label0x623c:                            ;Offset 0x623c
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6b6e
    pop  cx
    or   cl, cl
    je   Label0x624f                    ;Offset 0x624f
    dec  cl
    call Func0x610f                     ;Offset 0x610f
Label0x624f:                            ;Offset 0x624f
    pop  dx
    pop  bx
    pop  cx
    pop  si
    pop  es
    pop  ds
    ret
Func0x6137 ENDP

Func0x6256 PROC NEAR                    ;Offset 0x6256
    push      bx
    push      cx
    push      dx
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x66f6
    je        Return                    ;Offset 0x62f7
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, NOT (MGA_MISCCTRL_MAFC_MAFC OR MGA_MISCCTRL_DAC_8Bit);0xf5
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    jne       Label0x627a               ;Offset 0x627a
    and       cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_8Bit);0xf1h
Label0x627a:                            ;Offset 0x627a
    mov       ch, cl
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, NOT MGA_GENCTRL_PedestalContr7p5IRE;0xef
    mov       al, byte ptr cs:[Data0x7d3a];Offset 0x7d3a
    and       al, 01h
    shl       al, 04h                   ;Pedestal control?
    or        cl, al
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, NOT MGA_GENCTRL_GreenSyncOutDisable;0xdf
    mov       al, byte ptr cs:[Data0x7d3a];Offset 0x7d3a
    and       al, 20h                   ;green sync out config
    or        cl, al
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_MultiplexControl;0x19
    nop
    nop
    xor       ch, ch
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, MGA_GENCTRL_PedestalContr7p5IRE OR MGA_GENCTRL_GreenSyncOutDisable;x30
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_CursorControl;0x6
    nop
    nop
    mov       ch, MGA_CURCTRL_CursorDisabled;0x0
    call      WriteIndexedRegister      ;Offset 0x6b6e
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6b83
    and       cl, NOT MGA_PIXCLKCTRL_SelMASK;0fch
    or        cl, MGA_PIXCLKCTRL_SelPLL ;0x1
    mov       ch, cl
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6b6e
Return:                                 ;Offset 0x62f7
    mov       ax, (MGA_CRTCExt_AddrGeneratorExt SHL 8) OR 00h;0x0
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    mov       ax, (MGA_CRTCExt_VertCounterExt SHL 8) OR 00h;0x200
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       ah, byte ptr cs:[Data0x7d3a];Offset 0x7d3a
    and       ah, 10h                   ;bit 4
    shl       ah, 02h                   ;moves to bit 6, MGA_CRTCEXT3_CompSyncEnable
    mov       al, MGA_CRTCExt_Misc      ;0x3
    out       dx, ax
    mov       ax, (MGA_CRTCExt_MemoryPage SHL 8) OR 00h;0x400
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    mov       ax, (MGA_CRTCExt_HorVidHalfCount SHL 8) OR 00h;0x500
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    mov       ax, (MGA_CRTCExt_AddressExtension SHL 8) OR 00h;0x800
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    call      Func0x6b4d                ;Offset 0x6b4d
    pop       dx
    pop       cx
    pop       bx
    ret
Func0x6256 ENDP

Func0x632d PROC NEAR                    ;Offset 0x632d
    push      es
    push      di
    push      dx
    push      ecx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    nop
    call      GetMemoryIn64KBlocks      ;Offset 0x673e
    xchg      ah, al
    test      al, 02h
    je        Label0x634d               ;Offset 0x634d
    mov       al, MGA_CRTCExt_AddressExtension;0x8
    out       dx, al
    inc       dx
    in        al, dx
    or        al, MGA_CRTCEXT8_PageRegisterBit8;0x10
    out       dx, al
    dec       dx
Label0x634d:                            ;Offset 0x634d
    mov       al, MGA_CRTCExt_MemoryPage;0x4
Label0x634f:                            ;Offset 0x634f
    dec       ah
    jb        Label0x6369               ;Offset 0x6369
    out       dx, ax
    push      ax
    xor       eax, eax
    mov       di, 0000h
    mov       ecx, 00004000h
    rep stosd
    pop       ax
    or        ah, ah
    jne       Label0x634f               ;Offset 0x634f
Label0x6369:                            ;Offset 0x6369
    mov       al, MGA_CRTCExt_AddressExtension;0x8
    out       dx, al
    inc       dx
    in        al, dx
    test      al, MGA_CRTCEXT8_PageRegisterBit8;0x10
    je        Label0x637a               ;Offset 0x637a
    and       al, NOT MGA_CRTCEXT8_PageRegisterBit8;0xef
    out       dx, al
    mov       al, MGA_CRTCExt_MemoryPage;0x4
    dec       dx
    jmp       Label0x634f               ;Offset 0x634f
Label0x637a:                            ;Offset 0x637a
    pop       ecx
    pop       dx
    pop       di
    pop       es
    ret
Func0x632d ENDP

Func0x6380 PROC NEAR                    ;Offset 0x6380
    push      bp
    mov       bp, sp
    push      es
    push      ebx
    push      edx
    sub       sp, 001ch
    push      ss
    pop       es
    mov       di, sp
    mov       bx, ax
    shl       ebx, 10h
    xor       dx, dx
    mov       ds, dx
    lds       si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
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
    or        al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    out       dx, al
    pop       dx
    pop       ax
    ;mov       edx, dword ptr [si]
    DB 066h, 08Bh, 094h, 000h, 000h
    call      Func0x6f82                ;Offset 0x6f82
    xor       edx, edx
    xor       eax, eax
    ;mov       ax, word ptr [si + 0ah]
    DB 08Bh, 084h, 00Ah, 000h
    mov       bx, ds
    mov       cx, bx
    or        cx, ax
    je        Label0x63f1               ;Offset 0x63f1
    push      ax
    mov       cx, 0004h
    shr       ax, cl
    add       bx, ax
    ;mov       al, byte ptr [si + 08h]
    DB 08Ah, 084h, 008h, 000h
    xor       ah, ah
    shl       ax, 01h
    shl       ax, 01h
    sub       bx, ax
    pop       ax
    ;and       ax, 000fh
    DB 025h, 00Fh, 000h
    mov       word ptr es:[di], ax
    mov       word ptr es:[di + 02h], bx
Label0x63f1:                            ;Offset 0x63f1
    ;mov       dword ptr es:[di + 08h], 00000000h
    DB 026h, 066h, 0C7h, 045h, 008h, 000h, 000h, 000h, 000h
    push      ebx
    shr       ebx, 10h
    and       bh, 80h
    ;mov       dl, byte ptr [si + 09h]
    DB 08Ah, 094h, 009h, 000h
    or        dl, bh
    pop       ebx
    xor       ax, ax
    mov       ds, ax
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], di;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], es;Offset 0x4aa
    mov       ax, dx
    int       6dh
    call      TurnScreenOff             ;Offset 0x2b1f
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs];Offset 0x4a8
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg];Offset 0x4aa
    lds       si, [bp + 04h]
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    ;mov       ah, byte ptr [si + 0ch]
    DB 08Ah, 0A4h, 00Ch, 000h
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    ;mov       ah, byte ptr [si + 0dh]
    DB 08Ah, 0A4h, 00Dh, 000h
    mov       al, MGA_CRTCExt_VertCounterExt;0x2
    out       dx, ax
    shr       ebx, 10h
    push      bx
    add       bl, 20h
    xor       ax, ax
    mov       es, ax
    mov       al, byte ptr cs:[Data0x7ff1];Offset 0x7ff1
    and       al, 0c0h
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    je        Label0x6456               ;Offset 0x6456
    mov       al, 00h
Label0x6456:                            ;Offset 0x6456
    or        al, bl
    mov       ah, MGA_CRTCExt_HorVidHalfCount;0x5
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    mov       al, bl
    pop       bx
    mov       ah, bh
    and       ah, 0c0h
    or        al, ah
    mov       byte ptr es:[BDA_DisplayMode], al;Offset 0x449
    test      byte ptr es:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnector1_1;Offset 0x488 0x80
    je        Label0x647f               ;Offset 0x647f
    push      si
    push      ds
    mov       al, 02h
    mov       bx, 0000h
    call      TextFunctions             ;Offset 0x1fc9
    pop       ds
    pop       si
Label0x647f:                            ;Offset 0x647f
    call      Func0x6137                ;Offset 0x6137
    test      bh, 80h
    jne       Label0x6494               ;Offset 0x6494
    mov       al, byte ptr es:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x70
    cmp       al, BDA_EFBS_FeatureConnector1_0;0x40
    je        Label0x6494               ;Offset 0x6494
    call      Func0x632d                ;Offset 0x632d
Label0x6494:                            ;Offset 0x6494
    xor       eax, eax
    call      Func0x6526                ;Offset 0x6526
    call      Func0x694c                ;Offset 0x694c
    mov       ah, MGA_CRTCExt_MemoryPage;0x4
    mov       al, bl
    call      MGAWriteCRTCExtensionRegister;Offset 0x328
    call      TurnScreenOn              ;Offset 0x2b1b
    call      Func0x6b4d                ;Offset 0x6b4d
    add       sp, 001ch
    xor       ax, ax
    pop       edx
    pop       ebx
    pop       es
    pop       bp
    ret
Func0x6380 ENDP

Func0x64b6 PROC NEAR                    ;Offset 0x64b6
    push      ds
    xor       ax, ax
    mov       ds, ax
    mov       ax, 0001h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x64cb               ;Offset 0x64cb
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    shr       al, 04h
Label0x64cb:                            ;Offset 0x64cb
    ;and       ax, 0003h
    DB 025h, 003h, 000h
    pop       ds
    ret   
Func0x64b6 ENDP

Func0x64d0 PROC NEAR                    ;Offset 0x64d0
    push      ds
    push      0000h
    pop       ds
    or        al, al
    mov       ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    je        Label0x64e5               ;Offset 0x64e5
    shr       al, 04h
    and       cl, 0f0h
    or        cl, al
    jmp       Label0x64f1               ;Offset 0x64f1
Label0x64e5:                            ;Offset 0x64e5
    push      cx
    shl       cl, 04h
    and       al, 0fh
    or        al, cl
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    pop       cx
Label0x64f1:                            ;Offset 0x64f1
    pop       ds
    ret   
Func0x64d0 ENDP

Func0x64f3 PROC NEAR                    ;Offset 0x64f3
    push      dx
    xor       eax, eax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    nop
    mov       al, MGA_CRTCExt_AddrGeneratorExt;0x0
    out       dx, al
    inc       dx
    in        al, dx
    mov       dl, al
    and       dl, MGA_CRTEXT0_StartAddress20;0x40
    shr       dl, 02h
    and       al, MGA_CRTEXT0_StartAddress19_16;0xf
    or        al, dl
    shl       eax, 10h
    mov       dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov       al, VGA_CRTCIdx_StartAddrHigh;0xc
    out       dx, al
    inc       dx
    in        al, dx
    xchg      al, ah
    dec       dx
    mov       al, VGA_CRTCIdx_StartAddrLow;0xd
    out       dx, al
    inc       dx
    in        al, dx
    mov       dl, 01h
    call      Func0x69c1                ;Offset 0x69c1
    pop       dx
    ret
Func0x64f3 ENDP

Func0x6526 PROC NEAR                    ;Offset 0x6526
    push      cx
    push      dx
    mov       dl, 00h
    call      Func0x69c1                ;Offset 0x69c1
    push      eax
    test      bl, 80h
    ;je        Label0x6543               ;Offset 0x6543
    DB 00Fh, 084h, 00Dh, 000h
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
    xor       cx, cx
Label0x653b:                            ;Offset 0x653b
    dec       cx
    je        Label0x6543               ;Offset 0x6543
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x653b               ;Offset 0x653b
Label0x6543:                            ;Offset 0x6543
    mov       dx, VGA_CRTControllerIndexD;Port 0x3d4
    nop
    mov       al, VGA_CRTCIdx_StartAddrLow;0xd
    out       dx, al
    inc       dx
    pop       ax
    out       dx, al
    dec       dx
    mov       al, VGA_CRTCIdx_StartAddrHigh;0xc
    out       dx, ax
    mov       dl, MGA_CRTCExtensionIndex_lowbyte;0xde
    xor       al, al                    ;MGA_CRTCExt_AddrGeneratorExt
    out       dx, al
    inc       dx
    pop       ax
    mov       ah, al
    mov       ch, al
    and       ah, MGA_CRTEXT0_StartAddress19_16;0xf
    and       ch, 10h
    shl       ch, 02h
    in        al, dx
    and       al, MGA_CRTEXT0_Offset9_8 OR MGA_CRTEXT0_Interlace;0xb0
    or        al, ah
    or        al, ch
    out       dx, al
    pop       dx
    pop       cx
    ret
Func0x6526 ENDP

Func0x6570 PROC NEAR                    ;Offset 0x6570
    push      ebx
    push      edx
    xor       ebx, ebx
    call      Func0x6605                ;Offset 0x6605
    shr       bx, 03h
    call      Func0x64f3                ;Offset 0x64f3
    mov       dl, byte ptr cs:[BDA_VideoBufferSize];Offset 0x44c
    and       dl, 04h
    shr       dl, 02h
    or        al, dl
    xor       edx, edx
    div       ebx
    mov       cx, dx
    push      ax
    call      Func0x64b6                ;Offset 0x64b6
    je        Label0x65a5               ;Offset 0x65a5
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shl       ax, cl
    xchg      ax, cx
Label0x65a5:                            ;Offset 0x65a5
    pop       ax
    pop       edx
    pop       ebx
    mov       dx, ax
    mov       al, 01h
    call      Func0x64d0                ;Offset 0x64d0
    ret
Func0x6570 ENDP

Func0x65b2 PROC NEAR                    ;Offset 0x65b2
    push      ebx
    push      ecx
    push      edx
    xor       al, al
    call      Func0x64d0                ;Offset 0x64d0
    push      bx
    push      cx
    push      dx
    call      Func0x6605                ;Offset 0x6605
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
    call      Func0x64b6                ;Offset 0x64b6
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shr       ax, cl
    xchg      ax, cx
    pop       ax
    add       eax, ecx
    and       byte ptr cs:[BDA_VideoBufferSize], 0fbh;Offset 0x44c
    mov       cl, al
    and       cl, 01h
    shl       cl, 02h
    or        byte ptr cs:[BDA_VideoBufferSize], cl;Offset 0x44c
    and       al, 0feh
    call      Func0x6526                ;Offset 0x6526
    pop       edx
    pop       ecx
    pop       ebx
    ret
Func0x65b2 ENDP

Func0x6605 PROC NEAR                    ;Offset 0x6605
    push      ds
    xor       ax, ax
    mov       ds, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    nop
    out       dx, al                    ;MGA_CRTCExt_AddrGeneratorExt
    inc       dx
    in        al, dx
    shr       al, 04h
    xchg      al, ah
    and       ah, 03h
    mov       dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov       al, VGA_CRTCIdx_Offset    ;0x13
    out       dx, al
    inc       dx
    in        al, dx
    mov       bx, ax
    xor       ax, ax
    mov       dx, ax
    call      Func0x64b6                ;Offset 0x64b6
    mov       cl, al
    shl       bx, 04h
    dec       cl
    mov       ax, bx
    shr       ax, cl
    mov       cx, ax
    call      GetMemoryIn64KBlocks      ;Offset 0x673e
    cmp       ax, 0100h
    jbe       Label0x6641               ;Offset 0x6641
    mov       ax, 0100h
Label0x6641:                            ;Offset 0x6641
    mov       dx, ax
    xor       ax, ax
    div       bx
    mov       dx, ax
    pop       ds
    ret
Func0x6605 ENDP

Func0x664b PROC NEAR                    ;Offset 0x664b
    push      ds
    push      cx
    xor       cx, cx
    mov       ds, cx
    mov       ch, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    shr       ch, 04h
    and       ch, 03h
    dec       ch
    mov       cl, 04h
    sub       cl, ch
    shr       ax, cl
    push      ax
    shl       ah, 04h
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    nop
    xor       al, al                    ;MGA_CRTCExt_AddrGeneratorExt
    out       dx, al
    inc       dx
    in        al, dx
    and       al, NOT MGA_CRTEXT0_Offset9_8;0xcf
    or        al, ah
    out       dx, al
    mov       dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    mov       al, VGA_CRTCIdx_Offset    ;0x13
    out       dx, al
    inc       dx
    pop       ax
    out       dx, al
    pop       cx
    pop       ds
    ret   
Func0x664b ENDP

Func0x6680 PROC NEAR                    ;Offset 0x6680
    mov       bx, ax
    call      Func0x64b6                ;Offset 0x64b6
    je        Label0x669f               ;Offset 0x669f
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
    je        Label0x669f               ;Offset 0x669f
    sub       bx, dx
    add       ax, bx
Label0x669f:                            ;Offset 0x669f
    ret
Func0x6680 ENDP

Func0x66a0 PROC NEAR                    ;Offset 0x66a0
    push      dx
    push      cx
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x66b4               ;Offset 0x66b4
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    ;jne       Label0x66bb               ;Offset 0x66bb
    DB 00Fh, 085h, 007h, 000h
Label0x66b4:                            ;Offset 0x66b4
    call      Func0x64b6                ;Offset 0x64b6
    mov       cl, al
    jne       Label0x66c1               ;Offset 0x66c1
Label0x66bb:                            ;Offset 0x66bb
    ;mov       ax, word ptr [si + 04h]
    DB 08Bh, 084h, 004h, 000h
    jmp       Label0x66f3               ;Offset 0x66f3
Label0x66c1:                            ;Offset 0x66c1
    dec       cl
    push      cx
    call      GetMemoryIn64KBlocks      ;Offset 0x673e
    cmp       ax, 0100h
    jbe       Label0x66cf               ;Offset 0x66cf
    mov       ax, 0100h
Label0x66cf:                            ;Offset 0x66cf
    mov       dx, ax
    ;mov       ax, word ptr [si + 06h]
    DB 08Bh, 084h, 006h, 000h
    shl       ax, cl
    mov       cx, ax
    xor       ax, ax
    div       cx
    pop       cx
    shl       ax, cl
    shr       ax, 04h
    cmp       ax, 03f0h
    jbe       Label0x66eb               ;Offset 0x66eb
    mov       ax, 03f0h
Label0x66eb:                            ;Offset 0x66eb
    shl       ax, 04h
    shr       ax, cl
    call      Func0x6680                ;Offset 0x6680
Label0x66f3:                            ;Offset 0x66f3
    pop       cx
    pop       dx
    ret
Func0x66a0 ENDP

;
;inputs:
;   -
;
;outputs:
;   equal = failure
;   not equal = success
;
;destroys:
;   -
CheckMemoryMappedRegsAndExtendedRegister PROC NEAR;Offset 0x66f6
    push      bx
    push      cx
    push      dx
    push      si
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       si, MGA_MemAddr_PaletteRamWriteAddress;Offset 0x3c00
    nop
    call      IndirectRegisterReadByte  ;Offset 0x6afe  Read current index
    not       cl                        ;invert index
    call      IndirectRegisterWriteByte ;Offset 0x6aa9  write back inverted index
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    in        al, dx                    ;read back index
    mov       ah, cl                    ;store written index in ah. 
    push      ax                        ;preserve ax
    not       cl                        ;restore index to original value
    call      IndirectRegisterWriteByte ;Offset 0x6aa9
    mov       cl, 01h
    pop       ax                        ;restore ax. al = port read inverted index, ah = indirect read inverted index
    ;cmp       ah, al                    ;compare the two values.
    DB 03Ah, 0E0h
    jne       Failure                   ;Offset 0x6736
    mov       cl, 00h                   ;Set to zero
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_Unknown99 ;0x99
    out       dx, al
    in        ax, dx                    ;Store off requester control
    push      ax
    mov       ax, (0a0h SHL 8) OR MGA_CRTCExt_Unknown99;0xa099
    out       dx, ax
    in        ax, dx
    cmp       ah, 0a0h
    jne       Success                   ;Offset 0x6734
    mov       cl, 01h
Success:                                ;Offset 0x6734
    pop       ax
    out       dx, ax
Failure:                                ;Offset 0x6736
    cmp       cl, 01h
    pop       si
    pop       dx
    pop       cx
    pop       bx
    ret
CheckMemoryMappedRegsAndExtendedRegister ENDP

GetMemoryIn64KBlocks PROC NEAR          ;Offset 0x673e
    push      dx
    mov       ax, 0bd50h
    xor       dx, dx
    int       6dh
    or        ah, ah
    mov       ax, dx
    pop       dx
    ;je        Label0x6755               ;Offset 0x6755
    DB 00Fh, 084h, 006h, 000h
    movzx     ax, byte ptr cs:[MemorySizeInMB];Offset 0x7ff0
Label0x6755:                            ;Offset 0x6755
    shl       ax, 04h
    ret
GetMemoryIn64KBlocks ENDP

Func0x6759 PROC NEAR                    ;Offset 0x6759
    mov   ax, 0af03h
    int   6dh
    cmp   ax, 0af03h
    je    Label0x6764                   ;Offset 0x6764
    ret
Label0x6764:                            ;Offset 0x6764
    push  di
    xor   ax, ax
    mov   ds, ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    and   al, 3fh
    cmp   al, 20h
    jl    Label0x678d                   ;Offset 0x678d
    cmp   al, 40h
    jge   Label0x678d                   ;Offset 0x678d
    sub   al, 20h
    lea   si, [Data0x549f]              ;Offset 0x549f
    push  ax
    ;and   ax, 001fh
    DB 025h, 01Fh, 000h
    shl   ax, 02h
    add   si, ax
    mov   si, word ptr cs:[si]
    pop   ax
    inc   ah
    jmp   Label0x678f                   ;Offset 0x678f
Label0x678d:                            ;Offset 0x678d
    xor   si, si
Label0x678f:                            ;Offset 0x678f
    push  cs
    pop   ds
    pop   di
    ret
Func0x6759 ENDP

Func0x6793 PROC NEAR                    ;Offset 0x6793
    push  bx
    push  cx
    push  dx
    xor   dx, dx
    mov   cx, ax
    mov   ax, word ptr ss:[si + 0ah]
    ;sub   ax, 05h
    DB 02Dh, 005h, 000h
    mov   byte ptr es:[di + 0ah], al
    and   ah, 01h
    or    dl, ah
    mov   ax, word ptr ss:[si]
    dec   ax
    mov   byte ptr es:[di + 0bh], al
    mov   ax, word ptr ss:[si + 02h]
    dec   ax
    mov   byte ptr es:[di + 0ch], al
    and   ah, 01h
    shl   ah, 01h
    or    dl, ah
    mov   ax, word ptr ss:[si + 04h]
    dec   ax
    mov   word ptr es:[di + 0eh], ax
    and   ah, 01h
    shl   ah, 02h
    or    dl, ah
    mov   ax, word ptr ss:[si + 06h]
    dec   ax
    and   al, 1fh
    and   byte ptr es:[di + 0fh], 60h
    or    byte ptr es:[di + 0fh], al
    mov   al, byte ptr ss:[si + 08h]
    dec   al
    mov   bl, al
    and   bl, 20h
    shl   bl, 02h
    or    byte ptr es:[di + 0fh], bl
    mov   bl, al
    and   bl, 1fh
    or    bl, 80h
    mov   byte ptr es:[di + 0dh], bl
    and   al, 40h
    or    dl, al
    mov   ax, word ptr ss:[si + 16h]
    or    cx, cx
    jne   Label0x681d                   ;Offset 0x681d
    mov   ch, 00h
    cmp   ax, 03ffh
    jb    Label0x681d                   ;Offset 0x681d
    mov   ch, 0ffh
    shr   ax, 01h
    or    byte ptr es:[di + 21h], 04h
Label0x681d:                            ;Offset 0x681d
    ;sub   ax, 0002h
    DB 02Dh, 002h, 000h
    mov   byte ptr es:[di + 10h], al
    and   byte ptr es:[di + 11h], 10h
    mov   bh, ah
    and   bh, 01h
    or    byte ptr es:[di + 11h], bh
    mov   bh, ah
    and   bh, 02h
    shl   bh, 04h
    or    byte ptr es:[di + 11h], bh
    and   ah, 0ch
    shr   ah, 02h
    or    dh, ah
    mov   ax, word ptr ss:[si + 0ch]
    test  ch, 0ffh
    je    Label0x6851                   ;Offset 0x6851
    shr   ax, 01h
Label0x6851:                            ;Offset 0x6851
    dec   ax
    mov   byte ptr es:[di + 1ch], al
    mov   bh, ah
    and   bh, 01h
    shl   bh, 01h
    or    byte ptr es:[di + 11h], bh
    mov   bh, ah
    and   bh, 02h
    shl   bh, 05h
    or    byte ptr es:[di + 11h], bh
    and   ah, 04h
    or    dh, ah
    mov   ax, word ptr ss:[si + 0eh]
    test  ch, 0ffh
    je    Label0x687d                   ;Offset 0x687d
    shr   ax, 01h
Label0x687d:                            ;Offset 0x687d
    dec   ax
    mov   byte ptr es:[di + 1fh], al
    mov   bh, ah
    and   bh, 01h
    shl   bh, 03h
    or    byte ptr es:[di + 11h], bh
    mov   bh, ah
    and   bh, 02h
    shl   bh, 04h
    or    byte ptr es:[di + 13h], bh
    and   ah, 0ch
    shl   ah, 01h
    or    dh, ah
    mov   ax, word ptr ss:[si + 10h]
    test  ch, 0ffh
    je    Label0x68ac                   ;Offset 0x68ac
    shr   ax, 01h
Label0x68ac:                            ;Offset 0x68ac
    dec   ax
    mov   byte ptr es:[di + 1ah], al
    mov   bh, ah
    and   bh, 01h
    shl   bh, 02h
    or    byte ptr es:[di + 11h], bh
    mov   bh, ah
    and   bh, 02h
    shl   bh, 06h
    or    byte ptr es:[di + 11h], bh
    and   ah, 0ch
    shl   ah, 03h
    or    dh, ah
    mov   ax, word ptr ss:[si + 12h]
    test  ch, 0ffh
    je    Label0x68dc                   ;Offset 0x68dc
    shr   ax, 01h
Label0x68dc:                            ;Offset 0x68dc
    dec   ax
    ;and   ax, 0fh
    DB 025h, 00Fh, 000h
    mov   bh, byte ptr es:[di + 1bh]
    and   bh, 0f0h
    or    al, bh
    mov   byte ptr es:[di + 1bh], al
    mov   ax, word ptr ss:[si + 14h]
    test  ch, 0ffh
    je    Label0x68f8                   ;Offset 0x68f8
    shr   ax, 01h
Label0x68f8:                            ;Offset 0x68f8
    dec   ax
    mov   byte ptr es:[di + 20h], al
    mov   al, byte ptr es:[di + 05h]
    and   al, 0f7h
    or    al, 01h
    mov   byte ptr es:[di + 05h], al
    and   byte ptr es:[di + 36h], 07h
    and   byte ptr es:[di + 09h], 0f3h
    mov   ax, dx
    pop   dx
    pop   cx
    pop   bx
    ret
Func0x6793 ENDP

Func0x6919 PROC NEAR                    ;Offset 0x6919
    push  cx
    mov   bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov   al, byte ptr cs:[Data0x7ff1]  ;Offset 0x7ff1
    xor   cx, cx
    test  al, 80h
    nop
    nop
    je    Label0x692f                   ;Offset 0x692f
    or    ch, 20h
    nop
Label0x692f:                            ;Offset 0x692f
    test  al, 04h
    nop
    nop
    je    Label0x6939                   ;Offset 0x6939
    or    ch, 04h
    nop
Label0x6939:                            ;Offset 0x6939
    test  al, 02h
    nop
    nop
    je    Label0x6943                   ;Offset 0x6943
    or    ch, 01h
    nop
Label0x6943:                            ;Offset 0x6943
    mov   cl, 8ah                       ;undocumented register?
    nop
    nop
    call  WriteIndexedRegister          ;Offset 0x6b6e
    pop   cx
    ret
Func0x6919 ENDP

Func0x694c PROC NEAR                    ;Offset 0x694c
    push  ax
    push  dx
    push  si
    xor   ebx, ebx
    test  byte ptr cs:[Data0x7ff1], 80h ;Offset 0x7ff1
    je    Label0x69bd                   ;Offset 0x69bd
    test  byte ptr cs:[Data0x7ff1], 20h ;Offset 0x7ff1
    jne   Label0x69bd                   ;Offset 0x69bd
    push  ds
    push  0000h
    pop   ds
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop   ds
    and   al, 3fh
    cmp   al, 20h
    jb    Label0x69bd                   ;Offset 0x69bd
    test  byte ptr cs:[Data0x7ff1], 40h ;Offset 0x7ff1
    nop
    jne   Label0x6993                   ;Offset 0x6993
    cmp   al, 23h
    ja    Label0x6984                   ;Offset 0x6984
    lea   si, [Data0x711e]              ;Offset 0x711e
    jmp   Label0x69aa                   ;Offset 0x69aa
    nop
Label0x6984:                            ;Offset 0x6984
    cmp   al, 30h
    jb    Label0x69bd                   ;Offset 0x69bd
    cmp   al, 35h
    ja    Label0x69bd                   ;Offset 0x69bd
    lea   si, [Data0x712a]              ;Offset 0x712a
    jmp   Label0x69aa                   ;Offset 0x69aa
    nop
Label0x6993:                            ;Offset 0x6993
    cmp   al, 25h
    ja    Label0x699e                   ;Offset 0x699e
    lea   si, [Data0x713c]              ;Offset 0x713c
    jmp   Label0x69aa                   ;Offset 0x69aa
    nop
Label0x699e:                            ;Offset 0x699e
    cmp   al, 30h
    jb    Label0x69bd                   ;Offset 0x69bd
    cmp   al, 38h
    ja    Label0x69bd                   ;Offset 0x69bd
    lea   si, [Data0x714e]              ;Offset 0x714e
Label0x69aa:                            ;Offset 0x69aa
    and   al, 0fh
    mov   bl, 03h
    mul   bl
    add   si, ax
    mov   bx, word ptr cs:[si]
    shl   ebx, 08h
    mov   bl, byte ptr cs:[si + 02h]
Label0x69bd:                            ;Offset 0x69bd
    pop   si
    pop   dx
    pop   ax
    ret
Func0x694c ENDP

Func0x69c1 PROC NEAR                    ;Offset 0x69c1
    push  ebx
    call  Func0x694c                    ;Offset 0x694c
    shr   ebx, 08h
    cmp   dl, 00h
    jne   Label0x69d4                   ;Offset 0x69d4
    add   eax, ebx
    jmp   Label0x69d7                   ;Offset 0x69d7
Label0x69d4:                            ;Offset 0x69d4
    sub   eax, ebx
Label0x69d7:                            ;Offset 0x69d7
    pop   ebx
    ret
Func0x69c1 ENDP

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
AccessPCIRegister PROC NEAR             ;Offset 0x69da
    push  eax
    push  bx
    push  dx
    push  di
    pushf
    cli
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    cmp   bh, 0c0h
    jb    Mechanism1                    ;Offset 0x69fd
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
    jmp   PerformAccess                 ;Offset 0x6a19
Mechanism1:                             ;Offset 0x69fd
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
PerformAccess:                          ;Offset 0x6a19
    cmp   al, PCI_ACCESS_ReadByte       ;0x8
    je    ReadByte                      ;Offset 0x6a50
    cmp   al, PCI_ACCESS_ReadWord       ;0x9
    je    ReadWord                      ;Offset 0x6a4b
    cmp   al, PCI_ACCESS_ReadDWord      ;0xa
    je    ReadDWord                     ;Offset 0x6a44
    cmp   al, PCI_ACCESS_WriteByte      ;0xb
    je    WriteByte                     ;Offset 0x6a3f
    cmp   al, PCI_ACCESS_WriteWord      ;0xc
    je    WriteWord                     ;Offset 0x6a3a
    cmp   al, PCI_ACCESS_WriteDWord     ;0xd
    je    WriteDWord                    ;Offset 0x6a33
    jmp   Done                          ;Offset 0x6a53
WriteDWord:                             ;Offset 0x6a33
    mov   eax, ecx
    out   dx, eax
    jmp   Done                          ;Offset 0x6a53
WriteWord:                              ;Offset 0x6a3a
    mov   ax, cx
    out   dx, ax
    jmp   Done                          ;Offset 0x6a53
WriteByte:                              ;Offset 0x6a3f
    mov   al, cl
    out   dx, al
    jmp   Done                          ;Offset 0x6a53
ReadDWord:                              ;Offset 0x6a44
    in    eax, dx
    mov   ecx, eax
    jmp   Done                          ;Offset 0x6a53
ReadWord:                               ;Offset 0x6a4b
    in    ax, dx
    mov   cx, ax
    jmp   Done                          ;Offset 0x6a53
ReadByte:                               ;Offset 0x6a50
    in    al, dx
    mov   cl, al
Done:                                   ;Offset 0x6a53
    cmp   dh, 0c0h
    jb    IsMechanism1                  ;Offset 0x6a60
    xor   al, al
    mov   dx, PCI_M2_ConfigSpaceEnable  ;Port 0xcf8
    out   dx, al
    jmp   Return                        ;Offset 0x6a68
IsMechanism1:                           ;Offset 0x6a60
    xor   eax, eax
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    out   dx, eax
Return:                                 ;Offset 0x6a68
    popf
    pop   di
    pop   dx
    pop   bx
    pop   eax
    ret
AccessPCIRegister ENDP

;
;inputs:
;   bx = Bus, Device ID and Function ID
;
;outputs:
;   eax = BaseAddress1
;   ecx = BaseAddress0
;
;destroys:
;   -
;
ReadPCIBase0AndBase1 PROC NEAR          ;Offset 0x6a6f
    push  di
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 + 2;0x12
    call  AccessPCIRegister             ;Offset 0x69da
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 ;0x10
    call  AccessPCIRegister             ;Offset 0x69da
    and   ecx, 0fffffff0h
    push  ecx
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 + 2;0x16
    call  AccessPCIRegister             ;Offset 0x69da
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 ;0x14
    call  AccessPCIRegister             ;Offset 0x69da
    and   ecx, 0fffffff0h
    mov   eax, ecx
    pop   ecx
    pop   di
    ret
ReadPCIBase0AndBase1 ENDP

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
IndirectRegisterWriteByte PROC NEAR     ;Offset 0x6aa9
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xch
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    pop    cx
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x69da
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
IndirectRegisterWriteWord PROC NEAR     ;Offset 0x6ac7
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    pop    cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x69da
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
IndirectRegisterWriteDWord PROC NEAR    ;Offset 0x6ae5
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    pop    cx
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x69da
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
IndirectRegisterReadByte PROC NEAR      ;Offset 0x6afe
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x69da
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
IndirectRegisterReadWord PROC NEAR      ;Offset 0x6b1a
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, PCI_ACCESS_ReadWord      ;0x9
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x69da
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
IndirectRegisterReadDWord PROC NEAR     ;Offset 0x6b36
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x69da
    pop    di
    pop    si
    ret
IndirectRegisterReadDWord ENDP

Func0x6b4d PROC NEAR                    ;Offset 0x6b4d
    push   ax
    push   bx
    push   cx
    push   di
    cmp    byte ptr cs:[DeviceRevision], MGA_G400_Rev2;Offset 0x7ff4 0x2
    jne    Label0x6b69                  ;Offset 0x6b69
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    mov    cx, 1e14h                    ;M and N value?
    call   AccessPCIRegister            ;Offset 0x69da
Label0x6b69:                            ;Offset 0x6b69
    pop    di
    pop    cx
    pop    bx
    pop    ax
    ret
Func0x6b4d ENDP

;
;inputs:
;   cl = RAMDAC write index
;   ch = Value
;
;destroys:
;   ax
;
WriteIndexedRegister PROC NEAR          ;Offset 0x6b6e
    push   si
    push   dx
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
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
ReadIndexedRegister PROC NEAR           ;Offset 0x6b83
    push   si
    push   dx
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterReadByte     ;Offset 0x6afe
    pop    dx
    pop    si
    ret
ReadIndexedRegister ENDP

;Offset 0x6b96
Data0x6b96              DB 003h, 001h
                        DB 005h, 002h
                        DB 009h, 004h
                        DB 002h, 001h
                        DB 003h, 002h
                        DB 001h, 001h
                        DB 001h, 001h
                        DB 001h, 001h

Func0x6ba6 PROC NEAR                    ;Offset 0x6ba6
    push  ax
    push  bx
    movzx dx, byte ptr cs:[Data0x7d18]  ;Offset 0x7d18
    cmp   dx, 0000h
    je    Label0x6bbd                   ;Offset 0x6bbd
    cmp   dx, 0ffffh
    je    Label0x6bbd                   ;Offset 0x6bbd
    shl   dx, 02h
    jmp   Label0x6bed                   ;Offset 0x6bed
Label0x6bbd:                            ;Offset 0x6bbd
    movzx dx, byte ptr cs:[Data0x7d17]  ;Offset 0x7d17
    mov   ax, word ptr cs:[Option3Default];Offset 0x7d19
    mov   bl, al
    and   bl, 03h
    cmp   bl, 01h
    jne   Label0x6bed                   ;Offset 0x6bed
    lea   bx, [Data0x6b96]              ;Offset 0x6b96
    ;and   ax, 38h
    DB 025h, 038h, 000h
    shr   al, 02h
    add   bx, ax
    mov   al, byte ptr cs:[bx]
    mul   dl
    movzx bx, byte ptr cs:[bx + 01h]
    xor   dx, dx
    div   bx
    mov   dx, ax
Label0x6bed:                            ;Offset 0x6bed
    pop   bx
    pop   ax
    ret
Func0x6ba6 ENDP

Func0x6bf0 PROC NEAR                    ;Offset 0x6bf0
    push   bp
    mov    bp, sp
    sub    sp, 001ch
    push   ecx
    push   edx
    mov    dword ptr [bp - 18h], 0ffffffffh
    mov    dword ptr [bp - 04h], eax
    xor    eax, eax
    mov    al, byte ptr cs:[Data0x7d06] ;Offset 0x7d06
    cmp    al, 0ffh
    ;je     Label0x6c25                  ;Offset 0x6c25
    DB 00Fh, 084h, 012h, 000h
    shl    ax, 02h
    mov    ebx, 000003e8h
    mul    ebx
    mov    dword ptr [bp - 08h], eax
    jmp    Label0x6c2d                  ;Offset 0x6c2d
Label0x6c25:                            ;Offset 0x6c25
    mov    dword ptr [bp - 08h], 0004a380h
Label0x6c2d:                            ;Offset 0x6c2d
    mov    eax, dword ptr [bp - 04h]
    cmp    eax, 00001876h
    jae    Label0x6c3f                  ;Offset 0x6c3f
    mov    eax, 00001876h
Label0x6c3f:                            ;Offset 0x6c3f
    cmp    eax, dword ptr [bp - 08h]
    jbe    Label0x6c49                  ;Offset 0x6c49
    mov    eax, dword ptr [bp - 08h]
Label0x6c49:                            ;Offset 0x6c49
    mov    dword ptr [bp - 04h], eax
    xor    edx, edx
    mov    ebx, 00000005h
    mul    ebx
    mov    ebx, 000003e8h
    div    ebx
    mov    dword ptr [bp - 1ch], eax
    mov    ecx, 00000003h
Label0x6c6c:                            ;Offset 0x6c6c
    mov    eax, dword ptr [bp - 04h]
    shl    eax, cl
    shl    ecx, 10h
    cmp    eax, 0000c350h
    jb     Label0x6d26                  ;Offset 0x6d26
    cmp    eax, dword ptr [bp - 08h]
    ja     Label0x6d1c                  ;Offset 0x6d1c
    mov    ch, 01h
    mov    cl, 07h
Label0x6c8d:                            ;Offset 0x6c8d
    xor    edx, edx
    xor    ebx, ebx
    mov    eax, 00006978h
    test   byte ptr cs:[Data0x7d3c], 01h;Offset 0x7d3c
    ;je     Label0x6ca9                  ;Offset 0x6ca9
    DB 00Fh, 084h, 006h, 000h
    mov    eax, 000037eeh
Label0x6ca9:                            ;Offset 0x6ca9
    mov    bl, cl
    inc    bl
    mul    ebx
    mov    bl, ch
    inc    bl
    div    ebx
    cmp    eax, dword ptr [bp - 08h]
    ;ja     Label0x6d11                  ;Offset 0x6d11
    DB 00Fh, 087h, 052h, 000h
    ;cmp    eax, 0000c350h
    DB 066h, 03Dh, 050h, 0C3h, 000h, 000h
    ;jb    Label0x6d08                   ;Offset 0x6d08
    DB 00Fh, 082h, 03Fh, 000h
    mov    dword ptr [bp - 0ch], eax
    ror    ecx, 10h
    shr    eax, cl
    rol    ecx, 10h
    mov    ebx, dword ptr [bp - 04h]
    cmp    ebx, eax
    ;jb     Label0x6ce5                  ;Offset 0x6ce5
    DB 00Fh, 082h, 002h, 000h
    xchg   eax, ebx
Label0x6ce5:                            ;Offset 0x6ce5
    sub    eax, ebx
    mov    edx, dword ptr [bp - 0ch]
    cmp    eax, dword ptr [bp - 1ch]
    ;jbe    Label0x6d2e                  ;Offset 0x6d2e
    DB 00Fh, 086h, 03Ah, 000h
    cmp    eax, dword ptr [bp - 18h]
    ;jae    Label0x6d08                  ;Offset 0x6d08
    DB 00Fh, 083h, 00Ch, 000h
    mov    dword ptr [bp - 18h], eax
    mov    dword ptr [bp - 14h], ecx
    mov    dword ptr [bp - 10h], edx
Label0x6d08:                            ;Offset 0x6d08
    inc    cl
    cmp    cl, 7fh
    ;jbe    Label0x6c8d                  ;Offset 0x6c8d
    DB 00Fh, 086h, 07Ch, 0FFh
Label0x6d11:                            ;Offset 0x6d11
    mov    cl, 07h
    inc    ch
    cmp    ch, 1fh
    jbe    Label0x6c8d                  ;Offset 0x6c8d
Label0x6d1c:                            ;Offset 0x6d1c
    shr    ecx, 10h
    dec    cl
    jns    Label0x6c6c                  ;Offset 0x6c6c
Label0x6d26:                            ;Offset 0x6d26
    mov    ecx, dword ptr [bp - 14h]
    mov    edx, dword ptr [bp - 10h]
Label0x6d2e:                            ;Offset 0x6d2e
    mov    ebx, ecx
    and    ebx, 00001fffh
    shr    ecx, 10h
    mov    ax, 01h
    shl    ax, cl
    dec    ax
    shl    ax, 0dh
    or     bx, ax
    xor    eax, eax
    cmp    edx, 0001adb0h
    ja     Label0x6d58                  ;Offset 0x6d58
    mov    ax, 0000h
    jmp    Label0x6d77                  ;Offset 0x6d77
Label0x6d58:                            ;Offset 0x6d58
    cmp    edx, 00029810h
    ja     Label0x6d66                  ;Offset 0x6d66
    mov    ax, 0001h
    jmp    Label0x6d77                  ;Offset 0x6d77
Label0x6d66:                            ;Offset 0x6d66
    cmp    edx, 0003a980h
    ja     Label0x6d74                  ;Offset 0x6d74
    mov    ax, 0002h
    jmp    Label0x6d77                  ;Offset 0x6d77
Label0x6d74:                            ;Offset 0x6d74
    mov    ax, 0003h
Label0x6d77:                            ;Offset 0x6d77
    shl    eax, 10h
    or     ebx, eax
    mov    eax, edx
    shr    eax, cl
    pop    edx
    pop    ecx
    mov    sp, bp
    pop    bp
    ret
Func0x6bf0 ENDP

;
;inputs:
;   cx = bit 2 = gclkdiv
;            3 = mclkdiv
;
Func0x6d8c PROC NEAR                    ;Offset 0x6d8c
    push   eax
    push   ebx
    push   ecx
    push   edx
    push   esi
    mov    eax, edx
    mov    bx, 0000h
    call   Func0x6bf0                   ;Offset 0x6bf0
    push   ebx
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    di, PCI_MGA_Option3          ;0x54
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x69da
    and    ecx, NOT (PCI_MGA_OPTION3_GfxClkSelMASK OR PCI_MGA_OPTION3_MemClkSelMASK OR PCI_MGA_OPTION3_WrpClkSelMASK);0xffcff3fc
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x69da
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    and    cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    pop    edx
    mov    cl, MGA_INDD_SYSPLL_M_Value  ;0x2c
    mov    ch, dh
    and    ch, MGA_SYSPLLM_Mask         ;0x1f
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x6b6e
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
    xor    dx, dx
WaitForFrequencyLock:                   ;Offset 0x6e00
    dec    dx
    je     TimeOut                      ;Offset 0x6e0f
    mov    cl, MGA_INDD_SYSPLL_Status   ;0x2f
    call   ReadIndexedRegister          ;Offset 0x6b83
    and    cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je     WaitForFrequencyLock         ;Offset 0x6e00
    jmp    FrequencyLocked              ;Offset 0x6e35
TimeOut:                                ;Offset 0x6e0f
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    and    cl, NOT PCI_MGA_Opt_SysPLLPowerUp;0xdf
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, PCI_MGA_Opt_SysPLLPowerUp;0x20
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
FrequencyLocked:                        ;Offset 0x6e35
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    di, PCI_MGA_Option3          ;0x54
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    ;mov    ecx, dword ptr cs:[Option3Default];Offset 0x7d19
    DB 02Eh, 066h, 08Bh, 00Eh, 019h, 07Dh
    call   AccessPCIRegister            ;Offset 0x69da
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    and    cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    pop    esi
    pop    edx
    pop    ecx
    pop    ebx
    pop    eax
    ret
Func0x6d8c ENDP

Func0x6e6b PROC NEAR                    ;Offset 0x6e6b
    push   eax
    push   ebx
    push   ecx
    push   edx
    mov    edx, ebx
    push   edx
    push   cx
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    out    dx, al
    inc    dx
    in     al, dx
    or     al, VGA_SEQ1_ScreenOff       ;0x20
    out    dx, al
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6b83
    or     cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6b6e
    pop    cx
    mov    al, cl
    and    al, 03h
    cmp    al, 00h
    jne    Label0x6ea5                  ;Offset 0x6ea5
    mov    al, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jmp    Label0x6eaf                  ;Offset 0x6eaf
Label0x6ea5:                            ;Offset 0x6ea5
    cmp    al, 01h
    jne    Label0x6ead                  ;Offset 0x6ead
    mov    al, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jmp    Label0x6eaf                  ;Offset 0x6eaf
Label0x6ead:                            ;Offset 0x6ead
    mov    al, MGA_INDD_PIXPLL_M_ValueSetC;0x4c
Label0x6eaf:                            ;Offset 0x6eaf
    mov    ah, cl
    shr    ah, 02h
    cmp    ah, 01h
    jne    Label0x6ebd                  ;Offset 0x6ebd
    mov    ah, MGA_PIXCLKCTRL_SelPLL    ;0x1
    jmp    Label0x6ec8                  ;Offset 0x6ec8
Label0x6ebd:                            ;Offset 0x6ebd
    cmp    ah, 00h
    jne    Label0x6ec6                  ;Offset 0x6ec6
    mov    ah, MGA_PIXCLKCTRL_SelPCI    ;0x0
    jmp    Label0x6ec8                  ;Offset 0x6ec8
Label0x6ec6:                            ;Offset 0x6ec6
    mov    ah, MGA_PIXCLKCTRL_SelVDOCLK ;0x2
Label0x6ec8:                            ;Offset 0x6ec8
    pop    edx
    push   ax
    mov    ch, dh
    and    ch, MGA_PIXPLL_M_MASK        ;0x1f
    mov    cl, al
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    ch, dl
    inc    cl
    call   WriteIndexedRegister         ;Offset 0x6b6e
    shr    edx, 0dh
    mov    ch, dl
    inc    cl
    call   WriteIndexedRegister         ;Offset 0x6b6e
    pop    dx
    push   dx
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jne    Label0x6ef2                  ;Offset 0x6ef2
    mov    cl, VGA_MISC_Clock25p175MHz SHR 2;0x0
    jmp    Label0x6efd                  ;Offset 0x6efd
Label0x6ef2:                            ;Offset 0x6ef2
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jne    Label0x6efb                  ;Offset 0x6efb
    mov    cl, VGA_MISC_Clock28p322MHz SHR 2;0x1
    jmp    Label0x6efd                  ;Offset 0x6efd
Label0x6efb:                            ;Offset 0x6efb
    mov    cl, MGA_MISC_MGAPixelClock SHR 2;0x3
Label0x6efd:                            ;Offset 0x6efd
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    shl    cl, 02h
    and    al, NOT VGA_MISC_ClockSelectMask;0xf3
    or     al, cl
    mov    dx, VGA_MiscellaneousWrite   ;Port 0x3c2
    out    dx, al
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
    xor    dx, dx
Label0x6f14:                            ;Offset 0x6f14
    dec    dx
    je     Label0x6f23                  ;Offset 0x6f23
    mov    cl, MGA_INDD_PIXPLL_Status   ;0x4f
    call   ReadIndexedRegister          ;Offset 0x6b83
    and    cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je     Label0x6f14                  ;Offset 0x6f14
    jmp    Label0x6f4d                  ;Offset 0x6f4d
Label0x6f23:                            ;Offset 0x6f23
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6b83
    and    cl, NOT MGA_PIXCLKCTRL_PixelPLLPowerUp;0xf7
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6b83
    or     cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x5130
Label0x6f4d:                            ;Offset 0x6f4d
    pop    dx
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6b83
    and    cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or     cl, dh
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6b6e
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6b83
    and    cl, NOT MGA_PIXCLKCTRL_ClockDisable;0xfb
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6b6e
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
Func0x6e6b ENDP

Func0x6f82 PROC NEAR                    ;Offset 0x6f82
    push   eax
    push   ebx
    push   ecx
    mov    cl, 06h
    mov    eax, edx
    mov    bx, 0002h
    call   Func0x6bf0                   ;Offset 0x6bf0
    call   Func0x6e6b                   ;Offset 0x6e6b
    pop    ecx
    pop    ebx
    pop    eax
    ret
Func0x6f82 ENDP

Func0x6f9d PROC NEAR                    ;Offset 0x6f9d
    push   eax
    push   ebx
    push   ecx
    mov    cl, byte ptr cs:[Data0x7d13] ;Offset 0x7d13
    cmp    cl, 0ffh
    ;jne    Label0x6fb1                  ;Offset 0x6fb1
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x6fb1:                            ;Offset 0x6fb1
    xor    eax, eax
    mov    al, 0fah
    mul    cl
    or     ax, ax
    ;jne    Label0x6fc1                  ;Offset 0x6fc1
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6257h
Label0x6fc1:                            ;Offset 0x6fc1
    mov    bx, 0002h
    call   Func0x6bf0                   ;Offset 0x6bf0
    mov    cl, 04h
    call   Func0x6e6b                   ;Offset 0x6e6b
    mov    cl, byte ptr cs:[Data0x7d14] ;Offset 0x7d14
    cmp    cl, 0ffh
    ;jne    Label0x6fda                  ;Offset 0x6fda
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x6fda:                            ;Offset 0x6fda
    xor    eax, eax
    mov    al, 0fah
    mul    cl
    or     ax, ax
    ;jne    Label0x6fea                  ;Offset 0x6fea
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6ea2h
Label0x6fea:                            ;Offset 0x6fea
    mov    bx, 0002h
    call   Func0x6bf0                   ;Offset 0x6bf0
    mov    cl, 05h
    call   Func0x6e6b                   ;Offset 0x6e6b
    pop    ecx
    pop    ebx
    pop    eax
    ret    
Func0x6f9d ENDP

Func0x6ffc PROC NEAR                    ;Offset 0x6ffc
    push   eax
    push   ebx
    mov    eax, 0000fde8h
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    je     Label0x7014                  ;Offset 0x7014
    mov    eax, 00011d28h
Label0x7014:                            ;Offset 0x7014
    mov    bx, 0002h
    call   Func0x6bf0                   ;Offset 0x6bf0
    mov    cl, 04h
    call   Func0x6e6b                   ;Offset 0x6e6b
    mov    cl, 05h
    call   Func0x6e6b                   ;Offset 0x6e6b
    pop    ebx
    pop    eax
    ret    
Func0x6ffc ENDP

;Offset 0x7029
Data0x7029              DB 028h, 002h, 00Ch, 006h, 014h, 090h, 001h, 00Ah, 002h, 021h
                        DB 028h, 000h, 050h, 002h, 00Ch, 006h, 000h, 090h, 001h, 00Ah
                        DB 002h, 021h, 028h, 000h, 050h, 002h, 00Ch, 006h, 000h, 05Eh
                        DB 001h, 00Ah, 002h, 021h, 041h, 000h, 050h, 002h, 00Ch, 006h
                        DB 000h, 0E0h, 001h, 00Ah, 002h, 021h, 000h, 000h, 028h, 002h
                        DB 00Ch, 006h, 014h, 05Eh, 001h, 00Ah, 002h, 021h, 041h, 000h

;Offset 0x7065
Data0x7065              DB 028h, 003h, 011h, 014h, 02Ch, 090h, 001h, 003h, 006h, 01Dh
                        DB 0B8h, 000h, 050h, 003h, 011h, 014h, 018h, 090h, 001h, 003h
                        DB 006h, 01Dh, 0B8h, 000h, 050h, 003h, 011h, 014h, 018h, 05Eh
                        DB 001h, 003h, 006h, 01Dh, 0D1h, 000h, 050h, 003h, 011h, 014h
                        DB 018h, 0E0h, 001h, 003h, 006h, 01Dh, 090h, 000h, 028h, 003h
                        DB 011h, 014h, 02Ch, 05Eh, 001h, 003h, 006h, 01Dh, 0D1h, 000h

;Offset 0x70a1
Data0x70a1              DB 028h, 001h, 005h, 011h, 03Ch, 090h, 001h, 002h, 002h, 004h
                        DB 038h, 001h, 050h, 001h, 005h, 011h, 028h, 090h, 001h, 002h
                        DB 002h, 004h, 038h, 001h, 050h, 001h, 005h, 011h, 028h, 05Eh
                        DB 001h, 002h, 002h, 004h, 051h, 001h, 050h, 001h, 005h, 011h
                        DB 028h, 0E0h, 001h, 002h, 002h, 004h, 010h, 001h, 028h, 001h
                        DB 005h, 011h, 03Ch, 05Eh, 001h, 002h, 002h, 004h, 051h, 001h

;Offset 0x70dd
Data0x70dd              DB 0E8h, 0FDh, 000h, 000h, 0F2h, 055h, 000h, 000h

;Offset 0x70e5
Data0x70e5              DB 0A3h, 063h, 071h, 099h, 074h, 005h, 024h, 0F1h
                        DB 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 0AEh, 084h, 057h, 032h, 000h, 0ABh, 0D1h, 0E3h

;Offset 0x70fd
Data0x70fd              DB 0E0h, 0A5h, 001h, 000h, 032h, 056h, 040h, 021h

;Offset 0x7105
Data0x7105              DB 0B2h, 063h, 081h, 098h, 082h, 007h, 003h, 03Eh
                        DB 000h, 040h, 000h, 000h, 000h, 000h, 009h, 060h
                        DB 096h, 027h, 02Bh, 032h, 000h, 095h, 099h, 0E7h
                        
                        DB 0FFh

;Offset 0x711e
Data0x711e              DB 0E8h, 003h, 003h, 0E8h
                        DB 017h, 003h, 000h, 000h
                        DB 000h, 0F2h, 015h, 002h

;Offset 0x712a
Data0x712a              DB 0D0h, 00Fh, 005h, 0D0h, 00Fh, 005h, 0A0h, 01Fh, 00Ah
                        DB 0E4h, 00Bh, 003h, 0E4h, 00Bh, 003h, 0C8h, 017h, 006h

;Offset 0x713c
Data0x713c              DB 0D8h, 01Ch, 007h, 0D8h, 015h, 006h, 000h, 000h, 000h
                        DB 062h, 01Bh, 005h, 000h, 000h, 000h, 0F0h, 00Fh, 003h

;Offset 0x714e
Data0x714e              DB 0B0h, 00Bh, 00Bh, 0B0h, 00Bh, 00Bh, 060h, 017h, 016h
                        DB 0C4h, 016h, 009h, 0C4h, 016h, 009h, 088h, 00Dh, 011h
                        DB 0E0h, 01Fh, 006h, 0E0h, 01Fh, 006h, 0C0h, 01Fh, 00Bh

;Offset 0x7169
Data0x7169              DB 000h, 000h, 00Ch, 00Ch, 000h, 000h, 00Ch, 018h, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 000h, 00Ch, 018h, 018h, 018h
                        DB 018h, 030h, 030h, 018h, 018h, 000h, 00Ch, 00Ch, 024h
                        DB 024h, 00Ch

Func0x7186 PROC NEAR                    ;Offset 0x7186
    push   ds
    push   es
    pushaw
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    lea    si, [Data0x7029]             ;Offset 0x7029
    test   byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    nop
    jne    Label0x71ad                  ;Offset 0x71ad
    lea    si, [Data0x70a1]             ;Offset 0x70a1
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    nop
    jne    Label0x71ad                  ;Offset 0x71ad
    lea    si, [Data0x7065]             ;Offset 0x7065
Label0x71ad:                            ;Offset 0x71ad
    lea    di, [VideoParameters]        ;Offset 0x410
    add    di, 0700h
    mov    cx, 001dh
    push   bp
    sub    sp, 0018h
    mov    bp, sp
Label0x71be:                            ;Offset 0x71be
    push   si
    mov    bx, cx
    dec    bx
    movzx  ax, byte ptr cs:[bx + Data0x7169];Offset 0x7169
    cmp    al, 0ffh
    je     Label0x71dd                  ;Offset 0x71dd
    add    si, ax
    push   di
    mov    di, bp
    call   Func0x7211                   ;Offset 0x7211
    mov    si, bp
    pop    di
    mov    ax, 0000h
    call   Func0x6793                   ;Offset 0x6793
Label0x71dd:                            ;Offset 0x71dd
    sub    di, 0040h
    pop    si
    loop   Label0x71be                  ;Offset 0x71be
    add    sp, 0018h
    pop    bp
    test   byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    nop
    je     Label0x71f5                  ;Offset 0x71f5
    call   Func0x7303                   ;Offset 0x7303
    jmp    Label0x720d                  ;Offset 0x720d
Label0x71f5:                            ;Offset 0x71f5
    call   Func0x728c                   ;Offset 0x728c
    lea    di, [Data0x549b]             ;Offset 0x549b
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    nop
    jne    Label0x7209                  ;Offset 0x7209
    lea    di, [Data0x5493]             ;Offset 0x5493
Label0x7209:                            ;Offset 0x7209
    mov    word ptr [di], 0ffffh
Label0x720d:                            ;Offset 0x720d
    popaw
    pop    es
    pop    ds
    ret
Func0x7186 ENDP

Func0x7211 PROC NEAR                    ;Offset 0x7211
    push   ax
    push   bx
    xor    ax, ax
    xor    bx, bx
    mov    al, byte ptr [si]
    mov    word ptr ss:[di], ax
    mov    bl, byte ptr [si + 04h]
    add    ax, bx
    mov    word ptr ss:[di + 02h], ax
    mov    bl, byte ptr [si + 01h]
    add    ax, bx
    mov    word ptr ss:[di + 04h], ax
    mov    bl, byte ptr [si + 02h]
    add    ax, bx
    mov    word ptr ss:[di + 06h], ax
    mov    bl, byte ptr [si + 03h]
    add    ax, bx
    mov    word ptr ss:[di + 08h], ax
    ;cmp    ax, 0064h
    DB 03Dh, 064h, 000h
    jne    Label0x724b                  ;Offset 0x724b
    mov    word ptr ss:[di + 08h], 0001h
Label0x724b:                            ;Offset 0x724b
    mov    bl, byte ptr [si + 04h]
    add    ax, bx
    mov    word ptr ss:[di + 0ah], ax
    mov    ax, word ptr [si + 05h]
    mov    word ptr ss:[di + 0ch], ax
    mov    bx, word ptr [si + 0ah]
    add    ax, bx
    mov    word ptr ss:[di + 0eh], ax
    movzx  bx, byte ptr [si + 07h]
    add    ax, bx
    mov    word ptr ss:[di + 10h], ax
    mov    bl, byte ptr [si + 08h]
    add    ax, bx
    mov    word ptr ss:[di + 12h], ax
    mov    bl, byte ptr [si + 09h]
    add    ax, bx
    mov    word ptr ss:[di + 14h], ax
    mov    bx, word ptr [si + 0ah]
    add    ax, bx
    mov    word ptr ss:[di + 16h], ax
    pop    bx
    pop    ax
    ret
Func0x7211 ENDP

Func0x728c PROC NEAR                    ;Offset 0x728c
    push      eax
    push      bx
    push      cx
    push      dx
    push      di
    push      si
    lea       si, [Data0x70fd]          ;Offset 0x70fd
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x72a3               ;Offset 0x72a3
    lea       si, [Data0x70dd]          ;Offset 0x70dd
Label0x72a3:                            ;Offset 0x72a3
    lea       di, [Data0x56f1]          ;Offset 0x56f1
    mov       cx, 0004h
Label0x72aa:                            ;Offset 0x72aa
    ;mov       eax, dword ptr cs:[si]
    DB 02Eh, 066h, 08Bh, 004h
    ;mov       dword ptr cs:[di], eax
    DB 02Eh, 066h, 089h, 005h
    mov       ax, word ptr cs:[si + 04h]
    mov       word ptr cs:[di + 0ah], ax
    mov       ax, word ptr cs:[si + 06h]
    mov       word ptr cs:[di + 0ch], ax
    add       di, 000eh
    loop      Label0x72aa               ;Offset 0x72aa
    lea       si, [Data0x7105]          ;Offset 0x7105
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x72d7               ;Offset 0x72d7
    lea       si, [Data0x70e5]          ;Offset 0x70e5
Label0x72d7:                            ;Offset 0x72d7
    lea       di, [Data0x567b]          ;Offset 0x567b
    mov       cx, 0018h
    rep movsb
    mov       eax, 0000fde8h
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x72f4               ;Offset 0x72f4
    mov       eax, 00011d28h
Label0x72f4:                            ;Offset 0x72f4
    lea       di, [Data0x5737]          ;Offset 0x5737
    mov       dword ptr [di], eax
    pop       si
    pop       di
    pop       dx
    pop       cx
    pop       bx
    pop       eax
    ret
Func0x728c ENDP

Func0x7303 PROC NEAR                    ;Offset 0x7303
    cld       
    lea       di, [Data0x547f]          ;Offset 0x547f
    mov       al, byte ptr cs:[Data0x7fef];Offset 0x7fef
    test      al, 01h
    je        Label0x7313               ;Offset 0x7313
    add       di, 0002h
Label0x7313:                            ;Offset 0x7313
    test      al, 04h
    je        Label0x7320               ;Offset 0x7320
    lea       si, [Data0x5481]          ;Offset 0x5481
    mov       cx, 0005h
    rep movsw
Label0x7320:                            ;Offset 0x7320
    test      al, 08h
    je        Label0x732d               ;Offset 0x732d
    lea       si, [Data0x548b]          ;Offset 0x548b
    mov       cx, 0004h
    rep movsw
Label0x732d:                            ;Offset 0x732d
    test      al, 10h
    je        Label0x733a               ;Offset 0x733a
    lea       si, [Data0x5493]          ;Offset 0x5493
    mov       cx, 0004h
    rep movsw
Label0x733a:                            ;Offset 0x733a
    mov       ax, 0ffffh
    stosw
    ret
Func0x7303 ENDP

Func0x733f PROC NEAR                    ;Offset 0x733f
    push      es
    push      bx
    push      si
    push      ecx
    push      edx
    push      edi
    cli
    push      bp
    mov       bp, sp
    sub       sp, 0002h
    push      ss
    pop       es
    mov       di, sp
    mov       ax, di
    shl       edi, 10h
    mov       di, ax
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x7369:                            ;Offset 0x7369
    mov       si, offset MISCFuncs      ;Offset 0x4c09
    mov       edx, 00010000h
    mov       ch, 4eh
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    ;jne       Label0x738e               ;Offset 0x738e
    DB 00Fh, 085h, 011h, 000h
    mov       ch, 4ch
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    ;jne       Label0x738e               ;Offset 0x738e
    DB 00Fh, 085h, 006h, 000h
    dec       cl
    jne       Label0x7369               ;Offset 0x7369
    mov       ch, 00h
Label0x738e:                            ;Offset 0x738e
    mov       byte ptr cs:[Data0x7ff5], ch;Offset 0x7ff5
    mov       sp, bp
    pop       bp
    sti
    pop       edi
    pop       edx
    pop       ecx
    pop       si
    pop       bx
    pop       es
    ret
Func0x733f ENDP

Func0x73a1 PROC NEAR                    ;Offset 0x73a1
    push      es
    push      ds
    push      bx
    push      si
    push      ecx
    push      edx
    push      edi
    cli
    push      bp
    mov       bp, sp
    sub       sp, 0020h
    push      ss
    pop       es
    mov       di, sp
    mov       ax, di
    shl       edi, 10h
    mov       di, ax
    push      0000h
    pop       ds
    push      word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    push      ds
    mov       word ptr ds:[BDA_SoftResetFlag], 0ffefh;Offset 0x472
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x73da:                            ;Offset 0x73da
    push      ss
    pop       ds
    mov       byte ptr ss:[di], 00h
    mov       si, offset Data0x4c15     ;Offset 0x4c15
    test      byte ptr cs:[Data0x7d3b], 40h;Offset 0x7d3b
    jne       Label0x73fa               ;Offset 0x73fa
    mov       si, offset DDCFuncs       ;Offset 0x4bf1
    test      byte ptr cs:[Data0x7d3d], 08h;Offset 0x7d3d
    jne       Label0x73fa               ;Offset 0x73fa
    lea       si, [DDCFuncsNewer]       ;Offset 0x4be5
Label0x73fa:                            ;Offset 0x73fa
    mov       edx, 00010001h
    mov       ch, 0a2h
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x7414               ;Offset 0x7414
    mov       ch, 0a0h
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    ;je        Label0x748a               ;Offset 0x748a
    DB 00Fh, 084h, 076h, 000h
Label0x7414:                            ;Offset 0x7414
    cmp       byte ptr es:[di], 20h
    jne       Label0x7439               ;Offset 0x7439
    mov       byte ptr es:[di], 4fh
    mov       edx, 00010001h
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    je        Label0x748a               ;Offset 0x748a
    cmp       byte ptr es:[di], 10h
    jb        Label0x7490               ;Offset 0x7490
    mov       ax, 8000h
    call      Func0x74ea                ;Offset 0x74ea
    jmp       Label0x745d               ;Offset 0x745d
Label0x7439:                            ;Offset 0x7439
    mov       edx, 00030001h
    mov       byte ptr ss:[di], 12h
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    je        Label0x748a               ;Offset 0x748a
    cmp       byte ptr es:[di], 01h
    jne       Label0x7490               ;Offset 0x7490
    test      byte ptr es:[di + 02h], 80h
    je        Label0x7490               ;Offset 0x7490
    mov       ax, 8000h
    call      Func0x74a7                ;Offset 0x74a7
Label0x745d:                            ;Offset 0x745d
    and       ch, 06h
    shl       ch, 02h
    or        ah, ch
    test      byte ptr cs:[Data0x7fef], 10h;Offset 0x7fef
    jne       Label0x747a               ;Offset 0x747a
    test      byte ptr cs:[Data0x7fef], 07h;Offset 0x7fef
    je        Label0x7493               ;Offset 0x7493
    or        ah, 20h
    jmp       Label0x7493               ;Offset 0x7493
Label0x747a:                            ;Offset 0x747a
    or        ah, 40h
    test      byte ptr cs:[Data0x7fef], 0fh;Offset 0x7fef
    je        Label0x7493               ;Offset 0x7493
    or        ah, 20h
    jmp       Label0x7493               ;Offset 0x7493
Label0x748a:                            ;Offset 0x748a
    dec       cl
    jne       Label0x73da               ;Offset 0x73da
Label0x7490:                            ;Offset 0x7490
    mov       ax, 0000h
Label0x7493:                            ;Offset 0x7493
    pop       ds
    pop       word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    mov       sp, bp
    pop       bp
    sti
    pop       edi
    pop       edx
    pop       ecx
    pop       si
    pop       bx
    pop       ds
    pop       es
    ret
Func0x73a1 ENDP

Func0x74a7 PROC NEAR                    ;Offset 0x74a7
    push      bp
    mov       bp, sp
    sub       sp, 0010h
    mov       di, sp
    shl       edi, 10h
    mov       di, sp
    push      ax
    push      edi
    mov       ah, 23h
    mov       al, 02h
    push      ax
    call      Func0x75bb                ;Offset 0x75bb
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x74e3               ;Offset 0x74e3
    mov       ah, 26h
    mov       al, 08h
    push      ax
    call      Func0x76a3                ;Offset 0x76a3
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x74e3               ;Offset 0x74e3
    mov       ah, 36h
    mov       al, 04h
    push      ax
    call      Func0x7731                ;Offset 0x7731
    pop       ax
Label0x74e3:                            ;Offset 0x74e3
    pop       edi
    pop       ax
    mov       sp, bp
    pop       bp
    ret
Func0x74a7 ENDP

Func0x74ea PROC NEAR                    ;Offset 0x74ea
    push      bp
    mov       bp, sp
    sub       sp, 0004h
    push      ax
    push      ecx
    push      edx
    push      edi
    sub       sp, 0004h
    mov       di, sp
    shl       edi, 10h
    mov       di, sp
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x750b:                            ;Offset 0x750b
    mov       edx, 00030001h
    mov       byte ptr [di], 7eh
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x7522               ;Offset 0x7522
    dec       cl
    jne       Label0x750b               ;Offset 0x750b
    jmp       Label0x75b0               ;Offset 0x75b0
Label0x7522:                            ;Offset 0x7522
    mov       ax, 0000h
    test      byte ptr [di], 20h
    je        Label0x753a               ;Offset 0x753a
    mov       ah, 01h
    mov       al, byte ptr [di + 02h]
    test      al, 80h
    je        Label0x7535               ;Offset 0x7535
    mov       ah, 03h
Label0x7535:                            ;Offset 0x7535
    and       al, 0fh
    mul       ah
    inc       ax
Label0x753a:                            ;Offset 0x753a
    mov       byte ptr [bp - 01h], al
    mov       ax, word ptr [di]
    ;and       ax, 001ch
    DB 025h, 01Ch, 000h
    ;shl       ax, 01h
    DB 0D1h, 0E0h
    add       byte ptr [bp - 01h], al
    mov       ax, word ptr [di]
    ;and       ax, 0003h
    DB  025h, 003h, 000h
    mov       dl, 1bh
    mul       dl
    add       byte ptr [bp - 01h], al
    mov       dl, byte ptr [di + 01h]
    shr       dl, 03h
    mov       byte ptr [bp - 02h], dl
    mov       al, byte ptr [di + 01h]
    and       al, 07h
    mov       byte ptr [bp - 03h], al
    add       sp, 0004h
    xor       dh, dh
    shl       dx, 02h
    cmp       dx, 0006h
    jae       Label0x7574               ;Offset 0x7574
    mov       dx, 0006h
Label0x7574:                            ;Offset 0x7574
    sub       sp, dx
    mov       di, sp
    shl       edi, 10h
    mov       di, sp
    push      dx
    mov       al, byte ptr [bp - 02h]
    or        al, al
    je        Label0x7599               ;Offset 0x7599
    mov       ah, byte ptr [bp - 01h]
    add       ah, 80h
    push      ax
    call      Func0x7628                ;Offset 0x7628
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x75ad               ;Offset 0x75ad
Label0x7599:                            ;Offset 0x7599
    mov       al, byte ptr [bp - 03h]
    or        al, al
    je        Label0x75ad               ;Offset 0x75ad
    mov       dl, byte ptr [bp - 02h]
    shl       dl, 02h
    add       ah, dl
    push      ax
    call      Func0x7731                ;Offset 0x7731
    pop       ax
Label0x75ad:                            ;Offset 0x75ad
    pop       dx
    add       sp, dx
Label0x75b0:                            ;Offset 0x75b0
    pop       edi
    pop       edx
    pop       ecx
    pop       ax
    mov       sp, bp
    pop       bp
    ret       
Func0x74ea ENDP

Func0x75bb PROC NEAR                    ;Offset 0x75bb
    push      bp
    mov       bp, sp
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x75cb:                            ;Offset 0x75cb
    mov       edx, 00020001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x75e3               ;Offset 0x75e3
    dec       cl
    jne       Label0x75cb               ;Offset 0x75cb
    jmp       Label0x7613               ;Offset 0x7613
Label0x75e3:                            ;Offset 0x75e3
    test      word ptr [di], 003ch
    je        Label0x75ef               ;Offset 0x75ef
    or        byte ptr cs:[Data0x7fef], 02h;Offset 0x7fef
Label0x75ef:                            ;Offset 0x75ef
    test      word ptr [di], 0c003h
    je        Label0x75fb               ;Offset 0x75fb
    or        byte ptr cs:[Data0x7fef], 04h;Offset 0x7fef
Label0x75fb:                            ;Offset 0x75fb
    test      word ptr [di], 1e00h
    je        Label0x7607               ;Offset 0x7607
    or        byte ptr cs:[Data0x7fef], 08h;Offset 0x7fef
Label0x7607:                            ;Offset 0x7607
    test      word ptr [di], 0100h
    je        Label0x7613               ;Offset 0x7613
    or        byte ptr cs:[Data0x7fef], 10h;Offset 0x7fef
Label0x7613:                            ;Offset 0x7613
    pop       edx
    pop       ecx
    mov       sp, bp
    pop       bp
    ret
Func0x75bb ENDP

;Offset 0x761b
Data0x761b              DW 08554h
                        DW 07D40h
                        DW 08530h
                        DW 08522h
                        DW 08518h
                        DW 0A018h

;Offset 0x7627
Data0x7627              DB 006h

Func0x7628 PROC NEAR                    ;Offset 0x7628
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x763a:                            ;Offset 0x763a
    xor       dx, dx
    mov       dl, byte ptr [bp + 04h]
    shl       edx, 12h
    mov       dx, 0001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x7658               ;Offset 0x7658
    dec       cl
    jne       Label0x763a               ;Offset 0x763a
    jmp       Label0x768e               ;Offset 0x768e
Label0x7658:                            ;Offset 0x7658
    mov       ch, byte ptr [bp + 04h]
    sub       di, 0004h
Label0x765e:                            ;Offset 0x765e
    dec       ch
    js        Func0x770e                ;Offset 0x770e
    add       di, 0004h
    mov       dl, byte ptr [di]
    mov       dh, byte ptr [di + 02h]
    lea       si, [Data0x761b]          ;Offset 0x761b
    mov       cl, byte ptr cs:[Data0x7627];Offset 0x7627
Label0x7675:                            ;Offset 0x7675
    dec       cl
    js        Label0x765e               ;Offset 0x765e
    cmp       dx, word ptr cs:[si]
    je        Label0x7683               ;Offset 0x7683
    add       si, 0002h
    jmp       Label0x7675               ;Offset 0x7675
Label0x7683:                            ;Offset 0x7683
    mov       al, 01h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
    jmp       Label0x765e               ;Offset 0x765e
Label0x768e:                            ;Offset 0x768e
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x7628 ENDP

;Offset 0x7698
Data0x7698              DW 040A9h
                        DW 08081h
                        DW 04061h
                        DW 04045h
                        DW 04031h

;Offset 0x76a2
Data0x76a2              DB 005h

Func0x76a3 PROC NEAR                    ;Offset 0x76a3
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x76b5:                            ;Offset 0x76b5
    xor       dx, dx
    mov       dl, byte ptr [bp + 04h]
    shl       edx, 11h
    mov       dx, 0001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x76d3               ;Offset 0x76d3
    dec       cl
    jne       Label0x76b5               ;Offset 0x76b5
    jmp       Func0x770e                ;Offset 0x770e
Label0x76d3:                            ;Offset 0x76d3
    mov       ch, byte ptr [bp + 04h]
    sub       di, 0002h
Label0x76d9:                            ;Offset 0x76d9
    dec       ch
    js        Func0x770e                ;Offset 0x770e
    add       di, 0002h
    cmp       word ptr [di], 0101h
    je        Func0x770e                ;Offset 0x770e
    mov       dx, word ptr [di]
    and       dx, 0c0ffh
    lea       si, [Data0x7698]          ;Offset 0x7698
    mov       cl, byte ptr cs:[Data0x76a2];Offset 0x76a2
Label0x76f5:                            ;Offset 0x76f5
    dec       cl
    js        Label0x76d9               ;Offset 0x76d9
    cmp       dx, word ptr cs:[si]
    je        Label0x7703               ;Offset 0x7703
    add       si, 0002h
    jmp       Label0x76f5               ;Offset 0x76f5
Label0x7703:                            ;Offset 0x7703
    mov       al, 02h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
    jmp       Label0x76d9               ;Offset 0x76d9
Func0x76a3 ENDP
;continue!
Func0x770e PROC NEAR                    ;Offset 0x770e
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x770e ENDP

;Offset 0x7718
Data0x7718              DD 0064004B0h
                        DD 005000400h
                        DD 004000300h
                        DD 003200258h
                        DD 0028001E0h
                        DD 002800190h

;Offset 0x7730
Data0x7730              DB 006h

Func0x7731 PROC NEAR                    ;Offset 0x7731
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    and       ecx, 0000ffffh
Label0x7741:                            ;Offset 0x7741
    mov       cl, 04h
Label0x7743:                            ;Offset 0x7743
    mov       al, byte ptr [bp + 05h]
    add       al, 02h
    mov       byte ptr [di], al
    mov       si, word ptr [bp - 04h]
    mov       edx, 00060001h
    call      Func0x52fa                ;Offset 0x52fa
    or        ax, ax
    jne       Label0x7760               ;Offset 0x7760
    dec       cl
    jne       Label0x7743               ;Offset 0x7743
    jmp       Label0x77bb               ;Offset 0x77bb
Label0x7760:                            ;Offset 0x7760
    mov       dh, byte ptr [di + 02h]
    shr       dh, 04h
    mov       dl, byte ptr [di]
    cmp       dx, 0000h
    jne       Label0x7782               ;Offset 0x7782
    cmp       byte ptr [di + 01h], 0fah
    jne       Label0x77b2               ;Offset 0x77b2
    mov       ah, byte ptr [bp + 05h]
    add       ah, 05h
    mov       al, 06h
    push      ax
    call      Func0x76a3                ;Offset 0x76a3
    pop       ax
    jmp       Label0x77b2               ;Offset 0x77b2
Label0x7782:                            ;Offset 0x7782
    shl       edx, 10h
    mov       dh, byte ptr [di + 05h]
    shr       dh, 04h
    mov       dl, byte ptr [di + 03h]
    lea       si, [Data0x7718]          ;Offset 0x7718
    mov       cl, byte ptr cs:[Data0x7730];Offset 0x7730
Label0x7798:                            ;Offset 0x7798
    dec       cl
    js        Label0x77b2               ;Offset 0x77b2
    ;cmp       edx, dword ptr cs:[si]
    DB 02Eh, 066h, 03Bh, 014h
    je        Label0x77a9               ;Offset 0x77a9
    ja        Label0x77b2               ;Offset 0x77b2
    add       si, 0004h
    jmp       Label0x7798               ;Offset 0x7798
Label0x77a9:                            ;Offset 0x77a9
    mov       al, 01h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
Label0x77b2:                            ;Offset 0x77b2
    add       byte ptr [bp + 05h], 12h
    dec       byte ptr [bp + 04h]
    jne       Label0x7741               ;Offset 0x7741
Label0x77bb:                            ;Offset 0x77bb
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x7731 ENDP

RebootMGAG400 PROC NEAR                 ;Offset 0x77c5
    push      di
    push      ecx
    push      edx
    push      bx
    push      ax
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       di, PCI_Header_Word_Command;0x4
LoopPCIRegistersRead:                   ;Offset 0x77d4
    mov       al, PCI_ACCESS_ReadDWord  ;0xa
    call      AccessPCIRegister         ;Offset 0x69da
    cmp       di, PCI_H0_Word_SubsystemVendorID;0x2c
    ;jne       SkipSave                  ;Offset 0x77e3
    DB 00Fh, 085h, 003h, 000h
    mov       edx, ecx                  ;Save Subsystem Vendor ID
SkipSave:                               ;Offset 0x77e3
    push      ecx
    add       di, 0004h
    cmp       di, PCI_MGA_Option3       ;0x54
    jne       LoopPCIRegistersRead      ;Offset 0x77d4
    mov       di, PCI_MGA_AGPCommand    ;0xf8
    mov       al, PCI_ACCESS_ReadDWord  ;0xa
    call      AccessPCIRegister         ;Offset 0x69da
    push      ecx
    mov       di, PCI_MGA_PowerMgmtControlStatus;0xe0
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    mov       cl, PCI_MGA_PWRCTRL_D3    ;0x3
    call      AccessPCIRegister         ;Offset 0x69da
    mov       di, PCI_MGA_PowerMgmtControlStatus;0xe0
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    mov       cl, PCI_MGA_PWRCTRL_D0    ;0x0
    call      AccessPCIRegister         ;Offset 0x69da
    push      dx
    mov       dx, 2ee0h                 ;12,000 ticks
    call      Sleep                     ;Offset 0x5130
    pop       dx
    pop       ecx
    mov       di, PCI_MGA_AGPCommand    ;0xf8
    mov       al, PCI_ACCESS_WriteDWord ;0xd
    call      AccessPCIRegister         ;Offset 0x69da
    mov       di, PCI_MGA_Option2       ;0x50
LoopPCIRegisterWrite:                   ;Offset 0x7820
    mov       al, PCI_ACCESS_WriteDWord ;0xd
    pop       ecx
    cmp       di, PCI_MGA_SubsystemVendorIDWrite;0x4c
    ;jne       SkipStore                 ;Offset 0x782e
    DB 00Fh, 085h, 003h, 000h
    mov       ecx, edx                  ;Store Subsystem Vendor ID
SkipStore:                              ;Offset 0x782e
    call      AccessPCIRegister         ;Offset 0x69da
    sub       di, 0004h
    jne       LoopPCIRegisterWrite      ;Offset 0x7820
    pop       ax
    pop       bx
    pop       edx
    pop       ecx
    pop       di
    ret
RebootMGAG400 ENDP

Func0x783e PROC NEAR                    ;Offset 0x783e
    push   ax
    push   si
    push   di
    call   Func0x790f                   ;Offset 0x790f
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_AGPCommand       ;0xf8
    call   AccessPCIRegister            ;Offset 0x69da
    cmp    cl, PCI_MGA_AGPCMD_RateReset ;0x0
    jne    Label0x785f                  ;Offset 0x785f
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    mov    di, PCI_MGA_AGPCommand       ;0xf8
    mov    ecx, PCI_MGA_AGPCMD_Rate1x OR PCI_MGA_AGPCMD_AGPEnable OR PCI_MGA_AGPCMD_SBAEnable OR PCI_MGA_AGPCMD_RQDepthMask;0x1f000301
    call   AccessPCIRegister            ;Offset 0x69da
Label0x785f:                            ;Offset 0x785f
    mov    si, MGA_MemAddr_ZDepthOrigin ;0x1c0c
    xor    ecx, ecx
    call   IndirectRegisterWriteDWord   ;Offset 0x6ae5
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_Header_Byte_RevisionID;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    cmp    cl, MGA_G400_Rev3            ;0x3
    ja     Label0x7884                  ;Offset 0x7884
    mov    si, MGA_MemAddr_ConfigurationOverride + 2;0x1e4e
    call   IndirectRegisterReadByte     ;Offset 0x6afe
    and    cl, NOT (MGA_MA_CFGOR_RateCapOr_MASK SHR 16);0x1f
    or     cl, 60h
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
Label0x7884:                            ;Offset 0x7884
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_Option + 01h     ;0x41
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, PCI_MGA_Opt_VGAIOMapEnable SHR 8;0x1
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_Header_Byte_RevisionID;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    cmp    cl, MGA_G400_Rev2            ;0x2
    jae    Label0x78ad                  ;Offset 0x78ad
    mov    si, MGA_MemAddr_ConfigurationOverride;0x1e4c
    mov    ecx, MGA_MA_CFGOR_ComporUpDown OR MGA_MA_CFGOR_CompFreq16ms OR (03h SHL 8) OR (03h SHL 12);0x3308
    call   IndirectRegisterWriteWord    ;Offset 0x6ac7
Label0x78ad:                            ;Offset 0x78ad
    call   ConfigureAladdinChipset      ;Offset 0x78b4
    pop    di
    pop    si
    pop    ax
    ret
Func0x783e ENDP

ConfigureAladdinChipset PROC NEAR       ;Offset 0x78b4
    push   bx
    mov    ax, 0b102h                   ;Find PCI Device
    mov    cx, PCI_DEVICE_M1541_Aladdin ;0x1541 Device ID
    mov    dx, PCI_VENDOR_ALI           ;0x10b9 Vendor ID
    mov    si, 0000h                    ;Device Index
    int    1ah
    cmp    ah, 00h
    ;jne    NotFound                     ;Offset 0x7908
    DB 00Fh, 085h, 03Eh, 000h
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M1541_CPUtoPCIWriteBufferOption;0x86
    call   AccessPCIRegister            ;Offset 0x69da
    and    cl, NOT ALI_M1541_CPUPCIWBO_PrgFrameBuffer;0xfe
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    ax, 0b102h                   ;Find PCI Device
    mov    cx, PCI_DEVICE_M5243_PCItoAGP_Controller;0x5243 Device ID
    mov    dx, PCI_VENDOR_ALI           ;0x10b9 Vendor ID
    mov    si, 0000h                    ;Device Index
    int    1ah
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M5243_CPUtoPCI66WriteBufferOption;0x86
    call   AccessPCIRegister            ;Offset 0x69da
    and    cl, NOT ALI_M5243_CPUPCIWBO_VGAFixedFB;0xfe
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M5243_PCI66ToMainMemPCI66ArbiterOpt;0x88
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, ALI_M5243_ForcePCI66GATMode;0x8
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
NotFound:                               ;Offset 0x7908
    pop    bx
    ret
ConfigureAladdinChipset ENDP

;Offset 0x790a
Data0x790a              DB 012h
                        DB 000h
                        DB 013h
                        DB 03Fh
                        DB 060h

Func0x790f PROC NEAR                    ;Offset 0x790f
    push   ds
    push   ecx
    push   edx
    and    ecx, 0000ffffh
    mov    ch, 80h
    lea    di, [Data0x790a]             ;Offset 0x790a
    push   cs
    pop    ds
    lea    si, [MISCFuncs]              ;Offset 0x4c09
    mov    edx, 00000005h
Label0x792d:                            ;Offset 0x792d
    call   Func0x52fa                   ;Offset 0x52fa
    or     ax, ax
    jne    Label0x793e                  ;Offset 0x793e
    cmp    ch, 88h
    je     Label0x793e                  ;Offset 0x793e
    add    ch, 04h
    jmp    Label0x792d                  ;Offset 0x792d
Label0x793e:                            ;Offset 0x793e
    pop    edx
    pop    ecx
    pop    ds
    ret
Func0x790f ENDP

;PCI Configuration Mechanism 2
FindMGAG400Mechanism2 PROC NEAR         ;Offset 0x7944
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
Label0x7958:                            ;Offset 0x7958
    xor    dl, dl
    in     ax, dx
    cmp    ax, PCI_VENDOR_Matrox        ;0x102b
    jne    Label0x797a                  ;Offset 0x797a
    mov    dl, PCI_Header_Word_DeviceID ;0x2
    in     ax, dx
    cmp    ax, PCI_DEVICE_G400AGP       ;0x0525
    jne    Label0x797a                  ;Offset 0x797a
    mov    dl, PCI_Header_Word_Command  ;0x4
    in     al, dx
    and    al, 01h
    je     Label0x797a                  ;Offset 0x797a
    mov    dl, PCI_Header_Byte_SubClassCode;0xa
    in     al, dx
    or     al, al
    jne    Label0x797a                  ;Offset 0x797a
    mov    bh, dh
    jmp    Label0x7997                  ;Offset 0x7997
Label0x797a:                            ;Offset 0x797a
    xor    dl, dl
    inc    dh
    cmp    dh, 0d0h
    jne    Label0x7958                  ;Offset 0x7958
    mov    dh, 0c0h
    inc    bx
    mov    al, bl
    push   dx
    mov    dx, PCI_M2_ForwardRegister   ;Port 0xcfa
    out    dx, al
    pop    dx
    cmp    bx, 0100h
    jne    Label0x7958                  ;Offset 0x7958
    mov    bx, 0ffffh
Label0x7997:                            ;Offset 0x7997
    xor    al, al
    mov    dx, PCI_M2_ConfigSpaceEnable ;Port 0xcf8
    out    dx, al
    pop    dx
    pop    cx
    pop    ax
    ret
FindMGAG400Mechanism2 ENDP

;
;inputs:
;   -
;
;outputs:
;   bx = Bus, Device ID and function ID
;
;destroys:
;   -
;
FindMGAG400Mechanism1 PROC NEAR         ;Offset 0x79a1
    push  eax
    push  ebx
    push  ecx
    push  edx
    pushf 
    cli   
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch                       ;PCI M1 port high byte
LoopBus:                                ;Offset 0x79b5
    mov   cl, 20h                       ;Check 32 devices on this bus
LoopDevices:                            ;Offset 0x79b7
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax                       ;Address for Header field 0
    mov   dl, PCI_M1_ConfigData_lowbyte ;Port 0xcfc
    in    eax, dx                       ;Read Device + Vendor ID's
    cmp   eax, (PCI_DEVICE_G400AGP SHL 16) OR PCI_VENDOR_Matrox;0x525102b
    jne   NextDevice                    ;Offset 0x79f0
    mov   eax, ebx
    mov   al, PCI_Header_Word_Command   ;0x4
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B0_lowbyte;Port 0xcfc
    in    al, dx
    and   al, PCI_Cmd_IOSpace           ;0x1 Check if not IO space
    je    NextDevice                    ;Offset 0x79f0
    mov   eax, ebx
    mov   al, PCI_Header_Byte_RevisionID;0x8
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B2_lowbyte;Port 0xcfe
    in    al, dx                        ;PCI_Header_Byte_SubClassCode
    or    al, al                        ;Check if not 0
    jne   NextDevice                    ;Offset 0x79f0
    shr   ebx, 08h                      ;bx = Bus, Device ID and function ID
    jmp   DeviceFound                   ;Offset 0x7a0f
NextDevice:                             ;Offset 0x79f0
    dec   cl
    je    NextBus                       ;Offset 0x79fd
    add   ebx, 00000800h                ;Next device
    jmp   LoopDevices                   ;Offset 0x79b7
NextBus:                                ;Offset 0x79fd
    and   bx, 07ffh                     ;Drop device index
    add   ebx, 00010000h                ;Next Bus
    dec   ch
    jne   LoopBus                       ;Offset 0x79b5
    mov   bx, 0ffffh                    ;Device not found, set Bus, device ID and function ID to invalid
DeviceFound:                            ;Offset 0x7a0f
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    xor   eax, eax
    out   dx, eax
    mov   ax, bx
    popf
    pop   edx
    pop   ecx
    pop   ebx
    mov   bx, ax
    pop   eax
    ret
FindMGAG400Mechanism1 ENDP

;
;inputs:
;   -
;
;outputs:
;   eax = 0 = not found, eax = device ID, Vendor ID = found
;
;destroys:
;   cx, dx
;
FindIntelTriton430FXChipset PROC NEAR   ;Offset 0x7a24
    push  ebx
    pushf
    cli
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch
    mov   cl, 10h
CheckNextDevice:                        ;Offset 0x7a34
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_lowbyte;Port 0xcfc
    in    eax, dx
    cmp   eax, (PCI_DEVICE_Triton_430FX SHL 16) OR PCI_VENDOR_Intel;Intel Triton 430FX chipset 0x122d8086
    je    FoundDevice                   ;Offset 0x7a57
    dec   cl
    je    SearchDone                    ;Offset 0x7a54
    add   ebx, 00000800h
    jmp   CheckNextDevice               ;Offset 0x7a34
SearchDone:                             ;Offset 0x7a54
    xor   eax, eax
FoundDevice:                            ;Offset 0x7a57
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    xchg  eax, ebx
    out   dx, eax                       ;Select device for changes.
    xchg  eax, ebx
    popf
    pop   ebx
    ret
FindIntelTriton430FXChipset ENDP

;
;outputs:
;   bx = Bus, Device ID and Function ID
;
FindMGAG400 PROC NEAR                   ;Offset 0x7a63
    call  FindMGAG400Mechanism1         ;Offset 0x79a1
    inc   bx
    jne   Found                         ;Offset 0x7a6d
    call  FindMGAG400Mechanism2         ;Offset 0x7944
    inc   bx
Found:                                  ;Offset 0x7a6d
    dec   bx
    ret
FindMGAG400 ENDP

Func0x7a6f PROC NEAR                    ;Offset 0x7a6f
    push   ax
    push   bx
    push   ecx
    push   dx
    push   di
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    ax, (VGA_SEQ1_ScreenOff SHL 8)OR VGA_SEQIdx_ClockingMode;0x2001
    out    dx, ax
    mov    si, MGA_MemAddr_CRTC2Control ;0x3c10
    call   IndirectRegisterReadByte     ;Offset 0x6afe
    and    cl, NOT MGA_MA_CRTC2C_Enable ;0xfe
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    test   byte ptr cs:[Data0x7d3c], 10h;Offset 0x7d3c
    ;jne    Label0x7ab3                  ;Offset 0x7ab3
    DB 00Fh, 085h, 01Ch, 000h
    mov    si, MGA_MemAddr_PlaneWriteMask;0x1c1c
    mov    ecx, 0a5a5a5a5h
    call   IndirectRegisterWriteDWord   ;Offset 0x6ae5
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    or     cl, PCA_MGA_Opt_HWPlaneWriteMaskOn SHR 8;0x40
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
Label0x7ab3:                            ;Offset 0x7ab3
    mov    si, MGA_MemAddr_MemoryControlWaitState;0x1c08
    ;mov    ecx, dword ptr cs:[MemoryControlWaitState];Offset 0x7d1d
    DB 02Eh, 066h, 08Bh, 00Eh, 01Dh, 07Dh
    call   IndirectRegisterWriteDWord   ;Offset 0x6ae5
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x69da
    mov    dl, byte ptr cs:[Data0x7d15] ;Offset 0x7d15
    mov    ch, dl
    and    ch, 38h
    shr    ch, 01h
    or     cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x69da
    mov    si, MGA_MemAddr_MemoryReadBack + 03h;0x1e47
    mov    cl, byte ptr cs:[Data0x7d37] ;Offset 0x7d37
    and    cl, 0f0h
    shr    cl, 03h
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    mov    si, MGA_MemAddr_MemoryReadBack + 02h;0x1e46
    mov    cl, byte ptr cs:[Data0x7d37] ;Offset 0x7d37
    and    cl, 03h
    ror    cl, 02h
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x69da
    and    ecx, NOT PCI_MGA_Opt_RefreshCounter_MASK;0xffe07fff
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x69da
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2c06
    mov    si, MGA_MemAddr_MemoryAccess + 01h;0x1c05
    mov    cl, MGA_MA_MA_MemoryActive SHR 8;0x0
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    mov    cl, MGA_MA_MA_MemoryReset SHR 8;0x80
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2c06
    mov    si, MGA_MemAddr_MemoryReadBack;0x1e44
    xor    cx, cx
    mov    dl, byte ptr cs:[MemoryClockBaseReadDelayDefault];Offset 0x7d36
    mov    cl, dl
    and    cl, 0f0h
    shl    cx, 01h
    and    dl, 0fh
    or     cl, dl
    call   IndirectRegisterWriteWord    ;Offset 0x6ac7
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x69da
    and    ecx, NOT PCI_MGA_Opt_RefreshCounter_MASK;0xffe07fff
    or     ecx, 00040000h               ;Refresh Counter
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x69da
    mov    si, MGA_MemAddr_Status + 03h ;0x1e17
    mov    cl, 80h                      ;Software flag...
    call   IndirectRegisterWriteByte    ;Offset 0x6aa9
    pop    di
    pop    dx
    pop    ecx
    pop    bx
    pop    ax
    ret
Func0x7a6f ENDP

Func0x7b6a PROC NEAR                    ;Offset 0x7b6a
    push  ax
    push  bx
    push  ecx
    push  edx
    push  di
    push  cx
    mov   bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call  FindIntelTriton430FXChipset   ;Offset 0x7a24
    or    ax, ax
    je    NoIntelTriton430FX            ;Offset 0x7b8e
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_MGA_Option + 03h      ;0x43
    call  AccessPCIRegister             ;Offset 0x69da
    or    cl, (PCI_MGA_Opt_NoRetry SHR 24);0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x69da
NoIntelTriton430FX:                     ;Offset 0x7b8e
    mov   cl, MGA_INDD_VoltageReferenceControl;0x18
    mov   ch, MGA_VREFCTRL_SysPLLVoltRefBlkPwrUp OR MGA_VREFCTRL_SysPLLVoltRefPLLRefBlk;0x03
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   al, 0fh
    call  Sleep2                        ;Offset 0x2c06
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x69da
    or    cl, PCI_MGA_Opt_SysPLLPowerUp ;0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x69da
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2c06
    xor   dx, dx
PLLNotFrequencyLocked:                  ;Offset 0x7bb1
    dec   dx
    je    PLLFreqLockTimeout            ;Offset 0x7bbe
    mov   cl, MGA_INDD_SYSPLL_Status    ;0x2f
    call  ReadIndexedRegister           ;Offset 0x6b83
    and   cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je    PLLNotFrequencyLocked         ;Offset 0x7bb1
PLLFreqLockTimeout:                     ;Offset 0x7bbe
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6b83
    or    cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2c06
    xor   dx, dx
PIXNotFrequencyLocked:                  ;Offset 0x7bd4
    dec   dx
    je    PIXFreqLockTimeout            ;Offset 0x7be1
    mov   cl, MGA_INDD_PIXPLL_Status    ;0x4f
    call  ReadIndexedRegister           ;Offset 0x6b83
    and   cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je    PIXNotFrequencyLocked         ;Offset 0x7bd4
PIXFreqLockTimeout:                     ;Offset 0x7be1
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x6b83
    or    cl, MGA_MISCCTRL_LUT_Enable   ;0x10
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x6b83
    or    cl, MGA_MISCCTRL_DAC_Enable   ;0x1
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x69da
    or    cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x69da
    mov   di, PCI_MGA_Option3           ;0x54
    mov   al, PCI_ACCESS_ReadDWord      ;0xa
    call  AccessPCIRegister             ;Offset 0x69da
    and   ecx, NOT (PCI_MGA_OPTION3_GfxClkSelMASK OR PCI_MGA_OPTION3_MemClkSelMASK OR PCI_MGA_OPTION3_WrpClkSelMASK);0xffcff3fc
    or    ecx, PCI_MGA_OPTION3_GfxClkSelPLL OR PCI_MGA_OPTION3_MemClkSelPLL OR PCI_MGA_OPTION3_WrpClkSelPLL;0x100401
    mov   al, PCI_ACCESS_WriteDWord     ;0xd
    call  AccessPCIRegister             ;Offset 0x69da
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x69da
    and   cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x69da
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6b83
    or    cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6b83
    and   cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or    cl, MGA_PIXCLKCTRL_SelPLL     ;0x1
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6b83
    and   cl, NOT MGA_PIXCLKCTRL_ClockDisable;0xfb
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6b6e
    pop   dx
    call  Func0x6ba6                    ;Offset 0x6ba6
    mov   ax, dx
    xor   dx, dx
    mov   bx, 03e8h
    mul   bx
    shl   edx, 10h
    mov   dx, ax
    call  Func0x6d8c                    ;Offset 0x6d8c
    pop   di
    pop   edx
    pop   ecx
    pop   bx
    pop   ax
    ret
Func0x7b6a ENDP

CacheMemorySize PROC NEAR               ;Offset 0x7c88
    call  GetMemorySize                 ;Offset 0x7c97
    shr   ax, 04h
    mov   byte ptr cs:[MemorySizeInMB], al;Offset 0x7ff0
    ret
CacheMemorySize ENDP

;Offset 0x7c93
MemorySizesInMB DB 004h, 008h, 010h, 020h

;inputs:
;   -
;
;outputs:
;   ax = Size of memory in 64KB blocks
;
GetMemorySize PROC NEAR                 ;Offset 0x7c97
    push  bx
    xor   ax, ax
    mov   al, byte ptr cs:[Data0x7d3c]  ;Offset 0x7d3c
    and   al, 0ch
    shr   al, 02h
    movzx bx, al
    mov   al, byte ptr cs:[bx + MemorySizesInMB];Offset 0x7c93
    shl   ax, 04h
    pop   bx
    ret
GetMemorySize ENDP

Func0x7cb0 PROC NEAR                    ;Offset 0x7cb0
    push  si
    push  di
    lea   si, [Data0x4bfd]              ;Offset 0x4bfd
    call  word ptr cs:[si + 08h]
    call  word ptr cs:[si + 0ah]
    jae   Label0x7cd6                   ;Ofsfet 0x7cd6
    call  word ptr cs:[si + 04h]
    call  word ptr cs:[si + 0ah]
    rcl   di, 01h
    call  word ptr cs:[si + 08h]
    clc
    test  di, 01h
    jne   Label0x7cd6                   ;Offset 0x7cd6
    stc
Label0x7cd6:                            ;Offset 0x7cd6
    pop   di
    pop   si
    ret
Func0x7cb0 ENDP

    xchg  bx, bx                        ;hello
    xchg  bx, bx
    xchg  bx, bx
    nop   

;Offset 0x7ce0
PINS                    DB 02Eh, 041h
                        DB 080h ;Offset from 0x7ce0 to the protected mode table: 0x7ce0 + 0x80 = 0x7d60
                        DB 0FFh, 000h, 004h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FFh, 0FFh, 0EFh, 0FFh

;Offset 0x7d06
Data0x7d06              DB 0FFh

;Offset 0x7d07
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
                        DB 000h, 000h, 0FFh

;Offset 0x7d13
Data0x7d13              DB 0FFh

;Offset 0x7d14
Data0x7d14              DB 0FFh

;Offset 0x7d15
Data0x7d15              DB 002h     ;bit 0 = ?
                                    ;bit 1 =  vgaioen
                                    ;bit 2 = reserved
                                    ;bit 3 = lower bit of memconfig?

;Offset 0x7d16
                        DB 0FFh

;Offset 0x7d17
Data0x7d17              DB 085h

;Offset 0x7d18
Data0x7d18              DB 085h

;Offset 0x7d19  OPTION3
Option3Default          DD PCI_MGA_OPTION3_GfxClkSelPLL OR PCI_MGA_OPTION3_GfxClkMul1d2 OR PCI_MGA_OPTION3_GfxClkDCCorNone OR PCI_MGA_OPTION3_MemClkSelPLL OR PCI_MGA_OPTION3_MemClkMul2d3 OR PCI_MGA_OPTION3_MemClkDCCor7p75t8p25 OR PCI_MGA_OPTION3_WrpClkSelPLL OR PCI_MGA_OPTION3_WrpClkMul1d2 OR PCI_MGA_OPTION3_WrpClkDCCorNone;0x019B8419

;Offset 0x7d1d
MemoryControlWaitState  DD 042450A1h

;Offset 0x7d21
                        DB 0FFh, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x7d36
MemoryClockBaseReadDelayDefault DB 088h ;bit 3-0 = mclkbrd0 in MEMRDBK
                                        ;bit 7-4 = mclkbrd1 in MEMRDBK

;Offset 0x7d37
Data0x7d37              DB 000h         ;bit 1-0 = strmfctl in MEMRDBK
                                        ;bit 7-4 = mrsopcod(lower 4 bits) in MEMRDBK

;Offset 0x7d38
                        DB 088h, 000h


;Offset 0x7d3a
Data0x7d3a              DB 0EEh

;Offset 0x7d3b
Data0x7d3b              DB 0FFh

;Offset 0x7d3c
Data0x7d3c              DB 0FFh         ;bit 3-2 = memory size. 0 = 4Mb, 1 = 8MB, 2 = 16MB, 3 = 32MB

;Offset 0x7d3d
Data0x7d3d              DB 0FFh

;Offset 0x7d3e
                        DB 0FFh
                        DB 0FFh

;Offset 0x7d40
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x7d60
                        DW 0005fh       ;00h    WORD    offset (within table) of protected-mode code for Function 5 (Set Window) : Points to 0x7dbf
                        DW 00092h       ;02h    WORD    offset of protected-mode code for Function 7 (Set Disp Start) : Points to 0x7df2
                        DW 000feh       ;04h    WORD    offset of protected-mode code for Function 9 (Set Primary Palette) : Points to 0x7e5e
                        DW 0000ch       ;06h    WORD    offset (within table) of list of I/O ports and memory locations
                                        ;               for which I/O privilege may be required in protected mode (0000h if no list) : Points to 0x7d6c
                                        ;08h    var     code and optional port/memory list
                        DW 0020ch       ;Size
                        DW 00000h       ;?

;Offset 0x7d6c
                        DW 003DEh       ;Port list
                        DW 003DFh
                        DW 0FFFFh
                        DW 0FFFFh       ;Memory list

;Offset 0x7d74
                        DB 0E8h, 003h, 003h, 0E8h, 017h, 003h, 000h, 000h, 000h, 0F2h, 015h, 002h
                        DB 0D0h, 00Fh, 005h, 0D0h, 00Fh, 005h, 0A0h, 01Fh, 00Ah, 0E4h, 00Bh, 003h, 0E4h, 00Bh, 003h, 0C8h
                        DB 017h, 006h, 0D8h, 01Ch, 007h, 0D8h, 015h, 006h, 000h, 000h, 000h, 062h, 01Bh, 005h, 000h, 000h
                        DB 000h, 0F0h, 00Fh, 003h, 0B0h, 00Bh, 00Bh, 0B0h, 00Bh, 00Bh, 060h, 017h, 016h, 0C4h, 016h, 009h
                        DB 0C4h, 016h, 009h, 088h, 00Dh, 011h, 0E0h, 01Fh, 006h, 0E0h, 01Fh, 006h, 0C0h, 01Fh, 00Bh

;Can't have 16-bit tiny code and 32-bit flat code in the same file, so here is the data with code as comment:

                                        ;SetWindow32bit PROC NEAR                ;Offset 0x7dbf
DB 066h, 052h                           ;    push dx
DB 051h                                 ;    push ecx
DB 0E8h, 017h, 001h, 000h, 000h         ;    call Func32bit0x7ede                ;Offset 0x7ede
DB 00Ah, 0FFh                           ;    or   bh, bh
DB 075h, 00Fh                           ;    jne  Label0x7dda                    ;Offset 0x7dda
DB 08Ah, 0E2h                           ;    mov  ah, dl
DB 002h, 0E1h                           ;    add  ah, cl
DB 059h                                 ;    pop  ecx
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 066h, 0EFh                           ;    out  dx, ax
DB 0EBh, 011h                           ;    jmp  Label0x7deb                    ;Offset 0x7deb
                                        ;Label0x7dda:                            ;Offset 0x7dda
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 02Ah, 0C1h                           ;    sub  al, cl
DB 059h                                 ;    pop  ecx
DB 08Ah, 0D0h                           ;    mov  dl, al
DB 032h, 0F6h                           ;    xor  dh, dh
                                        ;Label0x7deb:                            ;Offset 0x7deb
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;SetWindow32bit ENDP

                                        ;SetDisplayStart32bit PROC NEAR          ;Offset 0x7df2
DB 050h                                 ;    push eax
DB 066h, 051h                           ;    push cx
DB 066h, 052h                           ;    push dx
DB 066h, 08Bh, 0C2h                     ;    mov  ax, dx
DB 0C1h, 0C0h, 010h                     ;    rol  eax, 10h
DB 066h, 08Bh, 0C1h                     ;    mov  ax, cx
DB 0D1h, 0E8h                           ;    shr  eax, 01h
DB 051h                                 ;    push ecx
DB 0E8h, 0D6h, 000h, 000h, 000h         ;    call Func32bit0x7ede                ;Offset 0x7ede
DB 0C1h, 0E9h, 008h                     ;    shr  ecx, 08h
DB 003h, 0C1h                           ;    add  eax, ecx
DB 059h                                 ;    pop  ecx
DB 050h                                 ;    push eax
DB 0F6h, 0C3h, 080h                     ;    test bl, 80h
DB 00Fh, 084h, 010h, 000h, 000h, 000h   ;    je   Label0x7e28                    ;Offset 0x7e28
DB 066h, 0BAh, 0DAh, 003h               ;    mov  dx, VGA_InputStatus1D          ;Port 0x3da
DB 066h, 033h, 0C9h                     ;    xor  cx, cx
                                        ;Label0x7e1f:                            ;Offset 0x7e1f
DB 066h, 049h                           ;    dec  cx
DB 074h, 005h                           ;    je   Label0x7e28                    ;Offset 0x7e28
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0F7h                           ;    je   Label0x7e1f                    ;Offset 0x7e1f
                                        ;Label0x7e28:                            ;Offset 0x7e28
DB 066h, 0BAh, 0D4h, 003h               ;    mov  dx, VGA_CRTControllerIndexD    ;Port 0x3d4
DB 0B0h, 00Dh                           ;    mov  al, VGA_CRTCIdx_StartAddrLow   ;0xd
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 066h, 058h                           ;    pop  ax
DB 0EEh                                 ;    out  dx, al
DB 066h, 04Ah                           ;    dec  dx
DB 0B0h, 00Ch                           ;    mov  al, VGA_CRTCIdx_StartAddrHigh  ;0xc
DB 066h, 0EFh                           ;    out  dx, ax
DB 0B2h, 0DEh                           ;    mov  dl, MGA_CRTCExtensionIndex_lowbyte;Port 0x3de
DB 032h, 0C0h                           ;    xor  al, al
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 066h, 058h                           ;    pop  ax
DB 08Ah, 0E0h                           ;    mov  ah, al
DB 08Ah, 0E8h                           ;    mov  ch, al
DB 080h, 0E4h, 00Fh                     ;    and  ah, 0fh
DB 080h, 0E5h, 010h                     ;    and  ch, 10h
DB 0C0h, 0E5h, 002h                     ;    shl  ch, 02h
DB 0ECh                                 ;    in   al, dx
DB 024h, 0B0h                           ;    and  al, 0b0h
DB 00Ah, 0C4h                           ;    or   al, ah
DB 00Ah, 0C5h                           ;    or   al, ch
DB 0EEh                                 ;    out  dx, al
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 059h                           ;    pop  cx
DB 058h                                 ;    pop  eax
DB 0C3h                                 ;    ret
                                        ;SetDisplayStart32bit ENDP

                                        ;SetPrimaryPalette32bit PROC NEAR        ;Offset 0x7e5e
DB 080h, 0FBh, 002h                     ;    cmp  bl, 02h
DB 074h, 076h                           ;    je   Label0x7ed9                    ;Offset 0x7ed9
DB 080h, 0FBh, 003h                     ;    cmp  bl, 03h
DB 074h, 071h                           ;    je   Label0x7ed9                    ;Offset 0x7ed9
DB 066h, 053h                           ;    push bx
DB 066h, 051h                           ;    push cx
DB 066h, 052h                           ;    push dx
DB 057h                                 ;    push edi
DB 08Ah, 0C2h                           ;    mov  al, dl
DB 066h, 0BAh, 0C8h, 003h               ;    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
DB 0EEh                                 ;    out  dx, al
DB 066h, 051h                           ;    push cx
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 03Ch, 080h                           ;    cmp  al, 80h
DB 075h, 017h                           ;    jne  Label0x7e95                    ;Offset 0x7e95
DB 0B2h, 0DAh                           ;    mov  dl, VGA_InputStatus1D_lowbyte  ;Port 0x3da
DB 066h, 033h, 0C9h                     ;    xor  cx, cx
                                        ;Label0x7e83:                            ;Offset 0x7e83
DB 066h, 049h                           ;    dec  cx
DB 074h, 00Eh                           ;    je   Label0x7e95                    ;Offset 0x7e95
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0F7h                           ;    jne  Label0x7e83                    ;Offset 0x7e83
                                        ;Label0x7e8c:                            ;Offset 0x7e8c
DB 066h, 049h                           ;    dec  cx
DB 074h, 005h                           ;    je   Label0x7e95                    ;Offset 0x7e95
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0F7h                           ;    je   Label0x7e8c                    ;Offset 0x7e8c
                                        ;Label0x7e95:                            ;Offset 0x7e95
DB 066h, 059h                           ;    pop  cx
DB 0B2h, 0C9h                           ;    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 024h, 001h                           ;    and  al, 01h
DB 074h, 018h                           ;    je   Label0x7eb7                    ;Offset 0x7eb7
                                        ;Label0x7e9f:                            ;Offset 0x7e9f
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
DB 075h, 0EAh                           ;    jne  Label0x7e9f                    ;Offset 0x7e9f
DB 0EBh, 016h                           ;    jmp  Label0x7ecd                    ;Offset 0x7ecd
                                        ;Label0x7eb7:                            ;Offset 0x7eb7
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
DB 075h, 0EAh                           ;    jne  Label0x7eb7                    ;Offset 0x7eb7
                                        ;Label0x7ecd:                            ;Offset 0x7ecd
DB 05Fh                                 ;    pop  edi
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 059h                           ;    pop  cx
DB 066h, 05Bh                           ;    pop  bx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;Label0x7ed9:                            ;Offset 0x7ed9
DB 066h, 0B8h, 04Fh, 002h               ;    mov  ax, 024fh
DB 0C3h                                 ;    ret
                                        ;SetPrimaryPalette32bit ENDP

                                        ;Func32bit0x7ede PROC NEAR               ;Offset 0x7ede
DB 050h                                 ;    push eax
DB 057h                                 ;    push edi
DB 056h                                 ;    push esi
DB 066h, 052h                           ;    push dx
DB 033h, 0C0h                           ;    xor  eax, eax
DB 033h, 0C9h                           ;    xor  ecx, ecx
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 005h                           ;    mov  al, MGA_CRTCExt_HorVidHalfCount;0x5
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 08Ah, 0E0h                           ;    mov  ah, al
DB 0A8h, 080h                           ;    test al, 80h
DB 074h, 06Bh                           ;    je   Label0x7f62                    ;Offset 0x7f62
DB 024h, 03Fh                           ;    and  al, 3fh
DB 03Ch, 020h                           ;    cmp  al, 20h
DB 072h, 065h                           ;    jb   Label0x7f62                    ;Offset 0x7f62
DB 080h, 0E4h, 040h                     ;    and  ah, 40h
DB 075h, 020h                           ;    jne  Label0x7f22                    ;Offset 0x7f22
DB 03Ch, 023h                           ;    cmp  al, 23h
DB 077h, 00Ah                           ;    ja   Label0x7f10                    ;Offset 0x7f10
DB 0BEh, 014h, 000h, 000h, 000h         ;    mov  esi, 00000014h
DB 0EBh, 030h                           ;    jmp  Label0x7f3d                    ;Offset 0x7f3d
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7f10:                            ;Offset 0x7f10
DB 03Ch, 030h                           ;    cmp  al, 30h
DB 072h, 04Eh                           ;    jb   Label0x7f62                    ;Offset 0x7f62
DB 03Ch, 035h                           ;    cmp  al, 35h
DB 077h, 04Ah                           ;    ja   Label0x7f62                    ;Offset 0x7f62
DB 0BEh, 020h, 000h, 000h, 000h         ;    mov  esi, 00000020h
DB 0EBh, 01Eh                           ;    jmp  Label0x7f3d                    ;Offset 0x7f3d
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7f22:                            ;Offset 0x7f22
DB 03Ch, 025h                           ;    cmp  al, 25h
DB 077h, 00Ah                           ;    ja   Label0x7f30                    ;Offset 0x7f30
DB 0BEh, 032h, 000h, 000h, 000h         ;    mov  esi, 00000032h
DB 0EBh, 010h                           ;    jmp  Label0x7f3d                    ;Offset 0x7f3d
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7f30:                            ;Offset 0x7f30
DB 03Ch, 030h                           ;    cmp  al, 30h
DB 072h, 02Eh                           ;    jb   Label0x7f62                    ;Offset 0x7f62
DB 03Ch, 038h                           ;    cmp  al, 38h
DB 077h, 02Ah                           ;    ja   Label0x7f62                    ;Offset 0x7f62
DB 0BEh, 044h, 000h, 000h, 000h         ;    mov  esi, 00000044h
                                        ;Label0x7f3d:                            ;Offset 0x7f3d
DB 0E8h, 026h, 000h, 000h, 000h         ;    call Func32bit0x7f68                ;Offset 0x7f68
DB 0BFh, 0E2h, 001h, 000h, 000h         ;    mov  edi, 000001e2h
DB 02Bh, 0CFh                           ;    sub  ecx, edi
DB 003h, 0CEh                           ;    add  ecx, esi
DB 08Bh, 0F1h                           ;    mov  esi, ecx
DB 024h, 00Fh                           ;    and  al, 0fh
DB 0B1h, 003h                           ;    mov  cl, 03h
DB 0F6h, 0E1h                           ;    mul  cl
DB 003h, 0F0h                           ;    add  esi, eax
DB 033h, 0C9h                           ;    xor  ecx, ecx
DB 02Eh, 066h, 08Bh, 00Eh               ;    mov  cx, word ptr cs:[esi]
DB 0C1h, 0E1h, 008h                     ;    shl  ecx, 08h
DB 02Eh, 08Ah, 04Eh, 002h               ;    mov  cl, byte ptr cs:[esi + 02h]
                                        ;Label0x7f62:                            ;Offset 0x7f62
DB 066h, 05Ah                           ;    pop  dx
DB 05Eh                                 ;    pop  esi
DB 05Fh                                 ;    pop  edi
DB 058h                                 ;    pop  eax
DB 0C3h                                 ;    ret
                                        ;Func32bit0x7ede ENDP

                                        ;Func32bit0x7f68 PROC NEAR               ;Offset 0x7f68
DB 08Bh, 00Ch, 024h                     ;    mov  ecx, dword ptr [esp]
DB 0C3h                                 ;    ret
                                        ;Func32bit0x7f68 ENDP

;Offset 0x7f6c
DB 000h

;Offset 0x7f6d
DB 82h dup (0FFh)

;Offset 0x7fef
Data0x7fef              DB 000h

;Offset 0x7ff0
MemorySizeInMB          DB 000h

;Offset 0x7ff1
Data0x7ff1              DB 000h

;Offset 0x7ff2
PCIBusDeviceIDFunctionID DW 00000h

;Offset 0x7ff4
DeviceRevision          DB 000h

;Offset 0x7ff5
Data0x7ff5              DB 000h

;Offset 0x7ff6
DB 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x7ffc
PINSPointer             DW offset PINS  ;Offset 0x7ce0

DB 0FFh

DB 0BBh
ROMEnd:
END
