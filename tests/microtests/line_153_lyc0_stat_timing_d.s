; pass - dmg

.include "header.inc"

; 0   - c1

; now we are in line 153
; 106 - c1
; 107 - c1
; 108 - c5
; 218 - c5
; 219 - c4

; now we are in line 0
; 220 - c6
; 239 - c6
; 240 - c7
; 282 - c7
; 283 - c4
; 332 - c4
; 333 - c0

; now we are in line 1
; 334 - c2

main:
  reset_lcd_for_test

  set_stat_int_lyc

  long_delay 114 * 152

  nops 108

  test_finish_stat $C5