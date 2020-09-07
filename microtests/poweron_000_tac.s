.include "header.inc"

.define DELAY  0
.define ADDR   $FF07
.define RESULT $F8

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
