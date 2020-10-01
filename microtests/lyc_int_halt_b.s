.include "header.inc"

main:
  ld a, 2
  ldh (LYC), a
  ldh (DIV), a

  lcd_off_unsafe
  set_stat_int_lyc
  set_ie_stat  
  ei
  lcd_on  

  halt
  di
  test_fail
  
  //nops 227
  //di
  //test_fail

.org STAT_INT_VECTOR
  nops 64
  ldh a, (DIV)
  test_finish_a 5