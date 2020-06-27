#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct PpuSignals {
  /*p27.TEVO*/ wire TEVO_CLK_STOPn;
  /*p21.WODU*/ wire WODU_RENDER_DONEp;
  /*p27.NYXU*/ wire NYXU_BFETCH_RSTn;
  /*p29.TEXY*/ wire TEXY_SPRITE_READp;
  /*p29.WUTY*/ wire WUTY_SPRITE_DONE;
  /*p28.ACYL*/ wire ACYL_PPU_USE_OAM1p;

  /*p24.POKY*/ wire POKY_AFTER_PORCH_LATCHp;
  /*p27.TAVE*/ wire TAVE_PORCH_ENDp;
  /*p21.XYMU*/ wire XYMU_RENDERINGp;
  /*p29.TEPA*/ wire TEPA_RENDERINGn;
  /*p24.LOBY*/ wire LOBY_RENDERINGn;
  /*p25.ROPY*/ wire ROPY_RENDERINGn;
  /*p24.SEGU*/ wire SEGU_xBxDxFxH;
  /*p24.ROXO*/ wire ROXO_AxCxExGx;
  /*p27.ROCO*/ wire ROCO_AxCxExGx;
  /*p25.SERE*/ wire SERE_VRAM_RD;
  /*p24.SACU*/ wire SACU_CLKPIPE_AxCxExGx;
  /*p27.ROZE*/ wire ROZE_FINE_COUNT_STOPn;
  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n;
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG;
  /*p27.NAKO*/ wire NAKO_FETCH_S1n;
  /*p27.NOFU*/ wire NOFU_FETCH_S2n;
  /*p21.VOTY*/ wire VOTY_INT_STATp;
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp;

};

//-----------------------------------------------------------------------------

struct PpuRegisters {

  PpuSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- VID_REG -----\n");

    text_painter.dprintf("PIX X      %d\n", pack(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), XYDO_X3.q(), TUHU_X4.q(), TUKY_X5.q(), TAKO_X6.q(), SYBE_X7.q()));
    //text_painter.dprintf("WIN MAP X  %d\n", pack(WIN_X3.q(), WIN_X4.q(), WIN_X5.q(), WIN_X6.q(), WIN_X7.q()));
    //text_painter.dprintf("WIN MAP Y  %d\n", pack(WIN_Y3.q(), WIN_Y4.q(), WIN_Y5.q(), WIN_Y6.q(), WIN_Y7.q()));
    //text_painter.dprintf("WIN TILE Y %d\n", pack(WIN_Y0.q(), WIN_Y1.q(), WIN_Y2.q()));
    text_painter.dprintf("FINE_CNT   %d\n", pack(RYKU_FINE_CNT0.q(), ROGA_FINE_CNT1.q(), RUBU_FINE_CNT2.q()));

    ROXY_FINE_MATCH_LATCHn.dump(text_painter, "ROXY_FINE_MATCH_LATCHn          ");
    PUXA_FINE_MATCH_SYNC1.dump(text_painter, "PUXA_FINE_MATCH_SYNC1         ");
    NYZE_FINE_MATCH_SYNC2.dump(text_painter, "NYZE_FINE_MATCH_SYNC2         ");
    XYMU_RENDERINGp.dump(text_painter, "RENDERING_LATCH     ");
    VOGA_RENDER_DONE_SYNC.dump(text_painter, "VOGA_RENDER_DONE_SYNC         ");
    ROXE_INT_HBL_EN.dump(text_painter, "ROXE_INT_HBL_EN               ");
    RUFO_INT_VBL_EN.dump(text_painter, "RUFO_INT_VBL_EN               ");
    REFE_INT_OAM_EN.dump(text_painter, "REFE_INT_OAM_EN               ");
    RUGU_INT_LYC_EN.dump(text_painter, "RUGU_INT_LYC_EN               ");
    LONY_BG_READ_VRAM_LATCHp.dump(text_painter, "BG_READ_VRAM_LATCH      ");
    LAXU_BFETCH_S0.dump(text_painter, "LAXU_BFETCH_S0          ");
    MESU_BFETCH_S1.dump(text_painter, "MESU_BFETCH_S1          ");
    NYVA_BFETCH_S2.dump(text_painter, "NYVA_BFETCH_S2          ");
    LOVY_BG_SEQ5_SYNC.dump(text_painter, "BG_SEQ5_SYNC             ");
    NYKA_BFETCH_DONE_SYNC.dump(text_painter, "NYKA_BFETCH_DONE_SYNC                 ");
    PORY_BFETCH_DONE_SYNC_DELAY.dump(text_painter, "PORY_BFETCH_DONE_SYNC_DELAY                 ");
    LYZU_BFETCH_S0_DELAY.dump(text_painter, "LYZU_BFETCH_S0_DELAY    ");
    PYGO_TILE_DONE.dump(text_painter, "PYGO_TILE_DONE           ");
    POKY_AFTER_PORCH_LATCHp.dump(text_painter, "POKY_AFTER_PORCH_LATCHp  ");
    TAKA_SFETCH_RUN_LATCH.dump(text_painter, "TAKA_SFETCH_RUN_LATCH      ");
    SOBU_SPRITE_FETCH_SYNC1.dump(text_painter, "SOBU_SPRITE_FETCH_SYNC1      ");
    SUDA_SPRITE_FETCH_SYNC2.dump(text_painter, "SUDA_SPRITE_FETCH_SYNC2      ");
    text_painter.newline();
  }

  // how the fck did i break all of these?
  /*p21.XEHO*/ Reg XEHO_X0;
  /*p21.SAVY*/ Reg SAVY_X1;
  /*p21.XODU*/ Reg XODU_X2;
  /*p21.XYDO*/ Reg XYDO_X3;
  /*p21.TUHU*/ Reg TUHU_X4;
  /*p21.TUKY*/ Reg TUKY_X5;
  /*p21.TAKO*/ Reg TAKO_X6;
  /*p21.SYBE*/ Reg SYBE_X7;

  /*p27.RYKU*/ Reg RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg RUBU_FINE_CNT2;

  /*p24.PAHO*/ Reg PAHO_X_8_SYNC;
  /*p24.RUJU*/ NorLatch POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME
  /* PIN_54 */ PinOut ST;
  /*p21.WUSA*/ NorLatch WUSA_CPEN_LATCH;
  /* PIN_53 */ PinOut CP;

  /*p??.ROXY*/ NorLatch ROXY_FINE_MATCH_LATCHn;
  /*p??.PUXA*/ Reg   PUXA_FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg   NYZE_FINE_MATCH_SYNC2;

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.VOGA*/ Reg VOGA_RENDER_DONE_SYNC;

  /*p21.ROXE*/ Reg ROXE_INT_HBL_EN;
  /*p21.RUFO*/ Reg RUFO_INT_VBL_EN;
  /*p21.REFE*/ Reg REFE_INT_OAM_EN;
  /*p21.RUGU*/ Reg RUGU_INT_LYC_EN;

  /*p27.LONY*/ NandLatch LONY_BG_READ_VRAM_LATCHp;

  /*p27.LAXU*/ Reg LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg NYVA_BFETCH_S2;

  /*p27.LOVY*/ Reg LOVY_BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg NYKA_BFETCH_DONE_SYNC;
  /*p24.PORY*/ Reg PORY_BFETCH_DONE_SYNC_DELAY;
  /*p27.LYZU*/ Reg LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg PYGO_TILE_DONE;
  /*p24.POKY*/ NorLatch POKY_AFTER_PORCH_LATCHp;

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUN_LATCH;
  /*p27.SOBU*/ Reg SOBU_SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg SUDA_SPRITE_FETCH_SYNC2;

  /*p21.RUPO*/ NorLatch RUPO_LYC_MATCH_LATCHn;

#if 0
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_REG -----\n");

    TOXE_SFETCH_S0.dump(text_painter, "TOXE_SFETCH_S0    ");
    TULY_SFETCH_S1.dump(text_painter, "TULY_SFETCH_S1    ");
    TESE_SFETCH_S2.dump(text_painter, "TESE_SFETCH_S2    ");
    TOBU_SFETCH_S1_D2.dump(text_painter, "TOBU_SFETCH_S1_D2  ");
    VONU_SFETCH_S1_D4.dump(text_painter, "VONU_SFETCH_S1_D4 ");
    SEBA_SFETCH_S1_D5.dump(text_painter, "SEBA_SFETCH_S1_D5 ");
    TYFO_SFETCH_S0_D1.dump(text_painter, "TYFO_SFETCH_S0_D1     ");
    //CENO_SCANNINGp.dump(text_painter, "CENO_SCANNINGp");

    //text_painter.dprintf("SCAN    %d\n", scan());
    //SCAN_DONE_d4.dump(text_painter, "SCAN_DONE_d4 ");
    //SCAN_DONE_d5.dump(text_painter, "SCAN_DONE_d5 ");

    //text_painter.dprintf("SPR_IDX %d\n", spr_idx());
    //text_painter.dprintf("TS_IDX  %d\n", ts_idx());
    //text_painter.dprintf("TS_LINE %d\n", ts_line());
    text_painter.newline();
  }
#endif

  /*p29.TOXE*/ Reg TOXE_SFETCH_S0;
  /*p29.TULY*/ Reg TULY_SFETCH_S1;
  /*p29.TESE*/ Reg TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg SEBA_SFETCH_S1_D5;
  /*p29.TYFO*/ Reg TYFO_SFETCH_S0_D1;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics