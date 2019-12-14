#include "Sch_OAM.h"

#include "Sch_Gameboy.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void OAM_tick(const Bus& bus_in,
              const Clocks& clocks,
              const Decoder& dec,
              const Sprites& spr,
              const DMA& dma,
              const Video& vid,
              const OAM& a,
              const OAM& b,
              OAM& next) {
  (void)a;
  (void)b;
  (void)next;

  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus_in.A15, bus_in.A14, bus_in.A13, bus_in.A12, bus_in.A11, bus_in.A10, bus_in.A09);
  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p07.SOHA*/ wire ADDR_FFXXn2 = not(dec.ADDR_FFXX);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ wire ADDR_OAM = not(ADDR_FEXXn);

  {
#if 0
    // def some polarity bugs here
    /*p25.CUFE*/ wire DMA_WR_OAM = or(and(ADDR_OAM, dma.DMA_RUNNING_SYNC), clocks.MOPA_xxxxEFGH);
    /*p25.AVER*/ wire AVER = and(spr.OAM_ADDR_PARSEn, or(!clocks.WUVU_AxxDExxH, !clocks.WOSU_QQQQQQQQ));
    /*p25.XUJY*/ wire XUJY = nand(or(RENDERINGn, spr.SPR_SEQ1, spr.SPR_DEL1), nand(SPR_SEQ2, SPR_SEQ0, SPR_SEQ_5_SYNCn));
    /*p25.BYCU*/ wire OAM_IN_CLK = nor(DMA_WR_OAM, XUJY, AVER);
  #endif

    /*p29.WOJO*/ wire WOJO = nor(!clocks.WUVU_AxxDExxH, !clocks.WOSU_QQQQQQQQ);
    /*p29.XYSO*/ wire XYSO = not(WOJO);
    /*p25.AVER*/ wire AVER = and(spr.OAM_ADDR_PARSEn, XYSO);
    /*p25.CUFE*/ wire OAM_WR = or(and(ADDR_OAM, dma.DMA_RUNNING_SYNC), clocks.MOPA_xxxxEFGH);
    /*p29.TAME*/ wire SEQ_5n = nand(spr.SPR_SEQ2, spr.SPR_SEQ0);
    /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);
    /*p29.TUVO*/ wire TUVO = or(vid.RENDERINGn, spr.SPR_SEQ1, spr.SPR_DEL1);
    /*p29.TACU*/ wire SPR_SEQ_5_TRIG = nand(SEQ_5, spr.SPR_SEQ_5_SYNCn);
    /*p25.VAPE*/ wire VAPE = and(TUVO, SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire XUJY = not(VAPE);

    /*p25.BYCU*/ wire OAM_IN_CLK = nor(OAM_WR, XUJY, AVER);
    /*p25.COTA*/ next.OAM_IN_CLKn = not(OAM_IN_CLK); // -> OAM

  }

#if 0
  {
    /*p28.AJEP*/ wire AJEP = and(b.spr.OAM_ADDR_PARSEn, b.spr.CLK_XOCE);
    /*p28.BOTA*/ wire BOTA = nor(FROM_CPU5n, ADDR_OAM, b.ctl.CPU_RD2);
    /*p29.TUVO*/     wire TUVO = or(b.RENDERINGn, b.spr.SPR_SEQ1, b.spr.SPR_DEL1);
    /*p28.WEFY*/   wire WEFY = and(TUVO, b.SPR_SEQ_5_SYNCn);
    /*p28.XUJA*/ wire XUJA = not(WEFY);
    /*p28.ASYT*/ wire OAM_LATCHn = and(AJEP, next.spr.XUJA, BOTA);
    /*p28.BODE*/ next.spr.OAM_LATCH = not(OAM_LATCHn);
  }

  {
    /*p28.YVAL*/ wire OAM_LATCHo = not(b.spr.OAM_LATCH);
    /*p28.YRYV*/ wire OAM_LATCHb = not(OAM_LATCHo);
    /*p28.ZODO*/ next.oam.OAM_CLK = not(OAM_LATCHb); // -> OAM
  }
    

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.rst.SYS_RESETn);
    /*p01.CUNU*/ wire SYS_RESETn2 = not(DULA_RESET); // video, sprites, dma
    /*p04.MAKA*/ next.spr.FROM_CPU5_SYNC = tock_pos(a.sys.ZEME_xBxDxFxH, b.sys.ZEME_xBxDxFxH, SYS_RESETn2, b.spr.FROM_CPU5_SYNC, FROM_CPU5);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.dma.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.dma.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(ADDR_OAM, OAM_BUSYn);

    /*p28.WAFO*/ wire OAM_A0n   = not(b.spr.OAM_A0);
    /*p04.NAXY*/ wire NAXY      = nor(b.spr.FROM_CPU5_SYNC, b.dma.DMA_STARTa);
    /*p04.POWU*/ wire POWU      = and(b.dma.DMA_RUNNING_SYNC, NAXY);
    /*p04.WYJA*/ wire OAM_CS    = unk3(OAM_LOCKn, b.ctl.CPU_WR2, POWU);
    /*p28.YNYC*/ wire OAM_B_CSn = and(OAM_CS, OAM_A0n);
    /*p28.YLYC*/ wire OAM_A_CSn = and(OAM_CS, b.spr.OAM_A0);
    /*p28.ZOFE*/ next.spr.OAM_B_CS = not(OAM_B_CSn); // -> OAM
    /*p28.ZONE*/ next.spr.OAM_A_CS = not(OAM_A_CSn); // -> OAM
  }

  {
    // OAM address mux

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.dma.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.dma.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(ADDR_OAM, OAM_BUSYn);

    /*p28.ADAH*/ wire ADDR_OAMn = not(ADDR_OAM);
    /*p28.XUTO*/ wire CPU_OAM_WR = and(ADDR_OAM, b.ctl.CPU_WR2);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(b.sys.MOPA_xxxxEFGH);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);

    /*p28.ZAXA*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D0 = b.D0;
    /*p28.ZAKY*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D1 = b.D1;
    /*p28.WULE*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D2 = b.D2;
    /*p28.ZOZO*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D3 = b.D3;
    /*p28.ZUFO*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D4 = b.D4;
    /*p28.ZATO*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D5 = b.D5;
    /*p28.YVUC*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D6 = b.D6;
    /*p28.ZUFE*/ if (D_TO_OAMD) next.oam.TS_OAM_A_D7 = b.D7;

    /*p28.ZAMY*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D0 = b.D0;
    /*p28.ZOPU*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D1 = b.D1;
    /*p28.WYKY*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D2 = b.D2;
    /*p28.ZAJA*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D3 = b.D3;
    /*p28.ZUGA*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D4 = b.D4;
    /*p28.ZUMO*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D5 = b.D5;
    /*p28.XYTO*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D6 = b.D6;
    /*p28.ZYFA*/ if (D_TO_OAMD) next.oam.TS_OAM_B_D7 = b.D7;
  }
  


  //----------
  // OAM data in latch/reg


  {
    /*p31.XYKY*/ next.spr.OAM_A_LATCH0 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH0, b.oam.TS_OAM_A_D0);
    /*p31.YRUM*/ next.spr.OAM_A_LATCH1 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH1, b.oam.TS_OAM_A_D1);
    /*p31.YSEX*/ next.spr.OAM_A_LATCH2 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH2, b.oam.TS_OAM_A_D2);
    /*p31.YVEL*/ next.spr.OAM_A_LATCH3 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH3, b.oam.TS_OAM_A_D3);
    /*p31.WYNO*/ next.spr.OAM_A_LATCH4 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH4, b.oam.TS_OAM_A_D4);
    /*p31.CYRA*/ next.spr.OAM_A_LATCH5 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH5, b.oam.TS_OAM_A_D5);
    /*p31.ZUVE*/ next.spr.OAM_A_LATCH6 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH6, b.oam.TS_OAM_A_D6);
    /*p31.ECED*/ next.spr.OAM_A_LATCH7 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH7, b.oam.TS_OAM_A_D7);

    /*p31.XEGA*/ next.spr.OAM_A_IN_CLK = not(b.vid.OAM_IN_CLKn);
    /*p31.YLOR*/ next.spr.OAM_A_D0 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D0, b.spr.OAM_A_LATCH0);
    /*p31.ZYTY*/ next.spr.OAM_A_D1 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D1, b.spr.OAM_A_LATCH1);
    /*p31.ZYVE*/ next.spr.OAM_A_D2 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D2, b.spr.OAM_A_LATCH2);
    /*p31.ZEZY*/ next.spr.OAM_A_D3 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D3, b.spr.OAM_A_LATCH3);
    /*p31.GOMO*/ next.spr.OAM_A_D4 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D4, b.spr.OAM_A_LATCH4);
    /*p31.BAXO*/ next.spr.OAM_A_D5 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D5, b.spr.OAM_A_LATCH5);
    /*p31.YZOS*/ next.spr.OAM_A_D6 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D6, b.spr.OAM_A_LATCH6);
    /*p31.DEPO*/ next.spr.OAM_A_D7 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D7, b.spr.OAM_A_LATCH7);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.dma.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.dma.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(ADDR_OAM, OAM_BUSYn);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.ctl.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p28.WUKU*/ wire OAM_A_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, b.spr.OAM_A0);
    /*p28.WEWU*/ wire OAM_A_CPU_RD  = not(OAM_A_CPU_RDn);
    /*p31.XACA*/ if (OAM_A_CPU_RD) next.D0 = b.spr.OAM_A_LATCH0;
    /*p31.XAGU*/ if (OAM_A_CPU_RD) next.D1 = b.spr.OAM_A_LATCH1;
    /*p31.XEPU*/ if (OAM_A_CPU_RD) next.D2 = b.spr.OAM_A_LATCH2;
    /*p31.XYGU*/ if (OAM_A_CPU_RD) next.D3 = b.spr.OAM_A_LATCH3;
    /*p31.XUNA*/ if (OAM_A_CPU_RD) next.D4 = b.spr.OAM_A_LATCH4;
    /*p31.DEVE*/ if (OAM_A_CPU_RD) next.D5 = b.spr.OAM_A_LATCH5;
    /*p31.ZEHA*/ if (OAM_A_CPU_RD) next.D6 = b.spr.OAM_A_LATCH6;
    /*p31.FYRA*/ if (OAM_A_CPU_RD) next.D7 = b.spr.OAM_A_LATCH7;
  }

  {
    /*p29.YDYV*/ next.spr.OAM_B_LATCH0 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH0, b.oam.TS_OAM_B_D0);
    /*p29.YCEB*/ next.spr.OAM_B_LATCH1 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH1, b.oam.TS_OAM_B_D1);
    /*p29.ZUCA*/ next.spr.OAM_B_LATCH2 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH2, b.oam.TS_OAM_B_D2);
    /*p29.WONE*/ next.spr.OAM_B_LATCH3 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH3, b.oam.TS_OAM_B_D3);
    /*p29.ZAXE*/ next.spr.OAM_B_LATCH4 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH4, b.oam.TS_OAM_B_D4);
    /*p29.XAFU*/ next.spr.OAM_B_LATCH5 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH5, b.oam.TS_OAM_B_D5);
    /*p29.YSES*/ next.spr.OAM_B_LATCH6 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH6, b.oam.TS_OAM_B_D6);
    /*p29.ZECA*/ next.spr.OAM_B_LATCH7 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH7, b.oam.TS_OAM_B_D7);

    /*p29.YWOK*/ next.spr.OAM_B_IN_CLK = not(b.vid.OAM_IN_CLKn);
    /*p29.XUSO*/ next.spr.OAM_B_D0 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D0, b.spr.OAM_B_LATCH0);
    /*p29.XEGU*/ next.spr.OAM_B_D1 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D1, b.spr.OAM_B_LATCH1);
    /*p29.YJEX*/ next.spr.OAM_B_D2 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D2, b.spr.OAM_B_LATCH2);
    /*p29.XYJU*/ next.spr.OAM_B_D3 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D3, b.spr.OAM_B_LATCH3);
    /*p29.YBOG*/ next.spr.OAM_B_D4 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D4, b.spr.OAM_B_LATCH4);
    /*p29.WYSO*/ next.spr.OAM_B_D5 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D5, b.spr.OAM_B_LATCH5);
    /*p29.XOTE*/ next.spr.OAM_B_D6 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D6, b.spr.OAM_B_LATCH6);
    /*p29.YZAB*/ next.spr.OAM_B_D7 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D7, b.spr.OAM_B_LATCH7);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.dma.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.dma.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(ADDR_OAM, OAM_BUSYn);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.ctl.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p28.WAFO*/ wire OAM_A0n   = not(b.spr.OAM_A0);
    /*p28.GUKO*/ wire OAM_B_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, OAM_A0n);
    /*p28.WUME*/ wire OAM_B_CPU_RD  = not(OAM_B_CPU_RDn);
    /*p29.YFAP*/ if (OAM_B_CPU_RD) next.D0 = b.spr.OAM_B_LATCH0;
    /*p29.XELE*/ if (OAM_B_CPU_RD) next.D1 = b.spr.OAM_B_LATCH1;
    /*p29.YPON*/ if (OAM_B_CPU_RD) next.D2 = b.spr.OAM_B_LATCH2;
    /*p29.XUVO*/ if (OAM_B_CPU_RD) next.D3 = b.spr.OAM_B_LATCH3;
    /*p29.ZYSA*/ if (OAM_B_CPU_RD) next.D4 = b.spr.OAM_B_LATCH4;
    /*p29.YWEG*/ if (OAM_B_CPU_RD) next.D5 = b.spr.OAM_B_LATCH5;
    /*p29.XABU*/ if (OAM_B_CPU_RD) next.D6 = b.spr.OAM_B_LATCH6;
    /*p29.YTUX*/ if (OAM_B_CPU_RD) next.D7 = b.spr.OAM_B_LATCH7;
  }

  //----------
  // OAM address generator

  {
    /*p28.GARO*/ wire A0n = not(b.A00);
    /*p28.WACU*/ wire A1n = not(b.A01);
    /*p28.GOSE*/ wire A2n = not(b.A02);
    /*p28.WAPE*/ wire A3n = not(b.A03);
    /*p28.FEVU*/ wire A4n = not(b.A04);
    /*p28.GERA*/ wire A5n = not(b.A05);
    /*p28.WAXA*/ wire A6n = not(b.A06);
    /*p28.FOBY*/ wire A7n = not(b.A07);

    /*p28.FYKE*/ wire IDX_0n = not(b.spr.TS_IDX_0);
    /*p28.FUGU*/ wire IDX_1n = not(b.spr.TS_IDX_1);
    /*p28.FACO*/ wire IDX_2n = not(b.spr.TS_IDX_2);
    /*p28.FABY*/ wire IDX_3n = not(b.spr.TS_IDX_3);
    /*p28.GYKA*/ wire IDX_4n = not(b.spr.TS_IDX_4);
    /*p28.GYBU*/ wire IDX_5n = not(b.spr.TS_IDX_5);


    /*p28.WUWE*/ wire SCANZn = not(pins.P10_B);
    /*p28.GUSE*/ wire SCAN0n = not(b.spr.SCAN0);
    /*p28.GEMA*/ wire SCAN1n = not(b.spr.SCAN1);
    /*p28.FUTO*/ wire SCAN2n = not(b.spr.SCAN2);
    /*p28.FAKU*/ wire SCAN3n = not(b.spr.SCAN3);
    /*p28.GAMA*/ wire SCAN4n = not(b.spr.SCAN4);
    /*p28.GOBY*/ wire SCAN5n = not(b.spr.SCAN5);


    /*p28.FETU*/ wire DMA_A07n = not(b.dma.DMA_A07);
    /*p28.FYDU*/ wire DMA_A06n = not(b.dma.DMA_A06);
    /*p28.EDOL*/ wire DMA_A05n = not(b.dma.DMA_A05);
    /*p28.ELUG*/ wire DMA_A04n = not(b.dma.DMA_A04);
    /*p28.FYKY*/ wire DMA_A03n = not(b.dma.DMA_A03);
    /*p28.FAGO*/ wire DMA_A02n = not(b.dma.DMA_A02);
    /*p28.FESA*/ wire DMA_A01n = not(b.dma.DMA_A01);
    /*p28.FODO*/ wire DMA_A00n = not(b.dma.DMA_A00);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.dma.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.ASAM*/ wire OAM_ADDR_CPU    = or(b.spr.OAM_ADDR_PARSEn , b.vid.RENDERING, b.dma.DMA_RUNNING_SYNC); // this is definitely or, which seems wrong
    /*p28.BETE*/ wire OAM_ADDR_RENDER = not(OAM_ADDR_RENDERn);
    /*p28.APAR*/ wire OAM_ADDR_PARSE  = not(b.spr.OAM_ADDR_PARSEn);
    /*p04.DUGA*/ wire OAM_ADDR_DMA    = not(b.dma.DMA_RUNNING_SYNC);

    /*p28.GECA*/ wire GECA = not(b.spr.P10_Bn);
    /*p28.WYDU*/ wire WYDU = not(b.spr.P10_Bn);
    /*p28.GEFY*/ wire GEFY = not(pins.P10_B);

    // so byte 0 is the palette number? something wrong here...

    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n

    /*p28.GEKA*/ next.OAM_A0 = not((A0n & OAM_ADDR_CPU) | (GECA   & OAM_ADDR_RENDER) | (GEFY   & OAM_ADDR_PARSE) | (DMA_A00n & OAM_ADDR_DMA));
    /*p28.ZYFO*/ next.OAM_A1 = not((A1n & OAM_ADDR_CPU) | (WYDU   & OAM_ADDR_RENDER) | (SCANZn & OAM_ADDR_PARSE) | (DMA_A01n & OAM_ADDR_DMA));
    /*p28.YFOT*/ next.OAM_A2 = not((A2n & OAM_ADDR_CPU) | (IDX_5n & OAM_ADDR_RENDER) | (SCAN0n & OAM_ADDR_PARSE) | (DMA_A02n & OAM_ADDR_DMA));
    /*p28.YFOC*/ next.OAM_A3 = not((A3n & OAM_ADDR_CPU) | (IDX_4n & OAM_ADDR_RENDER) | (SCAN1n & OAM_ADDR_PARSE) | (DMA_A03n & OAM_ADDR_DMA));
    /*p28.YVOM*/ next.OAM_A4 = not((A4n & OAM_ADDR_CPU) | (IDX_3n & OAM_ADDR_RENDER) | (SCAN2n & OAM_ADDR_PARSE) | (DMA_A04n & OAM_ADDR_DMA));
    /*p28.YMEV*/ next.OAM_A5 = not((A5n & OAM_ADDR_CPU) | (IDX_2n & OAM_ADDR_RENDER) | (SCAN3n & OAM_ADDR_PARSE) | (DMA_A05n & OAM_ADDR_DMA));
    /*p28.XEMU*/ next.OAM_A6 = not((A6n & OAM_ADDR_CPU) | (IDX_1n & OAM_ADDR_RENDER) | (SCAN4n & OAM_ADDR_PARSE) | (DMA_A06n & OAM_ADDR_DMA));
    /*p28.YZET*/ next.OAM_A7 = not((A7n & OAM_ADDR_CPU) | (IDX_0n & OAM_ADDR_RENDER) | (SCAN5n & OAM_ADDR_PARSE) | (DMA_A07n & OAM_ADDR_DMA));
  }

  /*p31.ZAGO*/ next.spr.OAM_A_D0b = not(!b.spr.OAM_A_D0);
  /*p31.ZOCY*/ next.spr.OAM_A_D1b = not(!b.spr.OAM_A_D1);
  /*p31.YPUR*/ next.spr.OAM_A_D2b = not(!b.spr.OAM_A_D2);
  /*p31.YVOK*/ next.spr.OAM_A_D3b = not(!b.spr.OAM_A_D3);
  /*p31.COSE*/ next.spr.OAM_A_D4b = not(!b.spr.OAM_A_D4);
  /*p31.AROP*/ next.spr.OAM_A_D5b = not(!b.spr.OAM_A_D5);
  /*p31.XATU*/ next.spr.OAM_A_D6b = not(!b.spr.OAM_A_D6);
  /*p31.BADY*/ next.spr.OAM_A_D7b = not(!b.spr.OAM_A_D7);


  /*p28.AZAR*/ wire DMA_READ_VRAMp = not(next.dma.DMA_READ_VRAM);
  /*p28.WUZU*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D0 = b.MD0;
  /*p28.AXER*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D1 = b.MD1;
  /*p28.ASOX*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D2 = b.MD2;
  /*p28.CETU*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D3 = b.MD3;
  /*p28.ARYN*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D4 = b.MD4;
  /*p28.ACOT*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D5 = b.MD5;
  /*p28.CUJE*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D6 = b.MD6;
  /*p28.ATER*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_A_D7 = b.MD7;
  /*p28.WOWA*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D0 = b.MD0;
  /*p28.AVEB*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D1 = b.MD1;
  /*p28.AMUH*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D2 = b.MD2;
  /*p28.COFO*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D3 = b.MD3;
  /*p28.AZOZ*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D4 = b.MD4;
  /*p28.AGYK*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D5 = b.MD5;
  /*p28.BUSE*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D6 = b.MD6;
  /*p28.ANUM*/ if (!DMA_READ_VRAMp) next.oam.TS_OAM_B_D7 = b.MD7;

  /*p25.WEJO*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D0 = not(/*p25.RALO*/ not(b.pin.D0_C));
  /*p25.BUBO*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D1 = not(/*p25.TUNE*/ not(b.pin.D1_C));
  /*p25.BETU*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D2 = not(/*p25.SERA*/ not(b.pin.D2_C));
  /*p25.CYME*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D3 = not(/*p25.TENU*/ not(b.pin.D3_C));
  /*p25.BAXU*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D4 = not(/*p25.SYSA*/ not(b.pin.D4_C));
  /*p25.BUHU*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D5 = not(/*p25.SUGY*/ not(b.pin.D5_C));
  /*p25.BYNY*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D6 = not(/*p25.TUBE*/ not(b.pin.D6_C));
  /*p25.BYPY*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_A_D7 = not(/*p25.SYZO*/ not(b.pin.D7_C));
  /*p25.WASA*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D0 = not(/*p25.RALO*/ not(b.pin.D0_C));
  /*p25.BOMO*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D1 = not(/*p25.TUNE*/ not(b.pin.D1_C));
  /*p25.BASA*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D2 = not(/*p25.SERA*/ not(b.pin.D2_C));
  /*p25.CAKO*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D3 = not(/*p25.TENU*/ not(b.pin.D3_C));
  /*p25.BUMA*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D4 = not(/*p25.SYSA*/ not(b.pin.D4_C));
  /*p25.BUPY*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D5 = not(/*p25.SUGY*/ not(b.pin.D5_C));
  /*p25.BASY*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D6 = not(/*p25.TUBE*/ not(b.pin.D6_C));
  /*p25.BAPE*/ if (!DMA_READ_CARTn) next.oam.TS_OAM_B_D7 = not(/*p25.SYZO*/ not(b.pin.D7_C));

#endif
}

//-----------------------------------------------------------------------------

};