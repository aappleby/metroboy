#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P36_Palettes {
  struct Input {
    bool CPU_RD2;
    bool CPU_WR2;
    bool FF47;
    bool FF48;
    bool FF49;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool BGP_D0,BGP_D1,BGP_D2,BGP_D3,BGP_D4,BGP_D5,BGP_D6,BGP_D7;
    bool OBP0_D0,OBP0_D1,OBP0_D2,OBP0_D3,OBP0_D4,OBP0_D5,OBP0_D6,OBP0_D7;
    bool OBP1_D0,OBP1_D1,OBP1_D2,OBP1_D3,OBP1_D4,OBP1_D5,OBP1_D6,OBP1_D7;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF47 BGP
  reg MENA,MORU,MAXY,NUSY,MOGY,MUKE,PYLU,PAVO;

  // FF48 OBP0
  reg XANA,XYZE,XALO,XUKY,XUPO,XERU,XOVA,XUFU;

  // FF49 OBP1
  reg LUXO,LUGU,LOSE,LAWO,LEPU,LUNE,MOSA,MOXY;


  void tick(const Input& in, Output& out) {
    // BGP
    bool VUSO = and(in.CPU_RD2, in.FF47);
    bool VELY = and(in.CPU_WR2, in.FF47);
    bool TEPY = not(VUSO);
    bool TEPO = not(VELY);

    bool MENA_Q = MENA.tock(TEPO, 0, in.D7);
    bool MORU_Q = MORU.tock(TEPO, 0, in.D5);
    bool MAXY_Q = MAXY.tock(TEPO, 0, in.D3);
    bool NUSY_Q = NUSY.tock(TEPO, 0, in.D1);
    bool MOGY_Q = MOGY.tock(TEPO, 0, in.D6);
    bool MUKE_Q = MUKE.tock(TEPO, 0, in.D4);
    bool PYLU_Q = PYLU.tock(TEPO, 0, in.D2);
    bool PAVO_Q = PAVO.tock(TEPO, 0, in.D0);

    out.BGP_D7 = MENA_Q;
    out.BGP_D5 = MORU_Q;
    out.BGP_D3 = MAXY_Q;
    out.BGP_D1 = NUSY_Q;
    out.BGP_D6 = MOGY_Q;
    out.BGP_D4 = MUKE_Q;
    out.BGP_D2 = PYLU_Q;
    out.BGP_D0 = PAVO_Q;

    bool LARY = not(!MENA_Q);
    bool LYKA = not(!MORU_Q);
    bool LOBE = not(!MAXY_Q);
    bool PABA = not(!NUSY_Q);
    bool LODY = not(!MOGY_Q);
    bool LACE = not(!MUKE_Q);
    bool REDO = not(!PYLU_Q);
    bool RARO = not(!PAVO_Q);

    if (TEPY) {
      out.D_OE = true;
      out.D7 = LARY;
      out.D5 = LYKA;
      out.D3 = LOBE;
      out.D1 = PABA;
      out.D6 = LODY;
      out.D4 = LACE;
      out.D2 = REDO;
      out.D0 = RARO;
    }

    // OBP0
    bool XUFY = and(in.CPU_RD2, in.FF48);
    bool XOMA = and(in.CPU_WR2, in.FF48);
    bool XOZY = not(XUFY);
    bool XELO = not(XOMA);

    bool XANA_Q = XANA.tock(XELO, 0, in.D7);
    bool XYZE_Q = XYZE.tock(XELO, 0, in.D5);
    bool XALO_Q = XALO.tock(XELO, 0, in.D3);
    bool XUKY_Q = XUKY.tock(XELO, 0, in.D1);
    bool XUPO_Q = XUPO.tock(XELO, 0, in.D6);
    bool XERU_Q = XERU.tock(XELO, 0, in.D4);
    bool XOVA_Q = XOVA.tock(XELO, 0, in.D2);
    bool XUFU_Q = XUFU.tock(XELO, 0, in.D0);

    out.OBP0_D7 = XANA_Q;
    out.OBP0_D5 = XYZE_Q;
    out.OBP0_D3 = XALO_Q;
    out.OBP0_D1 = XUKY_Q;
    out.OBP0_D6 = XUPO_Q;
    out.OBP0_D4 = XERU_Q;
    out.OBP0_D2 = XOVA_Q;
    out.OBP0_D0 = XUFU_Q;

    bool XAWO = not(!XANA_Q);
    bool XOBO = not(!XYZE_Q);
    bool XUBY = not(!XALO_Q);
    bool XOKE = not(!XUKY_Q);
    bool XAXA = not(!XUPO_Q);
    bool XAJU = not(!XERU_Q);
    bool XUNO = not(!XOVA_Q);
    bool XARY = not(!XUFU_Q);

    if (XOZY) {
      out.D_OE = true;
      out.D7 = XAWO;
      out.D5 = XOBO;
      out.D3 = XUBY;
      out.D1 = XOKE;
      out.D6 = XAXA;
      out.D4 = XAJU;
      out.D2 = XUNO;
      out.D0 = XARY;
    }

    // OBP1
    bool MUMY = and(in.CPU_RD2, in.FF49);
    bool MYXE = and(in.CPU_WR2, in.FF49);
    bool LOTE = not(MUMY);
    bool LEHO = not(MYXE);

    bool LUXO_Q = LUXO.tock(LEHO, 0, in.D7);
    bool LUGU_Q = LUGU.tock(LEHO, 0, in.D5);
    bool LOSE_Q = LOSE.tock(LEHO, 0, in.D3);
    bool LAWO_Q = LAWO.tock(LEHO, 0, in.D1);
    bool LEPU_Q = LEPU.tock(LEHO, 0, in.D6);
    bool LUNE_Q = LUNE.tock(LEHO, 0, in.D4);
    bool MOSA_Q = MOSA.tock(LEHO, 0, in.D2);
    bool MOXY_Q = MOXY.tock(LEHO, 0, in.D0);

    out.OBP1_D7 = LUXO_Q;
    out.OBP1_D5 = LUGU_Q;
    out.OBP1_D3 = LOSE_Q;
    out.OBP1_D1 = LAWO_Q;
    out.OBP1_D6 = LEPU_Q;
    out.OBP1_D4 = LUNE_Q;
    out.OBP1_D2 = MOSA_Q;
    out.OBP1_D0 = MOXY_Q;

    bool LELU = not(!LUXO_Q);
    bool LUGA = not(!LUGU_Q);
    bool LYZA = not(!LOSE_Q);
    bool LEPA = not(!LAWO_Q);
    bool LEBA = not(!LEPU_Q);
    bool LUKY = not(!LUNE_Q);
    bool LODE = not(!MOSA_Q);
    bool LAJU = not(!MOXY_Q);

    if (LOTE) {
      out.D_OE = true;
      out.D7 = LELU;
      out.D5 = LUGA;
      out.D3 = LYZA;
      out.D1 = LEPA;
      out.D6 = LEBA;
      out.D4 = LUKY;
      out.D2 = LODE;
      out.D0 = LAJU;
    }
  }};
