#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct VidSignals {
  /*p21.VOTY*/ bool INT_STAT;
  /*p21.XYMU*/ bool RENDERING_LATCH;
  /*p24.LOBY*/ bool RENDERINGn;
  /*p21.TADY*/ bool X_RST;

  /*p21.XEHO*/ bool X0;
  /*p21.SAVY*/ bool X1;
  /*p21.XODU*/ bool X2;
  /*p21.XYDO*/ bool X3;
  /*p21.TUHU*/ bool X4;
  /*p21.TUKY*/ bool X5;
  /*p21.TAKO*/ bool X6;
  /*p21.SYBE*/ bool X7;

  /*p21.ACAM*/ bool X0n;
  /*p21.AZUB*/ bool X1n;
  /*p21.AMEL*/ bool X2n;
  /*p21.AHAL*/ bool X3n;
  /*p21.APUX*/ bool X4n;
  /*p21.ABEF*/ bool X5n;
  /*p21.ADAZ*/ bool X6n;
  /*p21.ASAH*/ bool X7n;

  /*p21.XUGU*/ bool X_167n;
  /*p21.XANO*/ bool X_167;

  /*p27.PYNU*/ bool WIN_MODE_LATCH;
  /*p27.NOCU*/ bool WIN_MODE_NOCUn;
  /*p27.PORE*/ bool WIN_MODE_PORE;
  /*p27.WAZY*/ bool Y_CLK;
  /*p26.AXAD*/ bool WIN_MODE_AXADn;

  /*p27.NUNY*/ bool WIN_MODE_TRIG;
  /*p27.RYDY*/ bool WIN_MODE_LATCH2;

  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;
  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.ROXO*/ bool ROXO_4M;
  /*p27.ROCO*/ bool ROCO_4M;
  /*p24.SACU*/ bool CLKPIPE;

  /*p32.LOMA*/ bool BG_LATCH;
  /*p32.LABU*/ bool VRAM_TEMP_CLK;

  /*p21.WODU*/ bool RENDER_DONE;

  /*p27.SEKO*/ bool WIN_TRIGGER;
  /*p27.NUKO*/ bool WIN_MATCH;

  /*p27.LYRY*/ bool BG_SEQ_5;
  /*p27.NOGU*/ bool BG_SEQ_xx234567;

  /*p27.LENA*/ bool BG_READ_VRAM;

  /*p25.XEZE*/ bool WIN_MAP_READ;
  /*p26.ACEN*/ bool BG_MAP_READ;

  /*p27.XUHA*/ bool TILE_READ_AB;

  /*p27.NETA*/ bool TILE_READ;
  /*p25.XUCY*/ bool WIN_TILE_READ;
  /*p26.BEJE*/ bool BG_TILE_READ;

  /*p27.SYLO*/ bool WIN_MODE2n;
  /*p24.TOMU*/ bool WIN_MODE2b;
  
  /*p27.ROMO*/ bool FRONT_PORCH;

  /*p27.TAVE*/ bool TAVE;

  /*p27.LONY*/ bool BG_READ_VRAM_LATCH;
  /*p27.RYCE*/ bool SPRITE_FETCH_TRIG;

  /*p27.VYNO*/ bool TILE_Y0;
  /*p27.VUJO*/ bool TILE_Y1;
  /*p27.VYMU*/ bool TILE_Y2;

  /*p27.WYKA*/ bool MAP_X0;
  /*p27.WODY*/ bool MAP_X1;
  /*p27.WOBO*/ bool MAP_X2;
  /*p27.WYKO*/ bool MAP_X3;
  /*p27.XOLO*/ bool MAP_X4;

  /*p27.TUFU*/ bool MAP_Y0;
  /*p27.TAXA*/ bool MAP_Y1;
  /*p27.TOZO*/ bool MAP_Y2;
  /*p27.TATE*/ bool MAP_Y3;
  /*p27.TEKE*/ bool MAP_Y4;

};

//-----------------------------------------------------------------------------

struct VidRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  VidSignals tick(const ClkSignals& clk_sig,
                  const RstSignals& rst_sig1,
                  const VclkSignals& vid_clk,
                  const VrstSignals& vid_rst,
                  const JoypadPins& joy_pins,
                  const BusSignals& bus_sig,
                  const DecoderSignals& dec_sig,
                  const CartPins& cart_pins,
                  const LcdSignals& lcd_sig,
                  const SpriteStoreSignals& sst_sig,
                  BusTristates& bus_tri);
                  

  /*p27.ROXY*/ bool FINE_MATCH_DUMP;

  // x counter
  /*p21.XEHO*/ Reg2 X0;
  /*p21.SAVY*/ Reg2 X1;
  /*p21.XODU*/ Reg2 X2;
  /*p21.XYDO*/ Reg2 X3;
  /*p21.TUHU*/ Reg2 X4;
  /*p21.TUKY*/ Reg2 X5;
  /*p21.TAKO*/ Reg2 X6;
  /*p21.SYBE*/ Reg2 X7;

  /*p21.ROPO*/ Reg2 LYC_MATCH;

  /*p27.NOPA*/ Reg2 WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg2 WIN_MODE_SYNC2;
  /*p27.PUKU*/ bool PUKU; // NOR SR latch

  /*p21.VOGA*/ Reg2 RENDER_DONE_SYNC;

  /*p21.ROXE*/ Reg2 INT_HBL_EN;
  /*p21.RUFO*/ Reg2 INT_VBL_EN;
  /*p21.REFE*/ Reg2 INT_OAM_EN;
  /*p21.RUGU*/ Reg2 INT_LYC_EN;


  /*p27.SARY*/ Reg2 WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg2 WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg2 WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg2 WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg2 WIN_MATCH_SYNC2;

  /*p27.PUXA*/ Reg2 FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg2 FINE_MATCH_SYNC2;
  /*p27.POVA*/ bool FINE_MATCH_TRIG;
  /*p27.RYKU*/ Reg2 FINE_CNT0;
  /*p27.ROGA*/ Reg2 FINE_CNT1;
  /*p27.RUBU*/ Reg2 FINE_CNT2;


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

  /*p24.PYGO*/ Reg2 TILE_DONE;
  /*p24.POKY*/ bool FRONT_PORCH_LATCHn;

  /*p27.TAKA*/ bool SPRITE_FETCH_LATCH;
  /*p27.SOBU*/ Reg2 SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg2 SPRITE_FETCH_SYNC2;
};

//-----------------------------------------------------------------------------

};