#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyPixPipe.h"

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY SpriteDeltaY::sub(const OamTempA& oam_temp_a, const RegLY& reg_ly) {
  wire2 GND = 0;

  /*#p29.EBOS*/ wire2 EBOS_LY0n_new_evn = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire2 DASA_LY1n_new_evn = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire2 FUKY_LY2n_new_evn = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire2 FUVE_LY3n_new_evn = not1(reg_ly.LYDO_LY3p.qp_new());
  /* p29.FEPU*/ wire2 FEPU_LY4n_new_evn = not1(reg_ly.LOVU_LY4p.qp_new());
  /* p29.FOFA*/ wire2 FOFA_LY5n_new_evn = not1(reg_ly.LEMA_LY5p.qp_new());
  /* p29.FEMO*/ wire2 FEMO_LY6n_new_evn = not1(reg_ly.MATO_LY6p.qp_new());
  /* p29.GUSU*/ wire2 GUSU_LY7n_new_evn = not1(reg_ly.LAFO_LY7p.qp_new());

  /* p29.ERUC*/ wire2 _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ERUC*/ wire2 _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ENEF*/ wire2 _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.ENEF*/ wire2 _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.FECO*/ wire2 _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.FECO*/ wire2 _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.GYKY*/ wire2 _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GYKY*/ wire2 _GYKY_YDIFF_C3_new = add_c(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GOPU*/ wire2 _GOPU_YDIFF_S4_new = add_s(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.GOPU*/ wire2 _GOPU_YDIFF_C4_new = add_c(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.FUWA*/ wire2 _FUWA_YDIFF_S5_new = add_s(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.FUWA*/ wire2 _FUWA_YDIFF_C5_new = add_c(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.GOJU*/ wire2 _GOJU_YDIFF_S6_new = add_s(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.GOJU*/ wire2 _GOJU_YDIFF_C6_new = add_c(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.WUHU*/ wire2 _WUHU_YDIFF_S7_new = add_s(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);
  /* p29.WUHU*/ wire2 _WUHU_YDIFF_C7_new = add_c(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);

  /* p29.DEGE*/ wire2 _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
  /* p29.DABY*/ wire2 _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
  /* p29.DABU*/ wire2 _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
  /* p29.GYSA*/ wire2 _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);
  /* p29.GACE*/ wire2 _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF_S4_new);
  /* p29.GUVU*/ wire2 _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF_S5_new);
  /* p29.GYDA*/ wire2 _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF_S6_new);
  /* p29.GEWY*/ wire2 _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF_S7_new);

  return {
    _DEGE_SPRITE_DELTA0_new,
    _DABY_SPRITE_DELTA1_new,
    _DABU_SPRITE_DELTA2_new,
    _GYSA_SPRITE_DELTA3_new,
    _GACE_SPRITE_DELTA4_new,
    _GUVU_SPRITE_DELTA5_new,
    _GYDA_SPRITE_DELTA6_new,
    _GEWY_SPRITE_DELTA7_new,
    _GYKY_YDIFF_S3_new,
    _WUHU_YDIFF_C7_new
  };
}

wire2 SpriteDeltaY::GESE_SCAN_MATCH_Yp(wire2 XYMO_LCDC_SPSIZEn) {
  /*#p29.GOVU*/ wire2 _GOVU_SPSIZE_MATCH_new = or2(XYMO_LCDC_SPSIZEn, GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire2 _WOTA_SCAN_MATCH_Yn_new = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH_new);
  /* p29.GESE*/ wire2 _GESE_SCAN_MATCH_Yp_new = not1(_WOTA_SCAN_MATCH_Yn_new);
  return _GESE_SCAN_MATCH_Yp_new;
}

//-----------------------------------------------------------------------------
// Pixel counter

void PixCount::reset_to_cart() {
  XEHO_PX0p.reset(1, 1);
  SAVY_PX1p.reset(1, 1);
  XODU_PX2p.reset(1, 1);
  XYDO_PX3p.reset(1, 0);
  TUHU_PX4p.reset(1, 0);
  TUKY_PX5p.reset(1, 1);
  TAKO_PX6p.reset(1, 0);
  SYBE_PX7p.reset(1, 1);
}

void PixCount::tock(wire2 TADY_LINE_RSTn, wire2 SACU_CLKPIPE_evn) {
  // Pixel counter, has carry lookahead because this can increment every tcycle
  /* p21.RYBO*/ wire2 RYBO_old = xor2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
  /* p21.XUKE*/ wire2 XUKE_old = and2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old());
  /* p21.XYLE*/ wire2 XYLE_old = and2(XODU_PX2p.qp_old(), XUKE_old);
  /* p21.XEGY*/ wire2 XEGY_old = xor2(XODU_PX2p.qp_old(), XUKE_old); // feet facing gnd
  /* p21.XORA*/ wire2 XORA_old = xor2(XYDO_PX3p.qp_old(), XYLE_old); // feet facing gnd

  /* p21.XEHO*/ XEHO_PX0p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEHO_PX0p.qn_old());
  /* p21.SAVY*/ SAVY_PX1p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
  /* p21.XODU*/ XODU_PX2p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
  /* p21.XYDO*/ XYDO_PX3p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

  /* p24.TOCA*/ wire2 TOCA_new = not1(XYDO_PX3p.qp_new());
  /* p21.SAKE*/ wire2 SAKE_old = xor2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
  /* p21.TYBA*/ wire2 TYBA_old = and2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
  /* p21.SURY*/ wire2 SURY_old = and2(TAKO_PX6p.qp_old(), TYBA_old);
  /* p21.TYGE*/ wire2 TYGE_old = xor2(TAKO_PX6p.qp_old(), TYBA_old);
  /* p21.ROKU*/ wire2 ROKU_old = xor2(SYBE_PX7p.qp_old(), SURY_old);

  /* p21.TUHU*/ TUHU_PX4p.dff17(TOCA_new, TADY_LINE_RSTn, TUHU_PX4p.qn_old());
  /* p21.TUKY*/ TUKY_PX5p.dff17(TOCA_new, TADY_LINE_RSTn, SAKE_old);
  /* p21.TAKO*/ TAKO_PX6p.dff17(TOCA_new, TADY_LINE_RSTn, TYGE_old);
  /* p21.SYBE*/ SYBE_PX7p.dff17(TOCA_new, TADY_LINE_RSTn, ROKU_old);
}

wire2 PixCount::XANO_PX167p_old() const {
  /*#p21.XUGU*/ wire2 _XUGU_PX167n_old = nand5(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old(), XODU_PX2p.qp_old(), TUKY_PX5p.qp_old(), SYBE_PX7p.qp_old()); // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XANO*/ wire2 _XANO_PX167p_old = not1(_XUGU_PX167n_old);
  return _XANO_PX167p_old;
}

wire2 PixCount::XANO_PX167p_new() const {
  /*#p21.XUGU*/ wire2 _XUGU_PX167n = nand5(XEHO_PX0p.qp_new(), SAVY_PX1p.qp_new(), XODU_PX2p.qp_new(), TUKY_PX5p.qp_new(), SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XANO*/ wire2 _XANO_PX167p = not1(_XUGU_PX167n);
  return _XANO_PX167p;
}

//------------------------------------------------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4189)

BGScrollX BGScrollX::add(PixCount& pix_count, RegSCX reg_scx) {
  /*#p26.ATAD*/ wire2 _ATAD_TILE_X0S = add_s(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), 0);
  /*#p26.ATAD*/ wire2 _ATAD_TILE_X0C = add_c(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), 0);
  /* p26.BEHU*/ wire2 _BEHU_TILE_X1S = add_s(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C);
  /* p26.BEHU*/ wire2 _BEHU_TILE_X1C = add_c(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C);
  /* p26.APYH*/ wire2 _APYH_TILE_X2S = add_s(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C);
  /* p26.APYH*/ wire2 _APYH_TILE_X2C = add_c(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C);
  /* p26.BABE*/ wire2 _BABE_MAP_X0S  = add_s(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C);
  /* p26.BABE*/ wire2 _BABE_MAP_X0C  = add_c(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C);
  /* p26.ABOD*/ wire2 _ABOD_MAP_X1S  = add_s(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C);
  /* p26.ABOD*/ wire2 _ABOD_MAP_X1C  = add_c(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C);
  /* p26.BEWY*/ wire2 _BEWY_MAP_X2S  = add_s(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C);
  /* p26.BEWY*/ wire2 _BEWY_MAP_X2C  = add_c(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C);
  /* p26.BYCA*/ wire2 _BYCA_MAP_X3S  = add_s(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C);
  /* p26.BYCA*/ wire2 _BYCA_MAP_X3C  = add_c(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C);
  /* p26.ACUL*/ wire2 _ACUL_MAP_X4S  = add_s(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C);
  /* p26.ACUL*/ wire2 _ACUL_MAP_X4C  = add_c(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C);

  return {
    _ATAD_TILE_X0S,
    _BEHU_TILE_X1S,
    _APYH_TILE_X2S,
    _BABE_MAP_X0S,
    _ABOD_MAP_X1S,
    _BEWY_MAP_X2S,
    _BYCA_MAP_X3S,
    _ACUL_MAP_X4S
  };
}

BGScrollY BGScrollY::add(RegLY& reg_ly, RegSCY& reg_scy) {
  /*#p26.FAFO*/ wire2 _FAFO_TILE_Y0S = add_s(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), 0);
  /*#p26.FAFO*/ wire2 _FAFO_TILE_Y0C = add_c(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), 0);
  /* p26.EMUX*/ wire2 _EMUX_TILE_Y1S = add_s(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C);
  /* p26.EMUX*/ wire2 _EMUX_TILE_Y1C = add_c(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C);
  /* p26.ECAB*/ wire2 _ECAB_TILE_Y2S = add_s(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ECAB*/ wire2 _ECAB_TILE_Y2C = add_c(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ETAM*/ wire2 _ETAM_MAP_Y0S  = add_s(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C);
  /* p26.ETAM*/ wire2 _ETAM_MAP_Y0C  = add_c(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C);
  /* p26.DOTO*/ wire2 _DOTO_MAP_Y1S  = add_s(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DOTO*/ wire2 _DOTO_MAP_Y1C  = add_c(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DABA*/ wire2 _DABA_MAP_Y2S  = add_s(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C);
  /* p26.DABA*/ wire2 _DABA_MAP_Y2C  = add_c(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C);
  /* p26.EFYK*/ wire2 _EFYK_MAP_Y3S  = add_s(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C);
  /* p26.EFYK*/ wire2 _EFYK_MAP_Y3C  = add_c(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C);
  /* p26.EJOK*/ wire2 _EJOK_MAP_Y4S  = add_s(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C);
  /* p26.EJOK*/ wire2 _EJOK_MAP_Y4C  = add_c(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C);

  return {
    _FAFO_TILE_Y0S,
    _EMUX_TILE_Y1S,
    _ECAB_TILE_Y2S,
    _ETAM_MAP_Y0S,
    _DOTO_MAP_Y1S,
    _DABA_MAP_Y2S,
    _EFYK_MAP_Y3S,
    _EJOK_MAP_Y4S
  };
}

#pragma warning(pop)

//------------------------------------------------------------------------------------------------------------------------

SpritePix SpritePix::flip(Bus BUS_VRAM_Dp[8], wire2 TEXY_SFETCHINGp, wire2 BAXO_OAM_DB5p) {
  /*#p29.XONO*/ wire2 _XONO_FLIP_X_old = and2(BAXO_OAM_DB5p, TEXY_SFETCHINGp);
  /* p33.PUTE*/ wire2 _PUTE_FLIP0p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[7].qp_old(), BUS_VRAM_Dp[0].qp_old());
  /* p33.PELO*/ wire2 _PELO_FLIP1p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[6].qp_old(), BUS_VRAM_Dp[1].qp_old());
  /* p33.PONO*/ wire2 _PONO_FLIP2p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[5].qp_old(), BUS_VRAM_Dp[2].qp_old());
  /* p33.POBE*/ wire2 _POBE_FLIP3p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[4].qp_old(), BUS_VRAM_Dp[3].qp_old());
  /* p33.PACY*/ wire2 _PACY_FLIP4p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[3].qp_old(), BUS_VRAM_Dp[4].qp_old());
  /* p33.PUGU*/ wire2 _PUGU_FLIP5p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[2].qp_old(), BUS_VRAM_Dp[5].qp_old());
  /* p33.PAWE*/ wire2 _PAWE_FLIP6p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[1].qp_old(), BUS_VRAM_Dp[6].qp_old());
  /* p33.PULY*/ wire2 _PULY_FLIP7p = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[0].qp_old(), BUS_VRAM_Dp[7].qp_old());

  return {
    _PUTE_FLIP0p,
    _PELO_FLIP1p,
    _PONO_FLIP2p,
    _POBE_FLIP3p,
    _PACY_FLIP4p,
    _PUGU_FLIP5p,
    _PAWE_FLIP6p,
    _PULY_FLIP7p
  };
}

//------------------------------------------------------------------------------------------------------------------------

void RegLCDC::read(GateBoyCpuBus& cpu_bus)
{
  /* p23.VYRE*/ wire2 _VYRE_FF40_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VOCA_FF40p());
  /* p23.WYCE*/ wire2 _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);
  /*#p23.WYPO*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_WYCE_FF40_RDn, VYXE_LCDC_BGENn.qp_new());
  /*#p23.XERO*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_WYCE_FF40_RDn, XYLO_LCDC_SPENn.qp_new());
  /* p23.WYJU*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZEn.qp_new());
  /* p23.WUKA*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_WYCE_FF40_RDn, XAFO_LCDC_BGMAPn.qp_new());
  /* p23.VOKE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_WYCE_FF40_RDn, WEXU_LCDC_BGTILEn.qp_new());
  /* p23.VATO*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_WYCE_FF40_RDn, WYMO_LCDC_WINENn.qp_new());
  /*#p23.VAHA*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_WYCE_FF40_RDn, WOKY_LCDC_WINMAPn.qp_new());
  /*#p23.VAHA*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_WYCE_FF40_RDn, XONA_LCDC_LCDENn.qp_new());
}

void RegLCDC::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  /* p23.WARU*/ wire2 _WARU_FF40_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VOCA_FF40p());
  /* p23.XUBO*/ wire2 _XUBO_FF40_WRn = not1(_WARU_FF40_WRp);
  /*#p23.VYXE*/ VYXE_LCDC_BGENn  .dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.XYLO*/ XYLO_LCDC_SPENn  .dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.XYMO*/ XYMO_LCDC_SPSIZEn.dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.XAFO*/ XAFO_LCDC_BGMAPn .dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.WEXU*/ WEXU_LCDC_BGTILEn.dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.WYMO*/ WYMO_LCDC_WINENn .dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.WOKY*/ WOKY_LCDC_WINMAPn.dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.WOKY*/ XONA_LCDC_LCDENn. dff9(_XUBO_FF40_WRn, rst.XARE_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegStat::read(
  GateBoyCpuBus& cpu_bus,
  wire2 ACYL_SCANNINGp,
  wire2 XYMU_RENDERINGp,
  wire2 PARU_VBLANKp)
{
  /*#p21.SADU*/ wire2 _SADU_STAT_MODE0n = nor2(XYMU_RENDERINGp, PARU_VBLANKp);   // die NOR
  /*#p21.XATY*/ wire2 _XATY_STAT_MODE1n = nor2(ACYL_SCANNINGp, XYMU_RENDERINGp); // die NOR
  /* p21.TOBE*/ wire2 _TOBE_FF41_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VARY_FF41p());
  /* p21.VAVE*/ wire2 _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp);
  /*#p21.TEBY*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TOBE_FF41_RDp, _SADU_STAT_MODE0n);
  /*#p21.WUGA*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_TOBE_FF41_RDp, _XATY_STAT_MODE1n);
  /*#p21.SEGO*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_TOBE_FF41_RDp, RUPO_LYC_MATCHn.qp_new());
  /* p21.PUZO*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_VAVE_FF41_RDn, ROXE_STAT_HBI_ENn.qp_new());
  /* p21.POFO*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_VAVE_FF41_RDn, RUFO_STAT_VBI_ENn.qp_new());
  /* p21.SASY*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_VAVE_FF41_RDn, REFE_STAT_OAI_ENn.qp_new());
  /* p21.POTE*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_VAVE_FF41_RDn, RUGU_STAT_LYI_ENn.qp_new());
}

void RegStat::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  // FF41 STAT
  /* p21.SEPA*/ wire2 _SEPA_FF41_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VARY_FF41p());
  /* p21.RYVE*/ wire2 _RYVE_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.ROXE*/ ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p21.RUFO*/ RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p21.REFE*/ REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p21.RUGU*/ RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
}

void RegStat::tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, const RegLYC& reg_lyc)
{
  // FIXME polarity doesn't seem right, are we sure that RUPO is a nor latch and these signals are correct?
  /* p21.SEPA*/ wire2 _SEPA_FF41_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VARY_FF41p());
  /* p21.RYJU*/ wire2 _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.PAGO*/ wire2 _PAGO_LYC_MATCH_RST = or2(rst.WESY_SYS_RSTn(), _RYJU_FF41_WRn);
  /* p21.RUPO*/ RUPO_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST, reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegSCY::read(GateBoyCpuBus& cpu_bus) {
  /* p23.ANYP*/ wire2 _ANYP_FF42_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XARO_FF42p());
  /* p23.BUWY*/ wire2 _BUWY_FF42_RDn = not1(_ANYP_FF42_RDp);
  /*#p23.WARE*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_BUWY_FF42_RDn, GAVE_SCY0n.qp_new());
  /* p23.GOBA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_BUWY_FF42_RDn, FYMO_SCY1n.qp_new());
  /* p23.GONU*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_BUWY_FF42_RDn, FEZU_SCY2n.qp_new());
  /* p23.GODO*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_BUWY_FF42_RDn, FUJO_SCY3n.qp_new());
  /* p23.CUSA*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_BUWY_FF42_RDn, DEDE_SCY4n.qp_new());
  /* p23.GYZO*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_BUWY_FF42_RDn, FOTY_SCY5n.qp_new());
  /* p23.GUNE*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_BUWY_FF42_RDn, FOHA_SCY6n.qp_new());
  /* p23.GYZA*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_BUWY_FF42_RDn, FUNY_SCY7n.qp_new());
}

void RegSCY::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p23.BEDY*/ wire2 _BEDY_FF42_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.XARO_FF42p());
  /* p23.CAVO*/ wire2 _CAVO_FF42_WRn = not1(_BEDY_FF42_WRp);
  /* p23.GAVE*/ GAVE_SCY0n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.FYMO*/ FYMO_SCY1n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.FEZU*/ FEZU_SCY2n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.FUJO*/ FUJO_SCY3n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.DEDE*/ DEDE_SCY4n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.FOTY*/ FOTY_SCY5n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.FOHA*/ FOHA_SCY6n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.FUNY*/ FUNY_SCY7n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegSCX::read(GateBoyCpuBus& cpu_bus) {
  /* p23.AVOG*/ wire2 _AVOG_FF43_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XAVY_FF43p());
  /* p23.BEBA*/ wire2 _BEBA_FF43_RDn = not1(_AVOG_FF43_RDp);
  /*#p23.EDOS*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_BEBA_FF43_RDn, DATY_SCX0n.qp_new());
  /* p23.EKOB*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_BEBA_FF43_RDn, DUZU_SCX1n.qp_new());
  /* p23.CUGA*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_BEBA_FF43_RDn, CYXU_SCX2n.qp_new());
  /* p23.WONY*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_BEBA_FF43_RDn, GUBO_SCX3n.qp_new());
  /* p23.CEDU*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_BEBA_FF43_RDn, BEMY_SCX4n.qp_new());
  /* p23.CATA*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_BEBA_FF43_RDn, CUZY_SCX5n.qp_new());
  /* p23.DOXE*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_BEBA_FF43_RDn, CABU_SCX6n.qp_new());
  /* p23.CASY*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_BEBA_FF43_RDn, BAKE_SCX7n.qp_new());
}

void RegSCX::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p23.ARUR*/ wire2 _ARUR_FF43_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.XAVY_FF43p());
  /* p23.AMUN*/ wire2 _AMUN_FF43_WRn = not1(_ARUR_FF43_WRp);
  /* p23.DATY*/ DATY_SCX0n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.DUZU*/ DUZU_SCX1n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.CYXU*/ CYXU_SCX2n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.GUBO*/ GUBO_SCX3n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.BEMY*/ BEMY_SCX4n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.CUZY*/ CUZY_SCX5n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.CABU*/ CABU_SCX6n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.BAKE*/ BAKE_SCX7n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegLY::read(GateBoyCpuBus& cpu_bus)
{
  /* p23.WAFU*/ wire2 _WAFU_FF44_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XOGY_FF44p());
  /* p23.VARO*/ wire2 _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

  /*#p23.WURY*/ wire2 _WURY_LY0n = not1(MUWY_LY0p.qp_new());
  /* p23.XEPO*/ wire2 _XEPO_LY1n = not1(MYRO_LY1p.qp_new());
  /* p23.MYFA*/ wire2 _MYFA_LY2n = not1(LEXA_LY2p.qp_new());
  /* p23.XUHY*/ wire2 _XUHY_LY3n = not1(LYDO_LY3p.qp_new());
  /* p23.WATA*/ wire2 _WATA_LY4n = not1(LOVU_LY4p.qp_new());
  /* p23.XAGA*/ wire2 _XAGA_LY5n = not1(LEMA_LY5p.qp_new());
  /* p23.XUCE*/ wire2 _XUCE_LY6n = not1(MATO_LY6p.qp_new());
  /* p23.XOWO*/ wire2 _XOWO_LY7n = not1(LAFO_LY7p.qp_new());

  /* p23.VEGA*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_VARO_FF44_RDn, _WURY_LY0n);
  /* p23.WUVA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_VARO_FF44_RDn, _XEPO_LY1n);
  /* p23.LYCO*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_VARO_FF44_RDn, _MYFA_LY2n);
  /* p23.WOJY*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_VARO_FF44_RDn, _XUHY_LY3n);
  /* p23.VYNE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_VARO_FF44_RDn, _WATA_LY4n);
  /* p23.WAMA*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_VARO_FF44_RDn, _XAGA_LY5n);
  /* p23.WAVO*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_VARO_FF44_RDn, _XUCE_LY6n);
  /* p23.WEZE*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_VARO_FF44_RDn, _XOWO_LY7n);
}

void RegLY::tock2(GateBoyResetDebug& rst, RegLX& reg_lx)
{
  /*#p21.NOKO*/ wire2 _NOKO_y153p_old = and4(LAFO_LY7p.qp_old(), LOVU_LY4p.qp_old(), LYDO_LY3p.qp_old(), MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*#p21.MYTA*/ MYTA_y153p.dff17(reg_lx.NYPE_x113p_new(), rst.LYFE_VID_RSTn(), _NOKO_y153p_old);

  /*#p21.LAMA*/ wire2 _LAMA_Y_RSTn = nor2(MYTA_y153p.qp_new(), rst.LYHA_VID_RSTp());
  /*#p21.MUWY*/ MUWY_LY0p.dff17(reg_lx.RUTU_x113p_new(), _LAMA_Y_RSTn, MUWY_LY0p.qn_old());
  /*#p21.MYRO*/ MYRO_LY1p.dff17(MUWY_LY0p.qn_new(),      _LAMA_Y_RSTn, MYRO_LY1p.qn_old());
  /*#p21.LEXA*/ LEXA_LY2p.dff17(MYRO_LY1p.qn_new(),      _LAMA_Y_RSTn, LEXA_LY2p.qn_old());
  /*#p21.LYDO*/ LYDO_LY3p.dff17(LEXA_LY2p.qn_new(),      _LAMA_Y_RSTn, LYDO_LY3p.qn_old());
  /*#p21.LOVU*/ LOVU_LY4p.dff17(LYDO_LY3p.qn_new(),      _LAMA_Y_RSTn, LOVU_LY4p.qn_old());
  /*#p21.LEMA*/ LEMA_LY5p.dff17(LOVU_LY4p.qn_new(),      _LAMA_Y_RSTn, LEMA_LY5p.qn_old());
  /*#p21.MATO*/ MATO_LY6p.dff17(LEMA_LY5p.qn_new(),      _LAMA_Y_RSTn, MATO_LY6p.qn_old());
  /*#p21.LAFO*/ LAFO_LY7p.dff17(MATO_LY6p.qn_new(),      _LAMA_Y_RSTn, LAFO_LY7p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

wire2 RegLX::TEGY_STROBE() const {
  /* p21.SAXO*/ wire2 _SAXO_LX0p = this->SAXO_LX0p.qp_new();
  /* p21.TYPO*/ wire2 _TYPO_LX1p = this->TYPO_LX1p.qp_new();
  /* p21.VYZO*/ wire2 _VYZO_LX2p = this->VYZO_LX2p.qp_new();
  /* p21.TELU*/ wire2 _TELU_LX3p = this->TELU_LX3p.qp_new();
  /* p21.SUDE*/ wire2 _SUDE_LX4p = this->SUDE_LX4p.qp_new();
  /* p21.TAHA*/ wire2 _TAHA_LX5p = this->TAHA_LX5p.qp_new();
  /* p21.TYRY*/ wire2 _TYRY_LX6p = this->TYRY_LX6p.qp_new();

  /*#p21.TOCU*/ wire2 _TOCU_LX0n = not1(_SAXO_LX0p);
  /*#p21.VEPE*/ wire2 _VEPE_LX1n = not1(_TYPO_LX1p);
  /* p21.VUTY*/ wire2 _VUTY_LX2n = not1(_VYZO_LX2p);
  /* p21.VATE*/ wire2 _VATE_LX3n = not1(_TELU_LX3p);
  /* p21.TUDA*/ wire2 _TUDA_LX4n = not1(_SUDE_LX4p);
  /* p21.TAFY*/ wire2 _TAFY_LX5n = not1(_TAHA_LX5p);
  /* p21.TUJU*/ wire2 _TUJU_LX6n = not1(_TYRY_LX6p);

  /* p21.VOKU*/ wire2 _VOKU_LX000n = nand7(_TUJU_LX6n, _TAFY_LX5n, _TUDA_LX4n, _VATE_LX3n, _VUTY_LX2n, _VEPE_LX1n, _TOCU_LX0n); // 0000000 == 0
  /* p21.TOZU*/ wire2 _TOZU_LX007n = nand7(_TUJU_LX6n, _TAFY_LX5n, _TUDA_LX4n, _VATE_LX3n, _VYZO_LX2p, _TYPO_LX1p, _SAXO_LX0p); // 0000111 == 7
  /* p21.TECE*/ wire2 _TECE_LX045n = nand7(_TUJU_LX6n, _TAHA_LX5p, _TUDA_LX4n, _TELU_LX3p, _VYZO_LX2p, _VEPE_LX1n, _SAXO_LX0p); // 0101101 == 45
  /*#p21.TEBO*/ wire2 _TEBO_LX083n = nand7(_TYRY_LX6p, _TAFY_LX5n, _SUDE_LX4p, _VATE_LX3n, _VUTY_LX2n, _TYPO_LX1p, _SAXO_LX0p); // 1010011 == 83
  /*#p21.TEGY*/ wire2 _TEGY_STROBE = nand4(_VOKU_LX000n, _TOZU_LX007n, _TECE_LX045n, _TEBO_LX083n);

  return _TEGY_STROBE;
}

void RegLX::tock(GateBoyResetDebug& rst, GateBoyClock& clk) {
  /*#p21.SANU*/ wire2 _SANU_x113p_old = and4(TYRY_LX6p.qp_old(), TAHA_LX5p.qp_old(), SUDE_LX4p.qp_old(), SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*#p21.NYPE*/ NYPE_x113p.dff17(clk.TALU_xxCDEFxx(), rst.LYFE_VID_RSTn(), RUTU_x113p.qp_old());
  /*#p21.RUTU*/ RUTU_x113p.dff17(clk.SONO_ABxxxxGH(), rst.LYFE_VID_RSTn(), _SANU_x113p_old);

  /*#p21.MUDE*/ wire2 _MUDE_X_RSTn = nor2(RUTU_x113p.qp_new(), rst.LYHA_VID_RSTp());
  /*#p21.SAXO*/ SAXO_LX0p.dff17(clk.TALU_xxCDEFxx(), _MUDE_X_RSTn, SAXO_LX0p.qn_old());
  /*#p21.TYPO*/ TYPO_LX1p.dff17(SAXO_LX0p.qn_new(), _MUDE_X_RSTn, TYPO_LX1p.qn_old());
  /*#p21.VYZO*/ VYZO_LX2p.dff17(TYPO_LX1p.qn_new(), _MUDE_X_RSTn, VYZO_LX2p.qn_old());
  /*#p21.TELU*/ TELU_LX3p.dff17(VYZO_LX2p.qn_new(), _MUDE_X_RSTn, TELU_LX3p.qn_old());
  /*#p21.SUDE*/ SUDE_LX4p.dff17(TELU_LX3p.qn_new(), _MUDE_X_RSTn, SUDE_LX4p.qn_old());
  /*#p21.TAHA*/ TAHA_LX5p.dff17(SUDE_LX4p.qn_new(), _MUDE_X_RSTn, TAHA_LX5p.qn_old());
  /*#p21.TYRY*/ TYRY_LX6p.dff17(TAHA_LX5p.qn_new(), _MUDE_X_RSTn, TYRY_LX6p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegLYC::read(GateBoyCpuBus& cpu_bus)
{
  /* p23.XYLY*/ wire2 _XYLY_FF45_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XAYU_FF45p());
  /* p23.WEKU*/ wire2 _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);
  /*#p23.RETU*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_WEKU_FF45_RDn, SYRY_LYC0n.qp_new());
  /* p23.VOJO*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_WEKU_FF45_RDn, VUCE_LYC1n.qp_new());
  /* p23.RAZU*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_WEKU_FF45_RDn, SEDY_LYC2n.qp_new());
  /* p23.REDY*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_WEKU_FF45_RDn, SALO_LYC3n.qp_new());
  /* p23.RACE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_WEKU_FF45_RDn, SOTA_LYC4n.qp_new());
  /*#p23.VAZU*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_WEKU_FF45_RDn, VAFA_LYC5n.qp_new());
  /* p23.VAFE*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_WEKU_FF45_RDn, VEVO_LYC6n.qp_new());
  /* p23.PUFY*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_WEKU_FF45_RDn, RAHA_LYC7n.qp_new());
}

void RegLYC::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  /* p23.XUFA*/ wire2 _XUFA_FF45_WRn = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.XAYU_FF45p());
  /* p23.WANE*/ wire2 _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);
  /* p23.SYRY*/ SYRY_LYC0n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.VUCE*/ VUCE_LYC1n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.SEDY*/ SEDY_LYC2n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.SALO*/ SALO_LYC3n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.SOTA*/ SOTA_LYC4n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.VAFA*/ VAFA_LYC5n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.VEVO*/ VEVO_LYC6n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.RAHA*/ RAHA_LYC7n.dff9(_WANE_FF45_WRp, rst.WESY_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

void RegLYC::tock2(GateBoyResetDebug& rst, GateBoyClock& clk, const RegLY& reg_ly)
{
  // LYC matcher
  /* p21.RYME*/ wire2 _RYME_LY_MATCH0n_old = xor2(reg_ly.MUWY_LY0p.qp_old(), SYRY_LYC0n.qn_old());
  /* p21.TYDE*/ wire2 _TYDE_LY_MATCH1n_old = xor2(reg_ly.MYRO_LY1p.qp_old(), VUCE_LYC1n.qn_old());
  /* p21.REDA*/ wire2 _REDA_LY_MATCH2n_old = xor2(reg_ly.LEXA_LY2p.qp_old(), SEDY_LYC2n.qn_old());
  /* p21.RASY*/ wire2 _RASY_LY_MATCH3n_old = xor2(reg_ly.LYDO_LY3p.qp_old(), SALO_LYC3n.qn_old());
  /* p21.TYKU*/ wire2 _TYKU_LY_MATCH4n_old = xor2(reg_ly.LOVU_LY4p.qp_old(), SOTA_LYC4n.qn_old());
  /* p21.TUCY*/ wire2 _TUCY_LY_MATCH5n_old = xor2(reg_ly.LEMA_LY5p.qp_old(), VAFA_LYC5n.qn_old());
  /* p21.TERY*/ wire2 _TERY_LY_MATCH6n_old = xor2(reg_ly.MATO_LY6p.qp_old(), VEVO_LYC6n.qn_old());
  /* p21.SYFU*/ wire2 _SYFU_LY_MATCH7n_old = xor2(reg_ly.LAFO_LY7p.qp_old(), RAHA_LYC7n.qn_old());

  /*#p21.SOVU*/ wire2 _SOVU_LY_MATCHA_old = nor4 (_SYFU_LY_MATCH7n_old, _TERY_LY_MATCH6n_old, _TUCY_LY_MATCH5n_old, _TYKU_LY_MATCH4n_old);
  /*#p21.SUBO*/ wire2 _SUBO_LY_MATCHB_old = nor4 (_RASY_LY_MATCH3n_old, _REDA_LY_MATCH2n_old, _TYDE_LY_MATCH1n_old, _RYME_LY_MATCH0n_old);
  /*#p21.RAPE*/ wire2 _RAPE_LY_MATCHn_old = nand2(_SOVU_LY_MATCHA_old,  _SUBO_LY_MATCHB_old);
  /*#p21.PALY*/ wire2 _PALY_LY_MATCHa_old = not1 (_RAPE_LY_MATCHn_old);

  /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp.dff17(clk.TALU_xxCDEFxx(), rst.WESY_SYS_RSTn(), _PALY_LY_MATCHa_old);
}

//------------------------------------------------------------------------------------------------------------------------

void RegBGP::read(GateBoyCpuBus& cpu_bus)
{
  /* p36.VUSO*/ wire2 _VUSO_FF47_RDp = and2(cpu_bus.ASOT_CPU_RDp(), WERA_FF47p(cpu_bus.BUS_CPU_A));
  /* p36.TEPY*/ wire2 _TEPY_FF47_RDn = not1(_VUSO_FF47_RDp);

  /*#p36.RARO*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_TEPY_FF47_RDn, PAVO_BGP_D0n.qp_new());
  /* p36.PABA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_TEPY_FF47_RDn, NUSY_BGP_D1n.qp_new());
  /* p36.REDO*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_TEPY_FF47_RDn, PYLU_BGP_D2n.qp_new());
  /* p36.LOBE*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_TEPY_FF47_RDn, MAXY_BGP_D3n.qp_new());
  /* p36.LACE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_TEPY_FF47_RDn, MUKE_BGP_D4n.qp_new());
  /* p36.LYKA*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_TEPY_FF47_RDn, MORU_BGP_D5n.qp_new());
  /* p36.LODY*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_TEPY_FF47_RDn, MOGY_BGP_D6n.qp_new());
  /* p36.LARY*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_TEPY_FF47_RDn, MENA_BGP_D7n.qp_new());
}

void RegBGP::write_sync(GateBoyCpuBus& cpu_bus)
{
  /* p36.VELY*/ wire2 _VELY_FF47_WRp = and2(cpu_bus.CUPA_CPU_WRp(), WERA_FF47p(cpu_bus.BUS_CPU_A));
  /* p36.TEPO*/ wire2 _TEPO_FF47_WRp = not1(_VELY_FF47_WRp);

  /*#p36.PAVO*/ PAVO_BGP_D0n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[0].qp_old());
  /* p36.NUSY*/ NUSY_BGP_D1n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[1].qp_old());
  /* p36.PYLU*/ PYLU_BGP_D2n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[2].qp_old());
  /* p36.MAXY*/ MAXY_BGP_D3n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[3].qp_old());
  /* p36.MUKE*/ MUKE_BGP_D4n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[4].qp_old());
  /* p36.MORU*/ MORU_BGP_D5n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[5].qp_old());
  /* p36.MOGY*/ MOGY_BGP_D6n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[6].qp_old());
  /* p36.MENA*/ MENA_BGP_D7n.dff8p(_TEPO_FF47_WRp, cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegOBP0::read(GateBoyCpuBus& cpu_bus)
{
  // FF48 OBP0
  /* p36.XUFY*/ wire2 _XUFY_FF48_RDp = and2(cpu_bus.ASOT_CPU_RDp(), XAYO_FF48p(cpu_bus.BUS_CPU_A));
  /* p36.XOZY*/ wire2 _XOZY_FF48_RDn = not1(_XUFY_FF48_RDp);

  /*#p36.XARY*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_XOZY_FF48_RDn, XUFU_OBP0_D0n.qp_new());
  /* p36.XOKE*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_XOZY_FF48_RDn, XUKY_OBP0_D1n.qp_new());
  /* p36.XUNO*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_XOZY_FF48_RDn, XOVA_OBP0_D2n.qp_new());
  /* p36.XUBY*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_XOZY_FF48_RDn, XALO_OBP0_D3n.qp_new());
  /* p36.XAJU*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_XOZY_FF48_RDn, XERU_OBP0_D4n.qp_new());
  /* p36.XOBO*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_XOZY_FF48_RDn, XYZE_OBP0_D5n.qp_new());
  /* p36.XAXA*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_XOZY_FF48_RDn, XUPO_OBP0_D6n.qp_new());
  /* p36.XAWO*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_XOZY_FF48_RDn, XANA_OBP0_D7n.qp_new());
}

void RegOBP0::write_sync(GateBoyCpuBus& cpu_bus)
{
  // FF48 OBP0
  /* p36.XOMA*/ wire2 _XOMA_FF48_WRp = and2(cpu_bus.CUPA_CPU_WRp(), XAYO_FF48p(cpu_bus.BUS_CPU_A));
  /* p36.XELO*/ wire2 _XELO_FF48_WRn = not1(_XOMA_FF48_WRp);

  /* p36.XUFU*/ XUFU_OBP0_D0n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[0].qp_old());
  /* p36.XUKY*/ XUKY_OBP0_D1n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[1].qp_old());
  /* p36.XOVA*/ XOVA_OBP0_D2n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[2].qp_old());
  /* p36.XALO*/ XALO_OBP0_D3n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[3].qp_old());
  /* p36.XERU*/ XERU_OBP0_D4n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[4].qp_old());
  /* p36.XYZE*/ XYZE_OBP0_D5n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[5].qp_old());
  /* p36.XUPO*/ XUPO_OBP0_D6n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[6].qp_old());
  /* p36.XANA*/ XANA_OBP0_D7n.dff8p(_XELO_FF48_WRn, cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegOBP1::read(GateBoyCpuBus& cpu_bus)
{
  /* p36.MUMY*/ wire2 _MUMY_FF49_RDp = and2(cpu_bus.ASOT_CPU_RDp(), TEGO_FF49p(cpu_bus.BUS_CPU_A));
  /* p36.LOTE*/ wire2 _LOTE_FF49_RDn = not1(_MUMY_FF49_RDp);

  /*#p36.LAJU*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_LOTE_FF49_RDn, MOXY_OBP1_D0n.qp_new());
  /* p36.LEPA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_LOTE_FF49_RDn, LAWO_OBP1_D1n.qp_new());
  /* p36.LODE*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_LOTE_FF49_RDn, MOSA_OBP1_D2n.qp_new());
  /* p36.LYZA*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_LOTE_FF49_RDn, LOSE_OBP1_D3n.qp_new());
  /* p36.LUKY*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_LOTE_FF49_RDn, LUNE_OBP1_D4n.qp_new());
  /* p36.LUGA*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_LOTE_FF49_RDn, LUGU_OBP1_D5n.qp_new());
  /* p36.LEBA*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_LOTE_FF49_RDn, LEPU_OBP1_D6n.qp_new());
  /* p36.LELU*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_LOTE_FF49_RDn, LUXO_OBP1_D7n.qp_new());
}

void RegOBP1::write_sync(GateBoyCpuBus& cpu_bus)
{
  /* p36.MYXE*/ wire2 _MYXE_FF49_WRp = and2(cpu_bus.CUPA_CPU_WRp(), TEGO_FF49p(cpu_bus.BUS_CPU_A));
  /* p36.LEHO*/ wire2 _LEHO_FF49_WRn = not1(_MYXE_FF49_WRp);

  /* p36.MOXY*/ MOXY_OBP1_D0n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[0].qp_old());
  /* p36.LAWO*/ LAWO_OBP1_D1n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[1].qp_old());
  /* p36.MOSA*/ MOSA_OBP1_D2n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[2].qp_old());
  /* p36.LOSE*/ LOSE_OBP1_D3n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[3].qp_old());
  /* p36.LUNE*/ LUNE_OBP1_D4n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[4].qp_old());
  /* p36.LUGU*/ LUGU_OBP1_D5n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[5].qp_old());
  /* p36.LEPU*/ LEPU_OBP1_D6n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[6].qp_old());
  /* p36.LUXO*/ LUXO_OBP1_D7n.dff8p(_LEHO_FF49_WRn, cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void RegWY::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  /* p23.WEKO*/ wire2 _WEKO_FF4A_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VYGA_FF4Ap());
  /* p23.VEFU*/ wire2 _VEFU_FF4A_WRn = not1(_WEKO_FF4A_WRp);
  /* p23.NESO*/ NESO_WY0n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.NYRO*/ NYRO_WY1n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.NAGA*/ NAGA_WY2n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.MELA*/ MELA_WY3n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.NULO*/ NULO_WY4n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.NENE*/ NENE_WY5n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.NUKA*/ NUKA_WY6n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.NAFU*/ NAFU_WY7n.dff9(_VEFU_FF4A_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

void RegWY::read(GateBoyCpuBus& cpu_bus)
{
  /* p23.WAXU*/ wire2 _WAXU_FF4A_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VYGA_FF4Ap());
  /* p23.VOMY*/ wire2 _VOMY_FF4A_RDn = not1(_WAXU_FF4A_RDp);
  /*#p23.PUNU*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_VOMY_FF4A_RDn, NESO_WY0n.qp_new());
  /* p23.PODA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_VOMY_FF4A_RDn, NYRO_WY1n.qp_new());
  /* p23.PYGU*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_VOMY_FF4A_RDn, NAGA_WY2n.qp_new());
  /* p23.LOKA*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_VOMY_FF4A_RDn, MELA_WY3n.qp_new());
  /* p23.MEGA*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_VOMY_FF4A_RDn, NULO_WY4n.qp_new());
  /* p23.PELA*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_VOMY_FF4A_RDn, NENE_WY5n.qp_new());
  /* p23.POLO*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_VOMY_FF4A_RDn, NUKA_WY6n.qp_new());
  /* p23.MERA*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_VOMY_FF4A_RDn, NAFU_WY7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegWX::read(GateBoyCpuBus& cpu_bus) {
  /* p23.WYZE*/ wire2 _WYZE_FF4B_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VUMY_FF4Bp());
  /* p23.VYCU*/ wire2 _VYCU_FF4B_RDn = not1(_WYZE_FF4B_RDp);
  /*#p23.LOVA*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_VYCU_FF4B_RDn, MYPA_WX0n.qp_new());
  /* p23.MUKA*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_VYCU_FF4B_RDn, NOFE_WX1n.qp_new());
  /* p23.MOKO*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_VYCU_FF4B_RDn, NOKE_WX2n.qp_new());
  /* p23.LOLE*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_VYCU_FF4B_RDn, MEBY_WX3n.qp_new());
  /* p23.MELE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_VYCU_FF4B_RDn, MYPU_WX4n.qp_new());
  /* p23.MUFE*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_VYCU_FF4B_RDn, MYCE_WX5n.qp_new());
  /* p23.MULY*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_VYCU_FF4B_RDn, MUVO_WX6n.qp_new());
  /* p23.MARA*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_VYCU_FF4B_RDn, NUKU_WX7n.qp_new());
}

void RegWX::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p23.WUZA*/ wire2 _WUZA_FF4B_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.VUMY_FF4Bp());
  /* p23.VOXU*/ wire2 _VOXU_FF4B_WRn = not1(_WUZA_FF4B_WRp);
  /* p23.MYPA*/ MYPA_WX0n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p23.NOFE*/ NOFE_WX1n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p23.NOKE*/ NOKE_WX2n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p23.MEBY*/ MEBY_WX3n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p23.MYPU*/ MYPU_WX4n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p23.MYCE*/ MYCE_WX5n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p23.MUVO*/ MUVO_WX6n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p23.NUKU*/ NUKU_WX7n.dff9(_VOXU_FF4B_WRn, rst.WALU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void TileTempA::tock(Bus BUS_VRAM_Dp[8], wire2 LOMA_LATCH_TILE_DAn)
{
  /* p32.LEGU*/ LEGU_TILE_DA0n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[0].qp_old());
  /* p32.NUDU*/ NUDU_TILE_DA1n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[1].qp_old());
  /* p32.MUKU*/ MUKU_TILE_DA2n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[2].qp_old());
  /* p32.LUZO*/ LUZO_TILE_DA3n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[3].qp_old());
  /* p32.MEGU*/ MEGU_TILE_DA4n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[4].qp_old());
  /* p32.MYJY*/ MYJY_TILE_DA5n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[5].qp_old());
  /* p32.NASA*/ NASA_TILE_DA6n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[6].qp_old());
  /* p32.NEFO*/ NEFO_TILE_DA7n.dff8p(LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void TileTempB::tock(Bus BUS_VRAM_Dp[8], wire2 LABU_LATCH_TILE_DBn)
{
  wire2 _VYPO_VCC = 1;
  // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
  /* p32.RAWU*/ RAWU_TILE_DB0p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[0].qp_old());
  /* p32.POZO*/ POZO_TILE_DB1p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[1].qp_old());
  /* p32.PYZO*/ PYZO_TILE_DB2p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[2].qp_old());
  /* p32.POXA*/ POXA_TILE_DB3p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[3].qp_old());
  /* p32.PULO*/ PULO_TILE_DB4p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[4].qp_old());
  /* p32.POJU*/ POJU_TILE_DB5p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[5].qp_old());
  /* p32.POWY*/ POWY_TILE_DB6p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[6].qp_old());
  /* p32.PYJU*/ PYJU_TILE_DB7p.dff11(LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void SpritePixA::store_sprite_pix(SpritePix sprite, wire2 XADO_STORE_SPRITE_An) {
  /* p33.REWO*/ REWO_SPRITE_DA0n.dff8n(XADO_STORE_SPRITE_An, sprite.PUTE_FLIP0p);
  /* p33.PEBA*/ PEBA_SPRITE_DA1n.dff8n(XADO_STORE_SPRITE_An, sprite.PELO_FLIP1p);
  /* p33.MOFO*/ MOFO_SPRITE_DA2n.dff8n(XADO_STORE_SPRITE_An, sprite.PONO_FLIP2p);
  /* p33.PUDU*/ PUDU_SPRITE_DA3n.dff8n(XADO_STORE_SPRITE_An, sprite.POBE_FLIP3p);
  /* p33.SAJA*/ SAJA_SPRITE_DA4n.dff8n(XADO_STORE_SPRITE_An, sprite.PACY_FLIP4p);
  /* p33.SUNY*/ SUNY_SPRITE_DA5n.dff8n(XADO_STORE_SPRITE_An, sprite.PUGU_FLIP5p);
  /* p33.SEMO*/ SEMO_SPRITE_DA6n.dff8n(XADO_STORE_SPRITE_An, sprite.PAWE_FLIP6p);
  /* p33.SEGA*/ SEGA_SPRITE_DA7n.dff8n(XADO_STORE_SPRITE_An, sprite.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void SpritePixB::store_sprite_pix(SpritePix sprite, wire2 PUCO_STORE_SPRITE_Bn) {
  /* p33.PEFO*/ PEFO_SPRITE_DB0n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PUTE_FLIP0p);
  /* p33.ROKA*/ ROKA_SPRITE_DB1n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PELO_FLIP1p);
  /* p33.MYTU*/ MYTU_SPRITE_DB2n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PONO_FLIP2p);
  /* p33.RAMU*/ RAMU_SPRITE_DB3n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.POBE_FLIP3p);
  /* p33.SELE*/ SELE_SPRITE_DB4n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PACY_FLIP4p);
  /* p33.SUTO*/ SUTO_SPRITE_DB5n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PUGU_FLIP5p);
  /* p33.RAMA*/ RAMA_SPRITE_DB6n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PAWE_FLIP6p);
  /* p33.RYDU*/ RYDU_SPRITE_DB7n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void WinMapX::tock(const GateBoyResetDebug& rst, wire2 TEVO_FETCH_TRIGp, wire2 PORE_WIN_MODEp, wire2 WYMO_LCDC_WINENp, wire2 XAHY_LINE_RSTn) {
  /* p27.VETU*/ wire2 _VETU_WIN_MAPp = and2(TEVO_FETCH_TRIGp, PORE_WIN_MODEp);
  /*#p27.XOFO*/ wire2 _XOFO_WIN_RSTp = nand3(WYMO_LCDC_WINENp, XAHY_LINE_RSTn, rst.XAPO_VID_RSTn());
  /* p27.XACO*/ wire2 _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp);
  /* p27.WYKA*/ WYKA_WIN_X3.dff17(_VETU_WIN_MAPp,       _XACO_WIN_RSTn, WYKA_WIN_X3.qn_old());
  /* p27.WODY*/ WODY_WIN_X4.dff17(WYKA_WIN_X3.qn_new(), _XACO_WIN_RSTn, WODY_WIN_X4.qn_old());
  /* p27.WOBO*/ WOBO_WIN_X5.dff17(WODY_WIN_X4.qn_new(), _XACO_WIN_RSTn, WOBO_WIN_X5.qn_old());
  /* p27.WYKO*/ WYKO_WIN_X6.dff17(WOBO_WIN_X5.qn_new(), _XACO_WIN_RSTn, WYKO_WIN_X6.qn_old());
  /* p27.XOLO*/ XOLO_WIN_X7.dff17(WYKO_WIN_X6.qn_new(), _XACO_WIN_RSTn, XOLO_WIN_X7.qn_old());

}

//------------------------------------------------------------------------------------------------------------------------

void WinLineY::tock(const GateBoyResetDebug& rst, wire2 PORE_WIN_MODEp, wire2 PARU_VBLANKp) {
  // Every time we leave win mode we increment win_y
  /* p27.WAZY*/ wire2 _WAZY_WIN_MODEn = not1(PORE_WIN_MODEp);
  /* p27.REPU*/ wire2 _REPU_VBLANKp   = or2(PARU_VBLANKp, rst.PYRY_VID_RSTp());
  /* p27.SYNY*/ wire2 _SYNY_VBLANKn   = not1(_REPU_VBLANKp);
  /* p27.VYNO*/ VYNO_WIN_Y0.dff17(_WAZY_WIN_MODEn,      _SYNY_VBLANKn, VYNO_WIN_Y0.qn_old());
  /* p27.VUJO*/ VUJO_WIN_Y1.dff17(VYNO_WIN_Y0.qn_new(), _SYNY_VBLANKn, VUJO_WIN_Y1.qn_old());
  /* p27.VYMU*/ VYMU_WIN_Y2.dff17(VUJO_WIN_Y1.qn_new(), _SYNY_VBLANKn, VYMU_WIN_Y2.qn_old());
  /* p27.TUFU*/ TUFU_WIN_Y3.dff17(VYMU_WIN_Y2.qn_new(), _SYNY_VBLANKn, TUFU_WIN_Y3.qn_old());
  /* p27.TAXA*/ TAXA_WIN_Y4.dff17(TUFU_WIN_Y3.qn_new(), _SYNY_VBLANKn, TAXA_WIN_Y4.qn_old());
  /* p27.TOZO*/ TOZO_WIN_Y5.dff17(TAXA_WIN_Y4.qn_new(), _SYNY_VBLANKn, TOZO_WIN_Y5.qn_old());
  /* p27.TATE*/ TATE_WIN_Y6.dff17(TOZO_WIN_Y5.qn_new(), _SYNY_VBLANKn, TATE_WIN_Y6.qn_old());
  /* p27.TEKE*/ TEKE_WIN_Y7.dff17(TATE_WIN_Y6.qn_new(), _SYNY_VBLANKn, TEKE_WIN_Y7.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------
