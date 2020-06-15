.include "header.inc"

.define ST_A8_A  $E0
.define DMA_SRC  $FF
.define DMA_ECHO $DF
.define JR_A8    $18
.define NEG_2    $FE


main:
  // Turn on cart ram
  ld a, $0A
  ld ($0000), a

  // Stick a sentinel value at the beginning and end of the DMA source range
  ld a, $73
  ld h, DMA_ECHO
  ld l, $00
  ld (hl), a
  ld l, $9F
  ld (hl), a

  // Copy our dma code to zram
  ld h, $FF
  ld l, $80
  
  ld a, ST_A8_A
  ld (hl+), a

  ld a, $46
  ld (hl+), a

  ld a, JR_A8
  ld (hl+), a

  ld a, NEG_2
  ld (hl+), a

  // Start dma
  ld a, DMA_SRC
  jp $ff80
