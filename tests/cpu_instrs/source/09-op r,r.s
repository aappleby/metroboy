; Tests most register instructions.
; Takes 10 seconds.

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $00,0,0 ; NOP
     .byte $2F,0,0 ; CPL
     .byte $37,0,0 ; SCF
     .byte $3F,0,0 ; CCF
     
     .byte $B0,0,0 ; OR   B
     .byte $B1,0,0 ; OR   C
     .byte $B2,0,0 ; OR   D
     .byte $B3,0,0 ; OR   E
     .byte $B4,0,0 ; OR   H
     .byte $B5,0,0 ; OR   L
     .byte $B7,0,0 ; OR   A
     
     .byte $B8,0,0 ; CP   B
     .byte $B9,0,0 ; CP   C
     .byte $BA,0,0 ; CP   D
     .byte $BB,0,0 ; CP   E
     .byte $BC,0,0 ; CP   H
     .byte $BD,0,0 ; CP   L
     .byte $BF,0,0 ; CP   A
     
     .byte $80,0,0 ; ADD  B
     .byte $81,0,0 ; ADD  C
     .byte $82,0,0 ; ADD  D
     .byte $83,0,0 ; ADD  E
     .byte $84,0,0 ; ADD  H
     .byte $85,0,0 ; ADD  L
     .byte $87,0,0 ; ADD  A
     
     .byte $88,0,0 ; ADC  B
     .byte $89,0,0 ; ADC  C
     .byte $8A,0,0 ; ADC  D
     .byte $8B,0,0 ; ADC  E
     .byte $8C,0,0 ; ADC  H
     .byte $8D,0,0 ; ADC  L
     .byte $8F,0,0 ; ADC  A
     
     .byte $90,0,0 ; SUB  B
     .byte $91,0,0 ; SUB  C
     .byte $92,0,0 ; SUB  D
     .byte $93,0,0 ; SUB  E
     .byte $94,0,0 ; SUB  H
     .byte $95,0,0 ; SUB  L
     .byte $97,0,0 ; SUB  A
     
     .byte $98,0,0 ; SBC  B
     .byte $99,0,0 ; SBC  C
     .byte $9A,0,0 ; SBC  D
     .byte $9B,0,0 ; SBC  E
     .byte $9C,0,0 ; SBC  H
     .byte $9D,0,0 ; SBC  L
     .byte $9F,0,0 ; SBC  A
     
     .byte $A0,0,0 ; AND  B
     .byte $A1,0,0 ; AND  C
     .byte $A2,0,0 ; AND  D
     .byte $A3,0,0 ; AND  E
     .byte $A4,0,0 ; AND  H
     .byte $A5,0,0 ; AND  L
     .byte $A7,0,0 ; AND  A
     
     .byte $A8,0,0 ; XOR  B
     .byte $A9,0,0 ; XOR  C
     .byte $AA,0,0 ; XOR  D
     .byte $AB,0,0 ; XOR  E
     .byte $AC,0,0 ; XOR  H
     .byte $AD,0,0 ; XOR  L
     .byte $AF,0,0 ; XOR  A
     
     .byte $05,0,0 ; DEC  B
     .byte $0D,0,0 ; DEC  C
     .byte $15,0,0 ; DEC  D
     .byte $1D,0,0 ; DEC  E
     .byte $25,0,0 ; DEC  H
     .byte $2D,0,0 ; DEC  L
     .byte $3D,0,0 ; DEC  A
     
     .byte $04,0,0 ; INC  B
     .byte $0C,0,0 ; INC  C
     .byte $14,0,0 ; INC  D
     .byte $1C,0,0 ; INC  E
     .byte $24,0,0 ; INC  H
     .byte $2C,0,0 ; INC  L
     .byte $3C,0,0 ; INC  A
     
     .byte $07,0,0 ; RLCA
     .byte $17,0,0 ; RLA
     .byte $0F,0,0 ; RRCA
     .byte $1F,0,0 ; RRA
     
     .byte $CB,$00,0 ; RLC  B
     .byte $CB,$01,0 ; RLC  C
     .byte $CB,$02,0 ; RLC  D
     .byte $CB,$03,0 ; RLC  E
     .byte $CB,$04,0 ; RLC  H
     .byte $CB,$05,0 ; RLC  L
     .byte $CB,$07,0 ; RLC  A
     
     .byte $CB,$08,0 ; RRC  B
     .byte $CB,$09,0 ; RRC  C
     .byte $CB,$0A,0 ; RRC  D
     .byte $CB,$0B,0 ; RRC  E
     .byte $CB,$0C,0 ; RRC  H
     .byte $CB,$0D,0 ; RRC  L
     .byte $CB,$0F,0 ; RRC  A
     
     .byte $CB,$10,0 ; RL   B
     .byte $CB,$11,0 ; RL   C
     .byte $CB,$12,0 ; RL   D
     .byte $CB,$13,0 ; RL   E
     .byte $CB,$14,0 ; RL   H
     .byte $CB,$15,0 ; RL   L
     .byte $CB,$17,0 ; RL   A
     
     .byte $CB,$18,0 ; RR   B
     .byte $CB,$19,0 ; RR   C
     .byte $CB,$1A,0 ; RR   D
     .byte $CB,$1B,0 ; RR   E
     .byte $CB,$1C,0 ; RR   H
     .byte $CB,$1D,0 ; RR   L
     .byte $CB,$1F,0 ; RR   A
     
     .byte $CB,$20,0 ; SLA  B
     .byte $CB,$21,0 ; SLA  C
     .byte $CB,$22,0 ; SLA  D
     .byte $CB,$23,0 ; SLA  E
     .byte $CB,$24,0 ; SLA  H
     .byte $CB,$25,0 ; SLA  L
     .byte $CB,$27,0 ; SLA  A
     
     .byte $CB,$28,0 ; SRA  B
     .byte $CB,$29,0 ; SRA  C
     .byte $CB,$2A,0 ; SRA  D
     .byte $CB,$2B,0 ; SRA  E
     .byte $CB,$2C,0 ; SRA  H
     .byte $CB,$2D,0 ; SRA  L
     .byte $CB,$2F,0 ; SRA  A
     
     .byte $CB,$30,0 ; SWAP B
     .byte $CB,$31,0 ; SWAP C
     .byte $CB,$32,0 ; SWAP D
     .byte $CB,$33,0 ; SWAP E
     .byte $CB,$34,0 ; SWAP H
     .byte $CB,$35,0 ; SWAP L
     .byte $CB,$37,0 ; SWAP A
     
     .byte $CB,$38,0 ; SRL  B
     .byte $CB,$39,0 ; SRL  C
     .byte $CB,$3A,0 ; SRL  D
     .byte $CB,$3B,0 ; SRL  E
     .byte $CB,$3C,0 ; SRL  H
     .byte $CB,$3D,0 ; SRL  L
     .byte $CB,$3F,0 ; SRL  A
instrs_end:    

test_instr:
     ld   c,$00
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
     
     ; Go through each value for other registers
     ld   hl,values
values_loop:
     push bc
     push hl
     
     push bc
     
     ; BC
     ld   a,(hl+)
     ld   b,a
     ld   a,(hl+)
     ld   c,a
     
     ; HL
     ld   a,(hl+)
     ld   d,a
     ld   a,(hl+)
     ld   e,a
     push de
     
     ; DE
     ld   a,(hl+)
     ld   d,a
     ld   a,(hl+)
     ld   e,a
     
     pop  hl
     pop  af
     
;    call print_regs
     jp   instr
instr_done:
     
     ; Checksum registers
     call checksum_af_bc_de_hl
     
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
     .byte $00,$01,$0F,$10,$1F,$7F,$80,$F0,$FF
     
checksums:
     .byte $7C,$55,$BD,$05,$BA,$C7,$AC,$D1,$74,$6D,$82,$4A,$0F,$06,$2A,$C5
     .byte $FA,$97,$9B,$9D,$C3,$32,$A0,$78,$00,$C1,$9F,$69,$C0,$D1,$C2,$A1
     .byte $55,$0D,$3F,$C8,$09,$7D,$97,$92,$CE,$66,$30,$56,$95,$F3,$01,$A1
     .byte $5B,$97,$54,$4C,$56,$FC,$A0,$89,$42,$F8,$7B,$2A,$E6,$7C,$03,$40
     .byte $45,$60,$C5,$A8,$B7,$BF,$B0,$EF,$A0,$7A,$1B,$4F,$FB,$22,$B4,$33
     .byte $06,$3D,$B5,$C7,$3C,$A4,$D5,$23,$C1,$BE,$75,$8B,$E0,$9B,$98,$BB
     .byte $0E,$75,$D9,$E6,$82,$A7,$E2,$66,$CD,$78,$4F,$E8,$8E,$D4,$2D,$3E
     .byte $88,$5C,$58,$C7,$F9,$20,$5F,$B9,$A8,$E4,$CA,$5E,$C8,$DB,$88,$94
     .byte $A3,$0D,$87,$60,$8B,$BA,$2B,$27,$41,$88,$83,$B1,$0A,$41,$9E,$D6
     .byte $98,$8D,$19,$B7,$13,$C6,$D5,$BF,$83,$CE,$74,$9F,$00,$34,$07,$5E
     .byte $F0,$E1,$1A,$68,$8F,$BA,$85,$A7,$A0,$46,$06,$A5,$75,$F9,$83,$48
     .byte $12,$EF,$1B,$03,$C8,$FB,$79,$EA,$9B,$00,$6C,$A9,$0D,$5E,$CB,$57
     .byte $41,$1B,$4B,$0C,$B2,$08,$D8,$E3,$43,$07,$E1,$93,$34,$73,$23,$C9
     .byte $18,$2F,$38,$F9,$D1,$3B,$AB,$5A,$BF,$C6,$F8,$03,$50,$0C,$A4,$32
     .byte $6B,$06,$7E,$FE,$ED,$8B,$D4,$15,$29,$46,$6D,$24,$6E,$5B,$15,$1A
     .byte $32,$AE,$87,$B0,$DC,$20,$AC,$4B,$2B,$63,$60,$C7,$C1,$92,$75,$AA
     .byte $6F,$CA,$17,$53,$5A,$C5,$78,$EA,$61,$01,$10,$83,$DD,$08,$D8,$78
     .byte $CA,$0B,$F5,$1F,$92,$55,$08,$01,$7F,$EA,$CD,$9B,$2A,$AA,$73,$17
     .byte $E0,$9F,$D0,$BA,$E7,$73,$72,$3D,$B7,$95,$2F,$3B,$A7,$78,$50,$36
     .byte $81,$04,$5B,$9E,$9A,$DE,$A4,$DD,$21,$B2,$9B,$36,$9F,$D7,$C8,$32
     .byte $48,$0E,$FC,$E5,$55,$C3,$53,$75,$A4,$ED,$A9,$E0,$9E,$78,$A7,$1D
     .byte $B8,$F4,$7C,$D6,$90,$2A,$03,$87,$81,$D8,$D5,$90,$63,$02,$C4,$52
     .byte $C2,$BE,$85,$B3,$32,$9A,$9E,$2D,$E3,$FB,$22,$47,$8E,$65,$08,$73
     .byte $72,$5A,$73,$95,$ED,$EC,$59,$9D,$C8,$67,$68,$F1,$4B,$ED,$41,$D5
     .byte $68,$39,$75,$F3,$FC,$09,$EF,$0D,$20,$2B,$43,$A3,$69,$AA,$89,$4F
     .byte $84,$87,$7B,$58,$42,$0A,$56,$EF,$1B,$0E,$19,$CA,$6F,$1B,$F9,$17
     .byte $EA,$B6,$4C,$B2,$1A,$C4,$C0,$B1,$E2,$B2,$45,$4E,$91,$0A,$8D,$AE
     .byte $17,$31,$55,$A3,$1B,$69,$72,$D8,$03,$E9,$55,$8D,$87,$27,$36,$63
     .byte $E6,$85,$12,$D1,$F2,$32,$97,$4D,$B5,$FA,$08,$A9,$97,$2A,$5A,$C2
     .byte $FD,$2D,$A4,$27,$57,$7C,$EC,$BD,$CC,$67,$19,$21,$46,$D4,$CD,$D6
     .byte $CB,$55,$D4,$E2,$9E,$F3,$32,$2E,$AA,$F8,$BB,$B3,$F6,$3A,$CC,$08
     .byte $64,$8B,$C2,$5F,$58,$66,$AF,$67,$B3,$44,$2C,$66,$72,$E7,$3B,$3F
     .byte $5B,$87,$0C,$17,$58,$E2,$B4,$A0,$70,$18,$81,$E6,$42,$56,$12,$CE
     .byte $BB,$13,$46,$3C,$BE,$5A,$FB,$53
