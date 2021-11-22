; Tests most register instructions.
; Takes 15 seconds.

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $CB,$40,0 ; BIT  0,B
     .byte $CB,$41,0 ; BIT  0,C
     .byte $CB,$42,0 ; BIT  0,D
     .byte $CB,$43,0 ; BIT  0,E
     .byte $CB,$44,0 ; BIT  0,H
     .byte $CB,$45,0 ; BIT  0,L
     .byte $CB,$47,0 ; BIT  0,A
     
     .byte $CB,$48,0 ; BIT  1,B
     .byte $CB,$49,0 ; BIT  1,C
     .byte $CB,$4A,0 ; BIT  1,D
     .byte $CB,$4B,0 ; BIT  1,E
     .byte $CB,$4C,0 ; BIT  1,H
     .byte $CB,$4D,0 ; BIT  1,L
     .byte $CB,$4F,0 ; BIT  1,A
     
     .byte $CB,$50,0 ; BIT  2,B
     .byte $CB,$51,0 ; BIT  2,C
     .byte $CB,$52,0 ; BIT  2,D
     .byte $CB,$53,0 ; BIT  2,E
     .byte $CB,$54,0 ; BIT  2,H
     .byte $CB,$55,0 ; BIT  2,L
     .byte $CB,$57,0 ; BIT  2,A
     
     .byte $CB,$58,0 ; BIT  3,B
     .byte $CB,$59,0 ; BIT  3,C
     .byte $CB,$5A,0 ; BIT  3,D
     .byte $CB,$5B,0 ; BIT  3,E
     .byte $CB,$5C,0 ; BIT  3,H
     .byte $CB,$5D,0 ; BIT  3,L
     .byte $CB,$5F,0 ; BIT  3,A
     
     .byte $CB,$60,0 ; BIT  4,B
     .byte $CB,$61,0 ; BIT  4,C
     .byte $CB,$62,0 ; BIT  4,D
     .byte $CB,$63,0 ; BIT  4,E
     .byte $CB,$64,0 ; BIT  4,H
     .byte $CB,$65,0 ; BIT  4,L
     .byte $CB,$67,0 ; BIT  4,A
     
     .byte $CB,$68,0 ; BIT  5,B
     .byte $CB,$69,0 ; BIT  5,C
     .byte $CB,$6A,0 ; BIT  5,D
     .byte $CB,$6B,0 ; BIT  5,E
     .byte $CB,$6C,0 ; BIT  5,H
     .byte $CB,$6D,0 ; BIT  5,L
     .byte $CB,$6F,0 ; BIT  5,A
     
     .byte $CB,$70,0 ; BIT  6,B
     .byte $CB,$71,0 ; BIT  6,C
     .byte $CB,$72,0 ; BIT  6,D
     .byte $CB,$73,0 ; BIT  6,E
     .byte $CB,$74,0 ; BIT  6,H
     .byte $CB,$75,0 ; BIT  6,L
     .byte $CB,$77,0 ; BIT  6,A
     
     .byte $CB,$78,0 ; BIT  7,B
     .byte $CB,$79,0 ; BIT  7,C
     .byte $CB,$7A,0 ; BIT  7,D
     .byte $CB,$7B,0 ; BIT  7,E
     .byte $CB,$7C,0 ; BIT  7,H
     .byte $CB,$7D,0 ; BIT  7,L
     .byte $CB,$7F,0 ; BIT  7,A
     
     .byte $CB,$80,0 ; RES  0,B
     .byte $CB,$81,0 ; RES  0,C
     .byte $CB,$82,0 ; RES  0,D
     .byte $CB,$83,0 ; RES  0,E
     .byte $CB,$84,0 ; RES  0,H
     .byte $CB,$85,0 ; RES  0,L
     .byte $CB,$87,0 ; RES  0,A
     
     .byte $CB,$88,0 ; RES  1,B
     .byte $CB,$89,0 ; RES  1,C
     .byte $CB,$8A,0 ; RES  1,D
     .byte $CB,$8B,0 ; RES  1,E
     .byte $CB,$8C,0 ; RES  1,H
     .byte $CB,$8D,0 ; RES  1,L
     .byte $CB,$8F,0 ; RES  1,A
     
     .byte $CB,$90,0 ; RES  2,B
     .byte $CB,$91,0 ; RES  2,C
     .byte $CB,$92,0 ; RES  2,D
     .byte $CB,$93,0 ; RES  2,E
     .byte $CB,$94,0 ; RES  2,H
     .byte $CB,$95,0 ; RES  2,L
     .byte $CB,$97,0 ; RES  2,A
     
     .byte $CB,$98,0 ; RES  3,B
     .byte $CB,$99,0 ; RES  3,C
     .byte $CB,$9A,0 ; RES  3,D
     .byte $CB,$9B,0 ; RES  3,E
     .byte $CB,$9C,0 ; RES  3,H
     .byte $CB,$9D,0 ; RES  3,L
     .byte $CB,$9F,0 ; RES  3,A
     
     .byte $CB,$A0,0 ; RES  4,B
     .byte $CB,$A1,0 ; RES  4,C
     .byte $CB,$A2,0 ; RES  4,D
     .byte $CB,$A3,0 ; RES  4,E
     .byte $CB,$A4,0 ; RES  4,H
     .byte $CB,$A5,0 ; RES  4,L
     .byte $CB,$A7,0 ; RES  4,A
     
     .byte $CB,$A8,0 ; RES  5,B
     .byte $CB,$A9,0 ; RES  5,C
     .byte $CB,$AA,0 ; RES  5,D
     .byte $CB,$AB,0 ; RES  5,E
     .byte $CB,$AC,0 ; RES  5,H
     .byte $CB,$AD,0 ; RES  5,L
     .byte $CB,$AF,0 ; RES  5,A
     
     .byte $CB,$B0,0 ; RES  6,B
     .byte $CB,$B1,0 ; RES  6,C
     .byte $CB,$B2,0 ; RES  6,D
     .byte $CB,$B3,0 ; RES  6,E
     .byte $CB,$B4,0 ; RES  6,H
     .byte $CB,$B5,0 ; RES  6,L
     .byte $CB,$B7,0 ; RES  6,A
     
     .byte $CB,$B8,0 ; RES  7,B
     .byte $CB,$B9,0 ; RES  7,C
     .byte $CB,$BA,0 ; RES  7,D
     .byte $CB,$BB,0 ; RES  7,E
     .byte $CB,$BC,0 ; RES  7,H
     .byte $CB,$BD,0 ; RES  7,L
     .byte $CB,$BF,0 ; RES  7,A
     
     .byte $CB,$C0,0 ; SET  0,B
     .byte $CB,$C1,0 ; SET  0,C
     .byte $CB,$C2,0 ; SET  0,D
     .byte $CB,$C3,0 ; SET  0,E
     .byte $CB,$C4,0 ; SET  0,H
     .byte $CB,$C5,0 ; SET  0,L
     .byte $CB,$C7,0 ; SET  0,A
     
     .byte $CB,$C8,0 ; SET  1,B
     .byte $CB,$C9,0 ; SET  1,C
     .byte $CB,$CA,0 ; SET  1,D
     .byte $CB,$CB,0 ; SET  1,E
     .byte $CB,$CC,0 ; SET  1,H
     .byte $CB,$CD,0 ; SET  1,L
     .byte $CB,$CF,0 ; SET  1,A
     
     .byte $CB,$D0,0 ; SET  2,B
     .byte $CB,$D1,0 ; SET  2,C
     .byte $CB,$D2,0 ; SET  2,D
     .byte $CB,$D3,0 ; SET  2,E
     .byte $CB,$D4,0 ; SET  2,H
     .byte $CB,$D5,0 ; SET  2,L
     .byte $CB,$D7,0 ; SET  2,A
     
     .byte $CB,$D8,0 ; SET  3,B
     .byte $CB,$D9,0 ; SET  3,C
     .byte $CB,$DA,0 ; SET  3,D
     .byte $CB,$DB,0 ; SET  3,E
     .byte $CB,$DC,0 ; SET  3,H
     .byte $CB,$DD,0 ; SET  3,L
     .byte $CB,$DF,0 ; SET  3,A
     
     .byte $CB,$E0,0 ; SET  4,B
     .byte $CB,$E1,0 ; SET  4,C
     .byte $CB,$E2,0 ; SET  4,D
     .byte $CB,$E3,0 ; SET  4,E
     .byte $CB,$E4,0 ; SET  4,H
     .byte $CB,$E5,0 ; SET  4,L
     .byte $CB,$E7,0 ; SET  4,A
     
     .byte $CB,$E8,0 ; SET  5,B
     .byte $CB,$E9,0 ; SET  5,C
     .byte $CB,$EA,0 ; SET  5,D
     .byte $CB,$EB,0 ; SET  5,E
     .byte $CB,$EC,0 ; SET  5,H
     .byte $CB,$ED,0 ; SET  5,L
     .byte $CB,$EF,0 ; SET  5,A
     
     .byte $CB,$F0,0 ; SET  6,B
     .byte $CB,$F1,0 ; SET  6,C
     .byte $CB,$F2,0 ; SET  6,D
     .byte $CB,$F3,0 ; SET  6,E
     .byte $CB,$F4,0 ; SET  6,H
     .byte $CB,$F5,0 ; SET  6,L
     .byte $CB,$F7,0 ; SET  6,A
     
     .byte $CB,$F8,0 ; SET  7,B
     .byte $CB,$F9,0 ; SET  7,C
     .byte $CB,$FA,0 ; SET  7,D
     .byte $CB,$FB,0 ; SET  7,E
     .byte $CB,$FC,0 ; SET  7,H
     .byte $CB,$FD,0 ; SET  7,L
     .byte $CB,$FF,0 ; SET  7,A
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
     .byte $00,$01,$02,$04,$08,$10,$20,$40,$80,$FF
values_end:
     .byte $00,$01,$02,$04,$08,$10,$20,$40,$80,$FF
     
checksums:
     .byte $46,$51,$4A,$16,$D4,$18,$B2,$4E,$ED,$B5,$15,$EA,$74,$66,$66,$3E
     .byte $C2,$F3,$7F,$6A,$63,$CA,$62,$21,$72,$1E,$E4,$83,$6A,$56,$41,$1D
     .byte $91,$90,$DB,$38,$54,$0A,$6C,$24,$02,$9E,$EA,$5B,$6D,$A7,$CB,$80
     .byte $B4,$0B,$F3,$0F,$40,$38,$75,$BB,$AF,$30,$2B,$E5,$BD,$97,$D0,$33
     .byte $83,$CB,$FD,$0A,$BB,$21,$93,$95,$28,$2F,$A2,$F6,$1B,$5F,$47,$E5
     .byte $A3,$2E,$39,$63,$6C,$E0,$02,$BB,$78,$F1,$BA,$CB,$2C,$9F,$49,$E0
     .byte $6C,$E0,$02,$BB,$04,$28,$A9,$FD,$5E,$D7,$2E,$93,$1B,$78,$08,$00
     .byte $83,$CB,$FD,$0A,$BB,$21,$93,$95,$69,$17,$20,$96,$C3,$B4,$B6,$51
     .byte $C1,$4E,$C3,$05,$72,$D0,$25,$98,$44,$F0,$99,$B7,$B4,$0B,$F3,$0F
     .byte $54,$0A,$6C,$24,$45,$10,$2B,$9D,$86,$3C,$DF,$27,$02,$9E,$EA,$5B
     .byte $B7,$B6,$4F,$60,$70,$E0,$E1,$AA,$C2,$F3,$7F,$6A,$63,$CA,$62,$21
     .byte $80,$76,$41,$65,$AA,$3B,$D4,$2C,$ED,$B5,$15,$EA,$74,$66,$66,$3E
     .byte $AD,$FF,$A0,$43,$7B,$4C,$06,$A4,$15,$32,$EE,$44,$43,$A6,$68,$3B
     .byte $6F,$5D,$BE,$D4,$DA,$75,$1B,$EF,$9B,$4D,$99,$8F,$49,$E8,$A9,$1D
     .byte $F5,$1B,$58,$3A,$92,$25,$2D,$51,$38,$5C,$62,$05,$DD,$A9,$63,$AD
     .byte $E3,$78,$2F,$37,$90,$15,$DB,$62,$58,$E2,$E8,$35,$BB,$C1,$5A,$EA
     .byte $06,$FE,$28,$AA,$4F,$5D,$64,$BF,$83,$CF,$7F,$B2,$F9,$A9,$90,$BF
     .byte $DD,$06,$B6,$64,$25,$8A,$E0,$24,$FA,$40,$95,$13,$91,$61,$93,$0D
     .byte $69,$A8,$0E,$0B,$AE,$FD,$DF,$1A,$D4,$98,$D8,$11,$61,$E9,$16,$66
     .byte $BD,$82,$1F,$2C,$E2,$74,$26,$77,$13,$E4,$6A,$25,$D7,$DE,$8A,$4F
     .byte $1F,$7B,$47,$BC,$DA,$DB,$31,$E7,$2B,$06,$2C,$39,$15,$FC,$1C,$0B
     .byte $1A,$3B,$A0,$0F,$55,$E5,$D8,$1C,$6D,$6C,$7F,$B8,$14,$AD,$9C,$AF
     .byte $92,$B6,$60,$40,$76,$E6,$6D,$2F,$9E,$CA,$45,$6D,$54,$97,$47,$35
     .byte $EE,$39,$50,$63,$47,$8C,$8A,$AB,$18,$F7,$6D,$10,$B7,$A6,$74,$0C
     .byte $11,$24,$9C,$F5,$64,$5D,$FB,$16,$65,$1C,$59,$C6,$B9,$E3,$30,$52
     .byte $1D,$E4,$B8,$9E,$A3,$2F,$7B,$6F,$03,$20,$24,$41,$4C,$F7,$22,$B8
     .byte $92,$A7,$75,$E3,$1D,$F2,$5E,$FD,$B7,$A4,$F3,$34,$BF,$F7,$37,$CA
     .byte $67,$22,$D4,$4D,$DE,$1A,$99,$58,$B2,$65,$91,$12,$F2,$8C,$65,$08
     .byte $69,$E2,$9B,$D3,$94,$8C,$71,$F1,$D8,$22,$29,$53,$E8,$6A,$D9,$55
     .byte $3E,$24,$42,$EF,$38,$12,$AC,$02,$35,$84,$7D,$2C,$C2,$34,$AC,$E2
     .byte $4B,$AA,$E0,$31,$8F,$A0,$F2,$13,$A8,$4F,$7B,$98,$02,$16,$3B,$D4
     .byte $8D,$09,$58,$A4,$FF,$46,$CA,$17,$08,$AA,$78,$02,$4A,$CF,$72,$E1
     .byte $A8,$55,$52,$89,$F8,$FD,$D6,$4E,$22,$E7,$8F,$C6,$80,$F1,$BB,$3C
     .byte $09,$1B,$4A,$4A,$06,$A1,$FD,$54,$E4,$BF,$D8,$27,$14,$23,$42,$90
     .byte $B3,$7B,$55,$14,$77,$22,$EE,$92,$E9,$37,$76,$8C,$7D,$CF,$B7,$C7
     .byte $D2,$90,$17,$48,$BB,$52,$BC,$19,$AA,$91,$9F,$DC,$0D,$AA,$C9,$24
     .byte $C8,$45,$DF,$AB,$B3,$83,$A8,$9E,$0F,$AA,$62,$2F,$C4,$C0,$28,$BA
     .byte $32,$56,$99,$69,$C9,$77,$4B,$62,$6B,$FF,$B6,$DD,$42,$46,$7A,$00
     .byte $DA,$E9,$67,$4D,$46,$9C,$B5,$92,$04,$B5,$F6,$03,$01,$3C,$A2,$47
     .byte $40,$15,$4A,$D6,$04,$39,$BC,$2F,$E9,$E1,$39,$59,$9B,$6A,$A4,$12
     .byte $97,$23,$99,$30,$9E,$A6,$70,$AD,$C7,$1B,$D6,$1F,$05,$15,$D2,$5B
     .byte $29,$0F,$5A,$CC,$0A,$99,$A2,$68,$5D,$58,$ED,$9C,$B9,$82,$CD,$74
