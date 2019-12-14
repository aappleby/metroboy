#include "Sch_BusControl.h"

#include "Sch_CpuSignals.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"

namespace Schematics {

/*
0b000xxxxxxxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxxxxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxxxxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxxxxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxxxxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxxxxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxxxxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxxxxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)
*/

//-----------------------------------------------------------------------------

void Bus_tick(const Cpu& cpu,
              const Clocks& clocks,
              const Pins& pins,
              const Debug& debug,
              const BusControl& prev,
              BusControl& next) {

  /*p07.UJYV*/ wire CPU_RD_MUX   = mux2n(pins.RD_C, cpu.CPU_RAW_RD, debug.MODE_DBG2);
  /*p01.AREV*/ wire AREV = nand(cpu.CPU_RAW_WR, clocks.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
  /*p07.UBAL*/ wire CPU_WR_MUX   = mux2n(pins.WR_C, CPU_WR_xxxxEFGx, debug.MODE_DBG2);

  /*p07.TEDO*/ next.CPU_RD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ next.CPU_WR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(prev.CPU_RD);
  /*p07.DYKY*/ wire CPU_WRn = not(prev.CPU_WR);

  /*p07.ASOT*/ next.CPU_RD2 = not(CPU_RDn);
  /*p07.CUPA*/ next.CPU_WR2 = not(CPU_WRn);

  /*p01.ATYP*/ wire PHASE_ABCDxxxx2 = not(!clocks.PHASE_ABCDxxxx);  
  /*p01.AROV*/ wire PHASE_xxCDEFxx2 = not(!clocks.PHASE_xxCDEFxx);
  /*p01.AJAX*/ wire PHASE_xxxxEFGH3 = not(PHASE_ABCDxxxx2);
  /*p01.AGUT*/ wire AGUT_xxCDEFGH = and(or(PHASE_xxxxEFGH3, PHASE_xxCDEFxx2), cpu.ADDR_VALID);
  /*p01.AWOD*/ wire AWOD = or(debug.MODE_DBG2, AGUT_xxCDEFGH);
  /*p01.ABUZ*/ next.ADDR_VALID_ABxxxxxx = not(AWOD);

#if 0
  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), in.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);

  /*p11.BUDA*/ wire CPU_RDo = not(b.ctl.CPU_RD);
  /*p08.RORU*/ next.CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, dbg.MODE_DBG2);
  /*p08.LULA*/ next.CBUS_TO_CEXT  = not(prev.CBUS_TO_CEXTn);
#endif
}

//-----------------------------------------------------------------------------

};