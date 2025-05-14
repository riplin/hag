;ATI Rage 128 XL BIOS partial disassembly

;Resources:
;
; DOS video card tests: https://gona.mactar.hu/DOS_TESTS/
;
;Interrupt list: http://www.ctyme.com/intr/int.htm
;VGADOC: https://pdos.csail.mit.edu/6.828/2018/readings/hardware/vgadoc/S3.TXT
;VGA general info: https://wiki.osdev.org/VGA_Hardware
;VGA general info: http://www.osdever.net/FreeVGA/vga/vga.htm
;VGA general info: https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/video/Second%20Sight%20VGA%20Registers.pdf
;Ports list: https://github.com/cirosantilli/ralf-brown-interrupt-list/blob/master/inter61d/PORTS.B
;Memory map: https://wiki.osdev.org/Memory_Map_(x86)
;BIOS Data Area: https://stanislavs.org/helppc/bios_data_area.html
;BIOS Data Area: http://www.bioscentral.com/misc/bda.htm
;Video Information tables: https://stanislavs.org/helppc/video_information.html
;PCI: https://wiki.osdev.org/PCI
;
;Hex editor: https://hexed.it/ https://web.imhex.werwolv.net/
;Disassembler: https://shell-storm.org/online/Online-Assembler-and-Disassembler/ (allowed me to disassemble specific sections manually)
;
;I use MASM 6.11 (DOS)
;Build with MASM: ml /AT r128xla.asm
;Produces a .COM file. Just rename to .BIN

.MODEL tiny
.386
.CODE
ORG 0000h

;PCI vendor and device id.
VENDOR_ID               EQU 1002h
DEVICE_ID               EQU 474Dh

ROMStart:
ROMMagic                DB 55h, 0AAh
ROMSize                 DB (ROMEnd - ROMStart)/512

    jmp short entry
                        DB 5Ch, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h                    ;Offset 0x05
                        DB 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h                                   ;Offset 0x10

                        DW PCIHeader                                                                ;Offset 0x18
                        DW 0000h               ;PnP header pointer
                        DW 0000h               ;Unknown

                        DB 'IBM', 00h          ;Offset 0x1E
                        DB 04h, 7Dh, 00h, 2Ah, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h     ;Offset 0x22







ORG 80h
entry:




ORG 16ch
PCIHeader:
                        DB 'PCIR'                   ; PCI header magic
                        DW VENDOR_ID                ; Vendor ID
                        DW DEVICE_ID                ; Device ID
                        DW 0000h                    ; Pointer to vital product data (0 = none)
                        DW 0018h                    ; PCI data structure length
                        DB 00h                      ; PCI data structure revision
                        DB 00h, 00h, 03h            ; PCI device class code
                        DW 0048h                    ; Rom size in 512 byte blocks
                        DW 041Dh                    ; Revision level of code
                        DB 00h                      ; Code type (0 = x86)
                        DB 80h                      ; Last image indicator
                        DW 0000h                    ; Reserved




ORG 7fffh
DB 8Bh
ROMEnd:
END