.include "header.inc"

.define DELAY  0
.define ADDR   $FF40
.define RESULT $91

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
