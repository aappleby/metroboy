;-------------------------------------------------------------------------------

.gbheader
	name "microtest"
	nintendologo
.endgb

.memorymap
  defaultslot 0 
  slot 0 start $0000 size $4000
.endme
.rombanksize $4000
.rombanks 2

.bank 0 slot 0

;-------------------------------------------------------------------------------

.define VRAM $8000

.define DIV  $04
.define TIMA $05
.define TMA  $06
.define TAC  $07

.define LCDC $40
.define STAT $41
.define SCY  $42
.define SCX  $43
.define LY   $44
.define LYC  $45
.define DMA  $46
.define BGP  $47
.define OBP0 $48
.define OBP1 $49
.define WY   $4A
.define WX   $4B

.define IF   $0F
.define IE   $FF

;-------------------------------------------------------------------------------

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

;-------------------------------------------------------------------------------

.macro clear_stat
  xor a
  ldh (STAT), a
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

;-------------------------------------------------------------------------------

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

;-------------------------------------------------------------------------------
; Non-cycle-accurate wait for vblank

.macro wait_vblank
-	ldh a, ($41)
	and 3
	cp 1
	jr z, -
-	ldh a, ($41)
	and 3
	cp 1
	jr nz, -
.endm

; Non-cycle-accurate wait for hblank

.macro wait_hblank
-	ldh a, ($41)
	and 3
	jr z, -
-	ldh a, ($41)
	and 3
	jr nz, -
.endm

;-------------------------------------------------------------------------------
; Put a box pattern in tile 0

.macro load_box 
	ld a, %01111110
	ld hl, $8002
.repeat 12
	ld (hl+), a
.endr
.endm

;-------------------------------------------------------------------------------

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

.macro memset_1024 ARGS base index
  ld hl, base
  ld a, 0
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz, -
.endm

;-------------------------------------------------------------------------------
; Short delay w/ nops

.macro nops ARGS count
.repeat count
	nop
.endr
.endm

;-------------------------------------------------------------------------------
; Long delay w/ loop, clobbers ABC

.macro long_delay ARGS cycles
  xor a
  ld bc, ((cycles - 9) / 6) + 256
- dec bc
  cp b
  jr nz, -
  nops ((cycles - 9) # 6)
.endm

;-------------------------------------------------------------------------------

.macro reset_lcd_for_test
  di
  clear_stat
  clear_ie
  clear_if
  lcd_off_unsafe
  lcd_on
.endm

.macro skip_line
  nops 114
.endm

;-------------------------------------------------------------------------------
; Report test failure

; Report test pass if a == expected

.macro test_end
- ld (VRAM), a
  jr -
.endm

.macro test_pass
  ld a, $55
  test_end
.endm

.macro test_fail
  ld a, $FF
  test_end
.endm

.macro display_a
- ld (VRAM), a
  jr -
.endm

.macro test_finish_a ARGS expected
  cp expected
  jr z, +
  ld a, $FF
- ld (VRAM), a
  jr -

+ ld a, $55
- ld (VRAM), a
  jr -
.endm

.macro test_finish_b ARGS expected
  ld a, b
  sub <expected
  add $55
  test_end
.endm

.macro test_finish_div ARGS expected
  ldh a, (DIV)
  test_finish_a expected
.endm

.macro test_finish_stat ARGS expected
  ldh a, (STAT)
  test_finish_a expected
.endm

.macro test_finish_ly ARGS expected
  ldh a, (LY)
  test_finish_a expected
.endm

.macro test_finish_if ARGS expected
  ldh a, (IF)
  test_finish_a expected
.endm

;-------------------------------------------------------------------------------

.define VBLANK_INT_VECTOR $40
.define STAT_INT_VECTOR   $48

;-------------------------------------------------------------------------------

.org $100
  nop
  jp $150
.org $150