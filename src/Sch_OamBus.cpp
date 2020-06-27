#include "Sch_OamBus.h"

#include "TestGB.h"

using namespace Schematics;

// Schematic wrong, AVER is def nand
// AVER01 << ACYL03
// AVER02 << XYSO02
// AVER03 >> BYCU03

// VAPE is def and
// VAPE01 << TACU03
// VAPE02 << TUVO04
// VAPE03 nc
// VAPE04 >> XUJY01


// Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

// 4-rung whereas the or(and()) were 5 rung?
// Arm on left (gnd) side
// CUFE01 << SARO03
// CUFE02 << MATU17
// CUFE03 >> BYCU01
// CUFE04 << MOPA03

// Schematic very wrong
// TUVO01 << TEPA02
// TUVO02 << TULY17
// TUVO03 << TESE17
// TUVO04 >> VAPE02

// WEFY01
// WEFY02
// WEFY03
// WEFY04

// AJEP def nand
// XUJA def not
// BOTA def nand
// ASYT def and
// BODE def not
// YVAL def not
// YRYV def not
// ZODO def not

//------------------------------------------------------------------------------

struct OamBusSignals {
};

//------------------------------------------------------------------------------

void OamBus::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;
  auto& vram_bus = gb.vram_bus;

  auto bus_sig = gb.bus_mux.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto ext_sig = gb.ext_bus.sig(gb);


  PIN_A0.set(bus_sig.GEKA_OAM_A0p);
  PIN_A1.set(bus_sig.ZYFO_OAM_A1p);
  PIN_A2.set(bus_sig.YFOT_OAM_A2p);
  PIN_A3.set(bus_sig.YFOC_OAM_A3p);
  PIN_A4.set(bus_sig.YVOM_OAM_A4p);
  PIN_A5.set(bus_sig.YMEV_OAM_A5p);
  PIN_A6.set(bus_sig.XEMU_OAM_A6p);
  PIN_A7.set(bus_sig.YZET_OAM_A7p);

  {
    /*p28.YVAL*/ wire _YVAL_OAM_LATCHo = not(bus_sig.BODE_OAM_LATCH);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHb = not(_YVAL_OAM_LATCHo);
    /*p28.ZODO*/ wire _ZODO_OAM_OE = not(_YRYU_OAM_LATCHb);
    PIN_OE.set(_ZODO_OAM_OE);
  }

  {
    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(bus_sig.GEKA_OAM_A0p); // def not
    /*p28.YNYC*/ wire _YNYC_OAM_B_WRn = and (bus_sig.WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire _YLYC_OAM_A_WRn = and (bus_sig.WYJA_OAM_WRp, bus_sig.GEKA_OAM_A0p); // def and
    /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRn);

    PIN_WR_A.set(_ZONE);
    PIN_WR_B.set(_ZOFE);
  }

  PIN_CLK.set(bus_sig.COTA_OAM_CLK);

  {
    /*p28.ZAXA*/ PIN_DA0.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D0);
    /*p28.ZAKY*/ PIN_DA1.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D1);
    /*p28.WULE*/ PIN_DA2.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D2);
    /*p28.ZOZO*/ PIN_DA3.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D3);
    /*p28.ZUFO*/ PIN_DA4.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D4);
    /*p28.ZATO*/ PIN_DA5.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D5);
    /*p28.YVUC*/ PIN_DA6.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D6);
    /*p28.ZUFE*/ PIN_DA7.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D7);

    /*p28.ZAMY*/ PIN_DB0.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D0); // small tri
    /*p28.ZOPU*/ PIN_DB1.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D1);
    /*p28.WYKY*/ PIN_DB2.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D2);
    /*p28.ZAJA*/ PIN_DB3.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D3);
    /*p28.ZUGA*/ PIN_DB4.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D4);
    /*p28.ZUMO*/ PIN_DB5.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D5);
    /*p28.XYTO*/ PIN_DB6.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D6);
    /*p28.ZYFA*/ PIN_DB7.set_tribuf(bus_sig.AZUL_D_TO_OAMD, cpu_bus.TS_D7);
  }

  {

    /*p25.WEJO*/ PIN_DA0.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.RALO*/ not(ext_sig.PIN_D0_C));
    /*p25.BUBO*/ PIN_DA1.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TUNE*/ not(ext_sig.PIN_D1_C));
    /*p25.BETU*/ PIN_DA2.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SERA*/ not(ext_sig.PIN_D2_C));
    /*p25.CYME*/ PIN_DA3.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TENU*/ not(ext_sig.PIN_D3_C));
    /*p25.BAXU*/ PIN_DA4.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SYSA*/ not(ext_sig.PIN_D4_C));
    /*p25.BUHU*/ PIN_DA5.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SUGY*/ not(ext_sig.PIN_D5_C));
    /*p25.BYNY*/ PIN_DA6.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TUBE*/ not(ext_sig.PIN_D6_C));
    /*p25.BYPY*/ PIN_DA7.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SYZO*/ not(ext_sig.PIN_D7_C));
    /*p25.WASA*/ PIN_DB0.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.RALO*/ not(ext_sig.PIN_D0_C));
    /*p25.BOMO*/ PIN_DB1.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TUNE*/ not(ext_sig.PIN_D1_C));
    /*p25.BASA*/ PIN_DB2.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SERA*/ not(ext_sig.PIN_D2_C));
    /*p25.CAKO*/ PIN_DB3.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TENU*/ not(ext_sig.PIN_D3_C));
    /*p25.BUMA*/ PIN_DB4.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SYSA*/ not(ext_sig.PIN_D4_C));
    /*p25.BUPY*/ PIN_DB5.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SUGY*/ not(ext_sig.PIN_D5_C));
    /*p25.BASY*/ PIN_DB6.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.TUBE*/ not(ext_sig.PIN_D6_C));
    /*p25.BAPE*/ PIN_DB7.set_tribuf(dma_sig.CEDE_DMA_READ_CARTn, /*p25.SYZO*/ not(ext_sig.PIN_D7_C));
  }

  {
    /*p28.AZAR*/ wire _AZAR_DMA_READ_VRAMn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ PIN_DA0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD0);
    /*p28.AXER*/ PIN_DA1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD1);
    /*p28.ASOX*/ PIN_DA2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD2);
    /*p28.CETU*/ PIN_DA3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD3);
    /*p28.ARYN*/ PIN_DA4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD4);
    /*p28.ACOT*/ PIN_DA5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD5);
    /*p28.CUJE*/ PIN_DA6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD6);
    /*p28.ATER*/ PIN_DA7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD7);
    /*p28.WOWA*/ PIN_DB0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD0);
    /*p28.AVEB*/ PIN_DB1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD1);
    /*p28.AMUH*/ PIN_DB2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD2);
    /*p28.COFO*/ PIN_DB3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD3);
    /*p28.AZOZ*/ PIN_DB4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD4);
    /*p28.AGYK*/ PIN_DB5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD5);
    /*p28.BUSE*/ PIN_DB6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD6);
    /*p28.ANUM*/ PIN_DB7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD7);
  }

  {
    /*p31.XYKY*/ XYKY_LATCH_OAM_A0.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA0);
    /*p31.YRUM*/ YRUM_LATCH_OAM_A1.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA1);
    /*p31.YSEX*/ YSEX_LATCH_OAM_A2.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA2);
    /*p31.YVEL*/ YVEL_LATCH_OAM_A3.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA3);
    /*p31.WYNO*/ WYNO_LATCH_OAM_A4.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA4);
    /*p31.CYRA*/ CYRA_LATCH_OAM_A5.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA5);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_A6.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA6);
    /*p31.ECED*/ ECED_LATCH_OAM_A7.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DA7);

    /*p29.YDYV*/ YDYV_LATCH_OAM_B0.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB0);
    /*p29.YCEB*/ YCEB_LATCH_OAM_B1.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB1);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_B2.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB2);
    /*p29.WONE*/ WONE_LATCH_OAM_B3.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB3);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_B4.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB4);
    /*p29.XAFU*/ XAFU_LATCH_OAM_B5.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB5);
    /*p29.YSES*/ YSES_LATCH_OAM_B6.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB6);
    /*p29.ZECA*/ ZECA_LATCH_OAM_B7.tp_latch(bus_sig.BODE_OAM_LATCH, PIN_DB7);

    /*p31.XEGA*/ wire XEGA_OAM_CLKn = not(bus_sig.COTA_OAM_CLK);
    /*p31.YLOR*/ YLOR_SPRITE_X0.set(XEGA_OAM_CLKn, XYKY_LATCH_OAM_A0);
    /*p31.ZYTY*/ ZYTY_SPRITE_X1.set(XEGA_OAM_CLKn, YRUM_LATCH_OAM_A1);
    /*p31.ZYVE*/ ZYVE_SPRITE_X2.set(XEGA_OAM_CLKn, YSEX_LATCH_OAM_A2);
    /*p31.ZEZY*/ ZEZY_SPRITE_X3.set(XEGA_OAM_CLKn, YVEL_LATCH_OAM_A3);
    /*p31.GOMO*/ GOMO_SPRITE_X4.set(XEGA_OAM_CLKn, WYNO_LATCH_OAM_A4);
    /*p31.BAXO*/ BAXO_SPRITE_X5.set(XEGA_OAM_CLKn, CYRA_LATCH_OAM_A5);
    /*p31.YZOS*/ YZOS_SPRITE_X6.set(XEGA_OAM_CLKn, ZUVE_LATCH_OAM_A6);
    /*p31.DEPO*/ DEPO_SPRITE_X7.set(XEGA_OAM_CLKn, ECED_LATCH_OAM_A7);

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(bus_sig.COTA_OAM_CLK);
    /*p29.XUSO*/ XUSO_SPRITE_Y0.set(YWOK_OAM_CLKn, YDYV_LATCH_OAM_B0);
    /*p29.XEGU*/ XEGU_SPRITE_Y1.set(YWOK_OAM_CLKn, YCEB_LATCH_OAM_B1);
    /*p29.YJEX*/ YJEX_SPRITE_Y2.set(YWOK_OAM_CLKn, ZUCA_LATCH_OAM_B2);
    /*p29.XYJU*/ XYJU_SPRITE_Y3.set(YWOK_OAM_CLKn, WONE_LATCH_OAM_B3);
    /*p29.YBOG*/ YBOG_SPRITE_Y4.set(YWOK_OAM_CLKn, ZAXE_LATCH_OAM_B4);
    /*p29.WYSO*/ WYSO_SPRITE_Y5.set(YWOK_OAM_CLKn, XAFU_LATCH_OAM_B5);
    /*p29.XOTE*/ XOTE_SPRITE_Y6.set(YWOK_OAM_CLKn, YSES_LATCH_OAM_B6);
    /*p29.YZAB*/ YZAB_SPRITE_Y7.set(YWOK_OAM_CLKn, ZECA_LATCH_OAM_B7);
  }

  {
    /*p28.WAFO*/ wire WAFO_OAM_A0n = not(bus_sig.GEKA_OAM_A0p); // def not
    /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD = and (cpu_sig.LEKO_CPU_RDp, bus_sig.AMAB_OAM_LOCKn, PIN_A0);
    /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD = and (cpu_sig.LEKO_CPU_RDp, bus_sig.AMAB_OAM_LOCKn, WAFO_OAM_A0n);
    /*p28.WEWU*/ wire _WEWU_OAM_A_CPU_RDn = not(_WUKU_OAM_A_CPU_RD);
    /*p28.WUME*/ wire _WUME_OAM_B_CPU_RDn = not(_GUKO_OAM_B_CPU_RD);

    /*p31.XACA*/ cpu_bus.TS_D0.set_tribuf(_WEWU_OAM_A_CPU_RDn, XYKY_LATCH_OAM_A0);
    /*p31.XAGU*/ cpu_bus.TS_D1.set_tribuf(_WEWU_OAM_A_CPU_RDn, YRUM_LATCH_OAM_A1);
    /*p31.XEPU*/ cpu_bus.TS_D2.set_tribuf(_WEWU_OAM_A_CPU_RDn, YSEX_LATCH_OAM_A2);
    /*p31.XYGU*/ cpu_bus.TS_D3.set_tribuf(_WEWU_OAM_A_CPU_RDn, YVEL_LATCH_OAM_A3);
    /*p31.XUNA*/ cpu_bus.TS_D4.set_tribuf(_WEWU_OAM_A_CPU_RDn, WYNO_LATCH_OAM_A4);
    /*p31.DEVE*/ cpu_bus.TS_D5.set_tribuf(_WEWU_OAM_A_CPU_RDn, CYRA_LATCH_OAM_A5);
    /*p31.ZEHA*/ cpu_bus.TS_D6.set_tribuf(_WEWU_OAM_A_CPU_RDn, ZUVE_LATCH_OAM_A6);
    /*p31.FYRA*/ cpu_bus.TS_D7.set_tribuf(_WEWU_OAM_A_CPU_RDn, ECED_LATCH_OAM_A7);

    /*p29.YFAP*/ cpu_bus.TS_D0.set_tribuf(_WUME_OAM_B_CPU_RDn, YDYV_LATCH_OAM_B0); // big tri
    /*p29.XELE*/ cpu_bus.TS_D1.set_tribuf(_WUME_OAM_B_CPU_RDn, YCEB_LATCH_OAM_B1);
    /*p29.YPON*/ cpu_bus.TS_D2.set_tribuf(_WUME_OAM_B_CPU_RDn, ZUCA_LATCH_OAM_B2);
    /*p29.XUVO*/ cpu_bus.TS_D3.set_tribuf(_WUME_OAM_B_CPU_RDn, WONE_LATCH_OAM_B3);
    /*p29.ZYSA*/ cpu_bus.TS_D4.set_tribuf(_WUME_OAM_B_CPU_RDn, ZAXE_LATCH_OAM_B4);
    /*p29.YWEG*/ cpu_bus.TS_D5.set_tribuf(_WUME_OAM_B_CPU_RDn, XAFU_LATCH_OAM_B5);
    /*p29.XABU*/ cpu_bus.TS_D6.set_tribuf(_WUME_OAM_B_CPU_RDn, YSES_LATCH_OAM_B6);
    /*p29.YTUX*/ cpu_bus.TS_D7.set_tribuf(_WUME_OAM_B_CPU_RDn, ZECA_LATCH_OAM_B7);
  }
}

//------------------------------------------------------------------------------

bool OamBus::commit() {
  bool changed = false;
  /*p31.XYKY*/ changed |= XYKY_LATCH_OAM_A0.commit_latch();
  /*p31.YRUM*/ changed |= YRUM_LATCH_OAM_A1.commit_latch();
  /*p31.YSEX*/ changed |= YSEX_LATCH_OAM_A2.commit_latch();
  /*p31.YVEL*/ changed |= YVEL_LATCH_OAM_A3.commit_latch();
  /*p31.WYNO*/ changed |= WYNO_LATCH_OAM_A4.commit_latch();
  /*p31.CYRA*/ changed |= CYRA_LATCH_OAM_A5.commit_latch();
  /*p31.ZUVE*/ changed |= ZUVE_LATCH_OAM_A6.commit_latch();
  /*p31.ECED*/ changed |= ECED_LATCH_OAM_A7.commit_latch();
  /*p29.YDYV*/ changed |= YDYV_LATCH_OAM_B0.commit_latch();
  /*p29.YCEB*/ changed |= YCEB_LATCH_OAM_B1.commit_latch();
  /*p29.ZUCA*/ changed |= ZUCA_LATCH_OAM_B2.commit_latch();
  /*p29.WONE*/ changed |= WONE_LATCH_OAM_B3.commit_latch();
  /*p29.ZAXE*/ changed |= ZAXE_LATCH_OAM_B4.commit_latch();
  /*p29.XAFU*/ changed |= XAFU_LATCH_OAM_B5.commit_latch();
  /*p29.YSES*/ changed |= YSES_LATCH_OAM_B6.commit_latch();
  /*p29.ZECA*/ changed |= ZECA_LATCH_OAM_B7.commit_latch();
  /*p31.YLOR*/ changed |= YLOR_SPRITE_X0.commit_reg();
  /*p31.ZYTY*/ changed |= ZYTY_SPRITE_X1.commit_reg();
  /*p31.ZYVE*/ changed |= ZYVE_SPRITE_X2.commit_reg();
  /*p31.ZEZY*/ changed |= ZEZY_SPRITE_X3.commit_reg();
  /*p31.GOMO*/ changed |= GOMO_SPRITE_X4.commit_reg();
  /*p31.BAXO*/ changed |= BAXO_SPRITE_X5.commit_reg();
  /*p31.YZOS*/ changed |= YZOS_SPRITE_X6.commit_reg();
  /*p31.DEPO*/ changed |= DEPO_SPRITE_X7.commit_reg();
  /*p29.XUSO*/ changed |= XUSO_SPRITE_Y0.commit_reg();
  /*p29.XEGU*/ changed |= XEGU_SPRITE_Y1.commit_reg();
  /*p29.YJEX*/ changed |= YJEX_SPRITE_Y2.commit_reg();
  /*p29.XYJU*/ changed |= XYJU_SPRITE_Y3.commit_reg();
  /*p29.YBOG*/ changed |= YBOG_SPRITE_Y4.commit_reg();
  /*p29.WYSO*/ changed |= WYSO_SPRITE_Y5.commit_reg();
  /*p29.XOTE*/ changed |= XOTE_SPRITE_Y6.commit_reg();
  /*p29.YZAB*/ changed |= YZAB_SPRITE_Y7.commit_reg();

  changed |= PIN_CLK.commit_pinout();
  changed |= PIN_OE.commit_pinout();
  changed |= PIN_WR_A.commit_pinout(); // definitely write
  changed |= PIN_WR_B.commit_pinout(); // definitely write
  changed |= PIN_A0.commit_pinout();
  changed |= PIN_A1.commit_pinout();
  changed |= PIN_A2.commit_pinout();
  changed |= PIN_A3.commit_pinout();
  changed |= PIN_A4.commit_pinout();
  changed |= PIN_A5.commit_pinout();
  changed |= PIN_A6.commit_pinout();
  changed |= PIN_A7.commit_pinout();
  changed |= PIN_DA0.commit_tribuf();
  changed |= PIN_DA1.commit_tribuf();
  changed |= PIN_DA2.commit_tribuf();
  changed |= PIN_DA3.commit_tribuf();
  changed |= PIN_DA4.commit_tribuf();
  changed |= PIN_DA5.commit_tribuf();
  changed |= PIN_DA6.commit_tribuf();
  changed |= PIN_DA7.commit_tribuf();
  changed |= PIN_DB0.commit_tribuf();
  changed |= PIN_DB1.commit_tribuf();
  changed |= PIN_DB2.commit_tribuf();
  changed |= PIN_DB3.commit_tribuf();
  changed |= PIN_DB4.commit_tribuf();
  changed |= PIN_DB5.commit_tribuf();
  changed |= PIN_DB6.commit_tribuf();
  changed |= PIN_DB7.commit_tribuf();

  return changed;
}

//------------------------------------------------------------------------------