.include "header.inc"

.define DELAY  0
.define ADDR   $FF44
.define RESULT 0

main:
	nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
