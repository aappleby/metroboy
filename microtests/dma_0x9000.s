.include "header.inc"

main:
  ld a, $00;
  ldh ($40), a;
  ld a, $27;
  ld hl, $9000;
  ld (hl), a;
  ld a, $72;
  ld hl, $909F;
  ld (hl), a;

  ld hl, $FF80;
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

  ld a, $90;
  call $ff80;

  ld a, $00;
  ld hl, $9000;
  add (hl);
  ld hl, $909F;
  add (hl);
  ld b, a
  ld a, $91
  ldh ($40), a
  ld a, b
  test_finish_a $99