; pass - dmg

.include "header.inc"

main:
  ld a, 0
  ldh (DIV), a
  nops 61
  ldh a, (DIV)
  xor($55 ~ %00000001)
end:
  ld ($8000), a
  jr end