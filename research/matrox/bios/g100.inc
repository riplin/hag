;Matrox G100 Registers (only includes the ones used by the BIOS)

include gshared.inc

;MGA_CRTCExtensionIndex
    MGA_CRTCExt_Reserved7               EQU 007h;0x07

;
; 3 3 2 2 2222 2 2 2 211111 1 1 1 11
; 1 0 9 8 7654 3 2 1 098765 4 3 2 109 8 7 6 5 4 3 2 10
;        |    |       |   |      |     |       |
; 8 4 2 1 8421 8 4 2 184218 4 2 1 842 1 8 4 2 1 8 4 21
; 0 0 0 x xxxx x 0 x 000000 0 0 0 xxx 0 0 0 0 0 0 0 00
; | | |          |        |   | |     | | | | | | |  |
; | | |          |        |   | |     | | | | | | |  +--- sysclksl
; | | |          |        |   | |     | | | | | | |
; | | |          |        |   | |     | | | | | | +------ sysclkdis
; | | |          |        |   | |     | | | | | |
; | | |          |        |   | |     | | | | | +-------- gclkdiv
; | | |          |        |   | |     | | | | |
; | | |          |        |   | |     | | | | +---------- mclkdiv
; | | |          |        |   | |     | | | |
; | | |          |        |   | |     | | | +------------ syspllpdn
; | | |          |        |   | |     | | |
; | | |          |        |   | |     | | +-------------- pllsel
; | | |          |        |   | |     | |
; | | |          |        |   | |     | +---------------- fmclkdiv
; | | |          |        |   | |     |
; | | |          |        |   | |     +------------------ vgaioen
; | | |          |        |   | |  
; | | |          |        |   | +------------------------ memconfig
; | | |          |        |   |
; | | |          |        |   +-------------------------- splitmode
; | | |          |        |
; | | |          |        +------------------------------ rfhcnt
; | | |          |
; | | |          +--------------------------------------- mrmoption
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
    PCI_MGA_Opt_SysClock_MASK           EQU 00000004h;0x00000004
    PCI_MGA_Opt_GfxClockDivSel2         EQU 00000000h;0x00000000
    PCI_MGA_Opt_GfxClockDivSel3         EQU 00000008h;0x00000008    
    PCI_MGA_Opt_MemClockDivSel2         EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemClockDivSel3         EQU 00000010h;0x00000010
    PCI_MGA_Opt_SysPLLPowerDown         EQU 00000000h;0x00000000
    PCI_MGA_Opt_SysPLLPowerUp           EQU 00000020h;0x00000020
    PCI_MGA_Opt_PLLSelP0SysP1Pix        EQU 00000000h;0x00000000
    PCI_MGA_Opt_PLLSelP0PixP1Sys        EQU 00000040h;0x00000040
    PCI_MGA_Opt_FastMemClkDiv1          EQU 00000000h;0x00000000
    PCI_MGA_Opt_FastMemClkDiv2          EQU 00000080h;0x00000080
    PCI_MGA_Opt_VGAIOMapDisable         EQU 00000000h;0x00000000
    PCI_MGA_Opt_VGAIOMapEnable          EQU 00000100h;0x00000100
    PCI_MGA_Opt_MemCfg128Kx32           EQU 00000000h;0x00000000
    PCI_MGA_Opt_MemCfg256Kx32           EQU 00001000h;0x00001000
    PCI_MGA_Opt_MemCfg_MASK             EQU 00001000h;0x00001000
    PCI_MGA_Opt_SplitModeDisable        EQU 00000000h;0x00000000
    PCI_MGA_Opt_SplitModeEnable         EQU 00002000h;0x00002000
    PCI_MGA_Opt_RefreshCounter_MASK     EQU 001f8000h;0x001f8000
    PCI_MGA_Opt_PCIMR                   EQU 00000000h;0x00000000
    PCI_MGA_Opt_PCIMRM                  EQU 00400000h;0x00400000
    PCI_MGA_Opt_Retry                   EQU 00000000h;0x00000000
    PCI_MGA_Opt_NoRetry                 EQU 20000000h;0x20000000
    PCI_MGA_Opt_BIOSDisable             EQU 00000000h;0x00000000
    PCI_MGA_Opt_BIOSEnable              EQU 40000000h;0x40000000
    PCI_MGA_Opt_LittleEndian            EQU 00000000h;0x00000000
    PCI_MGA_Opt_BigEndian               EQU 80000000h;0x80000000


;PCI_MGA_Option2
    PCI_MGA_Opt2_MemReadClkDelay_MASK   EQU 0000000fh;0x0000000f

;
; 3 3 2 2 2 2 22 2222 1111 1 1 11 11
; 1 0 9 8 7 6 54 3210 9876 5 4 32 1098 7654 3 2 10
;        |      |    |    |      |    |    |
; 8 4 2 1 8 4 21 8421 8421 8 4 21 8421 8421 8 4 21
; 0 0 0 x x 0 xx xxxx xxxx 0 0 xx xxxx xxxx 0 x 00
; | | |     |              | |              |    |
; | | |     |              | |              |    +--- pwidth
; | | |     |              | |              |
; | | |     |              | |              +-------- zwidth
; | | |     |              | |
; | | |     |              | +----------------------- jedecrst
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
    MGA_MA_MA_MemSeqRegReset8Refresh    EQU 00000000h;0x00000000
    MGA_MA_MA_MemSeq8RefresRegRest      EQU 00004000h;0x00004000
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

;
; 3322 22 22 22 2 2 1 111 111 1 11 
; 1098 76 54 32 1 0 9 876 543 2 10 98 76 54 32 10 
;     |     |      |     |     |     |     |     
; 8421 84 21 84 2 1 8 421 842 1 84 21 84 21 84 21
; xxxx xx 00 xx 0 x x 000 000 x 00 00 xx 00 xx 00
;          |    |       |   |    |  |     |     |
;          |    |       |   |    |  |     |     +--- casltncy
;          |    |       |   |    |  |     |
;          |    |       |   |    |  |     +--------- rrddelay
;          |    |       |   |    |  |
;          |    |       |   |    |  +--------------- rcdelay
;          |    |       |   |    |
;          |    |       |   |    +------------------ bwcdelay
;          |    |       |   |
;          |    |       |   +----------------------- bpldelay
;          |    |       |
;          |    |       +--------------------------- rasmin
;          |    |
;          |    +----------------------------------- rddelay
;          |
;          +---------------------------------------- rpdelay
;

;MGA_MemAddr_MemoryControlWaitState
    MGA_MA_MCWS_CASLatency2Cycles       EQU 00000000h;0x00000000
    MGA_MA_MCWS_CASLatency3Cycles       EQU 00000001h;0x00000001
    MGA_MA_MCWS_CASLatency4Cycles       EQU 00000002h;0x00000002
    MGA_MA_MCWS_CASLatency_MASK         EQU 00000003h;0x00000003
    MGA_MA_MCWS_RAS2RASDelay1Cycle      EQU 00000000h;0x00000000
    MGA_MA_MCWS_RAS2RASDelay2Cycles     EQU 00000010h;0x00000010
    MGA_MA_MCWS_RAS2RASDelay3Cycles     EQU 00000020h;0x00000020
    MGA_MA_MCWS_RAS2RASDelay4Cycles     EQU 00000030h;0x00000020
    MGA_MA_MCWS_RAS2RASDelay_MASK       EQU 00000030h;0x00000030
    MGA_MA_MCWS_RAS2CASDelay2Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RAS2CASDelay3Cycles     EQU 00000100h;0x00000100
    MGA_MA_MCWS_RAS2CASDelay4Cycles     EQU 00000200h;0x00000200
    MGA_MA_MCWS_RAS2CASDelay_MASK       EQU 00000300h;0x00000300
    MGA_MA_MCWS_BlockWriteDelay1Cycle   EQU 00000000h;0x00000000
    MGA_MA_MCWS_BlockWriteDelay2Cycles  EQU 00000400h;0x00000400
    MGA_MA_MCWS_BlockWriteDelay3Cycles  EQU 00000800h;0x00000800
    MGA_MA_MCWS_BlockWriteDelay_MASK    EQU 00000c00h;0x00000c00
    MGA_MA_MCWS_BW2PrechrgDelay1Cycle   EQU 00000000h;0x00000000
    MGA_MA_MCWS_BW2PrechrgDelay2Cycles  EQU 00002000h;0x00002000
    MGA_MA_MCWS_BW2PrechrgDelay3Cycles  EQU 00004000h;0x00004000
    MGA_MA_MCWS_BW2PrechrgDelay4Cycles  EQU 00006000h;0x00006000
    MGA_MA_MCWS_BW2PrechrgDelay5Cycles  EQU 00008000h;0x00008000
    MGA_MA_MCWS_BW2PrechrgDelay_MASK    EQU 0000e000h;0x0000e000
    MGA_MA_MCWS_RASMinActive3Cycles     EQU 00000000h;0x00000000
    MGA_MA_MCWS_RASMinActive4Cycles     EQU 00010000h;0x00010000
    MGA_MA_MCWS_RASMinActive5Cycles     EQU 00020000h;0x00020000
    MGA_MA_MCWS_RASMinActive6Cycles     EQU 00030000h;0x00030000
    MGA_MA_MCWS_RASMinActive7Cycles     EQU 00040000h;0x00040000
    MGA_MA_MCWS_RASMinActive8Cycles     EQU 00050000h;0x00050000
    MGA_MA_MCWS_RASMinActive_MASK       EQU 00070000h;0x00070000
    MGA_MA_MCWS_Rd2PrechrgDelNCycles    EQU 00000000h;0x00000000
    MGA_MA_MCWS_Rd2PrechrgDelNpCLm2Cyc  EQU 00200000h;0x00200000
    MGA_MA_MCWS_Rd2PrechrgDelay_MASK    EQU 00200000h;0x00200000
    MGA_MA_MCWS_RowPrechrgDelay2Cycles  EQU 00000000h;0x00000000
    MGA_MA_MCWS_RowPrechrgDelay3Cycles  EQU 01000000h;0x01000000
    MGA_MA_MCWS_RowPrechrgDelay4Cycles  EQU 02000000h;0x02000000
    MGA_MA_MCWS_RowPrechrgDelay5Cycles  EQU 03000000h;0x03000000
    MGA_MA_MCWS_RowPrechrgDelay_MASK    EQU 03000000h;0x03000000


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