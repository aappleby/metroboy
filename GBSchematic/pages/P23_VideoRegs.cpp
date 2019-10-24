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
    bool FF40_D0,FF40_D1,FF40_D2,FF40_D3,FF40_D4,FF40_D5,FF40_D6,FF40_D7; // FF40 LCDC
    bool FF42_D0,FF42_D1,FF42_D2,FF42_D3,FF42_D4,FF42_D5,FF42_D6,FF42_D7; // FF42 SCY
    bool FF43_D0,FF43_D1,FF43_D2,FF43_D3,FF43_D4,FF43_D5,FF43_D6,FF43_D7; // FF43 SCX
    bool FF45_D0,FF45_D1,FF45_D2,FF45_D3,FF45_D4,FF45_D5,FF45_D6,FF45_D7; // FF45 LYC
    bool FF4A_D0,FF4A_D1,FF4A_D2,FF4A_D3,FF4A_D4,FF4A_D5,FF4A_D6,FF4A_D7; // FF4A WY
    bool FF4B_D0,FF4B_D1,FF4B_D2,FF4B_D3,FF4B_D4,FF4B_D5,FF4B_D6,FF4B_D7; // FF4B WX

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
    wire VYRE = and(in.FF40, in.CPU_RD2);
    wire WARU = and(in.FF40, in.CPU_WR2);
    wire WYCE = not(VYRE);
    wire XUBO = not(WARU);
    wire XARE = not(in.RESET7);

    wire XYLO_Q = XYLO.tock(XUBO, XARE, in.D1);
    wire XAFO_Q = XAFO.tock(XUBO, XARE, in.D3);
    wire XYMO_Q = XYMO.tock(XUBO, XARE, in.D2);
    wire XONA_Q = XONA.tock(XUBO, XARE, in.D7);
    wire WYMO_Q = WYMO.tock(XUBO, XARE, in.D5);
    wire WEXU_Q = WEXU.tock(XUBO, XARE, in.D4);
    wire WOKY_Q = WOKY.tock(XUBO, XARE, in.D6);
    wire VYXE_Q = VYXE.tock(XUBO, XARE, in.D0);

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
    wire WYZE = and(in.CPU_RD2, in.FF4B);
    wire WUZA = and(in.CPU_WR2, in.FF4B);
    wire VYCU = not(WYZE);
    wire VOXU = not(WUZA);

    wire MYCE_Q = MYCE.tock(VOXU, in.RESET8, in.D5);
    wire MYPA_Q = MYPA.tock(VOXU, in.RESET8, in.D0);
    wire NOFE_Q = NOFE.tock(VOXU, in.RESET8, in.D1);
    wire NUKU_Q = NUKU.tock(VOXU, in.RESET8, in.D7);
    wire MYPU_Q = MYPU.tock(VOXU, in.RESET8, in.D4);
    wire MUVO_Q = MUVO.tock(VOXU, in.RESET8, in.D6);
    wire MEBY_Q = MEBY.tock(VOXU, in.RESET8, in.D3);
    wire NOKE_Q = NOKE.tock(VOXU, in.RESET8, in.D2);

    wire MUFE = not(!MYCE_Q);
    wire LOVA = not(!MYPA_Q);
    wire MUKA = not(!NOFE_Q);
    wire MARA = not(!NUKU_Q);
    wire MELE = not(!MYPU_Q);
    wire MULY = not(!MUVO_Q);
    wire LOLE = not(!MEBY_Q);
    wire MOKO = not(!NOKE_Q);

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
    wire WAXU = and(in.CPU_RD2, in.FF4A);
    wire WEKO = and(in.CPU_WR2, in.FF4A);
    wire VEFU = not(WEKO);

    wire NAFU_Q = NAFU.tock(VEFU, in.RESET8, in.D7);
    wire MELA_Q = MELA.tock(VEFU, in.RESET8, in.D3);
    wire NYRO_Q = NYRO.tock(VEFU, in.RESET8, in.D1);
    wire NAGA_Q = NAGA.tock(VEFU, in.RESET8, in.D2);
    wire NULO_Q = NULO.tock(VEFU, in.RESET8, in.D4);
    wire NUKA_Q = NUKA.tock(VEFU, in.RESET8, in.D6);
    wire NENE_Q = NENE.tock(VEFU, in.RESET8, in.D5);
    wire NESO_Q = NESO.tock(VEFU, in.RESET8, in.D0);

    out.FF4B_D7 = NAFU_Q;
    out.FF4B_D3 = MELA_Q;
    out.FF4B_D1 = NYRO_Q;
    out.FF4B_D2 = NAGA_Q;
    out.FF4B_D4 = NULO_Q;
    out.FF4B_D6 = NUKA_Q;
    out.FF4B_D5 = NENE_Q;
    out.FF4B_D0 = NESO_Q;

    wire MERA = not(!NAFU_Q);
    wire LOKA = not(!MELA_Q);
    wire PODA = not(!NYRO_Q);
    wire PYGU = not(!NAGA_Q);
    wire MEGA = not(!NULO_Q);
    wire POLO = not(!NUKA_Q);
    wire PELA = not(!NENE_Q);
    wire PUNU = not(!NESO_Q);

    wire VOMY = not(WAXU);
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
    wire AVOG = and(in.FF43, in.CPU_RD2);
    wire ARUR = and(in.FF43, in.CPU_WR2);
    wire AMUN = not(ARUR);

    wire CYXU_Q = CYXU.tock(AMUN, in.RESET6, in.D2);
    wire BAKE_Q = BAKE.tock(AMUN, in.RESET6, in.D7);
    wire BEMY_Q = BEMY.tock(AMUN, in.RESET6, in.D4);
    wire CUZY_Q = CUZY.tock(AMUN, in.RESET6, in.D5);
    wire CABU_Q = CABU.tock(AMUN, in.RESET6, in.D6);
    wire DUZU_Q = DUZU.tock(AMUN, in.RESET6, in.D1);
    wire DATY_Q = DATY.tock(AMUN, in.RESET6, in.D0);
    wire GUBO_Q = GUBO.tock(AMUN, in.RESET6, in.D3);

    out.FF43_D2 = CYXU_Q;
    out.FF43_D7 = BAKE_Q;
    out.FF43_D4 = BEMY_Q;
    out.FF43_D5 = CUZY_Q;
    out.FF43_D6 = CABU_Q;
    out.FF43_D1 = DUZU_Q;
    out.FF43_D0 = DATY_Q;
    out.FF43_D3 = GUBO_Q;

    wire CUGA = not(!CYXU_Q);
    wire CASY = not(!BAKE_Q);
    wire CEDU = not(!BEMY_Q);
    wire CATA = not(!CUZY_Q);
    wire DOXE = not(!CABU_Q);
    wire EKOB = not(!DUZU_Q);
    wire EDOS = not(!DATY_Q);
    wire WONY = not(!GUBO_Q);

    wire BEBA = not(AVOG);
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
    wire ANYP = and(in.CPU_RD2, in.FF42);
    wire BEDY = and(in.CPU_WR2, in.FF42);
    wire CAVO = not(BEDY);

    wire FEZU_Q = FEZU.tock(CAVO, in.RESET6, in.D2);
    wire FUNY_Q = FUNY.tock(CAVO, in.RESET6, in.D7);
    wire DEDE_Q = DEDE.tock(CAVO, in.RESET6, in.D4);
    wire FOTY_Q = FOTY.tock(CAVO, in.RESET6, in.D5);
    wire FOHA_Q = FOHA.tock(CAVO, in.RESET6, in.D6);
    wire FYMO_Q = FYMO.tock(CAVO, in.RESET6, in.D1);
    wire GAVE_Q = GAVE.tock(CAVO, in.RESET6, in.D0);
    wire FUJO_Q = FUJO.tock(CAVO, in.RESET6, in.D3);

    out.FF42_D2 = FEZU_Q;
    out.FF42_D7 = FUNY_Q;
    out.FF42_D4 = DEDE_Q;
    out.FF42_D5 = FOTY_Q;
    out.FF42_D6 = FOHA_Q;
    out.FF42_D1 = FYMO_Q;
    out.FF42_D0 = GAVE_Q;
    out.FF42_D3 = FUJO_Q;

    wire GONU = not(!FEZU_Q);
    wire GYZA = not(!FUNY_Q);
    wire CUSA = not(!DEDE_Q);
    wire GYZO = not(!FOTY_Q);
    wire GUNE = not(!FOHA_Q);
    wire GOBA = not(!FYMO_Q);
    wire WARE = not(!GAVE_Q);
    wire GODO = not(!FUJO_Q);

    wire BUWY = not(ANYP);
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
    wire XYLY = and(in.CPU_RD2, in.FF45);
    wire XUFA = and(in.CPU_WR2, in.FF45);
    wire WANE = not(XUFA);

    wire SOTA_Q = SOTA.tock(WANE, in.RESET9, in.D4);
    wire SYRY_Q = SYRY.tock(WANE, in.RESET9, in.D0);
    wire SALO_Q = SALO.tock(WANE, in.RESET9, in.D3);
    wire SEDY_Q = SEDY.tock(WANE, in.RESET9, in.D2);
    wire VEVO_Q = VEVO.tock(WANE, in.RESET9, in.D6);
    wire VAFA_Q = VAFA.tock(WANE, in.RESET9, in.D5);
    wire VUCE_Q = VUCE.tock(WANE, in.RESET9, in.D1);
    wire RAHA_Q = RAHA.tock(WANE, in.RESET9, in.D7);

    out.FF45_D4 = SOTA_Q;
    out.FF45_D0 = SYRY_Q;
    out.FF45_D3 = SALO_Q;
    out.FF45_D2 = SEDY_Q;
    out.FF45_D6 = VEVO_Q;
    out.FF45_D5 = VAFA_Q;
    out.FF45_D1 = VUCE_Q;
    out.FF45_D7 = RAHA_Q;

    wire RACE = not(!SOTA_Q);
    wire RETU = not(!SYRY_Q);
    wire REDY = not(!SALO_Q);
    wire RAZU = not(!SEDY_Q);
    wire VAFE = not(!VEVO_Q);
    wire VAZU = not(!VAFA_Q);
    wire VOJO = not(!VUCE_Q);
    wire PUFY = not(!RAHA_Q);

    wire WEKU = not(XYLY);
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
    wire WAFU = and(in.CPU_RD2, in.FF44);

    wire WATA = not(in.V4);
    wire XAGA = not(in.V5);
    wire XEPO = not(in.V1);
    wire XUHY = not(in.V3);
    wire XOWO = not(in.V7);
    wire XUCE = not(in.V6);
    wire WURY = not(in.V0);
    wire MYFA = not(in.V2);

    wire VYNE = not(WATA);
    wire WAMA = not(XAGA);
    wire WUVA = not(XEPO);
    wire WOJY = not(XUHY);
    wire WEZE = not(XOWO);
    wire WAVO = not(XUCE);
    wire VEGA = not(WURY);
    wire LYCO = not(MYFA);

    wire VARO = not(WAFU);
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