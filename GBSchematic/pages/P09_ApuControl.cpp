#include "P09_ApuControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P09_ApuControl::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p09.CPU_RDn = not(b.p07.CPU_RD);
  c.p09.CPU_RD1 = not(b.p09.CPU_RDn);

  //---------
  // reset tree

  c.p09.RESETn1     = not(b.p01.RESET2);
  c.p09.RESET1      = not(b.p09.RESETn1);
  
  c.p09.APU_RESET2  = or(b.p09.RESETn1, !b.p09.ALL_SOUND_ON); // I don't think this is right...
  c.p09.APU_RESETn6 = not(b.p09.APU_RESET2);
  c.p09.APU_RESETn7 = not(b.p09.APU_RESET2);
  c.p09.APU_RESET1  = not(b.p09.APU_RESETn7);

  c.p09.APU_RESETn  = not(b.p09.APU_RESET1);
  c.p09.APU_RESETn2 = not(b.p09.APU_RESET1);
  c.p09.APU_RESETn3 = not(b.p09.APU_RESET1);
  c.p09.APU_RESETn4 = not(b.p09.APU_RESET1);
  c.p09.APU_RESETn5 = not(b.p09.APU_RESET1);

  //----------
  // clock dividers

  c.p09.AJER = tock_pos(a.p01.CLK_AxCxExGx1, b.p01.CLK_AxCxExGx1, b.p09.APU_RESETn3, b.p09.AJER, !b.p09.AJER);

  c.p09.BATA = not(b.p09.AJER_2M);
  c.p09.CALO = tock_pos(a.p09.BATA, b.p09.BATA, b.p09.APU_RESETn, b.p09.CALO, !b.p09.CALO);
  c.p09.DYFA = not(!b.p09.CALO);

  //----------
  // main logic chunk

  c.p09.NR52_WR1  = and(b.p10.FF26, b.p10.APU_WR);
  c.p09.NR52_RDn1 = nand(b.p10.FF26, b.p09.CPU_RD1);
  c.p09.NR52_WRn1 = nand(b.p10.FF26, b.p10.APU_WR);
  c.p09.NR52_WRn2 = nand(b.p10.FF26, b.p10.APU_WR);
  c.p09.NR52_WRn3 = not(b.p09.NR52_WR1);


  c.p09.EDEK = not(!b.p09.FERO);

  c.p09.NR52_7 = tock_pos(a.p09.NR52_WRn1, b.p09.NR52_WRn1, b.p09.RESET1,      b.p09.NR52_7, b.D7);

  // these must be debug-related and are only readable during apu reset...
  c.p09.EFOP = and(b.D4, b.p07.T1nT2);
  c.p09.NR52_5 = tock_pos(a.p09.NR52_WRn2, b.p09.NR52_WRn2, b.p09.APU_RESETn6, b.p09.NR52_5, b.D5);
  c.p09.FERO   = tock_pos(a.p09.NR52_WRn3, b.p09.NR52_WRn3, b.p09.APU_RESETn6, b.p09.FERO,   b.p09.EFOP);
  c.p09.BAZA   = tock_pos(a.p09.AJER_2M,   b.p09.AJER_2M,   b.p09.APU_RESETn3, b.p09.BAZA,   b.p09.NR52_5);

  c.p09.CELY = mux2(b.p09.BAZA, b.p01.BYFE_128, b.p09.NET03);
  c.p09.CONE = not(b.p09.CELY);
  c.p09.CATE = not(b.p09.CONE);

  c.p09.NR52_7b = not(!b.p09.NR52_7);

  if (b.p09.NR52_RDn1) {
    c.D7 = b.p09.NR52_7b;
  }

  // where's the read for the other bits?

  //----------
  // FF24 NR50

  c.p09.FF24n     = not(b.p10.FF24);
  c.p09.NR50_RDn1 = nor(b.p09.CPU_RDn, b.p09.FF24n);
  c.p09.NR50_RD1  = not(b.p09.BEFU);
  
  c.p09.NR50_WRn1 = nand(b.p10.FF24, b.p10.APU_WR);
  c.p09.NR50_WR1  = not(b.p09.NR50_WRn1);
  c.p09.NR50_WR2  = not(b.p09.NR50_WRn1);

  c.p09.NR50_WRn2 = not(b.p09.NR50_WR1);
  c.p09.NR50_WRn3 = not(b.p09.NR50_WR2);

  c.p09.NR50_0 = tock_pos(a.p09.NR50_WRn3, b.p09.NR50_WRn3, b.p09.APU_RESET2, c.p09.NR50_0, b.D0);
  c.p09.NR50_1 = tock_pos(a.p09.NR50_WRn3, b.p09.NR50_WRn3, b.p09.APU_RESET2, c.p09.NR50_1, b.D1);
  c.p09.NR50_2 = tock_pos(a.p09.NR50_WRn3, b.p09.NR50_WRn3, b.p09.APU_RESET2, c.p09.NR50_2, b.D2);
  c.p09.NR50_3 = tock_pos(a.p09.NR50_WRn3, b.p09.NR50_WRn3, b.p09.APU_RESET2, c.p09.NR50_3, b.D3);
  c.p09.NR50_4 = tock_pos(a.p09.NR50_WRn2, b.p09.NR50_WRn2, b.p09.APU_RESET2, c.p09.NR50_4, b.D4);
  c.p09.NR50_5 = tock_pos(a.p09.NR50_WRn2, b.p09.NR50_WRn2, b.p09.APU_RESET2, c.p09.NR50_5, b.D5);
  c.p09.NR50_6 = tock_pos(a.p09.NR50_WRn2, b.p09.NR50_WRn2, b.p09.APU_RESET2, c.p09.NR50_6, b.D6);
  c.p09.NR50_7 = tock_pos(a.p09.NR50_WRn2, b.p09.NR50_WRn2, b.p09.APU_RESET2, c.p09.NR50_7, b.D7);

  c.p09.NR50_D0 = not(!b.p09.NR50_0);
  c.p09.NR50_D1 = not(!b.p09.NR50_1);
  c.p09.NR50_D2 = not(!b.p09.NR50_2);
  c.p09.NR50_D3 = not(!b.p09.NR50_3);
  c.p09.NR50_D4 = not(!b.p09.NR50_4);
  c.p09.NR50_D5 = not(!b.p09.NR50_5);
  c.p09.NR50_D6 = not(!b.p09.NR50_6);
  c.p09.NR50_D7 = not(!b.p09.NR50_7);

  if (b.p09.NR50_RD1) {
    c.D0 = b.p09.NR50_D0;
    c.D1 = b.p09.NR50_D1;
    c.D2 = b.p09.NR50_D2;
    c.D3 = b.p09.NR50_D3;
    c.D4 = b.p09.NR50_D4;
    c.D5 = b.p09.NR50_D5;
    c.D6 = b.p09.NR50_D6;
    c.D7 = b.p09.NR50_D7;
  }

  //----------
  // FF25 NR51

  c.p09.BUPO = nand(b.p10.FF25, b.p10.APU_WR);
  c.p09.BONO = not(b.p09.BUPO);
  c.p09.BYFA = not(b.p09.BUPO);
  c.p09.GEPA = not(b.p10.FF25);
  c.p09.HEFA = nor(b.p09.GEPA, b.p09.CPU_RDn);
  c.p09.GUMU = not(b.p09.HEFA);

  c.p09.NR51_0 = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.APU_RESET2, c.p09.NR51_0, b.D0);
  c.p09.NR51_1 = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.APU_RESET2, c.p09.NR51_1, b.D1);
  c.p09.NR51_2 = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.APU_RESET2, c.p09.NR51_2, b.D2);
  c.p09.NR51_3 = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.APU_RESET2, c.p09.NR51_3, b.D3);
  c.p09.NR51_4 = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.APU_RESET2, c.p09.NR51_4, b.D4);
  c.p09.NR51_5 = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.APU_RESET2, c.p09.NR51_5, b.D5);
  c.p09.NR51_6 = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.APU_RESET2, c.p09.NR51_6, b.D6);
  c.p09.NR51_7 = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.APU_RESET2, c.p09.NR51_7, b.D7);

  c.p09.NR51_D0 = not(!b.p09.BOGU);
  c.p09.NR51_D1 = not(!b.p09.BAFO);
  c.p09.NR51_D2 = not(!b.p09.ATUF);
  c.p09.NR51_D3 = not(!b.p09.ANEV);
  c.p09.NR51_D4 = not(!b.p09.BEPU);
  c.p09.NR51_D5 = not(!b.p09.BEFO);
  c.p09.NR51_D6 = not(!b.p09.BUME);
  c.p09.NR51_D7 = not(!b.p09.BOFA);

  if (b.p09.GUMU) {
    c.D1 = b.p09.NR51_D0;
    c.D2 = b.p09.NR51_D1;
    c.D3 = b.p09.NR51_D2;
    c.D0 = b.p09.NR51_D3;
    c.D7 = b.p09.NR51_D4;
    c.D6 = b.p09.NR51_D5;
    c.D4 = b.p09.NR51_D6;
    c.D5 = b.p09.NR51_D7;
  }

  //----------
  // FF26 NR52

  c.p09.CETO = not(b.p09.CPU_RDn);
  c.p09.KAZO = not(b.p09.CPU_RDn);
  c.p09.CURU = not(b.p09.CPU_RDn);
  c.p09.GAXO = not(b.p09.CPU_RDn);

  c.p09.DOLE = nand(b.p10.FF26, b.p09.CETO);
  c.p09.KAMU = nand(b.p10.FF26, b.p09.KAZO);
  c.p09.DURU = nand(b.p10.FF26, b.p09.CURU);
  c.p09.FEWA = nand(b.p10.FF26, b.p09.GAXO);

  c.p09.COTO = not(b.p13.CH1_ACTIVEn);
  c.p09.EFUS = not(b.p15.CH2_ACTIVEn);
  c.p09.FATE = not(b.p18.CH3_ACTIVEn);
  c.p09.KOGE = not(b.p20.CH4_ACTIVEn);

  if (b.p09.DOLE) { c.D0 = b.p09.COTO; }
  if (b.p09.DURU) { c.D1 = b.p09.EFUS; }
  if (b.p09.FEWA) { c.D2 = b.p09.FATE; }
  if (b.p09.KAMU) { c.D3 = b.p09.KOGE; }
}