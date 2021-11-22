; Tests BC/DE/HL arithmetic

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"
     
instrs:
     .byte $0B,0,0     ; DEC  BC
     .byte $1B,0,0     ; DEC  DE
     .byte $2B,0,0     ; DEC  HL
     .byte $03,0,0     ; INC  BC
     .byte $13,0,0     ; INC  DE
     .byte $23,0,0     ; INC  HL
     .byte $09,0,0     ; ADD  HL,BC
     .byte $19,0,0     ; ADD  HL,DE
     .byte $29,0,0     ; ADD  HL,HL
instrs_end:

test_instr:
     ld   c,$00
     call test
     ld   c,$10
     call test
     ld   c,$E0
     call test
     ld   c,$F0
     call test
     ret

test:
     ; Go through each value for HL
     ld   hl,values
hl_loop:
     ld   e,(hl)
     inc  hl
     ld   d,(hl)
     inc  hl
     push hl
     
     ; Go through each value for BC, DE, A
     ld   hl,values
values_loop:
     push bc
     push de
     push hl
     
     push de
     push bc
     
     ; BC
     ld   c,(hl)
     inc  hl
     ld   b,(hl)
     inc  hl
     
     ; DE
     ld   e,(hl)
     inc  hl
     ld   d,(hl)
     inc  hl
     
     ; HL, AF
     pop  af
     ld   a,(hl)
     pop  hl
     
;    call print_regs
     jp   instr
instr_done:
     
     ; Checksum registers
     call checksum_af_bc_de_hl
     
     pop  hl
     pop  de
     pop  bc
     inc  hl
     inc  hl
     ld   a,l
     cp   <values_end
     jr   nz,values_loop
     
     pop  hl
     ld   a,l
     cp   <values_end
     jr   nz,hl_loop
     
     ret

values:
     .word $0000,$0001,$000F,$0010,$001F,$007F,$0080,$00FF
     .word $0100,$0F00,$1F00,$1000,$7FFF,$8000,$FFFF
values_end:
     .word $0000,$0001,$000F,$0010,$001F,$007F,$0080,$00FF
     .word $0100,$0F00,$1F00,$1000,$7FFF,$8000,$FFFF

checksums:
     .byte $C0,$A1,$36,$A3,$BE,$15,$B8,$2B,$9F,$93,$C6,$C2,$86,$C0,$07,$81,$0F,$75,$35,$38,$6B,$C7,$0A,$1B,$06,$68,$4B,$42,$64,$B4,$8C,$18,$FB,$6C,$31,$94,
