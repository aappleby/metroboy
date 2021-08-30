.include "header.inc"

main:
  // save old lcdc
  ldh a, ($40)
  ld b, a

  // clear lcdc
  ld a, 0
  ldh ($40), a

  // read ly
  ldh a, ($44)
  ld c, a

  // reload lcdc
  ld a, b
  ldh ($40), a

  // put ly on screen
  ld a, c

- ld ($8000), a
  jr -
