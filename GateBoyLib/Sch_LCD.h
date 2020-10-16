#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {
  void reset_cart() {
    PARU_VBLANKp_d4.reset(TRI_D1NP);
    ATEJ_LINE_TRIGp.reset(TRI_D0NP);
    VYPU_INT_VBLANKp.reset(TRI_D1NP);

    ROPO_LY_MATCH_SYNCp.reset(REG_D1C0);

    CATU_LINE_P000.reset(REG_D0C1);
    NYPE_LINE_P002.reset(REG_D0C0);
    ANEL_LINE_P002.reset(REG_D0C0);
    RUTU_LINE_P910.reset(REG_D0C1);

    MUWY_Y0p.reset(REG_D0C0);
    MYRO_Y1p.reset(REG_D0C1);
    LEXA_Y2p.reset(REG_D0C1);
    LYDO_Y3p.reset(REG_D0C1);
    LOVU_Y4p.reset(REG_D0C1);
    LEMA_Y5p.reset(REG_D0C1);
    MATO_Y6p.reset(REG_D0C1);
    LAFO_Y7p.reset(REG_D0C1);

    SAXO_X0p.reset(REG_D0C0);
    TYPO_X1p.reset(REG_D1C1);
    VYZO_X2p.reset(REG_D0C0);
    TELU_X3p.reset(REG_D0C1);
    SUDE_X4p.reset(REG_D0C1);
    TAHA_X5p.reset(REG_D1C1);
    TYRY_X6p.reset(REG_D1C0);

    POPU_IN_VBLANKp .reset(REG_D1C0);
    MYTA_LINE_153p  .reset(REG_D1C1);
    SYGU_LINE_STROBE.reset(REG_D0C1);
    MEDA_VSYNC_OUTn .reset(REG_D0C1);
    LUCA_LINE_EVEN  .reset(REG_D1C1);
    NAPO_FRAME_EVEN .reset(REG_D0C1);

    SYRY_LYC0n.reset(REG_D1C1);
    VUCE_LYC1n.reset(REG_D1C1);
    SEDY_LYC2n.reset(REG_D1C1);
    SALO_LYC3n.reset(REG_D1C1);
    SOTA_LYC4n.reset(REG_D1C1);
    VAFA_LYC5n.reset(REG_D1C1);
    VEVO_LYC6n.reset(REG_D1C1);
    RAHA_LYC7n.reset(REG_D1C1);
  }

  void reset_boot() {
    PARU_VBLANKp_d4.reset(TRI_HZNP);
    ATEJ_LINE_TRIGp.reset(TRI_HZNP);
    VYPU_INT_VBLANKp.reset(TRI_HZNP);

    ROPO_LY_MATCH_SYNCp.reset(REG_D0C0);

    CATU_LINE_P000.reset(REG_D0C0);
    NYPE_LINE_P002.reset(REG_D0C0);
    ANEL_LINE_P002.reset(REG_D0C0);
    RUTU_LINE_P910.reset(REG_D0C0);

    MUWY_Y0p.reset(REG_D0C0);
    MYRO_Y1p.reset(REG_D0C0);
    LEXA_Y2p.reset(REG_D0C0);
    LYDO_Y3p.reset(REG_D0C0);
    LOVU_Y4p.reset(REG_D0C0);
    LEMA_Y5p.reset(REG_D0C0);
    MATO_Y6p.reset(REG_D0C0);
    LAFO_Y7p.reset(REG_D0C0);
    SAXO_X0p.reset(REG_D0C0);
    TYPO_X1p.reset(REG_D0C0);
    VYZO_X2p.reset(REG_D0C0);
    TELU_X3p.reset(REG_D0C0);
    SUDE_X4p.reset(REG_D0C0);
    TAHA_X5p.reset(REG_D0C0);
    TYRY_X6p.reset(REG_D0C0);

    POPU_IN_VBLANKp.reset(REG_D0C0);
    MYTA_LINE_153p.reset(REG_D0C0);
    SYGU_LINE_STROBE.reset(REG_D0C0);
    MEDA_VSYNC_OUTn.reset(REG_D0C0);
    LUCA_LINE_EVEN.reset(REG_D0C0);
    NAPO_FRAME_EVEN.reset(REG_D0C0);

    SYRY_LYC0n.reset(REG_D0C0);
    VUCE_LYC1n.reset(REG_D0C0);
    SEDY_LYC2n.reset(REG_D0C0);
    SALO_LYC3n.reset(REG_D0C0);
    SOTA_LYC4n.reset(REG_D0C0);
    VAFA_LYC5n.reset(REG_D0C0);
    VEVO_LYC6n.reset(REG_D0C0);
    RAHA_LYC7n.reset(REG_D0C0);
  }

  void dump(Dumper& d, const SchematicTop& top) const;
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);

  uint8_t get_lx() const {
    return (uint8_t)pack_p(SAXO_X0p.qp17(), TYPO_X1p.qp17(), VYZO_X2p.qp17(), TELU_X3p.qp17(), SUDE_X4p.qp17(), TAHA_X5p.qp17(), TYRY_X6p.qp17(), 0);
  }

  uint8_t get_ly() const {
    return (uint8_t)pack_p(MUWY_Y0p.qp17(), MYRO_Y1p.qp17(), LEXA_Y2p.qp17(), LYDO_Y3p.qp17(), LOVU_Y4p.qp17(), LEMA_Y5p.qp17(), MATO_Y6p.qp17(), LAFO_Y7p.qp17());
  }

  uint8_t get_lyc() const {
    return (uint8_t)pack_p(SYRY_LYC0n.qn08(), VUCE_LYC1n.qn08(), SEDY_LYC2n.qn08(), SALO_LYC3n.qn08(),
                           SOTA_LYC4n.qn08(), VAFA_LYC5n.qn08(), VEVO_LYC6n.qn08(), RAHA_LYC7n.qn08());
  }

  /*p21.PARU*/ Sig   PARU_VBLANKp_d4;     // -> pix pipe
  /*p28.ATEJ*/ Sig   ATEJ_LINE_TRIGp;     // -> buncha stuff
  /*p21.VYPU*/ Sig   VYPU_INT_VBLANKp;    // -> interrupts

  /*p21.PURE*/ Sig PURE_LINE_P908n;
  /*p21.SELA*/ Sig SELA_LINE_P908p;

  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp; // -> pix pipe for int stat, fires on P002, clears on the following P002

  /*p29.CATU*/ DFF17 CATU_LINE_P000; // -> pix pipe, scanner.
  /*p21.NYPE*/ DFF17 NYPE_LINE_P002;
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002; // -> pix pipe for BYHA?
  /*p21.RUTU*/ DFF17 RUTU_LINE_P910; // -> pix pipe, could move PURE here.

  // -> pix pipe, sprite scanner, vram bus. Increments at P910
  /*p21.MUWY*/ DFF17 MUWY_Y0p;
  /*p21.MYRO*/ DFF17 MYRO_Y1p;
  /*p21.LEXA*/ DFF17 LEXA_Y2p;
  /*p21.LYDO*/ DFF17 LYDO_Y3p;
  /*p21.LOVU*/ DFF17 LOVU_Y4p;
  /*p21.LEMA*/ DFF17 LEMA_Y5p;
  /*p21.MATO*/ DFF17 MATO_Y6p;
  /*p21.LAFO*/ DFF17 LAFO_Y7p;

private:

  // Increments at P010 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ DFF17 SAXO_X0p;
  /*p21.TYPO*/ DFF17 TYPO_X1p;
  /*p21.VYZO*/ DFF17 VYZO_X2p;
  /*p21.TELU*/ DFF17 TELU_X3p;
  /*p21.SUDE*/ DFF17 SUDE_X4p;
  /*p21.TAHA*/ DFF17 TAHA_X5p;
  /*p21.TYRY*/ DFF17 TYRY_X6p;

  /*p21.POPU*/ DFF17 POPU_IN_VBLANKp; // FIXME firee on line 144, phase 002. clears on line 000, phase 004 (the real line 000 not the stubby 000 @ 153)
  /*p21.MYTA*/ DFF17 MYTA_LINE_153p;  // FIXME fires on line 153, phase 002. clears on line 000, phase 002
  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE;
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn;
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVEN;
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVEN;

  // FF45 - LYC
  /*p23.SYRY*/ DFF9 SYRY_LYC0n;
  /*p23.VUCE*/ DFF9 VUCE_LYC1n;
  /*p23.SEDY*/ DFF9 SEDY_LYC2n;
  /*p23.SALO*/ DFF9 SALO_LYC3n;
  /*p23.SOTA*/ DFF9 SOTA_LYC4n;
  /*p23.VAFA*/ DFF9 VAFA_LYC5n;
  /*p23.VEVO*/ DFF9 VEVO_LYC6n;
  /*p23.RAHA*/ DFF9 RAHA_LYC7n;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics