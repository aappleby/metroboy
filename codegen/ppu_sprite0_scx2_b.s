
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

.macro nops ARGS count
.repeat count
  nop
.endr
.endm

.org $100
  nop
  jp $150

.org $0150
  ld a, 0
  ldh ($40), a
  ld hl, $fe00
  ld a, 32
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 2
  ldh ($43), a
  ld a, 147
  ldh ($40), a
  nops 177
  ldh a, ($41)
  cp 128
  jr z, 7
  ld a, 255
  ld ($8000), a
  jr -5
  ld a, 85
  ld ($8000), a
  jr -5
