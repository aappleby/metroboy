.include "header.inc"

.define DELAY 64
.define RESULT $83
.define SPRITEX 0
.define SPRITEY 0

main:
  lcd_off_unsafe

  load_sprite 0 SPRITEY SPRITEX 0 0

  ld a, 5
  ldh (SCX), a

  lcd_on_sprites
  nops 114 - 2

  nops DELAY
  test_finish_stat RESULT