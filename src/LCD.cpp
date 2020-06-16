#include "LCD.h"
#include "Constants.h"

#pragma warning(disable:4189)

bool posedge(bool& old_v, bool new_v) {
  bool ret = !old_v && new_v;
  old_v = new_v;
  return ret;
}

//-----------------------------------------------------------------------------

void LCD::reset() {
  X = 100;
  Y = 153;
  LINE_153_d4 = 0;
  NEW_LINE_d0 = 0;
  NEW_LINE_d4 = 0;
  IN_VBLANK_d4 = 0;
  VSYNC_OUTn = 0;
}

//-----------------------------------------------------------------------------

void LCD::tick(const Req& /*req*/, Ack& /*ack*/) {
  /*
  if (req.read && req.addr == 0xFF44) {
    ack.addr = req.addr;
    ack.data = Y;
    ack.read++;
  }
  */
}

//-----------------------------------------------------------------------------

#if 0

/*p01.UNUT*/ wire TIMEOUT  = and (rst_reg.WAITING_FOR_CLKREQ, tim_reg.DIV_15);
/*p01.AVOR*/ wire AVOR_RST = or (rst_reg.RESET_REG, UNOR_MODE_DBG2, UMUT_MODE_DBG1, TIMEOUT, sys_pins.RST);
/*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RST);   // this goes all over the place
/*p01.LYFE*/ wire LYFE_VID_RSTn = and (ALUR_RSTn, cfg_reg.LCDC_EN);

//----------------------------------------
// sch_lcd

/*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(lcd_reg.RUTU_NEW_LINE_d0);
/*p21.SELA*/ wire SELA_NEW_LINE_d0 = not(PURE_NEW_LINE_d0n);
/*p24.KASA*/ wire LINE_DONEb = not(PURE_NEW_LINE_d0n);

/*p28.ABAF*/   wire VID_LINE_d4n = not(lcd_reg.VID_LINE_d4);
/*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.VID_LINE_d6, VID_LINE_d4n), ABEZ_VID_RSTn);
/*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4 = not(BYHA_VID_LINE_TRIG_d4n);

// Vertical sync
/*p24.NERU*/ wire LINE_000n = nor(lcd_reg.Y0, lcd_reg.Y1, lcd_reg.Y2, lcd_reg.Y3, lcd_reg.Y4, lcd_reg.Y5, lcd_reg.Y6, lcd_reg.Y7);

// PIN_CPL
{
  /*p24.KEDY*/ wire LCDC_ENn = not(cfg_reg.LCDC_EN);
  /*p01.UMEK*/ wire DIV_06n = not(tim_reg.DIV_06);
  /*p24.UMOB*/ wire DIV_06 = not(DIV_06n);
  /*p24.KAHE*/ wire CPLn = amux2(cfg_reg.LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
  /*p24.KYMO*/ wire KYMO = not(CPLn);
  lcd_pins.CPL.set(KYMO);
}

// so this is like a strobe that fires 4x per line


/*p21.MUDE*/ wire X_RSTn = nor(lcd_reg.RUTU_NEW_LINE_d0, LYHA_VID_RST);
/*p21.LAMA*/ wire FRAME_RSTn = nor(lcd_reg.LINE_153_d4, LYHA_VID_RST);

/*p21.SANU*/ wire LINE_END = and (lcd_reg.X6, lcd_reg.X5, lcd_reg.X4, lcd_reg.X0); // 113 = 64 + 32 + 16 + 1, schematic is wrong
/*p21.NOKO*/ wire LINE_153 = and (lcd_reg.Y7, lcd_reg.Y4, lcd_reg.Y3, lcd_reg.Y0); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
/*p21.XYVO*/ wire IN_VBLANK = and (lcd_reg.Y4, lcd_reg.Y7); // 128 + 16 = 144

/*p28.AWOH*/ wire AWOH_AxxDExxH = not(XUPY_xBCxxFGx);  // lcd

/*p29.ALES*/   wire IN_VBLANKn = not(IN_VBLANK);
/*p29.ABOV*/ wire VID_LINE_d0 = and (SELA_NEW_LINE_d0, IN_VBLANKn);

/*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4, ATAR_VID_RST);
/*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
/*p29.BEBU*/   wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, spr_reg.SCAN_DONE_d5, !spr_reg.SCAN_DONE_d4);
/*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);




#endif

void LCD::tock(int phase, const Req& /*req*/, bool LCDC_EN) {
  
  if (PHASE_B) {
    bool posedge_line_d4 = posedge(NEW_LINE_d4, NEW_LINE_d0);

    /*p21.MYTA*/ LINE_153_d4 = Y == 153;
    /*p21.POPU*/ IN_VBLANK_d4 = Y >= 144;

    X = NEW_LINE_d0 ? 0 : X + 1;

    VSYNC_OUTn = Y != 0;
  }

  if (PHASE_F) {
    /*p21.RUTU*/ NEW_LINE_d0 = X == 113;

    if (NEW_LINE_d0) {
      Y++;
      if (LINE_153_d4) Y = 0;
    }

  }

  if (!LCDC_EN) {
    X = 0;
    Y = 0;
    LINE_153_d4 = 0;
    NEW_LINE_d0 = 0;
    NEW_LINE_d4 = 0;
    IN_VBLANK_d4 = 0;
    VSYNC_OUTn = 0;
  }
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& d) {
  sprintf(d, "\002--------------LCD--------------\001\n");

  sprintf(d, "X            = %d\n", X);
  sprintf(d, "Y            = %d\n", Y);
  sprintf(d, "LY           = %d\n", LY);
  sprintf(d, "LYC          = %d\n", LYC);
  sprintf(d, "STAT         = %d\n", STAT);
  sprintf(d, "NEW_LINE_d0  = %d\n", NEW_LINE_d0);
  sprintf(d, "NEW_LINE_d4  = %d\n", NEW_LINE_d4);
  sprintf(d, "LINE_153_d4  = %d\n", LINE_153_d4);
  sprintf(d, "IN_VBLANK_d4 = %d\n", IN_VBLANK_d4);
  sprintf(d, "VSYNC_OUTn   = %d\n", VSYNC_OUTn);
  sprintf(d, "LINE_EVEN    = %d\n", LINE_EVEN);
  sprintf(d, "FRAME_EVEN   = %d\n", FRAME_EVEN);
}

//-----------------------------------------------------------------------------
