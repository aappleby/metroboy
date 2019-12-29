#include "Sch_Sprites.h"
#include "Sch_Gameboy.h"
#include "Sch_SpriteStore.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void Sprites_tick(const Pins& pins,
                  const ClockSignals1& clk_sig1,
                  const ClockSignals2& clk_sig2,
                  const DMA& dma,
                  const LCDRegisters& lcd,
                  const OAM& oam,
                  const Video& vid,
                  const ResetSignals2& rst_sig2,
                  const Registers& regs,
                  const SpriteStore& sst,
                  const Sprites& spr,

                  Sprites& next,
                  SpriteIndexLine& sil_out) {


  /*p28.WEFE*/ next.P10_Bn = not(pins.P10_B);
  
  {
    /*p28.BOGE*/ wire DMA_CLKENn = not(dma.DMA_CLKEN);
    /*p28.ASEN*/ wire ASEN = or(rst_sig2.VID_RESET6, spr.SCAN_DONE_d0_TRIG);
    /*p28.BESU*/ wire BESU = or(lcd.VID_LINE_d4, ASEN);
    /*p28.ACYL*/ next.OAM_ADDR_PARSE = and(DMA_CLKENn, BESU);
  }

  /*p29.CEHA*/ wire CEHA = not(!spr.STORE_SPRITE_IDXn);

  {
    /*p29.BYJO*/ wire BYJO = not(CEHA);
    /*p29.AZEM*/ wire AZEM = and(BYJO, vid.RENDERING_LATCH);
    /*p29.AROR*/ next.MATCH_EN = and(AZEM, regs.LCDC_SPEN);
  }

  //----------
  // Sequencer

  {
    /*p29.TAME*/ wire SEQ_5n = nand(spr.SPR_SEQ2, spr.SPR_SEQ0);

    {
      /*p29.TOMA*/ wire SEQ_CLK = nand(clk_sig1.LAPE_xBxDxFxH, SEQ_5n);
      /*p27.SECA*/ wire SPR_SEQ_RST = nor(vid.SPRITE_FETCH_TRIG, rst_sig2.VID_RESET5, lcd.VID_LINE_TRIG_d4n);
      /*p29.TOXE*/ next.SPR_SEQ0.tock(SEQ_CLK,       SPR_SEQ_RST, !spr.SPR_SEQ0);
      /*p29.TULY*/ next.SPR_SEQ1.tock(!spr.SPR_SEQ0, SPR_SEQ_RST, !spr.SPR_SEQ1);
      /*p29.TESE*/ next.SPR_SEQ2.tock(!spr.SPR_SEQ1, SPR_SEQ_RST, !spr.SPR_SEQ2);
    }


    {
      /*p27.VYPO*/ wire VYPO = not(pins.P10_B);
      /*p29.TYFO*/ next.SEQ_B0d.tock    (clk_sig1.LAPE_xBxDxFxH, VYPO, next.SPR_SEQ0);
      /*p29.TOBU*/ next.SEQ_xx23xx .tock(clk_sig1.TAVA_AxCxExGx, vid.RENDERING_LATCH, spr.SPR_SEQ1);    // note input is seq 1 not 2
      /*p29.VONU*/ next.SEQ_xxx34xn.tock(clk_sig1.TAVA_AxCxExGx, vid.RENDERING_LATCH, spr.SEQ_xx23xx);
      /*p29.SEBA*/ next.SEQ_xxxx45n.tock(clk_sig1.LAPE_xBxDxFxH, vid.RENDERING_LATCH, spr.SEQ_xxx34xn); // is this clock wrong?
    }

    {
      /*p29.TYTU*/ wire SPR_SEQ0n = not(spr.SPR_SEQ0);
      /*p29.SYCU*/ wire SYCU = nor(SPR_SEQ0n, vid.RENDERINGn, spr.SEQ_B0d);

      {
        /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(spr.SPR_SEQ1, SYCU);
        /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
        /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
        /*p29.XADO*/ next.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);
      }

      {
        /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(spr.SEQ_xxx34xn, SYCU);
        /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
        /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
        /*p29.PUCO*/ next.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);
      }
    }

    {
      /*p29.TYNO*/ wire TYNO = nand(spr.SPR_SEQ0, spr.SEQ_xxxx45n, spr.SEQ_xxx34xn);
      /*p29.VUSA*/ wire VUSA = or(!spr.SEQ_B0d, TYNO);
      /*p29.WUTY*/ next.SPRITE_DONE = not(VUSA);
    }

    {
      /*p29.SAKY*/ wire SAKY = nor(spr.SPR_SEQ1, spr.SEQ_xxx34xn);
      /*p29.TEPA*/ wire RENDERINGn = not(vid.RENDERING_LATCH);
      /*p29.TYSO*/ wire SPRITE_READq = or(SAKY, RENDERINGn); // seems wrong
      /*p29.TEXY*/ next.SPRITE_READ = not(SPRITE_READq);
    }
    
    ///*p29.XONO*/ wire FLIP_X = and(oam.OAM_A_D5, next.SPRITE_READn);
  }

  //----------
  // Sprite y comparator

  {
    /*p29.EBOS*/ wire V0n = not(lcd.Y0);
    /*p29.DASA*/ wire V1n = not(lcd.Y1);
    /*p29.FUKY*/ wire V2n = not(lcd.Y2);
    /*p29.FUVE*/ wire V3n = not(lcd.Y3);
    /*p29.FEPU*/ wire V4n = not(lcd.Y4);
    /*p29.FOFA*/ wire V5n = not(lcd.Y5);
    /*p29.FEMO*/ wire V6n = not(lcd.Y6);
    /*p29.GUSU*/ wire V7n = not(lcd.Y7);

    /*p29.ERUC*/ wire YDIFF0   = add_c(V0n, !oam.OAM_B_D0, pins.P10_B);
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(V0n, !oam.OAM_B_D0, pins.P10_B);
    /*p29.ENEF*/ wire YDIFF1   = add_s(V1n, !oam.OAM_B_D1, YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(V1n, !oam.OAM_B_D1, YDIFF_C0);
    /*p29.FECO*/ wire YDIFF2   = add_s(V2n, !oam.OAM_B_D2, YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(V2n, !oam.OAM_B_D2, YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF3   = add_s(V3n, !oam.OAM_B_D3, YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(V3n, !oam.OAM_B_D3, YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF4   = add_s(V4n, !oam.OAM_B_D4, YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(V4n, !oam.OAM_B_D4, YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF5   = add_s(V5n, !oam.OAM_B_D5, YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(V5n, !oam.OAM_B_D5, YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF6   = add_s(V6n, !oam.OAM_B_D6, YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(V6n, !oam.OAM_B_D6, YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF7   = add_s(V7n, !oam.OAM_B_D7, YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(V7n, !oam.OAM_B_D7, YDIFF_C6);

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
    /*p29.CARE*/ wire STORE_ENn = or(clk_sig2.XOCE_ABxxEFxx, CEHA, SPR_MATCH_Y);
    /*p29.DYTY*/ next.STORE_EN = not(STORE_ENn);


    /*p28.WEFE*/ wire WEFE = not(pins.P10_B);
    /*p30.CYKE*/ wire CYKE = not(clk_sig2.XUPY_xBCxxFGx);
    /*p30.WUDA*/ wire WUDA = not(CYKE);
    /*p30.XADU*/ next.SPRITE_IDX0.tock(WUDA, WEFE, oam.OAM_A2);
    /*p30.XEDY*/ next.SPRITE_IDX1.tock(WUDA, WEFE, oam.OAM_A3);
    /*p30.ZUZE*/ next.SPRITE_IDX2.tock(WUDA, WEFE, oam.OAM_A4);
    /*p30.XOBE*/ next.SPRITE_IDX3.tock(WUDA, WEFE, oam.OAM_A5);
    /*p30.YDUF*/ next.SPRITE_IDX4.tock(WUDA, WEFE, oam.OAM_A6);
    /*p30.XECU*/ next.SPRITE_IDX5.tock(WUDA, WEFE, oam.OAM_A7);

    /*p28.ASEN*/ wire ASEN = or(rst_sig2.VID_RESET6, spr.SCAN_DONE_d0_TRIG);
    /*p28.BESU*/ wire BESU = or(lcd.VID_LINE_d4, ASEN);
    /*p29.CENO*/ next.STORE_SPRITE_IDXn.tock(clk_sig2.XUPY_xBCxxFGx, rst_sig2.VID_RESETn3, BESU);

    /*p29.BUZA*/ wire STORE_SPRITE_IDX = and(!spr.STORE_SPRITE_IDXn, vid.RENDERING_LATCH);

    /*p30.WENU*/ if (sst.STORE_MATCH)  sil_out.TS_LINE_0 = SPRITE_DELTA1; // order wrong
    /*p30.CUCU*/ if (sst.STORE_MATCH)  sil_out.TS_LINE_1 = SPRITE_DELTA2;
    /*p30.CUCA*/ if (sst.STORE_MATCH)  sil_out.TS_LINE_2 = SPRITE_DELTA0;
    /*p30.CEGA*/ if (sst.STORE_MATCH)  sil_out.TS_LINE_3 = SPRITE_DELTA3;
    /*p30.WEZA*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_0  = spr.SPRITE_IDX5; // order wrong
    /*p30.WUCO*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_1  = spr.SPRITE_IDX4;
    /*p30.WYDA*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_2  = spr.SPRITE_IDX3;
    /*p30.ZYSU*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_3  = spr.SPRITE_IDX2;
    /*p30.WYSE*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_4  = spr.SPRITE_IDX1;
    /*p30.WUZY*/ if (STORE_SPRITE_IDX) sil_out.TS_IDX_5  = spr.SPRITE_IDX0;
  }
}


void Sprites_tickScanner(const ClockSignals1& clk_sig1,
                         const ClockSignals2& clk_sig2,
                         const LCDRegisters& lcd,
                         const ResetSignals2& rst_sig2,
                         const Sprites& spr,
                         
                         Sprites& next) {

  //----------
  // Sprite scanner

  {
    // something wrong here

    // ANOM = nor(ATEJ, ATAR);

    /*p28.ANOM*/ wire SCAN_RSTn = nor(lcd.VID_LINE_TRIG_d4a, rst_sig2.VID_RESET6);
    /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
    /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);


    /*p28.GAVA*/ wire SCAN_CLK = or(spr.SCAN_DONE_d0, clk_sig2.XUPY_xBCxxFGx);
    /*p28.YFEL*/ next.SCAN0.tock(SCAN_CLK,   SCAN_RSTn, !spr.SCAN0);
    /*p28.WEWY*/ next.SCAN1.tock(!spr.SCAN0, SCAN_RSTn, !spr.SCAN1);
    /*p28.GOSO*/ next.SCAN2.tock(!spr.SCAN1, SCAN_RSTn, !spr.SCAN2);
    /*p28.ELYN*/ next.SCAN3.tock(!spr.SCAN2, SCAN_RSTn, !spr.SCAN3);
    /*p28.FAHA*/ next.SCAN4.tock(!spr.SCAN3, SCAN_RSTn, !spr.SCAN4);
    /*p28.FONY*/ next.SCAN5.tock(!spr.SCAN4, SCAN_RSTn, !spr.SCAN5);

    /*p28.FETO*/ next.SCAN_DONE_d0 = and(spr.SCAN0, spr.SCAN1, spr.SCAN2, spr.SCAN5); // 32 + 4 + 2 + 1 = 39

    // the second clock here doesn't match the usual synchronizer pattern, but it matches the die.
    /*p29.BYBA*/ next.SCAN_DONE_d4.tock(clk_sig2.XUPY_xBCxxFGx, SCAN_RSTo, spr.SCAN_DONE_d0);
    /*p29.DOBA*/ next.SCAN_DONE_d5.tock(clk_sig1.ALET_AxCxExGx, SCAN_RSTo, spr.SCAN_DONE_d4);

    // which means this trigger is exactly 1 phase long
    /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or(SCAN_RSTa, spr.SCAN_DONE_d5, !spr.SCAN_DONE_d4);
    /*p29.AVAP*/ next.SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);
  }
}

//-----------------------------------------------------------------------------

};