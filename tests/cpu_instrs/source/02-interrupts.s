; Tests DI, EI, and HALT (STOP proved untestable)

.include "shell.inc"

main:
     wreg IE,$04
     
     set_test 2,"EI"
     ei
     ld   bc,0
     push bc
     pop  bc
     inc  b
     wreg IF,$04
interrupt_addr:
     dec  b
     jp   nz,test_failed
     ld   hl,sp-2
     ldi  a,(hl)
     cp   <interrupt_addr
     jp   nz,test_failed
     ld   a,(hl)
     cp   >interrupt_addr
     jp   nz,test_failed
     lda  IF
     and  $04
     jp   nz,test_failed
     
     set_test 3,"DI"
     di
     ld   bc,0
     push bc
     pop  bc
     wreg IF,$04
     ld   hl,sp-2
     ldi  a,(hl)
     or   (hl)
     jp   nz,test_failed
     lda  IF
     and  $04
     jp   z,test_failed
     
     set_test 4,"Timer doesn't work"
     wreg TAC,$05
     wreg TIMA,0
     wreg IF,0
     delay 500
     lda  IF
     delay 500
     and  $04
     jp   nz,test_failed
     delay 500
     lda  IF
     and  $04
     jp   z,test_failed
     pop  af
     
     set_test 5,"HALT"
     wreg TAC,$05
     wreg TIMA,0
     wreg IF,0
     halt      ; timer interrupt will exit halt
     nop       ; avoids DMG bug
     lda  IF
     and  $04
     jp   z,test_failed
     
     jp   tests_passed

.bank 0 slot 0
.org $50
     inc a
     ret
