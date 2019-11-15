#include "P01_ClocksReset.h"
#include "Gameboy.h"

/*
union { bool DIV_00; bool DIV_00; };
union { bool DIV_01; bool DIV_01; bool CLK_256K; };
union { bool DIV_02; bool DIV_02; };
union { bool DIV_03; bool DIV_03; bool CLK_64K; };
union { bool DIV_04; bool DIV_04; };
union { bool DIV_05; bool DIV_05; bool TAMA_16Kn; };
union { bool DIV_06; bool DIV_06; };
union { bool DIV_07; bool DIV_07; };
union { bool DIV_08; bool DIV_08; };
union { bool DIV_09; bool DIV_09; };
union { bool DIV_10; bool DIV_10; };
union { bool DIV_11; bool DIV_11; };
union { bool DIV_12; bool DIV_12; };
union { bool DIV_13; bool DIV_13; };
union { bool DIV_14; bool DIV_14; };
union { bool DIV_15; bool DIV_15; };
*/

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

#if 0
const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("-----Clocks-----"),
    SignalData("PHASE_ABCD", offsetof(P01_ClocksReset, CLK_ABCDxxxx1)),
    SignalData("PHASE_BCDE", offsetof(P01_ClocksReset, CLK_xBCDExxx1)),
    SignalData("PHASE_CDEF", offsetof(P01_ClocksReset, CLK_xxCDEFxx1)),
    SignalData("PHASE_DEFG", offsetof(P01_ClocksReset, CLK_xxxDEFGx1)),
    SignalData("COKE", offsetof(P01_ClocksReset, COKE)),

    /*
    SignalData("-----DIV-----"),
    SignalData("DIV_00",    offsetof(P01_ClocksReset, DIV_00)),
    SignalData("DIV_01",    offsetof(P01_ClocksReset, DIV_01)),
    SignalData("DIV_02",    offsetof(P01_ClocksReset, DIV_02)),
    SignalData("DIV_03",    offsetof(P01_ClocksReset, DIV_03)),
    SignalData("DIV_04",    offsetof(P01_ClocksReset, DIV_04)),
    SignalData("DIV_05",    offsetof(P01_ClocksReset, DIV_05)),
    SignalData("DIV_06",    offsetof(P01_ClocksReset, DIV_06)),
    SignalData("DIV_07",    offsetof(P01_ClocksReset, DIV_07)),
    SignalData("DIV_08",    offsetof(P01_ClocksReset, DIV_08)),
    SignalData("DIV_09",    offsetof(P01_ClocksReset, DIV_09)),
    SignalData("DIV_10",    offsetof(P01_ClocksReset, DIV_10)),
    SignalData("DIV_11",    offsetof(P01_ClocksReset, DIV_11)),
    SignalData("DIV_12",    offsetof(P01_ClocksReset, DIV_12)),
    SignalData("DIV_13",    offsetof(P01_ClocksReset, DIV_13)),
    SignalData("DIV_14",    offsetof(P01_ClocksReset, DIV_14)),
    SignalData("DIV_15",    offsetof(P01_ClocksReset, DIV_15)),
    */

    /*
    SignalData("----------"),
    SignalData("BARA",       offsetof(P01_ClocksReset, BARA)),
    SignalData("CARU",       offsetof(P01_ClocksReset, CARU)),
    SignalData("BYLU",       offsetof(P01_ClocksReset, BYLU)),

    SignalData("----------"),
    SignalData("ATYK",       offsetof(P01_ClocksReset, ATYK)),
    SignalData("AVOK",       offsetof(P01_ClocksReset, AVOK)),
    SignalData("JESO",       offsetof(P01_ClocksReset, JESO)),
    */
  };
}
#endif

//-----------------------------------------------------------------------------

/*
void blah(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  if (b.p07.MODE_DBG1 || b.p07.MODE_DBG2) {
    c.cpu.CPU_RESET   = 1;
    c.p01.RESET_REG   = 0;
    c.p01.SYS_RESETn1 = 0;
  }
  else if (!b.chip.RST && !b.p01.CPUCLK_REQn && !b.p01.CLK_BAD1) {
    c.p01.CPU_RESET   = 0;
    c.p01.RESET_REG   = tock_pos(a.p01.RESET_CLK, b.p01.RESET_CLK, 1, b.p01.RESET_REG, 0);
    c.p01.SYS_RESETn1 = not(b.p01.RESET_REG);
  }
  else {
    c.p01.CPU_RESET   = b.p01.DIV_15;
    c.p01.RESET_REG   = tock_pos(a.p01.RESET_CLK, b.p01.RESET_CLK, 1, b.p01.RESET_REG, b.chip.RST || b.p01.DIV_15);
    c.p01.SYS_RESETn1 = not(b.p01.RESET_REG) && not(b.chip.RST) && not(b.p01.DIV_15);
  }
}
*/

