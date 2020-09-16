; pass - ags

.include "header.inc"

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  ld a, $27
  ld ($FE00), a
  lcd_on

  nops 59
  ld a, ($FE00)
  test_finish_a $27