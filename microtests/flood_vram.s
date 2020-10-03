.include "header.inc"

; We should be able to write our dotted line pattern to vram on startup.

main:
  ld hl, $8000
  ld a, $FF
  ld bc, $0000

- ld (hl+), a
  inc bc
  bit 5,b
  jr z,-

- jr -