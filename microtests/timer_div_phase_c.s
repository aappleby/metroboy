.include "header.inc"

main:
  ld a, 0
  ld ($FF04), a
  nops 59
  ld a, ($FF04)
  add $55 - %00000000
end:
  ld ($8000), a
  jr end
