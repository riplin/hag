;Matrox Mystique Registers (only includes the ones used by the BIOS)

;MGA_CRTCExtensionIndex
    MGA_CRTCExt_Reserved7               EQU 007h;0x07

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

;
; 3 3 2 22222 222 2 1111 1 1 1 1 11
; 1 0 9 87654 321 0 9876 5 4 3 2 109 8 76 5 4 3 2 10
;        |   |     |    |       |     |      |
; 8 4 2 18421 842 1 8421 8 4 2 1 842 1 84 2 1 8 4 21
; 0 0 0 00000 xxx 0 0000 0 0 0 0 000 0 xx 0 0 0 0 00
; | | |     |     |    | | | | |   | |    | | | |  |
; | | |     |     |    | | | | |   | |    | | | |  +--- sysclksl
; | | |     |     |    | | | | |   | |    | | | |
; | | |     |     |    | | | | |   | |    | | | +------ sysclkdis
; | | |     |     |    | | | | |   | |    | | |
; | | |     |     |    | | | | |   | |    | | +-------- gclkdiv
; | | |     |     |    | | | | |   | |    | |
; | | |     |     |    | | | | |   | |    | +---------- mclkdiv
; | | |     |     |    | | | | |   | |    |
; | | |     |     |    | | | | |   | |    +------------ syspllpdn
; | | |     |     |    | | | | |   | |
; | | |     |     |    | | | | |   | +----------------- vgaioen
; | | |     |     |    | | | | |   |
; | | |     |     |    | | | | |   +------------------- fbmskn
; | | |     |     |    | | | | |
; | | |     |     |    | | | | +----------------------- memconfig
; | | |     |     |    | | | |
; | | |     |     |    | | | +------------------------- splitmode
; | | |     |     |    | | |
; | | |     |     |    | | +--------------------------- hardpwmsk
; | | |     |     |    | |
; | | |     |     |    | +----------------------------- unimem
; | | |     |     |    |
; | | |     |     |    +------------------------------- rfhcnt
; | | |     |     |
; | | |     |     +------------------------------------ eepromwt
; | | |     |
; | | |     +------------------------------------------ product id
; | | |
; | | +------------------------------------------------ noretry
; | |
; | +-------------------------------------------------- biosen
; |
; +---------------------------------------------------- powerpc
;
PCI_MGA_Option                          EQU 040h;0x40
    PCI_MGA_Opt_SysClockSelectPCI       EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysClockSelectPLL       EQU 00000001h;0x00000001
    PCI_MGA_Opt_SysClockSelectMCLK      EQU 00000002h;0x00000002
    PCI_MGA_Opt_SysClockSelect_MASK     EQU 00000003h;0x00000003
    PCI_MGA_Opt_SysClockEnable          EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysClockDisable         EQU 00000004h;0x00000004
    PCI_MGA_Opt_SysClock_MASK           EQU 00000004h;0x00000004
    PCI_MGA_Opt_GfxClockDivSel3         EQU 00000000h;0x00000000
    PCI_MGA_Opt_GfxClockDivSel1         EQU 00000008h;0x00000008    
    PCI_MGA_Opt_MemClockDivSel3         EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemClockDivSel1         EQU 00000010h;0x00000010
    PCI_MGA_Opt_SysPLLPowerDown         EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysPLLPowerUp           EQU 00000020h;0x00000020
    PCI_MGA_Opt_VGAIOMapDisable         EQU 00000000h;0x00000000
    PCI_MGA_Opt_VGAIOMapEnable          EQU 00000100h;0x00000100
    PCI_MGA_Opt_FrameBufMask1MB         EQU 00000000h;0x00000000
    PCI_MGA_Opt_FrameBufMask2MB         EQU 00000200h;0x00000200
    PCI_MGA_Opt_FrameBufMask4MB         EQU 00000600h;0x00000600
    PCI_MGA_Opt_FrameBufMask8MB         EQU 00000e00h;0x00000e00
    PCI_MGA_Opt_MemCfg128Kx32           EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemCfg512Kx16           EQU 00001000h;0x00001000
    PCI_MGA_Opt_MemCfg_MASK             EQU 00001000h;0x00001000
    PCI_MGA_Opt_SplitModeDisable        EQU 00000000h;0x00000000
    PCI_MGA_Opt_SplitModeEnable         EQU 00002000h;0x00002000
    PCA_MGA_Opt_HWPlaneWriteMaskOff     EQU 00000000h;0x00000000
    PCA_MGA_Opt_HWPlaneWriteMaskOn      EQU 00004000h;0x00004000
    PCI_MGA_Opt_RefreshCounter_MASK     EQU 000f0000h;0x000f0000
    PCI_MGA_Opt_EEPROMWriteDisable      EQU 00000000h;0x00000000
    PCI_MGA_Opt_EEPROMWriteEnable       EQU 00100000h;0x00100000
    PCI_MGA_Opt_ProductIdMask           EQU 1f000000h;0x1f000000
    PCI_MGA_Opt_Retry                   EQU 00000000h;0x00000000
    PCI_MGA_Opt_NoRetry                 EQU 20000000h;0x20000000
    PCI_MGA_Opt_BIOSDisable             EQU 00000000h;0x00000000
    PCI_MGA_Opt_BIOSEnable              EQU 40000000h;0x40000000
    PCI_MGA_Opt_LittleEndian            EQU 00000000h;0x00000000
    PCI_MGA_Opt_BigEndian               EQU 80000000h;0x80000000
PCI_MGA_IndirectAccessIndex             EQU 044h;0x44
PCI_MGA_IndirectAccessData              EQU 048h;0x48

;
; 3 3 2 2 2222 2222 1111 1 1 11 11
; 1 0 9 8 7654 3210 9876 5 4 32 1098 7654 32 10
;        |    |    |    |      |    |    |
; 8 4 2 1 8421 8421 8421 8 4 21 8421 8421 84 21
; 0 0 0 x xxxx xxxx xxxx 0 0 xx xxxx xxxx xx 00
; | | |                  | |                  |
; | | |                  | |                  +--- pwidth
; | | |                  | |
; | | |                  | +---------------------- jedecrst
; | | |                  |
; | | |                  +------------------------ memreset
; | | |
; | | +------------------------------------------- tlutload
; | |
; | +--------------------------------------------- nodither
; |
; +----------------------------------------------- dit555
;
MGA_MemAddr_MemoryAccess                EQU 1c04h;0x1c04
    MGA_MA_MA_PixelWidth8bpp            EQU 00000000h;0x00000000
    MGA_MA_MA_PixelWidth16bpp           EQU 00000001h;0x00000001
    MGA_MA_MA_PixelWidth32bpp           EQU 00000002h;0x00000002
    MGA_MA_MA_PixelWidth24bpp           EQU 00000003h;0x00000003
    MGA_MA_MA_PixelWidth_MASK           EQU 00000003h;0x00000003
    MGA_MA_MA_MemSeqRegReset8Refresh    EQU 00000000h;0x00000000
    MGA_MA_MA_MemSeq8RefresRegRest      EQU 00004000h;0x00004000
    MGA_MA_MA_MemoryActive              EQU 00000000h;0x00000000
    MGA_MA_MA_MemoryReset               EQU 00008000h;0x00008000
    MGA_MA_MA_FrameBufferTarget         EQU 00000000h;0x00000000
    MGA_MA_MA_TextureLUTTarget          EQU 20000000h;0x20000000
    MGA_MA_MA_DitherEnable              EQU 00000000h;0x00000000
    MGA_MA_MA_DitherDisable             EQU 40000000h;0x40000000
    MGA_MA_MA_Dither565Mode             EQU 00000000h;0x00000000
    MGA_MA_MA_Dither555Mode             EQU 80000000h;0x80000000


;
; 3322 2222 2222 11 11 1111 11 
; 1098 7654 3210 98 76 5432 109 8 7654 32 10 
;     |    |    |     |    |     |    |     
; 8421 8421 8421 84 21 8421 842 1 8421 84 21
; xxxx xxxx xxxx xx 00 xxxx xxx 0 xxxx xx 00
;                    |          |          |
;                    |          |          +--- casltncy
;                    |          |
;                    |          +-------------- rcdelay
;                    |
;                    +------------------------- rasmin
;
MGA_MemAddr_MemoryControlWaitState      EQU 1c08h;0x1c08
    MGA_MA_MCWS_CASLatency2Cycles       EQU 00000000h;0x00000000
    MGA_MA_MCWS_CASLatency3Cycles       EQU 00000001h;0x00000001
    MGA_MA_MCWS_CASLatency_MASK         EQU 00000001h;0x00000001
    MGA_MA_MCWS_RAS2CASDelay2Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RAS2CASDelay3Cycles     EQU 00000100h;0x00000100
    MGA_MA_MCWS_RAS2CASDelay_MASK       EQU 00000100h;0x00000100
    MGA_MA_MCWS_RASMinActive4Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RASMinActive5Cycles     EQU 00010000h;0x00010000
    MGA_MA_MCWS_RASMinActive6Cycles     EQU 00020000h;0x00020000
    MGA_MA_MCWS_RASMinActive7Cycles     EQU 00030000h;0x00030000
    MGA_MA_MCWS_RASMinActive_MASK       EQU 00030000h;0x00030000

MGA_MemAddr_PaletteRamWriteAddress      EQU 3c00h;0x3c00
MGA_MemAddr_IndexedData                 EQU 3c0ah;0x3c0a


VBE_MGA_VendorName                      EQU 013h;0x13 DWord
VBE_MGA_VendorNameOfs                   EQU 013h;0x13 Word
VBE_MGA_VendorNameSeg                   EQU 015h;0x15 Word
VBE_MGA_ProductName                     EQU 017h;0x17 DWord
VBE_MGA_ProductNameOfs                  EQU 017h;0x17 Word
VBE_MGA_ProductNameSeg                  EQU 019h;0x19 Word
VBE_MGA_Revision                        EQU 01bh;0x1b DWord
VBE_MGA_RevisionOfs                     EQU 01bh;0x1b Word
VBE_MGA_RevisionSeg                     EQU 01dh;0x1d Word
VBE_MGA_OEMName                         EQU 01fh;0x1f DWord
VBE_MGA_OEMNameOfs                      EQU 01fh;0x1f Word
VBE_MGA_OEMNameSeg                      EQU 021h;0x21 Word
VBE_MGA_BuildVersion                    EQU 023h;0x23 DWord
VBE_MGA_BuildVersionOfs                 EQU 023h;0x23 Word
VBE_MGA_BuildVersionSeg                 EQU 025h;0x25 Word
VBE_MGA_PINS                            EQU 027h;0x27 DWord
VBE_MGA_PINSOfs                         EQU 027h;0x27 Word
VBE_MGA_PINSSeg                         EQU 029h;0x29 Word
VBE_MGA_MemoryIn64KBBlocks              EQU 02bh;0x2b Word
VBE_MGA_LinearFrameBuffer               EQU 02dh;0x2d DWord
VBE_MGA_LinearControlAperture           EQU 031h;0x31 DWord
VBE_MGA_PCIBusDeviceIdFunctionId        EQU 035h;0x35 Word