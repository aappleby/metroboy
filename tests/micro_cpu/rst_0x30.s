.include "header.inc"

main:
  rst $30

.org $00
  ld a, $FF
  ld ($8000), a
- jr -

.org $08
  ld a, $FF
  ld ($8000), a
- jr -

.org $10
  ld a, $FF
  ld ($8000), a
- jr -

.org $18
  ld a, $FF
  ld ($8000), a
- jr -

.org $20
  ld a, $FF
  ld ($8000), a
- jr -

.org $28
  ld a, $FF
  ld ($8000), a
- jr -

.org $30
  ld a, $55
  ld ($8000), a
- jr -

.org $38
  ld a, $FF
  ld ($8000), a
- jr -

