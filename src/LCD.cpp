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
  X = 99;
  Y = 0;
  NEW_LINE_d0 = 0;
  NEW_LINE_d4 = 0;
  LINE_153_d4 = 1;
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

/*p28.AWOH*/ wire AWOH_AxxDExxH = vclk_reg.WUVU_AxxDExxH;
/*p29.XUPY*/ wire XUPY_xBCxxFGx = !vclk_reg.WUVU_AxxDExxH;

/*p21.TALU*/ wire TALU_xBCDExxx = vclk_reg.VENA_xBCDExxx;
/*p21.SONO*/ wire SONO_AxxxxFGH = !vclk_reg.VENA_xBCDExxx;

/*p01.ATAR*/ wire ATAR_VID_RST = !cfg_reg.LCDC_EN;


/*p21.NOKO*/ wire LINE_153 = (Y & 153) == 153;
/*p21.SANU*/ wire LINE_END = (X & 113) == 113;
/*p21.MUDE*/ wire X_RSTn = nor(NEW_LINE_d0, ATAR_VID_RST);
/*p21.LAMA*/ wire Y_RSTn = nor(LINE_153_d4, ATAR_VID_RST);

/*p21.RUTU*/ NEW_LINE_d0.set(SONO_AxxxxFGH, !ATAR_VID_RST, LINE_END);
/*p21.NYPE*/ NEW_LINE_d4.set(TALU_xBCDExxx, !ATAR_VID_RST, NEW_LINE_d0);

/*p21.MYTA*/ LINE_153_d4.set(NEW_LINE_d4, !ATAR_VID_RST, LINE_153);

/*p21.SAXO*/ X0.set(TALU_xBCDExxx, X_RSTn, !X0);
/*p21.TYPO*/ X1.set(!X0, X_RSTn, !X1);
/*p21.VYZO*/ X2.set(!X1, X_RSTn, !X2);
/*p21.TELU*/ X3.set(!X2, X_RSTn, !X3);
/*p21.SUDE*/ X4.set(!X3, X_RSTn, !X4);
/*p21.TAHA*/ X5.set(!X4, X_RSTn, !X5);
/*p21.TYRY*/ X6.set(!X5, X_RSTn, !X6);

/*p21.MUWY*/ Y0.set(NEW_LINE_d0, Y_RSTn, !Y0);
/*p21.MYRO*/ Y1.set(!Y0, Y_RSTn, !Y1);
/*p21.LEXA*/ Y2.set(!Y1, Y_RSTn, !Y2);
/*p21.LYDO*/ Y3.set(!Y2, Y_RSTn, !Y3);
/*p21.LOVU*/ Y4.set(!Y3, Y_RSTn, !Y4);
/*p21.LEMA*/ Y5.set(!Y4, Y_RSTn, !Y5);
/*p21.MATO*/ Y6.set(!Y5, Y_RSTn, !Y6);
/*p21.LAFO*/ Y7.set(!Y6, Y_RSTn, !Y7);


#endif

void LCD::tock(int phase, const Req& /*req*/, bool LCDC_EN) {
  if (PHASE_F) {
    if (X == 113 && !NEW_LINE_d0) Y++;
    NEW_LINE_d0 = (X == 113);
  }

  if (PHASE_B) {
    X++;
    if (NEW_LINE_d0 && !NEW_LINE_d4) LINE_153_d4 = (Y == 153);
    NEW_LINE_d4 = NEW_LINE_d0;
  }

  if (NEW_LINE_d0) X = 0;
  if (LINE_153_d4) Y = 0;

  if (!LCDC_EN) {
    X = 0;
    Y = 0;
    NEW_LINE_d0 = 0;
    NEW_LINE_d4 = 0;
    LINE_153_d4 = 0;
  }
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& d) {
  sprintf(d, "\002--------------LCD--------------\001\n");

  sprintf(d, "X            = %d\n", X);
  sprintf(d, "Y            = %d\n", Y);
  sprintf(d, "NEW_LINE_d0  = %d\n", NEW_LINE_d0);
  sprintf(d, "NEW_LINE_d4  = %d\n", NEW_LINE_d4);
  sprintf(d, "LINE_153_d4  = %d\n", LINE_153_d4);
}

//-----------------------------------------------------------------------------
