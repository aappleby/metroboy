#include "Sch_LCD.h"
#include "Schematics.h"
#include <memory.h>

#include "Sch_Clocks.h"
#include "Sch_Resets.h"

#define TCYCLES_LINE      456
#define TCYCLES_OAM       80
#define TCYCLES_VRAM      172
#define TCYCLES_HBLANK    204

#define HBLANK_DELAY_START  8

namespace Schematics {

//-----------------------------------------------------------------------------

void LCD_tick(const Clocks& clocks,
              const Resets& resets,
              const LcdIn& in,
              const LCD& prev,
              LCD& next)
{
  /*p01.LYHA*/ wire VID_RESET2  = not(resets.VID_RESETn);
  /*p01.TOFU*/ wire VID_RESET3  = not(resets.VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(resets.VID_RESETn);
  /*p01.AMYG*/ wire VID_RESET7  = not(resets.VID_RESETn);
  /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  /*p24.KEDY*/ wire LCDC_ENn = not(in.LCDC_EN);

  /*p21.MUDE*/ wire CNT_RSTn = nor(prev.LINE_ENDo, VID_RESET2);
  /*p21.SAXO*/ next.CNT0.tock(clocks.TALU_AxxxxFGH, CNT_RSTn, !prev.CNT0);
  /*p21.TYPO*/ next.CNT1.tock(!prev.CNT0,           CNT_RSTn, !prev.CNT1);
  /*p21.VYZO*/ next.CNT2.tock(!prev.CNT1,           CNT_RSTn, !prev.CNT2);
  /*p21.TELU*/ next.CNT3.tock(!prev.CNT2,           CNT_RSTn, !prev.CNT3);
  /*p21.SUDE*/ next.CNT4.tock(!prev.CNT3,           CNT_RSTn, !prev.CNT4);
  /*p21.TAHA*/ next.CNT5.tock(!prev.CNT4,           CNT_RSTn, !prev.CNT5);
  /*p21.TYRY*/ next.CNT6.tock(!prev.CNT5,           CNT_RSTn, !prev.CNT6);

  // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p21.SANU*/ wire LINE_ENDn = and(prev.CNT6, prev.CNT5, prev.CNT4, prev.CNT0);
  /*p21.RUTU*/ next.LINE_ENDo.tock(clocks.SONO_xBCDExxx, VID_RESETn2, LINE_ENDn);
  /*p21.NYPE*/ next.LINE_ENDp.tock(clocks.TALU_AxxxxFGH, VID_RESETn2, prev.LINE_ENDo);

  /*p21.XYVO*/ wire IN_VBLANK  = and(prev.V4, prev.V7); // 128 + 16 = 144
  /*p29.ALES*/ wire IN_VBLANKn = not(IN_VBLANK);

  /*p21.PURE*/ wire LINE_ENDa = not(prev.LINE_ENDo);
  /*p21.SELA*/ wire LINE_ENDq = not(LINE_ENDa);

  /*p29.ABOV*/ next.IN_LINEa = and(LINE_ENDq, IN_VBLANKn);
  /*p29.CATU*/ next.IN_LINEb.tock(clocks.XUPY_xBCxxFGx, VID_RESETn3, prev.IN_LINEa);
  /*p28.ANEL*/ next.IN_LINEc.tock(clocks.AWOH_AxxDExxH, VID_RESETn3, prev.IN_LINEb);

  /*p28.ABAF*/ wire IN_LINEn  = not(prev.IN_LINEb);
  /*p28.BYHA*/ next.NEW_LINEn = and(or(prev.IN_LINEc, IN_LINEn), VID_RESETn3);
  /*p28.ATEJ*/ next.NEW_LINE1 = not(prev.NEW_LINEn);
  /*p28.ABAK*/ next.NEW_LINE2 = or (prev.NEW_LINE1, VID_RESET7);
  /*p28.BYVA*/ next.NEW_LINE3 = not(prev.NEW_LINE2);
  /*p29.DYBA*/ next.NEW_LINE4 = not(prev.NEW_LINE3);

  // Schematic wrong
  // NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

  /*p21.NOKO*/ wire LINE_153 = and(prev.V7, prev.V4, prev.V3, prev.V0);
  /*p21.MYTA*/ next.LINE_153_SYNC.tock(prev.LINE_ENDp, VID_RESETn2, LINE_153);
  /*p21.LAMA*/ wire FRAME_RSTn = nor(prev.LINE_153_SYNC, VID_RESET2);

  /*p21.MUWY*/ next.V0.tock(prev.LINE_ENDo, FRAME_RSTn, !prev.V0);
  /*p21.MYRO*/ next.V1.tock(!prev.V0,       FRAME_RSTn, !prev.V1);
  /*p21.LEXA*/ next.V2.tock(!prev.V1,       FRAME_RSTn, !prev.V2);
  /*p21.LYDO*/ next.V3.tock(!prev.V2,       FRAME_RSTn, !prev.V3);
  /*p21.LOVU*/ next.V4.tock(!prev.V3,       FRAME_RSTn, !prev.V4);
  /*p21.LEMA*/ next.V5.tock(!prev.V4,       FRAME_RSTn, !prev.V5);
  /*p21.MATO*/ next.V6.tock(!prev.V5,       FRAME_RSTn, !prev.V6);
  /*p21.LAFO*/ next.V7.tock(!prev.V6,       FRAME_RSTn, !prev.V7);

  /*p21.POPU*/ next.REG_VBLANK.tock(prev.LINE_ENDp, VID_RESETn2, IN_VBLANK);


  {
    wire C0 = prev.CNT0;
    wire C1 = prev.CNT1;
    wire C2 = prev.CNT2;
    wire C3 = prev.CNT3;
    wire C4 = prev.CNT4;
    wire C5 = prev.CNT5;
    wire C6 = prev.CNT6;

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

    /*p21.SYGU*/ next.LINE_STROBE.tock(clocks.SONO_xBCDExxx, VID_RESETn2, LINE_STROBEa);

    /*p21.RYNO*/ wire CPGn = or(prev.LINE_STROBE, prev.LINE_ENDo);
    /*p21.POGU*/ next.CPG = not(CPGn);
  }


  {
    // the "pixel clock" signal is too tightly intertwined with the ppu right now
    /*p21.WEGO*/ wire WEGO  = or(VID_RESET3, in.RENDER_DONE_SYNC);
    /*p21.XAJO*/ wire X_009 = and(in.X0, in.X3);
    /*p21.WUSA*/ wire WUSA  = or(X_009, WEGO);
    /*p21.TOBA*/ wire TOBA  = and(in.CLKPIPE, WUSA);
    /*p21.SEMU*/ wire CPn   = or(TOBA, in.FINE_MATCH_TRIG);
    /*p21.RYPO*/ next.CP    = not(CPn);
  }

  {
    // ditto with hsync
    // Horizontal sync
    // ST = or(PAHO, VID_RESET3, nor(SCAN_DONE_TRIG, POFY));

    // Combi loop, please fix.
    ///*p24.POME*/ wire POME = nor(prev.spr.SCAN_DONE_TRIG, POFY);
    ///*p24.RUJU*/ wire RUJU = or(prev.vid.PAHO, prev.rst.VID_RESET3, POME);
    ///*p24.POFY*/ wire POFY = not(RUJU);

    /*p24.PAHO*/ next.PAHO.tock(in.ROXO_4M, in.RENDERING, in.X3);
    /*p24.RUJU*/ wire RUJU = or(prev.PAHO, VID_RESET3, in.SCAN_DONE_TRIG);
    /*p24.POFY*/ wire POFY = not(RUJU);

    /*p24.RUZE*/ next.ST = not(POFY);
  }

  {
    /*p21.PURE*/ wire LINE_DONEn = not(prev.LINE_ENDo);
    /*p24.KASA*/ wire LINE_DONEb = not(LINE_DONEn);
    /*p24.UMOB*/ wire DIV_06 = not(in.DIV_06n);
    /*p24.KAHE*/ wire CPLn = amux2(in.LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
    /*p24.KYMO*/ next.CPL = not(CPLn);
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)

    /*p24.LOFU*/ wire CLK_LINE_EVEN = not(prev.LINE_ENDo);
    /*p24.LUCA*/ next.LINE_EVEN.tock(CLK_LINE_EVEN, VID_RESETn2, !prev.LINE_EVEN);
    /*p21.NAPO*/ next.FRAME_EVEN.tock(prev.REG_VBLANK, VID_RESETn2, !prev.FRAME_EVEN);

    /*p24.MAGU*/ wire MAGU = xor(prev.FRAME_EVEN, prev.LINE_EVEN);
    /*p24.MECO*/ wire MECO = not(MAGU);
    /*p24.KEBO*/ wire KEBO = not(MECO);
    /*p24.USEC*/ wire USEC = not(in.DIV_07n);
    /*p24.KUPA*/ wire FRn  = amux2(in.LCDC_EN, KEBO, LCDC_ENn, USEC);
    /*p24.KOFO*/ next.FR  = not(FRn);
  }

  {
    // Vertical sync
    /*p24.NERU*/ wire LINE_000n = nor(prev.V0, prev.V1, prev.V2, prev.V3, prev.V4, prev.V5, prev.V6, prev.V7);
    /*p24.MEDA*/ next.VSYNC_OUTn.tock(prev.LINE_ENDp, VID_RESETn2, LINE_000n);
    /*p24.MURE*/ next.S = not(prev.VSYNC_OUTn);
  }
}

//-----------------------------------------------------------------------------

};
