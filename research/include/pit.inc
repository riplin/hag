;Programmable Interval Timer

;Ports list: https://github.com/cirosantilli/ralf-brown-interrupt-list/blob/master/inter61d/PORTS.B

PIT_Channel0Data                        EQU 040h;0x40
PIT_Channel1Data                        EQU 041h;0x41
PIT_Channel2Data                        EQU 042h;0x42
PIT_ModeCommand                         EQU 043h;0x43
    PIT_MC_BCDBinary                    EQU 001h;0x01
    PIT_MC_OpMode_IntrOnTerminal        EQU 000h;0x00
    PIT_MC_OpMode_HWRetrigOneShot       EQU 002h;0x02
    PIT_MC_OpMode_RateGenerator         EQU 004h;0x04
    PIT_MC_OpMode_SquareWaveGenerator   EQU 006h;0x06
    PIT_MC_OpMode_SWTriggeredStrobe     EQU 008h;0x08
    PIT_MC_OpMode_HWTriggeredStrobe     EQU 00ah;0x0a
    PIT_MC_OpMode_RateGenerator2        EQU 00ch;0x0c
    PIT_MC_OpMode_SquareWaveGenerator2  EQU 00eh;0x0e
    PIT_MC_AccMode_LatchCountValue      EQU 000h;0x00
    PIT_MC_AccMode_LoByteOnly           EQU 010h;0x10
    PIT_MC_AccMode_HiByteOnly           EQU 020h;0x20
    PIT_MC_AccMode_LoByteHiByte         EQU 030h;0x30
    PIT_MC_ChanSel_0                    EQU 000h;0x00
    PIT_MC_ChanSel_1                    EQU 040h;0x40
    PIT_MC_ChanSel_2                    EQU 080h;0x80
    PIT_MC_ChanSel_ReadBackCommand      EQU 0c0h;0xc0

;PORT 00EB - DUMMY PORT FOR DELAY???
;Note:  on a number of machines, the BIOS appears to write a copy of any
;       data sent to numerous other ports to this port as well; it seems
;       to be a dummy port used for short delays between writes to other
;       ports (used instead of JMP $+2, which no longer delays on Pentium+)

DUMMY_DelayEB                           EQU 0ebh;0xeb

;PORT 00ED - DUMMY PORT FOR DELAY???
;Note:  on a number of machines, the BIOS appears to write a copy of any
;       data sent to numerous other ports to this port as well; it seems
;       to be a dummy port used for short delays between writes to other
;       ports (used instead of JMP $+2, which no longer delays on Pentium+)

DUMMY_DelayED                           EQU 0edh;0xed