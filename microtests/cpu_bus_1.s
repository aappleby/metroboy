.include "header.inc"

.define BASE $C003

main:
  ld hl, BASE
  ld a, $55
  jp loop

.org $3FF0
loop:
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
