// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtBus.h"
#include "PPU.h"

//----------
// inputs

extern bool VAVA3;

//----------
// outputs

//----------
// registers

//-----------------------------------------------------------------------------

void tick_pixelmux() {
  bool WOXA = and(ppu.FF40_D1, ppu.SPR_PIX_A_7);
  bool XULA = and(ppu.FF40_D1, ppu.SPR_PIX_B_7);
  bool TADE = and(ppu.FF40_D0, ppu.BG_PIX_B_7);
  bool RAJY = and(ppu.FF40_D0, ppu.BG_PIX_A_7);

  bool WELE = not(XULA);
  bool VUMU = not(WOXA);
  bool NULY = nor(WOXA, XULA);
  bool RUTA = and(TADE, VAVA3);
  bool RYFU = and(RAJY, VAVA3);
  bool POKA = nor(NULY, RUTA, RYFU);
  ppu.BGPIXELn = POKA;

  bool LAVA = not(ppu.OBP0PIXELn);
  bool WOLO = not(WELE);
  bool WYRU = not(VUMU);

  bool VYRO = and(WYRU, WOLO, LAVA);
  bool VATA = and(WYRU, WELE, LAVA);
  bool VOLO = and(VUMU, WOLO, LAVA);
  bool VUGO = and(VUMU, WELE, LAVA);

  bool WUFU = amux4(ppu.OBP0_D7, VYRO, ppu.OBP0_D5, VATA, ppu.OBP0_D3, VOLO, ppu.OBP0_D1, VUGO);
  bool WALY = amux4(VYRO, ppu.OBP0_D6, VATA, ppu.OBP0_D4, VOLO, ppu.OBP0_D2, VUGO, ppu.OBP0_D0);

  bool MABY = not(XULA);
  bool LUKU = not(ppu.OBP1PIXELn);
  bool MEXA = not(WOXA);
  bool LOZO = not(MEXA);
  bool LYLE = not(MABY);
  bool LEDO = and(LOZO, LYLE, LUKU);
  bool LYKY = and(MEXA, LYLE, LUKU);
  bool LOPU = and(MEXA, MABY, LUKU);
  bool LARU = and(LOZO, MABY, LUKU);

  bool MOKA = amux4(ppu.OBP1_D7, LEDO, ppu.OBP1_D5, LYKY, ppu.OBP1_D3, LOPU, ppu.OBP1_D1, LARU);
  bool MUFA = amux4(LEDO, ppu.OBP1_D6, LYKY, ppu.OBP1_D4, LOPU, ppu.OBP1_D2, LARU, ppu.OBP1_D0);

  bool SOBA = not(RAJY);
  bool VYCO = not(TADE);

  bool MUVE = not(ppu.BGPIXELn);
  bool NUPO = not(SOBA);
  bool NALE = not(VYCO);
  bool NYPO = and(NALE, NUPO, MUVE);
  bool NUXO = and(VYCO, NUPO, MUVE);
  bool NUMA = and(NALE, SOBA, MUVE);
  bool POBU = and(VYCO, SOBA, MUVE);
  bool NURA = amux4(ppu.BGP_D7, NYPO, ppu.BGP_D5, NUMA, ppu.BGP_D3, NUXO, ppu.BGP_D1, POBU);
  bool NELO = amux4(NYPO, ppu.BGP_D6, NUMA, ppu.BGP_D4, NUXO, ppu.BGP_D2, POBU, ppu.BGP_D0);

  bool PATY = or(NURA, WUFU, MOKA);
  bool PERO = or(NELO, WALY, MUFA);

  bool RAVO = not(PATY);
  bool REMY = not(PERO);

  ext.LD1 = RAVO;
  ext.LD0 = REMY;
}