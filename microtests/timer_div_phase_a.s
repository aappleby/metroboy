.include "header.inc"

main:
  nops 5
  ldh a, (DIV)
  xor ($55 ~ %10101011)
end:
  ld ($8000), a
  jr end
