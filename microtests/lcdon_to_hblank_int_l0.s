; pass - dmg (sort of)

.include "header.inc"

; LCD on -> HBLANK int

; dmg = 0? erroneous line 0 ints
; ags = 50

.ifdef DMG
  .define RESULT 0
.endif

.ifdef AGS
  .define RESULT 50
.endif

;-------------------------------------------------------------------------------

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 0

  set_stat_int_hblank
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
