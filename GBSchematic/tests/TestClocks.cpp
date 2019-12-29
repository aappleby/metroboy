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

  printAt(0, line++, "PHAZ_xxxDEFGx");
  printAt(0, line++, "PHAZ_xxxxEFGH");
  printAt(0, line++, "PHAZ_AxxxxFGH");
  printAt(0, line++, "PHAZ_ABxxxxGH");
  printAt(0, line++, "AFEP_ABCDxxxx");
  printAt(0, line++, "ATYP_xxxDEFGx");
  printAt(0, line++, "ADAR_xxCDEFxx");
  printAt(0, line++, "AROV_AxxxxFGH");
  printAt(0, line++, "AFAS_ABxxxxxH");
  line++;

  printAt(0, line++, "NULE_ABCxxxxH");
  printAt(0, line++, "BYRY_xxxDEFGx");
  printAt(0, line++, "BUDE_ABCxxxxH");
  printAt(0, line++, "DOVA_xxxDEFGx");
  printAt(0, line++, "UVYT_xxxDEFGx");
  printAt(0, line++, "BEKO_xxxDEFGx");
  printAt(0, line++, "MOPA_ABCxxxxH");
  line++;

  printAt(0, line++, "BAPY_xBCxxxxx");
  printAt(0, line++, "BERU_AxxDEFGH");
  printAt(0, line++, "BUFA_xBCxxxxx");
  printAt(0, line++, "BOLO_AxxDEFGH");
  printAt(0, line++, "BEJA_ABCxxxxH");
  printAt(0, line++, "BANE_xxxDEFGx");
  printAt(0, line++, "BELO_ABCxxxxH");
  printAt(0, line++, "BAZE_xxxDEFGx");
  line++;

  printAt(0, line++, "BUTO_ABCxEFGH");
  printAt(0, line++, "BELE_xxxDxxxx");
  printAt(0, line++, "BYJU_ABCxEFGH");
  printAt(0, line++, "BALY_xxxDxxxx");
  printAt(0, line++, "BOGA_ABCxEFGH");
  printAt(0, line++, "BUVU_xxxDxxxx");
  printAt(0, line++, "BYXO_ABCxEFGH");
  printAt(0, line++, "BEDO_xxxDxxxx");
  printAt(0, line++, "BOWA_ABCxEFGH");
  line++;
}

void dump(int x, ClockSignals1& clk_sig1, ClockSignals2& clk_sig2) {
  int line = 3;
  plot(x, line++, clk_sig1.ROOT_AxCxExGx);
  line++;

  plot(x, line++, clk_sig1.PHAZ_xxxDEFGx);
  plot(x, line++, clk_sig1.PHAZ_xxxxEFGH);
  plot(x, line++, clk_sig1.PHAZ_AxxxxFGH);
  plot(x, line++, clk_sig1.PHAZ_ABxxxxGH);
  plot(x, line++, clk_sig1.AFEP_ABCDxxxx);
  plot(x, line++, clk_sig1.ATYP_xxxDEFGx);
  plot(x, line++, clk_sig1.ADAR_xxCDEFxx);
  plot(x, line++, clk_sig1.AROV_AxxxxFGH);
  plot(x, line++, clk_sig1.AFAS_ABxxxxxH);
  line++;

  plot(x, line++, clk_sig1.NULE_ABCxxxxH);
  plot(x, line++, clk_sig1.BYRY_xxxDEFGx);
  plot(x, line++, clk_sig1.BUDE_ABCxxxxH);
  plot(x, line++, clk_sig1.DOVA_xxxDEFGx);
  plot(x, line++, clk_sig1.UVYT_xxxDEFGx);
  plot(x, line++, clk_sig1.BEKO_xxxDEFGx);
  plot(x, line++, clk_sig1.MOPA_ABCxxxxH);
  line++;

  plot(x, line++, clk_sig1.BAPY_xBCxxxxx);
  plot(x, line++, clk_sig1.BERU_AxxDEFGH);
  plot(x, line++, clk_sig1.BUFA_xBCxxxxx);
  plot(x, line++, clk_sig1.BOLO_AxxDEFGH);
  plot(x, line++, clk_sig1.BEJA_ABCxxxxH);
  plot(x, line++, clk_sig1.BANE_xxxDEFGx);
  plot(x, line++, clk_sig1.BELO_ABCxxxxH);
  plot(x, line++, clk_sig1.BAZE_xxxDEFGx);
  line++;

  plot(x, line++, clk_sig1.BUTO_ABCxEFGH);
  plot(x, line++, clk_sig1.BELE_xxxDxxxx);
  plot(x, line++, clk_sig1.BYJU_ABCxEFGH);
  plot(x, line++, clk_sig1.BALY_xxxDxxxx);
  plot(x, line++, clk_sig1.BOGA_ABCxEFGH);
  plot(x, line++, clk_sig1.BUVU_xxxDxxxx);
  plot(x, line++, clk_sig1.BYXO_ABCxEFGH);
  plot(x, line++, clk_sig1.BEDO_xxxDxxxx);
  plot(x, line++, clk_sig1.BOWA_ABCxEFGH);
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