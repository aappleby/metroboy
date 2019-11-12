.include "header.inc"

.define DELAY  0
.define ADDR   $FF0F
.define RESULT $E1

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
