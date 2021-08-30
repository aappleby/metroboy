; pass - dmg

.include "header.inc"

main:
  reset_lcd_for_test

  set_stat_int_hblank
  ld a, 2
  ldh (SCX), a

  nops 103


  clear_if




  nops 57



  test_finish_if $E2
