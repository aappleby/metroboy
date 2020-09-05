.include "header.inc"

.define BASE $FF80

main:
  ld hl, BASE
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
