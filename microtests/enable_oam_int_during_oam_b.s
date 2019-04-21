.include "header.inc"

; What cycle can we turn oam int on and catch it for the next line?
; tested - ags, dmg

.define DELTA 104

;-------------------------------------------------------------------------------

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops DELTA

  ld a, %00000010
  ldh (IE), a

  ld a, %00100000
  ldh (STAT), a

  nops 114 - DELTA

  ldh a, (IF)

  test_finish_a $E0

;-------------------------------------------------------------------------------
