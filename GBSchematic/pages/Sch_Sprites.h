#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct OamSignals;
struct OamRegisters;
struct OamPins;
struct DmaRegisters;

//-----------------------------------------------------------------------------

struct SpriteSignals {
  /*p28.ACYL*/ bool ACYL_OAM_ADDR_PARSE;
  /*p29.PUCO*/ bool PUCO_SPRITE_PIX_LATCH_B;
  /*p29.XADO*/ bool XADO_SPRITE_PIX_LATCH_A;

  /*p29.WUTY*/ bool WUTY_SPRITE_DONE; // -> sst
  /*p29.DYTY*/ bool DYTY_STORE_EN;    // -> sst
  /*p29.AROR*/ bool AROR_MATCH_EN;    // -> sst

  /*p29.TEXY*/ bool TEXY_SPRITE_READ;

  /*p28.FETO*/ bool FETO_SCAN_DONE_d0;
  /*p29.AVAP*/ bool AVAP_SCAN_DONE_d0_TRIG;

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

struct SpriteDelta {
  /*p29.GESE*/ wire SPR_MATCH_Y;
  /*p29.DEGE*/ wire SPRITE_DELTA0;
  /*p29.DABY*/ wire SPRITE_DELTA1;
  /*p29.DABU*/ wire SPRITE_DELTA2;
  /*p29.GYSA*/ wire SPRITE_DELTA3;
  /*p29.GACE*/ wire SPRITE_DELTA4;
  /*p29.GUVU*/ wire SPRITE_DELTA5;
  /*p29.GYDA*/ wire SPRITE_DELTA6;
  /*p29.GEWY*/ wire SPRITE_DELTA7;
};

//-----------------------------------------------------------------------------

struct SpriteRegisters {
  void reset();
  uint32_t index() const;

  SpriteDelta storeMatchY(const SysSignals& sys_sig,
                          const LcdRegisters& lcd_reg,
                          const OamRegisters& oam_reg,
                          const ConfigRegisters& vid_reg2);

  SpriteSignals tick(const ClkSignals& clk_sig,
                     const SysSignals& sys_sig,
                     const VclkSignals& vid_clk,
                     const RstSignals& rst_sig,
                     const DmaRegisters& dma_reg,
                     const LcdSignals& lcd_sig,
                     const LcdRegisters& lcd_reg,
                     const ConfigRegisters& vid_cfg,
                     const VidRegisters& vid_reg,
                     const OamRegisters& oam_reg,
                     const OamPins& oam_pins,
                     SpriteTristate& sil_out,
                     bool STORE_MATCH);

  bool MATCH_EN(const VidRegisters& vid_reg,
                const ConfigRegisters& vid_reg2) const;

  void sprite_seq(const SysSignals& sys_sig,
                  const ClkSignals& clk_sig,
                  const RstSignals& rst_sig,
                  const VidSignals& vid_sig,
                  const VidRegisters& vid_reg,
                  const LcdSignals& lcd_sig);

  wire scanner(const ClkSignals& clk_sig, const VclkSignals& vid_clk, const RstSignals& rst_sig, const LcdSignals& lcd_sig);

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