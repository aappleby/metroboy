#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void Sprites_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p28.WEFE*/ c.p28.P10_Bn = not(b.chip.P10_B);


  //----------

  /*p28.AWOH*/ c.p28.CLK_2Mn = not(b.p21.CLK_2Mb);

  // this drives sprite count reset, it must be new line.
  /*p28.ATEJ*/ c.p28.NEW_LINE = not(b.p28.NEW_LINEn);
  /*p28.BYHA*/   c.p28.NEW_LINEn = or(b.p28.IN_LINE_SYNC, b.p28.IN_LINEn, b.sys.VID_RESETn3);
  /*p28.ANEL*/     c.p28.IN_LINE_SYNC = tock_pos(a.p28.CLK_2Mn, b.p28.CLK_2Mn, b.sys.VID_RESETn3, b.p28.IN_LINE_SYNC, b.p29.IN_LINE);
  /*p28.ABAF*/     c.p28.IN_LINEn = not(b.p29.IN_LINE);


  //----------
  // sprite scan counter

  /*p28.FETO*/ c.p28.SCAN_DONE = and(b.p28.SCAN0, b.p28.SCAN1, b.p28.SCAN2, b.p28.SCAN5);
  /*p28.GAVA*/ c.p28.SCAN_CLK  = or(b.p28.SCAN_DONE, b.p21.CLK_2Mb);
  /*p28.ANOM*/ c.p28.SCAN_RSTn = nor(b.sys.VID_RESET6, b.p28.NEW_LINE);

  /*p28.YFEL*/ c.p28.SCAN0 = tock_pos( a.p28.SCAN_CLK, b.p28.SCAN_CLK, b.p28.SCAN_RSTn, b.p28.SCAN0, !b.p28.SCAN0);
  /*p28.WEWY*/ c.p28.SCAN1 = tock_pos(!a.p28.SCAN0,    !b.p28.SCAN0,   b.p28.SCAN_RSTn, b.p28.SCAN1, !b.p28.SCAN1);
  /*p28.GOSO*/ c.p28.SCAN2 = tock_pos(!a.p28.SCAN1,    !b.p28.SCAN1,   b.p28.SCAN_RSTn, b.p28.SCAN2, !b.p28.SCAN2);
  /*p28.ELYN*/ c.p28.SCAN3 = tock_pos(!a.p28.SCAN2,    !b.p28.SCAN2,   b.p28.SCAN_RSTn, b.p28.SCAN3, !b.p28.SCAN3);
  /*p28.FAHA*/ c.p28.SCAN4 = tock_pos(!a.p28.SCAN3,    !b.p28.SCAN3,   b.p28.SCAN_RSTn, b.p28.SCAN4, !b.p28.SCAN4);
  /*p28.FONY*/ c.p28.SCAN5 = tock_pos(!a.p28.SCAN4,    !b.p28.SCAN4,   b.p28.SCAN_RSTn, b.p28.SCAN5, !b.p28.SCAN5);

  //----------

  /*p28.BESU*/ c.p28.BESU = or(b.p29.IN_LINE, b.p28.ASEN);
  /*p28.ASEN*/   c.p28.ASEN = or(b.sys.VID_RESET6, b.p29.AVAP);

  /*p28.XUTO*/ c.p28.OAM_WR = and(b.sys.ADDR_OAM, b.sys.CPU_WR2);


  /*p28.BOFE*/ c.p28.FROM_CPU5n = not(b.sys.FROM_CPU5);



  //----------











  //----------

  /*p28.AZUL*/ c.p28.D_TO_OAMD = not(b.p28.D_TO_OAMDn);
  /*p28.APAG*/   c.p28.D_TO_OAMDn = amux2(b.p28.XUPA, b.p28.AMAB, b.p28.AJUJ, b.p28.ADDR_OAMn);
  /*p28.XUPA*/     c.p28.XUPA    = not(b.p28.WUJE);
  /*p28.WUJE*/       c.p28.WUJE   = or(b.p28.XYNY, b.p28.OAM_WR);
  /*p28.XYNY*/         c.p28.XYNY = not(b.sys.CPUCLK_ABCDxxxx9);
  /*p28.AMAB*/     c.p28.AMAB = and(b.sys.ADDR_OAM, b.p28.AJUJ);
  /*p28.AJUJ*/     c.p28.AJUJ = nor(b.sys.DMA_RUNNING_SYNCn, b.p28.OAM_ADDR_PARSEn, b.p28.OAM_ADDR_RENDERn);
  /*p28.ADAH*/     c.p28.ADDR_OAMn        = not(b.sys.ADDR_OAM);


  /*p28.AZAR*/ c.p28.VRAM_TO_OAMc = not(b.sys.VRAM_TO_OAMn);
  /*p25.CEDE*/ c.p25.DO_DMAn = not(b.sys.DO_DMA);

  /*p28.ZAXA*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D0 = b.D0;
  /*p28.ZAKY*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D1 = b.D1;
  /*p28.WULE*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D2 = b.D2;
  /*p28.ZOZO*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D3 = b.D3;
  /*p28.ZUFO*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D4 = b.D4;
  /*p28.ZATO*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D5 = b.D5;
  /*p28.YVUC*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D6 = b.D6;
  /*p28.ZUFE*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_A_D7 = b.D7;

  /*p28.ZAMY*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D0 = b.D0;
  /*p28.ZOPU*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D1 = b.D1;
  /*p28.WYKY*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D2 = b.D2;
  /*p28.ZAJA*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D3 = b.D3;
  /*p28.ZUGA*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D4 = b.D4;
  /*p28.ZUMO*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D5 = b.D5;
  /*p28.XYTO*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D6 = b.D6;
  /*p28.ZYFA*/ if (b.p28.D_TO_OAMD)    c.TS_OAM_B_D7 = b.D7;

  /*p28.WUZU*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D0 = b.MD0;
  /*p28.AXER*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D1 = b.MD1;
  /*p28.ASOX*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D2 = b.MD2;
  /*p28.CETU*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D3 = b.MD3;
  /*p28.ARYN*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D4 = b.MD4;
  /*p28.ACOT*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D5 = b.MD5;
  /*p28.CUJE*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D6 = b.MD6;
  /*p28.ATER*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_A_D7 = b.MD7;

  /*p28.WOWA*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D0 = b.MD0;
  /*p28.AVEB*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D1 = b.MD1;
  /*p28.AMUH*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D2 = b.MD2;
  /*p28.COFO*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D3 = b.MD3;
  /*p28.AZOZ*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D4 = b.MD4;
  /*p28.AGYK*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D5 = b.MD5;
  /*p28.BUSE*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D6 = b.MD6;
  /*p28.ANUM*/ if (b.p28.VRAM_TO_OAMc) c.TS_OAM_B_D7 = b.MD7;

  /*p25.WEJO*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D0 = not(b.p25.D0n);
  /*p25.BUBO*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D1 = not(b.p25.D1n);
  /*p25.BETU*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D2 = not(b.p25.D2n);
  /*p25.CYME*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D3 = not(b.p25.D3n);
  /*p25.BAXU*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D4 = not(b.p25.D4n);
  /*p25.BUHU*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D5 = not(b.p25.D5n);
  /*p25.BYNY*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D6 = not(b.p25.D6n);
  /*p25.BYPY*/ if (b.p25.DO_DMAn)      c.TS_OAM_A_D7 = not(b.p25.D7n);

  /*p25.WASA*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D0 = not(b.p25.D0n);
  /*p25.BOMO*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D1 = not(b.p25.D1n);
  /*p25.BASA*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D2 = not(b.p25.D2n);
  /*p25.CAKO*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D3 = not(b.p25.D3n);
  /*p25.BUMA*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D4 = not(b.p25.D4n);
  /*p25.BUPY*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D5 = not(b.p25.D5n);
  /*p25.BASY*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D6 = not(b.p25.D6n);
  /*p25.BAPE*/ if (b.p25.DO_DMAn)      c.TS_OAM_B_D7 = not(b.p25.D7n);


  //----------

  /*p31.XYKY*/ c.p31.OAM_A_LATCH0 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH0, b.TS_OAM_A_D0);
  /*p31.YRUM*/ c.p31.OAM_A_LATCH1 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH1, b.TS_OAM_A_D1);
  /*p31.YSEX*/ c.p31.OAM_A_LATCH2 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH2, b.TS_OAM_A_D2);
  /*p31.YVEL*/ c.p31.OAM_A_LATCH3 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH3, b.TS_OAM_A_D3);
  /*p31.WYNO*/ c.p31.OAM_A_LATCH4 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH4, b.TS_OAM_A_D4);
  /*p31.CYRA*/ c.p31.OAM_A_LATCH5 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH5, b.TS_OAM_A_D5);
  /*p31.ZUVE*/ c.p31.OAM_A_LATCH6 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH6, b.TS_OAM_A_D6);
  /*p31.ECED*/ c.p31.OAM_A_LATCH7 = latch_pos(b.p28.OAM_LATCH, b.p31.OAM_A_LATCH7, b.TS_OAM_A_D7);

  /*p29.YDYV*/ c.p29.OAM_B_LATCH0 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH0, b.TS_OAM_B_D0);
  /*p29.YCEB*/ c.p29.OAM_B_LATCH1 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH1, b.TS_OAM_B_D1);
  /*p29.ZUCA*/ c.p29.OAM_B_LATCH2 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH2, b.TS_OAM_B_D2);
  /*p29.WONE*/ c.p29.OAM_B_LATCH3 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH3, b.TS_OAM_B_D3);
  /*p29.ZAXE*/ c.p29.OAM_B_LATCH4 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH4, b.TS_OAM_B_D4);
  /*p29.XAFU*/ c.p29.OAM_B_LATCH5 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH5, b.TS_OAM_B_D5);
  /*p29.YSES*/ c.p29.OAM_B_LATCH6 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH6, b.TS_OAM_B_D6);
  /*p29.ZECA*/ c.p29.OAM_B_LATCH7 = latch_pos(b.p28.OAM_LATCH, b.p29.OAM_B_LATCH7, b.TS_OAM_B_D7);

  /*p31.YLOR*/ c.p31.OAM_A_D0 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D0, b.p31.OAM_A_LATCH0);
  /*p31.ZYTY*/ c.p31.OAM_A_D1 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D1, b.p31.OAM_A_LATCH1);
  /*p31.ZYVE*/ c.p31.OAM_A_D2 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D2, b.p31.OAM_A_LATCH2);
  /*p31.ZEZY*/ c.p31.OAM_A_D3 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D3, b.p31.OAM_A_LATCH3);
  /*p31.GOMO*/ c.p31.OAM_A_D4 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D4, b.p31.OAM_A_LATCH4);
  /*p31.BAXO*/ c.p31.OAM_A_D5 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D5, b.p31.OAM_A_LATCH5);
  /*p31.YZOS*/ c.p31.OAM_A_D6 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D6, b.p31.OAM_A_LATCH6);
  /*p31.DEPO*/ c.p31.OAM_A_D7 = tock_pos(a.p31.OAM_A_IN_CLK, b.p31.OAM_A_IN_CLK, 0, b.p31.OAM_A_D7, b.p31.OAM_A_LATCH7);

  /*p29.XUSO*/ c.p29.OAM_B_D0 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D0, b.p29.OAM_B_LATCH0);
  /*p29.XEGU*/ c.p29.OAM_B_D1 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D1, b.p29.OAM_B_LATCH1);
  /*p29.YJEX*/ c.p29.OAM_B_D2 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D2, b.p29.OAM_B_LATCH2);
  /*p29.XYJU*/ c.p29.OAM_B_D3 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D3, b.p29.OAM_B_LATCH3);
  /*p29.YBOG*/ c.p29.OAM_B_D4 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D4, b.p29.OAM_B_LATCH4);
  /*p29.WYSO*/ c.p29.OAM_B_D5 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D5, b.p29.OAM_B_LATCH5);
  /*p29.XOTE*/ c.p29.OAM_B_D6 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D6, b.p29.OAM_B_LATCH6);
  /*p29.YZAB*/ c.p29.OAM_B_D7 = tock_pos(a.p29.OAM_B_IN_CLK, b.p29.OAM_B_IN_CLK, 0, b.p29.OAM_B_D7, b.p29.OAM_B_LATCH7);

  //----------

  /*p31.XACA*/ if (b.p28.OAM_A_CPU_RD) c.D0 = b.p31.OAM_A_LATCH0;
  /*p31.XAGU*/ if (b.p28.OAM_A_CPU_RD) c.D1 = b.p31.OAM_A_LATCH1;
  /*p31.XEPU*/ if (b.p28.OAM_A_CPU_RD) c.D2 = b.p31.OAM_A_LATCH2;
  /*p31.XYGU*/ if (b.p28.OAM_A_CPU_RD) c.D3 = b.p31.OAM_A_LATCH3;
  /*p31.XUNA*/ if (b.p28.OAM_A_CPU_RD) c.D4 = b.p31.OAM_A_LATCH4;
  /*p31.DEVE*/ if (b.p28.OAM_A_CPU_RD) c.D5 = b.p31.OAM_A_LATCH5;
  /*p31.ZEHA*/ if (b.p28.OAM_A_CPU_RD) c.D6 = b.p31.OAM_A_LATCH6;
  /*p31.FYRA*/ if (b.p28.OAM_A_CPU_RD) c.D7 = b.p31.OAM_A_LATCH7;

  /*p29.YFAP*/ if (b.p28.OAM_B_CPU_RD) c.D0 = b.p29.OAM_B_LATCH0;
  /*p29.XELE*/ if (b.p28.OAM_B_CPU_RD) c.D1 = b.p29.OAM_B_LATCH1;
  /*p29.YPON*/ if (b.p28.OAM_B_CPU_RD) c.D2 = b.p29.OAM_B_LATCH2;
  /*p29.XUVO*/ if (b.p28.OAM_B_CPU_RD) c.D3 = b.p29.OAM_B_LATCH3;
  /*p29.ZYSA*/ if (b.p28.OAM_B_CPU_RD) c.D4 = b.p29.OAM_B_LATCH4;
  /*p29.YWEG*/ if (b.p28.OAM_B_CPU_RD) c.D5 = b.p29.OAM_B_LATCH5;
  /*p29.XABU*/ if (b.p28.OAM_B_CPU_RD) c.D6 = b.p29.OAM_B_LATCH6;
  /*p29.YTUX*/ if (b.p28.OAM_B_CPU_RD) c.D7 = b.p29.OAM_B_LATCH7;

  //----------
  // OAM address generator

  /*p28.GARO*/ c.p28.A0n = not(b.A00);
  /*p28.WACU*/ c.p28.A1n = not(b.A01);
  /*p28.GOSE*/ c.p28.A2n = not(b.A02);
  /*p28.WAPE*/ c.p28.A3n = not(b.A03);
  /*p28.FEVU*/ c.p28.A4n = not(b.A04);
  /*p28.GERA*/ c.p28.A5n = not(b.A05);
  /*p28.WAXA*/ c.p28.A6n = not(b.A06);
  /*p28.FOBY*/ c.p28.A7n = not(b.A07);

  /*p28.FYKE*/ c.p28.IDX_0n = not(b.p30.TS_IDX_0);
  /*p28.FUGU*/ c.p28.IDX_1n = not(b.p30.TS_IDX_1);
  /*p28.FACO*/ c.p28.IDX_2n = not(b.p30.TS_IDX_2);
  /*p28.FABY*/ c.p28.IDX_3n = not(b.p30.TS_IDX_3);
  /*p28.GYKA*/ c.p28.IDX_4n = not(b.p30.TS_IDX_4);
  /*p28.GYBU*/ c.p28.IDX_5n = not(b.p30.TS_IDX_5);


  /*p28.WUWE*/ c.p28.SCANZn = not(b.chip.P10_B);
  /*p28.GUSE*/ c.p28.SCAN0n = not(b.p28.SCAN0);
  /*p28.GEMA*/ c.p28.SCAN1n = not(b.p28.SCAN1);
  /*p28.FUTO*/ c.p28.SCAN2n = not(b.p28.SCAN2);
  /*p28.FAKU*/ c.p28.SCAN3n = not(b.p28.SCAN3);
  /*p28.GAMA*/ c.p28.SCAN4n = not(b.p28.SCAN4);
  /*p28.GOBY*/ c.p28.SCAN5n = not(b.p28.SCAN5);


  /*p28.FETU*/ c.p28.DMA_A07n = not(b.sys.DMA_A07);
  /*p28.FYDU*/ c.p28.DMA_A06n = not(b.sys.DMA_A06);
  /*p28.EDOL*/ c.p28.DMA_A05n = not(b.sys.DMA_A05);
  /*p28.ELUG*/ c.p28.DMA_A04n = not(b.sys.DMA_A04);
  /*p28.FYKY*/ c.p28.DMA_A03n = not(b.sys.DMA_A03);
  /*p28.FAGO*/ c.p28.DMA_A02n = not(b.sys.DMA_A02);
  /*p28.FESA*/ c.p28.DMA_A01n = not(b.sys.DMA_A01);
  /*p28.FODO*/ c.p28.DMA_A00n = not(b.sys.DMA_A00);

  // polarity issues here
  /*p28.AJON*/ c.p28.OAM_ADDR_RENDERn = and(b.p28.DMA_RUNNING_SYNCb, b.p21.RENDERING);
  /*p28.ACYL*/ c.p28.OAM_ADDR_PARSEn  = and(b.p28.DMA_RUNNING_SYNCb, b.p28.BESU);

  /*p28.ASAM*/ c.p28.OAM_ADDR_CPU    = or(b.p28.OAM_ADDR_PARSEn , b.p21.RENDERING, b.sys.DMA_RUNNING_SYNCn);
  /*p28.BETE*/ c.p28.OAM_ADDR_RENDER = not(b.p28.OAM_ADDR_RENDERn);
  /*p28.APAR*/ c.p28.OAM_ADDR_PARSE  = not(b.p28.OAM_ADDR_PARSEn);
  /*p04.DUGA*/ c.sys.OAM_ADDR_DMA    = not(b.sys.DMA_RUNNING_SYNCn);

  /*p28.GECA*/ c.p28.GECA = not(b.p28.P10_Bn);
  /*p28.WYDU*/ c.p28.WYDU = not(b.p28.P10_Bn);
  /*p28.GEFY*/ c.p28.GEFY = not(b.chip.P10_B);

  // so byte 0 is the palette number? something wrong here...

  /*p28.GEKA*/ c.p28.OAM_A0 = not((b.p28.A0n & b.p28.OAM_ADDR_CPU) | (b.p28.GECA   & b.p28.OAM_ADDR_RENDER) | (b.p28.GEFY   & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A00n & b.sys.OAM_ADDR_DMA));
  /*p28.ZYFO*/ c.p28.OAM_A1 = not((b.p28.A1n & b.p28.OAM_ADDR_CPU) | (b.p28.WYDU   & b.p28.OAM_ADDR_RENDER) | (b.p28.SCANZn & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A01n & b.sys.OAM_ADDR_DMA));
  /*p28.YFOT*/ c.p28.OAM_A2 = not((b.p28.A2n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_5n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN0n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A02n & b.sys.OAM_ADDR_DMA));
  /*p28.YFOC*/ c.p28.OAM_A3 = not((b.p28.A3n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_4n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN1n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A03n & b.sys.OAM_ADDR_DMA));
  /*p28.YVOM*/ c.p28.OAM_A4 = not((b.p28.A4n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_3n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN2n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A04n & b.sys.OAM_ADDR_DMA));
  /*p28.YMEV*/ c.p28.OAM_A5 = not((b.p28.A5n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_2n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN3n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A05n & b.sys.OAM_ADDR_DMA));
  /*p28.XEMU*/ c.p28.OAM_A6 = not((b.p28.A6n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_1n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN4n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A06n & b.sys.OAM_ADDR_DMA));
  /*p28.YZET*/ c.p28.OAM_A7 = not((b.p28.A7n & b.p28.OAM_ADDR_CPU) | (b.p28.IDX_0n & b.p28.OAM_ADDR_RENDER) | (b.p28.SCAN5n & b.p28.OAM_ADDR_PARSE) | (b.p28.DMA_A07n & b.sys.OAM_ADDR_DMA));


  /*p04.LUPA*/ c.sys.WYJA          = unk3(b.p28.AMAB, b.sys.CPU_WR2, b.sys.POWU);

  /*p28.MYNU*/ c.p28.CPU_READ_MYSTERYn = nand(b.sys.CPU_RD2, b.sys.FROM_CPU5);
  /*p28.LEKO*/ c.p28.CPU_READ_MYSTERY  = not(b.p28.CPU_READ_MYSTERYn);

  /*p28.WAFO*/ c.p28.OAM_A0n       = not(b.p28.OAM_A0);
  /*p28.GUKO*/ c.p28.OAM_B_CPU_RDn = and(b.p28.CPU_READ_MYSTERY, b.p28.AMAB, b.p28.OAM_A0n);
  /*p28.WUKU*/ c.p28.OAM_A_CPU_RDn = and(b.p28.CPU_READ_MYSTERY, b.p28.AMAB, b.p28.OAM_A0);


  /*p28.YLYC*/ c.p28.OAM_A_CSn     = and(b.sys.WYJA, b.p28.OAM_A0);
  /*p28.YNYC*/ c.p28.OAM_B_CSn     = and(b.sys.WYJA, b.p28.OAM_A0n);

  /*p28.WEWU*/ c.p28.OAM_A_CPU_RD  = not(b.p28.OAM_A_CPU_RDn);
  /*p28.WUME*/ c.p28.OAM_B_CPU_RD  = not(b.p28.OAM_B_CPU_RDn);





  // This has to be OAM_WR...
  /*p25.CUFE*/ c.p25.OAM_WR = or(and(b.sys.ADDR_OAM, b.sys.DMA_RUNNING_SYNCn), b.sys.CPUCLK_ABCDxxxx9);
  // but then these don't make sense... unless the write signal just turns off the read clocks or something

  /*p25.BYCU*/ c.p25.OAM_IN_CLK   = nor(b.p25.OAM_WR, b.p25.XUJY, b.p25.AVER);
  /*p25.XUJY*/  c.p25.XUJY = not(b.p25.VAPE);
  /*p25.VAPE*/    c.p25.VAPE = and(b.p29.TACU, b.p29.TUVO);
  /*p25.AVER*/  c.p25.AVER = and(b.p28.OAM_ADDR_PARSEn, b.p29.XYSO);
  /*p29.XYSO*/    c.p29.XYSO = not(b.p29.WOJO);
  /*p29.WOJO*/      c.p29.WOJO = nor(!b.p21.CLK_2Ma, !b.p29.CLK_WOSU);

  /*p31.XEGA*/ c.p31.OAM_A_IN_CLK = not(b.p25.OAM_IN_CLKn);
  /*p29.YWOK*/ c.p29.OAM_B_IN_CLK = not(b.p25.OAM_IN_CLKn);

  /*p28.ASYT*/ c.p28.OAM_LATCHn = and(b.p28.AJEP, b.p28.XUJA, b.p28.BOTA);
  /*p28.AJEP*/   c.p28.AJEP = and(b.p28.OAM_ADDR_PARSEn, b.p29.CLK_XOCE);
  /*p28.XUJA*/   c.p28.XUJA = not(b.p28.WEFY);
  /*p28.WEFY*/     c.p28.WEFY = and(b.p29.TUVO, b.p29.TYFO);
  /*p28.BOTA*/   c.p28.BOTA = nor(b.p28.FROM_CPU5n, b.sys.ADDR_OAM, b.sys.CPU_RD2);

  /*p28.BODE*/ c.p28.OAM_LATCH  = not(b.p28.OAM_LATCHn);

  /*p28.YVAL*/ c.p28.OAM_LATCHo = not(b.p28.OAM_LATCH);
  /*p28.YRYV*/ c.p28.OAM_LATCHb = not(b.p28.OAM_LATCHo);

  /*p28.ZODO*/ c.p28.OAM_CLK     = not(b.p28.OAM_LATCHb); // -> OAM
  /*p25.COTA*/ c.p25.OAM_IN_CLKn = not(b.p25.OAM_IN_CLK); // -> OAM
  /*p28.ZONE*/ c.p28.OAM_A_CS    = not(b.p28.OAM_A_CSn); // -> OAM
  /*p28.ZOFE*/ c.p28.OAM_B_CS    = not(b.p28.OAM_B_CSn); // -> OAM

                                                         // these are unused
                                                         /*
                                                         c.p28.XUCA = not(b.p23.FF40_WR);
                                                         c.p28.XECY = tock_pos(a.p28.XUCA, b.p28.XUCA, 0,          b.p28.XECY, b.sys.SYS_RESETn4); // ? weird
                                                         c.p28.XUVA = tock_pos(a.p28.XYNY, b.p28.XYNY, b.sys.XARE, b.p28.XUVA, b.p28.XECY);
                                                         */
  //----------
  // Sprite y comparator

  /*p29.EBOS*/ c.p29.V0n = not(b.p21.V0);
  /*p29.DASA*/ c.p29.V1n = not(b.p21.V1);
  /*p29.FUKY*/ c.p29.V2n = not(b.p21.V2);
  /*p29.FUVE*/ c.p29.V3n = not(b.p21.V3);
  /*p29.FEPU*/ c.p29.V4n = not(b.p21.V4);
  /*p29.FOFA*/ c.p29.V5n = not(b.p21.V5);
  /*p29.FEMO*/ c.p29.V6n = not(b.p21.V6);
  /*p29.GUSU*/ c.p29.V7n = not(b.p21.V7);

  /*p29.ERUC*/ c.p29.ERUC_S = add_c(b.p29.V0n, !b.p29.OAM_B_D0, b.chip.P10_B);
  /*p29.ERUC*/ c.p29.ERUC_C = add_s(b.p29.V0n, !b.p29.OAM_B_D0, b.chip.P10_B);
  /*p29.ENEF*/ c.p29.ENEF_S = add_s(b.p29.V1n, !b.p29.OAM_B_D1, b.p29.ERUC_C);
  /*p29.ENEF*/ c.p29.ENEF_C = add_c(b.p29.V1n, !b.p29.OAM_B_D1, b.p29.ERUC_C);
  /*p29.FECO*/ c.p29.FECO_S = add_s(b.p29.V2n, !b.p29.OAM_B_D2, b.p29.ENEF_C);
  /*p29.FECO*/ c.p29.FECO_C = add_c(b.p29.V2n, !b.p29.OAM_B_D2, b.p29.ENEF_C);
  /*p29.GYKY*/ c.p29.GYKY_S = add_s(b.p29.V3n, !b.p29.OAM_B_D3, b.p29.FECO_C);
  /*p29.GYKY*/ c.p29.GYKY_C = add_c(b.p29.V3n, !b.p29.OAM_B_D3, b.p29.FECO_C);
  /*p29.GOPU*/ c.p29.GOPU_S = add_s(b.p29.V4n, !b.p29.OAM_B_D4, b.p29.GYKY_C);
  /*p29.GOPU*/ c.p29.GOPU_C = add_c(b.p29.V4n, !b.p29.OAM_B_D4, b.p29.GYKY_C);
  /*p29.FUWA*/ c.p29.FUWA_S = add_s(b.p29.V5n, !b.p29.OAM_B_D5, b.p29.GOPU_C);
  /*p29.FUWA*/ c.p29.FUWA_C = add_c(b.p29.V5n, !b.p29.OAM_B_D5, b.p29.GOPU_C);
  /*p29.GOJU*/ c.p29.GOJU_S = add_s(b.p29.V6n, !b.p29.OAM_B_D6, b.p29.FUWA_C);
  /*p29.GOJU*/ c.p29.GOJU_C = add_c(b.p29.V6n, !b.p29.OAM_B_D6, b.p29.FUWA_C);
  /*p29.WUHU*/ c.p29.WUHU_S = add_s(b.p29.V7n, !b.p29.OAM_B_D7, b.p29.GOJU_C);
  /*p29.WUHU*/ c.p29.WUHU_C = add_c(b.p29.V7n, !b.p29.OAM_B_D7, b.p29.GOJU_C);

  /*p29.DEGE*/ c.p29.SPRITE_DELTA0 = not(b.p29.ERUC_S);
  /*p29.DABY*/ c.p29.SPRITE_DELTA1 = not(b.p29.ENEF_S);
  /*p29.DABU*/ c.p29.SPRITE_DELTA2 = not(b.p29.FECO_S);
  /*p29.GYSA*/ c.p29.SPRITE_DELTA3 = not(b.p29.GYKY_S);
  /*p29.GACE*/ c.p29.SPRITE_DELTA4 = not(b.p29.GOPU_S);
  /*p29.GUVU*/ c.p29.SPRITE_DELTA5 = not(b.p29.FUWA_S);
  /*p29.GYDA*/ c.p29.SPRITE_DELTA6 = not(b.p29.GOJU_S);
  /*p29.GEWY*/ c.p29.SPRITE_DELTA7 = not(b.p29.WUHU_S);

  /*p29.GOVU*/ c.p29.GOVU = or(b.p29.GYKY_S, b.p23.LCDC_SPSIZE);

  /*p29.WOTA*/ c.p29.SPR_MATCH_Yn = nand(b.p29.SPRITE_DELTA4,
                                         b.p29.SPRITE_DELTA5,
                                         b.p29.SPRITE_DELTA6,
                                         b.p29.SPRITE_DELTA7,
                                         b.p29.WUHU_C,
                                         b.p29.GOVU);

  /*p29.GESE*/ c.p29.SPR_MATCH_Y = not(b.p29.SPR_MATCH_Yn);

  //----------
  // bottom

  /*p29.WOSU*/ c.p29.CLK_WOSU = tock_pos(a.p21.CLK_AxCxExGxc, b.p21.CLK_AxCxExGxc, b.sys.VID_RESETn1, b.p29.CLK_WOSU, !b.p21.CLK_2Ma);
  /*p29.XOCE*/ c.p29.CLK_XOCE = not(b.p29.CLK_WOSU);


  /*p29.ABOV*/ c.p29.IN_LINEb = and(b.p21.LINE_DONEo, b.p29.LINE_144n);
  /*p29.CATU*/ c.p29.IN_LINE  = tock_pos(a.p21.CLK_2Mb, b.p21.CLK_2Mb, b.sys.VID_RESETn3, b.p29.IN_LINE, b.p29.IN_LINEb);


  /*p29.CENO*/ c.p29.CENO = tock_pos(a.p21.CLK_2Mb, b.p21.CLK_2Mb, b.sys.VID_RESETn3, b.p29.CENO, b.p28.BESU);
  /*p29.CEHA*/ c.p29.CEHA = not(!b.p29.CENO);


  /*p29.ALES*/ c.p29.LINE_144n = not(b.p21.LINE_144);
  /*p29.BUZA*/ c.p29.STORE_SPRITE_IDX = and(!b.p29.CENO, b.p21.RENDERING);

  /*p29.BALU*/ c.p29.SCAN_RSTa = not(b.p28.SCAN_RSTn);
  /*p29.BAGY*/ c.p29.SCAN_RSTo = not(b.p29.SCAN_RSTa);

  /*p29.BYBA*/ c.p29.BYBA = tock_pos(a.p21.CLK_2Mb,       b.p21.CLK_2Mb,       b.p29.SCAN_RSTo, b.p29.BYBA, b.p28.SCAN_DONE);
  /*p29.DOBA*/ c.p29.DOBA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p29.SCAN_RSTo, b.p29.DOBA, b.p29.BYBA);
  /*p29.BEBU*/ c.p29.BEBU = or(b.p29.DOBA, b.p29.SCAN_RSTa, !b.p29.BYBA);
  /*p29.AVAP*/ c.p29.AVAP = not(b.p29.BEBU);

  //----------
  // center thing

  /*p29.TEPA*/ c.p29.TEPA = not(b.p21.RENDERING);

  {
    /*p29.SEBA*/ c.p29.SEBA = tock_pos( a.p21.CLK_xBxDxFxHd,  b.p21.CLK_xBxDxFxHd, b.p21.RENDERING, b.p29.SEBA, b.p29.VONU);
    /*p29.TYNO*/ c.p29.TYNO = nand(b.p29.TOXE, b.p29.SEBA, b.p29.VONU);
    /*p29.WUTY*/ c.p29.WUTY = not(b.p29.VUSA);
    /*p29.VUSA*/   c.p29.VUSA = or(!b.p29.TYFO, b.p29.TYNO);
  }
  /*p29.XEFY*/ c.p29.XEFY = not(b.p29.WUTY);
  /*p29.TYSO*/ c.p29.TYSO = or(b.p29.SAKY, b.p29.TEPA);
  /*p29.SAKY*/   c.p29.SAKY = nor(b.p29.TULY, b.p29.VONU);
  /*p29.TEXY*/ c.p29.TEXY = not(b.p29.TYSO);

  /*p29.TUVO*/ c.p29.TUVO = or(b.p29.TEPA, b.p29.TULY, b.p29.TESE);
  /*p29.TAME*/ c.p29.TAME = nand(b.p29.TESE, b.p29.TOXE);
  /*p29.TYTU*/ c.p29.TYTU = not(b.p29.TAME);
  /*p29.TACU*/ c.p29.TACU = nand(b.p29.TYTU, b.p29.TYFO);



  /*p29.TYFO*/ c.p29.TYFO = tock_pos( a.p21.CLK_xBxDxFxHd,  b.p21.CLK_xBxDxFxHd, b.p27.P10_Bn, b.p29.TYFO, b.p29.TAME);
  /*p29.TESE*/ c.p29.TESE = tock_pos(!a.p29.TULY, !b.p29.TULY, b.p27.SECA, b.p29.TESE, !b.p29.TESE);

  /*p29.TOXE*/ c.p29.TOXE = tock_pos( a.p29.TOMA,  b.p29.TOMA, b.p27.SECA, b.p29.TOXE, !b.p29.TOXE);
  /*p29.TOMA*/   c.p29.TOMA = nand(b.p21.CLK_xBxDxFxHd, b.p29.TAME);
  /*p29.TULY*/ c.p29.TULY = tock_pos(!a.p29.TOXE, !b.p29.TOXE, b.p27.SECA, b.p29.TULY, !b.p29.TULY);
  /*p29.TOBU*/ c.p29.TOBU = tock_pos(a.sys.CLK_AxCxExGx10, b.sys.CLK_AxCxExGx10, b.p21.RENDERING, b.p29.TOBU, b.p29.TULY);
  /*p29.VONU*/ c.p29.VONU = tock_pos(a.sys.CLK_AxCxExGx10, b.sys.CLK_AxCxExGx10, b.p21.RENDERING, b.p29.VONU, !b.p29.TOBU);
  /*p29.SYCU*/ c.p29.SYCU = nor(b.p29.TYTU, b.p29.TEPA, b.p29.TYFO);

  /*p29.TOPU*/ c.p29.SPRITE_PIX_LATCH_Ao = and(b.p29.TULY, b.p29.SYCU);
  /*p29.RACA*/ c.p29.SPRITE_PIX_LATCH_Bo = and(b.p29.VONU, b.p29.SYCU);

  /*p29.VYWA*/ c.p29.SPRITE_PIX_LATCH_Ab = not(b.p29.SPRITE_PIX_LATCH_Ao);
  /*p29.PEBY*/ c.p29.SPRITE_PIX_LATCH_Bb = not(b.p29.SPRITE_PIX_LATCH_Bo);

  /*p29.WENY*/ c.p29.SPRITE_PIX_LATCH_An = not(b.p29.SPRITE_PIX_LATCH_Ab);
  /*p29.NYBE*/ c.p29.SPRITE_PIX_LATCH_Bn = not(b.p29.SPRITE_PIX_LATCH_Bb);

  /*p29.XADO*/ c.p29.SPRITE_PIX_LATCH_A = not(b.p29.SPRITE_PIX_LATCH_An);
  /*p29.PUCO*/ c.p29.SPRITE_PIX_LATCH_B = not(b.p29.SPRITE_PIX_LATCH_Bn);

  //----------
  // Sprite priority

  /*p29.AROR*/ c.p29.AROR = and(b.p29.AZEM, b.p23.LCDC_SPEN);
  /*p29.AZEM*/   c.p29.AZEM = and(b.p29.BYJO, b.p21.RENDERING);
  /*p29.BYJO*/     c.p29.BYJO = not(b.p29.CEHA);


  /*p29.YLOZ*/ c.p29.YLOZ = nand(b.p29.AROR, b.p31.ZURE, b.p31.YWOS);
  /*p29.XAGE*/ c.p29.XAGE = nand(b.p29.AROR, b.p31.YNAZ, b.p31.YKOK);
  /*p29.DEGO*/ c.p29.DEGO = nand(b.p29.AROR, b.p31.EKES, b.p31.CEHU);
  /*p29.DYDU*/ c.p29.DYDU = nand(b.p29.AROR, b.p31.EWAM, b.p31.CYVY);
  /*p29.YGEM*/ c.p29.YGEM = nand(b.p29.AROR, b.p31.YLEV, b.p31.YTUB);
  /*p29.EFYL*/ c.p29.EFYL = nand(b.p29.AROR, b.p31.FEHA, b.p31.DAMA);
  /*p29.DYKA*/ c.p29.DYKA = nand(b.p29.AROR, b.p31.CYCO, b.p31.DAJE);
  /*p29.YBEZ*/ c.p29.YBEZ = nand(b.p29.AROR, b.p31.YDOT, b.p31.YWAP);
  /*p29.EGOM*/ c.p29.EGOM = nand(b.p29.AROR, b.p31.FYMA, b.p31.COGY);

  /*p29.YDUG*/ c.p29.SPRITE0_MATCHn    = nand(b.p29.AROR, b.p31.SP_MATCH_0B, b.p31.SP_MATCH_0A);
  /*p29.WEFU*/ c.p29.SPRITE0_MATCH_Xa  = not(b.p29.SPRITE0_MATCHn);
  /*p29.GEZE*/ c.p29.SPRITE0_MATCH_Xb  = or (b.p29.SPRITE0_MATCH_Xa, b.chip.P10_B);

  /*p29.FURO*/ c.p29.SPRITE0_SEL       = not(b.p29.SPRITE0_SELn);
  /*p29.GUVA*/   c.p29.SPRITE0_SELn      = nor(b.p29.SPRITE0_MATCHn,  b.chip.P10_B);

  /*p29.DYNA*/ c.p29.MATCH0_RSTn       = not(b.p29.MATCH0_RST);
  /*p29.DYWE*/   c.p29.MATCH0_RST        = or(b.p29.DYBA, b.p29.SPRITE0_SEL_SYNCn);
  /*p29.EBOJ*/   c.p29.SPRITE0_SEL_SYNCn = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.SPRITE0_SEL_SYNCn, b.p29.SPRITE0_SELn);

  /*p29.GAJA*/ c.p29.GAJA = not(b.p29.DYDU);
  /*p29.FUMA*/ c.p29.FUMA = or(b.p29.GAJA, b.p29.SPRITE0_MATCH_Xb);
  /*p29.ENUT*/ c.p29.ENUT = nor(b.p29.DYDU, b.p29.SPRITE0_MATCH_Xb);
  /*p29.GUPO*/ c.p29.GUPO = not(b.p29.DEGO);
  /*p29.GEDE*/ c.p29.GEDE = or(b.p29.GUPO, b.p29.FUMA);
  /*p29.EMOL*/ c.p29.EMOL = nor(b.p29.DEGO, b.p29.FUMA);
  /*p29.WEBO*/ c.p29.WEBO = not(b.p29.YLOZ);
  /*p29.WUTO*/ c.p29.WUTO = or(b.p29.WEBO, b.p29.GEDE);
  /*p29.GYFY*/ c.p29.GYFY = nor(b.p29.YLOZ, b.p29.GEDE);
  /*p29.WUNA*/ c.p29.WUNA = not(b.p29.XAGE);
  /*p29.XYLA*/ c.p29.XYLA = or(b.p29.WUNA, b.p29.WUTO);
  /*p29.GONO*/ c.p29.GONO = nor(b.p29.XAGE, b.p29.WUTO);
  /*p29.GABA*/ c.p29.GABA = not(b.p29.EGOM);
  /*p29.WEJA*/ c.p29.WEJA = or(b.p29.GABA, b.p29.XYLA);
  /*p29.GEGA*/ c.p29.GEGA = nor(b.p29.EGOM, b.p29.XYLA);
  /*p29.WASE*/ c.p29.WASE = not(b.p29.YBEZ);
  /*p29.WYLA*/ c.p29.WYLA = or(b.p29.WASE, b.p29.WEJA);
  /*p29.XOJA*/ c.p29.XOJA = nor(b.p29.YBEZ, b.p29.WEJA);
  /*p29.GYTE*/ c.p29.GYTE = not(b.p29.DYKA);
  /*p29.FAVO*/ c.p29.FAVO = or(b.p29.GYTE, b.p29.WYLA);
  /*p29.GUTU*/ c.p29.GUTU = nor(b.p29.DYKA, b.p29.WYLA);
  /*p29.GEKE*/ c.p29.GEKE = not(b.p29.EFYL);
  /*p29.GYGA*/ c.p29.GYGA = or(b.p29.GEKE, b.p29.FAVO);
  /*p29.GUZE*/ c.p29.GUZE = nor(b.p29.YGEM, b.p29.GYGA);
  /*p29.FADO*/ c.p29.FADO = not(b.p29.GUZE);

  /*p29.FOXA*/ c.p29.FOXA = nor(b.p29.EFYL, b.p29.FAVO);
  /*p29.DENY*/ c.p29.DENY = not(b.p29.FOXA);

  /*p29.GUGY*/ c.p29.GUGY = not(b.p29.GUTU);
  /*p29.XYME*/ c.p29.XYME = not(b.p29.XOJA);
  /*p29.GYGY*/ c.p29.GYGY = not(b.p29.GEGA);
  /*p29.GOWO*/ c.p29.GOWO = not(b.p29.GONO);
  /*p29.GYMA*/ c.p29.GYMA = not(b.p29.GYFY);
  /*p29.FAME*/ c.p29.FAME = not(b.p29.EMOL);
  /*p29.DYDO*/ c.p29.DYDO = not(b.p29.ENUT);


  /*p29.FEFY*/ c.p29.FEFY = nand(b.p29.XAGE, b.p29.YLOZ, b.p29.DEGO, b.p29.DYDU, b.p29.SPRITE0_MATCHn);
  /*p29.FOVE*/ c.p29.FOVE = nand(b.p29.YGEM, b.p29.EFYL, b.p29.DYKA, b.p29.YBEZ, b.p29.EGOM);
  /*p29.FEPO*/ c.p29.OAM_SCAN = or(b.p29.FEFY, b.p29.FOVE);

  /*p28.ABAK*/ c.p28.ABAK = or(b.p28.NEW_LINE, b.sys.VID_RESET7);
  /*p28.BYVA*/ c.p28.BYVA = not(b.p28.ABAK);

  /*p29.FONO*/ c.p29.FONO = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.FONO, b.p29.GUZE);
  /*p29.EXUQ*/ c.p29.EXUQ = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.EXUQ, b.p29.FOXA);
  /*p29.WAPO*/ c.p29.WAPO = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WAPO, b.p29.GUTU);
  /*p29.WOMY*/ c.p29.WOMY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WOMY, b.p29.XOJA);
  /*p29.WAFY*/ c.p29.WAFY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WAFY, b.p29.GEGA);
  /*p29.XUDY*/ c.p29.XUDY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.XUDY, b.p29.GONO);
  /*p29.GOTA*/ c.p29.GOTA = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.GOTA, b.p29.GYFY);
  /*p29.EGAV*/ c.p29.EGAV = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.EGAV, b.p29.EMOL);
  /*p29.CEDY*/ c.p29.CEDY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.CEDY, b.p29.ENUT);

  /*p29.DYBA*/ c.p29.DYBA = not(b.p28.BYVA);

  /*p29.DUBU*/ c.p29.DUBU = or(b.p29.DYBA, b.p29.FONO);
  /*p29.GORO*/ c.p29.GORO = or(b.p29.DYBA, b.p29.EXUQ);
  /*p29.GUKY*/ c.p29.GUKY = or(b.p29.DYBA, b.p29.WAPO);
  /*p29.WACY*/ c.p29.WACY = or(b.p29.DYBA, b.p29.WOMY);
  /*p29.FEVE*/ c.p29.FEVE = or(b.p29.DYBA, b.p29.WAFY);
  /*p29.WOHU*/ c.p29.WOHU = or(b.p29.DYBA, b.p29.XUDY);
  /*p29.GAKE*/ c.p29.GAKE = or(b.p29.DYBA, b.p29.GOTA);
  /*p29.FOKO*/ c.p29.FOKO = or(b.p29.DYBA, b.p29.EGAV);
  /*p29.EFEV*/ c.p29.EFEV = or(b.p29.DYBA, b.p29.CEDY);

  /*p29.DOSY*/ c.p29.DOSY = not(b.p29.DUBU);
  /*p29.WUZO*/ c.p29.WUZO = not(b.p29.GORO);
  /*p29.GAFY*/ c.p29.GAFY = not(b.p29.GUKY);
  /*p29.XAHO*/ c.p29.XAHO = not(b.p29.WACY);
  /*p29.EJAD*/ c.p29.EJAD = not(b.p29.FEVE);
  /*p29.WUNU*/ c.p29.WUNU = not(b.p29.WOHU);
  /*p29.WUPA*/ c.p29.WUPA = not(b.p29.GAKE);
  /*p29.GAMY*/ c.p29.GAMY = not(b.p29.FOKO);
  /*p29.DOKU*/ c.p29.DOKU = not(b.p29.EFEV);

  //----------
  // Sprite tile fetch address generation

  /*p29.ABON*/ c.p29.ABON = not(b.p29.TEXY);

  /*p29.WUKY*/ c.p29.FLIP_Y = not(b.p31.OAM_A_D6);

  /*p29.XUQU*/ c.p29.XUQU = not(!b.p29.VONU);
  /*p29.CYVU*/ c.p29.CYVU = xor(b.p29.FLIP_Y, b.p30.TS_LINE_1);
  /*p29.BORE*/ c.p29.BORE = xor(b.p29.FLIP_Y, b.p30.TS_LINE_2);
  /*p29.BUVY*/ c.p29.BUVY = xor(b.p29.FLIP_Y, b.p30.TS_LINE_3);

  /*p29.ABEM*/ if (b.p29.ABON) c.MA00 = b.p29.XUQU;
  /*p29.BAXE*/ if (b.p29.ABON) c.MA01 = b.p29.CYVU;
  /*p29.ARAS*/ if (b.p29.ABON) c.MA02 = b.p29.BORE;
  /*p29.AGAG*/ if (b.p29.ABON) c.MA03 = b.p29.BUVY;

  /*p29.FUFO*/ c.p29.LCDC_SPSIZEn = not(b.p23.LCDC_SPSIZE);
  /*p29.WAGO*/ c.p29.WAGO = xor(b.p29.FLIP_Y, b.p30.TS_LINE_0);
  /*p29.GEJY*/ c.p29.GEJY = amux2(b.p29.LCDC_SPSIZEn, !b.p29.OAM_B_D0, // polarity?
                                  b.p23.LCDC_SPSIZE,  b.p29.WAGO);

  /*p29.FAMU*/ if (b.p29.ABON) c.MA04 = b.p29.GEJY;
  /*p29.FUGY*/ if (b.p29.ABON) c.MA05 = b.p29.OAM_B_D1;
  /*p29.GAVO*/ if (b.p29.ABON) c.MA06 = b.p29.OAM_B_D2;
  /*p29.WYGA*/ if (b.p29.ABON) c.MA07 = b.p29.OAM_B_D3;
  /*p29.WUNE*/ if (b.p29.ABON) c.MA08 = b.p29.OAM_B_D4;
  /*p29.GOTU*/ if (b.p29.ABON) c.MA09 = b.p29.OAM_B_D5;
  /*p29.GEGU*/ if (b.p29.ABON) c.MA10 = b.p29.OAM_B_D6;
  /*p29.XEHE*/ if (b.p29.ABON) c.MA11 = b.p29.OAM_B_D7;
  /*p29.DYSO*/ if (b.p29.ABON) c.MA12 = b.chip.P10_B;   // sprites always in low half of tile store

  //----------
  // Decoder, right side

  /*p29.BAKY*/ c.p29.SPRITES_FULL = and(b.p29.SPRITE_COUNT1, b.p29.SPRITE_COUNT3);
  /*p29.CAKE*/ c.p29.SPRITE_COUNT_CLK = or(b.p29.SPRITES_FULL, b.p29.DEZY);
  /*p28.AZYB*/ c.p28.SPRITE_COUNT_RSTn = not(b.p28.NEW_LINE);

  // this must count how many sprites we've put in the matcher
  /*p29.BESE*/ c.p29.SPRITE_COUNT0 = tock_pos( a.p29.SPRITE_COUNT_CLK,  b.p29.SPRITE_COUNT_CLK, b.p28.SPRITE_COUNT_RSTn, b.p29.SPRITE_COUNT0, !b.p29.SPRITE_COUNT0);
  /*p29.CUXY*/ c.p29.SPRITE_COUNT1 = tock_pos(!a.p29.SPRITE_COUNT0,    !b.p29.SPRITE_COUNT0,    b.p28.SPRITE_COUNT_RSTn, b.p29.SPRITE_COUNT1, !b.p29.SPRITE_COUNT1);
  /*p29.BEGO*/ c.p29.SPRITE_COUNT2 = tock_pos(!a.p29.SPRITE_COUNT1,    !b.p29.SPRITE_COUNT1,    b.p28.SPRITE_COUNT_RSTn, b.p29.SPRITE_COUNT2, !b.p29.SPRITE_COUNT2);
  /*p29.DYBE*/ c.p29.SPRITE_COUNT3 = tock_pos(!a.p29.SPRITE_COUNT2,    !b.p29.SPRITE_COUNT2,    b.p28.SPRITE_COUNT_RSTn, b.p29.SPRITE_COUNT3, !b.p29.SPRITE_COUNT3);

  // 4 to 10 decoder
  /*p29.EDEN*/ c.p29.SPRITE_COUNT0n = not(b.p29.SPRITE_COUNT0);
  /*p29.FYCU*/ c.p29.SPRITE_COUNT0b = not(b.p29.SPRITE_COUNT0n);
  /*p29.CYPY*/ c.p29.SPRITE_COUNT1n = not(b.p29.SPRITE_COUNT1);
  /*p29.FONE*/ c.p29.SPRITE_COUNT1b = not(b.p29.SPRITE_COUNT1n);
  /*p29.CAPE*/ c.p29.SPRITE_COUNT2n = not(b.p29.SPRITE_COUNT2);
  /*p29.EKUD*/ c.p29.SPRITE_COUNT2b = not(b.p29.SPRITE_COUNT2n);
  /*p29.CAXU*/ c.p29.SPRITE_COUNT3n = not(b.p29.SPRITE_COUNT3);
  /*p29.ELYG*/ c.p29.SPRITE_COUNT3b = not(b.p29.SPRITE_COUNT3b);

  /*p29.DEZO*/ c.p29.DEZO = nand(b.p29.SPRITE_COUNT0n, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3n); // 0000
  /*p29.CUVA*/ c.p29.CUVA = nand(b.p29.SPRITE_COUNT0b, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3n); // 0001
  /*p29.GEBU*/ c.p29.GEBU = nand(b.p29.SPRITE_COUNT0n, b.p29.SPRITE_COUNT1b, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3n); // 0010
  /*p29.FOCO*/ c.p29.FOCO = nand(b.p29.SPRITE_COUNT0b, b.p29.SPRITE_COUNT1b, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3n); // 0011
  /*p29.CUPE*/ c.p29.CUPE = nand(b.p29.SPRITE_COUNT0n, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2b, b.p29.SPRITE_COUNT3n); // 0100
  /*p29.CUGU*/ c.p29.CUGU = nand(b.p29.SPRITE_COUNT0b, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2b, b.p29.SPRITE_COUNT3n); // 0101
  /*p29.WOMU*/ c.p29.WOMU = nand(b.p29.SPRITE_COUNT0n, b.p29.SPRITE_COUNT1b, b.p29.SPRITE_COUNT2b, b.p29.SPRITE_COUNT3n); // 0110
  /*p29.GUNA*/ c.p29.GUNA = nand(b.p29.SPRITE_COUNT0b, b.p29.SPRITE_COUNT1b, b.p29.SPRITE_COUNT2b, b.p29.SPRITE_COUNT3n); // 0111
  /*p29.DEWY*/ c.p29.DEWY = nand(b.p29.SPRITE_COUNT0n, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3b); // 1000
  /*p29.DOGU*/ c.p29.DOGU = nand(b.p29.SPRITE_COUNT0b, b.p29.SPRITE_COUNT1n, b.p29.SPRITE_COUNT2n, b.p29.SPRITE_COUNT3b); // 1001

  /*p29.CARE*/ c.p29.CARE = or(b.p29.CLK_XOCE, b.p29.CEHA, b.p29.SPR_MATCH_Y);
  /*p29.DYTY*/ c.p29.DYTY = not(b.p29.CARE);

  /*p29.CEMY*/ c.p29.CEMY = or(b.p29.DYTY, b.p29.DEZO); // 0000
  /*p29.BYBY*/ c.p29.BYBY = or(b.p29.DYTY, b.p29.CUVA); // 0001
  /*p29.WYXO*/ c.p29.WYXO = or(b.p29.DYTY, b.p29.GEBU); // 0010
  /*p29.GUVE*/ c.p29.GUVE = or(b.p29.DYTY, b.p29.FOCO); // 0011
  /*p29.CECU*/ c.p29.CECU = or(b.p29.DYTY, b.p29.CUPE); // 0100
  /*p29.CADO*/ c.p29.CADO = or(b.p29.DYTY, b.p29.CUGU); // 0101
  /*p29.XUJO*/ c.p29.XUJO = or(b.p29.DYTY, b.p29.WOMU); // 0110
  /*p29.GAPE*/ c.p29.GAPE = or(b.p29.DYTY, b.p29.GUNA); // 0111
  /*p29.CAHO*/ c.p29.CAHO = or(b.p29.DYTY, b.p29.DEWY); // 1000
  /*p29.CATO*/ c.p29.CATO = or(b.p29.DYTY, b.p29.DOGU); // 1001

  /*p29.DYHU*/ c.p29.DYHU        = not(b.p29.CEMY); // 0000
  /*p29.FUXU*/ c.p29.MATCH0_CLK  = not(b.p29.DYHU);
  /*p29.GENY*/ c.p29.STORE0_CLKa = not(b.p29.DYHU);
  /*p29.ENOB*/ c.p29.STORE0_CLKb = not(b.p29.DYHU);

  /*p29.BUCO*/ c.p29.BUCO = not(b.p29.BYBY); // 0001
  /*p29.ASYS*/ c.p29.ASYS = not(b.p29.BUCO);
  /*p29.AHOF*/ c.p29.AHOF = not(b.p29.BUCO);
  /*p29.BYVY*/ c.p29.BYVY = not(b.p29.BUCO);

  /*p29.GYFO*/ c.p29.GYFO = not(b.p29.WYXO); // 0010
  /*p29.CACU*/ c.p29.CACU = not(b.p29.GYFO);
  /*p29.BUZY*/ c.p29.BUZY = not(b.p29.GYFO);
  /*p29.FUKE*/ c.p29.FUKE = not(b.p29.GYFO);

  /*p29.WEKA*/ c.p29.WEKA = not(b.p29.XUJO);
  /*p29.GYVO*/ c.p29.GYVO = not(b.p29.GAPE);
  /*p29.GUSA*/ c.p29.GUSA = not(b.p29.GUVE);
  /*p29.BUKA*/ c.p29.BUKA = not(b.p29.CAHO);
  /*p29.DECU*/ c.p29.DECU = not(b.p29.CATO);
  /*p29.BEDE*/ c.p29.BEDE = not(b.p29.CADO);
  /*p29.DUKE*/ c.p29.DUKE = not(b.p29.CECU);
  /*p29.ZAPE*/ c.p29.ZAPE = not(b.p29.WEKA);
  /*p29.FEFO*/ c.p29.FEFO = not(b.p29.GYVO);
  /*p29.FEKA*/ c.p29.FEKA = not(b.p29.GUSA);
  /*p29.CEXU*/ c.p29.CEXU = not(b.p29.BUKA);


  /*p29.WEME*/ c.p29.WEME = not(b.p29.DECU);
  /*p29.CYLA*/ c.p29.CYLA = not(b.p29.BEDE);
  /*p29.WOFO*/ c.p29.WOFO = not(b.p29.DUKE);

  /*p29.WUSE*/ c.p29.WUSE = not(b.p29.WEKA);
  /*p29.GECY*/ c.p29.GECY = not(b.p29.GYVO);
  /*p29.XYHA*/ c.p29.XYHA = not(b.p29.GUSA);
  /*p29.AKOL*/ c.p29.AKOL = not(b.p29.BUKA);
  /*p29.WUFA*/ c.p29.WUFA = not(b.p29.DECU);
  /*p29.DYMO*/ c.p29.DYMO = not(b.p29.BEDE);
  /*p29.WYLU*/ c.p29.WYLU = not(b.p29.DUKE);

  /*p29.ZURU*/ c.p29.ZURU = not(b.p29.WEKA);
  /*p29.WABE*/ c.p29.WABE = not(b.p29.GYVO);
  /*p29.YFAG*/ c.p29.YFAG = not(b.p29.GUSA);
  /*p29.BYMY*/ c.p29.BYMY = not(b.p29.BUKA);
  /*p29.FAKA*/ c.p29.FAKA = not(b.p29.DECU);
  /*p29.BUCY*/ c.p29.BUCY = not(b.p29.BEDE);
  /*p29.EWOT*/ c.p29.EWOT = not(b.p29.DUKE);

  /*p29.DEZY*/ c.p29.DEZY = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.VID_RESETn1, b.p29.DEZY, b.p29.DYTY);

  /*p30.CYKE*/ c.p30.CYKE = not(b.p21.CLK_2Mb);
  /*p30.WUDA*/ c.p30.WUDA = not(b.p30.CYKE);

  /*p30.XADU*/ c.p30.IDX_0 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_0, b.p28.OAM_A2);
  /*p30.XEDY*/ c.p30.IDX_1 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_1, b.p28.OAM_A3);
  /*p30.ZUZE*/ c.p30.IDX_2 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_2, b.p28.OAM_A4);
  /*p30.XOBE*/ c.p30.IDX_3 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_3, b.p28.OAM_A5);
  /*p30.YDUF*/ c.p30.IDX_4 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_4, b.p28.OAM_A6);
  /*p30.XECU*/ c.p30.IDX_5 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_5, b.p28.OAM_A7);

  // so this is a bit weird as we have a tri-state bus inside one schematic...

  /*p30.WEZA*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_0 = b.p30.IDX_5; // order wrong
  /*p30.WUCO*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_1 = b.p30.IDX_4;
  /*p30.WYDA*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_2 = b.p30.IDX_3;
  /*p30.ZYSU*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_3 = b.p30.IDX_2;
  /*p30.WYSE*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_4 = b.p30.IDX_1;
  /*p30.WUZY*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_5 = b.p30.IDX_0;

  /*p30.WENU*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_0 = b.p29.SPRITE_DELTA1; // order wrong
  /*p30.CUCU*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_1 = b.p29.SPRITE_DELTA2;
  /*p30.CUCA*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_2 = b.p29.SPRITE_DELTA0;
  /*p30.CEGA*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_3 = b.p29.SPRITE_DELTA3;

  //----------
  // store 0

  /*p30.YGUS*/ c.p30.STORE0_IDX0  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX0,  b.p30.TS_IDX_0);
  /*p30.YSOK*/ c.p30.STORE0_IDX1  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX1,  b.p30.TS_IDX_1);
  /*p30.YZEP*/ c.p30.STORE0_IDX2  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX2,  b.p30.TS_IDX_2);
  /*p30.WYTE*/ c.p30.STORE0_IDX3  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX3,  b.p30.TS_IDX_3);
  /*p30.ZONY*/ c.p30.STORE0_IDX4  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX4,  b.p30.TS_IDX_4);
  /*p30.YWAK*/ c.p30.STORE0_IDX5  = tock_pos(a.p29.STORE0_CLKa, b.p29.STORE0_CLKa, 0, b.p30.STORE0_IDX5,  b.p30.TS_IDX_5);
  /*p30.FYHY*/ c.p30.STORE0_LINE0 = tock_pos(a.p29.STORE0_CLKb, b.p29.STORE0_CLKb, 0, b.p30.STORE0_LINE0, b.p30.TS_LINE_0);
  /*p30.GYHO*/ c.p30.STORE0_LINE1 = tock_pos(a.p29.STORE0_CLKb, b.p29.STORE0_CLKb, 0, b.p30.STORE0_LINE1, b.p30.TS_LINE_1);
  /*p30.BOZU*/ c.p30.STORE0_LINE2 = tock_pos(a.p29.STORE0_CLKb, b.p29.STORE0_CLKb, 0, b.p30.STORE0_LINE2, b.p30.TS_LINE_2);
  /*p30.CUFO*/ c.p30.STORE0_LINE3 = tock_pos(a.p29.STORE0_CLKb, b.p29.STORE0_CLKb, 0, b.p30.STORE0_LINE3, b.p30.TS_LINE_3);

  /*p30.ZETU*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_0  = b.p30.STORE0_IDX0;
  /*p30.ZECE*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_1  = b.p30.STORE0_IDX1;
  /*p30.ZAVE*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_2  = b.p30.STORE0_IDX2;
  /*p30.WOKO*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_3  = b.p30.STORE0_IDX3;
  /*p30.ZUMU*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_4  = b.p30.STORE0_IDX4;
  /*p30.ZEDY*/ if (b.p29.SPRITE0_SEL) c.p30.TS_IDX_5  = b.p30.STORE0_IDX5;
  /*p30.GOFO*/ if (b.p29.SPRITE0_SEL) c.p30.TS_LINE_0 = b.p30.STORE0_LINE0;
  /*p30.WEHE*/ if (b.p29.SPRITE0_SEL) c.p30.TS_LINE_1 = b.p30.STORE0_LINE1;
  /*p30.AJAL*/ if (b.p29.SPRITE0_SEL) c.p30.TS_LINE_2 = b.p30.STORE0_LINE2;
  /*p30.BUKY*/ if (b.p29.SPRITE0_SEL) c.p30.TS_LINE_3 = b.p30.STORE0_LINE3;








  /*p30.AXUV*/ c.p30.AXUV = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.AXUV, b.p30.TS_IDX_0 );
  /*p30.BADA*/ c.p30.BADA = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BADA, b.p30.TS_IDX_1 );
  /*p30.APEV*/ c.p30.APEV = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.APEV, b.p30.TS_IDX_2 );
  /*p30.BADO*/ c.p30.BADO = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BADO, b.p30.TS_IDX_3 );
  /*p30.BEXY*/ c.p30.BEXY = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BEXY, b.p30.TS_IDX_4 );
  /*p30.BYHE*/ c.p30.BYHE = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BYHE, b.p30.TS_IDX_5 );
  /*p30.AFYM*/ c.p30.AFYM = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFYM, b.p30.TS_LINE_0);
  /*p30.AZAP*/ c.p30.AZAP = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AZAP, b.p30.TS_LINE_1);
  /*p30.AFUT*/ c.p30.AFUT = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFUT, b.p30.TS_LINE_2);
  /*p30.AFYX*/ c.p30.AFYX = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFYX, b.p30.TS_LINE_3);

  /*p30.ADYB*/ if (b.p29.DENY) c.p30.TS_IDX_0  = b.p30.AXUV;
  /*p30.APOB*/ if (b.p29.DENY) c.p30.TS_IDX_1  = b.p30.BADA;
  /*p30.APYV*/ if (b.p29.DENY) c.p30.TS_IDX_2  = b.p30.APEV;
  /*p30.AFEN*/ if (b.p29.DENY) c.p30.TS_IDX_3  = b.p30.BADO;
  /*p30.AKYH*/ if (b.p29.DENY) c.p30.TS_IDX_4  = b.p30.BEXY;
  /*p30.APOC*/ if (b.p29.DENY) c.p30.TS_IDX_5  = b.p30.BYHE;
  /*p30.BUJY*/ if (b.p29.DENY) c.p30.TS_LINE_0 = b.p30.AFYM;
  /*p30.BOSO*/ if (b.p29.DENY) c.p30.TS_LINE_1 = b.p30.AZAP;
  /*p30.AHAC*/ if (b.p29.DENY) c.p30.TS_LINE_2 = b.p30.AFUT;
  /*p30.BAZU*/ if (b.p29.DENY) c.p30.TS_LINE_3 = b.p30.AFYX;

  /*p30.WOCY*/ c.p30.WOCY = not(!b.p30.GECU); if (b.p29.XYME) c.p30.TS_IDX_0  = b.p30.WOCY; c.p30.GECU = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GECU, b.p30.TS_IDX_0 );
  /*p30.ELYC*/ c.p30.ELYC = not(!b.p30.FOXY); if (b.p29.XYME) c.p30.TS_IDX_1  = b.p30.ELYC; c.p30.FOXY = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.FOXY, b.p30.TS_IDX_1 );
  /*p30.WABO*/ c.p30.WABO = not(!b.p30.GOHU); if (b.p29.XYME) c.p30.TS_IDX_2  = b.p30.WABO; c.p30.GOHU = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GOHU, b.p30.TS_IDX_2 );
  /*p30.EZOC*/ c.p30.EZOC = not(!b.p30.FOGO); if (b.p29.XYME) c.p30.TS_IDX_3  = b.p30.EZOC; c.p30.FOGO = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.FOGO, b.p30.TS_IDX_3 );
  /*p30.WYWY*/ c.p30.WYWY = not(!b.p30.GACY); if (b.p29.XYME) c.p30.TS_IDX_4  = b.p30.WYWY; c.p30.GACY = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GACY, b.p30.TS_IDX_4 );
  /*p30.WATO*/ c.p30.WATO = not(!b.p30.GABO); if (b.p29.XYME) c.p30.TS_IDX_5  = b.p30.WATO; c.p30.GABO = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GABO, b.p30.TS_IDX_5 );
  /*p30.ZUDO*/ c.p30.ZUDO = not(!b.p30.ZUBE); if (b.p29.XYME) c.p30.TS_LINE_0 = b.p30.ZUDO; c.p30.ZUBE = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZUBE, b.p30.TS_LINE_0);
  /*p30.YBUK*/ c.p30.YBUK = not(!b.p30.ZUMY); if (b.p29.XYME) c.p30.TS_LINE_1 = b.p30.YBUK; c.p30.ZUMY = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZUMY, b.p30.TS_LINE_1);
  /*p30.ZYTO*/ c.p30.ZYTO = not(!b.p30.ZEXO); if (b.p29.XYME) c.p30.TS_LINE_2 = b.p30.ZYTO; c.p30.ZEXO = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZEXO, b.p30.TS_LINE_2);
  /*p30.YKOZ*/ c.p30.YKOZ = not(!b.p30.ZAFU); if (b.p29.XYME) c.p30.TS_LINE_3 = b.p30.YKOZ; c.p30.ZAFU = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZAFU, b.p30.TS_LINE_3);

  /*p30.WAJA*/ c.p30.WAJA = not(!b.p30.XYNU); if (b.p29.GOWO) c.p30.TS_IDX_0  = b.p30.WAJA; c.p30.XYNU = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XYNU, b.p30.TS_IDX_0 );
  /*p30.WOXY*/ c.p30.WOXY = not(!b.p30.XEGE); if (b.p29.GOWO) c.p30.TS_IDX_1  = b.p30.WOXY; c.p30.XEGE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XEGE, b.p30.TS_IDX_1 );
  /*p30.XYRE*/ c.p30.XYRE = not(!b.p30.XABO); if (b.p29.GOWO) c.p30.TS_IDX_2  = b.p30.XYRE; c.p30.XABO = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XABO, b.p30.TS_IDX_2 );
  /*p30.WERU*/ c.p30.WERU = not(!b.p30.WANU); if (b.p29.GOWO) c.p30.TS_IDX_3  = b.p30.WERU; c.p30.WANU = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.WANU, b.p30.TS_IDX_3 );
  /*p30.WEPY*/ c.p30.WEPY = not(!b.p30.XEFE); if (b.p29.GOWO) c.p30.TS_IDX_4  = b.p30.WEPY; c.p30.XEFE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XEFE, b.p30.TS_IDX_4 );
  /*p30.WUXU*/ c.p30.WUXU = not(!b.p30.XAVE); if (b.p29.GOWO) c.p30.TS_IDX_5  = b.p30.WUXU; c.p30.XAVE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XAVE, b.p30.TS_IDX_5 );
  /*p30.BYDO*/ c.p30.BYDO = not(!b.p30.CUMU); if (b.p29.GOWO) c.p30.TS_LINE_0 = b.p30.BYDO; c.p30.CUMU = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CUMU, b.p30.TS_LINE_0);
  /*p30.BUCE*/ c.p30.BUCE = not(!b.p30.CAPO); if (b.p29.GOWO) c.p30.TS_LINE_1 = b.p30.BUCE; c.p30.CAPO = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CAPO, b.p30.TS_LINE_1);
  /*p30.BOVE*/ c.p30.BOVE = not(!b.p30.CONO); if (b.p29.GOWO) c.p30.TS_LINE_2 = b.p30.BOVE; c.p30.CONO = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CONO, b.p30.TS_LINE_2);
  /*p30.BEVY*/ c.p30.BEVY = not(!b.p30.CAJU); if (b.p29.GOWO) c.p30.TS_LINE_3 = b.p30.BEVY; c.p30.CAJU = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CAJU, b.p30.TS_LINE_3);

  /*p30.EVYT*/ c.p30.EVYT = not(!b.p30.FUZO); if (b.p29.GUGY) c.p30.TS_IDX_0  = b.p30.EVYT; c.p30.FUZO = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FUZO, b.p30.TS_IDX_0 );
  /*p30.WABA*/ c.p30.WABA = not(!b.p30.GESY); if (b.p29.GUGY) c.p30.TS_IDX_1  = b.p30.WABA; c.p30.GESY = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GESY, b.p30.TS_IDX_1 );
  /*p30.ETAD*/ c.p30.ETAD = not(!b.p30.FYSU); if (b.p29.GUGY) c.p30.TS_IDX_2  = b.p30.ETAD; c.p30.FYSU = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FYSU, b.p30.TS_IDX_2 );
  /*p30.ELEP*/ c.p30.ELEP = not(!b.p30.FEFA); if (b.p29.GUGY) c.p30.TS_IDX_3  = b.p30.ELEP; c.p30.FEFA = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FEFA, b.p30.TS_IDX_3 );
  /*p30.WYGO*/ c.p30.WYGO = not(!b.p30.GYNO); if (b.p29.GUGY) c.p30.TS_IDX_4  = b.p30.WYGO; c.p30.GYNO = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GYNO, b.p30.TS_IDX_4 );
  /*p30.WAKO*/ c.p30.WAKO = not(!b.p30.GULE); if (b.p29.GUGY) c.p30.TS_IDX_5  = b.p30.WAKO; c.p30.GULE = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GULE, b.p30.TS_IDX_5 );
  /*p30.WANA*/ c.p30.WANA = not(!b.p30.XYGO); if (b.p29.GUGY) c.p30.TS_LINE_0 = b.p30.WANA; c.p30.XYGO = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XYGO, b.p30.TS_LINE_0);
  /*p30.WAXE*/ c.p30.WAXE = not(!b.p30.XYNA); if (b.p29.GUGY) c.p30.TS_LINE_1 = b.p30.WAXE; c.p30.XYNA = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XYNA, b.p30.TS_LINE_1);
  /*p30.WABU*/ c.p30.WABU = not(!b.p30.XAKU); if (b.p29.GUGY) c.p30.TS_LINE_2 = b.p30.WABU; c.p30.XAKU = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XAKU, b.p30.TS_LINE_2);
  /*p30.YPOZ*/ c.p30.YPOZ = not(!b.p30.YGUM); if (b.p29.GUGY) c.p30.TS_LINE_3 = b.p30.YPOZ; c.p30.YGUM = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.YGUM, b.p30.TS_LINE_3);

  /*p30.BEMO*/ c.p30.BEMO = not(!b.p30.CAJY); if (b.p29.DYDO) c.p30.TS_IDX_0  = b.p30.BEMO; c.p30.CAJY = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CAJY, b.p30.TS_IDX_0 );
  /*p30.CYBY*/ c.p30.CYBY = not(!b.p30.CUZA); if (b.p29.DYDO) c.p30.TS_IDX_1  = b.p30.CYBY; c.p30.CUZA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CUZA, b.p30.TS_IDX_1 );
  /*p30.BETY*/ c.p30.BETY = not(!b.p30.COMA); if (b.p29.DYDO) c.p30.TS_IDX_2  = b.p30.BETY; c.p30.COMA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.COMA, b.p30.TS_IDX_2 );
  /*p30.CEGY*/ c.p30.CEGY = not(!b.p30.CUFA); if (b.p29.DYDO) c.p30.TS_IDX_3  = b.p30.CEGY; c.p30.CUFA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CUFA, b.p30.TS_IDX_3 );
  /*p30.CELU*/ c.p30.CELU = not(!b.p30.CEBO); if (b.p29.DYDO) c.p30.TS_IDX_4  = b.p30.CELU; c.p30.CEBO = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CEBO, b.p30.TS_IDX_4 );
  /*p30.CUBO*/ c.p30.CUBO = not(!b.p30.CADU); if (b.p29.DYDO) c.p30.TS_IDX_5  = b.p30.CUBO; c.p30.CADU = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CADU, b.p30.TS_IDX_5 );
  /*p30.BEFE*/ c.p30.BEFE = not(!b.p30.ABUG); if (b.p29.DYDO) c.p30.TS_LINE_0 = b.p30.BEFE; c.p30.ABUG = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.ABUG, b.p30.TS_LINE_0);
  /*p30.BYRO*/ c.p30.BYRO = not(!b.p30.AMES); if (b.p29.DYDO) c.p30.TS_LINE_1 = b.p30.BYRO; c.p30.AMES = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.AMES, b.p30.TS_LINE_1);
  /*p30.BACO*/ c.p30.BACO = not(!b.p30.ABOP); if (b.p29.DYDO) c.p30.TS_LINE_2 = b.p30.BACO; c.p30.ABOP = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.ABOP, b.p30.TS_LINE_2);
  /*p30.AHUM*/ c.p30.AHUM = not(!b.p30.AROF); if (b.p29.DYDO) c.p30.TS_LINE_3 = b.p30.AHUM; c.p30.AROF = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.AROF, b.p30.TS_LINE_3);

  /*p30.DALO*/ c.p30.DALO = not(!b.p30.EKAP); if (b.p29.GYGY) c.p30.TS_IDX_0  = b.p30.DALO; c.p30.EKAP = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EKAP, b.p30.TS_IDX_0 );
  /*p30.DALY*/ c.p30.DALY = not(!b.p30.ETAV); if (b.p29.GYGY) c.p30.TS_IDX_1  = b.p30.DALY; c.p30.ETAV = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.ETAV, b.p30.TS_IDX_1 );
  /*p30.DUZA*/ c.p30.DUZA = not(!b.p30.EBEX); if (b.p29.GYGY) c.p30.TS_IDX_2  = b.p30.DUZA; c.p30.EBEX = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EBEX, b.p30.TS_IDX_2 );
  /*p30.WAGA*/ c.p30.WAGA = not(!b.p30.GORU); if (b.p29.GYGY) c.p30.TS_IDX_3  = b.p30.WAGA; c.p30.GORU = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.GORU, b.p30.TS_IDX_3 );
  /*p30.DYNY*/ c.p30.DYNY = not(!b.p30.ETYM); if (b.p29.GYGY) c.p30.TS_IDX_4  = b.p30.DYNY; c.p30.ETYM = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.ETYM, b.p30.TS_IDX_4 );
  /*p30.DOBO*/ c.p30.DOBO = not(!b.p30.EKOP); if (b.p29.GYGY) c.p30.TS_IDX_5  = b.p30.DOBO; c.p30.EKOP = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EKOP, b.p30.TS_IDX_5 );
  /*p30.AWAT*/ c.p30.AWAT = not(!b.p30.ANED); if (b.p29.GYGY) c.p30.TS_LINE_0 = b.p30.AWAT; c.p30.ANED = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ANED, b.p30.TS_LINE_0);
  /*p30.BACE*/ c.p30.BACE = not(!b.p30.ACEP); if (b.p29.GYGY) c.p30.TS_LINE_1 = b.p30.BACE; c.p30.ACEP = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ACEP, b.p30.TS_LINE_1);
  /*p30.BODU*/ c.p30.BODU = not(!b.p30.ABUX); if (b.p29.GYGY) c.p30.TS_LINE_2 = b.p30.BODU; c.p30.ABUX = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ABUX, b.p30.TS_LINE_2);
  /*p30.BUJA*/ c.p30.BUJA = not(!b.p30.ABEG); if (b.p29.GYGY) c.p30.TS_LINE_3 = b.p30.BUJA; c.p30.ABEG = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ABEG, b.p30.TS_LINE_3);

  /*p30.DEZU*/ c.p30.DEZU = not(!b.p30.DAFU); if (b.p29.GYMA) c.p30.TS_IDX_0  = b.p30.DEZU; c.p30.DAFU = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DAFU, b.p30.TS_IDX_0 );
  /*p30.EFUD*/ c.p30.EFUD = not(!b.p30.DEBA); if (b.p29.GYMA) c.p30.TS_IDX_1  = b.p30.EFUD; c.p30.DEBA = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DEBA, b.p30.TS_IDX_1 );
  /*p30.DONY*/ c.p30.DONY = not(!b.p30.DUHA); if (b.p29.GYMA) c.p30.TS_IDX_2  = b.p30.DONY; c.p30.DUHA = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DUHA, b.p30.TS_IDX_2 );
  /*p30.DOWA*/ c.p30.DOWA = not(!b.p30.DUNY); if (b.p29.GYMA) c.p30.TS_IDX_3  = b.p30.DOWA; c.p30.DUNY = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DUNY, b.p30.TS_IDX_3 );
  /*p30.DYGO*/ c.p30.DYGO = not(!b.p30.DESE); if (b.p29.GYMA) c.p30.TS_IDX_4  = b.p30.DYGO; c.p30.DESE = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DESE, b.p30.TS_IDX_4 );
  /*p30.ENAP*/ c.p30.ENAP = not(!b.p30.DEVY); if (b.p29.GYMA) c.p30.TS_IDX_5  = b.p30.ENAP; c.p30.DEVY = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DEVY, b.p30.TS_IDX_5 );
  /*p30.ZYPO*/ c.p30.ZYPO = not(!b.p30.ZURY); if (b.p29.GYMA) c.p30.TS_LINE_0 = b.p30.ZYPO; c.p30.ZURY = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZURY, b.p30.TS_LINE_0);
  /*p30.ZEXE*/ c.p30.ZEXE = not(!b.p30.ZURO); if (b.p29.GYMA) c.p30.TS_LINE_1 = b.p30.ZEXE; c.p30.ZURO = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZURO, b.p30.TS_LINE_1);
  /*p30.YJEM*/ c.p30.YJEM = not(!b.p30.ZENE); if (b.p29.GYMA) c.p30.TS_LINE_2 = b.p30.YJEM; c.p30.ZENE = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZENE, b.p30.TS_LINE_2);
  /*p30.YWAV*/ c.p30.YWAV = not(!b.p30.ZYLU); if (b.p29.GYMA) c.p30.TS_LINE_3 = b.p30.YWAV; c.p30.ZYLU = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZYLU, b.p30.TS_LINE_3);

  /*p30.AXEC*/ c.p30.AXEC = not(!b.p30.BOXA); if (b.p29.FAME) c.p30.TS_IDX_0  = b.p30.AXEC; c.p30.BOXA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BOXA, b.p30.TS_IDX_0 );
  /*p30.CYRO*/ c.p30.CYRO = not(!b.p30.BUNA); if (b.p29.FAME) c.p30.TS_IDX_1  = b.p30.CYRO; c.p30.BUNA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BUNA, b.p30.TS_IDX_1 );
  /*p30.CUVU*/ c.p30.CUVU = not(!b.p30.BULU); if (b.p29.FAME) c.p30.TS_IDX_2  = b.p30.CUVU; c.p30.BULU = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BULU, b.p30.TS_IDX_2 );
  /*p30.APON*/ c.p30.APON = not(!b.p30.BECA); if (b.p29.FAME) c.p30.TS_IDX_3  = b.p30.APON; c.p30.BECA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BECA, b.p30.TS_IDX_3 );
  /*p30.AFOZ*/ c.p30.AFOZ = not(!b.p30.BYHU); if (b.p29.FAME) c.p30.TS_IDX_4  = b.p30.AFOZ; c.p30.BYHU = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BYHU, b.p30.TS_IDX_4 );
  /*p30.CUBE*/ c.p30.CUBE = not(!b.p30.BUHE); if (b.p29.FAME) c.p30.TS_IDX_5  = b.p30.CUBE; c.p30.BUHE = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BUHE, b.p30.TS_IDX_5 );
  /*p30.ZABY*/ c.p30.ZABY = not(!b.p30.YKUK); if (b.p29.FAME) c.p30.TS_LINE_0 = b.p30.ZABY; c.p30.YKUK = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.YKUK, b.p30.TS_LINE_0);
  /*p30.ZUKE*/ c.p30.ZUKE = not(!b.p30.YLOV); if (b.p29.FAME) c.p30.TS_LINE_1 = b.p30.ZUKE; c.p30.YLOV = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.YLOV, b.p30.TS_LINE_1);
  /*p30.WUXE*/ c.p30.WUXE = not(!b.p30.XAZY); if (b.p29.FAME) c.p30.TS_LINE_2 = b.p30.WUXE; c.p30.XAZY = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.XAZY, b.p30.TS_LINE_2);
  /*p30.WERE*/ c.p30.WERE = not(!b.p30.XOSY); if (b.p29.FAME) c.p30.TS_LINE_3 = b.p30.WERE; c.p30.XOSY = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.XOSY, b.p30.TS_LINE_3);

  /*p30.YHAL*/ c.p30.YHAL = not(!b.p30.XUFO); if (b.p29.FADO) c.p30.TS_IDX_0  = b.p30.YHAL; c.p30.XUFO = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XUFO, b.p30.TS_IDX_0 );
  /*p30.YRAD*/ c.p30.YRAD = not(!b.p30.XUTE); if (b.p29.FADO) c.p30.TS_IDX_1  = b.p30.YRAD; c.p30.XUTE = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XUTE, b.p30.TS_IDX_1 );
  /*p30.XYRA*/ c.p30.XYRA = not(!b.p30.XOTU); if (b.p29.FADO) c.p30.TS_IDX_2  = b.p30.XYRA; c.p30.XOTU = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XOTU, b.p30.TS_IDX_2 );
  /*p30.YNEV*/ c.p30.YNEV = not(!b.p30.XYFE); if (b.p29.FADO) c.p30.TS_IDX_3  = b.p30.YNEV; c.p30.XYFE = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XYFE, b.p30.TS_IDX_3 );
  /*p30.ZOJY*/ c.p30.ZOJY = not(!b.p30.YZOR); if (b.p29.FADO) c.p30.TS_IDX_4  = b.p30.ZOJY; c.p30.YZOR = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.YZOR, b.p30.TS_IDX_4 );
  /*p30.ZARO*/ c.p30.ZARO = not(!b.p30.YBER); if (b.p29.FADO) c.p30.TS_IDX_5  = b.p30.ZARO; c.p30.YBER = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.YBER, b.p30.TS_IDX_5 );
  /*p30.CAWO*/ c.p30.CAWO = not(!b.p30.DEWU); if (b.p29.FADO) c.p30.TS_LINE_0 = b.p30.CAWO; c.p30.DEWU = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.DEWU, b.p30.TS_LINE_0);
  /*p30.BYME*/ c.p30.BYME = not(!b.p30.CANA); if (b.p29.FADO) c.p30.TS_LINE_1 = b.p30.BYME; c.p30.CANA = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.CANA, b.p30.TS_LINE_1);
  /*p30.COHO*/ c.p30.COHO = not(!b.p30.DYSY); if (b.p29.FADO) c.p30.TS_LINE_2 = b.p30.COHO; c.p30.DYSY = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.DYSY, b.p30.TS_LINE_2);
  /*p30.GATE*/ c.p30.GATE = not(!b.p30.FOFO); if (b.p29.FADO) c.p30.TS_LINE_3 = b.p30.GATE; c.p30.FOFO = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.FOFO, b.p30.TS_LINE_3);

  /*p31.ZAGO*/ c.p31.OAM_A_D0b = not(!b.p31.OAM_A_D0);
  /*p31.ZOCY*/ c.p31.OAM_A_D1b = not(!b.p31.OAM_A_D1);
  /*p31.YPUR*/ c.p31.OAM_A_D2b = not(!b.p31.OAM_A_D2);
  /*p31.YVOK*/ c.p31.OAM_A_D3b = not(!b.p31.OAM_A_D3);
  /*p31.COSE*/ c.p31.OAM_A_D4b = not(!b.p31.OAM_A_D4);
  /*p31.AROP*/ c.p31.OAM_A_D5b = not(!b.p31.OAM_A_D5);
  /*p31.XATU*/ c.p31.OAM_A_D6b = not(!b.p31.OAM_A_D6);
  /*p31.BADY*/ c.p31.OAM_A_D7b = not(!b.p31.OAM_A_D7);

  // CHECK CLK/RESET WIRES  

  //----------
  // matcher 0

  /*p31.XEPE*/ c.p31.M0X0 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X0, b.p31.OAM_A_D0b);
  /*p31.YLAH*/ c.p31.M0X1 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X1, b.p31.OAM_A_D1b);
  /*p31.ZOLA*/ c.p31.M0X2 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X2, b.p31.OAM_A_D2b);
  /*p31.ZULU*/ c.p31.M0X3 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X3, b.p31.OAM_A_D3b);
  /*p31.WELO*/ c.p31.M0X4 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X4, b.p31.OAM_A_D4b);
  /*p31.XUNY*/ c.p31.M0X5 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X5, b.p31.OAM_A_D5b);
  /*p31.WOTE*/ c.p31.M0X6 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X6, b.p31.OAM_A_D6b);
  /*p31.XAKO*/ c.p31.M0X7 = tock_pos(a.p29.MATCH0_CLK, b.p29.MATCH0_CLK, b.p29.MATCH0_RSTn, b.p31.M0X7, b.p31.OAM_A_D7b);

  /*p31.ZOGY*/ c.p31.MATCHER0_MATCH0 = xor(b.p31.M0X0, b.p21.X0n);
  /*p31.ZEBA*/ c.p31.MATCHER0_MATCH1 = xor(b.p31.M0X1, b.p21.X1n);
  /*p31.ZAHA*/ c.p31.MATCHER0_MATCH2 = xor(b.p31.M0X2, b.p21.X2n);
  /*p31.ZOKY*/ c.p31.MATCHER0_MATCH3 = xor(b.p31.M0X3, b.p21.X3n);
  /*p31.WOJU*/ c.p31.MATCHER0_MATCH4 = xor(b.p31.M0X4, b.p21.X4n);
  /*p31.YFUN*/ c.p31.MATCHER0_MATCH5 = xor(b.p31.M0X5, b.p21.X5n);
  /*p31.WYZA*/ c.p31.MATCHER0_MATCH6 = xor(b.p31.M0X6, b.p21.X6n);
  /*p31.YPUK*/ c.p31.MATCHER0_MATCH7 = xor(b.p31.M0X7, b.p21.X7n);

  /*p31.XEBA*/ c.p31.SP_MATCH_0A = nor(b.p31.MATCHER0_MATCH4, b.p31.MATCHER0_MATCH5, b.p31.MATCHER0_MATCH6, b.p31.MATCHER0_MATCH7);
  /*p31.ZAKO*/ c.p31.SP_MATCH_0B = nor(b.p31.MATCHER0_MATCH0, b.p31.MATCHER0_MATCH1, b.p31.MATCHER0_MATCH2, b.p31.MATCHER0_MATCH3);

  //----------
  // matcher

  /*p31.XOLY*/ c.p31.XOLY = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XOLY, b.p31.OAM_A_D0b);
  /*p31.XYBA*/ c.p31.XYBA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XYBA, b.p31.OAM_A_D1b);
  /*p31.XABE*/ c.p31.XABE = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XABE, b.p31.OAM_A_D2b);
  /*p31.XEKA*/ c.p31.XEKA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XEKA, b.p31.OAM_A_D3b);
  /*p31.XOMY*/ c.p31.XOMY = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XOMY, b.p31.OAM_A_D4b);
  /*p31.WUHA*/ c.p31.WUHA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WUHA, b.p31.OAM_A_D5b);
  /*p31.WYNA*/ c.p31.WYNA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WYNA, b.p31.OAM_A_D6b);
  /*p31.WECO*/ c.p31.WECO = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WECO, b.p31.OAM_A_D7b);

  /*p31.YVAP*/ c.p31.YVAP = xor(b.p31.XOMY, b.p21.X4n);
  /*p31.XENY*/ c.p31.XENY = xor(b.p31.WUHA, b.p21.X5n);
  /*p31.XAVU*/ c.p31.XAVU = xor(b.p31.WYNA, b.p21.X6n);
  /*p31.XEVA*/ c.p31.XEVA = xor(b.p31.WECO, b.p21.X7n);
  /*p31.YHOK*/ c.p31.YHOK = xor(b.p31.XOLY, b.p21.X0n);
  /*p31.YCAH*/ c.p31.YCAH = xor(b.p31.XYBA, b.p21.X1n);
  /*p31.YDAJ*/ c.p31.YDAJ = xor(b.p31.XABE, b.p21.X2n);
  /*p31.YVUZ*/ c.p31.YVUZ = xor(b.p31.XEKA, b.p21.X3n);
  /*p31.YWOS*/ c.p31.YWOS = nor(b.p31.YVAP, b.p31.XENY, b.p31.XAVU, b.p31.XEVA);
  /*p31.ZURE*/ c.p31.ZURE = nor(b.p31.YHOK, b.p31.YCAH, b.p31.YDAJ, b.p31.YVUZ);

  //----------
  // matcher

  /*p31.ERAZ*/ c.p31.ERAZ = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.ERAZ, b.p31.OAM_A_D0b);
  /*p31.EPUM*/ c.p31.EPUM = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EPUM, b.p31.OAM_A_D1b);
  /*p31.EROL*/ c.p31.EROL = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EROL, b.p31.OAM_A_D2b);
  /*p31.EHYN*/ c.p31.EHYN = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EHYN, b.p31.OAM_A_D3b);
  /*p31.FAZU*/ c.p31.FAZU = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FAZU, b.p31.OAM_A_D4b);
  /*p31.FAXE*/ c.p31.FAXE = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FAXE, b.p31.OAM_A_D5b);
  /*p31.EXUK*/ c.p31.EXUK = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EXUK, b.p31.OAM_A_D6b);
  /*p31.FEDE*/ c.p31.FEDE = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FEDE, b.p31.OAM_A_D7b);

  /*p31.EJOT*/ c.p31.EJOT = xor(b.p31.FAZU, b.p21.X4n);
  /*p31.ESAJ*/ c.p31.ESAJ = xor(b.p31.FAXE, b.p21.X5n);
  /*p31.DUCU*/ c.p31.DUCU = xor(b.p31.EXUK, b.p21.X6n);
  /*p31.EWUD*/ c.p31.EWUD = xor(b.p31.FEDE, b.p21.X7n);
  /*p31.DUSE*/ c.p31.DUSE = xor(b.p31.ERAZ, b.p21.X0n);
  /*p31.DAGU*/ c.p31.DAGU = xor(b.p31.EPUM, b.p21.X1n);
  /*p31.DYZE*/ c.p31.DYZE = xor(b.p31.EROL, b.p21.X2n);
  /*p31.DESO*/ c.p31.DESO = xor(b.p31.EHYN, b.p21.X3n);
  /*p31.DAJE*/ c.p31.DAJE = nor(b.p31.EJOT, b.p31.ESAJ, b.p31.DUCU, b.p31.EWUD);
  /*p31.CYCO*/ c.p31.CYCO = nor(b.p31.DUSE, b.p31.DAGU, b.p31.DYZE, b.p31.DESO);

  //----------
  // matcher

  /*p31.DANY*/ c.p31.DANY = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DANY, b.p31.OAM_A_D0b);
  /*p31.DUKO*/ c.p31.DUKO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DUKO, b.p31.OAM_A_D1b);
  /*p31.DESU*/ c.p31.DESU = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DESU, b.p31.OAM_A_D2b);
  /*p31.DAZO*/ c.p31.DAZO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DAZO, b.p31.OAM_A_D3b);
  /*p31.DAKE*/ c.p31.DAKE = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DAKE, b.p31.OAM_A_D4b);
  /*p31.CESO*/ c.p31.CESO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.CESO, b.p31.OAM_A_D5b);
  /*p31.DYFU*/ c.p31.DYFU = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DYFU, b.p31.OAM_A_D6b);
  /*p31.CUSY*/ c.p31.CUSY = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.CUSY, b.p31.OAM_A_D7b);

  /*p31.COLA*/ c.p31.COLA = xor(b.p31.DAKE, b.p21.X4n);
  /*p31.BOBA*/ c.p31.BOBA = xor(b.p31.CESO, b.p21.X5n);
  /*p31.COLU*/ c.p31.COLU = xor(b.p31.DYFU, b.p21.X6n);
  /*p31.BAHU*/ c.p31.BAHU = xor(b.p31.CUSY, b.p21.X7n);
  /*p31.EDYM*/ c.p31.EDYM = xor(b.p31.DANY, b.p21.X0n);
  /*p31.EMYB*/ c.p31.EMYB = xor(b.p31.DUKO, b.p21.X1n);
  /*p31.EBEF*/ c.p31.EBEF = xor(b.p31.DESU, b.p21.X2n);
  /*p31.EWOK*/ c.p31.EWOK = xor(b.p31.DAZO, b.p21.X3n);
  /*p31.CYVY*/ c.p31.CYVY = nor(b.p31.COLA, b.p31.BOBA, b.p31.COLU, b.p31.BAHU);
  /*p31.EWAM*/ c.p31.EWAM = nor(b.p31.EDYM, b.p31.EMYB, b.p31.EBEF, b.p31.EWOK);

  //----------
  // matcher

  /*p31.YCOL*/ c.p31.YCOL = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YCOL, b.p31.OAM_A_D0b);
  /*p31.YRAC*/ c.p31.YRAC = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YRAC, b.p31.OAM_A_D1b);
  /*p31.YMEM*/ c.p31.YMEM = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YMEM, b.p31.OAM_A_D2b);
  /*p31.YVAG*/ c.p31.YVAG = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YVAG, b.p31.OAM_A_D3b);
  /*p31.ZOLY*/ c.p31.ZOLY = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZOLY, b.p31.OAM_A_D4b);
  /*p31.ZOGO*/ c.p31.ZOGO = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZOGO, b.p31.OAM_A_D5b);
  /*p31.ZECU*/ c.p31.ZECU = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZECU, b.p31.OAM_A_D6b);
  /*p31.ZESA*/ c.p31.ZESA = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZESA, b.p31.OAM_A_D7b);

  /*p31.ZARE*/ c.p31.ZARE = xor(b.p31.ZOLY, b.p21.X4n);
  /*p31.ZEMU*/ c.p31.ZEMU = xor(b.p31.ZOGO, b.p21.X5n);
  /*p31.ZYGO*/ c.p31.ZYGO = xor(b.p31.ZECU, b.p21.X6n);
  /*p31.ZUZY*/ c.p31.ZUZY = xor(b.p31.ZESA, b.p21.X7n);
  /*p31.XOSU*/ c.p31.XOSU = xor(b.p31.YCOL, b.p21.X0n);
  /*p31.ZUVU*/ c.p31.ZUVU = xor(b.p31.YRAC, b.p21.X1n);
  /*p31.XUCO*/ c.p31.XUCO = xor(b.p31.YMEM, b.p21.X2n);
  /*p31.ZULO*/ c.p31.ZULO = xor(b.p31.YVAG, b.p21.X3n);
  /*p31.YWAP*/ c.p31.YWAP = nor(b.p31.ZARE, b.p31.ZEMU, b.p31.ZYGO, b.p31.ZUZY);
  /*p31.YDOT*/ c.p31.YDOT = nor(b.p31.XOSU, b.p31.ZUVU, b.p31.XUCO, b.p31.ZULO);

  //----------
  // matcher

  /*p31.WEDU*/ c.p31.WEDU = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.WEDU, b.p31.OAM_A_D0b);
  /*p31.YGAJ*/ c.p31.YGAJ = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.YGAJ, b.p31.OAM_A_D1b);
  /*p31.ZYJO*/ c.p31.ZYJO = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.ZYJO, b.p31.OAM_A_D2b);
  /*p31.XURY*/ c.p31.XURY = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.XURY, b.p31.OAM_A_D3b);
  /*p31.YBED*/ c.p31.YBED = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.YBED, b.p31.OAM_A_D4b);
  /*p31.ZALA*/ c.p31.ZALA = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.ZALA, b.p31.OAM_A_D5b);
  /*p31.WYDE*/ c.p31.WYDE = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.WYDE, b.p31.OAM_A_D6b);
  /*p31.XEPA*/ c.p31.XEPA = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.XEPA, b.p31.OAM_A_D7b);

  /*p31.ZYKU*/ c.p31.ZYKU = xor(b.p31.YBED, b.p21.X4n);
  /*p31.ZYPU*/ c.p31.ZYPU = xor(b.p31.ZALA, b.p21.X5n);
  /*p31.XAHA*/ c.p31.XAHA = xor(b.p31.WYDE, b.p21.X6n);
  /*p31.ZEFE*/ c.p31.ZEFE = xor(b.p31.XEPA, b.p21.X7n);
  /*p31.XEJU*/ c.p31.XEJU = xor(b.p31.WEDU, b.p21.X0n);
  /*p31.ZATE*/ c.p31.ZATE = xor(b.p31.YGAJ, b.p21.X1n);
  /*p31.ZAKU*/ c.p31.ZAKU = xor(b.p31.ZYJO, b.p21.X2n);
  /*p31.YBOX*/ c.p31.YBOX = xor(b.p31.XURY, b.p21.X3n);
  /*p31.YKOK*/ c.p31.YKOK = nor(b.p31.ZYKU, b.p31.ZYPU, b.p31.XAHA, b.p31.ZEFE);
  /*p31.YNAZ*/ c.p31.YNAZ = nor(b.p31.XEJU, b.p31.ZATE, b.p31.ZAKU, b.p31.YBOX);

  //----------
  // matcher

  /*p31.GAVY*/ c.p31.GAVY = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GAVY, b.p31.OAM_A_D0b);
  /*p31.GYPU*/ c.p31.GYPU = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GYPU, b.p31.OAM_A_D1b);
  /*p31.GADY*/ c.p31.GADY = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GADY, b.p31.OAM_A_D2b);
  /*p31.GAZA*/ c.p31.GAZA = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GAZA, b.p31.OAM_A_D3b);
  /*p31.EZUF*/ c.p31.EZUF = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.EZUF, b.p31.OAM_A_D4b);
  /*p31.ENAD*/ c.p31.ENAD = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.ENAD, b.p31.OAM_A_D5b);
  /*p31.EBOW*/ c.p31.EBOW = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.EBOW, b.p31.OAM_A_D6b);
  /*p31.FYCA*/ c.p31.FYCA = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.FYCA, b.p31.OAM_A_D7b);

  /*p31.DUZE*/ c.p31.DUZE = xor(b.p31.EZUF, b.p21.X4n);
  /*p31.DAGA*/ c.p31.DAGA = xor(b.p31.ENAD, b.p21.X5n);
  /*p31.DAWU*/ c.p31.DAWU = xor(b.p31.EBOW, b.p21.X6n);
  /*p31.EJAW*/ c.p31.EJAW = xor(b.p31.FYCA, b.p21.X7n);
  /*p31.GOHO*/ c.p31.GOHO = xor(b.p31.GAVY, b.p21.X0n);
  /*p31.GASU*/ c.p31.GASU = xor(b.p31.GYPU, b.p21.X1n);
  /*p31.GABU*/ c.p31.GABU = xor(b.p31.GADY, b.p21.X2n);
  /*p31.GAFE*/ c.p31.GAFE = xor(b.p31.GAZA, b.p21.X3n);
  /*p31.DAMA*/ c.p31.DAMA = nor(b.p31.DUZE, b.p31.DAGA, b.p31.DAWU, b.p31.EJAW);
  /*p31.FEHA*/ c.p31.FEHA = nor(b.p31.GOHO, b.p31.GASU, b.p31.GABU, b.p31.GAFE);

  //----------
  // matcher

  /*p31.XUVY*/ c.p31.XUVY = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XUVY, b.p31.OAM_A_D0b);
  /*p31.XERE*/ c.p31.XERE = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XERE, b.p31.OAM_A_D1b);
  /*p31.XUZO*/ c.p31.XUZO = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XUZO, b.p31.OAM_A_D2b);
  /*p31.XEXA*/ c.p31.XEXA = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XEXA, b.p31.OAM_A_D3b);
  /*p31.YPOD*/ c.p31.YPOD = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YPOD, b.p31.OAM_A_D4b);
  /*p31.YROP*/ c.p31.YROP = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YROP, b.p31.OAM_A_D5b);
  /*p31.YNEP*/ c.p31.YNEP = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YNEP, b.p31.OAM_A_D6b);
  /*p31.YZOF*/ c.p31.YZOF = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YZOF, b.p31.OAM_A_D7b);

  /*p31.ZYWU*/ c.p31.ZYWU = xor(b.p31.YPOD, b.p21.X4n);
  /*p31.ZUZA*/ c.p31.ZUZA = xor(b.p31.YROP, b.p21.X5n);
  /*p31.ZEJO*/ c.p31.ZEJO = xor(b.p31.YNEP, b.p21.X6n);
  /*p31.ZEDA*/ c.p31.ZEDA = xor(b.p31.YZOF, b.p21.X7n);
  /*p31.YMAM*/ c.p31.YMAM = xor(b.p31.XUVY, b.p21.X0n);
  /*p31.YTYP*/ c.p31.YTYP = xor(b.p31.XERE, b.p21.X1n);
  /*p31.YFOP*/ c.p31.YFOP = xor(b.p31.XUZO, b.p21.X2n);
  /*p31.YVAC*/ c.p31.YVAC = xor(b.p31.XEXA, b.p21.X3n);
  /*p31.YTUB*/ c.p31.YTUB = nor(b.p31.ZYWU, b.p31.ZUZA, b.p31.ZEJO, b.p31.ZEDA);
  /*p31.YLEV*/ c.p31.YLEV = nor(b.p31.YMAM, b.p31.YTYP, b.p31.YFOP, b.p31.YVAC);

  //----------
  // matcher

  /*p31.FUSA*/ c.p31.FUSA = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FUSA, b.p31.OAM_A_D0b);
  /*p31.FAXA*/ c.p31.FAXA = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FAXA, b.p31.OAM_A_D1b);
  /*p31.FOZY*/ c.p31.FOZY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FOZY, b.p31.OAM_A_D2b);
  /*p31.FESY*/ c.p31.FESY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FESY, b.p31.OAM_A_D3b);
  /*p31.CYWE*/ c.p31.CYWE = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.CYWE, b.p31.OAM_A_D4b);
  /*p31.DYBY*/ c.p31.DYBY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.DYBY, b.p31.OAM_A_D5b);
  /*p31.DURY*/ c.p31.DURY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.DURY, b.p31.OAM_A_D6b);
  /*p31.CUVY*/ c.p31.CUVY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.CUVY, b.p31.OAM_A_D7b);

  /*p31.BAZY*/ c.p31.BAZY = xor(b.p31.CYWE, b.p21.X4n);
  /*p31.CYLE*/ c.p31.CYLE = xor(b.p31.DYBY, b.p21.X5n);
  /*p31.CEVA*/ c.p31.CEVA = xor(b.p31.DURY, b.p21.X6n);
  /*p31.BUMY*/ c.p31.BUMY = xor(b.p31.CUVY, b.p21.X7n);
  /*p31.GUZO*/ c.p31.GUZO = xor(b.p31.FUSA, b.p21.X0n);
  /*p31.GOLA*/ c.p31.GOLA = xor(b.p31.FAXA, b.p21.X1n);
  /*p31.GEVE*/ c.p31.GEVE = xor(b.p31.FOZY, b.p21.X2n);
  /*p31.GUDE*/ c.p31.GUDE = xor(b.p31.FESY, b.p21.X3n);
  /*p31.COGY*/ c.p31.COGY = nor(b.p31.BAZY, b.p31.CYLE, b.p31.CEVA, b.p31.BUMY);
  /*p31.FYMA*/ c.p31.FYMA = nor(b.p31.GUZO, b.p31.GOLA, b.p31.GEVE, b.p31.GUDE);

  //----------
  // matcher

  /*p31.FOKA*/ c.p31.FOKA = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FOKA, b.p31.OAM_A_D0b);
  /*p31.FYTY*/ c.p31.FYTY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FYTY, b.p31.OAM_A_D1b);
  /*p31.FUBY*/ c.p31.FUBY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FUBY, b.p31.OAM_A_D2b);
  /*p31.GOXU*/ c.p31.GOXU = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.GOXU, b.p31.OAM_A_D3b);
  /*p31.DUHY*/ c.p31.DUHY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.DUHY, b.p31.OAM_A_D4b);
  /*p31.EJUF*/ c.p31.EJUF = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.EJUF, b.p31.OAM_A_D5b);
  /*p31.ENOR*/ c.p31.ENOR = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.ENOR, b.p31.OAM_A_D6b);
  /*p31.DEPY*/ c.p31.DEPY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.DEPY, b.p31.OAM_A_D7b);

  /*p31.CEKO*/ c.p31.CEKO = xor(b.p31.DUHY, b.p21.X4n);
  /*p31.DETY*/ c.p31.DETY = xor(b.p31.EJUF, b.p21.X5n);
  /*p31.DOZO*/ c.p31.DOZO = xor(b.p31.ENOR, b.p21.X6n);
  /*p31.CONY*/ c.p31.CONY = xor(b.p31.DEPY, b.p21.X7n);
  /*p31.FUZU*/ c.p31.FUZU = xor(b.p31.FOKA, b.p21.X0n);
  /*p31.FESO*/ c.p31.FESO = xor(b.p31.FYTY, b.p21.X1n);
  /*p31.FOKY*/ c.p31.FOKY = xor(b.p31.FUBY, b.p21.X2n);
  /*p31.FYVA*/ c.p31.FYVA = xor(b.p31.GOXU, b.p21.X3n);
  /*p31.CEHU*/ c.p31.CEHU = nor(b.p31.CEKO, b.p31.DETY, b.p31.DOZO, b.p31.CONY);
  /*p31.EKES*/ c.p31.EKES = nor(b.p31.FUZU, b.p31.FESO, b.p31.FOKY, b.p31.FYVA);
}