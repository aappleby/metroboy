; pass - ags

.include "header.inc"

; ags - 
; 104 - off
; 105 - on
; 108 - on
; 109 - off

; dmg -
; line 0 is always on (line0 glitch)
; 114 + 57  = off
; 114 + 58  = on (stat glitch)
; 114 + 108 = on
; 114 + 109 = off

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  lcd_on
  nops 114

  nops 109

  set_stat_int_oam
  ldh a, (IF)

  test_finish_a $E0