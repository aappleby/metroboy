#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct RegLX {
  uint8_t get() const  { return pack_u8p(7, &SAXO_LX0p_evn); }

  wire PURE_LINE_ENDn_new_evn() const {
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_new_evn = not1(RUTU_x113p_g.qp_new());
    return _PURE_LINE_ENDn_new_evn;
  }

  void tock(wire XODO_VID_RSTp, wire TALU_xxCDEFxx_clkevn);

  /*p21.RUTU*/ DFF17 RUTU_x113p_g;  // xxxxxxGx
  /*p21.NYPE*/ DFF17 NYPE_x113p_c;  // xxCxxxxx

  /*p21.SAXO*/ DFF17 SAXO_LX0p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYPO*/ DFF17 TYPO_LX1p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.VYZO*/ DFF17 VYZO_LX2p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TELU*/ DFF17 TELU_LX3p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.SUDE*/ DFF17 SUDE_LX4p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TAHA*/ DFF17 TAHA_LX5p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYRY*/ DFF17 TYRY_LX6p_evn; // xxCxxxGx Ticks on C, reset on G
};

//-----------------------------------------------------------------------------

struct RegLY {
  uint8_t get() const  { return pack_u8p(8, &MUWY_LY0p_evn); }

  void tock(
    wire BUS_CPU_A[16],
    wire XODO_VID_RSTp,
    wire TEDO_CPU_RDp,
    wire NYPE_x113p_c_new,
    wire RUTU_x113p_g_new,
    BusOut BUS_CPU_D_out[8]);

  /*p21.MYTA*/ DFF17 MYTA_y153p_evn; // xxCxxxxH

  /*p21.MUWY*/ DFF17 MUWY_LY0p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MYRO*/ DFF17 MYRO_LY1p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEXA*/ DFF17 LEXA_LY2p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LYDO*/ DFF17 LYDO_LY3p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LOVU*/ DFF17 LOVU_LY4p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEMA*/ DFF17 LEMA_LY5p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MATO*/ DFF17 MATO_LY6p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LAFO*/ DFF17 LAFO_LY7p_evn;  // xxCxxxGx Ticks on G, reset on C
};

//-----------------------------------------------------------------------------
// XONA_LCDC_LCDENn is in GateBoyResetDebug

struct RegLCDC {

  //wire XONA_LCDC_LCDENp() const { return XONA_LCDC_LCDENn_h.qn_old(); }

  void tock(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    BusOut BUS_CPU_D_out[8]);

  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn_h;   // xxxxxxxH
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn_h;   // xxxxxxxH
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn_h; // xxxxxxxH
  /*p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPn_h;  // xxxxxxxH
  /*p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEn_h; // xxxxxxxH
  /*p23.WYMO*/ DFF9 WYMO_LCDC_WINENn_h;  // xxxxxxxH
  /*p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPn_h; // xxxxxxxH
  /*p23.WOKY*/ DFF9 XONA_LCDC_LCDENn_h;  // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct RegLYC {
  uint8_t get() const { return pack_u8n(8, &SYRY_LYC0n); }

  void tock(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TALU_xxCDEFxx_clkevn,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    const RegLY& reg_ly,
    BusOut BUS_CPU_D_out[8]);

  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp_c;   // xxCxxxxx

  /*p23.SYRY*/ DFF9 SYRY_LYC0n; // xxxxxxxH
  /*p23.VUCE*/ DFF9 VUCE_LYC1n; // xxxxxxxH
  /*p23.SEDY*/ DFF9 SEDY_LYC2n; // xxxxxxxH
  /*p23.SALO*/ DFF9 SALO_LYC3n; // xxxxxxxH
  /*p23.SOTA*/ DFF9 SOTA_LYC4n; // xxxxxxxH
  /*p23.VAFA*/ DFF9 VAFA_LYC5n; // xxxxxxxH
  /*p23.VEVO*/ DFF9 VEVO_LYC6n; // xxxxxxxH
  /*p23.RAHA*/ DFF9 RAHA_LYC7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct GateBoyLCD {

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp(wire XODO_VID_RSTp) const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn(wire XODO_VID_RSTp) const { return not1(ATEJ_LINE_RSTp(XODO_VID_RSTp)); }

  /*#p28.ANOM*/ wire ANOM_LINE_RSTn(wire XODO_VID_RSTp) const { return nor2(ATEJ_LINE_RSTp(XODO_VID_RSTp), ATAR_VID_RSTp(XODO_VID_RSTp)); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp(wire XODO_VID_RSTp) const { return  or2(ATEJ_LINE_RSTp(XODO_VID_RSTp), AMYG_VID_RSTp(XODO_VID_RSTp)); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp(wire XODO_VID_RSTp) const { return not1(ANOM_LINE_RSTn(XODO_VID_RSTp)); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn(wire XODO_VID_RSTp) const { return not1(ABAK_LINE_RSTp(XODO_VID_RSTp)); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp(wire XODO_VID_RSTp) const { return not1(BYVA_LINE_RSTn(XODO_VID_RSTp)); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn(wire XODO_VID_RSTp) const { return not1(BALU_LINE_RSTp(XODO_VID_RSTp)); }

  /*#p21.PARU*/ wire PARU_VBLANKp() const { return not1(POPU_VBLANKp_evn.qn_new()); }

  void tock(
    wire XODO_VID_RSTp_new_h,
    wire XUPY_ABxxEFxx_clk_evn,
    const RegLX& reg_lx,
    const RegLY& reg_ly);

  void set_pins(
    wire XODO_VID_RSTp,
    wire TALU_xxCDEFxx,
    wire TYFA_CLKPIPE_odd,
    wire XONA_LCDC_LCDENp,
    wire XYMU_RENDERINGp,
    wire AVAP_SCAN_DONE_TRIGp,
    wire XYDO_PX3p,
    wire TULU_DIV07p,
    wire REMY_LD0n,
    wire RAVO_LD1n,
    const RegLX& reg_lx,
    const RegLY& reg_ly);

  // H deltas are due to reg writes
  /*p29.CATU*/ DFF17 CATU_LINE_P000p_a;      // Axxxxxxx
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p_c;      // xxCxxxxx
  /*p21.POPU*/ DFF17 POPU_VBLANKp_evn;       // xxCxxxxH

  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE_evn;   // xxxxxxGH
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn_evn;    // xxCxxxxH
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp_evn;    // xxxxxxGH
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp_evn;   // xxCxxxxx

  // RUJU+POFY+POME form a nor latch
  /*p24.RUJU*/ Gate RUJU;                    // AxxxxFxx
  /*p24.POFY*/ Gate POFY;                    // AxxxxFxx
  /*p24.POME*/ Gate POME;                    // AxxxxFxx
  /*p24.PAHO*/ DFF17 PAHO_X_8_SYNC_odd;      // xBxDxFxH
  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE; // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?

  /*PIN_50*/ PinOut PIN_LCD_DATA1;
  /*PIN_51*/ PinOut PIN_LCD_DATA0;
  /*PIN_54*/ PinOut PIN_LCD_HSYNC_evn;
  /*PIN_56*/ PinOut PIN_LCD_FLIPS_evn;
  /*PIN_52*/ PinOut PIN_LCD_CNTRL_evn;
  /*PIN_55*/ PinOut PIN_LCD_LATCH_evn;
  /*PIN_53*/ PinOut PIN_LCD_CLOCK_xxx;
  /*PIN_57*/ PinOut PIN_LCD_VSYNC_evn;

  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];
};

//-----------------------------------------------------------------------------
