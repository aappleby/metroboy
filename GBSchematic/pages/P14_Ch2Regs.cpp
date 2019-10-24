#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P14_Ch2Regs {
  struct Input {
    bool APU_WR;
    bool CPU_RDn;
    bool APU_RESET2n;
    bool APU_RESET;
    bool ELOX_Q;
    bool CPU_WRQ;
    bool NET03;
    bool DERA_OUT;
    bool GANE_QN;
    bool GYPA_OUT;
    bool ANUJ;
    bool DOCA;
    bool COGU_OUT;
    bool EROG_OUT;
    bool DUCE;

    bool FF16;
    bool FF17;
    bool FF18;
    bool FF19;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool FF16_WRn;
    bool BENY_OUT;
    bool FF16_D7;
    bool FF16_D7n;
    bool FF16_D6;
    bool FF16_D6n;

    bool FF19_D0;
    bool FF19_D1;
    bool FF19_D2;
    bool FF19_D6;
    bool FF19_D6n;
    bool FF19_D7;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF16 NR21
  reg BAMY,BERA;

  // FF17 NR22
  reg GATA,FORE,GAGE,GURA,GUFE,HAVA,HORE,HYFU;

  // FF18 NR23
  reg FOFE,FOVA,FEDY,FOME,FORA,GODA,GUMY,GUPU;
  reg DONE,DYNU,EZOF,CYVO,FUXO,GANO,GOCA,GANE;

  // FF19 NR24
  reg JUPY,JANY,JEFU,ETAP,HERO,HEPU,HEVY,EMER;

  void tick(const Input& in, Output& out) {
    bool FOGE = not(in.CPU_RDn);
    bool FAPE = and(FOGE, in.NET03);

    //----------
    // FF16 NR21

    bool BAMY_Q = BAMY.q();
    bool BERA_Q = BERA.q();

    bool AGYN = nand(in.APU_WR, in.FF16);
    bool ASYP = not(AGYN);
    bool BENY = nor(ASYP, in.APU_RESET, in.ELOX_Q);

    bool BACU = and(in.FF16, in.APU_WR);
    bool BUDU = not(BACU);
    BAMY.tock(BUDU, in.APU_RESET2n, in.D7);
    BERA.tock(BUDU, in.APU_RESET2n, in.D6);

    bool BYGO = not(in.CPU_RDn);
    bool CORO = nand(in.FF16, BYGO);
    bool CEKA = not(!BAMY_Q);
    bool CECY = not(!BERA_Q);

    out.BENY_OUT = BENY;
    out.FF16_D7 = BAMY_Q;
    out.FF16_D7n = !BAMY_Q;
    out.FF16_D6 = BERA_Q;
    out.FF16_D6n = !BERA_Q;

    if (CORO) {
      out.D_OE = true;
      out.D7 = CEKA;
      out.D6 = CECY;
    }

    //----------
    // FF17 NR22

    bool ENUF = and(in.FF17, in.CPU_WRQ);
    bool JYBU = not(in.APU_RESET);
    bool FYRY = not(in.FF17);
    bool GURU = or(FYRY, in.CPU_RDn);
    
    bool GURE = not(in.FF17);
    bool GEXA = or(GURE, in.CPU_RDn);
    bool GERE = and(in.CPU_WRQ, in.FF17);
    bool JEDE = not(GERE);

    bool GATA_Q = GATA.tock(ENUF, JYBU, in.D4);
    bool FORE_Q = FORE.tock(ENUF, JYBU, in.D3);
    bool GAGE_Q = GAGE.tock(ENUF, JYBU, in.D7);
    bool GURA_Q = GURA.tock(ENUF, JYBU, in.D6);
    bool GUFE_Q = GUFE.tock(ENUF, JYBU, in.D5);
    bool HAVA_Q = HAVA.tock(JEDE, JYBU, in.D2);
    bool HORE_Q = HORE.tock(JEDE, JYBU, in.D1);
    bool HYFU_Q = HYFU.tock(JEDE, JYBU, in.D0);

    bool HUPE = not(!GATA_Q);
    bool GENE = not(!FORE_Q);
    bool HYRY = not(!GAGE_Q);
    bool HORO = not(!GURA_Q);
    bool HERE = not(!GUFE_Q);
    bool HAVU = not(!HAVA_Q);
    bool HYRE = not(!HORE_Q);
    bool HUVU = not(!HYFU_Q);

    if (GURU) {
      out.D_OE = true;
      out.D4 = HUPE;
      out.D3 = GENE;
      out.D7 = HYRY;
      out.D6 = HORO;
      out.D5 = HERE;
    }

    if (GEXA) {
      out.D_OE = true;
      out.D2 = HAVU;
      out.D1 = HYRE;
      out.D0 = HUVU;
    }

    //----------
    // FF18 NR23

    bool DOSA = and(in.FF18, in.CPU_WRQ);
    bool HUDE = not(in.APU_RESET);
    bool ESUR = not(DOSA);
    bool EXUC = and(in.FF18, in.CPU_WRQ);
    bool FYXO = not(EXUC);
    bool FERY = not(in.DUCE);
    bool GUZA = nor(FERY, FAPE);
    bool FUTY = not(GUZA);

    bool FOFE_Q = FOFE.tock(ESUR, HUDE, in.D0);
    bool FOVA_Q = FOVA.tock(ESUR, HUDE, in.D1);
    bool FEDY_Q = FEDY.tock(ESUR, HUDE, in.D2);
    bool FOME_Q = FOME.tock(ESUR, HUDE, in.D3);
    bool FORA_Q = FORA.tock(ESUR, HUDE, in.D4);
    bool GODA_Q = GODA.tock(FYXO, HUDE, in.D5);
    bool GUMY_Q = GUMY.tock(FYXO, HUDE, in.D6);
    bool GUPU_Q = GUPU.tock(FYXO, HUDE, in.D7);

    bool DONE_Q = DONE.count(in.DOCA,  in.COGU_OUT, FOFE_Q);
    bool DYNU_Q = DYNU.count(DONE.q(), in.COGU_OUT, FOVA_Q);
    bool EZOF_Q = EZOF.count(DYNU.q(), in.COGU_OUT, FEDY_Q);
    bool CYVO_Q = CYVO.count(EZOF.q(), in.COGU_OUT, FOME_Q);

    // odd
    bool EDEP = not(!CYVO.q());

    bool FUXO_Q = FUXO.count(EDEP,     in.EROG_OUT, FORA_Q);
    bool GANO_Q = GANO.count(FUXO.q(), in.EROG_OUT, GODA_Q);
    bool GOCA_Q = GOCA.count(GANO.q(), in.EROG_OUT, GUMY_Q);
    bool GANE_Q = GANE.count(GOCA.q(), in.EROG_OUT, GUPU_Q);

    bool FAVA = not(!DONE_Q);
    bool FAJY = not(!DYNU_Q);
    bool FEGU = not(!EZOF_Q);
    bool FOSE = not(!CYVO_Q);
    bool GERO = not(!FUXO_Q);
    bool GAKY = not(!GANO_Q);
    bool GADU = not(!GOCA_Q);
    bool GAZO = not(!GANE_Q);

    if (FUTY) {
      out.D_OE = true;
      out.D0 = FAVA;
      out.D1 = FAJY;
      out.D2 = FEGU;
      out.D3 = FOSE;
      // wtf, d4 and d5 are switched on the schematic
      out.D5 = GERO;
      out.D4 = GAKY;
      out.D6 = GADU;
      out.D7 = GAZO;
    }

    //----------
    // FF19 NR24

    bool KYPU = not(in.APU_RESET);
    bool JENU = and(in.FF19, in.CPU_WRQ);
    bool KYSA = not(JENU);

    bool JUPY_Q = JUPY.tock(KYSA, KYPU, in.D2);
    bool JANY_Q = JANY.tock(KYSA, KYPU, in.D1);
    bool JEFU_Q = JEFU.tock(KYSA, KYPU, in.D0);

    out.FF19_D2 = JUPY_Q;
    out.FF19_D1 = JANY_Q;
    out.FF19_D0 = JEFU_Q;

    bool DETA = nand(in.CPU_WRQ, in.FF19);
    bool GOTE = not(in.FF19);
    bool ETAP_Q = ETAP.tock(DETA, in.DERA_OUT, in.D7);
    out.FF19_D0 = ETAP_Q;
    bool HYPO = or(GOTE, FAPE);

    bool GALA = not(in.GANE_QN);
    // FIXME daisy chain
    bool HEVY_Q = HEVY.count(GALA,     in.GYPA_OUT, out.FF19_D0);
    bool HEPU_Q = HEPU.count(HEVY.q(), in.GYPA_OUT, out.FF19_D1);
    bool HERO_Q = HERO.count(HEPU.q(), in.GYPA_OUT, out.FF19_D2);

    bool JEKE = not(!HERO_Q);
    bool JARO = not(!HEPU_Q);
    bool HUNA = not(!HEVY_Q);

    if (HYPO) {
      out.D_OE = true;
      out.D2 = JEKE;
      out.D1 = JARO;
      out.D0 = HUNA;
    }

    bool GADO = not(in.CPU_RDn);
    bool EVYF = nor(in.ANUJ, in.FF19);
    bool FAZO = not(in.APU_RESET);
    bool HUMA = nor(in.FF19, GADO);
    bool EMER_Q = EMER.tock(EVYF, FAZO, in.D6);
    bool GOJY = EMER_Q;

    out.FF19_D6 = EMER_Q;
    out.FF19_D6n = !EMER_Q;

    if (HUMA) {
      out.D_OE = true;
      out.D6 = GOJY;
    }
  }
};