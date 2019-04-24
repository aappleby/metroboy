; pass - dmg

.include "header.inc"

; LCD on -> STAT mode reads 2

; AGS doesnt have lyc match reading 0 on last cycle of line?

.ifdef DMG
  .define RESULT $80
.endif

.ifdef AGS
  .define RESULT $84
.endif

;-------------------------------------------------------------------------------
  
main:

  lcd_off_unsafe

  lcd_on

  nops 110

  test_finish_stat RESULT

;-------------------------------------------------------------------------------
