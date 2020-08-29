#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {
  void dump(Dumper& d, const SchematicTop& top) const;
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);

  uint8_t get_x() const {
    return (uint8_t)pack_p(SAXO_X0p.qp(), TYPO_X1p.qp(), VYZO_X2p.qp(), TELU_X3p.qp(), SUDE_X4p.qp(), TAHA_X5p.qp(), TYRY_X6p.qp(), 0);
  }

  uint8_t get_y() const {
    return (uint8_t)pack_p(MUWY_Y0p.qp(), MYRO_Y1p.qp(), LEXA_Y2p.qp(), LYDO_Y3p.qp(), LOVU_Y4p.qp(), LEMA_Y5p.qp(), MATO_Y6p.qp(), LAFO_Y7p.qp());
  }

  uint8_t get_lyc() const {
    return (uint8_t)pack_p(SYRY_LYC0n.qn(), VUCE_LYC1n.qn(), SEDY_LYC2n.qn(), SALO_LYC3n.qn(),
                           SOTA_LYC4n.qn(), VAFA_LYC5n.qn(), VEVO_LYC6n.qn(), RAHA_LYC7n.qn());
  }

  /*p21.PARU*/ Sig   PARU_VBLANKp_d4;                // -> pix pipe
  /*p28.ATEJ*/ Sig   ATEJ_VID_LINE_END_TRIGp;        // -> buncha stuff
  /*p21.VYPU*/ Sig   VYPU_INT_VBLANKp;               // -> interrupts
  /*p21.ROPO*/ RegQP ROPO_LY_MATCH_SYNCp = REG_D0C0; // -> pix pipe for int stat
  /*p21.RUTU*/ RegQP RUTU_LINE_P908      = REG_D0C0; // -> pix pipe, could move PURE here. fires on line phase 908, high for 8 phases
  /*p29.CATU*/ RegQP CATU_VID_LINE_P910  = REG_D0C0; // -> pix pipe, scanner. fires on line phase 910, high for 8 phases
  /*p28.ANEL*/ RegQP ANEL_VID_LINE_P000  = REG_D0C0; // -> pix pipe for BYHA? fires on line phase 000, high for 8 phases

  // -> pix pipe, sprite scanner, vram bus. Increments when RUTU goes high.
  /*p21.MUWY*/ RegQPN MUWY_Y0p = REG_D0C0;
  /*p21.MYRO*/ RegQPN MYRO_Y1p = REG_D0C0;
  /*p21.LEXA*/ RegQPN LEXA_Y2p = REG_D0C0;
  /*p21.LYDO*/ RegQPN LYDO_Y3p = REG_D0C0;
  /*p21.LOVU*/ RegQPN LOVU_Y4p = REG_D0C0;
  /*p21.LEMA*/ RegQPN LEMA_Y5p = REG_D0C0;
  /*p21.MATO*/ RegQPN MATO_Y6p = REG_D0C0;
  /*p21.LAFO*/ RegQPN LAFO_Y7p = REG_D0C0;

private:

  // Increments at line phase 012 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ RegQPN SAXO_X0p = REG_D0C0; 
  /*p21.TYPO*/ RegQPN TYPO_X1p = REG_D0C0;
  /*p21.VYZO*/ RegQPN VYZO_X2p = REG_D0C0;
  /*p21.TELU*/ RegQPN TELU_X3p = REG_D0C0;
  /*p21.SUDE*/ RegQPN SUDE_X4p = REG_D0C0;
  /*p21.TAHA*/ RegQPN TAHA_X5p = REG_D0C0;
  /*p21.TYRY*/ RegQPN TYRY_X6p = REG_D0C0;

  /*p21.NYPE*/ RegQPN NYPE_LINE_P000   = REG_D0C0; // fires on line phase 000, high for 8 phases
  /*p21.MYTA*/ RegQP  MYTA_LINE_153p   = REG_D0C0; // fires on line 153, phase 004. clears on line 000, phase 004
  /*p21.POPU*/ RegQPN POPU_IN_VBLANKp  = REG_D0C0; // firce on line 144, phase 004. clears on line 000, phase 004 (the real line 000 not the stubby 000 @ 153)
  /*p21.SYGU*/ RegQP  SYGU_LINE_STROBE = REG_D0C0;
  /*p24.MEDA*/ RegQP  MEDA_VSYNC_OUTn  = REG_D0C0;
  /*p24.LUCA*/ RegQN  LUCA_LINE_EVEN   = REG_D0C0;
  /*p21.NAPO*/ RegQPN NAPO_FRAME_EVEN  = REG_D0C0;

  // FF45 - LYC
  /*p23.SYRY*/ Reg SYRY_LYC0n = REG_D0C0;
  /*p23.VUCE*/ Reg VUCE_LYC1n = REG_D0C0;
  /*p23.SEDY*/ Reg SEDY_LYC2n = REG_D0C0;
  /*p23.SALO*/ Reg SALO_LYC3n = REG_D0C0;
  /*p23.SOTA*/ Reg SOTA_LYC4n = REG_D0C0;
  /*p23.VAFA*/ Reg VAFA_LYC5n = REG_D0C0;
  /*p23.VEVO*/ Reg VEVO_LYC6n = REG_D0C0;
  /*p23.RAHA*/ Reg RAHA_LYC7n = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics