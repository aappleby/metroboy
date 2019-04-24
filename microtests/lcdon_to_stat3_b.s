; pass - dmg

.include "header.inc"

; LCD on -> STAT mode reads 3

;-------------------------------------------------------------------------------
  
main:

  lcd_off_unsafe

  lcd_on

  nops 17

  test_finish_stat $87

;-------------------------------------------------------------------------------
