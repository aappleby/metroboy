#include "Schematics.h"

#define TCYCLES_LINE      456
#define TCYCLES_OAM       80
#define TCYCLES_VRAM      172
#define TCYCLES_HBLANK    204

#define HBLANK_DELAY_START  8

namespace Schematics {

struct FrameTimer {
  bool NEW_LINEn;
  bool NEW_LINE1;
  bool NEW_LINE2;
  bool NEW_LINE3;
  bool NEW_LINE4;

  bool CNT0,CNT1,CNT2,CNT3,CNT4,CNT5,CNT6;
  bool V0,V1,V2,V3,V4,V5,V6,V7;

  bool LINE_ENDo;
  bool LINE_ENDp;

  bool LINE_153_SYNC;
  bool REG_VBLANK;

  /*p29.ABOV*/ bool IN_LINEa;
  /*p29.CATU*/ bool IN_LINEb;
  /*p28.ANEL*/ bool IN_LINEc;
};

struct FrameTimerIn {
  bool VID_RESETn;

  /*p29.XUPY*/ bool CLK_AxxDExxH;
  /*p28.AWOH*/ bool CLK_xBCxxFGx;

  /*p21.TALU*/ bool CLK_AxxxxFGH;
  /*p21.SONO*/ bool CLK_xBCDExxx;


};

void FrameTimer_tick(const FrameTimerIn& inA, const FrameTimerIn& inB,
                     const FrameTimer& a, const FrameTimer& b, FrameTimer& c)
{
#if 0
  /*P09.HAPO*/ c.sys.SYS_RESET2  = not(b.sys.SYS_RESETn); // apu

  /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
  /*p01.CUNU*/ c.sys.SYS_RESETn2 = not(DULA_RESET); // video, sprites, dma

  /*p01.XORE*/ wire XORE_RESET = not(b.sys.SYS_RESETn2);
  /*p01.WESY*/ c.sys.SYS_RESETn6 = not(XORE_RESET); // video
  /*p01.XARE*/ c.sys.SYS_RESETn7 = not(XORE_RESET);
  /*p01.WALU*/ c.sys.SYS_RESETn5 = not(XORE_RESET);
  /*p01.XEBE*/ c.sys.SYS_RESETn4 = not(XORE_RESET); // sprites, video

#endif
  /*p01.LYHA*/ wire VID_RESET2  = not(inB.VID_RESETn);
  /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
  /*p01.ATAR*/ wire VID_RESET6  = not(inB.VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
  /*p01.AMYG*/ wire VID_RESET7  = not(inB.VID_RESETn);

  // 113 = 64 + 32 + 16 + 1
  /*p21.SANU*/ wire LINE_ENDn = nand(b.CNT6, b.CNT5, b.CNT4, b.CNT0);
  /*p21.RUTU*/ c.LINE_ENDo = tock_pos(inA.CLK_xBCDExxx, inB.CLK_xBCDExxx, VID_RESETn2, b.LINE_ENDo, LINE_ENDn);
  /*p21.NYPE*/ c.LINE_ENDp = tock_pos(inA.CLK_AxxxxFGH, inB.CLK_AxxxxFGH, VID_RESETn2, b.LINE_ENDp, b.LINE_ENDo);

  /*p21.MUDE*/ wire CNT_RSTn = nor(b.LINE_ENDo, VID_RESET2);
  /*p21.SAXO*/ c.CNT0 = tock_pos(inA.CLK_AxxxxFGH, inB.CLK_AxxxxFGH, CNT_RSTn, c.CNT0, !c.CNT0);
  /*p21.TYPO*/ c.CNT1 = tock_pos(!a.CNT0,          !b.CNT0,          CNT_RSTn, c.CNT1, !c.CNT1);
  /*p21.VYZO*/ c.CNT2 = tock_pos(!a.CNT1,          !b.CNT1,          CNT_RSTn, c.CNT2, !c.CNT2);
  /*p21.TELU*/ c.CNT3 = tock_pos(!a.CNT2,          !b.CNT2,          CNT_RSTn, c.CNT3, !c.CNT3);
  /*p21.SUDE*/ c.CNT4 = tock_pos(!a.CNT3,          !b.CNT3,          CNT_RSTn, c.CNT4, !c.CNT4);
  /*p21.TAHA*/ c.CNT5 = tock_pos(!a.CNT4,          !b.CNT4,          CNT_RSTn, c.CNT5, !c.CNT5);
  /*p21.TYRY*/ c.CNT6 = tock_pos(!a.CNT5,          !b.CNT5,          CNT_RSTn, c.CNT6, !c.CNT6);

  /*p21.XYVO*/ wire IN_VBLANK  = and(b.V4, b.V7); // 128 + 16 = 144
  /*p29.ALES*/ wire IN_VBLANKn = not(IN_VBLANK);

  /*p21.PURE*/ wire LINE_ENDa = not(b.LINE_ENDo);
  /*p21.SELA*/ wire LINE_ENDq = not(LINE_ENDa);

  /*p29.ABOV*/ c.IN_LINEa = and(LINE_ENDq, IN_VBLANKn);
  /*p29.CATU*/ c.IN_LINEb = tock_pos(inA.CLK_AxxDExxH, inB.CLK_AxxDExxH, VID_RESETn3, b.IN_LINEb, b.IN_LINEa);
  /*p28.ANEL*/ c.IN_LINEc = tock_pos(inA.CLK_xBCxxFGx, inB.CLK_xBCxxFGx, VID_RESETn3, b.IN_LINEc, b.IN_LINEb);

  /*p28.ABAF*/ wire IN_LINEn  = not(b.IN_LINEb);
  /*p28.BYHA*/ c.NEW_LINEn = and(or(b.IN_LINEc, IN_LINEn), VID_RESETn3);
  /*p28.ATEJ*/ c.NEW_LINE1 = not(b.NEW_LINEn);
  /*p28.ABAK*/ c.NEW_LINE2 = or (b.NEW_LINE1, VID_RESET7);
  /*p28.BYVA*/ c.NEW_LINE3 = not(b.NEW_LINE2);
  /*p29.DYBA*/ c.NEW_LINE4 = not(b.NEW_LINE3);

  // Schematic wrong
  // NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

  /*p21.NOKO*/ wire LINE_153 = and(b.V7, b.V4, b.V3, b.V0);
  /*p21.MYTA*/ c.LINE_153_SYNC = tock_pos(a.LINE_ENDp, b.LINE_ENDp, VID_RESETn2, b.LINE_153_SYNC, LINE_153);
  /*p21.LAMA*/ wire FRAME_RSTn = nor(b.LINE_153_SYNC, VID_RESET2);

  /*p21.MUWY*/ c.V0 = tock_pos(a.LINE_ENDo,  b.LINE_ENDo, FRAME_RSTn, c.V0, !c.V0);
  /*p21.MYRO*/ c.V1 = tock_pos(!a.V0,        !b.V0,       FRAME_RSTn, c.V1, !c.V1);
  /*p21.LEXA*/ c.V2 = tock_pos(!a.V1,        !b.V1,       FRAME_RSTn, c.V2, !c.V2);
  /*p21.LYDO*/ c.V3 = tock_pos(!a.V2,        !b.V2,       FRAME_RSTn, c.V3, !c.V3);
  /*p21.LOVU*/ c.V4 = tock_pos(!a.V3,        !b.V3,       FRAME_RSTn, c.V4, !c.V4);
  /*p21.LEMA*/ c.V5 = tock_pos(!a.V4,        !b.V4,       FRAME_RSTn, c.V5, !c.V5);
  /*p21.MATO*/ c.V6 = tock_pos(!a.V5,        !b.V5,       FRAME_RSTn, c.V6, !c.V6);
  /*p21.LAFO*/ c.V7 = tock_pos(!a.V6,        !b.V6,       FRAME_RSTn, c.V7, !c.V7);

  /*p21.POPU*/ c.REG_VBLANK  = tock_pos(a.LINE_ENDp, b.LINE_ENDp, VID_RESETn2, b.REG_VBLANK, IN_VBLANK);
}


};