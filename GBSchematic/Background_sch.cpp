// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "Clocks.h"
#include "VramBus.h"
#include "PPU.h"

//----------
// inputs

extern bool WEFE;
extern bool PORE;
extern bool NETA;
extern bool POTU;
extern bool XUHA;

extern reg DEPO;

//----------
// outputs

bool VAVA3;

//-----------------------------------------------------------------------------

void tick_background() {
  //----------
  // y + scy;

  bool AXAD = not(PORE);
  bool ASUL = and(AXAD, NETA);
  bool ACEN = and(AXAD, POTU);
  bool BEJE = not(ASUL);
  bool BAFY = not(ACEN);

  bool FAFO_S = add_s(ppu.V0, ppu.FF42_D0, 0);
  bool FAFO_C = add_c(ppu.V0, ppu.FF42_D0, 0);

  bool EMUX_S = add_s(ppu.V1, ppu.FF42_D1, FAFO_C);
  bool EMUX_C = add_c(ppu.V1, ppu.FF42_D1, FAFO_C);

  bool ECAB_S = add_s(ppu.V2, ppu.FF42_D2, EMUX_C);
  bool ECAB_C = add_c(ppu.V2, ppu.FF42_D2, EMUX_C);

  bool ETAM_S = add_s(ppu.V3, ppu.FF42_D3, ECAB_C);
  bool ETAM_C = add_c(ppu.V3, ppu.FF42_D3, ECAB_C);

  bool DOTO_S = add_s(ppu.V4, ppu.FF42_D4, ETAM_C);
  bool DOTO_C = add_c(ppu.V4, ppu.FF42_D4, ETAM_C);

  bool DABA_S = add_s(ppu.V5, ppu.FF42_D5, DOTO_C);
  bool DABA_C = add_c(ppu.V5, ppu.FF42_D5, DOTO_C);

  bool EFYK_S = add_s(ppu.V6, ppu.FF42_D6, DABA_C);
  bool EFYK_C = add_c(ppu.V6, ppu.FF42_D6, DABA_C);

  bool EJOK_S = add_s(ppu.V7, ppu.FF42_D7, EFYK_C);
  //bool EJOK_C = add_c(ppu.V7, ppu.FF42_D7, EFYK_C);

  bool ASUM = not(XUHA);
  bool EVAD = not(FAFO_S);
  bool DAHU = not(EMUX_S);
  bool DODE = not(ECAB_S);
  bool DUHO = not(ETAM_S);
  bool CASE = not(DOTO_S);
  bool CYPO = not(DABA_S);
  bool CETA = not(EFYK_S);
  bool DAFE = not(EJOK_S);

  if (BEJE) {
    vram.MA0 = ASUM;
    vram.MA1 = EVAD;
    vram.MA2 = DAHU;
    vram.MA3 = DODE;
  }

  if (BAFY) {
    vram.MA5 = DUHO;
    vram.MA6 = CASE;
    vram.MA7 = CYPO;
    vram.MA8 = CETA;
    vram.MA9 = DAFE;
  }

  //----------
  // x + scx

  bool XEHO_Q = ppu.XEHO.q();
  bool SAVY_Q = ppu.SAVY.q();
  bool XODU_Q = ppu.XODU.q();
  bool XYDO_Q = ppu.XYDO.q();
  bool TUHU_Q = ppu.TUHU.q();
  bool TUKY_Q = ppu.TUKY.q();
  bool TAKO_Q = ppu.TAKO.q();
  bool SYBE_Q = ppu.SYBE.q();

  //bool ATAD_S = add_s(XEHO_Q, ppu.FF43_D0, 0);
  bool ATAD_C = add_c(XEHO_Q, ppu.FF43_D0, 0);

  //bool BEHU_S = add_s(SAVY_Q, ppu.FF43_D1, ATAD_C);
  bool BEHU_C = add_c(SAVY_Q, ppu.FF43_D1, ATAD_C);

  //bool APYH_S = add_s(XODU_Q, ppu.FF43_D2, BEHU_C);
  bool APYH_C = add_c(XODU_Q, ppu.FF43_D2, BEHU_C);

  bool BABE_S = add_s(XYDO_Q, ppu.FF43_D3, APYH_C);
  bool BABE_C = add_c(XYDO_Q, ppu.FF43_D3, APYH_C);

  bool ABOD_S = add_s(TUHU_Q, ppu.FF43_D4, BABE_C);
  bool ABOD_C = add_c(TUHU_Q, ppu.FF43_D4, BABE_C);

  bool BEWY_S = add_s(TUKY_Q, ppu.FF43_D5, ABOD_C);
  bool BEWY_C = add_c(TUKY_Q, ppu.FF43_D5, ABOD_C);

  bool BYCA_S = add_s(TAKO_Q, ppu.FF43_D6, BEWY_C);
  bool BYCA_C = add_c(TAKO_Q, ppu.FF43_D6, BEWY_C);

  bool ACUL_S = add_s(SYBE_Q, ppu.FF43_D7, BYCA_C);
  //bool ACUL_C = add_c(SYBE_Q, ppu.FF43_D7, BYCA_C);

  bool AMUV = not(ppu.FF40_D3);
  bool COXO = not(WEFE);
  bool COVE = not(WEFE);

  // maybe the 'unk1' block inverts its sum out? otherwise we'd be putting the inverted sum on the bus...
  // or maybe these are not inverters...
  bool AXEP = not(BABE_S);
  bool AFEB = not(ABOD_S);
  bool ALEL = not(BEWY_S);
  bool COLY = not(BYCA_S);
  bool AJAN = not(ACUL_S);

  if (BAFY) {
    vram.MA10 = AMUV;
    vram.MA12 = COXO;
    vram.MA11 = COVE;
    vram.MA0 = AXEP;
    vram.MA1 = AFEB;
    vram.MA2 = ALEL;
    vram.MA3 = COLY;
    vram.MA4 = AJAN;
  }

  //----------
  // some shift register connected to VAVA3?
  // this is the "draw background here" shift register, it should probably move to pixelmux

  bool DEPO_Q = DEPO.q();

  bool XOGA = not(!DEPO_Q);
  bool XURA = not(!DEPO_Q);
  bool TAJO = not(!DEPO_Q);
  bool XENU = not(!DEPO_Q);
  bool XYKE = not(!DEPO_Q);
  bool XABA = not(!DEPO_Q);
  bool TAFU = not(!DEPO_Q);
  bool XUHO = not(!DEPO_Q);

  bool TEDE = nand(!DEPO_Q, ppu.LESY);
  bool XALA = nand(!DEPO_Q, ppu.LOTA);
  bool TYRA = nand(!DEPO_Q, ppu.LYKU);
  bool XYRU = nand(!DEPO_Q, ppu.ROBY);
  bool XUKU = nand(!DEPO_Q, ppu.TYTA);
  bool XELY = nand(!DEPO_Q, ppu.TYCO);
  bool TYKO = nand(!DEPO_Q, ppu.SOKA);
  bool TUWU = nand(!DEPO_Q, ppu.XOVU);

  bool WOKA = nand(XOGA, ppu.LESY);
  bool WEDE = nand(XURA, ppu.LOTA);
  bool TUFO = nand(TAJO, ppu.LYKU);
  bool WEVO = nand(XENU, ppu.ROBY);
  bool WEDY = nand(XYKE, ppu.TYTA);
  bool WUJA = nand(XABA, ppu.TYCO);
  bool TENA = nand(TAFU, ppu.SOKA);
  bool WUBU = nand(XUHO, ppu.XOVU);

  //----------
  // registers = background pixel pipe?

  bool VEZO_Q = ppu.VEZO.srtock(clk.CLKPIPE, TEDE, WOKA, 0);
  bool WURU_Q = ppu.WURU.srtock(clk.CLKPIPE, XALA, WEDE, VEZO_Q);
  bool VOSA_Q = ppu.VOSA.srtock(clk.CLKPIPE, TYRA, TUFO, WURU_Q);
  bool WYFU_Q = ppu.WYFU.srtock(clk.CLKPIPE, XYRU, WEVO, VOSA_Q);
  bool XETE_Q = ppu.XETE.srtock(clk.CLKPIPE, XUKU, WEDY, WYFU_Q);
  bool WODA_Q = ppu.WODA.srtock(clk.CLKPIPE, XELY, WUJA, XETE_Q);
  bool VUMO_Q = ppu.VUMO.srtock(clk.CLKPIPE, TYKO, TENA, WODA_Q);
  bool VAVA_Q = ppu.VAVA.srtock(clk.CLKPIPE, TUWU, WUBU, VUMO_Q);
  VAVA3 = VAVA_Q;
}