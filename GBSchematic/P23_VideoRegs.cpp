#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P23_VideoRegs {
  struct Input {
    bool CPU_RD2;
    bool CPU_WR2;

    bool RESET6;
    bool RESET7;
    bool RESET8;
    bool RESET9;

    bool FF40;
    bool FF42;
    bool FF43;
    bool FF44;
    bool FF45;
    bool FF4A;
    bool FF4B;

    bool V0,V1,V2,V3,V4,V5,V6,V7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool FF40_D0,FF40_D1,FF40_D2,FF40_D3,FF40_D4,FF40_D5,FF40_D6,FF40_D7;
    bool FF42_D0,FF42_D1,FF42_D2,FF42_D3,FF42_D4,FF42_D5,FF42_D6,FF42_D7;
    bool FF43_D0,FF43_D1,FF43_D2,FF43_D3,FF43_D4,FF43_D5,FF43_D6,FF43_D7;
    bool FF45_D0,FF45_D1,FF45_D2,FF45_D3,FF45_D4,FF45_D5,FF45_D6,FF45_D7;
    bool FF4A_D0,FF4A_D1,FF4A_D2,FF4A_D3,FF4A_D4,FF4A_D5,FF4A_D6,FF4A_D7;
    bool FF4B_D0,FF4B_D1,FF4B_D2,FF4B_D3,FF4B_D4,FF4B_D5,FF4B_D6,FF4B_D7;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  reg XYLO,XAFO,XYMO,XONA,WYMO,WEXU,WOKY,VYXE; // FF40 LCDC
  reg FEZU,FUNY,DEDE,FOTY,FOHA,FYMO,GAVE,FUJO; // FF42 SCY
  reg CYXU,BAKE,BEMY,CUZY,CABU,DUZU,DATY,GUBO; // FF43 SCX
  reg SOTA,SYRY,SALO,SEDY,VEVO,VAFA,VUCE,RAHA; // FF45 LYC
  reg NAFU,MELA,NYRO,NAGA,NULO,NUKA,NENE,NESO; // FF4A WY
  reg MYCE,MYPA,NOFE,NUKU,MYPU,MUVO,MEBY,NOKE; // FF4B WX

  void tick_videoregs(const Input& in, Output& out) {
    // FF40 LCDC
    bool VYRE = and(in.FF40, in.CPU_RD2);
    bool WARU = and(in.FF40, in.CPU_WR2);
    bool WYCE = not(VYRE);
    bool XUBO = not(WARU);
    bool XARE = not(in.RESET7);

    bool XYLO_Q = XYLO.tock(XUBO, XARE, in.D1);
    bool XAFO_Q = XAFO.tock(XUBO, XARE, in.D3);
    bool XYMO_Q = XYMO.tock(XUBO, XARE, in.D2);
    bool XONA_Q = XONA.tock(XUBO, XARE, in.D7);
    bool WYMO_Q = WYMO.tock(XUBO, XARE, in.D5);
    bool WEXU_Q = WEXU.tock(XUBO, XARE, in.D4);
    bool WOKY_Q = WOKY.tock(XUBO, XARE, in.D6);
    bool VYXE_Q = VYXE.tock(XUBO, XARE, in.D0);

    out.FF40_D1 = XYLO_Q;
    out.FF40_D3 = XAFO_Q;
    out.FF40_D2 = XYMO_Q;
    out.FF40_D7 = XONA_Q;
    out.FF40_D5 = WYMO_Q;
    out.FF40_D4 = WEXU_Q;
    out.FF40_D6 = WOKY_Q;
    out.FF40_D0 = VYXE_Q;

    if (WYCE) {
      out.D_OE = true;
      out.D1 = XYLO_Q;
      out.D3 = XAFO_Q;
      out.D2 = XYMO_Q;
      out.D7 = XONA_Q;
      out.D5 = WYMO_Q;
      out.D5 = WEXU_Q;
      out.D6 = WOKY_Q;
      out.D0 = VYXE_Q;
    }

    // FF4B WX
    bool WYZE = and(in.CPU_RD2, in.FF4B);
    bool WUZA = and(in.CPU_WR2, in.FF4B);
    bool VYCU = not(WYZE);
    bool VOXU = not(WUZA);

    bool MYCE_Q = MYCE.tock(VOXU, in.RESET8, in.D5);
    bool MYPA_Q = MYPA.tock(VOXU, in.RESET8, in.D0);
    bool NOFE_Q = NOFE.tock(VOXU, in.RESET8, in.D1);
    bool NUKU_Q = NUKU.tock(VOXU, in.RESET8, in.D7);
    bool MYPU_Q = MYPU.tock(VOXU, in.RESET8, in.D4);
    bool MUVO_Q = MUVO.tock(VOXU, in.RESET8, in.D6);
    bool MEBY_Q = MEBY.tock(VOXU, in.RESET8, in.D3);
    bool NOKE_Q = NOKE.tock(VOXU, in.RESET8, in.D2);

    bool MUFE = not(!MYCE_Q);
    bool LOVA = not(!MYPA_Q);
    bool MUKA = not(!NOFE_Q);
    bool MARA = not(!NUKU_Q);
    bool MELE = not(!MYPU_Q);
    bool MULY = not(!MUVO_Q);
    bool LOLE = not(!MEBY_Q);
    bool MOKO = not(!NOKE_Q);

    out.FF4B_D5 = MYCE_Q;
    out.FF4B_D0 = MYPA_Q;
    out.FF4B_D1 = NOFE_Q;
    out.FF4B_D7 = NUKU_Q;
    out.FF4B_D4 = MYPU_Q;
    out.FF4B_D6 = MUVO_Q;
    out.FF4B_D3 = MEBY_Q;
    out.FF4B_D2 = NOKE_Q;

    if (VYCU) {
      out.D_OE = true;
      out.D5 = MUFE;
      out.D0 = LOVA;
      out.D1 = MUKA;
      out.D7 = MARA;
      out.D4 = MELE;
      out.D6 = MULY;
      out.D3 = LOLE;
      out.D2 = MOKO;
    }

    // FF4A WY
    bool WAXU = and(in.CPU_RD2, in.FF4A);
    bool WEKO = and(in.CPU_WR2, in.FF4A);
    bool VEFU = not(WEKO);

    bool NAFU_Q = NAFU.tock(VEFU, in.RESET8, in.D7);
    bool MELA_Q = MELA.tock(VEFU, in.RESET8, in.D3);
    bool NYRO_Q = NYRO.tock(VEFU, in.RESET8, in.D1);
    bool NAGA_Q = NAGA.tock(VEFU, in.RESET8, in.D2);
    bool NULO_Q = NULO.tock(VEFU, in.RESET8, in.D4);
    bool NUKA_Q = NUKA.tock(VEFU, in.RESET8, in.D6);
    bool NENE_Q = NENE.tock(VEFU, in.RESET8, in.D5);
    bool NESO_Q = NESO.tock(VEFU, in.RESET8, in.D0);

    out.FF4B_D7 = NAFU_Q;
    out.FF4B_D3 = MELA_Q;
    out.FF4B_D1 = NYRO_Q;
    out.FF4B_D2 = NAGA_Q;
    out.FF4B_D4 = NULO_Q;
    out.FF4B_D6 = NUKA_Q;
    out.FF4B_D5 = NENE_Q;
    out.FF4B_D0 = NESO_Q;

    bool MERA = not(!NAFU_Q);
    bool LOKA = not(!MELA_Q);
    bool PODA = not(!NYRO_Q);
    bool PYGU = not(!NAGA_Q);
    bool MEGA = not(!NULO_Q);
    bool POLO = not(!NUKA_Q);
    bool PELA = not(!NENE_Q);
    bool PUNU = not(!NESO_Q);

    bool VOMY = not(WAXU);
    if (VOMY) {
      out.D_OE = true;
      out.D7 = MERA;
      out.D3 = LOKA;
      out.D1 = PODA;
      out.D2 = PYGU;
      out.D4 = MEGA;
      out.D6 = POLO;
      out.D5 = PELA;
      out.D0 = PUNU;
    }

    // FF43 SCX
    bool AVOG = and(in.FF43, in.CPU_RD2);
    bool ARUR = and(in.FF43, in.CPU_WR2);
    bool AMUN = not(ARUR);

    bool CYXU_Q = CYXU.tock(AMUN, in.RESET6, in.D2);
    bool BAKE_Q = BAKE.tock(AMUN, in.RESET6, in.D7);
    bool BEMY_Q = BEMY.tock(AMUN, in.RESET6, in.D4);
    bool CUZY_Q = CUZY.tock(AMUN, in.RESET6, in.D5);
    bool CABU_Q = CABU.tock(AMUN, in.RESET6, in.D6);
    bool DUZU_Q = DUZU.tock(AMUN, in.RESET6, in.D1);
    bool DATY_Q = DATY.tock(AMUN, in.RESET6, in.D0);
    bool GUBO_Q = GUBO.tock(AMUN, in.RESET6, in.D3);

    out.FF43_D2 = CYXU_Q;
    out.FF43_D7 = BAKE_Q;
    out.FF43_D4 = BEMY_Q;
    out.FF43_D5 = CUZY_Q;
    out.FF43_D6 = CABU_Q;
    out.FF43_D1 = DUZU_Q;
    out.FF43_D0 = DATY_Q;
    out.FF43_D3 = GUBO_Q;

    bool CUGA = not(!CYXU_Q);
    bool CASY = not(!BAKE_Q);
    bool CEDU = not(!BEMY_Q);
    bool CATA = not(!CUZY_Q);
    bool DOXE = not(!CABU_Q);
    bool EKOB = not(!DUZU_Q);
    bool EDOS = not(!DATY_Q);
    bool WONY = not(!GUBO_Q);

    bool BEBA = not(AVOG);
    if (BEBA) {
      out.D_OE = true;
      out.D2 = CUGA;
      out.D7 = CASY;
      out.D4 = CEDU;
      out.D5 = CATA;
      out.D6 = DOXE;
      out.D1 = EKOB;
      out.D0 = EDOS;
      out.D3 = WONY;
    }

    // FF42 SCY
    bool ANYP = and(in.CPU_RD2, in.FF42);
    bool BEDY = and(in.CPU_WR2, in.FF42);
    bool CAVO = not(BEDY);

    bool FEZU_Q = FEZU.tock(CAVO, in.RESET6, in.D2);
    bool FUNY_Q = FUNY.tock(CAVO, in.RESET6, in.D7);
    bool DEDE_Q = DEDE.tock(CAVO, in.RESET6, in.D4);
    bool FOTY_Q = FOTY.tock(CAVO, in.RESET6, in.D5);
    bool FOHA_Q = FOHA.tock(CAVO, in.RESET6, in.D6);
    bool FYMO_Q = FYMO.tock(CAVO, in.RESET6, in.D1);
    bool GAVE_Q = GAVE.tock(CAVO, in.RESET6, in.D0);
    bool FUJO_Q = FUJO.tock(CAVO, in.RESET6, in.D3);

    out.FF42_D2 = FEZU_Q;
    out.FF42_D7 = FUNY_Q;
    out.FF42_D4 = DEDE_Q;
    out.FF42_D5 = FOTY_Q;
    out.FF42_D6 = FOHA_Q;
    out.FF42_D1 = FYMO_Q;
    out.FF42_D0 = GAVE_Q;
    out.FF42_D3 = FUJO_Q;

    bool GONU = not(!FEZU_Q);
    bool GYZA = not(!FUNY_Q);
    bool CUSA = not(!DEDE_Q);
    bool GYZO = not(!FOTY_Q);
    bool GUNE = not(!FOHA_Q);
    bool GOBA = not(!FYMO_Q);
    bool WARE = not(!GAVE_Q);
    bool GODO = not(!FUJO_Q);

    bool BUWY = not(ANYP);
    if (BUWY) {
      out.D_OE = true;
      out.D2 = GONU;
      out.D7 = GYZA;
      out.D4 = CUSA;
      out.D5 = GYZO;
      out.D6 = GUNE;
      out.D1 = GOBA;
      out.D0 = WARE;
      out.D3 = GODO;
    }

    // FF45 LYC
    bool XYLY = and(in.CPU_RD2, in.FF45);
    bool XUFA = and(in.CPU_WR2, in.FF45);
    bool WANE = not(XUFA);

    bool SOTA_Q = SOTA.tock(WANE, in.RESET9, in.D4);
    bool SYRY_Q = SYRY.tock(WANE, in.RESET9, in.D0);
    bool SALO_Q = SALO.tock(WANE, in.RESET9, in.D3);
    bool SEDY_Q = SEDY.tock(WANE, in.RESET9, in.D2);
    bool VEVO_Q = VEVO.tock(WANE, in.RESET9, in.D6);
    bool VAFA_Q = VAFA.tock(WANE, in.RESET9, in.D5);
    bool VUCE_Q = VUCE.tock(WANE, in.RESET9, in.D1);
    bool RAHA_Q = RAHA.tock(WANE, in.RESET9, in.D7);

    out.FF45_D4 = SOTA_Q;
    out.FF45_D0 = SYRY_Q;
    out.FF45_D3 = SALO_Q;
    out.FF45_D2 = SEDY_Q;
    out.FF45_D6 = VEVO_Q;
    out.FF45_D5 = VAFA_Q;
    out.FF45_D1 = VUCE_Q;
    out.FF45_D7 = RAHA_Q;

    bool RACE = not(!SOTA_Q);
    bool RETU = not(!SYRY_Q);
    bool REDY = not(!SALO_Q);
    bool RAZU = not(!SEDY_Q);
    bool VAFE = not(!VEVO_Q);
    bool VAZU = not(!VAFA_Q);
    bool VOJO = not(!VUCE_Q);
    bool PUFY = not(!RAHA_Q);

    bool WEKU = not(XYLY);
    if (WEKU) {
      out.D_OE = true;
      out.D4 = RACE;
      out.D0 = RETU;
      out.D3 = REDY;
      out.D2 = RAZU;
      out.D6 = VAFE;
      out.D5 = VAZU;
      out.D1 = VOJO;
      out.D7 = PUFY;
    }

    // FF44 LY
    bool WAFU = and(in.CPU_RD2, in.FF44);

    bool WATA = not(in.V4);
    bool XAGA = not(in.V5);
    bool XEPO = not(in.V1);
    bool XUHY = not(in.V3);
    bool XOWO = not(in.V7);
    bool XUCE = not(in.V6);
    bool WURY = not(in.V0);
    bool MYFA = not(in.V2);

    bool VYNE = not(WATA);
    bool WAMA = not(XAGA);
    bool WUVA = not(XEPO);
    bool WOJY = not(XUHY);
    bool WEZE = not(XOWO);
    bool WAVO = not(XUCE);
    bool VEGA = not(WURY);
    bool LYCO = not(MYFA);

    bool VARO = not(WAFU);
    if (VARO) {
      out.D_OE = true;
      out.D4 = VYNE;
      out.D5 = WAMA;
      out.D1 = WUVA;
      out.D3 = WOJY;
      out.D7 = WEZE;
      out.D6 = WAVO;
      out.D0 = VEGA;
      out.D2 = LYCO;
    }
  }
};