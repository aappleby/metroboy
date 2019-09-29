.include "header.inc"

main:
  ld a, %00000101
  ldh ($07), a

  xor a
  ldh ($04), a

  nops 2

  ld a, 10
  ldh ($05), a
  ldh a, ($05)
  add $55 - 11
end:
  ld ($8000), a
  jr end
