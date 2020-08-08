#include "Sch_PixPipe.h"

#include "Sch_Top.h"

using namespace Schematics;

void PixelPipe::dump(Dumper& d, const SchematicTop& top) const {
  d("---------- Pix Pipe ----------\n");

  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor3(top.sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), top.clk_reg.MYVO_AxCxExGx());
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not1(TOMU_WIN_HITp());
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and3(_SOCY_WIN_HITn, top.tile_fetcher.POKY_PRELOAD_DONEp(), _VYBO_PIX_CLK_xBxDxFxH);
  /*p24.SEGU*/ wire _SEGU_CLKPIPEn = not1(_TYFA_CLKPIPEp_xBxDxFxH);
  /*p24.SACU*/ wire _SACU_CLKPIPEp = nor2(_SEGU_CLKPIPEn, _ROXY_FINE_SCROLL_DONEn.qp());

  d("_VYBO_PIX_CLK_xBxDxFxH  %d\n", _VYBO_PIX_CLK_xBxDxFxH);
  d("_SOCY_WIN_HITn          %d\n", _SOCY_WIN_HITn);
  d("_TYFA_CLKPIPEp_xBxDxFxH %d\n", _TYFA_CLKPIPEp_xBxDxFxH);
  d("_SEGU_CLKPIPEn          %d\n", _SEGU_CLKPIPEn);
  d("_SACU_CLKPIPEp          %d\n", _SACU_CLKPIPEp);

  /*p21.TADY*/ wire _TADY_LINE_START_RST = nor2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());
  d("BYHA_VID_LINE_END_TRIGn     %d\n", top.lcd_reg.BYHA_VID_LINE_END_TRIGn());
  d("TOFU_VID_RSTp           %d\n", top.clk_reg.TOFU_VID_RSTp());
  d("_TADY_LINE_START_RST    %d\n", _TADY_LINE_START_RST);
  d("\n");

  d("PIX COUNT   %03d\n", pack_p(
    XEHO_X0p.qp(),
    SAVY_X1p.qp(),
    XODU_X2p.qp(),
    XYDO_X3p.qp(),
    TUHU_X4p.qp(),
    TUKY_X5p.qp(),
    TAKO_X6p.qp(),
    SYBE_X7p.qp()
  ));
  d("\n");

  d("VYXE_LCDC_BGEN   %c\n", VYXE_LCDC_BGEN  .c());
  d("XYLO_LCDC_SPEN   %c\n", XYLO_LCDC_SPEN  .c());
  d("XYMO_LCDC_SPSIZE %c\n", XYMO_LCDC_SPSIZE.c());
  d("XAFO_LCDC_BGMAP  %c\n", XAFO_LCDC_BGMAP .c());
  d("WEXU_LCDC_BGTILE %c\n", WEXU_LCDC_BGTILE.c());
  d("WYMO_LCDC_WINEN  %c\n", WYMO_LCDC_WINEN .c());
  d("WOKY_LCDC_WINMAP %c\n", WOKY_LCDC_WINMAP.c());
  d("XONA_LCDC_EN     %c\n", XONA_LCDC_EN    .c());
  d("\n");

  d("INT_HBL_EN  %c\n", ROXE_INT_HBL_EN.c());
  d("INT_VBL_EN  %c\n", RUFO_INT_VBL_EN.c());
  d("INT_OAM_EN  %c\n", REFE_INT_OAM_EN.c());
  d("INT_LYC_EN  %c\n", RUGU_INT_LYC_EN.c());

  d("ROXY_FINE_SCROLL_DONEn %c\n", _ROXY_FINE_SCROLL_DONEn.c());
  d("RYKU_FINE_CNT0         %c\n", _RYKU_FINE_CNT0        .c());
  d("ROGA_FINE_CNT1         %c\n", _ROGA_FINE_CNT1        .c());
  d("RUBU_FINE_CNT2         %c\n", _RUBU_FINE_CNT2        .c());
  d("_XENA_STORE_MATCHn     %c\n", _XENA_STORE_MATCHn     .c());
  d("XYMU_RENDERINGp        %c\n", _XYMU_RENDERINGp       .c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", _RUPO_LYC_MATCH_LATCHn .c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", _WUSA_LCD_CLOCK_GATE   .c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", _VOGA_RENDER_DONE_SYNC .c());
  d("PUXA_FINE_MATCH_A      %c\n", _PUXA_FINE_MATCH_A     .c());
  d("NYZE_FINE_MATCH_B      %c\n", _NYZE_FINE_MATCH_B     .c());
  d("PAHO_X_8_SYNC          %c\n", _PAHO_X_8_SYNC         .c());
  //d("RUJU_LCD_PIN_STp       %c\n", RUJU_LCD_PIN_STp.c());
  //d("POME_LCD_PIN_STn       %c\n", POME_LCD_PIN_STn.c());
  d("LCD_PIN_CP             %c\n", _LCD_PIN_CP            .c());
  d("LCD_PIN_ST             %c\n", _LCD_PIN_ST            .c());
  d("LCD_PIN_LD1            %c\n", LCD_PIN_LD1           .c());
  d("LCD_PIN_LD0            %c\n", LCD_PIN_LD0           .c());

  d("BG_PIPE_A  %c%c%c%c%c%c%c%c\n", 
    MYDE_BG_PIPE_A0.c(), NOZO_BG_PIPE_A1.c(), MOJU_BG_PIPE_A2.c(), MACU_BG_PIPE_A3.c(),
    NEPO_BG_PIPE_A4.c(), MODU_BG_PIPE_A5.c(), NEDA_BG_PIPE_A6.c(), PYBO_BG_PIPE_A7.c());

  d("BG_PIPE_B  %c%c%c%c%c%c%c%c\n", 
    TOMY_BG_PIPE_B0.c(), TACA_BG_PIPE_B1.c(), SADY_BG_PIPE_B2.c(), RYSA_BG_PIPE_B3.c(),
    SOBO_BG_PIPE_B4.c(), SETU_BG_PIPE_B5.c(), RALU_BG_PIPE_B6.c(), SOHU_BG_PIPE_B7.c());

  d("SPR_PIPE_A %c%c%c%c%c%c%c%c\n", 
    NYLU_SPR_PIPE_A0.c(), PEFU_SPR_PIPE_A1.c(), NATY_SPR_PIPE_A2.c(), PYJO_SPR_PIPE_A3.c(),
    VARE_SPR_PIPE_A4.c(), WEBA_SPR_PIPE_A5.c(), VANU_SPR_PIPE_A6.c(), VUPY_SPR_PIPE_A7.c());

  d("SPR_PIPE_B %c%c%c%c%c%c%c%c\n", 
    NURO_SPR_PIPE_B0.c(), MASO_SPR_PIPE_B1.c(), LEFE_SPR_PIPE_B2.c(), LESU_SPR_PIPE_B3.c(),
    WYHO_SPR_PIPE_B4.c(), WORA_SPR_PIPE_B5.c(), VAFO_SPR_PIPE_B6.c(), WUFY_SPR_PIPE_B7.c());

  d("PAL_PIPE   %c%c%c%c%c%c%c%c     %c\n", 
    RUGO_PAL_PIPE_0.c(), SATA_PAL_PIPE_1.c(), ROSA_PAL_PIPE_2.c(), SOMY_PAL_PIPE_3.c(), 
    PALU_PAL_PIPE_4.c(), NUKE_PAL_PIPE_5.c(), MODA_PAL_PIPE_6.c(), LYME_PAL_PIPE_7.c(),
    LCD_PIN_LD1.c());

  d("MASK_PIPE  %c%c%c%c%c%c%c%c     %c\n", 
    VEZO_MASK_PIPE_0.c(), WURU_MASK_PIPE_1.c(), VOSA_MASK_PIPE_2.c(), WYFU_MASK_PIPE_3.c(),
    XETE_MASK_PIPE_4.c(), WODA_MASK_PIPE_5.c(), VUMO_MASK_PIPE_6.c(), VAVA_MASK_PIPE_7.c(),
    LCD_PIN_LD0.c());

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
  int wx = pack_p(
    MYPA_WX0.qp(),
    NOFE_WX1.qp(),
    NOKE_WX2.qp(),
    MEBY_WX3.qp(),
    MYPU_WX4.qp(),
    MYCE_WX5.qp(),
    MUVO_WX6.qp(),
    NUKU_WX7.qp());
    
  int wy = pack_p(
    NESO_WY0.qp(),
    NYRO_WY1.qp(),
    NAGA_WY2.qp(),
    MELA_WY3.qp(),
    NULO_WY4.qp(),
    NENE_WY5.qp(),
    NUKA_WY6.qp(),
    NAFU_WY7.qp()
  );

  int win_x = pack_p(
    0,
    0,
    0,
    _WYKA_WIN_X3.qp(),
    _WODY_WIN_X4.qp(),
    _WOBO_WIN_X5.qp(),
    _WYKO_WIN_X6.qp(),
    _XOLO_WIN_X7.qp()
  );

  int win_y = pack_p(
    _VYNO_WIN_Y0.qp(),
    _VUJO_WIN_Y1.qp(),
    _VYMU_WIN_Y2.qp(),
    _TUFU_WIN_Y3.qp(),
    _TAXA_WIN_Y4.qp(),
    _TOZO_WIN_Y5.qp(),
    _TATE_WIN_Y6.qp(),
    _TEKE_WIN_Y7.qp()
  );

  d("WX                    : %d\n", wx);
  d("WY                    : %d\n", wy);
  d("WIN X                 : %d\n", win_x);
  d("WIN_Y                 : %d\n", win_y);
  d("NUKO_WX_MATCHp        : %d\n", wire(NUKO_WX_MATCHp));
  d("ROGE_WY_MATCHp        : %d\n", wire(ROGE_WY_MATCHp));
  d("PYNU_WIN_MODE_A       : %c\n", _PYNU_WIN_MODE_A      .c());
  d("RYDY_WIN_FIRST_TILE_A : %c\n", _RYDY_WIN_FIRST_TILE_A.c());
  d("NOPA_WIN_MODE_B       : %c\n", _NOPA_WIN_MODE_B      .c());
  d("SOVY_WIN_FIRST_TILE_B : %c\n", _SOVY_WIN_FIRST_TILE_B.c());
  d("REJO_WY_MATCH_LATCH   : %c\n", _REJO_WY_MATCH_LATCH  .c());
  d("SARY_WY_MATCH         : %c\n", _SARY_WY_MATCH        .c());
  d("RYFA_WX_MATCHn_A      : %c\n", _RYFA_WX_MATCHn_A     .c());
  d("RENE_WX_MATCHn_B      : %c\n", _RENE_WX_MATCHn_B     .c());
  d("PYCO_WX_MATCH_A       : %c\n", _PYCO_WX_MATCH_A      .c());
  d("NUNU_WX_MATCH_B       : %c\n", _NUNU_WX_MATCH_B      .c());
  d("\n");
}

//------------------------------------------------------------------------------

void PixelPipe::tick(const SchematicTop& top) {
  _XENA_STORE_MATCHn = top.sprite_store.XENA_STORE_MATCHn();

}

//------------------------------------------------------------------------------

void PixelPipe::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  wire GND = 0;

  // VYBO := nor(FEPO, WODU, MYVO)
  // SOCY := not(TOMU)
  // TYFA := and(SOCY, POKY_04, VYBO)
  // SEGU := not(TYFA)
  // SACU := or(SEGU, ROXY_04)

  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor3(top.sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), top.clk_reg.MYVO_AxCxExGx());
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not1(TOMU_WIN_HITp());
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and3(_SOCY_WIN_HITn, top.tile_fetcher.POKY_PRELOAD_DONEp(), _VYBO_PIX_CLK_xBxDxFxH);
  /*p24.SEGU*/ wire _SEGU_CLKPIPEn = not1(_TYFA_CLKPIPEp_xBxDxFxH);
  /*p24.SACU*/ wire _SACU_CLKPIPEp = or2(_SEGU_CLKPIPEn, _ROXY_FINE_SCROLL_DONEn.qp()); // Schematic wrong, this is OR

  //----------------------------------------
  // Pixel counter, has carry lookahead because this can increment every tcycle

  {
    /*p21.RYBO*/ wire _RYBO = xor2(XEHO_X0p.qp(), SAVY_X1p.qp());
    /*p21.XUKE*/ wire _XUKE = and2(XEHO_X0p.qp(), SAVY_X1p.qp());

    /*p21.XYLE*/ wire _XYLE = and2(XODU_X2p.qp(), _XUKE);
    /*p21.XEGY*/ wire _XEGY = xor2(XODU_X2p.qp(), _XUKE);
    /*p21.XORA*/ wire _XORA = xor2(XYDO_X3p.qp(), _XYLE);

    /*p21.SAKE*/ wire _SAKE = xor2(TUHU_X4p.qp(), TUKY_X5p.qp());
    /*p21.TYBA*/ wire _TYBA = and2(TUHU_X4p.qp(), TUKY_X5p.qp());
    /*p21.SURY*/ wire _SURY = and2(TAKO_X6p.qp(), _TYBA);
    /*p21.TYGE*/ wire _TYGE = xor2(TAKO_X6p.qp(), _TYBA);
    /*p21.ROKU*/ wire _ROKU = xor2(SYBE_X7p.qp(), _SURY);

    /*p24.TOCA*/ wire _TOCA_CLKPIPE_HI = not1(XYDO_X3p.qp());

    /*p21.TADY*/ wire _TADY_LINE_START_RST = nor2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());

    /*p21.XEHO*/ XEHO_X0p = dff17_AB(_SACU_CLKPIPEp,    _TADY_LINE_START_RST, XEHO_X0p.qn());
    /*p21.SAVY*/ SAVY_X1p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _RYBO);
    /*p21.XODU*/ XODU_X2p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XEGY);
    /*p21.XYDO*/ XYDO_X3p = dff17_B (_SACU_CLKPIPEp,    _TADY_LINE_START_RST, _XORA);
    /*p21.TUHU*/ TUHU_X4p = dff17_AB(_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, TUHU_X4p.qn());
    /*p21.TUKY*/ TUKY_X5p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _SAKE);
    /*p21.TAKO*/ TAKO_X6p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _TYGE);
    /*p21.SYBE*/ SYBE_X7p = dff17_B (_TOCA_CLKPIPE_HI,  _TADY_LINE_START_RST, _ROKU);
  }

  //----------------------------------------
  // LCD pins that are controlled by the pixel counter

  {
    // XAJO := and(XEHO_Q, XYDO_Q)
    // WEGO := or(TOFU, VOGA_Q)
    // WUSA := latch(XAJO, WEGO)
    // TOBA := and(WUSA_04, SACU)
    // POVA := and(NYZE_QN, PUXA_Q)
    // SEMU := or(TOBA, POVA)
    // RYPO := not(SEMU)
    // LCD_CP := RYPO

    /*p21.XAJO*/ wire _XAJO_X_009 = and2(XEHO_X0p.qp(), XYDO_X3p.qp());
    /*p21.WEGO*/ wire _WEGO_LINE_END_RST   = or2(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONE_SYNC.qp());
    /*p21.WUSA*/ _WUSA_LCD_CLOCK_GATE = nor_latch(_XAJO_X_009, _WEGO_LINE_END_RST);

    /*p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(_SACU_CLKPIPEp, _WUSA_LCD_CLOCK_GATE.qp());
    /*p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(_PUXA_FINE_MATCH_A.qp(), _NYZE_FINE_MATCH_B.qn());
    /*p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
    /*p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
    _LCD_PIN_CP = _RYPO_LCD_CLOCK;
  }

  {
    // LCD horizontal sync pin latch
    // AVAP high         -> LCD_PIN_ST = 1
    // PAHO or TOFU high -> LCD_PIN_ST = 0

    // ROXO := not(SEGU)
    // PAHO := dff17(ROXO, XYMU_03, XYDO_Q)
    // RUJU := nor(PAHO_Q, TOFU, POME)
    // POME := nor(AVAP, POFY)
    // POFY := not(RUJU)
    // RUZE := not(POFY)

    /*p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*p24.PAHO*/ _PAHO_X_8_SYNC = dff17_B(_ROXO_CLKPIPEp, _XYMU_RENDERINGp.qp(), XYDO_X3p.qp());
    
    ///*p24.POFY*/ wire POFY_LCD_PIN_STn = not1(RUJU_LCD_PIN_STp.qp());
    ///*p24.RUJU*/ RUJU_LCD_PIN_STp = nor3(_PAHO_X_8_SYNC.qp(), top.clk_reg.TOFU_VID_RSTp(), POME_LCD_PIN_STn.qp());
    ///*p24.POME*/ POME_LCD_PIN_STn = nor2(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), POFY_LCD_PIN_STn);
    ///*p24.RUZE*/ wire RUZE_LCD_PIN_STp = not1(POFY_LCD_PIN_STn);
   
    /*p24.RUJU*/ _POFY_ST_LATCH = nor_latch(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), _PAHO_X_8_SYNC.qp() || top.clk_reg.TOFU_VID_RSTp());
    /*p24.RUZE*/ wire RUZE_LCD_PIN_STp = not1(_POFY_ST_LATCH.qp());

    _LCD_PIN_ST = RUZE_LCD_PIN_STp;
  }

  //----------------------------------------
  // Fine scroll counter. This looks fine except for the "arm on ground side" xor thing. Not sure about that.

  {
    // ROXO := not(SEGU)
    // ROZE := nand(RUBU_Q, ROGA_Q, RYKU_Q)
    // PECU := nand(ROXO, ROZE)
    // PAHA := not(XYMU)
    // PASO := nor(PAHA, TEVO)

    // RYKU := dff17(PECU,    PASO, RYKU_QN);
    // ROGA := dff17(RYKU_QN, PASO, ROGA_QN);
    // RUBU := dff17(ROGA_QN, PASO, RUBU_QN);

    /*p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*p27.PAHA*/ wire _PAHA_RENDERINGn = not1(_XYMU_RENDERINGp.qp());
    /*p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(_RYKU_FINE_CNT0.qp(), _ROGA_FINE_CNT1.qp(), _RUBU_FINE_CNT2.qp());
    /*p27.PECU*/ wire _PECU_FINE_CLK = nand2(_ROXO_CLKPIPEp, _ROZE_FINE_COUNT_7n);
    /*p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn, top.TEVO_FINE_RSTp());

    /*p27.RYKU*/ _RYKU_FINE_CNT0 = dff17_AB(_PECU_FINE_CLK,       _PASO_FINE_RST, _RYKU_FINE_CNT0.qn());
    /*p27.ROGA*/ _ROGA_FINE_CNT1 = dff17_AB(_RYKU_FINE_CNT0.qn(), _PASO_FINE_RST, _ROGA_FINE_CNT1.qn());
    /*p27.RUBU*/ _RUBU_FINE_CNT2 = dff17_AB(_ROGA_FINE_CNT1.qn(), _PASO_FINE_RST, _RUBU_FINE_CNT2.qn());

    // Arms on the ground side, why?
    // SUHA := xor(DATY_QN, RYKU_Q)
    // SYBY := xor(DUZU_QN, ROGA_Q)
    // SOZU := xor(CYXU_QN, RUBU_Q)

    /*p27.SUHA*/ wire _SUHA_FINE_MATCHp = xor2(top.tile_fetcher.DATY_SCX0.qn(), _RYKU_FINE_CNT0.qp()); 
    /*p27.SYBY*/ wire _SYBY_FINE_MATCHp = xor2(top.tile_fetcher.DUZU_SCX1.qn(), _ROGA_FINE_CNT1.qp());
    /*p27.SOZU*/ wire _SOZU_FINE_MATCHp = xor2(top.tile_fetcher.CYXU_SCX2.qn(), _RUBU_FINE_CNT2.qp());

    // RONE := nand(ROXY, SUHA, SYBY, SOZU)
    // POHU := not(RONE)

    /*p27.RONE*/ wire _RONE_FINE_MATCHn = nand4(_ROXY_FINE_SCROLL_DONEn.qp(), _SUHA_FINE_MATCHp, _SYBY_FINE_MATCHp, _SOZU_FINE_MATCHp);
    /*p27.POHU*/ wire _POHU_FINE_MATCHp = not1(_RONE_FINE_MATCHn);

     /*p27.MOXE*/ wire MOXE_AxCxExGx = not1(top.clk_reg.ALET_xBxDxFxH());

    // PUXA := dff17(ROXO, XYMU, POHU)
    // NYZE := dff17(MOXE, XYMU, PUXA_Q);

    /*p27.PUXA*/ _PUXA_FINE_MATCH_A = dff17_B(_ROXO_CLKPIPEp, _XYMU_RENDERINGp.qp(), _POHU_FINE_MATCHp);
    /*p27.NYZE*/ _NYZE_FINE_MATCH_B = dff17_A(MOXE_AxCxExGx,  _XYMU_RENDERINGp.qp(), _PUXA_FINE_MATCH_A.qp());

    // POVA := and(NYZE_QN, PUXA_Q);

    // ROXY_00 << PAHA
    // ROXY_01 nc
    // ROXY_02 >> nc
    // ROXY_03 >> RONE_00
    // ROXY_04 nc
    // ROXY_05 << POVA

    /*p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(_NYZE_FINE_MATCH_B.qn(), _PUXA_FINE_MATCH_A.qp());
    /*p27.ROXY*/ _ROXY_FINE_SCROLL_DONEn = nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCHpe);
  }

  //----------------------------------------
  // Window matcher

  {
    /*p27.MYLO*/ wire _WX_MATCH0 = xnor2(XEHO_X0p.qp(), MYPA_WX0.qp());
    /*p27.PUWU*/ wire _WX_MATCH1 = xnor2(SAVY_X1p.qp(), NOFE_WX1.qp());
    /*p27.PUHO*/ wire _WX_MATCH2 = xnor2(XODU_X2p.qp(), NOKE_WX2.qp());
    /*p27.NYTU*/ wire _WX_MATCH3 = xnor2(XYDO_X3p.qp(), MEBY_WX3.qp());
    /*p27.NEZO*/ wire _WX_MATCH4 = xnor2(TUHU_X4p.qp(), MYPU_WX4.qp());
    /*p27.NORY*/ wire _WX_MATCH5 = xnor2(TUKY_X5p.qp(), MYCE_WX5.qp());
    /*p27.NONO*/ wire _WX_MATCH6 = xnor2(TAKO_X6p.qp(), MUVO_WX6.qp());
    /*p27.PASE*/ wire _WX_MATCH7 = xnor2(SYBE_X7p.qp(), NUKU_WX7.qp());

    /*p27.PUKY*/ wire _WX_MATCH_HIn  = nand5(_REJO_WY_MATCH_LATCH.qp(), _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
    /*p27.NUFA*/ wire _WX_MATCH_HI   = not1(_WX_MATCH_HIn);
    /*p27.NOGY*/ wire _WX_MATCHn     = nand5(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
    /*p27.NUKO*/ NUKO_WX_MATCHp = not1(_WX_MATCHn);

    /*p27.NAZE*/ wire _WY_MATCH0 = xnor2(top.lcd_reg.MUWY_Y0.qp(), NESO_WY0.qp());
    /*p27.PEBO*/ wire _WY_MATCH1 = xnor2(top.lcd_reg.MYRO_Y1.qp(), NYRO_WY1.qp());
    /*p27.POMO*/ wire _WY_MATCH2 = xnor2(top.lcd_reg.LEXA_Y2.qp(), NAGA_WY2.qp());
    /*p27.NEVU*/ wire _WY_MATCH3 = xnor2(top.lcd_reg.LYDO_Y3.qp(), MELA_WY3.qp());
    /*p27.NOJO*/ wire _WY_MATCH4 = xnor2(top.lcd_reg.LOVU_Y4.qp(), NULO_WY4.qp());
    /*p27.PAGA*/ wire _WY_MATCH5 = xnor2(top.lcd_reg.LEMA_Y5.qp(), NENE_WY5.qp());
    /*p27.PEZO*/ wire _WY_MATCH6 = xnor2(top.lcd_reg.MATO_Y6.qp(), NUKA_WY6.qp());
    /*p27.NUPA*/ wire _WY_MATCH7 = xnor2(top.lcd_reg.LAFO_Y7.qp(), NAFU_WY7.qp());

    /*p27.PALO*/ wire _WY_MATCH_HIn  = nand5(WYMO_LCDC_WINEN.qp(), _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
    /*p27.NELE*/ wire _WY_MATCH_HI   = not1(_WY_MATCH_HIn);
    /*p27.PAFU*/ wire _WY_MATCHn     = nand5(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
    /*p27.ROGE*/ ROGE_WY_MATCHp = not1(_WY_MATCHn);
  }

  //----------------------------------------
  // Window sequencer

  {
    // This trigger fires on the pixel _at_ WX
    /*p27.ROCO*/ wire _ROCO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
    /*p27.PYCO*/ _PYCO_WX_MATCH_A = dff17_B(_ROCO_CLKPIPEp, top.clk_reg.XAPO_VID_RSTn(), NUKO_WX_MATCHp);
    /*p27.NUNU*/ _NUNU_WX_MATCH_B = dff17_B(top.clk_reg.MEHE_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), _PYCO_WX_MATCH_A.qp());

    /*p28.ABAF*/ wire ABAF_LINE_END_Bn = not1(top.lcd_reg._CATU_LINE_p002p.qp());

    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIGn = and2(or2(top.lcd_reg._ANEL_LINE_p004p.qp(), ABAF_LINE_END_Bn), top.lcd_reg._ABEZ_VID_RSTn);

    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIGp = not1(BYHA_VID_LINE_TRIGn);

    /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not1(ATEJ_VID_LINE_TRIGp);
    
    /*p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(WYMO_LCDC_WINEN.qp(), XAHY_VID_LINE_TRIG_d4n, top.clk_reg.XAPO_VID_RSTn());
    
    /*p27.PYNU*/ _PYNU_WIN_MODE_A = nor_latch(_NUNU_WX_MATCH_B.qp(), XOFO_WIN_RSTp);
    /*p27.NOPA*/ _NOPA_WIN_MODE_B = dff17_A(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), _PYNU_WIN_MODE_A.qp());
  }

  {
    // This trigger fires on the pixel _after_ WX. Not sure what the fine count is about.
    /*p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(_RYKU_FINE_CNT0.qp(), _ROGA_FINE_CNT1.qp(), _RUBU_FINE_CNT2.qp());
    /*p27.PANY*/ wire _PANY_WX_MATCHn = nor2(NUKO_WX_MATCHp, _ROZE_FINE_COUNT_7n);
    /*p27.RYFA*/ _RYFA_WX_MATCHn_A = dff17_AB(_SEGU_CLKPIPEn, XYMU_RENDERINGp(), _PANY_WX_MATCHn);
    /*p27.RENE*/ _RENE_WX_MATCHn_B = dff17_B (top.clk_reg.ALET_xBxDxFxH(), XYMU_RENDERINGp(), _RYFA_WX_MATCHn_A.qp());
  }

  {
    /*p27.REPU*/ wire _REPU_VBLANK_RSTp = or2(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /*p27.SARY*/ _SARY_WY_MATCH = dff17_B(top.clk_reg.TALU_ABCDxxxx(), top.clk_reg.XAPO_VID_RSTn(), ROGE_WY_MATCHp);
    /*p27.REJO*/ _REJO_WY_MATCH_LATCH = nor_latch(_SARY_WY_MATCH.qp(), _REPU_VBLANK_RSTp);
  }

  {
    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
    ///*p27.PUKU*/ PUKU = nor4(RYDY, WIN_MODE_TRIG);
    ///*p27.RYDY*/ RYDY = nor4(PUKU, clk_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);

    /*p27.RYDY*/ _RYDY_WIN_FIRST_TILE_A = nor_latch(NUNY_WX_MATCHpe(), top.clk_reg.PYRY_VID_RSTp() || top.tile_fetcher.PORY_TILE_FETCH_DONE_Bp());
    /*p27.SOVY*/ _SOVY_WIN_FIRST_TILE_B = dff17_B(top.clk_reg.ALET_xBxDxFxH(), top.clk_reg.XAPO_VID_RSTn(), _RYDY_WIN_FIRST_TILE_A.qp());
  }

  // window x coordinate
  {
    // something weird here, PORE doesn't look like a clock

    /*p27.VETU*/ wire _VETU_WIN_MAP_CLK = and2(top.TEVO_FINE_RSTp(), PORE_WIN_MODEp());
    /*p27.XAHY*/ wire _XAHY_VID_LINE_TRIG_d4n = not1(top.lcd_reg.ATEJ_VID_LINE_TRIGp());
    /*p27.XOFO*/ wire _XOFO_WIN_RSTp = nand3(WYMO_LCDC_WINEN.qp(), _XAHY_VID_LINE_TRIG_d4n, top.clk_reg.XAPO_VID_RSTn());
    /*p27.XACO*/ wire _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp);

    /*p27.WYKA*/ _WYKA_WIN_X3 = dff17_AB(_VETU_WIN_MAP_CLK,  _XACO_WIN_RSTn, _WYKA_WIN_X3.qn());
    /*p27.WODY*/ _WODY_WIN_X4 = dff17_AB(_WYKA_WIN_X3.qn(),  _XACO_WIN_RSTn, _WODY_WIN_X4.qn());
    /*p27.WOBO*/ _WOBO_WIN_X5 = dff17_AB(_WODY_WIN_X4.qn(),  _XACO_WIN_RSTn, _WOBO_WIN_X5.qn());
    /*p27.WYKO*/ _WYKO_WIN_X6 = dff17_AB(_WOBO_WIN_X5.qn(),  _XACO_WIN_RSTn, _WYKO_WIN_X6.qn());
    /*p27.XOLO*/ _XOLO_WIN_X7 = dff17_AB(_WYKO_WIN_X6.qn(),  _XACO_WIN_RSTn, _XOLO_WIN_X7.qn());
  }

  // window y coordinate
  // every time we leave win mode we increment win_y
  {
    /*p27.WAZY*/ wire _WAZY_WIN_Y_CLK = not1(PORE_WIN_MODEp());
    /*p27.REPU*/ wire _REPU_VBLANK_RSTp = or2(top.lcd_reg.PARU_VBLANKp_d4(), top.clk_reg.PYRY_VID_RSTp());
    /*p27.SYNY*/ wire _SYNY_VBLANK_RSTn = not1(_REPU_VBLANK_RSTp);

    /*p27.VYNO*/ _VYNO_WIN_Y0 = dff17_AB(_WAZY_WIN_Y_CLK,   _SYNY_VBLANK_RSTn, _VYNO_WIN_Y0.qn());
    /*p27.VUJO*/ _VUJO_WIN_Y1 = dff17_AB(_VYNO_WIN_Y0.qn(), _SYNY_VBLANK_RSTn, _VUJO_WIN_Y1.qn());
    /*p27.VYMU*/ _VYMU_WIN_Y2 = dff17_AB(_VUJO_WIN_Y1.qn(), _SYNY_VBLANK_RSTn, _VYMU_WIN_Y2.qn());
    /*p27.TUFU*/ _TUFU_WIN_Y3 = dff17_AB(_VYMU_WIN_Y2.qn(), _SYNY_VBLANK_RSTn, _TUFU_WIN_Y3.qn());
    /*p27.TAXA*/ _TAXA_WIN_Y4 = dff17_AB(_TUFU_WIN_Y3.qn(), _SYNY_VBLANK_RSTn, _TAXA_WIN_Y4.qn());
    /*p27.TOZO*/ _TOZO_WIN_Y5 = dff17_AB(_TAXA_WIN_Y4.qn(), _SYNY_VBLANK_RSTn, _TOZO_WIN_Y5.qn());
    /*p27.TATE*/ _TATE_WIN_Y6 = dff17_AB(_TOZO_WIN_Y5.qn(), _SYNY_VBLANK_RSTn, _TATE_WIN_Y6.qn());
    /*p27.TEKE*/ _TEKE_WIN_Y7 = dff17_AB(_TATE_WIN_Y6.qn(), _SYNY_VBLANK_RSTn, _TEKE_WIN_Y7.qn());
  }

  {
    /*p21.TADY*/ wire _TADY_LINE_START_RST = nor2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.TOFU_VID_RSTp());
    /*p21.VOGA*/ _VOGA_RENDER_DONE_SYNC    = dff17_B(top.clk_reg.ALET_xBxDxFxH(), _TADY_LINE_START_RST, WODU_RENDER_DONEp());
    /*p21.WEGO*/ wire _WEGO_RENDER_DONE    = or2(top.clk_reg.TOFU_VID_RSTp(), _VOGA_RENDER_DONE_SYNC.qp());
    /*p21.XYMU*/ _XYMU_RENDERINGp          = nor_latch(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), _WEGO_RENDER_DONE);
  }

  //----------------------------------------
  // Background pipes

  {
    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOAD = not1(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.LUHE*/ wire BG_PIX_A0n = not1(top.vram_bus.LEGU_TILE_DA0.qn());
    /*p32.NOLY*/ wire BG_PIX_A1n = not1(top.vram_bus.NUDU_TILE_DA1.qn());
    /*p32.LEKE*/ wire BG_PIX_A2n = not1(top.vram_bus.MUKU_TILE_DA2.qn());
    /*p32.LOMY*/ wire BG_PIX_A3n = not1(top.vram_bus.LUZO_TILE_DA3.qn());
    /*p32.LALA*/ wire BG_PIX_A4n = not1(top.vram_bus.MEGU_TILE_DA4.qn());
    /*p32.LOXA*/ wire BG_PIX_A5n = not1(top.vram_bus.MYJY_TILE_DA5.qn());
    /*p32.NEZE*/ wire BG_PIX_A6n = not1(top.vram_bus.NASA_TILE_DA6.qn());
    /*p32.NOBO*/ wire BG_PIX_A7n = not1(top.vram_bus.NEFO_TILE_DA7.qn());

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.LEGU_TILE_DA0.qn());
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NUDU_TILE_DA1.qn());
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MUKU_TILE_DA2.qn());
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.LUZO_TILE_DA3.qn());
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MEGU_TILE_DA4.qn());
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.MYJY_TILE_DA5.qn());
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NASA_TILE_DA6.qn());
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand2(_LOZE_PIPE_A_LOAD, top.vram_bus.NEFO_TILE_DA7.qn());

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
    /*p32.LUXA*/ wire _LUXA_PIPE_B_LOAD = not1(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.TOSA*/ wire BG_PIX_B0n = not1(top.vram_bus.RAWU_TILE_DB0.qn());
    /*p32.RUCO*/ wire BG_PIX_B1n = not1(top.vram_bus.POZO_TILE_DB1.qn());
    /*p32.TYCE*/ wire BG_PIX_B2n = not1(top.vram_bus.PYZO_TILE_DB2.qn());
    /*p32.REVY*/ wire BG_PIX_B3n = not1(top.vram_bus.POXA_TILE_DB3.qn());
    /*p32.RYGA*/ wire BG_PIX_B4n = not1(top.vram_bus.PULO_TILE_DB4.qn());
    /*p32.RYLE*/ wire BG_PIX_B5n = not1(top.vram_bus.POJU_TILE_DB5.qn());
    /*p32.RAPU*/ wire BG_PIX_B6n = not1(top.vram_bus.POWY_TILE_DB6.qn());
    /*p32.SOJA*/ wire BG_PIX_B7n = not1(top.vram_bus.PYJU_TILE_DB7.qn());

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.RAWU_TILE_DB0.qn());
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POZO_TILE_DB1.qn());
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PYZO_TILE_DB2.qn());
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POXA_TILE_DB3.qn());
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PULO_TILE_DB4.qn());
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POJU_TILE_DB5.qn());
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.POWY_TILE_DB6.qn());
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand2(_LUXA_PIPE_B_LOAD, top.vram_bus.PYJU_TILE_DB7.qn());

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
    /*p29.WUTY*/ wire WUTY_SPRITE_DONEp   = not1(top.sprite_fetcher.VUSA_SPRITE_DONEn());
    /*p29.XEFY*/ wire _XEPY_SPRITE_DONEn  = not1(WUTY_SPRITE_DONEp);
    /*p34.MEFU*/ wire _MEFU_SPRITE_MASK0p = or3(_XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_A0.qp(), NURO_SPR_PIPE_B0.qp()); // def or
    /*p34.MEVE*/ wire _MEVE_SPRITE_MASK1p = or3(_XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_A1.qp(), MASO_SPR_PIPE_B1.qp());
    /*p34.MYZO*/ wire _MYZO_SPRITE_MASK2p = or3(_XEPY_SPRITE_DONEn, NATY_SPR_PIPE_A2.qp(), LEFE_SPR_PIPE_B2.qp());
    /*p34.RUDA*/ wire _RUDA_SPRITE_MASK3p = or3(_XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_A3.qp(), LESU_SPR_PIPE_B3.qp());
    /*p34.VOTO*/ wire _VOTO_SPRITE_MASK4p = or3(_XEPY_SPRITE_DONEn, VARE_SPR_PIPE_A4.qp(), WYHO_SPR_PIPE_B4.qp());
    /*p34.VYSA*/ wire _VYSA_SPRITE_MASK5p = or3(_XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_A5.qp(), WORA_SPR_PIPE_B5.qp());
    /*p34.TORY*/ wire _TORY_SPRITE_MASK6p = or3(_XEPY_SPRITE_DONEn, VANU_SPR_PIPE_A6.qp(), VAFO_SPR_PIPE_B6.qp());
    /*p34.WOPE*/ wire _WOPE_SPRITE_MASK7p = or3(_XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_A7.qp(), WUFY_SPR_PIPE_B7.qp());

    /*p34.LESY*/ wire _LESY_SPRITE_MASK0n = not1(_MEFU_SPRITE_MASK0p);
    /*p34.LOTA*/ wire _LOTA_SPRITE_MASK1n = not1(_MEVE_SPRITE_MASK1p);
    /*p34.LYKU*/ wire _LYKU_SPRITE_MASK2n = not1(_MYZO_SPRITE_MASK2p);
    /*p34.ROBY*/ wire _ROBY_SPRITE_MASK3n = not1(_RUDA_SPRITE_MASK3p);
    /*p34.TYTA*/ wire _TYTA_SPRITE_MASK4n = not1(_VOTO_SPRITE_MASK4p);
    /*p34.TYCO*/ wire _TYCO_SPRITE_MASK5n = not1(_VYSA_SPRITE_MASK5p);
    /*p34.SOKA*/ wire _SOKA_SPRITE_MASK6n = not1(_TORY_SPRITE_MASK6p);
    /*p34.XOVU*/ wire _XOVU_SPRITE_MASK7n = not1(_WOPE_SPRITE_MASK7p);

    // Sprite pipe A
    {
      /*p33.LOZA*/ wire SPR_PIX_A0n = not1(top.vram_bus.PEFO_SPRITE_DA0.qn());
      /*p33.SYBO*/ wire SPR_PIX_A1n = not1(top.vram_bus.ROKA_SPRITE_DA1.qn());
      /*p33.LUMO*/ wire SPR_PIX_A2n = not1(top.vram_bus.MYTU_SPRITE_DA2.qn());
      /*p33.SOLO*/ wire SPR_PIX_A3n = not1(top.vram_bus.RAMU_SPRITE_DA3.qn());
      /*p33.VOBY*/ wire SPR_PIX_A4n = not1(top.vram_bus.SELE_SPRITE_DA4.qn());
      /*p33.WYCO*/ wire SPR_PIX_A5n = not1(top.vram_bus.SUTO_SPRITE_DA5.qn());
      /*p33.SERY*/ wire SPR_PIX_A6n = not1(top.vram_bus.RAMA_SPRITE_DA6.qn());
      /*p33.SELU*/ wire SPR_PIX_A7n = not1(top.vram_bus.RYDU_SPRITE_DA7.qn());

      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand2(_LESY_SPRITE_MASK0n, top.vram_bus.PEFO_SPRITE_DA0.qn());
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand2(_LOTA_SPRITE_MASK1n, top.vram_bus.ROKA_SPRITE_DA1.qn());
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand2(_LYKU_SPRITE_MASK2n, top.vram_bus.MYTU_SPRITE_DA2.qn());
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand2(_ROBY_SPRITE_MASK3n, top.vram_bus.RAMU_SPRITE_DA3.qn());
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand2(_TYTA_SPRITE_MASK4n, top.vram_bus.SELE_SPRITE_DA4.qn());
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand2(_TYCO_SPRITE_MASK5n, top.vram_bus.SUTO_SPRITE_DA5.qn());
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand2(_SOKA_SPRITE_MASK6n, top.vram_bus.RAMA_SPRITE_DA6.qn());
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand2(_XOVU_SPRITE_MASK7n, top.vram_bus.RYDU_SPRITE_DA7.qn());

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand2(_LESY_SPRITE_MASK0n, SPR_PIX_A0n);
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand2(_LOTA_SPRITE_MASK1n, SPR_PIX_A1n);
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand2(_LYKU_SPRITE_MASK2n, SPR_PIX_A2n);
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand2(_ROBY_SPRITE_MASK3n, SPR_PIX_A3n);
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand2(_TYTA_SPRITE_MASK4n, SPR_PIX_A4n);
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand2(_TYCO_SPRITE_MASK5n, SPR_PIX_A5n);
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand2(_SOKA_SPRITE_MASK6n, SPR_PIX_A6n);
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand2(_XOVU_SPRITE_MASK7n, SPR_PIX_A7n);

      /*p33.NYLU*/ NYLU_SPR_PIPE_A0 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET0, SPR_PIX_A_RST0, GND);
      /*p33.PEFU*/ PEFU_SPR_PIPE_A1 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET1, SPR_PIX_A_RST1, NYLU_SPR_PIPE_A0.qp());
      /*p33.NATY*/ NATY_SPR_PIPE_A2 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET2, SPR_PIX_A_RST2, PEFU_SPR_PIPE_A1.qp());
      /*p33.PYJO*/ PYJO_SPR_PIPE_A3 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET3, SPR_PIX_A_RST3, NATY_SPR_PIPE_A2.qp());
      /*p33.VARE*/ VARE_SPR_PIPE_A4 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET4, SPR_PIX_A_RST4, PYJO_SPR_PIPE_A3.qp());
      /*p33.WEBA*/ WEBA_SPR_PIPE_A5 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET5, SPR_PIX_A_RST5, VARE_SPR_PIPE_A4.qp());
      /*p33.VANU*/ VANU_SPR_PIPE_A6 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET6, SPR_PIX_A_RST6, WEBA_SPR_PIPE_A5.qp());
      /*p33.VUPY*/ VUPY_SPR_PIPE_A7 = dff22(_SACU_CLKPIPEp, SPR_PIX_A_SET7, SPR_PIX_A_RST7, VANU_SPR_PIPE_A6.qp());
    }

    // Sprite pipe B
    {
      /*p33.RATA*/ wire SPR_PIX_B0n = not1(top.vram_bus.REWO_SPRITE_DB0.qn());
      /*p33.NUCA*/ wire SPR_PIX_B1n = not1(top.vram_bus.PEBA_SPRITE_DB1.qn());
      /*p33.LASE*/ wire SPR_PIX_B2n = not1(top.vram_bus.MOFO_SPRITE_DB2.qn());
      /*p33.LUBO*/ wire SPR_PIX_B3n = not1(top.vram_bus.PUDU_SPRITE_DB3.qn());
      /*p33.WERY*/ wire SPR_PIX_B4n = not1(top.vram_bus.SAJA_SPRITE_DB4.qn());
      /*p33.WURA*/ wire SPR_PIX_B5n = not1(top.vram_bus.SUNY_SPRITE_DB5.qn());
      /*p33.SULU*/ wire SPR_PIX_B6n = not1(top.vram_bus.SEMO_SPRITE_DB6.qn());
      /*p33.WAMY*/ wire SPR_PIX_B7n = not1(top.vram_bus.SEGA_SPRITE_DB7.qn());

      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand2(_LESY_SPRITE_MASK0n, top.vram_bus.REWO_SPRITE_DB0.qn());
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand2(_LOTA_SPRITE_MASK1n, top.vram_bus.PEBA_SPRITE_DB1.qn());
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand2(_LYKU_SPRITE_MASK2n, top.vram_bus.MOFO_SPRITE_DB2.qn());
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand2(_ROBY_SPRITE_MASK3n, top.vram_bus.PUDU_SPRITE_DB3.qn());
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand2(_TYTA_SPRITE_MASK4n, top.vram_bus.SAJA_SPRITE_DB4.qn());
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand2(_TYCO_SPRITE_MASK5n, top.vram_bus.SUNY_SPRITE_DB5.qn());
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand2(_SOKA_SPRITE_MASK6n, top.vram_bus.SEMO_SPRITE_DB6.qn());
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand2(_XOVU_SPRITE_MASK7n, top.vram_bus.SEGA_SPRITE_DB7.qn());

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand2(_LESY_SPRITE_MASK0n, SPR_PIX_B0n);
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand2(_LOTA_SPRITE_MASK1n, SPR_PIX_B1n);
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand2(_LYKU_SPRITE_MASK2n, SPR_PIX_B2n);
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand2(_ROBY_SPRITE_MASK3n, SPR_PIX_B3n);
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand2(_TYTA_SPRITE_MASK4n, SPR_PIX_B4n);
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand2(_TYCO_SPRITE_MASK5n, SPR_PIX_B5n);
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand2(_SOKA_SPRITE_MASK6n, SPR_PIX_B6n);
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand2(_XOVU_SPRITE_MASK7n, SPR_PIX_B7n);

      /*p33.NURO*/ NURO_SPR_PIPE_B0 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET0, SPR_PIX_B_RST0, GND);
      /*p33.MASO*/ MASO_SPR_PIPE_B1 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET1, SPR_PIX_B_RST1, NURO_SPR_PIPE_B0.qp());
      /*p33.LEFE*/ LEFE_SPR_PIPE_B2 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET2, SPR_PIX_B_RST2, MASO_SPR_PIPE_B1.qp());
      /*p33.LESU*/ LESU_SPR_PIPE_B3 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET3, SPR_PIX_B_RST3, LEFE_SPR_PIPE_B2.qp());
      /*p33.WYHO*/ WYHO_SPR_PIPE_B4 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET4, SPR_PIX_B_RST4, LESU_SPR_PIPE_B3.qp());
      /*p33.WORA*/ WORA_SPR_PIPE_B5 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET5, SPR_PIX_B_RST5, WYHO_SPR_PIPE_B4.qp());
      /*p33.VAFO*/ VAFO_SPR_PIPE_B6 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET6, SPR_PIX_B_RST6, WORA_SPR_PIPE_B5.qp());
      /*p33.WUFY*/ WUFY_SPR_PIPE_B7 = dff22(_SACU_CLKPIPEp, SPR_PIX_B_SET7, SPR_PIX_B_RST7, VAFO_SPR_PIPE_B6.qp());
    }

    // Palette pipe
    {
      wire GOMO_OBP = top.oam_bus.GOMO_OAM_DB4.qp();

      /*p34.SYPY*/ wire _SYPY = not1(GOMO_OBP);
      /*p34.TOTU*/ wire _TOTU = not1(GOMO_OBP);
      /*p34.NARO*/ wire _NARO = not1(GOMO_OBP);
      /*p34.WEXY*/ wire _WEXY = not1(GOMO_OBP);
      /*p34.RYZY*/ wire _RYZY = not1(GOMO_OBP);
      /*p34.RYFE*/ wire _RYFE = not1(GOMO_OBP);
      /*p34.LADY*/ wire _LADY = not1(GOMO_OBP);
      /*p34.LAFY*/ wire _LAFY = not1(GOMO_OBP);

      /*p34.PUME*/ wire _PUME_PAL_PIPE_SET0n = nand2(_LESY_SPRITE_MASK0n, GOMO_OBP);
      /*p34.SORO*/ wire _SORO_PAL_PIPE_SET1n = nand2(_LOTA_SPRITE_MASK1n, GOMO_OBP);
      /*p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n = nand2(_LYKU_SPRITE_MASK2n, GOMO_OBP);
      /*p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n = nand2(_ROBY_SPRITE_MASK3n, GOMO_OBP);
      /*p34.RORA*/ wire _RORA_PAL_PIPE_SET4n = nand2(_TYTA_SPRITE_MASK4n, GOMO_OBP);
      /*p34.MENE*/ wire _MENE_PAL_PIPE_SET5n = nand2(_TYCO_SPRITE_MASK5n, GOMO_OBP);
      /*p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n = nand2(_SOKA_SPRITE_MASK6n, GOMO_OBP);
      /*p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n = nand2(_XOVU_SPRITE_MASK7n, GOMO_OBP);

      /*p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n = nand2(_LESY_SPRITE_MASK0n, _SYPY);
      /*p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n = nand2(_LOTA_SPRITE_MASK1n, _TOTU);
      /*p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n = nand2(_LYKU_SPRITE_MASK2n, _NARO);
      /*p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n = nand2(_ROBY_SPRITE_MASK3n, _WEXY);
      /*p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n = nand2(_TYTA_SPRITE_MASK4n, _RYZY);
      /*p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n = nand2(_TYCO_SPRITE_MASK5n, _RYFE);
      /*p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n = nand2(_SOKA_SPRITE_MASK6n, _LADY);
      /*p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n = nand2(_XOVU_SPRITE_MASK7n, _LAFY);

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
      wire DEPO = top.oam_bus.DEPO_OAM_DB7.qp();

      /*p26.XOGA*/ wire _XOGA = not1(DEPO);
      /*p26.XURA*/ wire _XURA = not1(DEPO);
      /*p26.TAJO*/ wire _TAJO = not1(DEPO);
      /*p26.XENU*/ wire _XENU = not1(DEPO);
      /*p26.XYKE*/ wire _XYKE = not1(DEPO);
      /*p26.XABA*/ wire _XABA = not1(DEPO);
      /*p26.TAFU*/ wire _TAFU = not1(DEPO);
      /*p26.XUHO*/ wire _XUHO = not1(DEPO);

      /*p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0 = nand2(_LESY_SPRITE_MASK0n, DEPO);
      /*p26.XALA*/ wire _XALA_MASK_PIPE_SET1 = nand2(_LOTA_SPRITE_MASK1n, DEPO);
      /*p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2 = nand2(_LYKU_SPRITE_MASK2n, DEPO);
      /*p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3 = nand2(_ROBY_SPRITE_MASK3n, DEPO);
      /*p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4 = nand2(_TYTA_SPRITE_MASK4n, DEPO);
      /*p26.XELY*/ wire _XELY_MASK_PIPE_SET5 = nand2(_TYCO_SPRITE_MASK5n, DEPO);
      /*p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6 = nand2(_SOKA_SPRITE_MASK6n, DEPO);
      /*p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7 = nand2(_XOVU_SPRITE_MASK7n, DEPO);

      /*p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0 = nand2(_LESY_SPRITE_MASK0n, _XOGA);
      /*p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1 = nand2(_LOTA_SPRITE_MASK1n, _XURA);
      /*p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2 = nand2(_LYKU_SPRITE_MASK2n, _TAJO);
      /*p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3 = nand2(_ROBY_SPRITE_MASK3n, _XENU);
      /*p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4 = nand2(_TYTA_SPRITE_MASK4n, _XYKE);
      /*p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5 = nand2(_TYCO_SPRITE_MASK5n, _XABA);
      /*p26.TENA*/ wire _TENA_MASK_PIPE_RST6 = nand2(_SOKA_SPRITE_MASK6n, _TAFU);
      /*p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7 = nand2(_XOVU_SPRITE_MASK7n, _XUHO);

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
    // RAJY := and(

    /*p35.RAJY*/ wire RAJY_PIX_BG_LOp  = and2(VYXE_LCDC_BGEN.qp(), PYBO_BG_PIPE_A7.qp());
    /*p35.TADE*/ wire TADE_PIX_BG_HIp  = and2(VYXE_LCDC_BGEN.qp(), SOHU_BG_PIPE_B7.qp());

    /*p35.WOXA*/ wire WOXA_PIX_SP_LOp  = and2(XYLO_LCDC_SPEN.qp(), VUPY_SPR_PIPE_A7.qp());
    /*p35.XULA*/ wire XULA_PIX_SP_HIp  = and2(XYLO_LCDC_SPEN.qp(), WUFY_SPR_PIPE_B7.qp());

    /*p35.NULY*/ wire NULY_PIX_SP_MASKn  = nor2(WOXA_PIX_SP_LOp, XULA_PIX_SP_HIp);

    /*p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, VAVA_MASK_PIPE_7.qp());
    /*p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, VAVA_MASK_PIPE_7.qp());
    /*p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RYFU_MASK_BG0, RUTA_MASK_BG1);

    /*p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(LYME_PAL_PIPE_7.qp());
    /*p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
    /*p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    // VUMU := not(WOXA)
    // WYRU := not(VUMU)
    // WELE := not(XULA)
    // WOLO := not(WELE)
    // LAVA := not(LAFU)

    /*p35.VUMU*/ wire VUMU_PIX_SP_LOn = not1(WOXA_PIX_SP_LOp);
    /*p35.WYRU*/ wire WYRU_PIX_SP_LOp = not1(VUMU_PIX_SP_LOn);
    /*p35.WELE*/ wire WELE_PIX_SP_HIn = not1(XULA_PIX_SP_HIp);
    /*p35.WOLO*/ wire WOLO_PIX_SP_HIp = not1(WELE_PIX_SP_HIn);

    /*p35.LAVA*/ wire LAVA_MASK_OPB0  = not1(LAFU_OBP0PIXELn);

    // VUGO := and(VUMU, WELE, LAVA)
    // VOLO := and(VUMU, WOLO, LAVA)
    // VATA := and(WYRU, WELE, LAVA)
    // VYRO := and(WYRU, WOLO, LAVA)

    /*p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_LOn, WELE_PIX_SP_HIn, LAVA_MASK_OPB0); // does not have vcc arm
    /*p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_LOn, WOLO_PIX_SP_HIp, LAVA_MASK_OPB0); // does not have vcc arm
    /*p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_LOp, WELE_PIX_SP_HIn, LAVA_MASK_OPB0); // does not have vcc arm
    /*p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_LOp, WOLO_PIX_SP_HIp, LAVA_MASK_OPB0); // does not have vcc arm

    // WUFU := amux4(XANA_QN, VYRO, XYZE_QN, VATA, XALO_QN, VOLO, XUKY_QN, VUGO)

    /*p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(XANA_OBP0_D7.qn(), VYRO_PAL_OBP0D,
                                               XYZE_OBP0_D5.qn(), VATA_PAL_OBP0C,
                                               XALO_OBP0_D3.qn(), VOLO_PAL_OBP0B,
                                               XUKY_OBP0_D1.qn(), VUGO_PAL_OBP0A);

    // WALY := VYRO, XUPO_QN, VATA, XERU_QN, VOLO, XOVA_QN, VUGO, XUFU_QN);

    /*p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(XUPO_OBP0_D6.qn(), VYRO_PAL_OBP0D,
                                               XERU_OBP0_D4.qn(), VATA_PAL_OBP0C,
                                               XOVA_OBP0_D2.qn(), VOLO_PAL_OBP0B,
                                               XUFU_OBP0_D0.qn(), VUGO_PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    // MEXA := not(WOXA)
    // LOZO := not(MEXA)
    // MABY := not(XULA)
    // LYLE := not(MABY)

    /*p35.MEXA*/ wire MEXA_PIX_SP_LOn = not1(WOXA_PIX_SP_LOp);
    /*p35.LOZO*/ wire LOZO_PIX_SP_LOp = not1(MEXA_PIX_SP_LOn);
    /*p35.MABY*/ wire MABY_PIX_SP_HIn = not1(XULA_PIX_SP_HIp);
    /*p35.LYLE*/ wire LYLE_PIX_SP_HIp = not1(MABY_PIX_SP_HIn);

    // LUKU := not(LEKA)
    /*p35.LUKU*/ wire LUKU_MASK_OBP1  = not1(LEKA_OBP1PIXELn);

    // LOPU := and(MEXA, MABY, LUKU)
    // LARU := and(LOZO, MABY, LUKU)
    // LYKY := and(MEXA, LYLE, LUKU)
    // LEDO := and(LOZO, LYLE, LUKU)

    /*p35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MABY_PIX_SP_HIn, MEXA_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p35.LYKY*/ wire LYKY_PAL_OBP1B = and3(LYLE_PIX_SP_HIp, MEXA_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p35.LARU*/ wire LARU_PAL_OBP1C = and3(MABY_PIX_SP_HIn, LOZO_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
    /*p35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LYLE_PIX_SP_HIp, LOZO_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

    // MOKA := amux4(LUXO_QN, LEDO, LUGU_QN, LARU, LOSE_QN, LYKY, LAWO_QN, LOPU)

    /*p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(LUXO_OBP1_D7.qn(), LEDO_PAL_OBP1D,
                                               LUGU_OBP1_D5.qn(), LARU_PAL_OBP1C,
                                               LOSE_OBP1_D3.qn(), LYKY_PAL_OBP1B,
                                               LAWO_OBP1_D1.qn(), LOPU_PAL_OBP1A);
    
    // MUFA := amux4(LEDO, LEPU_QN, LARU, LUNE_QN, LYKY, MOSA_QN, LOPU, MOXY_QN)

    /*p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEPU_OBP1_D6.qn(), LEDO_PAL_OBP1D,
                                               LUNE_OBP1_D4.qn(), LARU_PAL_OBP1C,
                                               MOSA_OBP1_D2.qn(), LYKY_PAL_OBP1B,
                                               MOXY_OBP1_D0.qn(), LOPU_PAL_OBP1A);

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

    // NELO := amux4(NYPO, MOGY_QN, NUMA, MUKE_QN, NUXO, PYLU_QN, POBU, PAVO_QN);

    /*p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, MOGY_BGP_D6.qn(),
                                             NUMA_PAL_BGPC, MUKE_BGP_D4.qn(),
                                             NUXO_PAL_BGPB, PYLU_BGP_D2.qn(),
                                             POBU_PAL_BGPA, PAVO_BGP_D0.qn());

    // NURA := amux4(MENA_QN, NYPO, MORU_QN, NUMA, MAXY_QN, NUXO, NUSY_QN, POBU);

    /*p35.NURA*/ wire NURA_COL_BG_HI = amux4(MENA_BGP_D7.qn(), NYPO_PAL_BGPD,
                                             MORU_BGP_D5.qn(), NUMA_PAL_BGPC,
                                             MAXY_BGP_D3.qn(), NUXO_PAL_BGPB,
                                             NUSY_BGP_D1.qn(), POBU_PAL_BGPA);

    //----------
    // Pixel merge and send

    // PERO := or(NELO, WALY, MUFA)
    // PATY := or(NURA, WUFU, MOKA)
    // REMY := not(PERO)
    // RAVO := not(PATY)

    /*p35.PERO*/ wire PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
    /*p35.PATY*/ wire PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

    /*p35.REMY*/ wire REMY_LD0n = not1(PERO_COL_LO);
    /*p35.RAVO*/ wire RAVO_LD1n = not1(PATY_COL_HI);
    LCD_PIN_LD0 = REMY_LD0n;
    LCD_PIN_LD1 = RAVO_LD1n;
  }

  //----------------------------------------
  // FF40 LCDC

  {
    /*p22.WORU*/ wire _WORU_FF40n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not1(_WORU_FF40n);

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and2(_VOCA_FF40p, top.ASOT_CPU_RDp());
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);

    /*p23.WYPO*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_WYCE_FF40_RDn, VYXE_LCDC_BGEN.qp());
    /*p23.XERO*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_WYCE_FF40_RDn, XYLO_LCDC_SPEN.qp());
    /*p23.WYJU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZE.qp());
    /*p23.WUKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_WYCE_FF40_RDn, XAFO_LCDC_BGMAP.qp());
    /*p23.VOKE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_WYCE_FF40_RDn, WEXU_LCDC_BGTILE.qp());
    /*p23.VATO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_WYCE_FF40_RDn, WYMO_LCDC_WINEN.qp());
    /*p23.VAHA*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_WYCE_FF40_RDn, WOKY_LCDC_WINMAP.qp());
    /*p23.XEBU*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_WYCE_FF40_RDn, XONA_LCDC_EN.qp());

    /*p23.WARU*/ wire _WARU_FF40_WRp = and2(_VOCA_FF40p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not1(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not1(top.clk_reg.XORE_SYS_RSTp());
    /*p23.VYXE*/ VYXE_LCDC_BGEN    = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D0.qp());
    /*p23.XYLO*/ XYLO_LCDC_SPEN    = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D1.qp());
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D2.qp());
    /*p23.XAFO*/ XAFO_LCDC_BGMAP   = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D3.qp());
    /*p23.WEXU*/ WEXU_LCDC_BGTILE  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D4.qp());
    /*p23.WYMO*/ WYMO_LCDC_WINEN   = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D5.qp());
    /*p23.WOKY*/ WOKY_LCDC_WINMAP  = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D6.qp());
    /*p23.XONA*/ XONA_LCDC_EN      = dff9(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.CPU_BUS_D7.qp());
  }

  //----------------------------------------
  // FF41 STAT

  {
    /*p22.WOFA*/ wire _WOFA_FF41n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VARY*/ wire _VARY_FF41p = not1(_WOFA_FF41n);

    /*p21.TOBE*/ wire _TOBE_FF41_RDp = and2(_VARY_FF41p, top.ASOT_CPU_RDp()); // die AND
    /*p21.VAVE*/ wire _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp); // die INV

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

    // These tribufs are _different_

    /*p21.TEBY*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TOBE_FF41_RDp, not1(_SADU_STAT_MODE0n));
    /*p21.WUGA*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TOBE_FF41_RDp, not1(_XATY_STAT_MODE1n));
    /*p21.SEGO*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TOBE_FF41_RDp, not1(_RUPO_LYC_MATCH_LATCHn.qp()));
    /*p21.PUZO*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_VAVE_FF41_RDn, ROXE_INT_HBL_EN.qp());
    /*p21.POFO*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_VAVE_FF41_RDn, RUFO_INT_VBL_EN.qp());
    /*p21.SASY*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_VAVE_FF41_RDn, REFE_INT_OAM_EN.qp());
    /*p21.POTE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_VAVE_FF41_RDn, RUGU_INT_LYC_EN.qp());
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

    /*p36.RARO*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_TEPY_FF47_RDn, PAVO_BGP_D0.qp());
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

    /*p36.XARY*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_XOZY_FF48_RDn, XUFU_OBP0_D0.qp());
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

    /*p36.LAJU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_LOTE_FF49_RDn, MOXY_OBP1_D0.qp());
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
    /*p22.WYVO*/ wire _FF4An = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VYGA*/ wire _FF4Ap = not1(_FF4An);

    /*p23.WAXU*/ wire _FF4A_RDp = and2(_FF4Ap, top.ASOT_CPU_RDp());
    /*p23.VOMY*/ wire _FF4A_RDn = not1(_FF4A_RDp);

    /*p23.WEKO*/ wire _WEKO_FF4A_WRp = and2(_FF4Ap, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.VEFU*/ wire _VEFU_FF4A_WRn = not1(_WEKO_FF4A_WRp);

    /*p23.NESO*/ NESO_WY0 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.NYRO*/ NYRO_WY1 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.NAGA*/ NAGA_WY2 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.MELA*/ MELA_WY3 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.NULO*/ NULO_WY4 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.NENE*/ NENE_WY5 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.NUKA*/ NUKA_WY6 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.NAFU*/ NAFU_WY7 = dff9(_VEFU_FF4A_WRn, !_VEFU_FF4A_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());


    /*p23.PUNU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_FF4A_RDn, NESO_WY0.qp());
    /*p23.PODA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_FF4A_RDn, NYRO_WY1.qp());
    /*p23.PYGU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_FF4A_RDn, NAGA_WY2.qp());
    /*p23.LOKA*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_FF4A_RDn, MELA_WY3.qp());
    /*p23.MEGA*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_FF4A_RDn, NULO_WY4.qp());
    /*p23.PELA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_FF4A_RDn, NENE_WY5.qp());
    /*p23.POLO*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_FF4A_RDn, NUKA_WY6.qp());
    /*p23.MERA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_FF4A_RDn, NAFU_WY7.qp());
  }

  //----------------------------------------
  // FF4B WX

  {
    /*p22.WAGE*/ wire _FF4Bn = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.WEPO_A03p());
    /*p22.VUMY*/ wire _FF4Bp = not1(_FF4Bn);

    /*p23.WYZE*/ wire _FF4B_RDp = and2(_FF4Bp, top.ASOT_CPU_RDp());
    /*p23.VYCU*/ wire _FF4B_RDn = not1(_FF4B_RDp);

    /*p23.WUZA*/ wire _WUZA_FF4B_WRp = and2(_FF4Bp, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.VOXU*/ wire _VOXU_FF4B_WRn = not1(_WUZA_FF4B_WRp);

    /*p23.MYPA*/ MYPA_WX0 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.NOFE*/ NOFE_WX1 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.NOKE*/ NOKE_WX2 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.MEBY*/ MEBY_WX3 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.MYPU*/ MYPU_WX4 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.MYCE*/ MYCE_WX5 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.MUVO*/ MUVO_WX6 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.NUKU*/ NUKU_WX7 = dff9(_VOXU_FF4B_WRn, !_VOXU_FF4B_WRn, top.clk_reg.WALU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());


    /*p23.LOVA*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_FF4B_RDn, MYPA_WX0.qp());
    /*p23.MUKA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_FF4B_RDn, NOFE_WX1.qp());
    /*p23.MOKO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_FF4B_RDn, NOKE_WX2.qp());
    /*p23.LOLE*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_FF4B_RDn, MEBY_WX3.qp());
    /*p23.MELE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_FF4B_RDn, MYPU_WX4.qp());
    /*p23.MUFE*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_FF4B_RDn, MYCE_WX5.qp());
    /*p23.MULY*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_FF4B_RDn, MUVO_WX6.qp());
    /*p23.MARA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_FF4B_RDn, NUKU_WX7.qp());
  }
}

//------------------------------------------------------------------------------
