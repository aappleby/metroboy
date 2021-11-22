; Tests branch instructions

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     ;       JR cond,skip
     ;       INC A
     ; skip:
     .byte $18,$01,$3C ; JR   *+3
     .byte $20,$01,$3C ; JR   NZ,*+3
     .byte $28,$01,$3C ; JR   Z,*+3
     .byte $30,$01,$3C ; JR   NC,*+3
     .byte $38,$01,$3C ; JR   C,*+3
     
     .byte $C2,<taken,>taken ; JP   NZ,taken
     .byte $C3,<taken,>taken ; JP   taken
     .byte $CA,<taken,>taken ; JP   Z,taken
     .byte $D2,<taken,>taken ; JP   NC,taken
     .byte $DA,<taken,>taken ; JP   C,taken

     .byte $C4,<taken,>taken ; CALL NZ,taken
     .byte $CC,<taken,>taken ; CALL Z,taken
     .byte $CD,<taken,>taken ; CALL taken
     .byte $D4,<taken,>taken ; CALL NC,taken
     .byte $DC,<taken,>taken ; CALL C,taken
     
     ; RET cond
     ; INC A
     .byte $C0,$3C,0 ; RET  NZ
     .byte $C8,$3C,0 ; RET  Z
     .byte $C9,$3C,0 ; RET
     .byte $D0,$3C,0 ; RET  NC
     .byte $D8,$3C,0 ; RET  C
     .byte $D9,$3C,0 ; RETI
     
     ; RST
     ; can only easily test this one on devcart
     .byte $C7,0,0 ; RST $00
.ifndef BUILD_DEVCART
     .byte $CF,0,0 ; RST $08
     .byte $D7,0,0 ; RST $10
     .byte $DF,0,0 ; RST $18
     .byte $E7,0,0 ; RST $20
     .byte $EF,0,0 ; RST $28
     .byte $F7,0,0 ; RST $30
     .byte $FF,0,0 ; RST $38
.endif

instrs_end:

test_instr:
     wreg IE,0 ; disable interrupts, since RETI does EI
     
     ; Go through all 16 combinations of flags
     ld   bc,$1200
-    
     ; Fill 4 bytes of new stack
     ld   a,$12
     ld   ($DF80-2),a
     ld   a,$34
     ld   ($DF80-3),a
     ld   a,$56
     ld   ($DF80-4),a
     ld   a,$78
     ld   ($DF80-5),a
     
     ; Set AF
     push bc
     pop  af
     
     ; Switch to new stack
     ld   (temp),sp
     ld   sp,$DF80
     
     ; Set return address
     ld   de,instr+3
     push de
     
     jp   instr
instr_done:
     inc  a
taken:
     di             ; RETI enables interrupts
     
     ; Save new SP and switch to yet another stack
     ld   (temp+2),sp
     ld   sp,$DF70
     
     ; Checksum A and SP
     call update_crc_fast
     ld   a,(temp+2)
     call update_crc_fast
     ld   a,(temp+3)
     call update_crc_fast
     
     ; Checksum 4 bytes of stack
     ld   a,($DF80-2)
     call update_crc_fast
     ld   a,($DF80-3)
     call update_crc_fast
     ld   a,($DF80-4)
     call update_crc_fast
     ld   a,($DF80-5)
     call update_crc_fast
     
     ldsp temp
     
     ld   a,c
     add  $10
     ld   c,a
     jr   nz,-
     
     ret

checksums:
     .byte $EC,$A4,$94,$79,$C4,$00,$96,$2C,$C4,$64,$90,$33,$77,$C7,$0A,$D4
     .byte $77,$A3,$0C,$CB,$79,$E7,$7E,$AE,$DA,$DC,$03,$F7,$4F,$9F,$E9,$20
     .byte $72,$12,$DA,$01,$44,$6A,$4D,$8F,$D1,$79,$30,$4C,$AA,$37,$F2,$6A
     .byte $97,$EA,$56,$5F,$32,$28,$C7,$D1,$49,$66,$05,$F7,$80,$0F,$BA,$8E
     .byte $41,$E2,$A4,$9A,$2D,$2D,$8C,$72,$A5,$13,$76,$A8,$64,$FE,$68,$BC
     .byte $2D,$2D,$8C,$72,$50,$96,$24,$27,$50,$96,$24,$27,$50,$96,$24,$27
     .byte $50,$96,$24,$27,$50,$96,$24,$27,$50,$96,$24,$27,$50,$96,$24,$27
     .byte $50,$96,$24,$27

.include "multi_custom.s"
