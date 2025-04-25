;VGA Registers and data structures

;VGA general info: https://wiki.osdev.org/VGA_Hardware
;VGA general info: http://www.osdever.net/FreeVGA/vga/vga.htm
;VGA general info: https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/video/Second%20Sight%20VGA%20Registers.pdf
;Interrupt list: http://www.ctyme.com/intr/int.htm

VGA_CRTControllerIndexB                 EQU 03b4h;0x3b4
VGA_CRTControllerIndexB_lowbyte         EQU 0b4h;0xb4
VGA_CRTControllerDataB                  EQU 03b5h;0x3b5
VGA_CRTControllerDataB_lowbyte          EQU 0b5h;0xb5
VGA_MDAModeControl                      EQU 03b8h;0x3b8
VGA_MDAModeControl_lowbyte              EQU 0b8h;0xb8
VGA_InputStatus1B                       EQU 03bah;0x3ba
VGA_InputStatus1B_lowbyte               EQU 0bah;0xba
VGA_FeatureControlWB                    EQU 03bah;0x3ba
VGA_FeatureControlWB_lowbyte            EQU 0bah;0xba
VGA_AttributeControllerIndex            EQU 03c0h;0x3c0
VGA_AttributeControllerIndex_lowbyte    EQU 0c0h;0xc0
    VGA_ATTR_Palette0                   EQU 000h;0x00
    VGA_ATTR_Palette1                   EQU 001h;0x01
    VGA_ATTR_Palette2                   EQU 002h;0x02
    VGA_ATTR_Palette3                   EQU 003h;0x03
    VGA_ATTR_Palette4                   EQU 004h;0x04
    VGA_ATTR_Palette5                   EQU 005h;0x05
    VGA_ATTR_Palette6                   EQU 006h;0x06
    VGA_ATTR_Palette7                   EQU 007h;0x07
    VGA_ATTR_Palette8                   EQU 008h;0x08
    VGA_ATTR_Palette9                   EQU 009h;0x09
    VGA_ATTR_PaletteA                   EQU 00ah;0x0a
    VGA_ATTR_PaletteB                   EQU 00bh;0x0b
    VGA_ATTR_PaletteC                   EQU 00ch;0x0c
    VGA_ATTR_PaletteD                   EQU 00dh;0x0d
    VGA_ATTR_PaletteE                   EQU 00eh;0x0e
    VGA_ATTR_PaletteF                   EQU 00fh;0x0f
    VGA_ATTR_ModeControl                EQU 010h;0x10
    VGA_ATTR_OverscanColor              EQU 011h;0x11
    VGA_ATTR_ColorPlaneEnable           EQU 012h;0x12
    VGA_ATTR_HorizontalPelPanning       EQU 013h;0x13
    VGA_ATTR_ColorSelect                EQU 014h;0x14
    VGA_ATTR_PaletteAddressSource       EQU 020h;0x20
VGA_AttributeControllerDataW            EQU 03c0h;0x3c0
VGA_AttributeControllerDataW_lowbyte    EQU 0c0h;0xc0
VGA_AttributeControllerDataR            EQU 03c1h;0x3c1
VGA_AttributeControllerDataR_lowbyte    EQU 0c1h;0xc1
VGA_InputStatus0                        EQU 03c2h;0x3c2
VGA_InputStatus0_lowbyte                EQU 0c2h;0xc2
    VGA_IS_SwitchSense                  EQU 010h;0x10
    VGA_IS_FeatureInputs01              EQU 060h;0x60
    VGA_IS_VerticalRetraceInterrupt     EQU 080h;0x80
VGA_MiscellaneousWrite                  EQU 03c2h;0x3c2
VGA_MiscellaneousWrite_lowbyte          EQU 0c2h;0xc2
    VGA_Misc_IOAddressSel_MDA           EQU 000h;0x00
    VGA_Misc_IOAddressSel_CGA           EQU 001h;0x01
VGA_VideoSubsystemEnable                EQU 03c3h;0x3c3
VGA_VideoSubsystemEnable_lowbyte        EQU 0c3h;0xc3
VGA_SequenceIndex                       EQU 03c4h;0x3c4
VGA_SequenceIndex_lowbyte               EQU 0c4h;0xc4
    VGA_SEQIdx_Reset                    EQU 000h;0x00
        VGA_SEQ0_AsyncReset             EQU 001h;0x01
        VGA_SEQ0_SyncReset              EQU 002h;0x02
    VGA_SEQIdx_ClockingMode             EQU 001h;0x01
        VGA_SEQ1_9DotChar               EQU 000h;0x00
        VGA_SEQ1_8DotChar               EQU 001h;0x01
        VGA_SEQ1_ShiftLoadNormal        EQU 000h;0x00
        VGA_SEQ1_ShiftLoadHalf          EQU 004h;0x04
        VGA_SEQ1_DotClockVCLK           EQU 000h;0x00
        VGA_SEQ1_DotClockHalfVCLK       EQU 008h;0x08
        VGA_SEQ1_LoadCharClock          EQU 000h;0x00
        VGA_SEQ1_LoadFourthCharClock    EQU 010h;0x10
        VGA_SEQ1_ScreenOn               EQU 000h;0x00
        VGA_SEQ1_ScreenOff              EQU 020h;0x20
    VGA_SEQIdx_MapMask                  EQU 002h;0x02
        VGA_SEQ2_MaskNone               EQU 000h;0x00
        VGA_SEQ2_Mask0                  EQU 001h;0x01
        VGA_SEQ2_Mask1                  EQU 002h;0x02
        VGA_SEQ2_Mask2                  EQU 004h;0x04
        VGA_SEQ2_Mask3                  EQU 008h;0x08
        VGA_SEQ2_MaskAll                EQU 00fh;0x0f
    VGA_SEQIdx_CharacterMapSelect       EQU 003h;0x03
        VGA_SEQ3_MapBSel0               EQU 000h;0x00
        VGA_SEQ3_MapBSel1               EQU 001h;0x01
        VGA_SEQ3_MapBSel2               EQU 002h;0x02
        VGA_SEQ3_MapBSel3               EQU 003h;0x03
        VGA_SEQ3_MapBSel4               EQU 004h;0x04
        VGA_SEQ3_MapBSel5               EQU 005h;0x05
        VGA_SEQ3_MapBSel6               EQU 006h;0x06
        VGA_SEQ3_MapBSel7               EQU 007h;0x07
        VGA_SEQ3_MapASel0               EQU 000h;0x00
        VGA_SEQ3_MapASel1               EQU 008h;0x08
        VGA_SEQ3_MapASel2               EQU 010h;0x10
        VGA_SEQ3_MapASel3               EQU 018h;0x18
        VGA_SEQ3_MapASel4               EQU 020h;0x20
        VGA_SEQ3_MapASel5               EQU 028h;0x28
        VGA_SEQ3_MapASel6               EQU 030h;0x30
        VGA_SEQ3_MapASel7               EQU 038h;0x38
    VGA_SEQIdx_MemoryMode               EQU 004h;0x04
        VGA_SEQ4_Unknown1               EQU 001h;0x01
        VGA_SEQ4_MemSizeNot256k         EQU 000h;0x00
        VGA_SEQ4_MemSize256k            EQU 002h;0x02
        VGA_SEQ4_MapOddEven             EQU 000h;0x00
        VGA_SEQ4_CPUAnyMap              EQU 004h;0x04
        VGA_SEQ4_SequentialMemory       EQU 000h;0x00
        VGA_SEQ4_Chain4                 EQU 008h;0x08
VGA_SequenceData                        EQU 03c5h;0x3c5
VGA_SequenceData_lowbyte                EQU 0c5h;0xc5
VGA_DACMask                             EQU 03c6h;0x3c6
VGA_DACMask_lowbyte                     EQU 0c6h;0xc6
VGA_DACReadIndex                        EQU 03c7h;0x3c7
VGA_DACReadIndex_lowbyte                EQU 0c7h;0xc7
VGA_DACStatus                           EQU 03c7h;0x3c7
VGA_DACStatus_lowbyte                   EQU 0c7h;0xc7
VGA_DACWriteIndex                       EQU 03c8h;0x3c8
VGA_DACWriteIndex_lowbyte               EQU 0c8h;0xc8
VGA_RAMDACData                          EQU 03c9h;0x3c9
VGA_RAMDACData_lowbyte                  EQU 0c9h;0xc9
VGA_FeatureControlR                     EQU 03cah;0x3ca
VGA_FeatureControlR_lowbyte             EQU 0cah;0xca
VGA_MiscellaneousRead                   EQU 03cch;0x3cc
VGA_MiscellaneousRead_lowbyte           EQU 0cch;0xcc
    VGA_MISC_Clock25p175MHz             EQU 000h;0x00
    VGA_MISC_Clock28p322MHz             EQU 004h;0x04
    VGA_MISC_ClockSelectMask            EQU 00ch;0x0c
VGA_GraphicsControllerIndex             EQU 03ceh;0x3ce
VGA_GraphicsControllerIndex_lowbyte     EQU 0ceh;0xce
    VGA_GCTLIdx_SetReset                EQU 000h;0x00
    VGA_GCTLIdx_EnableSetReset          EQU 001h;0x01
    VGA_GCTLIdx_ColorCompare            EQU 002h;0x02
    VGA_GCTLIdx_DataRotate              EQU 003h;0x03
        VGA_GCTL3_Rotate0               EQU 000h;0x00
        VGA_GCTL3_Rotate1               EQU 001h;0x01
        VGA_GCTL3_Rotate2               EQU 002h;0x02
        VGA_GCTL3_Rotate3               EQU 003h;0x03
        VGA_GCTL3_Rotate4               EQU 004h;0x04
        VGA_GCTL3_Rotate5               EQU 005h;0x05
        VGA_GCTL3_Rotate6               EQU 006h;0x06
        VGA_GCTL3_Rotate7               EQU 007h;0x07
        VGA_GCTL3_FuncUnmodified        EQU 000h;0x00
        VGA_GCTL3_FuncSourcANDLatched   EQU 008h;0x08
        VGA_GCTL3_FuncSourceORLatched   EQU 010h;0x10
        VGA_GCTL3_FuncSourceXORLatched  EQU 018h;0x18
    VGA_GCTLIdx_ReadMapSelect           EQU 004h;0x04
        VGA_GCTL4_Map0                  EQU 000h;0x00
        VGA_GCTL4_Map1                  EQU 001h;0x01
        VGA_GCTL4_Map2                  EQU 002h;0x02
        VGA_GCTL4_Map3                  EQU 003h;0x03
    VGA_GCTLIdx_GraphicMode             EQU 005h;0x05
        VGA_GCTL5_BLU                   EQU 000h;0x00
        VGA_GCTL5_CPUDirect             EQU 001h;0x01
        VGA_GCTL5_SplatBLU              EQU 002h;0x02
        VGA_GCTL5_ReadMem               EQU 000h;0x00
        VGA_GCTL5_ReadColCmp            EQU 008h;0x08
        VGA_GCTL5_ReadMapSelect         EQU 000h;0x00
        VGA_GCTL5_OddEven               EQU 010h;0x10
        VGA_GCTL5_256ColorMode          EQU 040h;0x40
    VGA_GCTLIdx_Miscellaneous           EQU 006h;0x06
        VGA_GCTL6_AlphaMode             EQU 000h;0x00
        VGA_GCTL6_GfxMode               EQU 001h;0x01
        VGA_GCTL6_ChainOdd              EQU 000h;0x00
        VGA_GCTL6_ChainEven             EQU 002h;0x02
        VGA_GCTL6_Mem_A0000_BFFFF       EQU 000h;0x00
        VGA_GCTL6_Mem_A0000_AFFFF       EQU 004h;0x04
        VGA_GCTL6_Mem_B0000_B7FFF       EQU 008h;0x08
        VGA_GCTL6_Mem_B8000_BFFFF       EQU 00ch;0x0c
        VGA_GCTL6_Mem_MASK              EQU 0fch;0xfc - Including the reserved fields for easier use.
    VGA_GCTLIdx_ColorDontCare           EQU 007h;0x07
    VGA_GCTLIdx_BitMask                 EQU 008h;0x08
VGA_GraphicsControllerData              EQU 03cfh;0x3cf
VGA_GraphicsControllerData_lowbyte      EQU 0cfh;0xcf
VGA_CRTControllerIndexD                 EQU 03d4h;0x3d4
VGA_CRTControllerIndexD_lowbyte         EQU 0d4h;0xd4
    VGA_CRTCIdx_HorTotal                EQU 000h;0x00
    VGA_CRTCIdx_HorDispEnableEnd        EQU 001h;0x01
    VGA_CRTCIdx_StartHorBlanking        EQU 002h;0x02
    VGA_CRTCIdx_EndHorBlanking          EQU 003h;0x03
    VGA_CRTCIdx_StartHorRetracePulse    EQU 004h;0x04
    VGA_CRTCIdx_EndHorRetrace           EQU 005h;0x05
    VGA_CRTCIdx_VertTotal               EQU 006h;0x06
    VGA_CRTCIdx_Overflow                EQU 007h;0x07
    VGA_CRTCIdx_PresetRowScan           EQU 008h;0x08
    VGA_CRTCIdx_MaxScanLine             EQU 009h;0x09
        VGA_CRTC9_MaxScanMask           EQU 01fh;0x1f
    VGA_CRTCIdx_CursorStart             EQU 00ah;0x0a
    VGA_CRTCIdx_CursorEnd               EQU 00bh;0x0b
    VGA_CRTCIdx_StartAddrHigh           EQU 00ch;0x0c
    VGA_CRTCIdx_StartAddrLow            EQU 00dh;0x0d
    VGA_CRTCIdx_CursorLocHigh           EQU 00eh;0x0e
    VGA_CRTCIdx_CursorLocLow            EQU 00fh;0x0f
    VGA_CRTCIdx_VertRetraceStart        EQU 010h;0x10
    VGA_CRTCIdx_VertRetraceEnd          EQU 011h;0x11
    VGA_CRTCIdx_VertDispEnableEnd       EQU 012h;0x12
    VGA_CRTCIdx_Offset                  EQU 013h;0x13
    VGA_CRTCIdx_UnderlineLoc            EQU 014h;0x14
    VGA_CRTCIdx_StartVertBlank          EQU 015h;0x15
    VGA_CRTCIdx_EndVertBlank            EQU 016h;0x16
    VGA_CRTCIdx_ModeControl             EQU 017h;0x17
    VGA_CRTCIdx_LineCompare             EQU 018h;0x18
VGA_CRTControllerDataD                  EQU 03d5h;0x3d5
VGA_CRTControllerDataD_lowbyte          EQU 0d5h;0xd5
VGA_CGAModeControl                      EQU 03d8h;0x3d8
VGA_CGAModeControl_lowbyte              EQU 0d8h;0xd8
    VGA_CGAMC_LoResText                 EQU 000h;0x00
    VGA_CGAMC_HiResText                 EQU 001h;0x01
    VGA_CGAMC_TextMode                  EQU 000h;0x00
    VGA_CGAMC_GraphicsMode              EQU 002h;0x02
    VGA_CGAMC_320x200Color              EQU 000h;0x00
    VGA_CGAMC_320x200Monochrome         EQU 004h;0x04
    VGA_CGAMC_DisplayDisable            EQU 000h;0x00
    VGA_CGAMC_DisplayEnable             EQU 008h;0x08
    VGA_CGAMC_320x200Select             EQU 000h;0x00
    VGA_CGAMC_640x200Select             EQU 010h;0x10
    VGA_CGAMC_Intensity                 EQU 000h;0x00
    VGA_CGAMC_Blinking                  EQU 020h;0x20
VGA_FeatureControlWD                    EQU 03dah;0x3da
    VGA_FC_Bit0                         EQU 001h;0x01
    VGA_FC_Bit1                         EQU 002h;0x02
VGA_FeatureControlWD_lowbyte            EQU 0dah;0xda
VGA_InputStatus1D                       EQU 03dah;0x3da
VGA_InputStatus1D_lowbyte               EQU 0dah;0xda
    VGA_INSTS1_HorActiveDisplayIntv     EQU 000h;0x00
    VGA_INSTS1_HorInactiveDisplayIntv   EQU 001h;0x01
    VGA_INSTS1_VerActiveDisplayIntv     EQU 000h;0x00
    VGA_INSTS1_VerInactiveDisplayIntv   EQU 008h;0x08

FARPointer STRUCT
    off WORD ?
    sgm WORD ?
FARPointer ENDS

VideoOverridePointerTable STRUCT
    VideoParameterTable FARPointer <?,?>
    DynamicParameterSaveArea FARPointer <?,?>
    AplhaNumericCharacterSetOverride FARPointer <?,?>
    GraphicsCharacterSetOverride FARPointer <?,?>
    SecondarySavePointerTable FARPointer <?,?>
    Reserved1 FARPointer <?,?>
    Reserved2 FARPointer <?,?>
VideoOverridePointerTable ENDS

SecondarySavePointerTable STRUCT
    LengthInBytes WORD ?
    DisplayCombinationCodeTable FARPointer <?,?>
    SecondaryAplhanumericCharSetOverride FARPointer <?,?>
    UserPaletteProfileTable FARPointer <?,?>
    Reserved1 FARPointer <?,?>
    Reserved2 FARPointer <?,?>
    Reserved3 FARPointer <?,?>
SecondarySavePointerTable ENDS

DisplayCombinationCodeTable STRUCT
    TableEntryCount BYTE ?
    TableVersionNumber BYTE ?
    MaximumDisplayTypeCode BYTE ?
    Reserved BYTE ?
DisplayCombinationCodeTable ENDS

VideoParametersTable STRUCT ;Struct size = 64 bytes
    DisplayedCharacterColumns BYTE ?                    ;0x00
    DisplayedRowsMinus1 BYTE ?                          ;0x01
    CharacterMatrixHeightPoints BYTE ?                  ;0x02
    VideoBufferSize WORD ?                              ;0x03
    SequencerRegisters DWORD ?          ;SR1-SR4        ;0x05
    MiscOutputRegisterValues BYTE ?                     ;0x09
    CRTCRegisters BYTE 25 DUP (?)       ;CR0-CR18       ;0x0A
    AttributeControllerRegs BYTE 20 DUP (?)             ;0x23
    GraphicsControllerRegs BYTE 9 DUP (?) ;GR0-GR8      ;0x37
VideoParametersTable ENDS
