//-------------------------------------------------------------------------------
// Memset / memcpy helpers

.macro memset_1024 ARGS base value
  ld hl, base
  ld a, value
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz, -
.endm

//-------------------------------------------------------------------------------
// Cycle counter helper. Returns a value that increments every cycle in A.

.macro cycle_init
  ld a, %101
  ldh (TAC), a
.endm

.macro cycle_count
  ld hl, $FF05
  xor a
  add (hl)
  nop
  add (hl)
  nop
  add (hl)
  nop
  add (hl)
.endm

.macro test_finish_cycle ARGS expected
  cycle_count
  test_finish_a expected
.endm

//------------------------------------------------------------------------------

.macro set_if
  ld a, $FF
  ldh (IF), a
.endm

.macro clear_if
  xor a
  ldh (IF), a
.endm

.macro clear_ie
  xor a
  ldh (IE), a
.endm

.macro set_ie_stat
  ld a, %00000010
  ldh (IE), a
.endm

.macro set_ie_vblank
  ld a, %00000001
  ldh (IE), a
.endm

.macro set_ie_timer
  ld a, %00000100
  ldh (IE), a
.endm

.macro set_stat_int_lyc
  ld a, %01000000
  ldh (STAT), a
.endm

.macro set_stat_int_oam
  ld a, %00100000
  ldh (STAT), a
.endm

.macro set_stat_int_vblank
  ld a, %00010000
  ldh (STAT), a
.endm

.macro set_stat_int_hblank
  ld a, %00001000
  ldh (STAT), a
.endm

//------------------------------------------------------------------------------

.macro lcd_off_unsafe
  ld a, $0
  ldh (LCDC), a
.endm

.macro lcd_on
  ld a, %10010001
  ldh (LCDC), a
.endm

.macro lcd_on_sprites
  ld a, %10010011
  ldh (LCDC), a
.endm

.macro lcd_on_win
  ld a, %10110001
  ldh (LCDC), a
.endm

.macro lcd_on_win_sprites
  ld a, %10110011
  ldh (LCDC), a
.endm

.macro reset_lcd_for_test
  di
  clear_stat
  clear_ie
  clear_if
  lcd_off_unsafe
  lcd_on
.endm

.macro clear_map0 ARGS value
  ld hl, $9800
  ld a, value
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz,-
.endm

.macro clear_map1 ARGS value
  ld hl, $9C00
  ld a, value
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz,-
.endm

//------------------------------------------------------------------------------
// Non-cycle-accurate wait for vblank

.macro wait_vblank
- ldh a, ($41)
  and 3
  cp 1
  jr z, -
- ldh a, ($41)
  and 3
  cp 1
  jr nz, -
.endm

// Non-cycle-accurate wait for hblank

.macro wait_hblank
- ldh a, ($41)
  and 3
  jr z, -
- ldh a, ($41)
  and 3
  jr nz, -
.endm

//------------------------------------------------------------------------------
// Put a box pattern in tile 0

.macro load_box 
  ld a, %01111110
  ld hl, $8002
.repeat 12
  ld (hl+), a
.endr
.endm

//------------------------------------------------------------------------------

.macro load_sprite ARGS i y x p f
  ld hl, $FE00 + 4 * i
  ld a, y
  ld (hl+), a
  ld a, x
  ld (hl+), a
  ld a, p
  ld (hl+), a
  ld a, f
  ld (hl+), a
.endm

.macro load_tile ARGS index addr
  ld bc, addr
  ld hl, $8000 + (index * 16)
.repeat 16
  ld a, (bc)
  ld (hl+), a
  inc bc
.endr
.endm

//------------------------------------------------------------------------------
// Short cycle-accurate delay w/ nops

.macro nops ARGS count
.repeat count
  nop
.endr
.endm

.macro incs ARGS count
.repeat count
  inc a
.endr
.endm

//------------------------------------------------------------------------------
// Long cycle-accurate delay w/ loop, clobbers ABC

.macro long_delay ARGS cycles
  xor a
  ld bc, ((cycles - 9) / 6) + 256
- dec bc
  cp b
  jr nz, -
  nops ((cycles - 9) # 6)
.endm

//------------------------------------------------------------------------------

.macro clear_stat
  xor a
  ldh (STAT), a
.endm

.macro skip_line
  nops 114
.endm

//------------------------------------------------------------------------------
// Test end macros. Writes actual value to FF80, expected value to FF81, sanity
// check value to FF82, then dumps a pass/fail pattern to vram.

.define PASS_PATTERN $01
.define FAIL_PATTERN $FF

.macro test_display_a
  ld hl, $8000
- ld (hl), a
  inc l
  jr -
.endm

.macro test_end2 ARGS expected3, result
  ldh ($80), a
  ld a, expected3
  ldh ($81), a
  ld a, result
  ldh ($82), a
  test_display_a
.endm

.macro test_pass
  test_end2 PASS_PATTERN PASS_PATTERN
.endm

.macro test_fail
  test_end2 FAIL_PATTERN FAIL_PATTERN
.endm

.macro test_finish_a ARGS expected2
  cp expected2
  jr z, +
  test_end2 expected2 FAIL_PATTERN
+ test_end2 expected2 PASS_PATTERN
.endm

.macro test_finish_not_a ARGS expected2
  cp expected2
  jr z, +
  test_end2 expected2 PASS_PATTERN
+ test_end2 expected2 FAIL_PATTERN
.endm

.macro test_assert_a ARGS expected2
  cp expected2
  jr z, +
  test_end2 expected2 FAIL_PATTERN
+ nop
.endm

//------------------------------------------------------------------------------
// Helpers for debugging tests

.macro test_display_cycle
  cycle_count
  test_display_a
.endm

.macro test_display_div
  ldh a, (DIV)
  test_display_a
.endm

//------------------------------------------------------------------------------
// Helpers for writing tests.

.macro test_finish_b ARGS expected1
  ld a, b
  test_finish_a expected1
.endm

.macro test_finish_div ARGS expected1
  ldh a, (DIV)
  test_finish_a expected1
.endm

.macro test_finish_stat ARGS expected1
  ldh a, (STAT)
  test_finish_a expected1
.endm

.macro test_finish_ly ARGS expected1
  ldh a, (LY)
  test_finish_a expected1
.endm

.macro test_finish_if ARGS expected1
  ldh a, (IF)
  test_finish_a expected1
.endm

//------------------------------------------------------------------------------
