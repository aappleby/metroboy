#include "Sch_TileFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TileFetcher::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top) {
  {
    /*p27.LURY*/ wire LURY_BG_READ_VRAM_LATCH_RSTn = and(LOVY_FETCH_DONEp.qn(), top.XYMU_RENDERINGp());
    /*p27.LONY*/ LONY_BG_READ_VRAM_LATCHp.nand_latch(top.NYXU_TILE_FETCHER_RSTn(), LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  {
    /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0.q(), NYVA_BFETCH_S2.q(), top.NYXU_TILE_FETCHER_RSTn());
    /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);
    /*p27.LOVY*/ LOVY_FETCH_DONEp.set(top.MYVO_AxCxExGx(), top.NYXU_TILE_FETCHER_RSTn(), LYRY_BFETCH_DONEp);

    /*p27.LEBO*/ wire LEBO_AxCxExGx = nand(top.ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*p27.LAXU*/ LAXU_BFETCH_S0.set(LEBO_AxCxExGx,       top.NYXU_TILE_FETCHER_RSTn(), LAXU_BFETCH_S0.qn());
    /*p27.MESU*/ MESU_BFETCH_S1.set(LAXU_BFETCH_S0.qn(), top.NYXU_TILE_FETCHER_RSTn(), MESU_BFETCH_S1.qn());
    /*p27.NYVA*/ NYVA_BFETCH_S2.set(MESU_BFETCH_S1.qn(), top.NYXU_TILE_FETCHER_RSTn(), NYVA_BFETCH_S2.qn());

    /*p27.LYZU*/ LYZU_BFETCH_S0_DELAY.set (top.ALET_xBxDxFxH(), top.XYMU_RENDERINGp(), LAXU_BFETCH_S0.q());
  }

  {
    /*p24.LOBY*/ wire LOBY_RENDERINGn = not(top.XYMU_RENDERINGp());
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(top.NUNY_WX_MATCHpe());
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);
    /*p24.NAFY*/ wire NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);
    /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0.q(), NYVA_BFETCH_S2.q(), top.NYXU_TILE_FETCHER_RSTn());
    /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);

    /*p24.NYKA*/ NYKA_TILE_FETCH_DONE_Ap.set(top.ALET_xBxDxFxH(), NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
    /*p24.PORY*/ PORY_TILE_FETCH_DONE_Bp.set(top.MYVO_AxCxExGx(), NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_TILE_FETCH_DONE_Ap.q());
    /*p24.PYGO*/ PYGO_FETCH_DONE_Cp.     set(top.ALET_xBxDxFxH(), top.XYMU_RENDERINGp(),           PORY_TILE_FETCH_DONE_Bp.q());

    /*p24.POKY*/ POKY_PORCH_DONEp.nor_latch(PYGO_FETCH_DONE_Cp.q(), LOBY_RENDERINGn);
  }

  //----------------------------------------

  {
    /*p26.FAFO*/ wire FAFO_TILE_Y0S = add_s(top.MUWY_Y0(), GAVE_SCY0.q(), 0);
    /*p26.FAFO*/ wire FAFO_TILE_Y0C = add_c(top.MUWY_Y0(), GAVE_SCY0.q(), 0);
    /*p26.EMUX*/ wire EMUX_TILE_Y1S = add_s(top.MYRO_Y1(), FYMO_SCY1.q(), FAFO_TILE_Y0C);
    /*p26.EMUX*/ wire EMUX_TILE_Y1C = add_c(top.MYRO_Y1(), FYMO_SCY1.q(), FAFO_TILE_Y0C);
    /*p26.ECAB*/ wire ECAB_TILE_Y2S = add_s(top.LEXA_Y2(), FEZU_SCY2.q(), EMUX_TILE_Y1C);
    /*p26.ECAB*/ wire ECAB_TILE_Y2C = add_c(top.LEXA_Y2(), FEZU_SCY2.q(), EMUX_TILE_Y1C);
    /*p26.ETAM*/ wire ETAM_MAP_Y0S  = add_s(top.LYDO_Y3(), FUJO_SCY3.q(), ECAB_TILE_Y2C);
    /*p26.ETAM*/ wire ETAM_MAP_Y0C  = add_c(top.LYDO_Y3(), FUJO_SCY3.q(), ECAB_TILE_Y2C);
    /*p26.DOTO*/ wire DOTO_MAP_Y1S  = add_s(top.LOVU_Y4(), DEDE_SCY4.q(), ETAM_MAP_Y0C);
    /*p26.DOTO*/ wire DOTO_MAP_Y1C  = add_c(top.LOVU_Y4(), DEDE_SCY4.q(), ETAM_MAP_Y0C);
    /*p26.DABA*/ wire DABA_MAP_Y2S  = add_s(top.LEMA_Y5(), FOTY_SCY5.q(), DOTO_MAP_Y1C);
    /*p26.DABA*/ wire DABA_MAP_Y2C  = add_c(top.LEMA_Y5(), FOTY_SCY5.q(), DOTO_MAP_Y1C);
    /*p26.EFYK*/ wire EFYK_MAP_Y3S  = add_s(top.MATO_Y6(), FOHA_SCY6.q(), DABA_MAP_Y2C);
    /*p26.EFYK*/ wire EFYK_MAP_Y3C  = add_c(top.MATO_Y6(), FOHA_SCY6.q(), DABA_MAP_Y2C);
    /*p26.EJOK*/ wire EJOK_MAP_Y4S  = add_s(top.LAFO_Y7(), FUNY_SCY7.q(), EFYK_MAP_Y3C);
    /*p26.EJOK*/ wire EJOK_MAP_Y4C  = add_c(top.LAFO_Y7(), FUNY_SCY7.q(), EFYK_MAP_Y3C);

    (void)EJOK_MAP_Y4C;

    /*p26.ATAD*/ wire ATAD_TILE_X0S = add_s(top.XEHO_X0(), DATY_SCX0.q(), 0);
    /*p26.ATAD*/ wire ATAD_TILE_X0C = add_c(top.XEHO_X0(), DATY_SCX0.q(), 0);
    /*p26.BEHU*/ wire BEHU_TILE_X1S = add_s(top.SAVY_X1(), DUZU_SCX1.q(), ATAD_TILE_X0C);
    /*p26.BEHU*/ wire BEHU_TILE_X1C = add_c(top.SAVY_X1(), DUZU_SCX1.q(), ATAD_TILE_X0C);
    /*p26.APYH*/ wire APYH_TILE_X2S = add_s(top.XODU_X2(), CYXU_SCX2.q(), BEHU_TILE_X1C);
    /*p26.APYH*/ wire APYH_TILE_X2C = add_c(top.XODU_X2(), CYXU_SCX2.q(), BEHU_TILE_X1C);
    /*p26.BABE*/ wire BABE_MAP_X0S  = add_s(top.XYDO_X3(), GUBO_SCX3.q(), APYH_TILE_X2C);
    /*p26.BABE*/ wire BABE_MAP_X0C  = add_c(top.XYDO_X3(), GUBO_SCX3.q(), APYH_TILE_X2C);
    /*p26.ABOD*/ wire ABOD_MAP_X1S  = add_s(top.TUHU_X4(), BEMY_SCX4.q(), BABE_MAP_X0C);
    /*p26.ABOD*/ wire ABOD_MAP_X1C  = add_c(top.TUHU_X4(), BEMY_SCX4.q(), BABE_MAP_X0C);
    /*p26.BEWY*/ wire BEWY_MAP_X2S  = add_s(top.TUKY_X5(), CUZY_SCX5.q(), ABOD_MAP_X1C);
    /*p26.BEWY*/ wire BEWY_MAP_X2C  = add_c(top.TUKY_X5(), CUZY_SCX5.q(), ABOD_MAP_X1C);
    /*p26.BYCA*/ wire BYCA_MAP_X3S  = add_s(top.TAKO_X6(), CABU_SCX6.q(), BEWY_MAP_X2C);
    /*p26.BYCA*/ wire BYCA_MAP_X3C  = add_c(top.TAKO_X6(), CABU_SCX6.q(), BEWY_MAP_X2C);
    /*p26.ACUL*/ wire ACUL_MAP_X4S  = add_s(top.SYBE_X7(), BAKE_SCX7.q(), BYCA_MAP_X3C);
    /*p26.ACUL*/ wire ACUL_MAP_X4C  = add_c(top.SYBE_X7(), BAKE_SCX7.q(), BYCA_MAP_X3C);

    (void)ATAD_TILE_X0S;
    (void)BEHU_TILE_X1S;
    (void)APYH_TILE_X2S;
    (void)ACUL_MAP_X4C;

    // Background map read

    /*p27.NAKO*/ wire NAKO_FETCH_S1n = not(MESU_BFETCH_S1.q());
    /*p27.NOFU*/ wire NOFU_FETCH_TILE_AB = not(NYVA_BFETCH_S2.q());
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp.q());
    /*p27.LENA*/ wire LENA_BGW_VRAM_RDp = not(LUSU_BGW_VRAM_RDn);
    /*p27.NOGU*/ wire NOGU_FETCH_01p = nand(NAKO_FETCH_S1n, NOFU_FETCH_TILE_AB);
    /*p27.NENY*/ wire NENY_FETCH_01n = not(NOGU_FETCH_01p);
    /*p27.POTU*/ wire POTU_BG_MAP_READp = and (LENA_BGW_VRAM_RDp, NENY_FETCH_01n);
    
    /*p27.PORE*/ wire PORE_WIN_MODE = not(top.NOCU_WIN_MODEn());
    /*p26.AXAD*/ wire AXAD_WIN_MODEn = not(PORE_WIN_MODE);
    /*p26.ACEN*/ wire ACEN_BG_MAP_READp = and (POTU_BG_MAP_READp, AXAD_WIN_MODEn);
    /*p25.XEZE*/ wire XEZE_WIN_MAP_READp = and (POTU_BG_MAP_READp, PORE_WIN_MODE);

    /*p27.NETA*/ wire NETA_TILE_READp = and (LENA_BGW_VRAM_RDp, NOGU_FETCH_01p);
    /*p26.ASUL*/ wire ASUL_TILE_READp = and (NETA_TILE_READp, AXAD_WIN_MODEn);
    /*p26.BEJE*/ wire BEJE_BGD_TILE_READn = not (ASUL_TILE_READp);
    /*p25.XUCY*/ wire XUCY_WIN_TILE_READn = nand(NETA_TILE_READp, PORE_WIN_MODE);

    /*p26.BAFY*/ wire BAFY_BG_MAP_READn = not(ACEN_BG_MAP_READp);
    /*p26.AXEP*/ top.VRM_TRI_A00.set_tribuf_6n(BAFY_BG_MAP_READn, BABE_MAP_X0S);
    /*p26.AFEB*/ top.VRM_TRI_A01.set_tribuf_6n(BAFY_BG_MAP_READn, ABOD_MAP_X1S);
    /*p26.ALEL*/ top.VRM_TRI_A02.set_tribuf_6n(BAFY_BG_MAP_READn, BEWY_MAP_X2S);
    /*p26.COLY*/ top.VRM_TRI_A03.set_tribuf_6n(BAFY_BG_MAP_READn, BYCA_MAP_X3S);
    /*p26.AJAN*/ top.VRM_TRI_A04.set_tribuf_6n(BAFY_BG_MAP_READn, ACUL_MAP_X4S);
    /*p26.DUHO*/ top.VRM_TRI_A05.set_tribuf_6n(BAFY_BG_MAP_READn, ETAM_MAP_Y0S);
    /*p26.CASE*/ top.VRM_TRI_A06.set_tribuf_6n(BAFY_BG_MAP_READn, DOTO_MAP_Y1S);
    /*p26.CYPO*/ top.VRM_TRI_A07.set_tribuf_6n(BAFY_BG_MAP_READn, DABA_MAP_Y2S);
    /*p26.CETA*/ top.VRM_TRI_A08.set_tribuf_6n(BAFY_BG_MAP_READn, EFYK_MAP_Y3S);
    /*p26.DAFE*/ top.VRM_TRI_A09.set_tribuf_6n(BAFY_BG_MAP_READn, EJOK_MAP_Y4S);
    /*p26.AMUV*/ top.VRM_TRI_A10.set_tribuf_6n(BAFY_BG_MAP_READn, top.XAFO_LCDC_BGMAP.q());
    /*p26.COVE*/ top.VRM_TRI_A11.set_tribuf_6n(BAFY_BG_MAP_READn, top.VYPO_GND);
    /*p26.COXO*/ top.VRM_TRI_A12.set_tribuf_6n(BAFY_BG_MAP_READn, top.VYPO_GND);

    // Window map read
    /*p25.WUKO*/ wire WUKO_WIN_MAP_READn = not(XEZE_WIN_MAP_READp);
    /*p27.XEJA*/ top.VRM_TRI_A00.set_tribuf_6n(WUKO_WIN_MAP_READn, top.WYKA_WIN_X3());
    /*p27.XAMO*/ top.VRM_TRI_A01.set_tribuf_6n(WUKO_WIN_MAP_READn, top.WODY_WIN_X4());
    /*p27.XAHE*/ top.VRM_TRI_A02.set_tribuf_6n(WUKO_WIN_MAP_READn, top.WOBO_WIN_X5());
    /*p27.XULO*/ top.VRM_TRI_A03.set_tribuf_6n(WUKO_WIN_MAP_READn, top.WYKO_WIN_X6());
    /*p27.WUJU*/ top.VRM_TRI_A04.set_tribuf_6n(WUKO_WIN_MAP_READn, top.XOLO_WIN_X7());
    /*p27.VYTO*/ top.VRM_TRI_A05.set_tribuf_6n(WUKO_WIN_MAP_READn, top.TUFU_WIN_Y3());
    /*p27.VEHA*/ top.VRM_TRI_A06.set_tribuf_6n(WUKO_WIN_MAP_READn, top.TAXA_WIN_Y4());
    /*p27.VACE*/ top.VRM_TRI_A07.set_tribuf_6n(WUKO_WIN_MAP_READn, top.TOZO_WIN_Y5());
    /*p27.VOVO*/ top.VRM_TRI_A08.set_tribuf_6n(WUKO_WIN_MAP_READn, top.TATE_WIN_Y6());
    /*p27.VULO*/ top.VRM_TRI_A09.set_tribuf_6n(WUKO_WIN_MAP_READn, top.TEKE_WIN_Y7());
    /*p27.VEVY*/ top.VRM_TRI_A10.set_tribuf_6n(WUKO_WIN_MAP_READn, top.WOKY_LCDC_WINMAP.q());
    /*p27.VEZA*/ top.VRM_TRI_A11.set_tribuf_6n(WUKO_WIN_MAP_READn, top.VYPO_GND);
    /*p27.VOGU*/ top.VRM_TRI_A12.set_tribuf_6n(WUKO_WIN_MAP_READn, top.VYPO_GND);

    // Background/window tile read

    /*p27.XUHA*/ wire XUHA_FETCH_S2p  = not (NOFU_FETCH_TILE_AB);
    /*p26.ASUM*/ top.VRM_TRI_A00.set_tribuf_6n(BEJE_BGD_TILE_READn, XUHA_FETCH_S2p);
    /*p26.EVAD*/ top.VRM_TRI_A01.set_tribuf_6n(BEJE_BGD_TILE_READn, FAFO_TILE_Y0S);
    /*p26.DAHU*/ top.VRM_TRI_A02.set_tribuf_6n(BEJE_BGD_TILE_READn, EMUX_TILE_Y1S);
    /*p26.DODE*/ top.VRM_TRI_A03.set_tribuf_6n(BEJE_BGD_TILE_READn, ECAB_TILE_Y2S);

    /*p25.XONU*/ top.VRM_TRI_A00.set_tribuf_6n(XUCY_WIN_TILE_READn, XUHA_FETCH_S2p);
    /*p25.WUDO*/ top.VRM_TRI_A01.set_tribuf_6n(XUCY_WIN_TILE_READn, top.VYNO_WIN_Y0());
    /*p25.WAWE*/ top.VRM_TRI_A02.set_tribuf_6n(XUCY_WIN_TILE_READn, top.VUJO_WIN_Y1());
    /*p25.WOLU*/ top.VRM_TRI_A03.set_tribuf_6n(XUCY_WIN_TILE_READn, top.VYMU_WIN_Y2());

    // FIXME P? wat?
    /*p25.VAPY*/ top.VRM_TRI_A04.set_tribuf_6p(NETA_TILE_READp, top.RAWU_BG_PIX_B0.q());
    /*p25.SEZU*/ top.VRM_TRI_A05.set_tribuf_6p(NETA_TILE_READp, top.POZO_BG_PIX_B1.q());
    /*p25.VEJY*/ top.VRM_TRI_A06.set_tribuf_6p(NETA_TILE_READp, top.PYZO_BG_PIX_B2.q());
    /*p25.RUSA*/ top.VRM_TRI_A07.set_tribuf_6p(NETA_TILE_READp, top.POXA_BG_PIX_B3.q());
    /*p25.ROHA*/ top.VRM_TRI_A08.set_tribuf_6p(NETA_TILE_READp, top.PULO_BG_PIX_B4.q());
    /*p25.RESO*/ top.VRM_TRI_A09.set_tribuf_6p(NETA_TILE_READp, top.POJU_BG_PIX_B5.q());
    /*p25.SUVO*/ top.VRM_TRI_A10.set_tribuf_6p(NETA_TILE_READp, top.POWY_BG_PIX_B6.q());
    /*p25.TOBO*/ top.VRM_TRI_A11.set_tribuf_6p(NETA_TILE_READp, top.PYJU_BG_PIX_B7.q());

    /*p25.VUZA*/ wire VUZA_TILE_BANKp = nor(top.WEXU_LCDC_BGTILE.q(), top.PYJU_BG_PIX_B7.q()); // register reused
    /*p25.VURY*/ top.VRM_TRI_A12.set_tribuf_6p(NETA_TILE_READp, VUZA_TILE_BANKp);
  }

  //----------------------------------------

  {
    // LOBY

    /*p24.LOBY*/ wire LOBY_RENDERINGn      = not(top.XYMU_RENDERINGp());
    /*p27.LAXE*/ wire LAXE_BFETCH_S0n      = not(LAXU_BFETCH_S0.q());
    /*p27.NAKO*/ wire NAKO_BFETCH_S1n      = not(MESU_BFETCH_S1.q());
    /*p27.NOFU*/ wire NOFU_BFETCH_S2n      = not(NYVA_BFETCH_S2.q());
    /*p27.MYSO*/ wire MYSO_BG_TRIGp        = nor(LOBY_RENDERINGn, LAXE_BFETCH_S0n, LYZU_BFETCH_S0_DELAY.q());

    /*p27.NYDY*/ wire NYDY_LATCH_BG_PIX_Ap = nand(MYSO_BG_TRIGp, MESU_BFETCH_S1.q(), NOFU_BFETCH_S2n);
    /*p32.METE*/ wire METE_LATCH_BG_PIX_An = not(NYDY_LATCH_BG_PIX_Ap);
    /*p32.LOMA*/ wire LOMA_LATCH_BG_PIX_Ap = not(METE_LATCH_BG_PIX_An);

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

    // LEGU_01 << LOMA_02    CLKp
    // LEGU_02 << VRM_TRI_D0 D
    // LEGU_03 nc
    // LEGU_04 << COMP_CLK   CLKn
    // LEGU_05 nc
    // LEGU_06 nc
    // LEGU_07 >> LUHE_01, LAKY_02 Q
    // LEGU_08 >> NC         Qn


    /*p32.LEGU*/ top.LEGU_BG_PIX_A0.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D0.q());
    /*p32.NUDU*/ top.NUDU_BG_PIX_A1.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D1.q());
    /*p32.MUKU*/ top.MUKU_BG_PIX_A2.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D2.q());
    /*p32.LUZO*/ top.LUZO_BG_PIX_A3.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D3.q());
    /*p32.MEGU*/ top.MEGU_BG_PIX_A4.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D4.q());
    /*p32.MYJY*/ top.MYJY_BG_PIX_A5.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D5.q());
    /*p32.NASA*/ top.NASA_BG_PIX_A6.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D6.q());
    /*p32.NEFO*/ top.NEFO_BG_PIX_A7.set(LOMA_LATCH_BG_PIX_Ap, !LOMA_LATCH_BG_PIX_Ap, top.VRM_TRI_D7.q());

    /*p27.MOFU*/ wire MOFU_LATCH_BG_PIX_Bn = and(MYSO_BG_TRIGp, NAKO_BFETCH_S1n);
    /*p32.LESO*/ wire LESO_LATCH_BG_PIX_Bp = not(MOFU_LATCH_BG_PIX_Bn);
    /*p??.LUVE*/ wire LUVE_MATCH_BG_PIX_Bn = not(LESO_LATCH_BG_PIX_Bp); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire LABU_LATCH_BG_PIX_Bp = not(LUVE_MATCH_BG_PIX_Bn);

    // Why are the clocks swapped?
    /*p32.RAWU*/ top.RAWU_BG_PIX_B0.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D0.q());
    /*p32.POZO*/ top.POZO_BG_PIX_B1.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D1.q());
    /*p32.PYZO*/ top.PYZO_BG_PIX_B2.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D2.q());
    /*p32.POXA*/ top.POXA_BG_PIX_B3.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D3.q());
    /*p32.PULO*/ top.PULO_BG_PIX_B4.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D4.q());
    /*p32.POJU*/ top.POJU_BG_PIX_B5.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D5.q());
    /*p32.POWY*/ top.POWY_BG_PIX_B6.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D6.q());
    /*p32.PYJU*/ top.PYJU_BG_PIX_B7.set(LABU_LATCH_BG_PIX_Bp, LUVE_MATCH_BG_PIX_Bn, top.VYPO_GND, top.VRM_TRI_D7.q());
  }


  // FF42 SCY
  {
    /*p22.WEBU*/ wire WEBU_FF42n = nand(top.WERO_FF4Xp(), top.XOLA_A00n(), top.WESA_A01p(), top.XUSY_A02n(), top.XERA_A03n());
    /*p22.XARO*/ wire XARO_FF42p = not(WEBU_FF42n);

    /*p23.ANYP*/ wire ANYP_FF42_RDp = and(XARO_FF42p, top.ASOT_CPU_RDp());
    /*p23.BUWY*/ wire BUWY_FF42_RDn = not(ANYP_FF42_RDp);

    /*p23.BEDY*/ wire BEDY_FF42_WRp = and(XARO_FF42p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.CAVO*/ wire CAVO_FF42_WRn = not(BEDY_FF42_WRp);

    /*p23.GAVE*/ GAVE_SCY0.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D0.q());
    /*p23.FYMO*/ FYMO_SCY1.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D1.q());
    /*p23.FEZU*/ FEZU_SCY2.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D2.q());
    /*p23.FUJO*/ FUJO_SCY3.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D3.q());
    /*p23.DEDE*/ DEDE_SCY4.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D4.q());
    /*p23.FOTY*/ FOTY_SCY5.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D5.q());
    /*p23.FOHA*/ FOHA_SCY6.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D6.q());
    /*p23.FUNY*/ FUNY_SCY7.set(CAVO_FF42_WRn, !CAVO_FF42_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D7.q());

    /*p23.WARE*/ top.CPU_TRI_D0.set_tribuf_6n(BUWY_FF42_RDn, GAVE_SCY0.q());
    /*p23.GOBA*/ top.CPU_TRI_D1.set_tribuf_6n(BUWY_FF42_RDn, FYMO_SCY1.q());
    /*p23.GONU*/ top.CPU_TRI_D2.set_tribuf_6n(BUWY_FF42_RDn, FEZU_SCY2.q());
    /*p23.GODO*/ top.CPU_TRI_D3.set_tribuf_6n(BUWY_FF42_RDn, FUJO_SCY3.q());
    /*p23.CUSA*/ top.CPU_TRI_D4.set_tribuf_6n(BUWY_FF42_RDn, DEDE_SCY4.q());
    /*p23.GYZO*/ top.CPU_TRI_D5.set_tribuf_6n(BUWY_FF42_RDn, FOTY_SCY5.q());
    /*p23.GUNE*/ top.CPU_TRI_D6.set_tribuf_6n(BUWY_FF42_RDn, FOHA_SCY6.q());
    /*p23.GYZA*/ top.CPU_TRI_D7.set_tribuf_6n(BUWY_FF42_RDn, FUNY_SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire WAVU_FF43n = nand(top.WERO_FF4Xp(), top.WADO_A00p(), top.WESA_A01p(), top.XUSY_A02n(), top.XERA_A03n());
    /*p22.XAVY*/ wire XAVY_FF43p = not(WAVU_FF43n);

    /*p23.AVOG*/ wire AVOG_FF43_RDp = and (XAVY_FF43p, top.ASOT_CPU_RDp());
    /*p23.BEBA*/ wire BEBA_FF43_RDn = not(AVOG_FF43_RDp);

    /*p23.ARUR*/ wire ARUR_FF43_WRp = and (XAVY_FF43p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.AMUN*/ wire AMUN_FF43_WRn = not(ARUR_FF43_WRp);

    /*p23.DATY*/ DATY_SCX0.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D0.q());
    /*p23.DUZU*/ DUZU_SCX1.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D1.q());
    /*p23.CYXU*/ CYXU_SCX2.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D2.q());
    /*p23.GUBO*/ GUBO_SCX3.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D3.q());
    /*p23.BEMY*/ BEMY_SCX4.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D4.q());
    /*p23.CUZY*/ CUZY_SCX5.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D5.q());
    /*p23.CABU*/ CABU_SCX6.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D6.q());
    /*p23.BAKE*/ BAKE_SCX7.set(AMUN_FF43_WRn, !AMUN_FF43_WRn, top.CUNU_SYS_RSTn(), top.CPU_TRI_D7.q());

    /*p23.EDOS*/ top.CPU_TRI_D0.set_tribuf_6n(BEBA_FF43_RDn, DATY_SCX0.q());
    /*p23.EKOB*/ top.CPU_TRI_D1.set_tribuf_6n(BEBA_FF43_RDn, DUZU_SCX1.q());
    /*p23.CUGA*/ top.CPU_TRI_D2.set_tribuf_6n(BEBA_FF43_RDn, CYXU_SCX2.q());
    /*p23.WONY*/ top.CPU_TRI_D3.set_tribuf_6n(BEBA_FF43_RDn, GUBO_SCX3.q());
    /*p23.CEDU*/ top.CPU_TRI_D4.set_tribuf_6n(BEBA_FF43_RDn, BEMY_SCX4.q());
    /*p23.CATA*/ top.CPU_TRI_D5.set_tribuf_6n(BEBA_FF43_RDn, CUZY_SCX5.q());
    /*p23.DOXE*/ top.CPU_TRI_D6.set_tribuf_6n(BEBA_FF43_RDn, CABU_SCX6.q());
    /*p23.CASY*/ top.CPU_TRI_D7.set_tribuf_6n(BEBA_FF43_RDn, BAKE_SCX7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash TileFetcher::commit(SchematicTop& top) {
  SignalHash hash;

  hash << PYGO_FETCH_DONE_Cp.commit();
  hash << POKY_PORCH_DONEp.commit();

  hash << LAXU_BFETCH_S0.commit();
  hash << MESU_BFETCH_S1.commit();
  hash << NYVA_BFETCH_S2.commit();

  hash << LOVY_FETCH_DONEp.commit();
  hash << LONY_BG_READ_VRAM_LATCHp.commit();

  hash << NYKA_TILE_FETCH_DONE_Ap.commit();
  hash << PORY_TILE_FETCH_DONE_Bp.commit();
  hash << LYZU_BFETCH_S0_DELAY.commit();

  hash << GAVE_SCY0.commit();
  hash << FYMO_SCY1.commit();
  hash << FEZU_SCY2.commit();
  hash << FUJO_SCY3.commit();
  hash << DEDE_SCY4.commit();
  hash << FOTY_SCY5.commit();
  hash << FOHA_SCY6.commit();
  hash << FUNY_SCY7.commit();

  hash << DATY_SCX0.commit();
  hash << DUZU_SCX1.commit();
  hash << CYXU_SCX2.commit();
  hash << GUBO_SCX3.commit();
  hash << BEMY_SCX4.commit();
  hash << CUZY_SCX5.commit();
  hash << CABU_SCX6.commit();
  hash << BAKE_SCX7.commit();

  hash << top.LEGU_BG_PIX_A0.commit();
  hash << top.NUDU_BG_PIX_A1.commit();
  hash << top.MUKU_BG_PIX_A2.commit();
  hash << top.LUZO_BG_PIX_A3.commit();
  hash << top.MEGU_BG_PIX_A4.commit();
  hash << top.MYJY_BG_PIX_A5.commit();
  hash << top.NASA_BG_PIX_A6.commit();
  hash << top.NEFO_BG_PIX_A7.commit();

  hash << top.RAWU_BG_PIX_B0.commit();
  hash << top.POZO_BG_PIX_B1.commit();
  hash << top.PYZO_BG_PIX_B2.commit(); 
  hash << top.POXA_BG_PIX_B3.commit(); 
  hash << top.PULO_BG_PIX_B4.commit(); 
  hash << top.POJU_BG_PIX_B5.commit(); 
  hash << top.POWY_BG_PIX_B6.commit(); 
  hash << top.PYJU_BG_PIX_B7.commit();

  return hash;
}

//------------------------------------------------------------------------------
