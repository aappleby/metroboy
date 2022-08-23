.include "header.inc"

main:
  load_hi $ff24 $77  // vol 7/7
  load_hi $ff25 $FF  // all enable l/r
  load_hi $ff26 $80  // enable spu

  // ch1
  //load_hi $ff10 $00  // sweep period, sweep negate, sweep shift
  //load_hi $ff11 $80  // duty len
  //load_hi $ff12 $0F  // vol, env add, env period
  //load_hi $ff13 $0F  // freq lo
  //load_hi $ff14 $87  // trigger, len en, freq hi

  // ch4
  load_hi $ff1f $00  // unused
  load_hi $ff20 $00  // 64-len
  load_hi $ff21 $F0  // vol, env add, env period
  load_hi $ff22 $5F  // clock mux, lfsr mode, lfsr divisor
  load_hi $ff23 $87  // trigger, len en, freq hi

  jr -2