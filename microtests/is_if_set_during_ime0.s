; pass - dmg, ags

.include "header.inc"

; IF bits _do_ get set if IME=0

;-------------------------------------------------------------------------------

main:
  di

  lcd_off_unsafe
  lcd_on

  ; turn on hblank ints
  ld a, %00001000
  ldh (STAT), a

  ; turn on all other ints
  ld a, $FF
  ldh (IE), a

  ; clear ints and wait a really long time
  clear_if

  ld bc, 0
- dec bc
  cp b
  jr nz, -

  ; were any bits of IF set?
  ldh a, (IF)
  test_finish_a $E3

;-------------------------------------------------------------------------------
