.include "header.inc"

.define DELAY 61
.define RESULT $83
.define SPRITEX 0
.define SPRITEY 0

.macro run_test ARGS scroll spritex spritey delay result
  lcd_off_unsafe
  load_sprite 0 spritey spritex 0 0
  ld a, scroll
  ldh (SCX), a
  lcd_on_sprites
  long_delay (114 - 2) + delay

  ldh a, (STAT)
  cp result
  jp nz, fail
.endm

main:
  run_test 0 0 16 63 $83
  run_test 0 0 16 64 $80
  run_test 1 0 16 64 $83
  run_test 1 0 16 65 $80
  run_test 2 0 16 64 $83
  run_test 2 0 16 65 $80
  run_test 3 0 16 64 $83
  run_test 3 0 16 65 $80
  run_test 4 0 16 64 $83
  run_test 4 0 16 65 $80
  run_test 5 0 16 65 $83
  run_test 5 0 16 66 $80
  run_test 6 0 16 65 $83
  run_test 6 0 16 66 $80
  run_test 7 0 16 65 $83
  run_test 7 0 16 66 $80
  run_test 8 0 16 63 $83
  run_test 8 0 16 64 $80

  run_test 0 1 16 63 $83
  run_test 0 1 16 64 $80
  run_test 1 1 16 63 $83
  run_test 1 1 16 64 $80
  run_test 2 1 16 63 $83
  run_test 2 1 16 64 $80
  run_test 3 1 16 63 $83
  run_test 3 1 16 64 $80
  run_test 4 1 16 63 $83
  run_test 4 1 16 64 $80
  run_test 5 1 16 63 $83
  run_test 5 1 16 64 $80
  run_test 6 1 16 64 $83
  run_test 6 1 16 65 $80
  run_test 7 1 16 65 $83
  run_test 7 1 16 66 $80
  run_test 8 1 16 63 $83
  run_test 8 1 16 64 $80

  run_test 0 2 16 63 $83
  run_test 0 2 16 64 $80
  run_test 1 2 16 63 $83
  run_test 1 2 16 64 $80
  run_test 2 2 16 63 $83
  run_test 2 2 16 64 $80
  run_test 3 2 16 63 $83
  run_test 3 2 16 64 $80
  run_test 4 2 16 63 $83
  run_test 4 2 16 64 $80
  run_test 5 2 16 63 $83
  run_test 5 2 16 64 $80
  run_test 6 2 16 65 $83
  run_test 6 2 16 66 $80
  run_test 7 2 16 65 $83
  run_test 7 2 16 66 $80
  run_test 8 2 16 63 $83
  run_test 8 2 16 64 $80

  run_test 0 3 16 63 $83
  run_test 0 3 16 64 $80
  run_test 1 3 16 63 $83
  run_test 1 3 16 64 $80
  run_test 2 3 16 63 $83
  run_test 2 3 16 64 $80
  run_test 3 3 16 63 $83
  run_test 3 3 16 64 $80
  run_test 4 3 16 63 $83
  run_test 4 3 16 64 $80
  run_test 5 3 16 65 $83
  run_test 5 3 16 66 $80
  run_test 6 3 16 65 $83
  run_test 6 3 16 66 $80
  run_test 7 3 16 65 $83
  run_test 7 3 16 66 $80
  run_test 8 3 16 63 $83
  run_test 8 3 16 64 $80

  run_test 0 4 16 62 $83
  run_test 0 4 16 63 $80
  run_test 1 4 16 62 $83
  run_test 1 4 16 63 $80
  run_test 2 4 16 63 $83
  run_test 2 4 16 64 $80
  run_test 3 4 16 63 $83
  run_test 3 4 16 64 $80
  run_test 4 4 16 64 $83
  run_test 4 4 16 65 $80
  run_test 5 4 16 64 $83
  run_test 5 4 16 65 $80
  run_test 6 4 16 64 $83
  run_test 6 4 16 65 $80
  run_test 7 4 16 64 $83
  run_test 7 4 16 65 $80
  run_test 8 4 16 62 $83
  run_test 8 4 16 63 $80

  run_test 0 5 16 62 $83
  run_test 0 5 16 63 $80
  run_test 1 5 16 62 $83
  run_test 1 5 16 63 $80
  run_test 2 5 16 63 $83
  run_test 2 5 16 64 $80
  run_test 3 5 16 64 $83
  run_test 3 5 16 65 $80
  run_test 4 5 16 64 $83
  run_test 4 5 16 65 $80
  run_test 5 5 16 64 $83
  run_test 5 5 16 65 $80
  run_test 6 5 16 64 $83
  run_test 6 5 16 65 $80
  run_test 7 5 16 64 $83
  run_test 7 5 16 65 $80
  run_test 8 5 16 62 $83
  run_test 8 5 16 63 $80

  run_test 0 6 16 62 $83
  run_test 0 6 16 63 $80
  run_test 1 6 16 62 $83
  run_test 1 6 16 63 $80
  run_test 2 6 16 64 $83
  run_test 2 6 16 65 $80
  run_test 3 6 16 64 $83
  run_test 3 6 16 65 $80
  run_test 4 6 16 64 $83
  run_test 4 6 16 65 $80
  run_test 5 6 16 64 $83
  run_test 5 6 16 65 $80
  run_test 6 6 16 64 $83
  run_test 6 6 16 65 $80
  run_test 7 6 16 64 $83
  run_test 7 6 16 65 $80
  run_test 8 6 16 62 $83
  run_test 8 6 16 63 $80

  run_test 0 7 16 62 $83
  run_test 0 7 16 63 $80
  run_test 1 7 16 64 $83
  run_test 1 7 16 65 $80
  run_test 2 7 16 64 $83
  run_test 2 7 16 65 $80
  run_test 3 7 16 64 $83
  run_test 3 7 16 65 $80
  run_test 4 7 16 64 $83
  run_test 4 7 16 65 $80
  run_test 5 7 16 64 $83
  run_test 5 7 16 65 $80
  run_test 6 7 16 64 $83
  run_test 6 7 16 65 $80
  run_test 7 7 16 64 $83
  run_test 7 7 16 65 $80
  run_test 8 7 16 62 $83
  run_test 8 7 16 63 $80

  run_test 0 8 16 63 $83
  run_test 0 8 16 64 $80
  run_test 1 8 16 63 $83
  run_test 1 8 16 64 $80
  run_test 2 8 16 63 $83
  run_test 2 8 16 64 $80
  run_test 3 8 16 63 $83
  run_test 3 8 16 64 $80
  run_test 4 8 16 63 $83
  run_test 4 8 16 64 $80
  run_test 5 8 16 63 $83
  run_test 5 8 16 64 $80
  run_test 6 8 16 64 $83
  run_test 6 8 16 65 $80
  run_test 7 8 16 64 $83
  run_test 7 8 16 65 $80
  run_test 8 8 16 63 $83
  run_test 8 8 16 64 $80

  run_test 0 9 16 63 $83
  run_test 0 9 16 64 $80
  run_test 1 9 16 63 $83
  run_test 1 9 16 64 $80
  run_test 2 9 16 63 $83
  run_test 2 9 16 64 $80
  run_test 3 9 16 63 $83
  run_test 3 9 16 64 $80
  run_test 4 9 16 63 $83
  run_test 4 9 16 64 $80
  run_test 5 9 16 63 $83
  run_test 5 9 16 64 $80
  run_test 6 9 16 64 $83
  run_test 6 9 16 65 $80
  run_test 7 9 16 65 $83
  run_test 7 9 16 66 $80
  run_test 8 9 16 63 $83
  run_test 8 9 16 64 $80

  run_test 0 10 16 63 $83
  run_test 0 10 16 64 $80
  run_test 1 10 16 63 $83
  run_test 1 10 16 64 $80
  run_test 2 10 16 63 $83
  run_test 2 10 16 64 $80
  run_test 3 10 16 63 $83
  run_test 3 10 16 64 $80
  run_test 4 10 16 63 $83
  run_test 4 10 16 64 $80
  run_test 5 10 16 63 $83
  run_test 5 10 16 64 $80
  run_test 6 10 16 65 $83
  run_test 6 10 16 66 $80
  run_test 7 10 16 65 $83
  run_test 7 10 16 66 $80
  run_test 8 10 16 63 $83
  run_test 8 10 16 64 $80

  run_test 0 11 16 63 $83
  run_test 0 11 16 64 $80
  run_test 1 11 16 63 $83
  run_test 1 11 16 64 $80
  run_test 2 11 16 63 $83
  run_test 2 11 16 64 $80
  run_test 3 11 16 63 $83
  run_test 3 11 16 64 $80
  run_test 4 11 16 63 $83
  run_test 4 11 16 64 $80
  run_test 5 11 16 65 $83
  run_test 5 11 16 66 $80
  run_test 6 11 16 65 $83
  run_test 6 11 16 66 $80
  run_test 7 11 16 65 $83
  run_test 7 11 16 66 $80
  run_test 8 11 16 63 $83
  run_test 8 11 16 64 $80

  run_test 0 12 16 62 $83
  run_test 0 12 16 63 $80
  run_test 1 12 16 62 $83
  run_test 1 12 16 63 $80
  run_test 2 12 16 63 $83
  run_test 2 12 16 64 $80
  run_test 3 12 16 63 $83
  run_test 3 12 16 64 $80
  run_test 4 12 16 64 $83
  run_test 4 12 16 65 $80
  run_test 5 12 16 64 $83
  run_test 5 12 16 65 $80
  run_test 6 12 16 64 $83
  run_test 6 12 16 65 $80
  run_test 7 12 16 64 $83
  run_test 7 12 16 65 $80
  run_test 8 12 16 62 $83
  run_test 8 12 16 63 $80

  run_test 0 13 16 62 $83
  run_test 0 13 16 63 $80
  run_test 1 13 16 62 $83
  run_test 1 13 16 63 $80
  run_test 2 13 16 63 $83
  run_test 2 13 16 64 $80
  run_test 3 13 16 64 $83
  run_test 3 13 16 65 $80
  run_test 4 13 16 64 $83
  run_test 4 13 16 65 $80
  run_test 5 13 16 64 $83
  run_test 5 13 16 65 $80
  run_test 6 13 16 64 $83
  run_test 6 13 16 65 $80
  run_test 7 13 16 64 $83
  run_test 7 13 16 65 $80
  run_test 8 13 16 62 $83
  run_test 8 13 16 63 $80

  run_test 0 14 16 62 $83
  run_test 0 14 16 63 $80
  run_test 1 14 16 62 $83
  run_test 1 14 16 63 $80
  run_test 2 14 16 64 $83
  run_test 2 14 16 65 $80
  run_test 3 14 16 64 $83
  run_test 3 14 16 65 $80
  run_test 4 14 16 64 $83
  run_test 4 14 16 65 $80
  run_test 5 14 16 64 $83
  run_test 5 14 16 65 $80
  run_test 6 14 16 64 $83
  run_test 6 14 16 65 $80
  run_test 7 14 16 64 $83
  run_test 7 14 16 65 $80
  run_test 8 14 16 62 $83
  run_test 8 14 16 63 $80

  run_test 0 15 16 62 $83
  run_test 0 15 16 63 $80
  run_test 1 15 16 64 $83
  run_test 1 15 16 65 $80
  run_test 2 15 16 64 $83
  run_test 2 15 16 65 $80
  run_test 3 15 16 64 $83
  run_test 3 15 16 65 $80
  run_test 4 15 16 64 $83
  run_test 4 15 16 65 $80
  run_test 5 15 16 64 $83
  run_test 5 15 16 65 $80
  run_test 6 15 16 64 $83
  run_test 6 15 16 65 $80
  run_test 7 15 16 64 $83
  run_test 7 15 16 65 $80
  run_test 8 15 16 62 $83
  run_test 8 15 16 63 $80

  run_test 0 16 16 63 $83
  run_test 0 16 16 64 $80
  run_test 1 16 16 63 $83
  run_test 1 16 16 64 $80
  run_test 2 16 16 63 $83
  run_test 2 16 16 64 $80
  run_test 3 16 16 63 $83
  run_test 3 16 16 64 $80
  run_test 4 16 16 63 $83
  run_test 4 16 16 64 $80
  run_test 5 16 16 63 $83
  run_test 5 16 16 64 $80
  run_test 6 16 16 64 $83
  run_test 6 16 16 65 $80
  run_test 7 16 16 64 $83
  run_test 7 16 16 65 $80
  run_test 8 16 16 63 $83
  run_test 8 16 16 64 $80

  ld a, $55
- ld (VRAM), a
  jr -


fail:
  ld a, $FF
- ld (VRAM), a
  jr -
