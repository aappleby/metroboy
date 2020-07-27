#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

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
    
    d("SYRY_LYC0           %c\n", _SYRY_LYC0.c());
    d("VUCE_LYC1           %c\n", _VUCE_LYC1.c());
    d("SEDY_LYC2           %c\n", _SEDY_LYC2.c());
    d("SALO_LYC3           %c\n", _SALO_LYC3.c());
    d("SOTA_LYC4           %c\n", _SOTA_LYC4.c());
    d("VAFA_LYC5           %c\n", _VAFA_LYC5.c());
    d("VEVO_LYC6           %c\n", _VEVO_LYC6.c());
    d("RAHA_LYC7           %c\n", _RAHA_LYC7.c());
                           
    d("LCD_PIN_CPG         %c\n", _LCD_PIN_CPG.c());
    d("LCD_PIN_CPL         %c\n", _LCD_PIN_CPL.c());
    d("LCD_PIN_FR          %c\n", _LCD_PIN_FR .c());
    d("LCD_PIN_S           %c\n", _LCD_PIN_S  .c());
  }

  // -> interrupts, lcd, ppu
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return _ROPO_LY_MATCH_SYNCp.q(); }
  
  // -> sprite store
  /*p21.SAXO*/ Reg2 SAXO_X0 = Reg2::D0C0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg2 TYPO_X1 = Reg2::D0C0;
  /*p21.VYZO*/ Reg2 VYZO_X2 = Reg2::D0C0;
  /*p21.TELU*/ Reg2 TELU_X3 = Reg2::D0C0;
  /*p21.SUDE*/ Reg2 SUDE_X4 = Reg2::D0C0;
  /*p21.TAHA*/ Reg2 TAHA_X5 = Reg2::D0C0;
  /*p21.TYRY*/ Reg2 TYRY_X6 = Reg2::D0C0;

  // -> pix pipe, sprite scanner, vram bus
  /*p21.MUWY*/ Reg2 MUWY_Y0 = Reg2::D0C0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg2 MYRO_Y1 = Reg2::D0C0;
  /*p21.LEXA*/ Reg2 LEXA_Y2 = Reg2::D0C0;
  /*p21.LYDO*/ Reg2 LYDO_Y3 = Reg2::D0C0;
  /*p21.LOVU*/ Reg2 LOVU_Y4 = Reg2::D0C0;
  /*p21.LEMA*/ Reg2 LEMA_Y5 = Reg2::D0C0;
  /*p21.MATO*/ Reg2 MATO_Y6 = Reg2::D0C0;
  /*p21.LAFO*/ Reg2 LAFO_Y7 = Reg2::D0C0;

private:

  Sig2 _ABEZ_VID_RSTn;

  /*p21.RUTU*/ Reg2 _RUTU_LINE_ENDp     = Reg2::D0C0;         // p909+8
  /*p29.CATU*/ Reg2 _CATU_VID_LINE_ENDp = Reg2::D0C0;     // p001+8
  /*p21.NYPE*/ Reg2 _NYPE_LINE_STARTp   = Reg2::D0C0;       // p001+8
  /*p28.ANEL*/ Reg2 _ANEL_VID_LINE_ENDp = Reg2::D0C0;     // p003+8
  /*p21.MYTA*/ Reg2 _MYTA_LINE_153p     = Reg2::D0C0;         // p153:001 - p000:001
  /*p21.POPU*/ Reg2 _POPU_IN_VBLANKp    = Reg2::D0C0;        // p144:001 - p000:001
  /*p21.SYGU*/ Reg2 _SYGU_LINE_STROBE   = Reg2::D0C0;
  /*p24.MEDA*/ Reg2 _MEDA_VSYNC_OUTn    = Reg2::D0C0;
  /*p24.LUCA*/ Reg2 _LUCA_LINE_EVEN     = Reg2::D0C0;
  /*p21.NAPO*/ Reg2 _NAPO_FRAME_EVEN    = Reg2::D0C0;
  /*p21.ROPO*/ Reg2 _ROPO_LY_MATCH_SYNCp = Reg2::D0C0;

  // FF45 - LYC
  /*p23.SYRY*/ Reg2 _SYRY_LYC0 = Reg2::D0C0;
  /*p23.VUCE*/ Reg2 _VUCE_LYC1 = Reg2::D0C0;
  /*p23.SEDY*/ Reg2 _SEDY_LYC2 = Reg2::D0C0;
  /*p23.SALO*/ Reg2 _SALO_LYC3 = Reg2::D0C0;
  /*p23.SOTA*/ Reg2 _SOTA_LYC4 = Reg2::D0C0;
  /*p23.VAFA*/ Reg2 _VAFA_LYC5 = Reg2::D0C0;
  /*p23.VEVO*/ Reg2 _VEVO_LYC6 = Reg2::D0C0;
  /*p23.RAHA*/ Reg2 _RAHA_LYC7 = Reg2::D0C0;

  Pin2 _LCD_PIN_CPG = Pin2::HIZ_NP; // PIN_52 
  Pin2 _LCD_PIN_CPL = Pin2::HIZ_NP; // PIN_55 
  Pin2 _LCD_PIN_FR  = Pin2::HIZ_NP; // PIN_56 
  Pin2 _LCD_PIN_S   = Pin2::HIZ_NP; // PIN_57 
};

//-----------------------------------------------------------------------------

}; // namespace Schematics