.include "header.inc"

.define DELAY  69
.define ADDR   $FE00
.define RESULT $FF

// line 0
//   0 - ..
//   5 - ..
//   6 - FF
//  69 - FF
//  70 - ..
// 119 - ..

// line 1
// 120 - FF
// 121 - FF
// 183 - FF
// 184 - ..
// 233 - ..

// line 2
// 234 - FF
// 235 - FF

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT