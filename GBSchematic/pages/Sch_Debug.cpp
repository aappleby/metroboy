#include "Sch_Debug.h"

#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Pins.h"
#include "Sch_Joypad.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DebugRegisters::pwron() {
  pwron_all(FF60_0, FF60_1);
}

void DebugRegisters::reset() {
  // FIXME
  pwron_all(FF60_0, FF60_1);
}

void DebugRegisters::phase_begin() {
}

void DebugRegisters::phase_end() {
}

void DebugRegisters::pass_begin() {
}

bool DebugRegisters::pass_end() {
  return commit_all(FF60_0, FF60_1);
}

void DebugRegisters::tick(const SysSignals& sys_sig,
                          const BusSignals& bus_sig,
                          const RstSignals& rst_sig,
                          const ClkSignals& clk_sig,
                          const DecoderSignals& dec_sig,
                          BusTristates& bus_tri) {

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_sig.BEDO_Axxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_sig.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

  if (AFOP_Axxxxxxx) bus_tri.set_data(
    /*p05.ANOC*/ not(sys_sig.PIN_P10_B),
    /*p05.ATAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.AJEC*/ not(sys_sig.PIN_P10_B),
    /*p05.ASUZ*/ not(sys_sig.PIN_P10_B),
    /*p05.BENU*/ not(sys_sig.PIN_P10_B),
    /*p05.AKAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.ARAR*/ not(sys_sig.PIN_P10_B),
    /*p05.BEDA*/ not(sys_sig.PIN_P10_B)
  );

  if (LECO_xBCDEFGH) bus_tri.set_data(
    /*p07.ROMY*/ sys_sig.PIN_P10_B,
    /*p07.RYNE*/ sys_sig.PIN_P10_B,
    /*p07.REJY*/ sys_sig.PIN_P10_B,
    /*p07.RASE*/ sys_sig.PIN_P10_B,
    /*p07.REKA*/ sys_sig.PIN_P10_B,
    /*p07.ROWE*/ sys_sig.PIN_P10_B,
    /*p07.RYKE*/ sys_sig.PIN_P10_B,
    /*p07.RARU*/ sys_sig.PIN_P10_B
  );

  // FF60 debug reg
  /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, bus_tri.A05(), bus_tri.A06(), bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not(FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(FF60_0);


  /*p07.BURO*/ FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, bus_tri.D0());
  /*p07.AMUT*/ FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, bus_tri.D1());

#if 0

  // FIXME
  ///*p05.KORE*/ wire P05_NC0 = nand(DBG_FF00_D7, FF60_0);
  ///*p05.KYWE*/ wire P05_NC1 = nor (DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RDn = nand(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) bus_tri.D0 = not(/*p08.TOVO*/ not(pins.D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) bus_tri.D1 = not(/*p08.RUZY*/ not(pins.D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) bus_tri.D2 = not(/*p08.ROME*/ not(pins.D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) bus_tri.D3 = not(/*p08.SAZA*/ not(pins.D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) bus_tri.D4 = not(/*p08.TEHE*/ not(pins.D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) bus_tri.D5 = not(/*p08.RATU*/ not(pins.D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) bus_tri.D6 = not(/*p08.SOCA*/ not(pins.D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) bus_tri.D7 = not(/*p08.RYBA*/ not(pins.D7_C));


  //----------
  // more debug stuff

  /*p27.VYPO*/ wire VYPO = not(joy_pins.P10_B);
  /*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, clk.BOGA_xBCDEFGH);
  /*p25.SOLE*/ wire SOLE = not(TUSO);

  if (VYPO) bus_out.set_data(
    /*p25.TOVU*/ SOLE,
    /*p25.SOSA*/ SOLE,
    /*p25.SEDU*/ SOLE,
    /*p25.TAXO*/ SOLE,
    /*p25.TAHY*/ SOLE,
    /*p25.TESU*/ SOLE,
    /*p25.TAZU*/ SOLE,
    /*p25.TEWA*/ SOLE
  );

#endif
}

//-----------------------------------------------------------------------------

};