#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_Resets.h"
#include "../pages/Sch_LCD.h"
#include "../pages/Sch_System.h"
#include "../pages/Sch_BusControl.h"
#include "../pages/Sch_Registers.h"
#include "../pages/Sch_Serial.h"
#include "../pages/Sch_Timer.h"
#include "../pages/Sch_Video.h"
#include "../pages/Sch_Debug.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  TestGB();
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  int sim_phase(int phase_count);
  int sim_pass();

  void boot();

  SysRegisters    sys_reg;
  ClkRegisters    clk_reg;
  RstRegisters    rst_reg;
  DebugRegisters  dbg_reg;
  VclkRegisters   vclk_reg;
  VidRegisters    vid_reg;
  VidRegisters2   vid_reg2;
  LcdRegisters    lcd_reg;
  BusTristates    bus_tri;
  Timer           tim_reg;
  SerialRegisters ser_reg;

  uint8_t rom[65536];
};

//-----------------------------------------------------------------------------

};