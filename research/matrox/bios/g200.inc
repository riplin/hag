;Matrox G200 Registers (only includes the ones used by the BIOS)

include gshared.inc

;MGA_CRTCExtensionIndex
    MGA_CRTCExt_RequesterControl        EQU 007h;0x07

MGA_MemAddr_MemoryReadBack              EQU 1e44h;0x1e44
    MGA_MA_MRB_ModeRegSetCMDOpcode_MASK EQU 3e000000h;0x3e000000

;
; 3 3 2 2 2 2 22 2222 1111 1 111 11
; 1 0 9 8 7 6 54 3210 9876 5 432 1098 7654 3 2 10
;        |      |    |    |     |    |    |
; 8 4 2 1 8 4 21 8421 8421 8 421 8421 8421 8 4 21
; 0 0 0 x x 0 xx xxxx xxxx 0 xxx xxxx xxxx 0 x 00
; | | |     |              |               |    |
; | | |     |              |               |    +--- pwidth
; | | |     |              |               |
; | | |     |              |               +-------- zwidth
; | | |     |              | 
; | | |     |              +------------------------- memreset
; | | |     |
; | | |     +---------------------------------------- fogen
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
    MGA_MA_MA_ZDepthWidth16bit          EQU 00000000h;0x00000000
    MGA_MA_MA_ZDepthWidth32bit          EQU 00000008h;0x00000008
    MGA_MA_MA_MemoryActive              EQU 00000000h;0x00000000
    MGA_MA_MA_MemoryReset               EQU 00008000h;0x00008000
    MGA_MA_MA_FogDisable                EQU 00000000h;0x00000000
    MGA_MA_MA_FogEnable                 EQU 04000000h;0x04000000
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

;
; 3 3 2 2 2222 2 2 2 211111 1 1 111
; 1 0 9 8 7654 3 2 1 098765 4 3 210 9 8 7 6 5 4 3 2 10
;        |    |       |   |      |     |       |
; 8 4 2 1 8421 8 4 2 184218 4 2 184 2 1 8 4 2 1 8 4 21
; 0 0 0 x xxxx x 0 x 000000 0 x 000 x 0 x 0 0 0 0 0 00
; | | |          |        | |     |   |   | | | | |  |
; | | |          |        | |     |   |   | | | | |  +--- sysclksl
; | | |          |        | |     |   |   | | | | |
; | | |          |        | |     |   |   | | | | +------ sysclkdis
; | | |          |        | |     |   |   | | | |
; | | |          |        | |     |   |   | | | +-------- gclkdiv
; | | |          |        | |     |   |   | | |
; | | |          |        | |     |   |   | | +---------- mclkdiv
; | | |          |        | |     |   |   | |
; | | |          |        | |     |   |   | +------------ syspllpdn
; | | |          |        | |     |   |   |
; | | |          |        | |     |   |   +-------------- pllsel
; | | |          |        | |     |   |
; | | |          |        | |     |   +------------------ vgaioen
; | | |          |        | |     |
; | | |          |        | |     +---------------------- memconfig
; | | |          |        | |
; | | |          |        | +---------------------------- hardpwmsk
; | | |          |        |
; | | |          |        +------------------------------ rfhcnt
; | | |          |
; | | |          +--------------------------------------- enhmemacc
; | | |
; | | +-------------------------------------------------- noretry
; | |
; | +---------------------------------------------------- biosen
; |
; +------------------------------------------------------ powerpc
;

;PCI_MGA_Option
    PCI_MGA_Opt_SysClockSelectPCI       EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysClockSelectPLL       EQU 00000001h;0x00000001
    PCI_MGA_Opt_SysClockSelectMCLK      EQU 00000002h;0x00000002
    PCI_MGA_Opt_SysClockSelect_MASK     EQU 00000003h;0x00000003
    PCI_MGA_Opt_SysClockEnable          EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysClockDisable         EQU 00000004h;0x00000004
    PCI_MGA_Opt_GfxClockDivSel2         EQU 00000000h;0x00000000
    PCI_MGA_Opt_GfxClockDivSel3d2       EQU 00000008h;0x00000008    
    PCI_MGA_Opt_MemClockDivSel2         EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemClockDivSel3d2       EQU 00000010h;0x00000010
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
    PCI_MGA_Opt_Retry                   EQU 00000000h;0x00000000
    PCI_MGA_Opt_NoRetry                 EQU 20000000h;0x20000000
    PCI_MGA_Opt_BIOSDisable             EQU 00000000h;0x00000000
    PCI_MGA_Opt_BIOSEnable              EQU 40000000h;0x40000000
    PCI_MGA_Opt_LittleEndian            EQU 00000000h;0x00000000
    PCI_MGA_Opt_BigEndian               EQU 80000000h;0x80000000

;PCI_MGA_Option2
    PCI_MGA_Opt2_MBufType0LVTTL         EQU 00000000h;0x00000000
    PCI_MGA_Opt2_MBufType0SSTL_3        EQU 00001000h;0x00001000
    PCI_MGA_Opt2_MBufType1LVTTL         EQU 00000000h;0x00000000
    PCI_MGA_Opt2_MBufType1SSTL_3        EQU 00002000h;0x00002000
    PCI_MGA_Opt2_MBufType_MASK          EQU 00003000h;0x00003000
    PCI_MGA_Opt2_UseGfxClockDiv         EQU 00000000h;0x00000000
    PCI_MGA_Opt2_NoGfxClockDiv          EQU 00004000h;0x00004000
    PCI_MGA_Opt2_GfxClockDiv_MASK       EQU 00004000h;0x00004000
    PCI_MGA_Opt2_UseMemClockDiv         EQU 00000000h;0x00000000
    PCI_MGA_Opt2_NoMemClockDiv          EQU 00008000h;0x00008000
    PCI_MGA_Opt2_MemClockDiv_MASK       EQU 00008000h;0x00008000