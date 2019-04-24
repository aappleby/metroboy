; pass - dmg

.include "header.inc"

; LCD on -> OAM int

; dmg = 0? erroneous line 0 ints
; ags = 100

.ifdef DMG
  .define RESULT 0
.endif

.ifdef AGS
  .define RESULT 100
.endif

;-------------------------------------------------------------------------------

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 0

  set_stat_int_oam
  set_ie_stat
  ei

  xor a
.repeat 200
  inc a
.endr

  test_fail

.org STAT_INT_VECTOR
  test_finish_a RESULT

;-------------------------------------------------------------------------------
