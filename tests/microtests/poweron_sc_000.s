.include "header.inc"

.define DELAY  0
.define ADDR   $FF02
.define RESULT $7E

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
