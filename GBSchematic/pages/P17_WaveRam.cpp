#include "P17_WaveRam.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P17_WaveRam::tick(const Gameboy& a, const Gameboy& b, Gameboy& c ) {

  c.p17.BOKE = not(b.p09.CPU_RDn);
  c.p17.BENA = nand(b.p17.BOKE, b.p10.FF3X);
  c.p17.BYZA = and(b.p10.APU_WR, b.p10.FF3X);
  c.p17.CAZU = not(b.p17.BENA);
  c.p17.AMYT = not(b.p17.BYZA);

  //----------

  c.p17.BAMA = not(b.p09.APU_RESET1);
  c.p17.ARUC = not(b.p01.CLK_xBxDxFxH1);
  c.p17.CYBO = not(b.p01.CLK_xBxDxFxH1);
  c.p17.COZY = not(b.p01.CLK_xBxDxFxH1);

  c.p17.ABUR = not(b.p01.BUKE);
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
  c.p17.ALER = not(b.p17.ATUR);

  c.p17.BEKA = not(b.p17.BUTU_512K);
  c.p17.COJU = not(b.p17.BUTU_512K);
  c.p17.BAJA = not(b.p17.BUTU_512K);
  c.p17.BUFE = not(b.p17.BUTU_512K);

  //----------

  c.p17.AKAF = not(b.WAVE_RD_D7);
  c.p17.CUTO = not(b.WAVE_RD_D6);
  c.p17.BERO = not(b.WAVE_RD_D5);
  c.p17.BACA = not(b.WAVE_RD_D4);
  c.p17.ADOK = not(b.WAVE_RD_D3);
  c.p17.ATEC = not(b.WAVE_RD_D2);
  c.p17.CEGU = not(b.WAVE_RD_D1);
  c.p17.CUGO = not(b.WAVE_RD_D0);

  c.p17.BEZU = not(b.p17.AKAF);
  c.p17.DESA = not(b.p17.CUTO);
  c.p17.BAVA = not(b.p17.BERO);
  c.p17.BUNE = not(b.p17.BACA);
  c.p17.BADE = not(b.p17.ADOK);
  c.p17.BATY = not(b.p17.ATEC);
  c.p17.DESY = not(b.p17.CEGU);
  c.p17.DUGU = not(b.p17.CUGO);

  // uhhhh polarity? this is probably wrong...
  if (b.p17.WAVE_RAM_RDn) {
    c.D7 = b.p17.BEZU;
    c.D6 = b.p17.DESA;
    c.D5 = b.p17.BAVA;
    c.D4 = b.p17.BUNE;
    c.D3 = b.p17.BADE;
    c.D2 = b.p17.BATY;
    c.D1 = b.p17.DESY;
    c.D0 = b.p17.DUGU;
  }

  c.p17.ACOR = not(b.p09.APU_RESET1);

  c.p17.BEPA = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.BEPA, b.WAVE_RD_D7);
  c.p17.BORA = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.BORA, b.WAVE_RD_D6);
  c.p17.CEVO = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.CEVO, b.WAVE_RD_D5);
  c.p17.CUVO = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.CUVO, b.WAVE_RD_D4);
  c.p17.BEGU = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.BEGU, b.WAVE_RD_D3);
  c.p17.BUDY = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.BUDY, b.WAVE_RD_D2);
  c.p17.CESY = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.CESY, b.WAVE_RD_D1);
  c.p17.CYFO = tock_pos(a.p17.BUTU_512K, b.p17.BUTU_512K, b.p17.ACOR, b.p17.CYFO, b.WAVE_RD_D0);

  // why are these using the inverted output of the ff?
  c.p17.COPO = mux2(!b.p17.BEGU, !b.p17.BEPA, b.p18.EFAR);
  c.p17.CUZO = mux2(!b.p17.BUDY, !b.p17.BORA, b.p18.EFAR);
  c.p17.DAZY = mux2(!b.p17.CESY, !b.p17.CEVO, b.p18.EFAR);
  c.p17.DATE = mux2(!b.p17.CYFO, !b.p17.CUVO, b.p18.EFAR);
}