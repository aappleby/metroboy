// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "MemBus.h"
#include "PPU.h"
#include "CpuBus.h"
#include "Clocks.h"
#include "AddressDecoder.h"

//-----------------------------------------------------------------------------

void tick_videoregs(const Resets& rst, const AddressDecoder& dec) {
  // FF40 LCDC
  bool WARU = and(dec.FF40, cpu.CPU_WR2);
  bool XUBO = not(WARU);
  ppu.FF40_D0 = ppu.VYXE.tock(XUBO, rst.XARE, mem.D0);
  ppu.FF40_D1 = ppu.XYLO.tock(XUBO, rst.XARE, mem.D1);
  ppu.FF40_D2 = ppu.XYMO.tock(XUBO, rst.XARE, mem.D2);
  ppu.FF40_D3 = ppu.XAFO.tock(XUBO, rst.XARE, mem.D3);
  ppu.FF40_D4 = ppu.WEXU.tock(XUBO, rst.XARE, mem.D4);
  ppu.FF40_D5 = ppu.WYMO.tock(XUBO, rst.XARE, mem.D5);
  ppu.FF40_D6 = ppu.WOKY.tock(XUBO, rst.XARE, mem.D6);
  ppu.LCDC_EN = ppu.XONA.tock(XUBO, rst.XARE, mem.D7);
 
  if (and(dec.FF40, cpu.CPU_RD2)) {
    mem.D0 = ppu.FF40_D0;
    mem.D1 = ppu.FF40_D1;
    mem.D2 = ppu.FF40_D2;
    mem.D3 = ppu.FF40_D3;
    mem.D4 = ppu.FF40_D4;
    mem.D5 = ppu.FF40_D5;
    mem.D6 = ppu.FF40_D6;
    mem.D7 = ppu.LCDC_EN;
  }

  // FF4B WX
  bool WUZA = and(cpu.CPU_WR2, dec.FF4B);
  bool VOXU = not(WUZA);
  ppu.FF4B_D0 = ppu.WX_R0.tock(VOXU, rst.RESET8, mem.D0);
  ppu.FF4B_D1 = ppu.WX_R1.tock(VOXU, rst.RESET8, mem.D1);
  ppu.FF4B_D2 = ppu.WX_R2.tock(VOXU, rst.RESET8, mem.D2);
  ppu.FF4B_D3 = ppu.WX_R3.tock(VOXU, rst.RESET8, mem.D3);
  ppu.FF4B_D4 = ppu.WX_R4.tock(VOXU, rst.RESET8, mem.D4);
  ppu.FF4B_D5 = ppu.WX_R5.tock(VOXU, rst.RESET8, mem.D5);
  ppu.FF4B_D6 = ppu.WX_R6.tock(VOXU, rst.RESET8, mem.D6);
  ppu.FF4B_D7 = ppu.WX_R7.tock(VOXU, rst.RESET8, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF4B)) {
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
  bool WEKO = and(cpu.CPU_WR2, dec.FF4A);
  bool VEFU = not(WEKO);
  ppu.FF4B_D0 = ppu.NESO.tock(VEFU, rst.RESET8, mem.D0);
  ppu.FF4B_D1 = ppu.NYRO.tock(VEFU, rst.RESET8, mem.D1);
  ppu.FF4B_D2 = ppu.NAGA.tock(VEFU, rst.RESET8, mem.D2);
  ppu.FF4B_D3 = ppu.MELA.tock(VEFU, rst.RESET8, mem.D3);
  ppu.FF4B_D4 = ppu.NULO.tock(VEFU, rst.RESET8, mem.D4);
  ppu.FF4B_D5 = ppu.NENE.tock(VEFU, rst.RESET8, mem.D5);
  ppu.FF4B_D6 = ppu.NUKA.tock(VEFU, rst.RESET8, mem.D6);
  ppu.FF4B_D7 = ppu.NAFU.tock(VEFU, rst.RESET8, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF4A)) {
    mem.D0 = ppu.FF4B_D0;
    mem.D1 = ppu.FF4B_D1;
    mem.D2 = ppu.FF4B_D2;
    mem.D3 = ppu.FF4B_D3;
    mem.D4 = ppu.FF4B_D4;
    mem.D5 = ppu.FF4B_D5;
    mem.D6 = ppu.FF4B_D6;
    mem.D7 = ppu.FF4B_D7;
  }

  // FF43 SCX
  bool ARUR = and(dec.FF43, cpu.CPU_WR2);
  bool AMUN = not(ARUR);

  ppu.FF43_D0 = ppu.DATY.tock(AMUN, rst.RESET6, mem.D0);
  ppu.FF43_D1 = ppu.DUZU.tock(AMUN, rst.RESET6, mem.D1);
  ppu.FF43_D2 = ppu.CYXU.tock(AMUN, rst.RESET6, mem.D2);
  ppu.FF43_D3 = ppu.GUBO.tock(AMUN, rst.RESET6, mem.D3);
  ppu.FF43_D4 = ppu.BEMY.tock(AMUN, rst.RESET6, mem.D4);
  ppu.FF43_D5 = ppu.CUZY.tock(AMUN, rst.RESET6, mem.D5);
  ppu.FF43_D6 = ppu.CABU.tock(AMUN, rst.RESET6, mem.D6);
  ppu.FF43_D7 = ppu.BAKE.tock(AMUN, rst.RESET6, mem.D7);

  if (and(dec.FF43, cpu.CPU_RD2)) {
    mem.D0 = ppu.FF43_D0;
    mem.D1 = ppu.FF43_D1;
    mem.D2 = ppu.FF43_D2;
    mem.D3 = ppu.FF43_D3;
    mem.D4 = ppu.FF43_D4;
    mem.D5 = ppu.FF43_D5;
    mem.D6 = ppu.FF43_D6;
    mem.D7 = ppu.FF43_D7;
  }

  // FF42 SCY
  bool BEDY = and(cpu.CPU_WR2, dec.FF42);
  bool CAVO = not(BEDY);

  ppu.FF42_D0 = ppu.GAVE.tock(CAVO, rst.RESET6, mem.D0);
  ppu.FF42_D1 = ppu.FYMO.tock(CAVO, rst.RESET6, mem.D1);
  ppu.FF42_D2 = ppu.FEZU.tock(CAVO, rst.RESET6, mem.D2);
  ppu.FF42_D3 = ppu.FUJO.tock(CAVO, rst.RESET6, mem.D3);
  ppu.FF42_D4 = ppu.DEDE.tock(CAVO, rst.RESET6, mem.D4);
  ppu.FF42_D5 = ppu.FOTY.tock(CAVO, rst.RESET6, mem.D5);
  ppu.FF42_D6 = ppu.FOHA.tock(CAVO, rst.RESET6, mem.D6);
  ppu.FF42_D7 = ppu.FUNY.tock(CAVO, rst.RESET6, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF42)) {
    mem.D0 = ppu.FF42_D0;
    mem.D1 = ppu.FF42_D1;
    mem.D2 = ppu.FF42_D2;
    mem.D3 = ppu.FF42_D3;
    mem.D4 = ppu.FF42_D4;
    mem.D5 = ppu.FF42_D5;
    mem.D6 = ppu.FF42_D6;
    mem.D7 = ppu.FF42_D7;
  }

  // FF45 LYC
  bool XUFA = and(cpu.CPU_WR2, dec.FF45);
  bool WANE = not(XUFA);

  ppu.FF45_D0 = ppu.SYRY.tock(WANE, rst.RESET9, mem.D0);
  ppu.FF45_D1 = ppu.VUCE.tock(WANE, rst.RESET9, mem.D1);
  ppu.FF45_D2 = ppu.SEDY.tock(WANE, rst.RESET9, mem.D2);
  ppu.FF45_D3 = ppu.SALO.tock(WANE, rst.RESET9, mem.D3);
  ppu.FF45_D4 = ppu.SOTA.tock(WANE, rst.RESET9, mem.D4);
  ppu.FF45_D5 = ppu.VAFA.tock(WANE, rst.RESET9, mem.D5);
  ppu.FF45_D6 = ppu.VEVO.tock(WANE, rst.RESET9, mem.D6);
  ppu.FF45_D7 = ppu.RAHA.tock(WANE, rst.RESET9, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF45)) {
    mem.D0 = ppu.FF45_D0;
    mem.D1 = ppu.FF45_D1;
    mem.D2 = ppu.FF45_D2;
    mem.D3 = ppu.FF45_D3;
    mem.D4 = ppu.FF45_D4;
    mem.D5 = ppu.FF45_D5;
    mem.D6 = ppu.FF45_D6;
    mem.D7 = ppu.FF45_D7;
  }

  // FF44 LY
  if (and(cpu.CPU_RD2, dec.FF44)) {
    mem.D0 = ppu.V0;
    mem.D1 = ppu.V1;
    mem.D2 = ppu.V2;
    mem.D3 = ppu.V3;
    mem.D4 = ppu.V4;
    mem.D5 = ppu.V5;
    mem.D6 = ppu.V6;
    mem.D7 = ppu.V7;
  }
}