.include "header.inc"

.define DELAY  0
.define ADDR   $FF49
.define RESULT $FF

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
