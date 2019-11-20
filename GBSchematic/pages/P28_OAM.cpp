#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P28_OAM::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p28.WEFE*/ c.p28.P10_Bn = not(b.chip.P10_B);


  //----------

  /*p28.AWOH*/ c.p28.CLK_2Mn = not(b.p21.CLK_2Mb);
  /*p28.ABAF*/ c.p28.IN_FRAMEn = not(b.p29.IN_FRAME);
  /*p28.ANEL*/ c.p28.ANEL = tock_pos(a.p28.CLK_2Mn, b.p28.CLK_2Mn, b.sys.VID_RESETn3, b.p28.ANEL, b.p29.IN_FRAME);
  /*p28.BYHA*/ c.p28.BYHA = or(b.p28.ANEL, b.p28.IN_FRAMEn, b.sys.VID_RESETn3); // wat?
  /*p28.ATEJ*/ c.p28.SPRITE_COUNT_RST = not(b.p28.BYHA);

  //----------
  // sprite scan counter

  /*p28.FETO*/ c.p28.SCAN_DONE = and(b.p28.SCAN0, b.p28.SCAN1, b.p28.SCAN2, b.p28.SCAN5);
  /*p28.GAVA*/ c.p28.SCAN_CLK  = or(b.p28.SCAN_DONE, b.p21.CLK_2Mb);
  /*p28.ANOM*/ c.p28.SCAN_RSTn = nor(b.sys.VID_RESET6, b.p28.SPRITE_COUNT_RST);

  /*p28.YFEL*/ c.p28.SCAN0 = tock_pos( a.p28.SCAN_CLK, b.p28.SCAN_CLK, b.p28.SCAN_RSTn, b.p28.SCAN0, !b.p28.SCAN0);
  /*p28.WEWY*/ c.p28.SCAN1 = tock_pos(!a.p28.SCAN0,    !b.p28.SCAN0,   b.p28.SCAN_RSTn, b.p28.SCAN1, !b.p28.SCAN1);
  /*p28.GOSO*/ c.p28.SCAN2 = tock_pos(!a.p28.SCAN1,    !b.p28.SCAN1,   b.p28.SCAN_RSTn, b.p28.SCAN2, !b.p28.SCAN2);
  /*p28.ELYN*/ c.p28.SCAN3 = tock_pos(!a.p28.SCAN2,    !b.p28.SCAN2,   b.p28.SCAN_RSTn, b.p28.SCAN3, !b.p28.SCAN3);
  /*p28.FAHA*/ c.p28.SCAN4 = tock_pos(!a.p28.SCAN3,    !b.p28.SCAN3,   b.p28.SCAN_RSTn, b.p28.SCAN4, !b.p28.SCAN4);
  /*p28.FONY*/ c.p28.SCAN5 = tock_pos(!a.p28.SCAN4,    !b.p28.SCAN4,   b.p28.SCAN_RSTn, b.p28.SCAN5, !b.p28.SCAN5);

  //----------

  /*p28.ASEN*/ c.p28.ASEN = or(b.sys.VID_RESET6, b.p29.AVAP);
  /*p28.BESU*/ c.p28.BESU = or(b.p29.IN_FRAME, b.p28.ASEN);
  /*p28.XYNY*/ c.p28.XYNY = not(b.sys.CPUCLK_ABCDxxxx9);
  
  /*p28.XUTO*/ c.p28.OAM_WR = and(b.sys.ADDR_OAM, b.sys.CPU_WR2);

  /*p28.WUJE*/ c.p28.WUJE = or(b.p28.XYNY, b.p28.OAM_WR);
  /*p28.WEFY*/ c.p28.WEFY = and(b.p29.TUVO, b.p29.TYFO);
  /*p28.BOFE*/ c.p28.FROM_CPU5n = not(b.sys.FROM_CPU5);
  /*p28.AJEP*/ c.p28.AJEP = and(b.p28.OAM_ADDR_PARSEn, b.p29.CLK_XOCE);
  /*p28.XUJA*/ c.p28.XUJA = not(b.p28.WEFY);

  /*p28.BOTA*/ c.p28.BOTA = nor(b.p28.FROM_CPU5n, b.sys.ADDR_OAM, b.sys.CPU_RD2);


  //----------

  /*p28.WARU*/ c.p28.WARU = and(b.p22.FF40, b.sys.CPU_WR2);


  /*p28.XUPA*/ c.p28.XUPA      = not(b.p28.WUJE);

  /*p28.AJUJ*/ c.p28.AJUJ             = nor(b.sys.DMA_RUNNING_SYNCn, b.p28.OAM_ADDR_PARSEn, b.p28.OAM_ADDR_RENDERn);
  /*p28.AMAB*/ c.p28.AMAB      = and(b.sys.ADDR_OAM, b.p28.AJUJ);

  /*p28.ADAH*/ c.p28.ADDR_OAMn        = not(b.sys.ADDR_OAM);








  //----------

  /*p28.APAG*/ c.p28.D_TO_OAMDn = amux2(b.p28.XUPA, b.p28.AMAB,
                                        b.p28.AJUJ, b.p28.ADDR_OAMn);

  /*p28.AZUL*/ c.p28.D_TO_OAMD = not(b.p28.D_TO_OAMDn);
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


  /*p25.VAPE*/ c.p25.VAPE = and(b.p29.TACU, b.p29.TUVO);
  /*p25.AVER*/ c.p25.AVER = and(b.p28.OAM_ADDR_PARSEn, b.p29.XYSO);
  /*p25.XUJY*/ c.p25.XUJY = not(b.p25.VAPE);

  /*p25.BYCU*/ c.p25.OAM_IN_CLK   = nor(b.p25.OAM_WR, b.p25.XUJY, b.p25.AVER);
  /*p31.XEGA*/ c.p31.OAM_A_IN_CLK = not(b.p25.OAM_IN_CLKn);
  /*p29.YWOK*/ c.p29.OAM_B_IN_CLK = not(b.p25.OAM_IN_CLKn);

  /*p28.ASYT*/ c.p28.OAM_LATCHn = and(b.p28.AJEP, b.p28.XUJA, b.p28.BOTA);
  /*p28.BODE*/ c.p28.OAM_LATCH  = not(b.p28.OAM_LATCHn);

  /*p28.YVAL*/ c.p28.OAM_LATCHo = not(b.p28.OAM_LATCH);
  /*p28.YRYV*/ c.p28.OAM_LATCHb = not(b.p28.OAM_LATCHo);

  /*p28.ZODO*/ c.p28.OAM_CLK     = not(b.p28.OAM_LATCHb); // -> OAM
  /*p25.COTA*/ c.p25.OAM_IN_CLKn = not(b.p25.OAM_IN_CLK); // -> OAM
  /*p28.ZONE*/ c.p28.OAM_A_CS    = not(b.p28.OAM_A_CSn); // -> OAM
  /*p28.ZOFE*/ c.p28.OAM_B_CS    = not(b.p28.OAM_B_CSn); // -> OAM

  // these are unused
  /*
  c.p28.XUCA = not(b.p28.WARU);
  c.p28.XECY = tock_pos(a.p28.XUCA, b.p28.XUCA, 0,          b.p28.XECY, b.sys.SYS_RESETn4); // ? weird
  c.p28.XUVA = tock_pos(a.p28.XYNY, b.p28.XYNY, b.sys.XARE, b.p28.XUVA, b.p28.XECY);
  */
}
