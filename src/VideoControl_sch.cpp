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

extern bool ATEJ;
extern bool XEHO_Q;
extern bool SAVY;
extern bool XODU;
extern bool TUKY;
extern bool SYBE;
extern bool FEPO;
extern bool RUTU;
extern bool RUTU_OUT;
extern bool LYFE;
extern bool LYHA;
extern bool WUVU_Qn;
extern bool RESET_VIDEO;
extern bool CLKPIPE;
extern bool CPU_RD2;
extern bool CPU_WR2;
extern bool FF41;
extern bool CLK2;

extern bool TOFU;
extern bool XEHO;
extern bool XYDO;
extern bool ACYL;
extern bool WESY;
extern bool PALY;
extern bool AVAP;
extern bool POVA;

//----------
// outputs

bool CP; // lcd clock signal

bool PALY; // ly compare match
bool XYVO; // line >= 144;

bool LYHA; // !RESET_VIDEO
bool LYFE; // !!RESET_VIDEO

bool INT_OAM;
bool INT_HBL;
bool INT_VBL;
bool INT_VBL_BUF;
bool INT_STAT;

bool NAPO_OUT;
bool NYPE;
bool PIN_CPG;

bool V0;
bool V1;
bool V2;
bool V3;
bool V4;
bool V5;
bool V6;
bool V7;

//-----------------------------------------------------------------------------
// registers

bool NYPE_Q, NYPE_CLK;
bool POPU_Q, POPU_CLK;
bool NAPO_Q, NAPO_CLK;

//----------

bool VENA_Q, VENA_CLK;

bool SAXO_Q, SAXO_CLK;
bool TYPO_Q, TYPO_CLK;
bool VYZO_Q, VYZO_CLK;
bool TELU_Q, TELU_CLK;
bool SUDE_Q, SUDE_CLK;
bool TAHA_Q, TAHA_CLK;
bool TYRY_Q, TYRY_CLK;

bool RUTU_Q, RUTU_CLK;
bool SYGU_Q, SYGU_CLK;

//----------
// FF41 STAT

bool VOGA_Q, VOGA_CLK;

bool RUGU_Q, RUGU_CLK;
bool REFE_Q, REFE_CLK;
bool ROPO_Q, ROPO_CLK;
bool RUFO_Q, RUFO_CLK;
bool ROXE_Q, ROXE_CLK;

//----------
// x counter

bool XEHO_Q, XEHO_CLK;
bool SAVY_Q, SAVY_CLK;
bool XODU_Q, XODU_CLK;
bool XYDO_Q, XYDO_CLK;
bool TUHU_Q, TUHU_CLK;
bool TUKY_Q, TUKY_CLK;
bool TAKO_Q, TAKO_CLK;
bool SYBE_Q, SYBE_CLK;

//----------
// y counter

bool MYTA_Q, MYTA_CLK;

bool MUWY_Q, MUWY_CLK;
bool MYRO_Q, MYRO_CLK;
bool LEXA_Q, LEXA_CLK;
bool LYDO_Q, LYDO_CLK;
bool LOVU_Q, LOVU_CLK;
bool LEMA_Q, LEMA_CLK;
bool MATO_Q, MATO_CLK;
bool LAFO_Q, LAFO_CLK;

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

  bool MUDE = nor(RUTU_OUT, LYHA);
  bool TALU = not(!VENA_Q);

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

  bool NYPE_Q_ = NYPE_Q;
  bool POPU_Q_ = POPU_Q;
  bool NAPO_Q_ = NAPO_Q;

  if (NYPE_CLK && !TALU)   NYPE_Q_ = RUTU_OUT;
  if (POPU_CLK && !NYPE_Q) POPU_Q_ = XYVO;
  if (NAPO_CLK && !POPU_Q) NAPO_Q_ = !NAPO_Q;

  if (!LYFE) NYPE_Q_ = 0;
  if (!LYFE) POPU_Q_ = 0;
  if (!LYFE) NAPO_Q_ = 0;

  NYPE_CLK = TALU;
  POPU_CLK = NYPE_Q;
  NAPO_CLK = POPU_Q;

  NYPE_Q = NYPE_Q_;
  POPU_Q = POPU_Q_;
  NAPO_Q = NAPO_Q_;

  NYPE = !NYPE_Q;
  NAPO_OUT = NAPO_Q;

  //----------

  bool SEPA = and(CPU_WR2, FF41);
  bool RYVE = not(SEPA);

  bool WEGO = or(TOFU, VOGA_Q);
  bool XAJO = and(XEHO, XYDO);
  bool XYMU = unk2(WEGO, AVAP);
  bool WUSA = unk2(XAJO, WEGO);
  bool TOBA = and(CLKPIPE, WUSA);
  bool TOBE = and(CPU_RD2, FF41);
  
  //bool VAVE = not(TOBE);
  bool VAVE = TOBE; // buffer, not inverter
  
  bool SADU = nor(XYMU, PARU);
  bool XATY = nor(ACYL, XYMU);
  bool SEMU = or(TOBA, POVA);
  bool RYJU = not(SEPA);
  bool RYPO = not(SEMU);

  bool PUZO = not(!ROXE_Q);
  bool SASY = not(!REFE_Q);
  bool POFO = not(!RUFO_Q);
  bool POTE = not(!RUGU_Q);
  
  if (VAVE) {
    D3 = PUZO;
    D5 = SASY;
    D4 = POFO;
    D6 = POTE;
  }

  // these two are the STAT mode signal
  bool TEBY = not(SADU);
  bool WUGA = not(XATY);

  CP = RYPO;

  bool PAGO = or(WESY, RYJU);
  bool SUKO = amux4(RUGU_Q, ROPO_Q, REFE_Q, INT_OAM, RUFO_Q, INT_VBL, ROXE_Q, INT_HBL);
  bool TUVA = not(SUKO);
  bool VOTY = not(TUVA);
  INT_STAT = VOTY;

  bool RUPO = unk2(ROPO_Q, PAGO);
  bool SEGO = not(RUPO);
  
  if (TOBE) {
    D0 = TEBY;
    D1 = WUGA;
    D2 = SEGO;
  }

  //----------
  // x counter

  bool TADY = nor(ATEJ, TOFU);
  bool XUKE = and(XEHO_Q, SAVY_Q);
  bool XYLE = and(XODU_Q, XUKE);
  bool RYBO = xor(SAVY_Q, XEHO_Q);
  bool XEGY = xor(XODU_Q, XUKE);
  bool XORA = xor(XYDO_Q, XYLE);
  
  bool ACAM = not(XEHO_Q);
  bool AZUB = not(SAVY_Q);
  bool AMEL = not(XODU_Q);
  bool AHAL = not(XYDO_Q);

  bool TOCA = not(XYDO_Q);
  bool TYBA = and(TUKY_Q, TUHU_Q);
  bool SURY = and(TAKO_Q, TYBA);
  bool SAKE = xor(TUKY_Q, TUHU_Q);
  bool TYGE = xor(TAKO_Q, TYBA);
  bool ROKU = xor(SYBE_Q, SURY);

  bool APUX = not(TUHU_Q);
  bool ABEF = not(TUKY_Q);
  bool ADAZ = not(TAKO_Q);
  bool ASAH = not(SYBE_Q);

  // x counter probably goes to ppu...
  (void)ACAM;
  (void)AZUB;
  (void)AMEL;
  (void)AHAL;
  (void)APUX;
  (void)ABEF;
  (void)ADAZ;
  (void)ASAH;

  //----------
  // y counter

  bool NOKO = and(V7, V4, V0, V1);
  LYHA = not(RESET_VIDEO);
  LYFE = not(LYHA);
  bool LAMA = nor(MYTA_Q, LYHA);

  //----------
  // registers

  bool VENA_Q_ = VENA_Q;

  bool SAXO_Q_ = SAXO_Q;
  bool TYPO_Q_ = TYPO_Q;
  bool VYZO_Q_ = VYZO_Q;
  bool TELU_Q_ = TELU_Q;
  bool SUDE_Q_ = SUDE_Q;
  bool TAHA_Q_ = TAHA_Q;
  bool TYRY_Q_ = TYRY_Q;

  bool RUTU_Q_ = RUTU_Q;
  bool SYGU_Q_ = SYGU_Q;

  if (VENA_CLK && !WUVU_Qn) VENA_Q_ = !VENA_Q;

  if (SAXO_CLK && !TALU)    SAXO_Q_ = !SAXO_Q;
  if (TYPO_CLK && !!SAXO_Q) TYPO_Q_ = !TYPO_Q;
  if (VYZO_CLK && !!TYPO_Q) VYZO_Q_ = !VYZO_Q;
  if (TELU_CLK && !!VYZO_Q) TELU_Q_ = !TELU_Q;
  if (SUDE_CLK && !!TELU_Q) SUDE_Q_ = !SUDE_Q;
  if (TAHA_CLK && !!SUDE_Q) TAHA_Q_ = !TAHA_Q;
  if (TYRY_CLK && !!TAHA_Q) TYRY_Q_ = !TYRY_Q;

  if (RUTU_CLK && !SONO) RUTU_Q_ = SANU;
  if (SYGU_CLK && !SONO) SYGU_Q_ = TEGY;

  if (!RESET_VIDEO) {
    VENA_Q_ = 0;
  }

  if (!MUDE) {
    SAXO_Q_ = 0;
    TYPO_Q_ = 0;
    VYZO_Q_ = 0;
    TELU_Q_ = 0;
    SUDE_Q_ = 0;
    TAHA_Q_ = 0;
    TYRY_Q_ = 0;
  }

  if (!LYFE) {
    RUTU_Q_ = 0;
    SYGU_Q_ = 0;
  }

  VENA_CLK = WUVU_Qn;
  SAXO_CLK = TALU;
  TYPO_CLK = !SAXO_Q;
  VYZO_CLK = !TYPO_Q;
  TELU_CLK = !VYZO_Q;
  SUDE_CLK = !TELU_Q;
  TAHA_CLK = !SUDE_Q;
  TYRY_CLK = !TAHA_Q;
  RUTU_CLK = SONO;
  SYGU_CLK = SONO;

  VENA_Q = VENA_Q_;
  SAXO_Q = SAXO_Q_;
  TYPO_Q = TYPO_Q_;
  VYZO_Q = VYZO_Q_;
  TELU_Q = TELU_Q_;
  SUDE_Q = SUDE_Q_;
  TAHA_Q = TAHA_Q_;
  TYRY_Q = TYRY_Q_;
  RUTU_Q = RUTU_Q_;
  SYGU_Q = SYGU_Q_;

  //----------
  // registers

  bool VOGA_Q_ = VOGA_Q;

  bool RUGU_Q_ = RUGU_Q;
  bool REFE_Q_ = REFE_Q;
  bool ROPO_Q_ = ROPO_Q;
  bool RUFO_Q_ = RUFO_Q;
  bool ROXE_Q_ = ROXE_Q;

  if (VOGA_CLK && !CLK2) VOGA_Q_ = WODU;

  if (RUGU_CLK && !RYVE) RUGU_Q_ = D6;
  if (REFE_CLK && !RYVE) REFE_Q_ = D5;
  if (ROPO_CLK && !TALU) ROPO_Q_ = PALY;
  if (RUFO_CLK && !RYVE) RUFO_Q_ = D4;
  if (ROXE_CLK && !RYVE) ROXE_Q_ = D3;

  if (!TADY) VOGA_Q_ = 0;
  if (!WESY) RUGU_Q_ = 0;
  if (!WESY) REFE_Q_ = 0;
  if (!WESY) ROPO_Q_ = 0;
  if (!WESY) RUFO_Q_ = 0;
  if (!WESY) ROXE_Q_ = 0;
 
  VOGA_CLK = CLK2;
  RUGU_CLK = RYVE;
  REFE_CLK = RYVE;
  ROPO_CLK = TALU;
  RUFO_CLK = RYVE;
  ROXE_CLK = RYVE;

  VOGA_Q = VOGA_Q_;
  RUGU_Q = RUGU_Q_;
  REFE_Q = REFE_Q_;
  ROPO_Q = ROPO_Q_;
  RUFO_Q = RUFO_Q_;
  ROXE_Q = ROXE_Q_;

  //----------
  // x counter registers

  bool XEHO_Q_ = XEHO_Q;
  bool SAVY_Q_ = SAVY_Q;
  bool XODU_Q_ = XODU_Q;
  bool XYDO_Q_ = XYDO_Q;
  bool TUHU_Q_ = TUHU_Q;
  bool TUKY_Q_ = TUKY_Q;
  bool TAKO_Q_ = TAKO_Q;
  bool SYBE_Q_ = SYBE_Q;

  if (XEHO_CLK && !CLKPIPE) XEHO_Q_ = !XEHO_Q;
  if (SAVY_CLK && !CLKPIPE) SAVY_Q_ = RYBO;
  if (XODU_CLK && !CLKPIPE) XODU_Q_ = XEGY;
  if (XYDO_CLK && !CLKPIPE) XYDO_Q_ = XORA;

  if (TUHU_CLK && !TOCA) TUHU_Q_ = !TUHU_Q;
  if (TUKY_CLK && !TOCA) TUKY_Q_ = SAKE;
  if (TAKO_CLK && !TOCA) TAKO_Q_ = TYGE;
  if (SYBE_CLK && !TOCA) SYBE_Q_ = ROKU;

  XEHO_CLK = CLKPIPE;
  SAVY_CLK = CLKPIPE;
  XODU_CLK = CLKPIPE;
  XYDO_CLK = CLKPIPE;

  TUHU_CLK = TOCA;
  TUKY_CLK = TOCA;
  TAKO_CLK = TOCA;
  SYBE_CLK = TOCA;

  XEHO_Q = XEHO_Q_;
  SAVY_Q = SAVY_Q_;
  XODU_Q = XODU_Q_;
  XYDO_Q = XYDO_Q_;
  TUHU_Q = TUHU_Q_;
  TUKY_Q = TUKY_Q_;
  TAKO_Q = TAKO_Q_;
  SYBE_Q = SYBE_Q_;

  //----------
  // y counter registers

  bool MYTA_Q_ = MYTA_Q;
  bool MUWY_Q_ = MUWY_Q;
  bool MYRO_Q_ = MYRO_Q;
  bool LEXA_Q_ = LEXA_Q;
  bool LYDO_Q_ = LYDO_Q;
  bool LOVU_Q_ = LOVU_Q;
  bool LEMA_Q_ = LEMA_Q;
  bool MATO_Q_ = MATO_Q;
  bool LAFO_Q_ = LAFO_Q;

  if (MYTA_CLK && !NYPE) MYTA_Q_ = NOKO;

  if (MUWY_CLK && !RUTU)    MUWY_Q_ = !MUWY_Q;
  if (MYRO_CLK && !!MUWY_Q) MYRO_Q_ = !MYRO_Q;
  if (LEXA_CLK && !!MYRO_Q) LEXA_Q_ = !LEXA_Q;
  if (LYDO_CLK && !!LEXA_Q) LYDO_Q_ = !LYDO_Q;
  if (LOVU_CLK && !!LYDO_Q) LOVU_Q_ = !LOVU_Q;
  if (LEMA_CLK && !!LOVU_Q) LEMA_Q_ = !LEMA_Q;
  if (MATO_CLK && !!LEMA_Q) MATO_Q_ = !MATO_Q;
  if (LAFO_CLK && !!MATO_Q) LAFO_Q_ = !LAFO_Q;

  if (!LYFE) MYTA_Q_ = 0;
  if (!LAMA) MUWY_Q_ = 0;
  if (!LAMA) MYRO_Q_ = 0;
  if (!LAMA) LEXA_Q_ = 0;
  if (!LAMA) LYDO_Q_ = 0;
  if (!LAMA) LOVU_Q_ = 0;
  if (!LAMA) LEMA_Q_ = 0;
  if (!LAMA) MATO_Q_ = 0;
  if (!LAMA) LAFO_Q_ = 0;

  MYTA_Q = MYTA_Q_;
  MUWY_Q = MUWY_Q_;
  MYRO_Q = MYRO_Q_;
  LEXA_Q = LEXA_Q_;
  LYDO_Q = LYDO_Q_;
  LOVU_Q = LOVU_Q_;
  LEMA_Q = LEMA_Q_;
  MATO_Q = MATO_Q_;
  LAFO_Q = LAFO_Q_;

  V0 = MUWY_Q;
  V1 = MYRO_Q;
  V2 = LEXA_Q;
  V3 = LYDO_Q;
  V4 = LOVU_Q;
  V5 = LEMA_Q;
  V6 = MATO_Q;
  V7 = LAFO_Q;

}