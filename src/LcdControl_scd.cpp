// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CLK2;

extern bool NAPO_OUT;
extern bool RUTU_OUT;
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
extern bool XYDO;
extern bool NYPE;
extern bool LYFE;
extern bool FEPO;

//----------
// outputs

bool PIN_S;
bool PIN_ST;
bool PIN_CPL;
bool PIN_FR;
bool CLKPIPE;

//----------
// registers

static bool NYKA_Q, NYKA_CLK;
static bool PORY_Q, PORY_CLK;
static bool PYGO_Q, PYGO_CLK;
static bool PAHO_Q, PAHO_CLK;
static bool LUCA_Q, LUCA_CLK;
static bool LEBE_Q, LEBE_CLK;
static bool MEDA_Q, MEDA_CLK;

//-----------------------------------------------------------------------------

void tick_lcdcontrol() {
  bool LOBY = not(XYMU);
  bool NAFY = nor(MOSU, LOBY);

  bool POKY = unk2(PYGO_Q, LOBY);
  bool TOMU = not(SYLO);
  bool SOCY = not(TOMU);
  bool VYBO = nor(FEPO, WODU, MYVO);
  bool TYFA = and(SOCY, POKY, VYBO);
  bool SEGU = not(TYFA);
  bool ROXO = not(SEGU);

  bool LOFU = not(RUTU_OUT);
  
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
  bool TOFU = not(RESET_VIDEO);
  bool POME = nor(AVAP, /*POFY*/false);
  bool SACU = nor(SEGU, ROXY);
  bool RUJU = or(PAHO_Q, TOFU, POME);
  bool POFY = not(RUJU);
  bool RUZE = not(POFY);

  PIN_ST = RUZE;
  CLKPIPE = SACU;

  (void)LOBY;
  (void)NAFY;

  //----------
  // registers

  bool NYKA_Q_ = NYKA_Q;
  bool PORY_Q_ = PORY_Q;
  bool PYGO_Q_ = PYGO_Q;
  bool PAHO_Q_ = PAHO_Q;
  bool LUCA_Q_ = LUCA_Q;
  bool LEBE_Q_ = LEBE_Q;
  bool MEDA_Q_ = MEDA_Q;
  
  if (NYKA_CLK && !CLK2) NYKA_Q_ = LYRY;
  if (PORY_CLK && !MYVO) PORY_Q_ = NYKA_Q;
  if (PYGO_CLK && !CLK2) PYGO_Q_ = PORY_Q;
  if (PAHO_CLK && !ROXO) PAHO_Q_ = XYDO;
  if (LUCA_CLK && !LOFU) LUCA_Q_ = !LUCA_Q;
  if (LEBE_CLK && !!LUCA_Q) LEBE_Q_ = !LEBE_Q; // weird...
  if (MEDA_CLK && !NYPE) MEDA_Q_ = NERU;

  if (!NAFY) NYKA_Q_ = 0;
  if (!NAFY) PORY_Q_ = 0;
  if (!XYMU) PYGO_Q_ = 0;
  if (!XYMU) PAHO_Q_ = 0;
  if (!LYFE) LUCA_Q_ = 0;
  if (!LYFE) LEBE_Q_ = 0;
  if (!LYFE) MEDA_Q_ = 0;

  NYKA_Q = NYKA_Q_;
  PORY_Q = PORY_Q_;
  PYGO_Q = PYGO_Q_;
  PAHO_Q = PAHO_Q_;
  LUCA_Q = LUCA_Q_;
  LEBE_Q = LEBE_Q_;
  MEDA_Q = MEDA_Q_;

  NYKA_CLK = CLK2;
  PORY_CLK = MYVO;
  PYGO_CLK = CLK2;
  PAHO_CLK = ROXO;
  LUCA_CLK = LOFU;
  LEBE_CLK = !!LUCA_Q;
  MEDA_CLK = NYPE;
}