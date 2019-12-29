#include "Sch_LCD.h"
#include "Schematics.h"
#include <memory.h>

#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Registers.h"
#include "Sch_Timer.h"

#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void LCD::tick_slow(const ClockSignals1& clk_sig1,
                    const ClockSignals2& clk_sig2,
                    const ResetSignals1& rst_sig,
                    const LCD& lcd,
                    const Video& vid,
                    
                    bool SCAN_DONE_d0_TRIG,
                    bool DIV_06n,
                    bool DIV_07n,
                    bool LCDC_EN,
                    LCD& next)
{
  /*p01.LYHA*/ wire VID_RESET2  = not(rst_sig.VID_RESETn);
  /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
  /*p01.ATAR*/ wire VID_RESET6  = not(rst_sig.VID_RESETn);
  /*p01.AMYG*/ wire VID_RESET7  = not(rst_sig.VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  /*p21.MUDE*/ wire X_RSTn = nor(lcd.NEW_LINE_d0a, VID_RESET2);

  // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p21.SANU*/ wire LINE_END = and(lcd.X6, lcd.X5, lcd.X4, lcd.X0);
  /*p21.PURE*/ wire NEW_LINE_d0n = not(lcd.NEW_LINE_d0a);
  /*p21.SELA*/ wire NEW_LINE_d0b = not(NEW_LINE_d0n);
  /*p21.NOKO*/ wire LINE_153_d0 = and(lcd.Y7, lcd.Y4, lcd.Y3, lcd.Y0); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*p21.LAMA*/ wire FRAME_RSTn = nor(lcd.LINE_153_d4, VID_RESET2);
  /*p21.XYVO*/ wire LINE_144_d0  = and(lcd.Y4, lcd.Y7); // 128 + 16 = 144
  /*p29.ALES*/ wire LINE_144_d0n = not(LINE_144_d0);
  /*p21.PARU*/ next.VBLANK_d4b = not(!lcd.VBLANK_d4);
  /*p28.ABAF*/ wire VID_LINE_d4n  = not(lcd.VID_LINE_d4);
  /*p29.ABOV*/ wire VID_LINE_d0 = and(NEW_LINE_d0b, LINE_144_d0n);
  /*p28.BYHA*/ next.VID_LINE_TRIG_d4n = and(or(lcd.VID_LINE_d6, VID_LINE_d4n), VID_RESETn3);
  /*p28.ATEJ*/ next.VID_LINE_TRIG_d4a = not(lcd.VID_LINE_TRIG_d4n);
  /*p27.XAHY*/ next.VID_LINE_TRIG_d4o = not(lcd.VID_LINE_TRIG_d4a);
  /*p28.ABAK*/ wire VID_LINE_TRIG_d4b = or (lcd.VID_LINE_TRIG_d4a, VID_RESET7);
  /*p28.BYVA*/ next.VID_LINE_TRIG_d4p = not(VID_LINE_TRIG_d4b);
  /*p29.DYBA*/ next.VID_LINE_TRIG_d4c = not(lcd.VID_LINE_TRIG_d4p);


  /*p21.SAXO*/ next.X0.tock(clk_sig2.TALU_xxxxEFGH, X_RSTn, !lcd.X0);
  /*p21.TYPO*/ next.X1.tock(!lcd.X0,           X_RSTn, !lcd.X1);
  /*p21.VYZO*/ next.X2.tock(!lcd.X1,           X_RSTn, !lcd.X2);
  /*p21.TELU*/ next.X3.tock(!lcd.X2,           X_RSTn, !lcd.X3);
  /*p21.SUDE*/ next.X4.tock(!lcd.X3,           X_RSTn, !lcd.X4);
  /*p21.TAHA*/ next.X5.tock(!lcd.X4,           X_RSTn, !lcd.X5);
  /*p21.TYRY*/ next.X6.tock(!lcd.X5,           X_RSTn, !lcd.X6);

  /*p21.RUTU*/ next.NEW_LINE_d0a.tock(clk_sig2.SONO_ABCDxxxx, VID_RESETn2, LINE_END);
  /*p21.NYPE*/ next.NEW_LINE_d4a.tock(clk_sig2.TALU_xxxxEFGH, VID_RESETn2, lcd.NEW_LINE_d0a);
  /*p21.MYTA*/ next.LINE_153_d4.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_153_d0);

  /*p21.MUWY*/ next.Y0.tock(lcd.NEW_LINE_d0a, FRAME_RSTn, !lcd.Y0);
  /*p21.MYRO*/ next.Y1.tock(!lcd.Y0,              FRAME_RSTn, !lcd.Y1);
  /*p21.LEXA*/ next.Y2.tock(!lcd.Y1,              FRAME_RSTn, !lcd.Y2);
  /*p21.LYDO*/ next.Y3.tock(!lcd.Y2,              FRAME_RSTn, !lcd.Y3);
  /*p21.LOVU*/ next.Y4.tock(!lcd.Y3,              FRAME_RSTn, !lcd.Y4);
  /*p21.LEMA*/ next.Y5.tock(!lcd.Y4,              FRAME_RSTn, !lcd.Y5);
  /*p21.MATO*/ next.Y6.tock(!lcd.Y5,              FRAME_RSTn, !lcd.Y6);
  /*p21.LAFO*/ next.Y7.tock(!lcd.Y6,              FRAME_RSTn, !lcd.Y7);

  /*p21.POPU*/ next.VBLANK_d4.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_144_d0);
  
  /*p29.CATU*/ next.VID_LINE_d4.tock(clk_sig2.XUPY_ABxxEFxx, VID_RESETn3, VID_LINE_d0);
  /*p28.ANEL*/ next.VID_LINE_d6.tock(clk_sig2.AWOH_xxCDxxGH, VID_RESETn3, lcd.VID_LINE_d4);

  {
    wire C0 = lcd.X0;
    wire C1 = lcd.X1;
    wire C2 = lcd.X2;
    wire C3 = lcd.X3;
    wire C4 = lcd.X4;
    wire C5 = lcd.X5;
    wire C6 = lcd.X6;

    /*p21.TOCU*/ wire C0n = not(C0);
    /*p21.VEPE*/ wire C1n = not(C1);
    /*p21.VUTY*/ wire C2n = not(C2);
    /*p21.VATE*/ wire C3n = not(C3);
    /*p21.TUDA*/ wire C4n = not(C4);
    /*p21.TAFY*/ wire C5n = not(C5);
    /*p21.TUJU*/ wire C6n = not(C6);

    // so this is like a strobe that fires 4x per line
    /*p21.VOKU*/ wire CNT_000n = nand(C6n, C5n, C4n, C3n, C2n, C1n, C0n); // 0000000 == 0
    /*p21.TOZU*/ wire CNT_007n = nand(C6n, C5n, C4n, C3n, C2,  C1,  C0 ); // 0000111 == 7
    /*p21.TECE*/ wire CNT_045n = nand(C6n, C5,  C4n, C3,  C2,  C1n, C0 ); // 0101101 == 45
    /*p21.TEBO*/ wire CNT_083n = nand(C6,  C5n, C4,  C3n, C2n, C1,  C0 ); // 1010011 == 83

    /*p21.TEGY*/ wire LINE_STROBEa = nand(CNT_000n, CNT_007n, CNT_045n, CNT_083n);

    /*p21.SYGU*/ next.LINE_STROBE.tock(clk_sig2.SONO_ABCDxxxx, VID_RESETn2, LINE_STROBEa);

    /*p21.RYNO*/ wire CPGn = or(lcd.LINE_STROBE, lcd.NEW_LINE_d0a);
    /*p21.POGU*/ wire POGU = not(CPGn);
    next.PIN_CPG = POGU;
  }


  {
    /*p01.TOFU*/ wire VID_RESET3  = not(rst_sig.VID_RESETn);
    /*p21.WEGO*/ wire WEGO  = or(VID_RESET3, vid.RENDER_DONE_SYNC);
    /*p21.XAJO*/ wire X_009 = and(vid.X0, vid.X3);

    // weird gate
    /*p21.WUSA*/ if (X_009) next.CPEN_LATCH = 1;
    /*p21.WUSA*/ if (WEGO)  next.CPEN_LATCH = 0;

    /*p21.TOBA*/ wire TOBA = and(vid.CLKPIPE, lcd.CPEN_LATCH);
    /*p21.SEMU*/ wire CPn  = or(TOBA, vid.FINE_MATCH_TRIG);
    /*p21.RYPO*/ wire RYPO = not(CPn);
    next.PIN_CP = RYPO;
  }

  {
    // Horizontal sync

    /*p01.TOFU*/ wire VID_RESET3  = not(rst_sig.VID_RESETn);

    // Latch loop
    ///*p24.POME*/ wire POME = nor(SCAN_DONE_d0_TRIG, POFY);
    ///*p24.RUJU*/ wire RUJU = or(X_8_SYNC, VID_RESET3, POME);
    ///*p24.POFY*/ wire POFY = not(RUJU);

    if (lcd.X_8_SYNC || VID_RESET3) {
      next.RUJU = true;
      next.POME = true;
    }

    if (SCAN_DONE_d0_TRIG) {
      next.RUJU = false;
      next.POME = false;
    }

    /*p24.POFY*/ wire POFY = not(lcd.RUJU);
    /*p24.RUZE*/ wire RUZE = not(POFY);
    /*p24.PAHO*/ next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);
    next.PIN_ST = RUZE;
  }

  {
    /*p24.KEDY*/ wire LCDC_ENn = not(LCDC_EN);
    /*p21.PURE*/ wire LINE_DONEn = not(lcd.NEW_LINE_d0a);
    /*p24.KASA*/ wire LINE_DONEb = not(LINE_DONEn);
    /*p24.UMOB*/ wire DIV_06 = not(DIV_06n);
    /*p24.KAHE*/ wire CPLn = amux2(LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
    /*p24.KYMO*/ wire KYMO = not(CPLn);
    next.PIN_CPL = KYMO;
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)

    /*p24.KEDY*/ wire LCDC_ENn = not(LCDC_EN);
    /*p24.LOFU*/ wire CLK_LINE_EVEN = not(lcd.NEW_LINE_d0a);
    /*p24.LUCA*/ next.LINE_EVEN.tock(CLK_LINE_EVEN, VID_RESETn2, !lcd.LINE_EVEN);
    /*p21.NAPO*/ next.FRAME_EVEN.tock(lcd.VBLANK_d4, VID_RESETn2, !lcd.FRAME_EVEN);

    /*p24.MAGU*/ wire MAGU = xor(lcd.FRAME_EVEN, lcd.LINE_EVEN);
    /*p24.MECO*/ wire MECO = not(MAGU);
    /*p24.KEBO*/ wire KEBO = not(MECO);
    /*p24.USEC*/ wire USEC = not(DIV_07n);
    /*p24.KUPA*/ wire FRn  = amux2(LCDC_EN, KEBO, LCDC_ENn, USEC);
    /*p24.KOFO*/ wire KOFO = not(FRn);
    next.PIN_FR = KOFO;
  }

  {
    // Vertical sync
    /*p24.NERU*/ wire LINE_000n = nor(lcd.Y0, lcd.Y1, lcd.Y2, lcd.Y3, lcd.Y4, lcd.Y5, lcd.Y6, lcd.Y7);
    /*p24.MEDA*/ next.VSYNC_OUTn.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_000n);
    /*p24.MURE*/ wire MURE = not(lcd.VSYNC_OUTn);
    next.PIN_S = MURE;
  }
}

//-----------------------------------------------------------------------------

void LCD::tick_fast(const ClockSignals1& clk_sig1,
                    const ClockSignals2& clk_sig2,
                    const ResetSignals1& rst_sig,
                    const Video& vid,
                    
                    bool SCAN_DONE_d0_TRIG,
                    bool DIV_06n,
                    bool DIV_07n,
                    bool LCDC_EN,
                    LCD& next)
{
  if (!rst_sig.VID_RESETn) {
    next.VBLANK_d4b = next.VBLANK_d4;

    next.LINE_STROBE.reset(clk_sig2.SONO_ABCDxxxx);
    next.PIN_CPG = nor(false, next.NEW_LINE_d0a);
    next.CPEN_LATCH = 0;
    next.PIN_CP = !vid.FINE_MATCH_TRIG;

    next.VID_LINE_TRIG_d4n = 0;
    next.VID_LINE_TRIG_d4a = 1;
    next.VID_LINE_TRIG_d4o = 0;

    next.VID_LINE_TRIG_d4p = 0;
    next.VID_LINE_TRIG_d4c = 1;

    next.RUJU = true;
    next.POME = true;
    next.PIN_ST  = next.RUJU;
    next.PIN_S   = !next.VSYNC_OUTn;
    next.PIN_CPL = !amux2(LCDC_EN, next.NEW_LINE_d0a, !LCDC_EN, !DIV_06n);
    next.PIN_FR  = !amux2(LCDC_EN, xor(next.FRAME_EVEN, next.LINE_EVEN), !LCDC_EN, !DIV_07n);

    next.VID_LINE_d4.reset(clk_sig2.XUPY_ABxxEFxx);
    next.VID_LINE_d6.reset(clk_sig2.AWOH_xxCDxxGH);
    next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);

    next.VBLANK_d4.reset(next.NEW_LINE_d4a);
    next.FRAME_EVEN.reset(next.VBLANK_d4);
  
    next.NEW_LINE_d4a.reset(clk_sig2.TALU_xxxxEFGH);
    next.VSYNC_OUTn.reset(next.NEW_LINE_d4a);
    next.LINE_153_d4.reset(next.NEW_LINE_d4a);

    next.X0.reset(clk_sig2.TALU_xxxxEFGH);
    next.X1.reset(!next.X0);
    next.X2.reset(!next.X1);
    next.X3.reset(!next.X2);
    next.X4.reset(!next.X3);
    next.X5.reset(!next.X4);
    next.X6.reset(!next.X5);

    next.NEW_LINE_d0a.reset(clk_sig2.SONO_ABCDxxxx);

    next.LINE_EVEN.reset(!next.NEW_LINE_d0a);

    next.Y0.reset(next.NEW_LINE_d0a);
    next.Y1.reset(!next.Y0);
    next.Y2.reset(!next.Y1);
    next.Y3.reset(!next.Y2);
    next.Y4.reset(!next.Y3);
    next.Y5.reset(!next.Y4);
    next.Y6.reset(!next.Y5);
    next.Y7.reset(!next.Y6);

    return;
  }


  {
    int lcd_x = next.x();
    int lcd_y = next.y();

    /*p21.LAMA*/ wire FRAME_RSTn = nor(next.LINE_153_d4, !1);
    /*p21.PARU*/ next.VBLANK_d4b = next.VBLANK_d4;


    /*p21.TEGY*/ wire LINE_STROBEa = nand(lcd_x != 0, lcd_x != 7, lcd_x != 45, lcd_x != 83);
    /*p21.SYGU*/ next.LINE_STROBE.tock(clk_sig2.SONO_ABCDxxxx, 1, LINE_STROBEa);
    next.PIN_CPG = nor(next.LINE_STROBE, next.NEW_LINE_d0a);

    {
      // weird gate
      /*p21.WUSA*/ if (vid.X0 && vid.X3) next.CPEN_LATCH = 1;
      /*p21.WUSA*/ if (vid.RENDER_DONE_SYNC)  next.CPEN_LATCH = 0;

      next.PIN_CP = !or(and(vid.CLKPIPE, next.CPEN_LATCH), vid.FINE_MATCH_TRIG);
    }

    /*p28.BYHA*/ next.VID_LINE_TRIG_d4n = or(next.VID_LINE_d6, !next.VID_LINE_d4);
    /*p28.ATEJ*/ next.VID_LINE_TRIG_d4a = not(next.VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ next.VID_LINE_TRIG_d4o = not(next.VID_LINE_TRIG_d4a);

    /*p28.ABAK*/ wire VID_LINE_TRIG_d4b = next.VID_LINE_TRIG_d4a;
    /*p28.BYVA*/ next.VID_LINE_TRIG_d4p = not(VID_LINE_TRIG_d4b);
    /*p29.DYBA*/ next.VID_LINE_TRIG_d4c = not(next.VID_LINE_TRIG_d4p);

    // Horizontal sync

    if (next.X_8_SYNC) {
      next.RUJU = true;
      next.POME = true;
    }

    if (SCAN_DONE_d0_TRIG) {
      next.RUJU = false;
      next.POME = false;
    }

    next.PIN_ST  = next.RUJU;
    next.PIN_S   = !next.VSYNC_OUTn;
    next.PIN_CPL = !amux2(LCDC_EN, next.NEW_LINE_d0a, !LCDC_EN, !DIV_06n);
    next.PIN_FR  = !amux2(LCDC_EN, xor(next.FRAME_EVEN, next.LINE_EVEN), !LCDC_EN, !DIV_07n);

    /*p29.ABOV*/ wire VID_LINE_d0 = and(next.NEW_LINE_d0a, lcd_y < 144);
    next.VID_LINE_d4.tock(clk_sig2.XUPY_ABxxEFxx, 1, VID_LINE_d0);
    next.VID_LINE_d6.tock(clk_sig2.AWOH_xxCDxxGH, 1, next.VID_LINE_d4);
    next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);

    next.VBLANK_d4.tock(next.NEW_LINE_d4a, 1, lcd_y >= 144);
    next.FRAME_EVEN.tock(next.VBLANK_d4, 1, !next.FRAME_EVEN);
  
    next.NEW_LINE_d4a.tock(clk_sig2.TALU_xxxxEFGH, 1, next.NEW_LINE_d0a);
    next.VSYNC_OUTn.tock(next.NEW_LINE_d4a, 1, lcd_y == 0);
    next.LINE_153_d4.tock(next.NEW_LINE_d4a, 1, lcd_y == 153);

    /*p21.MUDE*/ wire X_RSTn = not(next.NEW_LINE_d0a);
    next.X0.tock(clk_sig2.TALU_xxxxEFGH, X_RSTn, !next.X0);
    next.X1.tock(!next.X0, X_RSTn, !next.X1);
    next.X2.tock(!next.X1, X_RSTn, !next.X2);
    next.X3.tock(!next.X2, X_RSTn, !next.X3);
    next.X4.tock(!next.X3, X_RSTn, !next.X4);
    next.X5.tock(!next.X4, X_RSTn, !next.X5);
    next.X6.tock(!next.X5, X_RSTn, !next.X6);

    next.NEW_LINE_d0a.tock(clk_sig2.SONO_ABCDxxxx, 1, lcd_x == 113);

    next.LINE_EVEN.tock(!next.NEW_LINE_d0a, 1, !next.LINE_EVEN);

    next.Y0.tock(next.NEW_LINE_d0a, FRAME_RSTn, !next.Y0);
    next.Y1.tock(!next.Y0, FRAME_RSTn, !next.Y1);
    next.Y2.tock(!next.Y1, FRAME_RSTn, !next.Y2);
    next.Y3.tock(!next.Y2, FRAME_RSTn, !next.Y3);
    next.Y4.tock(!next.Y3, FRAME_RSTn, !next.Y4);
    next.Y5.tock(!next.Y4, FRAME_RSTn, !next.Y5);
    next.Y6.tock(!next.Y5, FRAME_RSTn, !next.Y6);
    next.Y7.tock(!next.Y6, FRAME_RSTn, !next.Y7);
  }
}

//-----------------------------------------------------------------------------

};
