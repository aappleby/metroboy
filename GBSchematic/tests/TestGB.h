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
  
  void update_signals(bool fast);
  
  static void check_signals(const TestGB& a, const TestGB& b);
  static void check_states(const TestGB& a, const TestGB& b);
  static void check_match(const TestGB& a, const TestGB& b);

  void sim_slow(int phases);
  void sim_fast(int phases);

  void sim(int phases, bool fast) {
    fast ? sim_fast(phases) : sim_slow(phases);
  }

  SystemSignals   sys_sig_b;
  SystemSignals   sys_sig_c;

  ClockSignals1   clk_sig1;
  ClockRegisters1 clk_reg1;

  ClockSignals2   clk_sig2;
  ClockRegisters2 clk_reg2;

  ResetSignals1   rst_sig1;
  ResetSignals2   rst_sig2;
  ResetRegisters  rst_reg;
  LCDSignals      lcd_sig;
  LCDRegisters    lcd_reg;
};

//-----------------------------------------------------------------------------

};