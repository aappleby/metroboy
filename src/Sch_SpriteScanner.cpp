#include "Sch_SpriteScanner.h"
#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

SpriteScannerSignals SpriteScanner::sig(const TestGB& gb) const {
  SpriteScannerSignals sprite_scanner_sig;

  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto oam_sig = gb.oam_bus.sig();
  auto clk_sig = gb.clk_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  auto& ppu_config = gb.ppu_config;

  wire P10_B = 0;

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(lcd_sig.ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);

  {
    /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
    /*p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or(BALU_SCAN_RST, SCAN_DONE_TRIG_B.q(), !SCAN_DONE_TRIG_A.q());
    /*p29.AVAP*/ sprite_scanner_sig.AVAP_SCAN_DONE_TRIGp = not(BEBU_SCAN_DONE_TRIGn);
  }

  /*p29.CEHA*/ wire CEHA_SCANNINGp = not(CENO_SCANNINGp.qn());

  {
    /*p28.BESU*/ sprite_scanner_sig.BESU_SCANNINGp = BESU_SCANNINGp;
    /*p29.CENO*/ sprite_scanner_sig.CENO_SCANNINGp = CENO_SCANNINGp;
    /*p29.BYJO*/ sprite_scanner_sig.BYJO_SCANNINGn = not(CEHA_SCANNINGp);
  }

  {
    /*p29.EBOS*/ wire Y0n = not(lcd_sig.MUWY_Y0);
    /*p29.DASA*/ wire Y1n = not(lcd_sig.MYRO_Y1);
    /*p29.FUKY*/ wire Y2n = not(lcd_sig.LEXA_Y2);
    /*p29.FUVE*/ wire Y3n = not(lcd_sig.LYDO_Y3);
    /*p29.FEPU*/ wire Y4n = not(lcd_sig.LOVU_Y4);
    /*p29.FOFA*/ wire Y5n = not(lcd_sig.LEMA_Y5);
    /*p29.FEMO*/ wire Y6n = not(lcd_sig.MATO_Y6);
    /*p29.GUSU*/ wire Y7n = not(lcd_sig.LAFO_Y7);

    /*p29.ERUC*/ wire YDIFF_S0 = add_c(Y0n, oam_sig.XUSO_SPRITE_Y0, P10_B); // are these really connected directly to the pin?
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(Y0n, oam_sig.XUSO_SPRITE_Y0, P10_B);
    /*p29.ENEF*/ wire YDIFF_S1 = add_s(Y1n, oam_sig.XEGU_SPRITE_Y1, YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(Y1n, oam_sig.XEGU_SPRITE_Y1, YDIFF_C0);
    /*p29.FECO*/ wire YDIFF_S2 = add_s(Y2n, oam_sig.YJEX_SPRITE_Y2, YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(Y2n, oam_sig.YJEX_SPRITE_Y2, YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF_S3 = add_s(Y3n, oam_sig.XYJU_SPRITE_Y3, YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(Y3n, oam_sig.XYJU_SPRITE_Y3, YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF_S4 = add_s(Y4n, oam_sig.YBOG_SPRITE_Y4, YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(Y4n, oam_sig.YBOG_SPRITE_Y4, YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF_S5 = add_s(Y5n, oam_sig.WYSO_SPRITE_Y5, YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(Y5n, oam_sig.WYSO_SPRITE_Y5, YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF_S6 = add_s(Y6n, oam_sig.XOTE_SPRITE_Y6, YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(Y6n, oam_sig.XOTE_SPRITE_Y6, YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF_S7 = add_s(Y7n, oam_sig.YZAB_SPRITE_Y7, YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(Y7n, oam_sig.YZAB_SPRITE_Y7, YDIFF_C6);

    /*p29.DEGE*/ sprite_scanner_sig.DEGE_SPRITE_DELTA0 = not(YDIFF_S0);
    /*p29.DABY*/ sprite_scanner_sig.DABY_SPRITE_DELTA1 = not(YDIFF_S1);
    /*p29.DABU*/ sprite_scanner_sig.DABU_SPRITE_DELTA2 = not(YDIFF_S2);
    /*p29.GYSA*/ sprite_scanner_sig.GYSA_SPRITE_DELTA3 = not(YDIFF_S3);
    /*p29.GACE*/ wire GACE_SPRITE_DELTA4 = not(YDIFF_S4);
    /*p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not(YDIFF_S5);
    /*p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not(YDIFF_S6);
    /*p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not(YDIFF_S7);

    /*p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or (YDIFF_S3, ppu_config.XYMO_LCDC_SPSIZE);
    /*p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, YDIFF_C7, GOVU_SPSIZE_MATCH);
    /*p29.GESE*/ wire GESE_SCAN_MATCH_Y = not(WOTA_SCAN_MATCH_Yn);
    /*p29.CARE*/ wire CARE_STORE_ENp_ABxxEFxx = and (clk_sig.XOCE_ABxxEFxx, CEHA_SCANNINGp, GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*p29.DYTY*/ sprite_scanner_sig.DYTY_STORE_ENn_xxCDxxGH = not(CARE_STORE_ENp_ABxxEFxx);
  }

  {
    /*p28.GUSE*/ sprite_scanner_sig.GUSE_SCAN0n = not(SCAN0.q());
    /*p28.GEMA*/ sprite_scanner_sig.GEMA_SCAN1n = not(SCAN1.q());
    /*p28.FUTO*/ sprite_scanner_sig.FUTO_SCAN2n = not(SCAN2.q());
    /*p28.FAKU*/ sprite_scanner_sig.FAKU_SCAN3n = not(SCAN3.q());
    /*p28.GAMA*/ sprite_scanner_sig.GAMA_SCAN4n = not(SCAN4.q());
    /*p28.GOBY*/ sprite_scanner_sig.GOBY_SCAN5n = not(SCAN5.q());
  }



  return sprite_scanner_sig;
}

//------------------------------------------------------------------------------

void SpriteScanner::tick(TestGB& gb) {
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(lcd_sig.ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*p29.BALU*/ wire BALU_SCAN_RSTp = not(ANOM_SCAN_RSTn);
    /*p29.BAGY*/ wire BAGY_SCAN_RSTn = not(BALU_SCAN_RSTp);

    /*p28.FETO*/ wire FETO_SCAN_DONE_d0 = and (SCAN0, SCAN1, SCAN2, SCAN5); // 32 + 4 + 2 + 1 = 39
    /*p29.BYBA*/ SCAN_DONE_TRIG_A.set(clk_sig.XUPY_xBCxxFGx, BAGY_SCAN_RSTn, FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ SCAN_DONE_TRIG_B.set(clk_sig.ALET_xBxDxFxH, BAGY_SCAN_RSTn, SCAN_DONE_TRIG_A);

    /*p29.BEBU*/ wire BEBU_SCAN_RSTn = or (BALU_SCAN_RSTp, SCAN_DONE_TRIG_B.q(), !SCAN_DONE_TRIG_A.q());
    /*p29.AVAP*/ wire AVAP_SCAN_RSTp = not(BEBU_SCAN_RSTn);
    /*p28.ASEN*/ wire ASEN_SCAN_RSTp = or (rst_sig.ATAR_VID_RSTp, AVAP_SCAN_RSTp);

    /*p28.BESU*/ BESU_SCANNINGp.nor_latch(lcd_sig.CATU_VID_LINE_d4, ASEN_SCAN_RSTp);
    /*p29.CENO*/ CENO_SCANNINGp.set(clk_sig.XUPY_xBCxxFGx, rst_sig.ABEZ_VID_RSTn, BESU_SCANNINGp);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.FETO*/ wire FETO_SCAN_DONE_d0 = and (SCAN0, SCAN1, SCAN2, SCAN5); // 32 + 4 + 2 + 1 = 39
    /*p28.GAVA*/ wire SCAN_CLK = or(FETO_SCAN_DONE_d0, clk_sig.XUPY_xBCxxFGx);
    /*p28.YFEL*/ SCAN0.set(SCAN_CLK, ANOM_SCAN_RSTn, !SCAN0);
    /*p28.WEWY*/ SCAN1.set(!SCAN0,   ANOM_SCAN_RSTn, !SCAN1);
    /*p28.GOSO*/ SCAN2.set(!SCAN1,   ANOM_SCAN_RSTn, !SCAN2);
    /*p28.ELYN*/ SCAN3.set(!SCAN2,   ANOM_SCAN_RSTn, !SCAN3);
    /*p28.FAHA*/ SCAN4.set(!SCAN3,   ANOM_SCAN_RSTn, !SCAN4);
    /*p28.FONY*/ SCAN5.set(!SCAN4,   ANOM_SCAN_RSTn, !SCAN5);
  }
}

//------------------------------------------------------------------------------

bool SpriteScanner::commit() {
  bool changed = false;

  /*p28.BESU*/ changed |= BESU_SCANNINGp.commit_latch();
  /*p29.CENO*/ changed |= CENO_SCANNINGp.commit_reg();
  /*p28.YFEL*/ changed |= SCAN0.commit_reg();
  /*p28.WEWY*/ changed |= SCAN1.commit_reg();
  /*p28.GOSO*/ changed |= SCAN2.commit_reg();
  /*p28.ELYN*/ changed |= SCAN3.commit_reg();
  /*p28.FAHA*/ changed |= SCAN4.commit_reg();
  /*p28.FONY*/ changed |= SCAN5.commit_reg();
  /*p29.BYBA*/ changed |= SCAN_DONE_TRIG_A.commit_reg();
  /*p29.DOBA*/ changed |= SCAN_DONE_TRIG_B.commit_reg();

  return changed;
}

//------------------------------------------------------------------------------