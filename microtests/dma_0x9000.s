.include "header.inc"

main:
  // turn lcd off
  ld a, $00;
  ldh ($40), a;

  // copy sentinel bytes to ram
  ld a, $27;
  ld hl, $9000;
  ld (hl), a;
  ld a, $72;
  ld hl, $909F;
  ld (hl), a;

  // copy dma func to zram
  ld hl, $FF90;
  ld a, $E0;
  ld (hl+), a;
  ld a, $46;
  ld (hl+), a;
  ld a, $3E;
  ld (hl+), a;
  ld a, $28;
  ld (hl+), a;
  ld a, $3D;
  ld (hl+), a;
  ld a, $20;
  ld (hl+), a;
  ld a, $FD;
  ld (hl+), a;
  ld a, $C9;
  ld (hl+), a;

  // call dma func
  ld a, $90;
  call $FF90;

  // read endpoints of oam
  ld a, $00
  ld hl, $FE00;
  add (hl);
  ld hl, $FE9F;
  add (hl);
  ld b, a

  // turn lcd on
  ld a, $91
  ldh ($40), a

  // check result  
  ld a, b
  test_finish_a $99