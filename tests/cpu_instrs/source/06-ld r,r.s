; Tests LD r,r ($40-$7F)

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $40,0,0 ; LD   B,B
     .byte $41,0,0 ; LD   B,C
     .byte $42,0,0 ; LD   B,D
     .byte $43,0,0 ; LD   B,E
     .byte $44,0,0 ; LD   B,H
     .byte $45,0,0 ; LD   B,L
     .byte $46,0,0 ; LD   B,(HL)
     .byte $47,0,0 ; LD   B,A
     
     .byte $48,0,0 ; LD   C,B
     .byte $49,0,0 ; LD   C,C
     .byte $4A,0,0 ; LD   C,D
     .byte $4B,0,0 ; LD   C,E
     .byte $4C,0,0 ; LD   C,H
     .byte $4D,0,0 ; LD   C,L
     .byte $4E,0,0 ; LD   C,(HL)
     .byte $4F,0,0 ; LD   C,A
     
     .byte $50,0,0 ; LD   D,B
     .byte $51,0,0 ; LD   D,C
     .byte $52,0,0 ; LD   D,D
     .byte $53,0,0 ; LD   D,E
     .byte $54,0,0 ; LD   D,H
     .byte $55,0,0 ; LD   D,L
     .byte $56,0,0 ; LD   D,(HL)
     .byte $57,0,0 ; LD   D,A
     
     .byte $58,0,0 ; LD   E,B
     .byte $59,0,0 ; LD   E,C
     .byte $5A,0,0 ; LD   E,D
     .byte $5B,0,0 ; LD   E,E
     .byte $5C,0,0 ; LD   E,H
     .byte $5D,0,0 ; LD   E,L
     .byte $5E,0,0 ; LD   E,(HL)
     .byte $5F,0,0 ; LD   E,A
     
     .byte $60,0,0 ; LD   H,B
     .byte $61,0,0 ; LD   H,C
     .byte $62,0,0 ; LD   H,D
     .byte $63,0,0 ; LD   H,E
     .byte $64,0,0 ; LD   H,H
     .byte $65,0,0 ; LD   H,L
     .byte $66,0,0 ; LD   H,(HL)
     .byte $67,0,0 ; LD   H,A
     
     .byte $68,0,0 ; LD   L,B
     .byte $69,0,0 ; LD   L,C
     .byte $6A,0,0 ; LD   L,D
     .byte $6B,0,0 ; LD   L,E
     .byte $6C,0,0 ; LD   L,H
     .byte $6D,0,0 ; LD   L,L
     .byte $6E,0,0 ; LD   L,(HL)
     .byte $6F,0,0 ; LD   L,A
     
     .byte $70,0,0 ; LD   (HL),B
     .byte $71,0,0 ; LD   (HL),C
     .byte $72,0,0 ; LD   (HL),D
     .byte $73,0,0 ; LD   (HL),E
     .byte $74,0,0 ; LD   (HL),H
     .byte $75,0,0 ; LD   (HL),L
     .byte $77,0,0 ; LD   (HL),A
     
     .byte $78,0,0 ; LD   A,B
     .byte $79,0,0 ; LD   A,C
     .byte $7A,0,0 ; LD   A,D
     .byte $7B,0,0 ; LD   A,E
     .byte $7C,0,0 ; LD   A,H
     .byte $7D,0,0 ; LD   A,L
     .byte $7E,0,0 ; LD   A,(HL)
     .byte $7F,0,0 ; LD   A,A
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
     ; Put different value in each register and (hl_temp)
     ld   b,$BC
     push bc
     ld   a,$DE
     ld   (rp_temp),a
     ld   a,$12
     ld   bc,$3456
     ld   de,$789A
     ld   hl,rp_temp ; (HL) points to RAM
     pop  af
     
;    call print_regs
     jp   instr
instr_done:
     
     ; Checksum registers and (HL)
     call checksum_af_bc_de_hl
     ld   a,(rp_temp)
     call update_crc_fast
     
     ret
     
checksums:
     .byte $40,$3A,$AF,$06,$B6,$CB,$B2,$AB,$6F,$EF,$71,$9B,$75,$E3,$6C,$B9,$34,$FB,$26,$B7,$5A,$B9,$2F,$CE,$34,$FB,$26,$B7,$C2,$0A,$3B,$1A,$2A,$8A,$D6,$7C,$40,$3A,$AF,$06,$AF,$0A,$74,$70,$19,$A9,$6E,$6F,$11,$DA,$FE,$FE,$18,$10,$04,$2B,$11,$DA,$FE,$FE,$7B,$6A,$87,$84,$8B,$87,$34,$12,$00,$45,$DE,$01,$40,$3A,$AF,$06,$93,$E2,$8F,$C6,$DD,$7D,$90,$32,$FF,$90,$1B,$A8,$DD,$7D,$90,$32,$56,$BF,$7A,$21,$23,$C0,$FA,$06,$3B,$1D,$A0,$80,$3F,$44,$1B,$9C,$40,$3A,$AF,$06,$56,$25,$85,$CD,$D7,$B1,$DB,$F9,$56,$25,$85,$CD,$4E,$F8,$DF,$4B,$F0,$C3,$F9,$18,$20,$0F,$F6,$91,$71,$69,$CE,$46,$F0,$A0,$03,$4D,$40,$3A,$AF,$06,$29,$47,$E2,$36,$40,$3A,$AF,$06,$90,$F6,$A0,$8F,$3D,$62,$26,$A9,$A4,$52,$C1,$75,$45,$ED,$75,$40,$8A,$4D,$63,$56,$AF,$BA,$2D,$FE,$40,$3A,$AF,$06,$AF,$BA,$2D,$FE,$36,$8A,$CA,$22,$34,$8D,$C2,$65,$1A,$DB,$FF,$54,$32,$C0,$E8,$55,$ED,$4A,$87,$2F,$40,$3A,$AF,$06,$9D,$BC,$81,$E6,$6E,$6C,$92,$37,$B1,$EC,$C3,$29,$1D,$C5,$9F,$A1,$59,$6F,$66,$CD,$B4,$FB,$FD,$74,$EC,$13,$F3,$8E,$70,$0C,$5F,$ED,$EC,$13,$F3,$8E,$40,$3A,$AF,$06,
