; Game Boy hardware addresses

; Memory
.define VRAM   $8000 ; video memory
.define TILES  $8000 ; tile images
.define BGMAP0 $9800 ; first 32x32 tilemap
.define BGMAP1 $9C00 ; second 32x32 tilemap
.define WRAM   $C000 ; internal memory
.define OAM    $FE00 ; sprite memory
.define HRAM   $FF80 ; fast memory for LDH

.define P1     $FF00

; Game link I/O
.define SB     $FF01
.define SC     $FF02

; Interrupts
.define DIV    $FF04
.define TIMA   $FF05
.define TMA    $FF06
.define TAC    $FF07
.define IF     $FF0F
.define IE     $FFFF

; LCD registers
.define LCDC   $FF40 ; control
.define STAT   $FF41 ; status
.define SCY    $FF42 ; scroll Y
.define SCX    $FF43 ; scroll X
.define LY     $FF44 ; current Y being rendered
.define BGP    $FF47

.define KEY1   $FF4D ; for changing CPU speed
.define VBK    $FF4F

; Sound registers
.define NR10   $FF10
.define NR11   $FF11
.define NR12   $FF12
.define NR13   $FF13
.define NR14   $FF14

.define NR21   $FF16
.define NR22   $FF17
.define NR23   $FF18
.define NR24   $FF19

.define NR30   $FF1A
.define NR31   $FF1B
.define NR32   $FF1C
.define NR33   $FF1D
.define NR34   $FF1E

.define NR41   $FF20
.define NR42   $FF21
.define NR43   $FF22
.define NR44   $FF23

.define NR50   $FF24
.define NR51   $FF25
.define NR52   $FF26

.define WAVE   $FF30
