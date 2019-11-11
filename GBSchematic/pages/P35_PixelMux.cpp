#include "P35_PixelMux.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P35_PixelMux::tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  c.p35.WOXA = and(b.p23.FF40_D1, b.p33.SPR_PIX_A_7);
  c.p35.XULA = and(b.p23.FF40_D1, b.p33.SPR_PIX_B_7);
  c.p35.TADE = and(b.p23.FF40_D0, b.p32.BG_PIX_B_7);
  c.p35.RAJY = and(b.p23.FF40_D0, b.p32.BG_PIX_A_7);

  c.p35.WELE = not(b.p35.XULA);
  c.p35.VUMU = not(b.p35.WOXA);
  c.p35.NULY = nor(b.p35.WOXA, b.p35.XULA);
  c.p35.RUTA = and(b.p35.TADE, b.p26.VAVA);
  c.p35.RYFU = and(b.p35.RAJY, b.p26.VAVA);
  c.p35.POKA = nor(b.p35.NULY, b.p35.RUTA, b.p35.RYFU);

  c.p35.LAVA = not(b.p34.OBP0PIXELn);
  c.p35.WOLO = not(b.p35.WELE);
  c.p35.WYRU = not(b.p35.VUMU);

  c.p35.VYRO = and(b.p35.WYRU, b.p35.WOLO, b.p35.LAVA);
  c.p35.VATA = and(b.p35.WYRU, b.p35.WELE, b.p35.LAVA);
  c.p35.VOLO = and(b.p35.VUMU, b.p35.WOLO, b.p35.LAVA);
  c.p35.VUGO = and(b.p35.VUMU, b.p35.WELE, b.p35.LAVA);

  c.p35.WUFU = amux4(b.p36.OBP0_D7, b.p35.VYRO, b.p36.OBP0_D5, b.p35.VATA, b.p36.OBP0_D3, b.p35.VOLO, b.p36.OBP0_D1, b.p35.VUGO);
  c.p35.WALY = amux4(b.p35.VYRO, b.p36.OBP0_D6, b.p35.VATA, b.p36.OBP0_D4, b.p35.VOLO, b.p36.OBP0_D2, b.p35.VUGO, b.p36.OBP0_D0);

  c.p35.MABY = not(b.p35.XULA);
  c.p35.LUKU = not(b.p34.OBP1PIXELn);
  c.p35.MEXA = not(b.p35.WOXA);
  c.p35.LOZO = not(b.p35.MEXA);
  c.p35.LYLE = not(b.p35.MABY);
  c.p35.LEDO = and(b.p35.LOZO, b.p35.LYLE, b.p35.LUKU);
  c.p35.LYKY = and(b.p35.MEXA, b.p35.LYLE, b.p35.LUKU);
  c.p35.LOPU = and(b.p35.MEXA, b.p35.MABY, b.p35.LUKU);
  c.p35.LARU = and(b.p35.LOZO, b.p35.MABY, b.p35.LUKU);

  c.p35.MOKA = amux4(b.p36.OBP1_D7, b.p35.LEDO, b.p36.OBP1_D5, b.p35.LYKY, b.p36.OBP1_D3, b.p35.LOPU, b.p36.OBP1_D1, b.p35.LARU);
  c.p35.MUFA = amux4(b.p35.LEDO, b.p36.OBP1_D6, b.p35.LYKY, b.p36.OBP1_D4, b.p35.LOPU, b.p36.OBP1_D2, b.p35.LARU, b.p36.OBP1_D0);

  c.p35.SOBA = not(b.p35.RAJY);
  c.p35.VYCO = not(b.p35.TADE);

  c.p35.MUVE = not(b.p35.BGPIXELn);
  c.p35.NUPO = not(b.p35.SOBA);
  c.p35.NALE = not(b.p35.VYCO);
  c.p35.NYPO = and(b.p35.NALE, b.p35.NUPO, b.p35.MUVE);
  c.p35.NUXO = and(b.p35.VYCO, b.p35.NUPO, b.p35.MUVE);
  c.p35.NUMA = and(b.p35.NALE, b.p35.SOBA, b.p35.MUVE);
  c.p35.POBU = and(b.p35.VYCO, b.p35.SOBA, b.p35.MUVE);
  c.p35.NURA = amux4(b.p36.BGP_D7, b.p35.NYPO, b.p36.BGP_D5, b.p35.NUMA, b.p36.BGP_D3, b.p35.NUXO, b.p36.BGP_D1, b.p35.POBU);
  c.p35.NELO = amux4(b.p35.NYPO, b.p36.BGP_D6, b.p35.NUMA, b.p36.BGP_D4, b.p35.NUXO, b.p36.BGP_D2, b.p35.POBU, b.p36.BGP_D0);

  c.p35.PATY = or(b.p35.NURA, b.p35.WUFU, b.p35.MOKA);
  c.p35.PERO = or(b.p35.NELO, b.p35.WALY, b.p35.MUFA);

  c.p35.RAVO = not(b.p35.PATY);
  c.p35.REMY = not(b.p35.PERO);

  c.chip.LD1 = b.p35.RAVO;
  c.chip.LD0 = b.p35.REMY;
}