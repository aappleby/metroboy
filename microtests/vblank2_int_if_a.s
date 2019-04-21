; pass - ags

.include "header.inc"

; 103 - 0
; 104 - 1
; 105 - 1
; 106 - 1
; 107 - 0

main:
  di
  lcd_off_unsafe
  lcd_on

  ; This delay loop will run out at the beginning of line 143
  xor a
  ld bc, 2972
- dec bc
  cp b
  jr nz, -

  nops 103

  clear_if
  ldh a, (IF)
  test_finish_a $E0