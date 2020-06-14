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
  X = 0;
  Y = 0;
  LINE_153_d4 = 0;
  NEW_LINE_d0 = 0;
  NEW_LINE_d4 = 0;
  IN_VBLANK_d4 = 0;
  VSYNC_OUTn = 0;
}

//-----------------------------------------------------------------------------

void LCD::tick(const Req& req, Ack& ack) {

  if (req.read && req.addr == 0xFF44) {
    ack.addr = req.addr;
    ack.data = Y;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

#if 0

// PIN_S PIN_CPG PIN_ST, lcd regs
{
  // lcd horizontal sync pin
  /*p24.POFY*/ wire POFY = not(lcd_reg.RUJU);
  /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY);

  // lcd line strobe
  /*p21.TOCU*/ wire C0n = not(lcd_reg.X0);
  /*p21.VEPE*/ wire C1n = not(lcd_reg.X1);
  /*p21.VUTY*/ wire C2n = not(lcd_reg.X2);
  /*p21.VATE*/ wire C3n = not(lcd_reg.X3);
  /*p21.TUDA*/ wire C4n = not(lcd_reg.X4);
  /*p21.TAFY*/ wire C5n = not(lcd_reg.X5);
  /*p21.TUJU*/ wire C6n = not(lcd_reg.X6);

  /*p21.VOKU*/ wire CNT_000= = X == 0;
  /*p21.TOZU*/ wire CNT_007= = X == 7;
  /*p21.TECE*/ wire CNT_045= = X == 45
  /*p21.TEBO*/ wire CNT_083= = X == 83;


  /*p24.MEDA*/ lcd_reg.VSYNC_OUTn.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, LINE_000n);

  /*p21.SYGU*/ lcd_reg.LINE_STROBE.set(SONO_AxxxxFGH, LYFE_VID_RSTn, LINE_STROBE);
  /*p21.MYTA*/ lcd_reg.LINE_153_d4.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, LINE_153);
  /*p21.POPU*/ lcd_reg.POPU_IN_VBLANK_d4.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, IN_VBLANK);
  /*p21.NYPE*/ lcd_reg.NYPE_NEW_LINE_d4.set(TALU_xBCDExxx, LYFE_VID_RSTn, lcd_reg.RUTU_NEW_LINE_d0);
  /*p28.ANEL*/ lcd_reg.VID_LINE_d6.set(AWOH_AxxDExxH, ABEZ_VID_RSTn, lcd_reg.VID_LINE_d4);
  /*p29.CATU*/ lcd_reg.VID_LINE_d4.set(XUPY_xBCxxFGx, ABEZ_VID_RSTn, VID_LINE_d0);


  lcd_pins.S.set(not(VSYNC_OUTn));


  lcd_pins.ST.set(RUZE_PIN_ST);
  }

#endif

void LCD::tock(int phase, const Req& /*req*/, bool VID_RST) {
  
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

  /*p21.RYNO*/ bool CPGn = (X == 0) || (X == 7) || (X == 45) || (X == 83) || NEW_LINE_d0;
  /*p21.POGU*/ PIN_CPG = !CPGn;

#if 0
  /*p21.SEMU*/ wire CPn  = or(and(CLKPIPE_AxCxExGx, lcd_reg.CPEN_LATCH), FINE_MATCH_TRIG);
  lcd_pins.CP.set(not(CPn));
#endif


#if 0
  lcd_pins.ST.set(RUZE_PIN_ST);
  /*p24.POME*/ lcd_reg.POME.sr_latch(lcd_reg.X_8_SYNC || TOFU_VID_RST, AVAP_SCAN_DONE_d0_TRIG);   // Latch loop
  /*p24.RUJU*/ lcd_reg.RUJU.sr_latch(lcd_reg.X_8_SYNC || TOFU_VID_RST, AVAP_SCAN_DONE_d0_TRIG);
  /*p24.RUZE*/ wire RUZE_PIN_ST = lcd_reg.RUJU;
  lcd_pins.ST.set(RUZE_PIN_ST);
#endif

  /*p24.KYMO*/ PIN_CPL = !NEW_LINE_d0;

#if 0
  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)
    /*p24.LUCA*/ LINE_EVEN .set(!NEW_LINE_d0, LYFE_VID_RSTn, !lcd_reg.LINE_EVEN);
    /*p21.NAPO*/ FRAME_EVEN.set(IN_VBLANK_d4, LYFE_VID_RSTn, !lcd_reg.FRAME_EVEN);
    /*p24.KEBO*/ wire KEBO = xor(FRAME_EVEN, LINE_EVEN);
    /*p01.UREK*/ wire DIV_07n = not(tim_reg.DIV_07);
    /*p24.KUPA*/ wire FRn  = amux2(LCDC_EN, KEBO, !LCDC_EN, DIV_07);
    lcd_pins.FR.set(not(FRn));
  }
#endif

  /*p24.MURE*/ PIN_S   = !VSYNC_OUTn;


  if (VID_RST) reset();
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& /*d*/) {
  //text_painter.dprintf(" ----- LCD REG -----\n");

  /*
  dump(text, "LCD X ", X0, X1, X2, X3, X4, X5, X6);
  dump(text, "LCD Y ", Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7);

  dump_long(text, "RUTU_NEW_LINE_d0  ", RUTU_NEW_LINE_d0.a );
  dump_long(text, "VID_LINE_d4       ", VID_LINE_d4.a      );
  dump_long(text, "NYPE_NEW_LINE_d4  ", NYPE_NEW_LINE_d4.a );
  dump_long(text, "VID_LINE_d6       ", VID_LINE_d6.a      );
  dump_long(text, "LINE_153_d4       ", LINE_153_d4.a      );
  dump_long(text, "POPU_IN_VBLANK_d4 ", POPU_IN_VBLANK_d4.a);
  dump_long(text, "LINE_STROBE       ", LINE_STROBE.a      );
  dump_long(text, "X_8_SYNC          ", X_8_SYNC.a         );
  dump_long(text, "CPEN_LATCH        ", CPEN_LATCH.a       );
  dump_long(text, "POME              ", POME.a             );
  dump_long(text, "RUJU              ", RUJU.a             );
  dump_long(text, "LINE_EVEN         ", LINE_EVEN.a        );
  dump_long(text, "FRAME_EVEN        ", FRAME_EVEN.a       );
  */
  //text_painter.newline();
}

//-----------------------------------------------------------------------------
