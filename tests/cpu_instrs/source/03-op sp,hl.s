; Tests SP/HL instructions

;.define PRINT_CHECKSUMS 1
.include "shell.inc"
.include "instr_test.s"

instrs:
     .byte $33,0,0     ; INC  SP
     .byte $3B,0,0     ; DEC  SP
     .byte $39,0,0     ; ADD  HL,SP
     .byte $F9,0,0     ; LD   SP,HL
     .byte $E8,$01,0   ; ADD  SP,1
     .byte $E8,$FF,0   ; ADD  SP,-1
     .byte $F8,$01,0   ; LD   HL,SP+1
     .byte $F8,$FF,0   ; LD   HL,SP-1
instrs_end:

test_instr:
     ; C = flags register
     ld   c,$00
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
     
     ; Go through each value for SP
     ld   hl,values
values_loop:
     push bc
     push de
     push hl
     
     push bc
     pop  af
     
     ; Switch stack
     ld   (temp),sp
     ld   a,(hl+)
     ld   h,(hl)
     ld   l,a
;    call print_regs
     ld   sp,hl
     
     ; Set registers
     ld   h,d
     ld   l,e
     ld   a,$12
     ld   bc,$5691
     ld   de,$9ABC
     
     jp   instr
instr_done:
     ; Save new SP and switch to yet another stack
     ld   (temp+2),sp
     ld   sp,$DF70
     
     call checksum_af_bc_de_hl
     
     ; Checksum SP
     ld   a,(temp+2)
     call update_crc_fast
     ld   a,(temp+3)
     call update_crc_fast
     
     ldsp temp
     
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
     .byte $BC,$F4,$CD,$8C,$C7,$5E,$89,$E5,$36,$65,$21,$55,$D6,$6A,$2A,$FF
     .byte $EB,$34,$37,$B9,$08,$5F,$22,$13,$B6,$2A,$37,$C3,$72,$43,$5C,$4D
