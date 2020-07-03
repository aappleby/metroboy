#include "Sch_LCD.h"
#include "Sch_Top.h"

using namespace Schematics;

// Die trace:
// VYBO = nor(FEPO04, WODU04, MYVO02)
// TYFA = and(SOCY02, POKY04, VYBO04)
// SEGU = not(TYFA05) // 5 rung inverter, because fanout?
// ROXO = not(SEGU05)

//------------------------------------------------------------------------------

void LcdRegisters::tick(SchematicTop& top) {

  /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
  /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
  /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
  /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);

  /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);
  /*p22.WALO*/ wire WALO_A02p = not(XUSY_A02n);

  //----------------------------------------

  // LCD main timer
  {
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.MUDE*/ wire _MUDE_X_RSTn = nor(RUTU_NEW_LINE_d0.q(), LYHA_VID_RSTp);
    /*p21.TALU*/ wire TALU_xBCDExxx = not(top.VENA_xBCDExxx());
    /*p21.SAXO*/ XEHO_X0.set(TALU_xBCDExxx, _MUDE_X_RSTn, !XEHO_X0.q());
    /*p21.TYPO*/ SAVY_X1.set(!XEHO_X0.q(),  _MUDE_X_RSTn, !SAVY_X1.q());
    /*p21.VYZO*/ XODU_X2.set(!SAVY_X1.q(),  _MUDE_X_RSTn, !XODU_X2.q());
    /*p21.TELU*/ XYDO_X3.set(!XODU_X2.q(),  _MUDE_X_RSTn, !XYDO_X3.q());
    /*p21.SUDE*/ TUHU_X4.set(!XYDO_X3.q(),  _MUDE_X_RSTn, !TUHU_X4.q());
    /*p21.TAHA*/ TUKY_X5.set(!TUHU_X4.q(),  _MUDE_X_RSTn, !TUKY_X5.q());
    /*p21.TYRY*/ TAKO_X6.set(!TUKY_X5.q(),  _MUDE_X_RSTn, !TAKO_X6.q());
  }

  {
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor(MYTA_LINE_153_d4.q(), LYHA_VID_RSTp);
    /*p21.MUWY*/ MUWY_Y0.set(RUTU_NEW_LINE_d0.q(), _LAMA_FRAME_RSTn, !MUWY_Y0.q());
    /*p21.MYRO*/ MYRO_Y1.set(!MUWY_Y0.q(),         _LAMA_FRAME_RSTn, !MYRO_Y1.q());
    /*p21.LEXA*/ LEXA_Y2.set(!MYRO_Y1.q(),         _LAMA_FRAME_RSTn, !LEXA_Y2.q());
    /*p21.LYDO*/ LYDO_Y3.set(!LEXA_Y2.q(),         _LAMA_FRAME_RSTn, !LYDO_Y3.q());
    /*p21.LOVU*/ LOVU_Y4.set(!LYDO_Y3.q(),         _LAMA_FRAME_RSTn, !LOVU_Y4.q());
    /*p21.LEMA*/ LEMA_Y5.set(!LOVU_Y4.q(),         _LAMA_FRAME_RSTn, !LEMA_Y5.q());
    /*p21.MATO*/ MATO_Y6.set(!LEMA_Y5.q(),         _LAMA_FRAME_RSTn, !MATO_Y6.q());
    /*p21.LAFO*/ LAFO_Y7.set(!MATO_Y6.q(),         _LAMA_FRAME_RSTn, !LAFO_Y7.q());
  }

  {
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p21.XYVO*/ wire _XYVO_IN_VBLANKp = and(LOVU_Y4.q(), LAFO_Y7.q()); // 128 + 16 = 144
    /*p21.POPU*/ POPU_VBLANK_d4.set(NYPE_NEW_LINE_d4.q(), LYFE_VID_RSTn, _XYVO_IN_VBLANKp);
  }

  {
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p21.SANU*/ wire _SANU_LINE_END = and(TAKO_X6.q(), TUKY_X5.q(), TUHU_X4.q(), XEHO_X0.q()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.TALU*/ wire TALU_xBCDExxx = not(top.VENA_xBCDExxx());
    /*p21.SONO*/ wire SONO_AxxxxFGH = not(TALU_xBCDExxx);
    /*p21.RUTU*/ RUTU_NEW_LINE_d0.set(SONO_AxxxxFGH, LYFE_VID_RSTn, _SANU_LINE_END);
    /*p21.NYPE*/ NYPE_NEW_LINE_d4.set(TALU_xBCDExxx, LYFE_VID_RSTn, RUTU_NEW_LINE_d0.q());
  }

  {
    
    /*p21.XYVO*/ wire _XYVO_IN_VBLANKp = and(LOVU_Y4.q(), LAFO_Y7.q()); // 128 + 16 = 144
    /*p29.ALES*/ wire _ALES_IN_VBLANKn = not(_XYVO_IN_VBLANKp);
    /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(top.RUTU_NEW_LINE_d0());
    /*p21.SELA*/ wire SELA_NEW_LINE_d0p = not(PURE_NEW_LINE_d0n);
    /*p29.ABOV*/ wire _ABOV_VID_LINE_d0  = and(SELA_NEW_LINE_d0p, _ALES_IN_VBLANKn);
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
    /*p29.XUPY*/ wire XUPY_xBCxxFGx = not(top.WUVU_AxxDExxH());
    /*p29.CATU*/ CATU_VID_LINE_d4.set (XUPY_xBCxxFGx, ABEZ_VID_RSTn, _ABOV_VID_LINE_d0);
    /*p28.AWOH*/ wire AWOH_AxxDExxH = not(XUPY_xBCxxFGx);
    /*p28.ANEL*/ ANEL_VID_LINE_d6.set (AWOH_AxxDExxH, ABEZ_VID_RSTn, CATU_VID_LINE_d4.q());
  }

  {
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p21.NOKO*/ wire _NOKO_LINE_153 = and(LAFO_Y7.q(), LOVU_Y4.q(), LYDO_Y3.q(), MUWY_Y0.q()); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*p21.MYTA*/ MYTA_LINE_153_d4.set(NYPE_NEW_LINE_d4.q(), LYFE_VID_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_FR = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p24.LOFU*/ wire _LOFU_CLKn = not(RUTU_NEW_LINE_d0);
    /*p24.LUCA*/ LUCA_LINE_EVEN.set(_LOFU_CLKn, LYFE_VID_RSTn, !LUCA_LINE_EVEN.q());
    /*p21.NAPO*/ NAPO_FRAME_EVEN.set(POPU_VBLANK_d4.q(), LYFE_VID_RSTn, !NAPO_FRAME_EVEN.q());
    /*p24.MAGU*/ wire _MAGU = xor(NAPO_FRAME_EVEN, LUCA_LINE_EVEN.q());
    /*p24.MECO*/ wire _MECO = not(_MAGU);
    /*p24.KEBO*/ wire _KEBO = not(_MECO);
    /*p24.USEC*/ wire _USEC = not(top.UREK_DIV_07n());
    /*p24.KEDY*/ wire KEDY_LCDC_ENn = not(top.XONA_LCDC_EN);
    /*p24.KUPA*/ wire _KUPA = amux2(top.XONA_LCDC_EN, _KEBO, KEDY_LCDC_ENn, _USEC);
    /*p24.KOFO*/ wire _KOFO = not(_KUPA);
    top.LCD_PIN_FR.set(_KOFO);
  }

  // LCD line strobe
  {
    /*p21.TOCU*/ wire _C0n = not(XEHO_X0.q());
    /*p21.VEPE*/ wire _C1n = not(SAVY_X1.q());
    /*p21.VUTY*/ wire _C2n = not(XODU_X2.q());
    /*p21.VATE*/ wire _C3n = not(XYDO_X3.q());
    /*p21.TUDA*/ wire _C4n = not(TUHU_X4.q());
    /*p21.TAFY*/ wire _C5n = not(TUKY_X5.q());
    /*p21.TUJU*/ wire _C6n = not(TAKO_X6.q());

    /*p21.VOKU*/ wire _VOKU_000n = nand(_C6n, _C5n, _C4n, _C3n, _C2n, _C1n, _C0n); // 0000000 == 0
    /*p21.TOZU*/ wire _TOZU_007n = nand(_C6n, _C5n, _C4n, _C3n,  XODU_X2,  SAVY_X1,  XEHO_X0); // 0000111 == 7
    /*p21.TECE*/ wire _TECE_045n = nand(_C6n,  TUKY_X5, _C4n,  XYDO_X3,  XODU_X2, _C1n,  XEHO_X0); // 0101101 == 45
    /*p21.TEBO*/ wire _TEBO_083n = nand( TAKO_X6, _C5n,  TUHU_X4, _C3n, _C2n,  SAVY_X1,  XEHO_X0); // 1010011 == 83

    /*p21.TEGY*/ wire _TEGY_LINE_STROBE = nand(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p21.TALU*/ wire TALU_xBCDExxx = not(top.VENA_xBCDExxx());
    /*p21.SONO*/ wire SONO_AxxxxFGH = not(TALU_xBCDExxx);
    /*p21.SYGU*/ SYGU_LINE_STROBE.set(SONO_AxxxxFGH, LYFE_VID_RSTn, _TEGY_LINE_STROBE);
    /*p21.RYNO*/ wire _RYNO = or(_TEGY_LINE_STROBE, RUTU_NEW_LINE_d0);
    /*p21.POGU*/ wire _POGU = not(_RYNO);
    top.LCD_PIN_CPG.set(_POGU);
  }

  // LCD LCD_PIN_CPL pin
  {
    /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(top.RUTU_NEW_LINE_d0());
    /*p24.KASA*/ wire _KASA_LINE_DONE = not(PURE_NEW_LINE_d0n);
    /*p24.UMOB*/ wire _UMOB_DIV_06p = not(top.UMEK_DIV_06n());
    /*p24.KEDY*/ wire KEDY_LCDC_ENn = not(top.XONA_LCDC_EN);
    /*p24.KAHE*/ wire _KAHE = amux2(top.XONA_LCDC_EN, _KASA_LINE_DONE, KEDY_LCDC_ENn, _UMOB_DIV_06p);
    /*p24.KYMO*/ wire _KYMO = not(_KAHE);
    top.LCD_PIN_CPL.set(_KYMO);
  }

  // LCD vertical sync pin
  {
    /*p24.NERU*/ wire _LINE_000n = nor(MUWY_Y0, MYRO_Y1, LEXA_Y2, LYDO_Y3, LOVU_Y4, LEMA_Y5, MATO_Y6, LAFO_Y7);
    
    /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
    /*p24.MEDA*/ MEDA_VSYNC_OUTn.set(NYPE_NEW_LINE_d4, LYFE_VID_RSTn, _LINE_000n);
    /*p24.MURE*/ wire _MURE_PIN_S = not(MEDA_VSYNC_OUTn);
    top.LCD_PIN_S.set(_MURE_PIN_S);
  }

  // ly match
  {
    // SYFU01 >> SOVU01 (output on top rung?)
    // SYFU02 << LAFO17
    // SYFU03 << RAHA08
    // SYFU04 ?? XYVO04
    // SYFU05 ?? MATO17
    // SYFU06 nc

    // TERY has an arm on the VCC side

    
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WESY*/ wire WESY_RSTn = not(XORE_RSTp);
    /*p21.TALU*/ wire TALU_xBCDExxx = not(top.VENA_xBCDExxx());

    /*p21.SYFU*/ wire SYFU_LY_MATCH7n = xor (LAFO_Y7, RAHA_LYC7.qn());
    /*p21.TERY*/ wire TERY_LY_MATCH6n = xor (MATO_Y6, VEVO_LYC6.qn());
    /*p21.TUCY*/ wire TUCY_LY_MATCH5n = xor (LEMA_Y5, VAFA_LYC5.qn());
    /*p21.TYKU*/ wire TYKU_LY_MATCH4n = xor (LOVU_Y4, SOTA_LYC4.qn());
    /*p21.RASY*/ wire RASY_LY_MATCH3n = xor (LYDO_Y3, SALO_LYC3.qn());
    /*p21.REDA*/ wire REDA_LY_MATCH2n = xor (LEXA_Y2, SEDY_LYC2.qn());
    /*p21.TYDE*/ wire TYDE_LY_MATCH1n = xor (MYRO_Y1, VUCE_LYC1.qn());
    /*p21.RYME*/ wire RYME_LY_MATCH0n = xor (MUWY_Y0, SYRY_LYC0.qn());
    /*p21.SOVU*/ wire SOVU_LY_MATCHA  = nor (SYFU_LY_MATCH7n, TERY_LY_MATCH6n, TUCY_LY_MATCH5n, TYKU_LY_MATCH4n); // def nor
    /*p21.SUBO*/ wire SUBO_LY_MATCHB  = nor (RASY_LY_MATCH3n, REDA_LY_MATCH2n, TYDE_LY_MATCH1n, RYME_LY_MATCH0n); // def nor
    /*p21.RAPE*/ wire RAPE_LY_MATCHn  = nand(SOVU_LY_MATCHA,  SUBO_LY_MATCHB); // def nand
    /*p21.PALY*/ wire PALY_LY_MATCHa  = not (RAPE_LY_MATCHn); // def not

    /*p21.ROPO*/ ROPO_LY_MATCH_SYNCp.set(TALU_xBCDExxx, WESY_RSTn, PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire FF44n = nand(top.WERO_FF40_FF4Fp(), XOLA_A00n, XENO_A01n, WALO_A02p, XERA_A03n);
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.WAFU*/ wire FF44_RD = and (ASOT_CPU_RD, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(MUWY_Y0.q());
    /*p23.XEPO*/ wire LY1n = not(MYRO_Y1.q());
    /*p23.MYFA*/ wire LY2n = not(LEXA_Y2.q());
    /*p23.XUHY*/ wire LY3n = not(LYDO_Y3.q());
    /*p23.WATA*/ wire LY4n = not(LOVU_Y4.q());
    /*p23.XAGA*/ wire LY5n = not(LEMA_Y5.q());
    /*p23.XUCE*/ wire LY6n = not(MATO_Y6.q());
    /*p23.XOWO*/ wire LY7n = not(LAFO_Y7.q());

    /*p23.VEGA*/ top.CPU_TRI_D0.set_tribuf(!FF44_RDn, not(LY0n));
    /*p23.WUVA*/ top.CPU_TRI_D1.set_tribuf(!FF44_RDn, not(LY1n));
    /*p23.LYCO*/ top.CPU_TRI_D2.set_tribuf(!FF44_RDn, not(LY2n));
    /*p23.WOJY*/ top.CPU_TRI_D3.set_tribuf(!FF44_RDn, not(LY3n));
    /*p23.VYNE*/ top.CPU_TRI_D4.set_tribuf(!FF44_RDn, not(LY4n));
    /*p23.WAMA*/ top.CPU_TRI_D5.set_tribuf(!FF44_RDn, not(LY5n));
    /*p23.WAVO*/ top.CPU_TRI_D6.set_tribuf(!FF44_RDn, not(LY6n));
    /*p23.WEZE*/ top.CPU_TRI_D7.set_tribuf(!FF44_RDn, not(LY7n));
  }

  // FF45 LYC
  {

    /*p22.WETY*/ wire FF45n = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, XENO_A01n, WALO_A02p, XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.XYLY*/ wire FF45_RD = and (ASOT_CPU_RD, FF45);
    /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);
    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.XUFA*/ wire FF45_WR = and (CUPA_CPU_WR_xxxxxFGH, FF45);
    /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

    /*p23.RETU*/ top.CPU_TRI_D0.set_tribuf(!FF45_RDn, SYRY_LYC0.q());
    /*p23.VOJO*/ top.CPU_TRI_D1.set_tribuf(!FF45_RDn, VUCE_LYC1.q());
    /*p23.RAZU*/ top.CPU_TRI_D2.set_tribuf(!FF45_RDn, SEDY_LYC2.q());
    /*p23.REDY*/ top.CPU_TRI_D3.set_tribuf(!FF45_RDn, SALO_LYC3.q());
    /*p23.RACE*/ top.CPU_TRI_D4.set_tribuf(!FF45_RDn, SOTA_LYC4.q());
    /*p23.VAZU*/ top.CPU_TRI_D5.set_tribuf(!FF45_RDn, VAFA_LYC5.q());
    /*p23.VAFE*/ top.CPU_TRI_D6.set_tribuf(!FF45_RDn, VEVO_LYC6.q());
    /*p23.PUFY*/ top.CPU_TRI_D7.set_tribuf(!FF45_RDn, RAHA_LYC7.q());

    
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WESY*/ wire WESY_RSTn = not(XORE_RSTp);
    /*p23.SYRY*/ SYRY_LYC0.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D0);
    /*p23.VUCE*/ VUCE_LYC1.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D1);
    /*p23.SEDY*/ SEDY_LYC2.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D2);
    /*p23.SALO*/ SALO_LYC3.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D3);
    /*p23.SOTA*/ SOTA_LYC4.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D4);
    /*p23.VAFA*/ VAFA_LYC5.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D5);
    /*p23.VEVO*/ VEVO_LYC6.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D6);
    /*p23.RAHA*/ RAHA_LYC7.set(FF45_WRn, WESY_RSTn, top.CPU_TRI_D7);
  }
}

//------------------------------------------------------------------------------

SignalHash LcdRegisters::commit() {
  SignalHash hash;
  /*p21.SAXO*/ hash << XEHO_X0.commit_reg(); // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ hash << SAVY_X1.commit_reg();
  /*p21.VYZO*/ hash << XODU_X2.commit_reg();
  /*p21.TELU*/ hash << XYDO_X3.commit_reg();
  /*p21.SUDE*/ hash << TUHU_X4.commit_reg();
  /*p21.TAHA*/ hash << TUKY_X5.commit_reg();
  /*p21.TYRY*/ hash << TAKO_X6.commit_reg();
  /*p21.MUWY*/ hash << MUWY_Y0.commit_reg(); // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ hash << MYRO_Y1.commit_reg();
  /*p21.LEXA*/ hash << LEXA_Y2.commit_reg();
  /*p21.LYDO*/ hash << LYDO_Y3.commit_reg();
  /*p21.LOVU*/ hash << LOVU_Y4.commit_reg();
  /*p21.LEMA*/ hash << LEMA_Y5.commit_reg();
  /*p21.MATO*/ hash << MATO_Y6.commit_reg();
  /*p21.LAFO*/ hash << LAFO_Y7.commit_reg();
  /*p21.RUTU*/ hash << RUTU_NEW_LINE_d0.commit_reg(); // p909+8
  /*p29.CATU*/ hash << CATU_VID_LINE_d4.commit_reg();  // p001+8
  /*p21.NYPE*/ hash << NYPE_NEW_LINE_d4.commit_reg(); // p001+8
  /*p28.ANEL*/ hash << ANEL_VID_LINE_d6.commit_reg();  // p003+8
  /*p21.MYTA*/ hash << MYTA_LINE_153_d4.commit_reg();  // p153:001 - p000:001
  /*p21.POPU*/ hash << POPU_VBLANK_d4.commit_reg();    // p144:001 - p000:001
  /*p21.SYGU*/ hash << SYGU_LINE_STROBE.commit_reg();
  /*p24.MEDA*/ hash << MEDA_VSYNC_OUTn.commit_reg();
  /*p24.LUCA*/ hash << LUCA_LINE_EVEN.commit_reg();
  /*p21.NAPO*/ hash << NAPO_FRAME_EVEN.commit_reg();

  /*p23.SYRY*/ hash << SYRY_LYC0.commit_reg();
  /*p23.VUCE*/ hash << VUCE_LYC1.commit_reg();
  /*p23.SEDY*/ hash << SEDY_LYC2.commit_reg();
  /*p23.SALO*/ hash << SALO_LYC3.commit_reg();
  /*p23.SOTA*/ hash << SOTA_LYC4.commit_reg();
  /*p23.VAFA*/ hash << VAFA_LYC5.commit_reg();
  /*p23.VEVO*/ hash << VEVO_LYC6.commit_reg();
  /*p23.RAHA*/ hash << RAHA_LYC7.commit_reg();

  /*p21.ROPO*/ hash << ROPO_LY_MATCH_SYNCp.commit_reg();
  return hash;
}

//------------------------------------------------------------------------------

#if 0
uint32_t x() const {
  return (XEHO_X0.q() << 0) | (SAVY_X1.q() << 1) | (XODU_X2.q() << 2) | (XYDO_X3.q() << 3) | (TUHU_X4.q() << 4) | (TUKY_X5.q() << 5) | (TAKO_X6.q() << 6);
}

uint32_t y() const {
  return (MUWY_Y0.q() << 0) | (MYRO_Y1.q() << 1) | (LEXA_Y2.q() << 2) | (LYDO_Y3.q() << 3) | (LOVU_Y4.q() << 4) | (LEMA_Y5.q() << 5) | (MATO_Y6.q() << 6) | (LAFO_Y7.q() << 7);
}


void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- LCD REG -----\n");

  dump(text_painter, "LCD X ", XEHO_X0, SAVY_X1, XODU_X2, XYDO_X3, TUHU_X4, TUKY_X5, TAKO_X6);
  dump(text_painter, "LCD Y ", MUWY_Y0, MYRO_Y1, LEXA_Y2, LYDO_Y3, LOVU_Y4, LEMA_Y5, MATO_Y6, LAFO_Y7);

  dump_long(text_painter, "RUTU_NEW_LINE_d0   ", RUTU_NEW_LINE_d0.a);
  dump_long(text_painter, "CATU_VID_LINE_d4   ", CATU_VID_LINE_d4.a);
  dump_long(text_painter, "NYPE_NEW_LINE_d4   ", NYPE_NEW_LINE_d4.a);
  dump_long(text_painter, "ANEL_VID_LINE_d6   ", ANEL_VID_LINE_d6.a);
  dump_long(text_painter, "MYTA_LINE_153_d4   ", MYTA_LINE_153_d4.a);
  dump_long(text_painter, "POPU_VBLANK_d4     ", POPU_VBLANK_d4.a);
  dump_long(text_painter, "SYGU_LINE_STROBE   ", SYGU_LINE_STROBE.a);
  dump_long(text_painter, "MEDA_VSYNC_OUTn    ", MEDA_VSYNC_OUTn.a);
  dump_long(text_painter, "LUCA_LINE_EVEN     ", LUCA_LINE_EVEN.a);
  dump_long(text_painter, "NAPO_FRAME_EVEN    ", NAPO_FRAME_EVEN.a);
  text_painter.newline();
}



void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- LCD_PINS -----\n");
  //LD1.dump(text_painter, "LD1 ");
  //LD0.dump(text_painter, "LD0 ");
  LCD_PIN_CPG.dump(text_painter, "LCD_PIN_CPG ");
  LCD_PIN_CPL.dump(text_painter, "LCD_PIN_CPL ");
  LCD_PIN_FR.dump(text_painter, "LCD_PIN_FR  ");
  S.dump(text_painter, "S   ");
  text_painter.newline();
}

#endif