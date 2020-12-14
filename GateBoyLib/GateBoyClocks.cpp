#include "GateBoyLib/GateBoyClocks.h"

#if 1
void GateBoyResetDbg::tock2(wire BUS_CPU_A[16],
                            wire BUS_CPU_D[8],
                            wire TEDO_CPU_RDp,
                            wire TAPU_CPU_WRp,
                            BusOut BUS_CPU_D_out[8])
{
  // FF40 LCDC
  /* p23.WARU*/ wire _WARU_FF40_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VOCA_FF40p_ext(BUS_CPU_A));
  /* p23.XUBO*/ wire _XUBO_FF40_WRn_clk = not1(_WARU_FF40_WRp_clk);
  /* p23.XONA*/ XONA_LCDC_LCDENn_h .dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(), BUS_CPU_D[7]);

  // FF40 LCDC
  /* p23.VYRE*/ wire _VYRE_FF40_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VOCA_FF40p_ext(BUS_CPU_A));
  /* p23.WYCE*/ wire _WYCE_FF40_RDn_ext = not1(_VYRE_FF40_RDp_ext);
  /*#p23.XEBU*/ BUS_CPU_D_out[7].tri6_nn(_WYCE_FF40_RDn_ext, XONA_LCDC_LCDENn_h.qp_new());
}
#endif