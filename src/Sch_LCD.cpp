#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

void LcdRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = ResetSignals::get(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);
  auto win_sig = gb.win_reg.sig(gb);

  wire XEHO_X0 = gb.ppu_reg.SAXO_X0;
  wire XYDO_X3 = gb.ppu_reg.TELU_X3;

  wire POKY_FRONT_PORCH_LATCH = gb.ppu_reg.POKY_FRONT_PORCH_LATCH;
  wire VOGA_RENDER_DONE_SYNC = gb.ppu_reg.VOGA_RENDER_DONE_SYNC;
  wire ROXY_FINE_MATCH_LATCHn = gb.ppu_reg.ROXY_FINE_MATCH_LATCHn;
  wire PUXA_FINE_MATCH_SYNC1 = gb.ppu_reg.PUXA_FINE_MATCH_SYNC1;
  wire NYZE_FINE_MATCH_SYNC2 = gb.ppu_reg.NYZE_FINE_MATCH_SYNC2;

  wire WODU_RENDER_DONE = gb.WODU_RENDER_DONE();

  wire MYVO_AxCxExGx = clk_sig.MYVO_AxCxExGx;

  wire XONA_LCDC_EN = gb.cfg_reg.XONA_LCDC_EN.q();


  /*p24.VYBO*/ wire VYBO_PIX_CLK_AxCxExGx = nor(gb.sst_reg.FEPO_STORE_MATCHp, WODU_RENDER_DONE, MYVO_AxCxExGx);
  /*p24.TYFA*/ wire TYFA_AxCxExGx = and (win_sig.SOCY_WIN_HITn, POKY_FRONT_PORCH_LATCH, VYBO_PIX_CLK_AxCxExGx);
  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);

  /*p21.WEGO*/ wire WEGO_RST_LATCH = or (rst_sig.TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);

  /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(RUTU_NEW_LINE_d0.q());
  /*p21.SELA*/ wire SELA_NEW_LINE_d0 = not(PURE_NEW_LINE_d0n);

  /*p24.KEDY*/ wire KEDY_LCDC_ENn = not(XONA_LCDC_EN);

  /*p01.UREK*/ wire UREK_DIV_07n = not(gb.tim_reg.TULU_DIV_07);
  /*p01.UMEK*/ wire UMEK_DIV_06n = not(gb.tim_reg.UGOT_DIV_06);

  // LCD main timer
  {
    /*p21.MUDE*/ wire _MUDE_X_RSTn = nor(RUTU_NEW_LINE_d0.q(), rst_sig.LYHA_VID_RSTp);
    /*p21.SAXO*/ SAXO_X0.set(clk_sig.TALU_xBCDExxx, _MUDE_X_RSTn, !SAXO_X0.q());
    /*p21.TYPO*/ TYPO_X1.set(!SAXO_X0.q(),  _MUDE_X_RSTn, !TYPO_X1.q());
    /*p21.VYZO*/ VYZO_X2.set(!TYPO_X1.q(),  _MUDE_X_RSTn, !VYZO_X2.q());
    /*p21.TELU*/ TELU_X3.set(!VYZO_X2.q(),  _MUDE_X_RSTn, !TELU_X3.q());
    /*p21.SUDE*/ SUDE_X4.set(!TELU_X3.q(),  _MUDE_X_RSTn, !SUDE_X4.q());
    /*p21.TAHA*/ TAHA_X5.set(!SUDE_X4.q(),  _MUDE_X_RSTn, !TAHA_X5.q());
    /*p21.TYRY*/ TYRY_X6.set(!TAHA_X5.q(),  _MUDE_X_RSTn, !TYRY_X6.q());
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

  {
    /*p21.SANU*/ wire _SANU_LINE_END = and(TYRY_X6.q(), TAHA_X5.q(), SUDE_X4.q(), SAXO_X0.q()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.RUTU*/ RUTU_NEW_LINE_d0.set(clk_sig.SONO_AxxxxFGH, rst_sig.LYFE_VID_RSTn, _SANU_LINE_END);
    /*p21.NYPE*/ NYPE_NEW_LINE_d4.set(clk_sig.TALU_xBCDExxx, rst_sig.LYFE_VID_RSTn, RUTU_NEW_LINE_d0.q());

    /*p21.XYVO*/ wire _XYVO_IN_VBLANK    = and(LOVU_Y4.q(), LAFO_Y7.q()); // 128 + 16 = 144
    /*p29.ALES*/ wire _ALES_IN_VBLANKn   = not(_XYVO_IN_VBLANK);
    /*p29.ABOV*/ wire _ABOV_VID_LINE_d0  = and(SELA_NEW_LINE_d0, _ALES_IN_VBLANKn);

    /*p21.POPU*/ POPU_IN_VBLANK_d4.set(NYPE_NEW_LINE_d4.q(), rst_sig.LYFE_VID_RSTn, _XYVO_IN_VBLANK);
    /*p29.CATU*/ CATU_VID_LINE_d4.set (clk_sig.XUPY_xBCxxFGx,   rst_sig.ABEZ_VID_RSTn, _ABOV_VID_LINE_d0);
    /*p28.ANEL*/ ANEL_VID_LINE_d6.set (clk_sig.AWOH_AxxDExxH,   rst_sig.ABEZ_VID_RSTn, CATU_VID_LINE_d4.q());

    /*p21.NOKO*/ wire _NOKO_LINE_153 = and(LAFO_Y7.q(), LOVU_Y4.q(), LYDO_Y3.q(), MUWY_Y0.q()); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*p21.MYTA*/ MYTA_LINE_153_d4.set(NYPE_NEW_LINE_d4.q(), rst_sig.LYFE_VID_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)
    /*p24.LOFU*/ wire _LOFU_CLKn = not(RUTU_NEW_LINE_d0);
    /*p24.LUCA*/ LUCA_LINE_EVEN.set(_LOFU_CLKn, rst_sig.LYFE_VID_RSTn, !LUCA_LINE_EVEN.q());
    /*p21.NAPO*/ NAPO_FRAME_EVEN.set(POPU_IN_VBLANK_d4.q(), rst_sig.LYFE_VID_RSTn, !NAPO_FRAME_EVEN.q());
    /*p24.MAGU*/ wire _MAGU = xor(NAPO_FRAME_EVEN, LUCA_LINE_EVEN.q());
    /*p24.MECO*/ wire _MECO = not(_MAGU);
    /*p24.KEBO*/ wire _KEBO = not(_MECO);
    /*p24.USEC*/ wire _USEC = not(UREK_DIV_07n);
    /*p24.KUPA*/ wire _KUPA = amux2(XONA_LCDC_EN, _KEBO, KEDY_LCDC_ENn, _USEC);
    /*p24.KOFO*/ wire _KOFO = not(_KUPA);
    FR.set(_KOFO);
  }

  // LCD line strobe
  {
    /*p21.TOCU*/ wire _C0n = not(SAXO_X0.q());
    /*p21.VEPE*/ wire _C1n = not(TYPO_X1.q());
    /*p21.VUTY*/ wire _C2n = not(VYZO_X2.q());
    /*p21.VATE*/ wire _C3n = not(TELU_X3.q());
    /*p21.TUDA*/ wire _C4n = not(SUDE_X4.q());
    /*p21.TAFY*/ wire _C5n = not(TAHA_X5.q());
    /*p21.TUJU*/ wire _C6n = not(TYRY_X6.q());

    /*p21.VOKU*/ wire _VOKU_000n = nand(_C6n, _C5n, _C4n, _C3n, _C2n, _C1n, _C0n); // 0000000 == 0
    /*p21.TOZU*/ wire _TOZU_007n = nand(_C6n, _C5n, _C4n, _C3n,  VYZO_X2,  TYPO_X1,  SAXO_X0); // 0000111 == 7
    /*p21.TECE*/ wire _TECE_045n = nand(_C6n,  TAHA_X5, _C4n,  TELU_X3,  VYZO_X2, _C1n,  SAXO_X0); // 0101101 == 45
    /*p21.TEBO*/ wire _TEBO_083n = nand( TYRY_X6, _C5n,  SUDE_X4, _C3n, _C2n,  TYPO_X1,  SAXO_X0); // 1010011 == 83

    /*p21.TEGY*/ wire _TEGY_LINE_STROBE = nand(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    /*p21.SYGU*/ SYGU_LINE_STROBE.set(clk_sig.SONO_AxxxxFGH, rst_sig.LYFE_VID_RSTn, _TEGY_LINE_STROBE);
    /*p21.RYNO*/ wire _RYNO = or (_TEGY_LINE_STROBE, RUTU_NEW_LINE_d0);
    /*p21.POGU*/ wire _POGU = not(_RYNO);
    CPG.set(_POGU);
  }

  // LCD CPL pin
  {
    /*p24.KASA*/ wire _KASA_LINE_DONE = not(PURE_NEW_LINE_d0n);
    /*p24.UMOB*/ wire _UMOB_DIV_06p = not(UMEK_DIV_06n);
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

  Signal SEPA_FF41_WR;
  {
    /*p22.WOFA*/ wire FF41n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.VARY*/ wire FF41 = not(FF41n);
    /*p21.SEPA*/ SEPA_FF41_WR = and (gb.CUPA_CPU_WR_xxxxxFGH(), FF41);
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

    /*p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor(LAFO_Y7, RAHA_LYC7.qn());
    /*p21.TERY*/ wire _TERY_LY_MATCH6n = xor(MATO_Y6, VEVO_LYC6.qn());
    /*p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor(LEMA_Y5, VAFA_LYC5.qn());
    /*p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor(LOVU_Y4, SOTA_LYC4.qn());
    /*p21.RASY*/ wire _RASY_LY_MATCH3n = xor(LYDO_Y3, SALO_LYC3.qn());
    /*p21.REDA*/ wire _REDA_LY_MATCH2n = xor(LEXA_Y2, SEDY_LYC2.qn());
    /*p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor(MYRO_Y1, VUCE_LYC1.qn());
    /*p21.RYME*/ wire _RYME_LY_MATCH0n = xor(MUWY_Y0, SYRY_LYC0.qn());
    /*p21.SOVU*/ wire _SOVU_LY_MATCHA = nor(_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor
    /*p21.SUBO*/ wire _SUBO_LY_MATCHB = nor(_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor
    /*p21.RAPE*/ wire _RAPE_LY_MATCHn = nand(_SOVU_LY_MATCHA, _SUBO_LY_MATCHB); // def nand
    /*p21.PALY*/ wire _PALY_LY_MATCHa = not(_RAPE_LY_MATCHn); // def not
    /*p21.ROPO*/ ROPO_LY_MATCH_SYNC.set(clk_sig.TALU_xBCDExxx, rst_sig.WESY_RSTn, _PALY_LY_MATCHa);

    // RUPO arms on ground side, nor latch
    // RUPO00 << ROPO16
    // RUPO01 nc
    // RUPO02 >> SEGO03
    // RUPO03 >> nc
    // RUPO04 nc
    // RUPO05 << PAGO03

    // when PAGO03 goes high, RUPO02 goes high
    // when ROPO16 goes high, RUPO02 goes low.

    /*p21.RYJU*/ wire _RYJU_FF41_WRn = not(SEPA_FF41_WR);
    /*p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = nor(rst_sig.WESY_RSTn, _RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST, ROPO_LY_MATCH_SYNC);
  }

  {
    // WUSA arms on the ground side, nor latch
    // WUSA00 << XAJO03
    // WUSA01 nc
    // WUSA02 >> nc
    // WUSA03 >> TOBA00
    // WUSA04 nc 
    // WUSA05 << WEGO03

    // When XAJO03 goes high, WUSA03 goes high.
    // When WEGO03 goes high, WUSA03 goes low.

    /*p21.XAJO*/ wire XAJO_X_009 = and (XEHO_X0, XYDO_X3);
    /*p21.WUSA*/ WUSA_CPEN_LATCH.nor_latch(XAJO_X_009, WEGO_RST_LATCH);


    /*p24.SACU*/ wire SACU_CLKPIPE_AxCxExGx = nor(SEGU_xBxDxFxH, ROXY_FINE_MATCH_LATCHn);
    /*p21.TOBA*/ wire TOBA = and (SACU_CLKPIPE_AxCxExGx, gb.lcd_reg.WUSA_CPEN_LATCH);
    /*p27.POVA*/ wire POVA_FINE_MATCH_TRIG = and (PUXA_FINE_MATCH_SYNC1, !NYZE_FINE_MATCH_SYNC2);
    /*p21.SEMU*/ wire SEMU_LCD_CPn = or (TOBA, POVA_FINE_MATCH_TRIG);
    /*p21.RYPO_LCD_CP*/ wire RYPO_LCD_CP = not(SEMU_LCD_CPn);

    CP.set(RYPO_LCD_CP);
  }

  {
    /*p35.REMY*/ wire _REMY_LD0n = not(gb.PATY_PIX_OUT_LO);
    /*p35.RAVO*/ wire _RAVO_LD1n = not(gb.PERO_PIX_OUT_HI);

    LD0.set(not(_REMY_LD0n));
    LD1.set(not(_RAVO_LD1n));
  }
}

//------------------------------------------------------------------------------

void TestGB::tick_lcd() {
  auto clk_sig = clk_reg.sig(*this);
  auto rst_sig = ResetSignals::get(*this);
  auto win_sig = win_reg.sig(*this);

  wire XONA_LCDC_EN = cfg_reg.XONA_LCDC_EN.q();

  /*p01.XODO*/ wire XODO_VID_RSTp = nand(rst_sig.XEBE_RSTn, XONA_LCDC_EN);
  /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
  /*p01.TOFU*/ wire TOFU_VID_RST = not(XAPO_VID_RSTn);

  wire XYDO_X3 = ppu_reg.TELU_X3;

  wire POKY_FRONT_PORCH_LATCH = ppu_reg.POKY_FRONT_PORCH_LATCH;
  wire XYMU_RENDERINGp  = ppu_reg.XYMU_RENDERINGp;

  wire WODU_RENDER_DONE = this->WODU_RENDER_DONE();

  wire AVAP_SCAN_DONE_d0_TRIGp = this->AVAP_SCAN_DONE_d0_TRIGp();


  /*p24.VYBO*/ wire VYBO_PIX_CLK_AxCxExGx = nor(sst_reg.FEPO_STORE_MATCHp, WODU_RENDER_DONE, clk_sig.MYVO_AxCxExGx);
  /*p24.TYFA*/ wire TYFA_AxCxExGx = and(win_sig.SOCY_WIN_HITn, POKY_FRONT_PORCH_LATCH, VYBO_PIX_CLK_AxCxExGx);
  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
  /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);


  // LCD horizontal sync pin latch
  {

    // Die trace:
    // VYBO = nor(FEPO04, WODU04, MYVO02)
    // TYFA = and(SOCY02, POKY04, VYBO04)
    // SEGU = not(TYFA05) // 5 rung inverter, because fanout?
    // ROXO = not(SEGU05)

    /*p24.PAHO*/ lcd_reg.PAHO_X_8_SYNC.set(ROXO_AxCxExGx, XYMU_RENDERINGp, XYDO_X3);

    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.
    /*p24.RUJU*/ lcd_reg.POFY_ST_LATCH.nor_latch(AVAP_SCAN_DONE_d0_TRIGp, lcd_reg.PAHO_X_8_SYNC || TOFU_VID_RST);

    /*p24.RUZE*/ wire RUZE_PIN_ST = not(lcd_reg.POFY_ST_LATCH);
    lcd_reg.ST.set(RUZE_PIN_ST);
  }

  lcd_reg.tick(*this);
}