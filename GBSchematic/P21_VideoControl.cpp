#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P21_VideoContro {
  struct Input {
    bool CLKPIPE;
    bool CLK2;

    bool FF41;
    bool CPU_RD2;
    bool CPU_WR2;

    bool POVA;
    bool WESY;
    bool ACYL;
    bool AVAP;
    bool ATEJ; // used for x counter reset
    bool TOFU; // used for x counter reset
    bool XYDO;
    bool RESET_VIDEO;
    bool FEPO;

    bool WUVU_Qn; // 2 mhz clock from sprites, I think?

    bool V0,V1,V2,V3,V4,V5,V6,V7;
    bool FF45_D0,FF45_D1,FF45_D2,FF45_D3,FF45_D4,FF45_D5,FF45_D6,FF45_D7;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool PIN_CPG;
    bool PALY;
    bool CP;

    bool INT_VBL;
    bool INT_OAM;
    bool INT_HBL;
    bool INT_VBL_BUF;
    bool INT_STAT;

    // Y counter
    bool V0,V1,V2,V3,V4,V5,V6,V7;

    // X counter
    bool ACAM,AZUB,AMEL,AHAL,APUX,ABEF,ADAZ,ASAH;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF41 STAT
  reg RUGU,REFE,ROPO,RUFO,ROXE;

  // X counter
  reg XEHO,SAVY,XODU,XYDO,TUHU,TUKY,TAKO,SYBE;

  // Y counter
  reg MUWY,MYRO,LEXA,LYDO,LOVU,LEMA,MATO,LAFO;

  // something counter
  reg VENA,SAXO,TYPO,VYZO,TELU,SUDE,TAHA,TYRY;

  // something else counter
  reg NYPE,POPU,NAPO;

  // control regs for something...
  reg RUTU,SYGU,VOGA,MYTA;

  void tick(const Input& in, Output& out) {
    wire LYHA = not(in.RESET_VIDEO);
    wire LYFE = not(LYHA);

    //----------
    // x counter

    wire TADY = nor(in.ATEJ, in.TOFU);

    wire XEHO_Q = XEHO.q();
    wire SAVY_Q = SAVY.q();
    wire XODU_Q = XODU.q();
    wire XYDO_Q = XYDO.q();

    wire XUKE = and(XEHO_Q, SAVY_Q);
    wire XYLE = and(XODU_Q, XUKE);
    wire RYBO = xor(SAVY_Q, XEHO_Q);
    wire XEGY = xor(XODU_Q, XUKE);
    wire XORA = xor(XYDO_Q, XYLE);

    XEHO.tock(in.CLKPIPE, TADY, !XEHO_Q);
    SAVY.tock(in.CLKPIPE, TADY, RYBO);
    XODU.tock(in.CLKPIPE, TADY, XEGY);
    XYDO.tock(in.CLKPIPE, TADY, XORA);

    wire TOCA = not(XYDO_Q);

    wire TUHU_Q = TUHU.q();
    wire TUKY_Q = TUKY.q();
    wire TAKO_Q = TAKO.q();
    wire SYBE_Q = SYBE.q();

    wire TYBA = and(TUKY_Q, TUHU_Q);
    wire SURY = and(TAKO_Q, TYBA);
    wire SAKE = xor(TUKY_Q, TUHU_Q);
    wire TYGE = xor(TAKO_Q, TYBA);
    wire ROKU = xor(SYBE_Q, SURY);

    TUHU.tock(TOCA, TADY, !TUHU_Q);
    TUKY.tock(TOCA, TADY, SAKE);
    TAKO.tock(TOCA, TADY, TYGE);
    SYBE.tock(TOCA, TADY, ROKU);

    // polarity?
    out.ACAM = not(XEHO_Q);
    out.AZUB = not(SAVY_Q);
    out.AMEL = not(XODU_Q);
    out.AHAL = not(XYDO_Q);
    out.APUX = not(TUHU_Q);
    out.ABEF = not(TUKY_Q);
    out.ADAZ = not(TAKO_Q);
    out.ASAH = not(SYBE_Q);

    //----------
    // LY compare

    wire SYFU = xor(in.V7, in.FF45_D7);
    wire TERY = xor(in.V6, in.FF45_D6);
    wire TUCY = xor(in.V5, in.FF45_D5);
    wire TYKU = xor(in.V4, in.FF45_D4);
    wire RASY = xor(in.V3, in.FF45_D3);
    wire REDA = xor(in.V2, in.FF45_D2);
    wire TYDE = xor(in.V1, in.FF45_D1);
    wire RYME = xor(in.V0, in.FF45_D0);

    wire SOVU = nor(SYFU, TERY, TUCY, TYKU);
    wire SUBO = nor(RASY, REDA, TYDE, RYME);
    wire RAPE = nand(SOVU, SUBO);
    wire PALY = not(RAPE);

    out.PALY = PALY;

    //----------
    // top center

    wire RUTU_Q = RUTU.q();
    wire SYGU_Q = SYGU.q();
    wire RUTU_OUT = RUTU_Q; // not sure where this comes from

    wire VENA_Q = VENA.flip(!in.WUVU_Qn, in.RESET_VIDEO);
    wire MUDE = nor(RUTU_Q, LYHA); // schematic says RUTU_OUT, but I think this is just RUTU_Q?
    wire TALU = not(!VENA_Q);

    // FIXME daisy chain
    wire SAXO_Q = SAXO.flip(TALU,      MUDE);
    wire TYPO_Q = TYPO.flip(!SAXO.q(), MUDE);
    wire VYZO_Q = TYPO.flip(!TYPO.q(), MUDE);
    wire TELU_Q = TYPO.flip(!VYZO.q(), MUDE);
    wire SUDE_Q = TYPO.flip(!TELU.q(), MUDE);
    wire TAHA_Q = TYPO.flip(!SUDE.q(), MUDE);
    wire TYRY_Q = TYPO.flip(!TAHA.q(), MUDE);

    wire TOCU = not(SAXO_Q);
    wire VEPE = not(TYPO_Q);
    wire VUTY = not(VYZO_Q);
    wire TUJU = not(TELU_Q);
    wire TAFY = not(SUDE_Q);
    wire TUDA = not(TAHA_Q);
    wire VATE = not(TYRY_Q);

    wire VOKU = nand(TUJU,   TAFY,   TUDA,   VATE,   VUTY,   VEPE,   TOCU);
    wire TOZU = nand(TUJU,   TAFY,   TUDA,   VATE,   VYZO_Q, TYPO_Q, SAXO_Q);
    wire TECE = nand(TUJU,   TAHA_Q, TUDA,   TELU_Q, VYZO_Q, VEPE,   SAXO_Q);
    wire TEBO = nand(TYRY_Q, TAFY,   SUDE_Q, VATE,   VUTY,   TYPO_Q, SAXO_Q);
    wire TEGY = nand(VOKU, TOZU, TECE, TEBO);

    wire SANU = nand(TYRY_Q, TAHA_Q, SUDE_Q, SAXO_Q);
    wire SONO = not(TALU);
    RUTU.tock(SONO, LYFE, SANU);
    SYGU.tock(SONO, LYFE, TEGY);
    wire RYNO = or(SYGU_Q, RUTU_Q);
    wire POGU = not(RYNO);

    out.PIN_CPG = POGU;

    //----------
    // FF41 STAT

    wire XYVO = and(in.V4, in.V7);

    // FIXME daisy chain
    wire NYPE_Q = NYPE.tock(TALU,     LYFE, RUTU_Q); // RUTU_OUT on the schematic?
    wire POPU_Q = POPU.tock(NYPE.q(), LYFE, XYVO);
    wire NAPO_Q = NAPO.flip(POPU.q(), LYFE);

    wire XUGU = nand(XEHO_Q, SAVY_Q, XODU_Q, TUKY_Q, SYBE_Q);
    wire XENA = not(in.FEPO);
    wire XANO = not(XUGU);
    wire PARU = not(!POPU_Q);
    wire WODU = and(XENA, XANO);

    wire PURE = not(RUTU_Q);
    wire SELA = not(PURE);
    wire TOLU = not(PARU);
    wire TAPA = and(SELA, TOLU);
    wire TARU = and(TOLU, WODU);
    wire VYPU = not(TOLU);

    out.INT_VBL = PARU;
    out.INT_OAM = TAPA;
    out.INT_HBL = TARU;
    out.INT_VBL_BUF = VYPU;

    //---

    wire SEPA = and(in.CPU_WR2, in.FF41);

    wire VOGA_Q = VOGA.tock(in.CLK2, TADY, WODU);
    wire WEGO = or(in.TOFU, VOGA_Q);
    wire XAJO = and(XEHO_Q, XYDO_Q);
    wire XYMU = unk2(WEGO, in.AVAP);
    wire WUSA = unk2(XAJO, WEGO);
    wire TOBA = and(in.CLKPIPE, WUSA);
    wire SADU = nor(XYMU, PARU);
    wire XATY = nor(in.ACYL, XYMU);
    wire SEMU = or(TOBA, in.POVA);
    wire RYJU = not(SEPA);
    wire RYPO = not(SEMU);
    wire PAGO = or(in.WESY, RYJU);

    out.CP = RYPO;

    //---

    wire RYVE = not(SEPA);
    wire RUGU_Q = RUGU.tock(RYVE, in.WESY, in.D6);
    wire REFE_Q = REFE.tock(RYVE, in.WESY, in.D5);
    wire ROPO_Q = ROPO.tock(TALU, in.WESY, PALY); // this seems odd
    wire RUFO_Q = RUFO.tock(RYVE, in.WESY, in.D4);
    wire ROXE_Q = ROXE.tock(RYVE, in.WESY, in.D3);

    wire PUZO = not(!ROXE_Q);
    wire SASY = not(!REFE_Q);
    wire POFO = not(!RUFO_Q);
    wire POTE = not(!RUGU_Q);
    wire TEBY = not(SADU); // these two are the STAT mode signal
    wire WUGA = not(XATY);

    wire SUKO = amux4(RUGU_Q, ROPO_Q, REFE_Q, out.INT_OAM, RUFO_Q, out.INT_VBL, ROXE_Q, out.INT_HBL);
    wire TUVA = not(SUKO);
    wire VOTY = not(TUVA);
    wire RUPO = unk2(ROPO_Q, PAGO);
    wire SEGO = not(RUPO);

    out.INT_STAT = VOTY;

    wire TOBE = and(in.CPU_RD2, in.FF41);
    wire VAVE = TOBE; // buffer, not inverter

    if (TOBE) {
      out.D_OE = true;
      out.D0 = TEBY;
      out.D1 = WUGA;
      out.D2 = SEGO;
    }
    if (VAVE) {
      out.D_OE = true;
      out.D3 = PUZO;
      out.D4 = POFO;
      out.D5 = SASY;
      out.D6 = POTE;
    }

    //----------
    // y counter

    wire NOKO = and(in.V7, in.V4, in.V0, in.V1);
    wire MYTA_Q = MYTA.tock(NYPE_Q, LYFE, NOKO);
    wire LAMA = nor(MYTA_Q, LYHA);

    wire MUWY_Q = MUWY.flip(RUTU_Q,  LAMA);
    wire MYRO_Q = MYRO.flip(!MUWY_Q, LAMA);
    wire LEXA_Q = LEXA.flip(!MYRO_Q, LAMA);
    wire LYDO_Q = LYDO.flip(!LEXA_Q, LAMA);
    wire LOVU_Q = LOVU.flip(!LYDO_Q, LAMA);
    wire LEMA_Q = LEMA.flip(!LOVU_Q, LAMA);
    wire MATO_Q = MATO.flip(!LEMA_Q, LAMA);
    wire LAFO_Q = LAFO.flip(!MATO_Q, LAMA);

    out.V0 = MUWY_Q;
    out.V1 = MYRO_Q;
    out.V2 = LEXA_Q;
    out.V3 = LYDO_Q;
    out.V4 = LOVU_Q;
    out.V5 = LEMA_Q;
    out.V6 = MATO_Q;
    out.V7 = LAFO_Q;
  }
};