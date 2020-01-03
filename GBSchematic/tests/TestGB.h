#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_Resets.h"
#include "../pages/Sch_LCD.h"
#include "../pages/Sch_System.h"
#include "../pages/Sch_BusControl.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  TestGB();
  void pwron();
  void boot();
  void reset();
  
  void sim(int phases);
  void commit();

  SysRegisters sys_reg;
  ClkRegisters    clk_reg1;
  RstRegisters  rst_reg;
  VclkRegisters   vclk_reg;
  LcdRegisters    lcd_reg;
  BusTristates             bus;
};

//-----------------------------------------------------------------------------

};