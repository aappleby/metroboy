#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct PpuSignals {
  /*p27.TEVO*/ bool TEVO_CLK_STOPn;
  /*p21.WODU*/ bool WODU_RENDER_DONEp;
  /*p27.NYXU*/ bool NYXU_BFETCH_RSTn;
  /*p29.TEXY*/ bool TEXY_SPRITE_READp;
  /*p29.WUTY*/ bool WUTY_SPRITE_DONE;
  /*p28.ACYL*/ bool ACYL_PPU_USE_OAM1p;

  /*p24.POKY*/ bool POKY_AFTER_PORCH_LATCHp;
  /*p27.TAVE*/ bool TAVE_PORCH_ENDp;
  /*p21.XYMU*/ bool XYMU_RENDERINGp;
  /*p29.TEPA*/ bool TEPA_RENDERINGn;
  /*p24.LOBY*/ bool LOBY_RENDERINGn;
  /*p25.ROPY*/ bool ROPY_RENDERINGn;
  /*p24.SEGU*/ bool SEGU_xBxDxFxH;
  /*p24.ROXO*/ bool ROXO_AxCxExGx;
  /*p27.ROCO*/ bool ROCO_AxCxExGx;
  /*p25.SERE*/ bool SERE_VRAM_RD; // this signal shouldn't be in the ppu
  /*p24.SACU*/ bool SACU_CLKPIPE_AxCxExGx;
  /*p27.ROZE*/ bool ROZE_FINE_COUNT_STOPn;
  /*p29.TYTU*/ bool TYTU_SFETCH_S0_D0n;
  /*p29.TACU*/ bool TACU_SPR_SEQ_5_TRIG;
  /*p27.NAKO*/ bool NAKO_FETCH_S1n;
  /*p27.NOFU*/ bool NOFU_FETCH_S2n;
  /*p21.VOTY*/ bool VOTY_INT_STATp;
  /*p29.TUVO*/ bool TUVO_PPU_OAM_RDp;

  /*p27.MOFU*/ bool MOFU_LATCH_BGPIXB;
  /*p27.NYDY*/ bool NYDY_LATCH_BGPIXA;
  /*p29.TOPU*/ bool TOPU_LATCH_SPPIXA;
  /*p29.RACA*/ bool RACA_LATCH_SPPIXB;
  /*p29.XONO*/ bool XONO_FLIP_X;

  /*p21.XEHO*/ bool XEHO_X0;
  /*p21.SAVY*/ bool SAVY_X1;
  /*p21.XODU*/ bool XODU_X2;
  /*p21.XYDO*/ bool XYDO_X3;
  /*p21.TUHU*/ bool TUHU_X4;
  /*p21.TUKY*/ bool TUKY_X5;
  /*p21.TAKO*/ bool TAKO_X6;
  /*p21.SYBE*/ bool SYBE_X7;

  /*p23.VYXE*/ bool VYXE_LCDC_BGEN;
  /*p23.XYLO*/ bool XYLO_LCDC_SPEN;
  /*p23.XYMO*/ bool XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ bool XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ bool WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ bool WYMO_LCDC_WINEN;
  /*p23.WOKY*/ bool WOKY_LCDC_WINMAP;
  /*p23.XONA*/ bool XONA_LCDC_EN;
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

  /*
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- PPU CFG -----\n");
    dump_long(text_painter, "VYXE_LCDC_BGEN   ", VYXE_LCDC_BGEN.a  );
    dump_long(text_painter, "XYLO_LCDC_SPEN   ", XYLO_LCDC_SPEN.a  );   
    dump_long(text_painter, "XYMO_LCDC_SPSIZE ", XYMO_LCDC_SPSIZE.a);
    dump_long(text_painter, "XAFO_LCDC_BGMAP  ", XAFO_LCDC_BGMAP.a );
    dump_long(text_painter, "WEXU_LCDC_BGTILE ", WEXU_LCDC_BGTILE.a);
    dump_long(text_painter, "WYMO_LCDC_WINEN  ", WYMO_LCDC_WINEN.a );
    dump_long(text_painter, "WOKY_LCDC_WINMAP ", WOKY_LCDC_WINMAP.a);
    dump_long(text_painter, "XONA_LCDC_EN     ", XONA_LCDC_EN.a    );

    //dump(text_painter,      "LYC         ", LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
    //dump(text_painter,      "BGP         ", BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
    //dump(text_painter,      "OBP0        ", OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
    //dump(text_painter,      "OBP1        ", OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);

    text_painter.newline();
  }

  int get_lcdc() const {
    return pack(VYXE_LCDC_BGEN.q(),
      XYLO_LCDC_SPEN.q(),
      XYMO_LCDC_SPSIZE.q(),
      XAFO_LCDC_BGMAP.q(),
      WEXU_LCDC_BGTILE.q(),
      WYMO_LCDC_WINEN.q(),
      WOKY_LCDC_WINMAP.q(),
      XONA_LCDC_EN.q());
  }
  */

  // FF40 - LCDC
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  // FF41 - STAT
  /*p21.ROXE*/ Reg ROXE_INT_HBL_EN;
  /*p21.RUFO*/ Reg RUFO_INT_VBL_EN;
  /*p21.REFE*/ Reg REFE_INT_OAM_EN;
  /*p21.RUGU*/ Reg RUGU_INT_LYC_EN;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics