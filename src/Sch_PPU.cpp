#include "Sch_PPU.h"

#include "TestGB.h"

using namespace Schematics;

// Die trace
// NUNY04 = and(PYNU04, NOPA16)
// NYFO02 = not(NUNY04)
// MOSU03 = not(NYFO02) // 3 rung inverter
// ABAF02 = not(CATU17)

// BYHA = weirdgate
// BYHA01 << ANEL17
// BYHA02 << ABAF02
// BYHA03 xx CATU17 (crossover?)
// BYHA04 << ABEZ02
// BYHA05 >> ATEJ01

// ATEJ03 = not(BYHA05)
// ANOM03 = nor(ATEJ03, ATAR02)
// BALU02 = not(ANOM03)
// BEBU05 = or(DOBA17, BALU02, BYBA16)
// AVAP03 = not(BEBU05)
// NYXU04 = nor(AVAP03, MOSU03, TEVO05)

// Die trace:

// WUSA arms on the ground side, nor latch
// WUSA00 << XAJO03
// WUSA01 nc
// WUSA02 >> nc
// WUSA03 >> TOBA00
// WUSA04 nc 
// WUSA05 << WEGO03

// When XAJO03 goes high, WUSA03 goes high.
// When WEGO03 goes high, WUSA03 goes low.

// XEHO01 <> XEHO12
// XEHO02 << SACU04
// XEHO03 <> XEHO09
// XEHO04 nc
// XEHO05 nc
// XEHO06 << XYMU03
// XEHO07 << XEHO16 // so it's a counter
// XEHO08 nc
// XEHO09 <> XEHO03
// XEHO10 nc
// XEHO11 ?? RUMA01
// XEHO12 <> XEHO01
// XEHO13 << XYMU03
// XEHO14 nc
// XEHO15 nc
// XEHO16
// XEHO17 >> XAJO01

// ROXY = NOR latch
// ROXY00 << PAHA
// ROXY01 nc
// ROXY02 >> nc
// ROXY03 >> RONE00
// ROXY04 nc
// ROXY05 << POVA

// If PAHA goes high, ROXY03 goes high.
// If POVA goes high, ROXY03 goes low

// LURY01 << LOVY15 (next to bottom rung)
// LURY02 << XYMU02
// LURY03 nc
// LURY04 >> LONY01

// LONY has "arms" on the VCC side - different from the other latches?
// Probably a NAND latch instead of NOR
// LONY01 << LURY03
// LONY02 nc
// LONY03 == nc
// LONY04 >> LUSU01, MYMA01
// LONY05 nc
// LONY06 << NYXU03

// if LURY goes low, LONY goes low
// if NYXU goes low, LONY goes high

// XYMU has "arms" on the ground side
// XYMU00 << WEGO03
// XYMU01 nc
// XYMU02 >> bunch of stuff
// XYMU03 >> nc
// XYMU04 nc
// XYMU05 << AVAP02

// if AVAP02 goes high, XYMU02 goes high.
// if WEGO03 goes high, XYMU02 goes low.

// TAKA has arms on the VCC side - nand latch
// TAKA01 << VEKU02
// TAKA02 nc
// TAKA03 >> nc
// TAKA04 >> SOWO00
// TAKA05 nc
// TAKA06 << SECA03
// if SECA03 goes low, TAKA04 goes high
// if VEKU02 goes low, TAKA04 goes low

//------------------------------------------------------------------------------

PpuSignals PpuRegisters::sig(const TestGB& gb) const {
  PpuSignals ppu_sig;

  auto win_sig = gb.win_reg.sig(gb);

  auto sst_sig = gb.sst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto oam_sig = gb.oam_bus.sig();

  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto& vram_pins = gb.vram_pins;

  auto& tile_fetcher = gb.tile_fetcher;
  auto tile_fetcher_sig = tile_fetcher.sig(gb);

  auto sprite_scanner_sig = gb.sprite_scanner.sig(gb);

  auto& sprite_fetcher = gb.sprite_fetcher;
  auto sprite_fetcher_sig = sprite_fetcher.sig(gb);

  //----------

  ppu_sig.XYMU_RENDERINGp = XYMU_RENDERINGp;
  ppu_sig.XEHO_X0 = XEHO_X0;
  ppu_sig.SAVY_X1 = SAVY_X1;
  ppu_sig.XODU_X2 = XODU_X2;
  ppu_sig.XYDO_X3 = XYDO_X3;
  ppu_sig.TUHU_X4 = TUHU_X4;
  ppu_sig.TUKY_X5 = TUKY_X5;
  ppu_sig.TAKO_X6 = TAKO_X6;
  ppu_sig.SYBE_X7 = SYBE_X7;

  {
    /*p21.XUGU*/ wire _XUGU_X_167n = nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ wire _XANO_X_167 = not(_XUGU_X_167n);
    /*p21.XENA*/ wire _XENA_STORE_MATCHn = not(sst_sig.FEPO_STORE_MATCHp);
    /*p21.WODU*/ ppu_sig.WODU_RENDER_DONEp = and (_XENA_STORE_MATCHn, _XANO_X_167);
  }

  /*p27.TEVO*/ ppu_sig.TEVO_FINE_RSTp = nor(win_sig.SEKO_WIN_MATCH_TRIGp, win_sig.SUZU, tile_fetcher_sig.TAVE_PORCH_DONE_TRIGp);

  /*p27.NYXU*/ ppu_sig.NYXU_TILE_FETCHER_RSTn = nor(sprite_scanner_sig.AVAP_SCAN_DONE_TRIGp, win_sig.MOSU_WIN_MODE_TRIGp, ppu_sig.TEVO_FINE_RSTp);

  // so dma stops oam scan?
  /*p28.ACYL*/ ppu_sig.ACYL_SCANNINGp = and (dma_sig.BOGE_DMA_RUNNINGn, sprite_scanner_sig.BESU_SCANNINGp);

  {
#if 0
    /*p24.TYFA*/ wire TYFA_CLKPIPEp = and (win_sig.SOCY_WIN_HITn,
                                           tile_fetcher_sig.POKY_PORCH_DONEp,
                                           !sst_sig.FEPO_STORE_MATCHp,
                                           !ppu_sig.WODU_RENDER_DONEp,
                                           !clk_sig.MYVO_AxCxExGx);
#endif

    /*p24.VYBO*/ wire VYBO_PIX_CLK_xBxDxFxH = nor(sst_sig.FEPO_STORE_MATCHp, ppu_sig.WODU_RENDER_DONEp, clk_sig.MYVO_AxCxExGx);
    /*p24.TYFA*/ wire TYFA_CLKPIPEp_xBxDxFxH = and (win_sig.SOCY_WIN_HITn, tile_fetcher_sig.POKY_PORCH_DONEp, VYBO_PIX_CLK_xBxDxFxH);
    /*p24.SEGU*/ ppu_sig.SEGU_CLKPIPEn = not(TYFA_CLKPIPEp_xBxDxFxH);
    /*p24.SACU*/ ppu_sig.SACU_CLKPIPEp = nor(ppu_sig.SEGU_CLKPIPEn, ROXY_FINE_MATCH_LATCHn);
  }

  ///*p24.LOBY*/ ppu_sig.LOBY_RENDERINGn = not(XYMU_RENDERINGp.q());
  ///*p25.ROPY*/ ppu_sig.ROPY_RENDERINGn = not(XYMU_RENDERINGp.q());

  /*p27.ROZE*/ ppu_sig.ROZE_FINE_COUNT_STOPn = nand(RYKU_FINE_CNT0, ROGA_FINE_CNT1, RUBU_FINE_CNT2);

  {
    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RD = and (cpu_sig.SOSE_8000_9FFFp, dbg_sig.ABUZ);
    /*p25.TEFY*/ wire _TEFY_MCSn_Cn = not(vram_pins.PIN_MCSn_C);
    /*p25.TOLE*/ wire _TOLE_VRAM_RD = mux2_p(_TEFY_MCSn_Cn, _TUCA_CPU_VRAM_RD, dbg_sig.TUTO_DBG_VRAMp);
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp);
    /*p25.SERE*/ ppu_sig.SERE_VRAM_RD = and (_TOLE_VRAM_RD, ROPY_RENDERINGn);
  }

  {
    /*p21.TAPA*/ wire TAPA_INT_OAM = and (lcd_sig.TOLU_VBLANKn, lcd_sig.SELA_NEW_LINE_d0p);
    /*p21.TARU*/ wire TARU_INT_HBL = and (lcd_sig.TOLU_VBLANKn, ppu_sig.WODU_RENDER_DONEp);
    /*p21.SUKO*/ wire SUKO_INT_STATb = amux4(RUGU_INT_LYC_EN, lcd_sig.ROPO_LY_MATCH_SYNCp,
                                             REFE_INT_OAM_EN, TAPA_INT_OAM,
                                             RUFO_INT_VBL_EN, lcd_sig.PARU_VBLANKp, // polarity?
                                             ROXE_INT_HBL_EN, TARU_INT_HBL);
    /*p21.TUVA*/ wire TUVA_INT_STATn = not(SUKO_INT_STATb);
    /*p21.VOTY*/ ppu_sig.VOTY_INT_STATp = not(TUVA_INT_STATn);
  }

  {
    // This is the topmost "trigger sprite fetch" signal.
    /*p27.TEKY*/ ppu_sig.TEKY_SPRITE_FETCH = and (sst_sig.FEPO_STORE_MATCHp,
                                                  win_sig.TUKU_WIN_HITn,
                                                  tile_fetcher_sig.LYRY_BFETCH_DONEp,
                                                  sprite_fetcher_sig.SOWO_SFETCH_RUNNINGn);

    // And this is the topmost "reset sprite fetcher" signal
    /*p27.VEKU*/ ppu_sig.VEKU_SFETCH_RUNNING_RSTn = nor(sprite_fetcher_sig.WUTY_PIPE_LOAD_SPRITEp,
                                                        tile_fetcher_sig.TAVE_PORCH_DONE_TRIGp); // def nor
  }


  return ppu_sig;
}

//------------------------------------------------------------------------------

void PpuRegisters::tick(TestGB& gb) {
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto ppu_sig = sig(gb);
  auto oam_sig = gb.oam_bus.sig();

  auto& cpu_bus = gb.cpu_bus;
  auto& ppu_config = gb.ppu_config;
  
  auto tile_fetcher_sig = gb.tile_fetcher.sig(gb);

  auto sprite_scanner_sig = gb.sprite_scanner.sig(gb);

  //----------

  {
    /*p21.WEGO*/ wire _WEGO_RST_LATCH = or(rst_sig.TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);
    /*p21.XYMU*/ XYMU_RENDERINGp.nor_latch(sprite_scanner_sig.AVAP_SCAN_DONE_TRIGp, _WEGO_RST_LATCH);


    {
      /*p27.POVA*/ wire _POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_Ap, !NYZE_FINE_MATCH_Bp);
      // XAJO04 = and(XEHO17, XYDO17);
      /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
      /*p21.WUSA*/ WUSA_CPEN_LATCH.nor_latch(_XAJO_X_009, _WEGO_RST_LATCH);
      /*p21.TOBA*/ wire _TOBA = and (ppu_sig.SACU_CLKPIPEp, WUSA_CPEN_LATCH);
      /*p21.SEMU*/ wire _SEMU_LCD_CPn = or(_TOBA, _POVA_FINE_MATCH_SETp);
      /*p21.RYPO*/ wire _RYPO_LCD_CP = not(_SEMU_LCD_CPn);
      CP.set(_RYPO_LCD_CP);
    }

    {
      /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(ppu_sig.SEGU_CLKPIPEn);
      /*p27.PECU*/ wire PECU_FINE_CLK = nand(ROXO_CLKPIPEp, ppu_sig.ROZE_FINE_COUNT_STOPn);
      /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp);
      /*p27.PASO*/ wire PASO_FINE_RST = nor(ppu_sig.TEVO_FINE_RSTp, ROPY_RENDERINGn);
      /*p27.RYKU*/ RYKU_FINE_CNT0.set(PECU_FINE_CLK,   PASO_FINE_RST, !RYKU_FINE_CNT0);
      /*p27.ROGA*/ ROGA_FINE_CNT1.set(!RYKU_FINE_CNT0, PASO_FINE_RST, !ROGA_FINE_CNT1);
      /*p27.RUBU*/ RUBU_FINE_CNT2.set(!ROGA_FINE_CNT1, PASO_FINE_RST, !RUBU_FINE_CNT2);
    }

    {
      /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(ppu_sig.SEGU_CLKPIPEn);
      /*p27.MOXE*/ wire MOXE_AxCxExGx = not(clk_sig.ALET_xBxDxFxH);

      /*p27.SUHA*/ wire SUHA_FINE_MATCH0p = xnor(ppu_config.DATY_SCX0, RYKU_FINE_CNT0); // Arms on the ground side, XNOR
      /*p27.SYBY*/ wire SYBY_FINE_MATCH1p = xnor(ppu_config.DUZU_SCX1, ROGA_FINE_CNT1);
      /*p27.SOZU*/ wire SOZU_FINE_MATCH2p = xnor(ppu_config.CYXU_SCX2, RUBU_FINE_CNT2);
      /*p27.RONE*/ wire RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn, SUHA_FINE_MATCH0p, SYBY_FINE_MATCH1p, SOZU_FINE_MATCH2p);
      /*p27.POHU*/ wire POHU_FINE_MATCHp = not(RONE_FINE_MATCHn);
      /*p27.PUXA*/ PUXA_FINE_MATCH_Ap.set(ROXO_CLKPIPEp, XYMU_RENDERINGp, POHU_FINE_MATCHp);
      /*p27.NYZE*/ NYZE_FINE_MATCH_Bp.set(MOXE_AxCxExGx, XYMU_RENDERINGp, PUXA_FINE_MATCH_Ap);

      /*p27.POVA*/ wire POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_Ap, !NYZE_FINE_MATCH_Bp);
      /*p27.PAHA*/ wire PAHA_FINE_MATCH_RSTp = not(XYMU_RENDERINGp);
      /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(PAHA_FINE_MATCH_RSTp, POVA_FINE_MATCH_SETp);
    }
    {
      // LCD horizontal sync pin latch
      // if AVAP goes high, POFY goes high.
      // if PAHO or TOFU go high, POFY goes low.

      /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(ppu_sig.SEGU_CLKPIPEn);
      /*p24.PAHO*/ PAHO_X_8_SYNC.set(ROXO_CLKPIPEp, ppu_sig.XYMU_RENDERINGp, XYDO_X3);
      /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(sprite_scanner_sig.AVAP_SCAN_DONE_TRIGp, PAHO_X_8_SYNC || rst_sig.TOFU_VID_RSTp);
      /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY_ST_LATCH);
      ST.set(RUZE_PIN_ST);
    }
  }

  //----------

  {
    // vid x position, has carry lookahead because this increments every tcycle
    /*p21.RYBO*/ wire RYBO = xor(XEHO_X0, SAVY_X1);
    /*p21.XUKE*/ wire XUKE = and(XEHO_X0, SAVY_X1);

    /*p21.XYLE*/ wire XYLE = and(XODU_X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(XODU_X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(XYDO_X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(TUHU_X4, TUKY_X5);
    /*p21.TYBA*/ wire TYBA = and(TUHU_X4, TUKY_X5);
    /*p21.SURY*/ wire SURY = and(TAKO_X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(TAKO_X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(SYBE_X7, SURY);

    /*p21.TADY*/ wire TADY_X_RSTn = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);

    /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(XYDO_X3);

    /*p21.XEHO*/ XEHO_X0.set(ppu_sig.SACU_CLKPIPEp, TADY_X_RSTn, !XEHO_X0);
    /*p21.SAVY*/ SAVY_X1.set(ppu_sig.SACU_CLKPIPEp, TADY_X_RSTn, RYBO);
    /*p21.XODU*/ XODU_X2.set(ppu_sig.SACU_CLKPIPEp, TADY_X_RSTn, XEGY);
    /*p21.XYDO*/ XYDO_X3.set(ppu_sig.SACU_CLKPIPEp, TADY_X_RSTn, XORA);
    /*p21.TUHU*/ TUHU_X4.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, !TUHU_X4);
    /*p21.TUKY*/ TUKY_X5.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, SAKE);
    /*p21.TAKO*/ TAKO_X6.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, TYGE);
    /*p21.SYBE*/ SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, ROKU);
  }

  {
    /*p21.TADY*/ wire TADY_X_RST = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);
    // having this reset connected to both RENDER_DONE_SYNC and x seems odd
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(clk_sig.ALET_xBxDxFxH, TADY_X_RST, ppu_sig.WODU_RENDER_DONEp);
  }

  //----------------------------------------
  // Config registers

  // FF41 STAT
  {
    // stat read
    // hblank  = stat 0 = 00
    // vblank  = stat 1 = 01
    // oamscan = stat 2 = 10
    // render  = stat 3 = 11

    // RUPO arms on ground side, nor latch
    // RUPO00 << ROPO16
    // RUPO01 nc
    // RUPO02 >> SEGO03
    // RUPO03 >> nc
    // RUPO04 nc
    // RUPO05 << PAGO03

    // when PAGO03 goes high, RUPO02 goes high
    // when ROPO16 goes high, RUPO02 goes low.

    /*p22.WOFA*/ wire WOFA_FF41n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.WADO_A00p, cpu_sig.XENO_A01n, cpu_sig.XUSY_A02n, cpu_sig.XERA_A03n);
    /*p22.VARY*/ wire VARY_FF41p = not(WOFA_FF41n);

    /*p21.TOBE*/ wire TOBE_FF41_RDa = and (cpu_sig.ASOT_CPU_RD, VARY_FF41p); // die AND
    /*p21.VAVE*/ wire VAVE_FF41_RDb = not(TOBE_FF41_RDa); // die INV

    /*p21.SEPA*/ wire SEPA_FF41_WRp = and (cpu_sig.CUPA_CPU_WR_xxxxxFGH, VARY_FF41p);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(SEPA_FF41_WRp);

    /*p21.RYJU*/ wire RYJU_FF41_WRn = not(SEPA_FF41_WRp);
    /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(rst_sig.WESY_RSTn, RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ RUPO_LYC_MATCH_LATCHn.nor_latch(PAGO_LYC_MATCH_RST, lcd_sig.ROPO_LY_MATCH_SYNCp);

    /*p21.ROXE*/ ROXE_INT_HBL_EN.set(RYVE_FF41_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D0);
    /*p21.RUFO*/ RUFO_INT_VBL_EN.set(RYVE_FF41_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D1);
    /*p21.REFE*/ REFE_INT_OAM_EN.set(RYVE_FF41_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D2);
    /*p21.RUGU*/ RUGU_INT_LYC_EN.set(RYVE_FF41_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D3);

    /*p21.PARU*/ wire PARU_IN_VBLANK = not(!lcd_sig.POPU_VBLANK_d4);
    /*p21.XATY*/ wire XATY_STAT_MODE1n = nor(XYMU_RENDERINGp, ppu_sig.ACYL_SCANNINGp); // die NOR
    /*p21.SADU*/ wire SADU_STAT_MODE0n = nor(XYMU_RENDERINGp, PARU_IN_VBLANK); // die NOR

    // OK, these tribufs are _slightly_ different - compare SEGO and SASY, second rung.
    /*p21.TEBY*/ cpu_bus.TRI_D0.set_tribuf(TOBE_FF41_RDa, not(SADU_STAT_MODE0n));
    /*p21.WUGA*/ cpu_bus.TRI_D1.set_tribuf(TOBE_FF41_RDa, not(XATY_STAT_MODE1n));
    /*p21.SEGO*/ cpu_bus.TRI_D2.set_tribuf(TOBE_FF41_RDa, not(RUPO_LYC_MATCH_LATCHn.q()));
    /*p21.PUZO*/ cpu_bus.TRI_D3.set_tribuf(not(VAVE_FF41_RDb), ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ cpu_bus.TRI_D4.set_tribuf(not(VAVE_FF41_RDb), RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ cpu_bus.TRI_D5.set_tribuf(not(VAVE_FF41_RDb), REFE_INT_OAM_EN.q());
    /*p21.POTE*/ cpu_bus.TRI_D6.set_tribuf(not(VAVE_FF41_RDb), RUGU_INT_LYC_EN.q());
  }
}

//------------------------------------------------------------------------------

bool PpuRegisters::commit() {
  bool changed = false;
  /*p??.ROXY*/ changed |= ROXY_FINE_MATCH_LATCHn.commit_latch();
  /*p??.PUXA*/ changed |= PUXA_FINE_MATCH_Ap.commit_reg();
  /*p27.NYZE*/ changed |= NYZE_FINE_MATCH_Bp.commit_reg();
  /*p27.RYKU*/ changed |= RYKU_FINE_CNT0.commit_reg();
  /*p27.ROGA*/ changed |= ROGA_FINE_CNT1.commit_reg();
  /*p27.RUBU*/ changed |= RUBU_FINE_CNT2.commit_reg();
  /*p21.XEHO*/ changed |= XEHO_X0.commit_reg();
  /*p21.SAVY*/ changed |= SAVY_X1.commit_reg();
  /*p21.XODU*/ changed |= XODU_X2.commit_reg();
  /*p21.XYDO*/ changed |= XYDO_X3.commit_reg();
  /*p21.TUHU*/ changed |= TUHU_X4.commit_reg();
  /*p21.TUKY*/ changed |= TUKY_X5.commit_reg();
  /*p21.TAKO*/ changed |= TAKO_X6.commit_reg();
  /*p21.SYBE*/ changed |= SYBE_X7.commit_reg();
  /*p21.XYMU*/ changed |= XYMU_RENDERINGp.commit_latch();
  /*p21.VOGA*/ changed |= VOGA_RENDER_DONE_SYNC.commit_reg();

  /*p21.RUPO*/ changed |= RUPO_LYC_MATCH_LATCHn.commit_latch();


  /*p24.PAHO*/ changed |= PAHO_X_8_SYNC.commit_reg();
  /*p24.RUJU*/ changed |= POFY_ST_LATCH.commit_latch(); // nor latch with p24.RUJU, p24.POME
  /*p21.WUSA*/ changed |= WUSA_CPEN_LATCH.commit_latch();
  /* PIN_53 */ changed |= CP.commit_pinout();
  /* PIN_54 */ changed |= ST.commit_pinout();

  // FF41 - STAT
  /*p21.ROXE*/ changed |= ROXE_INT_HBL_EN.commit_reg();
  /*p21.RUFO*/ changed |= RUFO_INT_VBL_EN.commit_reg();
  /*p21.REFE*/ changed |= REFE_INT_OAM_EN.commit_reg();
  /*p21.RUGU*/ changed |= RUGU_INT_LYC_EN.commit_reg();

  return changed;
}

//------------------------------------------------------------------------------
