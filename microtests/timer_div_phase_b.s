; pass - dmg

.include "header.inc"

main:


  nops 6
  ldh a, (DIV)
  xor ($55 ~ %10101100)
end:
  ld ($8000), a
  jr end