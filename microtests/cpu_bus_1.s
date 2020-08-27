.include "header.inc"

.define BASE $FDF1

main:
  ld a, 0
  ld ($FF40), a

  ld hl, BASE
  ld a, $18
  ld (hl+), a
  ld a, $FE
  ld (hl+), a

  jp BASE




/*
  ld hl, BASE
  ld a, $77  // ld (hl),a
  ld (hl+), a
  ld a, $18 // jr
  ld (hl+), a
  ld a, $FD // -3
  ld (hl+), a
*/
