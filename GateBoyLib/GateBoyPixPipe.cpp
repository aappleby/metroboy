#include "GateBoyLib/GateBoyPixPipe.h"

#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyOamBus.h"

// Sprite temp loads  on AB/CD/EF/GH
// Tile   temp loads  on AB/CD/EF/GH
// Pipes       shifts on HA/BC/DE/FG
// WUTY        fires  on HA/BC/DE/FG
// NYXU        fires  on HA/BC/DE/FG

//------------------------------------------------------------------------------------------------------------------------

void WindowRegisters::tock(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  wire2 XYMU_RENDERINGp,
  wire2 TYFA_CLKPIPE_odd,
  wire2 NUKO_WX_MATCHp_old,
  wire2 ROZE_FINE_COUNT_7n_old,
  wire2 RYDY_WIN_HITp_old)
{
  /*#p24.SEGU*/ wire2 _SEGU_CLKPIPE_evn = not1b(TYFA_CLKPIPE_odd);
  /* p27.ROCO*/ wire2 _ROCO_CLKPIPE_odd = not1b(_SEGU_CLKPIPE_evn);
  /* p27.PYCO*/ PYCO_WIN_MATCHp.dff17(_ROCO_CLKPIPE_odd, rst.XAPO_VID_RSTn(), NUKO_WX_MATCHp_old);

  /* p27.SOVY*/ SOVY_WIN_HITp.dff17(clk.ALET_xBxDxFxH(), rst.XAPO_VID_RSTn(), RYDY_WIN_HITp_old);

  /* p27.PANY*/ wire2 _PANY_WIN_FETCHn_old = nor2b(NUKO_WX_MATCHp_old, ROZE_FINE_COUNT_7n_old);
  /* p27.RYFA*/ RYFA_WIN_FETCHn_A.dff17(_SEGU_CLKPIPE_evn, XYMU_RENDERINGp, _PANY_WIN_FETCHn_old);
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock_bgw_pipe(
  TileTempA& tile_temp_a,
  TileTempB& tile_temp_b,
  wire2 SACU_CLKPIPE_evn,
  wire2 NYXU_BFETCH_RSTn)
{
  wire2 GND = 0;

  {
    /* p32.LOZE*/ wire2 _LOZE_PIPE_A_LOADp = not1b(NYXU_BFETCH_RSTn);
    /* p32.LAKY*/ wire2 _LAKY_BG_PIX_SET0 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.LEGU_TILE_DA0n.qn_new2());
    /* p32.NYXO*/ wire2 _NYXO_BG_PIX_SET1 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.NUDU_TILE_DA1n.qn_new2());
    /* p32.LOTO*/ wire2 _LOTO_BG_PIX_SET2 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.MUKU_TILE_DA2n.qn_new2());
    /* p32.LYDU*/ wire2 _LYDU_BG_PIX_SET3 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.LUZO_TILE_DA3n.qn_new2());
    /* p32.MYVY*/ wire2 _MYVY_BG_PIX_SET4 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.MEGU_TILE_DA4n.qn_new2());
    /* p32.LODO*/ wire2 _LODO_BG_PIX_SET5 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.MYJY_TILE_DA5n.qn_new2());
    /* p32.NUTE*/ wire2 _NUTE_BG_PIX_SET6 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.NASA_TILE_DA6n.qn_new2());
    /* p32.NAJA*/ wire2 _NAJA_BG_PIX_SET7 = nand2b(_LOZE_PIPE_A_LOADp, tile_temp_a.NEFO_TILE_DA7n.qn_new2());

    /* p32.LUHE*/ wire2 _LUHE_BG_PIX_DA0n = not1b(tile_temp_a.LEGU_TILE_DA0n.qn_new2());
    /* p32.NOLY*/ wire2 _NOLY_BG_PIX_DA1n = not1b(tile_temp_a.NUDU_TILE_DA1n.qn_new2());
    /* p32.LEKE*/ wire2 _LEKE_BG_PIX_DA2n = not1b(tile_temp_a.MUKU_TILE_DA2n.qn_new2());
    /* p32.LOMY*/ wire2 _LOMY_BG_PIX_DA3n = not1b(tile_temp_a.LUZO_TILE_DA3n.qn_new2());
    /* p32.LALA*/ wire2 _LALA_BG_PIX_DA4n = not1b(tile_temp_a.MEGU_TILE_DA4n.qn_new2());
    /* p32.LOXA*/ wire2 _LOXA_BG_PIX_DA5n = not1b(tile_temp_a.MYJY_TILE_DA5n.qn_new2());
    /* p32.NEZE*/ wire2 _NEZE_BG_PIX_DA6n = not1b(tile_temp_a.NASA_TILE_DA6n.qn_new2());
    /* p32.NOBO*/ wire2 _NOBO_BG_PIX_DA7n = not1b(tile_temp_a.NEFO_TILE_DA7n.qn_new2());

    /* p32.LOTY*/ wire2 _LOTY_BG_PIX_RST0 = nand2b(_LOZE_PIPE_A_LOADp, _LUHE_BG_PIX_DA0n);
    /* p32.NEXA*/ wire2 _NEXA_BG_PIX_RST1 = nand2b(_LOZE_PIPE_A_LOADp, _NOLY_BG_PIX_DA1n);
    /* p32.LUTU*/ wire2 _LUTU_BG_PIX_RST2 = nand2b(_LOZE_PIPE_A_LOADp, _LEKE_BG_PIX_DA2n);
    /* p32.LUJA*/ wire2 _LUJA_BG_PIX_RST3 = nand2b(_LOZE_PIPE_A_LOADp, _LOMY_BG_PIX_DA3n);
    /* p32.MOSY*/ wire2 _MOSY_BG_PIX_RST4 = nand2b(_LOZE_PIPE_A_LOADp, _LALA_BG_PIX_DA4n);
    /* p32.LERU*/ wire2 _LERU_BG_PIX_RST5 = nand2b(_LOZE_PIPE_A_LOADp, _LOXA_BG_PIX_DA5n);
    /* p32.NYHA*/ wire2 _NYHA_BG_PIX_RST6 = nand2b(_LOZE_PIPE_A_LOADp, _NEZE_BG_PIX_DA6n);
    /* p32.NADY*/ wire2 _NADY_BG_PIX_RST7 = nand2b(_LOZE_PIPE_A_LOADp, _NOBO_BG_PIX_DA7n);

    /* p32.PYBO*/ PYBO_BGW_PIPE_A7.dff22(SACU_CLKPIPE_evn, _NAJA_BG_PIX_SET7, _NADY_BG_PIX_RST7, NEDA_BGW_PIPE_A6.qp_old2());
    /* p32.NEDA*/ NEDA_BGW_PIPE_A6.dff22(SACU_CLKPIPE_evn, _NUTE_BG_PIX_SET6, _NYHA_BG_PIX_RST6, MODU_BGW_PIPE_A5.qp_old2());
    /* p32.MODU*/ MODU_BGW_PIPE_A5.dff22(SACU_CLKPIPE_evn, _LODO_BG_PIX_SET5, _LERU_BG_PIX_RST5, NEPO_BGW_PIPE_A4.qp_old2());
    /* p32.NEPO*/ NEPO_BGW_PIPE_A4.dff22(SACU_CLKPIPE_evn, _MYVY_BG_PIX_SET4, _MOSY_BG_PIX_RST4, MACU_BGW_PIPE_A3.qp_old2());
    /* p32.MACU*/ MACU_BGW_PIPE_A3.dff22(SACU_CLKPIPE_evn, _LYDU_BG_PIX_SET3, _LUJA_BG_PIX_RST3, MOJU_BGW_PIPE_A2.qp_old2());
    /* p32.MOJU*/ MOJU_BGW_PIPE_A2.dff22(SACU_CLKPIPE_evn, _LOTO_BG_PIX_SET2, _LUTU_BG_PIX_RST2, NOZO_BGW_PIPE_A1.qp_old2());
    /* p32.NOZO*/ NOZO_BGW_PIPE_A1.dff22(SACU_CLKPIPE_evn, _NYXO_BG_PIX_SET1, _NEXA_BG_PIX_RST1, MYDE_BGW_PIPE_A0.qp_old2());
    /* p32.MYDE*/ MYDE_BGW_PIPE_A0.dff22(SACU_CLKPIPE_evn, _LAKY_BG_PIX_SET0, _LOTY_BG_PIX_RST0, GND);
  }

  {
    /* p32.LUXA*/ wire2 _LUXA_PIPE_B_LOADp = not1b(NYXU_BFETCH_RSTn);
    /* p32.TUXE*/ wire2 _TUXE_BG_PIX_SET0 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.RAWU_TILE_DB0p.qp_new2());
    /* p32.SOLY*/ wire2 _SOLY_BG_PIX_SET1 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.POZO_TILE_DB1p.qp_new2());
    /* p32.RUCE*/ wire2 _RUCE_BG_PIX_SET2 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.PYZO_TILE_DB2p.qp_new2());
    /* p32.RYJA*/ wire2 _RYJA_BG_PIX_SET3 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.POXA_TILE_DB3p.qp_new2());
    /* p32.RUTO*/ wire2 _RUTO_BG_PIX_SET4 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.PULO_TILE_DB4p.qp_new2());
    /* p32.RAJA*/ wire2 _RAJA_BG_PIX_SET5 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.POJU_TILE_DB5p.qp_new2());
    /* p32.RAJO*/ wire2 _RAJO_BG_PIX_SET6 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.POWY_TILE_DB6p.qp_new2());
    /* p32.RAGA*/ wire2 _RAGA_BG_PIX_SET7 = nand2b(_LUXA_PIPE_B_LOADp, tile_temp_b.PYJU_TILE_DB7p.qp_new2());

    /* p32.TOSA*/ wire2 _TOSA_BG_PIX_DB0n = not1b(tile_temp_b.RAWU_TILE_DB0p.qp_new2());
    /* p32.RUCO*/ wire2 _RUCO_BG_PIX_DB1n = not1b(tile_temp_b.POZO_TILE_DB1p.qp_new2());
    /* p32.TYCE*/ wire2 _TYCE_BG_PIX_DB2n = not1b(tile_temp_b.PYZO_TILE_DB2p.qp_new2());
    /* p32.REVY*/ wire2 _REVY_BG_PIX_DB3n = not1b(tile_temp_b.POXA_TILE_DB3p.qp_new2());
    /* p32.RYGA*/ wire2 _RYGA_BG_PIX_DB4n = not1b(tile_temp_b.PULO_TILE_DB4p.qp_new2());
    /* p32.RYLE*/ wire2 _RYLE_BG_PIX_DB5n = not1b(tile_temp_b.POJU_TILE_DB5p.qp_new2());
    /* p32.RAPU*/ wire2 _RAPU_BG_PIX_DB6n = not1b(tile_temp_b.POWY_TILE_DB6p.qp_new2());
    /* p32.SOJA*/ wire2 _SOJA_BG_PIX_DB7n = not1b(tile_temp_b.PYJU_TILE_DB7p.qp_new2());

    /* p32.SEJA*/ wire2 _SEJA_BG_PIX_RST0 = nand2b(_LUXA_PIPE_B_LOADp, _TOSA_BG_PIX_DB0n);
    /* p32.SENO*/ wire2 _SENO_BG_PIX_RST1 = nand2b(_LUXA_PIPE_B_LOADp, _RUCO_BG_PIX_DB1n);
    /* p32.SURE*/ wire2 _SURE_BG_PIX_RST2 = nand2b(_LUXA_PIPE_B_LOADp, _TYCE_BG_PIX_DB2n);
    /* p32.SEBO*/ wire2 _SEBO_BG_PIX_RST3 = nand2b(_LUXA_PIPE_B_LOADp, _REVY_BG_PIX_DB3n);
    /* p32.SUCA*/ wire2 _SUCA_BG_PIX_RST4 = nand2b(_LUXA_PIPE_B_LOADp, _RYGA_BG_PIX_DB4n);
    /* p32.SYWE*/ wire2 _SYWE_BG_PIX_RST5 = nand2b(_LUXA_PIPE_B_LOADp, _RYLE_BG_PIX_DB5n);
    /* p32.SUPU*/ wire2 _SUPU_BG_PIX_RST6 = nand2b(_LUXA_PIPE_B_LOADp, _RAPU_BG_PIX_DB6n);
    /* p32.RYJY*/ wire2 _RYJY_BG_PIX_RST7 = nand2b(_LUXA_PIPE_B_LOADp, _SOJA_BG_PIX_DB7n);

    /* p32.SOHU*/ SOHU_BGW_PIPE_B7.dff22(SACU_CLKPIPE_evn, _RAGA_BG_PIX_SET7, _RYJY_BG_PIX_RST7, RALU_BGW_PIPE_B6.qp_old2());
    /* p32.RALU*/ RALU_BGW_PIPE_B6.dff22(SACU_CLKPIPE_evn, _RAJO_BG_PIX_SET6, _SUPU_BG_PIX_RST6, SETU_BGW_PIPE_B5.qp_old2());
    /* p32.SETU*/ SETU_BGW_PIPE_B5.dff22(SACU_CLKPIPE_evn, _RAJA_BG_PIX_SET5, _SYWE_BG_PIX_RST5, SOBO_BGW_PIPE_B4.qp_old2());
    /* p32.SOBO*/ SOBO_BGW_PIPE_B4.dff22(SACU_CLKPIPE_evn, _RUTO_BG_PIX_SET4, _SUCA_BG_PIX_RST4, RYSA_BGW_PIPE_B3.qp_old2());
    /* p32.RYSA*/ RYSA_BGW_PIPE_B3.dff22(SACU_CLKPIPE_evn, _RYJA_BG_PIX_SET3, _SEBO_BG_PIX_RST3, SADY_BGW_PIPE_B2.qp_old2());
    /* p32.SADY*/ SADY_BGW_PIPE_B2.dff22(SACU_CLKPIPE_evn, _RUCE_BG_PIX_SET2, _SURE_BG_PIX_RST2, TACA_BGW_PIPE_B1.qp_old2());
    /* p32.TACA*/ TACA_BGW_PIPE_B1.dff22(SACU_CLKPIPE_evn, _SOLY_BG_PIX_SET1, _SENO_BG_PIX_RST1, TOMY_BGW_PIPE_B0.qp_old2());
    /* p32.TOMY*/ TOMY_BGW_PIPE_B0.dff22(SACU_CLKPIPE_evn, _TUXE_BG_PIX_SET0, _SEJA_BG_PIX_RST0, GND);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock_sprite_pipe(
  SpritePixA& sprite_pix_a,
  SpritePixB& sprite_pix_b,
  wire2 WUTY_SFETCH_DONE_TRIGp,
  wire2 SACU_CLKPIPE_evn)
{
  wire2 GND = 0;

  /* p29.XEFY*/ wire2 _XEPY_SPRITE_DONEn  = not1b(WUTY_SFETCH_DONE_TRIGp);
  /* p34.MEFU*/ wire2 _MEFU_SPRITE_MASK0n = or3(_XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp_old2(), NURO_SPR_PIPE_A0.qp_old2());
  /* p34.MEVE*/ wire2 _MEVE_SPRITE_MASK1n = or3(_XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_B1.qp_old2(), MASO_SPR_PIPE_A1.qp_old2());
  /* p34.MYZO*/ wire2 _MYZO_SPRITE_MASK2n = or3(_XEPY_SPRITE_DONEn, NATY_SPR_PIPE_B2.qp_old2(), LEFE_SPR_PIPE_A2.qp_old2());
  /* p34.RUDA*/ wire2 _RUDA_SPRITE_MASK3n = or3(_XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_B3.qp_old2(), LESU_SPR_PIPE_A3.qp_old2());
  /* p34.VOTO*/ wire2 _VOTO_SPRITE_MASK4n = or3(_XEPY_SPRITE_DONEn, VARE_SPR_PIPE_B4.qp_old2(), WYHO_SPR_PIPE_A4.qp_old2());
  /* p34.VYSA*/ wire2 _VYSA_SPRITE_MASK5n = or3(_XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_B5.qp_old2(), WORA_SPR_PIPE_A5.qp_old2());
  /* p34.TORY*/ wire2 _TORY_SPRITE_MASK6n = or3(_XEPY_SPRITE_DONEn, VANU_SPR_PIPE_B6.qp_old2(), VAFO_SPR_PIPE_A6.qp_old2());
  /* p34.WOPE*/ wire2 _WOPE_SPRITE_MASK7n = or3(_XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_B7.qp_old2(), WUFY_SPR_PIPE_A7.qp_old2());

  /* p34.LESY*/ wire2 _LESY_SPRITE_MASK0p = not1b(_MEFU_SPRITE_MASK0n);
  /* p34.LOTA*/ wire2 _LOTA_SPRITE_MASK1p = not1b(_MEVE_SPRITE_MASK1n);
  /* p34.LYKU*/ wire2 _LYKU_SPRITE_MASK2p = not1b(_MYZO_SPRITE_MASK2n);
  /* p34.ROBY*/ wire2 _ROBY_SPRITE_MASK3p = not1b(_RUDA_SPRITE_MASK3n);
  /* p34.TYTA*/ wire2 _TYTA_SPRITE_MASK4p = not1b(_VOTO_SPRITE_MASK4n);
  /* p34.TYCO*/ wire2 _TYCO_SPRITE_MASK5p = not1b(_VYSA_SPRITE_MASK5n);
  /* p34.SOKA*/ wire2 _SOKA_SPRITE_MASK6p = not1b(_TORY_SPRITE_MASK6n);
  /* p34.XOVU*/ wire2 _XOVU_SPRITE_MASK7p = not1b(_WOPE_SPRITE_MASK7n);

  // Sprite pipe A
  {
    /* p33.PABE*/ wire2 _PABE_SPR_PIX_SET0 = nand2b(_LESY_SPRITE_MASK0p, sprite_pix_a.REWO_SPRITE_DA0n.qn_new2());
    /* p33.MYTO*/ wire2 _MYTO_SPR_PIX_SET1 = nand2b(_LOTA_SPRITE_MASK1p, sprite_pix_a.PEBA_SPRITE_DA1n.qn_new2());
    /* p33.LELA*/ wire2 _LELA_SPR_PIX_SET2 = nand2b(_LYKU_SPRITE_MASK2p, sprite_pix_a.MOFO_SPRITE_DA2n.qn_new2());
    /* p33.MAME*/ wire2 _MAME_SPR_PIX_SET3 = nand2b(_ROBY_SPRITE_MASK3p, sprite_pix_a.PUDU_SPRITE_DA3n.qn_new2());
    /* p33.VEXU*/ wire2 _VEXU_SPR_PIX_SET4 = nand2b(_TYTA_SPRITE_MASK4p, sprite_pix_a.SAJA_SPRITE_DA4n.qn_new2());
    /* p33.VABY*/ wire2 _VABY_SPR_PIX_SET5 = nand2b(_TYCO_SPRITE_MASK5p, sprite_pix_a.SUNY_SPRITE_DA5n.qn_new2());
    /* p33.TUXA*/ wire2 _TUXA_SPR_PIX_SET6 = nand2b(_SOKA_SPRITE_MASK6p, sprite_pix_a.SEMO_SPRITE_DA6n.qn_new2());
    /* p33.VUNE*/ wire2 _VUNE_SPR_PIX_SET7 = nand2b(_XOVU_SPRITE_MASK7p, sprite_pix_a.SEGA_SPRITE_DA7n.qn_new2());

    /* p33.RATA*/ wire2 _RATA_SPR_PIX_DA0n = not1b(sprite_pix_a.REWO_SPRITE_DA0n.qn_new2());
    /* p33.NUCA*/ wire2 _NUCA_SPR_PIX_DA1n = not1b(sprite_pix_a.PEBA_SPRITE_DA1n.qn_new2());
    /* p33.LASE*/ wire2 _LASE_SPR_PIX_DA2n = not1b(sprite_pix_a.MOFO_SPRITE_DA2n.qn_new2());
    /* p33.LUBO*/ wire2 _LUBO_SPR_PIX_DA3n = not1b(sprite_pix_a.PUDU_SPRITE_DA3n.qn_new2());
    /* p33.WERY*/ wire2 _WERY_SPR_PIX_DA4n = not1b(sprite_pix_a.SAJA_SPRITE_DA4n.qn_new2());
    /* p33.WURA*/ wire2 _WURA_SPR_PIX_DA5n = not1b(sprite_pix_a.SUNY_SPRITE_DA5n.qn_new2());
    /* p33.SULU*/ wire2 _SULU_SPR_PIX_DA6n = not1b(sprite_pix_a.SEMO_SPRITE_DA6n.qn_new2());
    /* p33.WAMY*/ wire2 _WAMY_SPR_PIX_DA7n = not1b(sprite_pix_a.SEGA_SPRITE_DA7n.qn_new2());

    /* p33.PYZU*/ wire2 _PYZU_SPR_PIX_RST0 = nand2b(_LESY_SPRITE_MASK0p, _RATA_SPR_PIX_DA0n);
    /* p33.MADA*/ wire2 _MADA_SPR_PIX_RST1 = nand2b(_LOTA_SPRITE_MASK1p, _NUCA_SPR_PIX_DA1n);
    /* p33.LYDE*/ wire2 _LYDE_SPR_PIX_RST2 = nand2b(_LYKU_SPRITE_MASK2p, _LASE_SPR_PIX_DA2n);
    /* p33.LUFY*/ wire2 _LUFY_SPR_PIX_RST3 = nand2b(_ROBY_SPRITE_MASK3p, _LUBO_SPR_PIX_DA3n);
    /* p33.XATO*/ wire2 _XATO_SPR_PIX_RST4 = nand2b(_TYTA_SPRITE_MASK4p, _WERY_SPR_PIX_DA4n);
    /* p33.XEXU*/ wire2 _XEXU_SPR_PIX_RST5 = nand2b(_TYCO_SPRITE_MASK5p, _WURA_SPR_PIX_DA5n);
    /* p33.TUPE*/ wire2 _TUPE_SPR_PIX_RST6 = nand2b(_SOKA_SPRITE_MASK6p, _SULU_SPR_PIX_DA6n);
    /* p33.XYVE*/ wire2 _XYVE_SPR_PIX_RST7 = nand2b(_XOVU_SPRITE_MASK7p, _WAMY_SPR_PIX_DA7n);

    /* p33.WUFY*/ WUFY_SPR_PIPE_A7.dff22(SACU_CLKPIPE_evn, _VUNE_SPR_PIX_SET7, _XYVE_SPR_PIX_RST7, VAFO_SPR_PIPE_A6.qp_old2());
    /* p33.VAFO*/ VAFO_SPR_PIPE_A6.dff22(SACU_CLKPIPE_evn, _TUXA_SPR_PIX_SET6, _TUPE_SPR_PIX_RST6, WORA_SPR_PIPE_A5.qp_old2());
    /* p33.WORA*/ WORA_SPR_PIPE_A5.dff22(SACU_CLKPIPE_evn, _VABY_SPR_PIX_SET5, _XEXU_SPR_PIX_RST5, WYHO_SPR_PIPE_A4.qp_old2());
    /* p33.WYHO*/ WYHO_SPR_PIPE_A4.dff22(SACU_CLKPIPE_evn, _VEXU_SPR_PIX_SET4, _XATO_SPR_PIX_RST4, LESU_SPR_PIPE_A3.qp_old2());
    /* p33.LESU*/ LESU_SPR_PIPE_A3.dff22(SACU_CLKPIPE_evn, _MAME_SPR_PIX_SET3, _LUFY_SPR_PIX_RST3, LEFE_SPR_PIPE_A2.qp_old2());
    /* p33.LEFE*/ LEFE_SPR_PIPE_A2.dff22(SACU_CLKPIPE_evn, _LELA_SPR_PIX_SET2, _LYDE_SPR_PIX_RST2, MASO_SPR_PIPE_A1.qp_old2());
    /* p33.MASO*/ MASO_SPR_PIPE_A1.dff22(SACU_CLKPIPE_evn, _MYTO_SPR_PIX_SET1, _MADA_SPR_PIX_RST1, NURO_SPR_PIPE_A0.qp_old2());
    /* p33.NURO*/ NURO_SPR_PIPE_A0.dff22(SACU_CLKPIPE_evn, _PABE_SPR_PIX_SET0, _PYZU_SPR_PIX_RST0, GND);
  }

  // Sprite pipe B
  {
    /* p33.MEZU*/ wire2 _MEZU_SPR_PIX_SET0 = nand2b(_LESY_SPRITE_MASK0p, sprite_pix_b.PEFO_SPRITE_DB0n.qn_new2());
    /* p33.RUSY*/ wire2 _RUSY_SPR_PIX_SET1 = nand2b(_LOTA_SPRITE_MASK1p, sprite_pix_b.ROKA_SPRITE_DB1n.qn_new2());
    /* p33.MYXA*/ wire2 _MYXA_SPR_PIX_SET2 = nand2b(_LYKU_SPRITE_MASK2p, sprite_pix_b.MYTU_SPRITE_DB2n.qn_new2());
    /* p33.RANO*/ wire2 _RANO_SPR_PIX_SET3 = nand2b(_ROBY_SPRITE_MASK3p, sprite_pix_b.RAMU_SPRITE_DB3n.qn_new2());
    /* p33.TYGA*/ wire2 _TYGA_SPR_PIX_SET4 = nand2b(_TYTA_SPRITE_MASK4p, sprite_pix_b.SELE_SPRITE_DB4n.qn_new2());
    /* p33.VUME*/ wire2 _VUME_SPR_PIX_SET5 = nand2b(_TYCO_SPRITE_MASK5p, sprite_pix_b.SUTO_SPRITE_DB5n.qn_new2());
    /* p33.TAPO*/ wire2 _TAPO_SPR_PIX_SET6 = nand2b(_SOKA_SPRITE_MASK6p, sprite_pix_b.RAMA_SPRITE_DB6n.qn_new2());
    /* p33.TESO*/ wire2 _TESO_SPR_PIX_SET7 = nand2b(_XOVU_SPRITE_MASK7p, sprite_pix_b.RYDU_SPRITE_DB7n.qn_new2());

    /* p33.LOZA*/ wire2 _LOZA_SPR_PIX_DB0n = not1b(sprite_pix_b.PEFO_SPRITE_DB0n.qn_new2());
    /* p33.SYBO*/ wire2 _SYBO_SPR_PIX_DB1n = not1b(sprite_pix_b.ROKA_SPRITE_DB1n.qn_new2());
    /* p33.LUMO*/ wire2 _LUMO_SPR_PIX_DB2n = not1b(sprite_pix_b.MYTU_SPRITE_DB2n.qn_new2());
    /* p33.SOLO*/ wire2 _SOLO_SPR_PIX_DB3n = not1b(sprite_pix_b.RAMU_SPRITE_DB3n.qn_new2());
    /* p33.VOBY*/ wire2 _VOBY_SPR_PIX_DB4n = not1b(sprite_pix_b.SELE_SPRITE_DB4n.qn_new2());
    /* p33.WYCO*/ wire2 _WYCO_SPR_PIX_DB5n = not1b(sprite_pix_b.SUTO_SPRITE_DB5n.qn_new2());
    /* p33.SERY*/ wire2 _SERY_SPR_PIX_DB6n = not1b(sprite_pix_b.RAMA_SPRITE_DB6n.qn_new2());
    /* p33.SELU*/ wire2 _SELU_SPR_PIX_DB7n = not1b(sprite_pix_b.RYDU_SPRITE_DB7n.qn_new2());

    /* p33.MOFY*/ wire2 _MOFY_SPR_PIX_RST0 = nand2b(_LESY_SPRITE_MASK0p, _LOZA_SPR_PIX_DB0n);
    /* p33.RUCA*/ wire2 _RUCA_SPR_PIX_RST1 = nand2b(_LOTA_SPRITE_MASK1p, _SYBO_SPR_PIX_DB1n);
    /* p33.MAJO*/ wire2 _MAJO_SPR_PIX_RST2 = nand2b(_LYKU_SPRITE_MASK2p, _LUMO_SPR_PIX_DB2n);
    /* p33.REHU*/ wire2 _REHU_SPR_PIX_RST3 = nand2b(_ROBY_SPRITE_MASK3p, _SOLO_SPR_PIX_DB3n);
    /* p33.WAXO*/ wire2 _WAXO_SPR_PIX_RST4 = nand2b(_TYTA_SPRITE_MASK4p, _VOBY_SPR_PIX_DB4n);
    /* p33.XOLE*/ wire2 _XOLE_SPR_PIX_RST5 = nand2b(_TYCO_SPRITE_MASK5p, _WYCO_SPR_PIX_DB5n);
    /* p33.TABY*/ wire2 _TABY_SPR_PIX_RST6 = nand2b(_SOKA_SPRITE_MASK6p, _SERY_SPR_PIX_DB6n);
    /* p33.TULA*/ wire2 _TULA_SPR_PIX_RST7 = nand2b(_XOVU_SPRITE_MASK7p, _SELU_SPR_PIX_DB7n);

    /* p33.VUPY*/ VUPY_SPR_PIPE_B7.dff22(SACU_CLKPIPE_evn, _TESO_SPR_PIX_SET7, _TULA_SPR_PIX_RST7, VANU_SPR_PIPE_B6.qp_old2());
    /* p33.VANU*/ VANU_SPR_PIPE_B6.dff22(SACU_CLKPIPE_evn, _TAPO_SPR_PIX_SET6, _TABY_SPR_PIX_RST6, WEBA_SPR_PIPE_B5.qp_old2());
    /* p33.WEBA*/ WEBA_SPR_PIPE_B5.dff22(SACU_CLKPIPE_evn, _VUME_SPR_PIX_SET5, _XOLE_SPR_PIX_RST5, VARE_SPR_PIPE_B4.qp_old2());
    /* p33.VARE*/ VARE_SPR_PIPE_B4.dff22(SACU_CLKPIPE_evn, _TYGA_SPR_PIX_SET4, _WAXO_SPR_PIX_RST4, PYJO_SPR_PIPE_B3.qp_old2());
    /* p33.PYJO*/ PYJO_SPR_PIPE_B3.dff22(SACU_CLKPIPE_evn, _RANO_SPR_PIX_SET3, _REHU_SPR_PIX_RST3, NATY_SPR_PIPE_B2.qp_old2());
    /* p33.NATY*/ NATY_SPR_PIPE_B2.dff22(SACU_CLKPIPE_evn, _MYXA_SPR_PIX_SET2, _MAJO_SPR_PIX_RST2, PEFU_SPR_PIPE_B1.qp_old2());
    /* p33.PEFU*/ PEFU_SPR_PIPE_B1.dff22(SACU_CLKPIPE_evn, _RUSY_SPR_PIX_SET1, _RUCA_SPR_PIX_RST1, NYLU_SPR_PIPE_B0.qp_old2());
    /* p33.NYLU*/ NYLU_SPR_PIPE_B0.dff22(SACU_CLKPIPE_evn, _MEZU_SPR_PIX_SET0, _MOFY_SPR_PIX_RST0, GND);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock_mask_pipe(
  OamTempB& oam_temp_b,
  wire2 WUTY_SFETCH_DONE_TRIGp,
  wire2 SACU_CLKPIPE_evn)
{
  wire2 _VYPO_VCC = 1;

  /* p29.XEFY*/ wire2 _XEPY_SPRITE_DONEn  = not1b(WUTY_SFETCH_DONE_TRIGp);
  /* p34.MEFU*/ wire2 _MEFU_SPRITE_MASK0n = or3(_XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp_old2(), NURO_SPR_PIPE_A0.qp_old2());
  /* p34.MEVE*/ wire2 _MEVE_SPRITE_MASK1n = or3(_XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_B1.qp_old2(), MASO_SPR_PIPE_A1.qp_old2());
  /* p34.MYZO*/ wire2 _MYZO_SPRITE_MASK2n = or3(_XEPY_SPRITE_DONEn, NATY_SPR_PIPE_B2.qp_old2(), LEFE_SPR_PIPE_A2.qp_old2());
  /* p34.RUDA*/ wire2 _RUDA_SPRITE_MASK3n = or3(_XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_B3.qp_old2(), LESU_SPR_PIPE_A3.qp_old2());
  /* p34.VOTO*/ wire2 _VOTO_SPRITE_MASK4n = or3(_XEPY_SPRITE_DONEn, VARE_SPR_PIPE_B4.qp_old2(), WYHO_SPR_PIPE_A4.qp_old2());
  /* p34.VYSA*/ wire2 _VYSA_SPRITE_MASK5n = or3(_XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_B5.qp_old2(), WORA_SPR_PIPE_A5.qp_old2());
  /* p34.TORY*/ wire2 _TORY_SPRITE_MASK6n = or3(_XEPY_SPRITE_DONEn, VANU_SPR_PIPE_B6.qp_old2(), VAFO_SPR_PIPE_A6.qp_old2());
  /* p34.WOPE*/ wire2 _WOPE_SPRITE_MASK7n = or3(_XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_B7.qp_old2(), WUFY_SPR_PIPE_A7.qp_old2());

  /* p34.LESY*/ wire2 _LESY_SPRITE_MASK0p = not1b(_MEFU_SPRITE_MASK0n);
  /* p34.LOTA*/ wire2 _LOTA_SPRITE_MASK1p = not1b(_MEVE_SPRITE_MASK1n);
  /* p34.LYKU*/ wire2 _LYKU_SPRITE_MASK2p = not1b(_MYZO_SPRITE_MASK2n);
  /* p34.ROBY*/ wire2 _ROBY_SPRITE_MASK3p = not1b(_RUDA_SPRITE_MASK3n);
  /* p34.TYTA*/ wire2 _TYTA_SPRITE_MASK4p = not1b(_VOTO_SPRITE_MASK4n);
  /* p34.TYCO*/ wire2 _TYCO_SPRITE_MASK5p = not1b(_VYSA_SPRITE_MASK5n);
  /* p34.SOKA*/ wire2 _SOKA_SPRITE_MASK6p = not1b(_TORY_SPRITE_MASK6n);
  /* p34.XOVU*/ wire2 _XOVU_SPRITE_MASK7p = not1b(_WOPE_SPRITE_MASK7n);

  {
    /* p26.TEDE*/ wire2 _TEDE_MASK_PIPE_SET0 = nand2b(_LESY_SPRITE_MASK0p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XALA*/ wire2 _XALA_MASK_PIPE_SET1 = nand2b(_LOTA_SPRITE_MASK1p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.TYRA*/ wire2 _TYRA_MASK_PIPE_SET2 = nand2b(_LYKU_SPRITE_MASK2p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XYRU*/ wire2 _XYRU_MASK_PIPE_SET3 = nand2b(_ROBY_SPRITE_MASK3p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XUKU*/ wire2 _XUKU_MASK_PIPE_SET4 = nand2b(_TYTA_SPRITE_MASK4p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XELY*/ wire2 _XELY_MASK_PIPE_SET5 = nand2b(_TYCO_SPRITE_MASK5p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.TYKO*/ wire2 _TYKO_MASK_PIPE_SET6 = nand2b(_SOKA_SPRITE_MASK6p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.TUWU*/ wire2 _TUWU_MASK_PIPE_SET7 = nand2b(_XOVU_SPRITE_MASK7p, oam_temp_b.DEPO_OAM_DB7p.qp_new2());

    /* p26.XOGA*/ wire2 _XOGA_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XURA*/ wire2 _XURA_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.TAJO*/ wire2 _TAJO_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XENU*/ wire2 _XENU_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XYKE*/ wire2 _XYKE_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XABA*/ wire2 _XABA_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.TAFU*/ wire2 _TAFU_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());
    /* p26.XUHO*/ wire2 _XUHO_MASK_PIPE_DB7n = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_new2());

    /* p26.WOKA*/ wire2 _WOKA_MASK_PIPE_RST0 = nand2b(_LESY_SPRITE_MASK0p, _XOGA_MASK_PIPE_DB7n);
    /* p26.WEDE*/ wire2 _WEDE_MASK_PIPE_RST1 = nand2b(_LOTA_SPRITE_MASK1p, _XURA_MASK_PIPE_DB7n);
    /* p26.TUFO*/ wire2 _TUFO_MASK_PIPE_RST2 = nand2b(_LYKU_SPRITE_MASK2p, _TAJO_MASK_PIPE_DB7n);
    /* p26.WEVO*/ wire2 _WEVO_MASK_PIPE_RST3 = nand2b(_ROBY_SPRITE_MASK3p, _XENU_MASK_PIPE_DB7n);
    /* p26.WEDY*/ wire2 _WEDY_MASK_PIPE_RST4 = nand2b(_TYTA_SPRITE_MASK4p, _XYKE_MASK_PIPE_DB7n);
    /* p26.WUJA*/ wire2 _WUJA_MASK_PIPE_RST5 = nand2b(_TYCO_SPRITE_MASK5p, _XABA_MASK_PIPE_DB7n);
    /* p26.TENA*/ wire2 _TENA_MASK_PIPE_RST6 = nand2b(_SOKA_SPRITE_MASK6p, _TAFU_MASK_PIPE_DB7n);
    /* p26.WUBU*/ wire2 _WUBU_MASK_PIPE_RST7 = nand2b(_XOVU_SPRITE_MASK7p, _XUHO_MASK_PIPE_DB7n);

    /* p26.VAVA*/ VAVA_MASK_PIPE_7.dff22(SACU_CLKPIPE_evn, _TUWU_MASK_PIPE_SET7, _WUBU_MASK_PIPE_RST7, VUMO_MASK_PIPE_6.qp_old2());
    /* p26.VUMO*/ VUMO_MASK_PIPE_6.dff22(SACU_CLKPIPE_evn, _TYKO_MASK_PIPE_SET6, _TENA_MASK_PIPE_RST6, WODA_MASK_PIPE_5.qp_old2());
    /* p26.WODA*/ WODA_MASK_PIPE_5.dff22(SACU_CLKPIPE_evn, _XELY_MASK_PIPE_SET5, _WUJA_MASK_PIPE_RST5, XETE_MASK_PIPE_4.qp_old2());
    /* p26.XETE*/ XETE_MASK_PIPE_4.dff22(SACU_CLKPIPE_evn, _XUKU_MASK_PIPE_SET4, _WEDY_MASK_PIPE_RST4, WYFU_MASK_PIPE_3.qp_old2());
    /* p26.WYFU*/ WYFU_MASK_PIPE_3.dff22(SACU_CLKPIPE_evn, _XYRU_MASK_PIPE_SET3, _WEVO_MASK_PIPE_RST3, VOSA_MASK_PIPE_2.qp_old2());
    /* p26.VOSA*/ VOSA_MASK_PIPE_2.dff22(SACU_CLKPIPE_evn, _TYRA_MASK_PIPE_SET2, _TUFO_MASK_PIPE_RST2, WURU_MASK_PIPE_1.qp_old2());
    /* p26.WURU*/ WURU_MASK_PIPE_1.dff22(SACU_CLKPIPE_evn, _XALA_MASK_PIPE_SET1, _WEDE_MASK_PIPE_RST1, VEZO_MASK_PIPE_0.qp_old2());
    /* p26.VEZO*/ VEZO_MASK_PIPE_0.dff22(SACU_CLKPIPE_evn, _TEDE_MASK_PIPE_SET0, _WOKA_MASK_PIPE_RST0, _VYPO_VCC);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock_pal_pipe(
  OamTempB& oam_temp_b,
  wire2 WUTY_SFETCH_DONE_TRIGp,
  wire2 SACU_CLKPIPE_evn)
{
  wire2 GND = 0;

  /* p29.XEFY*/ wire2 _XEPY_SPRITE_DONEn  = not1b(WUTY_SFETCH_DONE_TRIGp);
  /* p34.MEFU*/ wire2 _MEFU_SPRITE_MASK0n = or3(_XEPY_SPRITE_DONEn, NYLU_SPR_PIPE_B0.qp_old2(), NURO_SPR_PIPE_A0.qp_old2());
  /* p34.MEVE*/ wire2 _MEVE_SPRITE_MASK1n = or3(_XEPY_SPRITE_DONEn, PEFU_SPR_PIPE_B1.qp_old2(), MASO_SPR_PIPE_A1.qp_old2());
  /* p34.MYZO*/ wire2 _MYZO_SPRITE_MASK2n = or3(_XEPY_SPRITE_DONEn, NATY_SPR_PIPE_B2.qp_old2(), LEFE_SPR_PIPE_A2.qp_old2());
  /* p34.RUDA*/ wire2 _RUDA_SPRITE_MASK3n = or3(_XEPY_SPRITE_DONEn, PYJO_SPR_PIPE_B3.qp_old2(), LESU_SPR_PIPE_A3.qp_old2());
  /* p34.VOTO*/ wire2 _VOTO_SPRITE_MASK4n = or3(_XEPY_SPRITE_DONEn, VARE_SPR_PIPE_B4.qp_old2(), WYHO_SPR_PIPE_A4.qp_old2());
  /* p34.VYSA*/ wire2 _VYSA_SPRITE_MASK5n = or3(_XEPY_SPRITE_DONEn, WEBA_SPR_PIPE_B5.qp_old2(), WORA_SPR_PIPE_A5.qp_old2());
  /* p34.TORY*/ wire2 _TORY_SPRITE_MASK6n = or3(_XEPY_SPRITE_DONEn, VANU_SPR_PIPE_B6.qp_old2(), VAFO_SPR_PIPE_A6.qp_old2());
  /* p34.WOPE*/ wire2 _WOPE_SPRITE_MASK7n = or3(_XEPY_SPRITE_DONEn, VUPY_SPR_PIPE_B7.qp_old2(), WUFY_SPR_PIPE_A7.qp_old2());

  /* p34.LESY*/ wire2 _LESY_SPRITE_MASK0p = not1b(_MEFU_SPRITE_MASK0n);
  /* p34.LOTA*/ wire2 _LOTA_SPRITE_MASK1p = not1b(_MEVE_SPRITE_MASK1n);
  /* p34.LYKU*/ wire2 _LYKU_SPRITE_MASK2p = not1b(_MYZO_SPRITE_MASK2n);
  /* p34.ROBY*/ wire2 _ROBY_SPRITE_MASK3p = not1b(_RUDA_SPRITE_MASK3n);
  /* p34.TYTA*/ wire2 _TYTA_SPRITE_MASK4p = not1b(_VOTO_SPRITE_MASK4n);
  /* p34.TYCO*/ wire2 _TYCO_SPRITE_MASK5p = not1b(_VYSA_SPRITE_MASK5n);
  /* p34.SOKA*/ wire2 _SOKA_SPRITE_MASK6p = not1b(_TORY_SPRITE_MASK6n);
  /* p34.XOVU*/ wire2 _XOVU_SPRITE_MASK7p = not1b(_WOPE_SPRITE_MASK7n);

  {
    /* p34.PUME*/ wire2 _PUME_PAL_PIPE_SET0 = nand2b(_LESY_SPRITE_MASK0p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.SORO*/ wire2 _SORO_PAL_PIPE_SET1 = nand2b(_LOTA_SPRITE_MASK1p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.PAMO*/ wire2 _PAMO_PAL_PIPE_SET2 = nand2b(_LYKU_SPRITE_MASK2p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.SUKY*/ wire2 _SUKY_PAL_PIPE_SET3 = nand2b(_ROBY_SPRITE_MASK3p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.RORA*/ wire2 _RORA_PAL_PIPE_SET4 = nand2b(_TYTA_SPRITE_MASK4p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.MENE*/ wire2 _MENE_PAL_PIPE_SET5 = nand2b(_TYCO_SPRITE_MASK5p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.LUKE*/ wire2 _LUKE_PAL_PIPE_SET6 = nand2b(_SOKA_SPRITE_MASK6p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.LAMY*/ wire2 _LAMY_PAL_PIPE_SET7 = nand2b(_XOVU_SPRITE_MASK7p, oam_temp_b.GOMO_OAM_DB4p.qp_new2());

    /* p34.SYPY*/ wire2 _SYPY_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.TOTU*/ wire2 _TOTU_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.NARO*/ wire2 _NARO_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.WEXY*/ wire2 _WEXY_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.RYZY*/ wire2 _RYZY_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.RYFE*/ wire2 _RYFE_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.LADY*/ wire2 _LADY_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());
    /* p34.LAFY*/ wire2 _LAFY_PAL_PIPE_DB4n = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_new2());

    /* p34.SUCO*/ wire2 _SUCO_PAL_PIPE_RST0 = nand2b(_LESY_SPRITE_MASK0p, _SYPY_PAL_PIPE_DB4n);
    /* p34.TAFA*/ wire2 _TAFA_PAL_PIPE_RST1 = nand2b(_LOTA_SPRITE_MASK1p, _TOTU_PAL_PIPE_DB4n);
    /* p34.PYZY*/ wire2 _PYZY_PAL_PIPE_RST2 = nand2b(_LYKU_SPRITE_MASK2p, _NARO_PAL_PIPE_DB4n);
    /* p34.TOWA*/ wire2 _TOWA_PAL_PIPE_RST3 = nand2b(_ROBY_SPRITE_MASK3p, _WEXY_PAL_PIPE_DB4n);
    /* p34.RUDU*/ wire2 _RUDU_PAL_PIPE_RST4 = nand2b(_TYTA_SPRITE_MASK4p, _RYZY_PAL_PIPE_DB4n);
    /* p34.PAZO*/ wire2 _PAZO_PAL_PIPE_RST5 = nand2b(_TYCO_SPRITE_MASK5p, _RYFE_PAL_PIPE_DB4n);
    /* p34.LOWA*/ wire2 _LOWA_PAL_PIPE_RST6 = nand2b(_SOKA_SPRITE_MASK6p, _LADY_PAL_PIPE_DB4n);
    /* p34.LUNU*/ wire2 _LUNU_PAL_PIPE_RST7 = nand2b(_XOVU_SPRITE_MASK7p, _LAFY_PAL_PIPE_DB4n);

    /* p34.LYME*/ LYME_PAL_PIPE_D7.dff22(SACU_CLKPIPE_evn, _LAMY_PAL_PIPE_SET7, _LUNU_PAL_PIPE_RST7, MODA_PAL_PIPE_D6.qp_old2());
    /* p34.MODA*/ MODA_PAL_PIPE_D6.dff22(SACU_CLKPIPE_evn, _LUKE_PAL_PIPE_SET6, _LOWA_PAL_PIPE_RST6, NUKE_PAL_PIPE_D5.qp_old2());
    /* p34.NUKE*/ NUKE_PAL_PIPE_D5.dff22(SACU_CLKPIPE_evn, _MENE_PAL_PIPE_SET5, _PAZO_PAL_PIPE_RST5, PALU_PAL_PIPE_D4.qp_old2());
    /* p34.PALU*/ PALU_PAL_PIPE_D4.dff22(SACU_CLKPIPE_evn, _RORA_PAL_PIPE_SET4, _RUDU_PAL_PIPE_RST4, SOMY_PAL_PIPE_D3.qp_old2());
    /* p34.SOMY*/ SOMY_PAL_PIPE_D3.dff22(SACU_CLKPIPE_evn, _SUKY_PAL_PIPE_SET3, _TOWA_PAL_PIPE_RST3, ROSA_PAL_PIPE_D2.qp_old2());
    /* p34.ROSA*/ ROSA_PAL_PIPE_D2.dff22(SACU_CLKPIPE_evn, _PAMO_PAL_PIPE_SET2, _PYZY_PAL_PIPE_RST2, SATA_PAL_PIPE_D1.qp_old2());
    /* p34.SATA*/ SATA_PAL_PIPE_D1.dff22(SACU_CLKPIPE_evn, _SORO_PAL_PIPE_SET1, _TAFA_PAL_PIPE_RST1, RUGO_PAL_PIPE_D0.qp_old2());
    /* p34.RUGO*/ RUGO_PAL_PIPE_D0.dff22(SACU_CLKPIPE_evn, _PUME_PAL_PIPE_SET0, _SUCO_PAL_PIPE_RST0, GND);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void PixelPipes::tock_pix_output(
  RegLCDC& reg_lcdc,
  RegBGP  _reg_bgp,
  RegOBP0 _reg_obp0,
  RegOBP1 _reg_obp1)
{
  /*#p35.RAJY*/ wire2 RAJY_PIX_BG_LOp = and2(PYBO_BGW_PIPE_A7.qp_new2(), reg_lcdc.VYXE_LCDC_BGENn.qn_new2());
  /*#p35.TADE*/ wire2 TADE_PIX_BG_HIp = and2(SOHU_BGW_PIPE_B7.qp_new2(), reg_lcdc.VYXE_LCDC_BGENn.qn_new2());
  /*#p35.XULA*/ wire2 XULA_PIX_SP_LOp = and2(reg_lcdc.XYLO_LCDC_SPENn.qn_new2(), WUFY_SPR_PIPE_A7.qp_new2());
  /*#p35.WOXA*/ wire2 WOXA_PIX_SP_HIp = and2(reg_lcdc.XYLO_LCDC_SPENn.qn_new2(), VUPY_SPR_PIPE_B7.qp_new2());

  /*#p35.NULY*/ wire2 NULY_PIX_SP_MASKn = nor2b(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

  /*#p35.RYFU*/ wire2 RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, VAVA_MASK_PIPE_7.qp_new2());
  /*#p35.RUTA*/ wire2 RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, VAVA_MASK_PIPE_7.qp_new2());
  /*#p35.POKA*/ wire2 POKA_BGPIXELn = nor3b(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

  /*#p34.LOME*/ wire2 LOME_PAL_PIPE_7n = not1b(LYME_PAL_PIPE_D7.qp_new2());
  /*#p34.LAFU*/ wire2 LAFU_OBP0PIXELn = nand2b(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
  /*#p34.LEKA*/ wire2 LEKA_OBP1PIXELn = nand2b(LYME_PAL_PIPE_D7.qp_new2(), POKA_BGPIXELn);

  //----------------------------------------
  // Sprite palette 0 lookup

  /*#p35.WELE*/ wire2 WELE_PIX_SP_LOn = not1b(XULA_PIX_SP_LOp);
  /*#p35.WOLO*/ wire2 WOLO_PIX_SP_LOp = not1b(WELE_PIX_SP_LOn);
  /*#p35.VUMU*/ wire2 VUMU_PIX_SP_HIn = not1b(WOXA_PIX_SP_HIp);
  /*#p35.WYRU*/ wire2 WYRU_PIX_SP_HIp = not1b(VUMU_PIX_SP_HIn);

  /*#p35.LAVA*/ wire2 LAVA_MASK_OPB0  = not1b(LAFU_OBP0PIXELn);

  /*#p35.VUGO*/ wire2 VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VOLO*/ wire2 VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VATA*/ wire2 VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VYRO*/ wire2 VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

  /*#p35.WUFU*/ wire2 WUFU_COL_OBP0_HI = amux4(_reg_obp0.XANA_OBP0_D7n.qp_any2(), VYRO_PAL_OBP0D,
                                              _reg_obp0.XYZE_OBP0_D5n.qp_any2(), VATA_PAL_OBP0C,
                                              _reg_obp0.XALO_OBP0_D3n.qp_any2(), VOLO_PAL_OBP0B,
                                              _reg_obp0.XUKY_OBP0_D1n.qp_any2(), VUGO_PAL_OBP0A);

  /*#p35.WALY*/ wire2 WALY_COL_OBP0_LO = amux4(_reg_obp0.XUPO_OBP0_D6n.qp_any2(), VYRO_PAL_OBP0D,
                                              _reg_obp0.XERU_OBP0_D4n.qp_any2(), VATA_PAL_OBP0C,
                                              _reg_obp0.XOVA_OBP0_D2n.qp_any2(), VOLO_PAL_OBP0B,
                                              _reg_obp0.XUFU_OBP0_D0n.qp_any2(), VUGO_PAL_OBP0A);

  //----------------------------------------
  // Sprite palette 1 lookup

  /*#p35.MABY*/ wire2 MABY_PIX_SP_LOn = not1b(XULA_PIX_SP_LOp);
  /*#p35.LYLE*/ wire2 LYLE_PIX_SP_LOp = not1b(MABY_PIX_SP_LOn);
  /*#p35.MEXA*/ wire2 MEXA_PIX_SP_HIn = not1b(WOXA_PIX_SP_HIp);
  /*#p35.LOZO*/ wire2 LOZO_PIX_SP_HIp = not1b(MEXA_PIX_SP_HIn);

  /*#p35.LUKU*/ wire2 LUKU_MASK_OBP1  = not1b(LEKA_OBP1PIXELn);

  /* p#35.LOPU*/ wire2 LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
  /* p#35.LYKY*/ wire2 LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
  /* p#35.LARU*/ wire2 LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
  /* p#35.LEDO*/ wire2 LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

  /*#p35.MOKA*/ wire2 MOKA_COL_OBP1_HI = amux4(_reg_obp1.LUXO_OBP1_D7n.qp_any2(), LEDO_PAL_OBP1D,
                                              _reg_obp1.LUGU_OBP1_D5n.qp_any2(), LARU_PAL_OBP1C,
                                              _reg_obp1.LOSE_OBP1_D3n.qp_any2(), LYKY_PAL_OBP1B,
                                              _reg_obp1.LAWO_OBP1_D1n.qp_any2(), LOPU_PAL_OBP1A);

  /*#p35.MUFA*/ wire2 MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, _reg_obp1.LEPU_OBP1_D6n.qp_any2(),
                                              LARU_PAL_OBP1C, _reg_obp1.LUNE_OBP1_D4n.qp_any2(),
                                              LYKY_PAL_OBP1B, _reg_obp1.MOSA_OBP1_D2n.qp_any2(),
                                              LOPU_PAL_OBP1A, _reg_obp1.MOXY_OBP1_D0n.qp_any2());

  //----------------------------------------
  // Background/window palette lookup

  /* p35.SOBA*/ wire2 SOBA_PIX_BG_LOn = not1b(RAJY_PIX_BG_LOp);
  /* p35.NUPO*/ wire2 NUPO_PIX_BG_LOp = not1b(SOBA_PIX_BG_LOn);
  /* p35.VYCO*/ wire2 VYCO_PIX_BG_HIn = not1b(TADE_PIX_BG_HIp);
  /* p35.NALE*/ wire2 NALE_PIX_BG_HIp = not1b(VYCO_PIX_BG_HIn);

  /* p35.MUVE*/ wire2 MUVE_MASK_BGP = not1b(POKA_BGPIXELn);

  /* p35.POBU*/ wire2 POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
  /* p35.NUXO*/ wire2 NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
  /* p35.NUMA*/ wire2 NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
  /* p35.NYPO*/ wire2 NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

  /*#p35.NELO*/ wire2 NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, _reg_bgp.MOGY_BGP_D6n.qp_any2(),
                                            NUMA_PAL_BGPC, _reg_bgp.MUKE_BGP_D4n.qp_any2(),
                                            NUXO_PAL_BGPB, _reg_bgp.PYLU_BGP_D2n.qp_any2(),
                                            POBU_PAL_BGPA, _reg_bgp.PAVO_BGP_D0n.qp_any2());

  /*#p35.NURA*/ wire2 NURA_COL_BG_HI = amux4(_reg_bgp.MENA_BGP_D7n.qp_any2(), NYPO_PAL_BGPD,
                                            _reg_bgp.MORU_BGP_D5n.qp_any2(), NUMA_PAL_BGPC,
                                            _reg_bgp.MAXY_BGP_D3n.qp_any2(), NUXO_PAL_BGPB,
                                            _reg_bgp.NUSY_BGP_D1n.qp_any2(), POBU_PAL_BGPA);

  //----------------------------------------
  // Pixel merge and send

  /*#p35.PERO*/ wire2 _PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
  /*#p35.PATY*/ wire2 _PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

  /*#p35.REMY*/ wire2 _REMY_LD0n = not1b(_PERO_COL_LO);
  /*#p35.RAVO*/ wire2 _RAVO_LD1n = not1b(_PATY_COL_HI);

  /*#p35.REMY*/ REMY_LD0n.set(_REMY_LD0n);
  /*#p35.RAVO*/ RAVO_LD1n.set(_RAVO_LD1n);
}

//------------------------------------------------------------------------------------------------------------------------




#if 0
  [
    this,

    XODO_VID_RSTp,
    &NUKO_WX_MATCHp_old_evn
  ](){

    /*#p27.MYLO*/ wire2 _MYLO_WX_MATCH0p_old_evn = xnor2(pix_count.XEHO_PX0p.qp_old2(), reg_wx.MYPA_WX0n_h.qn_old());
    /* p27.PUWU*/ wire2 _PUWU_WX_MATCH1p_old_evn = xnor2(pix_count.SAVY_PX1p.qp_old2(), reg_wx.NOFE_WX1n_h.qn_old());
    /* p27.PUHO*/ wire2 _PUHO_WX_MATCH2p_old_evn = xnor2(pix_count.XODU_PX2p.qp_old2(), reg_wx.NOKE_WX2n_h.qn_old());
    /* p27.NYTU*/ wire2 _NYTU_WX_MATCH3p_old_evn = xnor2(pix_count.XYDO_PX3p.qp_old2(), reg_wx.MEBY_WX3n_h.qn_old());
    /* p27.NEZO*/ wire2 _NEZO_WX_MATCH4p_old_evn = xnor2(pix_count.TUHU_PX4p.qp_old2(), reg_wx.MYPU_WX4n_h.qn_old());
    /* p27.NORY*/ wire2 _NORY_WX_MATCH5p_old_evn = xnor2(pix_count.TUKY_PX5p.qp_old2(), reg_wx.MYCE_WX5n_h.qn_old());
    /* p27.NONO*/ wire2 _NONO_WX_MATCH6p_old_evn = xnor2(pix_count.TAKO_PX6p.qp_old2(), reg_wx.MUVO_WX6n_h.qn_old());
    /* p27.PASE*/ wire2 _PASE_WX_MATCH7p_old_evn = xnor2(pix_count.SYBE_PX7p.qp_old2(), reg_wx.NUKU_WX7n_h.qn_old());

    /* p27.PUKY*/ wire2 _PUKY_WX_MATCHn_old_evn  = nand5(win_reg.REJO_WY_MATCH_LATCHp_evn.qp_old2(), _NEZO_WX_MATCH4p_old_evn, _NORY_WX_MATCH5p_old_evn, _NONO_WX_MATCH6p_old_evn, _PASE_WX_MATCH7p_old_evn);
    /* p27.NUFA*/ wire2 _NUFA_WX_MATCHp_old_evn  = not1(_PUKY_WX_MATCHn_old_evn);
    /* p27.NOGY*/ wire2 _NOGY_WX_MATCHn_old_evn  = nand5(_NUFA_WX_MATCHp_old_evn, _MYLO_WX_MATCH0p_old_evn, _PUWU_WX_MATCH1p_old_evn, _PUHO_WX_MATCH2p_old_evn, _NYTU_WX_MATCH3p_old_evn);
    /* p27.NUKO*/ NUKO_WX_MATCHp_old_evn  = not1(_NOGY_WX_MATCHn_old_evn);

    /*#p27.NAZE*/ wire2 _NAZE_WY_MATCH0p_old_evn = xnor2(reg_wy.NESO_WY0n_h.qn_old(), reg_ly.MUWY_LY0p_evn.qp_old2());
    /* p27.PEBO*/ wire2 _PEBO_WY_MATCH1p_old_evn = xnor2(reg_wy.NYRO_WY1n_h.qn_old(), reg_ly.MYRO_LY1p_evn.qp_old2());
    /* p27.POMO*/ wire2 _POMO_WY_MATCH2p_old_evn = xnor2(reg_wy.NAGA_WY2n_h.qn_old(), reg_ly.LEXA_LY2p_evn.qp_old2());
    /* p27.NEVU*/ wire2 _NEVU_WY_MATCH3p_old_evn = xnor2(reg_wy.MELA_WY3n_h.qn_old(), reg_ly.LYDO_LY3p_evn.qp_old2());
    /* p27.NOJO*/ wire2 _NOJO_WY_MATCH4p_old_evn = xnor2(reg_wy.NULO_WY4n_h.qn_old(), reg_ly.LOVU_LY4p_evn.qp_old2());
    /* p27.PAGA*/ wire2 _PAGA_WY_MATCH5p_old_evn = xnor2(reg_wy.NENE_WY5n_h.qn_old(), reg_ly.LEMA_LY5p_evn.qp_old2());
    /* p27.PEZO*/ wire2 _PEZO_WY_MATCH6p_old_evn = xnor2(reg_wy.NUKA_WY6n_h.qn_old(), reg_ly.MATO_LY6p_evn.qp_old2());
    /* p27.NUPA*/ wire2 _NUPA_WY_MATCH7p_old_evn = xnor2(reg_wy.NAFU_WY7n_h.qn_old(), reg_ly.LAFO_LY7p_evn.qp_old2());

    /*#p27.PALO*/ wire2 _PALO_WY_MATCH_HIn_old_evn = nand5(reg_lcdc.WYMO_LCDC_WINENn_h.qn_old(), _NOJO_WY_MATCH4p_old_evn, _PAGA_WY_MATCH5p_old_evn, _PEZO_WY_MATCH6p_old_evn, _NUPA_WY_MATCH7p_old_evn);
    /* p27.NELE*/ wire2 _NELE_WY_MATCH_HIp_old_evn = not1(_PALO_WY_MATCH_HIn_old_evn);
    /* p27.PAFU*/ wire2 _PAFU_WY_MATCHn_old_evn    = nand5(_NELE_WY_MATCH_HIp_old_evn, _NAZE_WY_MATCH0p_old_evn, _PEBO_WY_MATCH1p_old_evn, _POMO_WY_MATCH2p_old_evn, _NEVU_WY_MATCH3p_old_evn);
    /* p27.ROGE*/ wire2 _ROGE_WY_MATCHp_old_evn    = not1(_PAFU_WY_MATCHn_old_evn);

    {
      /* p01.XAPO*/ wire2 _XAPO_VID_RSTn = not1(XODO_VID_RSTp);
      /* p01.PYRY*/ wire2 _PYRY_VID_RSTp = not1(_XAPO_VID_RSTn);
      /* p27.SARY*/ win_reg.SARY_WY_MATCHp_evn.dff17(vclk.TALU_xxCDEFxx(), _XAPO_VID_RSTn, _ROGE_WY_MATCHp_old_evn);
      /* p27.REPU*/ wire2 _REPU_VBLANKp = or2(lcd.PARU_VBLANKp(), _PYRY_VID_RSTp);
      /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp_evn.nor_latch(win_reg.SARY_WY_MATCHp_evn.qp_new2(), _REPU_VBLANKp);
    }
  }();
#endif