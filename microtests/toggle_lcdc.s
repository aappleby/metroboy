.include "header.inc"

.define DELAY  0
.define ADDR   $FF40
.define RESULT $91

main:
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a
  ld a, $80
  ld (ADDR), a
  ld a, $00
  ld (ADDR), a


- jr -