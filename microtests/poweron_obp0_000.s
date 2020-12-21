.include "header.inc"

.define DELAY  0
.define ADDR   $FF48
//.define RESULT $9F
.define RESULT $FF

main:
  nops DELAY
  ld a, (ADDR)
//- ld ($8000),a
//  jr -

  test_finish_a RESULT
