#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void tick(const SchematicTop& top);
  void tock(int phase, const SchematicTop& top, CpuBus& cpu_bus);

  /*p28.BYHA*/ wire LcdRegisters::BYHA_VID_LINE_TRIGn() const {
    /*p28.ABAF*/ wire _ABAF_LINE_END_Bn = not(_CATU_VID_LINE_ENDp.q());
    return and (or (_ANEL_VID_LINE_ENDp.q(), _ABAF_LINE_END_Bn), _ABEZ_VID_RSTn);
  }

  // -> lcd, window
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIGp() const { return not(BYHA_VID_LINE_TRIGn()); }

  // -> interrupts, ppu
  /*p21.PARU*/ wire PARU_VBLANKp_d4()     const { return not(_POPU_IN_VBLANKp.qn()); }

  // -> lcd
  /*p21.PURE*/ wire PURE_LINE_ENDn()   const { return not(_RUTU_LINE_ENDp.q()); }

  // -> interrupts, lcd
  /*p21.SELA*/ wire SELA_LINE_ENDp()    const { return not(PURE_LINE_ENDn()); }

  // -> sprite scanner
  /*p29.CATU*/ wire CATU_VID_LINE_ENDp()       const { return _CATU_VID_LINE_ENDp.q(); }

  void dump(Dumper& d) {
    d("----------   LCD    ----------\n");
    d("LCD X               %03d\n", pack(0,           TYRY_X6.q(), TAHA_X5.q(), SUDE_X4.q(), TELU_X3.q(), VYZO_X2.q(), TYPO_X1.q(), SAXO_X0.q()));
    d("LCD Y               %03d\n", pack(LAFO_Y7.q(), MATO_Y6.q(), LEMA_Y5.q(), LOVU_Y4.q(), LYDO_Y3.q(), LEXA_Y2.q(), MYRO_Y1.q(), MUWY_Y0.q()));
    d("LYC                 %03d\n", pack(_SYRY_LYC0.q(), _VUCE_LYC1.q(), _SEDY_LYC2.q(), _SALO_LYC3.q(), _SOTA_LYC4.q(), _VAFA_LYC5.q(), _VEVO_LYC6.q(), _RAHA_LYC7.q()));

    d("RUTU_LINE_ENDp      %c\n", _RUTU_LINE_ENDp     .c());
    d("CATU_VID_LINE_ENDp  %c\n", _CATU_VID_LINE_ENDp .c());
    d("NYPE_LINE_STARTp    %c\n", _NYPE_LINE_STARTp   .c());
    d("ANEL_VID_LINE_ENDp  %c\n", _ANEL_VID_LINE_ENDp .c());
    d("MYTA_LINE_153p      %c\n", _MYTA_LINE_153p     .c());
    d("POPU_IN_VBLANKp     %c\n", _POPU_IN_VBLANKp    .c());
    d("SYGU_LINE_STROBE    %c\n", _SYGU_LINE_STROBE   .c());
    d("MEDA_VSYNC_OUTn     %c\n", _MEDA_VSYNC_OUTn    .c());
    d("LUCA_LINE_EVEN      %c\n", _LUCA_LINE_EVEN     .c());
    d("NAPO_FRAME_EVEN     %c\n", _NAPO_FRAME_EVEN    .c());
    d("ROPO_LY_MATCH_SYNCp %c\n", _ROPO_LY_MATCH_SYNCp.c());
    

    d("LCD_PIN_CPG         %c\n", _LCD_PIN_CPG.c());
    d("LCD_PIN_CPL         %c\n", _LCD_PIN_CPL.c());
    d("LCD_PIN_FR          %c\n", _LCD_PIN_FR .c());
    d("LCD_PIN_S           %c\n", _LCD_PIN_S  .c());
  }

  // -> interrupts, lcd, ppu
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return _ROPO_LY_MATCH_SYNCp.q(); }
  
  // -> sprite store
  /*p21.SAXO*/ Reg SAXO_X0 = REG_D0C0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg TYPO_X1 = REG_D0C0;
  /*p21.VYZO*/ Reg VYZO_X2 = REG_D0C0;
  /*p21.TELU*/ Reg TELU_X3 = REG_D0C0;
  /*p21.SUDE*/ Reg SUDE_X4 = REG_D0C0;
  /*p21.TAHA*/ Reg TAHA_X5 = REG_D0C0;
  /*p21.TYRY*/ Reg TYRY_X6 = REG_D0C0;

  // -> pix pipe, sprite scanner, vram bus
  /*p21.MUWY*/ Reg MUWY_Y0 = REG_D0C0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg MYRO_Y1 = REG_D0C0;
  /*p21.LEXA*/ Reg LEXA_Y2 = REG_D0C0;
  /*p21.LYDO*/ Reg LYDO_Y3 = REG_D0C0;
  /*p21.LOVU*/ Reg LOVU_Y4 = REG_D0C0;
  /*p21.LEMA*/ Reg LEMA_Y5 = REG_D0C0;
  /*p21.MATO*/ Reg MATO_Y6 = REG_D0C0;
  /*p21.LAFO*/ Reg LAFO_Y7 = REG_D0C0;

private:

  Sig _ABEZ_VID_RSTn;

  /*p21.RUTU*/ Reg _RUTU_LINE_ENDp     = REG_D0C0;         // p909+8
  /*p29.CATU*/ Reg _CATU_VID_LINE_ENDp = REG_D0C0;     // p001+8
  /*p21.NYPE*/ Reg _NYPE_LINE_STARTp   = REG_D0C0;       // p001+8
  /*p28.ANEL*/ Reg _ANEL_VID_LINE_ENDp = REG_D0C0;     // p003+8
  /*p21.MYTA*/ Reg _MYTA_LINE_153p     = REG_D0C0;         // p153:001 - p000:001
  /*p21.POPU*/ Reg _POPU_IN_VBLANKp    = REG_D0C0;        // p144:001 - p000:001
  /*p21.SYGU*/ Reg _SYGU_LINE_STROBE   = REG_D0C0;
  /*p24.MEDA*/ Reg _MEDA_VSYNC_OUTn    = REG_D0C0;
  /*p24.LUCA*/ Reg _LUCA_LINE_EVEN     = REG_D0C0;
  /*p21.NAPO*/ Reg _NAPO_FRAME_EVEN    = REG_D0C0;
  /*p21.ROPO*/ Reg _ROPO_LY_MATCH_SYNCp = REG_D0C0;

  // FF45 - LYC
  /*p23.SYRY*/ Reg _SYRY_LYC0 = REG_D0C0;
  /*p23.VUCE*/ Reg _VUCE_LYC1 = REG_D0C0;
  /*p23.SEDY*/ Reg _SEDY_LYC2 = REG_D0C0;
  /*p23.SALO*/ Reg _SALO_LYC3 = REG_D0C0;
  /*p23.SOTA*/ Reg _SOTA_LYC4 = REG_D0C0;
  /*p23.VAFA*/ Reg _VAFA_LYC5 = REG_D0C0;
  /*p23.VEVO*/ Reg _VEVO_LYC6 = REG_D0C0;
  /*p23.RAHA*/ Reg _RAHA_LYC7 = REG_D0C0;

  Tri _LCD_PIN_CPG = TRI_HZNP; // PIN_52 
  Tri _LCD_PIN_CPL = TRI_HZNP; // PIN_55 
  Tri _LCD_PIN_FR  = TRI_HZNP; // PIN_56 
  Tri _LCD_PIN_S   = TRI_HZNP; // PIN_57 
};

//-----------------------------------------------------------------------------

}; // namespace Schematics