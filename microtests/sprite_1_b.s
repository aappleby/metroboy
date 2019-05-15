.include "header.inc"

; x  - 60/61
; 8  - 62/63
; 9  - 62/63
; 10 - 62/63
; 11 - 62/63
; 12 - 61/62
; 13 - 61/62
; 14 - 61/62
; 15 - 61/62

.define SPRITEX 9
.define DELAY 64
.define RESULT $80

main:
  lcd_off_unsafe
  load_sprite 0 16 SPRITEX 0 0
  lcd_on_sprites
  nops 114 - 2

  nops DELAY
  test_finish_stat RESULT