.include "header.inc"

// line 0
//   0 - 00
//  25 - 00
//  26 - FF
//  69 - FF
//  70 - 00

// line 1
// 139 - 00
// 140 - FF
// 183 - FF
// 184 - 00

main:
  nops 70
  ld a, ($8059)
  test_finish_not_a $FF