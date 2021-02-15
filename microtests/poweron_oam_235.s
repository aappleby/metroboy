.include "header.inc"

// line 0
//   0 - ..
//   5 - ..
//   6 - FF
//  69 - FF
//  70 - ..
// 119 - ..
// 120 - FF

// line 1
// 121 - FF
// 183 - FF
// 184 - ..
// 233 - ..

// line 2
// 234 - FF
// 235 - FF

main:
  nops 235
  ld a, ($FE35)
  test_finish_a $FF