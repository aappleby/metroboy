#include "Sch_BusMux.h"

#include "TestGB.h"

using namespace Schematics;

void BusMux::tick(TestGB& gb) {
  auto& dma_reg = gb.dma_reg;
  auto& sst_reg = gb.sst_reg;
  auto& ppu_reg = gb.ppu_reg;
  auto& dbg_reg = gb.dbg_reg;
  auto& oam_reg = gb.oam_reg;

  auto clk_sig = gb.clk_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto cpu_sig = gb.cpu_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  //auto& ext_bus = gb.ext_bus;
  auto& vram_bus = gb.vram_bus;
  auto& oam_bus = gb.oam_bus;

  auto& vram_pins = gb.vram_pins;

  /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(dma_sig.LUFA_DMA_READ_VRAMp, ppu_sig.XYMU_RENDERINGp); // def nor

  {
    // CPU vram read
    /*p25.XEDU*/ wire _XEDU_VRAM_LOCK = not(_XANE_VRAM_LOCKn);
    /*p25.XAKY*/ vram_bus.TS_MA00.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A00);
    /*p25.XUXU*/ vram_bus.TS_MA01.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A01);
    /*p25.XYNE*/ vram_bus.TS_MA02.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A02);
    /*p25.XODY*/ vram_bus.TS_MA03.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A03);
    /*p25.XECA*/ vram_bus.TS_MA04.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A04);
    /*p25.XOBA*/ vram_bus.TS_MA05.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A05);
    /*p25.XOPO*/ vram_bus.TS_MA06.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A06);
    /*p25.XYBO*/ vram_bus.TS_MA07.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A07);
    /*p25.RYSU*/ vram_bus.TS_MA08.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A08);
    /*p25.RESE*/ vram_bus.TS_MA09.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A09);
    /*p25.RUSE*/ vram_bus.TS_MA10.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A10);
    /*p25.RYNA*/ vram_bus.TS_MA11.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A11);
    /*p25.RUMO*/ vram_bus.TS_MA12.set_tribuf(_XEDU_VRAM_LOCK, cpu_bus.A12);
  }

  {
    // DMA vram read
    /*p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p04.ECAL*/ vram_bus.TS_MA00.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A00.q());
    /*p04.EGEZ*/ vram_bus.TS_MA01.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A01.q());
    /*p04.FUHE*/ vram_bus.TS_MA02.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A02.q());
    /*p04.FYZY*/ vram_bus.TS_MA03.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A03.q());
    /*p04.DAMU*/ vram_bus.TS_MA04.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A04.q());
    /*p04.DAVA*/ vram_bus.TS_MA05.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A05.q());
    /*p04.ETEG*/ vram_bus.TS_MA06.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A06.q());
    /*p04.EREW*/ vram_bus.TS_MA07.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A07.q());
    /*p04.EVAX*/ vram_bus.TS_MA08.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A08.q());
    /*p04.DUVE*/ vram_bus.TS_MA09.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A09.q());
    /*p04.ERAF*/ vram_bus.TS_MA10.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A10.q());
    /*p04.FUSY*/ vram_bus.TS_MA11.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A11.q());
    /*p04.EXYF*/ vram_bus.TS_MA12.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A12.q());
  }

  /*p25.TAVY*/ wire TAVY_MOE_Cn = not(vram_pins.PIN_MOEn_C);
  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(adr_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAM);
  /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
  /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
  /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
  /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);
  /*p25.ROCY*/ wire ROCY_VBUS_TRISTATEn = and (REVO_VRAM_RDp, SAZO_VRAM_RD);
  /*p25.RAHU*/ wire RAHU_VBUS_TRISTATEp = not(ROCY_VBUS_TRISTATEn);

  {
    /*p25.TEME*/ vram_bus.TS_MD0.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D0);
    /*p25.TEWU*/ vram_bus.TS_MD1.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D1);
    /*p25.TYGO*/ vram_bus.TS_MD2.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D2);
    /*p25.SOTE*/ vram_bus.TS_MD3.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D3);
    /*p25.SEKE*/ vram_bus.TS_MD4.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D4);
    /*p25.RUJO*/ vram_bus.TS_MD5.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D5);
    /*p25.TOFA*/ vram_bus.TS_MD6.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D6);
    /*p25.SUZA*/ vram_bus.TS_MD7.set_tribuf(RAHU_VBUS_TRISTATEp, cpu_bus.D7);
  }


  {
    // cpu can read oam when there's no parsing, rendering, or dma
    // so byte 0 is the palette number? something wrong here...
    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GARO*/ wire _GARO_A0n = not(cpu_bus.A00);
    /*p28.WACU*/ wire _WACU_A1n = not(cpu_bus.A01);
    /*p28.GOSE*/ wire _GOSE_A2n = not(cpu_bus.A02);
    /*p28.WAPE*/ wire _WAPE_A3n = not(cpu_bus.A03);
    /*p28.FEVU*/ wire _FEVU_A4n = not(cpu_bus.A04);
    /*p28.GERA*/ wire _GERA_A5n = not(cpu_bus.A05);
    /*p28.WAXA*/ wire _WAXA_A6n = not(cpu_bus.A06);
    /*p28.FOBY*/ wire _FOBY_A7n = not(cpu_bus.A07);

    /*p28.GYBU*/ wire _GYBU_IDX_0n = not(sst_reg.WUZY_TS_IDX_0);
    /*p28.GYKA*/ wire _GYKA_IDX_1n = not(sst_reg.WYSE_TS_IDX_1);
    /*p28.FABY*/ wire _FABY_IDX_2n = not(sst_reg.ZYSU_TS_IDX_2);
    /*p28.FACO*/ wire _FACO_IDX_3n = not(sst_reg.WYDA_TS_IDX_3);
    /*p28.FUGU*/ wire _FUGU_IDX_4n = not(sst_reg.WUCO_TS_IDX_4);
    /*p28.FYKE*/ wire _FYKE_IDX_5n = not(sst_reg.WEZA_TS_IDX_5);

    /*p28.GUSE*/ wire _GUSE_SCAN0n = not(sst_reg.SCAN0.q());
    /*p28.GEMA*/ wire _GEMA_SCAN1n = not(sst_reg.SCAN1.q());
    /*p28.FUTO*/ wire _FUTO_SCAN2n = not(sst_reg.SCAN2.q());
    /*p28.FAKU*/ wire _FAKU_SCAN3n = not(sst_reg.SCAN3.q());
    /*p28.GAMA*/ wire _GAMA_SCAN4n = not(sst_reg.SCAN4.q());
    /*p28.GOBY*/ wire _GOBY_SCAN5n = not(sst_reg.SCAN5.q());

    /*p28.FETU*/ wire _FETU_DMA_A07n = not(dma_reg.DMA_A07.q());
    /*p28.FYDU*/ wire _FYDU_DMA_A06n = not(dma_reg.DMA_A06.q());
    /*p28.EDOL*/ wire _EDOL_DMA_A05n = not(dma_reg.DMA_A05.q());
    /*p28.ELUG*/ wire _ELUG_DMA_A04n = not(dma_reg.DMA_A04.q());
    /*p28.FYKY*/ wire _FYKY_DMA_A03n = not(dma_reg.DMA_A03.q());
    /*p28.FAGO*/ wire _FAGO_DMA_A02n = not(dma_reg.DMA_A02.q());
    /*p28.FESA*/ wire _FESA_DMA_A01n = not(dma_reg.DMA_A01.q());
    /*p28.FODO*/ wire _FODO_DMA_A00n = not(dma_reg.DMA_A00.q());

    /*p28.AJON*/ wire _AJON_PPU_USE_OAM2 = and (dma_sig.BOGE_DMA_RUNNINGn, ppu_reg.XYMU_RENDERINGp.q()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor(dma_reg.MATU_DMA_OAM_WRp.q(), ppu_sig.ACYL_PPU_USE_OAM1p, _AJON_PPU_USE_OAM2); // def nor
    /*p28.AMAB*/ wire _AMAB_OAM_LOCKn = and (adr_sig.SARO_FE00_FEFFp, _AJUJ_OAM_BUSYn); // def and

    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn = or (ppu_sig.ACYL_PPU_USE_OAM1p, ppu_reg.XYMU_RENDERINGp.q(), dma_reg.MATU_DMA_OAM_WRp.q());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn = not(_AJON_PPU_USE_OAM2);
    /*p28.APAR*/ wire _APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_PPU_USE_OAM1p);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn = not(dma_reg.MATU_DMA_OAM_WRp.q()); // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not((_GARO_A0n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FODO_DMA_A00n & !_DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ wire _ZYFO_OAM_A1p = not((_WACU_A1n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FESA_DMA_A01n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ wire _YFOT_OAM_A2p = not((_GOSE_A2n & !_ASAM_CPU_OAM_RDn) | (_GYBU_IDX_0n & !_BETE_PPU_OAM_RDn) | (_GUSE_SCAN0n & !_APAR_PPU_OAM_RDn) | (_FAGO_DMA_A02n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ wire _YFOC_OAM_A3p = not((_WAPE_A3n & !_ASAM_CPU_OAM_RDn) | (_GYKA_IDX_1n & !_BETE_PPU_OAM_RDn) | (_GEMA_SCAN1n & !_APAR_PPU_OAM_RDn) | (_FYKY_DMA_A03n & !_DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ wire _YVOM_OAM_A4p = not((_FEVU_A4n & !_ASAM_CPU_OAM_RDn) | (_FABY_IDX_2n & !_BETE_PPU_OAM_RDn) | (_FUTO_SCAN2n & !_APAR_PPU_OAM_RDn) | (_ELUG_DMA_A04n & !_DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ wire _YMEV_OAM_A5p = not((_GERA_A5n & !_ASAM_CPU_OAM_RDn) | (_FACO_IDX_3n & !_BETE_PPU_OAM_RDn) | (_FAKU_SCAN3n & !_APAR_PPU_OAM_RDn) | (_EDOL_DMA_A05n & !_DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ wire _XEMU_OAM_A6p = not((_WAXA_A6n & !_ASAM_CPU_OAM_RDn) | (_FUGU_IDX_4n & !_BETE_PPU_OAM_RDn) | (_GAMA_SCAN4n & !_APAR_PPU_OAM_RDn) | (_FYDU_DMA_A06n & !_DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ wire _YZET_OAM_A7p = not((_FOBY_A7n & !_ASAM_CPU_OAM_RDn) | (_FYKE_IDX_5n & !_BETE_PPU_OAM_RDn) | (_GOBY_SCAN5n & !_APAR_PPU_OAM_RDn) | (_FETU_DMA_A07n & !_DUGA_DMA_OAM_RDn));

    /*p28.GEKA*/ oam_bus.A0.set(_GEKA_OAM_A0p);
    /*p28.ZYFO*/ oam_bus.A1.set(_ZYFO_OAM_A1p);
    /*p28.YFOT*/ oam_bus.A2.set(_YFOT_OAM_A2p);
    /*p28.YFOC*/ oam_bus.A3.set(_YFOC_OAM_A3p);
    /*p28.YVOM*/ oam_bus.A4.set(_YVOM_OAM_A4p);
    /*p28.YMEV*/ oam_bus.A5.set(_YMEV_OAM_A5p);
    /*p28.XEMU*/ oam_bus.A6.set(_XEMU_OAM_A6p);
    /*p28.YZET*/ oam_bus.A7.set(_YZET_OAM_A7p);

    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(_GEKA_OAM_A0p); // def not

    {
      /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(dbg_reg.FROM_CPU5_SYNC.q(), dma_reg.LUVY_DMA_TRIG_d0.q()); // def nor
      /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (dma_reg.MATU_DMA_OAM_WRp.q(), _NAXY_DMA_OAM_WENp); // def and
      /*p04.WYJA*/ wire _WYJA_OAM_WRp = or (and (_AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), _POWU_DMA_OAM_WRp);

      /*p28.YNYC*/ wire _YNYC_OAM_B_WRn = and (_WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
      /*p28.YLYC*/ wire _YLYC_OAM_A_WRn = and (_WYJA_OAM_WRp, _GEKA_OAM_A0p); // def and

      /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRn);
      /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRn);

      oam_bus.WR_A.set(_ZONE);
      oam_bus.WR_B.set(_ZOFE);
    }

    /*p04.CATY*/ wire _CATY_FROM_CPU5p = not(cpu_sig.DECY_FROM_CPU5n);
    /*p28.MYNU*/ wire _MYNU_CPU_RDn = nand(cpu_sig.ASOT_CPU_RD, _CATY_FROM_CPU5p);
    /*p28.LEKO*/ wire _LEKO_CPU_RDp = not(_MYNU_CPU_RDn);

    {
      /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD = and (_LEKO_CPU_RDp, _AMAB_OAM_LOCKn, oam_bus.A0);
      /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD = and (_LEKO_CPU_RDp, _AMAB_OAM_LOCKn, _WAFO_OAM_A0n);
      /*p28.WEWU*/ wire _WEWU_OAM_A_CPU_RDn = not(_WUKU_OAM_A_CPU_RD);
      /*p28.WUME*/ wire _WUME_OAM_B_CPU_RDn = not(_GUKO_OAM_B_CPU_RD);

      /*p31.XACA*/ cpu_bus.D0.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A0);
      /*p31.XAGU*/ cpu_bus.D1.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A1);
      /*p31.XEPU*/ cpu_bus.D2.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A2);
      /*p31.XYGU*/ cpu_bus.D3.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A3);
      /*p31.XUNA*/ cpu_bus.D4.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A4);
      /*p31.DEVE*/ cpu_bus.D5.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A5);
      /*p31.ZEHA*/ cpu_bus.D6.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A6);
      /*p31.FYRA*/ cpu_bus.D7.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A7);

      /*p29.YFAP*/ cpu_bus.D0.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B0); // big tri
      /*p29.XELE*/ cpu_bus.D1.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B1);
      /*p29.YPON*/ cpu_bus.D2.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B2);
      /*p29.XUVO*/ cpu_bus.D3.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B3);
      /*p29.ZYSA*/ cpu_bus.D4.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B4);
      /*p29.YWEG*/ cpu_bus.D5.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B5);
      /*p29.XABU*/ cpu_bus.D6.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B6);
      /*p29.YTUX*/ cpu_bus.D7.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B7);
    }

    {
      /*p25.TYVY*/ wire _TYVY_MD_TO_Dn = nand(ppu_sig.SERE_VRAM_RD, _LEKO_CPU_RDp);
      /*p25.SEBY*/ wire _SEBY_MD_TO_D = not(_TYVY_MD_TO_Dn);
      /*p25.RUGA*/ /*p25.RERY*/ cpu_bus.D0.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD0));
      /*p25.ROTA*/ /*p25.RUNA*/ cpu_bus.D1.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD1));
      /*p25.RYBU*/ /*p25.RONA*/ cpu_bus.D2.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD2));
      /*p25.RAJU*/ /*p25.RUNO*/ cpu_bus.D3.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD3));
      /*p25.TYJA*/ /*p25.SANA*/ cpu_bus.D4.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD4));
      /*p25.REXU*/ /*p25.RORO*/ cpu_bus.D5.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD5));
      /*p25.RUPY*/ /*p25.RABO*/ cpu_bus.D6.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD6));
      /*p25.TOKU*/ /*p25.SAME*/ cpu_bus.D7.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD7));
    }

    {
      /*p28.XUTO*/ wire _XUTO_CPU_OAM_WR = and (adr_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
      /*p28.WUJE*/ wire _WUJE_CPU_OAM_WR = or (clk_sig.XYNY_xBCDExxx, _XUTO_CPU_OAM_WR);
      /*p28.XUPA*/ wire _XUPA_CPU_OAM_WR = not(_WUJE_CPU_OAM_WR);
      /*p28.ADAH*/ wire _ADAH_ADDR_OAM = not(adr_sig.SARO_FE00_FEFFp);
      /*p28.APAG*/ wire _APAG_D_TO_OAMD = amux2(_XUPA_CPU_OAM_WR, _AMAB_OAM_LOCKn, _AJUJ_OAM_BUSYn, _ADAH_ADDR_OAM);
      /*p28.AZUL*/ wire _AZUL_D_TO_OAMD = not(_APAG_D_TO_OAMD);

      /*p28.ZAXA*/ oam_bus.DA0.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D0);
      /*p28.ZAKY*/ oam_bus.DA1.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D1);
      /*p28.WULE*/ oam_bus.DA2.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D2);
      /*p28.ZOZO*/ oam_bus.DA3.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D3);
      /*p28.ZUFO*/ oam_bus.DA4.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D4);
      /*p28.ZATO*/ oam_bus.DA5.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D5);
      /*p28.YVUC*/ oam_bus.DA6.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D6);
      /*p28.ZUFE*/ oam_bus.DA7.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D7);

      /*p28.ZAMY*/ oam_bus.DB0.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D0); // small tri
      /*p28.ZOPU*/ oam_bus.DB1.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D1);
      /*p28.WYKY*/ oam_bus.DB2.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D2);
      /*p28.ZAJA*/ oam_bus.DB3.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D3);
      /*p28.ZUGA*/ oam_bus.DB4.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D4);
      /*p28.ZUMO*/ oam_bus.DB5.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D5);
      /*p28.XYTO*/ oam_bus.DB6.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D6);
      /*p28.ZYFA*/ oam_bus.DB7.set_tribuf(_AZUL_D_TO_OAMD, cpu_bus.D7);
    }
  }

  {
    // VRAM pin handler
    /*p25.LEXE*/ vram_pins.PIN_MA00_AD.set(not(/*p25.MYFU*/ not(vram_bus.TS_MA00)));
    /*p25.LOZU*/ vram_pins.PIN_MA01_AD.set(not(/*p25.MASA*/ not(vram_bus.TS_MA01)));
    /*p25.LACA*/ vram_pins.PIN_MA02_AD.set(not(/*p25.MYRE*/ not(vram_bus.TS_MA02)));
    /*p25.LUVO*/ vram_pins.PIN_MA03_AD.set(not(/*p25.MAVU*/ not(vram_bus.TS_MA03)));
    /*p25.LOLY*/ vram_pins.PIN_MA04_AD.set(not(/*p25.MEPA*/ not(vram_bus.TS_MA04)));
    /*p25.LALO*/ vram_pins.PIN_MA05_AD.set(not(/*p25.MYSA*/ not(vram_bus.TS_MA05)));
    /*p25.LEFA*/ vram_pins.PIN_MA06_AD.set(not(/*p25.MEWY*/ not(vram_bus.TS_MA06)));
    /*p25.LUBY*/ vram_pins.PIN_MA07_AD.set(not(/*p25.MUME*/ not(vram_bus.TS_MA07)));
    /*p25.TUJY*/ vram_pins.PIN_MA08_AD.set(not(/*p25.VOVA*/ not(vram_bus.TS_MA08)));
    /*p25.TAGO*/ vram_pins.PIN_MA09_AD.set(not(/*p25.VODE*/ not(vram_bus.TS_MA09)));
    /*p25.NUVA*/ vram_pins.PIN_MA10_AD.set(not(/*p25.RUKY*/ not(vram_bus.TS_MA10)));
    /*p25.PEDU*/ vram_pins.PIN_MA11_AD.set(not(/*p25.RUMA*/ not(vram_bus.TS_MA11)));
    /*p25.PONY*/ vram_pins.PIN_MA12_AD.set(not(/*p25.REHO*/ not(vram_bus.TS_MA12)));
  }
}