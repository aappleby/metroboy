#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"
#include "Sch_Timer.h"

using namespace Schematics;

// Die trace:
// VYBO = nor(FEPO04, WODU04, MYVO02)
// TYFA = and(SOCY02, POKY04, VYBO04)
// SEGU = not(TYFA05) // 5 rung inverter, because fanout?
// ROXO = not(SEGU05)

//------------------------------------------------------------------------------

LcdSignals LcdRegisters::sig(const TestGB& gb) const {
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(CATU_VID_LINE_d4.q());
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n);
  /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4p);

  /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(RUTU_NEW_LINE_d0);
  /*p21.SELA*/ wire SELA_NEW_LINE_d0p = not(PURE_NEW_LINE_d0n);

  /*p28.ABAK*/ wire ABAK_VID_LINE_TRIG_d4p = or (ATEJ_VID_LINE_TRIG_d4p, rst_sig.AMYG_VID_RSTp);
  /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n = not(ABAK_VID_LINE_TRIG_d4p);
  /*p29.DYBA*/ wire DYBA_VID_LINE_TRIG_d4p = not(BYVA_VID_LINE_TRIG_d4n);

  return {
    .CATU_VID_LINE_d4 = CATU_VID_LINE_d4,
    .BYHA_VID_LINE_TRIG_d4n = BYHA_VID_LINE_TRIG_d4n,
    .ATEJ_VID_LINE_TRIG_d4p = ATEJ_VID_LINE_TRIG_d4p,
    .XAHY_VID_LINE_TRIG_d4n = XAHY_VID_LINE_TRIG_d4n,
    .BYVA_VID_LINE_TRIG_d4n = BYVA_VID_LINE_TRIG_d4n,
    .DYBA_VID_LINE_TRIG_d4p = DYBA_VID_LINE_TRIG_d4p,
    .PURE_NEW_LINE_d0n = PURE_NEW_LINE_d0n,
    .SELA_NEW_LINE_d0p = SELA_NEW_LINE_d0p,
  };
}

//------------------------------------------------------------------------------

void LcdRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto tim_sig = gb.tim_reg.sig(gb);
  auto lcd_sig = sig(gb);

  auto& cpu_bus = gb.cpu_bus;

  wire XONA_LCDC_EN = gb.cfg_reg.XONA_LCDC_EN.q();
  /*p24.KEDY*/ wire KEDY_LCDC_ENn = not(XONA_LCDC_EN);

  //----------------------------------------

  // LCD main timer
  {
    /*p21.MUDE*/ wire _MUDE_X_RSTn = nor(RUTU_NEW_LINE_d0.q(), rst_sig.LYHA_VID_RSTp);
    /*p21.SAXO*/ XEHO_X0.set(clk_sig.TALU_xBCDExxx, _MUDE_X_RSTn, !XEHO_X0.q());
    /*p21.TYPO*/ SAVY_X1.set(!XEHO_X0.q(),  _MUDE_X_RSTn, !SAVY_X1.q());
    /*p21.VYZO*/ XODU_X2.set(!SAVY_X1.q(),  _MUDE_X_RSTn, !XODU_X2.q());
    /*p21.TELU*/ XYDO_X3.set(!XODU_X2.q(),  _MUDE_X_RSTn, !XYDO_X3.q());
    /*p21.SUDE*/ TUHU_X4.set(!XYDO_X3.q(),  _MUDE_X_RSTn, !TUHU_X4.q());
    /*p21.TAHA*/ TUKY_X5.set(!TUHU_X4.q(),  _MUDE_X_RSTn, !TUKY_X5.q());
    /*p21.TYRY*/ TAKO_X6.set(!TUKY_X5.q(),  _MUDE_X_RSTn, !TAKO_X6.q());
  }

  {
    /*p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor(MYTA_LINE_153_d4.q(), rst_sig.LYHA_VID_RSTp);
    /*p21.MUWY*/ MUWY_Y0.set(RUTU_NEW_LINE_d0.q(), _LAMA_FRAME_RSTn, !MUWY_Y0.q());
    /*p21.MYRO*/ MYRO_Y1.set(!MUWY_Y0.q(),         _LAMA_FRAME_RSTn, !MYRO_Y1.q());
    /*p21.LEXA*/ LEXA_Y2.set(!MYRO_Y1.q(),         _LAMA_FRAME_RSTn, !LEXA_Y2.q());
    /*p21.LYDO*/ LYDO_Y3.set(!LEXA_Y2.q(),         _LAMA_FRAME_RSTn, !LYDO_Y3.q());
    /*p21.LOVU*/ LOVU_Y4.set(!LYDO_Y3.q(),         _LAMA_FRAME_RSTn, !LOVU_Y4.q());
    /*p21.LEMA*/ LEMA_Y5.set(!LOVU_Y4.q(),         _LAMA_FRAME_RSTn, !LEMA_Y5.q());
    /*p21.MATO*/ MATO_Y6.set(!LEMA_Y5.q(),         _LAMA_FRAME_RSTn, !MATO_Y6.q());
    /*p21.LAFO*/ LAFO_Y7.set(!MATO_Y6.q(),         _LAMA_FRAME_RSTn, !LAFO_Y7.q());
  }

  /*p21.XYVO*/ wire _XYVO_IN_VBLANKp = and(LOVU_Y4.q(), LAFO_Y7.q()); // 128 + 16 = 144
  {
    /*p21.POPU*/ POPU_VBLANK_d4.set(NYPE_NEW_LINE_d4.q(), rst_sig.LYFE_VID_RSTn, _XYVO_IN_VBLANKp);
  }

  {
    /*p21.SANU*/ wire _SANU_LINE_END = and(TAKO_X6.q(), TUKY_X5.q(), TUHU_X4.q(), XEHO_X0.q()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.RUTU*/ RUTU_NEW_LINE_d0.set(clk_sig.SONO_AxxxxFGH, rst_sig.LYFE_VID_RSTn, _SANU_LINE_END);
    /*p21.NYPE*/ NYPE_NEW_LINE_d4.set(clk_sig.TALU_xBCDExxx, rst_sig.LYFE_VID_RSTn, RUTU_NEW_LINE_d0.q());
  }

  {
    /*p29.ALES*/ wire _ALES_IN_VBLANKn = not(_XYVO_IN_VBLANKp);
    /*p29.ABOV*/ wire _ABOV_VID_LINE_d0  = and(lcd_sig.SELA_NEW_LINE_d0p, _ALES_IN_VBLANKn);
    /*p29.CATU*/ CATU_VID_LINE_d4.set (clk_sig.XUPY_xBCxxFGx,   rst_sig.ABEZ_VID_RSTn, _ABOV_VID_LINE_d0);
    /*p28.ANEL*/ ANEL_VID_LINE_d6.set (clk_sig.AWOH_AxxDExxH,   rst_sig.ABEZ_VID_RSTn, CATU_VID_LINE_d4.q());
  }

  {
    /*p21.NOKO*/ wire _NOKO_LINE_153 = and(LAFO_Y7.q(), LOVU_Y4.q(), LYDO_Y3.q(), MUWY_Y0.q()); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*p21.MYTA*/ MYTA_LINE_153_d4.set(NYPE_NEW_LINE_d4.q(), rst_sig.LYFE_VID_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)
    /*p24.LOFU*/ wire _LOFU_CLKn = not(RUTU_NEW_LINE_d0);
    /*p24.LUCA*/ LUCA_LINE_EVEN.set(_LOFU_CLKn, rst_sig.LYFE_VID_RSTn, !LUCA_LINE_EVEN.q());
    /*p21.NAPO*/ NAPO_FRAME_EVEN.set(POPU_VBLANK_d4.q(), rst_sig.LYFE_VID_RSTn, !NAPO_FRAME_EVEN.q());
    /*p24.MAGU*/ wire _MAGU = xor(NAPO_FRAME_EVEN, LUCA_LINE_EVEN.q());
    /*p24.MECO*/ wire _MECO = not(_MAGU);
    /*p24.KEBO*/ wire _KEBO = not(_MECO);
    /*p24.USEC*/ wire _USEC = not(tim_sig.UREK_DIV_07n);
    /*p24.KUPA*/ wire _KUPA = amux2(XONA_LCDC_EN, _KEBO, KEDY_LCDC_ENn, _USEC);
    /*p24.KOFO*/ wire _KOFO = not(_KUPA);
    FR.set(_KOFO);
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
    /*p21.SYGU*/ SYGU_LINE_STROBE.set(clk_sig.SONO_AxxxxFGH, rst_sig.LYFE_VID_RSTn, _TEGY_LINE_STROBE);
    /*p21.RYNO*/ wire _RYNO = or(_TEGY_LINE_STROBE, RUTU_NEW_LINE_d0);
    /*p21.POGU*/ wire _POGU = not(_RYNO);
    CPG.set(_POGU);
  }

  // LCD CPL pin
  {
    /*p24.KASA*/ wire _KASA_LINE_DONE = not(lcd_sig.PURE_NEW_LINE_d0n);
    /*p24.UMOB*/ wire _UMOB_DIV_06p = not(tim_sig.UMEK_DIV_06n);
    /*p24.KAHE*/ wire _KAHE = amux2(XONA_LCDC_EN, _KASA_LINE_DONE, KEDY_LCDC_ENn, _UMOB_DIV_06p);
    /*p24.KYMO*/ wire _KYMO = not(_KAHE);
    CPL.set(_KYMO);
  }

  // LCD vertical sync pin
  {
    /*p24.NERU*/ wire _LINE_000n = nor(MUWY_Y0, MYRO_Y1, LEXA_Y2, LYDO_Y3, LOVU_Y4, LEMA_Y5, MATO_Y6, LAFO_Y7);
    /*p24.MEDA*/ MEDA_VSYNC_OUTn.set(NYPE_NEW_LINE_d4, rst_sig.LYFE_VID_RSTn, _LINE_000n);
    /*p24.MURE*/ wire _MURE_PIN_S = not(MEDA_VSYNC_OUTn);
    S.set(_MURE_PIN_S);
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

    /*p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor (LAFO_Y7, RAHA_LYC7.qn());
    /*p21.TERY*/ wire _TERY_LY_MATCH6n = xor (MATO_Y6, VEVO_LYC6.qn());
    /*p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor (LEMA_Y5, VAFA_LYC5.qn());
    /*p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor (LOVU_Y4, SOTA_LYC4.qn());
    /*p21.RASY*/ wire _RASY_LY_MATCH3n = xor (LYDO_Y3, SALO_LYC3.qn());
    /*p21.REDA*/ wire _REDA_LY_MATCH2n = xor (LEXA_Y2, SEDY_LYC2.qn());
    /*p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor (MYRO_Y1, VUCE_LYC1.qn());
    /*p21.RYME*/ wire _RYME_LY_MATCH0n = xor (MUWY_Y0, SYRY_LYC0.qn());
    /*p21.SOVU*/ wire _SOVU_LY_MATCHA = nor(_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor
    /*p21.SUBO*/ wire _SUBO_LY_MATCHB = nor(_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor
    /*p21.RAPE*/ wire _RAPE_LY_MATCHn = nand(_SOVU_LY_MATCHA, _SUBO_LY_MATCHB); // def nand
    /*p21.PALY*/ wire _PALY_LY_MATCHa = not(_RAPE_LY_MATCHn); // def not
    /*p21.ROPO*/ ROPO_LY_MATCH_SYNCp.set(clk_sig.TALU_xBCDExxx, rst_sig.WESY_RSTn, _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire FF44n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p23.WAFU*/ wire FF44_RD = and (cpu_sig.ASOT_CPU_RD, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(MUWY_Y0.q());
    /*p23.XEPO*/ wire LY1n = not(MYRO_Y1.q());
    /*p23.MYFA*/ wire LY2n = not(LEXA_Y2.q());
    /*p23.XUHY*/ wire LY3n = not(LYDO_Y3.q());
    /*p23.WATA*/ wire LY4n = not(LOVU_Y4.q());
    /*p23.XAGA*/ wire LY5n = not(LEMA_Y5.q());
    /*p23.XUCE*/ wire LY6n = not(MATO_Y6.q());
    /*p23.XOWO*/ wire LY7n = not(LAFO_Y7.q());

    /*p23.VEGA*/ cpu_bus.TS_D0.set_tribuf(!FF44_RDn, not(LY0n));
    /*p23.WUVA*/ cpu_bus.TS_D1.set_tribuf(!FF44_RDn, not(LY1n));
    /*p23.LYCO*/ cpu_bus.TS_D2.set_tribuf(!FF44_RDn, not(LY2n));
    /*p23.WOJY*/ cpu_bus.TS_D3.set_tribuf(!FF44_RDn, not(LY3n));
    /*p23.VYNE*/ cpu_bus.TS_D4.set_tribuf(!FF44_RDn, not(LY4n));
    /*p23.WAMA*/ cpu_bus.TS_D5.set_tribuf(!FF44_RDn, not(LY5n));
    /*p23.WAVO*/ cpu_bus.TS_D6.set_tribuf(!FF44_RDn, not(LY6n));
    /*p23.WEZE*/ cpu_bus.TS_D7.set_tribuf(!FF44_RDn, not(LY7n));
  }

  // FF45 LYC
  {
    /*p22.WETY*/ wire FF45n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p23.XUFA*/ wire FF45_WR = and (cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF45);
    /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

    /*p23.SYRY*/ SYRY_LYC0.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D0);
    /*p23.VUCE*/ VUCE_LYC1.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D1);
    /*p23.SEDY*/ SEDY_LYC2.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D2);
    /*p23.SALO*/ SALO_LYC3.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D3);
    /*p23.SOTA*/ SOTA_LYC4.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D4);
    /*p23.VAFA*/ VAFA_LYC5.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D5);
    /*p23.VEVO*/ VEVO_LYC6.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D6);
    /*p23.RAHA*/ RAHA_LYC7.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TS_D7);
  }

  {
    /*p22.WETY*/ wire FF45n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p23.XYLY*/ wire FF45_RD = and (cpu_sig.ASOT_CPU_RD, FF45);
    /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);

    /*p23.RETU*/ cpu_bus.TS_D0.set_tribuf(!FF45_RDn, SYRY_LYC0.q());
    /*p23.VOJO*/ cpu_bus.TS_D1.set_tribuf(!FF45_RDn, VUCE_LYC1.q());
    /*p23.RAZU*/ cpu_bus.TS_D2.set_tribuf(!FF45_RDn, SEDY_LYC2.q());
    /*p23.REDY*/ cpu_bus.TS_D3.set_tribuf(!FF45_RDn, SALO_LYC3.q());
    /*p23.RACE*/ cpu_bus.TS_D4.set_tribuf(!FF45_RDn, SOTA_LYC4.q());
    /*p23.VAZU*/ cpu_bus.TS_D5.set_tribuf(!FF45_RDn, VAFA_LYC5.q());
    /*p23.VAFE*/ cpu_bus.TS_D6.set_tribuf(!FF45_RDn, VEVO_LYC6.q());
    /*p23.PUFY*/ cpu_bus.TS_D7.set_tribuf(!FF45_RDn, RAHA_LYC7.q());
  }

}

//------------------------------------------------------------------------------

bool LcdRegisters::commit() {
  bool changed = false;
  /*p21.SAXO*/ changed |= XEHO_X0.commit_reg(); // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ changed |= SAVY_X1.commit_reg();
  /*p21.VYZO*/ changed |= XODU_X2.commit_reg();
  /*p21.TELU*/ changed |= XYDO_X3.commit_reg();
  /*p21.SUDE*/ changed |= TUHU_X4.commit_reg();
  /*p21.TAHA*/ changed |= TUKY_X5.commit_reg();
  /*p21.TYRY*/ changed |= TAKO_X6.commit_reg();
  /*p21.MUWY*/ changed |= MUWY_Y0.commit_reg(); // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ changed |= MYRO_Y1.commit_reg();
  /*p21.LEXA*/ changed |= LEXA_Y2.commit_reg();
  /*p21.LYDO*/ changed |= LYDO_Y3.commit_reg();
  /*p21.LOVU*/ changed |= LOVU_Y4.commit_reg();
  /*p21.LEMA*/ changed |= LEMA_Y5.commit_reg();
  /*p21.MATO*/ changed |= MATO_Y6.commit_reg();
  /*p21.LAFO*/ changed |= LAFO_Y7.commit_reg();
  /*p21.RUTU*/ changed |= RUTU_NEW_LINE_d0.commit_reg(); // p909+8
  /*p29.CATU*/ changed |= CATU_VID_LINE_d4.commit_reg();  // p001+8
  /*p21.NYPE*/ changed |= NYPE_NEW_LINE_d4.commit_reg(); // p001+8
  /*p28.ANEL*/ changed |= ANEL_VID_LINE_d6.commit_reg();  // p003+8
  /*p21.MYTA*/ changed |= MYTA_LINE_153_d4.commit_reg();  // p153:001 - p000:001
  /*p21.POPU*/ changed |= POPU_VBLANK_d4.commit_reg();    // p144:001 - p000:001
  /*p21.SYGU*/ changed |= SYGU_LINE_STROBE.commit_reg();
  /*p24.MEDA*/ changed |= MEDA_VSYNC_OUTn.commit_reg();
  /*p24.LUCA*/ changed |= LUCA_LINE_EVEN.commit_reg();
  /*p21.NAPO*/ changed |= NAPO_FRAME_EVEN.commit_reg();

  /*p23.SYRY*/ changed |= SYRY_LYC0.commit_reg();
  /*p23.VUCE*/ changed |= VUCE_LYC1.commit_reg();
  /*p23.SEDY*/ changed |= SEDY_LYC2.commit_reg();
  /*p23.SALO*/ changed |= SALO_LYC3.commit_reg();
  /*p23.SOTA*/ changed |= SOTA_LYC4.commit_reg();
  /*p23.VAFA*/ changed |= VAFA_LYC5.commit_reg();
  /*p23.VEVO*/ changed |= VEVO_LYC6.commit_reg();
  /*p23.RAHA*/ changed |= RAHA_LYC7.commit_reg();

  /*p21.ROPO*/ changed |= ROPO_LY_MATCH_SYNCp.commit_reg();

  return changed;
}
