#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void reset_cart() {
    ROPO_LY_MATCH_SYNCp.reset(REG_D1C0);

    CATU_LINE_P000p.reset(REG_D0C1);
    NYPE_x113p.reset(REG_D0C0);
    ANEL_LINE_P002p.reset(REG_D0C0);
    RUTU_x113p.reset(REG_D0C1);

    MUWY_LY0p.reset(REG_D0C0);
    MYRO_LY1p.reset(REG_D0C1);
    LEXA_LY2p.reset(REG_D0C1);
    LYDO_LY3p.reset(REG_D0C1);
    LOVU_LY4p.reset(REG_D0C1);
    LEMA_LY5p.reset(REG_D0C1);
    MATO_LY6p.reset(REG_D0C1);
    LAFO_LY7p.reset(REG_D0C1);

    SAXO_LX0p.reset(REG_D0C0);
    TYPO_LX1p.reset(REG_D1C1);
    VYZO_LX2p.reset(REG_D0C0);
    TELU_LX3p.reset(REG_D0C1);
    SUDE_LX4p.reset(REG_D0C1);
    TAHA_LX5p.reset(REG_D1C1);
    TYRY_LX6p.reset(REG_D1C0);

    POPU_VBLANK_CLKp .reset(REG_D1C0);
    MYTA_y153p  .reset(REG_D1C1);
    SYGU_LINE_STROBE.reset(REG_D0C1);
    MEDA_VSYNC_OUTn .reset(REG_D0C1);
    LUCA_LINE_EVENp  .reset(REG_D1C1);
    NAPO_FRAME_EVENp .reset(REG_D0C1);

    SYRY_LYC0n_s.reset(REG_D1C1);
    VUCE_LYC1n_s.reset(REG_D1C1);
    SEDY_LYC2n_s.reset(REG_D1C1);
    SALO_LYC3n_s.reset(REG_D1C1);
    SOTA_LYC4n_s.reset(REG_D1C1);
    VAFA_LYC5n_s.reset(REG_D1C1);
    VEVO_LYC6n_s.reset(REG_D1C1);
    RAHA_LYC7n_s.reset(REG_D1C1);
  }

  void reset_boot() {
    ROPO_LY_MATCH_SYNCp.reset(REG_D0C0);

    CATU_LINE_P000p.reset(REG_D0C0);
    NYPE_x113p.reset(REG_D0C0);
    ANEL_LINE_P002p.reset(REG_D0C0);
    RUTU_x113p.reset(REG_D0C0);

    MUWY_LY0p.reset(REG_D0C0);
    MYRO_LY1p.reset(REG_D0C0);
    LEXA_LY2p.reset(REG_D0C0);
    LYDO_LY3p.reset(REG_D0C0);
    LOVU_LY4p.reset(REG_D0C0);
    LEMA_LY5p.reset(REG_D0C0);
    MATO_LY6p.reset(REG_D0C0);
    LAFO_LY7p.reset(REG_D0C0);
    SAXO_LX0p.reset(REG_D0C0);
    TYPO_LX1p.reset(REG_D0C0);
    VYZO_LX2p.reset(REG_D0C0);
    TELU_LX3p.reset(REG_D0C0);
    SUDE_LX4p.reset(REG_D0C0);
    TAHA_LX5p.reset(REG_D0C0);
    TYRY_LX6p.reset(REG_D0C0);

    POPU_VBLANK_CLKp.reset(REG_D0C0);
    MYTA_y153p.reset(REG_D0C0);
    SYGU_LINE_STROBE.reset(REG_D0C0);
    MEDA_VSYNC_OUTn.reset(REG_D0C0);
    LUCA_LINE_EVENp.reset(REG_D0C0);
    NAPO_FRAME_EVENp.reset(REG_D0C0);

    SYRY_LYC0n_s.reset(REG_D0C0);
    VUCE_LYC1n_s.reset(REG_D0C0);
    SEDY_LYC2n_s.reset(REG_D0C0);
    SALO_LYC3n_s.reset(REG_D0C0);
    SOTA_LYC4n_s.reset(REG_D0C0);
    VAFA_LYC5n_s.reset(REG_D0C0);
    VEVO_LYC6n_s.reset(REG_D0C0);
    RAHA_LYC7n_s.reset(REG_D0C0);
  }

  uint8_t get_lx() const  { return pack_u8p_old(7, &SAXO_LX0p); }
  uint8_t get_ly() const  { return pack_u8p_old(8, &MUWY_LY0p); }
  uint8_t get_lyc() const { return pack_u8n_old(8, &SYRY_LYC0n_s); }

  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp; // -> pix pipe for int stat, fires on P002, clears on the following P002
  /*p29.CATU*/ DFF17 CATU_LINE_P000p; // -> pix pipe, scanner.
  /*p21.NYPE*/ DFF17 NYPE_x113p;
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p; // -> pix pipe for BYHA?
  /*p21.RUTU*/ DFF17 RUTU_x113p; // -> pix pipe, could move PURE here.
  /*p21.POPU*/ DFF17 POPU_VBLANK_CLKp; // FIXME firee on line 144, phase 002. clears on line 000, phase 004 (the real line 000 not the stubby 000 @ 153)
  /*p21.MYTA*/ DFF17 MYTA_y153p;  // FIXME fires on line 153, phase 002. clears on line 000, phase 002
  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE;
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn;
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp;
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp;

  // Increments at P010 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ DFF17 SAXO_LX0p;
  /*p21.TYPO*/ DFF17 TYPO_LX1p;
  /*p21.VYZO*/ DFF17 VYZO_LX2p;
  /*p21.TELU*/ DFF17 TELU_LX3p;
  /*p21.SUDE*/ DFF17 SUDE_LX4p;
  /*p21.TAHA*/ DFF17 TAHA_LX5p;
  /*p21.TYRY*/ DFF17 TYRY_LX6p;

  // -> pix pipe, sprite scanner, vram bus. Increments at P910
  /*p21.MUWY*/ DFF17 MUWY_LY0p;
  /*p21.MYRO*/ DFF17 MYRO_LY1p;
  /*p21.LEXA*/ DFF17 LEXA_LY2p;
  /*p21.LYDO*/ DFF17 LYDO_LY3p;
  /*p21.LOVU*/ DFF17 LOVU_LY4p;
  /*p21.LEMA*/ DFF17 LEMA_LY5p;
  /*p21.MATO*/ DFF17 MATO_LY6p;
  /*p21.LAFO*/ DFF17 LAFO_LY7p;

  // FF45 - LYC
  /*p23.SYRY*/ DFF9 SYRY_LYC0n_s;
  /*p23.VUCE*/ DFF9 VUCE_LYC1n_s;
  /*p23.SEDY*/ DFF9 SEDY_LYC2n_s;
  /*p23.SALO*/ DFF9 SALO_LYC3n_s;
  /*p23.SOTA*/ DFF9 SOTA_LYC4n_s;
  /*p23.VAFA*/ DFF9 VAFA_LYC5n_s;
  /*p23.VEVO*/ DFF9 VEVO_LYC6n_s;
  /*p23.RAHA*/ DFF9 RAHA_LYC7n_s;
};

//-----------------------------------------------------------------------------
