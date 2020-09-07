.include "header.inc"

.define DELAY  0
.define ADDR   $FF47
.define RESULT $FC

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
