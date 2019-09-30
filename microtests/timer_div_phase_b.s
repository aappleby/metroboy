.include "header.inc"

main:


  nops 22
  ldh a, (DIV)
  xor ($55 ~ %10011010)
end:
  ld ($8000), a
  jr end
