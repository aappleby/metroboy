// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "PPU.h"
#include "CpuBus.h"

//-----------------------------------------------------------------------------

void tick_palettes() {
  // BGP
  bool VUSO = and(cpu.CPU_RD2, dec.FF47);
  bool VELY = and(cpu.CPU_WR2, dec.FF47);
  bool TEPY = not(VUSO);
  bool TEPO = not(VELY);

  bool MENA_Q = ppu.MENA.tock(TEPO, 0, mem.D7);
  bool MORU_Q = ppu.MORU.tock(TEPO, 0, mem.D5);
  bool MAXY_Q = ppu.MAXY.tock(TEPO, 0, mem.D3);
  bool NUSY_Q = ppu.NUSY.tock(TEPO, 0, mem.D1);
  bool MOGY_Q = ppu.MOGY.tock(TEPO, 0, mem.D6);
  bool MUKE_Q = ppu.MUKE.tock(TEPO, 0, mem.D4);
  bool PYLU_Q = ppu.PYLU.tock(TEPO, 0, mem.D2);
  bool PAVO_Q = ppu.PAVO.tock(TEPO, 0, mem.D0);

  ppu.BGP_D7 = MENA_Q;
  ppu.BGP_D5 = MORU_Q;
  ppu.BGP_D3 = MAXY_Q;
  ppu.BGP_D1 = NUSY_Q;
  ppu.BGP_D6 = MOGY_Q;
  ppu.BGP_D4 = MUKE_Q;
  ppu.BGP_D2 = PYLU_Q;
  ppu.BGP_D0 = PAVO_Q;

  bool LARY = not(!MENA_Q);
  bool LYKA = not(!MORU_Q);
  bool LOBE = not(!MAXY_Q);
  bool PABA = not(!NUSY_Q);
  bool LODY = not(!MOGY_Q);
  bool LACE = not(!MUKE_Q);
  bool REDO = not(!PYLU_Q);
  bool RARO = not(!PAVO_Q);

  if (TEPY) {
    mem.D7 = LARY;
    mem.D5 = LYKA;
    mem.D3 = LOBE;
    mem.D1 = PABA;
    mem.D6 = LODY;
    mem.D4 = LACE;
    mem.D2 = REDO;
    mem.D0 = RARO;
  }

  // OBP0
  bool XUFY = and(cpu.CPU_RD2, dec.FF48);
  bool XOMA = and(cpu.CPU_WR2, dec.FF48);
  bool XOZY = not(XUFY);
  bool XELO = not(XOMA);

  bool XANA_Q = ppu.XANA.tock(XELO, 0, mem.D7);
  bool XYZE_Q = ppu.XYZE.tock(XELO, 0, mem.D5);
  bool XALO_Q = ppu.XALO.tock(XELO, 0, mem.D3);
  bool XUKY_Q = ppu.XUKY.tock(XELO, 0, mem.D1);
  bool XUPO_Q = ppu.XUPO.tock(XELO, 0, mem.D6);
  bool XERU_Q = ppu.XERU.tock(XELO, 0, mem.D4);
  bool XOVA_Q = ppu.XOVA.tock(XELO, 0, mem.D2);
  bool XUFU_Q = ppu.XUFU.tock(XELO, 0, mem.D0);

  ppu.OBP0_D7 = XANA_Q;
  ppu.OBP0_D5 = XYZE_Q;
  ppu.OBP0_D3 = XALO_Q;
  ppu.OBP0_D1 = XUKY_Q;
  ppu.OBP0_D6 = XUPO_Q;
  ppu.OBP0_D4 = XERU_Q;
  ppu.OBP0_D2 = XOVA_Q;
  ppu.OBP0_D0 = XUFU_Q;

  bool XAWO = not(!XANA_Q);
  bool XOBO = not(!XYZE_Q);
  bool XUBY = not(!XALO_Q);
  bool XOKE = not(!XUKY_Q);
  bool XAXA = not(!XUPO_Q);
  bool XAJU = not(!XERU_Q);
  bool XUNO = not(!XOVA_Q);
  bool XARY = not(!XUFU_Q);

  if (XOZY) {
    mem.D7 = XAWO;
    mem.D5 = XOBO;
    mem.D3 = XUBY;
    mem.D1 = XOKE;
    mem.D6 = XAXA;
    mem.D4 = XAJU;
    mem.D2 = XUNO;
    mem.D0 = XARY;
  }

  // OBP1
  bool MUMY = and(cpu.CPU_RD2, dec.FF49);
  bool MYXE = and(cpu.CPU_WR2, dec.FF49);
  bool LOTE = not(MUMY);
  bool LEHO = not(MYXE);

  bool LUXO_Q = ppu.LUXO.tock(LEHO, 0, mem.D7);
  bool LUGU_Q = ppu.LUGU.tock(LEHO, 0, mem.D5);
  bool LOSE_Q = ppu.LOSE.tock(LEHO, 0, mem.D3);
  bool LAWO_Q = ppu.LAWO.tock(LEHO, 0, mem.D1);
  bool LEPU_Q = ppu.LEPU.tock(LEHO, 0, mem.D6);
  bool LUNE_Q = ppu.LUNE.tock(LEHO, 0, mem.D4);
  bool MOSA_Q = ppu.MOSA.tock(LEHO, 0, mem.D2);
  bool MOXY_Q = ppu.MOXY.tock(LEHO, 0, mem.D0);

  ppu.OBP1_D7 = LUXO_Q;
  ppu.OBP1_D5 = LUGU_Q;
  ppu.OBP1_D3 = LOSE_Q;
  ppu.OBP1_D1 = LAWO_Q;
  ppu.OBP1_D6 = LEPU_Q;
  ppu.OBP1_D4 = LUNE_Q;
  ppu.OBP1_D2 = MOSA_Q;
  ppu.OBP1_D0 = MOXY_Q;

  bool LELU = not(!LUXO_Q);
  bool LUGA = not(!LUGU_Q);
  bool LYZA = not(!LOSE_Q);
  bool LEPA = not(!LAWO_Q);
  bool LEBA = not(!LEPU_Q);
  bool LUKY = not(!LUNE_Q);
  bool LODE = not(!MOSA_Q);
  bool LAJU = not(!MOXY_Q);

  if (LOTE) {
    mem.D7 = LELU;
    mem.D5 = LUGA;
    mem.D3 = LYZA;
    mem.D1 = LEPA;
    mem.D6 = LEBA;
    mem.D4 = LUKY;
    mem.D2 = LODE;
    mem.D0 = LAJU;
  }
}