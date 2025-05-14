;ATI Mach32 BIOS disassembly

;Build with MASM: ml /AT m32.asm
;Produces a .COM file. Just rename to .BIN

.MODEL tiny
.386
.CODE
ORG 0000h

ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

    jmp short entry


ORG 80h
entry:                                  ;offset 0x80
    jmp VideoBootstrap


ORG 100h
VideoBootstrap PROC FAR                 ;offset 0x100
    push ax
    mov  al, 40h
    mov  byte ptr cs:[0002h], al        ;offset 0x0002
    mov  ax, cs
    mov  word ptr cs:[23f6h], ax        ;offset 0x23f6
    mov  word ptr cs:[2406h], ax        ;offset 0x2406
    mov  word ptr cs:[2418h], ax        ;offset 0x2418
    pop  ax
    push ds
    push es
    push di
    push si
    push bp
    push dx
    push cx
    push bx
    push ax
    push ax
    call Func57d                        ;offset 0x57d
    pop  ax
    call Func146                        ;offset 0x146
    call Func6e8f                       ;offset 0x6e8f
    call Func85d                        ;offset 0x85d
    pop  ax
    pop  bx
    pop  cx
    pop  dx
    pop  bp
    pop  si
    pop  di
    pop  es
    pop  ds
    retf
VideoBootstrap ENDP

ORG 146h
Func146 PROC NEAR                       ;offset 0x146
    ret
Func146 ENDP

ORG 57dh
Func57d PROC NEAR                       ;offset 0x57d
    mov  dx, 46e8h                      ;port 0x46e8
    mov  al, 16h
    out  dx, al
    mov  dx, 102h                       ;port 0x102
    mov  al, 01h
    out  dx, al
    mov  dx, 46e8h                      ;port 0x46e8
    mov  al, 0eh
    out  dx, al
    mov  al, 04h
    mov  dx, 3c2h                       ;port 0x3c2
    out  dx, al
    mov  dl, 0dah                       ;port 0x3da
    in   al, dx
    mov  dl, 0bah                       ;port 0x3ba
    in   al, dx
    mov  dl, 0c0h                       ;port 0x3c0
    xor  al, al
    out  dx, al
    mov  dl, 0c4h                       ;port 0x3c4
    mov  ax, 2001h
    out  dx, ax
    call GetSystemSegment               ;offset 0x16e8
    cli
    mov  ax, 0f065h                     ;offset 0xf065
    mov  word ptr ds:[108h], ax         ;offset 0x108
    mov  ax, 0f000h                     ;segment 0xf000
    mov  word ptr ds:[10ah], ax         ;offset 0x10a
    mov  word ptr ds:[7ch], 3c18h       ;offset 0x7c offset 0x3c18
    mov  word ptr ds:[7eh], cs          ;offset 0x7e
    mov  word ptr ds:[10ch], 4018h      ;offset 0x10c offset 0x4018
    mov  word ptr ds:[10eh], cs         ;offset 0x10e
    mov  word ptr ds:[1b4h], offset InterruptHandlerEntrypoint;offset 0x1b4 offset 0x234b
    mov  word ptr ds:[1b6h], cs         ;offset 0x1b6
    mov  word ptr ds:[40h], offset InterruptHandlerEntrypoint;offset 0x40 offset 0x234b
    mov  word ptr ds:[42h], cs          ;offset 0x42
    mov  word ptr ds:[4a8h], 23f4h      ;offset 0x4a8 offset 0x23f4
    mov  word ptr ds:[4aah], cs         ;offset 0x4aa
    ret  
Func57d ENDP

ORG 85dh
Func85d PROC NEAR                       ;offset 0x85d
    ret
Func85d ENDP

Data1450                DB 020h, 0E5h, 040h, 009h, 0E5h, 084h, 010h, 0E5h, 000h, 009h, 0E5h, 0A4h, 008h, 0E5h, 080h
                        DB 009h, 0E5h, 0E4h, 004h, 065h, 0C0h, 008h, 0E5h, 0E4h, 001h, 0E5h, 0C0h, 009h, 0E5h, 0E4h
Data1450_end:
Data146e                DB 008h, 000h, 080h, 00Bh, 0E5h, 020h, 004h, 000h, 040h, 00Bh, 0E5h, 020h
                        DB 002h, 000h, 000h, 00Bh, 0E5h, 020h, 001h, 000h, 0C0h, 00Ah, 0E5h, 020h
Data146e_end:

Func1486 PROC NEAR                      ;offset 0x1486
    mov  ah, byte ptr [449h]            ;offset 0x449
Func1486 ENDP
;continue!
Func148a PROC NEAR                      ;offset 0x148a
    les bx, ds:[4a8h]                   ;offset 0x4a8
    les bx, es:[bx]
    mov dx, bx
    or  ah, ah
    jge Label1498                       ;offset 0x1498
    ret
Label1498:                              ;offset 0x1498
    cmp  ah, 6ah
    je   Label14a2                      ;offset 0x14a2
    cmp  ah, 53h
    jne  Label14a4                      ;offset 0x14a4
Label14a2:                              ;offset 0x14a2
    mov  ah, 54h
Label14a4:                              ;offset 0x14a4
    push ax
    call Func14b1                       ;offset 0x14b1
    pop  ax
    cmp  bx, dx
    jne  Label14b0                      ;offset 0x14b0
    call Func157a                       ;offset 0x157a
Label14b0:                              ;offset 0x14b0
    ret
Func148a ENDP

;Get legacy VideoParameters?
ORG 14b1h
Func14b1 PROC NEAR                      ;offset 0x14b1
    xor  si, si
    cmp  ah, 03h
    jg   Label150e                      ;offset 0x150e
    dec  dx
    mov  al, 0b1h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 01h
    jne  Label14cd                      ;offset 0x14cd
    test al, 04h
    jne  Label1501                      ;offset 0x1501
    test byte ptr ds:[489h], 10h        ;offset 0x489
    jne  Label14ed                      ;offset 0x14ed
Label14cd:                              ;offset 0x14cd
    mov  al, byte ptr ds:[488h]         ;offset 0x488
    and  al, 0fh
    cmp  al, 02h
    jle  Label14fa                      ;offset 0x14fa
    cmp  al, 08h
    je   Label14fa                      ;offset 0x14fa
    cmp  al, 06h
    je   Label14fa                      ;offset 0x14fa
    cmp  al, 07h
    je   Label14fa                      ;offset 0x14fa
    mov  al, 40h
    mul  ah
    add  bx, ax
    add  bx, 4c0h                       ;offset 0x4c0
    ret
Label14ed:                              ;offset 0x14ed
    mov   al, 40h
    shr   ah, 01h
    mul   ah
    add   bx, ax
    add   bx, 5c0h                      ;offset 0x5c0
    ret
Label14fa:                              ;offset 0x14fa
    mov   al, 40h
    mul   ah
    add   bx, ax
    ret
Label1501:                              ;offset 0x1501
    mov   al, 40h
    shr   ah, 01h
    mul   ah
    add   bx, ax
    add   bx, 240h                      ;offset 0x240
    ret
Label150e:                              ;offset 0x150e
    cmp   ah, 07h
    jne   Label1536                     ;offset 0x1536
    mov   al, 0b1h
    call  ReadExtendedRegisterByte      ;offset 0x1b9f
    test  al, 04h
    jne   Label1531                     ;offset 0x1531
    test  al, 01h
    jne   Label1527                     ;offset 0x1527
    test  byte ptr ds:[489h], 10h       ;offset 0x489
    jne   Label152c                     ;offset 0x152c
Label1527:                              ;offset 0x1527
    add   bx, 1c0h                      ;offset 0x1c0
    ret
Label152c:                              ;offset 0x152c
    add   bx, 640h                      ;offset 0x640
    ret
Label1531:                              ;offset 0x1531
    add   bx, 200h                      ;offset 0x200
    ret
Label1536:                              ;offset 0x1536
    call  Func1618                      ;offset 0x1618
    cmp   bx, dx
    je    Label153e                     ;offset 0x153e
    ret
Label153e:                              ;offset 0x153e
    mov   di, offset Data154a           ;offset 0x154a
    mov   si, offset Data154a_end       ;offset 0x157a
    xor   cx, cx
    call  Func1f45                      ;offset 0x1f45
    ret
Func14b1 ENDP

Data154a                DB 004h, 000h, 001h
                        DB 005h, 040h, 001h
                        DB 006h, 080h, 001h
                        DB 00Dh, 040h, 003h
                        DB 00Eh, 080h, 003h
                        DB 00Fh, 040h, 004h
                        DB 010h, 080h, 004h
                        DB 011h, 080h, 006h
                        DB 012h, 0C0h, 006h
                        DB 013h, 000h, 007h
                        DB 062h, 080h, 008h
                        DB 061h, 040h, 008h
                        DB 027h, 040h, 007h
                        DB 023h, 080h, 007h
                        DB 037h, 0C0h, 007h
                        DB 033h, 000h, 008h
                        DB 033h, 0C9h, 0BFh
                        DB 010h, 016h, 0EBh
Data154a_end:

Func157a PROC NEAR                      ;offset 0x157a
    xor  cx, cx
    mov  di, offset Data1610            ;offset 0x1610
    jmp  Label1593                      ;offset 0x1593
Label1581:                              ;offset 0x1581
    cmp  byte ptr cs:[di], ah
    jne  Label1590                      ;offset 0x1590
    mov  ah, byte ptr cs:[di + 01h]
    mov  cx, word ptr cs:[di + 02h]
    jmp  Label1599                      ;offset 0x1599
Label1590:                              ;offset 0x1590
    add  di, 04h
Label1593:                              ;offset 0x1593
    cmp  di, offset Data1610_end        ;offset 0x1618
    jne  Label1581                      ;offset 0x1581
Label1599:                              ;offset 0x1599
    push dx
    mov  al, 81h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    mov  di, offset Data1450            ;offset 0x1450
    mov  si, offset Data1450_end        ;offset 0x146e
    cmp  ah, 54h
    je   Label15bc                      ;offset 0x15bc
    mov  al, 80h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    mov  di, offset Data146e            ;offset 0x146e
    mov  si, offset Data146e_end        ;offset 0x1486
    cmp  ah, 55h
    je   Label15bc                      ;offset 0x15bc
    pop  dx
    ret
Label15bc:                              ;offset 0x15bc
    pop  dx
    mov  bx, dx
    or   al, al
    jne  Label15c4                      ;offset 0x15c4
    ret
Label15c4:                              ;offset 0x15c4
    push ax
    mov  ah, byte ptr cs:[di]
    and  al, ah
    cmp  ah, al
    pop  ax
    jne  Label1608                      ;offset 0x1608
    add  bx, word ptr cs:[di + 02h]
    push ax
    mov  ah, byte ptr es:[bx + 06h]
    mov  si, cx
    or   si, si
    je   Label15e2                      ;offset 0x15e2
    mov  ah, byte ptr es:[si + 15h]
Label15e2:                              ;offset 0x15e2
    and  ah, 60h
    je   Label1606                      ;offset 0x1606
    mov  al, 0b0h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 18h
    je   Label1601                      ;offset 0x1601
    not  al
    and  al, 18h
    cmp  ah, 60h
    je   Label1601                      ;offset 0x1601
    shr  ah, 01h
    shr  ah, 01h
    cmp  al, ah
    jge  Label1606                      ;offset 0x1606
Label1601:                              ;offset 0x1601
    mov  bx, dx
    pop  ax
    jmp  Label1608                      ;offset 0x1608
Label1606:                              ;offset 0x1606
    pop  ax
    ret
Label1608:                              ;offset 0x1608
    add  di, 06h
    cmp  di, si
    jne  Label15c4                      ;offset 0x15c4
    ret
Func157a ENDP

Data1610                DB 063h, 054h, 03Ah, 014h
                        DB 064h, 055h, 03Ah, 014h
Data1610_end:

Func1618 PROC NEAR                      ;offset 0x1618
    cmp  ah, 10h
    jle  Label1631                      ;offset 0x1631
    push ax
    mov  al, 0bah
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 01h
    pop  ax
    je   Label1631                      ;offset 0x1631
    mov  di, offset Data1632            ;offset 0x1632
    mov  si, offset Data1632_end        ;offset 0x163b
    call Func1f45                       ;offset 0x1f45
Label1631:                              ;offset 0x1631
    ret
Func1618 ENDP

Data1632                DB 011h, 000h, 00Ah
                        DB 012h, 040h, 00Ah
                        DB 062h, 080h, 00Ah
Data1632_end:
DB 0 ;Delete this, it's only here to keep the label alive!

ORG 16deh
Func16de PROC NEAR                      ;offset 0x16de
    ret
Func16de ENDP

ORG 16e8h
GetSystemSegment PROC NEAR              ;offset 0x16e8
    push ax
    xor  ax, ax
    mov  ds, ax
    pop  ax
    ret
GetSystemSegment ENDP

;output
;dx = Input Status 1 register
GetInputStatus1 PROC NEAR               ;offset 0x16ef
    push ds
    xor  dx, dx
    mov  ds, dx
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    add  dl, 06h
    pop  ds
    ret
GetInputStatus1 ENDP

;input
;ah = start index
;ch = first value
;cl = second value
ORG 17b6h
WriteTwoConsecutiveCRTCRegisters PROC NEAR;offset 0x17b6
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, ch
    xchg al, ah
    out  dx, ax
    mov  ah, cl
    inc  al
    out  dx, ax
    ret
WriteTwoConsecutiveCRTCRegisters ENDP

;output:
;dx = InputStatus1
;al = value read from InputStatus1
ORG 1857h
ReadInputStatus1 PROC NEAR              ;offset 0x1857
    call GetInputStatus1                ;offset 0x16ef
    in   al, dx
    ret
ReadInputStatus1 ENDP

EnableAttributePaletteRegisters PROC NEAR;offset 0x185c
    call ReadInputStatus1               ;offset 0x1857
    mov  dl, 0c0h                       ;port 0x3c0 AttributeControllerIndex
    mov  al, 20h                        ;Enable display use of Attribute palette registers
    out  dx, al
    ret
EnableAttributePaletteRegisters ENDP

MakeAttributePaletteRegistersWritable PROC NEAR;offset 0x1865
    call ReadInputStatus1               ;offset 0x1857
    mov  dl, 0c0h                       ;port 0x3c0 AttributeControllerIndex
    mov  al, 00h                        ;Enable CPU access of Attribute palette registers
    out  dx, al
    ret
MakeAttributePaletteRegistersWritable ENDP

ORG 186eh
Func186e PROC NEAR                      ;offset 0x186e
    ret
Func186e ENDP

ORG 1a9eh
Func1a9e PROC NEAR                      ;offset 0x1a9e
    call GetSystemSegment               ;offset 0x16e8
    mov  word ptr ds:[460h], cx         ;offset 0x460
    mov  al, ch
    and  al, 60h
    test al, 20h
    je   Label1ab6                      ;offset 0x1ab6
    mov  cx, 1e00h
    jmp  Label1b4d                      ;offset 0x1b4d
Label1ab3:                              ;offset 0x1ab3
    jmp  Label1b4d                      ;offset 0x1b4d
Label1ab6:                              ;offset 0x1ab6
    test byte ptr ds:[487h], 01h        ;offset 0x487
    jne  Label1ab3                      ;offset 0x1ab3
    cmp  byte ptr ds:[449h], 07h        ;offset 0x449
    je   Label1acc                      ;offset 0x1acc
    cmp  byte ptr ds:[449h], 04h        ;offset 0x449
    jl   Label1acc                      ;offset 0x1acc
    ret
Label1acc:                              ;offset 0x1acc
    test byte ptr ds:[487h], 08h        ;offset 0x487
    je   Label1ad6                      ;offset 0x1ad6
    jmp  Label23a3                      ;offset 0x23a3
Label1ad6:                              ;offset 0x1ad6
    mov  dh, byte ptr ds:[485h]         ;offset 0x485
    mov  dl, dh
    dec  dl
    cmp  ch, cl
    jle  Label1b02                      ;offset 0x1b02
    or   cl, cl
    je   Label1ab3                      ;offset 0x1ab3
    mov  ch, cl
    mov  cl, dh
    dec  cl
    jmp  Label1b4d                      ;offset 0x1b4d
    nop
Label1aef:                              ;offset 0x1aef
    cmp  cl, dl
    je   Label1b4d                      ;offset 0x1b4d
Label1af3:                              ;offset 0x1af3
    mov  cl, dh
    cmp  ch, 03h
    jl   Label1afe                      ;offset 0x1afe
    mov  ch, cl
    shr  ch, 01h
Label1afe:                              ;offset 0x1afe
    dec  cl
    jmp  Label1b4d                      ;offset 0x1b4d
Label1b02:                              ;offset 0x1b02
    cmp  cl, 03h
    jle  Label1b4d                      ;offset 0x1b4d
    mov  ah, cl
    sub  ah, ch
    or   ah, ah
    jne  Label1b19                      ;offset 0x1b19
    cmp  dl, cl
    je   Label1b4d                      ;offset 0x1b4d
    dec  dl
    cmp  dl, cl
    je   Label1b4d                      ;offset 0x1b4d
Label1b19:                              ;offset 0x1b19
    mov  al, ch
    or   al, cl
    cmp  al, dh
    jle  Label1b2b                      ;offset 0x1b2b
    cmp  ah, 02h
    jle  Label1b38                      ;offset 0x1b38
    cmp  ch, 02h
    jg   Label1af3                      ;offset 0x1af3
Label1b2b:                              ;offset 0x1b2b
    cmp  ah, 03h
    jge  Label1aef                      ;offset 0x1aef
    mov  dl, dh
    dec  dl
    cmp  cl, dl
    je   Label1b4d                      ;offset 0x1b4d
Label1b38:                              ;offset 0x1b38
    mov  cl, dh
    dec  cl
    dec  cl
    mov  ch, cl
    sub  ch, ah
    cmp  dh, 08h
    jg   Label1b4d                      ;offset 0x1b4d
    inc  cl
    inc  ch
    jmp  Label1b4d                      ;offset 0x1b4d
Label1b4d:                              ;offset 0x1b4d
    mov  al, 0b8h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    push ax
    test al, 04h
    je   Label1b5d                      ;offset 0x1b5d
    mov  ax, 0fbb8h
    call DisableExtendedRegisterBits    ;offset 0x1b83
Label1b5d:                              ;offset 0x1b5d
    mov  ah, 0ah
    call WriteTwoConsecutiveCRTCRegisters;offset 0x17b6
    pop  ax
    test al, 04h
    je   Label1b6d                      ;offset 0x1b6d
    mov  ax, 4b8h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label1b6d:                              ;offset 0x1b6d
    ret
Func1a9e ENDP

;input
;al = Extended Register index
;ah = bits to set to 1
;output
;ah = current state of extended register
EnableExtendedRegisterBits PROC NEAR    ;offset 0x1b6e
    push dx
    push cx
    mov  dx, 1ceh                       ;port 0x1ce
    cli
    out  dx, al
    mov  cl, al
    inc  dx
    in   al, dx
    or   ah, al
    mov  al, cl
    dec  dx
    out  dx, ax
    sti
    pop  cx
    pop  dx
    ret
EnableExtendedRegisterBits ENDP

;input
;al = index
;ah = bits to preserve should be 1, bits that are 0 wil be set to 0
;output
;ah = current state of extended register
DisableExtendedRegisterBits PROC NEAR   ;offset 0x1b83
    push dx
    push cx
    mov  dx, 1ceh                       ;port 0x1ce
    cli
    out  dx, al
    mov  cl, al
    inc  dx
    in   al, dx
    and  ah, al
    mov  al, cl
    dec  dx
    out  dx, ax
    sti
    pop  cx
    pop  dx
    ret
DisableExtendedRegisterBits ENDP

WriteExtendedRegisterByte PROC NEAR     ;offset 0x1b98
    push dx
    mov  dx, 1ceh                       ;port 0x1ce
    out  dx, ax
    pop  dx
    ret  
WriteExtendedRegisterByte ENDP

;input:
;al = index (bits 7-6 must be '10', bits 5-0 are the index)
;output:
;al = register value
ReadExtendedRegisterByte PROC NEAR      ;offset 0x1b9f
    push dx
    mov  dx, 1ceh                       ;port 0x1ce
    cli  
    out  dx, al
    inc  dx
    in   al, dx
    sti  
    pop  dx
    ret
ReadExtendedRegisterByte ENDP

;input
;al = index
;ch = AND mask of bits to preserve
;ah = bits to set to 1
;output
;ah = current state of extended register
UpdateExtendedRegisterBits PROC NEAR    ;offset 0x1baa
    push dx
    push cx
    mov  dx, 1ceh                       ;port 0x1ce
    cli
    out  dx, al
    mov  cl, al
    inc  dx
    in   al, dx
    and  al, ch
    or   ah, al
    mov  al, cl
    dec  dx
    out  dx, ax
    sti
    pop  cx
    pop  dx
    ret
UpdateExtendedRegisterBits ENDP

;input
;ch = Write page in bits 4-1
;output
;ah = current state of extended register ATI32
;destroyed
;al
SetCPUWritePage PROC NEAR               ;offset 0x1bc1
    push cx
    mov  al, 0b2h                       ;ATI32
    mov  ah, ch
    mov  ch, 0e1h                       ;CPU Address write paging bits are masked out
    call UpdateExtendedRegisterBits     ;offset 0x1baa
    pop  cx
    ret  
SetCPUWritePage ENDP

ClearGeneralPurposeFlagATI3E4 PROC NEAR ;offset 0x1bcd
    mov  ax, 0efbeh                     ;ATI3e, clear bit 4 - General purpose read/write bit
    call DisableExtendedRegisterBits    ;offset 0x1b83
    ret  
ClearGeneralPurposeFlagATI3E4 ENDP

Func1bd4 PROC NEAR                      ;offset 0x1bd4
    test byte ptr ds:[489h], 08h        ;offset 0x489
    je   Label1bdc                      ;offset 0x1bdc
    ret
Label1bdc:                              ;offset 0x1bdc
    mov  dx, 3c4h                       ;port 0x3c4
    mov  al, 01h
    mov  ah, byte ptr es:[bx + 05h]
    push ax
    or   ah, 20h
    out  dx, ax
    mov  al, 0ffh
    mov  dl, 0c6h                       ;port 0x3c6
    out  dx, al
    mov  dl, 0c8h                       ;port 0x3c8
    cmp  byte ptr ds:[463h], 0b4h       ;offset 0x463 port 0x3b4
    jne  Label1c4a                      ;offset 0x1c4a
    mov  si, offset Data1cfa            ;offset 0x1cfa
    test byte ptr ds:[489h], 06h        ;offset 0x489
    jne  Label1c19                      ;offset 0x1c19
    mov  al, 0bbh
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    and  al, 0c0h
    je   Label1c19                      ;offset 0x1c19
    cmp  al, 0c0h
    je   Label1c19                      ;offset 0x1c19
    mov  si, offset Data1d12            ;offset 0x1d12
    cmp  al, 80h
    je   Label1c19                      ;offset 0x1c19
    mov  si, offset Data1d06            ;offset 0x1d06
Label1c19:                              ;offset 0x1c19
    mov  cx, 40h
    xor  bx, bx
Label1c1e:                              ;offset 0x1c1e
    mov  al, bl
    cli
    out  dx, al
    mov  di, bx
    shr  di, 01h
    shr  di, 01h
    shr  di, 01h
    and  di, 03h
    add  di, si
    mov  al, byte ptr cs:[di]
    inc  dx
    out  dx, al
    mov  al, byte ptr cs:[di + 04h]
    out  dx, al
    mov  al, byte ptr cs:[di + 08h]
    out  dx, al
    sti
    inc  bl
    dec  dx
    loop Label1c1e                      ;offset 0x1c1e
Label1c44:                              ;offset 0x1c44
    mov  dx, 3c4h                       ;port 0x3c4
    pop  ax
    out  dx, ax
    ret
Label1c4a:                              ;offset 0x1c4a
    mov  al, byte ptr es:[si + 08h]
    test byte ptr ds:[489h], 06h        ;offset 0x489
    je   Label1c89                      ;offset 0x1c89
    mov  cx, 0f8h
    mov  di, 67e7h                      ;offset 0x67e7
    cmp  al, 08h
    je   Label1c70                      ;offset 0x1c70
    mov  cx, 40h
    mov  di, offset Data1d1e            ;offset 0x1d1e
    cmp  al, 38h
    je   Label1c70                      ;offset 0x1c70
    cmp  al, 3fh
    je   Label1c70                      ;offset 0x1c70
    mov  di, offset Data1d5e            ;offset 0x1d5e
Label1c70:                              ;offset 0x1c70
    xor  bx, bx
Label1c72:                              ;offset 0x1c72
    mov  al, bl
    cli
    out  dx, al
    mov  al, byte ptr cs:[bx + di]
    inc  dx
    out  dx, al
    nop
    nop
    out  dx, al
    nop
    nop
    out  dx, al
    sti
    inc  bl
    dec  dx
    loop Label1c72                      ;offset 0x1c72
    jmp  Label1c44                      ;offset 0x1c44
Label1c89:                              ;offset 0x1c89
    cmp  al, 08h
    je   Label1cb2                      ;offset 0x1cb2
    cmp  al, 38h
    je   Label1cbf                      ;offset 0x1cbf
    cmp  al, 3fh
    je   Label1cbf                      ;offset 0x1cbf
    mov  cx, 08h
    xor  bx, bx
Label1c9a:                              ;offset 0x1c9a
    push cx
    mov  cx, 08h
    mov  di, 68e7h                      ;offset 0x68e7
    test bx, 10h
    je   Label1caa                      ;offset 0x1caa
    mov  di, 68ffh                      ;offset 0x68ff
Label1caa:                              ;offset 0x1caa
    call Func1dae                       ;offset 0x1dae
    pop  cx
    loop Label1c9a                      ;offset 0x1c9a
    jmp  Label1c44                      ;offset 0x1c44
Label1cb2:                              ;offset 0x1cb2
    mov  di, 68e7h                      ;offset 0x68e7
    mov  cx, 100h
    xor  bx, bx
    call Func1dae                       ;offset 0x1dae
    jmp  Label1c44                      ;offset 0x1c44
Label1cbf:                              ;offset 0x1cbf
    mov  cx, 40h
    xor  bx, bx
Label1cc4:                              ;offset 0x1cc4
    push cx
    call Func1cd5                       ;offset 0x1cd5
    mov  al, bl
    call Func62b3                       ;offset 0x62b3
    inc  bl
    pop  cx
    loop Label1cc4                      ;offset 0x1cc4
    jmp  Label1c44                      ;offset 0x1c44
Func1bd4 ENDP

Func1cd5 PROC NEAR                      ;offset 0x1cd5
    mov  di, bx
    shr  di, 01h
    shr  di, 01h
    and  di, 0fh
    mov  dh, byte ptr cs:[di + offset Data1d9e];offset 0x1d9e
    mov  di, bx
    shr  di, 01h
    and  di, 0fh
    mov  ch, byte ptr cs:[di + offset Data1d9e];offset 0x1d9e
    mov  di, bx
    and  di, 0fh
    mov  cl, byte ptr cs:[di + offset Data1d9e];offset 0x1d9e
    ret
Func1cd5 ENDP

Data1cfa                DB 000h, 02Ah, 000h, 03Fh, 000h, 02Ah, 000h, 03Fh, 000h, 02Ah, 000h, 03Fh
Data1d06                DB 000h, 000h, 000h, 01Fh, 000h, 02Ah, 000h, 03Fh, 000h, 000h, 000h, 015h
Data1d12                DB 000h, 030h, 000h, 03Fh, 000h, 015h, 000h, 015h, 000h, 000h, 000h, 000h
Data1d1e                DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 025h, 028h
                        DB 002h, 007h, 01Bh, 020h, 00Fh, 014h, 028h, 02Ch
                        DB 00Ch, 011h, 025h, 02Ah, 014h, 01Eh, 032h, 036h
                        DB 00Fh, 013h, 027h, 02Ch, 01Bh, 020h, 034h, 039h
                        DB 006h, 00Bh, 01Fh, 024h, 013h, 018h, 02Ch, 030h
                        DB 009h, 00Dh, 021h, 026h, 015h, 01Ah, 02Eh, 033h
                        DB 013h, 017h, 02Bh, 030h, 01Fh, 024h, 038h, 03Dh
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
Data1d5e                DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 018h
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 018h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 018h
                        DB 000h, 005h, 011h, 01Ch, 008h, 00Bh, 014h, 018h
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh
                        DB 00Eh, 018h, 02Dh, 032h, 020h, 024h, 038h, 03Fh 

Data1d9e                DB 000h, 02Ah, 000h, 02Ah, 000h, 02Ah, 000h, 02Ah, 015h, 03Fh, 015h, 03Fh, 015h, 03Fh, 015h, 03Fh

ORG 1daeh
Func1dae PROC NEAR                      ;offset 0x1dae
Label1dae:                              ;offset 0x1dae
    mov       al, bl
    cli       
    out       dx, al
    mov       al, byte ptr cs:[di]
    inc       dx
    nop
    out       dx, al
    inc       di
    mov       al, byte ptr cs:[di]
    nop
    out       dx, al
    inc       di
    mov       al, byte ptr cs:[di]
    nop       
    out       dx, al
    sti
    inc       di
    inc       bl
    dec       dx
    loop      Label1dae                 ;offset 0x1dae
    ret
Func1dae ENDP

ClearTextScreen PROC NEAR               ;offset 0x1dcc
    push      es
    mov       cx, 4000h
    mov       di, 0b800h                ;segment 0xb800
    test      byte ptr es:[bx + 33h], 01h;offset 0x33
    jne       Label1de8                 ;offset 0x1de8
    test      byte ptr es:[bx + 09h], 01h;offset 0x09
    jne       Label1dec                 ;offset 0x1dec
    mov       di, 0b000h                ;offset 0xb000
    mov       ch, 20h
    jmp       Label1dec                 ;offset 0x1dec
Label1de8:                              ;offset 0x1de8
    xor       ax, ax
    jmp       Label1def                 ;offset 0x1def
Label1dec:                              ;offset 0x1dec
    mov       ax, 0720h
Label1def:                              ;offset 0x1def
    mov       es, di
    xor       di, di
    rep stosw
    pop       es
    ret
ClearTextScreen ENDP

ClearScreen PROC NEAR                   ;offset 0x1df7
    push      ds
    push      es
    call      GetSystemSegment          ;offset 0x16e8
    test      byte ptr ds:[487h], 80h   ;offset 0x487
    jne       Label1e4c  ;offset 0x1e4c
    cmp       word ptr ds:[44ch], 00h   ;offset 0x44c
    je        Label1e4c  ;offset 0x1e4c
    mov       al, byte ptr ds:[449h]    ;offset 0x449
    mov       cx, 4000h
    mov       di, 0b800h                ;segment 0xb800
    test      byte ptr es:[bx + 33h], 01h;offset 0x33
    jne       Label1e26  ;offset 0x1e26
    test      byte ptr es:[bx + 09h], 01h;offset 0x09
    jne       Label1e43                 ;offset 0x1e43
    mov       di, 0b000h                ;segment 0xb000
    jmp       Label1e43                 ;offset 0x1e43
Label1e26:                              ;offset 0x1e26
    cmp       al, 06h
    jle       Label1e3f                 ;offset 0x1e3f
    mov       di, 0a000h                ;segment 0xa000
    mov       ch, 80h
    cmp       al, 13h
    jle       Label1e3f                 ;offset 0x1e3f
    cmp       al, 6ah
    je        Label1e3f                 ;offset 0x1e3f
    cmp       al, 55h
    je        Label1e4f                 ;offset 0x1e4f
    cmp       al, 61h
    jge       Label1e54                 ;offset 0x1e54
Label1e3f:                              ;offset 0x1e3f
    xor       ax, ax
    jmp       Label1e46                 ;offset 0x1e46
Label1e43:                              ;offset 0x1e43
    mov       ax, 0720h
Label1e46:                              ;offset 0x1e46
    mov       es, di
    xor       di, di
    rep stosw
Label1e4c:                              ;offset 0x1e4c
    pop       es
    pop       ds
    ret
Label1e4f:                              ;offset 0x1e4f
    mov       cx, 02h
    jmp       Label1e57                 ;offset 0x1e57
Label1e54:                              ;offset 0x1e54
    mov       cx, 10h
Label1e57:                              ;offset 0x1e57
    mov       es, di
    xor       ah, ah
Label1e5b:                              ;offset 0x1e5b
    push      cx
    push      ax
    mov       ch, ah
    call      SetCPUWritePage           ;offset 0x1bc1
    xor       di, di
    xor       ax, ax
    mov       cx, 8000h
    rep stosw
    pop       ax
    pop       cx
    add       ah, 02h
    loop      Label1e5b                 ;offset 0x1e5b
    xor       ch, ch
    call      SetCPUWritePage           ;offset 0x1bc1
    jmp       Label1e4c                 ;offset 0x1e4c
ClearScreen ENDP

Func1e79 PROC NEAR                      ;offset 0x1e79
    push      bx
    add       bx, 05h
    mov       dx, 3c4h                  ;port 0x3c4
    mov       ax, 100h
    out       dx, ax
    mov       cx, 04h
    mov       al, 01h
Label1e89:                              ;offset 0x1e89
    mov       ah, byte ptr es:[bx]
    and       ah, 1fh
    out       dx, ax
    inc       bx
    inc       al
    loop      Label1e89                 ;offset 0x1e89
    pop       bx
    call      Func218e                  ;offset 0x218e
    mov       al, byte ptr es:[bx + 09h];offset 0x09
    mov       dx, 3c2h                  ;port 0x3c2
    out       dx, al
    mov       dl, 0c4h                  ;port 0x3c4
    mov       ax, 300h
    out       dx, ax
    ret
Func1e79 ENDP

Func1ea8 PROC NEAR                      ;offset 0x1ea8
    mov       dx, 3dah                  ;port 0x3da
    test      byte ptr es:[bx + 09h], 01h;offset 0x09
    jne       Label1eb4                 ;offset 0x1eb4
    mov       dl, 0bah                  ;port 0x3ba
Label1eb4:                              ;offset 0x1eb4
    test      byte ptr ds:[489h], 08h   ;offset 0x489
    je        Label1ed8                 ;offset 0x1ed8
    add       si, 10h
    in        al, dx
    mov       cx, 04h
    mov       ah, 10h
    mov       dl, 0c0h                  ;port 0x3c0
Label1ec6:                              ;offset 0x1ec6
    cmp       ah, 11h
    je        Label1ed2                 ;offset 0x1ed2
    mov       al, ah
    out       dx, al
    mov       al, byte ptr es:[si]
    out       dx, al
Label1ed2:                              ;offset 0x1ed2
    inc       si
    inc       ah
    loop      Label1ec6                 ;offset 0x1ec6
    ret
Label1ed8:                              ;offset 0x1ed8
    push      bx
    in        al, dx
    mov       bx, si
    mov       cx, 14h
    xor       ah, ah
    mov       dl, 0c0h                  ;port 0x3c0
Label1ee3:                              ;offset 0x1ee3
    mov       al, ah
    out       dx, al
    inc       ah
    mov       al, byte ptr es:[bx]
    out       dx, al
    inc       bx
    loop      Label1ee3                 ;offset 0x1ee3
    mov       al, ah
    out       dx, al
    xor       al, al
    out       dx, al
    push      ds
    push      es
    call      Func186e                  ;offset 0x186e
    or        bp, bp
    je        Label1f07                 ;offset 0x1f07
    pop       ds
    push      ds
    mov       cx, 10h
    rep movsb
    inc       si
    movsb
Label1f07:                              ;offset 0x1f07
    pop       es
    pop       ds
    pop       bx
    ret
Func1ea8 ENDP

Func1f0b PROC NEAR                      ;offset 0x1f0b
    push      bx
    mov       dh, 03h
    mov       dl, 0c4h                  ;port 0x3c4
    mov       al, 01h
    out       dx, al
    inc       dx
    in        al, dx
    push      ax
    or        al, 20h
    dec       dx
    mov       ah, al
    mov       al, 01h
    out       dx, ax
    mov       dl, 0cch                  ;port 0x3cc
    xor       al, al
    out       dx, al
    mov       dl, 0cah
    mov       al, 01h
    out       dx, al
    add       bx, 37h
    mov       dl, 0ceh                  ;port 0x3ce
    mov       cx, 09h
    xor       al, al
Label1f32:                              ;offset 0x1f32
    mov       ah, byte ptr es:[bx]
    out       dx, ax
    inc       bx
    inc       al
    loop      Label1f32                 ;offset 0x1f32
    pop       ax
    mov       dl, 0c4h                  ;port 0x3c4
    mov       ah, al
    mov       al, 01h
    out       dx, ax
    pop       bx
    ret
Func1f0b ENDP

Func1f45 PROC NEAR                      ;offset 0x1f45
Label1f45:                              ;offset 0x1f45
    cmp  byte ptr cs:[di], ah
    jne  Label1f51                      ;offset 0x1f51
    add  bx, word ptr cs:[di + 1]
    xor  si, si
    ret
Label1f51:                              ;offset 0x1f51
    add  di, 03h
    cmp  di, si
    jne  Label1f45                      ;offset 0x1f45
    ret
Func1f45 ENDP

Func1f59 PROC NEAR                      ;offset 0x1f59
    mov  al, 0b8h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    mov  ah, al
    mov  al, 0b8h
    push ax
    and  ah, 0fbh
    call DisableExtendedRegisterBits    ;offset 0x1b83
    call Func21de                       ;offset 0x21de
    call Func1e79                       ;offset 0x1e79
    call Func1f98                       ;offset 0x1f98
    call Func1f0b                       ;offset 0x1f0b
    push si
    or   si, si
    jne  Label1f7f                      ;offset 0x1f7f
    mov  si, bx
    add  si, 23h
Label1f7f:                              ;offset 0x1f7f
    push si
    call Func1ea8                       ;offset 0x1ea8
    pop  si
    push bx
    call Func1bd4                       ;offset 0x1bd4
    pop  bx
    pop  si
    call Func2229                       ;offset 0x2229
    pop  ax
    and  ah, 04h
    je   Label1f96                      ;offset 0x1f96
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label1f96:                              ;offset 0x1f96
    sti
    ret
Func1f59 ENDP

Func1f98 PROC NEAR                      ;offset 0x1f98
    cli
    mov  al, 0b4h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    mov  ah, al
    mov  al, 0b4h
    push ax
    xor  ah, ah
    call WriteExtendedRegisterByte      ;offset 0x1b98
    push bx
    add  bx, 0ah
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  cx, 19h
    mov  ax, 3011h
    out  dx, ax
    xor  al, al
Label1fb9:                              ;offset 0x1fb9
    mov  ah, byte ptr es:[bx]
    out  dx, ax
    inc  bx
    inc  al
    loop Label1fb9                      ;offset 0x1fb9
    pop  bx
    add  dl, 06h                        ;port 0x3?a
    xor  al, al
    out  dx, al
    pop  ax
    call WriteExtendedRegisterByte      ;offset 0x1b98
    mov  al, 0b6h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 20h
    je   Label1fe4                      ;offset 0x1fe4
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, 11h
    mov  ah, byte ptr es:[bx + 1bh]     ;offset 0x1b
    or   ah, 20h
    out  dx, ax
Label1fe4:                              ;offset 0x1fe4
    sti
    ret
Func1f98 ENDP

ORG 218eh
Func218e PROC NEAR                      ;offset 0x218e
    mov  cl, 40h
    test byte ptr es:[bx + 08h], 80h    ;offset 0x08
    jne  Label219f                      ;offset 0x219f
    mov  ax, 02b9h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    jmp  Label21a7                      ;offset 0x21a7
Label219f:                              ;offset 0x219f
    mov  ax, 0fdb9h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    jmp  Label21a7                      ;offset 0x21a7
Label21a7:                              ;offset 0x21a7
    mov  ah, byte ptr es:[bx + 05h]     ;offset 0x05
    and  ah, 0c0h
    or   ah, ah
    jne  Label21c5                      ;offset 0x21c5
    mov  al, byte ptr es:[bx + 09h]     ;offset 0x09
    and  al, 0ch
    cmp  al, 04h
    jg   Label21c5                      ;offset 0x21c5
    test byte ptr es:[bx + 08h], 0e0h   ;offset 0x08
    jne  Label21c5                      ;offset 0x21c5
    mov  ah, cl
Label21c5:                              ;offset 0x21c5
    mov  al, 0b8h
    mov  ch, 3fh
    call UpdateExtendedRegisterBits     ;offset 0x1baa
    test byte ptr es:[bx + 08h], 40h    ;offset 0x08
    je   Label21da                      ;offset 0x21da
    mov  ax, 10beh
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    ret
Label21da:                              ;offset 0x21da
    call ClearGeneralPurposeFlagATI3E4  ;offset 0x1bcd
    ret
Func218e ENDP

ORG 21deh
Func21de PROC NEAR                      ;offset 0x21de
    mov  ax, 0fab6h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 100h
    mov  dx, 3c4h                       ;port 0x3c4
    out  dx, ax
    mov  ax, 0dfb0h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 3fb3h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 0f0beh
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 0e1b2h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 5b1h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 7fb5h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 300h
    mov  dx, 3c4h                       ;port 0x3c4
    out  dx, ax
    mov  ax, 0f4b8h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 77b4h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  ax, 7fb9h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    ret
Func21de ENDP

Func2229 PROC NEAR                      ;offset 0x2229
    mov  dl, byte ptr es:[bx + 07h]     ;offset 0x07
    test dl, 40h
    je   Label2238                      ;offset 0x2238
    mov  ax, 2beh
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label2238:                              ;offset 0x2238
    test dl, 80h
    je   Label2243                      ;offset 0x2243
    mov  ax, 40b1h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label2243:                              ;offset 0x2243
    test byte ptr es:[bx + 37h], 10h    ;offset 0x37
    je   Label2256                      ;offset 0x2256
    mov  ax, 20b0h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  ax, 4b6h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label2256:                              ;offset 0x2256
    mov  dl, byte ptr es:[bx + 37h]     ;offset 0x37
    or   si, si
    je   Label2262                      ;offset 0x2262
    mov  dl, byte ptr es:[si + 14h]     ;offset 0x14
Label2262:                              ;offset 0x2262
    test dl, 40h
    je   Label226d                      ;offset 0x226d
    mov  ax, 2b8h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
Label226d:                              ;offset 0x226d
    test dl, 10h
    je   Label228a                      ;offset 0x228a
    mov  ax, 20b0h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  ax, 4b6h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  dx, 3c4h                       ;port 0x3c4
    mov  al, 04h
    out  dx, al
    inc  dx
    in   al, dx
    or   al, 08h
    out  dx, al
    ret
Label228a:                              ;offset 0x228a
    test dl, 20h
    je   Label229c                      ;offset 0x229c
    mov  ax, 8beh
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  ax, 1b6h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    ret
Label229c:                              ;offset 0x229c
    ret
Func2229 ENDP

Func229d PROC NEAR                      ;offset 0x229d
    mov  ax, 80b9h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  ax, 80b6h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    mov  ax, 7fb3h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  ax, 0ch
    out  dx, ax
    mov  al, 0dh
    out  dx, ax
    mov  al, 08h
    out  dx, ax
    cmp  byte ptr ds:[449h], 03h        ;offset 0x449
    jle  Label22d2                      ;offset 0x22d2
    cmp  byte ptr ds:[449h], 07h        ;offset 0x449
    jne  Label22d1                      ;offset 0x22d1
    mov  ax, 0c7b1h
    call DisableExtendedRegisterBits    ;offset 0x1b83
Label22d1:                              ;offset 0x22d1
    ret
Label22d2:                              ;offset 0x22d2
    mov  ah, byte ptr es:[bx + 13h]     ;offset 0x13
    mov  dx, 3d4h                       ;port 0x3d4
    mov  al, 09h
    out  dx, ax
    mov  ax, 67b1h
    call DisableExtendedRegisterBits    ;offset 0x1b83
    cmp  byte ptr ds:[485h], 10h        ;offset 0x485
    je   Label22f2                      ;offset 0x22f2
    mov  al, 0b1h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 01h
    jne  Label22f9                      ;offset 0x22f9
Label22f2:                              ;offset 0x22f2
    mov  ax, 8b1h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    ret
Label22f9:                              ;offset 0x22f9
    mov  ax, 80b1h
    call EnableExtendedRegisterBits     ;offset 0x1b6e
    ret
Func229d ENDP

ORG 2300h
Func2300 PROC NEAR                      ;offset 0x2300
    ret ;single instruction
Func2300 ENDP

ORG 2301h
Func2301 PROC NEAR                      ;offset 0x2301
    push bx
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    add  dl, 05h                        ;port 0x3?9
    mov  bl, byte ptr ds:[449h]         ;offset 0x449
    and  bl, 1fh
    mov  al, 30h
    cmp  bl, 07h
    jg   Label2341                      ;offset 0x2341
    je   Label2321                      ;offset 0x2321
    cmp  bl, 06h
    jne  Label2320                      ;offset 0x2320
    mov  al, 3fh
Label2320:                              ;offset 0x2320
    out  dx, al
Label2321:                              ;offset 0x2321
    mov  byte ptr ds:[466h], al         ;offset 0x466
    xor  bh, bh
    cli
    mov  al, byte ptr cs:[bx + Data2343];offset 0x2343
    mov  byte ptr ds:[465h], al         ;offset 0x465
    and  dl, 0f8h                       ;port 0x3f8
    out  dx, al
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    add  dl, 06h                        ;port 0x3?a
    in   al, dx
    mov  dl, 0c0h                       ;port 0x3c0
    mov  al, 20h
    out  dx, al
    sti
Label2341:                              ;offset 0x2341
    pop  bx
    ret
Func2301 ENDP

Data2343                DB 02Ch, 028h, 02Dh, 029h, 02Ah, 02Eh, 01Eh, 029h

InterruptHandlerEntrypoint:             ;offset 0x234b
    sti
    cld
    push  ds
    push  es
    push  di
    push  si
    push  bp
    push  dx
    push  cx
    push  bx
    push  ax
    push  ax
    mov   al, ah
    xor   ah, ah
    shl   ax, 01h
    cmp   ax, 20h
    jb    Label2367                     ;offset 0x2367
    cmp   ax, 26h
    jne   Label236f                     ;offset 0x236f
Label2367:                              ;offset 0x2367
    mov   si, ax
    pop   ax
    jmp   word ptr cs:[si + offset IntHandlerJumpTable];offset 0x23ba
Label236f:                              ;offset 0x236f
    cmp   ax, 24h
    jne   Label237f                     ;offset 0x237f
    cmp   bl, 32h
    je    Label2367                     ;offset 0x2367
    cmp   bx, 5506h
    je    Label2367                     ;offset 0x2367
Label237f:                              ;offset 0x237f
    mov   si, 3ah
    push  ax
    mov   al, 0b4h
    call  ReadExtendedRegisterByte      ;offset 0x1b9f
    test  al, 03h
    je    Label2391                     ;offset 0x2391
    mov   si, 20h
    jmp   Label239d                     ;offset 0x239d
Label2391:                              ;offset 0x2391
    mov   al, 0b1h
    call  ReadExtendedRegisterByte      ;offset 0x1b9f
    test  al, 02h
    je    Label239d                     ;offset 0x239d
    mov   si, 28h
Label239d:                              ;offset 0x239d
    pop   ax
    cmp   ax, si
    jl    Label2367                     ;offset 0x2367
    pop   ax
Label23a3:                              ;offset 0x23a3
    pop   ax
    pop   bx
    pop   cx
    pop   dx
    pop   bp
    pop   si
    pop   di
    pop   es
    pop   ds
    int   42h
    iret
Label23af:                              ;offset 0x23af
    pop   ax
    pop   bx
    pop   cx
    pop   dx
    pop   bp
    pop   si
    pop   di
    pop   es
    pop   ds
    iret

ORG 23bah
IntHandlerJumpTable     DW offset SetVideoMode;offset 0x2432
                        DW offset SetTextModeCursorShape;offset 0x2ccc
                        DW offset SetCursorPosition;offset 0x2ce3
                        DW offset GetCursorPositionAndSize;offset 0x2d00
                        DW offset Function4Forward;offset 0x2586
                        DW offset SelectActiveDisplayPage;offset 0x258d
                        DW offset ScrollUpWindow;offset 0x25ba
                        DW offset ScrollDownWindow;offset 0x262d
                        DW offset ReadCharAndAttrAtCursorPos;offset 0x26ba
                        DW offset WriteCharAndAttrAtCursorPos;offset 0x2833
                        DW offset WriteCharOnlyAtCursorPos;offset 0x2837
                        DW offset ColorFunctions;offset 0x350c
                        DW offset WriteGraphicsPixel;offset 0x2a57
                        DW offset ReadGraphicsPixel;offset 0x2ae1
                        DW offset TeletypeOutput;offset 0x2b48
                        DW offset GetCurrentVideoMode;offset 0x2d1c
                        DW offset DACFunctions;offset 0x607c
                        DW offset TextModeCharFunctions;offset 0x2da4
                        DW offset AlternateFunctions;offset 0x31df
                        DW offset WriteString;offset 0x344b
                        DW offset Label23af;offset 0x23af
                        DW offset Label23af;offset 0x23af
                        DW offset Label23af;offset 0x23af
                        DW offset Label23af;offset 0x23af
                        DW offset Label23af;offset 0x23af
                        DW offset Label23af;offset 0x23af
                        DW offset DisplayCombinationCodeFuncs;offset 0x62c3
                        DW offset FunctionalityAndStateInfo;offset 0x63dc
                        DW offset SaveRestoreVideoState;offset 0x6535


ORG 2432h
SetVideoMode:                           ;offset 0x2432
    call GetSystemSegment               ;offset 0x16e8
    and  byte ptr ds:[487h], 0f3h       ;offset 0x487
    mov  word ptr ds:[485h], 08h        ;offset 0x485
    mov  byte ptr ds:[484h], 18h        ;offset 0x484
    mov  word ptr ds:[10ch], offset Func3818;offset 0x10c offset 0x3818
    mov  word ptr ds:[10eh], cs         ;offset 0x10e
    mov  ah, al
    mov  al, 0b4h                       ;Extended Register 0x34
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 02h                        ;bit 1 - 1 = Hercules emulation on, 0 = Hercules emulation off
    je   Label2470                      ;offset 0x2470
    and  ah, 80h
    mov  al, byte ptr ds:[410h]         ;offset 0x410
    and  al, 30h
    cmp  al, 30h
    je   Label246d                      ;offset 0x246d
    mov  al, ah
    xor  ah, ah
    jmp  Label23a3                      ;offset 0x23a3
Label246d:                              ;offset 0x246d
    or   ah, 07h
Label2470:                              ;offset 0x2470
    mov  al, 0b1h                       ;Extended Register 0x31
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 03h                        ;bit 1-0 - 11 = EGA Compatibility On, 00 = EGA compatibility off
    je   Label2498                      ;offset 0x2498
    mov  al, byte ptr ds:[410h]         ;offset 0x410
    and  al, 30h
    cmp  al, 30h
    jne  Label2498                      ;offset 0x2498
    mov  al, ah
    and  al, 7fh
    cmp  al, 0fh
    je   Label2498                      ;offset 0x2498
    cmp  al, 07h
    je   Label2498                      ;offset 0x2498
    cmp  al, 04h
    jge  Label2498                      ;offset 0x2498
    and  ah, 80h
    or   ah, 07h
Label2498:                              ;offset 0x2498
    mov  al, ah
    push ax
    and  al, 7fh
    mov  ah, al
    call Func148a                       ;offset 0x148a
    pop  ax
    mov  ah, al
    and  al, 7fh
    cmp  al, 07h
    jle  Label24b2                      ;offset 0x24b2
    cmp  dx, bx
    jne  Label24b2                      ;offset 0x24b2
    jmp  Label2583                      ;offset 0x2583
Label24b2:                              ;offset 0x24b2
    push ax
    mov  dx, 3cch                       ;port 0x3cc
    in   al, dx
    mov  dl, 0b4h                       ;port 0x3b4
    mov  cl, 30h
    test byte ptr es:[bx + 09h], 01h
    je   Label24ce                      ;offset 0x24ce
    mov  dl, 0d4h                       ;port 0x3d4
    mov  cl, 20h
    test al, 01h
    jne  Label24fc                      ;offset 0x24fc
    mov  ch, 09h
    jmp  Label24d4                      ;offset 0x24d4
Label24ce:                              ;offset 0x24ce
    test al, 01h
    je   Label24fc                      ;offset 0x24fc
    mov  ch, 0bh
Label24d4:                              ;offset 0x24d4
    call Func62e4                       ;offset 0x62e4
    or   bh, bh
    jne  Label24e6                      ;offset 0x24e6
    and  byte ptr ds:[488h], 0f0h       ;offset 0x488
    or   byte ptr ds:[488h], ch         ;offset 0x488
    jmp  Label24fc                      ;offset 0x24fc
Label24e6:                              ;offset 0x24e6
    pop  ax
    or   byte ptr ds:[487h], 08h        ;offset 0x487
    and  byte ptr ds:[410h], 0cfh       ;offset 0x410
    or   byte ptr ds:[410h], cl         ;offset 0x410
    mov  word ptr ds:[463h], dx         ;offset 0x463
    jmp  Label23a3                      ;offset 0x23a3
Label24fc:                              ;offset 0x24fc
    and  byte ptr ds:[410h], 0cfh       ;offset 0x410
    or   byte ptr ds:[410h], cl         ;offset 0x410
    pop  ax
    and  ah, 80h
    and  byte ptr ds:[487h], 7fh        ;offset 0x487
    or   byte ptr ds:[487h], ah         ;offset 0x487
    mov  ah, al
    mov  al, 0b4h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 03h
    je   Label253f                      ;offset 0x253f
    cmp  ah, 07h
    jg   Label253c                      ;offset 0x253c
    mov  byte ptr ds:[449h], ah         ;offset 0x449
    call Func1486                       ;offset 0x1486
    call Func6c7a                       ;offset 0x6c7a
    mov  cx, word ptr ds:[460h]         ;offset 0x460
    call Func1a9e                       ;offset 0x1a9e
    call Func2301                       ;offset 0x2301
    call Func229d                       ;offset 0x229d
    call ClearTextScreen                ;offset 0x1dcc
Label253c:                              ;offset 0x253c
    jmp  Label23af                      ;offset 0x23af
Label253f:                              ;offset 0x253f
    mov  byte ptr ds:[449h], ah         ;offset 0x449
    call Func1486                       ;offset 0x1486
    call MakeAttributePaletteRegistersWritable;offset 0x1865
    call Func6c7a                       ;offset 0x6c7a
    call Func1f59                       ;offset 0x1f59
    call Func3613                       ;offset 0x3613
    call Func2f3c                       ;offset 0x2f3c
    call ClearScreen                    ;offset 0x1df7
    call Func2301                       ;offset 0x2301
    push es
    push bx
    call Func30ac                       ;offset 0x30ac
    pop  bx
    pop  es
    call Func6c5c                       ;offset 0x6c5c
    mov  al, 0b1h
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 02h
    je   Label2577                      ;offset 0x2577
    mov  cx, word ptr ds:[460h]         ;offset 0x460
    call Func1a9e                       ;offset 0x1a9e
    jmp  Label2577                      ;offset 0x2577
Label2577:                              ;offset 0x2577
    mov  cx, 01h
    call WaitForVSYNC                   ;offset 0x6c2b
    call EnableAttributePaletteRegisters;offset 0x185c
    call ReadInputStatus1               ;offset 0x1857
Label2583:                              ;offset 0x2583
    jmp  Label23af                      ;offset 0x23af

Function4Forward:                       ;offset 0x2586
    ret

ORG 258dh
SelectActiveDisplayPage:                ;offset 0x258d
    ret

ORG 25bah
ScrollUpWindow:                         ;offset 0x25ba
    ret

ORG 262dh
ScrollDownWindow:                       ;offset 0x262d
    ret

ORG 26bah
ReadCharAndAttrAtCursorPos:             ;offset 0x26ba
    ret

ORG 2833h
WriteCharAndAttrAtCursorPos:            ;offset 0x2833
    ret

ORG 2837h
WriteCharOnlyAtCursorPos:               ;offset 0x2837
    ret

ORG 2ccch
SetTextModeCursorShape:                 ;offset 0x2ccc
    ret

ORG 2ce3h
SetCursorPosition:                      ;offset 0x2ce3
    ret

ORG 2d00h
GetCursorPositionAndSize:               ;offset 0x2d00
    ret

ORG 2a57h
WriteGraphicsPixel:                     ;offset 0x2a57
    ret

ORG 2ae1h
ReadGraphicsPixel:                      ;offset 0x2ae1
    ret

ORG 2b48h
TeletypeOutput:                         ;offset 0x2b48
    ret

ORG 2d1ch
GetCurrentVideoMode:                    ;offset 0x2d1c
    ret

ORG 2da4h
TextModeCharFunctions:                  ;offset 0x2da4
    ret

ORG 2f3ch
Func2f3c PROC NEAR                      ;offset 0x2f3c
    push es
    push bx
    mov  ah, byte ptr ds:[449h]         ;offset 0x449
    call Func3008                       ;offset 0x3008
    pop  bx
    pop  es
    ret
Func2f3c ENDP

Func2f48 PROC NEAR                      ;offset 0x2f48
    mov       di, ax
    mov       si, dx
    mov       dx, 3ceh                  ;port 0x3ce
    mov       ax, 05h
    out       dx, ax
    mov       ax, 406h
    out       dx, ax
    mov       dl, 0c4h                  ;port 0x3c4
    mov       ax, 402h
    out       dx, ax
    mov       ax, 704h
    out       dx, ax
    mov       dx, si
    mov       ax, di
    push      ax
    and       al, 7fh
    or        al, al
    je        Label2f8e                 ;offset 0x2f8e
    push      cs
    pop       es
    xor       dx, dx
    mov       cx, 100h
    cmp       al, 04h
    jne       Label2f7e                 ;offset 0x2f7e
    mov       bh, 10h
    mov       bp, 4eb0h                 ;offset 0x4eb0
    jmp       Label2f8e                 ;offset 0x2f8e
Label2f7e:                              ;offset 0x2f7e
    dec       al
    jne       Label2f89                 ;offset 0x2f89
    mov       bh, 0eh
    mov       bp, 4018h                 ;offset 0x4018
    jmp       Label2f8e                 ;offset 0x2f8e
Label2f89:                              ;offset 0x2f89
    mov       bh, 08h
    mov       bp, 3818h                 ;offset 0x3818
Label2f8e:                              ;offset 0x2f8e
    call      Func2fbc                  ;offset 0x2fbc
    pop       ax
    test      al, 80h
    je        Label2f99                 ;offset 0x2f99
    call      Func314a                  ;offset 0x314a
Label2f99:                              ;offset 0x2f99
    mov       dx, 3c4h                  ;port 0x3c4
    mov       ax, 302h
    out       dx, ax
    mov       ax, 204h
    out       dx, ax
    mov       dl, 0cch                  ;port 0x3cc
    in        al, dx
    test      al, 01h
    mov       ax, 0e06h
    jne       Label2fb0                 ;offset 0x2fb0
    mov       ah, 0ah
Label2fb0:                              ;offset 0x2fb0
    mov       dl, 0ceh                  ;port 0x3ce
    out       dx, ax
    mov       ax, 04h
    out       dx, ax
    mov       ax, 1005h
    out       dx, ax
    ret
Func2f48 ENDP

Func2fbc PROC NEAR                      ;offset 0x2fbc
    cld
    or        cx, cx
    jne       Label2fc2 ;offset 0x2fc2
    ret
Label2fc2:                              ;offset 0x2fc2
    push      ds
    push      es
    pop       ds
    mov       ax, 0a000h                ;segment 0xa000
    mov       es, ax
    push      cx
    mov       cl, 05h
    mov       di, dx
    shl       di, cl
    pop       cx
    mov       al, 40h
    push      bx
    and       bl, 03h
    mul       bl
    pop       bx
    test      bl, 04h
    je        Label2fe2                 ;offset 0x2fe2
    add       al, 20h
Label2fe2:                              ;offset 0x2fe2
    xchg      al, ah
    mov       dx, ax
    add       di, dx
    mov       si, bp
    jcxz      Label3006                 ;offset 0x3006
    xor       al, al
    xchg      bl, bh
    xor       bh, bh
Label2ff2:                              ;offset 0x2ff2
    push      cx
    mov       cx, bx
    rep movsb
    cmp       bx, 20h
    je        Label3003                 ;offset 0x3003
    mov       cx, 20h
    sub       cx, bx
    rep stosb
Label3003:                              ;offset 0x3003
    pop       cx
    loop      Label2ff2                 ;offset 0x2ff2
Label3006:                              ;offset 0x3006
    pop       ds
    ret
Func2fbc ENDP

Func3008 PROC NEAR                      ;offset 0x3008
    call      Func148a                  ;offset 0x148a
    mov       al, byte ptr es:[bx + 02h];offset 0x02
    mov       ah, byte ptr es:[bx + 33h];offset 0x33
    test      ah, 01h
    je        Label3035                 ;offset 0x3035
    mov       bp, 4eb0h                 ;offset 0x4eb0
    cmp       al, 10h
    jge       Label3029                 ;offset 0x3029
    mov       bp, 4018h                 ;offset 0x4018
    cmp       al, 0eh
    jge       Label3029                 ;offset 0x3029
    mov       bp, 3818h                 ;offset 0x3818
Label3029:                              ;offset 0x3029
    mov       word ptr ds:[10ch], bp    ;offset 0x10c
    mov       word ptr ds:[10eh], cs    ;offset 0x10e
    call      Func3063                  ;offset 0x3063
    ret
Label3035:                              ;offset 0x3035
    mov       ah, 84h
    cmp       al, 10h
    jge       Label304e                 ;offset 0x304e
    mov       ah, 81h
    cmp       al, 0eh
    je        Label304e                 ;offset 0x304e
    jg        Label3058                 ;offset 0x3058
    mov       ah, 02h
    cmp       al, 08h
    je        Label304e                 ;offset 0x304e
    or        ah, 80h
    jmp       Label3058                 ;offset 0x3058
Label304e:                              ;offset 0x304e
    test      byte ptr es:[bx + 05h], 01h;offset 0x05
    je        Label3058                 ;offset 0x3058
    and       ah, 7fh
Label3058:                              ;offset 0x3058
    xchg      ah, al
    xor       bl, bl
    call      Func2f48                  ;offset 0x2f48
    call      Func317c                  ;offset 0x317c
    ret
Func3008 ENDP

Func3063 PROC NEAR                      ;offset 0x3063
    mov  ah, byte ptr ds:[449h]         ;offset 0x449
    push ax
    push es
    push di
    les  di, ds:[4a8h]                  ;offset 0x4a8
    les  di, es:[di + 0ch]              ;offset 0xc
    mov  bp, es
    or   bp, di
    je   Label30a8                      ;offset 0x30a8
    mov  bp, di
    add  di, 07h
Label307d:                              ;offset 0x307d
    mov  al, byte ptr es:[di]
    cmp  al, 0ffh
    je   Label30a8                      ;offset 0x30a8
    cmp  al, ah
    je   Label308b                      ;offset 0x308b
    inc  di
    jmp  Label307d                      ;offset 0x307d
Label308b:                              ;offset 0x308b
    mov  di, bp
    mov  al, byte ptr es:[di]
    dec  al
    mov  byte ptr ds:[484h], al         ;offset 0x484
    mov  ax, word ptr es:[di + 01h]     ;offset 0x01
    mov  word ptr ds:[485h], ax         ;offset 0x485
    les  di, es:[di + 03h]              ;offset 0x03
    mov  word ptr ds:[10ch], di         ;offset 0x10c
    mov  word ptr ds:[10eh], es         ;offset 0x10e
Label30a8:                              ;offset 0x30a8
    pop  di
    pop  es
    pop  ax
    ret
Func3063 ENDP

Func30ac PROC NEAR                      ;offset 0x30ac
    test byte ptr es:[bx + 33h], 01h    ;offset 0x33
    je   Label30b4                      ;offset 0x30b4
    ret
Label30b4:                              ;offset 0x30b4
    les  bx, ds:[4a8h]                  ;offset 0x4a8
    les  bx, es:[bx + 08h]              ;offset 0x08
    mov  ax, es
    or   ax, bx
    je   Label30e1                      ;offset 0x30e1
    mov  di, 0bh
Label30c5:                              ;offset 0x30c5
    mov  al, byte ptr es:[bx + di]
    cmp  al, 0ffh
    je   Label30e1                      ;offset 0x30e1
    inc  di
    cmp  byte ptr ds:[449h], al         ;offset 0x449
    jne  Label30c5                      ;offset 0x30c5
    mov  al, byte ptr es:[bx]
    xor  ah, ah
    xchg word ptr ds:[485h], ax         ;offset 0x485
    mov  bx, ax
    call Func30e2                       ;offset 0x30e2
Label30e1:                              ;offset 0x30e1
    ret
Func30ac ENDP

Func30e2 PROC NEAR                      ;offset 0x30e2
    mov       dx, word ptr [463h]       ;offset 0x463
    mov       ah, byte ptr [485h]       ;offset 0x485
    dec       ah
    and       ah, 1fh
    cli       
    cmp       dl, 0d4h                  ;port 0x3d4
    je        Label3100                 ;offset 0x3100
    mov       al, 14h
    out       dx, al
    inc       dx
    in        al, dx
    and       al, 0e0h
    or        al, ah
    out       dx, al
    dec       dx
Label3100:                              ;offset 0x3100
    mov       al, 09h
    out       dx, al
    inc       dx
    in        al, dx
    and       al, 0e0h
    or        al, ah
    out       dx, al
    sti       
    mov       cl, al
    mov       ax, bx
    mov       dl, byte ptr ds:[485h]    ;offset 0x485
    div       dl
    cmp       byte ptr ds:[484h], 0ffh  ;offset 0x484
    jne       Label3123                 ;offset 0x3123
    mov       byte ptr ds:[484h], al    ;offset 0x484
    dec       byte ptr ds:[484h]        ;offset 0x484
Label3123:                              ;offset 0x3123
    mul       dl
    test      cl, 80h
    je        Label312c                 ;offset 0x312c
    shl       ax, 01h
Label312c:                              ;offset 0x312c
    dec       ax
    mov       ah, al
    mov       al, 12h
    mov       dx, word ptr ds:[463h]    ;offset 0x463
    out       dx, ax
    mov       al, byte ptr ds:[44ah]    ;offset 0x44a
    mov       ah, byte ptr ds:[484h]    ;offset 0x484
    inc       ah
    mul       ah
    shl       ax, 01h
    add       ax, 100h
    mov       word ptr ds:[44ch], ax    ;offset 0x44c
    ret
Func30e2 ENDP

Func314a PROC NEAR                      ;offset 0x314a
    push      ds
    xor       bh, bh
    mov       bl, ah
    mov       si, 5eb0h                 ;offset 0x5eb0
    cmp       bl, 10h
    je        Label3161                 ;offset 0x3161
    mov       si, 4e18h                 ;offset 0x4e18
    cmp       bl, 0eh
    je        Label3161                 ;offset 0x3161
    pop       ds
    ret
Label3161:                              ;offset 0x3161
    push      cs
    pop       ds
Label3163:                              ;offset 0x3163
    mov       di, word ptr ds:[si]
    or        di, di
    je        Label317a                 ;offset 0x317a
    and       di, 0ffh
    inc       si
    mov       cl, 05h
    shl       di, cl
    add       di, dx
    mov       cx, bx
    rep movsb
    jmp       Label3163                 ;offset 0x3163
Label317a:                              ;offset 0x317a
    pop       ds
    ret
Func314a ENDP

Func317c PROC NEAR                      ;offset 0x317c
    mov  ah, byte ptr ds:[449h]         ;offset 0x449
    push ax
    push es
    push di
    les  di, ptr ds:[4a8h]              ;offset 0x4a8
    les  di, ptr es:[di + 08h]          ;offset 0x08
    mov  bp, es
    or   bp, di
    je   Label31db                      ;offset 0x31db
    mov  bp, di
    add  di, 0bh
Label3196:                              ;offset 0x3196
    mov  al, byte ptr es:[di]
    cmp  al, 0ffh
    je   Label31db                      ;offset 0x31db
    cmp  al, ah
    je   Label31a4                      ;offset 0x31a4
    inc  di
    jmp  Label3196                      ;offset 0x3196
Label31a4:                              ;offset 0x31a4
    mov  di, bp
    mov  bh, byte ptr es:[di]
    mov  bl, byte ptr es:[di + 01h]     ;offset 0x01
    mov  cx, word ptr es:[di + 02h]     ;offset 0x02
    mov  dx, word ptr es:[di + 04h]     ;offset 0x04
    mov  al, byte ptr es:[di + 0ah]     ;offset 0x0a
    cmp  al, 0ffh
    je   Label31bf                      ;offset 0x31bf
    dec  al
Label31bf:                              ;offset 0x31bf
    push ax
    mov  al, byte ptr ds:[484h]         ;offset 0x484
    inc  al
    mul  byte ptr ds:[485h]             ;offset 0x485
    mov  word ptr ds:[485h], ax         ;offset 0x485
    pop  ax
    mov  byte ptr ds:[484h], al         ;offset 0x484
    les  di, ptr es:[di + 06h]          ;offset 0x06
    mov  bp, di
    xor  al, al
    call Func2f48                       ;offset 0x2f48
Label31db:                              ;offset 0x31db
    pop  di
    pop  es
    pop  ax
    ret
Func317c ENDP

ORG 31dfh
AlternateFunctions:                     ;offset 0x31df
    ret

ORG 344bh
WriteString:                            ;offset 0x344b
    ret

ORG 350ch
ColorFunctions:                         ;offset 0x350c
    ret

ORG 3613h
Func3613 PROC NEAR                      ;offset 0x3613
    push bx
    push cx
    mov  cx, 0dh
    cmp  byte ptr ds:[449h], 13h        ;offset 0x449
    ja   Label3639                      ;offset 0x3639
    cmp  byte ptr ds:[449h], 12h        ;offset 0x449
    jne  Label3661                      ;offset 0x3661
    mov  al, 0bah
    call ReadExtendedRegisterByte       ;offset 0x1b9f
    test al, 01h
    je   Label3661                      ;offset 0x3661
    jmp  Label3639                      ;offset 0x3639
Label3631:                              ;offset 0x3631
    add  cl, 0fh
    cmp  cl, 76h
    jge  Label3661                      ;offset 0x3661
Label3639:                              ;offset 0x3639
    mov  bl, 01h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    cmp  byte ptr ds:[449h], bh         ;offset 0x449
    je   Label3664                      ;offset 0x3664
    cmp  byte ptr ds:[449h], bl         ;offset 0x449
    je   Label3664                      ;offset 0x3664
    mov  bl, 02h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    cmp  byte ptr ds:[449h], bh         ;offset 0x449
    je   Label3664                      ;offset 0x3664
    cmp  byte ptr ds:[449h], bl         ;offset 0x449
    je   Label3664                      ;offset 0x3664
    jmp  Label3631                      ;offset 0x3631
Label3661:                              ;offset 0x3661
    pop  cx
    pop  bx
    ret
Label3664:                              ;offset 0x3664
    mov  bl, 00h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    test bh, 01h
    jne  Label3631                      ;offset 0x3631
    test bl, 40h
    je   Label36d3                      ;offset 0x36d3
    mov  al, 11h
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    call Func16de                       ;offset 0x16de
    and  al, 7fh
    mov  ah, 11h
    xchg al, ah
    out  dx, ax
    mov  bl, 3
    add  bl, cl
    call Func375f                       ;offset 0x375f
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, 00h
    out  dx, ax
    mov  al, 06h
    mov  ah, bl
    out  dx, ax
    mov  bl, 04h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, 04h
    out  dx, ax
    inc  al
    mov  ah, bl
    out  dx, ax
    mov  bl, 08h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, 07h
    out  dx, ax
    mov  bl, 05h
    add  bl, cl
    call Func375f                       ;offset 0x375f
    mov  dx, word ptr ds:[463h]         ;offset 0x463
    mov  al, 10h
    out  dx, ax
    inc  al
    mov  ah, bl
    out  dx, ax
    mov  bl, 00h
    add  bl, cl
    call Func375f                       ;offset 0x375f
Label36d3:                              ;offset 0x36d3
    mov  dx, 3cch                       ;port 0x3cc
    in   al, dx
    and  al, 3fh
    and  bh, 0c0h
    or   al, bh
    mov  dl, 0c2h                       ;port 0x3c2
    out  dx, al
    jmp  Label3661                      ;offset 0x3661
    or   ah, 02h
    out  dx, ax
    call SpeedSensitiveDelay            ;offset 0x36f0
    and  ah, 0fdh
    out  dx, ax
    sti
Func3613 ENDP
;continue!
SpeedSensitiveDelay PROC NEAR           ;offset 0x36f0
    push cx
    cld  
    mov  cx, 10h
Label36f5:                              ;offset 0x36f5
    jmp  Label36f7                      ;offset 0x36f7
Label36f7:                              ;offset 0x36f7
    loop Label36f5                      ;offset 0x36f5
    pop  cx
    ret
SpeedSensitiveDelay ENDP

ORG 375fh
Func375f PROC NEAR                      ;offset 0x375f
    ret
Func375f ENDP

ORG 3818h
Func3818 PROC NEAR                      ;offset 0x3818
    ret
Func3818 ENDP

ORG 606ch
WaitForVSYNCBlank PROC NEAR             ;offset 0x606c
    push  ax
    push  cx
    call  ReadInputStatus1              ;offset 0x1857
    mov   cx, 7fffh
Label6074:                              ;offset 0x6074
    in    al, dx
    test  al, 08h
    loope Label6074                     ;offset 0x6074
    pop   cx
    pop   ax
    ret
WaitForVSYNCBlank ENDP

ORG 607ch
DACFunctions:                           ;offset 0x607c
    ret

ORG 62b3h
Func62b3 PROC NEAR                      ;offset 0x62b3
    ret
Func62b3 ENDP

ORG 62c3h
DisplayCombinationCodeFuncs:            ;offset 0x62c3
    ret

ORG 62e4h
Func62e4 PROC NEAR                      ;offset 0x62e4
    mov  al, byte ptr ds:[48ah]         ;offset 0x48a
    mov  di, 637ch                      ;offset 0x637c
    cmp  al, byte ptr cs:[di]
    jb   Label62f3                      ;offset 0x62f3
    mov  bx, 0ffffh
    ret
Label62f3:                              ;offset 0x62f3
    xor  ah, ah
    shl  ax, 01h
    add  di, ax
    mov  bx, word ptr cs:[di + 04h]
    or   bl, bl
    je   Label6306                      ;offset 0x6306
    or   bh, bh
    jne  Label6309                      ;offset 0x6309
    ret
Label6306:                              ;offset 0x6306
    xchg bl, bh
    ret
Label6309:                              ;offset 0x6309
    mov  al, byte ptr [410h]            ;offset 0x410
    and  al, 30h
    cmp  al, 30h
    je   Label6318                      ;offset 0x6318
    test bl, 01h
    jne  Label6306                      ;offset 0x6306
    ret
Label6318:                              ;offset 0x6318
    test bl, 01h
    je   Label6306                      ;offset 0x6306
    ret  
Func62e4 ENDP

ORG 63dch
FunctionalityAndStateInfo:              ;offset 0x63dc
    ret

ORG 6535h
SaveRestoreVideoState:                  ;offset 0x6535
    ret

ORG 6c2bh
WaitForVSYNC PROC NEAR                  ;offset 0x6c2b
    push   ax
    call   ReadInputStatus1             ;offset 0x1857
Label6c2f:                              ;offset 0x6c2f
    push   cx
    mov    cx, 7fffh
Label6c33:                              ;offset 0x6c33
    in     al, dx
    test   al, 08h
    loopne Label6c33                    ;offset 0x6c33
    mov    cx, 7fffh
Label6c3b:                              ;offset 0x6c3b
    in     al, dx
    test   al, 08h
    loope  Label6c3b                    ;offset 0x6c3b
    pop    cx
    loop   Label6c2f                    ;offset 0x6c2f
    pop    ax
    ret
WaitForVSYNC ENDP

EnableVideoDisplay PROC NEAR            ;offset 0x6c45
    call   WaitForVSYNCBlank            ;offset 0x606c
    mov    dl, 0c0h                     ;port 0x3c0
    xchg   ah, al
    out    dx, al
    jmp    Label6c4f                    ;offset 0x6c4f
Label6c4f:                              ;offset 0x6c4f
    jmp    Label6c51                    ;offset 0x6c51
Label6c51:                              ;offset 0x6c51
    xchg   ah, al
    out    dx, al
    mov    al, 20h
    jmp    Label6c58                    ;offset 0x6c58
Label6c58:                              ;offset 0x6c58
    jmp    Label6c5a                    ;offset 0x6c5a
Label6c5a:                              ;offset 0x6c5a
    out    dx, al
    ret
EnableVideoDisplay ENDP

ORG 6c5ch
Func6c5c PROC NEAR                      ;offset 0x6c5c
    cmp  byte ptr ds:[449h], 07h        ;offset 0x449
    jle  Label6c79                      ;offset 0x6c79
    test byte ptr es:[bx + 33h], 01h    ;offset 0x33
    jne  Label6c79                      ;offset 0x6c79
    mov  ah, 03h
    test byte ptr es:[bx + 09h], 01h    ;offset 0x09
    jne  Label6c75                      ;offset 0x6c75
    mov  ah, 07h
Label6c75:                              ;offset 0x6c75
    mov  byte ptr ds:[449h], ah         ;offset 0x449
Label6c79:                              ;offset 0x6c79
    ret
Func6c5c ENDP

ORG 6c7ah
Func6c7a PROC NEAR                      ;offset 0x6c7a
    or        byte ptr ds:[487h], 02h   ;offset 0x487
    mov       dx, 3b4h                  ;port 0x3b4
    test      byte ptr es:[bx + 09h], 01h
    je        Label6c90                 ;offset 0x6c90
    mov       dl, 0d4h                  ;port 0x3d4
    and       byte ptr ds:[487h], 0fdh  ;offset 0x487
Label6c90:                              ;offset 0x6c90
    mov       word ptr ds:[463h], dx    ;offset 0x463
    mov       word ptr ds:[44eh], 00h   ;offset 0x44e
    mov       byte ptr ds:[462h], 00h   ;offset 0x462
    mov       cx, 08h
    mov       di, 450h                  ;offset 0x450
    push      es
    push      ds
    pop       es
    xor       ax, ax
    rep stosw
    pop       es
    mov       al, byte ptr es:[bx]
    xor       ah, ah
    mov       word ptr ds:[44ah], ax    ;offset 0x44a
    mov       al, byte ptr es:[bx + 01h]
    mov       byte ptr ds:[484h], al    ;offset 0x484
    mov       al, byte ptr es:[bx + 02h]
    mov       word ptr ds:[485h], ax    ;offset 0x485
    mov       ax, word ptr es:[bx + 03h]
    mov       word ptr ds:[44ch], ax    ;offset 0x44c
    mov       ax, word ptr es:[bx + 14h]
    xchg      ah, al
    mov       word ptr ds:[460h], ax    ;offset 0x460
    ret
Func6c7a ENDP

ORG 6e8fh
Func6e8f PROC NEAR                      ;offset 0x6e8f
    ret
Func6e8f ENDP



ORG 8000h
ROMEnd:
END