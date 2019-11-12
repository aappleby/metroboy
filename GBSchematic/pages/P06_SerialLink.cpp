#include "P06_SerialLink.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P06_SerialLink::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF02 SC

  c.p06.UWAM = nand(b.p07.CPU_WR, b.p06.SANO, b.p03.TOVY_A0n, b.A01); // ok these are _both_ nand?
  c.p06.UCOM = nand(b.p07.CPU_RD, b.p06.SANO, b.p03.TOVY_A0n, b.A01); // need to compare w/ other regs...

  c.p06.COTY = tock_pos(a.p01.CLK_16K, b.p01.CLK_16K, b.p06.UWAM, b.p06.COTY, !b.p06.COTY);

  c.p06.ETAF_07 = tock_pos(a.p06.UWAM, b.p06.UWAM, b.p06.CABY,       b.p06.ETAF_07, b.D7);
  c.p06.CULY_00 = tock_pos(a.p06.UWAM, b.p06.UWAM, b.p01.RESET2, b.p06.CULY_00, b.D0);

  c.p06.CORE_00 = not(!b.p06.CULY_00);
  c.p06.ELUV_07 = not(!b.p06.ETAF_07);

  c.p06.CARO = and(b.p06.UWAM, b.p01.RESET2);

  c.p06.CAVE = mux2(b.p06.COTY, b.chip.SCK_C, b.p06.CULY_00);
  c.p06.DAWA = or(b.p06.CAVE, !b.p06.ETAF_07);

  c.p06.JAGO = not(b.chip.SCK_B);
  c.p06.EDYL = not(b.p06.DAWA);

  c.p06.KEXU = nand(b.p06.DAWA, b.chip.SCK_B);
  c.p06.KUJO = nand(b.p06.JAGO, b.p06.DAWA);

  c.p06.CAFA_00 = tock_pos(a.p06.DAWA,     b.p06.DAWA,     b.p06.CARO, b.p06.CAFA_00, !b.p06.CAFA_00);
  c.p06.CYLO_01 = tock_pos(!a.p06.CAFA_00, !b.p06.CAFA_00, b.p06.CARO, b.p06.CYLO_01, !b.p06.CYLO_01);
  c.p06.CYDE_02 = tock_pos(!a.p06.CYLO_01, !b.p06.CYLO_01, b.p06.CARO, b.p06.CYDE_02, !b.p06.CYDE_02);
  c.p06.CALY_03 = tock_pos(!a.p06.CYDE_02, !b.p06.CYDE_02, b.p06.CARO, b.p06.CALY_03, !b.p06.CALY_03);

  c.p06.COBA = not(b.p06.CALY_03);
  c.p06.CABY = and(b.p06.COBA, b.p01.RESET2);

  c.chip.SCK_A = b.p06.KEXU;    // not in schematic, but it's there
  c.chip.SCK_B = b.p06.CULY_00; // SCK_DIR in schematic
  c.chip.SCK_D = b.p06.KUJO;    // not in schematic, but it's there

  if (b.p06.UCOM) {
    c.D0 = b.p06.CORE_00;
    c.D7 = b.p06.ELUV_07;
  }

  //----------

  c.p06.URYS = nand(b.p07.CPU_WR, b.p06.SANO, b.A00, b.p08.TOLA_A1n); // see, these are NAND/AND...
  c.p06.UFEG = and (b.p07.CPU_RD, b.p06.SANO, b.A00, b.p08.TOLA_A1n);

  c.p06.SARE = nor(b.A07, b.A06, b.A05, b.A04, b.A03);
  c.p06.SEFY = not(b.A02);
  c.p06.SANO = and(b.p06.SARE, b.p06.SEFY, b.p07.FFXX);
  c.p06.DAKU = not(b.p06.URYS);

  c.p06.EPYT = not(b.p06.SER_TICKn);
  c.p06.DEHO = not(b.p06.EPYT);
  c.p06.DAWE = not(b.p06.DEHO);
  c.p06.CAGE = not(b.chip.SIN_C); // check this


  //----------
  // FF01 SB

  c.p06.CUFU_00 = nand(b.D0, b.p06.DAKU);
  c.p06.DOCU_01 = nand(b.D1, b.p06.DAKU);
  c.p06.DELA_02 = nand(b.D2, b.p06.DAKU);
  c.p06.DYGE_03 = nand(b.D3, b.p06.DAKU);
  c.p06.DOLA_04 = nand(b.D4, b.p06.DAKU);
  c.p06.ELOK_05 = nand(b.D5, b.p06.DAKU);
  c.p06.EDEL_06 = nand(b.D6, b.p06.DAKU);
  c.p06.EFEF_07 = nand(b.D7, b.p06.DAKU);

  c.p06.COHY_00 = unk3(b.p06.URYS, b.D0, b.p01.RESET2);
  c.p06.DUMO_01 = unk3(b.p06.URYS, b.D1, b.p01.RESET2);
  c.p06.DYBO_02 = unk3(b.p06.URYS, b.D2, b.p01.RESET2);
  c.p06.DAJU_03 = unk3(b.p06.URYS, b.D3, b.p01.RESET2);
  c.p06.DYLY_04 = unk3(b.p06.URYS, b.D4, b.p01.RESET2);
  c.p06.EHUJ_05 = unk3(b.p06.URYS, b.D5, b.p01.RESET2);
  c.p06.EFAK_06 = unk3(b.p06.URYS, b.D6, b.p01.RESET2);
  c.p06.EGUV_07 = unk3(b.p06.URYS, b.D7, b.p01.RESET2);

  c.p06.CUBA_00 = srtock_pos(a.p06.DAWE, b.p06.DAWE, b.p06.CUFU_00, b.p06.COHY_00, b.p06.CUBA_00, b.p06.CAGE);
  c.p06.DEGU_01 = srtock_pos(a.p06.DAWE, b.p06.DAWE, b.p06.DOCU_01, b.p06.DUMO_01, b.p06.DEGU_01, b.p06.CUBA_00);
  c.p06.DYRA_02 = srtock_pos(a.p06.DAWE, b.p06.DAWE, b.p06.DELA_02, b.p06.DYBO_02, b.p06.DYRA_02, b.p06.DEGU_01);
  c.p06.DOJO_03 = srtock_pos(a.p06.DAWE, b.p06.DAWE, b.p06.DYGE_03, b.p06.DAJU_03, b.p06.DOJO_03, b.p06.DYRA_02);
  c.p06.DOVU_04 = srtock_pos(a.p06.EPYT, b.p06.EPYT, b.p06.DOLA_04, b.p06.DYLY_04, b.p06.DOVU_04, b.p06.DOJO_03);
  c.p06.EJAB_05 = srtock_pos(a.p06.EPYT, b.p06.EPYT, b.p06.ELOK_05, b.p06.EHUJ_05, b.p06.EJAB_05, b.p06.DOVU_04);
  c.p06.EROD_06 = srtock_pos(a.p06.EPYT, b.p06.EPYT, b.p06.EDEL_06, b.p06.EFAK_06, b.p06.EROD_06, b.p06.EJAB_05);
  c.p06.EDER_07 = srtock_pos(a.p06.EPYT, b.p06.EPYT, b.p06.EFEF_07, b.p06.EGUV_07, b.p06.EDER_07, b.p06.EROD_06);

  c.p06.CUGY_00 = not(!b.p06.CUBA_00);
  c.p06.DUDE_01 = not(!b.p06.DEGU_01);
  c.p06.DETU_02 = not(!b.p06.DYRA_02);
  c.p06.DASO_03 = not(!b.p06.DOJO_03);
  c.p06.DAME_04 = not(!b.p06.DOVU_04);
  c.p06.EVOK_05 = not(!b.p06.EJAB_05);
  c.p06.EFAB_06 = not(!b.p06.EROD_06);
  c.p06.ETAK_07 = not(!b.p06.EDER_07);

  if (b.p06.UFEG) {
    c.D0 = b.p06.CUGY_00;
    c.D1 = b.p06.DUDE_01;
    c.D2 = b.p06.DETU_02;
    c.D3 = b.p06.DASO_03;
    c.D4 = b.p06.DAME_04;
    c.D5 = b.p06.EVOK_05;
    c.D6 = b.p06.EFAB_06;
    c.D7 = b.p06.ETAK_07;
  }

  c.p06.ELYS = tock_pos(a.p06.SER_TICKn, b.p06.SER_TICKn, b.p01.RESET2, b.p06.ELYS, b.p06.EDER_07);
}
