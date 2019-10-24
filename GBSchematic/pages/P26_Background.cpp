#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P26_Background {
  struct Input {
    bool CLKPIPE;

    bool PORE;
    bool NETA;
    bool POTU;
    bool XUHA;
    bool WEFE;
    bool DEPO;

    bool XEHO,SAVY,XODU,XYDO,TUHU,TUKY,TAKO,SYBE; // P21 X counter regs
    bool LESY,LOTA,LYKU,ROBY,TYTA,TYCO,SOKA,XOVU; // P34 or of spr_pix_A/B
    bool V0,V1,V2,V3,V4,V5,V6,V7;
    bool FF40_D0,FF40_D1,FF40_D2,FF40_D3,FF40_D4,FF40_D5,FF40_D6,FF40_D7; // FF40 LCDC
    bool FF42_D0,FF42_D1,FF42_D2,FF42_D3,FF42_D4,FF42_D5,FF42_D6,FF42_D7; // FF42 SCY
    bool FF43_D0,FF43_D1,FF43_D2,FF43_D3,FF43_D4,FF43_D5,FF43_D6,FF43_D7; // FF43 SCX
  };

  struct Output {
    bool VAVA3; // shift reg out

    bool MA_OE;
    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;
  };

  // background shift reg, i think this holds the sprite mask?
  reg VEZO,WURU,VOSA,WYFU,XETE,WODA,VUMO,VAVA;

  //-----------------------------------------------------------------------------

  void tick(const Input& in, Output& out) {
    //----------
    // y + scy;

    bool AXAD = not(in.PORE);
    bool ASUL = and(AXAD, in.NETA);
    bool ACEN = and(AXAD, in.POTU);
    bool BEJE = not(ASUL);
    bool BAFY = not(ACEN);

    bool FAFO_S = add_s(in.V0, in.FF42_D0, 0);
    bool FAFO_C = add_c(in.V0, in.FF42_D0, 0);

    bool EMUX_S = add_s(in.V1, in.FF42_D1, FAFO_C);
    bool EMUX_C = add_c(in.V1, in.FF42_D1, FAFO_C);

    bool ECAB_S = add_s(in.V2, in.FF42_D2, EMUX_C);
    bool ECAB_C = add_c(in.V2, in.FF42_D2, EMUX_C);

    bool ETAM_S = add_s(in.V3, in.FF42_D3, ECAB_C);
    bool ETAM_C = add_c(in.V3, in.FF42_D3, ECAB_C);

    bool DOTO_S = add_s(in.V4, in.FF42_D4, ETAM_C);
    bool DOTO_C = add_c(in.V4, in.FF42_D4, ETAM_C);

    bool DABA_S = add_s(in.V5, in.FF42_D5, DOTO_C);
    bool DABA_C = add_c(in.V5, in.FF42_D5, DOTO_C);

    bool EFYK_S = add_s(in.V6, in.FF42_D6, DABA_C);
    bool EFYK_C = add_c(in.V6, in.FF42_D6, DABA_C);

    bool EJOK_S = add_s(in.V7, in.FF42_D7, EFYK_C);
    //bool EJOK_C = add_c(in.V7, in.FF42_D7, EFYK_C);

    bool ASUM = not(in.XUHA);
    bool EVAD = not(FAFO_S);
    bool DAHU = not(EMUX_S);
    bool DODE = not(ECAB_S);
    bool DUHO = not(ETAM_S);
    bool CASE = not(DOTO_S);
    bool CYPO = not(DABA_S);
    bool CETA = not(EFYK_S);
    bool DAFE = not(EJOK_S);

    if (BEJE) {
      out.MA_OE = true;
      out.MA0 = ASUM;
      out.MA1 = EVAD;
      out.MA2 = DAHU;
      out.MA3 = DODE;
    }

    if (BAFY) {
      out.MA_OE = true;
      out.MA5 = DUHO;
      out.MA6 = CASE;
      out.MA7 = CYPO;
      out.MA8 = CETA;
      out.MA9 = DAFE;
    }

    //----------
    // x + scx

    //bool ATAD_S = add_s(in.XEHO, in.FF43_D0, 0);
    bool ATAD_C = add_c(in.XEHO, in.FF43_D0, 0);

    //bool BEHU_S = add_s(in.SAVY, in.FF43_D1, ATAD_C);
    bool BEHU_C = add_c(in.SAVY, in.FF43_D1, ATAD_C);

    //bool APYH_S = add_s(in.XODU, in.FF43_D2, BEHU_C);
    bool APYH_C = add_c(in.XODU, in.FF43_D2, BEHU_C);

    bool BABE_S = add_s(in.XYDO, in.FF43_D3, APYH_C);
    bool BABE_C = add_c(in.XYDO, in.FF43_D3, APYH_C);

    bool ABOD_S = add_s(in.TUHU, in.FF43_D4, BABE_C);
    bool ABOD_C = add_c(in.TUHU, in.FF43_D4, BABE_C);

    bool BEWY_S = add_s(in.TUKY, in.FF43_D5, ABOD_C);
    bool BEWY_C = add_c(in.TUKY, in.FF43_D5, ABOD_C);

    bool BYCA_S = add_s(in.TAKO, in.FF43_D6, BEWY_C);
    bool BYCA_C = add_c(in.TAKO, in.FF43_D6, BEWY_C);

    bool ACUL_S = add_s(in.SYBE, in.FF43_D7, BYCA_C);
    //bool ACUL_C = add_c(in.SYBE, in.FF43_D7, BYCA_C);

    // polarity?
    bool AMUV = in.FF40_D3;
    bool COXO = in.WEFE;
    bool COVE = in.WEFE;
    bool AXEP = BABE_S;
    bool AFEB = ABOD_S;
    bool ALEL = BEWY_S;
    bool COLY = BYCA_S;
    bool AJAN = ACUL_S;

    if (BAFY) {
      out.MA_OE = true;
      out.MA10 = AMUV;
      out.MA12 = COXO;
      out.MA11 = COVE;
      out.MA0 = AXEP;
      out.MA1 = AFEB;
      out.MA2 = ALEL;
      out.MA3 = COLY;
      out.MA4 = AJAN;
    }

    //----------
    // background shift register connected to VAVA3

    bool XOGA = not(in.DEPO);
    bool XURA = not(in.DEPO);
    bool TAJO = not(in.DEPO);
    bool XENU = not(in.DEPO);
    bool XYKE = not(in.DEPO);
    bool XABA = not(in.DEPO);
    bool TAFU = not(in.DEPO);
    bool XUHO = not(in.DEPO);

    bool TEDE = nand(in.DEPO, in.LESY);
    bool XALA = nand(in.DEPO, in.LOTA);
    bool TYRA = nand(in.DEPO, in.LYKU);
    bool XYRU = nand(in.DEPO, in.ROBY);
    bool XUKU = nand(in.DEPO, in.TYTA);
    bool XELY = nand(in.DEPO, in.TYCO);
    bool TYKO = nand(in.DEPO, in.SOKA);
    bool TUWU = nand(in.DEPO, in.XOVU);

    bool WOKA = nand(XOGA, in.LESY);
    bool WEDE = nand(XURA, in.LOTA);
    bool TUFO = nand(TAJO, in.LYKU);
    bool WEVO = nand(XENU, in.ROBY);
    bool WEDY = nand(XYKE, in.TYTA);
    bool WUJA = nand(XABA, in.TYCO);
    bool TENA = nand(TAFU, in.SOKA);
    bool WUBU = nand(XUHO, in.XOVU);

    //----------
    // registers = background pixel pipe?

    bool VEZO_Q = VEZO.srtock(in.CLKPIPE, TEDE, WOKA, 0);
    bool WURU_Q = WURU.srtock(in.CLKPIPE, XALA, WEDE, VEZO_Q);
    bool VOSA_Q = VOSA.srtock(in.CLKPIPE, TYRA, TUFO, WURU_Q);
    bool WYFU_Q = WYFU.srtock(in.CLKPIPE, XYRU, WEVO, VOSA_Q);
    bool XETE_Q = XETE.srtock(in.CLKPIPE, XUKU, WEDY, WYFU_Q);
    bool WODA_Q = WODA.srtock(in.CLKPIPE, XELY, WUJA, XETE_Q);
    bool VUMO_Q = VUMO.srtock(in.CLKPIPE, TYKO, TENA, WODA_Q);
    bool VAVA_Q = VAVA.srtock(in.CLKPIPE, TUWU, WUBU, VUMO_Q);
    out.VAVA3 = VAVA_Q;
  }
};