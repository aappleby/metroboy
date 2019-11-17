#include "P17_WaveRam.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P17_WaveRam::tick(const Gameboy& a, const Gameboy& b, Gameboy& c ) {

  c.p17.BOKE = not(b.apu.CPU_RDn);
  c.p17.BENA = nand(b.p17.BOKE, b.apu.ADDR_FF3X);

  c.p17.WAVE_RAM_WR  = and(b.apu.APU_WR, b.apu.ADDR_FF3X);
  c.p17.WAVE_RAM_RDn = not(b.p17.BENA);
  c.p17.WAVE_RAM_WRn = not(b.p17.WAVE_RAM_WR);

  //----------

  c.p17.BORY = not(b.p17.ABUR);
  c.p17.BETA = or(b.p17.WAVE_RAM_WR, b.p17.WAVE_RAM_RDn, b.p17.BORY);
  c.p17.AZOR = not(b.p17.BETA);
  c.p17.BUKU = not(b.p17.AZOR);

  // Schematic makes this look like a clock divider, but it's a 4 bit shift register.
  c.p17.BUSA = tock_pos(a.p01.CLK_xBxDxFxH1, b.p01.CLK_xBxDxFxH1, b.p17.BAMA, b.p17.BUSA, b.p18.GASE);
  c.p17.BANO = tock_pos(a.p17.COZY,          b.p17.COZY,          b.p17.BAMA, b.p17.BANO, b.p17.BUSA);
  c.p17.AZUS = tock_pos(a.p01.CLK_xBxDxFxH1, b.p01.CLK_xBxDxFxH1, b.p17.BAMA, b.p17.AZUS, b.p17.BANO);
  c.p17.AZET = tock_pos(a.p17.ARUC,          b.p17.ARUC,          b.p17.BAMA, b.p17.AZET, b.p17.AZUS);

  c.p17.BUTU = not(b.p17.AZUS);

  c.p17.BOXO = nor(b.p17.AZUS, b.p17.AZET);
  c.p17.BORU = not(b.p17.BOXO);

  c.p17.ATUR = mux2(b.p17.BORU, b.p17.BUKU, b.p18.CH3_ACTIVE);
  c.p17.WAVE_RAM_CTRL1 = not(b.p17.ATUR);

  c.p17.BEKA = not(b.p17.BUTU); // where does this go?
  c.p17.COJU = not(b.p17.BUTU);
  c.p17.BAJA = not(b.p17.BUTU);
  c.p17.BUFE = not(b.p17.BUTU);

  //----------

  /*p17.CUGO*/ c.p17.WAVE_D0n = not(b.WAVE_D0);
  /*p17.CEGU*/ c.p17.WAVE_D1n = not(b.WAVE_D1);
  /*p17.ATEC*/ c.p17.WAVE_D2n = not(b.WAVE_D2);
  /*p17.ADOK*/ c.p17.WAVE_D3n = not(b.WAVE_D3);
  /*p17.BACA*/ c.p17.WAVE_D4n = not(b.WAVE_D4);
  /*p17.BERO*/ c.p17.WAVE_D5n = not(b.WAVE_D5);
  /*p17.CUTO*/ c.p17.WAVE_D6n = not(b.WAVE_D6);
  /*p17.AKAF*/ c.p17.WAVE_D7n = not(b.WAVE_D7);

  /*p17.DUGU*/ c.p17.WAVE_D0 = not(b.p17.WAVE_D0n);
  /*p17.DESY*/ c.p17.WAVE_D1 = not(b.p17.WAVE_D1n);
  /*p17.BATY*/ c.p17.WAVE_D2 = not(b.p17.WAVE_D2n);
  /*p17.BADE*/ c.p17.WAVE_D3 = not(b.p17.WAVE_D3n);
  /*p17.BUNE*/ c.p17.WAVE_D4 = not(b.p17.WAVE_D4n);
  /*p17.BAVA*/ c.p17.WAVE_D5 = not(b.p17.WAVE_D5n);
  /*p17.DESA*/ c.p17.WAVE_D6 = not(b.p17.WAVE_D6n);
  /*p17.BEZU*/ c.p17.WAVE_D7 = not(b.p17.WAVE_D7n);

  // uhhhh polarity? this is probably wrong...
  if (b.p17.WAVE_RAM_RDn) {
    c.D7 = b.p17.WAVE_D0;
    c.D6 = b.p17.WAVE_D1;
    c.D5 = b.p17.WAVE_D2;
    c.D4 = b.p17.WAVE_D3;
    c.D3 = b.p17.WAVE_D4;
    c.D2 = b.p17.WAVE_D5;
    c.D1 = b.p17.WAVE_D6;
    c.D0 = b.p17.WAVE_D7;
  }

  /*p17.CYFO*/ c.p17.SAMPLE_0 = tock_pos(a.p17.BEKA, b.p17.BEKA, b.p17.ACOR, b.p17.SAMPLE_0, b.WAVE_D0);
  /*p17.CESY*/ c.p17.SAMPLE_1 = tock_pos(a.p17.BUFE, b.p17.BUFE, b.p17.ACOR, b.p17.SAMPLE_1, b.WAVE_D1);
  /*p17.BUDY*/ c.p17.SAMPLE_2 = tock_pos(a.p17.BAJA, b.p17.BAJA, b.p17.ACOR, b.p17.SAMPLE_2, b.WAVE_D2);
  /*p17.BEGU*/ c.p17.SAMPLE_3 = tock_pos(a.p17.COJU, b.p17.COJU, b.p17.ACOR, b.p17.SAMPLE_3, b.WAVE_D3);
  /*p17.CUVO*/ c.p17.SAMPLE_4 = tock_pos(a.p17.BEKA, b.p17.BEKA, b.p17.ACOR, b.p17.SAMPLE_4, b.WAVE_D4);
  /*p17.CEVO*/ c.p17.SAMPLE_5 = tock_pos(a.p17.BUFE, b.p17.BUFE, b.p17.ACOR, b.p17.SAMPLE_5, b.WAVE_D5);
  /*p17.BORA*/ c.p17.SAMPLE_6 = tock_pos(a.p17.BAJA, b.p17.BAJA, b.p17.ACOR, b.p17.SAMPLE_6, b.WAVE_D6);
  /*p17.BEPA*/ c.p17.SAMPLE_7 = tock_pos(a.p17.COJU, b.p17.COJU, b.p17.ACOR, b.p17.SAMPLE_7, b.WAVE_D7);

  c.p17.WAVE_PLAY_D0 = mux2n(!b.p17.SAMPLE_0, !b.p17.SAMPLE_4, b.p18.WAVE_IDX0);
  c.p17.WAVE_PLAY_D1 = mux2n(!b.p17.SAMPLE_1, !b.p17.SAMPLE_5, b.p18.WAVE_IDX0);
  c.p17.WAVE_PLAY_D3 = mux2n(!b.p17.SAMPLE_2, !b.p17.SAMPLE_6, b.p18.WAVE_IDX0);
  c.p17.WAVE_PLAY_D2 = mux2n(!b.p17.SAMPLE_3, !b.p17.SAMPLE_7, b.p18.WAVE_IDX0);

}
  