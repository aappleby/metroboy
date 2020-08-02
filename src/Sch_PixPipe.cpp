#include "Sch_PixPipe.h"

#include "Sch_Top.h"

using namespace Schematics;

void PixelPipe::dump(Dumper& d, const SchematicTop& top) const {
  d("---------- Pix Pipe ----------\n");

  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor(top.sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), top.clk_reg.MYVO_AxCxExGx());
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not(TOMU_WIN_HITp());
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and (_SOCY_WIN_HITn, top.tile_fetcher.POKY_PORCH_DONEp(), _VYBO_PIX_CLK_xBxDxFxH);
  /*p24.SEGU*/ wire _SEGU_CLKPIPEn = not(_TYFA_CLKPIPEp_xBxDxFxH);
  /*p24.SACU*/ wire _SACU_CLKPIPEp = nor(_SEGU_CLKPIPEn, _ROXY_FINE_SCROLL_DONEn);

  d("_VYBO_PIX_CLK_xBxDxFxH  %d\n", _VYBO_PIX_CLK_xBxDxFxH);
  d("_SOCY_WIN_HITn          %d\n", _SOCY_WIN_HITn);
  d("_TYFA_CLKPIPEp_xBxDxFxH %d\n", _TYFA_CLKPIPEp_xBxDxFxH);
  d("_SEGU_CLKPIPEn          %d\n", _SEGU_CLKPIPEn);
  d("_SACU_CLKPIPEp          %d\n", _SACU_CLKPIPEp);

  /*p21.TADY*/ wire _TADY_LINE_START_RST = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());
  d("BYHA_VID_LINE_TRIGn     %d\n", top.lcd_reg.BYHA_VID_LINE_TRIGn());
  d("TOFU_VID_RSTp           %d\n", top.clk_reg.TOFU_VID_RSTp());
  d("_TADY_LINE_START_RST    %d\n", _TADY_LINE_START_RST);


  d("PIX COUNT   %03d\n", pack(
    XEHO_X0.q(),
    SAVY_X1.q(),
    XODU_X2.q(),
    XYDO_X3.q(),
    TUHU_X4.q(),
    TUKY_X5.q(),
    TAKO_X6.q(),
    SYBE_X7.q()
  ));

  d("LCDC_BGEN   %c\n", VYXE_LCDC_BGEN  .c());
  d("LCDC_SPEN   %c\n", XYLO_LCDC_SPEN  .c());
  d("LCDC_SPSIZE %c\n", XYMO_LCDC_SPSIZE.c());
  d("LCDC_BGMAP  %c\n", XAFO_LCDC_BGMAP .c());
  d("LCDC_BGTILE %c\n", WEXU_LCDC_BGTILE.c());
  d("LCDC_WINEN  %c\n", WYMO_LCDC_WINEN .c());
  d("LCDC_WINMAP %c\n", WOKY_LCDC_WINMAP.c());
  d("LCDC_EN     %c\n", XONA_LCDC_EN    .c());

  d("INT_HBL_EN  %c\n", ROXE_INT_HBL_EN.c());
  d("INT_VBL_EN  %c\n", RUFO_INT_VBL_EN.c());
  d("INT_OAM_EN  %c\n", REFE_INT_OAM_EN.c());
  d("INT_LYC_EN  %c\n", RUGU_INT_LYC_EN.c());

  d("ROXY_FINE_SCROLL_DONEn %c\n", _ROXY_FINE_SCROLL_DONEn.c());
  d("RYKU_FINE_CNT0         %c\n", _RYKU_FINE_CNT0        .c());
  d("ROGA_FINE_CNT1         %c\n", _ROGA_FINE_CNT1        .c());
  d("RUBU_FINE_CNT2         %c\n", _RUBU_FINE_CNT2        .c());
  d("XYMU_RENDERINGp        %c\n", _XYMU_RENDERINGp       .c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", _RUPO_LYC_MATCH_LATCHn .c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", _WUSA_LCD_CLOCK_GATE   .c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", _VOGA_RENDER_DONE_SYNC .c());
  d("PUXA_FINE_MATCH_A      %c\n", _PUXA_FINE_MATCH_A     .c());
  d("NYZE_FINE_MATCH_B      %c\n", _NYZE_FINE_MATCH_B     .c());
  d("PAHO_X_8_SYNC          %c\n", _PAHO_X_8_SYNC         .c());
  d("POFY_ST_LATCH          %c\n", _POFY_ST_LATCH         .c());
  d("LCD_PIN_CP             %c\n", _LCD_PIN_CP            .c());
  d("LCD_PIN_ST             %c\n", _LCD_PIN_ST            .c());
  d("LCD_PIN_LD1            %c\n", _LCD_PIN_LD1           .c());
  d("LCD_PIN_LD0            %c\n", _LCD_PIN_LD0           .c());

  d("BG_PIPE_A 0x%02x %c%c%c%c%c%c%c%c\n", 
    pack(BG_PIPE_A0.q(), BG_PIPE_A1.q(), BG_PIPE_A2.q(), BG_PIPE_A3.q(),
         BG_PIPE_A4.q(), BG_PIPE_A5.q(), BG_PIPE_A6.q(), BG_PIPE_A7.q()),
    BG_PIPE_A0.c(), BG_PIPE_A1.c(), BG_PIPE_A2.c(), BG_PIPE_A3.c(),
    BG_PIPE_A4.c(), BG_PIPE_A5.c(), BG_PIPE_A6.c(), BG_PIPE_A7.c());

  d("BG_PIPE_B 0x%02x %c%c%c%c%c%c%c%c\n", 
    pack(BG_PIPE_B0.q(), BG_PIPE_B1.q(), BG_PIPE_B2.q(), BG_PIPE_B3.q(),
         BG_PIPE_B4.q(), BG_PIPE_B5.q(), BG_PIPE_B6.q(), BG_PIPE_B7.q()),
    BG_PIPE_B0.c(), BG_PIPE_B1.c(), BG_PIPE_B2.c(), BG_PIPE_B3.c(),
    BG_PIPE_B4.c(), BG_PIPE_B5.c(), BG_PIPE_B6.c(), BG_PIPE_B7.c());

  d("SPR_PIPE_A %c%c%c%c%c%c%c%c\n", 
    SPR_PIPE_A0.c(), SPR_PIPE_A1.c(), SPR_PIPE_A2.c(), SPR_PIPE_A3.c(),
    SPR_PIPE_A4.c(), SPR_PIPE_A5.c(), SPR_PIPE_A6.c(), SPR_PIPE_A7.c());

  d("SPR_PIPE_B %c%c%c%c%c%c%c%c\n", 
    SPR_PIPE_B0.c(), SPR_PIPE_B1.c(), SPR_PIPE_B2.c(), SPR_PIPE_B3.c(),
    SPR_PIPE_B4.c(), SPR_PIPE_B5.c(), SPR_PIPE_B6.c(), SPR_PIPE_B7.c());

  d("PAL_PIPE   %c%c%c%c%c%c%c%c\n", 
    PAL_PIPE_0.c(), PAL_PIPE_1.c(), PAL_PIPE_2.c(), PAL_PIPE_3.c(), 
    PAL_PIPE_4.c(), PAL_PIPE_5.c(), PAL_PIPE_6.c(), PAL_PIPE_7.c());

  d("MASK_PIPE  %c%c%c%c%c%c%c%c\n", 
    MASK_PIPE_0.c(), MASK_PIPE_1.c(), MASK_PIPE_2.c(), MASK_PIPE_3.c(),
    MASK_PIPE_4.c(), MASK_PIPE_5.c(), MASK_PIPE_6.c(), MASK_PIPE_7.c());

  d("BGP  %c%c %c%c %c%c %c%c\n",
    _MENA_BGP7.c(), _MOGY_BGP6.c(), _MORU_BGP5.c(), _MUKE_BGP4.c(),
    _MAXY_BGP3.c(), _PYLU_BGP2.c(), _NUSY_BGP1.c(), _PAVO_BGP0.c());

  d("OBP0 %c%c %c%c %c%c %c%c\n",
    _XANA_OBP07.c(), _XUPO_OBP06.c(), _XYZE_OBP05.c(), _XERU_OBP04.c(),
    _XALO_OBP03.c(), _XOVA_OBP02.c(), _XUKY_OBP01.c(), _XUFU_OBP00.c());

  d("OBP1 %c%c %c%c %c%c %c%c\n",
    _LUXO_OBP17.c(), _LEPU_OBP16.c(), _LUGU_OBP15.c(), _LUNE_OBP14.c(),
    _LOSE_OBP13.c(), _MOSA_OBP12.c(), _LAWO_OBP11.c(), _MOXY_OBP10.c());

  d("WY %03d\n",
    pack(_NAFU_WY7.q(), _NUKA_WY6.q(), _NENE_WY5.q(), _NULO_WY4.q(),
        _MELA_WY3.q(), _NAGA_WY2.q(), _NYRO_WY1.q(), _NESO_WY0.q()));

  d("WX %03d\n",
    pack(_NUKU_WX7.q(), _MUVO_WX6.q(), _MYCE_WX5.q(), _MYPU_WX4.q(),
          _MEBY_WX3.q(), _NOKE_WX2.q(), _NOFE_WX1.q(), _MYPA_WX0.q()));

  d("\n");


  d("----------  Window  ----------\n");
  int wx = pack(
    _MYPA_WX0.q(),
    _NOFE_WX1.q(),
    _NOKE_WX2.q(),
    _MEBY_WX3.q(),
    _MYPU_WX4.q(),
    _MYCE_WX5.q(),
    _MUVO_WX6.q(),
    _NUKU_WX7.q());
    
  int wy = pack(
    _NESO_WY0.q(),
    _NYRO_WY1.q(),
    _NAGA_WY2.q(),
    _MELA_WY3.q(),
    _NULO_WY4.q(),
    _NENE_WY5.q(),
    _NUKA_WY6.q(),
    _NAFU_WY7.q()
  );

  int win_x = pack(
    0,
    0,
    0,
    _WYKA_WIN_X3.q(),
    _WODY_WIN_X4.q(),
    _WOBO_WIN_X5.q(),
    _WYKO_WIN_X6.q(),
    _XOLO_WIN_X7.q()
  );

  int win_y = pack(
    _VYNO_WIN_Y0.q(),
    _VUJO_WIN_Y1.q(),
    _VYMU_WIN_Y2.q(),
    _TUFU_WIN_Y3.q(),
    _TAXA_WIN_Y4.q(),
    _TOZO_WIN_Y5.q(),
    _TATE_WIN_Y6.q(),
    _TEKE_WIN_Y7.q()
  );

  d("WX               : %d\n", wx);
  d("WY               : %d\n", wy);
  d("WIN X            : %d\n", win_x);
  d("WIN_Y            : %d\n", win_y);
  d("NUKO_WX_MATCHp   : %d\n", wire(NUKO_WX_MATCHp));
  d("ROGE_WY_MATCHp   : %d\n", wire(ROGE_WY_MATCHp));

  d("WIN_MODE_A       : %c\n", _PYNU_WIN_MODE_A      .c());
  d("WIN_FIRST_TILE_A : %c\n", _RYDY_WIN_FIRST_TILE_A.c());
  d("WIN_MODE_B       : %c\n", _NOPA_WIN_MODE_B      .c());
  d("WIN_FIRST_TILE_B : %c\n", _SOVY_WIN_FIRST_TILE_B.c());
  d("WY_MATCH_LATCH   : %c\n", _REJO_WY_MATCH_LATCH  .c());
  d("WY_MATCH         : %c\n", _SARY_WY_MATCH        .c());
  d("WX_MATCHn_A      : %c\n", _RYFA_WX_MATCHn_A     .c());
  d("WX_MATCHn_B      : %c\n", _RENE_WX_MATCHn_B     .c());
  d("WX_MATCH_A       : %c\n", _PYCO_WX_MATCH_A      .c());
  d("WX_MATCH_B       : %c\n", _NUNU_WX_MATCH_B      .c());
  d("\n");
}

//------------------------------------------------------------------------------

void PixelPipe::tick(const SchematicTop& top) {
  _XENA_STORE_MATCHn = top.sprite_store.XENA_STORE_MATCHn();

}

//------------------------------------------------------------------------------

void PixelPipe::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  wire GND = 0;

  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor(top.sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), top.clk_reg.MYVO_AxCxExGx());
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not(TOMU_WIN_HITp());
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and (_SOCY_WIN_HITn, top.tile_fetcher.POKY_PORCH_DONEp(), _VYBO_PIX_CLK_xBxDxFxH);
  /*p24.SEGU*/ wire _SEGU_CLKPIPEn = not(_TYFA_CLKPIPEp_xBxDxFxH);
  /*p24.SACU*/ wire _SACU_CLKPIPEp = nor(_SEGU_CLKPIPEn, _ROXY_FINE_SCROLL_DONEn);

  //----------------------------------------
  // Pixel counter, has carry lookahead because this can increment every tcycle

  {
    /*p21.RYBO*/ wire _RYBO = xor(XEHO_X0.q(), SAVY_X1.q());
    /*p21.XUKE*/ wire _XUKE = and(XEHO_X0.q(), SAVY_X1.q());

    /*p21.XYLE*/ wire _XYLE = and(XODU_X2.q(), _XUKE);
    /*p21.XEGY*/ wire _XEGY = xor(XODU_X2.q(), _XUKE);
    /*p21.XORA*/ wire _XORA = xor(XYDO_X3.q(), _XYLE);

    /*p21.SAKE*/ wire _SAKE = xor(TUHU_X4.q(), TUKY_X5.q());
    /*p21.TYBA*/ wire _TYBA = and(TUHU_X4.q(), TUKY_X5.q());
    /*p21.SURY*/ wire _SURY = and(TAKO_X6.q(), _TYBA);
    /*p21.TYGE*/ wire _TYGE = xor(TAKO_X6.q(), _TYBA);
    /*p21.ROKU*/ wire _ROKU = xor(SYBE_X7.q(), _SURY);

    /*p24.TOCA*/ wire _TOCA_CLKPIPE_HI = not(XYDO_X3.q());

    /*p21.TADY*/ wire _TADY_LINE_START_RST = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());

    /*p21.XEHO*/ XEHO_X0 = dff17(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, XEHO_X0.qn());
    /*p21.SAVY*/ SAVY_X1 = dff17(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _RYBO);
    /*p21.XODU*/ XODU_X2 = dff17(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XEGY);
    /*p21.XYDO*/ XYDO_X3 = dff17(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XORA);
    /*p21.TUHU*/ TUHU_X4 = dff17(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, TUHU_X4.qn());
    /*p21.TUKY*/ TUKY_X5 = dff17(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _SAKE);
    /*p21.TAKO*/ TAKO_X6 = dff17(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _TYGE);
    /*p21.SYBE*/ SYBE_X7 = dff17(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _ROKU);
  }

  //----------------------------------------
  // LCD pins that are controlled by the pixel counter

  {
    /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
    /*p21.WEGO*/ wire _WEGO_LINE_END_RST   = or(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONE_SYNC.q());
    /*p21.WUSA*/ _WUSA_LCD_CLOCK_GATE = nor_latch(_XAJO_X_009, _WEGO_LINE_END_RST);
  }

  {
    /*p21.TOBA*/ wire _TOBA_LCD_CLOCK = and (_SACU_CLKPIPEp, _WUSA_LCD_CLOCK_GATE);
    /*p27.POVA*/ wire _POVA_FINE_MATCHpe = and(_PUXA_FINE_MATCH_A.q(), _NYZE_FINE_MATCH_B.qn());
    /*p21.SEMU*/ wire _SEMU_LCD_CLOCK = or(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
    /*p21.RYPO*/ wire _RYPO_LCD_CLOCK = not(_SEMU_LCD_CLOCK);
    _LCD_PIN_CP = _RYPO_LCD_CLOCK;
  }

  {
    // LCD horizontal sync pin latch
    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.

    /*p24.ROXO*/ wire _ROXO_CLKPIPEp = not(_SEGU_CLKPIPEn);
    /*p24.PAHO*/ _PAHO_X_8_SYNC = dff17(_ROXO_CLKPIPEp, _XYMU_RENDERINGp, XYDO_X3.q());
    /*p24.RUJU*/ _POFY_ST_LATCH = nor_latch(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), _PAHO_X_8_SYNC.q() || top.clk_reg.TOFU_VID_RSTp());
    /*p24.RUZE*/ wire _RUZE_PIN_ST = not(_POFY_ST_LATCH);
    _LCD_PIN_ST = _RUZE_PIN_ST;
  }

  //----------------------------------------
  // Fine scroll counter

  {
    /*p24.ROXO*/ wire _ROXO_CLKPIPEp = not(_SEGU_CLKPIPEn);
    /*p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand(_RYKU_FINE_CNT0.q(), _ROGA_FINE_CNT1.q(), _RUBU_FINE_CNT2.q());
    /*p27.PECU*/ wire _PECU_FINE_CLK = nand(_ROXO_CLKPIPEp, _ROZE_FINE_COUNT_7n);
    /*p27.PASO*/ wire _PASO_FINE_RST = nor(top.TEVO_FINE_RSTp(), ROPY_RENDERINGn());
    /*p27.RYKU*/ _RYKU_FINE_CNT0 = dff17(_PECU_FINE_CLK,       _PASO_FINE_RST, _RYKU_FINE_CNT0.qn());
    /*p27.ROGA*/ _ROGA_FINE_CNT1 = dff17(_RYKU_FINE_CNT0.qn(), _PASO_FINE_RST, _ROGA_FINE_CNT1.qn());
    /*p27.RUBU*/ _RUBU_FINE_CNT2 = dff17(_ROGA_FINE_CNT1.qn(), _PASO_FINE_RST, _RUBU_FINE_CNT2.qn());

    // There's a feedback loop here of sorts

    /*p27.SUHA*/ wire _SUHA_FINE_MATCHp = xnor(top.tile_fetcher.DATY_SCX0.q(), _RYKU_FINE_CNT0.q()); // Arms on the ground side, XNOR
    /*p27.SYBY*/ wire _SYBY_FINE_MATCHp = xnor(top.tile_fetcher.DUZU_SCX1.q(), _ROGA_FINE_CNT1.q());
    /*p27.SOZU*/ wire _SOZU_FINE_MATCHp = xnor(top.tile_fetcher.CYXU_SCX2.q(), _RUBU_FINE_CNT2.q());
    /*p27.RONE*/ wire _RONE_FINE_MATCHn = nand(_ROXY_FINE_SCROLL_DONEn, _SUHA_FINE_MATCHp, _SYBY_FINE_MATCHp, _SOZU_FINE_MATCHp);
    /*p27.POHU*/ wire _POHU_FINE_MATCHp = not(_RONE_FINE_MATCHn);

    /*p27.PUXA*/ _PUXA_FINE_MATCH_A = dff17(_ROXO_CLKPIPEp,              _XYMU_RENDERINGp, _POHU_FINE_MATCHp);
    /*p27.NYZE*/ _NYZE_FINE_MATCH_B = dff17(top.clk_reg.MOXE_AxCxExGx(), _XYMU_RENDERINGp, _PUXA_FINE_MATCH_A.q());

    /*p27.PAHA*/ wire _PAHA_RENDERINGn = not(_XYMU_RENDERINGp);
    /*p27.POVA*/ wire _POVA_FINE_MATCHpe = and(_PUXA_FINE_MATCH_A.q(), _NYZE_FINE_MATCH_B.qn());
    /*p27.ROXY*/ _ROXY_FINE_SCROLL_DONEn = nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCHpe);
  }

  //----------------------------------------
  // Window matcher

  {
    /*p27.MYLO*/ wire _WX_MATCH0 = xnor(XEHO_X0.q(), _MYPA_WX0.q());
    /*p27.PUWU*/ wire _WX_MATCH1 = xnor(SAVY_X1.q(), _NOFE_WX1.q());
    /*p27.PUHO*/ wire _WX_MATCH2 = xnor(XODU_X2.q(), _NOKE_WX2.q());
    /*p27.NYTU*/ wire _WX_MATCH3 = xnor(XYDO_X3.q(), _MEBY_WX3.q());
    /*p27.NEZO*/ wire _WX_MATCH4 = xnor(TUHU_X4.q(), _MYPU_WX4.q());
    /*p27.NORY*/ wire _WX_MATCH5 = xnor(TUKY_X5.q(), _MYCE_WX5.q());
    /*p27.NONO*/ wire _WX_MATCH6 = xnor(TAKO_X6.q(), _MUVO_WX6.q());
    /*p27.PASE*/ wire _WX_MATCH7 = xnor(SYBE_X7.q(), _NUKU_WX7.q());

    /*p27.PUKY*/ wire _WX_MATCH_HIn  = nand(_REJO_WY_MATCH_LATCH, _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
    /*p27.NUFA*/ wire _WX_MATCH_HI   = not (_WX_MATCH_HIn);
    /*p27.NOGY*/ wire _WX_MATCHn     = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
    /*p27.NUKO*/ NUKO_WX_MATCHp = not(_WX_MATCHn);

    /*p27.NAZE*/ wire _WY_MATCH0 = xnor(top.lcd_reg.MUWY_Y0.q(), _NESO_WY0.q());
    /*p27.PEBO*/ wire _WY_MATCH1 = xnor(top.lcd_reg.MYRO_Y1.q(), _NYRO_WY1.q());
    /*p27.POMO*/ wire _WY_MATCH2 = xnor(top.lcd_reg.LEXA_Y2.q(), _NAGA_WY2.q());
    /*p27.NEVU*/ wire _WY_MATCH3 = xnor(top.lcd_reg.LYDO_Y3.q(), _MELA_WY3.q());
    /*p27.NOJO*/ wire _WY_MATCH4 = xnor(top.lcd_reg.LOVU_Y4.q(), _NULO_WY4.q());
    /*p27.PAGA*/ wire _WY_MATCH5 = xnor(top.lcd_reg.LEMA_Y5.q(), _NENE_WY5.q());
    /*p27.PEZO*/ wire _WY_MATCH6 = xnor(top.lcd_reg.MATO_Y6.q(), _NUKA_WY6.q());
    /*p27.NUPA*/ wire _WY_MATCH7 = xnor(top.lcd_reg.LAFO_Y7.q(), _NAFU_WY7.q());

    /*p27.PALO*/ wire _WY_MATCH_HIn  = nand(WYMO_LCDC_WINEN.q(), _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
    /*p27.NELE*/ wire _WY_MATCH_HI   = not(_WY_MATCH_HIn);
    /*p27.PAFU*/ wire _WY_MATCHn     = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
    /*p27.ROGE*/ ROGE_WY_MATCHp = not(_WY_MATCHn);
  }

  //----------------------------------------
  // Window sequencer

  {
    // This trigger fires on the pixel _at_ WX
    /*p27.ROCO*/ wire _ROCO_CLKPIPEp = not(_SEGU_CLKPIPEn);
    /*p27.PYCO*/ _PYCO_WX_MATCH_A = dff17(_ROCO_CLKPIPEp, top.clk_reg.XAPO_VID_RSTn(), NUKO_WX_MATCHp);
    /*p27.NUNU*/ _NUNU_WX_MATCH_B = dff17(top.clk_reg.MEHE_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), _PYCO_WX_MATCH_A.q());

    /*p27.XAHY*/ wire _XAHY_VID_LINE_TRIG_d4n = not(top.lcd_reg.ATEJ_VID_LINE_TRIGp());
    /*p27.XOFO*/ wire _XOFO_WIN_RSTp = nand(WYMO_LCDC_WINEN.q(), _XAHY_VID_LINE_TRIG_d4n, top.clk_reg.XAPO_VID_RSTn());
    /*p27.PYNU*/ _PYNU_WIN_MODE_A = nor_latch(_NUNU_WX_MATCH_B.q(), _XOFO_WIN_RSTp);
    /*p27.NOPA*/ _NOPA_WIN_MODE_B = dff17(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), _PYNU_WIN_MODE_A);
  }

  {
    // This trigger fires on the pixel _after_ WX. Not sure what the fine count is about.
    /*p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand(_RYKU_FINE_CNT0.q(), _ROGA_FINE_CNT1.q(), _RUBU_FINE_CNT2.q());
    /*p27.PANY*/ wire _PANY_WX_MATCHn = nor(NUKO_WX_MATCHp, _ROZE_FINE_COUNT_7n);
    /*p27.RYFA*/ _RYFA_WX_MATCHn_A = dff17(_SEGU_CLKPIPEn, XYMU_RENDERINGp(), _PANY_WX_MATCHn);
    /*p27.RENE*/ _RENE_WX_MATCHn_B = dff17(top.clk_reg.ALET_xBxDxFxH(), XYMU_RENDERINGp(), _RYFA_WX_MATCHn_A.q());
  }

  {
    /*p27.REPU*/ wire _REPU_VBLANK_RSTp = or(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /*p27.SARY*/ _SARY_WY_MATCH = dff17(top.clk_reg.TALU_ABCDxxxx(), top.clk_reg.XAPO_VID_RSTn(), ROGE_WY_MATCHp);
    /*p27.REJO*/ _REJO_WY_MATCH_LATCH = nor_latch(_SARY_WY_MATCH.q(), _REPU_VBLANK_RSTp);
  }

  {
    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
    ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
    ///*p27.RYDY*/ RYDY = nor(PUKU, clk_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);

    /*p27.RYDY*/ _RYDY_WIN_FIRST_TILE_A = nor_latch(NUNY_WX_MATCHpe(), top.clk_reg.PYRY_VID_RSTp() || top.tile_fetcher.PORY_TILE_FETCH_DONE_Bp());
    /*p27.SOVY*/ _SOVY_WIN_FIRST_TILE_B = dff17(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), _RYDY_WIN_FIRST_TILE_A);
  }

  // window x coordinate
  {
    // something weird here, PORE doesn't look like a clock

    /*p27.VETU*/ wire _VETU_WIN_MAP_CLK = and (top.TEVO_FINE_RSTp(), PORE_WIN_MODEp());
    /*p27.XAHY*/ wire _XAHY_VID_LINE_TRIG_d4n = not(top.lcd_reg.ATEJ_VID_LINE_TRIGp());
    /*p27.XOFO*/ wire _XOFO_WIN_RSTp = nand(WYMO_LCDC_WINEN.q(), _XAHY_VID_LINE_TRIG_d4n, top.clk_reg.XAPO_VID_RSTn());
    /*p27.XACO*/ wire _XACO_WIN_RSTn = not(_XOFO_WIN_RSTp);

    /*p27.WYKA*/ _WYKA_WIN_X3 = dff17(_VETU_WIN_MAP_CLK,  _XACO_WIN_RSTn, _WYKA_WIN_X3.qn());
    /*p27.WODY*/ _WODY_WIN_X4 = dff17(_WYKA_WIN_X3.qn(),  _XACO_WIN_RSTn, _WODY_WIN_X4.qn());
    /*p27.WOBO*/ _WOBO_WIN_X5 = dff17(_WODY_WIN_X4.qn(),  _XACO_WIN_RSTn, _WOBO_WIN_X5.qn());
    /*p27.WYKO*/ _WYKO_WIN_X6 = dff17(_WOBO_WIN_X5.qn(),  _XACO_WIN_RSTn, _WYKO_WIN_X6.qn());
    /*p27.XOLO*/ _XOLO_WIN_X7 = dff17(_WYKO_WIN_X6.qn(),  _XACO_WIN_RSTn, _XOLO_WIN_X7.qn());
  }

  // window y coordinate
  // every time we leave win mode we increment win_y
  {
    /*p27.WAZY*/ wire _WAZY_WIN_Y_CLK = not(PORE_WIN_MODEp());
    /*p27.REPU*/ wire _REPU_VBLANK_RSTp = or(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /*p27.SYNY*/ wire _SYNY_VBLANK_RSTn = not(_REPU_VBLANK_RSTp);

    /*p27.VYNO*/ _VYNO_WIN_Y0 = dff17(_WAZY_WIN_Y_CLK,   _SYNY_VBLANK_RSTn, _VYNO_WIN_Y0.qn());
    /*p27.VUJO*/ _VUJO_WIN_Y1 = dff17(_VYNO_WIN_Y0.qn(), _SYNY_VBLANK_RSTn, _VUJO_WIN_Y1.qn());
    /*p27.VYMU*/ _VYMU_WIN_Y2 = dff17(_VUJO_WIN_Y1.qn(), _SYNY_VBLANK_RSTn, _VYMU_WIN_Y2.qn());
    /*p27.TUFU*/ _TUFU_WIN_Y3 = dff17(_VYMU_WIN_Y2.qn(), _SYNY_VBLANK_RSTn, _TUFU_WIN_Y3.qn());
    /*p27.TAXA*/ _TAXA_WIN_Y4 = dff17(_TUFU_WIN_Y3.qn(), _SYNY_VBLANK_RSTn, _TAXA_WIN_Y4.qn());
    /*p27.TOZO*/ _TOZO_WIN_Y5 = dff17(_TAXA_WIN_Y4.qn(), _SYNY_VBLANK_RSTn, _TOZO_WIN_Y5.qn());
    /*p27.TATE*/ _TATE_WIN_Y6 = dff17(_TOZO_WIN_Y5.qn(), _SYNY_VBLANK_RSTn, _TATE_WIN_Y6.qn());
    /*p27.TEKE*/ _TEKE_WIN_Y7 = dff17(_TATE_WIN_Y6.qn(), _SYNY_VBLANK_RSTn, _TEKE_WIN_Y7.qn());
  }

  {
    /*p21.TADY*/ wire _TADY_LINE_START_RST = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());
    /*p21.VOGA*/ _VOGA_RENDER_DONE_SYNC = dff17(top.clk_reg.ALET_xBxDxFxH(), _TADY_LINE_START_RST, WODU_RENDER_DONEp());
  }

  {
    /*p21.WEGO*/ wire _WEGO_LINE_END_RST   = or(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONE_SYNC.q());
    /*p21.XYMU*/ _XYMU_RENDERINGp = nor_latch(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), _WEGO_LINE_END_RST);
  }

  //----------------------------------------
  // Background pipes

  {
    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.LUHE*/ wire BG_PIX_A0n = not(top.vram_bus.LEGU_TILE_DA0.q());
    /*p32.NOLY*/ wire BG_PIX_A1n = not(top.vram_bus.NUDU_TILE_DA1.q());
    /*p32.LEKE*/ wire BG_PIX_A2n = not(top.vram_bus.MUKU_TILE_DA2.q());
    /*p32.LOMY*/ wire BG_PIX_A3n = not(top.vram_bus.LUZO_TILE_DA3.q());
    /*p32.LALA*/ wire BG_PIX_A4n = not(top.vram_bus.MEGU_TILE_DA4.q());
    /*p32.LOXA*/ wire BG_PIX_A5n = not(top.vram_bus.MYJY_TILE_DA5.q());
    /*p32.NEZE*/ wire BG_PIX_A6n = not(top.vram_bus.NASA_TILE_DA6.q());
    /*p32.NOBO*/ wire BG_PIX_A7n = not(top.vram_bus.NEFO_TILE_DA7.q());

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.LEGU_TILE_DA0.q());
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.NUDU_TILE_DA1.q());
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.MUKU_TILE_DA2.q());
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.LUZO_TILE_DA3.q());
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.MEGU_TILE_DA4.q());
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.MYJY_TILE_DA5.q());
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.NASA_TILE_DA6.q());
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(_LOZE_PIPE_A_LOAD, top.vram_bus.NEFO_TILE_DA7.q());

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A0n);
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A1n);
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A2n);
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A3n);
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A4n);
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A5n);
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A6n);
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A7n);

    /*p32.MYDE*/ BG_PIPE_A0 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET0, BG_PIPE_A_RST0, GND);
    /*p32.NOZO*/ BG_PIPE_A1 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET1, BG_PIPE_A_RST1, BG_PIPE_A0.q());
    /*p32.MOJU*/ BG_PIPE_A2 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET2, BG_PIPE_A_RST2, BG_PIPE_A1.q());
    /*p32.MACU*/ BG_PIPE_A3 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET3, BG_PIPE_A_RST3, BG_PIPE_A2.q());
    /*p32.NEPO*/ BG_PIPE_A4 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET4, BG_PIPE_A_RST4, BG_PIPE_A3.q());
    /*p32.MODU*/ BG_PIPE_A5 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET5, BG_PIPE_A_RST5, BG_PIPE_A4.q());
    /*p32.NEDA*/ BG_PIPE_A6 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET6, BG_PIPE_A_RST6, BG_PIPE_A5.q());
    /*p32.PYBO*/ BG_PIPE_A7 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET7, BG_PIPE_A_RST7, BG_PIPE_A6.q());
  }

  {
    /*p32.LUXA*/ wire _LUXA_PIPE_B_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.TOSA*/ wire BG_PIX_B0n = not(top.vram_bus.RAWU_TILE_DB0.q());
    /*p32.RUCO*/ wire BG_PIX_B1n = not(top.vram_bus.POZO_TILE_DB1.q());
    /*p32.TYCE*/ wire BG_PIX_B2n = not(top.vram_bus.PYZO_TILE_DB2.q());
    /*p32.REVY*/ wire BG_PIX_B3n = not(top.vram_bus.POXA_TILE_DB3.q());
    /*p32.RYGA*/ wire BG_PIX_B4n = not(top.vram_bus.PULO_TILE_DB4.q());
    /*p32.RYLE*/ wire BG_PIX_B5n = not(top.vram_bus.POJU_TILE_DB5.q());
    /*p32.RAPU*/ wire BG_PIX_B6n = not(top.vram_bus.POWY_TILE_DB6.q());
    /*p32.SOJA*/ wire BG_PIX_B7n = not(top.vram_bus.PYJU_TILE_DB7.q());

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.RAWU_TILE_DB0.q());
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.POZO_TILE_DB1.q());
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.PYZO_TILE_DB2.q());
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.POXA_TILE_DB3.q());
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.PULO_TILE_DB4.q());
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.POJU_TILE_DB5.q());
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.POWY_TILE_DB6.q());
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(_LUXA_PIPE_B_LOAD, top.vram_bus.PYJU_TILE_DB7.q());

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B0n);
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B1n);
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B2n);
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B3n);
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B4n);
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B5n);
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B6n);
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B7n);

    /*p32.TOMY*/ BG_PIPE_B0 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET0, BG_PIPE_B_RST0, GND);
    /*p32.TACA*/ BG_PIPE_B1 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET1, BG_PIPE_B_RST1, BG_PIPE_B0.q());
    /*p32.SADY*/ BG_PIPE_B2 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET2, BG_PIPE_B_RST2, BG_PIPE_B1.q());
    /*p32.RYSA*/ BG_PIPE_B3 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET3, BG_PIPE_B_RST3, BG_PIPE_B2.q());
    /*p32.SOBO*/ BG_PIPE_B4 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET4, BG_PIPE_B_RST4, BG_PIPE_B3.q());
    /*p32.SETU*/ BG_PIPE_B5 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET5, BG_PIPE_B_RST5, BG_PIPE_B4.q());
    /*p32.RALU*/ BG_PIPE_B6 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET6, BG_PIPE_B_RST6, BG_PIPE_B5.q());
    /*p32.SOHU*/ BG_PIPE_B7 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET7, BG_PIPE_B_RST7, BG_PIPE_B6.q());
  }

  //----------------------------------------
  // Sprite pipes

  {
    /*p29.XEFY*/ wire _XEPY_SPRITE_DONEn = not(top.sprite_fetcher.WUTY_SPRITE_DONEp());
    /*p34.MEFU*/ wire _MEFU_SPRITE_MASK0p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A0.q(), SPR_PIPE_B0.q()); // def or
    /*p34.MEVE*/ wire _MEVE_SPRITE_MASK1p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A1.q(), SPR_PIPE_B1.q());
    /*p34.MYZO*/ wire _MYZO_SPRITE_MASK2p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A2.q(), SPR_PIPE_B2.q());
    /*p34.RUDA*/ wire _RUDA_SPRITE_MASK3p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A3.q(), SPR_PIPE_B3.q());
    /*p34.VOTO*/ wire _VOTO_SPRITE_MASK4p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A4.q(), SPR_PIPE_B4.q());
    /*p34.VYSA*/ wire _VYSA_SPRITE_MASK5p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A5.q(), SPR_PIPE_B5.q());
    /*p34.TORY*/ wire _TORY_SPRITE_MASK6p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A6.q(), SPR_PIPE_B6.q());
    /*p34.WOPE*/ wire _WOPE_SPRITE_MASK7p = or(_XEPY_SPRITE_DONEn, SPR_PIPE_A7.q(), SPR_PIPE_B7.q());

    /*p34.LESY*/ wire _LESY_SPRITE_MASK0n = not(_MEFU_SPRITE_MASK0p);
    /*p34.LOTA*/ wire _LOTA_SPRITE_MASK1n = not(_MEVE_SPRITE_MASK1p);
    /*p34.LYKU*/ wire _LYKU_SPRITE_MASK2n = not(_MYZO_SPRITE_MASK2p);
    /*p34.ROBY*/ wire _ROBY_SPRITE_MASK3n = not(_RUDA_SPRITE_MASK3p);
    /*p34.TYTA*/ wire _TYTA_SPRITE_MASK4n = not(_VOTO_SPRITE_MASK4p);
    /*p34.TYCO*/ wire _TYCO_SPRITE_MASK5n = not(_VYSA_SPRITE_MASK5p);
    /*p34.SOKA*/ wire _SOKA_SPRITE_MASK6n = not(_TORY_SPRITE_MASK6p);
    /*p34.XOVU*/ wire _XOVU_SPRITE_MASK7n = not(_WOPE_SPRITE_MASK7p);

    // Sprite pipe A
    {
      /*p33.LOZA*/ wire SPR_PIX_A0n = not(top.vram_bus.PEFO_SPRITE_DA0.q());
      /*p33.SYBO*/ wire SPR_PIX_A1n = not(top.vram_bus.ROKA_SPRITE_DA1.q());
      /*p33.LUMO*/ wire SPR_PIX_A2n = not(top.vram_bus.MYTU_SPRITE_DA2.q());
      /*p33.SOLO*/ wire SPR_PIX_A3n = not(top.vram_bus.RAMU_SPRITE_DA3.q());
      /*p33.VOBY*/ wire SPR_PIX_A4n = not(top.vram_bus.SELE_SPRITE_DA4.q());
      /*p33.WYCO*/ wire SPR_PIX_A5n = not(top.vram_bus.SUTO_SPRITE_DA5.q());
      /*p33.SERY*/ wire SPR_PIX_A6n = not(top.vram_bus.RAMA_SPRITE_DA6.q());
      /*p33.SELU*/ wire SPR_PIX_A7n = not(top.vram_bus.RYDU_SPRITE_DA7.q());

      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(_LESY_SPRITE_MASK0n, top.vram_bus.PEFO_SPRITE_DA0.q());
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(_LOTA_SPRITE_MASK1n, top.vram_bus.ROKA_SPRITE_DA1.q());
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(_LYKU_SPRITE_MASK2n, top.vram_bus.MYTU_SPRITE_DA2.q());
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(_ROBY_SPRITE_MASK3n, top.vram_bus.RAMU_SPRITE_DA3.q());
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(_TYTA_SPRITE_MASK4n, top.vram_bus.SELE_SPRITE_DA4.q());
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(_TYCO_SPRITE_MASK5n, top.vram_bus.SUTO_SPRITE_DA5.q());
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(_SOKA_SPRITE_MASK6n, top.vram_bus.RAMA_SPRITE_DA6.q());
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(_XOVU_SPRITE_MASK7n, top.vram_bus.RYDU_SPRITE_DA7.q());

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(_LESY_SPRITE_MASK0n, SPR_PIX_A0n);
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(_LOTA_SPRITE_MASK1n, SPR_PIX_A1n);
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(_LYKU_SPRITE_MASK2n, SPR_PIX_A2n);
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(_ROBY_SPRITE_MASK3n, SPR_PIX_A3n);
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(_TYTA_SPRITE_MASK4n, SPR_PIX_A4n);
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(_TYCO_SPRITE_MASK5n, SPR_PIX_A5n);
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(_SOKA_SPRITE_MASK6n, SPR_PIX_A6n);
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(_XOVU_SPRITE_MASK7n, SPR_PIX_A7n);

      /*p33.NYLU*/ SPR_PIPE_A0 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET0, SPR_PIX_A_RST0, GND);
      /*p33.PEFU*/ SPR_PIPE_A1 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET1, SPR_PIX_A_RST1, SPR_PIPE_A0.q());
      /*p33.NATY*/ SPR_PIPE_A2 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET2, SPR_PIX_A_RST2, SPR_PIPE_A1.q());
      /*p33.PYJO*/ SPR_PIPE_A3 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET3, SPR_PIX_A_RST3, SPR_PIPE_A2.q());
      /*p33.VARE*/ SPR_PIPE_A4 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET4, SPR_PIX_A_RST4, SPR_PIPE_A3.q());
      /*p33.WEBA*/ SPR_PIPE_A5 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET5, SPR_PIX_A_RST5, SPR_PIPE_A4.q());
      /*p33.VANU*/ SPR_PIPE_A6 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET6, SPR_PIX_A_RST6, SPR_PIPE_A5.q());
      /*p33.VUPY*/ SPR_PIPE_A7 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET7, SPR_PIX_A_RST7, SPR_PIPE_A6.q());
    }

    // Sprite pipe B
    {
      /*p33.RATA*/ wire SPR_PIX_B0n = not(top.vram_bus.REWO_SPRITE_DB0.q());
      /*p33.NUCA*/ wire SPR_PIX_B1n = not(top.vram_bus.PEBA_SPRITE_DB1.q());
      /*p33.LASE*/ wire SPR_PIX_B2n = not(top.vram_bus.MOFO_SPRITE_DB2.q());
      /*p33.LUBO*/ wire SPR_PIX_B3n = not(top.vram_bus.PUDU_SPRITE_DB3.q());
      /*p33.WERY*/ wire SPR_PIX_B4n = not(top.vram_bus.SAJA_SPRITE_DB4.q());
      /*p33.WURA*/ wire SPR_PIX_B5n = not(top.vram_bus.SUNY_SPRITE_DB5.q());
      /*p33.SULU*/ wire SPR_PIX_B6n = not(top.vram_bus.SEMO_SPRITE_DB6.q());
      /*p33.WAMY*/ wire SPR_PIX_B7n = not(top.vram_bus.SEGA_SPRITE_DB7.q());

      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(_LESY_SPRITE_MASK0n, top.vram_bus.REWO_SPRITE_DB0.q());
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(_LOTA_SPRITE_MASK1n, top.vram_bus.PEBA_SPRITE_DB1.q());
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(_LYKU_SPRITE_MASK2n, top.vram_bus.MOFO_SPRITE_DB2.q());
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(_ROBY_SPRITE_MASK3n, top.vram_bus.PUDU_SPRITE_DB3.q());
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(_TYTA_SPRITE_MASK4n, top.vram_bus.SAJA_SPRITE_DB4.q());
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(_TYCO_SPRITE_MASK5n, top.vram_bus.SUNY_SPRITE_DB5.q());
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(_SOKA_SPRITE_MASK6n, top.vram_bus.SEMO_SPRITE_DB6.q());
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(_XOVU_SPRITE_MASK7n, top.vram_bus.SEGA_SPRITE_DB7.q());

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(_LESY_SPRITE_MASK0n, SPR_PIX_B0n);
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(_LOTA_SPRITE_MASK1n, SPR_PIX_B1n);
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(_LYKU_SPRITE_MASK2n, SPR_PIX_B2n);
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(_ROBY_SPRITE_MASK3n, SPR_PIX_B3n);
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(_TYTA_SPRITE_MASK4n, SPR_PIX_B4n);
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(_TYCO_SPRITE_MASK5n, SPR_PIX_B5n);
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(_SOKA_SPRITE_MASK6n, SPR_PIX_B6n);
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(_XOVU_SPRITE_MASK7n, SPR_PIX_B7n);

      /*p33.NURO*/ SPR_PIPE_B0 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET0, SPR_PIX_B_RST0, GND);
      /*p33.MASO*/ SPR_PIPE_B1 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET1, SPR_PIX_B_RST1, SPR_PIPE_B0.q());
      /*p33.LEFE*/ SPR_PIPE_B2 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET2, SPR_PIX_B_RST2, SPR_PIPE_B1.q());
      /*p33.LESU*/ SPR_PIPE_B3 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET3, SPR_PIX_B_RST3, SPR_PIPE_B2.q());
      /*p33.WYHO*/ SPR_PIPE_B4 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET4, SPR_PIX_B_RST4, SPR_PIPE_B3.q());
      /*p33.WORA*/ SPR_PIPE_B5 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET5, SPR_PIX_B_RST5, SPR_PIPE_B4.q());
      /*p33.VAFO*/ SPR_PIPE_B6 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET6, SPR_PIX_B_RST6, SPR_PIPE_B5.q());
      /*p33.WUFY*/ SPR_PIPE_B7 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET7, SPR_PIX_B_RST7, SPR_PIPE_B6.q());
    }

    // Palette pipe
    {
      /*p34.SYPY*/ wire _SYPY = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.TOTU*/ wire _TOTU = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.NARO*/ wire _NARO = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.WEXY*/ wire _WEXY = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.RYZY*/ wire _RYZY = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.RYFE*/ wire _RYFE = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.LADY*/ wire _LADY = not(top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.LAFY*/ wire _LAFY = not(top.oam_bus.GOMO_OAM_DA4.q());

      /*p34.PUME*/ wire _PUME_PAL_PIPE_SET0n = nand(_LESY_SPRITE_MASK0n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.SORO*/ wire _SORO_PAL_PIPE_SET1n = nand(_LOTA_SPRITE_MASK1n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n = nand(_LYKU_SPRITE_MASK2n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n = nand(_ROBY_SPRITE_MASK3n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.RORA*/ wire _RORA_PAL_PIPE_SET4n = nand(_TYTA_SPRITE_MASK4n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.MENE*/ wire _MENE_PAL_PIPE_SET5n = nand(_TYCO_SPRITE_MASK5n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n = nand(_SOKA_SPRITE_MASK6n, top.oam_bus.GOMO_OAM_DA4.q());
      /*p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n = nand(_XOVU_SPRITE_MASK7n, top.oam_bus.GOMO_OAM_DA4.q());

      /*p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n = nand(_LESY_SPRITE_MASK0n, _SYPY);
      /*p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n = nand(_LOTA_SPRITE_MASK1n, _TOTU);
      /*p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n = nand(_LYKU_SPRITE_MASK2n, _NARO);
      /*p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n = nand(_ROBY_SPRITE_MASK3n, _WEXY);
      /*p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n = nand(_TYTA_SPRITE_MASK4n, _RYZY);
      /*p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n = nand(_TYCO_SPRITE_MASK5n, _RYFE);
      /*p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n = nand(_SOKA_SPRITE_MASK6n, _LADY);
      /*p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n = nand(_XOVU_SPRITE_MASK7n, _LAFY);

      /*p34.RUGO*/ PAL_PIPE_0 = dff22(_SACU_CLKPIPEp, _PUME_PAL_PIPE_SET0n, _SUCO_PAL_PIPE_RST0n, GND);
      /*p34.SATA*/ PAL_PIPE_1 = dff22(_SACU_CLKPIPEp, _SORO_PAL_PIPE_SET1n, _TAFA_PAL_PIPE_RST1n, PAL_PIPE_0.q());
      /*p34.ROSA*/ PAL_PIPE_2 = dff22(_SACU_CLKPIPEp, _PAMO_PAL_PIPE_SET2n, _PYZY_PAL_PIPE_RST2n, PAL_PIPE_1.q());
      /*p34.SOMY*/ PAL_PIPE_3 = dff22(_SACU_CLKPIPEp, _SUKY_PAL_PIPE_SET3n, _TOWA_PAL_PIPE_RST3n, PAL_PIPE_2.q());
      /*p34.PALU*/ PAL_PIPE_4 = dff22(_SACU_CLKPIPEp, _RORA_PAL_PIPE_SET4n, _RUDU_PAL_PIPE_RST4n, PAL_PIPE_3.q());
      /*p34.NUKE*/ PAL_PIPE_5 = dff22(_SACU_CLKPIPEp, _MENE_PAL_PIPE_SET5n, _PAZO_PAL_PIPE_RST5n, PAL_PIPE_4.q());
      /*p34.MODA*/ PAL_PIPE_6 = dff22(_SACU_CLKPIPEp, _LUKE_PAL_PIPE_SET6n, _LOWA_PAL_PIPE_RST6n, PAL_PIPE_5.q());
      /*p34.LYME*/ PAL_PIPE_7 = dff22(_SACU_CLKPIPEp, _LAMY_PAL_PIPE_SET7n, _LUNU_PAL_PIPE_RST7n, PAL_PIPE_6.q());
    }

    // Background mask pipe
    {
      /*p26.XOGA*/ wire _XOGA = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XURA*/ wire _XURA = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.TAJO*/ wire _TAJO = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XENU*/ wire _XENU = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XYKE*/ wire _XYKE = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XABA*/ wire _XABA = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.TAFU*/ wire _TAFU = not(top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XUHO*/ wire _XUHO = not(top.oam_bus.DEPO_OAM_DA7.q());

      /*p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0 = nand(_LESY_SPRITE_MASK0n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XALA*/ wire _XALA_MASK_PIPE_SET1 = nand(_LOTA_SPRITE_MASK1n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2 = nand(_LYKU_SPRITE_MASK2n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3 = nand(_ROBY_SPRITE_MASK3n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4 = nand(_TYTA_SPRITE_MASK4n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.XELY*/ wire _XELY_MASK_PIPE_SET5 = nand(_TYCO_SPRITE_MASK5n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6 = nand(_SOKA_SPRITE_MASK6n, top.oam_bus.DEPO_OAM_DA7.q());
      /*p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7 = nand(_XOVU_SPRITE_MASK7n, top.oam_bus.DEPO_OAM_DA7.q());

      /*p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0 = nand(_LESY_SPRITE_MASK0n, _XOGA);
      /*p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1 = nand(_LOTA_SPRITE_MASK1n, _XURA);
      /*p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2 = nand(_LYKU_SPRITE_MASK2n, _TAJO);
      /*p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3 = nand(_ROBY_SPRITE_MASK3n, _XENU);
      /*p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4 = nand(_TYTA_SPRITE_MASK4n, _XYKE);
      /*p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5 = nand(_TYCO_SPRITE_MASK5n, _XABA);
      /*p26.TENA*/ wire _TENA_MASK_PIPE_RST6 = nand(_SOKA_SPRITE_MASK6n, _TAFU);
      /*p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7 = nand(_XOVU_SPRITE_MASK7n, _XUHO);

      /*p26.VEZO*/ MASK_PIPE_0 = dff22(_SACU_CLKPIPEp, _TEDE_MASK_PIPE_SET0, _WOKA_MASK_PIPE_RST0, GND);
      /*p26.WURU*/ MASK_PIPE_1 = dff22(_SACU_CLKPIPEp, _XALA_MASK_PIPE_SET1, _WEDE_MASK_PIPE_RST1, MASK_PIPE_0.q());
      /*p26.VOSA*/ MASK_PIPE_2 = dff22(_SACU_CLKPIPEp, _TYRA_MASK_PIPE_SET2, _TUFO_MASK_PIPE_RST2, MASK_PIPE_1.q());
      /*p26.WYFU*/ MASK_PIPE_3 = dff22(_SACU_CLKPIPEp, _XYRU_MASK_PIPE_SET3, _WEVO_MASK_PIPE_RST3, MASK_PIPE_2.q());
      /*p26.XETE*/ MASK_PIPE_4 = dff22(_SACU_CLKPIPEp, _XUKU_MASK_PIPE_SET4, _WEDY_MASK_PIPE_RST4, MASK_PIPE_3.q());
      /*p26.WODA*/ MASK_PIPE_5 = dff22(_SACU_CLKPIPEp, _XELY_MASK_PIPE_SET5, _WUJA_MASK_PIPE_RST5, MASK_PIPE_4.q());
      /*p26.VUMO*/ MASK_PIPE_6 = dff22(_SACU_CLKPIPEp, _TYKO_MASK_PIPE_SET6, _TENA_MASK_PIPE_RST6, MASK_PIPE_5.q());
      /*p26.VAVA*/ MASK_PIPE_7 = dff22(_SACU_CLKPIPEp, _TUWU_MASK_PIPE_SET7, _WUBU_MASK_PIPE_RST7, MASK_PIPE_6.q());
    }
  }

  //----------------------------------------
  // Pixel merge + emit

  {
    /*p35.RAJY*/ wire _PIX_BG0  = and(VYXE_LCDC_BGEN.q(), BG_PIPE_A7.q());
    /*p35.TADE*/ wire _PIX_BG1  = and(VYXE_LCDC_BGEN.q(), BG_PIPE_B7.q());

    /*p35.WOXA*/ wire _PIX_SP0  = and(XYLO_LCDC_SPEN.q(), SPR_PIPE_A7.q());
    /*p35.XULA*/ wire _PIX_SP1  = and(XYLO_LCDC_SPEN.q(), SPR_PIPE_B7.q());

    /*p35.NULY*/ wire _PIX_SPn  = nor(_PIX_SP0, _PIX_SP1);

    /*p35.RYFU*/ wire _MASK_BG0 = and(_PIX_BG0, MASK_PIPE_7.q());
    /*p35.RUTA*/ wire _MASK_BG1 = and(_PIX_BG1, MASK_PIPE_7.q());
    /*p35.POKA*/ wire _BGPIXELn = nor(_PIX_SPn, _MASK_BG0, _MASK_BG1);

    /*p34.LOME*/ wire _SPRITE_PAL_PIPE_7n = not(PAL_PIPE_7.q());
    /*p34.LAFU*/ wire _OBP0PIXELn = nand(_SPRITE_PAL_PIPE_7n, _BGPIXELn);
    /*p34.LEKA*/ wire _OBP1PIXELn = nand(_SPRITE_PAL_PIPE_7n, _BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*p35.VUMU*/ wire _PIX_SP0n = not(_PIX_SP0);
    /*p35.WYRU*/ wire _PIX_SP0a = not(_PIX_SP0n);
    /*p35.WELE*/ wire _PIX_SP1n = not(_PIX_SP1);
    /*p35.WOLO*/ wire _PIX_SP1a = not(_PIX_SP1n);

    /*p35.LAVA*/ wire _MASK_OPB0  = not(_OBP0PIXELn);
    /*p35.VYRO*/ wire _PAL_OBP0D = and(_PIX_SP0a, _PIX_SP1a, _MASK_OPB0);
    /*p35.VATA*/ wire _PAL_OBP0C = and(_PIX_SP0a, _PIX_SP1n, _MASK_OPB0);
    /*p35.VOLO*/ wire _PAL_OBP0B = and(_PIX_SP0n, _PIX_SP1a, _MASK_OPB0);
    /*p35.VUGO*/ wire _PAL_OBP0A = and(_PIX_SP0n, _PIX_SP1n, _MASK_OPB0);

    /*p35.WUFU*/ wire _COL_OBP00 = amux4(_XANA_OBP07.q(), _PAL_OBP0D, _XYZE_OBP05.q(), _PAL_OBP0C, _XALO_OBP03.q(), _PAL_OBP0B, _XUKY_OBP01.q(), _PAL_OBP0A);
    /*p35.WALY*/ wire _COL_OBP01 = amux4(_XUPO_OBP06.q(), _PAL_OBP0D, _XERU_OBP04.q(), _PAL_OBP0C, _XOVA_OBP02.q(), _PAL_OBP0B, _XUFU_OBP00.q(), _PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*p35.MEXA*/ wire _PIX_SP0o = not(_PIX_SP0);
    /*p35.LOZO*/ wire _PIX_SP0b = not(_PIX_SP0o);
    /*p35.MABY*/ wire _PIX_SP1o = not(_PIX_SP1);
    /*p35.LYLE*/ wire _PIX_SP1b = not(_PIX_SP1o);

    /*p35.LUKU*/ wire _MASK_OBP1  = not(_OBP1PIXELn);
    /*p35.LEDO*/ wire _PAL_OBP1D = and(_PIX_SP1b, _PIX_SP0b, _MASK_OBP1);
    /*p35.LYKY*/ wire _PAL_OBP1C = and(_PIX_SP1b, _PIX_SP0o, _MASK_OBP1);
    /*p35.LARU*/ wire _PAL_OBP1B = and(_PIX_SP1o, _PIX_SP0b, _MASK_OBP1);
    /*p35.LOPU*/ wire _PAL_OBP1A = and(_PIX_SP1o, _PIX_SP0o, _MASK_OBP1);

    /*p35.MOKA*/ wire _COL_OBP10 = amux4(_LUXO_OBP17.q(), _PAL_OBP1D, _LUGU_OBP15.q(), _PAL_OBP1C, _LOSE_OBP13.q(), _PAL_OBP1B, _LAWO_OBP11.q(), _PAL_OBP1A);
    /*p35.MUFA*/ wire _COL_OBP11 = amux4(_LEPU_OBP16.q(), _PAL_OBP1D, _LUNE_OBP14.q(), _PAL_OBP1C, _MOSA_OBP12.q(), _PAL_OBP1B, _MOXY_OBP10.q(), _PAL_OBP1A);

    //----------
    // Background/window palette lookup

    /*p35.SOBA*/ wire _PIX_BG0n = not(_PIX_BG0);
    /*p35.VYCO*/ wire _PIX_BG1n = not(_PIX_BG1);
    /*p35.NUPO*/ wire _PIX_BG0a = not(_PIX_BG0n);
    /*p35.NALE*/ wire _PIX_BG1a = not(_PIX_BG1n);

    /*p35.MUVE*/ wire _MASK_BGP = not(_BGPIXELn);
    /*p35.POBU*/ wire _PAL_BGPA = and(_PIX_BG1n, _PIX_BG0n, _MASK_BGP);
    /*p35.NUMA*/ wire _PAL_BGPB = and(_PIX_BG1a, _PIX_BG0n, _MASK_BGP);
    /*p35.NUXO*/ wire _PAL_BGPC = and(_PIX_BG1n, _PIX_BG0a, _MASK_BGP);
    /*p35.NYPO*/ wire _PAL_BGPD = and(_PIX_BG1a, _PIX_BG0a, _MASK_BGP);

    /*p35.NURA*/ wire _COL_BGP1 = amux4(_MENA_BGP7.q(), _PAL_BGPD, _MORU_BGP5.q(), _PAL_BGPC, _MAXY_BGP3.q(), _PAL_BGPB, _NUSY_BGP1.q(), _PAL_BGPA);
    /*p35.NELO*/ wire _COL_BGP0 = amux4(_MOGY_BGP6.q(), _PAL_BGPD, _MUKE_BGP4.q(), _PAL_BGPC, _PYLU_BGP2.q(), _PAL_BGPB, _PAVO_BGP0.q(), _PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ wire _PATY_PIX_OUT_LO = or(_COL_BGP1, _COL_OBP00, _COL_OBP10);
    /*p35.PERO*/ wire _PERO_PIX_OUT_HI = or(_COL_BGP0, _COL_OBP01, _COL_OBP11);

    /*p35.REMY*/ wire _REMY_LD0n = not(_PATY_PIX_OUT_LO);
    /*p35.RAVO*/ wire _RAVO_LD1n = not(_PERO_PIX_OUT_HI);
    _LCD_PIN_LD0 = _REMY_LD0n;
    _LCD_PIN_LD1 = _RAVO_LD1n;
  }

  //----------------------------------------
  // FF40 LCDC

  {
    /*p22.WORU*/ wire _WORU_FF40n = nand(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not(_WORU_FF40n);

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and (_VOCA_FF40p, top.ASOT_CPU_RDp());
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not(_VYRE_FF40_RDp);

    /*p23.WYPO*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_WYCE_FF40_RDn, VYXE_LCDC_BGEN.q());
    /*p23.XERO*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_WYCE_FF40_RDn, XYLO_LCDC_SPEN.q());
    /*p23.WYJU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZE.q());
    /*p23.WUKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_WYCE_FF40_RDn, XAFO_LCDC_BGMAP.q());
    /*p23.VOKE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_WYCE_FF40_RDn, WEXU_LCDC_BGTILE.q());
    /*p23.VATO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_WYCE_FF40_RDn, WYMO_LCDC_WINEN.q());
    /*p23.VAHA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_WYCE_FF40_RDn, WOKY_LCDC_WINMAP.q());
    /*p23.XEBU*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_WYCE_FF40_RDn, XONA_LCDC_EN.q());

    /*p23.WARU*/ wire _WARU_FF40_WRp = and (_VOCA_FF40p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not(top.clk_reg.XORE_SYS_RSTp());
    /*p23.VYXE*/ VYXE_LCDC_BGEN    = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D0);
    /*p23.XYLO*/ XYLO_LCDC_SPEN    = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D1);
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D2);
    /*p23.XAFO*/ XAFO_LCDC_BGMAP   = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D3);
    /*p23.WEXU*/ WEXU_LCDC_BGTILE  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D4);
    /*p23.WYMO*/ WYMO_LCDC_WINEN   = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D5);
    /*p23.WOKY*/ WOKY_LCDC_WINMAP  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D6);
    /*p23.XONA*/ XONA_LCDC_EN      = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D7);
  }

  //----------------------------------------
  // FF41 STAT

  {
    /*p22.WOFA*/ wire _WOFA_FF41n = nand(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VARY*/ wire _VARY_FF41p = not(_WOFA_FF41n);

    /*p21.TOBE*/ wire _TOBE_FF41_RDp = and (_VARY_FF41p, top.ASOT_CPU_RDp()); // die AND
    /*p21.VAVE*/ wire _VAVE_FF41_RDn = not(_TOBE_FF41_RDp); // die INV

    /*p21.SEPA*/ wire _SEPA_FF41_WRp = and (_VARY_FF41p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p21.RYVE*/ wire _RYVE_FF41_WRn = not(_SEPA_FF41_WRp);

    /*p21.RYJU*/ wire _RYJU_FF41_WRn = not(_SEPA_FF41_WRp);
    
    /*p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = nor(top.clk_reg.WESY_SYS_RSTn(), _RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ _RUPO_LYC_MATCH_LATCHn = nor_latch(_PAGO_LYC_MATCH_RST, top.lcd_reg.ROPO_LY_MATCH_SYNCp());

    /*p21.ROXE*/ ROXE_INT_HBL_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D0);
    /*p21.RUFO*/ RUFO_INT_VBL_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D1);
    /*p21.REFE*/ REFE_INT_OAM_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D2);
    /*p21.RUGU*/ RUGU_INT_LYC_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D3);

    /*p21.XATY*/ wire _XATY_STAT_MODE1n = nor(_XYMU_RENDERINGp, top.ACYL_SCANNINGp()); // die NOR
    /*p21.SADU*/ wire _SADU_STAT_MODE0n = nor(_XYMU_RENDERINGp, top.lcd_reg.PARU_VBLANKp_d4()); // die NOR

    // These tribufs are _different_

    /*p21.TEBY*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TOBE_FF41_RDp, not(_SADU_STAT_MODE0n));
    /*p21.WUGA*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TOBE_FF41_RDp, not(_XATY_STAT_MODE1n));
    /*p21.SEGO*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TOBE_FF41_RDp, not(_RUPO_LYC_MATCH_LATCHn));
    /*p21.PUZO*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_VAVE_FF41_RDn, ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_VAVE_FF41_RDn, RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_VAVE_FF41_RDn, REFE_INT_OAM_EN.q());
    /*p21.POTE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_VAVE_FF41_RDn, RUGU_INT_LYC_EN.q());
  }

  //----------------------------------------
  // FF47 BGP

  {
    /*p22.WYBO*/ wire _WYBO_FF47n = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*p22.WERA*/ wire _WERA_FF47 = not(_WYBO_FF47n);
    /*p36.VELY*/ wire _VELY_FF47_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _WERA_FF47);
    /*p36.TEPO*/ wire _TEPO_FF47_WRn = not(_VELY_FF47_WR);

    /*p36.PAVO*/ _PAVO_BGP0 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D0);
    /*p36.NUSY*/ _NUSY_BGP1 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D1);
    /*p36.PYLU*/ _PYLU_BGP2 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D2);
    /*p36.MAXY*/ _MAXY_BGP3 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D3);
    /*p36.MUKE*/ _MUKE_BGP4 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D4);
    /*p36.MORU*/ _MORU_BGP5 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D5);
    /*p36.MOGY*/ _MOGY_BGP6 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D6);
    /*p36.MENA*/ _MENA_BGP7 = dff8(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D7);

    /*p36.VUSO*/ wire _VUSO_FF47_RD = and (top.ASOT_CPU_RDp(), _WERA_FF47);
    /*p36.TEPY*/ wire _TEPY_FF47_RDn = not(_VUSO_FF47_RD);

    /*p36.RARO*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_TEPY_FF47_RDn, _PAVO_BGP0.q());
    /*p36.PABA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_TEPY_FF47_RDn, _NUSY_BGP1.q());
    /*p36.REDO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_TEPY_FF47_RDn, _PYLU_BGP2.q());
    /*p36.LOBE*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_TEPY_FF47_RDn, _MAXY_BGP3.q());
    /*p36.LACE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_TEPY_FF47_RDn, _MUKE_BGP4.q());
    /*p36.LYKA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_TEPY_FF47_RDn, _MORU_BGP5.q());
    /*p36.LODY*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_TEPY_FF47_RDn, _MOGY_BGP6.q());
    /*p36.LARY*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_TEPY_FF47_RDn, _MENA_BGP7.q());
  }

  //----------------------------------------
  // FF48 OBP0

  {
    /*p22.WETA*/ wire _WETA_FF48n = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.XAYO*/ wire _XAYO_FF48 = not(_WETA_FF48n);
    /*p36.XOMA*/ wire _XOMA_FF48_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _XAYO_FF48);
    /*p36.XELO*/ wire _XELO_FF48_WRn = not(_XOMA_FF48_WR);

    /*p36.XUFU*/ _XUFU_OBP00 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D0);
    /*p36.XUKY*/ _XUKY_OBP01 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D1);
    /*p36.XOVA*/ _XOVA_OBP02 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D2);
    /*p36.XALO*/ _XALO_OBP03 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D3);
    /*p36.XERU*/ _XERU_OBP04 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D4);
    /*p36.XYZE*/ _XYZE_OBP05 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D5);
    /*p36.XUPO*/ _XUPO_OBP06 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D6);
    /*p36.XANA*/ _XANA_OBP07 = dff8(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D7);

    /*p36.XUFY*/ wire _XUFY_FF48_RD = and (top.ASOT_CPU_RDp(), _XAYO_FF48);
    /*p36.XOZY*/ wire _XOZY_FF48_RDn = not(_XUFY_FF48_RD);

    /*p36.XARY*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_XOZY_FF48_RDn, _XUFU_OBP00.q());
    /*p36.XOKE*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_XOZY_FF48_RDn, _XUKY_OBP01.q());
    /*p36.XUNO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_XOZY_FF48_RDn, _XOVA_OBP02.q());
    /*p36.XUBY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_XOZY_FF48_RDn, _XALO_OBP03.q());
    /*p36.XAJU*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_XOZY_FF48_RDn, _XERU_OBP04.q());
    /*p36.XOBO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_XOZY_FF48_RDn, _XYZE_OBP05.q());
    /*p36.XAXA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_XOZY_FF48_RDn, _XUPO_OBP06.q());
    /*p36.XAWO*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_XOZY_FF48_RDn, _XANA_OBP07.q());
  }

  //----------------------------------------
  // FF49 OBP1

  {
    /*p22.VAMA*/ wire _VAMA_FF49n = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.TEGO*/ wire _TEGO_FF49 = not(_VAMA_FF49n);
    /*p36.MYXE*/ wire _MYXE_FF49_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _TEGO_FF49);
    /*p36.LEHO*/ wire _LEHO_FF49_WRn = not(_MYXE_FF49_WR);

    /*p36.MOXY*/ _MOXY_OBP10 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D0);
    /*p36.LAWO*/ _LAWO_OBP11 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D1);
    /*p36.MOSA*/ _MOSA_OBP12 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D2);
    /*p36.LOSE*/ _LOSE_OBP13 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D3);
    /*p36.LUNE*/ _LUNE_OBP14 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D4);
    /*p36.LUGU*/ _LUGU_OBP15 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D5);
    /*p36.LEPU*/ _LEPU_OBP16 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D6);
    /*p36.LUXO*/ _LUXO_OBP17 = dff8(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D7);

    /*p36.MUMY*/ wire _MUMY_FF49_RD = and (top.ASOT_CPU_RDp(), _TEGO_FF49);
    /*p36.LOTE*/ wire _LOTE_FF49_RDn = not(_MUMY_FF49_RD);

    /*p36.LAJU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_LOTE_FF49_RDn, _MOXY_OBP10.q());
    /*p36.LEPA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_LOTE_FF49_RDn, _LAWO_OBP11.q());
    /*p36.LODE*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_LOTE_FF49_RDn, _MOSA_OBP12.q());
    /*p36.LYZA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_LOTE_FF49_RDn, _LOSE_OBP13.q());
    /*p36.LUKY*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_LOTE_FF49_RDn, _LUNE_OBP14.q());
    /*p36.LUGA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_LOTE_FF49_RDn, _LUGU_OBP15.q());
    /*p36.LEBA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_LOTE_FF49_RDn, _LEPU_OBP16.q());
    /*p36.LELU*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_LOTE_FF49_RDn, _LUXO_OBP17.q());
  }

  //----------------------------------------
  // FF4A WY

  {
    /*p22.WYVO*/ wire _FF4An = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VYGA*/ wire _FF4Ap = not(_FF4An);

    /*p23.WAXU*/ wire _FF4A_RDp = and (_FF4Ap, top.ASOT_CPU_RDp());
    /*p23.VOMY*/ wire _FF4A_RDn = not(_FF4A_RDp);

    /*p23.WEKO*/ wire _WEKO_FF4A_WRp = and (_FF4Ap, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.VEFU*/ wire _VEFU_FF4A_WRn = not(_WEKO_FF4A_WRp);

    /*p23.NESO*/ _NESO_WY0 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0);
    /*p23.NYRO*/ _NYRO_WY1 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1);
    /*p23.NAGA*/ _NAGA_WY2 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2);
    /*p23.MELA*/ _MELA_WY3 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3);
    /*p23.NULO*/ _NULO_WY4 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4);
    /*p23.NENE*/ _NENE_WY5 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5);
    /*p23.NUKA*/ _NUKA_WY6 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6);
    /*p23.NAFU*/ _NAFU_WY7 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7);


    /*p23.PUNU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_FF4A_RDn, _NESO_WY0.q());
    /*p23.PODA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_FF4A_RDn, _NYRO_WY1.q());
    /*p23.PYGU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_FF4A_RDn, _NAGA_WY2.q());
    /*p23.LOKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_FF4A_RDn, _MELA_WY3.q());
    /*p23.MEGA*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_FF4A_RDn, _NULO_WY4.q());
    /*p23.PELA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_FF4A_RDn, _NENE_WY5.q());
    /*p23.POLO*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_FF4A_RDn, _NUKA_WY6.q());
    /*p23.MERA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_FF4A_RDn, _NAFU_WY7.q());
  }

  //----------------------------------------
  // FF4B WX

  {
    /*p22.WAGE*/ wire _FF4Bn = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VUMY*/ wire _FF4Bp = not(_FF4Bn);

    /*p23.WYZE*/ wire _FF4B_RDp = and (_FF4Bp, top.ASOT_CPU_RDp());
    /*p23.VYCU*/ wire _FF4B_RDn = not(_FF4B_RDp);

    /*p23.WUZA*/ wire _WUZA_FF4B_WRp = and (_FF4Bp, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.VOXU*/ wire _VOXU_FF4B_WRn = not(_WUZA_FF4B_WRp);

    /*p23.MYPA*/ _MYPA_WX0 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0);
    /*p23.NOFE*/ _NOFE_WX1 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1);
    /*p23.NOKE*/ _NOKE_WX2 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2);
    /*p23.MEBY*/ _MEBY_WX3 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3);
    /*p23.MYPU*/ _MYPU_WX4 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4);
    /*p23.MYCE*/ _MYCE_WX5 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5);
    /*p23.MUVO*/ _MUVO_WX6 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6);
    /*p23.NUKU*/ _NUKU_WX7 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7);


    /*p23.LOVA*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_FF4B_RDn, _MYPA_WX0.q());
    /*p23.MUKA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_FF4B_RDn, _NOFE_WX1.q());
    /*p23.MOKO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_FF4B_RDn, _NOKE_WX2.q());
    /*p23.LOLE*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_FF4B_RDn, _MEBY_WX3.q());
    /*p23.MELE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_FF4B_RDn, _MYPU_WX4.q());
    /*p23.MUFE*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_FF4B_RDn, _MYCE_WX5.q());
    /*p23.MULY*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_FF4B_RDn, _MUVO_WX6.q());
    /*p23.MARA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_FF4B_RDn, _NUKU_WX7.q());
  }
}

//------------------------------------------------------------------------------
