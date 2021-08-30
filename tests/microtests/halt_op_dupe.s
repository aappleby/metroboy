; pass - dmg

.include "header.inc"

; LCD on -> HBLANK int

; so if (if & ie & !ime) when the halt is hit, we execute the following byte of opcode
; instead of the halt

;-------------------------------------------------------------------------------

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 114

  set_ie_stat
  set_stat_int_hblank

  nops 114

  xor a
  halt
  inc a
  nop

  test_finish_a 2

;-------------------------------------------------------------------------------
