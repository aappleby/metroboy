; pass - ags

.include "header.inc"

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  lcd_on
  nops 114

  nops 104

  ldh a, ($FE00)

  test_finish_a $55