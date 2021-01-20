#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::reset_to_bootrom() {
}

void GateBoyCpuBus::reset_to_cart() {
  TEPU_BOOT_BITn_h.reset(1, 1);
  SIG_CPU_BOOTp.reset(0);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_pins(
  const GateBoyResetDebug& rst,
  const GateBoyClock& clk,
  const GateBoyBuses& new_bus,
  int phase_total,
  Req bus_req_new)
{
  SIG_CPU_RDp.sig_in(DELTA_HA ? 0 : bus_req_new.read);
  SIG_CPU_WRp.sig_in(DELTA_HA ? 0 : bus_req_new.write);

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req_new.read && (bus_req_new.addr < 0xFF00))) {
    SIG_CPU_LATCH_EXT.sig_in(1);
  }
  else {
    SIG_CPU_LATCH_EXT.sig_in(0);
  }

  //SIG_CPU_6.set(0);

  // FIXME yeeeeeech this is nasty. probably not right.

  uint16_t bus_addr_new = bus_req_new.addr;
  bool addr_ext_new = (bus_req_new.read || bus_req_new.write) && (bus_addr_new < 0xFE00);
  if (bus_addr_new <= 0x00FF && bit(~TEPU_BOOT_BITn_h.qp_old())) addr_ext_new = false;
  if (DELTA_HA) {
    if ((bus_addr_new >= 0x8000) && (bus_addr_new < 0x9FFF)) addr_ext_new = false;
  }
  SIG_CPU_EXT_BUSp.sig_in(addr_ext_new);

  // Data has to be driven on EFGH or we fail the wave tests

  /*SIG_CPU_ADDR_HIp*/ SIG_CPU_ADDR_HIp.sig_out(new_bus.SYRO_FE00_FFFF());
  /*SIG_CPU_UNOR_DBG*/ SIG_CPU_UNOR_DBG.sig_out(rst.UNOR_MODE_DBG2p());
  /*SIG_CPU_UMUT_DBG*/ SIG_CPU_UMUT_DBG.sig_out(rst.UMUT_MODE_DBG1p());

  ///* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_79_EXT_RDn.qn_new()*/ 0, SIG_CPU_RDp.qp_new()); // Ignoring debug stuff for now
  ///* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_78_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = not1(SIG_CPU_RDp);
  /* p07.TEDO*/ TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(clk.ADAR_ABCxxxxH(), clk.ATYP_ABCDxxxx());
  /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(SIG_CPU_WRp, _AFAS_xxxxEFGx);
  /* p01.APOV*/ APOV_CPU_WRp = not1(_AREV_CPU_WRn);

  /* p07.UBAL*/ wire _UBAL_CPU_WRn = not1(APOV_CPU_WRp);
  /* p07.TAPU*/ TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx
}

//------------------------------------------------------------------------------------------------------------------------
