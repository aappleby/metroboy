; Tests (HL/BC/DE) instructions.
; Takes 20 seconds.

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $0A,0,0     ; LD   A,(BC)
     .byte $1A,0,0     ; LD   A,(DE)
     .byte $02,0,0     ; LD   (BC),A
     .byte $12,0,0     ; LD   (DE),A
     .byte $2A,0,0     ; LD   A,(HL+)
     .byte $3A,0,0     ; LD   A,(HL-)
     .byte $22,0,0     ; LD   (HL+),A
     .byte $32,0,0     ; LD   (HL-),A
     .byte $B6,0,0     ; OR   (HL)
     .byte $BE,0,0     ; CP   (HL)
     .byte $86,0,0     ; ADD  (HL)
     .byte $8E,0,0     ; ADC  (HL)
     .byte $96,0,0     ; SUB  (HL)
     .byte $9E,0,0     ; SBC  (HL)
     .byte $A6,0,0     ; AND  (HL)
     .byte $AE,0,0     ; XOR  (HL)
     .byte $35,0,0     ; DEC  (HL)
     .byte $34,0,0     ; INC  (HL)
     .byte $CB,$06,0   ; RLC  (HL)
     .byte $CB,$0E,0   ; RRC  (HL)
     .byte $CB,$16,0   ; RL   (HL)
     .byte $CB,$1E,0   ; RR   (HL)
     .byte $CB,$26,0   ; SLA  (HL)
     .byte $CB,$2E,0   ; SRA  (HL)
     .byte $CB,$36,0   ; SWAP (HL)
     .byte $CB,$3E,0   ; SRL  (HL)
     .byte $CB,$46,0   ; BIT  0,(HL)
     .byte $CB,$4E,0   ; BIT  1,(HL)
     .byte $CB,$56,0   ; BIT  2,(HL)
     .byte $CB,$5E,0   ; BIT  3,(HL)
     .byte $CB,$66,0   ; BIT  4,(HL)
     .byte $CB,$6E,0   ; BIT  5,(HL)
     .byte $CB,$76,0   ; BIT  6,(HL)
     .byte $CB,$7E,0   ; BIT  7,(HL)
     .byte $CB,$86,0   ; RES  0,(HL)
     .byte $CB,$8E,0   ; RES  1,(HL)
     .byte $CB,$96,0   ; RES  2,(HL)
     .byte $CB,$9E,0   ; RES  3,(HL)
     .byte $CB,$A6,0   ; RES  4,(HL)
     .byte $CB,$AE,0   ; RES  5,(HL)
     .byte $CB,$B6,0   ; RES  6,(HL)
     .byte $CB,$BE,0   ; RES  7,(HL)
     .byte $CB,$C6,0   ; SET  0,(HL)
     .byte $CB,$CE,0   ; SET  1,(HL)
     .byte $CB,$D6,0   ; SET  2,(HL)
     .byte $CB,$DE,0   ; SET  3,(HL)
     .byte $CB,$E6,0   ; SET  4,(HL)
     .byte $CB,$EE,0   ; SET  5,(HL)
     .byte $CB,$F6,0   ; SET  6,(HL)
     .byte $CB,$FE,0   ; SET  7,(HL)
     .byte $27,0,0     ; DAA
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
     
     ; Go through each value for (HL)
     ld   hl,values
values_loop:
     push bc
     push hl
     
     push bc
     
     ; BC
     ld   a,(hl+)
     ld   bc,rp_temp
     ld   (rp_temp),a
     
     ; DE
     ld   a,(hl+)
     ld   de,rp_temp+1
     ld   (rp_temp+1),a
     
     ; HL
     ld   a,(hl)
     ld   hl,rp_temp+2
     ld   (rp_temp+2),a
     
     ; AF
     pop  af
     
;    call print_regs
     jp   instr
instr_done:
     
     ; Checksum AF, HL, and (HL)
     push hl
     push af
     call update_crc_fast
     pop  hl
     ld   a,l
     call update_crc_fast
     pop  bc
     ld   a,b
     call update_crc_fast
     ld   a,c
     call update_crc_fast
     ld   a,(rp_temp)
     call update_crc_fast
     ld   a,(rp_temp+1)
     call update_crc_fast
     ld   a,(rp_temp+2)
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
     .byte $00,$01,$0F,$10,$1F,$7F,$80,$F0,$FF,$02,$04,$08,$20,$40
values_end:
     .byte $00,$01,$0F,$10,$1F,$7F,$80,$F0,$FF,$02,$04,$08,$20,$40

checksums:
     .byte $E0,$E5,$09,$A7,$FB,$28,$0D,$AE,$AC,$BB,$91,$D8,$B3,$E2,$AF,$C4
     .byte $3D,$B5,$02,$07,$4F,$6E,$5B,$7E,$AE,$02,$E7,$14,$DC,$D9,$BE,$6D
     .byte $F1,$48,$A9,$42,$67,$08,$FE,$57,$06,$6A,$A9,$B1,$FD,$A5,$84,$F0
     .byte $82,$FC,$24,$A9,$A8,$1D,$BB,$E2,$F8,$23,$8C,$DE,$0E,$1D,$64,$D1
     .byte $05,$E0,$24,$41,$53,$75,$47,$55,$F4,$D9,$10,$6A,$38,$16,$28,$D8
     .byte $D1,$28,$A3,$E0,$A2,$05,$B8,$FE,$B0,$F4,$F5,$8F,$4B,$39,$03,$B0
     .byte $8A,$07,$BA,$90,$25,$99,$A7,$78,$E6,$9A,$D1,$49,$C9,$B2,$A3,$E5
     .byte $36,$34,$CB,$5A,$97,$42,$71,$09,$39,$87,$25,$EC,$54,$EE,$C5,$B3
     .byte $FC,$B5,$6F,$BD,$0B,$D8,$46,$6F,$6A,$27,$81,$9F,$F8,$38,$E2,$71
     .byte $55,$19,$21,$83,$4B,$85,$9F,$4B,$A1,$78,$14,$60,$58,$08,$D9,$57
     .byte $11,$8C,$83,$9A,$9F,$01,$D1,$90,$E8,$82,$0B,$5A,$BD,$75,$86,$21
     .byte $DF,$83,$E9,$23,$1E,$B6,$7F,$D1,$4A,$18,$A5,$8E,$CF,$CF,$CA,$51
     .byte $3F,$03,$A4,$96,$C3,$1F,$9E,$88,$0C,$DF,$1F,$B1
