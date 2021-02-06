//------------------------------------------------------------------------------

.gbheader
	name "microtest"
	nintendologo
.endgb

.memorymap
  defaultslot 0 
  slot 0 start $0000 size $4000
  slot 1 start $4000 size $4000
.endme
.rombanksize $4000
.rombanks 2
.ramsize 2
.bank 0 slot 0

//------------------------------------------------------------------------------

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

.define VBLANK_INT_VECTOR $40
.define STAT_INT_VECTOR   $48
.define TIMER_INT_VECTOR  $50
.define SERIAL_INT_VECTOR $58
.define JOYPAD_INT_VECTOR $60

.include "macros.inc"

.org $100
  nop
  jp $150
.org $150