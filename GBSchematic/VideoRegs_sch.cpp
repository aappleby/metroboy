// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "MemBus.h"
#include "PpuRegs.h"
#include "CpuBus.h"
#include "Clocks.h"

//----------
// inputs

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

//----------
// registers

//-----------------------------------------------------------------------------

void tick_videoregs() {
  // FF40 LCDC
  bool VYRE = and(FF40, cpu.CPU_RD2);
  WARU = and(FF40, cpu.CPU_WR2);
  bool XUBO = not(WARU);
  XARE = not(rst.RESET7);

  bool XYLO_Q = XYLO.tock(XUBO, XARE, mem.D1);
  bool XAFO_Q = XYLO.tock(XUBO, XARE, mem.D3);
  bool XYMO_Q = XYLO.tock(XUBO, XARE, mem.D2);
  bool XONA_Q = XYLO.tock(XUBO, XARE, mem.D7);
  bool WYMO_Q = XYLO.tock(XUBO, XARE, mem.D5);
  bool WEXU_Q = XYLO.tock(XUBO, XARE, mem.D4);
  bool WOKY_Q = XYLO.tock(XUBO, XARE, mem.D6);
  bool VYXE_Q = XYLO.tock(XUBO, XARE, mem.D0);
 
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
    mem.D1 = XERO;
    mem.D3 = WUKA;
    mem.D2 = WYJU;
    mem.D7 = XEBU;
    mem.D5 = VATO;
    mem.D5 = VOKE;
    mem.D6 = VAHA;
    mem.D0 = WYPO;
  }

  // FF4B WX
  bool WYZE = and(cpu.CPU_RD2, FF4B);
  bool WUZA = and(cpu.CPU_WR2, FF4B);
  bool VYCU = not(WYZE);
  bool VOXU = not(WUZA);

  bool MYCE_Q = MYCE.tock(VOXU, rst.RESET8, mem.D5);
  bool MYPA_Q = MYPA.tock(VOXU, rst.RESET8, mem.D0);
  bool NOFE_Q = NOFE.tock(VOXU, rst.RESET8, mem.D1);
  bool NUKU_Q = NUKU.tock(VOXU, rst.RESET8, mem.D7);
  bool MYPU_Q = MYPU.tock(VOXU, rst.RESET8, mem.D4);
  bool MUVO_Q = MUVO.tock(VOXU, rst.RESET8, mem.D6);
  bool MEBY_Q = MEBY.tock(VOXU, rst.RESET8, mem.D3);
  bool NOKE_Q = NOKE.tock(VOXU, rst.RESET8, mem.D2);

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
    mem.D5 = MUFE;
    mem.D0 = LOVA;
    mem.D1 = MUKA;
    mem.D7 = MARA;
    mem.D4 = MELE;
    mem.D6 = MULY;
    mem.D3 = LOLE;
    mem.D2 = MOKO;
  }

  // FF4A WY
  bool WAXU = and(cpu.CPU_RD2, FF4A);
  bool WEKO = and(cpu.CPU_WR2, FF4A);
  bool VEFU = not(WEKO);

  bool NAFU_Q = NAFU.tock(VEFU, rst.RESET8, mem.D7);
  bool MELA_Q = MELA.tock(VEFU, rst.RESET8, mem.D3);
  bool NYRO_Q = NYRO.tock(VEFU, rst.RESET8, mem.D1);
  bool NAGA_Q = NAGA.tock(VEFU, rst.RESET8, mem.D2);
  bool NULO_Q = NULO.tock(VEFU, rst.RESET8, mem.D4);
  bool NUKA_Q = NUKA.tock(VEFU, rst.RESET8, mem.D6);
  bool NENE_Q = NENE.tock(VEFU, rst.RESET8, mem.D5);
  bool NESO_Q = NESO.tock(VEFU, rst.RESET8, mem.D0);

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
    mem.D7 = MERA;
    mem.D3 = LOKA;
    mem.D1 = PODA;
    mem.D2 = PYGU;
    mem.D4 = MEGA;
    mem.D6 = POLO;
    mem.D5 = PELA;
    mem.D0 = PUNU;
  }

  // FF43 SCX
  bool AVOG = and(FF43, cpu.CPU_RD2);
  bool ARUR = and(FF43, cpu.CPU_WR2);
  bool AMUN = not(ARUR);

  bool CYXU_Q = CYXU.tock(AMUN, rst.RESET6, mem.D2);
  bool BAKE_Q = BAKE.tock(AMUN, rst.RESET6, mem.D7);
  bool BEMY_Q = BEMY.tock(AMUN, rst.RESET6, mem.D4);
  bool CUZY_Q = CUZY.tock(AMUN, rst.RESET6, mem.D5);
  bool CABU_Q = CABU.tock(AMUN, rst.RESET6, mem.D6);
  bool DUZU_Q = DUZU.tock(AMUN, rst.RESET6, mem.D1);
  bool DATY_Q = DATY.tock(AMUN, rst.RESET6, mem.D0);
  bool GUBO_Q = GUBO.tock(AMUN, rst.RESET6, mem.D3);

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
    mem.D2 = CUGA;
    mem.D7 = CASY;
    mem.D4 = CEDU;
    mem.D5 = CATA;
    mem.D6 = DOXE;
    mem.D1 = EKOB;
    mem.D0 = EDOS;
    mem.D3 = WONY;
  }

  // FF42 SCY
  bool ANYP = and(cpu.CPU_RD2, FF42);
  bool BEDY = and(cpu.CPU_WR2, FF42);
  bool CAVO = not(BEDY);

  bool FEZU_Q = FEZU.tock(CAVO, rst.RESET6, mem.D2);
  bool FUNY_Q = FUNY.tock(CAVO, rst.RESET6, mem.D7);
  bool DEDE_Q = DEDE.tock(CAVO, rst.RESET6, mem.D4);
  bool FOTY_Q = FOTY.tock(CAVO, rst.RESET6, mem.D5);
  bool FOHA_Q = FOHA.tock(CAVO, rst.RESET6, mem.D6);
  bool FYMO_Q = FYMO.tock(CAVO, rst.RESET6, mem.D1);
  bool GAVE_Q = GAVE.tock(CAVO, rst.RESET6, mem.D0);
  bool FUJO_Q = FUJO.tock(CAVO, rst.RESET6, mem.D3);

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
    mem.D2 = GONU;
    mem.D7 = GYZA;
    mem.D4 = CUSA;
    mem.D5 = GYZO;
    mem.D6 = GUNE;
    mem.D1 = GOBA;
    mem.D0 = WARE;
    mem.D3 = GODO;
  }

  // FF45 LYC
  bool XYLY = and(cpu.CPU_RD2, FF45);
  bool XUFA = and(cpu.CPU_WR2, FF45);
  bool WANE = not(XUFA);

  bool SOTA_Q = SOTA.tock(WANE, rst.RESET9, mem.D4);
  bool SYRY_Q = SYRY.tock(WANE, rst.RESET9, mem.D0);
  bool SALO_Q = SALO.tock(WANE, rst.RESET9, mem.D3);
  bool SEDY_Q = SEDY.tock(WANE, rst.RESET9, mem.D2);
  bool VEVO_Q = VEVO.tock(WANE, rst.RESET9, mem.D6);
  bool VAFA_Q = VAFA.tock(WANE, rst.RESET9, mem.D5);
  bool VUCE_Q = VUCE.tock(WANE, rst.RESET9, mem.D1);
  bool RAHA_Q = RAHA.tock(WANE, rst.RESET9, mem.D7);

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
    mem.D4 = RACE;
    mem.D0 = RETU;
    mem.D3 = REDY;
    mem.D2 = RAZU;
    mem.D6 = VAFE;
    mem.D5 = VAZU;
    mem.D1 = VOJO;
    mem.D7 = PUFY;
  }

  // FF44 LY
  bool WAFU = and(cpu.CPU_RD2, FF44);

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
    mem.D4 = VYNE;
    mem.D5 = WAMA;
    mem.D1 = WUVA;
    mem.D3 = WOJY;
    mem.D7 = WEZE;
    mem.D6 = WAVO;
    mem.D0 = VEGA;
    mem.D2 = LYCO;
  }
}