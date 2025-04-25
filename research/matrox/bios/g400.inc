;Matrox G400 Registers (only includes the ones used by the BIOS)

include gshared.inc

;MGA_CRTCExtensionIndex
    MGA_CRTCExt_RequesterControl        EQU 007h;0x07
    MGA_CRTCExt_AddressExtension        EQU 008h;0x08
        MGA_CRTCEXT8_StartAddressBit21  EQU 001h;0x01
        MGA_CRTCEXT8_PageRegisterBit8   EQU 010h;0x10
    MGA_CRTCExt_Unknown99               EQU 099h;0x99

MGA_INDD_PanelMode                      EQU 01fh;0x1f
    MGA_PANELMODE_EclipsePanelLink      EQU 000h;0x00
    MGA_PANELMODE_ClientPanelLinkMode   EQU 001h;0x01
    MGA_PANELMODE_HorSyncOn             EQU 000h;0x00
    MGA_PANELMODE_HorSyncOff            EQU 010h;0x10
    MGA_PANELMODE_VerSyncOn             EQU 000h;0x00
    MGA_PANELMODE_VerSyncOff            EQU 020h;0x20
    MGA_PANELMODE_HorPolarityActiveHigh EQU 000h;0x00
    MGA_PANELMODE_HorPolarityActiveLow  EQU 040h;0x40
    MGA_PANELMODE_VerPolarityActiveHigh EQU 000h;0x00
    MGA_PANELMODE_VerPolarityActiveLow  EQU 080h;0x80
MGA_INDD_MAFCDelay                      EQU 020h;0x20

;PCI_MGA_Option
;
; 3 3 2 2 2222 2 2 2 211111 1 1 111
; 1 0 9 8 7654 3 2 1 098765 4 3 210 9 8 7 6 5 4 3 2 10
;        |    |       |   |      |     |       |
; 8 4 2 1 8421 8 4 2 184218 4 2 184 2 1 8 4 2 1 8 4 21
; 0 0 0 0 xxxx x 0 x 000000 0 x 000 x 0 x 0 0 x x 0 xx
; | | | |        |        | |     |   |   | |     |
; | | | |        |        | |     |   |   | |     +------ sysclkdis
; | | | |        |        | |     |   |   | |
; | | | |        |        | |     |   |   | +------------ syspllpdn
; | | | |        |        | |     |   |   |
; | | | |        |        | |     |   |   +-------------- pllsel
; | | | |        |        | |     |   |
; | | | |        |        | |     |   +------------------ vgaioen
; | | | |        |        | |     |
; | | | |        |        | |     +---------------------- memconfig
; | | | |        |        | |
; | | | |        |        | +---------------------------- hardpwmsk
; | | | |        |        |
; | | | |        |        +------------------------------ rfhcnt
; | | | |        |
; | | | |        +--------------------------------------- enhmemacc
; | | | |
; | | | +------------------------------------------------ nohireq
; | | |
; | | +-------------------------------------------------- noretry
; | |
; | +---------------------------------------------------- biosen
; |
; +------------------------------------------------------ powerpc
;

;PCI_MGA_Option
    PCI_MGA_Opt_SysClockEnable          EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysClockDisable         EQU 00000004h;0x00000004
    PCI_MGA_Opt_SysPLLPowerDown         EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysPLLPowerUp           EQU 00000020h;0x00000020
    PCI_MGA_Opt_PLLSelP1PixP2Sys        EQU 00000000h;0x00000000
    PCI_MGA_Opt_PLLSelP1SysP2Pix        EQU 00000040h;0x00000040
    PCI_MGA_Opt_VGAIOMapDisable         EQU 00000000h;0x00000000
    PCI_MGA_Opt_VGAIOMapEnable          EQU 00000100h;0x00000100
    PCI_MGA_Opt_MemCfgOneBankOnBaseBrd  EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemCfgTwoBanksOnBaseBrd EQU 00000400h;0x00000400
    PCI_MGA_Opt_MemCfg8Mb2x128Kx32      EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemCfg16Mb2x256Kx32     EQU 00000800h;0x00000800
    PCI_MGA_Opt_MemCfg16Mb2x512Kx16     EQU 00001000h;0x00001000
    PCI_MGA_Opt_MemCfg16Mb4x128Kx32     EQU 00001800h;0x00001800
    PCA_MGA_Opt_HWPlaneWriteMaskOff     EQU 00000000h;0x00000000
    PCA_MGA_Opt_HWPlaneWriteMaskOn      EQU 00004000h;0x00004000
    PCI_MGA_Opt_RefreshCounter_MASK     EQU 001f8000h;0x001f8000
    PCI_MGA_Opt_EnhancedMemAccessOff    EQU 00000000h;0x00000000
    PCI_MGA_Opt_EnhancedMemAccessOn     EQU 00400000h;0x00400000
    PCI_MGA_OPTION_MFIFONoHiPrioReqEnbl EQU 00000000h;0x00000000
    PCI_MGA_OPTION_MFIFONoHiPrioReqDsbl EQU 10000000h;0x10000000
    PCI_MGA_Opt_Retry                   EQU 00000000h;0x00000000
    PCI_MGA_Opt_NoRetry                 EQU 20000000h;0x20000000
    PCI_MGA_Opt_BIOSDisable             EQU 00000000h;0x00000000
    PCI_MGA_Opt_BIOSEnable              EQU 40000000h;0x40000000
    PCI_MGA_Opt_LittleEndian            EQU 00000000h;0x00000000
    PCI_MGA_Opt_BigEndian               EQU 80000000h;0x80000000

;PCI_MGA_Option2
    PCI_MGA_OPTION2_CPDECClockSelPCI      EQU 00000000h;0x00000000
    PCI_MGA_OPTION2_CPDECClockSelPLL      EQU 01000000h;0x01000000
    PCI_MGA_OPTION2_CPDECClockSelMCLK     EQU 02000000h;0x02000000
    PCI_MGA_OPTION2_CPDECClockSelAGPDLL   EQU 03000000h;0x03000000

;todo:check this.
PCI_MGA_Option3                         EQU 054h;0x54
    PCI_MGA_OPTION3_GfxClkSelPCI          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_GfxClkSelPLL          EQU 00000001h;0x00000001
    PCI_MGA_OPTION3_GfxClkSelMCLK         EQU 00000002h;0x00000002
    PCI_MGA_OPTION3_GfxClkSelAGPDLL       EQU 00000003h;0x00000003
    PCI_MGA_OPTION3_GfxClkSelMASK         EQU 00000003h;0x00000003
    PCI_MGA_OPTION3_GfxClkMul1d3          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_GfxClkMul2d5          EQU 00000008h;0x00000008
    PCI_MGA_OPTION3_GfxClkMul4d9          EQU 00000010h;0x00000010
    PCI_MGA_OPTION3_GfxClkMul1d2          EQU 00000018h;0x00000018
    PCI_MGA_OPTION3_GfxClkMul2d3          EQU 00000020h;0x00000020
    PCI_MGA_OPTION3_GfxClkSrcBypass       EQU 00000028h;0x00000028
    PCI_MGA_OPTION3_GfxClkMulMASK         EQU 00000038h;0x00000038
    PCI_MGA_OPTION3_GfxClkDCCorNone       EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_GfxClkDCCor2p75t3p25  EQU 00000040h;0x00000040
    PCI_MGA_OPTION3_GfxClkDCCor3p25t3p75  EQU 00000080h;0x00000080
    PCI_MGA_OPTION3_GfxClkDCCor3p75t4p25  EQU 000000c0h;0x000000c0
    PCI_MGA_OPTION3_GfxClkDCCor4p25t4p75  EQU 00000100h;0x00000100
    PCI_MGA_OPTION3_GfxClkDCCor4p75t5p25  EQU 00000140h;0x00000140
    PCI_MGA_OPTION3_GfxClkDCCor5p25t5p75  EQU 00000180h;0x00000180
    PCI_MGA_OPTION3_GfxClkDCCor5p75t6p25  EQU 000001c0h;0x000001c0
    PCI_MGA_OPTION3_GfxClkDCCor6p25t6p75  EQU 00000200h;0x00000200
    PCI_MGA_OPTION3_GfxClkDCCor6p75t7p25  EQU 00000240h;0x00000240
    PCI_MGA_OPTION3_GfxClkDCCor7p25t7p75  EQU 00000280h;0x00000280
    PCI_MGA_OPTION3_GfxClkDCCor7p75t8p25  EQU 000002c0h;0x000002c0
    PCI_MGA_OPTION3_GfxClkDCCor8p25t8p75  EQU 00000300h;0x00000300
    PCI_MGA_OPTION3_GfxClkDCCor8p75t9p25  EQU 00000340h;0x00000340
    PCI_MGA_OPTION3_GfxClkDCCor9p25t9p75  EQU 00000380h;0x00000380
    PCI_MGA_OPTION3_GfxClkDCCor9p75t10p25 EQU 000003c0h;0x000003c0
    PCI_MGA_OPTION3_GfxClkDCorMASK        EQU 000003c0h;0x000003c0
    PCI_MGA_OPTION3_MemClkSelPCI          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_MemClkSelPLL          EQU 00000400h;0x00000400
    PCI_MGA_OPTION3_MemClkSelMCLK         EQU 00000800h;0x00000800
    PCI_MGA_OPTION3_MemClkSelAGPDLL       EQU 00000c00h;0x00000c00
    PCI_MGA_OPTION3_MemClkSelMASK         EQU 00000c00h;0x00000c00
    PCI_MGA_OPTION3_MemClkMul1d3          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_MemClkMul2d5          EQU 00002000h;0x00002000
    PCI_MGA_OPTION3_MemClkMul4d9          EQU 00004000h;0x00004000
    PCI_MGA_OPTION3_MemClkMul1d2          EQU 00006000h;0x00006000
    PCI_MGA_OPTION3_MemClkMul2d3          EQU 00008000h;0x00008000
    PCI_MGA_OPTION3_MemClkSrcBypass       EQU 0000a000h;0x0000a000
    PCI_MGA_OPTION4_MemClkMulMASK         EQU 0000e000h;0x0000e000
    PCI_MGA_OPTION3_MemClkDCCorNone       EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_MemClkDCCor2p75t3p25  EQU 00010000h;0x00010000
    PCI_MGA_OPTION3_MemClkDCCor3p25t3p75  EQU 00020000h;0x00020000
    PCI_MGA_OPTION3_MemClkDCCor3p75t4p25  EQU 00030000h;0x00030000
    PCI_MGA_OPTION3_MemClkDCCor4p25t4p75  EQU 00040000h;0x00040000
    PCI_MGA_OPTION3_MemClkDCCor4p75t5p25  EQU 00050000h;0x00050000
    PCI_MGA_OPTION3_MemClkDCCor5p25t5p75  EQU 00060000h;0x00060000
    PCI_MGA_OPTION3_MemClkDCCor5p75t6p25  EQU 00070000h;0x00070000
    PCI_MGA_OPTION3_MemClkDCCor6p25t6p75  EQU 00080000h;0x00080000
    PCI_MGA_OPTION3_MemClkDCCor6p75t7p25  EQU 00090000h;0x00090000
    PCI_MGA_OPTION3_MemClkDCCor7p25t7p75  EQU 000a0000h;0x000a0000
    PCI_MGA_OPTION3_MemClkDCCor7p75t8p25  EQU 000b0000h;0x000b0000
    PCI_MGA_OPTION3_MemClkDCCor8p25t8p75  EQU 000c0000h;0x000c0000
    PCI_MGA_OPTION3_MemClkDCCor8p75t9p25  EQU 000d0000h;0x000d0000
    PCI_MGA_OPTION3_MemClkDCCor9p25t9p75  EQU 000e0000h;0x000e0000
    PCI_MGA_OPTION3_MemClkDCCor9p75t10p25 EQU 000f0000h;0x000f0000
    PCI_MGA_OPTION3_MemClkDCCorMASK       EQU 000f0000h;0x000f0000
    PCI_MGA_OPTION3_WrpClkSelPCI          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_WrpClkSelPLL          EQU 00100000h;0x00100000
    PCI_MGA_OPTION3_WrpClkSelMCLK         EQU 00200000h;0x00200000
    PCI_MGA_OPTION3_WrpClkSelAGPDLL       EQU 00300000h;0x00300000
    PCI_MGA_OPTION3_WrpClkSelMASK         EQU 00300000h;0x00300000
    PCI_MGA_OPTION3_WrpClkMul1d3          EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_WrpClkMul2d5          EQU 00800000h;0x00800000
    PCI_MGA_OPTION3_WrpClkMul4d9          EQU 01000000h;0x01000000
    PCI_MGA_OPTION3_WrpClkMul1d2          EQU 01800000h;0x01800000
    PCI_MGA_OPTION3_WrpClkMul2d3          EQU 02000000h;0x02000000
    PCI_MGA_OPTION3_WrpClkSrcBypass       EQU 02800000h;0x02800000
    PCI_MGA_OPTION3_WrpClkMulMASK         EQU 03800000h;0x03800000
    PCI_MGA_OPTION3_WrpClkDCCorNone       EQU 00000000h;0x00000000
    PCI_MGA_OPTION3_WrpClkDCCor2p75t3p25  EQU 04000000h;0x04000000
    PCI_MGA_OPTION3_WrpClkDCCor3p25t3p75  EQU 08000000h;0x08000000
    PCI_MGA_OPTION3_WrpClkDCCor3p75t4p25  EQU 0c000000h;0x0c000000
    PCI_MGA_OPTION3_WrpClkDCCor4p25t4p75  EQU 10000000h;0x10000000
    PCI_MGA_OPTION3_WrpClkDCCor4p75t5p25  EQU 14000000h;0x14000000
    PCI_MGA_OPTION3_WrpClkDCCor5p25t5p75  EQU 18000000h;0x18000000
    PCI_MGA_OPTION3_WrpClkDCCor5p75t6p25  EQU 1c000000h;0x1c000000
    PCI_MGA_OPTION3_WrpClkDCCor6p25t6p75  EQU 20000000h;0x20000000
    PCI_MGA_OPTION3_WrpClkDCCor6p75t7p25  EQU 24000000h;0x24000000
    PCI_MGA_OPTION3_WrpClkDCCor7p25t7p75  EQU 28000000h;0x28000000
    PCI_MGA_OPTION3_WrpClkDCCor7p75t8p25  EQU 2c000000h;0x2c000000
    PCI_MGA_OPTION3_WrpClkDCCor8p25t8p75  EQU 30000000h;0x30000000
    PCI_MGA_OPTION3_WrpClkDCCor8p75t9p25  EQU 34000000h;0x34000000
    PCI_MGA_OPTION3_WrpClkDCCor9p25t9p75  EQU 38000000h;0x38000000
    PCI_MGA_OPTION3_WrpClkDCCor9p75t10p25 EQU 3c000000h;0x3c000000
    PCI_MGA_OPTION3_WrpClkDCCorMASK       EQU 3c000000h;0x3c000000

;
; 3 3 2 2 2 2 22 2222 1111 1 111 11
; 1 0 9 8 7 6 54 3210 9876 5 432 1098 7654 3 2 10
;        |      |    |    |     |    |     |
; 8 4 2 1 8 4 21 8421 8421 8 421 8421 842 18 4 21
; 0 0 0 0 x 0 xx xxxx xxxx 0 xxx xxxx xxx 00 x 00
; | | | |   |              |               |    |
; | | | |   |              |               |    +--- pwidth
; | | | |   |              |               |
; | | | |   |              |               +-------- zwidth
; | | | |   |              | 
; | | | |   |              +------------------------- memreset
; | | | |   |
; | | | |   +---------------------------------------- fogen
; | | | |
; | | | +-------------------------------------------- bypass332 (Rev B and later)
; | | |
; | | +---------------------------------------------- tlutload
; | |
; | +------------------------------------------------ nodither
; |
; +-------------------------------------------------- dit555
;

;MGA_MemAddr_MemoryAccess
    MGA_MA_MA_PixelWidth8bpp            EQU 00000000h;0x00000000
    MGA_MA_MA_PixelWidth16bpp           EQU 00000001h;0x00000001
    MGA_MA_MA_PixelWidth32bpp           EQU 00000002h;0x00000002
    MGA_MA_MA_PixelWidth24bpp           EQU 00000003h;0x00000003
    MGA_MA_MA_PixelWidth_MASK           EQU 00000003h;0x00000003
    MGA_MA_MA_ZDepthWidth16bitNoStencil EQU 00000000h;0x00000000
    MGA_MA_MA_ZDepthWidth32bitNoStencil EQU 00000008h;0x00000008
    MGA_MA_MA_ZDepthWidth15bitStencil1b EQU 00000010h;0x00000010
    MGA_MA_MA_ZDepthWidth24bitStencil8b EQU 00000018h;0x00000018
    MGA_MA_MA_MemoryActive              EQU 00000000h;0x00000000
    MGA_MA_MA_MemoryReset               EQU 00008000h;0x00008000
    MGA_MA_MA_FogDisable                EQU 00000000h;0x00000000
    MGA_MA_MA_FogEnable                 EQU 04000000h;0x04000000
    MGA_MA_MA_332ConversionEnabled      EQU 00000000h;0x00000000
    MGA_MA_MA_332ConversionBypassed     EQU 10000000h;0x10000000
    MGA_MA_MA_FrameBufferTarget         EQU 00000000h;0x00000000
    MGA_MA_MA_TextureLUTTarget          EQU 20000000h;0x20000000
    MGA_MA_MA_DitherEnable              EQU 00000000h;0x00000000
    MGA_MA_MA_DitherDisable             EQU 40000000h;0x40000000
    MGA_MA_MA_Dither565Mode             EQU 00000000h;0x00000000
    MGA_MA_MA_Dither555Mode             EQU 80000000h;0x80000000

;MGA_MemAddr_MemoryControlWaitState
    MGA_MA_MCWS_CASLatency2Cycles       EQU 00000000h;0x00000000
    MGA_MA_MCWS_CASLatency3Cycles       EQU 00000001h;0x00000001
    MGA_MA_MCWS_CASLatency4Cycles       EQU 00000002h;0x00000002
    MGA_MA_MCWS_CASLatency_MASK         EQU 00000007h;0x00000007
    MGA_MA_MCWS_RAS2RASDelay1Cycle      EQU 00000000h;0x00000000
    MGA_MA_MCWS_RAS2RASDelay2Cycles     EQU 00000010h;0x00000010
    MGA_MA_MCWS_RAS2RASDelay3Cycles     EQU 00000020h;0x00000020
    MGA_MA_MCWS_RAS2RASDelay_MASK       EQU 00000030h;0x00000030
    MGA_MA_MCWS_RAS2CASDelay2Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RAS2CASDelay3Cycles     EQU 00000080h;0x00000080
    MGA_MA_MCWS_RAS2CASDelay4Cycles     EQU 00000100h;0x00000100
    MGA_MA_MCWS_RAS2CASDelay_MASK       EQU 00000180h;0x00000180
    MGA_MA_MCWS_RASMinActive3Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RASMinActive4Cycles     EQU 00000800h;0x00000800
    MGA_MA_MCWS_RASMinActive5Cycles     EQU 00001000h;0x00001000
    MGA_MA_MCWS_RASMinActive6Cycles     EQU 00001800h;0x00001800
    MGA_MA_MCWS_RASMinActive7Cycles     EQU 00002000h;0x00002000
    MGA_MA_MCWS_RASMinActive8Cycles     EQU 00002800h;0x00002800
    MGA_MA_MCWS_RASMinActive_MASK       EQU 00003800h;0x00003800
    MGA_MA_MCWS_RASPrechrgDelay2Cycles  EQU 00000000h;0x00000000
    MGA_MA_MCWS_RASPrechrgDelay3Cycles  EQU 00004000h;0x00004000
    MGA_MA_MCWS_RASPrechrgDelay4Cycles  EQU 00008000h;0x00008000
    MGA_MA_MCWS_RASPrechrgDelay5Cycles  EQU 0000c000h;0x0000c000
    MGA_MA_MCWS_RASPrechrgDelay_MASK    EQU 0000c000h;0x0000c000
    MGA_MA_MCWS_WriteRecovrDelay1Cycle  EQU 00000000h;0x00000000
    MGA_MA_MCWS_WriteRecovrDelay2Cycles EQU 00040000h;0x00040000
    MGA_MA_MCWS_WriteRecovrDelay_MASK   EQU 000c0000h;0x000c0000
    MGA_MA_MCWS_Rd2PrechrgDelNCycles    EQU 00000000h;0x00000000
    MGA_MA_MCWS_Rd2PrechrgDelNpCLm2Cyc  EQU 00200000h;0x00200000
    MGA_MA_MCWS_Rd2PrechrgDelay_MASK    EQU 00200000h;0x00200000
    MGA_MA_MCWS_SpecModeRegDelay1Cycle  EQU 00000000h;0x00000000
    MGA_MA_MCWS_SpecModeRegDelay2Cycles EQU 00800000h;0x00800000
    MGA_MA_MCWS_SpecModeRegDelay_MASK   EQU 01800000h;0x01800000
    MGA_MA_MCWS_BlockWriteDelay1Cycle   EQU 00000000h;0x00000000
    MGA_MA_MCWS_BlockWriteDelay2Cycles  EQU 04000000h;0x04000000
    MGA_MA_MCWS_BlockWriteDelay_MASK    EQU 0c000000h;0x0c000000
    MGA_MA_MCWS_BW2PrechrgDelay1Cycle   EQU 00000000h;0x00000000
    MGA_MA_MCWS_BW2PrechrgDelay2Cycles  EQU 20000000h;0x20000000
    MGA_MA_MCWS_BW2PrechrgDelay3Cycles  EQU 40000000h;0x40000000
    MGA_MA_MCWS_BW2PrechrgDelay4Cycles  EQU 60000000h;0x60000000
    MGA_MA_MCWS_BW2PrechrgDelay5Cycles  EQU 80000000h;0x80000000
    MGA_MA_MCWS_BW2PrechrgDelay_MASK    EQU e0000000h;0xe0000000

MGA_MemAddr_MemoryReadBack              EQU 1e44h;0x1e44
    MGA_MA_MRB_ModeRegSetCMDOpcode_MASK EQU 3e000000h;0x3e000000
MGA_MemAddr_ConfigurationOverride       EQU 1e4ch;0x1e4c
    MGA_MA_CFGOR_InternalCompensation   EQU 00000000h;0x00000000
    MGA_MA_CFGOR_ComporUpDown           EQU 00000008h;0x00000008
    MGA_MA_CFGOR_CompFreq16ms           EQU 00000000h;0x00000000
    MGA_MA_CFGOR_CompFreq31ms           EQU 00000010h;0x00000010
    MGA_MA_CFGOR_CompFreq63ms           EQU 00000020h;0x00000020
    MGA_MA_CFGOR_CompFreq125ms          EQU 00000030h;0x00000030
    MGA_MA_CFGOR_CompFreq250ms          EQU 00000040h;0x00000040
    MGA_MA_CFGOR_CompFreq500ms          EQU 00000050h;0x00000050
    MGA_MA_CFGOR_CompFreq1000ms         EQU 00000060h;0x00000060
    MGA_MA_CFGOR_CompFreq2000ms         EQU 00000070h;0x00000070
    MGA_MA_CFGOR_ComporUp_MASK          EQU 00000f00h;0x00000f00
    MGA_MA_CFGOR_ComporDown_MASK        EQU 0000f000h;0x0000f000
    MGA_MA_CFGOR_SEEPROM_DataOut        EQU 00010000h;0x00010000
    MGA_MA_CFGOR_SEEPROM_BypassChipSel  EQU 00020000h;0x00020000
    MGA_MA_CFGOR_SEEPROM_BypassData     EQU 00040000h;0x00040000
    MGA_MA_CFGOR_SEEPROM_BypassClock    EQU 00080000h;0x00080000
    MGA_MA_CFGOR_SEEPROM_StatMchBypass  EQU 00100000h;0x00100000
    MGA_MA_CFGOR_RateCapOr_MASK         EQU 00e00000h;0x00e00000
    MGA_MA_CFGOR_RQOr_MASK              EQU 1f000000h;0x1f000000
MGA_MemAddr_CRTC2Control                EQU 3c10h;0x3c10
    MGA_MA_CRTC2C_Enable                EQU 00000001h;0x00000001

