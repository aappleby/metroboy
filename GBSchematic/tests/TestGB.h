#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_Resets.h"
#include "../pages/Sch_LCD.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  TestGB();
  void pwron();
  void boot(bool fast);
  void reset();
  void sim_slow(int phases);
  void sim_fast(int phases);
  void sim(int phases, bool fast);

  static void check_match(const TestGB& a, const TestGB& b) {
    SystemSignals::check_match(a.sys_sig,  b.sys_sig);
    check_byte_match(a.clk_sig1, b.clk_sig1);
    check_byte_match(a.clk_sig2, b.clk_sig2);
    ClockRegisters::check_match(a.clk_reg,  b.clk_reg);
    check_byte_match(a.rst_sig1, b.rst_sig1);
    check_byte_match(a.rst_sig2, b.rst_sig2);
    ResetRegisters::check_match(a.rst_reg,  b.rst_reg);
    check_byte_match(a.lcd_sig,  b.lcd_sig);
    LCDRegisters::check_match(a.lcd_reg,  b.lcd_reg);
  }

  SystemSignals  sys_sig;
  ClockSignals1  clk_sig1;
  ClockSignals2  clk_sig2;
  ClockRegisters clk_reg;
  ResetSignals1  rst_sig1;
  ResetSignals2  rst_sig2;
  ResetRegisters rst_reg;
  LCDSignals     lcd_sig;
  LCDRegisters   lcd_reg;
};

//-----------------------------------------------------------------------------

};