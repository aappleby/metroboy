#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// Die trace:
// TEFA = nor(SYRO, TEXO);         OK
// SOSE = and( A15, TEFA);         OK
// TUCA = and(SOSE, ABUZ);         OK
// TEGU = nand(SOSE, CPU_RAW_WR);  OK
// TAVY = not(MOE_C);              OK
// TEFY = not(MCS_C);              OK

// TUTO = and(TOVA, SOTO);         OK
// ROPY = not(XYMU);               OK

// TOLE = mux2(TUTO, TEFY, TUCA)   OK
// SERE = and(TOLE, ROPY)          OK
// SALE = mux2(TUTO, TAVY, TEGU)   OK
// RUVY = not(SALE);               OK
// SAZO = and(SERE, RUVY)          OK
// RYJE = not(SAZO)                OK
// REVO = not(RYJE)                OK
// ROCY = and(SAZO, REVO)          OK
// RAHU = not(ROCY)                OK, big inverter
// ROVE = not(RAHU)                OK
// RELA = or (REVO, SAZO)          OK
// RENA = not(RELA)                OK

void TestGB::tick_vram_addr() {
  auto clk_sig = clk_reg.sig(*this);
  auto dbg_sig = dbg_reg.sig(*this);
  auto adr_sig = adr_reg.sig(cpu_pins);
  auto dma_sig = dma_reg.sig(*this);
  auto win_sig = win_reg.sig(*this);
  auto cpu_sig = cpu_reg.sig(*this);
  auto ppu_sig = ppu_reg.sig(*this);

  /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(dma_sig.LUFA_DMA_READ_VRAMp, ppu_reg.XYMU_RENDERINGp.q()); // def nor
  
  /*p25.TUCA*/ wire TUCA_CPU_VRAM_RD = and (adr_sig.SOSE_8000_9FFFp, dbg_sig.ABUZ);
  /*p25.TEFY*/ wire TEFY_MCS_Cn = not(vram_pins.MCS_C);
  /*p25.TOLE*/ wire TOLE_VRAM_RD = mux2_p(TEFY_MCS_Cn, TUCA_CPU_VRAM_RD, dbg_sig.TUTO_DBG_VRAM);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp.q());
  /*p25.SERE*/ wire SERE_VRAM_RD = and (TOLE_VRAM_RD, ROPY_RENDERINGn);

  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(ppu_reg.TOXE_SFETCH_S0_D0.q());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(ppu_reg.TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);

  /*p27.NAKO*/ wire NAKO_FETCH_S1n = not(ppu_reg.MESU_BFETCH_S1.q());
  /*p27.NOFU*/ wire NOFU_FETCH_S2n = not(ppu_reg.NYVA_BFETCH_S2.q());

  //---------------------------------------------------------------------------

  // Original

  /*p26.FAFO*/ wire _TILE_Y0S = add_s(lcd_reg.MUWY_Y0.q(), cfg_reg.SCY0.q(), 0);
  /*p26.FAFO*/ wire _TILE_Y0C = add_c(lcd_reg.MUWY_Y0.q(), cfg_reg.SCY0.q(), 0);
  /*p26.EMUX*/ wire _TILE_Y1S = add_s(lcd_reg.MYRO_Y1.q(), cfg_reg.SCY1.q(), _TILE_Y0C);
  /*p26.EMUX*/ wire _TILE_Y1C = add_c(lcd_reg.MYRO_Y1.q(), cfg_reg.SCY1.q(), _TILE_Y0C);
  /*p26.ECAB*/ wire _TILE_Y2S = add_s(lcd_reg.LEXA_Y2.q(), cfg_reg.SCY2.q(), _TILE_Y1C);
  /*p26.ECAB*/ wire _TILE_Y2C = add_c(lcd_reg.LEXA_Y2.q(), cfg_reg.SCY2.q(), _TILE_Y1C);
  /*p26.ETAM*/ wire _MAP_Y0S  = add_s(lcd_reg.LYDO_Y3.q(), cfg_reg.SCY3.q(), _TILE_Y2C);
  /*p26.ETAM*/ wire _MAP_Y0C  = add_c(lcd_reg.LYDO_Y3.q(), cfg_reg.SCY3.q(), _TILE_Y2C);
  /*p26.DOTO*/ wire _MAP_Y1S  = add_s(lcd_reg.LOVU_Y4.q(), cfg_reg.SCY4.q(), _MAP_Y0C);
  /*p26.DOTO*/ wire _MAP_Y1C  = add_c(lcd_reg.LOVU_Y4.q(), cfg_reg.SCY4.q(), _MAP_Y0C);
  /*p26.DABA*/ wire _MAP_Y2S  = add_s(lcd_reg.LEMA_Y5.q(), cfg_reg.SCY5.q(), _MAP_Y1C);
  /*p26.DABA*/ wire _MAP_Y2C  = add_c(lcd_reg.LEMA_Y5.q(), cfg_reg.SCY5.q(), _MAP_Y1C);
  /*p26.EFYK*/ wire _MAP_Y3S  = add_s(lcd_reg.MATO_Y6.q(), cfg_reg.SCY6.q(), _MAP_Y2C);
  /*p26.EFYK*/ wire _MAP_Y3C  = add_c(lcd_reg.MATO_Y6.q(), cfg_reg.SCY6.q(), _MAP_Y2C);
  /*p26.EJOK*/ wire _MAP_Y4S  = add_s(lcd_reg.LAFO_Y7.q(), cfg_reg.SCY7.q(), _MAP_Y3C);
  /*p26.EJOK*/ wire _MAP_Y4C  = add_c(lcd_reg.LAFO_Y7.q(), cfg_reg.SCY7.q(), _MAP_Y3C);

  /*p26.ATAD*/ wire _TILE_X0S = add_s(ppu_reg.SAXO_X0.q(), cfg_reg.SCX0.q(), 0); // unused
  /*p26.ATAD*/ wire _TILE_X0C = add_c(ppu_reg.SAXO_X0.q(), cfg_reg.SCX0.q(), 0);
  /*p26.BEHU*/ wire _TILE_X1S = add_s(ppu_reg.TYPO_X1.q(), cfg_reg.SCX1.q(), _TILE_X0C); // unused
  /*p26.BEHU*/ wire _TILE_X1C = add_c(ppu_reg.TYPO_X1.q(), cfg_reg.SCX1.q(), _TILE_X0C);
  /*p26.APYH*/ wire _TILE_X2S = add_s(ppu_reg.VYZO_X2.q(), cfg_reg.SCX2.q(), _TILE_X1C); // unused
  /*p26.APYH*/ wire _TILE_X2C = add_c(ppu_reg.VYZO_X2.q(), cfg_reg.SCX2.q(), _TILE_X1C);
  /*p26.BABE*/ wire _MAP_X0S  = add_s(ppu_reg.TELU_X3.q(), cfg_reg.SCX3.q(), _TILE_X2C);
  /*p26.BABE*/ wire _MAP_X0C  = add_c(ppu_reg.TELU_X3.q(), cfg_reg.SCX3.q(), _TILE_X2C);
  /*p26.ABOD*/ wire _MAP_X1S  = add_s(ppu_reg.SUDE_X4.q(), cfg_reg.SCX4.q(), _MAP_X0C);
  /*p26.ABOD*/ wire _MAP_X1C  = add_c(ppu_reg.SUDE_X4.q(), cfg_reg.SCX4.q(), _MAP_X0C);
  /*p26.BEWY*/ wire _MAP_X2S  = add_s(ppu_reg.TAHA_X5.q(), cfg_reg.SCX5.q(), _MAP_X1C);
  /*p26.BEWY*/ wire _MAP_X2C  = add_c(ppu_reg.TAHA_X5.q(), cfg_reg.SCX5.q(), _MAP_X1C);
  /*p26.BYCA*/ wire _MAP_X3S  = add_s(ppu_reg.TYRY_X6.q(), cfg_reg.SCX6.q(), _MAP_X2C);
  /*p26.BYCA*/ wire _MAP_X3C  = add_c(ppu_reg.TYRY_X6.q(), cfg_reg.SCX6.q(), _MAP_X2C);
  /*p26.ACUL*/ wire _MAP_X4S  = add_s(ppu_reg.SYBE_X7.q(), cfg_reg.SCX7.q(), _MAP_X3C);
  /*p26.ACUL*/ wire _MAP_X4C  = add_c(ppu_reg.SYBE_X7.q(), cfg_reg.SCX7.q(), _MAP_X3C);

  (void)_TILE_X0S; (void)_TILE_X1S; (void)_TILE_X2S; (void)_MAP_X4C; (void)_MAP_Y4C;

  //---------------------------------------------------------------------------


#if 0
  wire _NETA_TILE_READ = and(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q(), !ppu_reg.MESU_BFETCH_S1.q(), !ppu_reg.NYVA_BFETCH_S2.q());
  wire _POTU_BGW_FETCH_01 = and(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q(), !ppu_reg.MESU_BFETCH_S1.q(), !ppu_reg.NYVA_BFETCH_S2.q());
  wire SPRITE_READn = not(TEXY_SPRITE_READ());
#endif


  // CPU vram read
  // polarity? compare cell w/ dma vram read below

  // XAKY 6-rung tribuf, inputs on 1 and 4

  // XAKY01
  // XAKY02 nc
  // XAKY03 nc
  // XAKY04
  // XAKY05 nc
  // XAKY06

  {
    /*p25.XEDU*/ wire _XEDU_VRAM_LOCK = not(XANE_VRAM_LOCKn);
    /*p25.XAKY*/ ppu_reg.MA00.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A00);
    /*p25.XUXU*/ ppu_reg.MA01.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A01);
    /*p25.XYNE*/ ppu_reg.MA02.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A02);
    /*p25.XODY*/ ppu_reg.MA03.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A03);
    /*p25.XECA*/ ppu_reg.MA04.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A04);
    /*p25.XOBA*/ ppu_reg.MA05.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A05);
    /*p25.XOPO*/ ppu_reg.MA06.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A06);
    /*p25.XYBO*/ ppu_reg.MA07.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A07);
    /*p25.RYSU*/ ppu_reg.MA08.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A08);
    /*p25.RESE*/ ppu_reg.MA09.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A09);
    /*p25.RUSE*/ ppu_reg.MA10.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A10);
    /*p25.RYNA*/ ppu_reg.MA11.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A11);
    /*p25.RUMO*/ ppu_reg.MA12.set_tribuf(_XEDU_VRAM_LOCK, cpu_pins.A12);
  }

  {
    // DMA vram read
    /*p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p04.ECAL*/ ppu_reg.MA00.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A00.q());
    /*p04.EGEZ*/ ppu_reg.MA01.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A01.q());
    /*p04.FUHE*/ ppu_reg.MA02.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A02.q());
    /*p04.FYZY*/ ppu_reg.MA03.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A03.q());
    /*p04.DAMU*/ ppu_reg.MA04.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A04.q());
    /*p04.DAVA*/ ppu_reg.MA05.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A05.q());
    /*p04.ETEG*/ ppu_reg.MA06.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A06.q());
    /*p04.EREW*/ ppu_reg.MA07.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A07.q());
    /*p04.EVAX*/ ppu_reg.MA08.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A08.q());
    /*p04.DUVE*/ ppu_reg.MA09.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A09.q());
    /*p04.ERAF*/ ppu_reg.MA10.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A10.q());
    /*p04.FUSY*/ ppu_reg.MA11.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A11.q());
    /*p04.EXYF*/ ppu_reg.MA12.set_tribuf(_AHOC_DMA_VRAM_RDn, dma_reg.DMA_A12.q());
  }


  /*p27.LUSU*/ wire _LUSU_BGW_VRAM_RDn = not(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q());
  /*p27.LENA*/ wire _LENA_BGW_VRAM_RD = not(_LUSU_BGW_VRAM_RDn);

  {
    /*p27.NOGU*/ wire _NOGU_FETCH_01 = nand(NAKO_FETCH_S1n, NOFU_FETCH_S2n);
    /*p27.NENY*/ wire _NENY_FETCH_01n = not(_NOGU_FETCH_01);
    /*p27.POTU*/ wire _POTU_BGW_FETCH_01 = and (_LENA_BGW_VRAM_RD, _NENY_FETCH_01n);
    /*p26.AXAD*/ wire _AXAD_WIN_MODEn = not(win_sig.PORE_WIN_MODE);

    {
      // Background map read
      /*p26.ACEN*/ wire _ACEN_BG_MAP_READ = and (_POTU_BGW_FETCH_01, _AXAD_WIN_MODEn);
      /*p26.BAFY*/ wire _BAFY_BG_MAP_READn = not(_ACEN_BG_MAP_READ);
      /*p26.AXEP*/ ppu_reg.MA00.set_tribuf(_BAFY_BG_MAP_READn, _MAP_X0S);
      /*p26.AFEB*/ ppu_reg.MA01.set_tribuf(_BAFY_BG_MAP_READn, _MAP_X1S);
      /*p26.ALEL*/ ppu_reg.MA02.set_tribuf(_BAFY_BG_MAP_READn, _MAP_X2S);
      /*p26.COLY*/ ppu_reg.MA03.set_tribuf(_BAFY_BG_MAP_READn, _MAP_X3S);
      /*p26.AJAN*/ ppu_reg.MA04.set_tribuf(_BAFY_BG_MAP_READn, _MAP_X4S);
      /*p26.DUHO*/ ppu_reg.MA05.set_tribuf(_BAFY_BG_MAP_READn, _MAP_Y0S);
      /*p26.CASE*/ ppu_reg.MA06.set_tribuf(_BAFY_BG_MAP_READn, _MAP_Y1S);
      /*p26.CYPO*/ ppu_reg.MA07.set_tribuf(_BAFY_BG_MAP_READn, _MAP_Y2S);
      /*p26.CETA*/ ppu_reg.MA08.set_tribuf(_BAFY_BG_MAP_READn, _MAP_Y3S);
      /*p26.DAFE*/ ppu_reg.MA09.set_tribuf(_BAFY_BG_MAP_READn, _MAP_Y4S);
      /*p26.AMUV*/ ppu_reg.MA10.set_tribuf(_BAFY_BG_MAP_READn, cfg_reg.LCDC_BGMAP.q());
      /*p26.COVE*/ ppu_reg.MA11.set_tribuf(_BAFY_BG_MAP_READn, dbg_sig.VYPO_P10_Bn);
      /*p26.COXO*/ ppu_reg.MA12.set_tribuf(_BAFY_BG_MAP_READn, dbg_sig.VYPO_P10_Bn);
    }

    {
      // Window map read
      /*p25.XEZE*/ wire _XEZE_WIN_MAP_READ = and (_POTU_BGW_FETCH_01, win_sig.PORE_WIN_MODE);
      /*p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not(_XEZE_WIN_MAP_READ);
      /*p27.XEJA*/ ppu_reg.MA00.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X3.q());
      /*p27.XAMO*/ ppu_reg.MA01.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X4.q());
      /*p27.XAHE*/ ppu_reg.MA02.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X5.q());
      /*p27.XULO*/ ppu_reg.MA03.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X6.q());
      /*p27.WUJU*/ ppu_reg.MA04.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X7.q());
      /*p27.VYTO*/ ppu_reg.MA05.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y3.q());
      /*p27.VEHA*/ ppu_reg.MA06.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y4.q());
      /*p27.VACE*/ ppu_reg.MA07.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y5.q());
      /*p27.VOVO*/ ppu_reg.MA08.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y6.q());
      /*p27.VULO*/ ppu_reg.MA09.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y7.q());
      /*p27.VEVY*/ ppu_reg.MA10.set_tribuf(_WUKO_WIN_MAP_READn, cfg_reg.LCDC_WINMAP.q());
      /*p27.VEZA*/ ppu_reg.MA11.set_tribuf(_WUKO_WIN_MAP_READn, dbg_sig.VYPO_P10_Bn);
      /*p27.VOGU*/ ppu_reg.MA12.set_tribuf(_WUKO_WIN_MAP_READn, dbg_sig.VYPO_P10_Bn);
    }

    /*p27.XUHA*/ wire _XUHA_FETCH_S2 = not(NOFU_FETCH_S2n);
    /*p27.NETA*/ wire _NETA_TILE_READ = and (_LENA_BGW_VRAM_RD, _NENY_FETCH_01n);

    {
      // Background tile read low
      /*p26.ASUL*/ wire _ASUL_TILE_READ = and (_NETA_TILE_READ, _AXAD_WIN_MODEn);
      /*p26.BEJE*/ wire _BEJE_BG_TILE_READn = not(_ASUL_TILE_READ);
      /*p26.ASUM*/ ppu_reg.MA00.set_tribuf(!_BEJE_BG_TILE_READn, _XUHA_FETCH_S2);
      /*p26.EVAD*/ ppu_reg.MA01.set_tribuf(!_BEJE_BG_TILE_READn, _TILE_Y0S);
      /*p26.DAHU*/ ppu_reg.MA02.set_tribuf(!_BEJE_BG_TILE_READn, _TILE_Y1S);
      /*p26.DODE*/ ppu_reg.MA03.set_tribuf(!_BEJE_BG_TILE_READn, _TILE_Y2S);
    }

    {
      // Window tile read low
      /*p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand(_NETA_TILE_READ, win_sig.PORE_WIN_MODE);
      /*p25.XONU*/ ppu_reg.MA00.set_tribuf(!_XUCY_WIN_TILE_READn, _XUHA_FETCH_S2);
      /*p25.WUDO*/ ppu_reg.MA01.set_tribuf(!_XUCY_WIN_TILE_READn, win_reg.WIN_Y0.q());
      /*p25.WAWE*/ ppu_reg.MA02.set_tribuf(!_XUCY_WIN_TILE_READn, win_reg.WIN_Y1.q());
      /*p25.WOLU*/ ppu_reg.MA03.set_tribuf(!_XUCY_WIN_TILE_READn, win_reg.WIN_Y2.q());
    }

    {
      // Background/window tile read high
      /*p25.VUZA*/ wire _VUZA_TILE_BANK = nor(cfg_reg.LCDC_BGTILE.q(), pxp_reg.BG_PIX_B7.q()); // register reused
      /*p25.VAPY*/ ppu_reg.MA04.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B0.q()); // register reused
      /*p25.SEZU*/ ppu_reg.MA05.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B1.q());
      /*p25.VEJY*/ ppu_reg.MA06.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B2.q());
      /*p25.RUSA*/ ppu_reg.MA07.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B3.q());
      /*p25.ROHA*/ ppu_reg.MA08.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B4.q());
      /*p25.RESO*/ ppu_reg.MA09.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B5.q());
      /*p25.SUVO*/ ppu_reg.MA10.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B6.q());
      /*p25.TOBO*/ ppu_reg.MA11.set_tribuf(_NETA_TILE_READ, pxp_reg.BG_PIX_B7.q());
      /*p25.VURY*/ ppu_reg.MA12.set_tribuf(_NETA_TILE_READ, _VUZA_TILE_BANK);
    }
  }


  /*p29.ABON*/ wire _ABON_SPR_VRAM_RDp1 = not(ppu_sig.TEXY_SPRITE_READ);

  {
    // TS_LINE_* out of order
    // why is there a ! on REG_OAM_B0?

    /*p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not(cfg_reg.LCDC_SPSIZE.q());
    /*p29.WUKY*/ wire _WUKY_FLIP_Y = not(oam_reg.REG_OAM_A6.q());

    /*p29.WAGO*/ wire _WAGO = xor (_WUKY_FLIP_Y, sst_reg.WENU_TS_LINE_0);
    /*p29.GEJY*/ wire _GEJY_SPRITE_Y3 = amux2(_FUFO_LCDC_SPSIZEn, !oam_reg.REG_OAM_B0.q(), cfg_reg.LCDC_SPSIZE.q(), _WAGO);
    /*p29.XUQU*/ wire _XUQU_SPRITE_AB = not(!ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.CYVU*/ wire _CYVU_SPRITE_Y0 = xor (_WUKY_FLIP_Y, sst_reg.CUCU_TS_LINE_1);
    /*p29.BORE*/ wire _BORE_SPRITE_Y1 = xor (_WUKY_FLIP_Y, sst_reg.CUCA_TS_LINE_2);
    /*p29.BUVY*/ wire _BUVY_SPRITE_Y2 = xor (_WUKY_FLIP_Y, sst_reg.CEGA_TS_LINE_3);


    /*p29.ABEM*/ ppu_reg.MA00.set_tribuf(!_ABON_SPR_VRAM_RDp1, _XUQU_SPRITE_AB);
    /*p29.BAXE*/ ppu_reg.MA01.set_tribuf(!_ABON_SPR_VRAM_RDp1, _CYVU_SPRITE_Y0);
    /*p29.ARAS*/ ppu_reg.MA02.set_tribuf(!_ABON_SPR_VRAM_RDp1, _BORE_SPRITE_Y1);
    /*p29.AGAG*/ ppu_reg.MA03.set_tribuf(!_ABON_SPR_VRAM_RDp1, _BUVY_SPRITE_Y2);
    /*p29.FAMU*/ ppu_reg.MA04.set_tribuf(!_ABON_SPR_VRAM_RDp1, _GEJY_SPRITE_Y3);
    /*p29.FUGY*/ ppu_reg.MA05.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B1.q());
    /*p29.GAVO*/ ppu_reg.MA06.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B2.q());
    /*p29.WYGA*/ ppu_reg.MA07.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B3.q());
    /*p29.WUNE*/ ppu_reg.MA08.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B4.q());
    /*p29.GOTU*/ ppu_reg.MA09.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B5.q());
    /*p29.GEGU*/ ppu_reg.MA10.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B6.q());
    /*p29.XEHE*/ ppu_reg.MA11.set_tribuf(!_ABON_SPR_VRAM_RDp1, oam_reg.REG_OAM_B7.q());
    /*p29.DYSO*/ ppu_reg.MA12.set_tribuf(!_ABON_SPR_VRAM_RDp1, joy_pin.P10_B);   // sprites always in low half of tile store

    // VRAM pin handler
    /*p25.LEXE*/ vram_pins.MA00_AD.set(not(/*p25.MYFU*/ not(ppu_reg.MA00)));
    /*p25.LOZU*/ vram_pins.MA01_AD.set(not(/*p25.MASA*/ not(ppu_reg.MA01)));
    /*p25.LACA*/ vram_pins.MA02_AD.set(not(/*p25.MYRE*/ not(ppu_reg.MA02)));
    /*p25.LUVO*/ vram_pins.MA03_AD.set(not(/*p25.MAVU*/ not(ppu_reg.MA03)));
    /*p25.LOLY*/ vram_pins.MA04_AD.set(not(/*p25.MEPA*/ not(ppu_reg.MA04)));
    /*p25.LALO*/ vram_pins.MA05_AD.set(not(/*p25.MYSA*/ not(ppu_reg.MA05)));
    /*p25.LEFA*/ vram_pins.MA06_AD.set(not(/*p25.MEWY*/ not(ppu_reg.MA06)));
    /*p25.LUBY*/ vram_pins.MA07_AD.set(not(/*p25.MUME*/ not(ppu_reg.MA07)));
    /*p25.TUJY*/ vram_pins.MA08_AD.set(not(/*p25.VOVA*/ not(ppu_reg.MA08)));
    /*p25.TAGO*/ vram_pins.MA09_AD.set(not(/*p25.VODE*/ not(ppu_reg.MA09)));
    /*p25.NUVA*/ vram_pins.MA10_AD.set(not(/*p25.RUKY*/ not(ppu_reg.MA10)));
    /*p25.PEDU*/ vram_pins.MA11_AD.set(not(/*p25.RUMA*/ not(ppu_reg.MA11)));
    /*p25.PONY*/ vram_pins.MA12_AD.set(not(/*p25.REHO*/ not(ppu_reg.MA12)));
  }

  // How does DMA/PPU lock come into play here? Does it?
  // PPU locks via RENDERING_LATCH, dma by...? well it doesn't have to lock since it's just reading.
  // so really if the address is muxed right, we only need to block writes while vram's locked.
  // oh there's also some locking down in the pin driver...

  //----------

  /*p25.TAVY*/ wire _TAVY_MOE_Cn = not(vram_pins.MOE_C);
  /*p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand(adr_sig.SOSE_8000_9FFFp, cpu_pins.CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.SALE*/ wire _SALE_DBG_VRAM_RDb = mux2_p(_TAVY_MOE_Cn, _TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAM);

  {
    /*p25.RUVY*/ wire _RUVY_VRAM_WR  = not(_SALE_DBG_VRAM_RDb);

    /*p25.SAZO*/ wire _SAZO_VRAM_RD  = and(_RUVY_VRAM_WR, SERE_VRAM_RD);
    /*p25.RYJE*/ wire _RYJE_VRAM_RDn = not(_SAZO_VRAM_RD);
    /*p25.REVO*/ wire _REVO_VRAM_RDp  = not(_RYJE_VRAM_RDn);

    /*p25.ROCY*/ wire _ROCY_VBUS_TRISTATEn  = and(_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RAHU*/ wire _RAHU_VBUS_TRISTATEp = not(_ROCY_VBUS_TRISTATEn);
    /*p25.ROVE*/ wire _ROVE_VBUS_TRISTATEn  = not(_RAHU_VBUS_TRISTATEp);

    /*p25.RELA*/ wire _RELA_VRAM_DRIVE = or(_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RENA*/ wire _RENA_VRAM_DRIVEn = not(_RELA_VRAM_DRIVE);

    /*p25.TEME*/ ppu_reg.MD0.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D0);
    /*p25.TEWU*/ ppu_reg.MD1.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D1);
    /*p25.TYGO*/ ppu_reg.MD2.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D2);
    /*p25.SOTE*/ ppu_reg.MD3.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D3);
    /*p25.SEKE*/ ppu_reg.MD4.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D4);
    /*p25.RUJO*/ ppu_reg.MD5.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D5);
    /*p25.TOFA*/ ppu_reg.MD6.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D6);
    /*p25.SUZA*/ ppu_reg.MD7.set_tribuf(!_RAHU_VBUS_TRISTATEp, cpu_pins.D7);

    /*p25.REGE*/ vram_pins.MD0_A.set(not(/*p25.SEFA*/ and(ppu_reg.MD0, _ROVE_VBUS_TRISTATEn)));
    /*p25.RYKY*/ vram_pins.MD1_A.set(not(/*p25.SOGO*/ and(ppu_reg.MD1, _ROVE_VBUS_TRISTATEn)));
    /*p25.RAZO*/ vram_pins.MD2_A.set(not(/*p25.SEFU*/ and(ppu_reg.MD2, _ROVE_VBUS_TRISTATEn)));
    /*p25.RADA*/ vram_pins.MD3_A.set(not(/*p25.SUNA*/ and(ppu_reg.MD3, _ROVE_VBUS_TRISTATEn)));
    /*p25.RYRO*/ vram_pins.MD4_A.set(not(/*p25.SUMO*/ and(ppu_reg.MD4, _ROVE_VBUS_TRISTATEn)));
    /*p25.REVU*/ vram_pins.MD5_A.set(not(/*p25.SAZU*/ and(ppu_reg.MD5, _ROVE_VBUS_TRISTATEn)));
    /*p25.REKU*/ vram_pins.MD6_A.set(not(/*p25.SAMO*/ and(ppu_reg.MD6, _ROVE_VBUS_TRISTATEn)));
    /*p25.RYZE*/ vram_pins.MD7_A.set(not(/*p25.SUKE*/ and(ppu_reg.MD7, _ROVE_VBUS_TRISTATEn)));

    /*p25.ROFA*/ vram_pins.MD0_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD1_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD2_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD3_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD4_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD5_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD6_B.set(not(_RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ vram_pins.MD7_B.set(not(_RENA_VRAM_DRIVEn));

    /*p25.RODY*/ ppu_reg.MD0.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD0_C);
    /*p25.REBA*/ ppu_reg.MD1.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD1_C);
    /*p25.RYDO*/ ppu_reg.MD2.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD2_C);
    /*p25.REMO*/ ppu_reg.MD3.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD3_C);
    /*p25.ROCE*/ ppu_reg.MD4.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD4_C);
    /*p25.ROPU*/ ppu_reg.MD5.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD5_C);
    /*p25.RETA*/ ppu_reg.MD6.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD6_C);
    /*p25.RAKU*/ ppu_reg.MD7.set_tribuf(!_RENA_VRAM_DRIVEn, vram_pins.MD7_C);

    /*p25.RURA*/ vram_pins.MD0_D.set(not(/*p25.SYNU*/ or (ppu_reg.MD0, _RAHU_VBUS_TRISTATEp)));
    /*p25.RULY*/ vram_pins.MD1_D.set(not(/*p25.SYMA*/ or (ppu_reg.MD1, _RAHU_VBUS_TRISTATEp)));
    /*p25.RARE*/ vram_pins.MD2_D.set(not(/*p25.ROKO*/ or (ppu_reg.MD2, _RAHU_VBUS_TRISTATEp)));
    /*p25.RODU*/ vram_pins.MD3_D.set(not(/*p25.SYBU*/ or (ppu_reg.MD3, _RAHU_VBUS_TRISTATEp)));
    /*p25.RUBE*/ vram_pins.MD4_D.set(not(/*p25.SAKO*/ or (ppu_reg.MD4, _RAHU_VBUS_TRISTATEp)));
    /*p25.RUMU*/ vram_pins.MD5_D.set(not(/*p25.SEJY*/ or (ppu_reg.MD5, _RAHU_VBUS_TRISTATEp)));
    /*p25.RYTY*/ vram_pins.MD6_D.set(not(/*p25.SEDO*/ or (ppu_reg.MD6, _RAHU_VBUS_TRISTATEp)));
    /*p25.RADY*/ vram_pins.MD7_D.set(not(/*p25.SAWU*/ or (ppu_reg.MD7, _RAHU_VBUS_TRISTATEp)));
  }

  // Die trace
  // MYMA = not(LONY);
  // APAM = not(LUFA);
  // RACU = and(RYLU, RAWA, MYMA, APAM);

  // original
  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WR = and(adr_sig.SOSE_8000_9FFFp, cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p25.SUDO*/ wire _SUDO_MWR_Cn = not(vram_pins.MWR_C);
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WR = mux2_p(_SUDO_MWR_Cn, _TUJA_CPU_VRAM_WR, dbg_sig.TUTO_DBG_VRAM);
    /*p25.SOHY*/ wire _SOHY_MWR = nand(_TYJY_DBG_VRAM_WR, SERE_VRAM_RD);
    /*p25.RACO*/ wire _RACO_DBG_VRAMn = not(dbg_sig.TUTO_DBG_VRAM);
    /*p25.TAXY*/ wire _TAXY_MWR_An = and(_SOHY_MWR, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWR_Dn = or (_SOHY_MWR, dbg_sig.TUTO_DBG_VRAM);
    /*p25.RYLU*/ wire _RYLU_DBG_VRAM_RDn = nand(_SALE_DBG_VRAM_RDb, XANE_VRAM_LOCKn);
    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp2 = and(TACU_SPR_SEQ_5_TRIG, _ABON_SPR_VRAM_RDp1);
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn1 = not(_SOHO_SPR_VRAM_RDp2);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn2 = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q()); // this should be correct
    /*p25.RACU*/ wire _RACU_MOEn = and(_RYLU_DBG_VRAM_RDn, _RAWA_SPR_VRAM_RDn1, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn2); // def and
    /*p25.SUTU*/ wire _SUTU_READ_VRAMn = nor(_LENA_BGW_VRAM_RD, dma_sig.LUFA_DMA_READ_VRAMp, _ABON_SPR_VRAM_RDp1, SERE_VRAM_RD);
    /*p25.SEMA*/ wire _SEMA_MOE_An = and (_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOE_Dn = or (dbg_sig.TUTO_DBG_VRAM, _RACU_MOEn); // schematic wrong, second input is RACU
    /*p25.TODE*/ wire _TODE_MCS_An = and(_SUTU_READ_VRAMn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCS_Dn = or (_SUTU_READ_VRAMn, dbg_sig.TUTO_DBG_VRAM);

    /*p25.SYSY*/ vram_pins.MWRn_A.set(not(_TAXY_MWR_An));
    /*p25.RAGU*/ vram_pins.MWRn_D.set(not(_SOFY_MWR_Dn));
    /*p25.REFO*/ vram_pins.MOEn_A.set(not(_SEMA_MOE_An));
    /*p25.SAHA*/ vram_pins.MOEn_D.set(not(_RUTE_MOE_Dn));
    /*p25.SOKY*/ vram_pins.MCSn_A.set(not(_TODE_MCS_An));
    /*p25.SETY*/ vram_pins.MCSn_D.set(not(_SEWO_MCS_Dn));
    }
}