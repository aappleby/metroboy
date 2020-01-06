#include "Sch_Debug.h"

#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Pins.h"
#include "Sch_Joypad.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DebugRegisters::pwron() {
  pwron_all(FF60_0, FF60_1, SOTO_DBG);
}

void DebugRegisters::reset() {
  // FIXME
  pwron_all(FF60_0, FF60_1, SOTO_DBG);
}

void DebugRegisters::phase_begin() {
}

void DebugRegisters::phase_end() {
}

void DebugRegisters::pass_begin() {
}

bool DebugRegisters::pass_end() {
  return commit_all(FF60_0, FF60_1, SOTO_DBG);
}

void DebugRegisters::tick(const BusSignals& bus_sig,
                          const ClkSignals& /*clk_sig*/,
                          const SysPins& sys_pins,
                          const JoypadPins& joy_pins,
                          const DecoderSignals& dec_sig,
                          const RstSignals& rst_sig1,
                          BusTristates& bus_tri) {

  /*p27.VYPO*/ wire VYPO = not(joy_pins.P10_B);
  /*p07.UBET*/ wire T1n = not(sys_pins.T1);
  /*p07.UVAR*/ wire T2n = not(sys_pins.T2);

  /*p07.UPOJ*/ wire MODE_PROD   = nand(T1n, T2n, sys_pins.RST);
  /*p07.UMUT*/ wire MODE_DBG1   = and(sys_pins.T1, T2n);
  /*p07.UNOR*/ wire MODE_DBG2   = and(sys_pins.T2, T1n);
  /*p08.TOVA*/ wire MODE_DBG2n1 = not(MODE_DBG2);
  /*p08.RYCA*/ wire MODE_DBG2n2 = not(MODE_DBG2);

  // so is one of the debug pins a clock, and this is a clock divider?
  /*p25.SYCY*/ wire SYCY_CLK = not(MODE_DBG2);
  /*p01.SOTO*/ wire CUNU_RST = not(rst_sig1.SYS_RESET);

  /*p25.TUTO*/ wire DBG_VRAM  = and(MODE_DBG2, !SOTO_DBG);
  /*p25.RACO*/ wire DBG_VRAMn = not(DBG_VRAM);

  //----------
  // weird debug things, probably not right

#if 0
  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_sig.BEDO_Axxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_sig.BEDO_Axxxxxxx, MODE_DBG2);

  if (AFOP_Axxxxxxx) bus_out.set_data(
    /*p05.ANOC*/ not(joy_pins.P10_B),
    /*p05.ATAJ*/ not(joy_pins.P10_B),
    /*p05.AJEC*/ not(joy_pins.P10_B),
    /*p05.ASUZ*/ not(joy_pins.P10_B),
    /*p05.BENU*/ not(joy_pins.P10_B),
    /*p05.AKAJ*/ not(joy_pins.P10_B),
    /*p05.ARAR*/ not(joy_pins.P10_B),
    /*p05.BEDA*/ not(joy_pins.P10_B)
  );

  if (LECO_xBCDEFGH) bus_out.set_data(
    /*p07.ROMY*/ joy_pins.P10_B,
    /*p07.RYNE*/ joy_pins.P10_B,
    /*p07.REJY*/ joy_pins.P10_B,
    /*p07.RASE*/ joy_pins.P10_B,
    /*p07.REKA*/ joy_pins.P10_B,
    /*p07.ROWE*/ joy_pins.P10_B,
    /*p07.RYKE*/ joy_pins.P10_B,
    /*p07.RARU*/ joy_pins.P10_B
  );
#endif

  // FF60 debug reg

  /*p07.APET*/ wire MODE_DEBUG = or(MODE_DBG1, MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(MODE_DEBUG, bus_tri.A05(), bus_tri.A06(), bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not(FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(FF60_0);

  // FIXME
  ///*p05.KORE*/ wire P05_NC0 = nand(DBG_FF00_D7, FF60_0);
  ///*p05.KYWE*/ wire P05_NC1 = nor (DBG_FF00_D7, FF60_0o);

  /*p07.BURO*/ FF60_0.set(FF60_WRn, rst_sig1.SYS_RESETn, bus_tri.D0());
  /*p07.AMUT*/ FF60_1.set(FF60_WRn, rst_sig1.SYS_RESETn, bus_tri.D1());
  /*p25.SOTO*/ SOTO_DBG.set(SYCY_CLK, CUNU_RST, !SOTO_DBG);

#if 0
  /*p08.LYRA*/ wire DBG_D_RDn = nand(MODE_DBG2, ctl.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) bus_out.D0 = not(/*p08.TOVO*/ not(pins.D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) bus_out.D1 = not(/*p08.RUZY*/ not(pins.D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) bus_out.D2 = not(/*p08.ROME*/ not(pins.D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) bus_out.D3 = not(/*p08.SAZA*/ not(pins.D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) bus_out.D4 = not(/*p08.TEHE*/ not(pins.D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) bus_out.D5 = not(/*p08.RATU*/ not(pins.D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) bus_out.D6 = not(/*p08.SOCA*/ not(pins.D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) bus_out.D7 = not(/*p08.RYBA*/ not(pins.D7_C));
#endif

#if 0

  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ wire A00_Cn = not(pins.A00_C);
  /*p08.CAMU*/ wire A01_Cn = not(pins.A01_C);
  /*p08.BUXU*/ wire A02_Cn = not(pins.A02_C);
  /*p08.BASE*/ wire A03_Cn = not(pins.A03_C);
  /*p08.AFEC*/ wire A04_Cn = not(pins.A04_C);
  /*p08.ABUP*/ wire A05_Cn = not(pins.A05_C);
  /*p08.CYGU*/ wire A06_Cn = not(pins.A06_C);
  /*p08.COGO*/ wire A07_Cn = not(pins.A07_C);
  /*p08.MUJY*/ wire A08_Cn = not(pins.A08_C);
  /*p08.NENA*/ wire A09_Cn = not(pins.A09_C);
  /*p08.SURA*/ wire A10_Cn = not(pins.A10_C);
  /*p08.MADY*/ wire A11_Cn = not(pins.A11_C);
  /*p08.LAHE*/ wire A12_Cn = not(pins.A12_C);
  /*p08.LURA*/ wire A13_Cn = not(pins.A13_C);
  /*p08.PEVO*/ wire A14_Cn = not(pins.A14_C);
  /*p08.RAZA*/ wire A15_Cn = not(pins.A15_C);

  if (MODE_DBG2n1) bus_out.set_addr(
    /*p08.KEJO*/ not(A00_Cn),
    /*p08.BYXE*/ not(A01_Cn),
    /*p08.AKAN*/ not(A02_Cn),
    /*p08.ANAR*/ not(A03_Cn),
    /*p08.AZUV*/ not(A04_Cn),
    /*p08.AJOV*/ not(A05_Cn),
    /*p08.BYNE*/ not(A06_Cn),
    /*p08.BYNA*/ not(A07_Cn),
    /*p08.LOFA*/ not(A08_Cn),
    /*p08.MAPU*/ not(A09_Cn),
    /*p08.RALA*/ not(A10_Cn),
    /*p08.LORA*/ not(A11_Cn),
    /*p08.LYNA*/ not(A12_Cn),
    /*p08.LEFY*/ not(A13_Cn),
    /*p08.NEFE*/ not(A14_Cn),
    /*p08.SYZU*/ not(A15_Cn)
  );

  //----------
  // more debug stuff

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