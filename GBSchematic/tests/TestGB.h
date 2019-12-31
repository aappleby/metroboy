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
  
  static void check_signals(TestGB& gb);
  static void check_states(TestGB& a, TestGB& b);
  static void check_match(TestGB& a, TestGB& b);

  // only call this if LCDC_EN is high and clocks are stable.
  void check_clock_phases();

  void sim_slow(int phases);
  void sim_fast(int phases);

  void sim(int phases, bool fast) {
    fast ? sim_fast(phases) : sim_slow(phases);
  }

  SystemRegisters sys_reg;
  ClockRegisters1 clk_reg1;
  ClockRegisters2 clk_reg2;
  ResetRegisters  rst_reg;
  //LCDRegisters    lcd_reg;
};

//-----------------------------------------------------------------------------

};