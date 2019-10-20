// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "MemBus.h"
#include "PPU.h"
#include "CpuBus.h"
#include "Clocks.h"
#include "AddressDecoder.h"

//-----------------------------------------------------------------------------

void tick_videoregs(const Resets& rst) {
  // FF40 LCDC
  bool WARU = and(dec.FF40, cpu.CPU_WR2);
  bool XUBO = not(WARU);

  bool XYLO_Q = ppu.XYLO.tock(XUBO, rst.XARE, mem.D1);
  bool XAFO_Q = ppu.XAFO.tock(XUBO, rst.XARE, mem.D3);
  bool XYMO_Q = ppu.XYMO.tock(XUBO, rst.XARE, mem.D2);
  bool XONA_Q = ppu.XONA.tock(XUBO, rst.XARE, mem.D7);
  bool WYMO_Q = ppu.WYMO.tock(XUBO, rst.XARE, mem.D5);
  bool WEXU_Q = ppu.WEXU.tock(XUBO, rst.XARE, mem.D4);
  bool WOKY_Q = ppu.WOKY.tock(XUBO, rst.XARE, mem.D6);
  bool VYXE_Q = ppu.VYXE.tock(XUBO, rst.XARE, mem.D0);
 
  ppu.FF40_D1 = XYLO_Q;
  ppu.FF40_D3 = XAFO_Q;
  ppu.FF40_D2 = XYMO_Q;
  ppu.FF40_D7 = XONA_Q;
  ppu.FF40_D5 = WYMO_Q;
  ppu.FF40_D4 = WEXU_Q;
  ppu.FF40_D6 = WOKY_Q;
  ppu.FF40_D0 = VYXE_Q;

  if (nand(dec.FF40, cpu.CPU_RD2)) {
    mem.D1 = XYLO_Q;
    mem.D3 = XAFO_Q;
    mem.D2 = XYMO_Q;
    mem.D7 = XONA_Q;
    mem.D5 = WYMO_Q;
    mem.D5 = WEXU_Q;
    mem.D6 = WOKY_Q;
    mem.D0 = VYXE_Q;
  }

  // FF4B WX
  bool WYZE = and(cpu.CPU_RD2, dec.FF4B);
  bool WUZA = and(cpu.CPU_WR2, dec.FF4B);
  bool VYCU = not(WYZE);
  bool VOXU = not(WUZA);

  ppu.FF4B_D0 = ppu.WX_R0.tock(VOXU, rst.RESET8, mem.D0);
  ppu.FF4B_D1 = ppu.WX_R1.tock(VOXU, rst.RESET8, mem.D1);
  ppu.FF4B_D2 = ppu.WX_R2.tock(VOXU, rst.RESET8, mem.D2);
  ppu.FF4B_D3 = ppu.WX_R3.tock(VOXU, rst.RESET8, mem.D3);
  ppu.FF4B_D4 = ppu.WX_R4.tock(VOXU, rst.RESET8, mem.D4);
  ppu.FF4B_D5 = ppu.WX_R5.tock(VOXU, rst.RESET8, mem.D5);
  ppu.FF4B_D6 = ppu.WX_R6.tock(VOXU, rst.RESET8, mem.D6);
  ppu.FF4B_D7 = ppu.WX_R7.tock(VOXU, rst.RESET8, mem.D7);

  if (VYCU) {
    mem.D0 = ppu.FF4B_D0;
    mem.D1 = ppu.FF4B_D1;
    mem.D2 = ppu.FF4B_D2;
    mem.D3 = ppu.FF4B_D3;
    mem.D4 = ppu.FF4B_D4;
    mem.D5 = ppu.FF4B_D5;
    mem.D6 = ppu.FF4B_D6;
    mem.D7 = ppu.FF4B_D7;
  }

  // FF4A WY
  bool WAXU = and(cpu.CPU_RD2, dec.FF4A);
  bool WEKO = and(cpu.CPU_WR2, dec.FF4A);
  bool VEFU = not(WEKO);

  bool NAFU_Q = ppu.NAFU.tock(VEFU, rst.RESET8, mem.D7);
  bool MELA_Q = ppu.MELA.tock(VEFU, rst.RESET8, mem.D3);
  bool NYRO_Q = ppu.NYRO.tock(VEFU, rst.RESET8, mem.D1);
  bool NAGA_Q = ppu.NAGA.tock(VEFU, rst.RESET8, mem.D2);
  bool NULO_Q = ppu.NULO.tock(VEFU, rst.RESET8, mem.D4);
  bool NUKA_Q = ppu.NUKA.tock(VEFU, rst.RESET8, mem.D6);
  bool NENE_Q = ppu.NENE.tock(VEFU, rst.RESET8, mem.D5);
  bool NESO_Q = ppu.NESO.tock(VEFU, rst.RESET8, mem.D0);

  ppu.FF4B_D7 = NAFU_Q;
  ppu.FF4B_D3 = MELA_Q;
  ppu.FF4B_D1 = NYRO_Q;
  ppu.FF4B_D2 = NAGA_Q;
  ppu.FF4B_D4 = NULO_Q;
  ppu.FF4B_D6 = NUKA_Q;
  ppu.FF4B_D5 = NENE_Q;
  ppu.FF4B_D0 = NESO_Q;

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
  bool AVOG = and(dec.FF43, cpu.CPU_RD2);
  bool ARUR = and(dec.FF43, cpu.CPU_WR2);
  bool AMUN = not(ARUR);

  bool CYXU_Q = ppu.CYXU.tock(AMUN, rst.RESET6, mem.D2);
  bool BAKE_Q = ppu.BAKE.tock(AMUN, rst.RESET6, mem.D7);
  bool BEMY_Q = ppu.BEMY.tock(AMUN, rst.RESET6, mem.D4);
  bool CUZY_Q = ppu.CUZY.tock(AMUN, rst.RESET6, mem.D5);
  bool CABU_Q = ppu.CABU.tock(AMUN, rst.RESET6, mem.D6);
  bool DUZU_Q = ppu.DUZU.tock(AMUN, rst.RESET6, mem.D1);
  bool DATY_Q = ppu.DATY.tock(AMUN, rst.RESET6, mem.D0);
  bool GUBO_Q = ppu.GUBO.tock(AMUN, rst.RESET6, mem.D3);

  ppu.FF43_D2 = CYXU_Q;
  ppu.FF43_D7 = BAKE_Q;
  ppu.FF43_D4 = BEMY_Q;
  ppu.FF43_D5 = CUZY_Q;
  ppu.FF43_D6 = CABU_Q;
  ppu.FF43_D1 = DUZU_Q;
  ppu.FF43_D0 = DATY_Q;
  ppu.FF43_D3 = GUBO_Q;

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
  bool ANYP = and(cpu.CPU_RD2, dec.FF42);
  bool BEDY = and(cpu.CPU_WR2, dec.FF42);
  bool CAVO = not(BEDY);

  bool FEZU_Q = ppu.FEZU.tock(CAVO, rst.RESET6, mem.D2);
  bool FUNY_Q = ppu.FUNY.tock(CAVO, rst.RESET6, mem.D7);
  bool DEDE_Q = ppu.DEDE.tock(CAVO, rst.RESET6, mem.D4);
  bool FOTY_Q = ppu.FOTY.tock(CAVO, rst.RESET6, mem.D5);
  bool FOHA_Q = ppu.FOHA.tock(CAVO, rst.RESET6, mem.D6);
  bool FYMO_Q = ppu.FYMO.tock(CAVO, rst.RESET6, mem.D1);
  bool GAVE_Q = ppu.GAVE.tock(CAVO, rst.RESET6, mem.D0);
  bool FUJO_Q = ppu.FUJO.tock(CAVO, rst.RESET6, mem.D3);

  ppu.FF42_D2 = FEZU_Q;
  ppu.FF42_D7 = FUNY_Q;
  ppu.FF42_D4 = DEDE_Q;
  ppu.FF42_D5 = FOTY_Q;
  ppu.FF42_D6 = FOHA_Q;
  ppu.FF42_D1 = FYMO_Q;
  ppu.FF42_D0 = GAVE_Q;
  ppu.FF42_D3 = FUJO_Q;

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
  bool XYLY = and(cpu.CPU_RD2, dec.FF45);
  bool XUFA = and(cpu.CPU_WR2, dec.FF45);
  bool WANE = not(XUFA);

  bool SOTA_Q = ppu.SOTA.tock(WANE, rst.RESET9, mem.D4);
  bool SYRY_Q = ppu.SYRY.tock(WANE, rst.RESET9, mem.D0);
  bool SALO_Q = ppu.SALO.tock(WANE, rst.RESET9, mem.D3);
  bool SEDY_Q = ppu.SEDY.tock(WANE, rst.RESET9, mem.D2);
  bool VEVO_Q = ppu.VEVO.tock(WANE, rst.RESET9, mem.D6);
  bool VAFA_Q = ppu.VAFA.tock(WANE, rst.RESET9, mem.D5);
  bool VUCE_Q = ppu.VUCE.tock(WANE, rst.RESET9, mem.D1);
  bool RAHA_Q = ppu.RAHA.tock(WANE, rst.RESET9, mem.D7);

  ppu.FF45_D4 = SOTA_Q;
  ppu.FF45_D0 = SYRY_Q;
  ppu.FF45_D3 = SALO_Q;
  ppu.FF45_D2 = SEDY_Q;
  ppu.FF45_D6 = VEVO_Q;
  ppu.FF45_D5 = VAFA_Q;
  ppu.FF45_D1 = VUCE_Q;
  ppu.FF45_D7 = RAHA_Q;

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
  bool WAFU = and(cpu.CPU_RD2, dec.FF44);

  bool WATA = not(ppu.V4);
  bool XAGA = not(ppu.V5);
  bool XEPO = not(ppu.V1);
  bool XUHY = not(ppu.V3);
  bool XOWO = not(ppu.V7);
  bool XUCE = not(ppu.V6);
  bool WURY = not(ppu.V0);
  bool MYFA = not(ppu.V2);

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