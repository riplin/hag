; 3Dfx Voodoo Banshee AGP BIOS v1.00.10-SG disassembly

;Resources:
;
;BIOS files: https://www.3dfxzone.it/dir/3dfx/banshee/bios/roms/
;
;DOS video card tests: https://gona.mactar.hu/DOS_TESTS/
;
;Display Data Channel:https://glenwing.github.io/docs/VESA-EDDC-1.2.pdf
;Display Data Channel:https://glenwing.github.io/docs/VESA-DDCCI-1.1.pdf
;VESA EDID:https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
;
;3Dfx Banshee Specification: https://theretroweb.com/chip/documentation/banshee-spec-662aff3a8597f179587013.pdf
;Additional documentation: https://theretroweb.com/chip/documentation/banshee-2d-spec-662aff3a84890000637393.pdf
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT vbasg10.asm
;Produces a .COM file. Just rename to .BIN
;

include ..\..\include\bda.inc
include ..\..\include\pci.inc
include ..\..\include\vga.inc
include ..\..\include\pit.inc
include ..\..\include\vesa.inc
include ..\..\include\keyboard.inc
include banshee.inc

PCI_VENDOR_3Dfx                         EQU 121ah;0x121a
PCI_DEVICE_Banshee                      EQU 0003h;0x0003

.MODEL tiny
.386
.CODE
ORG 0000h

ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

;Offset 0x3
jmp trampoline                          ;Offset 0x52

                        DB '7400'       ;Offset 0x5    Looks like a string, but who knows.
                        DB 0E9h, 04Ch, 019h, 077h, 0CCh ;Offset 0x9
                        DB 'VIDEO ', 00Dh, 000h, 000h, 000h ;Offset 0xE
                        
;Offset 0x18
                        DW offset PCIHeader;Offset 0x7D20
                        DW 0000h        ;PnP header pointer
                        DW 0000h        ;Unknown
                        DB 'IBM VGA Compatible'

;Offset 0x30
                        DB 001h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB '02/09/99051562LGC'
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x50
                        DW offset ROMConfigTable ;0x76F7

trampoline:                             ;Offset 0x52
jmp entry                               ;Offset 0xa1

                        DB 000h
                        DB 'VGA BIOS - Version 1.00.10-SG Copyright (C) 1990-1998 Elpin Systems, Inc.', 00Dh, 00Ah, 000h

;Offset 0xa1
entry:
    push   bp
    call   GetBIOSDataSelectorInDS      ;Offset 0x3f40
    push   cs
    pop    es
    call   Func0x347c                   ;Offset 0x347c
    jb     InitializationFailure        ;Offset 0xaf
    call   Func0x110                    ;Offset 0x110
InitializationFailure:                  ;Offset 0xaf
    pop    bp
    retf

;Offset 0xb1
Int10JumpTable          DW SetVideoMode;Offset 0x02C2                                       ;  0
                        DW SetTextModeCursorShape;Offset 0x06E0                             ;  1
                        DW SetCursorPosition;Offset 0x075B                                  ;  2
                        DW GetCursorPositionAndSize;Offset 0x079C                           ;  3
                        DW ReadLightPenPosition;Offset 0x1912                               ;  4
                        DW SelectActiveDisplayPage;Offset 0x07AF                            ;  5
                        DW ScrollUpWindow;Offset 0x0806                                     ;  6
                        DW ScrollDownWindow;Offset 0x0C9C                                   ;  7
                        DW ReadCharacterAndAttributeAtCursorPosition;Offset 0x0F5A          ;  8
                        DW WriteCharacterAndAttributeAtCursorPosition;Offset 0x121A         ;  9
                        DW WriteCharacterOnlyAtCursorPosition;Offset 0x12FD                 ;  a
                        DW SetBorderColorOrPalette;Offset 0x1B46                            ;  b
                        DW WriteGraphicsPixel;Offset 0x192B                                 ;  c
                        DW ReadGraphicsPixel;Offset 0x1A4E                                  ;  d
                        DW TeletypeOutput;Offset 0x1377                                     ;  e
                        DW GetCurrentVideoMode;Offset 0x06CB                                ;  f
                        DW RAMDACFunctions;Offset 0x1C00                                    ; 10
                        DW TextFunctions;Offset 0x1FBD                                      ; 11
                        DW AlternateFunctions;Offset 0x2299                                 ; 12
                        DW WriteString  ;Offset 0x23C0                                      ; 13
                        DW LCDFunctions ;Offset 0x3D0A                                      ; 14
                        DW GetPhysicalDisplayParameters;Offset 0x3D0D                       ; 15
                        DW NotImplemented;Offset 0x010C                                     ; 16
                        DW NotImplemented;Offset 0x010C                                     ; 17
                        DW NotImplemented;Offset 0x010C                                     ; 18
                        DW NotImplemented;Offset 0x010C                                     ; 19
                        DW DisplayCombinationCodeFunctions;Offset 0x25DE                    ; 1a
                        DW FunctionalityAndStateInfo;Offset 0x2647                          ; 1b
                        DW SaveRestoreVideoState;Offset 0x27A5                              ; 1c

Int10Handler:
    sti
    cld
    push si
    push ds
    mov  si, ax
    mov  ds, word ptr cs:[BIOSDataSelector];Offset 0x77de
    shr  si, 08h
    shl  si, 01h
    cmp  ah, 4fh
    je   VESAHandler                    ;Offset 0x2a3c
    cmp  ah, 1dh
    jae  NotImplemented                 ;Offset 0x10c
    jmp  word ptr cs:[si + Int10JumpTable];Offset 0xb1
NotImplemented:                         ;Offset 0x10c
    pop  ds
    pop  si
    iret

                        DB 000h

Func0x110 PROC NEAR                     ;Offset 0x110
    cli
    mov    word ptr ds:[INT_10_HandlerOfs], offset Int10Handler;Offset 0x40 Offset 0xeb
    mov    word ptr ds:[INT_10_HandlerSeg], cs;Offset 0x42
    mov    word ptr ds:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x4f3a
    mov    word ptr ds:[INT_1F_HandlerSeg], cs;Offset 0x7e
    mov    word ptr ds:[INT_42_HandlerOfs], SYS_INT_10h_HandlerOfs;Offset 0x108 Offset 0xf065
    mov    word ptr ds:[INT_42_HandlerSeg], SYS_INT_10h_HandlerSeg;Offset 0x10a Segment 0xf000
    mov    word ptr ds:[INT_43_HandlerOfs], offset Font8x8   ;Offset 0x10c Offset 0x4b3a
    mov    word ptr ds:[INT_43_HandlerSeg], cs;Offset 0x10e
    mov    word ptr ds:[BDA_VideoParameterControlBlockPtrOfs], offset VideoOverrideTable;Offset 0x4a8 Offset 0x776b
    mov    word ptr ds:[BDA_VideoParameterControlBlockPtrSeg], cs;Offset 0x4aa
    sti
    call   Func0x3506                   ;Offset 0x3506
    call   Func0x17d                    ;Offset 0x17d
    call   Func0x352a                   ;Offset 0x352a
    call   Func0x24e                    ;Offset 0x24e
    int    10h
    mov    ax, 1301h                    ;Write String, bit 0 = 1 - Update cursor after writing
    sub    bh, bh                       ;Page number 0
    mov    bl, byte ptr cs:[DefaultAttribute];Offset 0x77f4 default attribute
    mov    cx, 62h                      ;Number of characters in string
    sub    dx, dx                       ;Write at 0,0
    mov    bp, cs
    mov    es, bp
    mov    bp, offset BootSplash        ;Offset 0x7708
    int    10h
    call   PrintNameAndMemorySize       ;Offset 0x3542
    mov    cx, word ptr cs:[SplashWaitFrames];Offset 0x77f2
WaitLoop:                               ;Offset 0x174
    call   WaitUntilVSyncInactive       ;Offset 0x406a
    call   WaitUntilVSyncActive         ;Offset 0x4057
    loop   WaitLoop                     ;Offset 0x174
    ret
Func0x110 ENDP

Func0x17d PROC NEAR                     ;Offset 0x17d
    push   dx
    mov    byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Memory256k;Offset 0x487 0x60
    mov    byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_MDAHiResEnhanced_2;Offset 0x488 0x9
    mov    byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA OR BDA_VDDA_LineMode400;Offset 0x489 0x11
    mov    byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], 0bh;Offset 0x48a - 11) Color Video graphics Array (CVGA)
    mov    al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and    al, NOT BDA_DH_InitialVideoModeMask;0xcf
    or     al, BDA_DH_80x25Color        ;0x20
    mov    byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    mov    al, VGA_Misc_IOAddressSel_CGA;0x1
    call   SetIOAddressSelect           ;Offset 0x23b
    mov    dx, VGA_CRTControllerIndexB  ;Port 0x3b4
    call   Func0x1fa                    ;Offset 0x1fa
    jne    Label0x1ca                   ;Offset 0x1ca
    and    byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_VGA;Offset 0x489 0xfe
    mov    byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], 0ch;Offset 0x48a 0xc - 12) CVGA + MDPA
    push   word ptr ds:[BDA_DetectedHardware];Offset 0x410
    or     byte ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Monochrome;Offset 0x410 0x30
    mov    ax, 0007h                    ;Set video mode - 80x25 Text (Monochrome)
    int    10h
    pop    word ptr ds:[BDA_DetectedHardware];Offset 0x410
    mov    ax, 0003h
    jmp    Label0x1f8                   ;Offset 0x1f8
Label0x1ca:                             ;Offset 0x1ca
    sub    al, al                       ;VGA_Misc_IOAddressSel_MDA
    call   SetIOAddressSelect           ;Offset 0x23b
    mov    dx, VGA_CRTControllerIndexD  ;Port 0x3d4
    call   Func0x1fa                    ;Offset 0x1fa
    jne    Label0x1f5                   ;Offset 0x1f5
    mov    al, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and    al, NOT BDA_VDDA_VGA         ;0xfe
    or     al, BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;0x6
    mov    byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    mov    byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome OR BDA_VMO_Memory256k;Offset 0x487 0x62
    mov    ax, 0003h                    ;Set video mode - 80x25 Text (Color / Grayscale)
    int    10h
    or     byte ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Monochrome;Offset 0x410 0x30
    mov    ax, 0007h
    jmp    Label0x1f8                   ;Offset 0x1f8
Label0x1f5:                             ;Offset 0x1f5
    mov    ax, 0003h
Label0x1f8:                             ;Offset 0x1f8
    pop    dx
    ret
Func0x17d ENDP

;Tests cursor location bit width?
Func0x1fa PROC NEAR
    mov    ax, VGA_CRTCIdx_CursorLocLow OR (0aah SHL 8);0xaa0f
    out    dx, ax
    call   ResetAttributeIndex          ;Offset 0x4135
    call   ReadIndexedRegister          ;Offset 0x3f84
    cmp    ah, 0aah
    jne    Label0x23a                   ;Offset 0x23a
    mov    ax, VGA_CRTCIdx_CursorLocHigh OR (2ah SHL 8);0x2a0e
    out    dx, ax
    call   ResetAttributeIndex          ;Offset 0x4135
    call   ReadIndexedRegister          ;Offset 0x3f84
    cmp    ah, 2ah
    jne    Label0x23a                   ;Offset 0x23a
    mov    ax, VGA_CRTCIdx_CursorLocLow OR (55h SHL 8);0x550f
    out    dx, ax
    call   ResetAttributeIndex          ;Offset 0x4135
    call   ReadIndexedRegister          ;Offset 0x3f84
    cmp    ah, 55h
    jne    Label0x23a                   ;Offset 0x23a
    sub    ah, ah
    out    dx, ax
    mov    ax, VGA_CRTCIdx_CursorLocHigh OR (15h SHL 8);0x150e
    out    dx, ax
    call   ResetAttributeIndex          ;Offset 0x4135
    call   ReadIndexedRegister          ;Offset 0x3f84
    cmp    ah, 15h
    mov    ah, 00h
    out    dx, ax
Label0x23a:                             ;Offset 0x23a
    ret
Func0x1fa ENDP

;inputs:
;   al<0>: 1 = CGA, 0 = MDA (or IO address select)
SetIOAddressSelect PROC NEAR            ;Offset 0x23b
    push   ax
    push   dx
    mov    ah, al
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    and    ax, (VGA_Misc_IOAddressSel_CGA SHL 8) OR (0ffh XOR VGA_Misc_IOAddressSel_CGA);0x1fe
    or     al, ah
    mov    dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    out    dx, al
    pop    dx
    pop    ax
    ret
SetIOAddressSelect ENDP

Func0x24e PROC NEAR                     ;Offset 0x24e
    pushaw
    int    10h
    mov    bx, 0808h
    mov    ah, 08h
Label0x256:                             ;Offset 0x256
    call   Func0x1e9d                   ;Offset 0x1e9d
    call   Func0x2af                    ;Offset 0x2af
    je     Label0x267                   ;Offset 0x267
    inc    ah
    cmp    ah, 3fh
    jb     Label0x256                   ;Offset 0x256
    jmp    Label0x2a4                   ;Offset 0x2a4
Label0x267:                             ;Offset 0x267
    push   ax
    mov    ah, 08h
Label0x26a:                             ;Offset 0x26a
    call   Func0x1e9d                   ;Offset 0x1e9d
    call   Func0x2af                    ;Offset 0x2af
    je     Label0x27c                   ;Offset 0x27c
    inc    bl
    cmp    bl, 3fh
    jb     Label0x26a                   ;Offset 0x26a
    pop    ax
    jmp    Label0x2a4                   ;Offset 0x2a4
Label0x27c:                             ;Offset 0x27c
    pop    ax
    mov    al, bl
    cmp    ah, 18h
    jb     Label0x288                   ;Offset 0x288
    cmp    al, 18h
    jae    Label0x29b                   ;Offset 0x29b
Label0x288:                             ;Offset 0x288
    cmp    ah, 18h
    jae    Label0x298                   ;Offset 0x298
    cmp    al, 18h
    jb     Label0x298                   ;Offset 0x298
    or     byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    jmp    Label0x29b                   ;Offset 0x29b
Label0x298:                             ;Offset 0x298
    call   Func0x3541                   ;Offset 0x3541
Label0x29b:                             ;Offset 0x29b
    sub    bx, bx
    sub    ah, ah
    call   Func0x1e9d                   ;Offset 0x1e9d
    popaw
    ret
Label0x2a4:                             ;Offset 0x2a4
    mov    bx, 0001h                    ;One long
    mov    cx, 0003h                    ;Three short
    call   PlayErrorBeeps               ;Offset 0x40ba
    jmp    Label0x29b                   ;Offset 0x29b
Func0x24e ENDP

Func0x2af PROC NEAR                     ;Offset 0x2af
    push   ax
    push   dx
    cli
    call   WaitHBlankVBlankInactive     ;Offset 0x4033
    call   WaitHBlankVBlankActive       ;Offset 0x4045
    mov    dl, VGA_InputStatus0_lowbyte ;Port 0x3c2
    in     al, dx
    sti
    test   al, VGA_IS_SwitchSense       ;0x10
    pop    dx
    pop    ax
    ret
Func0x2af ENDP

                        DB 000h

SetVideoMode:                           ;Offset 0x02C2
    pushaw
    push      es
    call      SetupBDA                  ;Offset 0x2fc
    jb        Label0x2f7                ;Offset 0x2f7
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x35a7                ;Offset 0x35a7
    call      ApplyModeSettingsToBDA    ;Offset 0x3d6
    call      DisableAttributeControllerPaletteOutput;Offset 0x407d
    call      Func0x3615                ;Offset 0x3615
    call      ApplyModeSettingsToVGARegisters;Offset 0x431
    call      TurnScreenOff             ;Offset 0x3f6a
    call      Func0x48a                 ;Offset 0x48a
    call      Func0x363c                ;Offset 0x363c
    call      Func0x5b8                 ;Offset 0x5b8
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_DontClearDisplay;Offset 0x487 0x80
    jne       Label0x2f1                ;Offset 0x2f1
    call      ClearScreen               ;Offset 0x3740
Label0x2f1:                             ;Offset 0x2f1
    call      TurnScreenOn              ;Offset 0x3f77
    call      EnableAttributeControllerPaletteOutput;Offset 0x408d
Label0x2f7:                             ;Offset 0x2f7
    pop       es
    popaw
    pop       ds
    pop       si
    iret

SetupBDA PROC NEAR                      ;Offset 0x2fc
    mov       ah, al
    and       ax, (BDA_DM_DONT_CLEAR_SCREEN SHL 8) OR (0ffh XOR BDA_DM_DONT_CLEAR_SCREEN);0x807f
    call      Func0x3f8d                ;Offset 0x3f8d
    jne       Label0x3cc                ;Offset 0x3cc
    mov       byte ptr ds:[BDA_DisplayMode], al ;Offset 0x449
    call      Func0x4152                ;Offset 0x4152
    je        Label0x3cc                ;Offset 0x3cc
    mov       cl, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       cl, NOT (BDA_VMO_Inactive OR BDA_VMO_DontClearDisplay);0x77
    or        cl, ah
    mov       byte ptr ds:[BDA_VideoModeOptions], cl;Offset 0x487
    mov       bl, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    mov       bh, bl
    and       bx, ((NOT BDA_DH_InitialVideoModeMask) SHL 8) OR BDA_DH_InitialVideoModeMask;0xcf30
    mov       dx, VGA_CRTControllerIndexB;Port 0x3b4
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_VGA;Offset 0x489 0x1
    je        Label0x36a                ;Offset 0x36a
    or        bh, BDA_DH_80x25Monochrome;0x30
    mov       bl, BDA_EFBS_CGAMono80x25_2;0xb
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487 0x2
    call      Func0x3f99                ;Offset 0x3f99
    je        Label0x357                ;Offset 0x357
    and       byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_Monochrome;Offset 0x487 0xfd
    mov       dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    and       bh, NOT BDA_DH_40x25Color ;0xef
    mov       bl, BDA_EFBS_MDAHiRes80x25_2;0x8
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x489 0x80
    jne       Label0x357                ;Offset 0x357
    mov       bl, BDA_EFBS_MDAHiResEnhanced_2;0x9
Label0x357:                             ;Offset 0x357
    mov       word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0x463
    mov       byte ptr ds:[BDA_DetectedHardware], bh;Offset 0x410
    and       byte ptr ds:[BDA_EGAFeatureBitSwitches], NOT BDA_EFBS_AdapterTypeMask;Offset 0x488 0xf0
    or        byte ptr ds:[BDA_EGAFeatureBitSwitches], bl;Offset 0x488
    jmp       Label0x3b1                ;Offset 0x3b1
Label0x36a:                             ;Offset 0x36a
    call      Func0x3f99                ;Offset 0x3f99
    je        Label0x391                ;Offset 0x391
    cmp       bl, 30h
    je        Label0x381                ;Offset 0x381
    mov       dl, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    test      cl, 02h
    jne       Label0x3b3                ;Offset 0x3b3
    mov       word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0x463
    jmp       Label0x3b1                ;Offset 0x3b1
Label0x381:                             ;Offset 0x381
    test      cl, 02h
    je        Label0x3b3                ;Offset 0x3b3
    mov       al, BDA_DM_80x25_Monochrome_Text;0x7
    mov       byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov       word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0x463
    jmp       Label0x3b1                ;Offset 0x3b1
Label0x391:                             ;Offset 0x391
    cmp       bl, 30h
    jne       Label0x3a1                ;Offset 0x3a1
    test      cl, 02h
    je        Label0x3b3                ;Offset 0x3b3
    mov       word ptr ds:[BDA_VideoBaseIOPort], dx;Offset 0x463
    jmp       Label0x3b1                ;Offset 0x3b1
Label0x3a1:                             ;Offset 0x3a1
    test      cl, 02h
    jne       Label0x3b3                ;Offset 0x3b3
    sub       al, al                    ;BDA_DM_40x25_BW_Text
    mov       byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    mov       word ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexD;Offset 0x463 Port 0x3d4
Label0x3b1:                             ;Offset 0x3b1
    clc
    ret
Label0x3b3:                             ;Offset 0x3b3
    cbw
    mov       bl, 0eh
    cmp       al, 07h
    je        Label0x3bc                ;Offset 0x3bc
    mov       bl, 08h
Label0x3bc:                             ;Offset 0x3bc
    mov       byte ptr ds:[BDA_RowsOnScreen], 18h;Offset 0x484 0x18
    mov       byte ptr ds:[BDA_PointHeightOfCharacterMatrix], bl;Offset 0x485
    or        byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    int       42h
Label0x3cc:                             ;Offset 0x3cc
    stc
    ret
SetupBDA ENDP

CRTControlRegValues     DB 02Ch
                        DB 028h
                        DB 02Dh
                        DB 029h
                        DB 02Ah
                        DB 02Eh
                        DB 01Eh
                        DB 029h

ApplyModeSettingsToBDA PROC NEAR        ;Offset 0x3d6
    push      di
    push      es
    mov       cx, 0009h
    mov       di, BDA_VideoBufferOffset ;Offset 0x44e
    call      GetBIOSDataSelectorInES   ;Offset 0x3f46
    sub       ax, ax
    rep stosw
    pop       es
    pop       di
    mov       byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    mov       bl, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       bh, ah
    cmp       bl, BDA_DM_80x25_Monochrome_Text;0x7
    ja        Label0x40c                ;Offset 0x40c
    mov       al, byte ptr cs:[bx + CRTControlRegValues];Offset 0x3ce
    mov       byte ptr ds:[BDA_CRTModeControlRegValue], al;Offset 0x465
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], 3fh;Offset 0x466 0x3f
    cmp       bl, BDA_DM_640x200_BW_Graphics;0x6
    je        Label0x40c                ;Offset 0x40c
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], 30h;Offset 0x466 0x30
Label0x40c:                             ;Offset 0x40c
    mov       al, byte ptr es:[di]
    mov       word ptr ds:[BDA_NumberOfScreenColumns], ax;Offset 0x44a
    mov       al, byte ptr es:[di + 02h]
    mov       word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    mov       al, byte ptr es:[di + 01h]
    mov       byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    mov       ax, word ptr es:[di + 03h]
    mov       word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    mov       ax, word ptr es:[di + 14h]
    xchg      al, ah
    mov       word ptr ds:[BDA_CursorEndStartScanLine], ax;Offset 0x460
    ret
ApplyModeSettingsToBDA ENDP

ApplyModeSettingsToVGARegisters PROC NEAR;Offset 0x431
    push      di
    push      ds
    push      es
    mov       si, di
    push      es
    pop       ds
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, VGA_SEQIdx_Reset OR (VGA_SEQ0_AsyncReset SHL 8);0x100
    out       dx, ax
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    mov       al, byte ptr [si + 09h]
    out       dx, al
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    push      si
    mov       cx, 0004h
    mov       bl, 01h
    lea       si, [si + 05h]
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    pop       si
    mov       ax, VGA_SEQIdx_Reset OR ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8);0x300
    out       dx, ax
    push      ds
    call      GetBIOSDataSelectorInDS   ;Offset 0x3f40
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    pop       ds
    mov       ax, VGA_CRTCIdx_VertRetraceEnd OR (20h SHL 8);0x2011 Unlock CRTC registers 0-7
    out       dx, ax
    sub       bl, bl
    mov       cx, 0019h
    push      si
    lea       si, [si + 0ah]
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    pop       si
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    sub       bl, bl
    mov       cx, 0009h
    push      si
    lea       si, [si + 37h]
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    pop       si
    mov       dl, VGA_FeatureControlWD_lowbyte;Offset 0x3da
    sub       al, al
    out       dx, al
    pop       es
    pop       ds
    pop       di
    ret
ApplyModeSettingsToVGARegisters ENDP

Func0x48a PROC NEAR                     ;Offset 0x48a
    push      di
    push      es
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    push      dx
    lea       si, [di + 23h]
    call      Func0x687                 ;Offset 0x687
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_ColorSelect  ;0x14
    out       dx, al
    sub       al, al
    out       dx, al
    sti
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0x8
    jne       Label0x4af                ;Offset 0x4af
    call      Func0x509                 ;Offset 0x509
Label0x4af:                             ;Offset 0x4af
    pop       dx
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       di, es:[di + BDA_VPCB_SecondarySavePointerTbl];Offset 0x10
    mov       ax, es
    or        ax, di
    je        Label0x4f2                ;Offset 0x4f2
    les       di, es:[di + BDA_VPCB_PaletterProfileTable];Offset 0xa
    mov       ax, es
    or        ax, di
    je        Label0x4f2                ;Offset 0x4f2
    lea       si, [di + BDA_PPT_ApplicableModeList];Offset 0x14
    call      CheckModeList             ;Offset 0x6bb
    jne       Label0x4f2                ;Offset 0x4f2
    push      ds
    mov       cx, word ptr es:[di + BDA_PPT_AttributeRegCount];Offset 0x4
    mov       ah, byte ptr es:[di + BDA_PPT_AttributeRegStartIndex];Offset 0x6
    lds       si, es:[di + BDA_PPT_AttributeRegsPtr];Offset 0x8
    call      SetAttributePalette       ;Offset 0x1e7f
    mov       ax, 1012h                 ;Dac functions - Set block of dac registers
    mov       cx, word ptr es:[di + BDA_PPT_DACRegCount];Offset 0xc
    mov       bx, word ptr es:[di + BDA_PPT_DACRegStartIndex];Offset 0xe
    les       dx, es:[di + BDA_PPT_DacRegsPtr];Offset 0x10
    int       10h
    pop       ds
Label0x4f2:                             ;Offset 0x4f2
    pop       es
    pop       di
    ret
Func0x48a ENDP

;Offset 0x4f5
PaletteSetCallTable     DW offset Func0x53d;0x53d
                        DW offset Func0x58b;0x58b
                        DW offset Func0x578;0x578
                        DW offset Func0x58b;0x58b
                        DW offset Func0x544;0x544
                        DW offset Func0x544;0x544
                        DW offset Func0x5ad;0x5ad
                        DW offset Func0x3d04;0x3d04
                        DW offset Func0x3d07;0x3d07
                        DW offset Func0x6ca;0x6ca

Func0x509 PROC NEAR                     ;Offset 0x509
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, 0ffh
    out       dx, al
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       bx, 04h
    cmp       al, BDA_DM_320x200_16_Color_Graphics;0xd
    je        Label0x52f                ;Offset 0x52f
    cmp       al, BDA_DM_640x200_16_Color_Graphics;0xe
    je        Label0x52f                ;Offset 0x52f
    call      FindModeData              ;Offset 0x40d9
    mov       bl, byte ptr es:[bx + 04h]
    sub       bh, bh
    cmp       bl, 0ah
    jb        Label0x52d                ;Offset 0x52d
    mov       bl, 09h
Label0x52d:                             ;Offset 0x52d
    shl       bx, 01h
Label0x52f:                             ;Offset 0x52f
    push      es
    push      cs
    pop       es
    call      word ptr cs:[bx + PaletteSetCallTable];Offset 0x4f5
    pop       es
    ret
Func0x509 ENDP

;Offset 0x539
Data0x539               DB 000h, 015h, 02Ah, 03Fh

Func0x53d PROC NEAR                     ;Offset 0x53d
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x489 0x80
    jne       Func0x578                 ;Offset 0x578
Func0x53d ENDP

Func0x544 PROC NEAR                     ;Offset 0x544
    mov       si, offset Data0x73ab     ;Offset 0x73ab
    mov       di, 0040h
    sub       al, al
Func0x544 ENDP
;continue!
Func0x54c PROC NEAR                     ;Offset 0x54c
    mov       bx, offset Data0x539      ;Offset 0x539
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
Label0x553:                             ;Offset 0x553
    mov       ah, byte ptr es:[si]
    inc       si
    mov       al, ah
    and       al, 03h
    xlatb     cs:[bx]
    mov       dh, al
    mov       al, ah
    and       al, 0ch
    shr       al, 02h
    xlatb     cs:[bx]
    mov       ch, al
    shr       ax, 0ch
    xlatb     cs:[bx]
    mov       cl, al
    call      Func0x1d41                ;Offset 0x1d41
    dec       di
    jne       Label0x553                ;Offset 0x553
    ret
Func0x54c ENDP

Func0x578 PROC NEAR                     ;Offset 0x578
    mov       si, offset Data0x73eb     ;Offset 0x73eb
    mov       di, 0020h
    sub       al, al
    push      si
    push      di
    call      Func0x54c                 ;Offset 0x54c
    pop       di
    pop       si
    mov       al, 20h
    jmp       Func0x54c                 ;Offset 0x54c
Func0x578 ENDP

Func0x58b PROC NEAR                     ;Offset 0x58b
    mov       si, offset Data0x740b     ;Offset 0x740b
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    sub       al, al
    out       dx, al
    inc       dx
    mov       cx, 0002h
Label0x598:                             ;Offset 0x598
    push      cx
    push      si
    mov       cx, 0004h
Label0x59d:                             ;Offset 0x59d
    lodsb     byte ptr es:[si]
    mov       bx, 0018h
Label0x5a2:                             ;Offset 0x5a2
    out       dx, al
    dec       bx
    jne       Label0x5a2                ;Offset 0x5a2
    loop      Label0x59d                ;Offset 0x59d
    pop       si
    pop       cx
    loop      Label0x598                ;Offset 0x598
    ret
Func0x58b ENDP

Func0x5ad PROC NEAR                     ;Offset 0x5ad
    mov       si, offset Data0x740f     ;Offset 0x740f
    mov       cx, 0f8h
    sub       bl, bl
    jmp       Func0x1d6d                ;Offset 0x1d6d
Func0x5ad ENDP

Func0x5b8 PROC NEAR                     ;Offset 0x5b8
    push      bp
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x3faf                ;Offset 0x3faf
    jne       Label0x650                ;Offset 0x650
    call      Func0x1ee5                ;Offset 0x1ee5
    les       di, ds:[BDA_VideoParameterControlBlockPtrOfs];Offset 0x4a8
    les       di, es:[di + BDA_VPCB_AlphaNumCharSetOverride];Offset 0x8
    mov       dx, es
    or        dx, di
    je        Label0x5f6                ;Offset 0x5f6
    lea       si, [di + BDA_ACSO_ApplicableModeList];Offset 0xb
    call      CheckModeList             ;Offset 0x6bb
    jne       Label0x5f6                ;Offset 0x5f6
    mov       ax, 1110h                 ;Text functions - load user specified patterns
    mov       bh, byte ptr es:[di + BDA_ACSO_ChardDefLength];Offset 0x0
    mov       bl, byte ptr es:[di + BDA_ACSO_RamBankIndex];Offset 0x1
    mov       cx, word ptr es:[di + BDA_ACSO_NumCharacters];Offset 0x2
    mov       dx, word ptr es:[di + BDA_ACSO_StartIndex];Offset 0x4
    les       bp, es:[di + BDA_ACSO_FontData];Offset 0x6
    int       10h
    jmp       Label0x613                ;Offset 0x613
Label0x5f6:                             ;Offset 0x5f6
    call      Func0x1ec8                ;Offset 0x1ec8
    sub       dx, dx
    mov       bl, dl
    mov       cx, 0100h
    call      Func0x1f3d                ;Offset 0x1f3d
    call      Func0x37b5                ;Offset 0x37b5
    call      Func0x3fc5                ;Offset 0x3fc5
    jne       Label0x613                ;Offset 0x613
    call      Func0x1f29                ;Offset 0x1f29
    sub       bl, bl
    call      Func0x1f63                ;Offset 0x1f63
Label0x613:                             ;Offset 0x613
    call      Func0x1f05                ;Offset 0x1f05
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       di, es:[di + BDA_VPCB_SecondarySavePointerTbl];Offset 0x10
    les       di, es:[di + BDA_SSP_SecondaryAlphaOverr];Offset 0x6
    mov       dx, es
    or        dx, di
    je        Label0x685                ;Offset 0x685
    mov       al, byte ptr es:[di + BDA_SACSO_CharDefLength];Offset 0x0
    cmp       byte ptr ds:[BDA_PointHeightOfCharacterMatrix], al;Offset 0x485
    jne       Label0x685                ;Offset 0x685
    lea       si, [di + BDA_SACSO_ApplicableModeLst];Offset 0x7
    call      CheckModeList             ;Offset 0x6bb
    jne       Label0x685                ;Offset 0x685
    mov       ax, 1100h                 ;Text functions - load user specified patterns
    mov       bl, byte ptr es:[di + BDA_SACSO_RamBankIndex];Offset 0x1
    mov       bh, byte ptr es:[di + BDA_SACSO_CharDefLength]; Offset 0x0
    mov       cx, 0100h
    sub       dx, dx
    les       bp, es:[di + BDA_SACSO_FontData];Offset 0x3
    int       10h
    jmp       Label0x685                ;Offset 0x685
Label0x650:                             ;Offset 0x650
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       di, es:[di + BDA_VPCB_GrahicsCharSetOverride];Offset 0xc
    mov       dx, es
    or        dx, di
    je        Label0x67a                ;Offset 0x67a
    lea       si, [di + BDA_GCSO_ApplicableModeList];Offset 0x7
    call      CheckModeList             ;Offset 0x6bb
    jne       Label0x67a                ;Offset 0x67a
    mov       ax, 1121h                 ;Text functions - set user specified graphics characters
    sub       bl, bl
    mov       cx, word ptr es:[di + BDA_GCSO_CharDefLength];Offset 0x1
    mov       dl, byte ptr es:[di + BDA_GCSO_NumCharacters];Offset 0x0
    les       bp, es:[di + BDA_GCSO_FontData];Offset 0x3
    int       10h
    jmp       Label0x685                ;Offset 0x685
Label0x67a:                             ;Offset 0x67a
    call      Func0x1ec8                ;Offset 0x1ec8
    mov       word ptr ds:[INT_43_HandlerOfs], di;Offset 0x10c
    mov       word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
Label0x685:                             ;Offset 0x685
    pop       bp
    ret       
Func0x5b8 ENDP

Func0x687 PROC NEAR                     ;Offset 0x687
    push      dx
    push      di
    push      ds
    push      es
    push      es
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       di, es:[di + BDA_VPCB_DynamicParamSaveArea];Offset 0x4
    pop       ds
    mov       ax, es
    or        ax, di
    je        Label0x6a4                ;Offset 0x6a4
    mov       cx, 0010h
    push      si
    rep movsb
    inc       si
    movsb
    pop       si
Label0x6a4:                             ;Offset 0x6a4
    sub       bl, bl
    mov       cx, 0014h
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
Label0x6ad:                             ;Offset 0x6ad
    mov       al, bl
    out       dx, al
    lodsb
    out       dx, al
    inc       bx
    loop      Label0x6ad                ;Offset 0x6ad
    sti
    pop       es
    pop       ds
    pop       di
    pop       dx
    ret
Func0x687 ENDP

CheckModeList PROC NEAR                 ;Offset 0x6bb
    mov       ah, byte ptr ds:[BDA_DisplayMode]    ;Offset 0x449
Label0x6bf:                             ;Offset 0x6bf
    lodsb     byte ptr es:[si]
    cmp       ah, al
    je        Func0x6ca                 ;Offset 0x6ca
    cmp       al, 0ffh
    jne       Label0x6bf                ;Offset 0x6bf
    dec       ax
CheckModeList ENDP
;continue!
Func0x6ca PROC NEAR                     ;Offset 0x6ca
    ret
Func0x6ca ENDP

GetCurrentVideoMode:                    ;Offset 0x06CB
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, BDA_VMO_DontClearDisplay;0x80
    or        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    mov       ah, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    pop       ds
    pop       si
    iret

                        DB 000h

SetTextModeCursorShape:                 ;Offset 0x06E0
    push   cx
    push   dx
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   Func0x3faf                   ;Offset 0x3faf
    jne    Label0x70f                   ;Offset 0x70f
    mov    word ptr ds:[BDA_CursorEndStartScanLine], cx;Offset 0x460
    test   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive OR BDA_VMO_CursorEmulationEnabled;Offset 0x487 9
    jne    Label0x702                   ;Offset 0x702
    test   ch, VGA_CS_CursorOff         ;0x20
    jne    Label0x6ff                   ;Offset 0x6ff
    call   Func0x714                    ;Offset 0x714
    jmp    Label0x702                   ;Offset 0x702
Label0x6ff:                             ;Offset 0x6ff
    mov    cx, 1e00h                    ;Cursor start / end values
Label0x702:                             ;Offset 0x702
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov    al, VGA_CRTCIdx_CursorStart  ;0xa
    mov    ah, ch
    out    dx, ax
    inc    ax
    mov    ah, cl
    out    dx, ax
Label0x70f:                             ;Offset 0x70f
    pop    dx
    pop    cx
    pop    ds
    pop    si
    iret

Func0x714 PROC NEAR                     ;Offset 0x714
    mov    dl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec    dx
    cmp    cl, ch
    jge    Label0x728                   ;Offset 0x728
    test   cl, cl
    mov    cl, dl
    jne    Label0x75a                   ;Offset 0x75a
    mov    cx, 2000h
    jmp    Label0x75a                   ;Offset 0x75a
Label0x728:                             ;Offset 0x728
    cmp    cl, 03h
    jbe    Label0x75a                   ;Offset 0x75a
    mov    ah, ch
    inc    ah
    inc    ah
    cmp    ah, cl
    jb     Label0x74b                   ;Offset 0x74b
    dec    dx
    sub    cl, ch
    mov    ch, dl
    sub    dl, cl
    mov    cl, dl
    cmp    cl, ch
    jge    Label0x75a                   ;Offset 0x75a
    mov    cl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    dec    cx
    jmp    Label0x75a                   ;Offset 0x75a
Label0x74b:                             ;Offset 0x74b
    cmp    ch, 02h
    jg     Label0x754                   ;Offset 0x754
    mov    cl, dl
    jmp    Label0x75a                   ;Offset 0x75a
Label0x754:                             ;Offset 0x754
    mov    cl, dl
    mov    ch, dl
    shr    ch, 01h
Label0x75a:                             ;Offset 0x75a
    ret
Func0x714 ENDP

SetCursorPosition:                      ;Offset 0x75b
    pushaw
    call   Func0x763                    ;Offset 0x763
    popaw
    pop    ds
    pop    si
    iret

Func0x763 PROC NEAR                     ;Offset 0x763
    push   bx
    push   dx
    sub    ax, ax
    mov    bl, bh
    sub    bh, bh
    shl    bx, 01h
    mov    word ptr ds:[bx + DBA_CursorPositionPage0], dx;Offset 0x450
    shr    bx, 01h
    cmp    bl, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    jne    Label0x799                   ;Offset 0x799
    mov    bx, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    shr    bx, 01h
    mov    al, dh
    mul    byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    add    al, dl
    adc    ah, 00h
    add    bx, ax
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov    al, VGA_CRTCIdx_CursorLocHigh;0xe
    mov    ah, bh
    out    dx, ax
    inc    ax
    mov    ah, bl
    out    dx, ax
Label0x799:                             ;Offset 0x799
    pop    dx
    pop    bx
    ret
Func0x763 ENDP

GetCursorPositionAndSize:               ;Offset 0x079C
    push   bx
    mov    bl, bh
    sub    bh, bh
    shl    bx, 01h
    mov    dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    mov    cx, word ptr ds:[BDA_CursorEndStartScanLine];Offset 0x460
    pop    bx
    pop    ds
    pop    si
    iret

SelectActiveDisplayPage:                ;Offset 0x07AF
    pushaw 
    mov    byte ptr ds:[BDA_ActiveDisplayNumber], al;Offset 0x462
    sub    bx, bx
    cbw
    push   ax
    test   al, al
    je     Label0x7c2                   ;Offset 0x7c2
Label0x7bb:                             ;Offset 0x7bb
    add    bx, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec    ax
    jne    Label0x7bb                   ;Offset 0x7bb
Label0x7c2:                             ;Offset 0x7c2
    mov    word ptr ds:[BDA_VideoBufferOffset], bx;Offset 0x44e
    push   ax
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   Func0x3faf                   ;Offset 0x3faf
    pop    ax
    jne    Label0x7d2                   ;Offset 0x7d2
    shr    bx, 01h
Label0x7d2:                             ;Offset 0x7d2
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov    al, VGA_CRTCIdx_StartAddrHigh;0xc
    mov    ah, bh
    out    dx, ax
    inc    ax
    mov    ah, bl
    out    dx, ax
    pop    bx
    shl    bx, 01h
    mov    dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    shr    bx, 01h
    mov    bh, bl
    call   Func0x763                    ;Offset 0x763
    popaw
    pop    ds
    pop    si
    iret

                        DB 000h

;Offset 0x7f2
Data0x7f2               DW offset Func0x862;0x862
                        DW offset Func0x85d;0x85d
                        DW offset Func0x930;0x930
                        DW offset Func0xa35;0xa35
                        DW offset Func0xa35;0xa35
                        DW offset Func0xa35;0xa35
                        DW offset Func0xb5a;0xb5a
                        DW offset Func0xb5a;0xb5a
                        DW offset Func0xb5a;0xb5a
                        DW offset Func0x85c;0x85c

ScrollUpWindow:                         ;Offset 0x0806
    pushaw
    push      es
    push      ax
    push      bx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    movzx     si, byte ptr es:[bx + 04h]
    pop       bx
    pop       ax
    jb        Label0x820                ;Offset 0x820
    shl       si, 01h
    call      word ptr cs:[si + Data0x7f2];Offset 0x7f2
Label0x820:                             ;Offset 0x820
    pop       es
    popaw
    pop       ds
    pop       si
    iret

Func0x825 PROC NEAR                     ;Offset 0x825
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ch
    add       al, cl
    adc       ah, 00h
    shl       ax, 01h
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       di, ax
    sub       dl, cl
    sub       dh, ch
    mov       cl, dl
    sub       ch, ch
    inc       cx
    mov       dl, dh
    sub       dh, dh
    inc       dx
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       bp, 01h
    mov       al, 20h
    mov       ah, bh
    mov       bx, cx
Label0x851:                             ;Offset 0x851
    mov       cx, bx
    push      di
    rep stosw
    pop       di
    add       di, bp
    dec       dx
    jne       Label0x851                ;Offset 0x851
Func0x825 ENDP
;continue!
Func0x85c PROC NEAR                    ;Offset 0x85c
    ret
Func0x85c ENDP

Func0x85d PROC NEAR                     ;Offset 0x85d
    call      GetB000SelectorInES       ;Offset 0x3f52
    jmp       Func0x865                 ;Offset 0x865
Func0x85d ENDP

Func0x862 PROC NEAR                     ;Offset 0x0862
    call      GetB800SelectorInES       ;Offset 0x3f58
Func0x862 ENDP
;continue!
Func0x865 PROC NEAR                     ;Offset 865
    test      al, al
    je        Func0x825                 ;Offset 0x825
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0x825                 ;Offset  0x825
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       al, 20h
    mov       ah, bh
    push      ax
    mov       ax, bp
    mul       ch
    add       al, cl
    adc       ah, 00h
    shl       ax, 01h
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       di, ax
    shl       bp, 01h
    sub       bh, bh
    push      bx
    mov       si, di
Label0x894:                             ;Offset 0x894
    add       si, bp
    dec       bx
    jne       Label0x894                ;Offset 0x894
    pop       bx
    push      es
    pop       ds
    sub       dl, cl
    sub       dh, ch
    mov       cl, dl
    sub       ch, ch
    inc       cx
    mov       dl, dh
    sub       dh, dh
    inc       dx
    sub       dx, bx
    pop       ax
Func0x865 ENDP
;continue!
Func0x8ad PROC NEAR                     ;Offset 0x8ad
Label0x8ad:                             ;Offset 0x8ad
    push      cx
    push      di
    push      si
    rep movsw
    pop       si
    pop       di
    pop       cx
    add       si, bp
    add       di, bp
    dec       dx
    jne       Label0x8ad                ;Offset 0x8ad
    mov       dx, cx
Label0x8be:                             ;Offset 0x8be
    mov       cx, dx
    push      di
    rep stosw
    pop       di
    add       di, bp
    dec       bx
    jne       Label0x8be                ;Offset 0x8be
    ret
Func0x8ad ENDP

Func0x8ca PROC NEAR                     ;Offset 0x8ca
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       ch
    push      dx
    mul       word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    shr       ax, 01h
    pop       dx
    add       al, cl
    adc       ah, 00h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x8eb                ;Offset 0x8eb
    shl       ax, 01h
    shl       bp, 01h
Label0x8eb:                             ;Offset 0x8eb
    mov       di, ax
    sub       dl, cl
    sub       dh, ch
    mov       cl, dl
    sub       ch, ch
    inc       cx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0x8ff                ;Offset 0x8ff
    shl       cx, 01h
Label0x8ff:                             ;Offset 0x8ff
    mov       dl, dh
    sub       dh, dh
    inc       dx
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       dl
    mov       dx, ax
    shr       dx, 01h
    mov       al, bh
    push      dx
    push      di
    mov       bx, cx
Label0x913:                             ;Offset 0x913
    mov       cx, bx
    push      di
    rep stosb
    pop       di
    add       di, bp
    dec       dx
    jne       Label0x913                ;Offset 0x913
    pop       di
    pop       dx
    add       di, 2000h
Label0x924:                             ;Offset 0x924
    mov       cx, bx
    push      di
    rep stosb
    pop       di
    add       di, bp
    dec       dx
    jne       Label0x924                ;Offset 0x924
    ret
Func0x8ca ENDP

Func0x930 PROC NEAR                     ;Offset 0x930
    call      GetB800SelectorInES       ;Offset 0x3f58
    test      al, al
    je        Func0x8ca                 ;Offset 0x8ca
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    jae       Func0x8ca                 ;Offset 0x8ca
    push      bx
    sub       dh, ch
    sub       dl, cl
    mov       al, ch
    mov       bh, cl
    mov       cx, dx
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    shr       si, 01h
    sub       ah, ah
    mul       bp
    mul       si
    add       al, bh
    adc       ah, 00h
    mov       di, ax
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    sub       ch, ch
    inc       cx
    cmp       byte ptr ds:[BDA_DisplayMode], 6;Offset 0x449 0x6
    je        Label0x97e                ;Offset 0x97e
    shl       di, 01h
    shl       cx, 01h
    shl       bp, 01h
Label0x97e:                             ;Offset 0x97e
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       bp
    mov       si, di
    add       si, ax
    pop       dx
    pop       ax
    mov       al, ah
    push      es
    pop       ds
Func0x930 ENDP

Func0x993 PROC NEAR                     ;Offset 0x993
    push      dx
    push      di
    push      si
Label0x996:                             ;Offset 0x996
    push      cx
    push      di
    push      si
    rep movsb
    pop       si
    pop       di
    pop       cx
    add       di, bp
    add       si, bp
    dec       dx
    jne       Label0x996                ;Offset 0x996
    pop       si
    pop       di
    pop       dx
    add       di, 2000h
    add       si, 2000h
Label0x9b0:                             ;Offset 0x9b0
    push      cx
    push      di
    push      si
    rep movsb
    pop       si
    pop       di
    pop       cx
    add       di, bp
    add       si, bp
    dec       dx
    jne       Label0x9b0                ;Offset 0x9b0
    push      bx
    push      di
Label0x9c1:                             ;Offset 0x9c1
    push      cx
    push      di
    rep stosb
    pop       di
    pop       cx
    add       di, bp
    dec       bx
    jne       Label0x9c1                ;Offset 0x9c1
    pop       di
    pop       bx
    sub       di, 2000h
Label0x9d2:                             ;Offset 0x9d2
    push      cx
    push      di
    rep stosb
    pop       di
    pop       cx
    add       di, bp
    dec       bx
    jne       Label0x9d2                ;Offset 0x9d2
    ret
Func0x993 ENDP

Func0x9de PROC NEAR                     ;Offset 0x9de
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    sub       dh, ch
    sub       dl, cl
    mov       al, ch
    push      dx
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       bp
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    add       al, cl
    adc       ah, 00h
    mov       di, ax
    pop       cx
    mov       al, ch
    sub       ch, ch
    inc       cx
    inc       al
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    push      ax
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    or        ah, VGA_GCTL5_SplatBLU    ;Offset 0x2
    out       dx, ax
    pop       ax
    mov       dx, ax
    mov       al, bh
    mov       bx, cx
    sub       bp, cx
Label0xa1f:                             ;Offset 0xa1f
    mov       cx, bx
    rep stosb
    add       di, bp
    dec       dx
    jne       Label0xa1f                ;Offset 0xa1f
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    out       dx, ax
    ret
Func0x9de ENDP

Func0xa35 PROC NEAR                     ;Offset 0xa35
    call      GetA000SelectorInES       ;Offset 0x3f4c
    test      al, al
    je        Func0x9de                 ;Offset 0x9de
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0x9de                 ;Offset 0x9de
    push      bx
    sub       dh, ch
    sub       dl, cl
    mov       al, ch
    mov       bh, cl
    mov       cx, dx
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       ah, ah
    mul       bp
    mul       si
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    add       al, bh
    adc       ah, 00h
    mov       di, ax
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    sub       ch, ch
    inc       cx
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       bp
    mov       si, di
    add       si, ax
    pop       dx
    pop       ax
    mov       al, ah
    push      es
    pop       ds
    push      ax
    push      dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    or        ah, VGA_GCTL5_CPUDirect   ;0x1
    out       dx, ax
    pop       dx
    sub       bp, cx
Label0xaa1:                             ;Offset 0xaa1
    push      cx
    rep movsb
    pop       cx
    add       di, bp
    add       si, bp
    dec       dx
    jne       Label0xaa1                ;Offset 0xaa1
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    or        ah, VGA_GCTL5_SplatBLU    ;0x2
    out       dx, ax
    pop       ax
Label0xabc:                             ;Offset 0xabc
    push      cx
    rep stosb
    pop       cx
    add       di, bp
    dec       bx
    jne       Label0xabc                ;Offset 0xabc
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    out       dx, ax
    ret
Func0xa35 ENDP

Func0xad2 PROC NEAR                     ;Offset 0xad2
    inc       byte ptr [bp + 07h]
    mov       dl, byte ptr [bp + 07h]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Func0xb4a                 ;Offset 0xb4a
Func0xad2 ENDP

Func0xadd PROC NEAR                     ;Offset 0xadd
    inc       byte ptr [bp + 07h]
    mov       dl, byte ptr [bp + 07h]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Func0xb51                 ;Offset 0xb51
Func0xadd ENDP

Func0xae8 PROC NEAR                     ;Offset 0xae8
    sub       dh, ch
    sub       dl, cl
    mov       al, ch
    push      bx
    push      dx
    mov       bl, dl
    sub       bh, bh
    inc       bx
    shl       bx, 03h
    mov       word ptr [bp + 04h], bx
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul       word ptr [bp + 02h]
    sub       ch, ch
    shl       cx, 03h
    mov       di, cx
    cmp       byte ptr [bp + 06h], 0fh
    jb        Label0xb1f                ;Offset 0xb1f
    add       cx, di
    add       word ptr [bp + 04h], bx
    cmp       byte ptr [bp + 06h], 18h
    jne       Label0xb1f                ;Offset 0xb1f
    add       cx, di
    add       word ptr [bp + 04h], bx
Label0xb1f:                             ;Offset 0xb1f
    add       ax, cx
    mov       di, ax
    adc       dx, 0000h
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
    mov       byte ptr [bp + 07h], dl
    pop       cx
    mov       al, ch
    inc       al
    mul       byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov       dx, ax
    pop       ax
    mov       al, ah
    mov       cx, word ptr [bp + 04h]
    sub       word ptr [bp + 02h], cx
    mov       bx, dx
    shr       cx, 01h
Func0xae8 ENDP
;continue!
Func0xb44 PROC NEAR                     ;Offset 0xb44
    push      cx
Func0xb44 ENDP
;continue!
Func0xb45 PROC NEAR                     ;Offset 0xb45
    stosw
    test      di, di
    je        Func0xad2                 ;Offset 0xad2
Func0xb45 ENDP
;continue!
Func0xb4a PROC NEAR                     ;Offset 0xb4a
    loop      Func0xb45                 ;Offset 0xb45
    add       di, word ptr [bp + 02h]
    jb        Func0xadd                 ;Offset 0xadd
Func0xb4a ENDP
;continue!
Func0xb51 PROC NEAR                     ;Offset 0xb51
    pop       cx
    dec       bx
    jne       Func0xb44                 ;Offset 0xb44
    add       sp, 000ch
    pop       bp
    ret
Func0xb51 ENDP

Func0xb5a PROC NEAR                    ;Offset 0xb5a
    push      bp
    sub       sp, 0ch
    mov       bp, sp
    push      bx
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    mov       al, byte ptr es:[bx + 06h]
    mov       byte ptr [bp + 06h], al
    mov       ax, word ptr es:[bx + 0bh]
    mov       word ptr [bp + 02h], ax
    pop       ax
    pop       bx
    call      GetA000SelectorInES       ;Offset 0x3f4c
    test      al, al
    je        Func0xae8                 ;Offset 0xae8
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0xae8                 ;Offset 0xae8
    push      bx
    sub       dh, ch
    sub       dl, cl
    mov       al, ch
    mov       bl, cl
    mov       cx, dx
    push      cx
    sub       ch, ch
    inc       cx
    shl       cx, 03h
    mov       word ptr [bp + 04h], cx
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       ah, ah
    mul       si
    mul       word ptr [bp + 02h]
    sub       bh, bh
    shl       bx, 03h
    mov       di, bx
    cmp       byte ptr [bp + 06h], 0fh
    jb        Label0xbca                ;Offset 0xbca
    add       bx, di
    add       word ptr [bp + 04h], cx
    cmp       byte ptr [bp + 06h], 18h
    jne       Label0xbca                ;Offset 0xbca
    add       bx, di
    add       word ptr [bp + 04h], cx
Label0xbca:                             ;Offset 0xbca
    add       ax, bx
    mov       di, ax
    adc       dx, 0000h
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
    mov       byte ptr [bp + 07h], dl
    mov       byte ptr [bp + 08h], dl
    pop       cx
    pop       bx
    push      bx
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       word ptr [bp + 02h]
    mov       si, di
    add       si, ax
    adc       dx, 0000h
    add       dl, byte ptr [bp + 07h]
    call      SetVGAReadAperture        ;Offset 0x3c66
    mov       byte ptr [bp + 08h], dl
    pop       dx
    pop       ax
    mov       al, ah
    push      es
    pop       ds
    mov       cx, word ptr [bp + 04h]
    sub       word ptr [bp + 02h], cx
    push      bx
    mov       bx, dx
    shr       cx, 01h
Label0xc15:                             ;Offset 0xc15
    push      cx
Label0xc16:                             ;Offset 0xc16
    movsw
    test      di, di
    je        Label0xc46                ;Offset 0xc46
Label0xc1b:                             ;Offset 0xc1b
    test      si, si
    je        Label0xc51                ;Offset 0xc51
Label0xc1f:                             ;Offset 0xc1f
    loop      Label0xc16                ;Offset 0xc16
    add       di, word ptr [bp + 02h]
    jb        Label0xc5c                ;Offset 0xc5c
Label0xc26:                             ;Offset 0xc26
    add       si, word ptr [bp + 02h]
    jb        Label0xc67                ;Offset 0xc67
Label0xc2b:                             ;Offset 0xc2b
    pop       cx
    dec       bx
    jne       Label0xc15                ;Offset 0xc15
    pop       bx
Label0xc30:                             ;Offset 0xc30
    push      cx
Label0xc31:                             ;Offset 0xc31
    stosw
    test      di, di
    je        Label0xc72                ;Offset 0xc72
Label0xc36:                             ;Offset 0xc36
    loop      Label0xc31                ;Offset 0xc31
    pop       cx
    add       di, word ptr [bp + 02h]
    jb        Label0xc7d                ;Offset 0xc7d
Label0xc3e:                             ;Offset 0xc3e
    dec       bx
    jne       Label0xc30                ;Offset 0xc30
    add       sp, 0ch
    pop       bp
    ret
Label0xc46:                             ;Offset 0xc46
    inc       byte ptr [bp + 7]
    mov       dl, byte ptr [bp + 7]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Label0xc1b                ;Offset 0xc1b
Label0xc51:                             ;Offset 0xc51
    inc       byte ptr [bp + 8]
    mov       dl, byte ptr [bp + 8]
    call      SetVGAReadAperture        ;Offset 0x3c66
    jmp       Label0xc1f                ;Offset 0xc1f
Label0xc5c:                             ;Offset 0xc5c
    inc       byte ptr [bp + 7]
    mov       dl, byte ptr [bp + 7]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Label0xc26                ;Offset 0xc26
Label0xc67:                             ;Offset 0xc67
    inc       byte ptr [bp + 8]
    mov       dl, byte ptr [bp + 8]
    call      SetVGAReadAperture        ;Offset 0x3c66
    jmp       Label0xc2b                ;Offset 0xc2b
Label0xc72:                             ;Offset 0xc72
    inc       byte ptr [bp + 7]
    mov       dl, byte ptr [bp + 7]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Label0xc36                ;Offset 0xc36
Label0xc7d:                             ;Offset 0xc7d
    inc       byte ptr [bp + 7]
    mov       dl, byte ptr [bp + 7]
    call      SetVGAWriteAperture       ;Offset 0x3cbb
    jmp       Label0xc3e                ;Offset 0xc3e
Func0xb5a ENDP

;Offset 0xc88
Data0xc88               DW offset Func0xcc3;Offset 0xcc3
                        DW offset Func0xcbb;Offset 0xcbb
                        DW offset Func0xd16;Offset 0xd16
                        DW offset Func0xd81;Offset 0xd81
                        DW offset Func0xd81;Offset 0xd81
                        DW offset Func0xd81;Offset 0xd81
                        DW offset Func0xe27;Offset 0xe27
                        DW offset Func0xe27;Offset 0xe27
                        DW offset Func0xe27;Offset 0xe27
                        DW offset Func0x85c;Offset 0x85c

ScrollDownWindow:                       ;Offset 0x0c9c
    pushaw
    push      es
    push      ax
    push      bx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    movzx     si, byte ptr es:[bx + 04h]
    pop       bx
    pop       ax
    jb        Label0xcb6                ;Offset 0xcb6
    shl       si, 01h
    call      word ptr cs:[si + Data0xc88];Offset 0xc88
Label0xcb6:                             ;Offset 0xcb6
    pop       es
    popaw
    pop       ds
    pop       si
    iret

Func0xcbb PROC NEAR                     ;Offset 0xcbb
    call      GetB000SelectorInES       ;Offset 0x3f52
    jmp       Func0xcc6                 ;Offset 0xcc6
Func0xcbb ENDP

Func0xcc0 PROC NEAR                     ;Offset 0xcc0
    jmp       Func0x825                 ;Offset 0x825
Func0xcc0 ENDP

Func0xcc3 PROC NEAR                     ;Offset 0xcc3
    call      GetB800SelectorInES       ;Offset 0x3f58
Func0xcc3 ENDP
;continue!
Func0xcc6 PROC NEAR                     ;Offset 0xcc6
    test      al, al
    je        Func0xcc0                 ;Offset 0xcc0
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0xcc0                 ;Offset 0xcc0
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       al, 20h
    mov       ah, bh
    push      ax
    mov       ax, bp
    mul       dh
    add       al, cl
    adc       ah, 00h
    shl       ax, 01h
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov       di, ax
    shl       bp, 01h
    sub       bh, bh
    push      bx
    mov       si, di
Label0xcf5:                             ;Offset 0xcf5
    sub       si, bp
    dec       bx
    jne       Label0xcf5                ;Offset 0xcf5
    pop       bx
    push      es
    pop       ds
    sub       dl, cl
    sub       dh, ch
    mov       cl, dl
    sub       ch, ch
    inc       cx
    mov       dl, dh
    sub       dh, dh
    inc       dx
    sub       dx, bx
    pop       ax
    neg       bp
    jmp       Func0x8ad                 ;Offset 0x8ad
Func0xcc6 ENDP

Func0xd13 PROC NEAR                     ;Offset 0xd13
    jmp       Func0x8ca                 ;Offset 0x8ca
Func0xd13 ENDP

Func0xd16 PROC NEAR                     ;Offset 0xd16
    call      GetB800SelectorInES       ;Offset 0x3f58
    test      al, al
    je        Func0xd13                 ;Offset 0xd13
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0xd13                 ;Offset 0xd13
    push      bx
    mov       al, dh
    sub       dh, ch
    sub       dl, cl
    mov       bh, cl
    mov       cx, dx
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    shr       si, 01h
    sub       ah, ah
    inc       ax
    mul       bp
    mul       si
    add       al, bh
    adc       ah, 00h
    mov       di, ax
    sub       di, bp
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    sub       ch, ch
    inc       cx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    je        Label0xd67                ;Offset 0xd67
    shl       di, 01h
    shl       cx, 01h
    shl       bp, 01h
Label0xd67:                             ;Offset 0xd67
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       bp
    mov       si, di
    sub       si, ax
    pop       dx
    pop       ax
    mov       al, ah
    push      es
    pop       ds
    neg       bp
    jmp       Func0x993                 ;Offset 0x993
Func0xd16 ENDP

Func0xd81 PROC NEAR                     ;Offset 0xd81
    call      GetA000SelectorInES       ;Offset 0x3f4c
    test      al, al
    je        Func0x9de                 ;Offset 0x9de
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0x9de                 ;Offset 0x9de
    push      bx
    mov       al, dh
    sub       dh, ch
    sub       dl, cl
    mov       bh, cl
    mov       cx, dx
    mov       bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       ah, ah
    inc       ax
    mul       bp
    mul       si
    add       ax, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    add       al, bh
    adc       ah, 00h
    mov       di, ax
    sub       di, bp
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    sub       ch, ch
    inc       cx
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       bp
    mov       si, di
    sub       si, ax
    pop       dx
    pop       ax
    mov       al, ah
    push      es
    pop       ds
    neg       bp
    push      ax
    push      dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    or        ah, VGA_GCTL5_CPUDirect   ;0x1
    out       dx, ax
    pop       dx
    sub       bp, cx
Label0xdf6:                             ;Offset 0xdf6
    push      cx
    rep movsb
    pop       cx
    add       di, bp
    add       si, bp
    dec       dx
    jne       Label0xdf6                ;Offset 0xdf6
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    or        ah, VGA_GCTL5_SplatBLU    ;0x2
    out       dx, ax
    pop       ax
Label0xe11:                             ;Offset 0xe11
    push      cx
    rep stosb
    pop       cx
    add       di, bp
    dec       bx
    jne       Label0xe11                ;Offset 0xe11
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       al, VGA_GCTLIdx_GraphicMode;0x5
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, NOT (VGA_GCTL5_CPUDirect OR VGA_GCTL5_SplatBLU);0xfc
    out       dx, ax
    ret       
Func0xd81 ENDP

Func0xe27 PROC NEAR                     ;Offset 0xe27
    push      bp
    sub       sp, 0ch
    mov       bp, sp
    push      bx
    push      ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    mov       al, byte ptr es:[bx + 06h]
    mov       byte ptr [bp + 06h], al
    mov       ax, word ptr es:[bx + 0bh]
    mov       word ptr [bp + 02h], ax
    pop       ax
    pop       bx
    call      GetA000SelectorInES       ;Offset 0x3f4c
    test      al, al
    je        Func0xae8                 ;Offset 0xae8
    mov       bl, al
    mov       al, dh
    sub       al, ch
    cmp       bl, al
    ja        Func0xae8                 ;Offset 0xae8
    push      bx
    mov       al, dh
    sub       dh, ch
    sub       dl, cl
    mov       bl, cl
    mov       cx, dx
    push      cx
    sub       ch, ch
    inc       cx
    shl       cx, 03h
    mov       word ptr [bp + 04h], cx
    mov       si, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       ah, ah
    inc       ax
    mul       si
    mul       word ptr [bp + 02h]
    sub       bh, bh
    shl       bx, 03h
    mov       di, bx
    cmp       byte ptr [bp + 06h], 0fh
    jb        Label0xe98                ;Offset 0xe98
    add       bx, di
    add       word ptr [bp + 04h], cx
    cmp       byte ptr [bp + 06h], 18h
    jne       Label0xe98                ;Offset 0xe98
    add       bx, di
    add       word ptr [bp + 04h], cx
Label0xe98:                             ;Offset 0xe98
    add       ax, bx
    mov       di, ax
    adc       dx, 0000h
    sub       di, word ptr [bp + 02h]
    sbb       dx, 00h
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
    mov       byte ptr [bp + 07h], dl
    mov       byte ptr [bp + 08h], dl
    pop       cx
    pop       bx
    push      bx
    mov       al, ch
    sub       ah, ah
    sub       al, bl
    inc       ax
    mul       si
    mov       dx, ax
    sub       ch, ch
    shl       cx, 03h
    pop       ax
    push      ax
    push      dx
    sub       ah, ah
    mul       si
    mov       bx, ax
    mul       word ptr [bp + 02h]
    mov       si, di
    sub       si, ax
    adc       dx, 00h
    jb        Label0xee3                ;Offset 0xee3
    mov       dh, byte ptr [bp + 07h]
    sub       dh, dl
    xchg      dh, dl
    call      SetVGAReadAperture        ;Offset 0x3c66
    mov       byte ptr [bp + 08h], dl
Label0xee3:                             ;Offset 0xee3
    pop       dx
    pop       ax
    mov       al, ah
    mov       cx, word ptr [bp + 04h]
    add       word ptr [bp + 02h], cx
    neg       word ptr [bp + 02h]
    push      es
    pop       ds
    shr       cx, 01h
Label0xef4:                             ;Offset 0xef4
    push      cx
Label0xef5:                             ;Offset 0xef5
    movsw
    test      di, di
    je        Label0xf28                ;Offset 0xf28
Label0xefa:                             ;Offset 0xefa
    test      si, si
    je        Label0xf2d                ;Offset 0xf2d
Label0xefe:                             ;Offset 0xefe
    loop      Label0xef5                ;Offset 0xef5
    pop       cx
    add       di, word ptr [bp + 02h]
    jae       Label0xf32                ;Offset 0xf32
Label0xf06:                             ;Offset 0xf06
    dec       dx
    je        Label0xf10                ;Offset 0xf10
    add       si, word ptr [bp + 02h]
    jae       Label0xf37                ;Offset 0xf37
Label0xf0e:
    jmp       Label0xef4                ;Offset 0xef4
Label0xf10:                             ;Offset 0xf10
    push      cx
Label0xf11:                             ;Offset 0xf11
    stosw
    test      di, di
    je        Label0xf3c                ;Offset 0xf3c
Label0xf16:                             ;Offset 0xf16
    loop      Label0xf11                ;Offset 0xf11
    pop       cx
    dec       bx
    je        Label0xf23                ;Offset 0xf23
    add       di, word ptr [bp + 02h]
    jae       Label0xf41                ;Offset 0xf41
Label0xf21:                             ;Offset 0xf21
    jmp       Label0xf10                ;Offset 0xf10
Label0xf23:                             ;Offset 0xf23
    add       sp, 0ch
    pop       bp
    ret
Label0xf28:                             ;Offset 0xf28
    call      MoveVGAWriteApertureUp64K ;Offset 0x3cdc
    jmp       Label0xefa                ;Offset 0xefa
Label0xf2d:                             ;Offset 0xf2d
    call      MoveVGAReadApertureUp64K  ;Offset 0x3c8d
    jmp       Label0xefe                ;Offset 0xefe
Label0xf32:                             ;Offset 0xf32
    call      MoveVGAWriteApertureDown64K;Offset 0x3cf0
    jmp       Label0xf06                ;Offset 0xf06
Label0xf37:                             ;Offset 0xf37
    call      MoveVGAReadApertureDown64K;Offset 0x3ca4
    jmp       Label0xf0e                ;Offset 0xf0e
Label0xf3c:                             ;Offset 0xf3c
    call      MoveVGAWriteApertureUp64K ;Offset 0x3cdc
    jmp       Label0xf16                ;Offset 0xf16
Label0xf41:                             ;Offset 0xf41
    call      MoveVGAWriteApertureDown64K;Offset 0x3cf0
    jmp       Label0xf21                ;Offset 0xf21
Func0xe27 ENDP

;Offset 0xf46
Data0xf46               DW Func0xf87;Offset 0xf87
                        DW Func0xf82;Offset 0xf82
                        DW Func0xfad;Offset 0xfad
                        DW Func0x10f0;Offset 0x10f0
                        DW Func0x10f0;Offset 0x10f0
                        DW Func0x10f0;Offset 0x10f0
                        DW Func0x117c;Offset 0x117c
                        DW Func0xfac;Offset 0xfac
                        DW Func0xfac;Offset 0xfac
                        DW Func0xfac;Offset 0xfac

ReadCharacterAndAttributeAtCursorPosition:;Offset 0x0f5a
    push       bx
    push       dx
    push       es
    push       bx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       FindModeData             ;Offset 0x40d9
    mov        si, word ptr es:[bx + 04h]
    pop        bx
    jb         Label0xf7c               ;Offset 0xf7c
    and        si, 00ffh
    shl        si, 01h
    ;cmp        si, 0014h
    DB 081h, 0FEh, 014h, 000h
    jae        Label0xf7c               ;Offset 0xf7c
    call       word ptr cs:[si + Data0xf46];Offset 0xf46
Label0xf7c:                             ;Offset 0xf7c
    pop        es
    pop        dx
    pop        bx
    pop        ds
    pop        si
    iret       

Func0xf82 PROC NEAR                     ;Offset 0xf82
    call       GetB000SelectorInES      ;Offset 0x3f52
    jmp        Func0xf8a                ;Offset 0xf8a
Func0xf82 ENDP

Func0xf87 PROC NEAR                     ;Offset 0xf87
    call       GetB800SelectorInES      ;Offset 0x3f58
Func0xf87 ENDP
;continue!
Func0xf8a PROC NEAR                     ;Offset 0xf8a
    mov        bl, bh
    sub        bh, bh
    mov        ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    imul       ax, bx
    mov        si, ax
    shl        bx, 01h
    mov        bx, word ptr [bx + DBA_CursorPositionPage0];Offset 0x450
    mov        al, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        bh
    add        al, bl
    adc        ah, 00h
    shl        ax, 01h
    add        si, ax
    lodsw      word ptr es:[si]
Func0xf8a ENDP
;continue!
Func0xfac PROC NEAR                     ;Offset 0xfac
    ret
Func0xfac ENDP

Func0xfad PROC NEAR                     ;Offset 0xfad
    push       cx
    push       bp
    push       di
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub        sp, cx
    mov        bp, sp
    push       cx
    les        di, ds:[INT_43_Handler]  ;Offset 0x10c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov        al, cl
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        bh, dl
    shl        bh, 01h
    mov        dl, dh
    sub        dh, dh
    mul        dx
    add        al, bh
    adc        ah, 00h
    mov        si, ax
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       GetB800SelectorInDS      ;Offset 0x3f64
    push       cx
    push       di
    push       es
    mov        bx, ss
    mov        es, bx
    mov        di, bp
    cmp        al, 06h
    jne        Label0x1003              ;Offset 0x1003
    shr        si, 01h
Label0xfed:                             ;Offset 0xfed
    movsb
    dec        si
    test       si, 2000h
    jne        Label0xffb               ;Offset 0xffb
    add        si, 2000h
    jmp        Label0xfff               ;Offset 0xfff
Label0xffb:                             ;Offset 0xffb
    sub        si, 1fb0h
Label0xfff:                             ;Offset 0xfff
    loop       Label0xfed               ;Offset 0xfed
    jmp        Label0x101e              ;Offset 0x101e
Label0x1003:                            ;Offset 0x1003
    lodsw
    xchg       al, ah
    call       Func0x105b               ;Offset 0x105b
    stosb
    dec        si
    dec        si
    test       si, 2000h
    jne        Label0x1018              ;Offset 0x1018
    add        si, 2000h
    jmp        Label0x101c              ;Offset 0x101c
Label0x1018:                            ;Offset 0x1018
    sub        si, 1fb0h
Label0x101c:                            ;Offset 0x101c
    loop       Label0x1003              ;Offset 0x1003
Label0x101e:                            ;Offset 0x101e
    pop        ds
    pop        si
    pop        cx
    sub        ax, ax
    mov        bx, cx
Label0x1025:                            ;Offset 0x1025
    mov        di, bp
    mov        cx, bx
    repe cmpsb
    je         Label0x1033              ;Offset 0x1033
    add        si, cx
    inc        al
    jne        Label0x1025              ;Offset 0x1025
Label0x1033:                            ;Offset 0x1033
    cbw
    pop        bx
    add        sp, bx
    pop        di
    pop        bp
    pop        cx
    ret
Func0xfad ENDP

;Offset 0x103b
Data0x103b              DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 001h, 002h, 004h, 008h, 010h, 020h, 040h, 080h
                        DB 001h, 002h, 004h, 008h, 010h, 020h, 040h, 080h
                        DB 001h, 002h, 004h, 008h, 010h, 020h, 040h, 080h

Func0x105b PROC NEAR                    ;Offset 0x105b
    push di
    sub  bx, bx
    mov  dl, bl
    mov  di, offset Data0x103b          ;Offset 0x103b
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    shr  ax, 02h
    inc  di
    mov  bl, al
    and  bl, 03h
    shl  bx, 03h
    add  dl, byte ptr cs:[bx + di]
    mov  al, dl
    pop  di
    ret  
Func0x105b ENDP

Func0x10db PROC NEAR                    ;Offset 0x10db
    mov  bl, bh
    sub  bh, bh
    shl  bx, 01h
    mov  dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    shr  bx, 01h
Label0x10e7:                            ;Offset 0x10e7
    add  si, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec  bx
    jne  Label0x10e7                    ;Offset 0x10e7
    jmp  Func0x10fd                     ;Offset 0x10fd
Func0x10db ENDP

Func0x10f0 PROC NEAR                    ;Offset 0x10f0
    push       cx
    push       bp
    push       di
    sub        si, si
    or         bh, bh
    jne        Func0x10db               ;Offset 0x10db
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Func0x10f0 ENDP

Func0x10fd PROC NEAR                    ;Offset 0x10fd
    mov        ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        cx, ax
    inc        ax
    shr        ax, 01h
    shl        ax, 01h
    sub        sp, ax
    mov        bp, sp
    push       ax
    les        di, ds:[INT_43_Handler]  ;Offset 0x10c
    mov        al, dh
    mul        cl
    mov        bh, dl
    mul        word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    add        al, bh
    adc        ah, 00h
    add        si, ax
    mov        bx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    call       GetA000SelectorInDS      ;Offset 0x3f5e
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        al, VGA_GCTLIdx_ColorCompare;0x2
    call       ReadIndexedRegister      ;Offset 0x3f84
    push       ax
    sub        ah, ah
    out        dx, ax
    mov        al, VGA_GCTLIdx_ColorDontCare;0x7
    call       ReadIndexedRegister      ;Offset 0x3f84
    push       ax
    mov        ah, 0fh
    out        dx, ax
    mov        al, VGA_GCTLIdx_GraphicMode;0x5
    call       ReadIndexedRegister      ;Offset 0x3f84
    push       ax
    or         ah, VGA_GCTL5_ReadColCmp ;0x8
    out        dx, ax
    push       cx
    push       di
    push       es
    push       ss
    pop        es
    mov        di, bp
    dec        bx
Label0x114f:                            ;Offset 0x114f
    lodsb
    not        al
    stosb
    add        si, bx
    loop       Label0x114f              ;Offset 0x114f
    pop        ds
    pop        si
    pop        cx
    mov        di, bp
    pop        ax
    out        dx, ax
    pop        ax
    out        dx, ax
    pop        ax
    out        dx, ax
    sub        ax, ax
    mov        bx, cx
Label0x1166:                            ;Offset 0x1166
    mov        di, bp
    mov        cx, bx
    repe cmpsb
    je         Label0x1174              ;Offset 0x1174
    add        si, cx
    inc        al
    jne        Label0x1166              ;Offset 0x1166
Label0x1174:                            ;Offset 0x1174
    cbw
    pop        bx
    add        sp, bx
    pop        di
    pop        bp
    pop        cx
    ret
Func0x10fd ENDP

Func0x117c PROC NEAR                    ;Offset 0x117c
    push       cx
    push       bp
    push       di
    mov        ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        cx, ax
    inc        ax
    shr        ax, 01h
    shl        ax, 01h
    sub        sp, ax
    mov        bp, sp
    push       ax
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov        al, dh
    mov        di, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl        di, 03h
    sub        dh, dh
    mov        si, dx
    shl        si, 03h
    mul        cl
    mul        di
    add        si, ax
    adc        dx, 00h
    call       SetVGAReadWriteAperture64K;Offset 0x3c10
    mov        dx, di
    les        di, ds:[INT_43_Handler]  ;Offset 0x10c
    call       GetA000SelectorInDS      ;Offset 0x3f5e
    push       cx
    push       di
    push       es
    mov        ax, ss
    mov        es, ax
    mov        di, bp
    sub        dx, 0008h
    mov        bx, cx
Label0x11c5:                            ;Offset 0x11c5
    mov        cx, 0008h
    sub        ax, ax
Label0x11ca:                            ;Offset 0x11ca
    cmp        byte ptr [si], 00h
    je         Label0x11d1              ;Offset 0x11d1
    or         al, 80h
Label0x11d1:                            ;Offset 0x11d1
    rol        al, 01h
    inc        si
    loop       Label0x11ca              ;Offset 0x11ca
    stosb
    add        si, dx
    jb         Label0x1201              ;Offset 0x1201
    cmp        si, dx
    je         Label0x1201              ;Offset 0x1201
Label0x11df:                            ;Offset 0x11df
    dec        bx
    jne        Label0x11c5              ;Offset 0x11c5
    pop        ds
    pop        si
    pop        cx
    mov        di, bp
    sub        ax, ax
    mov        bx, cx
Label0x11eb:                            ;Offset 0x11eb
    mov        di, bp
    mov        cx, bx
    repe cmpsb
    je         Label0x11f9              ;Offset 0x11f9
    add        si, cx
    inc        al
    jne        Label0x11eb              ;Offset 0x11eb
Label0x11f9:                            ;Offset 0x11f9
    cbw
    pop        bx
    add        sp, bx
    pop        di
    pop        bp
    pop        cx
    ret
Label0x1201:                            ;Offset 0x1201
    call       MoveVGAReadWriteAperturesUp64K;Offset 0x3c4f
    jmp        Label0x11df              ;Offset 0x11df
Func0x117c ENDP

;Offset 0x1206
WriteCharacterCalltable DW offset Func0x125b;Offset 0x125b
                        DW offset Func0x1241;Offset 0x1241
                        DW offset Func0x1280;Offset 0x1280
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x12b7;Offset 0x12b7
                        DW offset Func0x12c9;Offset 0x12c9
                        DW offset Func0x12d9;Offset 0x12d9
                        DW offset Func0x127f;Offset 0x127f

WriteCharacterAndAttributeAtCursorPosition:;Offset 0x121a
    pushaw
    push       es
    push       ax
    push       bx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       FindModeData             ;Offset 0x40d9
    mov        si, word ptr es:[bx + 04h]
    pop        bx
    pop        ax
    jb         Label0x1237              ;Offset 0x1237
    and        si, 00ffh
    shl        si, 01h
    call       word ptr cs:[si + WriteCharacterCalltable];Offset 0x1206
Label0x1237:                            ;Offset 0x1237
    pop        es
    mov        bp, sp
    mov        word ptr [bp + 0eh], ax
    popaw
    pop        ds
    pop        si
    iret

Func0x1241 PROC NEAR                    ;Offset 0x1241
    call       GetB000SelectorInES      ;Offset 0x3f52
    jmp        Func0x125e               ;Offset 0x125e
Func0x1241 ENDP

Func0x1246 PROC NEAR                    ;Offset 0x1246
    mov        bl, bh
    sub        bh, bh
    shl        bx, 01h
    mov        dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    shr        bx, 01h
Label0x1252:                            ;Offset 0x1252
    add        si, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec        bx
    jne        Label0x1252              ;Offset 0x1252
    jmp        Func0x126b               ;Offset 0x126b
Func0x1246 ENDP

Func0x125b PROC NEAR                    ;Offset 0x125b
    call       GetB800SelectorInES      ;Offset 0x3f58
Func0x125b ENDP

Func0x125e PROC NEAR                    ;Offset 0x125e
    push       ax
    mov        ah, bl
    sub        si, si
    test       bh, bh
    jne        Func0x1246               ;Offset 0x1246
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Func0x125e ENDP
;continue!
Func0x126b PROC NEAR                    ;Offset 0x126b
    mov        di, ax
    mov        ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        dh
    add        al, dl
    adc        ah, 00h
    shl        ax, 01h
    add        ax, si
    xchg       ax, di
    rep stosw
    pop        ax
Func0x126b ENDP
;continue!
Func0x127f PROC NEAR                    ;Offset 0x127f
    ret
Func0x127f ENDP

Func0x1280 PROC NEAR                    ;Offset 0x1280
    call       GetB800SelectorInES      ;Offset 0x3f58
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Label0x1287:                            ;Offset 0x1287
    push       ax
    call       Func0x168a               ;Offset 0x168a
    pop        ax
    inc        dx
    loop       Label0x1287              ;Offset 0x1287
    sub        ax, ax
    ret
Func0x1280 ENDP

Func0x1292 PROC NEAR                    ;Offset 0x1292
    call       GetA000SelectorInES      ;Offset 0x3f4c
    push       bx
    mov        bl, bh
    sub        bh, bh
    shl        bx, 01h
    mov        dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    pop        bx
    cmp        byte ptr ds:[BDA_DisplayMode], BDA_DM_640x480_BW_Graphics;Offset 0x449 0x11
    jne        Label0x12ab              ;Offset 0x12ab
    or         bl, 0fh
Label0x12ab:                            ;Offset 0x12ab
    push       ax
    call       Func0x1766               ;Offset 0x1766
    pop        ax
    inc        dx
    loop       Label0x12ab              ;Offset 0x12ab
    mov        ax, 0f02h
    ret
Func0x1292 ENDP

Func0x12b7 PROC NEAR                    ;Offset 0x12b7
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Label0x12be:                            ;Offset 0x12be
    push       ax
    call       Func0x17e1               ;Offset 0x17e1
    pop        ax
    inc        dx
    loop       Label0x12be              ;Offset 0x12be
    sub        ax, ax
    ret
Func0x12b7 ENDP

Func0x12c9 PROC NEAR                    ;Offset 0x12c9
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Label0x12d0:                            ;Offset 0x12d0
    push       ax
    call       Func0x183f               ;Offset 0x183f
    pop        ax
    inc        dx
    loop       Label0x12d0              ;Offset 0x12d0
    ret
Func0x12c9 ENDP

Func0x12d9 PROC NEAR                    ;Offset 0x12d9
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Label0x12e0:                            ;Offset 0x12e0
    push       ax
    call       Func0x1897               ;Offset 0x1897
    pop        ax
    inc        dx
    loop       Label0x12e0              ;Offset 0x12e0
    ret
Func0x12d9 ENDP

;Offset 0x12e9
Data0x12e9              DW offset Func0x133e;Offset 0x133e
                        DW offset Func0x1324;Offset 0x1324
                        DW offset Func0x1280;Offset 0x1280
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x1292;Offset 0x1292
                        DW offset Func0x12b7;Offset 0x12b7
                        DW offset Func0x12c9;Offset 0x12c9
                        DW offset Func0x12d9;Offset 0x12d9
                        DW offset Func0x127f;Offset 0x127f

WriteCharacterOnlyAtCursorPosition:     ;Offset 0x12fd
    pushaw
    push       es
    push       ax
    push       bx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       FindModeData             ;Offset 0x40d9
    mov        si, word ptr es:[bx + 04h]
    pop        bx
    pop        ax
    jb         Label0x131a              ;Offset 0x131a
    and        si, 0ffh
    shl        si, 01h
    call       word ptr cs:[si + Data0x12e9];Offset 0x12e9
Label0x131a:                            ;Offset 0x131a
    pop        es
    mov        bp, sp
    mov        word ptr [bp + 0eh], ax
    popaw
    pop        ds
    pop        si
    iret

Func0x1324 PROC NEAR                    ;Offset 0x1324
    call       GetB000SelectorInES      ;Offset 0x3f52
    jmp        Func0x1341               ;Offset 0x1341
Func0x1324 ENDP

Func0x1329 PROC NEAR                    ;Offset 0x1329
    mov        bl, bh
    sub        bh, bh
    shl        bx, 01h
    mov        dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    shr        bx, 01h
Label0x1335:                            ;Offset 0x1335
    add        si, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec        bx
    jne        Label0x1335              ;Offset 0x1335
    jmp        Func0x134c               ;Offset 0x134c
Func0x1329 ENDP

Func0x133e PROC NEAR                    ;Offset 0x133e
    call       GetB800SelectorInES      ;Offset 0x3f58
Func0x133e ENDP
;continue!
Func0x1341 PROC NEAR                    ;Offset 0x1341
    push       ax
    sub        si, si
    test       bh, bh
    jne        Func0x1329               ;Offset 0x1329
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
Func0x1341 ENDP
;continue!
Func0x134c PROC NEAR                    ;Offset 0x134c
    mov        di, ax
    mov        ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        dh
    add        al, dl
    adc        ah, 00h
    shl        ax, 01h
    add        ax, si
    xchg       ax, di
Label0x135d:                            ;Offset 0x135d
    stosb
    inc        di
    loop       Label0x135d              ;Offset 0x135d
    pop        ax
    ret
Func0x134c ENDP

;Offset 0x1363
TeletypeCalltable       DW offset Func0x139e;Offset 0x139e
                        DW offset Func0x1399;Offset 0x1399
                        DW offset Func0x1437;Offset 0x1437
                        DW offset Func0x14bd;Offset 0x14bd
                        DW offset Func0x14bd;Offset 0x14bd
                        DW offset Func0x14bd;Offset 0x14bd
                        DW offset Func0x1555;Offset 0x1555
                        DW offset Func0x15bc;Offset 0x15bc
                        DW offset Func0x1623;Offset 0x1623
                        DW offset Func0x127f;Offset 0x127f

TeletypeOutput:                         ;Offset 0x1377
    pushaw
    push       es
    push       ax
    push       bx
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       FindModeData             ;Offset 0x40d9
    mov        si, word ptr es:[bx + 04h]
    pop        bx
    pop        ax
    jb         Label0x1394              ;Offset 0x1394
    and        si, 0ffh
    shl        si, 01h
    call       word ptr cs:[si + TeletypeCalltable];Offset 0x1363
Label0x1394:                            ;Offset 0x1394
    pop        es
    popaw
    pop        ds
    pop        si
    iret

Func0x1399 PROC NEAR                    ;Offset 0x1399
    call       GetB000SelectorInES      ;Offset 0x3f52
    jmp        Func0x13a1               ;Offset 0x13a1
Func0x1399 ENDP

Func0x139e PROC NEAR                    ;Offset 0x139e
    call       GetB800SelectorInES      ;Offset 0x3f58
Func0x139e ENDP
;continue!
Func0x13a1 PROC NEAR                    ;Offset 0x13a1
    mov        bl, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    sub        bh, bh
    shl        bx, 01h
    mov        dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    mov        bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    cmp        al, 0dh
    jbe        Label0x13da              ;Offset 0x13da
Label0x13b5:                            ;Offset 0x13b5
    push       ax
    mov        ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul        dh
    mov        di, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    add        al, dl
    adc        ah, 00h
    shl        ax, 01h
    add        di, ax
    pop        ax
    stosb
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x13d6              ;Offset 0x13d6
    call       Func0x763                ;Offset 0x763
    ret
Label0x13d6:                            ;Offset 0x13d6
    sub        dl, dl
    jmp        Label0x13e8              ;Offset 0x13e8
Label0x13da:                            ;Offset 0x13da
    jb         Label0x13e2              ;Offset 0x13e2
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
Label0x13e2:                            ;Offset 0x13e2
    cmp        al, 0ah
    ja         Label0x13b5              ;Offset 0x13b5
    jb         Label0x1422              ;Offset 0x1422
Label0x13e8:                            ;Offset 0x13e8
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x13f4              ;Offset 0x13f4
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x13f4:                            ;Offset 0x13f4
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    call       Func0x763                ;Offset 0x763
    mov        di, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov        si, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        ax, si
    shl        si, 01h
    mul        byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    mov        cx, ax
    push       ds
    push       es
    pop        ds
    rep movsw
    pop        ds
    mov        ah, byte ptr es:[di + 01h]
    mov        al, 20h
    mov        cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    rep stosw
    pop        cx
    ret
Label0x1422:                            ;Offset 0x1422
    cmp        al, 07h
    je         Label0x1433              ;Offset 0x1433
    cmp        al, 08h
    jne        Label0x13b5              ;Offset 0x13b5
    test       dl, dl
    je         Label0x1432              ;Offset 0x1432
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x1432:                            ;Offset 0x1432
    ret
Label0x1433:                            ;Offset 0x1433
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x13a1 ENDP

Func0x1437 PROC NEAR                    ;Offset 0x1437
    call       GetB800SelectorInES      ;Offset 0x3f58
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    sub        bh, bh
    cmp        al, 0dh
    jbe        Label0x1459              ;Offset 0x1459
Label0x1444:                            ;Offset 0x1444
    push       dx
    call       Func0x168a               ;Offset 0x168a
    pop        dx
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x1455              ;Offset 0x1455
    call       Func0x763                ;Offset 0x763
    ret
Label0x1455:                            ;Offset 0x1455
    sub        dl, dl
    jmp        Label0x1467              ;Offset 0x1467
Label0x1459:                            ;Offset 0x1459
    jb         Label0x1461              ;Offset 0x1461
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
Label0x1461:                            ;Offset 0x1461
    cmp        al, 0ah
    ja         Label0x1444              ;Offset 0x1444
    jb         Label0x14a8              ;Offset 0x14a8
Label0x1467:                            ;Offset 0x1467
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x1473              ;Offset 0x1473
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x1473:                            ;Offset 0x1473
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    call       Func0x763                ;Offset 0x763
    sub        di, di
    mov        si, 0140h
    mov        cx, 1e00h
    push       ds
    push       es
    pop        ds
    rep movsb
    mov        di, 2000h
    mov        si, 2140h
    mov        cx, 1e00h
    rep movsb
    sub        ax, ax
    mov        di, 1e00h
    mov        cx, 00a0h
    rep stosw
    mov        di, 3e00h
    mov        cx, 00a0h
    rep stosw
    pop        ds
    pop        cx
    ret
Label0x14a8:                            ;Offset 0x14a8
    cmp        al, 07h
    je         Label0x14b9              ;Offset 0x14b9
    cmp        al, 08h
    jne        Label0x1444              ;Offset 0x1444
    test       dl, dl
    je         Label0x14b8              ;Offset 0x14b8
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x14b8:                            ;Offset 0x14b8
    ret
Label0x14b9:                            ;Offset 0x14b9
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x1437 ENDP

Func0x14bd PROC NEAR                    ;Offset 0x14bd
    call       GetA000SelectorInES      ;Offset 0x3f4c
    movzx      si, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    shl        si, 01h
    mov        dx, word ptr [si + DBA_CursorPositionPage0];Offset 0x450
    mov        bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    cmp        al, 0dh
    jbe        Label0x14e6              ;Offset 0x14e6
Label0x14d3:                            ;Offset 0x14d3
    call       Func0x1766               ;Offset 0x1766
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x14e2              ;Offset 0x14e2
    call       Func0x763                ;Offset 0x763
    ret
Label0x14e2:                            ;Offset 0x14e2
    sub        dl, dl
    jmp        Label0x14f4              ;Offset 0x14f4
Label0x14e6:                            ;Offset 0x14e6
    jb         Label0x14ee              ;Offset 0x14ee
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
Label0x14ee:                            ;Offset 0x14ee
    cmp        al, 0ah
    ja         Label0x14d3              ;Offset 0x14d3
    jb         Label0x1540              ;Offset 0x1540
Label0x14f4:                            ;Offset 0x14f4
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x1500              ;Offset 0x1500
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x1500:                            ;Offset 0x1500
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    call       Func0x763                ;Offset 0x763
    mov        di, word ptr ds:[BDA_VideoBufferOffset];Offset 0x44e
    mov        cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mul        cl
    mov        si, di
    add        si, ax
    push       ax
    mov        dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    sub        dh, dh
    mul        dx
    mov        cx, ax
    push       ds
    push       es
    pop        ds
    mov        dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov        al, VGA_GCTLIdx_GraphicMode;0x5
    call       ReadIndexedRegister      ;Offset 0x3f84
    push       ax
    or         ah, VGA_GCTL5_CPUDirect  ;0x1
    out        dx, ax
    rep movsb
    pop        ax
    out        dx, ax
    pop        ds
    pop        cx
    sub        ax, ax
    rep stosb
    pop        cx
    ret
Label0x1540:                            ;Offset 0x1540
    cmp        al, 07h
    je         Label0x1551              ;Offset 0x1551
    cmp        al, 08h
    jne        Label0x14d3              ;Offset 0x14d3
    test       dl, dl
    je         Label0x1550              ;Offset 0x1550
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x1550:                            ;Offset 0x1550
    ret
Label0x1551:                            ;Offset 0x1551
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x14bd ENDP

Func0x1555 PROC NEAR                    ;Offset 0x1555
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    sub        bh, bh
    cmp        al, 0dh
    jbe        Label0x1577              ;Offset 0x1577
Label0x1562:                            ;Offset 0x1562
    push       dx
    call       Func0x17e1               ;Offset 0x17e1
    pop        dx
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x1573              ;Offset 0x1573
    call       Func0x763                ;Offset 0x763
    ret
Label0x1573:                            ;Offset 0x1573
    sub        dl, dl
    jmp        Label0x1585              ;Offset 0x1585
Label0x1577:                            ;Offset 0x1577
    jb         Label0x157f              ;Offset 0x157f
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
label0x157f:                            ;Offset 0x157f
    cmp        al, 0ah
    ja         Label0x1562              ;Offset 0x1562
    jb         Label0x15a7              ;Offset 0x15a7
Label0x1585:                            ;Offset 0x1585
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x1591              ;Offset 0x1591
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x1591:                            ;Offset 0x1591
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    sub        cx, cx
    call       Func0x763                ;Offset 0x763
    mov        ax, 0601h                ;Scroll up window by 1 line
    mov        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec        dx
    int        10h
    pop        cx
    ret
Label0x15a7:                            ;Offset 0x15a7
    cmp        al, 07h
    je         Label0x15b8              ;Offset 0x15b8
    cmp        al, 08h
    jne        Label0x1562              ;Offset 0x1562
    test       dl, dl
    je         Label0x15b7              ;Offset 0x15b7
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x15b7:                            ;Offset 0x15b7
    ret
Label0x15b8:                            ;Offset 0x15b8
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x1555 ENDP

Func0x15bc PROC NEAR                    ;Offset 0x15bc
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    sub        bh, bh
    cmp        al, 0dh
    jbe        Label0x15de              ;Offset 0x15de
Label0x15c9:                            ;Offset 0x15c9
    push       dx
    call       Func0x183f               ;Offset 0x183f
    pop        dx
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x15da              ;Offset 0x15da
    call       Func0x763                ;Offset 0x763
    ret
Label0x15da:                            ;Offset 0x15da
    sub        dl, dl
    jmp        Label0x15ec              ;Offset 0x15ec
Label0x15de:                            ;Offset 0x15de
    jb         Label0x15e6              ;Offset 0x15e6
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
Label0x15e6:                            ;Offset 0x15e6
    cmp        al, 0ah
    ja         Label0x15c9              ;Offset 0x15c9
    jb         Label0x160e              ;Offset 0x160e
Label0x15ec:                            ;Offset 0x15ec
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x15f8              ;Offset 0x15f8
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x15f8:                            ;Offset 0x15f8
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    sub        cx, cx
    call       Func0x763                ;Offset 0x763
    mov        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec        dx
    mov        ax, 0601h                ;Scroll up window by 1 line
    int        10h
    pop        cx
    ret
Label0x160e:                            ;Offset 0x160e
    cmp        al, 07h
    je         Label0x161f              ;Offset 0x161f
    cmp        al, 08h
    jne        Label0x15c9              ;Offset 0x15c9
    test       dl, dl
    je         Label0x161e              ;Offset 0x161e
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x161e:                            ;Offset 0x161e
    ret
Label0x161f:                            ;Offset 0x161f
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x15bc ENDP

Func0x1623 PROC NEAR                    ;Offset 0x1623
    call       GetA000SelectorInES      ;Offset 0x3f4c
    mov        dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    sub        bh, bh
    cmp        al, 0dh
    jbe        Label0x1645              ;Offset 0x1645
Label0x1630:                            ;Offset 0x1630
    push       dx
    call       Func0x1897               ;Offset 0x1897
    pop        dx
    inc        dl
    cmp        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jae        Label0x1641              ;Offset 0x1641
    call       Func0x763                ;Offset 0x763
    ret
Label0x1641:                            ;Offset 0x1641
    sub        dl, dl
    jmp        Label0x1653              ;Offset 0x1653
Label0x1645:                            ;Offset 0x1645
    jb         Label0x164d              ;Offset 0x164d
    sub        dl, dl
    call       Func0x763                ;Offset 0x763
    ret
Label0x164d:                            ;Offset 0x164d
    cmp        al, 0ah
    ja         Label0x1630              ;Offset 0x1630
    jb         Label0x1675              ;Offset 0x1675
Label0x1653:                            ;Offset 0x1653
    cmp        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jae        Label0x165f              ;Offset 0x165f
    inc        dh
    call       Func0x763                ;Offset 0x763
    ret
Label0x165f:                            ;Offset 0x165f
    push       cx
    mov        dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    sub        cx, cx
    call       Func0x763                ;Offset 0x763
    mov        dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    dec        dx
    mov        ax, 0601h                ;Scroll window up 1 line
    int        10h
    pop        cx
    ret
Label0x1675:                            ;Offset 0x1675
    cmp        al, 07h
    je         Label0x1686              ;Offset 0x1686
    cmp        al, 08h
    jne        Label0x1630              ;Offset 0x1630
    test       dl, dl
    je         Label0x1685              ;Offset 0x1685
    dec        dx
    call       Func0x763                ;Offset 0x763
Label0x1685:                            ;Offset 0x1685
    ret
Label0x1686:                            ;Offset 0x1686
    call       ShortBeep                ;Offset 0x3fd3
    ret
Func0x1623 ENDP

Func0x168a PROC NEAR                    ;Offset 0x168a
    push       bx
    push       cx
    push       dx
    push       ds
    push       ax
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        al, cl
    mul        byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mov        bh, dl
    shl        bh, 01h
    mov        dl, dh
    sub        dh, dh
    mul        dx
    add        al, bh
    adc        ah, 00h
    mov        di, ax
    mov        bh, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    lds        si, ds:[INT_43_Handler]  ;Offset 0x10c
    pop        ax
    test       al, 80h
    je         Label0x16c0              ;Offset 0x16c0
    and        al, 7fh
    call       GetBIOSDataSelectorInDS  ;Offset 0x3f40
    lds        si, ds:[INT_1F_Handler]  ;Offset 0x7c
Label0x16c0:                            ;Offset 0x16c0
    mul        cl
    add        si, ax
    cmp        bh, 06h
    je         Label0x170a              ;Offset 0x170a
    test       bl, 80h
    jne        Label0x16ec              ;Offset 0x16ec
    and        bl, 03h
Label0x16d1:                            ;Offset 0x16d1
    lodsb
    call       Func0x1742               ;Offset 0x1742
    stosw
    dec        di
    dec        di
    test       di, 2000h
    jne        Label0x16e4              ;Offset 0x16e4
    add        di, 2000h
    jmp        Label0x16e8              ;Offset 0x16e8
Label0x16e4:                            ;Offset 0x16e4
    sub        di, 1fb0h
Label0x16e8:                            ;Offset 0x16e8
    loop       Label0x16d1              ;Offset 0x16d1
    jmp        Label0x173d              ;Offset 0x173d
Label0x16ec:                            ;Offset 0x16ec
    and        bl, 03h
Label0x16ef:                            ;Offset 0x16ef
    lodsb
    call       Func0x1742               ;Offset 0x1742
    xor        word ptr es:[di], ax
    test       di, 2000h
    jne        Label0x1702              ;Offset 0x1702
    add        di, 2000h
    jmp        Label0x1706              ;Offset 0x1706
Label0x1702:                            ;Offset 0x1702
    sub        di, 1fb0h
Label0x1706:                            ;Offset 0x1706
    loop       Label0x16ef              ;Offset 0x16ef
    jmp        Label0x173d              ;Offset 0x173d
Label0x170a:                            ;Offset 0x170a
    shr        di, 01h
    test       bl, 80h
    jne        Label0x1727              ;Offset 0x1727
Label0x1711:                            ;Offset 0x1711
    movsb
    dec        di
    test       di, 2000h
    jne        Label0x171f              ;Offset 0x171f
    add        di, 2000h
    jmp        Label0x1723              ;Offset 0x1723
Label0x171f:                            ;Offset 0x171f
    sub        di, 1fb0h
Label0x1723:                            ;Offset 0x1723
    loop       Label0x1711              ;Offset 0x1711
    jmp        Label0x173d              ;Offset 0x173d
Label0x1727:                            ;Offset 0x1727
    lodsb
    xor        byte ptr es:[di], al
    test       di, 2000h
    jne        Label0x1737              ;Offset 0x1737
    add        di, 2000h
    jmp        Label0x173b              ;Offset 0x173b
Label0x1737:                            ;Offset 0x1737
    sub        di, 1fb0h
Label0x173b:                            ;Offset 0x173b
    loop       Label0x1727              ;Offset 0x1727
Label0x173d:                            ;Offset 0x173d
    pop        ds
    pop        dx
    pop        cx
    pop        bx
    ret
Func0x168a ENDP

Func0x1742 PROC NEAR                    ;Offset 0x1742
    push       cx
    push       dx
    sub        dx, dx
    mov        cx, 0008h
Label0x1749:                            ;Offset 0x1749
    rol        dx, 01h
    rol        dx, 01h
    rcl        al, 01h
    jae        Label0x1753              ;Offset 0x1753
    or         dl, bl
Label0x1753:                            ;Offset 0x1753
    loop       Label0x1749              ;Offset 0x1749
    mov        ax, dx
    xchg       al, ah
    pop        dx
    pop        cx
    ret
Func0x1742 ENDP

Func0x175c PROC NEAR                    ;Offset 0x175c
Label0x175c:                            ;Offset 0x175c
    add        di, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    dec        bh
    jne        Label0x175c              ;Offset 0x175c
    jmp        Func0x1776               ;Offset 0x1776
Func0x175c ENDP

Func0x1766 PROC NEAR                    ;Offset 0x1766
    push       bx
    push       cx
    push       dx
    push       bp
    push       ds
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    push       ax
    sub        di, di
    test       bh, bh
    jne        Func0x175c               ;Offset 0x175c
Func0x1766 ENDP
;continue!
Func0x1776 PROC NEAR
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        al, dh
    sub        dh, dh
    add        di, dx
    mul        cl
    mul        bp
    add        di, ax
    pop        ax
    mul        cl
    lds        si, ds:[INT_43_Handler]  ;Offset 0x10c
    add        si, ax
    mov        dx, VGA_SequenceIndex    ;Port 0x3c4
    mov        al, VGA_SEQIdx_MapMask   ;0x2
    call       ReadIndexedRegister      ;Offset 0x3f84
    mov        ah, VGA_SEQ2_MaskAll     ;0xf
    out        dx, ax
    mov        dl, VGA_GraphicsControllerIndex_lowbyte;0xce
    sub        al, al                   ;VGA_GCTLIdx_SetReset
    call       ReadIndexedRegister      ;Offset 0x3f84
    sub        ah, ah
    out        dx, ax
    inc        ax                       ;VGA_GCTLIdx_EnableSetReset
    call       ReadIndexedRegister      ;Offset 0x3f84
    mov        ah, bl
    not        ah
    out        dx, ax
    mov        al, VGA_GCTLIdx_DataRotate;0x3
    call       ReadIndexedRegister      ;Offset 0x3f84
    mov        ah, VGA_GCTL3_FuncSourceXORLatched;0x18
    test       bl, 80h
    jne        Label0x17bb              ;Offset 0x17bb
    sub        ah, ah
Label0x17bb:                            ;Offset 0x17bb
    out        dx, ax
    mov        al, VGA_GCTLIdx_BitMask  ;0x8
    call       ReadIndexedRegister      ;Offset 0x3f84
    mov        ah, 0ffh
    out        dx, ax
Label0x17c4:                            ;Offset 0x17c4
    lodsb
    xchg       byte ptr es:[di], al
    add        di, bp
    loop       Label0x17c4              ;Offset 0x17c4
    mov        dx, VGA_GraphicsControllerIndex;0x3ce
    sub        ax, ax                   ;VGA_GCTLIdx_SetReset 0
    out        dx, ax
    inc        ax                       ;VGA_GCTLIdx_EnableSetReset 0
    out        dx, ax
    mov        al, VGA_GCTLIdx_DataRotate;0x3 0x0
    out        dx, ax
    mov        ax, (0FFh SHL 8) OR VGA_GCTLIdx_BitMask;0xff08
    out        dx, ax
    pop        ds
    pop        bp
    pop        dx
    pop        cx
    pop        bx
    ret
Func0x1776 ENDP

Func0x17e1 PROC NEAR                    ;Offset 0x17e1
    push       bx
    push       cx
    push       dx
    push       bp
    push       ds
    mov        bp, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl        bp, 03h
    push       ax
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        al, dh
    sub        dh, dh
    mov        si, dx
    shl        si, 03h
    mul        cl
    mul        bp
    add        ax, si
    adc        dx, 0000h
    call       SetVGAReadWriteAperture64K;Offset 0x3c10
    mov        di, ax
    pop        ax
    mul        cl
    lds        si, ds:[INT_43_Handler]  ;Offset 0x10c
    add        si, ax
    sub        bp, 0008h
    mov        dx, cx
Label0x1817:                            ;Offset 0x1817
    lodsb
    mov        ah, al
    mov        cx, 0008h
Label0x181d:                            ;Offset 0x181d
    sub        al, al
    rcl        ah, 01h
    jae        Label0x1825              ;Offset 0x1825
    mov        al, bl
Label0x1825:                            ;Offset 0x1825
    stosb
    test       di, di
    jne        Label0x182d              ;Offset 0x182d
    call       MoveVGAReadWriteAperturesUp64K;Offset 0x3c4f
Label0x182d:                            ;Offset 0x182d
    loop       Label0x181d              ;Offset 0x181d
    add        di, bp
    jae        Label0x1836              ;Offset 0x1836
    call       MoveVGAReadWriteAperturesUp64K;Offset 0x3c4f
Label0x1836:                            ;Offset 0x1836
    dec        dx
    jne        Label0x1817              ;Offset 0x1817
    pop        ds
    pop        bp
    pop        dx
    pop        cx
    pop        bx
    ret
Func0x17e1 ENDP

Func0x183f PROC NEAR                    ;Offset 0x183f
    push       bx
    push       cx
    push       dx
    push       bp
    push       ds
    call       Func0x4141               ;Offset 0x4141
    push       ax
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        al, dh
    sub        dh, dh
    mov        si, dx
    shl        si, 04h
    mul        cl
    mul        bp
    add        ax, si
    adc        dx, 0000h
    mov        di, ax
    call       SetVGAReadWriteAperture64K;Offset 0x3c10
    pop        ax
    mul        cl
    lds        si, ds:[INT_43_Handler]  ;Offset 0x10c
    add        si, ax
    sub        bp, 0010h
    mov        bx, cx
Label0x1871:                            ;Offset 0x1871
    lodsb
    mov        ah, al
    mov        cx, 0008h
Label0x1877:                            ;Offset 0x1877
    cwd
    mov        word ptr es:[di], dx
    inc        di
    inc        di
    rol        ah, 01h
    loop       Label0x1877              ;Offset 0x1877
    add        di, bp
    jb         Label0x1892              ;Offset 0x1892
    cmp        di, bp
    je         Label0x1892              ;Offset 0x1892
Label0x1889:                            ;Offset 0x1889
    dec        bx
    jne        Label0x1871              ;Offset 0x1871
    pop        ds
    pop        bp
    pop        dx
    pop        cx
    pop        bx
    ret
Label0x1892:                            ;Offset 0x1892
    call       MoveVGAReadWriteAperturesUp64K;Offset 0x3c4f
    jmp        Label0x1889              ;Offset 0x1889
Func0x183f ENDP

Func0x1897 PROC NEAR                    ;Offset 0x1897
    push       bx
    push       cx
    push       dx
    push       bp
    push       ds
    push       ax
    call       Func0x4141               ;Offset 0x4141
    mov        cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov        al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call       Func0x417c               ;Offset 0x417c
    movzx      bx, al
    mov        al, dh
    sub        dh, dh
    mov        si, dx
    cmp        bl, 18h
    je         Label0x18bd              ;Offset 0x18bd
    shl        si, 05h
    jmp        Label0x18c4              ;Offset 0x18c4
Label0x18bd:                            ;Offset 0x18bd
    shl        si, 01h
    add        si, dx
    shl        si, 03h
Label0x18c4:                            ;Offset 0x18c4
    mul        cl
    mul        bp
    add        ax, si
    adc        dx, 0000h
    mov        di, ax
    call       SetVGAReadWriteAperture64K;Offset 0x3c10
    pop        ax
    mul        cl
    lds        si, ds:[INT_43_Handler]  ;Offset 0x10c
    add        si, ax
    sub        bp, bx
    mov        bh, cl
Label0x18df:                            ;Offset 0x18df
    lodsb
    mov        ah, al
    mov        cx, 0008h
Label0x18e5:                            ;Offset 0x18e5
    cwd
    mov        word ptr es:[di], dx
    inc        di
    inc        di
    mov        byte ptr es:[di], dl
    inc        di
    cmp        bl, 20h
    cmc
    adc        di, 0000h
    rol        ah, 01h
    loop       Label0x18e5              ;Offset 0x18e5
    add        di, bp
    jb         Label0x190c              ;Offset 0x190c
    cmp        di, bp
    je         Label0x190c              ;Offset 0x190c
Label0x1902:                            ;Offset 0x1902
    dec        bh
    jne        Label0x18df              ;Offset 0x18df
    pop        ds
    pop        bp
    pop        dx
    pop        cx
    pop        bx
    ret
Label0x190c:                            ;Offset 0x190c
    call       MoveVGAReadWriteAperturesUp64K;Offset 0x3c4f
    jmp        Label0x1902              ;Offset 0x1902
Func0x1897 ENDP

                        DB 000h

ReadLightPenPosition:                   ;Offset 0x1912
    sub   ax, ax
    pop   ds
    pop   si
    iret

;Offset 0x1917
Data0x1917              DW offset Func0x19b7;Offset 0x19b7
                        DW offset Func0x19b7;Offset 0x19b7
                        DW offset Func0x194d;Offset 0x194d
                        DW offset Func0x19c3;Offset 0x19c3
                        DW offset Func0x19c3;Offset 0x19c3
                        DW offset Func0x19c3;Offset 0x19c3
                        DW offset Func0x1a1e;Offset 0x1a1e
                        DW offset Func0x19b7;Offset 0x19b7
                        DW offset Func0x19b7;Offset 0x19b7
                        DW offset Func0x19b7;Offset 0x19b7

WriteGraphicsPixel:                     ;Offset 0x192b
    pushaw 
    push   es
    push   ax
    push   bx
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   FindModeData                 ;Offset 0x40d9
    mov    si, word ptr es:[bx + 04h]
    pop    bx
    pop    ax
    jb     Label0x1948                  ;Offset 0x1948
    and    si, 0ffh
    shl    si, 01h
    call   word ptr cs:[si + Data0x1917];Offset 0x1917
Label0x1948:                            ;Offset 0x1948
    pop    es
    popaw  
    pop    ds
    pop    si
    iret   

Func0x194d PROC NEAR                    ;Offset 0x194d
    call   GetB800SelectorInES          ;Offset 0x3f58
    sub    di, di
    mov    bl, al
    shr    dx, 01h
    jae    Label0x195b                  ;Offset 0x195b
    mov    di, 2000h
Label0x195b:                            ;Offset 0x195b
    mov    ax, 0050h
    mul    dx
    add    di, ax
    mov    ax, cx
    shr    ax, 01h
    shr    ax, 01h
    cmp    byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 6
    jne    Label0x1993                  ;Offset 0x1993
    shr    ax, 01h
    add    di, ax
    and    cl, 07h
    test   bl, 80h
    lahf
    and    bl, 01h
    ror    bl, 01h
    ror    bl, cl
    sahf
    jne    Label0x198f                  ;Offset 0x198f
    mov    al, 7fh
    ror    al, cl
    and    al, byte ptr es:[di]
    or     al, bl
    stosb
    ret
Label0x198f:                            ;Offset 0x198f
    xor    byte ptr es:[di], bl
    ret
Label0x1993:                            ;Offset 0x1993
    add    di, ax
    and    cl, 03h
    shl    cl, 01h
    test   bl, 80h
    lahf
    and    bl, 03h
    ror    bl, 02h
    ror    bl, cl
    sahf
    jne    Label0x19b4                  ;Offset 0x19b4
    mov    al, 3fh
    ror    al, cl
    and    al, byte ptr es:[di]
    or     al, bl
    stosb
    ret
Label0x19b4:                            ;Offset 0x19b4
    xor    byte ptr es:[di], bl
Func0x194d ENDP
;continue!
Func0x19b7 PROC NEAR                    ;Offset 0x19b7
    ret
Func0x19b7 ENDP

Func0x19b8 PROC NEAR                    ;Offset 0x19b8
    mov       al, bh
    cbw
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add       di, ax
    jmp       Func0x19da                ;Offset 0x19da
Func0x19b8 ENDP

Func0x19c3 PROC NEAR                    ;Offset 0x19c3
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    ja        Func0x1a1d                ;Offset 0x1a1d
    call      GetA000SelectorInES       ;Offset 0x3f4c
    mov       si, ax
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       dx
    mov       di, ax
    test      bh, bh
    jne       Func0x19b8                ;Offset 0x19b8
Func0x19c3 ENDP

Func0x19da PROC NEAR                    ;Offset 0x19da
    mov       ax, cx
    shr       ax, 03h
    add       di, ax
    and       cl, 07h
    mov       ah, 80h
    ror       ah, cl
    mov       al, VGA_GCTLIdx_BitMask   ;0x8
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    out       dx, ax
    mov       ax, VGA_GCTLIdx_EnableSetReset OR (0fh SHL 8);0xf01
    out       dx, ax
    mov       ax, si
    mov       ah, al
    sub       al, al                    ;VGA_GCTLIdx_SetReset
    out       dx, ax
    test      ah, 80h                   ;Unknown bit
    mov       ax, VGA_GCTLIdx_DataRotate OR (VGA_GCTL3_FuncSourceXORLatched SHL 8);0x1803
    jne       Label0x1a03               ;Offset 0x1a03
    sub       ah, ah                    ;VGA_GCTL3_FuncUnmodified
Label0x1a03:                            ;Offset 0x1a03
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, VGA_SEQIdx_MapMask OR (VGA_SEQ2_MaskAll SHL 8);0xf02
    out       dx, ax
    mov       al, 0ffh
    xchg      byte ptr es:[di], al
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;0xce
    sub       ax, ax                    ;VGA_GCTLIdx_SetReset
    out       dx, ax
    inc       ax                        ;VGA_GCTLIdx_EnableSetReset
    out       dx, ax
    inc       ax
    inc       ax                        ;VGA_GCTLIdx_DataRotate
    out       dx, ax
    mov       ax, VGA_GCTLIdx_BitMask OR (0ffh SHL 8);0xff08
    out       dx, ax
Func0x19da ENDP
;continue!
Func0x1a1d PROC NEAR                    ;Offset 1a1d
    ret
Func0x1a1d ENDP

Func0x1a1e PROC NEAR                    ;Offset 0x1a1e
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    ja        Label0x1a39               ;Offset 0x1a39
    call      GetA000SelectorInES       ;Offset 0x3f4c
    mov       bh, al
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       ax, 03h
    mul       dx
    add       ax, cx
    mov       di, ax
    mov       al, bh
    stosb
Label0x1a39:                            ;Offset 0x1a39
    ret
Func0x1a1e ENDP

Data0x1a3a              DW offset Func0x1ab1;Offset 0x1ab1
                        DW offset Func0x1ab1;Offset 0x1ab1
                        DW offset Func0x1a72;Offset 0x1a72
                        DW offset Func0x1abd;Offset 0x1abd
                        DW offset Func0x1abd;Offset 0x1abd
                        DW offset Func0x1abd;Offset 0x1abd
                        DW offset Func0x1b1a;Offset 0x1b1a
                        DW offset Func0x1ab1;Offset 0x1ab1
                        DW offset Func0x1ab1;Offset 0x1ab1
                        DW offset Func0x1ab1;Offset 0x1ab1

ReadGraphicsPixel:                      ;Offset 0x1a4e
    push      cx
    push      dx
    push      es
    push      bx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    mov       si, word ptr es:[bx + 04h]
    pop       bx
    jb        Label0x1a6c               ;Offset 0x1a6c
    and       si, 00ffh
    shl       si, 01h
    call      word ptr cs:[si + Data0x1a3a];Offset 0x1a3a
    mov       ah, 0dh
Label0x1a6c:                            ;Offset 0x1a6c
    pop       es
    pop       dx
    pop       cx
    pop       ds
    pop       si
    iret

Func0x1a72 PROC NEAR                    ;Offset 0x1a72
    push      bx
    sub       si, si
    shr       dx, 01h
    jae       Label0x1a7c               ;Offset 0x1a7c
    mov       si, 2000h
Label0x1a7c:                            ;Offset 0x1a7c
    mov       ax, 0050h
    mul       dx
    add       si, ax
    mov       ax, cx
    shr       ax, 01h
    shr       ax, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_640x200_BW_Graphics;Offset 0x449 0x6
    call      GetB800SelectorInDS       ;Offset 0x3f64
    jne       Label0x1aa2               ;Offset 0x1aa2
    shr       ax, 01h
    add       si, ax
    and       cl, 07h
    lodsb
    inc       cx
    rol       al, cl
    and       al, 01h
    pop       bx
    ret
Label0x1aa2:                            ;Offset 0x1aa2
    add       si, ax
    and       cl, 03h
    shl       cl, 01h
    lodsb
    inc       cx
    inc       cx
    rol       al, cl
    and       al, 03h
    pop       bx
Func0x1a72 ENDP
;continue!
Func0x1ab1 PROC NEAR                    ;Offset 0x1ab1
    ret
Func0x1ab1 ENDP

Func0x1ab2 PROC NEAR                    ;Offset 0x1ab2
    mov       al, bh
    cbw
    mul       word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    add       si, ax
    jmp       Func0x1ad0                ;Offset 0x1ad0
Func0x1ab2 ENDP

Func0x1abd PROC NEAR                    ;Offset 0x1abd
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    ja        Func0x1b19                ;Offset 0x1b19
    push      bx
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    mul       dx
    mov       si, ax
    test      bh, bh
    jne       Func0x1ab2                ;Offset 0x1ab2
Func0x1abd ENDP
;continue!
Func0x1ad0 PROC NEAR                    ;Offset 0x1ad0
    mov       ax, cx
    shr       ax, 03h
    add       si, ax
    and       cl, 07h
    mov       bh, 80h
    shr       bh, cl
    call      GetA000SelectorInDS       ;Offset 0x3f5e
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, VGA_GCTLIdx_ReadMapSelect OR (VGA_GCTL4_Map0 SHL 8);0x4
    sub       bl, bl
    out       dx, ax
    mov       bl, byte ptr [si]
    inc       ah                        ;VGA_GCTL4_Map1
    and       bl, bh
    ror       bl, 01h
    out       dx, ax
    mov       ch, byte ptr [si]
    inc       ah                        ;VGA_GCTL4_Map2
    and       ch, bh
    or        bl, ch
    ror       bl, 01h
    out       dx, ax
    mov       ch, byte ptr [si]
    inc       ah                        ;VGA_GCTL4_Map3
    and       ch, bh
    or        bl, ch
    ror       bl, 01h
    out       dx, ax
    mov       ch, byte ptr [si]
    inc       ah                        ;?
    and       ch, bh
    or        bl, ch
    sub       cl, 04h
    rol       bl, cl
    mov       al, bl
    pop       bx
Func0x1ad0 ENDP
;continue!
Func0x1b19 PROC NEAR                    ;Offset 0x1b19
    ret
Func0x1b19 ENDP

Func0x1b1a PROC NEAR                    ;Offset 0x1b1a
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    ja        Label0x1b31               ;Offset 0x1b31
    mov       ax, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl       ax, 03h
    mul       dx
    add       ax, cx
    call      GetA000SelectorInDS       ;Offset 0x3f5e
    mov       si, ax
    lodsb
Label0x1b31:                            ;Offset 0x1b31
    ret
Func0x1b1a ENDP

;Offset 0x1b32
Data0x1b32              DB 012h, 014h, 016h

;Offset 0x1b35
Data0x1b35              DB 013h, 015h, 017h

;Offset 0x1b38
Data0x1b38              DB 002h, 004h, 006h

;Offset 0x1b3b
Data0x1b3b              DB 003h, 005h, 007h

;Offset 0x1b3e
Data0x1b3e              DW offset Data0x1b38;Offset 0x1b38
                        DW offset Data0x1b32;Offset 0x1b32
                        DW offset Data0x1b3b;Offset 0x1b3b
                        DW offset Data0x1b35;Offset 0x1b35

SetBorderColorOrPalette:                ;Offset 0x1b46
    pushaw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x3f99                ;Offset 0x3f99
    je        Label0x1bc4               ;Offset 0x1bc4
    cmp       bh, 01h
    je        Label0x1b68               ;Offset 0x1b68
    ja        Label0x1bc4               ;Offset 0x1bc4
    mov       al, bl
    mov       ah, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       ax, 0e010h
    or        al, ah
    and       bl, 0fh
    or        al, bl
    jmp       Label0x1b76               ;Offset 0x1b76
Label0x1b68:                            ;Offset 0x1b68
    mov       al, bl
    shl       al, 05h
    mov       ah, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    and       ax, 0df20h
    or        al, ah
Label0x1b76:                            ;Offset 0x1b76
    mov       byte ptr ds:[BDA_CGAColorPaletteMaskSetting], al;Offset 0x466
    call      WaitUntilVSyncActive      ;Offset 0x4057
    push      dx
    mov       bh, al
    and       bx, 0f00h
    cmp       bh, 08h
    jb        Label0x1b8b               ;Offset 0x1b8b
    add       bh, 08h
Label0x1b8b:                            ;Offset 0x1b8b
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x3faf                ;Offset 0x3faf
    cli
    je        Label0x1b9c               ;Offset 0x1b9c
    mov       al, VGA_ATTR_Palette0     ;0x0
    out       dx, al
    mov       al, bh
    out       dx, al
Label0x1b9c:                            ;Offset 0x1b9c
    mov       al, VGA_ATTR_OverscanColor;0x11
    out       dx, al
    mov       al, bh
    out       dx, al
    sti
    pop       dx
    je        Label0x1bbf               ;Offset 0x1bbf
    mov       bl, byte ptr ds:[BDA_CGAColorPaletteMaskSetting];Offset 0x466
    push      cs
    pop       ds
    and       bx, 30h
    shr       bx, 03h
    mov       si, word ptr cs:[bx + Data0x1b3e];Offset 0x1b3e
    mov       bl, 01h
    mov       cx, 0003h
    call      Func0x1eb5                ;Offset 0x1eb5
Label0x1bbf:                            ;Offset 0x1bbf
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
Label0x1bc4:                            ;Offset 0x1bc4
    popaw
    pop       ds
    pop       si
    iret

;Offset 0x1bc8
RAMDACCallTable         DW SetSinglePaletteRegister;Offset 0x1c1a       ;0x00
                        DW SetBorderColor;Offset 0x1c4d                 ;0x01
                        DW SetAllPaletteRegisters;Offset 0x1c75         ;0x02
                        DW ToggleIntensityBlinkingBit;Offset 0x1ca2     ;0x03
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW GetIndividualPaletteRegister;Offset 0x1cc6   ;0x07
                        DW GetBorderColor;Offset 0x1ce1                 ;0x08
                        DW GetAllPaletteRegisters;Offset 0x1cfd         ;0x09
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW SetIndividualDACRegister;Offset 0x1d2e       ;0x10
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW SetBlockOfDACRegisters;Offset 0x1d5e         ;0x12
                        DW SelectVideoDACColorPage;Offset 0x1d88        ;0x13
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW GetIndividualDACRegister;Offset 0x1dcc       ;0x15
                        DW EmptyFunction;Offset 0x1c4c                  ;
                        DW GetBlockOfDACRegisters;Offset 0x1de4         ;0x17
                        DW SetPELMask   ;Offset 0x1e01                  ;0x18
                        DW GetPELMask   ;Offset 0x1e0a                  ;0x19
                        DW GetVideoDACColorPageState;Offset 0x1e13      ;0x1a
                        DW PerformGrayScaleSumming;Offset 0x1e43        ;0x1b

RAMDACFunctions:                        ;Offset 0x1c00
    cmp       al, 1ch
    jae       Label0x1c17               ;Offset 0x1c17
    test      byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Inactive;Offset 0x487 0x8
    jne       Label0x1c17               ;Offset 0x1c17
    push      ax
    cbw
    mov       si, ax
    shl       si, 01h
    call      word ptr cs:[si + RAMDACCallTable];Offset 0x1bc8
    pop       ax
Label0x1c17:                            ;Offset 0x1c17
    pop       ds
    pop       si
    iret

SetSinglePaletteRegister PROC NEAR      ;Offset 0x1c1a
    push      dx
    push      ds
    lds       si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    lds       si, [si + BDA_VPCB_DynamicParamSaveArea];Offset 0x4
    mov       ax, ds
    or        ax, si
    je        Label0x1c39               ;Offset 0x1c39
    mov       al, bl
    sub       ah, ah
    cmp       al, 10h
    je        Label0x1c39               ;Offset 0x1c39
    cmp       al, 11h
    ja        Label0x1c39               ;Offset 0x1c39
    add       si, ax
    mov       byte ptr [si], bh
Label0x1c39:                            ;Offset 0x1c39
    pop       ds
    call      WaitUntilVSyncActive      ;Offset 0x4057
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, bl
    out       dx, al
    mov       al, bh
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
SetSinglePaletteRegister ENDP
;continue!
EmptyFunction PROC NEAR                 ;Offset 0x1c4c
    ret
EmptyFunction ENDP

SetBorderColor PROC NEAR                ;Offset 0x1c4d
    push      dx
    push      ds
    lds       si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    lds       si, [si + BDA_VPCB_DynamicParamSaveArea];Offset 0x4
    mov       ax, ds
    or        ax, si
    je        Label0x1c61               ;Offset 0x1c61
    add       si, 0011h
    mov       byte ptr [si], bh
Label0x1c61:                            ;Offset 0x1c61
    pop       ds
    call      WaitUntilVSyncActive      ;Offset 0x4057
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_OverscanColor OR VGA_ATTR_PaletteAddressSource;0x31
    out       dx, al
    mov       al, bh
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    ret
SetBorderColor ENDP

SetAllPaletteRegisters PROC NEAR        ;Offset 0x1c75
    push      cx
    push      dx
    push      di
    push      es
    mov       si, dx
    call      WaitUntilVSyncActive      ;Offset 0x4057
    push      es
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    pop       ds
    les       di, es:[di + BDA_VPCB_DynamicParamSaveArea];Offset 0x4
    mov       ax, es
    or        ax, di
    je        Label0x1c95               ;Offset 0x1c95
    mov       cx, 0011h
    push      si
    rep movsb
    pop       si
Label0x1c95:                            ;Offset 0x1c95
    mov       cx, 0011h
    sub       ah, ah
    call      SetAttributePalette       ;Offset 0x1e7f
    pop       es
    pop       di
    pop       dx
    pop       cx
    ret
SetAllPaletteRegisters ENDP

ToggleIntensityBlinkingBit PROC NEAR    ;Offset 0x1ca2
    push      dx
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_ModeControl OR VGA_ATTR_PaletteAddressSource;0x30
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, bl
    and       al, 01h
    shl       al, 03h                   ;Blinking / background color bit
    and       ah, NOT VGA_MC_Blinking   ;0xf7
    or        al, ah
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    ret
ToggleIntensityBlinkingBit ENDP

GetIndividualPaletteRegister PROC NEAR  ;Offset 0x1cc6
    push      dx
    cli
    call      ResetAttributeIndex       ;Offset 0x4135
    mov       dx, VGA_AttributeControllerIndex;Port 0x3c0
    mov       al, bl
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       bh, ah
    mov       al, ah
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    call      ResetAttributeIndex       ;Offset 0x4135
    sti
    pop       dx
    ret
GetIndividualPaletteRegister ENDP

GetBorderColor PROC NEAR                ;Offset 0x1ce1
    push      dx
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_OverscanColor;0x11
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       bh, ah
    mov       al, ah
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       dx
    ret
GetBorderColor ENDP

GetAllPaletteRegisters PROC NEAR        ;Offset 0x1cfd
    push      cx
    push      dx
    push      di
    mov       di, dx
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    sub       ah, ah
    mov       cx, 0010h
Label0x1d12:                            ;Offset 0x1d12
    mov       al, ah
    out       dx, al
    inc       dx                        ;Port 0x3c1 VGA_AttributeControllerDataR
    in        al, dx
    stosb
    dec       dx
    out       dx, al
    inc       ah
    loop      Label0x1d12               ;Offset 0x1d12
    mov       al, VGA_ATTR_OverscanColor;0x11
    out       dx, al
    inc       dx
    in        al, dx
    stosb
    dec       dx
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    pop       di
    pop       dx
    pop       cx
    ret
GetAllPaletteRegisters ENDP

SetIndividualDACRegister PROC NEAR      ;Offset 0x1d2e
    push      cx
    push      dx
    push      dx
    call      WaitUntilVSyncActive      ;Offset 0x4057
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    mov       al, bl
    out       dx, al
    pop       dx
    call      Func0x1d41                ;Offset 0x1d41
    pop       dx
    pop       cx
    ret
SetIndividualDACRegister ENDP

Func0x1d41 PROC NEAR                    ;Offset 0x1d41
    push      ax
    push      dx
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489 0x6
    je        Label0x1d4d               ;Offset 0x1d4d
    call      Func0x1e5a                ;Offset 0x1e5a
Label0x1d4d:                            ;Offset 0x1d4d
    mov       al, dh
    mov       dx, VGA_RAMDACData        ;Port 0x3c9
    cli
    out       dx, al
    mov       al, ch
    out       dx, al
    mov       al, cl
    out       dx, al
    sti
    pop       dx
    pop       ax
    ret
Func0x1d41 ENDP

SetBlockOfDACRegisters PROC NEAR        ;Offset 0x1d5e
    push      bx
    push      cx
    push      dx
    mov       si, dx
    call      WaitUntilVSyncActive      ;Offset 0x4057
    call      Func0x1d6d                ;Offset 0x1d6d
    pop       dx
    pop       cx
    pop       bx
    ret
SetBlockOfDACRegisters ENDP

Func0x1d6d PROC NEAR                    ;Offset 0x1d6d
    mov       al, bl
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    out       dx, al
    mov       bx, cx
Label0x1d75:                            ;Offset 0x1d75
    mov       dh, byte ptr es:[si]
    inc       si
    mov       ch, byte ptr es:[si]
    inc       si
    mov       cl, byte ptr es:[si]
    inc       si
    call      Func0x1d41                ;Offset 0x1d41
    dec       bx
    jne       Label0x1d75               ;Offset 0x1d75
    ret
Func0x1d6d ENDP

SelectVideoDACColorPage PROC NEAR       ;Offset 0x1d88
    push      bx
    push      dx
    cmp       bl, 01h
    ja        Label0x1dc8               ;Offset 0x1dc8
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, VGA_ATTR_ModeControl  ;0x10
    call      ReadIndexedRegister       ;Offset 0x3f84
    test      bl, bl
    jne       Label0x1db4               ;Offset 0x1db4
    mov       al, ah
    and       al, 7fh
    and       bh, 01h
    ror       bh, 01h
    or        al, bh
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    jmp       Label0x1dc8               ;Offset 0x1dc8
Label0x1db4:                            ;Offset 0x1db4
    mov       al, ah
    out       dx, al
    mov       al, VGA_ATTR_ColorSelect  ;0x14
    out       dx, al
    test      ah, 80h
    jne       Label0x1dc2               ;Offset 0x1dc2
    shl       bh, 02h
Label0x1dc2:                            ;Offset 0x1dc2
    mov       al, bh
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
Label0x1dc8:                            ;Offset 0x1dc8
    sti
    pop       dx
    pop       bx
    ret
SelectVideoDACColorPage ENDP

GetIndividualDACRegister PROC NEAR      ;Offset 0x1dcc
    push      dx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    inc       dx
    inc       dx
    cli
    in        al, dx
    mov       ah, al
    in        al, dx
    mov       ch, al
    in        al, dx
    mov       cl, al
    sti
    pop       dx
    mov       dh, ah
    ret
GetIndividualDACRegister ENDP

GetBlockOfDACRegisters PROC NEAR        ;Offset 0x1de4
    push      cx
    push      dx
    push      di
    mov       di, dx
    mov       dx, VGA_DACReadIndex      ;Port 0x3c7
    mov       al, bl
    out       dx, al
    inc       dx
    inc       dx
    mov       ax, cx
Label0x1df3:                            ;Offset 0x1df3
    cli
    in        al, dx
    stosb
    in        al, dx
    stosb
    in        al, dx
    stosb
    sti
    loop      Label0x1df3               ;Offset 0x1df3
    pop       di
    pop       dx
    pop       cx
    ret
GetBlockOfDACRegisters ENDP

SetPELMask PROC NEAR                    ;Offset 0x1e01
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    mov       al, bl
    out       dx, al
    pop       dx
    ret
SetPELMask ENDP

GetPELMask PROC NEAR                    ;Offset 0x1e0a
    push      dx
    mov       dx, VGA_DACMask           ;Port 0x3c6
    in        al, dx
    mov       bl, al
    pop       dx
    ret
GetPELMask ENDP

GetVideoDACColorPageState PROC NEAR     ;Offset 0x1e13
    push      dx
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;0xc0
    mov       al, VGA_ATTR_ModeControl  ;0x10
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, ah
    out       dx, al
    mov       bl, ah
    mov       al, VGA_ATTR_ColorSelect  ;0x14
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, ah
    out       dx, al
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    mov       bh, ah
    and       bl, 80h
    rol       bl, 01h
    jb        Label0x1e41               ;Offset 0x1e41
    shr       bh, 02h
Label0x1e41:                            ;Offset 0x1e41
    pop       dx
    ret
GetVideoDACColorPageState ENDP

PerformGrayScaleSumming PROC NEAR       ;Offset 0x1e43
    push      bx
    push      cx
    push      dx
    jcxz      Label0x1e56               ;Offset 0x1e56
Label0x1e48:                            ;Offset 0x1e48
    push      cx
    call      GetIndividualDACRegister  ;Offset 0x1dcc
    call      Func0x1e5a                ;Offset 0x1e5a
    call      SetIndividualDACRegister  ;Offset 0x1d2e
    inc       bx
    pop       cx
    loop      Label0x1e48               ;Offset 0x1e48
Label0x1e56:                            ;Offset 0x1e56
    pop       dx
    pop       cx
    pop       bx
    ret
PerformGrayScaleSumming ENDP

Func0x1e5a PROC NEAR                    ;Offset 0x1e5a
    push      bx
    mov       al, 1eh
    mul       dh
    mov       bx, ax
    mov       al, 3bh
    mul       ch
    add       bx, ax
    mov       al, 0bh
    mul       cl
    add       ax, bx
    sub       dx, dx
    mov       bx, 0064h
    add       ax, 0032h
    div       bx
    mov       dh, al
    mov       cl, al
    mov       ch, al
    pop       bx
    ret
Func0x1e5a ENDP

SetAttributePalette PROC NEAR           ;Offset 0x1e7f
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0h
Label0x1e83:                            ;Offset 0x1e83
    cmp       ah, 10h
    jae       Label0x1e96               ;Offset 0x1e96
    mov       al, ah
    out       dx, al
    lodsb
    out       dx, al
    inc       ah
    loop      Label0x1e83               ;Offset 0x1e83
Label0x1e91:                            ;Offset 0x1e91
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    ret
Label0x1e96:                            ;Offset 0x1e96
    mov       al, VGA_ATTR_OverscanColor;0x11
    out       dx, al
    lodsb
    out       dx, al
    jmp       Label0x1e91               ;Offset 0x1e91
SetAttributePalette ENDP

Func0x1e9d PROC NEAR                    ;Offset 0x1e9d
    pushaw
    mov       dx, VGA_DACWriteIndex     ;Port 0x3c8
    sub       al, al
    out       dx, al
    inc       dx
    mov       cx, 0100h
Label0x1ea8:                            ;Offset 0x1ea8
    mov       al, ah
    out       dx, al
    mov       al, bl
    out       dx, al
    mov       al, bh
    out       dx, al
    loop      Label0x1ea8               ;Offset 0x1ea8
    popaw
    ret
Func0x1e9d ENDP

Func0x1eb5 PROC NEAR                    ;Offset 0x1eb5
    push      ax
    push      dx
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
Label0x1ebb:                            ;Offset 0x1ebb
    mov       al, bl
    out       dx, al
    lodsb
    out       dx, al
    inc       bx
    loop      Label0x1ebb               ;Offset 0x1ebb
    sti
    pop       dx
    pop       ax
    ret
Func0x1eb5 ENDP

                        DB 000h

Func0x1ec8 PROC NEAR                    ;Offset 0x1ec8
    push      cs
    pop       es
    mov       di, offset Font8x8        ;Offset 0x4b3a
    mov       bh, 08h
    mov       al, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    cmp       al, 09h
    jbe       Label0x1ee4               ;Offset 0x1ee4
    mov       bh, 0eh
    mov       di, offset Font8x14       ;Offset 0x533a
    cmp       al, 0eh
    jbe       Label0x1ee4               ;Offset 0x1ee4
    mov       bh, 10h
    mov       di, offset Font8x16       ;Offset 0x613a
Label0x1ee4:                            ;Offset 0x1ee4
    ret
Func0x1ec8 ENDP

Func0x1ee5 PROC NEAR                    ;Offset 0x1ee5
    push      dx
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, VGA_GCTLIdx_GraphicMode;0x5
    out       dx, ax
    mov       al, VGA_GCTLIdx_Miscellaneous;0x6
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, VGA_GCTL6_GfxMode     ;0x1
    or        ah, VGA_GCTL6_Mem_A0000_AFFFF;0x4
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, VGA_SEQIdx_MapMask OR (VGA_SEQ2_Mask2 SHL 8);0x402
    out       dx, ax
    mov       ax, VGA_SEQIdx_MemoryMode OR ((VGA_SEQ4_MemSize256k OR VGA_SEQ4_CPUAnyMap) SHL 8);0x604
    out       dx, ax
    pop       dx
    ret
Func0x1ee5 ENDP

Func0x1f05 PROC NEAR                    ;Offset 0x1f05
    push      dx
    mov       dx, VGA_MiscellaneousRead ;Port 0x3cc
    mov       bx, VGA_GCTLIdx_Miscellaneous OR ((VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B0000_B7FFF) SHL 8);0xa06
    in        al, dx
    test      al, VGA_Misc_IOAddressSel_CGA;0x1
    je        Label0x1f14               ;Offset 0x1f14
    mov       bx, VGA_GCTLIdx_Miscellaneous OR ((VGA_GCTL6_ChainEven OR VGA_GCTL6_Mem_B8000_BFFFF) SHL 8);0xe06
Label0x1f14:                            ;Offset 0x1f14
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       ax, bx
    out       dx, ax
    mov       ax, VGA_GCTLIdx_GraphicMode OR (VGA_GCTL5_OddEven SHL 8);0x1005
    out       dx, ax
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, VGA_SEQIdx_MapMask OR ((VGA_SEQ2_Mask0 OR VGA_SEQ2_Mask1) SHL 8);0x302
    out       dx, ax
    mov       ax, VGA_SEQIdx_MemoryMode OR (VGA_SEQ4_MemSize256k SHL 8);0x204
    out       dx, ax
    pop       dx
    ret
Func0x1f05 ENDP

Func0x1f29 PROC NEAR                    ;Offset 0x1f29
    push      cs
    pop       es
    mov       di, offset Font8x14Patch  ;Offset 0x713a
    mov       bh, 0eh
    cmp       word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485
    jbe       Label0x1f3c               ;Offset 0x1f3c
    mov       di, Offset Font8x16Patch  ;Offset 0x7267
    mov       bh, 10h
Label0x1f3c:
    ret
Func0x1f29 ENDP

Func0x1f3d PROC NEAR                    ;Offset 0x1f3d
    push      bx
    push      ds
    push      es
    mov       si, di
    push      es
    pop       ds
    call      Func0x2267                ;Offset 0x2267
    jcxz      Label0x1f5f               ;Offset 0x1f5f
    mov       dx, cx
    mov       bl, bh
    sub       bh, bh
    push      bp
    mov       bp, 0020h
    sub       bp, bx
Label0x1f55:                            ;Offset 0x1f55
    mov       cx, bx
    rep movsb
    add       di, bp
    dec       dx
    jne       Label0x1f55               ;Offset 0x1f55
    pop       bp
Label0x1f5f:                            ;Offset 0x1f5f
    pop       es
    pop       ds
    pop       bx
    ret
Func0x1f3d ENDP

Func0x1f63 PROC NEAR                    ;Offset 0x1f63
    push      bx
    push      ds
    push      es
    mov       si, di
    push      es
    pop       ds
    push      bx
    sub       bh, bh
    shl       bx, 01h
    call      GetA000SelectorInES       ;Offset 0x3f4c
    mov       di, word ptr cs:[bx + Data0x2257];Offset 0x2257
    pop       bx
    mov       dl, 20h
    mov       bl, bh
    sub       bh, bh
Label0x1f7e:                            ;Offset 0x1f7e
    lodsb
    test      al, al
    je        Label0x1f8f               ;Offset 0x1f8f
    push      di
    mul       dl
    add       di, ax
    mov       cx, bx
    rep movsb
    pop       di
    jmp       Label0x1f7e               ;Offset 0x1f7e
Label0x1f8f:                            ;Offset 0x1f8f
    pop       es
    pop       ds
    pop       bx
    ret       
Func0x1f63 ENDP

;Offset 0x1f93
Data0x1f93              DB 00h
                        DW offset LoadUserSpecifiedPatterns;Offset 0x2045   Int 10/AX=1100h - VIDEO - TEXT-MODE CHARGEN - LOAD USER-SPECIFIED PATTERNS (PS,EGA,VGA)
                        DB 01h
                        DW offset LoadRomMonochromePatterns;Offset 0x2053   Int 10/AX=1101h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM MONOCHROME PATTERNS (PS,EGA,VGA)
                        DB 02h
                        DW offset LoadRom8x8DoubleDotPatterns;Offset 0x2069 Int 10/AX=1102h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM 8x8 DBL-DOT PATTERNS (PS,EGA,VGA)
                        DB 03h
                        DW offset SetBlockSpecifier;Offset 0x207f   Int 10/AX=1103h - VIDEO - TEXT-MODE CHARGEN - SET BLOCK SPECIFIER (PS,EGA,VGA)
                        DB 04h
                        DW offset LoadRom8x16CharacterSet;Offset 0x2088 Int 10/AX=1104h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM 8x16 CHARACTER SET (VGA)
                        DB 10h
                        DW offset LoadUserSpecifiedPatterns2;Offset 0x209e  Int 10/AX=1110h - VIDEO - TEXT-MODE CHARGEN - LOAD USER-SPECIFIED PATTERNS (PS,EGA,VGA)
                        DB 11h
                        DW offset LoadRomMonochromePatterns2;Offset 0x20af  Int 10/AX=1111h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM MONOCHROME PATTERNS (PS,EGA,VGA)
                        DB 12h
                        DW offset LoadRom8x8DoubleDotPatterns2;Offset 0x20c8    Int 10/AX=1112h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM 8x8 DBL-DOT PATTERNS (PS,EGA,VGA)
                        DB 14h
                        DW offset LoadRom8x16CharacterSet2;Offset 0x20e1    Int 10/AX=1114h - VIDEO - TEXT-MODE CHARGEN - LOAD ROM 8x16 CHARACTER SET (VGA)
                        DB 20h
                        DW offset SetUser8x8GraphicsCharacters;Offset 0x20fa    Int 10/AX=1120h - VIDEO - GRAPH-MODE CHARGEN - SET USER 8x8 GRAPHICS CHARS (PS,EGA,VGA)
                        DB 21h
                        DW offset SetUserGraphicsCharacters;Offset 0x2109   Int 10/AX=1121h - VIDEO - GRAPH-MODE CHARGEN - SET USER GRAPHICS CHARACTERS (PS,EGA,VGA)
                        DB 22h
                        DW offset SetRom8x14GraphicsCharacters;Offset 0x2126    Int 10/AX=1122h - VIDEO - GRAPH-MODE CHARGEN - SET ROM 8x14 GRAPHICS CHARS (PS,EGA,VGA)
                        DB 23h
                        DW offset SetRom8x8DoubleDotCharacters;Offset 0x214a    Int 10/AX=1123h - VIDEO - GRAPH-MODE CHARGEN - SET ROM 8x8 DOUBLE-DOT CHARS (PS,EGA,VGA)
                        DB 24h
                        DW offset Load8x16GraphicsCharacters;Offset 0x216e  Int 10/AX=1124h - VIDEO - GRAPH-MODE CHARGEN - LOAD 8x16 GRAPHICS CHARS (VGA,MCGA)

TextFunctions:                          ;Offset 0x1FBD
    cmp    al, 30h
    je     TextFunction30               ;Offset 0x2023
    push   ax
    push   bx
    push   cx
    push   dx
    push   di
    push   es
    mov    di, 0eh
    mov    si, offset Data0x1f93        ;Offset 0x1f93
    mov    ah, al
Label0x1fcf:                            ;Offset 0x1fcf
    lodsb  byte ptr cs:[si]
    cmp    al, ah
    je     Label0x1fdc                  ;Offset 0x1fdc
    inc    si
    inc    si
    dec    di
    jne    Label0x1fcf                  ;Offset 0x1fcf
    jmp    Label0x1fe2                  ;Offset 0x1fe2
Label0x1fdc:                            ;Offset 0x1fdc
    lodsw  word ptr cs:[si]
    mov    si, ax
    call   si
Label0x1fe2:                            ;Offset 0x1fe2
    pop    es
    pop    di
    pop    dx
    pop    cx
    pop    bx
    pop    ax
    pop    ds
    pop    si
    iret

;Offset 0x1feb
Data0x1feb              FARPointer <INT_1F_HandlerOfs, 00000h>;Offset 0x007c
Data0x1fef              FARPointer <INT_43_HandlerOfs, 00000h>;Offset 0x010c
Data0x1ff3              FARPointer <offset Data0x2013, 0c000h>;Offset 0x2013
Data0x1ff7              FARPointer <offset Data0x200b, 0c000h>;Offset 0x200b
Data0x1ffb              FARPointer <offset Data0x200f, 0c000h>;Offset 0x200f
Data0x1fff              FARPointer <offset Data0x201b, 0c000h>;Offset 0x201b
Data0x2003              FARPointer <offset Data0x2017, 0c000h>;Offset 0x2017
Data0x2007              FARPointer <offset Data0x201f, 0c000h>;Offset 0x201f

;Offset 0x200b
Data0x200b              FARPointer <offset Font8x8, 0c000h>;Offset 0x4b3a

;Offset 0x200f
Data0x200f              FARPointer <offset GraphicsFont8x8, 0c000h>;Offset 0x4f3a

;Offset 0x2013
Data0x2013              FARPointer <offset Font8x14, 0c000h>;Offset 0x533a

;Offset 0x2017
Data0x2017              FARPointer <offset Font8x16, 0c000h>;Offset 0x613a

;Offset 0x201b
Data0x201b              FARPointer <offset Font8x14Patch, 0c000h>;Offset 0x713a

;Offset 0x201f
Data0x201f              FARPointer <offset Font8x16Patch, 0c000h>;Offset 0x7267

TextFunction30:                         ;Offset 2023
    push bx
    mov  cx, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    mov  dl, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    cmp  bh, 07h
    ja   Label0x2041                    ;Offset 0x2041
    mov  bl, bh
    sub  bh, bh
    shl  bx, 02h
    les  bp, cs:[bx + Data0x1feb]       ;Offset 0x1feb
    les  bp, es:[bp]
Label0x2041:
    pop  bx
    pop  ds
    pop  si
    iret

LoadUserSpecifiedPatterns PROC NEAR     ;Offset 0x2045
    jcxz Label0x2052                    ;Offset 0x2052
    mov  di, bp
    call Func0x1ee5                     ;Offset 0x1ee5
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x1f05                     ;Offset 0x1f05
Label0x2052:                            ;Offset 0x2052
    ret
LoadUserSpecifiedPatterns ENDP

LoadRomMonochromePatterns PROC NEAR     ;Offset 0x2053
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 0eh
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x2013]            ;Offset 0x2013
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRomMonochromePatterns ENDP

LoadRom8x8DoubleDotPatterns PROC NEAR   ;Offset 0x2069
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 08h
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x200b]            ;Offset 0x200b
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRom8x8DoubleDotPatterns ENDP

SetBlockSpecifier PROC NEAR             ;Offset 0x207f
    mov  dx, VGA_SequenceIndex          ;Port 0x3c4
    mov  al, VGA_SEQIdx_CharacterMapSelect;0x3
    mov  ah, bl
    out  dx, ax
    ret
SetBlockSpecifier ENDP

LoadRom8x16CharacterSet PROC NEAR       ;Offset 0x2088
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 10h
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x2017]            ;Offset 0x2017
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRom8x16CharacterSet ENDP

LoadUserSpecifiedPatterns2 PROC NEAR    ;Offset 0x209e
    jcxz Label0x20ae                    ;Offset 0x20ae
    mov  di, bp
    call Func0x1ee5                     ;Offset 0x1ee5
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x2192                     ;Offset 0x2192
    call Func0x1f05                     ;Offset 0x1f05
Label0x20ae:                            ;Offset 0x20ae
    ret
LoadUserSpecifiedPatterns2 ENDP

LoadRomMonochromePatterns2 PROC NEAR    ;Offset 0x20af
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 0eh
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x2013]            ;Offset 0x2013
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x2192                     ;Offset 0x2192
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRomMonochromePatterns2 ENDP

LoadRom8x8DoubleDotPatterns2 PROC NEAR  ;Offset 0x20c8
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 08h
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x200b]            ;Offset 0x200b
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x2192                     ;Offset 0x2192
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRom8x8DoubleDotPatterns2 ENDP

LoadRom8x16CharacterSet2 PROC NEAR      ;Offset 0x20e1
    call Func0x1ee5                     ;Offset 0x1ee5
    mov  bh, 10h
    mov  cx, 0100h
    sub  dx, dx
    les  di, cs:[Data0x2017]            ;Offset 0x2017
    call Func0x1f3d                     ;Offset 0x1f3d
    call Func0x2192                     ;Offset 0x2192
    call Func0x1f05                     ;Offset 0x1f05
    ret
LoadRom8x16CharacterSet2 ENDP

SetUser8x8GraphicsCharacters PROC NEAR  ;Offset 0x20fa
    cli
    mov  word ptr ds:[INT_1F_HandlerOfs], bp;Offset 0x7c
    mov  word ptr ds:[INT_1F_HandlerSeg], es;Offset 0x7e
    sti
    ret
SetUser8x8GraphicsCharacters ENDP

;Offset 0x2105
Data0x2105              DB 000h, 00Eh, 019h, 02Bh

SetUserGraphicsCharacters PROC NEAR     ;Offset 0x2109
    mov  word ptr ds:[INT_43_HandlerOfs], bp;Offset 0x10c
    mov  word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
    test bl, bl
    je   Label0x2120                    ;Offset 0x2120
    sub  bh, bh
    mov  dl, byte ptr cs:[bx + Data0x2105];Offset 0x2105
Label0x2120:                            ;Offset 0x2120
    dec  dx
    mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
    ret
SetUserGraphicsCharacters ENDP

SetRom8x14GraphicsCharacters PROC NEAR  ;Offset 0x2126
    les  di, cs:[Data0x2013]            ;Offset 0x2013
    mov  word ptr ds:[INT_43_HandlerOfs], di;Offset 0x10c
    mov  word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485
    test bl, bl
    je   Label0x2144                    ;Offset 0x2144
    sub  bh, bh
    mov  dl, byte ptr cs:[bx + Data0x2105];Offset 0x2105
Label0x2144:                            ;Offset 0x2144
    dec  dx
    mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
    ret
SetRom8x14GraphicsCharacters ENDP

SetRom8x8DoubleDotCharacters PROC NEAR  ;Offset 0x214a
    les  di, cs:[Data0x200b]            ;Offset 0x200b
    mov  word ptr ds:[INT_43_HandlerOfs], di;Offset 0x10c
    mov  word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485
    test bl, bl
    je   Label0x2168                    ;Offset 0x2168
    sub  bh, bh
    mov  dl, byte ptr cs:[bx + Data0x2105];Offset 0x2105
Label0x2168:                            ;Offset 0x2168
    dec  dx
    mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
    ret
SetRom8x8DoubleDotCharacters ENDP

Load8x16GraphicsCharacters PROC NEAR    ;Offset 0x216e
    les  di, cs:[Data0x2017]            ;Offset 0x2017
    mov  word ptr ds:[INT_43_HandlerOfs], di;Offset 0x10c
    mov  word ptr ds:[INT_43_HandlerSeg], es;Offset 0x10e
    mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], 10h;Offset 0x485
    test bl, bl
    je   Label0x218c                    ;Offset 0x218c
    sub  bh, bh
    mov  dl, byte ptr cs:[bx + Data0x2105];Offset 0x2105
Label0x218c:                            ;Offset 0x218c
    dec  dx
    mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
    ret  
Load8x16GraphicsCharacters ENDP

Func0x2192 PROC NEAR                    ;Offset 0x2192
    call Func0x21e6                     ;Offset 0x21e6
    div  bh
    dec  ax
    mov  byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    mov  byte ptr ds:[BDA_PointHeightOfCharacterMatrix], bh;Offset 0x485
    inc  ax
    mul  bh
    dec  ax
    call Func0x221d                     ;Offset 0x221d
    cmp  byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449 0x7
    jne  Label0x21b2                    ;Offset 0x21b2
    mov  ah, bh
    mov  al, VGA_CRTCIdx_UnderlineLoc   ;0x14
    out  dx, ax
Label0x21b2:                            ;Offset 0x21b2
    mov  al, VGA_CRTCIdx_MaxScanLine    ;0x9
    call ReadIndexedRegister            ;Offset 0x3f84
    and  ah, NOT VGA_CRTC9_MaxScanMask  ;0xe0
    dec  bh
    or   ah, bh
    out  dx, ax
    inc  ax
    cmp  bh, 07h
    jbe  Label0x21c7                    ;Offset 0x21c7
    dec  bh
Label0x21c7:                            ;Offset 0x21c7
    mov  ah, bh
    dec  ah
    out  dx, ax
    mov  byte ptr ds:[BDA_CursorStartScanLine], ah;Offset 0x461
    inc  ax
    mov  ah, bh
    out  dx, ax
    mov  byte ptr ds:[BDA_CursorEndScanLine], ah;Offset 0x460
    mov  al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    inc  ax
    mul  byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    shl  ax, 01h
    mov  word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    ret
Func0x2192 ENDP

Func0x21e6 PROC NEAR                    ;Offset 0x21e6
    push di
    push es
    call Func0x35a7                     ;Offset 0x35a7
    mov  al, byte ptr es:[di + 1ch]
    mov  ah, byte ptr es:[di + 11h]
    mov  ch, ah
    and  ah, 02h
    and  ch, 40h
    shr  ah, 01h
    rol  ch, 03h
    or   ah, ch
    inc  ax
    test byte ptr es:[di + 21h], 04h
    je   Label0x220c                    ;Offset 0x220c
    shl  ax, 01h
Label0x220c:                            ;Offset 0x220c
    test byte ptr es:[di + 13h], 80h
    je   Label0x2216                    ;Offset 0x2216
    inc  ax
    shr  ax, 01h
Label0x2216:                            ;Offset 0x2216
    mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    pop  es
    pop  di
    ret
Func0x21e6 ENDP

Func0x221d PROC NEAR                    ;Offset 0x221d
    mov  cx, ax
    mov  al, VGA_CRTCIdx_MaxScanLine    ;0x9
    call ReadIndexedRegister            ;Offset 0x3f84
    test ah, VGA_CRTC9_DoubleScan       ;0x80
    je   Label0x222d                    ;Offset 0x222d
    inc  cx
    shl  cx, 01h
    dec  cx
Label0x222d:
    mov  ah, cl
    mov  al, VGA_CRTCIdx_VertDispEnableEnd;0x12
    out  dx, ax
    mov  al, VGA_CRTCIdx_VertRetraceEnd ;0x11
    call ReadIndexedRegister            ;Offset 0x3f84
    push ax
    and  ah, NOT VGA_CRTC9_DoubleScan   ;0x7f
    out  dx, ax
    mov  al, VGA_CRTCIdx_Overflow       ;0x7
    call ReadIndexedRegister            ;Offset 0x3f84
    and  ah, 0bdh
    mov  cl, ch
    and  cx, 0201h
    shl  cl, 01h
    ror  ch, 03h
    or   ah, cl
    or   ah, ch
    out  dx, ax
    pop  ax
    out  dx, ax
    ret
Func0x221d ENDP

;Offset 0x2257
Data0x2257              DW 00000h
                        DW 04000h
                        DW 08000h
                        DW 0c000h
                        DW 02000h
                        DW 06000h
                        DW 0a000h
                        DW 0e000h

Func0x2267 PROC NEAR                    ;Offset 0x2267
    push bx
    push dx
    and  bx, 07h
    shl  bx, 01h
    call GetA000SelectorInES            ;Offset 0x3f4c
    mov  di, word ptr cs:[bx + Data0x2257];Offset 0x2257
    shl  dx, 05h
    add  di, dx
    pop  dx
    pop  bx
    ret
Func0x2267 ENDP

;Offset 0x227e
Data0x227e              DB 010h
                        DW offset AlternateGetEGAInfo;Offset 0x22b6     Int 10/AH=12h/BL=10h - VIDEO - ALTERNATE FUNCTION SELECT (PS, EGA, VGA, MCGA) - GET EGA INFO
                        DB 020h
                        DW offset AlternatePrintScreen;Offset 0x22d2        Int 10/AH=12h/BL=20h - VIDEO - ALTERNATE FUNCTION SELECT (PS,EGA,VGA,MCGA) - ALTERNATE PRTSC
                        DB 030h
                        DW offset AlternateSelectVerticalResolution;Offset 0x22e5       Int 10/AH=12h/BL=30h - VIDEO - ALTERNATE FUNCTION SELECT (VGA) - SELECT VERTICAL RESOLUTION
                        DB 031h
                        DW offset AlternatePaletteLoading;Offset 0x2309     Int 10/AH=12h/BL=31h - VIDEO - ALTERNATE FUNCTION SELECT (VGA, MCGA) - PALETTE LOADING
                        DB 032h
                        DW offset AlternateVideoAddressing;Offset 0x231c        Int 10/AH=12h/BL=32h - VIDEO - ALTERNATE FUNCTION SELECT (VGA, MCGA) - VIDEO ADDRESSING
                        DB 033h
                        DW offset AlernateGrayScaleSumming;Offset 0x232d        Int 10/AH=12h/BL=33h - VIDEO - ALTERNATE FUNCTION SELECT (VGA, MCGA) - GRAY-SCALE SUMMING
                        DB 034h
                        DW offset AlternateCursorEmulation;Offset 0x2341        Int 10/AH=12h/BL=34h - VIDEO - ALTERNATE FUNCTION SELECT (VGA) - CURSOR EMULATION
                        DB 035h
                        DW offset AlternateDisplaySwitchInterface;Offset 0x2351     Int 10/AH=12h/BL=35h - VIDEO - ALTERNATE FUNCTION SELECT (PS) - DISPLAY-SWITCH INTERFACE
                        DB 036h
                        DW offset AlternateVideoRefreshControl;Offset 0x2352        Int 10/AH=12h/BL=36h - VIDEO - ALTERNATE FUNCTION SELECT (PS, VGA) - VIDEO REFRESH CONTROL

AlternateFunctions:                     ;Offset 0x2299
    mov       si, offset Data0x227e     ;Offset 0x227e
Label0x229c:                            ;Offset 0x229c
    cmp       byte ptr cs:[si], bl
    je        Label0x22af               ;Offset 0x22af
    add       si, 0003h
    cmp       si, offset AlternateFunctions;Offset 0x2299
    jb        Label0x229c               ;Offset 0x229c
    call      Func0x3d10                ;Offset 0x3d10
    jmp       Label0x22b3               ;Offset 0x22b3
Label0x22af:                            ;Offset 0x22af
    call      word ptr cs:[si + 01h]
Label0x22b3:                            ;Offset 0x22b3
    pop       ds
    pop       si
    iret      

AlternateGetEGAInfo PROC NEAR           ;Offset 0x22b6
    mov       bx, 0103h
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexD_lowbyte;Offset 0x463 0xd4
    jne       Label0x22c2               ;Offset 0x22c2
    sub       bh, bh
Label0x22c2:                            ;Offset 0x22c2
    mov       cl, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    mov       ch, cl
    and       cx, BDA_EFBS_AdapterTypeMask OR (BDA_EFBS_FeatureConnectorMask SHL 8);0xf00f
    shr       ch, 04h
    mov       al, bl
    ret
AlternateGetEGAInfo ENDP

AlternatePrintScreen PROC NEAR          ;Offset 0x22d2
    cli
    mov       word ptr ds:[INT_5_HandlerOfs], offset AlternatePrintScreenHandler;Offset 0x14 Offset 0x2365
    mov       word ptr ds:[INT_5_HandlerSeg], cs;Offset 0x16
    sti
    ret
AlternatePrintScreen ENDP

;Offset 0x22df
Data0x22df              DW 00880h
                        DW 00900h
                        DW 00910h

AlternateSelectVerticalResolution PROC NEAR;Offset 0x22e5
    cmp    al, 02h
    ja     Label0x2308                  ;Offset 0x2308
    cbw
    mov    si, ax
    shl    si, 01h
    mov    ax, word ptr cs:[si + Data0x22df];Offset 0x22df
    and    byte ptr ds:[BDA_VideoDisplayDataArea], NOT (BDA_VDDA_LineMode400 OR BDA_VDDA_LineMode200);Offset 0x489 0x6f
    or     byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    and    byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488 0xf0
    or     byte ptr ds:[BDA_EGAFeatureBitSwitches], ah;Offset 0x488
    mov    ax, 1212h
Label0x2308:                            ;Offset 0x2308
    ret
AlternateSelectVerticalResolution ENDP

AlternatePaletteLoading PROC NEAR       ;Offset 0x2309
    cmp    al, 01h
    ja     Label0x231b                  ;Offset 0x231b
    shl    al, 03h
    and    byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_PaletteLoadingEnabled;Offset 0x489 0xf7
    or     byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    mov    al, 12h
Label0x231b:                            ;Offset 0x231b
    ret
AlternatePaletteLoading ENDP

AlternateVideoAddressing PROC NEAR      ;Offset 0x231c
    cmp    al, 01h
    ja     Label0x232c                  ;Offset 0x232c
    je     Label0x2327                  ;Offset 0x2327
    call   EnableVideoSubsystem         ;Offset 0x352b
    jmp    Label0x232a                  ;Offset 0x232a
Label0x2327:                            ;Offset 0x2327
    call   DisableVideoSubsystem        ;Offset 0x3536
Label0x232a:                            ;Offset 0x232a
    mov    al, 12h
Label0x232c:                            ;Offset 0x232c
    ret
AlternateVideoAddressing ENDP

AlernateGrayScaleSumming PROC NEAR      ;Offset 0x232d
    cmp    al, 01h
    ja     Label0x2340                  ;Offset 0x2340
    shl    al, 01h
    xor    al, 02h
    and    byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_GrayScale;Offset 0x489 0xfd
    or     byte ptr ds:[BDA_VideoDisplayDataArea], al;Offset 0x489
    mov    al, 12h
Label0x2340:                            ;Offset 0x2340
    ret
AlernateGrayScaleSumming ENDP

AlternateCursorEmulation PROC NEAR      ;Offset 0x2341
    cmp    al, 01h
    ja     Label0x2350                  ;Offset 0x2350
    and    byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_CursorEmulationEnabled;Offset 0x487 0xfe
    or     byte ptr ds:[BDA_VideoModeOptions], al;Offset 0x487
    mov    al, 12h
Label0x2350:                            ;Offset 0x2350
    ret
AlternateCursorEmulation ENDP

AlternateDisplaySwitchInterface PROC NEAR;Offset 0x2351
    ret
AlternateDisplaySwitchInterface ENDP

AlternateVideoRefreshControl PROC NEAR  ;Offset 0x2352
    push   dx
    cmp    al, 01h
    ja     Label0x2363                  ;Offset 0x2363
    je     Label0x235e                  ;Offset 0x235e
    call   TurnScreenOn                 ;Offset 0x3f77
    jmp    Label0x2361                  ;Offset 0x2361
Label0x235e:                            ;Offset 0x235e
    call   TurnScreenOff                ;Offset 0x3f6a
Label0x2361:                            ;Offset 0x2361
    mov    al, 12h
Label0x2363:                            ;Offset 0x2363
    pop    dx
    ret    
AlternateVideoRefreshControl ENDP

AlternatePrintScreenHandler:
    sti
    pushaw
    push   ds
    call   GetBIOSDataSelectorInDS      ;Offset 0x3f40
    mov    bl, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    sub    bh, bh
    shl    bx, 01h
    push   word ptr [bx + DBA_CursorPositionPage0];Offset 0x450
    mov    bh, byte ptr ds:[BDA_ActiveDisplayNumber];Offset 0x462
    sub    dx, dx
Label0x237d:                            ;Offset 0x237d
    call   Func0x763                    ;Offset 0x763
    mov    ah, 08h                      ;Read character and attribute at cursor position
    int    10h
    call   PrintCharacter               ;Offset 0x23ad
    inc    dx
    cmp    dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb     Label0x237d                  ;Offset 0x237d
    call   PrintCrLf                    ;Offset 0x23a2
    sub    dl, dl
    inc    dh
    cmp    dh, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    jbe    Label0x237d                  ;Offset 0x237d
    pop    dx
    call   Func0x763                    ;Offset 0x763
    pop    ds
    popaw
    iret

PrintCrLf PROC NEAR                     ;Offset 0x23a2
    mov    al, 0dh
    call   PrintCharacter               ;Offset 0x23ad
    mov    al, 0ah
    call   PrintCharacter               ;Offset 0x23ad
    ret
PrintCrLf ENDP

PrintCharacter PROC NEAR                ;Offset 0x23ad
    push   ax
    push   dx
    sub    dx, dx
    mov    ah, dl
    int    17h
    pop    dx
    pop    ax
    ret
PrintCharacter ENDP

;Offset 0x23b8
WriteStringCalltable    DW offset WriteStringBasic;Offset 0x23e8
                        DW offset WriteStringBasicUpdateCursor;Offset 0x246a
                        DW offset WriteStringWithAttributes;Offset 0x24de
                        DW offset WriteStringWithAttributesUpdateCursor;Offset 0x2565

WriteString:                            ;Offset 0x23c0
    pushaw
    jcxz   Label0x23e4                  ;Offset 0x23e4
    cmp    al, 03h
    ja     Label0x23e4                  ;Offset 0x23e4
    cbw
    mov    si, ax
    shl    si, 01h
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    push   bx
    push   es
    call   FindModeData                 ;Offset 0x40d9
    mov    al, byte ptr es:[bx + 04h]
    pop    es
    pop    bx
    jb     Label0x23e4                  ;Offset 0x23e4
    cbw
    shl    ax, 01h
    call   word ptr cs:[si + WriteStringCalltable];Offset 0x23b8
Label0x23e4:                            ;Offset 0x23e4
    popaw
    pop    ds
    pop    si
    iret

WriteStringBasic PROC NEAR              ;Offset 0x23e8
    mov       si, ax
    push      bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       ax, word ptr [bx + DBA_CursorPositionPage0];Offset 0x450
    pop       bx
    push      ax
Label0x23f7:                            ;Offset 0x23f7
    call      Func0x763                 ;Offset 0x763
    mov       al, byte ptr es:[bp]
    inc       bp
    push      bx
    push      cx
    push      si
    push      es
    cmp       al, 0dh
    ja        Label0x242a               ;Offset 0x242a
    je        Label0x2417               ;Offset 0x2417
    cmp       al, 0ah
    ja        Label0x242a               ;Offset 0x242a
    je        Label0x2417               ;Offset 0x2417
    cmp       al, 08h
    ja        Label0x242a               ;Offset 0x242a
    cmp       al, 07h
    jb        Label0x242a               ;Offset 0x242a
Label0x2417:                            ;Offset 0x2417
    push      bx
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x245f               ;Offset 0x245f
Label0x242a:                            ;Offset 0x242a
    mov       cx, 0001h
    push      bx
    push      dx
    push      si
    call      word ptr cs:[si + WriteCharacterCalltable];Offset 0x1206
    pop       si
    pop       dx
    pop       bx
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb        Label0x245f               ;Offset 0x245f
    push      bx
    mov       al, 0dh
    push      bx
    push      si
    push      es
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       es
    pop       si
    pop       bx
    mov       al, 0ah
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
Label0x245f:                            ;Offset 0x245f
    pop       es
    pop       si
    pop       cx
    pop       bx
    loop      Label0x23f7               ;Offset 0x23f7
    pop       dx
    call      Func0x763                 ;Offset 0x763
    ret
WriteStringBasic ENDP

WriteStringBasicUpdateCursor PROC NEAR  ;Offset 0x246a
    mov       si, ax
Label0x246c:                            ;Offset 0x246c
    call      Func0x763                 ;Offset 0x763
    mov       al, byte ptr es:[bp]
    inc       bp
    push      bx
    push      cx
    push      si
    push      es
    cmp       al, 0dh
    ja        Label0x249f               ;Offset 0x249f
    je        Label0x248c               ;Offset 0x248c
    cmp       al, 0ah
    ja        Label0x249f               ;Offset 0x249f
    je        Label0x248c               ;Offset 0x248c
    cmp       al, 08h
    ja        Label0x249f               ;Offset 0x249f
    cmp       al, 07h
    jb        Label0x249f               ;Offset 0x249f
Label0x248c:                            ;Offset 0x248c
    push      bx
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x24d4               ;Offset 0x24d4
Label0x249f:                            ;Offset 0x249f
    mov       cx, 0001h
    push      bx
    push      dx
    push      si
    call      word ptr cs:[si + WriteCharacterCalltable];Offset 0x1206
    pop       si
    pop       dx
    pop       bx
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb        Label0x24d4               ;Offset 0x24d4
    push      bx
    mov       al, 0dh
    push      bx
    push      si
    push      es
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       es
    pop       si
    pop       bx
    mov       al, 0ah
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
Label0x24d4:                            ;Offset 0x24d4
    pop       es
    pop       si
    pop       cx
    pop       bx
    loop      Label0x246c               ;Offset 0x246c
    call      Func0x763                 ;Offset 0x763
    ret       
WriteStringBasicUpdateCursor ENDP

WriteStringWithAttributes PROC NEAR     ;Offset 0x24de
    mov       si, ax
    push      bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       ax, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    pop       bx
    push      ax
Label0x24ed:                            ;Offset 0x24ed
    call      Func0x763                 ;Offset 0x763
    mov       al, byte ptr es:[bp]
    inc       bp
    push      bx
    push      cx
    push      si
    push      es
    cmp       al, 0dh
    ja        Label0x2520               ;Offset 0x2520
    je        Label0x250d               ;Offset 0x250d
    cmp       al, 0ah
    ja        Label0x2520               ;Offset 0x2520
    je        Label0x250d               ;Offset 0x250d
    cmp       al, 08h
    ja        Label0x2520               ;Offset 0x2520
    cmp       al, 07h
    jb        Label0x2520               ;Offset 0x2520
Label0x250d:                            ;Offset 0x250d
    push      bx
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x255a               ;Offset 0x255a
Label0x2520:                            ;Offset 0x2520
    mov       bl, byte ptr es:[bp]
    inc       bp
    mov       cx, 0001h
    push      bx
    push      dx
    push      si
    call      word ptr cs:[si + WriteCharacterCalltable];Offset 0x1206
    pop       si
    pop       dx
    pop       bx
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb        Label0x255a               ;Offset 0x255a
    push      bx
    mov       al, 0dh
    push      bx
    push      si
    push      es
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       es
    pop       si
    pop       bx
    mov       al, 0ah
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
Label0x255a:                            ;Offset 0x255a
    pop       es
    pop       si
    pop       cx
    pop       bx
    loop      Label0x24ed               ;Offset 0x24ed
    pop       dx
    call      Func0x763                 ;Offset 0x763
    ret       
WriteStringWithAttributes ENDP

WriteStringWithAttributesUpdateCursor PROC NEAR;Offset 0x2565
    mov       si, ax
Label0x2567:                            ;Offset 0x2567
    call      Func0x763                 ;Offset 0x763
    mov       al, byte ptr es:[bp]
    inc       bp
    push      bx
    push      cx
    push      si
    push      es
    cmp       al, 0dh
    ja        Label0x259a               ;Offset 0x259a
    je        Label0x2587               ;Offset 0x2587
    cmp       al, 0ah
    ja        Label0x259a               ;Offset 0x259a
    je        Label0x2587               ;Offset 0x2587
    cmp       al, 08h
    ja        Label0x259a               ;Offset 0x259a
    cmp       al, 07h
    jb        Label0x259a               ;Offset 0x259a
Label0x2587:                            ;Offset 0x2587
    push      bx
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
    jmp       Label0x25d4               ;Offset 0x25d4
Label0x259a:                            ;Offset 0x259a
    mov       bl, byte ptr es:[bp]
    inc       bp
    mov       cx, 0001h
    push      bx
    push      dx
    push      si
    call      word ptr cs:[si + WriteCharacterCalltable];Offset 0x1206
    pop       si
    pop       dx
    pop       bx
    inc       dx
    cmp       dl, byte ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a
    jb        Label0x25d4               ;Offset 0x25d4
    push      bx
    mov       al, 0dh
    push      bx
    push      si
    push      es
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       es
    pop       si
    pop       bx
    mov       al, 0ah
    call      word ptr cs:[si + TeletypeCalltable];Offset 0x1363
    pop       bx
    mov       bl, bh
    sub       bh, bh
    shl       bx, 01h
    mov       dx, word ptr ds:[bx + DBA_CursorPositionPage0];Offset 0x450
Label0x25d4:                            ;Offset 0x25d4
    pop       es
    pop       si
    pop       cx
    pop       bx
    loop      Label0x2567               ;Offset 0x2567
    call      Func0x763                 ;Offset 0x763
    ret
WriteStringWithAttributesUpdateCursor ENDP

DisplayCombinationCodeFunctions:        ;Offset 0x25DE
    cmp       al, 01h
    ja        Label0x261b               ;Offset 0x261b
    mov       ah, al
    je        Label0x25ec               ;Offset 0x25ec
    mov       bl, byte ptr ds:[BDA_DisplayCombinationCodeTableIndex];Offset 0x48a
    mov       bh, al
Label0x25ec:                            ;Offset 0x25ec
    mov       al, byte ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    lds       si, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    lds       si, [si + BDA_VPCB_SecondarySavePointerTbl];Offset 0x10
    lds       si, [si + BDA_SSP_DisplayCombinationCode];Offset 0x2
    je        Label0x261e               ;Offset 0x261e
    shl       bx, 01h
    mov       bx, word ptr [bx + si + 04h]
    xchg      bl, bh
    test      bh, bh
    je        Label0x2618               ;Offset 0x2618
    cmp       al, VGA_CRTControllerIndexD_lowbyte;Port 0x3d4
    jne       Label0x2611               ;Offset 0x2611
    test      bl, 01h
    je        Label0x2618               ;Offset 0x2618
    jmp       Label0x2616               ;Offset 0x2616
Label0x2611:                            ;Offset 0x2611
    test      bl, 01h
    jne       Label0x2618               ;Offset 0x2618
Label0x2616:                            ;Offset 0x2616
    xchg      bl, bh
Label0x2618:                            ;Offset 0x2618
    mov       ax, 1a1ah
Label0x261b:                            ;Offset 0x261b
    pop       ds
    pop       si
    iret
Label0x261e:                            ;Offset 0x261e
    push      cx
    sub       ch, ch
    mov       cl, byte ptr [si]
    add       si, 04h
    add       si, cx
    add       si, cx
    dec       si
    dec       si
    std
Label0x262d:                            ;Offset 0x262d
    lodsw
    cmp       ax, bx
    je        Label0x263c               ;Offset 0x263c
    xchg      al, ah
    cmp       ax, bx
    je        Label0x263c               ;Offset 0x263c
    loop      Label0x262d               ;Offset 0x262d
    jmp       Label0x2644               ;Offset 0x2644
Label0x263c:                            ;Offset 0x263c
    dec       cx
    call      GetBIOSDataSelectorInDS   ;Offset 0x3f40
    mov       byte ptr ds:[BDA_DisplayCombinationCodeTableIndex], cl;Offset 0x48a
Label0x2644:                            ;Offset 0x2644
    pop       cx
    jmp       Label0x2618               ;Offset 0x2618

FunctionalityAndStateInfo:              ;Offset 0x2647
    test      bx, bx
    jne       Label0x26a2               ;Offset 0x26a2
    push      bx
    push      cx
    push      dx
    push      di
    mov       ax, offset StaticFunctionalityTable ;Offset 0x77c5
    stosw
    mov       ax, cs
    stosw
    mov       si, BDA_DisplayMode       ;Offset 0x449
    mov       cx, 001eh
    rep movsb
    mov       al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
    inc       ax
    stosb
    mov       ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    stosw
    mov       ax, 1a00h                 ;Get Display Combination Code
    int       10h
    mov       ax, bx
    stosw
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x26a5                ;Offset 0x26a5
    stosw                               ;Number of colors supported in current mode
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x26c5                ;Offset 0x26c5
    stosb                               ;Number of pages supported in current mode
    call      Func0x26d3                ;Offset 0x26d3
    stosb                               ;Number of active scanlines
    call      Func0x26f6                ;Offset 0x26f6
    stosw                               ;character clocks?
    call      Func0x270a                ;Offset 0x270a
    stosb                               ;misc flags?
    sub       ax, ax
    stosw                               ;non-vga and reserved
    stosb                               ;reserved
    mov       al, 03h
    stosb                               ;video memory available (3 = 256k)
    call      Func0x2729                ;Offset 0x2729
    stosb                               ;save pointer state flags
    mov       cx, 000dh
    sub       ax, ax
    rep stosb                           ;display info and reserved
    pop       di
    pop       dx
    pop       cx
    pop       bx
    mov       ax, 001bh
Label0x26a2:                            ;Offset 0x26a2
    pop       ds
    pop       si
    iret

Func0x26a5 PROC NEAR                    ;Offset 0x26a5
    push      es
    call      FindModeData              ;Offset 0x40d9
    mov       ax, 0000h
    jb        Label0x26c3               ;Offset 0x26c3
    cmp       byte ptr ds:[BDA_VideoBaseIOPort], VGA_CRTControllerIndexB_lowbyte;Offset 0x463 0xb4
    je        Label0x26c3               ;Offset 0x26c3
    mov       cl, byte ptr es:[bx + 06h]
    mov       ax, 0001h
    shl       ax, cl
    test      ax, ax
    jne       Label0x26c3               ;Offset 0x26c3
    dec       ax
Label0x26c3:                            ;Offset 0x26c3
    pop       es
    ret
Func0x26a5 ENDP

Func0x26c5 PROC NEAR                    ;Offset 0x26c5
    push      es
    call      FindModeData              ;Offset 0x40d9
    mov       al, 01h
    jb        Label0x26d1               ;Offset 0x26d1
    mov       al, byte ptr es:[bx + 05h]
Label0x26d1:                            ;Offset 0x26d1
    pop       es
    ret
Func0x26c5 ENDP

Func0x26d3 PROC NEAR                    ;Offset 0x26d3
    sub       dx, dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    je        Label0x26f3               ;Offset 0x26f3
    call      Func0x21e6                ;Offset 0x21e6
    sub       dx, dx
    cmp       ax, 00c8h                 ;200
    je        Label0x26f3               ;Offset 0x26f3
    inc       dx
    cmp       ax, 015eh                 ;350
    je        Label0x26f3               ;Offset 0x26f3
    inc       dx
    cmp       ax, 0190h                 ;400
    je        Label0x26f3               ;Offset 0x26f3
    inc       dx
Label0x26f3:                            ;Offset 0x26f3
    mov       ax, dx
    ret
Func0x26d3 ENDP

Func0x26f6 PROC NEAR                    ;Offset 0x26f6
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, ah
    and       ax, 1003h
    shr       ah, 02h
    or        al, ah
    cbw
    ret
Func0x26f6 ENDP

Func0x270a PROC NEAR                    ;Offset 0x270a
    mov       bl, 10h
    call      GetIndividualPaletteRegister;Offset 0x1cc6
    and       bh, 08h
    shl       bh, 02h
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    mov       ah, byte ptr ds:[BDA_VideoDisplayDataArea];Offset 0x489
    and       ax, 0f01h
    shl       al, 04h
    or        al, ah
    or        al, bh
    xor       al, 10h
    ret
Func0x270a ENDP

Func0x2729 PROC NEAR                    ;Offset 0x2729
    push      es
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       al, VGA_SEQIdx_CharacterMapSelect;0x3
    call      ReadIndexedRegister       ;Offset 0x3f84
    sub       al, al
    test      ah, ah
    je        Label0x2739               ;Offset 0x2739
    inc       ax
Label0x2739:                            ;Offset 0x2739
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       bx, es:[bx + BDA_VPCB_DynamicParamSaveArea];0x4
    mov       dx, es
    or        dx, bx
    je        Label0x2749               ;Offset 0x2749
    or        al, 02h
Label0x2749:                            ;Offset 0x2749
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       bx, es:[bx + BDA_VPCB_AlphaNumCharSetOverride];0x8
    mov       dx, es
    or        dx, bx
    je        Label0x2759               ;Offset 0x2759
    or        al, 04h
Label0x2759:                            ;Offset 0x2759
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       bx, es:[bx + BDA_VPCB_GrahicsCharSetOverride];0xc
    mov       dx, es
    or        dx, bx
    je        Label0x2769               ;Offset 0x2769
    or        al, 08h
Label0x2769:                            ;Offset 0x2769
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       bx, es:[bx + BDA_VPCB_SecondarySavePointerTbl];0x10
    les       bx, es:[bx + BDA_SSP_UserPaletteProfileTbl];0xa
    mov       dx, es
    or        dx, bx
    je        Label0x277d               ;Offset 0x277d
    or        al, 10h
Label0x277d:                            ;Offset 0x277d
    les       bx, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       bx, es:[bx + 10h]         ;0x10 custom table
    les       bx, es:[bx + 02h]         ;0x2 DCC override active
    mov       dx, es
    mov       bx, cs
    cmp       dx, bx
    je        Label0x2793               ;Offset 0x2793
    or        al, 20h
Label0x2793:                            ;Offset 0x2793
    pop       es
    ret
Func0x2729 ENDP

;Offset 0x2795
Data0x2795              DB 000h, 002h, 002h, 003h, 00Dh, 00Eh, 00Eh, 00Fh, 004h, 006h, 006h, 007h, 011h, 012h, 012h, 013h

SaveRestoreVideoState:                  ;Offset 0x27a5
    test      al, al
    jne       Label0x27b9               ;Offset 0x27b9
    mov       bx, cx
    and       bx, 0fh
    mov       bl, byte ptr cs:[bx + Data0x2795];Offset 0x2795
    mov       ax, 001ch
Label0x27b6:                            ;Offset 0x27b6
    pop       ds
    pop       si
    iret
Label0x27b9:                            ;Offset 0x27b9
    call      Func0x388b                ;Offset 0x388b
    cmp       al, 02h
    ja        Label0x27b6               ;Offset 0x27b6
    je        Label0x2805               ;Offset 0x2805
    push      bx
    push      dx
    push      di
    mov       di, bx
    add       di, 0020h
    test      cl, 01h
    je        Label0x27d5               ;Offset 0x27d5
    mov       word ptr es:[bx], di
    call      Func0x2844                ;Offset 0x2844
Label0x27d5:                            ;Offset 0x27d5
    test      cl, 02h
    je        Label0x27e1               ;Offset 0x27e1
    mov       word ptr es:[bx + 02h], di
    call      Func0x2929                ;Offset 0x2929
Label0x27e1:                            ;Offset 0x27e1
    test      cl, 04h
    je        Label0x27ed               ;Offset 0x27ed
    mov       word ptr es:[bx + 04h], di
    call      Func0x299d                ;Offset 0x299d
Label0x27ed:                            ;Offset 0x27ed
    test      cl, 08h
    je        Label0x27f9               ;Offset 0x27f9
    mov       word ptr es:[bx + 06h], di
    call      Func0x3889                ;Offset 0x3889
Label0x27f9:                            ;Offset 0x27f9
    call      Func0x388c                ;Offset 0x388c
    mov       ax, 001ch
    pop       di
    pop       dx
    pop       bx
    pop       ds
    pop       si
    iret
Label0x2805:                            ;Offset 0x2805
    call      Func0x388b                ;Offset 0x388b
    push      bx
    push      dx
    test      cl, 01h
    je        Label0x2815               ;Offset 0x2815
    mov       si, word ptr es:[bx]
    call      Func0x28b7                ;Offset 0x28b7
Label0x2815:                            ;Offset 0x2815
    test      cl, 02h
    je        Label0x2821               ;Offset 0x2821
    mov       si, word ptr es:[bx + 02h]
    call      Func0x295b                ;Offset 0x295b
Label0x2821:                            ;Offset 0x2821
    test      cl, 04h
    je        Label0x282d               ;Offset 0x282d
    mov       si, word ptr es:[bx + 04h]
    call      Func0x29be                ;Offset 0x29be
Label0x282d:                            ;Offset 0x282d
    test      cl, 08h
    je        Label0x2839               ;Offset 0x2839
    mov       si, word ptr es:[bx + 06h]
    call      Func0x388a                ;Offset 0x388a
Label0x2839:                            ;Offset 0x2839
    call      Func0x388c                ;Offset 0x388c
    mov       ax, 001ch
    pop       dx
    pop       bx
    pop       ds
    pop       si
    iret

Func0x2844 PROC NEAR                    ;Offset 0x2844
    push      bx
    push      cx
    push      bp
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    in        al, dx
    stosb
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    push      dx
    in        al, dx
    stosb
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    in        al, dx
    stosb
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    in        al, dx
    stosb
    mov       bl, VGA_ATTR_ColorSelect  ;0x14
    call      GetIndividualPaletteRegister;Offset 0x1cc6
    mov       al, bh
    stosb
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ah, VGA_SEQIdx_ClockingMode;0x1
    mov       cx, 0004h
    call      Func0x29e8                ;Offset 0x29e8
    mov       dl, VGA_MiscellaneousRead_lowbyte;Port 0x3cc
    in        al, dx
    stosb
    pop       dx
    push      dx                        ;Port 0x3?4
    mov       cx, 0019h
    mov       ah, ch                    ;VGA_CRTCIdx_HorTotal
    call      Func0x29e8                ;Offset 0x29e8
    mov       cx, 0014h
    mov       ah, ch
    call      Func0x29f4                ;Offset 0x29f4
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cx, 09h
    mov       ah, ch                    ;VGA_GCTLIdx_SetReset
    call      Func0x29e8                ;Offset 0x29e8
    pop       bp                        ;Port 0x3?4
    mov       ax, bp
    stosw
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       al, VGA_GCTLIdx_ReadMapSelect;0x4
    call      ReadIndexedRegister       ;Offset 0x3f84
    push      ax
    mov       bx, 04h                   ;Undocumented index...
    mov       cx, 04h
Label0x289e:                            ;Offset 0x289e
    mov       ax, bx
    out       dx, ax
    xchg      dx, bp
    mov       al, VGA_CRTCIdx_LatchReadback;0x22
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, ah
    stosb
    xchg      dx, bp
    inc       bh
    loop      Label0x289e               ;Offset 0x289e
    pop       ax
    out       dx, ax
    pop       bp
    pop       cx
    pop       bx
    ret
Func0x2844 ENDP

Func0x28b7 PROC NEAR                    ;Offset 0x28b7
    push      bx
    push      cx
    push      ds
    push      es
    pop       ds
    push      si
    add       si, 04h
    lodsb
    mov       bh, al
    mov       bl, 14h
    push      si
    call      SetSinglePaletteRegister  ;Offset 0x1c1a
    pop       si
    mov       dx, VGA_SequenceIndex     ;Port 0x3c4
    mov       ax, VGA_SEQIdx_Reset OR (VGA_SEQ0_AsyncReset SHL 8);0x100
    out       dx, ax
    mov       bl, VGA_SEQIdx_ClockingMode;0x1
    mov       cx, 0004h
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    mov       dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    lodsb
    out       dx, al
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       ax, VGA_SEQIdx_Reset OR ((VGA_SEQ0_AsyncReset OR VGA_SEQ0_SyncReset) SHL 8);0x300
    out       dx, ax
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    mov       ax, VGA_CRTCIdx_VertRetraceEnd OR (VGA_VRE_DisableVertRetrIntr SHL 8);0x2011
    out       dx, ax
    mov       cx, 0019h
    mov       bl, ch
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    push      dx
    add       dl, 06h                   ;Port 0x3?a
    mov       cx, 0014h
    mov       bl, ch
    call      Func0x1eb5                ;Offset 0x1eb5
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       cx, 09h
    mov       bl, ch
    call      OutputListOfIndexedRegisters;Offset 0x40a0
    pop       dx
    pop       bx
    mov       al, byte ptr [bx + 01h]
    out       dx, al
    add       dl, 06h                   ;Port 0x3?a
    cli
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
    mov       al, byte ptr [bx + 03h]
    out       dx, al
    sti
    mov       dl, VGA_SequenceIndex_lowbyte;Port 0x3c4
    mov       al, byte ptr [bx]
    out       dx, al
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;0xce
    mov       al, byte ptr [bx + 02h]
    out       dx, al
    pop       ds
    pop       cx
    pop       bx
    ret
Func0x28b7 ENDP

Func0x2929 PROC NEAR                    ;Offset 0x2929
    push      cx
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    stosb
    mov       cx, 001eh
    mov       si, BDA_DisplayMode       ;Offset 0x449
    rep movsb
    mov       cx, 0007h
    mov       si, BDA_RowsOnScreen      ;Offset 0x484
    rep movsb
    mov       si, BDA_VideoParameterControlBlockPtrOfs;0x4a8
    movsw
    movsw
    mov       si, INT_5_Handler         ;Offset 0x14
    movsw
    movsw
    mov       si, INT_1D_Handler        ;Offset 0x74
    movsw
    movsw
    mov       si, INT_1F_Handler        ;Offset 0x7c
    movsw
    movsw
    mov       si, INT_43_Handler        ;Offset 0x10c
    movsw
    movsw
    pop       cx
    ret
Func0x2929 ENDP

Func0x295b PROC NEAR                    ;Offset 0x295b
    push      cx
    push      di
    push      ds
    push      es
    mov       al, byte ptr ds:[BDA_DetectedHardware];Offset 0x410
    and       al, BDA_DH_InitialVideoModeMask;0x30
    or        byte ptr es:[si], al
    mov       byte ptr ds:[BDA_DetectedHardware], al;Offset 0x410
    inc       si
    push      ds
    push      es
    pop       ds
    pop       es
    mov       cx, 001eh
    mov       di, BDA_DisplayMode       ;Offset 0x449
    rep movsb
    mov       cx, 0007h
    mov       di, BDA_RowsOnScreen      ;Offset 0x484
    rep movsb
    mov       di, BDA_VideoParameterControlBlockPointer;Offset 0x4a8
    movsw
    movsw
    mov       di, INT_5_Handler         ;Offset 0x14
    movsw
    movsw
    mov       di, INT_1D_Handler        ;Offset 0x74
    movsw
    movsw
    mov       di, INT_1F_Handler        ;Offset 0x7c
    movsw
    movsw
    mov       di, INT_43_Handler        ;Offset 0x10c
    movsw
    movsw
    pop       es
    pop       ds
    pop       di
    pop       cx
    ret
Func0x295b ENDP

Func0x299d PROC NEAR                    ;Offset 0x299d
    push      cx
    mov       dx, VGA_DACStatus         ;Port 0x3c7
    in        al, dx
    stosb
    inc       dx
    test      al, VGA_DACS_ReadCycle    ;0x3
    in        al, dx
    je        Label0x29aa               ;Offset 0x29aa
    dec       ax
Label0x29aa:                            ;Offset 0x29aa
    stosb
    mov       dl, VGA_DACMask_lowbyte   ;Port 0x3c6
    in        al, dx
    stosb
    inc       dx
    sub       al, al
    out       dx, al
    inc       dx
    inc       dx
    mov       cx, 0300h
Label0x29b8:                            ;Offset 0x29b8
    in        al, dx
    stosb
    loop      Label0x29b8               ;Offset 0x29b8
    pop       cx
    ret
Func0x299d ENDP

Func0x29be PROC NEAR                    ;Offset 0x29be
    push      bx
    push      cx
    push      ds
    push      es
    pop       ds
    push      si
    mov       dx, VGA_DACMask           ;Port 0x3c6
    inc       si
    inc       si
    lodsb
    out       dx, al
    inc       dx
    inc       dx
    sub       al, al
    out       dx, al
    mov       cx, 0300h
    inc       dx
Label0x29d4:                            ;Offset 0x29d4
    lodsb
    out       dx, al
    loop      Label0x29d4               ;Offset 0x29d4
    pop       bx
    test      byte ptr [bx], VGA_DACS_ReadCycle;0x3
    dec       dx
    je        Label0x29e0               ;Offset 0x29e0
    dec       dx
Label0x29e0:                            ;Offset 0x29e0
    mov       al, byte ptr [bx + 01h]
    out       dx, al
    pop       ds
    pop       cx
    pop       bx
    ret
Func0x29be ENDP

Func0x29e8 PROC NEAR                    ;Offset 0x29e8
Label0x29e8:                            ;Offset 0x29e8
    mov       al, ah
    out       dx, al
    inc       dx
    in        al, dx
    stosb
    dec       dx
    inc       ah
    loop      Label0x29e8               ;Offset 0x29e8
    ret
Func0x29e8 ENDP

Func0x29f4 PROC NEAR                    ;Offset 0x29f4
    mov       dx, VGA_InputStatus1B     ;Port 0x3ba
    cli
    in        al, dx
    mov       dl, VGA_InputStatus1D_lowbyte;Port 0x3da
    in        al, dx
    mov       dl, VGA_AttributeControllerIndex_lowbyte;Port 0x3c0
Label0x29fe:                            ;Offset 0x29fe
    mov       al, ah
    out       dx, al
    inc       dx
    in        al, dx
    stosb
    dec       dx
    out       dx, al
    inc       ah
    loop      Label0x29fe               ;Offset 0x29fe
    mov       al, VGA_ATTR_PaletteAddressSource;0x20
    out       dx, al
    sti
    ret
Func0x29f4 ENDP

                        DB 000h

;Offset 0x2a10
VESACallTable           DW offset GetSuperVGAInformation;0x2a52          ;0
                        DW offset GetSuperVGAModeInformation;0x2af7      ;1
                        DW offset SetSuperVGAVideoMode;0x2d4c            ;2
                        DW offset SuperVGAGetCurrentVideoMode;0x2d92     ;3
                        DW offset SaveRestoreSuperVGAVideoState;0x2db5   ;4
                        DW offset CPUVideoMemoryControl;0x2dc4           ;5
                        DW offset LogicalScanLineLengthHandling;0x2df5   ;6
                        DW offset DisplayStartHandling;0x2fc6            ;7
                        DW offset GetSetDACPaletteControl;0x30f8         ;8
                        DW offset GetSetPaletteEntries;0x312c            ;9
                        DW offset GetProtectedModeInterface;0x3194       ;a
                        DW offset GetNearestPixelClock;0x31a0            ;b
                        DW offset VESANotImplemented;0x2af6              ;c
                        DW offset VESANotImplemented;0x2af6              ;d
                        DW offset VESANotImplemented;0x2af6              ;e
                        DW offset VESANotImplemented;0x2af6              ;f
                        DW offset PowerManagement;0x31af                 ;10
                        DW offset VESANotImplemented;0x2af6              ;11
                        DW offset VESANotImplemented;0x2af6              ;12
                        DW offset VESANotImplemented;0x2af6              ;13
                        DW offset VESANotImplemented;0x2af6              ;14
                        DW offset DisplayDataChannel;0x31d0              ;15

VESAHandler:                            ;Offset 0x2a3c
    cmp       al, 16h
    jae       Label0x2a4d               ;Offset 0x2a4d
    cbw
    shl       ax, 01h
    mov       si, ax
    call      word ptr cs:[si + VESACallTable];Offset 0x2a10
Label0x2a4a:                            ;Offset 0x2a4a
    pop       ds
    pop       si
    iret
Label0x2a4d:                            ;Offset 0x2a4d
    mov       ax, 0100h
    jmp       Label0x2a4a               ;Offset 0x2a4a

GetSuperVGAInformation PROC NEAR        ;0x2a52
    pushaw    
    push      ds
    push      cs
    pop       ds
    sub       cx, cx
    cmp       dword ptr es:[di], 032454256h  ;'VBE2'
    jne       Label0x2a63               ;Offset 0x2a63
    inc       cx
Label0x2a63:                            ;Offset 0x2a63
    push      cx
    mov       cx, 0014h
    mov       si, offset VESAResponseTemplate;Offset 0x7d38
    push      di
    rep movsb
    mov       cx, 0ech
    sub       al, al
    rep stosb
    pop       di
    call      GetMemorySizeIn256KBlocks ;Offset 0x37fc
    shl       ax, 02h
    mov       word ptr es:[di + VESA_INFO_MemoryIn64KBBlocks], ax;Offset 0x12
    mov       word ptr es:[di + VESA_INFO_Version], 0300h;Offset 0x4
    mov       word ptr es:[di + VESA_INFO_OEMNamePointerSeg], cs;Offset 0x8
    mov       word ptr es:[di + VESA_INFO_SupportedVideoModesPointerSeg], cs;Offset 0x10
    pop       cx
    jcxz      Label0x2af1               ;Offset 0x2af1
    lea       bx, [di + 100h]
    push      bx
    xchg      bx, di
    mov       word ptr es:[bx + VESA_INFO_OEMSoftwareVersion], 0100h;Offset 0x14
    mov       word ptr es:[bx + VESA_INFO_OEMNamePointerOfs], di;Offset 0x6
    mov       word ptr es:[bx + VESA_INFO_OEMNamePointerSeg], es;Offset 0x8
    mov       si, offset OEMName        ;Offset 0x7dbe
    call      StringCopy                ;Offset 0x41c7
    mov       word ptr es:[bx + VESA_INFO_SupportedVideoModesPointerOfs], di;Offset 0xe
    mov       word ptr es:[bx + VESA_INFO_SupportedVideoModesPointerSeg], es;Offset 0x10
    mov       cx, 0022h
    mov       si, offset SupportedVESAVideoModes;Offset 0x7d4c
    rep movsw
    mov       word ptr es:[bx + VESA_INFO_VendorNamePointerOfs], di;Offset 0x16
    mov       word ptr es:[bx + VESA_INFO_VendorNamePointerSeg], es;Offset 0x18
    mov       si, offset VendorName     ;Offset 0x7d90
    call      StringCopy                ;Offset 0x41c7
    mov       word ptr es:[bx + VESA_INFO_ProductNamePointerOfs], di;Offset 0x1a
    mov       word ptr es:[bx + VESA_INFO_ProductNamePointerSeg], es;Offset 0x1c
    mov       si, offset ProductName    ;Offset 0x7da4
    call      StringCopy                ;Offset 0x41c7
    mov       word ptr es:[bx + VESA_INFO_RevisionStringPointerOfs], di;Offset 0x1e
    mov       word ptr es:[bx + VESA_INFO_RevisionStringPointerSeg], es;Offset 0x20
    mov       si, offset RevisionString ;Offset 0x7db1
    call      StringCopy                ;Offset 0x41c7
    pop       ax
    mov       cx, 0100h
    sub       cx, di
    add       cx, ax
    sub       al, al
    rep stosb
Label0x2af1:                            ;Offset 0x2af1
    pop       ds
    popaw
    mov       ax, 004fh
GetSuperVGAInformation ENDP
;continue!
VESANotImplemented PROC NEAR            ;Offset 0x2af6
    ret
VESANotImplemented ENDP

GetSuperVGAModeInformation PROC NEAR    ;Offset 0x2af7
    pushaw
    call      Func0x2b1c                ;Offset 0x2b1c
    jb        Label0x2b17               ;Offset 0x2b17
    call      Func0x2b52                ;Offset 0x2b52
    call      Func0x2ba7                ;Offset 0x2ba7
    call      Func0x2be7                ;Offset 0x2be7
    call      Func0x2cba                ;Offset 0x2cba
    call      Func0x2ce2                ;Offset 0x2ce2
    call      Func0x2cfe                ;Offset 0x2cfe
    call      Func0x37f7                ;Offset 0x37f7
    popaw
    mov       ax, 004fh
    ret
Label0x2b17:                            ;Offset 0x2b17
    popaw
    mov       ax, 014fh
    ret
GetSuperVGAModeInformation ENDP

Func0x2b1c PROC NEAR                    ;Offset 0x2b1c
    push      ax
    push      cx
    push      bp
    push      es
    mov       bp, es
    call      EnableVGAExtensions       ;Offset 0x3852
    mov       ax, cx
    cmp       ax, 0100h
    jae       Label0x2b31               ;Offset 0x2b31
    call      FindModeData              ;Offset 0x40d9
    jmp       Label0x2b34               ;Offset 0x2b34
Label0x2b31:                            ;Offset 0x2b31
    call      FindVESAModeData          ;Offset 0x41a9
Label0x2b34:                            ;Offset 0x2b34
    jb        Label0x2b4d               ;Offset 0x2b4d
    push      es
    pop       ds
    mov       si, bx
    mov       al, byte ptr [si + 01h]
    mov       bl, byte ptr [si]
    mov       bh, al
    mov       es, bp
    push      di
    mov       cx, 0080h
    sub       ax, ax
    rep stosw
    pop       di
    clc
Label0x2b4d:                            ;Offset 0x2b4d
    pop       es
    pop       bp
    pop       cx
    pop       ax
    ret
Func0x2b1c ENDP

Func0x2b52 PROC NEAR                    ;Offset 0x2b52
    push      cx
    push      dx
    mov       dx, 0006h
    mov       al, bl
    call      Func0x3863                ;Offset 0x3863
    jb        Label0x2b61               ;Offset 0x2b61
    or        dl, 01h
Label0x2b61:                            ;Offset 0x2b61
    call      Func0x3faf                ;Offset 0x3faf
    je        Label0x2b6a               ;Offset 0x2b6a
    cmp       al, 13h
    ja        Label0x2b6d               ;Offset 0x2b6d
Label0x2b6a:                            ;Offset 0x2b6a
    or        dl, 04h
Label0x2b6d:                            ;Offset 0x2b6d
    call      Func0x3f99                ;Offset 0x3f99
    je        Label0x2b75               ;Offset 0x2b75
    or        dl, 08h
Label0x2b75:                            ;Offset 0x2b75
    call      Func0x3faf                ;Offset 0x3faf
    je        Label0x2b7d               ;Offset 0x2b7d
    or        dl, 10h
Label0x2b7d:                            ;Offset 0x2b7d
    push      ds
    call      GetBIOSDataSelectorInDS   ;Offset 0x3f40
    test      byte ptr ds:[BDA_VideoDisplayDataArea], 01h;Offset 0x489
    jne       Label0x2ba0               ;Offset 0x2ba0
    mov       cl, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    not       cl
    and       cl, 02h
    shl       cl, 02h
    mov       ch, dl
    and       ch, 08h
    cmp       cl, ch
    je        Label0x2ba0               ;Offset 0x2ba0
    and       dl, 0feh
Label0x2ba0:                            ;Offset 0x2ba0
    pop       ds
    mov       word ptr es:[di + VESA_MODE_INFO_Attributes], dx;Offset 0x0
    pop       dx
    pop       cx
    ret
Func0x2b52 ENDP

Func0x2ba7 PROC NEAR                    ;Offset 0x2ba7
    pushaw
    mov       al, 06h
    cmp       bl, 13h
    jbe       Label0x2bb0               ;Offset 0x2bb0
    inc       ax
Label0x2bb0:                            ;Offset 0x2bb0
    mov       byte ptr es:[di + VESA_MODE_INFO_AttributesWindowA], al; Offset 0x2
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningOfs], offset Func0x2de0;Offset 0xc Offset 0x2de0
    mov       word ptr es:[di + VESA_MODE_INFO_FARWindowPositioningSeg], cs;Offset 0xe
    mov       al, byte ptr [si + 04h]
    mov       dx, 0b800h                ;Segment 0xb800
    mov       cx, 0020h
    cmp       al, 00h
    je        Label0x2bd9               ;Offset 0x2bd9
    cmp       al, 02h
    je        Label0x2bd9               ;Offset 0x2bd9
    mov       dh, 0b0h                  ;Segment 0xb000
    cmp       al, 01h
    je        Label0x2bd9               ;Offset 0x2bd9
    mov       dh, 0a0h                  ;Segment 0xa000
    shl       cx, 01h
Label0x2bd9:                            ;Offset 0x2bd9
    mov       word ptr es:[di + VESA_MODE_INFO_StartSegmentWindowA], dx
    mov       word ptr es:[di + VESA_MODE_INFO_WindowGranularityInKB], cx
    mov       word ptr es:[di + VESA_MODE_INFO_WindowSizeInKB], cx
    popaw
    ret
Func0x2ba7 ENDP

Func0x2be7 PROC NEAR                    ;Offset 0x2be7
    pushad    
    mov       ax, word ptr [si + 0bh]
    mov       word ptr es:[di + VESA_MODE_INFO_BytesPerScanLine], ax;Offset 0x10
    mov       word ptr es:[di + VESA_MODE_INFO_BytesPerScanLineLinear], ax;Offset 0x32
    mov       ax, word ptr [si + 07h]
    mov       word ptr es:[di + VESA_MODE_INFO_WidthInPixelsOrChars], ax;Offset 0x12
    mov       ax, word ptr [si + 09h]
    mov       word ptr es:[di + VESA_MODE_INFO_HeightInPixelsOrChars], ax;Offset 0x14
    mov       al, byte ptr [si + 0dh]
    mov       byte ptr es:[di + VESA_MODE_INFO_CharCellHeight], al;Offset 0x17
    mov       al, byte ptr [si + 06h]
    mov       byte ptr es:[di + VESA_MODE_INFO_NumberOfBitsPerPixel], al;Offset 0x19
    mov       al, bl
    sub       cx, cx
    call      Func0x3faf                ;Offset 0x3faf
    je        Label0x2c1a               ;Offset 0x2c1a
    inc       cx
Label0x2c1a:                            ;Offset 0x2c1a
    mov       al, 09h
    jcxz      Label0x2c1f               ;Offset 0x2c1f
    dec       ax
Label0x2c1f:                            ;Offset 0x2c1f
    mov       byte ptr es:[di + VESA_MODE_INFO_CharCellWidth], al;Offset 0x16
    mov       dl, 01h
    mov       al, bl
    call      Func0x418c                ;Offset 0x418c
    jne       Label0x2c2e               ;Offset 0x2c2e
    mov       dl, 04h
Label0x2c2e:                            ;Offset 0x2c2e
    mov       byte ptr es:[di + VESA_MODE_INFO_NumberOfMemoryPlanes], dl;Offset 0x18
    mov       al, byte ptr es:[di + VESA_MODE_INFO_NumberOfBitsPerPixel];Offset 0x19
    sub       dl, dl
    jcxz      Label0x2c4d               ;Offset 0x2c4d
    inc       dx
    cmp       bl, 06h
    jbe       Label0x2c4d               ;Offset 0x2c4d
    inc       dx
    inc       dx
    cmp       al, 04h
    jbe       Label0x2c4d               ;Offset 0x2c4d
    inc       dx
    cmp       al, 08h
    je        Label0x2c4d               ;Offset 0x2c4d
    inc       dx
    inc       dx
Label0x2c4d:                            ;Offset 0x2c4d
    mov       byte ptr es:[di + VESA_MODE_INFO_MemoryModelType], dl;Offset 0x1b
    mov       dx, 01h
    jcxz      Label0x2c5f               ;Offset 0x2c5f
    cmp       bl, 06h
    ja        Label0x2c5f               ;Offset 0x2c5f
    add       dx, 0801h
Label0x2c5f:                            ;Offset 0x2c5f
    mov       byte ptr es:[di + VESA_MODE_INFO_NumberOfBanks], dl;Offset 0x1a
    mov       byte ptr es:[di + VESA_MODE_INFO_SizeOfBankInKB], dh;Offset 0x1c
    mov       al, bl
    call      Func0x2d0b                ;Offset 0x2d0b
    shl       eax, 10h
    mov       ax, dx
    rol       eax, 10h
    movzx     ecx, word ptr es:[di + VESA_MODE_INFO_BytesPerScanLine];Offset 0x10
    movzx     edx, byte ptr es:[di + VESA_MODE_INFO_NumberOfMemoryPlanes];Offset 0x18
    imul      ecx, edx
    movzx     edx, word ptr es:[di + VESA_MODE_INFO_HeightInPixelsOrChars];Offset 0x14
    imul      ecx, edx
    cdq
    add       eax, 0000ffffh
    adc       edx, 00000000h
    sub       ax, ax
    div       ecx
    test      al, al
    je        Label0x2ca6               ;Offset 0x2ca6
    dec       ax
Label0x2ca6:                            ;Offset 0x2ca6
    mov       byte ptr es:[di + VESA_MODE_INFO_NumberOfImgPagesMinusOne], al;Offset 0x1d
    mov       byte ptr es:[di + VESA_MODE_INFO_ImageCountMinusOneBanked], al;Offset 0x34
    mov       byte ptr es:[di + VESA_MODE_INFO_ImageCountMinusOneLinear], al;Offset 0x35
    mov       byte ptr es:[di + VESA_MODE_INFO_Reserved1E], 01h;Offset 0x1e
    popad
    ret
Func0x2be7 ENDP

Func0x2cba PROC NEAR                    ;Offset 0x2cba
    pushaw
    mov       si, offset Data0x7dd5     ;Offset 0x7dd5
    cmp       byte ptr es:[di + VESA_MODE_INFO_NumberOfBitsPerPixel], 10h;Offset 0x19
    jb        Label0x2ce0               ;Offset 0x2ce0
    je        Label0x2cca               ;Offset 0x2cca
    mov       si, offset Data0x7dde     ;Offset 0x7dde
Label0x2cca:                            ;Offset 0x2cca
    mov       cx, 0009h
    push      cx
    push      di
    push      si
    lea       di, [di + VESA_MODE_INFO_RedMaskSize];Offset 0x1f
    rep movsb byte ptr es:[di], byte ptr cs:[si]
    pop       si
    pop       di
    pop       cx
    dec       cx
    lea       di, [di + VESA_MODE_INFO_LinearRedMaskSizeBits];Offset 0x36
    rep movsb byte ptr es:[di], byte ptr cs:[si]
Label0x2ce0:                            ;Offset 0x2ce0
    popaw
    ret
Func0x2cba ENDP

Func0x2ce2 PROC NEAR                    ;Offset 0x2ce2
    push      eax
    cmp       bl, BDA_DM_320x200_256_Color_Graphics;0x13
    jbe       Label0x2cfb               ;Offset 0x2cfb
    call      GetLinearFrameBuffer      ;Offset 0x3817
    mov       dword ptr es:[di + VESA_MODE_INFO_LinearFrameBufferAddress], eax;Offset 0x28
    test      eax, eax
    je        Label0x2cfb               ;Offset 0x2cfb
    or        word ptr es:[di], 80h
Label0x2cfb:                            ;Offset 0x2cfb
    pop       eax
    ret
Func0x2ce2 ENDP

Func0x2cfe PROC NEAR                    ;Offset 0x2cfe
    push      eax
    call      GetMaxPixelClockHz        ;Offset 0x3998
    mov       dword ptr es:[di + VESA_MODE_INFO_MaxPixelClockHz], eax;Offset 0x3e
    pop       eax
    ret
Func0x2cfe ENDP

Func0x2d0b PROC NEAR                    ;Offset 0x2d0b
    push      bx
    mov       bl, al
    sub       dx, dx
    mov       ax, 8000h
    cmp       bl, 07h
    jbe       Label0x2d4a               ;Offset 0x2d4a
    push      ax
    mov       al, bl
    call      Func0x3faf                ;Offset 0x3faf
    pop       ax
    je        Label0x2d4a               ;Offset 0x2d4a
    mov       ax, dx
    inc       dx
    cmp       bl, 13h
    je        Label0x2d4a               ;Offset 0x2d4a
    mov       dx, 0004h
    jb        Label0x2d4a               ;Offset 0x2d4a
    mov       al, bl
    call      Func0x418c                ;Offset 0x418c
    mov       ax, 0000h
    mov       dx, 0004h
    je        Label0x2d4a               ;Offset 0x2d4a
    call      GetMemorySizeIn256KBlocks ;Offset 0x37fc
    shl       ax, 02h
    mov       bx, ax
    mov       ax, 8000h
    shl       bx, 01h
    mul       bx
Label0x2d4a:                            ;Offset 0x2d4a
    pop       bx
    ret
Func0x2d0b ENDP

SetSuperVGAVideoMode PROC NEAR          ;Offset 0x2d4c
    push      bx
    push      cx
    push      dx
    mov       ax, bx
    and       ah, 07h
    cmp       ax, 0100h
    jb        Label0x2d5e               ;Offset 0x2d5e
    call      Func0x41ce                ;Offset 0x41ce
    jb        Label0x2d8d               ;Offset 0x2d8d
Label0x2d5e:                            ;Offset 0x2d5e
    call      Func0x3863                ;Offset 0x3863
    jb        Label0x2d8d               ;Offset 0x2d8d
    push      ax
    push      bx
    and       bh, 80h
    or        al, bh
    sub       ah, ah
    int       10h
    pop       bx
    pop       ax
    cmp       byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    jne       Label0x2d8d               ;Offset 0x2d8d
    test      bh, 40h
    je        Label0x2d7e               ;Offset 0x2d7e
    call      Func0x3824                ;Offset 0x3824
Label0x2d7e:                            ;Offset 0x2d7e
    test      bh, 08h
    je        Label0x2d86               ;Offset 0x2d86
    call      Func0x39cd                ;Offset 0x39cd
Label0x2d86:                            ;Offset 0x2d86
    mov       ax, 004fh
Label0x2d89:                            ;Offset 0x2d89
    pop       dx
    pop       cx
    pop       bx
    ret
Label0x2d8d:                            ;Offset 0x2d8d
    mov       ax, 014fh
    jmp       Label0x2d89               ;Offset 0x2d89
SetSuperVGAVideoMode ENDP

SuperVGAGetCurrentVideoMode PROC NEAR   ;Offset 0x2d92
    push      dx
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x41e2                ;Offset 0x41e2
    cmp       bx, 0ffffh
    jne       Label0x2da1               ;Offset 0x2da1
    cbw
    mov       bx, ax
Label0x2da1:                            ;Offset 0x2da1
    call      Func0x3835                ;Offset 0x3835
    jae       Label0x2da9               ;Offset 0x2da9
    or        bh, 40h
Label0x2da9:                            ;Offset 0x2da9
    mov       al, byte ptr ds:[BDA_VideoModeOptions];Offset 0x487
    and       al, 80h
    or        bh, al
    mov       ax, 004fh
    pop       dx
    ret
SuperVGAGetCurrentVideoMode ENDP

SaveRestoreSuperVGAVideoState PROC NEAR ;Offset 0x2db5
    push      cx
    mov       ah, 1ch                   ;Save / restore video state.
    mov       al, dl
    and       cx, 0fh
    int       10h
    mov       ax, 004fh
    pop       cx
    ret
SaveRestoreSuperVGAVideoState ENDP

CPUVideoMemoryControl PROC NEAR         ;Offset 0x2dc4
    mov       ax, 014fh
    test      bl, bl
    jne       Label0x2dda               ;Offset 0x2dda
    cmp       bh, 01h
    je        Label0x2ddb               ;Offset 0x2ddb
    test      bh, bh
    jne       Label0x2dda               ;Offset 0x2dda
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
Label0x2dd7:                            ;Offset 0x2dd7
    mov       ax, 004fh
Label0x2dda:                            ;Offset 0x2dda
    ret
Label0x2ddb:                            ;Offset 0x2ddb
    call      GetVGAWriteApertureAddress64K;Offset 0x3c3b
    jmp       Label0x2dd7               ;Offset 0x2dd7
CPUVideoMemoryControl ENDP

Func0x2de0 PROC FAR                     ;Offset 0x2de0
    test      bl, bl
    jne       Label0x2df0               ;Offset 0x2df0
    cmp       bh, 01h
    je        Label0x2df1               ;Offset 0x2df1
    test      bh, bh
    jne       Label0x2df0               ;Offset 0x2df0
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
Label0x2df0:                            ;Offset 0x2df0
    retf
Label0x2df1:                            ;Offset 0x2df1
    call      GetVGAWriteApertureAddress64K;Offset 0x3c3b
    retf
Func0x2de0 ENDP

LogicalScanLineLengthHandling PROC NEAR ;Offset 0x2df5
    push      bp
    push      di
    call      EnableVGAExtensions       ;Offset 0x3852
    mov       al, bl
    test      al, al
    je        Label0x2e16               ;Offset 0x2e16
    cmp       al, 01h
    je        Label0x2e2c               ;Offset 0x2e2c
    cmp       al, 02h
    je        Label0x2e20               ;Offset 0x2e20
    cmp       al, 03h
    je        Label0x2e51               ;Offset 0x2e51
    mov       ax, 014fh
    jmp       Label0x2e4e               ;Offset 0x2e4e
Label0x2e11:                            ;Offset 0x2e11
    mov       ax, 024fh
    jmp       Label0x2e4e               ;Offset 0x2e4e
Label0x2e16:                            ;Offset 0x2e16
    call      Func0x2e56                ;Offset 0x2e56
    cmp       cx, ax
    jb        Label0x2e11               ;Offset 0x2e11
    call      Func0x2e65                ;Offset 0x2e65
Label0x2e20:                            ;Offset 0x2e20
    call      Func0x2ee7                ;Offset 0x2ee7
    cmp       cx, ax
    ja        Label0x2e11               ;Offset 0x2e11
    mov       ax, cx
    call      Func0x2f66                ;Offset 0x2f66
Label0x2e2c:                            ;Offset 0x2e2c
    call      Func0x2f80                ;Offset 0x2f80
Label0x2e2f:                            ;Offset 0x2e2f
    mov       bx, ax
    call      Func0x2f8b                ;Offset 0x2f8b
    mov       cx, ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x2d0b                ;Offset 0x2d0b
    div       bx
    mov       dx, ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x418c                ;Offset 0x418c
    jne       Label0x2e4b               ;Offset 0x2e4b
    shr       dx, 02h
Label0x2e4b:                            ;Offset 0x2e4b
    mov       ax, 004fh
Label0x2e4e:                            ;Offset 0x2e4e
    pop       di
    pop       bp
    ret
Label0x2e51:                            ;Offset 0x2e51
    call      Func0x2ee7                ;Offset 0x2ee7
    jmp       Label0x2e2f               ;Offset 0x2e2f
LogicalScanLineLengthHandling ENDP

Func0x2e56 PROC NEAR                    ;Offset 0x2e56
    push      bx
    push      es
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    mov       ax, word ptr es:[bx + 07h]
    pop       es
    pop       bx
    ret
Func0x2e56 ENDP

Func0x2e65 PROC NEAR                    ;Offset 0x2e65
    push      ax
    push      bx
    push      dx
    mov       dx, cx
    call      Func0x388d                ;Offset 0x388d
    mov       bl, al
    call      Func0x2ec3                ;Offset 0x2ec3
    movzx     ax, ch
    push      ax
    mul       dx
    sub       bh, bh
    div       bx
    pop       bx
    cmp       bl, 18h
    je        Label0x2ea6               ;Offset 0x2ea6
    test      dx, dx
    je        Label0x2e8f               ;Offset 0x2e8f
    mov       bx, 01h
    shl       bx, cl
    add       ax, bx
    jmp       Label0x2ea0               ;Offset 0x2ea0
Label0x2e8f:                            ;Offset 0x2e8f
    mov       bx, ax
    shr       bx, cl
    shl       bx, cl
    cmp       bx, ax
    jae       Label0x2ea0               ;Offset 0x2ea0
    mov       ax, 01h
    shl       ax, cl
    add       ax, bx
Label0x2ea0:                            ;Offset 0x2ea0
    mov       cx, ax
    pop       dx
    pop       bx
    pop       ax
    ret
Label0x2ea6:                            ;Offset 0x2ea6
    test      al, 07h
    je        Label0x2ea0               ;Offset 0x2ea0
    add       ax, 03h
    jmp       Label0x2ea6               ;Offset 0x2ea6
Func0x2e65 ENDP

;Offset 0x2eaf
Data0x2eaf              DW 00202h       ;Offset 0x0202
                        DW 00202h       ;Offset 0x0202
                        DW 00102h       ;Offset 0x0102
                        DW 00101h       ;Offset 0x0101
                        DW 00101h       ;Offset 0x0101
                        DW 00101h       ;Offset 0x0101
                        DW 00803h       ;Offset 0x0803
                        DW 01003h       ;Offset 0x1003
                        DW 01803h       ;Offset 0x1803
                        DW 00000h       ;Offset 0x0000

Func0x2ec3 PROC NEAR                    ;Offset 0x2ec3
    push      ax
    push      bx
    push      si
    push      es
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    movzx     si, byte ptr es:[bx + 04h]
    shl       si, 01h
    mov       cx, word ptr cs:[si + Data0x2eaf];Offset 0x2eaf
    cmp       ch, 18h
    jb        Label0x2ee2               ;Offset 0x2ee2
    mov       ch, byte ptr es:[bx + 06h]
Label0x2ee2:                            ;Offset 0x2ee2
    pop       es
    pop       si
    pop       bx
    pop       ax
    ret
Func0x2ec3 ENDP

Func0x2ee7 PROC NEAR                    ;Offset 0x2ee7
    push      bx
    push      dx
    push      si
    call      Func0x2f40                ;Offset 0x2f40
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x2f56                ;Offset 0x2f56
    mov       bx, ax
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      Func0x2d0b                ;Offset 0x2d0b
    jmp       word ptr cs:[si + Data0x2f2e];Offset 0x2f2e
Func0x2ee7 ENDP

Label0x2f00:                            ;Offset 0x2f00
    div       bx
    imul      word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    and       al, 0f0h
    jmp       Label0x2f27               ;Offset 0x2f27

Label0x2f0a:                            ;Offset 0x2f0a
    shl       bx, 02h
;continue!
Label0x2f0d:                            ;Offset 0x2f0d
    div       bx
    and       al, 0f0h
    jmp       Label0x2f27               ;Offset 0x2f27

Label0x2f13:                            ;Offset 0x2f13
    div       bx
    mov       bx, 03h
    cwd
    div       bx
    imul      ax, ax, 0003h
;continue!
Label0x2f1e:                            ;Offset 0x2f1e
    test      al, 07h
    je        Label0x2f27               ;Offset 0x2f27
    sub       ax, 0003h
    jmp       Label0x2f1e               ;Offset 0x2f1e

Label0x2f27:                            ;Offset 0x2f27
    call      Func0x38f0                ;Offset 0x38f0
    pop       si
    pop       dx
    pop       bx
    ret

;Offset 0x2f2e
Data0x2f2e              DW offset Label0x2f00;Offset 0x2f00
                        DW offset Label0x2f00;Offset 0x2f00
                        DW offset Label0x2f0d;Offset 0x2f0d
                        DW offset Label0x2f0a;Offset 0x2f0a
                        DW offset Label0x2f0a;Offset 0x2f0a
                        DW offset Label0x2f0a;Offset 0x2f0a
                        DW offset Label0x2f0d;Offset 0x2f0d
                        DW offset Label0x2f0d;Offset 0x2f0d
                        DW offset Label0x2f13;Offset 0x2f13

Func0x2f40 PROC NEAR                    ;Offset 0x2f40
    push      ax
    push      bx
    push      es
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    mov       al, byte ptr es:[bx + 04h]
    cbw
    shl       ax, 01h
    mov       si, ax
    pop       es
    pop       bx
    pop       ax
    ret
Func0x2f40 ENDP

Func0x2f56 PROC NEAR                    ;Offset 0x2f56
    push      bx
    push      es
    call      FindModeData              ;Offset 0x40d9
    mov       al, 00h
    jb        Label0x2f63               ;Offset 0x2f63
    mov       ax, word ptr es:[bx + 09h]
Label0x2f63:                            ;Offset 0x2f63
    pop       es
    pop       bx
    ret       
Func0x2f56 ENDP

Func0x2f66 PROC NEAR                    ;Offset 0x2f66
    push      ax
    push      cx
    push      dx
    call      Func0x2ec3                ;Offset 0x2ec3
    push      ax
    shr       ax, cl
    call      Func0x3890                ;Offset 0x3890
    pop       ax
    cwd
    shr       cx, 08h
    div       cx
    mov       word ptr ds:[BDA_NumberOfScreenColumns], ax;Offset 0x44a
    pop       dx
    pop       cx
    pop       ax
    ret
Func0x2f66 ENDP

Func0x2f80 PROC NEAR                    ;Offset 0x2f80
    push      cx
    call      Func0x38c2                ;Offset 0x38c2
    call      Func0x2ec3                ;Offset 0x2ec3
    shl       ax, cl
    pop       cx
    ret
Func0x2f80 ENDP

Func0x2f8b PROC NEAR                    ;Offset 0x2f8b
    push      dx
    push      cx
    call      Func0x2ec3                ;Offset 0x2ec3
    call      Func0x388d                ;Offset 0x388d
    cbw
    mul       bx
    shr       cx, 08h
    div       cx
    pop       cx
    pop       dx
    ret
Func0x2f8b ENDP

;Offset 0x2f9e
Data0x2f9e              DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset Func0x300c;Offset 0x300c
                        DW offset Func0x3027;Offset 0x3027
                        DW offset Func0x3045;Offset 0x3045
                        DW offset Func0x305d;Offset 0x305d
                        DW offset VESANotImplemented;Offset 0x2af6

;Offset 0x2fb2
Data0x2fb2              DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset VESANotImplemented;Offset 0x2af6
                        DW offset Func0x307a;Offset 0x307a
                        DW offset Func0x3093;Offset 0x3093
                        DW offset Func0x30a7;Offset 0x30a7
                        DW offset Func0x30ba;Offset 0x30ba
                        DW offset VESANotImplemented;Offset 0x2af6

DisplayStartHandling PROC NEAR          ;Offset 0x2fc6
    call   EnableVGAExtensions          ;Offset 0x3852
    cmp    bl, 01h
    je     Label0x2ff6                  ;Offset 0x2ff6
    test   bl, bl
    je     Label0x2fdf                  ;Offset 0x2fdf
    cmp    bl, 80h
    jne    Label0x2ff1                  ;Offset 0x2ff1
    push   dx
    call   WaitUntilVSyncInactive       ;Offset 0x406a
    call   WaitUntilVSyncActive         ;Offset 0x4057
    pop    dx
Label0x2fdf:                            ;Offset 0x2fdf
    pushad
    call   Func0x2f40                   ;Offset 0x2f40
    call   word ptr cs:[si + Data0x2f9e];Offset 0x2f9e
    popad
    jb     Label0x2ff1                  ;Offset 0x2ff1
Label0x2fed:                            ;Offset 0x2fed
    mov    ax, 004fh
Label0x2ff0:                            ;Offset 0x2ff0
    ret
Label0x2ff1:                            ;Offset 0x2ff1
    mov    ax, 014fh
    jmp    Label0x2ff0                  ;Offset 0x2ff0
Label0x2ff6:                            ;Offset 0x2ff6
    push   eax
    push   bx
    push   si
    push   di
    call   Func0x2f40                   ;Offset 0x2f40
    call   word ptr cs:[si + Data0x2fb2];Offset 0x2fb2
    pop    di
    pop    si
    pop    bx
    pop    eax
    sub    bh, bh
    jmp    Label0x2fed                  ;Offset 0x2fed
DisplayStartHandling ENDP

Func0x300c PROC NEAR                    ;Offset 0x300c
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mul    dx
    push   cx
    shr    cx, 03h
    add    ax, cx
    adc    dx, 0000h
    call   Func0x38f1                   ;Offset 0x38f1
    pop    ax
    and    al, 07h
    call   SetHorizontalPELPanning      ;Offset 0x30d6
    clc
    ret
Func0x300c ENDP

Func0x3027 PROC NEAR                    ;Offset 0x3027
    cmp    dx, 0178h
    test   cl, 03h
    stc
    jne    Label0x3044                  ;Offset 0x3044
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mul    dx
    shr    cx, 02h
    add    ax, cx
    adc    dx, 0000h
    call   Func0x38f1                   ;Offset 0x38f1
    clc
Label0x3044:                            ;Offset 0x3044
    ret
Func0x3027 ENDP

Func0x3045 PROC NEAR                    ;Offset 0x3045
    test   cl, 03h
    stc    
    jne    Label0x305c                  ;Offset 0x305c
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mul    dx
    shr    cx, 01h
    add    ax, cx
    adc    dx, 0000h
    call   Func0x38f1                   ;Offset 0x38f1
Label0x305c:                            ;Offset 0x305c
    ret
Func0x3045 ENDP

Func0x305d PROC NEAR                    ;Offset 0x305d
    imul   cx, cx, 0003h
    test   cl, 03h
    stc
    jne    Label0x3079                  ;Offset 0x3079
    shr    cx, 02h
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mul    dx
    add    ax, cx
    adc    dx, 0000h
    call   Func0x38f1                   ;Offset 0x38f1
    clc
Label0x3079:                            ;Offset 0x3079
    ret
Func0x305d ENDP

Func0x307a PROC NEAR                    ;Offset 0x307a
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mov    bx, ax
    call   GetStartAddress              ;Offset 0x3926
    div    bx
    mov    cx, dx
    mov    dx, ax
    shl    cx, 03h
    call   GetHorizontalPELPanning      ;Offset 0x30e7
    add    cx, ax
    ret
Func0x307a ENDP

Func0x3093 PROC NEAR                    ;Offset 0x3093
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mov    bx, ax
    call   GetStartAddress              ;Offset 0x3926
    div    bx
    mov    cx, dx
    mov    dx, ax
    shl    cx, 02h
    ret
Func0x3093 ENDP

Func0x30a7 PROC NEAR                    ;Offset 0x30a7
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mov    bx, ax
    call   GetStartAddress              ;Offset 0x3926
    div    bx
    mov    cx, dx
    mov    dx, ax
    shl    cx, 01h
    ret
Func0x30a7 ENDP

Func0x30ba PROC NEAR                    ;Offset 0x30ba
    call   Func0x38c2                   ;Offset 0x38c2
    shl    ax, 01h
    mov    bx, ax
    call   GetStartAddress              ;Offset 0x3926
    div    bx
    push   ax
    mov    ax, dx
    cwd
    shl    ax, 02h
    mov    cx, 0003h
    div    cx
    mov    cx, ax
    pop    dx
    ret
Func0x30ba ENDP

SetHorizontalPELPanning PROC NEAR       ;Offset 0x30d6
    push   dx
    cli
    call   ResetAttributeIndex          ;Offset 0x4135
    mov    dx, VGA_AttributeControllerIndex;Port 0x3c0
    push   ax
    mov    al, VGA_ATTR_HorizontalPelPanning OR VGA_ATTR_PaletteAddressSource;0x33
    out    dx, al
    pop    ax
    out    dx, al
    sti
    pop    dx
    ret
SetHorizontalPELPanning ENDP

GetHorizontalPELPanning PROC NEAR       ;Offset 0x30e7
    push   dx
    cli
    call   ResetAttributeIndex          ;Offset 0x4135
    mov    dx, VGA_AttributeControllerIndex;Offset 0x3c0
    mov    al, VGA_ATTR_HorizontalPelPanning OR VGA_ATTR_PaletteAddressSource;0x33
    out    dx, al
    inc    dx
    in     al, dx
    sti
    cbw
    pop    dx
    ret
GetHorizontalPELPanning ENDP

GetSetDACPaletteControl PROC NEAR       ;Offset 0x30f8
    push   es
    cmp    bl, 01h
    ja     Label0x310a                  ;Offset 0x310a
    jb     Label0x310f                  ;Offset 0x310f
Label0x3100:                            ;Offset 0x3100
    call   GetDACBitdepth               ;Offset 0x397d
    mov    bh, al
    mov    ax, 004fh
Label0x3108:                            ;Offset 0x3108
    pop    es
    ret
Label0x310a:                            ;Offset 0x310a
    mov    ax, 014fh
    jmp    Label0x3108                  ;Offset 0x3108
Label0x310f:                            ;Offset 0x310f
    push   bx
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   FindModeData                 ;Offset 0x40d9
    cmp    byte ptr es:[bx + 06h], 08h
    pop    bx
    ja     Label0x3127                  ;Offset 0x3127
    mov    al, bh
    call   SetDACBitdepth               ;Offset 0x395b
    jb     Label0x310a                  ;Offset 0x310a
    jmp    Label0x3100                  ;Offset 0x3100
Label0x3127:                            ;Offset 0x3127
    mov    ax, 034fh
    jmp    Label0x3108                  ;Offset 0x3108
GetSetDACPaletteControl ENDP

GetSetPaletteEntries PROC NEAR          ;Offset 0x312c
    test   bl, bl
    je     Label0x3144                  ;Offset 0x3144
    cmp    bl, 01h
    je     Label0x316c                  ;Offset 0x316c
    cmp    bl, 03h
    jbe    Label0x3190                  ;Offset 0x3190
    cmp    bl, 80h
    jne    Label0x318c                  ;Offset 0x318c
    push   dx
    call   WaitUntilVSyncActive         ;Offset 0x4057
    pop    dx
Label0x3144:                            ;Offset 0x3144
    pushad
    push   ds
    push   es
    pop    ds
    mov    si, di
    mov    al, dl
    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
    out    dx, al
    inc    dx
Label0x3152:                            ;Offset 0x3152
    lodsd
    ror    eax, 10h
    out    dx, al
    rol    eax, 08h
    out    dx, al
    rol    eax, 08h
    out    dx, al
    loop   Label0x3152                  ;Offset 0x3152
    pop    ds
    popad
Label0x3168:                            ;Offset 0x3168
    mov    ax, 004fh
    ret
Label0x316c:                            ;Offset 0x316c
    pushad
    mov    al, dl
    mov    dx, VGA_DACStatus            ;Port 0x3c7
    out    dx, al
    inc    dx
    inc    dx
Label0x3176:                            ;Offset 0x3176
    sub    eax, eax
    in     al, dx
    shl    eax, 08h
    in     al, dx
    shl    eax, 08h
    in     al, dx
    stosd
    loop   Label0x3176                  ;Offset 0x3176
    popad
    jmp    Label0x3168                  ;Offset 0x3168
Label0x318c:                            ;Offset 0x318c
    mov    ax, 014fh
    ret
Label0x3190:                            ;Offset 0x3190
    mov    ax, 024fh
    ret
GetSetPaletteEntries ENDP

GetProtectedModeInterface PROC NEAR     ;Offset 0x3194
    mov    di, offset ProtectedModeTable;Offset 0x7de7
    push   cs
    pop    es
    mov    cx, 00a9h
    mov    ax, 004fh
    ret
GetProtectedModeInterface ENDP

GetNearestPixelClock PROC NEAR          ;Offset 0x31a0
    test   bl, bl
    jne    Label0x31ab                  ;Offset 0x31ab
    call   Func0x39ac                   ;Offset 0x39ac
    mov    ax, 004fh
    ret
Label0x31ab:                            ;Offset 0x31ab
    mov    ax, 014fh
    ret
GetNearestPixelClock ENDP

PowerManagement PROC NEAR               ;Offset 0x31af
    mov    ax, 014fh
    cmp    bl, 02h
    ja     Return                       ;Offset 0x31cf
    je     GetDisplayPowerState         ;Offset 0x31c9
    test   bl, bl
    jne    SetDisplayPowerState         ;Offset 0x31c2
    mov    bx, 0710h                    ;BL = Version 1.0, BH = Supports: Standby, Suspend, Off. Not supported: Reduced on
    jmp    Success                      ;Offset 0x31cc
SetDisplayPowerState:                   ;Offset 0x31c2
    call   SetMonitorSleepWakeStatus    ;Offset 0x3afd
    jb     Return                       ;Offset 0x31cf
    jmp    Success                      ;Offset 0x31cc
GetDisplayPowerState:                   ;Offset 0x31c9
    call   GetMonitorSleepWakeStatus    ;Offset 0x3b3b
Success:                                ;Offset 0x31cc
    mov    ax, 004fh
Return:                                 ;Offset 0x31cf
    ret
PowerManagement ENDP

DisplayDataChannel PROC NEAR            ;Offset 0x31d0
    call   EnableVGAExtensions          ;Offset 0x3852
    mov    ah, 01h
    cmp    bl, 01h
    jb     DDCInstallationCheck         ;Offset 0x31e4
    je     ReadEDID                     ;Offset 0x3205
    cmp    bl, 02h
    je     ReadVDIF                     ;Offset 0x3242
Return:                                 ;Offset 0x31e1
    mov    al, 4fh
    ret
DDCInstallationCheck:                   ;Offset 0x31e4
    cli
    sub    bx, bx
    sub    al, al
    call   DDCSendReadCommand           ;Offset 0x3246
    je     Label0x31fa                  ;Offset 0x31fa
    push   cx
    sub    cx, cx
    call   DDCReadByte                  ;Offset 0x335f
    pop    cx
    je     Label0x31fa                  ;Offset 0x31fa
    mov    bx, 0802h
Label0x31fa:                            ;Offset 0x31fa
    sub    ah, ah
    call   DDCSendAck                   ;Offset 0x32d4
    call   DDCShutdown                  ;Offset 0x32b8
    sti
    jmp    Return                       ;Offset 0x31e1
ReadEDID:                               ;Offset 0x3205
    test   dx, dx
    jne    Failure                      ;Offset 0x3233
    mov    si, 0008h                    ;Number of tries
Retry:                                  ;Offset 0x320c
    cli
    mov    cx, 0080h
    sub    al, al
    push   di
    call   DDCSendReadCommand           ;Offset 0x3246
    je     CommFailure                  ;Offset 0x323b
ReadData:                               ;Offset 0x3218
    call   DDCReadByte                  ;Offset 0x335f
    je     CommFailure                  ;Offset 0x323b
    stosb
    loop   ReadData                     ;Offset 0x3218
    pop    di
    call   DDCSendAck                   ;Offset 0x32d4
    call   DDCShutdown                  ;Offset 0x32b8
    sti
    call   DDCVerifyEDIDIntegrity       ;Offset 0x32bc
    je     Success                      ;Offset 0x323e
    call   WaitUntilVSyncActive         ;Offset 0x4057
    dec    si
    jne    Retry                        ;Offset 0x320c
Failure:                                ;Offset 0x3233
    call   DDCShutdown                  ;Offset 0x32b8
    sti
    mov    ah, 01h
    jmp    Return                       ;Offset 0x31e1
CommFailure:                            ;Offset 0x323b
    pop    di
    jmp    Failure                      ;Offset 0x3233
Success:                                ;Offset 0x323e
    sub    ah, ah
    jmp    Return                       ;Offset 0x31e1
ReadVDIF:                               ;Offset 0x3242
    mov    ah, 01h
    jmp    Return                       ;Offset 0x31e1
DisplayDataChannel ENDP

;
;inputs:
;   al = start address
;
DDCSendReadCommand PROC NEAR            ;Offset 0x3246
    call   DDStartup                    ;Offset 0x3267
    je     Failure                      ;Offset 0x3266
    call   DDCSendStartBit              ;Offset 0x3294
    je     Failure                      ;Offset 0x3266
    push   ax
    mov    al, DDC_ADDRESS_A0           ;0xa0
    call   DDCWriteByte                 ;Offset 0x3309
    pop    ax
    je     Failure                      ;Offset 0x3266
    call   DDCWriteByte                 ;Offset 0x3309
    je     Failure                      ;Offset 0x3266
    call   DDCSendStartBit              ;Offset 0x3294
    mov    al, DDC_ADDRESS_A1           ;0xa1
    call   DDCWriteByte                 ;Offset 0x3309
Failure:                                ;Offset 0x3266
    ret
DDCSendReadCommand ENDP

DDStartup PROC NEAR                     ;Offset 0x3267
    pushaw
    call   EnableDDC                    ;Offset 0x3be2
    call   SpeedSensitiveWaitLoop       ;Offset 0x328a
    call   DDCSetClockAndDataTriState   ;Offset 0x3b58
    call   SpeedSensitiveWaitLoop       ;Offset 0x328a
    mov    bx, 0028h                    ;Wait cycles
Label0x3277:                            ;Offset 0x3277
    sub    cx, cx
    call   DDCReadMonitorClock          ;Offset 0x3bb8
    jne    Label0x3286                  ;Offset 0x3286
    call   DDCReadMonitorData           ;Offset 0x3bcd
    jne    Label0x3286                  ;Offset 0x3286
    dec    bx
    jne    Label0x3277                  ;Offset 0x3277
Label0x3286:                            ;Offset 0x3286
    or     cx, bx
    popaw
    ret
DDStartup ENDP

SpeedSensitiveWaitLoop PROC NEAR        ;Offset 0x328a
    push   cx
    mov    cx, 0032h
Label0x328e:                            ;Offset 0x328e
    jmp    Label0x3290                  ;Offset 0x3290
Label0x3290:                            ;Offset 0x3290
    loop   Label0x328e                  ;Offset 0x328e
    pop    cx
    ret
SpeedSensitiveWaitLoop ENDP

DDCSendStartBit PROC NEAR               ;Offset 0x3294
    pushaw
    call      DDCSetClockAndDataTriState;Offset 0x3b58      1   1
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    call      DDCSetClockTriStateAndDataLow;Offset 0x3b88   1   0
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3      0   0
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    sub       cx, cx
    inc       cx
    popaw
    ret
DDCSendStartBit ENDP

DDCShutdown PROC NEAR                   ;Offset 0x32b8
    call      DisableDDC                ;Offset 0x3bf9
    ret
DDCShutdown ENDP

DDCVerifyEDIDIntegrity PROC NEAR        ;Offset 0x32bc
    pushaw
    mov       cx, 0080h
    sub       al, al
Label0x32c2:                            ;Offset 0x32c2
    add       al, byte ptr es:[di]
    inc       di
    loop      Label0x32c2               ;Offset 0x32c2
    test      al, al
    jne       Label0x32d2               ;Offset 0x32d2
    cmp       word ptr es:[di - 80h], 0ff00h
Label0x32d2:                            ;Offset 0x32d2
    popaw
    ret
DDCVerifyEDIDIntegrity ENDP

DDCSendAck PROC NEAR                    ;Offset 0x32d4
    pushaw
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3      0   0
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockTriStateAndDataLow;Offset 0x3b88   1   0
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    call      DDCSetClockAndDataTriState;Offset 0x3b58      1   1
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    popaw
    ret
DDCSendAck ENDP

DDCSyncMonitorClock PROC NEAR           ;Offset 0x32f2
    push      bx
    mov       bx, 0002h
Label0x32f6:                            ;Offset 0x32f6
    sub       cx, cx
Label0x32f8:                            ;Offset 0x32f8
    call      DDCReadMonitorClock       ;Offset 0x3bb8
    jne       Label0x3305               ;Offset 0x3305
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    loop      Label0x32f8               ;Offset 0x32f8
    dec       bx
    jne       Label0x32f6               ;Offset 0x32f6
Label0x3305:                            ;Offset 0x3305
    or        cx, bx
    pop       bx
    ret
DDCSyncMonitorClock ENDP

DDCWriteByte PROC NEAR                  ;Offset 0x3309
    pushaw
    mov       si, 0008h
Label0x330d:                            ;Offset 0x330d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    test      al, 80h
    je        Label0x332a               ;Offset 0x332a
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockAndDataTriState;Offset 0x3b58
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x335b               ;Offset 0x335b
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    jmp       Label0x333b               ;Offset 0x333b
Label0x332a:                            ;Offset 0x332a
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockTriStateAndDataLow;Offset 0x3b88
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x335b               ;Offset 0x335b
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3
Label0x333b:                            ;Offset 0x333b
    shl       al, 01h
    dec       si
    jne       Label0x330d               ;Offset 0x330d
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      DDCSetClockAndDataTriState;Offset 0x3b58
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x335b               ;Offset 0x335b
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
Label0x3357:                            ;Offset 0x3357
    test      cx, cx
    popaw
    ret
Label0x335b:                            ;Offset 0x335b
    sub       cx, cx
    jmp       Label0x3357               ;Offset 0x3357
DDCWriteByte ENDP

DDCReadByte PROC NEAR                   ;Offset 0x335f
    push      bx
    push      cx
    push      dx
    push      si
    sub       bl, bl
    mov       si, 0008h
Label0x3368:                            ;Offset 0x3368
    shl       bl, 01h
    call      DDCSetClockAndDataTriState;Offset 0x3b58
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x33c5               ;Offset 0x33c5
    call      DDCReadMonitorData        ;Offset 0x3bcd
    je        Label0x337d               ;Offset 0x337d
    or        bl, 01h
Label0x337d:                            ;Offset 0x337d
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    dec       si
    jne       Label0x3368               ;Offset 0x3368
    test      dx, dx
    je        Label0x33a9               ;Offset 0x33a9
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockTriStateAndDataLow;Offset 0x3b88
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x33c5               ;Offset 0x33c5
    call      DDCSetClockAndDataLow     ;Offset 0x3ba3
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    jmp       Label0x33c0               ;Offset 0x33c0
Label0x33a9:                            ;Offset 0x33a9
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSetClockAndDataTriState;Offset 0x3b58
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    call      DDCSyncMonitorClock       ;Offset 0x32f2
    je        Label0x33c5               ;Offset 0x33c5
    call      DDCSetClockLowAndDataTriState;Offset 0x3b6d
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
Label0x33c0:                            ;Offset 0x33c0
    call      SpeedSensitiveWaitLoop    ;Offset 0x328a
    mov       al, bl
Label0x33c5:                            ;Offset 0x33c5
    test      cx, cx
    pop       si
    pop       dx
    pop       cx
    pop       bx
    ret
DDCReadByte ENDP

Func0x33cc PROC FAR                     ;Offset 0x33cc
    cmp       ah, 4fh
    jne       Label0x33e6               ;Offset 0x33e6
    push      si
    push      ds
    call      GetBIOSDataSelectorInDS   ;Offset 0x3f40
    cmp       al, 16h
    jae       Label0x33e7               ;Offset 0x33e7
    cbw
    shl       ax, 01h
    mov       si, ax
    call      word ptr cs:[si + VESACallTable];Offset 0x2a10
Label0x33e4:                            ;Offset 0x33e4
    pop       ds
    pop       si
Label0x33e6:                            ;Offset 0x33e6
    retf
Label0x33e7:                            ;Offset 0x33e7
    mov       ax, 0100h
    jmp       Label0x33e4               ;Offset 0x33e4
Func0x33cc ENDP

Func0x33ec PROC FAR                     ;Offset 0x33ec
    push      ds
    push      es
    mov       ds, word ptr cs:[BIOSSegment];Offset 0x77e6
    call      GetBIOSDataSelectorInES   ;Offset 0x3f46
    mov       word ptr es:[INT_1F_HandlerOfs], offset GraphicsFont8x8;Offset 0x7c Offset 0x4f3a
    mov       word ptr es:[INT_1F_HandlerSeg], ds;Offset 0x7e
    mov       word ptr es:[INT_43_HandlerOfs], offset Font8x8;Offset 0x10c Offset 0x4b3a
    mov       word ptr es:[INT_43_HandlerSeg], ds;Offset 0x10e
    mov       word ptr es:[BDA_VideoParameterControlBlockPtrOfs], offset VideoOverrideTable;Offset 0x4a8 Offset 0x776b
    mov       word ptr es:[BDA_VideoParameterControlBlockPtrSeg], ds;Offset 0x4aa
    mov       word ptr [VideoOverrideTable.VideoParameterTable.sgm], ds;Offset 0x776d
    mov       word ptr [VideoOverrideTable.SecondarySavePointerTable.sgm], ds;Offset 0x777d
    mov       word ptr [SecondarySaveTable.DisplayCombinationCodeTable.sgm], ds;Offset 0x778b
    mov       word ptr [SecondarySaveTable.Reserved3.sgm], ds;Offset 0x779f
    cmp       word ptr [Data0x77ea.sgm], 0000h;Offset 0x77ec
    je        Label0x3435               ;Offset 0x3435
    mov       word ptr [Data0x77ea.sgm], ds;Offset 0x77ec
Label0x3435:                            ;Offset 0x3435
    mov       word ptr [Data0x77ee.sgm], ds;Offset 0x77f0
    mov       word ptr [Data0x1ff3.sgm], ds;Offset 0x1ff5
    mov       word ptr [Data0x1ff7.sgm], ds;Offset 0x1ff9
    mov       word ptr [Data0x1ffb.sgm], ds;Offset 0x1ffd
    mov       word ptr [Data0x1fff.sgm], ds;Offset 0x2001
    mov       word ptr [Data0x2003.sgm], ds;Offset 0x2005
    mov       word ptr [Data0x2007.sgm], ds;Offset 0x2009
    mov       word ptr [Data0x200b.sgm], ds;Offset 0x200d
    mov       word ptr [Data0x200f.sgm], ds;Offset 0x2011
    mov       word ptr [Data0x2013.sgm], ds;Offset 0x2015
    mov       word ptr [Data0x2017.sgm], ds;Offset 0x2019
    mov       word ptr [Data0x201b.sgm], ds;Offset 0x201d
    mov       word ptr [Data0x201f.sgm], ds;Offset 0x2021
    mov       word ptr [BIOSSegment], ds;Offset 0x77e6
    mov       word ptr [Data0x1feb.sgm], es;Offset 0x1fed
    mov       word ptr [Data0x1fef.sgm], es;Offset 0x1ff1
    call      Func0x3af7                ;Offset 0x3af7
    pop       es
    pop       ds
    retf
Func0x33ec ENDP

                        DB 000h

Func0x347c PROC NEAR                    ;Offset 0x347c
    pushad
    mov       ax, 0b102h                ;Find PCI Device
    sub       si, si
    mov       cx, PCI_DEVICE_Banshee    ;0x0003
    mov       dx, PCI_VENDOR_3Dfx       ;0x121a
    call      Call1AHandlerSafely       ;Offset 0x4116
    jb        Label0x34f6               ;Offset 0x34f6
    test      ah, ah
    jne       Label0x34f6               ;Offset 0x34f6
    mov       ax, 0b10bh                ;Write Configuration Byte
    mov       di, TDFX_PCI_AGPCommand + 1;0x5d AGP Command byte 1
    mov       cl, 03h                   ;AGP enable, Sideband addressing enable
    int       1ah
    mov       ax, 0b10bh                ;Write Configuration Byte
    mov       di, TDFX_PCI_AGPCommand + 1;0x5d AGP Command byte 1
    xor       cl, cl                    ;AGP disable, Sideband addressing disable
    int       1ah
    mov       ax, 0b10ah                ;Read Configuration Dword
    mov       di, PCI_H0_DWord_BaseAddress2;0x18 IO base address
    int       1ah
    test      ah, ah
    jne       Label0x34f6               ;Offset 0x34f6
    and       cx, 0fffeh
    mov       dx, cx
    add       dx, TDFX_IO_Unknown38     ;0x38
    in        eax, dx
    xor       edi, edi
    test      al, 08h                   ;Something about waitstates?
    je        Label0x34c7               ;Offset 0x34c7
    or        di, TDFX_PI_PCIReadWaitState2Cycle OR TDFX_PI_PCIWriteWaitState2Cycle;0x300
Label0x34c7:                            ;Offset 0x34c7
    mov       dx, cx
    add       dx, TDFX_IO_PCIInit       ;0x4
    mov       eax, dword ptr cs:[PCIInitDefault];Offset 0x7cf9
    or        eax, edi
    out       dx, eax
    mov       dx, cx
    add       dx, TDFX_IO_VGAInit0      ;0x28
    mov       eax, TDFX_VI0_WakeUpSelect OR TDFX_VI0_EnableVGAExtensions;0x140
    out       dx, eax
    mov       dx, cx
    add       dx, TDFX_IO_MiscInit1     ;0x14
    in        eax, dx
    test      eax, TDFX_MI1_PCIDeviceType;0x10000000
    jne       Label0x34f6               ;Offset 0x34f6
    clc
Label0x34f3:                            ;Offset 0x34f3
    popad
    ret
Label0x34f6:                            ;Offset 0x34f6
    mov       dx, cx
    add       dx, TDFX_IO_VGAInit0      ;0x28
    mov       eax, TDFX_VI0_EnableVGAExtensions OR TDFX_VI0_WakeUpSelect OR TDFX_VI0_LegacyVGAMemoryAndIODisable;0x340
    out       dx, eax
    stc
    jmp       Label0x34f3               ;Offset 0x34f3
Func0x347c ENDP

Func0x3506 PROC NEAR                    ;Offset 0x3506
    pushaw
    mov       dx, VGA_VideoSubsystemEnable;0x3c3
    mov       al, 01h                   ;Video subsystem enable
    out       dx, al
    mov       dx, 004ah                 ;EISA PIT2?
    sub       ax, ax
    out       dx, ax
    mov       dx, VGA_MiscellaneousWrite;Port 0x3c2
    mov       al, 67h                   ;0x3dX ports, enable ram mapping, 28.322 MHz pixel clock, high page ram, negative hsync, positive vsync
    out       dx, al
    mov       dl, VGA_GraphicsControllerIndex_lowbyte;Port 0x3ce
    mov       ax, VGA_GCTLIdx_Miscellaneous OR ((VGA_GCTL6_GfxMode OR VGA_GCTL6_Mem_A0000_AFFFF) SHL 8);0x506
    out       dx, ax
    call      EnableVGAExtensions       ;Offset 0x3852
    call      CacheIOBaseAddress        ;Offset 0x3d14
    call      LoadDefaultRegisterValues ;Offset 0x3d78
    popaw
    ret
Func0x3506 ENDP

Func0x352a PROC NEAR                    ;Offset 0x352a
    ret
Func0x352a ENDP

EnableVideoSubsystem PROC NEAR          ;Offset 0x352b
    push      ax
    push      dx
    mov       dx, VGA_VideoSubsystemEnable;Port 0x3c3
    mov       al, 01h                   ;Video subsystem enable
    out       dx, al
    pop       dx
    pop       ax
    ret
EnableVideoSubsystem ENDP

DisableVideoSubsystem PROC NEAR         ;Offset 0x3536
    push      ax
    push      dx
    mov       dx, VGA_VideoSubsystemEnable;Port 0x3c3
    sub       al, al                    ;Video subsystem disable
    out       dx, al
    pop       dx
    pop       ax
    ret
DisableVideoSubsystem ENDP

Func0x3541 PROC NEAR                    ;Offset 0x3541
    ret
Func0x3541 ENDP

PrintNameAndMemorySize PROC NEAR        ;Offset 0x3542
    pushaw
    sub       bh, bh
    mov       bl, byte ptr es:[DefaultAttribute];Offset 0x77f4
    mov       ax, 1301h                 ;Write string, update cursor
    mov       dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       bp, offset ThreeDfxName   ;Offset 0x77f7
    mov       cx, word ptr es:[bp - 02h];ThreeDfxNameLength
    int       10h
    mov       dx, word ptr ds:[DBA_CursorPositionPage0];Offset 0x450
    mov       ax, 1301h                 ;Write string, update cursor
    mov       cx, 0013h                 ;Length
    mov       bp, offset MemorySizeMessage ;Offset 0x7cc9
    int       10h
    call      GetMemorySizeIn256KBlocks ;Offset 0x37fc
    mov       bp, offset Mem4096K       ;Offset 0x7cef
Label0x356f:                            ;Offset 0x356f
    cmp       byte ptr es:[bp], 00h
    je        Label0x3596               ;Offset 0x3596
    cmp       byte ptr es:[bp], al
    je        Label0x3581               ;Offset 0x3581
    add       bp, 0003h
    jmp       Label0x356f               ;Offset 0x356f
Label0x3581:                            ;Offset 0x3581
    inc       bp
    mov       bp, word ptr es:[bp]
    mov       ah, 03h                   ;Get cursor position
    int       10h
    mov       ax, 1301h                 ;Write string, update cursor
    mov       cl, byte ptr es:[bp]
    sub       ch, ch
    inc       bp
    int       10h
Label0x3596:                            ;Offset 0x3596
    mov       ax, 0e0dh                 ;Teletype output, write 0dh
    int       10h
    mov       ax, 0e0ah                 ;Teletype output, write 0ah
    int       10h
    mov       ax, 0e0ah                 ;Teletype output, write 0ah
    int       10h
    popaw
    ret
PrintNameAndMemorySize ENDP

Func0x35a7 PROC NEAR                    ;Offset 0x35a7
    push      bx
    push      si
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    cmp       al, BDA_DM_320x200_256_Color_Graphics;0x13
    jbe       Label0x35c1               ;Offset 0x35c1
    call      FindModeData              ;Offset 0x40d9
    call      Func0x3f29                ;Offset 0x3f29
    mov       al, byte ptr es:[si + 03h]
    test      al, al
    jne       Label0x35c4               ;Offset 0x35c4
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
Label0x35c1:                            ;Offset 0x35c1
    call      Func0x35d4                ;Offset 0x35d4
Label0x35c4:                            ;Offset 0x35c4
    les       di, ds:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les       di, es:[di]               ;BDA_VPCB_VideoParameterTable
    cbw
    shl       ax, 06h
    add       di, ax
    pop       si
    pop       bx
    ret
Func0x35a7 ENDP

Func0x35d4 PROC NEAR                    ;Offset 0x35d4
    push      bx
    push      es
    cmp       al, 07h
    ja        Label0x3605               ;Offset 0x3605
    je        Label0x35fa               ;Offset 0x35fa
    cmp       al, 03h
    ja        Label0x360e               ;Offset 0x360e
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode200;Offset 0x80
    jne       Label0x360e               ;Offset 0x360e
    mov       bl, al
    add       al, 13h
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    je        Label0x360e               ;Offset 0x360e
    mov       al, bl
    shr       al, 01h
    add       al, 17h
    jmp       Label0x360e               ;Offset 0x360e
Label0x35fa:                            ;Offset 0x35fa
    test      byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489 0x10
    je        Label0x360e               ;Offset 0x360e
    mov       al, 19h
    jmp       Label0x360e               ;Offset 0x360e
Label0x3605:                            ;Offset 0x3605
    call      FindModeData              ;Offset 0x40d9
    jb        Label0x3611               ;Offset 0x3611
    mov       al, byte ptr es:[bx + 01h]
Label0x360e:                            ;Offset 0x360e
    pop       es
    pop       bx
    ret
Label0x3611:                            ;Offset 0x3611
    sub       al, al
    jmp       Label0x360e               ;Offset 0x360e
Func0x35d4 ENDP

Func0x3615 PROC NEAR                    ;Offset 0x3615
    push      eax
    push      dx
    call      EnableVGAExtensions       ;Offset 0x3852
    call      GetIOBaseAddress          ;Offset 0x3d69
    add       dx, TDFX_IO_VGAInit0      ;0x28
    in        eax, dx
    and       eax, TDFX_VI0_WakeUpSelect;0x100
    or        al, TDFX_VI0_EnableVGAExtensions;0x40
    out       dx, eax
    sub       dx, dx
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
    mov       dx, VGA_MiscellaneousWrite;Port 0x3c2
    mov       al, 67h                   ;IO address select, Enable RAM, Clock 28.322 MHz, High page ram, negative hsync, positive vsync
    out       dx, al
    pop       dx
    pop       eax
    ret
Func0x3615 ENDP

Func0x363c PROC NEAR                    ;Offset 0x363c
    pushad
    push      es
    mov       ax, 0b102h                ;Find PCI Device
    sub       si, si
    mov       cx, PCI_DEVICE_Banshee    ;0003h
    mov       dx, PCI_VENDOR_3Dfx       ;0x121a
    int       1ah
    mov       ax, 0b109h                ;Read Configuration Word
    mov       di, PCI_Header_Word_Command;0x4
    int       1ah
    and       cl, NOT PCI_Cmd_VGAPaletteSnoop;0dfh
    mov       ax, 0b10ch                ;Write Configuration Word
    int       1ah
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    jb        Label0x373c               ;Offset 0x373c
    mov       ax, word ptr es:[bx + 0bh]
    push      ax
    movzx     eax, word ptr es:[bx + 09h]
    shl       eax, 0ch
    push      eax
    mov       cx, word ptr es:[bx + 07h]
    and       cx, 0fffh
    or        ax, cx
    push      eax
    call      Func0x3f29                ;Offset 0x3f29
    mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov       ax, TDFX_CRTC_HorizontalExtension;0x1a
    out       dx, ax
    inc       ax
    mov       ah, byte ptr es:[si + 01h]
    out       dx, ax                    ;Horizontal Extension Register
    mov       al, TDFX_CRTC_ExtensionByte1;0x1d
    call      ReadIndexedRegister       ;Offset 0x3f84
    and       ah, 7fh
    out       dx, ax
    call      GetIOBaseAddress          ;Offset 0x3d69
    mov       di, dx
    add       dx, TDFX_IO_PLLControl0   ;0x40
    sub       eax, eax
    mov       ax, word ptr es:[si + 04h]
    test      ax, ax
    je        Label0x36b2               ;Offset 0x36b2
    out       dx, eax
Label0x36b2:                            ;Offset 0x36b2
    movzx     ecx, byte ptr es:[si]
    lea       dx, [di + TDFX_IO_DACMode];0x4c
    sub       eax, eax
    out       dx, eax
    lea       dx, [di + TDFX_IO_VideoDesktopStartAddress];0xe4
    sub       eax, eax
    out       dx, eax
    pop       eax
    pop       edx
    test      cl, 04h
    je        Label0x36d4               ;Offset 0x36d4
    add       eax, edx
Label0x36d4:                            ;Offset 0x36d4
    lea       dx, [di + TDFX_IO_VideoScreenSize];0x98
    out       dx, eax
    sub       eax, eax
    pop       ax
    lea       dx, [di + TDFX_IO_VideoDesktopOverlayStride];0xe8
    out       dx, eax
    mov       bl, cl
    lea       dx, [di + TDFX_IO_VGAInit0];0x28
    in        eax, dx
    and       eax, 00000100h
    and       bl, 01h
    shl       bx, 0ch
    or        bx, 0040h
    or        ax, bx
    out       dx, eax
    lea       dx, [di + TDFX_IO_VGAInit1];0x2c
    mov       eax, ecx
    and       eax, 00000001h
    shl       eax, 14h
    out       dx, eax
    lea       dx, [di + TDFX_IO_VideoProcessorConfig];0x5c
    mov       eax, ecx
    and       al, 38h
    shl       eax, 0fh
    test      cl, 01h
    je        Label0x3731               ;Offset 0x3731
    or        al, 81h
    test      cl, 38h
    je        Label0x3729               ;Offset 0x3729
    or        ah, 0ch
Label0x3729:                            ;Offset 0x3729
    and       cl, 06h
    shl       cl, 02h
    or        al, cl
Label0x3731:                            ;Offset 0x3731
    out       dx, eax
    lea       dx, [di + TDFX_IO_DRamInit1];0x1c
    in        eax, dx
    or        al, 01h
    out       dx, eax
Label0x373c:                            ;Offset 0x373c
    pop       es
    popad
    ret
Func0x363c ENDP

ClearScreen PROC NEAR                   ;Offset 0x3740
    pushaw
    push      eax
    push      es
    mov       al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call      FindModeData              ;Offset 0x40d9
    jb        Label0x37b0               ;Offset 0x37b0
    sub       di, di
    mov       ax, 0720h
    mov       cx, 4000h
    mov       dx, word ptr cs:[B800Selector];Offset 0x77e4
    mov       bl, byte ptr es:[bx + 04h]
    cmp       bl, 00h
    je        Label0x3778               ;Offset 0x3778
    mov       dx, word ptr cs:[B000Selector];Offset 0x77e2
    cmp       bl, 01h
    je        Label0x3778               ;Offset 0x3778
    cmp       bl, 02h
    jne       Label0x377e               ;Offset 0x377e
    mov       dx, word ptr cs:[B800Selector];Offset 0x77e4
    sub       ax, ax
Label0x3778:                            ;Offset 0x3778
    mov       es, dx
    rep stosw
    jmp       Label0x37b0               ;Offset 0x37b0
Label0x377e:                            ;Offset 0x377e
    mov       ax, 01h
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    jbe       Label0x3793               ;Offset 0x3793
    call      GetMemorySizeIn256KBlocks ;Offset 0x37fc
    cmp       bl, 05h
    je        Label0x3793               ;Offset 0x3793
    shl       ax, 02h
Label0x3793:                            ;Offset 0x3793
    mov       si, ax
    mov       ax, word ptr cs:[A000Selector];Offset 0x77e0
    mov       es, ax
    mov       ax, di
    mov       dx, di
Label0x379f:                            ;Offset 0x379f
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
    mov       cx, 8000h
    rep stosw
    inc       dx
    dec       si
    jne       Label0x379f               ;Offset 0x379f
    sub       dx, dx
    call      SetVGAReadWriteAperture64K;Offset 0x3c10
Label0x37b0:                            ;Offset 0x37b0
    pop       es
    pop       eax
    popaw
    ret
ClearScreen ENDP

Func0x37b5 PROC NEAR                    ;Offset 0x37b5
    pushaw
    push      ds
    push      es
    sub       dx, dx
    mov       ds, dx
    mov       cl, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    sub       cl, bh
    jbe       Label0x37f3               ;Offset 0x37f3
    call      Func0x2267                ;Offset 0x2267
    sub       ch, ch
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, VGA_GCTLIdx_ReadMapSelect OR (VGA_GCTL4_Map2 SHL 8);0x204
    out       dx, ax
    mov       dx, 0020h
    sub       dx, cx
    mov       al, bh
    cbw
    add       di, ax
    mov       si, 0100h
    mov       bl, cl
Label0x37df:                            ;Offset 0x37df
    mov       al, byte ptr es:[di - 01h]
    mov       cl, bl
    rep stosb
    add       di, dx
    dec       si
    jne       Label0x37df               ;Offset 0x37df
    mov       dx, VGA_GraphicsControllerIndex;Port 0x3ce
    mov       ax, 0004h
    out       dx, ax
Label0x37f3:                            ;Offset 0x37f3
    pop       es
    pop       ds
    popaw
    ret
Func0x37b5 ENDP

Func0x37f7 PROC NEAR                    ;Offset 0x37f7
    ret
Func0x37f7 ENDP

;Offset 0x37f8
Data0x37f8              DB 010h, 020h, 020h, 040h

GetMemorySizeIn256KBlocks PROC NEAR     ;Offset 0x37fc
    push      bx
    push      dx
    call      GetIOBaseAddress          ;Offset 0x3d69
    add       dx, TDFX_IO_DRamInit0     ;0x18
    in        eax, dx
    shr       eax, 1ah                  ;26
    and       ax, 0003h                 ;26-27 : number of chipsets, size of chips
    mov       bx, ax
    mov       al, byte ptr cs:[bx + Data0x37f8];Offset 0x37f8
    pop       dx
    pop       bx
    ret
GetMemorySizeIn256KBlocks ENDP

GetLinearFrameBuffer PROC NEAR          ;Offset 0x3817
    sub       eax, eax
    cmp       byte ptr [si + 06h], 08h
    jb        Label0x3823               ;Offset 0x3823
    call      GetLinearFrameBufferAddress;Offset 0x3e1c
Label0x3823:                            ;Offset 0x3823
    ret
GetLinearFrameBuffer ENDP

Func0x3824 PROC NEAR                    ;Offset 0x3824
    push      ax
    push      dx
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    mov       al, TDFX_CRTC_ExtensionByte1;0x1d
    call      ReadIndexedRegister       ;Offset 0x3f84
    or        ah, 80h
    out       dx, ax
    pop       dx
    pop       ax
    ret
Func0x3824 ENDP

Func0x3835 PROC NEAR                    ;Offset 0x3835
    push      ax
    push      dx
    cmp       byte ptr ds:[BDA_DisplayMode], BDA_DM_320x200_256_Color_Graphics;Offset 0x449 0x13
    jbe       Label0x384e               ;Offset 0x384e
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    mov       al, TDFX_CRTC_ExtensionByte1;0x1d
    call      ReadIndexedRegister       ;Offset 0x3f84
    test      ah, 80h
    je        Label0x384e               ;Offset 0x384e
    stc
    jmp       Label0x384f               ;Offset 0x384f
Label0x384e:                            ;Offset 0x384e
    clc
Label0x384f:                            ;Offset 0x384f
    pop       dx
    pop       ax
    ret
Func0x3835 ENDP

EnableVGAExtensions PROC NEAR           ;Offset 0x3852
    push      ax
    push      dx
    call      GetIOBaseAddress          ;Offset 0x3d69
    add       dx, TDFX_IO_VGAInit0      ;0x28
    in        eax, dx
    or        al, TDFX_VI0_EnableVGAExtensions;0x40
    out       dx, eax
    pop       dx
    pop       ax
    ret
EnableVGAExtensions ENDP

;Check to see if there's enough memory for the mode.
Func0x3863 PROC NEAR                    ;Offset 0x3863
    pushaw
    push      es
    call      FindModeData              ;Offset 0x40d9
    jb        Label0x3886               ;Offset 0x3886
    mov       ax, word ptr es:[bx + 0bh];Bytes per scanline
    mul       word ptr es:[bx + 09h]    ;Height in pixels or chars
    mov       cx, 8000h
    div       cx
    add       dx, 0ffffh
    adc       ax, 0000h
    xchg      ax, dx
    call      GetMemorySizeIn256KBlocks ;Offset 0x37fc
    shl       ax, 03h
    cmp       ax, dx
Label0x3886:                            ;Offset 0x3886
    pop       es
    popaw
    ret
Func0x3863 ENDP

Func0x3889 PROC NEAR                    ;Offset 0x3889
    ret
Func0x3889 ENDP

Func0x388a PROC NEAR                    ;Offset 0x388a
    ret
Func0x388a ENDP

Func0x388b PROC NEAR                    ;Offset 0x388b
    ret
Func0x388b ENDP

Func0x388c PROC NEAR                    ;Offset 0x388c
    ret
Func0x388c ENDP

Func0x388d PROC NEAR                    ;Offset 0x388d
    mov       al, 08h
    ret
Func0x388d ENDP

Func0x3890 PROC NEAR                    ;Offset 0x3890
    push      eax
    push      bx
    push      dx
    call      VideoProcessorStatus      ;Offset 0x3e53
    jne       Label0x38a5               ;Offset 0x38a5
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    mov       bx, ax
    mov       ah, al
    mov       al, VGA_CRTCIdx_Offset    ;0x13
    out       dx, ax
    jmp       Label0x38bd               ;Offset 0x38bd
Label0x38a5:                            ;Offset 0x38a5
    shl       ax, 03h
    mov       bx, ax
    call      GetIOBaseAddress          ;Offset 0x3d69
    add       dx, TDFX_IO_VideoDesktopOverlayStride;0xe8
    in        eax, dx
    and       eax, 0ffff8000h           ;reset overlay stride to 0
    or        ax, bx
    out       dx, eax
Label0x38bd:                            ;Offset 0x38bd
    pop       dx
    pop       bx
    pop       eax
    ret
Func0x3890 ENDP

Func0x38c2 PROC NEAR                    ;Offset 0x38c2
    push      dx
    call      VideoProcessorStatus      ;Offset 0x3e53
    jne       Label0x38d6               ;Offset 0x38d6
    call      GetCRTControllerIndexPort ;Offset 0x40aa
    mov       al, 13h
    call      ReadIndexedRegister       ;Offset 0x3f84
    mov       al, ah
    sub       ah, ah
    jmp       Label0x38ee               ;Offset 0x38ee
Label0x38d6:                            ;Offset 0x38d6
    push      eax
    call      GetIOBaseAddress          ;Offset 0x3d69
    add       dx, TDFX_IO_VideoDesktopOverlayStride;0xe8
    in        eax, dx
    and       eax, 7fffh
    shr       ax, 03h
    xchg      ax, dx
    pop       eax
    xchg      ax, dx
Label0x38ee:                            ;Offset 0x38ee
    pop       dx
    ret
Func0x38c2 ENDP

Func0x38f0 PROC NEAR                    ;Offset 0x38f0
    ret
Func0x38f0 ENDP

Func0x38f1 PROC NEAR                    ;Offset 0x38f1
    push   bx
    call   VideoProcessorStatus         ;Offset 0x3e53
    jne    Label0x3907                  ;Offset 0x3907
    mov    bh, dl
    mov    bl, al
    call   GetCRTControllerIndexPort    ;Offset 0x40aa
    mov    al, 0ch
    out    dx, ax
    inc    ax
    mov    ah, bl
    out    dx, ax
    jmp    Label0x3924                  ;Offset 0x3924
Label0x3907:                            ;Offset 0x3907
    push   eax
    push   dx
    shl    eax, 10h
    mov    ax, dx
    ror    eax, 10h
    shl    eax, 02h
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoDesktopStartAddress;0xe4
    out    dx, eax
    pop    dx
    pop    eax
Label0x3924:                            ;Offset 0x3924
    pop    bx
    ret
Func0x38f1 ENDP

GetStartAddress PROC NEAR               ;Offset 0x3926
    push   bx
    call   VideoProcessorStatus         ;Offset 0x3e53
    jne    Label0x3942                  ;Offset 0x3942
    call   GetCRTControllerIndexPort    ;Offset 0x40aa
    mov    al, VGA_CRTCIdx_StartAddrHigh;0xc
    call   ReadIndexedRegister          ;Offset 0x3f84
    mov    bh, ah
    inc    ax                           ;VGA_CRTCIdx_StartAddrLow
    call   ReadIndexedRegister          ;Offset 0x3f84
    mov    bl, ah
    sub    dx, dx
    mov    ax, bx
    jmp    Label0x3959                  ;Offset 0x3959
Label0x3942:                            ;Offset 0x3942
    push   eax
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoDesktopStartAddress;0xe4
    in     eax, dx
    shr    eax, 02h
    mov    bx, ax
    mov    dx, ax
    pop    eax
    mov    ax, bx
Label0x3959:                            ;Offset 0x3959
    pop    bx
    ret
GetStartAddress ENDP

;
;input:
;   al = DAC bitdepth (8 or 6)
;
SetDACBitdepth PROC NEAR                ;Offset 0x395b
    cmp    al, 06h
    jb     Label0x397b                  ;Offset 0x397b
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit0         ;0x28
    cmp    al, 08h
    in     eax, dx
    jb     Label0x3972                  ;Offset 0x3972
    or     al, TDFX_VI_DACBitDepth8     ;0x4
    jmp    Label0x3974                  ;Offset 0x3974
Label0x3972:                            ;Offset 0x3972
    and    al, NOT TDFX_VI_DACBitDepth8 ;0xfb
Label0x3974:                            ;Offset 0x3974
    out    dx, eax
    pop    dx
    pop    eax
    clc
    ret
Label0x397b:                            ;Offset 0x397b 
    stc
    ret
SetDACBitdepth ENDP

GetDACBitdepth PROC NEAR                ;Offset 0x397d
    push   bx
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit0         ;0x28
    push   eax
    in     eax, dx
    mov    bl, 06h
    test   al, TDFX_VI0_DACBitDepth     ;0x4
    je     Is6Bit                       ;Offset 0x3991
    mov    bl, 08h
Is6Bit:                                 ;Offset 0x3991
    pop    eax
    mov    al, bl
    pop    dx
    pop    bx
    ret
GetDACBitdepth ENDP

GetMaxPixelClockHz PROC NEAR            ;Offset 0x3998
    mov    al, bl
    call   Func0x3faf                   ;Offset 0x3faf
    mov    eax, 02625a00h               ;40,000,000
    je     Label0x39ab                  ;Offset 0x39ab
    mov    eax, 09896800h               ;160,000,000
Label0x39ab:                            ;Offset 0x39ab
    ret
GetMaxPixelClockHz ENDP

Func0x39ac PROC NEAR                    ;Offset 0x39ac
    push   eax
    push   ebx
    push   esi
    push   edi
    push   ebp
    mov    eax, ecx
    call   Func0x3e83                   ;Offset 0x3e83
    call   Func0x3f19                   ;Offset 0x3f19
    mov    ecx, eax
    pop    ebp
    pop    edi
    pop    esi
    pop    ebx
    pop    eax
    ret
Func0x39ac ENDP

Func0x39cd PROC NEAR                    ;Offset 0x39cd
    pushad
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    mov    ax, VGA_CRTCIdx_VertRetraceEnd OR (VGA_VRE_DisableVertRetrIntr SHL 8);0x2011
    out    dx, ax
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   Func0x417c                   ;Offset 0x417c
    cmp    al, BDA_DM_Unknown1          ;0x8
    jb     Label0x3af4                  ;Offset 0x3af4
    mov    eax, dword ptr es:[di + 0dh]
    call   SetAndSelectVideoPLL         ;Offset 0x3e64
    mov    ax, word ptr es:[di]
    shr    ax, 03h
    sub    ax, 05h
    mov    ch, ah
    and    ch, 01h
    shl    ax, 08h
    out    dx, ax
    mov    bh, ah
    add    bh, 03h
    mov    al, 03h
    call   ReadIndexedRegister          ;Offset 0x3f84
    push   bx
    and    bh, 1fh
    and    ah, 0e0h
    or     ah, bh
    out    dx, ax
    mov    ax, word ptr es:[di + 02h]
    shr    ax, 03h
    bt     ax, 08h
    setb   cl
    shl    cl, 06h
    or     ch, cl
    mov    ah, al
    mov    al, 04h
    out    dx, ax
    mov    bx, word ptr es:[di + 04h]
    shr    bx, 03h
    mov    al, 05h
    call   ReadIndexedRegister          ;Offset 0x3f84
    and    ah, 60h
    and    bl, 1fh
    or     ah, bl
    pop    bx
    and    bh, 20h
    shl    bh, 02h
    or     ah, bh
    out    dx, ax
    mov    al, 1ah
    call   ReadIndexedRegister          ;Offset 0x3f84
    and    ah, 3ch
    or     ah, ch
    out    dx, ax
    mov    ax, word ptr es:[di + 06h]
    dec    ax
    dec    ax
    mov    ch, ah
    and    ch, 01h
    bt     ax, 09h
    setb   cl
    shl    cl, 05h
    or     ch, cl
    mov    bh, ah
    and    bh, 04h
    shr    bh, 02h
    mov    ah, al
    mov    al, 06h
    out    dx, ax
    mov    al, 16h
    sub    ah, 02h
    out    dx, ax
    mov    ax, word ptr es:[di + 08h]
    bt     ax, 08h
    setb   cl
    shl    cl, 02h
    or     ch, cl
    bt     ax, 09h
    setb   cl
    shl    cl, 07h
    or     ch, cl
    bt     ax, 0ah
    setb   cl
    shl    cl, 06h
    or     bh, cl
    mov    ah, al
    mov    al, 10h
    out    dx, ax
    mov    al, 07h
    call   ReadIndexedRegister          ;Offset 0x3f84
    and    ah, 5ah
    or     ah, ch
    out    dx, ax
    mov    al, 1bh
    call   ReadIndexedRegister          ;Offset 0x3f84
    and    ah, 3ch
    or     ah, bh
    out    dx, ax
    mov    ah, byte ptr es:[di + 0ah]
    and    ah, 0fh
    or     ah, 20h
    mov    al, 11h
    out    dx, ax
    mov    ah, byte ptr es:[di + 0ch]
    and    ah, 0ch
    shl    ah, 04h
    mov    dl, 0cch
    in     al, dx
    and    al, 3fh
    or     al, ah
    mov    dl, 0c2h
    out    dx, al
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoProcessorConfig;0x5c
    in     eax, dx
    mov    bl, byte ptr es:[di + 0ch]
    and    bl, 02h
    shl    bl, 02h
    out    dx, eax
Label0x3af4:                            ;Offset 0x3af4
    popad
    ret
Func0x39cd ENDP

Func0x3af7 PROC NEAR                    ;Offset 0x3af7
    ret
Func0x3af7 ENDP

;Offset 0x3af8
MonitorState            DB TDFX_DM_VSyncDPMSDisable OR TDFX_DM_HSyncDPMSDisable;0x0
                        DB TDFX_DM_VSyncDPMSDisable OR TDFX_DM_HSyncDPMSEnable;0x8
                        DB TDFX_DM_VSyncDPMSEnable OR TDFX_DM_HSyncDPMSDisable;0x2
                        DB TDFX_DM_VSyncDPMSDisable OR TDFX_DM_HSyncDPMSDisable;0x0 - Not used
                        DB TDFX_DM_VSyncDPMSEnable OR TDFX_DM_HSyncDPMSEnable;0xA

;
;inputs:
;   bh = requested state: 0 = on, 1 = standby, 2 = suspend, 4 = 0ff
;
SetMonitorSleepWakeStatus PROC NEAR     ;Offset 0x3afd
    cmp    bh, 02h
    jbe    Label0x3b07                  ;Offset 0x3b07
    cmp    bh, 04h                      ;filter out 3
    jne    Label0x3b31                  ;Offset 0x3b31
Label0x3b07:                            ;Offset 0x3b07
    push   eax
    push   ebx
    push   dx
    call   TurnScreenOff                ;Offset 0x3f6a
    movzx  bx, bh
    test   bx, bx
    jne    Label0x3b19                  ;Offset 0x3b19
    call   TurnScreenOn                 ;Offset 0x3f77
Label0x3b19:                            ;Offset 0x3b19
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_DACMode          ;0x4c
    in     eax, dx
    and    al, NOT (TDFX_DM_VSyncDPMS OR TDFX_DM_VSyncValue OR TDFX_DM_HSyncDPMS OR TDFX_DM_HSyncValue);0xe1
    or     al, byte ptr cs:[bx + MonitorState];Offset 0x3af8
    out    dx, eax
    pop    dx
    pop    ebx
    pop    eax
    clc
    ret
Label0x3b31:                            ;Offset 0x3b31 
    stc
    ret
SetMonitorSleepWakeStatus ENDP

;Offset 0x3b33
Data0x3b33              DB 000h         ;000    VSyncValue has no effect.
                        DB 002h         ;001    Result is the same if VSyncValue = 0 or 1
                        DB 000h         ;010    So: VSyncDPMS HSyncDPMS     Value   Meaning
                        DB 002h         ;011            0       0           000h    On
                        DB 001h         ;100            0       1           001h    Standby
                        DB 004h         ;101            1       0           002h    Suspend
                        DB 001h         ;110            1       1           004h    Off
                        DB 004h         ;111        HSync and VSync swapped to count up result values
;
;outputs:
;   bh = Monitor sleep/wake status
;
GetMonitorSleepWakeStatus PROC NEAR     ;Offset 0x3b3b
    push   eax
    push   dx
    push   si
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_DACMode          ;0x4c
    in     eax, dx
    and    ax, TDFX_DM_VSyncDPMS OR TDFX_DM_VSyncValue OR TDFX_DM_HSyncDPMS;0xe
    shr    ax, 01h
    mov    si, ax
    mov    bh, byte ptr cs:[si + Data0x3b33];Offset 0x3b33
    pop    si
    pop    dx
    pop    eax
    ret
GetMonitorSleepWakeStatus ENDP

;This text is blanked out in the document, but still readable if copied.
;
;vidSerialParallelPort Register
;The vidSerialParallelPort register controls the chip’s I2C, DDC, GPIO, and the host port interface.
;Bit[17:0] of the register are shared between the I2C and GPIO interface. If VideoIn interface is configured
;to VMI (vidInFormat bit[15:14] == 2’b01), vidSerialParallelPort[17:0] are for VMI’s host port interface.
;If configured to TV out (vidInFormat bit[15:14] == 2’b10), the bits are used to co [MISSING TEXT]
;additional GPIO interface. This is in addition to the two GPIO pins (one input and one output), which are
;controlled by vidSerialParallelPort[30:29].

DDCSetClockAndDataTriState PROC NEAR    ;Offset 0x3b58
    push   ax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    or     eax, TDFX_VSPP_DDC_HostClock OR TDFX_VSPP_DDC_HostData;0x180000
    out    dx, eax
    pop    dx
    pop    ax
    ret
DDCSetClockAndDataTriState ENDP

DDCSetClockLowAndDataTriState PROC NEAR ;Offset 0x3b6d
    push   ax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    and    eax, NOT (TDFX_VSPP_DDC_HostClock OR TDFX_VSPP_DDC_HostData);0ffe7ffffh
    or     eax, TDFX_VSPP_DDC_HostData  ;0x100000
    out    dx, eax
    pop    dx
    pop    ax
    ret
DDCSetClockLowAndDataTriState ENDP

DDCSetClockTriStateAndDataLow PROC NEAR ;Offset 0x3b88
    push   ax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    and    eax, NOT (TDFX_VSPP_DDC_HostClock OR TDFX_VSPP_DDC_HostData);0ffe7ffffh
    or     eax, TDFX_VSPP_DDC_HostClock  ;0x80000
    out    dx, eax
    pop    dx
    pop    ax
    ret
DDCSetClockTriStateAndDataLow ENDP

DDCSetClockAndDataLow PROC NEAR         ;Offset 0x3ba3
    push   ax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    and    eax, NOT (TDFX_VSPP_DDC_HostClock OR TDFX_VSPP_DDC_HostData);0ffe7ffffh
    out    dx, eax
    pop    dx
    pop    ax
    ret
DDCSetClockAndDataLow ENDP

DDCReadMonitorClock PROC NEAR           ;Offset 0x3bb8
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    test   eax, TDFX_VSPP_DDC_MonitorClock;00200000h
    pop    dx
    pop    eax
    ret
DDCReadMonitorClock ENDP

DDCReadMonitorData PROC NEAR            ;Offset 0x3bcd
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    test   eax, TDFX_VSPP_DDC_MonitorData;00400000h
    pop    dx
    pop    eax
    ret
DDCReadMonitorData ENDP

EnableDDC PROC NEAR                     ;Offset 0x3be2
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    or     eax, TDFX_VSPP_DDC_Enabled   ;0x40000
    out    dx, eax
    pop    dx
    pop    eax
    ret
EnableDDC ENDP

DisableDDC PROC NEAR                    ;Offset 0x3bf9
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoSerialParallelPort;0x78
    in     eax, dx
    and    eax, NOT TDFX_VSPP_DDC_Enabled;0xfffbffff
    out    dx, eax
    pop    dx
    pop    eax
    ret
DisableDDC ENDP

SetVGAReadWriteAperture64K PROC NEAR    ;Offset 0x3c10
    push   eax
    push   ebx
    push   dx
    sub    ebx, ebx
    mov    bl, dl
    shl    bx, 01h
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    and    eax, 0fff00000h
    mov    ax, bx
    shl    ebx, 0ah
    or     eax, ebx
    out    dx, eax
    pop    dx
    pop    ebx
    pop    eax
    ret
SetVGAReadWriteAperture64K ENDP

GetVGAWriteApertureAddress64K PROC NEAR ;Offset 0x3c3b
    push   eax
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    and    ax, 03ffh
    shr    ax, 01h
    mov    dx, ax
    pop    eax
    ret
GetVGAWriteApertureAddress64K ENDP

MoveVGAReadWriteAperturesUp64K PROC NEAR;Offset 0x3c4f
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    add    eax, 00000802h
    out    dx, eax
    pop    dx
    pop    eax
    ret
MoveVGAReadWriteAperturesUp64K ENDP

SetVGAReadAperture PROC NEAR            ;Offset 0x3c66
    push   eax
    push   ebx
    push   dx
    sub    ebx, ebx
    mov    bh, dl
    shl    ebx, 03h
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    and    eax, 0fff003ffh
    or     eax, ebx
    out    dx, eax
    pop    dx
    pop    ebx
    pop    eax
    ret
SetVGAReadAperture ENDP

MoveVGAReadApertureUp64K PROC NEAR      ;Offset 0x3c8d
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    add    eax, 00000800h
    out    dx, eax
    pop    dx
    pop    eax
    ret
MoveVGAReadApertureUp64K ENDP

MoveVGAReadApertureDown64K PROC NEAR    ;Offset 0x3ca4
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    sub    eax, 00000800h
    out    dx, eax
    pop    dx
    pop    eax
    ret
MoveVGAReadApertureDown64K ENDP

SetVGAWriteAperture PROC NEAR           ;Offset 0x3cbb
    push   eax
    push   bx
    push   dx
    sub    bx, bx
    mov    bl, dl
    shl    bx, 01h
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    and    eax, 0fffffc00h
    or     ax, bx
    out    dx, eax
    pop    dx
    pop    bx
    pop    eax
    ret
SetVGAWriteAperture ENDP

MoveVGAWriteApertureUp64K PROC NEAR     ;Offset 0x3cdc
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    add    ax, 0002h
    out    dx, eax
    pop    dx
    pop    eax
    ret
MoveVGAWriteApertureUp64K ENDP

MoveVGAWriteApertureDown64K PROC NEAR   ;Offset 0x3cf0
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VGAInit1         ;0x2c
    in     eax, dx
    sub    ax, 0002h
    out    dx, eax
    pop    dx
    pop    eax
    ret
MoveVGAWriteApertureDown64K ENDP

Func0x3d04 PROC NEAR                    ;Offset 0x3d04
    jmp    Func0x5ad                    ;Offset 0x5ad
Func0x3d04 ENDP

Func0x3d07 PROC NEAR                    ;Offset 0x3d07
    jmp    Func0x5ad                    ;Offset 0x5ad
Func0x3d07 ENDP

LCDFunctions:                           ;Offset 0x3D0A
    pop    ds
    pop    si
    iret

GetPhysicalDisplayParameters:           ;Offset 0x3D0D
    pop    ds
    pop    si
    iret

Func0x3d10 PROC NEAR                    ;Offset 0x3d10
    ret
Func0x3d10 ENDP

SetALto2 PROC NEAR                      ;Offset 0x3d11
    mov    al, 02h
    ret
SetALto2 ENDP

CacheIOBaseAddress PROC NEAR            ;Offset 0x3d14
    push   ax
    push   bx
    push   ecx
    push   di
    push   si
    mov    ax, 0b102h                   ;Find PCI device
    sub    si, si         
    mov    cx, PCI_DEVICE_Banshee       ;0x0003
    mov    dx, PCI_VENDOR_3Dfx          ;0x121a
    call   Call1AHandlerSafely          ;Offset 0x4116
    jb     Failure                      ;Offset 0x3d65
    test   ah, ah
    jne    Failure                      ;Offset 0x3d65
    mov    ax, 0b10ah                   ;Read Configuration Dword
    mov    di, PCI_H0_DWord_BaseAddress2;0x18 = 24 - IO Base Address
    int    1ah
    test   ah, ah                       ;0 == success
    jne    Failure                      ;Offset 0x3d65
    and    cx, 0fffeh                   ;drop bottom bit
    mov    dx, cx
    push   dx
    mov    bl, dh
    add    dx, TDFX_IO_VGAInit0         ;0x28
    in     eax, dx
    and    al, NOT TDFX_VI0_Reserved7   ;0x7f
    or     al, TDFX_VI0_EnableVGAExtensions;0x40
    out    dx, eax
    mov    dx, VGA_CRTControllerIndexD  ;Port 0x3d4
    mov    al, TDFX_CRTC_ExtensionByte0 ;0x1c
    mov    ah, bl                       ;Set IO Base address byte 1 to scratch pad byte 0
    out    dx, ax
    mov    ax, TDFX_CRTC_ExtensionByte1 ;0x1d
    out    dx, ax                       ;Clear scratch pad byte 1
    mov    ax, TDFX_CRTC_ExtensionByte2 ;0x1e
    out    dx, ax                       ;Clear scratch pad byte 2
    pop    dx
Label0x3d5e:                            ;Offset 0x3d5e
    pop    si
    pop    di
    pop    ecx
    pop    bx
    pop    ax
    ret
Failure:                                ;Offset 0x3d65
    sub    dx, dx
    jmp    Label0x3d5e                  ;Offset 0x3d5e
CacheIOBaseAddress ENDP

;outputs:
;   dx = IO Base Address
;
GetIOBaseAddress PROC NEAR              ;Offset 0x3d69
    push   ax
    call   GetCRTControllerIndexPort    ;Offset 0x40aa
    mov    al, 1ch
    call   ReadIndexedRegister          ;Offset 0x3f84
    mov    dh, ah
    sub    dl, dl
    pop    ax
    ret
GetIOBaseAddress ENDP

LoadDefaultRegisterValues PROC NEAR     ;Offset 0x3d78
    push   eax
    push   dx
    push   si
    call   GetIOBaseAddress             ;Offset 0x3d69
    mov    si, dx
    add    dx, TDFX_IO_DRamInit1        ;0x1c
    in     eax, dx
    and    eax, TDFX_DI1_Unknown31 OR TDFX_DI1_RamType OR TDFX_DI1_PowerOnTime;0xc2000000
    or     eax, dword ptr cs:[DRAMInit1Default];Offset 0x7d09
    out    dx, eax
    lea    dx, [si + TDFX_IO_DRamInit0];Offset 0x18
    in     eax, dx
    and    eax, TDFX_DI0_Unknown28_31 OR TDFX_DI0_SGRAMType OR TDFX_DI0_SGRAMChipsetCount;0fc000000h
    or     eax, dword ptr cs:[DRAMInit0Default];Offset 0x7d05
    out    dx, eax
    lea    dx, [si + TDFX_IO_MiscInit0];Offset 0x10
    mov    eax, dword ptr cs:[MiscInit0Default];Offset 0x7cfd
    out    dx, eax
    lea    dx, [si + TDFX_IO_DRamData];Offset 0x34
    mov    eax, dword ptr cs:[DRamDataDefault];Offset 0x7d19
    out    dx, eax
    lea    dx, [si + TDFX_IO_DRamCommand];Offset 0x30
    mov    eax, TDFX_DRC_TestMode2 OR TDFX_DRC_BurstTypeInterleave OR 05h;0x10d
    out    dx, eax
    lea    dx, [si + TDFX_IO_DRamData];0x34
    mov    eax, 0ffffffffh
    out    dx, eax
    lea    dx, [si + TDFX_IO_DRamCommand];Offset 0x30
    mov    eax, TDFX_DRC_TestMode2 OR TDFX_DRC_BurstTypeInterleave OR 06h;0000010eh
    out    dx, eax
    lea    dx, [si + TDFX_IO_MiscInit1];Offset 0x14
    in     eax, dx
    and    eax, 1ff40000h
    or     eax, dword ptr cs:[MiscInit1Default];Offset 0x7d01
    out    dx, eax
    lea    dx, [si + TDFX_IO_AgpInit];Offset 0x20
    mov    eax, dword ptr cs:[AGPInitDefault];Offset 0x7d0d
    out    dx, eax
    lea    dx, [si + TDFX_IO_PLLControl1];Offset 0x44
    mov    eax, dword ptr cs:[PLLControl1Default];Offset 0x7d11
    out    dx, eax
    lea    dx, [si + TDFX_IO_PLLControl2];Offset 0x48
    mov    eax, dword ptr cs:[PLLControl2Default];Offset 0x7d15
    out    dx, eax
    lea    dx, [si + TDFX_IO_LFBMemoryConfig];Offset 0xc
    in     eax, dx
    or     ax, 1fffh
    out    dx, eax
    pop    si
    pop    dx
    pop    eax
    ret
LoadDefaultRegisterValues ENDP

GetLinearFrameBufferAddress PROC NEAR   ;Offset 0x3e1c
    push   bx
    push   ecx
    push   dx
    push   di
    push   si
    mov    ax, 0b102h                   ;Find PCI device
    sub    si, si
    mov    cx, PCI_DEVICE_Banshee       ;0003h
    mov    dx, PCI_VENDOR_3Dfx          ;0x121a
    call   Call1AHandlerSafely          ;Offset 0x4116
    jb     Label0x3e4e                  ;Offset 0x3e4e
    test   ah, ah
    jne    Label0x3e4e                  ;Offset 0x3e4e
    mov    ax, 0b10ah                   ;Read Configuration Dword
    mov    di, PCI_H0_DWord_BaseAddress1;0014h
    int    1ah
    test   ah, ah
    jne    Label0x3e4e                  ;Offset 0x3e4e
    mov    eax, ecx
    and    al, 0f0h
Label0x3e47:                            ;Offset 0x3e47
    pop    si
    pop    di
    pop    dx
    pop    ecx
    pop    bx
    ret
Label0x3e4e:                            ;Offset 0x3e4e
    sub    eax, eax
    jmp    Label0x3e47                  ;Offset 0x3e47
GetLinearFrameBufferAddress ENDP

VideoProcessorStatus PROC NEAR          ;Offset 0x3e53
    push   eax
    push   dx
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_VideoProcessorConfig;0x5c
    in     eax, dx
    test   al, 01h
    pop    dx
    pop    eax
    ret
VideoProcessorStatus ENDP

SetAndSelectVideoPLL PROC NEAR          ;Offset 0x3e64
    pushad
    call   Func0x3e83                   ;Offset 0x3e83
    call   GetIOBaseAddress             ;Offset 0x3d69
    add    dx, TDFX_IO_PLLControl0      ;0x40
    and    eax, 0000ffffh
    out    dx, eax
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    or     al, VGA_MISC_ClockSelectMask ;0xc
    mov    dl, VGA_MiscellaneousWrite_lowbyte;Port 0x3c2
    out    dx, al
    popad
    ret
SetAndSelectVideoPLL ENDP

Func0x3e83 PROC NEAR                    ;Offset 0x3e83
    mov    ebp, eax
    mov    esi, 0ffffffffh
    sub    ebx, ebx
    mov    ecx, ebx
    sub    bl, bl
Label0x3e94:                            ;Offset 0x3e94
    cmp    bl, 04h
    jae    Label0x3ed2                  ;Offset 0x3ed2
    sub    bh, bh
Label0x3e9b:                            ;Offset 0x3e9b
    cmp    bh, 40h
    jae    Label0x3ece                  ;Offset 0x3ece
    sub    ch, ch
Label0x3ea2:                            ;Offset 0x3ea2
    cmp    ch, 0fdh
    jae    Label0x3eca                  ;Offset 0x3eca
    call   Func0x3ee4                   ;Offset 0x3ee4
    sub    eax, ebp
    jae    Label0x3eb2                  ;Offset 0x3eb2
    neg    eax
Label0x3eb2:                            ;Offset 0x3eb2
    cmp    esi, eax
    jb     Label0x3ec6                  ;Offset 0x3ec6
    mov    esi, eax
    push   bx
    shl    ebx, 10h
    pop    bx
    push   cx
    shl    ecx, 10h
    pop    cx
Label0x3ec6:                            ;Offset 0x3ec6
    inc    ch
    jmp    Label0x3ea2                  ;Offset 0x3ea2
Label0x3eca:                            ;Offset 0x3eca
    inc    bh
    jmp    Label0x3e9b                  ;Offset 0x3e9b
Label0x3ece:                            ;Offset 0x3ece
    inc    bl
    jmp    Label0x3e94                  ;Offset 0x3e94
Label0x3ed2:                            ;Offset 0x3ed2
    shr    ebx, 10h
    shr    ecx, 10h
    mov    al, bl
    shl    bh, 02h
    or     al, bh
    mov    ah, ch
    ret
Func0x3e83 ENDP

Func0x3ee4 PROC NEAR                    ;Offset 0x3ee4
    push   bx
    push   ecx
    push   edx
    add    bh, 02h
    add    ch, 02h
    mov    cl, bl
    mov    bl, 01h
    shl    bl, cl
    mov    eax, 00da7a64h
    movzx  edx, ch
    imul   edx
    movzx  ecx, bh
    idiv   ecx
    sub    edx, edx
    movzx  ecx, bl
    idiv   ecx
    pop    edx
    pop    ecx
    pop    bx
    ret
Func0x3ee4 ENDP

Func0x3f19 PROC NEAR                    ;Offset 0x3f19
    mov    bl, al
    and    bl, 03h
    mov    bh, al
    shr    bh, 02h
    mov    ch, ah
    call   Func0x3ee4                   ;Offset 0x3ee4
    ret
Func0x3f19 ENDP

Func0x3f29 PROC NEAR                    ;Offset 0x3f29
    push   ax
    push   cx
    movzx  ax, byte ptr es:[bx + 01h]
    mov    cl, 06h
    mul    cl
    call   FetchCustomDataPointer       ;Offset 0x40f6
    les    si, es:[bx]
    add    si, ax
    pop    cx
    pop    ax
    ret
Func0x3f29 ENDP

                        DB 000h

GetBIOSDataSelectorInDS PROC NEAR       ;Offset 0x3f40
    mov    ds, word ptr cs:[BIOSDataSelector];Offset 0x77de
    ret
GetBIOSDataSelectorInDS ENDP

GetBIOSDataSelectorInES PROC NEAR       ;Offset 0x3f46
    mov    es, word ptr cs:[BIOSDataSelector];Offset 0x77de
    ret
GetBIOSDataSelectorInES ENDP

GetA000SelectorInES PROC NEAR           ;Offset 0x3f4c
    mov    es, word ptr cs:[A000Selector];Offset 0x77e0
    ret
GetA000SelectorInES ENDP

GetB000SelectorInES PROC NEAR           ;Offset 0x3f52
    mov    es, word ptr cs:[B000Selector];Offset 0x77e2
    ret
GetB000SelectorInES ENDP

GetB800SelectorInES PROC NEAR           ;Offset 0x3f58
    mov    es, word ptr cs:[B800Selector];Offset 0x77e4
    ret
GetB800SelectorInES ENDP

GetA000SelectorInDS PROC NEAR           ;Offset 0x3f5e
    mov    ds, word ptr cs:[A000Selector];Offset 0x77e0
    ret
GetA000SelectorInDS ENDP

GetB800SelectorInDS PROC NEAR           ;Offset 0x3f64
    mov    ds, word ptr cs:[B800Selector];Offset 0x77e4
    ret
GetB800SelectorInDS ENDP

TurnScreenOff PROC NEAR                 ;Offset 0x3f6a
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    call   ReadIndexedRegister          ;Offset 0x3f84
    or     ah, VGA_SEQ1_ScreenOff       ;0x20
    out    dx, ax
    ret
TurnScreenOff ENDP

TurnScreenOn PROC NEAR                  ;Offset 0x3f77
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    call   ReadIndexedRegister          ;Offset 0x3f84
    and    ah, NOT VGA_SEQ1_ScreenOff   ;0xdf
    out    dx, ax
    ret
TurnScreenOn ENDP

;
;inputs:
;   dx = port
;   al = index
;
;outputs:
;   dx = port
;   al = index
;   ah = value
;
ReadIndexedRegister PROC NEAR           ;Offset 0x3f84
    out    dx, al
    mov    ah, al
    inc    dx
    in     al, dx
    dec    dx
    xchg   al, ah
    ret
ReadIndexedRegister ENDP

Func0x3f8d PROC NEAR                    ;Offset 0x3f8d
    pushaw
    push   es
    call   FindModeData                 ;Offset 0x40d9
    lahf
    test   ah, 01h
    pop    es
    popaw
    ret
Func0x3f8d ENDP

Func0x3f99 PROC NEAR                    ;Offset 0x3f99
    push   bx
    push   es
    call   FindModeData                 ;Offset 0x40d9
    jb     Label0x3fac                  ;Offset 0x3fac
    cmp    byte ptr es:[bx + 04h], 01h
    je     Label0x3fac                  ;Offset 0x3fac
    cmp    byte ptr es:[bx + 04h], 03h
Label0x3fac:                            ;Offset 0x3fac
    pop    es
    pop    bx
    ret
Func0x3f99 ENDP

;inputs:
;   -
;
;outputs:
;   carry = 
;
Func0x3faf PROC NEAR                    ;Offset 0x3faf
    push   bx
    push   es
    call   FindModeData                 ;Offset 0x40d9
    jb     Label0x3fc2                  ;Offset 0x3fc2
    cmp    byte ptr es:[bx + 04h], 00h
    je     Label0x3fc2                  ;Offset 0x3fc2
    cmp    byte ptr es:[bx + 04h], 01h
Label0x3fc2:                            ;Offset 0x3fc2
    pop    es
    pop    bx
    ret
Func0x3faf ENDP

Func0x3fc5 PROC NEAR                    ;Offset 0x3fc5
    pushaw
    mov    dx, VGA_SequenceIndex        ;Port 0x3c4
    mov    al, VGA_SEQIdx_ClockingMode  ;0x1
    call   ReadIndexedRegister          ;Offset 0x3f84
    test   ah, VGA_SEQ1_8DotChar        ;0x1
    popaw
    ret
Func0x3fc5 ENDP

ShortBeep PROC NEAR                     ;Offset 0x3fd3
    pushaw
    mov    al, PIT_MC_OpMode_SquareWaveGenerator OR PIT_MC_ChanSel_2 OR PIT_MC_AccMode_LoByteHiByte;0xb6
    out    PIT_ModeCommand, al          ;Port 0x43
    mov    al, 33h
    out    PIT_Channel2Data, al         ;Port 0x42
    mov    al, 05h
    out    PIT_Channel2Data, al         ;Port 0x42
    in     al, KB_PortB                 ;Port 0x61
    push   ax
    or     al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out    KB_PortB, al                 ;Port KB_PortB
    mov    cx, 0004h
    call   Sleep                        ;Offset 0x4011
    pop    ax
    out    KB_PortB, al                 ;Port KB_PortB
    popaw
    ret
ShortBeep ENDP

LongBeep PROC NEAR                      ;Offset 0x3ff2
    pushaw
    mov    al, PIT_MC_OpMode_SquareWaveGenerator OR PIT_MC_ChanSel_2 OR PIT_MC_AccMode_LoByteHiByte;0xb6
    out    PIT_ModeCommand, al          ;Port 0x43
    mov    al, 33h
    out    PIT_Channel2Data, al         ;Port 0x42
    mov    al, 05h
    out    PIT_Channel2Data, al         ;Port 0x42
    in     al, KB_PortB                 ;Port 0x61
    push   ax
    or     al, KB_PBW_Timer2GateSpeakerEnable OR KB_PBW_SpeakerEnable;0x3
    out    KB_PortB, al                 ;Port KB_PortB
    mov    cx, 000ch
    call   Sleep                        ;Offset 0x4011
    pop    ax
    out    KB_PortB, al                 ;Port KB_PortB
    popaw
    ret
LongBeep ENDP

Sleep PROC NEAR                         ;Offset 0x4011
    push   cx
Label0x4012:                            ;Offset 0x4012
    push   cx
    mov    cx, 0e74h                    ;3700
    call   WaitTimer                    ;Offset 0x401e
    pop    cx
    loop   Label0x4012                  ;Offset 0x4012
    pop    cx
    ret
Sleep ENDP

WaitTimer PROC NEAR                     ;Offset 0x401e
    push   ax
    jcxz   Label0x4031                  ;Offset 0x4031
    sub    ah, ah
Label0x4023:                            ;Offset 0x4023
    in     al, DUMMY_DelayED            ;Port 0xed
    in     al, KB_PortB                 ;Port 0x61
    and    al, KB_PBR_ToggleRefreshRequest;0x10
    cmp    al, ah
    je     Label0x4023                  ;Offset 0x4023
    mov    ah, al
    loop   Label0x4023                  ;Offset 0x4023
Label0x4031:                            ;Offset 0x4031
    pop    ax
    ret
WaitTimer ENDP

WaitHBlankVBlankInactive PROC NEAR      ;Offset 0x4033
    push   ax
    push   cx
    call   GetCRTControllerIndexPort    ;Offset 0x40aa
    add    dl, 06h                      ;Port 0x3?a VGA_InputStatus1D
    sub    cx, cx
IsZero:                                 ;Offset 0x403d
    in     al, dx
    test   al, VGA_INSTS1_HorInactiveDisplayIntv;0x1
    loope  IsZero                       ;Offset 0x403d
    pop    cx
    pop    ax
    ret
WaitHBlankVBlankInactive ENDP

WaitHBlankVBlankActive PROC NEAR        ;Offset 0x4045
    push   ax
    push   cx
    call   GetCRTControllerIndexPort    ;Offset 0x40aa
    add    dl, 06h                      ;Port 0x3?a VGA_InputStatus1D
    sub    cx, cx
IsOne:                                  ;Offset 0x404f
    in     al, dx
    test   al, VGA_INSTS1_HorInactiveDisplayIntv;0x1
    loopne IsOne                        ;Offset 0x404f
    pop    cx
    pop    ax
    ret    
WaitHBlankVBlankActive ENDP

WaitUntilVSyncActive PROC NEAR          ;Offset 0x4057
    push   ax
    push   cx
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add    dl, 06h                      ;Port 0x3?a
    sub    cx, cx
IsZero:
    in     al, dx
    test   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    loope  IsZero                       ;Offset 0x4062
    pop    cx
    pop    ax
    ret
WaitUntilVSyncActive ENDP

WaitUntilVSyncInactive PROC NEAR        ;Offset 0x406a
    push   ax
    push   cx
    mov    dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    add    dl, 06h                      ;Port 0x3?a
    sub    cx, cx
IsOne:                                  ;Offset 0x4075
    in     al, dx
    test   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
    loopne IsOne                        ;Offset 0x4075
    pop    cx
    pop    ax
    ret
WaitUntilVSyncInactive ENDP

DisableAttributeControllerPaletteOutput PROC NEAR;Offset 0x407d
    push   ax
    push   dx
    cli
    call   ResetAttributeIndex          ;Offset 0x4135
    mov    dx, VGA_AttributeControllerIndex;Port 0x3c0
    sub    al, al
    out    dx, al
    sti
    pop    dx
    pop    ax
    ret
DisableAttributeControllerPaletteOutput ENDP

EnableAttributeControllerPaletteOutput PROC NEAR;Offset 0x408d
    push   ax
    push   dx
    cli
    call   ResetAttributeIndex          ;Offset 0x4135
    mov    dx, VGA_AttributeControllerIndex;Port 0x3c0
    mov    al, VGA_ATTR_PaletteAddressSource;0x20
    out    dx, al
    call   ResetAttributeIndex          ;Offset 0x4135
    sti
    pop    dx
    pop    ax
    ret
EnableAttributeControllerPaletteOutput ENDP

OutputListOfIndexedRegisters PROC NEAR  ;Offset 0x40a0
Label0x40a0:                            ;Offset 0x40a0
    mov    ah, bl
    lodsb
    xchg   al, ah
    out    dx, ax
    inc    bx
    loop   Label0x40a0                  ;Offset 0x40a0
    ret
OutputListOfIndexedRegisters ENDP

GetCRTControllerIndexPort PROC NEAR     ;Offset 0x40aa
    push   ax
    mov    dx, VGA_MiscellaneousRead    ;Port 0x3cc
    in     al, dx
    mov    dx, VGA_CRTControllerIndexB  ;0x3b4
    test   al, VGA_Misc_IOAddressSel_CGA;0x1
    je     Label0x40b8                  ;Offset 0x40b8
    mov    dl, VGA_CRTControllerIndexD_lowbyte;0x3d4
Label0x40b8:                            ;Offset 0x40b8
    pop    ax
    ret
GetCRTControllerIndexPort ENDP

;inputs:
;   bx = number of long beeps
;   cx = number of short beeps
;
PlayErrorBeeps PROC NEAR                ;Offset 0x40ba
    pushaw
    push   cx
Label0x40bc:                            ;Offset 0x40bc
    call   LongBeep                     ;Offset 0x3ff2
    mov    cx, 0002h
    call   Sleep                        ;Offset 0x4011
    dec    bx
    jne    Label0x40bc                  ;Offset 0x40bc
    pop    cx
    mov    bx, cx
Label0x40cb:                            ;Offset 0x40cb
    call   ShortBeep                    ;Offset 0x3fd3
    mov    cx, 0002h
    call   Sleep                        ;Offset 0x4011
    dec    bx
    jne    Label0x40cb                  ;Offset 0x40cb
    popaw
    ret
PlayErrorBeeps ENDP

;inputs:
;   al = mode
;
;outputs:
;   carry = 0 = found, 1 = not found
;   es:bx = pointer to mode data
;
FindModeData PROC NEAR                  ;Offset 0x40d9
    call   FetchCustomDataPointer       ;Offset 0x40f6
    les    bx, es:[bx + 04h]
Find:                                   ;Offset 0x40e0
    cmp    byte ptr es:[bx], 0ffh
    je     NotFound                     ;Offset 0x40f2
    cmp    byte ptr es:[bx], al
    je     Found                        ;Offset 0x40f0
    add    bx, 000eh
    jmp    Find                         ;Offset 0x40e0
Found:                                  ;Offset 0x40f0
    clc
    ret
NotFound:                               ;Offset 0x40f2
    sub    bx, bx
    stc
    ret
FindModeData ENDP

FetchCustomDataPointer PROC NEAR        ;Offset 0x40f6
    push   ax
    call   GetBIOSDataSelectorInES      ;Offset 0x3f46
    les    bx, es:[BDA_VideoParameterControlBlockPointer];Offset 0x4a8
    les    bx, es:[bx + BDA_VPCB_SecondarySavePointerTbl];Offset 0x10
    les    bx, es:[bx + BDA_SSP_Reserved16];Offset 0x16   //That custom pointer
    mov    ax, es
    or     ax, bx
    je     Label0x410f                  ;Offset 0x410f
Label0x410d:                            ;Offset 0x410d
    pop    ax
    ret
Label0x410f:                            ;Offset 0x410f
    push   cs
    pop    es
    mov    bx, offset Data0x77ea        ;Offset 0x77ea
    jmp    Label0x410d                  ;Offset 0x410d
FetchCustomDataPointer ENDP

Call1AHandlerSafely PROC NEAR           ;Offset 0x4116
    pushaw
    push   ds
    call   SetALto2                     ;Offset 0x3d11
    cmp    al, 02h
    jne    Label0x4131                  ;Offset 0x4131
    call   GetBIOSDataSelectorInDS      ;Offset 0x3f40
    lds    ax, ds:[INT_1A_Handler]      ;Offset 0x68
    mov    bx, ds
    or     ax, bx
    je     Label0x4131                  ;Offset 0x4131
    pop    ds
    popaw
    int    1ah
    ret
Label0x4131:                            ;Offset 0x4131
    stc
    pop    ds
    popaw
    ret
Call1AHandlerSafely ENDP

ResetAttributeIndex PROC NEAR           ;Offset 0x4135
    push  ax
    push  dx
    mov   dx, VGA_InputStatus1D         ;Port 0x3da
    in    al, dx
    mov   dl, VGA_InputStatus1B_lowbyte ;Port 0x3ba
    in    al, dx
    pop   dx
    pop   ax
    ret
ResetAttributeIndex ENDP

Func0x4141 PROC NEAR                    ;Offset 0x4141
    push   ax
    push   bx
    push   es
    mov    al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    call   FindModeData                 ;Offset 0x40d9
    mov    bp, word ptr es:[bx + 0bh]
    pop    es
    pop    bx
    pop    ax
    ret
Func0x4141 ENDP

Func0x4152 PROC NEAR                    ;Offset 0x4152
    push   ax
    push   bx
    push   es
    and    al, 7fh
    call   FindModeData                 ;Offset 0x40d9
    cmp    byte ptr es:[bx + 04h], 09h
    pop    es
    pop    bx
    pop    ax
    ret
Func0x4152 ENDP

FindExtraModeData PROC NEAR             ;Offset 0x4163
    push   ax
    call   FindModeData                 ;Offset 0x40d9
    mov    al, byte ptr es:[bx + 01h]
    call   FetchCustomDataPointer       ;Offset 0x40f6
    les    bx, es:[bx]
    mov    ah, byte ptr cs:[Data0x786f] ;Offset 0x786f
    mul    ah
    add    bx, ax
    pop    ax
    ret
FindExtraModeData ENDP

Func0x417c PROC NEAR                    ;Offset 0x417c
    push   bx
    push   es
    call   FindModeData                 ;Offset 0x40d9
    mov    al, 00h
    jb     Label0x4189                  ;Offset 0x4189
    mov    al, byte ptr es:[bx + 06h]
Label0x4189:                            ;Offset 0x4189
    pop    es
    pop    bx
    ret
Func0x417c ENDP

Func0x418c PROC NEAR                    ;Offset 0x418c
    push   bx
    push   es
    call   FindModeData                 ;Offset 0x40d9
    jb     Label0x41a6                  ;Offset 0x41a6
    cmp    byte ptr es:[bx + 04h], 05h
    je     Label0x41a6                  ;Offset 0x41a6
    cmp    byte ptr es:[bx + 04h], 03h
    je     Label0x41a6                  ;Offset 0x41a6
    cmp    byte ptr es:[bx + 04h], 04h
Label0x41a6:                            ;Offset 0x41a6
    pop    es
    pop    bx
    ret
Func0x418c ENDP

;inputs:
;   ax = VESA mode
;
;
FindVESAModeData PROC NEAR              ;Offset 0x41a9
    call   FetchCustomDataPointer       ;Offset 0x40f6
    les    bx, es:[bx + 04h]
Find:                                   ;Offset 0x41b0
    cmp    byte ptr es:[bx], 0ffh
    je     NotFound                     ;Offset 0x41c3
    cmp    word ptr es:[bx + 02h], ax
    je     Found                        ;Offset 0x41c1
    add    bx, 000eh
    jmp    Find                         ;Offset 0x41b0
Found:                                  ;Offset 0x41c1
    clc
    ret
NotFound:                               ;Offset 0x41c3
    sub    bx, bx
    stc
    ret
FindVESAModeData ENDP

StringCopy PROC NEAR                    ;Offset 0x41c7
ContinueCopy:                           ;Offset 0x41c7
    lodsb
    stosb
    test   al, al
    jne    ContinueCopy                 ;Offset 0x41c7
    ret
StringCopy ENDP

;inputs:
;   ax = mode
;
;outputs:
;   al = legacy mode (byte)
;   ah = extra data index
;
Func0x41ce PROC NEAR                    ;Offset 0x41ce
    push   bx
    push   es
    call   FindVESAModeData             ;Offset 0x41a9
    mov    ax, 0ffffh
    jb     Label0x41df                  ;Offset 0x41df
    mov    al, byte ptr es:[bx]
    mov    ah, byte ptr es:[bx + 01h]
Label0x41df:                            ;Offset 0x41df
    pop    es
    pop    bx
    ret
Func0x41ce ENDP

Func0x41e2 PROC NEAR                    ;Offset 0x41e2
    push   es
    call   FindModeData                 ;Offset 0x40d9
    jb     Label0x41f6                  ;Offset 0x41f6
    mov    bx, word ptr es:[bx + 02h]
    cmp    bx, 0ffffh
    jne    Label0x41f6                  ;Offset 0x41f6
    mov    bl, al
    sub    bh, bh
    clc
Label0x41f6:                            ;Offset 0x41f6
    cmc
    pop    es
    ret
Func0x41e2 ENDP

                        DB 000h

;Offset 0x41fa
VideoParameters VideoParametersTable 37 dup(<>)

ORG 41fah
;Offset 0x41fa
                        DB 028h, 018h, 008h, 000h, 008h, 009h, 003h, 000h, 002h, 063h, 02Dh, 027h, 028h, 090h, 02Bh, 0A0h
                        DB 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 028h, 018h, 008h, 000h, 008h, 009h, 003h, 000h, 002h, 063h, 02Dh, 027h, 028h, 090h, 02Bh, 0A0h
                        DB 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 008h, 000h, 010h, 001h, 003h, 000h, 002h, 063h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 008h, 000h, 010h, 001h, 003h, 000h, 002h, 063h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 0C7h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 028h, 018h, 008h, 000h, 040h, 009h, 003h, 000h, 002h, 063h, 02Dh, 027h, 028h, 090h, 02Bh, 080h
                        DB 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h
                        DB 0B9h, 0A2h, 0FFh, 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 001h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

                        DB 028h, 018h, 008h, 000h, 040h, 009h, 003h, 000h, 002h, 063h, 02Dh, 027h, 028h, 090h, 02Bh, 080h
                        DB 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h
                        DB 0B9h, 0A2h, 0FFh, 000h, 013h, 015h, 017h, 002h, 004h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 001h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 00Fh, 000h, 0FFh

                        DB 050h, 018h, 008h, 000h, 040h, 001h, 001h, 000h, 006h, 063h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 0BFh, 01Fh, 000h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h
                        DB 0B9h, 0C2h, 0FFh, 000h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h, 017h
                        DB 017h, 017h, 017h, 001h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh

                        DB 050h, 018h, 00Eh, 000h, 010h, 000h, 003h, 000h, 003h, 0A6h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Dh, 063h
                        DB 0BAh, 0A3h, 0FFh, 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h, 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

                        DB 050h, 018h, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 063h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 00Eh, 08Fh, 050h, 01Fh, 096h
                        DB 0B9h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 050h, 01Dh, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 0E3h, 05Fh, 04Fh, 050h, 082h, 052h, 09Eh
                        DB 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 00Ch, 0DFh, 050h, 000h, 0E7h
                        DB 004h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 064h, 024h, 010h, 000h, 0FAh, 001h, 00Fh, 000h, 006h, 02Fh, 07Fh, 063h, 064h, 082h, 06Bh, 01Bh
                        DB 072h, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 059h, 00Dh, 057h, 032h, 000h, 057h
                        DB 073h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

                        DB 064h, 024h, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 02Fh, 07Fh, 063h, 064h, 082h, 069h, 019h
                        DB 072h, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 059h, 00Dh, 057h, 064h, 000h, 058h
                        DB 073h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 0A0h, 03Fh, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 02Fh, 0CEh, 09Fh, 0A0h, 091h, 0A6h, 014h
                        DB 028h, 052h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 0FFh, 0A0h, 000h, 001h
                        DB 028h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 028h, 018h, 008h, 000h, 020h, 009h, 00Fh, 000h, 006h, 063h, 02Dh, 027h, 028h, 090h, 02Bh, 080h
                        DB 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 000h, 096h
                        DB 0B9h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

                        DB 050h, 018h, 008h, 000h, 040h, 001h, 00Fh, 000h, 006h, 063h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 000h, 096h
                        DB 0B9h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 010h, 011h, 012h, 013h, 014h
                        DB 015h, 016h, 017h, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

                        DB 080h, 02Fh, 010h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 02Fh, 0A3h, 07Fh, 080h, 087h, 083h, 094h
                        DB 024h, 0F5h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 009h, 0FFh, 080h, 000h, 0FFh
                        DB 025h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 050h, 03Bh, 008h, 000h, 026h, 001h, 003h, 000h, 002h, 0E3h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 00Bh, 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 01Fh, 0E7h
                        DB 004h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 00Eh, 000h, 080h, 001h, 00Fh, 000h, 006h, 0A2h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h
                        DB 0BAh, 0E3h, 0FFh, 000h, 008h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
                        DB 018h, 000h, 000h, 00Bh, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 0FFh

                        DB 050h, 018h, 00Eh, 000h, 080h, 001h, 00Fh, 000h, 006h, 0A3h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 0BFh, 01Fh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 00Fh, 063h
                        DB 0BAh, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

                        DB 028h, 018h, 00Eh, 000h, 008h, 009h, 003h, 000h, 002h, 0A3h, 02Dh, 027h, 028h, 090h, 02Bh, 0A0h
                        DB 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h
                        DB 0BAh, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 028h, 018h, 00Eh, 000h, 008h, 009h, 003h, 000h, 002h, 0A3h, 02Dh, 027h, 028h, 090h, 02Bh, 0A0h
                        DB 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 014h, 01Fh, 063h
                        DB 0BAh, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 00Eh, 000h, 010h, 001h, 003h, 000h, 002h, 0A3h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h
                        DB 0BAh, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 00Eh, 000h, 010h, 001h, 003h, 000h, 002h, 0A3h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 04Dh, 00Bh, 00Ch, 000h, 000h, 000h, 000h, 083h, 085h, 05Dh, 028h, 01Fh, 063h
                        DB 0BAh, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 008h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 028h, 018h, 010h, 000h, 008h, 008h, 003h, 000h, 002h, 067h, 02Dh, 027h, 028h, 090h, 02Bh, 0A0h
                        DB 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 014h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 010h, 000h, 010h, 000h, 003h, 000h, 002h, 067h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 008h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 050h, 018h, 010h, 000h, 010h, 000h, 003h, 000h, 002h, 066h, 05Fh, 04Fh, 050h, 082h, 055h, 081h
                        DB 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 00Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 010h, 018h, 018h, 018h, 018h
                        DB 018h, 018h, 018h, 00Eh, 000h, 00Fh, 008h, 000h, 000h, 000h, 000h, 000h, 010h, 00Ah, 000h, 0FFh

                        DB 050h, 01Dh, 010h, 000h, 0A0h, 001h, 00Fh, 000h, 006h, 0E3h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h
                        DB 004h, 0C3h, 0FFh, 000h, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh
                        DB 03Fh, 03Fh, 03Fh, 001h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 0FFh

                        DB 050h, 01Dh, 010h, 000h, 0A0h, 001h, 00Fh, 000h, 006h, 0E3h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 00Bh, 03Eh, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 028h, 000h, 0E7h
                        DB 004h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 00Fh, 0FFh

                        DB 028h, 018h, 008h, 000h, 020h, 001h, 00Fh, 000h, 00Eh, 063h, 05Fh, 04Fh, 050h, 082h, 054h, 080h
                        DB 0BFh, 01Fh, 000h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 040h, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 084h, 018h, 010h, 000h, 020h, 001h, 003h, 000h, 002h, 06Fh, 09Ah, 083h, 084h, 09Dh, 085h, 013h
                        DB 0BFh, 01Fh, 000h, 04Fh, 00Dh, 00Eh, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 042h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 084h, 02Ah, 009h, 000h, 040h, 001h, 003h, 000h, 002h, 06Fh, 09Ah, 083h, 084h, 09Dh, 085h, 013h
                        DB 0BFh, 01Fh, 000h, 048h, 007h, 008h, 000h, 000h, 000h, 000h, 092h, 084h, 082h, 042h, 01Fh, 089h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 084h, 031h, 008h, 000h, 040h, 001h, 003h, 000h, 002h, 06Fh, 09Eh, 083h, 084h, 081h, 08Ah, 09Eh
                        DB 0BFh, 01Fh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 042h, 01Fh, 096h
                        DB 0B9h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 084h, 03Bh, 008h, 000h, 040h, 001h, 003h, 000h, 002h, 0EFh, 09Eh, 083h, 084h, 081h, 08Ah, 09Eh
                        DB 00Bh, 03Eh, 000h, 047h, 006h, 007h, 000h, 000h, 000h, 000h, 0EAh, 08Ch, 0DFh, 042h, 01Fh, 0E7h
                        DB 004h, 0A3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 014h, 007h, 038h, 039h, 03Ah, 03Bh, 03Ch
                        DB 03Dh, 03Eh, 03Fh, 00Ch, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 00Eh, 000h, 0FFh

                        DB 028h, 018h, 008h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 06Fh, 02Dh, 027h, 028h, 090h, 029h, 08Fh
                        DB 0BFh, 01Fh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 08Eh, 08Fh, 028h, 01Fh, 096h
                        DB 0B9h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 028h, 01Dh, 008h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 0EFh, 02Dh, 027h, 028h, 090h, 029h, 08Fh
                        DB 00Bh, 03Eh, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 0EAh, 00Ch, 0DFh, 028h, 01Fh, 0E7h
                        DB 004h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 032h, 024h, 008h, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 02Fh, 03Dh, 031h, 032h, 080h, 035h, 01Dh
                        DB 072h, 0F0h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 059h, 00Dh, 057h, 064h, 000h, 058h
                        DB 073h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 001h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

                        DB 040h, 01Ch, 00Eh, 0FFh, 0FFh, 001h, 00Fh, 000h, 00Eh, 02Fh, 04Fh, 03Fh, 040h, 083h, 042h, 00Ch
                        DB 024h, 0F5h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 009h, 0FFh, 080h, 000h, 0FFh
                        DB 025h, 0E3h, 0FFh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch
                        DB 00Dh, 00Eh, 00Fh, 041h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 005h, 00Fh, 0FFh

;Offset 0x4b3a
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

;Offset 0x4f3a
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

;Offset 0x533a
Font8x14                DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 06Ch, 0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h, 018h, 018h, 03Ch, 000h, 000h, 000h
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
                        DB 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 0FCh, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 060h, 0C0h, 0C0h, 0FCh, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 006h, 00Ch, 018h, 030h, 030h, 030h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 006h, 00Ch, 078h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h
                        DB 000h, 000h, 006h, 00Ch, 018h, 030h, 060h, 030h, 018h, 00Ch, 006h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 000h, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 07Ch, 000h, 000h, 000h
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
                        DB 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h, 0D6h, 0D6h, 0C6h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 0DCh, 076h, 066h, 060h, 060h, 0F0h, 000h, 000h, 000h
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
                        DB 000h, 018h, 018h, 03Ch, 066h, 060h, 060h, 066h, 03Ch, 018h, 018h, 000h, 000h, 000h
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
                        DB 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h, 030h, 060h, 0DCh, 086h, 00Ch, 018h, 03Eh, 000h
                        DB 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h, 030h, 066h, 0CEh, 09Eh, 03Eh, 006h, 006h, 000h
                        DB 000h, 000h, 018h, 018h, 000h, 018h, 018h, 03Ch, 03Ch, 03Ch, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 036h, 06Ch, 0D8h, 06Ch, 036h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0D8h, 06Ch, 036h, 06Ch, 0D8h, 000h, 000h, 000h, 000h, 000h
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
                        DB 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FCh, 0C6h, 0C6h, 0FCh, 0C0h, 0C0h, 040h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h
                        DB 000h, 000h, 0FEh, 0C6h, 060h, 030h, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h, 0D8h, 0D8h, 070h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 07Ch, 060h, 060h, 0C0h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h
                        DB 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h
                        DB 000h, 000h, 01Eh, 030h, 018h, 00Ch, 03Eh, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 07Eh, 0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 003h, 006h, 07Eh, 0DBh, 0DBh, 0F3h, 07Eh, 060h, 0C0h, 000h, 000h, 000h
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
                        DB 000h, 070h, 0D8h, 030h, 060h, 0C8h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

;Offset 0x613a
Font8x16                DB 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0A5h, 099h, 081h, 081h, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0DBh, 0E7h, 0FFh, 0FFh, 07Eh, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 06Ch, 0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 010h, 038h, 07Ch, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 07Eh, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h
                        DB 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
                        DB 000h, 000h, 01Eh, 006h, 00Eh, 01Ah, 078h, 0CCh, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h
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

;Offset 0x713a
Font8x14Patch           DB 01Dh, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h
                        DB 022h, 000h, 063h, 063h, 063h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 02Bh, 000h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 000h, 000h, 000h
                        DB 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
                        DB 04Dh, 000h, 000h, 0C3h, 0E7h, 0FFh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h
                        DB 054h, 000h, 000h, 0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 056h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h
                        DB 057h, 000h, 000h, 0C3h, 0C3h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h
                        DB 058h, 000h, 000h, 0C3h, 0C3h, 066h, 03Ch, 018h, 03Ch, 066h, 0C3h, 0C3h, 000h, 000h, 000h
                        DB 059h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h
                        DB 05Ah, 000h, 000h, 0FFh, 0C3h, 086h, 00Ch, 018h, 030h, 061h, 0C3h, 0FFh, 000h, 000h, 000h
                        DB 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh, 0DBh, 0DBh, 0DBh, 000h, 000h, 000h
                        DB 076h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h
                        DB 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 000h, 000h, 000h
                        DB 091h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h, 000h, 000h, 000h
                        DB 09Bh, 000h, 018h, 018h, 07Eh, 0C3h, 0C0h, 0C0h, 0C3h, 07Eh, 018h, 018h, 000h, 000h, 000h
                        DB 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 000h, 000h, 000h
                        DB 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 0F3h, 000h, 000h, 000h
                        DB 0F1h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 0FFh, 000h, 000h, 000h
                        DB 0F6h, 000h, 000h, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 018h, 018h, 000h, 000h, 000h
                        DB 000h

;Offset 0x7267
Font8x16Patch           DB 01Dh, 000h, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h
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

;Offset 0x73ab
Data0x73ab              DB 000h, 020h, 008h, 028h, 002h, 022h, 00Ah, 02Ah, 010h, 030h, 018h, 038h, 012h, 032h, 01Ah, 03Ah
                        DB 004h, 024h, 00Ch, 02Ch, 006h, 026h, 00Eh, 02Eh, 014h, 034h, 01Ch, 03Ch, 016h, 036h, 01Eh, 03Eh
                        DB 001h, 021h, 009h, 029h, 003h, 023h, 00Bh, 02Bh, 011h, 031h, 019h, 039h, 013h, 033h, 01Bh, 03Bh
                        DB 005h, 025h, 00Dh, 02Dh, 007h, 027h, 00Fh, 02Fh, 015h, 035h, 01Dh, 03Dh, 017h, 037h, 01Fh, 03Fh

;Offset 0x73eb
Data0x73eb              DB 000h, 020h, 008h, 028h, 002h, 022h, 006h, 02Ah, 000h, 020h, 008h, 028h, 002h, 022h, 006h, 02Ah
                        DB 015h, 035h, 01Dh, 03Dh, 017h, 037h, 01Fh, 03Fh, 015h, 035h, 01Dh, 03Dh, 017h, 037h, 01Fh, 03Fh

;Offset 0x740b
Data0x740b              DB 000h, 02Ah, 000h, 03Fh

;Offset 0x740f
Data0x740f              DB 000h, 000h, 000h, 000h, 000h, 02Ah, 000h, 02Ah, 000h, 000h, 02Ah, 02Ah, 02Ah, 000h, 000h, 02Ah
                        DB 000h, 02Ah, 02Ah, 015h, 000h, 02Ah, 02Ah, 02Ah, 015h, 015h, 015h, 015h, 015h, 03Fh, 015h, 03Fh
                        DB 015h, 015h, 03Fh, 03Fh, 03Fh, 015h, 015h, 03Fh, 015h, 03Fh, 03Fh, 03Fh, 015h, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 000h, 005h, 005h, 005h, 008h, 008h, 008h, 00Bh, 00Bh, 00Bh, 00Eh, 00Eh, 00Eh, 011h
                        DB 011h, 011h, 014h, 014h, 014h, 018h, 018h, 018h, 01Ch, 01Ch, 01Ch, 020h, 020h, 020h, 024h, 024h
                        DB 024h, 028h, 028h, 028h, 02Dh, 02Dh, 02Dh, 032h, 032h, 032h, 038h, 038h, 038h, 03Fh, 03Fh, 03Fh
                        DB 000h, 000h, 03Fh, 010h, 000h, 03Fh, 01Fh, 000h, 03Fh, 02Fh, 000h, 03Fh, 03Fh, 000h, 03Fh, 03Fh
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

;Offset 0x76f7
ROMConfigTable          DW offset OEMConfigTable;Offset 0x7cf9

;Offset 0x76f9
                        DW offset VideoOverrideTable;Offset 0x776b

;Offset 0x76fb
                        DW offset Font8x8;Offset 0x4b3a

;Offset 0x76fd
                        DW offset Font8x14;Offset 0x533a

;Offset 0x76ff
                        DW offset Font8x16;Offset 0x613a

;Offset 0x7701
                        DW offset Font8x14Patch;Offset 0x713a

;Offset 0x7703
                        DW offset Font8x16Patch;Offset 0x7267

;Offset 0x7705
                        DW offset ThreeDfxNameLength;Offset 0x77f5

                        DB 000h

;Offset 0x7708
BootSplash              DB 'VGA BIOS - Version 1.00.10-SG', 00Dh, 00AH
                        DB 'Copyright (C) 1990-1998 Elpin Systems, Inc.', 00Dh, 00Ah
                        DB 'All rights reserved.', 00Dh, 00Ah, 000h

;Offset 0x776b
VideoOverrideTable VideoOverridePointerTable < <offset VideoParameters, 0C000h>, <00000h, 00000h>, <00000h, 00000h>, <00000h, 00000h>, <offset SecondarySaveTable, 0C000h>, <00000h, 00000h>, <00000h, 00000h> >

;Offset 0x7787
SecondarySaveTable SecondarySavePointerTable <SIZEOF SecondarySavePointerTable, <offset DisplayCominationCodes, 0C000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <0000h, 0000h>, <offset Data0x77ea, 0C000h> >

;Offset 0x77a1
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
                        DB BDA_DCC_NoDisplay, BDA_DCC_CVGA      ; 11) Color Video graphics Array (CVGA)
                        DB BDA_DCC_MDPA, BDA_DCC_CVGA           ; 12) CVGA + MDPA
                        DB BDA_DCC_NoDisplay, BDA_DCC_MVGA      ; 13) VGA based with monochrome display (MVGA)
                        DB BDA_DCC_CGA, BDA_DCC_MVGA            ; 14) MVGA + CGA
                        DB BDA_DCC_CGA, BDA_DCC_PGC             ; 15) PGC + CGA

;Offset 0x77c5
                                        ;Offset  Size    Description     (Table 00045)
StaticFunctionalityTable DB 0FFh        ;00h    BYTE    modes supported 1
                                        ;               bit 0 to bit 7 = 1 modes 0,1,2,3,4,5,6,7 supported
                        DB 0E0h         ;01h    BYTE    modes supported 2
                                        ;               bit 0 to bit 7 = 1 modes 8,9,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh supported
                        DB 00Fh         ;02h    BYTE    modes supported 3
                                        ;               bit 0 to bit 3 = 1 modes 10h,11h,12h,13h supported
                                        ;               bit 4 to bit 7 unused (0)
                        DB 000h         ;03h    BYTE    (IBM) reserved
                        DB 000h         ;04h    BYTE    (IBM) reserved
                        DB 000h         ;05h    BYTE    (IBM) reserved
                        DB 000h         ;06h    BYTE    (IBM) reserved
                        DB 007h         ;07h    BYTE    scan lines supported
                                        ;               bit 0 to bit 2 = 1 if scan lines 200,350,400 supported
                                        ;               bits 3-7 = unused (0)
                        DB 002h         ;08h    BYTE    total number of character blocks available in text modes
                        DB 008h         ;09h    BYTE    maximum number of active character blocks in text modes
                        DW 00EFFh       ;0Ah    WORD    miscellaneous function support flags (see #00046)
                        DW 00000h       ;0Ch    WORD    reserved
                        DB 03Fh         ;0Eh    BYTE    save pointer function flags (see #00047)
                        DB 000h         ;0Fh    BYTE    reserved
;Offset 0x77d5
DB 090h

;Protected Mode Info Block
;Offset 0x77d6
                        DB 'PMID'       ;00h Signature 'PMID'
                        DW offset Func0x33cc;Offset 0x33cc 04h  Offset of PM entrypoint within BIOS
                        DW offset Func0x33ec;Offset 0x33ec 06h  Offset of PM initialization entrypoint
;Offset 0x77de
BIOSDataSelector        DW 00000h

;Offset 0x77e0
A000Selector            DW 0a000h

;Offset 0x77e2
B000Selector            DW 0b000h

;Offset 0x77e4
B800Selector            DW 0b800h

;Offset 0x77e6
BIOSSegment             DW 0c000h

;Offset 0x77e8
InProtectedMode         DB 000h ; 0 = real mode, 1 = protected mode

;Offset 0x77e9
PMIDChecksum            DB 0f0h

;Offset 0x77ea
Data0x77ea              FARPointer <offset Data0x7b11, 0C000h>;Offset 0x7b11
Data0x77ee              FARPointer <offset Data0x7870, 0C000h>;Offset 0x7870

;Offset 0x77f2
SplashWaitFrames        DW 00118h   ;280 / 70 = 4 seconds

;Offset 0x77f4
DefaultAttribute        DB 007h

;Offset 0x77f5
ThreeDfxNameLength      DW 00018h

;Offset 0x77f7
ThreeDfxName            DB '3Dfx Interactive, Inc.', 00Dh, 00Ah

;Offset 0x780f
DB 60h dup (000h)

;Offset 0x786f
Data0x786f              DB 006h




                        ;                            +----
                        ;                            | 0 =  0xb800, 32KB
                        ;                            | 1 =  0xb000, 32KB
                        ;                            | 2 =  0xb800, 32KB
                        ;                            | else 0xa000, 64KB
                        ;                            |
                        ;                            |
                        ;        Table               |        Bits    Width in    Height in
                        ; Legacy   0                 |         per     pixels      pixels     Bytes per  char
;Offset 0x7870          ;  Mode  index VESA Mode     |        pixel   or chars    or chars    scan line height
Data0x7870              DB 000h, 017h, 000h, 000h, 000h, 008h, 004h, 028h, 000h, 019h, 000h, 050h, 000h, 010h
                        DB 001h, 017h, 001h, 000h, 000h, 008h, 004h, 028h, 000h, 019h, 000h, 050h, 000h, 010h
                        DB 002h, 018h, 002h, 000h, 000h, 008h, 004h, 050h, 000h, 019h, 000h, 0A0h, 000h, 010h
                        DB 003h, 018h, 003h, 000h, 000h, 008h, 004h, 050h, 000h, 019h, 000h, 0A0h, 000h, 010h
                        DB 004h, 004h, 004h, 000h, 002h, 001h, 002h, 040h, 001h, 0C8h, 000h, 050h, 000h, 008h
                        DB 005h, 005h, 005h, 000h, 002h, 001h, 002h, 040h, 001h, 0C8h, 000h, 050h, 000h, 008h
                        DB 006h, 006h, 006h, 000h, 002h, 001h, 001h, 080h, 002h, 0C8h, 000h, 050h, 000h, 008h
                        DB 007h, 019h, 007h, 000h, 001h, 008h, 000h, 050h, 000h, 019h, 000h, 0A0h, 000h, 010h
                        DB 00Dh, 00Dh, 00Dh, 000h, 005h, 008h, 004h, 040h, 001h, 0C8h, 000h, 028h, 000h, 008h
                        DB 00Eh, 00Eh, 00Eh, 000h, 005h, 004h, 004h, 080h, 002h, 0C8h, 000h, 050h, 000h, 008h
                        DB 00Fh, 011h, 00Fh, 000h, 003h, 002h, 001h, 080h, 002h, 05Eh, 001h, 050h, 000h, 00Eh
                        DB 010h, 012h, 010h, 000h, 005h, 002h, 004h, 080h, 002h, 05Eh, 001h, 050h, 000h, 00Eh
                        DB 011h, 01Ah, 011h, 000h, 004h, 001h, 001h, 080h, 002h, 0E0h, 001h, 050h, 000h, 010h
                        DB 012h, 01Bh, 012h, 000h, 005h, 001h, 004h, 080h, 002h, 0E0h, 001h, 050h, 000h, 010h
                        DB 013h, 01Ch, 013h, 000h, 006h, 001h, 008h, 040h, 001h, 0C8h, 000h, 040h, 001h, 008h
                        DB 05Bh, 008h, 000h, 001h, 006h, 001h, 008h, 080h, 002h, 090h, 001h, 080h, 002h, 010h
                        DB 05Fh, 009h, 001h, 001h, 006h, 001h, 008h, 080h, 002h, 0E0h, 001h, 080h, 002h, 010h
                        DB 06Ah, 00Ah, 002h, 001h, 005h, 001h, 004h, 020h, 003h, 058h, 002h, 064h, 000h, 010h
                        DB 05Ch, 00Bh, 003h, 001h, 006h, 001h, 008h, 020h, 003h, 058h, 002h, 020h, 003h, 010h
                        DB 05Eh, 00Fh, 005h, 001h, 006h, 001h, 008h, 000h, 004h, 000h, 003h, 000h, 004h, 010h
                        DB 06Bh, 00Ch, 007h, 001h, 006h, 001h, 008h, 000h, 005h, 000h, 004h, 000h, 005h, 010h
                        DB 068h, 010h, 008h, 001h, 000h, 006h, 004h, 050h, 000h, 03Ch, 000h, 0A0h, 000h, 008h
                        DB 055h, 01Dh, 009h, 001h, 000h, 002h, 004h, 084h, 000h, 019h, 000h, 008h, 001h, 010h
                        DB 054h, 01Eh, 00Ah, 001h, 000h, 002h, 004h, 084h, 000h, 02Bh, 000h, 008h, 001h, 008h
                        DB 065h, 01Fh, 00Bh, 001h, 000h, 002h, 004h, 084h, 000h, 032h, 000h, 008h, 001h, 008h
                        DB 064h, 020h, 00Ch, 001h, 000h, 002h, 004h, 084h, 000h, 03Ch, 000h, 008h, 001h, 008h
                        DB 078h, 021h, 080h, 001h, 006h, 001h, 008h, 040h, 001h, 0C8h, 000h, 040h, 001h, 008h
                        DB 020h, 022h, 081h, 001h, 006h, 001h, 008h, 040h, 001h, 0F0h, 000h, 040h, 001h, 008h
                        DB 023h, 023h, 084h, 001h, 006h, 001h, 008h, 090h, 001h, 02Ch, 001h, 090h, 001h, 008h
                        DB 026h, 024h, 087h, 001h, 006h, 001h, 008h, 000h, 002h, 080h, 001h, 000h, 002h, 008h
                        DB 079h, 025h, 00Eh, 001h, 007h, 001h, 010h, 040h, 001h, 0C8h, 000h, 080h, 002h, 008h
                        DB 07Ah, 026h, 00Fh, 001h, 008h, 001h, 018h, 040h, 001h, 0C8h, 000h, 000h, 004h, 008h
                        DB 021h, 027h, 082h, 001h, 007h, 001h, 010h, 040h, 001h, 0F0h, 000h, 080h, 002h, 008h
                        DB 022h, 028h, 083h, 001h, 008h, 001h, 018h, 040h, 001h, 0F0h, 000h, 000h, 004h, 008h
                        DB 024h, 029h, 085h, 001h, 007h, 001h, 010h, 090h, 001h, 02Ch, 001h, 020h, 003h, 008h
                        DB 025h, 02Ah, 086h, 001h, 008h, 001h, 018h, 090h, 001h, 02Ch, 001h, 000h, 008h, 008h
                        DB 027h, 02Bh, 088h, 001h, 007h, 001h, 010h, 000h, 002h, 080h, 001h, 000h, 004h, 00Eh
                        DB 028h, 02Ch, 089h, 001h, 008h, 001h, 018h, 000h, 002h, 080h, 001h, 000h, 008h, 00Eh
                        DB 029h, 02Dh, 08Ah, 001h, 007h, 001h, 010h, 080h, 002h, 090h, 001h, 000h, 005h, 010h
                        DB 02Ah, 02Eh, 08Bh, 001h, 008h, 001h, 018h, 080h, 002h, 090h, 001h, 000h, 008h, 010h
                        DB 06Eh, 02Fh, 011h, 001h, 007h, 001h, 010h, 080h, 002h, 0E0h, 001h, 000h, 005h, 010h
                        DB 069h, 030h, 012h, 001h, 008h, 001h, 018h, 080h, 002h, 0E0h, 001h, 000h, 008h, 010h
                        DB 070h, 031h, 014h, 001h, 007h, 001h, 010h, 020h, 003h, 058h, 002h, 040h, 006h, 010h
                        DB 071h, 032h, 015h, 001h, 008h, 001h, 018h, 020h, 003h, 058h, 002h, 000h, 010h, 010h
                        DB 072h, 033h, 017h, 001h, 007h, 001h, 010h, 000h, 004h, 000h, 003h, 000h, 008h, 010h
                        DB 073h, 034h, 018h, 001h, 008h, 001h, 018h, 000h, 004h, 000h, 003h, 000h, 010h, 010h
                        DB 074h, 035h, 01Ah, 001h, 007h, 001h, 010h, 000h, 005h, 000h, 004h, 000h, 00Ah, 010h
                        DB 075h, 036h, 01Bh, 001h, 008h, 001h, 018h, 000h, 005h, 000h, 004h, 000h, 010h, 010h
                        DB 0FFh

;Offset 0x7b11
Data0x7b11              DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h
                        DB 001h, 000h, 001h, 000h, 000h, 000h
                        DB 001h, 000h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 003h, 000h, 03Eh, 0BCh
                        DB 001h, 000h, 003h, 000h, 03Eh, 0BCh
                        DB 001h, 051h, 0FFh, 000h, 058h, 0B3h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 001h, 000h, 0FFh, 000h, 05Dh, 0E1h
                        DB 000h, 000h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 000h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 004h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 002h, 000h, 000h, 000h
                        DB 000h, 000h, 001h, 000h, 000h, 000h
                        DB 000h, 000h, 0FFh, 000h, 0F4h, 0AEh
                        DB 000h, 000h, 0FFh, 000h, 0F4h, 0AEh
                        DB 000h, 000h, 0FFh, 000h, 0F4h, 0AEh
                        DB 000h, 000h, 0FFh, 000h, 0F4h, 0AEh
                        DB 005h, 000h, 001h, 000h, 0EAh, 0D1h
                        DB 005h, 000h, 002h, 000h, 0EAh, 0D1h
                        DB 005h, 000h, 003h, 000h, 0F4h, 056h
                        DB 005h, 000h, 0FFh, 000h, 0F4h, 08Dh
                        DB 00Dh, 000h, 001h, 021h, 0EAh, 0D1h
                        DB 015h, 000h, 001h, 021h, 0EAh, 0D1h
                        DB 00Dh, 000h, 002h, 022h, 0EAh, 0D1h
                        DB 015h, 000h, 002h, 022h, 0EAh, 0D1h
                        DB 00Dh, 000h, 003h, 023h, 0F4h, 056h
                        DB 015h, 000h, 003h, 023h, 0F4h, 056h
                        DB 00Dh, 000h, 0FFh, 024h, 0F4h, 08Dh
                        DB 015h, 000h, 0FFh, 024h, 0F4h, 08Dh
                        DB 009h, 000h, 001h, 008h, 000h, 000h
                        DB 011h, 000h, 001h, 008h, 000h, 000h
                        DB 009h, 000h, 002h, 009h, 000h, 000h
                        DB 011h, 000h, 002h, 009h, 000h, 000h
                        DB 009h, 000h, 003h, 00Bh, 03Eh, 0BCh
                        DB 011h, 000h, 003h, 00Bh, 03Eh, 0BCh
                        DB 009h, 000h, 0FFh, 00Fh, 05Dh, 0E1h
                        DB 011h, 000h, 0FFh, 00Fh, 05Dh, 0E1h
                        DB 009h, 051h, 0FFh, 00Ch, 058h, 0B3h
                        DB 011h, 051h, 0FFh, 00Ch, 058h, 0B3h

;Offset 0x7c5b
                        DB 078h, 03Dh, 000h, 075h, 05Dh, 080h, 056h, 080h, 056h, 0BAh, 00Ch
                        DB 078h, 03Dh, 000h, 07Dh, 05Dh, 080h, 055h, 080h, 056h, 03Eh, 0F0h
                        DB 078h, 03Dh, 000h, 075h, 05Dh, 080h, 056h, 080h, 056h, 0BAh, 00Ch
                        DB 0FCh, 0AEh, 002h, 020h, 069h, 080h, 06Dh, 01Bh, 0DDh, 0F8h, 088h
                        DB 078h, 03Dh, 000h, 07Dh, 02Ch, 090h, 02Bh, 080h, 056h, 03Eh, 0F0h
                        DB 0C0h, 065h, 001h, 004h, 071h, 080h, 058h, 080h, 06Fh, 0BAh, 016h
                        DB 0C0h, 065h, 001h, 004h, 071h, 080h, 058h, 080h, 06Fh, 03Eh, 0F0h
                        DB 0C0h, 065h, 001h, 004h, 071h, 080h, 058h, 080h, 06Fh, 0BAh, 016h
                        DB 084h, 056h, 003h, 050h, 073h, 096h, 070h, 095h, 0ECh, 0F0h, 08Ch
                        DB 0C0h, 065h, 001h, 004h, 036h, 090h, 02Dh, 080h, 06Fh, 03Eh, 0F0h

;Offset 0x7cc9
MemorySizeMessage       DB 'Video Memory Size: '

;Offset 0x7cdc
Mem4096KMessageLength   DB 005h

;Offset 0x7cee
Mem4096KMessage         DB '4096K'

;Offset 0x7ce2
Mem8192KMessageLength   DB 005h

;Offset 0x7ce3
Mem8192KMessage         DB '8192K'

;Offset 0x7ce8
Mem16384KMessageLength  DB 006h

;Offset 0x7ce9
Mem16384KMessage        DB '16384K'

;Offset 0x7cef
Mem4096K                DB 010h

;Offset 0x7cf0
Mem4096KMessagePointer  DW offset Mem4096KMessageLength

;Offset 0x7cf2
Mem8192K                DB 020h

;Offset 0x7cf3
Mem8192KMessagePointer  DW offset Mem8192KMessageLength

;Offset 0x7cf5
Mem16384K               DB 040h

;Offset 0x7cf6
Mem16384KMessagePointer DW offset Mem16384KMessageLength

;Offset 0x7cf8
                        DB 000h

;Offset 0x7cf9
OEMConfigTable:
PCIInitDefault          DD 00180F840h

;Offset 0x7cfd
MiscInit0Default        DD 000000000h

;Offset 0x7d01
MiscInit1Default        DD 000000001h

;Offset 0x7d05
DRAMInit0Default        DD 00017A16Ah

;Offset 0x7d09
DRAMInit1Default        DD 000548031h

;Offset 0x7d0d
AGPInitDefault          DD 00000049Eh

;Offset 0x7d11
PLLControl1Default      DD 000002805h

;Offset 0x7d15
PLLControl2Default      DD 000007919h

;Offset 0x7d19
DRamDataDefault         DD 000000037h

;Offset 0x7d1d
DB 090h, 000h, 000h

;Offset 0x7D20
PCIHeader               DB 'PCIR'       ;PCI header magic
                        DW PCI_VENDOR_3Dfx;Vendor ID
                        DW PCI_DEVICE_Banshee;Device ID
                        DW 0000h        ;Pointer to vital product data (0 = none)
                        DW 0018h        ;PCI data structure length
                        DB 00h          ;PCI data structure revision
                        DB 00h, 00h, 03h;PCI device class code
                        DW (ROMEnd - ROMStart)/512;Rom size in 512 byte blocks
                        DW 0000h        ;Revision level of code
                        DB 00h          ;Code type (0 = x86)
                        DB 80h          ;Last image indicator
                        DW 0000h        ;Reserved

;Offset 0x7d38
VESAResponseTemplate    DB 'VESA'                                   ;00h  4 BYTEs   (ret) signature ("VESA")
                        DW 0102h                                    ;04h    WORD    VESA version number (one-digit minor version -- 0102h = v1.2)
                        DW offset OEMName, 0000h                    ;06h    DWORD   pointer to OEM name ;Offset 0x7dbe
                        DD 00000001h                                ;0Ah    DWORD   capabilities flags
                        DW offset SupportedVESAVideoModes, 0000h    ;0Eh    DWORD   pointer to list of supported VESA and OEM video modes ;Offset 0x7d4c
                        DW 0004h                                    ;12h    WORD    total amount of video memory in 64K blocks
                                                                    ;14h    WORD    OEM software version (BCD, high byte = major, low byte = minor)
                                                                    ;16h    DWORD   pointer to vendor name
                                                                    ;1Ah    DWORD   pointer to product name
                                                                    ;1Eh    DWORD   pointer to product revision string

;Offset 0x7d4c
SupportedVESAVideoModes DW 00100h
                        DW 00101h
                        DW 00102h
                        DW 00103h
                        DW 00105h
                        DW 00107h
                        DW 00108h
                        DW 00109h
                        DW 0010ah
                        DW 0010bh
                        DW 0010ch
                        DW 00180h
                        DW 0010eh
                        DW 0010fh
                        DW 00181h
                        DW 00182h
                        DW 00183h
                        DW 00184h
                        DW 00185h
                        DW 00186h
                        DW 00187h
                        DW 00188h
                        DW 00189h
                        DW 0018ah
                        DW 0018bh
                        DW 00111h
                        DW 00112h
                        DW 00114h
                        DW 00115h
                        DW 00117h
                        DW 00118h
                        DW 0011ah
                        DW 0011bh
                        DW 0ffffh

;Offset 0x7d90
VendorName              DB 'Elpin Systems, Inc.', 000h

;Offset 0x7da4
ProductName             DB '3Dfx Banshee', 000h

;Offset 0x7db1
RevisionString          DB 'Version 1.00', 000h

;Offset 0x7dbe
OEMName                 DB '3Dfx Interactive, Inc.', 000h

;Offset 0x7dd5
Data0x7dd5              DB 005h, 00Bh, 006h, 005h, 005h, 000h, 000h, 000h, 000h

;Offset 0x7dde
Data0x7dde              DB 008h, 010h, 008h, 008h, 008h, 000h, 000h, 000h, 000h

;Offset 0x7de7
ProtectedModeTable      DW 0008h        ;00h    WORD    offset (within table) of protected-mode code for Function 5 (Set Window) Points to 0x7def
                        DW 0029h        ;02h    WORD    offset of protected-mode code for Function 7 (Set Disp Start) Points to 0x7e10
                        DW 0053h        ;04h    WORD    offset of protected-mode code for Function 9 (Set Primary Palette)
                        DW 009bh        ;06h    WORD    offset (within table) of list of I/O ports and memory locations
                                        ;               for which I/O privilege may be required in protected mode Points to 0x7e82
                                        ;               (0000h if no list) (see #04085)

;Can't have 16-bit tiny code and 32-bit flat code in the same file, so here is the data with code as comment:

                                        ;SetWindow32bit PROC NEAR                ;Offset 0x7def
DB 060h                                 ;    pushad
DB 02Bh, 0DBh                           ;    sub    ebx, ebx
DB 08Ah, 0DAh                           ;    mov    bl, dl
DB 0D1h, 0E3h                           ;    shl    ebx, 01h
DB 0E8h, 075h, 000h, 000h, 000h         ;    call   ReadPCIConfigRegister        ;Offset 0x7e70
DB 066h, 083h, 0C2h, 02Ch               ;    add    dx, 002ch
DB 0EDh                                 ;    in     eax, dx
DB 025h, 000h, 000h, 0F0h, 0FFh         ;    and    eax, 0fff00000h
DB 066h, 08Bh, 0C3h                     ;    mov    ax, bx
DB 0C1h, 0E3h, 00Ah                     ;    shl    ebx, 0ah
DB 00Bh, 0C3h                           ;    or     eax, ebx
DB 0EFh                                 ;    out    dx, eax
DB 061h                                 ;    popad
DB 0C3h                                 ;    ret
                                        ;SetWindow32bit ENDP

                                        ;SetDisplayStart32bit PROC NEAR          ;Offset 0x7e10
DB 060h                                 ;    pushad
DB 0F6h, 0C3h, 080h                     ;    test   bl, 080h
DB 074h, 00Bh                           ;    je     Label0x7e21                  ;Offset 0x7e21
DB 052h                                 ;    push   edx
DB 066h, 0BAh, 0DAh, 003h               ;    mov    dx, VGA_InputStatus1D        ;Port 0x3da
                                        ;Label0x7e1b:                            ;Offset 0x7e1b
DB 0ECh                                 ;    in     al, dx
DB 0A8h, 008h                           ;    test   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0FBh                           ;    jne    Label0x7e1b                  ;Offset 0x7e1b
DB 05Ah                                 ;    pop    edx
                                        ;Label0x7e21:                            ;Offset 0x7e21
DB 066h, 08Bh, 0C2h                     ;    mov    ax, dx
DB 0C1h, 0E0h, 010h                     ;    shl    eax, 10h
DB 066h, 08Bh, 0C1h                     ;    mov    ax, cx
DB 0E8h, 041h, 000h, 000h, 000h         ;    call   ReadPCIConfigRegister        ;Offset 0x7e70
DB 0C1h, 0E0h, 002h                     ;    shl    eax, 02h
DB 066h, 081h, 0C2h, 0E4h, 000h         ;    add    dx, 00e4h
DB 0EFh                                 ;    out    dx, eax
DB 061h                                 ;    popad
DB 0C3h                                 ;    ret
                                        ;SetDisplayStart32bit ENDP

                                        ;SetPrimaryPalette32bit PROC NEAR        ;Offset 0x7e3a
DB 060h                                 ;    pushad
DB 01Eh                                 ;    push   ds
DB 0F6h, 0C3h, 080h                     ;    test   bl, 80h
DB 074h, 00Bh                           ;    je     Label0x7e4c                  ;Offset 0x7e4c
DB 052h                                 ;    push   edx
DB 066h, 0BAh, 0DAh, 003h               ;    mov    dx, VGA_InputStatus1D        ;Port 0x3da
                                        ;Label0x7e46:                            ;Offset 0x7e46
DB 0ECh                                 ;    in     al, dx
DB 0A8h, 008h                           ;    test   al, VGA_INSTS1_VerInactiveDisplayIntv;0x8
DB 075h, 0FBh                           ;    jne    Label0x7e46                  ;Offset 0x7e46
DB 05Ah                                 ;    pop    edx
                                        ;Label0x7e4c:                            ;Offset 0x7e4c
DB 0FCh                                 ;    cld
DB 006h                                 ;    push   es
DB 01Fh                                 ;    pop    ds
DB 08Bh, 0F7h                           ;    mov    esi, edi
DB 00Fh, 0B7h, 0C9h                     ;    movzx  ecx, cx
DB 08Ah, 0C2h                           ;    mov    al, dl
DB 066h, 0BAh, 0C8h, 003h               ;    mov    dx, VGA_DACWriteIndex        ;Port 0x3c8
DB 0EEh                                 ;    out    dx, al
DB 042h                                 ;    inc    edx
                                        ;Label0x7e5c:                            ;Offset 0x7e5c
DB 0ADh                                 ;    lodsd  eax, dword ptr [esi]
DB 0C1h, 0C8h, 010h                     ;    ror    eax, 10h
DB 0EEh                                 ;    out    dx, al
DB 0C1h, 0C0h, 008h                     ;    rol    eax, 08h
DB 0EEh                                 ;    out    dx, al
DB 0C1h, 0C0h, 008h                     ;    rol    eax, 08h
DB 0EEh                                 ;    out    dx, al
DB 0E2h, 0F1h                           ;    loop   Label0x7e5c                  ;Offset 0x7e5c
DB 01Fh                                 ;    pop    ds
DB 061h                                 ;    popad
DB 0C3h                                 ;    ret
                                        ;SetPrimaryPalette32bit ENDP

DB 090h                                 ;    nop
DB 090h                                 ;    nop

                                        ;ReadPCIConfigRegister PROC NEAR         ;Offset 0x7e70
DB 050h                                 ;    push   eax
DB 066h, 0BAh, 0D4h, 003h               ;    mov    dx, VGA_CRTControllerIndexD;Port 0x3d4
DB 0B0h, 01Ch                           ;    mov    al, 01ch      ;Banshee PCI Config readback register
DB 0EEh                                 ;    out    dx, al
DB 042h                                 ;    inc    edx           ;VGA_CRTControllerDataD
DB 0ECh                                 ;    in     al, dx
DB 00Fh, 0B6h, 0D0h                     ;    movzx  edx, al
DB 0C1h, 0E2h, 008h                     ;    shl    edx, 08h
DB 058h                                 ;    pop    eax
DB 0C3h                                 ;    ret
                                        ;ReadPCIConfigRegister ENDP

;Offset 0x7e82
                        DW 003c8h       ;port list
                        DW 003c9h
                        DW 003d4h
                        DW 003d5h
                        DW 003dah
                        DW 0ffffh
                        DW 0ffffh       ;memory list

;Offset 0x7e90
DB 168h dup (0FFh)

;Offset 0x7ff8
                        DW PCI_VENDOR_3Dfx
                        DW PCI_DEVICE_Banshee

DB 0FFh, 0FFh, 0FFh

DB 0C1h
ROMEnd:
END
