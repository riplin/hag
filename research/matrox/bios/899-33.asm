;Matrox G200 PCI BIOS v3.3 b30 disassembly

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
;Matrox G200 Specification: https://bitsavers.computerhistory.org/components/matrox/_dataSheets/MGA-G200_199811.pdf
;
;ALI M1541 Specification: https://bitsavers.org/components/ali/M1541_M1542_Aladdin_V_Socket_7_North_Bridge.pdf
;
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT 899-33.asm
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
include g200.inc

;Offset in calltables. Not sure on the names yet.
MGA_DDCPin0Send1                        EQU 00h;0x00
MGA_DDCPin1Send1                        EQU 02h;0x02
MGA_DDCPin0Send0                        EQU 04h;0x04
MGA_DDCPin1Send0                        EQU 06h;0x06
MGA_DDCPin0Read                         EQU 08h;0x08
MGA_DDCPin1Read                         EQU 0ah;0x0a

PCI_VENDOR_Intel                        EQU 8086h;0x8086
PCI_DEVICE_Triton_430FX                 EQU 122dh;0x122d
PCI_VENDOR_ALI                          EQU 10b9h;0x10b9
PCI_DEVICE_M1541_Aladdin                EQU 1541h;0x1541
PCI_DEVICE_M5243_PCItoAGP_Controller    EQU 5243h;0x5243
PCI_VENDOR_Matrox                       EQU 102bh;0x102b
PCI_DEVICE_G200PCI                      EQU 0520h;0x0520

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

MGA_G200_Rev3                           EQU 03h;0x03

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
                        DB 033h         ;BIOS Version (3.3)
                        DB 0E2h, 002h, 0C0h, 003h, 000h, 00Bh, 0DEh, 030h, 03Ah, 00Bh
                        DB 044h, 00Bh, 0FFh, 0FFh, 000h, 000h, 000h, 000h

;Offset 0x18
                        DW offset PCIHeader
                        DW 0000h        ;PnP header pointer
                        DW 2000h        ;Unknown
                        DB 'IBM COMPATIBLE MATROX/MGA-G200 VGA/VBE BIOS (V3.3) b30 ', 000h

;Offset 0x56
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

;Offset 0x60
PCIHeader               DB 'PCIR'       ;PCI header magic
                        DW PCI_VENDOR_Matrox;Vendor ID
                        DW PCI_DEVICE_G200PCI;Device ID
                        DW 0000h        ;Pointer to vital product data (0 = none)
                        DW 0018h        ;PCI data structure length
                        DB 00h          ;PCI data structure revision
                        DB 00h, 00h, 03h;PCI device class code
                        DW (ROMEnd - ROMStart)/512;Rom size in 512 byte blocks
                        DW 1e33h        ;Revision level of code
                        DB 00h          ;Code type (0 = x86)
                        DB 80h          ;Last image indicator
                        DW 0000h        ;Reserved
                        DB '899-33', 000h
                        DB 0FFh

entry:                                  ;Offset 0x80
    push      bp
    xor       ax, ax
    mov       ds, ax
    mov       es, ax
    call      FindMGAG200               ;Offset 0x79ea
    mov       word ptr cs:[PCIBusDeviceIDFunctionID], bx;Offset 0x7ff2 Store Bus, Device ID and Function ID
    call      RebootMGAG200             ;Offset 0x77b5
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call      TalkToMonitorAndConfigureAladdinChipset;Offset 0x782e
    call      TurnScreenOff             ;Offset 0x2b15
    call      Func0x7325                ;Offset 0x7325
    cmp       byte ptr cs:[Data0x7ff5], 00h;Offset 0x7ff5
    je        Label0xc1                 ;Offset 0xc1
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    jne       Label0xbe                 ;Offset 0xbe
    call      Func0x7387                ;Offset 0x7387
    or        byte ptr cs:[Data0x7ff1], ah;Offset 0x7ff1
    test      ah, 80h
    nop
    je        Label0xc1                 ;Offset 0xc1
Label0xbe:                              ;Offset 0xbe
    call      Func0x716c                ;Offset 0x716c
Label0xc1:                              ;Offset 0xc1
    test      byte ptr cs:[Data0x7ff1], 81h;Offset 0x7ff1
    je        Label0xd0                 ;Offset 0xd0
    mov       word ptr cs:[Data0x53f6], 0ffffh;Offset 0x53f6
Label0xd0:                              ;Offset 0xd0
    test      byte ptr cs:[Data0x7d14], 08h;Offset 0x7d14
    je        Label0xeb                 ;Offset 0xeb
    mov       di, PCI_MGA_Option        ;0x40
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x68f8
    or        cl, PCI_MGA_Opt_PLLSelP1SysP2Pix;0x40
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    nop
    call      AccessPCIRegister         ;Offset 0x68f8
Label0xeb:                              ;Offset 0xeb
    mov       ax, MGA_CRTCExt_PrioRequestControl OR ((MGA_CRTCEXT6_MaxPrioLevel6 OR MGA_CRTCEXT6_HiPrioLevel1) SHL 8);0x5006
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    out       dx, ax
    mov       cx, 0078h
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x7b13
    call      SetMemoryTimingsAndRefresh ;Offset 0x79f6
    push      bx
    mov       al, 40h                   ;Color
    call      SetupInterruptsTextModeAndBDA;Offset 0x158
    pop       bx
    call      CacheMemorySize           ;Offset 0x7c33
    xor       cx, cx
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x7b13
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x11f                ;Offset 0x11f
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x11f                ;Offset 0x11f
    call      ConfigureAndSelectPLLSetsAB65Or73;Offset 0x6fe2
    jmp       Label0x122                ;Offset 0x122
Label0x11f:                             ;Offset 0x11f
    call      ConfigureAndSelectPLLSetsAB;Offset 0x6f83
Label0x122:                             ;Offset 0x122
    cld
    mov       dx, VGA_CRTControllerIndexD;Port 0x3d4
    call      Func0x26e5                ;Offset 0x26e5
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       di, 2000h
    mov       cx, 0c000h
    xor       ax, ax
    rep stosb                           ;clear off-screen video memory
    mov       es, ax
    call      Func0x26eb                ;Offset 0x26eb
    mov       ax, BDA_DM_80x25_16_Color_Text;0x3 Set Text Mode
    int       10h
    xor       ax, ax
    xor       si, si
    mov       cx, 7fffh                 ;Rom size - 1
ReadROMByteLoop:                        ;Offset 0x149
    add       al, byte ptr cs:[si]      ;Recalculate the check byte at end of ROM
    inc       si
    loop      ReadROMByteLoop           ;Offset 0x149
    neg       al
    mov       byte ptr cs:[si], al      ;Store new check byte
    pop       bp
    xor       ax, ax
    retf                                ;All set up. Return control to system.

SetupInterruptsTextModeAndBDA PROC NEAR ;Offset 0x158
    cli       
    mov       word ptr ds:[INT_42_HandlerOfs], SYS_INT_10h_HandlerOfs;Offset 0x108 Offset 0xf065
    mov       word ptr ds:[INT_42_HandlerSeg], SYS_INT_10h_HandlerSeg;Offset 0x10a Segment 0xf000
    mov       word ptr ds:[INT_10_HandlerOfs], offset Int10Handler;Offset 0x40 Offset 0xb60
    mov       word ptr ds:[INT_10_HandlerSeg], cs;Offset 0x42
    mov       word ptr ds:[INT_6D_HandlerOfs], offset Int6DHandler;Offset 0x1b4 Offset 0xb70
    mov       word ptr ds:[INT_6D_HandlerSeg], cs;Offset 0x1b6
    sti       
    mov       word ptr ds:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x34f0
    mov       word ptr ds:[INT_1F_HandlerSeg], cs;Offset 0x7e
    mov       word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x30f0
    mov       word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    mov       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0x487 0x60
    mov       ch, BDA_VDDA_VGA OR BDA_VDDA_LineMode400;0x11
    test      al, 40h
    jne       IsColor                   ;Offset 0x19c
    or        ch, BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;0x6
IsColor:                                ;Offset 0x19c
    lea       bx, [VideoOverrideTable1] ;Offset 0x2e2
    call      SetupVideoParameterControlBlock;Offset 0x1b1
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], ch;Offset 0x489
    call      SetCardAddressOrFallbackToSystemAdapter;Offset 0x1ce
    call      SetTextMode               ;Offset 0x232
    call      SetDisplayCombinationCode ;Offset 0x27f
    ret
SetupInterruptsTextModeAndBDA ENDP

SetupVideoParameterControlBlock PROC NEAR;Offset 0x1b1
    mov       ax, cs
    cmp       ax, word ptr cs:[VideoOverrideTable2.VideoParameterTable.sgm];Offset 0x31a
    jne       IsShadowed                ;Offset 0x1be
    ;add       bx, (SIZEOF VideoOverridePointerTable) + (SIZEOF SecondarySavePointerTable);0x36
    DB 081h, 0C3h, 036h, 000h           ;!!!!!!!!!!!!!! Assembles to different variant.
IsShadowed:                             ;Offset 0x1be
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], bx;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], ax;Offset 0x4aa
    ret
SetupVideoParameterControlBlock ENDP

;Offset 0x1c6
InitMDA                 DB VGA_Misc_IOAddressSel_MDA, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Color, BDA_DM_80x25_16_Color_Text;0x0, 0xD4, 0x20, 0x03

;Offset 0x1ca
InitCGA                 DB VGA_Misc_IOAddressSel_CGA, VGA_CRTControllerIndexB_lowbyte, BDA_DH_80x25Monochrome, BDA_DM_80x25_Monochrome_Text;0x1, 0xB4, 0x30, 0x07

SetCardAddressOrFallbackToSystemAdapter PROC NEAR;Offset 0x1ce
    lea       si, [InitMDA]             ;Offset 0x1c6
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    call      SetCardMode               ;Offset 0x1e9   Attempt to set card to Monochrome
    jae       Success                   ;Offset 0x1e8
    lea       si, [InitCGA]             ;Offset 0x1ca
    and       byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487 0xfd
    call      SetCardMode               ;Offset 0x1e9   Set card to Color
Success:                                ;Offset 0x1e8
    ret
SetCardAddressOrFallbackToSystemAdapter ENDP

SetCardMode PROC NEAR                   ;Offset 0x1e9
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
    call      TestRegister              ;Offset 0x222 - If non-selected CRTC register is still writeable, it's a VGA card
    jne       IsVGA                     ;Offset 0x221
    mov       ah, 1ah
    call      TestRegister              ;Offset 0x222 - Make sure
    jne       IsVGA                     ;Offset 0x221
    lodsb     byte ptr cs:[si]          ;Card is not VGA.
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    lodsb     byte ptr cs:[si]          ;Load fallback mode
    cbw                                 ;zero out ah
    int       42h                       ;Call System interrupt handler to set video mode.
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
IsVGA:                                  ;Offset 0x221
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
TestRegister PROC NEAR                  ;Offset 0x222
    in        al, dx                    ;Read register
    mov       bl, al                    ;Store result in bl
    mov       al, ah                    ;Move test value to al
    out       dx, al                    ;Write test value to register
    jmp       Delay                     ;Offset 0x22a   !!!!!!Speed sensitive!
Delay:                                  ;Offset 0x22a
    in        al, dx                    ;Read back test value
    ;cmp       al, ah                    ;Compare read test value to original test value
    DB 03Ah, 0C4h                       ;!!!!!! Misassembled
    mov       al, bl                    ;Move original register value to al
    out       dx, al                    ;Restore original value back to register
    stc                                 ;Set carry flag
    ret
TestRegister ENDP

SetTextMode PROC NEAR                   ;Offset 0x232
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    xor       al, al                    ;zero al
    out       dx, al                    ;select color 0
    inc       dx                        ;Port 0x3c9
    mov       cx, 300h                  ;768 values
LoopColors:                             ;Offset 0x23c
    out       dx, al                    ;Zero all colors
    loop      LoopColors                ;Offset 0x23c
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    cmp       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    mov       bl, al                    ;store modified vdda
    je        SetMonochrome             ;Offset 0x256
    mov       ax, (BDA_DH_80x25Color SHL 8) OR BDA_DM_80x25_16_Color_Text;0x2003
    mov       bh, BDA_EFBS_MDAHiResEnhanced_2;0x9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        SetColor                  ;Offset 0x260
SetMonochrome:                          ;Offset 0x256
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    mov       ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_DM_80x25_Monochrome_Text;0x3007
    mov       bh, BDA_EFBS_CGAMono80x25_2;0xb
SetColor:                               ;Offset 0x260
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

SetDisplayCombinationCode PROC NEAR     ;Offset 0x27f
    mov   ax, BDA_DCC_CVGA SHL 8        ;0x800
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsColor                       ;Offset 0x28b
    mov   ah, BDA_DCC_MVGA              ;0x7
IsColor:                                ;Offset 0x28b
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    jne   IsVGA                         ;Offset 0x2a2
    inc   ax                            ;BDA_DCC_MDPA
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsVGA                         ;Offset 0x2a2
    inc   ax                            ;BDA_DCC_CGA
    call  CheckRegistersMemoryMapped    ;Offset 0x2a8
    jne   IsVGA                         ;Offset 0x2a2
    mov   al, BDA_DCC_PGC               ;0x6
IsVGA:                                  ;Offset 0x2a2
    mov   bx, ax
    call  SetDisplayCombinationCodeIndex;Offset 0x2b2c
    ret
SetDisplayCombinationCode ENDP

;
;input:
;   -
;output
;   zero flag = 0 = failed, 1 = success
;destroys:
;   dx, si
CheckRegistersMemoryMapped PROC NEAR    ;Offset 0x2a8
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
MGAReadCRTCExtensionRegister PROC NEAR  ;Offset 0x2c0
    push  dx
    push  bx
    mov   bl, al
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, al
    inc   dx
    in    al, dx
    pushf
    cmp   bl, MGA_CRTCExt_RequesterControl;0x7
    jne   NotRequesterControl           ;Offset 0x2d2
    or    al, 0fh
NotRequesterControl:                    ;Offset 0x2d2
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
MGAWriteCRTCExtensionRegister PROC NEAR ;Offset 0x2d6
    push  dx
    xchg  al, ah
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, ax
    xchg  al, ah
    pop   dx
    ret
MGAWriteCRTCExtensionRegister ENDP

    nop                                 ;hello

;Offset 0x2e2
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

;Offset 0x372
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

;Offset 0x382
Data0x382               DB BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2c
                        DB BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x28
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2d
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2a
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2e
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_GraphicsOperation;0x1e
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29

;Offset 0x38a
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
    xchg bx, bx

;The assembler doesn't like all that data on one line, so I am just overlapping these to make struct accesses work.
;If anyone knows a cleaner way, please let me know
VideoParameters VideoParametersTable 29 dup(<>);Offset 0x3c0

ORG offset VideoParameters

;Offset 0x3c0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x400
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x440
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x480
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x4c0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x500
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x540
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000101h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0C2h, 0FFh
                        DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

;Offset 0x580
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000300h
                        DB 0A6h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0x5c0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 07D00h
                        DD 006000F21h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x600
                        DB 000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 000000000h
                        DB 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x640
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x680
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 062h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x6c0
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x700
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 006000F09h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x740
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x780
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 000h, 05Eh, 06Eh, 08Bh, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x7c0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
                        DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x800
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A2h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh

;Offset 0x840
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x880
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x8c0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x900
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x940
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x980
                        DB 028h
                        DB 018h
                        DB 010h
                        DW 00800h
                        DD 002000308h
                        DB 067h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x9c0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 067h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa00
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 066h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0xa40
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0C3h, 0FFh
                        DB 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh

;Offset 0xa80
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xac0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 00E000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0xb00
Int10CallTable          DW SetVideoMode;Offset 0x0c30                                       ;  0
                        DW SetTextModeCursorShape;Offset 0x105d                             ;  1
                        DW SetCursorPosition;Offset 0x10d9                                  ;  2
                        DW GetCursorPositionAndSize;Offset 0x110d                           ;  3
                        DW ReadLightPenPosition_SystemForward;Offset 0x111f                 ;  4
                        DW SelectActiveDisplayPage;Offset 0x112c                            ;  5
                        DW ScrollUpWindow;Offset 0x1162                                     ;  6
                        DW ScrollDownWindow;Offset 0x1387                                   ;  7
                        DW ReadCharacterAndAttributeAtCursorPosition;Offset 0x15c4          ;  8
                        DW WriteCharacterAndAttributeAtCursorPosition;Offset 0x1791         ;  9
                        DW WriteCharacterOnlyAtCursorPosition;Offset 0x1a9a                 ;  a
                        DW SetBorderColorOrPalette;Offset 0x1afb                            ;  b
                        DW WriteGraphicsPixel;Offset 0x1ba4                                 ;  c
                        DW ReadGraphicsPixel;Offset 0x1c81                                  ;  d
                        DW TeletypeOutput;Offset 0x1d37                                     ;  e
                        DW GetCurrentVideoMode;Offset 0x1dbe                                ;  f
                        DW RAMDACFunctions;Offset 0x1dd0                                    ; 10
                        DW TextFunctions;Offset 0x1fbf                                      ; 11
                        DW AlternateFunctions;Offset 0x20c3                                 ; 12
                        DW WriteString  ;Offset 0x224a                                      ; 13
                        DW NotImplemented ;Offset 0x22d6                                    ; 14
                        DW NotImplemented ;Offset 0x22d6                                    ; 15
                        DW NotImplemented ;Offset 0x22d6                                    ; 16
                        DW NotImplemented ;Offset 0x22d6                                    ; 17
                        DW NotImplemented ;Offset 0x22d6                                    ; 18
                        DW NotImplemented ;Offset 0x22d6                                    ; 19
                        DW DisplayCombinationCodeFunctions;Offset 0x22d7                    ; 1a
                        DW FunctionalityAndStateInfo;Offset 0x231e                          ; 1b
                        DW SaveRestoreVideoState;Offset 0x242a                              ; 1c

;Offset 0xb3a
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 070h, 00Bh, 000h, 000h, 0FFh, 0FFh, 04Eh, 003h, 072h, 003h, 002h, 024h, 016h, 024h, 010h, 02Ah
                        DB 08Ch, 02Ah, 0BEh, 02Ch, 000h, 000h

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

Int10Handler:                           ;Offset 0xb60
    cmp  ax, 4f10h
    jne  Label0xb6a                     ;Offset 0xb6a
    sti
    cld
    jmp  VESAPowerManagement            ;Offset 0x4afc
Label0xb6a:                             ;Offset 0xb6a
    int  6dh
    iret

    xchg bx, bx                         ;hello
    nop

Int6DHandler:                           ;Offset 0xb70
    sti
    cld
    cmp  ax, 4f10h
    jne  Label0xb7a                     ;Offset 0xb7a
    jmp  VESAPowerManagement            ;Offset 0x4afc
Label0xb7a:                             ;Offset 0xb7a
    cmp  ax, 4f15h
    jne  Label0xb82                     ;Offset 0xb82
    jmp  VESADDC                        ;Offset 0x4c05
Label0xb82:                             ;Offset 0xb82
    cmp  ah, 0afh
    je   Label0xb8c                     ;Offset 0xb8c
    cmp  ah, 0bdh
    jne  Label0xb8d                     ;Offset 0xb8d
Label0xb8c:                             ;Offset 0xb8c
    iret
Label0xb8d:                             ;Offset 0xb8d
    cmp  ax, 0bf00h
    jne  Label0xba2                     ;Offset 0xba2
    mov  bx, cs
    mov  cx, offset Func0x5c72          ;Offset 0x5c72
    mov  ax, 00bfh
    mov  ah, byte ptr cs:[Data0x7ff1]   ;Offset 0x7ff1
    mov  al, 0bfh
    iret
Label0xba2:                             ;Offset 0xba2
    cmp  ah, 01ch
    ja   Label0xbfa                     ;Offset 0xbfa
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
    jl   Label0xbf1                     ;Offset 0xbf1
    cmp  ah, 0eh
    je   Label0xbcc                     ;Offset 0xbcc
    cmp  ah, 06h
    jl   Label0xbf1                     ;Offset 0xbf1
    cmp  ah, 0ah
    jg   Label0xbf1                     ;Offset 0xbf1
Label0xbcc:                             ;Offset 0xbcc
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
    call word ptr cs:[si + Int10CallTable];Offset 0xb00
    pop  di
    push ax
    push dx
    mov  ax, di
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    out  dx, ax
    pop  dx
    pop  ax
    jmp  Label0xbf6                     ;Offset 0xbf6
Label0xbf1:                             ;Offset 0xbf1
    call word ptr cs:[si + Int10CallTable];Offset 0xb00
Label0xbf6:                             ;Offset 0xbf6
    pop  di
    pop  si
    pop  ds
    iret
Label0xbfa:                             ;Offset 0xbfa
    cmp  ah, 04fh
    jne  Label0xc02                     ;Offset 0xc02
    jmp  VESAHandler                    ;Offset 0x5743
Label0xc02:                             ;Offset 0xc02
    int  42h
    iret

Func0xc05 PROC NEAR                     ;Offset 0xc05
    push ax
    push bx
    mov  ax, cs
    mov  bx, ds
    cmp  ax, bx
    pop  bx
    pop  ax
    jne  Label0xc2e                     ;Offset 0xc2e
    cmp  cl, 0eh
    jne  Label0xc2e                     ;Offset 0xc2e
    cmp  al, 67h
    je   Label0xc2c                     ;Offset 0xc2c
    cmp  al, 6ah
    je   Label0xc2c                     ;Offset 0xc2c
    cmp  al, 79h
    je   Label0xc2c                     ;Offset 0xc2c
    cmp  al, 71h
    je   Label0xc2c                     ;Offset 0xc2c
    cmp  al, 70h
    je   Label0xc2c                     ;Offset 0xc2c
    cmp  al, 98h
Label0xc2c:                             ;Offset 0xc2c
    clc
    ret
Label0xc2e:                             ;Offset 0xc2e
    stc
    ret
Func0xc05 ENDP

SetVideoMode PROC NEAR                  ;Offset 0x0c30
    push ax
    mov  ah, al
    mov  al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and  al, BDA_VDDA_Reserved          ;0x20
    je   Label0xc3d                     ;Offset 0xc3d
    or   ah, BDA_DM_DONT_CLEAR_SCREEN   ;0x80
Label0xc3d:                             ;Offset 0xc3d
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shl  ax, 01h
    xchg al, ah
    rcr  ax, 01h
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    ja   Label0xcaa                     ;Offset 0xcaa
    push ax
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x7
    je   Label0xc5c                     ;Offset 0xc5c
    cmp  al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je   Label0xc5c                     ;Offset 0xc5c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Color          ;0x20
    jmp  Label0xc63                     ;Offset 0xc63
Label0xc5c:                             ;Offset 0xc5c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Monochrome     ;0x30
Label0xc63:                             ;Offset 0xc63
    mov  byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    pop  ax
    call Func0xe75                      ;Offset 0xe75
    mov  word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x30f0
    mov  word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    and  ah, 0f3h
    test byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je   Label0xc80                     ;Offset 0xc80
    jp   Func0xcf3                      ;Offset 0xcf3
Label0xc80:                             ;Offset 0xc80
    test ah, 02h
    jne  Label0xc94                     ;Offset 0xc94
    mov  si, 03d4h                      ;Port 0x3d4
    cmp  al, 07h
    je   Label0xc90                     ;Offset 0xc90
    cmp  al, 0fh
    jne  Func0xd08                      ;Offset 0xd08
Label0xc90:                             ;Offset 0xc90
    mov  al, 03h
    jmp  Func0xd05                      ;Offset 0xd05
Label0xc94:                             ;Offset 0xc94
    cmp  al, 02h
    jb   Label0xc9f                     ;Offset 0xc9f
    cmp  al, 03h
    ja   Label0xc9f                     ;Offset 0xc9f
    or   ah, 04h
Label0xc9f:                             ;Offset 0xc9f
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485 0x8
    call Func0xce3                      ;Offset 0xce3
Label0xca8:                             ;Offset 0xca8
    pop  ax
    ret
Label0xcaa:                             ;Offset 0xcaa
    cmp  al, 6ah
    jne  Label0xcb3                     ;Offset 0xcb3
    mov  ax, 0102h
    jmp  Label0xcc7                     ;Offset 0xcc7
Label0xcb3:                             ;Offset 0xcb3
    mov  ah, al
    and  ah, 0c0h
    or   ah, 01h
    and  al, 3fh
    cmp  al, 20h
    jb   Label0xca8                     ;Offset 0xca8
    cmp  al, 3fh
    ja   Label0xca8                     ;Offset 0xca8
    sub  al, 20h
Label0xcc7:                             ;Offset 0xcc7
    push bx
    mov  bx, ax
    mov  ax, 4f02h
    int  10h
    pop  bx
    pop  ax
    ret
SetVideoMode ENDP

Func0xcd2 PROC NEAR                     ;Offset 0xcd2
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 0xe
    call Func0xce3                      ;Offset 0xce3
    mov  word ptr ds:[BDA_CursorEndStartScanLine], 0b0ch;Offset 0x460
    pop  ax
    ret
Func0xcd2 ENDP

Func0xce3 PROC NEAR                     ;Offset 0xce3
    mov  byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484
    or   ah, BDA_VMO_Inactive           ;0x8
    mov  byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    cbw
    int  42h
    ret
Func0xce3 ENDP

;
;inputs:
;   al = display mode
;   ah = video mode options
;   si = video base io port
;
;
Func0xcf3 PROC NEAR                     ;Offset 0xcf3
    test      ah, BDA_VMO_Monochrome    ;0x2
    je        Func0xcd2                 ;Offset 0xcd2
    mov       si, VGA_CRTControllerIndexB;Port 0x3b4
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Func0xd08                 ;Offset 0xd08
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Func0xd08                 ;Offset 0xd08
    mov       al, BDA_DM_80x25_Monochrome_Text;0x7
Func0xcf3 ENDP
;continue!
Func0xd05 PROC NEAR                     ;Offset 0xd05
    and       ah, NOT BDA_VMO_DontClearDisplay;0x7f
Func0xd05 ENDP
;continue!
Func0xd08 PROC NEAR                     ;Offset 0xd08
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    push      ax
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x6614
    je        Label0xd63                ;Offset 0xd63
    push      cx
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0xd5c                ;Offset 0xd5c
    mov       ax, 1600h
    int       2fh                       ;Windows Enhanced Mode Installation Check
    test      al, 7fh
    je        Label0xd5c                ;Offset 0xd5c
    push      bx
    push      di
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       di, PCI_MGA_Option        ;0x40
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8h
    nop
    call      AccessPCIRegister         ;Offset 0x68f8
    test      cl, 03h
    pop       di
    pop       bx
    je        Label0xd5c                ;Offset 0xd5c
    call      TurnScreenOff             ;Offset 0x2b15
    push      bx
    push      si
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       si, MGA_MemAddr_SequencerIndex;0x1fc4
    call      IndirectRegisterReadWord  ;Offset 0x6a38
    test      ch, 20h                   ;Test a reserved bit...
    pop       si
    pop       bx
    je        Label0xd5c                ;Offset 0xd5c
    xor       cx, cx
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x7b13
    call      TurnScreenOn              ;Offset 0x2b11
Label0xd5c:                             ;Offset 0xd5c
    pop       cx
    mov       ax, 0000h
    call      Func0x5382                ;Offset 0x5382
Label0xd63:                             ;Offset 0xd63
    pop       ax
    push      ax
    call      TurnScreenOff             ;Offset 0x2b15
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
    test      al, 80h                   ;Unknown bit
    jne       Label0xd88                ;Offset 0xd88
    mov       ah, 00h
Label0xd88:                             ;Offset 0xd88
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    pop       dx
    pop       ax
    call      Func0xe72                 ;Offset 0xe72
    xor       ax, ax
    mov       es, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    lea       di, ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       cx, 0008h
    rep stosw
    call      Func0x29f0                ;Offset 0x29f0
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
    call      Func0x2845                ;Offset 0x2845
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
    call      Func0xf03                 ;Offset 0xf03
    call      ClearScreen               ;Offset 0x26a7
    lea       si, [Func0xf22]           ;Offset 0xf22
    call      Func0x2aae                ;Offset 0x2aae
    je        Label0xdf3                ;Offset 0xdf3
    lea       si, [Func0xf4d]           ;Offset 0xf4d
Label0xdf3:                             ;Offset 0xdf3
    call      si
    call      Func0xf70                 ;Offset 0xf70
    call      Func0x2ad5                ;Offset 0x2ad5
    pop       ax
    call      TurnScreenOn              ;Offset 0x2b11
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0xe1d                ;Offset 0xe1d
    mov       ah, al
    lea       bx, [Data0x382]           ;Offset 0x382
    xlatb     cs:[bx]
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    mov       al, 30h
    cmp       ah, BDA_DM_640x200_BW_Graphics;0x6
    jne       Label0xe1a                ;Offset 0xe1a
    mov       al, 3fh
Label0xe1a:                             ;Offset 0xe1a
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
Label0xe1d:                             ;Offset 0xe1d
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_Reserved     ;0x20
    je        Label0xe29                ;Offset 0xe29
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_Reserved;Offset 0x489 0xdf
Label0xe29:                             ;Offset 0xe29
    push      ax
    push      bx
    push      cx
    push      edx
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x6614
    je        Label0xe66                ;Offset 0xe66
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0xe66                ;Offset 0xe66
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, NOT MGA_MISCCTRL_MAFC_MASK;0xf9
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0xe55                ;Offset 0xe55
    and       cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_Enable);0xf8
Label0xe55:                             ;Offset 0xe55
    or        cl, MGA_MISCCTRL_MAFC_PanelLinkMode;0x4
    mov       ch, cl
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       al, 01h
    call      Func0x5382                ;Offset 0x5382
Label0xe66:                             ;Offset 0xe66
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
Func0xd08 ENDP

Func0xe72 PROC NEAR                     ;Offset 0xe72
    call      Func0x61ae                ;Offset 0x61ae
Func0xe72 ENDP
;continue!
Func0xe75 PROC NEAR                     ;Offset 0xe75
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    je        Label0xef8                ;Offset 0xef8
    cmp       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je        Label0xef8                ;Offset 0xef8
    push      bx
    push      dx
    lea       si, [Data0xef9]           ;Offset 0xef9
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0xe94                ;Offset 0xe94
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0xe94                ;Offset 0xe94
    ;add       si, 0005h
    DB 81h, 0C6h, 05h, 00h              ;!!!!!!!!!!!!! misassembly
Label0xe94:                             ;Offset 0xe94
    xchg      ax, bx
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       al, byte ptr cs:[si + 01h]
    je        Label0xef5                ;Offset 0xef5
    mov       ah, bl
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    mov       bl, BDA_DM_80x25_Monochrome_Text;0x7
    cmp       al, byte ptr cs:[si + 02h]
    je        Label0xef5                ;Offset 0xef5
    mov       bl, BDA_DM_80x25_16_Color_Text;0x3
    cmp       al, byte ptr cs:[si + 03h]
    js        Label0xef5                ;Offset 0xef5
    mov       bl, ah
    and       bh, 0fdh
    or        bh, byte ptr cs:[si + 04h]
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_AdapterTypeMask;0xf
    sub       al, BDA_EFBS_MDAColor40x25_2;0x6
    cmp       al, BDA_EFBS_CGAMono80x25 ;0x5
    ja        Label0xef5                ;Offset 0xef5
    sub       al, BDA_EFBS_MDAHiResEnhanced;0x3
    cbw
    mov       al, BDA_EFBS_CGAMono80x25_2;0xb
    jbe       Label0xed2                ;Offset 0xed2
    mov       al, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0xed2:                             ;Offset 0xed2
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
Label0xef5:                             ;Offset 0xef5
    xchg      ax, bx
    pop       dx
    pop       bx
Label0xef8:                             ;Offset 0xef8
    ret
Func0xe75 ENDP

;Offset 0xef9
Data0xef9               DB 080h, VGA_CRTControllerIndexB_lowbyte, 0FFh, BDA_DH_80x25Monochrome, 002h;0x80 0xB4 0xFF 0x30 0x02

;Offset 0xefe
Data0xefe               DB 001h, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Monochrome, 000h, 000h;0x01 0xD4 0x30 0x00 0x00

Func0xf03 PROC NEAR                     ;Offset 0xf03
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   PaletteLoadingDisabled        ;Offset 0xf21
    push  si
    xor   ax, ax
    mov   cx, 0010h
    call  Func0x28c5                    ;Offset 0x28c5
    inc   ax
    inc   si
    mov   cl, 01h
    call  Func0x28c5                    ;Offset 0x28c5
    pop   si
    call  Func0x290b                    ;Offset 0x290b
    call  Func0x2920                    ;Offset 0x2920
PaletteLoadingDisabled:                 ;Offset 0xf21
    ret
Func0xf03 ENDP

Func0xf22 PROC NEAR                     ;Offset 0xf22
    call  Func0x26e5                    ;Offset 0x26e5
    call  GetNumberOfActiveScanlines    ;Offset 0x2a60
    dec   al
    mov   ax, 0002h
    js    Label0xf3e                    ;Offset 0xf3e
    mov   al, 03h
    jne   Label0xf3c                    ;Offset 0xf3c
    mov   al, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne   Label0xf3e                    ;Offset 0xf3e
Label0xf3c:                             ;Offset 0xf3c
    mov   ah, 80h
Label0xf3e:                             ;Offset 0xf3e
    mov   bl, ah
    call  Func0x2726                    ;Offset 0x2726
    call  Func0xfeb                     ;Offset 0xfeb
    call  Func0x101e                    ;Offset 0x101e
    call  Func0x26eb                    ;Offset 0x26eb
    ret
Func0xf22 ENDP

Func0xf4d PROC NEAR                     ;Offset 0xf4d
    call  GetNumberOfActiveScanlines    ;Offset 0x2a60
    dec   al
    js    Label0xf67                    ;Offset 0xf67
    lea   ax, [Font8x16]                ;Offset 0x38f0
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    mov   ax, cs
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
    jne   Label0xf67                    ;Offset 0xf67
    mov   ax, 0bd70h
    int   6dh
Label0xf67:                             ;Offset 0xf67
    xor   ax, ax
    mov   word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call  Func0xfc7                     ;Offset 0xfc7
    ret
Func0xf4d ENDP

Func0xf70 PROC NEAR                     ;Offset 0xf70
    mov   al, BDA_VPCB_PaletterProfileTable;0xa
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    Label0xfc6                    ;Offset 0xfc6
    mov   al, 14h
    call  Func0x2ac1                    ;Offset 0x2ac1
    jne   Label0xfc6                    ;Offset 0xfc6
    lodsb byte ptr es:[si]
    or    al, al
    je    Label0xf94                    ;Offset 0xf94
    mov   ax, 1420h
    js    Label0xf8c                    ;Offset 0xf8c
    mov   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
Label0xf8c:                             ;Offset 0xf8c
    dec   ax
    xchg  al, ah
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    out   dx, ax
Label0xf94:                             ;Offset 0xf94
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   Label0xfc6                    ;Offset 0xfc6
    push  es
    push  si
    add   si, 0003h
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0xfaf                    ;Offset 0xfaf
    mov   ah, 10h
    call  Func0x28c7                    ;Offset 0x28c7
Label0xfaf:                             ;Offset 0xfaf
    pop   si
    pop   es
    add   si, 000bh
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0xfc6                    ;Offset 0xfc6
    mov   dx, VGA_DACWriteIndex         ;Port 0x3c8
    out   dx, al
    inc   dx
    call  Func0x299f                    ;Offset 0x299f
Label0xfc6:                             ;Offset 0xfc6
    ret
Func0xf70 ENDP

Func0xfc7 PROC NEAR                     ;Offset 0xfc7
    mov   al, BDA_VPCB_GrahicsCharSetOverride;0xc
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    Label0xfea                    ;Offset 0xfea
    mov   al, 07h
    call  Func0x2ac1                    ;Offset 0x2ac1
    jne   Label0xfea                    ;Offset 0xfea
    lodsb byte ptr es:[si]
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsw word ptr es:[si]
    mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
Label0xfea:                             ;Offset 0xfea
    ret
Func0xfc7 ENDP

Func0xfeb PROC NEAR                     ;Offset 0xfeb
    mov   al, BDA_VPCB_AlphaNumCharSetOverride;0x8
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    Label0x101d                   ;Offset 0x101d
    mov   al, 0bh
    call  Func0x2ac1                    ;Offset 0x2ac1
    jne   Label0x101d                   ;Offset 0x101d
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
    call  Func0x2749                    ;Offset 0x2749
    call  Func0x27c8                    ;Offset 0x27c8
    pop   ax
    cmp   al, 0ffh
    je    Label0x101d                   ;Offset 0x101d
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x101d:                            ;Offset 0x101d
    ret
Func0xfeb ENDP

Func0x101e PROC NEAR                    ;Offset 0x101e
    mov   al, BDA_VPCB_SecondaryCharSetOverride;0x6
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    Label0x105c                   ;Offset 0x105c
    mov   al, 07h
    call  Func0x2ac1                    ;Offset 0x2ac1
    jne   Label0x105c                   ;Offset 0x105c
    lodsw word ptr es:[si]
    cmp   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    jne   Label0x105c                   ;Offset 0x105c
    push  ax
    xchg  al, ah
    and   al, 7fh
    xchg  ax, bx
    inc   si
    les   bp, es:[si]
    xor   dx, dx
    mov   cx, 0100h
    call  Func0x2749                    ;Offset 0x2749
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
Label0x105c:                            ;Offset 0x105c
    ret
Func0x101e ENDP

SetTextModeCursorShape PROC NEAR        ;Offset 0x105d
    push      dx
    mov       ax, cx
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x1086               ;Offset 0x1086
    and       ah, 60h
    cmp       ah, 20h
    mov       ax, 1e00h
    je        Label0x1086               ;Offset 0x1086
    mov       ax, cx
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_CursorEmulationEnabled;Offset 0x487 0x8
    jne       Label0x1086               ;Offset 0x1086
    call      Func0x108b                ;Offset 0x108b
    call      SetCursorEndStart         ;Offset 0x2698
    pop       dx
    ret
Label0x1086:                            ;Offset 0x1086
    call      SetCursorEndStart         ;Offset 0x2698
    pop       dx
    ret
SetTextModeCursorShape ENDP

Func0x108b PROC NEAR                    ;Offset 0x108b
    call      Func0x2aae                ;Offset 0x2aae
    jne       Label0x10d8               ;Offset 0x10d8
    test      ax, 0e0e0h
    jne       Label0x10d8               ;Offset 0x10d8
    mov       dl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       dx
    ;cmp       ah, al
    DB 3Ah, 0E0h                        ;!!!!!!!! Misassembly
    ja        Label0x10d1               ;Offset 0x10d1
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    ja        Label0x10ae               ;Offset 0x10ae
    ;cmp       al, dl
    DB 3Ah, 0C2h                        ;!!!!!!!! Misassembly
    ja        Label0x10ae               ;Offset 0x10ae
    je        Label0x10d8               ;Offset 0x10d8
    dec       dx
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    je        Label0x10d8               ;Offset 0x10d8
    inc       dx
Label0x10ae:                            ;Offset 0x10ae
    cmp       al, 03h
    jbe       Label0x10d8               ;Offset 0x10d8
    sub       al, ah
    cmp       al, 02h
    mov       al, dl
    jbe       Label0x10c5               ;Offset 0x10c5
    cmp       ah, 02h
    jbe       Label0x10d8               ;Offset 0x10d8
    inc       dx
    shr       dl, 01h
    mov       ah, dl
    ret
Label0x10c5:                            ;Offset 0x10c5
    add       ah, dl
    sub       ah, cl
    cmp       al, 0dh
    jb        Label0x10d8               ;Offset 0x10d8
    sub       ax, 0101h
    ret
Label0x10d1:                            ;Offset 0x10d1
    cmp       al, 00h
    je        Label0x10d8               ;Offset 0x10d8
    mov       dh, al
    xchg      ax, dx
Label0x10d8:                            ;Offset 0x10d8
    ret
Func0x108b ENDP

SetCursorPosition PROC NEAR             ;Offset 0x10d9
    push      ax
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       word ptr [si + DBA_CursorPositionPage0], dx;Offset 0x450
    cmp       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    jne       Func0x110b                ;Offset 0x110b
    push      dx
    mov       ax, dx
    jmp       Func0x10f3                ;Offset 0x10f3
SetCursorPosition ENDP

Func0x10f0 PROC NEAR                    ;Offset 0x10f0
    pop       dx
    push      ax
    push      dx
Func0x10f0 ENDP
;continue!
Func0x10f3 PROC NEAR                    ;Offset 0x10f3
    call      Func0x2650                ;Offset 0x2650
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
Func0x10f3 ENDP
;continue!
Func0x110b PROC NEAR                    ;Offset 0x110b
    pop       ax
    ret
Func0x110b ENDP

GetCursorPositionAndSize PROC NEAR      ;Offset 0x110d
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

ReadLightPenPosition_SystemForward PROC NEAR;Offset 0x111f
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x1129               ;Offset 0x1129
    xor       ah, ah
    ret
Label0x1129:                            ;Offset 0x1129
    int       42h
    ret
ReadLightPenPosition_SystemForward ENDP

SelectActiveDisplayPage PROC NEAR       ;Offset 0x112c
    push      dx
    cbw
    mov       si, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, 07h
    pop       ax
    ja        Label0x1148               ;Offset 0x1148
    shr       ax, 01h
Label0x1148:                            ;Offset 0x1148
    call      SetStartAddressVGA        ;Offset 0x1153
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Func0x10f0                ;Offset 0x10f0
SelectActiveDisplayPage ENDP

SetStartAddressVGA PROC NEAR            ;Offset 0x1153
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

ScrollUpWindow PROC NEAR                ;Offset 0x1162
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
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x1195               ;Offset 0x1195
    ja        Label0x118a               ;Offset 0x118a
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x119e               ;Offset 0x119e
Label0x1184:                            ;Offset 0x1184
    jmp       Label0x121c               ;Offset 0x121c
Label0x1187:                            ;Offset 0x1187
    jmp       Label0x130a               ;Offset 0x130a
Label0x118a:                            ;Offset 0x118a
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x11ff               ;Offset 0x11ff
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x1187               ;Offset 0x1187
    jmp       Label0x128f               ;Offset 0x128f
Label0x1195:                            ;Offset 0x1195
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x1184               ;Offset 0x1184
Label0x119e:                            ;Offset 0x119e
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2650                ;Offset 0x2650
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
    jne       Label0x1209               ;Offset 0x1209
Label0x11cf:                            ;Offset 0x11cf
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1205               ;Offset 0x1205
Label0x11d7:                            ;Offset 0x11d7
    mov       cx, dx
    rep movsw
    add       si, bp
    add       di, bp
    dec       ax
    jne       Label0x11d7               ;Offset 0x11d7
Label0x11e2:                            ;Offset 0x11e2
    xchg      ah, bh
    mov       al, 20h
Label0x11e6:                            ;Offset 0x11e6
    mov       cx, dx
    rep stosw
    add       di, bp
    dec       bx
    jne       Label0x11e6               ;Offset 0x11e6
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x11ff               ;Offset 0x11ff
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x11ff:                            ;Offset 0x11ff
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x1205:                            ;Offset 0x1205
    mov       bl, al
    jmp       Label0x11e2               ;Offset 0x11e2
Label0x1209:                            ;Offset 0x1209
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x120e:                            ;Offset 0x120e
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x120e               ;Offset 0x120e
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x11cf               ;Offset 0x11cf
Label0x121c:                            ;Offset 0x121c
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    shl       ah, 01h
    shl       ah, 01h
    xchg      ax, cx
    call      Func0x265e                ;Offset 0x265e
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x1236               ;Offset 0x1236
    shl       ax, 01h
    shl       cl, 01h
Label0x1236:                            ;Offset 0x1236
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
    je        Label0x128b               ;Offset 0x128b
Label0x1258:                            ;Offset 0x1258
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 1fb0h]     ;Offset 0x1fb0
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       ax
    jne       Label0x1258               ;Offset 0x1258
Label0x1273:                            ;Offset 0x1273
    xchg      al, bh
Label0x1275:                            ;Offset 0x1275
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       bx
    jne       Label0x1275               ;Offset 0x1275
    jmp       Label0x11ff               ;Offset 0x11ff
Label0x128b:                            ;Offset 0x128b
    xchg      bl, al
    jmp       Label0x1273               ;Offset 0x1273
Label0x128f:                            ;Offset 0x128f
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x266e                ;Offset 0x266e
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
    je        Label0x1305               ;Offset 0x1305
Label0x12d6:                            ;Offset 0x12d6
    mov       cx, dx
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x12d6               ;Offset 0x12d6
    pop       bx
Label0x12e2:                            ;Offset 0x12e2
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x12f2:                            ;Offset 0x12f2
    mov       cx, si
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x12f2               ;Offset 0x12f2
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x11ff               ;Offset 0x11ff
Label0x1305:                            ;Offset 0x1305
    pop       bx
    mov       bl, ch
    jmp       Label0x12e2               ;Offset 0x12e2
Label0x130a:                            ;Offset 0x130a
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2680                ;Offset 0x2680
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
    je        Label0x1381               ;Offset 0x1381
Label0x1359:                            ;Offset 0x1359
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x1359               ;Offset 0x1359
    pop       bx
Label0x1369:                            ;Offset 0x1369
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x1371:                            ;Offset 0x1371
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x1371               ;Offset 0x1371
    jmp       Label0x11ff               ;Offset 0x11ff
Label0x1381:                            ;Offset 0x1381
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x1369               ;Offset 0x1369
ScrollUpWindow ENDP

ScrollDownWindow PROC NEAR              ;Offset 0x1387
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
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x13bb               ;Offset 0x13bb
    ja        Label0x13b0               ;Offset 0x13b0
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x13c4               ;Offset 0x13c4
Label0x13aa:                            ;Offset 0x13aa
    jmp       Label0x1444               ;Offset 0x1444
Label0x13ad:                            ;Offset 0x13ad
    jmp       Label0x153e               ;Offset 0x153e
Label0x13b0:                            ;Offset 0x13b0
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x1427               ;Offset 0x1427
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x13ad               ;Offset 0x13ad
    jmp       Label0x14bb               ;Offset 0x14bb
Label0x13bb:                            ;Offset 0x13bb
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x13aa               ;Offset 0x13aa
Label0x13c4:                            ;Offset 0x13c4
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    call      Func0x2650                ;Offset 0x2650
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
    jne       Label0x1431               ;Offset 0x1431
Label0x13f7:                            ;Offset 0x13f7
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x142d               ;Offset 0x142d
Label0x13ff:                            ;Offset 0x13ff
    mov       cx, dx
    rep movsw
    sub       si, bp
    sub       di, bp
    dec       ax
    jne       Label0x13ff               ;Offset 0x13ff
Label0x140a:                            ;Offset 0x140a
    xchg      ah, bh
    mov       al, 20h
Label0x140e:                            ;Offset 0x140e
    mov       cx, dx
    rep stosw
    sub       di, bp
    dec       bx
    jne       Label0x140e               ;Offset 0x140e
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x1427               ;Offset 0x1427
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1427:                            ;Offset 0x1427
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x142d:                            ;Offset 0x142d
    mov       bl, al
    jmp       Label0x140a               ;Offset 0x140a
Label0x1431:                            ;Offset 0x1431
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x1436:                            ;Offset 0x1436
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x1436               ;Offset 0x1436
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x13f7               ;Offset 0x13f7
Label0x1444:                            ;Offset 0x1444
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    shl       dh, 01h
    shl       dh, 01h
    mov       cx, dx
    call      Func0x265e                ;Offset 0x265e
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x1461               ;Offset 0x1461
    shl       ax, 01h
    inc       ax
    shl       cl, 01h
Label0x1461:                            ;Offset 0x1461
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
    je        Label0x14b7               ;Offset 0x14b7
Label0x1484:                            ;Offset 0x1484
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 2050h]     ;Offset 0x2050
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       ax
    jne       Label0x1484               ;Offset 0x1484
Label0x149f:                            ;Offset 0x149f
    xchg      al, bh
Label0x14a1:                            ;Offset 0x14a1
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       bx
    jne       Label0x14a1               ;Offset 0x14a1
    jmp       Label0x1427               ;Offset 0x1427
Label0x14b7:                            ;Offset 0x14b7
    xchg      bl, al
    jmp       Label0x149f               ;Offset 0x149f
Label0x14bb:                            ;Offset 0x14bb
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x266e                ;Offset 0x266e
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
    je        Label0x1539               ;Offset 0x1539
Label0x150a:                            ;Offset 0x150a
    mov       cx, dx
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x150a               ;Offset 0x150a
    pop       bx
Label0x1516:                            ;Offset 0x1516
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x1526:                            ;Offset 0x1526
    mov       cx, si
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x1526               ;Offset 0x1526
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1427               ;Offset 0x1427
Label0x1539:                            ;Offset 0x1539
    pop       bx
    mov       bl, ch
    jmp       Label0x1516               ;Offset 0x1516
Label0x153e:                            ;Offset 0x153e
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x2680                ;Offset 0x2680
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
    je        Label0x15be               ;Offset 0x15be
Label0x1596:                            ;Offset 0x1596
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x1596               ;Offset 0x1596
    pop       bx
Label0x15a6:                            ;Offset 0x15a6
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x15ae:                            ;Offset 0x15ae
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x15ae               ;Offset 0x15ae
    jmp       Label0x1427               ;Offset 0x1427
Label0x15be:                            ;Offset 0x15be
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x15a6               ;Offset 0x15a6
ScrollDownWindow ENDP

ReadCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x15c4
    push       dx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x5ff3               ;Offset 0x5ff3
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x15d8              ;Offset 0x15d8
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Label0x1611              ;Offset 0x1611
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x15d8:                            ;Offset 0x15d8
    push       dx
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2650               ;Offset 0x2650
    add        si, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x15fa              ;Offset 0x15fa
    pop        ds
    lodsw
    pop        dx
    ret
Label0x15fa:                            ;Offset 0x15fa
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a VGA Input Status
    pop        ds
Label0x1602:                            ;Offset 0x1602
    in         al, dx
    shr        al, 01h                  ;check HSync
    jb         Label0x1602              ;Offset 0x1602
    cli
Label0x1608:                            ;Offset 0x1608
    in         al, dx
    shr        al, 01h                  ;check HSync
    jae        Label0x1608              ;Offset 0x1608
    lodsw
    sti
    pop        dx
    ret
Label0x1611:                            ;Offset 0x1611
    push       cx
    push       es
    ja         Label0x1680              ;Offset 0x1680
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x265e               ;Offset 0x265e
    mov        si, ax
    sub        sp, 0008h
    mov        di, sp
    mov        ax, ss
    mov        es, ax
    mov        ax, 0b800h               ;Segment 0xb800
    mov        cx, 0004h
    cmp        byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov        ds, ax
    je         Label0x1644              ;Offset 0x1644
    shl        si, 01h
Label0x1637:                            ;Offset 0x1637
    call       Func0x1775               ;Offset 0x1775
    call       Func0x1775               ;Offset 0x1775
    add        si, 0050h
    loop       Label0x1637              ;Offset 0x1637
    jmp        Label0x1651              ;Offset 0x1651
Label0x1644:                            ;Offset 0x1644
    mov        al, byte ptr [si]
    stosb
    mov        al, byte ptr [si + 2000h];Offset 0x2000
    stosb
    add        si, 0050h
    loop       Label0x1644              ;Offset 0x1644
Label0x1651:                            ;Offset 0x1651
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, 0008h
    xor        ax, ax
Label0x165c:                            ;Offset 0x165c
    push       si
    push       di
    mov        cl, 04h
    repe cmpsw
    pop        di
    pop        si
    je         Label0x1679              ;Offset 0x1679
    add        si, 0008h
    inc        al
    je         Label0x1679              ;Offset 0x1679
    jno        Label0x165c              ;Offset 0x165c
    xor        cx, cx
    mov        ds, cx
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    jmp        Label0x165c              ;Offset 0x165c
Label0x1679:                            ;Offset 0x1679
    add        sp, 0008h
    pop        es
    pop        cx
    pop        dx
    ret
Label0x1680:                            ;Offset 0x1680
    push       bx
    mov        dh, 0a0h                 ;Segment 0xa000
    push       dx
    cmp        al, 13h
    jne        Label0x168b              ;Offset 0x168b
    jmp        Label0x1712              ;Offset 0x1712
Label0x168b:                            ;Offset 0x168b
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x266e               ;Offset 0x266e
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
Label0x16bb:                            ;Offset 0x16bb
    mov        al, byte ptr [si]
    not        al
    stosb
    add        si, dx
    loop       Label0x16bb              ;Offset 0x16bb
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out        dx, ax
Label0x16cb:                            ;Offset 0x16cb
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, bx
    xor        ax, ax
Label0x16d5:                            ;Offset 0x16d5
    push       si
    push       di
    mov        cx, bx
    call       Func0xc05                ;Offset 0xc05
    jb         Label0x16f5              ;Offset 0x16f5
    jne        Label0x16f5              ;Offset 0x16f5
    mov        cl, 08h
    repe cmpsb
    je         Label0x16ea              ;Offset 0x16ea
    pop        di
    pop        si
    jmp        Label0x16fb              ;Offset 0x16fb
Label0x16ea:                            ;Offset 0x16ea
    inc        si
    mov        cl, 06h
    repe cmpsb
    pop        di
    pop        si
    je         Label0x170b              ;Offset 0x170b
    jmp        Label0x16fb              ;Offset 0x16fb
Label0x16f5:                            ;Offset 0x16f5
    repe cmpsb
    pop        di
    pop        si
    je         Label0x170b              ;Offset 0x170b
Label0x16fb:                            ;Offset 0x16fb
    mov        cx, bx
    call       Func0xc05                ;Offset 0xc05
    jb         Label0x1705              ;Offset 0x1705
    add        si, 0002h
Label0x1705:                            ;Offset 0x1705
    add        si, bx
    inc        al
    jne        Label0x16d5              ;Offset 0x16d5
Label0x170b:                            ;Offset 0x170b
    add        sp, bx
    pop        bx
    pop        es
    pop        cx
    pop        dx
    ret
Label0x1712:                            ;Offset 0x1712
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2680               ;Offset 0x2680
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
Label0x173f:                            ;Offset 0x173f
    call       Func0x1751               ;Offset 0x1751
    stosb
    add        si, bp
    loop       Label0x173f              ;Offset 0x173f
    pop        bp
    pop        bx
    mov        dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov        al, VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4;0xe
    out        dx, al
    jmp        Label0x16cb              ;Offset 0x16cb
ReadCharacterAndAttributeAtCursorPosition ENDP

Func0x1751 PROC NEAR                    ;Offset 0x1751
    mov        bx, 0100h
    mov        al, byte ptr [si + 04h]
    call       Func0x175c               ;Offset 0x175c
    mov        al, byte ptr [si]
Func0x1751 ENDP
;continue!
Func0x175c PROC NEAR                    ;Offset 0x175c
    mov        ah, 03h
Label0x175e:                            ;Offset 0x175e
    mov        dl, VGA_GraphicsControllerData_lowbyte;Port 0x3cf
    mov        al, ah
    out        dx, al
    mov        dl, VGA_CRTControllerDataD_lowbyte;Port 0x3d5
    in         al, dx
    or         al, al
    je         Label0x176c              ;Offset 0x176c
    or         bl, bh
Label0x176c:                            ;Offset 0x176c
    rol        bh, 01h
    dec        ah
    jns        Label0x175e              ;Offset 0x175e
    mov        al, bl
    ret
Func0x175c ENDP

Func0x1775 PROC NEAR                    ;Offset 0x1775
    push       cx
    mov        dx, word ptr [si]
    xchg       dl, dh
    mov        cx, 0008h
Label0x177d:                            ;Offset 0x177d
    shl        dx, 01h
    jb         Label0x1784              ;Offset 0x1784
    jns        Label0x1784              ;Offset 0x1784
    stc
Label0x1784:                            ;Offset 0x1784
    rcl        al, 01h
    shl        dx, 01h
    loop       Label0x177d              ;Offset 0x177d
    stosb
    xor        si, 2000h
    pop        cx
    ret
Func0x1775 ENDP

WriteCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x1791
    push       cx
    push       dx
    push       es
    mov        ah, bl
    push       ax
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x5ff3               ;Offset 0x5ff3
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x17ae              ;Offset 0x17ae
    cmp        al, BDA_DM_640x480_BW_Graphics;0x11
    je         Label0x17f1              ;Offset 0x17f1
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Func0x17f7               ;Offset 0x17f7
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x17ae:                            ;Offset 0x17ae
    mov        es, dx
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2650               ;Offset 0x2650
    add        di, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x17d4              ;Offset 0x17d4
    pop        ax
    rep stosw
    pop        es
    pop        dx
    pop        cx
    ret
Label0x17d4:                            ;Offset 0x17d4
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a
    pop        si
Label0x17dc:                            ;Offset 0x17dc
    in         al, dx
    shr        al, 01h
    jb         Label0x17dc              ;Offset 0x17dc
    cli
Label0x17e2:                            ;Offset 0x17e2
    in         al, dx
    shr        al, 01h
    jae        Label0x17e2              ;Offset 0x17e2
    mov        ax, si
    stosw
    sti
    loop       Label0x17dc              ;Offset 0x17dc
    pop        es
    pop        dx
    pop        cx
    ret
Label0x17f1:                            ;Offset 0x17f1
    and        bl, 80h
    or         bl, 0fh
WriteCharacterAndAttributeAtCursorPosition ENDP
;continue!
Func0x17f7 PROC NEAR                    ;Offset 0x17f7
    ja         Label0x1836              ;Offset 0x1836
    mov        es, dx
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x265e               ;Offset 0x265e
    mov        di, ax
    mov        dl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop        ax
    or         al, al
    jns        Label0x1814              ;Offset 0x1814
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    and        al, 7fh
    jmp        Label0x1818              ;Offset 0x1818
Label0x1814:                            ;Offset 0x1814
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
Label0x1818:                            ;Offset 0x1818
    cbw
    shl        ax, 01h
    shl        ax, 01h
    shl        ax, 01h
    add        si, ax
    cmp        dl, BDA_DM_640x200_BW_Graphics;0x6
    je         Label0x182f              ;Offset 0x182f
    shl        di, 01h
    push       bx
    call       Func0x1a0c               ;Offset 0x1a0c
    pop        bx
    jmp        Label0x1832              ;Offset 0x1832
Label0x182f:                            ;Offset 0x182f
    call       Func0x19a8               ;Offset 0x19a8
Label0x1832:                            ;Offset 0x1832
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1836:                            ;Offset 0x1836
    mov        dh, 0a0h                 ;Segment 0xa000
    mov        es, dx
    cmp        al, 13h
    jne        Label0x1841              ;Offset 0x1841
    jmp        Label0x1942              ;Offset 0x1942
Label0x1841:                            ;Offset 0x1841
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x266e               ;Offset 0x266e
    add        di, ax
    pop        ax
    push       bp
    push       cx
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    xor        ch, ch
    call       Func0xc05                ;Offset 0xc05
    jb         Label0x187a              ;Offset 0x187a
    jne        Label0x1872              ;Offset 0x1872
    or         ch, 02h
Label0x1872:                            ;Offset 0x1872
    mov        cl, 10h
    mul        cl
    mov        cl, 0eh
    jmp        Label0x187c              ;Offset 0x187c
Label0x187a:                            ;Offset 0x187a
    mul        cl
Label0x187c:                            ;Offset 0x187c
    add        si, ax
    or         bl, bl
    js         Label0x18f2              ;Offset 0x18f2
    mov        al, 52h
    push       cx
    push       cx
    xor        ch, ch
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out        dx, ax
    xor        ax, ax
    push       di
Label0x1892:                            ;Offset 0x1892
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1892              ;Offset 0x1892
    pop        di
    mov        al, VGA_SEQIdx_MapMask    ;0x2
    mov        ah, bl
    out        dx, ax
    mov        dx, di
    pop        cx
    and        ch, 02h
    je         Label0x18c1              ;Offset 0x18c1
    xor        ch, ch
    mov        cl, 08h
Label0x18ab:                            ;Offset 0x18ab
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18ab              ;Offset 0x18ab
    mov        cl, 07h
    inc        si
Label0x18b6:                            ;Offset 0x18b6
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18b6              ;Offset 0x18b6
    inc        di
    jmp        Label0x18c9              ;Offset 0x18c9
Label0x18c1:                            ;Offset 0x18c1
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18c1              ;Offset 0x18c1
Label0x18c9:                            ;Offset 0x18c9
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x18d4              ;Offset 0x18d4
    mov        cx, 0010h
Label0x18d4:                            ;Offset 0x18d4
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    ;jne        Label0x1883              ;Offset 0x1883 !!!!!!!!!!!
    DB 075h, 0A4h                       ;Generates a broken jump (target = middle of instruction)
Label0x18df:                            ;Offset 0x18df
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
Label0x18f1:                            ;Offset 0x18f1
    push       dx
Label0x18f2:                            ;Offset 0x18f2
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
    je         Label0x1922              ;Offset 0x1922
    xor        ch, ch
    mov        cl, 08h
Label0x190c:                            ;Offset 0x190c
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x190c              ;Offset 0x190c
    mov        cl, 07h
    inc        si
Label0x1917:                            ;Offset 0x1917
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1917              ;Offset 0x1917
    inc        di
    jmp        Label0x192a              ;Offset 0x192a
Label0x1922:                            ;Offset 0x1922
    lodsb
    xchg       byte ptr es:[di], al
    add        di, bp
    loop       Label0x1922              ;Offset 0x1922
Label0x192a:                            ;Offset 0x192a
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x1935              ;Offset 0x1935
    mov        cx, 0010h
Label0x1935:                            ;Offset 0x1935
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    jne        Label0x18f1              ;Offset 0x18f1
    jmp        Label0x18df              ;Offset 0x18df
Label0x1942:                            ;Offset 0x1942
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2680               ;Offset 0x2680
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
Label0x1975:                            ;Offset 0x1975
    lodsb
    push       di
    push       cx
    mov        cx, 0008h
Label0x197b:                            ;Offset 0x197b
    rcl        al, 01h
    jae        Label0x1984              ;Offset 0x1984
    mov        byte ptr es:[di], bl
    jmp        Label0x1987              ;Offset 0x1987
Label0x1984:                            ;Offset 0x1984
    mov        byte ptr es:[di], bh
Label0x1987:                            ;Offset 0x1987
    inc        di
    loop       Label0x197b              ;Offset 0x197b
    pop        cx
    pop        di
    add        di, bp
    loop       Label0x1975              ;Offset 0x1975
    pop        cx
    sub        si, cx
    mov        di, dx
    add        di, 0008h
    pop        dx
    dec        dx
    ;jne        Label0x1971              ;Offset 0x1971 !!!!!!!!!!!!!!
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
Func0x17f7 ENDP

Func0x19a8 PROC NEAR                    ;Offset 0x19a8
    or        bl, bl
    js        Label0x19dc               ;Offset 0x19dc
Label0x19ac:                            ;Offset 0x19ac
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
    loop      Label0x19ac               ;Offset 0x19ac
    ret
Label0x19dc:                            ;Offset 0x19dc
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
    loop      Label0x19dc               ;Offset 0x19dc
    ret
Func0x19a8 ENDP

Func0x1a0c PROC NEAR                    ;Offset 0x1a0c
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
    js        Label0x1a4e               ;Offset 0x1a4e
Label0x1a2b:                            ;Offset 0x1a2b
    mov       bh, 04h
Label0x1a2d:                            ;Offset 0x1a2d
    lodsb
    call      Func0x1a71                ;Offset 0x1a71
    mov       word ptr es:[di], ax
    lodsb
    call      Func0x1a71                ;Offset 0x1a71
    mov       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a2d               ;Offset 0x1a2d
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a2b               ;Offset 0x1a2b
    ret
Label0x1a4e:                            ;Offset 0x1a4e
    mov       bh, 04h
Label0x1a50:                            ;Offset 0x1a50
    lodsb
    call      Func0x1a71                ;Offset 0x1a71
    xor       word ptr es:[di], ax
    lodsb
    call      Func0x1a71                ;Offset 0x1a71
    xor       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a50               ;Offset 0x1a50
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a4e               ;Offset 0x1a4e
    ret
Func0x1a0c ENDP

Func0x1a71 PROC NEAR                    ;Offset 0x1a71
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
Func0x1a71 ENDP

WriteCharacterOnlyAtCursorPosition PROC NEAR;Offset 0x1a9a
    push      cx
    push      dx
    push      es
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x5ff3                ;Offset 0x5ff3
    mov       dx, 0b800h                ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1ab1               ;Offset 0x1ab1
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x1af8               ;Offset 0x1af8
    mov       dh, 0b0h                  ;Segment 0xb000
Label0x1ab1:                            ;Offset 0x1ab1
    mov       es, dx
    mov       al, bh
    cbw
    mov       si, ax
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       di, ax
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call      Func0x2650                ;Offset 0x2650
    add       di, ax
    pop       ax
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x1ad9               ;Offset 0x1ad9
Label0x1ad1:                            ;Offset 0x1ad1
    stosb
    inc       di
    loop      Label0x1ad1               ;Offset 0x1ad1
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1ad9:                            ;Offset 0x1ad9
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a VGA_InputStatus1
    mov       ah, al
Label0x1ae2:                            ;Offset 0x1ae2
    in        al, dx
    shr       al, 01h
    jb        Label0x1ae2               ;Offset 0x1ae2
    cli
Label0x1ae8:                            ;Offset 0x1ae8
    in        al, dx
    shr       al, 01h
    jae       Label0x1ae8               ;Offset 0x1ae8
    mov       al, ah
    stosb
    sti
    inc       di
    loop      Label0x1ae2               ;Offset 0x1ae2
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1af8:                            ;Offset 0x1af8
    jmp       Func0x17f7                ;Offset 0x17f7
WriteCharacterOnlyAtCursorPosition ENDP

SetBorderColorOrPalette PROC NEAR       ;Offset 0x1afb
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexB_lowbyte;Offset 0x463 0xb4
    je        Label0x1b0b               ;Offset 0x1b0b
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x1b0c               ;Offset 0x1b0c
    int       42h
Label0x1b0b:                            ;Offset 0x1b0b
    ret
Label0x1b0c:                            ;Offset 0x1b0c
    push      bx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    xchg      ax, si
    or        bh, bh
    jne       Label0x1b65               ;Offset 0x1b65
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
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1b4b               ;Offset 0x1b4b
    mov       al, bl
    xor       ah, ah
    call      Func0x2ae5                ;Offset 0x2ae5
    or        si, si
    je        Label0x1b4b               ;Offset 0x1b4b
    mov       byte ptr es:[di], al
    jmp       Label0x1b4b               ;Offset 0x1b4b
Label0x1b4b:                            ;Offset 0x1b4b
    mov       al, bl
    mov       ah, 11h
    call      Func0x2ae5                ;Offset 0x2ae5
    or        si, si
    je        Label0x1b5a               ;Offset 0x1b5a
    mov       byte ptr es:[di + 10h], al
Label0x1b5a:                            ;Offset 0x1b5a
    mov       bl, bh
    and       bl, 20h
    shl       bl, 01h
    shl       bl, 01h
    rol       bl, 01h
Label0x1b65:                            ;Offset 0x1b65
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1b8f               ;Offset 0x1b8f
    mov       al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       al, 0dfh
    and       bl, 01h
    je        Label0x1b7b               ;Offset 0x1b7b
    or        al, 20h
Label0x1b7b:                            ;Offset 0x1b7b
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    and       al, 10h
    or        al, 02h
    or        al, bl
    mov       ah, 01h
    call      Func0x1b95                ;Offset 0x1b95
    call      Func0x1b95                ;Offset 0x1b95
    call      Func0x1b95                ;Offset 0x1b95
Label0x1b8f:                            ;Offset 0x1b8f
    call      Func0x2ad5                ;Offset 0x2ad5
    pop       es
    pop       bx
    ret
SetBorderColorOrPalette ENDP

Func0x1b95 PROC NEAR                    ;Offset 0x1b95
    call      Func0x2ae5                ;Offset 0x2ae5
    or        si, si
    je        Label0x1ba0               ;Offset 0x1ba0
    inc       di
    mov       byte ptr es:[di], al
Label0x1ba0:                            ;Offset 0x1ba0
    add       ax, 0102h
    ret
Func0x1b95 ENDP

WriteGraphicsPixel PROC NEAR            ;Offset 0x1ba4
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1c0c               ;Offset 0x1c0c
    push      cx
    push      dx
    push      ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1c23               ;Offset 0x1c23
    mov       dh, al
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1bc7               ;Offset 0x1bc7
    or        ah, 20h
Label0x1bc7:                            ;Offset 0x1bc7
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    mov       ah, dh
    jb        Label0x1be3               ;Offset 0x1be3
    shr       si, 01h
    mov       al, 80h
    jmp       Label0x1be9               ;Offset 0x1be9
Label0x1be3:                            ;Offset 0x1be3
    shl       cl, 01h
    mov       al, 0c0h
    ror       ah, 01h
Label0x1be9:                            ;Offset 0x1be9
    add       di, si
    ror       ah, 01h
    and       cl, 07h
    and       ah, al
    shr       ax, cl
    mov       cl, byte ptr [di]
    shl       dh, 01h
    jb        Label0x1c04               ;Offset 0x1c04
    not       al
    and       cl, al
    or        cl, ah
    mov       byte ptr [di], cl
    jmp       Label0x1c08               ;Offset 0x1c08
Label0x1c04:                            ;Offset 0x1c04
    xor       cl, ah
    mov       byte ptr [di], cl
Label0x1c08:                            ;Offset 0x1c08
    pop       ax
    pop       dx
    pop       cx
    ret
Label0x1c0c:                            ;Offset 0x1c0c
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
Label0x1c23:                            ;Offset 0x1c23
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
    jns       Label0x1c5c               ;Offset 0x1c5c
    mov       ax, (VGA_GCTL3_FuncSourceXORLatched SHL 8) OR VGA_GCTLIdx_DataRotate;0x1803
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    jmp       Label0x1c66               ;Offset 0x1c66
Label0x1c5c:                            ;Offset 0x1c5c
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    xor       al, al
    xchg      byte ptr [di], al
Label0x1c66:                            ;Offset 0x1c66
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

ReadGraphicsPixel PROC NEAR             ;Offset 0x1c81
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1cda               ;Offset 0x1cda
    push      cx
    push      dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1cf0               ;Offset 0x1cf0
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1ca1               ;Offset 0x1ca1
    or        ah, 20h
Label0x1ca1:                            ;Offset 0x1ca1
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    jb        Label0x1cc6               ;Offset 0x1cc6
    shr       si, 01h
    add       di, si
    and       cl, 07h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    and       al, 01h
    jmp       Label0x1cd7               ;Offset 0x1cd7
Label0x1cc6:                            ;Offset 0x1cc6
    add       di, si
    and       cl, 03h
    shl       cl, 01h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    rol       al, 01h
    and       al, 03h
Label0x1cd7:                            ;Offset 0x1cd7
    pop       dx
    pop       cx
    ret
Label0x1cda:                            ;Offset 0x1cda
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
Label0x1cf0:                            ;Offset 0x1cf0
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
Label0x1d1e:                            ;Offset 0x1d1e
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, ax
    mov       al, byte ptr [di]
    and       al, ch
    rol       bl, 01h
    or        bl, al
    dec       ah
    jns       Label0x1d1e               ;Offset 0x1d1e
    inc       cx
    rol       bl, cl
    mov       al, bl
    mov       bx, si
    pop       dx
    pop       cx
    ret
ReadGraphicsPixel ENDP

TeletypeOutput PROC NEAR                ;Offset 0x1d37
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
    jbe       Func0x1d6b                ;Offset 0x1d6b
TeletypeOutput ENDP
;continue!
Func0x1d4e PROC NEAR                    ;Offset 0x1d4e
    mov       cx, 0001h
    push      ds
    call      WriteCharacterOnlyAtCursorPosition;Offset 0x1a9a
    pop       ds
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    je        Func0x1d8b                ;Offset 0x1d8b
Func0x1d4e ENDP
;continue!
Func0x1d5d PROC NEAR                    ;Offset 0x1d5d
    call      SetCursorPosition         ;Offset 0x10d9
Func0x1d5d ENDP
;continue!
Func0x1d60 PROC NEAR                    ;Offset 0x1d60
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0x1d60 ENDP

Func0x1d65 PROC NEAR                    ;Offset 0x1d65
    push      ax
    push      bx
    push      cx
    push      dx
    cmp       al, 0dh
Func0x1d65 ENDP
;continue!
Func0x1d6b PROC NEAR                    ;Offset 0x1d6b
    je        Label0x1d80               ;Offset 0x1d80
    cmp       al, 0ah
    je        Func0x1d8d                ;Offset 0x1d8d
    cmp       al, 08h
    je        Label0x1d84               ;Offset 0x1d84
    cmp       al, 07h
    jne       Func0x1d4e                ;Offset 0x1d4e
    mov       al, 25h
    call      Beep                      ;Offset 0x2bea
    jmp       Func0x1d60                ;Offset 0x1d60
Label0x1d80:                            ;Offset 0x1d80
    xor       dl, dl
    jmp       Func0x1d5d                ;Offset 0x1d5d
Label0x1d84:                            ;Offset 0x1d84
    or        dl, dl
    je        Func0x1d5d                ;Offset 0x1d5d
    dec       dx
    jmp       Func0x1d5d                ;Offset 0x1d5d
Func0x1d6b ENDP

Func0x1d8b PROC NEAR                    ;Offset 0x1d8b
    xor       dl, dl
Func0x1d8b ENDP
;continue!
Func0x1d8d PROC NEAR                    ;Offset 0x1d8d
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Func0x1d5d                ;Offset 0x1d5d
    dec       dh
    call      SetCursorPosition         ;Offset 0x10d9
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x5ff3                ;Offset 0x5ff3
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1da9               ;Offset 0x1da9
    cbw
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x1dae               ;Offset 0x1dae
Label0x1da9:                            ;Offset 0x1da9
    push      ds
    call      ReadCharacterAndAttributeAtCursorPosition;Offset 0x15c4
    pop       ds
Label0x1dae:                            ;Offset 0x1dae
    mov       bh, ah
    xor       cx, cx
    mov       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec       dx
    mov       al, 01h
    call      ScrollUpWindow            ;Offset 0x1162
    jmp       Func0x1d60                ;Offset 0x1d60
Func0x1d8d ENDP

GetCurrentVideoMode PROC NEAR           ;Offset 0x1dbe
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, BDA_VMO_DontClearDisplay;0x80
    or        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       ah, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    ret
GetCurrentVideoMode ENDP

RAMDACFunctions PROC NEAR               ;Offset 0x1dd0
    mov       ah, 1bh
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;misassemble
    ja        EmptyFunction             ;Offset 0x1de4
    cbw
    mov       si, ax
    shl       si, 01h
    push      es
    push      bx
    call      word ptr cs:[si + RAMDACCallTable];Offset 0x1dea
    pop       bx
    pop       es
RAMDACFunctions ENDP

EmptyFunction PROC NEAR                 ;Offset 0x1de4
    ret
EmptyFunction ENDP

Func0x1de5 PROC NEAR                    ;Offset 0x1de5
    add       sp, 04h
    pop       es
    ret
Func0x1de5 ENDP

;Offset 0x1dea
RAMDACCallTable         DW SetSinglePaletteRegister;Offset 0x1e22       ;0x00
                        DW SetBorderColor;Offset 0x1e3f                 ;0x01
                        DW SetAllPaletteRegisters;Offset 0x1e43         ;0x02
                        DW ToggleIntensityBlinkingBit;Offset 0x1e75     ;0x03
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW GetIndividualPaletteRegister;Offset 0x1e97   ;0x07
                        DW GetBorderColor;Offset 0x1ea1                 ;0x08
                        DW GetAllPaletteRegisters;Offset 0x1ea5         ;0x09
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW SetIndividualDACRegister;Offset 0x1eab       ;0x10
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW SetBlockOfDACRegisters;Offset 0x1ed8         ;0x12
                        DW SelectVideoDACColorPage;Offset 0x1ef1        ;0x13
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW GetIndividualDACRegister;Offset 0x1f17       ;0x15
                        DW EmptyFunction;Offset 0x1de4                  ;
                        DW GetBlockOfDACRegisters;Offset 0x1f36         ;0x17
                        DW SetPELMask   ;Offset 0x1f50                  ;0x18
                        DW GetPELMask   ;Offset 0x1f58                  ;0x19
                        DW GetVideoDACColorPageState;Offset 0x1f65      ;0x1a
                        DW PerformGrayScaleSumming;Offset 0x1f82        ;0x1b

SetSinglePaletteRegister PROC NEAR      ;Offset 0x1e22
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Func0x1e3e                ;Offset 0x1e3e
SetSinglePaletteRegister ENDP
;continue!
Func0x1e29 PROC NEAR                    ;Offset 0x1e29
    mov       al, bh
    mov       ah, bl
    call      Func0x2ae5                ;Offset 0x2ae5
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je        Func0x1e3e                ;Offset 0x1e3e
    mov       al, bh
    xor       bh, bh
    mov       byte ptr es:[bx + di], al
Func0x1e29 ENDP
;continue!
Func0x1e3e PROC NEAR                    ;Offset 0x1e3e
    ret
Func0x1e3e ENDP

SetBorderColor PROC NEAR                ;Offset 0x1e3f
    mov       bl, 11h
    jmp       Func0x1e29                ;Offset 0x1e29
SetBorderColor ENDP

SetAllPaletteRegisters PROC NEAR        ;Offset 0x1e43
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1e74               ;Offset 0x1e74
    push      cx
    push      dx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je        Label0x1e5f               ;Offset 0x1e5f
    pop       ds
    push      ds
    mov       si, dx
    mov       cx, 0011h
    rep movsb
    mov       ds, cx
Label0x1e5f:                            ;Offset 0x1e5f
    pop       es
    mov       si, dx
    mov       ax, 1000h
    mov       cx, 0011h
    call      Func0x2bca                ;Offset 0x2bca
    call      Func0x28c7                ;Offset 0x28c7
    mov       al, 20h
    out       dx, al
    sti
    pop       dx
    pop       cx
Label0x1e74:                            ;Offset 0x1e74
    ret
SetAllPaletteRegisters ENDP

ToggleIntensityBlinkingBit PROC NEAR    ;Offset 0x1e75
    mov       ah, 10h
    call      Func0x2af9                ;Offset 0x2af9
    cmp       bl, 01h
    ja        Label0x1e91               ;Offset 0x1e91
    and       al, 0f7h
    and       byte ptr ds:[BDA_CRTModeControlRegValue], NOT BDA_CMCRV_Blinking;Offset 0x465 0xdf
    or        bl, bl
    je        Label0x1e91               ;Offset 0x1e91
    or        al, 08h
    or        byte ptr ds:[BDA_CRTModeControlRegValue], BDA_CMCRV_Blinking;Offset 0x465 0x20
Label0x1e91:                            ;Offset 0x1e91
    mov       ah, 10h
    call      Func0x2ae5                ;Offset 0x2ae5
    ret
ToggleIntensityBlinkingBit ENDP

GetIndividualPaletteRegister PROC NEAR  ;Offset 0x1e97
    mov       ah, bl
GetIndividualPaletteRegister ENDP
;continue!
Func0x1e99 PROC NEAR                    ;Offset 0x1e99
    call      Func0x2af9                ;Offset 0x2af9
    mov       bh, al
    jmp       Func0x1de5                ;Offset 0x1de5
Func0x1e99 ENDP

GetBorderColor PROC NEAR                ;Offset 0x1ea1
    mov       ah, 11h
    jmp       Func0x1e99                ;Offset 0x1e99
GetBorderColor ENDP

GetAllPaletteRegisters PROC NEAR        ;Offset 0x1ea5
    mov       di, dx
    call      Func0x28dd                ;Offset 0x28dd
    ret
GetAllPaletteRegisters ENDP

SetIndividualDACRegister PROC NEAR      ;Offset 0x1eab
    push      cx
    push      dx
    mov       ah, dh
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    je        Label0x1ebb               ;Offset 0x1ebb
    call      Func0x29d8                ;Offset 0x29d8
    mov       cx, ax
Label0x1ebb:                            ;Offset 0x1ebb
    call      Func0x2bd8                ;Offset 0x2bd8
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    jmp       Label0x1ec6               ;Offset 0x1ec6 Speed sensitive!
Label0x1ec6:                            ;Offset 0x1ec6
    inc       dx
    mov       al, ah
    out       dx, al
    jmp       Label0x1ecc               ;Offset 0x1ecc Speed sensitive!
Label0x1ecc:                            ;Offset 0x1ecc
    mov       al, ch
    out       dx, al
    jmp       Label0x1ed1               ;Offset 0x1ed1 Speed sensitive!
Label0x1ed1:                            ;Offset 0x1ed1
    mov       al, cl
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
SetIndividualDACRegister ENDP

SetBlockOfDACRegisters PROC NEAR        ;Offset 0x1ed8
    push      cx
    push      dx
    mov       si, dx
    call      TurnScreenOff             ;Offset 0x2b15
    push      ax
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    inc       dx
    call      Func0x299f                ;Offset 0x299f
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b17
    pop       dx
    pop       cx
    ret
SetBlockOfDACRegisters ENDP

SelectVideoDACColorPage PROC NEAR       ;Offset 0x1ef1
    mov       ah, 10h
    call      Func0x2af9                ;Offset 0x2af9
    or        bl, bl
    jne       Label0x1f05               ;Offset 0x1f05
    shl       al, 01h
    add       bh, 0ffh
    rcr       al, 01h
    call      Func0x2ae5                ;Offset 0x2ae5
    ret
Label0x1f05:                            ;Offset 0x1f05
    or        al, al
    mov       al, bh
    js        Label0x1f0f               ;Offset 0x1f0f
    shl       al, 01h
    shl       al, 01h
Label0x1f0f:                            ;Offset 0x1f0f
    and       al, 0fh
    mov       ah, 14h
    call      Func0x2ae5                ;Offset 0x2ae5
    ret
SelectVideoDACColorPage ENDP

GetIndividualDACRegister PROC NEAR      ;Offset 0x1f17
    mov       al, bl
    mov       bl, dl
    call      Func0x2bd8                ;Offset 0x2bd8
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    out       dx, al
    jmp       Label0x1f24               ;Offset 0x1f24 Speed sensitive!
Label0x1f24:                            ;Offset 0x1f24
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1f29               ;Offset 0x1f29 Speed sensitive!
Label0x1f29:                            ;Offset 0x1f29
    mov       bh, al
    in        al, dx
    jmp       Label0x1f2e               ;Offset 0x1f2e Speed sensitive!
Label0x1f2e:                            ;Offset 0x1f2e
    mov       ah, al
    in        al, dx
    sti
    xchg      ax, cx
    mov       dx, bx
    ret
GetIndividualDACRegister ENDP

GetBlockOfDACRegisters PROC NEAR        ;Offset 0x1f36
    push      cx
    push      dx
    mov       di, dx
    call      TurnScreenOff             ;Offset 0x2b15
    push      ax
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    call      Func0x29cf                ;Offset 0x29cf
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b17
    pop       dx
    pop       cx
    ret
GetBlockOfDACRegisters ENDP

SetPELMask PROC NEAR                    ;Offset 0x1f50
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    xchg      ax, bx
    out       dx, al
    pop       dx
    ret
SetPELMask ENDP

GetPELMask PROC NEAR                    ;Offset 0x1f58
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    in        al, dx
    pop       dx
    xor       ah, ah
    xchg      ax, bx
    jmp       Func0x1de5                ;Offset 0x1de5
    ret
GetPELMask ENDP

GetVideoDACColorPageState PROC NEAR     ;Offset 0x1f65
    mov       ah, 14h
    call      Func0x2af9                ;Offset 0x2af9
    and       al, 0fh
    mov       bh, al
    mov       ah, 10h
    call      Func0x2af9                ;Offset 0x2af9
    rol       al, 01h
    and       al, 01h
    mov       bl, al
    jne       Label0x1f7f               ;Offset 0x1f7f
    shr       bh, 01h
    shr       bh, 01h
Label0x1f7f:                            ;Offset 0x1f7f
    jmp       Func0x1de5                ;Offset 0x1de5
GetVideoDACColorPageState ENDP

PerformGrayScaleSumming PROC NEAR       ;Offset 0x1f82
    push      cx
    push      dx
    call      TurnScreenOff             ;Offset 0x2b15
    push      ax
Label0x1f88:                            ;Offset 0x1f88
    push      cx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    jmp       Label0x1f91               ;Offset 0x1f91 Speed sensitive!
Label0x1f91:                            ;Offset 0x1f91
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1f96               ;Offset 0x1f96 Speed sensitive!
Label0x1f96:                            ;Offset 0x1f96
    mov       bh, al
    in        al, dx
    jmp       Label0x1f9b               ;Offset 0x1f9b Speed sensitive!
Label0x1f9b:                            ;Offset 0x1f9b
    mov       ah, al
    in        al, dx
    jmp       Label0x1fa0               ;Offset 0x1fa0 Speed sensitive!
Label0x1fa0:                            ;Offset 0x1fa0
    xchg      ax, cx
    dec       dx
    mov       al, bl
    out       dx, al
    mov       dh, bh
    call      Func0x29d8                ;Offset 0x29d8
    mov       dx, VGA_RAMDACData        ;Port 0x3c9
    out       dx, al
    jmp       Label0x1fb0               ;Offset 0x1fb0 Speed sensitive!
Label0x1fb0:                            ;Offset 0x1fb0
    out       dx, al
    jmp       Label0x1fb3               ;Offset 0x1fb3 Speed sensitive!
Label0x1fb3:                            ;Offset 0x1fb3
    out       dx, al
    inc       bx
    pop       cx
    loop      Label0x1f88               ;Offset 0x1f88
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b17
    pop       dx
    pop       cx
    ret
PerformGrayScaleSumming ENDP

TextFunctions PROC NEAR                 ;Offset 0x1fbf
    lea       si, [TextFunctions0X]     ;Offset 0x1fec
    cmp       al, 10h
    jb        Label0x1fdd               ;Offset 0x1fdd
    lea       si, [TextFunctions1X]     ;Offset 0x200d
    cmp       al, 20h
    jb        Label0x1fdd               ;Offset 0x1fdd
    lea       si, [TextFunctions2X]     ;Offset 0x2028
    cmp       al, 30h
    jb        Label0x1fdd               ;Offset 0x1fdd
    jne       Label0x1feb               ;Offset 0x1feb
    call      GetFontInformation        ;Offset 0x208c
    ret
Label0x1fdd:                            ;Offset 0x1fdd
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
Label0x1feb:                            ;Offset 0x1feb
    ret
TextFunctions ENDP

TextFunctions0X PROC NEAR               ;Offset 0x1fec
    cmp       al, 03h
    je        SelectCharacterMap        ;Offset 0x2004
    cmp       al, 04h
    ja        Return                    ;Offset 0x200c
    and       bl, 7fh
    call      Func0x26e5                ;Offset 0x26e5
    call      Func0x2726                ;Offset 0x2726
    call      Func0x26eb                ;Offset 0x26eb
    call      Func0x2ad5                ;Offset 0x2ad5
    ret
SelectCharacterMap:                     ;Offset 0x2004
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    mov       ah, bl
    out       dx, ax
Return:                                 ;Offset 0x200c
    ret
TextFunctions0X ENDP

TextFunctions1X PROC NEAR               ;Offset 0x200d
    cmp       al, 03h
    je        Label0x2027               ;Offset 0x2027
    cmp       al, 04h
    ja        Label0x2027               ;Offset 0x2027
    and       bl, 7fh
    call      Func0x26e5                ;Offset 0x26e5
    call      Func0x2726                ;Offset 0x2726
    call      Func0x27c8                ;Offset 0x27c8
    call      Func0x26eb                ;Offset 0x26eb
    call      Func0x2ad5                ;Offset 0x2ad5
Label0x2027:                            ;Offset 0x2027
    ret
TextFunctions1X ENDP

TextFunctions2X PROC NEAR               ;Offset 0x2028
    cmp       al, 04h
    ja        Label0x208b               ;Offset 0x208b
    cbw
    push      ax
    dec       ax
    je        Label0x2058               ;Offset 0x2058
    jns       Label0x203d               ;Offset 0x203d
    pop       ax
    mov       word ptr ds:[INT_1F_HandlerOfs], bp;Offset 0x7c
    mov       word ptr ds:[INT_1F_HandlerSeg], es;Offset 0x7e
    ret
Label0x203d:                            ;Offset 0x203d
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x38f0
    mov       cx, 000eh
    dec       ax
    je        Label0x2058               ;Offset 0x2058
    lea       bp, [Font8x8]             ;Offset 0x30f0
    mov       cl, 08h
    dec       ax
    je        Label0x2058               ;Offset 0x2058
    lea       bp, [Font8x16]            ;Offset 0x38f0
    mov       cl, 10h
Label0x2058:                            ;Offset 0x2058
    mov       word ptr ds:[INT_43_HandlerOfs], bp;Offset 0x10c
    mov       word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    pop       ax
    cmp       al, 01h
    je        Label0x206f               ;Offset 0x206f
    cmp       cl, 0eh
    jne       Label0x206f               ;Offset 0x206f
    mov       ax, 0bd70h
    int       6dh
Label0x206f:                            ;Offset 0x206f
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    mov       al, dl
    and       bx, 00ffh
    je        Label0x2087               ;Offset 0x2087
    mov       al, 19h
    cmp       bl, 03h
    ja        Label0x2087               ;Offset 0x2087
    mov       al, byte ptr cs:[bx + RowsOnScreen];Offset 0x30da
Label0x2087:                            ;Offset 0x2087
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x208b:                            ;Offset 0x208b
    ret
TextFunctions2X ENDP

GetFontInformation PROC NEAR            ;Offset 0x208c
    mov       cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov       al, bh
    sub       al, 02h
    cbw
    jb        Label0x20b6               ;Offset 0x20b6
    cmp       al, 05h
    ja        Label0x20c2               ;Offset 0x20c2
    shl       ax, 01h
    mov       si, ax
    mov       bp, word ptr cs:[si + Data0x30de];Offset 0x30de
    push      cs
    pop       es
    or        ax, ax
    jne       Label0x20b5               ;Offset 0x20b5
    mov       ax, 0bd71h
    int       6dh
    xor       ax, ax
Label0x20b5:                            ;Offset 0x20b5
    ret
Label0x20b6:                            ;Offset 0x20b6
    inc       ax
    js        Label0x20be               ;Offset 0x20be
    les       bp, ds:[INT_43_HandlerOfs];Offset 0x10c
    ret
Label0x20be:                            ;Offset 0x20be
    les       bp, ds:[INT_1F_HandlerOfs];Offset 0x7c
Label0x20c2:                            ;Offset 0x20c2
    ret
GetFontInformation ENDP

AlternateFunctions PROC NEAR            ;Offset 0x20c3
    cmp       bl, 20h
    je        InstallAlternatePrintScreenHandler;Offset 0x20ef
    cmp       bl, 10h
    je        Label0x20cf               ;Offset 0x20cf
    jmp       Label0x20fc               ;Offset 0x20fc
Label0x20cf:                            ;Offset 0x20cf
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
InstallAlternatePrintScreenHandler:     ;Offset 0x20ef
    cli
    mov       word ptr ds:[INT_5_HandlerOfs], offset PrintScreenHandler;Offset 0x14 Offset 0x2c50
    mov       word ptr ds:[INT_5_HandlerSeg], cs;Offset 0x16
    sti
    ret
Label0x20fc:                            ;Offset 0x20fc
    push      bx
    push      cx
    push      dx
    mov       cl, 03h
    xor       bh, bh
    sub       bl, 30h
    shl       bx, 01h
    je        Label0x2115               ;Offset 0x2115
    cmp       bl, 0ah
    je        Label0x2116               ;Offset 0x2116
    cmp       bl, 0ch
    ja        Label0x2127               ;Offset 0x2127
    dec       cx
Label0x2115:                            ;Offset 0x2115
    dec       cx
Label0x2116:                            ;Offset 0x2116
    ;cmp       al, cl
    DB 3Ah, 0C1h                        ;misassembly
    ja        Label0x2127               ;Offset 0x2127
    call      word ptr cs:[bx + AlternateFunctionsTable];Offset 0x212c
    jb        Label0x2127               ;Offset 0x2127
    mov       al, 12h
Label0x2123:                            ;Offset 0x2123
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x2127:                            ;Offset 0x2127
    mov       al, 00h
    jmp       Label0x2123               ;Offset 0x2123
AlternateFunctions ENDP

    nop                                 ;hello

;Offset 0x212c
AlternateFunctionsTable DW SelectVerticalResolution;Offset 0x21b2
                        DW SetPaletteLoading;Offset 0x213a
                        DW SetVGAMemoryIOMapping;Offset 0x2148
                        DW SetGrayscaleSumming;Offset 0x2188
                        DW SetCursorEmulation;Offset 0x2196
                        DW DisplaySwitchInterface;Offset 0x21f6
                        DW TurnScreenOnOff;Offset 0x21a4

SetPaletteLoading PROC NEAR             ;Offset 0x213a
    je   Enabled                        ;Offset 0x2142
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0xf7
    ret
Enabled:                                ;Offset 0x2142
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
SetVGAMemoryIOMapping PROC NEAR         ;Offset 0x2148
    mov  dl, 00h
    je   Disable                        ;Offset 0x214e
    dec  dl
Disable:                                ;Offset 0x214e
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_Header_Word_Command    ;0x4
    call AccessPCIRegister              ;Offset 0x68f8
    and  cl, NOT PCI_Cmd_MemSpace       ;0xfd
    and  dl, PCI_Cmd_IOSpace OR PCI_Cmd_MemSpace;0x3
    je   DisableMemSpace                ;Offset 0x2166
    or   cl, dl
DisableMemSpace:                        ;Offset 0x2166
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_MGA_Option + 01h       ;0x41
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    and  cl, NOT (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0xfe
    and  dl, (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0x1
    je   DoWrite                        ;Offset 0x2180
    or   cl, dl                         ;OR in the new value (one)
DoWrite:                                ;Offset 0x2180
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    clc  
    ret
SetVGAMemoryIOMapping ENDP

SetGrayscaleSumming PROC NEAR           ;Offset 0x2188
    je   Disable                        ;Offset 0x2190
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0x489 0x2
    ret
Disable:                                ;Offset 0x2190
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489 0xfd
    ret
SetGrayscaleSumming ENDP

SetCursorEmulation PROC NEAR            ;Offset 0x2196
    je   Enable                         ;Offset 0x219e
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487 0xfe
    ret
Enable:                                 ;Offset 0x219e
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
TurnScreenOnOff PROC NEAR               ;Offset 0x21a4
    lea  dx, [TurnScreenOff]            ;Offset 0x2b15
    je   TurnOff                        ;Offset 0x21ae
    lea  dx, [TurnScreenOn]             ;Offset 0x2b11
TurnOff:                                ;Offset 0x21ae
    call dx
    clc
    ret
TurnScreenOnOff ENDP

SelectVerticalResolution PROC NEAR      ;Offset 0x21b2
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x21d7               ;Offset 0x21d7
    cbw
    xchg      ax, si
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, NOT BDA_VDDA_LineMode400;0xef
    test      al, BDA_VDDA_VGA          ;0x1
    je        Label0x21c6               ;Offset 0x21c6
    and       al, NOT BDA_VDDA_LineMode200;0x7f
Label0x21c6:                            ;Offset 0x21c6
    mov       bl, BDA_EFBS_MDAHiResEnhanced_2;0x9
    dec       si
    jns       Label0x21db               ;Offset 0x21db
    dec       bx
    test      al, BDA_VDDA_VGA          ;0x1
    jne       Label0x21d9               ;Offset 0x21d9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x21e4               ;Offset 0x21e4
Label0x21d7:                            ;Offset 0x21d7
    stc
    ret
Label0x21d9:                            ;Offset 0x21d9
    or        al, BDA_VDDA_LineMode200  ;0x80
Label0x21db:                            ;Offset 0x21db
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x21e4               ;Offset 0x21e4
    mov       bl, BDA_EFBS_CGAMono80x25_2;0xb
Label0x21e4:                            ;Offset 0x21e4
    dec       si
    js        Label0x21e9               ;Offset 0x21e9
    or        al, BDA_VDDA_LineMode400  ;0x10
Label0x21e9:                            ;Offset 0x21e9
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    ret
SelectVerticalResolution ENDP

DisplaySwitchInterface PROC NEAR        ;Offset 0x21f6
    cbw
    xchg      ax, si
    dec       si
    je        Label0x2248               ;Offset 0x2248
    js        Label0x2214               ;Offset 0x2214
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2248               ;Offset 0x2248
    dec       si
    je        Label0x2227               ;Offset 0x2227
    mov       si, dx
    call      RestoreBiosDataAreaVideoSections;Offset 0x25fc
    mov       al, 0eh
Label0x220e:                            ;Offset 0x220e
    mov       dx, 46e8h                 ;Port 0x46e8 Some kind of video subsystem enable port?
    out       dx, al
    clc
    ret
Label0x2214:                            ;Offset 0x2214
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    jne       Label0x2248               ;Offset 0x2248
    mov       ax, 1280h
    int       42h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x2248               ;Offset 0x2248
Label0x2227:                            ;Offset 0x2227
    mov       di, dx
    call      BackupBiosDataAreaVideoSections;Offset 0x2519
    push      es
    les       bx, ds:[INT_42_HandlerOfs];Offset 0x108
    mov       word ptr ds:[INT_6D_HandlerOfs], bx;Offset 0x1b4
    mov       word ptr ds:[INT_6D_HandlerSeg], es;Offset 0x1b6
    pop       es
    mov       word ptr ds:[INT_42_HandlerOfs], Int6DHandler;Offset 0x108 Offset 0xb70
    mov       word ptr ds:[INT_42_HandlerSeg], cs;Offset 0x10a
    mov       al, 06h
    jmp       Label0x220e               ;Offset 0x220e
Label0x2248:                            ;Offset 0x2248
    stc
    ret
DisplaySwitchInterface ENDP

WriteString PROC NEAR                   ;Offset 0x224a
    push      bx
    push      cx
    push      dx
    push      bp
    cmp       al, 03h
    ja        Label0x2290               ;Offset 0x2290
    jcxz      Label0x2290               ;Offset 0x2290
    cbw
    mov       di, ax
    dec       di
    mov       al, bh
    mov       si, ax
    shl       si, 01h
    push      word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    push      si
    call      SetCursorPosition         ;Offset 0x10d9
Label0x2266:                            ;Offset 0x2266
    mov       al, byte ptr es:[bp]
    inc       bp
    cmp       al, 0dh
    jbe       Label0x2295               ;Offset 0x2295
Label0x226f:                            ;Offset 0x226f
    or        di, di
    jle       Label0x2278               ;Offset 0x2278
    mov       bl, byte ptr es:[bp]
    inc       bp
Label0x2278:                            ;Offset 0x2278
    call      Func0x22c1                ;Offset 0x22c1
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae       Label0x22ae               ;Offset 0x22ae
Label0x2282:                            ;Offset 0x2282
    call      SetCursorPosition         ;Offset 0x10d9
Label0x2285:                            ;Offset 0x2285
    loop      Label0x2266               ;Offset 0x2266
    pop       si
    pop       dx
    shr       di, 01h
    jae       Label0x2290               ;Offset 0x2290
    call      SetCursorPosition         ;Offset 0x10d9
Label0x2290:                            ;Offset 0x2290
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x2295:                            ;Offset 0x2295
    je        Label0x22a3               ;Offset 0x22a3
    cmp       al, 0ah
    je        Label0x22a3               ;Offset 0x22a3
    cmp       al, 08h
    je        Label0x22a3               ;Offset 0x22a3
    cmp       al, 07h
    jne       Label0x226f               ;Offset 0x226f
Label0x22a3:                            ;Offset 0x22a3
    call      Func0x22ce                ;Offset 0x22ce
    pop       si
    push      si
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x2285               ;Offset 0x2285
Label0x22ae:                            ;Offset 0x22ae
    xor       dl, dl
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Label0x2282               ;Offset 0x2282
    dec       dh
    mov       al, 0ah
    call      Func0x22ce                ;Offset 0x22ce
    jmp       Label0x2282               ;Offset 0x2282
WriteString ENDP

Func0x22c1 PROC NEAR                    ;Offset 0x22c1
    push      cx
    push      di
    push      ds
    mov       cx, 0001h
    call      WriteCharacterAndAttributeAtCursorPosition;Offset 0x1791
    pop       ds
    pop       di
    pop       cx
    ret
Func0x22c1 ENDP

Func0x22ce PROC NEAR                    ;Offset 0x22ce
    push      di
    push      ds
    call      Func0x1d65                ;Offset 0x1d65
    pop       ds
    pop       di
    ret
Func0x22ce ENDP

NotImplemented PROC NEAR                ;Offset 0x22d6
    ret
NotImplemented ENDP

DisplayCombinationCodeFunctions PROC NEAR;Offset 0x22d7
    push      cx
    push      dx
    push      es
    push      ax
    or        al, al
    jne       Label0x2308               ;Offset 0x2308
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x6614
    je        Label0x2308               ;Offset 0x2308
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_Misc      ;0x3
    out       dx, al
    inc       dx
    in        al, dx
    and       al, MGA_CRTCEXT3_MGAModeEnable;0x80
    je        Label0x2308               ;Offset 0x2308
    push      ds
    xor       ax, ax
    mov       ds, ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    pop       ds
    ja        Label0x2308               ;Offset 0x2308
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x2308               ;Offset 0x2308
    pop       ax
    jmp       Label0x231a               ;Offset 0x231a
Label0x2308:                            ;Offset 0x2308
    pop       ax
    xor       ah, ah
    dec       ax
    js        Label0x2315               ;Offset 0x2315
    jne       Label0x231a               ;Offset 0x231a
    call      SetDisplayCombinationCodeIndex;Offset 0x2b2c
    jmp       Label0x2318               ;Offset 0x2318
Label0x2315:                            ;Offset 0x2315
    call      GetDisplayCombinationCode ;Offset 0x2b53
Label0x2318:                            ;Offset 0x2318
    mov       al, 1ah
Label0x231a:                            ;Offset 0x231a
    pop       es
    pop       dx
    pop       cx
    ret
DisplayCombinationCodeFunctions ENDP

FunctionalityAndStateInfo PROC NEAR     ;Offset 0x231e
    or        bx, bx
    je        Label0x2325               ;Offset 0x2325
    xor       al, al
    ret
Label0x2325:                            ;Offset 0x2325
    push      bx
    push      cx
    push      dx
    lea       ax, [StaticFunctionalityTable];Offset 0x372
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
    call      GetDisplayCombinationCode ;Offset 0x2b53
    xchg      ax, bx
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    push      ax
    lea       bx, [ColorsPerModePlusOne];Offset 0x2402
    xlatb     cs:[bx]
    dec       ax
    jns       ColorsUnder256            ;Offset 0x2357
    mov       ax, 0100h                 ;Set colors to 256 (mode 0x13)
ColorsUnder256:                         ;Offset 0x2357
    stosw
    pop       ax
    lea       bx, [NumberOfPagesPerMode];Offset 0x2416
    xlatb     cs:[bx]
    stosb
    call      GetNumberOfActiveScanlines;Offset 0x2a60
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
    call      Func0x23f1                ;Offset 0x23f1
    mov       al, 0ah
    call      Func0x23e8                ;Offset 0x23e8
    mov       al, 0ch
    call      Func0x23e8                ;Offset 0x23e8
    mov       al, 08h
    call      Func0x23e8                ;Offset 0x23e8
    mov       al, 04h
    call      Func0x23e8                ;Offset 0x23e8
    mov       al, 06h
    call      Func0x23e8                ;Offset 0x23e8
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

Func0x23e8 PROC NEAR                    ;Offset 0x23e8
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je        Label0x23f0               ;Offset 0x23f0
    inc       dx
Label0x23f0:                            ;Offset 0x23f0
    ret
Func0x23e8 ENDP

Func0x23f1 PROC NEAR                    ;Offset 0x23f1
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je        Label0x2401               ;Offset 0x2401
    mov       ax, cs
    mov       bx, es
    cmp       ax, bx
    je        Label0x2401               ;Offset 0x2401
    inc       dx
Label0x2401:                            ;Offset 0x2401
    ret
Func0x23f1 ENDP

;Offset 0x2402
ColorsPerModePlusOne    DB 011h, 011h, 011h, 011h, 005h, 005h, 003h, 001h, 001h, 001h
                        DB 001h, 001h, 001h, 011h, 011h, 001h, 011h, 003h, 011h, 000h

;Offset 0x2416
NumberOfPagesPerMode    DB 008h, 008h, 008h, 008h, 001h, 001h, 001h, 008h, 000h, 000h
                        DB 000h, 000h, 000h, 008h, 004h, 002h, 002h, 001h, 001h, 001h

SaveRestoreVideoState PROC NEAR         ;Offset 0x242a
    push      cx
    test      cl, 07h
    je        Label0x243b               ;Offset 0x243b
    cbw
    inc       ax
    dec       ax
    je        Label0x243f               ;Offset 0x243f
    dec       ax
    je        Label0x2465               ;Offset 0x2465
    dec       ax
    je        Label0x2481               ;Offset 0x2481
Label0x243b:                            ;Offset 0x243b
    mov       al, 00h
    pop       cx
    ret
Label0x243f:                            ;Offset 0x243f
    xor       ax, ax
    shr       cl, 01h
    jae       Label0x2447               ;Offset 0x2447
    add       al, 46h
Label0x2447:                            ;Offset 0x2447
    shr       cl, 01h
    jae       Label0x244d               ;Offset 0x244d
    add       al, 3ah
Label0x244d:                            ;Offset 0x244d
    shr       cl, 01h
    jae       Label0x2454               ;Offset 0x2454
    add       ax, 0303h
Label0x2454:                            ;Offset 0x2454
    add       al, 20h
    mov       cl, 06h
    ror       ax, cl
    add       ah, 0ffh
    adc       al, 00h
    cbw
    xchg      ax, bx
Label0x2461:                            ;Offset 0x2461
    mov       al, 1ch
    pop       cx
    ret
Label0x2465:                            ;Offset 0x2465
    push      dx
    lea       di, [bx + 20h]
    shr       cl, 01h
    jae       Label0x2470               ;Offset 0x2470
    call      Func0x249a                ;Offset 0x249a
Label0x2470:                            ;Offset 0x2470
    shr       cl, 01h
    jae       Label0x2477               ;Offset 0x2477
    call      Func0x2515                ;Offset 0x2515
Label0x2477:                            ;Offset 0x2477
    shr       cl, 01h
    jae       Label0x247e               ;Offset 0x247e
    call      Func0x2552                ;Offset 0x2552
Label0x247e:                            ;Offset 0x247e
    pop       dx
    jmp       Label0x2461               ;Offset 0x2461
Label0x2481:                            ;Offset 0x2481
    push      dx
    shr       cl, 01h
    jae       Label0x2489               ;Offset 0x2489
    call      Func0x2577                ;Offset 0x2577
Label0x2489:                            ;Offset 0x2489
    shr       cl, 01h
    jae       Label0x2490               ;Offset 0x2490
    call      Func0x25f8                ;Offset 0x25f8
Label0x2490:                            ;Offset 0x2490
    shr       cl, 01h
    jae       Label0x2497               ;Offset 0x2497
    call      Func0x2626                ;Offset 0x2626
Label0x2497:                            ;Offset 0x2497
    pop       dx
    jmp       Label0x2461               ;Offset 0x2461
SaveRestoreVideoState ENDP

Func0x249a PROC NEAR                    ;Offset 0x249a
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
    call      Func0x28ba                ;Offset 0x28ba
    mov       dl, VGA_MiscellaneousRead_lowbyte;Port 0x3cc
    in        al, dx
    stosb
    mov       dx, si
    mov       cl, 19h
    call      Func0x28b8                ;Offset 0x28b8
    mov       ah, 00h
    mov       cl, 14h
Label0x24d4:                            ;Offset 0x24d4
    call      Func0x2af9                ;Offset 0x2af9
    stosb
    inc       ah
    loop      Label0x24d4               ;Offset 0x24d4
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    call      Func0x28b8                ;Offset 0x28b8
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
Label0x24f9:                            ;Offset 0x24f9
    out       dx, al
    push      ax
    xchg      dx, si
    in        al, dx
    stosb
    xchg      dx, si
    pop       ax
    inc       ax
    cmp       al, 03h
    jbe       Label0x24f9               ;Offset 0x24f9
    mov       al, byte ptr es:[di - 0bh]
    out       dx, al
    mov       dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov       al, byte ptr es:[di - 3eh]
    out       dx, al
    pop       cx
    ret
Func0x249a ENDP

Func0x2515 PROC NEAR                    ;Offset 0x2515
    mov       word ptr es:[bx + 02h], di
Func0x2515 ENDP
;continue!
BackupBiosDataAreaVideoSections PROC NEAR;Offset 0x2519
    push      cx
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    lea       si, [BiosDataAreaVideoSections];Offset 0x2534
ProcessNextSection:                     ;Offset 0x2524
    lodsw     word ptr cs:[si]          ;Count
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2531
    lodsw     word ptr cs:[si]          ;Source address
    xchg      ax, si
    rep movsb
    xchg      ax, si
    jmp       ProcessNextSection        ;Offset 0x2524
Return:                                 ;Offset 0x2531
    pop       cx
    ret
BackupBiosDataAreaVideoSections ENDP

    nop                                 ;https://i.imgur.com/NGkg1Ip.jpeg

;Offset 0x2534
BiosDataAreaVideoSections   DW 001eh, BDA_DisplayMode;Offset 0x449
                            DW 0007h, BDA_RowsOnScreen;Offset 0x484
                            DW 0004h, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
                            DW 0004h, INT_5_HandlerOfs;Offset 0x014
                            DW 0004h, INT_1D_HandlerOfs;Offset 0x074
                            DW 0004h, INT_1F_HandlerOfs;Offset 0x07c
                            DW 0004h, INT_43_HandlerOfs;Offset 0x10c
                            DW 0000h

Func0x2552 PROC NEAR                    ;Offset 0x2552
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
    call      Func0x29cf                ;Offset 0x29cf
    pop       cx
    ret
Func0x2552 ENDP

Func0x2577 PROC NEAR                    ;Offset 0x2577
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
    call      Func0x2841                ;Offset 0x2841
    add       dl, 0006h
    mov       al, byte ptr es:[si - 1fh]
    out       dx, al
    xor       ax, ax
    mov       cx, 0010h
    call      Func0x28c5                ;Offset 0x28c5
    inc       ax
    inc       si
    mov       cl, 01h
    call      Func0x28c5                ;Offset 0x28c5
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
Func0x2577 ENDP

Func0x25f8 PROC NEAR                    ;Offset 0x25f8
    mov       si, word ptr es:[bx + 02h]
Func0x25f8 ENDP
;continue!
RestoreBiosDataAreaVideoSections PROC NEAR;Offset 0x25fc
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
    lea       dx, [BiosDataAreaVideoSections];Offset 0x2534
ProcessNextSection:                     ;Offset 0x2612
    xchg      si, dx
    lodsw     word ptr cs:[si]
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2622
    lodsw     word ptr cs:[si]
    xchg      ax, di
    xchg      si, dx
    rep movsb
    jmp       ProcessNextSection        ;Offset 0x2612
Return:                                 ;Offset 0x2622
    pop       es
    pop       ds
    pop       cx
    ret
RestoreBiosDataAreaVideoSections ENDP

Func0x2626 PROC NEAR                    ;Offset 0x2626
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
    call      Func0x299f                ;Offset 0x299f
    sub       si, 0303h
    lodsw     word ptr es:[si]
    xchg      al, ah
    shr       ah, 01h
    sbb       dl, 01h
    out       dx, al
    pop       cx
    ret
Func0x2626 ENDP

Func0x2650 PROC NEAR                    ;Offset 0x2650
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    xor       dh, dh
    add       ax, dx
    shl       ax, 01h
    ret
Func0x2650 ENDP

Func0x265e PROC NEAR                    ;Offset 0x265e
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    shl       ax, 01h
    shl       ax, 01h
    xor       dh, dh
    add       ax, dx
    ret
Func0x265e ENDP

Func0x266e PROC NEAR                    ;Offset 0x266e
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    pop       dx
    xor       dh, dh
    add       ax, dx
    ret
Func0x266e ENDP

Func0x2680 PROC NEAR                    ;Offset 0x2680
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
Func0x2680 ENDP

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
SetCursorEndStart PROC NEAR             ;Offset 0x2698
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

ClearScreen PROC NEAR                   ;Offset 0x26a7
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487 0x80
    jne       Label0x26e4               ;Offset 0x26e4
    cmp       word ptr ds:[BDA_VideoBufferSize], 00h;Offset 0x44c
    je        Label0x26e4               ;Offset 0x26e4
    push      bx
    push      cx
    push      dx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x5ff3                ;Offset 0x5ff3
    mov       bx, 0a000h                ;Segment 0xa000
    mov       cx, 8000h
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x26d7               ;Offset 0x26d7
    mov       bh, 0b0h                  ;Segment 0xb000
    mov       ch, 40h
    mov       dx, 0720h                 ;Attribute + space
    je        Label0x26d9               ;Offset 0x26d9
    mov       bh, 0b8h                  ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x26d9               ;Offset 0x26d9
Label0x26d7:                            ;Offset 0x26d7
    xor       dx, dx
Label0x26d9:                            ;Offset 0x26d9
    mov       es, bx
    mov       ax, dx
    xor       di, di
    rep stosw
    pop       dx
    pop       cx
    pop       bx
Label0x26e4:                            ;Offset 0x26e4
    ret
ClearScreen ENDP

Func0x26e5 PROC NEAR                    ;Offset 0x26e5
    lea       si, [Data0x2710]          ;Offset 0x2710
    jmp       Func0x26ef                ;Offset 0x26ef
Func0x26e5 ENDP

Func0x26eb PROC NEAR                    ;Offset 0x26eb
    lea       si, [Data0x271a]          ;Offset 0x271a
Func0x26eb ENDP
;continue!
Func0x26ef PROC NEAR                    ;Offset 0x26ef
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
Label0x2702:                            ;Offset 0x2702
    lodsw     word ptr cs:[si]
    out       dx, ax
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       ax, 0eb4h
    je        Label0x2702               ;Offset 0x2702
    pop       dx
    pop       ax
    ret
Func0x26ef ENDP

;Offset 0x2710
Data0x2710              DW (VGA_SEQ2_Mask2 SHL 8) OR VGA_SEQIdx_MapMask;0x0402
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0704
                        DW (VGA_GCTL4_Map2 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0204
                        DW (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x0005
                        DW (VGA_GCTL6_Mem_A0000_AFFFF SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0406

;Offset 0x271a
Data0x271a              DW ((VGA_SEQ2_Mask0 OR VGA_SEQ2_Mask1) SHL 8)OR VGA_SEQIdx_MapMask;0x0302
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0304
                        DW (VGA_GCTL4_Map0 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0004
                        DW (VGA_GCTL5_OddEven SHL 8) OR VGA_GCTLIdx_GraphicMode;0x1005
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B8000_BFFFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0e06
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B0000_B7FFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0a06

Func0x2726 PROC NEAR                    ;Offset 0x2726
    cbw      
    dec       ax
    js        Func0x2749                ;Offset 0x2749
    mov       cx, 0100h
    xor       dx, dx
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x38f0
    mov       bh, 00h
    dec       ax
    js        Func0x2749                ;Offset 0x2749
    lea       bp, [Font8x8]             ;Offset 0x30f0
    mov       bh, 08h
    dec       ax
    js        Func0x2749                ;Offset 0x2749
    lea       bp, [Font8x16]            ;Offset 0x38f0
    mov       bh, 10h
Func0x2726 ENDP
;continue!
Func0x2749 PROC NEAR                    ;Offset 0x2749
    mov       si, bp
    push      es
    pop       ds
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    jcxz      Label0x27a8               ;Offset 0x27a8
Label0x2754:                            ;Offset 0x2754
    push      cx
    mov       cl, 05h
    shl       dx, cl
    push      bx
    and       bx, 0007h
    add       dh, byte ptr cs:[bx + Data0x27c0];Offset 0x27c0
    pop       bx
    mov       di, dx
    mov       dl, bh
    xor       dh, dh
    or        dl, dl
    jne       Label0x276f               ;Offset 0x276f
    mov       dl, 10h
Label0x276f:                            ;Offset 0x276f
    pop       ax
Label0x2770:                            ;Offset 0x2770
    or        bh, bh
    jne       Label0x279c               ;Offset 0x279c
    cmp       al, 99h
    je        Label0x278e               ;Offset 0x278e
    cmp       al, 96h
    je        Label0x278e               ;Offset 0x278e
    cmp       al, 87h
    je        Label0x278e               ;Offset 0x278e
    cmp       al, 90h
    je        Label0x278e               ;Offset 0x278e
    cmp       al, 8fh
    je        Label0x278e               ;Offset 0x278e
    cmp       al, 68h
    je        Label0x278e               ;Offset 0x278e
    jmp       Label0x279c               ;Offset 0x279c
Label0x278e:                            ;Offset 0x278e
    mov       cx, 0008h
    rep movsb
    inc       si
    mov       cx, 0007h
    rep movsb
    inc       di
    jmp       Label0x27a0               ;Offset 0x27a0
Label0x279c:                            ;Offset 0x279c
    mov       cx, dx
    rep movsb
Label0x27a0:                            ;Offset 0x27a0
    sub       di, dx
    add       di, 0020h
    dec       ax
    jne       Label0x2770               ;Offset 0x2770
Label0x27a8:                            ;Offset 0x27a8
    or        bl, bl
    jns       Label0x27b5               ;Offset 0x27b5
    mov       dl, byte ptr [si]
    inc       si
    inc       cx
    or        dl, dl
    jne       Label0x2754               ;Offset 0x2754
    dec       cx
Label0x27b5:                            ;Offset 0x27b5
    or        bh, bh
    jne       Label0x27bb               ;Offset 0x27bb
    mov       bh, 0eh
Label0x27bb:                            ;Offset 0x27bb
    push      ds
    pop       es
    mov       ds, cx
    ret
Func0x2749 ENDP

;Offset 0x27c0
Data0x27c0              DB 000h, 040h, 080h, 0C0h, 020h, 060h, 0A0h, 0E0h

Func0x27c8 PROC NEAR                    ;Offset 0x27c8
    mov       al, bh
    cbw
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    dec       ax
    mov       ah, al
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne       Label0x27df               ;Offset 0x27df
    mov       al, VGA_CRTCIdx_UnderlineLoc;0x14
    out       dx, ax
Label0x27df:                            ;Offset 0x27df
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
    jb        Label0x27f4               ;Offset 0x27f4
    sub       ax, 0101h
Label0x27f4:                            ;Offset 0x27f4
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call      SetCursorEndStart         ;Offset 0x2698
    call      GetNumberOfActiveScanlines;Offset 0x2a60
    dec       al
    mov       ax, 00c8h
    js        Label0x280c               ;Offset 0x280c
    mov       ax, 015eh
    je        Label0x280c               ;Offset 0x280c
    mov       ax, 0190h
Label0x280c:                            ;Offset 0x280c
    div       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    inc       ax
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp       ax, 00c8h
    ja        Label0x2820               ;Offset 0x2820
    shl       ax, 01h
Label0x2820:                            ;Offset 0x2820
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    je        Label0x282a               ;Offset 0x282a
    shr       ax, 01h
Label0x282a:                            ;Offset 0x282a
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
Func0x27c8 ENDP

Func0x2841 PROC NEAR                    ;Offset 0x2841
    push      cx
    push      dx
    jmp       Func0x2851                ;Offset 0x2851
Func0x2841 ENDP

Func0x2845 PROC NEAR                    ;Offset 0x2845
    push      cx
    push      dx
    call      Func0x29f0                ;Offset 0x29f0
    add       si, 0005h
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
Func0x2845 ENDP
;continue!
Func0x2851 PROC NEAR                    ;Offset 0x2851
    mov       di, dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       cx, 0004h
    mov       ax, (VGA_SEQ0_AsyncReset SHL 8) OR VGA_SEQIdx_Reset;0x100
    cli
    out       dx, ax
    inc       ax
    call      Func0x28af                ;Offset 0x28af
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    lodsb     byte ptr es:[si]
    out       dx, al
    push      cx
    mov       cx, 8000h
Label0x286b:                            ;Offset 0x286b
    loop      Label0x286b               ;Offset 0x286b  Speed sensitive!
    pop       cx
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8) OR VGA_SEQIdx_Reset;0x300
    out       dx, ax
    sti
    call      TurnScreenOff             ;Offset 0x2b15
    push      ax
    mov       dx, di
    mov       ax, VGA_CRTCIdx_VertRetraceEnd;0x11
    out       dx, ax
    mov       cl, 19h
    xor       ax, ax
    call      Func0x28af                ;Offset 0x28af
    push      si
    add       dl, 06h                   ;Port 0x3?a
    in        al, dx
    xor       ax, ax
    out       dx, al
    mov       al, 10h
    add       si, ax
    mov       cl, 01h
    call      Func0x28c5                ;Offset 0x28c5
    inc       ax
    inc       si
    mov       cl, 02h
    call      Func0x28c5                ;Offset 0x28c5
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    xor       ax, ax
    call      Func0x28af                ;Offset 0x28af
    pop       si
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b17
    pop       dx
    pop       cx
    ret       
Func0x2851 ENDP

Func0x28af PROC NEAR                    ;Offset 0x28af
Label0x28af:                            ;Offset 0x28af
    mov       ah, byte ptr es:[si]
    inc       si
    out       dx, ax
    inc       ax
    loop      Label0x28af               ;Offset 0x28af
    ret
Func0x28af ENDP

Func0x28b8 PROC NEAR                    ;Offset 0x28b8
    mov       al, 00h
Func0x28b8 ENDP
;continue!
Func0x28ba PROC NEAR                    ;Offset 0x28ba
Label0x28ba:                            ;Offset 0x28ba
    out       dx, al
    push      ax
    inc       dx
    in        al, dx
    dec       dx
    stosb
    pop       ax
    inc       ax
    loop      Label0x28ba               ;Offset 0x28ba
    ret
Func0x28ba ENDP

Func0x28c5 PROC NEAR                    ;Offset 0x28c5
    mov       ah, 0ffh
Func0x28c5 ENDP
;continue!
Func0x28c7 PROC NEAR                    ;Offset 0x28c7
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
Label0x28ca:                            ;Offset 0x28ca
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;Misassemble!
    jb        Label0x28d3               ;Offset 0x28d3
    ja        Label0x28dc               ;Offset 0x28dc
    inc       ax
    mov       cl, 01h
Label0x28d3:                            ;Offset 0x28d3
    out       dx, al
    push      ax
    lodsb     byte ptr es:[si]
    out       dx, al
    pop       ax
    inc       ax
    loop      Label0x28ca               ;Offset 0x28ca
Label0x28dc:                            ;Offset 0x28dc
    ret
Func0x28c7 ENDP

Func0x28dd PROC NEAR                    ;Offset 0x28dd
    mov       ah, 0ffh
    push      cx
    push      dx
    mov       al, 00h
    mov       cx, 0010h
    call      Func0x2bd8                ;Offset 0x2bd8
Label0x28e9:                            ;Offset 0x28e9
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
    js        Label0x2902               ;Offset 0x2902
    jne       Label0x28e9               ;Offset 0x28e9
    add       ax, 0101h
    jb        Label0x28e9               ;Offset 0x28e9
    inc       di
    jmp       Label0x28e9               ;Offset 0x28e9
Label0x2902:                            ;Offset 0x2902
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
Func0x28dd ENDP

Func0x290b PROC NEAR                    ;Offset 0x290b
    push      ds
    push      es
    push      si
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    pop       si
    pop       ds
    je        Label0x291e               ;Offset 0x291e
    mov       cx, 0008h
    rep movsw
    inc       si
    movsb
Label0x291e:                            ;Offset 0x291e
    pop       ds
    ret
Func0x290b ENDP

Func0x2920 PROC NEAR                    ;Offset 0x2920
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, 0ffh
    out       dx, al
    push      cs
    pop       es
    call      GetNumberOfActiveScanlines;Offset 0x2a60
    mov       ah, al
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    ;cmp       ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 3Dh, 13h, 00h                    ;Misassemble!
    je        Label0x294f               ;Offset 0x294f
    lea       si, [Data0x2d4a]          ;Offset 0x2d4a
    jb        Label0x294b               ;Offset 0x294b
    lea       si, [Data0x2dd2]          ;Offset 0x2dd2
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0x294b               ;Offset 0x294b
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0x294b               ;Offset 0x294b
    lea       si, [Data0x2cc2]          ;Offset 0x2cc2
Label0x294b:                            ;Offset 0x294b
    call      Func0x2969                ;Offset 0x2969
    ret
Label0x294f:                            ;Offset 0x294f
    lea       si, [Data0x2e16]          ;Offset 0x2e16
    call      Func0x2969                ;Offset 0x2969
    lea       si, [Data0x2e3e]          ;Offset 0x2e3e
    mov       al, 10h
    call      Func0x296b                ;Offset 0x296b
    lea       si, [Data0x2e52]          ;Offset 0x2e52
    mov       al, 20h
    call      Func0x296b                ;Offset 0x296b
    ret
Func0x2920 ENDP

Func0x2969 PROC NEAR                    ;Offset 0x2969
    xor       al, al
Func0x2969 ENDP
;continue!
Func0x296b PROC NEAR                    ;Offset 0x296b
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    inc       dx
    mov       ax, word ptr es:[si - 04h]
    mov       cx, word ptr es:[si - 02h]
    test      byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    je        Label0x2987               ;Offset 0x2987
    add       si, cx
    add       si, 0004h
    mov       ax, word ptr es:[si - 04h]
Label0x2987:                            ;Offset 0x2987
    or        ah, ah
    je        Label0x2999               ;Offset 0x2999
    jns       Func0x299f                ;Offset 0x299f
Label0x298d:                            ;Offset 0x298d
    lodsb     byte ptr es:[si]
    out       dx, al
    jmp       Label0x2992               ;Offset 0x2992 Speed sensitive!
Label0x2992:                            ;Offset 0x2992
    out       dx, al
    jmp       Label0x2995               ;Offset 0x2995 Speed sensitive!
Label0x2995:                            ;Offset 0x2995
    out       dx, al
    loop      Label0x298d               ;Offset 0x298d
    ret
Label0x2999:                            ;Offset 0x2999
    push      bx
    call      Func0x2ba7                ;Offset 0x2ba7
    pop       bx
    ret
Func0x296b ENDP

Func0x299f PROC NEAR                    ;Offset 0x299f
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    jne       Label0x29b2               ;Offset 0x29b2
Label0x29a6:                            ;Offset 0x29a6
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    loop      Label0x29a6               ;Offset 0x29a6
    ret
Label0x29b2:                            ;Offset 0x29b2
    push      cx
    push      dx
    mov       dh, byte ptr es:[si]
    inc       si
    mov       ch, byte ptr es:[si]
    inc       si
    mov       cl, byte ptr es:[si]
    inc       si
    call      Func0x29d8                ;Offset 0x29d8
    pop       dx
    pop       cx
    out       dx, al
    jmp       Label0x29c8               ;Offset 0x29c8 Speed sensitive!
Label0x29c8:                            ;Offset 0x29c8
    out       dx, al
    jmp       Label0x29cb               ;Offset 0x29cb Speed sensitive!
Label0x29cb:                            ;Offset 0x29cb
    out       dx, al
    loop      Label0x29b2               ;Offset 0x29b2
    ret
Func0x299f ENDP

Func0x29cf PROC NEAR                    ;Offset 0x29cf
Label0x29cf:                            ;Offset 0x29cf
    in        al, dx
    stosb
    in        al, dx
    stosb
    in        al, dx
    stosb
    loop      Label0x29cf               ;Offset 0x29cf
    ret
Func0x29cf ENDP

Func0x29d8 PROC NEAR                    ;Offset 0x29d8
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
Func0x29d8 ENDP

Func0x29f0 PROC NEAR                    ;Offset 0x29f0
    mov       al, BDA_VPCB_VideoParameterTable;0x0
    call      LookupVideoParameterControlBlockPointer;Offset 0x2b89
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw  
    shl       ax, 01h
    shl       ax, 01h
    mov       di, ax
    call      GetNumberOfActiveScanlines;Offset 0x2a60
    add       di, ax
    mov       al, byte ptr cs:[di + Data0x2a10];Offset 0x2a10
    mov       ah, 40h
    mul       ah
    add       si, ax
    ret
Func0x29f0 ENDP

;
;                           +------------------------ 200 Scan lines
;                           |
;                           |     +------------------ 350 Scan lines
;                           |     |
;                           |     |     +------------ 400 Scan lines
;                           |     |     |
;                           |     |     |     +------ 480 Scan lines
;                           |     |     |     |
;Offset 0x2a10              |     |     |     |       Display mode:
Data0x2a10              DB 000h, 013h, 017h, 000h   ; 0x00
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
GetNumberOfActiveScanlines PROC NEAR    ;Offset 0x2a60
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push  bx
    lea   bx, [NumberOfActiveScanlinesPerModePlusOne];Offset 0x2a8c
    cmp   al, BDA_DM_320x200_256_Color_Graphics;0x13
    xlatb cs:[bx]
    ja    Label0x2a72                   ;Offset 0x2a72
    dec   al                            ;Subtract one to see if it turns negative (and thus, not set)
    jns   Label0x2a89                   ;Offset 0x2a89
Label0x2a72:                            ;Offset 0x2a72
    mov   al, 02h                       ;400 Scan lines
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    jne   Label0x2a89                   ;Offset 0x2a89
    dec   ax                            ;350 Scan lines
    call  IsMDAHiResEnhanced            ;Offset 0x2aa0
    je    Label0x2a89                   ;Offset 0x2a89
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je    Label0x2a89                   ;Offset 0x2a89
    dec   ax                            ;200 Scan lines
Label0x2a89:                            ;Offset 0x2a89
    pop   bx
    ret
GetNumberOfActiveScanlines ENDP

    nop                                 ;hello

;
;
;                                               000h means it's determined another way
;                                               All others are decremented by one and returned.
;Offset 0x2a8c                                    Display mode:
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

IsMDAHiResEnhanced PROC NEAR            ;Offset 0x2aa0
    push  ax
    mov   al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and   al, BDA_EFBS_AdapterTypeMask  ;0xf
    cmp   al, BDA_EFBS_MDAHiResEnhanced ;0x3
    je    Found                         ;Offset 0x2aac
    cmp   al, BDA_EFBS_MDAHiResEnhanced_2;0x9 Also check the other one
Found:                                  ;Offset 0x2aac
    pop   ax
    ret
IsMDAHiResEnhanced ENDP

Func0x2aae PROC NEAR                    ;Offset 0x2aae
    push  ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call  Func0x5ff3                    ;Offset 0x5ff3
    cmp   al, BDA_DM_80x25_Monochrome_Text;0x7
    je    Label0x2abf                   ;Offset 0x2abf
    cmp   al, BDA_DM_80x25_16_Color_Text;0x3
    ja    Label0x2abf                   ;Offset 0x2abf
    xor   al, al
Label0x2abf:                            ;Offset 0x2abf
    pop   ax
    ret
Func0x2aae ENDP

Func0x2ac1 PROC NEAR                    ;Offset 0x2ac1
    push  si
    cbw  
    xchg  ax, si
    add   si, di
Label0x2ac6:                            ;Offset 0x2ac6
    lodsb byte ptr es:[si]
    cmp   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je    label0x2ad3                   ;Offset 0x2ad3
    inc   al
    jne   Label0x2ac6                   ;Offset 0x2ac6
    inc   ax
Label0x2ad3:                            ;Offset 0x2ad3
    pop   si
    ret
Func0x2ac1 ENDP

Func0x2ad5 PROC NEAR                    ;Offset 0x2ad5
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
    in    al, dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    pop   dx
    ret
Func0x2ad5 ENDP

Func0x2ae5 PROC NEAR                    ;Offset 0x2ae5
    push  ax
    push  dx
    call  Func0x2bd8                    ;Offset 0x2bd8
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
Func0x2ae5 ENDP

Func0x2af9 PROC NEAR                    ;Offset 0x2af9
    push  dx
    call  Func0x2bd8                    ;Offset 0x2bd8
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
Func0x2af9 ENDP

TurnScreenOn PROC NEAR                  ;Offset 0x2b11
    mov   ah, 00h
    jmp   ToggleScreenOnOff             ;Offset 0x2b17
TurnScreenOn ENDP

TurnScreenOff PROC NEAR                 ;Offset 0x2b15
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
ToggleScreenOnOff PROC NEAR             ;Offset 0x2b17
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
SetDisplayCombinationCodeIndex PROC NEAR;Offset 0x2b2c
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    NotFound                      ;Offset 0x2b4c
    xor   dx, dx
    xor   cx, cx
    mov   cl, byte ptr es:[si]
    add   si, sizeof DisplayCombinationCodeTable;0x4 Skip display combination code header
Find:                                   ;Offset 0x2b3d
    lodsw word ptr es:[si]
    cmp   ax, bx
    je    Found                         ;Offset 0x2b4e
    xchg  al, ah
    cmp   ax, bx
    je    Found                         ;Offset 0x2b4e
    inc   dx
    loop  Find                          ;Offset 0x2b3d
NotFound:                               ;Offset 0x2b4c
    mov   dl, 0ffh                      ;Entry not found set index to -1
Found:                                  ;Offset 0x2b4e
    mov   byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], dl;Offset 0x48a
    ret
SetDisplayCombinationCodeIndex ENDP

GetDisplayCombinationCode PROC NEAR     ;Offset 0x2b53
    push  di
    push  es
    mov   bx, 0ffffh
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2b89
    je    Label0x2b86                   ;Offset 0x2b86
    mov   al, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    cmp   al, byte ptr es:[si]
    jae   Label0x2b86                   ;Offset 0x2b86
    cbw  
    shl   ax, 01h
    xchg  ax, bx
    mov   ax, word ptr es:[bx + si + 04h]
    mov   bx, ax
    cmp   al, 00h
    je    Label0x2b84                   ;Offset 0x2b84
    test  byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je    Label0x2b7e                   ;Offset 0x2b7e
    jp    Label0x2b80                   ;Offset 0x2b80
Label0x2b7e:                            ;Offset 0x2b7e
    xor   al, 01h
Label0x2b80:                            ;Offset 0x2b80
    test  al, 01h
    jne   Label0x2b86                   ;Offset 0x2b86
Label0x2b84:                            ;Offset 0x2b84
    xchg  bl, bh
Label0x2b86:                            ;Offset 0x2b86
    pop   es
    pop   di
    ret
GetDisplayCombinationCode ENDP

LookupVideoParameterControlBlockPointer PROC NEAR;Offset 0x2b89
    cbw  
    les   si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8    es:si = Video Save/Override Pointer table
    test  al, 02h                       ;Test if al bit 2 is set.
    je    DereferencePointerWithOffset  ;Offset 0x2b9b  if not set, then lookup in primary table
    push  ax                            ;else save ax
    mov   al, BDA_VPCB_SecondarySavePointerTbl;0x10 load offset 0x10 instead (secondary save pointer)
    call  DereferencePointerWithOffset  ;Offset 0x2b9b
    pop   ax                            ;restore ax
    je    Func0x2ba6                    ;Offset 0x2ba6  if previous call yielded null, return else call with original value
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
DereferencePointerWithOffset PROC NEAR  ;Offset 0x2b9b
    add   si, ax                        ;add ax offset to video save/override pointer
    les   si, es:[si]                   ;load es:si from table
    mov   di, si                        ;store si in di
    mov   ax, es                        ;store es in ax
    or    ax, si                        ;or ax and si (set flags)
DereferencePointerWithOffset ENDP
;continue!
Func0x2ba6 PROC NEAR                    ;Offset 0x2ba6
    ret
Func0x2ba6 ENDP

Func0x2ba7 PROC NEAR                    ;Offset 0x2ba7
Label0x2ba7:                            ;Offset 0x2ba7
    lodsb byte ptr es:[si]
    mov   ah, al
    mov   bx, 0003h
Label0x2bae:                            ;Offset 0x2bae
    shl   ah, 01h
    shl   ah, 01h
    xor   al, al
    test  ah, 0c0h
    je    Label0x2bc3                   ;Offset 0x2bc3
    jns   Label0x2bc1                   ;Offset 0x2bc1
    jnp   Label0x2bbf                   ;Offset 0x2bbf
    add   al, 15h
Label0x2bbf:                            ;Offset 0x2bbf
    add   al, 15h
Label0x2bc1:                            ;Offset 0x2bc1
    add   al, 15h
Label0x2bc3:                            ;Offset 0x2bc3
    out   dx, al
    dec   bx
    jne   Label0x2bae                   ;Offset 0x2bae
    loop  Label0x2ba7                   ;Offset 0x2ba7
    ret
Func0x2ba7 ENDP

Func0x2bca PROC NEAR                    ;Offset 0x2bca
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2bd2:                            ;Offset 0x2bd2
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne   Label0x2bd2                   ;Offset 0x2bd2
    pop   ax
Func0x2bca ENDP
;continue!
Func0x2bd8 PROC NEAR                    ;Offset 0x2bd8
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2be0:                            ;Offset 0x2be0
    sti
    nop
    cli
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x2be0                   ;Offset 0x2be0
    pop   ax
    ret
Func0x2bd8 ENDP

;
;inputs:
;   al * 8 * 1193 ticks time
;
Beep PROC NEAR                          ;Offset 0x2bea
    mov   ah, al
    in    al, KB_PortB                  ;Port 0x61
    push  ax
    or    al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out   KB_PortB, al                  ;Port 0x61
    mov   al, ah
    call  Sleep2                        ;Offset 0x2bfc
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
Sleep2 PROC NEAR                        ;Offset 0x2bfc
    call  SetupSquareWave               ;Offset 0x2c3c
    push  ax
    push  cx
    push  dx
    push  ds
    mov   dx, SYS_Segment               ;Segment 0xf000
    mov   ds, dx
    mov   dx, KB_CommandPort_XT         ;0x62
    cmp   byte ptr ds:[SYS_MachineID], SYS_MachineID_IBM_AT;Offset 0xfffe 0xfc
    jne   NotAT                         ;Offset 0x2c13
    dec   dx
NotAT:                                  ;Offset 0x2c13
    pop   ds
    mov   cl, al
    xor   ch, ch
    shl   cx, 01h
    shl   cx, 01h
    shl   cx, 01h
    jcxz  NoSleep                       ;Offset 0x2c38
    in    al, KB_PortB                  ;Port 0x61
    mov   ah, al                        ;store off original value
    or    al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out   KB_PortB, al                  ;Port 0x61
WaitTimeExpired:                        ;Offset 0x2c28
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne   WaitTimeExpired               ;Offset 0x2c28
WaitAgain:                              ;Offset 0x2c2d
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    je    WaitAgain                     ;Offset 0x2c2d
    loop  WaitTimeExpired               ;Offset 0x2c28
    mov   al, ah                        ;restore original value
    out   KB_PortB, al                  ;Port 0x61
NoSleep:                                ;Offset 0x2c38
    pop   dx
    pop   cx
    pop   ax
    ret
Sleep2 ENDP

SetupSquareWave PROC NEAR               ;Offset 0x2c3c
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

PrintScreenHandler:                     ;Offset 0x2c50
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
    je   Label0x2ca4                    ;Offset 0x2ca4
    mov  ah, 0fh
    int  10h
    mov  bl, ah
    mov  ah, 03h
    int  10h
    push dx
    call PrintCRLF                      ;Offset 0x2caa
    mov  cl, 0ffh
    xor  dx, dx
Label0x2c74:                            ;Offset 0x2c74
    mov  ah, 02h
    int  10h
    mov  ah, 08h
    int  10h
    or   al, al
    jne  Label0x2c82                    ;Offset 0x2c82
    mov  al, 20h
Label0x2c82:                            ;Offset 0x2c82
    call PrintCharacter                 ;Offset 0x2cb1
    jne  Label0x2c9b                    ;Offset 0x2c9b
    inc  dx
    ;cmp  dl, bl
    DB 3Ah, 0D3h                        ;Misassemble!
    jb   Label0x2c74                    ;Offset 0x2c74
    call PrintCRLF                      ;Offset 0x2caa
    xor  dl, dl
    inc  dh
    cmp  dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe  Label0x2c74                    ;Offset 0x2c74
    xor  cl, cl
Label0x2c9b:                            ;Offset 0x2c9b
    pop  dx
    mov  ah, 02h
    int  10h
    mov  byte ptr ds:[BDA_PrintScreenStatus], cl;Offset 0x500
Label0x2ca4:                            ;Offset 0x2ca4
    pop  ds
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    iret 

PrintCRLF PROC NEAR                     ;Offset 0x2caa
    mov  al, 0dh
    call PrintCharacter                 ;Offset 0x2cb1
    mov  al, 0ah
PrintCRLF ENDP
;continue!
PrintCharacter PROC NEAR                ;Offset 0x2cb1
    push dx
    xor  dx, dx
    xor  ah, ah
    int  17h
    test ah, 29h
    pop  dx
    ret
PrintCharacter ENDP

    nop                                 ;hello

;Offset 0x2cbe
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2cc2
Data0x2cc2              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 028h, 02Ah, 001h, 003h, 009h, 00Bh, 021h, 023h, 029h, 02Bh
                        DB 004h, 006h, 00Ch, 00Eh, 024h, 026h, 02Ch, 02Eh, 005h, 007h, 00Dh, 00Fh, 025h, 027h, 02Dh, 02Fh
                        DB 010h, 012h, 018h, 01Ah, 030h, 032h, 038h, 03Ah, 011h, 013h, 019h, 01Bh, 031h, 033h, 039h, 03Bh
                        DB 014h, 016h, 01Ch, 01Eh, 034h, 036h, 03Ch, 03Eh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d02
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d06
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 025h, 028h, 002h, 007h, 01Bh, 020h, 00Fh, 014h, 028h, 02Ch
                        DB 00Ch, 011h, 025h, 02Ah, 014h, 01Eh, 032h, 036h, 00Fh, 013h, 027h, 02Ch, 01Bh, 020h, 034h, 039h
                        DB 006h, 00Bh, 01Fh, 024h, 013h, 018h, 02Ch, 030h, 009h, 00Dh, 021h, 026h, 015h, 01Ah, 02Eh, 033h
                        DB 013h, 017h, 02Bh, 030h, 01Fh, 024h, 038h, 03Dh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2d46
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d4a
Data0x2d4a              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d8a
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d8e
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2dce
                        DW 0000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2dd2
Data0x2dd2              DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
;Offset 0x2e12
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e16
Data0x2e16              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2e26
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e2a
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2e3a
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e3e
Data0x2e3e              DB 000h, 005h, 008h, 00Bh, 00Eh, 011h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Dh, 032h, 038h, 03Fh
;Offset 0x2e4e
                        DW 4000h        ;BDA_VideoDisplayDataArea
                        DW 00D8h        ;count (below) RGB, 0xd8 * 3
;Offset 0x2e52
Data0x2e52              DB 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 03Fh, 03Fh
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

;Offset 0x30da
RowsOnScreen            DB 000h, 00Eh, 019h, 02Bh

;Offset 0x30de
Data0x30de              DW offset Font8x16;Offset 0x38f0
                        DW offset Font8x8;Offset 0x30f0
                        DW offset GraphicsFont8x8;Offset 0x34f0
                        DW offset Data0x48f0;Offset 0x48f0
                        DW offset Font8x16;Offset 0x38f0
                        DW offset Data0x48f0;Offset 0x48f0

;Offset 0x30ea
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx

;Offset 0x30f0
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

;Offset 0x34f0
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
 
;Offset 0x38f0
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

;Offset 0x48f0
Data0x48f0              DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h
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

;Offset 0x4a34
                        DB 00Dh, 00Ah
                        DB 'MATROX POWER GRAPHICS ACCELERATOR', 00Dh, 00Ah
                        DB 'MGA Series', 00Dh, 00Ah
                        DB 'VGA/VBE BIOS, Version V3.3', 00Dh, 00Ah
                        DB 'Copyright (C) 1998, Matrox Graphics Inc.', 00Dh, 00Ah
                        DB 'Copyright (C) LSI Logic Corporation 1990-1991', 00Dh, 00Ah
                        DB 00Dh, 00Ah, 000h
;Offset 0x4add
                        DB 'DATE: 11/21/00', 000h
;Offset 0x4aec
                        DB 'Revision: 0.30 ', 000h

VESAPowerManagement:                    ;Offset 0x4afc
    cmp  bl, 00h
    jne  VPMNot0                        ;Offset 0x4b03
    jmp  VPMGetCaps                     ;Offset 0x4b15
VPMNot0:                                ;Offset 0x4b03
    cmp  bl, 01h
    jne  VPMNot1                        ;Offset 0x4b0a
    jmp  VPMSetDisplayPowerState        ;Offset 0x4b40
VPMNot1:                                ;Offset 0x4b0a
    cmp  bl, 02h
    jne  VPMNot2                        ;Offset 0x4b11
    jmp  VPMGetDisplayPowerState        ;Offset 0x4b1d
VPMNot2:                                ;Offset 0x4b11
    mov  ax, 014fh
    iret
VPMGetCaps:                             ;Offset 0x4b15
    mov  ax, 004fh
    mov  bh, 07h
    mov  bl, 10h
    iret
VPMGetDisplayPowerState:                ;Offset 0x4b1d
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop  
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    xor  bh, bh                         ;0 = Display is on
    and  al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    je   Label0x4b3b                    ;Offset 0x4b3b
    inc  bh                             ;1 = Display in standby
    cmp  al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    je   Label0x4b3b                    ;Offset 0x4b3b
    inc  bh                             ;2 = Display is suspended
    cmp  al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    je   Label0x4b3b                    ;Offset 0x4b3b
    shl  bh, 01h                        ;4 = Display is off
Label0x4b3b:                            ;Offset 0x4b3b
    mov  ax, 004fh
    pop  dx
    iret
VPMSetDisplayPowerState:                ;Offset 0x4b40
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop  
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    mov  al, bh
    and  bh, 0fh
    je   Label0x4b54                    ;Offset 0x4b54
    and  al, 07h
    je   Label0x4ba0                    ;Offset 0x4ba0
Label0x4b54:                            ;Offset 0x4b54
    in   al, dx
    and  al, NOT (MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff);0xcf
    or   bh, bh                         ;0 = Turn display on
    jne  Label0x4b71                    ;Offset 0x4b71
    out  dx, al
    call ScreenOn2                      ;Offset 0x4bb8
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    je   Label0x4b6c                    ;Offset 0x4b6c
    mov  al, 01h
    call Func0x5382                     ;Offset 0x5382
Label0x4b6c:                            ;Offset 0x4b6c
    mov  ax, 004fh
    jmp  Label0x4ba3                    ;Offset 0x4ba3
Label0x4b71:                            ;Offset 0x4b71
    cmp  bh, 01h                        ;1 = Set display to standby
    jne  Label0x4b7a                    ;Offset 0x4b7a
    or   al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    jmp  Label0x4b8a                    ;Offset 0x4b8a
Label0x4b7a:                            ;Offset 0x4b7a
    cmp  bh, 02h                        ;2 = Suspend display
    jne  Label0x4b83                    ;Offset 0x4b83
    or   al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    jmp  Label0x4b8a                    ;Offset 0x4b8a
Label0x4b83:                            ;Offset 0x4b83
    cmp  bh, 04h                        ;4 = Turn display off
    jne  Label0x4ba0                    ;Offset 0x4ba0
    or   al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
Label0x4b8a:                            ;Offset 0x4b8a
    out  dx, al
    call ScreenOff2                     ;Offset 0x4ba5
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    je   Label0x4b9b                    ;Offset 0x4b9b
    mov  al, 00h
    call Func0x5382                     ;Offset 0x5382
Label0x4b9b:                            ;Offset 0x4b9b
    mov  ax, 004fh
    jmp  Label0x4ba3                    ;Offset 0x4ba3
Label0x4ba0:                            ;Offset 0x4ba0
    mov  ax, 014fh
Label0x4ba3:                            ;Offset 0x4ba3
    pop  dx
    iret 

ScreenOff2 PROC NEAR                    ;Offset 0x4ba5
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

ScreenOn2 PROC NEAR                     ;Offset 0x4bb8
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

;Offset 0x4bcb
                        DB 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
                        DB 000h, 000h

;These four tables are order dependent (There's a pointer compare in one of the functions)

;Offset 0x4bd5
DDCFuncsRev3Up          DW offset DDCPin1Send1      ;Offset 0x4d37  ;0
                        DW offset DDCPin3Send1      ;Offset 0x4d1b  ;2
                        DW offset DDCPin1Send0      ;Offset 0x4d82  ;4
                        DW offset DDCPin3Send0      ;Offset 0x4d53  ;6
                        DW offset DDCPins01Read     ;Offset 0x4dfd  ;8
                        DW offset DDCPins23Read     ;Offset 0x4e23  ;a

;Offset 0x4be1
DDCFuncs                DW offset DDCPin1Send1      ;Offset 0x4d37  ;0
                        DW offset DDCPin3Send1      ;Offset 0x4d1b  ;2
                        DW offset DDCPin1Send0      ;Offset 0x4d82  ;4
                        DW offset DDCPin3Send0      ;Offset 0x4d53  ;6
                        DW offset DDCPin1Read       ;Offset 0x4db1  ;8
                        DW offset DDCPin3Read       ;Offset 0x4dd7  ;a

;Offset 0x4bed
MISCFuncs               DW offset MISCPin0Send1     ;Offset 0x4e65  ;0
                        DW offset MISCPin1Send1     ;Offset 0x4e49  ;2
                        DW offset MISCPin0Send0     ;Offset 0x4eb0  ;4
                        DW offset MISCPin1Send0     ;Offset 0x4e81  ;6
                        DW offset MISCPin0Read      ;Offset 0x4edf  ;8
                        DW offset MISCPin1Read      ;Offset 0x4f05  ;a

;Offset 0x4bf9
Data0x4bf9              DW offset Func0x4f6b        ;Offset 0x4f6b  ;0
                        DW offset Func0x4f2b        ;Offset 0x4f2b  ;2
                        DW offset Func0x4ff0        ;Offset 0x4ff0  ;4
                        DW offset Func0x4fab        ;Offset 0x4fab  ;6
                        DW offset Func0x5037        ;Offset 0x5037  ;8
                        DW offset Func0x5035        ;Offset 0x5035  ;a


VESADDC:                                ;Offset 0x4c05
    cmp  bl, 00h
    je   DDCInstallationCheck           ;Offset 0x4c2c
    cmp  bl, 01h
    jne  DDCUnknownFunction             ;Offset 0x4c13
    call DDCReadEDID                    ;Offset 0x4c59
    iret
DDCUnknownFunction:                     ;Offset 0x4c13
    mov  ax, 0100h
    iret

;Unused function
Func0x4c17 PROC NEAR                    ;Offset 0x4c17
    push cx
    mov  cx, 0000h
    call Func0x4cdb                     ;Offset 0x4cdb
    call Func0x50d9                     ;Offset 0x50d9
    mov  cx, 0001h
    call Func0x4cdb                     ;Offset 0x4cdb
    call Func0x50d9                     ;Offset 0x50d9
    pop  cx
    ret
Func0x4c17 ENDP

DDCInstallationCheck:                   ;Offset 0x4c2c
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
    call DDCReadEDID                    ;Offset 0x4c59
    or   ah, ah
    jne  Label0x4c46                    ;Offset 0x4c46
    or   bl, 02h
Label0x4c46:                            ;Offset 0x4c46
    mov  bh, 01h
    or   bl, bl
    jne  Label0x4c4e                    ;Offset 0x4c4e
    mov  bh, 00h
Label0x4c4e:                            ;Offset 0x4c4e
    mov  ah, 00h
    mov  al, 4fh
    mov  sp, bp
    pop  bp
    sti
    pop  di
    pop  es
    iret

DDCReadEDID PROC NEAR                   ;Offset 0x4c59
    push ds
    push es
    push bx
    push si
    push ecx
    push edx
    push edi
    mov  ax, 014fh
    test cx, 0fffeh
    jne  Label0x4cd0                    ;Offset 0x4cd0
    call Func0x4cdb                     ;Offset 0x4cdb
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
    je   Label0x4c95                    ;Offset 0x4c95
    mov  word ptr [di], 0080h
Label0x4c95:                            ;Offset 0x4c95
    shr  dx, 01h
    mov  cx, dx
    shl  ecx, 10h
    mov  dx, 0080h
    shl  edx, 10h
    mov  dx, 0001h
    mov  cl, 04h
    mov  ch, 0a0h
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
Label0x4cb0:                            ;Offset 0x4cb0
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    je   Label0x4cc3                    ;Offset 0x4cc3
    call Func0x5362                     ;Offset 0x5362
    or   ah, ah
    jne  Label0x4cc3                    ;Offset 0x4cc3
    mov  ax, 004fh
    jmp  Label0x4cca                    ;Offset 0x4cca
Label0x4cc3:                            ;Offset 0x4cc3
    dec  cl
    jne  Label0x4cb0                    ;Offset 0x4cb0
    mov  ax, 014fh
Label0x4cca:                            ;Offset 0x4cca
    pop  ds
    pop  word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    sti
Label0x4cd0:                            ;Offset 0x4cd0
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  bx
    pop  es
    pop  ds
    ret
DDCReadEDID ENDP

Func0x4cdb PROC NEAR                    ;Offset 0x4cdb
    mov  al, 00h
    test byte ptr cs:[Data0x7d1b], 40h  ;Offset 0x7d1b
    je   Label0x4cf1                    ;Offset 0x4cf1
    mov  al, 01h
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    jne  Label0x4cf1                    ;Offset 0x4cf1
    mov  al, 00h
Label0x4cf1:                            ;Offset 0x4cf1
    lea  si, [Data0x4bf9]               ;Offset 0x4bf9
    xor  al, cl
    jne  Label0x4d1a                    ;Offset 0x4d1a
    lea  si, [DDCFuncs]                 ;Offset 0x4be1
    push bx
    push cx
    push di
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_Header_Byte_RevisionID ;0x8
    call AccessPCIRegister              ;Offset 0x68f8
    cmp  cl, MGA_G200_Rev3              ;0x3
    jb   Label0x4d17                    ;Offset 0x4d17
    lea  si, [DDCFuncsRev3Up]           ;Offset 0x4bd5
Label0x4d17:                            ;Offset 0x4d17
    pop  di
    pop  cx
    pop  bx
Label0x4d1a:                            ;Offset 0x4d1a
    ret
Func0x4cdb ENDP

DDCPin3Send1 PROC NEAR                  ;Offset 0x4d1b
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
DDCPin3Send1 ENDP

DDCPin1Send1 PROC NEAR                  ;Offset 0x4d37
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
DDCPin1Send1 ENDP

DDCPin3Send0 PROC NEAR                  ;Offset 0x4d53
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC3Data ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_GENIOCTRL_DDC3Enable   ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
DDCPin3Send0 ENDP

DDCPin1Send0 PROC NEAR                  ;Offset 0x4d82
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC1Data ;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_GENIOCTRL_DDC1Enable   ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
DDCPin1Send0 ENDP

DDCPin1Read PROC NEAR                   ;Offset 0x4db1
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 07h
    pop  edx
    pop  ecx
    ret
DDCPin1Read ENDP

DDCPin3Read PROC NEAR                   ;Offset 0x4dd7
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 05h
    pop  edx
    pop  ecx
    ret
DDCPin3Read ENDP

DDCPins01Read PROC NEAR                 ;Offset 0x4dfd
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT (MGA_GENIOCTRL_DDC0Enable OR MGA_GENIOCTRL_DDC1Enable);0xfc
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 08h
    pop  edx
    pop  ecx
    ret
DDCPins01Read ENDP

DDCPins23Read PROC NEAR                 ;Offset 0x4e23
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT (MGA_GENIOCTRL_DDC2Enable OR MGA_GENIOCTRL_DDC3Enable);0xf3
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 06h
    pop  edx
    pop  ecx
    ret
DDCPins23Read ENDP

MISCPin1Send1 PROC NEAR                 ;Offset 0x4e49
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
MISCPin1Send1 ENDP

MISCPin0Send1 PROC NEAR                 ;Offset 0x4e65
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
MISCPin0Send1 ENDP

MISCPin1Send0 PROC NEAR                 ;Offset 0x4e81
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc1Data;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_GENIOCTRL_Misc1Enable  ;20h
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
MISCPin1Send0 ENDP

MISCPin0Send0 PROC NEAR                 ;Offset 0x4eb0
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc0Data;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_GENIOCTRL_Misc0Enable  ;0x10
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    pop  edx
    pop  ecx
    ret
MISCPin0Send0 ENDP

MISCPin0Read PROC NEAR                  ;Offset 0x4edf
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 04h
    pop  edx
    pop  ecx
    ret
MISCPin0Read ENDP

MISCPin1Read PROC NEAR                  ;Offset 0x4f05
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    shl  cl, 03h
    pop  edx
    pop  ecx
    ret
MISCPin1Read ENDP

Func0x4f2b PROC NEAR                    ;Offset 0x4f2b
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4bed
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 41h;Offset 0x472 0x41
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x4f55:                            ;Offset 0x4f55
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    ;jne  Label0x4f62                    ;Offset 0x4f62
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x4f55                    ;Offset 0x4f55
Label0x4f62:                            ;Offset 0x4f62
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x4f2b ENDP

Func0x4f6b PROC NEAR                    ;Offset 0x4f6b
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4bed
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 42h;Offset 0x472 0x42
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x4f95:                            ;Offset 0x4f95
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    ;jne  Label0x4fa2                    ;Offset 0x4fa2
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x4f95                    ;Offset 0x4f95
Label0x4fa2:                            ;Offset 0x4fa2
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x4f6b ENDP

Func0x4fab PROC NEAR                    ;Offset 0x4fab
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4bed
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    and  byte ptr ds:[BDA_SoftResetFlag], 0feh ;Offset 0x472 0xfe
    or   byte ptr ds:[BDA_SoftResetFlag], 40h ;Offset 0x472 0x40
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x4fda:                            ;Offset 0x4fda
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    ;jne  Label0x4fe7                    ;Offset 0x4fe7
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x4fda                    ;Offset 0x4fda
Label0x4fe7:                            ;Offset 0x4fe7
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x4fab ENDP

Func0x4ff0 PROC NEAR                    ;Offset 0x4ff0
    push ds
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  ds
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4bed
    mov  di, BDA_SoftResetFlag          ;Offset 0x472
    and  byte ptr ds:[BDA_SoftResetFlag], 0fdh;Offset 0x472
    or   byte ptr ds:[BDA_SoftResetFlag], 40h;Offset 0x472
    mov  edx, 00000001h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x501f:                            ;Offset 0x501f
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    ;jne  Label0x502c                    ;Offset 0x502c
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x501f                    ;Offset 0x501f
Label0x502c:                            ;Offset 0x502c
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  ds
    ret
Func0x4ff0 ENDP

Func0x5035 PROC NEAR                    ;Offset 0x5035
    stc
    ret
Func0x5035 ENDP

Func0x5037 PROC NEAR                    ;Offset 0x5037
    push ds
    push es
    push si
    push ecx
    push edx
    push edi
    push 0000h
    pop  es
    mov  ch, byte ptr cs:[Data0x7ff5]   ;Offset 0x7ff5
    mov  si, offset MISCFuncs           ;Offset 0x4bed
    mov  di, BDA_SoftResetFlagHighByte  ;Offset 0x473
    shl  edi, 10h
    mov  edx, 00010000h
    mov  cl, 04h
    and  ecx, 0000ffffh
Label0x5061:                            ;Offset 0x5061
    call Func0x5258                     ;Offset 0x5258
    or   ax, ax
    ;jne  Label0x506e                    ;Offset 0x506e
    DB 00Fh, 085h, 004h, 000h           ;Weird misassemble
    dec  cl
    jne  Label0x5061                    ;Offset 0x5061
Label0x506e:                            ;Offset 0x506e
    mov  al, byte ptr es:[BDA_SoftResetFlagHighByte];Offset 0x473
    shl  al, 07h
    pop  edi
    pop  edx
    pop  ecx
    pop  si
    pop  es
    pop  ds
    ret
Func0x5037 ENDP

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
SleepIfNeeded PROC NEAR                 ;Offset 0x507f
    cmp  si, offset DDCFuncs            ;Offset 0x4be1
    ja   DontSleep                      ;Offset 0x508d
    push dx
    mov  dx, 0d2h
    call Sleep                          ;Offset 0x508e
    pop  dx
DontSleep:                              ;Offset 0x508d
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
Sleep PROC NEAR                         ;Offset 0x508e
    push ax
    push bx
    push cx
    push edx
    mov  al, PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2;0xb0
    out  PIT_ModeCommand, al            ;Port 0x43
    jmp  Delay1                         ;Offset 0x5099 Speed sensitive!
Delay1:                                 ;Offset 0x5099
    mov  ax, dx
    out  PIT_Channel2Data, al           ;Port 0x42
    jmp  Delay2                         ;Offset 0x509f Speed sensitive!
Delay2:                                 ;Offset 0x509f
    mov  al, ah
    out  PIT_Channel2Data, al           ;Port 0x42
    jmp  Delay3                         ;Offset 0x50a5 Speed sensitive!
Delay3:                                 ;Offset 0x50a5
    in   al, KB_PortB                   ;Port 0x61
    mov  cl, al
    and  cl, KB_PBR_LowerNibbleMask     ;0xf
    and  al, KB_PBR_Timer2ClkGateSpeakerStatus OR KB_PBR_NMIParityCheckStatus OR KB_PBR_NMIIOChanCheckStatus;0xd
    or   al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out  KB_PortB, al                   ;Port 0x61
    jmp  Delay4                         ;Offset 0x50b4 Speed sensitive!
Delay4:                                 ;Offset 0x50b4
    mov  edx, 004c4b40h                 ;5,000,000 time out
WaitLoop:                               ;Offset 0x50ba
    in   al, KB_PortB                   ;0x61
    test al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne  TimeOut                        ;Offset 0x50c4
    dec  edx
    jne  WaitLoop                       ;Offset 0x50ba
TimeOut:                                ;Offset 0x50c4
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

;Unused function
;
;Pattern:
;
;Pin0: 1111111111111111111
;Pin1: x010101010101010101
;
Func0x50d9 PROC NEAR                    ;Offset 0x50d9
    push cx
    push ds
    push 0000h
    pop  ds
    push word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    push ds
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
    mov  cx, 0009h
Label0x50ec:                            ;Offset 0x50ec
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    call SleepIfNeeded                  ;Offset 0x507f
    loop Label0x50ec                    ;Offset 0x50ec
    pop  ds
    pop  word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    pop  ds
    pop  cx
    ret
Func0x50d9 ENDP

;Pin1: 0
;
Func0x5104 PROC NEAR                    ;Offset 0x5104
    push ecx
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x507f
    pop  ecx
    ret
Func0x5104 ENDP

;
;Pin0: 11r...00
;Pin1: x1r...x0
;
;Note: Reads until both Pin0 and Pin1 are 1
SendStartBit PROC NEAR                  ;Offset 0x5110
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 0ah
WaitFor1:                               ;Offset 0x511e
    call SleepIfNeeded                  ;Offset 0x507f
    dec  cl
    je   TimeOut                        ;Offset 0x513c
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  WaitFor1                       ;Offset 0x511e
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jae  WaitFor1                       ;Offset 0x511e
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
TimeOut:                                ;Offset 0x513c
    mov  al, cl
    pop  ecx
    ret
SendStartBit ENDP

;
;Pin0: 0001
;Pin1: x011
;
SendStopBit PROC NEAR                   ;Offset 0x5141
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
    pop  ecx
    ret
SendStopBit ENDP

;
;Pin0: 1r...xxxxxx
;Pin1: xxxxx1r...0
;
WaitAck PROC NEAR                       ;Offset 0x5161
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    mov  cl, 0ah
Wait0:                                  ;Offset 0x5168
    dec  cl
    je   TimeOut                        ;Offset 0x518f
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jb   Wait0                          ;Offset 0x5168
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x517b
    dec  cl
    je   TimeOut                        ;Offset 0x518f
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x517b
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x507f
TimeOut:                                ;Offset 0x518f
    mov  al, cl
    pop  ecx
    ret
WaitAck ENDP

;
;Pin0: 0000001
;Pin1: x1r...0
;
SendAck PROC NEAR                       ;Offset 0x5194
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 0ah
Wait1:                                  ;Offset 0x51a3
    dec  cl
    je   TimeOut                        ;Offset 0x51ba
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x51a3
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
TimeOut:                                ;Offset 0x51ba
    mov  al, cl
    pop  ecx
    ret
SendAck ENDP

;
;Pin0: 1111111
;Pin1: x1r...0
;
Func0x51bf PROC NEAR                    ;Offset 0x51bf
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x51cd
    dec  cl
    je   TimeOut                        ;Offset 0x51e4
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x51cd
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x507f
TimeOut:                                ;Offset 0x51e4
    mov  al, cl
    pop  ecx
    ret
Func0x51bf ENDP

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
I2CSendByte PROC NEAR                   ;Offset 0x51e9
    push ecx
    push edx
    mov  cl, 08h
    mov  ch, al
NextBit:                                ;Offset 0x51f1
    shl  ch, 01h
    jb   Send1                          ;Offset 0x51fb
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    jmp  Sent0                          ;Offset 0x51fe
Send1:                                  ;Offset 0x51fb
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
Sent0:                                  ;Offset 0x51fe
    mov  dl, 0ah
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x5207
    dec  dl
    je   TimeOut                        ;Offset 0x521c
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x5207
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    dec  cl
    jne  NextBit                        ;Offset 0x51f1
TimeOut:                                ;Offset 0x521c
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
I2CReadByte PROC NEAR                   ;Offset 0x5223
    push ecx
    push edx
    xor  ch, ch
    mov  cl, 08h
NextBit:                                ;Offset 0x522b
    mov  dl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x5231
    dec  dl
    je   TimeOut                        ;Offset 0x5251
    call SleepIfNeeded                  ;Offset 0x507f
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x5231
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    rcl  ch, 01h
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x507f
    dec  cl
    jne  NextBit                        ;Offset 0x522b
    mov  al, ch
TimeOut:                                ;Offset 0x5251
    mov  ah, dl
    pop  edx
    pop  ecx
    ret
I2CReadByte ENDP

;
;inputs:
;   ecx   = 
;   edx   =
;   es:di = 
;
;
Func0x5258 PROC NEAR                    ;Offset 0x5258
    push     ecx
    shr      ecx, 08h
    xchg     ch, cl
    and      ecx, 0000ffffh
    cmp      ch, 0a0h
    jne      Label0x526f                ;Offset 0x526f
    call     Func0x5104                 ;Offset 0x5104
Label0x526f:                            ;Offset 0x526f
    cmp      cl, 00h
    je       Label0x5299                ;Offset 0x5299
    call     SendStartBit               ;Offset 0x5110
    or       al, al
    je       TimeOut                    ;Offset 0x534f
    mov      al, 60h                    ;Segment pointer address?
    call     I2CSendByte                ;Offset 0x51e9
    or       al, al
    je       SendStop                   ;Offset 0x5340
    call     WaitAck                    ;Offset 0x5161
    mov      al, cl                     ;Segment Pointer value?
    call     I2CSendByte                ;Offset 0x51e9
    or       al, al
    je       SendStop                   ;Offset 0x5340
    call     WaitAck                    ;Offset 0x5161
Label0x5299:                            ;Offset 0x5299
    mov      cl, 01h
Label0x529b:                            ;Offset 0x529b
    push     edx
    push     edi
    call     SendStartBit               ;Offset 0x5110
    or       al, al
    je       TimeOut                    ;Offset 0x534f
    or       dx, dx
    ;je       Label0x52f6                ;Offset 0x52f6
    DB 00Fh, 084h, 048h, 000h           ;Weird misassemble
    mov      al, ch
    and      al, 0feh                   ;DDC Write address?
    call     I2CSendByte                ;Offset 0x51e9
    or       al, al
    ;je       SendStop                   ;Offset 0x5340
    DB 00Fh, 084h, 085h, 000h           ;Weird misassemble
    call     WaitAck                    ;Offset 0x5161
    or       al, al
    ;je       SendStop                   ;Offset 0x5340
    DB 00Fh, 084h, 07Ch, 000h           ;Weird misassemble
Label0x52c4:                            ;Offset 0x52c4
    mov      al, byte ptr [di]          ;DDC Offset?
    call     I2CSendByte                ;Offset 0x51e9
    or       al, al
    je       SendStop                   ;Offset 0x5340
    call     WaitAck                    ;Offset 0x5161
    or       al, al
    je       SendStop                   ;Offset 0x5340
    inc      di
    dec      dx
    jne      Label0x52c4                ;Offset 0x52c4
    or       edx, edx
    ;je       Label0x5337                ;Offset 0x5337
    DB 00Fh, 084h, 058h, 000h           ;Weird misassemble
    and      ecx, 0000ffffh
    mov      al, 05h
Label0x52e8:                            ;Offset 0x52e8
    call     SleepIfNeeded              ;Offset 0x507f
    dec      al
    jne      Label0x52e8                ;Offset 0x52e8
    call     SendStartBit               ;Offset 0x5110
    or       al, al
    je       SendStop                   ;Offset 0x5340
Label0x52f6:                            ;Offset 0x52f6
    mov      al, ch
    or       al, 01h
    call     I2CSendByte                ;Offset 0x51e9
    or       al, al
    je       SendStop                   ;Offset 0x5340
    call     WaitAck                    ;Offset 0x5161
    or       al, al
    je       SendStop                   ;Offset 0x5340
    shr      edi, 10h
    shr      edx, 10h
    dec      dx
    ;je       Label0x532a                ;Offset 0x532a
    DB 00Fh, 084h, 015h, 000h           ;Weird misassemble
Label0x5315:                            ;Offset 0x5315
    call     I2CReadByte                ;Offset 0x5223
    or       ah, ah
    je       SendStop                   ;Offset 0x5340
    mov      byte ptr es:[di], al
    call     SendAck                    ;Offset 0x5194
    or       al, al
    je       SendStop                   ;Offset 0x5340
    inc      di
    dec      dx
    jne      Label0x5315                ;Offset 0x5315
Label0x532a:                            ;Offset 0x532a
    call     I2CReadByte                ;Offset 0x5223
    or       ah, ah
    je       SendStop                   ;Offset 0x5340
    mov      byte ptr es:[di], al
    call     Func0x51bf                 ;Offset 0x51bf
Label0x5337:                            ;Offset 0x5337
    mov      cl, 01h
    or       ecx, 00010000h
SendStop:                               ;Offset 0x5340
    call     SleepIfNeeded              ;Offset 0x507f
    call     SendStopBit                ;Offset 0x5141
    mov      al, 05h
Delay:                                  ;Offset 0x5348
    call     SleepIfNeeded              ;Offset 0x507f
    dec      al
    jne      Delay                      ;Offset 0x5348
TimeOut:                                ;Offset 0x534f
    pop      edi
    pop      edx
    dec      cl
    jne      Label0x529b                ;Offset 0x529b
    shr      ecx, 10h
    mov      ax, cx
    pop      ecx
    ret
Func0x5258 ENDP

Func0x5362 PROC NEAR                    ;Offset 0x5362
    push     ds
    push     edx
    push     esi
    push     es
    pop      ds
    xor      ax, ax
    mov      esi, edi
    shr      esi, 10h
    shr      edx, 10h
Label0x5376:                            ;Offset 0x5376
    lodsb
    add      ah, al
    dec      dx
    jne      Label0x5376                ;Offset 0x5376
    pop      esi
    pop      edx
    pop      ds
    ret
Func0x5362 ENDP

;
;
;
;
Func0x5382 PROC NEAR                    ;Offset 0x5382
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
    ;je       Label0x53a4                ;Offset 0x53a4
    DB 00Fh, 084h, 006h, 000h           ;Misassemble
    mov      word ptr ds:[BDA_SoftResetFlag], 0efh;Offset 0x472
Label0x53a4:                            ;Offset 0x53a4
    mov      bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    and      ecx, 0000ffffh
    mov      ch, byte ptr cs:[Data0x7ff5];Offset 0x7ff5
    mov      si, offset MISCFuncs       ;Offset 0x4bed
    mov      di, BDA_SoftResetFlag      ;Offset 0x472
    mov      edx, 00000001h
    call     Func0x5258                 ;Offset 0x5258
    pop      word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    sti
    pop      edx
    pop      ecx
    pop      di
    pop      si
    pop      bx
    pop      ds
    ret
Func0x5382 ENDP

;Offset 0x53d2
SupportedVESAVideoModes DW 0101h        ;640x480x256
                        DW 0110h        ;640x480x32K
                        DW 0111h        ;640x480x64K
                        DW 0112h        ;640x480x16M

;Offset 0x53da
Data0x53da              DW 0100h        ;640x400x256

;Offset 0x53dc
Data0x53dc              DW 0102h        ;800x600x16
                        DW 0103h        ;800x600x256
                        DW 0113h        ;800x600x32K
                        DW 0114h        ;800x600x64K
                        DW 0115h        ;800x600x16M

;Offset 0x53e6
Data0x53e6              DW 0105h        ;1024x768x256
                        DW 0116h        ;1024x768x32K
                        DW 0117h        ;1024x768x64K
                        DW 0118h        ;1024x768x16M

;Offset 0x53ee
Data0x53ee              DW 0107h        ;1280x1024x256
                        DW 0119h        ;1280x1024x32K
                        DW 011ah        ;1280x1024x64K
                        DW 011bh        ;1280x1024x16M

;Offset 0x53f6
Data0x53f6              DW 010ah        ;132x43 text
                        DW 0FFFFh

;Offset 0x53fa
Data0x53fa              DW offset Data0x564c            ;Offset 0x564c  0x100   640x400x256
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h
                        DW offset Data0x565a            ;Offset 0x565a  0x101   640x480x256
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h
                        DW offset Data0x5692            ;Offset 0x5692  0x102   800x600x16
                        DB 000h
                        DB 000h
                        DW offset Data0x5668            ;Offset 0x5668  0x103   800x600x256
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h
                        DW 00000h
                        DB 000h
                        DB 000h
                        DW offset Data0x5676            ;Offset 0x5676  0x105   1024x768x256
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h
                        DW 00000h
                        DB 000h
                        DB 000h
                        DW offset Data0x5684            ;Offset 0x5684  0x107   1280x1024x256
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h
                        DW 00000h
                        DB BDA_EFBS_FeatureConnector1_0 OR BDA_EFBS_FeatureConnector1_1;0xc0
                        DB 000h
                        DW 00000h
                        DB BDA_EFBS_FeatureConnector1_0 ;0x40
                        DB 000h
                        DW offset Data0x56a0            ;Offset 0x56a0  0x10a   132x43 text
                        DB BDA_EFBS_FeatureConnector1_0 OR BDA_EFBS_FeatureConnector1_1;0xc0
                        DB 000h
                        DW 00000h
                        DB BDA_EFBS_FeatureConnector1_0 OR BDA_EFBS_FeatureConnector1_1;0xc0
                        DB 000h
                        DW 0000h
                        DB BDA_EFBS_FeatureConnector1_0 OR BDA_EFBS_FeatureConnector1_1;0xc0
                        DB 000h
                        DW 00000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DB 000h
                        DB 000h
                        DW offset Data0x565a            ;Offset 0x565a  0x110   640x480x32K
                        DB BDA_EFBS_FeatureConnector0_1 ;0x20
                        DB 000h
                        DW offset Data0x565a            ;Offset 0x565a  0x111   640x480x64K
                        DB BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x60
                        DB 000h
                        DW offset Data0x565a            ;Offset 0x565a  0x112   640x480x16M
                        DB BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1;0x30
                        DB 000h
                        DW offset Data0x5668            ;Offset 0x5668  0x113   800x600x32K
                        DB BDA_EFBS_FeatureConnector0_1 ;0x20
                        DB 000h
                        DW offset Data0x5668            ;Offset 0x5668  0x114   800x600x64K
                        DB BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x60
                        DB 000h
                        DW offset Data0x5668            ;Offset 0x5668  0x115   800x600x16M
                        DB BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1;0x30
                        DB 000h
                        DW offset Data0x5676            ;Offset 0x5676  0x116   1024x768x32K
                        DB BDA_EFBS_FeatureConnector0_1 ;0x20
                        DB 000h
                        DW offset Data0x5676            ;Offset 0x5676  0x117   1024x768x64K
                        DB BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x60
                        DB 000h
                        DW offset Data0x5676            ;Offset 0x5676  0x118   1024x768x16M
                        DB BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1;0x30
                        DB 000h
                        DW offset Data0x5684            ;Offset 0x5684  0x119   1280x1024x32K
                        DB BDA_EFBS_FeatureConnector0_1 ;0x20
                        DB 000h
                        DW offset Data0x5684            ;Offset 0x5684  0x11a   1280x1024x64K
                        DB BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x60
                        DB 000h
                        DW offset Data0x5684            ;Offset 0x5684  0x11b   1280x1024x16M
                        DB BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1;0x30
                        DB 000h
                        DW offset Data0x564c            ;Offset 0x564c  0x11c   640x400x16 again?
                        DB BDA_EFBS_FeatureConnector0_0 ;0x10
                        DB 000h

;Offset 0x546e
Data0x546e              DB 008h, 010h, 001h, 008h, 001h, 004h, 000h

;Offset 0x5475
Data0x5475              DB 005h, 00Ah, 005h, 005h, 005h, 000h, 001h, 00Fh

;Offset 0x547d
Data0x547d              DB 005h, 00Bh, 006h, 005h, 005h, 000h, 000h, 000h

;Offset 0x5485
Data0x5485              DB 008h, 010h, 008h, 008h, 008h, 000h, 008h, 018h

;Offset 0x548d
Data0x548d              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 06Fh, 05Fh, 04Fh, 04Fh, 003h, 051h, 09Dh
                        DB 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0C1h, 023h, 08Fh, 050h, 000h, 08Fh
                        DB 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x54cd
Data0x54cd              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 05Fh, 04Fh, 04Fh, 003h, 051h, 09Dh
                        DB 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0E9h, 0ABh, 0DFh, 050h, 000h, 0DFh
                        DB 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x550d
Data0x550d              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 07Fh, 063h, 063h, 003h, 068h, 018h
                        DB 072h, 0F0h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 064h, 000h, 057h
                        DB 073h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x554d
Data0x554d              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 0A3h, 07Fh, 07Fh, 007h, 082h, 093h
                        DB 024h, 0F5h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 002h, 0A8h, 0FFh, 080h, 000h, 0FFh
                        DB 025h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x558d
Data0x558d              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 0CEh, 09Fh, 09Fh, 012h, 0A5h, 013h
                        DB 028h, 05Ah, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 0A3h, 0FFh, 0A0h, 000h, 0FFh
                        DB 029h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x55cc
Data0x55cc              DB 064h, 024h, 010h, 000h, 0F0h, 001h, 00Fh, 000h, 006h, 02Fh

;Offset 0x55d6
Data0x55d6              DB 07Ah, 063h, 063h, 080h, 06Bh, 01Bh
                        DB 072h, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 032h, 000h, 057h
                        DB 000h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x560c
Data0x560c              DB 084h, 02Ah, 008h, 058h, 02Ch, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh
                        DB 0CBh, 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 09Bh, 0ADh, 057h, 042h, 01Fh, 05Bh
                        DB 0C8h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x564c
Data0x564c              DD 000006257h   ;25,175 KHz
                        DW 0280h        ;640
                        DW 0190h        ;400
                        DB 01Ch, 013h
                        DW offset Data0x548d;Offset 0548dh
                        DB MGA_CRTCEXT1_HorBlankEnd;0x40
                        DB 000h

;Offset 0x565a
Data0x565a              DD 000006257h   ;25,175 KHz
                        DW 0280h        ;640
                        DW 01E0h        ;480
                        DB 01Ch, 013h
                        DW offset Data0x54cd;Offset 0x54cd
                        DB MGA_CRTCEXT1_HorBlankEnd;0x40
                        DB 000h

;Offset 0x5668
Data0x5668              DD 000009C40h   ;40,000 KHz
                        DW 0320h        ;800
                        DW 0258h        ;600
                        DB 01Ch, 013h
                        DW offset Data0x550d;Offset 0x550d
                        DB 000h
                        DB 000h

;Offset 0x5676
Data0x5676              DD 00000FDE8h   ;65,000 KHz
                        DW 0400h        ;1024
                        DW 0300h        ;768
                        DB 01Ch, 013h
                        DW offset Data0x554d;Offset 0x554d
                        DB 000h
                        DB 000h

;Offset 0x5684
Data0x5684              DD 00001A5E0h   ;108,000 KHz
                        DW 0500h        ;1280
                        DW 0400h        ;1024
                        DB 01Ch, 013h
                        DW offset Data0x558d;Offset 0x558d
                        DB MGA_CRTCEXT1_HorBlankEnd;0x40
                        DB MGA_CRTCEXT2_VertRetrStrt10 OR MGA_CRTCEXT2_VertTotal10;021h

;Offset 0x5692
Data0x5692              DD 000009858h   ;39,000 KHz
                        DW 0320h        ;800
                        DW 0258h        ;600
                        DB 01Bh, 012h
                        DW offset Data0x55cc;Offset 0x55cc
                        DB 000h
                        DB 000h

;Offset 0x56a0
Data0x56a0              DD 00000A078h   ;41,080 KHz
                        DW 0420h        ;1056
                        DW 0158h        ;344
                        DB 018h, 003h
                        DW offset Data0x560c;Offset 0x560c
                        DB 000h
                        DB 000h

;Offset 0x56ae
VESACallTable           DW offset GetSuperVGAInformation;Offset 0x57a7          ;0
                        DW offset GetSuperVGAModeInformation;Offset 0x5847      ;1
                        DW offset SetSuperVGAVideoMode;Offset 0x5a66            ;2
                        DW offset SuperVGAGetCurrentVideoMode;Offset 0x5bfb     ;3
                        DW offset SaveRestoreSuperVGAVideoState;Offset 0x5c23   ;4
                        DW offset CPUVideoMemoryControl;Offset 0x5ca2           ;5
                        DW offset LogicalScanLineLengthHandling;Offset 0x5caa   ;6
                        DW offset DisplayStartHandling;Offset 0x5d22            ;7
                        DW offset GetSetDACPaletteControl;Offset 0x5db8         ;8
                        DW offset GetSetPaletteEntries;Offset 0x5e01            ;9
                        DW offset GetProtectedModeInterface;Offset 0x5e73       ;a
                        DW offset GetNearestPixelClock;Offset 0x5e93            ;b

;Offset 0x56c6
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

;Offset 0x56e8
Data0x56e8              DB 01Bh, 000h, 007h, 000h, 040h, 000h, 040h, 000h
                        DB 000h, 0A0h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x56f8
Data0x56f8              DB 00Fh, 000h, 007h, 000h, 020h, 000h, 020h, 000h
                        DB 000h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x5708
OEMName                 DB 'Matrox Graphics Inc.', 000h

;Offset 0x571d
VendorName              DB 'Matrox', 000h

;Offset 0x5724
ProductName             DB 'MGA-G200', 000h

;Offset 0x572d
RevisionString          DB '00', 000h

;Offset 0x5730
                        DB 'VBE/MGA'
                        DB 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh
                        DW 00303h       ;BIOS Version (3.3)

VESAHandler:                            ;Offset 0x5743
    cmp       al, 14h                   ;Matrox OEM extension (bl=00h = installation check / get caps) https://www.ctyme.com/intr/rb-0306.htm
    je        Label0x5757               ;Offset 0x5757
    cmp       al, 0bh
    ja        Label0x5768               ;Offset 0x5768
    sti
    cld
    xor       ah, ah
    shl       ax, 01h
    xchg      ax, bx
    jmp       word ptr cs:[bx + VESACallTable];Offset 0x56ae
Label0x5757:                            ;Offset 0x5757
    cmp       bl, 03h
    je        Label0x5ed0               ;Offset 0x5ed0
    cmp       bl, 05h
    je        Label0x5edf               ;Offset 0x5edf
    jmp       Label0x5768               ;Offset 0x5768
    nop
Label0x5768:                            ;Offset 0x5768
    mov       ax, 0ffffh
    iret

Func0x576c PROC NEAR                    ;Offset 0x576c
    push      cx
    mov       cx, bx
    and       cx, 01ffh
    cmp       cx, 006ah
    je        Label0x577e               ;Offset 0x577e
    cmp       cx, 0102h
    jne       Label0x5786               ;Offset 0x5786
Label0x577e:                            ;Offset 0x577e
    and       bx, 0f600h
    or        bx, 0102h
Label0x5786:                            ;Offset 0x5786
    pop       cx
    ret       
Func0x576c ENDP

Func0x5788 PROC NEAR                    ;Offset 0x5788
    push      ax
    push      bx
    push      si
    push      ds
    push      cs
    pop       ds
    and       bx, 01ffh
    lea       si, [SupportedVESAVideoModes];Offset 0x53d2
Label0x5796:                            ;Offset 0x5796
    lodsw
    cmp       bx, ax
    je        Label0x57a2               ;Offset 0x57a2
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    jne       Label0x5796               ;Offset 0x5796
    or        ax, ax
Label0x57a2:                            ;Offset 0x57a2
    pop       ds
    pop       si
    pop       bx
    pop       ax
    ret
Func0x5788 ENDP

GetSuperVGAInformation:                 ;Offset 0x57a7
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
    jne       NoVBE2Requested           ;Offset 0x57c0
    mov       dx, 0001h
NoVBE2Requested:                        ;Offset 0x57c0
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       cx, 0022h
    mov       si, offset VESAResponseTemplate;Offset 0x56c6
    rep movsb
    pop       di
    call      GetMemoryIn64KBlocks      ;Offset 0x665c
    mov       word ptr es:[di + VESA_INFO_MemoryIn64KBBlocks], ax;0x12
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerSeg], cs;0x10
    mov       ax, offset SupportedVESAVideoModes;Offset 0x53d2
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerOfs], ax;0xe
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerSeg], cs;0x8
    mov       ax, offset OEMName        ;Offset 0x5708
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerOfs], ax;0x6
    cmp       dx, 0001h
    jne       DontFillOutVBE2Info       ;Offset 0x583e
    mov       ax, 0033h                 ;OEM Version 3.3
    shl       ax, 04h
    mov       al, 0033h                 ;OEM Version 3.3
    and       al, 0fh
    mov       word ptr es:[di + VESA_INFO_OEMSoftwareVersion], ax;0x14
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerSeg], ax;0x18
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerSeg], ax;0x1c
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerSeg], ax;0x20
    mov       ax, offset VendorName     ;Offset 0x571d
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerOfs], ax;0x16
    mov       ax, offset ProductName    ;Offset 0x5724
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerOfs], ax;0x1a
    mov       ax, offset RevisionString ;Offset 0x572d
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerOfs], ax;0x1e
    add       di, 0100h
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    mov       cx, 0013h
    lea       si, [VendorName]          ;Offset 0x571d
    rep movsb
DontFillOutVBE2Info:                    ;Offset 0x583e
    mov       ax, 004fh
    pop       dx
    pop       cx
    pop       si
    pop       di
    pop       ds
    iret      

GetSuperVGAModeInformation:             ;Offset 0x5847
    push      ax
    push      ds
    push      di
    push      si
    push      ebx
    push      ecx
    push      edx
    mov       bx, cx
    mov       dx, 0000h
    call      Func0x576c                ;Offset 0x576c
    cmp       bl, 0ffh
    jne       Label0x5863               ;Offset 0x5863
    mov       bx, 0116h
    jmp       Label0x5879               ;Offset 0x5879
Label0x5863:                            ;Offset 0x5863
    mov       ah, 01h
    call      Func0x5788                ;Offset 0x5788
    je        Label0x5879               ;Offset 0x5879
    mov       ax, 0af01h
    int       6dh
    cmp       ax, 0af01h
    je        Label0x5a59               ;Offset 0x5a59
    mov       dx, 0001h
Label0x5879:                            ;Offset 0x5879
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
    lea       si, [Data0x56e8]          ;Offset 0x56e8
    cmp       bl, 08h
    jb        Label0x589f               ;Offset 0x589f
    cmp       bl, 0ch
    ja        Label0x589f               ;Offset 0x589f
    lea       si, [Data0x56f8]          ;Offset 0x56f8
Label0x589f:                            ;Offset 0x589f
    rep movsb
    pop       si
    pop       ds
    or        dx, dx
    jne       Label0x58b5               ;Offset 0x58b5
    push      cs
    pop       ds
    lea       si, [Data0x53fa]          ;Offset 0x53fa
    and       bx, 001fh
    shl       bx, 02h
    add       si, bx
Label0x58b5:                            ;Offset 0x58b5
    mov       bx, word ptr [si + 02h]
    mov       si, word ptr [si]
    mov       ax, cs
    mov       word ptr es:[di - 02h], ax
    lea       ax, [Func0x5c72]          ;Offset 0x5c72
    mov       word ptr es:[di - 04h], ax
    push      ds
    push      si
    push      di
    push      cs
    pop       ds
    lea       si, [Data0x546e]          ;Offset 0x546e
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
    je        Label0x590a               ;Offset 0x590a
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x590a               ;Offset 0x590a
    mov       dx, 0500h
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x590a               ;Offset 0x590a
    mov       dx, 0400h
Label0x590a:                            ;Offset 0x590a
    mov       byte ptr es:[di + 0eh], 01h
    mov       ax, bx
    ;and       ax, 30h
    DB 025h, 030h, 000h
    je        Label0x5973               ;Offset 0x5973
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
    je        Label0x59af               ;Offset 0x59af
    mov       byte ptr es:[di + 0bh], 06h
    push      cs
    pop       ds
    lea       si, [Data0x5475]          ;Offset 0x5475
    cmp       bx, 0020h
    je        Label0x5956               ;Offset 0x5956
    lea       si, [Data0x547d]          ;Offset 0x547d
    cmp       bx, 0060h
    je        Label0x5956               ;Offset 0x5956
    lea       si, [Data0x5485]          ;Offset 0x5485
Label0x5956:                            ;Offset 0x5956
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
    jmp       Label0x59af               ;Offset 0x59af
Label0x5973:                            ;Offset 0x5973
    shr       dx, 03h
    mov       word ptr es:[di], dx
    mov       byte ptr es:[di + 08h], 04h
    mov       byte ptr es:[di + 09h], 04h
    test      bx, 40h
    je        Label0x59a5               ;Offset 0x59a5
    shr       word ptr es:[di + 02h], 03h
    shr       word ptr es:[di + 04h], 03h
    test      bx, 80h
    je        Label0x599e               ;Offset 0x599e
    mov       byte ptr es:[di + 07h], 08h
Label0x599e:                            ;Offset 0x599e
    mov       byte ptr es:[di + 0bh], 00h
    jmp       Label0x59af               ;Offset 0x59af
Label0x59a5:                            ;Offset 0x59a5
    mov       byte ptr es:[di + 07h], 0eh
    mov       byte ptr es:[di + 0bh], 03h
Label0x59af:                            ;Offset 0x59af
    mov       si, bx
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call      ReadPCIBase0AndBase1      ;Offset 0x698d
    pop       dx
    push      es
    push      0000h
    pop       es
    push      word ptr es:[BDA_DisplayMode];Offset 0x449
    add       dl, 20h
    mov       byte ptr es:[BDA_DisplayMode], dl;Offset 0x449
    call      Func0x686a                ;Offset 0x686a
    pop       word ptr es:[BDA_DisplayMode];Offset 0x449
    pop       es
    mov       eax, ebx
    and       eax, 000000ffh
    mov       edx, 00010000h
    mul       edx
    add       ecx, eax
    test      si, 0030h
    je        Label0x59f4               ;Offset 0x59f4
    ;mov       dword ptr es:[di + 18h], ecx
    DB 026h, 066h, 089h, 04Dh, 018h
Label0x59f4:                            ;Offset 0x59f4
    mov       ecx, eax
    test      si, 0030h
    je        Label0x5a4c               ;Offset 0x5a4c
    mov       bx, word ptr es:[di]
    mov       ax, word ptr es:[di + 04h]
    or        bx, bx
    je        Label0x5a4c               ;Offset 0x5a4c
    xor       dx, dx
    mul       bx
    mov       bx, 0ffffh
    div       bx
    or        dx, dx
    je        Label0x5a16               ;Offset 0x5a16
    inc       ax
Label0x5a16:                            ;Offset 0x5a16
    xor       dx, dx
    mul       bx
    shl       edx, 10h
    mov       dx, ax
    mov       ebx, edx
    xor       edx, edx
    call      GetMemoryIn64KBlocks      ;Offset 0x665c
    cmp       ax, 0100h
    jbe       Label0x5a31               ;Offset 0x5a31
    mov       ax, 0100h
Label0x5a31:                            ;Offset 0x5a31
    shl       eax, 10h
    shl       ecx, 01h
    sub       eax, ecx
    div       ebx
    dec       al
    mov       byte ptr es:[di + 0dh], al
    mov       byte ptr es:[di + 24h], al
    mov       byte ptr es:[di + 25h], al
Label0x5a4c:                            ;Offset 0x5a4c
    ;mov       dword ptr es:[di + 2eh], 0cfe6a80h
    DB 026h, 066h, 0C7h, 045h, 02Eh, 080h, 06Ah, 0FEh, 00Ch
    pop       eax
    mov       ah, 00h
Label0x5a59:                            ;Offset 0x5a59
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
SetSuperVGAVideoMode:                   ;Offset 0x5a66
    push      ax
    push      ds
    push      di
    push      si
    push      cx
    push      cs
    pop       ds
    mov       bx, ax
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x6614
    je        Exit                      ;Offset 0x5bf5
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    je        Label0x5a82               ;Offset 0x5a82
    and       bx, 0f7ffh
Label0x5a82:                            ;Offset 0x5a82
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x5a8d               ;Offset 0x5a8d
    and       bh, 7fh
Label0x5a8d:                            ;Offset 0x5a8d
    push      bx
    call      Func0x576c                ;Offset 0x576c
    cmp       bl, 0ffh
    jne       Label0x5a99               ;Offset 0x5a99
    mov       bx, 0100h
Label0x5a99:                            ;Offset 0x5a99
    mov       ax, bx
    push      ax
    mov       ax, 0af02h
    int       6dh
    cmp       ax, 0af02h
    pop       ax
    mov       cx, si
    jne       Label0x5ab3               ;Offset 0x5ab3
    mov       cx, 0000h
    call      Func0x5788                ;Offset 0x5788
    jne       Label0x5bde               ;Offset 0x5bde
Label0x5ab3:                            ;Offset 0x5ab3
    lea       si, [Data0x53fa]          ;Offset 0x53fa
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
    jne       Label0x5ada               ;Offset 0x5ada
    mov       cx, word ptr cs:[si]
Label0x5ada:                            ;Offset 0x5ada
    push      bp
    mov       bp, sp
    test      ax, 0800h
    je        Label0x5bc3               ;Offset 0x5bc3
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
    call      Func0x66b1                ;Offset 0x66b1
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
Label0x5bc3:                            ;Offset 0x5bc3
    push      ds
    push      cx
    call      Func0x62c3                ;Offset 0x62c3
    pop       ax
    pop       ax
    mov       sp, bp
    pop       bp
    pop       bx
    cmp       bl, 0ffh
    jne       Label0x5bf2               ;Offset 0x5bf2
    push      dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out       dx, ax
    pop       dx
    jmp       Label0x5bf2               ;Offset 0x5bf2
Label0x5bde:                            ;Offset 0x5bde
    pop       bx
    mov       ax, bx
    and       al, 7fh
    ;cmp       ax, 0013h
    DB 03Dh, 013h, 000h
    jle       Label0x5bed               ;Offset 0x5bed
    mov       ax, 014fh
    jmp       Exit                      ;Offset 0x5bf5
Label0x5bed:                            ;Offset 0x5bed
    movzx     ax, bl
    int       10h
Label0x5bf2:                            ;Offset 0x5bf2
    mov       ax, 004fh
Exit:                                   ;Offset 0x5bf5
    pop       cx
    pop       si
    pop       di
    pop       ds
    pop       bx
    iret      

SuperVGAGetCurrentVideoMode:            ;Offset 0x5bfb
    mov       ax, 0f00h
    int       10h
    xor       bx, bx
    mov       ah, al
    and       ah, 0c0h
    and       al, 3fh
    cmp       al, 20h
    jl        Label0x5c1d               ;Offset 0x5c1d
    cmp       al, 40h
    jge       Label0x5c1d               ;Offset 0x5c1d
    sub       al, 20h
    inc       bh
    or        bh, ah
    cmp       al, 16h
    jne       Label0x5c1d               ;Offset 0x5c1d
    mov       al, 0ffh
Label0x5c1d:                            ;Offset 0x5c1d
    mov       bl, al
    mov       ax, 004fh
    iret      

SaveRestoreSuperVGAVideoState:          ;Offset 0x5c23
    mov       bx, ax
    mov       ah, 1ch
    mov       al, dl
    push      ax
    push      cx
    test      cx, 0008h
    je        Label0x5c5c               ;Offset 0x5c5c
    cmp       al, 00h
    jne       Label0x5c42               ;Offset 0x5c42
    cmp       cx, 0008h
    jne       Label0x5c5c               ;Offset 0x5c5c
    mov       bx, 0001h
    nop
    pop       cx
    pop       ax
    jmp       Label0x5c6e               ;Offset 0x5c6e
Label0x5c42:                            ;Offset 0x5c42
    cmp       al, 01h
    jne       Label0x5c4f               ;Offset 0x5c4f
    push      bx
    call      SaveMGAState              ;Offset 0x5f1e
    int       10h
    pop       bx
    jmp       Label0x5c5e               ;Offset 0x5c5e
Label0x5c4f:                            ;Offset 0x5c4f
    cmp       al, 02h
    jne       Label0x5c5c               ;Offset 0x5c5c
    push      bx
    call      RestoreMGAState           ;Offset 0x5f8b
    int       10h
    pop       bx
    jmp       Label0x5c5e               ;Offset 0x5c5e
Label0x5c5c:                            ;Offset 0x5c5c
    int       10h
Label0x5c5e:                            ;Offset 0x5c5e
    pop       cx
    pop       ax
    test      cx, 0008h
    je        Label0x5c6e               ;Offset 0x5c6e
    cmp       al, 00h
    jne       Label0x5c6e               ;Offset 0x5c6e
    ;add       bx, 0001h
    DB 081h, 0C3h, 001h, 000h
Label0x5c6e:                            ;Offset 0x5c6e
    mov       ax, 004fh
    iret      

Func0x5c72 PROC FAR                     ;Offset 0x5c72
    call      Func0x5c76                ;Offset 0x5c76
    retf
Func0x5c72 ENDP

Func0x5c76 PROC NEAR                    ;Offset 0x5c76
    push      ebx
    mov       ah, 02h
    or        bl, bl
    jne       Label0x5c9f               ;Offset 0x5c9f
    mov       ah, bh
    call      Func0x686a                ;Offset 0x686a
    or        ah, ah
    jne       Label0x5c93               ;Offset 0x5c93
    mov       ah, MGA_CRTCExt_MemoryPage;0x4
    mov       al, dl
    add       al, bl
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    jmp       Label0x5c9d               ;Offset 0x5c9d
    nop
Label0x5c93:                            ;Offset 0x5c93
    mov       al, MGA_CRTCExt_MemoryPage;0x4
    call      MGAReadCRTCExtensionRegister;Offset 0x2c0
    sub       al, bl
    movzx     dx, al
Label0x5c9d:                            ;Offset 0x5c9d
    mov       ah, 00h
Label0x5c9f:                            ;Offset 0x5c9f
    pop       ebx
    ret
Func0x5c76 ENDP

CPUVideoMemoryControl:                  ;Offset 0x5ca2
    mov       bx, ax
    call      Func0x5c76                ;Offset 0x5c76
    mov       al, 4fh
    iret

LogicalScanLineLengthHandling:          ;Offset 0x5caa
    mov       bx, ax
    call      Func0x63f6                ;Offset 0x63f6
    je        Label0x5cfd               ;Offset 0x5cfd
    push      ds
    push      si
    cmp       bl, 01h
    je        Label0x5cf4               ;Offset 0x5cf4
    call      Func0x6677                ;Offset 0x6677
    cmp       bl, 03h
    jne       Label0x5cc7               ;Offset 0x5cc7
    call      Func0x65be                ;Offset 0x65be
    mov       cx, ax
    jmp       Label0x5cf7               ;Offset 0x5cf7
Label0x5cc7:                            ;Offset 0x5cc7
    cmp       bl, 02h
    jne       Label0x5cd4               ;Offset 0x5cd4
Label0x5ccc:                            ;Offset 0x5ccc
    dec       al
    je        Label0x5cd4               ;Offset 0x5cd4
    shr       cx, 01h
    jmp       Label0x5ccc               ;Offset 0x5ccc
Label0x5cd4:                            ;Offset 0x5cd4
    ;mov       ax, word ptr [si + 04h]
    DB 08Bh, 084h, 004h, 000h
    cmp       ax, cx
    jb        Label0x5cde               ;Offset 0x5cde
    mov       cx, ax
Label0x5cde:                            ;Offset 0x5cde
    mov       ax, cx
    call      Func0x659e                ;Offset 0x659e
    mov       cx, ax
    call      Func0x65be                ;Offset 0x659e
    cmp       ax, cx
    mov       ax, 024fh
    jb        Label0x5cfa               ;Offset 0x5cfa
    mov       ax, cx
    call      Func0x6569                ;Offset 0x6569
Label0x5cf4:                            ;Offset 0x5cf4
    call      Func0x6523                ;Offset 0x6523
Label0x5cf7:                            ;Offset 0x5cf7
    mov       ax, 004fh
Label0x5cfa:                            ;Offset 0x5cfa
    pop       si
    pop       ds
    iret
Label0x5cfd:                            ;Offset 0x5cfd
    cmp       bl, 03h
    jne       Label0x5d07               ;Offset 0x5d07
    mov       cx, 0320h
    jmp       Label0x5d1e               ;Offset 0x5d1e
Label0x5d07:                            ;Offset 0x5d07
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
Label0x5d1e:                            ;Offset 0x5d1e
    mov       ax, 004fh
    iret

DisplayStartHandling:                   ;Offset 0x5d22
    mov       bx, ax
    call      Func0x63f6                ;Offset 0x63f6
    je        Label0x5d47               ;Offset 0x5d47
    cmp       bl, 01h
    je        Label0x5d40               ;Offset 0x5d40
    or        bl, bl
    je        Label0x5d39               ;Offset 0x5d39
    cmp       bl, 80h
    ;jne       Label0x5db2               ;Offset 0x5db2
    DB 00Fh, 085h, 079h, 000h
Label0x5d39:                            ;Offset 0x5d39
    call      Func0x64e5                ;Offset 0x64e5
    mov       ax, 004fh
    iret
Label0x5d40:                            ;Offset 0x5d40
    call      Func0x64b0                ;Offset 0x64b0
    mov       ax, 004fh
    iret
Label0x5d47:                            ;Offset 0x5d47
    push      di
    push      ds
    push      0000h
    pop       ds
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop       ds
    mov       di, 0064h
    cmp       al, 22h
    je        Label0x5d61               ;Offset 0x5d61
    mov       di, 0050h
    cmp       al, 28h
    je        Label0x5d61               ;Offset 0x5d61
    mov       di, 0084h
Label0x5d61:                            ;Offset 0x5d61
    and       bl, 01h
    jne       Label0x5d8d               ;Offset 0x5d8d
    or        bl, bl
    je        Label0x5d6f               ;Offset 0x5d6f
    cmp       bl, 80h
    jne       Label0x5db2               ;Offset 0x5db2
Label0x5d6f:                            ;Offset 0x5d6f
    push      cx
    push      dx
    xor       al, al
    call      Func0x6410                ;Offset 0x6410
    mov       ax, di
    mul       dx
    push      dx
    push      ax
    pop       eax
    shr       cx, 03h
    add       ax, cx
    call      SetStartAddress           ;Offset 0x6466
    pop       dx
    pop       cx
    pop       di
    mov       ax, 004fh
    iret
Label0x5d8d:                            ;Offset 0x5d8d
    call      GetStartAddress           ;Offset 0x6433
    xor       cx, cx
    xor       dx, dx
    or        eax, eax
    je        Label0x5da8               ;Offset 0x5da8
    push      eax
    pop       ax
    pop       dx
    mov       bx, di
    div       bx
    mov       cx, dx
    shl       cx, 03h
    mov       dx, ax
Label0x5da8:                            ;Offset 0x5da8
    mov       al, 01h
    call      Func0x6410                ;Offset 0x6410
    pop       di
    mov       ax, 004fh
    iret
Label0x5db2:                            ;Offset 0x5db2
    pop       di
    mov       ah, 01h
    mov       al, 4fh
    iret

GetSetDACPaletteControl:                ;Offset 0x5db8
    push      cx
    push      dx
    mov       dx, ax
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    mov       ah, 01h
    or        dl, dl
    je        Label0x5de0               ;Offset 0x5de0
    cmp       dl, 01h
    jne       Label0x5dfa               ;Offset 0x5dfa
    xor       ah, ah
    mov       dh, 06h
    and       cl, MGA_MISCCTRL_DAC_8Bit ;08h
    je        Label0x5dfa               ;Offset 0x5dfa
    mov       dh, 08h
    jmp       Label0x5dfa               ;Offset 0x5dfa
Label0x5de0:                            ;Offset 0x5de0
    and       cl, NOT MGA_MISCCTRL_DAC_8Bit;0xf7
    cmp       dh, 08h
    mov       dh, 06h
    jb        Label0x5def               ;Offset 0x5def
    mov       dh, 08h
    or        cl, MGA_MISCCTRL_DAC_8Bit ;0x8
Label0x5def:                            ;Offset 0x5def
    xchg      cl, ch
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6a6b
    xor       ah, ah
Label0x5dfa:                            ;Offset 0x5dfa
    mov       bx, dx
    pop       dx
    pop       cx
    mov       al, 004fh
    iret      

GetSetPaletteEntries:                   ;Offset 0x5e01
    mov       bx, ax
    cmp       bl, 02h
    je        Label0x5e6f               ;Offset 0x5e6f
    cmp       bl, 03h
    je        Label0x5e6f               ;Offset 0x5e6f
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
    jne       Label0x5e32               ;Offset 0x5e32
    mov       dl, 0dah
    xor       cx, cx
Label0x5e22:                            ;Offset 0x5e22
    dec       cx
    je        Label0x5e32               ;Offset 0x5e32
    in        al, dx
    and       al, 08h
    jne       Label0x5e22               ;Offset 0x5e22
Label0x5e2a:                            ;Offset 0x5e2a
    dec       cx
    je        Label0x5e32               ;Offset 0x5e32
    in        al, dx
    and       al, 08h
    je        Label0x5e2a               ;Offset 0x5e2a
Label0x5e32:                            ;Offset 0x5e32
    pop       cx
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    mov       al, bl
    and       al, 01h
    je        Label0x5e52               ;Offset 0x5e52
Label0x5e3b:                            ;Offset 0x5e3b
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
    jne       Label0x5e3b               ;Offset 0x5e3b
    jmp       Label0x5e67               ;Offset 0x5e67
Label0x5e52:                            ;Offset 0x5e52
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
    jne       Label0x5e52               ;Offset 0x5e52
Label0x5e67:                            ;Offset 0x5e67
    pop       di
    pop       dx
    pop       cx
    pop       bx
    mov       ax, 004fh
    iret
Label0x5e6f:                            ;Offset 0x5e6f
    mov       ax, 024fh
    iret

GetProtectedModeInterface:              ;Offset 0x5e73
    mov       bx, ax
    mov       ah, 01h
    or        bl, bl
    jne       Label0x5e90               ;Offset 0x5e90
    mov       ax, cs
    mov       es, ax
    mov       di, offset PINS           ;Offset 0x7ce0
    xor       ax, ax
    mov       al, byte ptr es:[di + 02h]
    add       di, ax
    mov       cx, word ptr es:[di + 08h]
    xor       ah, ah
Label0x5e90:                            ;Offset 0x5e90
    mov       al, 004fh
    iret

;inputs:
;    ax = sub function (0 = request), (not 0 = set)
;   ecx = Requested clock frequency in Hz
;    dx = mode number (unused)
;
;outputs (sub function 0):
;   ecx = nearest clock rate in Hz
;
GetNearestPixelClock:                   ;Offset 0x5e93
    push      ax
    push      si
    push      edx
    push      eax
    push      ebx
    mov       si, ax
    xor       edx, edx
    mov       eax, ecx
    mov       ecx, 000003e8h            ;1,000
    div       ecx                       ;divide clock rate by 1,000 (so convert to Khz)
    mov       bx, 0002h                 ;Unused in function, stomped
    nop
    call      CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    mul       ecx                       ;multiply by 1,000 (back to Hz)
    mov       ecx, eax
    and       si, 00ffh
    je        Label0x5ec4               ;Offset 0x5ec4
    mov       cl, MGA_CPC_PLLSetC OR MGA_CPC_ClockPLL;0x6
    call      ConfigurePixelClocks      ;Offset 0x6e51
Label0x5ec4:                            ;Offset 0x5ec4
    pop       ebx
    pop       eax
    mov       ax, 004fh
    pop       edx
    pop       si
    pop       bx
    iret

Label0x5ed0:                            ;Offset 0x5ed0
    push      si
    mov       bx, ax
    add       si, offset PINS           ;Offset 0x7ce0
    mov       cl, byte ptr cs:[si]
    mov       ax, 004fh
    pop       si
    iret

Label0x5edf:                            ;Offset 0x5edf
    mov       bx, ax
    mov       cl, byte ptr cs:[si]
    mov       ax, 004fh
    iret

;Offset 0x5ee8
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
                        DB 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

SaveMGAState PROC NEAR                  ;Offset 0x5f1e
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
LoopIndexedRegisters:                   ;Offset 0x5f2f
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5ee8
    cmp  cl, 0ffh
    je   EndOfIndexedRegisterData       ;Offset 0x5f43
    call ReadIndexedRegister            ;Offset 0x6a80
    mov  byte ptr es:[di], cl
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x5f2f
EndOfIndexedRegisterData:               ;Offset 0x5f43
    mov  byte ptr es:[di], 0ffh
    inc  di
    mov  si, di
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    nop
    mov  di, PCI_MGA_Option             ;0x40
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    ;mov  dword ptr es:[si], ecx
    DB 026h, 066h, 089h, 00Ch
    add  si, 0004h
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    nop
    mov  di, PCI_MGA_Option2            ;0x50
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    ;mov  dword ptr es:[si], ecx
    DB 026h, 066h, 089h, 00Ch
    add  si, 0004h
    mov  di, si
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x5f73
    mov  al, cl
    out  dx, al
    in   ax, dx
    mov  byte ptr es:[di], ah
    inc  di
    inc  cl
    cmp  cl, 08h
    jbe  LoopExtensionRegisters         ;Offset 0x5f73
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  di
    pop  si
    pop  ds
    ret
SaveMGAState ENDP

RestoreMGAState PROC NEAR               ;Offset 0x5f8b
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
LoopIndexedRegisters:                   ;Offset 0x5f9c
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5ee8
    cmp  cl, 0ffh
    je   EndOfIndexedRegisterData       ;Offset 0x5fb0
    mov  ch, byte ptr es:[di]
    call WriteIndexedRegister           ;Offset 0x6a6b
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x5f9c
EndOfIndexedRegisterData:               ;Offset 0x5fb0
    inc  di
    mov  si, di
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    nop
    ;mov  ecx, dword ptr es:[si]
    DB 026h, 066h, 08Bh, 00Ch
    mov  di, PCI_MGA_Option             ;0x40
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    add  si, 0004h
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    nop
    ;mov  ecx, dword ptr es:[si]
    DB 026h, 066h, 08Bh, 00Ch
    mov  di, PCI_MGA_Option2            ;0x50
    nop
    call AccessPCIRegister              ;Offset 0x68f8
    add  si, 0004h
    mov  di, si
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x5fdc
    mov  al, cl
    mov  ah, byte ptr es:[di]
    out  dx, ax
    inc  di
    inc  cl
    cmp  cl, 08h
    jbe  LoopExtensionRegisters         ;Offset 0x5fdc
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  si
    pop  di
    pop  ds
    ret
RestoreMGAState ENDP

Func0x5ff3 PROC NEAR                    ;Offset 0x5ff3
    cmp  al, 20h
    jb   Label0x600b                    ;Offset 0x600b
    cmp  al, 22h
    je   Label0x6005                    ;Offset 0x6005
    cmp  al, 28h
    jb   Label0x6009                    ;Offset 0x6009
    cmp  al, 2dh
    mov  al, 03h
    jb   Label0x600b                    ;Offset 0x600b
Label0x6005:                            ;Offset 0x6005
    mov  al, 12h
    jmp  Label0x600b                    ;Offset 0x600b
Label0x6009:                            ;Offset 0x6009
    mov  al, 13h
Label0x600b:                            ;Offset 0x600b
    ret
Func0x5ff3 ENDP

Func0x600c PROC NEAR                    ;Offset 0x600c
    push dx
    push cx
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, MGA_MULCTRL_DepthMask      ;0x7
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    xor  al, al
    out  dx, al
    inc  dx
    cmp  cl, MGA_MULCTRL_Depth32Pal     ;0x7
    je   Label0x602b                    ;Offset 0x602b
    cmp  cl, MGA_MULCTRL_Depth16        ;0x2
    je   Label0x6034                    ;Offset 0x6034
    jmp  Label0x604e                    ;Offset 0x604e
Label0x602b:                            ;Offset 0x602b
    out  dx, al
    out  dx, al
    out  dx, al
    inc  al
    jne  Label0x602b                    ;Offset 0x602b
    jmp  Label0x605c                    ;Offset 0x605c
Label0x6034:                            ;Offset 0x6034
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
    jne  Label0x6034                    ;Offset 0x6034
    jmp  Label0x605c                    ;Offset 0x605c
Label0x604e:                            ;Offset 0x604e
    mov  cl, al
    shl  al, 03h
    out  dx, al
    out  dx, al
    out  dx, al
    mov  al, cl
    inc  al
    jne  Label0x604e                    ;Offset 0x604e
Label0x605c:                            ;Offset 0x605c
    pop  cx
    pop  dx
    ret
Func0x600c ENDP

Func0x605f PROC NEAR                    ;Offset 0x605f
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
    jne  Label0x607a                    ;Offset 0x607a
    shr  ax, 01h
    jmp  Label0x607c                    ;Offset 0x607c
Label0x607a:                            ;Offset 0x607a
    shl  ax, cl
Label0x607c:                            ;Offset 0x607c
    out  dx, al
    mov  dl, MGA_CRTCExtensionData_lowbyte;Port 0x3df
    mov  al, ah
    shl  al, 04h
    out  dx, al
    pop  dx
    ret
Func0x605f ENDP

Func0x6087 PROC NEAR                    ;Offset 0x6087
    push ds
    push es
    push si
    push cx
    push bx
    push dx
    xor  ax, ax
    mov  es, ax
    call GetMemoryIn64KBlocks           ;Offset 0x665c
    cmp  ax, 0080h
    jbe  Label0x609c                    ;Offset 0x609c
    mov  ax, 0080h
Label0x609c:                            ;Offset 0x609c
    mov  byte ptr es:[BDA_VideoBufferSize + 1], al;Offset 0x44d
    mov  byte ptr es:[BDA_VideoBufferSize], 00h;Offset 0x44c
    mov  al, byte ptr es:[BDA_EGAFeatureBitSwitches];Offset 0x488
    shr  al, 04h
    and  al, 03h
    je   Label0x61a7                    ;Offset 0x61a7
    push ax
    mov  bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_MemoryPage     ;0x4 Select memory page 0
    out  dx, ax
    pop  ax
    sub  al, 02h
    js   Label0x6133                    ;Offset 0x6133
    je   Label0x60f8                    ;Offset 0x60f8
    mov  byte ptr es:[BDA_VideoBufferSize], 03h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    mov  ch, MGA_MULCTRL_Depth32Pal     ;0x7
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_ScaleDiv4 OR MGA_CRTCEXT3_MGAModeEnable) SHL 8);0x8303
    out  dx, ax
    call Func0x600c                     ;Offset 0x600c
    mov  cx, 0002h
    jmp  Label0x615f                    ;Offset 0x615f
Label0x60f8:                            ;Offset 0x60f8
    mov  byte ptr es:[BDA_VideoBufferSize], 02h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    mov  ch, MGA_MULCTRL_Depth16        ;0x2
    test byte ptr es:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnector1_0;Offset 0x488 0x40
    jne  Label0x6121                    ;Offset 0x6121
    mov  ch, MGA_MULCTRL_Depth15        ;0x1
Label0x6121:                            ;Offset 0x6121
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv2) SHL 8);0x8103
    out  dx, ax
    call Func0x600c                     ;Offset 0x600c
    mov  cx, 0000h
    jmp  Label0x615f                    ;Offset 0x615f
Label0x6133:                            ;Offset 0x6133
    mov  byte ptr es:[BDA_VideoBufferSize], 01h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT MGA_MISCCTRL_DAC_8Bit  ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    nop
    nop
    xor  ch, ch
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_Misc OR ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv1) SHL 8);8003h
    out  dx, ax
    mov  cx, 0001h
Label0x615f:                            ;Offset 0x615f
    push cx
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x6a80
    and  cl, NOT (MGA_MISCCTRL_DAC_Enable OR MGA_MISCCTRL_MAFC_MASK);0xf8
    test byte ptr cs:[Data0x7ff1], 20h  ;Offset 0x7ff1
    je   Label0x6175                    ;Offset 0x6175
    or   cl, MGA_MISCCTRL_DAC_Enable    ;0x1
Label0x6175:                            ;Offset 0x6175
    or   cl, MGA_MISCCTRL_MAFC_PanelLinkMode;0x4
    test byte ptr cs:[Data0x7ff1], 80h  ;Offset 0x7ff1
    jne  Label0x6183                    ;Offset 0x6183
    or   cl, MGA_MISCCTRL_DAC_Enable OR MGA_MISCCTRL_MAFC_Disable;0x7
Label0x6183:                            ;Offset 0x6183
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x6a6b
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, byte ptr cs:[Data0x7d17]   ;Offset 0x7d17
    shr  ah, 03h
    and  ah, MGA_CRTCEXT6_HiPrioLevelMASK;0x7
    mov  al, MGA_CRTCExt_PrioRequestControl;0x6
    out  dx, ax
    pop  cx
    or   cl, cl
    je   Label0x61a7                    ;Offset 0x61a7
    dec  cl
    call Func0x605f                     ;Offset 0x605f
Label0x61a7:                            ;Offset 0x61a7
    pop  dx
    pop  bx
    pop  cx
    pop  si
    pop  es
    pop  ds
    ret
Func0x6087 ENDP

Func0x61ae PROC NEAR                    ;Offset 0x61ae
    push      bx
    push      cx
    push      dx
    call      CheckMemoryMappedRegsAndExtendedRegister;Offset 0x6614
    je        Return                    ;Offset 0x624f
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, NOT (MGA_MISCCTRL_MAFC_MAFC OR MGA_MISCCTRL_DAC_8Bit);0xf5
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    jne       Label0x61d2               ;Offset 0x61d2
    and       cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_8Bit);0xf1h
Label0x61d2:                            ;Offset 0x61d2
    mov       ch, cl
    mov       cl, MGA_INDD_MiscellaneousControl;0x1e
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, NOT MGA_GENCTRL_PedestalContr7p5IRE;0xef
    mov       al, byte ptr cs:[Data0x7d15];Offset 0x7d15
    and       al, 01h
    shl       al, 04h                   ;Pedestal control?
    or        cl, al
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, NOT MGA_GENCTRL_GreenSyncOutDisable;0xdf
    mov       al, byte ptr cs:[Data0x7d15];Offset 0x7d15
    and       al, 20h                   ;green sync out config
    or        cl, al
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_MultiplexControl;0x19
    nop
    nop
    xor       ch, ch
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, MGA_GENCTRL_PedestalContr7p5IRE OR MGA_GENCTRL_GreenSyncOutDisable;x30
    mov       ch, MGA_INDD_GeneralControl;0x1d
    nop
    nop
    xchg      cl, ch
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_CursorControl;0x6
    nop
    nop
    mov       ch, MGA_CURCTRL_CursorDisabled;0x0
    call      WriteIndexedRegister      ;Offset 0x6a6b
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x6a80
    and       cl, NOT MGA_PIXCLKCTRL_SelMASK;0fch
    or        cl, MGA_PIXCLKCTRL_SelPLL ;0x1
    mov       ch, cl
    mov       cl, MGA_INDD_PixelClockControl;0x1a
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x6a6b
Return:                                 ;Offset 0x624f
    mov       ax, (MGA_CRTCExt_AddrGeneratorExt SHL 8) OR 00h;0x0
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    mov       ax, (MGA_CRTCExt_VertCounterExt SHL 8) OR 00h;0x200
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       ah, byte ptr cs:[Data0x7d15];Offset 0x7d15
    and       ah, 10h                   ;bit 4
    shl       ah, 02h                   ;moves to bit 6, MGA_CRTCEXT3_CompSyncEnable
    mov       al, MGA_CRTCExt_Misc      ;0x3
    out       dx, ax
    mov       ax, (MGA_CRTCExt_MemoryPage SHL 8) OR 00h;0x400
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    mov       ax, (MGA_CRTCExt_HorVidHalfCount SHL 8) OR 00h;0x500
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       ah, byte ptr cs:[Data0x7d17];Offset 0x7d17
    and       ah, MGA_CRTCEXT6_HiPrioLevelMASK;0x7 bits 0,1,2
    mov       al, MGA_CRTCExt_PrioRequestControl;0x6
    out       dx, ax
    pop       dx
    pop       cx
    pop       bx
    ret
Func0x61ae ENDP

Func0x628a PROC NEAR                    ;Offset 0x628a
    push      es
    push      di
    push      dx
    push      ecx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    nop
    call      GetMemoryIn64KBlocks      ;Offset 0x665c
    xchg      ah, al
    test      al, 02h
    je        Label0x62a1               ;Offet 0x62a1 Speed sensitive!
Label0x62a1:                            ;Offset 0x62a1
    mov       al, MGA_CRTCExt_MemoryPage;0x4
Label0x62a3:                            ;Offset 0x62a3
    dec       ah
    jb        Label0x62bd               ;Offset 0x62bd
    out       dx, ax
    push      ax
    xor       eax, eax
    mov       di, 0000h
    mov       ecx, 00004000h
    rep stosd
    pop       ax
    or        ah, ah
    jne       Label0x62a3               ;Offset 0x62a3
Label0x62bd:                            ;Offset 0x62bd
    pop       ecx
    pop       dx
    pop       di
    pop       es
    ret
Func0x628a ENDP

Func0x62c3 PROC NEAR                    ;Offset 0x62c3
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
    call      ConfigureAndSelectPLLSetC ;Offset 0x6f68
    xor       edx, edx
    xor       eax, eax
    ;mov       ax, word ptr [si + 0ah]
    DB 08Bh, 084h, 00Ah, 000h
    mov       bx, ds
    mov       cx, bx
    or        cx, ax
    je        Label0x6334               ;Offset 0x6334
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
Label0x6334:                            ;Offset 0x6334
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
    call      TurnScreenOff             ;Offset 0x2b15
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
    je        Label0x6399               ;Offset 0x6399
    mov       al, 00h
Label0x6399:                            ;Offset 0x6399
    or        al, bl
    mov       ah, MGA_CRTCExt_HorVidHalfCount;0x5
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    mov       al, bl
    pop       bx
    mov       ah, bh
    and       ah, 0c0h
    or        al, ah
    mov       byte ptr es:[BDA_DisplayMode], al;Offset 0x449
    test      byte ptr es:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnector1_1;Offset 0x488 0x80
    je        Label0x63c2               ;Offset 0x63c2
    push      si
    push      ds
    mov       al, 02h
    mov       bx, 0000h
    call      TextFunctions             ;Offset 0x1fbf
    pop       ds
    pop       si
Label0x63c2:                            ;Offset 0x63c2
    call      Func0x6087                ;Offset 0x6087
    test      bh, 80h
    jne       Label0x63d7               ;Offset 0x63d7
    mov       al, byte ptr es:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_FeatureConnector0_0 OR BDA_EFBS_FeatureConnector0_1 OR BDA_EFBS_FeatureConnector1_0;0x70
    cmp       al, BDA_EFBS_FeatureConnector1_0;0x40
    je        Label0x63d7               ;Offset 0x63d7
    call      Func0x628a                ;Offset 0x628a
Label0x63d7:                            ;Offset 0x63d7
    xor       eax, eax
    call      SetStartAddress           ;Offset 0x6466
    call      Func0x686a                ;Offset 0x686a
    mov       ah, MGA_CRTCExt_MemoryPage;0x4
    mov       al, bl
    call      MGAWriteCRTCExtensionRegister;Offset 0x2d6
    call      TurnScreenOn              ;Offset 0x2b11
    add       sp, 001ch
    xor       ax, ax
    pop       edx
    pop       ebx
    pop       es
    pop       bp
    ret
Func0x62c3 ENDP

Func0x63f6 PROC NEAR                    ;Offset 0x63f6
    push      ds
    xor       ax, ax
    mov       ds, ax
    mov       ax, 0001h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x640b               ;Offset 0x640b
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    shr       al, 04h
Label0x640b:                            ;Offset 0x640b
    ;and       ax, 0003h
    DB 025h, 003h, 000h
    pop       ds
    ret
Func0x63f6 ENDP

Func0x6410 PROC NEAR                    ;Offset 0x6410
    push      ds
    push      0000h
    pop       ds
    or        al, al
    mov       ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    je        Label0x6425               ;Offset 0x6425
    shr       al, 04h
    and       cl, 0f0h
    or        cl, al
    jmp       Label0x6431               ;Offset 0x6431
Label0x6425:                            ;Offset 0x6425
    push      cx
    shl       cl, 04h
    and       al, 0fh
    or        al, cl
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    pop       cx
Label0x6431:                            ;Offset 0x6431
    pop       ds
    ret
Func0x6410 ENDP

GetStartAddress PROC NEAR               ;Offset 0x6433
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
    call      Func0x68df                ;Offset 0x68df
    pop       dx
    ret
GetStartAddress ENDP

SetStartAddress PROC NEAR               ;Offset 0x6466
    push      cx
    push      dx
    mov       dl, 00h
    call      Func0x68df                ;Offset 0x68df
    push      eax
    test      bl, 80h
    ;je        Label0x6483               ;Offset 0x6483
    DB 00Fh, 084h, 00Dh, 000h
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
    xor       cx, cx
Label0x647b:                            ;Offset 0x647b
    dec       cx
    je        Label0x6483               ;Offset 0x6483
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x647b               ;Offset 0x647b
Label0x6483:                            ;Offset 0x6483
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
SetStartAddress ENDP

Func0x64b0 PROC NEAR                    ;Offset 0x64b0
    push      ebx
    push      edx
    xor       ebx, ebx
    call      Func0x6523                ;Offset 0x6523
    shr       bx, 03h
    call      GetStartAddress           ;Offset 0x6433
    xor       edx, edx
    div       ebx
    mov       cx, dx
    push      ax
    call      Func0x63f6                ;Offset 0x63f6
    je        Label0x64d8               ;Offset 0x64d8
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shl       ax, cl
    xchg      ax, cx
Label0x64d8:                            ;Offset 0x64d8
    pop       ax
    pop       edx
    pop       ebx
    mov       dx, ax
    mov       al, 01h
    call      Func0x6410                ;Offset 0x6410
    ret
Func0x64b0 ENDP

Func0x64e5 PROC NEAR                    ;Offset 0x64e5
    push      ebx
    push      ecx
    push      edx
    xor       al, al
    call      Func0x6410                ;Offset 0x6410
    push      bx
    push      cx
    push      dx
    call      Func0x6523                ;Offset 0x6523
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
    call      Func0x63f6                ;Offset 0x63f6
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shr       ax, cl
    xchg      ax, cx
    pop       ax
    add       eax, ecx
    call      SetStartAddress           ;Offset 0x6466
    pop       edx
    pop       ecx
    pop       ebx
    ret
Func0x64e5 ENDP

Func0x6523 PROC NEAR                    ;Offset 0x6523
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
    call      Func0x63f6                ;Offset 0x63f6
    mov       cl, al
    shl       bx, 04h
    dec       cl
    mov       ax, bx
    shr       ax, cl
    mov       cx, ax
    call      GetMemoryIn64KBlocks      ;Offset 0x665c
    cmp       ax, 0100h
    jbe       Label0x655f               ;Offset 0x655f
    mov       ax, 0100h
Label0x655f:                            ;Offset 0x655f
    mov       dx, ax
    xor       ax, ax
    div       bx
    mov       dx, ax
    pop       ds
    ret
Func0x6523 ENDP

Func0x6569 PROC NEAR                    ;Offset 0x6569
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
Func0x6569 ENDP

Func0x659e PROC NEAR                    ;Offset 0x659e
    mov       bx, ax
    call      Func0x63f6                ;Offset 0x63f6
    je        Label0x65bd               ;Offset 0x65bd
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
    je        Label0x65bd               ;Offset 0x65bd
    sub       bx, dx
    add       ax, bx
Label0x65bd:                            ;Offset 0x65bd
    ret
Func0x659e ENDP

Func0x65be PROC NEAR                    ;Offset 0x65be
    push      dx
    push      cx
    test      byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    jne       Label0x65d2               ;Offset 0x65d2
    test      byte ptr cs:[Data0x7ff1], 80h;Offset 0x7ff1
    ;jne       Label0x65d9               ;Offset 0x65d9
    DB 00Fh, 085h, 007h, 000h
Label0x65d2:                            ;Offset 0x65d2
    call      Func0x63f6                ;Offset 0x63f6
    mov       cl, al
    jne       Label0x65df               ;Offset 0x65df
Label0x65d9:                            ;Offset 0x65d9
    ;mov       ax, word ptr [si + 04h]
    DB 08Bh, 084h, 004h, 000h
    jmp       Label0x6611               ;Offset 0x6611
Label0x65df:                            ;Offset 0x65df
    dec       cl
    push      cx
    call      GetMemoryIn64KBlocks      ;Offset 0x665c
    cmp       ax, 0100h
    jbe       Label0x65ed               ;Offset 0x65ed
    mov       ax, 0100h
Label0x65ed:                            ;Offset 0x65ed
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
    jbe       Label0x6609               ;Offset 0x6609
    mov       ax, 03f0h
Label0x6609:                            ;Offset 0x6609
    shl       ax, 04h
    shr       ax, cl
    call      Func0x659e                ;Offset 0x659e
Label0x6611:                            ;Offset 0x6611
    pop       cx
    pop       dx
    ret
Func0x65be ENDP

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
CheckMemoryMappedRegsAndExtendedRegister PROC NEAR;Offset 0x6614
    push      bx
    push      cx
    push      dx
    push      si
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       si, MGA_MemAddr_PaletteRamWriteAddress;Offset 0x3c00
    nop
    call      IndirectRegisterReadByte  ;Offset 0x6a1c  Read current index
    not       cl                        ;invert index
    call      IndirectRegisterWriteByte ;Offset 0x69c7  write back inverted index
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    in        al, dx                    ;read back index
    mov       ah, cl                    ;store written index in ah. 
    push      ax                        ;preserve ax
    not       cl                        ;restore index to original value
    call      IndirectRegisterWriteByte ;Offset 0x69c7
    mov       cl, 01h
    pop       ax                        ;restore ax. al = port read inverted index, ah = indirect read inverted index
    ;cmp       ah, al                    ;compare the two values.
    DB 03Ah, 0E0h
    jne       Failure                   ;Offset 0x6654
    mov       cl, 00h                   ;Set to zero
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       al, MGA_CRTCExt_RequesterControl;0x7
    out       dx, al
    in        ax, dx                    ;Store off requester control
    push      ax
    mov       ax, (0a0h SHL 8) OR MGA_CRTCExt_RequesterControl;0xa007 - illegal value for register?
    out       dx, ax
    in        ax, dx
    cmp       ah, 0a0h
    jne       Success                   ;Offset 0x6652
    mov       cl, 01h
Success:                                ;Offset 0x6652
    pop       ax
    out       dx, ax
Failure:                                ;Offset 0x6654
    cmp       cl, 01h
    pop       si
    pop       dx
    pop       cx
    pop       bx
    ret
CheckMemoryMappedRegsAndExtendedRegister ENDP

GetMemoryIn64KBlocks PROC NEAR          ;Offset 0x665c
    push      dx
    mov       ax, 0bd50h
    xor       dx, dx
    int       6dh
    or        ah, ah
    mov       ax, dx
    pop       dx
    ;je        Label0x6673               ;Offset 0x6673
    DB 00Fh, 084h, 006h, 000h
    movzx     ax, byte ptr cs:[MemorySizeInMB];Offset 0x7ff0
Label0x6673:                            ;Offset 0x6673
    shl       ax, 04h
    ret
GetMemoryIn64KBlocks ENDP

Func0x6677 PROC NEAR                    ;Offset 0x6677
    mov   ax, 0af03h
    int   6dh
    cmp   ax, 0af03h
    je    Label0x6682                   ;Offset 0x6682
    ret
Label0x6682:                            ;Offset 0x6682
    push  di
    xor   ax, ax
    mov   ds, ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    and   al, 3fh
    cmp   al, 20h
    jl    Label0x66ab                   ;Offset 0x66ab
    cmp   al, 40h
    jge   Label0x66ab                   ;Offset 0x66ab
    sub   al, 20h
    lea   si, [Data0x53fa]              ;Offset 0x53fa
    push  ax
    ;and   ax, 001fh
    DB 025h, 01Fh, 000h
    shl   ax, 02h
    add   si, ax
    mov   si, word ptr cs:[si]
    pop   ax
    inc   ah
    jmp   Label0x66ad                   ;Offset 0x66ad
Label0x66ab:                            ;Offset 0x66ab
    xor   si, si
Label0x66ad:                            ;Offset 0x66ad
    push  cs
    pop   ds
    pop   di
    ret   
Func0x6677 ENDP

Func0x66b1 PROC NEAR                    ;Offset 0x66b1
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
    jne   Label0x673b                   ;Offset 0x673b
    mov   ch, 00h
    cmp   ax, 03ffh
    jb    Label0x673b                   ;Offset 0x673b
    mov   ch, 0ffh
    shr   ax, 01h
    or    byte ptr es:[di + 21h], 04h
Label0x673b:                            ;Offset 0x673b
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
    je    Label0x676f                   ;Offset 0x676f
    shr   ax, 01h
Label0x676f:                            ;Offset 0x676f
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
    je    Label0x679b                   ;Offset 0x679b
    shr   ax, 01h
Label0x679b:                            ;Offset 0x679b
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
    je    Label0x67ca                   ;Offset 0x67ca
    shr   ax, 01h
Label0x67ca:                            ;Offset 0x67ca
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
    je    Label0x67fa                   ;Offset 0x67fa
    shr   ax, 01h
Label0x67fa:                            ;Offset 0x67fa
    dec   ax
    ;and   ax, 0fh
    DB 025h, 00Fh, 000h
    mov   bh, byte ptr es:[di + 1bh]
    and   bh, 0f0h
    or    al, bh
    mov   byte ptr es:[di + 1bh], al
    mov   ax, word ptr ss:[si + 14h]
    test  ch, 0ffh
    je    Label0x6816                   ;Offset 0x6816
    shr   ax, 01h
Label0x6816:                            ;Offset 0x6816
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
Func0x66b1 ENDP

;Thankfully, this function is not used.
WriteUndocumentedRegister PROC NEAR     ;Offset 0x6837
    push  cx
    mov   bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov   al, byte ptr cs:[Data0x7ff1]  ;Offset 0x7ff1
    xor   cx, cx
    test  al, 80h
    nop
    nop
    je    Label0x684d                   ;Offset 0x684d
    or    ch, 20h
    nop
Label0x684d:                            ;Offset 0x684d
    test  al, 04h
    nop
    nop
    je    Label0x6857                   ;Offset 0x6857
    or    ch, 04h
    nop
Label0x6857:                            ;Offset 0x6857
    test  al, 02h
    nop
    nop
    je    Label0x6861                   ;Offset 0x6861
    or    ch, 01h
    nop
Label0x6861:                            ;Offset 0x6861
    mov   cl, 8ah                       ;undocumented register?
    nop
    nop
    call  WriteIndexedRegister          ;Offset 0x6a6b
    pop   cx
    ret
WriteUndocumentedRegister ENDP

Func0x686a PROC NEAR                    ;Offset 0x686a
    push  ax
    push  dx
    push  si
    xor   ebx, ebx
    test  byte ptr cs:[Data0x7ff1], 80h ;Offset 0x7ff1
    je    Label0x68db                   ;Offset 0x68db
    test  byte ptr cs:[Data0x7ff1], 20h ;Offset 0x7ff1
    jne   Label0x68db                   ;Offset 0x68db
    push  ds
    push  0000h
    pop   ds
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop   ds
    and   al, 3fh
    cmp   al, 20h
    jb    Label0x68db                   ;Offset 0x68db
    test  byte ptr cs:[Data0x7ff1], 40h ;Offset 0x7ff1
    nop
    jne   Label0x68b1                   ;Offset 0x68b1
    cmp   al, 23h
    ja    Label0x68a2                   ;Offset 0x68a2
    lea   si, [Data0x7104]              ;Offset 0x7104
    jmp   Label0x68c8                   ;Offset 0x68c8
    nop
Label0x68a2:                            ;Offset 0x68a2
    cmp   al, 30h
    jb    Label0x68db                   ;Offset 0x68db
    cmp   al, 35h
    ja    Label0x68db                   ;Offset 0x68db
    lea   si, [Data0x7110]              ;Offset 0x7110
    jmp   Label0x68c8                   ;Offset 0x68c8
    nop
Label0x68b1:                            ;Offset 0x68b1
    cmp   al, 25h
    ja    Label0x68bc                   ;Offset 0x68bc
    lea   si, [Data0x7122]              ;Offset 0x7122
    jmp   Label0x68c8                   ;Offset 0x68c8
    nop
Label0x68bc:                            ;Offset 0x68bc
    cmp   al, 30h
    jb    Label0x68db                   ;Offset 0x68db
    cmp   al, 38h
    ja    Label0x68db                   ;Offset 0x68db
    lea   si, [Data0x7134]              ;Offset 0x7134
Label0x68c8:                            ;Offset 0x68c8
    and   al, 0fh
    mov   bl, 03h
    mul   bl
    add   si, ax
    mov   bx, word ptr cs:[si]
    shl   ebx, 08h
    mov   bl, byte ptr cs:[si + 02h]
Label0x68db:                            ;Offset 0x68db
    pop   si
    pop   dx
    pop   ax
    ret
Func0x686a ENDP

Func0x68df PROC NEAR                    ;Offset 0x68df
    push  ebx
    call  Func0x686a                    ;Offset 0x686a
    shr   ebx, 08h
    cmp   dl, 00h
    jne   Label0x68f2                   ;Offset 0x68f2
    add   eax, ebx
    jmp   Label0x68f5                   ;Offset 0x68f5
Label0x68f2:                            ;Offset 0x68f2
    sub   eax, ebx
Label0x68f5:                            ;Offset 0x68f5
    pop   ebx
    ret
Func0x68df ENDP

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
AccessPCIRegister PROC NEAR             ;Offset 0x68f8
    push  eax
    push  bx
    push  dx
    push  di
    pushf
    cli
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    cmp   bh, 0c0h
    jb    Mechanism1                    ;Offset 0x691b
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
    jmp   PerformAccess                 ;Offset 0x6937
Mechanism1:                             ;Offset 0x691b
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
PerformAccess:                          ;Offset 0x6937
    cmp   al, PCI_ACCESS_ReadByte       ;0x8
    je    ReadByte                      ;Offset 0x696e
    cmp   al, PCI_ACCESS_ReadWord       ;0x9
    je    ReadWord                      ;Offset 0x6969
    cmp   al, PCI_ACCESS_ReadDWord      ;0xa
    je    ReadDWord                     ;Offset 0x6962
    cmp   al, PCI_ACCESS_WriteByte      ;0xb
    je    WriteByte                     ;Offset 0x695d
    cmp   al, PCI_ACCESS_WriteWord      ;0xc
    je    WriteWord                     ;Offset 0x6958
    cmp   al, PCI_ACCESS_WriteDWord     ;0xd
    je    WriteDWord                    ;Offset 0x6951
    jmp   Done                          ;Offset 0x6971
WriteDWord:                             ;Offset 0x6951
    mov   eax, ecx
    out   dx, eax
    jmp   Done                          ;Offset 0x6971
WriteWord:                              ;Offset 0x6958
    mov   ax, cx
    out   dx, ax
    jmp   Done                          ;Offset 0x6971
WriteByte:                              ;Offset 0x695d
    mov   al, cl
    out   dx, al
    jmp   Done                          ;Offset 0x6971
ReadDWord:                              ;Offset 0x6962
    in    eax, dx
    mov   ecx, eax
    jmp   Done                          ;Offset 0x6971
ReadWord:                               ;Offset 0x6969
    in    ax, dx
    mov   cx, ax
    jmp   Done                          ;Offset 0x6971
ReadByte:                               ;Offset 0x696e
    in    al, dx
    mov   cl, al
Done:                                   ;Offset 0x6971
    cmp   dh, 0c0h
    jb    IsMechanism1                  ;Offset 0x697e
    xor   al, al
    mov   dx, PCI_M2_ConfigSpaceEnable  ;Port 0xcf8
    out   dx, al
    jmp   Return                        ;Offset 0x6986
IsMechanism1:                           ;Offset 0x697e
    xor   eax, eax
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    out   dx, eax
Return:                                 ;Offset 0x6986
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
ReadPCIBase0AndBase1 PROC NEAR          ;Offset 0x698d
    push  di
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 + 2;0x12
    call  AccessPCIRegister             ;Offset 0x68f8
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 ;0x10
    call  AccessPCIRegister             ;Offset 0x68f8
    and   ecx, 0fffffff0h
    push  ecx
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 + 2;0x16
    call  AccessPCIRegister             ;Offset 0x68f8
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 ;0x14
    call  AccessPCIRegister             ;Offset 0x68f8
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
IndirectRegisterWriteByte PROC NEAR     ;Offset 0x69c7
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xch
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    cx
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x68f8
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
IndirectRegisterWriteWord PROC NEAR     ;Offset 0x69e5
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x68f8
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
IndirectRegisterWriteDWord PROC NEAR    ;Offset 0x6a03
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    cx
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x68f8
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
IndirectRegisterReadByte PROC NEAR      ;Offset 0x6a1c
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x68f8
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
IndirectRegisterReadWord PROC NEAR      ;Offset 0x6a38
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadWord      ;0x9
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x68f8
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
IndirectRegisterReadDWord PROC NEAR     ;Offset 0x6a54
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    di
    pop    si
    ret   
IndirectRegisterReadDWord ENDP

;
;inputs:
;   cl = RAMDAC write index
;   ch = Value
;
;destroys:
;   ax
;
WriteIndexedRegister PROC NEAR          ;Offset 0x6a6b
    push   si
    push   dx
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
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
ReadIndexedRegister PROC NEAR           ;Offset 0x6a80
    push   si
    push   dx
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterReadByte     ;Offset 0x6a1c
    pop    dx
    pop    si
    ret
ReadIndexedRegister ENDP

;Offset 0x6a93
Data0x6a93              DB 002h, 004h, 008h, 010h

;inputs:
;   cx = 
;
;outputs:
;
;destroys:
;   dx
FetchDefaultSystemClockSpeed PROC NEAR  ;Offset 0x6a97
    push  ax
    push  bx
    xor   dx, dx
    mov   dl, byte ptr cs:[Data0x7d0b]  ;Offset 0x7d0b
    cmp   dl, 0ffh
    jne   Label0x6b02                   ;Offset 0x6b02
    call  GetMemoryIn64KBlocks          ;Offset 0x665c
    mov   dl, byte ptr cs:[Data0x7d17]  ;Offset 0x7d17
    cmp   dl, 0ffh
    jne   Label0x6ace                   ;Offset 0x6ace
    push  di
    push  cx
    push  ax
    mov   bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov   di, PCI_MGA_Option + 01h      ;0x41
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x68f8
    test  cl, (PCI_MGA_Opt_MemCfg16Mb2x512Kx16 SHR 8);10h
    pop   ax
    pop   cx
    pop   di
    mov   dl, 00h
    je    Label0x6ace                   ;Offset 0x6ace
    mov   dl, 40h
Label0x6ace:                            ;Offset 0x6ace
    and   dl, 0c0h
    rol   dl, 02h
    xchg  ax, dx
    push  ds
    push  cs
    pop   ds
    lea   bx, [Data0x6a93]              ;Offset 0x6a93
    xlatb 
    pop   ds
    shl   ax, 04h
    sub   dx, ax
    mov   dh, 00h
    jle   Label0x6af6                   ;Offset 0x6af6
    mov   bx, dx
    shr   bl, 06h
    mov   dl, byte ptr cs:[bx + Data0x7d0d];Offset 0x7d0d
    cmp   dl, 0ffh
    jne   Label0x6b02                   ;Offset 0x6b02
Label0x6af6:                            ;Offset 0x6af6
    mov   dl, byte ptr cs:[Data0x7d0c]  ;Offset 0x7d0c
    cmp   dl, 0ffh
    jne   Label0x6b02                   ;Offset 0x6b02
    mov   dl, 3fh
Label0x6b02:                            ;Offset 0x6b02
    mov   bh, byte ptr cs:[Data0x7d1a]  ;Offset 0x7d1a
    and   bh, 0ch
    shl   bh, 03h
    mov   cx, 0001h
    or    cl, bh
    mov   bl, byte ptr cs:[Data0x7d14]  ;Offset 0x7d14
    cmp   bl, 0ffh
    jne   Label0x6b1e                   ;Offset 0x6b1e
    mov   bl, 00h
Label0x6b1e:                            ;Offset 0x6b1e
    test  byte ptr cs:[Data0x7d1a], 04h ;Offset 0x7d1a
    jne   Label0x6b3c                   ;Offset 0x6b3c
    test  bl, 01h
    jne   Label0x6b30                   ;Offset 0x6b30
    shl   dx, 01h
    jmp   Label0x6b3c                   ;Offset 0x6b3c
    nop
Label0x6b30:                            ;Offset 0x6b30
    mov   ax, 03h
    mul   dx
    shr   ax, 01h
    mov   dx, ax
    or    cx, 0004h
Label0x6b3c:                            ;Offset 0x6b3c
    and   bl, 02h
    shl   bl, 02h
    or    cl, bl
    pop   bx
    pop   ax
    ret
FetchDefaultSystemClockSpeed ENDP

;
;Calculate the M, N, P and S values for the PLL's and nearest clock rate in KHz
;given an input frequency in KHz
;
;inputs:
;   eax = requested frequency in KHz
;
;outputs:
;   eax = nearest clock rate in KHz
;   ebx = M,N,P and S values
;         bl holds N in lower 7 bits
;         bh holds M in lower 5 bits
;         bh holds P in upper 3 bits
;         S 2 bits sit above bh
;         00000000 000000SS PPPMMMMM xNNNNNNN
;
CalculatePLL_MNPS_AndNearestClockRate PROC NEAR;Offset 0x6b47
    push   bp
    mov    bp, sp
    sub    sp, 001ch
    push   ecx
    push   edx
    mov    dword ptr [bp - 18h], 0ffffffffh
    mov    dword ptr [bp - 04h], eax
    xor    eax, eax
    mov    al, byte ptr cs:[Data0x7d04] ;Offset 0x7d04
    cmp    al, 0ffh
    ;je     Label0x6b7c                  ;Offset 0x6b7c
    DB 00Fh, 084h, 012h, 000h
    ;add    ax, 64h
    DB 005h, 064h, 000h
    mov    ebx, 000003e8h               ;1,000
    mul    ebx
    mov    dword ptr [bp - 08h], eax
    jmp    Label0x6b84                  ;Offset 0x6b84
Label0x6b7c:                            ;Offset 0x6b7c
    mov    dword ptr [bp - 08h], 00035b60h;220,000
Label0x6b84:                            ;Offset 0x6b84
    mov    eax, dword ptr [bp - 04h]
    cmp    eax, 00001876h               ;6,262
    jae    Label0x6b96                  ;Offset 0x6b96
    mov    eax, 00001876h               ;6,262
Label0x6b96:                            ;Offset 0x6b96
    cmp    eax, dword ptr [bp - 08h]
    jbe    Label0x6ba0                  ;Offset 0x6ba0
    mov    eax, dword ptr [bp - 08h]
Label0x6ba0:                            ;Offset 0x6ba0
    mov    dword ptr [bp - 04h], eax
    xor    edx, edx
    mov    ebx, 00000005h
    mul    ebx
    mov    ebx, 000003e8h               ;1,000
    div    ebx
    mov    dword ptr [bp - 1ch], eax
    mov    ecx, 00000003h
Label0x6bc3:                            ;Offset 0x6bc3
    mov    eax, dword ptr [bp - 04h]
    shl    eax, cl
    shl    ecx, 10h
    cmp    eax, 0000c350h               ;50,000
    jb     Label0x6c7d                  ;Offset 0x6c7d
    cmp    eax, dword ptr [bp - 08h]
    ja     Label0x6c73                  ;Offset 0x6c73
    mov    ch, 01h
    mov    cl, 07h
Label0x6be4:                            ;Offset 0x6be4
    xor    edx, edx
    xor    ebx, ebx
    mov    eax, 00006978h               ;27,000
    test   byte ptr cs:[Data0x7d14], 20h;Offset 0x7d14
    ;je     Label0x6c00                  ;Offset 0x6c00
    DB 00Fh, 084h, 006h, 000h
    mov    eax, 000037eeh               ;14,318
Label0x6c00:                            ;Offset 0x6c00
    mov    bl, cl
    inc    bl
    mul    ebx
    mov    bl, ch
    inc    bl
    div    ebx
    cmp    eax, dword ptr [bp - 08h]
    ;ja     Label0x6c68                  ;Offset 0x6c68
    DB 00Fh, 087h, 052h, 000h
    ;cmp    eax, 0000c350h                ;50,000
    DB 066h, 03Dh, 050h, 0C3h, 000h, 000h
    ;jb    Label0x6c5f                   ;Offset 0x6c5f
    DB 00Fh, 082h, 03Fh, 000h
    mov    dword ptr [bp - 0ch], eax
    ror    ecx, 10h
    shr    eax, cl
    rol    ecx, 10h
    mov    ebx, dword ptr [bp - 04h]
    cmp    ebx, eax
    ;jb     Label0x6c3c                  ;Offset 0x6c3c
    DB 00Fh, 082h, 002h, 000h
    xchg   eax, ebx
Label0x6c3c:                            ;Offset 0x6c3c
    sub    eax, ebx
    mov    edx, dword ptr [bp - 0ch]
    cmp    eax, dword ptr [bp - 1ch]
    ;jbe    Label0x6c85                  ;Offset 0x6c85
    DB 00Fh, 086h, 03Ah, 000h
    cmp    eax, dword ptr [bp - 18h]
    ;jae    Label0x6c5f                  ;Offset 0x6c5f
    DB 00Fh, 083h, 00Ch, 000h
    mov    dword ptr [bp - 18h], eax
    mov    dword ptr [bp - 14h], ecx
    mov    dword ptr [bp - 10h], edx
Label0x6c5f:                            ;Offset 0x6c5f
    inc    cl
    cmp    cl, 7fh
    ;jbe    Label0x6be4                  ;Offset 0x6be4
    DB 00Fh, 086h, 07Ch, 0FFh
Label0x6c68:                            ;Offset 0x6c68
    mov    cl, 07h
    inc    ch
    cmp    ch, 1fh
    jbe    Label0x6be4                  ;Offset 0x6be4
Label0x6c73:                            ;Offset 0x6c73
    shr    ecx, 10h
    dec    cl
    jns    Label0x6bc3                  ;Offset 0x6bc3
Label0x6c7d:                            ;Offset 0x6c7d
    mov    ecx, dword ptr [bp - 14h]
    mov    edx, dword ptr [bp - 10h]
Label0x6c85:                            ;Offset 0x6c85
    mov    ebx, ecx
    and    ebx, 00001fffh
    shr    ecx, 10h
    mov    ax, 01h
    shl    ax, cl
    dec    ax
    shl    ax, 0dh
    or     bx, ax
    xor    eax, eax
    cmp    edx, 000186a0h               ;100,000
    ja     Label0x6caf                  ;Offset 0x6caf
    mov    ax, 0000h                    ;PixPLL S
    jmp    Label0x6cce                  ;Offset 0x6cce
Label0x6caf:                            ;Offset 0x6caf
    cmp    edx, 000222e0h               ;140,000
    ja     Label0x6cbd                  ;Offset 0x6cbd
    mov    ax, 0001h                    ;PixPLL S
    jmp    Label0x6cce                  ;Offset 0x6cce
Label0x6cbd:                            ;Offset 0x6cbd
    cmp    edx, 0002bf20h               ;180,000
    ja     Label0x6ccb                  ;Offset 0x6ccb
    mov    ax, 0002h                    ;PixPLL S
    jmp    Label0x6cce                  ;Offset 0x6cce
Label0x6ccb:                            ;Offset 0x6ccb
    mov    ax, 0003h                    ;PixPLL S
Label0x6cce:                            ;Offset 0x6cce
    shl    eax, 10h
    or     ebx, eax                     ;Merge S value
    mov    eax, edx
    shr    eax, cl
    pop    edx
    pop    ecx
    mov    sp, bp
    pop    bp
    ret
CalculatePLL_MNPS_AndNearestClockRate ENDP

;
;inputs:
;  edx = Requested frequency in Hz
;   cx = bits 1-0 = sysclksl
;        bit    2 = gclkdiv
;        bit    3 = mclkdiv
;        bit    5 = nogclkdiv
;        bit    6 = nomclkdiv
ConfigureSystemClock PROC NEAR          ;Offset 0x6ce3
    push   eax
    push   ebx
    push   ecx
    push   edx
    push   esi
    push   cx
    and    cl, 01h                      ;bit 0
    jne    NotPLL                       ;Offset 0x6d32
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, dl
    and    al, PCI_MGA_Opt_SysClockSelect_MASK;bits 1,0
    jne    Label0x6d1f                  ;Offset 0x6d1f
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    jmp    Label0x6de5                  ;Offset 0x6de5
Label0x6d1f:                            ;Offset 0x6d1f
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectMCLK;0x2
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    jmp    Label0x6de5                  ;Offset 0x6de5
NotPLL:                                 ;Offset 0x6d32
    mov    eax, edx
    mov    bx, 0000h                    ;Unused in function, stomped
    call   CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    push   ebx
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    edx
    mov    cl, MGA_INDD_SYSPLL_M_Value  ;0x2c
    mov    ch, dh
    and    ch, MGA_SYSPLLM_Mask         ;0x1f
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x6a6b
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    dx, 2ee0h                    ;12,000
    call   Sleep                        ;Offset 0x508e
    xor    dx, dx
WaitForFrequencyLock:                   ;Offset 0x6d92
    dec    dx
    je     TimeOut                      ;Offset 0x6da1
    mov    cl, MGA_INDD_SYSPLL_Status   ;0x2f
    call   ReadIndexedRegister          ;Offset 0x6a80
    and    cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je     WaitForFrequencyLock         ;Offset 0x6d92
    jmp    FrequencyLocked              ;Offset 0x6dc7
TimeOut:                                ;Offset 0x6da1
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysPLLPowerUp;0xdf
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x508e
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, PCI_MGA_Opt_SysPLLPowerUp;0x20
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x508e
FrequencyLocked:                        ;Offset 0x6dc7
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
Label0x6de5:                            ;Offset 0x6de5
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, dl
    and    al, 0ch                      ;bits 3,2
    shl    al, 01h                      ;bits 4,3
    or     cl, al
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    di, PCI_MGA_Option2 + 01h    ;0x51
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT ((PCI_MGA_Opt2_GfxClockDiv_MASK OR PCI_MGA_Opt2_MemClockDiv_MASK) SHR 8);0x3f
    and    dl, 60h                      ;bits 6,5
    shl    dl, 01h                      ;bits 7,6
    or     cl, dl
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    di, PCI_MGA_Option2 + 01h    ;0x51
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    dl, byte ptr cs:[Data0x7d1a] ;Offset 0x7d1a
    and    dl, PCI_MGA_Opt2_MBufType_MASK SHR 12;0x3
    shl    dl, 04h
    mov    ch, cl
    and    cl, PCI_MGA_Opt2_MBufType_MASK SHR 8;0x30
    ;cmp    cl, dl
    DB 03Ah, 0CAh
    je     Label0x6e36                  ;Offset 0x6e36
    and    ch, 0cfh
    or     ch, dl
    mov    cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
Label0x6e36:                            ;Offset 0x6e36
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
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
ConfigurePixelClocks PROC NEAR          ;Offset 0x6e51
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
    call   ReadIndexedRegister          ;Offset 0x6a80
    or     cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6a6b
    pop    cx
    mov    al, cl
    and    al, 03h
    cmp    al, 00h
    jne    Label0x6e8b                  ;Offset 0x6e8b
    mov    al, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jmp    Label0x6e95                  ;Offset 0x6e95
Label0x6e8b:                            ;Offset 0x6e8b
    cmp    al, 01h
    jne    Label0x6e93                  ;Offset 0x6e93
    mov    al, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jmp    Label0x6e95                  ;Offset 0x6e95
Label0x6e93:                            ;Offset 0x6e93
    mov    al, MGA_INDD_PIXPLL_M_ValueSetC;0x4c
Label0x6e95:                            ;Offset 0x6e95
    mov    ah, cl
    shr    ah, 02h
    cmp    ah, 01h
    jne    Label0x6ea3                  ;Offset 0x6ea3
    mov    ah, MGA_PIXCLKCTRL_SelPLL    ;0x1
    jmp    Label0x6eae                  ;Offset 0x6eae
Label0x6ea3:                            ;Offset 0x6ea3
    cmp    ah, 00h
    jne    Label0x6eac                  ;Offset 0x6eac
    mov    ah, MGA_PIXCLKCTRL_SelPCI    ;0x0
    jmp    Label0x6eae                  ;Offset 0x6eae
Label0x6eac:                            ;Offset 0x6eac
    mov    ah, MGA_PIXCLKCTRL_SelVDOCLK ;0x2
Label0x6eae:                            ;Offset 0x6eae
    pop    edx
    push   ax
    mov    ch, dh
    and    ch, MGA_PIXPLL_M_MASK        ;0x1f
    mov    cl, al
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x6a6b
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x6a6b
    pop    dx
    push   dx
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jne    Label0x6ed8                  ;Offset 0x6ed8
    mov    cl, VGA_MISC_Clock25p175MHz SHR 2;0x0
    jmp    Label0x6ee3                  ;Offset 0x6ee3
Label0x6ed8:                            ;Offset 0x6ed8
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jne    Label0x6ee1                  ;Offset 0x6ee1
    mov    cl, VGA_MISC_Clock28p322MHz SHR 2;0x1
    jmp    Label0x6ee3                  ;Offset 0x6ee3
Label0x6ee1:                            ;Offset 0x6ee1
    mov    cl, MGA_MISC_MGAPixelClock SHR 2;0x3
Label0x6ee3:                            ;Offset 0x6ee3
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    shl    cl, 02h
    and    al, NOT VGA_MISC_ClockSelectMask;0xf3
    or     al, cl
    mov    dx, VGA_MiscellaneousWrite   ;Port 0x3c2
    out    dx, al
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x508e
    xor    dx, dx
Label0x6efa:                            ;Offset 0x6efa
    dec    dx
    je     Label0x6f09                  ;Offset 0x6f09
    mov    cl, MGA_INDD_PIXPLL_Status   ;0x4f
    call   ReadIndexedRegister          ;Offset 0x6a80
    and    cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je     Label0x6efa                  ;Offset 0x6efa
    jmp    Label0x6f33                  ;Offset 0x6f33
Label0x6f09:                            ;Offset 0x6f09
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6a80
    and    cl, NOT MGA_PIXCLKCTRL_PixelPLLPowerUp;0xf7
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x508e
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6a80
    or     cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    dx, 2ee0h
    call   Sleep                        ;Offset 0x508e
Label0x6f33:                            ;Offset 0x6f33
    pop    dx
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6a80
    and    cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or     cl, dh
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6a6b
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x6a80
    and    cl, NOT MGA_PIXCLKCTRL_ClockDisable;0xfb
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x6a6b
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
ConfigureAndSelectPLLSetC PROC NEAR     ;Offset 0x6f68
    push   eax
    push   ebx
    push   ecx
    mov    cl, MGA_CPC_PLLSetC OR MGA_CPC_ClockPLL;0x6
    mov    eax, edx
    mov    bx, 0002h                    ;Unused in function, stomped
    call   CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    call   ConfigurePixelClocks         ;Offset 0x6e51
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigureAndSelectPLLSetC ENDP

ConfigureAndSelectPLLSetsAB PROC NEAR   ;Offset 0x6f83
    push   eax
    push   ebx
    push   ecx
    mov    cl, byte ptr cs:[PLLADefaultFreq];Offset 0x7d09
    cmp    cl, 0ffh
    ;jne    Label0x6f97                  ;Offset 0x6f97
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x6f97:                            ;Offset 0x6f97
    xor    eax, eax
    mov    al, 0fah                     ;250
    mul    cl
    or     ax, ax
    ;jne    Label0x6fa7                  ;Offset 0x6fa7
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6257h                    ;25,175
Label0x6fa7:                            ;Offset 0x6fa7
    mov    bx, 0002h                    ;Unused in function, stomped
    call   CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    mov    cl, MGA_CPC_PLLSetA OR MGA_CPC_ClockPLL;0x4
    call   ConfigurePixelClocks         ;Offset 0x6e51
    mov    cl, byte ptr cs:[PLLBDefaultFreq];Offset 0x7d0a
    cmp    cl, 0ffh
    ;jne    Label0x6fc0                  ;Offset 0x6fc0
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x6fc0:                            ;Offset 0x6fc0
    xor    eax, eax
    mov    al, 0fah                     ;250
    mul    cl
    or     ax, ax
    ;jne    Label0x6fd0                  ;Offset 0x6fd0
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6ea2h                    ;28,322
Label0x6fd0:                            ;Offset 0x6fd0
    mov    bx, 0002h                    ;Unused in function, stomped
    call   CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    mov    cl, MGA_CPC_PLLSetB OR MGA_CPC_ClockPLL;0x5
    call   ConfigurePixelClocks         ;Offset 0x6e51
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigureAndSelectPLLSetsAB ENDP

ConfigureAndSelectPLLSetsAB65Or73 PROC NEAR;Offset 0x6fe2
    push   eax
    push   ebx
    mov    eax, 0000fde8h               ;65,000
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    je     Freq65                       ;Offset 0x6ffa
    mov    eax, 00011d28h               ;73,000
Freq65:                                 ;Offset 0x6ffa
    mov    bx, 0002h                    ;Unused in function, stomped
    call   CalculatePLL_MNPS_AndNearestClockRate;Offset 0x6b47
    mov    cl, MGA_CPC_PLLSetA OR MGA_CPC_ClockPLL;0x4
    call   ConfigurePixelClocks         ;Offset 0x6e51
    mov    cl, MGA_CPC_PLLSetB OR MGA_CPC_ClockPLL;0x5
    call   ConfigurePixelClocks         ;Offset 0x6e51
    pop    ebx
    pop    eax
    ret
ConfigureAndSelectPLLSetsAB65Or73 ENDP

;Offset 0x700f
Data0x700f              DB 028h, 002h, 00Ch, 006h, 014h, 090h, 001h, 00Ah, 002h, 021h
                        DB 028h, 000h, 050h, 002h, 00Ch, 006h, 000h, 090h, 001h, 00Ah
                        DB 002h, 021h, 028h, 000h, 050h, 002h, 00Ch, 006h, 000h, 05Eh
                        DB 001h, 00Ah, 002h, 021h, 041h, 000h, 050h, 002h, 00Ch, 006h
                        DB 000h, 0E0h, 001h, 00Ah, 002h, 021h, 000h, 000h, 028h, 002h
                        DB 00Ch, 006h, 014h, 05Eh, 001h, 00Ah, 002h, 021h, 041h, 000h

;Offset 0x704b
Data0x704b              DB 028h, 003h, 011h, 014h, 02Ch, 090h, 001h, 003h, 006h, 01Dh
                        DB 0B8h, 000h, 050h, 003h, 011h, 014h, 018h, 090h, 001h, 003h
                        DB 006h, 01Dh, 0B8h, 000h, 050h, 003h, 011h, 014h, 018h, 05Eh
                        DB 001h, 003h, 006h, 01Dh, 0D1h, 000h, 050h, 003h, 011h, 014h
                        DB 018h, 0E0h, 001h, 003h, 006h, 01Dh, 090h, 000h, 028h, 003h
                        DB 011h, 014h, 02Ch, 05Eh, 001h, 003h, 006h, 01Dh, 0D1h, 000h

;Offset 0x7087                        
Data0x7087              DB 028h, 001h, 005h, 011h, 03Ch, 090h, 001h, 002h, 002h, 004h
                        DB 038h, 001h, 050h, 001h, 005h, 011h, 028h, 090h, 001h, 002h
                        DB 002h, 004h, 038h, 001h, 050h, 001h, 005h, 011h, 028h, 05Eh
                        DB 001h, 002h, 002h, 004h, 051h, 001h, 050h, 001h, 005h, 011h
                        DB 028h, 0E0h, 001h, 002h, 002h, 004h, 010h, 001h, 028h, 001h
                        DB 005h, 011h, 03Ch, 05Eh, 001h, 002h, 002h, 004h, 051h, 001h

;Offset 0x70c3
Data0x70c3              DB 0E8h, 0FDh, 000h, 000h, 04Dh, 055h, 000h, 000h

;Offset 0x70cb
Data0x70cb              DB 0A3h, 063h, 071h, 099h, 074h, 005h, 024h, 0F1h
                        DB 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 0AEh, 084h, 057h, 032h, 000h, 0ABh, 0D1h, 0E3h

;Offset 0x70e3
Data0x70e3              DB 0E0h, 0A5h, 001h, 000h, 08Dh, 055h, 040h, 021h

;Offset 0x70eb
Data0x70eb              DB 0B2h, 063h, 081h, 098h, 082h, 007h, 003h, 03Eh
                        DB 000h, 040h, 000h, 000h, 000h, 000h, 009h, 060h
                        DB 096h, 027h, 02Bh, 032h, 000h, 095h, 099h, 0E7h

                        DB 0FFh

;Offset 0x7104
Data0x7104              DB 0E8h, 003h, 003h, 0E8h
                        DB 017h, 003h, 000h, 000h
                        DB 000h, 0F2h, 015h, 002h

;Offset 0x7110
Data0x7110              DB 0D0h, 00Fh, 005h, 0D0h, 00Fh, 005h, 0A0h, 01Fh, 00Ah
                        DB 0E4h, 00Bh, 003h, 0E4h, 00Bh, 003h, 0C8h, 017h, 006h

;Offset 0x7122
Data0x7122              DB 0D8h, 01Ch, 007h, 0D8h, 015h, 006h, 000h, 000h, 000h
                        DB 062h, 01Bh, 005h, 000h, 000h, 000h, 0F0h, 00Fh, 003h

;Offset 0x7134
Data0x7134              DB 0B0h, 00Bh, 00Bh, 0B0h, 00Bh, 00Bh, 060h, 017h, 016h
                        DB 0C4h, 016h, 009h, 0C4h, 016h, 009h, 088h, 00Dh, 011h
                        DB 0E0h, 01Fh, 006h, 0E0h, 01Fh, 006h, 0C0h, 01Fh, 00Bh

;Offset 0x714f
Data0x714f              DB 000h, 000h, 00Ch, 00Ch, 000h, 000h, 00Ch, 018h, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 000h, 00Ch, 018h, 018h, 018h
                        DB 018h, 030h, 030h, 018h, 018h, 000h, 00Ch, 00Ch, 024h
                        DB 024h, 00Ch

Func0x716c PROC NEAR                    ;Offset 0x716c
    push   ds
    push   es
    pushaw
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    lea    si, [Data0x700f]             ;Offset 0x700f
    test   byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    nop
    jne    Label0x7193                  ;Offset 0x7193
    lea    si, [Data0x7087]             ;Offset 0x7087
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    nop
    jne    Label0x7193                  ;Offset 0x7193
    lea    si, [Data0x704b]             ;Offset 0x704b
Label0x7193:                            ;Offset 0x7193
    lea    di, [VideoParameters]        ;Offset 0x3c0
    add    di, 0700h
    mov    cx, 001dh
    push   bp
    sub    sp, 0018h
    mov    bp, sp
Label0x71a4:                            ;Offset 0x71a4
    push   si
    mov    bx, cx
    dec    bx
    movzx  ax, byte ptr cs:[bx + Data0x714f];Offset 0x714f
    cmp    al, 0ffh
    je     Label0x71c3                  ;Offset 0x71c3
    add    si, ax
    push   di
    mov    di, bp
    call   Func0x71f7                   ;Offset 0x71f7
    mov    si, bp
    pop    di
    mov    ax, 0000h
    call   Func0x66b1                   ;Offset 0x66b1
Label0x71c3:                            ;Offset 0x71c3
    sub    di, 0040h
    pop    si
    loop   Label0x71a4                  ;Offset 0x71a4
    add    sp, 0018h
    pop    bp
    test   byte ptr cs:[Data0x7ff1], 20h;Offset 0x7ff1
    nop
    je     Label0x71db                  ;Offset 0x71db
    call   Func0x72e9                   ;Offset 0x72e9
    jmp    Label0x71f3                  ;Offset 0x71f3
Label0x71db:                            ;Offset 0x71db
    call   Func0x7272                   ;Offset 0x7272
    lea    di, [Data0x53f6]             ;Offset 0x53f6
    test   byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    nop
    jne    Label0x71ef                  ;Offset 0x71ef
    lea    di, [Data0x53ee]             ;Offset 0x53ee
Label0x71ef:                            ;Offset 0x71ef
    mov    word ptr [di], 0ffffh
Label0x71f3:                            ;Offset 0x71f3
    popaw
    pop    es
    pop    ds
    ret
Func0x716c ENDP

Func0x71f7 PROC NEAR                    ;Offset 0x71f7
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
    jne    Label0x7231                  ;Offset 0x7231
    mov    word ptr ss:[di + 08h], 0001h
Label0x7231:                            ;Offset 0x7231
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
Func0x71f7 ENDP

Func0x7272 PROC NEAR                    ;Offset 0x7272
    push      eax
    push      bx
    push      cx
    push      dx
    push      di
    push      si
    lea       si, [Data0x70e3]          ;Offset 0x70e3
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x7289               ;Offset 0x7289
    lea       si, [Data0x70c3]          ;Offset 0x70c3
Label0x7289:                            ;Offset 0x7289
    lea       di, [Data0x564c]          ;Offset 0x564c
    mov       cx, 0004h
Label0x7290:                            ;Offset 0x7290
    ;mov       eax, dword ptr cs:[si]
    DB 02Eh, 066h, 08Bh, 004h
    ;mov       dword ptr cs:[di], eax
    DB 02Eh, 066h, 089h, 005h
    mov       ax, word ptr cs:[si + 04h]
    mov       word ptr cs:[di + 0ah], ax
    mov       ax, word ptr cs:[si + 06h]
    mov       word ptr cs:[di + 0ch], ax
    add       di, 000eh
    loop      Label0x7290               ;Offset 0x7290
    lea       si, [Data0x70eb]          ;Offset 0x70eb
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x72bd               ;Offset 0x72bd
    lea       si, [Data0x70cb]          ;Offset 0x70cb
Label0x72bd:                            ;Offset 0x72bd
    lea       di, [Data0x55d6]          ;Offset 0x55d6
    mov       cx, 0018h
    rep movsb
    mov       eax, 0000fde8h
    test      byte ptr cs:[Data0x7ff1], 40h;Offset 0x7ff1
    jne       Label0x72da               ;Offset 0x72da
    mov       eax, 00011d28h
Label0x72da:                            ;Offset 0x72da
    lea       di, [Data0x5692]          ;Offset 0x5692
    mov       dword ptr [di], eax
    pop       si
    pop       di
    pop       dx
    pop       cx
    pop       bx
    pop       eax
    ret
Func0x7272 ENDP

Func0x72e9 PROC NEAR                    ;Offset 0x72e9
    cld       
    lea       di, [Data0x53da]          ;Offset 0x53da
    mov       al, byte ptr cs:[Data0x7fef];Offset 0x7fef
    test      al, 01h
    je        Label0x72f9               ;Offset 0x72f9
    add       di, 0002h
Label0x72f9:                            ;Offset 0x72f9
    test      al, 04h
    je        Label0x7306               ;Offset 0x7306
    lea       si, [Data0x53dc]          ;Offset 0x53dc
    mov       cx, 0005h
    rep movsw
Label0x7306:                            ;Offset 0x7306
    test      al, 08h
    je        Label0x7313               ;Offset 0x7313
    lea       si, [Data0x53e6]          ;Offset 0x53e6
    mov       cx, 0004h
    rep movsw
Label0x7313:                            ;Offset 0x7313
    test      al, 10h
    je        Label0x7320               ;Offset 0x7320
    lea       si, [Data0x53ee]          ;Offset 0x53ee
    mov       cx, 0004h
    rep movsw
Label0x7320:                            ;Offset 0x7320
    mov       ax, 0ffffh
    stosw
    ret
Func0x72e9 ENDP

Func0x7325 PROC NEAR                    ;Offset 0x7325
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
Label0x734f:                            ;Offset 0x734f
    mov       si, offset MISCFuncs      ;Offset 0x4bed
    mov       edx, 00010000h
    mov       ch, 44h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    ;jne       Label0x7374               ;Offset 0x7374
    DB 00Fh, 085h, 011h, 000h
    mov       ch, 74h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    ;jne       Label0x7374               ;Offset 0x7374
    DB 00Fh, 085h, 006h, 000h
    dec       cl
    jne       Label0x734f               ;Offset 0x734f
    mov       ch, 00h
Label0x7374:                            ;Offset 0x7374
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
Func0x7325 ENDP

Func0x7387 PROC NEAR                    ;Offset 0x7387
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
Label0x73c0:                            ;Offset 0x73c0
    push      ss
    pop       ds
    mov       byte ptr ss:[di], 00h
    mov       si, offset Data0x4bf9     ;Offset 0x4bf9
    test      byte ptr cs:[Data0x7d1b], 40h;Offset 0x7d1b
    jne       Label0x73ea               ;Offset 0x73ea
    mov       si, offset DDCFuncs       ;Offset 0x4be1
    push      cx
    push      di
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    mov       di, PCI_Header_Byte_RevisionID;0x8
    call      AccessPCIRegister         ;Offset 0x68f8
    cmp       cl, MGA_G200_Rev3         ;0x3
    ;jb        Label0x73e8               ;Offset 0x73e8
    DB 00Fh, 082h, 003h, 000h
    mov       si, offset DDCFuncsRev3Up ;Offset 0x4bd5
Label0x73e8:                            ;Offset 0x73e8
    pop       di
    pop       cx
Label0x73ea:                            ;Offset 0x73ea
    mov       edx, 00010001h
    mov       ch, 0a2h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x7404               ;Offset 0x7404
    mov       ch, 0a0h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    ;je        Label0x747a               ;Offset 0x747a
    DB 00Fh, 084h, 076h, 000h
Label0x7404:                            ;Offset 0x7404
    cmp       byte ptr es:[di], 20h
    jne       Label0x7429               ;Offset 0x7429
    mov       byte ptr es:[di], 4fh
    mov       edx, 00010001h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    je        Label0x747a               ;Offset 0x747a
    cmp       byte ptr es:[di], 10h
    jb        Label0x7480               ;Offset 0x7480
    mov       ax, 8000h
    call      Func0x74da                ;Offset 0x74da
    jmp       Label0x744d               ;Offset 0x744d
Label0x7429:                            ;Offset 0x7429
    mov       edx, 00030001h
    mov       byte ptr ss:[di], 12h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    je        Label0x747a               ;Offset 0x747a
    cmp       byte ptr es:[di], 01h
    jne       Label0x7480               ;Offset 0x7480
    test      byte ptr es:[di + 02h], 80h
    je        Label0x7480               ;Offset 0x7480
    mov       ax, 8000h
    call      Func0x7497                ;Offset 0x7497
Label0x744d:                            ;Offset 0x744d
    and       ch, 06h
    shl       ch, 02h
    or        ah, ch
    test      byte ptr cs:[Data0x7fef], 10h;Offset 0x7fef
    jne       Label0x746a               ;Offset 0x746a
    test      byte ptr cs:[Data0x7fef], 07h;Offset 0x7fef
    je        Label0x7483               ;Offset 0x7483
    or        ah, 20h
    jmp       Label0x7483               ;Offset 0x7483
Label0x746a:                            ;Offset 0x746a
    or        ah, 40h
    test      byte ptr cs:[Data0x7fef], 0fh;Offset 0x7fef
    je        Label0x7483               ;Offset 0x7483
    or        ah, 20h
    jmp       Label0x7483               ;Offset 0x7483
Label0x747a:                            ;Offset 0x747a
    dec       cl
    jne       Label0x73c0               ;Offset 0x73c0
Label0x7480:                            ;Offset 0x7480
    mov       ax, 0000h
Label0x7483:                            ;Offset 0x7483
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
Func0x7387 ENDP

Func0x7497 PROC NEAR                    ;Offset 0x7497
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
    call      Func0x75ab                ;Offset 0x75ab
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x74d3               ;Offset 0x74d3
    mov       ah, 26h
    mov       al, 08h
    push      ax
    call      Func0x7693                ;Offset 0x7693
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x74d3               ;Offset 0x74d3
    mov       ah, 36h
    mov       al, 04h
    push      ax
    call      Func0x7721                ;Offset 0x7721
    pop       ax
Label0x74d3:                            ;Offset 0x74d3
    pop       edi
    pop       ax
    mov       sp, bp
    pop       bp
    ret
Func0x7497 ENDP

Func0x74da PROC NEAR                    ;Offset 0x74da
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
Label0x74fb:                            ;Offset 0x74fb
    mov       edx, 00030001h
    mov       byte ptr [di], 7eh
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x7512               ;Offset 0x7512
    dec       cl
    jne       Label0x74fb               ;Offset 0x74fb
    jmp       Label0x75a0               ;Offset 0x75a0
Label0x7512:                            ;Offset 0x7512
    mov       ax, 0000h
    test      byte ptr [di], 20h
    je        Label0x752a               ;Offset 0x752a
    mov       ah, 01h
    mov       al, byte ptr [di + 02h]
    test      al, 80h
    je        Label0x7525               ;Offset 0x7525
    mov       ah, 03h
Label0x7525:                            ;Offset 0x7525
    and       al, 0fh
    mul       ah
    inc       ax
Label0x752a:                            ;Offset 0x752a
    mov       byte ptr [bp - 01h], al
    mov       ax, word ptr [di]
    ;and       ax, 001ch
    DB 025h, 01Ch, 000h
    ;shl       ax, 10h
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
    jae       Label0x7564               ;Offset 0x7564
    mov       dx, 0006h
Label0x7564:                            ;Offset 0x7564
    sub       sp, dx
    mov       di, sp
    shl       edi, 10h
    mov       di, sp
    push      dx
    mov       al, byte ptr [bp - 02h]
    or        al, al
    je        Label0x7589               ;Offset 0x7589
    mov       ah, byte ptr [bp - 01h]
    add       ah, 80h
    push      ax
    call      Func0x7618                ;Offset 0x7618
    pop       ax
    cmp       byte ptr cs:[Data0x7fef], 3eh;Offset 0x7fef
    je        Label0x759d               ;Offset 0x759d
Label0x7589:                            ;Offset 0x7589
    mov       al, byte ptr [bp - 03h]
    or        al, al
    je        Label0x759d               ;Offset 0x759d
    mov       dl, byte ptr [bp - 02h]
    shl       dl, 02h
    add       ah, dl
    push      ax
    call      Func0x7721                ;Offset 0x7721
    pop       ax
Label0x759d:                            ;Offset 0x759d
    pop       dx
    add       sp, dx
Label0x75a0:                            ;Offset 0x75a0
    pop       edi
    pop       edx
    pop       ecx
    pop       ax
    mov       sp, bp
    pop       bp
    ret
Func0x74da ENDP

Func0x75ab PROC NEAR                    ;Offset 0x75ab
    push      bp
    mov       bp, sp
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x75bb:                            ;Offset 0x75bb
    mov       edx, 00020001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x75d3               ;Offset 0x75d3
    dec       cl
    jne       Label0x75bb               ;Offset 0x75bb
    jmp       Label0x7603               ;Offset 0x7603
Label0x75d3:                            ;Offset 0x75d3
    test      word ptr [di], 003ch
    je        Label0x75df               ;Offset 0x75df
    or        byte ptr cs:[Data0x7fef], 02h;Offset 0x7fef
Label0x75df:                            ;Offset 0x75df
    test      word ptr [di], 0c003h
    je        Label0x75eb               ;Offset 0x75eb
    or        byte ptr cs:[Data0x7fef], 04h;Offset 0x7fef
Label0x75eb:                            ;Offset 0x75eb
    test      word ptr [di], 1e00h
    je        Label0x75f7               ;Offset 0x75f7
    or        byte ptr cs:[Data0x7fef], 08h;Offset 0x7fef
Label0x75f7:                            ;Offset 0x75f7
    test      word ptr [di], 0100h
    je        Label0x7603               ;Offset 0x7603
    or        byte ptr cs:[Data0x7fef], 10h;Offset 0x7fef
Label0x7603:                            ;Offset 0x7603
    pop       edx
    pop       ecx
    mov       sp, bp
    pop       bp
    ret
Func0x75ab ENDP

;Offset 0x760b
Data0x760b              DW 08554h
                        DW 07D40h
                        DW 08530h
                        DW 08522h
                        DW 08518h
                        DW 0A018h

;Offset 0x7617
Data0x7617              DB 006h

Func0x7618 PROC NEAR                    ;Offset 0x7618 
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x762a:                            ;Offset 0x762a
    xor       dx, dx
    mov       dl, byte ptr [bp + 04h]
    shl       edx, 12h
    mov       dx, 0001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x7648               ;Offset 0x7648
    dec       cl
    jne       Label0x762a               ;Offset 0x762a
    jmp       Label0x767e               ;Offset 0x767e
Label0x7648:                            ;Offset 0x7648
    mov       ch, byte ptr [bp + 04h]
    sub       di, 0004h
Label0x764e:                            ;Offset 0x764e
    dec       ch
    js        Func0x76fe                ;Offset 0x76fe
    add       di, 0004h
    mov       dl, byte ptr [di]
    mov       dh, byte ptr [di + 02h]
    lea       si, [Data0x760b]          ;Offset 0x760b
    mov       cl, byte ptr cs:[Data0x7617];Offset 0x7617
Label0x7665:                            ;Offset 0x7665
    dec       cl
    js        Label0x764e               ;Offset 0x764e
    cmp       dx, word ptr cs:[si]
    je        Label0x7673               ;Offset 0x7673
    add       si, 0002h
    jmp       Label0x7665               ;Offset 0x7665
Label0x7673:                            ;Offset 0x7673
    mov       al, 01h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
    jmp       Label0x764e               ;Offset 0x764e
Label0x767e:                            ;Offset 0x767e
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x7618 ENDP

;Offset 0x7688
Data0x7688              DW 040A9h
                        DW 08081h
                        DW 04061h
                        DW 04045h
                        DW 04031h

;Offset 0x7692
Data0x7692              DB 005h

Func0x7693 PROC NEAR                    ;Offset 0x7693
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    mov       cl, 04h
    and       ecx, 0000ffffh
Label0x76a5:                            ;Offset 0x76a5
    xor       dx, dx
    mov       dl, byte ptr [bp + 04h]
    shl       edx, 11h
    mov       dx, 0001h
    mov       al, byte ptr [bp + 05h]
    mov       byte ptr [di], al
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x76c3               ;Offset 0x76c3
    dec       cl
    jne       Label0x76a5               ;Offset 0x76a5
    jmp       Func0x76fe                ;Offset 0x76fe
Label0x76c3:                            ;Offset 0x76c3
    mov       ch, byte ptr [bp + 04h]
    sub       di, 0002h
Label0x76c9:                            ;Offset 0x76c9
    dec       ch
    js        Func0x76fe                ;Offset 0x76fe
    add       di, 0002h
    cmp       word ptr [di], 0101h
    je        Func0x76fe                ;Offset 0x76fe
    mov       dx, word ptr [di]
    and       dx, 0c0ffh
    lea       si, [Data0x7688]          ;Offset 0x7688
    mov       cl, byte ptr cs:[Data0x7692];Offset 0x7692
Label0x76e5:                            ;Offset 0x76e5
    dec       cl
    js        Label0x76c9               ;Offset 0x76c9
    cmp       dx, word ptr cs:[si]
    je        Label0x76f3               ;Offset 0x76f3
    add       si, 0002h
    jmp       Label0x76e5               ;Offset 0x76e5
Label0x76f3:                            ;Offset 0x76f3
    mov       al, 02h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
    jmp       Label0x76c9               ;Offset 0x76c9
Func0x7693 ENDP
;continue!
Func0x76fe PROC NEAR                    ;Offset 0x76fe
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x76fe ENDP

;Offset 0x7708
Data0x7708              DD 0064004B0h
                        DD 005000400h
                        DD 004000300h
                        DD 003200258h
                        DD 0028001E0h
                        DD 002800190h

;Offset 0x7720
Data0x7720              DB 006h

Func0x7721 PROC NEAR                    ;Offset 0x7721
    push      bp
    mov       bp, sp
    push      di
    push      si
    push      ecx
    push      edx
    and       ecx, 0000ffffh
Label0x7731:                            ;Offset 0x7731
    mov       cl, 04h
Label0x7733:                            ;Offset 0x7733
    mov       al, byte ptr [bp + 05h]
    add       al, 02h
    mov       byte ptr [di], al
    mov       si, word ptr [bp - 04h]
    mov       edx, 00060001h
    call      Func0x5258                ;Offset 0x5258
    or        ax, ax
    jne       Label0x7750               ;Offset 0x7750
    dec       cl
    jne       Label0x7733               ;Offset 0x7733
    jmp       Label0x77ab               ;Offset 0x77ab
Label0x7750:                            ;Offset 0x7750
    mov       dh, byte ptr [di + 02h]
    shr       dh, 04h
    mov       dl, byte ptr [di]
    cmp       dx, 0000h
    jne       Label0x7772               ;Offset 0x7772
    cmp       byte ptr [di + 01h], 0fah
    jne       Label0x77a2               ;Offset 0x77a2
    mov       ah, byte ptr [bp + 05h]
    add       ah, 05h
    mov       al, 06h
    push      ax
    call      Func0x7693                ;Offset 0x7693
    pop       ax
    jmp       Label0x77a2               ;Offset 0x77a2
Label0x7772:                            ;Offset 0x7772
    shl       edx, 10h
    mov       dh, byte ptr [di + 05h]
    shr       dh, 04h
    mov       dl, byte ptr [di + 03h]
    lea       si, [Data0x7708]          ;Offset 0x7708
    mov       cl, byte ptr cs:[Data0x7720];Offset 0x7720
Label0x7788:                            ;Offset 0x7788
    dec       cl
    js        Label0x77a2               ;Offset 0x77a2
    ;cmp       edx, dword ptr cs:[si]
    DB 02Eh, 066h, 03Bh, 014h
    je        Label0x7799               ;Offset 0x7799
    ja        Label0x77a2               ;Offset 0x77a2
    add       si, 0004h
    jmp       Label0x7788               ;Offset 0x7788
Label0x7799:                            ;Offset 0x7799
    mov       al, 01h
    shl       al, cl
    or        byte ptr cs:[Data0x7fef], al;Offset 0x7fef
Label0x77a2:                            ;Offset 0x77a2
    add       byte ptr [bp + 05h], 12h
    dec       byte ptr [bp + 04h]
    jne       Label0x7731               ;Offset 0x7731
Label0x77ab:                            ;Offset 0x77ab
    pop       edx
    pop       ecx
    pop       si
    pop       di
    mov       sp, bp
    pop       bp
    ret
Func0x7721 ENDP

RebootMGAG200 PROC NEAR                 ;Offset 0x77b5
    push      di
    push      ecx
    push      edx
    push      bx
    push      ax
    mov       bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov       di, PCI_Header_Word_Command;0x4
LoopPCIRegistersRead:                   ;Offset 0x77c4
    mov       al, PCI_ACCESS_ReadDWord  ;0xa
    call      AccessPCIRegister         ;Offset 0x68f8
    cmp       di, PCI_H0_Word_SubsystemVendorID;0x2c
    ;jne       SkipSave                  ;Offset 0x77d3
    DB 00Fh, 085h, 003h, 000h
    mov       edx, ecx                  ;Save Subsystem Vendor ID
SkipSave:                               ;Offset 0x77d3
    push      ecx
    add       di, 0004h
    cmp       di, PCI_MGA_Option2 + 04h ;0x54
    jne       LoopPCIRegistersRead      ;Offset 0x77c4
    mov       di, PCI_MGA_AGPCommand    ;0xf8
    mov       al, PCI_ACCESS_ReadDWord  ;0xa
    call      AccessPCIRegister         ;Offset 0x68f8
    push      ecx
    mov       di, PCI_MGA_PowerMgmtControlStatus;0xe0
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    mov       cl, PCI_MGA_PWRCTRL_D3    ;0x3
    call      AccessPCIRegister         ;Offset 0x68f8
    mov       di, PCI_MGA_PowerMgmtControlStatus;0xe0
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    mov       cl, PCI_MGA_PWRCTRL_D0    ;0x0
    call      AccessPCIRegister         ;Offset 0x68f8
    push      dx
    mov       dx, 2ee0h                 ;12,000 ticks
    call      Sleep                     ;Offset 0x508e
    pop       dx
    pop       ecx
    mov       di, PCI_MGA_AGPCommand    ;0xf8
    mov       al, PCI_ACCESS_WriteDWord ;0xd
    call      AccessPCIRegister         ;Offset 0x68f8
    mov       di, PCI_MGA_Option2       ;0x50
LoopPCIRegisterWrite:                   ;Offset 0x7810
    mov       al, PCI_ACCESS_WriteDWord ;0xd
    pop       ecx
    cmp       di, PCI_MGA_SubsystemVendorIDWrite;0x4c
    ;jne       SkipStore                 ;Offset 0x781e
    DB 00Fh, 085h, 003h, 000h
    mov       ecx, edx                  ;Store Subsystem Vendor ID
SkipStore:                              ;Offset 0x781e
    call      AccessPCIRegister         ;Offset 0x68f8
    sub       di, 0004h
    jne       LoopPCIRegisterWrite      ;Offset 0x7810
    pop       ax
    pop       bx
    pop       edx
    pop       ecx
    pop       di
    ret
RebootMGAG200 ENDP

TalkToMonitorAndConfigureAladdinChipset PROC NEAR;Offset 0x782e
    push   ax
    push   si
    push   di
    call   Func0x7896                   ;Offset 0x7896
    call   ConfigureAladdinChipset      ;Offset 0x783b
    pop    di
    pop    si
    pop    ax
    ret
TalkToMonitorAndConfigureAladdinChipset ENDP

ConfigureAladdinChipset PROC NEAR       ;Offset 0x783b
    push   bx
    mov    ax, 0b102h                   ;Find PCI Device
    mov    cx, PCI_DEVICE_M1541_Aladdin ;0x1541 Device ID
    mov    dx, PCI_VENDOR_ALI           ;0x10b9 Vendor ID
    mov    si, 0000h                    ;Device Index
    int    1ah
    cmp    ah, 00h
    ;jne    NotFound                     ;Offset 0x788f
    DB 00Fh, 085h, 03Eh, 000h
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M1541_CPUtoPCIWriteBufferOption;0x86
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT ALI_M1541_CPUPCIWBO_PrgFrameBuffer;0xfe
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    ax, 0b102h                   ;Find PCI Device
    mov    cx, PCI_DEVICE_M5243_PCItoAGP_Controller;0x5243 Device ID
    mov    dx, PCI_VENDOR_ALI           ;0x10b9 Vendor ID
    mov    si, 0000h                    ;Device Index
    int    1ah
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M5243_CPUtoPCI66WriteBufferOption;0x86
    call   AccessPCIRegister            ;Offset 0x68f8
    and    cl, NOT ALI_M5243_CPUPCIWBO_VGAFixedFB;0xfe
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, ALI_M5243_PCI66ToMainMemPCI66ArbiterOpt;0x88
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, ALI_M5243_ForcePCI66GATMode;0x8
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
NotFound:                               ;Offset 0x788f
    pop    bx
    ret
ConfigureAladdinChipset ENDP

;Offset 0x7891
Data0x7891              DB 012h
                        DB 000h
                        DB 013h
                        DB 03Fh
                        DB 060h

Func0x7896 PROC NEAR                    ;Offset 0x7896
    push   ds
    push   ecx
    push   edx
    and    ecx, 0000ffffh
    mov    ch, 80h
    lea    di, [Data0x7891]             ;Offset 0x7891
    push   cs
    pop    ds
    lea    si, [MISCFuncs]              ;Offset 0x4bed
    mov    edx, 00000005h
Label0x78b4:                            ;Offset 0x78b4
    call   Func0x5258                   ;Offset 0x5258
    or     ax, ax
    jne    Label0x78c5                  ;Offset 0x78c5
    cmp    ch, 88h
    je     Label0x78c5                  ;Offset 0x78c5
    add    ch, 04h
    jmp    Label0x78b4                  ;Offset 0x78b4
Label0x78c5:                            ;Offset 0x78c5
    pop    edx
    pop    ecx
    pop    ds
    ret
Func0x7896 ENDP

;PCI Configuration Mechanism 2
FindMGAG200Mechanism2 PROC NEAR         ;Offset 0x78cb
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
Label0x78df:                            ;Offset 0x78df
    xor    dl, dl
    in     ax, dx
    cmp    ax, PCI_VENDOR_Matrox        ;0x102b
    jne    Label0x7901                  ;Offset 0x7901
    mov    dl, PCI_Header_Word_DeviceID ;0x2
    in     ax, dx
    cmp    ax, PCI_DEVICE_G200PCI       ;0x0520
    jne    Label0x7901                  ;Offset 0x7901
    mov    dl, PCI_Header_Word_Command  ;0x4
    in     al, dx
    and    al, PCI_Cmd_IOSpace          ;0x1
    je     Label0x7901                  ;Offset 0x7901
    mov    dl, PCI_Header_Byte_SubClassCode;0xa
    in     al, dx
    or     al, al
    jne    Label0x7901                  ;Offset 0x7901
    mov    bh, dh
    jmp    Label0x791e                  ;Offset 0x791e
Label0x7901:                            ;Offset 0x7901
    xor    dl, dl
    inc    dh
    cmp    dh, 0d0h
    jne    Label0x78df                  ;Offset 0x78df
    mov    dh, 0c0h
    inc    bx
    mov    al, bl
    push   dx
    mov    dx, PCI_M2_ForwardRegister   ;Port 0xcfa
    out    dx, al
    pop    dx
    cmp    bx, 0100h
    jne    Label0x78df                  ;Offset 0x78df
    mov    bx, 0ffffh
Label0x791e:                            ;Offset 0x791e
    xor    al, al
    mov    dx, PCI_M2_ConfigSpaceEnable ;Port 0xcf8
    out    dx, al
    pop    dx
    pop    cx
    pop    ax
    ret
FindMGAG200Mechanism2 ENDP

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
FindMGAG200Mechanism1 PROC NEAR         ;Offset 0x7928
    push  eax
    push  ebx
    push  ecx
    push  edx
    pushf
    cli
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch                       ;PCI M1 port high byte
LoopBus:                                ;Offset 0x793c
    mov   cl, 20h                       ;Check 32 devices on this bus
LoopDevices:                            ;Offset 0x793e
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax                       ;Address for Header field 0
    mov   dl, PCI_M1_ConfigData_lowbyte ;Port 0xcfc
    in    eax, dx                       ;Read Device + Vendor ID's
    cmp   eax, (PCI_DEVICE_G200PCI SHL 16) OR PCI_VENDOR_Matrox;0x0520102b
    jne   NextDevice                    ;Offset 0x7977
    mov   eax, ebx
    mov   al, PCI_Header_Word_Command   ;0x4
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B0_lowbyte;Port 0xcfc
    in    al, dx
    and   al, PCI_Cmd_IOSpace           ;0x1 Check if not IO space
    je    NextDevice                    ;Offset 0x7977
    mov   eax, ebx
    mov   al, PCI_Header_Byte_RevisionID;0x8
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B2_lowbyte;Port 0xcfe
    in    al, dx                        ;PCI_Header_Byte_SubClassCode
    or    al, al                        ;Check if not 0
    jne   NextDevice                    ;Offset 0x7977
    shr   ebx, 08h                      ;bx = Bus, Device ID and function ID
    jmp   DeviceFound                   ;Offset 0x7996
NextDevice:                             ;Offset 0x7977
    dec   cl
    je    NextBus                       ;Offset 0x7984
    add   ebx, 00000800h                ;Next device
    jmp   LoopDevices                   ;Offset 0x793e
NextBus:                                ;Offset 0x7984
    and   bx, 07ffh                     ;Drop device index
    add   ebx, 00010000h                ;Next Bus
    dec   ch
    jne   LoopBus                       ;Offset 0x793c
    mov   bx, 0ffffh                    ;Device not found, set Bus, device ID and function ID to invalid
DeviceFound:                            ;Offset 0x7996
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
FindMGAG200Mechanism1 ENDP

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
FindIntelTriton430FXChipset PROC NEAR   ;Offset 0x79ab
    push  ebx
    pushf
    cli
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch
    mov   cl, 10h
CheckNextDevice:                        ;Offset 0x79bb
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_lowbyte;Port 0xcfc
    in    eax, dx
    cmp   eax, (PCI_DEVICE_Triton_430FX SHL 16) OR PCI_VENDOR_Intel;Intel Triton 430FX chipset 0x122d8086
    je    FoundDevice                   ;Offset 0x79de
    dec   cl
    je    SearchDone                    ;Offset 0x79db
    add   ebx, 00000800h
    jmp   CheckNextDevice               ;Offset 0x79bb
SearchDone:                             ;Offset 0x79db
    xor   eax, eax
FoundDevice:                            ;Offset 0x79de
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
FindMGAG200 PROC NEAR                   ;Offset 0x79ea
    call  FindMGAG200Mechanism1         ;Offset 0x7928
    inc   bx                            ;Check if found
    jne   Found                         ;Offset 0x79f4
    call  FindMGAG200Mechanism2         ;Offset 0x78cb
    inc   bx
Found:                                  ;Offset 0x79f4
    dec   bx
    ret
FindMGAG200 ENDP

SetMemoryTimingsAndRefresh PROC NEAR    ;Offset 0x79f6
    push   ax
    push   bx
    push   ecx
    push   dx
    push   di
    mov    bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    ax, (VGA_SEQ1_ScreenOff SHL 8)OR VGA_SEQIdx_ClockingMode;0x2001
    out    dx, ax
    test   byte ptr cs:[Data0x7d14], 10h;Offset 0x7d14
    ;jne    Label0x7a2e                  ;Offset 0x7a2e
    DB 00Fh, 085h, 01Ch, 000h
    mov    si, MGA_MemAddr_PlaneWriteMask;0x1c1c
    mov    ecx, 0a5a5a5a5h              ;mask pattern
    call   IndirectRegisterWriteDWord   ;Offset 0x6a03
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    or     cl, PCA_MGA_Opt_HWPlaneWriteMaskOn SHR 8;0x40
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
Label0x7a2e:                            ;Offset 0x7a2e
    mov    si, MGA_MemAddr_MemoryControlWaitState;0x1c08
    ;mov    ecx, dword ptr cs:[MemoryControlWaitState];Offset 0x7d10
    DB 02Eh, 066h, 08Bh, 00Eh, 010h, 07Dh
    cmp    ecx, 0ffffffffh
    ;jne    Label0x7a45                  ;Offset 0x7a45
    DB 00Fh, 085h, 006h, 000h
    mov    ecx, 04244ca1h
Label0x7a45:                            ;Offset 0x7a45
    call   IndirectRegisterWriteDWord   ;Offset 0x6a03
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    dl, byte ptr cs:[Data0x7d16] ;Offset 0x7d16
    mov    ch, dl
    and    ch, 07h
    shl    ch, 02h
    or     cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    di, PCI_MGA_Option2 + 01h    ;0x51
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    dl, byte ptr cs:[Data0x7d1a] ;Offset 0x7d1a
    mov    ch, dl
    and    ch, 03h
    shl    ch, 04h
    and    cl, 0cfh
    or     cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    si, MGA_MemAddr_MemoryReadBack + 03h;0x1e47
    mov    cl, byte ptr cs:[Data0x7d19] ;Offset 0x7d19
    and    cl, 0f0h
    shr    cl, 03h
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
    mov    si, MGA_MemAddr_MemoryReadBack + 02h;0x1e46
    mov    cl, byte ptr cs:[Data0x7d19] ;Offset 0x7d19
    and    cl, 03h
    ror    cl, 02h
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x68f8
    and    ecx, NOT PCI_MGA_Opt_RefreshCounter_MASK;0xffe07fff
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2bfc
    mov    si, MGA_MemAddr_MemoryAccess + 01h;0x1c05
    mov    cl, MGA_MA_MA_MemoryActive SHR 8;0x0
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
    mov    cl, MGA_MA_MA_MemoryReset SHR 8;0x80
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2bfc
    mov    si, MGA_MemAddr_MemoryReadBack;0x1e44
    xor    cx, cx
    mov    dl, byte ptr cs:[MemoryClockBaseReadDelayDefault];Offset 0x7d18
    mov    cl, dl
    and    cl, 0f0h
    shl    cx, 01h
    and    dl, 0fh
    or     cl, dl
    call   IndirectRegisterWriteWord    ;Offset 0x69e5
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x68f8
    and    ecx, NOT PCI_MGA_Opt_RefreshCounter_MASK;0xffe07fff
    or     ecx, 00078000h               ;Refresh Counter
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x68f8
    mov    si, MGA_MemAddr_Status + 03h ;0x1e17
    mov    cl, 80h                      ;Software flag...
    call   IndirectRegisterWriteByte    ;Offset 0x69c7
    pop    di
    pop    dx
    pop    ecx
    pop    bx
    pop    ax
    ret
SetMemoryTimingsAndRefresh ENDP

PowerUpPLLsLUTsAndConfigure PROC NEAR   ;Offset 0x7b13
    push  ax
    push  bx
    push  ecx
    push  edx
    push  di
    push  cx
    mov   bx, word ptr cs:[PCIBusDeviceIDFunctionID];Offset 0x7ff2
    call  FindIntelTriton430FXChipset   ;Offset 0x79ab
    or    ax, ax
    je    NoIntelTriton430FX            ;Offset 0x7b37
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_MGA_Option + 03h      ;0x43
    call  AccessPCIRegister             ;Offset 0x68f8
    or    cl, (PCI_MGA_Opt_NoRetry SHR 24);0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x68f8
NoIntelTriton430FX:                     ;Offset 0x7b37
    mov   cl, MGA_INDD_VoltageReferenceControl;0x18
    mov   ch, MGA_VREFCTRL_SysPLLVoltRefBlkPwrUp OR MGA_VREFCTRL_SysPLLVoltRefPLLRefBlk;0x03
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   al, 0fh
    call  Sleep2                        ;Offset 0x2bfc
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x68f8
    or    cl, PCI_MGA_Opt_SysPLLPowerUp ;0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x68f8
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2bfc
    xor   dx, dx
PLLNotFrequencyLocked:                  ;Offset 0x7b5a
    dec   dx
    je    PLLFreqLockTimeout            ;Offset 0x7b67
    mov   cl, MGA_INDD_SYSPLL_Status    ;0x2f
    call  ReadIndexedRegister           ;Offset 0x6a80
    and   cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je    PLLNotFrequencyLocked         ;Offset 0x7b5a
PLLFreqLockTimeout:                     ;Offset 0x7b67
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6a80
    or    cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2bfc
    xor   dx, dx
PIXNotFrequencyLocked:                  ;Offset 0x7b7d
    dec   dx
    je    PIXFreqLockTimeout            ;Offset 0x7b8a
    mov   cl, MGA_INDD_PIXPLL_Status    ;0x4f
    call  ReadIndexedRegister           ;Offset 0x6a80
    and   cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je    PIXNotFrequencyLocked         ;Offset 0x7b7d
PIXFreqLockTimeout:                     ;Offset 0x7b8a
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x6a80
    or    cl, MGA_MISCCTRL_LUT_Enable   ;0x10
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x6a80
    or    cl, MGA_MISCCTRL_DAC_Enable   ;0x1
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x68f8
    or    cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x68f8
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x68f8
    and   cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or    cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x68f8
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x68f8
    and   cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x68f8
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6a80
    or    cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6a80
    and   cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or    cl, MGA_PIXCLKCTRL_SelPLL     ;0x1
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x6a80
    and   cl, NOT MGA_PIXCLKCTRL_ClockDisable;0fbh
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x6a6b
    pop   dx
    or    dx, dx
    je    Label0x7c16                   ;Offset 0x7c16
    mov   cx, PCI_MGA_Opt_SysClockSelectPLL OR ((PCI_MGA_Opt2_NoMemClockDiv OR PCI_MGA_Opt2_UseGfxClockDiv) SHR 9);0x41
    jmp   Label0x7c19                   ;Offset 0x7c19
    nop
Label0x7c16:                            ;Offset 0x7c16
    call  FetchDefaultSystemClockSpeed  ;Offset 0x6a97
Label0x7c19:                            ;Offset 0x7c19
    mov   ax, dx                        ;ax = frequency in KHz
    xor   dx, dx
    mov   bx, 03e8h                     ;1,000
    mul   bx                            ;dx:ax is frequency in Hz
    shl   edx, 10h
    mov   dx, ax                        ;edx = frequency in Hz
    call  ConfigureSystemClock          ;Offset 0x6ce3
    pop   di
    pop   edx
    pop   ecx
    pop   bx
    pop   ax
    ret
PowerUpPLLsLUTsAndConfigure ENDP

CacheMemorySize PROC NEAR               ;Offset 0x7c33
    call  GetMemorySize                 ;Offset 0x7c3e
    shr   ax, 04h
    mov   byte ptr cs:[MemorySizeInMB], al;Offset 0x7ff0
    ret
CacheMemorySize ENDP

;inputs:
;   -
;
;outputs:
;   ax = Size of memory in 64KB blocks
;
GetMemorySize PROC NEAR                 ;Offset 0x7c3e
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
Label0x7c55:                            ;Offset 0x7c55
    loop  Label0x7c55                   ;Offset 0x7c55 Speed sensitive!
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
Label0x7c6b:                            ;Offset 0x7c6b
    dec   cx
    je    Label0x7c7b                   ;Offset 0x7c7b
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne   Label0x7c6b                   ;Offset 0x7c6b
Label0x7c73:                            ;Offset 0x7c73
    dec   cx
    je    Label0x7c7b                   ;Offset 0x7c7b
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x7c73                   ;Offset 0x7c73
Label0x7c7b:                            ;Offset 0x7c7b
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
    mov   si, 0fffeh
    mov   di, 0020h                     ;2MB
    mov   bx, 0100h                     ;16MB max
    mov   ax, (20h SHL 8)OR MGA_CRTCExt_MemoryPage;0x2004 Start at 2MB
Label0x7c9d:                            ;Offset 0x7c9d
    out   dx, ax
    mov   ch, byte ptr [si]
    mov   cl, 0a5h
    mov   byte ptr [si], cl
    push  di
    push  cx
    not   cx
    mov   di, 000eh
    mov   byte ptr [di], cl
    pop   cx
    pop   di
    cmp   cl, byte ptr [si]
    mov   byte ptr [si], ch
    jne   Label0x7cbf                   ;Offset 0x7cbf
    add   di, 0010h                     ;Found one more MB
    add   ah, 10h                       ;Increment by 1MB
    cmp   bx, di
    jne   Label0x7c9d                   ;Offset 0x7c9d
Label0x7cbf:                            ;Offset 0x7cbf
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
    mov   ax, di
    pop   si
    pop   di
    pop   ds
    pop   dx
    pop   cx
    pop   bx
    ret
GetMemorySize ENDP

    xchg  bx, bx                        ;hello
    xchg  bx, bx
    xchg  bx, bx
    xchg  bx, bx
    xchg  bx, bx

;Offset 0x7ce0
PINS                    DB 02Eh, 041h
                        DB 040h ;Offset from 0x7ce0 to the protected mode table: 0x7d00 + 0x40 = 0x7d40
                        DB 0FFh, 000h, 003h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 0FFh, 0FFh

;Offset 0x7d04
Data0x7d04              DB 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x7d09
PLLADefaultFreq         DB 0FFh

;Offset 0x7d0a
PLLBDefaultFreq         DB 0FFh

;OFfset 0x7d0b
Data0x7d0b              DB 0FFh

;Offset 0x7d0c
Data0x7d0c              DB 03Eh

;Offset 0x7d0d
Data0x7d0d              DB 0FFh

                        DB 0FFh, 0FFh

;Offset 0x7d10
MemoryControlWaitState  DD 03258A31h    ;These are the memory timings of the G100, which are incompatible with the G200!!!!!

;Offset 0x7d14
Data0x7d14              DB 000h

;Offset 0x7d15
Data0x7d15              DB 0EEh         ;bit 0 = Pedestal control (pedcon), General control register XGENCTRL
                                        ;bit 4 = Composite Sync Enable (csyncen) in CRTCEXT3
                                        ;bit 5 = Green sync output control (iogsyncdis) 

;Offset 0x7d16
Data0x7d16              DB 0FFh

;Offset 0x7d17
Data0x7d17              DB 0FFh         ;bit 2-0 = hiprilvl in CRTCEXT3
                                        ;bit 5-3 = hiprilvl in CRTCEXT3

;Offset 0x7d18
MemoryClockBaseReadDelayDefault DB 088h ;bit 3-0 = mclkbrd0 in MEMRDBK
                                        ;bit 7-4 = mclkbrd1 in MEMRDBK

;Offset 0x7d19
Data0x7d19              DB 000h         ;bit 1-0 = strmfctl in MEMRDBK
                                        ;bit 7-4 = mrsopcod(lower 4 bits) in MEMRDBK

;Offset 0x7d1a
Data0x7d1a              DB 000h         ;bit 1-0 = mbuftype in OPTION2

;Offset 0x7d1b
Data0x7d1b              DB 0FFh

;Offset 0x7d1e
                        DB 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x7d20
                        DW 0005Fh       ;00h    WORD    offset (within table) of protected-mode code for Function 5 (Set Window) : Points to 0x7d7f
                        DW 00092h       ;02h    WORD    offset of protected-mode code for Function 7 (Set Disp Start) : Points to 0x7dc2
                        DW 000FEh       ;04h    WORD    offset of protected-mode code for Function 9 (Set Primary Palette) : Points to 0x7e1e
                        DW 0000Ch       ;06h    WORD    offset (within table) of list of I/O ports and memory locations
                                        ;               for which I/O privilege may be required in protected mode (0000h if no list) : Points to 0x7d2c
                                        ;08h    var     code and optional port/memory list
                        DW 0020Ch       ;Size
                        DW 00000h       ;?

;Offset 0x7d2c
                        DW 003DEh       ;Port list
                        DW 003DFh
                        DW 0FFFFh
                        DW 0FFFFh       ;Memory list

;Offset 0x7d34
                        DB 0E8h, 003h, 003h, 0E8h, 017h, 003h, 000h, 000h, 000h, 0F2h, 015h, 002h
                        DB 0D0h, 00Fh, 005h, 0D0h, 00Fh, 005h, 0A0h, 01Fh, 00Ah, 0E4h, 00Bh, 003h, 0E4h, 00Bh, 003h, 0C8h
                        DB 017h, 006h, 0D8h, 01Ch, 007h, 0D8h, 015h, 006h, 000h, 000h, 000h, 062h, 01Bh, 005h, 000h, 000h
                        DB 000h, 0F0h, 00Fh, 003h, 0B0h, 00Bh, 00Bh, 0B0h, 00Bh, 00Bh, 060h, 017h, 016h, 0C4h, 016h, 009h
                        DB 0C4h, 016h, 009h, 088h, 00Dh, 011h, 0E0h, 01Fh, 006h, 0E0h, 01Fh, 006h, 0C0h, 01Fh, 00Bh

;Can't have 16-bit tiny code and 32-bit flat code in the same file, so here is the data with code as comment:

                                        ;SetWindow32bit PROC NEAR                ;Offset 0x7d7f
DB 066h, 052h                           ;    push dx
DB 051h                                 ;    push ecx
DB 0E8h, 017h, 001h, 000h, 000h         ;    call Func32bit0x7eae                ;Offset 0x7eae
DB 00Ah, 0FFh                           ;    or   bh, bh
DB 075h, 00Fh                           ;    jne  Label0x7d9a                    ;Offset 0x7d9a
DB 08Ah, 0E2h                           ;    mov  ah, dl
DB 002h, 0E1h                           ;    add  ah, cl
DB 059h                                 ;    pop  ecx
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 066h, 0EFh                           ;    out  dx, ax
DB 0EBh, 011h                           ;    jmp  Label0x7dab                    ;Offset 0x7dab
                                        ;Label0x7d9a:                            ;Offset 0x7d9a
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 02Ah, 0C1h                           ;    sub  al, cl
DB 059h                                 ;    pop  ecx
DB 08Ah, 0D0h                           ;    mov  dl, al
DB 032h, 0F6h                           ;    xor  dh, dh
                                        ;Label0x7dab:                            ;Offset 0x7dab
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;SetWindow32bit ENDP

                                        ;SetDisplayStart32bit PROC NEAR          ;Offset 0x7db2
DB 050h                                 ;    push eax
DB 066h, 051h                           ;    push cx
DB 066h, 052h                           ;    push dx
DB 066h, 08Bh, 0C2h                     ;    mov  ax, dx
DB 0C1h, 0C0h, 010h                     ;    rol  eax, 10h
DB 066h, 08Bh, 0C1h                     ;    mov  ax, cx
DB 0D1h, 0E8h                           ;    shr  eax, 01h
DB 051h                                 ;    push ecx
DB 0E8h, 0D6h, 000h, 000h, 000h         ;    call Func32bit0x7e9e                ;Offset 0x7e9e
DB 0C1h, 0E9h, 008h                     ;    shr  ecx, 08h
DB 003h, 0C1h                           ;    add  eax, ecx
DB 059h                                 ;    pop  ecx
DB 050h                                 ;    push eax
DB 0F6h, 0C3h, 080h                     ;    test bl, 80h
DB 00Fh, 084h, 010h, 000h, 000h, 000h   ;    je   Label0x7de8                    ;Offset 0x7de8
DB 066h, 0BAh, 0DAh, 003h               ;    mov  dx, VGA_InputStatus1D          ;Port 0x3da
DB 066h, 033h, 0C9h                     ;    xor  cx, cx
                                        ;Label0x7ddf:                            ;Offset 0x7ddf
DB 066h, 049h                           ;    dec  cx
DB 074h, 005h                           ;    je   Label0x7de8                    ;Offset 0x7de8
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0F7h                           ;    je   Label0x7ddf                    ;Offset 0x7ddf
                                        ;Label0x7de8:                            ;Offset 0x7de8
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

                                        ;SetPrimaryPalette32bit PROC NEAR        ;Offset 0x7e2e
DB 080h, 0FBh, 002h                     ;    cmp  bl, 02h
DB 074h, 076h                           ;    je   Label0x7e99                    ;Offset 0x7e99
DB 080h, 0FBh, 003h                     ;    cmp  bl, 03h
DB 074h, 071h                           ;    je   Label0x7e99                    ;Offset 0x7e99
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
DB 075h, 017h                           ;    jne  Label0x7e55                    ;Offset 0x7e55
DB 0B2h, 0DAh                           ;    mov  dl, VGA_InputStatus1D_lowbyte  ;Port 0x3da
DB 066h, 033h, 0C9h                     ;    xor  cx, cx
                                        ;Label0x7e43:                            ;Offset 0x7e43
DB 066h, 049h                           ;    dec  cx
DB 074h, 00Eh                           ;    je   Label0x7e55                    ;Offset 0x7e55
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0F7h                           ;    jne  Label0x7e43                    ;Offset 0x7e43
                                        ;Label0x7e4c:                            ;Offset 0x7e4c
DB 066h, 049h                           ;    dec  cx
DB 074h, 005h                           ;    je   Label0x7e55                    ;Offset 0x7e55
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0F7h                           ;    je   Label0x7e4c                    ;Offset 0x7e4c
                                        ;Label0x7e55:                            ;Offset 0x7e55
DB 066h, 059h                           ;    pop  cx
DB 0B2h, 0C9h                           ;    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 024h, 001h                           ;    and  al, 01h
DB 074h, 018h                           ;    je   Label0x7e77                    ;Offset 0x7e77
                                        ;Label0x7e5f:                            ;Offset 0x7e5f
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
DB 075h, 0EAh                           ;    jne  Label0x7e5f                    ;Offset 0x7e5f
DB 0EBh, 016h                           ;    jmp  Label0x7e8d                    ;Offset 0x7e8d
                                        ;Label0x7e77:                            ;Offset 0x7e77
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
DB 075h, 0EAh                           ;    jne  Label0x7e77                    ;Offset 0x7e77
                                        ;Label0x7e8d:                            ;Offset 0x7e8d
DB 05Fh                                 ;    pop  edi
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 059h                           ;    pop  cx
DB 066h, 05Bh                           ;    pop  bx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;Label0x7e99:                            ;Offset 0x7e99
DB 066h, 0B8h, 04Fh, 002h               ;    mov  ax, 024fh
DB 0C3h                                 ;    ret
                                        ;SetPrimaryPalette32bit ENDP

                                        ;Func32bit0x7e9e PROC NEAR               ;Offset 0x7e9e
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
DB 074h, 06Bh                           ;    je   Label0x7f22                    ;Offset 0x7f22
DB 024h, 03Fh                           ;    and  al, 3fh
DB 03Ch, 020h                           ;    cmp  al, 20h
DB 072h, 065h                           ;    jb   Label0x7f22                    ;Offset 0x7f22
DB 080h, 0E4h, 040h                     ;    and  ah, 40h
DB 075h, 020h                           ;    jne  Label0x7ee2                    ;Offset 0x7ee2
DB 03Ch, 023h                           ;    cmp  al, 23h
DB 077h, 00Ah                           ;    ja   Label0x7ed0                    ;Offset 0x7ed0
DB 0BEh, 014h, 000h, 000h, 000h         ;    mov  esi, 00000014h
DB 0EBh, 030h                           ;    jmp  Label0x7efd                    ;Offset 0x7efd
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7ed0:                            ;Offset 0x7ed0
DB 03Ch, 030h                           ;    cmp  al, 30h
DB 072h, 04Eh                           ;    jb   Label0x7f22                    ;Offset 0x7f22
DB 03Ch, 035h                           ;    cmp  al, 35h
DB 077h, 04Ah                           ;    ja   Label0x7f22                    ;Offset 0x7f22
DB 0BEh, 020h, 000h, 000h, 000h         ;    mov  esi, 00000020h
DB 0EBh, 01Eh                           ;    jmp  Label0x7efd                    ;Offset 0x7efd
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7ee2:                            ;Offset 0x7ee2
DB 03Ch, 025h                           ;    cmp  al, 25h
DB 077h, 00Ah                           ;    ja   Label0x7ef0                    ;Offset 0x7ef0
DB 0BEh, 032h, 000h, 000h, 000h         ;    mov  esi, 00000032h
DB 0EBh, 010h                           ;    jmp  Label0x7efd                    ;Offset 0x7efd
DB 090h                                 ;    nop
DB 090h                                 ;    nop
DB 090h                                 ;    nop
                                        ;Label0x7ef0:                            ;Offset 0x7ef0
DB 03Ch, 030h                           ;    cmp  al, 30h
DB 072h, 02Eh                           ;    jb   Label0x7f22                    ;Offset 0x7f22
DB 03Ch, 038h                           ;    cmp  al, 38h
DB 077h, 02Ah                           ;    ja   Label0x7f22                    ;Offset 0x7f22
DB 0BEh, 044h, 000h, 000h, 000h         ;    mov  esi, 00000044h
                                        ;Label0x7efd:                            ;Offset 0x7efd
DB 0E8h, 026h, 000h, 000h, 000h         ;    call Func32bit0x7f28                ;Offset 0x7f28
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
                                        ;Label0x7f22:                            ;Offset 0x7f22
DB 066h, 05Ah                           ;    pop  dx
DB 05Eh                                 ;    pop  esi
DB 05Fh                                 ;    pop  edi
DB 058h                                 ;    pop  eax
DB 0C3h                                 ;    ret
                                        ;Func32bit0x7e9e ENDP

                                        ;Func32bit0x7f28 PROC NEAR               ;Offset 0x7f28
DB 08Bh, 00Ch, 024h                     ;    mov  ecx, dword ptr [esp]
DB 0C3h                                 ;    ret
                                        ;Func32bit0x7f28 ENDP

;Offset 0x7f2c
DB 000h

;Offset 0x7f2d
DB 0c2h dup (0FFh)

;Offset 0x7fef
Data0x7fef              DB 000h

;Offset 0x7ff0
MemorySizeInMB          DB 000h

;Offset 0x7ff1
Data0x7ff1              DB 000h

;Offset 0x7ff2
PCIBusDeviceIDFunctionID DW 0000h

;Offset 0x7ff4
Data0x7ff4              DB 000h

;Offset 0x7ff5
Data0x7ff5              DB 000h

;Offset 0x7ff6
DB 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x77fc
PINSPointer             DW offset PINS  ;Offset 0x7d00

DB 0FFh

DB 5Eh
ROMEnd:
END
