#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

//-----------------------------------------------------------------------------

struct P9_ApuControl {
  struct Input {
    bool APUV_4MHZ;
    bool BYFE_128HZ;
    bool RESET2;

    bool CPU_RD;
    bool CPU_WRQ;
    bool FROM_CPU;

    bool FF24;
    bool FF26;
    bool FF25;

    bool CH1_ACTIVEn;
    bool CH2_ACTIVEn;
    bool CH3_ACTIVEn;
    bool CH4_ACTIVEn;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool GAXO;
    bool AJER_2MHZ;
    bool DYFA_1MHZ;

    bool APU_RESET;
    bool APU_RESETn;
    bool APU_RESET2n;
    bool APU_RESET3n;
    bool APU_RESET4n;
    bool APU_RESET5n;
    bool JYRO;
    bool CATE;
    bool FERO_Q;
    bool NET03;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF24 NR50
  reg APEG,BYGA,AGER,APOS,BYRE,BUMO,COZU,BEDU;

  // FF25 NR51
  reg ANEV,BOGU,BAFO,ATUF,BUME,BOFA,BEFO,BEPU;

  // clock dividers
  reg AJER,CALO;

  // mystery
  reg HADA,BOWY,BAZA,FERO;

  void tick(const Input& in, Output& out) {

    //---------
    // reset tree

    wire AGUR = not(out.APU_RESET);
    wire AFAT = not(out.APU_RESET);
    wire ATYV = not(out.APU_RESET);
    wire DAPA = not(out.APU_RESET);
    wire KAME = not(out.APU_RESET);

    out.APU_RESETn  = AGUR;
    out.APU_RESET2n = AFAT;
    out.APU_RESET3n = ATYV;
    out.APU_RESET4n = DAPA;
    out.APU_RESET5n = KAME;

    //----------
    // clock dividers

    wire AJER_Q = AJER.flip(in.APUV_4MHZ, out.APU_RESET3n);
    out.AJER_2MHZ = AJER_Q;
    wire AJER_2MHZn = not(AJER_Q);

    wire BATA = not(out.AJER_2MHZ);
    wire CALO_Q = CALO.flip(BATA, out.APU_RESETn);
    wire DYFA = not(!CALO_Q);
    out.DYFA_1MHZ = DYFA;

    //----------
    // main logic chunk

    wire HAWU = nand(in.FF26, in.CPU_WRQ);
    wire BOPY = nand(in.CPU_WRQ, in.FF26);
    wire HAPO = not(in.RESET2);

    wire GUFO = not(HAPO);
    wire HADA_Q = HADA.tock(HAWU, GUFO, in.D7);

    wire JYRO = or(HAPO, !HADA_Q);
    wire KEPY = not(JYRO);

    wire KUBY = not(JYRO);
    wire KEBA = not(KUBY);
    out.APU_RESET = KEBA;

    wire ETUC = and(in.CPU_WRQ, in.FF26);
    wire EFOP = and(in.D4, in.FROM_CPU);
    wire FOKU = not(ETUC);
    out.FERO_Q = FERO.tock(FOKU, KEPY, EFOP);
    wire EDEK = not(!out.FERO_Q);
    out.NET03 = EDEK;

    wire BOWY_Q = BOWY.tock(BOPY, KEPY, in.D5);
    wire BAZA_Q = BAZA.tock(AJER_2MHZn, out.APU_RESET3n, BOWY_Q);
    wire CELY = mux2(BAZA_Q, in.BYFE_128HZ, out.NET03);
    wire CONE = not(CELY);
    out.CATE = not(CONE);

    wire AGUZ = not(in.CPU_RD);
    wire CPU_RDn = AGUZ;
    wire KYDU = not(CPU_RDn);
    wire JURE = nand(KYDU, in.FF26);
    wire HOPE = not(!HADA_Q);
    if (JURE) {
      out.D_OE = true;
      out.D7 = HOPE;
    }

    //----------
    // FF24 NR50

    wire BYMA = not(in.FF24);
    wire BEFU = nor(AGUZ, BYMA);
    wire ADAK = not(BEFU);

    wire BOSU = nand(in.FF24, in.CPU_WRQ);
    wire BAXY = not(BOSU);
    wire BUBU = not(BAXY);
    wire BOWE = not(BOSU);
    wire ATAF = not(BOWE);

    wire APEG_Q = APEG.tock(ATAF, JYRO, in.D0);
    wire BYGA_Q = BYGA.tock(ATAF, JYRO, in.D1);
    wire AGER_Q = AGER.tock(ATAF, JYRO, in.D2);
    wire APOS_Q = APOS.tock(ATAF, JYRO, in.D3);
    wire BYRE_Q = BYRE.tock(BUBU, JYRO, in.D4);
    wire BUMO_Q = BUMO.tock(BUBU, JYRO, in.D5);
    wire COZU_Q = COZU.tock(BUBU, JYRO, in.D6);
    wire BEDU_Q = BEDU.tock(BUBU, JYRO, in.D7);

    wire AKOD = not(!APEG_Q);
    wire AWED = not(!BYGA_Q);
    wire AVUD = not(!AGER_Q);
    wire AXEM = not(!APOS_Q);
    wire AMAD = not(!BYRE_Q);
    wire ARUX = not(!BUMO_Q);
    wire BOCY = not(!COZU_Q);
    wire ATUM = not(!BEDU_Q);

    if (ADAK) {
      out.D_OE = true;
      out.D0 = AKOD;
      out.D1 = AWED;
      out.D2 = AVUD;
      out.D3 = AXEM;
      out.D4 = AMAD;
      out.D5 = ARUX;
      out.D6 = BOCY;
      out.D7 = ATUM;
    }

    //----------
    // FF25 NR51

    wire BUPO = nand(in.FF25, in.CPU_WRQ);
    wire BONO = not(BUPO);
    wire BYFA = not(BUPO);

    wire BOGU_Q = BOGU.tock(BONO, JYRO, in.D1);
    wire BAFO_Q = BAFO.tock(BONO, JYRO, in.D2);
    wire ATUF_Q = ATUF.tock(BONO, JYRO, in.D3);
    wire ANEV_Q = ANEV.tock(BONO, JYRO, in.D0);
    wire BEPU_Q = BEPU.tock(BYFA, JYRO, in.D7);
    wire BEFO_Q = BEFO.tock(BYFA, JYRO, in.D6);
    wire BUME_Q = BUME.tock(BYFA, JYRO, in.D4);
    wire BOFA_Q = BOFA.tock(BYFA, JYRO, in.D5);

    wire GEPA = not(in.FF25);
    wire HEFA = nor(GEPA, CPU_RDn);
    wire GUMU = not(HEFA);

    wire CAPU = not(!BOGU_Q);
    wire CAGA = not(!BAFO_Q);
    wire BOCA = not(!ATUF_Q);
    wire BUZU = not(!ANEV_Q);
    wire CERE = not(!BEPU_Q);
    wire CADA = not(!BEFO_Q);
    wire CAVU = not(!BUME_Q);
    wire CUDU = not(!BOFA_Q);

    if (GUMU) {
      out.D_OE = true;
      out.D1 = CAPU;
      out.D2 = CAGA;
      out.D3 = BOCA;
      out.D0 = BUZU;
      out.D7 = CERE;
      out.D6 = CADA;
      out.D4 = CAVU;
      out.D5 = CUDU;
    }

    //----------
    // FF26 NR52

    wire CETO = not(CPU_RDn);
    wire KAZO = not(CPU_RDn);
    wire CURU = not(CPU_RDn);
    out.GAXO  = not(CPU_RDn);

    wire DOLE = nand(in.FF26, CETO);
    wire KAMU = nand(in.FF26, KAZO);
    wire DURU = nand(in.FF26, CURU);
    wire FEWA = nand(in.FF26, out.GAXO);

    wire COTO = not(in.CH1_ACTIVEn);
    wire KOGE = not(in.CH4_ACTIVEn);
    wire EFUS = not(in.CH2_ACTIVEn);
    wire FATE = not(in.CH3_ACTIVEn);

    if (DOLE) { out.D_OE = true; out.D0 = COTO; }
    if (DURU) { out.D_OE = true; out.D1 = EFUS; }
    if (FEWA) { out.D_OE = true; out.D2 = FATE; }
    if (KAMU) { out.D_OE = true; out.D3 = KOGE; }
  }
};