.include "header.inc"

main:
  ld a, $55
  ld hl, $C003
- ld (hl), a
  jr -


/*
  ld a, $55
  ld hl, $0158
- ld a, (hl)
  jr -
  daa
*/