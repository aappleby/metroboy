; pass -

.include "header.inc"

; immediately after reset -
; IE   = $00
; IF   = $E0
; LCDC = $91
; STAT = $84

main:
  ld a, 1
  ldh (LYC), a

  xor a
  ldh (STAT), a
  ldh (IE), a
  ldh (IF), a
  ldh (LCDC), a

  ld a, $91
  ldh (LCDC), a

  xor a
  ldh (IF), a
  ldh (STAT), a

  test_finish_if $E0