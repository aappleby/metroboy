; Tests immediate instructions

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $36,0,0 ; LD   (HL),$00
     .byte $06,0,0 ; LD   B,$00
     .byte $0E,0,0 ; LD   C,$00
     .byte $16,0,0 ; LD   D,$00
     .byte $1E,0,0 ; LD   E,$00
     .byte $26,0,0 ; LD   H,$00
     .byte $2E,0,0 ; LD   L,$00
     .byte $3E,0,0 ; LD   A,$00
     .byte $F6,0,0 ; OR   $00
     .byte $FE,0,0 ; CP   $00
     .byte $C6,0,0 ; ADD  $00
     .byte $CE,0,0 ; ADC  $00
     .byte $D6,0,0 ; SUB  $00
     .byte $DE,0,0 ; SBC  $00
     .byte $E6,0,0 ; AND  $00
     .byte $EE,0,0 ; XOR  $00
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
     ; Go through each value for A
     ld   hl,values
a_loop:
     ld   b,(hl)
     push hl
     
     ; Go through each value for immediate data
     ld   hl,values
values_loop:
     push bc
     push hl
     
     ; Set registers
     push bc
     ld   a,(hl)
     ld   (instr+1),a
     ld   bc,$1234
     ld   de,$5678
     ld   hl,rp_temp
     pop  af
     
;    call print_regs
     jp   instr
instr_done:
     
     ; Checksum registers and (hl)
     call checksum_af_bc_de_hl
     ld   a,(rp_temp)
     call update_crc_fast
     
     pop  hl
     pop  bc
     inc  hl
     ld   a,l
     cp   <values_end
     jr   nz,values_loop
     
     pop  hl
     inc  hl
     ld   a,l
     cp   <values_end
     jr   nz,a_loop
     
     ret

values:
     .byte $00,$01,$0F,$10,$1F,$7F,$80,$F0,$FF
values_end:

checksums:
     .byte $7F,$7F,$05,$B7,$85,$82,$94,$B6,$D8,$0A,$D6,$F5,$44,$8C,$37,$2A,$FB,$46,$05,$FA,$BD,$2F,$9E,$C1,$5A,$56,$2A,$DA,$D0,$EE,$14,$BA,$EA,$42,$36,$D2,$87,$28,$AB,$30,$4D,$A2,$63,$C6,$34,$4E,$55,$08,$9B,$1C,$97,$0E,$49,$F8,$73,$D4,$86,$C7,$DC,$C6,$03,$BF,$43,$21,
