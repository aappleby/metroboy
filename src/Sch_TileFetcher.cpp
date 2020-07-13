#include "Sch_TileFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TileFetcher::tick(SchematicTop& top) {
  _XYMU_RENDERINGp = top.ppu_reg.XYMU_RENDERINGp();
  _NYXU_TILE_FETCHER_RSTn = top.NYXU_TILE_FETCHER_RSTn();
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top) {
  /*p27.LAXE*/ wire _LAXE_BFETCH_S0n = not(_LAXU_BFETCH_S0.q());
  /*p27.NAKO*/ wire _NAKO_BFETCH_S1n = not(_MESU_BFETCH_S1.q());
  /*p27.NOFU*/ wire _NOFU_BFETCH_S2n = not(_NYVA_BFETCH_S2.q());

  {
    /*p27.LURY*/ wire _LURY_BG_READ_VRAM_LATCH_RSTn = and(_LOVY_FETCH_DONEp.qn(), top.ppu_reg.XYMU_RENDERINGp());
    /*p27.LONY*/ _LONY_BG_READ_VRAM_LATCHp.nand_latch(top.NYXU_TILE_FETCHER_RSTn(), _LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  {
    /*p27.LOVY*/ _LOVY_FETCH_DONEp.set(top.clk_reg.MYVO_AxCxExGx(), top.NYXU_TILE_FETCHER_RSTn(), top.tile_fetcher.LYRY_BFETCH_DONEp());

    /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand(top.clk_reg.ALET_xBxDxFxH(), top.tile_fetcher.MOCE_BFETCH_DONEn());
    /*p27.LAXU*/ _LAXU_BFETCH_S0.set(_LEBO_AxCxExGx,       top.NYXU_TILE_FETCHER_RSTn(), _LAXU_BFETCH_S0.qn());
    /*p27.MESU*/ _MESU_BFETCH_S1.set(_LAXU_BFETCH_S0.qn(), top.NYXU_TILE_FETCHER_RSTn(), _MESU_BFETCH_S1.qn());
    /*p27.NYVA*/ _NYVA_BFETCH_S2.set(_MESU_BFETCH_S1.qn(), top.NYXU_TILE_FETCHER_RSTn(), _NYVA_BFETCH_S2.qn());

    /*p27.LYZU*/ _LYZU_BFETCH_S0_DELAY.set (top.clk_reg.ALET_xBxDxFxH(), top.ppu_reg.XYMU_RENDERINGp(), _LAXU_BFETCH_S0.q());
  }

  {
    /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(top.win_reg.MOSU_WIN_MODE_TRIGp(), top.ppu_reg.LOBY_RENDERINGn());

    /*p24.NYKA*/ _NYKA_TILE_FETCH_DONE_Ap.set(top.clk_reg.ALET_xBxDxFxH(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, top.tile_fetcher.LYRY_BFETCH_DONEp());
    /*p24.PORY*/ _PORY_TILE_FETCH_DONE_Bp.set(top.clk_reg.MYVO_AxCxExGx(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, _NYKA_TILE_FETCH_DONE_Ap.q());
    /*p24.PYGO*/ _PYGO_FETCH_DONE_Cp.     set(top.clk_reg.ALET_xBxDxFxH(), _XYMU_RENDERINGp,           _PORY_TILE_FETCH_DONE_Bp.q());

    /*p24.POKY*/ _POKY_PORCH_DONEp.nor_latch(_PYGO_FETCH_DONE_Cp.q(), top.ppu_reg.LOBY_RENDERINGn());
  }

  //----------------------------------------

  {
    /*p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0.q(), _GAVE_SCY0.q(), 0);
    /*p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0.q(), _GAVE_SCY0.q(), 0);
    /*p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1.q(), _FYMO_SCY1.q(), _FAFO_TILE_Y0C);
    /*p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1.q(), _FYMO_SCY1.q(), _FAFO_TILE_Y0C);
    /*p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2.q(), _FEZU_SCY2.q(), _EMUX_TILE_Y1C);
    /*p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2.q(), _FEZU_SCY2.q(), _EMUX_TILE_Y1C);
    /*p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3.q(), _FUJO_SCY3.q(), _ECAB_TILE_Y2C);
    /*p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3.q(), _FUJO_SCY3.q(), _ECAB_TILE_Y2C);
    /*p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4.q(), _DEDE_SCY4.q(), _ETAM_MAP_Y0C);
    /*p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4.q(), _DEDE_SCY4.q(), _ETAM_MAP_Y0C);
    /*p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5.q(), _FOTY_SCY5.q(), _DOTO_MAP_Y1C);
    /*p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5.q(), _FOTY_SCY5.q(), _DOTO_MAP_Y1C);
    /*p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6.q(), _FOHA_SCY6.q(), _DABA_MAP_Y2C);
    /*p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6.q(), _FOHA_SCY6.q(), _DABA_MAP_Y2C);
    /*p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7.q(), _FUNY_SCY7.q(), _EFYK_MAP_Y3C);
    /*p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7.q(), _FUNY_SCY7.q(), _EFYK_MAP_Y3C);

    (void)_EJOK_MAP_Y4C;

    /*p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(top.ppu_reg.XEHO_X0.q(), _DATY_SCX0.q(), 0);
    /*p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(top.ppu_reg.XEHO_X0.q(), _DATY_SCX0.q(), 0);
    /*p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(top.ppu_reg.SAVY_X1.q(), _DUZU_SCX1.q(), _ATAD_TILE_X0C);
    /*p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(top.ppu_reg.SAVY_X1.q(), _DUZU_SCX1.q(), _ATAD_TILE_X0C);
    /*p26.APYH*/ wire _APYH_TILE_X2S = add_s(top.ppu_reg.XODU_X2.q(), _CYXU_SCX2.q(), _BEHU_TILE_X1C);
    /*p26.APYH*/ wire _APYH_TILE_X2C = add_c(top.ppu_reg.XODU_X2.q(), _CYXU_SCX2.q(), _BEHU_TILE_X1C);
    /*p26.BABE*/ wire _BABE_MAP_X0S  = add_s(top.ppu_reg.XYDO_X3.q(), _GUBO_SCX3.q(), _APYH_TILE_X2C);
    /*p26.BABE*/ wire _BABE_MAP_X0C  = add_c(top.ppu_reg.XYDO_X3.q(), _GUBO_SCX3.q(), _APYH_TILE_X2C);
    /*p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(top.ppu_reg.TUHU_X4.q(), _BEMY_SCX4.q(), _BABE_MAP_X0C);
    /*p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(top.ppu_reg.TUHU_X4.q(), _BEMY_SCX4.q(), _BABE_MAP_X0C);
    /*p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(top.ppu_reg.TUKY_X5.q(), _CUZY_SCX5.q(), _ABOD_MAP_X1C);
    /*p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(top.ppu_reg.TUKY_X5.q(), _CUZY_SCX5.q(), _ABOD_MAP_X1C);
    /*p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(top.ppu_reg.TAKO_X6.q(), _CABU_SCX6.q(), _BEWY_MAP_X2C);
    /*p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(top.ppu_reg.TAKO_X6.q(), _CABU_SCX6.q(), _BEWY_MAP_X2C);
    /*p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(top.ppu_reg.SYBE_X7.q(), _BAKE_SCX7.q(), _BYCA_MAP_X3C);
    /*p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(top.ppu_reg.SYBE_X7.q(), _BAKE_SCX7.q(), _BYCA_MAP_X3C);

    (void)_ATAD_TILE_X0S;
    (void)_BEHU_TILE_X1S;
    (void)_APYH_TILE_X2S;
    (void)_ACUL_MAP_X4C;

    // Background map read

    /*p27.NOGU*/ wire _NOGU_FETCH_01p = nand(_NAKO_BFETCH_S1n, _NOFU_BFETCH_S2n);
    /*p27.NENY*/ wire _NENY_FETCH_01n = not(_NOGU_FETCH_01p);
    /*p27.POTU*/ wire _POTU_BG_MAP_READp = and (LENA_BGW_VRM_RDp(), _NENY_FETCH_01n);
    /*p26.AXAD*/ wire _AXAD_WIN_MODEn = not(top.win_reg.PORE_WIN_MODEp());


    {
      /*p26.ACEN*/ wire _ACEN_BG_MAP_READp = and (_POTU_BG_MAP_READp, _AXAD_WIN_MODEn);
      /*p26.BAFY*/ wire _BAFY_BG_MAP_READn = not(_ACEN_BG_MAP_READp);
      /*p26.AXEP*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(_BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /*p26.AFEB*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(_BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /*p26.ALEL*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(_BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /*p26.COLY*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(_BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /*p26.AJAN*/ top.vram_bus.VRM_TRI_A04.set_tribuf_6n(_BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /*p26.DUHO*/ top.vram_bus.VRM_TRI_A05.set_tribuf_6n(_BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /*p26.CASE*/ top.vram_bus.VRM_TRI_A06.set_tribuf_6n(_BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /*p26.CYPO*/ top.vram_bus.VRM_TRI_A07.set_tribuf_6n(_BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /*p26.CETA*/ top.vram_bus.VRM_TRI_A08.set_tribuf_6n(_BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /*p26.DAFE*/ top.vram_bus.VRM_TRI_A09.set_tribuf_6n(_BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*p26.AMUV*/ top.vram_bus.VRM_TRI_A10.set_tribuf_6n(_BAFY_BG_MAP_READn, top.XAFO_LCDC_BGMAP.q());
      /*p26.COVE*/ top.vram_bus.VRM_TRI_A11.set_tribuf_6n(_BAFY_BG_MAP_READn, top.VYPO_GND);
      /*p26.COXO*/ top.vram_bus.VRM_TRI_A12.set_tribuf_6n(_BAFY_BG_MAP_READn, top.VYPO_GND);
    }

    {
      // Window map read
      /*p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and (_POTU_BG_MAP_READp, top.win_reg.PORE_WIN_MODEp());
      /*p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not(_XEZE_WIN_MAP_READp);
      /*p27.XEJA*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.WYKA_WIN_X3());
      /*p27.XAMO*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.WODY_WIN_X4());
      /*p27.XAHE*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.WOBO_WIN_X5());
      /*p27.XULO*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.WYKO_WIN_X6());
      /*p27.WUJU*/ top.vram_bus.VRM_TRI_A04.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.XOLO_WIN_X7());
      /*p27.VYTO*/ top.vram_bus.VRM_TRI_A05.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.TUFU_WIN_Y3());
      /*p27.VEHA*/ top.vram_bus.VRM_TRI_A06.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.TAXA_WIN_Y4());
      /*p27.VACE*/ top.vram_bus.VRM_TRI_A07.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.TOZO_WIN_Y5());
      /*p27.VOVO*/ top.vram_bus.VRM_TRI_A08.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.TATE_WIN_Y6());
      /*p27.VULO*/ top.vram_bus.VRM_TRI_A09.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.win_reg.TEKE_WIN_Y7());
      /*p27.VEVY*/ top.vram_bus.VRM_TRI_A10.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.WOKY_LCDC_WINMAP.q());
      /*p27.VEZA*/ top.vram_bus.VRM_TRI_A11.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.VYPO_GND);
      /*p27.VOGU*/ top.vram_bus.VRM_TRI_A12.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.VYPO_GND);
    }

    // Background/window tile read

    /*p27.XUHA*/ wire _XUHA_FETCH_S2p  = not (_NOFU_BFETCH_S2n);
    /*p27.NETA*/ wire _NETA_TILE_READp = and (LENA_BGW_VRM_RDp(), _NOGU_FETCH_01p);
      
    {
      /*p26.ASUL*/ wire ASUL_TILE_READp = and (_NETA_TILE_READp, _AXAD_WIN_MODEn);
      /*p26.BEJE*/ wire _BEJE_BGD_TILE_READn = not (ASUL_TILE_READp);
      /*p26.ASUM*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(_BEJE_BGD_TILE_READn, _XUHA_FETCH_S2p);
      /*p26.EVAD*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(_BEJE_BGD_TILE_READn, _FAFO_TILE_Y0S);
      /*p26.DAHU*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(_BEJE_BGD_TILE_READn, _EMUX_TILE_Y1S);
      /*p26.DODE*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(_BEJE_BGD_TILE_READn, _ECAB_TILE_Y2S);
    }

    {
      /*p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand(_NETA_TILE_READp, top.win_reg.PORE_WIN_MODEp());
      /*p25.XONU*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(_XUCY_WIN_TILE_READn, _XUHA_FETCH_S2p);
      /*p25.WUDO*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.win_reg.VYNO_WIN_Y0());
      /*p25.WAWE*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.win_reg.VUJO_WIN_Y1());
      /*p25.WOLU*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.win_reg.VYMU_WIN_Y2());
    }

    // FIXME P? wat?
    /*p25.VAPY*/ top.vram_bus.VRM_TRI_A04.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.RAWU_BG_PIX_B0.q());
    /*p25.SEZU*/ top.vram_bus.VRM_TRI_A05.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.POZO_BG_PIX_B1.q());
    /*p25.VEJY*/ top.vram_bus.VRM_TRI_A06.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.PYZO_BG_PIX_B2.q());
    /*p25.RUSA*/ top.vram_bus.VRM_TRI_A07.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.POXA_BG_PIX_B3.q());
    /*p25.ROHA*/ top.vram_bus.VRM_TRI_A08.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.PULO_BG_PIX_B4.q());
    /*p25.RESO*/ top.vram_bus.VRM_TRI_A09.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.POJU_BG_PIX_B5.q());
    /*p25.SUVO*/ top.vram_bus.VRM_TRI_A10.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.POWY_BG_PIX_B6.q());
    /*p25.TOBO*/ top.vram_bus.VRM_TRI_A11.set_tribuf_6p(_NETA_TILE_READp, top.tile_fetcher.PYJU_BG_PIX_B7.q());

    /*p25.VUZA*/ wire _VUZA_TILE_BANKp = nor(top.WEXU_LCDC_BGTILE.q(), top.tile_fetcher.PYJU_BG_PIX_B7.q()); // register reused
    /*p25.VURY*/ top.vram_bus.VRM_TRI_A12.set_tribuf_6p(_NETA_TILE_READp, _VUZA_TILE_BANKp);
  }

  {
    /*p27.MYSO*/ wire _MYSO_BG_TRIGp        = nor(top.ppu_reg.LOBY_RENDERINGn(), _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0_DELAY.q());

    /*p27.NYDY*/ wire _NYDY_LATCH_BG_PIX_Ap = nand(_MYSO_BG_TRIGp, _MESU_BFETCH_S1.q(), _NOFU_BFETCH_S2n);
    /*p32.METE*/ wire _METE_LATCH_BG_PIX_An = not(_NYDY_LATCH_BG_PIX_Ap);
    /*p32.LOMA*/ wire _LOMA_LATCH_BG_PIX_Ap = not(_METE_LATCH_BG_PIX_An);

    /*p32.LEGU*/ LEGU_BG_PIX_A0.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D0.q());
    /*p32.NUDU*/ NUDU_BG_PIX_A1.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D1.q());
    /*p32.MUKU*/ MUKU_BG_PIX_A2.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D2.q());
    /*p32.LUZO*/ LUZO_BG_PIX_A3.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D3.q());
    /*p32.MEGU*/ MEGU_BG_PIX_A4.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D4.q());
    /*p32.MYJY*/ MYJY_BG_PIX_A5.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D5.q());
    /*p32.NASA*/ NASA_BG_PIX_A6.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D6.q());
    /*p32.NEFO*/ NEFO_BG_PIX_A7.set(_LOMA_LATCH_BG_PIX_Ap, !_LOMA_LATCH_BG_PIX_Ap, top.vram_bus.VRM_TRI_D7.q());

    /*p27.MOFU*/ wire _MOFU_LATCH_BG_PIX_Bn = and(_MYSO_BG_TRIGp, _NAKO_BFETCH_S1n);
    /*p32.LESO*/ wire _LESO_LATCH_BG_PIX_Bp = not(_MOFU_LATCH_BG_PIX_Bn);
    /*p??.LUVE*/ wire _LUVE_MATCH_BG_PIX_Bn = not(_LESO_LATCH_BG_PIX_Bp); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire _LABU_LATCH_BG_PIX_Bp = not(_LUVE_MATCH_BG_PIX_Bn);

    // Why are the clocks swapped?
    /*p32.RAWU*/ RAWU_BG_PIX_B0.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D0.q());
    /*p32.POZO*/ POZO_BG_PIX_B1.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D1.q());
    /*p32.PYZO*/ PYZO_BG_PIX_B2.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D2.q());
    /*p32.POXA*/ POXA_BG_PIX_B3.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D3.q());
    /*p32.PULO*/ PULO_BG_PIX_B4.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D4.q());
    /*p32.POJU*/ POJU_BG_PIX_B5.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D5.q());
    /*p32.POWY*/ POWY_BG_PIX_B6.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D6.q());
    /*p32.PYJU*/ PYJU_BG_PIX_B7.set(_LABU_LATCH_BG_PIX_Bp, _LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.vram_bus.VRM_TRI_D7.q());
  }


  // FF42 SCY
  {
    /*p22.WEBU*/ wire _WEBU_FF42n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.XOLA_A00n(), top.int_bus.WESA_A01p(), top.int_bus.XUSY_A02n(), top.int_bus.XERA_A03n());
    /*p22.XARO*/ wire _XARO_FF42p = not(_WEBU_FF42n);

    /*p23.ANYP*/ wire _ANYP_FF42_RDp = and(_XARO_FF42p, top.ASOT_CPU_RDp());
    /*p23.BUWY*/ wire _BUWY_FF42_RDn = not(_ANYP_FF42_RDp);

    /*p23.BEDY*/ wire _BEDY_FF42_WRp = and(_XARO_FF42p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.CAVO*/ wire _CAVO_FF42_WRn = not(_BEDY_FF42_WRp);

    /*p23.GAVE*/ _GAVE_SCY0.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D0.q());
    /*p23.FYMO*/ _FYMO_SCY1.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D1.q());
    /*p23.FEZU*/ _FEZU_SCY2.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D2.q());
    /*p23.FUJO*/ _FUJO_SCY3.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D3.q());
    /*p23.DEDE*/ _DEDE_SCY4.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D4.q());
    /*p23.FOTY*/ _FOTY_SCY5.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D5.q());
    /*p23.FOHA*/ _FOHA_SCY6.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D6.q());
    /*p23.FUNY*/ _FUNY_SCY7.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D7.q());

    /*p23.WARE*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_BUWY_FF42_RDn, _GAVE_SCY0.q());
    /*p23.GOBA*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_BUWY_FF42_RDn, _FYMO_SCY1.q());
    /*p23.GONU*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_BUWY_FF42_RDn, _FEZU_SCY2.q());
    /*p23.GODO*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_BUWY_FF42_RDn, _FUJO_SCY3.q());
    /*p23.CUSA*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_BUWY_FF42_RDn, _DEDE_SCY4.q());
    /*p23.GYZO*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_BUWY_FF42_RDn, _FOTY_SCY5.q());
    /*p23.GUNE*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_BUWY_FF42_RDn, _FOHA_SCY6.q());
    /*p23.GYZA*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_BUWY_FF42_RDn, _FUNY_SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire _WAVU_FF43n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.WADO_A00p(), top.int_bus.WESA_A01p(), top.int_bus.XUSY_A02n(), top.int_bus.XERA_A03n());
    /*p22.XAVY*/ wire _XAVY_FF43p = not(_WAVU_FF43n);

    /*p23.AVOG*/ wire _AVOG_FF43_RDp = and (_XAVY_FF43p, top.ASOT_CPU_RDp());
    /*p23.BEBA*/ wire _BEBA_FF43_RDn = not(_AVOG_FF43_RDp);

    /*p23.ARUR*/ wire _ARUR_FF43_WRp = and (_XAVY_FF43p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.AMUN*/ wire _AMUN_FF43_WRn = not(_ARUR_FF43_WRp);

    /*p23.DATY*/ _DATY_SCX0.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D0.q());
    /*p23.DUZU*/ _DUZU_SCX1.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D1.q());
    /*p23.CYXU*/ _CYXU_SCX2.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D2.q());
    /*p23.GUBO*/ _GUBO_SCX3.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D3.q());
    /*p23.BEMY*/ _BEMY_SCX4.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D4.q());
    /*p23.CUZY*/ _CUZY_SCX5.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D5.q());
    /*p23.CABU*/ _CABU_SCX6.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D6.q());
    /*p23.BAKE*/ _BAKE_SCX7.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.rst_reg.CUNU_SYS_RSTn(), top.int_bus.INT_TRI_D7.q());

    /*p23.EDOS*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_BEBA_FF43_RDn, _DATY_SCX0.q());
    /*p23.EKOB*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_BEBA_FF43_RDn, _DUZU_SCX1.q());
    /*p23.CUGA*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_BEBA_FF43_RDn, _CYXU_SCX2.q());
    /*p23.WONY*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_BEBA_FF43_RDn, _GUBO_SCX3.q());
    /*p23.CEDU*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_BEBA_FF43_RDn, _BEMY_SCX4.q());
    /*p23.CATA*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_BEBA_FF43_RDn, _CUZY_SCX5.q());
    /*p23.DOXE*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_BEBA_FF43_RDn, _CABU_SCX6.q());
    /*p23.CASY*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_BEBA_FF43_RDn, _BAKE_SCX7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash TileFetcher::commit(SchematicTop& /*top*/) {
  SignalHash hash;

  hash << LEGU_BG_PIX_A0.commit();
  hash << NUDU_BG_PIX_A1.commit();
  hash << MUKU_BG_PIX_A2.commit();
  hash << LUZO_BG_PIX_A3.commit();
  hash << MEGU_BG_PIX_A4.commit();
  hash << MYJY_BG_PIX_A5.commit();
  hash << NASA_BG_PIX_A6.commit();
  hash << NEFO_BG_PIX_A7.commit();

  hash << RAWU_BG_PIX_B0.commit();
  hash << POZO_BG_PIX_B1.commit();
  hash << PYZO_BG_PIX_B2.commit(); 
  hash << POXA_BG_PIX_B3.commit(); 
  hash << PULO_BG_PIX_B4.commit(); 
  hash << POJU_BG_PIX_B5.commit(); 
  hash << POWY_BG_PIX_B6.commit(); 
  hash << PYJU_BG_PIX_B7.commit();

  hash << _XYMU_RENDERINGp.commit();
  hash << _NYXU_TILE_FETCHER_RSTn.commit();

  hash << _PYGO_FETCH_DONE_Cp.commit();
  hash << _POKY_PORCH_DONEp.commit();

  hash << _LAXU_BFETCH_S0.commit();
  hash << _MESU_BFETCH_S1.commit();
  hash << _NYVA_BFETCH_S2.commit();

  hash << _LOVY_FETCH_DONEp.commit();
  hash << _LONY_BG_READ_VRAM_LATCHp.commit();

  hash << _NYKA_TILE_FETCH_DONE_Ap.commit();
  hash << _PORY_TILE_FETCH_DONE_Bp.commit();
  hash << _LYZU_BFETCH_S0_DELAY.commit();

  hash << _GAVE_SCY0.commit();
  hash << _FYMO_SCY1.commit();
  hash << _FEZU_SCY2.commit();
  hash << _FUJO_SCY3.commit();
  hash << _DEDE_SCY4.commit();
  hash << _FOTY_SCY5.commit();
  hash << _FOHA_SCY6.commit();
  hash << _FUNY_SCY7.commit();

  hash << _DATY_SCX0.commit();
  hash << _DUZU_SCX1.commit();
  hash << _CYXU_SCX2.commit();
  hash << _GUBO_SCX3.commit();
  hash << _BEMY_SCX4.commit();
  hash << _CUZY_SCX5.commit();
  hash << _CABU_SCX6.commit();
  hash << _BAKE_SCX7.commit();

  return hash;
}

//------------------------------------------------------------------------------
