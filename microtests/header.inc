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
.org $100
	nop
	jp $150
.org $150

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

.macro lcd_off_unsafe
  ld a, $0
  ldh (LCDC), a
.endm

.macro lcd_on
  ld a, $91
  ldh (LCDC), a
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

.macro set_ie_stat
  ld a, %00000010
  ldh (IE), a
.endm

.macro set_vblank_int
  ld a, %00000001
  ldh (IE), a
.endm

.macro clear_if
  xor a
  ldh (IF), a
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
; Short delay w/ nops

.macro nops ARGS count
.repeat count
	nop
.endr
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

.macro test_finish_a ARGS expected
  sub <expected
  add $55
  test_end
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

;-------------------------------------------------------------------------------

.define VBLANK_INT_VECTOR $40
.define STAT_INT_VECTOR   $48

;-------------------------------------------------------------------------------
