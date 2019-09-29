.include "header.inc"

main:
  ld a, 0
  ld ($FF04), a
  nops 60
  ld a, ($FF04)
  add $55 - %00000001
end:
  ld ($8000), a
  jr end
