#include "TestGB.h"

using namespace Schematics;

#if 0

void plot(int x, int y, bool v) {
  printf("\033[%d;%dH%c", y, x, v ? '#' : '.');
}

void plot2(int x, int y, int v) {
  printf("\033[%d;%dH%d", y, x, v);
}


void printAt(int x, int y, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\033[%d;%dH", y, x);
  vprintf(format, args);
  va_end(args);
}

void check_phase_name(int phase, const bool val, char* name) {
  bool expected = name[phase + 5] != 'x';
  if (val != expected) {
    printf("Phase of %s FAIL - phase %d, expected %d, actual %d\n", name, phase, expected, val);
    __debugbreak();
  }
}

//-----------------------------------------------------------------------------

void check_phase(SysSignals&    sys_sig,
                 ClkRegisters&  clk_reg1,
                 ClkSignals&    clk_sig1,
                 VclkRegisters& vclk_reg,
                 VclkSignals&   vclk_sig)
{
  int phase = sys_sig.phase;

  check_phase_name(phase, sys_sig.ATAL_AxCxExGx, "ATAL_AxCxExGx");
  check_phase_name(phase, sys_sig.AZOF_xBxDxFxH, "AZOF_xBxDxFxH");

  check_phase_name(phase, clk_reg1.PHAZ_ABCDxxxx, "PHAZ_ABCDxxxx");
  check_phase_name(phase, clk_reg1.PHAZ_xBCDExxx, "PHAZ_xBCDExxx");
  check_phase_name(phase, clk_reg1.PHAZ_xxCDEFxx, "PHAZ_xxCDEFxx");
  check_phase_name(phase, clk_reg1.PHAZ_xxxDEFGx, "PHAZ_xxxDEFGx");

  check_phase_name(phase, clk_sig1.ZEME_xBxDxFxH, "ZEME_xBxDxFxH");
  check_phase_name(phase, clk_sig1.ALET_AxCxExGx, "ALET_AxCxExGx");
  check_phase_name(phase, clk_sig1.AMUK_xBxDxFxH, "AMUK_xBxDxFxH");
  check_phase_name(phase, clk_sig1.AROV_xxCDEFxx, "AROV_xxCDEFxx");
  check_phase_name(phase, clk_sig1.AJAX_xxxxEFGH, "AJAX_xxxxEFGH");
  check_phase_name(phase, clk_sig1.AFAS_xxxxEFGx, "AFAS_xxxxEFGx");
  check_phase_name(phase, clk_sig1.BOGA_xBCDEFGH, "BOGA_xBCDEFGH");
  check_phase_name(phase, clk_sig1.DOVA_ABCDxxxx, "DOVA_ABCDxxxx");
  check_phase_name(phase, clk_sig1.UVYT_ABCDxxxx, "UVYT_ABCDxxxx");
  check_phase_name(phase, clk_sig1.MOPA_xxxxEFGH, "MOPA_xxxxEFGH");
  check_phase_name(phase, clk_sig1.BEDO_Axxxxxxx, "BEDO_Axxxxxxx");
  check_phase_name(phase, clk_sig1.BOWA_xBCDEFGH, "BOWA_xBCDEFGH");
  check_phase_name(phase, clk_sig1.BORY_AxxxxxGH, "BORY_AxxxxxGH");

  if (sys_sig.LCDC_EN) {
    check_phase_name(phase, vclk_reg.WUVU_AxxDExxH, "WUVU_AxxDExxH");
    check_phase_name(phase, vclk_reg.VENA_xBCDExxx, "VENA_xBCDExxx");
    check_phase_name(phase, vclk_reg.WOSU_xxCDxxGH, "WOSU_xxCDxxGH");

    check_phase_name(phase, vclk_sig.XUPY_xBCxxFGx, "XUPY_xBCxxFGx");
    check_phase_name(phase, vclk_sig.AWOH_AxxDExxH, "AWOH_AxxDExxH");
    check_phase_name(phase, vclk_sig.TALU_xBCDExxx, "TALU_xBCDExxx");
    check_phase_name(phase, vclk_sig.SONO_AxxxxFGH, "SONO_AxxxxFGH");
    check_phase_name(phase, vclk_sig.XOCE_ABxxEFxx, "XOCE_ABxxEFxx");
  }
}

//-----------------------------------------------------------------------------

void labels() {
  int line = 3;
  printAt(0, line++, "PHASE");
  printAt(0, line++, "ATAL_AxCxExGx");
  printAt(0, line++, "AZOF_xBxDxFxH");
  line++;

  printAt(0, line++, "PHAZ_ABCDxxxx");
  printAt(0, line++, "PHAZ_xBCDExxx");
  printAt(0, line++, "PHAZ_xxCDEFxx");
  printAt(0, line++, "PHAZ_xxxDEFGx");
  line++;

  printAt(0, line++, "ZEME_xBxDxFxH");
  printAt(0, line++, "ALET_AxCxExGx");
  printAt(0, line++, "AMUK_xBxDxFxH");
  printAt(0, line++, "AROV_xxCDEFxx");
  printAt(0, line++, "AJAX_xxxxEFGH");
  printAt(0, line++, "AFAS_xxxxEFGx");
  printAt(0, line++, "BOGA_xBCDEFGH");
  printAt(0, line++, "DOVA_ABCDxxxx");
  printAt(0, line++, "UVYT_ABCDxxxx");
  printAt(0, line++, "MOPA_xxxxEFGH");
  printAt(0, line++, "BEDO_Axxxxxxx");
  printAt(0, line++, "BOWA_xBCDEFGH");
  printAt(0, line++, "BORY_AxxxxxGH");
  line++;

  printAt(0, line++, "WUVU_AxxDExxH");
  printAt(0, line++, "VENA_xBCDExxx");
  printAt(0, line++, "WOSU_xxCDxxGH");
  line++;

  printAt(0, line++, "XUPY_xBCxxFGx");
  printAt(0, line++, "AWOH_AxxDExxH");
  printAt(0, line++, "TALU_xBCDExxx");
  printAt(0, line++, "SONO_AxxxxFGH");
  printAt(0, line++, "XOCE_ABxxEFxx");
  printAt(0, line++, "XYSO_ABCxDEFx");
  line++;
}

#endif