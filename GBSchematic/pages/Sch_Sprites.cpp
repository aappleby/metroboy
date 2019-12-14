#include "Sch_Sprites.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct SpritesIn {
  bool FROM_CPU5;
};

void Sprites_tick(const SpritesIn& in,
                  const Bus& bus_in,
                  const Pins& pins,
                  const Clocks& clocks,
                  const Decoder& dec,
                  const Resets& resets,
                  const DMA& dma,
                  const LCD& lcd,
                  const Video& vid,
                  const Registers& regs,
                  const Sprites& b,
                  Sprites& next) {

  ///*p29.TEPA not*/ next.RENDERINGn = not(prev.RENDERING);

  /*p28.WEFE*/ next.P10_Bn = not(pins.P10_B);
  /*p01.ROSY*/ wire VID_RESET5  = not(resets.VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(resets.VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
  
  /*p04.DECY*/ wire FROM_CPU5n = not(in.FROM_CPU5);
  /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);
  

  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus_in.A15, bus_in.A14, bus_in.A13, bus_in.A12, bus_in.A11, bus_in.A10, bus_in.A09);
  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p07.SOHA*/ wire ADDR_FFXXn2 = not(dec.ADDR_FFXX);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ wire ADDR_OAM = not(ADDR_FEXXn);

  {
    // dma polarity backwards?
    //OAM_ADDR_PARSEn  = and(DMA_RUNNING_SYNC, or(IN_LINE_SYNCa, VID_RESET6, SCAN_DONE_TRIG));


    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(dma.DMA_RUNNING_SYNC);
    /*p28.ASEN*/ wire ASEN = or(VID_RESET6, b.SCAN_DONE_TRIG);
    /*p28.BESU*/ wire BESU = or(b.IN_LINE_SYNCa, ASEN);
    /*p28.ACYL*/ next.OAM_ADDR_PARSEn  = and(DMA_RUNNING_SYNCb, BESU);
  }


  // these are unused
  /*
  next.XUCA = not(vid.FF40_WR);
  next.XECY = tock_pos(a.spr.XUCA, b.XUCA, 0,          b.XECY, resets.SYS_RESETn4); // ? weird
  next.XUVA = tock_pos(a.spr.XYNY, b.XYNY, b.sys.XARE, b.XUVA, b.XECY);
  */

  //----------
  // bottom


  //----------
  // Sprite scanner
  


  /*p29.BUZA*/ next.STORE_SPRITE_IDX = and(!b.CENO, vid.RENDERING);
  /*p29.CEHA*/ next.CEHA = not(!b.CENO);
  
  {

    /*p28.ASEN*/ wire ASEN = or(VID_RESET6, b.SCAN_DONE_TRIG);
    /*p28.BESU*/ wire BESU = or(b.IN_LINE_SYNCa, ASEN);

    /*p29.CENO*/ next.CENO.tock(clocks.XUPY_xBCxxFGx, VID_RESETn3, BESU);
  }



  {
    /*p28.ANOM*/ wire SCAN_RSTn = nor(VID_RESET6, lcd.NEW_LINE1);
    /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
    /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);

    /*p28.FETO*/ wire SCAN_DONE = and(b.SCAN0, b.SCAN1, b.SCAN2, b.SCAN5); // 32 + 4 + 2 + 1 = 39

    /*p28.GAVA*/ wire SCAN_CLK  = or(SCAN_DONE, clocks.XUPY_xBCxxFGx);
    /*p28.YFEL*/ next.SCAN0.tock(SCAN_CLK, SCAN_RSTn, !b.SCAN0);
    /*p28.WEWY*/ next.SCAN1.tock(!b.SCAN0, SCAN_RSTn, !b.SCAN1);
    /*p28.GOSO*/ next.SCAN2.tock(!b.SCAN1, SCAN_RSTn, !b.SCAN2);
    /*p28.ELYN*/ next.SCAN3.tock(!b.SCAN2, SCAN_RSTn, !b.SCAN3);
    /*p28.FAHA*/ next.SCAN4.tock(!b.SCAN3, SCAN_RSTn, !b.SCAN4);
    /*p28.FONY*/ next.SCAN5.tock(!b.SCAN4, SCAN_RSTn, !b.SCAN5);

    /*p29.BYBA*/ next.SCAN_DONE_SYNC1.tock(clocks.XUPY_xBCxxFGx, SCAN_RSTo, SCAN_DONE);
    /*p29.DOBA*/ next.SCAN_DONE_SYNC2.tock(clocks.ALET_AxCxExGx, SCAN_RSTo, b.SCAN_DONE_SYNC1);

    /*p29.BEBU or  */ wire SCAN_DONE_TRIGn = or(SCAN_RSTa, b.SCAN_DONE_SYNC2, !b.SCAN_DONE_SYNC1);
    /*p29.AVAP not2*/ next.SCAN_DONE_TRIG = not(SCAN_DONE_TRIGn);
  }

  
  //----------
  // undeciphered



  {
    // tags wrong somewhere
    /*p29.TAME*/ wire SEQ_5n = nand(b.SPR_SEQ2, b.SPR_SEQ0);
    /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);

  
    /*p27.RYCE*/ wire SPRITE_TRIG = and(vid.TEKY_SYNC1, !vid.TEKY_SYNC2);

    /*p29.TOMA*/ wire SEQ_CLK = nand(clocks.LAPE_xBxDxFxH, SEQ_5n);
    /*p27.SECA*/ wire SPR_SEQ_RST = nor(SPRITE_TRIG, VID_RESET5, lcd.NEW_LINE1);

    /*p29.TOXE*/ next.SPR_SEQ0.tock(SEQ_CLK,     SPR_SEQ_RST, !b.SPR_SEQ0);
    /*p29.TULY*/ next.SPR_SEQ1.tock(!b.SPR_SEQ0, SPR_SEQ_RST, !b.SPR_SEQ1);
    /*p29.TESE*/ next.SPR_SEQ2.tock(!b.SPR_SEQ1, SPR_SEQ_RST, !b.SPR_SEQ2);

    /*p29.TOBU*/ next.SPR_DEL0.tock(clocks.TAVA_AxCxExGx, vid.RENDERING, b.SPR_SEQ1);
    /*p29.VONU*/ next.SPR_DEL1.tock(clocks.TAVA_AxCxExGx, vid.RENDERING, !b.SPR_DEL0);
    /*p29.SEBA*/ next.SPR_DEL2.tock(clocks.LAPE_xBxDxFxH, vid.RENDERING, b.SPR_DEL1);

    /*p27.VYPO*/ wire VYPO = not(pins.P10_B);
    /*p29.TYFO*/ next.SPR_SEQ_5_SYNCn.tock(clocks.LAPE_xBxDxFxH, VYPO, SEQ_5n);

#if 0
    {
      /*p29.SYCU*/         wire SYCU = nor(SEQ_5, b.RENDERINGn, b.SPR_SEQ_5_SYNCn);
      /*p29.TOPU*/       wire SPRITE_PIX_LATCH_Ao = and(b.SPR_SEQ1, SYCU);
      /*p29.VYWA*/     wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
      /*p29.WENY*/   wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
      /*p29.XADO*/ next.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);
    }

    {
      /*p29.SYCU*/         wire SYCU = nor(SEQ_5, b.RENDERINGn, b.SPR_SEQ_5_SYNCn);
      /*p29.RACA*/       wire SPRITE_PIX_LATCH_Bo = and(b.SPR_DEL1, SYCU);
      /*p29.PEBY*/     wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
      /*p29.NYBE*/   wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
      /*p29.PUCO*/ next.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);
    }

    {
      /*p29.TYNO*/     wire TYNO = nand(b.SPR_SEQ0, b.SPR_DEL2, b.SPR_DEL1);
      /*p29.VUSA*/   wire VUSA = or(!b.SPR_SEQ_5_SYNCn, TYNO);
      /*p29.WUTY*/ next.SPRITE_DONE = not(VUSA);
    }

    {
      /*p29.SAKY*/     wire SAKY = nor(b.SPR_SEQ1, b.SPR_DEL1);
      /*p29.TYSO*/   wire SPRITE_READb = or(SAKY, b.RENDERINGn); // seems wrong
      /*p29.TEXY*/ next.SPRITE_READn = not(SPRITE_READb);
    }

    {
      /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, b.SPR_SEQ_5_SYNCn);
      /*p25.SOHO*/   wire SOHO = and(SPR_SEQ_5_TRIG, b.SPRITE_READn);
      /*p25.RAWA*/ next.RAWA = not(SOHO);
    }

    {
      /*p29.TUVO*/     wire TUVO = or(b.RENDERINGn, b.SPR_SEQ1, b.SPR_DEL1);
      /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, b.SPR_SEQ_5_SYNCn);
      /*p25.VAPE*/   wire VAPE = and(TUVO, SPR_SEQ_5_TRIG);
      /*p25.XUJY*/ next.XUJY = not(VAPE);
    }

    {
      /*p29.TUVO*/     wire TUVO = or(b.RENDERINGn, b.SPR_SEQ1, b.SPR_DEL1);
      /*p28.WEFY*/   wire WEFY = and(TUVO, b.SPR_SEQ_5_SYNCn);
      /*p28.XUJA*/ next.XUJA = not(WEFY);
    }
#endif
  }

  //----------












  //----------


  //----------
  // Sprite y comparator

  {
    /*p29.EBOS*/ wire V0n = not(lcd.V0);
    /*p29.DASA*/ wire V1n = not(lcd.V1);
    /*p29.FUKY*/ wire V2n = not(lcd.V2);
    /*p29.FUVE*/ wire V3n = not(lcd.V3);
    /*p29.FEPU*/ wire V4n = not(lcd.V4);
    /*p29.FOFA*/ wire V5n = not(lcd.V5);
    /*p29.FEMO*/ wire V6n = not(lcd.V6);
    /*p29.GUSU*/ wire V7n = not(lcd.V7);

    /*p29.ERUC*/ wire YDIFF0   = add_c(V0n, !b.OAM_B_D0, pins.P10_B);
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(V0n, !b.OAM_B_D0, pins.P10_B);
    /*p29.ENEF*/ wire YDIFF1   = add_s(V1n, !b.OAM_B_D1, YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(V1n, !b.OAM_B_D1, YDIFF_C0);
    /*p29.FECO*/ wire YDIFF2   = add_s(V2n, !b.OAM_B_D2, YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(V2n, !b.OAM_B_D2, YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF3   = add_s(V3n, !b.OAM_B_D3, YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(V3n, !b.OAM_B_D3, YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF4   = add_s(V4n, !b.OAM_B_D4, YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(V4n, !b.OAM_B_D4, YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF5   = add_s(V5n, !b.OAM_B_D5, YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(V5n, !b.OAM_B_D5, YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF6   = add_s(V6n, !b.OAM_B_D6, YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(V6n, !b.OAM_B_D6, YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF7   = add_s(V7n, !b.OAM_B_D7, YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(V7n, !b.OAM_B_D7, YDIFF_C6);

    /*p29.DEGE*/ wire SPRITE_DELTA0 = not(YDIFF0);
    /*p29.DABY*/ wire SPRITE_DELTA1 = not(YDIFF1);
    /*p29.DABU*/ wire SPRITE_DELTA2 = not(YDIFF2);
    /*p29.GYSA*/ wire SPRITE_DELTA3 = not(YDIFF3);
    /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF4);
    /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF5);
    /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF6);
    /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF7);

    /*p29.GOVU*/ wire GOVU = or(YDIFF3, regs.LCDC_SPSIZE);
    /*p29.WOTA*/ wire SPR_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU);
    /*p29.GESE*/ wire SPR_MATCH_Y = not(SPR_MATCH_Yn);
    /*p29.CARE*/ wire STORE_ENn = or(b.CLK_XOCE, b.CEHA, SPR_MATCH_Y);
    /*p29.DYTY*/ next.STORE_EN = not(STORE_ENn);

    /*p30.WENU*/ if (b.STORE_MATCH) next.TS_LINE_0 = SPRITE_DELTA1; // order wrong
    /*p30.CUCU*/ if (b.STORE_MATCH) next.TS_LINE_1 = SPRITE_DELTA2;
    /*p30.CUCA*/ if (b.STORE_MATCH) next.TS_LINE_2 = SPRITE_DELTA0;
    /*p30.CEGA*/ if (b.STORE_MATCH) next.TS_LINE_3 = SPRITE_DELTA3;
  }


  //----------
  // Sprite priority

  {
    /*p29.CEHA*/ wire CEHA = not(!b.CENO);
    /*p29.BYJO*/ wire BYJO = not(CEHA);
    /*p29.AZEM*/ wire AZEM = and(BYJO, vid.RENDERING);
    /*p29.AROR*/ next.MATCH_EN = and(AZEM, regs.LCDC_SPEN);
  }


#if 0
  /*p21.ACAM*/ next.vid.X0n = not(vid.X0);
  /*p21.AZUB*/ next.vid.X1n = not(vid.X1);
  /*p21.AMEL*/ next.vid.X2n = not(vid.X2);
  /*p21.AHAL*/ next.vid.X3n = not(vid.X3);
  /*p21.APUX*/ next.vid.X4n = not(vid.X4);
  /*p21.ABEF*/ next.vid.X5n = not(vid.X5);
  /*p21.ADAZ*/ next.vid.X6n = not(vid.X6);
  /*p21.ASAH*/ next.vid.X7n = not(vid.X7);
#endif

  //----------

}

//-----------------------------------------------------------------------------

};