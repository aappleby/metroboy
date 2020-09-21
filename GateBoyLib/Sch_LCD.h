#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {
  void dump(Dumper& d, const SchematicTop& top) const;
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);

  uint8_t get_lx() const {
    return (uint8_t)pack_p(SAXO_X0p.qp(), TYPO_X1p.qp(), VYZO_X2p.qp(), TELU_X3p.qp(), SUDE_X4p.qp(), TAHA_X5p.qp(), TYRY_X6p.qp(), 0);
  }

  uint8_t get_ly() const {
    return (uint8_t)pack_p(MUWY_Y0p.qp(), MYRO_Y1p.qp(), LEXA_Y2p.qp(), LYDO_Y3p.qp(), LOVU_Y4p.qp(), LEMA_Y5p.qp(), MATO_Y6p.qp(), LAFO_Y7p.qp());
  }

  uint8_t get_lyc() const {
    return (uint8_t)pack_p(SYRY_LYC0n.q08n(), VUCE_LYC1n.q08n(), SEDY_LYC2n.q08n(), SALO_LYC3n.q08n(),
                           SOTA_LYC4n.q08n(), VAFA_LYC5n.q08n(), VEVO_LYC6n.q08n(), RAHA_LYC7n.q08n());
  }

  /*p21.PARU*/ Sig   PARU_VBLANKp_d4;                // -> pix pipe
  /*p28.ATEJ*/ Sig   ATEJ_VID_LINE_END_TRIGp;        // -> buncha stuff
  /*p21.VYPU*/ Sig   VYPU_INT_VBLANKp;               // -> interrupts
  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp; // -> pix pipe for int stat
  /*p21.RUTU*/ DFF17 RUTU_LINE_P908; // -> pix pipe, could move PURE here. fires on line phase 908, high for 8 phases
  /*p29.CATU*/ DFF17 CATU_VID_LINE_P910; // -> pix pipe, scanner. fires on line phase 910, high for 8 phases
  /*p28.ANEL*/ DFF17 ANEL_VID_LINE_P000; // -> pix pipe for BYHA? fires on line phase 000, high for 8 phases

  // -> pix pipe, sprite scanner, vram bus. Increments when RUTU goes high.
  /*p21.MUWY*/ DFF17 MUWY_Y0p;
  /*p21.MYRO*/ DFF17 MYRO_Y1p;
  /*p21.LEXA*/ DFF17 LEXA_Y2p;
  /*p21.LYDO*/ DFF17 LYDO_Y3p;
  /*p21.LOVU*/ DFF17 LOVU_Y4p;
  /*p21.LEMA*/ DFF17 LEMA_Y5p;
  /*p21.MATO*/ DFF17 MATO_Y6p;
  /*p21.LAFO*/ DFF17 LAFO_Y7p;

private:

  // Increments at line phase 012 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ DFF17 SAXO_X0p; 
  /*p21.TYPO*/ DFF17 TYPO_X1p;
  /*p21.VYZO*/ DFF17 VYZO_X2p;
  /*p21.TELU*/ DFF17 TELU_X3p;
  /*p21.SUDE*/ DFF17 SUDE_X4p;
  /*p21.TAHA*/ DFF17 TAHA_X5p;
  /*p21.TYRY*/ DFF17 TYRY_X6p;

  /*p21.NYPE*/ DFF17 NYPE_LINE_P000; // fires on line phase 000, high for 8 phases
  /*p21.MYTA*/ DFF17 MYTA_LINE_153p; // fires on line 153, phase 004. clears on line 000, phase 004
  /*p21.POPU*/ DFF17 POPU_IN_VBLANKp; // firce on line 144, phase 004. clears on line 000, phase 004 (the real line 000 not the stubby 000 @ 153)
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