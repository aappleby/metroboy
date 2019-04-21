.include "header.inc"

; LCD on -> VBLANK int

; metroboy - 155
; dmg - 154
; ags - 154

;-------------------------------------------------------------------------------

main:
  di

  lcd_off_unsafe
  lcd_on

  ld a, %00010000
  ldh (STAT), a
  ld a, %00000010
  ldh (IE), a
  xor a
  ldh (IF), a

  ei

  ld b, 0

delay:
.repeat 100
  inc a
.endr
  dec b
  jr nz, delay

  test_fail

.org STAT_INT_VECTOR
  test_finish_a 154

;-------------------------------------------------------------------------------
