#include "TestGB.h"

using namespace Schematics;

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

#pragma warning(disable:4100)

void check_phase(SystemRegisters& sys_reg,
                 ClockRegisters1& clk_reg1,
                 ClockSignals1&   clk_sig1,
                 VideoClocks&     vid_clk_reg,
                 ClockSignals2&   vid_clk_sig)
{
  int phase = sys_reg.phase();

  check_phase_name(phase, sys_reg.ATAL_AxCxExGx, "ATAL_AxCxExGx");
  check_phase_name(phase, sys_reg.AZOF_xBxDxFxH, "AZOF_xBxDxFxH");

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

  if (sys_reg.LCDC_EN) {
    check_phase_name(phase, vid_clk_reg.WUVU_AxxDExxH, "WUVU_AxxDExxH");
    check_phase_name(phase, vid_clk_reg.VENA_xBCDExxx, "VENA_xBCDExxx");
    check_phase_name(phase, vid_clk_reg.WOSU_xxCDxxGH, "WOSU_xxCDxxGH");

    check_phase_name(phase, vid_clk_sig.XUPY_xBCxxFGx, "XUPY_xBCxxFGx");
    check_phase_name(phase, vid_clk_sig.AWOH_AxxDExxH, "AWOH_AxxDExxH");
    check_phase_name(phase, vid_clk_sig.TALU_xBCDExxx, "TALU_xBCDExxx");
    check_phase_name(phase, vid_clk_sig.SONO_AxxxxFGH, "SONO_AxxxxFGH");
    check_phase_name(phase, vid_clk_sig.XOCE_ABxxEFxx, "XOCE_ABxxEFxx");
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

//-----------------------------------------------------------------------------

void dump(int x,
          SystemRegisters& sys_reg,
          ClockRegisters1& clk_reg1,
          ClockSignals1& clk_sig1,
          VideoClocks& vid_clk_reg,
          ClockSignals2& vid_clk_sig) {
  int line = 3;
  plot2(x, line++, sys_reg.phase());
  plot(x, line++, sys_reg.ATAL_AxCxExGx);
  plot(x, line++, sys_reg.AZOF_xBxDxFxH);
  line++;

  plot(x, line++, clk_reg1.PHAZ_ABCDxxxx);
  plot(x, line++, clk_reg1.PHAZ_xBCDExxx);
  plot(x, line++, clk_reg1.PHAZ_xxCDEFxx);
  plot(x, line++, clk_reg1.PHAZ_xxxDEFGx);
  line++;

  plot(x, line++, clk_sig1.ZEME_xBxDxFxH);
  plot(x, line++, clk_sig1.ALET_AxCxExGx);
  plot(x, line++, clk_sig1.AMUK_xBxDxFxH);
  plot(x, line++, clk_sig1.AROV_xxCDEFxx);
  plot(x, line++, clk_sig1.AJAX_xxxxEFGH);
  plot(x, line++, clk_sig1.AFAS_xxxxEFGx);
  plot(x, line++, clk_sig1.BOGA_xBCDEFGH);
  plot(x, line++, clk_sig1.DOVA_ABCDxxxx);
  plot(x, line++, clk_sig1.UVYT_ABCDxxxx);
  plot(x, line++, clk_sig1.MOPA_xxxxEFGH);
  plot(x, line++, clk_sig1.BEDO_Axxxxxxx);
  plot(x, line++, clk_sig1.BOWA_xBCDEFGH);
  plot(x, line++, clk_sig1.BORY_AxxxxxGH);
  line++;

  plot(x, line++, vid_clk_reg.WUVU_AxxDExxH);
  plot(x, line++, vid_clk_reg.VENA_xBCDExxx);
  plot(x, line++, vid_clk_reg.WOSU_xxCDxxGH);
  line++;

  plot(x, line++, vid_clk_sig.XUPY_xBCxxFGx);
  plot(x, line++, vid_clk_sig.AWOH_AxxDExxH);
  plot(x, line++, vid_clk_sig.TALU_xBCDExxx);
  plot(x, line++, vid_clk_sig.SONO_AxxxxFGH);
  plot(x, line++, vid_clk_sig.XOCE_ABxxEFxx);
  plot(x, line++, vid_clk_sig.XYSO_ABCxDEFx);
  line++;
}

//-----------------------------------------------------------------------------

void test_clock_phases() {
  printf("test_clock_phases: ");
  TestGB gb;
  gb.reset();

  for (int phase = 0; phase < 48; phase++) {
    gb.sim(1);

    ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(gb.sys_reg, gb.clk_reg1);
    ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(gb.sys_reg, clk_sig1, gb.rst_reg);
    VideoResets vid_rst = VideoResets::tick_slow(gb.sys_reg, rst_sig1);
    ClockSignals2 vid_clk_sig = ClockSignals2::tick_slow(gb.sys_reg, vid_rst, gb.vid_clk_reg);

    check_phase(gb.sys_reg,
                gb.clk_reg1, clk_sig1,
                gb.vid_clk_reg, vid_clk_sig);
  }
  printf("pass\n");
}

//-----------------------------------------------------------------------------

static int cursor = 20;

void TestClocks() {
  printf("TestClocks: ");
  printf("\033[?6l");
  labels();

  TestGB gb;
  //gb.reset();
  gb.pwron();

  // CLK_GOOD - BOGA_xBCDEFGH

  // The video clock gen WUVU/VENA/WOSU can be in different phases depending on when LCDC_EN goes high.

  gb.sys_reg.MODE_PROD = true;
  gb.sys_reg.RST = false;
  gb.sys_reg.CLK_GOOD = true;
  gb.sys_reg.CLK_REQ = true;

  // setting LCDC_EN on phase 6 or 7 makes phase match
  // but it looks like cpu writes take effect on phase 4?

  gb.sys_reg.LCDC_EN = false;
  gb.sim(16);

  for (int phase = 0; phase < 48; phase++) {
    gb.sim(1);

    ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(gb.sys_reg, gb.clk_reg1);
    ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(gb.sys_reg, clk_sig1, gb.rst_reg);
    VideoResets vid_rst = VideoResets::tick_slow(gb.sys_reg, rst_sig1);
    ClockSignals2 vid_clk_sig = ClockSignals2::tick_slow(gb.sys_reg, vid_rst, gb.vid_clk_reg);

    check_phase(gb.sys_reg,
                gb.clk_reg1, clk_sig1,
                gb.vid_clk_reg, vid_clk_sig);

    dump(cursor,
         gb.sys_reg,
         gb.clk_reg1, clk_sig1,
         gb.vid_clk_reg, vid_clk_sig);
    cursor++;
  }

  /*
  bool CLK_GOOD = false;
  bool BUTY_CLKREQ = false;
  bool MODE_PROD = false;
  bool VID_RESETn = 0;

  //----------

  CLK_GOOD = false;
  BUTY_CLKREQ = false;
  MODE_PROD = false;
  VID_RESETn = 0;

  printAt(cursor, 1, "rst");
  sim(clk, CLK_GOOD, BUTY_CLKREQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  MODE_PROD = true;
  printAt(cursor, 1, "prod");
  sim(clk, CLK_GOOD, BUTY_CLKREQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  CLK_GOOD = true;
  printAt(cursor, 1, "clk_good");
  sim(clk, CLK_GOOD, BUTY_CLKREQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  BUTY_CLKREQ = true;
  printAt(cursor, 1, "clkreq");
  sim(clk, CLK_GOOD, BUTY_CLKREQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  VID_RESETn = true;
  printAt(cursor, 1, "vid_rst");
  sim(clk, CLK_GOOD, BUTY_CLKREQ, MODE_PROD, VID_RESETn);
  cursor++;
  */

  printf("pass\n");
}

//-----------------------------------------------------------------------------