.gbheader
  name "microtest"
  cartridgetype $03
  nintendologo
  romdmg
  ramsize 4
.endgb

.memorymap
  slot 0 start $0000 size $4000
  defaultslot 0 
.endme

// 2 meg rom with marker bytes at the beginning and end of each bank

.define NUM_ROM_BANKS 128

.rombanksize $4000
.rombanks NUM_ROM_BANKS

.repeat NUM_ROM_BANKS-1 INDEX bank
.bank bank+1
.org $0000
.db bank+1
.org $3FFF
.db bank+1
.endr

//----------------------------------------------------------------------------------------------------
// test app

.macro switch_bank ARGS bank
  ld a, bank & $1F
  ld ($2000), a
  ld a, (bank >> 5) & $03
  ld ($4000), a
.endm

.macro check_bank ARGS bank
  ld a, ($4000)
  cp a, bank
  jr z, 3
  jp test_fail
  //ld a, ($7FFF)
  //cp a, bank
  //jr z, 3
  //jp test_fail
.endm

//----------------------------------------------------------------------------------------------------

.bank 0

.org $100
  nop
  jp $150

.org $150
main:

  switch_bank 0
  check_bank 1

  switch_bank 1
  check_bank 1

  switch_bank 2
  check_bank 2

.repeat 31 INDEX b
  switch_bank b+1
  check_bank b+1
.endr

  switch_bank 32
  check_bank 33

.repeat 31 INDEX b
  switch_bank b+33
  check_bank b+33
.endr

  switch_bank 64
  check_bank 65

.repeat 31 INDEX b
  switch_bank b+65
  check_bank b+65
.endr

  switch_bank 96
  check_bank 97

.repeat 31 INDEX b
  switch_bank b+97
  check_bank b+97
.endr

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
