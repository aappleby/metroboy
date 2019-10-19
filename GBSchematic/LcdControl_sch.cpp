// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtBus.h"
#include "PPU.h"
#include "Clocks.h"

//----------
// inputs

extern bool NAPO_OUT;
extern bool FF04_D0n;
extern bool FF04_D1n;

extern bool AVAP;
extern bool ROXY;
extern bool PURE;
extern bool XYMU;
extern bool MOSU;
extern bool LYRY;
extern bool MYVO;
extern bool SYLO;
extern bool WODU;
extern bool LYFE;
extern bool FEPO;

extern reg RUTU;

extern reg NYPE;

//----------
// outputs

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
  bool NERU = !(ppu.V7 || ppu.V6 || ppu.V5 || ppu.V4 || ppu.V3 || ppu.V2 || ppu.V1); // biiig nor
  bool MURE = not(MEDA_Q);

  ext.PIN_S = MURE;

  bool MAGU = xor(NAPO_OUT, LUCA_Q);
  bool MECO = not(MAGU);
  bool KEBO = not(MECO);
  bool KASA = not(PURE);
  bool UMOB = not(FF04_D0n);
  bool USEC = not(FF04_D1n);
  bool KEDY = not(ppu.FF40_D7);
  bool KAHE = amux2(ppu.FF40_D7, KASA, KEDY, UMOB);
  bool KUPA = amux2(ppu.FF40_D7, KEBO, KEDY, USEC);

  bool KYMO = not(KAHE);
  bool KOFO = not(KUPA);

  ext.PIN_CPL = KYMO;
  ext.PIN_FR = KOFO;

  // FIXME another logic loop...
  TOFU = not(rst.RESET_VIDEO);
  bool POME = nor(AVAP, /*POFY*/false);
  bool SACU = nor(SEGU, ROXY);
  bool RUJU = or(PAHO_Q, TOFU, POME);
  bool POFY = not(RUJU);
  bool RUZE = not(POFY);

  ext.PIN_ST = RUZE;
  clk.CLKPIPE = SACU;

  NYKA.tock(clk.CLK2, NAFY, LYRY);
  PORY.tock(MYVO, NAFY, NYKA_Q);
  PYGO.tock(clk.CLK2, XYMU, PORY_Q);

  bool XYDO_Q = ppu.XYDO.q();
  PAHO.tock(ROXO, XYMU, XYDO_Q);

  LUCA.tock(LOFU,    LYFE, !LUCA_Q);
  LEBE.tock(!LUCA_Q, LYFE, !LEBE_Q);

  bool NYPE_Q = NYPE.q();
  MEDA.tock(NYPE_Q,    LYFE, NERU);
}