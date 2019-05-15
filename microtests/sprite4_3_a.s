.include "header.inc"

.define SPRITEX 11
.define DELAY 69
.define RESULT $83

main:
  lcd_off_unsafe
  load_sprite 0 16 (SPRITEX + 0) 0 0
  load_sprite 1 16 (SPRITEX + 8) 0 0
  load_sprite 2 16 (SPRITEX + 16) 0 0
  load_sprite 3 16 (SPRITEX + 24) 0 0
  lcd_on_sprites
  nops 114 - 2

  nops DELAY
  test_finish_stat RESULT