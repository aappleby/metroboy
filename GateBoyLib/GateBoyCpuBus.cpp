#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyResetDebug.h"
#include "GateBoyClocks.h"

void GateBoyCpuBus::reset_cart() {
  BOOT_BITn_h.reset(REG_D1C1);
  PIN_CPU_BOOTp.reset(REG_D0C0);
}

void GateBoyCpuBus::set_addr(uint16_t cpu_addr) {
  BUS_CPU_A[ 0] = wire((cpu_addr >>  0) & 1);
  BUS_CPU_A[ 1] = wire((cpu_addr >>  1) & 1);
  BUS_CPU_A[ 2] = wire((cpu_addr >>  2) & 1);
  BUS_CPU_A[ 3] = wire((cpu_addr >>  3) & 1);
  BUS_CPU_A[ 4] = wire((cpu_addr >>  4) & 1);
  BUS_CPU_A[ 5] = wire((cpu_addr >>  5) & 1);
  BUS_CPU_A[ 6] = wire((cpu_addr >>  6) & 1);
  BUS_CPU_A[ 7] = wire((cpu_addr >>  7) & 1);
  BUS_CPU_A[ 8] = wire((cpu_addr >>  8) & 1);
  BUS_CPU_A[ 9] = wire((cpu_addr >>  9) & 1);
  BUS_CPU_A[10] = wire((cpu_addr >> 10) & 1);
  BUS_CPU_A[11] = wire((cpu_addr >> 11) & 1);
  BUS_CPU_A[12] = wire((cpu_addr >> 12) & 1);
  BUS_CPU_A[13] = wire((cpu_addr >> 13) & 1);
  BUS_CPU_A[14] = wire((cpu_addr >> 14) & 1);
  BUS_CPU_A[15] = wire((cpu_addr >> 15) & 1);
}

void GateBoyCpuBus::set_data(wire BUS_CPU_OEp, uint8_t cpu_data) {
  BUS_CPU_D[0] = wire(!BUS_CPU_OEp || (cpu_data >> 0) & 1);
  BUS_CPU_D[1] = wire(!BUS_CPU_OEp || (cpu_data >> 1) & 1);
  BUS_CPU_D[2] = wire(!BUS_CPU_OEp || (cpu_data >> 2) & 1);
  BUS_CPU_D[3] = wire(!BUS_CPU_OEp || (cpu_data >> 3) & 1);
  BUS_CPU_D[4] = wire(!BUS_CPU_OEp || (cpu_data >> 4) & 1);
  BUS_CPU_D[5] = wire(!BUS_CPU_OEp || (cpu_data >> 5) & 1);
  BUS_CPU_D[6] = wire(!BUS_CPU_OEp || (cpu_data >> 6) & 1);
  BUS_CPU_D[7] = wire(!BUS_CPU_OEp || (cpu_data >> 7) & 1);

  BUS_CPU_D_out[0].reset(REG_D1C0);
  BUS_CPU_D_out[1].reset(REG_D1C0);
  BUS_CPU_D_out[2].reset(REG_D1C0);
  BUS_CPU_D_out[3].reset(REG_D1C0);
  BUS_CPU_D_out[4].reset(REG_D1C0);
  BUS_CPU_D_out[5].reset(REG_D1C0);
  BUS_CPU_D_out[6].reset(REG_D1C0);
  BUS_CPU_D_out[7].reset(REG_D1C0);
}

void GateBoyCpuBus::tock(const GateBoyResetDebug& rst, const GateBoyClock& clk) {
  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_EXT_RDn.qn_new()*/ 0, PIN_CPU_RDp.qp_new()); // Ignoring debug stuff for now
  /* p07.TEDO*/ wire _TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(clk.ADAR_ABCxxxxH(), clk.ATYP_ABCDxxxx());
  /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(PIN_CPU_WRp.qp_new(), _AFAS_xxxxEFGx);
  /* p01.APOV*/ wire _APOV_CPU_WRp = not1(_AREV_CPU_WRn);

  /* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx

  TEDO_CPU_RDp = _TEDO_CPU_RDp;
  APOV_CPU_WRp = _APOV_CPU_WRp;
  TAPU_CPU_WRp = _TAPU_CPU_WRp;
}
