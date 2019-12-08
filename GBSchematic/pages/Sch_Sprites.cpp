#include "Sch_Sprites.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void Sprites_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p28.WEFE*/ c.spr.P10_Bn = not(chip_in.P10_B);
  /*p28.AWOH*/ c.spr.CLK_2Mn = not(b.vid.CLK_AxxDExxHb);
  
  /*p04.DECY*/ c.sys.FROM_CPU5n = not(cpu_in.FROM_CPU5);
  /*p04.CATY*/ c.sys.FROM_CPU5  = not(b.sys.FROM_CPU5n);
  /*p28.BOFE*/ c.spr.FROM_CPU5n = not(b.sys.FROM_CPU5);
  



  /*p29.WOSU*/ c.spr.CLK_WOSU = tock_pos(a.vid.CLK_AxCxExGxc, b.vid.CLK_AxCxExGxc, b.sys.VID_RESETn1, b.spr.CLK_WOSU, !b.vid.CLK_AxxDExxHa);
  /*p29.XOCE*/ c.spr.CLK_XOCE = not(b.spr.CLK_WOSU);

  {
    // dma polarity backwards?
    //OAM_ADDR_PARSEn  = and(DMA_RUNNING_SYNC, or(IN_LINE_SYNCa, VID_RESET6, SCAN_DONE_TRIG));

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.ASEN*/ wire ASEN = or(b.sys.VID_RESET6, b.spr.SCAN_DONE_TRIG);
    /*p28.BESU*/ wire BESU = or(b.spr.IN_LINE_SYNCa, ASEN);
    /*p28.ACYL*/ c.spr.OAM_ADDR_PARSEn  = and(DMA_RUNNING_SYNCb, BESU);
  }

  {
    /*p29.WOJO*/ wire WOJO = nor(!b.vid.CLK_AxxDExxHa, !b.spr.CLK_WOSU);
    /*p29.XYSO*/ wire XYSO = not(WOJO);
    /*p25.AVER*/ wire AVER = and(b.spr.OAM_ADDR_PARSEn, XYSO);
    /*p29.TUVO*/ wire TUVO = or(b.spr.RENDERINGn, b.spr.SEQ1, b.spr.SPRITE_ABn);
    /*p25.VAPE*/ wire VAPE = and(b.spr.SEQ_5_TRIG, TUVO);
    /*p25.XUJY*/ wire XUJY = not(VAPE);
    /*p25.CUFE*/ wire OAM_WR = or(and(b.sys.ADDR_OAM, b.sys.DMA_RUNNING_SYNC), b.sys.PHASE_xxxxEFGH8b);
    /*p25.BYCU*/ wire OAM_IN_CLK = nor(OAM_WR, XUJY, AVER);
    /*p25.COTA*/ c.vid.OAM_IN_CLKn = not(OAM_IN_CLK); // -> OAM

  }

  {
    /*p29.TUVO*/ wire TUVO = or(b.spr.RENDERINGn, b.spr.SEQ1, b.spr.SPRITE_ABn);
    /*p28.WEFY*/ wire WEFY = and(TUVO, b.spr.SEQ_5_SYNCn);

    /*p28.AJEP*/ wire AJEP = and(b.spr.OAM_ADDR_PARSEn, b.spr.CLK_XOCE);
    /*p28.XUJA*/ wire XUJA = not(WEFY);
    /*p28.BOTA*/ wire BOTA = nor(b.spr.FROM_CPU5n, b.sys.ADDR_OAM, b.sys.CPU_RD2);

    /*p28.ASYT*/ wire OAM_LATCHn = and(AJEP, XUJA, BOTA);
    /*p28.BODE*/ c.spr.OAM_LATCH = not(OAM_LATCHn);
  }

  {
    /*p28.YVAL*/ wire OAM_LATCHo = not(b.spr.OAM_LATCH);
    /*p28.YRYV*/ wire OAM_LATCHb = not(OAM_LATCHo);
    /*p28.ZODO*/ c.OAM_CLK = not(OAM_LATCHb); // -> OAM
  }
    

  {
    /*p04.MAKA*/ c.sys.FROM_CPU5_SYNC = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.SYS_RESETn2, b.sys.FROM_CPU5_SYNC, b.sys.FROM_CPU5);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);

    /*p28.WAFO*/ wire OAM_A0n   = not(b.spr.OAM_A0);
    /*p04.NAXY*/ wire NAXY      = nor(b.sys.FROM_CPU5_SYNC, b.sys.DMA_STARTa);
    /*p04.POWU*/ wire POWU      = and(b.sys.DMA_RUNNING_SYNC, NAXY);
    /*p04.WYJA*/ wire OAM_CS    = unk3(OAM_LOCKn, b.sys.CPU_WR2, POWU);
    /*p28.YNYC*/ wire OAM_B_CSn = and(OAM_CS, OAM_A0n);
    /*p28.YLYC*/ wire OAM_A_CSn = and(OAM_CS, b.spr.OAM_A0);
    /*p28.ZOFE*/ c.spr.OAM_B_CS = not(OAM_B_CSn); // -> OAM
    /*p28.ZONE*/ c.spr.OAM_A_CS = not(OAM_A_CSn); // -> OAM
  }


  // these are unused
  /*
  c.spr.XUCA = not(b.vid.FF40_WR);
  c.spr.XECY = tock_pos(a.spr.XUCA, b.spr.XUCA, 0,          b.spr.XECY, b.sys.SYS_RESETn4); // ? weird
  c.spr.XUVA = tock_pos(a.spr.XYNY, b.spr.XYNY, b.sys.XARE, b.spr.XUVA, b.spr.XECY);
  */

  //----------
  // bottom


  //----------
  // Sprite scanner
  
  {
    /*p01.XAPO*/ wire VID_RESETn1 = not(b.sys.VID_RESET1);
    /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn1);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
    /*p01.AMYG*/ wire VID_RESET7  = not(VID_RESETn1);

    /*p21.PURE*/ wire LINE_DONEa     = not(b.vid.LINE_DONE_SYNCa);
    /*p21.SELA*/ wire LINE_DONEo     = not(LINE_DONEa);
    /*p21.XYVO*/ wire LINE_144       = and(b.vid.V4, b.vid.V7); // 128 + 16 = 144
    /*p29.ALES*/ wire LINE_144n      = not(LINE_144);
    /*p29.ABOV*/ wire IN_LINEb       = and(LINE_DONEo, LINE_144n);
    /*p29.CATU*/ c.spr.IN_LINE_SYNCa = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, VID_RESETn3, b.spr.IN_LINE_SYNCa, IN_LINEb);
    /*p28.ANEL*/ c.spr.IN_LINE_SYNCb = tock_pos(a.spr.CLK_2Mn,       b.spr.CLK_2Mn,       VID_RESETn3, b.spr.IN_LINE_SYNCb, b.spr.IN_LINE_SYNCa);

    /*p28.ABAF*/ wire IN_LINEn  = not(b.spr.IN_LINE_SYNCa);
    /*p28.BYHA*/ wire NEW_LINEn = or(b.spr.IN_LINE_SYNCb, IN_LINEn, b.sys.VID_RESETn3);
    
    /*p28.ATEJ*/ c.spr.NEW_LINE  = not(NEW_LINEn); // this drives sprite count reset, it must be new line.
    /*p28.ABAK*/ wire NEW_LINE2 = or(b.spr.NEW_LINE, VID_RESET7);
    /*p28.BYVA*/ c.spr.NEW_LINE3 = not(NEW_LINE2);
    /*p29.DYBA*/ c.spr.NEW_LINE4 = not(b.spr.NEW_LINE3);
  }



  /*p29.BUZA*/ c.spr.STORE_SPRITE_IDX = and(!b.spr.CENO, b.vid.RENDERING);
  /*p29.CEHA*/ c.spr.CEHA = not(!b.spr.CENO);
  
  {
    /*p28.ASEN*/ wire ASEN = or(b.sys.VID_RESET6, b.spr.SCAN_DONE_TRIG);
    /*p28.BESU*/ wire BESU = or(b.spr.IN_LINE_SYNCa, ASEN);
    /*p29.CENO*/ c.spr.CENO = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, b.sys.VID_RESETn3, b.spr.CENO, BESU);
  }



  {
    /*p28.ANOM*/ wire SCAN_RSTn = nor(b.sys.VID_RESET6, b.spr.NEW_LINE);

    /*p28.FETO*/ wire SCAN_DONE = and(b.spr.SCAN0, b.spr.SCAN1, b.spr.SCAN2, b.spr.SCAN5); // 32 + 4 + 2 + 1 = 39

    /*p28.GAVA*/ c.spr.SCAN_CLK  = or(SCAN_DONE, b.vid.CLK_AxxDExxHb);
    /*p28.YFEL*/ c.spr.SCAN0 = tock_pos( a.spr.SCAN_CLK, b.spr.SCAN_CLK, SCAN_RSTn, b.spr.SCAN0, !b.spr.SCAN0);
    /*p28.WEWY*/ c.spr.SCAN1 = tock_pos(!a.spr.SCAN0,    !b.spr.SCAN0,   SCAN_RSTn, b.spr.SCAN1, !b.spr.SCAN1);
    /*p28.GOSO*/ c.spr.SCAN2 = tock_pos(!a.spr.SCAN1,    !b.spr.SCAN1,   SCAN_RSTn, b.spr.SCAN2, !b.spr.SCAN2);
    /*p28.ELYN*/ c.spr.SCAN3 = tock_pos(!a.spr.SCAN2,    !b.spr.SCAN2,   SCAN_RSTn, b.spr.SCAN3, !b.spr.SCAN3);
    /*p28.FAHA*/ c.spr.SCAN4 = tock_pos(!a.spr.SCAN3,    !b.spr.SCAN3,   SCAN_RSTn, b.spr.SCAN4, !b.spr.SCAN4);
    /*p28.FONY*/ c.spr.SCAN5 = tock_pos(!a.spr.SCAN4,    !b.spr.SCAN4,   SCAN_RSTn, b.spr.SCAN5, !b.spr.SCAN5);

    /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
    /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);

    // clocks on these are odd...
    /*p29.BYBA*/ c.spr.SCAN_DONE_SYNC1 = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, SCAN_RSTo, b.spr.SCAN_DONE_SYNC1, SCAN_DONE);
    /*p29.DOBA*/ c.spr.SCAN_DONE_SYNC2 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, SCAN_RSTo, b.spr.SCAN_DONE_SYNC2, b.spr.SCAN_DONE_SYNC1);

    /*p29.BEBU or  */ wire SCAN_DONE_TRIGn = or(SCAN_RSTa, b.spr.SCAN_DONE_SYNC2, !b.spr.SCAN_DONE_SYNC1);
    /*p29.AVAP not2*/ c.spr.SCAN_DONE_TRIG = not(SCAN_DONE_TRIGn);
  }

  
  //----------
  // undeciphered






  {
    // tags wrong somewhere
    /*p29.TAME*/ wire SEQ_5n = nand(b.spr.SEQ2, b.spr.SEQ0);
    /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);
    /*p27.VYPO*/ wire VYPO = not(chip_in.P10_B);

    /*p01.ATAL*/ wire ATAL_AxCxExGx = not(b.sys.ROOTCLK_xBxDxFxH3);
    /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
    /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
    /*p01.ALET*/ wire ALET_AxCxExGx = not(ZEME_xBxDxFxH);
    /*p29.LAPE*/ c.vid.CLK_xBxDxFxHd  = not(ALET_AxCxExGx);
    /*p29.TYFO*/ c.spr.SEQ_5_SYNCn = tock_pos(a.vid.CLK_xBxDxFxHd,  b.vid.CLK_xBxDxFxHd, VYPO, b.spr.SEQ_5_SYNCn, SEQ_5n);
    /*p29.TACU*/ c.spr.SEQ_5_TRIG = nand(SEQ_5, b.spr.SEQ_5_SYNCn);
    
    /*p27.RYCE*/ wire TEKY_TRIG = and(b.vid.TEKY_SYNC1, !b.vid.TEKY_SYNC2);
    /*p27.SECA*/ wire SECA_RST = nor(TEKY_TRIG, b.sys.VID_RESET5, b.spr.NEW_LINE);

    /*p29.TOMA*/ c.spr.TOMA_CLK = nand(b.vid.CLK_xBxDxFxHd, SEQ_5n);
    /*p29.TOXE*/ c.spr.SEQ0     = tock_pos( a.spr.TOMA_CLK, b.spr.TOMA_CLK, SECA_RST, b.spr.SEQ0, !b.spr.SEQ0);
    /*p29.TULY*/ c.spr.SEQ1     = tock_pos(!a.spr.SEQ0,     !b.spr.SEQ0,    SECA_RST, b.spr.SEQ1, !b.spr.SEQ1);
    /*p29.TESE*/ c.spr.SEQ2     = tock_pos(!a.spr.SEQ1,     !b.spr.SEQ1,    SECA_RST, b.spr.SEQ2, !b.spr.SEQ2);

    /*p29.SYCU*/ wire SYCU = nor(SEQ_5, b.spr.RENDERINGn, b.spr.SEQ_5_SYNCn);

    /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(b.spr.SEQ1, SYCU);
    /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
    /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
    /*p29.XADO*/ c.spr.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);

    /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(b.spr.SPRITE_ABn, SYCU);
    /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
    /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
    /*p29.PUCO*/ c.spr.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);

    /*p27.LAXE*/ wire BG_SEQ0n = not(b.vid.BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(b.vid.RENDERINGn, BG_SEQ0n, b.vid.BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(b.vid.BG_SEQ_xxxx4567);
    /*p27.NYDY*/ wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, b.vid.BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/ wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ c.pix.BG_LATCH = not(BG_SEQ_TRIG_3);
     
    /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(b.vid.BG_SEQ_xx23xx67);
    /*p27.MOFU*/ wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/ wire LESO = not(MOFU);
    /*p32.AJAR*/ wire AJAR = not(LESO);
    /*p32.LABU*/ c.pix.VRAM_TEMP_CLK = not(AJAR);

    /*p29.LAPE*/ c.vid.CLK_xBxDxFxHd  = not(ALET_AxCxExGx);
    /*p01.LAPE*/ c.spr.CLK_AxCxExGx10 = not(b.vid.CLK_xBxDxFxHd);
    /*p29.TOBU*/ c.spr.TULY_SYNC1 = tock_pos(a.spr.CLK_AxCxExGx10, b.spr.CLK_AxCxExGx10, b.vid.RENDERING, b.spr.TULY_SYNC1, b.spr.SEQ1);
    /*p29.VONU*/ c.spr.SPRITE_ABn = tock_pos(a.spr.CLK_AxCxExGx10, b.spr.CLK_AxCxExGx10, b.vid.RENDERING, b.spr.SPRITE_ABn, !b.spr.TULY_SYNC1);
    /*p29.SEBA*/ c.spr.SEBA       = tock_pos(a.vid.CLK_xBxDxFxHd,  b.vid.CLK_xBxDxFxHd,  b.vid.RENDERING, b.spr.SEBA,       b.spr.SPRITE_ABn);

    /*p29.TYNO*/ wire TYNO = nand(b.spr.SEQ0, b.spr.SEBA, b.spr.SPRITE_ABn);
    /*p29.VUSA*/ wire VUSA = or(!b.spr.SEQ_5_SYNCn, TYNO);
    /*p29.WUTY*/ c.spr.WUTY_CLK = not(VUSA);
  }

  //----------











  //----------


  {
    // OAM address mux

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);

    /*p28.ADAH*/ wire ADDR_OAMn = not(b.sys.ADDR_OAM);
    /*p28.XUTO*/ wire CPU_OAM_WR = and(b.sys.ADDR_OAM, b.sys.CPU_WR2);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(b.sys.PHASE_xxxxEFGH8b);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);

    /*p28.ZAXA*/ if (D_TO_OAMD) c.TS_OAM_A_D0 = b.D0;
    /*p28.ZAKY*/ if (D_TO_OAMD) c.TS_OAM_A_D1 = b.D1;
    /*p28.WULE*/ if (D_TO_OAMD) c.TS_OAM_A_D2 = b.D2;
    /*p28.ZOZO*/ if (D_TO_OAMD) c.TS_OAM_A_D3 = b.D3;
    /*p28.ZUFO*/ if (D_TO_OAMD) c.TS_OAM_A_D4 = b.D4;
    /*p28.ZATO*/ if (D_TO_OAMD) c.TS_OAM_A_D5 = b.D5;
    /*p28.YVUC*/ if (D_TO_OAMD) c.TS_OAM_A_D6 = b.D6;
    /*p28.ZUFE*/ if (D_TO_OAMD) c.TS_OAM_A_D7 = b.D7;
    /*p28.ZAMY*/ if (D_TO_OAMD) c.TS_OAM_B_D0 = b.D0;
    /*p28.ZOPU*/ if (D_TO_OAMD) c.TS_OAM_B_D1 = b.D1;
    /*p28.WYKY*/ if (D_TO_OAMD) c.TS_OAM_B_D2 = b.D2;
    /*p28.ZAJA*/ if (D_TO_OAMD) c.TS_OAM_B_D3 = b.D3;
    /*p28.ZUGA*/ if (D_TO_OAMD) c.TS_OAM_B_D4 = b.D4;
    /*p28.ZUMO*/ if (D_TO_OAMD) c.TS_OAM_B_D5 = b.D5;
    /*p28.XYTO*/ if (D_TO_OAMD) c.TS_OAM_B_D6 = b.D6;
    /*p28.ZYFA*/ if (D_TO_OAMD) c.TS_OAM_B_D7 = b.D7;
  }
  

  //----------
  // OAM data in latch/reg


  {
    /*p31.XYKY*/ c.spr.OAM_A_LATCH0 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH0, b.TS_OAM_A_D0);
    /*p31.YRUM*/ c.spr.OAM_A_LATCH1 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH1, b.TS_OAM_A_D1);
    /*p31.YSEX*/ c.spr.OAM_A_LATCH2 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH2, b.TS_OAM_A_D2);
    /*p31.YVEL*/ c.spr.OAM_A_LATCH3 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH3, b.TS_OAM_A_D3);
    /*p31.WYNO*/ c.spr.OAM_A_LATCH4 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH4, b.TS_OAM_A_D4);
    /*p31.CYRA*/ c.spr.OAM_A_LATCH5 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH5, b.TS_OAM_A_D5);
    /*p31.ZUVE*/ c.spr.OAM_A_LATCH6 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH6, b.TS_OAM_A_D6);
    /*p31.ECED*/ c.spr.OAM_A_LATCH7 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_A_LATCH7, b.TS_OAM_A_D7);

    /*p31.XEGA*/ c.spr.OAM_A_IN_CLK = not(b.vid.OAM_IN_CLKn);
    /*p31.YLOR*/ c.spr.OAM_A_D0 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D0, b.spr.OAM_A_LATCH0);
    /*p31.ZYTY*/ c.spr.OAM_A_D1 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D1, b.spr.OAM_A_LATCH1);
    /*p31.ZYVE*/ c.spr.OAM_A_D2 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D2, b.spr.OAM_A_LATCH2);
    /*p31.ZEZY*/ c.spr.OAM_A_D3 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D3, b.spr.OAM_A_LATCH3);
    /*p31.GOMO*/ c.spr.OAM_A_D4 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D4, b.spr.OAM_A_LATCH4);
    /*p31.BAXO*/ c.spr.OAM_A_D5 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D5, b.spr.OAM_A_LATCH5);
    /*p31.YZOS*/ c.spr.OAM_A_D6 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D6, b.spr.OAM_A_LATCH6);
    /*p31.DEPO*/ c.spr.OAM_A_D7 = tock_pos(a.spr.OAM_A_IN_CLK, b.spr.OAM_A_IN_CLK, 0, b.spr.OAM_A_D7, b.spr.OAM_A_LATCH7);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);
    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.sys.CPU_RD2, b.sys.FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p28.WUKU*/ wire OAM_A_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, b.spr.OAM_A0);
    /*p28.WEWU*/ wire OAM_A_CPU_RD  = not(OAM_A_CPU_RDn);
    /*p31.XACA*/ if (OAM_A_CPU_RD) c.D0 = b.spr.OAM_A_LATCH0;
    /*p31.XAGU*/ if (OAM_A_CPU_RD) c.D1 = b.spr.OAM_A_LATCH1;
    /*p31.XEPU*/ if (OAM_A_CPU_RD) c.D2 = b.spr.OAM_A_LATCH2;
    /*p31.XYGU*/ if (OAM_A_CPU_RD) c.D3 = b.spr.OAM_A_LATCH3;
    /*p31.XUNA*/ if (OAM_A_CPU_RD) c.D4 = b.spr.OAM_A_LATCH4;
    /*p31.DEVE*/ if (OAM_A_CPU_RD) c.D5 = b.spr.OAM_A_LATCH5;
    /*p31.ZEHA*/ if (OAM_A_CPU_RD) c.D6 = b.spr.OAM_A_LATCH6;
    /*p31.FYRA*/ if (OAM_A_CPU_RD) c.D7 = b.spr.OAM_A_LATCH7;
  }

  {
    /*p29.YDYV*/ c.spr.OAM_B_LATCH0 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH0, b.TS_OAM_B_D0);
    /*p29.YCEB*/ c.spr.OAM_B_LATCH1 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH1, b.TS_OAM_B_D1);
    /*p29.ZUCA*/ c.spr.OAM_B_LATCH2 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH2, b.TS_OAM_B_D2);
    /*p29.WONE*/ c.spr.OAM_B_LATCH3 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH3, b.TS_OAM_B_D3);
    /*p29.ZAXE*/ c.spr.OAM_B_LATCH4 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH4, b.TS_OAM_B_D4);
    /*p29.XAFU*/ c.spr.OAM_B_LATCH5 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH5, b.TS_OAM_B_D5);
    /*p29.YSES*/ c.spr.OAM_B_LATCH6 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH6, b.TS_OAM_B_D6);
    /*p29.ZECA*/ c.spr.OAM_B_LATCH7 = latch_pos(b.spr.OAM_LATCH, b.spr.OAM_B_LATCH7, b.TS_OAM_B_D7);

    /*p29.YWOK*/ c.spr.OAM_B_IN_CLK = not(b.vid.OAM_IN_CLKn);
    /*p29.XUSO*/ c.spr.OAM_B_D0 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D0, b.spr.OAM_B_LATCH0);
    /*p29.XEGU*/ c.spr.OAM_B_D1 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D1, b.spr.OAM_B_LATCH1);
    /*p29.YJEX*/ c.spr.OAM_B_D2 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D2, b.spr.OAM_B_LATCH2);
    /*p29.XYJU*/ c.spr.OAM_B_D3 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D3, b.spr.OAM_B_LATCH3);
    /*p29.YBOG*/ c.spr.OAM_B_D4 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D4, b.spr.OAM_B_LATCH4);
    /*p29.WYSO*/ c.spr.OAM_B_D5 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D5, b.spr.OAM_B_LATCH5);
    /*p29.XOTE*/ c.spr.OAM_B_D6 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D6, b.spr.OAM_B_LATCH6);
    /*p29.YZAB*/ c.spr.OAM_B_D7 = tock_pos(a.spr.OAM_B_IN_CLK, b.spr.OAM_B_IN_CLK, 0, b.spr.OAM_B_D7, b.spr.OAM_B_LATCH7);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.sys.CPU_RD2, b.sys.FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNC, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);
    /*p28.WAFO*/ wire OAM_A0n   = not(b.spr.OAM_A0);
    /*p28.GUKO*/ wire OAM_B_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, OAM_A0n);
    /*p28.WUME*/ wire OAM_B_CPU_RD  = not(OAM_B_CPU_RDn);
    /*p29.YFAP*/ if (OAM_B_CPU_RD) c.D0 = b.spr.OAM_B_LATCH0;
    /*p29.XELE*/ if (OAM_B_CPU_RD) c.D1 = b.spr.OAM_B_LATCH1;
    /*p29.YPON*/ if (OAM_B_CPU_RD) c.D2 = b.spr.OAM_B_LATCH2;
    /*p29.XUVO*/ if (OAM_B_CPU_RD) c.D3 = b.spr.OAM_B_LATCH3;
    /*p29.ZYSA*/ if (OAM_B_CPU_RD) c.D4 = b.spr.OAM_B_LATCH4;
    /*p29.YWEG*/ if (OAM_B_CPU_RD) c.D5 = b.spr.OAM_B_LATCH5;
    /*p29.XABU*/ if (OAM_B_CPU_RD) c.D6 = b.spr.OAM_B_LATCH6;
    /*p29.YTUX*/ if (OAM_B_CPU_RD) c.D7 = b.spr.OAM_B_LATCH7;
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


    /*p28.WUWE*/ wire SCANZn = not(chip_in.P10_B);
    /*p28.GUSE*/ wire SCAN0n = not(b.spr.SCAN0);
    /*p28.GEMA*/ wire SCAN1n = not(b.spr.SCAN1);
    /*p28.FUTO*/ wire SCAN2n = not(b.spr.SCAN2);
    /*p28.FAKU*/ wire SCAN3n = not(b.spr.SCAN3);
    /*p28.GAMA*/ wire SCAN4n = not(b.spr.SCAN4);
    /*p28.GOBY*/ wire SCAN5n = not(b.spr.SCAN5);


    /*p28.FETU*/ wire DMA_A07n = not(b.sys.DMA_A07);
    /*p28.FYDU*/ wire DMA_A06n = not(b.sys.DMA_A06);
    /*p28.EDOL*/ wire DMA_A05n = not(b.sys.DMA_A05);
    /*p28.ELUG*/ wire DMA_A04n = not(b.sys.DMA_A04);
    /*p28.FYKY*/ wire DMA_A03n = not(b.sys.DMA_A03);
    /*p28.FAGO*/ wire DMA_A02n = not(b.sys.DMA_A02);
    /*p28.FESA*/ wire DMA_A01n = not(b.sys.DMA_A01);
    /*p28.FODO*/ wire DMA_A00n = not(b.sys.DMA_A00);

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNC);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.ASAM*/ wire OAM_ADDR_CPU    = or(b.spr.OAM_ADDR_PARSEn , b.vid.RENDERING, b.sys.DMA_RUNNING_SYNC); // this is definitely or, which seems wrong
    /*p28.BETE*/ wire OAM_ADDR_RENDER = not(OAM_ADDR_RENDERn);
    /*p28.APAR*/ wire OAM_ADDR_PARSE  = not(b.spr.OAM_ADDR_PARSEn);
    /*p04.DUGA*/ wire OAM_ADDR_DMA    = not(b.sys.DMA_RUNNING_SYNC);

    /*p28.GECA*/ wire GECA = not(b.spr.P10_Bn);
    /*p28.WYDU*/ wire WYDU = not(b.spr.P10_Bn);
    /*p28.GEFY*/ wire GEFY = not(chip_in.P10_B);

    // so byte 0 is the palette number? something wrong here...

    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n

    /*p28.GEKA*/ c.spr.OAM_A0 = not((A0n & OAM_ADDR_CPU) | (GECA   & OAM_ADDR_RENDER) | (GEFY   & OAM_ADDR_PARSE) | (DMA_A00n & OAM_ADDR_DMA));
    /*p28.ZYFO*/ c.spr.OAM_A1 = not((A1n & OAM_ADDR_CPU) | (WYDU   & OAM_ADDR_RENDER) | (SCANZn & OAM_ADDR_PARSE) | (DMA_A01n & OAM_ADDR_DMA));
    /*p28.YFOT*/ c.spr.OAM_A2 = not((A2n & OAM_ADDR_CPU) | (IDX_5n & OAM_ADDR_RENDER) | (SCAN0n & OAM_ADDR_PARSE) | (DMA_A02n & OAM_ADDR_DMA));
    /*p28.YFOC*/ c.spr.OAM_A3 = not((A3n & OAM_ADDR_CPU) | (IDX_4n & OAM_ADDR_RENDER) | (SCAN1n & OAM_ADDR_PARSE) | (DMA_A03n & OAM_ADDR_DMA));
    /*p28.YVOM*/ c.spr.OAM_A4 = not((A4n & OAM_ADDR_CPU) | (IDX_3n & OAM_ADDR_RENDER) | (SCAN2n & OAM_ADDR_PARSE) | (DMA_A04n & OAM_ADDR_DMA));
    /*p28.YMEV*/ c.spr.OAM_A5 = not((A5n & OAM_ADDR_CPU) | (IDX_2n & OAM_ADDR_RENDER) | (SCAN3n & OAM_ADDR_PARSE) | (DMA_A05n & OAM_ADDR_DMA));
    /*p28.XEMU*/ c.spr.OAM_A6 = not((A6n & OAM_ADDR_CPU) | (IDX_1n & OAM_ADDR_RENDER) | (SCAN4n & OAM_ADDR_PARSE) | (DMA_A06n & OAM_ADDR_DMA));
    /*p28.YZET*/ c.spr.OAM_A7 = not((A7n & OAM_ADDR_CPU) | (IDX_0n & OAM_ADDR_RENDER) | (SCAN5n & OAM_ADDR_PARSE) | (DMA_A07n & OAM_ADDR_DMA));
  }

  //----------
  // Sprite y comparator

  {
    /*p29.EBOS*/ wire V0n = not(b.vid.V0);
    /*p29.DASA*/ wire V1n = not(b.vid.V1);
    /*p29.FUKY*/ wire V2n = not(b.vid.V2);
    /*p29.FUVE*/ wire V3n = not(b.vid.V3);
    /*p29.FEPU*/ wire V4n = not(b.vid.V4);
    /*p29.FOFA*/ wire V5n = not(b.vid.V5);
    /*p29.FEMO*/ wire V6n = not(b.vid.V6);
    /*p29.GUSU*/ wire V7n = not(b.vid.V7);

    /*p29.ERUC*/ wire YDIFF0   = add_c(V0n, !b.spr.OAM_B_D0, chip_in.P10_B);
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(V0n, !b.spr.OAM_B_D0, chip_in.P10_B);
    /*p29.ENEF*/ wire YDIFF1   = add_s(V1n, !b.spr.OAM_B_D1, YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(V1n, !b.spr.OAM_B_D1, YDIFF_C0);
    /*p29.FECO*/ wire YDIFF2   = add_s(V2n, !b.spr.OAM_B_D2, YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(V2n, !b.spr.OAM_B_D2, YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF3   = add_s(V3n, !b.spr.OAM_B_D3, YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(V3n, !b.spr.OAM_B_D3, YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF4   = add_s(V4n, !b.spr.OAM_B_D4, YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(V4n, !b.spr.OAM_B_D4, YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF5   = add_s(V5n, !b.spr.OAM_B_D5, YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(V5n, !b.spr.OAM_B_D5, YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF6   = add_s(V6n, !b.spr.OAM_B_D6, YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(V6n, !b.spr.OAM_B_D6, YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF7   = add_s(V7n, !b.spr.OAM_B_D7, YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(V7n, !b.spr.OAM_B_D7, YDIFF_C6);

    /*p29.DEGE*/ wire SPRITE_DELTA0 = not(YDIFF0);
    /*p29.DABY*/ wire SPRITE_DELTA1 = not(YDIFF1);
    /*p29.DABU*/ wire SPRITE_DELTA2 = not(YDIFF2);
    /*p29.GYSA*/ wire SPRITE_DELTA3 = not(YDIFF3);
    /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF4);
    /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF5);
    /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF6);
    /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF7);

    /*p29.GOVU*/ wire GOVU = or(YDIFF3, b.vid.LCDC_SPSIZE);
    /*p29.WOTA*/ wire SPR_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU);
    /*p29.GESE*/ wire SPR_MATCH_Y = not(SPR_MATCH_Yn);
    /*p29.CARE*/ wire STORE_ENn = or(b.spr.CLK_XOCE, b.spr.CEHA, SPR_MATCH_Y);
    /*p29.DYTY*/ c.spr.STORE_EN = not(STORE_ENn);

    /*p30.WENU*/ if (b.spr.STORE_MATCH) c.spr.TS_LINE_0 = SPRITE_DELTA1; // order wrong
    /*p30.CUCU*/ if (b.spr.STORE_MATCH) c.spr.TS_LINE_1 = SPRITE_DELTA2;
    /*p30.CUCA*/ if (b.spr.STORE_MATCH) c.spr.TS_LINE_2 = SPRITE_DELTA0;
    /*p30.CEGA*/ if (b.spr.STORE_MATCH) c.spr.TS_LINE_3 = SPRITE_DELTA3;
  }


  //----------
  // Sprite priority

  /*p29.AROR*/ c.spr.MATCH_EN = and(b.spr.AZEM, b.vid.LCDC_SPEN);
  /*p29.AZEM*/   c.spr.AZEM = and(b.spr.BYJO, b.vid.RENDERING);
  /*p29.BYJO*/     c.spr.BYJO = not(b.spr.CEHA);




  //----------
  // Active sprite counter

  {
    /*p29.BAKY*/ wire SPRITES_FULL = and(b.spr.SPRITE_COUNT1, b.spr.SPRITE_COUNT3);
    /*p29.DEZY*/ c.spr.STORE_EN_SYNC = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.VID_RESETn1, b.spr.STORE_EN_SYNC, b.spr.STORE_EN);

    /*p28.AZYB*/ wire SPRITE_COUNT_RSTn = not(b.spr.NEW_LINE);
    /*p29.CAKE*/ c.spr.SPRITE_COUNT_CLK = or(SPRITES_FULL, b.spr.STORE_EN_SYNC);
    /*p29.BESE*/ c.spr.SPRITE_COUNT0 = tock_pos( a.spr.SPRITE_COUNT_CLK,  b.spr.SPRITE_COUNT_CLK, SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT0, !b.spr.SPRITE_COUNT0);
    /*p29.CUXY*/ c.spr.SPRITE_COUNT1 = tock_pos(!a.spr.SPRITE_COUNT0,    !b.spr.SPRITE_COUNT0,    SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT1, !b.spr.SPRITE_COUNT1);
    /*p29.BEGO*/ c.spr.SPRITE_COUNT2 = tock_pos(!a.spr.SPRITE_COUNT1,    !b.spr.SPRITE_COUNT1,    SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT2, !b.spr.SPRITE_COUNT2);
    /*p29.DYBE*/ c.spr.SPRITE_COUNT3 = tock_pos(!a.spr.SPRITE_COUNT2,    !b.spr.SPRITE_COUNT2,    SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT3, !b.spr.SPRITE_COUNT3);

    // 4 to 10 decoder
    /*p29.EDEN*/ c.spr.SPRITE_COUNT0n = not(b.spr.SPRITE_COUNT0);
    /*p29.FYCU*/ c.spr.SPRITE_COUNT0b = not(b.spr.SPRITE_COUNT0n);
    /*p29.CYPY*/ c.spr.SPRITE_COUNT1n = not(b.spr.SPRITE_COUNT1);
    /*p29.FONE*/ c.spr.SPRITE_COUNT1b = not(b.spr.SPRITE_COUNT1n);
    /*p29.CAPE*/ c.spr.SPRITE_COUNT2n = not(b.spr.SPRITE_COUNT2);
    /*p29.EKUD*/ c.spr.SPRITE_COUNT2b = not(b.spr.SPRITE_COUNT2n);
    /*p29.CAXU*/ c.spr.SPRITE_COUNT3n = not(b.spr.SPRITE_COUNT3);
    /*p29.ELYG*/ c.spr.SPRITE_COUNT3b = not(b.spr.SPRITE_COUNT3b);
  }

  //----------

  {
    /*p30.CYKE*/     c.spr.CYKE = not(b.vid.CLK_AxxDExxHb);
    /*p30.WUDA*/   c.spr.WUDA = not(b.spr.CYKE);
    /*p30.XADU*/ c.spr.IDX_0 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_0, b.spr.OAM_A2);
    /*p30.XEDY*/ c.spr.IDX_1 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_1, b.spr.OAM_A3);
    /*p30.ZUZE*/ c.spr.IDX_2 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_2, b.spr.OAM_A4);
    /*p30.XOBE*/ c.spr.IDX_3 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_3, b.spr.OAM_A5);
    /*p30.YDUF*/ c.spr.IDX_4 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_4, b.spr.OAM_A6);
    /*p30.XECU*/ c.spr.IDX_5 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_5, b.spr.OAM_A7);

    /*p30.WEZA*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_0 = b.spr.IDX_5; // order wrong
    /*p30.WUCO*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_1 = b.spr.IDX_4;
    /*p30.WYDA*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_2 = b.spr.IDX_3;
    /*p30.ZYSU*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_3 = b.spr.IDX_2;
    /*p30.WYSE*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_4 = b.spr.IDX_1;
    /*p30.WUZY*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_5 = b.spr.IDX_0;
  }

  /*p31.ZAGO*/ c.spr.OAM_A_D0b = not(!b.spr.OAM_A_D0);
  /*p31.ZOCY*/ c.spr.OAM_A_D1b = not(!b.spr.OAM_A_D1);
  /*p31.YPUR*/ c.spr.OAM_A_D2b = not(!b.spr.OAM_A_D2);
  /*p31.YVOK*/ c.spr.OAM_A_D3b = not(!b.spr.OAM_A_D3);
  /*p31.COSE*/ c.spr.OAM_A_D4b = not(!b.spr.OAM_A_D4);
  /*p31.AROP*/ c.spr.OAM_A_D5b = not(!b.spr.OAM_A_D5);
  /*p31.XATU*/ c.spr.OAM_A_D6b = not(!b.spr.OAM_A_D6);
  /*p31.BADY*/ c.spr.OAM_A_D7b = not(!b.spr.OAM_A_D7);

  /*p21.ACAM*/ c.vid.X0n = not(b.vid.X0);
  /*p21.AZUB*/ c.vid.X1n = not(b.vid.X1);
  /*p21.AMEL*/ c.vid.X2n = not(b.vid.X2);
  /*p21.AHAL*/ c.vid.X3n = not(b.vid.X3);
  /*p21.APUX*/ c.vid.X4n = not(b.vid.X4);
  /*p21.ABEF*/ c.vid.X5n = not(b.vid.X5);
  /*p21.ADAZ*/ c.vid.X6n = not(b.vid.X6);
  /*p21.ASAH*/ c.vid.X7n = not(b.vid.X7);

  //----------
  // store 0

  {
    /*p31.ZOGY*/ wire STORE0_MATCH0 = xor(b.spr.STORE0_X0, b.vid.X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1 = xor(b.spr.STORE0_X1, b.vid.X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2 = xor(b.spr.STORE0_X2, b.vid.X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3 = xor(b.spr.STORE0_X3, b.vid.X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4 = xor(b.spr.STORE0_X4, b.vid.X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5 = xor(b.spr.STORE0_X5, b.vid.X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6 = xor(b.spr.STORE0_X6, b.vid.X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7 = xor(b.spr.STORE0_X7, b.vid.X7n);
    /*p31.ZAKO*/ wire STORE0_MATCHA = nor(STORE0_MATCH0, STORE0_MATCH1, STORE0_MATCH2, STORE0_MATCH3);
    /*p31.XEBA*/ wire STORE0_MATCHB = nor(STORE0_MATCH4, STORE0_MATCH5, STORE0_MATCH6, STORE0_MATCH7);

    /*p29.YDUG*/ c.spr.STORE0_MATCHn = nand(b.spr.MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
    /*p29.WEFU*/ wire WEFU = not(b.spr.STORE0_MATCHn);
    /*p29.GEZE*/ c.spr.STORE0_MATCH_OUT = or (WEFU, chip_in.P10_B);

    /*p29.GUVA*/ wire SPRITE0_GETn = nor(b.spr.STORE0_MATCHn,  chip_in.P10_B);
    /*p29.EBOJ*/ c.spr.SPRITE0_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE0_GET_SYNCn, SPRITE0_GETn);

    /*p29.DEZO*/ wire STORE0_SEL    = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n);
    /*p29.CEMY*/ wire STORE0_CLK    = or(b.spr.STORE_EN, STORE0_SEL);
    /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
    /*p29.DYWE*/ wire STORE0_RST    = or(b.spr.NEW_LINE4, b.spr.SPRITE0_GET_SYNCn);
    /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

    /*p29.GENY*/ c.spr.STORE0_CLKa  = not(STORE0_CLKn);
    /*p30.YGUS*/ c.spr.STORE0_IDX0  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX0,  b.spr.TS_IDX_0);
    /*p30.YSOK*/ c.spr.STORE0_IDX1  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX1,  b.spr.TS_IDX_1);
    /*p30.YZEP*/ c.spr.STORE0_IDX2  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX2,  b.spr.TS_IDX_2);
    /*p30.WYTE*/ c.spr.STORE0_IDX3  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX3,  b.spr.TS_IDX_3);
    /*p30.ZONY*/ c.spr.STORE0_IDX4  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX4,  b.spr.TS_IDX_4);
    /*p30.YWAK*/ c.spr.STORE0_IDX5  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX5,  b.spr.TS_IDX_5);
    
    /*p29.ENOB*/ c.spr.STORE0_CLKb  = not(STORE0_CLKn);
    /*p30.FYHY*/ c.spr.STORE0_LINE0 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE0, b.spr.TS_LINE_0);
    /*p30.GYHO*/ c.spr.STORE0_LINE1 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE1, b.spr.TS_LINE_1);
    /*p30.BOZU*/ c.spr.STORE0_LINE2 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE2, b.spr.TS_LINE_2);
    /*p30.CUFO*/ c.spr.STORE0_LINE3 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE3, b.spr.TS_LINE_3);

    /*p29.FUXU*/ c.spr.STORE0_CLKc  = not(STORE0_CLKn);
    /*p31.XEPE*/ c.spr.STORE0_X0    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X0,    b.spr.OAM_A_D0b);
    /*p31.YLAH*/ c.spr.STORE0_X1    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X1,    b.spr.OAM_A_D1b);
    /*p31.ZOLA*/ c.spr.STORE0_X2    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X2,    b.spr.OAM_A_D2b);
    /*p31.ZULU*/ c.spr.STORE0_X3    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X3,    b.spr.OAM_A_D3b);
    /*p31.WELO*/ c.spr.STORE0_X4    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X4,    b.spr.OAM_A_D4b);
    /*p31.XUNY*/ c.spr.STORE0_X5    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X5,    b.spr.OAM_A_D5b);
    /*p31.WOTE*/ c.spr.STORE0_X6    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X6,    b.spr.OAM_A_D6b);
    /*p31.XAKO*/ c.spr.STORE0_X7    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X7,    b.spr.OAM_A_D7b);

    /*p29.FURO*/ wire SPRITE0_GET = not(SPRITE0_GETn);
    /*p30.ZETU*/ if (SPRITE0_GET) c.spr.TS_IDX_0  = b.spr.STORE0_IDX0;
    /*p30.ZECE*/ if (SPRITE0_GET) c.spr.TS_IDX_1  = b.spr.STORE0_IDX1;
    /*p30.ZAVE*/ if (SPRITE0_GET) c.spr.TS_IDX_2  = b.spr.STORE0_IDX2;
    /*p30.WOKO*/ if (SPRITE0_GET) c.spr.TS_IDX_3  = b.spr.STORE0_IDX3;
    /*p30.ZUMU*/ if (SPRITE0_GET) c.spr.TS_IDX_4  = b.spr.STORE0_IDX4;
    /*p30.ZEDY*/ if (SPRITE0_GET) c.spr.TS_IDX_5  = b.spr.STORE0_IDX5;
    /*p30.GOFO*/ if (SPRITE0_GET) c.spr.TS_LINE_0 = b.spr.STORE0_LINE0;
    /*p30.WEHE*/ if (SPRITE0_GET) c.spr.TS_LINE_1 = b.spr.STORE0_LINE1;
    /*p30.AJAL*/ if (SPRITE0_GET) c.spr.TS_LINE_2 = b.spr.STORE0_LINE2;
    /*p30.BUKY*/ if (SPRITE0_GET) c.spr.TS_LINE_3 = b.spr.STORE0_LINE3;
  }

  //----------
  // store 1

  {
    /*p31.EDYM*/ wire STORE1_MATCH0 = xor(b.spr.STORE1_X0, b.vid.X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1 = xor(b.spr.STORE1_X1, b.vid.X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2 = xor(b.spr.STORE1_X2, b.vid.X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3 = xor(b.spr.STORE1_X3, b.vid.X3n);
    /*p31.COLA*/ wire STORE1_MATCH4 = xor(b.spr.STORE1_X4, b.vid.X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5 = xor(b.spr.STORE1_X5, b.vid.X5n);
    /*p31.COLU*/ wire STORE1_MATCH6 = xor(b.spr.STORE1_X6, b.vid.X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7 = xor(b.spr.STORE1_X7, b.vid.X7n);
    /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4, STORE1_MATCH5, STORE1_MATCH6, STORE1_MATCH7);
    /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0, STORE1_MATCH1, STORE1_MATCH2, STORE1_MATCH3);

    /*p29.DYDU*/ c.spr.STORE1_MATCHn    = nand(b.spr.MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.GAJA*/ wire STORE1_MATCH = not(b.spr.STORE1_MATCHn);
    /*p29.FUMA*/ c.spr.STORE1_MATCH_OUT = or(STORE1_MATCH, b.spr.STORE0_MATCH_OUT);

    /*p29.ENUT*/ wire SPRITE1_GETn = nor(b.spr.STORE1_MATCHn, b.spr.STORE0_MATCH_OUT);
    /*p29.CEDY*/ c.spr.SPRITE1_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE1_GET_SYNCn, SPRITE1_GETn);

    /*p29.CUVA*/ wire STORE1_SEL  = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n);
    /*p29.BYBY*/ wire STORE1_CLK  = or(b.spr.STORE_EN, STORE1_SEL);
    /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);
    /*p29.EFEV*/ wire STORE1_RST = or(b.spr.NEW_LINE4, b.spr.SPRITE1_GET_SYNCn);
    /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

    /*p29.BYVY*/ c.spr.STORE1_CLKc = not(STORE1_CLKn);
    /*p30.CAJY*/ c.spr.STORE1_IDX0  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX0,  b.spr.TS_IDX_0 );
    /*p30.CUZA*/ c.spr.STORE1_IDX1  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX1,  b.spr.TS_IDX_1 );
    /*p30.COMA*/ c.spr.STORE1_IDX2  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX2,  b.spr.TS_IDX_2 );
    /*p30.CUFA*/ c.spr.STORE1_IDX3  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX3,  b.spr.TS_IDX_3 );
    /*p30.CEBO*/ c.spr.STORE1_IDX4  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX4,  b.spr.TS_IDX_4 );
    /*p30.CADU*/ c.spr.STORE1_IDX5  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX5,  b.spr.TS_IDX_5 );
                                                                                           
    /*p29.AHOF*/ c.spr.STORE1_CLKb = not(STORE1_CLKn);
    /*p30.ABUG*/ c.spr.STORE1_LINE0 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE0, b.spr.TS_LINE_0);
    /*p30.AMES*/ c.spr.STORE1_LINE1 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE1, b.spr.TS_LINE_1);
    /*p30.ABOP*/ c.spr.STORE1_LINE2 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE2, b.spr.TS_LINE_2);
    /*p30.AROF*/ c.spr.STORE1_LINE3 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE3, b.spr.TS_LINE_3);

    /*p29.ASYS*/ c.spr.STORE1_CLKa = not(STORE1_CLKn);
    /*p31.DANY*/ c.spr.STORE1_X0    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X0,    b.spr.OAM_A_D0b);
    /*p31.DUKO*/ c.spr.STORE1_X1    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X1,    b.spr.OAM_A_D1b);
    /*p31.DESU*/ c.spr.STORE1_X2    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X2,    b.spr.OAM_A_D2b);
    /*p31.DAZO*/ c.spr.STORE1_X3    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X3,    b.spr.OAM_A_D3b);
    /*p31.DAKE*/ c.spr.STORE1_X4    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X4,    b.spr.OAM_A_D4b);
    /*p31.CESO*/ c.spr.STORE1_X5    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X5,    b.spr.OAM_A_D5b);
    /*p31.DYFU*/ c.spr.STORE1_X6    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X6,    b.spr.OAM_A_D6b);
    /*p31.CUSY*/ c.spr.STORE1_X7    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X7,    b.spr.OAM_A_D7b);

    /*p29.DYDO*/ wire SPRITE1_GET = not(SPRITE1_GETn);
    /*p30.BEMO*/ if (SPRITE1_GET) c.spr.TS_IDX_0  = not(!b.spr.STORE1_IDX0);
    /*p30.CYBY*/ if (SPRITE1_GET) c.spr.TS_IDX_1  = not(!b.spr.STORE1_IDX1);
    /*p30.BETY*/ if (SPRITE1_GET) c.spr.TS_IDX_2  = not(!b.spr.STORE1_IDX2);
    /*p30.CEGY*/ if (SPRITE1_GET) c.spr.TS_IDX_3  = not(!b.spr.STORE1_IDX3);
    /*p30.CELU*/ if (SPRITE1_GET) c.spr.TS_IDX_4  = not(!b.spr.STORE1_IDX4);
    /*p30.CUBO*/ if (SPRITE1_GET) c.spr.TS_IDX_5  = not(!b.spr.STORE1_IDX5);
    /*p30.BEFE*/ if (SPRITE1_GET) c.spr.TS_LINE_0 = not(!b.spr.STORE1_LINE0);
    /*p30.BYRO*/ if (SPRITE1_GET) c.spr.TS_LINE_1 = not(!b.spr.STORE1_LINE1);
    /*p30.BACO*/ if (SPRITE1_GET) c.spr.TS_LINE_2 = not(!b.spr.STORE1_LINE2);
    /*p30.AHUM*/ if (SPRITE1_GET) c.spr.TS_LINE_3 = not(!b.spr.STORE1_LINE3);
  }

  //----------
  // store 2

  {
    /*p31.FUZU*/ wire STORE2_MATCH0 = xor(b.spr.STORE2_X0, b.vid.X0n);
    /*p31.FESO*/ wire STORE2_MATCH1 = xor(b.spr.STORE2_X1, b.vid.X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2 = xor(b.spr.STORE2_X2, b.vid.X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3 = xor(b.spr.STORE2_X3, b.vid.X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4 = xor(b.spr.STORE2_X4, b.vid.X4n);
    /*p31.DETY*/ wire STORE2_MATCH5 = xor(b.spr.STORE2_X5, b.vid.X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6 = xor(b.spr.STORE2_X6, b.vid.X6n);
    /*p31.CONY*/ wire STORE2_MATCH7 = xor(b.spr.STORE2_X7, b.vid.X7n);
    /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4, STORE2_MATCH5, STORE2_MATCH6, STORE2_MATCH7);
    /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0, STORE2_MATCH1, STORE2_MATCH2, STORE2_MATCH3);

    /*p29.DEGO*/ c.spr.STORE2_MATCHn = nand(b.spr.MATCH_EN, STORE2_MATCHB, STORE2_MATCHA);
    /*p29.GUPO*/ wire STORE2_MATCH = not(b.spr.STORE2_MATCHn);
    /*p29.GEDE*/ c.spr.STORE2_MATCH_OUT = or(STORE2_MATCH, b.spr.STORE1_MATCH_OUT);

    /*p29.EMOL*/ wire SPRITE2_GETn = nor(c.spr.STORE2_MATCHn, b.spr.STORE1_MATCH_OUT);
    /*p29.EGAV*/ c.spr.SPRITE2_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE2_GET_SYNCn, SPRITE2_GETn);

    /*p29.GEBU*/ wire STORE2_SEL = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n); // 0010
    /*p29.WYXO*/ wire STORE2_CLK = or(b.spr.STORE_EN, STORE2_SEL); // 0010
    /*p29.GYFO*/ wire STORE2_CLKn = not(STORE2_CLK); // 0010
    /*p29.FOKO*/ wire STORE2_RST  = or(b.spr.NEW_LINE4, b.spr.SPRITE2_GET_SYNCn);
    /*p29.GAMY*/ wire STORE2_RSTn = not(STORE2_RST);

    /*p29.CACU*/ c.spr.STORE2_CLKa = not(STORE2_CLKn);
    /*p29.BUZY*/ c.spr.STORE2_CLKb = not(STORE2_CLKn);
    /*p29.FUKE*/ c.spr.STORE2_CLKc = not(STORE2_CLKn);

    /*p30.BOXA*/ c.spr.STORE2_IDX0 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX0, b.spr.TS_IDX_0 );
    /*p30.BUNA*/ c.spr.STORE2_IDX1 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX1, b.spr.TS_IDX_1 );
    /*p30.BULU*/ c.spr.STORE2_IDX2 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX2, b.spr.TS_IDX_2 );
    /*p30.BECA*/ c.spr.STORE2_IDX3 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX3, b.spr.TS_IDX_3 );
    /*p30.BYHU*/ c.spr.STORE2_IDX4 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX4, b.spr.TS_IDX_4 );
    /*p30.BUHE*/ c.spr.STORE2_IDX5 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX5, b.spr.TS_IDX_5 );

    /*p30.YKUK*/ c.spr.STORE2_LINE0 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE0, b.spr.TS_LINE_0);
    /*p30.YLOV*/ c.spr.STORE2_LINE1 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE1, b.spr.TS_LINE_1);
    /*p30.XAZY*/ c.spr.STORE2_LINE2 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE2, b.spr.TS_LINE_2);
    /*p30.XOSY*/ c.spr.STORE2_LINE3 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE3, b.spr.TS_LINE_3);

    /*p31.FOKA*/ c.spr.STORE2_X0 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X0, b.spr.OAM_A_D0b);
    /*p31.FYTY*/ c.spr.STORE2_X1 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X1, b.spr.OAM_A_D1b);
    /*p31.FUBY*/ c.spr.STORE2_X2 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X2, b.spr.OAM_A_D2b);
    /*p31.GOXU*/ c.spr.STORE2_X3 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X3, b.spr.OAM_A_D3b);
    /*p31.DUHY*/ c.spr.STORE2_X4 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X4, b.spr.OAM_A_D4b);
    /*p31.EJUF*/ c.spr.STORE2_X5 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X5, b.spr.OAM_A_D5b);
    /*p31.ENOR*/ c.spr.STORE2_X6 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X6, b.spr.OAM_A_D6b);
    /*p31.DEPY*/ c.spr.STORE2_X7 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X7, b.spr.OAM_A_D7b);


    /*p29.FAME*/ wire SPRITE2_GET = not(SPRITE2_GETn);
    /*p30.AXEC*/ if (SPRITE2_GET) c.spr.TS_IDX_0  = not(!b.spr.STORE2_IDX0);
    /*p30.CYRO*/ if (SPRITE2_GET) c.spr.TS_IDX_1  = not(!b.spr.STORE2_IDX1);
    /*p30.CUVU*/ if (SPRITE2_GET) c.spr.TS_IDX_2  = not(!b.spr.STORE2_IDX2);
    /*p30.APON*/ if (SPRITE2_GET) c.spr.TS_IDX_3  = not(!b.spr.STORE2_IDX3);
    /*p30.AFOZ*/ if (SPRITE2_GET) c.spr.TS_IDX_4  = not(!b.spr.STORE2_IDX4);
    /*p30.CUBE*/ if (SPRITE2_GET) c.spr.TS_IDX_5  = not(!b.spr.STORE2_IDX5);
    /*p30.ZABY*/ if (SPRITE2_GET) c.spr.TS_LINE_0 = not(!b.spr.STORE2_LINE0);
    /*p30.ZUKE*/ if (SPRITE2_GET) c.spr.TS_LINE_1 = not(!b.spr.STORE2_LINE1);
    /*p30.WUXE*/ if (SPRITE2_GET) c.spr.TS_LINE_2 = not(!b.spr.STORE2_LINE2);
    /*p30.WERE*/ if (SPRITE2_GET) c.spr.TS_LINE_3 = not(!b.spr.STORE2_LINE3);
  }

  // store 3
  {
    /*p31.YHOK*/ wire STORE3_MATCH0 = xor(b.spr.XOLY, b.vid.X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1 = xor(b.spr.XYBA, b.vid.X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2 = xor(b.spr.XABE, b.vid.X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3 = xor(b.spr.XEKA, b.vid.X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4 = xor(b.spr.XOMY, b.vid.X4n);
    /*p31.XENY*/ wire STORE3_MATCH5 = xor(b.spr.WUHA, b.vid.X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6 = xor(b.spr.WYNA, b.vid.X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7 = xor(b.spr.WECO, b.vid.X7n);
    /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0, STORE3_MATCH1, STORE3_MATCH2, STORE3_MATCH3);
    /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4, STORE3_MATCH5, STORE3_MATCH6, STORE3_MATCH7);

    /*p29.YLOZ*/ c.spr.STORE3_MATCHn = nand(b.spr.MATCH_EN, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.WEBO*/ wire STORE3_MATCH = not(b.spr.STORE3_MATCHn);
    /*p29.WUTO*/ c.spr.STORE3_MATCH_OUT = or(STORE3_MATCH, b.spr.STORE2_MATCH_OUT);

    /*p29.GYFY*/ wire SPRITE3_GETn = nor(b.spr.STORE3_MATCHn, b.spr.STORE2_MATCH_OUT);
    /*p29.GOTA*/ c.spr.SPRITE3_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE3_GET_SYNCn, SPRITE3_GETn);

    /*p29.GAKE*/ wire STORE3_RST = or(b.spr.NEW_LINE4, b.spr.SPRITE3_GET_SYNCn);
    /*p29.WUPA*/ wire STORE3_RSTn = not(STORE3_RST);

    /*p29.FOCO*/ wire STORE3_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n); // 0011
    /*p29.GUVE*/ wire STORE3_CLK = or(b.spr.STORE_EN, STORE3_SEL); // 0011
    /*p29.GUSA*/ wire STORE3_CLKn = not(STORE3_CLK);
    /*p29.FEKA*/ c.spr.FEKA = not(STORE3_CLKn);
    /*p29.XYHA*/ c.spr.XYHA = not(STORE3_CLKn);
    /*p29.YFAG*/ c.spr.YFAG = not(STORE3_CLKn);
    /*p30.DAFU*/ c.spr.DAFU = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DAFU, b.spr.TS_IDX_0 );
    /*p30.DEBA*/ c.spr.DEBA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEBA, b.spr.TS_IDX_1 );
    /*p30.DUHA*/ c.spr.DUHA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUHA, b.spr.TS_IDX_2 );
    /*p30.DUNY*/ c.spr.DUNY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUNY, b.spr.TS_IDX_3 );
    /*p30.DESE*/ c.spr.DESE = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DESE, b.spr.TS_IDX_4 );
    /*p30.DEVY*/ c.spr.DEVY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEVY, b.spr.TS_IDX_5 );
    /*p30.ZURY*/ c.spr.ZURY = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURY, b.spr.TS_LINE_0);
    /*p30.ZURO*/ c.spr.ZURO = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURO, b.spr.TS_LINE_1);
    /*p30.ZENE*/ c.spr.ZENE = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZENE, b.spr.TS_LINE_2);
    /*p30.ZYLU*/ c.spr.ZYLU = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZYLU, b.spr.TS_LINE_3);
    /*p31.XOLY*/ c.spr.XOLY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOLY, b.spr.OAM_A_D0b);
    /*p31.XYBA*/ c.spr.XYBA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XYBA, b.spr.OAM_A_D1b);
    /*p31.XABE*/ c.spr.XABE = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XABE, b.spr.OAM_A_D2b);
    /*p31.XEKA*/ c.spr.XEKA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XEKA, b.spr.OAM_A_D3b);
    /*p31.XOMY*/ c.spr.XOMY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOMY, b.spr.OAM_A_D4b);
    /*p31.WUHA*/ c.spr.WUHA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WUHA, b.spr.OAM_A_D5b);
    /*p31.WYNA*/ c.spr.WYNA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WYNA, b.spr.OAM_A_D6b);
    /*p31.WECO*/ c.spr.WECO = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WECO, b.spr.OAM_A_D7b);

    /*p29.GYMA*/ wire SPRITE3_GET = not(SPRITE3_GETn);
    /*p30.DEZU*/ if (SPRITE3_GET) c.spr.TS_IDX_0  = not(!b.spr.DAFU);
    /*p30.EFUD*/ if (SPRITE3_GET) c.spr.TS_IDX_1  = not(!b.spr.DEBA);
    /*p30.DONY*/ if (SPRITE3_GET) c.spr.TS_IDX_2  = not(!b.spr.DUHA);
    /*p30.DOWA*/ if (SPRITE3_GET) c.spr.TS_IDX_3  = not(!b.spr.DUNY);
    /*p30.DYGO*/ if (SPRITE3_GET) c.spr.TS_IDX_4  = not(!b.spr.DESE);
    /*p30.ENAP*/ if (SPRITE3_GET) c.spr.TS_IDX_5  = not(!b.spr.DEVY);
    /*p30.ZYPO*/ if (SPRITE3_GET) c.spr.TS_LINE_0 = not(!b.spr.ZURY);
    /*p30.ZEXE*/ if (SPRITE3_GET) c.spr.TS_LINE_1 = not(!b.spr.ZURO);
    /*p30.YJEM*/ if (SPRITE3_GET) c.spr.TS_LINE_2 = not(!b.spr.ZENE);
    /*p30.YWAV*/ if (SPRITE3_GET) c.spr.TS_LINE_3 = not(!b.spr.ZYLU);
  }

  // store 4
  {
    /*p31.ZYKU*/ wire ZYKU = xor(b.spr.YBED, b.vid.X4n);
    /*p31.ZYPU*/ wire ZYPU = xor(b.spr.ZALA, b.vid.X5n);
    /*p31.XAHA*/ wire XAHA = xor(b.spr.WYDE, b.vid.X6n);
    /*p31.ZEFE*/ wire ZEFE = xor(b.spr.XEPA, b.vid.X7n);
    /*p31.XEJU*/ wire XEJU = xor(b.spr.WEDU, b.vid.X0n);
    /*p31.ZATE*/ wire ZATE = xor(b.spr.YGAJ, b.vid.X1n);
    /*p31.ZAKU*/ wire ZAKU = xor(b.spr.ZYJO, b.vid.X2n);
    /*p31.YBOX*/ wire YBOX = xor(b.spr.XURY, b.vid.X3n);
    /*p31.YKOK*/ wire YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
    /*p31.YNAZ*/ wire YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);
    /*p29.XAGE*/ c.spr.STORE4_MATCHn = nand(b.spr.MATCH_EN, YNAZ, YKOK);

    /*p29.XUDY*/ c.spr.SPRITE4_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE4_GET_SYNCn, b.spr.GONO);
    /*p29.WOHU*/ c.spr.WOHU = or(b.spr.NEW_LINE4, b.spr.SPRITE4_GET_SYNCn);
    /*p29.WUNU*/ c.spr.WUNU = not(b.spr.WOHU);
    /*p29.XYLA*/ c.spr.XYLA = or(b.spr.WUNA, b.spr.STORE3_MATCH_OUT);

    /*p29.CUPE*/ c.spr.STORE4_SEL = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2b, b.spr.SPRITE_COUNT3n); // 0100
    /*p29.CECU*/ c.spr.STORE4_CLK = or(b.spr.STORE_EN, b.spr.STORE4_SEL); // 0100
    /*p29.DUKE*/ c.spr.DUKE = not(b.spr.STORE4_CLK);
    /*p29.WOFO*/ c.spr.WOFO = not(b.spr.DUKE);
    /*p29.WYLU*/ c.spr.WYLU = not(b.spr.DUKE);
    /*p29.EWOT*/ c.spr.EWOT = not(b.spr.DUKE);
    /*p31.WEDU*/ c.spr.WEDU = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WEDU, b.spr.OAM_A_D0b);
    /*p31.YGAJ*/ c.spr.YGAJ = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YGAJ, b.spr.OAM_A_D1b);
    /*p31.ZYJO*/ c.spr.ZYJO = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZYJO, b.spr.OAM_A_D2b);
    /*p31.XURY*/ c.spr.XURY = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XURY, b.spr.OAM_A_D3b);
    /*p31.YBED*/ c.spr.YBED = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YBED, b.spr.OAM_A_D4b);
    /*p31.ZALA*/ c.spr.ZALA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZALA, b.spr.OAM_A_D5b);
    /*p31.WYDE*/ c.spr.WYDE = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WYDE, b.spr.OAM_A_D6b);
    /*p31.XEPA*/ c.spr.XEPA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XEPA, b.spr.OAM_A_D7b);
    /*p30.XYNU*/ c.spr.XYNU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XYNU, b.spr.TS_IDX_0 );
    /*p30.XEGE*/ c.spr.XEGE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEGE, b.spr.TS_IDX_1 );
    /*p30.XABO*/ c.spr.XABO = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XABO, b.spr.TS_IDX_2 );
    /*p30.WANU*/ c.spr.WANU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.WANU, b.spr.TS_IDX_3 );
    /*p30.XEFE*/ c.spr.XEFE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEFE, b.spr.TS_IDX_4 );
    /*p30.XAVE*/ c.spr.XAVE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XAVE, b.spr.TS_IDX_5 );
    /*p30.CUMU*/ c.spr.CUMU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CUMU, b.spr.TS_LINE_0);
    /*p30.CAPO*/ c.spr.CAPO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAPO, b.spr.TS_LINE_1);
    /*p30.CONO*/ c.spr.CONO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CONO, b.spr.TS_LINE_2);
    /*p30.CAJU*/ c.spr.CAJU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAJU, b.spr.TS_LINE_3);

    /*p29.GONO*/ c.spr.GONO = nor(b.spr.STORE4_MATCHn, b.spr.STORE3_MATCH_OUT);
    /*p29.WUNA*/ c.spr.WUNA = not(b.spr.STORE4_MATCHn);

    /*p29.GOWO*/ c.spr.GOWO = not(b.spr.GONO);
    /*p30.WAJA*/ if (b.spr.GOWO) c.spr.TS_IDX_0  = not(!b.spr.XYNU);
    /*p30.WOXY*/ if (b.spr.GOWO) c.spr.TS_IDX_1  = not(!b.spr.XEGE);
    /*p30.XYRE*/ if (b.spr.GOWO) c.spr.TS_IDX_2  = not(!b.spr.XABO);
    /*p30.WERU*/ if (b.spr.GOWO) c.spr.TS_IDX_3  = not(!b.spr.WANU);
    /*p30.WEPY*/ if (b.spr.GOWO) c.spr.TS_IDX_4  = not(!b.spr.XEFE);
    /*p30.WUXU*/ if (b.spr.GOWO) c.spr.TS_IDX_5  = not(!b.spr.XAVE);
    /*p30.BYDO*/ if (b.spr.GOWO) c.spr.TS_LINE_0 = not(!b.spr.CUMU);
    /*p30.BUCE*/ if (b.spr.GOWO) c.spr.TS_LINE_1 = not(!b.spr.CAPO);
    /*p30.BOVE*/ if (b.spr.GOWO) c.spr.TS_LINE_2 = not(!b.spr.CONO);
    /*p30.BEVY*/ if (b.spr.GOWO) c.spr.TS_LINE_3 = not(!b.spr.CAJU);
  }

  {
    // store 5
    /*p31.BAZY*/ wire BAZY = xor(b.spr.CYWE, b.vid.X4n);
    /*p31.CYLE*/ wire CYLE = xor(b.spr.DYBY, b.vid.X5n);
    /*p31.CEVA*/ wire CEVA = xor(b.spr.DURY, b.vid.X6n);
    /*p31.BUMY*/ wire BUMY = xor(b.spr.CUVY, b.vid.X7n);
    /*p31.GUZO*/ wire GUZO = xor(b.spr.FUSA, b.vid.X0n);
    /*p31.GOLA*/ wire GOLA = xor(b.spr.FAXA, b.vid.X1n);
    /*p31.GEVE*/ wire GEVE = xor(b.spr.FOZY, b.vid.X2n);
    /*p31.GUDE*/ wire GUDE = xor(b.spr.FESY, b.vid.X3n);
    /*p31.COGY*/ wire COGY = nor(BAZY, CYLE, CEVA, BUMY);
    /*p31.FYMA*/ wire FYMA = nor(GUZO, GOLA, GEVE, GUDE);
    /*p29.EGOM*/ c.spr.STORE5_MATCHn = nand(b.spr.MATCH_EN, FYMA, COGY);

    /*p29.GEGA*/ c.spr.GEGA = nor(b.spr.STORE5_MATCHn, b.spr.XYLA);
    /*p29.WAFY*/ c.spr.SPRITE5_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE5_GET_SYNCn, b.spr.GEGA);
    /*p29.FEVE*/ c.spr.FEVE = or(b.spr.NEW_LINE4, b.spr.SPRITE5_GET_SYNCn);
    /*p29.EJAD*/ c.spr.EJAD = not(b.spr.FEVE);
    /*p29.GABA*/ c.spr.GABA = not(b.spr.STORE5_MATCHn);
    
    /*p29.WEJA*/ c.spr.WEJA = or(b.spr.GABA, b.spr.XYLA);

    /*p29.CUGU*/ c.spr.STORE5_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2b, b.spr.SPRITE_COUNT3n); // 0101
    /*p29.CADO*/ c.spr.STORE5_CLK = or(b.spr.STORE_EN, b.spr.STORE5_SEL); // 0101
    /*p29.BEDE*/ c.spr.BEDE = not(b.spr.STORE5_CLK);
    /*p29.CYLA*/ c.spr.CYLA = not(b.spr.BEDE);
    /*p29.DYMO*/ c.spr.DYMO = not(b.spr.BEDE);
    /*p29.BUCY*/ c.spr.BUCY = not(b.spr.BEDE);
    /*p31.FUSA*/ c.spr.FUSA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FUSA, b.spr.OAM_A_D0b);
    /*p31.FAXA*/ c.spr.FAXA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FAXA, b.spr.OAM_A_D1b);
    /*p31.FOZY*/ c.spr.FOZY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FOZY, b.spr.OAM_A_D2b);
    /*p31.FESY*/ c.spr.FESY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FESY, b.spr.OAM_A_D3b);
    /*p31.CYWE*/ c.spr.CYWE = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CYWE, b.spr.OAM_A_D4b);
    /*p31.DYBY*/ c.spr.DYBY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DYBY, b.spr.OAM_A_D5b);
    /*p31.DURY*/ c.spr.DURY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DURY, b.spr.OAM_A_D6b);
    /*p31.CUVY*/ c.spr.CUVY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CUVY, b.spr.OAM_A_D7b);

    /*p30.EKAP*/ c.spr.EKAP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKAP, b.spr.TS_IDX_0 );
    /*p30.ETAV*/ c.spr.ETAV = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETAV, b.spr.TS_IDX_1 );
    /*p30.EBEX*/ c.spr.EBEX = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EBEX, b.spr.TS_IDX_2 );
    /*p30.GORU*/ c.spr.GORU = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.GORU, b.spr.TS_IDX_3 );
    /*p30.ETYM*/ c.spr.ETYM = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETYM, b.spr.TS_IDX_4 );
    /*p30.EKOP*/ c.spr.EKOP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKOP, b.spr.TS_IDX_5 );
    /*p30.ANED*/ c.spr.ANED = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ANED, b.spr.TS_LINE_0);
    /*p30.ACEP*/ c.spr.ACEP = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ACEP, b.spr.TS_LINE_1);
    /*p30.ABUX*/ c.spr.ABUX = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABUX, b.spr.TS_LINE_2);
    /*p30.ABEG*/ c.spr.ABEG = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABEG, b.spr.TS_LINE_3);

    /*p29.GYGY*/ c.spr.GYGY = not(b.spr.GEGA);
    /*p30.DALO*/ if (b.spr.GYGY) c.spr.TS_IDX_0  = not(!b.spr.EKAP);
    /*p30.DALY*/ if (b.spr.GYGY) c.spr.TS_IDX_1  = not(!b.spr.ETAV);
    /*p30.DUZA*/ if (b.spr.GYGY) c.spr.TS_IDX_2  = not(!b.spr.EBEX);
    /*p30.WAGA*/ if (b.spr.GYGY) c.spr.TS_IDX_3  = not(!b.spr.GORU);
    /*p30.DYNY*/ if (b.spr.GYGY) c.spr.TS_IDX_4  = not(!b.spr.ETYM);
    /*p30.DOBO*/ if (b.spr.GYGY) c.spr.TS_IDX_5  = not(!b.spr.EKOP);
    /*p30.AWAT*/ if (b.spr.GYGY) c.spr.TS_LINE_0 = not(!b.spr.ANED);
    /*p30.BACE*/ if (b.spr.GYGY) c.spr.TS_LINE_1 = not(!b.spr.ACEP);
    /*p30.BODU*/ if (b.spr.GYGY) c.spr.TS_LINE_2 = not(!b.spr.ABUX);
    /*p30.BUJA*/ if (b.spr.GYGY) c.spr.TS_LINE_3 = not(!b.spr.ABEG);

  }

  {
    // store 6
    /*p31.ZARE*/ wire ZARE = xor(b.spr.ZOLY, b.vid.X4n);
    /*p31.ZEMU*/ wire ZEMU = xor(b.spr.ZOGO, b.vid.X5n);
    /*p31.ZYGO*/ wire ZYGO = xor(b.spr.ZECU, b.vid.X6n);
    /*p31.ZUZY*/ wire ZUZY = xor(b.spr.ZESA, b.vid.X7n);
    /*p31.XOSU*/ wire XOSU = xor(b.spr.YCOL, b.vid.X0n);
    /*p31.ZUVU*/ wire ZUVU = xor(b.spr.YRAC, b.vid.X1n);
    /*p31.XUCO*/ wire XUCO = xor(b.spr.YMEM, b.vid.X2n);
    /*p31.ZULO*/ wire ZULO = xor(b.spr.YVAG, b.vid.X3n);
    /*p31.YWAP*/ wire YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
    /*p31.YDOT*/ wire YDOT = nor(XOSU, ZUVU, XUCO, ZULO);
    /*p29.YBEZ*/ c.spr.STORE6_MATCHn = nand(b.spr.MATCH_EN, YDOT, YWAP);

    /*p29.XOJA*/ c.spr.XOJA = nor(b.spr.STORE6_MATCHn, b.spr.WEJA);
    /*p29.WOMY*/ c.spr.SPRITE6_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE6_GET_SYNCn, b.spr.XOJA);
    /*p29.WACY*/ c.spr.WACY = or(b.spr.NEW_LINE4, b.spr.SPRITE6_GET_SYNCn);
    /*p29.XAHO*/ c.spr.XAHO = not(b.spr.WACY);
    /*p29.WASE*/ c.spr.WASE = not(b.spr.STORE6_MATCHn);
    /*p29.WYLA*/ c.spr.WYLA = or(b.spr.WASE, b.spr.WEJA);

    /*p29.WOMU*/ c.spr.STORE6_SEL = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2b, b.spr.SPRITE_COUNT3n); // 0110
    /*p29.XUJO*/ c.spr.STORE6_CLK = or(b.spr.STORE_EN, b.spr.STORE6_SEL); // 0110
    /*p29.WEKA*/ c.spr.WEKA = not(b.spr.STORE6_CLK);
    /*p29.ZAPE*/ c.spr.ZAPE = not(b.spr.WEKA);
    /*p29.WUSE*/ c.spr.WUSE = not(b.spr.WEKA);
    /*p29.ZURU*/ c.spr.ZURU = not(b.spr.WEKA);
    /*p30.GECU*/ c.spr.GECU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GECU, b.spr.TS_IDX_0 );
    /*p30.FOXY*/ c.spr.FOXY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOXY, b.spr.TS_IDX_1 );
    /*p30.GOHU*/ c.spr.GOHU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GOHU, b.spr.TS_IDX_2 );
    /*p30.FOGO*/ c.spr.FOGO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOGO, b.spr.TS_IDX_3 );
    /*p30.GACY*/ c.spr.GACY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GACY, b.spr.TS_IDX_4 );
    /*p30.GABO*/ c.spr.GABO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GABO, b.spr.TS_IDX_5 );
    /*p30.ZUBE*/ c.spr.ZUBE = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUBE, b.spr.TS_LINE_0);
    /*p30.ZUMY*/ c.spr.ZUMY = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUMY, b.spr.TS_LINE_1);
    /*p30.ZEXO*/ c.spr.ZEXO = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZEXO, b.spr.TS_LINE_2);
    /*p30.ZAFU*/ c.spr.ZAFU = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZAFU, b.spr.TS_LINE_3);
    /*p31.YCOL*/ c.spr.YCOL = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YCOL, b.spr.OAM_A_D0b);
    /*p31.YRAC*/ c.spr.YRAC = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YRAC, b.spr.OAM_A_D1b);
    /*p31.YMEM*/ c.spr.YMEM = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YMEM, b.spr.OAM_A_D2b);
    /*p31.YVAG*/ c.spr.YVAG = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YVAG, b.spr.OAM_A_D3b);
    /*p31.ZOLY*/ c.spr.ZOLY = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOLY, b.spr.OAM_A_D4b);
    /*p31.ZOGO*/ c.spr.ZOGO = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOGO, b.spr.OAM_A_D5b);
    /*p31.ZECU*/ c.spr.ZECU = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZECU, b.spr.OAM_A_D6b);
    /*p31.ZESA*/ c.spr.ZESA = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZESA, b.spr.OAM_A_D7b);

    /*p29.XYME*/ c.spr.XYME = not(b.spr.XOJA);
    /*p30.WOCY*/ if (b.spr.XYME) c.spr.TS_IDX_0  = not(!b.spr.GECU);
    /*p30.ELYC*/ if (b.spr.XYME) c.spr.TS_IDX_1  = not(!b.spr.FOXY);
    /*p30.WABO*/ if (b.spr.XYME) c.spr.TS_IDX_2  = not(!b.spr.GOHU);
    /*p30.EZOC*/ if (b.spr.XYME) c.spr.TS_IDX_3  = not(!b.spr.FOGO);
    /*p30.WYWY*/ if (b.spr.XYME) c.spr.TS_IDX_4  = not(!b.spr.GACY);
    /*p30.WATO*/ if (b.spr.XYME) c.spr.TS_IDX_5  = not(!b.spr.GABO);
    /*p30.ZUDO*/ if (b.spr.XYME) c.spr.TS_LINE_0 = not(!b.spr.ZUBE);
    /*p30.YBUK*/ if (b.spr.XYME) c.spr.TS_LINE_1 = not(!b.spr.ZUMY);
    /*p30.ZYTO*/ if (b.spr.XYME) c.spr.TS_LINE_2 = not(!b.spr.ZEXO);
    /*p30.YKOZ*/ if (b.spr.XYME) c.spr.TS_LINE_3 = not(!b.spr.ZAFU);
  }

  {
    // store 7
    /*p31.EJOT*/ wire EJOT = xor(b.spr.FAZU, b.vid.X4n);
    /*p31.ESAJ*/ wire ESAJ = xor(b.spr.FAXE, b.vid.X5n);
    /*p31.DUCU*/ wire DUCU = xor(b.spr.EXUK, b.vid.X6n);
    /*p31.EWUD*/ wire EWUD = xor(b.spr.FEDE, b.vid.X7n);
    /*p31.DUSE*/ wire DUSE = xor(b.spr.ERAZ, b.vid.X0n);
    /*p31.DAGU*/ wire DAGU = xor(b.spr.EPUM, b.vid.X1n);
    /*p31.DYZE*/ wire DYZE = xor(b.spr.EROL, b.vid.X2n);
    /*p31.DESO*/ wire DESO = xor(b.spr.EHYN, b.vid.X3n);
    /*p31.DAJE*/ wire DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
    /*p31.CYCO*/ wire CYCO = nor(DUSE, DAGU, DYZE, DESO);
    /*p29.DYKA*/ c.spr.STORE7_MATCHn = nand(b.spr.MATCH_EN, CYCO, DAJE);

    /*p29.GUTU*/ c.spr.GUTU = nor(b.spr.STORE7_MATCHn, b.spr.WYLA);
    /*p29.WAPO*/ c.spr.SPRITE7_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE7_GET_SYNCn, b.spr.GUTU);
    /*p29.GUKY*/ c.spr.GUKY = or(b.spr.NEW_LINE4, b.spr.SPRITE7_GET_SYNCn);
    /*p29.GAFY*/ c.spr.GAFY = not(b.spr.GUKY);
    /*p29.GYTE*/ c.spr.GYTE = not(b.spr.STORE7_MATCHn);
    /*p29.FAVO*/ c.spr.FAVO = or(b.spr.GYTE, b.spr.WYLA);

    /*p29.GUNA*/ c.spr.STORE7_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2b, b.spr.SPRITE_COUNT3n); // 0111
    /*p29.GAPE*/ c.spr.STORE7_CLK = or(b.spr.STORE_EN, b.spr.STORE7_SEL); // 0111
    /*p29.GYVO*/ c.spr.GYVO = not(b.spr.STORE7_CLK);
    /*p29.FEFO*/ c.spr.FEFO = not(b.spr.GYVO);
    /*p29.GECY*/ c.spr.GECY = not(b.spr.GYVO);
    /*p29.WABE*/ c.spr.WABE = not(b.spr.GYVO);
    /*p30.FUZO*/ c.spr.FUZO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FUZO, b.spr.TS_IDX_0 );
    /*p30.GESY*/ c.spr.GESY = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GESY, b.spr.TS_IDX_1 );
    /*p30.FYSU*/ c.spr.FYSU = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FYSU, b.spr.TS_IDX_2 );
    /*p30.FEFA*/ c.spr.FEFA = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FEFA, b.spr.TS_IDX_3 );
    /*p30.GYNO*/ c.spr.GYNO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GYNO, b.spr.TS_IDX_4 );
    /*p30.GULE*/ c.spr.GULE = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GULE, b.spr.TS_IDX_5 );
    /*p30.XYGO*/ c.spr.XYGO = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYGO, b.spr.TS_LINE_0);
    /*p30.XYNA*/ c.spr.XYNA = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYNA, b.spr.TS_LINE_1);
    /*p30.XAKU*/ c.spr.XAKU = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XAKU, b.spr.TS_LINE_2);
    /*p30.YGUM*/ c.spr.YGUM = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.YGUM, b.spr.TS_LINE_3);
    /*p31.ERAZ*/ c.spr.ERAZ = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.ERAZ, b.spr.OAM_A_D0b);
    /*p31.EPUM*/ c.spr.EPUM = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.EPUM, b.spr.OAM_A_D1b);
    /*p31.EROL*/ c.spr.EROL = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.EROL, b.spr.OAM_A_D2b);
    /*p31.EHYN*/ c.spr.EHYN = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.EHYN, b.spr.OAM_A_D3b);
    /*p31.FAZU*/ c.spr.FAZU = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.FAZU, b.spr.OAM_A_D4b);
    /*p31.FAXE*/ c.spr.FAXE = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.FAXE, b.spr.OAM_A_D5b);
    /*p31.EXUK*/ c.spr.EXUK = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.EXUK, b.spr.OAM_A_D6b);
    /*p31.FEDE*/ c.spr.FEDE = tock_pos(a.spr.GECY, b.spr.GECY, b.spr.GAFY, b.spr.FEDE, b.spr.OAM_A_D7b);

    /*p29.GUGY*/ c.spr.GUGY = not(b.spr.GUTU);
    /*p30.EVYT*/ if (b.spr.GUGY) c.spr.TS_IDX_0  = not(!b.spr.FUZO);
    /*p30.WABA*/ if (b.spr.GUGY) c.spr.TS_IDX_1  = not(!b.spr.GESY);
    /*p30.ETAD*/ if (b.spr.GUGY) c.spr.TS_IDX_2  = not(!b.spr.FYSU);
    /*p30.ELEP*/ if (b.spr.GUGY) c.spr.TS_IDX_3  = not(!b.spr.FEFA);
    /*p30.WYGO*/ if (b.spr.GUGY) c.spr.TS_IDX_4  = not(!b.spr.GYNO);
    /*p30.WAKO*/ if (b.spr.GUGY) c.spr.TS_IDX_5  = not(!b.spr.GULE);
    /*p30.WANA*/ if (b.spr.GUGY) c.spr.TS_LINE_0 = not(!b.spr.XYGO);
    /*p30.WAXE*/ if (b.spr.GUGY) c.spr.TS_LINE_1 = not(!b.spr.XYNA);
    /*p30.WABU*/ if (b.spr.GUGY) c.spr.TS_LINE_2 = not(!b.spr.XAKU);
    /*p30.YPOZ*/ if (b.spr.GUGY) c.spr.TS_LINE_3 = not(!b.spr.YGUM);

  }


  {
    // store 8
    /*p31.DUZE*/ wire DUZE = xor(b.spr.EZUF, b.vid.X4n);
    /*p31.DAGA*/ wire DAGA = xor(b.spr.ENAD, b.vid.X5n);
    /*p31.DAWU*/ wire DAWU = xor(b.spr.EBOW, b.vid.X6n);
    /*p31.EJAW*/ wire EJAW = xor(b.spr.FYCA, b.vid.X7n);
    /*p31.GOHO*/ wire GOHO = xor(b.spr.GAVY, b.vid.X0n);
    /*p31.GASU*/ wire GASU = xor(b.spr.GYPU, b.vid.X1n);
    /*p31.GABU*/ wire GABU = xor(b.spr.GADY, b.vid.X2n);
    /*p31.GAFE*/ wire GAFE = xor(b.spr.GAZA, b.vid.X3n);
    /*p31.DAMA*/ wire DAMA = nor(DUZE, DAGA, DAWU, EJAW);
    /*p31.FEHA*/ wire FEHA = nor(GOHO, GASU, GABU, GAFE);
    /*p29.EFYL*/ c.spr.STORE8_MATCHn = nand(b.spr.MATCH_EN, FEHA, DAMA);
    /*p29.GEKE*/ c.spr.GEKE = not(b.spr.STORE8_MATCHn);
    /*p29.GYGA*/ c.spr.GYGA = or(b.spr.GEKE, b.spr.FAVO);
    /*p29.FOXA*/ c.spr.FOXA = nor(b.spr.STORE8_MATCHn, b.spr.FAVO);

    /*p29.EXUQ*/ c.spr.SPRITE8_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE8_GET_SYNCn, b.spr.FOXA);
    /*p29.GORO*/ c.spr.GORO = or(b.spr.NEW_LINE4, b.spr.SPRITE8_GET_SYNCn);
    /*p29.WUZO*/ c.spr.WUZO = not(b.spr.GORO);

    /*p29.DEWY*/ c.spr.STORE8_SEL = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3b); // 1000
    /*p29.CAHO*/ c.spr.STORE8_CLK = or(b.spr.STORE_EN, b.spr.STORE8_SEL); // 1000
    /*p29.BUKA*/ c.spr.BUKA = not(b.spr.STORE8_CLK);
    /*p29.CEXU*/ c.spr.CEXU = not(b.spr.BUKA);
    /*p29.AKOL*/ c.spr.AKOL = not(b.spr.BUKA);
    /*p29.BYMY*/ c.spr.BYMY = not(b.spr.BUKA);
    /*p31.GAVY*/ c.spr.GAVY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAVY, b.spr.OAM_A_D0b);
    /*p31.GYPU*/ c.spr.GYPU = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GYPU, b.spr.OAM_A_D1b);
    /*p31.GADY*/ c.spr.GADY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GADY, b.spr.OAM_A_D2b);
    /*p31.GAZA*/ c.spr.GAZA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAZA, b.spr.OAM_A_D3b);
    /*p31.EZUF*/ c.spr.EZUF = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EZUF, b.spr.OAM_A_D4b);
    /*p31.ENAD*/ c.spr.ENAD = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.ENAD, b.spr.OAM_A_D5b);
    /*p31.EBOW*/ c.spr.EBOW = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EBOW, b.spr.OAM_A_D6b);
    /*p31.FYCA*/ c.spr.FYCA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.FYCA, b.spr.OAM_A_D7b);


    /*p30.AXUV*/ c.spr.AXUV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.AXUV, b.spr.TS_IDX_0 );
    /*p30.BADA*/ c.spr.BADA = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADA, b.spr.TS_IDX_1 );
    /*p30.APEV*/ c.spr.APEV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.APEV, b.spr.TS_IDX_2 );
    /*p30.BADO*/ c.spr.BADO = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADO, b.spr.TS_IDX_3 );
    /*p30.BEXY*/ c.spr.BEXY = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BEXY, b.spr.TS_IDX_4 );
    /*p30.BYHE*/ c.spr.BYHE = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BYHE, b.spr.TS_IDX_5 );
    /*p30.AFYM*/ c.spr.AFYM = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYM, b.spr.TS_LINE_0);
    /*p30.AZAP*/ c.spr.AZAP = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AZAP, b.spr.TS_LINE_1);
    /*p30.AFUT*/ c.spr.AFUT = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFUT, b.spr.TS_LINE_2);
    /*p30.AFYX*/ c.spr.AFYX = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYX, b.spr.TS_LINE_3);

    /*p29.DENY*/ c.spr.DENY = not(b.spr.FOXA);
    /*p30.ADYB*/ if (b.spr.DENY) c.spr.TS_IDX_0  = b.spr.AXUV;
    /*p30.APOB*/ if (b.spr.DENY) c.spr.TS_IDX_1  = b.spr.BADA;
    /*p30.APYV*/ if (b.spr.DENY) c.spr.TS_IDX_2  = b.spr.APEV;
    /*p30.AFEN*/ if (b.spr.DENY) c.spr.TS_IDX_3  = b.spr.BADO;
    /*p30.AKYH*/ if (b.spr.DENY) c.spr.TS_IDX_4  = b.spr.BEXY;
    /*p30.APOC*/ if (b.spr.DENY) c.spr.TS_IDX_5  = b.spr.BYHE;
    /*p30.BUJY*/ if (b.spr.DENY) c.spr.TS_LINE_0 = b.spr.AFYM;
    /*p30.BOSO*/ if (b.spr.DENY) c.spr.TS_LINE_1 = b.spr.AZAP;
    /*p30.AHAC*/ if (b.spr.DENY) c.spr.TS_LINE_2 = b.spr.AFUT;
    /*p30.BAZU*/ if (b.spr.DENY) c.spr.TS_LINE_3 = b.spr.AFYX;
  }

  {
    // store 9
    /*p31.YMAM*/ wire MATCH0 = xor(b.spr.STORE9_X0, b.vid.X0n);
    /*p31.YTYP*/ wire MATCH1 = xor(b.spr.STORE9_X1, b.vid.X1n);
    /*p31.YFOP*/ wire MATCH2 = xor(b.spr.STORE9_X2, b.vid.X2n);
    /*p31.YVAC*/ wire MATCH3 = xor(b.spr.STORE9_X3, b.vid.X3n);
    /*p31.ZYWU*/ wire MATCH4 = xor(b.spr.STORE9_X4, b.vid.X4n);
    /*p31.ZUZA*/ wire MATCH5 = xor(b.spr.STORE9_X5, b.vid.X5n);
    /*p31.ZEJO*/ wire MATCH6 = xor(b.spr.STORE9_X6, b.vid.X6n);
    /*p31.ZEDA*/ wire MATCH7 = xor(b.spr.STORE9_X7, b.vid.X7n);
    /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
    /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
    /*p29.YGEM*/ c.spr.STORE9_MATCHn = nand(b.spr.MATCH_EN, MATCHA, MATCHB);

    /*p29.DOGU*/ wire STORE9_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3b); // 1001
    /*p29.CATO*/ wire STORE9_CLK = or(b.spr.STORE_EN, STORE9_SEL); // 1001
    /*p29.DECU*/ wire STORE9_CLKn = not(STORE9_CLK);

    /*p29.GUZE*/ wire SPRITE9_GETn = nor(b.spr.STORE9_MATCHn, b.spr.GYGA);
    /*p29.FONO*/ c.spr.SPRITE9_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE9_GET_SYNCn, SPRITE9_GETn);

    /*p29.DUBU*/ wire STORE9_RST  = or(b.spr.NEW_LINE4, b.spr.SPRITE9_GET_SYNCn);
    /*p29.DOSY*/ wire STORE9_RSTn = not(STORE9_RST);
    
    /*p29.WEME*/ c.spr.STORE9_CLKa = not(STORE9_CLKn);
    /*p31.XUVY*/ c.spr.STORE9_X0    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X0,    b.spr.OAM_A_D0b);
    /*p31.XERE*/ c.spr.STORE9_X1    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X1,    b.spr.OAM_A_D1b);
    /*p31.XUZO*/ c.spr.STORE9_X2    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X2,    b.spr.OAM_A_D2b);
    /*p31.XEXA*/ c.spr.STORE9_X3    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X3,    b.spr.OAM_A_D3b);
    /*p31.YPOD*/ c.spr.STORE9_X4    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X4,    b.spr.OAM_A_D4b);
    /*p31.YROP*/ c.spr.STORE9_X5    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X5,    b.spr.OAM_A_D5b);
    /*p31.YNEP*/ c.spr.STORE9_X6    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X6,    b.spr.OAM_A_D6b);
    /*p31.YZOF*/ c.spr.STORE9_X7    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X7,    b.spr.OAM_A_D7b);

    /*p29.WUFA*/ c.spr.STORE9_CLKb = not(STORE9_CLKn);
    /*p30.XUFO*/ c.spr.STORE9_IDX0  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX0,  b.spr.TS_IDX_0 );
    /*p30.XUTE*/ c.spr.STORE9_IDX1  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX1,  b.spr.TS_IDX_1 );
    /*p30.XOTU*/ c.spr.STORE9_IDX2  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX2,  b.spr.TS_IDX_2 );
    /*p30.XYFE*/ c.spr.STORE9_IDX3  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX3,  b.spr.TS_IDX_3 );
    /*p30.YZOR*/ c.spr.STORE9_IDX4  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX4,  b.spr.TS_IDX_4 );
    /*p30.YBER*/ c.spr.STORE9_IDX5  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX5,  b.spr.TS_IDX_5 );

    /*p29.FAKA*/ c.spr.STORE9_CLKc = not(STORE9_CLKn);
    /*p30.DEWU*/ c.spr.STORE9_LINE0 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE0, b.spr.TS_LINE_0);
    /*p30.CANA*/ c.spr.STORE9_LINE1 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE1, b.spr.TS_LINE_1);
    /*p30.DYSY*/ c.spr.STORE9_LINE2 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE2, b.spr.TS_LINE_2);
    /*p30.FOFO*/ c.spr.STORE9_LINE3 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE3, b.spr.TS_LINE_3);

    /*p29.FADO*/ wire SPRITE9_GET = not(SPRITE9_GETn);
    /*p30.YHAL*/ if (SPRITE9_GET) c.spr.TS_IDX_0  = not(!b.spr.STORE9_IDX0);
    /*p30.YRAD*/ if (SPRITE9_GET) c.spr.TS_IDX_1  = not(!b.spr.STORE9_IDX1);
    /*p30.XYRA*/ if (SPRITE9_GET) c.spr.TS_IDX_2  = not(!b.spr.STORE9_IDX2);
    /*p30.YNEV*/ if (SPRITE9_GET) c.spr.TS_IDX_3  = not(!b.spr.STORE9_IDX3);
    /*p30.ZOJY*/ if (SPRITE9_GET) c.spr.TS_IDX_4  = not(!b.spr.STORE9_IDX4);
    /*p30.ZARO*/ if (SPRITE9_GET) c.spr.TS_IDX_5  = not(!b.spr.STORE9_IDX5);
    /*p30.CAWO*/ if (SPRITE9_GET) c.spr.TS_LINE_0 = not(!b.spr.STORE9_LINE0);
    /*p30.BYME*/ if (SPRITE9_GET) c.spr.TS_LINE_1 = not(!b.spr.STORE9_LINE1);
    /*p30.COHO*/ if (SPRITE9_GET) c.spr.TS_LINE_2 = not(!b.spr.STORE9_LINE2);
    /*p30.GATE*/ if (SPRITE9_GET) c.spr.TS_LINE_3 = not(!b.spr.STORE9_LINE3);

  }


  


  {
    /*p29.FEFY*/ wire FEFY = nand(b.spr.STORE4_MATCHn, b.spr.STORE3_MATCHn, c.spr.STORE2_MATCHn, b.spr.STORE1_MATCHn, b.spr.STORE0_MATCHn);
    /*p29.FOVE*/ wire FOVE = nand(b.spr.STORE9_MATCHn, b.spr.STORE8_MATCHn, b.spr.STORE7_MATCHn, b.spr.STORE6_MATCHn, b.spr.STORE5_MATCHn);
    /*p29.FEPO*/ c.spr.STORE_MATCH = or(FEFY, FOVE);
  }


  //----------

}

//-----------------------------------------------------------------------------

};