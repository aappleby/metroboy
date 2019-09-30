.include "header.inc"

.define DELAY 14

main:
  ld a, %00000101
  ldh ($07), a
  xor a
  ldh ($04), a

  ld a, $80
  ldh (TMA), a
  ld a, $FA
  ldh (TIMA), a

  // 18: TIMA = FF
  // 19: TIMA = 00
  // 20: TIMA = TMA
  // 21: TIMA = TMA
  // 22: TIMA = TMA
  // 23: TIMA = TMA + 1
  // 24: TIMA = TMA + 1
  // 25: TIMA = TMA + 1
  // 26: TIMA = TMA + 1
  // 27: TIMA = TMA + 2

  nops 23 - 5

  ld a, $70     // 2
  ldh (TMA), a  // 3

  // this load fires on the same cycle as TIMA is set to TMA
  ldh a, (TIMA)
  xor ($55 ~ $71)
  
end:
  ld ($8000), a
  jr end
