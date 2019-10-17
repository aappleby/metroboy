// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CPU_RD2;
extern bool CPU_WR2;

extern bool RESET6;
extern bool RESET7;
extern bool RESET8;
extern bool RESET9;

extern bool FF40;
extern bool FF42;
extern bool FF43;
extern bool FF44;
extern bool FF45;
extern bool FF4A;
extern bool FF4B;

//----------
// outputs

bool WARU;
bool XARE;

// FF40 LCDC
bool FF40_D0;
bool FF40_D1;
bool FF40_D2;
bool FF40_D3;
bool FF40_D4;
bool FF40_D5;
bool FF40_D6;
bool FF40_D7;

// FF42 SCY
bool FF42_D0;
bool FF42_D1;
bool FF42_D2;
bool FF42_D3;
bool FF42_D4;
bool FF42_D5;
bool FF42_D6;
bool FF42_D7;

// FF43 SCX
bool FF43_D0;
bool FF43_D1;
bool FF43_D2;
bool FF43_D3;
bool FF43_D4;
bool FF43_D5;
bool FF43_D6;
bool FF43_D7;

// FF45 LYC
bool FF45_D0;
bool FF45_D1;
bool FF45_D2;
bool FF45_D3;
bool FF45_D4;
bool FF45_D5;
bool FF45_D6;
bool FF45_D7;

// FF4A WY
bool FF4A_D0;
bool FF4A_D1;
bool FF4A_D2;
bool FF4A_D3;
bool FF4A_D4;
bool FF4A_D5;
bool FF4A_D6;
bool FF4A_D7;

// FF4B WX
bool FF4B_D0;
bool FF4B_D1;
bool FF4B_D2;
bool FF4B_D3;
bool FF4B_D4;
bool FF4B_D5;
bool FF4B_D6;
bool FF4B_D7;

//----------
// registers

// FF40 LCDC
static reg XYLO;
static reg XAFO;
static reg XYMO;
static reg XONA;
static reg WYMO;
static reg WEXU;
static reg WOKY;
static reg VYXE;

// FF42 SCY
static reg FEZU;
static reg FUNY;
static reg DEDE;
static reg FOTY;
static reg FOHA;
static reg FYMO;
static reg GAVE;
static reg FUJO;

// FF43 SCX
static reg CYXU;
static reg BAKE;
static reg BEMY;
static reg CUZY;
static reg CABU;
static reg DUZU;
static reg DATY;
static reg GUBO;

// FF45 LYC
static reg SOTA;
static reg SYRY;
static reg SALO;
static reg SEDY;
static reg VEVO;
static reg VAFA;
static reg VUCE;
static reg RAHA;

// FF4A WY
static reg NAFU;
static reg MELA;
static reg NYRO;
static reg NAGA;
static reg NULO;
static reg NUKA;
static reg NENE;
static reg NESO;

// FF4B WX
static reg MYCE;
static reg MYPA;
static reg NOFE;
static reg NUKU;
static reg MYPU;
static reg MUVO;
static reg MEBY;
static reg NOKE;

//-----------------------------------------------------------------------------

void tick_videoregs() {
  // FF40 LCDC
  bool VYRE = and(FF40, CPU_RD2);
  WARU = and(FF40, CPU_WR2);
  bool XUBO = not(WARU);
  XARE = not(RESET7);

  bool XYLO_Q = XYLO.tock(XUBO, XARE, D1);
  bool XAFO_Q = XYLO.tock(XUBO, XARE, D3);
  bool XYMO_Q = XYLO.tock(XUBO, XARE, D2);
  bool XONA_Q = XYLO.tock(XUBO, XARE, D7);
  bool WYMO_Q = XYLO.tock(XUBO, XARE, D5);
  bool WEXU_Q = XYLO.tock(XUBO, XARE, D4);
  bool WOKY_Q = XYLO.tock(XUBO, XARE, D6);
  bool VYXE_Q = XYLO.tock(XUBO, XARE, D0);
 
  FF40_D1 = XYLO_Q;
  FF40_D3 = XAFO_Q;
  FF40_D2 = XYMO_Q;
  FF40_D7 = XONA_Q;
  FF40_D5 = WYMO_Q;
  FF40_D4 = WEXU_Q;
  FF40_D6 = WOKY_Q;
  FF40_D0 = VYXE_Q;

  bool XERO = not(!XYLO_Q);
  bool WUKA = not(!XAFO_Q);
  bool WYJU = not(!XYMO_Q);
  bool XEBU = not(!XONA_Q);
  bool VATO = not(!WYMO_Q);
  bool VOKE = not(!WEXU_Q);
  bool VAHA = not(!WOKY_Q);
  bool WYPO = not(!VYXE_Q);

  bool WYCE = not(VYRE);
  if (WYCE) {
    D1 = XERO;
    D3 = WUKA;
    D2 = WYJU;
    D7 = XEBU;
    D5 = VATO;
    D5 = VOKE;
    D6 = VAHA;
    D0 = WYPO;
  }

  // FF4B WX
  bool WYZE = and(CPU_RD2, FF4B);
  bool WUZA = and(CPU_WR2, FF4B);
  bool VYCU = not(WYZE);
  bool VOXU = not(WUZA);

  bool MYCE_Q = MYCE.tock(VOXU, RESET8, D5);
  bool MYPA_Q = MYPA.tock(VOXU, RESET8, D0);
  bool NOFE_Q = NOFE.tock(VOXU, RESET8, D1);
  bool NUKU_Q = NUKU.tock(VOXU, RESET8, D7);
  bool MYPU_Q = MYPU.tock(VOXU, RESET8, D4);
  bool MUVO_Q = MUVO.tock(VOXU, RESET8, D6);
  bool MEBY_Q = MEBY.tock(VOXU, RESET8, D3);
  bool NOKE_Q = NOKE.tock(VOXU, RESET8, D2);

  FF4B_D5 = MYCE_Q;
  FF4B_D0 = MYPA_Q;
  FF4B_D1 = NOFE_Q;
  FF4B_D7 = NUKU_Q;
  FF4B_D4 = MYPU_Q;
  FF4B_D6 = MUVO_Q;
  FF4B_D3 = MEBY_Q;
  FF4B_D2 = NOKE_Q;

  bool MUFE = not(!MYCE_Q);
  bool LOVA = not(!MYPA_Q);
  bool MUKA = not(!NOFE_Q);
  bool MARA = not(!NUKU_Q);
  bool MELE = not(!MYPU_Q);
  bool MULY = not(!MUVO_Q);
  bool LOLE = not(!MEBY_Q);
  bool MOKO = not(!NOKE_Q);

  if (VYCU) {
    D5 = MUFE;
    D0 = LOVA;
    D1 = MUKA;
    D7 = MARA;
    D4 = MELE;
    D6 = MULY;
    D3 = LOLE;
    D2 = MOKO;
  }

  // FF4A WY
  bool WAXU = and(CPU_RD2, FF4A);
  bool WEKO = and(CPU_WR2, FF4A);
  bool VEFU = not(WEKO);

  bool NAFU_Q = NAFU.tock(VEFU, RESET8, D7);
  bool MELA_Q = MELA.tock(VEFU, RESET8, D3);
  bool NYRO_Q = NYRO.tock(VEFU, RESET8, D1);
  bool NAGA_Q = NAGA.tock(VEFU, RESET8, D2);
  bool NULO_Q = NULO.tock(VEFU, RESET8, D4);
  bool NUKA_Q = NUKA.tock(VEFU, RESET8, D6);
  bool NENE_Q = NENE.tock(VEFU, RESET8, D5);
  bool NESO_Q = NESO.tock(VEFU, RESET8, D0);

  FF4B_D7 = NAFU_Q;
  FF4B_D3 = MELA_Q;
  FF4B_D1 = NYRO_Q;
  FF4B_D2 = NAGA_Q;
  FF4B_D4 = NULO_Q;
  FF4B_D6 = NUKA_Q;
  FF4B_D5 = NENE_Q;
  FF4B_D0 = NESO_Q;

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
    D7 = MERA;
    D3 = LOKA;
    D1 = PODA;
    D2 = PYGU;
    D4 = MEGA;
    D6 = POLO;
    D5 = PELA;
    D0 = PUNU;
  }

  // FF43 SCX
  bool AVOG = and(FF43, CPU_RD2);
  bool ARUR = and(FF43, CPU_WR2);
  bool AMUN = not(ARUR);

  bool CYXU_Q = CYXU.tock(AMUN, RESET6, D2);
  bool BAKE_Q = BAKE.tock(AMUN, RESET6, D7);
  bool BEMY_Q = BEMY.tock(AMUN, RESET6, D4);
  bool CUZY_Q = CUZY.tock(AMUN, RESET6, D5);
  bool CABU_Q = CABU.tock(AMUN, RESET6, D6);
  bool DUZU_Q = DUZU.tock(AMUN, RESET6, D1);
  bool DATY_Q = DATY.tock(AMUN, RESET6, D0);
  bool GUBO_Q = GUBO.tock(AMUN, RESET6, D3);

  FF43_D2 = CYXU_Q;
  FF43_D7 = BAKE_Q;
  FF43_D4 = BEMY_Q;
  FF43_D5 = CUZY_Q;
  FF43_D6 = CABU_Q;
  FF43_D1 = DUZU_Q;
  FF43_D0 = DATY_Q;
  FF43_D3 = GUBO_Q;

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
    D2 = CUGA;
    D7 = CASY;
    D4 = CEDU;
    D5 = CATA;
    D6 = DOXE;
    D1 = EKOB;
    D0 = EDOS;
    D3 = WONY;
  }

  // FF42 SCY
  bool ANYP = and(CPU_RD2, FF42);
  bool BEDY = and(CPU_WR2, FF42);
  bool CAVO = not(BEDY);

  bool FEZU_Q = FEZU.tock(CAVO, RESET6, D2);
  bool FUNY_Q = FUNY.tock(CAVO, RESET6, D7);
  bool DEDE_Q = DEDE.tock(CAVO, RESET6, D4);
  bool FOTY_Q = FOTY.tock(CAVO, RESET6, D5);
  bool FOHA_Q = FOHA.tock(CAVO, RESET6, D6);
  bool FYMO_Q = FYMO.tock(CAVO, RESET6, D1);
  bool GAVE_Q = GAVE.tock(CAVO, RESET6, D0);
  bool FUJO_Q = FUJO.tock(CAVO, RESET6, D3);

  FF42_D2 = FEZU_Q;
  FF42_D7 = FUNY_Q;
  FF42_D4 = DEDE_Q;
  FF42_D5 = FOTY_Q;
  FF42_D6 = FOHA_Q;
  FF42_D1 = FYMO_Q;
  FF42_D0 = GAVE_Q;
  FF42_D3 = FUJO_Q;

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
    D2 = GONU;
    D7 = GYZA;
    D4 = CUSA;
    D5 = GYZO;
    D6 = GUNE;
    D1 = GOBA;
    D0 = WARE;
    D3 = GODO;
  }

  // FF45 LYC
  bool XYLY = and(CPU_RD2, FF45);
  bool XUFA = and(CPU_WR2, FF45);
  bool WANE = not(XUFA);

  bool SOTA_Q = SOTA.tock(WANE, RESET9, D4);
  bool SYRY_Q = SYRY.tock(WANE, RESET9, D0);
  bool SALO_Q = SALO.tock(WANE, RESET9, D3);
  bool SEDY_Q = SEDY.tock(WANE, RESET9, D2);
  bool VEVO_Q = VEVO.tock(WANE, RESET9, D6);
  bool VAFA_Q = VAFA.tock(WANE, RESET9, D5);
  bool VUCE_Q = VUCE.tock(WANE, RESET9, D1);
  bool RAHA_Q = RAHA.tock(WANE, RESET9, D7);

  FF45_D4 = SOTA_Q;
  FF45_D0 = SYRY_Q;
  FF45_D3 = SALO_Q;
  FF45_D2 = SEDY_Q;
  FF45_D6 = VEVO_Q;
  FF45_D5 = VAFA_Q;
  FF45_D1 = VUCE_Q;
  FF45_D7 = RAHA_Q;

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
    D4 = RACE;
    D0 = RETU;
    D3 = REDY;
    D2 = RAZU;
    D6 = VAFE;
    D5 = VAZU;
    D1 = VOJO;
    D7 = PUFY;
  }

  // FF44 LY
  bool WAFU = and(CPU_RD2, FF44);

  bool WATA = not(V4);
  bool XAGA = not(V5);
  bool XEPO = not(V1);
  bool XUHY = not(V3);
  bool XOWO = not(V7);
  bool XUCE = not(V6);
  bool WURY = not(V0);
  bool MYFA = not(V2);

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
    D4 = VYNE;
    D5 = WAMA;
    D1 = WUVA;
    D3 = WOJY;
    D7 = WEZE;
    D6 = WAVO;
    D0 = VEGA;
    D2 = LYCO;
  }
}