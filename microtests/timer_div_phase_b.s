.include "header.inc"

main:
  nops 21
  ld a, ($FF04)
  add $55 - %10011010
end:
  ld ($8000), a
  jr end
