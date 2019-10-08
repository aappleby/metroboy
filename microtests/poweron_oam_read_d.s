.include "header.inc"

.define DELAY  114
.define ADDR   $FE00
.define RESULT $FF

//  63 = FF
//  64 = F0
// 113 = F0
// 114 = FF

main:
  ld a, RESULT
  ld (ADDR), a
  nops DELAY
  ld a, (ADDR)

  test_finish_a RESULT