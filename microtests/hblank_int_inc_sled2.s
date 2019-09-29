; havent tested this yet

.include "header.inc"

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 114

  set_stat_int_hblank
  set_ie_stat
  ei

  xor a
.repeat 200
  inc a
.endr

  test_finish_a 200

.org STAT_INT_VECTOR
  reti
  
