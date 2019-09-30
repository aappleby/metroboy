.include "header.inc"

.define DELAY 2
.define RESULT $FF

// 0 - FE
// 1 - FF
// 2 - FF
// 3 - FF
// 4 - FF
// 5 - 00
// 6 - 80
// 7 - 80
// 8 - 80
// 9 - 81

main:
  ld a, $80
  ldh (TMA), a

  ld a, %00000101
  ldh (TAC), a
  ld a, $FD
  ldh (TIMA), a
  ldh (DIV), a

  nops DELAY

  ldh a, (TIMA)
  xor ($55 ~ RESULT)
end:
  ld ($8000), a
  jr end
