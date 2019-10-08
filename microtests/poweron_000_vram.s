.include "header.inc"

.define DELAY  0
.define ADDR   $8000
.define RESULT $FF

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
  nops DELAY
  ld a, (ADDR)
  test_finish_not_a RESULT
