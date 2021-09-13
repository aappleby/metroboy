#include "GateBoyLib/GateBoyOamBus.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void GateBoy::oam_latch_to_temp_a(wire COTA_OAM_CLKn, const OamLatchA& oam_latch_a, OamTempA& oam_temp_a)
{
  /*_p29.YWOK*/ wire YWOK_OAM_CLKp = not1(COTA_OAM_CLKn); // inverting this clock does not break anything
  /*#p29.XUSO*/ oam_temp_a.XUSO_OAM_DA0p.dff8n(YWOK_OAM_CLKp, oam_latch_a.YDYV_OAM_LATCH_DA0n.qp_old());
  /*_p29.XEGU*/ oam_temp_a.XEGU_OAM_DA1p.dff8n(YWOK_OAM_CLKp, oam_latch_a.YCEB_OAM_LATCH_DA1n.qp_old());
  /*_p29.YJEX*/ oam_temp_a.YJEX_OAM_DA2p.dff8n(YWOK_OAM_CLKp, oam_latch_a.ZUCA_OAM_LATCH_DA2n.qp_old());
  /*_p29.XYJU*/ oam_temp_a.XYJU_OAM_DA3p.dff8n(YWOK_OAM_CLKp, oam_latch_a.WONE_OAM_LATCH_DA3n.qp_old());
  /*_p29.YBOG*/ oam_temp_a.YBOG_OAM_DA4p.dff8n(YWOK_OAM_CLKp, oam_latch_a.ZAXE_OAM_LATCH_DA4n.qp_old());
  /*_p29.WYSO*/ oam_temp_a.WYSO_OAM_DA5p.dff8n(YWOK_OAM_CLKp, oam_latch_a.XAFU_OAM_LATCH_DA5n.qp_old());
  /*_p29.XOTE*/ oam_temp_a.XOTE_OAM_DA6p.dff8n(YWOK_OAM_CLKp, oam_latch_a.YSES_OAM_LATCH_DA6n.qp_old());
  /*_p29.YZAB*/ oam_temp_a.YZAB_OAM_DA7p.dff8n(YWOK_OAM_CLKp, oam_latch_a.ZECA_OAM_LATCH_DA7n.qp_old());
}

void GateBoy::oam_latch_to_temp_b(wire COTA_OAM_CLKn, const OamLatchB& oam_latch_b, OamTempB& oam_temp_b)
{
  /*#p31.XEGA*/ wire XEGA_OAM_CLKp = not1(COTA_OAM_CLKn); // inverting this clock does not break anything
  /*_p31.YLOR*/ oam_temp_b.YLOR_OAM_DB0p.dff8n(XEGA_OAM_CLKp, oam_latch_b.XYKY_OAM_LATCH_DB0n.qp_old());
  /*_p31.ZYTY*/ oam_temp_b.ZYTY_OAM_DB1p.dff8n(XEGA_OAM_CLKp, oam_latch_b.YRUM_OAM_LATCH_DB1n.qp_old());
  /*_p31.ZYVE*/ oam_temp_b.ZYVE_OAM_DB2p.dff8n(XEGA_OAM_CLKp, oam_latch_b.YSEX_OAM_LATCH_DB2n.qp_old());
  /*_p31.ZEZY*/ oam_temp_b.ZEZY_OAM_DB3p.dff8n(XEGA_OAM_CLKp, oam_latch_b.YVEL_OAM_LATCH_DB3n.qp_old());
  /*_p31.GOMO*/ oam_temp_b.GOMO_OAM_DB4p.dff8n(XEGA_OAM_CLKp, oam_latch_b.WYNO_OAM_LATCH_DB4n.qp_old());
  /*_p31.BAXO*/ oam_temp_b.BAXO_OAM_DB5p.dff8n(XEGA_OAM_CLKp, oam_latch_b.CYRA_OAM_LATCH_DB5n.qp_old());
  /*_p31.YZOS*/ oam_temp_b.YZOS_OAM_DB6p.dff8n(XEGA_OAM_CLKp, oam_latch_b.ZUVE_OAM_LATCH_DB6n.qp_old());
  /*_p31.DEPO*/ oam_temp_b.DEPO_OAM_DB7p.dff8n(XEGA_OAM_CLKp, oam_latch_b.ECED_OAM_LATCH_DB7n.qp_old());
}

//-----------------------------------------------------------------------------

void GateBoy::tock_oam_bus_gates()
{
  memset(&reg.oam_abus,   BIT_NEW | BIT_PULLED | 1, sizeof(reg.oam_abus));
  memset(&reg.oam_dbus_a, BIT_NEW | BIT_PULLED | 1, sizeof(reg.oam_dbus_a));
  memset(&reg.oam_dbus_b, BIT_NEW | BIT_PULLED | 1, sizeof(reg.oam_dbus_b));

  // OAM address from CPU
  /*_p28.ASAM*/ wire ASAM_CPU_OAM_RDn = or3(reg.ACYL_SCANNINGp.out_new(), reg.XYMU_RENDERINGn.qn_new(), reg.MATU_DMA_RUNNINGp.qp_new());
  /*_p28.GARO*/ triwire GARO_CA0_TO_OA0 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A00p.out_new());
  /*_p28.WACU*/ triwire WACU_CA1_TO_OA1 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A01p.out_new());
  /*_p28.GOSE*/ triwire GOSE_CA2_TO_OA2 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A02p.out_new());
  /*_p28.WAPE*/ triwire WAPE_CA3_TO_OA3 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A03p.out_new());
  /*_p28.FEVU*/ triwire FEVU_CA4_TO_OA4 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A04p.out_new());
  /*_p28.GERA*/ triwire GERA_CA5_TO_OA5 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A05p.out_new());
  /*_p28.WAXA*/ triwire WAXA_CA6_TO_OA6 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A06p.out_new());
  /*_p28.FOBY*/ triwire FOBY_CA7_TO_OA7 = tri6_nn(ASAM_CPU_OAM_RDn, reg.cpu_abus.BUS_CPU_A07p.out_new());

  /*_BUS_OAM_A00n*/ reg.oam_abus.BUS_OAM_A00n.tri_bus(GARO_CA0_TO_OA0);
  /*_BUS_OAM_A01n*/ reg.oam_abus.BUS_OAM_A01n.tri_bus(WACU_CA1_TO_OA1);
  /*_BUS_OAM_A02n*/ reg.oam_abus.BUS_OAM_A02n.tri_bus(GOSE_CA2_TO_OA2);
  /*_BUS_OAM_A03n*/ reg.oam_abus.BUS_OAM_A03n.tri_bus(WAPE_CA3_TO_OA3);
  /*_BUS_OAM_A04n*/ reg.oam_abus.BUS_OAM_A04n.tri_bus(FEVU_CA4_TO_OA4);
  /*_BUS_OAM_A05n*/ reg.oam_abus.BUS_OAM_A05n.tri_bus(GERA_CA5_TO_OA5);
  /*_BUS_OAM_A06n*/ reg.oam_abus.BUS_OAM_A06n.tri_bus(WAXA_CA6_TO_OA6);
  /*_BUS_OAM_A07n*/ reg.oam_abus.BUS_OAM_A07n.tri_bus(FOBY_CA7_TO_OA7);

  // DMA OAM write address driver
  /*_p04.DUGA*/ wire DUGA_DMA_RUNNINGn = not1(reg.MATU_DMA_RUNNINGp.qp_new());
  /*_p28.FODO*/ triwire FODO_DMA0_TO_OA0 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.NAKY_DMA_A00p.qp_new());
  /*_p28.FESA*/ triwire FESA_DMA1_TO_OA1 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.PYRO_DMA_A01p.qp_new());
  /*_p28.FAGO*/ triwire FAGO_DMA2_TO_OA2 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.NEFY_DMA_A02p.qp_new());
  /*_p28.FYKY*/ triwire FYKY_DMA3_TO_OA3 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.MUTY_DMA_A03p.qp_new());
  /*_p28.ELUG*/ triwire ELUG_DMA4_TO_OA4 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.NYKO_DMA_A04p.qp_new());
  /*_p28.EDOL*/ triwire EDOL_DMA5_TO_OA5 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.PYLO_DMA_A05p.qp_new());
  /*_p28.FYDU*/ triwire FYDU_DMA6_TO_OA6 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.NUTO_DMA_A06p.qp_new());
  /*_p28.FETU*/ triwire FETU_DMA7_TO_OA7 = tri6_nn(DUGA_DMA_RUNNINGn, reg.dma_lo.MUGU_DMA_A07p.qp_new());

  /*_BUS_OAM_A00n*/ reg.oam_abus.BUS_OAM_A00n.tri_bus(FODO_DMA0_TO_OA0);
  /*_BUS_OAM_A01n*/ reg.oam_abus.BUS_OAM_A01n.tri_bus(FESA_DMA1_TO_OA1);
  /*_BUS_OAM_A02n*/ reg.oam_abus.BUS_OAM_A02n.tri_bus(FAGO_DMA2_TO_OA2);
  /*_BUS_OAM_A03n*/ reg.oam_abus.BUS_OAM_A03n.tri_bus(FYKY_DMA3_TO_OA3);
  /*_BUS_OAM_A04n*/ reg.oam_abus.BUS_OAM_A04n.tri_bus(ELUG_DMA4_TO_OA4);
  /*_BUS_OAM_A05n*/ reg.oam_abus.BUS_OAM_A05n.tri_bus(EDOL_DMA5_TO_OA5);
  /*_BUS_OAM_A06n*/ reg.oam_abus.BUS_OAM_A06n.tri_bus(FYDU_DMA6_TO_OA6);
  /*_BUS_OAM_A07n*/ reg.oam_abus.BUS_OAM_A07n.tri_bus(FETU_DMA7_TO_OA7);

  // OAM address from sprite scanner
  /*_p28.APAR*/ wire APAR_SCANNINGn = not1(reg.ACYL_SCANNINGp.out_new());
  /*_p28.GEFY*/ triwire GEFY_SCANX_TO_OA0 = tri6_nn(APAR_SCANNINGn, reg.SIG_GND.out_new());
  /*_p28.WUWE*/ triwire WUWE_SCANX_TO_OA1 = tri6_nn(APAR_SCANNINGn, reg.SIG_GND.out_new());
  /*_p28.GUSE*/ triwire GUSE_SCAN0_TO_OA2 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.YFEL_SCAN0.qp_new());
  /*_p28.GEMA*/ triwire GEMA_SCAN1_TO_OA3 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.WEWY_SCAN1.qp_new());
  /*_p28.FUTO*/ triwire FUTO_SCAN2_TO_OA4 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.GOSO_SCAN2.qp_new());
  /*_p28.FAKU*/ triwire FAKU_SCAN3_TO_OA5 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.ELYN_SCAN3.qp_new());
  /*_p28.GAMA*/ triwire GAMA_SCAN4_TO_OA6 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.FAHA_SCAN4.qp_new());
  /*_p28.GOBY*/ triwire GOBY_SCAN5_TO_OA7 = tri6_nn(APAR_SCANNINGn, reg.scan_counter.FONY_SCAN5.qp_new());

  /*_BUS_OAM_A00n*/ reg.oam_abus.BUS_OAM_A00n.tri_bus(GEFY_SCANX_TO_OA0);
  /*_BUS_OAM_A01n*/ reg.oam_abus.BUS_OAM_A01n.tri_bus(WUWE_SCANX_TO_OA1);
  /*_BUS_OAM_A02n*/ reg.oam_abus.BUS_OAM_A02n.tri_bus(GUSE_SCAN0_TO_OA2);
  /*_BUS_OAM_A03n*/ reg.oam_abus.BUS_OAM_A03n.tri_bus(GEMA_SCAN1_TO_OA3);
  /*_BUS_OAM_A04n*/ reg.oam_abus.BUS_OAM_A04n.tri_bus(FUTO_SCAN2_TO_OA4);
  /*_BUS_OAM_A05n*/ reg.oam_abus.BUS_OAM_A05n.tri_bus(FAKU_SCAN3_TO_OA5);
  /*_BUS_OAM_A06n*/ reg.oam_abus.BUS_OAM_A06n.tri_bus(GAMA_SCAN4_TO_OA6);
  /*_BUS_OAM_A07n*/ reg.oam_abus.BUS_OAM_A07n.tri_bus(GOBY_SCAN5_TO_OA7);

  // OAM address from sprite fetcher
  /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(reg.MATU_DMA_RUNNINGp.qp_new());
  /*_p28.AJON*/ wire AJON_RENDERINGp = and2(BOGE_DMA_RUNNINGn, reg.XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /*_p28.BETE*/ wire BETE_SPR_I_TO_OAM_An = not1(AJON_RENDERINGp);
  /*_p28.GECA*/ triwire GECA_FETCHX_TO_OA0 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.SIG_VCC.out_new());
  /*_p28.WYDU*/ triwire WYDU_FETCHX_TO_OA1 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.SIG_VCC.out_new());
  /*_p28.GYBU*/ triwire GYBU_FETCH0_TO_OA2 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I0.out_new());
  /*_p28.GYKA*/ triwire GYKA_FETCH1_TO_OA3 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I1.out_new());
  /*_p28.FABY*/ triwire FABY_FETCH2_TO_OA4 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I2.out_new());
  /*_p28.FACO*/ triwire FACO_FETCH3_TO_OA5 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I3.out_new());
  /*_p28.FUGU*/ triwire FUGU_FETCH4_TO_OA6 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I4.out_new());
  /*_p28.FYKE*/ triwire FYKE_FETCH5_TO_OA7 = tri6_nn(BETE_SPR_I_TO_OAM_An, reg.sprite_ibus.BUS_SPR_I5.out_new());

  /*_BUS_OAM_A00n*/ reg.oam_abus.BUS_OAM_A00n.tri_bus(GECA_FETCHX_TO_OA0);
  /*_BUS_OAM_A01n*/ reg.oam_abus.BUS_OAM_A01n.tri_bus(WYDU_FETCHX_TO_OA1);
  /*_BUS_OAM_A02n*/ reg.oam_abus.BUS_OAM_A02n.tri_bus(GYBU_FETCH0_TO_OA2);
  /*_BUS_OAM_A03n*/ reg.oam_abus.BUS_OAM_A03n.tri_bus(GYKA_FETCH1_TO_OA3);
  /*_BUS_OAM_A04n*/ reg.oam_abus.BUS_OAM_A04n.tri_bus(FABY_FETCH2_TO_OA4);
  /*_BUS_OAM_A05n*/ reg.oam_abus.BUS_OAM_A05n.tri_bus(FACO_FETCH3_TO_OA5);
  /*_BUS_OAM_A06n*/ reg.oam_abus.BUS_OAM_A06n.tri_bus(FUGU_FETCH4_TO_OA6);
  /*_BUS_OAM_A07n*/ reg.oam_abus.BUS_OAM_A07n.tri_bus(FYKE_FETCH5_TO_OA7);

  //probe_wire(16, "ASAM CPU", ASAM_CPU_OAM_RDn);
  //probe_wire(17, "DUGA DMA", DUGA_DMA_RUNNINGn);
  //probe_wire(18, "APAR SCN", APAR_SCANNINGn);
  //probe_wire(19, "BETE PPU", BETE_SPR_I_TO_OAM_An);

  // cpu to oam data bus

  /*_p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and2(reg.cpu_abus.SARO_ADDR_OAMp(), CUPA_CPU_WRp());
  /*_p28.WUJE*/ reg.oam_ctrl.WUJE_CPU_OAM_WRn.nor_latch(XYNY_ABCDxxxx(), XUTO_CPU_OAM_WRp);

  /*_p28.XUPA*/ wire XUPA_CPU_OAM_WRp  = not1(reg.oam_ctrl.WUJE_CPU_OAM_WRn.qp_new());
  /*#p28.AJUJ*/ wire AJUJ_OAM_BUSYn    = nor3(reg.MATU_DMA_RUNNINGp.qp_new(), reg.ACYL_SCANNINGp.out_new(), AJON_RENDERINGp);
  /*#p28.AMAB*/ wire AMAB_CPU_OAM_ENp  = and2(reg.cpu_abus.SARO_ADDR_OAMp(), AJUJ_OAM_BUSYn); // def and
  /*#p28.APAG*/ wire APAG_CBD_TO_OBDp  = amux2(XUPA_CPU_OAM_WRp, AMAB_CPU_OAM_ENp, AJUJ_OAM_BUSYn, reg.cpu_abus.ADAH_FE00_FEFFn());
  /*#p28.AZUL*/ wire AZUL_CBD_TO_OBDn  = not1(APAG_CBD_TO_OBDp);

  /*_p28.ZAMY*/ triwire ZAMY_CD0_TO_ODA0 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p28.ZOPU*/ triwire ZOPU_CD1_TO_ODA1 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p28.WYKY*/ triwire WYKY_CD2_TO_ODA2 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p28.ZAJA*/ triwire ZAJA_CD3_TO_ODA3 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p28.ZUGA*/ triwire ZUGA_CD4_TO_ODA4 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D04p.out_new());
  /*_p28.ZUMO*/ triwire ZUMO_CD5_TO_ODA5 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D05p.out_new());
  /*_p28.XYTO*/ triwire XYTO_CD6_TO_ODA6 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D06p.out_new());
  /*_p28.ZYFA*/ triwire ZYFA_CD7_TO_ODA7 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D07p.out_new());

  /*_p28.ZAXA*/ triwire ZAXA_CD0_TO_ODB0 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p28.ZAKY*/ triwire ZAKY_CD1_TO_ODB1 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p28.WULE*/ triwire WULE_CD2_TO_ODB2 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p28.ZOZO*/ triwire ZOZO_CD3_TO_ODB3 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p28.ZUFO*/ triwire ZUFO_CD4_TO_ODB4 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D04p.out_new());
  /*_p28.ZATO*/ triwire ZATO_CD5_TO_ODB5 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D05p.out_new());
  /*_p28.YVUC*/ triwire YVUC_CD6_TO_ODB6 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D06p.out_new());
  /*_p28.ZUFE*/ triwire ZUFE_CD7_TO_ODB7 = tri6_nn(AZUL_CBD_TO_OBDn, reg.cpu_dbus.BUS_CPU_D07p.out_new());

  /*_BUS_OAM_DA00n*/ reg.oam_dbus_a.BUS_OAM_DA00n.tri_bus(ZAMY_CD0_TO_ODA0);
  /*_BUS_OAM_DA01n*/ reg.oam_dbus_a.BUS_OAM_DA01n.tri_bus(ZOPU_CD1_TO_ODA1);
  /*_BUS_OAM_DA02n*/ reg.oam_dbus_a.BUS_OAM_DA02n.tri_bus(WYKY_CD2_TO_ODA2);
  /*_BUS_OAM_DA03n*/ reg.oam_dbus_a.BUS_OAM_DA03n.tri_bus(ZAJA_CD3_TO_ODA3);
  /*_BUS_OAM_DA04n*/ reg.oam_dbus_a.BUS_OAM_DA04n.tri_bus(ZUGA_CD4_TO_ODA4);
  /*_BUS_OAM_DA05n*/ reg.oam_dbus_a.BUS_OAM_DA05n.tri_bus(ZUMO_CD5_TO_ODA5);
  /*_BUS_OAM_DA06n*/ reg.oam_dbus_a.BUS_OAM_DA06n.tri_bus(XYTO_CD6_TO_ODA6);
  /*_BUS_OAM_DA07n*/ reg.oam_dbus_a.BUS_OAM_DA07n.tri_bus(ZYFA_CD7_TO_ODA7);

  /*_BUS_OAM_DB00n*/ reg.oam_dbus_b.BUS_OAM_DB00n.tri_bus(ZAXA_CD0_TO_ODB0);
  /*_BUS_OAM_DB01n*/ reg.oam_dbus_b.BUS_OAM_DB01n.tri_bus(ZAKY_CD1_TO_ODB1);
  /*_BUS_OAM_DB02n*/ reg.oam_dbus_b.BUS_OAM_DB02n.tri_bus(WULE_CD2_TO_ODB2);
  /*_BUS_OAM_DB03n*/ reg.oam_dbus_b.BUS_OAM_DB03n.tri_bus(ZOZO_CD3_TO_ODB3);
  /*_BUS_OAM_DB04n*/ reg.oam_dbus_b.BUS_OAM_DB04n.tri_bus(ZUFO_CD4_TO_ODB4);
  /*_BUS_OAM_DB05n*/ reg.oam_dbus_b.BUS_OAM_DB05n.tri_bus(ZATO_CD5_TO_ODB5);
  /*_BUS_OAM_DB06n*/ reg.oam_dbus_b.BUS_OAM_DB06n.tri_bus(YVUC_CD6_TO_ODB6);
  /*_BUS_OAM_DB07n*/ reg.oam_dbus_b.BUS_OAM_DB07n.tri_bus(ZUFE_CD7_TO_ODB7);

  // ext dma to oam data bus

  /*_p25.RALO*/ wire RALO_EXT_D0p = not1(reg.ext_dbus.PIN_17_D00.qp_int_new());
  /*_p25.TUNE*/ wire TUNE_EXT_D1p = not1(reg.ext_dbus.PIN_18_D01.qp_int_new());
  /*_p25.SERA*/ wire SERA_EXT_D2p = not1(reg.ext_dbus.PIN_19_D02.qp_int_new());
  /*_p25.TENU*/ wire TENU_EXT_D3p = not1(reg.ext_dbus.PIN_20_D03.qp_int_new());
  /*_p25.SYSA*/ wire SYSA_EXT_D4p = not1(reg.ext_dbus.PIN_21_D04.qp_int_new());
  /*_p25.SUGY*/ wire SUGY_EXT_D5p = not1(reg.ext_dbus.PIN_22_D05.qp_int_new());
  /*_p25.TUBE*/ wire TUBE_EXT_D6p = not1(reg.ext_dbus.PIN_23_D06.qp_int_new());
  /*_p25.SYZO*/ wire SYZO_EXT_D7p = not1(reg.ext_dbus.PIN_24_D07.qp_int_new());

  /*#p04.LEBU*/ wire LEBU_DMA_A15n  = not1(reg.reg_dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp = nor3(reg.reg_dma.PULA_DMA_A13n.qn_new(), reg.reg_dma.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n);
  /*_p04.LOGO*/ wire LOGO_DMA_VRAMn = not1(MUDA_DMA_VRAMp);
  /*_p04.MORY*/ wire MORY_DMA_CARTn = nand2(reg.MATU_DMA_RUNNINGp.qp_new(), LOGO_DMA_VRAMn);
  /*_p04.LUMA*/ wire LUMA_DMA_CARTp = not1(MORY_DMA_CARTn);
  /*_p25.CEDE*/ wire CEDE_EBD_TO_OBDn = not1(LUMA_DMA_CARTp);

  /*_p25.WASA*/ triwire WASA_ED0_TO_ODA0 = tri6_nn(CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
  /*_p25.BOMO*/ triwire BOMO_ED1_TO_ODA1 = tri6_nn(CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
  /*_p25.BASA*/ triwire BASA_ED2_TO_ODA2 = tri6_nn(CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
  /*_p25.CAKO*/ triwire CAKO_ED3_TO_ODA3 = tri6_nn(CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
  /*_p25.BUMA*/ triwire BUMA_ED4_TO_ODA4 = tri6_nn(CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
  /*_p25.BUPY*/ triwire BUPY_ED5_TO_ODA5 = tri6_nn(CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
  /*_p25.BASY*/ triwire BASY_ED6_TO_ODA6 = tri6_nn(CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
  /*_p25.BAPE*/ triwire BAPE_ED7_TO_ODA7 = tri6_nn(CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);

  /*_p25.WEJO*/ triwire WEJO_ED0_TO_ODB0 = tri6_nn(CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
  /*_p25.BUBO*/ triwire BUBO_ED1_TO_ODB1 = tri6_nn(CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
  /*_p25.BETU*/ triwire BETU_ED2_TO_ODB2 = tri6_nn(CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
  /*_p25.CYME*/ triwire CYME_ED3_TO_ODB3 = tri6_nn(CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
  /*_p25.BAXU*/ triwire BAXU_ED4_TO_ODB4 = tri6_nn(CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
  /*_p25.BUHU*/ triwire BUHU_ED5_TO_ODB5 = tri6_nn(CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
  /*_p25.BYNY*/ triwire BYNY_ED6_TO_ODB6 = tri6_nn(CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
  /*_p25.BYPY*/ triwire BYPY_ED7_TO_ODB7 = tri6_nn(CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);

  /*_BUS_OAM_DA00n*/ reg.oam_dbus_a.BUS_OAM_DA00n.tri_bus(WASA_ED0_TO_ODA0);
  /*_BUS_OAM_DA01n*/ reg.oam_dbus_a.BUS_OAM_DA01n.tri_bus(BOMO_ED1_TO_ODA1);
  /*_BUS_OAM_DA02n*/ reg.oam_dbus_a.BUS_OAM_DA02n.tri_bus(BASA_ED2_TO_ODA2);
  /*_BUS_OAM_DA03n*/ reg.oam_dbus_a.BUS_OAM_DA03n.tri_bus(CAKO_ED3_TO_ODA3);
  /*_BUS_OAM_DA04n*/ reg.oam_dbus_a.BUS_OAM_DA04n.tri_bus(BUMA_ED4_TO_ODA4);
  /*_BUS_OAM_DA05n*/ reg.oam_dbus_a.BUS_OAM_DA05n.tri_bus(BUPY_ED5_TO_ODA5);
  /*_BUS_OAM_DA06n*/ reg.oam_dbus_a.BUS_OAM_DA06n.tri_bus(BASY_ED6_TO_ODA6);
  /*_BUS_OAM_DA07n*/ reg.oam_dbus_a.BUS_OAM_DA07n.tri_bus(BAPE_ED7_TO_ODA7);

  /*_BUS_OAM_DB00n*/ reg.oam_dbus_b.BUS_OAM_DB00n.tri_bus(WEJO_ED0_TO_ODB0);
  /*_BUS_OAM_DB01n*/ reg.oam_dbus_b.BUS_OAM_DB01n.tri_bus(BUBO_ED1_TO_ODB1);
  /*_BUS_OAM_DB02n*/ reg.oam_dbus_b.BUS_OAM_DB02n.tri_bus(BETU_ED2_TO_ODB2);
  /*_BUS_OAM_DB03n*/ reg.oam_dbus_b.BUS_OAM_DB03n.tri_bus(CYME_ED3_TO_ODB3);
  /*_BUS_OAM_DB04n*/ reg.oam_dbus_b.BUS_OAM_DB04n.tri_bus(BAXU_ED4_TO_ODB4);
  /*_BUS_OAM_DB05n*/ reg.oam_dbus_b.BUS_OAM_DB05n.tri_bus(BUHU_ED5_TO_ODB5);
  /*_BUS_OAM_DB06n*/ reg.oam_dbus_b.BUS_OAM_DB06n.tri_bus(BYNY_ED6_TO_ODB6);
  /*_BUS_OAM_DB07n*/ reg.oam_dbus_b.BUS_OAM_DB07n.tri_bus(BYPY_ED7_TO_ODB7);

  // vram dma to oam data bus

  /*_p04.MUHO*/ wire MUHO_DMA_VRAMp = nand2(reg.MATU_DMA_RUNNINGp.qp_new(), MUDA_DMA_VRAMp);
  /*_p04.LUFA*/ wire LUFA_DMA_VRAMp = not1(MUHO_DMA_VRAMp);
  /*_p28.AZAR*/ wire AZAR_VBD_TO_OBDn = not1(LUFA_DMA_VRAMp);

  /*_p28.WUZU*/ triwire WUZU_VD0_TO_ODA0 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D00p.out_new());
  /*_p28.AXER*/ triwire AXER_VD1_TO_ODA1 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D01p.out_new());
  /*_p28.ASOX*/ triwire ASOX_VD2_TO_ODA2 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D02p.out_new());
  /*_p28.CETU*/ triwire CETU_VD3_TO_ODA3 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D03p.out_new());
  /*_p28.ARYN*/ triwire ARYN_VD4_TO_ODA4 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D04p.out_new());
  /*_p28.ACOT*/ triwire ACOT_VD5_TO_ODA5 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D05p.out_new());
  /*_p28.CUJE*/ triwire CUJE_VD6_TO_ODA6 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D06p.out_new());
  /*_p28.ATER*/ triwire ATER_VD7_TO_ODA7 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D07p.out_new());

  /*_p28.WOWA*/ triwire WOWA_VD0_TO_ODB0 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D00p.out_new());
  /*_p28.AVEB*/ triwire AVEB_VD1_TO_ODB1 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D01p.out_new());
  /*_p28.AMUH*/ triwire AMUH_VD2_TO_ODB2 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D02p.out_new());
  /*_p28.COFO*/ triwire COFO_VD3_TO_ODB3 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D03p.out_new());
  /*_p28.AZOZ*/ triwire AZOZ_VD4_TO_ODB4 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D04p.out_new());
  /*_p28.AGYK*/ triwire AGYK_VD5_TO_ODB5 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D05p.out_new());
  /*_p28.BUSE*/ triwire BUSE_VD6_TO_ODB6 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D06p.out_new());
  /*_p28.ANUM*/ triwire ANUM_VD7_TO_ODB7 = tri6_nn(AZAR_VBD_TO_OBDn, reg.vram_dbus.BUS_VRAM_D07p.out_new());

  /*_BUS_OAM_DA00n*/ reg.oam_dbus_a.BUS_OAM_DA00n.tri_bus(WUZU_VD0_TO_ODA0);
  /*_BUS_OAM_DA01n*/ reg.oam_dbus_a.BUS_OAM_DA01n.tri_bus(AXER_VD1_TO_ODA1);
  /*_BUS_OAM_DA02n*/ reg.oam_dbus_a.BUS_OAM_DA02n.tri_bus(ASOX_VD2_TO_ODA2);
  /*_BUS_OAM_DA03n*/ reg.oam_dbus_a.BUS_OAM_DA03n.tri_bus(CETU_VD3_TO_ODA3);
  /*_BUS_OAM_DA04n*/ reg.oam_dbus_a.BUS_OAM_DA04n.tri_bus(ARYN_VD4_TO_ODA4);
  /*_BUS_OAM_DA05n*/ reg.oam_dbus_a.BUS_OAM_DA05n.tri_bus(ACOT_VD5_TO_ODA5);
  /*_BUS_OAM_DA06n*/ reg.oam_dbus_a.BUS_OAM_DA06n.tri_bus(CUJE_VD6_TO_ODA6);
  /*_BUS_OAM_DA07n*/ reg.oam_dbus_a.BUS_OAM_DA07n.tri_bus(ATER_VD7_TO_ODA7);

  /*_BUS_OAM_DB00n*/ reg.oam_dbus_b.BUS_OAM_DB00n.tri_bus(WOWA_VD0_TO_ODB0);
  /*_BUS_OAM_DB01n*/ reg.oam_dbus_b.BUS_OAM_DB01n.tri_bus(AVEB_VD1_TO_ODB1);
  /*_BUS_OAM_DB02n*/ reg.oam_dbus_b.BUS_OAM_DB02n.tri_bus(AMUH_VD2_TO_ODB2);
  /*_BUS_OAM_DB03n*/ reg.oam_dbus_b.BUS_OAM_DB03n.tri_bus(COFO_VD3_TO_ODB3);
  /*_BUS_OAM_DB04n*/ reg.oam_dbus_b.BUS_OAM_DB04n.tri_bus(AZOZ_VD4_TO_ODB4);
  /*_BUS_OAM_DB05n*/ reg.oam_dbus_b.BUS_OAM_DB05n.tri_bus(AGYK_VD5_TO_ODB5);
  /*_BUS_OAM_DB06n*/ reg.oam_dbus_b.BUS_OAM_DB06n.tri_bus(BUSE_VD6_TO_ODB6);
  /*_BUS_OAM_DB07n*/ reg.oam_dbus_b.BUS_OAM_DB07n.tri_bus(ANUM_VD7_TO_ODB7);

  /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(reg.ACYL_SCANNINGp.out_new(), XYSO_xBCDxFGH());
  /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(reg.cpu_abus.SARO_ADDR_OAMp(), reg.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
  /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(reg.sfetch_counter.TOXE_SFETCH_S0p.qp_new());
  /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(reg.sfetch_control.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
  /*_p29.TEPA*/ wire TEPA_RENDERINGp = not1(reg.XYMU_RENDERINGn.qn_new());
  /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, reg.sfetch_counter.TULY_SFETCH_S1p.qp_new(), reg.sfetch_counter.TESE_SFETCH_S2p.qp_new());
  /*_p25.VAPE*/ wire VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  /*_p25.XUJY*/ wire XUJY_OAM_CLKENp = not1(VAPE_OAM_CLKENn);
  /*_p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, XUJY_OAM_CLKENp, CUFE_OAM_CLKp);
  /*_p25.COTA*/ wire COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);

  /*_SIG_OAM_CLKn*/ reg.oam_ctrl.SIG_OAM_CLKn.sig_out(COTA_OAM_CLKn);

  /*#p28.GEKA*/ wire GEKA_OAM_A0p = not1(reg.oam_abus.BUS_OAM_A00n.out_new());
  /*_p28.WAFO*/ wire WAFO_OAM_A0n = not1(GEKA_OAM_A0p);

  /*_p04.MAKA*/ reg.oam_ctrl.MAKA_LATCH_EXTp.dff17(ZEME_AxCxExGx(), AVOR_SYS_RSTp(), CATY_LATCH_EXTp());

  /*_p04.NAXY*/ wire NAXY_DMA_OAM_WRp = nor2(UVYT_ABCDxxxx(), reg.oam_ctrl.MAKA_LATCH_EXTp.qp_new()); // def nor2
  /*_p04.POWU*/ wire POWU_DMA_OAM_WRp = and2(reg.MATU_DMA_RUNNINGp.qp_new(), NAXY_DMA_OAM_WRp); // def and
  /*_p04.WYJA*/ wire WYJA_OAM_WRp     = and_or3(AMAB_CPU_OAM_ENp, CUPA_CPU_WRp(), POWU_DMA_OAM_WRp);
  /*_p28.YNYC*/ wire YNYC_OAM_A_WRp = and2(WYJA_OAM_WRp, WAFO_OAM_A0n);
  /*_p28.YLYC*/ wire YLYC_OAM_B_WRp = and2(WYJA_OAM_WRp, GEKA_OAM_A0p);
  /*_p28.ZOFE*/ wire ZOFE_OAM_A_WRn = not1(YNYC_OAM_A_WRp);
  /*_p28.ZONE*/ wire ZONE_OAM_B_WRn = not1(YLYC_OAM_B_WRp);

  /*_SIG_OAM_WRn_A*/ reg.oam_ctrl.SIG_OAM_WRn_A.sig_out(ZOFE_OAM_A_WRn);
  /*_SIG_OAM_WRn_B*/ reg.oam_ctrl.SIG_OAM_WRn_B.sig_out(ZONE_OAM_B_WRn);

  /*#p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn = nand2(reg.ACYL_SCANNINGp.out_new(), XOCE_xBCxxFGx()); // schematic wrong, is def nand2
  /*_p28.WEFY*/ wire WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp, reg.sfetch_control.TYFO_SFETCH_S0p_D1.qp_new());
  /*#p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn  = not1(WEFY_SPR_READp);

  /*#p28.BOTA*/ wire BOTA_OAM_OEn  = nand3(BOFE_LATCH_EXTn(), reg.cpu_abus.SARO_ADDR_OAMp(), ASOT_CPU_RDp()); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire ASYT_OAM_OEn = and3(AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, BOTA_OAM_OEn); // def and

  /*#p28.BODE*/ wire BODE_OAM_OEp = not1(ASYT_OAM_OEn);
  /*#p28.YVAL*/ wire YVAL_OAM_OEn = not1(BODE_OAM_OEp);
  /*#p28.YRYV*/ wire YRYV_OAM_OEp = not1(YVAL_OAM_OEn);
  /*#p28.ZODO*/ wire ZODO_OAM_OEn = not1(YRYV_OAM_OEp);   // schematic thinks this is OAM_CLK?
  /*_SIG_OAM_OEn*/ reg.oam_ctrl.SIG_OAM_OEn.sig_out(ZODO_OAM_OEn);

  // FIXME do the pack
  uint8_t oam_addr   = (uint8_t)bit_pack_inv(reg.oam_abus) >> 1;
  uint8_t oam_data_a = (uint8_t)bit_pack_inv(reg.oam_dbus_a);
  uint8_t oam_data_b = (uint8_t)bit_pack_inv(reg.oam_dbus_b);

  if (bit(~reg.oam_ctrl.old_oam_clk.out_old()) && bit(~reg.oam_ctrl.SIG_OAM_CLKn.out_new())) {
    if (bit(~reg.oam_ctrl.SIG_OAM_WRn_A.out_new())) mem.oam_ram[(oam_addr << 1) + 0] = oam_data_a;
    if (bit(~reg.oam_ctrl.SIG_OAM_WRn_B.out_new())) mem.oam_ram[(oam_addr << 1) + 1] = oam_data_b;
  }

  oam_data_a = mem.oam_ram[(oam_addr << 1) + 0];
  oam_data_b = mem.oam_ram[(oam_addr << 1) + 1];

  reg.oam_ctrl.old_oam_clk = bit(~reg.oam_ctrl.SIG_OAM_CLKn.out_new());

  triwire oam_data_a0 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 0));
  triwire oam_data_a1 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 1));
  triwire oam_data_a2 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 2));
  triwire oam_data_a3 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 3));
  triwire oam_data_a4 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 4));
  triwire oam_data_a5 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 5));
  triwire oam_data_a6 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 6));
  triwire oam_data_a7 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_a, 7));

  triwire oam_data_b0 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 0));
  triwire oam_data_b1 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 1));
  triwire oam_data_b2 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 2));
  triwire oam_data_b3 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 3));
  triwire oam_data_b4 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 4));
  triwire oam_data_b5 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 5));
  triwire oam_data_b6 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 6));
  triwire oam_data_b7 = tri6_nn(ZODO_OAM_OEn, get_bit(oam_data_b, 7));

  reg.oam_dbus_a.BUS_OAM_DA00n.tri_bus(oam_data_a0);
  reg.oam_dbus_a.BUS_OAM_DA01n.tri_bus(oam_data_a1);
  reg.oam_dbus_a.BUS_OAM_DA02n.tri_bus(oam_data_a2);
  reg.oam_dbus_a.BUS_OAM_DA03n.tri_bus(oam_data_a3);
  reg.oam_dbus_a.BUS_OAM_DA04n.tri_bus(oam_data_a4);
  reg.oam_dbus_a.BUS_OAM_DA05n.tri_bus(oam_data_a5);
  reg.oam_dbus_a.BUS_OAM_DA06n.tri_bus(oam_data_a6);
  reg.oam_dbus_a.BUS_OAM_DA07n.tri_bus(oam_data_a7);

  reg.oam_dbus_b.BUS_OAM_DB00n.tri_bus(oam_data_b0);
  reg.oam_dbus_b.BUS_OAM_DB01n.tri_bus(oam_data_b1);
  reg.oam_dbus_b.BUS_OAM_DB02n.tri_bus(oam_data_b2);
  reg.oam_dbus_b.BUS_OAM_DB03n.tri_bus(oam_data_b3);
  reg.oam_dbus_b.BUS_OAM_DB04n.tri_bus(oam_data_b4);
  reg.oam_dbus_b.BUS_OAM_DB05n.tri_bus(oam_data_b5);
  reg.oam_dbus_b.BUS_OAM_DB06n.tri_bus(oam_data_b6);
  reg.oam_dbus_b.BUS_OAM_DB07n.tri_bus(oam_data_b7);

  /*#p29.YDYV*/ reg.oam_latch_a.YDYV_OAM_LATCH_DA0n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA00n.out_new());
  /*_p29.YCEB*/ reg.oam_latch_a.YCEB_OAM_LATCH_DA1n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA01n.out_new());
  /*_p29.ZUCA*/ reg.oam_latch_a.ZUCA_OAM_LATCH_DA2n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA02n.out_new());
  /*_p29.WONE*/ reg.oam_latch_a.WONE_OAM_LATCH_DA3n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA03n.out_new());
  /*_p29.ZAXE*/ reg.oam_latch_a.ZAXE_OAM_LATCH_DA4n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA04n.out_new());
  /*_p29.XAFU*/ reg.oam_latch_a.XAFU_OAM_LATCH_DA5n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA05n.out_new());
  /*_p29.YSES*/ reg.oam_latch_a.YSES_OAM_LATCH_DA6n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA06n.out_new());
  /*_p29.ZECA*/ reg.oam_latch_a.ZECA_OAM_LATCH_DA7n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_a.BUS_OAM_DA07n.out_new());

  /*#p31.XYKY*/ reg.oam_latch_b.XYKY_OAM_LATCH_DB0n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB00n.out_new());
  /*_p31.YRUM*/ reg.oam_latch_b.YRUM_OAM_LATCH_DB1n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB01n.out_new());
  /*_p31.YSEX*/ reg.oam_latch_b.YSEX_OAM_LATCH_DB2n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB02n.out_new());
  /*_p31.YVEL*/ reg.oam_latch_b.YVEL_OAM_LATCH_DB3n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB03n.out_new());
  /*_p31.WYNO*/ reg.oam_latch_b.WYNO_OAM_LATCH_DB4n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB04n.out_new());
  /*_p31.CYRA*/ reg.oam_latch_b.CYRA_OAM_LATCH_DB5n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB05n.out_new());
  /*_p31.ZUVE*/ reg.oam_latch_b.ZUVE_OAM_LATCH_DB6n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB06n.out_new());
  /*_p31.ECED*/ reg.oam_latch_b.ECED_OAM_LATCH_DB7n.tp_latchn(BODE_OAM_OEp, reg.oam_dbus_b.BUS_OAM_DB07n.out_new());

  /*_p28.GUKO*/ wire GUKO_OBL_TO_CBDp = and3(LEKO_CPU_RDp(), AMAB_CPU_OAM_ENp, WAFO_OAM_A0n);
  /*_p28.WUME*/ wire WUME_OBL_TO_CBDn = not1(GUKO_OBL_TO_CBDp);
  /*_p29.YFAP*/ triwire YFAP_OLA_TO_CBD0 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.YDYV_OAM_LATCH_DA0n.qn_new());
  /*_p29.XELE*/ triwire XELE_OLA_TO_CBD1 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.YCEB_OAM_LATCH_DA1n.qn_new());
  /*_p29.YPON*/ triwire YPON_OLA_TO_CBD2 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.ZUCA_OAM_LATCH_DA2n.qn_new());
  /*_p29.XUVO*/ triwire XUVO_OLA_TO_CBD3 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.WONE_OAM_LATCH_DA3n.qn_new());
  /*_p29.ZYSA*/ triwire ZYSA_OLA_TO_CBD4 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.ZAXE_OAM_LATCH_DA4n.qn_new());
  /*_p29.YWEG*/ triwire YWEG_OLA_TO_CBD5 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.XAFU_OAM_LATCH_DA5n.qn_new());
  /*_p29.XABU*/ triwire XABU_OLA_TO_CBD6 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.YSES_OAM_LATCH_DA6n.qn_new());
  /*_p29.YTUX*/ triwire YTUX_OLA_TO_CBD7 = tri10_np(WUME_OBL_TO_CBDn, reg.oam_latch_a.ZECA_OAM_LATCH_DA7n.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(YFAP_OLA_TO_CBD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(XELE_OLA_TO_CBD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(YPON_OLA_TO_CBD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(XUVO_OLA_TO_CBD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(ZYSA_OLA_TO_CBD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(YWEG_OLA_TO_CBD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(XABU_OLA_TO_CBD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(YTUX_OLA_TO_CBD7);

  /*_p28.WUKU*/ wire WUKU_OBL_TO_CBDp = and3(LEKO_CPU_RDp(), AMAB_CPU_OAM_ENp, GEKA_OAM_A0p);
  /*_p28.WEWU*/ wire WEWU_OBL_TO_CBDn = not1(WUKU_OBL_TO_CBDp);
  /*_p31.XACA*/ triwire XACA_OLB_TO_CBD0 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.XYKY_OAM_LATCH_DB0n.qn_new());
  /*_p31.XAGU*/ triwire XAGU_OLB_TO_CBD1 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.YRUM_OAM_LATCH_DB1n.qn_new());
  /*_p31.XEPU*/ triwire XEPU_OLB_TO_CBD2 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.YSEX_OAM_LATCH_DB2n.qn_new());
  /*_p31.XYGU*/ triwire XYGU_OLB_TO_CBD3 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.YVEL_OAM_LATCH_DB3n.qn_new());
  /*_p31.XUNA*/ triwire XUNA_OLB_TO_CBD4 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.WYNO_OAM_LATCH_DB4n.qn_new());
  /*_p31.DEVE*/ triwire DEVE_OLB_TO_CBD5 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.CYRA_OAM_LATCH_DB5n.qn_new());
  /*_p31.ZEHA*/ triwire ZEHA_OLB_TO_CBD6 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.ZUVE_OAM_LATCH_DB6n.qn_new());
  /*_p31.FYRA*/ triwire FYRA_OLB_TO_CBD7 = tri10_np(WEWU_OBL_TO_CBDn, reg.oam_latch_b.ECED_OAM_LATCH_DB7n.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(XACA_OLB_TO_CBD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(XAGU_OLB_TO_CBD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(XEPU_OLB_TO_CBD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(XYGU_OLB_TO_CBD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(XUNA_OLB_TO_CBD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(DEVE_OLB_TO_CBD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(ZEHA_OLB_TO_CBD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(FYRA_OLB_TO_CBD7);
}

//-----------------------------------------------------------------------------
