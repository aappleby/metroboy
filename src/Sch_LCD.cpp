#include "Sch_LCD.h"
#include "TestGB.h"

using namespace Schematics;

// Die trace:
// VYBO = nor(FEPO04, WODU04, MYVO02)
// TYFA = and(SOCY02, POKY04, VYBO04)
// SEGU = not(TYFA05) // 5 rung inverter, because fanout?
// ROXO = not(SEGU05)

//------------------------------------------------------------------------------

LcdSignals LcdRegisters::sig(const TestGB& gb) const {
  LcdSignals sig;

  auto rst_sig = gb.rst_reg.sig(gb);

  /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(CATU_VID_LINE_d4.q());
  /*p28.BYHA*/ sig.BYHA_VID_LINE_TRIG_d4n = and (or (ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
  /*p28.ATEJ*/ sig.ATEJ_VID_LINE_TRIG_d4p = not(sig.BYHA_VID_LINE_TRIG_d4n);
  /*p27.XAHY*/ sig.XAHY_VID_LINE_TRIG_d4n = not(sig.ATEJ_VID_LINE_TRIG_d4p);

  /*p21.PURE*/ sig.PURE_NEW_LINE_d0n = not(RUTU_NEW_LINE_d0);
  /*p21.SELA*/ sig.SELA_NEW_LINE_d0p = not(sig.PURE_NEW_LINE_d0n);

  sig.CATU_VID_LINE_d4 = CATU_VID_LINE_d4;

  /*p28.ABAK*/ wire ABAK_VID_LINE_TRIG_d4p = or (sig.ATEJ_VID_LINE_TRIG_d4p, rst_sig.AMYG_VID_RSTp);
  /*p28.BYVA*/ sig.BYVA_VID_LINE_TRIG_d4n = not(ABAK_VID_LINE_TRIG_d4p);
  /*p29.DYBA*/ sig.DYBA_VID_LINE_TRIG_d4p = not(sig.BYVA_VID_LINE_TRIG_d4n);

  /*p21.PARU*/ sig.PARU_VBLANKp = not(POPU_VBLANK_d4.qn());
  /*p21.TOLU*/ sig.TOLU_VBLANKn = not(sig.PARU_VBLANKp);
  /*p21.VYPU*/ sig.VYPU_VBLANKp = not(sig.TOLU_VBLANKn);

  {
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
    /*p21.PALY*/ sig.PALY_LY_MATCHa  = not (RAPE_LY_MATCHn); // def not
  }

  sig.POPU_VBLANK_d4 = POPU_VBLANK_d4;
  sig.ROPO_LY_MATCH_SYNCp = ROPO_LY_MATCH_SYNCp;

  sig.MUWY_Y0 = MUWY_Y0;
  sig.MYRO_Y1 = MYRO_Y1;
  sig.LEXA_Y2 = LEXA_Y2;
  sig.LYDO_Y3 = LYDO_Y3;
  sig.LOVU_Y4 = LOVU_Y4;
  sig.LEMA_Y5 = LEMA_Y5;
  sig.MATO_Y6 = MATO_Y6;
  sig.LAFO_Y7 = LAFO_Y7;

  return sig;
}

//------------------------------------------------------------------------------

void LcdRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto tim_sig = gb.tim_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto lcd_sig = sig(gb);
  auto pxp_sig = gb.pxp_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;

  /*p24.KEDY*/ wire KEDY_LCDC_ENn = not(ppu_sig.XONA_LCDC_EN);

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
    LD0.set(not(pxp_sig.REMY_LD0n));
    LD1.set(not(pxp_sig.RAVO_LD1n));
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
    /*p24.KUPA*/ wire _KUPA = amux2(ppu_sig.XONA_LCDC_EN, _KEBO, KEDY_LCDC_ENn, _USEC);
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
    /*p24.KAHE*/ wire _KAHE = amux2(ppu_sig.XONA_LCDC_EN, _KASA_LINE_DONE, KEDY_LCDC_ENn, _UMOB_DIV_06p);
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

    /*p21.ROPO*/ ROPO_LY_MATCH_SYNCp.set(clk_sig.TALU_xBCDExxx, rst_sig.WESY_RSTn, lcd_sig.PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire FF44n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.XENO_A01n, cpu_sig.WALO_A02p, cpu_sig.XERA_A03n);
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

    /*p23.VEGA*/ cpu_bus.TRI_D0.set_tribuf(!FF44_RDn, not(LY0n));
    /*p23.WUVA*/ cpu_bus.TRI_D1.set_tribuf(!FF44_RDn, not(LY1n));
    /*p23.LYCO*/ cpu_bus.TRI_D2.set_tribuf(!FF44_RDn, not(LY2n));
    /*p23.WOJY*/ cpu_bus.TRI_D3.set_tribuf(!FF44_RDn, not(LY3n));
    /*p23.VYNE*/ cpu_bus.TRI_D4.set_tribuf(!FF44_RDn, not(LY4n));
    /*p23.WAMA*/ cpu_bus.TRI_D5.set_tribuf(!FF44_RDn, not(LY5n));
    /*p23.WAVO*/ cpu_bus.TRI_D6.set_tribuf(!FF44_RDn, not(LY6n));
    /*p23.WEZE*/ cpu_bus.TRI_D7.set_tribuf(!FF44_RDn, not(LY7n));
  }

  // FF45 LYC
  {
    /*p22.WETY*/ wire FF45n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.WADO_A00p, cpu_sig.XENO_A01n, cpu_sig.WALO_A02p, cpu_sig.XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);

    /*p23.XYLY*/ wire FF45_RD = and (cpu_sig.ASOT_CPU_RD, FF45);
    /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);

    /*p23.XUFA*/ wire FF45_WR = and (cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF45);
    /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

    /*p23.RETU*/ cpu_bus.TRI_D0.set_tribuf(!FF45_RDn, SYRY_LYC0.q());
    /*p23.VOJO*/ cpu_bus.TRI_D1.set_tribuf(!FF45_RDn, VUCE_LYC1.q());
    /*p23.RAZU*/ cpu_bus.TRI_D2.set_tribuf(!FF45_RDn, SEDY_LYC2.q());
    /*p23.REDY*/ cpu_bus.TRI_D3.set_tribuf(!FF45_RDn, SALO_LYC3.q());
    /*p23.RACE*/ cpu_bus.TRI_D4.set_tribuf(!FF45_RDn, SOTA_LYC4.q());
    /*p23.VAZU*/ cpu_bus.TRI_D5.set_tribuf(!FF45_RDn, VAFA_LYC5.q());
    /*p23.VAFE*/ cpu_bus.TRI_D6.set_tribuf(!FF45_RDn, VEVO_LYC6.q());
    /*p23.PUFY*/ cpu_bus.TRI_D7.set_tribuf(!FF45_RDn, RAHA_LYC7.q());

    /*p23.SYRY*/ SYRY_LYC0.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D0);
    /*p23.VUCE*/ VUCE_LYC1.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D1);
    /*p23.SEDY*/ SEDY_LYC2.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D2);
    /*p23.SALO*/ SALO_LYC3.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D3);
    /*p23.SOTA*/ SOTA_LYC4.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D4);
    /*p23.VAFA*/ VAFA_LYC5.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D5);
    /*p23.VEVO*/ VEVO_LYC6.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D6);
    /*p23.RAHA*/ RAHA_LYC7.set(FF45_WRn, rst_sig.WESY_RSTn, cpu_bus.TRI_D7);
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

  /* PIN_50 */ changed |= LD1.commit_pinout();
  /* PIN_51 */ changed |= LD0.commit_pinout();
  /* PIN_52 */ changed |= CPG.commit_pinout();
  /* PIN_55 */ changed |= CPL.commit_pinout();
  /* PIN_56 */ changed |= FR.commit_pinout();
  /* PIN_57 */ changed |= S.commit_pinout();

  return changed;
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
  CPG.dump(text_painter, "CPG ");
  CPL.dump(text_painter, "CPL ");
  FR.dump(text_painter, "FR  ");
  S.dump(text_painter, "S   ");
  text_painter.newline();
}

#endif