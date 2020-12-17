#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

void GateBoyCpuBus::reset_cart() {
  BOOT_BITn_h.reset(REG_D1C1);
  PIN_CPU_BOOTp.reset(REG_D0C0);
}

void GateBoyCpuBus::tock(
  const GateBoyResetDebug& rst,
  const GateBoyClock& clk,
  int phase_total,
  Req bus_req)
{
  uint16_t bus_addr = DELTA_HA ? bus_req.addr & 0x00FF : bus_req.addr;
  BUS_CPU_A[ 0].set_new(wire((bus_addr >>  0) & 1));
  BUS_CPU_A[ 1].set_new(wire((bus_addr >>  1) & 1));
  BUS_CPU_A[ 2].set_new(wire((bus_addr >>  2) & 1));
  BUS_CPU_A[ 3].set_new(wire((bus_addr >>  3) & 1));
  BUS_CPU_A[ 4].set_new(wire((bus_addr >>  4) & 1));
  BUS_CPU_A[ 5].set_new(wire((bus_addr >>  5) & 1));
  BUS_CPU_A[ 6].set_new(wire((bus_addr >>  6) & 1));
  BUS_CPU_A[ 7].set_new(wire((bus_addr >>  7) & 1));
  BUS_CPU_A[ 8].set_new(wire((bus_addr >>  8) & 1));
  BUS_CPU_A[ 9].set_new(wire((bus_addr >>  9) & 1));
  BUS_CPU_A[10].set_new(wire((bus_addr >> 10) & 1));
  BUS_CPU_A[11].set_new(wire((bus_addr >> 11) & 1));
  BUS_CPU_A[12].set_new(wire((bus_addr >> 12) & 1));
  BUS_CPU_A[13].set_new(wire((bus_addr >> 13) & 1));
  BUS_CPU_A[14].set_new(wire((bus_addr >> 14) & 1));
  BUS_CPU_A[15].set_new(wire((bus_addr >> 15) & 1));

  wire bus_oe = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;
  BUS_CPU_D[0].set_new(wire(!bus_oe || (bus_req.data_lo >> 0) & 1));
  BUS_CPU_D[1].set_new(wire(!bus_oe || (bus_req.data_lo >> 1) & 1));
  BUS_CPU_D[2].set_new(wire(!bus_oe || (bus_req.data_lo >> 2) & 1));
  BUS_CPU_D[3].set_new(wire(!bus_oe || (bus_req.data_lo >> 3) & 1));
  BUS_CPU_D[4].set_new(wire(!bus_oe || (bus_req.data_lo >> 4) & 1));
  BUS_CPU_D[5].set_new(wire(!bus_oe || (bus_req.data_lo >> 5) & 1));
  BUS_CPU_D[6].set_new(wire(!bus_oe || (bus_req.data_lo >> 6) & 1));
  BUS_CPU_D[7].set_new(wire(!bus_oe || (bus_req.data_lo >> 7) & 1));

  BUS_CPU_D_out[0].reset(REG_D1C0);
  BUS_CPU_D_out[1].reset(REG_D1C0);
  BUS_CPU_D_out[2].reset(REG_D1C0);
  BUS_CPU_D_out[3].reset(REG_D1C0);
  BUS_CPU_D_out[4].reset(REG_D1C0);
  BUS_CPU_D_out[5].reset(REG_D1C0);
  BUS_CPU_D_out[6].reset(REG_D1C0);
  BUS_CPU_D_out[7].reset(REG_D1C0);

  PIN_CPU_RDp.setp(DELTA_HA ? 0 : bus_req.read);
  PIN_CPU_WRp.setp(DELTA_HA ? 0 : bus_req.write);

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
    PIN_CPU_LATCH_EXT.setp(1);
  }
  else {
    PIN_CPU_LATCH_EXT.setp(0);
  }

  PIN_CPU_6.setp(0);

  bool addr_ext = (PIN_CPU_RDp.qp_new() || PIN_CPU_WRp.qp_new()) && (bus_addr < 0xFE00);
  if (bus_addr <= 0x00FF && !BOOT_BITn_h.qp_old()) addr_ext = false;
  PIN_CPU_EXT_BUSp.setp(addr_ext);


#if 0
  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    PIN_CPU_EXT_BUSp.setp(addr_ext);
  }
  else {
    // This seems wrong, but it passes tests. *shrug*
    if (bus_addr >= 0x8000 && bus_addr <= 0x9FFF) {
      PIN_CPU_EXT_BUSp.setp(0);
    }
    else {
      PIN_CPU_EXT_BUSp.setp(addr_ext);
    }
  }
#endif

  // Data has to be driven on EFGH or we fail the wave tests

  PIN_CPU_ADDR_HIp.setp(SYRO_FE00_FFFF());
  PIN_CPU_UNOR_DBG.setp(rst.UNOR_MODE_DBG2p());
  PIN_CPU_UMUT_DBG.setp(rst.UMUT_MODE_DBG1p());

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_EXT_RDn.qn_new()*/ 0, PIN_CPU_RDp.qp_new()); // Ignoring debug stuff for now
  /* p07.TEDO*/ wire _TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(clk.ADAR_ABCxxxxH(), clk.ATYP_ABCDxxxx());
  /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(PIN_CPU_WRp.qp_new(), _AFAS_xxxxEFGx);
  /* p01.APOV*/ wire _APOV_CPU_WRp = not1(_AREV_CPU_WRn);

  /* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx

  TEDO_CPU_RDp.set_new(_TEDO_CPU_RDp);
  APOV_CPU_WRp.set_new(_APOV_CPU_WRp);
  TAPU_CPU_WRp.set_new(_TAPU_CPU_WRp);
}
