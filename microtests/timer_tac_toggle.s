.include "header.inc"

test:
  xor a
  ldh (DIV), a
  ldh (TIMA), a

  ld hl, $FF07
  ld a, %00000100
  ld (hl), a

.repeat 100
  ld a, %00000100 // 2
  ld (hl), a      // 2
  ld a, %00000000 // 2
  ld (hl), a      // 2
  nop             // 1
.endr

  ldh a, (TIMA)
  xor $55 ~ %00101101
  ld ($8000), a
- jr -
