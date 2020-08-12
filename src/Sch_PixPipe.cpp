#include "Sch_PixPipe.h"

#include "Sch_Top.h"
#include "Probe.h"

using namespace Schematics;

void PixelPipe::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("---------- Pix Pipe ----------\n");

  d("PIX COUNT   %03d\n", get_pix_count());
  d("\n");

  d("VYXE_LCDC_BGENn   %c\n", VYXE_LCDC_BGENn  .c());
  d("XYLO_LCDC_SPENn   %c\n", XYLO_LCDC_SPENn  .c());
  d("XYMO_LCDC_SPSIZEn %c\n", XYMO_LCDC_SPSIZEn.c());
  d("XAFO_LCDC_BGMAPn  %c\n", XAFO_LCDC_BGMAPn .c());
  d("WEXU_LCDC_BGTILEn %c\n", WEXU_LCDC_BGTILEn.c());
  d("WYMO_LCDC_WINENn  %c\n", WYMO_LCDC_WINENn .c());
  d("WOKY_LCDC_WINMAP %c\n", WOKY_LCDC_WINMAP.c());
  d("XONA_LCDC_ENn     %c\n", XONA_LCDC_ENn    .c());
  d("\n");

  d("INT_HBL_EN  %c\n", ROXE_INT_HBL_EN.c());
  d("INT_VBL_EN  %c\n", RUFO_INT_VBL_EN.c());
  d("INT_OAM_EN  %c\n", REFE_INT_OAM_EN.c());
  d("INT_LYC_EN  %c\n", RUGU_INT_LYC_EN.c());

  d("ROXY_FINE_SCROLL_DONEn %c\n", _ROXY_SCX_FINE_MATCH_LATCHn.c());
  d("RYKU_FINE_CNT0         %c\n", _RYKU_FINE_CNT0        .c());
  d("ROGA_FINE_CNT1         %c\n", _ROGA_FINE_CNT1        .c());
  d("RUBU_FINE_CNT2         %c\n", _RUBU_FINE_CNT2        .c());
  d("_XENA_STORE_MATCHn     %c\n", _XENA_STORE_MATCHn     .c());
  d("XYMU_RENDERINGp        %c\n", _XYMU_RENDERINGp       .c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", _RUPO_LYC_MATCH_LATCHn .c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", _WUSA_LCD_CLOCK_GATE   .c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", _VOGA_RENDER_DONEp .c());
  d("PUXA_FINE_MATCH_A      %c\n", _PUXA_SCX_FINE_MATCH_A     .c());
  d("NYZE_FINE_MATCH_B      %c\n", _NYZE_SCX_FINE_MATCH_B     .c());
  d("PAHO_X_8_SYNC          %c\n", _PAHO_X_8_SYNC         .c());
  d("RUJU_LCD_PIN_STp       %c\n", RUJU_HSYNCn.c());
  d("POME_LCD_PIN_STn       %c\n", POME_HSYNCn.c());

  d("BG_PIPE_A  %c%c%c%c%c%c%c%c\n", 
    MYDE_BG_PIPE_A0.c(), NOZO_BG_PIPE_A1.c(), MOJU_BG_PIPE_A2.c(), MACU_BG_PIPE_A3.c(),
    NEPO_BG_PIPE_A4.c(), MODU_BG_PIPE_A5.c(), NEDA_BG_PIPE_A6.c(), PYBO_BG_PIPE_A7.c());

  d("BG_PIPE_B  %c%c%c%c%c%c%c%c\n", 
    TOMY_BG_PIPE_B0.c(), TACA_BG_PIPE_B1.c(), SADY_BG_PIPE_B2.c(), RYSA_BG_PIPE_B3.c(),
    SOBO_BG_PIPE_B4.c(), SETU_BG_PIPE_B5.c(), RALU_BG_PIPE_B6.c(), SOHU_BG_PIPE_B7.c());

  d("SPR_PIPE_A %c%c%c%c%c%c%c%c\n", 
    NYLU_SPR_PIPE_B0.c(), PEFU_SPR_PIPE_B1.c(), NATY_SPR_PIPE_B2.c(), PYJO_SPR_PIPE_B3.c(),
    VARE_SPR_PIPE_B4.c(), WEBA_SPR_PIPE_B5.c(), VANU_SPR_PIPE_B6.c(), VUPY_SPR_PIPE_B7.c());

  d("SPR_PIPE_B %c%c%c%c%c%c%c%c\n", 
    NURO_SPR_PIPE_A0.c(), MASO_SPR_PIPE_A1.c(), LEFE_SPR_PIPE_A2.c(), LESU_SPR_PIPE_A3.c(),
    WYHO_SPR_PIPE_A4.c(), WORA_SPR_PIPE_A5.c(), VAFO_SPR_PIPE_A6.c(), WUFY_SPR_PIPE_A7.c());

  d("PAL_PIPE   %c%c%c%c%c%c%c%c\n", 
    RUGO_PAL_PIPE_0.c(), SATA_PAL_PIPE_1.c(), ROSA_PAL_PIPE_2.c(), SOMY_PAL_PIPE_3.c(), 
    PALU_PAL_PIPE_4.c(), NUKE_PAL_PIPE_5.c(), MODA_PAL_PIPE_6.c(), LYME_PAL_PIPE_7.c()
  );

  d("MASK_PIPE  %c%c%c%c%c%c%c%c\n", 
    VEZO_MASK_PIPE_0.c(), WURU_MASK_PIPE_1.c(), VOSA_MASK_PIPE_2.c(), WYFU_MASK_PIPE_3.c(),
    XETE_MASK_PIPE_4.c(), WODA_MASK_PIPE_5.c(), VUMO_MASK_PIPE_6.c(), VAVA_MASK_PIPE_7.c()
  );

  d("BGP  %c%c %c%c %c%c %c%c\n",
    MENA_BGP_D7.c(), MOGY_BGP_D6.c(), MORU_BGP_D5.c(), MUKE_BGP_D4.c(),
    MAXY_BGP_D3.c(), PYLU_BGP_D2.c(), NUSY_BGP_D1.c(), PAVO_BGP_D0.c());

  d("OBP0 %c%c %c%c %c%c %c%c\n",
    XANA_OBP0_D7.c(), XUPO_OBP0_D6.c(), XYZE_OBP0_D5.c(), XERU_OBP0_D4.c(),
    XALO_OBP0_D3.c(), XOVA_OBP0_D2.c(), XUKY_OBP0_D1.c(), XUFU_OBP0_D0.c());

  d("OBP1 %c%c %c%c %c%c %c%c\n",
    LUXO_OBP1_D7.c(), LEPU_OBP1_D6.c(), LUGU_OBP1_D5.c(), LUNE_OBP1_D4.c(),
    LOSE_OBP1_D3.c(), MOSA_OBP1_D2.c(), LAWO_OBP1_D1.c(), MOXY_OBP1_D0.c());

  d("WY %03d\n",
    pack_p(NAFU_WY7.qp(), NUKA_WY6.qp(), NENE_WY5.qp(), NULO_WY4.qp(),
           MELA_WY3.qp(), NAGA_WY2.qp(), NYRO_WY1.qp(), NESO_WY0.qp()));

  d("WX %03d\n",
    pack_p(NUKU_WX7.qp(), MUVO_WX6.qp(), MYCE_WX5.qp(), MYPU_WX4.qp(),
           MEBY_WX3.qp(), NOKE_WX2.qp(), NOFE_WX1.qp(), MYPA_WX0.qp()));

  d("\n");

  d("----------  Window  ----------\n");

  d("WX                    : %d\n", get_wx());
  d("WY                    : %d\n", get_wy());
  d("WIN X                 : %d\n", get_win_x());
  d("WIN_Y                 : %d\n", get_win_y());
  d("NUKO_WX_MATCHp        : %c\n", NUKO_WX_MATCHp.c());
  d("ROGE_WY_MATCHp        : %c\n", ROGE_WY_MATCHp.c());
  d("PYNU_WIN_MODE_A       : %c\n", PYNU_WIN_MODE_A.c());
  d("PUKU_WIN_FIRST_TILE_A : %c\n", PUKU_WIN_FIRST_TILE_A.c());
  d("RYDY_WIN_FIRST_TILE_A : %c\n", RYDY_WIN_FIRST_TILE_A.c());
  d("NOPA_WIN_MODE_B       : %c\n", NOPA_WIN_MODE_B.c());
  d("SOVY_WIN_FIRST_TILE_B : %c\n", SOVY_WIN_FIRST_TILE_B.c());
  d("REJO_WY_MATCH_LATCH   : %c\n", REJO_WY_MATCH_LATCH.c());
  d("SARY_WY_MATCH         : %c\n", SARY_WY_MATCH.c());
  d("RYFA_FETCHn_A         : %c\n", RYFA_FETCHn_A.c());
  d("RENE_FETCHn_B         : %c\n", RENE_FETCHn_B.c());
  d("PYCO_WX_MATCH_A       : %c\n", PYCO_WX_MATCH_A.c());
  d("NUNU_WX_MATCH_B       : %c\n", NUNU_WX_MATCH_B.c());
  d("\n");
}

//------------------------------------------------------------------------------

void PixelPipe::tick(const SchematicTop& top) {
  _XENA_STORE_MATCHn = top.sprite_store.XENA_STORE_MATCHn();
}

//------------------------------------------------------------------------------

void PixelPipe::tock(SchematicTop& top, CpuBus& cpu_bus) {
  wire GND = 0;

  /*#p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor3(top.sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), top.clk_reg.MYVO_AxCxExGx());
  /*#p24.SOCY*/ wire _SOCY_WIN_HITn = not1(TOMU_WIN_HITp());
  /*#p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and3(_SOCY_WIN_HITn, top.tile_fetcher._POKY_PRELOAD_LATCHp.qp(), _VYBO_PIX_CLK_xBxDxFxH);
  /*#p24.SEGU*/ wire _SEGU_CLKPIPEn = not1(_TYFA_CLKPIPEp_xBxDxFxH);
  /*#p24.SACU*/ wire _SACU_CLKPIPEp = or2(_SEGU_CLKPIPEn, _ROXY_SCX_FINE_MATCH_LATCHn.qp()); // Schematic wrong, this is OR

  //----------------------------------------
  // XYMU is the main "we're rendering" flag

  {
    /*#p21.TADY*/ wire TADY_LINE_START_RSTn = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.TOFU_VID_RSTp());
    /*#p21.VOGA*/ _VOGA_RENDER_DONEp        = dff17_B(top.clk_reg.ALET_xBxDxFxH(), TADY_LINE_START_RSTn, WODU_RENDER_DONEp());
    /*#p21.WEGO*/ wire WEGO_RENDER_DONEp    = or2(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONEp.qp());
    /*#p21.XYMU*/ _XYMU_RENDERINGp          = nor_latch(top.AVAP_RENDER_START_TRIGp(), WEGO_RENDER_DONEp);
  }

  //----------------------------------------
  // Pixel counter, has carry lookahead because this can increment every tcycle

  {
    /*p21.TADY*/ wire _TADY_LINE_START_RST = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.TOFU_VID_RSTp());

    /*p21.RYBO*/ wire _RYBO = xor2_gnd(XEHO_X0p.qp(), SAVY_X1p.qp()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*p21.XUKE*/ wire _XUKE = and2(XEHO_X0p.qp(), SAVY_X1p.qp());

    /*p21.XYLE*/ wire _XYLE = and2(XODU_X2p.qp(), _XUKE);
    /*p21.XEGY*/ wire _XEGY = xor2_gnd(XODU_X2p.qp(), _XUKE); // feet facing gnd
    /*p21.XORA*/ wire _XORA = xor2_gnd(XYDO_X3p.qp(), _XYLE); // feet facing gnd

    /*p21.XEHO*/ XEHO_X0p = dff17_AB(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, XEHO_X0p.qn());
    /*p21.SAVY*/ SAVY_X1p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _RYBO);
    /*p21.XODU*/ XODU_X2p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XEGY);
    /*p21.XYDO*/ XYDO_X3p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XORA);

    /*p21.SAKE*/ wire _SAKE = xor2_gnd(TUHU_X4p.qp(), TUKY_X5p.qp());
    /*p21.TYBA*/ wire _TYBA = and2(TUHU_X4p.qp(), TUKY_X5p.qp());
    /*p21.SURY*/ wire _SURY = and2(TAKO_X6p.qp(), _TYBA);
    /*p21.TYGE*/ wire _TYGE = xor2_gnd(TAKO_X6p.qp(), _TYBA);
    /*p21.ROKU*/ wire _ROKU = xor2_gnd(SYBE_X7p.qp(), _SURY);

    /*p24.TOCA*/ wire _TOCA_CLKPIPE_HI = not1(XYDO_X3p.qp());
    /*p21.TUHU*/ TUHU_X4p = dff17_AB(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, TUHU_X4p.qn());
    /*p21.TUKY*/ TUKY_X5p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _SAKE);
    /*p21.TAKO*/ TAKO_X6p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _TYGE);
    /*p21.SYBE*/ SYBE_X7p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _ROKU);
  }

  //----------------------------------------
  // LCD pins that are controlled by the pixel counter

  {
    /*#p21.XAJO*/ wire _XAJO_X_009p = and2(XEHO_X0p.qp(), XYDO_X3p.qp());
    /*#p21.WEGO*/ wire _WEGO_LINE_END_RSTp = or2(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONEp.qp());
    /*#p21.WUSA*/ _WUSA_LCD_CLOCK_GATE = nor_latch(_XAJO_X_009p, _WEGO_LINE_END_RSTp);
    /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(_WUSA_LCD_CLOCK_GATE.qp(), _SACU_CLKPIPEp);
    /*#p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(_NYZE_SCX_FINE_MATCH_B.qn(), _PUXA_SCX_FINE_MATCH_A.qp());
    /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
    /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);

    top.LCD_PIN_CLOCK = _RYPO_LCD_CLOCK;
  }

  {
    // LCD horizontal sync pin latch

    // FIXME I don't know why ROXO has to be inverted here but it extends hblank by one phase, which
    // seems to be correct and makes it match the trace. With that change, HBLANK is 30 phases.

    // PAHO := dff17(ROXO, XYMU, XYDO_Q)   // def dff17

    /*#p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /* p24.PAHO*/ _PAHO_X_8_SYNC = dff17_B(!_ROXO_CLKPIPEp, _XYMU_RENDERINGp.qp(), XYDO_X3p.qp());
    
    /*#p24.POFY*/ wire POFY_HSYNCp = not1(RUJU_HSYNCn.qp());
    /*#p24.RUZE*/ wire RUZE_HSYNCn = not1(POFY_HSYNCp);

    /*#p24.RUJU*/ RUJU_HSYNCn = or3(_PAHO_X_8_SYNC.qp(), top.clk_reg.TOFU_VID_RSTp(), POME_HSYNCn.qp());
    /*#p24.POME*/ POME_HSYNCn = nor2(top.AVAP_RENDER_START_TRIGp(), POFY_HSYNCp);

    top.LCD_PIN_HSYNC = RUZE_HSYNCn;
  }

  //----------------------------------------
  // Fine counter.

  {
    /*#p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(_XYMU_RENDERINGp.qp());
    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(_RUBU_FINE_CNT2.qp(), _ROGA_FINE_CNT1.qp(), _RYKU_FINE_CNT0.qp());
    /*#p27.PECU*/ wire _PECU_FINE_CLK = nand2(_ROXO_CLKPIPEp, _ROZE_FINE_COUNT_7n);
    /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn, top.TEVO_FETCH_TRIGp());

    /*#p27.RYKU*/ _RYKU_FINE_CNT0 = dff17_AB(_PECU_FINE_CLK,       _PASO_FINE_RST, _RYKU_FINE_CNT0.qn());
    /*#p27.ROGA*/ _ROGA_FINE_CNT1 = dff17_AB(_RYKU_FINE_CNT0.qn(), _PASO_FINE_RST, _ROGA_FINE_CNT1.qn());
    /*#p27.RUBU*/ _RUBU_FINE_CNT2 = dff17_AB(_ROGA_FINE_CNT1.qn(), _PASO_FINE_RST, _RUBU_FINE_CNT2.qn());
  }

  //----------------------------------------

  {
    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp = xor2_vcc(DATY_SCX0.qn(), _RYKU_FINE_CNT0.qp());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp = xor2_vcc(DUZU_SCX1.qn(), _ROGA_FINE_CNT1.qp());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp = xor2_vcc(CYXU_SCX2.qn(), _RUBU_FINE_CNT2.qp());

    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn = nand4(_ROXY_SCX_FINE_MATCH_LATCHn.qp(), _SUHA_SCX_FINE_MATCHp, _SYBY_SCX_FINE_MATCHp, _SOZU_SCX_FINE_MATCHp);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp = not1(_RONE_SCX_FINE_MATCHn);

    /* p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*p 27.MOXE*/ wire MOXE_AxCxExGx = not1(top.clk_reg.ALET_xBxDxFxH());
    /*#p27.PUXA*/ _PUXA_SCX_FINE_MATCH_A = dff17_B(_ROXO_CLKPIPEp, _XYMU_RENDERINGp.qp(), _POHU_SCX_FINE_MATCHp);
    /*#p27.NYZE*/ _NYZE_SCX_FINE_MATCH_B = dff17_A(MOXE_AxCxExGx,  _XYMU_RENDERINGp.qp(), _PUXA_SCX_FINE_MATCH_A.qp());

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(_XYMU_RENDERINGp.qp());
    /*#p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(_NYZE_SCX_FINE_MATCH_B.qn(), _PUXA_SCX_FINE_MATCH_A.qp());
    /*#p27.ROXY*/ _ROXY_SCX_FINE_MATCH_LATCHn = nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCHpe);
  }

  //----------------------------------------
  // Window matcher

  {
    /*p27.NAZE*/ wire NAZE_WY_MATCH0 = xnor2_vcc(top.lcd_reg.MUWY_Y0p.qp(), NESO_WY0.qp());
    /*p27.PEBO*/ wire PEBO_WY_MATCH1 = xnor2_vcc(top.lcd_reg.MYRO_Y1p.qp(), NYRO_WY1.qp());
    /*p27.POMO*/ wire POMO_WY_MATCH2 = xnor2_vcc(top.lcd_reg.LEXA_Y2p.qp(), NAGA_WY2.qp());
    /*p27.NEVU*/ wire NEVU_WY_MATCH3 = xnor2_vcc(top.lcd_reg.LYDO_Y3p.qp(), MELA_WY3.qp());
    /*p27.NOJO*/ wire NOJO_WY_MATCH4 = xnor2_vcc(top.lcd_reg.LOVU_Y4p.qp(), NULO_WY4.qp());
    /*p27.PAGA*/ wire PAGA_WY_MATCH5 = xnor2_vcc(top.lcd_reg.LEMA_Y5p.qp(), NENE_WY5.qp());
    /*p27.PEZO*/ wire PEZO_WY_MATCH6 = xnor2_vcc(top.lcd_reg.MATO_Y6p.qp(), NUKA_WY6.qp());
    /*p27.NUPA*/ wire NUPA_WY_MATCH7 = xnor2_vcc(top.lcd_reg.LAFO_Y7p.qp(), NAFU_WY7.qp());

    /*#p27.PALO*/ wire PALO_WY_MATCH_HIn  = nand5(WYMO_LCDC_WINENn.qn(), NOJO_WY_MATCH4, PAGA_WY_MATCH5, PEZO_WY_MATCH6, NUPA_WY_MATCH7);
    /* p27.NELE*/ wire NELE_WY_MATCH_HI   = not1(PALO_WY_MATCH_HIn);
    /* p27.PAFU*/ wire PAFU_WY_MATCHn     = nand5(NELE_WY_MATCH_HI, NAZE_WY_MATCH0, PEBO_WY_MATCH1, POMO_WY_MATCH2, NEVU_WY_MATCH3);
    /* p27.ROGE*/ ROGE_WY_MATCHp = not1(PAFU_WY_MATCHn);
    /* p27.REPU*/ wire REPU_VBLANK_RSTp = or2(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /* p27.SARY*/ SARY_WY_MATCH = dff17_B(top.clk_reg.TALU_ABCDxxxx(), top.clk_reg.XAPO_VID_RSTn(), ROGE_WY_MATCHp);
    /* p27.REJO*/ REJO_WY_MATCH_LATCH = nor_latch(SARY_WY_MATCH.qp(), REPU_VBLANK_RSTp);
  }

  {
    /*p27.MYLO*/ wire MYLO_WX_MATCH0 = xnor2_vcc(XEHO_X0p.qp(), MYPA_WX0.qp());
    /*p27.PUWU*/ wire PUWU_WX_MATCH1 = xnor2_vcc(SAVY_X1p.qp(), NOFE_WX1.qp());
    /*p27.PUHO*/ wire PUHO_WX_MATCH2 = xnor2_vcc(XODU_X2p.qp(), NOKE_WX2.qp());
    /*p27.NYTU*/ wire NYTU_WX_MATCH3 = xnor2_vcc(XYDO_X3p.qp(), MEBY_WX3.qp());
    /*p27.NEZO*/ wire NEZO_WX_MATCH4 = xnor2_vcc(TUHU_X4p.qp(), MYPU_WX4.qp());
    /*p27.NORY*/ wire NORY_WX_MATCH5 = xnor2_vcc(TUKY_X5p.qp(), MYCE_WX5.qp());
    /*p27.NONO*/ wire NONO_WX_MATCH6 = xnor2_vcc(TAKO_X6p.qp(), MUVO_WX6.qp());
    /*p27.PASE*/ wire PASE_WX_MATCH7 = xnor2_vcc(SYBE_X7p.qp(), NUKU_WX7.qp());

    /*p27.PUKY*/ wire PUKY_WX_MATCH_HIn  = nand5(REJO_WY_MATCH_LATCH.qp(), NEZO_WX_MATCH4, NORY_WX_MATCH5, NONO_WX_MATCH6, PASE_WX_MATCH7);
    /*p27.NUFA*/ wire NUFA_WX_MATCH_HI   = not1(PUKY_WX_MATCH_HIn);
    /*p27.NOGY*/ wire NOGY_WX_MATCHn     = nand5(NUFA_WX_MATCH_HI, MYLO_WX_MATCH0, PUWU_WX_MATCH1, PUHO_WX_MATCH2, NYTU_WX_MATCH3);
    /*p27.NUKO*/ NUKO_WX_MATCHp = not1(NOGY_WX_MATCHn);

    // This trigger fires on the pixel _at_ WX
    /*p27.ROCO*/ wire ROCO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*p27.PYCO*/ PYCO_WX_MATCH_A = dff17_B(ROCO_CLKPIPEp, top.clk_reg.XAPO_VID_RSTn(), NUKO_WX_MATCHp);
    /*p27.NUNU*/ NUNU_WX_MATCH_B = dff17_B(top.clk_reg.MEHE_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), PYCO_WX_MATCH_A.qp());
  }

  //----------------------------------------
  // Window sequencer

  /* p28.ABAF*/ wire ABAF_LINE_END_Bn = not1(top.lcd_reg._CATU_LINE_P910.qp());
  /* p28.BYHA*/ wire BYHA_VID_LINE_TRIGn = or_and3(top.lcd_reg._ANEL_LINE_P000.qp(), ABAF_LINE_END_Bn, top.lcd_reg._ABEZ_VID_RSTn);
  /* p28.ATEJ*/ wire ATEJ_VID_LINE_END_TRIGp = not1(BYHA_VID_LINE_TRIGn);
  /* p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not1(ATEJ_VID_LINE_END_TRIGp);
  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(WYMO_LCDC_WINENn.qn(), XAHY_VID_LINE_TRIG_d4n, top.clk_reg.XAPO_VID_RSTn());

  {
    /* p27.PYNU*/ PYNU_WIN_MODE_A = nor_latch(NUNU_WX_MATCH_B.qp(), XOFO_WIN_RSTp);
    /* p27.NOPA*/ NOPA_WIN_MODE_B = dff17_A(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), PYNU_WIN_MODE_A.qp());

    /* p27.NUNY*/ wire NUNY_WX_MATCH_TRIGp = and2(PYNU_WIN_MODE_A.qp(), NOPA_WIN_MODE_B.qn());
    /*#p27.PUKU*/ PUKU_WIN_FIRST_TILE_A = nor2(NUNY_WX_MATCH_TRIGp, RYDY_WIN_FIRST_TILE_A.qp());
    /*#p27.RYDY*/ RYDY_WIN_FIRST_TILE_A = nor3(PUKU_WIN_FIRST_TILE_A.qp(), top.tile_fetcher._PORY_FETCH_DONE_P12.qp(), top.clk_reg.PYRY_VID_RSTp());
    /* p27.SOVY*/ SOVY_WIN_FIRST_TILE_B = dff17_B(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), RYDY_WIN_FIRST_TILE_A.qp());
  }

  //----------------------------------------
  // Window x coordinate
  {
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not1(PYNU_WIN_MODE_A.qp());
    /*p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);

    /*p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(_RYKU_FINE_CNT0.qp(), _ROGA_FINE_CNT1.qp(), _RUBU_FINE_CNT2.qp());

    /*p27.PANY*/ wire PANY_FETCHn = nor2(NUKO_WX_MATCHp, ROZE_FINE_COUNT_7n);

    /*p27.RYFA*/ RYFA_FETCHn_A = dff17_AB(_SEGU_CLKPIPEn, XYMU_RENDERINGp(), PANY_FETCHn);
    /*p27.RENE*/ RENE_FETCHn_B = dff17_B (top.clk_reg.ALET_xBxDxFxH(), XYMU_RENDERINGp(), RYFA_FETCHn_A.qp());
    /*p27.SEKO*/ wire SEKO_FETCH_TRIGp = nor2(RYFA_FETCHn_A.qn(), RENE_FETCHn_B.qp());

    /*p27.TEVO*/ wire TEVO_FETCH_TRIGp  = or3(SEKO_FETCH_TRIGp, top.pix_pipe.SUZU_WIN_FIRST_TILEne(), top.TAVE_PRELOAD_DONE_TRIGp()); // Schematic wrong, this is OR
    /*p27.VETU*/ wire VETU_WIN_MAP_CLK = and2(TEVO_FETCH_TRIGp, PORE_WIN_MODEp);

    /*p27.XACO*/ wire XACO_WIN_RSTn = not1(XOFO_WIN_RSTp);

    /*p27.WYKA*/ WYKA_WIN_X3 = dff17_AB(VETU_WIN_MAP_CLK,  XACO_WIN_RSTn, WYKA_WIN_X3.qn());
    /*p27.WODY*/ WODY_WIN_X4 = dff17_AB(WYKA_WIN_X3.qn(),  XACO_WIN_RSTn, WODY_WIN_X4.qn());
    /*p27.WOBO*/ WOBO_WIN_X5 = dff17_AB(WODY_WIN_X4.qn(),  XACO_WIN_RSTn, WOBO_WIN_X5.qn());
    /*p27.WYKO*/ WYKO_WIN_X6 = dff17_AB(WOBO_WIN_X5.qn(),  XACO_WIN_RSTn, WYKO_WIN_X6.qn());
    /*p27.XOLO*/ XOLO_WIN_X7 = dff17_AB(WYKO_WIN_X6.qn(),  XACO_WIN_RSTn, XOLO_WIN_X7.qn());
  }

  //----------------------------------------
  // Window y coordinate
  {
    /*p27.WAZY*/ wire WAZY_WIN_Y_CLKp = not1(PORE_WIN_MODEp());
    /*p27.REPU*/ wire REPU_WIN_Y_RSTp = or2(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /*p27.SYNY*/ wire SYNY_WIN_Y_RSTn = not1(REPU_WIN_Y_RSTp);

    // Every time we leave win mode we increment win_y
    /*p27.VYNO*/ VYNO_WIN_Y0 = dff17_AB(WAZY_WIN_Y_CLKp,  SYNY_WIN_Y_RSTn, VYNO_WIN_Y0.qn());
    /*p27.VUJO*/ VUJO_WIN_Y1 = dff17_AB(VYNO_WIN_Y0.qn(), SYNY_WIN_Y_RSTn, VUJO_WIN_Y1.qn());
    /*p27.VYMU*/ VYMU_WIN_Y2 = dff17_AB(VUJO_WIN_Y1.qn(), SYNY_WIN_Y_RSTn, VYMU_WIN_Y2.qn());
    /*p27.TUFU*/ TUFU_WIN_Y3 = dff17_AB(VYMU_WIN_Y2.qn(), SYNY_WIN_Y_RSTn, TUFU_WIN_Y3.qn());
    /*p27.TAXA*/ TAXA_WIN_Y4 = dff17_AB(TUFU_WIN_Y3.qn(), SYNY_WIN_Y_RSTn, TAXA_WIN_Y4.qn());
    /*p27.TOZO*/ TOZO_WIN_Y5 = dff17_AB(TAXA_WIN_Y4.qn(), SYNY_WIN_Y_RSTn, TOZO_WIN_Y5.qn());
    /*p27.TATE*/ TATE_WIN_Y6 = dff17_AB(TOZO_WIN_Y5.qn(), SYNY_WIN_Y_RSTn, TATE_WIN_Y6.qn());
    /*p27.TEKE*/ TEKE_WIN_Y7 = dff17_AB(TATE_WIN_Y6.qn(), SYNY_WIN_Y_RSTn, TEKE_WIN_Y7.qn());
  }

  //----------------------------------------
  // Background pipes

  // Background pipes are loaded at phase 16 of tile fetch

  {
#if 0

    if (TILE_DA0) {
      /*p32.MYDE*/ MYDE_BG_PIPE_A0 = dff22(_SACU_CLKPIPEp, 1, 0, GND);
    }
    else {
      /*p32.MYDE*/ MYDE_BG_PIPE_A0 = dff22(_SACU_CLKPIPEp, 0, 1, GND);
    }

    // -> TILE_DA0 stores inverted pixels somehow

#endif

    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOAD = not1(top.NYXU_FETCH_TRIGn());
    /*p32.LUHE*/ wire BG_PIX_A0n = not1(top.vram_bus.LEGU_TILE_DA0n.qn());
    /*p32.NOLY*/ wire BG_PIX_A1n = not1(top.vram_bus.NUDU_TILE_DA1n.qn());
    /*p32.LEKE*/ wire BG_PIX_A2n = not1(top.vram_bus.MUKU_TILE_DA2n.qn());
    /*p32.LOMY*/ wire BG_PIX_A3n = not1(top.vram_bus.LUZO_TILE_DA3n.qn());
    /*p32.LALA*/ wire BG_PIX_A4n = not1(top.vram_bus.MEGU_TILE_DA4n.qn());
    /*p32.LOXA*/ wire BG_PIX_A5n = not1(top.vram_bus.MYJY_TILE_DA5n.qn());
    /*p32.NEZE*/ wire BG_PIX_A6n = not1(top.vram_bus.NASA_TILE_DA6n.qn());
    /*p32.NOBO*/ wire BG_PIX_A7n = not1(top.vram_bus.NEFO_TILE_DA7n.qn());

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.LEGU_TILE_DA0n.qn());
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NUDU_TILE_DA1n.qn());
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MUKU_TILE_DA2n.qn());
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.LUZO_TILE_DA3n.qn());
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MEGU_TILE_DA4n.qn());
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MYJY_TILE_DA5n.qn());
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NASA_TILE_DA6n.qn());
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NEFO_TILE_DA7n.qn());

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A0n);
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A1n);
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A2n);
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A3n);
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A4n);
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A5n);
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A6n);
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand2(_LOZE_PIPE_A_LOAD, BG_PIX_A7n);

    /*p32.MYDE*/ MYDE_BG_PIPE_A0 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET0, BG_PIPE_A_RST0, GND);
    /*p32.NOZO*/ NOZO_BG_PIPE_A1 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET1, BG_PIPE_A_RST1, MYDE_BG_PIPE_A0.qp());
    /*p32.MOJU*/ MOJU_BG_PIPE_A2 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET2, BG_PIPE_A_RST2, NOZO_BG_PIPE_A1.qp());
    /*p32.MACU*/ MACU_BG_PIPE_A3 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET3, BG_PIPE_A_RST3, MOJU_BG_PIPE_A2.qp());
    /*p32.NEPO*/ NEPO_BG_PIPE_A4 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET4, BG_PIPE_A_RST4, MACU_BG_PIPE_A3.qp());
    /*p32.MODU*/ MODU_BG_PIPE_A5 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET5, BG_PIPE_A_RST5, NEPO_BG_PIPE_A4.qp());
    /*p32.NEDA*/ NEDA_BG_PIPE_A6 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET6, BG_PIPE_A_RST6, MODU_BG_PIPE_A5.qp());
    /*p32.PYBO*/ PYBO_BG_PIPE_A7 = dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET7, BG_PIPE_A_RST7, NEDA_BG_PIPE_A6.qp());
  }

  {
    /*p32.LUXA*/ wire _LUXA_PIPE_B_LOAD = not1(top.NYXU_FETCH_TRIGn());

    /*p32.TOSA*/ wire BG_PIX_B0n = not1(top.vram_bus.RAWU_TILE_DB0n.qn());
    /*p32.RUCO*/ wire BG_PIX_B1n = not1(top.vram_bus.POZO_TILE_DB1n.qn());
    /*p32.TYCE*/ wire BG_PIX_B2n = not1(top.vram_bus.PYZO_TILE_DB2n.qn());
    /*p32.REVY*/ wire BG_PIX_B3n = not1(top.vram_bus.POXA_TILE_DB3n.qn());
    /*p32.RYGA*/ wire BG_PIX_B4n = not1(top.vram_bus.PULO_TILE_DB4n.qn());
    /*p32.RYLE*/ wire BG_PIX_B5n = not1(top.vram_bus.POJU_TILE_DB5n.qn());
    /*p32.RAPU*/ wire BG_PIX_B6n = not1(top.vram_bus.POWY_TILE_DB6n.qn());
    /*p32.SOJA*/ wire BG_PIX_B7n = not1(top.vram_bus.PYJU_TILE_DB7n.qn());

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.RAWU_TILE_DB0n.qn());
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POZO_TILE_DB1n.qn());
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PYZO_TILE_DB2n.qn());
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POXA_TILE_DB3n.qn());
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PULO_TILE_DB4n.qn());
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POJU_TILE_DB5n.qn());
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POWY_TILE_DB6n.qn());
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PYJU_TILE_DB7n.qn());

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B0n);
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B1n);
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B2n);
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B3n);
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B4n);
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B5n);
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B6n);
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand2(_LUXA_PIPE_B_LOAD, BG_PIX_B7n);

    /*p32.TOMY*/ TOMY_BG_PIPE_B0 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET0, BG_PIPE_B_RST0, GND);
    /*p32.TACA*/ TACA_BG_PIPE_B1 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET1, BG_PIPE_B_RST1, TOMY_BG_PIPE_B0.qp());
    /*p32.SADY*/ SADY_BG_PIPE_B2 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET2, BG_PIPE_B_RST2, TACA_BG_PIPE_B1.qp());
    /*p32.RYSA*/ RYSA_BG_PIPE_B3 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET3, BG_PIPE_B_RST3, SADY_BG_PIPE_B2.qp());
    /*p32.SOBO*/ SOBO_BG_PIPE_B4 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET4, BG_PIPE_B_RST4, RYSA_BG_PIPE_B3.qp());
    /*p32.SETU*/ SETU_BG_PIPE_B5 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET5, BG_PIPE_B_RST5, SOBO_BG_PIPE_B4.qp());
    /*p32.RALU*/ RALU_BG_PIPE_B6 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET6, BG_PIPE_B_RST6, SETU_BG_PIPE_B5.qp());
    /*p32.SOHU*/ SOHU_BG_PIPE_B7 = dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET7, BG_PIPE_B_RST7, RALU_BG_PIPE_B6.qp());
  }

  //----------------------------------------
  // Sprite pipes

  {
    /*p29.TYNO*/ wire TYNO = nand3(top.sprite_fetcher._TOXE_SFETCH_S0.qp(), top.sprite_fetcher._SEBA_SFETCH_S1_D5.qp(), top.sprite_fetcher._VONU_SFETCH_S1_D4.qp());
    /*p29.VUSA*/ wire VUSA_SPRITE_DONEn  = or2(top.sprite_fetcher._TYFO_SFETCH_S0_D1.qn(), TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONEp  = not1(VUSA_SPRITE_DONEn);
    /*p29.XEFY*/ wire XEPY_SPRITE_DONEn  = not1(WUTY_SPRITE_DONEp);

    /*p34.MEFU*/ wire MEFU_SPRITE_MASK0n = or3(XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp(), NURO_SPR_PIPE_A0.qp()); // def or
    /*p34.MEVE*/ wire MEVE_SPRITE_MASK1n = or3(XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_B1.qp(), MASO_SPR_PIPE_A1.qp());
    /*p34.MYZO*/ wire MYZO_SPRITE_MASK2n = or3(XEPY_SPRITE_DONEn, NATY_SPR_PIPE_B2.qp(), LEFE_SPR_PIPE_A2.qp());
    /*p34.RUDA*/ wire RUDA_SPRITE_MASK3n = or3(XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_B3.qp(), LESU_SPR_PIPE_A3.qp());
    /*p34.VOTO*/ wire VOTO_SPRITE_MASK4n = or3(XEPY_SPRITE_DONEn, VARE_SPR_PIPE_B4.qp(), WYHO_SPR_PIPE_A4.qp());
    /*p34.VYSA*/ wire VYSA_SPRITE_MASK5n = or3(XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_B5.qp(), WORA_SPR_PIPE_A5.qp());
    /*p34.TORY*/ wire TORY_SPRITE_MASK6n = or3(XEPY_SPRITE_DONEn, VANU_SPR_PIPE_B6.qp(), VAFO_SPR_PIPE_A6.qp());
    /*p34.WOPE*/ wire WOPE_SPRITE_MASK7n = or3(XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_B7.qp(), WUFY_SPR_PIPE_A7.qp());

    // The mask is 1 where there are _no_ sprite pixels
    /*p34.LESY*/ wire LESY_SPRITE_MASK0p = not1(MEFU_SPRITE_MASK0n);
    /*p34.LOTA*/ wire LOTA_SPRITE_MASK1p = not1(MEVE_SPRITE_MASK1n);
    /*p34.LYKU*/ wire LYKU_SPRITE_MASK2p = not1(MYZO_SPRITE_MASK2n);
    /*p34.ROBY*/ wire ROBY_SPRITE_MASK3p = not1(RUDA_SPRITE_MASK3n);
    /*p34.TYTA*/ wire TYTA_SPRITE_MASK4p = not1(VOTO_SPRITE_MASK4n);
    /*p34.TYCO*/ wire TYCO_SPRITE_MASK5p = not1(VYSA_SPRITE_MASK5n);
    /*p34.SOKA*/ wire SOKA_SPRITE_MASK6p = not1(TORY_SPRITE_MASK6n);
    /*p34.XOVU*/ wire XOVU_SPRITE_MASK7p = not1(WOPE_SPRITE_MASK7n);

    // Sprite pipe A
    {
      /*p33.RATA*/ wire RATA_SPR_PIX_DA0n = not1(top.vram_bus.REWO_SPRITE_DA0n.qn());
      /*p33.NUCA*/ wire NUCA_SPR_PIX_DA1n = not1(top.vram_bus.PEBA_SPRITE_DA1n.qn());
      /*p33.LASE*/ wire LASE_SPR_PIX_DA2n = not1(top.vram_bus.MOFO_SPRITE_DA2n.qn());
      /*p33.LUBO*/ wire LUBO_SPR_PIX_DA3n = not1(top.vram_bus.PUDU_SPRITE_DA3n.qn());
      /*p33.WERY*/ wire WERY_SPR_PIX_DA4n = not1(top.vram_bus.SAJA_SPRITE_DA4n.qn());
      /*p33.WURA*/ wire WURA_SPR_PIX_DA5n = not1(top.vram_bus.SUNY_SPRITE_DA5n.qn());
      /*p33.SULU*/ wire SULU_SPR_PIX_DA6n = not1(top.vram_bus.SEMO_SPRITE_DA6n.qn());
      /*p33.WAMY*/ wire WAMY_SPR_PIX_DA7n = not1(top.vram_bus.SEGA_SPRITE_DA7n.qn());

      /*p33.PABE*/ wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, top.vram_bus.REWO_SPRITE_DA0n.qn());
      /*p33.MYTO*/ wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, top.vram_bus.PEBA_SPRITE_DA1n.qn());
      /*p33.LELA*/ wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, top.vram_bus.MOFO_SPRITE_DA2n.qn());
      /*p33.MAME*/ wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, top.vram_bus.PUDU_SPRITE_DA3n.qn());
      /*p33.VEXU*/ wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, top.vram_bus.SAJA_SPRITE_DA4n.qn());
      /*p33.VABY*/ wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, top.vram_bus.SUNY_SPRITE_DA5n.qn());
      /*p33.TUXA*/ wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, top.vram_bus.SEMO_SPRITE_DA6n.qn());
      /*p33.VUNE*/ wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, top.vram_bus.SEGA_SPRITE_DA7n.qn());

      /*p33.PYZU*/ wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
      /*p33.MADA*/ wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
      /*p33.LYDE*/ wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
      /*p33.LUFY*/ wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
      /*p33.XATO*/ wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
      /*p33.XEXU*/ wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
      /*p33.TUPE*/ wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
      /*p33.XYVE*/ wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

      /*p33.NURO*/ NURO_SPR_PIPE_A0 = dff22(_SACU_CLKPIPEp, PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0, GND);
      /*p33.MASO*/ MASO_SPR_PIPE_A1 = dff22(_SACU_CLKPIPEp, MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1, NURO_SPR_PIPE_A0.qp());
      /*p33.LEFE*/ LEFE_SPR_PIPE_A2 = dff22(_SACU_CLKPIPEp, LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2, MASO_SPR_PIPE_A1.qp());
      /*p33.LESU*/ LESU_SPR_PIPE_A3 = dff22(_SACU_CLKPIPEp, MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3, LEFE_SPR_PIPE_A2.qp());
      /*p33.WYHO*/ WYHO_SPR_PIPE_A4 = dff22(_SACU_CLKPIPEp, VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4, LESU_SPR_PIPE_A3.qp());
      /*p33.WORA*/ WORA_SPR_PIPE_A5 = dff22(_SACU_CLKPIPEp, VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5, WYHO_SPR_PIPE_A4.qp());
      /*p33.VAFO*/ VAFO_SPR_PIPE_A6 = dff22(_SACU_CLKPIPEp, TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6, WORA_SPR_PIPE_A5.qp());
      /*p33.WUFY*/ WUFY_SPR_PIPE_A7 = dff22(_SACU_CLKPIPEp, VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7, VAFO_SPR_PIPE_A6.qp());
    }

    // Sprite pipe B
    {
      // lol, not(REGn.qn()) = !REG

#if 0

      if (nor(XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp(), NURO_SPR_PIPE_A0.qp())) {
        NYLU_SPR_PIPE_B0 = dff22(_SACU_CLKPIPEp, !SPRITE_DB0p, !SPRITE_DB0n, GND);
      }
      else {
        NYLU_SPR_PIPE_B0 = dff22(_SACU_CLKPIPEp, 1, 1, GND);
      }
#endif

      /*p33.LOZA*/ wire LOZA_SPR_PIX_DB0n = not1(top.vram_bus.PEFO_SPRITE_DB0n.qn());
      /*p33.SYBO*/ wire SYBO_SPR_PIX_DB1n = not1(top.vram_bus.ROKA_SPRITE_DB1n.qn());
      /*p33.LUMO*/ wire LUMO_SPR_PIX_DB2n = not1(top.vram_bus.MYTU_SPRITE_DB2n.qn());
      /*p33.SOLO*/ wire SOLO_SPR_PIX_DB3n = not1(top.vram_bus.RAMU_SPRITE_DB3n.qn());
      /*p33.VOBY*/ wire VOBY_SPR_PIX_DB4n = not1(top.vram_bus.SELE_SPRITE_DB4n.qn());
      /*p33.WYCO*/ wire WYCO_SPR_PIX_DB5n = not1(top.vram_bus.SUTO_SPRITE_DB5n.qn());
      /*p33.SERY*/ wire SERY_SPR_PIX_DB6n = not1(top.vram_bus.RAMA_SPRITE_DB6n.qn());
      /*p33.SELU*/ wire SELU_SPR_PIX_DB7n = not1(top.vram_bus.RYDU_SPRITE_DB7n.qn());

      /*p33.MEZU*/ wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, top.vram_bus.PEFO_SPRITE_DB0n.qn());
      /*p33.RUSY*/ wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, top.vram_bus.ROKA_SPRITE_DB1n.qn());
      /*p33.MYXA*/ wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, top.vram_bus.MYTU_SPRITE_DB2n.qn());
      /*p33.RANO*/ wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, top.vram_bus.RAMU_SPRITE_DB3n.qn());
      /*p33.TYGA*/ wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, top.vram_bus.SELE_SPRITE_DB4n.qn());
      /*p33.VUME*/ wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, top.vram_bus.SUTO_SPRITE_DB5n.qn());
      /*p33.TAPO*/ wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, top.vram_bus.RAMA_SPRITE_DB6n.qn());
      /*p33.TESO*/ wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, top.vram_bus.RYDU_SPRITE_DB7n.qn());

      /*p33.MOFY*/ wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
      /*p33.RUCA*/ wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
      /*p33.MAJO*/ wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
      /*p33.REHU*/ wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
      /*p33.WAXO*/ wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
      /*p33.XOLE*/ wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
      /*p33.TABY*/ wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
      /*p33.TULA*/ wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

      /*p33.NYLU*/ NYLU_SPR_PIPE_B0 = dff22(_SACU_CLKPIPEp, MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0, GND);
      /*p33.PEFU*/ PEFU_SPR_PIPE_B1 = dff22(_SACU_CLKPIPEp, RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1, NYLU_SPR_PIPE_B0.qp());
      /*p33.NATY*/ NATY_SPR_PIPE_B2 = dff22(_SACU_CLKPIPEp, MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2, PEFU_SPR_PIPE_B1.qp());
      /*p33.PYJO*/ PYJO_SPR_PIPE_B3 = dff22(_SACU_CLKPIPEp, RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3, NATY_SPR_PIPE_B2.qp());
      /*p33.VARE*/ VARE_SPR_PIPE_B4 = dff22(_SACU_CLKPIPEp, TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4, PYJO_SPR_PIPE_B3.qp());
      /*p33.WEBA*/ WEBA_SPR_PIPE_B5 = dff22(_SACU_CLKPIPEp, VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5, VARE_SPR_PIPE_B4.qp());
      /*p33.VANU*/ VANU_SPR_PIPE_B6 = dff22(_SACU_CLKPIPEp, TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6, WEBA_SPR_PIPE_B5.qp());
      /*p33.VUPY*/ VUPY_SPR_PIPE_B7 = dff22(_SACU_CLKPIPEp, TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7, VANU_SPR_PIPE_B6.qp());
    }

    // Palette pipe
    {
      wire GOMO_OBP = top.oam_bus.GOMO_OAM_DB4n.qp();

      /*p34.SYPY*/ wire _SYPY = not1(GOMO_OBP);
      /*p34.TOTU*/ wire _TOTU = not1(GOMO_OBP);
      /*p34.NARO*/ wire _NARO = not1(GOMO_OBP);
      /*p34.WEXY*/ wire _WEXY = not1(GOMO_OBP);
      /*p34.RYZY*/ wire _RYZY = not1(GOMO_OBP);
      /*p34.RYFE*/ wire _RYFE = not1(GOMO_OBP);
      /*p34.LADY*/ wire _LADY = not1(GOMO_OBP);
      /*p34.LAFY*/ wire _LAFY = not1(GOMO_OBP);

      /*p34.PUME*/ wire _PUME_PAL_PIPE_SET0n = nand2(LESY_SPRITE_MASK0p, GOMO_OBP);
      /*p34.SORO*/ wire _SORO_PAL_PIPE_SET1n = nand2(LOTA_SPRITE_MASK1p, GOMO_OBP);
      /*p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n = nand2(LYKU_SPRITE_MASK2p, GOMO_OBP);
      /*p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n = nand2(ROBY_SPRITE_MASK3p, GOMO_OBP);
      /*p34.RORA*/ wire _RORA_PAL_PIPE_SET4n = nand2(TYTA_SPRITE_MASK4p, GOMO_OBP);
      /*p34.MENE*/ wire _MENE_PAL_PIPE_SET5n = nand2(TYCO_SPRITE_MASK5p, GOMO_OBP);
      /*p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n = nand2(SOKA_SPRITE_MASK6p, GOMO_OBP);
      /*p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n = nand2(XOVU_SPRITE_MASK7p, GOMO_OBP);

      /*p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n = nand2(LESY_SPRITE_MASK0p, _SYPY);
      /*p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n = nand2(LOTA_SPRITE_MASK1p, _TOTU);
      /*p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n = nand2(LYKU_SPRITE_MASK2p, _NARO);
      /*p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n = nand2(ROBY_SPRITE_MASK3p, _WEXY);
      /*p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n = nand2(TYTA_SPRITE_MASK4p, _RYZY);
      /*p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n = nand2(TYCO_SPRITE_MASK5p, _RYFE);
      /*p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n = nand2(SOKA_SPRITE_MASK6p, _LADY);
      /*p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n = nand2(XOVU_SPRITE_MASK7p, _LAFY);

      wire VYPO = 0;
      /*p34.RUGO*/ RUGO_PAL_PIPE_0 = dff22(_SACU_CLKPIPEp, _PUME_PAL_PIPE_SET0n, _SUCO_PAL_PIPE_RST0n, VYPO);
      /*p34.SATA*/ SATA_PAL_PIPE_1 = dff22(_SACU_CLKPIPEp, _SORO_PAL_PIPE_SET1n, _TAFA_PAL_PIPE_RST1n, RUGO_PAL_PIPE_0.qp());
      /*p34.ROSA*/ ROSA_PAL_PIPE_2 = dff22(_SACU_CLKPIPEp, _PAMO_PAL_PIPE_SET2n, _PYZY_PAL_PIPE_RST2n, SATA_PAL_PIPE_1.qp());
      /*p34.SOMY*/ SOMY_PAL_PIPE_3 = dff22(_SACU_CLKPIPEp, _SUKY_PAL_PIPE_SET3n, _TOWA_PAL_PIPE_RST3n, ROSA_PAL_PIPE_2.qp());
      /*p34.PALU*/ PALU_PAL_PIPE_4 = dff22(_SACU_CLKPIPEp, _RORA_PAL_PIPE_SET4n, _RUDU_PAL_PIPE_RST4n, SOMY_PAL_PIPE_3.qp());
      /*p34.NUKE*/ NUKE_PAL_PIPE_5 = dff22(_SACU_CLKPIPEp, _MENE_PAL_PIPE_SET5n, _PAZO_PAL_PIPE_RST5n, PALU_PAL_PIPE_4.qp());
      /*p34.MODA*/ MODA_PAL_PIPE_6 = dff22(_SACU_CLKPIPEp, _LUKE_PAL_PIPE_SET6n, _LOWA_PAL_PIPE_RST6n, NUKE_PAL_PIPE_5.qp());
      /*p34.LYME*/ LYME_PAL_PIPE_7 = dff22(_SACU_CLKPIPEp, _LAMY_PAL_PIPE_SET7n, _LUNU_PAL_PIPE_RST7n, MODA_PAL_PIPE_6.qp());
    }

    // Background mask pipe
    {
      wire DEPO = top.oam_bus.DEPO_OAM_DB7n.qp();

      /*p26.XOGA*/ wire _XOGA = not1(DEPO);
      /*p26.XURA*/ wire _XURA = not1(DEPO);
      /*p26.TAJO*/ wire _TAJO = not1(DEPO);
      /*p26.XENU*/ wire _XENU = not1(DEPO);
      /*p26.XYKE*/ wire _XYKE = not1(DEPO);
      /*p26.XABA*/ wire _XABA = not1(DEPO);
      /*p26.TAFU*/ wire _TAFU = not1(DEPO);
      /*p26.XUHO*/ wire _XUHO = not1(DEPO);

      /*p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, DEPO);
      /*p26.XALA*/ wire _XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, DEPO);
      /*p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, DEPO);
      /*p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, DEPO);
      /*p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, DEPO);
      /*p26.XELY*/ wire _XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, DEPO);
      /*p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, DEPO);
      /*p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, DEPO);

      /*p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, _XOGA);
      /*p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, _XURA);
      /*p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, _TAJO);
      /*p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, _XENU);
      /*p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, _XYKE);
      /*p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, _XABA);
      /*p26.TENA*/ wire _TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, _TAFU);
      /*p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, _XUHO);

      wire VYPO = 1;

      /*p26.VEZO*/ VEZO_MASK_PIPE_0 = dff22(_SACU_CLKPIPEp, _TEDE_MASK_PIPE_SET0, _WOKA_MASK_PIPE_RST0, VYPO);
      /*p26.WURU*/ WURU_MASK_PIPE_1 = dff22(_SACU_CLKPIPEp, _XALA_MASK_PIPE_SET1, _WEDE_MASK_PIPE_RST1, VEZO_MASK_PIPE_0.qp());
      /*p26.VOSA*/ VOSA_MASK_PIPE_2 = dff22(_SACU_CLKPIPEp, _TYRA_MASK_PIPE_SET2, _TUFO_MASK_PIPE_RST2, WURU_MASK_PIPE_1.qp());
      /*p26.WYFU*/ WYFU_MASK_PIPE_3 = dff22(_SACU_CLKPIPEp, _XYRU_MASK_PIPE_SET3, _WEVO_MASK_PIPE_RST3, VOSA_MASK_PIPE_2.qp());
      /*p26.XETE*/ XETE_MASK_PIPE_4 = dff22(_SACU_CLKPIPEp, _XUKU_MASK_PIPE_SET4, _WEDY_MASK_PIPE_RST4, WYFU_MASK_PIPE_3.qp());
      /*p26.WODA*/ WODA_MASK_PIPE_5 = dff22(_SACU_CLKPIPEp, _XELY_MASK_PIPE_SET5, _WUJA_MASK_PIPE_RST5, XETE_MASK_PIPE_4.qp());
      /*p26.VUMO*/ VUMO_MASK_PIPE_6 = dff22(_SACU_CLKPIPEp, _TYKO_MASK_PIPE_SET6, _TENA_MASK_PIPE_RST6, WODA_MASK_PIPE_5.qp());
      /*p26.VAVA*/ VAVA_MASK_PIPE_7 = dff22(_SACU_CLKPIPEp, _TUWU_MASK_PIPE_SET7, _WUBU_MASK_PIPE_RST7, VUMO_MASK_PIPE_6.qp());
    }
  }

  //----------------------------------------
  // Pixel merge + emit

  {
    /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp  = and2(PYBO_BG_PIPE_A7.qp(), VYXE_LCDC_BGENn.qn());
    /*#p35.TADE*/ wire TADE_PIX_BG_HIp  = and2(SOHU_BG_PIPE_B7.qp(), VYXE_LCDC_BGENn.qn());
    /*#p35.XULA*/ wire XULA_PIX_SP_LOp  = and2(XYLO_LCDC_SPENn.qn(), WUFY_SPR_PIPE_A7.qp());
    /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp  = and2(XYLO_LCDC_SPENn.qn(), VUPY_SPR_PIPE_B7.qp());

    /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

    /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, VAVA_MASK_PIPE_7.qp());
    /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, VAVA_MASK_PIPE_7.qp());
    /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

    /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(LYME_PAL_PIPE_7.qp());
    /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
    /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(LYME_PAL_PIPE_7.qp(), POKA_BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*#p35.WELE*/ wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
    /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

    /*#p35.LAVA*/ wire LAVA_MASK_OPB0  = not1(LAFU_OBP0PIXELn);

    /*#p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

    /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(XANA_OBP0_D7.qn(), VYRO_PAL_OBP0D,
                                                XYZE_OBP0_D5.qn(), VATA_PAL_OBP0C,
                                                XALO_OBP0_D3.qn(), VOLO_PAL_OBP0B,
                                                XUKY_OBP0_D1.qn(), VUGO_PAL_OBP0A);

    /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(XUPO_OBP0_D6.qn(), VYRO_PAL_OBP0D,
                                                XERU_OBP0_D4.qn(), VATA_PAL_OBP0C,
                                                XOVA_OBP0_D2.qn(), VOLO_PAL_OBP0B,
                                                XUFU_OBP0_D0.qn(), VUGO_PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*#p35.MABY*/ wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
    /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

    /*#p35.LUKU*/ wire LUKU_MASK_OBP1  = not1(LEKA_OBP1PIXELn);

    /*p#35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LYKY*/ wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LARU*/ wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

    /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(LUXO_OBP1_D7.qn(), LEDO_PAL_OBP1D,
                                                LUGU_OBP1_D5.qn(), LARU_PAL_OBP1C,
                                                LOSE_OBP1_D3.qn(), LYKY_PAL_OBP1B,
                                                LAWO_OBP1_D1.qn(), LOPU_PAL_OBP1A);
    
    /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, LEPU_OBP1_D6.qn(),
                                                LARU_PAL_OBP1C, LUNE_OBP1_D4.qn(),
                                                LYKY_PAL_OBP1B, MOSA_OBP1_D2.qn(),
                                                LOPU_PAL_OBP1A, MOXY_OBP1_D0.qn());

    //----------
    // Background/window palette lookup

    /*p35.SOBA*/ wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
    /*p35.NUPO*/ wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
    /*p35.VYCO*/ wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
    /*p35.NALE*/ wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

    /*p35.MUVE*/ wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

    /*p35.POBU*/ wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NUXO*/ wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NUMA*/ wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NYPO*/ wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

    /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, MOGY_BGP_D6.qn(),
                                              NUMA_PAL_BGPC, MUKE_BGP_D4.qn(),
                                              NUXO_PAL_BGPB, PYLU_BGP_D2.qn(),
                                              POBU_PAL_BGPA, PAVO_BGP_D0.qn());

    /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(MENA_BGP_D7.qn(), NYPO_PAL_BGPD,
                                              MORU_BGP_D5.qn(), NUMA_PAL_BGPC,
                                              MAXY_BGP_D3.qn(), NUXO_PAL_BGPB,
                                              NUSY_BGP_D1.qn(), POBU_PAL_BGPA);

    //----------
    // Pixel merge and send

    /*#p35.PERO*/ wire PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
    /*#p35.PATY*/ wire PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

    /*p#35.REMY*/ wire REMY_LD0n = not1(PERO_COL_LO);
    /*#p35.RAVO*/ wire RAVO_LD1n = not1(PATY_COL_HI);

    top.LCD_PIN_DATA0 = REMY_LD0n;
    top.LCD_PIN_DATA1 = RAVO_LD1n;
  }

  //----------------------------------------
  // FF40 LCDC

  {
    /*p22.WORU*/ wire _WORU_FF40n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not1(_WORU_FF40n);

    /*p23.WARU*/ wire _WARU_FF40_WRp = and2(_VOCA_FF40p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not1(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not1(top.clk_reg.XORE_SYS_RSTp());

    // REG9_01 NC
    // REG9_02 << CLKp
    // REG9_03 << D
    // REG9_04 << CLKn
    // REG9_05 NC
    // REG9_06 << RSTn
    // REG9_07 NC
    // REG9_08 >> Qn
    // REG9_09 >> Q

    // VYXE = dff9(XUBO, !XUBO, XARE, CPU_D0);

    // VYXE_QN >> TADE, RAJY
    // VYXE_QP >> WYPO

    // FIXME not sure why these are inverted

    /*p23.VYXE*/ VYXE_LCDC_BGENn   = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D0.qp());
    /*p23.XYLO*/ XYLO_LCDC_SPENn   = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D1.qp());
    /*p23.XYMO*/ XYMO_LCDC_SPSIZEn = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D2.qp());
    /*p23.XAFO*/ XAFO_LCDC_BGMAPn  = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D3.qp());
    /*p23.WEXU*/ WEXU_LCDC_BGTILEn = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D4.qp());
    /*p23.WYMO*/ WYMO_LCDC_WINENn  = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D5.qp());
    /*p23.WOKY*/ WOKY_LCDC_WINMAP  = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D6.qp());
    /*p23.XONA*/ XONA_LCDC_ENn     = dff9_inv(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D7.qp());

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and2(_VOCA_FF40p, top.ASOT_CPU_RDp());
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);

    /*#p23.WYPO*/ cpu_bus.CPU_BUS_D0 = tribuf_6nn(_WYCE_FF40_RDn, VYXE_LCDC_BGENn.qp());
    /*#p23.XERO*/ cpu_bus.CPU_BUS_D1 = tribuf_6nn(_WYCE_FF40_RDn, XYLO_LCDC_SPENn.qp());
    /* p23.WYJU*/ cpu_bus.CPU_BUS_D2 = tribuf_6nn(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZEn.qp());
    /* p23.WUKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6nn(_WYCE_FF40_RDn, XAFO_LCDC_BGMAPn.qp());
    /* p23.VOKE*/ cpu_bus.CPU_BUS_D4 = tribuf_6nn(_WYCE_FF40_RDn, WEXU_LCDC_BGTILEn.qp());
    /* p23.VATO*/ cpu_bus.CPU_BUS_D5 = tribuf_6nn(_WYCE_FF40_RDn, WYMO_LCDC_WINENn.qp());
    /*#p23.VAHA*/ cpu_bus.CPU_BUS_D6 = tribuf_6nn(_WYCE_FF40_RDn, WOKY_LCDC_WINMAP.qp());
    /*#p23.XEBU*/ cpu_bus.CPU_BUS_D7 = tribuf_6nn(_WYCE_FF40_RDn, XONA_LCDC_ENn.qp());
  }

  //----------------------------------------
  // FF41 STAT

  {
    /*p22.WOFA*/ wire _WOFA_FF41n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VARY*/ wire _VARY_FF41p = not1(_WOFA_FF41n);

    /*p21.SEPA*/ wire _SEPA_FF41_WRp = and2(_VARY_FF41p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p21.RYVE*/ wire _RYVE_FF41_WRn = not1(_SEPA_FF41_WRp);

    /*p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
    
    /*p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = nor2(top.clk_reg.WESY_SYS_RSTn(), _RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ _RUPO_LYC_MATCH_LATCHn = nor_latch(_PAGO_LYC_MATCH_RST, top.lcd_reg.ROPO_LY_MATCH_SYNCp());

    /*p21.ROXE*/ ROXE_INT_HBL_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D0.qp());
    /*p21.RUFO*/ RUFO_INT_VBL_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D1.qp());
    /*p21.REFE*/ REFE_INT_OAM_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D2.qp());
    /*p21.RUGU*/ RUGU_INT_LYC_EN = dff9(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.clk_reg.WESY_SYS_RSTn(), cpu_bus.CPU_BUS_D3.qp());

    /*p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(_XYMU_RENDERINGp.qp(), top.ACYL_SCANNINGp()); // die NOR
    /*p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(_XYMU_RENDERINGp.qp(), top.lcd_reg.PARU_VBLANKp_d4()); // die NOR

    // These tribufs are _different_. Why? Dunno.

    /*p21.TOBE*/ wire _TOBE_FF41_RDp = and2(_VARY_FF41p, top.ASOT_CPU_RDp()); // die AND
    /*p21.VAVE*/ wire _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp); // die INV

    // FIXME more evidence that tri6p is (or can be) inverting

    /*#p21.TEBY*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TOBE_FF41_RDp, _SADU_STAT_MODE0n);
    /*#p21.WUGA*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TOBE_FF41_RDp, _XATY_STAT_MODE1n);

    // SEGO := tri(TOBE, RUPO_03)

    /*p21.SEGO*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TOBE_FF41_RDp, _RUPO_LYC_MATCH_LATCHn.qp());

    /*p21.PUZO*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_VAVE_FF41_RDn, ROXE_INT_HBL_EN.qp());
    /*p21.POFO*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_VAVE_FF41_RDn, RUFO_INT_VBL_EN.qp());
    /*p21.SASY*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_VAVE_FF41_RDn, REFE_INT_OAM_EN.qp());
    /*p21.POTE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_VAVE_FF41_RDn, RUGU_INT_LYC_EN.qp());
  }

  //----------------------------------------
  // FF42 SCY
  {
    /*p22.WEBU*/ wire _WEBU_FF42n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XARO*/ wire _XARO_FF42p = not1(_WEBU_FF42n);

    /*p23.BEDY*/ wire _BEDY_FF42_WRp = and2(_XARO_FF42p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.CAVO*/ wire _CAVO_FF42_WRn = not1(_BEDY_FF42_WRp);

    /*p23.GAVE*/ GAVE_SCY0 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.FYMO*/ FYMO_SCY1 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.FEZU*/ FEZU_SCY2 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.FUJO*/ FUJO_SCY3 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.DEDE*/ DEDE_SCY4 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.FOTY*/ FOTY_SCY5 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.FOHA*/ FOHA_SCY6 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.FUNY*/ FUNY_SCY7 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.ANYP*/ wire _ANYP_FF42_RDp = and2(_XARO_FF42p, top.ASOT_CPU_RDp());
    /*p23.BUWY*/ wire _BUWY_FF42_RDn = not1(_ANYP_FF42_RDp);

    /*#p23.WARE*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_BUWY_FF42_RDn, GAVE_SCY0.qp());
    /*p23.GOBA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_BUWY_FF42_RDn, FYMO_SCY1.qp());
    /*p23.GONU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_BUWY_FF42_RDn, FEZU_SCY2.qp());
    /*p23.GODO*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_BUWY_FF42_RDn, FUJO_SCY3.qp());
    /*p23.CUSA*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_BUWY_FF42_RDn, DEDE_SCY4.qp());
    /*p23.GYZO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_BUWY_FF42_RDn, FOTY_SCY5.qp());
    /*p23.GUNE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_BUWY_FF42_RDn, FOHA_SCY6.qp());
    /*p23.GYZA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_BUWY_FF42_RDn, FUNY_SCY7.qp());
  }

  //----------------------------------------
  // FF43 SCX
  {
    /*p22.WAVU*/ wire _WAVU_FF43n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XAVY*/ wire _XAVY_FF43p = not1(_WAVU_FF43n);

    /*p23.ARUR*/ wire _ARUR_FF43_WRp = and2(_XAVY_FF43p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.AMUN*/ wire _AMUN_FF43_WRn = not1(_ARUR_FF43_WRp);

    /*p23.DATY*/ DATY_SCX0 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.DUZU*/ DUZU_SCX1 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.CYXU*/ CYXU_SCX2 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.GUBO*/ GUBO_SCX3 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.BEMY*/ BEMY_SCX4 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.CUZY*/ CUZY_SCX5 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.CABU*/ CABU_SCX6 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.BAKE*/ BAKE_SCX7 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.AVOG*/ wire _AVOG_FF43_RDp = and2(_XAVY_FF43p, top.ASOT_CPU_RDp());
    /*p23.BEBA*/ wire _BEBA_FF43_RDn = not1(_AVOG_FF43_RDp);

    /*#p23.EDOS*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_BEBA_FF43_RDn, DATY_SCX0.qp());
    /*p23.EKOB*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_BEBA_FF43_RDn, DUZU_SCX1.qp());
    /*p23.CUGA*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_BEBA_FF43_RDn, CYXU_SCX2.qp());
    /*p23.WONY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_BEBA_FF43_RDn, GUBO_SCX3.qp());
    /*p23.CEDU*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_BEBA_FF43_RDn, BEMY_SCX4.qp());
    /*p23.CATA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_BEBA_FF43_RDn, CUZY_SCX5.qp());
    /*p23.DOXE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_BEBA_FF43_RDn, CABU_SCX6.qp());
    /*p23.CASY*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_BEBA_FF43_RDn, BAKE_SCX7.qp());
  }

  //----------------------------------------
  // FF47 BGP

  {
    /*p22.WYBO*/ wire _WYBO_FF47n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*p22.WERA*/ wire _WERA_FF47 = not1(_WYBO_FF47n);

    /*p36.VELY*/ wire _VELY_FF47_WR = and2(top.CUPA_CPU_WRp_xxxDxxxx(), _WERA_FF47);
    /*p36.TEPO*/ wire _TEPO_FF47_WRn = not1(_VELY_FF47_WR);

    /*p36.PAVO*/ PAVO_BGP_D0 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p36.NUSY*/ NUSY_BGP_D1 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p36.PYLU*/ PYLU_BGP_D2 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p36.MAXY*/ MAXY_BGP_D3 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p36.MUKE*/ MUKE_BGP_D4 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p36.MORU*/ MORU_BGP_D5 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p36.MOGY*/ MOGY_BGP_D6 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p36.MENA*/ MENA_BGP_D7 = dff8_AB(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.cpu_bus.CPU_BUS_D7.qp());

    /*p36.VUSO*/ wire _VUSO_FF47_RD = and2(top.ASOT_CPU_RDp(), _WERA_FF47);
    /*p36.TEPY*/ wire _TEPY_FF47_RDn = not1(_VUSO_FF47_RD);

    /*#p36.RARO*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_TEPY_FF47_RDn, PAVO_BGP_D0.qp());
    /*p36.PABA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_TEPY_FF47_RDn, NUSY_BGP_D1.qp());
    /*p36.REDO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_TEPY_FF47_RDn, PYLU_BGP_D2.qp());
    /*p36.LOBE*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_TEPY_FF47_RDn, MAXY_BGP_D3.qp());
    /*p36.LACE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_TEPY_FF47_RDn, MUKE_BGP_D4.qp());
    /*p36.LYKA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_TEPY_FF47_RDn, MORU_BGP_D5.qp());
    /*p36.LODY*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_TEPY_FF47_RDn, MOGY_BGP_D6.qp());
    /*p36.LARY*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_TEPY_FF47_RDn, MENA_BGP_D7.qp());
  }

  //----------------------------------------
  // FF48 OBP0

  {
    /*p22.WETA*/ wire _WETA_FF48n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.XAYO*/ wire _XAYO_FF48 = not1(_WETA_FF48n);

    /*p36.XOMA*/ wire _XOMA_FF48_WR = and2(top.CUPA_CPU_WRp_xxxDxxxx(), _XAYO_FF48);
    /*p36.XELO*/ wire _XELO_FF48_WRn = not1(_XOMA_FF48_WR);

    /*p36.XUFU*/ XUFU_OBP0_D0 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p36.XUKY*/ XUKY_OBP0_D1 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p36.XOVA*/ XOVA_OBP0_D2 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p36.XALO*/ XALO_OBP0_D3 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p36.XERU*/ XERU_OBP0_D4 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p36.XYZE*/ XYZE_OBP0_D5 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p36.XUPO*/ XUPO_OBP0_D6 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p36.XANA*/ XANA_OBP0_D7 = dff8_AB(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.cpu_bus.CPU_BUS_D7.qp());

    /*p36.XUFY*/ wire _XUFY_FF48_RD = and2(top.ASOT_CPU_RDp(), _XAYO_FF48);
    /*p36.XOZY*/ wire _XOZY_FF48_RDn = not1(_XUFY_FF48_RD);

    /*#p36.XARY*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_XOZY_FF48_RDn, XUFU_OBP0_D0.qp());
    /*p36.XOKE*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_XOZY_FF48_RDn, XUKY_OBP0_D1.qp());
    /*p36.XUNO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_XOZY_FF48_RDn, XOVA_OBP0_D2.qp());
    /*p36.XUBY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_XOZY_FF48_RDn, XALO_OBP0_D3.qp());
    /*p36.XAJU*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_XOZY_FF48_RDn, XERU_OBP0_D4.qp());
    /*p36.XOBO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_XOZY_FF48_RDn, XYZE_OBP0_D5.qp());
    /*p36.XAXA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_XOZY_FF48_RDn, XUPO_OBP0_D6.qp());
    /*p36.XAWO*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_XOZY_FF48_RDn, XANA_OBP0_D7.qp());
  }

  //----------------------------------------
  // FF49 OBP1

  {
    /*p22.VAMA*/ wire _VAMA_FF49n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.TEGO*/ wire _TEGO_FF49 = not1(_VAMA_FF49n);

    /*p36.MYXE*/ wire _MYXE_FF49_WR = and2(top.CUPA_CPU_WRp_xxxDxxxx(), _TEGO_FF49);
    /*p36.LEHO*/ wire _LEHO_FF49_WRn = not1(_MYXE_FF49_WR);

    /*p36.MOXY*/ MOXY_OBP1_D0 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p36.LAWO*/ LAWO_OBP1_D1 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p36.MOSA*/ MOSA_OBP1_D2 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p36.LOSE*/ LOSE_OBP1_D3 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p36.LUNE*/ LUNE_OBP1_D4 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p36.LUGU*/ LUGU_OBP1_D5 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p36.LEPU*/ LEPU_OBP1_D6 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p36.LUXO*/ LUXO_OBP1_D7 = dff8_AB(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.cpu_bus.CPU_BUS_D7.qp());

    /*p36.MUMY*/ wire _MUMY_FF49_RD = and2(top.ASOT_CPU_RDp(), _TEGO_FF49);
    /*p36.LOTE*/ wire _LOTE_FF49_RDn = not1(_MUMY_FF49_RD);

    /*#p36.LAJU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_LOTE_FF49_RDn, MOXY_OBP1_D0.qp());
    /*p36.LEPA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_LOTE_FF49_RDn, LAWO_OBP1_D1.qp());
    /*p36.LODE*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_LOTE_FF49_RDn, MOSA_OBP1_D2.qp());
    /*p36.LYZA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_LOTE_FF49_RDn, LOSE_OBP1_D3.qp());
    /*p36.LUKY*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_LOTE_FF49_RDn, LUNE_OBP1_D4.qp());
    /*p36.LUGA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_LOTE_FF49_RDn, LUGU_OBP1_D5.qp());
    /*p36.LEBA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_LOTE_FF49_RDn, LEPU_OBP1_D6.qp());
    /*p36.LELU*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_LOTE_FF49_RDn, LUXO_OBP1_D7.qp());
  }

  //----------------------------------------
  // FF4A WY

  {
    /*p22.WYVO*/ wire WYVO_FF4An = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VYGA*/ wire VYGA_FF4Ap = not1(WYVO_FF4An);

    /*p23.WEKO*/ wire WEKO_FF4A_WRp = and2(VYGA_FF4Ap, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.VEFU*/ wire VEFU_FF4A_WRn = not1(WEKO_FF4A_WRp);

    /*p23.NESO*/ NESO_WY0 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.NYRO*/ NYRO_WY1 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.NAGA*/ NAGA_WY2 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.MELA*/ MELA_WY3 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.NULO*/ NULO_WY4 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.NENE*/ NENE_WY5 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.NUKA*/ NUKA_WY6 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.NAFU*/ NAFU_WY7 = dff9(VEFU_FF4A_WRn, !VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.WAXU*/ wire WAXU_FF4A_RDp = and2(VYGA_FF4Ap, top.ASOT_CPU_RDp());
    /*p23.VOMY*/ wire VOMY_FF4A_RDn = not1(WAXU_FF4A_RDp);

    /*#p23.PUNU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(VOMY_FF4A_RDn, NESO_WY0.qp());
    /*p23.PODA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(VOMY_FF4A_RDn, NYRO_WY1.qp());
    /*p23.PYGU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(VOMY_FF4A_RDn, NAGA_WY2.qp());
    /*p23.LOKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(VOMY_FF4A_RDn, MELA_WY3.qp());
    /*p23.MEGA*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(VOMY_FF4A_RDn, NULO_WY4.qp());
    /*p23.PELA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(VOMY_FF4A_RDn, NENE_WY5.qp());
    /*p23.POLO*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(VOMY_FF4A_RDn, NUKA_WY6.qp());
    /*p23.MERA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(VOMY_FF4A_RDn, NAFU_WY7.qp());
  }

  //----------------------------------------
  // FF4B WX

  {
    /*p22.WAGE*/ wire WAGE_FF4Bn = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VUMY*/ wire VUMY_FF4Bp = not1(WAGE_FF4Bn);

    /*p23.WUZA*/ wire WUZA_FF4B_WRp = and2(VUMY_FF4Bp, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.VOXU*/ wire VOXU_FF4B_WRn = not1(WUZA_FF4B_WRp);

    /*p23.MYPA*/ MYPA_WX0 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.NOFE*/ NOFE_WX1 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.NOKE*/ NOKE_WX2 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.MEBY*/ MEBY_WX3 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.MYPU*/ MYPU_WX4 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.MYCE*/ MYCE_WX5 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.MUVO*/ MUVO_WX6 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.NUKU*/ NUKU_WX7 = dff9(VOXU_FF4B_WRn, !VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.WYZE*/ wire WYZE_FF4B_RDp = and2(VUMY_FF4Bp, top.ASOT_CPU_RDp());
    /*p23.VYCU*/ wire VYCU_FF4B_RDn = not1(WYZE_FF4B_RDp);

    /*#p23.LOVA*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(VYCU_FF4B_RDn, MYPA_WX0.qp());
    /*p23.MUKA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(VYCU_FF4B_RDn, NOFE_WX1.qp());
    /*p23.MOKO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(VYCU_FF4B_RDn, NOKE_WX2.qp());
    /*p23.LOLE*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(VYCU_FF4B_RDn, MEBY_WX3.qp());
    /*p23.MELE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(VYCU_FF4B_RDn, MYPU_WX4.qp());
    /*p23.MUFE*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(VYCU_FF4B_RDn, MYCE_WX5.qp());
    /*p23.MULY*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(VYCU_FF4B_RDn, MUVO_WX6.qp());
    /*p23.MARA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(VYCU_FF4B_RDn, NUKU_WX7.qp());
  }
}

//------------------------------------------------------------------------------
