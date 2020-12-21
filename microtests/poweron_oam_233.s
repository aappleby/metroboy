.include "header.inc"

.define DELAY  233
.define ADDR   $FE00
.define RESULT $FF

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
// 234 - FF
// 235 - FF

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_not_a RESULT