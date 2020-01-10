#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct VidSignals {
  /*p21.VOTY*/ bool VOTY_INT_STAT;
  /*p26.ACEN*/ bool ACEN_BG_MAP_READ;
  /*p25.XEZE*/ bool XEZE_WIN_MAP_READ;
  /*p26.BEJE*/ bool BEJE_BG_TILE_READ;
  /*p27.XUHA*/ bool XUHA_TILE_READ_AB;
  /*p25.XUCY*/ bool XUCY_WIN_TILE_READ;
  /*p27.NETA*/ bool NETA_TILE_READ;
  /*p27.RYCE*/ bool RYCE_SPRITE_FETCH_TRIG;
};

//-----------------------------------------------------------------------------

struct VidRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  VidSignals tick(const SysSignals& sys_sig,
                  const ClkSignals& clk_sig,
                  const RstSignals& rst_sig1,
                  const VclkSignals& vid_clk,
                  const BusSignals& bus_sig,
                  const DecoderSignals& dec_sig,
                  const LcdSignals& lcd_sig,
                  const LcdRegisters& lcd_reg,
                  const SpriteSignals& spr_sig,
                  const SpriteStoreSignals& sst_sig,
                  BusTristates& bus_tri,
                  bool STORE_MATCH,
                  bool AVAP_SCAN_DONE_d0_TRIG,
                  bool WUTY_SPRITE_DONE);
                  

  void lyMatch(const BusSignals& bus_sig,
               const DecoderSignals& dec_sig,
               const VclkSignals& vid_clk,
               const RstSignals& rst_sig,
               const LcdRegisters& lcd_reg,
               const ConfigRegisters& vid_reg2);

  bool winMatch(const ClkSignals& clk_sig,
                const VclkSignals& vid_clk,
                const RstSignals& rst_sig,
                const LcdSignals& lcd_sig,
                const LcdRegisters& lcd_reg,
                const ConfigRegisters& vid_reg2,
                bool SEGU_4M,
                bool ROCO_4M);

  void pixelCounter(const RstSignals& rst_sig, const LcdSignals& lcd_sig, bool CLKPIPE);

  void mapCounter(const RstSignals& rst_sig,
                  const LcdSignals& lcd_sig,
                  const ConfigRegisters& vid_reg2,
                  bool MAP_X_CLK_STOPn);

  void fineMatch(const ClkSignals& clk_sig,
                 const ConfigRegisters& vid_reg2,
                 bool MAP_X_CLK_STOPn,
                 bool STORE_MATCH);

  /*p??.ROXY*/ Reg2 FINE_MATCH_DUMP;
  /*p??.PUXA*/ Reg2 FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg2 FINE_MATCH_SYNC2;
  /*p27.RYKU*/ Reg2 FINE_CNT0;
  /*p27.ROGA*/ Reg2 FINE_CNT1;
  /*p27.RUBU*/ Reg2 FINE_CNT2;

  //----------

  // x counter
  /*p21.XEHO*/ Reg2 X0;
  /*p21.SAVY*/ Reg2 X1;
  /*p21.XODU*/ Reg2 X2;
  /*p21.XYDO*/ Reg2 X3;
  /*p21.TUHU*/ Reg2 X4;
  /*p21.TUKY*/ Reg2 X5;
  /*p21.TAKO*/ Reg2 X6;
  /*p21.SYBE*/ Reg2 X7;


  /*p27.NOPA*/ Reg2 NOPA_WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg2 SOVY_WIN_MODE_SYNC;

  /*p21.XYMU*/ Reg2 XYMU_RENDERING_LATCH;

  /*p21.VOGA*/ Reg2 RENDER_DONE_SYNC;

  /*p27.PYNU*/ Reg2 WIN_MODE_LATCH1;
  /*p27.RYDY*/ Reg2 RYDY_WIN_MODE_LATCH;

  /*p21.ROXE*/ Reg2 INT_HBL_EN;
  /*p21.RUFO*/ Reg2 INT_VBL_EN;
  /*p21.REFE*/ Reg2 INT_OAM_EN;
  /*p21.RUGU*/ Reg2 INT_LYC_EN;

  /*p21.ROPO*/ Reg2 ROPO_INT_LYC;
  /*p21.RUPO*/ Reg2 RUPO_LATCH_LYC_MATCH;

  /*p27.SARY*/ Reg2 WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg2 WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg2 WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg2 WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg2 WIN_MATCH_SYNC2;

  /*p27.LONY*/ Reg2 LONY_LATCH;

  /*p27.LAXU*/ Reg2 BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ Reg2 BG_SEQ_xx23xx67;
  /*p27.NYVA*/ Reg2 BG_SEQ_xxxx4567;

  /*p27.LOVY*/ Reg2 BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg2 BG_SEQ_6;
  /*p24.PORY*/ Reg2 BG_SEQ_7;
  /*p27.LYZU*/ Reg2 BG_SEQ_x1x3x5x7_DELAY;

  /*p27.WYKA*/ Reg2 MAP_X0;
  /*p27.WODY*/ Reg2 MAP_X1;
  /*p27.WOBO*/ Reg2 MAP_X2;
  /*p27.WYKO*/ Reg2 MAP_X3;
  /*p27.XOLO*/ Reg2 MAP_X4;

  /*p27.VYNO*/ Reg2 TILE_Y0;
  /*p27.VUJO*/ Reg2 TILE_Y1;
  /*p27.VYMU*/ Reg2 TILE_Y2;

  /*p27.TUFU*/ Reg2 MAP_Y0;
  /*p27.TAXA*/ Reg2 MAP_Y1;
  /*p27.TOZO*/ Reg2 MAP_Y2;
  /*p27.TATE*/ Reg2 MAP_Y3;
  /*p27.TEKE*/ Reg2 MAP_Y4;

  /*p24.PYGO*/ Reg2 PYGO_TILE_DONE;
  /*p24.POKY*/ Reg2 POKY_FRONT_PORCH_LATCHn;

private:

  /*p27.TAKA*/ Reg2 _SPRITE_FETCH_LATCH;
  /*p27.SOBU*/ Reg2 _SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg2 _SPRITE_FETCH_SYNC2;
};

//-----------------------------------------------------------------------------

};