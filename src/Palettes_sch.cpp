// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CPU_RD2;
extern bool CPU_WR2;
extern bool FF47;
extern bool FF48;
extern bool FF49;

//----------
// outputs

bool BGP_D7;
bool BGP_D5;
bool BGP_D3;
bool BGP_D1;
bool BGP_D6;
bool BGP_D4;
bool BGP_D2;
bool BGP_D0;

bool OBP0_D7;
bool OBP0_D5;
bool OBP0_D3;
bool OBP0_D1;
bool OBP0_D6;
bool OBP0_D4;
bool OBP0_D2;
bool OBP0_D0;

bool OBP1_D7;
bool OBP1_D5;
bool OBP1_D3;
bool OBP1_D1;
bool OBP1_D6;
bool OBP1_D4;
bool OBP1_D2;
bool OBP1_D0;

//----------
// mystery signals

//----------
// registers

// BGP
bool MENA = 0;
bool MORU = 0;
bool MAXY = 0;
bool NUSY = 0;
bool MOGY = 0;
bool MUKE = 0;
bool PYLU = 0;
bool PAVO = 0;
bool BGP_CLK;

// OBP0
bool XANA = 0;
bool XYZE = 0;
bool XALO = 0;
bool XUKY = 0;
bool XUPO = 0;
bool XERU = 0;
bool XOVA = 0;
bool XUFU = 0;
bool OBP0_CLK = 0;

// OBP1
bool LUXO = 0;
bool LUGU = 0;
bool LOSE = 0;
bool LAWO = 0;
bool LEPU = 0;
bool LUNE = 0;
bool MOSA = 0;
bool MOXY = 0;
bool OBP1_CLK = 0;

//-----------------------------------------------------------------------------

void tick_palettes() {
  // BGP
  bool VUSO = and(CPU_RD2, FF47);
  bool VELY = and(CPU_WR2, FF47);
  bool TEPY = not(VUSO);
  bool TEPO = not(VELY);

  bool LARY = not(!MENA);
  bool LYKA = not(!MORU);
  bool LOBE = not(!MAXY);
  bool PABA = not(!NUSY);
  bool LODY = not(!MOGY);
  bool LACE = not(!MUKE);
  bool REDO = not(!PYLU);
  bool RARO = not(!PAVO);

  if (TEPY) {
    D7 = LARY;
    D5 = LYKA;
    D3 = LOBE;
    D1 = PABA;
    D6 = LODY;
    D4 = LACE;
    D2 = REDO;
    D0 = RARO;
  }

  BGP_D7 = MENA;
  BGP_D5 = MORU;
  BGP_D3 = MAXY;
  BGP_D1 = NUSY;
  BGP_D6 = MOGY;
  BGP_D4 = MUKE;
  BGP_D2 = PYLU;
  BGP_D0 = PAVO;

  // OBP0
  bool XUFY = and(CPU_RD2, FF48);
  bool XOMA = and(CPU_WR2, FF48);
  bool XOZY = not(XUFY);
  bool XELO = not(XOMA);

  bool XAWO = not(!XANA);
  bool XOBO = not(!XYZE);
  bool XUBY = not(!XALO);
  bool XOKE = not(!XUKY);
  bool XAXA = not(!XUPO);
  bool XAJU = not(!XERU);
  bool XUNO = not(!XOVA);
  bool XARY = not(!XUFU);

  if (XOZY) {
    D7 = XAWO;
    D5 = XOBO;
    D3 = XUBY;
    D1 = XOKE;
    D6 = XAXA;
    D4 = XAJU;
    D2 = XUNO;
    D0 = XARY;
  }

  OBP0_D7 = XAWO;
  OBP0_D5 = XOBO;
  OBP0_D3 = XUBY;
  OBP0_D1 = XOKE;
  OBP0_D6 = XAXA;
  OBP0_D4 = XAJU;
  OBP0_D2 = XUNO;
  OBP0_D0 = XARY;

  // OBP1
  bool MUMY = and(CPU_RD2, FF49);
  bool MYXE = and(CPU_WR2, FF49);
  bool LOTE = not(MUMY);
  bool LEHO = not(MYXE);

  bool LELU = not(!LUXO);
  bool LUGA = not(!LUGU);
  bool LYZA = not(!LOSE);
  bool LEPA = not(!LAWO);
  bool LEBA = not(!LEPU);
  bool LUKY = not(!LUNE);
  bool LODE = not(!MOSA);
  bool LAJU = not(!MOXY);

  if (LOTE) {
    D7 = LELU;
    D5 = LUGA;
    D3 = LYZA;
    D1 = LEPA;
    D6 = LEBA;
    D4 = LUKY;
    D2 = LODE;
    D0 = LAJU;
  }

  OBP1_D7 = LELU;
  OBP1_D5 = LUGA;
  OBP1_D3 = LYZA;
  OBP1_D1 = LEPA;
  OBP1_D6 = LEBA;
  OBP1_D4 = LUKY;
  OBP1_D2 = LODE;
  OBP1_D0 = LAJU;

  // clocks

  bool BGP_CLK_ = TEPO;
  if (BGP_CLK && !BGP_CLK_) {
    MENA = D7;
    MORU = D5;
    MAXY = D3;
    NUSY = D1;
    MOGY = D6;
    MUKE = D4;
    PYLU = D2;
    PAVO = D0;
  }
  BGP_CLK_ = BGP_CLK;

  bool OBP0_CLK_ = XELO;
  if (OBP0_CLK && !OBP0_CLK_) {
    XANA = D7;
    XYZE = D5;
    XALO = D3;
    XUKY = D1;
    XUPO = D6;
    XERU = D4;
    XOVA = D2;
    XUFU = D0;
  }
  OBP0_CLK = OBP0_CLK_;

  bool OBP1_CLK_ = LEHO;
  if (OBP1_CLK && !OBP1_CLK_) {
    LUXO = D7;
    LUGU = D5;
    LOSE = D3;
    LAWO = D1;
    LEPU = D6;
    LUNE = D4;
    MOSA = D2;
    MOXY = D0;
  }

}