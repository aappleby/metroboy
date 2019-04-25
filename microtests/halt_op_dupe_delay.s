; pass - dmg

.include "header.inc"

; LCD on -> HBLANK int

; so if (if & ie) when the halt is hit, we execute the following byte of opcode
; instead of the halt

;-------------------------------------------------------------------------------

main:
  di
  clear_if
  clear_stat
  lcd_off_unsafe
  lcd_on
  nops 114

  set_ie_stat
  set_stat_int_hblank

  nops 114

  xor a
  ldh (DIV), a

  halt   ; halt takes two cycles, next op is duped on second cycle
  nop

  ; 57/58
  nops 58

  test_finish_div $55

;-------------------------------------------------------------------------------
