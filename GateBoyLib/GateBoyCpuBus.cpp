#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::reset_to_bootrom() {
  BUS_CPU_D00p.reset(1);
  BUS_CPU_D01p.reset(1);
  BUS_CPU_D02p.reset(1);
  BUS_CPU_D03p.reset(1);
  BUS_CPU_D04p.reset(1);
  BUS_CPU_D05p.reset(1);
  BUS_CPU_D06p.reset(1);
  BUS_CPU_D07p.reset(1);
}

void GateBoyCpuBus::reset_to_cart() {
  TEPU_BOOT_BITn_h.reset(1, 1);
  SIG_CPU_BOOTp.reset(0);

  BUS_CPU_A00p.reset(0);
  BUS_CPU_A01p.reset(0);
  BUS_CPU_A02p.reset(0);
  BUS_CPU_A03p.reset(0);
  BUS_CPU_A04p.reset(1);
  BUS_CPU_A05p.reset(0);
  BUS_CPU_A06p.reset(1);
  BUS_CPU_A07p.reset(0);
  BUS_CPU_A08p.reset(0);
  BUS_CPU_A09p.reset(0);
  BUS_CPU_A10p.reset(0);
  BUS_CPU_A11p.reset(0);
  BUS_CPU_A12p.reset(0);
  BUS_CPU_A13p.reset(0);
  BUS_CPU_A14p.reset(0);
  BUS_CPU_A15p.reset(0);

  BUS_CPU_D00p.reset(1);
  BUS_CPU_D01p.reset(1);
  BUS_CPU_D02p.reset(1);
  BUS_CPU_D03p.reset(1);
  BUS_CPU_D04p.reset(1);
  BUS_CPU_D05p.reset(1);
  BUS_CPU_D06p.reset(1);
  BUS_CPU_D07p.reset(1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_addr(int phase_total, Req bus_req_new)
{
  uint16_t bus_addr_new = DELTA_HA ? bus_req_new.addr & 0x00FF : bus_req_new.addr;
  BUS_CPU_A00p.set((bus_addr_new >>  0) & 1);
  BUS_CPU_A01p.set((bus_addr_new >>  1) & 1);
  BUS_CPU_A02p.set((bus_addr_new >>  2) & 1);
  BUS_CPU_A03p.set((bus_addr_new >>  3) & 1);
  BUS_CPU_A04p.set((bus_addr_new >>  4) & 1);
  BUS_CPU_A05p.set((bus_addr_new >>  5) & 1);
  BUS_CPU_A06p.set((bus_addr_new >>  6) & 1);
  BUS_CPU_A07p.set((bus_addr_new >>  7) & 1);
  BUS_CPU_A08p.set((bus_addr_new >>  8) & 1);
  BUS_CPU_A09p.set((bus_addr_new >>  9) & 1);
  BUS_CPU_A10p.set((bus_addr_new >> 10) & 1);
  BUS_CPU_A11p.set((bus_addr_new >> 11) & 1);
  BUS_CPU_A12p.set((bus_addr_new >> 12) & 1);
  BUS_CPU_A13p.set((bus_addr_new >> 13) & 1);
  BUS_CPU_A14p.set((bus_addr_new >> 14) & 1);
  BUS_CPU_A15p.set((bus_addr_new >> 15) & 1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_data(int phase_total, Req bus_req_new) {
  wire bus_oe_new = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req_new.write;
  BUS_CPU_D00p.tri(bus_oe_new, (bus_req_new.data_lo >> 0) & 1);
  BUS_CPU_D01p.tri(bus_oe_new, (bus_req_new.data_lo >> 1) & 1);
  BUS_CPU_D02p.tri(bus_oe_new, (bus_req_new.data_lo >> 2) & 1);
  BUS_CPU_D03p.tri(bus_oe_new, (bus_req_new.data_lo >> 3) & 1);
  BUS_CPU_D04p.tri(bus_oe_new, (bus_req_new.data_lo >> 4) & 1);
  BUS_CPU_D05p.tri(bus_oe_new, (bus_req_new.data_lo >> 5) & 1);
  BUS_CPU_D06p.tri(bus_oe_new, (bus_req_new.data_lo >> 6) & 1);
  BUS_CPU_D07p.tri(bus_oe_new, (bus_req_new.data_lo >> 7) & 1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_pins(
  const GateBoyResetDebug& rst,
  const GateBoyClock& clk,
  int phase_total,
  Req bus_req_new)
{
  SIG_CPU_RDp.set(DELTA_HA ? 0 : bus_req_new.read);
  SIG_CPU_WRp.set(DELTA_HA ? 0 : bus_req_new.write);

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req_new.read && (bus_req_new.addr < 0xFF00))) {
    SIG_CPU_LATCH_EXT.set(1);
  }
  else {
    SIG_CPU_LATCH_EXT.set(0);
  }

  //SIG_CPU_6.set(0);

  // FIXME yeeeeeech this is nasty. probably not right.

  uint16_t bus_addr_new = bus_req_new.addr;
  bool addr_ext_new = (bus_req_new.read || bus_req_new.write) && (bus_addr_new < 0xFE00);
  if (bus_addr_new <= 0x00FF && bit(~TEPU_BOOT_BITn_h.qp_old())) addr_ext_new = false;
  if (DELTA_HA) {
    if ((bus_addr_new >= 0x8000) && (bus_addr_new < 0x9FFF)) addr_ext_new = false;
  }
  SIG_CPU_EXT_BUSp.set(addr_ext_new);

  // Data has to be driven on EFGH or we fail the wave tests

  /*SIG_CPU_ADDR_HIp*/ SIG_CPU_ADDR_HIp.set(SYRO_FE00_FFFF());
  /*SIG_CPU_UNOR_DBG*/ SIG_CPU_UNOR_DBG.set(rst.UNOR_MODE_DBG2p());
  /*SIG_CPU_UMUT_DBG*/ SIG_CPU_UMUT_DBG.set(rst.UMUT_MODE_DBG1p());

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
