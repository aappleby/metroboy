.include "header.inc"

.define DELAY  27
.define ADDR   $FF41
.define RESULT $87

// line 0
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
// 140 - 10000010 - oam
// 141 - 10000011 - vram
// 183 - 10000011 - vram
// 184 - 10000000 - hblank
// 234 - 10000000 - hblank

// line 2
// 235 - 10000010 - oam

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT