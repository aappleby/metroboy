#include "Sch_LCD.h"
#include "Schematics.h"
#include <memory.h>

#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Registers.h"
#include "Sch_Timer.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void LCD_tick(const Clocks& clk,
              const Resets& rst,
              const LCD& lcd,
              const Video& vid,
           
              bool SCAN_DONE_TRIG,
              bool DIV_06n,
              bool DIV_07n,
              bool LCDC_EN,
              LCD& next)
{
  {
    /*p01.LYHA*/ wire VID_RESET2  = not(rst.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);

    /*p21.MUDE*/ wire X_RSTn = nor(lcd.NEW_LINE_d0a, VID_RESET2);
    /*p21.SAXO*/ next.X0.tock(clk.TALU_xxxxEFGH, X_RSTn, !lcd.X0);
    /*p21.TYPO*/ next.X1.tock(!lcd.X0,           X_RSTn, !lcd.X1);
    /*p21.VYZO*/ next.X2.tock(!lcd.X1,           X_RSTn, !lcd.X2);
    /*p21.TELU*/ next.X3.tock(!lcd.X2,           X_RSTn, !lcd.X3);
    /*p21.SUDE*/ next.X4.tock(!lcd.X3,           X_RSTn, !lcd.X4);
    /*p21.TAHA*/ next.X5.tock(!lcd.X4,           X_RSTn, !lcd.X5);
    /*p21.TYRY*/ next.X6.tock(!lcd.X5,           X_RSTn, !lcd.X6);

    // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.SANU*/ wire LINE_ENDn = and(lcd.X6, lcd.X5, lcd.X4, lcd.X0);
    /*p21.RUTU*/ next.NEW_LINE_d0a.tock(clk.SONO_ABCDxxxx, VID_RESETn2, LINE_ENDn);
    /*p21.NYPE*/ next.NEW_LINE_d4a.tock(clk.TALU_xxxxEFGH, VID_RESETn2, lcd.NEW_LINE_d0a);
    /*p21.PURE*/ next.NEW_LINE_d0n = not(lcd.NEW_LINE_d0a);
    /*p21.SELA*/ next.NEW_LINE_d0b = not(lcd.NEW_LINE_d0n);

    // Schematic wrong
    // NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

    /*p21.NOKO*/ next.LINE_153_d0 = and(lcd.Y7, lcd.Y4, lcd.Y3, lcd.Y0);
    /*p21.MYTA*/ next.LINE_153_d4.tock(lcd.NEW_LINE_d4a, VID_RESETn2, lcd.LINE_153_d0);
    /*p21.LAMA*/ wire FRAME_RSTn = nor(lcd.LINE_153_d4, VID_RESET2);

    /*p21.MUWY*/ next.Y0.tock(lcd.NEW_LINE_d0a, FRAME_RSTn, !lcd.Y0);
    /*p21.MYRO*/ next.Y1.tock(!lcd.Y0,              FRAME_RSTn, !lcd.Y1);
    /*p21.LEXA*/ next.Y2.tock(!lcd.Y1,              FRAME_RSTn, !lcd.Y2);
    /*p21.LYDO*/ next.Y3.tock(!lcd.Y2,              FRAME_RSTn, !lcd.Y3);
    /*p21.LOVU*/ next.Y4.tock(!lcd.Y3,              FRAME_RSTn, !lcd.Y4);
    /*p21.LEMA*/ next.Y5.tock(!lcd.Y4,              FRAME_RSTn, !lcd.Y5);
    /*p21.MATO*/ next.Y6.tock(!lcd.Y5,              FRAME_RSTn, !lcd.Y6);
    /*p21.LAFO*/ next.Y7.tock(!lcd.Y6,              FRAME_RSTn, !lcd.Y7);

    /*p21.XYVO*/ next.VBLANK_d0  = and(lcd.Y4, lcd.Y7); // 128 + 16 = 144
    /*p29.ALES*/ next.VBLANK_d0n = not(lcd.VBLANK_d0);
    /*p21.POPU*/ next.VBLANK_d4.tock(lcd.NEW_LINE_d4a, VID_RESETn2, lcd.VBLANK_d0);
    /*p21.PARU*/ next.VBLANK_d4n = not(!lcd.VBLANK_d4);
  }

  {
    /*p01.ATAR*/ wire VID_RESET6  = not(rst.VID_RESETn);
    /*p01.AMYG*/ wire VID_RESET7  = not(rst.VID_RESETn);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

    /*p29.ABOV*/ next.VID_LINE_d0 = and(lcd.NEW_LINE_d0b, lcd.VBLANK_d0n);
    /*p29.CATU*/ next.VID_LINE_d4.tock(clk.XUPY_ABxxEFxx, VID_RESETn3, lcd.VID_LINE_d0);
    /*p28.ANEL*/ next.VID_LINE_d6.tock(clk.AWOH_xxCDxxGH, VID_RESETn3, lcd.VID_LINE_d4);

    /*p28.ABAF*/ wire VID_LINE_d4n  = not(lcd.VID_LINE_d4);

    /*p28.BYHA*/ next.VID_LINE_TRIG_d4n = and(or(lcd.VID_LINE_d6, VID_LINE_d4n), VID_RESETn3);
    /*p28.ATEJ*/ next.VID_LINE_TRIG_d4a = not(lcd.VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ next.VID_LINE_TRIG_d4o = not(lcd.VID_LINE_TRIG_d4a);

    /*p28.ABAK*/ next.VID_LINE_TRIG_d4b = or (lcd.VID_LINE_TRIG_d4a, VID_RESET7);
    /*p28.BYVA*/ next.VID_LINE_TRIG_d4p = not(lcd.VID_LINE_TRIG_d4b);
    /*p29.DYBA*/ next.VID_LINE_TRIG_d4c = not(lcd.VID_LINE_TRIG_d4p);

  }

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

    /*p01.LYHA*/ wire VID_RESET2  = not(rst.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
    /*p21.SYGU*/ next.LINE_STROBE.tock(clk.SONO_ABCDxxxx, VID_RESETn2, LINE_STROBEa);

    /*p21.RYNO*/ wire CPGn = or(lcd.LINE_STROBE, lcd.NEW_LINE_d0a);
    /*p21.POGU*/ wire POGU = not(CPGn);
    next.PIN_CPG = POGU;
  }


  {
    /*p01.TOFU*/ wire VID_RESET3  = not(rst.VID_RESETn);
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

    /*p01.TOFU*/ wire VID_RESET3  = not(rst.VID_RESETn);

    // Latch loop
    ///*p24.POME*/ wire POME = nor(SCAN_DONE_TRIG, POFY);
    ///*p24.RUJU*/ wire RUJU = or(X_8_SYNC, VID_RESET3, POME);
    ///*p24.POFY*/ wire POFY = not(RUJU);

    if (lcd.X_8_SYNC || VID_RESET3) {
      next.RUJU = true;
      next.POME = true;
    }

    if (SCAN_DONE_TRIG) {
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
    /*p01.LYHA*/ wire VID_RESET2  = not(rst.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
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
    /*p01.LYHA*/ wire VID_RESET2  = not(rst.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
    /*p24.NERU*/ wire LINE_000n = nor(lcd.Y0, lcd.Y1, lcd.Y2, lcd.Y3, lcd.Y4, lcd.Y5, lcd.Y6, lcd.Y7);
    /*p24.MEDA*/ next.VSYNC_OUTn.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_000n);
    /*p24.MURE*/ wire MURE = not(lcd.VSYNC_OUTn);
    next.PIN_S = MURE;
  }
}

//-----------------------------------------------------------------------------

#if 0
void LCD_tick2(const LCD2& lcd,
               bool VID_RESETn,
               bool SONO_ABCDxxxx,
               bool TALU_xxxxEFGH,

               LCD2& next)
{
  /*p01.LYHA*/ wire VID_RESET2  = not(VID_RESETn);
  /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);

  /*p21.MUDE*/ wire CNT_RSTn = nor(lcd.NEW_LINE_d0a, VID_RESET2);
  /*p21.SAXO*/ next.X0.tock(TALU_xxxxEFGH, CNT_RSTn, !lcd.X0);
  /*p21.TYPO*/ next.X1.tock(!lcd.X0, CNT_RSTn, !lcd.X1);
  /*p21.VYZO*/ next.X2.tock(!lcd.X1, CNT_RSTn, !lcd.X2);
  /*p21.TELU*/ next.X3.tock(!lcd.X2, CNT_RSTn, !lcd.X3);
  /*p21.SUDE*/ next.X4.tock(!lcd.X3, CNT_RSTn, !lcd.X4);
  /*p21.TAHA*/ next.X5.tock(!lcd.X4, CNT_RSTn, !lcd.X5);
  /*p21.TYRY*/ next.X6.tock(!lcd.X5, CNT_RSTn, !lcd.X6);

  // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p21.SANU*/ wire LINE_ENDn = and(lcd.X6, lcd.X5, lcd.X4, lcd.X0);
  /*p21.RUTU*/ next.NEW_LINE_d0a.tock(SONO_ABCDxxxx, VID_RESETn2, LINE_ENDn);
  /*p21.NYPE*/ next.NEW_LINE_d4a.tock(TALU_xxxxEFGH, VID_RESETn2, lcd.NEW_LINE_d0a);

  // Schematic wrong
  // NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

  /*p21.NOKO*/ wire LINE_153_d0 = and(lcd.Y7, lcd.Y4, lcd.Y3, lcd.Y0);
  /*p21.MYTA*/ next.LINE_153_d4.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_153_d0);
  /*p21.LAMA*/ wire FRAME_RSTn = nor(lcd.LINE_153_d4, VID_RESET2);

  /*p21.MUWY*/ next.Y0.tock(lcd.NEW_LINE_d0a, FRAME_RSTn, !lcd.Y0);
  /*p21.MYRO*/ next.Y1.tock(!lcd.Y0,       FRAME_RSTn, !lcd.Y1);
  /*p21.LEXA*/ next.Y2.tock(!lcd.Y1,       FRAME_RSTn, !lcd.Y2);
  /*p21.LYDO*/ next.Y3.tock(!lcd.Y2,       FRAME_RSTn, !lcd.Y3);
  /*p21.LOVU*/ next.Y4.tock(!lcd.Y3,       FRAME_RSTn, !lcd.Y4);
  /*p21.LEMA*/ next.Y5.tock(!lcd.Y4,       FRAME_RSTn, !lcd.Y5);
  /*p21.MATO*/ next.Y6.tock(!lcd.Y5,       FRAME_RSTn, !lcd.Y6);
  /*p21.LAFO*/ next.Y7.tock(!lcd.Y6,       FRAME_RSTn, !lcd.Y7);
}
#endif

//-----------------------------------------------------------------------------

};
