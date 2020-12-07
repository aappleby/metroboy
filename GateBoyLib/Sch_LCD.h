#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void reset_cart() {
    ROPO_LY_MATCH_SYNCp_evn.reset(REG_D1C0);

    CATU_LINE_P000p_evn.reset(REG_D0C1);
    NYPE_x113p_evn.reset(REG_D0C0);
    ANEL_LINE_P002p_evn.reset(REG_D0C0);
    RUTU_x113p_evn.reset(REG_D0C1);

    MUWY_LY0p_evn.reset(REG_D0C0);
    MYRO_LY1p_evn.reset(REG_D0C1);
    LEXA_LY2p_evn.reset(REG_D0C1);
    LYDO_LY3p_evn.reset(REG_D0C1);
    LOVU_LY4p_evn.reset(REG_D0C1);
    LEMA_LY5p_evn.reset(REG_D0C1);
    MATO_LY6p_evn.reset(REG_D0C1);
    LAFO_LY7p_evn.reset(REG_D0C1);

    SAXO_LX0p_evn.reset(REG_D0C0);
    TYPO_LX1p_evn.reset(REG_D1C1);
    VYZO_LX2p_evn.reset(REG_D0C0);
    TELU_LX3p_evn.reset(REG_D0C1);
    SUDE_LX4p_evn.reset(REG_D0C1);
    TAHA_LX5p_evn.reset(REG_D1C1);
    TYRY_LX6p_evn.reset(REG_D1C0);

    POPU_VBLANKp_evn .reset(REG_D1C0);
    MYTA_y153p_evn  .reset(REG_D1C1);
    SYGU_LINE_STROBE_evn.reset(REG_D0C1);
    MEDA_VSYNC_OUTn_evn .reset(REG_D0C1);
    LUCA_LINE_EVENp_evn  .reset(REG_D1C1);
    NAPO_FRAME_EVENp_evn .reset(REG_D0C1);

    SYRY_LYC0n.reset(REG_D1C1);
    VUCE_LYC1n.reset(REG_D1C1);
    SEDY_LYC2n.reset(REG_D1C1);
    SALO_LYC3n.reset(REG_D1C1);
    SOTA_LYC4n.reset(REG_D1C1);
    VAFA_LYC5n.reset(REG_D1C1);
    VEVO_LYC6n.reset(REG_D1C1);
    RAHA_LYC7n.reset(REG_D1C1);
  }

  void reset_boot() {
    ROPO_LY_MATCH_SYNCp_evn.reset(REG_D0C0);

    CATU_LINE_P000p_evn.reset(REG_D0C0);
    NYPE_x113p_evn.reset(REG_D0C0);
    ANEL_LINE_P002p_evn.reset(REG_D0C0);
    RUTU_x113p_evn.reset(REG_D0C0);

    MUWY_LY0p_evn.reset(REG_D0C0);
    MYRO_LY1p_evn.reset(REG_D0C0);
    LEXA_LY2p_evn.reset(REG_D0C0);
    LYDO_LY3p_evn.reset(REG_D0C0);
    LOVU_LY4p_evn.reset(REG_D0C0);
    LEMA_LY5p_evn.reset(REG_D0C0);
    MATO_LY6p_evn.reset(REG_D0C0);
    LAFO_LY7p_evn.reset(REG_D0C0);

    SAXO_LX0p_evn.reset(REG_D0C0);
    TYPO_LX1p_evn.reset(REG_D0C0);
    VYZO_LX2p_evn.reset(REG_D0C0);
    TELU_LX3p_evn.reset(REG_D0C0);
    SUDE_LX4p_evn.reset(REG_D0C0);
    TAHA_LX5p_evn.reset(REG_D0C0);
    TYRY_LX6p_evn.reset(REG_D0C0);

    POPU_VBLANKp_evn.reset(REG_D0C0);
    MYTA_y153p_evn.reset(REG_D0C0);
    SYGU_LINE_STROBE_evn.reset(REG_D0C0);
    MEDA_VSYNC_OUTn_evn.reset(REG_D0C0);
    LUCA_LINE_EVENp_evn.reset(REG_D0C0);
    NAPO_FRAME_EVENp_evn.reset(REG_D0C0);

    SYRY_LYC0n.reset(REG_D0C0);
    VUCE_LYC1n.reset(REG_D0C0);
    SEDY_LYC2n.reset(REG_D0C0);
    SALO_LYC3n.reset(REG_D0C0);
    SOTA_LYC4n.reset(REG_D0C0);
    VAFA_LYC5n.reset(REG_D0C0);
    VEVO_LYC6n.reset(REG_D0C0);
    RAHA_LYC7n.reset(REG_D0C0);
  }

  uint8_t get_lx() const  { return pack_u8p(7, &SAXO_LX0p_evn); }
  uint8_t get_ly() const  { return pack_u8p(8, &MUWY_LY0p_evn); }
  uint8_t get_lyc() const { return pack_u8n(8, &SYRY_LYC0n); }

  // H deltas are due to reg writes
  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp_evn; // xxCxxxxx
  /*p29.CATU*/ DFF17 CATU_LINE_P000p_evn;     // Axxxxxxx
  /*p21.NYPE*/ DFF17 NYPE_x113p_evn;          // xxCxxxxx
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p_evn;     // xxCxxxxx
  /*p21.RUTU*/ DFF17 RUTU_x113p_evn;          // xxxxxxGx
  /*p21.POPU*/ DFF17 POPU_VBLANKp_evn;        // xxCxxxxH
  /*p21.MYTA*/ DFF17 MYTA_y153p_evn;          // xxCxxxxH
  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE_evn;    // xxxxxxGH
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn_evn;     // xxCxxxxH
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp_evn;     // xxxxxxGH
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp_evn;    // xxCxxxxx

  // Increments at P010 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ DFF17 SAXO_LX0p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYPO*/ DFF17 TYPO_LX1p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.VYZO*/ DFF17 VYZO_LX2p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TELU*/ DFF17 TELU_LX3p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.SUDE*/ DFF17 SUDE_LX4p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TAHA*/ DFF17 TAHA_LX5p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYRY*/ DFF17 TYRY_LX6p_evn; // xxCxxxGx Ticks on C, reset on G

  // -> pix pipe, sprite scanner, vram bus. Increments at P910
  /*p21.MUWY*/ DFF17 MUWY_LY0p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.MYRO*/ DFF17 MYRO_LY1p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LEXA*/ DFF17 LEXA_LY2p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LYDO*/ DFF17 LYDO_LY3p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LOVU*/ DFF17 LOVU_LY4p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LEMA*/ DFF17 LEMA_LY5p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.MATO*/ DFF17 MATO_LY6p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LAFO*/ DFF17 LAFO_LY7p_evn; // xxCxxxGx Ticks on G, reset on C

  // FF45 - LYC
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
