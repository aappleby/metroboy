; pass - dmg, ags

.include "header.inc"

; LCD on -> STAT mode reads 3

;-------------------------------------------------------------------------------
  
main:

  lcd_off_unsafe

  lcd_on

  nops 16

  test_finish_stat $84

;-------------------------------------------------------------------------------
