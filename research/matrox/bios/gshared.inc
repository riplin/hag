;Matrox G100/G200/G400 shared registers

;VGA_CRTControllerIndex
    MGA_CRTCIdx_CPUReadLatch            EQU 022h;0x22
    MGA_CRTCIdx_AttribAddrDataSelect    EQU 024h;0x24
    MGA_CRTCIdx_AttribAddress           EQU 026h;0x26

;VGA_MiscellaneousRead
    MGA_MISC_MGAPixelClock              EQU 00ch;0x0c

MGA_CRTCExtensionIndex                  EQU 03deh;0x3de
MGA_CRTCExtensionIndex_lowbyte          EQU 0deh;0xde
    MGA_CRTCExt_AddrGeneratorExt        EQU 000h;0x00
        MGA_CRTEXT0_StartAddress19_16   EQU 00fh;0x0f
        MGA_CRTEXT0_Offset9_8           EQU 030h;0x30
        MGA_CRTEXT0_StartAddress20      EQU 040h;0x40
        MGA_CRTEXT0_Interlace           EQU 080h;0x80
    MGA_CRTCExt_HorCounterExt           EQU 001h;0x01
        MGA_CRTCEXT1_HorTotal8          EQU 001h;0x01
        MGA_CRTCEXT1_HorBlankStart8     EQU 002h;0x02
        MGA_CRTCEXT1_HorRetraceStart8   EQU 004h;0x04
        MGA_CRTCEXT1_HorResetEnable     EQU 008h;0x08
        MGA_CRTCEXT1_HorSyncOff         EQU 010h;0x10
        MGA_CRTCEXT1_VerSyncOff         EQU 020h;0x20
        MGA_CRTCEXT1_HorBlankEnd        EQU 040h;0x40
        MGA_CRTCEXT1_VerResetEnable     EQU 080h;0x80
    MGA_CRTCExt_VertCounterExt          EQU 002h;0x02
        MGA_CRTCEXT2_VertTotal10        EQU 001h;0x01
        MGA_CRTCEXT2_VertTotal11        EQU 002h;0x02
        MGA_CRTCEXT2_VertTotal11_10     EQU 003h;0x03
        MGA_CRTCEXT2_VertDispEnd10      EQU 004h;0x04
        MGA_CRTCEXT2_VertBlnkStart10    EQU 008h;0x08
        MGA_CRTCEXT2_VertBlnkStart11    EQU 010h;0x10
        MGA_CRTCEXT2_VertBlnkStart11_10 EQU 018h;0x18
        MGA_CRTCEXT2_VertRetrStrt10     EQU 020h;0x20
        MGA_CRTCEXT2_VertRetrStrt11     EQU 040h;0x40
        MGA_CRTCEXT2_VertRetrStrt11_10  EQU 060h;0x60
        MGA_CRTCEXT2_LineComp10         EQU 080h;0x80
    MGA_CRTCExt_Misc                    EQU 003h;0x03
        MGA_CRTCEXT3_ScaleDiv1          EQU 000h;0x00
        MGA_CRTCEXT3_ScaleDiv2          EQU 001h;0x01
        MGA_CRTCEXT3_ScaleDiv3          EQU 002h;0x02
        MGA_CRTCEXT3_ScaleDiv4          EQU 003h;0x03
        MGA_CRTCEXT3_ScaleDiv6          EQU 005h;0x05
        MGA_CRTCEXT3_ScaleDiv8          EQU 007h;0x07
        MGA_CRTCEXT3_ScaleMask          EQU 007h;0x07
        MGA_CRTCEXT3_Accel256Disable    EQU 020h;0x20
        MGA_CRTCEXT3_CompSyncEnable     EQU 040h;0x40
        MGA_CRTCEXT3_MGAModeEnable      EQU 080h;0x80
    MGA_CRTCExt_MemoryPage              EQU 004h;0x04
    MGA_CRTCExt_HorVidHalfCount         EQU 005h;0x05
    MGA_CRTCExt_PrioRequestControl      EQU 006h;0x06
        MGA_CRTCEXT6_HiPrioLevel1       EQU 000h;0x00
        MGA_CRTCEXT6_HiPrioLevel2       EQU 001h;0x01
        MGA_CRTCEXT6_HiPrioLevel3       EQU 002h;0x02
        MGA_CRTCEXT6_HiPrioLevel4       EQU 003h;0x03
        MGA_CRTCEXT6_HiPrioLevel5       EQU 004h;0x04
        MGA_CRTCEXT6_HiPrioLevel6       EQU 005h;0x05
        MGA_CRTCEXT6_HiPrioLevel7       EQU 006h;0x06
        MGA_CRTCEXT6_HiPrioLevel8       EQU 007h;0x07
        MGA_CRTCEXT6_HiPrioLevelMASK    EQU 007h;0x07
        MGA_CRTCEXT6_MaxPrioLevel1      EQU 000h;0x00
        MGA_CRTCEXT6_MaxPrioLevel2      EQU 010h;0x10
        MGA_CRTCEXT6_MaxPrioLevel3      EQU 020h;0x20
        MGA_CRTCEXT6_MaxPrioLevel4      EQU 030h;0x30
        MGA_CRTCEXT6_MaxPrioLevel5      EQU 040h;0x40
        MGA_CRTCEXT6_MaxPrioLevel6      EQU 050h;0x50
        MGA_CRTCEXT6_MaxPrioLevel7      EQU 060h;0x60
        MGA_CRTCEXT6_MaxPrioLevel8      EQU 070h;0x70
        MGA_CRTCEXT6_MaxPrioLevelMASK   EQU 070h;0x70
MGA_CRTCExtensionData                   EQU 03dfh;0x3df
MGA_CRTCExtensionData_lowbyte           EQU 0dfh;0xdf

MGA_INDD_CursorBaseAddressLow           EQU 004h;0x04
MGA_INDD_CursorBaseAddressHigh          EQU 005h;0x05
MGA_INDD_CursorControl                  EQU 006h;0x06
    MGA_CURCTRL_CursorDisabled          EQU 000h;0x00
    MGA_CURCTRL_ThreeColorCursot        EQU 001h;0x01
    MGA_CURCTRL_XGACursor               EQU 002h;0x02
    MGA_CURCTRL_XWindowsCursor          EQU 003h;0x03
    MGA_CURCTRL_16ColorPalettizedCursor EQU 004h;0x04
MGA_INDD_CursorColor0Red                EQU 008h;0x08
MGA_INDD_CursorColor0Green              EQU 009h;0x09
MGA_INDD_CursorColor0Blue               EQU 00ah;0x0a
MGA_INDD_CursorColor1Red                EQU 00ch;0x0c
MGA_INDD_CursorColor1Green              EQU 00dh;0x0d
MGA_INDD_CursorColor1Blue               EQU 00eh;0x0e
MGA_INDD_CursorColor2Red                EQU 010h;0x10
MGA_INDD_CursorColor2Green              EQU 011h;0x11
MGA_INDD_CursorColor2Blue               EQU 012h;0x12
MGA_INDD_VoltageReferenceControl        EQU 018h;0x18
    MGA_VREFCTRL_SysPLLVoltRefBlkPwrDwn EQU 000h;0x00
    MGA_VREFCTRL_SysPLLVoltRefBlkPwrUp  EQU 001h;0x01
    MGA_VREFCTRL_SysPLLVoltRefExtern    EQu 000h;0x00
    MGA_VREFCTRL_SysPLLVoltRefPLLRefBlk EQU 002h;0x02
MGA_INDD_MultiplexControl               EQU 019h;0x19
    MGA_MULCTRL_Depth8                  EQU 000h;0x00
    MGA_MULCTRL_Depth15                 EQU 001h;0x01
    MGA_MULCTRL_Depth16                 EQU 002h;0x02
    MGA_MULCTRL_Depth24                 EQU 003h;0x03
    MGA_MULCTRL_Depth32Direct           EQU 004h;0x04
    MGA_MULCTRL_Depth32Pal              EQU 007h;0x07
    MGA_MULCTRL_DepthMask               EQU 007h;0x07
MGA_INDD_PixelClockControl              EQU 01ah;0x1a
    MGA_PIXCLKCTRL_SelPCI               EQU 000h;0x00
    MGA_PIXCLKCTRL_SelPLL               EQU 001h;0x01
    MGA_PIXCLKCTRL_SelVDOCLK            EQU 002h;0x02
    MGA_PIXCLKCTRL_SelMASK              EQU 003h;0x03
    MGA_PIXCLKCTRL_ClockEnable          EQU 000h;0x00
    MGA_PIXCLKCTRL_ClockDisable         EQU 004h;0x04
    MGA_PIXCLKCTRL_ClockMASK            EQU 004h;0x04
    MGA_PIXCLKCTRL_PixelPLLPowerDown    EQU 000h;0x00
    MGA_PIXCLKCTRL_PixelPLLPowerUp      EQU 008h;0x08
MGA_INDD_GeneralControl                 EQU 01dh;0x1d
    MGA_GENCTRL_AlphaDisable            EQU 000h;0x00
    MGA_GENCTRL_AlphaEnable             EQU 002h;0x02
    MGA_GENCTRL_PedestalContr0IRE       EQU 000h;0x00
    MGA_GENCTRL_PedestalContr7p5IRE     EQU 010h;0x10
    MGA_GENCTRL_PedestalControl_MASK    EQU 010h;0x10
    MGA_GENCTRL_GreenSyncOutEnable      EQU 000h;0x00
    MGA_GENCTRL_GreenSyncOutDisable     EQU 020h;0x20
    MGA_GENCTRL_GreenSyncOut_MASK       EQU 020h;0x20
MGA_INDD_MiscellaneousControl           EQU 01eh;0x1e
    MGA_MISCCTRL_DAC_Disable            EQU 000h;0x00
    MGA_MISCCTRL_DAC_Enable             EQU 001h;0x01
    MGA_MISCCTRL_DAC_MASK               EQU 001h;0x01
    MGA_MISCCTRL_MAFC_MAFC              EQU 002h;0x02
    MGA_MISCCTRL_MAFC_PanelLinkMode     EQU 004h;0x04
    MGA_MISCCTRL_MAFC_Disable           EQU 006h;0x06
    MGA_MISCCTRL_MAFC_MASK              EQU 006h;0x06
    MGA_MISCCTRL_DAC_6Bit               EQU 000h;0x00
    MGA_MISCCTRL_DAC_8Bit               EQU 008h;0x08
    MGA_MISCCTRL_DAC_Bit_MASK           EQU 008h;0x08
    MGA_MISCCTRL_LUT_Disable            EQU 000h;0x00
    MGA_MISCCTRL_LUT_Enable             EQU 010h;0x00
    MGA_MISCCTRL_LUT_MASK               EQU 010h;0x00
    MGA_MISCCTRL_VDOutSel_MAFC12        EQU 000h;0x00
    MGA_MISCCTRL_VDOutSel_BYPASS656     EQU 040h;0x40
MGA_INDD_GeneralPurposeIOControl        EQU 02ah;0x2a
    MGA_GENIOCTRL_DDC0Enable            EQU 001h;0x01
    MGA_GENIOCTRL_DDC1Enable            EQU 002h;0x02
    MGA_GENIOCTRL_DDC2Enable            EQU 004h;0x04
    MGA_GENIOCTRL_DDC3Enable            EQU 008h;0x08
    MGA_GENIOCTRL_Misc0Enable           EQU 010h;0x10
    MGA_GENIOCTRL_Misc1Enable           EQU 020h;0x20
    MGA_GENIOCTRL_Misc2Enable           EQU 040h;0x40
MGA_INDD_GeneralPurposeIOData           EQU 02bh;0x2b
    MGA_GENIOCTRL_DDC0Data              EQU 001h;0x01
    MGA_GENIOCTRL_DDC1Data              EQU 002h;0x02
    MGA_GENIOCTRL_DDC2Data              EQU 004h;0x04
    MGA_GENIOCTRL_DDC3Data              EQU 008h;0x08
    MGA_GENIOCTRL_Misc0Data             EQU 010h;0x10
    MGA_GENIOCTRL_Misc1Data             EQU 020h;0x20
    MGA_GENIOCTRL_Misc2Data             EQU 040h;0x40
MGA_INDD_SYSPLL_M_Value                 EQU 02ch;0x2c
    MGA_SYSPLLM_Mask                    EQU 01fh;0x1f
MGA_INDD_SYSPLL_N_Value                 EQU 02dh;0x2d
MGA_INDD_SYSPLL_P_Value                 EQU 02eh;0x2e
MGA_INDD_SYSPLL_Status                  EQU 02fh;0x2f
    MGA_SYSPLLSTAT_FrequencyLock        EQU 040h;0x40
MGA_INDD_ZoomControl                    EQU 038h;0x38
MGA_INDD_SenseTest                      EQU 03ah;0x3a
MGA_INDD_CRC_RemainderLow               EQU 03ch;0x3c
MGA_INDD_CRC_RemainderHigh              EQU 03dh;0x3d
MGA_INDD_CRC_BitSelect                  EQU 03eh;0x3e
MGA_INDD_Reserved3f                     EQU 03fh;0x3f
MGA_INDD_ColorKeyMask                   EQU 040h;0x40
MGA_INDD_Reserved41                     EQU 041h;0x41
MGA_INDD_ColorKey                       EQU 042h;0x42
MGA_INDD_Reserved43                     EQU 043h;0x43
MGA_INDD_PIXPLL_M_ValueSetA             EQU 044h;0x44
MGA_INDD_PIXPLL_N_ValueSetA             EQU 045h;0x45
MGA_INDD_PIXPLL_P_ValueSetA             EQU 046h;0x46
MGA_INDD_PIXPLL_M_ValueSetB             EQU 048h;0x48
MGA_INDD_PIXPLL_N_ValueSetB             EQU 049h;0x49
MGA_INDD_PIXPLL_P_ValueSetB             EQU 04ah;0x4a
MGA_INDD_PIXPLL_M_ValueSetC             EQU 04ch;0x4c
MGA_INDD_PIXPLL_N_ValueSetC             EQU 04dh;0x4d
MGA_INDD_PIXPLL_P_ValueSetC             EQU 04eh;0x4e
    MGA_PIXPLL_M_MASK                   EQU 01fh;0x1f
MGA_INDD_PIXPLL_Status                  EQU 04fh;0x4f
    MGA_PIXPLLSTAT_FrequencyLock        EQU 040h;0x40
MGA_INDD_KEYING_OperatingMode           EQU 051h;0x51
MGA_INDD_ColorMask0Red                  EQU 052h;0x52
MGA_INDD_ColorMask0Green                EQU 053h;0x53
MGA_INDD_ColorMask0Blue                 EQU 054h;0x54
MGA_INDD_ColorKey0Red                   EQU 055h;0x55
MGA_INDD_ColorKey0Green                 EQU 056h;0x56
MGA_INDD_ColorKey0Blue                  EQU 057h;0x57
MGA_INDD_CursorColor3Red                EQU 060h;0x60
MGA_INDD_CursorColor3Green              EQU 061h;0x61
MGA_INDD_CursorColor3Blue               EQU 062h;0x62
MGA_INDD_CursorColor4Red                EQU 063h;0x63
MGA_INDD_CursorColor4Green              EQU 064h;0x64
MGA_INDD_CursorColor4Blue               EQU 065h;0x65
MGA_INDD_CursorColor5Red                EQU 066h;0x66
MGA_INDD_CursorColor5Green              EQU 067h;0x67
MGA_INDD_CursorColor5Blue               EQU 068h;0x68
MGA_INDD_CursorColor6Red                EQU 069h;0x69
MGA_INDD_CursorColor6Green              EQU 06ah;0x6a
MGA_INDD_CursorColor6Blue               EQU 06bh;0x6b
MGA_INDD_CursorColor7Red                EQU 06ch;0x6c
MGA_INDD_CursorColor7Green              EQU 06dh;0x6d
MGA_INDD_CursorColor7Blue               EQU 06eh;0x6e
MGA_INDD_CursorColor8Red                EQU 06fh;0x6f
MGA_INDD_CursorColor8Green              EQU 070h;0x70
MGA_INDD_CursorColor8Blue               EQU 071h;0x71
MGA_INDD_CursorColor9Red                EQU 072h;0x72
MGA_INDD_CursorColor9Green              EQU 073h;0x73
MGA_INDD_CursorColor9Blue               EQU 074h;0x74
MGA_INDD_CursorColor10Red               EQU 075h;0x75
MGA_INDD_CursorColor10Green             EQU 076h;0x76
MGA_INDD_CursorColor10Blue              EQU 077h;0x77
MGA_INDD_CursorColor11Red               EQU 078h;0x78
MGA_INDD_CursorColor11Green             EQU 079h;0x79
MGA_INDD_CursorColor11Blue              EQU 07ah;0x7a
MGA_INDD_CursorColor12Red               EQU 07bh;0x7b
MGA_INDD_CursorColor12Green             EQU 07ch;0x7c
MGA_INDD_CursorColor12Blue              EQU 07dh;0x7d
MGA_INDD_CursorColor13Red               EQU 07eh;0x7e
MGA_INDD_CursorColor13Green             EQU 07fh;0x7f
MGA_INDD_CursorColor13Blue              EQU 080h;0x80
MGA_INDD_CursorColor14Red               EQU 081h;0x81
MGA_INDD_CursorColor14Green             EQU 082h;0x82
MGA_INDD_CursorColor14Blue              EQU 083h;0x83
MGA_INDD_CursorColor15Red               EQU 084h;0x84
MGA_INDD_CursorColor15Green             EQU 085h;0x85
MGA_INDD_CursorColor15Blue              EQU 086h;0x86

PCI_MGA_Option                          EQU 040h;0x40
PCI_MGA_IndirectAccessIndex             EQU 044h;0x44
PCI_MGA_IndirectAccessData              EQU 048h;0x48
PCI_MGA_SubsystemVendorIDWrite          EQU 04ch;0x4c
PCI_MGA_SubsystemIDWrite                EQU 04eh;0x4e
PCI_MGA_Option2                         EQU 050h;0x50
PCI_MGA_PowerMgmtIdentifier             EQU 0dch;0xdc
PCI_MGA_PowerMgmtControlStatus          EQU 0e0h;0xe0
    PCI_MGA_PWRCTRL_D0                  EQU 000h;0x00
    PCI_MGA_PWRCTRL_D3                  EQU 003h;0x03
PCI_MGA_AGPCapIdentifier                EQU 0f0h;0xf0
PCI_MGA_AGPStatus                       EQU 0f4h;0xf4
PCI_MGA_AGPCommand                      EQU 0f8h;0xf8
    PCI_MGA_AGPCMD_RateReset            EQU 000h;0x00
    PCI_MGA_AGPCMD_Rate1x               EQU 001h;0x01
    PCI_MGA_AGPCMD_Rate2x               EQU 002h;0x02
    PCI_MGA_AGPCMD_AGPEnable            EQU 100h;0x100
    PCI_MGA_AGPCMD_SBAEnable            EQU 200h;0x200
    PCI_MGA_AGPCMD_RQDepthMask          EQU 1f000000h;0x1f000000

MGA_MemAddr_MemoryAccess                EQU 1c04h;0x1c04
MGA_MemAddr_MemoryControlWaitState      EQU 1c08h;0x1c08
MGA_MemAddr_ZDepthOrigin                EQU 1c0ch;0x1c0c
MGA_MemAddr_PlaneWriteMask              EQU 1c1ch;0x1c1c
MGA_MemAddr_Status                      EQU 1e14h;0x1e14
MGA_MemAddr_SequencerIndex              EQU 1fc4h;0x1fc4
    MGA_MA_STAT_SoftwareFlag_MASK       EQU 0f0000000h;0xf0000000
MGA_MemAddr_PaletteRamWriteAddress      EQU 3c00h;0x3c00
MGA_MemAddr_IndexedData                 EQU 3c0ah;0x3c0a
