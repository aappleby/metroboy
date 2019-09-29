.include "header.inc"

; DIV and TIMA should start such that adding up four values of TIMA every 3 clocks should equal XX.

.ifdef AGS
  .define PASS 11
  //broken broken broken
.endif

.ifdef DMG
  .define PASS 9
.endif

main:
  ; Start the fast timer
  ld a, %00000101
  ldh ($07), a
  ld hl, $FF05
  xor a

  ; Adding four timer reads at odd clock intervals should establish where in the timer phase we are.
  add (hl)
  nop
  add (hl)
  nop
  add (hl)
  nop
  add (hl)
  nop

  add $55 - PASS
end:
  ld ($8000), a
  jr end