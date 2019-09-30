.include "header.inc"

main:


  nops 21
  ldh a, (DIV)
  xor ($55 ~ %10011001)
end:
  ld ($8000), a
  jr end
