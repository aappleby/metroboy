#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyLCD.h"

void RegStat::tock(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  wire ACYL_SCANNINGp,
  wire XYMU_RENDERINGp,
  wire PARU_VBLANKp,
  const RegLYC& reg_lyc,

  BusOut BUS_CPU_D_out[8])
{
  // FF41 STAT
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VARY_FF41p_ext(BUS_CPU_A));
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_clk = not1(_SEPA_FF41_WRp_clk);
  /* p21.ROXE*/ ROXE_STAT_HBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
  /* p21.RUFO*/ RUFO_STAT_VBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
  /* p21.REFE*/ REFE_STAT_OAI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
  /* p21.RUGU*/ RUGU_STAT_LYI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);

  /* p21.RYJU*/ wire _RYJU_FF41_WRn_clk_evn = not1(_SEPA_FF41_WRp_clk);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST_new_evn = or2(WESY_SYS_RSTn(AVOR_SYS_RSTp), _RYJU_FF41_WRn_clk_evn);
  /* p21.RUPO*/ RUPO_STAT_LYC_MATCHn_evn.nor_latch(_PAGO_LYC_MATCH_RST_new_evn, reg_lyc.ROPO_LY_MATCH_SYNCp_c.qp_new());

  // FF41 STAT
  /* p21.SADU*/ wire _SADU_STAT_MODE0n_new  = nor2(XYMU_RENDERINGp, PARU_VBLANKp); // die NOR
  /* p21.XATY*/ wire _XATY_STAT_MODE1n_new  = nor2(ACYL_SCANNINGp, XYMU_RENDERINGp); // die NOR

  /* p21.TOBE*/ wire _TOBE_FF41_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VARY_FF41p_ext(BUS_CPU_A));
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_ext = not1(_TOBE_FF41_RDp_ext);
  /*#p21.TEBY*/ BUS_CPU_D_out[0].tri6_pn(_TOBE_FF41_RDp_ext, _SADU_STAT_MODE0n_new);
  /*#p21.WUGA*/ BUS_CPU_D_out[1].tri6_pn(_TOBE_FF41_RDp_ext, _XATY_STAT_MODE1n_new);
  /*#p21.SEGO*/ BUS_CPU_D_out[2].tri6_pn(_TOBE_FF41_RDp_ext, RUPO_STAT_LYC_MATCHn_evn.qp_new());
  /* p21.PUZO*/ BUS_CPU_D_out[3].tri6_nn(_VAVE_FF41_RDn_ext, ROXE_STAT_HBI_ENn_h.qp_new());
  /* p21.POFO*/ BUS_CPU_D_out[4].tri6_nn(_VAVE_FF41_RDn_ext, RUFO_STAT_VBI_ENn_h.qp_new());
  /* p21.SASY*/ BUS_CPU_D_out[5].tri6_nn(_VAVE_FF41_RDn_ext, REFE_STAT_OAI_ENn_h.qp_new());
  /* p21.POTE*/ BUS_CPU_D_out[6].tri6_nn(_VAVE_FF41_RDn_ext, RUGU_STAT_LYI_ENn_h.qp_new());
}
