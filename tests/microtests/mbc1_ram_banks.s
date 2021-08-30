.gbheader
  name "microtest"
  cartridgetype $03
  nintendologo
  romdmg
  ramsize 3
.endgb

.memorymap
  slot 0 start $0000 size $4000
  defaultslot 0 
.endme

.rombanksize $4000
.rombanks 2

//----------------------------------------------------------------------------------------------------

.bank 0

.org $100
  nop
  jp $150

.org $150
main:
  ld a, $0A
  ld ($0000), a
  ld a, $01
  ld ($6000), a

  //----------

  ld a, 0
  ld ($4000), a
  ld a, $11
  ld ($A000), a

  ld a, 1
  ld ($4000), a
  ld a, $22
  ld ($A000), a

  ld a, 2
  ld ($4000), a
  ld a, $33
  ld ($A000), a

  ld a, 3
  ld ($4000), a
  ld a, $44
  ld ($A000), a

  //----------

  ld a, 0
  ld ($4000), a
  ld a, ($A000)
  cp a, $11
  jr z, 3
  jp test_fail

  ld a, 1
  ld ($4000), a
  ld a, ($A000)
  cp a, $22
  jr z, 3
  jp test_fail

  ld a, 2
  ld ($4000), a
  ld a, ($A000)
  cp a, $33
  jr z, 3
  jp test_fail

  ld a, 3
  ld ($4000), a
  ld a, ($A000)
  cp a, $44
  jr z, 3
  jp test_fail

  //----------

  ld a, $01
  jp display_a

//----------------------------------------------------------------------------------------------------

test_fail:
  ld a, $FF
  jp display_a

//----------------------------------------------------------------------------------------------------

display_a:
  ldh ($80), a
  ldh ($81), a
  ldh ($82), a
  ld hl, $8000
- ld (hl), a
  inc l
  jr -

//----------------------------------------------------------------------------------------------------
