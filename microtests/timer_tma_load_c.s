.include "header.inc"

main:
  ld a, $80
  ldh (TMA), a
  ld a, $FC
  ldh (TIMA), a
  ld a, %00000101
  ldh (TAC), a
  xor a
  ldh (DIV), a

  nops 6
  ldh a, (TIMA)
  add $D5
  
end:
  ld ($8000), a
  jr end
