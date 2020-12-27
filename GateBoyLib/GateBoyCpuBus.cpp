#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::reset_to_bootrom() {
  for (int i = 0; i < 8; i++) {
    BUS_CPU_D[i].reset(1);
  }
}

void GateBoyCpuBus::reset_to_cart() {
  TEPU_BOOT_BITn_h.reset(1, 1);
  SIG_CPU_BOOTp.reset(0);

  BUS_CPU_A[ 0].reset(0);
  BUS_CPU_A[ 1].reset(0);
  BUS_CPU_A[ 2].reset(0);
  BUS_CPU_A[ 3].reset(0);
  BUS_CPU_A[ 4].reset(1);
  BUS_CPU_A[ 5].reset(0);
  BUS_CPU_A[ 6].reset(1);
  BUS_CPU_A[ 7].reset(0);
  BUS_CPU_A[ 8].reset(0);
  BUS_CPU_A[ 9].reset(0);
  BUS_CPU_A[10].reset(0);
  BUS_CPU_A[11].reset(0);
  BUS_CPU_A[12].reset(0);
  BUS_CPU_A[13].reset(0);
  BUS_CPU_A[14].reset(0);
  BUS_CPU_A[15].reset(0);

  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
  BUS_CPU_D[0].reset(1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_addr(int phase_total, Req bus_req_new)
{
  uint16_t bus_addr_new = DELTA_HA ? bus_req_new.addr & 0x00FF : bus_req_new.addr;
  BUS_CPU_A[ 0].set((bus_addr_new >>  0) & 1);
  BUS_CPU_A[ 1].set((bus_addr_new >>  1) & 1);
  BUS_CPU_A[ 2].set((bus_addr_new >>  2) & 1);
  BUS_CPU_A[ 3].set((bus_addr_new >>  3) & 1);
  BUS_CPU_A[ 4].set((bus_addr_new >>  4) & 1);
  BUS_CPU_A[ 5].set((bus_addr_new >>  5) & 1);
  BUS_CPU_A[ 6].set((bus_addr_new >>  6) & 1);
  BUS_CPU_A[ 7].set((bus_addr_new >>  7) & 1);
  BUS_CPU_A[ 8].set((bus_addr_new >>  8) & 1);
  BUS_CPU_A[ 9].set((bus_addr_new >>  9) & 1);
  BUS_CPU_A[10].set((bus_addr_new >> 10) & 1);
  BUS_CPU_A[11].set((bus_addr_new >> 11) & 1);
  BUS_CPU_A[12].set((bus_addr_new >> 12) & 1);
  BUS_CPU_A[13].set((bus_addr_new >> 13) & 1);
  BUS_CPU_A[14].set((bus_addr_new >> 14) & 1);
  BUS_CPU_A[15].set((bus_addr_new >> 15) & 1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuBus::set_data(int phase_total, Req bus_req_new) {
  wire bus_oe_new = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req_new.write;
  BUS_CPU_D[0].tri(bus_oe_new, (bus_req_new.data_lo >> 0) & 1);
  BUS_CPU_D[1].tri(bus_oe_new, (bus_req_new.data_lo >> 1) & 1);
  BUS_CPU_D[2].tri(bus_oe_new, (bus_req_new.data_lo >> 2) & 1);
  BUS_CPU_D[3].tri(bus_oe_new, (bus_req_new.data_lo >> 3) & 1);
  BUS_CPU_D[4].tri(bus_oe_new, (bus_req_new.data_lo >> 4) & 1);
  BUS_CPU_D[5].tri(bus_oe_new, (bus_req_new.data_lo >> 5) & 1);
  BUS_CPU_D[6].tri(bus_oe_new, (bus_req_new.data_lo >> 6) & 1);
  BUS_CPU_D[7].tri(bus_oe_new, (bus_req_new.data_lo >> 7) & 1);
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

  SIG_CPU_6.set(0);

  // FIXME yeeeeeech this is nasty. probably not right.

  uint16_t bus_addr_new = bus_req_new.addr;
  bool addr_ext_new = (bus_req_new.read || bus_req_new.write) && (bus_addr_new < 0xFE00);
  if (bus_addr_new <= 0x00FF && bit(~TEPU_BOOT_BITn_h.qp_old2())) addr_ext_new = false;
  if (DELTA_HA) {
    if ((bus_addr_new >= 0x8000) && (bus_addr_new < 0x9FFF)) addr_ext_new = false;
  }
  SIG_CPU_EXT_BUSp.set(addr_ext_new);

  // Data has to be driven on EFGH or we fail the wave tests

  SIG_CPU_ADDR_HIp.set(SYRO_FE00_FFFF());
  SIG_CPU_UNOR_DBG.set(rst.UNOR_MODE_DBG2p());
  SIG_CPU_UMUT_DBG.set(rst.UMUT_MODE_DBG1p());

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2nb(rst.UNOR_MODE_DBG2p(), /*PIN79_EXT_RDn.qn_new2()*/ 0, SIG_CPU_RDp.qp_new2()); // Ignoring debug stuff for now
  /* p07.TEDO*/ wire _TEDO_CPU_RDp = not1b(_UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2b(clk.ADAR_ABCxxxxH(), clk.ATYP_ABCDxxxx());
  /* p01.AREV*/ wire _AREV_CPU_WRn = nand2b(SIG_CPU_WRp.qp_new2(), _AFAS_xxxxEFGx);
  /* p01.APOV*/ wire _APOV_CPU_WRp = not1b(_AREV_CPU_WRn);

  /* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2nb(rst.UNOR_MODE_DBG2p(), /*PIN78_EXT_WRn.qn_new2()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp = not1b(_UBAL_CPU_WRn); // xxxxEFGx

  TEDO_CPU_RDp.set(_TEDO_CPU_RDp);
  APOV_CPU_WRp.set(_APOV_CPU_WRp);
  TAPU_CPU_WRp.set(_TAPU_CPU_WRp);
}

//------------------------------------------------------------------------------------------------------------------------
