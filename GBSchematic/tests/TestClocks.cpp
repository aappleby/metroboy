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

void dump(int x, ClockSignals1& clk_sig1, ClockSignals2& clk_sig2) {
  int line = 3;
  plot(x, line++, clk_sig1.ROOT_AxCxExGx);
  plot(x, line++, clk_sig1.MYVO_AxCxExGx);
  plot(x, line++, clk_sig1.ZEME_AxCxExGx);
  plot(x, line++, clk_sig1.XOTA_AxCxExGx);
  plot(x, line++, clk_sig1.MOXE_AxCxExGx);
  plot(x, line++, clk_sig1.MEHE_AxCxExGx);
  plot(x, line++, clk_sig1.LAPE_AxCxExGx);
  line++;

  plot(x, line++, clk_sig1.ARYS_xBxDxFxH);
  plot(x, line++, clk_sig1.ANOS_xBxDxFxH);
  plot(x, line++, clk_sig1.ALET_xBxDxFxH);
  plot(x, line++, clk_sig1.TAVA_xBxDxFxH);
  line++;

  //plot(x, line++, clk.MODE_PROD);
  plot(x, line++, clk_sig1.PHAZ_xBCDExxx);
  plot(x, line++, clk_sig1.PHAZ_xxCDEFxx);
  plot(x, line++, clk_sig1.PHAZ_xxxDEFGx);
  plot(x, line++, clk_sig1.PHAZ_xxxxEFGH);
  plot(x, line++, clk_sig1.AFAS_xxxxxFGH);
  line++;

  //plot(x, line++, clk.CLKIN_A);
  plot(x, line++, clk_sig1.BOGA_AxCDEFGH);
  line++;

  plot(x, line++, clk_sig1.CPUCLK_REQ);
  plot(x, line++, clk_sig1.UVYT_xBCDExxx);
  plot(x, line++, clk_sig1.DOVA_xBCDExxx);
  plot(x, line++, clk_sig1.BEDO_xBxxxxxx);
  plot(x, line++, clk_sig1.BOWA_AxCDEFGH);
  plot(x, line++, clk_sig1.MOPA_AxxxxFGH);
  line++;

  //plot(x, line++, clk.VID_RESETn);
  plot(x, line++, clk_sig2.SONO_ABCDxxxx);
  plot(x, line++, clk_sig2.WOSU_xBCxxFGx);
  plot(x, line++, clk_sig2.XUPY_ABxxEFxx);
  plot(x, line++, clk_sig2.WUVU_xxCDxxGH);
  plot(x, line++, clk_sig2.AWOH_xxCDxxGH);
  plot(x, line++, clk_sig2.XOCE_AxxDExxH);
  plot(x, line++, clk_sig2.VENA_xxxxEFGH);
  plot(x, line++, clk_sig2.TALU_xxxxEFGH);
  line++;

  plot(x, line++, clk_sig2.WUVU_xxCDxxGH);
  plot(x, line++, clk_sig2.VENA_xxxxEFGH);
  plot(x, line++, clk_sig2.WOSU_xBCxxFGx);
  line++;
}

static int cursor = 20;

void sim(int phase, Clocks& clk_reg, bool CLK_GOOD, bool CLK_REQ, bool MODE_PROD, bool VID_RESETn) {
  SystemSignals sys_sig = {
    .RST = false,
    .CLK_GOOD = CLK_GOOD,
    .MODE_PROD = MODE_PROD,
    .MODE_DBG1 = false,
    .MODE_DBG2 = false,
    .CLK_REQ = CLK_REQ,
    .ADDR_VALID = false,
    .BOOT_BIT = true,
    .LCDC_EN = true,
    .DIV_06n = true,
    .DIV_07n = true,
    .DIV_15 = false,
  };

  bool CLKIN = !(phase & 1);
  for (int j = 0; j < 8; j++) {
    Clocks prev_clk = clk_reg;
    ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_sig, prev_clk, CLKIN);
    ClockSignals2 clk_sig2 = ClockSignals2::tick_slow(prev_clk);
    Clocks::tock_slow1(sys_sig, clk_sig1, clk_reg);
    Clocks::tock_slow2(sys_sig, clk_sig1, clk_sig2, VID_RESETn, clk_reg);
  }
}

void TestClocks() {
  printf("\033[?6l");
  labels();

  Clocks clk_reg = {};

  clk_reg.reset();
  printAt(cursor, 1, "rst");

  SystemSignals sys_sig = {
    .RST = false,
    .CLK_GOOD = true,
    .MODE_PROD = true,
    .MODE_DBG1 = false,
    .MODE_DBG2 = false,
    .CLK_REQ = true,
    .ADDR_VALID = false,
    .BOOT_BIT = true,
    .LCDC_EN = true,
    .DIV_06n = true,
    .DIV_07n = true,
    .DIV_15 = false,
  };

  for (int phase = 0; phase < 16; phase++) {
    bool CLKIN = !(phase & 1);
    sim(phase, clk_reg, true, true, true, true);
    ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_sig, clk_reg, CLKIN);
    ClockSignals2 clk_sig2 = ClockSignals2::tick_slow(clk_reg);
    dump(cursor++, clk_sig1, clk_sig2);
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