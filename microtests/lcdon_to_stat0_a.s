; pass - dmg, ags

.include "header.inc"

; LCD on -> STAT mode reads 0

;-------------------------------------------------------------------------------
  
main:

  lcd_off_unsafe

  lcd_on

  test_finish_stat $FF

;-------------------------------------------------------------------------------
