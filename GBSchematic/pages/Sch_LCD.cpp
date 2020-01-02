#include "Sch_LCD.h"
#include "Schematics.h"
#include <memory.h>

#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Registers.h"
#include "Sch_Timer.h"

#pragma warning(disable:4458)

namespace Schematics {

//-----------------------------------------------------------------------------

void LcdRegisters::pwron() {
  X0.val = false; X0.clk = false;
  X1.val = false; X1.clk = false;
  X2.val = false; X2.clk = false;
  X3.val = false; X3.clk = false;
  X4.val = false; X4.clk = false;
  X5.val = false; X5.clk = false;
  X6.val = false; X6.clk = false;
  Y0.val = false; Y0.clk = false;
  Y1.val = false; Y1.clk = false;
  Y2.val = false; Y2.clk = false;
  Y3.val = false; Y3.clk = false;
  Y4.val = false; Y4.clk = false;
  Y5.val = false; Y5.clk = false;
  Y6.val = false; Y6.clk = false;
  Y7.val = false; Y7.clk = false;

  VID_LINE_d4.val  = false; VID_LINE_d4.clk  = false; 
  VID_LINE_d6.val  = false; VID_LINE_d6.clk  = false;
  NEW_LINE_d0a_val = false; NEW_LINE_d0a_clk = false;
  NEW_LINE_d4a.val = false; NEW_LINE_d4a.clk = false;
  LINE_153_d4.val  = false; LINE_153_d4.clk  = false;
  VBLANK_d4.val    = false; VBLANK_d4.clk    = false;
}

//----------------------------------------

void LcdRegisters::reset() {
  X0.val = false; X0.clk = false;
  X1.val = false; X1.clk = true;
  X2.val = false; X2.clk = true;
  X3.val = false; X3.clk = true;
  X4.val = false; X4.clk = true;
  X5.val = false; X5.clk = true;
  X6.val = false; X6.clk = true;
  Y0.val = false; Y0.clk = true;
  Y1.val = false; Y1.clk = true;
  Y2.val = false; Y2.clk = true;
  Y3.val = false; Y3.clk = true;
  Y4.val = false; Y4.clk = true;
  Y5.val = false; Y5.clk = true;
  Y6.val = false; Y6.clk = true;
  Y7.val = false; Y7.clk = true;

  VID_LINE_d4.val  = false; VID_LINE_d4.clk  = false; 
  VID_LINE_d6.val  = false; VID_LINE_d6.clk  = true;
  NEW_LINE_d0a_val = true;  NEW_LINE_d0a_clk = true;
  NEW_LINE_d4a.val = false; NEW_LINE_d4a.clk = false;
  LINE_153_d4.val  = true;  LINE_153_d4.clk  = false;
  VBLANK_d4.val    = true;  VBLANK_d4.clk    = false;
}

//----------------------------------------

void LcdRegisters::tock_slow(const ClockSignals2& vid_clk,
                             const VideoResets& vid_rst)
{
  LcdRegisters prev = *this;
  LcdRegisters& next = *this;

  /*p01.LYHA*/ bool VID_RESET2  = not(vid_rst.VID_RESETn);
  /*p01.LYFE*/ bool VID_RESETn2 = not(VID_RESET2);
  /*p01.ATAR*/ bool VID_RESET6  = not(vid_rst.VID_RESETn);
  /*p01.ABEZ*/ bool VID_RESETn3 = not(VID_RESET6);

  /*p21.MUDE*/ bool X_RSTn       = nor(prev.NEW_LINE_d0a_val, VID_RESET2);
  /*p21.SANU*/ bool LINE_END     = and(prev.X6, prev.X5, prev.X4, prev.X0); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p21.PURE*/ bool NEW_LINE_d0n = not(prev.NEW_LINE_d0a_val);
  /*p21.SELA*/ bool NEW_LINE_d0b = not(NEW_LINE_d0n);
  /*p21.NOKO*/ bool LINE_153_d0  = and(prev.Y7, prev.Y4, prev.Y3, prev.Y0); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*p21.LAMA*/ bool FRAME_RSTn   = nor(prev.LINE_153_d4, VID_RESET2);
  /*p21.XYVO*/ bool LINE_144_d0  = and(prev.Y4, prev.Y7); // 128 + 16 = 144
  /*p29.ALES*/ bool LINE_144_d0n = not(LINE_144_d0);
  /*p29.ABOV*/ bool VID_LINE_d0  = and(NEW_LINE_d0b, LINE_144_d0n);

  /*p21.SAXO*/ next.X0.tock(vid_clk.TALU_xBCDExxx, X_RSTn, !prev.X0);
  /*p21.TYPO*/ next.X1.tock(!prev.X0,              X_RSTn, !prev.X1);
  /*p21.VYZO*/ next.X2.tock(!prev.X1,              X_RSTn, !prev.X2);
  /*p21.TELU*/ next.X3.tock(!prev.X2,              X_RSTn, !prev.X3);
  /*p21.SUDE*/ next.X4.tock(!prev.X3,              X_RSTn, !prev.X4);
  /*p21.TAHA*/ next.X5.tock(!prev.X4,              X_RSTn, !prev.X5);
  /*p21.TYRY*/ next.X6.tock(!prev.X5,              X_RSTn, !prev.X6);

  if (!next.NEW_LINE_d0a_clk && vid_clk.SONO_AxxxxFGH) {
    next.NEW_LINE_d0a_val = LINE_END;
  }
  next.NEW_LINE_d0a_clk = vid_clk.SONO_AxxxxFGH;

  if (!vid_rst.VID_RESETn) {
    next.NEW_LINE_d0a_val = 0;
  }

  /*p21.NYPE*/ next.NEW_LINE_d4a.tock(vid_clk.TALU_xBCDExxx, VID_RESETn2, prev.NEW_LINE_d0a_val);
  /*p21.MYTA*/ next.LINE_153_d4.tock(prev.NEW_LINE_d4a, VID_RESETn2, LINE_153_d0);

  /*p21.MUWY*/ next.Y0.tock(prev.NEW_LINE_d0a_val, FRAME_RSTn, !prev.Y0);
  /*p21.MYRO*/ next.Y1.tock(!prev.Y0,              FRAME_RSTn, !prev.Y1);
  /*p21.LEXA*/ next.Y2.tock(!prev.Y1,              FRAME_RSTn, !prev.Y2);
  /*p21.LYDO*/ next.Y3.tock(!prev.Y2,              FRAME_RSTn, !prev.Y3);
  /*p21.LOVU*/ next.Y4.tock(!prev.Y3,              FRAME_RSTn, !prev.Y4);
  /*p21.LEMA*/ next.Y5.tock(!prev.Y4,              FRAME_RSTn, !prev.Y5);
  /*p21.MATO*/ next.Y6.tock(!prev.Y5,              FRAME_RSTn, !prev.Y6);
  /*p21.LAFO*/ next.Y7.tock(!prev.Y6,              FRAME_RSTn, !prev.Y7);

  /*p21.POPU*/ next.VBLANK_d4.tock(prev.NEW_LINE_d4a, VID_RESETn2, LINE_144_d0);
  
  /*p29.CATU*/ next.VID_LINE_d4.tock(vid_clk.XUPY_xBCxxFGx, VID_RESETn3, VID_LINE_d0);
  /*p28.ANEL*/ next.VID_LINE_d6.tock(vid_clk.AWOH_AxxDExxH, VID_RESETn3, prev.VID_LINE_d4);
}

//----------------------------------------

void LcdRegisters::commit() {

#if 0
  /*p21.SAXO*/ Reg X0;
  /*p21.TYPO*/ Reg X1;
  /*p21.VYZO*/ Reg X2;
  /*p21.TELU*/ Reg X3;
  /*p21.SUDE*/ Reg X4;
  /*p21.TAHA*/ Reg X5;
  /*p21.TYRY*/ Reg X6;

  /*p21.MUWY*/ Reg Y0;
  /*p21.MYRO*/ Reg Y1;
  /*p21.LEXA*/ Reg Y2;
  /*p21.LYDO*/ Reg Y3;
  /*p21.LOVU*/ Reg Y4;
  /*p21.LEMA*/ Reg Y5;
  /*p21.MATO*/ Reg Y6;
  /*p21.LAFO*/ Reg Y7;

  /*p29.CATU*/ Reg VID_LINE_d4;
  /*p28.ANEL*/ Reg VID_LINE_d6;

  ///*p21.RUTU*/ Reg NEW_LINE_d0a;
  /*p21.RUTU*/ bool NEW_LINE_d0a_clk;
  /*p21.RUTU*/ bool NEW_LINE_d0a_val;

  /*p21.NYPE*/ Reg NEW_LINE_d4a;
  /*p21.MYTA*/ Reg LINE_153_d4;
  /*p21.POPU*/ Reg VBLANK_d4;
#endif
}

//-----------------------------------------------------------------------------

LcdSignals LcdSignals::tick_slow(const VideoResets& vid_rst,
                                 const LcdRegisters& lcd)
{
  /*p01.ATAR*/ bool VID_RESET6  = not(vid_rst.VID_RESETn);
  /*p01.AMYG*/ bool VID_RESET7  = not(vid_rst.VID_RESETn);
  /*p01.ABEZ*/ bool VID_RESETn3 = not(VID_RESET6);

  /*p28.ABAF*/ bool VID_LINE_d4n      = not(lcd.VID_LINE_d4);

  /*p21.PARU*/ wire VBLANK_d4b        = not(!lcd.VBLANK_d4);
  /*p28.BYHA*/ wire VID_LINE_TRIG_d4n = and(or(lcd.VID_LINE_d6, VID_LINE_d4n), VID_RESETn3);
  /*p28.ATEJ*/ wire VID_LINE_TRIG_d4a = not(VID_LINE_TRIG_d4n);
  /*p28.ABAK*/ wire VID_LINE_TRIG_d4b = or (VID_LINE_TRIG_d4a, VID_RESET7);
  /*p27.XAHY*/ wire VID_LINE_TRIG_d4o = not(VID_LINE_TRIG_d4a);
  /*p28.BYVA*/ wire VID_LINE_TRIG_d4p = not(VID_LINE_TRIG_d4b);
  /*p29.DYBA*/ wire VID_LINE_TRIG_d4c = not(VID_LINE_TRIG_d4p);

  LcdSignals sig;

  /*p21.PARU*/ sig.VBLANK_d4b         = VBLANK_d4b;
  /*p28.BYHA*/ sig.VID_LINE_TRIG_d4n  = VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ sig.VID_LINE_TRIG_d4a  = VID_LINE_TRIG_d4a;
  /*p27.XAHY*/ sig.VID_LINE_TRIG_d4o  = VID_LINE_TRIG_d4o;
  /*p28.BYVA*/ sig.VID_LINE_TRIG_d4p  = VID_LINE_TRIG_d4p;
  /*p29.DYBA*/ sig.VID_LINE_TRIG_d4c  = VID_LINE_TRIG_d4c;

  return sig;
}

//-----------------------------------------------------------------------------

};









#if 0

  {
    bool C0 = lcd.X0;
    bool C1 = lcd.X1;
    bool C2 = lcd.X2;
    bool C3 = lcd.X3;
    bool C4 = lcd.X4;
    bool C5 = lcd.X5;
    bool C6 = lcd.X6;

    /*p21.TOCU*/ bool C0n = not(C0);
    /*p21.VEPE*/ bool C1n = not(C1);
    /*p21.VUTY*/ bool C2n = not(C2);
    /*p21.VATE*/ bool C3n = not(C3);
    /*p21.TUDA*/ bool C4n = not(C4);
    /*p21.TAFY*/ bool C5n = not(C5);
    /*p21.TUJU*/ bool C6n = not(C6);

    // so this is like a strobe that fires 4x per line
    /*p21.VOKU*/ bool CNT_000n = nand(C6n, C5n, C4n, C3n, C2n, C1n, C0n); // 0000000 == 0
    /*p21.TOZU*/ bool CNT_007n = nand(C6n, C5n, C4n, C3n, C2,  C1,  C0 ); // 0000111 == 7
    /*p21.TECE*/ bool CNT_045n = nand(C6n, C5,  C4n, C3,  C2,  C1n, C0 ); // 0101101 == 45
    /*p21.TEBO*/ bool CNT_083n = nand(C6,  C5n, C4,  C3n, C2n, C1,  C0 ); // 1010011 == 83

    /*p21.TEGY*/ bool LINE_STROBEa = nand(CNT_000n, CNT_007n, CNT_045n, CNT_083n);

    /*p21.SYGU*/ next.LINE_STROBE.tock(vid_clk.SONO_AxxxxFGH, VID_RESETn2, LINE_STROBEa);

    /*p21.RYNO*/ bool CPGn = or(lcd.LINE_STROBE, lcd.NEW_LINE_d0a);
    /*p21.POGU*/ bool POGU = not(CPGn);
    next.PIN_CPG = POGU;
  }


  {
    /*p01.TOFU*/ bool VID_RESET3  = not(vid_rst.VID_RESETn);
    /*p21.WEGO*/ bool WEGO  = or(VID_RESET3, vid.RENDER_DONE_SYNC);
    /*p21.XAJO*/ bool X_009 = and(vid.X0, vid.X3);

    // weird gate
    /*p21.WUSA*/ if (X_009) next.CPEN_LATCH = 1;
    /*p21.WUSA*/ if (WEGO)  next.CPEN_LATCH = 0;

    /*p21.TOBA*/ bool TOBA = and(vid.CLKPIPE, lcd.CPEN_LATCH);
    /*p21.SEMU*/ bool CPn  = or(TOBA, vid.FINE_MATCH_TRIG);
    /*p21.RYPO*/ bool RYPO = not(CPn);
    next.PIN_CP = RYPO;
  }

  {
    // Horizontal sync

    /*p01.TOFU*/ bool VID_RESET3  = not(vid_rst.VID_RESETn);

    // Latch loop
    ///*p24.POME*/ bool POME = nor(SCAN_DONE_d0_TRIG, POFY);
    ///*p24.RUJU*/ bool RUJU = or(X_8_SYNC, VID_RESET3, POME);
    ///*p24.POFY*/ bool POFY = not(RUJU);

    if (lcd.X_8_SYNC || VID_RESET3) {
      next.RUJU = true;
      next.POME = true;
    }

    if (SCAN_DONE_d0_TRIG) {
      next.RUJU = false;
      next.POME = false;
    }

    /*p24.POFY*/ bool POFY = not(lcd.RUJU);
    /*p24.RUZE*/ bool RUZE = not(POFY);
    /*p24.PAHO*/ next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);
    next.PIN_ST = RUZE;
  }

  {
    /*p24.KEDY*/ bool LCDC_ENn = not(LCDC_EN);
    /*p21.PURE*/ bool LINE_DONEn = not(lcd.NEW_LINE_d0a);
    /*p24.KASA*/ bool LINE_DONEb = not(LINE_DONEn);
    /*p24.UMOB*/ bool DIV_06 = not(DIV_06n);
    /*p24.KAHE*/ bool CPLn = amux2(LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
    /*p24.KYMO*/ bool KYMO = not(CPLn);
    next.PIN_CPL = KYMO;
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)

    /*p24.KEDY*/ bool LCDC_ENn = not(LCDC_EN);
    /*p24.LOFU*/ bool CLK_LINE_EVEN = not(lcd.NEW_LINE_d0a);
    /*p24.LUCA*/ next.LINE_EVEN.tock(CLK_LINE_EVEN, VID_RESETn2, !lcd.LINE_EVEN);
    /*p21.NAPO*/ next.FRAME_EVEN.tock(lcd.VBLANK_d4, VID_RESETn2, !lcd.FRAME_EVEN);

    /*p24.MAGU*/ bool MAGU = xor(lcd.FRAME_EVEN, lcd.LINE_EVEN);
    /*p24.MECO*/ bool MECO = not(MAGU);
    /*p24.KEBO*/ bool KEBO = not(MECO);
    /*p24.USEC*/ bool USEC = not(DIV_07n);
    /*p24.KUPA*/ bool FRn  = amux2(LCDC_EN, KEBO, LCDC_ENn, USEC);
    /*p24.KOFO*/ bool KOFO = not(FRn);
    next.PIN_FR = KOFO;
  }

  {
    // Vertical sync
    /*p24.NERU*/ bool LINE_000n = nor(lcd.Y0, lcd.Y1, lcd.Y2, lcd.Y3, lcd.Y4, lcd.Y5, lcd.Y6, lcd.Y7);
    /*p24.MEDA*/ next.VSYNC_OUTn.tock(lcd.NEW_LINE_d4a, VID_RESETn2, LINE_000n);
    /*p24.MURE*/ bool MURE = not(lcd.VSYNC_OUTn);
    next.PIN_S = MURE;
  }

#endif


#if 0
    //next.LINE_STROBE.reset(vid_clk.SONO_AxxxxFGH);
    //next.PIN_CPG = nor(false, next.NEW_LINE_d0a);
    //next.CPEN_LATCH = 0;
    //next.PIN_CP = !vid.FINE_MATCH_TRIG;

    next.RUJU = true;
    next.POME = true;
    next.PIN_ST  = next.RUJU;
    next.PIN_S   = !next.VSYNC_OUTn;
    next.PIN_CPL = !amux2(LCDC_EN, next.NEW_LINE_d0a, !LCDC_EN, !DIV_06n);
    next.PIN_FR  = !amux2(LCDC_EN, xor(next.FRAME_EVEN, next.LINE_EVEN), !LCDC_EN, !DIV_07n);

    //next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);
    //next.FRAME_EVEN.reset(next.VBLANK_d4);
    //next.VSYNC_OUTn.reset(next.NEW_LINE_d4a);
    //next.LINE_EVEN.reset(!next.NEW_LINE_d0a);

#endif




#if 0
    ///*p21.PARU*/ next.VBLANK_d4b = next.VBLANK_d4;


    ///*p21.TEGY*/ bool LINE_STROBEa = nand(lcd_x != 0, lcd_x != 7, lcd_x != 45, lcd_x != 83);
    ///*p21.SYGU*/ next.LINE_STROBE.tock(vid_clk.SONO_AxxxxFGH, 1, LINE_STROBEa);
    //next.PIN_CPG = nor(next.LINE_STROBE, next.NEW_LINE_d0a);

    {
      // weird gate
      ///*p21.WUSA*/ if (vid.X0 && vid.X3) next.CPEN_LATCH = 1;
      ///*p21.WUSA*/ if (vid.RENDER_DONE_SYNC)  next.CPEN_LATCH = 0;

      //next.PIN_CP = !or(and(vid.CLKPIPE, next.CPEN_LATCH), vid.FINE_MATCH_TRIG);
    }

    if (next.X_8_SYNC) {
      next.RUJU = true;
      next.POME = true;
    }

    if (SCAN_DONE_d0_TRIG) {
      next.RUJU = false;
      next.POME = false;
    }

    //next.PIN_ST  = next.RUJU;
    //next.PIN_S   = !next.VSYNC_OUTn;
    //next.PIN_CPL = !amux2(LCDC_EN, next.NEW_LINE_d0a, !LCDC_EN, !DIV_06n);
    //next.PIN_FR  = !amux2(LCDC_EN, xor(next.FRAME_EVEN, next.LINE_EVEN), !LCDC_EN, !DIV_07n);

    //next.X_8_SYNC.tock(vid.ROXO_4M, vid.RENDERING_LATCH, vid.X3);
    //next.FRAME_EVEN.tock(next.VBLANK_d4, 1, !next.FRAME_EVEN);
    //next.VSYNC_OUTn.tock(next.NEW_LINE_d4a, 1, lcd_y == 0);
    //next.LINE_EVEN.tock(!next.NEW_LINE_d0a, 1, !next.LINE_EVEN);

#endif




#if 0
  if (!vid_rst.VID_RESETn) {
    next.VBLANK_d4b = next.VBLANK_d4;


    next.VID_LINE_TRIG_d4n = 0;
    next.VID_LINE_TRIG_d4a = 1;
    next.VID_LINE_TRIG_d4o = 0;

    next.VID_LINE_TRIG_d4p = 0;
    next.VID_LINE_TRIG_d4c = 1;

    next.VID_LINE_d4.reset(vid_clk.XUPY_xBCxxFGx);
    next.VID_LINE_d6.reset(vid_clk.AWOH_AxxDExxH);

    next.VBLANK_d4.reset(next.NEW_LINE_d4a);
  
    next.NEW_LINE_d4a.reset(vid_clk.TALU_xBCDExxx);
    next.LINE_153_d4.reset(next.NEW_LINE_d4a);

    next.X0.reset(vid_clk.TALU_xBCDExxx);
    next.X1.reset(!next.X0);
    next.X2.reset(!next.X1);
    next.X3.reset(!next.X2);
    next.X4.reset(!next.X3);
    next.X5.reset(!next.X4);
    next.X6.reset(!next.X5);

    next.NEW_LINE_d0a.reset(vid_clk.SONO_AxxxxFGH);

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

    /*p28.BYHA*/ next.VID_LINE_TRIG_d4n = or(next.VID_LINE_d6, !next.VID_LINE_d4);
    /*p28.ATEJ*/ next.VID_LINE_TRIG_d4a = not(next.VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ next.VID_LINE_TRIG_d4o = not(next.VID_LINE_TRIG_d4a);

    /*p28.ABAK*/ bool VID_LINE_TRIG_d4b = next.VID_LINE_TRIG_d4a;
    /*p28.BYVA*/ next.VID_LINE_TRIG_d4p = not(VID_LINE_TRIG_d4b);
    /*p29.DYBA*/ next.VID_LINE_TRIG_d4c = not(next.VID_LINE_TRIG_d4p);

    // Horizontal sync

    /*p21.LAMA*/ bool FRAME_RSTn = nor(next.LINE_153_d4, !1);


    /*p29.ABOV*/ bool VID_LINE_d0 = and(next.NEW_LINE_d0a, lcd_y < 144);
    next.VID_LINE_d4.tock(vid_clk.XUPY_xBCxxFGx, 1, VID_LINE_d0);
    next.VID_LINE_d6.tock(vid_clk.AWOH_AxxDExxH, 1, next.VID_LINE_d4);

    next.VBLANK_d4.tock(next.NEW_LINE_d4a, 1, lcd_y >= 144);
  
    next.NEW_LINE_d4a.tock(vid_clk.TALU_xBCDExxx, 1, next.NEW_LINE_d0a);
    next.LINE_153_d4.tock(next.NEW_LINE_d4a, 1, lcd_y == 153);

    /*p21.MUDE*/ bool X_RSTn = not(next.NEW_LINE_d0a);
    next.X0.tock(vid_clk.TALU_xBCDExxx, X_RSTn, !next.X0);
    next.X1.tock(!next.X0, X_RSTn, !next.X1);
    next.X2.tock(!next.X1, X_RSTn, !next.X2);
    next.X3.tock(!next.X2, X_RSTn, !next.X3);
    next.X4.tock(!next.X3, X_RSTn, !next.X4);
    next.X5.tock(!next.X4, X_RSTn, !next.X5);
    next.X6.tock(!next.X5, X_RSTn, !next.X6);

    next.NEW_LINE_d0a.tock(vid_clk.SONO_AxxxxFGH, 1, lcd_x == 113);

    next.Y0.tock(next.NEW_LINE_d0a, FRAME_RSTn, !next.Y0);
    next.Y1.tock(!next.Y0, FRAME_RSTn, !next.Y1);
    next.Y2.tock(!next.Y1, FRAME_RSTn, !next.Y2);
    next.Y3.tock(!next.Y2, FRAME_RSTn, !next.Y3);
    next.Y4.tock(!next.Y3, FRAME_RSTn, !next.Y4);
    next.Y5.tock(!next.Y4, FRAME_RSTn, !next.Y5);
    next.Y6.tock(!next.Y5, FRAME_RSTn, !next.Y6);
    next.Y7.tock(!next.Y6, FRAME_RSTn, !next.Y7);
  }
#endif