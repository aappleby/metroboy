#include "Sch_OAM.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// ACYL04 = and(BOGE01, BESU04)

void TestGB::tick_oam() {
  auto clk_sig = clk_reg.sig(*this);
  auto adr_sig = adr_reg.sig(cpu_pins);
  auto dma_sig = dma_reg.sig(*this);
  auto cpu_sig = cpu_reg.sig(*this);
  auto dbg_sig = dbg_reg.sig(*this);

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp.q());

  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_reg.MATU_DMA_OAM_WRp.q());
  /*p28.ACYL*/ wire ACYL_PPU_OAM_RDp = and(BOGE_DMA_RUNNINGn, sst_reg.BESU_SCANNINGp);
  /*p28.AJON*/ wire AJON_PPU_OAM_RDp = and(BOGE_DMA_RUNNINGn, ppu_reg.XYMU_RENDERINGp.q()); // def AND. ppu can read oam when there's rendering but no dma
  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_reg.MATU_DMA_OAM_WRp.q(), ACYL_PPU_OAM_RDp, AJON_PPU_OAM_RDp); // def nor
  /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and(adr_sig.SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and

  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(ppu_reg.TOXE_SFETCH_S0_D0.q());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYTU_SFETCH_S0_D0n, ppu_reg.TYFO_SFETCH_S0_D1.q());



  {
    // cpu can read oam when there's no parsing, rendering, or dma
    // so byte 0 is the palette number? something wrong here...
    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GARO*/ wire GARO_A0n = not(cpu_pins.A00);
    /*p28.WACU*/ wire WACU_A1n = not(cpu_pins.A01);
    /*p28.GOSE*/ wire GOSE_A2n = not(cpu_pins.A02);
    /*p28.WAPE*/ wire WAPE_A3n = not(cpu_pins.A03);
    /*p28.FEVU*/ wire FEVU_A4n = not(cpu_pins.A04);
    /*p28.GERA*/ wire GERA_A5n = not(cpu_pins.A05);
    /*p28.WAXA*/ wire WAXA_A6n = not(cpu_pins.A06);
    /*p28.FOBY*/ wire FOBY_A7n = not(cpu_pins.A07);

    /*p28.GYBU*/ wire GYBU_IDX_0n = not(sst_reg.WUZY_TS_IDX_0);
    /*p28.GYKA*/ wire GYKA_IDX_1n = not(sst_reg.WYSE_TS_IDX_1);
    /*p28.FABY*/ wire FABY_IDX_2n = not(sst_reg.ZYSU_TS_IDX_2);
    /*p28.FACO*/ wire FACO_IDX_3n = not(sst_reg.WYDA_TS_IDX_3);
    /*p28.FUGU*/ wire FUGU_IDX_4n = not(sst_reg.WUCO_TS_IDX_4);
    /*p28.FYKE*/ wire FYKE_IDX_5n = not(sst_reg.WEZA_TS_IDX_5);

    /*p28.GUSE*/ wire GUSE_SCAN0n = not(sst_reg.SCAN0.q());
    /*p28.GEMA*/ wire GEMA_SCAN1n = not(sst_reg.SCAN1.q());
    /*p28.FUTO*/ wire FUTO_SCAN2n = not(sst_reg.SCAN2.q());
    /*p28.FAKU*/ wire FAKU_SCAN3n = not(sst_reg.SCAN3.q());
    /*p28.GAMA*/ wire GAMA_SCAN4n = not(sst_reg.SCAN4.q());
    /*p28.GOBY*/ wire GOBY_SCAN5n = not(sst_reg.SCAN5.q());

    /*p28.FETU*/ wire FETU_DMA_A07n = not(dma_reg.DMA_A07.q());
    /*p28.FYDU*/ wire FYDU_DMA_A06n = not(dma_reg.DMA_A06.q());
    /*p28.EDOL*/ wire EDOL_DMA_A05n = not(dma_reg.DMA_A05.q());
    /*p28.ELUG*/ wire ELUG_DMA_A04n = not(dma_reg.DMA_A04.q());
    /*p28.FYKY*/ wire FYKY_DMA_A03n = not(dma_reg.DMA_A03.q());
    /*p28.FAGO*/ wire FAGO_DMA_A02n = not(dma_reg.DMA_A02.q());
    /*p28.FESA*/ wire FESA_DMA_A01n = not(dma_reg.DMA_A01.q());
    /*p28.FODO*/ wire FODO_DMA_A00n = not(dma_reg.DMA_A00.q());

    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn = or (ACYL_PPU_OAM_RDp, ppu_reg.XYMU_RENDERINGp.q(), dma_reg.MATU_DMA_OAM_WRp.q());
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn = not(AJON_PPU_OAM_RDp);
    /*p28.APAR*/ wire APAR_PPU_OAM_RDn = not(ACYL_PPU_OAM_RDp);
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn = not(dma_reg.MATU_DMA_OAM_WRp.q()); // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p28.GEKA*/ wire GEKA_OAM_A0p = not((GARO_A0n & !ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp  & !BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn & !APAR_PPU_OAM_RDn) | (FODO_DMA_A00n & !DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ wire ZYFO_OAM_A1p = not((WACU_A1n & !ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp  & !BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn & !APAR_PPU_OAM_RDn) | (FESA_DMA_A01n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ wire YFOT_OAM_A2p = not((GOSE_A2n & !ASAM_CPU_OAM_RDn) | (GYBU_IDX_0n & !BETE_PPU_OAM_RDn) | (GUSE_SCAN0n & !APAR_PPU_OAM_RDn) | (FAGO_DMA_A02n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ wire YFOC_OAM_A3p = not((WAPE_A3n & !ASAM_CPU_OAM_RDn) | (GYKA_IDX_1n & !BETE_PPU_OAM_RDn) | (GEMA_SCAN1n & !APAR_PPU_OAM_RDn) | (FYKY_DMA_A03n & !DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ wire YVOM_OAM_A4p = not((FEVU_A4n & !ASAM_CPU_OAM_RDn) | (FABY_IDX_2n & !BETE_PPU_OAM_RDn) | (FUTO_SCAN2n & !APAR_PPU_OAM_RDn) | (ELUG_DMA_A04n & !DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ wire YMEV_OAM_A5p = not((GERA_A5n & !ASAM_CPU_OAM_RDn) | (FACO_IDX_3n & !BETE_PPU_OAM_RDn) | (FAKU_SCAN3n & !APAR_PPU_OAM_RDn) | (EDOL_DMA_A05n & !DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ wire XEMU_OAM_A6p = not((WAXA_A6n & !ASAM_CPU_OAM_RDn) | (FUGU_IDX_4n & !BETE_PPU_OAM_RDn) | (GAMA_SCAN4n & !APAR_PPU_OAM_RDn) | (FYDU_DMA_A06n & !DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ wire YZET_OAM_A7p = not((FOBY_A7n & !ASAM_CPU_OAM_RDn) | (FYKE_IDX_5n & !BETE_PPU_OAM_RDn) | (GOBY_SCAN5n & !APAR_PPU_OAM_RDn) | (FETU_DMA_A07n & !DUGA_DMA_OAM_RDn));

    /*p28.GEKA*/ oam_pins.A0.set(GEKA_OAM_A0p);
    /*p28.ZYFO*/ oam_pins.A1.set(ZYFO_OAM_A1p);
    /*p28.YFOT*/ oam_pins.A2.set(YFOT_OAM_A2p);
    /*p28.YFOC*/ oam_pins.A3.set(YFOC_OAM_A3p);
    /*p28.YVOM*/ oam_pins.A4.set(YVOM_OAM_A4p);
    /*p28.YMEV*/ oam_pins.A5.set(YMEV_OAM_A5p);
    /*p28.XEMU*/ oam_pins.A6.set(XEMU_OAM_A6p);
    /*p28.YZET*/ oam_pins.A7.set(YZET_OAM_A7p);

    /*p04.NAXY*/ wire NAXY_DMA_OAM_WENp = nor(dbg_reg.FROM_CPU5_SYNC.q(), dma_reg.LUVY_DMA_TRIG_d0.q()); // def nor
    /*p04.POWU*/ wire POWU_DMA_OAM_WRp  = and(dma_reg.MATU_DMA_OAM_WRp.q(), NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire WYJA_OAM_WRp      = or (and(AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), POWU_DMA_OAM_WRp);

    /*p28.WAFO*/ wire WAFO_OAM_A0n      = not(GEKA_OAM_A0p); // def not
    /*p28.YNYC*/ wire YNYC_OAM_B_WRn    = and(WYJA_OAM_WRp, WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire YLYC_OAM_A_WRn    = and(WYJA_OAM_WRp, GEKA_OAM_A0p); // def and

    /*p28.ZONE*/ wire ZONE = not(YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire ZOFE = not(YNYC_OAM_B_WRn);

    oam_pins.WR_A.set(ZONE);
    oam_pins.WR_B.set(ZOFE);

  }

  {
    /*p28.XUTO*/ wire XUTO_CPU_OAM_WR = and(adr_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p28.WUJE*/ wire WUJE_CPU_OAM_WR = or (clk_sig.XYNY_xBCDExxx, XUTO_CPU_OAM_WR);
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WR = not(WUJE_CPU_OAM_WR);
    /*p28.ADAH*/ wire ADAH_ADDR_OAMn  = not(adr_sig.SARO_FE00_FEFFp);
    /*p28.APAG*/ wire APAG_D_TO_OAMDn = amux2(XUPA_CPU_OAM_WR, AMAB_OAM_LOCKn, AJUJ_OAM_BUSYn, ADAH_ADDR_OAMn);
    /*p28.AZUL*/ wire AZUL_D_TO_OAMD  = not(APAG_D_TO_OAMDn);

    /*p28.ZAXA*/ oam_pins.A_D0.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D0);
    /*p28.ZAKY*/ oam_pins.A_D1.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D1);
    /*p28.WULE*/ oam_pins.A_D2.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D2);
    /*p28.ZOZO*/ oam_pins.A_D3.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUFO*/ oam_pins.A_D4.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D4);
    /*p28.ZATO*/ oam_pins.A_D5.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D5);
    /*p28.YVUC*/ oam_pins.A_D6.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D6);
    /*p28.ZUFE*/ oam_pins.A_D7.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D7);

    /*p28.ZAMY*/ oam_pins.B_D0.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D0); // small tri
    /*p28.ZOPU*/ oam_pins.B_D1.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D1);
    /*p28.WYKY*/ oam_pins.B_D2.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D2);
    /*p28.ZAJA*/ oam_pins.B_D3.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUGA*/ oam_pins.B_D4.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D4);
    /*p28.ZUMO*/ oam_pins.B_D5.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D5);
    /*p28.XYTO*/ oam_pins.B_D6.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D6);
    /*p28.ZYFA*/ oam_pins.B_D7.set_tribuf(AZUL_D_TO_OAMD, cpu_pins.D7);
  }

  {
    /*p28.AZAR*/ wire AZAR_DMA_READ_VRAMn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ oam_pins.A_D0.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD0);
    /*p28.AXER*/ oam_pins.A_D1.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD1);
    /*p28.ASOX*/ oam_pins.A_D2.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD2);
    /*p28.CETU*/ oam_pins.A_D3.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD3);
    /*p28.ARYN*/ oam_pins.A_D4.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD4);
    /*p28.ACOT*/ oam_pins.A_D5.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD5);
    /*p28.CUJE*/ oam_pins.A_D6.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD6);
    /*p28.ATER*/ oam_pins.A_D7.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD7);

    /*p28.WOWA*/ oam_pins.B_D0.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD0);
    /*p28.AVEB*/ oam_pins.B_D1.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD1);
    /*p28.AMUH*/ oam_pins.B_D2.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD2);
    /*p28.COFO*/ oam_pins.B_D3.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD3);
    /*p28.AZOZ*/ oam_pins.B_D4.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD4);
    /*p28.AGYK*/ oam_pins.B_D5.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD5);
    /*p28.BUSE*/ oam_pins.B_D6.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD6);
    /*p28.ANUM*/ oam_pins.B_D7.set_tribuf(!AZAR_DMA_READ_VRAMn, ppu_reg.MD7);
  }

  {
    /*p25.WEJO*/ oam_pins.A_D0.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BUBO*/ oam_pins.A_D1.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BETU*/ oam_pins.A_D2.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CYME*/ oam_pins.A_D3.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BAXU*/ oam_pins.A_D4.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUHU*/ oam_pins.A_D5.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BYNY*/ oam_pins.A_D6.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BYPY*/ oam_pins.A_D7.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));

    /*p25.WASA*/ oam_pins.B_D0.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BOMO*/ oam_pins.B_D1.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BASA*/ oam_pins.B_D2.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CAKO*/ oam_pins.B_D3.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BUMA*/ oam_pins.B_D4.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUPY*/ oam_pins.B_D5.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BASY*/ oam_pins.B_D6.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BAPE*/ oam_pins.B_D7.set_tribuf(!dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));
  }

  {
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDn = or (LOBY_RENDERINGn, ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.VONU_SFETCH_S1_D4.q()); // TUVO is low for the first 4 phases of a sprite fetch
    
    /*p25.AVER*/ wire AVER = nand(ACYL_PPU_OAM_RDp, clk_sig.XYSO_ABCxDEFx); // schematic wrong, is def nand
    /*p25.VAPE*/ wire VAPE = and(TUVO_PPU_OAM_RDn, TACU_SPR_SEQ_5_TRIG); // def and
    /*p25.XUJY*/ wire XUJY = not(VAPE); // XUJY seems to have a one-phase glitch?
    /*p25.CUFE*/ wire CUFE_OAM_WR = and(or (adr_sig.SARO_FE00_FEFFp, dma_reg.MATU_DMA_OAM_WRp.q()), clk_sig.MOPA_AxxxxFGH); // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

    /*p25.BYCU*/ wire BYCU_OAM_CLKn = nand(AVER, XUJY, CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p25.COTA*/ wire COTA_OAM_CLKp = not(BYCU_OAM_CLKn);
    oam_pins.COTA_CLK.set(COTA_OAM_CLKp);

    /*p28.WEFY*/ wire WEFY = and(TUVO_PPU_OAM_RDn, ppu_reg.TYFO_SFETCH_S0_D1.q());
    /*p28.AJEP*/ wire AJEP = nand(ACYL_PPU_OAM_RDp, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire XUJA = not(WEFY);
    /*p28.BOTA*/ wire BOTA_CPU_RD_OAMn = nand(cpu_sig.DECY_FROM_CPU5n, adr_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire ASYT_OAM_LATCHn = and(AJEP, XUJA, BOTA_CPU_RD_OAMn);
    /*p28.BODE*/ wire BODE_OAM_LATCH = not(ASYT_OAM_LATCHn); // to the tribus receiver below

    /*p28.YVAL*/ wire YVAL_OAM_LATCHo = not(BODE_OAM_LATCH);
    /*p28.YRYV*/ wire YRYU_OAM_LATCHb = not(YVAL_OAM_LATCHo);
    /*p28.ZODO*/ wire ZODO_OAM_OE = not(YRYU_OAM_LATCHb);
    oam_pins.ZODO_OE.set(ZODO_OAM_OE);

    /*p31.XYKY*/ oam_reg.LATCH_OAM_A0.tp_latch(BODE_OAM_LATCH, oam_pins.A_D0);
    /*p31.YRUM*/ oam_reg.LATCH_OAM_A1.tp_latch(BODE_OAM_LATCH, oam_pins.A_D1);
    /*p31.YSEX*/ oam_reg.LATCH_OAM_A2.tp_latch(BODE_OAM_LATCH, oam_pins.A_D2);
    /*p31.YVEL*/ oam_reg.LATCH_OAM_A3.tp_latch(BODE_OAM_LATCH, oam_pins.A_D3);
    /*p31.WYNO*/ oam_reg.LATCH_OAM_A4.tp_latch(BODE_OAM_LATCH, oam_pins.A_D4);
    /*p31.CYRA*/ oam_reg.LATCH_OAM_A5.tp_latch(BODE_OAM_LATCH, oam_pins.A_D5);
    /*p31.ZUVE*/ oam_reg.LATCH_OAM_A6.tp_latch(BODE_OAM_LATCH, oam_pins.A_D6);
    /*p31.ECED*/ oam_reg.LATCH_OAM_A7.tp_latch(BODE_OAM_LATCH, oam_pins.A_D7);

    /*p29.YDYV*/ oam_reg.LATCH_OAM_B0.tp_latch(BODE_OAM_LATCH, oam_pins.B_D0);
    /*p29.YCEB*/ oam_reg.LATCH_OAM_B1.tp_latch(BODE_OAM_LATCH, oam_pins.B_D1);
    /*p29.ZUCA*/ oam_reg.LATCH_OAM_B2.tp_latch(BODE_OAM_LATCH, oam_pins.B_D2);
    /*p29.WONE*/ oam_reg.LATCH_OAM_B3.tp_latch(BODE_OAM_LATCH, oam_pins.B_D3);
    /*p29.ZAXE*/ oam_reg.LATCH_OAM_B4.tp_latch(BODE_OAM_LATCH, oam_pins.B_D4);
    /*p29.XAFU*/ oam_reg.LATCH_OAM_B5.tp_latch(BODE_OAM_LATCH, oam_pins.B_D5);
    /*p29.YSES*/ oam_reg.LATCH_OAM_B6.tp_latch(BODE_OAM_LATCH, oam_pins.B_D6);
    /*p29.ZECA*/ oam_reg.LATCH_OAM_B7.tp_latch(BODE_OAM_LATCH, oam_pins.B_D7);

    /*p31.XEGA*/ wire XEGA_OAM_CLKn = not(COTA_OAM_CLKp);
    /*p31.YLOR*/ oam_reg.REG_OAM_A0.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A0);
    /*p31.ZYTY*/ oam_reg.REG_OAM_A1.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A1);
    /*p31.ZYVE*/ oam_reg.REG_OAM_A2.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A2);
    /*p31.ZEZY*/ oam_reg.REG_OAM_A3.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A3);
    /*p31.GOMO*/ oam_reg.REG_OAM_A4.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A4);
    /*p31.BAXO*/ oam_reg.REG_OAM_A5.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A5);
    /*p31.YZOS*/ oam_reg.REG_OAM_A6.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A6);
    /*p31.DEPO*/ oam_reg.REG_OAM_A7.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A7);

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(COTA_OAM_CLKp);
    /*p29.XUSO*/ oam_reg.REG_OAM_B0.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B0);
    /*p29.XEGU*/ oam_reg.REG_OAM_B1.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B1);
    /*p29.YJEX*/ oam_reg.REG_OAM_B2.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B2);
    /*p29.XYJU*/ oam_reg.REG_OAM_B3.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B3);
    /*p29.YBOG*/ oam_reg.REG_OAM_B4.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B4);
    /*p29.WYSO*/ oam_reg.REG_OAM_B5.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B5);
    /*p29.XOTE*/ oam_reg.REG_OAM_B6.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B6);
    /*p29.YZAB*/ oam_reg.REG_OAM_B7.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B7);
  }
}
