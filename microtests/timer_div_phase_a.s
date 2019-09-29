.include "header.inc"

main:
  nops 20
  ld a, ($FF04)
  add $55 - %10011001  
end:
  ld ($8000), a
  jr end
