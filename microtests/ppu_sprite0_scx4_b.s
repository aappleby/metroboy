.include "header.inc"

main:
  lcd_off_unsafe

  load_sprite 0 0 0 0 0

  ld a, 4
  ldh (SCX), a

  lcd_on_sprites
  nops 177
  test_finish_stat $80