#include "GateBoyLib/GateBoyOamBus.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoyExtBus.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoySpriteScanner.h"
#include "GateBoyLib/GateBoyVramBus.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::latch_bus(
  GateBoyCpuBus& cpu_bus,
  wire ACYL_SCANNINGp,
  wire XOCE_xBCxxFGx,
  wire XUJA_SPR_OAM_LATCHn)
{
  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(cpu_bus.BOFE_LATCH_EXTn(), cpu_bus.SARO_ADDR_OAMp(), cpu_bus.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);

  /* p29.YDYV*/ oam_latch_a.YDYV_OAM_LATCH_DA0n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA00n.qp_new());
  /* p29.YCEB*/ oam_latch_a.YCEB_OAM_LATCH_DA1n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA01n.qp_new());
  /* p29.ZUCA*/ oam_latch_a.ZUCA_OAM_LATCH_DA2n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA02n.qp_new());
  /* p29.WONE*/ oam_latch_a.WONE_OAM_LATCH_DA3n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA03n.qp_new());
  /* p29.ZAXE*/ oam_latch_a.ZAXE_OAM_LATCH_DA4n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA04n.qp_new());
  /* p29.XAFU*/ oam_latch_a.XAFU_OAM_LATCH_DA5n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA05n.qp_new());
  /* p29.YSES*/ oam_latch_a.YSES_OAM_LATCH_DA6n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA06n.qp_new());
  /* p29.ZECA*/ oam_latch_a.ZECA_OAM_LATCH_DA7n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DA07n.qp_new());

  /*#p31.XYKY*/ oam_latch_b.XYKY_OAM_LATCH_DB0n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB00n.qp_new());
  /* p31.YRUM*/ oam_latch_b.YRUM_OAM_LATCH_DB1n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB01n.qp_new());
  /* p31.YSEX*/ oam_latch_b.YSEX_OAM_LATCH_DB2n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB02n.qp_new());
  /* p31.YVEL*/ oam_latch_b.YVEL_OAM_LATCH_DB3n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB03n.qp_new());
  /* p31.WYNO*/ oam_latch_b.WYNO_OAM_LATCH_DB4n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB04n.qp_new());
  /* p31.CYRA*/ oam_latch_b.CYRA_OAM_LATCH_DB5n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB05n.qp_new());
  /* p31.ZUVE*/ oam_latch_b.ZUVE_OAM_LATCH_DB6n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB06n.qp_new());
  /* p31.ECED*/ oam_latch_b.ECED_OAM_LATCH_DB7n.tp_latchn(_BODE_OAM_OEp, BUS_OAM_DB07n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::latch_to_cpu(
  GateBoyCpuBus& cpu_bus,
  DFF17 MATU_DMA_RUNNINGp,
  wire ACYL_SCANNINGp,
  NorLatch XYMU_RENDERINGn)
{
  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_A00n.qp_new());
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp.qp_new());
  /* p28.AJON*/ wire _AJON_RENDERINGp = and2(_BOGE_DMA_RUNNINGn, XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /*#p28.AJUJ*/ wire _AJUJ_OAM_BUSYn   = nor3(MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, _AJON_RENDERINGp);
  /*#p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(cpu_bus.SARO_ADDR_OAMp(), _AJUJ_OAM_BUSYn); // def and

  /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(cpu_bus.LEKO_CPU_RDp(), _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
  /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
  /* p29.YFAP_OLA_TO_CBD0*/ cpu_bus.BUS_CPU_D00p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.YDYV_OAM_LATCH_DA0n.qn_new());
  /* p29.XELE_OLA_TO_CBD1*/ cpu_bus.BUS_CPU_D01p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.YCEB_OAM_LATCH_DA1n.qn_new());
  /* p29.YPON_OLA_TO_CBD2*/ cpu_bus.BUS_CPU_D02p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.ZUCA_OAM_LATCH_DA2n.qn_new());
  /* p29.XUVO_OLA_TO_CBD3*/ cpu_bus.BUS_CPU_D03p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.WONE_OAM_LATCH_DA3n.qn_new());
  /* p29.ZYSA_OLA_TO_CBD4*/ cpu_bus.BUS_CPU_D04p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.ZAXE_OAM_LATCH_DA4n.qn_new());
  /* p29.YWEG_OLA_TO_CBD5*/ cpu_bus.BUS_CPU_D05p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.XAFU_OAM_LATCH_DA5n.qn_new());
  /* p29.XABU_OLA_TO_CBD6*/ cpu_bus.BUS_CPU_D06p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.YSES_OAM_LATCH_DA6n.qn_new());
  /* p29.YTUX_OLA_TO_CBD7*/ cpu_bus.BUS_CPU_D07p.tri10_np(_WUME_OBL_TO_CBDn, oam_latch_a.ZECA_OAM_LATCH_DA7n.qn_new());

  /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(cpu_bus.LEKO_CPU_RDp(), _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);
  /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
  /* p31.XACA_OLB_TO_CBD0*/ cpu_bus.BUS_CPU_D00p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.XYKY_OAM_LATCH_DB0n.qn_new());
  /* p31.XAGU_OLB_TO_CBD1*/ cpu_bus.BUS_CPU_D01p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.YRUM_OAM_LATCH_DB1n.qn_new());
  /* p31.XEPU_OLB_TO_CBD2*/ cpu_bus.BUS_CPU_D02p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.YSEX_OAM_LATCH_DB2n.qn_new());
  /* p31.XYGU_OLB_TO_CBD3*/ cpu_bus.BUS_CPU_D03p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.YVEL_OAM_LATCH_DB3n.qn_new());
  /* p31.XUNA_OLB_TO_CBD4*/ cpu_bus.BUS_CPU_D04p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.WYNO_OAM_LATCH_DB4n.qn_new());
  /* p31.DEVE_OLB_TO_CBD5*/ cpu_bus.BUS_CPU_D05p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.CYRA_OAM_LATCH_DB5n.qn_new());
  /* p31.ZEHA_OLB_TO_CBD6*/ cpu_bus.BUS_CPU_D06p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.ZUVE_OAM_LATCH_DB6n.qn_new());
  /* p31.FYRA_OLB_TO_CBD7*/ cpu_bus.BUS_CPU_D07p.tri10_np(_WEWU_OBL_TO_CBDn, oam_latch_b.ECED_OAM_LATCH_DB7n.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::latch_to_temp_a(
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  wire ACYL_SCANNINGp,
  DFF17 MATU_DMA_RUNNINGp,
  wire XUJY_OAM_CLKENp)
{
  /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not_or_and3(cpu_bus.SARO_ADDR_OAMp(), MATU_DMA_RUNNINGp.qp_new(), clk.MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
  /* p25.AVER*/ wire _AVER_AxxxExxx = nand2(ACYL_SCANNINGp, clk.XYSO_xBCDxFGH());
  /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
  /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);

  /* p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_COTA_OAM_CLKn);
  /*#p29.XUSO*/ oam_temp_a.XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YDYV_OAM_LATCH_DA0n.qp_old());
  /* p29.XEGU*/ oam_temp_a.XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YCEB_OAM_LATCH_DA1n.qp_old());
  /* p29.YJEX*/ oam_temp_a.YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZUCA_OAM_LATCH_DA2n.qp_old());
  /* p29.XYJU*/ oam_temp_a.XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.WONE_OAM_LATCH_DA3n.qp_old());
  /* p29.YBOG*/ oam_temp_a.YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZAXE_OAM_LATCH_DA4n.qp_old());
  /* p29.WYSO*/ oam_temp_a.WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.XAFU_OAM_LATCH_DA5n.qp_old());
  /* p29.XOTE*/ oam_temp_a.XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YSES_OAM_LATCH_DA6n.qp_old());
  /* p29.YZAB*/ oam_temp_a.YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZECA_OAM_LATCH_DA7n.qp_old());
}

void GateBoyOamBus::latch_to_temp_b(
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  wire ACYL_SCANNINGp,
  DFF17 MATU_DMA_RUNNINGp,
  wire XUJY_OAM_CLKENp)
{
  /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not_or_and3(cpu_bus.SARO_ADDR_OAMp(), MATU_DMA_RUNNINGp.qp_new(), clk.MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
  /* p25.AVER*/ wire _AVER_AxxxExxx = nand2(ACYL_SCANNINGp, clk.XYSO_xBCDxFGH());
  /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
  /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);

  /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
  /* p31.YLOR*/ oam_temp_b.YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.XYKY_OAM_LATCH_DB0n.qp_old());
  /* p31.ZYTY*/ oam_temp_b.ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YRUM_OAM_LATCH_DB1n.qp_old());
  /* p31.ZYVE*/ oam_temp_b.ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YSEX_OAM_LATCH_DB2n.qp_old());
  /* p31.ZEZY*/ oam_temp_b.ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YVEL_OAM_LATCH_DB3n.qp_old());
  /* p31.GOMO*/ oam_temp_b.GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.WYNO_OAM_LATCH_DB4n.qp_old());
  /* p31.BAXO*/ oam_temp_b.BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.CYRA_OAM_LATCH_DB5n.qp_old());
  /* p31.YZOS*/ oam_temp_b.YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ZUVE_OAM_LATCH_DB6n.qp_old());
  /* p31.DEPO*/ oam_temp_b.DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ECED_OAM_LATCH_DB7n.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::ext_to_data_bus(const GateBoyDMA& dma, const GateBoyExtBus& ext_bus)
{
  // DMA write OAM from ram/cart
  /* p25.RALO*/ wire _RALO_EXT_D0p = not1(ext_bus.PIN_17_D00.qp_new());
  /* p25.TUNE*/ wire _TUNE_EXT_D1p = not1(ext_bus.PIN_18_D01.qp_new());
  /* p25.SERA*/ wire _SERA_EXT_D2p = not1(ext_bus.PIN_19_D02.qp_new());
  /* p25.TENU*/ wire _TENU_EXT_D3p = not1(ext_bus.PIN_20_D03.qp_new());
  /* p25.SYSA*/ wire _SYSA_EXT_D4p = not1(ext_bus.PIN_21_D04.qp_new());
  /* p25.SUGY*/ wire _SUGY_EXT_D5p = not1(ext_bus.PIN_22_D05.qp_new());
  /* p25.TUBE*/ wire _TUBE_EXT_D6p = not1(ext_bus.PIN_23_D06.qp_new());
  /* p25.SYZO*/ wire _SYZO_EXT_D7p = not1(ext_bus.PIN_24_D07.qp_new());

  /* p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(dma.LUMA_DMA_CARTp());

  /* p25.WASA_ED0_TO_ODA0*/ BUS_OAM_DA00n.tri6_nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
  /* p25.BOMO_ED1_TO_ODA1*/ BUS_OAM_DA01n.tri6_nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
  /* p25.BASA_ED2_TO_ODA2*/ BUS_OAM_DA02n.tri6_nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
  /* p25.CAKO_ED3_TO_ODA3*/ BUS_OAM_DA03n.tri6_nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
  /* p25.BUMA_ED4_TO_ODA4*/ BUS_OAM_DA04n.tri6_nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
  /* p25.BUPY_ED5_TO_ODA5*/ BUS_OAM_DA05n.tri6_nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
  /* p25.BASY_ED6_TO_ODA6*/ BUS_OAM_DA06n.tri6_nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
  /* p25.BAPE_ED7_TO_ODA7*/ BUS_OAM_DA07n.tri6_nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

  /* p25.WEJO_ED0_TO_ODB0*/ BUS_OAM_DB00n.tri6_nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
  /* p25.BUBO_ED1_TO_ODB1*/ BUS_OAM_DB01n.tri6_nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
  /* p25.BETU_ED2_TO_ODB2*/ BUS_OAM_DB02n.tri6_nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
  /* p25.CYME_ED3_TO_ODB3*/ BUS_OAM_DB03n.tri6_nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
  /* p25.BAXU_ED4_TO_ODB4*/ BUS_OAM_DB04n.tri6_nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
  /* p25.BUHU_ED5_TO_ODB5*/ BUS_OAM_DB05n.tri6_nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
  /* p25.BYNY_ED6_TO_ODB6*/ BUS_OAM_DB06n.tri6_nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
  /* p25.BYPY_ED7_TO_ODB7*/ BUS_OAM_DB07n.tri6_nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::vram_to_data_bus(const GateBoyDMA& dma, const GateBoyVramBus& vram_bus)
{
  /* p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(dma.LUFA_DMA_VRAMp());

  /* p28.WUZU_VD0_TO_ODA0*/ BUS_OAM_DA00n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D00p.qp_new());
  /* p28.AXER_VD1_TO_ODA1*/ BUS_OAM_DA01n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D01p.qp_new());
  /* p28.ASOX_VD2_TO_ODA2*/ BUS_OAM_DA02n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D02p.qp_new());
  /* p28.CETU_VD3_TO_ODA3*/ BUS_OAM_DA03n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D03p.qp_new());
  /* p28.ARYN_VD4_TO_ODA4*/ BUS_OAM_DA04n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D04p.qp_new());
  /* p28.ACOT_VD5_TO_ODA5*/ BUS_OAM_DA05n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D05p.qp_new());
  /* p28.CUJE_VD6_TO_ODA6*/ BUS_OAM_DA06n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D06p.qp_new());
  /* p28.ATER_VD7_TO_ODA7*/ BUS_OAM_DA07n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D07p.qp_new());

  /* p28.WOWA_VD0_TO_ODB0*/ BUS_OAM_DB00n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D00p.qp_new());
  /* p28.AVEB_VD1_TO_ODB1*/ BUS_OAM_DB01n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D01p.qp_new());
  /* p28.AMUH_VD2_TO_ODB2*/ BUS_OAM_DB02n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D02p.qp_new());
  /* p28.COFO_VD3_TO_ODB3*/ BUS_OAM_DB03n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D03p.qp_new());
  /* p28.AZOZ_VD4_TO_ODB4*/ BUS_OAM_DB04n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D04p.qp_new());
  /* p28.AGYK_VD5_TO_ODB5*/ BUS_OAM_DB05n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D05p.qp_new());
  /* p28.BUSE_VD6_TO_ODB6*/ BUS_OAM_DB06n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D06p.qp_new());
  /* p28.ANUM_VD7_TO_ODB7*/ BUS_OAM_DB07n.tri6_nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D07p.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::dma_to_addr_bus(GateBoyDMA& dma) {
  // DMA OAM write address driver
  /* p28.FODO_DMA0_TO_OA0*/ BUS_OAM_A00n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.NAKY_DMA_A00p.qp_new());
  /* p28.FESA_DMA1_TO_OA1*/ BUS_OAM_A01n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.PYRO_DMA_A01p.qp_new());
  /* p28.FAGO_DMA2_TO_OA2*/ BUS_OAM_A02n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.NEFY_DMA_A02p.qp_new());
  /* p28.FYKY_DMA3_TO_OA3*/ BUS_OAM_A03n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.MUTY_DMA_A03p.qp_new());
  /* p28.ELUG_DMA4_TO_OA4*/ BUS_OAM_A04n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.NYKO_DMA_A04p.qp_new());
  /* p28.EDOL_DMA5_TO_OA5*/ BUS_OAM_A05n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.PYLO_DMA_A05p.qp_new());
  /* p28.FYDU_DMA6_TO_OA6*/ BUS_OAM_A06n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.NUTO_DMA_A06p.qp_new());
  /* p28.FETU_DMA7_TO_OA7*/ BUS_OAM_A07n.tri6_nn(dma.DUGA_DMA_RUNNINGn(), dma.MUGU_DMA_A07p.qp_new());
};

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::sprite_index_to_addr_bus(const GateBoyDMA& dma, const SpriteStore& sprite_store, NorLatch XYMU_RENDERINGn){
  SigIn SIG_VCC = 1;
  // OAM address from sprite fetcher
  /* p28.AJON*/ wire _AJON_RENDERINGp = and2(dma.BOGE_DMA_RUNNINGn(), XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /* p28.BETE*/ wire _BETE_SFETCHINGn = not1(_AJON_RENDERINGp);
  /* p28.GECA_FETCHX_TO_OA0*/ BUS_OAM_A00n.tri6_nn(_BETE_SFETCHINGn, SIG_VCC);
  /* p28.WYDU_FETCHX_TO_OA1*/ BUS_OAM_A01n.tri6_nn(_BETE_SFETCHINGn, SIG_VCC);
  /* p28.GYBU_FETCH0_TO_OA2*/ BUS_OAM_A02n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I0.qp_new());
  /* p28.GYKA_FETCH1_TO_OA3*/ BUS_OAM_A03n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I1.qp_new());
  /* p28.FABY_FETCH2_TO_OA4*/ BUS_OAM_A04n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I2.qp_new());
  /* p28.FACO_FETCH3_TO_OA5*/ BUS_OAM_A05n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I3.qp_new());
  /* p28.FUGU_FETCH4_TO_OA6*/ BUS_OAM_A06n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I4.qp_new());
  /* p28.FYKE_FETCH5_TO_OA7*/ BUS_OAM_A07n.tri6_nn(_BETE_SFETCHINGn, sprite_store.BUS_SPR_I5.qp_new());
};

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::scan_index_to_addr_bus(SpriteScanner& sprite_scanner, wire ACYL_SCANNINGp) {
  // OAM address from sprite scanner
  SigIn SIG_GND = 0;
  /* p28.APAR*/ wire _APAR_SCANNINGn = not1(ACYL_SCANNINGp);
  /* p28.GEFY_SCANX_TO_OA0*/ BUS_OAM_A00n.tri6_nn(_APAR_SCANNINGn, SIG_GND);
  /* p28.WUWE_SCANX_TO_OA1*/ BUS_OAM_A01n.tri6_nn(_APAR_SCANNINGn, SIG_GND);
  /* p28.GUSE_SCAN0_TO_OA2*/ BUS_OAM_A02n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.YFEL_SCAN0.qp_new());
  /* p28.GEMA_SCAN1_TO_OA3*/ BUS_OAM_A03n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.WEWY_SCAN1.qp_new());
  /* p28.FUTO_SCAN2_TO_OA4*/ BUS_OAM_A04n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.GOSO_SCAN2.qp_new());
  /* p28.FAKU_SCAN3_TO_OA5*/ BUS_OAM_A05n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.ELYN_SCAN3.qp_new());
  /* p28.GAMA_SCAN4_TO_OA6*/ BUS_OAM_A06n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.FAHA_SCAN4.qp_new());
  /* p28.GOBY_SCAN5_TO_OA7*/ BUS_OAM_A07n.tri6_nn(_APAR_SCANNINGn, sprite_scanner.FONY_SCAN5.qp_new());
};

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::cpu_to_addr_bus(
  GateBoyCpuBus& cpu_bus,
  NorLatch XYMU_RENDERINGn,
  DFF17 MATU_DMA_RUNNINGp,
  wire ACYL_SCANNINGp)
{
  // OAM address from CPU
  /* p28.ASAM*/ wire _ASAM_CPU_OAM_RDn = or3(ACYL_SCANNINGp, XYMU_RENDERINGn.qn_new(), MATU_DMA_RUNNINGp.qp_new());
  /* p28.GARO_CA0_TO_OA0*/ BUS_OAM_A00n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A00p.qp_new());
  /* p28.WACU_CA1_TO_OA1*/ BUS_OAM_A01n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A01p.qp_new());
  /* p28.GOSE_CA2_TO_OA2*/ BUS_OAM_A02n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A02p.qp_new());
  /* p28.WAPE_CA3_TO_OA3*/ BUS_OAM_A03n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A03p.qp_new());
  /* p28.FEVU_CA4_TO_OA4*/ BUS_OAM_A04n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A04p.qp_new());
  /* p28.GERA_CA5_TO_OA5*/ BUS_OAM_A05n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A05p.qp_new());
  /* p28.WAXA_CA6_TO_OA6*/ BUS_OAM_A06n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A06p.qp_new());
  /* p28.FOBY_CA7_TO_OA7*/ BUS_OAM_A07n.tri6_nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A07p.qp_new());
}


void GateBoyOamBus::cpu_to_data_bus(
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  NorLatch XYMU_RENDERINGn,
  DFF17 MATU_DMA_RUNNINGp,
  wire ACYL_SCANNINGp)
{
  // CPU write OAM
  wire SARO = cpu_bus.SARO_ADDR_OAMp();

  /* p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(SARO, cpu_bus.CUPA_CPU_WRp());
  /* p28.WUJE*/ WUJE_CPU_OAM_WRn.nor_latch(clk.XYNY_ABCDxxxx(), _XUTO_CPU_OAM_WRp); // slightly weird

  /* p28.XUPA*/ wire _XUPA_CPU_OAM_WRp  = not1(WUJE_CPU_OAM_WRn.qp_new());
  /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp.qp_new());
  /* p28.AJON*/ wire _AJON_RENDERINGp   = and2(_BOGE_DMA_RUNNINGn, XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /*#p28.AJUJ*/ wire _AJUJ_OAM_BUSYn    = nor3(MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, _AJON_RENDERINGp);
  /*#p28.AMAB*/ wire _AMAB_CPU_OAM_ENp  = and2(cpu_bus.SARO_ADDR_OAMp(), _AJUJ_OAM_BUSYn); // def and
  /*#p28.APAG*/ wire _APAG_CBD_TO_OBDp  = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, cpu_bus.ADAH_FE00_FEFFn());
  /*#p28.AZUL*/ wire _AZUL_CBD_TO_OBDn  = not1(_APAG_CBD_TO_OBDp);

  /* p28.ZAXA_CD0_TO_ODA0*/ BUS_OAM_DA00n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D00p.qp_new());
  /* p28.ZAKY_CD1_TO_ODA1*/ BUS_OAM_DA01n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D01p.qp_new());
  /* p28.WULE_CD2_TO_ODA2*/ BUS_OAM_DA02n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D02p.qp_new());
  /* p28.ZOZO_CD3_TO_ODA3*/ BUS_OAM_DA03n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D03p.qp_new());
  /* p28.ZUFO_CD4_TO_ODA4*/ BUS_OAM_DA04n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D04p.qp_new());
  /* p28.ZATO_CD5_TO_ODA5*/ BUS_OAM_DA05n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D05p.qp_new());
  /* p28.YVUC_CD6_TO_ODA6*/ BUS_OAM_DA06n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D06p.qp_new());
  /* p28.ZUFE_CD7_TO_ODA7*/ BUS_OAM_DA07n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D07p.qp_new());

  /* p28.ZAMY_CD0_TO_ODB0*/ BUS_OAM_DB00n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D00p.qp_new());
  /* p28.ZOPU_CD1_TO_ODB1*/ BUS_OAM_DB01n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D01p.qp_new());
  /* p28.WYKY_CD2_TO_ODB2*/ BUS_OAM_DB02n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D02p.qp_new());
  /* p28.ZAJA_CD3_TO_ODB3*/ BUS_OAM_DB03n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D03p.qp_new());
  /* p28.ZUGA_CD4_TO_ODB4*/ BUS_OAM_DB04n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D04p.qp_new());
  /* p28.ZUMO_CD5_TO_ODB5*/ BUS_OAM_DB05n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D05p.qp_new());
  /* p28.XYTO_CD6_TO_ODB6*/ BUS_OAM_DB06n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D06p.qp_new());
  /* p28.ZYFA_CD7_TO_ODB7*/ BUS_OAM_DB07n.tri6_nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D07p.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::set_pin_clk(
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  DFF17 MATU_DMA_RUNNINGp,
  wire ACYL_SCANNINGp,
  wire XUJY_OAM_CLKENp)
{
  /* p25.AVER*/ wire _AVER_AxxxExxx = nand2(ACYL_SCANNINGp, clk.XYSO_xBCDxFGH());
  /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not_or_and3(cpu_bus.SARO_ADDR_OAMp(), MATU_DMA_RUNNINGp.qp_new(), clk.MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
  /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
  /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);

  /*SIG_OAM_CLKn*/ SIG_OAM_CLKn.sig_out(_COTA_OAM_CLKn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::set_pin_wr(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  NorLatch XYMU_RENDERINGn,
  DFF17 MATU_DMA_RUNNINGp,
  wire ACYL_SCANNINGp)
{
  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_A00n.qp_new());
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp.qp_new());
  /* p28.AJON*/ wire _AJON_RENDERINGp = and2(_BOGE_DMA_RUNNINGn, XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
  /*#p28.AJUJ*/ wire _AJUJ_OAM_BUSYn   = nor3(MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, _AJON_RENDERINGp);
  /*#p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(cpu_bus.SARO_ADDR_OAMp(), _AJUJ_OAM_BUSYn); // def and

  /* p04.MAKA*/ MAKA_LATCH_EXTp.dff17(clk.ZEME_AxCxExGx(), rst.AVOR_SYS_RSTp(), cpu_bus.CATY_LATCH_EXTp());

  /* p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(clk.UVYT_ABCDxxxx(), MAKA_LATCH_EXTp.qp_new()); // def nor2
  /* p04.POWU*/ wire _POWU_DMA_OAM_WRp = and2(MATU_DMA_RUNNINGp.qp_new(), _NAXY_DMA_OAM_WRp); // def and
  /* p04.WYJA*/ wire _WYJA_OAM_WRp     = and_or3(_AMAB_CPU_OAM_ENp, cpu_bus.CUPA_CPU_WRp(), _POWU_DMA_OAM_WRp);
  /* p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
  /* p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
  /* p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
  /* p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);

  /*SIG_OAM_WRn_A*/ SIG_OAM_WRn_A.sig_out(_ZOFE_OAM_A_WRn);
  /*SIG_OAM_WRn_B*/ SIG_OAM_WRn_B.sig_out(_ZONE_OAM_B_WRn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::set_pin_oe(
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  wire ACYL_SCANNINGp,
  wire XUJA_SPR_OAM_LATCHn)
{
  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, clk.XOCE_xBCxxFGx()); // schematic wrong, is def nand2
  /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(cpu_bus.BOFE_LATCH_EXTn(), cpu_bus.SARO_ADDR_OAMp(), cpu_bus.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
  /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
  /*#p28.YRYV*/ wire _YRYV_OAM_OEp = not1(_YVAL_OAM_OEn);
  /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYV_OAM_OEp);

  /*SIG_OAM_OEn*/ SIG_OAM_OEn.sig_out(_ZODO_OAM_OEn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyOamBus::tock(GateBoyCpuBus& cpu_bus, wire XOCE_xBCxxFGx, wire ACYL_SCANNINGp, wire XUJA_SPR_OAM_LATCHn, uint8_t* oam_ram) {
  uint8_t oam_addr   = (uint8_t)BitBase::pack_newn(7, &BUS_OAM_A01n);
  uint8_t oam_data_a = (uint8_t)BitBase::pack_newn(8, &BUS_OAM_DA00n);
  uint8_t oam_data_b = (uint8_t)BitBase::pack_newn(8, &BUS_OAM_DB00n);

  if (bit(~old_oam_clk) && bit(~SIG_OAM_CLKn.qp_new())) {
    if (bit(~SIG_OAM_WRn_A.qp_new())) oam_ram[(oam_addr << 1) + 0] = oam_data_a;
    if (bit(~SIG_OAM_WRn_B.qp_new())) oam_ram[(oam_addr << 1) + 1] = oam_data_b;
  }

  if (bit(~SIG_OAM_OEn.qp_new())) {
    oam_data_a = oam_ram[(oam_addr << 1) + 0];
    oam_data_b = oam_ram[(oam_addr << 1) + 1];
  }

  old_oam_clk = bit(~SIG_OAM_CLKn.qp_new());

  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(cpu_bus.BOFE_LATCH_EXTn(), cpu_bus.SARO_ADDR_OAMp(), cpu_bus.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
  /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
  /*#p28.YRYV*/ wire _YRYV_OAM_OEp = not1(_YVAL_OAM_OEn);
  /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYV_OAM_OEp);

  BUS_OAM_DA00n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 0) & 1);
  BUS_OAM_DA01n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 1) & 1);
  BUS_OAM_DA02n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 2) & 1);
  BUS_OAM_DA03n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 3) & 1);
  BUS_OAM_DA04n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 4) & 1);
  BUS_OAM_DA05n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 5) & 1);
  BUS_OAM_DA06n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 6) & 1);
  BUS_OAM_DA07n.tri6_nn(_ZODO_OAM_OEn, (oam_data_a >> 7) & 1);

  BUS_OAM_DB00n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 0) & 1);
  BUS_OAM_DB01n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 1) & 1);
  BUS_OAM_DB02n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 2) & 1);
  BUS_OAM_DB03n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 3) & 1);
  BUS_OAM_DB04n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 4) & 1);
  BUS_OAM_DB05n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 5) & 1);
  BUS_OAM_DB06n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 6) & 1);
  BUS_OAM_DB07n.tri6_nn(_ZODO_OAM_OEn, (oam_data_b >> 7) & 1);
}

//------------------------------------------------------------------------------------------------------------------------
