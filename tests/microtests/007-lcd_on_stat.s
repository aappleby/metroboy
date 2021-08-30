.include "header.inc"

; Check the state of the LCD registers after turning the LCD back on.

; AGS no lcd reset -
; STAT = 0x81
; DMG-01 no lcd reset -
; STAT = 0x80

; DMG-1 after reset
; STAT = 0x84

main:

  di
  xor a
  ldh ($0F), a

  ld a, $01
  ldh ($FF), a
  ei

  halt
  nop

end1:
  jr end1

.org $40
vblank_intr:

turn_lcd_off:
  ld a, $00
  ldh ($40), a

  ld a, $91
  ldh ($40), a

  ;ldh a, ($41)
  ldh a, ($44)
  ld ($8000), a

end2:
  jr end2