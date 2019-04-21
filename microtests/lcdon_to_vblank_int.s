; pass - ags

.include "header.inc"

; LCD on -> VBLANK int

;-------------------------------------------------------------------------------

main:
  di
  lcd_off_unsafe
  lcd_on

  ld a, %00000001
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

.org VBLANK_INT_VECTOR
  test_finish_a %10011111

;-------------------------------------------------------------------------------
