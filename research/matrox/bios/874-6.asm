;Matrox G100 AGP BIOS v1.6 b22 disassembly

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
;Matrox G100 Specification: https://old.vgamuseum.info/images/stories/doc/matrox/mga-g100_spec.pdf
;
;ALI M1541 Specification: https://bitsavers.org/components/ali/M1541_M1542_Aladdin_V_Socket_7_North_Bridge.pdf
;
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT 874-6.asm
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
include g100.inc

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
PCI_DEVICE_G100AGP                      EQU 1001h;0x1001
PCI_DEVICE_MillenniumIIPCI              EQU 051bh;0x051b

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
                        DB 016h         ;BIOS Version (1.6)
                        DB 0E6h, 003h, 0C0h, 004h, 000h, 00Ch, 04Eh, 031h, 03Ah, 00Ch
                        DB 044h, 00Ch, 0FFh, 0FFh, 000h, 000h, 000h, 000h

;Offset 0x18
                        DW offset PCIHeader;0x60
                        DW 0000h        ;PnP header pointer
                        DW 2000h        ;Unknown
                        DB 'IBM COMPATIBLE MATROX/MGA-G100 VGA/VBE BIOS (V1.6 )', 000h

;Offset 0x52
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

;Offset 0x60
PCIHeader               DB 'PCIR'       ;PCI header magic
                        DW PCI_VENDOR_Matrox;Vendor ID
                        DW PCI_DEVICE_G100AGP;Device ID
                        DW 0000h        ;Pointer to vital product data (0 = none)
                        DW 0018h        ;PCI data structure length
                        DB 00h          ;PCI data structure revision
                        DB 00h, 00h, 03h;PCI device class code
                        DW (ROMEnd - ROMStart)/512;Rom size in 512 byte blocks
                        DW 1616h        ;Revision level of code
                        DB 00h          ;Code type (0 = x86)
                        DB 80h          ;Last image indicator
                        DW 0000h        ;Reserved
;Offset 0x78
BuildVersion            DB '874-6', 000h
                        DB 0FFh, 0FFh

entry:                                  ;Offset 0x80
    call      ConfigureAGPBus           ;Offset 0x5149
    mov       bx, ax
    call      ReadSubClassCode          ;Offset 0x51e5
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
    call      FindMGAG100               ;Offset 0x53d6
    call      FindMGAG100               ;Offset 0x53d6 This has to be a bug...
    call      TurnScreenOff             ;Offset 0x2b3d
    test      byte ptr cs:[Data0x7554], 08h;Offset 0x7554
    je        Label0xd7                 ;Offset 0xd7
    mov       di, PCI_MGA_Option        ;0x40
    nop
    mov       al, PCI_ACCESS_ReadByte   ;0x8
    nop
    call      AccessPCIRegister         ;Offset 0x5344
    or        cl, PCI_MGA_Opt_PLLSelP0PixP1Sys;0x40
    mov       al, PCI_ACCESS_WriteByte  ;0xb
    nop
    call      AccessPCIRegister         ;Offset 0x5344
Label0xd7:                              ;Offset 0xd7
    mov       ax, ((MGA_CRTCEXT6_MaxPrioLevel6 OR MGA_CRTCEXT6_HiPrioLevel1) SHL 8) OR MGA_CRTCExt_PrioRequestControl;0x5006
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    out       dx, ax
    mov       cx, 0078h
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x586b
    call      ConfigureAndSelectPLLSetsAB;Offset 0x5e5e
    push      bx
    mov       al, 40h
    call      SetupInterruptsTextModeAndBDA;Offset 0x15d
    pop       bx
    test      byte ptr cs:[Data0x7554], 10h;Offset 0x7554
    je        Label0x11c                ;Offset 0x11c
    mov       cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x578f
    and       cl, NOT MGA_GENIOCTRL_DDC0Data;0feh
    mov       ch, cl
    mov       cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x5774
    mov       cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call      ReadIndexedRegister       ;Offset 0x578f
    or        cl, MGA_GENIOCTRL_DDC0Data;0x1
    mov       ch, cl
    mov       cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call      WriteIndexedRegister      ;Offset 0x5774
Label0x11c:                             ;Offset 0x11c
    call      SetMemoryTimingsAndRefresh;Offset 0x5987
    call      CacheMemorySize           ;Offset 0x5463
    xor       cx, cx
    call      PowerUpPLLsLUTsAndConfigure;Offset 0x586b
    cld
    mov       dx, VGA_CRTControllerIndexD;0x3d4
    call      Func0x2717                ;Offset 0x2717
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       di, 2000h
    mov       cx, 0c000h
    xor       ax, ax
    rep stosb                           ;clear off-screen video memory
    mov       es, ax
    call      Func0x271d                ;Offset 0x271d
    xor       ax, ax
    xor       si, si
    mov       cx, 7fffh                 ;Rom size - 1
ReadROMByteLoop:                        ;Offset 0x149
    add       al, byte ptr cs:[si]      ;Recalculate the check byte at end of ROM
    inc       si
    loop      ReadROMByteLoop           ;Offset 0x149
    neg       al
    mov       byte ptr cs:[si], al      ;Store new check byte
    mov       ax, BDA_DM_80x25_16_Color_Text;0x3 Set Text Mode
    int       10h
    pop       bp
    xor       ax, ax
    retf                                ;All set up. Return control to system.

SetupInterruptsTextModeAndBDA PROC NEAR ;Offset 0x15d
    cli
    mov       word ptr ds:[INT_42_HandlerOfs], SYS_INT_10h_HandlerOfs;Offset 0x108 Offset 0xf065
    mov       word ptr ds:[INT_42_HandlerSeg], SYS_INT_10h_HandlerSeg;Offset 0x10a Segment 0xf000
    mov       word ptr ds:[INT_10_HandlerOfs], offset Int10Handler;Offset 0x40 Offset 0xc60
    mov       word ptr ds:[INT_10_HandlerSeg], cs;Offset 0x42
    mov       word ptr ds:[INT_6D_HandlerOfs], offset Int6DHandler;Offset 0x1b4 Offset 0xc70
    mov       word ptr ds:[INT_6D_HandlerSeg], cs;Offset 0x1b6
    sti
    mov       word ptr ds:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x3560
    mov       word ptr ds:[INT_1F_HandlerSeg], cs;Offset 0x7e
    mov       word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3160
    mov       word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    mov       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0x487 0x60
    mov       ch, BDA_VDDA_VGA OR BDA_VDDA_LineMode400;0x11
    test      al, 40h
    jne       IsColor                   ;Offset 0x1a1
    or        ch, BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;0x6
IsColor:                                ;Offset 0x1a1
    lea       bx, [VideoOverrideTable1] ;Offset 0x3e6
    call      SetupVideoParameterControlBlock;Offset 0x1b9
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], ch;Offset 0x489
    call      SetCardAddressOrFallbackToSystemAdapter;Offset 0x1d6
    call      SetupEGAFeatureBitSwitches;Offset 0x23a
    call      SetTextMode               ;Offset 0x265
    call      SetDisplayCombinationCode ;Offset 0x2b2
    ret
SetupInterruptsTextModeAndBDA ENDP

SetupVideoParameterControlBlock PROC NEAR;Offset 0x1b9
    mov       ax, cs
    cmp       ax, word ptr cs:[VideoOverrideTable2.VideoParameterTable.sgm];Offset 0x41e
    jne       IsShadowed                ;Offset 0x1c6
    ;add       bx, (offset VideoOverrideTable2) - (offset VideoOverrideTable1);0x36
    DB 081h, 0C3h, 036h, 000h           ;!!!!!!!!!!!!!! Assembles to different variant.
IsShadowed:                             ;Offset 0x1c6
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], bx;Offset 0x4a8
    mov       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], ax;Offset 0x4aa
    ret
SetupVideoParameterControlBlock ENDP

;Offset 0x1ce
InitMDA                 DB VGA_Misc_IOAddressSel_MDA, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Color, BDA_DM_80x25_16_Color_Text;0x0, 0xD4, 0x20, 0x03

;Offset 0x1d2
InitCGA                 DB VGA_Misc_IOAddressSel_CGA, VGA_CRTControllerIndexB_lowbyte, BDA_DH_80x25Monochrome, BDA_DM_80x25_Monochrome_Text;0x1, 0xB4, 0x30, 0x07

SetCardAddressOrFallbackToSystemAdapter PROC NEAR;Offset 0x1d6
    lea       si, [InitMDA]             ;Offset 0x1ce
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    call      SetCardMode               ;Offset 0x1f1   Attempt to set card to Monochrome
    jae       Success                   ;Offset 0x1f0
    lea       si, [InitCGA]             ;Offset 0x1d2
    and       byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487 0xfd
    call      SetCardMode               ;Offset 0x1f1   Set card to Color
Success:                                ;Offset 0x1f0
    ret
SetCardAddressOrFallbackToSystemAdapter ENDP

SetCardMode PROC NEAR                   ;Offset 0x1f1
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
    call      TestRegister              ;Offset 0x22a - If non-selected CRTC register is still writeable, it's a VGA card
    jne       IsVGA                     ;Offset 0x229
    mov       ah, 1ah
    call      TestRegister              ;Offset 0x22a - Make sure
    jne       IsVGA                     ;Offset 0x229
    lodsb     byte ptr cs:[si]          ;Card is not VGA.
    and       byte ptr ds:[BDA_DetectedHardware], NOT BDA_DH_InitialVideoModeMask;Offset 0x410 0xcf
    or        byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    lodsb     byte ptr cs:[si]          ;Load fallback mode
    cbw                                 ;zero out ah
    int       42h                       ;Call System interrupt handler to set video mode.
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
IsVGA:                                  ;Offset 0x229
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
TestRegister PROC NEAR                  ;Offset 0x22a
    in        al, dx                    ;Read register
    mov       bl, al                    ;Store result in bl
    mov       al, ah                    ;Move test value to al
    out       dx, al                    ;Write test value to register
    jmp       Delay                     ;Offset 0x232   !!!!!!Speed sensitive!
Delay:                                  ;Offset 0x232
    in        al, dx                    ;Read back test value
    ;cmp       al, ah                    ;Compare read test value to original test value
    DB 03Ah, 0C4h                       ;!!!!!! Misassembled
    mov       al, bl                    ;Move original register value to al
    out       dx, al                    ;Restore original value back to register
    stc                                 ;Set carry flag
    ret
TestRegister ENDP

SetupEGAFeatureBitSwitches PROC NEAR    ;Offset 0x23a
    mov       al, VGA_FC_Bit0           ;0x1
    call      ReadFeatureInputBits      ;Offset 0x256
    shr       al, 01h
    mov       ah, al
    mov       al, VGA_FC_Bit1           ;0x2
    call      ReadFeatureInputBits      ;Offset 0x256
    shl       al, 01h
    or        al, ah
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], NOT BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], al;Offset 0x488
    ret
SetupEGAFeatureBitSwitches ENDP

ReadFeatureInputBits PROC NEAR          ;Offset 0x256
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a - VGA_FeatureControlW
    out       dx, al
    mov       dx, VGA_InputStatus0      ;Port 0x3c2
    in        al, dx
    and       al, VGA_IS_FeatureInputs01;0x60
    ret
ReadFeatureInputBits ENDP

SetTextMode PROC NEAR                   ;Offset 0x265
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    xor       al, al                    ;zero al
    out       dx, al                    ;select color 0
    inc       dx                        ;Port 0x3c9
    mov       cx, 300h                  ;768 values
LoopColors:                             ;Offset 0x26f
    out       dx, al                    ;Zero all colors
    loop      LoopColors                ;Offset 0x26f
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    cmp       al, BDA_VDDA_MonochromeMonitor OR BDA_VDDA_VGA;0x5
    mov       bl, al                    ;store modified vdda
    je        SetMonochrome             ;Offset 0x289
    mov       ax, (BDA_DH_80x25Color SHL 8) OR BDA_DM_80x25_16_Color_Text;0x2003
    mov       bh, BDA_EFBS_MDAHiResEnhanced_2;0x9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        SetColor                  ;Offset 0x293
SetMonochrome:                          ;Offset 0x289
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    mov       ax, (BDA_DH_80x25Monochrome SHL 8) OR BDA_DM_80x25_Monochrome_Text;0x3007
    mov       bh, BDA_EFBS_CGAMono80x25_2;0xb
SetColor:                               ;Offset 0x293
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

SetDisplayCombinationCode PROC NEAR     ;Offset 0x2b2
    mov   ax, BDA_DCC_CVGA SHL 8        ;0x800
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsColor                       ;Offset 0x2be
    mov   ah, BDA_DCC_MVGA              ;0x7
IsColor:                                ;Offset 0x2be
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    jne   IsVGA                         ;Offset 0x2d5
    inc   ax                            ;BDA_DCC_MDPA
    test  byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je    IsVGA                         ;Offset 0x2d5
    inc   ax                            ;BDA_DCC_CGA
    call  CheckRegistersMemoryMapped    ;Offset 0x2db
    jne   IsVGA                         ;Offset 0x2d5
    mov   al, BDA_DCC_PGC               ;0x6
IsVGA:                                  ;Offset 0x2d5
    mov   bx, ax
    call  SetDisplayCombinationCodeIndex;Offset 0x2b54
    ret
SetDisplayCombinationCode ENDP

;
;input:
;   -
;output
;   zero flag = 0 = failed, 1 = success
;destroys:
;   dx, si
CheckRegistersMemoryMapped PROC NEAR    ;Offset 0x2db
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

;Offset 0x2f3
StartupSplash           DB 00Dh, 00Ah
                        DB 'MATROX POWER GRAPHICS ACCELERATOR', 00Dh, 00Ah
                        DB 'MGA Series', 00Dh, 00Ah
                        DB 'VGA/VBE BIOS, Version V1.6 ', 00Dh, 00Ah
                        DB 'Copyright (C) 1998, Matrox Graphics Inc.', 00Dh, 00Ah
                        DB 'Copyright (C) LSI Logic Corporation 1990-1991', 00Dh, 00Ah
                        DB 00Dh, 00Ah, 000h

;Offset 0x39d
Data0x39d               DB 000h

;Unreferenced code.
PrintStartupSplash PROC NEAR            ;Offset 0x39e
    test  byte ptr cs:[Data0x39d], 01h  ;Offset 0x39d
    je    Return                        ;Offset 0x3c3
    cmp   word ptr ds:[BDA_SoftResetFlag], 1234h;Offset 0x472
    je    Return                        ;Offset 0x3c3
    lea   si, [StartupSplash]           ;Offset 0x2f3
NextCharacter:                          ;Offset 0x3b2
    lodsb byte ptr cs:[si]
    or    al, al
    je    EndOfString                   ;Offset 0x3be
    mov   ah, 0eh                       ;Teletype Output
    int   10h
    jmp   NextCharacter                 ;Offset 0x3b2
EndOfString:                            ;Offset 0x3be
    mov   al, 0fah
    call  Sleep2                        ;Offset 0x2c24
Return:                                 ;Offset 0x3c3
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
MGAReadCRTCExtensionRegister PROC NEAR  ;Offset 0x3c4
    push  dx
    push  bx
    mov   bl, al
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, al
    inc   dx
    in    al, dx
    pushf
    cmp   bl, 07h                       ;MGA_CRTCExt_RequesterControl doesn't exist on the G100?
    jne   NotRequesterControl           ;Offset 0x3d6
    or    al, 0fh
NotRequesterControl:                    ;Offset 0x3d6
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
MGAWriteCRTCExtensionRegister PROC NEAR ;Offset 0x3da
    push  dx
    xchg  al, ah
    mov   dx, MGA_CRTCExtensionIndex    ;Port 0x3de
    out   dx, ax
    xchg  al, ah
    pop   dx
    ret
MGAWriteCRTCExtensionRegister ENDP

    nop                                 ;hello

;Offset 0x3e6
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

;Offset 0x476
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

;Offset 0x486
Data0x486               DB BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2c
                        DB BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x28
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2d
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2a
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x2e
                        DB BDA_CMCRV_Mode45Graphics OR BDA_CMCRV_Monochrome OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_GraphicsOperation;0x1e
                        DB BDA_CMCRV_Mode23Text OR BDA_CMCRV_VideoEnabled OR BDA_CMCRV_Blinking;0x29

;Offset 0x48e
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

;The assembler doesn't like all that data on one line, so I am just overlapping these to make struct accesses work.
;If anyone knows a cleaner way, please let me know
VideoParameters VideoParametersTable 29 dup(<>);Offset 0x4c0

ORG offset VideoParameters

;Offset 0x4c0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x500
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 00800h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x540
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x580
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 01000h
                        DD 002000301h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x5c0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x600
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 002000309h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0A2h, 0FFh
                        DB 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 003h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

;Offset 0x640
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000101h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0C2h, 0FFh
                        DB 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 001h, 000h, 001h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

;Offset 0x680
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 003000300h
                        DB 0A6h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0x6c0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 07D00h
                        DD 006000F21h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x700
                        DB 000h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 000000000h
                        DB 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x740
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 003000000h
                        DB 023h
                        DB 037h, 027h, 02Dh, 037h, 031h, 015h, 004h, 011h, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0E1h, 024h, 0C7h, 014h, 008h, 0E0h, 0F0h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x780
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 062h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x7c0
                        DB 050h
                        DB 000h
                        DB 000h
                        DW 00000h
                        DD 006000F29h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 00Fh, 000h, 000h, 008h, 005h, 00Fh, 0FFh

;Offset 0x800
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 006000F09h
                        DB 063h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x840
                        DB 050h
                        DB 018h
                        DB 008h
                        DW 04000h
                        DD 006000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h, 0B9h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x880
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A2h
                        DB 060h, 04Fh, 056h, 01Ah, 050h, 0E0h, 070h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Eh, 05Dh, 014h, 000h, 05Eh, 06Eh, 08Bh, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x8c0
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 000000F05h
                        DB 0A7h
                        DB 05Bh, 04Fh, 053h, 017h, 050h, 0BAh, 06Ch, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 05Eh, 02Bh, 05Dh, 014h, 00Fh, 05Fh, 00Ah, 08Bh, 0FFh
                        DB 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 000h, 001h, 000h, 000h, 004h, 007h, 000h, 000h, 001h, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 007h, 00Fh, 0FFh

;Offset 0x900
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A2h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 018h, 000h, 000h, 00Bh, 000h, 005h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh

;Offset 0x940
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 08000h
                        DD 006000F01h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h, 0BAh, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x980
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x9c0
                        DB 028h
                        DB 018h
                        DB 00Eh
                        DW 00800h
                        DD 002000309h
                        DB 0A3h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa00
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa40
                        DB 050h
                        DB 018h
                        DB 00Eh
                        DW 01000h
                        DD 002000301h
                        DB 0A3h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h, 0BAh, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xa80
                        DB 028h
                        DB 018h
                        DB 010h
                        DW 00800h
                        DD 002000308h
                        DB 067h
                        DB 02Dh, 027h, 028h, 090h, 02Bh, 0A0h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xac0
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 067h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0xb00
                        DB 050h
                        DB 018h
                        DB 010h
                        DW 01000h
                        DD 002000300h
                        DB 066h
                        DB 05Fh, 04Fh, 050h, 082h, 055h, 081h, 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h
                        DB 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

;Offset 0xb40
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0C3h, 0FFh
                        DB 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh

;Offset 0xb80
                        DB 050h
                        DB 01Dh
                        DB 010h
                        DW 0A000h
                        DD 006000F01h
                        DB 0E3h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h, 004h, 0E3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0xbc0
                        DB 028h
                        DB 018h
                        DB 008h
                        DW 02000h
                        DD 00E000F01h
                        DB 063h
                        DB 05Fh, 04Fh, 050h, 082h, 054h, 080h, 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h, 0B9h, 0A3h, 0FFh
                        DB 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0xc00
Int10CallTable          DW SetVideoMode;Offset 0x0d10                                       ;  0
                        DW SetTextModeCursorShape;Offset 0x10ab                             ;  1
                        DW SetCursorPosition;Offset 0x1127                                  ;  2
                        DW GetCursorPositionAndSize;Offset 0x115b                           ;  3
                        DW ReadLightPenPosition_SystemForward;Offset 0x116b                 ;  4
                        DW SelectActiveDisplayPage;Offset 0x1178                            ;  5
                        DW ScrollUpWindow;Offset 0x11ae                                     ;  6
                        DW ScrollDownWindow;Offset 0x13d3                                   ;  7
                        DW ReadCharacterAndAttributeAtCursorPosition;Offset 0x1610          ;  8
                        DW WriteCharacterAndAttributeAtCursorPosition;Offset 0x17dd         ;  9
                        DW WriteCharacterOnlyAtCursorPosition;Offset 0x1ae6                 ;  a
                        DW SetBorderColorOrPalette;Offset 0x1b47                            ;  b
                        DW WriteGraphicsPixel;Offset 0x1bf0                                 ;  c
                        DW ReadGraphicsPixel;Offset 0x1ccd                                  ;  d
                        DW TeletypeOutput;Offset 0x1d83                                     ;  e
                        DW GetCurrentVideoMode;Offset 0x1e0a                                ;  f
                        DW RAMDACFunctions;Offset 0x1e1c                                    ; 10
                        DW TextFunctions;Offset 0x200b                                      ; 11
                        DW AlternateFunctions;Offset 0x210f                                 ; 12
                        DW WriteString  ;Offset 0x229c                                      ; 13
                        DW NotImplemented;Offset 0x2328                                     ; 14
                        DW NotImplemented;Offset 0x2328                                     ; 15
                        DW NotImplemented;Offset 0x2328                                     ; 16
                        DW NotImplemented;Offset 0x2328                                     ; 17
                        DW NotImplemented;Offset 0x2328                                     ; 18
                        DW NotImplemented;Offset 0x2328                                     ; 19
                        DW DisplayCombinationCodeFunctions;Offset 0x2329                    ; 1a
                        DW FunctionalityAndStateInfo;Offset 0x2341                          ; 1b
                        DW SaveRestoreVideoState;Offset 0x244e                              ; 1c

;Offset 0xc3a
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 070h, 00Ch, 000h, 000h, 0FFh, 0FFh, 052h, 004h, 076h, 004h, 026h, 024h, 03Ah, 024h, 038h, 02Ah
                        DB 0B4h, 02Ah, 02Eh, 02Dh, 000h, 000h

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx

Int10Handler:                           ;Offset 0xc60
    cmp  ax, 4f10h
    jne  NotPowerManagement             ;Offset 0xc6a
    sti
    cld
    jmp  VESAPowerManagement            ;Offset 0x4ac2
NotPowerManagement:                     ;Offset 0xc6a
    int  6dh
    iret

    xchg bx, bx                         ;hello
    nop  

Int6DHandler:                           ;Offset 0xc70
    sti  
    cld  
    cmp  ax, 4f10h
    jne  NotVESAPowerManagement         ;Offset 0xc7a
    jmp  VESAPowerManagement            ;Offset 0x4ac2
NotVESAPowerManagement:                 ;Offset 0xc7a
    cmp  ax, 4f15h
    jne  NotVESADDC                     ;Offset 0xc82
    jmp  VESADDC                        ;Offset 0x4b9a
NotVESADDC:                             ;Offset 0xc82
    cmp  ah, 1ch
    ja   Label0xcda                     ;Offset 0xcda
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
    jl   Label0xcd1                     ;Offset 0xcd1
    cmp  ah, 0eh
    je   Label0xcac                     ;Offset 0xcac
    cmp  ah, 06h
    jl   Label0xcd1                     ;Offset 0xcd1
    cmp  ah, 0ah
    jg   Label0xcd1                     ;Offset 0xcd1
Label0xcac:                             ;Offset 0xcac
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
    call word ptr cs:[si + Int10CallTable];Offset 0xc00
    pop  di
    push ax
    push dx
    mov  ax, di
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    out  dx, ax
    pop  dx
    pop  ax
    jmp  Label0xcd6                     ;Offset 0xcd6
Label0xcd1:                             ;Offset 0xcd1
    call word ptr cs:[si + Int10CallTable];Offset 0xc00
Label0xcd6:                             ;Offset 0xcd6
    pop  di
    pop  si
    pop  ds
    iret
Label0xcda:                             ;Offset 0xcda
    cmp  ah, 4fh
    jne  Label0xce2                     ;Offset 0xce2
    jmp  VESAHandler                    ;Offset 0x6dca
Label0xce2:                             ;Offset 0xce2
    int  42h
    iret

Func0xce5 PROC NEAR                     ;Offset 0xce5
    push ax
    push bx
    mov  ax, cs
    mov  bx, ds
    cmp  ax, bx
    pop  bx
    pop  ax
    jne  Label0xd0e                     ;Offset 0xd0e
    cmp  cl, 0eh
    jne  Label0xd0e                     ;Offset 0xd0e
    cmp  al, 67h
    je   Label0xd0c                     ;Offset 0xd0c
    cmp  al, 6ah
    je   Label0xd0c                     ;Offset 0xd0c
    cmp  al, 79h
    je   Label0xd0c                     ;Offset 0xd0c
    cmp  al, 71h
    je   Label0xd0c                     ;Offset 0xd0c
    cmp  al, 70h
    je   Label0xd0c                     ;Offset 0xd0c
    cmp  al, 98h
Label0xd0c:                             ;Offset 0xd0c
    clc
    ret
Label0xd0e:                             ;Offset 0xd0e
    stc
    ret
Func0xce5 ENDP

SetVideoMode PROC NEAR                  ;Offset 0x0d10
    push ax
    mov  ah, al
    mov  al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and  al, BDA_VDDA_Reserved          ;0x20
    je   Label0xd1d                     ;Offset 0xd1d
    or   ah, BDA_DM_DONT_CLEAR_SCREEN   ;0x80
Label0xd1d:                             ;Offset 0xd1d
    mov  al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    shl  ax, 01h
    xchg al, ah
    rcr  ax, 01h
    cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
    ja   Label0xd8a                     ;Offset 0xd8a
    push ax
    cmp  al, BDA_DM_80x25_Monochrome_Text;0x7
    je   Label0xd3c                     ;Offset 0xd3c
    cmp  al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je   Label0xd3c                     ;Offset 0xd3c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Color          ;0x20
    jmp  Label0xd43                     ;Offset 0xd43
Label0xd3c:                             ;Offset 0xd3c
    mov  al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and  al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or   al, BDA_DH_80x25Monochrome     ;0x30
Label0xd43:                             ;Offset 0xd43
    mov  byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    pop  ax
    call Func0xec3                      ;Offset 0xec3
    mov  word ptr ds:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x3160
    mov  word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    and  ah, 0f3h
    test byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je   Label0xd60                     ;Offset 0xd60
    jp   Func0xdda                      ;Offset 0xdda
Label0xd60:                             ;Offset 0xd60
    test ah, 02h
    jne  Label0xd74                     ;Offset 0xd74
    mov  si, 3d4h
    cmp  al, 07h
    je   Label0xd70                     ;Offset 0xd70
    cmp  al, 0fh
    jne  Func0xdef                      ;Offset 0xdef
Label0xd70:                             ;Offset 0xd70
    mov  al, 03h
    jmp  Func0xdec                      ;Offset 0xdec
Label0xd74:                             ;Offset 0xd74
    cmp  al, 02h
    jb   Label0xd7f                     ;Offset 0xd7f
    cmp  al, 03h
    ja   Label0xd7f                     ;Offset 0xd7f
    or   ah, 04h
Label0xd7f:                             ;Offset 0xd7f
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485 0x8
    call Func0xdca                      ;Offset 0xdca
Label0xd88:                             ;Offset 0xd88
    pop  ax
    ret
Label0xd8a:                             ;Offset 0xd8a
    cmp  al, 6ah
    je   Label0xdad                     ;Offset 0xdad
    mov  ah, al
    and  ah, 0c0h
    or   ah, 01h
    and  al, 3fh
    cmp  al, 20h
    jb   Label0xd88                     ;Offset 0xd88
    cmp  al, 3fh
    ja   Label0xd88                     ;Offset 0xd88
    sub  al, 20h
    push bx
    mov  bx, ax
    mov  ax, 4f02h
    int  10h
    pop  bx
    pop  ax
    ret
Label0xdad:                             ;Offset 0xdad
    pop  ax
    push bx
    mov  ax, 4f02h
    mov  bx, 0102h
    int  10h
    pop  bx
    ret
SetVideoMode ENDP

Func0xdb9 PROC NEAR                     ;Offset 0xdb9
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 0xe
    call Func0xdca                      ;Offset 0xdca
    mov  word ptr ds:[BDA_CursorEndStartScanLine], 0b0ch;Offset 0x460
    pop  ax
    ret
Func0xdb9 ENDP

Func0xdca PROC NEAR                     ;Offset 0xdca
    mov  byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484 0x18
    or   ah, BDA_VMO_Inactive           ;0x8
    mov  byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    cbw
    int  42h
    ret
Func0xdca ENDP

;
;inputs:
;   al = display mode
;   ah = video mode options
;   si = video base io port
;
;
Func0xdda PROC NEAR                     ;Offset 0xdda
    test      ah, BDA_VMO_Monochrome    ;0x2
    je        Func0xdb9                 ;Offset 0xdb9
    mov       si, VGA_CRTControllerIndexB;Port 0x3b4
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Func0xdef                 ;Offset 0xdef
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Func0xdef                 ;Offset 0xdef
    mov       al, BDA_DM_80x25_Monochrome_Text;0x7
Func0xdda ENDP
;continue!
Func0xdec PROC NEAR                     ;Offset 0xdec
    and       ah, NOT BDA_VMO_DontClearDisplay;0x7f
Func0xdec ENDP
;continue!
Func0xdef PROC NEAR                     ;Offset 0xdef
    push      bx
    push      cx
    push      dx
    push      bp
    push      es
    push      ax
    call      TurnScreenOff             ;Offset 0x2b3d
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
    jne       Label0xe18                ;Offset 0xe18
    mov       ah, 00h
Label0xe18:                             ;Offset 0xe18
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    pop       dx
    pop       ax
    call      Func0xec0                 ;Offset 0xec0
    xor       ax, ax
    mov       es, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    lea       di, ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       cx, 0008h
    rep stosw
    call      Func0x2a18                ;Offset 0x2a18
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
    call      Func0x286d                ;Offset 0x286d
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
    call      Func0xf51                 ;Offset 0xf51
    call      ClearScreen               ;Offset 0x26d9
    lea       si, [Func0xf70]           ;Offset 0xf70
    call      Func0x2ad6                ;Offset 0x2ad6
    je        Label0xe83                ;Offset 0xe83
    lea       si, [Func0xf9b]           ;Offset 0xf9b
Label0xe83:                             ;Offset 0xe83
    call      si
    call      Func0xfbe                 ;Offset 0xfbe
    call      Func0x2afd                ;Offset 0x2afd
    pop       ax
    call      TurnScreenOn              ;Offset 0x2b39
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0xead                ;Offset 0xead
    mov       ah, al
    lea       bx, [Data0x486]           ;Offset 0x486
    xlatb     cs:[bx]
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    mov       al, 30h
    cmp       ah, BDA_DM_640x200_BW_Graphics;0x6
    jne       Label0xeaa                ;Offset 0xeaa
    mov       al, 3fh
Label0xeaa:                             ;Offset 0xeaa
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
Label0xead:                             ;Offset 0xead
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, BDA_VDDA_Reserved     ;0x20
    je        Label0xeb9                ;Offset 0xeb9
    and       byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_Reserved;Offset 0x489 0xdf
Label0xeb9:                             ;Offset 0xeb9
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0xdef ENDP

Func0xec0 PROC NEAR                     ;Offset 0xec0
    call      Func0x606f                ;Offset 0x606f
Func0xec0 ENDP
;continue!
Func0xec3 PROC NEAR                     ;Offset 0xec3
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    je        Label0xf46                ;Offset 0xf46
    cmp       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je        Label0xf46                ;Offset 0xf46
    push      bx
    push      dx
    lea       si, [Data0xf47]           ;Offset 0xf47
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0xee2                ;Offset 0xee2
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0xee2                ;Offset 0xee2
    ;add       si, 0005h
    DB 81h, 0C6h, 05h, 00h              ;!!!!!!!!!!!!! misassembly
Label0xee2:                             ;Offset 0xee2
    xchg      ax, bx
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       al, byte ptr cs:[si + 01h]
    je        Label0xf43                ;Offset 0xf43
    mov       ah, bl
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    mov       bl, BDA_DM_80x25_Monochrome_Text;0x7
    cmp       al, byte ptr cs:[si + 02h]
    je        Label0xf43                ;Offset 0xf43
    mov       bl, BDA_DM_80x25_16_Color_Text;0x3
    cmp       al, byte ptr cs:[si + 03h]
    js        Label0xf43                ;Offset 0xf43
    mov       bl, ah
    and       bh, 0fdh
    or        bh, byte ptr cs:[si + 04h]
    mov       al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and       al, BDA_EFBS_AdapterTypeMask;0xf
    sub       al, BDA_EFBS_MDAColor40x25_2;0x6
    cmp       al, BDA_EFBS_CGAMono80x25 ;0x5
    ja        Label0xf43                ;Offset 0xf43
    sub       al, BDA_EFBS_MDAHiResEnhanced;0x3
    cbw
    mov       al, BDA_EFBS_CGAMono80x25_2;0xb
    jbe       Label0xf20                ;Offset 0xf20
    mov       al, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0xf20:                             ;Offset 0xf20
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
Label0xf43:                             ;Offset 0xf43
    xchg      ax, bx
    pop       dx
    pop       bx
Label0xf46:                             ;Offset 0xf46
    ret
Func0xec3 ENDP

;Offset 0xf47
Data0xf47               DB 080h, VGA_CRTControllerIndexB_lowbyte, 0FFh, BDA_DH_80x25Monochrome, 002h;0x80 0xB4 0xFF 0x30 0x02

;Offset 0xf4c
Data0xf4c               DB 001h, VGA_CRTControllerIndexD_lowbyte, BDA_DH_80x25Monochrome, 000h, 000h;0x01 0xD4 0x30 0x00 0x00

Func0xf51 PROC NEAR                     ;Offset 0xf51
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   PaletteLoadingDisabled        ;Offset 0xf6f
    push  si
    xor   ax, ax
    mov   cx, 0010h
    call  Func0x28ed                    ;Offset 0x28ed
    inc   ax
    inc   si
    mov   cl, 01h
    call  Func0x28ed                    ;Offset 0x28ed
    pop   si
    call  Func0x2933                    ;Offset 0x2933
    call  Func0x2948                    ;Offset 0x2948
PaletteLoadingDisabled:                 ;Offset 0xf6f
    ret
Func0xf51 ENDP

Func0xf70 PROC NEAR                     ;Offset 0xf70
    call  Func0x2717                    ;Offset 0x2717
    call  GetNumberOfActiveScanlines    ;Offset 0x2a88
    dec   al
    mov   ax, 0002h
    js    Label0xf8c                    ;Offset 0xf8c
    mov   al, 03h
    jne   Label0xf8a                    ;Offset 0xf8a
    mov   al, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne   Label0xf8c                    ;Offset 0xf8c
Label0xf8a:                             ;Offset 0xf8a
    mov   ah, 80h
Label0xf8c:                             ;Offset 0xf8c
    mov   bl, ah
    call  Func0x2758                    ;Offset 0x2758
    call  Func0x1039                    ;Offset 0x1039
    call  Func0x106c                    ;Offset 0x106c
    call  Func0x271d                    ;Offset 0x271d
    ret
Func0xf70 ENDP

Func0xf9b PROC NEAR                     ;Offset 0xf9b
    call  GetNumberOfActiveScanlines    ;Offset 0x2a88
    dec   al
    js    Label0xfb5                    ;Offset 0xfb5
    lea   ax, [Font8x16]                ;Offset 0x3960
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    mov   ax, cs
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
    jne   Label0xfb5                    ;Offset 0xfb5
    mov   ax, 0bd70h
    int   6dh
Label0xfb5:                             ;Offset 0xfb5
    xor   ax, ax
    mov   word ptr ds:[BDA_CursorEndScanLine], ax;Offset 0x460
    call  Func0x1015                    ;Offset 0x1015
    ret
Func0xf9b ENDP

Func0xfbe PROC NEAR                     ;Offset 0xfbe
    mov   al, BDA_VPCB_PaletterProfileTable;0xa
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    Label0x1014                   ;Offset 0x1014
    mov   al, 14h
    call  Func0x2ae9                    ;Offset 0x2ae9
    jne   Label0x1014                   ;Offset 0x1014
    lodsb byte ptr es:[si]
    or    al, al
    je    Label0xfe2                    ;Offset 0xfe2
    mov   ax, 1420h
    js    Label0xfda                    ;Offset 0xfda
    mov   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
Label0xfda:                             ;Offset 0xfda
    dec   ax
    xchg  al, ah
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    out   dx, ax
Label0xfe2:                             ;Offset 0xfe2
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne   Label0x1014                   ;Offset 0x1014
    push  es
    push  si
    add   si, 0003h
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0xffd                    ;Offset 0xffd
    mov   ah, 10h
    call  Func0x28ef                    ;Offset 0x28ef
Label0xffd:                             ;Offset 0xffd
    pop   si
    pop   es
    add   si, 000bh
    lodsw word ptr es:[si]
    xchg  ax, cx
    lodsw word ptr es:[si]
    les   si, es:[si]
    jcxz  Label0x1014                   ;Offset 0x1014
    mov   dx, VGA_DACWriteIndex         ;Port 0x3c8
    out   dx, al
    inc   dx
    call  Func0x29c7                    ;Offset 0x29c7
Label0x1014:                            ;Offset 0x1014
    ret
Func0xfbe ENDP

Func0x1015 PROC NEAR                    ;Offset 0x1015
    mov   al, BDA_VPCB_GrahicsCharSetOverride;0xc
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    Label0x1038                   ;Offset 0x1038
    mov   al, 07h
    call  Func0x2ae9                    ;Offset 0x2ae9
    jne   Label0x1038                   ;Offset 0x1038
    lodsb byte ptr es:[si]
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    lodsw word ptr es:[si]
    mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerOfs], ax;Offset 0x10c
    lodsw word ptr es:[si]
    mov   word ptr ds:[INT_43_HandlerSeg], ax;Offset 0x10e
Label0x1038:                            ;Offset 0x1038
    ret
Func0x1015 ENDP

Func0x1039 PROC NEAR                    ;Offset 0x1039
    mov   al, BDA_VPCB_AlphaNumCharSetOverride;0x8
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    Label0x106b                   ;Offset 0x106b
    mov   al, 0bh
    call  Func0x2ae9                    ;Offset 0x2ae9
    jne   Label0x106b                   ;Offset 0x106b
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
    call  Func0x277b                    ;Offset 0x277b
    call  Func0x27fa                    ;Offset 0x27fa
    pop   ax
    cmp   al, 0ffh
    je    Label0x106b                   ;Offset 0x106b
    dec   ax
    mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x106b:                            ;Offset 0x106b
    ret
Func0x1039 ENDP

Func0x106c PROC NEAR                    ;Offset 0x106c
    mov   al, BDA_VPCB_SecondaryCharSetOverride;0x6
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    Label0x10aa                   ;Offset 0x10aa
    mov   al, 07h
    call  Func0x2ae9                    ;Offset 0x2ae9
    jne   Label0x10aa                   ;Offset 0x10aa
    lodsw word ptr es:[si]
    cmp   al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    jne   Label0x10aa                   ;Offset 0x10aa
    push  ax
    xchg  al, ah
    and   al, 7fh
    xchg  ax, bx
    inc   si
    les   bp, es:[si]
    xor   dx, dx
    mov   cx, 0100h
    call  Func0x277b                    ;Offset 0x277b
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
Label0x10aa:                            ;Offset 0x10aa
    ret
Func0x106c ENDP

SetTextModeCursorShape PROC NEAR        ;Offset 0x10ab
    push      dx
    mov       ax, cx
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x10d4               ;Offset 0x10d4
    and       ah, 60h
    cmp       ah, 20h
    mov       ax, 1e00h
    je        Label0x10d4               ;Offset 0x10d4
    mov       ax, cx
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_CursorEmulationEnabled;Offset 0x487 0x8
    jne       Label0x10d4               ;Offset 0x10d4
    call      Func0x10d9                ;Offset 0x10d9
    call      SetCursorEndStart         ;Offset 0x26ca
    pop       dx
    ret
Label0x10d4:                            ;Offset 0x10d4
    call      SetCursorEndStart         ;Offset 0x26ca
    pop       dx
    ret
SetTextModeCursorShape ENDP

Func0x10d9 PROC NEAR                    ;Offset 0x10d9
    call      Func0x2ad6                ;Offset 0x2ad6
    jne       Label0x1126               ;Offset 0x1126
    test      ax, 0e0e0h
    jne       Label0x1126               ;Offset 0x1126
    mov       dl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       dx
    ;cmp       ah, al
    DB 3Ah, 0E0h                        ;!!!!!!!! Misassembly
    ja        Label0x111f               ;Offset 0x111f
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    ja        Label0x10fc               ;Offset 0x10fc
    ;cmp       al, dl
    DB 3Ah, 0C2h                        ;!!!!!!!! Misassembly
    ja        Label0x10fc               ;Offset 0x10fc
    je        Label0x1126               ;Offset 0x1126
    dec       dx
    ;cmp       ah, dl
    DB 3Ah, 0E2h                        ;!!!!!!!! Misassembly
    je        Label0x1126               ;Offset 0x1126
    inc       dx
Label0x10fc:                            ;Offset 0x10fc
    cmp       al, 03h
    jbe       Label0x1126               ;Offset 0x1126
    sub       al, ah
    cmp       al, 02h
    mov       al, dl
    jbe       Label0x1113               ;Offset 0x1113
    cmp       ah, 02h
    jbe       Label0x1126               ;Offset 0x1126
    inc       dx
    shr       dl, 01h
    mov       ah, dl
    ret
Label0x1113:                            ;Offset 0x1113
    add       ah, dl
    sub       ah, cl
    cmp       al, 0dh
    jb        Label0x1126               ;Offset 0x1126
    sub       ax, 0101h
    ret
Label0x111f:                            ;Offset 0x111f
    cmp       al, 00h
    je        Label0x1126               ;Offset 0x1126
    mov       dh, al
    xchg      ax, dx
Label0x1126:                            ;Offset 0x1126
    ret
Func0x10d9 ENDP

SetCursorPosition PROC NEAR             ;Offset 0x1127
    push      ax
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       word ptr [si + DBA_CursorPositionPage0], dx;Offset 0x450
    cmp       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    jne       Func0x1159                ;Offset 0x1159
    push      dx
    mov       ax, dx
    jmp       Func0x1141                ;Offset 0x1141
SetCursorPosition ENDP

Func0x113e PROC NEAR                    ;Offset 0x113e
    pop       dx
    push      ax
    push      dx
Func0x113e ENDP
;continue!
Func0x1141 PROC NEAR                    ;Offset 0x1141
    call      Func0x2682                ;Offset 0x2682
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
Func0x1141 ENDP
;continue!
Func0x1159 PROC NEAR                    ;Offset 0x1159
    pop       ax
    ret
Func0x1159 ENDP

GetCursorPositionAndSize PROC NEAR      ;Offset 0x115b
    mov       al, bh
    cbw
    mov       si, ax
    shl       si, 01h
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    mov       cx, word ptr ds:[BDA_CursorEndStartScanLine];Offset 0x460
    ret
GetCursorPositionAndSize ENDP

ReadLightPenPosition_SystemForward PROC NEAR;Offset 0x116b
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x1175               ;Offset 0x1175
    xor       ah, ah
    ret
Label0x1175:                            ;Offset 0x1175
    int       42h
    ret
ReadLightPenPosition_SystemForward ENDP

SelectActiveDisplayPage PROC NEAR       ;Offset 0x1178
    push      dx
    cbw
    mov       si, ax
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       word ptr ds:[BDA_VideoBufferOffset], ax;Offset 0x44e
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, 07h
    pop       ax
    ja        Label0x1194               ;Offset 0x1194
    shr       ax, 01h
Label0x1194:                            ;Offset 0x1194
    call      SetStartAddressVGA        ;Offset 0x119f
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Func0x113e                ;Offset 0x113e
SelectActiveDisplayPage ENDP

SetStartAddressVGA PROC NEAR            ;Offset 0x119f
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

ScrollUpWindow PROC NEAR                ;Offset 0x11ae
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
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x11e1               ;Offset 0x11e1
    ja        Label0x11d6               ;Offset 0x11d6
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x11ea               ;Offset 0x11ea
Label0x11d0:                            ;Offset 0x11d0
    jmp       Label0x1268               ;Offset 0x1268
Label0x11d3:                            ;Offset 0x11d3
    jmp       Label0x1356               ;Offset 0x1356
Label0x11d6:                            ;Offset 0x11d6
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x124b               ;Offset 0x124b
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x11d3               ;Offset 0x11d3
    jmp       Label0x12db               ;Offset 0x12db
Label0x11e1:                            ;Offset 0x11e1
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x11d0               ;Offset 0x11d0
Label0x11ea:                            ;Offset 0x11ea
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x2682                ;Offset 0x2682
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
    jne       Label0x1255               ;Offset 0x1255
Label0x121b:                            ;Offset 0x121b
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1251               ;Offset 0x1251
Label0x1223:                            ;Offset 0x1223
    mov       cx, dx
    rep movsw
    add       si, bp
    add       di, bp
    dec       ax
    jne       Label0x1223               ;Offset 0x1223
Label0x122e:                            ;Offset 0x122e
    xchg      ah, bh
    mov       al, 20h
Label0x1232:                            ;Offset 0x1232
    mov       cx, dx
    rep stosw
    add       di, bp
    dec       bx
    jne       Label0x1232               ;Offset 0x1232
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x124b               ;Offset 0x124b
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x124b:                            ;Offset 0x124b
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x1251:                            ;Offset 0x1251
    mov       bl, al
    jmp       Label0x122e               ;Offset 0x122e
Label0x1255:                            ;Offset 0x1255
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x125a:                            ;Offset 0x125a
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x125a               ;Offset 0x125a
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x121b               ;Offset 0x121b
Label0x1268:                            ;Offset 0x1268
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    shl       ah, 01h
    shl       ah, 01h
    xchg      ax, cx
    call      Func0x2690                ;Offset 0x2690
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x1282               ;Offset 0x1282
    shl       ax, 01h
    shl       cl, 01h
Label0x1282:                            ;Offset 0x1282
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
    je        Label0x12d7               ;Offset 0x12d7
Label0x12a4:                            ;Offset 0x12a4
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 1fb0h]     ;Offset 0x1fb0
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       ax
    jne       Label0x12a4               ;Offset 0x12a4
Label0x12bf:                            ;Offset 0x12bf
    xchg      al, bh
Label0x12c1:                            ;Offset 0x12c1
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 1fb0h]     ;Offset 0x1fb0
    dec       bx
    jne       Label0x12c1               ;Offset 0x12c1
    jmp       Label0x124b               ;Offset 0x124b
Label0x12d7:                            ;Offset 0x12d7
    xchg      bl, al
    jmp       Label0x12bf               ;Offset 0x12bf
Label0x12db:                            ;Offset 0x12db
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x26a0                ;Offset 0x26a0
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
    je        Label0x1351               ;Offset 0x1351
Label0x1322:                            ;Offset 0x1322
    mov       cx, dx
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x1322               ;Offset 0x1322
    pop       bx
Label0x132e:                            ;Offset 0x132e
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x133e:                            ;Offset 0x133e
    mov       cx, si
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x133e               ;Offset 0x133e
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x124b               ;Offset 0x124b
Label0x1351:                            ;Offset 0x1351
    pop       bx
    mov       bl, ch
    jmp       Label0x132e               ;Offset 0x132e
Label0x1356:                            ;Offset 0x1356
    mov       ax, dx
    sub       ax, cx
    add       ax, 0101h
    xchg      ax, cx
    call      Func0x26b2                ;Offset 0x26b2
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
    je        Label0x13cd               ;Offset 0x13cd
Label0x13a5:                            ;Offset 0x13a5
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    add       si, bp
    add       di, bp
    dec       bx
    jne       Label0x13a5               ;Offset 0x13a5
    pop       bx
Label0x13b5:                            ;Offset 0x13b5
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x13bd:                            ;Offset 0x13bd
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0x13bd               ;Offset 0x13bd
    jmp       Label0x124b               ;Offset 0x124b
Label0x13cd:                            ;Offset 0x13cd
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x13b5               ;Offset 0x13b5
ScrollUpWindow ENDP

ScrollDownWindow PROC NEAR              ;Offset 0x13d3
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
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jb        Label0x1407               ;Offset 0x1407
    ja        Label0x13fc               ;Offset 0x13fc
    mov       si, 0b000h                ;Segment 0xb000
    mov       es, si
    jmp       Label0x1410               ;Offset 0x1410
Label0x13f6:                            ;Offset 0x13f6
    jmp       Label0x1490               ;Offset 0x1490
Label0x13f9:                            ;Offset 0x13f9
    jmp       Label0x158a               ;Offset 0x158a
Label0x13fc:                            ;Offset 0x13fc
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    jb        Label0x1473               ;Offset 0x1473
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    je        Label0x13f9               ;Offset 0x13f9
    jmp       Label0x1507               ;Offset 0x1507
Label0x1407:                            ;Offset 0x1407
    mov       si, 0b800h                ;Segment 0xb800
    mov       es, si
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    ja        Label0x13f6               ;Offset 0x13f6
Label0x1410:                            ;Offset 0x1410
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    call      Func0x2682                ;Offset 0x2682
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
    jne       Label0x147d               ;Offset 0x147d
Label0x1443:                            ;Offset 0x1443
    mov       cx, es
    mov       ds, cx
    or        bl, bl
    je        Label0x1479               ;Offset 0x1479
Label0x144b:                            ;Offset 0x144b
    mov       cx, dx
    rep movsw
    sub       si, bp
    sub       di, bp
    dec       ax
    jne       Label0x144b               ;Offset 0x144b
Label0x1456:                            ;Offset 0x1456
    xchg      ah, bh
    mov       al, 20h
Label0x145a:                            ;Offset 0x145a
    mov       cx, dx
    rep stosw
    sub       di, bp
    dec       bx
    jne       Label0x145a               ;Offset 0x145a
    mov       ds, bx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je        Label0x1473               ;Offset 0x1473
    mov       al, byte ptr ds:[BDA_CRTModeControlRegValue];Offset 0x465
    mov       dx, VGA_CGAModeControl    ;Port 0x3d8
    out       dx, al
Label0x1473:                            ;Offset 0x1473
    pop       es
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x1479:                            ;Offset 0x1479
    mov       bl, al
    jmp       Label0x1456               ;Offset 0x1456
Label0x147d:                            ;Offset 0x147d
    push      ax
    push      dx
    mov       dx, VGA_InputStatus1D     ;Port 0x3da
Label0x1482:                            ;Offset 0x1482
    in        al, dx
    and       al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je        Label0x1482               ;Offset 0x1482
    mov       dl, VGA_CGAModeControl_lowbyte;Port 0x3d8
    mov       al, VGA_CGAMC_HiResText OR VGA_CGAMC_320x200Monochrome OR VGA_CGAMC_Blinking;0x25
    out       dx, al
    pop       dx
    pop       ax
    jmp       Label0x1443               ;Offset 0x1443
Label0x1490:                            ;Offset 0x1490
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    shl       dh, 01h
    shl       dh, 01h
    mov       cx, dx
    call      Func0x2690                ;Offset 0x2690
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x14ad               ;Offset 0x14ad
    shl       ax, 01h
    inc       ax
    shl       cl, 01h
Label0x14ad:                            ;Offset 0x14ad
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
    je        Label0x1503               ;Offset 0x1503
Label0x14d0:                            ;Offset 0x14d0
    mov       cx, dx
    rep movsb
    lea       si, [bp + si + 2000h]     ;Offset 0x2000
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep movsb
    lea       si, [bp + si - 2050h]     ;Offset 0x2050
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       ax
    jne       Label0x14d0               ;Offset 0x14d0
Label0x14eb:                            ;Offset 0x14eb
    xchg      al, bh
Label0x14ed:                            ;Offset 0x14ed
    mov       cx, dx
    rep stosb
    lea       di, [bp + di + 2000h]     ;Offset 0x2000
    mov       cx, dx
    rep stosb
    lea       di, [bp + di - 2050h]     ;Offset 0x2050
    dec       bx
    jne       Label0x14ed               ;Offset 0x14ed
    jmp       Label0x1473               ;Offset 0x1473
Label0x1503:                            ;Offset 0x1503
    xchg      bl, al
    jmp       Label0x14eb               ;Offset 0x14eb
Label0x1507:                            ;Offset 0x1507
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x26a0                ;Offset 0x26a0
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
    je        Label0x1585               ;Offset 0x1585
Label0x1556:                            ;Offset 0x1556
    mov       cx, dx
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x1556               ;Offset 0x1556
    pop       bx
Label0x1562:                            ;Offset 0x1562
    mov       si, dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_SplatBLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x205
    out       dx, ax
    pop       ax
    mul       bl
    mov       dx, ax
    mov       al, bh
Label0x1572:                            ;Offset 0x1572
    mov       cx, si
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x1572               ;Offset 0x1572
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    jmp       Label0x1473               ;Offset 0x1473
Label0x1585:                            ;Offset 0x1585
    pop       bx
    mov       bl, ch
    jmp       Label0x1562               ;Offset 0x1562
Label0x158a:                            ;Offset 0x158a
    mov       ax, dx
    sub       dx, cx
    add       dx, 0101h
    mov       cx, dx
    inc       ah
    call      Func0x26b2                ;Offset 0x26b2
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
    je        Label0x160a               ;Offset 0x160a
Label0x15e2:                            ;Offset 0x15e2
    mov       cx, dx
    shl       cx, 01h
    shl       cx, 01h
    rep movsb
    sub       si, bp
    sub       di, bp
    dec       bx
    jne       Label0x15e2               ;Offset 0x15e2
    pop       bx
Label0x15f2:                            ;Offset 0x15f2
    mov       si, dx
    pop       ax
    mul       bl
    xchg      ax, dx
    mov       al, bh
Label0x15fa:                            ;Offset 0x15fa
    mov       cx, si
    shl       cx, 01h
    shl       cx, 01h
    rep stosb
    sub       di, bp
    dec       dx
    jne       Label0x15fa               ;Offset 0x15fa
    jmp       Label0x1473               ;Offset 0x1473
Label0x160a:                            ;Offset 0x160a
    pop       bx
    mov       bl, ch
    xchg      ax, cx
    jmp       Label0x15f2               ;Offset 0x15f2
ScrollDownWindow ENDP

ReadCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x1610
    push       dx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x61e2               ;Offset 0x61e2
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x1624              ;Offset 0x1624
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Label0x165d              ;Offset 0x165d
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x1624:                            ;Offset 0x1624
    push       dx
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2682               ;Offset 0x2682
    add        si, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x1646              ;Offset 0x1646
    pop        ds
    lodsw
    pop        dx
    ret
Label0x1646:                            ;Offset 0x1646
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a VGA Input Status
    pop        ds
Label0x164e:                            ;Offset 0x164e
    in         al, dx
    shr        al, 01h                  ;check HSync
    jb         Label0x164e              ;Offset 0x164e
    cli
Label0x1654:                            ;Offset 0x1654
    in         al, dx
    shr        al, 01h                  ;check HSync
    jae        Label0x1654              ;Offset 0x1654
    lodsw
    sti
    pop        dx
    ret
Label0x165d:                            ;Offset 0x165d
    push       cx
    push       es
    ja         Label0x16cc              ;Offset 0x16cc
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2690               ;Offset 0x2690
    mov        si, ax
    sub        sp, 0008h
    mov        di, sp
    mov        ax, ss
    mov        es, ax
    mov        ax, 0b800h               ;Segment 0xb800
    mov        cx, 0004h
    cmp        byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov        ds, ax
    je         Label0x1690              ;Offset 0x1690
    shl        si, 01h
Label0x1683:                            ;Offset 0x1683
    call       Func0x17c1               ;Offset 0x17c1
    call       Func0x17c1               ;Offset 0x17c1
    add        si, 0050h
    loop       Label0x1683              ;Offset 0x1683
    jmp        Label0x169d              ;Offset 0x169d
Label0x1690:                            ;Offset 0x1690
    mov        al, byte ptr [si]
    stosb
    mov        al, byte ptr [si + 2000h];Offset 0x2000
    stosb
    add        si, 0050h
    loop       Label0x1690              ;Offset 0x1690
Label0x169d:                            ;Offset 0x169d
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, 0008h
    xor        ax, ax
Label0x16a8:                            ;Offset 0x16a8
    push       si
    push       di
    mov        cl, 04h
    repe cmpsw
    pop        di
    pop        si
    je         Label0x16c5              ;Offset 0x16c5
    add        si, 0008h
    inc        al
    je         Label0x16c5              ;Offset 0x16c5
    jno        Label0x16a8              ;Offset 0x16a8
    xor        cx, cx
    mov        ds, cx
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    jmp        Label0x16a8              ;Offset 0x16a8
Label0x16c5:                            ;Offset 0x16c5
    add        sp, 0008h
    pop        es
    pop        cx
    pop        dx
    ret
Label0x16cc:                            ;Offset 0x16cc
    push       bx
    mov        dh, 0a0h                 ;Segment 0xa000
    push       dx
    cmp        al, 13h
    jne        Label0x16d7              ;Offset 0x16d7
    jmp        Label0x175e              ;Offset 0x175e
Label0x16d7:                            ;Offset 0x16d7
    mov        al, bh
    cbw
    mov        di, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        si, ax
    shl        di, 01h
    mov        ax, word ptr [di + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x26a0               ;Offset 0x26a0
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
Label0x1707:                            ;Offset 0x1707
    mov        al, byte ptr [si]
    not        al
    stosb
    add        si, dx
    loop       Label0x1707              ;Offset 0x1707
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        ax, (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x5
    out        dx, ax
Label0x1717:                            ;Offset 0x1717
    mov        ds, cx
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    sub        di, bx
    xor        ax, ax
Label0x1721:                            ;Offset 0x1721
    push       si
    push       di
    mov        cx, bx
    call       Func0xce5                ;Offset 0xce5
    jb         Label0x1741              ;Offset 0x1741
    jne        Label0x1741              ;Offset 0x1741
    mov        cl, 08h
    repe cmpsb
    je         Label0x1736              ;Offset 0x1736
    pop        di
    pop        si
    jmp        Label0x1747              ;Offset 0x1747
Label0x1736:                            ;Offset 0x1736
    inc        si
    mov        cl, 06h
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1757              ;Offset 0x1757
    jmp        Label0x1747              ;Offset 0x1747
Label0x1741:                            ;Offset 0x1741
    repe cmpsb
    pop        di
    pop        si
    je         Label0x1757              ;Offset 0x1757
Label0x1747:                            ;Offset 0x1747
    mov        cx, bx
    call       Func0xce5                ;Offset 0xce5
    jb         Label0x1751              ;Offset 0x1751
    add        si, 0002h
Label0x1751:                            ;Offset 0x1751
    add        si, bx
    inc        al
    jne        Label0x1721              ;Offset 0x1721
Label0x1757:                            ;Offset 0x1757
    add        sp, bx
    pop        bx
    pop        es
    pop        cx
    pop        dx
    ret
Label0x175e:                            ;Offset 0x175e
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x26b2               ;Offset 0x26b2
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
Label0x178b:                            ;Offset 0x178b
    call       Func0x179d               ;Offset 0x179d
    stosb
    add        si, bp
    loop       Label0x178b              ;Offset 0x178b
    pop        bp
    pop        bx
    mov        dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov        al, VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap OR VGA_SEQ4_Chain4;0xe
    out        dx, al
    jmp        Label0x1717              ;Offset 0x1717
ReadCharacterAndAttributeAtCursorPosition ENDP

Func0x179d PROC NEAR                    ;Offset 0x179d
    mov        bx, 0100h
    mov        al, byte ptr [si + 04h]
    call       Func0x17a8               ;Offset 0x17a8
    mov        al, byte ptr [si]
Func0x179d ENDP
;continue!
Func0x17a8 PROC NEAR                    ;Offset 0x17a8
    mov        ah, 03h
Label0x17aa:                            ;Offset 0x17aa
    mov        dl, VGA_GraphicsControllerData_lowbyte;Port 0x3cf
    mov        al, ah
    out        dx, al
    mov        dl, VGA_CRTControllerDataD_lowbyte;Port 0x3d5
    in         al, dx
    or         al, al
    je         Label0x17b8              ;Offset 0x17b8
    or         bl, bh
Label0x17b8:                            ;Offset 0x17b8
    rol        bh, 01h
    dec        ah
    jns        Label0x17aa              ;Offset 0x17aa
    mov        al, bl
    ret
Func0x17a8 ENDP

Func0x17c1 PROC NEAR                    ;Offset 0x17c1
    push       cx
    mov        dx, word ptr [si]
    xchg       dl, dh
    mov        cx, 0008h
Label0x17c9:                            ;Offset 0x17c9
    shl        dx, 01h
    jb         Label0x17d0              ;Offset 0x17d0
    jns        Label0x17d0              ;Offset 0x17d0
    stc
Label0x17d0:                            ;Offset 0x17d0
    rcl        al, 01h
    shl        dx, 01h
    loop       Label0x17c9              ;Offset 0x17c9
    stosb
    xor        si, 2000h
    pop        cx
    ret
Func0x17c1 ENDP

WriteCharacterAndAttributeAtCursorPosition PROC NEAR;Offset 0x17dd
    push       cx
    push       dx
    push       es
    mov        ah, bl
    push       ax
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x61e2               ;Offset 0x61e2
    mov        dx, 0b800h               ;Segment 0xb800
    cmp        al, BDA_DM_80x25_16_Color_Text;0x3
    jbe        Label0x17fa              ;Offset 0x17fa
    cmp        al, BDA_DM_640x480_BW_Graphics;0x11
    je         Label0x183d              ;Offset 0x183d
    cmp        al, BDA_DM_80x25_Monochrome_Text;0x7
    jne        Func0x1843               ;Offset 0x1843
    mov        dh, 0b0h                 ;Segment 0xb000
Label0x17fa:                            ;Offset 0x17fa
    mov        es, dx
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2682               ;Offset 0x2682
    add        di, ax
    test       byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne        Label0x1820              ;Offset 0x1820
    pop        ax
    rep stosw
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1820:                            ;Offset 0x1820
    mov        dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add        dl, 06h                  ;Port 0x3?a
    pop        si
Label0x1828:                            ;Offset 0x1828
    in         al, dx
    shr        al, 01h
    jb         Label0x1828              ;Offset 0x1828
    cli
Label0x182e:                            ;Offset 0x182e
    in         al, dx
    shr        al, 01h
    jae        Label0x182e              ;Offset 0x182e
    mov        ax, si
    stosw
    sti
    loop       Label0x1828              ;Offset 0x1828
    pop        es
    pop        dx
    pop        cx
    ret
Label0x183d:                            ;Offset 0x183d
    and        bl, 80h
    or         bl, 0fh
WriteCharacterAndAttributeAtCursorPosition ENDP
;continue!
Func0x1843 PROC NEAR                    ;Offset 0x1843
    ja         Label0x1882              ;Offset 0x1882
    mov        es, dx
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x2690               ;Offset 0x2690
    mov        di, ax
    mov        dl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop        ax
    or         al, al
    jns        Label0x1860              ;Offset 0x1860
    lds        si, ds:[INT_1F_HandlerOfs];Offset 0x7c
    and        al, 7fh
    jmp        Label0x1864              ;Offset 0x1864
Label0x1860:                            ;Offset 0x1860
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
Label0x1864:                            ;Offset 0x1864
    cbw
    shl        ax, 01h
    shl        ax, 01h
    shl        ax, 01h
    add        si, ax
    cmp        dl, BDA_DM_640x200_BW_Graphics;0x6
    je         Label0x187b              ;Offset 0x187b
    shl        di, 01h
    push       bx
    call       Func0x1a58               ;Offset 0x1a58
    pop        bx
    jmp        Label0x187e              ;Offset 0x187e
Label0x187b:                            ;Offset 0x187b
    call       Func0x19f4               ;Offset 0x19f4
Label0x187e:                            ;Offset 0x187e
    pop        es
    pop        dx
    pop        cx
    ret
Label0x1882:                            ;Offset 0x1882
    mov        dh, 0a0h                 ;Segment 0xa000
    mov        es, dx
    cmp        al, 13h
    jne        Label0x188d              ;Offset 0x188d
    jmp        Label0x198e              ;Offset 0x198e
Label0x188d:                            ;Offset 0x188d
    mov        al, bh
    cbw
    mov        si, ax
    mul        word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov        di, ax
    shl        si, 01h
    mov        ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call       Func0x26a0               ;Offset 0x26a0
    add        di, ax
    pop        ax
    push       bp
    push       cx
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    lds        si, ds:[INT_43_HandlerOfs];Offset 0x10c
    xor        ch, ch
    call       Func0xce5                ;Offset 0xce5
    jb         Label0x18c6              ;Offset 0x18c6
    jne        Label0x18be              ;Offset 0x18be
    or         ch, 02h
Label0x18be:                            ;Offset 0x18be
    mov        cl, 10h
    mul        cl
    mov        cl, 0eh
    jmp        Label0x18c8              ;Offset 0x18c8
Label0x18c6:                            ;Offset 0x18c6
    mul        cl
Label0x18c8:                            ;Offset 0x18c8
    add        si, ax
    or         bl, bl
    js         Label0x193e              ;Offset 0x193e
    mov        al, 52h
    push       cx
    push       cx
    xor        ch, ch
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out        dx, ax
    xor        ax, ax
    push       di
Label0x18de:                            ;Offset 0x18de
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18de              ;Offset 0x18de
    pop        di
    mov        al, VGA_SEQIdx_MapMask   ;0x2
    mov        ah, bl
    out        dx, ax
    mov        dx, di
    pop        cx
    and        ch, 02h
    je         Label0x190d              ;Offset 0x190d
    xor        ch, ch
    mov        cl, 08h
Label0x18f7:                            ;Offset 0x18f7
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x18f7              ;Offset 0x18f7
    mov        cl, 07h
    inc        si
Label0x1902:                            ;Offset 0x1902
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1902              ;Offset 0x1902
    inc        di
    jmp        Label0x1915              ;Offset 0x1915
Label0x190d:                            ;Offset 0x190d
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x190d              ;Offset 0x190d
Label0x1915:                            ;Offset 0x1915
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x1920              ;Offset 0x1920
    mov        cx, 0010h
Label0x1920:                            ;Offset 0x1920
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    ;jne        Label0x18cf              ;Offset 0x18cf !!!!!!!!!!!
    DB 075h, 0A4h                       ;Generates a broken jump (target = middle of instruction)
Label0x192b:                            ;Offset 0x192b
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
Label0x193d:                            ;Offset 0x193d
    push       dx
Label0x193e:                            ;Offset 0x193e
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
    je         Label0x196e              ;Offset 0x196e
    xor        ch, ch
    mov        cl, 08h
Label0x1958:                            ;Offset 0x1958
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1958              ;Offset 0x1958
    mov        cl, 07h
    inc        si
Label0x1963:                            ;Offset 0x1963
    lodsb
    mov        byte ptr es:[di], al
    add        di, bp
    loop       Label0x1963              ;Offset 0x1963
    inc        di
    jmp        Label0x1976              ;Offset 0x1976
Label0x196e:                            ;Offset 0x196e
    lodsb
    xchg       byte ptr es:[di], al
    add        di, bp
    loop       Label0x196e              ;Offset 0x196e
Label0x1976:                            ;Offset 0x1976
    pop        cx
    mov        al, ch
    and        ch, 01h
    je         Label0x1981              ;Offset 0x1981
    mov        cx, 0010h
Label0x1981:                            ;Offset 0x1981
    sub        si, cx
    mov        ch, al
    mov        di, dx
    inc        di
    pop        dx
    dec        dx
    jne        Label0x193d              ;Offset 0x193d
    jmp        Label0x192b              ;Offset 0x192b
Label0x198e:                            ;Offset 0x198e
    mov        ax, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    call       Func0x26b2               ;Offset 0x26b2
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
Label0x19c1:                            ;Offset 0x19c1
    lodsb
    push       di
    push       cx
    mov        cx, 0008h
Label0x19c7:                            ;Offset 0x19c7
    rcl        al, 01h
    jae        Label0x19d0              ;Offset 0x19d0
    mov        byte ptr es:[di], bl
    jmp        Label0x19d3              ;Offset 0x19d3
Label0x19d0:                            ;Offset 0x19d0
    mov        byte ptr es:[di], bh
Label0x19d3:                            ;Offset 0x19d3
    inc        di
    loop       Label0x19c7              ;Offset 0x19c7
    pop        cx
    pop        di
    add        di, bp
    loop       Label0x19c1              ;Offset 0x19c1
    pop        cx
    sub        si, cx
    mov        di, dx
    add        di, 0008h
    pop        dx
    dec        dx
    ;jne        Label0x19bd              ;Offset 0x19bd !!!!!!!!!!!!!!
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
Func0x1843 ENDP

Func0x19f4 PROC NEAR                    ;Offset 0x19f4
    or        bl, bl
    js        Label0x1a28               ;Offset 0x1a28
Label0x19f8:                            ;Offset 0x19f8
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
    loop      Label0x19f8               ;Offset 0x19f8
    ret
Label0x1a28:                            ;Offset 0x1a28
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
    loop      Label0x1a28               ;Offset 0x1a28
    ret
Func0x19f4 ENDP

Func0x1a58 PROC NEAR                    ;Offset 0x1a58
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
    js        Label0x1a9a               ;Offset 0x1a9a
Label0x1a77:                            ;Offset 0x1a77
    mov       bh, 04h
Label0x1a79:                            ;Offset 0x1a79
    lodsb
    call      Func0x1abd                ;Offset 0x1abd
    mov       word ptr es:[di], ax
    lodsb
    call      Func0x1abd                ;Offset 0x1abd
    mov       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a79               ;Offset 0x1a79
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a77               ;Offset 0x1a77
    ret
Label0x1a9a:                            ;Offset 0x1a9a
    mov       bh, 04h
Label0x1a9c:                            ;Offset 0x1a9c
    lodsb
    call      Func0x1abd                ;Offset 0x1abd
    xor       word ptr es:[di], ax
    lodsb
    call      Func0x1abd                ;Offset 0x1abd
    xor       word ptr es:[di + 2000h], ax;Offset 0x2000
    add       di, 0050h
    dec       bh
    jne       Label0x1a9c               ;Offset 0x1a9c
    sub       si, 0008h
    sub       di, 013eh
    loop      Label0x1a9a               ;Offset 0x1a9a
    ret
Func0x1a58 ENDP

Func0x1abd PROC NEAR                    ;Offset 0x1abd
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
Func0x1abd ENDP

WriteCharacterOnlyAtCursorPosition PROC NEAR;Offset 0x1ae6
    push      cx
    push      dx
    push      es
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x61e2                ;Offset 0x61e2
    mov       dx, 0b800h                ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1afd               ;Offset 0x1afd
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x1b44               ;Offset 0x1b44
    mov       dh, 0b0h                  ;Segment 0xb000
Label0x1afd:                            ;Offset 0x1afd
    mov       es, dx
    mov       al, bh
    cbw
    mov       si, ax
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    mov       di, ax
    shl       si, 01h
    mov       ax, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    call      Func0x2682                ;Offset 0x2682
    add       di, ax
    pop       ax
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Unknown;Offset 0x487 0x4
    jne       Label0x1b25               ;Offset 0x1b25
Label0x1b1d:                            ;Offset 0x1b1d
    stosb
    inc       di
    loop      Label0x1b1d               ;Offset 0x1b1d
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1b25:                            ;Offset 0x1b25
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a VGA_InputStatus1
    mov       ah, al
Label0x1b2e:                            ;Offset 0x1b2e
    in        al, dx
    shr       al, 01h
    jb        Label0x1b2e               ;Offset 0x1b2e
    cli
Label0x1b34:                            ;Offseet 0x1b34
    in        al, dx
    shr       al, 01h
    jae       Label0x1b34               ;Offset 0x1b34
    mov       al, ah
    stosb
    sti
    inc       di
    loop      Label0x1b2e               ;Offset 0x1b2e
    pop       es
    pop       dx
    pop       cx
    ret
Label0x1b44:                            ;Offset 0x1b44
    jmp       Func0x1843                ;Offset 0x1843
WriteCharacterOnlyAtCursorPosition ENDP

SetBorderColorOrPalette PROC NEAR       ;Offset 0x1b47
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexB_lowbyte;Offset 0x463 0xb4
    je        Label0x1b57               ;Offset 0x1b57
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x1b58               ;Offset 0x1b58
    int       42h
Label0x1b57:                            ;Offset 0x1b57
    ret
Label0x1b58:                            ;Offset 0x1b58
    push      bx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    xchg      ax, si
    or        bh, bh
    jne       Label0x1bb1               ;Offset 0x1bb1
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
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1b97               ;Offset 0x1b97
    mov       al, bl
    xor       ah, ah
    call      Func0x2b0d                ;Offset 0x2b0d
    or        si, si
    je        Label0x1b97               ;Offset 0x1b97
    mov       byte ptr es:[di], al
    jmp       Label0x1b97               ;Offset 0x1b97
Label0x1b97:                            ;Offset 0x1b97
    mov       al, bl
    mov       ah, 11h
    call      Func0x2b0d                ;Offset 0x2b0d
    or        si, si
    je        Label0x1ba6               ;Offset 0x1ba6
    mov       byte ptr es:[di + 10h], al
Label0x1ba6:                            ;Offset 0x1ba6
    mov       bl, bh
    and       bl, 20h
    shl       bl, 01h
    shl       bl, 01h
    rol       bl, 01h
Label0x1bb1:                            ;Offset 0x1bb1
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1bdb               ;Offset 0x1bdb
    mov       al, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       al, 0dfh
    and       bl, 01h
    je        Label0x1bc7               ;Offset 0x1bc7
    or        al, 20h
Label0x1bc7:                            ;Offset 0x1bc7
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    and       al, 10h
    or        al, 02h
    or        al, bl
    mov       ah, 01h
    call      Func0x1be1                ;Offset 0x1be1
    call      Func0x1be1                ;Offset 0x1be1
    call      Func0x1be1                ;Offset 0x1be1
Label0x1bdb:                            ;Offset 0x1bdb
    call      Func0x2afd                ;Offset 0x2afd
    pop       es
    pop       bx
    ret
SetBorderColorOrPalette ENDP

Func0x1be1 PROC NEAR                    ;Offset 0x1be1
    call      Func0x2b0d                ;Offset 0x2b0d
    or        si, si
    je        Label0x1bec               ;Offset 0x1bec
    inc       di
    mov       byte ptr es:[di], al
Label0x1bec:                            ;Offset 0x1bec
    add       ax, 0102h
    ret
Func0x1be1 ENDP

WriteGraphicsPixel PROC NEAR            ;Offset 0x1bf0
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1c58               ;Offset 0x1c58
    push      cx
    push      dx
    push      ax
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1c6f               ;Offset 0x1c6f
    mov       dh, al
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1c13               ;Offset 0x1c13
    or        ah, 20h
Label0x1c13:                            ;Offset 0x1c13
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    mov       ah, dh
    jb        Label0x1c2f               ;Offset 0x1c2f
    shr       si, 01h
    mov       al, 80h
    jmp       Label0x1c35               ;Offset 0x1c35
Label0x1c2f:                            ;Offset 0x1c2f
    shl       cl, 01h
    mov       al, 0c0h
    ror       ah, 01h
Label0x1c35:                            ;Offset 0x1c35
    add       di, si
    ror       ah, 01h
    and       cl, 07h
    and       ah, al
    shr       ax, cl
    mov       cl, byte ptr [di]
    shl       dh, 01h
    jb        Label0x1c50               ;Offset 0x1c50
    not       al
    and       cl, al
    or        cl, ah
    mov       byte ptr [di], cl
    jmp       Label0x1c54               ;Offset 0x1c54
Label0x1c50:                            ;Offset 0x1c50
    xor       cl, ah
    mov       byte ptr [di], cl
Label0x1c54:                            ;Offset 0x1c54
    pop       ax
    pop       dx
    pop       cx
    ret
Label0x1c58:                            ;Offset 0x1c58
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
Label0x1c6f:                            ;Offset 0x1c6f
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
    jns       Label0x1ca8               ;Offset 0x1ca8
    mov       ax, (VGA_GCTL3_FuncSourceXORLatched SHL 8) OR VGA_GCTLIdx_DataRotate;0x1803
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    jmp       Label0x1cb2               ;Offset 0x1cb2
Label0x1ca8:                            ;Offset 0x1ca8
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ2_MaskAll SHL 8) OR VGA_SEQIdx_MapMask;0xf02
    out       dx, ax
    xor       al, al
    xchg      byte ptr [di], al
Label0x1cb2:                            ;Offset 0x1cb2
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

ReadGraphicsPixel PROC NEAR             ;Offset 0x1ccd
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1d26               ;Offset 0x1d26
    push      cx
    push      dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    ja        Label0x1d3c               ;Offset 0x1d3c
    mov       ah, dl
    and       ah, 0feh
    mov       al, 28h
    mul       ah
    shr       dl, 01h
    jae       Label0x1ced               ;Offset 0x1ced
    or        ah, 20h
Label0x1ced:                            ;Offset 0x1ced
    mov       di, ax
    mov       si, cx
    shr       si, 01h
    shr       si, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    mov       ax, 0b800h                ;Segment 0xb800
    mov       ds, ax
    jb        Label0x1d12               ;Offset 0x1d12
    shr       si, 01h
    add       di, si
    and       cl, 07h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    and       al, 01h
    jmp       Label0x1d23               ;Offset 0x1d23
Label0x1d12:                            ;Offset 0x1d12
    add       di, si
    and       cl, 03h
    shl       cl, 01h
    mov       al, byte ptr [di]
    shl       al, cl
    rol       al, 01h
    rol       al, 01h
    and       al, 03h
Label0x1d23:                            ;Offset 0x1d23
    pop       dx
    pop       cx
    ret
Label0x1d26:                            ;Offset 0x1d26
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
Label0x1d3c:                            ;Offset 0x1d3c
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
Label0x1d6a:                            ;Offset 0x1d6a
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, ax
    mov       al, byte ptr [di]
    and       al, ch
    rol       bl, 01h
    or        bl, al
    dec       ah
    jns       Label0x1d6a               ;Offset 0x1d6a
    inc       cx
    rol       bl, cl
    mov       al, bl
    mov       bx, si
    pop       dx
    pop       cx
    ret
ReadGraphicsPixel ENDP

TeletypeOutput PROC NEAR                ;Offset 0x1d83
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
    jbe       Func0x1db7                ;Offset 0x1db7
TeletypeOutput ENDP
;continue!
Func0x1d9a PROC NEAR                    ;Offset 0x1d9a
    mov       cx, 0001h
    push      ds
    call      WriteCharacterOnlyAtCursorPosition;Offset 0x1ae6
    pop       ds
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    je        Func0x1dd7                ;Offset 0x1dd7
Func0x1d9a ENDP
;continue!
Func0x1da9 PROC NEAR                    ;Offset 0x1da9
    call      SetCursorPosition         ;Offset 0x1127
Func0x1da9 ENDP
;continue!
Func0x1dac PROC NEAR                    ;Offset 0x1dac
    pop       dx
    pop       cx
    pop       bx
    pop       ax
    ret
Func0x1dac ENDP

Func0x1db1 PROC NEAR                    ;Offset 0x1db1
    push      ax
    push      bx
    push      cx
    push      dx
    cmp       al, 0dh
Func0x1db1 ENDP
;continue!
Func0x1db7 PROC NEAR                    ;Offset 0x1db7
    je        Label0x1dcc               ;Offset 0x1dcc
    cmp       al, 0ah
    je        Func0x1dd9                ;Offset 0x1dd9
    cmp       al, 08h
    je        Label0x1dd0               ;Offset 0x1dd0
    cmp       al, 07h
    jne       Func0x1d9a                ;Offset 0x1d9a
    mov       al, 25h
    call      Beep                      ;Offset 0x2c12
    jmp       Func0x1dac                ;Offset 0x1dac
Label0x1dcc:                            ;Offset 0x1dcc
    xor       dl, dl
    jmp       Func0x1da9                ;Offset 0x1da9
Label0x1dd0:                            ;Offset 0x1dd0
    or        dl, dl
    je        Func0x1da9                ;Offset 0x1da9
    dec       dx
    jmp       Func0x1da9                ;Offset 0x1da9
Func0x1db7 ENDP

Func0x1dd7 PROC NEAR                    ;Offset 0x1dd7
    xor       dl, dl
Func0x1dd7 ENDP
;continue!
Func0x1dd9 PROC NEAR                    ;Offset 0x1dd9
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Func0x1da9                ;Offset 0x1da9
    dec       dh
    call      SetCursorPosition         ;Offset 0x1127
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x61e2                ;Offset 0x61e2
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x1df5               ;Offset 0x1df5
    cbw
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    jne       Label0x1dfa               ;Offset 0x1dfa
Label0x1df5:                            ;Offset 0x1df5
    push      ds
    call      ReadCharacterAndAttributeAtCursorPosition;Offset 0x1610
    pop       ds
Label0x1dfa:                            ;Offset 0x1dfa
    mov       bh, ah
    xor       cx, cx
    mov       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec       dx
    mov       al, 01h
    call      ScrollUpWindow            ;Offset 0x11ae
    jmp       Func0x1dac                ;Offset 0x1dac
Func0x1dd9 ENDP

GetCurrentVideoMode PROC NEAR           ;Offset 0x1e0a
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, BDA_VMO_DontClearDisplay;0x80
    or        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       ah, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    ret
GetCurrentVideoMode ENDP

RAMDACFunctions PROC NEAR               ;Offset 0x1e1c
    mov       ah, 1bh
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;misassemble
    ja        EmptyFunction             ;Offset 0x1e30
    cbw
    mov       si, ax
    shl       si, 01h
    push      es
    push      bx
    call      word ptr cs:[si + RAMDACCallTable];Offset 0x1e36
    pop       bx
    pop       es
RAMDACFunctions ENDP

EmptyFunction PROC NEAR                 ;Offset 0x1e30
    ret
EmptyFunction ENDP

Func0x1e31 PROC NEAR                    ;Offset 0x1e31
    add       sp, 0004h
    pop       es
    ret
Func0x1e31 ENDP

;Offset 0x1e36
RAMDACCallTable         DW SetSinglePaletteRegister;Offset 0x1e6e       ;0x00
                        DW SetBorderColor;Offset 0x1e8b                 ;0x01
                        DW SetAllPaletteRegisters;Offset 0x1e8f         ;0x02
                        DW ToggleIntensityBlinkingBit;Offset 0x1ec1     ;0x03
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW GetIndividualPaletteRegister;Offset 0x1ee3   ;0x07
                        DW GetBorderColor;Offset 0x1eed                 ;0x08
                        DW GetAllPaletteRegisters;Offset 0x1ef1         ;0x09
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW SetIndividualDACRegister;Offset 0x1ef7       ;0x10
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW SetBlockOfDACRegisters;Offset 0x1f24         ;0x12
                        DW SelectVideoDACColorPage;Offset 0x1f3d        ;0x13
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW GetIndividualDACRegister;Offset 0x1f63       ;0x15
                        DW EmptyFunction;Offset 0x1e30                  ;
                        DW GetBlockOfDACRegisters;Offset 0x1f82         ;0x17
                        DW SetPELMask   ;Offset 0x1f9c                  ;0x18
                        DW GetPELMask   ;Offset 0x1fa4                  ;0x19
                        DW GetVideoDACColorPageState;Offset 0x1fb1      ;0x1a
                        DW PerformGrayScaleSumming;Offset 0x1fce        ;0x1b

SetSinglePaletteRegister PROC NEAR      ;Offset 0x1e6e
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Func0x1e8a                ;Offset 0x1e8a
SetSinglePaletteRegister ENDP
;continue!
Func0x1e75 PROC NEAR                    ;Offset 0x1e75
    mov       al, bh
    mov       ah, bl
    call      Func0x2b0d                ;Offset 0x2b0d
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je        Func0x1e8a                ;Offset 0x1e8a
    mov       al, bh
    xor       bh, bh
    mov       byte ptr es:[bx + di], al
Func0x1e75 ENDP
;continue!
Func0x1e8a PROC NEAR                    ;Offset 0x1e8a
    ret
Func0x1e8a ENDP

SetBorderColor PROC NEAR                ;Offset 0x1e8b
    mov       bl, 11h
    jmp       Func0x1e75                ;Offset 0x1e75
SetBorderColor ENDP

SetAllPaletteRegisters PROC NEAR        ;Offset 0x1e8f
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x1ec0               ;Offset 0x1ec0
    push      cx
    push      dx
    push      es
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je        Label0x1eab               ;Offset 0x1eab
    pop       ds
    push      ds
    mov       si, dx
    mov       cx, 0011h
    rep movsb
    mov       ds, cx
Label0x1eab:                            ;Offset 0x1eab
    pop       es
    mov       si, dx
    mov       ax, 1000h
    mov       cx, 0011h
    call      Func0x2bf2                ;Offset 0x2bf2
    call      Func0x28ef                ;Offset 0x28ef
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
Label0x1ec0:                            ;Offset 0x1ec0
    ret
SetAllPaletteRegisters ENDP

ToggleIntensityBlinkingBit PROC NEAR    ;Offset 0x1ec1
    mov       ah, 10h
    call      Func0x2b21                ;Offset 0x2b21
    cmp       bl, 01h
    ja        Label0x1edd               ;Offset 0x1edd
    and       al, 0f7h
    and       byte ptr ds:[BDA_CRTModeControlRegValue], NOT BDA_CMCRV_Blinking;Offset 0x465 0xdf
    or        bl, bl
    je        Label0x1edd               ;Offset 0x1edd
    or        al, 08h
    or        byte ptr ds:[BDA_CRTModeControlRegValue], BDA_CMCRV_Blinking;Offset 0x465 0x20
Label0x1edd:                            ;Offset 0x1edd
    mov       ah, 10h
    call      Func0x2b0d                ;Offset 0x2b0d
    ret
ToggleIntensityBlinkingBit ENDP

GetIndividualPaletteRegister PROC NEAR  ;Offset 0x1ee3
    mov       ah, bl
GetIndividualPaletteRegister ENDP
;continue!
Func0x1ee5 PROC NEAR                    ;Offset 0x1ee5
    call      Func0x2b21                ;Offset 0x2b21
    mov       bh, al
    jmp       Func0x1e31                ;Offset 0x1e31
Func0x1ee5 ENDP

GetBorderColor PROC NEAR                ;Offset 0x1eed
    mov       ah, 11h
    jmp       Func0x1ee5                ;Offset 0x1ee5
GetBorderColor ENDP

GetAllPaletteRegisters PROC NEAR        ;Offset 0x1ef1
    mov       di, dx
    call      Func0x2905                ;Offset 0x2905
    ret
GetAllPaletteRegisters ENDP

SetIndividualDACRegister PROC NEAR      ;Offset 0x1ef7
    push      cx
    push      dx
    mov       ah, dh
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    je        Label0x1f07               ;Offset 0x1f07
    call      Func0x2a00                ;Offset 0x2a00
    mov       cx, ax
Label0x1f07:                            ;Offset 0x1f07
    call      Func0x2c00                ;Offset 0x2c00
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    jmp       Label0x1f12               ;Offset 0x1f12 Speed sensitive!
Label0x1f12:                            ;Offset 0x1f12
    inc       dx
    mov       al, ah
    out       dx, al
    jmp       Label0x1f18               ;Offset 0x1f18 Speed sensitive!
Label0x1f18:                            ;Offset 0x1f18
    mov       al, ch
    out       dx, al
    jmp       Label0x1f1d               ;Offset 0x1f1d Speed sensitive!
Label0x1f1d:                            ;Offset 0x1f1d
    mov       al, cl
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
SetIndividualDACRegister ENDP

SetBlockOfDACRegisters PROC NEAR        ;Offset 0x1f24
    push      cx
    push      dx
    mov       si, dx
    call      TurnScreenOff             ;Offset 0x2b3d
    push      ax
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    inc       dx
    call      Func0x29c7                ;Offset 0x29c7
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b3f
    pop       dx
    pop       cx
    ret
SetBlockOfDACRegisters ENDP

SelectVideoDACColorPage PROC NEAR       ;Offset 0x1f3d
    mov       ah, 10h
    call      Func0x2b21                ;Offset 0x2b21
    or        bl, bl
    jne       Label0x1f51               ;Offset 0x1f51
    shl       al, 01h
    add       bh, 0ffh
    rcr       al, 01h
    call      Func0x2b0d                ;Offset 0x2b0d
    ret
Label0x1f51:                            ;Offset 0x1f51
    or        al, al
    mov       al, bh
    js        Label0x1f5b               ;Offset 0x1f5b
    shl       al, 01h
    shl       al, 01h
Label0x1f5b:                            ;Offset 0x1f5b
    and       al, 0fh
    mov       ah, 14h
    call      Func0x2b0d                ;Offset 0x2b0d
    ret
SelectVideoDACColorPage ENDP

GetIndividualDACRegister PROC NEAR      ;Offset 0x1f63
    mov       al, bl
    mov       bl, dl
    call      Func0x2c00                ;Offset 0x2c00
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    out       dx, al
    jmp       Label0x1f70               ;Offset 0x1f70 Speed sensitive!
Label0x1f70:                            ;Offset 0x1f70
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1f75               ;Offset 0x1f75 Speed sensitive!
Label0x1f75:                            ;Offset 0x1f75
    mov       bh, al
    in        al, dx
    jmp       Label0x1f7a               ;Offset 0x1f7a Speed sensitive!
Label0x1f7a:                            ;Offset 0x1f7a
    mov       ah, al
    in        al, dx
    sti
    xchg      ax, cx
    mov       dx, bx
    ret
GetIndividualDACRegister ENDP

GetBlockOfDACRegisters PROC NEAR        ;Offset 0x1f82
    push      cx
    push      dx
    mov       di, dx
    call      TurnScreenOff             ;Offset 0x2b3d
    push      ax
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    call      Func0x29f7                ;Offset 0x29f7
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b3f
    pop       dx
    pop       cx
    ret
GetBlockOfDACRegisters ENDP

SetPELMask PROC NEAR                    ;Offset 0x1f9c
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    xchg      ax, bx
    out       dx, al
    pop       dx
    ret
SetPELMask ENDP

GetPELMask PROC NEAR                    ;Offset 0x1fa4
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    in        al, dx
    pop       dx
    xor       ah, ah
    xchg      ax, bx
    jmp       Func0x1e31                ;Offset 0x1e31
    ret
GetPELMask ENDP

GetVideoDACColorPageState PROC NEAR     ;Offset 0x1fb1
    mov       ah, 14h
    call      Func0x2b21                ;Offset 0x2b21
    and       al, 0fh
    mov       bh, al
    mov       ah, 10h
    call      Func0x2b21                ;Offset 0x2b21
    rol       al, 01h
    and       al, 01h
    mov       bl, al
    jne       Label0x1fcb               ;Offset 0x1fcb
    shr       bh, 01h
    shr       bh, 01h
Label0x1fcb:                            ;Offset 0x1fcb
    jmp       Func0x1e31                ;Offset 0x1e31
GetVideoDACColorPageState ENDP

PerformGrayScaleSumming PROC NEAR       ;Offset 0x1fce
    push      cx
    push      dx
    call      TurnScreenOff             ;Offset 0x2b3d
    push      ax
Label0x1fd4:                            ;Offset 0x1fd4
    push      cx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    jmp       Label0x1fdd               ;Offset 0x1fdd Speed sensitive!
Label0x1fdd:                            ;Offset 0x1fdd
    mov       dl, VGA_RAMDACData_lowbyte;Port 0x3c9
    in        al, dx
    jmp       Label0x1fe2               ;Offset 0x1fe2 Speed sensitive!
Label0x1fe2:                            ;Offset 0x1fe2
    mov       bh, al
    in        al, dx
    jmp       Label0x1fe7               ;Offset 0x1fe7 Speed sensitive!
Label0x1fe7:                            ;Offset 0x1fe7
    mov       ah, al
    in        al, dx
    jmp       Label0x1fec               ;Offset 0x1fec Speed sensitive!
Label0x1fec:                            ;Offset 0x1fec
    xchg      ax, cx
    dec       dx
    mov       al, bl
    out       dx, al
    mov       dh, bh
    call      Func0x2a00                ;Offset 0x2a00
    mov       dx, VGA_RAMDACData        ;Port 0x3c9
    out       dx, al
    jmp       Label0x1ffc               ;Offset 0x1ffc Speed sensitive!
Label0x1ffc:                            ;Offset 0x1ffc
    out       dx, al
    jmp       Label0x1fff               ;Offset 0x1fff Speed sensitive!
Label0x1fff:                            ;Offset 0x1fff
    out       dx, al
    inc       bx
    pop       cx
    loop      Label0x1fd4               ;Offset 0x1fd4
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b3f
    pop       dx
    pop       cx
    ret
PerformGrayScaleSumming ENDP

TextFunctions PROC NEAR                 ;Offset 0x200b
    lea       si, [TextFunctions0X]     ;Offset 0x2038
    cmp       al, 10h
    jb        Label0x2029               ;Offset 0x2029
    lea       si, [TextFunctions1X]     ;Offset 0x2059
    cmp       al, 20h
    jb        Label0x2029               ;Offset 0x2029
    lea       si, [TextFunctions2X]     ;Offset 0x2074
    cmp       al, 30h
    jb        Label0x2029               ;Offset 0x2029
    jne       Label0x2037               ;Offset 0x2037
    call      GetFontInformation        ;Offset 0x20d8
    ret
Label0x2029:                            ;Offset 0x2029
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
Label0x2037:                            ;Offset 0x2037
    ret
TextFunctions ENDP

TextFunctions0X PROC NEAR               ;Offset 0x2038
    cmp       al, 03h
    je        SelectCharacterMap        ;Offset 0x2050
    cmp       al, 04h
    ja        Return                    ;Offset 0x2058
    and       bl, 7fh
    call      Func0x2717                ;Offset 0x2717
    call      Func0x2758                ;Offset 0x2758
    call      Func0x271d                ;Offset 0x271d
    call      Func0x2afd                ;Offset 0x2afd
    ret
SelectCharacterMap:                     ;Offset 0x2050
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    mov       ah, bl
    out       dx, ax
Return:                                 ;Offset 0x2058
    ret
TextFunctions0X ENDP

TextFunctions1X PROC NEAR               ;Offset 0x2059
    cmp       al, 03h
    je        Label0x2073               ;Offset 0x2073
    cmp       al, 04h
    ja        Label0x2073               ;Offset 0x2073
    and       bl, 7fh
    call      Func0x2717                ;Offset 0x2717
    call      Func0x2758                ;Offset 0x2758
    call      Func0x27fa                ;Offset 0x27fa
    call      Func0x271d                ;Offset 0x271d
    call      Func0x2afd                ;Offset 0x2afd
Label0x2073:                            ;Offset 0x2073
    ret
TextFunctions1X ENDP

TextFunctions2X PROC NEAR               ;Offset 0x2074
    cmp       al, 04h
    ja        Label0x20d7               ;Offset 0x20d7
    cbw
    push      ax
    dec       ax
    je        Label0x20a4               ;Offset 0x20a4
    jns       Label0x2089               ;Offset 0x2089
    pop       ax
    mov       word ptr ds:[INT_1F_HandlerOfs], bp;Offset 0x7c
    mov       word ptr ds:[INT_1F_HandlerSeg], es;Offset 0x7e
    ret
Label0x2089:                            ;Offset 0x2089
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3960
    mov       cx, 000eh
    dec       ax
    je        Label0x20a4               ;Offset 0x20a4
    lea       bp, [Font8x8]             ;Offset 0x3160
    mov       cl, 08h
    dec       ax
    je        Label0x20a4               ;Offset 0x20a4
    lea       bp, [Font8x16]            ;Offset 0x3960
    mov       cl, 10h
Label0x20a4:                            ;Offset 0x20a4
    mov       word ptr ds:[INT_43_HandlerOfs], bp;Offset 0x10c
    mov       word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    pop       ax
    cmp       al, 01h
    je        Label0x20bb               ;Offset 0x20bb
    cmp       cl, 0eh
    jne       Label0x20bb               ;Offset 0x20bb
    mov       ax, 0bd70h
    int       6dh
Label0x20bb:                            ;Offset 0x20bb
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    mov       al, dl
    and       bx, 00ffh
    je        Label0x20d3               ;Offset 0x20d3
    mov       al, 19h
    cmp       bl, 03h
    ja        Label0x20d3               ;Offset 0x20d3
    mov       al, byte ptr cs:[bx + RowsOnScreen];Offset 0x314a
Label0x20d3:                            ;Offset 0x20d3
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
Label0x20d7:                            ;Offset 0x20d7
    ret
TextFunctions2X ENDP

GetFontInformation PROC NEAR            ;Offset 0x20d8
    mov       cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov       al, bh
    sub       al, 02h
    cbw
    jb        Label0x2102               ;Offset 0x2102
    cmp       al, 05h
    ja        Label0x210e               ;Offset 0x210e
    shl       ax, 01h
    mov       si, ax
    mov       bp, word ptr cs:[si + Data0x314e];Offset 0x314e
    push      cs
    pop       es
    or        ax, ax
    jne       Label0x2101               ;Offset 0x2101
    mov       ax, 0bd71h
    int       6dh
    xor       ax, ax
Label0x2101:                            ;Offset 0x2101
    ret
Label0x2102:                            ;Offset 0x2102
    inc       ax
    js        Label0x210a               ;Offset 0x210a
    les       bp, ds:[INT_43_HandlerOfs];Offset 0x10c
    ret
Label0x210a:                            ;Offset 0x210a
    les       bp, ds:[INT_1F_HandlerOfs];Offset 0x7c
Label0x210e:                            ;Offset 0x210e
    ret
GetFontInformation ENDP

AlternateFunctions PROC NEAR            ;Offset 0x210f
    cmp       bl, 20h
    je        InstallAlternatePrintScreenHandler;Offset 0x2143
    cmp       bl, 10h
    je        GetEGAInfo                ;Offset 0x211b
    jmp       Label0x2150               ;Offset 0x2150
GetEGAInfo:                             ;Offset 0x211b
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
InstallAlternatePrintScreenHandler:     ;Offset 0x2143
    cli
    mov       word ptr ds:[INT_5_HandlerOfs], offset PrintScreenHandler;Offset 0x14 Offset 0x2c80
    mov       word ptr ds:[INT_5_HandlerSeg], cs;Offset 0x16
    sti
    ret
Label0x2150:                            ;Offset 0x2150
    push      bx
    push      cx
    push      dx
    mov       cl, 03h
    xor       bh, bh
    sub       bl, 30h
    shl       bx, 01h
    je        Label0x2169               ;Offset 0x2169
    cmp       bl, 0ah
    je        Label0x216a               ;Offset 0x216a
    cmp       bl, 0ch
    ja        Label0x217b               ;Offset 0x217b
    dec       cx
Label0x2169:                            ;Offset 0x2169
    dec       cx
Label0x216a:                            ;Offset 0x216a
    ;cmp       al, cl
    DB 3Ah, 0C1h                        ;misassembly
    ja        Label0x217b               ;Offset 0x217b
    call      word ptr cs:[bx + AlternateFunctionsTable];Offset 0x2180
    jb        Label0x217b               ;Offset 0x217b
    mov       al, 12h
Label0x2177:                            ;Offset 0x2177
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x217b:                            ;Offset 0x212b
    mov       al, 00h
    jmp       Label0x2177               ;Offset 0x2177
AlternateFunctions ENDP

    nop                                 ;hello

;Offset 0x2180
AlternateFunctionsTable DW SelectVerticalResolution;Offset 0x2204
                        DW SetPaletteLoading;Offset 0x218e
                        DW SetVGAMemoryIOMapping;Offset 0x219c
                        DW SetGrayscaleSumming;Offset 0x21da
                        DW SetCursorEmulation;Offset 0x21e8
                        DW DisplaySwitchInterface;Offset 0x2248
                        DW TurnScreenOnOff;Offset 0x21f6

SetPaletteLoading PROC NEAR             ;Offset 0x218e
    je   Enable                         ;Offset 0x2196
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0xf7
    ret
Enable:                                 ;Offset 0x2196
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
SetVGAMemoryIOMapping PROC NEAR         ;Offset 0x219c
    mov  dl, 00h
    je   Disable                        ;Offset 0x21a2
    dec  dl
Disable:                                ;Offset 0x21a2
    call FindMGAG100                    ;Offset 0x53d6
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_Header_Word_Command    ;0x4
    call AccessPCIRegister              ;Offset 0x5344
    and  cl, NOT PCI_Cmd_MemSpace       ;0xfd
    and  dl, PCI_Cmd_IOSpace OR PCI_Cmd_MemSpace;0x3
    je   DisableMemSpace                ;Offset 0x21b8
    or   cl, dl
DisableMemSpace:                        ;Offset 0x21b8
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x5344
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    nop
    mov  di, PCI_MGA_Option + 01h       ;0x41
    nop
    call AccessPCIRegister              ;Offset 0x5344
    and  cl, NOT (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0xfe
    and  dl, (PCI_MGA_Opt_VGAIOMapEnable SHR 8);0x1
    je   DoWrite                        ;Offset 0x21d2
    or   cl, dl                         ;OR in the new value (one)
DoWrite:                                ;Offset 0x21d2
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    nop
    call AccessPCIRegister              ;Offset 0x5344
    clc
    ret
SetVGAMemoryIOMapping ENDP

SetGrayscaleSumming PROC NEAR           ;Offset 0x21da
    je   Disable                        ;Offset 0x21e2
    or   byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale;Offset 0x489 0x2
    ret
Disable:                                ;Offset 0x21e2
    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489 0xfd
    ret
SetGrayscaleSumming ENDP

SetCursorEmulation PROC NEAR            ;Offset 0x21e8
    je   Enable                         ;Offset 0x21f0
    and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487 0xfe
    ret
Enable:                                 ;Offset 0x21f0
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
TurnScreenOnOff PROC NEAR               ;Offset 0x21f6
    lea  dx, [TurnScreenOff]            ;Offset 0x2b3d
    je   TurnOff                        ;Offset 0x2200
    lea  dx, [TurnScreenOn]             ;Offset 0x2b39
TurnOff:                                ;Offset 0x2200
    call dx
    clc
    ret
TurnScreenOnOff ENDP

SelectVerticalResolution PROC NEAR      ;Offset 0x2204
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x2229               ;Offset 0x2229
    cbw
    xchg      ax, si
    mov       al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       al, NOT BDA_VDDA_LineMode400;0xef
    test      al, BDA_VDDA_VGA          ;0x1
    je        Label0x2218               ;Offset 0x2218
    and       al, NOT BDA_VDDA_LineMode200;0x7f
Label0x2218:                            ;Offset 0x2218
    mov       bl, BDA_EFBS_MDAHiResEnhanced_2;0x9
    dec       si
    jns       Label0x222d               ;Offset 0x222d
    dec       bx
    test      al, BDA_VDDA_VGA          ;0x1
    jne       Label0x222b               ;Offset 0x222b
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x2236               ;Offset 0x2236
Label0x2229:                            ;Offset 0x2229
    stc
    ret
Label0x222b:                            ;Offset 0x222b
    or        al, BDA_VDDA_LineMode200  ;0x80
Label0x222d:                            ;Offset 0x222d
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    je        Label0x2236               ;Offset 0x2236
    mov       bl, BDA_EFBS_CGAMono80x25_2;0xb
Label0x2236:                            ;Offset 0x2236
    dec       si
    js        Label0x223b               ;Offset 0x223b
    or        al, BDA_VDDA_LineMode400  ;0x10
Label0x223b:                            ;Offset 0x223b
    mov       byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    ret
SelectVerticalResolution ENDP

DisplaySwitchInterface PROC NEAR        ;Offset 0x2248
    cbw
    xchg      ax, si
    dec       si
    je        Label0x229a               ;Offset 0x229a
    js        Label0x2266               ;Offset 0x2266
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x229a               ;Offset 0x229a
    dec       si
    je        Label0x2279               ;Offset 0x2279
    mov       si, dx
    call      RestoreBiosDataAreaVideoSections;Offset 0x262e
    mov       al, 0eh
Label0x2260:                            ;Offset 0x2260
    mov       dx, 46e8h                 ;Port 0x46e8 Some kind of video subsystem enable port?
    out       dx, al
    clc
    ret
Label0x2266:                            ;Offset 0x2266
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    jne       Label0x229a               ;Offset 0x229a
    mov       ax, 1280h
    int       42h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_DisplaySwitchingEnabled;Offset 0x489 0x40
    je        Label0x229a               ;Offset 0x229a
Label0x2279:                            ;Offset 0x2279
    mov       di, dx
    call      BackupBiosDataAreaVideoSections;Offset 0x254b
    push      es
    les       bx, ds:[INT_42_HandlerOfs];Offset 0x108
    mov       word ptr ds:[INT_6D_HandlerOfs], bx;Offset 0x1b4
    mov       word ptr ds:[INT_6D_HandlerSeg], es;Offset 0x1b6
    pop       es
    mov       word ptr ds:[INT_42_HandlerOfs], Int6DHandler;Offset 0x108 Offset 0xb70
    mov       word ptr ds:[INT_42_HandlerSeg], cs;Offset 0x10a
    mov       al, 06h
    jmp       Label0x2260               ;Offset 0x2260
Label0x229a:                            ;Offset 0x229a
    stc
    ret
DisplaySwitchInterface ENDP

WriteString PROC NEAR                   ;Offset 0x229c
    push      bx
    push      cx
    push      dx
    push      bp
    cmp       al, 03h
    ja        Label0x22e2               ;Offset 0x22e2
    jcxz      Label0x22e2               ;Offset 0x22e2
    cbw
    mov       di, ax
    dec       di
    mov       al, bh
    mov       si, ax
    shl       si, 01h
    push      word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    push      si
    call      SetCursorPosition         ;Offset 0x1127
Label0x22b8:                            ;Offset 0x22b8
    mov       al, byte ptr es:[bp]
    inc       bp
    cmp       al, 0dh
    jbe       Label0x22e7               ;Offset 0x22e7
Label0x22c1:                            ;Offset 0x22c1
    or        di, di
    jle       Label0x22ca               ;Offset 0x22ca
    mov       bl, byte ptr es:[bp]
    inc       bp
Label0x22ca:                            ;Offset 0x22ca
    call      Func0x2313                ;Offset 0x2313
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae       Label0x2300               ;Offset 0x2300
Label0x22d4:                            ;Offset 0x22d4
    call      SetCursorPosition         ;Offset 0x1127
Label0x22d7:                            ;Offset 0x22d7
    loop      Label0x22b8               ;Offset 0x22b8
    pop       si
    pop       dx
    shr       di, 01h
    jae       Label0x22e2               ;Offset 0x22e2
    call      SetCursorPosition         ;Offset 0x1127
Label0x22e2:                            ;Offset 0x22e2
    pop       bp
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x22e7:                            ;Offset 0x22e7
    je        Label0x22f5               ;Offset 0x22f5
    cmp       al, 0ah
    je        Label0x22f5               ;Offset 0x22f5
    cmp       al, 08h
    je        Label0x22f5               ;Offset 0x22f5
    cmp       al, 07h
    jne       Label0x22c1               ;Offset 0x22c1
Label0x22f5:                            ;Offset 0x22f5
    call      Func0x2320                ;Offset 0x2320
    pop       si
    push      si
    mov       dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x22d7               ;Offset 0x22d7
Label0x2300:                            ;Offset 0x2300
    xor       dl, dl
    inc       dh
    cmp       dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe       Label0x22d4               ;Offset 0x22d4
    dec       dh
    mov       al, 0ah
    call      Func0x2320                ;Offset 0x2320
    jmp       Label0x22d4               ;Offset 0x22d4
WriteString ENDP

Func0x2313 PROC NEAR                    ;Offset 0x2313
    push      cx
    push      di
    push      ds
    mov       cx, 0001h
    call      WriteCharacterAndAttributeAtCursorPosition;Offset 0x17dd
    pop       ds
    pop       di
    pop       cx
    ret
Func0x2313 ENDP

Func0x2320 PROC NEAR                    ;Offset 0x2320
    push      di
    push      ds
    call      Func0x1db1                ;Offset 0x1db1
    pop       ds
    pop       di
    ret
Func0x2320 ENDP

NotImplemented PROC NEAR                ;Offset 0x2328
    ret       
NotImplemented ENDP

DisplayCombinationCodeFunctions PROC NEAR;Offset 0x2329
    push      cx
    push      dx
    push      es
    xor       ah, ah
    dec       ax
    js        IsGet                     ;Offset 0x2338
    jne       NotSupported              ;Offset 0x233d
    call      SetDisplayCombinationCodeIndex;Offset 0x2b54
    jmp       Done                      ;Offset 0x233b
IsGet:                                  ;Offset 0x2338
    call      GetDisplayCombinationCode ;Offset 0x2b7b
Done:                                   ;Offset 0x233b
    mov       al, 1ah
NotSupported:                           ;Offset 0x233d
    pop       es
    pop       dx
    pop       cx
    ret
DisplayCombinationCodeFunctions ENDP

FunctionalityAndStateInfo PROC NEAR     ;Offset 0x2341
    or        bx, bx
    je        Label0x2348               ;Offset 0x2348
    xor       al, al
    ret
Label0x2348:                            ;Offset 0x2348
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
    call      GetDisplayCombinationCode ;Offset 0x2b7b
    xchg      ax, bx
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    push      ax
    lea       bx, [ColorsPerModePlusOne];Offset 0x2426
    xlatb     cs:[bx]
    dec       ax
    jns       ColorsUnder256            ;Offset 0x237a
    mov       ax, 0100h                 ;Set colors to 256 (mode 0x13)
ColorsUnder256:                         ;Offset 0x237a
    stosw
    pop       ax
    lea       bx, [NumberOfPagesPerMode];Offset 0x243a
    xlatb     cs:[bx]
    stosb
    call      GetNumberOfActiveScanlines;Offset 0x2a88
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
    call      Func0x2414                ;Offset 0x2414
    mov       al, 0ah
    call      Func0x240b                ;Offset 0x240b
    mov       al, 0ch
    call      Func0x240b                ;Offset 0x240b
    mov       al, 08h
    call      Func0x240b                ;Offset 0x240b
    mov       al, 04h
    call      Func0x240b                ;Offset 0x240b
    mov       al, 06h
    call      Func0x240b                ;Offset 0x240b
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

Func0x240b PROC NEAR                    ;Offset 0x240b
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je        Label0x2413               ;Offset 0x2413
    inc       dx
Label0x2413:                            ;Offset 0x2413
    ret
Func0x240b ENDP

Func0x2414 PROC NEAR                    ;Offset 0x2414
    shl       dl, 01h
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je        Label0x2424               ;Offset 0x2424
    mov       ax, cs
    mov       bx, es
    cmp       ax, bx
    je        Label0x2424               ;Offset 0x2424
    inc       dx
Label0x2424:                            ;Offset 0x2424
    ret
Func0x2414 ENDP

    nop                                 ;hello

;Offset 0x2426
ColorsPerModePlusOne    DB 011h, 011h, 011h, 011h, 005h, 005h, 003h, 001h, 001h, 001h
                        DB 001h, 001h, 001h, 011h, 011h, 001h, 011h, 003h, 011h, 000h

;Offset 0x243a
NumberOfPagesPerMode    DB 008h, 008h, 008h, 008h, 001h, 001h, 001h, 008h, 000h, 000h
                        DB 000h, 000h, 000h, 008h, 004h, 002h, 002h, 001h, 001h, 001h 

SaveRestoreVideoState PROC NEAR         ;Offset 0x244e
    push      cx
    test      cl, 07h
    je        Label0x245f               ;Offset 0x245f
    cbw
    inc       ax
    dec       ax
    je        Label0x2463               ;Offset 0x2463
    dec       ax
    je        Label0x2489               ;Offset 0x2489
    dec       ax
    je        Label0x24a5               ;Offset 0x24a5
Label0x245f:                            ;Offset 0x245f
    mov       al, 00h
    pop       cx
    ret
Label0x2463:                            ;Offset 0x2463
    xor       ax, ax
    shr       cl, 01h
    jae       Label0x246b               ;Offset 0x246b
    add       al, 46h
Label0x246b:                            ;Offset 0x246b
    shr       cl, 01h
    jae       Label0x2471               ;Offset 0x2471
    add       al, 3ah
Label0x2471:                            ;Offset 0x2471
    shr       cl, 01h
    jae       Label0x2478               ;Offset 0x2478
    add       ax, 0303h
Label0x2478:                            ;Offset 0x2478
    add       al, 20h
    mov       cl, 06h
    ror       ax, cl
    add       ah, 0ffh
    adc       al, 00h
    cbw
    xchg      ax, bx
Label0x2485:                            ;Offset 0x2485
    mov       al, 1ch
    pop       cx
    ret
Label0x2489:                            ;Offset 0x2489
    push      dx
    lea       di, [bx + 20h]
    shr       cl, 01h
    jae       Label0x2494               ;Offset 0x2494
    call      Func0x24be                ;Offset 0x24be
Label0x2494:                            ;Offset 0x2494
    shr       cl, 01h
    jae       Label0x249b               ;Offset 0x249b
    call      Func0x2547                ;Offset 0x2547
Label0x249b:                            ;Offset 0x249b
    shr       cl, 01h
    jae       Label0x24a2               ;Offset 0x24a2
    call      Func0x2584                ;Offset 0x2584
Label0x24a2:                            ;Offset 0x24a2
    pop       dx
    jmp       Label0x2485               ;Offset 0x2485
Label0x24a5:                            ;Offset 0x24a5
    push      dx
    shr       cl, 01h
    jae       Label0x24ad               ;Offset 0x24ad
    call      Func0x25a9                ;Offset 0x25a9
Label0x24ad:                            ;Offset 0x24ad
    shr       cl, 01h
    jae       Label0x24b4               ;Offset 0x24b4
    call      Func0x262a                ;Offset 0x262a
Label0x24b4:                            ;Offset 0x24b4
    shr       cl, 01h
    jae       Label0x24bb               ;Offset 0x24bb
    call      Func0x2658                ;Offset 0x2658
Label0x24bb:                            ;Offset 0x24bb
    pop       dx
    jmp       Label0x2485               ;Offset 0x2485
SaveRestoreVideoState ENDP

Func0x24be PROC NEAR                    ;Offset 0x24be
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
    call      Func0x28e2                ;Offset 0x28e2
    mov       dl, VGA_MiscellaneousRead_lowbyte;Port 0x3cc
    in        al, dx
    stosb
    mov       dx, si
    mov       cl, 19h
    call      Func0x28e0                ;Offset 0x28e0
    mov       ah, 00h
    mov       cl, 14h
Label0x24f8:                            ;Offset 0x24f8
    call      Func0x2b21                ;Offset 0x2b21
    stosb
    inc       ah
    loop      Label0x24f8               ;Offset 0x24f8
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    call      Func0x28e0                ;Offset 0x28e0
    mov       ax, si
    stosw
    xchg      ax, dx
    mov       al, 22h
    out       dx, al
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, (VGA_SEQ4_MemSize256k SHL 8) OR VGA_SEQIdx_MemoryMode;0x204
    out       dx, ax
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    out       dx, al
    in        ax, dx
    push      ax
    and       ah, NOT (VGA_GCTL5_OddEven OR VGA_GCTL5_ReadColCmp);0xe7
    out       dx, ax
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    out       dx, al
    inc       dx
    inc       si
    mov       al, 00h
Label0x2526:                            ;Offset 0x2526
    out       dx, al
    push      ax
    xchg      dx, si
    in        al, dx
    stosb
    xchg      dx, si
    pop       ax
    inc       ax
    cmp       al, 03h
    jbe       Label0x2526               ;Offset 0x2526
    mov       al, byte ptr es:[di - 0bh]
    out       dx, al
    mov       dl, VGA_SequenceData_lowbyte;Port 0x3c5
    mov       al, byte ptr es:[di - 3eh]
    out       dx, al
    pop       ax
    mov       dx, VGA_GraphicsControllerIndex;0x3ce
    out       dx, ax
    pop       cx
    ret
Func0x24be ENDP

Func0x2547 PROC NEAR                    ;Offset 0x2547
    mov       word ptr es:[bx + 2], di
Func0x2547 ENDP
;continue!
BackupBiosDataAreaVideoSections PROC NEAR;Offset 0x254b
    push      cx
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    lea       si, [BiosDataAreaVideoSections];Offset 0x2566
ProcessNextSection:                     ;Offset 0x2556
    lodsw     word ptr cs:[si]          ;Count
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2563
    lodsw     word ptr cs:[si]          ;Source address
    xchg      ax, si
    rep movsb
    xchg      ax, si
    jmp       ProcessNextSection        ;Offset 0x2556
Return:                                 ;Offset 0x2563
    pop       cx
    ret       
BackupBiosDataAreaVideoSections ENDP

    nop                                 ;https://i.imgur.com/NGkg1Ip.jpeg

;Offset 0x2566
BiosDataAreaVideoSections   DW 001eh, BDA_DisplayMode;Offset 0x449
                            DW 0007h, BDA_RowsOnScreen;Offset 0x484
                            DW 0004h, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
                            DW 0004h, INT_5_HandlerOfs;Offset 0x014
                            DW 0004h, INT_1D_HandlerOfs;Offset 0x074
                            DW 0004h, INT_1F_HandlerOfs;Offset 0x07c
                            DW 0004h, INT_43_HandlerOfs;Offset 0x10c
                            DW 0000h

Func0x2584 PROC NEAR                    ;Offset 0x2584
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
    call      Func0x29f7                ;Offset 0x29f7
    pop       cx
    ret
Func0x2584 ENDP

Func0x25a9 PROC NEAR                    ;Offset 0x25a9
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
    call      Func0x2869                ;Offset 0x2869
    add       dl, 06h
    mov       al, byte ptr es:[si - 1fh]
    out       dx, al
    xor       ax, ax
    mov       cx, 0010h
    call      Func0x28ed                ;Offset 0x28ed
    inc       ax
    inc       si
    mov       cl, 01h
    call      Func0x28ed                ;Offset 0x28ed
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
Func0x25a9 ENDP

Func0x262a PROC NEAR                    ;Offset 0x262a
    mov       si, word ptr es:[bx + 02h]
Func0x262a ENDP
;continue!
RestoreBiosDataAreaVideoSections PROC NEAR;Offset 0x262e
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
    lea       dx, [BiosDataAreaVideoSections];Offset 0x2566
ProcessNextSection:                     ;Offset 0x2644
    xchg      si, dx
    lodsw     word ptr cs:[si]
    xchg      ax, cx
    jcxz      Return                    ;Offset 0x2654
    lodsw     word ptr cs:[si]
    xchg      ax, di
    xchg      si, dx
    rep movsb
    jmp       ProcessNextSection        ;Offset 0x2644
Return:                                 ;Offset 0x2654
    pop       es
    pop       ds
    pop       cx
    ret       
RestoreBiosDataAreaVideoSections ENDP

Func0x2658 PROC NEAR                    ;Offset 0x2658
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
    call      Func0x29c7                ;Offset 0x29c7
    sub       si, 0303h
    lodsw     word ptr es:[si]
    xchg      al, ah
    shr       ah, 01h
    sbb       dl, 01h
    out       dx, al
    pop       cx
    ret
Func0x2658 ENDP

Func0x2682 PROC NEAR                    ;Offset 0x2682
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    xor       dh, dh
    add       ax, dx
    shl       ax, 01h
    ret
Func0x2682 ENDP

Func0x2690 PROC NEAR                    ;Offset 0x2690
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    shl       ax, 01h
    shl       ax, 01h
    xor       dh, dh
    add       ax, dx
    ret
Func0x2690 ENDP

Func0x26a0 PROC NEAR                    ;Offset 0x26a0
    mov       dl, al
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ah
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    pop       dx
    xor       dh, dh
    add       ax, dx
    ret
Func0x26a0 ENDP

Func0x26b2 PROC NEAR                    ;Offset 0x26b2
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
Func0x26b2 ENDP

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
SetCursorEndStart PROC NEAR             ;Offset 0x26ca
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

ClearScreen PROC NEAR                   ;Offset 0x26d9
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487 0x80
    jne       Label0x2716               ;Offset 0x2716
    cmp       word ptr ds:[BDA_VideoBufferSize], 00h;Offset 0x44c
    je        Label0x2716               ;Offset 0x2716
    push      bx
    push      cx
    push      dx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x61e2                ;Offset 0x61e2
    mov       bx, 0a000h                ;Segment 0xa000
    mov       cx, 8000h
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x2709               ;Offset 0x2709
    mov       bh, 0b0h                  ;Segment 0xb000
    mov       ch, 40h
    mov       dx, 0720h                 ;Attribute + space
    je        Label0x270b               ;Offset 0x270b
    mov       bh, 0b8h                  ;Segment 0xb800
    cmp       al, BDA_DM_80x25_16_Color_Text;0x3
    jbe       Label0x270b               ;Offset 0x270b
Label0x2709:                            ;Offset 0x2709
    xor       dx, dx
Label0x270b:                            ;Offset 0x270b
    mov       es, bx
    mov       ax, dx
    xor       di, di
    rep stosw
    pop       dx
    pop       cx
    pop       bx
Label0x2716:                            ;Offset 0x2716
    ret
ClearScreen ENDP

Func0x2717 PROC NEAR                    ;Offset 0x2717
    lea       si, [Data0x2742]          ;Offset 0x2742
    jmp       Func0x2721                ;Offset 0x2721
Func0x2717 ENDP

Func0x271d PROC NEAR                    ;Offset 0x271d
    lea       si, [Data0x274c]          ;Offset 0x274c
Func0x271d ENDP
;continue!
Func0x2721 PROC NEAR                    ;Offset 0x2721
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
Label0x2734:                            ;Offset 0x2734
    lodsw     word ptr cs:[si]
    out       dx, ax
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       ax, 0eb4h
    je        Label0x2734               ;Offset 0x2734
    pop       dx
    pop       ax
    ret
Func0x2721 ENDP

;Offset 0x2742
Data0x2742              DW (VGA_SEQ2_Mask2 SHL 8) OR VGA_SEQIdx_MapMask;0x0402
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0704
                        DW (VGA_GCTL4_Map2 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0204
                        DW (VGA_GCTL5_BLU SHL 8) OR VGA_GCTLIdx_GraphicMode;0x0005
                        DW (VGA_GCTL6_Mem_A0000_AFFFF SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0406

;Offset 0x274c
Data0x274c              DW ((VGA_SEQ2_Mask0 OR VGA_SEQ2_Mask1) SHL 8)OR VGA_SEQIdx_MapMask;0x0302
                        DW ((VGA_SEQ4_Unknown1 OR VGA_SEQ4_MemSize256k) SHL 8) OR VGA_SEQIdx_MemoryMode;0x0304
                        DW (VGA_GCTL4_Map0 SHL 8) OR VGA_GCTLIdx_ReadMapSelect;0x0004
                        DW (VGA_GCTL5_OddEven SHL 8) OR VGA_GCTLIdx_GraphicMode;0x1005
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B8000_BFFFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0e06
                        DW ((VGA_GCTL6_AlphaMode OR VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B0000_B7FFF) SHL 8) OR VGA_GCTLIdx_Miscellaneous;0x0a06

Func0x2758 PROC NEAR                    ;Offset 0x2758
    cbw
    dec       ax
    js        Func0x277b                ;Offset 0x277b
    mov       cx, 0100h
    xor       dx, dx
    push      cs
    pop       es
    lea       bp, [Font8x16]            ;Offset 0x3960
    mov       bh, 00h
    dec       ax
    js        Func0x277b                ;Offset 0x277b
    lea       bp, [Font8x8]             ;Offset 0x3160
    mov       bh, 08h
    dec       ax
    js        Func0x277b                ;Offset 0x277b
    lea       bp, [Font8x16]            ;Offset 0x3960
    mov       bh, 10h
Func0x2758 ENDP
;continue!
Func0x277b PROC NEAR                    ;Offset 0x277b
    mov       si, bp
    push      es
    pop       ds
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    jcxz      Label0x27da               ;Offset 0x27da
Label0x2786:                            ;Offset 0x2786
    push      cx
    mov       cl, 05h
    shl       dx, cl
    push      bx
    and       bx, 0007h
    add       dh, byte ptr cs:[bx + Data0x27f2];Offset 0x27f2
    pop       bx
    mov       di, dx
    mov       dl, bh
    xor       dh, dh
    or        dl, dl
    jne       Label0x27a1               ;Offset 0x27a1
    mov       dl, 10h
Label0x27a1:                            ;Offset 0x27a1
    pop       ax
Label0x27a2:                            ;Offset 0x27a2
    or        bh, bh
    jne       Label0x27ce               ;Offset 0x27ce
    cmp       al, 99h
    je        Label0x27c0               ;Offset 0x27c0
    cmp       al, 96h
    je        Label0x27c0               ;Offset 0x27c0
    cmp       al, 87h
    je        Label0x27c0               ;Offset 0x27c0
    cmp       al, 90h
    je        Label0x27c0               ;Offset 0x27c0
    cmp       al, 8fh
    je        Label0x27c0               ;Offset 0x27c0
    cmp       al, 68h
    je        Label0x27c0               ;Offset 0x27c0
    jmp       Label0x27ce               ;Offset 0x27ce
Label0x27c0:                            ;Offset 0x27c0
    mov       cx, 0008h
    rep movsb
    inc       si
    mov       cx, 0007h
    rep movsb
    inc       di
    jmp       Label0x27d2               ;Offset 0x27d2
Label0x27ce:                            ;Offset 0x27ce
    mov       cx, dx
    rep movsb
Label0x27d2:                            ;Offset 0x27d2
    sub       di, dx
    add       di, 0020h
    dec       ax
    jne       Label0x27a2               ;Offset 0x27a2
Label0x27da:                            ;Offset 0x27da
    or        bl, bl
    jns       Label0x27e7               ;Offset 0x27e7
    mov       dl, byte ptr [si]
    inc       si
    inc       cx
    or        dl, dl
    jne       Label0x2786               ;Offset 0x2786
    dec       cx
Label0x27e7:                            ;Offset 0x27e7
    or        bh, bh
    jne       Label0x27ed               ;Offset 0x27ed
    mov       bh, 0eh
Label0x27ed:                            ;Offset 0x27ed
    push      ds
    pop       es
    mov       ds, cx
    ret
Func0x277b ENDP

;Offset 0x27f2
Data0x27f2              DB 000h, 040h, 080h, 0C0h, 020h, 060h, 0A0h, 0E0h

Func0x27fa PROC NEAR                    ;Offset 0x27fa
    mov       al, bh
    cbw
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    dec       ax
    mov       ah, al
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne       Label0x2811               ;Offset 0x2811
    mov       al, VGA_CRTCIdx_UnderlineLoc;0x14
    out       dx, ax
Label0x2811:                            ;Offset 0x2811
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
    jb        Label0x2826               ;Offset 0x2826
    sub       ax, 0101h
Label0x2826:                            ;Offset 0x2826
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    call      SetCursorEndStart         ;Offset 0x26ca
    call      GetNumberOfActiveScanlines;Offset 0x2a88
    dec       al
    mov       ax, 00c8h
    js        Label0x283e               ;Offset 0x283e
    mov       ax, 015eh
    je        Label0x283e               ;Offset 0x283e
    mov       ax, 0190h
Label0x283e:                            ;Offset 0x283e
    div       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec       ax
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    inc       ax
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp       ax, 00c8h
    ja        Label0x2852               ;Offset 0x2852
    shl       ax, 01h
Label0x2852:                            ;Offset 0x2852
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
Func0x27fa ENDP

Func0x2869 PROC NEAR                    ;Offset 0x2869
    push      cx
    push      dx
    jmp       Func0x2879                ;Offset 0x2879
Func0x2869 ENDP

Func0x286d PROC NEAR                    ;Offset 0x286d
    push      cx
    push      dx
    call      Func0x2a18                ;Offset 0x2a18
    add       si, 0005h
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
Func0x286d ENDP
;continue!
Func0x2879 PROC NEAR                    ;Offset 0x2879
    mov       di, dx
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       cx, 0004h
    mov       ax, (VGA_SEQ0_AsyncReset SHL 8) OR VGA_SEQIdx_Reset;0x100
    cli
    out       dx, ax
    inc       ax
    call      Func0x28d7                ;Offset 0x28d7
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    lodsb     byte ptr es:[si]
    out       dx, al
    push      cx
    mov       cx, 8000h
Label0x2893:                            ;Offset 0x2893
    loop      Label0x2893               ;Offset 0x2893  Speed sensitive!
    pop       cx
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8) OR VGA_SEQIdx_Reset;0x300
    out       dx, ax
    sti       
    call      TurnScreenOff             ;Offset 0x2b3d
    push      ax
    mov       dx, di
    mov       ax, VGA_CRTCIdx_VertRetraceEnd;0x11
    out       dx, ax
    mov       cl, 19h
    xor       ax, ax
    call      Func0x28d7                ;Offset 0x28d7
    push      si
    add       dl, 06h                   ;Port 0x3ca
    in        al, dx
    xor       ax, ax
    out       dx, al
    mov       al, 10h
    add       si, ax
    mov       cl, 01h
    call      Func0x28ed                ;Offset 0x28ed
    inc       ax
    inc       si
    mov       cl, 02h
    call      Func0x28ed                ;Offset 0x28ed
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cl, 09h
    xor       ax, ax
    call      Func0x28d7                ;Offset 0x28d7
    pop       si
    pop       ax
    call      ToggleScreenOnOff         ;Offset 0x2b3f
    pop       dx
    pop       cx
    ret
Func0x2879 ENDP

Func0x28d7 PROC NEAR                    ;Offset 0x28d7
Label0x28d7:                            ;Offset 0x28d7
    mov       ah, byte ptr es:[si]
    inc       si
    out       dx, ax
    inc       ax
    loop      Label0x28d7               ;Offset 0x28d7
    ret
Func0x28d7 ENDP

Func0x28e0 PROC NEAR                    ;Offset 0x28e0
    mov       al, 00h
Func0x28e0 ENDP
;continue!
Func0x28e2 PROC NEAR                    ;Offset 0x28e2
Label0x28e2:                            ;Offset 0x28e2
    out       dx, al
    push      ax
    inc       dx
    in        al, dx
    dec       dx
    stosb
    pop       ax
    inc       ax
    loop      Label0x28e2               ;Offset 0x28e2
    ret
Func0x28e2 ENDP

Func0x28ed PROC NEAR                    ;Offset 0x28ed
    mov       ah, 0ffh
Func0x28ed ENDP
;continue!
Func0x28ef PROC NEAR                    ;Offset 0x28ef
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
Label0x28f2:                            ;Offset 0x28f2
    ;cmp       al, ah
    DB 3Ah, 0C4h                        ;Misassemble!
    jb        Label0x28fb               ;Offset 0x28fb
    ja        Label0x2904               ;Offset 0x2904
    inc       ax
    mov       cl, 01h
Label0x28fb:                            ;Offset 0x28fb
    out       dx, al
    push      ax
    lodsb     byte ptr es:[si]
    out       dx, al
    pop       ax
    inc       ax
    loop      Label0x28f2               ;Offset 0x28f2
Label0x2904:                            ;Offset 0x2904
    ret
Func0x28ef ENDP

Func0x2905 PROC NEAR                    ;Offset 0x2905
    mov       ah, 0ffh
    push      cx
    push      dx
    mov       al, 00h
    mov       cx, 0010h
    call      Func0x2c00                ;Offset 0x2c00
Label0x2911:                            ;Offset 0x2911
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
    js        Label0x292a               ;Offset 0x292a
    jne       Label0x2911               ;Offset 0x2911
    add       ax, 0101h
    jb        Label0x2911               ;Offset 0x2911
    inc       di
    jmp       Label0x2911               ;Offset 0x2911
Label0x292a:                            ;Offset 0x292a
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    pop       cx
    ret
Func0x2905 ENDP

Func0x2933 PROC NEAR                    ;Offset 0x2933
    push      ds
    push      es
    push      si
    mov       al, BDA_VPCB_DynamicParamSaveArea;0x4
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    pop       si
    pop       ds
    je        Label0x2946               ;Offset 0x2946
    mov       cx, 0008h
    rep movsw
    inc       si
    movsb
Label0x2946:                            ;Offset 0x2946
    pop       ds
    ret       
Func0x2933 ENDP

Func0x2948 PROC NEAR                    ;Offset 0x2948
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, 0ffh
    out       dx, al
    push      cs
    pop       es
    call      GetNumberOfActiveScanlines;Offset 0x2a88
    mov       ah, al
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    ;cmp       ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 3Dh, 13h, 00h                    ;Misassemble!
    je        Label0x2977               ;Offset 0x2977
    lea       si, [Data0x2dba]          ;Offset 0x2dba
    jb        Label0x2973               ;Offset 0x2973
    lea       si, [Data0x2e42]          ;Offset 0x2e42
    cmp       al, BDA_DM_80x25_Monochrome_Text;0x7
    je        Label0x2973               ;Offset 0x2973
    cmp       al, BDA_DM_640x350_Monochrome_Graphics;0xf
    je        Label0x2973               ;Offset 0x2973
    lea       si, [Data0x2d32]          ;Offset 0x2d32
Label0x2973:                            ;Offset 0x2973
    call      Func0x2991                ;Offset 0x2991
    ret
Label0x2977:                            ;Offset 0x2977
    lea       si, [Data0x2e86]          ;Offset 0x2e86
    call      Func0x2991                ;Offset 0x2991
    lea       si, [Data0x2eae]          ;Offset 0x2eae
    mov       al, 10h
    call      Func0x2993                ;Offset 0x2993
    lea       si, [Data0x2ec2]          ;Offset 0x2ec2
    mov       al, 20h
    call      Func0x2993                ;Offset 0x2993
    ret
Func0x2948 ENDP

Func0x2991 PROC NEAR                    ;Offset 0x2991
    xor       al, al
Func0x2991 ENDP
;continue!
Func0x2993 PROC NEAR                    ;Offset 0x2993
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    inc       dx
    mov       ax, word ptr es:[si - 04h]
    mov       cx, word ptr es:[si - 02h]
    test      byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    je        Label0x29af               ;Offset 0x29af
    add       si, cx
    add       si, 0004h
    mov       ax, word ptr es:[si - 04h]
Label0x29af:                            ;Offset 0x29af
    or        ah, ah
    je        Label0x29c1               ;Offset 0x29c1
    jns       Func0x29c7                ;Offset 0x29c7
Label0x29b5:                            ;Offset 0x29b5
    lodsb     byte ptr es:[si]
    out       dx, al
    jmp       Label0x29ba               ;Offset 0x29ba Speed sensitive!
Label0x29ba:                            ;Offset 0x29ba
    out       dx, al
    jmp       Label0x29bd               ;Offset 0x29bd Speed sensitive!
Label0x29bd:                            ;Offset 0x29bd
    out       dx, al
    loop      Label0x29b5               ;Offset 0x29b5
    ret
Label0x29c1:                            ;Offset 0x29c1
    push      bx
    call      Func0x2bcf                ;Offset 0x2bcf
    pop       bx
    ret
Func0x2993 ENDP

Func0x29c7 PROC NEAR                    ;Offset 0x29c7
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    jne       Label0x29da               ;Offset 0x29da
Label0x29ce:                            ;Offset 0x29ce
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    lodsb     byte ptr es:[si]
    out       dx, al
    loop      Label0x29ce               ;Offset 0x29ce
    ret
Label0x29da:                            ;Offset 0x29da
    push      cx
    push      dx
    mov       dh, byte ptr es:[si]
    inc       si
    mov       ch, byte ptr es:[si]
    inc       si
    mov       cl, byte ptr es:[si]
    inc       si
    call      Func0x2a00                ;Offset 0x2a00
    pop       dx
    pop       cx
    out       dx, al
    jmp       Label0x29f0               ;Offset 0x29f0 Speed sensitive!
Label0x29f0:                            ;Offset 0x29f0
    out       dx, al
    jmp       Label0x29f3               ;Offset 0x29f3 Speed sensitive!
Label0x29f3:                            ;Offset 0x29f3
    out       dx, al
    loop      Label0x29da               ;Offset 0x29da
    ret       
Func0x29c7 ENDP

Func0x29f7 PROC NEAR                    ;Offset 0x29f7
Label0x29f7:                            ;Offset 0x29f7
    in        al, dx
    stosb
    in        al, dx
    stosb
    in        al, dx
    stosb
    loop      Label0x29f7               ;Offset 0x29f7
    ret
Func0x29f7 ENDP

Func0x2a00 PROC NEAR                    ;Offset 0x2a00
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
Func0x2a00 ENDP

Func0x2a18 PROC NEAR                    ;Offset 0x2a18
    mov       al, BDA_VPCB_VideoParameterTable;0x0
    call      LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cbw
    shl       ax, 01h
    shl       ax, 01h
    mov       di, ax
    call      GetNumberOfActiveScanlines;Offset 0x2a88
    add       di, ax
    mov       al, byte ptr cs:[di + Data0x2a38];Offset 0x2a38
    mov       ah, 40h
    mul       ah
    add       si, ax
    ret
Func0x2a18 ENDP

;
;                           +------------------------ 200 Scan lines
;                           |
;                           |     +------------------ 350 Scan lines
;                           |     |
;                           |     |     +------------ 400 Scan lines
;                           |     |     |
;                           |     |     |     +------ 480 Scan lines
;                           |     |     |     |
;Offset 0x2a38              |     |     |     |       Display mode:
Data0x2a38              DB 000h, 013h, 017h, 000h   ; 0x00
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
GetNumberOfActiveScanlines PROC NEAR    ;Offset 0x2a88
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push  bx
    lea   bx, [NumberOfActiveScanlinesPerModePlusOne];Offset 0x2ab4
    cmp   al, BDA_DM_320x200_256_Color_Graphics;0x13
    xlatb cs:[bx]
    ja    Label0x2a9a                   ;Offset 0x2a9a
    dec   al                            ;Subtract one to see if it turns negative (and thus, not set)
    jns   Label0x2ab1                   ;Offset 0x2ab1
Label0x2a9a:                            ;Offset 0x2a9a
    mov   al, 02h                       ;400 Scan lines
    test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    jne   Label0x2ab1                   ;Offset 0x2ab1
    dec   ax                            ;350 Scan lines
    call  IsMDAHiResEnhanced            ;Offset 0x2ac8
    je    Label0x2ab1                   ;Offset 0x2ab1
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    je    Label0x2ab1                   ;Offset 0x2ab1
    dec   ax                            ;200 Scan lines
Label0x2ab1:                            ;Offset 0x2ab1
    pop   bx
    ret
GetNumberOfActiveScanlines ENDP

    nop                                 ;hello

;
;
;                                               000h means it's determined another way
;                                               All others are decremented by one and returned.
;Offset 0x2ab4                                    Display mode:
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

IsMDAHiResEnhanced PROC NEAR            ;Offset 0x2ac8
    push  ax
    mov   al, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    and   al, BDA_EFBS_AdapterTypeMask  ;0xf
    cmp   al, BDA_EFBS_MDAHiResEnhanced ;0x3
    je    Found                         ;Offset 0x2ad4
    cmp   al, BDA_EFBS_MDAHiResEnhanced_2;0x9 Also check the other one
Found:                                  ;Offset 0x2ad4
    pop   ax
    ret
IsMDAHiResEnhanced ENDP

Func0x2ad6 PROC NEAR                    ;Offset 0x2ad6
    push  ax
    mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call  Func0x61e2                    ;Offset 0x61e2
    cmp   al, BDA_DM_80x25_Monochrome_Text;0x7
    je    Label0x2ae7                   ;Offset 0x2ae7
    cmp   al, BDA_DM_80x25_16_Color_Text;0x3
    ja    Label0x2ae7                   ;Offset 0x2ae7
    xor   al, al
Label0x2ae7:                            ;Offset 0x2ae7
    pop   ax
    ret   
Func0x2ad6 ENDP

Func0x2ae9 PROC NEAR                    ;Offset 0x2ae9
    push  si
    cbw
    xchg  ax, si
    add   si, di
Label0x2aee:                            ;Offset 0x2aee
    lodsb byte ptr es:[si]
    cmp   al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    je    Label0x2afb                   ;Offset 0x2afb
    inc   al
    jne   Label0x2aee                   ;Offset 0x2aee
    inc   ax
Label0x2afb:                            ;Offset 0x2afb
    pop   si
    ret
Func0x2ae9 ENDP

Func0x2afd PROC NEAR                    ;Offset 0x2afd
    push  dx
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
    in    al, dx
    mov   dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov   al, VGA_ATTR_PaletteAddressSource;0x20
    out   dx, al
    pop   dx
    ret
Func0x2afd ENDP

Func0x2b0d PROC NEAR                    ;Offset 0x2b0d
    push  ax
    push  dx
    call  Func0x2c00                    ;Offset 0x2c00
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
Func0x2b0d ENDP

Func0x2b21 PROC NEAR                    ;Offset 0x2b21
    push  dx
    call  Func0x2c00                    ;Offset 0x2c00
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
Func0x2b21 ENDP

TurnScreenOn PROC NEAR                  ;Offset 0x2b39
    mov   ah, 00h
    jmp   ToggleScreenOnOff             ;Offset 0x2b3f
TurnScreenOn ENDP

TurnScreenOff PROC NEAR                 ;Offset 0x2b3d
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
ToggleScreenOnOff PROC NEAR             ;Offset 0x2b3f
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
SetDisplayCombinationCodeIndex PROC NEAR;Offset 0x2b54
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    NotFound                      ;Offset 0x2b74
    xor   dx, dx
    xor   cx, cx
    mov   cl, byte ptr es:[si]
    add   si, sizeof DisplayCombinationCodeTable;0x4 Skip display combination code header
Find:                                   ;Offset 0x2b65
    lodsw word ptr es:[si]
    cmp   ax, bx
    je    Found                         ;Offset 0x2b76
    xchg  al, ah
    cmp   ax, bx
    je    Found                         ;Offset 0x2b76
    inc   dx
    loop  Find                          ;Offset 0x2b65
NotFound:                               ;Offset 0x2b74
    mov   dl, 0ffh                      ;Entry not found set index to -1
Found:                                  ;Offset 0x2b76
    mov   byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], dl;Offset 0x48a
    ret
SetDisplayCombinationCodeIndex ENDP

GetDisplayCombinationCode PROC NEAR     ;Offset 0x2b7b
    push  di
    push  es
    mov   bx, 0ffffh
    mov   al, BDA_VPCB_DisplayCombinationCodePtr;0x2
    call  LookupVideoParameterControlBlockPointer;Offset 0x2bb1
    je    Label0x2bae                   ;Offset 0x2bae
    mov   al, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    cmp   al, byte ptr es:[si]
    jae   Label0x2bae                   ;Offset 0x2bae
    cbw
    shl   ax, 01h
    xchg  ax, bx
    mov   ax, word ptr es:[bx + si + 04h]
    mov   bx, ax
    cmp   al, 00h
    je    Label0x2bac                   ;Offset 0x2bac
    test  byte ptr ds:[BDA_DetectedHardware], BDA_DH_InitialVideoModeMask;Offset 0x410 0x30
    je    Label0x2ba6                   ;Offset 0x2ba6
    jp    Label0x2ba8                   ;Offset 0x2ba8
Label0x2ba6:                            ;Offset 0x2ba6
    xor   al, 01h
Label0x2ba8:                            ;Offset 0x2ba8
    test  al, 01h
    jne   Label0x2bae                   ;Offset 0x2bae
Label0x2bac:                            ;Offset 0x2bac
    xchg  bl, bh
Label0x2bae:                            ;Offset 0x2bae
    pop   es
    pop   di
    ret
GetDisplayCombinationCode ENDP

LookupVideoParameterControlBlockPointer PROC NEAR;Offset 0x2bb1
    cbw
    les   si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8    es:si = Video Save/Override Pointer table
    test  al, 02h                       ;Test if al bit 2 is set.
    je    DereferencePointerWithOffset  ;Offset 0x2bc3  if not set, then lookup in primary table
    push  ax                            ;else save ax
    mov   al, BDA_VPCB_SecondarySavePointerTbl;0x10 load offset 0x10 instead (secondary save pointer)
    call  DereferencePointerWithOffset  ;Offset 0x2bc3
    pop   ax                            ;restore ax
    je    Func0x2bce                    ;Offset 0x2bce  if previous call yielded null, return else call with original value
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
DereferencePointerWithOffset PROC NEAR  ;Offset 0x2bc3
    add   si, ax                        ;add ax offset to video save/override pointer
    les   si, es:[si]                   ;load es:si from table
    mov   di, si                        ;store si in di
    mov   ax, es                        ;store es in ax
    or    ax, si                        ;or ax and si (set flags)
DereferencePointerWithOffset ENDP
;continue!
Func0x2bce PROC NEAR                    ;Offset 0x2bce
    ret
Func0x2bce ENDP

Func0x2bcf PROC NEAR;                   ;Offset 0x2bcf
Label0x2bcf:                            ;Offset 0x2bcf
    lodsb byte ptr es:[si]
    mov   ah, al
    mov   bx, 0003h
Label0x2bd6:                            ;Offset 0x2bd6
    shl   ah, 01h
    shl   ah, 01h
    xor   al, al
    test  ah, 0c0h
    je    Label0x2beb                   ;Offset 0x2beb
    jns   Label0x2be9                   ;Offset 0x2be9
    jnp   Label0x2be7                   ;Offset 0x2be7
    add   al, 15h
Label0x2be7:                            ;Offset 0x2be7
    add   al, 15h
Label0x2be9:                            ;Offset 0x2be9
    add   al, 15h
Label0x2beb:                            ;Offset 0x2beb
    out   dx, al
    dec   bx
    jne   Label0x2bd6                   ;Offset 0x2bd6
    loop  Label0x2bcf                   ;Offset 0x2bcf
    ret
Func0x2bcf ENDP

Func0x2bf2 PROC NEAR                    ;Offset 0x2bf2
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2bfa:                            ;Offset 0x2bfa
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne   Label0x2bfa                   ;Offset 0x2bfa
    pop   ax
Func0x2bf2 ENDP
;continue!
Func0x2c00 PROC NEAR                    ;Offset 0x2c00
    push  ax
    mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add   dl, 06h                       ;Port 0x3?a
Label0x2c08:                            ;Offset 0x2c08
    sti
    nop
    cli
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x2c08                   ;Offset 0x2c08
    pop   ax
    ret
Func0x2c00 ENDP

;
;inputs:
;   al * 8 * 1193 ticks time
;
Beep PROC NEAR                          ;Offset 0x2c12
    mov   ah, al
    in    al, KB_PortB                  ;Port 0x61
    push  ax
    or    al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out   KB_PortB, al                  ;Port 0x61
    mov   al, ah
    call  Sleep2                        ;Offset 0x2c24
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
Sleep2 PROC NEAR                        ;Offset 0x2c24
    call  SetupSquareWave               ;Offset 0x2c64
    push  ax
    push  cx
    push  dx
    push  ds
    mov   dx, SYS_Segment               ;Segment 0xf000
    mov   ds, dx
    mov   dx, KB_CommandPort_XT         ;0x62
    cmp   byte ptr ds:[SYS_MachineID], SYS_MachineID_IBM_AT;Offset 0xfffe 0xfc
    jne   NotAT                         ;Offset 0x2c3b
    dec   dx
NotAT:                                  ;Offset 0x2c3b
    pop   ds
    mov   cl, al
    xor   ch, ch
    shl   cx, 01h
    shl   cx, 01h
    shl   cx, 01h
    jcxz  NoSleep                       ;Offset 0x2c60
    in    al, KB_PortB                  ;Port 0x61
    mov   ah, al                        ;store off original value
    or    al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out   KB_PortB, al                  ;Port 0x61
WaitTimeExpired:                        ;Offset 0x2c50
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    jne   WaitTimeExpired               ;Offset 0x2c50
WaitAgain:                              ;Offset 0x2c55
    in    al, dx
    and   al, KB_PBR_MirrorTimer2OutputCond;0x20
    je    WaitAgain                     ;Offset 0x2c55
    loop  WaitTimeExpired               ;Offset 0x2c50
    mov   al, ah                        ;restore original value
    out   KB_PortB, al                  ;Port 0x61
NoSleep:                                ;Offset 0x2c60
    pop   dx
    pop   cx
    pop   ax
    ret
Sleep2 ENDP

SetupSquareWave PROC NEAR               ;Offset 0x2c64
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
    xchg bx, bx

PrintScreenHandler:                     ;Offset 0x2c80
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
    je   Label0x2cd4                    ;Offset 0x2cd4
    mov  ah, 0fh
    int  10h
    mov  bl, ah
    mov  ah, 03h
    int  10h
    push dx
    call PrintCRLF                      ;Offset 0x2cda
    mov  cl, 0ffh
    xor  dx, dx
Label0x2ca4:                            ;Offset 0x2ca4
    mov  ah, 02h
    int  10h
    mov  ah, 08h
    int  10h
    or   al, al
    jne  Label0x2cb2                    ;Offset 0x2cb2
    mov  al, 20h
Label0x2cb2:                            ;Offset 0x2cb2
    call PrintCharacter                 ;Offset 0x2ce1
    jne  Label0x2ccb                    ;Offset 0x2ccb
    inc  dx
    ;cmp  dl, bl
    DB 3Ah, 0D3h                        ;Misassemble!
    jb   Label0x2ca4                    ;Offset 0x2ca4
    call PrintCRLF                      ;Offset 0x2cda
    xor  dl, dl
    inc  dh
    cmp  dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe  Label0x2ca4                    ;Offset 0x2ca4
    xor  cl, cl
Label0x2ccb:                            ;Offset 0x2ccb
    pop  dx
    mov  ah, 02h
    int  10h
    mov  byte ptr ds:[BDA_PrintScreenStatus], cl;Offset 0x500
Label0x2cd4:                            ;Offset 0x2cd4
    pop  ds
    pop  dx
    pop  cx
    pop  bx
    pop  ax
    iret

PrintCRLF PROC NEAR                     ;Offset 0x2cda
    mov  al, 0dh
    call PrintCharacter                 ;Offset 0x2ce1
    mov  al, 0ah
PrintCRLF ENDP
;continue!
PrintCharacter PROC NEAR                ;Offset 0x2ce1
    push dx
    xor  dx, dx
    xor  ah, ah
    int  17h
    test ah, 29h
    pop  dx
    ret
PrintCharacter ENDP

    nop                                 ;hello

;Offset 0x2cee
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d2e                        
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d32
Data0x2d32              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 028h, 02Ah, 001h, 003h, 009h, 00Bh, 021h, 023h, 029h, 02Bh
                        DB 004h, 006h, 00Ch, 00Eh, 024h, 026h, 02Ch, 02Eh, 005h, 007h, 00Dh, 00Fh, 025h, 027h, 02Dh, 02Fh
                        DB 010h, 012h, 018h, 01Ah, 030h, 032h, 038h, 03Ah, 011h, 013h, 019h, 01Bh, 031h, 033h, 039h, 03Bh
                        DB 014h, 016h, 01Ch, 01Eh, 034h, 036h, 03Ch, 03Eh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2d72
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2d76
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 025h, 028h, 002h, 007h, 01Bh, 020h, 00Fh, 014h, 028h, 02Ch
                        DB 00Ch, 011h, 025h, 02Ah, 014h, 01Eh, 032h, 036h, 00Fh, 013h, 027h, 02Ch, 01Bh, 020h, 034h, 039h
                        DB 006h, 00Bh, 01Fh, 024h, 013h, 018h, 02Ch, 030h, 009h, 00Dh, 021h, 026h, 015h, 01Ah, 02Eh, 033h
                        DB 013h, 017h, 02Bh, 030h, 01Fh, 024h, 038h, 03Dh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2db6
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2dba
Data0x2dba              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
                        DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah
                        DB 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2dfa
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0040h        ;count (below)
;Offset 0x2dfe
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2e3e
                        DW 0000h        ;BDA_VideoDisplayDataArea 0 = terminate
                        DW 0040h        ;count (below)
;Offset 0x2e42
Data0x2e42              DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah
                        DB 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
;Offset 0x2e82
                        DW 0006h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e86
Data0x2e86              DB 000h, 002h, 008h, 00Ah, 020h, 022h, 024h, 02Ah, 015h, 017h, 01Dh, 01Fh, 035h, 037h, 03Dh, 03Fh
;Offset 0x2e96
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2e9a
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 028h, 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
;Offset 0x2eaa
                        DW 8000h        ;BDA_VideoDisplayDataArea
                        DW 0010h        ;count (below)
;Offset 0x2eae
Data0x2eae              DB 000h, 005h, 008h, 00Bh, 00Eh, 011h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Dh, 032h, 038h, 03Fh
;Offset 0x2ebe
                        DW 4000h        ;BDA_VideoDisplayDataArea
                        DW 00D8h        ;count (below) RGB, 0xd8 * 3
;Offset 0x2ec2
Data0x2ec2              DB 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 03Fh, 03Fh
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

;Offset 0x314a
RowsOnScreen            DB 000h, 00Eh, 019h, 02Bh

;Offset 0x314e
Data0x314e              DW offset Font8x16;Offset 0x3960
                        DW offset Font8x8;Offset 0x3160
                        DW offset GraphicsFont8x8;Offset 0x3560
                        DW offset Data0x4960;Offset 0x4960
                        DW offset Font8x16;Offset 0x3960
                        DW offset Data0x4960;Offset 0x4960

;Offset 0x315a
    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx

;Offset 0x3160
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

;Offset 0x3560
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

;Offset 0x3960
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

;Offset 0x4960
Data0x4960              DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h
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

;Offset 0x4aa4
                        DB 'DATE: 03/08/99', 000h
;Offset 0x4ab3
                        DB 'Revision: 0.22', 000h

VESAPowerManagement:                    ;Offset 0x4ac2
    cmp  bl, 00h
    jne  VPMNot0                        ;Offset 0x4ac9
    jmp  VPMGetCaps                     ;Offset 0x4adb
VPMNot0:                                ;Offset 0x4ac9
    cmp  bl, 01h
    jne  VPMNot1                        ;Offset 0x4ad0
    jmp  VPMSetDisplayPowerState        ;Offset 0x4b06
VPMNot1:                                ;Offset 0x4ad0
    cmp  bl, 02h
    jne  VPMNot2                        ;Offset 0x4ad7
    jmp  VPMGetDisplayPowerState        ;Offset 0x4ae3
VPMNot2:                                ;Offset 0x4ad7
    mov  ax, 014fh
    iret
VPMGetCaps:                             ;Offset 0x4adb
    mov  ax, 004fh
    mov  bh, 07h
    mov  bl, 10h
    iret
VPMGetDisplayPowerState:                ;Offset 0x4ae3
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop  
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    in   al, dx
    xor  bh, bh                         ;0 = Display is on
    and  al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
    je   Label0x4b01                    ;Offset 0x4b01
    inc  bh                             ;1 = Display in standby
    cmp  al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    je   Label0x4b01                    ;Offset 0x4b01
    inc  bh                             ;2 = Display is suspended
    cmp  al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    je   Label0x4b01                    ;Offset 0x4b01
    shl  bh, 01h                        ;4 = Display is off
Label0x4b01:                            ;Offset 0x4b01
    mov  ax, 004fh
    pop  dx
    iret
VPMSetDisplayPowerState:                ;Offset 0x4b06
    push dx
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop  
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    out  dx, al
    inc  dx
    mov  al, bh
    and  bh, 0fh
    je   Label0x4b1b                    ;Offset 0x4b1b
    and  bl, 07h
    je   Label0x4b4d                    ;Offset 0x4b4d
Label0x4b1b:                            ;Offset 0x4b1b
    in   al, dx
    and  al, NOT (MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff);0xcf
    or   bh, bh                         ;0 = Turn display on
    jne  Label0x4b2b                    ;Offset 0x4b2b
    out  dx, al
    call ScreenOn2                      ;Offset 0x4b65
    mov  ax, 004fh
    jmp  Label0x4b50                    ;Offset 0x4b50
Label0x4b2b:                            ;Offset 0x4b2b
    cmp  bh, 01h                        ;1 = Set display to standby
    jne  Label0x4b34                    ;Offset 0x4b34
    or   al, MGA_CRTCEXT1_HorSyncOff    ;0x10
    jmp  Label0x4b44                    ;Offset 0x4b44
Label0x4b34:                            ;Offset 0x4b34
    cmp  bh, 02h                        ;2 = Suspend display
    jne  Label0x4b3d                    ;Offset 0x4b3d
    or   al, MGA_CRTCEXT1_VerSyncOff    ;0x20
    jmp  Label0x4b44                    ;Offset 0x4b44
Label0x4b3d:                            ;Offset 0x4b3d
    cmp  bh, 04h                        ;4 = Turn display off
    jne  Label0x4b4d                    ;Offset 0x4b4d
    or   al, MGA_CRTCEXT1_HorSyncOff OR MGA_CRTCEXT1_VerSyncOff;0x30
Label0x4b44:                            ;Offset 0x4b44
    out  dx, al
    call ScreenOff2                     ;Offset 0x4b52
    mov  ax, 004fh
    jmp  Label0x4b50                    ;Offset 0x4b50
Label0x4b4d:                            ;Offset 0x4b4d
    mov  ax, 014fh
Label0x4b50:                            ;Offset 0x4b50
    pop  dx
    iret

ScreenOff2 PROC NEAR                    ;Offset 0x4b52
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

ScreenOn2 PROC NEAR                     ;Offset 0x4b65
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

;Offset 0x4b78
                        DB 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
                        DB 000h, 000h

;These two tables are order dependent (There's a pointer compare in one of the functions)

;Offset 0x4b82
DDCFuncs                DW offset DDCPin1Send1      ;Offset 0x4c90  ;0
                        DW offset DDCPin3Send1      ;Offset 0x4c74  ;2
                        DW offset DDCPin1Send0      ;Offset 0x4cdb  ;4
                        DW offset DDCPin3Send0      ;Offset 0x4cac  ;6
                        DW offset DDCPin1Read       ;Offset 0x4d0a  ;8
                        DW offset DDCPin3Read       ;Offset 0x4d30  ;a

;Offset 0x4b8e
MISCFuncs               DW offset MISCPin0Send1     ;Offset 0x4d72  ;0
                        DW offset MISCPin1Send1     ;Offset 0x4d56  ;2
                        DW offset MISCPin0Send0     ;Offset 0x4dbd  ;4
                        DW offset MISCPin1Send0     ;Offset 0x4d8e  ;6
                        DW offset MISCPin0Read      ;Offset 0x4dec  ;8
                        DW offset MISCPin1Read      ;Offset 0x4e12  ;a

VESADDC:                                ;Offset 0x4b9a
    cmp  bl, 00h
    je   DDCInstallationCheck           ;Offset 0x4bbd
    cmp  bl, 01h
    je   DDCReadEDID                    ;Offset 0x4bb4
    cmp  bl, 02h
    ;je   DDCReadVDIF                    ;Offset 0x4bb8
    DB 00Fh, 084h, 00Dh, 000h
    cmp  bl, 04h
    jbe  DDCReadEDID                    ;Offset 0x4bb4
    mov  ax, 014fh
    iret
DDCReadEDID:                            ;Offset 0x4bb4
    call Func0x4c17                     ;Offset 0x4c17
    iret
DDCReadVDIF:                            ;Offset 0x4bb8
    mov  al, 00h
    mov  ah, 01h
    iret
DDCInstallationCheck:                   ;Offset 
    push ds
    push es
    push dx
    push di
    cli
    push ss
    pop  es
    push bp
    mov  bp, sp
    sub  sp, 0100h
    mov  di, sp
    xor  dx, dx
    xor  cx, cx
    mov  bl, 01h
    call Func0x4c17                     ;Offset 0x4c17
    or   ah, ah
    ;jne  Label0x4bdf                    ;Offset 0x4bdf
    DB 00Fh, 085h, 003h, 000h
    or   cx, 0002h
Label0x4bdf:                            ;Offset 0x4bdf
    mov  bl, 03h
    call Func0x4c17                     ;Offset 0x4c17
    or   ah, ah
    ;jne  Label0x4bed                    ;Offset 0x4bed
    DB 00Fh, 085h, 003h, 000h
    or   cx, 0008h
Label0x4bed:                            ;Offset 0x4bed
    mov  bl, 04h
    call Func0x4c17                     ;Offset 0x4c17
    or   ah, ah
    ;jne  Label0x4bfb                    ;Offset 0x4bfb
    DB 00Fh, 085h, 003h, 000h
    or   cx, 0010h
Label0x4bfb:                            ;Offset 0x4bfb
    mov  bl, cl
    mov  bh, 01h
    or   bl, bl
    ;jne  Label0x4c07                    ;Offset 0x4c07
    DB 00Fh, 085h, 002h, 000h
    mov  bh, 00h
Label0x4c07:                            ;Offset 0x4c07
    mov  ah, 00h
    mov  al, 004fh
    mov  cx, 0200h
    mov  sp, bp
    pop  bp
    sti
    pop  di
    pop  dx
    pop  es
    pop  ds
    iret

Func0x4c17 PROC NEAR                    ;Offset 0x4c17
    push es
    push bx
    push cx
    push si
    mov  cl, bl
    cmp  dx, 0ffffh
    ;je   Label0x4c62                    ;Offset 0x4c62
    DB 00Fh, 084h, 03Eh, 000h
    push dx
    mov  si, offset DDCFuncs             ;Offset 0x4b82
    cmp  cl, 01h
    ;jne  Label0x4c3a                    ;Offset 0x4c3a
    DB 00Fh, 085h, 00Bh, 000h
    mov  ch, 0a0h
    mov  bx, 0080h
    shl  dx, 07h
    jmp  Label0x4c5b                    ;Offset 0x4c5b
    nop
Label0x4c3a:                            ;Offset 0x4c3a
    mov  bx, 0100h
    cmp  cl, 03h
    ;jne  Label0x4c55                    ;Offset 0x4c55
    DB 00Fh, 085h, 011h, 000h
    mov  ch, 0a4h
    cmp  dx, 0000h
    mov  dx, 0000h
    ;jne  Label0x4c5b                    ;Offset 0x4c5b
    DB 00Fh, 085h, 00Bh, 000h
    mov  ch, 0a2h
    jmp  Label0x4c5b                    ;Offset 0x4c5b
    nop
Label0x4c55:                            ;Offset 0x4c55
    mov  ax, 0014fh
    jmp  Label0x4c5e                    ;Offset 0x4c5e
    nop
Label0x4c5b:                            ;Offset 0x4c5b
    call Func0x50cc                     ;Offset 0x50cc
Label0x4c5e:                            ;Offset 0x4c5e
    pop  dx
    jmp  Label0x4c6f                    ;Offset 0x4c6f
    nop
Label0x4c62:                            ;Offset 0x4c62
    mov  dx, 0100h
    cmp  cl, 01h
    ;jne  Label0x4c6f                    ;Offset 0x4c6f
    DB 00Fh, 085h, 003h, 000h
    mov  dx, 0080h
Label0x4c6f:                            ;Offset 0x4c6f
    pop  si
    pop  cx
    pop  bx
    pop  es
    ret
Func0x4c17 ENDP

DDCPin3Send1 PROC NEAR                  ;Offset 0x4c74
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
DDCPin3Send1 ENDP

DDCPin1Send1 PROC NEAR                  ;Offset 0x4c90
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
DDCPin1Send1 ENDP

DDCPin3Send0 PROC NEAR                  ;Offset 0x4cac
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC3Data ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_GENIOCTRL_DDC3Enable   ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
DDCPin3Send0 ENDP

DDCPin1Send0 PROC NEAR                  ;Offset 0x4cdb
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC1Data ;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_GENIOCTRL_DDC1Enable   ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
DDCPin1Send0 ENDP

DDCPin1Read PROC NEAR                   ;Offset 0x4d0a
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC1Enable;0xfd
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    shl  cl, 07h
    pop  edx
    pop  ecx
    ret
DDCPin1Read ENDP

DDCPin3Read PROC NEAR                   ;Offset 0x4d30
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_DDC3Enable;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    shl  cl, 05h
    pop  edx
    pop  ecx
    ret
DDCPin3Read ENDP

MISCPin1Send1 PROC NEAR                 ;Offset 0x4d56
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
MISCPin1Send1 ENDP

MISCPin0Send1 PROC NEAR                 ;Offset 0x4d72
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
MISCPin0Send1 ENDP

MISCPin1Send0 PROC NEAR                 ;Offset 0x4d8e
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc1Data;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_GENIOCTRL_Misc1Enable  ;20h
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
MISCPin1Send0 ENDP

MISCPin0Send0 PROC NEAR                 ;Offset 0x4dbd
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc0Data;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_GENIOCTRL_Misc0Enable  ;0x10
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    pop  edx
    pop  ecx
    ret
MISCPin0Send0 ENDP

MISCPin0Read PROC NEAR                  ;Offset 0x4dec
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc0Enable;0xef
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    shl  cl, 04h
    pop  edx
    pop  ecx
    ret
MISCPin0Read ENDP

MISCPin1Read PROC NEAR                  ;Offset 0x4e12
    push ecx
    push edx
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENIOCTRL_Misc1Enable;0xdf
    mov  ch, cl
    mov  cl, MGA_INDD_GeneralPurposeIOControl;0x2a
    nop
    nop
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralPurposeIOData;0x2b
    nop
    nop
    call ReadIndexedRegister            ;Offset 0x578f
    shl  cl, 03h
    pop  edx
    pop  ecx
    ret
MISCPin1Read ENDP

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
SleepIfNeeded PROC NEAR                 ;Offset 0x4e38
    ;cmp   si, DDCFuncs                  ;Offset 0x4b82
    DB 081h, 0FEh, 082h, 04Bh
    ;ja    DontSleep                     ;Offset 0x4e43
    DB 00Fh, 087h, 003h, 000h
    call  Sleep                         ;Offset 0x4e44
DontSleep:                              ;Offset 0x4e43
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
Sleep PROC NEAR                         ;Offset 0x4e44
    push ax
    push bx
    push dx
    mov  ax, 0100h
    ;or   ax, 000fh
    DB 00Dh, 00Fh, 000h
    mov  bx, ax
    mov  al, PIT_MC_AccMode_LoByteHiByte OR PIT_MC_ChanSel_2 OR PIT_MC_OpMode_SquareWaveGenerator;0xb6
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, KB_PortB                   ;Port 0x61
    and  al, NOT (KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable);0xfc
    or   al, KB_PBW_Timer2GateSpeakerEnable;0x1
    out  KB_PortB, al                   ;Port 0x61
    mov  ax, bx
    out  PIT_Channel2Data, al           ;Port 0x42
    mov  al, ah
    out  PIT_Channel2Data, al           ;Port 0x42
    mov  ax, bx
    mov  bx, 8000h
Label0x4e68:                            ;Offset 0x4e68
    shl  ax, 01h
    jb   Label0x4e70                    ;Offset 0x4e70
    shr  bx, 01h
    jmp  Label0x4e68                    ;Offset 0x4e68
Label0x4e70:                            ;Offset 0x4e70
    mov  dl, PIT_MC_ChanSel_2 OR PIT_MC_OpMode_SquareWaveGenerator;0x86
Label0x4e72:                            ;Offset 0x4e72
    mov  al, dl
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, PIT_Channel2Data           ;Port 0x42
    mov  ah, al
    in   al, PIT_Channel2Data           ;Port 0x42
    xchg al, ah
    and  ax, bx
    jne  Label0x4e72                    ;Offset 0x4e72
Label0x4e82:                            ;Offset 0x4e82
    mov  al, dl
    out  PIT_ModeCommand, al            ;Port 0x43
    in   al, PIT_Channel2Data           ;Port 0x42
    mov  ah, al
    in   al, PIT_Channel2Data           ;Port 0x42
    xchg al, ah
    and  ax, bx
    je   Label0x4e82                    ;Offset 0x4e82
    in   al, KB_PortB                   ;Port 0x61
    and  al, NOT (KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable);0xfc
    out  KB_PortB, al                   ;Port 0x61
    pop  dx
    pop  bx
    pop  ax
    ret
Sleep ENDP

;Pin1: 0
;
Func0x4e9c PROC NEAR                    ;Offset 0x4e9c
    push ecx
    call word ptr cs:[si + MGA_DDCPin1Send0];0x6
    call SleepIfNeeded                  ;Offset 0x4e38
    pop  ecx
    ret
Func0x4e9c ENDP

;
;Pin0: 11r...00
;Pin1: x1r...x0
;
;Note: Reads until both Pin0 and Pin1 are 1
SendStartBit PROC NEAR                  ;Offset 0x4ea8
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 0ah
WaitFor1:                               ;Offset 0x4eb6
    call SleepIfNeeded                  ;Offset 0x4e38
    dec  cl
    je   TimeOut                        ;Offset 0x4ed4
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  WaitFor1                       ;Offset 0x4eb6
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jae  WaitFor1                       ;Offset 0x4eb6
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
TimeOut:                                ;Offset 0x4ed4
    mov  al, cl
    pop  ecx
    ret
SendStartBit ENDP

;
;Pin0: 0001
;Pin1: x011
;
SendStopBit PROC NEAR                   ;Offset 0x4ed9
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x4e38
    pop  ecx
    ret
SendStopBit ENDP

;
;Pin0: 1r...xxxxxx
;Pin1: xxxxx1r...0
;
WaitAck PROC NEAR                       ;Offset 0x4ef9
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    mov  cl, 0ah
Wait0:                                  ;Offset 0x4f00
    dec  cl
    je   TimeOut                        ;Offset 0x4f27
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    jb   Wait0                          ;Offset 0x4f00
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x4f13
    dec  cl
    je   TimeOut                        ;Offset 0x4f27
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x4f13
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x4e38
TimeOut:                                ;Offset 0x4f27
    mov  al, cl
    pop  ecx
    ret
WaitAck ENDP

;
;Pin0: 0000001
;Pin1: x1r...0
;
SendAck PROC NEAR                       ;Offset 0x4f2c
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
    mov  cl, 0ah
Wait1:                                  ;Offset 0x4f3b
    dec  cl
    je   TimeOut                        ;Offset 0x4f52
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x4f3b
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x4e38
TimeOut:                                ;Offset 0x4f52
    mov  al, cl
    pop  ecx
    ret
SendAck ENDP

;
;Pin0: 1111111
;Pin1: x1r...0
;
Func0x4f57 PROC NEAR                    ;Offset 0x4f57
    push ecx
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x4e38
    mov  cl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x4f65
    dec  cl
    je   TimeOut                        ;Offset 0x4f7c
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x4f65
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
    call SleepIfNeeded                  ;Offset 0x4e38
TimeOut:                                ;Offset 0x4f7c
    mov  al, cl
    pop  ecx
    ret
Func0x4f57 ENDP

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
I2CSendByte PROC NEAR                   ;Offset 0x4f81
    push ecx
    push edx
    mov  cl, 08h
    mov  ch, al
NextBit:                                ;Offset 0x4f89
    shl  ch, 01h
    jb   Send1                          ;Offset 0x4f93
    call word ptr cs:[si + MGA_DDCPin0Send0];0x04
    jmp  Sent0                          ;Offset 0x4f96
Send1:                                  ;Offset 0x4f93
    call word ptr cs:[si + MGA_DDCPin0Send1];0x00
Sent0:                                  ;Offset 0x4f96
    mov  dl, 0ah
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x4f9f
    dec  dl
    je   TimeOut                        ;Offset 0x4fb4
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x4f9f
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    dec  cl
    jne  NextBit                        ;Offset 0x4f89
TimeOut:                                ;Offset 0x4fb4
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
I2CReadByte PROC NEAR                   ;Offset 0x4fbb
    push ecx
    push edx
    xor  ch, ch
    mov  cl, 08h
NextBit:                                ;Offset 0x4fc3
    mov  dl, 0ah
    call word ptr cs:[si + MGA_DDCPin1Send1];0x02
Wait1:                                  ;Offset 0x4fc9
    dec  dl
    je   TimeOut                        ;Offset 0x4fe9
    call SleepIfNeeded                  ;Offset 0x4e38
    call word ptr cs:[si + MGA_DDCPin1Read];0x0a
    jae  Wait1                          ;Offset 0x4fc9
    call word ptr cs:[si + MGA_DDCPin0Read];0x08
    rcl  ch, 01h                        ;Carry bit is read data
    call word ptr cs:[si + MGA_DDCPin1Send0];0x06
    call SleepIfNeeded                  ;Offset 0x4e38
    dec  cl
    jne  NextBit                        ;Offset 0x4fc3
    mov  al, ch
TimeOut:                                ;Offset 0x4fe9
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
Func0x4ff0 PROC NEAR                    ;Offset 0x4ff0
    push ecx
    and  ecx, 0000ffffh
    cmp  ch, 0a0h
    ;jne  Label0x5003                    ;Offset 0x5003
    DB 00Fh, 085h, 003h, 000h
    call Func0x4e9c                     ;Offset 0x4e9c
Label0x5003:                            ;Offset 0x5003
    mov  cl, 04h
Label0x5005:                            ;Offset 0x5005
    push edx
    push edi
    call SendStartBit                   ;Offset 0x4ea8
    or   al, al
    je   Label0x50b9                    ;Offset 0x50b9
    or   dx, dx
    ;je   Label0x5060                    ;Offset 0x5060
    DB 00Fh, 084h, 048h, 000h
    mov  al, ch
    and  al, 0feh
    call I2CSendByte                    ;Offset 0x4f81
    or   al, al
    ;je   TimeOut                        ;Offset 0x50aa
    DB 00Fh, 084h, 085h, 000h
    call WaitAck                        ;Offset 0x4ef9
    or   al, al
    ;je   TimeOut                        ;Offset 0x50aa
    DB 00Fh, 084h, 07Ch, 000h
Label0x502e:                            ;Offset 0x502e
    mov  al, byte ptr [di]
    call I2CSendByte                    ;Offset 0x4f81
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
    call WaitAck                        ;Offset 0x4ef9
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
    inc  di
    dec  dx
    jne  Label0x502e                    ;Offset 0x502e
    or   edx, edx
    ;je   Label0x50a1                    ;Offset 0x50a1
    DB 00Fh, 084h, 058h, 000h
    and  ecx, 0000ffffh
    mov  al, 05h
Label0x5052:                            ;Offset 0x5052
    call SleepIfNeeded                  ;Offset 0x4e38
    dec  al
    jne  Label0x5052                    ;Offset 0x5052
    call SendStartBit                   ;Offset 0x4ea8
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
Label0x5060:                            ;Offset 0x5060
    mov  al, ch
    or   al, 01h
    call I2CSendByte                    ;Offset 0x4f81
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
    call WaitAck                        ;Offset 0x4ef9
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
    shr  edi, 10h
    shr  edx, 10h
    dec  dx
    ;je   Label0x5094                    ;Offset 0x5094
    DB 00Fh, 084h, 015h, 000h
Label0x507f:                            ;Offset 0x507f
    call I2CReadByte                    ;Offset 0x4fbb
    or   ah, ah
    je   TimeOut                        ;Offset 0x50aa
    mov  byte ptr es:[di], al
    call SendAck                        ;Offset 0x4f2c
    or   al, al
    je   TimeOut                        ;Offset 0x50aa
    inc  di
    dec  dx
    jne  Label0x507f                    ;Offset 0x507f
Label0x5094:                            ;Offset 0x5094
    call I2CReadByte                    ;Offset 0x4fbb
    or   ah, ah
    je   TimeOut                        ;Offset 0x50aa
    mov  byte ptr es:[di], al
    call Func0x4f57                     ;Offset 0x4f57
Label0x50a1:                            ;Offset 0x50a1
    mov  cl, 01h
    or   ecx, 00010000h
TimeOut:                                ;Offset 0x50aa
    call SleepIfNeeded                  ;Offset 0x4e38
    call SendStopBit                    ;Offset 0x4ed9
    mov  al, 05h
Label0x50b2:                            ;Offset 0x50b2
    call SleepIfNeeded                  ;Offset 0x4e38
    dec  al
    jne  Label0x50b2                    ;Offset 0x50b2
Label0x50b9:                            ;Offset 0x50b9
    pop  edi
    pop  edx
    dec  cl
    jne  Label0x5005                    ;Offset 0x5005
    shr  ecx, 10h
    mov  ax, cx
    pop  ecx
    ret
Func0x4ff0 ENDP

Func0x50cc PROC NEAR                    ;Offset 0x50cc
    cli
    push ds
    push bx
    push cx
    push edx
    push edi
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
    mov  word ptr [di], dx
    mov  dx, bx
    shl  edx, 10h
    mov  dx, 01h
    call FindMGAG100                    ;Offset 0x53d6
    mov  cl, 04h
Label0x50fc:                            ;Offset 0x50fc
    call Func0x4ff0                     ;Offset 0x4ff0
    or   ax, ax
    ;je   Label0x5114                    ;Offset 0x5114
    DB 00Fh, 084h, 00Fh, 000h
    call Func0x5129                     ;Offset 0x5129
    or   ah, ah
    ;jne  Label0x5114                    ;Offset 0x5114
    DB 00Fh, 085h, 006h, 000h
    mov  ax, 004fh
    jmp  Label0x511b                    ;Offset 0x511b
    nop
Label0x5114:                            ;Offset 0x5114
    dec  cl
    jne  Label0x50fc                    ;Offset 0x50fc
    mov  ax, 014fh
Label0x511b:                            ;Offset 0x511b
    pop  ds
    pop  word ptr ds:[BDA_SoftResetFlag];Offset 0x472
    pop  edi
    pop  edx
    pop  cx
    pop  bx
    pop  ds
    sti
    ret
Func0x50cc ENDP

Func0x5129 PROC NEAR                    ;Offset 0x5129
    push     ds
    push     edx
    push     esi
    push     es
    pop      ds
    xor      ax, ax
    mov      esi, edi
    shr      esi, 10h
    shr      edx, 10h
Label0x513d:                            ;Offset 0x513d
    lodsb
    add      ah, al
    dec      dx
    jne      Label0x513d                   ;Offset 0x513d
    pop      esi
    pop      edx
    pop      ds
    ret
Func0x5129 ENDP

ConfigureAGPBus PROC NEAR               ;Offset 0x5149
    push ax
    push si
    push di
    xor  si, si
Label0x514e:                            ;Offset 0x514e
    mov  ax, 0b102h                     ;Find PCI Device
    mov  cx, PCI_DEVICE_G100AGP         ;0x1001
    mov  dx, PCI_VENDOR_Matrox          ;0x102b
    int  1ah
    or   ah, ah
    jne  Label0x5188                    ;Offset 0x5188
    mov  ax, 0b10ch                     ;Write Configuration Word
    mov  di, PCI_MGA_IndirectAccessIndex;0x44
    mov  cx, MGA_MemAddr_PaletteRamWriteAddress;0x3c00
    int  1ah
    mov  ax, 0b10dh                     ;Write Configuration DWord
    mov  di, PCI_MGA_AGPCommand         ;0xf8
    mov  ecx, PCI_MGA_AGPCMD_Rate1x OR PCI_MGA_AGPCMD_AGPEnable OR PCI_MGA_AGPCMD_SBAEnable OR (01h SHL 24);0x1000301
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_Header_Byte_SubClassCode;0xa
    int  1ah
    and  cl, cl
    jne  Label0x5182                    ;Offset 0x5182 ???
Label0x5182:                            ;Offset 0x5182
    inc  si
    cmp  si, 0020h
    jl   Label0x514e                    ;Offset 0x514e
Label0x5188:                            ;Offset 0x5188
    call ConfigureAladdinChipset        ;Offset 0x518f
    pop  di
    pop  si
    pop  ax
    ret
ConfigureAGPBus ENDP

ConfigureAladdinChipset PROC NEAR       ;Offset 0x518f
    push bx
    mov  ax, 0b102h                     ;Find PCI Device
    mov  cx, PCI_DEVICE_M1541_Aladdin   ;0x1541
    mov  dx, PCI_VENDOR_ALI             ;0x10b9
    mov  si, 0000h
    int  1ah
    cmp  ah, 00h
    ;jne  NotFound                       ;Offset 0x51e3
    DB 00Fh, 085h, 03Eh, 000h
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, ALI_M1541_CPUtoPCIWriteBufferOption;0x86
    int  1ah
    and  cl, NOT ALI_M1541_CPUPCIWBO_PrgFrameBuffer;0xfe
    mov  ax, 0b10bh                     ;Write Configuration Byte
    int  1ah
    mov  ax, 0b102h                     ;Find PCI Device
    mov  cx, PCI_DEVICE_M5243_PCItoAGP_Controller;0x5243
    mov  dx, PCI_VENDOR_ALI             ;0x10b9
    mov  si, 00000h
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, ALI_M5243_CPUtoPCI66WriteBufferOption;0x86
    int  1ah
    and  cl, NOT ALI_M5243_CPUPCIWBO_VGAFixedFB;0xfe
    mov  ax, 0b10bh                     ;Write Configuration Byte
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, ALI_M5243_PCI66ToMainMemPCI66ArbiterOpt;0x88
    int  1ah
    or   cl, ALI_M5243_ForcePCI66GATMode;0x8
    mov  ax, 0b10bh                     ;Write Configuration Byte
    int  1ah
NotFound:                               ;Offset 0x51e3
    pop  bx
    ret
ConfigureAladdinChipset ENDP

ReadSubClassCode PROC NEAR              ;Offset 0x51e5
    push di
    mov  ax, 0b109h                     ;Read Configuration Word
    mov  di, PCI_Header_Word_VendorID   ;0x0
    int  1ah
    cmp  cx, PCI_VENDOR_Matrox          ;0x102b
    jne  NotFound                       ;Offset 0x5209
    mov  ax, 0b10ch                     ;Write Configuration Word
    mov  di, PCI_MGA_IndirectAccessIndex;0x44
    mov  cx, MGA_MemAddr_PaletteRamWriteAddress;0x3c00
    int  1ah
    mov  ax, 0b108h                     ;Read Configuration Byte
    mov  di, PCI_Header_Byte_SubClassCode;0xa
    int  1ah
    and  cl, cl
NotFound:                               ;Offset 0x5209
    pop  di
    ret
ReadSubClassCode ENDP

Func0x520b PROC NEAR                    ;Offset 0x520b
    push cx
    mov  cl, MGA_INDD_Reserved3f        ;0x3f ???
    call ReadIndexedRegister            ;Offset 0x578f
    cmp  cl, 30h
    pop  cx
    ret
Func0x520b ENDP

IsExtensionReg7Writeable PROC NEAR      ;Offset 0x5216
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
FindMGAG100Mechanism2 PROC NEAR         ;Offset 0x522b
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
Label0x523f:                            ;Offset 0x523f
    xor    dl, dl
    in     ax, dx
    cmp    ax, PCI_VENDOR_Matrox        ;0x102b
    jne    Label0x5261                  ;Offset 0x5261
    mov    dl, PCI_Header_Word_DeviceID ;0x2
    in     ax, dx
    cmp    ax, PCI_DEVICE_G100AGP       ;0x1001
    jne    Label0x5261                  ;Offset 0x5261
    mov    dl, PCI_Header_Word_Command  ;0x4
    in     al, dx
    and    al, PCI_Cmd_IOSpace          ;0x1
    je     Label0x5261                  ;Offset 0x5261
    mov    dl, PCI_Header_Byte_SubClassCode;0xa
    in     al, dx
    or     al, al
    jne    Label0x5261                  ;Offset 0x5261
    mov    bh, dh
    jmp    Label0x527e                  ;Offset 0x527e
Label0x5261:                            ;Offset 0x5261
    xor    dl, dl
    inc    dh
    cmp    dh, 0d0h
    jne    Label0x523f                  ;Offset 0x523f
    mov    dh, 0c0h
    inc    bx
    mov    al, bl
    push   dx
    mov    dx, PCI_M2_ForwardRegister   ;Port 0xcfa
    out    dx, al
    pop    dx
    cmp    bx, 0100h
    jne    Label0x523f                  ;Offset 0x523f
    mov    bx, 0ffffh
Label0x527e:                            ;Offset 0x527e
    xor    al, al
    mov    dx, PCI_M2_ConfigSpaceEnable ;Port 0xcf8
    out    dx, al
    pop    dx
    pop    cx
    pop    ax
    ret
FindMGAG100Mechanism2 ENDP

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
FindMGAG100Mechanism1 PROC NEAR         ;Offset 0x5288
    push  eax
    push  ebx
    push  ecx
    push  edx
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch                       ;PCI M1 port high byte
LoopBus:                                ;Offset 0x529a
    mov   cl, 20h                       ;Check 32 devices on this bus
LoopDevices:                            ;Offset 0x529c
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax                       ;Address for Header field 0
    mov   dl, PCI_M1_ConfigData_lowbyte ;Port 0xcfc
    in    eax, dx                       ;Read Device + Vendor ID's
    cmp   eax, (PCI_DEVICE_G100AGP SHL 16) OR PCI_VENDOR_Matrox;0x1001102b
    jne   NextDevice                    ;Offset 0x52d5
    mov   eax, ebx
    mov   al, PCI_Header_Word_Command   ;0x4
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B0_lowbyte;Port 0xcfc
    in    al, dx
    and   al, PCI_Cmd_IOSpace           ;0x1 Check if not IO space
    je    NextDevice                    ;Offset 0x52d5
    mov   eax, ebx
    mov   al, PCI_Header_Byte_RevisionID;0x8
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_B2_lowbyte;Port 0xcfe
    in    al, dx                        ;PCI_Header_Byte_SubClassCode
    or    al, al                        ;Check if not 0
    jne   NextDevice                    ;Offset 0x52d5
    shr   ebx, 08h                      ;bx = Bus, Device ID and function ID
    jmp   DeviceFound                   ;Offset 0x52f4
NextDevice:                             ;Offset 0x52d5
    dec   cl
    je    NextBus                       ;Offset 0x52e2
    add   ebx, 00000800h                ;Next device
    jmp   LoopDevices                   ;Offset 0x529c
NextBus:                                ;Offset 0x52e2
    and   bx, 07ffh                     ;Drop device index
    add   ebx, 00010000h                ;Next Bus
    dec   ch
    jne   LoopBus                       ;Offset 0x529a
    mov   bx, 0ffffh                    ;Device not found, set Bus, device ID and function ID to invalid
DeviceFound:                            ;Offset 0x52f4
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
FindMGAG100Mechanism1 ENDP

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
FindIntelTriton430FXChipset PROC NEAR   ;Offset 0x5308
    push  ebx
    xor   ch, ch
    mov   ebx, PCI_M1_ConfigAddres_Enable;0x80000000
    mov   dh, 0ch
    mov   cl, 10h
CheckNextDevice:                        ;Offset 0x5316
    mov   dl, PCI_M1_ConfigAddress_lowbyte;Port 0xcf8
    mov   eax, ebx
    out   dx, eax
    mov   dl, PCI_M1_ConfigData_lowbyte;Port 0xcfc
    in    eax, dx
    cmp   eax, (PCI_DEVICE_Triton_430FX SHL 16) OR PCI_VENDOR_Intel;Intel Triton 430FX chipset 0x122d8086
    je    FoundDevice                   ;Offset 0x5339
    dec   cl
    je    SearchDone                    ;Offset 0x5336
    add   ebx, 00000800h
    jmp   CheckNextDevice               ;Offset 0x5316
SearchDone:                             ;Offset 0x5336
    xor   eax, eax
FoundDevice:                            ;Offset 0x5339
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
AccessPCIRegister PROC NEAR             ;Offset 0x5344
    push  eax
    push  bx
    push  dx
    push  di
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    cmp   bh, 0c0h
    jb    Mechanism1                    ;Offset 0x5365
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
    jmp   PerformAccess                 ;Offset 0x5381
Mechanism1:                             ;Offset 0x5365
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
PerformAccess:                          ;Offset 0x5381
    cmp   al, PCI_ACCESS_ReadByte       ;0x8
    je    ReadByte                      ;Offset 0x53b8
    cmp   al, PCI_ACCESS_ReadWord       ;0x9
    je    ReadWord                      ;Offset 0x53b3
    cmp   al, PCI_ACCESS_ReadDWord      ;0xa
    je    ReadDWord                     ;Offset 0x53ac
    cmp   al, PCI_ACCESS_WriteByte      ;0xb
    je    WriteByte                     ;Offset 0x53a7
    cmp   al, PCI_ACCESS_WriteWord      ;0xc
    je    WriteWord                     ;Offset 0x53a2
    cmp   al, PCI_ACCESS_WriteDWord     ;0xd
    je    WriteDWord                    ;Offset 0x539b
    jmp   Done                          ;Offset 0x53bb
WriteDWord:                             ;Offset 0x539b
    mov   eax, ecx
    out   dx, eax
    jmp   Done                          ;Offset 0x53bb
WriteWord:                              ;Offset 0x53a2
    mov   ax, cx
    out   dx, ax
    jmp   Done                          ;Offset 0x53bb
WriteByte:                              ;Offset 0x53a7
    mov   al, cl
    out   dx, al
    jmp   Done                          ;Offset 0x53bb
ReadDWord:                              ;Offset 0x53ac
    in    eax, dx
    mov   ecx, eax
    jmp   Done                          ;Offset 0x53bb
ReadWord:                               ;Offset 0x53b3
    in    ax, dx
    mov   cx, ax
    jmp   Done                          ;Offset 0x53bb
ReadByte:                               ;Offset 0x53b8
    in    al, dx
    mov   cl, al
Done:                                   ;Offset 0x53bb
    cmp   dh, 0c0h
    jl    IsMechanism1                  ;Offset 0x53c8
    xor   al, al
    mov   dx, PCI_M2_ConfigSpaceEnable  ;Port 0xcf8
    out   dx, al
    jmp   Return                        ;Offset 0x53d0
IsMechanism1:                           ;Offset 0x53c8
    xor   eax, eax
    mov   dx, PCI_M1_ConfigAddress      ;Port 0xcf8
    out   dx, eax
Return:                                 ;Offset 0x53d0
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
FindMGAG100 PROC NEAR                   ;Offset 0x53d6
    call  FindMGAG100Mechanism1         ;Offset 0x5288
    inc   bx                            ;Check if found
    jne   Found                         ;Offset 0x53e0
    call  FindMGAG100Mechanism2         ;Offset 0x522b
    inc   bx
Found:                                  ;Offset 0x53e0
    dec   bx
    ret
FindMGAG100 ENDP

ReadPCIBase0AndBase1 PROC NEAR          ;Offset 0x53e2
    push  di
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 + 2;0x12
    call  AccessPCIRegister             ;Offset 0x5344
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress0 ;0x10
    call  AccessPCIRegister             ;Offset 0x5344
    and   ecx, 0fffffff0h
    mov   eax, ecx
    push  eax
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 + 2;0x16
    call  AccessPCIRegister             ;Offset 0x5344
    shl   ecx, 10h
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_H0_DWord_BaseAddress1 ;0x14
    call  AccessPCIRegister             ;Offset 0x5344
    and   ecx, 0fffffff0h
    push  ecx
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_Header_Byte_RevisionID;0x8
    call  AccessPCIRegister             ;Offset 0x5344
    pop   eax
    pop   ecx                           ;It discards the revision ID?
    jmp   Label0x542c                   ;Offset 0x542c
    pop   ecx
    pop   eax
Label0x542c:                            ;Offset 0x542c
    pop   di
    ret
ReadPCIBase0AndBase1 ENDP

ReadPCIDeviceID PROC NEAR               ;Offset 0x542e
    push  di
    push  cx
    mov   al, PCI_ACCESS_ReadWord       ;0x9
    mov   di, PCI_Header_Word_DeviceID  ;0x2
    call  AccessPCIRegister             ;Offset 0x5344
    mov   ax, cx
    pop   cx
    pop   di
    ret
ReadPCIDeviceID ENDP

CapAXTo0x40 PROC NEAR                   ;Offset 0x543d
    ;cmp   ax, 40h
    DB 03Dh, 040h, 000h
    ;jbe   IsLess                        ;Offset 0x5447
    DB 00Fh, 086h, 003h, 000h
    mov   ax, 40h
IsLess:                                 ;Offset 0x5447
    ret
CapAXTo0x40 ENDP

GetMemoryIn64KBlocks PROC NEAR          ;Offset 0x5448
    push  dx
    mov   ax, 0bd50h
    xor   dx, dx
    int   6dh
    or    ah, ah
    mov   ax, dx
    pop   dx
    ;je    Label0x545f                   ;Offset 0x545f
    DB 00Fh, 084h, 006h, 000h
    movzx ax, byte ptr cs:[MemorySizeInMB];Offset 0x7ff0
Label0x545f:                            ;Offset 0x545f
    shl   ax, 04h
    ret
GetMemoryIn64KBlocks ENDP

CacheMemorySize PROC NEAR               ;Offset 0x5463
    call  GetMemorySize                 ;Offset 0x546e
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
GetMemorySize PROC NEAR                 ;Offset 0x546e
    push  bx
    push  cx
    push  dx
    push  ds
    push  di
    push  si
    mov   di, 0080h                     ;Default to 8MB
    test  byte ptr cs:[Data0x7554], 10h ;Offset 0x7554
    jne   Label0x551b                   ;Offset 0x551b
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
Label0x5492:                            ;Offset 0x5492
    loop  Label0x5492                   ;Offset 0x5492 Speed sensitive!
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
Label0x54a8:                            ;Offset 0x54a8
    dec   cx
    je    Label0x54b8                   ;Offset 0x54b8
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x54a8                   ;Offset 0x54a8
Label0x54b0:                            ;Offset 0x54b0
    dec   cx
    je    Label0x54b8                   ;Offset 0x54b8
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x54b0                   ;Offset 0x54b0
Label0x54b8:                            ;Offset 0x54b8
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
    call  FindMGAG100                   ;Offset 0x53d6
    call  ReadPCIDeviceID               ;Offset 0x542e
    mov   di, 0020h
    mov   bx, 0080h                     ;8MB max
    cmp   ax, PCI_DEVICE_MillenniumIIPCI;0x051b
    ;jb    Label0x54e8                   ;Offset 0x54e8
    DB 00Fh, 082h, 003h, 000h
    mov   bx, 0100h                     ;16MB max
Label0x54e8:                            ;Offset 0x54e8
    mov   ax, (20h SHL 8)OR MGA_CRTCExt_MemoryPage;0x2004 Start at 2MB
Label0x54eb:                            ;Offset 0x54eb
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
    jne   Label0x550d                   ;Offset 0x550d
    add   di, 0010h                     ;Found one more MB
    add   ah, 10h                       ;Increment by 1MB
    cmp   bx, di
    jne   Label0x54eb                   ;Offset 0x54eb
Label0x550d:                            ;Offset 0x550d
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
Label0x551b:                            ;Offset 0x551b
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
IndirectRegisterWriteByte PROC NEAR     ;Offset 0x5524
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xch
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    pop    cx
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5344
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
IndirectRegisterWriteWord PROC NEAR     ;Offset 0x5542
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    pop    cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5344
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
IndirectRegisterWriteDWord PROC NEAR    ;Offset 0x5560
    push   si
    push   di
    push   cx
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    pop    cx
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x5344
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
IndirectRegisterReadByte PROC NEAR      ;Offset 0x5579
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5344
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
IndirectRegisterReadWord PROC NEAR      ;Offset 0x5595
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadWord      ;0x9
    mov    di, PCI_MGA_IndirectAccessData;0x48
    and    si, 0003h
    add    di, si
    call   AccessPCIRegister            ;Offset 0x5344
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
IndirectRegisterReadDWord PROC NEAR     ;Offset 0x55b1
    push   si
    push   di
    mov    al, PCI_ACCESS_WriteWord     ;0xc
    mov    di, PCI_MGA_IndirectAccessIndex;0x44
    mov    cx, si
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    mov    di, PCI_MGA_IndirectAccessData;0x48
    call   AccessPCIRegister            ;Offset 0x5344
    pop    di
    pop    si
    ret
IndirectRegisterReadDWord ENDP

Func0x55c8 PROC NEAR                    ;Offset 0x55c8
    push  ds
    xor   ax, ax
    mov   ds, ax
    mov   ax, 01h
    cmp   byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je    IsMode13                      ;Offset 0x55da
    mov   ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
IsMode13:                               ;Offset 0x55da
    and   al, 0fh
    pop   ds
    ret
Func0x55c8 ENDP

Func0x55de PROC NEAR                    ;Offset 0x55de
    push  ds
    push  0000h
    pop   ds
    or    al, al
    mov   ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    je    Label0x55f3                   ;Offset 0x55f3
    shr   al, 04h
    and   cl, 0f0h
    or    cl, al
    jmp   Label0x55ff                   ;Offset 0x55ff
Label0x55f3:                            ;Offset 0x55f3
    push  cx
    shl   cl, 04h
    and   al, 0fh
    or    al, cl
    mov   word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    pop   cx
Label0x55ff:                            ;Offset 0x55ff
    pop   ds
    ret
Func0x55de ENDP

GetStartAddress PROC NEAR               ;Offset 0x5601
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
    pop   ds
    pop   dx
    ret
GetStartAddress ENDP

SetStartAddress PROC NEAR               ;Offset 0x5628
    push  dx
    push  eax
    test  bl, 80h
    ;je    Label0x563a                   ;Offset 0x563a
    DB 00Fh, 084h, 008h, 000h
    mov   dx, VGA_InputStatus1D         ;Port 0x3da
Label0x5635:                            ;Offset 0x5635
    in    al, dx
    and   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je    Label0x5635                   ;Offset 0x5635
Label0x563a:                            ;Offset 0x563a
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

Func0x565c PROC NEAR                    ;Offset 0x565c
    push      ebx
    push      edx
    xor       ebx, ebx
    call      Func0x56d3                ;Offset 0x56d3
    shr       bx, 03h
    call      GetStartAddress           ;Offset 0x5601
    xor       edx, edx
    div       ebx
    mov       cx, dx
    push      ax
    call      Func0x55c8                ;Offset 0x55c8
    and       al, 03h
    je        Label0x5686               ;Offset 0x5686
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shl       ax, cl
    xchg      ax, cx
Label0x5686:                            ;Offset 0x5686
    pop       ax
    pop       edx
    pop       ebx
    mov       dx, ax
    mov       al, 01h
    call      Func0x55de                ;Offset 0x55de
    ret
Func0x565c ENDP

Func0x5693 PROC NEAR                    ;Offset 0x5693
    push      ebx
    push      ecx
    push      edx
    xor       al, al
    call      Func0x55de                ;Offset 0x55de
    push      bx
    push      cx
    push      dx
    call      Func0x56d3                ;Offset 0x56d3
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
    call      Func0x55c8                ;Offset 0x55c8
    and       al, 03h
    mov       ah, 04h
    sub       ah, al
    xchg      al, ah
    xchg      ax, cx
    shr       ax, cl
    xchg      ax, cx
    pop       ax
    add       eax, ecx
    call      SetStartAddress           ;Offset 0x5628
    pop       edx
    pop       ecx
    pop       ebx
    ret
Func0x5693 ENDP

Func0x56d3 PROC NEAR                    ;Offset 0x56d3
    push      ds
    xor       ax, ax
    mov       ds, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    out       dx, al                    ;MGA_CRTCExt_AddrGeneratorExt
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
    mov       bx, ax
    xor       ax, ax
    mov       dx, ax
    call      Func0x55c8                ;Offset 0x55c8
    mov       dl, ah
    mov       cl, al
    and       cl, 03h
    shl       bx, 04h
    dec       cl
    mov       ax, bx
    shr       ax, cl
    mov       cx, ax
    xor       ax, ax
    div       bx
    mov       dx, ax
    pop       ds
    ret
Func0x56d3 ENDP

Func0x5710 PROC NEAR                    ;Offset 0x5710
    push      ds
    push      cx
    xor       cx, cx
    mov       ds, cx
    mov       ch, byte ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    and       ch, 03h
    dec       ch
    mov       cl, 04h
    sub       cl, ch
    shr       ax, cl
    mov       bx, ax
    shl       bx, 03h
    push      ax
    shl       ah, 04h
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
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
    mov       dl, byte ptr ds:[BDA_VideoBufferSize + 01h];Offset 0x44d
    xor       dh, dh
    xor       ax, ax
    div       bx
    mov       dx, ax
    pop       cx
    pop       ds
    ret
Func0x5710 ENDP

Func0x5752 PROC NEAR                    ;Offset 0x5752
    mov       bx, ax
    call      Func0x55c8                ;Offset 0x55c8
    and       al, 03h
    je        Label0x5773               ;Offset 0x5773
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
    je        Label0x5773               ;Offset 0x5773
    sub       bx, dx
    add       ax, bx
Label0x5773:                            ;Offset 0x5773
    ret
Func0x5752 ENDP

;
;inputs:
;   cl = RAMDAC write index
;   ch = Value
;
;destroys:
;   ax
;
WriteIndexedRegister PROC NEAR          ;Offset 0x5774
    push   si
    push   dx
    mov    al, cl
    and    al, 80h
    jne    Label0x5782                  ;Offset 0x5782
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
Label0x5782:                            ;Offset 0x5782
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterWriteByte    ;Offset 0x5524
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
ReadIndexedRegister PROC NEAR           ;Offset 0x578f
    push   si
    push   dx
    mov    al, cl
    and    al, 80h
    jne    Label0x579d                  ;Offset 0x579d
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    mov    al, cl
    out    dx, al
Label0x579d:                            ;Offset 0x579d
    mov    si, MGA_MemAddr_IndexedData  ;0x3c0a
    xchg   cl, ch
    call   IndirectRegisterReadByte     ;Offset 0x5579
    pop    dx
    pop    si
    ret
ReadIndexedRegister ENDP

;Unused function
WriteIndexedRegisters PROC NEAR         ;Offset 0x57a8
    push cx
    push si
Label0x57aa:                            ;Offset 0x57aa
    mov  cx, word ptr [si]
    cmp  cl, 0ffh
    je   Label0x57b8                    ;Offset 0x57b8
    call WriteIndexedRegister           ;Offset 0x5774
    inc  si
    inc  si
    jmp  Label0x57aa                    ;Offset 0x57aa
Label0x57b8:                            ;Offset 0x57b8
    xor  ax, ax
    pop  si
    pop  cx
    ret
WriteIndexedRegisters ENDP

FetchDefaultSystemClockSpeed PROC NEAR  ;Offset 0x57bd
    push ax
    push bx
    xor  dx, dx
    mov  dl, byte ptr cs:[Data0x754b]   ;Offset 0x754b
    cmp  dl, 0ffh
    ;jne  Label0x5839                    ;Offset 0x5839
    DB 00Fh, 085h, 06Ch, 000h
    call GetMemoryIn64KBlocks           ;Offset 0x5448
    mov  bl, byte ptr cs:[Data0x7557]   ;Offset 0x7557
    and  bl, 0c0h
    rol  bl, 02h
    cmp  bl, 03h
    ;jne  Label0x57fd                    ;Offset 0x57fd
    DB 00Fh, 085h, 01Bh, 000h
    push di
    push cx
    push ax
    call FindMGAG100                    ;Offset 0x53d6
    mov  di, PCI_MGA_Option + 1         ;0x41
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    call AccessPCIRegister              ;Offset 0x5344
    test cl, PCI_MGA_Opt_MemCfg256Kx32 SHR 8;0x10
    pop  ax
    pop  cx
    pop  di
    ;je   Label0x5813                    ;Offset 0x5813
    DB 00Fh, 084h, 019h, 000h
    jmp  Label0x580d                    ;Offset 0x580d
    nop
Label0x57fd:                            ;Offset 0x57fd
    dec  bl
    ;js   Label0x5813                    ;Offset 0x5813
    DB 00Fh, 088h, 010h, 000h
    ;je   Label0x580d                    ;Offset 0x580d
    DB 00Fh, 084h, 006h, 000h
    sub  ax, 0080h
    jmp  Label0x5816                    ;Offset 0x5816
    nop
Label0x580d:                            ;Offset 0x580d
    ;sub  ax, 0040h
    DB 02Dh, 040h, 000h
    jmp  Label0x5816                    ;Offset 0x5816
    nop
Label0x5813:                            ;Offset 0x5813
    ;sub  ax, 0020h
    DB 02Dh, 020h, 000h
Label0x5816:                            ;Offset 0x5816
    ;je   Label0x582b                    ;Offset 0x582b
    DB 00Fh, 084h, 011h, 000h
    mov  bx, ax
    shr  bl, 06h
    mov  dl, byte ptr cs:[bx + Data0x754d];Offset 0x754d
    cmp  dl, 0ffh
    ;jne  Label0x5839                    ;Offset 0x5839
    DB 00Fh, 085h, 00Eh, 000h
Label0x582b:                            ;Offset 0x582b
    mov  dl, byte ptr cs:[Data0x754c]   ;Offset 0x754c
    cmp  dl, 0ffh
    ;jne  Label0x5839                    ;Offset 0x5839
    DB 00Fh, 085h, 002h, 000h
    mov  dl, 03fh
Label0x5839:                            ;Offset 0x5839
    mov  bl, byte ptr cs:[Data0x7554]   ;Offset 0x7554
    cmp  bl, 0ffh
    ;jne  Label0x5847                    ;Offset 0x5847
    DB 00Fh, 085h, 002h, 000h
    mov  bl, 00h
Label0x5847:                            ;Offset 0x5847
    test bl, 01h
    ;jne  Label0x5856                    ;Offset 0x5856
    DB 00Fh, 085h, 008h, 000h
    shl  dx, 01h
    mov  cx, 0001h
    jmp  Label0x5860                    ;Offset 0x5860
    nop
Label0x5856:                            ;Offset 0x5856
    mov  ax, 0003h
    mul  dx
    mov  dx, ax
    mov  cx, 0005h
Label0x5860:                            ;Offset 0x5860
    and  bl, 04h
    shl  bl, 02h
    or   cl, bl
    pop  bx
    pop  ax
    ret
FetchDefaultSystemClockSpeed ENDP

PowerUpPLLsLUTsAndConfigure PROC NEAR   ;Offset 0x586b
    push  ax
    push  bx
    push  cx
    push  dx
    push  di
    push  cx
    call  FindMGAG100                   ;Offset 0x53d6
    call  FindIntelTriton430FXChipset   ;Offset 0x5308
    or    ax, ax
    je    NoIntelTriton430FX            ;Offset 0x588b
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    mov   di, PCI_MGA_Option + 03h      ;0x43
    call  AccessPCIRegister             ;Offset 0x5344
    or    cl, (PCI_MGA_Opt_NoRetry SHR 24);0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5344
NoIntelTriton430FX:                     ;Offset 0x588b
    mov   cl, MGA_INDD_VoltageReferenceControl;0x18
    mov   ch, MGA_VREFCTRL_SysPLLVoltRefBlkPwrUp OR MGA_VREFCTRL_SysPLLVoltRefPLLRefBlk;0x03
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   al, 0fh
    call  Sleep2                        ;Offset 0x2c24
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5344
    or    cl, PCI_MGA_Opt_SysPLLPowerUp ;0x20
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5344
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2c24
    xor   dx, dx
PLLNotFrequencyLocked:                  ;Offset 0x58ae
    dec   dx
    je    PLLFreqLockTimeout            ;Offset 0x58bb
    mov   cl, MGA_INDD_SYSPLL_Status    ;0x2f
    call  ReadIndexedRegister           ;Offset 0x578f
    and   cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je    PLLNotFrequencyLocked         ;Offset 0x58ae
PLLFreqLockTimeout:                     ;Offset 0x58bb
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x578f
    or    cl, MGA_PIXCLKCTRL_PixelPLLPowerUp;0x8
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   al, 01h
    call  Sleep2                        ;Offset 0x2c24
    xor   dx, dx
PIXNotFrequencyLocked:                  ;Offset 0x58d1
    dec   dx
    je    PIXFreqLockTimeout            ;Offset 0x58de
    mov   cl, MGA_INDD_PIXPLL_Status    ;0x4f
    call  ReadIndexedRegister           ;Offset 0x578f
    and   cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je    PIXNotFrequencyLocked         ;Offset 0x58d1
PIXFreqLockTimeout:                     ;Offset 0x58de
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x578f
    or    cl, MGA_MISCCTRL_LUT_Enable   ;0x10
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   cl, MGA_INDD_MiscellaneousControl;0x1e
    call  ReadIndexedRegister           ;Offset 0x578f
    or    cl, MGA_MISCCTRL_DAC_Enable   ;0x1
    mov   ch, MGA_INDD_MiscellaneousControl;0x1e
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5344
    or    cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5344
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5344
    and   cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or    cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5344
    mov   di, PCI_MGA_Option            ;0x40
    mov   al, PCI_ACCESS_ReadByte       ;0x8
    call  AccessPCIRegister             ;Offset 0x5344
    and   cl, NOT PCI_MGA_Opt_SysClockDisable;0xfb
    mov   al, PCI_ACCESS_WriteByte      ;0xb
    call  AccessPCIRegister             ;Offset 0x5344
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x578f
    or    cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x578f
    and   cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or    cl, MGA_PIXCLKCTRL_SelPLL     ;0x1
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    mov   cl, MGA_INDD_PixelClockControl;0x1a
    call  ReadIndexedRegister           ;Offset 0x578f
    and   cl, NOT MGA_PIXCLKCTRL_ClockDisable;0xfb
    mov   ch, MGA_INDD_PixelClockControl;0x1a
    xchg  cl, ch
    call  WriteIndexedRegister          ;Offset 0x5774
    pop   dx
    or    dx, dx
    ;je   Label0x596c                    ;Offset 0x596c
    DB 00Fh, 084h, 006h, 000h
    mov   cx, PCI_MGA_Opt_SysClockSelectPLL OR 40h;0x41 - 0x40 - bit 6 - is unused in the function.
    jmp   Label0x596f                   ;Offset 0x596f
    nop
Label0x596c:                            ;Offset 0x596c
    call  FetchDefaultSystemClockSpeed  ;Offset 0x57bd
Label0x596f:                            ;Offset 0x596f
    mov   ax, dx                        ;ax = frequency in KHz
    xor   dx, dx
    mov   bx, 03e8h                     ;1,000
    mul   bx                            ;dx:ax is frequency in Hz
    shl   edx, 10h
    mov   dx, ax                        ;edx = frequency in Hz
    call  ConfigureSystemClock          ;Offset 0x5c4c
    pop   di
    pop   dx
    pop   cx
    pop   bx
    pop   ax
    ret
PowerUpPLLsLUTsAndConfigure ENDP

SetMemoryTimingsAndRefresh PROC NEAR    ;Offset 0x5987
    push   ax
    push   bx
    push   ecx
    push   dx
    push   di
    call   FindMGAG100                 ;Offset 0x53d6
    mov    dx, VGA_SequenceIndex       ;Port 0x3c4
    mov    ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out    dx, ax
    mov    si, MGA_MemAddr_MemoryControlWaitState;0x1c08
    ;mov    ecx, dword ptr cs:[MemoryControlWaitState];Offset 0x7550
    DB 02Eh, 066h, 08Bh, 00Eh, 050h, 075h
    cmp    ecx, 0ffffffffh
    ;jne    Label0x59ae                  ;Offset 0x59ae
    DB 00Fh, 085h, 006h, 000h
    mov    ecx, MGA_MA_MCWS_RowPrechrgDelay4Cycles OR MGA_MA_MCWS_Rd2PrechrgDelNCycles OR MGA_MA_MCWS_RASMinActive6Cycles OR MGA_MA_MCWS_BW2PrechrgDelay2Cycles OR MGA_MA_MCWS_BlockWriteDelay2Cycles OR MGA_MA_MCWS_RAS2CASDelay3Cycles OR MGA_MA_MCWS_RAS2RASDelay3Cycles OR MGA_MA_MCWS_CASLatency3Cycles;0x2032521
Label0x59ae:                            ;Offset 0x59ae
    call   IndirectRegisterWriteDWord   ;Offset 0x5560
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    mov    dl, byte ptr cs:[Data0x7556] ;Offset 0x7556
    mov    ch, dl
    push   cx
    test   ch, 08h
    ;je     Label0x59ca                  ;Offset 0x59ca
    DB 00Fh, 084h, 002h, 000h
    mov    ch, 04h
Label0x59ca:                            ;Offset 0x59ca
    and    ch, 04h
    shl    ch, 02h
    or     cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    di, PCI_MGA_Option2 + 01h    ;0x51
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    mov    ch, dl
    and    ch, 03h
    cmp    ch, 03h
    ;jne    Label0x59ed                  ;Offset 0x59ed
    DB 00Fh, 085h, 002h, 000h
    mov    ch, 00h
Label0x59ed:                            ;Offset 0x59ed
    shl    ch, 04h
    and    cl, 0cfh
    or     cl, ch
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2c24
    mov    si, MGA_MemAddr_MemoryAccess + 01h;0x1c05
    mov    cl, MGA_MA_MA_MemoryActive SHR 8;0x0
    call   IndirectRegisterWriteByte    ;Offset 0x5524
    mov    cl, MGA_MA_MA_MemoryReset SHR 8;0x80
    call   IndirectRegisterWriteByte    ;Offset 0x5524
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2c24
    mov    cl, (MGA_MA_MA_MemSeq8RefresRegRest OR MGA_MA_MA_MemoryActive) SHR 8;0x40
    call   IndirectRegisterWriteByte    ;Offset 0x5524
    mov    cl, (MGA_MA_MA_MemSeq8RefresRegRest OR MGA_MA_MA_MemoryReset) SHR 8;0xc0
    call   IndirectRegisterWriteByte    ;Offset 0x5524
    mov    di, PCI_MGA_Option2          ;0x50
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    mov    cl, dl
    and    cl, NOT PCI_MGA_Opt2_MemReadClkDelay_MASK;0xf0
    shr    cl, 04h
    call   AccessPCIRegister            ;Offset 0x5344
    pop    cx
    test   ch, 08h
    ;je     Label0x5a79                  ;Offset 0x5a79
    DB 00Fh, 084h, 046h, 000h
    push   cx
    mov    dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov    al, VGA_GCTLIdx_Miscellaneous;0x6
    out    dx, al
    inc    dx
    in     al, dx
    and    al, NOT VGA_GCTL6_Mem_MASK   ;0x3
    or     al, VGA_GCTL6_Mem_A0000_AFFFF;0x4
    out    dx, al
    mov    dx, MGA_CRTCExtensionIndex   ;0x3de
    mov    ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv2) SHL 8) OR MGA_CRTCExt_Misc;0x8103
    out    dx, ax
    mov    dx, MGA_CRTCExtensionIndex   ;0x3de
    mov    ax, MGA_CRTCExt_MemoryPage   ;0x4
    out    dx, ax
    push   ds
    push   si
    mov    ax, 0a000h                   ;Segment 0xa000
    mov    ds, ax
    xor    si, si
    mov    cl, 0aah
    mov    byte ptr [si], cl
    mov    di, 0800h
    mov    al, 55h
    mov    byte ptr [di], al
    mov    cl, byte ptr [si]
    pop    si
    pop    ds
    ;cmp    cl, al
    DB 03Ah, 0C8h
    pop    cx
    ;jne    Label0x5a79                  ;Offset 0x5a79
    DB 00Fh, 085h, 00Bh, 000h
    and    cl, NOT (PCI_MGA_Opt_MemCfg_MASK SHR 8);0xef
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
Label0x5a79:                            ;Offset 0x5a79
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadDWord     ;0xa
    call   AccessPCIRegister            ;Offset 0x5344
    and    ecx, NOT PCI_MGA_Opt_RefreshCounter_MASK;0xffe07fff
    or     ecx, 00078000h
    mov    al, PCI_ACCESS_WriteDWord    ;0xd
    call   AccessPCIRegister            ;Offset 0x5344
    test   byte ptr cs:[Data0x7554], 10h;Offset 0x7554
    ;jne    Label0x5aae                  ;Offset 0x5aae
    DB 00Fh, 085h, 010h, 000h
    mov    di, PCI_MGA_Option + 01h     ;0x41
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    or     cl, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
Label0x5aae:                            ;Offset 0x5aae
    pop    di
    pop    dx
    pop    ecx
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
CalculatePLL_MNPS PROC NEAR;Offset 0x5ab5
    push   bp
    mov    bp, sp
    sub    sp, 001ch
    push   ecx
    push   edx
    mov    dword ptr [bp - 18h], 0ffffffffh
    mov    dword ptr [bp - 04h], eax
    xor    eax, eax
    mov    al, byte ptr cs:[Data0x7544] ;Offset 0x7544
    cmp    al, 0ffh
    ;je     Label0x5aeb                  ;Offset 0x5aeb
    DB 00Fh, 084h, 013h, 000h
    ;add    ax, 0064h
    DB 005h, 064h, 000h
    mov    ebx, 000003e8h               ;1,000
    mul    ebx
    mov    dword ptr [bp - 08h], eax
    jmp    Label0x5af3                  ;Offset 0x5af3
    nop
Label0x5aeb:                            ;Offset 0x5aeb
    mov    dword ptr [bp - 08h], 00035b60h;220,000
Label0x5af3:                            ;Offset 0x5af3
    mov    eax, dword ptr [bp - 04h]
    cmp    eax, 00001876h               ;6,262
    jae    Label0x5b05                  ;Offset 0x5b05
    mov    eax, 00001876h               ;6,262
Label0x5b05:                            ;Offset 0x5b05
    cmp    eax, dword ptr [bp - 08h]
    jbe    Label0x5b0f                  ;Offset 0x5b0f
    mov    eax, dword ptr [bp - 08h]
Label0x5b0f:                            ;Offset 0x5b0f
    mov    dword ptr [bp - 04h], eax
    xor    edx, edx
    mov    ebx, 00000005h
    mul    ebx
    mov    ebx, 000003e8h               ;1,000
    div    ebx
    mov    dword ptr [bp - 1ch], eax
    mov    ecx, 00000003h
Label0x5b32:                            ;Offset 0x5b32
    mov    eax, dword ptr [bp - 04h]
    shl    eax, cl
    shl    ecx, 10h
    cmp    eax, 0000c350h               ;50,000
    jb     Label0x5bec                  ;Offset 0x5bec
    cmp    eax, dword ptr [bp - 08h]
    ja     Label0x5be2                  ;Offset 0x5be2
    mov    ch, 01h
    mov    cl, 07h
Label0x5b53:                            ;Offset 0x5b53
    xor    edx, edx
    xor    ebx, ebx
    mov    eax, 00006978h               ;27,000
    test   byte ptr cs:[Data0x7554], 20h;Offset 0x7554
    ;je     Label0x5b6f                  ;Offset 0x5b6f
    DB 00Fh, 084h, 006h, 000h
    mov    eax, 000037eeh               ;14,318
Label0x5b6f:                            ;Offset 0x5b6f
    mov    bl, cl
    inc    bl
    mul    ebx
    mov    bl, ch
    inc    bl
    div    ebx
    cmp    eax, dword ptr [bp - 08h]
    ;ja     Label0x5bd7                  ;Offset 0x5bd7
    DB 00Fh, 087h, 052h, 000h
    cmp    eax, 0000c350h                ;50,000
    ;jb     Label0x5bce                  ;Offset 0x5bce
    DB 00Fh, 082h, 03Fh, 000h
    mov    dword ptr [bp - 0ch], eax
    ror    ecx, 10h
    shr    eax, cl
    rol    ecx, 10h
    mov    ebx, dword ptr [bp - 04h]
    cmp    ebx, eax
    ;jb     Label0x5bab                  ;Offset 0x5bab
    DB 00Fh, 082h, 002h, 000h
    xchg   eax, ebx
Label0x5bab:                            ;Offset 0x5bab
    sub    eax, ebx
    mov    edx, dword ptr [bp - 0ch]
    cmp    eax, dword ptr [bp - 1ch]
    ;jbe    Label0x5bf4                  ;Offset 0x5bf4
    DB 00Fh, 086h, 03Ah, 000h
    cmp    eax, dword ptr [bp - 18h]
    ;jae    Label0x5bce                  ;Offset 0x5bce
    DB 00Fh, 083h, 00Ch, 000h
    mov    dword ptr [bp - 18h], eax
    mov    dword ptr [bp - 14h], ecx
    mov    dword ptr [bp - 10h], edx
Label0x5bce:                            ;Offset 0x5bce
    inc    cl
    cmp    cl, 7fh
    jbe    Label0x5b53                  ;Offset 0x5b53
Label0x5bd7:                            ;Offset 0x5bd7
    mov    cl, 07h
    inc    ch
    cmp    ch, 1fh
    jbe    Label0x5b53                  ;Offset 0x5b53
Label0x5be2:                            ;Offset 0x5be2
    shr    ecx, 10h
    dec    cl
    jns    Label0x5b32                  ;Offset 0x5b32
Label0x5bec:                            ;Offset 0x5bec
    mov    ecx, dword ptr [bp - 14h]
    mov    edx, dword ptr [bp - 10h]
Label0x5bf4:                            ;Offset 0x5bf4
    mov    ebx, ecx
    and    ebx, 00001fffh
    shr    ecx, 10h
    mov    ax, 0001h
    shl    ax, cl
    dec    ax
    shl    ax, 0dh
    or     bx, ax
    xor    eax, eax
    cmp    edx, 000186a0h               ;100,000
    ja     Label0x5c1e                  ;Offset 0x5c1e
    mov    ax, 0000h                    ;PixPLL S
    jmp    Label0x5c3d                  ;Offset 0x5c3d
Label0x5c1e:                            ;Offset 0x5c1e
    cmp    edx, 000222e0h               ;140,000
    ja     Label0x5c2c                  ;Offset 0x5c2c
    mov    ax, 0001h                    ;PixPLL S
    jmp    Label0x5c3d                  ;Offset 0x5c3d
Label0x5c2c:                            ;Offset 0x5c2c
    cmp    edx, 0002bf20h               ;180,000
    ja     Label0x5c3a                  ;Offset 0x5c3a
    mov    ax, 0002h                    ;PixPLL S
    jmp    Label0x5c3d                  ;Offset 0x5c3d
Label0x5c3a:                            ;Offset 0x5c3a
    mov    ax, 0003h                    ;PixPLL S
Label0x5c3d:                            ;Offset 0x5c3d
    shl    eax, 10h
    or     ebx, eax                     ;Merge S value
    pop    edx
    pop    ecx
    mov    sp, bp
    pop    bp
    ret
CalculatePLL_MNPS ENDP

;
;inputs:
;  edx = Requested frequency in Hz
;   cx = bits 1-0 = sysclksl
;        bit    2 = gclkdiv
;        bit    3 = mclkdiv
;        bit    4 = fmclkdiv
;        bit  7-5 = unused
;        
ConfigureSystemClock PROC NEAR          ;Offset 0x5c4c
    push   eax
    push   ebx
    push   ecx
    push   edx
    push   esi
    push   cx
    and    cl, 01h                      ;bit 0
    jne    NotPLL                       ;Offset 0x5c99
    call   FindMGAG100                  ;Offset 0x53d6
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, dl
    and    al, PCI_MGA_Opt_SysClockSelect_MASK;bits 1,0
    jne    Label0x5c86                  ;Offset 0x5c86
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    jmp    Label0x5d1e                  ;Offset 0x5d1e
Label0x5c86:                            ;Offset 0x5c86
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectMCLK;0x2
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    jmp    Label0x5d1e                  ;Offset 0x5d1e
NotPLL:                                 ;Offset 0x5c99
    mov    eax, edx
    call   CalculatePLL_MNPS            ;Offset 0x5ab5
    push   ebx
    call   FindMGAG100                  ;Offset 0x53d6
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    and    cl, NOT PCI_MGA_Opt_SysClock_MASK;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    pop    edx
    mov    cl, MGA_INDD_SYSPLL_M_Value  ;0x2c
    mov    ch, dh
    and    ch, MGA_SYSPLLM_Mask         ;0x1f
    call   WriteIndexedRegister         ;Offset 0x5774
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x5774
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_SYSPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x5774
    mov    al, 01h
    call   Sleep2                       ;Offset 0x2c24
    xor    dx, dx
WaitForFrequencyLock:                   ;Offset 0x5cf3
    dec    dx
    je     TimeOut                      ;Offset 0x5d00
    mov    cl, MGA_INDD_SYSPLL_Status   ;0x2f
    call   ReadIndexedRegister          ;Offset 0x578f
    and    cl, MGA_SYSPLLSTAT_FrequencyLock;0x40
    je     WaitForFrequencyLock         ;Offset 0x5cf3
TimeOut:                                ;Offset 0x5d00
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    or     cl, PCI_MGA_Opt_SysClockDisable;0x4
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    pop    dx
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    and    cl, NOT PCI_MGA_Opt_SysClockSelect_MASK;0xfc
    or     cl, PCI_MGA_Opt_SysClockSelectPLL;0x1
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
Label0x5d1e:                            ;Offset 0x5d1e
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, dl
    and    al, 0ch                      ;gclkdiv & mclkdiv
    shl    al, 01h                      ;...
    or     cl, al
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    mov    al, dl
    and    al, 10h                      ;fmclkdiv
    shl    al, 3                        ;...
    or     cl, al
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
    mov    di, PCI_MGA_Option           ;0x40
    mov    al, PCI_ACCESS_ReadByte      ;0x8
    call   AccessPCIRegister            ;Offset 0x5344
    and    cl, NOT PCI_MGA_Opt_SysClock_MASK;0xfb
    mov    al, PCI_ACCESS_WriteByte     ;0xb
    call   AccessPCIRegister            ;Offset 0x5344
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
ConfigurePixelClocks PROC NEAR          ;Offset 0x5d5e
    push   eax
    push   ebx
    push   ecx
    push   edx
    mov    edx, ebx
    push   edx
    push   cx
    call   FindMGAG100                  ;Offset 0x53d6
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    out    dx, al
    inc    dx
    in     al, dx
    or     al, VGA_SEQ1_ScreenOff       ;0x20
    out    dx, al
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x578f
    or     cl, MGA_PIXCLKCTRL_ClockDisable;0x4
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5774
    pop    cx
    mov    al, cl
    and    al, 03h
    cmp    al, 00h
    jne    Label0x5d96                  ;Offset 0x5d96
    mov    al, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jmp    Label0x5da0                  ;Offset 0x5da0
Label0x5d96:                            ;Offset 0x5d96
    cmp    al, 01h
    jne    Label0x5d9e                  ;Offset 0x5d9e
    mov    al, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jmp    Label0x5da0                  ;Offset 0x5da0
Label0x5d9e:                            ;Offset 0x5d9e
    mov    al, MGA_INDD_PIXPLL_M_ValueSetC;0x4c
Label0x5da0:                            ;Offset 0x5da0
    mov    ah, cl
    shr    ah, 02h
    cmp    ah, 01h
    jne    Label0x5dae                  ;Offset 0x5dae
    mov    ah, MGA_PIXCLKCTRL_SelPLL    ;0x01
    jmp    Label0x5db9                  ;Offset 0x5db9
Label0x5dae:                            ;Offset 0x5dae
    cmp    ah, 00h
    jne    Label0x5db7                  ;Offset 0x5db7
    mov    ah, MGA_PIXCLKCTRL_SelPCI    ;0x0
    jmp    Label0x5db9                  ;Offset 0x5db9
Label0x5db7:                            ;Offset 0x5db7
    mov    ah, MGA_PIXCLKCTRL_SelVDOCLK ;0x2
Label0x5db9:                            ;Offset 0x5db9
    pop    edx
    push   ax
    mov    ch, dh
    and    ch, MGA_PIXPLL_M_MASK        ;0x1f
    mov    cl, al
    call   WriteIndexedRegister         ;Offset 0x5774
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_N_Value
    call   WriteIndexedRegister         ;Offset 0x5774
    shr    edx, 0dh
    mov    ch, dl
    inc    cl                           ;MGA_INDD_PIXPLL_P_Value
    call   WriteIndexedRegister         ;Offset 0x5774
    pop    dx
    push   dx
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetA;0x44
    jne    Label0x5de3                  ;Offset 0x5de3
    mov    cl, VGA_MISC_Clock25p175MHz SHR 2;00h
    jmp    Label0x5dee                  ;Offset 0x5dee
Label0x5de3:                            ;Offset 0x5de3
    cmp    dl, MGA_INDD_PIXPLL_M_ValueSetB;0x48
    jne    Label0x5dec                  ;Offset 0x5dec
    mov    cl, VGA_MISC_Clock28p322MHz SHR 2;01h
    jmp    Label0x5dee                  ;Offset 0x5dee
Label0x5dec:                            ;Offset 0x5dec
    mov    cl, MGA_MISC_MGAPixelClock SHR 2;03h
Label0x5dee:                            ;Offset 0x5dee
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    shl    cl, 02h
    and    al, NOT VGA_MISC_ClockSelectMask;0xf3
    or     al, cl
    mov    dx, VGA_MiscellaneousWrite   ;Port 0x3c2
    out    dx, al
    mov    al, 01h
    call   Sleep                        ;Offset 0x4e44
    xor    dx, dx
Label0x5e04:                            ;Offset 0x5e04
    dec    dx
    je     Label0x5e11                  ;Offset 0x5e11
    mov    cl, MGA_INDD_PIXPLL_Status   ;0x4f
    call   ReadIndexedRegister          ;Offset 0x578f
    and    cl, MGA_PIXPLLSTAT_FrequencyLock;0x40
    je     Label0x5e04                  ;Offset 0x5e04
Label0x5e11:                            ;Offset 0x5e11
    pop    dx
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x578f
    and    cl, NOT MGA_PIXCLKCTRL_SelMASK;0xfc
    or     cl, dh
    xchg   cl, ch
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5774
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   ReadIndexedRegister          ;Offset 0x578f
    and    cl, NOT MGA_PIXCLKCTRL_ClockMASK;0xfb
    xchg   ch, cl
    mov    cl, MGA_INDD_PixelClockControl;0x1a
    call   WriteIndexedRegister         ;Offset 0x5774
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
ConfigureAndSelectPLLSetC PROC NEAR     ;Offset 0x5e46
    push   eax
    push   ebx
    push   ecx
    mov    cl, MGA_CPC_PLLSetC OR MGA_CPC_ClockPLL;0x6
    mov    eax, edx
    call   CalculatePLL_MNPS            ;Offset 0x5ab5
    call   ConfigurePixelClocks         ;Offset 0x5d5e
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigureAndSelectPLLSetC ENDP

ConfigureAndSelectPLLSetsAB PROC NEAR   ;Offset 0x5e5e
    push   eax
    push   ebx
    push   ecx
    mov    cl, byte ptr cs:[PLLADefaultFreq];Offset 0x7549
    cmp    cl, 0ffh
    ;jne    Label0x5e72                  ;Offset 0x5e72
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x5e72:                            ;Offset 0x5e72
    xor    eax, eax
    mov    al, 0fah                     ;250
    mul    cl
    or     ax, ax
    ;jne    Label0x5e82                  ;Offset 0x5e82
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6257h                    ;25,175
Label0x5e82:                            ;Offset 0x5e82
    call   CalculatePLL_MNPS            ;Offset 0x5ab5
    mov    cl, MGA_CPC_PLLSetA OR MGA_CPC_ClockPLL;0x4
    call   ConfigurePixelClocks         ;Offset 0x5d5e
    mov    cl, byte ptr cs:[PLLBDefaultFreq];Offset 0x754a
    cmp    cl, 0ffh
    ;jne    Label0x5e98                  ;Offset 0x5e98
    DB 00Fh, 085h, 002h, 000h
    mov    cl, 00h
Label0x5e98:                            ;Offset 0x5e98
    xor    eax, eax
    mov    al, 0fah                     ;250
    mul    cl
    or     ax, ax
    ;jne    Label0x5ea8                  ;Offset 0x5ea8
    DB 00Fh, 085h, 003h, 000h
    mov    ax, 6ea2h                    ;28,322
Label0x5ea8:                            ;Offset 0x5ea8
    call   CalculatePLL_MNPS            ;Offset 0x5ab5
    mov    cl, MGA_CPC_PLLSetB OR MGA_CPC_ClockPLL;0x5
    call   ConfigurePixelClocks         ;Offset 0x5d5e
    pop    ecx
    pop    ebx
    pop    eax
    ret
ConfigureAndSelectPLLSetsAB ENDP

;Offset 0x5eb7
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

Func0x5ee6 PROC NEAR                    ;Offset 0x5ee6
    push dx
    push cx
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, MGA_MULCTRL_DepthMask      ;0x7
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    xor  al, al
    cmp  cl, MGA_MULCTRL_Depth32Pal     ;0x7
    je   Label0x5f01                    ;Offset 0x5f01
    cmp  cl, MGA_MULCTRL_Depth16        ;0x2
    je   Label0x5f0d                    ;Offset 0x5f0d
    jmp  Label0x5f2a                    ;Offset 0x5f2a
Label0x5f01:                            ;Offset 0x5f01
    out  dx, al
    inc  dx
    out  dx, al
    out  dx, al
    out  dx, al
    dec  dx
    inc  al
    jne  Label0x5f01                    ;Offset 0x5f01
    jmp  Label0x5f3b                    ;Offset 0x5f3b
Label0x5f0d:                            ;Offset 0x5f0d
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
    jne  Label0x5f0d                    ;Offset 0x5f0d
    jmp  Label0x5f3b                    ;Offset 0x5f3b
Label0x5f2a:                            ;Offset 0x5f2a
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
    jne  Label0x5f2a                    ;Offset 0x5f2a
Label0x5f3b:                            ;Offset 0x5f3b
    pop  cx
    pop  dx
    ret
Func0x5ee6 ENDP

Func0x5f3e PROC NEAR                    ;Offset 0x5f3e
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
    jne  Label0x5f59                    ;Offset 0x5f59
    shr  ax, 01h
    jmp  Label0x5f5b                    ;Offset 0x5f5b
Label0x5f59:                            ;Offset 0x5f59
    shl  ax, cl
Label0x5f5b:                            ;Offset 0x5f5b
    out  dx, al
    mov  dl, MGA_CRTCExtensionData_lowbyte;Port 0x3df
    mov  al, ah
    shl  al, 04h
    out  dx, al
    pop  dx
    ret
Func0x5f3e ENDP

Func0x5f66 PROC NEAR                    ;Offset 0x5f66
    push ds
    push es
    push si
    push cx
    push bx
    push dx
    xor  ax, ax
    mov  es, ax
    call GetMemoryIn64KBlocks           ;Offset 0x5448
    call CapAXTo0x40                    ;Offset 0x543d
    mov  byte ptr es:[BDA_VideoBufferSize + 01h], al;Offset 0x44d
    mov  byte ptr es:[BDA_VideoBufferSize], 00h;Offset 0x44c
    mov  al, 03h
    and  al, byte ptr [si + 0bh]
    jne  Label0x6068                    ;Offset 0x6068
    call FindMGAG100                    ;Offset 0x53d6
    mov  al, PCI_ACCESS_ReadByte        ;0x8
    mov  di, PCI_MGA_Option + 01h       ;0x41
    call AccessPCIRegister              ;Offset 0x5344
    and  cl, NOT (PCI_MGA_Opt_SplitModeEnable SHR 8);0xdf
    mov  al, PCI_ACCESS_WriteByte       ;0xb
    call AccessPCIRegister              ;Offset 0x5344
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, MGA_CRTCExt_MemoryPage     ;0x4
    out  dx, ax
    test byte ptr [si + 0bh], 10h
    je   Label0x600d                    ;Offset 0x600d
    test byte ptr [si + 0bh], 20h
    je   Label0x5fda                    ;Offset 0x5fda
    mov  byte ptr es:[BDA_VideoBufferSize], 03h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    mov  ch, MGA_MULCTRL_Depth32Pal     ;0x7
    call WriteIndexedRegister           ;Offset 0x5774
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv4) SHL 8) OR MGA_CRTCExt_Misc;0x8303
    out  dx, ax
    call Func0x5ee6                     ;Offset 0x5ee6
    mov  cx, 0002h
    jmp  Label0x6033                    ;Offset 0x6033
Label0x5fda:                            ;Offset 0x5fda
    mov  byte ptr es:[BDA_VideoBufferSize], 02h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    mov  ch, 02h
    test byte ptr [si + 0bh], 40h
    je   Label0x5ffb                    ;Offset 0x5ffb
    mov  ch, 01h
Label0x5ffb:                            ;Offset 0x5ffb
    call WriteIndexedRegister           ;Offset 0x5774
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv2) SHL 8) OR MGA_CRTCExt_Misc;0x8103
    out  dx, ax
    call Func0x5ee6                     ;Offset 0x5ee6
    mov  cx, 0000h
    jmp  Label0x6033                    ;Offset 0x6033
Label0x600d:                            ;Offset 0x600d
    mov  byte ptr es:[BDA_VideoBufferSize], 01h;Offset 0x44c
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_MISCCTRL_DAC_8Bit  ;0xf7
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    xor  ch, ch
    call WriteIndexedRegister           ;Offset 0x5774
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ax, ((MGA_CRTCEXT3_MGAModeEnable OR MGA_CRTCEXT3_ScaleDiv1) SHL 8) OR MGA_CRTCExt_Misc;0x8003
    out  dx, ax
    mov  cx, 0001h
Label0x6033:                            ;Offset 0x6033
    push cx
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    or   cl, MGA_MISCCTRL_MAFC_Disable  ;0x6
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    pop  cx
    mov  al, MGA_CRTCExt_HorVidHalfCount;0x5
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, 01h
    out  dx, ax
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, byte ptr cs:[Data0x7557]   ;Offset 0x7557
    shr  ah, 03h
    and  ah, MGA_CRTCEXT6_HiPrioLevelMASK;0x7
    mov  al, MGA_CRTCExt_PrioRequestControl;0x6
    out  dx, ax
    or   cl, cl
    je   Label0x6068                    ;Offset 0x6068
    dec  cl
    call Func0x5f3e                     ;Offset 0x5f3e
    jmp  Label0x6068                    ;Offset 0x6068
Label0x6068:                            ;Offset 0x6068
    pop  dx
    pop  bx
    pop  cx
    pop  si
    pop  es
    pop  ds
    ret
Func0x5f66 ENDP

Func0x606f PROC NEAR                    ;Offset 0x606f
    push bx
    push cx
    push dx
    push si
    push di
    call IsExtensionReg7Writeable       ;Offset 0x5216
    ;je   Label0x60f3                    ;Offset 0x60f3
    DB 00Fh, 084h, 078h, 000h
    push es
    push ds
    push cs
    pop  ds
    call FindMGAG100                    ;Offset 0x53d6
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT (MGA_MISCCTRL_MAFC_MASK OR MGA_MISCCTRL_DAC_Bit_MASK);0xf1
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_MultiplexControl  ;0x19
    xor  ch, ch                         ;MGA_MULCTRL_Depth8
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENCTRL_PedestalControl_MASK;0xef
    mov  al, byte ptr cs:[Data0x7555]   ;Offset 0x7555
    and  al, 01h
    shl  al, 04h                        ;Pedestal Control
    or   cl, al
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_GENCTRL_GreenSyncOut_MASK;0xdf
    mov  al, byte ptr cs:[Data0x7555]   ;Offset 0x7555
    and  al, MGA_GENCTRL_GreenSyncOut_MASK;0x20
    or   cl, al
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_GeneralControl    ;0x1d
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, MGA_GENCTRL_PedestalControl_MASK OR MGA_GENCTRL_GreenSyncOut_MASK;0x30
    mov  ch, MGA_INDD_GeneralControl    ;0x1d
    xchg cl, ch
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_CursorControl     ;0x6
    mov  ch, MGA_CURCTRL_CursorDisabled ;0x0
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_PIXCLKCTRL_SelMASK ;0xfc
    or   cl, MGA_PIXCLKCTRL_SelPLL      ;0x1
    mov  ch, cl
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call WriteIndexedRegister           ;Offset 0x5774
    pop  ds
    pop  es
Label0x60f3:                            ;Offset 0x60f3
    mov  ax, MGA_CRTCExt_AddrGeneratorExt SHL 8;0x0
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3da
    mov  ax, MGA_CRTCExt_VertCounterExt SHL 8;0x200
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3da
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, byte ptr cs:[Data0x7555]   ;Offset 0x7555
    and  ah, MGA_CRTCEXT3_CompSyncEnable SHR 2;10h
    shl  ah, 02h                        ;MGA_CRTCEXT3_CompSyncEnable
    mov  al, MGA_CRTCExt_Misc           ;0x3
    out  dx, ax
    mov  ax, MGA_CRTCExt_MemoryPage SHL 8;0x400
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3da
    mov  ax, MGA_CRTCExt_HorVidHalfCount SHL 8;0x500
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3da
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    mov  ah, byte ptr cs:[Data0x7557]   ;Offset 0x7557
    and  ah, MGA_CRTCEXT6_HiPrioLevelMASK;0x7
    mov  al, MGA_CRTCExt_PrioRequestControl;0x6
    out  dx, ax
    pop  di
    pop  si
    pop  dx
    pop  cx
    pop  bx
    ret
Func0x606f ENDP

SaveMGAState PROC NEAR                  ;Offset 0x6130
    push ds
    push si
    push di
    push dx
    push cx
    push ax
    mov  ax, cs
    mov  ds, ax
    mov  di, bx
    call FindMGAG100                    ;Offset 0x53d6
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x6141
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5eb7
    cmp  cl, 80h
    je   EndOfIndexedRegisterData       ;Offset 0x6155
    call ReadIndexedRegister            ;Offset 0x578f
    mov  byte ptr es:[di], cl
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x6141
EndOfIndexedRegisterData:               ;Offset 0x6155
    mov  byte ptr es:[di], 80h
    inc  di
    push di
    mov  al, PCI_ACCESS_ReadDWord       ;0xa
    mov  di, PCI_MGA_Option             ;0x40
    call AccessPCIRegister              ;Offset 0x5344
    pop  di
    ;mov  dword ptr es:[di], ecx
    DB 026h, 066h, 089h, 00Dh
    add  di, 0004h
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x6171
    mov  al, cl
    out  dx, al
    inc  dx
    in   al, dx
    dec  dx
    mov  byte ptr es:[di], al
    inc  di
    inc  cl
    cmp  cl, 06h
    jl   LoopExtensionRegisters         ;Offset 0x6171
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  di
    pop  si
    pop  ds
    ret
SaveMGAState ENDP

RestoreMGAState PROC NEAR               ;Offset 0x618b
    push ds
    push di
    push si
    push dx
    push cx
    push ax
    mov  ax, cs
    mov  ds, ax
    mov  di, bx
    call FindMGAG100                    ;Offset 0x53d6
    xor  si, si
LoopIndexedRegisters:                   ;Offset 0x619c
    mov  cl, byte ptr cs:[si + IndexedRegisters];Offset 0x5eb7
    cmp  cl, 80h
    je   EndOfIndexedRegisterData       ;Offset 0x61b0
    mov  ch, byte ptr es:[di]
    call WriteIndexedRegister           ;Offset 0x5774
    inc  di
    inc  si
    jmp  LoopIndexedRegisters           ;Offset 0x619c
EndOfIndexedRegisterData:               ;Offset 0x61b0
    inc  di
    push di
    mov  al, PCI_ACCESS_WriteDWord      ;0xd
    ;mov  ecx, dword ptr es:[di]
    DB 026h, 066h, 08Bh, 00Dh
    mov  di, PCI_MGA_Option             ;0x40
    call AccessPCIRegister              ;Offset 0x5344
    pop  di
    add  di, 0004h
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    nop
    xor  cl, cl
LoopExtensionRegisters:                 ;Offset 0x61c8
    mov  al, cl
    out  dx, al
    inc  dx
    mov  al, byte ptr es:[di]
    out  dx, al
    dec  dx
    inc  di
    inc  cl
    cmp  cl, 06h
    jl   LoopExtensionRegisters         ;Offset 0x61c8
    mov  bx, di
    pop  ax
    pop  cx
    pop  dx
    pop  si
    pop  di
    pop  ds
    ret
RestoreMGAState ENDP

Func0x61e2 PROC NEAR                    ;Offset 0x61e2
    cmp  al, 20h
    jb   Label0x61fa                    ;Offset 0x61fa
    cmp  al, 22h
    je   Label0x61f4                    ;Offset 0x61f4
    cmp  al, 28h
    jb   Label0x61f8                    ;Offset 0x61f8
    cmp  al, 2dh
    mov  al, 03h
    jb   Label0x61fa                    ;Offset 0x61fa
Label0x61f4:                            ;Offset 0x61f4
    mov  al, 12h
    jmp  Label0x61fa                    ;Offset 0x61fa
Label0x61f8:                            ;Offset 0x61f8
    mov  al, 13h
Label0x61fa:                            ;Offset 0x61fa
    ret
Func0x61e2 ENDP

Func0x61fb PROC NEAR                    ;Offset 0x61fb
    push      es
    push      di
    push      dx
    push      ecx
    mov       ax, 0a000h                ;Segment 0xa000
    mov       es, ax
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    call      Func0x55c8                ;Offset 0x55c8
    mov       al, 04h
Label0x620d:                            ;Offset 0x620d
    dec       ah
    jb        Label0x6227               ;Offset 0x6227
    out       dx, ax
    push      ax
    xor       eax, eax
    mov       di, 0000h
    mov       ecx, 00004000h
    rep stosd
    pop       ax
    or        ah, ah
    jne       Label0x620d               ;Offset 0x620d
Label0x6227:                            ;Offset 0x6227
    pop       ecx
    pop       dx
    pop       di
    pop       es
    ret
Func0x61fb ENDP

Func0x622d PROC NEAR                    ;Offset 0x622d
    push bx
    push cx
    push dx
    push si
    push di
    call FindMGAG100                    ;Offset 0x53d6
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, MGA_MISCCTRL_LUT_MASK OR MGA_MISCCTRL_DAC_Bit_MASK OR MGA_MISCCTRL_DAC_MASK;0x19
    or   cl, MGA_MISCCTRL_MAFC_MAFC     ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call ReadIndexedRegister            ;Offset 0x578f
    and  cl, NOT MGA_PIXCLKCTRL_SelMASK ;0xfc
    or   cl, MGA_PIXCLKCTRL_SelVDOCLK   ;0x2
    mov  ch, cl
    mov  cl, MGA_INDD_PixelClockControl ;0x1a
    call WriteIndexedRegister           ;Offset 0x5774
    mov  al, MGA_CRTCExt_HorCounterExt  ;0x1
    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
    out  dx, al
    inc  dx
    in   al, dx
    or   al, MGA_CRTCEXT1_VerResetEnable OR MGA_CRTCEXT1_HorResetEnable;0x88
    out  dx, al
    pop  di
    pop  si
    pop  dx
    pop  cx
    pop  bx
    ret
Func0x622d ENDP

Func0x626a PROC NEAR                    ;Offset 0x626a
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
    mov       edx, dword ptr [si]
    mov       cl, 2dh
    call      ConfigureAndSelectPLLSetC ;Offset 0x5e46
    xor       dx, dx
    xor       eax, eax
    mov       ax, word ptr [si + 0ch]
    mov       bx, ds
    mov       cx, bx
    or        cx, ax
    je        Label0x62d5               ;Offset 0x62d5
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
Label0x62d5:                            ;Offset 0x62d5
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
    call      TurnScreenOff             ;Offset 0x2b3d
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrOfs];Offset 0x4a8
    pop       word ptr ds:[BDA_VideoParameterControlBlockPtrSeg];Offset 0x4aa
    lds       si, [bp + 04h]
    test      byte ptr [si + 0bh], 80h
    je        Label0x631b               ;Offset 0x631b
    push      si
    push      ds
    mov       al, 02h
    mov       bx, 0000h
    call      TextFunctions             ;Offset 0x200b
    pop       ds
    pop       si
Label0x631b:                            ;Offset 0x631b
    mov       dx, MGA_CRTCExtensionIndex;Port 0x3de
    mov       ah, byte ptr [si + 10h]
    mov       al, MGA_CRTCExt_HorCounterExt;0x1
    out       dx, ax
    mov       ah, byte ptr [si + 11h]
    mov       al, MGA_CRTCExt_VertCounterExt;0x2
    out       dx, ax
    call      Func0x5f66                ;Offset 0x5f66
    shr       ebx, 01h
    and       bh, 80h
    jne       Label0x633e               ;Offset 0x633e
    test      byte ptr [si + 0bh], 02h
    jne       Label0x633e               ;Offset 0x633e
    call      Func0x61fb                ;Offset 0x61fb
Label0x633e:                            ;Offset 0x633e
    xor       eax, eax
    call      SetStartAddress           ;Offset 0x5628
    call      TurnScreenOn              ;Offset 0x2b39
    add       sp, 002ch
    xor       ax, ax
    pop       ebx
    pop       es
    pop       bp
    ret       
Func0x626a ENDP

;Offset 0x6351
VESACallTable           DW offset GetSuperVGAInformation;Offset 0x6e75          ;0
                        DW offset GetSuperVGAModeInformation;Offset 0x6f4c      ;1
                        DW offset SetSuperVGAVideoMode;Offset 0x7093            ;2
                        DW offset SuperVGAGetCurrentVideoMode;Offset 0x7124     ;3
                        DW offset SaveRestoreSuperVGAVideoState;Offset 0x714c   ;4
                        DW offset CPUVideoMemoryControl;Offset 0x71bc           ;5
                        DW offset LogicalScanLineLengthHandling;Offset 0x71c5   ;6
                        DW offset DisplayStartHandling;Offset 0x723d            ;7
                        DW offset GetSetDACPaletteControl;Offset 0x72d7         ;8
                        DW offset GetSetPaletteEntries;Offset 0x731a            ;9
                        DW offset GetProtectedModeInterface;Offset 0x7382       ;a

;Offset 0x6367
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
;Offset 0x6389
VBEGfxModeInfoTemplate  DW VESA_MODE_ATTR_ModeGraphics OR VESA_MODE_ATTR_Color OR VESA_MODE_ATTR_BIOSOutputNotSupported OR VESA_MODE_ATTR_OptionalInfoAvailable OR VESA_MODE_ATTR_Supported;0x1B
                        DB VESA_WND_ATTR_Writeable OR VESA_WND_ATTR_Readable OR VESA_WND_ATTR_Exists;0x7
                        DB VESA_WND_ATTR_NotWriteable OR VESA_WND_ATTR_NotReadable OR VESA_WND_ATTR_DoesntExist;0x0
                        DW 00040h       ;VESA_MODE_INFO_WindowGranularityInKB
                        DW 00040h       ;VESA_MODE_INFO_WindowSizeInKB
                        DW 0A000h       ;VESA_MODE_INFO_StartSegmentWindowA
                        DW 00000h       ;VESA_MODE_INFO_StartSegmentWindowB
                        DD 000000000h   ;VESA_MODE_INFO_FARWindowPositioningFunc

;Offset 0x6399
VBETextModeInfoTemplate DW VESA_MODE_ATTR_ModeText OR VESA_MODE_ATTR_Color OR VESA_MODE_ATTR_BIOSOutputSupported OR VESA_MODE_ATTR_OptionalInfoAvailable OR VESA_MODE_ATTR_Supported;0xF
                        DB VESA_WND_ATTR_Writeable OR VESA_WND_ATTR_Readable OR VESA_WND_ATTR_Exists;0x7
                        DB VESA_WND_ATTR_NotWriteable OR VESA_WND_ATTR_NotReadable OR VESA_WND_ATTR_DoesntExist;0x0
                        DW 00020h       ;VESA_MODE_INFO_WindowGranularityInKB
                        DW 00020h       ;VESA_MODE_INFO_WindowSizeInKB
                        DW 0B800h       ;VESA_MODE_INFO_StartSegmentWindowA
                        DW 0B800h       ;VESA_MODE_INFO_StartSegmentWindowB
                        DD 000000000h   ;VESA_MODE_INFO_FARWindowPositioningFunc

;Offset 0x63a9
SupportedVESAVideoModes DW VESA_MODE_640x400x256    ;0x100      2MB
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

;Offset 0x63c1
                        DW VESA_MODE_640x480x32K    ;0x110      4MB
                        DW VESA_MODE_640x480x64K    ;0x111
                        DW VESA_MODE_640x480x16M    ;0x112
                        DW VESA_MODE_800x600x32K    ;0x113
                        DW VESA_MODE_800x600x64K    ;0x114
                        DW VESA_MODE_800x600x16M    ;0x115
                        DW VESA_MODE_1024x768x32K   ;0x116
                        DW VESA_MODE_1024x768x64K   ;0x117
                        DW VESA_MODE_1600x1200x256  ;0x11c
                        DW 0FFFFh

;Offset 0x63d5
                        DW VESA_MODE_1024x768x16M   ;0x118      8MB
                        DW VESA_MODE_1280x1024x32K  ;0x119
                        DW VESA_MODE_1280x1024x64K  ;0x11a
                        DW VESA_MODE_1600x1200x32K  ;0x11d
                        DW VESA_MODE_1600x1200x64K  ;0x11e
                        DW 0FFFFh

;Offet 0x63e1
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x641d
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

;Offset 0x643f
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

;Offset 0x6461
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

;Offset 0x6483
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

;Offset 0x64a5
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

;Offset 0x64c7
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

;Offset 0x64e9
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

;Offset 0x650b
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

;Offset 0x652d
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

;Offset 0x654f
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

;Offset 0x6571
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

;Offset 0x6593
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

;Offset 0x65b5
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

;Offset 0x65d7
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

;Offset 0x65f9
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

;Offset 0x661b
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

;Offset 0x663d
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

;Offset 0x665f
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

;Offset 0x6681
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

;Offset 0x66a3
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

;Offset 0x66c5
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

;Offset 0x66e7
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

;Offset 0x6709
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

;Offset 0x672b
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

;Offset 0x674d
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

;Offset 0x676f
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

;Offset 0x6791
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

;Offset 0x67b3
VESAModeInfoPointers    DW offset VESAModeInfo640x400x256   ;Offset 0x641d
                        DW offset VESAModeInfo640x480x256   ;Offset 0x643f
                        DW offset VESAModeInfo800x600x16    ;Offset 0x6461
                        DW offset VESAModeInfo800x600x256   ;Offset 0x6483
                        DW 0000h
                        DW offset VESAModeInfo1024x768x256  ;Offset 0x64a5
                        DW 0000h
                        DW offset VESAModeInfo1280x1024x256 ;Offset 0x64c7
                        DW offset VESAModeInfo80x60xText    ;Offset 0x64e9
                        DW offset VESAModeInfo132x25xText   ;Offset 0x650b
                        DW offset VESAModeInfo132x43xText   ;Offset 0x652d
                        DW offset VESAModeInfo132x50xText   ;Offset 0x654f
                        DW offset VESAModeInfo132x60xText   ;Offset 0x6571
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW offset VESAModeInfo640x480x32K   ;Offset 0x6593
                        DW offset VESAModeInfo640x480x64K   ;Offset 0x65b5
                        DW offset VESAModeInfo640x480x16M   ;Offset 0x65d7
                        DW offset VESAModeInfo800x600x32K   ;Offset 0x65f9
                        DW offset VESAModeInfo800x600x64K   ;Offset 0x661b
                        DW offset VESAModeInfo800x600x16M   ;Offset 0x663d
                        DW offset VESAModeInfo1024x768x32K  ;Offset 0x665f
                        DW offset VESAModeInfo1024x768x64K  ;Offset 0x6681
                        DW offset VESAModeInfo1024x768x16M  ;Offset 0x66a3
                        DW offset VESAModeInfo1280x1024x32K ;Offset 0x66c5
                        DW offset VESAModeInfo1280x1024x64K ;Offset 0x66e7
                        DW offset VESAModeInfo1280x1024x16M ;Offset 0x6709
                        DW offset VESAModeInfo1600x1200x256 ;Offset 0x672b
                        DW offset VESAModeInfo1600x1200x32K ;Offset 0x674d
                        DW offset VESAModeInfo1600x1200x64K ;Offset 0x676f
                        DW offset VESAModeInfoUnknown       ;Offset 0x6791

;Offset 0x67f3
Data0x67f3              DW Data0x6b2f   ;Offset 0x6b2f VESA_MODE_640x400x256
                        DW Data0x6b45   ;Offset 0x6b45 VESA_MODE_640x480x256
                        DW Data0x6b9d   ;Offset 0x6b9d VESA_MODE_800x600x16
                        DW Data0x6bb5   ;Offset 0x6bb5 VESA_MODE_800x600x256
                        DW 0000h
                        DW Data0x6c0d   ;Offset 0x6c0d VESA_MODE_1024x768x256
                        DW 0000h
                        DW Data0x6c65   ;Offset 0x6c65 VESA_MODE_1280x1024x256
                        DW Data0x6d15   ;Offset 0x6d15 VESA_MODE_80x60xText
                        DW Data0x6d2b   ;Offset 0x6d2b VESA_MODE_132x25xText
                        DW Data0x6d41   ;Offset 0x6d41 VESA_MODE_132x43xText
                        DW Data0x6d57   ;Offset 0x6d57 VESA_MODE_132x50xText
                        DW Data0x6d6d   ;Offset 0x6d6d VESA_MODE_132x60xText
                        DW 0000h
                        DW 0000h
                        DW 0000h
                        DW Data0x6b5b   ;Offset 0x6b5b VESA_MODE_640x480x32K
                        DW Data0x6b71   ;Offset 0x6b71 VESA_MODE_640x480x64K
                        DW Data0x6b87   ;Offset 0x6b87 VESA_MODE_640x480x16M
                        DW Data0x6bcb   ;Offset 0x6bcb VESA_MODE_800x600x32K
                        DW Data0x6be1   ;Offset 0x6be1 VESA_MODE_800x600x64K
                        DW Data0x6bf7   ;Offset 0x6bf7 VESA_MODE_800x600x16M
                        DW Data0x6c23   ;Offset 0x6c23 VESA_MODE_1024x768x32K
                        DW Data0x6c39   ;Offset 0x6c39 VESA_MODE_1024x768x64K
                        DW Data0x6c4f   ;Offset 0x6c4f VESA_MODE_1024x768x16M
                        DW Data0x6c7b   ;Offset 0x6c7b VESA_MODE_1280x1024x32K
                        DW Data0x6c91   ;Offset 0x6c91 VESA_MODE_1280x1024x64K
                        DW Data0x6ca7   ;Offset 0x6ca7 VESA_MODE_1280x1024x16M
                        DW Data0x6cbd   ;Offset 0x6cbd VESA_MODE_1600x1200x256
                        DW Data0x6cd3   ;Offset 0x6cd3 VESA_MODE_1600x1200x32K
                        DW Data0x6ce9   ;Offset 0x6ce9 VESA_MODE_1600x1200x64K

;Offset 0x6831
Data0x6831              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 06Fh, 067h, 04Fh, 04Fh, 00Bh, 053h
                        DB 09Fh, 0C0h, 01Fh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 097h, 0ADh, 08Fh, 050h, 000h
                        DB 08Fh, 0C1h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6871
Data0x6871              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 05Fh, 04Fh, 04Fh, 003h, 051h
                        DB 09Dh, 00Bh, 03Eh, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0E9h, 0ABh, 0DFh, 050h, 000h
                        DB 0DFh, 00Ch, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x68B1
Data0x68B1              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 07Fh, 063h, 063h, 003h, 068h
                        DB 018h, 072h, 0F0h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 064h, 000h
                        DB 057h, 073h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x68F1
Data0x68F1              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 0EFh, 0A3h, 07Fh, 07Fh, 007h, 082h
                        DB 093h, 024h, 0F5h, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 002h, 0A8h, 0FFh, 080h, 000h
                        DB 0FFh, 025h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh
                        DB 0FFh

;Offset 0x6931
Data0x6931              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 0CEh, 09Fh, 09Fh, 012h, 0A5h
                        DB 013h, 028h, 05Ah, 000h, 060h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 0A3h, 0FFh, 0A0h, 000h
                        DB 0FFh, 029h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh
                        DB 00Ch, 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x6970
Data0x6970              DB 050h, 00Bh, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 02Fh, 009h, 0C7h, 0C7h, 00Dh, 0CFh, 007h
                        DB 0E0h, 000h, 000h, 040h, 020h, 000h, 000h, 000h, 000h, 000h, 0B0h, 0A3h, 0AFh, 0C8h, 000h, 0AFh
                        DB 0E1h, 0C3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 002h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh

;Offset 0x69AF
Data0x69AF              DB 064h
                        DB 024h, 010h, 000h, 0F0h, 001h, 00Fh, 000h, 006h, 02Fh, 07Fh, 063h, 063h, 082h, 06Bh, 01Bh, 072h
                        DB 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 058h, 0ACh, 057h, 032h, 000h, 057h, 073h
                        DB 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

;Offset 0x69EF
Data0x69EF              DB 050h
                        DB 03Bh, 008h, 080h, 025h, 001h, 003h, 000h, 002h, 0EFh, 060h, 04Fh, 050h, 083h, 052h, 09Eh, 00Bh
                        DB 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EBh, 0ACh, 0DFh, 028h, 01Fh, 0E6h, 006h
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6A2F
Data0x6A2F              DB 084h
                        DB 018h, 010h, 0D0h, 019h, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 0C0h
                        DB 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Eh, 0A6h, 08Fh, 042h, 01Fh, 096h, 0BBh
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6A6F
Data0x6A6F              DB 084h
                        DB 02Ah, 008h, 058h, 02Ch, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 088h
                        DB 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 066h, 0A4h, 057h, 042h, 01Fh, 05Eh, 083h
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6AAF
Data0x6AAF              DB 084h
                        DB 031h, 008h, 000h, 034h, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 0C0h
                        DB 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 09Eh, 0A6h, 08Fh, 042h, 01Fh, 096h, 0BBh
                        DB 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        DB 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

;Offset 0x6AEF
Data0x6AEF              DB 084h
                        Db 03Bh, 008h, 000h, 03Eh, 001h, 003h, 000h, 002h, 0EFh, 09Eh, 083h, 084h, 001h, 087h, 08Dh, 011h
                        Db 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EBh, 0AEh, 0DFh, 042h, 01Fh, 0E6h, 00Ch
                        Db 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh
                        Db 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh, 

;Offset 0x6b2f      ;VESA_MODE_640x400x256
Data0x6b2f              DB 086h, 06Ah, 000h, 000h
                        DW 00280h
                        DW 00190h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6831
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 000h

;Offset 0x6b45      ;VESA_MODE_640x480x256
Data0x6b45              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6871
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6b5b      ;VESA_MODE_640x480x32K
Data0x6b5b              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 00Fh, 050h
                        DW offset Data0x6871
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6b71      ;VESA_MODE_640x480x64K
Data0x6b71              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 010h, 010h
                        DW offset Data0x6871
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6b87      ;VESA_MODE_640x480x16M
Data0x6b87              DB 057h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 000h, 001h, 020h, 030h
                        DW offset Data0x6871
                        DB 000h, 000h, 040h, 000h, 01Ch, 013h, 004h, 02Eh

;Offset 0x6b9d      ;VESA_MODE_800x600x16
Data0x6b9d              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 010h, 000h, 001h, 009h
                        DW offset Data0x69AF
                        DB 000h, 000h, 000h, 000h, 01Bh, 012h, 000h, 000h, 007h, 016h

;Offset 0x6bb5      ;VESA_MODE_800x600x256
Data0x6bb5              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x68B1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 007h, 016h 

;Offset 0x6bcb      ;VESA_MODE_800x600x32K
Data0x6bcb              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x68B1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6be1      ;VESA_MODE_800x600x64K
Data0x6be1              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x68B1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6bf7      ;VESA_MODE_800x600x16M
Data0x6bf7              DB 040h, 09Ch, 000h, 000h
                        DW 00320h
                        DW 00258h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x68B1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c0d      ;VESA_MODE_1024x768x256
Data0x6c0d              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x68F1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c23      ;VESA_MODE_1024x768x32K
Data0x6c23              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x68F1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c39      ;VESA_MODE_1024x768x64K
Data0x6c39              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x68F1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c4f      ;VESA_MODE_1024x768x16M
Data0x6c4f              DB 0E8h, 0FDh, 000h, 000h
                        DW 00400h
                        DW 00300h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x68F1
                        DB 000h, 000h, 000h, 000h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c65      ;VESA_MODE_1280x1024x256
Data0x6c65              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6931
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c7b      ;VESA_MODE_1280x1024x32K
Data0x6c7b              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6931
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6c91      ;VESA_MODE_1280x1024x64K
Data0x6c91              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6931
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6ca7      ;VESA_MODE_1280x1024x16M
Data0x6ca7              DB 0E0h, 0A5h, 001h, 000h
                        DW 00500h
                        DW 00400h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6931
                        DB 000h, 000h, 040h, 021h, 01Ch, 013h, 00Ch, 000h

;Offset 0x6cbd      ;VESA_MODE_1600x1200x256
Data0x6cbd              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 000h, 001h, 008h, 000h
                        DW offset Data0x6970
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6cd3      ;VESA_MODE_1600x1200x32K
Data0x6cd3              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 0FFh, 07Fh, 008h, 050h
                        DW offset Data0x6970
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6ce9      ;VESA_MODE_1600x1200x64K
Data0x6ce9              DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 0FFh, 0FFh, 008h, 010h
                        DW offset Data0x6970
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6cff      ;VESA_MODE_1600x1200x16M (unreferenced)
                        DB 0D0h, 078h, 002h, 000h
                        DW 00640h
                        DW 004B0h
                        DB 000h, 001h, 008h, 030h
                        DW offset Data0x6970
                        DB 000h, 000h, 001h, 02Dh, 01Ch, 013h, 00Ch, 000h

;Offset 0x6d15      ;VESA_MODE_80x60xText
Data0x6d15              DB 070h, 062h, 000h, 000h
                        DW 00280h
                        DW 001E0h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x69EF
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6d2b      ;VESA_MODE_132x25xText
Data0x6d2b              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00190h
                        DB 010h, 000h, 001h, 002h
                        DW offset Data0x6A2F
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6d41      ;VESA_MODE_132x43xText
Data0x6d41              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00158h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6A6F
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6d57      ;VESA_MODE_132x50xText
Data0x6d57              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 00190h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6AAF
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6d6d      ;VESA_MODE_132x60xText
Data0x6d6d              DB 078h, 0A0h, 000h, 000h
                        DW 00420h
                        DW 001E0h
                        DB 010h, 000h, 001h, 082h
                        DW offset Data0x6AEF
                        DB 000h, 000h, 000h, 000h, 018h, 003h, 000h, 000h

;Offset 0x6d83
OEMName                 DB 'Matrox Graphics Inc.', 000h

;Offset 0x6d98
VendorName              DB 'Matrox', 000h

;Offset 0x6d9f
ProductName             DB 'MGA-G100', 000h

;Offset 0x6da8
RevisionString          DB '00', 000h

;Offset 0x6dab
VBEMGATemplate          DB 'VBE/MGA'
                        DB 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h
                        DW 00106h   ;BIOS Version (1.6)

;Offset 0x6dbe
MatroxOEMExtensionTable DW offset InstallationCheckGetCaps;Offset 0x740a
                        DW offset AccessMGAIndexedDataRegister;Offset 0x748f
                        DW offset AccessMGAIndirectRegister;Offset 0x74a5
                        DW offset Label0x74e2;Offset 0x74e2
                        DW offset Label0x74f1;Offset 0x74f1
                        DW offset Label0x750c;Offset 0x750c

VESAHandler:                            ;Offset 0x6dca
    cmp       al, 14h                   ;Matrox OEM extension
    je        Label0x6dde               ;Offset 0x6dde
    cmp       al, 0ah
    ja        Label0x6df0               ;Offset 0x6df0
    sti
    cld
    xor       ah, ah
    shl       ax, 01h
    xchg      ax, bx
    jmp       word ptr cs:[bx + VESACallTable];Offset 0x6351
Label0x6dde:                            ;Offset 0x6dde
    cmp       bl, 05h
    ja        Label0x6df0               ;Offset 0x6df0
    mov       ax, bx
    and       bx, 00ffh
    shl       bx, 01h
    jmp       word ptr cs:[bx + MatroxOEMExtensionTable];Offset 0x6dbe
Label0x6df0:                            ;Offset 0x6df0
    mov       ax, 0ffffh
    iret

Func0x6df4 PROC NEAR                    ;Offset 0x6df4
    push cx
    cmp  bx, 81ffh
    jne  Label0x6e01                    ;Offset 0x6e01
    mov  bx, 011fh
    jmp  Label0x6e16                    ;Offset 0x6e16
    nop
Label0x6e01:                            ;Offset 0x6e01
    mov  cx, bx
    and  cx, 01ffh
    cmp  cx, 006ah
    ;jne  Label0x6e16                    ;Offset 0x6e16
    DB 00Fh, 085h, 008h, 000h
    and  bx, 0fe00h
    or   bx, 0102h
Label0x6e16:                            ;Offset 0x6e16
    pop  cx
    ret
Func0x6df4 ENDP

Func0x6e18 PROC NEAR                    ;Offset 0x6e18
    push  ax
    push  bx
    push  si
    push  ds
    push  cs
    pop   ds
    and   bx, 01ffh
    mov   si, offset SupportedVESAVideoModes;Offset 0x63a9
Label0x6e25:                            ;Offset 0x6e25
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x6e35                   ;Offset 0x6e35
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x6e6e                   ;Offset 0x6e6e
    DB 00Fh, 084h, 03Bh, 000h
    jmp   Label0x6e25                   ;Offset 0x6e25
Label0x6e35:                            ;Offset 0x6e35
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x6e45                   ;Offset 0x6e45
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x6e6e                   ;Offset 0x6e6e
    DB 00Fh, 084h, 02Bh, 000h
    jmp   Label0x6e35                   ;Offset 0x6e35
Label0x6e45:                            ;Offset 0x6e45
    call  GetMemoryIn64KBlocks          ;Offset 0x5448
    call  CapAXTo0x40                   ;Offset 0x543d
    ;cmp   ax, 0020h
    DB 03Dh, 020h, 000h
    ;jne   Label0x6e58                   ;Offset 0x6e58
    DB 00Fh, 085h, 006h, 000h
    ;or    ax, 0001h
    DB 00Dh, 001h, 000h
    jmp   Label0x6e70                   ;Offset 0x6e70
    nop
Label0x6e58:                            ;Offset 0x6e58
    lodsw
    ;cmp   ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    ;je    Label0x6e68                   ;Offset 0x6e68
    DB 00Fh, 084h, 008h, 000h
    cmp   bx, ax
    ;je    Label0x6e6e                   ;Offset 0x6e6e
    DB 00Fh, 084h, 008h, 000h
    jmp   Label0x6e58                   ;Offset 0x6e58
Label0x6e68:                            ;Offset 0x6e68
    ;or    ax, 0001h
    DB 00Dh, 001h, 000h
    jmp   Label0x6e70                   ;Offset 0x6e70
    nop
Label0x6e6e:                            ;Offset 0x6e6e
    xor   ax, ax
Label0x6e70:                            ;Offset 0x6e70
    pop   ds
    pop   si
    pop   bx
    pop   ax
    ret
Func0x6e18 ENDP

GetSuperVGAInformation:                 ;Offset 0x6e75
    mov       bx, ax
    push      ds
    push      si
    push      cx
    push      di
    push      cs
    pop       ds
    mov       ax, 4256h                 ;'VB'
    cmp       ax, word ptr es:[di + VESA_INFO_Signature];0x0
    jne       Label0x6ea5               ;Offset 0x6ea5
    mov       ax, 3245h                 ;'E2'
    cmp       ax, word ptr es:[di + VESA_INFO_Signature + 02h];0x2
    jne       Label0x6ea5               ;Offset 0x6ea5
    push      di
    add       di, 0100h
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    mov       cx, 0013h
    mov       si, offset VendorName     ;Offset 0x6d98
    rep movsb
    pop       di
Label0x6ea5:                            ;Offset 0x6ea5
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       cx, 0022h
    mov       si, offset VESAResponseTemplate;Offset 0x6367
    rep movsb
    pop       di
    mov       al, 16h                   ;OEM Version 1.6
    shl       ax, 04h
    mov       al, 16h                   ;OEM Version 1.6
    and       al, 0fh
    mov       word ptr es:[di + VESA_INFO_OEMSoftwareVersion], ax;0x14
    mov       ax, cs
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerSeg], ax;0x8
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerSeg], ax;0x18
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerSeg], ax;0x1c
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerSeg], ax;0x20
    mov       ax, offset OEMName        ;Offset 0x6d83
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerOfs], ax;0x6
    mov       ax, offset VendorName     ;Offset 0x6d98
    mov       word ptr es:[di + VESA_INFO_VendorNamePointerOfs], ax;0x16
    mov       ax, offset ProductName    ;Offset 0x6d9f
    mov       word ptr es:[di + VESA_INFO_ProductNamePointerOfs], ax;0x1a
    mov       ax, offset RevisionString ;Offset 0x6da8
    mov       word ptr es:[di + VESA_INFO_RevisionStringPointerOfs], ax;0x1e
    call      GetMemoryIn64KBlocks      ;Offset 0x5448
    mov       word ptr es:[di + VESA_INFO_MemoryIn64KBBlocks], ax;0x12
    call      CapAXTo0x40               ;Offset 0x543d
    mov       cl, al
    mov       ax, es
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerSeg], es;0x10
    mov       ax, di
    ;add       ax, 0022h
    DB 005h, 022h, 000h
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerOfs], ax;0xe
    push      bx
    push      di
    mov       di, ax
    mov       si, offset SupportedVESAVideoModes;Offset 0x63a9
Label0x6f14:                            ;Offset 0x6f14
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x6f1d               ;Offset 0x6f1d
    stosw
    jmp       Label0x6f14               ;Offset 0x6f14
Label0x6f1d:                            ;Offset 0x6f1d
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x6f26               ;Offset 0x6f26
    stosw
    jmp       Label0x6f1d               ;Offset 0x6f1d
Label0x6f26:                            ;Offset 0x6f26
    cmp       cl, 20h
    je        Label0x6f42               ;Offset 0x6f42
Label0x6f2b:                            ;Offset 0x6f2b
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x6f34               ;Offset 0x6f34
    stosw
    jmp       Label0x6f2b               ;Offset 0x6f2b
Label0x6f34:                            ;Offset 0x6f34
    cmp       cl, 40h
    je        Label0x6f42               ;Offset 0x6f42
Label0x6f39:                            ;Offset 0x6f39
    lodsw
    ;cmp       ax, 0ffffh
    DB 03Dh, 0FFh, 0FFh
    je        Label0x6f42               ;Offset 0x6f42
    stosw
    jmp       Label0x6f39               ;Offset 0x6f39
Label0x6f42:                            ;Offset 0x6f42
    stosw
    pop       di
    pop       bx
    mov       ax, 004fh
    pop       cx
    pop       si
    pop       ds
    iret

GetSuperVGAModeInformation:             ;Offset 0x6f4c
    push      ax
    push      ds
    push      di
    push      si
    push      ecx
    push      edx
    mov       bx, cx
    call      Func0x6df4                ;Offset 0x6df4
    cmp       bx, 011fh
    ;je        Label0x6f72               ;Offset 0x6f72
    DB 00Fh, 084h, 011h, 000h
    call      Func0x6e18                ;Offset 0x6e18
    ;jne       Label0x6f6d               ;Offset 0x6f6d
    DB 00Fh, 085h, 005h, 000h
    and       bx, 007fh
    jmp       Label0x6f72               ;Offset 0x6f72
Label0x6f6d:                            ;Offset 0x6f6d
    mov       ah, 01h
    jmp       Label0x7088               ;Offset 0x7088
Label0x6f72:                            ;Offset 0x6f72
    push      eax
    push      di
    mov       cx, 0080h
    xor       ax, ax
    rep stosw
    pop       di
    push      di
    mov       ax, cs
    mov       ds, ax
    mov       cx, 0010h
    ;mov       si, VBEGfxModeInfoTemplate;Offset 0x6389
    DB 0BEh, 089h, 063h
    cmp       bx, VESA_MODE_80x60xText AND 7fh;0x8
    jb        IsGraphicsMode            ;Offset 0x6f98
    cmp       bx, VESA_MODE_132x60xText AND 7fh;0xc
    ja        IsGraphicsMode            ;Offset 0x6f98
    mov       cx, 0010h
    ;mov       si, VBETextModeInfoTemplate;Offset 0x6399
    DB 0BEh, 099h, 063h
IsGraphicsMode:                         ;Offset 0x6f98
    cld
    rep movsb
    shl       bx, 01h
    mov       si, word ptr cs:[bx + VESAModeInfoPointers];Offset 0x67b3
    push      si
    mov       cx, 0022h
    rep movsb
    pop       si
    pop       di
    push      ebx
    push      edx
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType], VESA_MEM_MODEL_EGA_Graphics;0x1b 0x3
    ;je        Label0x6ff8               ;Offset 0x6ff8
    DB 00Fh, 084h, 041h, 000h
    call      GetMemoryIn64KBlocks      ;Offset 0x5448
    call      CapAXTo0x40               ;Offset 0x543d
    ;cmp       ax, 0020h
    DB 03Dh, 020h, 000h
    ;je        Label0x6ff8               ;Offset 0x6ff8
    DB 00Fh, 084h, 034h, 000h
    xor       dx, dx
    mov       bx, word ptr [si + VESA_MODE_INFO_BytesPerScanLine - 10h];0x0
    mov       ax, word ptr [si + VESA_MODE_INFO_HeightInPixelsOrChars - 10h];0x04
    mul       bx
    mov       bx, 0ffffh
    div       bx
    or        dx, dx
    ;je        Label0x6fd9               ;Offset 0x6fd9
    DB 00Fh, 084h, 001h, 000h
    inc       ax
Label0x6fd9:                            ;Offset 0x6fd9
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
Label0x6ff8:                            ;Offset 0x6ff8
    pop       edx
    pop       ebx
    mov       al, byte ptr [si + VESA_MODE_INFO_MemoryModelType - 010h]   ;0xb
    cmp       al, VESA_MEM_MODEL_Text   ;0x0
    jne       Label0x700d               ;Offset 0x700d
    mov       ax, VESA_MODE_ATTR_ModeGraphics;0x10
    not       al
    and       word ptr es:[di + VESA_MODE_INFO_Attributes], ax
    jmp       Label0x7017               ;Offset 0x7017
Label0x700d:                            ;Offset 0x700d
    cmp       al, VESA_MEM_MODEL_EGA_Graphics;0x3
    je        Label0x7017               ;Offset 0x7017
    mov       ax, VESA_MODE_ATTR_LinearSupported;0x80
    or        word ptr es:[di + VESA_MODE_INFO_Attributes], ax
Label0x7017:                            ;Offset 0x7017
    mov       ax, cs
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningSeg], ax;0xe
    mov       ax, offset AccessMemoryPageFAR;Offset 0x7199
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningOfs], ax;0xc
    add       di, 0010h
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType - 10h], VESA_MEM_MODEL_EGA_Graphics;0xb 0x3
    ;je        Label0x7084               ;Offset 0x7084
    DB 00Fh, 084h, 054h, 000h
    cmp       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType - 10h], VESA_MEM_MODEL_Text;0xb 0x0
    ;je        Label0x7084               ;Offset 0x7084
    DB 00Fh, 084h, 04Bh, 000h
    call      FindMGAG100               ;Offset 0x53d6
    call      ReadPCIBase0AndBase1      ;Offset 0x53e2
    ;mov       dword ptr es:[di + VESA_MODE_INFO_LinearFrameBufferAddress - 10h], ecx;0x18
    DB 026h, 066h, 089h, 04Dh, 018h
    xor       dx, dx
    mov       ax, word ptr es:[di + VESA_MODE_INFO_HeightInPixelsOrChars - 10h];0x4
    mul       word ptr es:[di + VESA_MODE_INFO_BytesPerScanLine - 10h];0x0
    shl       edx, 10h
    mov       dx, ax
    mov       ecx, edx
    and       ecx, 000003ffh
    ;je        Label0x7068               ;Offset 0x7068
    DB 00Fh, 084h, 007h, 000h
    add       edx, 00000400h
Label0x7068:                            ;Offset 0x7068
    and       edx, 0fffffc00h
    ;mov       dword ptr es:[di + VESA_MODE_INFO_OffScreenMemoryAddress - 10h], edx;0x1c
    DB 026h, 066h, 089h, 055h, 01Ch
    shr       edx, 0ah
    call      GetMemoryIn64KBlocks      ;Offset 0x5448
    shl       ax, 06h
    sub       ax, dx
    mov       word ptr es:[di + VESA_MODE_INFO_OffScreenMemorySizeInKB - 10h], ax;0x20
Label0x7084:                            ;Offset 0x7084
    pop       eax
    mov       ah, 00h
Label0x7088:                            ;Offset 0x7088
    mov       al, 4fh
    pop       edx
    pop       ecx
    pop       si
    pop       di
    pop       ds
    pop       bx
    iret      

SetSuperVGAVideoMode:                   ;Offset 0x7093
    push  ax
    push  ds
    push  di
    push  si
    push  cx
    push  dx
    push  cs
    pop   ds
    mov   bx, ax
    call  IsExtensionReg7Writeable      ;Offset 0x5216
    ;je    Label0x711d                   ;Offset 0x711d
    DB 00Fh, 084h, 079h, 000h
    push  bx
    call  Func0x6df4                    ;Offset 0x6df4
    mov   ax, bx
    cmp   bl, 1fh
    jne   Label0x70b2                   ;Offset 0x70b2
    mov   bx, VESA_MODE_640x400x256     ;0x100
Label0x70b2:                            ;Offset 0x70b2
    call  Func0x6e18                    ;Offset 0x6e18
    jne   Label0x70e1                   ;Offset 0x70e1
    and   bx, 00ffh
    shl   bx, 01h
    ;add   bx, Data0x67f3                ;Offset 0x67f3
    DB 081h, 0C3h, 0F3h, 067h
    mov   si, word ptr cs:[bx]
    push  ax
    mov   ax, 0bd60h
    int   6dh
    pop   ax
    push  ds
    push  si
    call  Func0x626a                    ;Offset 0x626a
    pop   ax
    pop   ax
    pop   bx
    cmp   bl, 1fh
    jne   Label0x70df                   ;Offset 0x70df
    mov   dx, VGA_SequenceIndex         ;Port 0x3c4
    mov   ax, (VGA_SEQ1_ScreenOff SHL 8) OR VGA_SEQIdx_ClockingMode;0x2001
    out   dx, ax
Label0x70df:                            ;Offset 0x70df
    jmp   Label0x70fb                   ;Offset 0x70fb
Label0x70e1:                            ;Offset 0x70e1
    pop   bx
    mov   ax, bx
    ;cmp   ax, BDA_DM_320x200_256_Color_Graphics;0x13
    DB 03Dh, 013h, 000h
    jle   Label0x70ee                   ;Offset 0x70ee
    mov   ax, 014fh
    jmp   Label0x711d                   ;Offset 0x711d
Label0x70ee:                            ;Offset 0x70ee
    movzx ax, bl
    int   10h                           ;Regular int 10h handler for standard modes
    inc   ax
    jne   Label0x70fb                   ;Offset 0x70fb
    mov   ax, 014fh
    jmp   Label0x711d                   ;Offset 0x711d
Label0x70fb:                            ;Offset 0x70fb
    push  bx
    and   bx, 7f7fh
    cmp   bx, VESA_MODE_640x400x256     ;0x100
    jl    Label0x7109                   ;Offset 0x7109
    add   bl, 20h                       ;VESA mode is (mode & 0x7f) + 0x20
Label0x7109:                            ;Offset 0x7109
    xor   ax, ax
    mov   ds, ax
    mov   al, bl
    pop   bx
    mov   ah, bh
    and   ah, 0c0h                      ;Top 2 bits are preserved
    or    al, ah                        ;And stored along with the mode
    mov   byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov   ax, 004fh
Label0x711d:                            ;Offset 0x711d
    pop   dx
    pop   cx
    pop   si
    pop   di
    pop   ds
    pop   bx
    iret

SuperVGAGetCurrentVideoMode:            ;Offset 0x7124
    mov  ax, 0f00h
    int  10h
    xor  bx, bx
    mov  ah, al
    and  ah, 0c0h
    and  al, 3fh
    cmp  al, 20h
    jl   Label0x7146                    ;Offset 0x7146
    cmp  al, 40h
    jge  Label0x7146                    ;Offset 0x7146
    sub  al, 20h
    inc  bh
    or   bh, ah
    cmp  al, 1fh
    jne  Label0x7146                    ;Offset 0x7146
    mov  al, 0ffh
Label0x7146:                            ;Offset 0x7146
    mov  bl, al
    mov  ax, 004fh
    iret

SaveRestoreSuperVGAVideoState:          ;Offset 0x714c
    mov  bx, ax
    mov  ah, 1ch
    mov  al, dl
    push ax
    push cx
    test cx, 0008h
    je   Label0x7184                    ;Offset 0x7184
    cmp  al, 00h
    jne  Label0x716a                    ;Offset 0x716a
    cmp  cx, 0008h
    jne  Label0x7184                    ;Offset 0x7184
    mov  bx, 0001h
    pop  cx
    pop  ax
    jmp  Label0x7195                    ;Offset 0x7195
Label0x716a:                            ;Offset 0x716a
    cmp  al, 01h
    jne  Label0x7177                    ;Offset 0x7177
    push bx
    call SaveMGAState                   ;Offset 0x6130
    int  10h
    pop  bx
    jmp  Label0x7186                    ;Offset 0x7186
Label0x7177:                            ;Offset 0x7177
    cmp  al, 02h
    jne  Label0x7184                    ;Offset 0x7184
    push bx
    call RestoreMGAState                ;Offset 0x618b
    int  10h
    pop  bx
    jmp  Label0x7186                    ;Offset 0x7186
Label0x7184:                            ;Offset 0x7184
    int  10h
Label0x7186:                            ;Offset 0x7186
    pop  cx
    pop  ax
    test cx, 0008h
    je   Label0x7195                    ;Offset 0x7195
    cmp  al, 00h
    jne  Label0x7195                    ;Offset 0x7195
    add  bx, 0001h
Label0x7195:                            ;Offset 0x7195
    mov  ax, 004fh
    iret

AccessMemoryPageFAR PROC FAR            ;Offset 0x7199
    call AccessMemoryPage               ;Offset 0x719d
    retf
AccessMemoryPageFAR ENDP

AccessMemoryPage PROC NEAR              ;Offset 0x719d
    push ds
    push dx
    push si
    or   bh, bh
    jne  ReadPage                       ;Offset 0x71af
    mov  ah, MGA_CRTCExt_MemoryPage     ;0x4
    mov  al, dl
    call MGAWriteCRTCExtensionRegister  ;Offset 0x3da
    pop  si
    pop  dx
    pop  ds
    ret
ReadPage:                               ;Offset 0x71af
    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
    call MGAReadCRTCExtensionRegister   ;Offset 0x3c4
    mov  dl, al
    xor  dh, dh
    pop  si
    pop  dx
    pop  ds
    ret
AccessMemoryPage ENDP

CPUVideoMemoryControl:                  ;Offset 0x71bc
    mov  bx, ax
    call AccessMemoryPage               ;Offset 0x719d
    mov  ax, 004fh
    iret

LogicalScanLineLengthHandling:          ;Offset 0x71c5
    mov  bx, ax
    call Func0x55c8                     ;Offset 0x55c8
    and  al, 03h
    je   Label0x7219                    ;Offset 0x7219
    push ds
    push si
    cmp  bl, 01h
    je   Label0x7210                    ;Offset 0x7210
    call Func0x73e0                     ;Offset 0x73e0
    cmp  bl, 03h
    jne  Label0x71e4                    ;Offset 0x71e4
    call Func0x73a2                     ;Offset 0x73a2
    mov  cx, ax
    jmp  Label0x7213                    ;Offset 0x7213
Label0x71e4:                            ;Offset 0x71e4
    cmp  bl, 02h
    jne  Label0x71f1                    ;Offset 0x71f1
Label0x71e9:                            ;Offset 0x71e9
    dec  al
    je   Label0x71f1                    ;Offset 0x71f1
    shr  cx, 01h
    jmp  Label0x71e9                    ;Offset 0x71e9
Label0x71f1:                            ;Offset 0x71f1
    mov  ax, word ptr [si + 04h]
    cmp  ax, cx
    jb   Label0x71fa                    ;Offset 0x71fa
    mov  cx, ax
Label0x71fa:                            ;Offset 0x71fa
    mov  ax, cx
    call Func0x5752                     ;Offset 0x5752
    mov  cx, ax
    call Func0x73a2                     ;Offset 0x73a2
    cmp  ax, cx
    mov  ax, 0024fh
    jb   Label0x7216                    ;Offset 0x7216
    mov  ax, cx
    call Func0x5710                     ;Offset 0x5710
Label0x7210:                            ;Offset 0x7210
    call Func0x56d3                     ;Offset 0x56d3
Label0x7213:                            ;Offset 0x7213
    mov  ax, 004fh
Label0x7216:                            ;Offset 0x7216
    pop  si
    pop  ds
    iret
Label0x7219:                            ;Offset 0x7219
    cmp  bl, 03h
    jne  Label0x7223                    ;Offset 0x7223
    mov  cx, 0320h
    jmp  Label0x7239                    ;Offset 0x7239
Label0x7223:                            ;Offset 0x7223
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
Label0x7239:                            ;Offset 0x7239
    mov  ax, 004fh
    iret

DisplayStartHandling:                   ;Offset 0x723d
    mov  bx, ax
    call Func0x55c8                     ;Offset 0x55c8
    and  al, 03h
    je   Label0x7262                    ;Offset 0x7262
    cmp  bl, 01h
    je   Label0x725b                    ;Offset 0x725b
    or   bl, bl
    je   Label0x7254                    ;Offset 0x7254
    cmp  bl, 80h
    jne  Label0x72d1                    ;Offset 0x72d1
Label0x7254:                            ;Offset 0x7254
    call Func0x5693                     ;Offset 0x5693
    mov  ax, 004fh
    iret
Label0x725b:                            ;Offset 0x725b
    call Func0x565c                     ;Offset 0x565c
    mov  ax, 004fh
    iret
Label0x7262:                            ;Offset 0x7262
    push si
    push ds
    push 0000h
    pop  ds
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    pop  ds
    mov  si, 0064h
    cmp  al, 22h
    ;je   Label0x7280                    ;Offset 0x7280
    DB 00Fh, 084h, 00Ch, 000h
    mov  si, 0050h
    cmp  al, 28h
    ;je   Label0x7280                    ;Offset 0x7280
    DB 00Fh, 084h, 003h, 000h
    mov  si, 0084h
Label0x7280:                            ;Offset 0x7280
    and  bl, 01h
    jne  Label0x72ac                    ;Offset 0x72ac
    or   bl, bl
    je   Label0x728e                    ;Offset 0x728e
    cmp  bl, 80h
    jne  Label0x72d1                    ;Offset 0x72d1
Label0x728e:                            ;Offset 0x728e
    push cx
    push dx
    xor  al, al
    call Func0x55de                     ;Offset 0x55de
    mov  ax, si
    mul  dx
    push dx
    push ax
    pop  eax
    shr  cx, 03h
    add  ax, cx
    call SetStartAddress                ;Offset 0x5628
    pop  dx
    pop  cx
    pop  si
    mov  ax, 004fh
    iret
Label0x72ac:                            ;Offset 0x72ac
    call GetStartAddress                ;Offset 0x5601
    xor  cx, cx
    xor  dx, dx
    or   eax, eax
    je   Label0x72c7                    ;Offset 0x72c7
    push eax
    pop  ax
    pop  dx
    mov  bx, si
    div  bx
    mov  cx, dx
    shl  cx, 03h
    mov  dx, ax
Label0x72c7:                            ;Offset 0x72c7
    mov  al, 01h
    call Func0x55de                     ;Offset 0x55de
    pop  si
    mov  ax, 004fh
    iret
Label0x72d1:                            ;Offset 0x72d1
    pop  si
    mov  ah, 01h
    mov  al, 4fh
    iret

GetSetDACPaletteControl:                ;Offset 0x72d7
    push cx
    push dx
    mov  dx, ax
    call FindMGAG100                    ;Offset 0x53d6
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call ReadIndexedRegister            ;Offset 0x578f
    mov  ah, 01h
    or   dl, dl
    je   Label0x72fb                    ;Offset 0x72fb
    cmp  dl, 01h
    jne  Label0x7313                    ;Offset 0x7313
    xor  ah, ah
    mov  dh, 06h
    and  cl, MGA_MISCCTRL_DAC_Bit_MASK  ;0x8
    je   Label0x7313                    ;Offset 0x7313
    mov  dh, 08h
    jmp  Label0x7313                    ;Offset 0x7313
Label0x72fb:                            ;Offset 0x72fb
    and  cl, NOT MGA_MISCCTRL_DAC_Bit_MASK;0xf7
    cmp  dh, 08h
    mov  dh, 06h
    jb   Label0x730a                    ;Offset 0x730a
    mov  dh, 08h
    or   cl, MGA_MISCCTRL_DAC_8Bit      ;0x8
Label0x730a:                            ;Offset 0x730a
    xchg cl, ch
    mov  cl, MGA_INDD_MiscellaneousControl;0x1e
    call WriteIndexedRegister           ;Offset 0x5774
    xor  ah, ah
Label0x7313:                            ;Offset 0x7313
    mov  bx, dx
    pop  dx
    pop  cx
    mov  al, 4fh
    iret

GetSetPaletteEntries:                   ;Offset 0x731a
    mov  bx, ax
    cmp  bl, 02h
    je   Label0x737e                    ;Offset 0x737e
    cmp  bl, 03h
    je   Label0x737e                    ;Offset 0x737e
    push bx
    push cx
    push dx
    push di
    mov  al, dl
    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
    out  dx, al
    mov  al, bl
    cmp  al, 80h
    jne  Label0x7342                    ;Offset 0x7342
    mov  dl, VGA_InputStatus1D_lowbyte  ;Port 0x3da
Label0x7338:                            ;Offset 0x7338
    in   al, dx
    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    jne  Label0x7338                    ;Offset 0x7338
Label0x733d:                            ;Offset 0x733d
    in   al, dx
    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    je   Label0x733d                    ;Offset 0x733d
Label0x7342:                            ;Offset 0x7342
    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
    mov  al, bl
    and  al, 01h
    je   Label0x7361                    ;Offset 0x7361
Label0x734a:                            ;Offset 0x734a
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
    jne  Label0x734a                    ;Offset 0x734a
    jmp  Label0x7376                    ;Offset 0x7376
Label0x7361:                            ;Offset 0x7361
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
    jne  Label0x7361                    ;Offset 0x7361
Label0x7376:                            ;Offset 0x7376
    pop  di
    pop  dx
    pop  cx
    pop  bx
    mov  ax, 004fh
    iret
Label0x737e:                            ;Offset 0x737e
    mov  ax, 024fh
    iret

GetProtectedModeInterface:              ;Offset 0x7382
    mov  bx, ax
    mov  ah, 01h
    or   bl, bl
    jne  Label0x739f                    ;Offset 0x739f
    mov  ax, cs
    mov  es, ax
    mov  di, offset PINS                ;Offset 0x7520
    xor  ax, ax
    mov  al, byte ptr es:[di + 02h]
    add  di, ax
    mov  cx, word ptr es:[di + 08h]
    xor  ah, ah
Label0x739f:                            ;Offset 0x739f
    mov  al, 4fh
    iret

Func0x73a2 PROC NEAR                    ;Offset 0x73a2
    push dx
    push cx
    call Func0x55c8                     ;Offset 0x55c8
    xor  dx, dx
    mov  dl, ah
    mov  cl, al
    and  cl, 03h
    jne  Label0x73b7                    ;Offset 0x73b7
    mov  ax, word ptr [si + 04h]
    jmp  Label0x73dd                    ;Offset 0x73dd
Label0x73b7:                            ;Offset 0x73b7
    dec  cl
    push bx
    push cx
    mov  ax, word ptr [si + 06h]
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
    jbe  Label0x73da                    ;Offset 0x73da
    mov  ax, 1f80h
Label0x73da:                            ;Offset 0x73da
    call Func0x5752                     ;Offset 0x5752
Label0x73dd:                            ;Offset 0x73dd
    pop  cx
    pop  dx
    ret
Func0x73a2 ENDP

Func0x73e0 PROC NEAR                    ;Offset 0x73e0
    push di
    xor  ax, ax
    mov  ds, ax
    ;mov  si, Data0x67f3                 ;Offset 0x67f3
    DB 0BEh, 0F3h, 067h
    mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    and  al, 3fh
    cmp  al, 20h
    jl   Label0x7404                    ;Offset 0x7404
    cmp  al, 40h
    jge  Label0x7404                    ;Offset 0x7404
    sub  al, 20h
    shl  ax, 01h
    add  si, ax
    shr  ax, 01h
    inc  ah
    mov  si, word ptr cs:[si]
    jmp  Label0x7406                    ;Offset 0x7406
Label0x7404:                            ;Offset 0x7404
    xor  si, si
Label0x7406:                            ;Offset 0x7406
    push cs
    pop  ds
    pop  di
    ret
Func0x73e0 ENDP

InstallationCheckGetCaps:               ;Offset 0x740a
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
    mov       si, offset VBEMGATemplate ;Offset 0x6dab
    rep movsb
    pop       di
    mov       ax, cs
    mov       word ptr es:[di + VBE_MGA_VendorNameSeg], ax;0x15
    mov       word ptr es:[di + VBE_MGA_ProductNameSeg], ax;0x19
    mov       word ptr es:[di + VBE_MGA_RevisionSeg], ax;0x1d
    mov       word ptr es:[di + VBE_MGA_OEMNameSeg], ax;0x21
    mov       word ptr es:[di + VBE_MGA_BuildVersionSeg], ax;0x25
    mov       word ptr es:[di + VBE_MGA_PINSSeg], ax;0x29
    mov       ax, offset VendorName     ;Offset 0x6d98
    mov       word ptr es:[di + VBE_MGA_VendorNameOfs], ax;0x13
    mov       ax, offset ProductName    ;Offset 0x6d9f
    mov       word ptr es:[di + VBE_MGA_ProductNameOfs], ax;0x17
    mov       ax, offset RevisionString ;Offset 0x6da8
    mov       word ptr es:[di + VBE_MGA_RevisionOfs], ax;0x1b
    mov       ax, offset OEMName        ;Offset 0x6d83
    mov       word ptr es:[di + VBE_MGA_OEMNameOfs], ax;0x1f
    mov       ax, offset BuildVersion   ;Offset 0x78
    mov       word ptr es:[di + VBE_MGA_BuildVersionOfs], ax;0x23
    mov       ax, word ptr cs:[PINSPointer];Offset 0x7ffc
    mov       word ptr es:[di + VBE_MGA_PINSOfs], ax;0x27
    call      FindMGAG100               ;Offset 0x53d6
    mov       word ptr es:[di + VBE_MGA_PCIBusDeviceIdFunctionId], bx;0x35
    call      ReadPCIBase0AndBase1      ;Offset 0x53e2
    ;mov       dword ptr es:[di + VBE_MGA_LinearFrameBuffer], eax;0x2d
    DB 026h, 066h, 089h, 045h, 02Dh
    ;mov       dword ptr es:[di + VBE_MGA_LinearControlAperture], ecx;0x31
    DB 026h, 066h, 089h, 04Dh, 031h
    call      GetMemoryIn64KBlocks      ;Offset 0x5448
    mov       word ptr es:[di + VBE_MGA_MemoryIn64KBBlocks], ax;0x2b
    mov       ax, 004fh
    pop       si
    pop       dx
    pop       cx
    pop       ds
    iret

AccessMGAIndexedDataRegister:           ;Offset 0x748f
    mov  bx, ax
    xchg bx, dx
    or   dh, dh
    jne  Label0x749c                    ;Offset 0x749c
    call WriteIndexedRegister           ;Offset 0x5774
    jmp  Label0x749f                    ;Offset 0x749f
Label0x749c:                            ;Offset 0x749c
    call ReadIndexedRegister            ;Offset 0x578f
Label0x749f:                            ;Offset 0x749f
    xchg bx, dx
    mov  ax, 004fh
    iret

AccessMGAIndirectRegister:              ;Offset 0x74a5
    mov  bx, ax
    xchg bx, dx
    and  ah, 10h
    je   Label0x74c6                    ;Offset 0x74c6
    and  dh, 03h
    je   Label0x74c1                    ;Offset 0x74c1
    dec  dl
    je   Label0x74bc                    ;Offset 0x74bc
    call IndirectRegisterReadDWord      ;Offset 0x55b1
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74bc:                            ;Offset 0x74bc
    call IndirectRegisterReadWord       ;Offset 0x5595
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74c1:                            ;Offset 0x74c1
    call IndirectRegisterReadByte       ;Offset 0x5579
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74c6:                            ;Offset 0x74c6
    and  dh, 03h
    je   Label0x74d9                    ;Offset 0x74d9
    dec  dl
    je   Label0x74d4                    ;Offset 0x74d4
    call IndirectRegisterWriteDWord     ;Offset 0x5560
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74d4:                            ;Offset 0x74d4
    call IndirectRegisterWriteWord      ;Offset 0x5542
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74d9:                            ;Offset 0x74d9
    call IndirectRegisterWriteByte      ;Offset 0x5524
    jmp  Label0x74de                    ;Offset 0x74de
Label0x74de:                            ;Offset 0x74de
    mov  ax, 004fh
    iret

Label0x74e2:                            ;Offset 0x74e2
    push si
    mov  bx, ax
    add  si, offset PINS                ;Offset 0x7520
    mov  cl, byte ptr cs:[si]
    mov  ax, 004fh
    pop  si
    iret

Label0x74f1:                            ;Offset 0x74f1
    or   ax, ax
    mov  ah, 01h
    je   Label0x7509                    ;Offset 0x7509
    mov  ax, 000ah
    mul  dx
    shl  edx, 10h
    mov  dx, ax
    mov  cl, 2dh
    call ConfigureAndSelectPLLSetC      ;Offset 0x5e46
    xor  ah, ah
Label0x7509:                            ;Offset 0x7509
    mov  al, 4fh
    iret

Label0x750c:                            ;Offset 0x750c
    mov  bx, ax
    mov  cl, byte ptr cs:[si]
    mov  ax, 004fh
    iret

    xchg bx, bx                         ;hello
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    xchg bx, bx
    nop

;Offset 0x7520
PINS                    DB 02Eh, 041h   ;signature ".A"
                        DB 040h         ;Size. Also used as offset from 0x7520 to the protected mode table: 0x7520 + 0x40 = 0x7560
                        DB 0FFh         ;??
                        DW 00300h       ;Structure version (3.0)
                        DW 0FFFFh       ;Date last programmed
                        DW 00000h       ;Number of times programmed
                        DW 00000h       ;Flags
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h   ;ASCII Serial number, 3 letters + 5 digits
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h   ;8 bytes ???
                        DB 000h, 000h, 000h, 000h   ;4 bytes ASCII Card Version
                        DB 000h, 000h, 0FFh, 0FFh   ;DWORD PCB Revision
;Offset 0x7544
Data0x7544              DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
;Offset 0x7549
PLLADefaultFreq         DB 0FFh
;Offset 0x754a
PLLBDefaultFreq         DB 0FFh
;Offset 0x754b
Data0x754b              DB 0FFh
;Offset 0x754c
Data0x754c              DB 03Eh
;Offset 0x754d
Data0x754d              DB 0FFh, 0FFh, 0FFh
;Offset 0x7550
MemoryControlWaitState  DD MGA_MA_MCWS_RowPrechrgDelay5Cycles OR MGA_MA_MCWS_Rd2PrechrgDelNpCLm2Cyc OR MGA_MA_MCWS_RASMinActive8Cycles OR MGA_MA_MCWS_BW2PrechrgDelay5Cycles OR MGA_MA_MCWS_BlockWriteDelay3Cycles OR MGA_MA_MCWS_RAS2CASDelay4Cycles OR MGA_MA_MCWS_RAS2RASDelay4Cycles OR MGA_MA_MCWS_CASLatency3Cycles;0x03258A31
;Offset 0x7554
Data0x7554              DB 000h
;Offset 0x7555
Data0x7555              DB 0EEh
;Offset 0x7556
Data0x7556              DB 0FFh
;Offset 0x7557
Data0x7557              DB 0FFh
;Offset 0x7558
                        DB 088h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

;Offset 0x7560
                        DW 00014h       ;00h    WORD    offset (within table) of protected-mode code for Function 5 (Set Window) : Points to 0x7574
                        DW 0003Bh       ;02h    WORD    offset of protected-mode code for Function 7 (Set Disp Start) : Points to 0x759b
                        DW 00087h       ;04h    WORD    offset of protected-mode code for Function 9 (Set Primary Palette) : Points to 0x75e7
                        DW 0000Ch       ;06h    WORD    offset (within table) of list of I/O ports and memory locations
                                        ;               for which I/O privilege may be required in protected mode (0000h if no list) : Points to 0x756c
                                        ;08h    var     code and optional port/memory list
                        DW 000F8h       ;Size
                        DW 00000h       ;?

;Offset 0x756c
                        DW 003DEh       ;Port list
                        DW 003DFh
                        DW 0FFFFh
                        DW 0FFFFh       ;Memory list

;Can't have 16-bit tiny code and 32-bit flat code in the same file, so here is the data with code as comment:

                                        ;SetWindow32bit PROC NEAR                ;Offset 0x7574
DB 066h, 052h                           ;    push dx
DB 00Ah, 0FFh                           ;    or   bh, bh
DB 075h, 00Ch                           ;    jne  Label0x7586                    ;Offset 0x7586
DB 08Ah, 0E2h                           ;    mov  ah, dl
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 066h, 0EFh                           ;    out  dx, ax
DB 0EBh, 00Eh                           ;    jmp  Label0x7594                    ;Offset 0x7594
                                        ;Label0x7586:                            ;Offset 0x7586
DB 066h, 0BAh, 0DEh, 003h               ;    mov  dx, MGA_CRTCExtensionIndex     ;Port 0x3de
DB 0B0h, 004h                           ;    mov  al, MGA_CRTCExt_MemoryPage     ;0x4
DB 0EEh                                 ;    out  dx, al
DB 066h, 042h                           ;    inc  dx
DB 0ECh                                 ;    in   al, dx
DB 08Ah, 0D0h                           ;    mov  dl, al
DB 032h, 0F6h                           ;    xor  dh, dh
                                        ;Label0x7594:                            ;Offset 0x7594
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;SetWindow32bit ENDP

                                        ;SetDisplayStart32bit PROC NEAR          ;Offset 0x759b
DB 050h                                 ;    push eax
DB 066h, 052h                           ;    push dx
DB 066h, 08Bh, 0C2h                     ;    mov  ax, dx
DB 0C1h, 0C0h, 010h                     ;    rol  eax, 10h
DB 066h, 08Bh, 0C1h                     ;    mov  ax, cx
DB 0D1h, 0E8h                           ;    shr  eax, 01h
DB 050h                                 ;    push eax
DB 0F6h, 0C3h, 080h                     ;    test bl, 80h
DB 00Fh, 084h, 009h, 000h, 000h, 000h   ;    je   Label0x75bc                    ;Offset 0x75bc
DB 066h, 0BAh, 0DAh, 003h               ;    mov  dx, VGA_InputStatus1D          ;Port 0x3da
                                        ;Label0x75b7:                            ;Offset 0x75b7
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0FBh                           ;    je   Label0x75b7                    ;Offset 0x75b7
                                        ;Label0x75bc:                            ;Offset 0x75bc
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

                                        ;SetPrimaryPalette32bit PROC NEAR        ;Offset 0x75e7
DB 080h, 0FBh, 002h                     ;    cmp  bl, 02h
DB 074h, 067h                           ;    je   Label0x7653                    ;Offset 0x7653
DB 080h, 0FBh, 003h                     ;    cmp  bl, 03h
DB 074h, 062h                           ;    je   Label0x7653                    ;Offset 0x7653
DB 066h, 053h                           ;    push bx
DB 066h, 051h                           ;    push cx
DB 066h, 052h                           ;    push dx
DB 057h                                 ;    push edi
DB 08Ah, 0C2h                           ;    mov  al, dl
DB 066h, 0BAh, 0C8h, 003h               ;    mov  dx, VGA_DACWriteIndex          ;Port 0x3c8
DB 0EEh                                 ;    out  dx, al
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 03Ch, 080h                           ;    cmp  al, 80h
DB 075h, 00Ch                           ;    jne  Label0x7611                    ;Offset 0x7611
DB 0B2h, 0DAh                           ;    mov  dl, VGA_FeatureControlWD_lowbyte;Port 0x3da
                                        ;Label0x7607:                            ;Offset 0x7607
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0FBh                           ;    jne  Label0x7607                    ;Offset 0x7607
                                        ;Label0x760c:                            ;Offset 0x760c
DB 0ECh                                 ;    in   al, dx
DB 024h, 008h                           ;    and  al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 074h, 0FBh                           ;    je   Label0x760c                    ;Offset 0x760c
                                        ;Label0x7611:                            ;Offset 0x7611
DB 0B2h, 0C9h                           ;    mov  dl, VGA_RAMDACData_lowbyte     ;Port 0x3c9
DB 08Ah, 0C3h                           ;    mov  al, bl
DB 024h, 001h                           ;    and  al, 01h
DB 074h, 018h                           ;    je   Label0x7631                    ;Offset 0x7631
                                        ;Label0x7619:                            ;Offset 0x7619
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
DB 075h, 0EAh                           ;    jne  Label0x7619                    ;Offset 0x7619
DB 0EBh, 016h                           ;    jmp  Label0x7647                    ;Offset 0x7647
                                        ;Label0x7631:                            ;Offset 0x7631
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
DB 075h, 0EAh                           ;    jne  Label0x7631                    ;Offset 0x7631
                                        ;Label0x7647:                            ;Offset 0x7647
DB 05Fh                                 ;    pop  edi
DB 066h, 05Ah                           ;    pop  dx
DB 066h, 059h                           ;    pop  cx
DB 066h, 05Bh                           ;    pop  bx
DB 066h, 0B8h, 04Fh, 000h               ;    mov  ax, 004fh
DB 0C3h                                 ;    ret
                                        ;Label0x7653:                            ;Offset 0x7653
DB 066h, 0B8h, 04Fh, 002h               ;    mov  ax, 024fh
DB 0C3h                                 ;    ret
                                        ;SetPrimaryPalette32bit ENDP
DB 000h

;Offset 0x7659
DB 997h dup (0FFh)

;Offset 0x7ff0
MemorySizeInMB          DB 000h

DB 000h, 000h, 000h, 000h, 000h

;Offset 0x7ff6
DW offset Data0x6831

DB 000h, 000h, 000h, 000h

;Offset 0x7ffc
PINSPointer             DW offset PINS  ;Offset 0x7520

DB 0FFh

DB 0BEh
ROMEnd:
END
