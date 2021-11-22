; Tests miscellaneous instructions

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $F0,$91,0   ; LDH  A,($91)
     .byte $E0,$91,0   ; LDH  ($91),A
     .byte $F2,$00,0   ; LDH  A,(C)
     .byte $E2,$00,0   ; LDH  (C),A
     .byte $FA,$91,$FF ; LD   A,($FF91)
     .byte $EA,$91,$FF ; LD   ($FF91),A
     .byte $08,$91,$FF ; LD   ($FF91),SP
     .byte $01,$23,$01 ; LD   BC,$0123
     .byte $11,$23,$01 ; LD   DE,$0123
     .byte $21,$23,$01 ; LD   HL,$0123
     .byte $31,$23,$01 ; LD   SP,$0123
     .byte $F5,0,0     ; PUSH AF
     .byte $C5,0,0     ; PUSH BC
     .byte $D5,0,0     ; PUSH DE
     .byte $E5,0,0     ; PUSH HL
     .byte $F1,0,0     ; POP  AF
     .byte $C1,0,0     ; POP  BC
     .byte $D1,0,0     ; POP  DE
     .byte $E1,0,0     ; POP  HL
instrs_end:

test_instr:
     ; C = flags register
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
     ; Fill RAM
     ld   a,$FE
     ld   ($FF90),a
     ld   a,$DC
     ld   ($FF91),a
     ld   a,$BA
     ld   ($FF92),a
     
     ; Fill stack
     ld   a,$13
     ld   ($DF80),a
     ld   a,$57
     ld   ($DF80-1),a
     ld   a,$9B
     ld   ($DF80-2),a
     ld   a,$DF
     ld   ($DF80-3),a
     
     ; Set registers
     ld   b,$12
     push bc
     ld   bc,$5691
     ld   de,$9ABC
     ld   hl,$DEF0
     pop  af
     
     ; Switch stack
     ld   (temp),sp
     ld   sp,$DF80-2
     
     jp   instr
instr_done:
     ; Save new SP and switch to another stack
     ld   (temp+2),sp
     ld   sp,$DF70
     
     call checksum_af_bc_de_hl
     
     ; Checksum SP
     ld   a,(temp+2)
     call update_crc_fast
     ld   a,(temp+3)
     call update_crc_fast
     
     ; Checksum RAM
     ld   a,($FF90)
     call update_crc_fast
     ld   a,($FF91)
     call update_crc_fast
     ld   a,($FF92)
     call update_crc_fast
     
     ; Checksum stack
     ld   a,($DF80)
     call update_crc_fast
     ld   a,($DF80-1)
     call update_crc_fast
     ld   a,($DF80-2)
     call update_crc_fast
     ld   a,($DF80-3)
     call update_crc_fast
     
     ; Restore SP
     ldsp temp
     
     ret

checksums:
     .byte $4D,$FF,$15,$97,$6D,$A7,$35,$65,$4D,$FF,$15,$97,$6D,$A7,$35,$65,$4D,$FF,$15,$97,$6D,$A7,$35,$65,$AD,$FA,$5E,$41,$D0,$78,$79,$C1,$AF,$66,$99,$34,$0D,$E1,$97,$99,$6F,$D0,$6F,$5D,$C3,$1F,$A3,$8A,$C2,$F1,$9C,$F3,$C1,$C3,$DC,$78,$C0,$2D,$E3,$01,$8F,$C4,$0F,$44,$95,$22,$6A,$39,$61,$C5,$AB,$55,$FB,$DF,$2C,$52,
