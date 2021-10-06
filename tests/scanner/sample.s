
  .include "header.inc"

  main:
    reset_lcd_for_test
    long_delay (114 + (114 * 154) - 7)
    ld a, $55
    ld ($9000), a

    lcd_off_unsafe
    ld a, ($9000)
    ld b, a
    lcd_on
    ld a, b
    test_display_a
  