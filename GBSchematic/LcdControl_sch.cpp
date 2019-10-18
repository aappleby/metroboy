// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CLK2;

extern bool NAPO_OUT;
extern reg RUTU;
extern bool FF04_D0n;
extern bool FF04_D1n;
extern bool FF40_D7;
extern bool RESET_VIDEO;

extern bool AVAP;
extern bool ROXY;
extern bool PURE;
extern bool XYMU;
extern bool MOSU;
extern bool CLK2;
extern bool LYRY;
extern bool MYVO;
extern bool SYLO;
extern bool WODU;
extern reg XYDO;
extern reg NYPE;
extern bool LYFE;
extern bool FEPO;

extern bool V0, V1, V2, V3, V4, V5, V6, V7;

//----------
// outputs

bool PIN_S;
bool PIN_ST;
bool PIN_CPL;
bool PIN_FR;
bool CLKPIPE;

bool LOBY;
bool POKY;
bool ROXO;
bool TOMU;
bool TOFU;
bool SEGU;

//----------
// registers

reg NYKA;
reg PORY;
reg PYGO;
reg PAHO;
reg LUCA;
reg LEBE;
reg MEDA;

//-----------------------------------------------------------------------------

void tick_lcdcontrol() {
  wire NYKA_Q = NYKA.q();
  wire PORY_Q = PORY.q();
  wire PYGO_Q = PYGO.q();
  wire PAHO_Q = PAHO.q();
  wire LUCA_Q = LUCA.q();
  wire LEBE_Q = LEBE.q();
  wire MEDA_Q = MEDA.q();

  LOBY = not(XYMU);
  bool NAFY = nor(MOSU, LOBY);

  POKY = unk2(PYGO_Q, LOBY);
  TOMU = not(SYLO);
  bool SOCY = not(TOMU);
  bool VYBO = nor(FEPO, WODU, MYVO);
  bool TYFA = and(SOCY, POKY, VYBO);
  SEGU = not(TYFA);
  ROXO = not(SEGU);

  bool RUTU_Q = RUTU.q();
  bool LOFU = not(RUTU_Q);
  
  // FIXME why 7
  bool NERU = !(V7 || V6 || V5 || V4 || V3 || V2 || V1); // biiig nor
  bool MURE = not(MEDA_Q);

  PIN_S = MURE;

  bool MAGU = xor(NAPO_OUT, LUCA_Q);
  bool MECO = not(MAGU);
  bool KEBO = not(MECO);
  bool KASA = not(PURE);
  bool UMOB = not(FF04_D0n);
  bool USEC = not(FF04_D1n);
  bool KEDY = not(FF40_D7);
  bool KAHE = amux2(FF40_D7, KASA, KEDY, UMOB);
  bool KUPA = amux2(FF40_D7, KEBO, KEDY, USEC);

  bool KYMO = not(KAHE);
  bool KOFO = not(KUPA);

  PIN_CPL = KYMO;
  PIN_FR = KOFO;

  // FIXME another logic loop...
  TOFU = not(RESET_VIDEO);
  bool POME = nor(AVAP, /*POFY*/false);
  bool SACU = nor(SEGU, ROXY);
  bool RUJU = or(PAHO_Q, TOFU, POME);
  bool POFY = not(RUJU);
  bool RUZE = not(POFY);

  PIN_ST = RUZE;
  CLKPIPE = SACU;

  NYKA.tock(CLK2, NAFY, LYRY);
  PORY.tock(MYVO, NAFY, NYKA_Q);
  PYGO.tock(CLK2, XYMU, PORY_Q);

  bool XYDO_Q = XYDO.q();
  PAHO.tock(ROXO, XYMU, XYDO_Q);

  LUCA.tock(LOFU,    LYFE, !LUCA_Q);
  LEBE.tock(!LUCA_Q, LYFE, !LEBE_Q);

  bool NYPE_Q = NYPE.q();
  MEDA.tock(NYPE_Q,    LYFE, NERU);
}