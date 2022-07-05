#include "GateBoyLib/GateBoyOamBus.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

//, reg_old.oam_latch_a, reg_new.oam_temp_a
//, reg_old.oam_latch_b, reg_new.oam_temp_b

void GateBoy::oam_latch_to_temp_a_gates(const GateBoyState& reg_old, wire COTA_OAM_CLKn)
{
  auto& reg_new = gb_state;

  /*_p29.YWOK*/ wire YWOK_OAM_CLKp = not1(COTA_OAM_CLKn); // inverting this clock does not break anything

  /*#p29.XUSO*/ reg_new.oam_temp_a.XUSO_OAM_DA0p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.YDYV_OAM_LATCH_DA0n.qp_old());
  /*_p29.XEGU*/ reg_new.oam_temp_a.XEGU_OAM_DA1p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.YCEB_OAM_LATCH_DA1n.qp_old());
  /*_p29.YJEX*/ reg_new.oam_temp_a.YJEX_OAM_DA2p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.ZUCA_OAM_LATCH_DA2n.qp_old());
  /*_p29.XYJU*/ reg_new.oam_temp_a.XYJU_OAM_DA3p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.WONE_OAM_LATCH_DA3n.qp_old());
  /*_p29.YBOG*/ reg_new.oam_temp_a.YBOG_OAM_DA4p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.ZAXE_OAM_LATCH_DA4n.qp_old());
  /*_p29.WYSO*/ reg_new.oam_temp_a.WYSO_OAM_DA5p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.XAFU_OAM_LATCH_DA5n.qp_old());
  /*_p29.XOTE*/ reg_new.oam_temp_a.XOTE_OAM_DA6p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.YSES_OAM_LATCH_DA6n.qp_old());
  /*_p29.YZAB*/ reg_new.oam_temp_a.YZAB_OAM_DA7p.dff8n(YWOK_OAM_CLKp, reg_old.oam_latch_a.ZECA_OAM_LATCH_DA7n.qp_old());
}

void GateBoy::oam_latch_to_temp_b_gates(const GateBoyState& reg_old, wire COTA_OAM_CLKn)
{
  auto& reg_new = gb_state;

  /*#p31.XEGA*/ wire XEGA_OAM_CLKp = not1(COTA_OAM_CLKn); // inverting this clock does not break anything
  /*_p31.YLOR*/ reg_new.oam_temp_b.YLOR_OAM_DB0p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.XYKY_OAM_LATCH_DB0n.qp_old());
  /*_p31.ZYTY*/ reg_new.oam_temp_b.ZYTY_OAM_DB1p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.YRUM_OAM_LATCH_DB1n.qp_old());
  /*_p31.ZYVE*/ reg_new.oam_temp_b.ZYVE_OAM_DB2p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.YSEX_OAM_LATCH_DB2n.qp_old());
  /*_p31.ZEZY*/ reg_new.oam_temp_b.ZEZY_OAM_DB3p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.YVEL_OAM_LATCH_DB3n.qp_old());
  /*_p31.GOMO*/ reg_new.oam_temp_b.GOMO_OAM_DB4p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.WYNO_OAM_LATCH_DB4n.qp_old());
  /*_p31.BAXO*/ reg_new.oam_temp_b.BAXO_OAM_DB5p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.CYRA_OAM_LATCH_DB5n.qp_old());
  /*_p31.YZOS*/ reg_new.oam_temp_b.YZOS_OAM_DB6p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.ZUVE_OAM_LATCH_DB6n.qp_old());
  /*_p31.DEPO*/ reg_new.oam_temp_b.DEPO_OAM_DB7p.dff8n(XEGA_OAM_CLKp, reg_old.oam_latch_b.ECED_OAM_LATCH_DB7n.qp_old());
}

//-----------------------------------------------------------------------------

void GateBoy::tock_oam_bus_gates(const GateBoyState& reg_old)
{
  auto& reg_new = gb_state;

  memset(&reg_new.oam_abus,   BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.oam_abus));
  memset(&reg_new.oam_dbus_a, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.oam_dbus_a));
  memset(&reg_new.oam_dbus_b, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.oam_dbus_b));

  // OAM address from CPU
  /*_p28.ASAM*/ wire ASAM_CPU_OAM_RDn = or3(reg_new.ACYL_SCANNINGp_odd.out_new(), reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_new.MATU_DMA_RUNNINGp_odd.qp_new());
  /*_p28.GARO*/ triwire GARO_CA0_TO_OA0 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*_p28.WACU*/ triwire WACU_CA1_TO_OA1 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A01p.out_new());
  /*_p28.GOSE*/ triwire GOSE_CA2_TO_OA2 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A02p.out_new());
  /*_p28.WAPE*/ triwire WAPE_CA3_TO_OA3 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A03p.out_new());
  /*_p28.FEVU*/ triwire FEVU_CA4_TO_OA4 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A04p.out_new());
  /*_p28.GERA*/ triwire GERA_CA5_TO_OA5 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A05p.out_new());
  /*_p28.WAXA*/ triwire WAXA_CA6_TO_OA6 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A06p.out_new());
  /*_p28.FOBY*/ triwire FOBY_CA7_TO_OA7 = tri6_nn(ASAM_CPU_OAM_RDn, reg_new.cpu_abus.BUS_CPU_A07p.out_new());

  /*_BUS_OAM_A00n*/ reg_new.oam_abus.BUS_OAM_A00n.tri_bus(GARO_CA0_TO_OA0);
  /*_BUS_OAM_A01n*/ reg_new.oam_abus.BUS_OAM_A01n.tri_bus(WACU_CA1_TO_OA1);
  /*_BUS_OAM_A02n*/ reg_new.oam_abus.BUS_OAM_A02n.tri_bus(GOSE_CA2_TO_OA2);
  /*_BUS_OAM_A03n*/ reg_new.oam_abus.BUS_OAM_A03n.tri_bus(WAPE_CA3_TO_OA3);
  /*_BUS_OAM_A04n*/ reg_new.oam_abus.BUS_OAM_A04n.tri_bus(FEVU_CA4_TO_OA4);
  /*_BUS_OAM_A05n*/ reg_new.oam_abus.BUS_OAM_A05n.tri_bus(GERA_CA5_TO_OA5);
  /*_BUS_OAM_A06n*/ reg_new.oam_abus.BUS_OAM_A06n.tri_bus(WAXA_CA6_TO_OA6);
  /*_BUS_OAM_A07n*/ reg_new.oam_abus.BUS_OAM_A07n.tri_bus(FOBY_CA7_TO_OA7);

  // DMA OAM write address driver
  /*_p04.DUGA*/ wire DUGA_DMA_RUNNINGn = not1(reg_new.MATU_DMA_RUNNINGp_odd.qp_new());
  /*_p28.FODO*/ triwire FODO_DMA0_TO_OA0 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.NAKY_DMA_A00p_odd.qp_new());
  /*_p28.FESA*/ triwire FESA_DMA1_TO_OA1 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.PYRO_DMA_A01p_odd.qp_new());
  /*_p28.FAGO*/ triwire FAGO_DMA2_TO_OA2 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.NEFY_DMA_A02p_odd.qp_new());
  /*_p28.FYKY*/ triwire FYKY_DMA3_TO_OA3 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.MUTY_DMA_A03p_odd.qp_new());
  /*_p28.ELUG*/ triwire ELUG_DMA4_TO_OA4 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.NYKO_DMA_A04p_odd.qp_new());
  /*_p28.EDOL*/ triwire EDOL_DMA5_TO_OA5 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.PYLO_DMA_A05p_odd.qp_new());
  /*_p28.FYDU*/ triwire FYDU_DMA6_TO_OA6 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.NUTO_DMA_A06p_odd.qp_new());
  /*_p28.FETU*/ triwire FETU_DMA7_TO_OA7 = tri6_nn(DUGA_DMA_RUNNINGn, reg_new.dma_lo.MUGU_DMA_A07p_odd.qp_new());

  /*_BUS_OAM_A00n*/ reg_new.oam_abus.BUS_OAM_A00n.tri_bus(FODO_DMA0_TO_OA0);
  /*_BUS_OAM_A01n*/ reg_new.oam_abus.BUS_OAM_A01n.tri_bus(FESA_DMA1_TO_OA1);
  /*_BUS_OAM_A02n*/ reg_new.oam_abus.BUS_OAM_A02n.tri_bus(FAGO_DMA2_TO_OA2);
  /*_BUS_OAM_A03n*/ reg_new.oam_abus.BUS_OAM_A03n.tri_bus(FYKY_DMA3_TO_OA3);
  /*_BUS_OAM_A04n*/ reg_new.oam_abus.BUS_OAM_A04n.tri_bus(ELUG_DMA4_TO_OA4);
  /*_BUS_OAM_A05n*/ reg_new.oam_abus.BUS_OAM_A05n.tri_bus(EDOL_DMA5_TO_OA5);
  /*_BUS_OAM_A06n*/ reg_new.oam_abus.BUS_OAM_A06n.tri_bus(FYDU_DMA6_TO_OA6);
  /*_BUS_OAM_A07n*/ reg_new.oam_abus.BUS_OAM_A07n.tri_bus(FETU_DMA7_TO_OA7);

  // OAM address from sprite scanner
  /*_p28.APAR*/ wire APAR_SCANNINGn = not1(reg_new.ACYL_SCANNINGp_odd.out_new());
  /*_p28.GEFY*/ triwire GEFY_SCANX_TO_OA0 = tri6_nn(APAR_SCANNINGn, reg_new.SIG_GND.out_new());
  /*_p28.WUWE*/ triwire WUWE_SCANX_TO_OA1 = tri6_nn(APAR_SCANNINGn, reg_new.SIG_GND.out_new());
  /*_p28.GUSE*/ triwire GUSE_SCAN0_TO_OA2 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.YFEL_SCAN0_odd.qp_new());
  /*_p28.GEMA*/ triwire GEMA_SCAN1_TO_OA3 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.WEWY_SCAN1_odd.qp_new());
  /*_p28.FUTO*/ triwire FUTO_SCAN2_TO_OA4 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.GOSO_SCAN2_odd.qp_new());
  /*_p28.FAKU*/ triwire FAKU_SCAN3_TO_OA5 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.ELYN_SCAN3_odd.qp_new());
  /*_p28.GAMA*/ triwire GAMA_SCAN4_TO_OA6 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.FAHA_SCAN4_odd.qp_new());
  /*_p28.GOBY*/ triwire GOBY_SCAN5_TO_OA7 = tri6_nn(APAR_SCANNINGn, reg_new.scan_counter.FONY_SCAN5_odd.qp_new());

  /*_BUS_OAM_A00n*/ reg_new.oam_abus.BUS_OAM_A00n.tri_bus(GEFY_SCANX_TO_OA0);
  /*_BUS_OAM_A01n*/ reg_new.oam_abus.BUS_OAM_A01n.tri_bus(WUWE_SCANX_TO_OA1);
  /*_BUS_OAM_A02n*/ reg_new.oam_abus.BUS_OAM_A02n.tri_bus(GUSE_SCAN0_TO_OA2);
  /*_BUS_OAM_A03n*/ reg_new.oam_abus.BUS_OAM_A03n.tri_bus(GEMA_SCAN1_TO_OA3);
  /*_BUS_OAM_A04n*/ reg_new.oam_abus.BUS_OAM_A04n.tri_bus(FUTO_SCAN2_TO_OA4);
  /*_BUS_OAM_A05n*/ reg_new.oam_abus.BUS_OAM_A05n.tri_bus(FAKU_SCAN3_TO_OA5);
  /*_BUS_OAM_A06n*/ reg_new.oam_abus.BUS_OAM_A06n.tri_bus(GAMA_SCAN4_TO_OA6);
  /*_BUS_OAM_A07n*/ reg_new.oam_abus.BUS_OAM_A07n.tri_bus(GOBY_SCAN5_TO_OA7);

  // OAM address from sprite fetcher
  /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn_odd_new = not1(reg_new.MATU_DMA_RUNNINGp_odd.qp_new());
  /*_p28.AJON*/ wire AJON_RENDERINGp = and2(BOGE_DMA_RUNNINGn_odd_new, reg_new.XYMU_RENDERING_LATCHn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /*_p28.BETE*/ wire BETE_SPR_I_TO_OAM_An = not1(AJON_RENDERINGp);
  /*_p28.GECA*/ triwire GECA_FETCHX_TO_OA0 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.SIG_VCC.out_new());
  /*_p28.WYDU*/ triwire WYDU_FETCHX_TO_OA1 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.SIG_VCC.out_new());
  /*_p28.GYBU*/ triwire GYBU_FETCH0_TO_OA2 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I0.out_new());
  /*_p28.GYKA*/ triwire GYKA_FETCH1_TO_OA3 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I1.out_new());
  /*_p28.FABY*/ triwire FABY_FETCH2_TO_OA4 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I2.out_new());
  /*_p28.FACO*/ triwire FACO_FETCH3_TO_OA5 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I3.out_new());
  /*_p28.FUGU*/ triwire FUGU_FETCH4_TO_OA6 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I4.out_new());
  /*_p28.FYKE*/ triwire FYKE_FETCH5_TO_OA7 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg_new.sprite_ibus.BUS_SPR_I5.out_new());

  /*_BUS_OAM_A00n*/ reg_new.oam_abus.BUS_OAM_A00n.tri_bus(GECA_FETCHX_TO_OA0);
  /*_BUS_OAM_A01n*/ reg_new.oam_abus.BUS_OAM_A01n.tri_bus(WYDU_FETCHX_TO_OA1);
  /*_BUS_OAM_A02n*/ reg_new.oam_abus.BUS_OAM_A02n.tri_bus(GYBU_FETCH0_TO_OA2);
  /*_BUS_OAM_A03n*/ reg_new.oam_abus.BUS_OAM_A03n.tri_bus(GYKA_FETCH1_TO_OA3);
  /*_BUS_OAM_A04n*/ reg_new.oam_abus.BUS_OAM_A04n.tri_bus(FABY_FETCH2_TO_OA4);
  /*_BUS_OAM_A05n*/ reg_new.oam_abus.BUS_OAM_A05n.tri_bus(FACO_FETCH3_TO_OA5);
  /*_BUS_OAM_A06n*/ reg_new.oam_abus.BUS_OAM_A06n.tri_bus(FUGU_FETCH4_TO_OA6);
  /*_BUS_OAM_A07n*/ reg_new.oam_abus.BUS_OAM_A07n.tri_bus(FYKE_FETCH5_TO_OA7);

  // cpu to oam data bus

  /*_p28.XUTO*/ wire XUTO_CPU_OAM_WRp_new = and2(reg_new.cpu_abus.SARO_ADDR_OAMp_new(), reg_new.cpu_signals.CUPA_CPU_WRp_new());
  /*_p28.WUJE*/ reg_new.oam_ctrl.WUJE_CPU_OAM_WRn.nor_latch(reg_new.sys_clk.XYNY_ABCDxxxx_new(), XUTO_CPU_OAM_WRp_new);

  /*_p28.XUPA*/ wire XUPA_CPU_OAM_WRp_new  = not1(reg_new.oam_ctrl.WUJE_CPU_OAM_WRn.qp_new());
  /*#p28.AJUJ*/ wire AJUJ_OAM_BUSYn_new    = nor3(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), reg_new.ACYL_SCANNINGp_odd.out_new(), AJON_RENDERINGp);
  /*#p28.AMAB*/ wire AMAB_CPU_OAM_ENp_new  = and2(reg_new.cpu_abus.SARO_ADDR_OAMp_new(), AJUJ_OAM_BUSYn_new); // def and
  /*#p28.APAG*/ wire APAG_CBD_TO_OBDp_new  = amux2(XUPA_CPU_OAM_WRp_new, AMAB_CPU_OAM_ENp_new, AJUJ_OAM_BUSYn_new, reg_new.cpu_abus.ADAH_FE00_FEFFn_new());
  /*#p28.AZUL*/ wire AZUL_CBD_TO_OBDn_new  = not1(APAG_CBD_TO_OBDp_new);

  /*_p28.ZAMY*/ triwire ZAMY_CD0_TO_ODA0_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p28.ZOPU*/ triwire ZOPU_CD1_TO_ODA1_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p28.WYKY*/ triwire WYKY_CD2_TO_ODA2_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p28.ZAJA*/ triwire ZAJA_CD3_TO_ODA3_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p28.ZUGA*/ triwire ZUGA_CD4_TO_ODA4_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
  /*_p28.ZUMO*/ triwire ZUMO_CD5_TO_ODA5_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D05p.out_new());
  /*_p28.XYTO*/ triwire XYTO_CD6_TO_ODA6_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D06p.out_new());
  /*_p28.ZYFA*/ triwire ZYFA_CD7_TO_ODA7_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D07p.out_new());

  /*_p28.ZAXA*/ triwire ZAXA_CD0_TO_ODB0_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p28.ZAKY*/ triwire ZAKY_CD1_TO_ODB1_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p28.WULE*/ triwire WULE_CD2_TO_ODB2_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p28.ZOZO*/ triwire ZOZO_CD3_TO_ODB3_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p28.ZUFO*/ triwire ZUFO_CD4_TO_ODB4_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
  /*_p28.ZATO*/ triwire ZATO_CD5_TO_ODB5_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D05p.out_new());
  /*_p28.YVUC*/ triwire YVUC_CD6_TO_ODB6_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D06p.out_new());
  /*_p28.ZUFE*/ triwire ZUFE_CD7_TO_ODB7_new = tri6_nn(AZUL_CBD_TO_OBDn_new, reg_new.cpu_dbus.BUS_CPU_D07p.out_new());

  /*_BUS_OAM_DA00n*/ reg_new.oam_dbus_a.BUS_OAM_DA00n.tri_bus(ZAMY_CD0_TO_ODA0_new);
  /*_BUS_OAM_DA01n*/ reg_new.oam_dbus_a.BUS_OAM_DA01n.tri_bus(ZOPU_CD1_TO_ODA1_new);
  /*_BUS_OAM_DA02n*/ reg_new.oam_dbus_a.BUS_OAM_DA02n.tri_bus(WYKY_CD2_TO_ODA2_new);
  /*_BUS_OAM_DA03n*/ reg_new.oam_dbus_a.BUS_OAM_DA03n.tri_bus(ZAJA_CD3_TO_ODA3_new);
  /*_BUS_OAM_DA04n*/ reg_new.oam_dbus_a.BUS_OAM_DA04n.tri_bus(ZUGA_CD4_TO_ODA4_new);
  /*_BUS_OAM_DA05n*/ reg_new.oam_dbus_a.BUS_OAM_DA05n.tri_bus(ZUMO_CD5_TO_ODA5_new);
  /*_BUS_OAM_DA06n*/ reg_new.oam_dbus_a.BUS_OAM_DA06n.tri_bus(XYTO_CD6_TO_ODA6_new);
  /*_BUS_OAM_DA07n*/ reg_new.oam_dbus_a.BUS_OAM_DA07n.tri_bus(ZYFA_CD7_TO_ODA7_new);

  /*_BUS_OAM_DB00n*/ reg_new.oam_dbus_b.BUS_OAM_DB00n.tri_bus(ZAXA_CD0_TO_ODB0_new);
  /*_BUS_OAM_DB01n*/ reg_new.oam_dbus_b.BUS_OAM_DB01n.tri_bus(ZAKY_CD1_TO_ODB1_new);
  /*_BUS_OAM_DB02n*/ reg_new.oam_dbus_b.BUS_OAM_DB02n.tri_bus(WULE_CD2_TO_ODB2_new);
  /*_BUS_OAM_DB03n*/ reg_new.oam_dbus_b.BUS_OAM_DB03n.tri_bus(ZOZO_CD3_TO_ODB3_new);
  /*_BUS_OAM_DB04n*/ reg_new.oam_dbus_b.BUS_OAM_DB04n.tri_bus(ZUFO_CD4_TO_ODB4_new);
  /*_BUS_OAM_DB05n*/ reg_new.oam_dbus_b.BUS_OAM_DB05n.tri_bus(ZATO_CD5_TO_ODB5_new);
  /*_BUS_OAM_DB06n*/ reg_new.oam_dbus_b.BUS_OAM_DB06n.tri_bus(YVUC_CD6_TO_ODB6_new);
  /*_BUS_OAM_DB07n*/ reg_new.oam_dbus_b.BUS_OAM_DB07n.tri_bus(ZUFE_CD7_TO_ODB7_new);

  // ext dma to oam data bus

  /*_p25.RALO*/ wire RALO_EXT_D0p_new = not1(pins.dbus.PIN_17_D00.qp_int_new());
  /*_p25.TUNE*/ wire TUNE_EXT_D1p_new = not1(pins.dbus.PIN_18_D01.qp_int_new());
  /*_p25.SERA*/ wire SERA_EXT_D2p_new = not1(pins.dbus.PIN_19_D02.qp_int_new());
  /*_p25.TENU*/ wire TENU_EXT_D3p_new = not1(pins.dbus.PIN_20_D03.qp_int_new());
  /*_p25.SYSA*/ wire SYSA_EXT_D4p_new = not1(pins.dbus.PIN_21_D04.qp_int_new());
  /*_p25.SUGY*/ wire SUGY_EXT_D5p_new = not1(pins.dbus.PIN_22_D05.qp_int_new());
  /*_p25.TUBE*/ wire TUBE_EXT_D6p_new = not1(pins.dbus.PIN_23_D06.qp_int_new());
  /*_p25.SYZO*/ wire SYZO_EXT_D7p_new = not1(pins.dbus.PIN_24_D07.qp_int_new());

  /*#p04.LEBU*/ wire LEBU_DMA_A15n_new  = not1(reg_new.reg_dma.MARU_DMA_A15p.qp_newB());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new = nor3(reg_new.reg_dma.PULA_DMA_A13p.qp_newB(), reg_new.reg_dma.POKU_DMA_A14p.qp_newB(), LEBU_DMA_A15n_new);
  /*_p04.LOGO*/ wire LOGO_DMA_VRAMn_new = not1(MUDA_DMA_VRAMp_new);
  /*_p04.MORY*/ wire MORY_DMA_CARTn_new = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), LOGO_DMA_VRAMn_new);
  /*_p04.LUMA*/ wire LUMA_DMA_CARTp_new = not1(MORY_DMA_CARTn_new);
  /*_p25.CEDE*/ wire CEDE_EBD_TO_OBDn_new = not1(LUMA_DMA_CARTp_new);

  /*_p25.WASA*/ triwire WASA_ED0_TO_ODA0_new = tri6_nn(CEDE_EBD_TO_OBDn_new, RALO_EXT_D0p_new);
  /*_p25.BOMO*/ triwire BOMO_ED1_TO_ODA1_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TUNE_EXT_D1p_new);
  /*_p25.BASA*/ triwire BASA_ED2_TO_ODA2_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SERA_EXT_D2p_new);
  /*_p25.CAKO*/ triwire CAKO_ED3_TO_ODA3_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TENU_EXT_D3p_new);
  /*_p25.BUMA*/ triwire BUMA_ED4_TO_ODA4_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SYSA_EXT_D4p_new);
  /*_p25.BUPY*/ triwire BUPY_ED5_TO_ODA5_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SUGY_EXT_D5p_new);
  /*_p25.BASY*/ triwire BASY_ED6_TO_ODA6_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TUBE_EXT_D6p_new);
  /*_p25.BAPE*/ triwire BAPE_ED7_TO_ODA7_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SYZO_EXT_D7p_new);

  /*_p25.WEJO*/ triwire WEJO_ED0_TO_ODB0_new = tri6_nn(CEDE_EBD_TO_OBDn_new, RALO_EXT_D0p_new);
  /*_p25.BUBO*/ triwire BUBO_ED1_TO_ODB1_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TUNE_EXT_D1p_new);
  /*_p25.BETU*/ triwire BETU_ED2_TO_ODB2_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SERA_EXT_D2p_new);
  /*_p25.CYME*/ triwire CYME_ED3_TO_ODB3_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TENU_EXT_D3p_new);
  /*_p25.BAXU*/ triwire BAXU_ED4_TO_ODB4_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SYSA_EXT_D4p_new);
  /*_p25.BUHU*/ triwire BUHU_ED5_TO_ODB5_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SUGY_EXT_D5p_new);
  /*_p25.BYNY*/ triwire BYNY_ED6_TO_ODB6_new = tri6_nn(CEDE_EBD_TO_OBDn_new, TUBE_EXT_D6p_new);
  /*_p25.BYPY*/ triwire BYPY_ED7_TO_ODB7_new = tri6_nn(CEDE_EBD_TO_OBDn_new, SYZO_EXT_D7p_new);

  /*_BUS_OAM_DA00n*/ reg_new.oam_dbus_a.BUS_OAM_DA00n.tri_bus(WASA_ED0_TO_ODA0_new);
  /*_BUS_OAM_DA01n*/ reg_new.oam_dbus_a.BUS_OAM_DA01n.tri_bus(BOMO_ED1_TO_ODA1_new);
  /*_BUS_OAM_DA02n*/ reg_new.oam_dbus_a.BUS_OAM_DA02n.tri_bus(BASA_ED2_TO_ODA2_new);
  /*_BUS_OAM_DA03n*/ reg_new.oam_dbus_a.BUS_OAM_DA03n.tri_bus(CAKO_ED3_TO_ODA3_new);
  /*_BUS_OAM_DA04n*/ reg_new.oam_dbus_a.BUS_OAM_DA04n.tri_bus(BUMA_ED4_TO_ODA4_new);
  /*_BUS_OAM_DA05n*/ reg_new.oam_dbus_a.BUS_OAM_DA05n.tri_bus(BUPY_ED5_TO_ODA5_new);
  /*_BUS_OAM_DA06n*/ reg_new.oam_dbus_a.BUS_OAM_DA06n.tri_bus(BASY_ED6_TO_ODA6_new);
  /*_BUS_OAM_DA07n*/ reg_new.oam_dbus_a.BUS_OAM_DA07n.tri_bus(BAPE_ED7_TO_ODA7_new);

  /*_BUS_OAM_DB00n*/ reg_new.oam_dbus_b.BUS_OAM_DB00n.tri_bus(WEJO_ED0_TO_ODB0_new);
  /*_BUS_OAM_DB01n*/ reg_new.oam_dbus_b.BUS_OAM_DB01n.tri_bus(BUBO_ED1_TO_ODB1_new);
  /*_BUS_OAM_DB02n*/ reg_new.oam_dbus_b.BUS_OAM_DB02n.tri_bus(BETU_ED2_TO_ODB2_new);
  /*_BUS_OAM_DB03n*/ reg_new.oam_dbus_b.BUS_OAM_DB03n.tri_bus(CYME_ED3_TO_ODB3_new);
  /*_BUS_OAM_DB04n*/ reg_new.oam_dbus_b.BUS_OAM_DB04n.tri_bus(BAXU_ED4_TO_ODB4_new);
  /*_BUS_OAM_DB05n*/ reg_new.oam_dbus_b.BUS_OAM_DB05n.tri_bus(BUHU_ED5_TO_ODB5_new);
  /*_BUS_OAM_DB06n*/ reg_new.oam_dbus_b.BUS_OAM_DB06n.tri_bus(BYNY_ED6_TO_ODB6_new);
  /*_BUS_OAM_DB07n*/ reg_new.oam_dbus_b.BUS_OAM_DB07n.tri_bus(BYPY_ED7_TO_ODB7_new);

  // vram dma to oam data bus

  /*#p04.MUHO*/ wire MUHO_DMA_VRAMp_new = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MUDA_DMA_VRAMp_new);
  /*#p04.LUFA*/ wire LUFA_DMA_VRAMp_new = not1(MUHO_DMA_VRAMp_new);
  /*_p28.AZAR*/ wire AZAR_VBD_TO_OBDn_new = not1(LUFA_DMA_VRAMp_new);

  /*_p28.WUZU*/ triwire WUZU_VD0_TO_ODA0_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D00p.out_new());
  /*_p28.AXER*/ triwire AXER_VD1_TO_ODA1_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D01p.out_new());
  /*_p28.ASOX*/ triwire ASOX_VD2_TO_ODA2_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D02p.out_new());
  /*_p28.CETU*/ triwire CETU_VD3_TO_ODA3_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D03p.out_new());
  /*_p28.ARYN*/ triwire ARYN_VD4_TO_ODA4_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D04p.out_new());
  /*_p28.ACOT*/ triwire ACOT_VD5_TO_ODA5_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D05p.out_new());
  /*_p28.CUJE*/ triwire CUJE_VD6_TO_ODA6_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D06p.out_new());
  /*_p28.ATER*/ triwire ATER_VD7_TO_ODA7_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D07p.out_new());

  /*_p28.WOWA*/ triwire WOWA_VD0_TO_ODB0_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D00p.out_new());
  /*_p28.AVEB*/ triwire AVEB_VD1_TO_ODB1_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D01p.out_new());
  /*_p28.AMUH*/ triwire AMUH_VD2_TO_ODB2_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D02p.out_new());
  /*_p28.COFO*/ triwire COFO_VD3_TO_ODB3_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D03p.out_new());
  /*_p28.AZOZ*/ triwire AZOZ_VD4_TO_ODB4_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D04p.out_new());
  /*_p28.AGYK*/ triwire AGYK_VD5_TO_ODB5_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D05p.out_new());
  /*_p28.BUSE*/ triwire BUSE_VD6_TO_ODB6_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D06p.out_new());
  /*_p28.ANUM*/ triwire ANUM_VD7_TO_ODB7_new = tri6_nn(AZAR_VBD_TO_OBDn_new, reg_new.vram_dbus.BUS_VRAM_D07p.out_new());

  /*_BUS_OAM_DA00n*/ reg_new.oam_dbus_a.BUS_OAM_DA00n.tri_bus(WUZU_VD0_TO_ODA0_new);
  /*_BUS_OAM_DA01n*/ reg_new.oam_dbus_a.BUS_OAM_DA01n.tri_bus(AXER_VD1_TO_ODA1_new);
  /*_BUS_OAM_DA02n*/ reg_new.oam_dbus_a.BUS_OAM_DA02n.tri_bus(ASOX_VD2_TO_ODA2_new);
  /*_BUS_OAM_DA03n*/ reg_new.oam_dbus_a.BUS_OAM_DA03n.tri_bus(CETU_VD3_TO_ODA3_new);
  /*_BUS_OAM_DA04n*/ reg_new.oam_dbus_a.BUS_OAM_DA04n.tri_bus(ARYN_VD4_TO_ODA4_new);
  /*_BUS_OAM_DA05n*/ reg_new.oam_dbus_a.BUS_OAM_DA05n.tri_bus(ACOT_VD5_TO_ODA5_new);
  /*_BUS_OAM_DA06n*/ reg_new.oam_dbus_a.BUS_OAM_DA06n.tri_bus(CUJE_VD6_TO_ODA6_new);
  /*_BUS_OAM_DA07n*/ reg_new.oam_dbus_a.BUS_OAM_DA07n.tri_bus(ATER_VD7_TO_ODA7_new);

  /*_BUS_OAM_DB00n*/ reg_new.oam_dbus_b.BUS_OAM_DB00n.tri_bus(WOWA_VD0_TO_ODB0_new);
  /*_BUS_OAM_DB01n*/ reg_new.oam_dbus_b.BUS_OAM_DB01n.tri_bus(AVEB_VD1_TO_ODB1_new);
  /*_BUS_OAM_DB02n*/ reg_new.oam_dbus_b.BUS_OAM_DB02n.tri_bus(AMUH_VD2_TO_ODB2_new);
  /*_BUS_OAM_DB03n*/ reg_new.oam_dbus_b.BUS_OAM_DB03n.tri_bus(COFO_VD3_TO_ODB3_new);
  /*_BUS_OAM_DB04n*/ reg_new.oam_dbus_b.BUS_OAM_DB04n.tri_bus(AZOZ_VD4_TO_ODB4_new);
  /*_BUS_OAM_DB05n*/ reg_new.oam_dbus_b.BUS_OAM_DB05n.tri_bus(AGYK_VD5_TO_ODB5_new);
  /*_BUS_OAM_DB06n*/ reg_new.oam_dbus_b.BUS_OAM_DB06n.tri_bus(BUSE_VD6_TO_ODB6_new);
  /*_BUS_OAM_DB07n*/ reg_new.oam_dbus_b.BUS_OAM_DB07n.tri_bus(ANUM_VD7_TO_ODB7_new);

  /*_p25.AVER*/ wire AVER_AxxxExxx_new = nand2(reg_new.ACYL_SCANNINGp_odd.out_new(), reg_new.sys_clk.XYSO_xBCDxFGH_new());
  /*_p25.CUFE*/ wire CUFE_OAM_CLKp_new = not_or_and3(reg_new.cpu_abus.SARO_ADDR_OAMp_new(), reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), reg_new.sys_clk.MOPA_xxxxEFGH_new()); // CUFE looks like BYHA minus an inverter
  /*_p29.TYTU*/ wire TYTU_SFETCH_S0n_new = not1(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_new());
  /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG_new = nand2(reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qp_new(), TYTU_SFETCH_S0n_new);
  /*_p29.TEPA*/ wire TEPA_RENDERINGn_new = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());
  /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp_new = nor3(TEPA_RENDERINGn_new, reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qp_new(), reg_new.sfetch_counter_evn.TESE_SFETCH_S2p_evn.qp_new());
  /*_p25.VAPE*/ wire VAPE_OAM_CLKENn_new = and2(TUVO_PPU_OAM_RDp_new, TACU_SPR_SEQ_5_TRIG_new);
  /*_p25.XUJY*/ wire XUJY_OAM_CLKENp_new = not1(VAPE_OAM_CLKENn_new);
  /*_p25.BYCU*/ wire BYCU_OAM_CLKp_new = nand3(AVER_AxxxExxx_new, XUJY_OAM_CLKENp_new, CUFE_OAM_CLKp_new);
  /*_p25.COTA*/ wire COTA_OAM_CLKn_new = not1(BYCU_OAM_CLKp_new);

  /*_SIG_OAM_CLKn*/ reg_new.oam_ctrl.SIG_OAM_CLKn.sig_out(COTA_OAM_CLKn_new);

  /*#p28.GEKA*/ wire GEKA_OAM_A0p_new = not1(reg_new.oam_abus.BUS_OAM_A00n.out_new());
  /*_p28.WAFO*/ wire WAFO_OAM_A0n_new = not1(GEKA_OAM_A0p_new);

  /*_p04.MAKA*/ reg_new.oam_ctrl.MAKA_LATCH_EXTp.dff17(reg_new.sys_clk.ZEME_odd_new(), reg_new.sys_rst.AVOR_SYS_RSTp_new(), reg_old.cpu_signals.CATY_LATCH_EXTp_old());

  /*_p04.NAXY*/ wire NAXY_DMA_OAM_WRp_new = nor2(reg_new.sys_clk.UVYT_ABCDxxxx_new(), reg_new.oam_ctrl.MAKA_LATCH_EXTp.qp_new()); // def nor2
  /*_p04.POWU*/ wire POWU_DMA_OAM_WRp_new = and2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), NAXY_DMA_OAM_WRp_new); // def and
  /*_p04.WYJA*/ wire WYJA_OAM_WRp_new     = and_or3(AMAB_CPU_OAM_ENp_new, reg_new.cpu_signals.CUPA_CPU_WRp_new(), POWU_DMA_OAM_WRp_new);
  /*_p28.YNYC*/ wire YNYC_OAM_A_WRp_new = and2(WYJA_OAM_WRp_new, WAFO_OAM_A0n_new);
  /*_p28.YLYC*/ wire YLYC_OAM_B_WRp_new = and2(WYJA_OAM_WRp_new, GEKA_OAM_A0p_new);
  /*_p28.ZOFE*/ wire ZOFE_OAM_A_WRn_new = not1(YNYC_OAM_A_WRp_new);
  /*_p28.ZONE*/ wire ZONE_OAM_B_WRn_new = not1(YLYC_OAM_B_WRp_new);

  /*_SIG_OAM_WRn_A*/ reg_new.oam_ctrl.SIG_OAM_WRn_A.sig_out(ZOFE_OAM_A_WRn_new);
  /*_SIG_OAM_WRn_B*/ reg_new.oam_ctrl.SIG_OAM_WRn_B.sig_out(ZONE_OAM_B_WRn_new);

  /*#p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn_new = nand2(reg_new.ACYL_SCANNINGp_odd.out_new(), reg_new.sys_clk.XOCE_xBCxxFGx_new()); // schematic wrong, is def nand2
  /*_p28.WEFY*/ wire WEFY_SPR_READp_new       = and2(TUVO_PPU_OAM_RDp_new, reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qp_new());
  /*#p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn_new  = not1(WEFY_SPR_READp_new);

  /*#p28.BOTA*/ wire BOTA_OAM_OEn_new  = nand3(reg_new.cpu_signals.BOFE_LATCH_EXTn_new(), reg_new.cpu_abus.SARO_ADDR_OAMp_new(), reg_new.cpu_signals.ASOT_CPU_RDp_new()); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire ASYT_OAM_OEn_new = and3(AJEP_SCAN_OAM_LATCHn_new, XUJA_SPR_OAM_LATCHn_new, BOTA_OAM_OEn_new); // def and

  /*#p28.BODE*/ wire BODE_OAM_OEp_new = not1(ASYT_OAM_OEn_new);
  /*#p28.YVAL*/ wire YVAL_OAM_OEn_new = not1(BODE_OAM_OEp_new);
  /*#p28.YRYV*/ wire YRYV_OAM_OEp_new = not1(YVAL_OAM_OEn_new);
  /*#p28.ZODO*/ wire ZODO_OAM_OEn_new = not1(YRYV_OAM_OEp_new);   // schematic thinks this is OAM_CLK?
  /*_SIG_OAM_OEn*/ reg_new.oam_ctrl.SIG_OAM_OEn.sig_out(ZODO_OAM_OEn_new);

  uint8_t oam_addr   = (uint8_t)bit_pack_inv(reg_new.oam_abus) >> 1;
  uint8_t oam_data_a = (uint8_t)bit_pack_inv(reg_new.oam_dbus_a);
  uint8_t oam_data_b = (uint8_t)bit_pack_inv(reg_new.oam_dbus_b);

  if (bit0(reg_old.oam_ctrl.old_oam_clk.out_old()) && !bit0(reg_new.oam_ctrl.SIG_OAM_CLKn.out_new())) {
    //printf("oam writing 0x%02x 0x%02x 0x%02x\n", oam_addr, oam_data_a, oam_data_b);

    if (bit0(~reg_new.oam_ctrl.SIG_OAM_WRn_A.out_new())) mem.oam_ram[(oam_addr << 1) + 0] = oam_data_a;
    if (bit0(~reg_new.oam_ctrl.SIG_OAM_WRn_B.out_new())) mem.oam_ram[(oam_addr << 1) + 1] = oam_data_b;
  }

  oam_data_a = mem.oam_ram[(oam_addr << 1) + 0];
  oam_data_b = mem.oam_ram[(oam_addr << 1) + 1];

  reg_new.oam_ctrl.old_oam_clk <<= reg_new.oam_ctrl.SIG_OAM_CLKn.out_new();

  triwire oam_data_a0 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 0));
  triwire oam_data_a1 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 1));
  triwire oam_data_a2 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 2));
  triwire oam_data_a3 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 3));
  triwire oam_data_a4 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 4));
  triwire oam_data_a5 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 5));
  triwire oam_data_a6 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 6));
  triwire oam_data_a7 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_a, 7));

  triwire oam_data_b0 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 0));
  triwire oam_data_b1 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 1));
  triwire oam_data_b2 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 2));
  triwire oam_data_b3 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 3));
  triwire oam_data_b4 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 4));
  triwire oam_data_b5 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 5));
  triwire oam_data_b6 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 6));
  triwire oam_data_b7 = tri6_nn(ZODO_OAM_OEn_new, bit(oam_data_b, 7));

  reg_new.oam_dbus_a.BUS_OAM_DA00n.tri_bus(oam_data_a0);
  reg_new.oam_dbus_a.BUS_OAM_DA01n.tri_bus(oam_data_a1);
  reg_new.oam_dbus_a.BUS_OAM_DA02n.tri_bus(oam_data_a2);
  reg_new.oam_dbus_a.BUS_OAM_DA03n.tri_bus(oam_data_a3);
  reg_new.oam_dbus_a.BUS_OAM_DA04n.tri_bus(oam_data_a4);
  reg_new.oam_dbus_a.BUS_OAM_DA05n.tri_bus(oam_data_a5);
  reg_new.oam_dbus_a.BUS_OAM_DA06n.tri_bus(oam_data_a6);
  reg_new.oam_dbus_a.BUS_OAM_DA07n.tri_bus(oam_data_a7);

  reg_new.oam_dbus_b.BUS_OAM_DB00n.tri_bus(oam_data_b0);
  reg_new.oam_dbus_b.BUS_OAM_DB01n.tri_bus(oam_data_b1);
  reg_new.oam_dbus_b.BUS_OAM_DB02n.tri_bus(oam_data_b2);
  reg_new.oam_dbus_b.BUS_OAM_DB03n.tri_bus(oam_data_b3);
  reg_new.oam_dbus_b.BUS_OAM_DB04n.tri_bus(oam_data_b4);
  reg_new.oam_dbus_b.BUS_OAM_DB05n.tri_bus(oam_data_b5);
  reg_new.oam_dbus_b.BUS_OAM_DB06n.tri_bus(oam_data_b6);
  reg_new.oam_dbus_b.BUS_OAM_DB07n.tri_bus(oam_data_b7);

  /*#p29.YDYV*/ reg_new.oam_latch_a.YDYV_OAM_LATCH_DA0n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA00n.out_new());
  /*_p29.YCEB*/ reg_new.oam_latch_a.YCEB_OAM_LATCH_DA1n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA01n.out_new());
  /*_p29.ZUCA*/ reg_new.oam_latch_a.ZUCA_OAM_LATCH_DA2n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA02n.out_new());
  /*_p29.WONE*/ reg_new.oam_latch_a.WONE_OAM_LATCH_DA3n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA03n.out_new());
  /*_p29.ZAXE*/ reg_new.oam_latch_a.ZAXE_OAM_LATCH_DA4n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA04n.out_new());
  /*_p29.XAFU*/ reg_new.oam_latch_a.XAFU_OAM_LATCH_DA5n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA05n.out_new());
  /*_p29.YSES*/ reg_new.oam_latch_a.YSES_OAM_LATCH_DA6n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA06n.out_new());
  /*_p29.ZECA*/ reg_new.oam_latch_a.ZECA_OAM_LATCH_DA7n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_a.BUS_OAM_DA07n.out_new());

  /*#p31.XYKY*/ reg_new.oam_latch_b.XYKY_OAM_LATCH_DB0n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB00n.out_new());
  /*_p31.YRUM*/ reg_new.oam_latch_b.YRUM_OAM_LATCH_DB1n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB01n.out_new());
  /*_p31.YSEX*/ reg_new.oam_latch_b.YSEX_OAM_LATCH_DB2n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB02n.out_new());
  /*_p31.YVEL*/ reg_new.oam_latch_b.YVEL_OAM_LATCH_DB3n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB03n.out_new());
  /*_p31.WYNO*/ reg_new.oam_latch_b.WYNO_OAM_LATCH_DB4n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB04n.out_new());
  /*_p31.CYRA*/ reg_new.oam_latch_b.CYRA_OAM_LATCH_DB5n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB05n.out_new());
  /*_p31.ZUVE*/ reg_new.oam_latch_b.ZUVE_OAM_LATCH_DB6n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB06n.out_new());
  /*_p31.ECED*/ reg_new.oam_latch_b.ECED_OAM_LATCH_DB7n.tp_latchn(BODE_OAM_OEp_new, reg_new.oam_dbus_b.BUS_OAM_DB07n.out_new());

  /*_p28.GUKO*/ wire GUKO_OBL_TO_CBDp_new = and3(reg_new.cpu_signals.LEKO_CPU_RDp_new(), AMAB_CPU_OAM_ENp_new, WAFO_OAM_A0n_new);
  /*_p28.WUME*/ wire WUME_OBL_TO_CBDn_new = not1(GUKO_OBL_TO_CBDp_new);
  /*_p29.YFAP*/ triwire YFAP_OLA_TO_CBD0_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.YDYV_OAM_LATCH_DA0n.qn_new());
  /*_p29.XELE*/ triwire XELE_OLA_TO_CBD1_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.YCEB_OAM_LATCH_DA1n.qn_new());
  /*_p29.YPON*/ triwire YPON_OLA_TO_CBD2_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.ZUCA_OAM_LATCH_DA2n.qn_new());
  /*_p29.XUVO*/ triwire XUVO_OLA_TO_CBD3_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.WONE_OAM_LATCH_DA3n.qn_new());
  /*_p29.ZYSA*/ triwire ZYSA_OLA_TO_CBD4_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.ZAXE_OAM_LATCH_DA4n.qn_new());
  /*_p29.YWEG*/ triwire YWEG_OLA_TO_CBD5_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.XAFU_OAM_LATCH_DA5n.qn_new());
  /*_p29.XABU*/ triwire XABU_OLA_TO_CBD6_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.YSES_OAM_LATCH_DA6n.qn_new());
  /*_p29.YTUX*/ triwire YTUX_OLA_TO_CBD7_new = tri10_np(WUME_OBL_TO_CBDn_new, reg_new.oam_latch_a.ZECA_OAM_LATCH_DA7n.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(YFAP_OLA_TO_CBD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(XELE_OLA_TO_CBD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(YPON_OLA_TO_CBD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(XUVO_OLA_TO_CBD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(ZYSA_OLA_TO_CBD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(YWEG_OLA_TO_CBD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(XABU_OLA_TO_CBD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(YTUX_OLA_TO_CBD7_new);

  /*_p28.WUKU*/ wire WUKU_OBL_TO_CBDp_new = and3(reg_new.cpu_signals.LEKO_CPU_RDp_new(), AMAB_CPU_OAM_ENp_new, GEKA_OAM_A0p_new);
  /*_p28.WEWU*/ wire WEWU_OBL_TO_CBDn_new = not1(WUKU_OBL_TO_CBDp_new);
  /*_p31.XACA*/ triwire XACA_OLB_TO_CBD0_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.XYKY_OAM_LATCH_DB0n.qn_new());
  /*_p31.XAGU*/ triwire XAGU_OLB_TO_CBD1_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.YRUM_OAM_LATCH_DB1n.qn_new());
  /*_p31.XEPU*/ triwire XEPU_OLB_TO_CBD2_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.YSEX_OAM_LATCH_DB2n.qn_new());
  /*_p31.XYGU*/ triwire XYGU_OLB_TO_CBD3_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.YVEL_OAM_LATCH_DB3n.qn_new());
  /*_p31.XUNA*/ triwire XUNA_OLB_TO_CBD4_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.WYNO_OAM_LATCH_DB4n.qn_new());
  /*_p31.DEVE*/ triwire DEVE_OLB_TO_CBD5_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.CYRA_OAM_LATCH_DB5n.qn_new());
  /*_p31.ZEHA*/ triwire ZEHA_OLB_TO_CBD6_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.ZUVE_OAM_LATCH_DB6n.qn_new());
  /*_p31.FYRA*/ triwire FYRA_OLB_TO_CBD7_new = tri10_np(WEWU_OBL_TO_CBDn_new, reg_new.oam_latch_b.ECED_OAM_LATCH_DB7n.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(XACA_OLB_TO_CBD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(XAGU_OLB_TO_CBD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(XEPU_OLB_TO_CBD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(XYGU_OLB_TO_CBD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(XUNA_OLB_TO_CBD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(DEVE_OLB_TO_CBD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(ZEHA_OLB_TO_CBD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(FYRA_OLB_TO_CBD7_new);
}

//-----------------------------------------------------------------------------

void OamControl::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamControl::reset_to_bootrom() {
  MAKA_LATCH_EXTp.state  = 0b00011000;
  WUJE_CPU_OAM_WRn.state = 0b00011001;
  SIG_OAM_CLKn.state     = 0b00011001;
  SIG_OAM_WRn_A.state    = 0b00011001;
  SIG_OAM_WRn_B.state    = 0b00011001;
  SIG_OAM_OEn.state      = 0b00011001;
  old_oam_clk.state      = 0b00011001;
}

void OamControl::reset_to_cart() {
  MAKA_LATCH_EXTp.state  = 0b00011000;
  WUJE_CPU_OAM_WRn.state = 0b00011001;
  SIG_OAM_CLKn.state     = 0b00011001;
  SIG_OAM_WRn_A.state    = 0b00011001;
  SIG_OAM_WRn_B.state    = 0b00011001;
  SIG_OAM_OEn.state      = 0b00011001;
  old_oam_clk.state      = 0b00011001;
}

//-----------------------------------------------------------------------------

void OamABus::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

void OamABus::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

void OamABus::reset_to_cart() {
  BUS_OAM_A00n.state = BIT_OLD | BIT_DRIVEN | 1;
  BUS_OAM_A01n.state = BIT_OLD | BIT_DRIVEN | 1;
  BUS_OAM_A02n.state = BIT_OLD | BIT_DRIVEN | 1;
  BUS_OAM_A03n.state = BIT_OLD | BIT_DRIVEN | 1;
  BUS_OAM_A04n.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_OAM_A05n.state = BIT_OLD | BIT_DRIVEN | 1;
  BUS_OAM_A06n.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_OAM_A07n.state = BIT_OLD | BIT_DRIVEN | 1;
}

//-----------------------------------------------------------------------------

void OamDBusA::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamDBusA::reset_to_bootrom() {
  BUS_OAM_DA00n.state = 0b00011000;
  BUS_OAM_DA01n.state = 0b00011001;
  BUS_OAM_DA02n.state = 0b00011001;
  BUS_OAM_DA03n.state = 0b00011001;
  BUS_OAM_DA04n.state = 0b00011000;
  BUS_OAM_DA05n.state = 0b00011000;
  BUS_OAM_DA06n.state = 0b00011001;
  BUS_OAM_DA07n.state = 0b00011001;
}

void OamDBusA::reset_to_cart() {
  BUS_OAM_DA00n.state = 0b00011000;
  BUS_OAM_DA01n.state = 0b00011001;
  BUS_OAM_DA02n.state = 0b00011001;
  BUS_OAM_DA03n.state = 0b00011001;
  BUS_OAM_DA04n.state = 0b00011001;
  BUS_OAM_DA05n.state = 0b00011001;
  BUS_OAM_DA06n.state = 0b00011001;
  BUS_OAM_DA07n.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void OamDBusB::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamDBusB::reset_to_bootrom() {
  BUS_OAM_DB00n.state = 0b00011000;
  BUS_OAM_DB01n.state = 0b00011001;
  BUS_OAM_DB02n.state = 0b00011001;
  BUS_OAM_DB03n.state = 0b00011001;
  BUS_OAM_DB04n.state = 0b00011000;
  BUS_OAM_DB05n.state = 0b00011000;
  BUS_OAM_DB06n.state = 0b00011001;
  BUS_OAM_DB07n.state = 0b00011001;
}

void OamDBusB::reset_to_cart() {
  BUS_OAM_DB00n.state = 0b00011000;
  BUS_OAM_DB01n.state = 0b00011001;
  BUS_OAM_DB02n.state = 0b00011001;
  BUS_OAM_DB03n.state = 0b00011001;
  BUS_OAM_DB04n.state = 0b00011001;
  BUS_OAM_DB05n.state = 0b00011001;
  BUS_OAM_DB06n.state = 0b00011001;
  BUS_OAM_DB07n.state = 0b00011001;
}

//-----------------------------------------------------------------------------
