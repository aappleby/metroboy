#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct PpuRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- VID_REG -----\n");

    text_painter.dprintf("PIX X      %d\n", pack(SAXO_X0.q(), TYPO_X1.q(), VYZO_X2.q(), TELU_X3.q(), SUDE_X4.q(), TAHA_X5.q(), TYRY_X6.q(), SYBE_X7.q()));
    //text_painter.dprintf("WIN MAP X  %d\n", pack(WIN_X3.q(), WIN_X4.q(), WIN_X5.q(), WIN_X6.q(), WIN_X7.q()));
    //text_painter.dprintf("WIN MAP Y  %d\n", pack(WIN_Y3.q(), WIN_Y4.q(), WIN_Y5.q(), WIN_Y6.q(), WIN_Y7.q()));
    //text_painter.dprintf("WIN TILE Y %d\n", pack(WIN_Y0.q(), WIN_Y1.q(), WIN_Y2.q()));
    text_painter.dprintf("FINE_CNT   %d\n", pack(RYKU_FINE_CNT0.q(), ROGA_FINE_CNT1.q(), RUBU_FINE_CNT2.q()));

    text_painter.dprintf("MAxx     0x%04x\n", pack(MA00.q(), MA01.q(), MA02.q(), MA03.q(), MA04.q(), MA05.q(), MA06.q(), MA07.q(), MA08.q(), MA09.q(), MA10.q(), MA11.q(), MA12.q()));
    text_painter.dprintf("MDxx     0x%02x\n", pack(MD0.q(), MD1.q(), MD2.q(), MD3.q(), MD4.q(), MD5.q(), MD6.q(), MD7.q()));

    ROXY_FINE_MATCH_LATCHn.dump(text_painter, "ROXY_FINE_MATCH_LATCHn          ");
    PUXA_FINE_MATCH_SYNC1.dump(text_painter, "PUXA_FINE_MATCH_SYNC1         ");
    NYZE_FINE_MATCH_SYNC2.dump(text_painter, "NYZE_FINE_MATCH_SYNC2         ");
    //NOPA_WIN_MODE_SYNC.dump(text_painter, "NOPA_WIN_MODE_SYNC       ");
    //SOVY_WIN_HIT_SYNC.dump(text_painter, "SOVY_WIN_HIT_SYNC       ");
    XYMU_RENDERINGp.dump(text_painter, "RENDERING_LATCH     ");
    VOGA_RENDER_DONE_SYNC.dump(text_painter, "VOGA_RENDER_DONE_SYNC         ");
    //PYNU_WIN_MODE_LATCH.dump(text_painter, "PYNU_WIN_MODE_LATCH      ");
    //RYDY_WIN_HIT_LATCH.dump(text_painter, "RYDY_WIN_HIT_LATCH      ");
    ROXE_INT_HBL_EN.dump(text_painter, "ROXE_INT_HBL_EN               ");
    RUFO_INT_VBL_EN.dump(text_painter, "RUFO_INT_VBL_EN               ");
    REFE_INT_OAM_EN.dump(text_painter, "REFE_INT_OAM_EN               ");
    RUGU_INT_LYC_EN.dump(text_painter, "RUGU_INT_LYC_EN               ");
    //LY_MATCH_SYNC      .dump(text_painter, "LY_MATCH_SYNC       ");
    //LYC_MATCH_LATCHn    .dump(text_painter, "LYC_MATCH_LATCHn     ");
    //SARY_WIN_MATCH_Y_SYNC.dump(text_painter, "SARY_WIN_MATCH_Y_SYNC            ");
    //RYFA_WIN_MATCH_ONSCREEN_SYNC1.dump(text_painter, "RYFA_WIN_MATCH_ONSCREEN_SYNC1 ");
    //RENE_WIN_MATCH_ONSCREEN_SYNC2.dump(text_painter, "RENE_WIN_MATCH_ONSCREEN_SYNC2 ");
    //PYCO_WIN_MATCH_SYNC1.dump(text_painter, "PYCO_WIN_MATCH_SYNC1          ");
    //NUNU_WIN_MATCH_SYNC2.dump(text_painter, "NUNU_WIN_MATCH_SYNC2          ");
    LONY_BG_READ_VRAM_LATCHp.dump(text_painter, "BG_READ_VRAM_LATCH      ");
    LAXU_BFETCH_S0.dump(text_painter, "LAXU_BFETCH_S0          ");
    MESU_BFETCH_S1.dump(text_painter, "MESU_BFETCH_S1          ");
    NYVA_BFETCH_S2.dump(text_painter, "NYVA_BFETCH_S2          ");
    LOVY_BG_SEQ5_SYNC.dump(text_painter, "BG_SEQ5_SYNC             ");
    NYKA_BFETCH_DONE_SYNC.dump(text_painter, "NYKA_BFETCH_DONE_SYNC                 ");
    PORY_BFETCH_DONE_SYNC_DELAY.dump(text_painter, "PORY_BFETCH_DONE_SYNC_DELAY                 ");
    LYZU_BFETCH_S0_DELAY.dump(text_painter, "LYZU_BFETCH_S0_DELAY    ");
    PYGO_TILE_DONE.dump(text_painter, "PYGO_TILE_DONE           ");
    POKY_FRONT_PORCH_LATCH.dump(text_painter, "POKY_FRONT_PORCH_LATCH  ");
    //REJO_WY_MATCH_LATCH.dump(text_painter, "REJO_WY_MATCH_LATCH           ");
    TAKA_SFETCH_RUN_LATCH.dump(text_painter, "TAKA_SFETCH_RUN_LATCH      ");
    SOBU_SPRITE_FETCH_SYNC1.dump(text_painter, "SOBU_SPRITE_FETCH_SYNC1      ");
    SUDA_SPRITE_FETCH_SYNC2.dump(text_painter, "SUDA_SPRITE_FETCH_SYNC2      ");
    text_painter.newline();
  }

  /*p21.XEHO*/ Reg SAXO_X0;
  /*p21.SAVY*/ Reg TYPO_X1;
  /*p21.XODU*/ Reg VYZO_X2;
  /*p21.XYDO*/ Reg TELU_X3;
  /*p21.TUHU*/ Reg SUDE_X4;
  /*p21.TUKY*/ Reg TAHA_X5;
  /*p21.TAKO*/ Reg TYRY_X6;
  /*p21.SYBE*/ Reg SYBE_X7;

  /*p27.RYKU*/ Reg RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg RUBU_FINE_CNT2;

  Tribuf MA00;
  Tribuf MA01;
  Tribuf MA02;
  Tribuf MA03;
  Tribuf MA04;
  Tribuf MA05;
  Tribuf MA06;
  Tribuf MA07;
  Tribuf MA08;
  Tribuf MA09;
  Tribuf MA10;
  Tribuf MA11;
  Tribuf MA12;

  Tribuf MD0;
  Tribuf MD1;
  Tribuf MD2;
  Tribuf MD3;
  Tribuf MD4;
  Tribuf MD5;
  Tribuf MD6;
  Tribuf MD7;

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
  /*p24.POKY*/ NorLatch POKY_FRONT_PORCH_LATCH;

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUN_LATCH;
  /*p27.SOBU*/ Reg SOBU_SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg SUDA_SPRITE_FETCH_SYNC2;

#if 0
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_REG -----\n");

    TOXE_SFETCH_S0_D0.dump(text_painter, "TOXE_SFETCH_S0_D0    ");
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

  /*p29.TOXE*/ Reg TOXE_SFETCH_S0_D0;
  /*p29.TULY*/ Reg TULY_SFETCH_S1;
  /*p29.TESE*/ Reg TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg SEBA_SFETCH_S1_D5;
  /*p29.TYFO*/ Reg TYFO_SFETCH_S0_D1;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics