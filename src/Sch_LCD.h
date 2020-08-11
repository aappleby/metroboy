#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {
  void dump(Dumper& d, const SchematicTop& top) const;
  void tick(const SchematicTop& top);
  void tock(int phase, SchematicTop& top, CpuBus& cpu_bus);

  uint8_t get_x() const {
    return (uint8_t)pack_p(SAXO_X0p.qp(), TYPO_X1p.qp(), VYZO_X2p.qp(), TELU_X3p.qp(), SUDE_X4p.qp(), TAHA_X5p.qp(), TYRY_X6p.qp(), 0);
  }

  uint8_t get_y() const {
    return (uint8_t)pack_p(MUWY_Y0p.qp(), MYRO_Y1p.qp(), LEXA_Y2p.qp(), LYDO_Y3p.qp(), LOVU_Y4p.qp(), LEMA_Y5p.qp(), MATO_Y6p.qp(), LAFO_Y7p.qp());
  }

  uint8_t get_lyc() const {
    return (uint8_t)pack_p(_SYRY_LYC0.qp(), _VUCE_LYC1.qp(), _SEDY_LYC2.qp(), _SALO_LYC3.qp(),
                _SOTA_LYC4.qp(), _VAFA_LYC5.qp(), _VEVO_LYC6.qp(), _RAHA_LYC7.qp());
  }

  // fires on P910 and P911
  wire BYHA_VID_LINE_END_TRIGn() const {
    /*p28.ABAF*/ wire _ABAF_LINE_P910n = not1(_CATU_LINE_P910.qp());
    
    // so if this is or_and, BYHA should go low on 910 and 911
    /*p28.BYHA*/ wire BYHA_VID_LINE_END_TRIGn = or_and3(_ANEL_LINE_P000.qp(), _ABAF_LINE_P910n, _ABEZ_VID_RSTn);
    
    return BYHA_VID_LINE_END_TRIGn;
  }

  // fires on P910 and P911
  wire ATEJ_VID_LINE_END_TRIGp() const {
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_END_TRIGp = not1(BYHA_VID_LINE_END_TRIGn());
    return ATEJ_VID_LINE_END_TRIGp;
  }

  // -> interrupts, ppu
  /*p21.PARU*/ wire PARU_VBLANKp_d4()     const { return not1(_POPU_IN_VBLANKp.qn()); }

  // -> interrupts, lcd, ppu
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return _ROPO_LY_MATCH_SYNCp.qp(); }
  
  // -> sprite store

  // Stays at 0 for 12 phases, 113 for 4 phases, all others for 8 phases
  /*p21.SAXO*/ RegQPN SAXO_X0p = REG_D0C0; // increments at line pase 012 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.TYPO*/ RegQPN TYPO_X1p = REG_D0C0;
  /*p21.VYZO*/ RegQPN VYZO_X2p = REG_D0C0;
  /*p21.TELU*/ RegQPN TELU_X3p = REG_D0C0;
  /*p21.SUDE*/ RegQPN SUDE_X4p = REG_D0C0;
  /*p21.TAHA*/ RegQPN TAHA_X5p = REG_D0C0;
  /*p21.TYRY*/ RegQPN TYRY_X6p = REG_D0C0;

  // -> pix pipe, sprite scanner, vram bus
  // Increments when RUTU goes high.
  /*p21.MUWY*/ RegQPN MUWY_Y0p = REG_D0C0;
  /*p21.MYRO*/ RegQPN MYRO_Y1p = REG_D0C0;
  /*p21.LEXA*/ RegQPN LEXA_Y2p = REG_D0C0;
  /*p21.LYDO*/ RegQPN LYDO_Y3p = REG_D0C0;
  /*p21.LOVU*/ RegQPN LOVU_Y4p = REG_D0C0;
  /*p21.LEMA*/ RegQPN LEMA_Y5p = REG_D0C0;
  /*p21.MATO*/ RegQPN MATO_Y6p = REG_D0C0;
  /*p21.LAFO*/ RegQPN LAFO_Y7p = REG_D0C0;

//private:

  Sig _ABEZ_VID_RSTn;

  /*p21.NYPE*/ RegQPN _NYPE_LINE_P000 = REG_D0C0; // fires on line phase 000, high for 8 phases
  /*p28.ANEL*/ RegQP  _ANEL_LINE_P000 = REG_D0C0; // fires on line phase 000, high for 8 phases
  /*p21.RUTU*/ RegQP  _RUTU_LINE_P908 = REG_D0C0; // fires on line phase 908, high for 8 phases
  /*p29.CATU*/ RegQP  _CATU_LINE_P910 = REG_D0C0; // fires on line phase 910, high for 8 phases

  /*p21.MYTA*/ RegQP  _MYTA_LINE_153p      = REG_D0C0; // fires on line 153, phase 004. clears on line 000, phase 004
  /*p21.POPU*/ RegQPN _POPU_IN_VBLANKp     = REG_D0C0; // firce on line 144, phase 004. clears on line 000, phase 004 (the real line 000 not the stubby 000 @ 153)
  /*p21.SYGU*/ RegQP  _SYGU_LINE_STROBE    = REG_D0C0;
  /*p24.MEDA*/ RegQP  _MEDA_VSYNC_OUTn     = REG_D0C0;
  /*p24.LUCA*/ RegQN  _LUCA_LINE_EVEN      = REG_D0C0;
  /*p21.NAPO*/ RegQPN _NAPO_FRAME_EVEN     = REG_D0C0;
  /*p21.ROPO*/ RegQP  _ROPO_LY_MATCH_SYNCp = REG_D0C0;

  // FF45 - LYC
  /*p23.SYRY*/ Reg _SYRY_LYC0 = REG_D0C0;
  /*p23.VUCE*/ Reg _VUCE_LYC1 = REG_D0C0;
  /*p23.SEDY*/ Reg _SEDY_LYC2 = REG_D0C0;
  /*p23.SALO*/ Reg _SALO_LYC3 = REG_D0C0;
  /*p23.SOTA*/ Reg _SOTA_LYC4 = REG_D0C0;
  /*p23.VAFA*/ Reg _VAFA_LYC5 = REG_D0C0;
  /*p23.VEVO*/ Reg _VEVO_LYC6 = REG_D0C0;
  /*p23.RAHA*/ Reg _RAHA_LYC7 = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics