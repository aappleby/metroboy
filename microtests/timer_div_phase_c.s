.include "header.inc"

main:
  ld a, 0
  ldh (DIV), a
  nops 60
  ldh a, (DIV)
  xor ($55 ~ %00000000)
end:
  ld ($8000), a
  jr end
