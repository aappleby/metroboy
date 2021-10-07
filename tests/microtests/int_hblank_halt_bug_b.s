.include "header.inc"

main:
  cycle_init

  ld a, %00001000
  ldh (STAT), a
  ld a, %00000010
  ldh (IE), a
  ld a, %00000010
  ldh (IF), a

  //cycle_count
  //test_display_a
  // 0b00011000 24

  // IF _is_ set, so we do _not_ halt.

  ld a, 0
  halt          // halt takes two cycles
                // but it's only one byte so we read the next op here
  inc a         // inc takes one cycle

  cycle_count
  test_display_a // 0b00011101 // 29

  //ld a, 0
  //halt
  //inc a

  //test_finish_a 2

.org STAT_INT_VECTOR
  test_fail
