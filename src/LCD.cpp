#include "LCD.h"

#if 0
{
  //----------------------------------------
  // sch_lcd

  lcd_reg.LINE_153_d4.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, Y == 153);
  lcd_reg.NYPE_NEW_LINE_d4.set(TALU_xBCDExxx, LYFE_VID_RSTn, lcd_reg.RUTU_NEW_LINE_d0);
  lcd_reg.RUTU_NEW_LINE_d0.set(SONO_AxxxxFGH, LYFE_VID_RSTn, X == 113);

  if (posedge(TALU_xBCDExxx)) {
    X++;
  }

  if (posedge(lcd_reg.RUTU_NEW_LINE_d0)) {
    Y++;
  }

  if (lcd_reg.RUTU_NEW_LINE_d0) X = 0;
  if (lcd_reg.LINE_153_d4))     Y = 0;

  if (LYHA_VID_RST) { X = 0; Y = 0; }


  // ly match
  [this, TALU_xBCDExxx, WESY_RST] {
    /*p21.SYFU*/ wire LY_MATCH7 = xor(lcd_reg.Y7, cfg_reg.LYC7);
    /*p21.TERY*/ wire LY_MATCH6 = xor(lcd_reg.Y6, cfg_reg.LYC6);
    /*p21.TUCY*/ wire LY_MATCH5 = xor(lcd_reg.Y5, cfg_reg.LYC5);
    /*p21.TYKU*/ wire LY_MATCH4 = xor(lcd_reg.Y4, cfg_reg.LYC4);
    /*p21.RASY*/ wire LY_MATCH3 = xor(lcd_reg.Y3, cfg_reg.LYC3);
    /*p21.REDA*/ wire LY_MATCH2 = xor(lcd_reg.Y2, cfg_reg.LYC2);
    /*p21.TYDE*/ wire LY_MATCH1 = xor(lcd_reg.Y1, cfg_reg.LYC1);
    /*p21.RYME*/ wire LY_MATCH0 = xor(lcd_reg.Y0, cfg_reg.LYC0);
    /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
    /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);
    /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
    /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);
    /*p21.ROPO*/ vid_reg.ROPO_LY_MATCH_SYNC.set(TALU_xBCDExxx, WESY_RST, LY_MATCHa);
  }();

  /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);
  /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RST, FF41_WRn);  // schematic wrong, this is NOR
  /*p21.RUPO*/ vid_reg.RUPO_LATCH_LYC_MATCH.sr_latch(vid_reg.ROPO_LY_MATCH_SYNC, PAGO_LYC_MATCH_RST);

  // stat read
  [this, ASOT_BUS_RD_ABCDEFGH, CUPA_BUS_WR_ABCDxxxH, FF41n, WESY_RST, PARU_IN_VBLANK, ACYL_OAM_ADDR_PARSE] {
    /*p22.VARY*/ wire FF41 = not(FF41n);
    /*p21.SEPA*/ wire FF41_WR = and(CUPA_BUS_WR_ABCDxxxH, FF41);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(FF41_WR);

    /*p21.ROXE*/ vid_reg.INT_HBL_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D0);
    /*p21.RUFO*/ vid_reg.INT_VBL_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D1);
    /*p21.REFE*/ vid_reg.INT_OAM_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D2);
    /*p21.RUGU*/ vid_reg.INT_LYC_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D3);

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(vid_reg.XYMU_RENDERING_LATCH, PARU_IN_VBLANK);
    /*p21.XATY*/ wire STAT_MODE1n = nor(vid_reg.XYMU_RENDERING_LATCH, ACYL_OAM_ADDR_PARSE);
    /*p21.TOBE*/ wire FF41_RDa = and(ASOT_BUS_RD_ABCDEFGH, FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?. 

    /*p21.TEBY*/ cpu_pins.D0.set_tribuf(FF41_RDa, not(STAT_MODE0n));
    /*p21.WUGA*/ cpu_pins.D1.set_tribuf(FF41_RDa, not(STAT_MODE1n));
    /*p21.SEGO*/ cpu_pins.D2.set_tribuf(FF41_RDa, not(vid_reg.RUPO_LATCH_LYC_MATCH));
    /*p21.PUZO*/ cpu_pins.D3.set_tribuf(FF41_RDb, vid_reg.INT_HBL_EN);
    /*p21.POFO*/ cpu_pins.D4.set_tribuf(FF41_RDb, vid_reg.INT_VBL_EN);
    /*p21.SASY*/ cpu_pins.D5.set_tribuf(FF41_RDb, vid_reg.INT_OAM_EN);
    /*p21.POTE*/ cpu_pins.D6.set_tribuf(FF41_RDb, vid_reg.INT_LYC_EN);

    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);
    /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RST, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ vid_reg.RUPO_LATCH_LYC_MATCH.sr_latch(vid_reg.ROPO_LY_MATCH_SYNC, PAGO_LYC_MATCH_RST);
  }();


  {
    // uhhhh probably not ack_serial here either? wtf did i do?
    /*p21.TOLU*/ wire INT_VBLn = not(PARU_IN_VBLANK);
    /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu_pins.ACK_SERIAL);
    /*p02.SULO*/ wire SULO = or(cpu_pins.D3, FF0F_WRn);
    /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, cpu_pins.D3);
    /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, ALUR_RSTn);
    /*p21.TAPA*/ wire TAPA_INT_OAM = and(INT_VBLn, SELA_NEW_LINE_d0);
    /*p21.TARU*/ wire TARU_INT_HBL = and(INT_VBLn, WODU_RENDER_DONE);
    /*p21.SUKO*/ wire INT_STATb = amux4(vid_reg.INT_LYC_EN, vid_reg.ROPO_LY_MATCH_SYNC,
      vid_reg.INT_OAM_EN, TAPA_INT_OAM,
      vid_reg.INT_VBL_EN, PARU_IN_VBLANK, // polarity?
      vid_reg.INT_HBL_EN, TARU_INT_HBL);
    /*p21.TUVA*/ wire TUVA_INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ wire VOTY_INT_STAT  = not(TUVA_INT_STATn);
    /*p02.LALU*/ int_reg.FF0F_3.set(VOTY_INT_STAT,     FF0F_SET3, FF0F_RST3, FF0F_IN);
  }
}
#endif

//-----------------------------------------------------------------------------

void LCD::tick() {
}

//-----------------------------------------------------------------------------

void LCD::tock() {
}

//-----------------------------------------------------------------------------

void LCD::dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- LCD REG -----\n");

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
  text_painter.newline();
}

//-----------------------------------------------------------------------------
