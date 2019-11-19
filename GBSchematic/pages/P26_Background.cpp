#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P26_Background::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // y + scy;

  c.p26.AXAD = not(b.p27.PORE);
  c.p26.ASUL = and(b.p26.AXAD, b.p27.NETA);
  c.p26.ACEN = and(b.p26.AXAD, b.p27.POTU);
  c.p26.BEJE = not(b.p26.ASUL);
  c.p26.BAFY = not(b.p26.ACEN);

  c.p26.FAFO_S = add_s(b.p21.V0, b.p23.SCY0, 0);
  c.p26.FAFO_C = add_c(b.p21.V0, b.p23.SCY0, 0);
  c.p26.EMUX_S = add_s(b.p21.V1, b.p23.SCY1, b.p26.FAFO_C);
  c.p26.EMUX_C = add_c(b.p21.V1, b.p23.SCY1, b.p26.FAFO_C);
  c.p26.ECAB_S = add_s(b.p21.V2, b.p23.SCY2, b.p26.EMUX_C);
  c.p26.ECAB_C = add_c(b.p21.V2, b.p23.SCY2, b.p26.EMUX_C);
  c.p26.ETAM_S = add_s(b.p21.V3, b.p23.SCY3, b.p26.ECAB_C);
  c.p26.ETAM_C = add_c(b.p21.V3, b.p23.SCY3, b.p26.ECAB_C);
  c.p26.DOTO_S = add_s(b.p21.V4, b.p23.SCY4, b.p26.ETAM_C);
  c.p26.DOTO_C = add_c(b.p21.V4, b.p23.SCY4, b.p26.ETAM_C);
  c.p26.DABA_S = add_s(b.p21.V5, b.p23.SCY5, b.p26.DOTO_C);
  c.p26.DABA_C = add_c(b.p21.V5, b.p23.SCY5, b.p26.DOTO_C);
  c.p26.EFYK_S = add_s(b.p21.V6, b.p23.SCY6, b.p26.DABA_C);
  c.p26.EFYK_C = add_c(b.p21.V6, b.p23.SCY6, b.p26.DABA_C);
  c.p26.EJOK_S = add_s(b.p21.V7, b.p23.SCY7, b.p26.EFYK_C);
  c.p26.EJOK_C = add_c(b.p21.V7, b.p23.SCY7, b.p26.EFYK_C);

  c.p26.ASUM = not(b.p27.MA00_2);
  c.p26.EVAD = not(b.p26.FAFO_S);
  c.p26.DAHU = not(b.p26.EMUX_S);
  c.p26.DODE = not(b.p26.ECAB_S);
  c.p26.DUHO = not(b.p26.ETAM_S);
  c.p26.CASE = not(b.p26.DOTO_S);
  c.p26.CYPO = not(b.p26.DABA_S);
  c.p26.CETA = not(b.p26.EFYK_S);
  c.p26.DAFE = not(b.p26.EJOK_S);

  if (b.p26.BEJE) {
    c.MA00 = b.p26.ASUM;
    c.MA01 = b.p26.EVAD;
    c.MA02 = b.p26.DAHU;
    c.MA03 = b.p26.DODE;
  }

  if (b.p26.BAFY) {
    c.MA05 = b.p26.DUHO;
    c.MA06 = b.p26.CASE;
    c.MA07 = b.p26.CYPO;
    c.MA08 = b.p26.CETA;
    c.MA09 = b.p26.DAFE;
  }

  //----------
  // x + scx

  c.p26.ATAD_S = add_s(b.p21.X0, b.p23.SCX0, 0);
  c.p26.ATAD_C = add_c(b.p21.X0, b.p23.SCX0, 0);
  c.p26.BEHU_S = add_s(b.p21.X1, b.p23.SCX1, b.p26.ATAD_C);
  c.p26.BEHU_C = add_c(b.p21.X1, b.p23.SCX1, b.p26.ATAD_C);
  c.p26.APYH_S = add_s(b.p21.X2, b.p23.SCX2, b.p26.BEHU_C);
  c.p26.APYH_C = add_c(b.p21.X2, b.p23.SCX2, b.p26.BEHU_C);
  c.p26.BABE_S = add_s(b.p21.X3, b.p23.SCX3, b.p26.APYH_C);
  c.p26.BABE_C = add_c(b.p21.X3, b.p23.SCX3, b.p26.APYH_C);
  c.p26.ABOD_S = add_s(b.p21.X4, b.p23.SCX4, b.p26.BABE_C);
  c.p26.ABOD_C = add_c(b.p21.X4, b.p23.SCX4, b.p26.BABE_C);
  c.p26.BEWY_S = add_s(b.p21.X5, b.p23.SCX5, b.p26.ABOD_C);
  c.p26.BEWY_C = add_c(b.p21.X5, b.p23.SCX5, b.p26.ABOD_C);
  c.p26.BYCA_S = add_s(b.p21.X6, b.p23.SCX6, b.p26.BEWY_C);
  c.p26.BYCA_C = add_c(b.p21.X6, b.p23.SCX6, b.p26.BEWY_C);
  c.p26.ACUL_S = add_s(b.p21.X7, b.p23.SCX7, b.p26.BYCA_C);
  c.p26.ACUL_C = add_c(b.p21.X7, b.p23.SCX7, b.p26.BYCA_C);

  // polarity?
  c.p26.AMUV = b.p23.BG_MAP_SEL;
  c.p26.COXO = b.p28.WEFE;
  c.p26.COVE = b.p28.WEFE;
  c.p26.AXEP = b.p26.BABE_S;
  c.p26.AFEB = b.p26.ABOD_S;
  c.p26.ALEL = b.p26.BEWY_S;
  c.p26.COLY = b.p26.BYCA_S;
  c.p26.AJAN = b.p26.ACUL_S;

  if (b.p26.BAFY) {
    c.MA00 = b.p26.AXEP;
    c.MA01 = b.p26.AFEB;
    c.MA02 = b.p26.ALEL;
    c.MA03 = b.p26.COLY;
    c.MA04 = b.p26.AJAN;

    c.MA10 = b.p26.AMUV;
    c.MA11 = b.p26.COVE;
    c.MA12 = b.p26.COXO;
  }

  //----------
  // background shift register connected to VAVA3

  c.p26.XOGA = not(b.p31.OAM_A_D7);
  c.p26.XURA = not(b.p31.OAM_A_D7);
  c.p26.TAJO = not(b.p31.OAM_A_D7);
  c.p26.XENU = not(b.p31.OAM_A_D7);
  c.p26.XYKE = not(b.p31.OAM_A_D7);
  c.p26.XABA = not(b.p31.OAM_A_D7);
  c.p26.TAFU = not(b.p31.OAM_A_D7);
  c.p26.XUHO = not(b.p31.OAM_A_D7);

  c.p26.TEDE = nand(b.p31.OAM_A_D7, b.p34.LESY);
  c.p26.XALA = nand(b.p31.OAM_A_D7, b.p34.LOTA);
  c.p26.TYRA = nand(b.p31.OAM_A_D7, b.p34.LYKU);
  c.p26.XYRU = nand(b.p31.OAM_A_D7, b.p34.ROBY);
  c.p26.XUKU = nand(b.p31.OAM_A_D7, b.p34.TYTA);
  c.p26.XELY = nand(b.p31.OAM_A_D7, b.p34.TYCO);
  c.p26.TYKO = nand(b.p31.OAM_A_D7, b.p34.SOKA);
  c.p26.TUWU = nand(b.p31.OAM_A_D7, b.p34.XOVU);

  c.p26.WOKA = nand(b.p26.XOGA, b.p34.LESY);
  c.p26.WEDE = nand(b.p26.XURA, b.p34.LOTA);
  c.p26.TUFO = nand(b.p26.TAJO, b.p34.LYKU);
  c.p26.WEVO = nand(b.p26.XENU, b.p34.ROBY);
  c.p26.WEDY = nand(b.p26.XYKE, b.p34.TYTA);
  c.p26.WUJA = nand(b.p26.XABA, b.p34.TYCO);
  c.p26.TENA = nand(b.p26.TAFU, b.p34.SOKA);
  c.p26.WUBU = nand(b.p26.XUHO, b.p34.XOVU);

  c.p26.VEZO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.TEDE, b.p26.WOKA, b.p26.VEZO, 0);
  c.p26.WURU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.XALA, b.p26.WEDE, b.p26.WURU, b.p26.VEZO);
  c.p26.VOSA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.TYRA, b.p26.TUFO, b.p26.VOSA, b.p26.WURU);
  c.p26.WYFU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.XYRU, b.p26.WEVO, b.p26.WYFU, b.p26.VOSA);
  c.p26.XETE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.XUKU, b.p26.WEDY, b.p26.XETE, b.p26.WYFU);
  c.p26.WODA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.XELY, b.p26.WUJA, b.p26.WODA, b.p26.XETE);
  c.p26.VUMO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.TYKO, b.p26.TENA, b.p26.VUMO, b.p26.WODA);
  c.p26.VAVA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.TUWU, b.p26.WUBU, b.p26.VAVA, b.p26.VUMO);
}