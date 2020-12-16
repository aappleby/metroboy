#include "GateBoyLib/GateBoyPixPipe.h"

#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoySpriteFetcher.h"
#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyOamBus.h"

//------------------------------------------------------------------------------------------------------------------------

void RegWY::tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  // FF4A WY
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VYGA_FF4Ap());
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn = not1(_WEKO_FF4A_WRp);
  /* p23.NESO*/ NESO_WY0n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp());
  /* p23.NYRO*/ NYRO_WY1n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp());
  /* p23.NAGA*/ NAGA_WY2n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp());
  /* p23.MELA*/ MELA_WY3n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp());
  /* p23.NULO*/ NULO_WY4n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp());
  /* p23.NENE*/ NENE_WY5n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp());
  /* p23.NUKA*/ NUKA_WY6n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp());
  /* p23.NAFU*/ NAFU_WY7n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp());

  // FF4A WY
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VYGA_FF4Ap());
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn = not1(_WAXU_FF4A_RDp);
  /*#p23.PUNU*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_VOMY_FF4A_RDn, NESO_WY0n.qp_new());
  /* p23.PODA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_VOMY_FF4A_RDn, NYRO_WY1n.qp_new());
  /* p23.PYGU*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_VOMY_FF4A_RDn, NAGA_WY2n.qp_new());
  /* p23.LOKA*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VOMY_FF4A_RDn, MELA_WY3n.qp_new());
  /* p23.MEGA*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VOMY_FF4A_RDn, NULO_WY4n.qp_new());
  /* p23.PELA*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VOMY_FF4A_RDn, NENE_WY5n.qp_new());
  /* p23.POLO*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VOMY_FF4A_RDn, NUKA_WY6n.qp_new());
  /* p23.MERA*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_VOMY_FF4A_RDn, NAFU_WY7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegWX::tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  // FF4B WX
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VUMY_FF4Bp());
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn = not1(_WUZA_FF4B_WRp);
  /* p23.MYPA*/ MYPA_WX0n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp());
  /* p23.NOFE*/ NOFE_WX1n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp());
  /* p23.NOKE*/ NOKE_WX2n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp());
  /* p23.MEBY*/ MEBY_WX3n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp());
  /* p23.MYPU*/ MYPU_WX4n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp());
  /* p23.MYCE*/ MYCE_WX5n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp());
  /* p23.MUVO*/ MUVO_WX6n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp());
  /* p23.NUKU*/ NUKU_WX7n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp());

  // FF4B WX
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VUMY_FF4Bp());
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn = not1(_WYZE_FF4B_RDp);
  /*#p23.LOVA*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_VYCU_FF4B_RDn, MYPA_WX0n.qp_new());
  /* p23.MUKA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_VYCU_FF4B_RDn, NOFE_WX1n.qp_new());
  /* p23.MOKO*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_VYCU_FF4B_RDn, NOKE_WX2n.qp_new());
  /* p23.LOLE*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VYCU_FF4B_RDn, MEBY_WX3n.qp_new());
  /* p23.MELE*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VYCU_FF4B_RDn, MYPU_WX4n.qp_new());
  /* p23.MUFE*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VYCU_FF4B_RDn, MYCE_WX5n.qp_new());
  /* p23.MULY*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VYCU_FF4B_RDn, MUVO_WX6n.qp_new());
  /* p23.MARA*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_VYCU_FF4B_RDn, NUKU_WX7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegStat::tock(
  GateBoyResetDebug& rst,
  GateBoyCpuBus& cpu_bus,
  wire ACYL_SCANNINGp,
  wire XYMU_RENDERINGp,
  wire PARU_VBLANKp,
  const RegLYC& reg_lyc)
{
  // FF41 STAT
  /* p21.SEPA*/ wire _SEPA_FF41_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VARY_FF41p());
  /* p21.RYVE*/ wire _RYVE_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.ROXE*/ ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp());
  /* p21.RUFO*/ RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp());
  /* p21.REFE*/ REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp());
  /* p21.RUGU*/ RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp());

  /* p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);

  // FIXME polarity doesn't seem right, are we sure that RUPO is a nor latch and these signals are correct?
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(rst.WESY_SYS_RSTn(), _RYJU_FF41_WRn);
  /* p21.RUPO*/ RUPO_STAT_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST, reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());

  // FF41 STAT
  /*#p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(XYMU_RENDERINGp, PARU_VBLANKp);   // die NOR
  /*#p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(ACYL_SCANNINGp, XYMU_RENDERINGp); // die NOR

  /* p21.TOBE*/ wire _TOBE_FF41_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VARY_FF41p());
  /* p21.VAVE*/ wire _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp);
  /*#p21.TEBY*/ cpu_bus.BUS_CPU_D_out[0].tri6_pn(_TOBE_FF41_RDp, _SADU_STAT_MODE0n);
  /*#p21.WUGA*/ cpu_bus.BUS_CPU_D_out[1].tri6_pn(_TOBE_FF41_RDp, _XATY_STAT_MODE1n);
  /*#p21.SEGO*/ cpu_bus.BUS_CPU_D_out[2].tri6_pn(_TOBE_FF41_RDp, RUPO_STAT_LYC_MATCHn.qp_new());
  /* p21.PUZO*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VAVE_FF41_RDn, ROXE_STAT_HBI_ENn.qp_new());
  /* p21.POFO*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VAVE_FF41_RDn, RUFO_STAT_VBI_ENn.qp_new());
  /* p21.SASY*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VAVE_FF41_RDn, REFE_STAT_OAI_ENn.qp_new());
  /* p21.POTE*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VAVE_FF41_RDn, RUGU_STAT_LYI_ENn.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void WindowRegisters::tock(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  wire XYMU_RENDERINGp,
  wire TYFA_CLKPIPE_odd,
  wire NUKO_WX_MATCHp_old,
  wire ROZE_FINE_COUNT_7n_old,
  wire RYDY_WIN_HITp_old)
{
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /* p27.ROCO*/ wire _ROCO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);
  /* p27.PYCO*/ PYCO_WIN_MATCHp.dff17(_ROCO_CLKPIPE_odd, rst.XAPO_VID_RSTn(), NUKO_WX_MATCHp_old);

  /* p27.SOVY*/ SOVY_WIN_HITp.dff17(clk.ALET_xBxDxFxH(), rst.XAPO_VID_RSTn(), RYDY_WIN_HITp_old);

  /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(NUKO_WX_MATCHp_old, ROZE_FINE_COUNT_7n_old);
  /* p27.RYFA*/ RYFA_WIN_FETCHn_A.dff17(_SEGU_CLKPIPE_evn, XYMU_RENDERINGp, _PANY_WIN_FETCHn_old);
}

//------------------------------------------------------------------------------------------------------------------------

void RegBGP::tock(GateBoyCpuBus& cpu_bus)
{
  // FF47 BGP
  /* p36.VUSO*/ wire _VUSO_FF47_RDp = and2(cpu_bus.ASOT_CPU_RDp(), WERA_FF47p(cpu_bus.BUS_CPU_A));
  /* p36.VELY*/ wire _VELY_FF47_WRp = and2(cpu_bus.CUPA_CPU_WRp(), WERA_FF47p(cpu_bus.BUS_CPU_A));
  /* p36.TEPO*/ wire _TEPO_FF47_WRp = not1(_VELY_FF47_WRp);
  /* p36.TEPY*/ wire _TEPY_FF47_RDn = not1(_VUSO_FF47_RDp);

  /*#p36.PAVO*/ PAVO_BGP_D0n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[0].qp());
  /* p36.NUSY*/ NUSY_BGP_D1n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[1].qp());
  /* p36.PYLU*/ PYLU_BGP_D2n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[2].qp());
  /* p36.MAXY*/ MAXY_BGP_D3n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[3].qp());
  /* p36.MUKE*/ MUKE_BGP_D4n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[4].qp());
  /* p36.MORU*/ MORU_BGP_D5n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[5].qp());
  /* p36.MOGY*/ MOGY_BGP_D6n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[6].qp());
  /* p36.MENA*/ MENA_BGP_D7n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[7].qp());

  /*#p36.RARO*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_TEPY_FF47_RDn, PAVO_BGP_D0n.qp_new());
  /* p36.PABA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_TEPY_FF47_RDn, NUSY_BGP_D1n.qp_new());
  /* p36.REDO*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_TEPY_FF47_RDn, PYLU_BGP_D2n.qp_new());
  /* p36.LOBE*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_TEPY_FF47_RDn, MAXY_BGP_D3n.qp_new());
  /* p36.LACE*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_TEPY_FF47_RDn, MUKE_BGP_D4n.qp_new());
  /* p36.LYKA*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_TEPY_FF47_RDn, MORU_BGP_D5n.qp_new());
  /* p36.LODY*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_TEPY_FF47_RDn, MOGY_BGP_D6n.qp_new());
  /* p36.LARY*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_TEPY_FF47_RDn, MENA_BGP_D7n.qp_new());
}

void RegOBP0::tock(GateBoyCpuBus& cpu_bus)
{
  // FF48 OBP0
  /* p36.XUFY*/ wire _XUFY_FF48_RDp = and2(cpu_bus.ASOT_CPU_RDp(), XAYO_FF48p(cpu_bus.BUS_CPU_A));
  /* p36.XOMA*/ wire _XOMA_FF48_WRp = and2(cpu_bus.CUPA_CPU_WRp(), XAYO_FF48p(cpu_bus.BUS_CPU_A));
  /* p36.XELO*/ wire _XELO_FF48_WRn = not1(_XOMA_FF48_WRp);
  /* p36.XOZY*/ wire _XOZY_FF48_RDn = not1(_XUFY_FF48_RDp);

  /* p36.XUFU*/ XUFU_OBP0_D0n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[0].qp());
  /* p36.XUKY*/ XUKY_OBP0_D1n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[1].qp());
  /* p36.XOVA*/ XOVA_OBP0_D2n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[2].qp());
  /* p36.XALO*/ XALO_OBP0_D3n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[3].qp());
  /* p36.XERU*/ XERU_OBP0_D4n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[4].qp());
  /* p36.XYZE*/ XYZE_OBP0_D5n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[5].qp());
  /* p36.XUPO*/ XUPO_OBP0_D6n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[6].qp());
  /* p36.XANA*/ XANA_OBP0_D7n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[7].qp());

  /*#p36.XARY*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_XOZY_FF48_RDn, XUFU_OBP0_D0n.qp_new());
  /* p36.XOKE*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_XOZY_FF48_RDn, XUKY_OBP0_D1n.qp_new());
  /* p36.XUNO*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_XOZY_FF48_RDn, XOVA_OBP0_D2n.qp_new());
  /* p36.XUBY*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_XOZY_FF48_RDn, XALO_OBP0_D3n.qp_new());
  /* p36.XAJU*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_XOZY_FF48_RDn, XERU_OBP0_D4n.qp_new());
  /* p36.XOBO*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_XOZY_FF48_RDn, XYZE_OBP0_D5n.qp_new());
  /* p36.XAXA*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_XOZY_FF48_RDn, XUPO_OBP0_D6n.qp_new());
  /* p36.XAWO*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_XOZY_FF48_RDn, XANA_OBP0_D7n.qp_new());
}

void RegOBP1::tock(GateBoyCpuBus& cpu_bus)
{
  // FF49 OBP1
  /* p36.MUMY*/ wire _MUMY_FF49_RDp = and2(cpu_bus.ASOT_CPU_RDp(), TEGO_FF49p(cpu_bus.BUS_CPU_A));
  /* p36.MYXE*/ wire _MYXE_FF49_WRp = and2(cpu_bus.CUPA_CPU_WRp(), TEGO_FF49p(cpu_bus.BUS_CPU_A));
  /* p36.LEHO*/ wire _LEHO_FF49_WRn = not1(_MYXE_FF49_WRp);
  /* p36.LOTE*/ wire _LOTE_FF49_RDn = not1(_MUMY_FF49_RDp);

  /* p36.MOXY*/ MOXY_OBP1_D0n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[0].qp());
  /* p36.LAWO*/ LAWO_OBP1_D1n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[1].qp());
  /* p36.MOSA*/ MOSA_OBP1_D2n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[2].qp());
  /* p36.LOSE*/ LOSE_OBP1_D3n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[3].qp());
  /* p36.LUNE*/ LUNE_OBP1_D4n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[4].qp());
  /* p36.LUGU*/ LUGU_OBP1_D5n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[5].qp());
  /* p36.LEPU*/ LEPU_OBP1_D6n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[6].qp());
  /* p36.LUXO*/ LUXO_OBP1_D7n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[7].qp());

  /*#p36.LAJU*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_LOTE_FF49_RDn, MOXY_OBP1_D0n.qp_new());
  /* p36.LEPA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_LOTE_FF49_RDn, LAWO_OBP1_D1n.qp_new());
  /* p36.LODE*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_LOTE_FF49_RDn, MOSA_OBP1_D2n.qp_new());
  /* p36.LYZA*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_LOTE_FF49_RDn, LOSE_OBP1_D3n.qp_new());
  /* p36.LUKY*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_LOTE_FF49_RDn, LUNE_OBP1_D4n.qp_new());
  /* p36.LUGA*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_LOTE_FF49_RDn, LUGU_OBP1_D5n.qp_new());
  /* p36.LEBA*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_LOTE_FF49_RDn, LEPU_OBP1_D6n.qp_new());
  /* p36.LELU*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_LOTE_FF49_RDn, LUXO_OBP1_D7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock(
  SpriteFetcher& sprite_fetcher,
  TileTempA& tile_temp_a,
  TileTempB& tile_temp_b,
  SpriteTempA& sprite_temp_a,
  SpriteTempB& sprite_temp_b,
  OamTempB& oam_temp_b,
  RegLCDC& reg_lcdc,
  RegBGP& reg_bgp,
  RegOBP0& reg_obp0,
  RegOBP1& reg_obp1,
  //wire XYMU_RENDERINGp,
  wire SACU_CLKPIPE_evn,
  wire NYXU_BFETCH_RSTn)
{
  wire _VYPO_VCC = 1;
  wire GND = 0;

  // Sprite temp loads  on AB/CD/EF/GH
  // Tile   temp loads  on AB/CD/EF/GH
  // Pipes       shifts on HA/BC/DE/FG
  // WUTY        fires  on HA/BC/DE/FG
  // NYXU        fires  on HA/BC/DE/FG

  // The pipes aren't shifting when we load a sprite, so it's OK to use the old value from the sprite pipes to construct the mask.
  /* p29.XEFY*/ wire _XEPY_SPRITE_DONEn  = not1(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp());
  /* p34.MEFU*/ wire _MEFU_SPRITE_MASK0n = or3(_XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp_old(), NURO_SPR_PIPE_A0.qp_old());
  /* p34.MEVE*/ wire _MEVE_SPRITE_MASK1n = or3(_XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_B1.qp_old(), MASO_SPR_PIPE_A1.qp_old());
  /* p34.MYZO*/ wire _MYZO_SPRITE_MASK2n = or3(_XEPY_SPRITE_DONEn, NATY_SPR_PIPE_B2.qp_old(), LEFE_SPR_PIPE_A2.qp_old());
  /* p34.RUDA*/ wire _RUDA_SPRITE_MASK3n = or3(_XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_B3.qp_old(), LESU_SPR_PIPE_A3.qp_old());
  /* p34.VOTO*/ wire _VOTO_SPRITE_MASK4n = or3(_XEPY_SPRITE_DONEn, VARE_SPR_PIPE_B4.qp_old(), WYHO_SPR_PIPE_A4.qp_old());
  /* p34.VYSA*/ wire _VYSA_SPRITE_MASK5n = or3(_XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_B5.qp_old(), WORA_SPR_PIPE_A5.qp_old());
  /* p34.TORY*/ wire _TORY_SPRITE_MASK6n = or3(_XEPY_SPRITE_DONEn, VANU_SPR_PIPE_B6.qp_old(), VAFO_SPR_PIPE_A6.qp_old());
  /* p34.WOPE*/ wire _WOPE_SPRITE_MASK7n = or3(_XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_B7.qp_old(), WUFY_SPR_PIPE_A7.qp_old());

  /* p34.LESY*/ wire _LESY_SPRITE_MASK0p = not1(_MEFU_SPRITE_MASK0n);
  /* p34.LOTA*/ wire _LOTA_SPRITE_MASK1p = not1(_MEVE_SPRITE_MASK1n);
  /* p34.LYKU*/ wire _LYKU_SPRITE_MASK2p = not1(_MYZO_SPRITE_MASK2n);
  /* p34.ROBY*/ wire _ROBY_SPRITE_MASK3p = not1(_RUDA_SPRITE_MASK3n);
  /* p34.TYTA*/ wire _TYTA_SPRITE_MASK4p = not1(_VOTO_SPRITE_MASK4n);
  /* p34.TYCO*/ wire _TYCO_SPRITE_MASK5p = not1(_VYSA_SPRITE_MASK5n);
  /* p34.SOKA*/ wire _SOKA_SPRITE_MASK6p = not1(_TORY_SPRITE_MASK6n);
  /* p34.XOVU*/ wire _XOVU_SPRITE_MASK7p = not1(_WOPE_SPRITE_MASK7n);


  // Reload all the pipes using set/rst
  {
    /* p32.LOZE*/ wire _LOZE_PIPE_A_LOADp = not1(NYXU_BFETCH_RSTn);
    /* p32.LAKY*/ wire _LAKY_BG_PIX_SET0 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.LEGU_TILE_DA0n.qn_new());
    /* p32.NYXO*/ wire _NYXO_BG_PIX_SET1 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.NUDU_TILE_DA1n.qn_new());
    /* p32.LOTO*/ wire _LOTO_BG_PIX_SET2 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.MUKU_TILE_DA2n.qn_new());
    /* p32.LYDU*/ wire _LYDU_BG_PIX_SET3 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.LUZO_TILE_DA3n.qn_new());
    /* p32.MYVY*/ wire _MYVY_BG_PIX_SET4 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.MEGU_TILE_DA4n.qn_new());
    /* p32.LODO*/ wire _LODO_BG_PIX_SET5 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.MYJY_TILE_DA5n.qn_new());
    /* p32.NUTE*/ wire _NUTE_BG_PIX_SET6 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.NASA_TILE_DA6n.qn_new());
    /* p32.NAJA*/ wire _NAJA_BG_PIX_SET7 = nand2(_LOZE_PIPE_A_LOADp, tile_temp_a.NEFO_TILE_DA7n.qn_new());

    /* p32.LUHE*/ wire _LUHE_BG_PIX_DA0n = not1(tile_temp_a.LEGU_TILE_DA0n.qn_new());
    /* p32.NOLY*/ wire _NOLY_BG_PIX_DA1n = not1(tile_temp_a.NUDU_TILE_DA1n.qn_new());
    /* p32.LEKE*/ wire _LEKE_BG_PIX_DA2n = not1(tile_temp_a.MUKU_TILE_DA2n.qn_new());
    /* p32.LOMY*/ wire _LOMY_BG_PIX_DA3n = not1(tile_temp_a.LUZO_TILE_DA3n.qn_new());
    /* p32.LALA*/ wire _LALA_BG_PIX_DA4n = not1(tile_temp_a.MEGU_TILE_DA4n.qn_new());
    /* p32.LOXA*/ wire _LOXA_BG_PIX_DA5n = not1(tile_temp_a.MYJY_TILE_DA5n.qn_new());
    /* p32.NEZE*/ wire _NEZE_BG_PIX_DA6n = not1(tile_temp_a.NASA_TILE_DA6n.qn_new());
    /* p32.NOBO*/ wire _NOBO_BG_PIX_DA7n = not1(tile_temp_a.NEFO_TILE_DA7n.qn_new());

    /* p32.LOTY*/ wire _LOTY_BG_PIX_RST0 = nand2(_LOZE_PIPE_A_LOADp, _LUHE_BG_PIX_DA0n);
    /* p32.NEXA*/ wire _NEXA_BG_PIX_RST1 = nand2(_LOZE_PIPE_A_LOADp, _NOLY_BG_PIX_DA1n);
    /* p32.LUTU*/ wire _LUTU_BG_PIX_RST2 = nand2(_LOZE_PIPE_A_LOADp, _LEKE_BG_PIX_DA2n);
    /* p32.LUJA*/ wire _LUJA_BG_PIX_RST3 = nand2(_LOZE_PIPE_A_LOADp, _LOMY_BG_PIX_DA3n);
    /* p32.MOSY*/ wire _MOSY_BG_PIX_RST4 = nand2(_LOZE_PIPE_A_LOADp, _LALA_BG_PIX_DA4n);
    /* p32.LERU*/ wire _LERU_BG_PIX_RST5 = nand2(_LOZE_PIPE_A_LOADp, _LOXA_BG_PIX_DA5n);
    /* p32.NYHA*/ wire _NYHA_BG_PIX_RST6 = nand2(_LOZE_PIPE_A_LOADp, _NEZE_BG_PIX_DA6n);
    /* p32.NADY*/ wire _NADY_BG_PIX_RST7 = nand2(_LOZE_PIPE_A_LOADp, _NOBO_BG_PIX_DA7n);

    /* p32.PYBO*/ PYBO_BGW_PIPE_A7.dff22(SACU_CLKPIPE_evn, _NAJA_BG_PIX_SET7, _NADY_BG_PIX_RST7, NEDA_BGW_PIPE_A6.qp_old());
    /* p32.NEDA*/ NEDA_BGW_PIPE_A6.dff22(SACU_CLKPIPE_evn, _NUTE_BG_PIX_SET6, _NYHA_BG_PIX_RST6, MODU_BGW_PIPE_A5.qp_old());
    /* p32.MODU*/ MODU_BGW_PIPE_A5.dff22(SACU_CLKPIPE_evn, _LODO_BG_PIX_SET5, _LERU_BG_PIX_RST5, NEPO_BGW_PIPE_A4.qp_old());
    /* p32.NEPO*/ NEPO_BGW_PIPE_A4.dff22(SACU_CLKPIPE_evn, _MYVY_BG_PIX_SET4, _MOSY_BG_PIX_RST4, MACU_BGW_PIPE_A3.qp_old());
    /* p32.MACU*/ MACU_BGW_PIPE_A3.dff22(SACU_CLKPIPE_evn, _LYDU_BG_PIX_SET3, _LUJA_BG_PIX_RST3, MOJU_BGW_PIPE_A2.qp_old());
    /* p32.MOJU*/ MOJU_BGW_PIPE_A2.dff22(SACU_CLKPIPE_evn, _LOTO_BG_PIX_SET2, _LUTU_BG_PIX_RST2, NOZO_BGW_PIPE_A1.qp_old());
    /* p32.NOZO*/ NOZO_BGW_PIPE_A1.dff22(SACU_CLKPIPE_evn, _NYXO_BG_PIX_SET1, _NEXA_BG_PIX_RST1, MYDE_BGW_PIPE_A0.qp_old());
    /* p32.MYDE*/ MYDE_BGW_PIPE_A0.dff22(SACU_CLKPIPE_evn, _LAKY_BG_PIX_SET0, _LOTY_BG_PIX_RST0, GND);
  }

  {
    /* p32.LUXA*/ wire _LUXA_PIPE_B_LOADp = not1(NYXU_BFETCH_RSTn);
    /* p32.TUXE*/ wire _TUXE_BG_PIX_SET0 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /* p32.SOLY*/ wire _SOLY_BG_PIX_SET1 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.POZO_TILE_DB1p.qp_new());
    /* p32.RUCE*/ wire _RUCE_BG_PIX_SET2 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /* p32.RYJA*/ wire _RYJA_BG_PIX_SET3 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.POXA_TILE_DB3p.qp_new());
    /* p32.RUTO*/ wire _RUTO_BG_PIX_SET4 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.PULO_TILE_DB4p.qp_new());
    /* p32.RAJA*/ wire _RAJA_BG_PIX_SET5 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.POJU_TILE_DB5p.qp_new());
    /* p32.RAJO*/ wire _RAJO_BG_PIX_SET6 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.POWY_TILE_DB6p.qp_new());
    /* p32.RAGA*/ wire _RAGA_BG_PIX_SET7 = nand2(_LUXA_PIPE_B_LOADp, tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /* p32.TOSA*/ wire _TOSA_BG_PIX_DB0n = not1(tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /* p32.RUCO*/ wire _RUCO_BG_PIX_DB1n = not1(tile_temp_b.POZO_TILE_DB1p.qp_new());
    /* p32.TYCE*/ wire _TYCE_BG_PIX_DB2n = not1(tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /* p32.REVY*/ wire _REVY_BG_PIX_DB3n = not1(tile_temp_b.POXA_TILE_DB3p.qp_new());
    /* p32.RYGA*/ wire _RYGA_BG_PIX_DB4n = not1(tile_temp_b.PULO_TILE_DB4p.qp_new());
    /* p32.RYLE*/ wire _RYLE_BG_PIX_DB5n = not1(tile_temp_b.POJU_TILE_DB5p.qp_new());
    /* p32.RAPU*/ wire _RAPU_BG_PIX_DB6n = not1(tile_temp_b.POWY_TILE_DB6p.qp_new());
    /* p32.SOJA*/ wire _SOJA_BG_PIX_DB7n = not1(tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /* p32.SEJA*/ wire _SEJA_BG_PIX_RST0 = nand2(_LUXA_PIPE_B_LOADp, _TOSA_BG_PIX_DB0n);
    /* p32.SENO*/ wire _SENO_BG_PIX_RST1 = nand2(_LUXA_PIPE_B_LOADp, _RUCO_BG_PIX_DB1n);
    /* p32.SURE*/ wire _SURE_BG_PIX_RST2 = nand2(_LUXA_PIPE_B_LOADp, _TYCE_BG_PIX_DB2n);
    /* p32.SEBO*/ wire _SEBO_BG_PIX_RST3 = nand2(_LUXA_PIPE_B_LOADp, _REVY_BG_PIX_DB3n);
    /* p32.SUCA*/ wire _SUCA_BG_PIX_RST4 = nand2(_LUXA_PIPE_B_LOADp, _RYGA_BG_PIX_DB4n);
    /* p32.SYWE*/ wire _SYWE_BG_PIX_RST5 = nand2(_LUXA_PIPE_B_LOADp, _RYLE_BG_PIX_DB5n);
    /* p32.SUPU*/ wire _SUPU_BG_PIX_RST6 = nand2(_LUXA_PIPE_B_LOADp, _RAPU_BG_PIX_DB6n);
    /* p32.RYJY*/ wire _RYJY_BG_PIX_RST7 = nand2(_LUXA_PIPE_B_LOADp, _SOJA_BG_PIX_DB7n);

    /* p32.SOHU*/ SOHU_BGW_PIPE_B7.dff22(SACU_CLKPIPE_evn, _RAGA_BG_PIX_SET7, _RYJY_BG_PIX_RST7, RALU_BGW_PIPE_B6.qp_old());
    /* p32.RALU*/ RALU_BGW_PIPE_B6.dff22(SACU_CLKPIPE_evn, _RAJO_BG_PIX_SET6, _SUPU_BG_PIX_RST6, SETU_BGW_PIPE_B5.qp_old());
    /* p32.SETU*/ SETU_BGW_PIPE_B5.dff22(SACU_CLKPIPE_evn, _RAJA_BG_PIX_SET5, _SYWE_BG_PIX_RST5, SOBO_BGW_PIPE_B4.qp_old());
    /* p32.SOBO*/ SOBO_BGW_PIPE_B4.dff22(SACU_CLKPIPE_evn, _RUTO_BG_PIX_SET4, _SUCA_BG_PIX_RST4, RYSA_BGW_PIPE_B3.qp_old());
    /* p32.RYSA*/ RYSA_BGW_PIPE_B3.dff22(SACU_CLKPIPE_evn, _RYJA_BG_PIX_SET3, _SEBO_BG_PIX_RST3, SADY_BGW_PIPE_B2.qp_old());
    /* p32.SADY*/ SADY_BGW_PIPE_B2.dff22(SACU_CLKPIPE_evn, _RUCE_BG_PIX_SET2, _SURE_BG_PIX_RST2, TACA_BGW_PIPE_B1.qp_old());
    /* p32.TACA*/ TACA_BGW_PIPE_B1.dff22(SACU_CLKPIPE_evn, _SOLY_BG_PIX_SET1, _SENO_BG_PIX_RST1, TOMY_BGW_PIPE_B0.qp_old());
    /* p32.TOMY*/ TOMY_BGW_PIPE_B0.dff22(SACU_CLKPIPE_evn, _TUXE_BG_PIX_SET0, _SEJA_BG_PIX_RST0, GND);
  }

  // Sprite pipe A
  {
    /* p33.PABE*/ wire _PABE_SPR_PIX_SET0 = nand2(_LESY_SPRITE_MASK0p, sprite_temp_a.REWO_SPRITE_DA0n.qn_new());
    /* p33.MYTO*/ wire _MYTO_SPR_PIX_SET1 = nand2(_LOTA_SPRITE_MASK1p, sprite_temp_a.PEBA_SPRITE_DA1n.qn_new());
    /* p33.LELA*/ wire _LELA_SPR_PIX_SET2 = nand2(_LYKU_SPRITE_MASK2p, sprite_temp_a.MOFO_SPRITE_DA2n.qn_new());
    /* p33.MAME*/ wire _MAME_SPR_PIX_SET3 = nand2(_ROBY_SPRITE_MASK3p, sprite_temp_a.PUDU_SPRITE_DA3n.qn_new());
    /* p33.VEXU*/ wire _VEXU_SPR_PIX_SET4 = nand2(_TYTA_SPRITE_MASK4p, sprite_temp_a.SAJA_SPRITE_DA4n.qn_new());
    /* p33.VABY*/ wire _VABY_SPR_PIX_SET5 = nand2(_TYCO_SPRITE_MASK5p, sprite_temp_a.SUNY_SPRITE_DA5n.qn_new());
    /* p33.TUXA*/ wire _TUXA_SPR_PIX_SET6 = nand2(_SOKA_SPRITE_MASK6p, sprite_temp_a.SEMO_SPRITE_DA6n.qn_new());
    /* p33.VUNE*/ wire _VUNE_SPR_PIX_SET7 = nand2(_XOVU_SPRITE_MASK7p, sprite_temp_a.SEGA_SPRITE_DA7n.qn_new());

    /* p33.RATA*/ wire _RATA_SPR_PIX_DA0n = not1(sprite_temp_a.REWO_SPRITE_DA0n.qn_new());
    /* p33.NUCA*/ wire _NUCA_SPR_PIX_DA1n = not1(sprite_temp_a.PEBA_SPRITE_DA1n.qn_new());
    /* p33.LASE*/ wire _LASE_SPR_PIX_DA2n = not1(sprite_temp_a.MOFO_SPRITE_DA2n.qn_new());
    /* p33.LUBO*/ wire _LUBO_SPR_PIX_DA3n = not1(sprite_temp_a.PUDU_SPRITE_DA3n.qn_new());
    /* p33.WERY*/ wire _WERY_SPR_PIX_DA4n = not1(sprite_temp_a.SAJA_SPRITE_DA4n.qn_new());
    /* p33.WURA*/ wire _WURA_SPR_PIX_DA5n = not1(sprite_temp_a.SUNY_SPRITE_DA5n.qn_new());
    /* p33.SULU*/ wire _SULU_SPR_PIX_DA6n = not1(sprite_temp_a.SEMO_SPRITE_DA6n.qn_new());
    /* p33.WAMY*/ wire _WAMY_SPR_PIX_DA7n = not1(sprite_temp_a.SEGA_SPRITE_DA7n.qn_new());

    /* p33.PYZU*/ wire _PYZU_SPR_PIX_RST0 = nand2(_LESY_SPRITE_MASK0p, _RATA_SPR_PIX_DA0n);
    /* p33.MADA*/ wire _MADA_SPR_PIX_RST1 = nand2(_LOTA_SPRITE_MASK1p, _NUCA_SPR_PIX_DA1n);
    /* p33.LYDE*/ wire _LYDE_SPR_PIX_RST2 = nand2(_LYKU_SPRITE_MASK2p, _LASE_SPR_PIX_DA2n);
    /* p33.LUFY*/ wire _LUFY_SPR_PIX_RST3 = nand2(_ROBY_SPRITE_MASK3p, _LUBO_SPR_PIX_DA3n);
    /* p33.XATO*/ wire _XATO_SPR_PIX_RST4 = nand2(_TYTA_SPRITE_MASK4p, _WERY_SPR_PIX_DA4n);
    /* p33.XEXU*/ wire _XEXU_SPR_PIX_RST5 = nand2(_TYCO_SPRITE_MASK5p, _WURA_SPR_PIX_DA5n);
    /* p33.TUPE*/ wire _TUPE_SPR_PIX_RST6 = nand2(_SOKA_SPRITE_MASK6p, _SULU_SPR_PIX_DA6n);
    /* p33.XYVE*/ wire _XYVE_SPR_PIX_RST7 = nand2(_XOVU_SPRITE_MASK7p, _WAMY_SPR_PIX_DA7n);

    /* p33.WUFY*/ WUFY_SPR_PIPE_A7.dff22(SACU_CLKPIPE_evn, _VUNE_SPR_PIX_SET7, _XYVE_SPR_PIX_RST7, VAFO_SPR_PIPE_A6.qp_old());
    /* p33.VAFO*/ VAFO_SPR_PIPE_A6.dff22(SACU_CLKPIPE_evn, _TUXA_SPR_PIX_SET6, _TUPE_SPR_PIX_RST6, WORA_SPR_PIPE_A5.qp_old());
    /* p33.WORA*/ WORA_SPR_PIPE_A5.dff22(SACU_CLKPIPE_evn, _VABY_SPR_PIX_SET5, _XEXU_SPR_PIX_RST5, WYHO_SPR_PIPE_A4.qp_old());
    /* p33.WYHO*/ WYHO_SPR_PIPE_A4.dff22(SACU_CLKPIPE_evn, _VEXU_SPR_PIX_SET4, _XATO_SPR_PIX_RST4, LESU_SPR_PIPE_A3.qp_old());
    /* p33.LESU*/ LESU_SPR_PIPE_A3.dff22(SACU_CLKPIPE_evn, _MAME_SPR_PIX_SET3, _LUFY_SPR_PIX_RST3, LEFE_SPR_PIPE_A2.qp_old());
    /* p33.LEFE*/ LEFE_SPR_PIPE_A2.dff22(SACU_CLKPIPE_evn, _LELA_SPR_PIX_SET2, _LYDE_SPR_PIX_RST2, MASO_SPR_PIPE_A1.qp_old());
    /* p33.MASO*/ MASO_SPR_PIPE_A1.dff22(SACU_CLKPIPE_evn, _MYTO_SPR_PIX_SET1, _MADA_SPR_PIX_RST1, NURO_SPR_PIPE_A0.qp_old());
    /* p33.NURO*/ NURO_SPR_PIPE_A0.dff22(SACU_CLKPIPE_evn, _PABE_SPR_PIX_SET0, _PYZU_SPR_PIX_RST0, GND);
  }

  // Sprite pipe B
  {
    /* p33.MEZU*/ wire _MEZU_SPR_PIX_SET0 = nand2(_LESY_SPRITE_MASK0p, sprite_temp_b.PEFO_SPRITE_DB0n.qn_new());
    /* p33.RUSY*/ wire _RUSY_SPR_PIX_SET1 = nand2(_LOTA_SPRITE_MASK1p, sprite_temp_b.ROKA_SPRITE_DB1n.qn_new());
    /* p33.MYXA*/ wire _MYXA_SPR_PIX_SET2 = nand2(_LYKU_SPRITE_MASK2p, sprite_temp_b.MYTU_SPRITE_DB2n.qn_new());
    /* p33.RANO*/ wire _RANO_SPR_PIX_SET3 = nand2(_ROBY_SPRITE_MASK3p, sprite_temp_b.RAMU_SPRITE_DB3n.qn_new());
    /* p33.TYGA*/ wire _TYGA_SPR_PIX_SET4 = nand2(_TYTA_SPRITE_MASK4p, sprite_temp_b.SELE_SPRITE_DB4n.qn_new());
    /* p33.VUME*/ wire _VUME_SPR_PIX_SET5 = nand2(_TYCO_SPRITE_MASK5p, sprite_temp_b.SUTO_SPRITE_DB5n.qn_new());
    /* p33.TAPO*/ wire _TAPO_SPR_PIX_SET6 = nand2(_SOKA_SPRITE_MASK6p, sprite_temp_b.RAMA_SPRITE_DB6n.qn_new());
    /* p33.TESO*/ wire _TESO_SPR_PIX_SET7 = nand2(_XOVU_SPRITE_MASK7p, sprite_temp_b.RYDU_SPRITE_DB7n.qn_new());

    /* p33.LOZA*/ wire _LOZA_SPR_PIX_DB0n = not1(sprite_temp_b.PEFO_SPRITE_DB0n.qn_new());
    /* p33.SYBO*/ wire _SYBO_SPR_PIX_DB1n = not1(sprite_temp_b.ROKA_SPRITE_DB1n.qn_new());
    /* p33.LUMO*/ wire _LUMO_SPR_PIX_DB2n = not1(sprite_temp_b.MYTU_SPRITE_DB2n.qn_new());
    /* p33.SOLO*/ wire _SOLO_SPR_PIX_DB3n = not1(sprite_temp_b.RAMU_SPRITE_DB3n.qn_new());
    /* p33.VOBY*/ wire _VOBY_SPR_PIX_DB4n = not1(sprite_temp_b.SELE_SPRITE_DB4n.qn_new());
    /* p33.WYCO*/ wire _WYCO_SPR_PIX_DB5n = not1(sprite_temp_b.SUTO_SPRITE_DB5n.qn_new());
    /* p33.SERY*/ wire _SERY_SPR_PIX_DB6n = not1(sprite_temp_b.RAMA_SPRITE_DB6n.qn_new());
    /* p33.SELU*/ wire _SELU_SPR_PIX_DB7n = not1(sprite_temp_b.RYDU_SPRITE_DB7n.qn_new());

    /* p33.MOFY*/ wire _MOFY_SPR_PIX_RST0 = nand2(_LESY_SPRITE_MASK0p, _LOZA_SPR_PIX_DB0n);
    /* p33.RUCA*/ wire _RUCA_SPR_PIX_RST1 = nand2(_LOTA_SPRITE_MASK1p, _SYBO_SPR_PIX_DB1n);
    /* p33.MAJO*/ wire _MAJO_SPR_PIX_RST2 = nand2(_LYKU_SPRITE_MASK2p, _LUMO_SPR_PIX_DB2n);
    /* p33.REHU*/ wire _REHU_SPR_PIX_RST3 = nand2(_ROBY_SPRITE_MASK3p, _SOLO_SPR_PIX_DB3n);
    /* p33.WAXO*/ wire _WAXO_SPR_PIX_RST4 = nand2(_TYTA_SPRITE_MASK4p, _VOBY_SPR_PIX_DB4n);
    /* p33.XOLE*/ wire _XOLE_SPR_PIX_RST5 = nand2(_TYCO_SPRITE_MASK5p, _WYCO_SPR_PIX_DB5n);
    /* p33.TABY*/ wire _TABY_SPR_PIX_RST6 = nand2(_SOKA_SPRITE_MASK6p, _SERY_SPR_PIX_DB6n);
    /* p33.TULA*/ wire _TULA_SPR_PIX_RST7 = nand2(_XOVU_SPRITE_MASK7p, _SELU_SPR_PIX_DB7n);

    /* p33.VUPY*/ VUPY_SPR_PIPE_B7.dff22(SACU_CLKPIPE_evn, _TESO_SPR_PIX_SET7, _TULA_SPR_PIX_RST7, VANU_SPR_PIPE_B6.qp_old());
    /* p33.VANU*/ VANU_SPR_PIPE_B6.dff22(SACU_CLKPIPE_evn, _TAPO_SPR_PIX_SET6, _TABY_SPR_PIX_RST6, WEBA_SPR_PIPE_B5.qp_old());
    /* p33.WEBA*/ WEBA_SPR_PIPE_B5.dff22(SACU_CLKPIPE_evn, _VUME_SPR_PIX_SET5, _XOLE_SPR_PIX_RST5, VARE_SPR_PIPE_B4.qp_old());
    /* p33.VARE*/ VARE_SPR_PIPE_B4.dff22(SACU_CLKPIPE_evn, _TYGA_SPR_PIX_SET4, _WAXO_SPR_PIX_RST4, PYJO_SPR_PIPE_B3.qp_old());
    /* p33.PYJO*/ PYJO_SPR_PIPE_B3.dff22(SACU_CLKPIPE_evn, _RANO_SPR_PIX_SET3, _REHU_SPR_PIX_RST3, NATY_SPR_PIPE_B2.qp_old());
    /* p33.NATY*/ NATY_SPR_PIPE_B2.dff22(SACU_CLKPIPE_evn, _MYXA_SPR_PIX_SET2, _MAJO_SPR_PIX_RST2, PEFU_SPR_PIPE_B1.qp_old());
    /* p33.PEFU*/ PEFU_SPR_PIPE_B1.dff22(SACU_CLKPIPE_evn, _RUSY_SPR_PIX_SET1, _RUCA_SPR_PIX_RST1, NYLU_SPR_PIPE_B0.qp_old());
    /* p33.NYLU*/ NYLU_SPR_PIPE_B0.dff22(SACU_CLKPIPE_evn, _MEZU_SPR_PIX_SET0, _MOFY_SPR_PIX_RST0, GND);
  }

  {
    /* p34.PUME*/ wire _PUME_PAL_PIPE_SET0 = nand2(_LESY_SPRITE_MASK0p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.SORO*/ wire _SORO_PAL_PIPE_SET1 = nand2(_LOTA_SPRITE_MASK1p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2 = nand2(_LYKU_SPRITE_MASK2p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3 = nand2(_ROBY_SPRITE_MASK3p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.RORA*/ wire _RORA_PAL_PIPE_SET4 = nand2(_TYTA_SPRITE_MASK4p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.MENE*/ wire _MENE_PAL_PIPE_SET5 = nand2(_TYCO_SPRITE_MASK5p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6 = nand2(_SOKA_SPRITE_MASK6p, oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7 = nand2(_XOVU_SPRITE_MASK7p, oam_temp_b.GOMO_OAM_DB4p.qp_new());

    /* p34.SYPY*/ wire _SYPY_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.TOTU*/ wire _TOTU_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.NARO*/ wire _NARO_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.WEXY*/ wire _WEXY_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.RYZY*/ wire _RYZY_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.RYFE*/ wire _RYFE_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.LADY*/ wire _LADY_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /* p34.LAFY*/ wire _LAFY_PAL_PIPE_DB4n = not1(oam_temp_b.GOMO_OAM_DB4p.qp_new());

    /* p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0 = nand2(_LESY_SPRITE_MASK0p, _SYPY_PAL_PIPE_DB4n);
    /* p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1 = nand2(_LOTA_SPRITE_MASK1p, _TOTU_PAL_PIPE_DB4n);
    /* p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2 = nand2(_LYKU_SPRITE_MASK2p, _NARO_PAL_PIPE_DB4n);
    /* p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3 = nand2(_ROBY_SPRITE_MASK3p, _WEXY_PAL_PIPE_DB4n);
    /* p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4 = nand2(_TYTA_SPRITE_MASK4p, _RYZY_PAL_PIPE_DB4n);
    /* p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5 = nand2(_TYCO_SPRITE_MASK5p, _RYFE_PAL_PIPE_DB4n);
    /* p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6 = nand2(_SOKA_SPRITE_MASK6p, _LADY_PAL_PIPE_DB4n);
    /* p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7 = nand2(_XOVU_SPRITE_MASK7p, _LAFY_PAL_PIPE_DB4n);

    /* p34.LYME*/ LYME_PAL_PIPE_D7.dff22(SACU_CLKPIPE_evn, _LAMY_PAL_PIPE_SET7, _LUNU_PAL_PIPE_RST7, MODA_PAL_PIPE_D6.qp_old());
    /* p34.MODA*/ MODA_PAL_PIPE_D6.dff22(SACU_CLKPIPE_evn, _LUKE_PAL_PIPE_SET6, _LOWA_PAL_PIPE_RST6, NUKE_PAL_PIPE_D5.qp_old());
    /* p34.NUKE*/ NUKE_PAL_PIPE_D5.dff22(SACU_CLKPIPE_evn, _MENE_PAL_PIPE_SET5, _PAZO_PAL_PIPE_RST5, PALU_PAL_PIPE_D4.qp_old());
    /* p34.PALU*/ PALU_PAL_PIPE_D4.dff22(SACU_CLKPIPE_evn, _RORA_PAL_PIPE_SET4, _RUDU_PAL_PIPE_RST4, SOMY_PAL_PIPE_D3.qp_old());
    /* p34.SOMY*/ SOMY_PAL_PIPE_D3.dff22(SACU_CLKPIPE_evn, _SUKY_PAL_PIPE_SET3, _TOWA_PAL_PIPE_RST3, ROSA_PAL_PIPE_D2.qp_old());
    /* p34.ROSA*/ ROSA_PAL_PIPE_D2.dff22(SACU_CLKPIPE_evn, _PAMO_PAL_PIPE_SET2, _PYZY_PAL_PIPE_RST2, SATA_PAL_PIPE_D1.qp_old());
    /* p34.SATA*/ SATA_PAL_PIPE_D1.dff22(SACU_CLKPIPE_evn, _SORO_PAL_PIPE_SET1, _TAFA_PAL_PIPE_RST1, RUGO_PAL_PIPE_D0.qp_old());
    /* p34.RUGO*/ RUGO_PAL_PIPE_D0.dff22(SACU_CLKPIPE_evn, _PUME_PAL_PIPE_SET0, _SUCO_PAL_PIPE_RST0, GND);
  }

  {
    /* p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0 = nand2(_LESY_SPRITE_MASK0p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XALA*/ wire _XALA_MASK_PIPE_SET1 = nand2(_LOTA_SPRITE_MASK1p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2 = nand2(_LYKU_SPRITE_MASK2p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3 = nand2(_ROBY_SPRITE_MASK3p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4 = nand2(_TYTA_SPRITE_MASK4p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XELY*/ wire _XELY_MASK_PIPE_SET5 = nand2(_TYCO_SPRITE_MASK5p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6 = nand2(_SOKA_SPRITE_MASK6p, oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7 = nand2(_XOVU_SPRITE_MASK7p, oam_temp_b.DEPO_OAM_DB7p.qp_new());

    /* p26.XOGA*/ wire _XOGA_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XURA*/ wire _XURA_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.TAJO*/ wire _TAJO_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XENU*/ wire _XENU_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XYKE*/ wire _XYKE_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XABA*/ wire _XABA_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.TAFU*/ wire _TAFU_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /* p26.XUHO*/ wire _XUHO_MASK_PIPE_DB7n = not1(oam_temp_b.DEPO_OAM_DB7p.qp_new());

    /* p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0 = nand2(_LESY_SPRITE_MASK0p, _XOGA_MASK_PIPE_DB7n);
    /* p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1 = nand2(_LOTA_SPRITE_MASK1p, _XURA_MASK_PIPE_DB7n);
    /* p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2 = nand2(_LYKU_SPRITE_MASK2p, _TAJO_MASK_PIPE_DB7n);
    /* p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3 = nand2(_ROBY_SPRITE_MASK3p, _XENU_MASK_PIPE_DB7n);
    /* p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4 = nand2(_TYTA_SPRITE_MASK4p, _XYKE_MASK_PIPE_DB7n);
    /* p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5 = nand2(_TYCO_SPRITE_MASK5p, _XABA_MASK_PIPE_DB7n);
    /* p26.TENA*/ wire _TENA_MASK_PIPE_RST6 = nand2(_SOKA_SPRITE_MASK6p, _TAFU_MASK_PIPE_DB7n);
    /* p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7 = nand2(_XOVU_SPRITE_MASK7p, _XUHO_MASK_PIPE_DB7n);

    /* p26.VAVA*/ VAVA_MASK_PIPE_7.dff22(SACU_CLKPIPE_evn, _TUWU_MASK_PIPE_SET7, _WUBU_MASK_PIPE_RST7, VUMO_MASK_PIPE_6.qp_old());
    /* p26.VUMO*/ VUMO_MASK_PIPE_6.dff22(SACU_CLKPIPE_evn, _TYKO_MASK_PIPE_SET6, _TENA_MASK_PIPE_RST6, WODA_MASK_PIPE_5.qp_old());
    /* p26.WODA*/ WODA_MASK_PIPE_5.dff22(SACU_CLKPIPE_evn, _XELY_MASK_PIPE_SET5, _WUJA_MASK_PIPE_RST5, XETE_MASK_PIPE_4.qp_old());
    /* p26.XETE*/ XETE_MASK_PIPE_4.dff22(SACU_CLKPIPE_evn, _XUKU_MASK_PIPE_SET4, _WEDY_MASK_PIPE_RST4, WYFU_MASK_PIPE_3.qp_old());
    /* p26.WYFU*/ WYFU_MASK_PIPE_3.dff22(SACU_CLKPIPE_evn, _XYRU_MASK_PIPE_SET3, _WEVO_MASK_PIPE_RST3, VOSA_MASK_PIPE_2.qp_old());
    /* p26.VOSA*/ VOSA_MASK_PIPE_2.dff22(SACU_CLKPIPE_evn, _TYRA_MASK_PIPE_SET2, _TUFO_MASK_PIPE_RST2, WURU_MASK_PIPE_1.qp_old());
    /* p26.WURU*/ WURU_MASK_PIPE_1.dff22(SACU_CLKPIPE_evn, _XALA_MASK_PIPE_SET1, _WEDE_MASK_PIPE_RST1, VEZO_MASK_PIPE_0.qp_old());
    /* p26.VEZO*/ VEZO_MASK_PIPE_0.dff22(SACU_CLKPIPE_evn, _TEDE_MASK_PIPE_SET0, _WOKA_MASK_PIPE_RST0, _VYPO_VCC);
  }

  //----------------------------------------
  // Pixel output

  {
    // Pixel merge + emit
    /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp = and2(PYBO_BGW_PIPE_A7.qp_new(), reg_lcdc.VYXE_LCDC_BGENn.qn_new());
    /*#p35.TADE*/ wire TADE_PIX_BG_HIp = and2(SOHU_BGW_PIPE_B7.qp_new(), reg_lcdc.VYXE_LCDC_BGENn.qn_new());
    /*#p35.XULA*/ wire XULA_PIX_SP_LOp = and2(reg_lcdc.XYLO_LCDC_SPENn.qn_new(), WUFY_SPR_PIPE_A7.qp_new());
    /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp = and2(reg_lcdc.XYLO_LCDC_SPENn.qn_new(), VUPY_SPR_PIPE_B7.qp_new());

    /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

    /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, VAVA_MASK_PIPE_7.qp_new());
    /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, VAVA_MASK_PIPE_7.qp_new());
    /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

    /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(LYME_PAL_PIPE_D7.qp_new());
    /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
    /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(LYME_PAL_PIPE_D7.qp_new(), POKA_BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*#p35.WELE*/ wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
    /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

    /*#p35.LAVA*/ wire LAVA_MASK_OPB0  = not1(LAFU_OBP0PIXELn);

    /*#p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

    /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(reg_obp0.XANA_OBP0_D7n.qn_new(), VYRO_PAL_OBP0D,
                                                reg_obp0.XYZE_OBP0_D5n.qn_new(), VATA_PAL_OBP0C,
                                                reg_obp0.XALO_OBP0_D3n.qn_new(), VOLO_PAL_OBP0B,
                                                reg_obp0.XUKY_OBP0_D1n.qn_new(), VUGO_PAL_OBP0A);

    /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(reg_obp0.XUPO_OBP0_D6n.qn_new(), VYRO_PAL_OBP0D,
                                                reg_obp0.XERU_OBP0_D4n.qn_new(), VATA_PAL_OBP0C,
                                                reg_obp0.XOVA_OBP0_D2n.qn_new(), VOLO_PAL_OBP0B,
                                                reg_obp0.XUFU_OBP0_D0n.qn_new(), VUGO_PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*#p35.MABY*/ wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
    /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

    /*#p35.LUKU*/ wire LUKU_MASK_OBP1  = not1(LEKA_OBP1PIXELn);

    /* p#35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /* p#35.LYKY*/ wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
    /* p#35.LARU*/ wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /* p#35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

    /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(reg_obp1.LUXO_OBP1_D7n.qn_new(), LEDO_PAL_OBP1D,
                                                reg_obp1.LUGU_OBP1_D5n.qn_new(), LARU_PAL_OBP1C,
                                                reg_obp1.LOSE_OBP1_D3n.qn_new(), LYKY_PAL_OBP1B,
                                                reg_obp1.LAWO_OBP1_D1n.qn_new(), LOPU_PAL_OBP1A);

    /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, reg_obp1.LEPU_OBP1_D6n.qn_new(),
                                                    LARU_PAL_OBP1C, reg_obp1.LUNE_OBP1_D4n.qn_new(),
                                                    LYKY_PAL_OBP1B, reg_obp1.MOSA_OBP1_D2n.qn_new(),
                                                    LOPU_PAL_OBP1A, reg_obp1.MOXY_OBP1_D0n.qn_new());

    //----------
    // Background/window palette lookup

    /* p35.SOBA*/ wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
    /* p35.NUPO*/ wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
    /* p35.VYCO*/ wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
    /* p35.NALE*/ wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

    /* p35.MUVE*/ wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

    /* p35.POBU*/ wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /* p35.NUXO*/ wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
    /* p35.NUMA*/ wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /* p35.NYPO*/ wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

    /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, reg_bgp.MOGY_BGP_D6n.qn_new(),
                                              NUMA_PAL_BGPC, reg_bgp.MUKE_BGP_D4n.qn_new(),
                                              NUXO_PAL_BGPB, reg_bgp.PYLU_BGP_D2n.qn_new(),
                                              POBU_PAL_BGPA, reg_bgp.PAVO_BGP_D0n.qn_new());

    /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(reg_bgp.MENA_BGP_D7n.qn_new(), NYPO_PAL_BGPD,
                                              reg_bgp.MORU_BGP_D5n.qn_new(), NUMA_PAL_BGPC,
                                              reg_bgp.MAXY_BGP_D3n.qn_new(), NUXO_PAL_BGPB,
                                              reg_bgp.NUSY_BGP_D1n.qn_new(), POBU_PAL_BGPA);

    //----------
    // Pixel merge and send

    /*#p35.PERO*/ wire _PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
    /*#p35.PATY*/ wire _PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

    /*#p35.REMY*/ wire _REMY_LD0n = not1(_PERO_COL_LO);
    /*#p35.RAVO*/ wire _RAVO_LD1n = not1(_PATY_COL_HI);

    /*#p35.REMY*/ REMY_LD0n.set_new(_REMY_LD0n);
    /*#p35.RAVO*/ RAVO_LD1n.set_new(_RAVO_LD1n);
  }
}




#if 0
  [
    this,

    XODO_VID_RSTp,
    &NUKO_WX_MATCHp_old_evn
  ](){

    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_old_evn = xnor2(pix_count.XEHO_PX0p.qp_old(), reg_wx.MYPA_WX0n_h.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_old_evn = xnor2(pix_count.SAVY_PX1p.qp_old(), reg_wx.NOFE_WX1n_h.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_old_evn = xnor2(pix_count.XODU_PX2p.qp_old(), reg_wx.NOKE_WX2n_h.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_old_evn = xnor2(pix_count.XYDO_PX3p.qp_old(), reg_wx.MEBY_WX3n_h.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_old_evn = xnor2(pix_count.TUHU_PX4p.qp_old(), reg_wx.MYPU_WX4n_h.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p_old_evn = xnor2(pix_count.TUKY_PX5p.qp_old(), reg_wx.MYCE_WX5n_h.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p_old_evn = xnor2(pix_count.TAKO_PX6p.qp_old(), reg_wx.MUVO_WX6n_h.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p_old_evn = xnor2(pix_count.SYBE_PX7p.qp_old(), reg_wx.NUKU_WX7n_h.qn_old());

    /* p27.PUKY*/ wire _PUKY_WX_MATCHn_old_evn  = nand5(win_reg.REJO_WY_MATCH_LATCHp_evn.qp_old(), _NEZO_WX_MATCH4p_old_evn, _NORY_WX_MATCH5p_old_evn, _NONO_WX_MATCH6p_old_evn, _PASE_WX_MATCH7p_old_evn);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp_old_evn  = not1(_PUKY_WX_MATCHn_old_evn);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn_old_evn  = nand5(_NUFA_WX_MATCHp_old_evn, _MYLO_WX_MATCH0p_old_evn, _PUWU_WX_MATCH1p_old_evn, _PUHO_WX_MATCH2p_old_evn, _NYTU_WX_MATCH3p_old_evn);
    /* p27.NUKO*/ NUKO_WX_MATCHp_old_evn  = not1(_NOGY_WX_MATCHn_old_evn);

    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p_old_evn = xnor2(reg_wy.NESO_WY0n_h.qn_old(), reg_ly.MUWY_LY0p_evn.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p_old_evn = xnor2(reg_wy.NYRO_WY1n_h.qn_old(), reg_ly.MYRO_LY1p_evn.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p_old_evn = xnor2(reg_wy.NAGA_WY2n_h.qn_old(), reg_ly.LEXA_LY2p_evn.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p_old_evn = xnor2(reg_wy.MELA_WY3n_h.qn_old(), reg_ly.LYDO_LY3p_evn.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p_old_evn = xnor2(reg_wy.NULO_WY4n_h.qn_old(), reg_ly.LOVU_LY4p_evn.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p_old_evn = xnor2(reg_wy.NENE_WY5n_h.qn_old(), reg_ly.LEMA_LY5p_evn.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p_old_evn = xnor2(reg_wy.NUKA_WY6n_h.qn_old(), reg_ly.MATO_LY6p_evn.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p_old_evn = xnor2(reg_wy.NAFU_WY7n_h.qn_old(), reg_ly.LAFO_LY7p_evn.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn_old_evn = nand5(reg_lcdc.WYMO_LCDC_WINENn_h.qn_old(), _NOJO_WY_MATCH4p_old_evn, _PAGA_WY_MATCH5p_old_evn, _PEZO_WY_MATCH6p_old_evn, _NUPA_WY_MATCH7p_old_evn);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp_old_evn = not1(_PALO_WY_MATCH_HIn_old_evn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn_old_evn    = nand5(_NELE_WY_MATCH_HIp_old_evn, _NAZE_WY_MATCH0p_old_evn, _PEBO_WY_MATCH1p_old_evn, _POMO_WY_MATCH2p_old_evn, _NEVU_WY_MATCH3p_old_evn);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old_evn    = not1(_PAFU_WY_MATCHn_old_evn);

    {
      /* p01.XAPO*/ wire _XAPO_VID_RSTn = not1(XODO_VID_RSTp);
      /* p01.PYRY*/ wire _PYRY_VID_RSTp = not1(_XAPO_VID_RSTn);
      /* p27.SARY*/ win_reg.SARY_WY_MATCHp_evn.dff17(vclk.TALU_xxCDEFxx(), _XAPO_VID_RSTn, _ROGE_WY_MATCHp_old_evn);
      /* p27.REPU*/ wire _REPU_VBLANKp = or2(lcd.PARU_VBLANKp(), _PYRY_VID_RSTp);
      /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp_evn.nor_latch(win_reg.SARY_WY_MATCHp_evn.qp_new(), _REPU_VBLANKp);
    }
  }();
#endif