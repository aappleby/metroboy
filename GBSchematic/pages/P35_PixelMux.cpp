#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P35_PixelMux {
  struct Input {
    bool FF40_D0;
    bool FF40_D1;

    bool BG_PIX_A_7;
    bool BG_PIX_B_7;
    bool SPR_PIX_A_7;
    bool SPR_PIX_B_7;
    bool VAVA3;
    bool OBP0PIXELn;
    bool OBP1PIXELn;

    bool BGP_D0,BGP_D1,BGP_D2,BGP_D3,BGP_D4,BGP_D5,BGP_D6,BGP_D7;
    bool OBP0_D0,OBP0_D1,OBP0_D2,OBP0_D3,OBP0_D4,OBP0_D5,OBP0_D6,OBP0_D7;
    bool OBP1_D0,OBP1_D1,OBP1_D2,OBP1_D3,OBP1_D4,OBP1_D5,OBP1_D6,OBP1_D7;
  };

  struct Output {
    bool LD0,LD1; // physical lcd output pins
  };

  void tick_pixelmux(const Input& in, Output& out) {
    wire WOXA = and(in.FF40_D1, in.SPR_PIX_A_7);
    wire XULA = and(in.FF40_D1, in.SPR_PIX_B_7);
    wire TADE = and(in.FF40_D0, in.BG_PIX_B_7);
    wire RAJY = and(in.FF40_D0, in.BG_PIX_A_7);

    wire WELE = not(XULA);
    wire VUMU = not(WOXA);
    wire NULY = nor(WOXA, XULA);
    wire RUTA = and(TADE, in.VAVA3);
    wire RYFU = and(RAJY, in.VAVA3);
    wire POKA = nor(NULY, RUTA, RYFU);
    wire BGPIXELn = POKA;

    wire LAVA = not(in.OBP0PIXELn);
    wire WOLO = not(WELE);
    wire WYRU = not(VUMU);

    wire VYRO = and(WYRU, WOLO, LAVA);
    wire VATA = and(WYRU, WELE, LAVA);
    wire VOLO = and(VUMU, WOLO, LAVA);
    wire VUGO = and(VUMU, WELE, LAVA);

    wire WUFU = amux4(in.OBP0_D7, VYRO, in.OBP0_D5, VATA, in.OBP0_D3, VOLO, in.OBP0_D1, VUGO);
    wire WALY = amux4(VYRO, in.OBP0_D6, VATA, in.OBP0_D4, VOLO, in.OBP0_D2, VUGO, in.OBP0_D0);

    wire MABY = not(XULA);
    wire LUKU = not(in.OBP1PIXELn);
    wire MEXA = not(WOXA);
    wire LOZO = not(MEXA);
    wire LYLE = not(MABY);
    wire LEDO = and(LOZO, LYLE, LUKU);
    wire LYKY = and(MEXA, LYLE, LUKU);
    wire LOPU = and(MEXA, MABY, LUKU);
    wire LARU = and(LOZO, MABY, LUKU);

    wire MOKA = amux4(in.OBP1_D7, LEDO, in.OBP1_D5, LYKY, in.OBP1_D3, LOPU, in.OBP1_D1, LARU);
    wire MUFA = amux4(LEDO, in.OBP1_D6, LYKY, in.OBP1_D4, LOPU, in.OBP1_D2, LARU, in.OBP1_D0);

    wire SOBA = not(RAJY);
    wire VYCO = not(TADE);

    wire MUVE = not(BGPIXELn);
    wire NUPO = not(SOBA);
    wire NALE = not(VYCO);
    wire NYPO = and(NALE, NUPO, MUVE);
    wire NUXO = and(VYCO, NUPO, MUVE);
    wire NUMA = and(NALE, SOBA, MUVE);
    wire POBU = and(VYCO, SOBA, MUVE);
    wire NURA = amux4(in.BGP_D7, NYPO, in.BGP_D5, NUMA, in.BGP_D3, NUXO, in.BGP_D1, POBU);
    wire NELO = amux4(NYPO, in.BGP_D6, NUMA, in.BGP_D4, NUXO, in.BGP_D2, POBU, in.BGP_D0);

    wire PATY = or(NURA, WUFU, MOKA);
    wire PERO = or(NELO, WALY, MUFA);

    wire RAVO = not(PATY);
    wire REMY = not(PERO);

    out.LD1 = RAVO;
    out.LD0 = REMY;
  }
};