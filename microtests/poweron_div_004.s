.include "header.inc"

.define DELAY  4
.define ADDR   $FF04
.define RESULT $AB

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
