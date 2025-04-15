;Keyboard ports

;Ports list: https://github.com/cirosantilli/ralf-brown-interrupt-list/blob/master/inter61d/PORTS.B

KB_PortB                                EQU 061h;0x61
    KB_PBW_Timer2GateSpeakerEnable      EQU 001h;0x01
    KB_PBW_SpeakerEnable                EQU 002h;0x02
    KB_PBW_RamParityCheckDisable        EQU 004h;0x04
    KB_PBW_IOChanParityCheckDisable     EQU 008h;0x08
    KB_PBW_IRQ1Reset                    EQU 080h;0x80
    KB_PBR_Timer2ClkGateSpeakerStatus   EQU 001h;0x01
    KB_PBR_SpeakerStatus                EQU 002h;0x02
    KB_PBR_NMIParityCheckStatus         EQU 004h;0x04
    KB_PBR_NMIIOChanCheckStatus         EQU 008h;0x08
    KB_PBR_LowerNibbleMask              EQU 00fh;0x0f
    KB_PBR_ToggleRefreshRequest         EQU 010h;0x10
    KB_PBR_MirrorTimer2OutputCond       EQU 020h;0x20
    KB_PBR_IOChannelParityError         EQU 040h;0x40
    KB_PBR_RAMParityError               EQU 080h;0x80
KB_CommandPort_XT                       EQU 062h;0x62
