#include "../pages/Sch_Clocks.h"

using namespace Schematics;

void plot(int x, int y, bool v) {
    printf("\033[%d;%dH%c", y, x, v ? '#' : '.');
}

void printAt(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", y, x);
    vprintf(format, args);
    va_end(args);
}

void labels() {
  int line = 3;
  printAt(0, line++, "ROOT_AxCxExGx");
  printAt(0, line++, "MYVO_AxCxExGx");
  printAt(0, line++, "ZEME_AxCxExGx");
  printAt(0, line++, "XOTA_AxCxExGx");
  printAt(0, line++, "MOXE_AxCxExGx");
  printAt(0, line++, "MEHE_AxCxExGx");
  printAt(0, line++, "LAPE_AxCxExGx");
  line++;

  printAt(0, line++, "ARYS_xBxDxFxH");
  printAt(0, line++, "ANOS_xBxDxFxH");
  printAt(0, line++, "ALET_xBxDxFxH");
  printAt(0, line++, "TAVA_xBxDxFxH");
  line++;

  //printAt(0, line++, "MODE_PROD");
  printAt(0, line++, "PHAZ_xBCDExxx");
  printAt(0, line++, "PHAZ_xxCDEFxx");
  printAt(0, line++, "PHAZ_xxxDEFGx");
  printAt(0, line++, "PHAZ_xxxxEFGH");
  printAt(0, line++, "AFAS_xxxxxFGH");
  line++;

  //printAt(0, line++, "CLKIN_A");
  printAt(0, line++, "BOGA_AxCDEFGH");
  line++;

  printAt(0, line++, "CPUCLK_REQ ");
  printAt(0, line++, "UVYT_xBCDExxx");
  printAt(0, line++, "DOVA_xBCDExxx");
  printAt(0, line++, "BEDO_xBxxxxxx");
  printAt(0, line++, "BOWA_AxCDEFGH");
  printAt(0, line++, "MOPA_AxxxxFGH");
  line++;

  //printAt(0, line++, "VID_RESETn");
  printAt(0, line++, "SONO_ABCDxxxx");
  printAt(0, line++, "WOSU_xBCxxFGx");
  printAt(0, line++, "XUPY_ABxxEFxx");
  printAt(0, line++, "WUVU_xxCDxxGH");
  printAt(0, line++, "AWOH_xxCDxxGH");
  printAt(0, line++, "XOCE_AxxDExxH");
  printAt(0, line++, "VENA_xxxxEFGH");
  printAt(0, line++, "TALU_xxxxEFGH");
  line++;

  printAt(0, line++, "WUVU_xxCDxxGH");
  printAt(0, line++, "VENA_xxxxEFGH");
  printAt(0, line++, "WOSU_xBCxxFGx");
  line++;
}

void dump(int x, ClockSignals& clk) {
  int line = 3;
  plot(x, line++, clk.ROOT_AxCxExGx);
  plot(x, line++, clk.MYVO_AxCxExGx);
  plot(x, line++, clk.ZEME_AxCxExGx);
  plot(x, line++, clk.XOTA_AxCxExGx);
  plot(x, line++, clk.MOXE_AxCxExGx);
  plot(x, line++, clk.MEHE_AxCxExGx);
  plot(x, line++, clk.LAPE_AxCxExGx);
  line++;

  plot(x, line++, clk.ARYS_xBxDxFxH);
  plot(x, line++, clk.ANOS_xBxDxFxH);
  plot(x, line++, clk.ALET_xBxDxFxH);
  plot(x, line++, clk.TAVA_xBxDxFxH);
  line++;

  //plot(x, line++, clk.MODE_PROD);
  plot(x, line++, clk.PHAZ_xBCDExxx);
  plot(x, line++, clk.PHAZ_xxCDEFxx);
  plot(x, line++, clk.PHAZ_xxxDEFGx);
  plot(x, line++, clk.PHAZ_xxxxEFGH);
  plot(x, line++, clk.AFAS_xxxxxFGH);
  line++;

  //plot(x, line++, clk.CLKIN_A);
  plot(x, line++, clk.BOGA_AxCDEFGH);
  line++;

  plot(x, line++, clk.CPUCLK_REQ);
  plot(x, line++, clk.UVYT_xBCDExxx);
  plot(x, line++, clk.DOVA_xBCDExxx);
  plot(x, line++, clk.BEDO_xBxxxxxx);
  plot(x, line++, clk.BOWA_AxCDEFGH);
  plot(x, line++, clk.MOPA_AxxxxFGH);
  line++;

  //plot(x, line++, clk.VID_RESETn);
  plot(x, line++, clk.SONO_ABCDxxxx);
  plot(x, line++, clk.WOSU_xBCxxFGx);
  plot(x, line++, clk.XUPY_ABxxEFxx);
  plot(x, line++, clk.WUVU_xxCDxxGH);
  plot(x, line++, clk.AWOH_xxCDxxGH);
  plot(x, line++, clk.XOCE_AxxDExxH);
  plot(x, line++, clk.VENA_xxxxEFGH);
  plot(x, line++, clk.TALU_xxxxEFGH);
  line++;

  plot(x, line++, clk.WUVU_xxCDxxGH);
  plot(x, line++, clk.VENA_xxxxEFGH);
  plot(x, line++, clk.WOSU_xBCxxFGx);
  line++;
}

static int cursor = 20;

void sim(int phase, Clocks& clk_reg, bool CLK_GOOD, bool CPUCLK_REQ, bool MODE_PROD, bool VID_RESETn) {
  bool CLKIN = !(phase & 1);
  for (int j = 0; j < 8; j++) {
    Clocks prev_clk = clk_reg;
    ClockSignals clk_sig = ClockSignals::tick_slow(prev_clk, CLKIN, CLK_GOOD, CPUCLK_REQ);
    Clocks::tock_slow1(clk_sig, MODE_PROD, clk_reg);
    Clocks::tock_slow2(clk_sig, VID_RESETn, clk_reg);
  }
}

void TestClocks() {
  printf("\033[?6l");
  labels();

  Clocks clk_reg = {};

  clk_reg.reset();
  printAt(cursor, 1, "rst");

  for (int phase = 0; phase < 16; phase++) {
    bool CLKIN = !(phase & 1);
    sim(phase, clk_reg, true, true, true, true);
    ClockSignals sig = ClockSignals::tick_slow(clk_reg, CLKIN, true, true);
    dump(cursor++, sig);
  }

  /*
  bool CLK_GOOD = false;
  bool CPUCLK_REQ = false;
  bool MODE_PROD = false;
  bool VID_RESETn = 0;

  //----------

  CLK_GOOD = false;
  CPUCLK_REQ = false;
  MODE_PROD = false;
  VID_RESETn = 0;

  printAt(cursor, 1, "rst");
  sim(clk, CLK_GOOD, CPUCLK_REQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  MODE_PROD = true;
  printAt(cursor, 1, "prod");
  sim(clk, CLK_GOOD, CPUCLK_REQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  CLK_GOOD = true;
  printAt(cursor, 1, "clk_good");
  sim(clk, CLK_GOOD, CPUCLK_REQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  CPUCLK_REQ = true;
  printAt(cursor, 1, "clkreq");
  sim(clk, CLK_GOOD, CPUCLK_REQ, MODE_PROD, VID_RESETn);
  cursor++;

  //----------

  VID_RESETn = true;
  printAt(cursor, 1, "vid_rst");
  sim(clk, CLK_GOOD, CPUCLK_REQ, MODE_PROD, VID_RESETn);
  cursor++;
  */

  printAt(0, 50, "done");
}