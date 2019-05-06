.include "header.inc"

main:
  lcd_off_unsafe

  ld hl, $FDFF
  ld a, $77
  ld (hl+), a
  ld a, $3C
  ld (hl+), a
  ld (hl+), a

  lcd_on

  ld a, $80
  ld hl, $FF46
  jp $FDFF

.org $38
  test_finish_a $81
