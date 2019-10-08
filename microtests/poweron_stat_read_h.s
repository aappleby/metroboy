.include "header.inc"

.define DELAY  70
.define ADDR   $FF41
.define RESULT $84

//   0 - 10000101 - vblank
//   5 - 10000101 - vblank
//   6 - 10000100 - glitch hblank
//   7 - 10000110 - oam
//  26 - 10000110 - oam
//  27 - 10000111 - vram
//  69 - 10000111 - vram
//  70 - 10000100 - hblank
// 119 - 10000100 - hblank
// 120 - 10000000 - hblank

// line 1
// 121 - 10000010 - oam
// 122 - 10000010 - oam
// 124 - 10000010 - oam
// 128 - 10000010 - oam

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT