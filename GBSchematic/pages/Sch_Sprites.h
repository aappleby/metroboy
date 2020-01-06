#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct OamSignals;
struct OamPins;

//-----------------------------------------------------------------------------

struct SpriteSignals {
  /*p28.WEFE*/ bool P10_Bn;
  /*p28.ACYL*/ bool OAM_ADDR_PARSE;
  /*p29.PUCO*/ bool SPRITE_PIX_LATCH_B;
  /*p29.XADO*/ bool SPRITE_PIX_LATCH_A;

  /*p29.WUTY*/ bool SPRITE_DONE;
  
  /*p29.DYTY*/ bool STORE_EN;

  /*p29.AROR*/ bool MATCH_EN;

  /*p29.TEXY*/ bool SPRITE_READ;

  /*p28.FETO*/ bool SCAN_DONE_d0;
  /*p29.AVAP*/ bool SCAN_DONE_d0_TRIG;

  /*p29.TOXE*/ bool SPR_SEQ0;
  /*p29.TULY*/ bool SPR_SEQ1;
  /*p29.TESE*/ bool SPR_SEQ2;
  /*p29.VONU*/ bool SEQ_xxx34xn;

  /*p29.TYFO*/ bool SEQ_B0d;

  /*p28.YFEL*/ bool SCAN0;
  /*p28.WEWY*/ bool SCAN1;
  /*p28.GOSO*/ bool SCAN2;
  /*p28.ELYN*/ bool SCAN3;
  /*p28.FAHA*/ bool SCAN4;
  /*p28.FONY*/ bool SCAN5;
};

//-----------------------------------------------------------------------------

struct SpriteRegisters {
  void reset();
  uint32_t index() const;

  SpriteSignals tick(const ClkSignals& clk_sig1,
                     const VclkSignals& vid_clk,
                     const VrstSignals& vid_rst,
                     const JoypadPins& joy_pins,
                     const DmaSignals& dma_sig,
                     const LcdSignals& lcd_sig,
                     const VidRegisters2& vid_reg,
                     const VidSignals& vid_sig,
                     const SpriteStoreSignals& sst_sig,
                     const OamSignals& oam_sig,
                     const OamPins& oam_pins,
                     SpriteTristate& sil_out);

  /*p29.CENO*/ Reg2 STORE_SPRITE_IDXn;

  /*p28.YFEL*/ Reg2 SCAN0;
  /*p28.WEWY*/ Reg2 SCAN1;
  /*p28.GOSO*/ Reg2 SCAN2;
  /*p28.ELYN*/ Reg2 SCAN3;
  /*p28.FAHA*/ Reg2 SCAN4;
  /*p28.FONY*/ Reg2 SCAN5;

  /*p30.XADU*/ Reg2 SPRITE_IDX0;
  /*p30.XEDY*/ Reg2 SPRITE_IDX1;
  /*p30.ZUZE*/ Reg2 SPRITE_IDX2;
  /*p30.XOBE*/ Reg2 SPRITE_IDX3;
  /*p30.YDUF*/ Reg2 SPRITE_IDX4;
  /*p30.XECU*/ Reg2 SPRITE_IDX5;

  /*p29.BYBA*/ Reg2 SCAN_DONE_d4;
  /*p29.DOBA*/ Reg2 SCAN_DONE_d5;

  /*p29.TOXE*/ Reg2 SPR_SEQ0;
  /*p29.TULY*/ Reg2 SPR_SEQ1;
  /*p29.TESE*/ Reg2 SPR_SEQ2;
  
  /*p29.TOBU*/ Reg2 SEQ_xx23xx;
  /*p29.VONU*/ Reg2 SEQ_xxx34xn;
  /*p29.SEBA*/ Reg2 SEQ_xxxx45n;
  /*p29.TYFO*/ Reg2 SEQ_B0d;

};

//-----------------------------------------------------------------------------

};