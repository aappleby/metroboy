// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtBus.h"
#include "PpuRegs.h"

//----------
// inputs

extern bool VAVA3;

//----------
// outputs

//----------
// registers

//-----------------------------------------------------------------------------

void tick_pixelmux() {
  bool WOXA = and(FF40_D1, SPR_PIX_A_7);
  bool XULA = and(FF40_D1, SPR_PIX_B_7);
  bool TADE = and(FF40_D0, BG_PIX_B_7);
  bool RAJY = and(FF40_D0, BG_PIX_A_7);

  bool WELE = not(XULA);
  bool VUMU = not(WOXA);
  bool NULY = nor(WOXA, XULA);
  bool RUTA = and(TADE, VAVA3);
  bool RYFU = and(RAJY, VAVA3);
  bool POKA = nor(NULY, RUTA, RYFU);
  BGPIXELn = POKA;

  bool LAVA = not(OBP0PIXELn);
  bool WOLO = not(WELE);
  bool WYRU = not(VUMU);

  bool VYRO = and(WYRU, WOLO, LAVA);
  bool VATA = and(WYRU, WELE, LAVA);
  bool VOLO = and(VUMU, WOLO, LAVA);
  bool VUGO = and(VUMU, WELE, LAVA);

  bool WUFU = amux4(OBP0_D7, VYRO, OBP0_D5, VATA, OBP0_D3, VOLO, OBP0_D1, VUGO);
  bool WALY = amux4(VYRO, OBP0_D6, VATA, OBP0_D4, VOLO, OBP0_D2, VUGO, OBP0_D0);

  bool MABY = not(XULA);
  bool LUKU = not(OBP1PIXELn);
  bool MEXA = not(WOXA);
  bool LOZO = not(MEXA);
  bool LYLE = not(MABY);
  bool LEDO = and(LOZO, LYLE, LUKU);
  bool LYKY = and(MEXA, LYLE, LUKU);
  bool LOPU = and(MEXA, MABY, LUKU);
  bool LARU = and(LOZO, MABY, LUKU);

  bool MOKA = amux4(OBP1_D7, LEDO, OBP1_D5, LYKY, OBP1_D3, LOPU, OBP1_D1, LARU);
  bool MUFA = amux4(LEDO, OBP1_D6, LYKY, OBP1_D4, LOPU, OBP1_D2, LARU, OBP1_D0);

  bool SOBA = not(RAJY);
  bool VYCO = not(TADE);

  bool MUVE = not(BGPIXELn);
  bool NUPO = not(SOBA);
  bool NALE = not(VYCO);
  bool NYPO = and(NALE, NUPO, MUVE);
  bool NUXO = and(VYCO, NUPO, MUVE);
  bool NUMA = and(NALE, SOBA, MUVE);
  bool POBU = and(VYCO, SOBA, MUVE);
  bool NURA = amux4(BGP_D7, NYPO, BGP_D5, NUMA, BGP_D3, NUXO, BGP_D1, POBU);
  bool NELO = amux4(NYPO, BGP_D6, NUMA, BGP_D4, NUXO, BGP_D2, POBU, BGP_D0);

  bool PATY = or(NURA, WUFU, MOKA);
  bool PERO = or(NELO, WALY, MUFA);

  bool RAVO = not(PATY);
  bool REMY = not(PERO);

  ext.LD1 = RAVO;
  ext.LD0 = REMY;
}