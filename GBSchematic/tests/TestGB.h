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

  void sim(int phases, bool fast) {
    fast ? sim_fast(phases) : sim_slow(phases);
  }

  ClockSignals1 clk_sig1() const {
    return ClockSignals1::tick_fast(sys_reg, clk_reg1);
  }

  SystemRegisters sys_reg;
  ClockRegisters1 clk_reg1;
  ResetRegisters  rst_reg;
  ClockRegisters2 clk_reg2;
  //LCDRegisters    lcd_reg;
};

//-----------------------------------------------------------------------------

};