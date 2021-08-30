.include "header.inc"

.define DELAY  234
.define ADDR   $FF44
.define RESULT 2

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
