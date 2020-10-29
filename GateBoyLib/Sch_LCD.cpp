#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

void LcdRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== LCD =====\001\n");
  d("PIX COUNT : %03d\n", top.pix_pipe.get_pix_count());
  d("LCD X     : %03d\n", get_lx());
  d("LCD Y     : %03d\n", get_ly());
  d("LYC       : %03d\n", get_lyc());
  d("\n");

  d("PIN_LCD_CLOCK   : "); top.PIN_LCD_CLOCK.dump(d); d("\n");
  d("PIN_LCD_HSYNC   : "); top.PIN_LCD_HSYNC.dump(d); d("\n");
  d("PIN_LCD_VSYNC   : "); top.PIN_LCD_VSYNC.dump(d); d("\n");
  d("PIN_LCD_DATA1   : "); top.PIN_LCD_DATA1.dump(d); d("\n");
  d("PIN_LCD_DATA0   : "); top.PIN_LCD_DATA0.dump(d); d("\n");
  d("PIN_LCD_CNTRL   : "); top.PIN_LCD_CNTRL.dump(d); d("\n");
  d("PIN_LCD_DATALCH : "); top.PIN_LCD_LATCH.dump(d); d("\n");
  d("PIN_LCD_ALTSIGL   : "); top.PIN_LCD_FLIPS.dump(d); d("\n");
  d("\n");

  d("CATU_LINE_P000      %c\n", CATU_LINE_P000.c());
  d("NYPE_LINE_P002      %c\n", NYPE_LINE_P002.c());
  d("ANEL_LINE_P002      %c\n", ANEL_LINE_P002.c());
  d("RUTU_LINE_P910      %c\n", RUTU_LINE_P910.c());
  d("ATEJ_LINE_TRIGp     %c\n", ATEJ_LINE_TRIGp.c());
  d("MYTA_LINE_153p      %c\n", MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", POPU_IN_VBLANKp    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", ROPO_LY_MATCH_SYNCp.c());
  d("PARU_VBLANKp_d4     %c\n", PARU_VBLANKp_d4.c());
  d("VYPU_INT_VBLANKp    %c\n", VYPU_INT_VBLANKp.c());
  d("\n");
}

//------------------------------------------------------------------------------
