.include "header.inc"

main:

  ld a, $FF
  ld ($8000), a

  ld h, $FF
  ld l, $80
  
  ld a, $E0
  ld (hl+), a

  ld a, $46
  ld (hl+), a

  ld a, $18
  ld (hl+), a

  ld a, $FE
  ld (hl+), a

  ld a, $80
  jp $ff80
