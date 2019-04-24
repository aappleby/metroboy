; pass - dmg, ags

.include "header.inc"

; LCD on -> STAT mode reads 2

;-------------------------------------------------------------------------------
  
main:

  lcd_off_unsafe

  lcd_on

  nops 111

  test_finish_stat $82

;-------------------------------------------------------------------------------
