// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool FF45_D7;
extern bool FF45_D6;
extern bool FF45_D5;
extern bool FF45_D4;
extern bool FF45_D3;
extern bool FF45_D2;
extern bool FF45_D1;
extern bool FF45_D0;

extern bool XEHO_Q;
extern bool SAVY;
extern bool XODU;
extern bool TUKY;
extern bool SYBE;
extern bool FEPO;
extern bool RUTU;

//----------
// outputs

bool PALY; // ly compare match
bool XYVO; // line >= 144;

bool INT_OAM;
bool INT_HBL;
bool INT_VBL;
bool INT_VBL_BUF;

//----------
// registers

bool NYPE_Q, NYPE_CLK;
bool POPU_Q, POPU_CLK;
bool NAPO_Q, NAPO_CLK;

//-----------------------------------------------------------------------------

void tick_videocontrol() {
  bool SYFU = xor(V7, FF45_D7);
  bool TERY = xor(V6, FF45_D6);
  bool TUCY = xor(V5, FF45_D5);
  bool TYKU = xor(V4, FF45_D4);
  bool RASY = xor(V3, FF45_D3);
  bool REDA = xor(V2, FF45_D2);
  bool TYDE = xor(V1, FF45_D1);
  bool RYME = xor(V0, FF45_D0);

  bool SOVU = nor(SYFU, TERY, TUCY, TYKU);
  bool SUBO = nor(RASY, REDA, TYDE, RYME);
  bool RAPE = nand(SOVU, SUBO);
  PALY = not(RAPE);

  //----------

  bool XUGU = nand(XEHO_Q, SAVY, XODU, TUKY, SYBE);
  bool XENA = not(FEPO);
  bool XANO = not(XUGU);
  bool WODU = and(XENA, XANO);

  XYVO = and(V4, V7);
  bool PARU = not(!POPU_Q);
  bool PURE = not(RUTU);
  bool SELA = not(PURE);
  bool TOLU = not(PARU);
  bool TAPA = and(SELA, TOLU);
  bool TARU = and(TOLU, WODU);
  bool VYPU = not(TOLU);

  INT_VBL = PARU;
  INT_OAM = TAPA;
  INT_HBL = TARU;
  INT_VBL_BUF = VYPU;
}