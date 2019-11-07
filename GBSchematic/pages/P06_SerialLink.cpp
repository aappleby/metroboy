#include "P06_SerialLink.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P06_SerialLink::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P06_SerialLink& pa = ga.p06;
  const P06_SerialLink& pb = gb.p06;
  P06_SerialLink& pc = gc.p06;

  //----------
  // FF02 SC

  pc.UWAM = nand(gb.CPU_WR, pb.SANO, gb.TOVY_A0n, gb.cpu.A1); // ok these are _both_ nand?
  pc.UCOM = nand(gb.CPU_RD, pb.SANO, gb.TOVY_A0n, gb.cpu.A1); // need to compare w/ other regs...

  pc.COTY = tock_pos(ga.CLK_16K, gb.CLK_16K, pb.UWAM, pb.COTY, !pb.COTY);

  pc.ETAF_07 = tock_pos(pa.UWAM, pb.UWAM, pb.CABY,   pb.ETAF_07, gb.cpu.D7);
  pc.CULY_00 = tock_pos(pa.UWAM, pb.UWAM, gb.RESET2, pb.CULY_00, gb.cpu.D0);

  pc.CORE_00 = not(!pb.CULY_00);
  pc.ELUV_07 = not(!pb.ETAF_07);

  pc.CARO = and(pb.UWAM, gb.RESET2);

  pc.CAVE = mux2(pb.COTY, gb.chip.SCK_C, pb.CULY_00);
  pc.DAWA = or(pb.CAVE, !pb.ETAF_07);

  pc.JAGO = not(gb.chip.SCK_B);
  pc.EDYL = not(pb.DAWA);

  pc.KEXU = nand(pb.DAWA, gb.chip.SCK_B);
  pc.KUJO = nand(pb.JAGO, pb.DAWA);

  pc.CAFA_00 = tock_pos(pa.DAWA,     pb.DAWA,     pb.CARO, pb.CAFA_00, !pb.CAFA_00);
  pc.CYLO_01 = tock_pos(!pa.CAFA_00, !pb.CAFA_00, pb.CARO, pb.CYLO_01, !pb.CYLO_01);
  pc.CYDE_02 = tock_pos(!pa.CYLO_01, !pb.CYLO_01, pb.CARO, pb.CYDE_02, !pb.CYDE_02);
  pc.CALY_03 = tock_pos(!pa.CYDE_02, !pb.CYDE_02, pb.CARO, pb.CALY_03, !pb.CALY_03);

  pc.COBA = not(pb.CALY_03);
  pc.CABY = and(pb.COBA, gb.RESET2);

  gc.chip.SCK_A = pb.KEXU;    // not in schematic, but it's there
  gc.chip.SCK_B = pb.CULY_00; // SCK_DIR in schematic
  gc.chip.SCK_D = pb.KUJO;    // not in schematic, but it's there

  gc.INT_SERIAL = pb.CALY_03;
  gc.SER_TICKn  = pb.EDYL;

  if (pb.UCOM) {
    gc.cpu.D0 = pb.CORE_00;
    gc.cpu.D7 = pb.ELUV_07;
  }

  //----------

  pc.URYS = nand(gb.CPU_WR, pb.SANO, gb.cpu.A0, gb.TOLA_A1n); // see, these are NAND/AND...
  pc.UFEG = and (gb.CPU_RD, pb.SANO, gb.cpu.A0, gb.TOLA_A1n);

  pc.SARE = nor(gb.cpu.A7, gb.cpu.A6, gb.cpu.A5, gb.cpu.A4, gb.cpu.A3);
  pc.SEFY = not(gb.cpu.A2);
  pc.SANO = and(pb.SARE, pb.SEFY, gb.FFXX);
  pc.DAKU = not(pb.URYS);

  pc.EPYT = not(gb.SER_TICKn);
  pc.DEHO = not(pb.EPYT);
  pc.DAWE = not(pb.DEHO);
  pc.CAGE = not(gb.chip.SIN_C); // check this


  gc.A00_07 = pb.SARE;

  //----------
  // FF01 SB

  pc.CUFU_00 = nand(gb.cpu.D0, pb.DAKU);
  pc.DOCU_01 = nand(gb.cpu.D1, pb.DAKU);
  pc.DELA_02 = nand(gb.cpu.D2, pb.DAKU);
  pc.DYGE_03 = nand(gb.cpu.D3, pb.DAKU);
  pc.DOLA_04 = nand(gb.cpu.D4, pb.DAKU);
  pc.ELOK_05 = nand(gb.cpu.D5, pb.DAKU);
  pc.EDEL_06 = nand(gb.cpu.D6, pb.DAKU);
  pc.EFEF_07 = nand(gb.cpu.D7, pb.DAKU);

  pc.COHY_00 = unk3(pb.URYS, gb.cpu.D0, gb.RESET2);
  pc.DUMO_01 = unk3(pb.URYS, gb.cpu.D1, gb.RESET2);
  pc.DYBO_02 = unk3(pb.URYS, gb.cpu.D2, gb.RESET2);
  pc.DAJU_03 = unk3(pb.URYS, gb.cpu.D3, gb.RESET2);
  pc.DYLY_04 = unk3(pb.URYS, gb.cpu.D4, gb.RESET2);
  pc.EHUJ_05 = unk3(pb.URYS, gb.cpu.D5, gb.RESET2);
  pc.EFAK_06 = unk3(pb.URYS, gb.cpu.D6, gb.RESET2);
  pc.EGUV_07 = unk3(pb.URYS, gb.cpu.D7, gb.RESET2);

  pc.CUBA_00 = srtock_pos(pa.DAWE, pb.DAWE, pb.CUFU_00, pb.COHY_00, pb.CUBA_00, pb.CAGE);
  pc.DEGU_01 = srtock_pos(pa.DAWE, pb.DAWE, pb.DOCU_01, pb.DUMO_01, pb.DEGU_01, pb.CUBA_00);
  pc.DYRA_02 = srtock_pos(pa.DAWE, pb.DAWE, pb.DELA_02, pb.DYBO_02, pb.DYRA_02, pb.DEGU_01);
  pc.DOJO_03 = srtock_pos(pa.DAWE, pb.DAWE, pb.DYGE_03, pb.DAJU_03, pb.DOJO_03, pb.DYRA_02);
  pc.DOVU_04 = srtock_pos(pa.EPYT, pb.EPYT, pb.DOLA_04, pb.DYLY_04, pb.DOVU_04, pb.DOJO_03);
  pc.EJAB_05 = srtock_pos(pa.EPYT, pb.EPYT, pb.ELOK_05, pb.EHUJ_05, pb.EJAB_05, pb.DOVU_04);
  pc.EROD_06 = srtock_pos(pa.EPYT, pb.EPYT, pb.EDEL_06, pb.EFAK_06, pb.EROD_06, pb.EJAB_05);
  pc.EDER_07 = srtock_pos(pa.EPYT, pb.EPYT, pb.EFEF_07, pb.EGUV_07, pb.EDER_07, pb.EROD_06);

  pc.CUGY_00 = not(!pb.CUBA_00);
  pc.DUDE_01 = not(!pb.DEGU_01);
  pc.DETU_02 = not(!pb.DYRA_02);
  pc.DASO_03 = not(!pb.DOJO_03);
  pc.DAME_04 = not(!pb.DOVU_04);
  pc.EVOK_05 = not(!pb.EJAB_05);
  pc.EFAB_06 = not(!pb.EROD_06);
  pc.ETAK_07 = not(!pb.EDER_07);

  if (pb.UFEG) {
    gc.cpu.D0 = pb.CUGY_00;
    gc.cpu.D1 = pb.DUDE_01;
    gc.cpu.D2 = pb.DETU_02;
    gc.cpu.D3 = pb.DASO_03;
    gc.cpu.D4 = pb.DAME_04;
    gc.cpu.D5 = pb.EVOK_05;
    gc.cpu.D6 = pb.EFAB_06;
    gc.cpu.D7 = pb.ETAK_07;
  }

  pc.ELYS = tock_pos(ga.SER_TICKn, gb.SER_TICKn, gb.RESET2, pb.ELYS, pb.EDER_07);
  gc.SER_OUT = pb.ELYS;
}
