// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "APU.h"

//-----------------------------------------------------------------------------

void tick_apucontrol() {
  //----------
  // top left

  clk.AJER_2MHZ = apu.AJER.flip(clk.APUV_4MHZ, rst.APU_RESET3n);
  clk.AJER_2MHZn = not(clk.AJER_2MHZ);

  bool BATA = not(clk.AJER_2MHZ);
  bool CALO_Q = apu.CALO.flip(BATA, rst.APU_RESETn);
  bool DYFA = not(!CALO_Q);
  clk.DYFA_1MHZ = DYFA;

  bool DAPA = not(rst.APU_RESET);
  bool AFAT = not(rst.APU_RESET);
  bool AGUR = not(rst.APU_RESET);
  bool ATYV = not(rst.APU_RESET);
  bool KAME = not(rst.APU_RESET);

  rst.APU_RESET4n = DAPA;
  rst.APU_RESET2n = AFAT;
  rst.APU_RESETn = AGUR;
  rst.APU_RESET3n = ATYV;
  rst.APU_RESET5n = KAME;

  bool KYDU = not(cpu.CPU_RDn);

  bool HAWU = nand(dec.FF26, cpu.CPU_WRQ);
  bool BOPY = nand(cpu.CPU_WRQ, dec.FF26);
  bool JURE = nand(KYDU, dec.FF26);
  
  bool HAPO = not(rst.RESET2);
  bool GUFO = not(HAPO);
  bool HADA_Q = apu.HADA.tock(HAWU, GUFO, mem.D7);
  bool JYRO = or(HAPO, !HADA_Q);
  
  bool KUBY = not(JYRO);
  bool KEBA = not(KUBY);
  rst.APU_RESET = KEBA;

  bool HOPE = not(!HADA_Q);
  if (JURE) {
    mem.D7 = HOPE;
  }

  bool KEPY = not(JYRO);
  bool ETUC = and(cpu.CPU_WRQ, dec.FF26);
  bool EFOP = and(mem.D4, cpu.FROM_CPU);
  bool FOKU = not(ETUC);
  apu.FERO_Q = apu.FERO.tock(FOKU, KEPY, EFOP);
  bool EDEK = not(!apu.FERO_Q);
  apu.NET03 = EDEK;

  bool BOWY_Q = apu.BOWY.tock(BOPY, KEPY, mem.D5);
  bool BAZA_Q = apu.BAZA.tock(clk.AJER_2MHZn, rst.APU_RESET3n, BOWY_Q);
  bool CELY = mux2(BAZA_Q, clk.BYFE_128, apu.NET03);
  bool CONE = not(CELY);
  apu.CATE = not(CONE);

  //----------
  // FF24 NR50

  bool AGUZ = not(cpu.CPU_RD);
  cpu.CPU_RDn = AGUZ;

  bool BYMA = not(dec.FF24);
  bool BEFU = nor(AGUZ, BYMA);
  bool ADAK = not(BEFU);

  bool BOSU = nand(dec.FF24, cpu.CPU_WRQ);

  bool BAXY = not(BOSU);
  bool BUBU = not(BAXY);
  bool BEDU_Q = apu.BEDU.tock(BUBU, JYRO, mem.D7);
  bool COZU_Q = apu.COZU.tock(BUBU, JYRO, mem.D6);
  bool BUMO_Q = apu.BUMO.tock(BUBU, JYRO, mem.D5);
  bool BYRE_Q = apu.BYRE.tock(BUBU, JYRO, mem.D4);

  bool BOWE = not(BOSU);
  bool ATAF = not(BOWE);
  bool APOS_Q = apu.APOS.tock(ATAF, JYRO, mem.D3);
  bool AGER_Q = apu.AGER.tock(ATAF, JYRO, mem.D2);
  bool BYGA_Q = apu.BYGA.tock(ATAF, JYRO, mem.D1);
  bool APEG_Q = apu.APEG.tock(ATAF, JYRO, mem.D0);

  bool ATUM = not(!BEDU_Q);
  bool BOCY = not(!COZU_Q);
  bool ARUX = not(!BUMO_Q);
  bool AMAD = not(!BYRE_Q);
  bool AXEM = not(!APOS_Q);
  bool AVUD = not(!AGER_Q);
  bool AWED = not(!BYGA_Q);
  bool AKOD = not(!APEG_Q);

  if (ADAK) {
    mem.D7 = ATUM;
    mem.D6 = BOCY;
    mem.D5 = ARUX;
    mem.D4 = AMAD;
    mem.D3 = AXEM;
    mem.D2 = AVUD;
    mem.D1 = AWED;
    mem.D0 = AKOD;
  }

  //----------
  // FF25 NR51
  
  bool BUPO = nand(dec.FF25, cpu.CPU_WRQ);
  bool BONO = not(BUPO);
  bool BYFA = not(BUPO);

  bool BOGU_Q = apu.BOGU.tock(BONO, JYRO, mem.D1);
  bool BAFO_Q = apu.BAFO.tock(BONO, JYRO, mem.D2);
  bool ATUF_Q = apu.ATUF.tock(BONO, JYRO, mem.D3);
  bool ANEV_Q = apu.ANEV.tock(BONO, JYRO, mem.D0);
  bool BEPU_Q = apu.BEPU.tock(BYFA, JYRO, mem.D7);
  bool BEFO_Q = apu.BEFO.tock(BYFA, JYRO, mem.D6);
  bool BUME_Q = apu.BUME.tock(BYFA, JYRO, mem.D4);
  bool BOFA_Q = apu.BOFA.tock(BYFA, JYRO, mem.D5);

  bool GEPA = not(dec.FF25);
  bool HEFA = nor(GEPA, cpu.CPU_RDn);
  bool GUMU = not(HEFA);

  bool CAPU = not(!BOGU_Q);
  bool CAGA = not(!BAFO_Q);
  bool BOCA = not(!ATUF_Q);
  bool BUZU = not(!ANEV_Q);
  bool CERE = not(!BEPU_Q);
  bool CADA = not(!BEFO_Q);
  bool CAVU = not(!BUME_Q);
  bool CUDU = not(!BOFA_Q);

  if (GUMU) {
    mem.D1 = CAPU;
    mem.D2 = CAGA;
    mem.D3 = BOCA;
    mem.D0 = BUZU;
    mem.D7 = CERE;
    mem.D6 = CADA;
    mem.D4 = CAVU;
    mem.D5 = CUDU;
  }

  //----------
  // FF26 NR52

  bool CETO = not(cpu.CPU_RDn);
  bool KAZO = not(cpu.CPU_RDn);
  bool CURU = not(cpu.CPU_RDn);
  apu.GAXO = not(cpu.CPU_RDn);

  bool DOLE = nand(dec.FF26, CETO);
  bool KAMU = nand(dec.FF26, KAZO);
  bool DURU = nand(dec.FF26, CURU);
  bool FEWA = nand(dec.FF26, apu.GAXO);

  bool COTO = not(apu.CH1_ACTIVEn);
  bool KOGE = not(apu.CH4_ACTIVEn);
  bool EFUS = not(apu.CH2_ACTIVEn);
  bool FATE = not(apu.CH3_ACTIVEn);

  if (DOLE) mem.D0 = COTO;
  if (KAMU) mem.D3 = KOGE;
  if (DURU) mem.D1 = EFUS;
  if (FEWA) mem.D2 = FATE;
}