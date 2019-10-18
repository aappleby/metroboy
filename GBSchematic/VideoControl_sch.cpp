// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "PpuRegs.h"

//----------
// inputs

extern bool FF41;
extern bool CLK2;

extern bool ATEJ;
extern bool FEPO;
extern bool LYFE;
extern bool LYHA;
extern bool TOFU;
extern bool ACYL;
extern bool WESY;
extern bool PALY;
extern bool AVAP;
extern bool POVA;
extern reg WUVU;

//----------
// outputs

bool PALY; // ly compare match
bool XYVO; // line >= 144;

bool LYHA; // !RESET_VIDEO
bool LYFE; // !!RESET_VIDEO

bool PURE;
bool TALU;

bool INT_OAM;
bool INT_HBL;
bool INT_VBL;

bool NAPO_OUT;

bool WODU;
bool SELA;
bool XYMU;

//-----------------------------------------------------------------------------
// registers

//----------

reg VENA;

reg SAXO, TYPO, VYZO, TELU, SUDE, TAHA, TYRY;

reg RUTU;
reg SYGU;

//----------
// FF41 STAT

reg NYPE;
reg POPU;
reg NAPO;

reg VOGA;

reg RUGU;
reg REFE;
reg ROPO;
reg RUFO;
reg ROXE;

reg MYTA;

//-----------------------------------------------------------------------------

void tick_videocontrol() {

  //----------
  // LY compare

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
  // top center

  bool WUVU_Q = WUVU.q();
  bool VENA_Q = VENA.flip(!WUVU_Q, RESET_VIDEO);
  bool RUTU_Q = RUTU.q();
  bool SYGU_Q = SYGU.q();
  
  bool MUDE = nor(RUTU_Q, LYHA); // schematic says RUTU_OUT, but I think this is just RUTU_Q?
  TALU = not(!VENA_Q);

  bool SAXO_Q = SAXO.flip(TALU,    MUDE);
  bool TYPO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool VYZO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TELU_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool SUDE_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TAHA_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TYRY_Q = TYPO.flip(!SAXO_Q, MUDE);


  bool TOCU = not(SAXO_Q);
  bool VEPE = not(TYPO_Q);
  bool VUTY = not(VYZO_Q);
  bool TUJU = not(TELU_Q);
  bool TAFY = not(SUDE_Q);
  bool TUDA = not(TAHA_Q);
  bool VATE = not(TYRY_Q);

  bool VOKU = nand(TUJU,   TAFY,   TUDA,   VATE,   VUTY,   VEPE,   TOCU);
  bool TOZU = nand(TUJU,   TAFY,   TUDA,   VATE,   VYZO_Q, TYPO_Q, SAXO_Q);
  bool TECE = nand(TUJU,   TAHA_Q, TUDA,   TELU_Q, VYZO_Q, VEPE,   SAXO_Q);
  bool TEBO = nand(TYRY_Q, TAFY,   SUDE_Q, VATE,   VUTY,   TYPO_Q, SAXO_Q);

  bool TEGY = nand(VOKU, TOZU, TECE, TEBO);
  bool SANU = nand(TYRY_Q, TAHA_Q, SUDE_Q, SAXO_Q);

  bool SONO = not(TALU);

  bool RYNO = or(SYGU_Q, RUTU_Q);
  bool POGU = not(RYNO);
  PIN_CPG = POGU;

  RUTU.tock(SONO, LYFE, SANU);
  SYGU.tock(SONO, LYFE, TEGY);

  //----------
  // x counter

  bool TADY = nor(ATEJ, TOFU);

  bool XEHO_Q = XEHO.q();
  bool SAVY_Q = SAVY.q();
  bool XODU_Q = XODU.q();
  bool XYDO_Q = XYDO.q();

  bool XUKE = and(XEHO_Q, SAVY_Q);
  bool XYLE = and(XODU_Q, XUKE);
  bool RYBO = xor(SAVY_Q, XEHO_Q);
  bool XEGY = xor(XODU_Q, XUKE);
  bool XORA = xor(XYDO_Q, XYLE);

  XEHO.tock(CLKPIPE, TADY, !XEHO_Q);
  SAVY.tock(CLKPIPE, TADY, RYBO);
  XODU.tock(CLKPIPE, TADY, XEGY);
  XYDO.tock(CLKPIPE, TADY, XORA);

  bool TOCA = not(XYDO_Q);

  bool TUHU_Q = TUHU.q();
  bool TUKY_Q = TUKY.q();
  bool TAKO_Q = TAKO.q();
  bool SYBE_Q = SYBE.q();

  bool TYBA = and(TUKY_Q, TUHU_Q);
  bool SURY = and(TAKO_Q, TYBA);
  bool SAKE = xor(TUKY_Q, TUHU_Q);
  bool TYGE = xor(TAKO_Q, TYBA);
  bool ROKU = xor(SYBE_Q, SURY);

  TUHU.tock(TOCA, TADY, !TUHU_Q);
  TUKY.tock(TOCA, TADY, SAKE);
  TAKO.tock(TOCA, TADY, TYGE);
  SYBE.tock(TOCA, TADY, ROKU);

  ACAM = not(XEHO_Q);
  AZUB = not(SAVY_Q);
  AMEL = not(XODU_Q);
  AHAL = not(XYDO_Q);
  APUX = not(TUHU_Q);
  ABEF = not(TUKY_Q);
  ADAZ = not(TAKO_Q);
  ASAH = not(SYBE_Q);

  //----------
  // FF41 STAT

  XYVO = and(V4, V7);

  bool NYPE_Q = NYPE.tock(TALU,   LYFE, RUTU_Q); // RUTU_OUT on the schematic?
  bool POPU_Q = POPU.tock(NYPE_Q, LYFE, XYVO);
  NAPO.flip(POPU_Q, LYFE);

  bool XUGU = nand(XEHO_Q, SAVY_Q, XODU_Q, TUKY_Q, SYBE_Q);
  bool XENA = not(FEPO);
  bool XANO = not(XUGU);
  bool PARU = not(!POPU_Q);
  INT_VBL = PARU;
  WODU = and(XENA, XANO);

  PURE = not(RUTU_Q);
  SELA = not(PURE);
  bool TOLU = not(PARU);
  bool TAPA = and(SELA, TOLU);
  INT_OAM = TAPA;
  bool TARU = and(TOLU, WODU);
  INT_HBL = TARU;
  bool VYPU = not(TOLU);
  INT_VBL_BUF = VYPU;

  //---

  bool SEPA = and(CPU_WR2, FF41);

  bool VOGA_Q = VOGA.tock(CLK2, TADY, WODU);
  bool WEGO = or(TOFU, VOGA_Q);
  bool XAJO = and(XEHO_Q, XYDO_Q);
  XYMU = unk2(WEGO, AVAP);
  bool WUSA = unk2(XAJO, WEGO);
  bool TOBA = and(CLKPIPE, WUSA);
  bool SADU = nor(XYMU, PARU);
  bool XATY = nor(ACYL, XYMU);
  bool SEMU = or(TOBA, POVA);
  bool RYJU = not(SEPA);
  bool RYPO = not(SEMU);
  bool PAGO = or(WESY, RYJU);

  //---

  bool RYVE = not(SEPA);
  bool RUGU_Q = RUGU.tock(RYVE, WESY, D6);
  bool REFE_Q = REFE.tock(RYVE, WESY, D5);
  bool ROPO_Q = ROPO.tock(TALU, WESY, PALY);
  bool RUFO_Q = RUFO.tock(RYVE, WESY, D4);
  bool ROXE_Q = ROXE.tock(RYVE, WESY, D3);

  bool PUZO = not(!ROXE_Q);
  bool SASY = not(!REFE_Q);
  bool POFO = not(!RUFO_Q);
  bool POTE = not(!RUGU_Q);
  bool TEBY = not(SADU); // these two are the STAT mode signal
  bool WUGA = not(XATY);

  bool SUKO = amux4(RUGU_Q, ROPO_Q, REFE_Q, INT_OAM, RUFO_Q, INT_VBL, ROXE_Q, INT_HBL);
  bool TUVA = not(SUKO);
  bool VOTY = not(TUVA);
  bool RUPO = unk2(ROPO_Q, PAGO);
  bool SEGO = not(RUPO);

  // pin output?
  CP = RYPO;
  INT_STAT = VOTY;

  bool TOBE = and(CPU_RD2, FF41);
  bool VAVE = TOBE; // buffer, not inverter
  if (TOBE) {
    D0 = TEBY;
    D1 = WUGA;
    D2 = SEGO;
  }
  if (VAVE) {
    D3 = PUZO;
    D4 = POFO;
    D5 = SASY;
    D6 = POTE;
  }

  //----------
  // y counter

  bool NOKO = and(V7, V4, V0, V1);
  LYHA = not(RESET_VIDEO);
  LYFE = not(LYHA);
  bool MYTA_Q = MYTA.tock(NYPE_Q, LYFE, NOKO);
  bool LAMA = nor(MYTA_Q, LYHA);

  bool MUWY_Q = MUWY.flip(RUTU_Q,  LAMA);
  bool MYRO_Q = MYRO.flip(!MUWY_Q, LAMA);
  bool LEXA_Q = LEXA.flip(!MYRO_Q, LAMA);
  bool LYDO_Q = LYDO.flip(!LEXA_Q, LAMA);
  bool LOVU_Q = LOVU.flip(!LYDO_Q, LAMA);
  bool LEMA_Q = LEMA.flip(!LOVU_Q, LAMA);
  bool MATO_Q = MATO.flip(!LEMA_Q, LAMA);
  bool LAFO_Q = LAFO.flip(!MATO_Q, LAMA);

  V0 = MUWY_Q;
  V1 = MYRO_Q;
  V2 = LEXA_Q;
  V3 = LYDO_Q;
  V4 = LOVU_Q;
  V5 = LEMA_Q;
  V6 = MATO_Q;
  V7 = LAFO_Q;
}