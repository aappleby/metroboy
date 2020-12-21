.include "header.inc"

.define DELAY  0
.define ADDR   $FF00
.define RESULT $CF

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
