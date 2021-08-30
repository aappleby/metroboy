.include "header.inc"

.define DELAY  5
.define ADDR   $FF04
.define RESULT $AC

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
