.include "header.inc"

.define BASE $FDF1

main:
  ld hl, $C003
  ld a, $55

- ld (hl), a
  jr -




/*
  ld hl, BASE
  ld a, $77  // ld (hl),a
  ld (hl+), a
  ld a, $18 // jr
  ld (hl+), a
  ld a, $FD // -3
  ld (hl+), a
*/
