.include "header.inc"

.define WINX   6
.define DELAY  62
.define RESULT $83

main:
  lcd_off_unsafe

  ld a, 0
  ldh (WY), a

  ld a, WINX
  ldh (WX), a
  
  ld a, %10110001
  ldh (LCDC), a
  nops 114 - 2

  nops DELAY
  test_finish_stat RESULT