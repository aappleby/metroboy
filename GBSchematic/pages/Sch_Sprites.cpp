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
    /*p29.WOJO*/ wire WOJO = nor(!b.vid.CLK_AxxDExxHa, !b.spr.CLK_WOSU);
    /*p29.XYSO*/ wire XYSO = not(WOJO);
    /*p25.AVER*/ wire AVER = and(b.spr.OAM_ADDR_PARSEn, XYSO);
    /*p29.TUVO*/ wire TUVO = or(b.spr.RENDERINGn, b.spr.SEQ1, b.spr.SPRITE_ABn);
    /*p25.VAPE*/ wire VAPE = and(b.spr.SEQ_5_TRIG, TUVO);
    /*p25.XUJY*/ wire XUJY = not(VAPE);
    /*p25.CUFE*/ wire OAM_WR = or(and(b.sys.ADDR_OAM, b.sys.DMA_RUNNING_SYNCn), b.sys.PHASE_xxxxEFGH8b);
    /*p25.BYCU*/ wire OAM_IN_CLK = nor(OAM_WR, XUJY, AVER);
    /*p25.COTA*/ c.vid.OAM_IN_CLKn = not(OAM_IN_CLK); // -> OAM

  }


  // does this mean that cpu reading oam during render blocks ppu oam read?
  /*
  if (OAM_ADDR_PARSE) { OAM_LATCH = 1; return; }
  if (!FROM_CPU5)     { OAM_LATCH = 1; return; }
  if (ADDR_OAM)       { OAM_LATCH = 1; return; }
  if (CPU_RD2)        { OAM_LATCH = 1; return; }

  if (SEQ_5_SYNC)      OAM_LATCH = not(CLK_XOCE);
  else if (RENDERINGn) OAM_LATCH = 1;
  else                 OAM_LATCH = nand(CLK_XOCE, SEQ1n, SPRITE_AB);
  */

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

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNCn, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);

    /*p28.WAFO*/ wire OAM_A0n   = not(b.spr.OAM_A0);
    /*p04.NAXY*/ wire NAXY      = nor(b.sys.FROM_CPU5_SYNC, b.sys.LUVY);
    /*p04.POWU*/ wire POWU      = and(b.sys.DMA_RUNNING_SYNCn, NAXY);
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
    /*p21.XYVO*/ wire LINE_144 = and(b.vid.V4, b.vid.V7);
    /*p29.ALES*/ wire LINE_144n = not(LINE_144);
    /*p29.ABOV*/ wire IN_LINEb = and(b.vid.LINE_DONEo, LINE_144n);
    /*p29.CATU*/ c.spr.IN_LINE  = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, VID_RESETn3, b.spr.IN_LINE, IN_LINEb);
    /*p28.ANEL*/ c.spr.IN_LINE_SYNC = tock_pos(a.spr.CLK_2Mn, b.spr.CLK_2Mn, VID_RESETn3, b.spr.IN_LINE_SYNC, b.spr.IN_LINE);
  }

  {

    /*p28.ABAF*/ wire IN_LINEn = not(b.spr.IN_LINE);
    /*p28.BYHA*/ wire NEW_LINEn = or(b.spr.IN_LINE_SYNC, IN_LINEn, b.sys.VID_RESETn3);
    /*p28.ATEJ*/ c.spr.NEW_LINE = not(NEW_LINEn); // this drives sprite count reset, it must be new line.

    /*p28.ABAK*/ c.spr.NEW_LINE2 = or(b.spr.NEW_LINE, b.sys.VID_RESET7);
    /*p28.BYVA*/ c.spr.NEW_LINE3 = not(b.spr.NEW_LINE2);
    /*p29.DYBA*/ c.spr.NEW_LINE4 = not(b.spr.NEW_LINE3);
  }



  /*p29.BUZA*/ c.spr.STORE_SPRITE_IDX = and(!b.spr.CENO, b.vid.RENDERING);
  /*p29.CEHA*/ c.spr.CEHA = not(!b.spr.CENO);
  
  /*p29.CENO*/ c.spr.CENO = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, b.sys.VID_RESETn3, b.spr.CENO, b.spr.BESU);
  /*p28.BESU*/   c.spr.BESU = or(b.spr.IN_LINE, b.spr.ASEN);
  /*p28.ASEN*/     c.spr.ASEN = or(b.sys.VID_RESET6, b.spr.SCAN_DONE_TRIG);



  {
    /*p28.ANOM*/ wire SCAN_RSTn = nor(b.sys.VID_RESET6, b.spr.NEW_LINE);
    /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
    /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);

    /*p28.FETO*/ wire SCAN_DONE = and(b.spr.SCAN0, b.spr.SCAN1, b.spr.SCAN2, b.spr.SCAN5);

    /*p28.GAVA*/ c.spr.SCAN_CLK  = or(SCAN_DONE, b.vid.CLK_AxxDExxHb);
    /*p28.YFEL*/ c.spr.SCAN0 = tock_pos( a.spr.SCAN_CLK, b.spr.SCAN_CLK, SCAN_RSTn, b.spr.SCAN0, !b.spr.SCAN0);
    /*p28.WEWY*/ c.spr.SCAN1 = tock_pos(!a.spr.SCAN0,    !b.spr.SCAN0,   SCAN_RSTn, b.spr.SCAN1, !b.spr.SCAN1);
    /*p28.GOSO*/ c.spr.SCAN2 = tock_pos(!a.spr.SCAN1,    !b.spr.SCAN1,   SCAN_RSTn, b.spr.SCAN2, !b.spr.SCAN2);
    /*p28.ELYN*/ c.spr.SCAN3 = tock_pos(!a.spr.SCAN2,    !b.spr.SCAN2,   SCAN_RSTn, b.spr.SCAN3, !b.spr.SCAN3);
    /*p28.FAHA*/ c.spr.SCAN4 = tock_pos(!a.spr.SCAN3,    !b.spr.SCAN3,   SCAN_RSTn, b.spr.SCAN4, !b.spr.SCAN4);
    /*p28.FONY*/ c.spr.SCAN5 = tock_pos(!a.spr.SCAN4,    !b.spr.SCAN4,   SCAN_RSTn, b.spr.SCAN5, !b.spr.SCAN5);

    /*p29.BYBA*/ c.spr.SCAN_DONE_SYNC1 = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, SCAN_RSTo, b.spr.SCAN_DONE_SYNC1, SCAN_DONE);
    /*p29.DOBA*/ c.spr.SCAN_DONE_SYNC2 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, SCAN_RSTo, b.spr.SCAN_DONE_SYNC2, b.spr.SCAN_DONE_SYNC1);

    /*p29.BEBU*/ wire SCAN_DONE_TRIGn = or(SCAN_RSTa, b.spr.SCAN_DONE_SYNC2, !b.spr.SCAN_DONE_SYNC1);
    /*p29.AVAP*/ c.spr.SCAN_DONE_TRIG = not(SCAN_DONE_TRIGn);
  }

  
  //----------
  // undeciphered

  /*p29.TEPA*/ c.spr.RENDERINGn = not(b.vid.RENDERING);


  /*p29.XEFY*/ c.spr.XEFY_CLK = not(b.spr.WUTY_CLK);

  /*p29.SAKY*/     c.spr.SAKY = nor(b.spr.SEQ1, b.spr.SPRITE_ABn);
  /*p29.TYSO*/   c.spr.SPRITE_READb = or(b.spr.SAKY, b.spr.RENDERINGn); // seems wrong
  /*p29.TEXY*/ c.spr.SPRITE_READn = not(b.spr.SPRITE_READb);





  {
    /*p29.TAME*/ wire SEQ_5n = nand(b.spr.SEQ2, b.spr.SEQ0);
    /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);

    /*p29.TYFO*/ c.spr.SEQ_5_SYNCn = tock_pos(a.vid.CLK_xBxDxFxHd,  b.vid.CLK_xBxDxFxHd, b.vid.P10_Bn, b.spr.SEQ_5_SYNCn, SEQ_5n);
    /*p29.TACU*/ c.spr.SEQ_5_TRIG = nand(SEQ_5, b.spr.SEQ_5_SYNCn);
    /*p29.SYCU*/ c.spr.SYCU = nor(SEQ_5, b.spr.RENDERINGn, b.spr.SEQ_5_SYNCn);
    
    /*p27.RYCE*/ wire TEKY_TRIG = and(b.vid.TEKY_SYNC1, !b.vid.TEKY_SYNC2);
    /*p27.SECA*/ wire SECA_RST = nor(TEKY_TRIG, b.sys.VID_RESET5, b.spr.NEW_LINE);

    /*p29.TOMA*/ c.spr.TOMA_CLK = nand(b.vid.CLK_xBxDxFxHd, SEQ_5n);
    /*p29.TOXE*/ c.spr.SEQ0     = tock_pos( a.spr.TOMA_CLK, b.spr.TOMA_CLK, SECA_RST, b.spr.SEQ0, !b.spr.SEQ0);
    /*p29.TULY*/ c.spr.SEQ1     = tock_pos(!a.spr.SEQ0,     !b.spr.SEQ0,    SECA_RST, b.spr.SEQ1, !b.spr.SEQ1);
    /*p29.TESE*/ c.spr.SEQ2     = tock_pos(!a.spr.SEQ1,     !b.spr.SEQ1,    SECA_RST, b.spr.SEQ2, !b.spr.SEQ2);

    /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(b.spr.SEQ1, b.spr.SYCU);
    /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
    /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
    /*p29.XADO*/ c.spr.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);

    /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(b.spr.SPRITE_ABn, b.spr.SYCU);
    /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
    /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
    /*p29.PUCO*/ c.spr.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);

    /*p25.SOHO*/ c.vid.SOHO = and(b.spr.SEQ_5_TRIG, b.spr.SPRITE_READn);
    /*p25.RAWA*/ c.vid.RAWA = not(b.vid.SOHO);
  }




  {
    // tags wrong somewhere

    /*p01.ATAL*/ wire ATAL_AxCxExGx = not(b.sys.ROOTCLK_xBxDxFxH3);
    /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
    /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
    /*p01.ALET*/ wire ALET_AxCxExGx = not(ZEME_xBxDxFxH);

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

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNCn, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(b.sys.ADDR_OAM, OAM_BUSYn);

    /*p28.ADAH*/ wire ADDR_OAMn = not(b.sys.ADDR_OAM);
    /*p28.XUTO*/ wire CPU_OAM_WR = and(b.sys.ADDR_OAM, b.sys.CPU_WR2);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(b.sys.PHASE_xxxxEFGH8b);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);

    /*p28.AZAR*/ wire VRAM_TO_OAMc = not(b.sys.VRAM_TO_OAMn);

    /*p25.CEDE*/ wire DO_DMAn = not(b.sys.DO_DMA);

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


    /*p28.WUZU*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D0 = b.MD0;
    /*p28.AXER*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D1 = b.MD1;
    /*p28.ASOX*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D2 = b.MD2;
    /*p28.CETU*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D3 = b.MD3;
    /*p28.ARYN*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D4 = b.MD4;
    /*p28.ACOT*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D5 = b.MD5;
    /*p28.CUJE*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D6 = b.MD6;
    /*p28.ATER*/ if (VRAM_TO_OAMc) c.TS_OAM_A_D7 = b.MD7;
    /*p28.WOWA*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D0 = b.MD0;
    /*p28.AVEB*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D1 = b.MD1;
    /*p28.AMUH*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D2 = b.MD2;
    /*p28.COFO*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D3 = b.MD3;
    /*p28.AZOZ*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D4 = b.MD4;
    /*p28.AGYK*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D5 = b.MD5;
    /*p28.BUSE*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D6 = b.MD6;
    /*p28.ANUM*/ if (VRAM_TO_OAMc) c.TS_OAM_B_D7 = b.MD7;

    // DO_DMAn polarity probably wrong
    /*p25.WEJO*/ if (DO_DMAn) c.TS_OAM_A_D0 = not(/*p25.RALO*/ not(chip_in.D0_C));
    /*p25.BUBO*/ if (DO_DMAn) c.TS_OAM_A_D1 = not(/*p25.TUNE*/ not(chip_in.D1_C));
    /*p25.BETU*/ if (DO_DMAn) c.TS_OAM_A_D2 = not(/*p25.SERA*/ not(chip_in.D2_C));
    /*p25.CYME*/ if (DO_DMAn) c.TS_OAM_A_D3 = not(/*p25.TENU*/ not(chip_in.D3_C));
    /*p25.BAXU*/ if (DO_DMAn) c.TS_OAM_A_D4 = not(/*p25.SYSA*/ not(chip_in.D4_C));
    /*p25.BUHU*/ if (DO_DMAn) c.TS_OAM_A_D5 = not(/*p25.SUGY*/ not(chip_in.D5_C));
    /*p25.BYNY*/ if (DO_DMAn) c.TS_OAM_A_D6 = not(/*p25.TUBE*/ not(chip_in.D6_C));
    /*p25.BYPY*/ if (DO_DMAn) c.TS_OAM_A_D7 = not(/*p25.SYZO*/ not(chip_in.D7_C));
    /*p25.WASA*/ if (DO_DMAn) c.TS_OAM_B_D0 = not(/*p25.RALO*/ not(chip_in.D0_C));
    /*p25.BOMO*/ if (DO_DMAn) c.TS_OAM_B_D1 = not(/*p25.TUNE*/ not(chip_in.D1_C));
    /*p25.BASA*/ if (DO_DMAn) c.TS_OAM_B_D2 = not(/*p25.SERA*/ not(chip_in.D2_C));
    /*p25.CAKO*/ if (DO_DMAn) c.TS_OAM_B_D3 = not(/*p25.TENU*/ not(chip_in.D3_C));
    /*p25.BUMA*/ if (DO_DMAn) c.TS_OAM_B_D4 = not(/*p25.SYSA*/ not(chip_in.D4_C));
    /*p25.BUPY*/ if (DO_DMAn) c.TS_OAM_B_D5 = not(/*p25.SUGY*/ not(chip_in.D5_C));
    /*p25.BASY*/ if (DO_DMAn) c.TS_OAM_B_D6 = not(/*p25.TUBE*/ not(chip_in.D6_C));
    /*p25.BAPE*/ if (DO_DMAn) c.TS_OAM_B_D7 = not(/*p25.SYZO*/ not(chip_in.D7_C));
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

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNCn, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
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
    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(b.sys.DMA_RUNNING_SYNCn, b.spr.OAM_ADDR_PARSEn, OAM_ADDR_RENDERn);
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
    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.ASEN*/ wire ASEN = or(b.sys.VID_RESET6, b.spr.SCAN_DONE_TRIG);
    /*p28.BESU*/ wire BESU = or(b.spr.IN_LINE, ASEN);
    /*p28.ACYL*/ c.spr.OAM_ADDR_PARSEn  = and(DMA_RUNNING_SYNCb, BESU);
  }

  {
  }

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

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(b.sys.DMA_RUNNING_SYNCn);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, b.vid.RENDERING);

    /*p28.ASAM*/ wire OAM_ADDR_CPU    = or(b.spr.OAM_ADDR_PARSEn , b.vid.RENDERING, b.sys.DMA_RUNNING_SYNCn); // this is definitely or, which seems wrong
    /*p28.BETE*/ wire OAM_ADDR_RENDER = not(OAM_ADDR_RENDERn);
    /*p28.APAR*/ wire OAM_ADDR_PARSE  = not(b.spr.OAM_ADDR_PARSEn);
    /*p04.DUGA*/ wire OAM_ADDR_DMA    = not(b.sys.DMA_RUNNING_SYNCn);

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



  /*p29.GEDE*/ c.spr.GEDE = or(b.spr.GUPO, b.spr.STORE1_MATCH_OUT);

  /*p29.WEBO*/ c.spr.WEBO = not(b.spr.YLOZ);
  /*p29.WUTO*/ c.spr.WUTO = or(b.spr.WEBO, b.spr.GEDE);
  /*p29.XYLA*/ c.spr.XYLA = or(b.spr.WUNA, b.spr.WUTO);
  /*p29.GABA*/ c.spr.GABA = not(b.spr.EGOM);
  /*p29.WEJA*/ c.spr.WEJA = or(b.spr.GABA, b.spr.XYLA);
  /*p29.WASE*/ c.spr.WASE = not(b.spr.YBEZ);
  /*p29.WYLA*/ c.spr.WYLA = or(b.spr.WASE, b.spr.WEJA);
  /*p29.GYTE*/ c.spr.GYTE = not(b.spr.DYKA);
  /*p29.FAVO*/ c.spr.FAVO = or(b.spr.GYTE, b.spr.WYLA);

  
  /*p29.GUZE*/   c.spr.GUZE = nor(b.spr.YGEM, b.spr.GYGA);
  /*p29.FADO*/ c.spr.FADO = not(b.spr.GUZE);

  /*p29.GUTU*/ c.spr.GUTU = nor(b.spr.DYKA, b.spr.WYLA);
  /*p29.XOJA*/ c.spr.XOJA = nor(b.spr.YBEZ, b.spr.WEJA);
  /*p29.GEGA*/ c.spr.GEGA = nor(b.spr.EGOM, b.spr.XYLA);
  /*p29.GYFY*/ c.spr.GYFY = nor(b.spr.YLOZ, b.spr.GEDE);


  /*p29.GUGY*/ c.spr.GUGY = not(b.spr.GUTU);
  /*p29.XYME*/ c.spr.XYME = not(b.spr.XOJA);
  /*p29.GYGY*/ c.spr.GYGY = not(b.spr.GEGA);
  /*p29.GOWO*/ c.spr.GOWO = not(b.spr.GONO);
  /*p29.GYMA*/ c.spr.GYMA = not(b.spr.GYFY);
  /*p29.FAME*/ c.spr.FAME = not(b.spr.EMOL);


  {
    /*p29.FEFY*/ wire FEFY = nand(b.spr.XAGE, b.spr.YLOZ, b.spr.DEGO, b.spr.STORE1_MATCHn, b.spr.STORE0_MATCHn);
    /*p29.FOVE*/ wire FOVE = nand(b.spr.YGEM, b.spr.EFYL, b.spr.DYKA, b.spr.YBEZ, b.spr.EGOM);
    /*p29.FEPO*/ c.spr.STORE_MATCH = or(FEFY, FOVE);
  }

  /*p29.FONO*/ c.spr.SPRITE9_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE9_GET_SYNCn, b.spr.GUZE);
  /*p29.EXUQ*/ c.spr.SPRITE8_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE8_GET_SYNCn, b.spr.FOXA);
  /*p29.WAPO*/ c.spr.SPRITE7_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE7_GET_SYNCn, b.spr.GUTU);
  /*p29.WOMY*/ c.spr.SPRITE6_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE6_GET_SYNCn, b.spr.XOJA);
  /*p29.WAFY*/ c.spr.SPRITE5_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE5_GET_SYNCn, b.spr.GEGA);
  /*p29.XUDY*/ c.spr.SPRITE4_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE4_GET_SYNCn, b.spr.GONO);
  /*p29.GOTA*/ c.spr.SPRITE3_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE3_GET_SYNCn, b.spr.GYFY);
  /*p29.EGAV*/ c.spr.SPRITE2_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE2_GET_SYNCn, b.spr.EMOL);

  /*p29.DUBU*/ c.spr.DUBU = or(b.spr.NEW_LINE4, b.spr.SPRITE9_GET_SYNCn);
  /*p29.GORO*/ c.spr.GORO = or(b.spr.NEW_LINE4, b.spr.SPRITE8_GET_SYNCn);
  /*p29.GUKY*/ c.spr.GUKY = or(b.spr.NEW_LINE4, b.spr.SPRITE7_GET_SYNCn);
  /*p29.WACY*/ c.spr.WACY = or(b.spr.NEW_LINE4, b.spr.SPRITE6_GET_SYNCn);
  /*p29.FEVE*/ c.spr.FEVE = or(b.spr.NEW_LINE4, b.spr.SPRITE5_GET_SYNCn);
  /*p29.WOHU*/ c.spr.WOHU = or(b.spr.NEW_LINE4, b.spr.SPRITE4_GET_SYNCn);
  /*p29.GAKE*/ c.spr.GAKE = or(b.spr.NEW_LINE4, b.spr.SPRITE3_GET_SYNCn);
  /*p29.FOKO*/ c.spr.FOKO = or(b.spr.NEW_LINE4, b.spr.SPRITE2_GET_SYNCn);

  /*p29.DOSY*/ c.spr.DOSY = not(b.spr.DUBU);
  /*p29.WUZO*/ c.spr.WUZO = not(b.spr.GORO);
  /*p29.GAFY*/ c.spr.GAFY = not(b.spr.GUKY);
  /*p29.XAHO*/ c.spr.XAHO = not(b.spr.WACY);
  /*p29.EJAD*/ c.spr.EJAD = not(b.spr.FEVE);
  /*p29.WUNU*/ c.spr.WUNU = not(b.spr.WOHU);
  /*p29.WUPA*/ c.spr.WUPA = not(b.spr.GAKE);
  /*p29.GAMY*/ c.spr.GAMY = not(b.spr.FOKO);

  //----------
  // Sprite tile fetch address generation


  /*p29.WUKY*/ c.spr.FLIP_Y = not(b.spr.OAM_A_D6);

  /*p29.XUQU*/ c.spr.SPRITE_AB = not(!b.spr.SPRITE_ABn);
  /*p29.CYVU*/ c.spr.SPRITE_Y0 = xor(b.spr.FLIP_Y, b.spr.TS_LINE_1);
  /*p29.BORE*/ c.spr.SPRITE_Y1 = xor(b.spr.FLIP_Y, b.spr.TS_LINE_2);
  /*p29.BUVY*/ c.spr.SPRITE_Y2 = xor(b.spr.FLIP_Y, b.spr.TS_LINE_3);


  /*p29.FUFO*/ c.spr.LCDC_SPSIZEn = not(b.vid.LCDC_SPSIZE);
  /*p29.WAGO*/ c.spr.WAGO = xor(b.spr.FLIP_Y, b.spr.TS_LINE_0);
  /*p29.GEJY*/ c.spr.GEJY = amux2(b.spr.LCDC_SPSIZEn, !b.spr.OAM_B_D0, // polarity?
                                  b.vid.LCDC_SPSIZE,  b.spr.WAGO);

  //----------
  // Active sprite counter

  /*p29.BESE*/ c.spr.SPRITE_COUNT0 = tock_pos( a.spr.SPRITE_COUNT_CLK,  b.spr.SPRITE_COUNT_CLK, b.spr.SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT0, !b.spr.SPRITE_COUNT0);
  /*p29.CUXY*/ c.spr.SPRITE_COUNT1 = tock_pos(!a.spr.SPRITE_COUNT0,    !b.spr.SPRITE_COUNT0,    b.spr.SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT1, !b.spr.SPRITE_COUNT1);
  /*p29.BEGO*/ c.spr.SPRITE_COUNT2 = tock_pos(!a.spr.SPRITE_COUNT1,    !b.spr.SPRITE_COUNT1,    b.spr.SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT2, !b.spr.SPRITE_COUNT2);
  /*p29.DYBE*/ c.spr.SPRITE_COUNT3 = tock_pos(!a.spr.SPRITE_COUNT2,    !b.spr.SPRITE_COUNT2,    b.spr.SPRITE_COUNT_RSTn, b.spr.SPRITE_COUNT3, !b.spr.SPRITE_COUNT3);
  /*p29.CAKE*/   c.spr.SPRITE_COUNT_CLK = or(b.spr.SPRITES_FULL, b.spr.STORE_EN_SYNC);
  /*p29.BAKY*/     c.spr.SPRITES_FULL = and(b.spr.SPRITE_COUNT1, b.spr.SPRITE_COUNT3);
  /*p29.DEZY*/     c.spr.STORE_EN_SYNC = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.VID_RESETn1, b.spr.STORE_EN_SYNC, b.spr.STORE_EN);
  /*p28.AZYB*/   c.spr.SPRITE_COUNT_RSTn = not(b.spr.NEW_LINE);

  // 4 to 10 decoder
  /*p29.EDEN*/ c.spr.SPRITE_COUNT0n = not(b.spr.SPRITE_COUNT0);
  /*p29.FYCU*/ c.spr.SPRITE_COUNT0b = not(b.spr.SPRITE_COUNT0n);
  /*p29.CYPY*/ c.spr.SPRITE_COUNT1n = not(b.spr.SPRITE_COUNT1);
  /*p29.FONE*/ c.spr.SPRITE_COUNT1b = not(b.spr.SPRITE_COUNT1n);
  /*p29.CAPE*/ c.spr.SPRITE_COUNT2n = not(b.spr.SPRITE_COUNT2);
  /*p29.EKUD*/ c.spr.SPRITE_COUNT2b = not(b.spr.SPRITE_COUNT2n);
  /*p29.CAXU*/ c.spr.SPRITE_COUNT3n = not(b.spr.SPRITE_COUNT3);
  /*p29.ELYG*/ c.spr.SPRITE_COUNT3b = not(b.spr.SPRITE_COUNT3b);

  //----------

  /*p30.XADU*/ c.spr.IDX_0 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_0, b.spr.OAM_A2);
  /*p30.XEDY*/ c.spr.IDX_1 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_1, b.spr.OAM_A3);
  /*p30.ZUZE*/ c.spr.IDX_2 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_2, b.spr.OAM_A4);
  /*p30.XOBE*/ c.spr.IDX_3 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_3, b.spr.OAM_A5);
  /*p30.YDUF*/ c.spr.IDX_4 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_4, b.spr.OAM_A6);
  /*p30.XECU*/ c.spr.IDX_5 = tock_pos(a.spr.WUDA, b.spr.WUDA, b.spr.P10_Bn, b.spr.IDX_5, b.spr.OAM_A7);
  /*p30.WUDA*/   c.spr.WUDA = not(b.spr.CYKE);
  /*p30.CYKE*/     c.spr.CYKE = not(b.vid.CLK_AxxDExxHb);

  // so this is a bit weird as we have a tri-state bus inside one schematic...

  /*p30.WEZA*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_0 = b.spr.IDX_5; // order wrong
  /*p30.WUCO*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_1 = b.spr.IDX_4;
  /*p30.WYDA*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_2 = b.spr.IDX_3;
  /*p30.ZYSU*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_3 = b.spr.IDX_2;
  /*p30.WYSE*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_4 = b.spr.IDX_1;
  /*p30.WUZY*/ if (b.spr.STORE_SPRITE_IDX) c.spr.TS_IDX_5 = b.spr.IDX_0;

  /*p31.ZAGO*/ c.spr.OAM_A_D0b = not(!b.spr.OAM_A_D0);
  /*p31.ZOCY*/ c.spr.OAM_A_D1b = not(!b.spr.OAM_A_D1);
  /*p31.YPUR*/ c.spr.OAM_A_D2b = not(!b.spr.OAM_A_D2);
  /*p31.YVOK*/ c.spr.OAM_A_D3b = not(!b.spr.OAM_A_D3);
  /*p31.COSE*/ c.spr.OAM_A_D4b = not(!b.spr.OAM_A_D4);
  /*p31.AROP*/ c.spr.OAM_A_D5b = not(!b.spr.OAM_A_D5);
  /*p31.XATU*/ c.spr.OAM_A_D6b = not(!b.spr.OAM_A_D6);
  /*p31.BADY*/ c.spr.OAM_A_D7b = not(!b.spr.OAM_A_D7);

  //----------
  // store 0

  {
    /*p31.ZOGY*/     wire STORE0_MATCH0 = xor(b.spr.STORE0_X0, b.vid.X0n);
    /*p31.ZEBA*/     wire STORE0_MATCH1 = xor(b.spr.STORE0_X1, b.vid.X1n);
    /*p31.ZAHA*/     wire STORE0_MATCH2 = xor(b.spr.STORE0_X2, b.vid.X2n);
    /*p31.ZOKY*/     wire STORE0_MATCH3 = xor(b.spr.STORE0_X3, b.vid.X3n);
    /*p31.WOJU*/     wire STORE0_MATCH4 = xor(b.spr.STORE0_X4, b.vid.X4n);
    /*p31.YFUN*/     wire STORE0_MATCH5 = xor(b.spr.STORE0_X5, b.vid.X5n);
    /*p31.WYZA*/     wire STORE0_MATCH6 = xor(b.spr.STORE0_X6, b.vid.X6n);
    /*p31.YPUK*/     wire STORE0_MATCH7 = xor(b.spr.STORE0_X7, b.vid.X7n);
    /*p31.ZAKO*/   wire STORE0_MATCHA = nor(STORE0_MATCH0, STORE0_MATCH1, STORE0_MATCH2, STORE0_MATCH3);
    /*p31.XEBA*/   wire STORE0_MATCHB = nor(STORE0_MATCH4, STORE0_MATCH5, STORE0_MATCH6, STORE0_MATCH7);
    /*p29.YDUG*/ c.spr.STORE0_MATCHn = nand(b.spr.MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);


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

    // this chains to the next matcher
    /*p29.GEZE*/ c.spr.STORE0_MATCH_OUT = or (/*p29.WEFU*/ not(b.spr.STORE0_MATCHn), chip_in.P10_B);
  }

  //----------
  // store 1

  {
    /*p31.EDYM*/     c.spr.STORE1_MATCH0 = xor(b.spr.STORE1_X0, b.vid.X0n);
    /*p31.EMYB*/     c.spr.STORE1_MATCH1 = xor(b.spr.STORE1_X1, b.vid.X1n);
    /*p31.EBEF*/     c.spr.STORE1_MATCH2 = xor(b.spr.STORE1_X2, b.vid.X2n);
    /*p31.EWOK*/     c.spr.STORE1_MATCH3 = xor(b.spr.STORE1_X3, b.vid.X3n);
    /*p31.COLA*/     c.spr.STORE1_MATCH4 = xor(b.spr.STORE1_X4, b.vid.X4n);
    /*p31.BOBA*/     c.spr.STORE1_MATCH5 = xor(b.spr.STORE1_X5, b.vid.X5n);
    /*p31.COLU*/     c.spr.STORE1_MATCH6 = xor(b.spr.STORE1_X6, b.vid.X6n);
    /*p31.BAHU*/     c.spr.STORE1_MATCH7 = xor(b.spr.STORE1_X7, b.vid.X7n);
    /*p31.CYVY*/   c.spr.STORE1_MATCHB = nor(b.spr.STORE1_MATCH4, b.spr.STORE1_MATCH5, b.spr.STORE1_MATCH6, b.spr.STORE1_MATCH7);
    /*p31.EWAM*/   c.spr.STORE1_MATCHA = nor(b.spr.STORE1_MATCH0, b.spr.STORE1_MATCH1, b.spr.STORE1_MATCH2, b.spr.STORE1_MATCH3);
    /*p29.DYDU*/ c.spr.STORE1_MATCHn = nand(b.spr.MATCH_EN, b.spr.STORE1_MATCHA, b.spr.STORE1_MATCHB);

    /*p29.CUVA*/       c.spr.STORE1_SEL  = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n); // 0001
    /*p29.BYBY*/     c.spr.STORE1_CLK  = or(b.spr.STORE_EN, b.spr.STORE1_SEL); // 0001
    /*p29.BUCO*/   c.spr.STORE1_CLKn = not(b.spr.STORE1_CLK); // 0001
    /*p29.BYVY*/ c.spr.STORE1_CLKc = not(b.spr.STORE1_CLKn);
    /*p29.AHOF*/ c.spr.STORE1_CLKb = not(b.spr.STORE1_CLKn);
    /*p29.ASYS*/ c.spr.STORE1_CLKa = not(b.spr.STORE1_CLKn);

    /*p29.CEDY*/     c.spr.SPRITE1_GET_SYNCn = tock_pos(a.spr.WUTY_CLK, b.spr.WUTY_CLK, b.spr.NEW_LINE3, b.spr.SPRITE1_GET_SYNCn, b.spr.ENUT);
    /*p29.EFEV*/   c.spr.EFEV = or(b.spr.NEW_LINE4, b.spr.SPRITE1_GET_SYNCn);
    /*p29.DOKU*/ c.spr.DOKU = not(b.spr.EFEV);

    /*p30.CAJY*/ c.spr.STORE1_IDX0  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX0,  b.spr.TS_IDX_0 );
    /*p30.CUZA*/ c.spr.STORE1_IDX1  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX1,  b.spr.TS_IDX_1 );
    /*p30.COMA*/ c.spr.STORE1_IDX2  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX2,  b.spr.TS_IDX_2 );
    /*p30.CUFA*/ c.spr.STORE1_IDX3  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX3,  b.spr.TS_IDX_3 );
    /*p30.CEBO*/ c.spr.STORE1_IDX4  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX4,  b.spr.TS_IDX_4 );
    /*p30.CADU*/ c.spr.STORE1_IDX5  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,          b.spr.STORE1_IDX5,  b.spr.TS_IDX_5 );
                                                                                                 
    /*p30.ABUG*/ c.spr.STORE1_LINE0 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,          b.spr.STORE1_LINE0, b.spr.TS_LINE_0);
    /*p30.AMES*/ c.spr.STORE1_LINE1 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,          b.spr.STORE1_LINE1, b.spr.TS_LINE_1);
    /*p30.ABOP*/ c.spr.STORE1_LINE2 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,          b.spr.STORE1_LINE2, b.spr.TS_LINE_2);
    /*p30.AROF*/ c.spr.STORE1_LINE3 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,          b.spr.STORE1_LINE3, b.spr.TS_LINE_3);

    /*p31.DANY*/ c.spr.STORE1_X0    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X0,    b.spr.OAM_A_D0b);
    /*p31.DUKO*/ c.spr.STORE1_X1    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X1,    b.spr.OAM_A_D1b);
    /*p31.DESU*/ c.spr.STORE1_X2    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X2,    b.spr.OAM_A_D2b);
    /*p31.DAZO*/ c.spr.STORE1_X3    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X3,    b.spr.OAM_A_D3b);
    /*p31.DAKE*/ c.spr.STORE1_X4    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X4,    b.spr.OAM_A_D4b);
    /*p31.CESO*/ c.spr.STORE1_X5    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X5,    b.spr.OAM_A_D5b);
    /*p31.DYFU*/ c.spr.STORE1_X6    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X6,    b.spr.OAM_A_D6b);
    /*p31.CUSY*/ c.spr.STORE1_X7    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, b.spr.DOKU, b.spr.STORE1_X7,    b.spr.OAM_A_D7b);

    /*p29.ENUT*/   c.spr.ENUT = nor(b.spr.STORE1_MATCHn, b.spr.STORE0_MATCH_OUT);
    /*p29.DYDO*/ c.spr.DYDO = not(b.spr.ENUT);

    /*p30.BEMO*/ if (b.spr.DYDO) c.spr.TS_IDX_0  = not(!b.spr.STORE1_IDX0);
    /*p30.CYBY*/ if (b.spr.DYDO) c.spr.TS_IDX_1  = not(!b.spr.STORE1_IDX1);
    /*p30.BETY*/ if (b.spr.DYDO) c.spr.TS_IDX_2  = not(!b.spr.STORE1_IDX2);
    /*p30.CEGY*/ if (b.spr.DYDO) c.spr.TS_IDX_3  = not(!b.spr.STORE1_IDX3);
    /*p30.CELU*/ if (b.spr.DYDO) c.spr.TS_IDX_4  = not(!b.spr.STORE1_IDX4);
    /*p30.CUBO*/ if (b.spr.DYDO) c.spr.TS_IDX_5  = not(!b.spr.STORE1_IDX5);
    /*p30.BEFE*/ if (b.spr.DYDO) c.spr.TS_LINE_0 = not(!b.spr.STORE1_LINE0);
    /*p30.BYRO*/ if (b.spr.DYDO) c.spr.TS_LINE_1 = not(!b.spr.STORE1_LINE1);
    /*p30.BACO*/ if (b.spr.DYDO) c.spr.TS_LINE_2 = not(!b.spr.STORE1_LINE2);
    /*p30.AHUM*/ if (b.spr.DYDO) c.spr.TS_LINE_3 = not(!b.spr.STORE1_LINE3);

    /*p29.GAJA*/     c.spr.STORE1_MATCH = not(b.spr.STORE1_MATCHn);
    /*p29.FUMA*/ c.spr.STORE1_MATCH_OUT = or(b.spr.STORE1_MATCH, b.spr.STORE0_MATCH_OUT);
  }

  //----------
  // store 2

  {
    /*p31.FUZU*/ c.spr.STORE2_MATCH0 = xor(b.spr.STORE2_X0, b.vid.X0n);
    /*p31.FESO*/ c.spr.STORE2_MATCH1 = xor(b.spr.STORE2_X1, b.vid.X1n);
    /*p31.FOKY*/ c.spr.STORE2_MATCH2 = xor(b.spr.STORE2_X2, b.vid.X2n);
    /*p31.FYVA*/ c.spr.STORE2_MATCH3 = xor(b.spr.STORE2_X3, b.vid.X3n);
    /*p31.CEKO*/ c.spr.STORE2_MATCH4 = xor(b.spr.STORE2_X4, b.vid.X4n);
    /*p31.DETY*/ c.spr.STORE2_MATCH5 = xor(b.spr.STORE2_X5, b.vid.X5n);
    /*p31.DOZO*/ c.spr.STORE2_MATCH6 = xor(b.spr.STORE2_X6, b.vid.X6n);
    /*p31.CONY*/ c.spr.STORE2_MATCH7 = xor(b.spr.STORE2_X7, b.vid.X7n);
    /*p31.CEHU*/ c.spr.CEHU = nor(b.spr.STORE2_MATCH4, b.spr.STORE2_MATCH5, b.spr.STORE2_MATCH6, b.spr.STORE2_MATCH7);
    /*p31.EKES*/ c.spr.EKES = nor(b.spr.STORE2_MATCH0, b.spr.STORE2_MATCH1, b.spr.STORE2_MATCH2, b.spr.STORE2_MATCH3);
    /*p29.DEGO*/ c.spr.DEGO = nand(b.spr.MATCH_EN, b.spr.EKES, b.spr.CEHU);
    /*p29.GUPO*/ c.spr.GUPO = not(b.spr.DEGO);
    /*p29.EMOL*/ c.spr.EMOL = nor(b.spr.DEGO, b.spr.STORE1_MATCH_OUT);

    /*p29.GEBU*/ c.spr.STORE2_SEL = nand(b.spr.SPRITE_COUNT0n, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n); // 0010
    /*p29.WYXO*/ c.spr.STORE2_CLK = or(b.spr.STORE_EN, b.spr.STORE2_SEL); // 0010
    /*p29.GYFO*/ c.spr.STORE2_CLKn = not(b.spr.STORE2_CLK); // 0010
    /*p29.CACU*/ c.spr.STORE2_CLKa = not(b.spr.STORE2_CLKn);
    /*p29.BUZY*/ c.spr.STORE2_CLKb = not(b.spr.STORE2_CLKn);
    /*p29.FUKE*/ c.spr.STORE2_CLKc = not(b.spr.STORE2_CLKn);

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

    /*p31.FOKA*/ c.spr.STORE2_X0 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X0, b.spr.OAM_A_D0b);
    /*p31.FYTY*/ c.spr.STORE2_X1 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X1, b.spr.OAM_A_D1b);
    /*p31.FUBY*/ c.spr.STORE2_X2 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X2, b.spr.OAM_A_D2b);
    /*p31.GOXU*/ c.spr.STORE2_X3 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X3, b.spr.OAM_A_D3b);
    /*p31.DUHY*/ c.spr.STORE2_X4 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X4, b.spr.OAM_A_D4b);
    /*p31.EJUF*/ c.spr.STORE2_X5 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X5, b.spr.OAM_A_D5b);
    /*p31.ENOR*/ c.spr.STORE2_X6 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X6, b.spr.OAM_A_D6b);
    /*p31.DEPY*/ c.spr.STORE2_X7 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, b.spr.GAMY, b.spr.STORE2_X7, b.spr.OAM_A_D7b);


    /*p30.AXEC*/ if (b.spr.FAME) c.spr.TS_IDX_0  = not(!b.spr.STORE2_IDX0);
    /*p30.CYRO*/ if (b.spr.FAME) c.spr.TS_IDX_1  = not(!b.spr.STORE2_IDX1);
    /*p30.CUVU*/ if (b.spr.FAME) c.spr.TS_IDX_2  = not(!b.spr.STORE2_IDX2);
    /*p30.APON*/ if (b.spr.FAME) c.spr.TS_IDX_3  = not(!b.spr.STORE2_IDX3);
    /*p30.AFOZ*/ if (b.spr.FAME) c.spr.TS_IDX_4  = not(!b.spr.STORE2_IDX4);
    /*p30.CUBE*/ if (b.spr.FAME) c.spr.TS_IDX_5  = not(!b.spr.STORE2_IDX5);
    /*p30.ZABY*/ if (b.spr.FAME) c.spr.TS_LINE_0 = not(!b.spr.STORE2_LINE0);
    /*p30.ZUKE*/ if (b.spr.FAME) c.spr.TS_LINE_1 = not(!b.spr.STORE2_LINE1);
    /*p30.WUXE*/ if (b.spr.FAME) c.spr.TS_LINE_2 = not(!b.spr.STORE2_LINE2);
    /*p30.WERE*/ if (b.spr.FAME) c.spr.TS_LINE_3 = not(!b.spr.STORE2_LINE3);

  }

  // store 3
  {
    /*p31.YHOK*/     c.spr.YHOK = xor(b.spr.XOLY, b.vid.X0n);
    /*p31.YCAH*/     c.spr.YCAH = xor(b.spr.XYBA, b.vid.X1n);
    /*p31.YDAJ*/     c.spr.YDAJ = xor(b.spr.XABE, b.vid.X2n);
    /*p31.YVUZ*/     c.spr.YVUZ = xor(b.spr.XEKA, b.vid.X3n);
    /*p31.ZURE*/   c.spr.ZURE = nor(b.spr.YHOK, b.spr.YCAH, b.spr.YDAJ, b.spr.YVUZ);
    /*p31.YVAP*/     c.spr.YVAP = xor(b.spr.XOMY, b.vid.X4n);
    /*p31.XENY*/     c.spr.XENY = xor(b.spr.WUHA, b.vid.X5n);
    /*p31.XAVU*/     c.spr.XAVU = xor(b.spr.WYNA, b.vid.X6n);
    /*p31.XEVA*/     c.spr.XEVA = xor(b.spr.WECO, b.vid.X7n);
    /*p31.YWOS*/   c.spr.YWOS = nor(b.spr.YVAP, b.spr.XENY, b.spr.XAVU, b.spr.XEVA);
    /*p29.YLOZ*/ c.spr.YLOZ = nand(b.spr.MATCH_EN, b.spr.ZURE, b.spr.YWOS);

    /*p29.FOCO*/ c.spr.STORE3_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1b, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3n); // 0011
    /*p29.GUVE*/ c.spr.STORE3_CLK = or(b.spr.STORE_EN, b.spr.STORE3_SEL); // 0011
    /*p29.GUSA*/ c.spr.GUSA = not(b.spr.STORE3_CLK);
    /*p29.FEKA*/ c.spr.FEKA = not(b.spr.GUSA);
    /*p29.XYHA*/ c.spr.XYHA = not(b.spr.GUSA);
    /*p29.YFAG*/ c.spr.YFAG = not(b.spr.GUSA);
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
    /*p31.XOLY*/ c.spr.XOLY = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.XOLY, b.spr.OAM_A_D0b);
    /*p31.XYBA*/ c.spr.XYBA = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.XYBA, b.spr.OAM_A_D1b);
    /*p31.XABE*/ c.spr.XABE = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.XABE, b.spr.OAM_A_D2b);
    /*p31.XEKA*/ c.spr.XEKA = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.XEKA, b.spr.OAM_A_D3b);
    /*p31.XOMY*/ c.spr.XOMY = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.XOMY, b.spr.OAM_A_D4b);
    /*p31.WUHA*/ c.spr.WUHA = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.WUHA, b.spr.OAM_A_D5b);
    /*p31.WYNA*/ c.spr.WYNA = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.WYNA, b.spr.OAM_A_D6b);
    /*p31.WECO*/ c.spr.WECO = tock_pos(a.spr.YFAG, b.spr.YFAG, b.spr.WUPA, b.spr.WECO, b.spr.OAM_A_D7b);
  }

  // store 4
  {
    /*p31.ZYKU*/ c.spr.ZYKU = xor(b.spr.YBED, b.vid.X4n);
    /*p31.ZYPU*/ c.spr.ZYPU = xor(b.spr.ZALA, b.vid.X5n);
    /*p31.XAHA*/ c.spr.XAHA = xor(b.spr.WYDE, b.vid.X6n);
    /*p31.ZEFE*/ c.spr.ZEFE = xor(b.spr.XEPA, b.vid.X7n);
    /*p31.XEJU*/ c.spr.XEJU = xor(b.spr.WEDU, b.vid.X0n);
    /*p31.ZATE*/ c.spr.ZATE = xor(b.spr.YGAJ, b.vid.X1n);
    /*p31.ZAKU*/ c.spr.ZAKU = xor(b.spr.ZYJO, b.vid.X2n);
    /*p31.YBOX*/ c.spr.YBOX = xor(b.spr.XURY, b.vid.X3n);
    /*p31.YKOK*/ c.spr.YKOK = nor(b.spr.ZYKU, b.spr.ZYPU, b.spr.XAHA, b.spr.ZEFE);
    /*p31.YNAZ*/ c.spr.YNAZ = nor(b.spr.XEJU, b.spr.ZATE, b.spr.ZAKU, b.spr.YBOX);
    /*p29.XAGE*/ c.spr.XAGE = nand(b.spr.MATCH_EN, b.spr.YNAZ, b.spr.YKOK);

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

    /*p29.GONO*/ c.spr.GONO = nor(b.spr.XAGE, b.spr.WUTO);
    /*p29.WUNA*/ c.spr.WUNA = not(b.spr.XAGE);

  }

  // store 5
  /*p31.BAZY*/ c.spr.BAZY = xor(b.spr.CYWE, b.vid.X4n);
  /*p31.CYLE*/ c.spr.CYLE = xor(b.spr.DYBY, b.vid.X5n);
  /*p31.CEVA*/ c.spr.CEVA = xor(b.spr.DURY, b.vid.X6n);
  /*p31.BUMY*/ c.spr.BUMY = xor(b.spr.CUVY, b.vid.X7n);
  /*p31.GUZO*/ c.spr.GUZO = xor(b.spr.FUSA, b.vid.X0n);
  /*p31.GOLA*/ c.spr.GOLA = xor(b.spr.FAXA, b.vid.X1n);
  /*p31.GEVE*/ c.spr.GEVE = xor(b.spr.FOZY, b.vid.X2n);
  /*p31.GUDE*/ c.spr.GUDE = xor(b.spr.FESY, b.vid.X3n);
  /*p31.COGY*/ c.spr.COGY = nor(b.spr.BAZY, b.spr.CYLE, b.spr.CEVA, b.spr.BUMY);
  /*p31.FYMA*/ c.spr.FYMA = nor(b.spr.GUZO, b.spr.GOLA, b.spr.GEVE, b.spr.GUDE);
  /*p29.EGOM*/ c.spr.EGOM = nand(b.spr.MATCH_EN, b.spr.FYMA, b.spr.COGY);

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


  // store 6
  /*p31.ZARE*/ c.spr.ZARE = xor(b.spr.ZOLY, b.vid.X4n);
  /*p31.ZEMU*/ c.spr.ZEMU = xor(b.spr.ZOGO, b.vid.X5n);
  /*p31.ZYGO*/ c.spr.ZYGO = xor(b.spr.ZECU, b.vid.X6n);
  /*p31.ZUZY*/ c.spr.ZUZY = xor(b.spr.ZESA, b.vid.X7n);
  /*p31.XOSU*/ c.spr.XOSU = xor(b.spr.YCOL, b.vid.X0n);
  /*p31.ZUVU*/ c.spr.ZUVU = xor(b.spr.YRAC, b.vid.X1n);
  /*p31.XUCO*/ c.spr.XUCO = xor(b.spr.YMEM, b.vid.X2n);
  /*p31.ZULO*/ c.spr.ZULO = xor(b.spr.YVAG, b.vid.X3n);
  /*p31.YWAP*/ c.spr.YWAP = nor(b.spr.ZARE, b.spr.ZEMU, b.spr.ZYGO, b.spr.ZUZY);
  /*p31.YDOT*/ c.spr.YDOT = nor(b.spr.XOSU, b.spr.ZUVU, b.spr.XUCO, b.spr.ZULO);
  /*p29.YBEZ*/ c.spr.YBEZ = nand(b.spr.MATCH_EN, b.spr.YDOT, b.spr.YWAP);

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


  // store 7
  /*p31.EJOT*/ c.spr.EJOT = xor(b.spr.FAZU, b.vid.X4n);
  /*p31.ESAJ*/ c.spr.ESAJ = xor(b.spr.FAXE, b.vid.X5n);
  /*p31.DUCU*/ c.spr.DUCU = xor(b.spr.EXUK, b.vid.X6n);
  /*p31.EWUD*/ c.spr.EWUD = xor(b.spr.FEDE, b.vid.X7n);
  /*p31.DUSE*/ c.spr.DUSE = xor(b.spr.ERAZ, b.vid.X0n);
  /*p31.DAGU*/ c.spr.DAGU = xor(b.spr.EPUM, b.vid.X1n);
  /*p31.DYZE*/ c.spr.DYZE = xor(b.spr.EROL, b.vid.X2n);
  /*p31.DESO*/ c.spr.DESO = xor(b.spr.EHYN, b.vid.X3n);
  /*p31.DAJE*/ c.spr.DAJE = nor(b.spr.EJOT, b.spr.ESAJ, b.spr.DUCU, b.spr.EWUD);
  /*p31.CYCO*/ c.spr.CYCO = nor(b.spr.DUSE, b.spr.DAGU, b.spr.DYZE, b.spr.DESO);
  /*p29.DYKA*/ c.spr.DYKA = nand(b.spr.MATCH_EN, b.spr.CYCO, b.spr.DAJE);

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



  // store 8
  /*p31.DUZE*/ c.spr.DUZE = xor(b.spr.EZUF, b.vid.X4n);
  /*p31.DAGA*/ c.spr.DAGA = xor(b.spr.ENAD, b.vid.X5n);
  /*p31.DAWU*/ c.spr.DAWU = xor(b.spr.EBOW, b.vid.X6n);
  /*p31.EJAW*/ c.spr.EJAW = xor(b.spr.FYCA, b.vid.X7n);
  /*p31.GOHO*/ c.spr.GOHO = xor(b.spr.GAVY, b.vid.X0n);
  /*p31.GASU*/ c.spr.GASU = xor(b.spr.GYPU, b.vid.X1n);
  /*p31.GABU*/ c.spr.GABU = xor(b.spr.GADY, b.vid.X2n);
  /*p31.GAFE*/ c.spr.GAFE = xor(b.spr.GAZA, b.vid.X3n);
  /*p31.DAMA*/ c.spr.DAMA = nor(b.spr.DUZE, b.spr.DAGA, b.spr.DAWU, b.spr.EJAW);
  /*p31.FEHA*/ c.spr.FEHA = nor(b.spr.GOHO, b.spr.GASU, b.spr.GABU, b.spr.GAFE);
  /*p29.EFYL*/ c.spr.EFYL = nand(b.spr.MATCH_EN, b.spr.FEHA, b.spr.DAMA);
  /*p29.GEKE*/ c.spr.GEKE = not(b.spr.EFYL);
  /*p29.GYGA*/ c.spr.GYGA = or(b.spr.GEKE, b.spr.FAVO);
  /*p29.FOXA*/ c.spr.FOXA = nor(b.spr.EFYL, b.spr.FAVO);

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


  // store 9
  /*p31.ZYWU*/ c.spr.ZYWU = xor(b.spr.YPOD, b.vid.X4n);
  /*p31.ZUZA*/ c.spr.ZUZA = xor(b.spr.YROP, b.vid.X5n);
  /*p31.ZEJO*/ c.spr.ZEJO = xor(b.spr.YNEP, b.vid.X6n);
  /*p31.ZEDA*/ c.spr.ZEDA = xor(b.spr.YZOF, b.vid.X7n);
  /*p31.YMAM*/ c.spr.YMAM = xor(b.spr.XUVY, b.vid.X0n);
  /*p31.YTYP*/ c.spr.YTYP = xor(b.spr.XERE, b.vid.X1n);
  /*p31.YFOP*/ c.spr.YFOP = xor(b.spr.XUZO, b.vid.X2n);
  /*p31.YVAC*/ c.spr.YVAC = xor(b.spr.XEXA, b.vid.X3n);
  /*p31.YTUB*/ c.spr.YTUB = nor(b.spr.ZYWU, b.spr.ZUZA, b.spr.ZEJO, b.spr.ZEDA);
  /*p31.YLEV*/ c.spr.YLEV = nor(b.spr.YMAM, b.spr.YTYP, b.spr.YFOP, b.spr.YVAC);
  /*p29.YGEM*/ c.spr.YGEM = nand(b.spr.MATCH_EN, b.spr.YLEV, b.spr.YTUB);

  /*p29.DOGU*/ c.spr.STORE9_SEL = nand(b.spr.SPRITE_COUNT0b, b.spr.SPRITE_COUNT1n, b.spr.SPRITE_COUNT2n, b.spr.SPRITE_COUNT3b); // 1001
  /*p29.CATO*/ c.spr.STORE9_CLK = or(b.spr.STORE_EN, b.spr.STORE9_SEL); // 1001
  /*p29.DECU*/ c.spr.DECU = not(b.spr.STORE9_CLK);
  /*p29.WEME*/ c.spr.WEME = not(b.spr.DECU);
  /*p29.WUFA*/ c.spr.WUFA = not(b.spr.DECU);
  /*p29.FAKA*/ c.spr.FAKA = not(b.spr.DECU);
  /*p31.XUVY*/ c.spr.XUVY = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.XUVY, b.spr.OAM_A_D0b);
  /*p31.XERE*/ c.spr.XERE = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.XERE, b.spr.OAM_A_D1b);
  /*p31.XUZO*/ c.spr.XUZO = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.XUZO, b.spr.OAM_A_D2b);
  /*p31.XEXA*/ c.spr.XEXA = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.XEXA, b.spr.OAM_A_D3b);
  /*p31.YPOD*/ c.spr.YPOD = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.YPOD, b.spr.OAM_A_D4b);
  /*p31.YROP*/ c.spr.YROP = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.YROP, b.spr.OAM_A_D5b);
  /*p31.YNEP*/ c.spr.YNEP = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.YNEP, b.spr.OAM_A_D6b);
  /*p31.YZOF*/ c.spr.YZOF = tock_pos(a.spr.WEME, b.spr.WEME, b.spr.DOSY, b.spr.YZOF, b.spr.OAM_A_D7b);

  /*p30.XUFO*/ c.spr.XUFO = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.XUFO, b.spr.TS_IDX_0 );
  /*p30.XUTE*/ c.spr.XUTE = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.XUTE, b.spr.TS_IDX_1 );
  /*p30.XOTU*/ c.spr.XOTU = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.XOTU, b.spr.TS_IDX_2 );
  /*p30.XYFE*/ c.spr.XYFE = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.XYFE, b.spr.TS_IDX_3 );
  /*p30.YZOR*/ c.spr.YZOR = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.YZOR, b.spr.TS_IDX_4 );
  /*p30.YBER*/ c.spr.YBER = tock_pos(a.spr.WUFA, b.spr.WUFA, 0, b.spr.YBER, b.spr.TS_IDX_5 );
  /*p30.DEWU*/ c.spr.DEWU = tock_pos(a.spr.FAKA, b.spr.FAKA, 0, b.spr.DEWU, b.spr.TS_LINE_0);
  /*p30.CANA*/ c.spr.CANA = tock_pos(a.spr.FAKA, b.spr.FAKA, 0, b.spr.CANA, b.spr.TS_LINE_1);
  /*p30.DYSY*/ c.spr.DYSY = tock_pos(a.spr.FAKA, b.spr.FAKA, 0, b.spr.DYSY, b.spr.TS_LINE_2);
  /*p30.FOFO*/ c.spr.FOFO = tock_pos(a.spr.FAKA, b.spr.FAKA, 0, b.spr.FOFO, b.spr.TS_LINE_3);


  //----------

  /*p30.WOCY*/ if (b.spr.XYME) c.spr.TS_IDX_0  = not(!b.spr.GECU);;
  /*p30.ELYC*/ if (b.spr.XYME) c.spr.TS_IDX_1  = not(!b.spr.FOXY);;
  /*p30.WABO*/ if (b.spr.XYME) c.spr.TS_IDX_2  = not(!b.spr.GOHU);;
  /*p30.EZOC*/ if (b.spr.XYME) c.spr.TS_IDX_3  = not(!b.spr.FOGO);;
  /*p30.WYWY*/ if (b.spr.XYME) c.spr.TS_IDX_4  = not(!b.spr.GACY);;
  /*p30.WATO*/ if (b.spr.XYME) c.spr.TS_IDX_5  = not(!b.spr.GABO);;
  /*p30.ZUDO*/ if (b.spr.XYME) c.spr.TS_LINE_0 = not(!b.spr.ZUBE);;
  /*p30.YBUK*/ if (b.spr.XYME) c.spr.TS_LINE_1 = not(!b.spr.ZUMY);;
  /*p30.ZYTO*/ if (b.spr.XYME) c.spr.TS_LINE_2 = not(!b.spr.ZEXO);;
  /*p30.YKOZ*/ if (b.spr.XYME) c.spr.TS_LINE_3 = not(!b.spr.ZAFU);;

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

  /*p30.DEZU*/ if (b.spr.GYMA) c.spr.TS_IDX_0  = not(!b.spr.DAFU);
  /*p30.EFUD*/ if (b.spr.GYMA) c.spr.TS_IDX_1  = not(!b.spr.DEBA);
  /*p30.DONY*/ if (b.spr.GYMA) c.spr.TS_IDX_2  = not(!b.spr.DUHA);
  /*p30.DOWA*/ if (b.spr.GYMA) c.spr.TS_IDX_3  = not(!b.spr.DUNY);
  /*p30.DYGO*/ if (b.spr.GYMA) c.spr.TS_IDX_4  = not(!b.spr.DESE);
  /*p30.ENAP*/ if (b.spr.GYMA) c.spr.TS_IDX_5  = not(!b.spr.DEVY);
  /*p30.ZYPO*/ if (b.spr.GYMA) c.spr.TS_LINE_0 = not(!b.spr.ZURY);
  /*p30.ZEXE*/ if (b.spr.GYMA) c.spr.TS_LINE_1 = not(!b.spr.ZURO);
  /*p30.YJEM*/ if (b.spr.GYMA) c.spr.TS_LINE_2 = not(!b.spr.ZENE);
  /*p30.YWAV*/ if (b.spr.GYMA) c.spr.TS_LINE_3 = not(!b.spr.ZYLU);

  /*p30.YHAL*/ if (b.spr.FADO) c.spr.TS_IDX_0  = not(!b.spr.XUFO);
  /*p30.YRAD*/ if (b.spr.FADO) c.spr.TS_IDX_1  = not(!b.spr.XUTE);
  /*p30.XYRA*/ if (b.spr.FADO) c.spr.TS_IDX_2  = not(!b.spr.XOTU);
  /*p30.YNEV*/ if (b.spr.FADO) c.spr.TS_IDX_3  = not(!b.spr.XYFE);
  /*p30.ZOJY*/ if (b.spr.FADO) c.spr.TS_IDX_4  = not(!b.spr.YZOR);
  /*p30.ZARO*/ if (b.spr.FADO) c.spr.TS_IDX_5  = not(!b.spr.YBER);
  /*p30.CAWO*/ if (b.spr.FADO) c.spr.TS_LINE_0 = not(!b.spr.DEWU);
  /*p30.BYME*/ if (b.spr.FADO) c.spr.TS_LINE_1 = not(!b.spr.CANA);
  /*p30.COHO*/ if (b.spr.FADO) c.spr.TS_LINE_2 = not(!b.spr.DYSY);
  /*p30.GATE*/ if (b.spr.FADO) c.spr.TS_LINE_3 = not(!b.spr.FOFO);
}

//-----------------------------------------------------------------------------

};