#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// just the counters

#if 0
struct LCD2 {

  // Resets to immediately before the first phase on the first line
  void reset() {
    X0.val=true ; X0.clk=true ;
    X1.val=false; X1.clk=false;
    X2.val=false; X2.clk=true ;
    X3.val=false; X3.clk=true ;
    X4.val=true ; X4.clk=true ;
    X5.val=true ; X5.clk=false;
    X6.val=true ; X6.clk=false;
    Y0.val=false; Y0.clk=false;
    Y1.val=false; Y1.clk=true ;
    Y2.val=false; Y2.clk=true ;
    Y3.val=false; Y3.clk=true ;
    Y4.val=false; Y4.clk=true ;
    Y5.val=false; Y5.clk=true ;
    Y6.val=false; Y6.clk=true ;
    Y7.val=false; Y7.clk=true ;
    NEW_LINE_d0a.val=false; NEW_LINE_d0a.clk=false;
    NEW_LINE_d4a.val=false; NEW_LINE_d4a.clk=true;
    LINE_153_d4.val=true; LINE_153_d4.clk=false;
  }

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

  /*p21.RUTU*/ Reg NEW_LINE_d0a;
  /*p21.NYPE*/ Reg NEW_LINE_d4a;

  /*p21.MYTA*/ Reg LINE_153_d4;

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }
};

void LCD_tick2(const LCD2& lcd,
               bool VID_RESETn,
               bool SONO_ABCDxxxx,
               bool TALU_xxxxEFGH,
               LCD2& next);
#endif

//-----------------------------------------------------------------------------

struct LCD {

  static void tick(const Clocks& clk,
                   const Resets& rst,
                   const LCD& lcd,
                   const Video& vid,
                   bool SCAN_DONE_d0_TRIG,
                   bool DIV_06n,
                   bool DIV_07n,
                   bool LCDC_EN,
                   LCD& next);

  static void tick2(const Clocks& clk,
                    const Resets& rst,
                    const LCD& lcd,
                    const Video& vid,
                    bool SCAN_DONE_d0_TRIG,
                    bool DIV_06n,
                    bool DIV_07n,
                    bool LCDC_EN,
                    LCD& next);

  // Resets to immediately before the first phase on the first line
  void reset() {
    X0.val=true ; X0.clk=true ;
    X1.val=false; X1.clk=false;
    X2.val=false; X2.clk=true ;
    X3.val=false; X3.clk=true ;
    X4.val=true ; X4.clk=true ;
    X5.val=true ; X5.clk=false;
    X6.val=true ; X6.clk=false;
    Y0.val=false; Y0.clk=false;
    Y1.val=false; Y1.clk=true ;
    Y2.val=false; Y2.clk=true ;
    Y3.val=false; Y3.clk=true ;
    Y4.val=false; Y4.clk=true ;
    Y5.val=false; Y5.clk=true ;
    Y6.val=false; Y6.clk=true ;
    Y7.val=false; Y7.clk=true ;
    NEW_LINE_d0a.val=false; NEW_LINE_d0a.clk=false;
    NEW_LINE_d4a.val=false; NEW_LINE_d4a.clk=true;
    LINE_153_d4.val=true; LINE_153_d4.clk=false;
  }

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }

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

  /*p21.RUTU*/ Reg NEW_LINE_d0a;
  /*p21.NYPE*/ Reg NEW_LINE_d4a;

  /*p21.NOKO*/ bool LINE_153_d0;
  /*p21.MYTA*/ Reg  LINE_153_d4;

  /*p21.POPU*/ Reg  VBLANK_d4;
  /*p21.PARU*/ bool VBLANK_d4n;

  /*p21.PURE*/ bool NEW_LINE_d0n;
  /*p21.SELA*/ bool NEW_LINE_d0b;

  /*p21.XYVO*/ bool LINE_144_d0;
  /*p29.ALES*/ bool LINE_144_d0n;

  /*p29.ABOV*/ bool VID_LINE_d0;
  /*p29.CATU*/ Reg  VID_LINE_d4;
  /*p28.ANEL*/ Reg  VID_LINE_d6;

  /*p28.BYHA*/ bool VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ bool VID_LINE_TRIG_d4a;
  /*p28.ABAK*/ bool VID_LINE_TRIG_d4b;
  /*p28.BYVA*/ bool VID_LINE_TRIG_d4p;
  /*p29.DYBA*/ bool VID_LINE_TRIG_d4c;
  /*p27.XAHY*/ bool VID_LINE_TRIG_d4o;

#if 0

  /*p24.MEDA*/ Reg VSYNC_OUTn;

  /*p24.LUCA*/ Reg LINE_EVEN;
  /*p21.NAPO*/ Reg FRAME_EVEN;

  /*p21.SYGU*/ Reg LINE_STROBE;




  /*p21.WUSA*/ bool CPEN_LATCH;

  /*p24.POME*/ bool POME; 
  /*p24.RUJU*/ bool RUJU;

  //----------
  // LCD pins

  /* PIN_50 */ bool PIN_LD1;
  /* PIN_51 */ bool PIN_LD0;
  /* PIN_52 */ bool PIN_CPG;
  /* PIN_53 */ bool PIN_CP;
  /* PIN_54 */ bool PIN_ST;
  /* PIN_55 */ bool PIN_CPL;
  /* PIN_56 */ bool PIN_FR;
  /* PIN_57 */ bool PIN_S;
#endif


  /*p24.PAHO*/ Reg X_8_SYNC;

};

//-----------------------------------------------------------------------------

};