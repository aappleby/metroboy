; pass - dmg

.include "header.inc"

; line 152
; 100 - C1

; line 153
; 101 - C5
; 102 - C1
; 213 - C1
; 214 - C0

; line 0
; 215 - C2

main:
  reset_lcd_for_test

  set_stat_int_lyc

  ld a, 153
  ldh (LYC), a

  long_delay 114 * 152

  nops 101

  test_finish_stat $C5