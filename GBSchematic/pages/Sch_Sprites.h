#pragma once
#include "Sch_Common.h"

namespace Schematics {

void Sprites_tickScanner(const ClockSignals& clk,
                         const LCD& lcd,
                         const ResetRegisters& rst,
                         const Sprites& spr,
                         Sprites& next);

//-----------------------------------------------------------------------------

struct Sprites {
  void reset() {
    P10_Bn = false;
    OAM_ADDR_PARSE = false;
    STORE_SPRITE_IDXn.val = false; STORE_SPRITE_IDXn.clk = false;

    SCAN0.val = true;  SCAN0.clk = true;
    SCAN1.val = true;  SCAN1.clk = false;
    SCAN2.val = true;  SCAN2.clk = false;
    SCAN3.val = false; SCAN3.clk = false;
    SCAN4.val = false; SCAN4.clk = true;
    SCAN5.val = true;  SCAN5.clk = true;

    SCAN_DONE_d0 = true;

    SPRITE_IDX0.val = false; SPRITE_IDX0.clk = false;
    SPRITE_IDX1.val = false; SPRITE_IDX1.clk = false;
    SPRITE_IDX2.val = false; SPRITE_IDX2.clk = false;
    SPRITE_IDX3.val = false; SPRITE_IDX3.clk = false;
    SPRITE_IDX4.val = false; SPRITE_IDX4.clk = false;
    SPRITE_IDX5.val = false; SPRITE_IDX5.clk = false;

    SCAN_DONE_d4.val = true; SCAN_DONE_d4.clk = false;
    SCAN_DONE_d5.val = true; SCAN_DONE_d5.clk = true;
    SCAN_DONE_d0_TRIG = false;

    SPR_SEQ0.val = false; SPR_SEQ0.clk = false;
    SPR_SEQ1.val = false; SPR_SEQ1.clk = false;
    SPR_SEQ2.val = false; SPR_SEQ2.clk = false;

    SEQ_xx23xx.val  = false; SEQ_xx23xx.clk  = false;
    SEQ_xxx34xn.val = false; SEQ_xxx34xn.clk = false;
    SEQ_xxxx45n.val = false; SEQ_xxxx45n.clk = false;
    SEQ_B0d.val = false; SEQ_B0d.clk = false;

    SPRITE_PIX_LATCH_B = false;
    SPRITE_PIX_LATCH_A = false;
    SPRITE_DONE = false;
    STORE_EN = false;
    MATCH_EN = false;
    SPRITE_READ = false;
  }

  uint32_t index() const {
    return (SCAN0 << 0) | (SCAN1 << 1) | (SCAN2 << 2) | (SCAN3 << 3) | (SCAN4 << 4) | (SCAN5 << 5);
  }

  /*p28.WEFE*/ bool P10_Bn;
  /*p28.ACYL*/ bool OAM_ADDR_PARSE;
  /*p29.CENO*/ Reg STORE_SPRITE_IDXn;

  /*p28.YFEL*/ Reg SCAN0;
  /*p28.WEWY*/ Reg SCAN1;
  /*p28.GOSO*/ Reg SCAN2;
  /*p28.ELYN*/ Reg SCAN3;
  /*p28.FAHA*/ Reg SCAN4;
  /*p28.FONY*/ Reg SCAN5;

  /*p28.FETO*/ bool SCAN_DONE_d0;

  /*p30.XADU*/ Reg SPRITE_IDX0;
  /*p30.XEDY*/ Reg SPRITE_IDX1;
  /*p30.ZUZE*/ Reg SPRITE_IDX2;
  /*p30.XOBE*/ Reg SPRITE_IDX3;
  /*p30.YDUF*/ Reg SPRITE_IDX4;
  /*p30.XECU*/ Reg SPRITE_IDX5;

  /*p29.BYBA*/ Reg SCAN_DONE_d4;
  /*p29.DOBA*/ Reg SCAN_DONE_d5;
  /*p29.AVAP*/ bool SCAN_DONE_d0_TRIG;

  /*p29.TOXE*/ Reg SPR_SEQ0;
  /*p29.TULY*/ Reg SPR_SEQ1;
  /*p29.TESE*/ Reg SPR_SEQ2;
  
  /*p29.TOBU*/ Reg SEQ_xx23xx;
  /*p29.VONU*/ Reg SEQ_xxx34xn;
  /*p29.SEBA*/ Reg SEQ_xxxx45n;

  /*p29.TYFO*/ Reg SEQ_B0d;

  /*p29.PUCO*/ bool SPRITE_PIX_LATCH_B;
  /*p29.XADO*/ bool SPRITE_PIX_LATCH_A;

  /*p29.WUTY*/ bool SPRITE_DONE;
  
  /*p29.DYTY*/ bool STORE_EN;

  /*p29.AROR*/ bool MATCH_EN;

  /*p29.TEXY*/ bool SPRITE_READ;
};

//-----------------------------------------------------------------------------

};