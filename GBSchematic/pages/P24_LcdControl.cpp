#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P24_LcdControl {
  struct Input {
    bool CLK2;
    bool RESET_VIDEO;

    bool XYMU;
    bool SYLO;
    bool MOSU;
    bool WODU;
    bool MYVO;
    bool FEPO;
    bool LYRY;
    bool LYFE;
    bool NYPE;
    bool PURE;
    bool XYDO;
    bool ROXY; // CLPIPE doesn't tick if this is high
    bool AVAP;

    bool FF04_D0n;
    bool FF04_D1n;
    bool FF40_D7;

    bool NAPO_OUT;
    bool RUTU_OUT;

    bool V0,V1,V2,V3,V4,V5,V6,V7;
  };

  struct Output {
    bool CLKPIPE;

    bool PIN_S;
    bool PIN_CPL;
    bool PIN_FR;
    bool PIN_ST;
  };

  // 3-bit shift register
  reg NYKA,PORY,PYGO;

  // dunno
  reg PAHO,LUCA,LEBE,MEDA;

  void tick(const Input& in, Output& out) {
    wire LOBY = not(in.XYMU);
    wire NAFY = nor(in.MOSU, LOBY);
    wire NYKA_Q = NYKA.tock(in.CLK2, NAFY, in.LYRY);
    wire PORY_Q = PORY.tock(in.MYVO, NAFY, NYKA_Q);
    wire PYGO_Q = PYGO.tock(in.CLK2, in.XYMU, PORY_Q);
    wire TOMU = not(in.SYLO);
    wire POKY = unk2(PYGO_Q, LOBY);
    wire SOCY = not(TOMU);
    wire VYBO = nor(in.FEPO, in.WODU, in.MYVO);
    wire TYFA = and(SOCY, POKY, VYBO);
    wire SEGU = not(TYFA);
    wire SACU = nor(SEGU, in.ROXY);
    wire ROXO = not(SEGU);
    wire PAHO_Q = PAHO.tock(ROXO,in.XYMU,in.XYDO);
    wire TOFU = not(in.RESET_VIDEO);

    // FIXME another logic loop...
    wire POME = nor(in.AVAP, /*POFY*/false);
    wire RUJU = or(PAHO_Q, TOFU, POME);
    wire POFY = not(RUJU);
    wire RUZE = not(POFY);

    out.PIN_ST = RUZE;
    out.CLKPIPE = SACU;

    //----------
    // Vertical sync

    wire NERU = nor(in.V0, in.V1, in.V2, in.V3, in.V4, in.V5, in.V6, in.V7);
    wire MEDA_Q = MEDA.tock(in.NYPE, in.LYFE, NERU);
    wire MURE = not(MEDA_Q);

    out.PIN_S = MURE;

    //----------

    wire LOFU = not(in.RUTU_OUT);
    wire LUCA_Q = LUCA.flip(LOFU, in.LYFE);
    wire MAGU = xor(in.NAPO_OUT, LUCA_Q);
    wire LEBE_Q = LEBE.flip(!LUCA.q(), in.LYFE);
    wire MECO = not(MAGU);
    wire KEBO = not(MECO);
    wire KASA = not(in.PURE);
    wire UMOB = not(in.FF04_D0n);
    wire USEC = not(in.FF04_D1n);
    wire KEDY = not(in.FF40_D7);
    wire KAHE = amux2(in.FF40_D7, KASA, KEDY, UMOB);
    wire KUPA = amux2(in.FF40_D7, KEBO, KEDY, USEC);
    wire KYMO = not(KAHE);
    wire KOFO = not(KUPA);

    // Unused on schematic
    (void)LEBE_Q;

    out.PIN_CPL = KYMO;
    out.PIN_FR = KOFO;
  }
};