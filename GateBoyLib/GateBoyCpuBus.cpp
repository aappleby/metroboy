#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::reset_to_bootrom() {
  for (int i = 0; i < 8; i++) {
    BUS_CPU_D[i].reset_to_cart(1);
  }
}

void GateBoyCpuBus::reset_to_cart() {
  BOOT_BITn_h.reset_to_cart(REG_D1C1);
  SIG_CPU_BOOTp.reset_to_cart(REG_D0C0);

  for (int i = 0; i < 8; i++) {
    BUS_CPU_D[i].reset_to_cart(1);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_addr(int phase_total, Req bus_req_new)
{
  uint16_t bus_addr_new = DELTA_HA ? bus_req_new.addr & 0x00FF : bus_req_new.addr;
  BUS_CPU_A[ 0].set_new(wire(bus_addr_new & 0x0001));
  BUS_CPU_A[ 1].set_new(wire(bus_addr_new & 0x0002));
  BUS_CPU_A[ 2].set_new(wire(bus_addr_new & 0x0004));
  BUS_CPU_A[ 3].set_new(wire(bus_addr_new & 0x0008));
  BUS_CPU_A[ 4].set_new(wire(bus_addr_new & 0x0010));
  BUS_CPU_A[ 5].set_new(wire(bus_addr_new & 0x0020));
  BUS_CPU_A[ 6].set_new(wire(bus_addr_new & 0x0040));
  BUS_CPU_A[ 7].set_new(wire(bus_addr_new & 0x0080));
  BUS_CPU_A[ 8].set_new(wire(bus_addr_new & 0x0100));
  BUS_CPU_A[ 9].set_new(wire(bus_addr_new & 0x0200));
  BUS_CPU_A[10].set_new(wire(bus_addr_new & 0x0400));
  BUS_CPU_A[11].set_new(wire(bus_addr_new & 0x0800));
  BUS_CPU_A[12].set_new(wire(bus_addr_new & 0x1000));
  BUS_CPU_A[13].set_new(wire(bus_addr_new & 0x2000));
  BUS_CPU_A[14].set_new(wire(bus_addr_new & 0x4000));
  BUS_CPU_A[15].set_new(wire(bus_addr_new & 0x8000));
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_data(int phase_total, Req bus_req_new) {
  wire bus_oe_new = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req_new.write;
  BUS_CPU_D[0].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x01));
  BUS_CPU_D[1].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x02));
  BUS_CPU_D[2].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x04));
  BUS_CPU_D[3].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x08));
  BUS_CPU_D[4].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x10));
  BUS_CPU_D[5].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x20));
  BUS_CPU_D[6].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x40));
  BUS_CPU_D[7].tri(bus_oe_new, wire(bus_req_new.data_lo & 0x80));
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_pins(
  const GateBoyResetDebug& rst,
  const GateBoyClock& clk,
  int phase_total,
  Req bus_req_new)
{
  SIG_CPU_RDp.set_new(DELTA_HA ? 0 : bus_req_new.read);
  SIG_CPU_WRp.set_new(DELTA_HA ? 0 : bus_req_new.write);

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req_new.read && (bus_req_new.addr < 0xFF00))) {
    SIG_CPU_LATCH_EXT.set_new(1);
  }
  else {
    SIG_CPU_LATCH_EXT.set_new(0);
  }

  SIG_CPU_6.set_new(0);

  // FIXME yeeeeeech this is nasty. probably not right.

  uint16_t bus_addr_new = bus_req_new.addr;
  bool addr_ext_new = (bus_req_new.read || bus_req_new.write) && (bus_addr_new < 0xFE00);
  if (bus_addr_new <= 0x00FF && !BOOT_BITn_h.qp_old()) addr_ext_new = false;
  if (DELTA_HA) {
    if ((bus_addr_new >= 0x8000) && (bus_addr_new < 0x9FFF)) addr_ext_new = false;
  }
  SIG_CPU_EXT_BUSp.set_new(addr_ext_new);

  // Data has to be driven on EFGH or we fail the wave tests

  SIG_CPU_ADDR_HIp.set_new(SYRO_FE00_FFFF());
  SIG_CPU_UNOR_DBG.set_new(rst.UNOR_MODE_DBG2p());
  SIG_CPU_UMUT_DBG.set_new(rst.UMUT_MODE_DBG1p());

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN79_EXT_RDn.qn_new()*/ 0, SIG_CPU_RDp.qp_new()); // Ignoring debug stuff for now
  /* p07.TEDO*/ wire _TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(clk.ADAR_ABCxxxxH(), clk.ATYP_ABCDxxxx());
  /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(SIG_CPU_WRp.qp_new(), _AFAS_xxxxEFGx);
  /* p01.APOV*/ wire _APOV_CPU_WRp = not1(_AREV_CPU_WRn);

  /* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN78_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx

  TEDO_CPU_RDp.set_new(_TEDO_CPU_RDp);
  APOV_CPU_WRp.set_new(_APOV_CPU_WRp);
  TAPU_CPU_WRp.set_new(_TAPU_CPU_WRp);
}

//------------------------------------------------------------------------------------------------------------------------
