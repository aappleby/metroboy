; pass - dmg

.include "header.inc"

; l0
; 0   - E2
; 16  - E2
; 32  - E2
; 48  - E2
; 64  - E2
; 80  - E2
; 96  - E2
; 109 - E2
; 110 - E0

; l1
; 58  - E0
; 59  - E2
; 109 - E2
; 110 - E0

; l143
; 58  - E0
; 59  - E2
; 100 - E2
; 106 - E2
; 107 - E3

; 114 * 11 + 110 - E1

main:
  reset_lcd_for_test

  long_delay 114 * 154

  nops 110
  
  clear_stat
  test_finish_if $E0