#include "Sch_TileFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

// NOR latch
// POKY00 << PYGO
// POKY01 nc
// POKY02 >> nc
// POKY03 >> ROMO00, others
// POKY04 nc
// POKY05 << LOBY01
// If PYGO goes high, POKY03 goes high
// if LOBY goes high, POKY03 goes low.

//------------------------------------------------------------------------------

TileFetcherSignals TileFetcher::sig(const SchematicTop& gb) const {
  TileFetcherSignals tile_fetcher_sig;

  auto ppu_sig = gb.ppu_reg.sig(gb);

  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0, NYVA_BFETCH_S2, ppu_sig.NYXU_TILE_FETCHER_RSTn);
  /*p27.LYRY*/ tile_fetcher_sig.LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);


  /*p24.POKY*/ tile_fetcher_sig.POKY_PORCH_DONEp = POKY_PORCH_DONEp;
  /*p24.PORY*/ tile_fetcher_sig.PORY_FETCH_DONE_Bp = PORY_FETCH_DONE_Bp;

  tile_fetcher_sig.LONY_BG_READ_VRAM_LATCHp = LONY_BG_READ_VRAM_LATCHp;

  /*p32.LEGU*/ tile_fetcher_sig.BG_PIX_A0 = BG_PIX_A0;
  /*p32.NUDU*/ tile_fetcher_sig.BG_PIX_A1 = BG_PIX_A1;
  /*p32.MUKU*/ tile_fetcher_sig.BG_PIX_A2 = BG_PIX_A2;
  /*p32.LUZO*/ tile_fetcher_sig.BG_PIX_A3 = BG_PIX_A3;
  /*p32.MEGU*/ tile_fetcher_sig.BG_PIX_A4 = BG_PIX_A4;
  /*p32.MYJY*/ tile_fetcher_sig.BG_PIX_A5 = BG_PIX_A5;
  /*p32.NASA*/ tile_fetcher_sig.BG_PIX_A6 = BG_PIX_A6;
  /*p32.NEFO*/ tile_fetcher_sig.BG_PIX_A7 = BG_PIX_A7;

  /*p32.RAWU*/ tile_fetcher_sig.BG_PIX_B0 = BG_PIX_B0;
  /*p32.POZO*/ tile_fetcher_sig.BG_PIX_B1 = BG_PIX_B1;
  /*p32.PYZO*/ tile_fetcher_sig.BG_PIX_B2 = BG_PIX_B2; 
  /*p32.POXA*/ tile_fetcher_sig.BG_PIX_B3 = BG_PIX_B3; 
  /*p32.PULO*/ tile_fetcher_sig.BG_PIX_B4 = BG_PIX_B4; 
  /*p32.POJU*/ tile_fetcher_sig.BG_PIX_B5 = BG_PIX_B5; 
  /*p32.POWY*/ tile_fetcher_sig.BG_PIX_B6 = BG_PIX_B6; 
  /*p32.PYJU*/ tile_fetcher_sig.BG_PIX_B7 = BG_PIX_B7;

  {
    /*p27.ROMO*/ wire _ROMO_AFTER_PORCHn = not(POKY_PORCH_DONEp);
    /*p27.SUVU*/ wire _SUVU_PORCH_ENDn = nand(ppu_sig.XYMU_RENDERINGp, _ROMO_AFTER_PORCHn, NYKA_FETCH_DONE_Ap, PORY_FETCH_DONE_Bp);
    /*p27.TAVE*/ tile_fetcher_sig.TAVE_PORCH_DONE_TRIGp = not(_SUVU_PORCH_ENDn);
  }

  // something here doesn't make sense... these two signals should not be constant during a line
  /*p32.LOZE*/ tile_fetcher_sig.LOZE_PIPE_A_LOAD = not(ppu_sig.NYXU_TILE_FETCHER_RSTn);
  /*p32.LUXA*/ tile_fetcher_sig.LUXA_PIPE_B_LOAD = not(ppu_sig.NYXU_TILE_FETCHER_RSTn);

  return tile_fetcher_sig;
}

//------------------------------------------------------------------------------

void TileFetcher::tick(SchematicTop& top) {
  auto ppu_sig = top.ppu_reg.sig(top);
  
  auto win_sig = top.win_reg.sig(top);
  auto tile_fetcher_sig = sig(top);

  auto& ppu_config = top.ppu_config;

  //----------------------------------------

  {
    /*p27.LURY*/ wire LURY_BG_READ_VRAM_LATCH_RSTn = and(!LOVY_FETCH_DONEp, ppu_sig.XYMU_RENDERINGp);
    /*p27.LONY*/ LONY_BG_READ_VRAM_LATCHp.nand_latch(ppu_sig.NYXU_TILE_FETCHER_RSTn, LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  {
    /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0, NYVA_BFETCH_S2, ppu_sig.NYXU_TILE_FETCHER_RSTn);
    /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.PIN_CLK_IN_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
    /*p27.MYVO*/ wire MYVO_AxCxExGx = not(ALET_xBxDxFxH);
    /*p27.LOVY*/ LOVY_FETCH_DONEp.set(MYVO_AxCxExGx, ppu_sig.NYXU_TILE_FETCHER_RSTn, LYRY_BFETCH_DONEp);

    /*p27.LEBO*/ wire LEBO_AxCxExGx = nand(ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

    /*p27.LAXU*/ LAXU_BFETCH_S0.set(LEBO_AxCxExGx,   ppu_sig.NYXU_TILE_FETCHER_RSTn, !LAXU_BFETCH_S0);
    /*p27.MESU*/ MESU_BFETCH_S1.set(!LAXU_BFETCH_S0, ppu_sig.NYXU_TILE_FETCHER_RSTn, !MESU_BFETCH_S1);
    /*p27.NYVA*/ NYVA_BFETCH_S2.set(!MESU_BFETCH_S1, ppu_sig.NYXU_TILE_FETCHER_RSTn, !NYVA_BFETCH_S2);

    /*p27.LYZU*/ LYZU_BFETCH_S0_DELAY.set (ALET_xBxDxFxH, ppu_sig.XYMU_RENDERINGp,         LAXU_BFETCH_S0);
  }

  {
    /*p24.LOBY*/ wire LOBY_RENDERINGn = not(ppu_sig.XYMU_RENDERINGp);
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(win_sig.NUNY_WIN_MODE_TRIGp);
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);
    /*p24.NAFY*/ wire NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);
    /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0, NYVA_BFETCH_S2, ppu_sig.NYXU_TILE_FETCHER_RSTn);
    /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.PIN_CLK_IN_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);

    /*p27.MYVO*/ wire MYVO_AxCxExGx = not(ALET_xBxDxFxH);
    /*p24.NYKA*/ NYKA_FETCH_DONE_Ap.set(ALET_xBxDxFxH, NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
    /*p24.PORY*/ PORY_FETCH_DONE_Bp.set(MYVO_AxCxExGx, NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_FETCH_DONE_Ap);
    /*p24.PYGO*/ PYGO_FETCH_DONE_Cp.set(ALET_xBxDxFxH, ppu_sig.XYMU_RENDERINGp,         PORY_FETCH_DONE_Bp);

    /*p24.POKY*/ POKY_PORCH_DONEp.nor_latch(PYGO_FETCH_DONE_Cp, LOBY_RENDERINGn);
  }

  //----------------------------------------

  {
    /*p26.FAFO*/ wire FAFO_TILE_Y0S = add_s(top.MUWY_Y0(), ppu_config.GAVE_SCY0.q(), 0);
    /*p26.FAFO*/ wire FAFO_TILE_Y0C = add_c(top.MUWY_Y0(), ppu_config.GAVE_SCY0.q(), 0);
    /*p26.EMUX*/ wire EMUX_TILE_Y1S = add_s(top.MYRO_Y1(), ppu_config.FYMO_SCY1.q(), FAFO_TILE_Y0C);
    /*p26.EMUX*/ wire EMUX_TILE_Y1C = add_c(top.MYRO_Y1(), ppu_config.FYMO_SCY1.q(), FAFO_TILE_Y0C);
    /*p26.ECAB*/ wire ECAB_TILE_Y2S = add_s(top.LEXA_Y2(), ppu_config.FEZU_SCY2.q(), EMUX_TILE_Y1C);
    /*p26.ECAB*/ wire ECAB_TILE_Y2C = add_c(top.LEXA_Y2(), ppu_config.FEZU_SCY2.q(), EMUX_TILE_Y1C);
    /*p26.ETAM*/ wire ETAM_MAP_Y0S  = add_s(top.LYDO_Y3(), ppu_config.FUJO_SCY3.q(), ECAB_TILE_Y2C);
    /*p26.ETAM*/ wire ETAM_MAP_Y0C  = add_c(top.LYDO_Y3(), ppu_config.FUJO_SCY3.q(), ECAB_TILE_Y2C);
    /*p26.DOTO*/ wire DOTO_MAP_Y1S  = add_s(top.LOVU_Y4(), ppu_config.DEDE_SCY4.q(), ETAM_MAP_Y0C);
    /*p26.DOTO*/ wire DOTO_MAP_Y1C  = add_c(top.LOVU_Y4(), ppu_config.DEDE_SCY4.q(), ETAM_MAP_Y0C);
    /*p26.DABA*/ wire DABA_MAP_Y2S  = add_s(top.LEMA_Y5(), ppu_config.FOTY_SCY5.q(), DOTO_MAP_Y1C);
    /*p26.DABA*/ wire DABA_MAP_Y2C  = add_c(top.LEMA_Y5(), ppu_config.FOTY_SCY5.q(), DOTO_MAP_Y1C);
    /*p26.EFYK*/ wire EFYK_MAP_Y3S  = add_s(top.MATO_Y6(), ppu_config.FOHA_SCY6.q(), DABA_MAP_Y2C);
    /*p26.EFYK*/ wire EFYK_MAP_Y3C  = add_c(top.MATO_Y6(), ppu_config.FOHA_SCY6.q(), DABA_MAP_Y2C);
    /*p26.EJOK*/ wire EJOK_MAP_Y4S  = add_s(top.LAFO_Y7(), ppu_config.FUNY_SCY7.q(), EFYK_MAP_Y3C);
    /*p26.EJOK*/ wire EJOK_MAP_Y4C  = add_c(top.LAFO_Y7(), ppu_config.FUNY_SCY7.q(), EFYK_MAP_Y3C);

    (void)EJOK_MAP_Y4C;

    /*p26.ATAD*/ wire ATAD_TILE_X0S = add_s(ppu_sig.XEHO_X0, ppu_config.DATY_SCX0.q(), 0);
    /*p26.ATAD*/ wire ATAD_TILE_X0C = add_c(ppu_sig.XEHO_X0, ppu_config.DATY_SCX0.q(), 0);
    /*p26.BEHU*/ wire BEHU_TILE_X1S = add_s(ppu_sig.SAVY_X1, ppu_config.DUZU_SCX1.q(), ATAD_TILE_X0C);
    /*p26.BEHU*/ wire BEHU_TILE_X1C = add_c(ppu_sig.SAVY_X1, ppu_config.DUZU_SCX1.q(), ATAD_TILE_X0C);
    /*p26.APYH*/ wire APYH_TILE_X2S = add_s(ppu_sig.XODU_X2, ppu_config.CYXU_SCX2.q(), BEHU_TILE_X1C);
    /*p26.APYH*/ wire APYH_TILE_X2C = add_c(ppu_sig.XODU_X2, ppu_config.CYXU_SCX2.q(), BEHU_TILE_X1C);
    /*p26.BABE*/ wire BABE_MAP_X0S  = add_s(ppu_sig.XYDO_X3, ppu_config.GUBO_SCX3.q(), APYH_TILE_X2C);
    /*p26.BABE*/ wire BABE_MAP_X0C  = add_c(ppu_sig.XYDO_X3, ppu_config.GUBO_SCX3.q(), APYH_TILE_X2C);
    /*p26.ABOD*/ wire ABOD_MAP_X1S  = add_s(ppu_sig.TUHU_X4, ppu_config.BEMY_SCX4.q(), BABE_MAP_X0C);
    /*p26.ABOD*/ wire ABOD_MAP_X1C  = add_c(ppu_sig.TUHU_X4, ppu_config.BEMY_SCX4.q(), BABE_MAP_X0C);
    /*p26.BEWY*/ wire BEWY_MAP_X2S  = add_s(ppu_sig.TUKY_X5, ppu_config.CUZY_SCX5.q(), ABOD_MAP_X1C);
    /*p26.BEWY*/ wire BEWY_MAP_X2C  = add_c(ppu_sig.TUKY_X5, ppu_config.CUZY_SCX5.q(), ABOD_MAP_X1C);
    /*p26.BYCA*/ wire BYCA_MAP_X3S  = add_s(ppu_sig.TAKO_X6, ppu_config.CABU_SCX6.q(), BEWY_MAP_X2C);
    /*p26.BYCA*/ wire BYCA_MAP_X3C  = add_c(ppu_sig.TAKO_X6, ppu_config.CABU_SCX6.q(), BEWY_MAP_X2C);
    /*p26.ACUL*/ wire ACUL_MAP_X4S  = add_s(ppu_sig.SYBE_X7, ppu_config.BAKE_SCX7.q(), BYCA_MAP_X3C);
    /*p26.ACUL*/ wire ACUL_MAP_X4C  = add_c(ppu_sig.SYBE_X7, ppu_config.BAKE_SCX7.q(), BYCA_MAP_X3C);

    (void)ATAD_TILE_X0S;
    (void)BEHU_TILE_X1S;
    (void)APYH_TILE_X2S;
    (void)ACUL_MAP_X4C;

    // Background map read

    wire P10_B = 0;
    /*p27.VYPO*/ wire VYPO_P10_Bn = not(P10_B);

    /*p27.NAKO*/ wire NAKO_FETCH_S1n = not(MESU_BFETCH_S1.q());
    /*p27.NOFU*/ wire NOFU_FETCH_TILE_AB = not(NYVA_BFETCH_S2.q());
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp);
    /*p27.LENA*/ wire LENA_BGW_VRAM_RD = not(LUSU_BGW_VRAM_RDn);
    /*p27.NOGU*/ wire NOGU_FETCH_01p = nand(NAKO_FETCH_S1n, NOFU_FETCH_TILE_AB);
    /*p27.NENY*/ wire NENY_FETCH_01n = not(NOGU_FETCH_01p);
    /*p27.POTU*/ wire POTU_BG_MAP_READp = and (LENA_BGW_VRAM_RD, NENY_FETCH_01n);
    
    /*p27.PORE*/ wire PORE_WIN_MODE = not(win_sig.NOCU_WIN_MODEn);
    /*p26.AXAD*/ wire AXAD_WIN_MODEn = not(PORE_WIN_MODE);
    /*p26.ACEN*/ wire ACEN_BG_MAP_READp = and (POTU_BG_MAP_READp, AXAD_WIN_MODEn);
    /*p25.XEZE*/ wire XEZE_WIN_MAP_READp = and (POTU_BG_MAP_READp, PORE_WIN_MODE);

    /*p27.NETA*/ wire NETA_TILE_READn = and (LENA_BGW_VRAM_RD, NOGU_FETCH_01p);
    /*p26.ASUL*/ wire ASUL_TILE_READp = and (NETA_TILE_READn, AXAD_WIN_MODEn);
    /*p26.BEJE*/ wire BEJE_TILE_READn = not (ASUL_TILE_READp);
    /*p25.XUCY*/ wire XUCY_TILE_READn = nand(NETA_TILE_READn, PORE_WIN_MODE);

    /*p26.BAFY*/ wire BAFY_BG_MAP_READn = not(ACEN_BG_MAP_READp);
    /*p26.AXEP*/ top.VRM_TRI_A00.set_tribuf(BAFY_BG_MAP_READn, BABE_MAP_X0S);
    /*p26.AFEB*/ top.VRM_TRI_A01.set_tribuf(BAFY_BG_MAP_READn, ABOD_MAP_X1S);
    /*p26.ALEL*/ top.VRM_TRI_A02.set_tribuf(BAFY_BG_MAP_READn, BEWY_MAP_X2S);
    /*p26.COLY*/ top.VRM_TRI_A03.set_tribuf(BAFY_BG_MAP_READn, BYCA_MAP_X3S);
    /*p26.AJAN*/ top.VRM_TRI_A04.set_tribuf(BAFY_BG_MAP_READn, ACUL_MAP_X4S);
    /*p26.DUHO*/ top.VRM_TRI_A05.set_tribuf(BAFY_BG_MAP_READn, ETAM_MAP_Y0S);
    /*p26.CASE*/ top.VRM_TRI_A06.set_tribuf(BAFY_BG_MAP_READn, DOTO_MAP_Y1S);
    /*p26.CYPO*/ top.VRM_TRI_A07.set_tribuf(BAFY_BG_MAP_READn, DABA_MAP_Y2S);
    /*p26.CETA*/ top.VRM_TRI_A08.set_tribuf(BAFY_BG_MAP_READn, EFYK_MAP_Y3S);
    /*p26.DAFE*/ top.VRM_TRI_A09.set_tribuf(BAFY_BG_MAP_READn, EJOK_MAP_Y4S);
    /*p26.AMUV*/ top.VRM_TRI_A10.set_tribuf(BAFY_BG_MAP_READn, ppu_config.XAFO_LCDC_BGMAP);
    /*p26.COVE*/ top.VRM_TRI_A11.set_tribuf(BAFY_BG_MAP_READn, VYPO_P10_Bn);
    /*p26.COXO*/ top.VRM_TRI_A12.set_tribuf(BAFY_BG_MAP_READn, VYPO_P10_Bn);

    // Window map read
    /*p25.WUKO*/ wire WUKO_WIN_MAP_READn = not(XEZE_WIN_MAP_READp);
    /*p27.XEJA*/ top.VRM_TRI_A00.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_X3);
    /*p27.XAMO*/ top.VRM_TRI_A01.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_X4);
    /*p27.XAHE*/ top.VRM_TRI_A02.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_X5);
    /*p27.XULO*/ top.VRM_TRI_A03.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_X6);
    /*p27.WUJU*/ top.VRM_TRI_A04.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_X7);
    /*p27.VYTO*/ top.VRM_TRI_A05.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_Y3);
    /*p27.VEHA*/ top.VRM_TRI_A06.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_Y4);
    /*p27.VACE*/ top.VRM_TRI_A07.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_Y5);
    /*p27.VOVO*/ top.VRM_TRI_A08.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_Y6);
    /*p27.VULO*/ top.VRM_TRI_A09.set_tribuf(WUKO_WIN_MAP_READn, win_sig.WIN_Y7);
    /*p27.VEVY*/ top.VRM_TRI_A10.set_tribuf(WUKO_WIN_MAP_READn, ppu_config.WOKY_LCDC_WINMAP);
    /*p27.VEZA*/ top.VRM_TRI_A11.set_tribuf(WUKO_WIN_MAP_READn, VYPO_P10_Bn);
    /*p27.VOGU*/ top.VRM_TRI_A12.set_tribuf(WUKO_WIN_MAP_READn, VYPO_P10_Bn);

    // Background/window tile read

    /*p27.XUHA*/ wire XUHA_FETCH_S2p  = not (NOFU_FETCH_TILE_AB);
    /*p26.ASUM*/ top.VRM_TRI_A00.set_tribuf(BEJE_TILE_READn, XUHA_FETCH_S2p);
    /*p26.EVAD*/ top.VRM_TRI_A01.set_tribuf(BEJE_TILE_READn, FAFO_TILE_Y0S);
    /*p26.DAHU*/ top.VRM_TRI_A02.set_tribuf(BEJE_TILE_READn, EMUX_TILE_Y1S);
    /*p26.DODE*/ top.VRM_TRI_A03.set_tribuf(BEJE_TILE_READn, ECAB_TILE_Y2S);

    /*p25.XONU*/ top.VRM_TRI_A00.set_tribuf(XUCY_TILE_READn, XUHA_FETCH_S2p);
    /*p25.WUDO*/ top.VRM_TRI_A01.set_tribuf(XUCY_TILE_READn, win_sig.WIN_Y0);
    /*p25.WAWE*/ top.VRM_TRI_A02.set_tribuf(XUCY_TILE_READn, win_sig.WIN_Y1);
    /*p25.WOLU*/ top.VRM_TRI_A03.set_tribuf(XUCY_TILE_READn, win_sig.WIN_Y2);

    /*p25.VAPY*/ top.VRM_TRI_A04.set_tribuf(NETA_TILE_READn, BG_PIX_B0);
    /*p25.SEZU*/ top.VRM_TRI_A05.set_tribuf(NETA_TILE_READn, BG_PIX_B1);
    /*p25.VEJY*/ top.VRM_TRI_A06.set_tribuf(NETA_TILE_READn, BG_PIX_B2);
    /*p25.RUSA*/ top.VRM_TRI_A07.set_tribuf(NETA_TILE_READn, BG_PIX_B3);
    /*p25.ROHA*/ top.VRM_TRI_A08.set_tribuf(NETA_TILE_READn, BG_PIX_B4);
    /*p25.RESO*/ top.VRM_TRI_A09.set_tribuf(NETA_TILE_READn, BG_PIX_B5);
    /*p25.SUVO*/ top.VRM_TRI_A10.set_tribuf(NETA_TILE_READn, BG_PIX_B6);
    /*p25.TOBO*/ top.VRM_TRI_A11.set_tribuf(NETA_TILE_READn, BG_PIX_B7);

    /*p25.VUZA*/ wire VUZA_TILE_BANKp = nor(ppu_config.WEXU_LCDC_BGTILE, BG_PIX_B7); // register reused
    /*p25.VURY*/ top.VRM_TRI_A12.set_tribuf(NETA_TILE_READn, VUZA_TILE_BANKp);
  }

  //----------------------------------------

  {
#if 0

  /*p27.MYSO*/ wire MYSO_BG_TRIGp        = and(!ppu_sig.LOBY_RENDERINGn, LAXU_BFETCH_S0, !LYZU_BFETCH_S0_DELAY);
  /*p32.LOMA*/ wire LOMA_LATCH_BG_PIX_Ap = nand(MYSO_BG_TRIGp,  MESU_BFETCH_S1, !NYVA_BFETCH_S2);
  /*p32.LABU*/ wire LABU_LATCH_BG_PIX_Bp = nand(MYSO_BG_TRIGp, !MESU_BFETCH_S1);

#endif

    /*p24.LOBY*/ wire LOBY_RENDERINGn      = not(ppu_sig.XYMU_RENDERINGp);
    /*p27.LAXE*/ wire LAXE_BFETCH_S0n      = not(LAXU_BFETCH_S0.q());
    /*p27.NAKO*/ wire NAKO_BFETCH_S1n      = not(MESU_BFETCH_S1.q());
    /*p27.NOFU*/ wire NOFU_BFETCH_S2n      = not(NYVA_BFETCH_S2.q());
    /*p27.MYSO*/ wire MYSO_BG_TRIGp        = nor(LOBY_RENDERINGn, LAXE_BFETCH_S0n, LYZU_BFETCH_S0_DELAY.q());
    /*p27.NYDY*/ wire NYDY_LATCH_BG_PIX_Ap = nand(MYSO_BG_TRIGp, MESU_BFETCH_S1.q(), NOFU_BFETCH_S2n);
    /*p27.MOFU*/ wire MOFU_LATCH_BG_PIX_Bn = and (MYSO_BG_TRIGp, NAKO_BFETCH_S1n);

    /*p32.METE*/ wire METE_LATCH_BG_PIX_An = not(NYDY_LATCH_BG_PIX_Ap);
    /*p32.LOMA*/ wire LOMA_LATCH_BG_PIX_Ap = not(METE_LATCH_BG_PIX_An);
    /*p32.LESO*/ wire LESO_LATCH_BG_PIX_Bp = not(MOFU_LATCH_BG_PIX_Bn);
    /*p32.AJAR*/ wire AJAR_LATCH_BG_PIX_Bn = not(LESO_LATCH_BG_PIX_Bp);
    /*p32.LABU*/ wire LABU_LATCH_BG_PIX_Bp = not(AJAR_LATCH_BG_PIX_Bn);

    // Clock polarity wrong? Check regs on die.

    // BFETCH_000 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_001 - LABU_BG_PIX_B_CLKp = or(!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q())
    // BFETCH_010 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_011 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_100 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_101 - LABU_BG_PIX_B_CLKp = or(!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q())

    // BFETCH_000 - LOMA_BG_LATCHn = 1;
    // BFETCH_001 - LOMA_BG_LATCHn = 1;
    // BFETCH_010 - LOMA_BG_LATCHn = 1;
    // BFETCH_011 - LOMA_BG_LATCHn = or(!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q());
    // BFETCH_100 - LOMA_BG_LATCHn = 1;
    // BFETCH_101 - LOMA_BG_LATCHn = 1;

    /*p32.LEGU*/ BG_PIX_A0.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D0);
    /*p32.NUDU*/ BG_PIX_A1.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D1);
    /*p32.MUKU*/ BG_PIX_A2.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D2);
    /*p32.LUZO*/ BG_PIX_A3.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D3);
    /*p32.MEGU*/ BG_PIX_A4.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D4);
    /*p32.MYJY*/ BG_PIX_A5.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D5);
    /*p32.NASA*/ BG_PIX_A6.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D6);
    /*p32.NEFO*/ BG_PIX_A7.set(LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D7);

    wire P10_B = 0;
    /*p27.VYPO*/ wire VYPO_P10_Bn = not(P10_B);

    /*p32.RAWU*/ BG_PIX_B0.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D0);
    /*p32.POZO*/ BG_PIX_B1.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D1);
    /*p32.PYZO*/ BG_PIX_B2.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D2);
    /*p32.POXA*/ BG_PIX_B3.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D3);
    /*p32.PULO*/ BG_PIX_B4.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D4);
    /*p32.POJU*/ BG_PIX_B5.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D5);
    /*p32.POWY*/ BG_PIX_B6.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D6);
    /*p32.PYJU*/ BG_PIX_B7.set(LABU_LATCH_BG_PIX_Bp, VYPO_P10_Bn, top.VRM_TRI_D7);
  }
}

//------------------------------------------------------------------------------

SignalHash TileFetcher::commit() {
  SignalHash hash;

  hash << PYGO_FETCH_DONE_Cp.commit_reg();
  hash << POKY_PORCH_DONEp.commit_latch();

  hash << LAXU_BFETCH_S0.commit_reg();
  hash << MESU_BFETCH_S1.commit_reg();
  hash << NYVA_BFETCH_S2.commit_reg();

  hash << LOVY_FETCH_DONEp.commit_reg();
  hash << LONY_BG_READ_VRAM_LATCHp.commit_latch();

  hash << NYKA_FETCH_DONE_Ap.commit_reg();
  hash << PORY_FETCH_DONE_Bp.commit_reg();
  hash << LYZU_BFETCH_S0_DELAY.commit_reg();

  hash << BG_PIX_A0.commit_reg();
  hash << BG_PIX_A1.commit_reg();
  hash << BG_PIX_A2.commit_reg();
  hash << BG_PIX_A3.commit_reg();
  hash << BG_PIX_A4.commit_reg();
  hash << BG_PIX_A5.commit_reg();
  hash << BG_PIX_A6.commit_reg();
  hash << BG_PIX_A7.commit_reg();

  hash << BG_PIX_B0.commit_reg();
  hash << BG_PIX_B1.commit_reg();
  hash << BG_PIX_B2.commit_reg(); 
  hash << BG_PIX_B3.commit_reg(); 
  hash << BG_PIX_B4.commit_reg(); 
  hash << BG_PIX_B5.commit_reg(); 
  hash << BG_PIX_B6.commit_reg(); 
  hash << BG_PIX_B7.commit_reg();

  return hash;
}

//------------------------------------------------------------------------------
