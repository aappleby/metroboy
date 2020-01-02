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

  SystemRegisters sys_reg;
  ClockRegisters1 clk_reg1;
  ResetRegisters  rst_reg;
  VideoClocks     vid_clk_reg;
  LcdRegisters    lcd_reg;
  Bus             bus;
};

//-----------------------------------------------------------------------------

};