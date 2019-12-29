#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_Resets.h"

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
  line++;

  printAt(0, line++, "PHAZ_ABCDxxxx");
  printAt(0, line++, "PHAZ_xBCDExxx");
  printAt(0, line++, "PHAZ_xxCDEFxx");
  printAt(0, line++, "PHAZ_xxxDEFGx");
  printAt(0, line++, "AFEP_AxxxxFGH");
  printAt(0, line++, "ATYP_ABCDxxxx");
  printAt(0, line++, "ADAR_ABCxxxxH");
  printAt(0, line++, "AROV_xxCDEFxx");
  printAt(0, line++, "AFAS_xxxxxFGH");
  line++;

  printAt(0, line++, "NULE_xxxxEFGH");
  printAt(0, line++, "BYRY_ABCDxxxx");
  printAt(0, line++, "BUDE_xxxxEFGH");
  printAt(0, line++, "DOVA_ABCDxxxx");
  printAt(0, line++, "UVYT_ABCDxxxx");
  printAt(0, line++, "BEKO_ABCDxxxx");
  printAt(0, line++, "MOPA_xxxxEFGH");
  line++;

  printAt(0, line++, "BAPY_xxxxxxGH");
  printAt(0, line++, "BERU_ABCDEFxx");
  printAt(0, line++, "BUFA_xxxxxxGH");
  printAt(0, line++, "BOLO_ABCDEFxx");
  printAt(0, line++, "BEJA_xxxxEFGH");
  printAt(0, line++, "BANE_ABCDxxxx");
  printAt(0, line++, "BELO_xxxxEFGH");
  printAt(0, line++, "BAZE_ABCDxxxx");
  line++;

  printAt(0, line++, "BUTO_xBCDEFGH");
  printAt(0, line++, "BELE_Axxxxxxx");
  printAt(0, line++, "BYJU_xBCDEFGH");
  printAt(0, line++, "BALY_Axxxxxxx");
  printAt(0, line++, "BOGA_xBCDEFGH");
  printAt(0, line++, "BUVU_Axxxxxxx");
  printAt(0, line++, "BYXO_xBCDEFGH");
  printAt(0, line++, "BEDO_Axxxxxxx");
  printAt(0, line++, "BOWA_xBCDEFGH");
  line++;

  printAt(0, line++, "WUVU_AxxDExxH");
  printAt(0, line++, "XUPY_xBCxxFGx");
  printAt(0, line++, "AWOH_AxxDExxH");
  printAt(0, line++, "VENA_xBCDExxx");
  printAt(0, line++, "TALU_xBCDExxx");
  printAt(0, line++, "SONO_AxxxxFGH");
  printAt(0, line++, "WOSU_xxCDxxGH");
  printAt(0, line++, "XOCE_ABxxEFxx");
  line++;
}

void dump(int x, ClockSignals1& clk_sig1, ClockSignals2& clk_sig2) {
  int line = 3;
  plot(x, line++, clk_sig1.ROOT_AxCxExGx);
  line++;

  plot(x, line++, clk_sig1.PHAZ_ABCDxxxx);
  plot(x, line++, clk_sig1.PHAZ_xBCDExxx);
  plot(x, line++, clk_sig1.PHAZ_xxCDEFxx);
  plot(x, line++, clk_sig1.PHAZ_xxxDEFGx);
  plot(x, line++, clk_sig1.AFEP_AxxxxFGH);
  plot(x, line++, clk_sig1.ATYP_ABCDxxxx);
  plot(x, line++, clk_sig1.ADAR_ABCxxxxH);
  plot(x, line++, clk_sig1.AROV_xxCDEFxx);
  plot(x, line++, clk_sig1.AFAS_xxxxxFGH);
  line++;

  plot(x, line++, clk_sig1.NULE_xxxxEFGH);
  plot(x, line++, clk_sig1.BYRY_ABCDxxxx);
  plot(x, line++, clk_sig1.BUDE_xxxxEFGH);
  plot(x, line++, clk_sig1.DOVA_ABCDxxxx);
  plot(x, line++, clk_sig1.UVYT_ABCDxxxx);
  plot(x, line++, clk_sig1.BEKO_ABCDxxxx);
  plot(x, line++, clk_sig1.MOPA_xxxxEFGH);
  line++;

  plot(x, line++, clk_sig1.BAPY_xxxxxxGH);
  plot(x, line++, clk_sig1.BERU_ABCDEFxx);
  plot(x, line++, clk_sig1.BUFA_xxxxxxGH);
  plot(x, line++, clk_sig1.BOLO_ABCDEFxx);
  plot(x, line++, clk_sig1.BEJA_xxxxEFGH);
  plot(x, line++, clk_sig1.BANE_ABCDxxxx);
  plot(x, line++, clk_sig1.BELO_xxxxEFGH);
  plot(x, line++, clk_sig1.BAZE_ABCDxxxx);
  line++;

  plot(x, line++, clk_sig1.BUTO_xBCDEFGH);
  plot(x, line++, clk_sig1.BELE_Axxxxxxx);
  plot(x, line++, clk_sig1.BYJU_xBCDEFGH);
  plot(x, line++, clk_sig1.BALY_Axxxxxxx);
  plot(x, line++, clk_sig1.BOGA_xBCDEFGH);
  plot(x, line++, clk_sig1.BUVU_Axxxxxxx);
  plot(x, line++, clk_sig1.BYXO_xBCDEFGH);
  plot(x, line++, clk_sig1.BEDO_Axxxxxxx);
  plot(x, line++, clk_sig1.BOWA_xBCDEFGH);
  line++;

  plot(x, line++, clk_sig2.WUVU_AxxDExxH);
  plot(x, line++, clk_sig2.XUPY_xBCxxFGx);
  plot(x, line++, clk_sig2.AWOH_AxxDExxH);
  plot(x, line++, clk_sig2.VENA_xBCDExxx);
  plot(x, line++, clk_sig2.TALU_xBCDExxx);
  plot(x, line++, clk_sig2.SONO_AxxxxFGH);
  plot(x, line++, clk_sig2.WOSU_xxCDxxGH);
  plot(x, line++, clk_sig2.XOCE_ABxxEFxx);
  line++;
}

static int cursor = 20;

void TestClocks() {
  printf("\033[?6l");
  labels();

  ClockRegisters clk_reg;
  ResetRegisters rst_reg;
  SystemSignals sys_sig;

  clk_reg.reset();
  rst_reg.reset();
  sys_sig.reset();

  for (int phase = 0; phase < 16; phase++) {
    sys_sig.next_phase();

    for (int pass = 0; pass < 12; pass++) {
      ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_sig, clk_reg);
      ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(sys_sig, clk_sig1, rst_reg);
      ResetSignals2 rst_sig2 = ResetSignals2::tick_slow(sys_sig, rst_reg);
      ClockSignals2 clk_sig2 = ClockSignals2::tick_slow(rst_sig2, clk_reg);
      
      ClockRegisters::tock_slow1(sys_sig, clk_sig1, clk_reg);
      ClockRegisters::tock_slow2(sys_sig, clk_sig1, clk_sig2, rst_sig2, clk_reg);
    }

    ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_sig, clk_reg);
    ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(sys_sig, clk_sig1, rst_reg);
    ResetSignals2 rst_sig2 = ResetSignals2::tick_slow(sys_sig, rst_reg);
    ClockSignals2 clk_sig2 = ClockSignals2::tick_slow(rst_sig2, clk_reg);
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