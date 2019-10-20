// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "Serial.h"

//-----------------------------------------------------------------------------

void Serial::tick() {
  bool SARE = nor(mem.A7, mem.A7, mem.A7, mem.A4, mem.A3);
  bool SEFY = not(mem.A2);
  bool SANO = and(SARE, SEFY, dec.FFXX);
  bool UWAM = nand(mem.TOVY_A0n, mem.A1, cpu.CPU_WR, SANO);
  bool UCOM = nand(SANO, cpu.CPU_RD, mem.A1, mem.TOVY_A0n);
  bool CARO = and(UWAM, rst.RESET2);
  bool CAVE = mux2(COTY_Q, ext.SCK_IN, CULY_Q);
  bool DAWA = or(CAVE, !ETAF_Q);
  bool ELUV = not(!ETAF_Q);
  bool CORE = not(!CULY_Q);

  if (UCOM) {
    mem.D7 = ELUV;
    mem.D0 = CORE;
  }

  bool KEXU = nand(DAWA, ext.SCK_DIR);
  bool JAGO = not(ext.SCK_DIR);
  bool KUJO = nor(JAGO, DAWA);
  bool EDYL = not(DAWA);
  SER_TICKn = EDYL;
  bool COBA = CALY_Q;
  bool CABY = and(COBA, rst.RESET2);

  cpu.INT_SERIAL = CALY_Q;


  dec.A00_07 = SARE;

  bool URYS = nand(SANO, cpu.CPU_WR, mem.TOLA_A1n, mem.A0);
  bool DAKU = not(URYS);
  bool EPYT = not(SER_TICKn);
  bool DEHO = not(EPYT);
  bool DAWE = not(DEHO);
  bool CAGE = not(ext.SIN_IN);

  bool COHY = unk3(URYS, mem.D0, rst.RESET2);
  bool DUMO = unk3(URYS, mem.D1, rst.RESET2);
  bool DYBO = unk3(URYS, mem.D2, rst.RESET2);
  bool DAJU = unk3(URYS, mem.D3, rst.RESET2);
  bool DYLY = unk3(URYS, mem.D4, rst.RESET2);
  bool EHUJ = unk3(URYS, mem.D5, rst.RESET2);
  bool EFAK = unk3(URYS, mem.D6, rst.RESET2);
  bool EGUV = unk3(URYS, mem.D7, rst.RESET2);

  bool CUFU = nand(mem.D0, DAKU);
  bool DOCU = nand(mem.D1, DAKU);
  bool DELA = nand(mem.D2, DAKU);
  bool DYGE = nand(mem.D3, DAKU);
  bool DOLA = nand(mem.D4, DAKU);
  bool ELOK = nand(mem.D5, DAKU);
  bool EDEL = nand(mem.D6, DAKU);
  bool EFEF = nand(mem.D7, DAKU);

  bool CUGY = not(!CUBA_Q);
  bool DUDE = not(!DEGU_Q);
  bool DETU = not(!DYRA_Q);
  bool DASO = not(!DOJO_Q);
  bool DAME = not(!DOVU_Q);
  bool EVOK = not(!EJAB_Q);
  bool EFAB = not(!EROD_Q);
  bool ETAK = not(!EDER_Q);

  bool UFEG = and(SANO, cpu.CPU_RD, mem.TOLA_A1n, mem.A0);
  if (UFEG) {
    mem.D0 = CUGY;
    mem.D1 = DUDE;
    mem.D2 = DETU;
    mem.D3 = DASO;
    mem.D4 = DAME;
    mem.D5 = EVOK;
    mem.D6 = EFAB;
    mem.D7 = ETAK;
  }

  //----------
  // registers

  bool COTY_Q_ = COTY_Q;
  bool ETAF_Q_ = ETAF_Q;
  bool CULY_Q_ = CULY_Q;
  bool CAFA_Q_ = CAFA_Q;
  bool CYLO_Q_ = CYLO_Q;
  bool CYDE_Q_ = CYDE_Q;
  bool CALY_Q_ = CALY_Q;

  if (COTY_CLK && !clk.CLK_16k) COTY_Q_ = !COTY_Q;
  if (ETAF_CLK && !UWAM) ETAF_Q_ = mem.D7;
  if (CAFA_CLK && !DAWA) CAFA_Q_ = !CAFA_Q;
  if (CYLO_CLK && !!CAFA_Q) CYLO_Q_ = !CYLO_Q;
  if (CYDE_CLK && !!CYLO_Q) CYDE_Q_ = !CYDE_Q;
  if (CALY_CLK && !!CYDE_Q) CALY_Q_ = !CALY_Q;
  
  if (!UWAM) COTY_Q_ = 0;
  if (!CABY) ETAF_Q_ = 0;
  if (!CARO) CAFA_Q_ = 0;
  if (!CARO) CYLO_Q_ = 0;
  if (!CARO) CYDE_Q_ = 0;
  if (!CARO) CALY_Q_ = 0;

  COTY_CLK = clk.CLK_16k;
  ETAF_CLK = UWAM;
  CULY_CLK = UWAM;
  CAFA_CLK = DAWA;
  CYLO_CLK = !CAFA_Q;
  CYDE_CLK = !CYLO_Q;
  CALY_CLK = !CYDE_Q;

  COTY_Q = COTY_Q_;
  ETAF_Q = ETAF_Q_;
  CULY_Q = CULY_Q_;
  CAFA_Q = CAFA_Q_;
  CYLO_Q = CYLO_Q_;
  CYDE_Q = CYDE_Q_;
  CALY_Q = CALY_Q_;

  //----------
  // registers

  bool CUBA_Q_ = CUBA_Q;
  bool DEGU_Q_ = DEGU_Q;
  bool DYRA_Q_ = DYRA_Q;
  bool DOJO_Q_ = DOJO_Q;
  bool DOVU_Q_ = DOVU_Q;
  bool EJAB_Q_ = EJAB_Q;
  bool EROD_Q_ = EROD_Q;
  bool EDER_Q_ = EDER_Q;
  bool ELYS_Q_ = ELYS_Q;

  // shift register
  if (CUBA_CLK && !DAWE) CUBA_Q_ = CAGE;
  if (DEGU_CLK && !DAWE) DEGU_Q_ = CUBA_Q;
  if (DYRA_CLK && !DAWE) DYRA_Q_ = DEGU_Q;
  if (DOJO_CLK && !DAWE) DOJO_Q_ = DYRA_Q;

  if (DOVU_CLK && !EPYT) DOVU_Q_ = DOJO_Q;
  if (EJAB_CLK && !EPYT) EJAB_Q_ = DOVU_Q;
  if (EROD_CLK && !EPYT) EROD_Q_ = EJAB_Q;
  if (EDER_CLK && !EPYT) EDER_Q_ = EROD_Q;

  if (ELYS_CLK && !SER_TICKn) ELYS_Q_ = EDER_Q;

  // this doesn't seem right... not sure UNK4 is really a sr ff
  
  if (CUFU) CUBA_Q_ = 1;
  if (DOCU) DEGU_Q_ = 1;
  if (DELA) DYRA_Q_ = 1;
  if (DYGE) DOJO_Q_ = 1;
  if (DOLA) DOVU_Q_ = 1;
  if (ELOK) EJAB_Q_ = 1;
  if (EDEL) EROD_Q_ = 1;
  if (EFEF) EDER_Q_ = 1;

  if (!COHY) CUBA_Q_ = 0;
  if (!DUMO) DEGU_Q_ = 0;
  if (!DYBO) DYRA_Q_ = 0;
  if (!DAJU) DOJO_Q_ = 0;
  if (!DYLY) DOVU_Q_ = 0;
  if (!EHUJ) EJAB_Q_ = 0;
  if (!EFAK) EROD_Q_ = 0;
  if (!EGUV) EDER_Q_ = 0;

  if (!rst.RESET2) ELYS_Q_ = 0;

  CUBA_CLK = DAWE;
  DEGU_CLK = DAWE;
  DYRA_CLK = DAWE;
  DOJO_CLK = DAWE;
  DOVU_CLK = EPYT;
  EJAB_CLK = EPYT;
  EROD_CLK = EPYT;
  EDER_CLK = EPYT;
  ELYS_CLK = SER_TICKn;

  CUBA_Q = CUBA_Q_;
  DEGU_Q = DEGU_Q_;
  DYRA_Q = DYRA_Q_;
  DOJO_Q = DOJO_Q_;
  DOVU_Q = DOVU_Q_;
  EJAB_Q = EJAB_Q_;
  EROD_Q = EROD_Q_;
  EDER_Q = EDER_Q_;
  ELYS_Q = ELYS_Q_;

  //----------
  // unused

  (void)KUJO;
  (void)KEXU;
}