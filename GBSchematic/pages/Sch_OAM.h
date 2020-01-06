#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct VramBus;
struct OamPins;

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamPins {
  /*p25.COTA*/ bool CLK_A;
  /*p28.ZODO*/ bool CLK_B;
  /*p28.ZONE*/ bool WR_A;
  /*p28.ZOFE*/ bool WR_B;

  /*p28.GEKA*/ bool A0;
  /*p28.ZYFO*/ bool A1;
  /*p28.YFOT*/ bool A2;
  /*p28.YFOC*/ bool A3;
  /*p28.YVOM*/ bool A4;
  /*p28.YMEV*/ bool A5;
  /*p28.XEMU*/ bool A6;
  /*p28.YZET*/ bool A7;

  bool A_D0;
  bool A_D1;
  bool A_D2;
  bool A_D3;
  bool A_D4;
  bool A_D5;
  bool A_D6;
  bool A_D7;

  bool B_D0;
  bool B_D1;
  bool B_D2;
  bool B_D3;
  bool B_D4;
  bool B_D5;
  bool B_D6;
  bool B_D7;
};

//-----------------------------------------------------------------------------

struct OamSignals {
  /*p28.BODE*/ bool OAM_LATCH;

  /*p31.ZAGO*/ bool OAM_A_D0b;
  /*p31.ZOCY*/ bool OAM_A_D1b;
  /*p31.YPUR*/ bool OAM_A_D2b;
  /*p31.YVOK*/ bool OAM_A_D3b;
  /*p31.COSE*/ bool OAM_A_D4b;
  /*p31.AROP*/ bool OAM_A_D5b;
  /*p31.XATU*/ bool OAM_A_D6b;
  /*p31.BADY*/ bool OAM_A_D7b;

  /*p29.XUSO*/ bool OAM_B_D0;
  /*p29.XEGU*/ bool OAM_B_D1;
  /*p29.YJEX*/ bool OAM_B_D2;
  /*p29.XYJU*/ bool OAM_B_D3;
  /*p29.YBOG*/ bool OAM_B_D4;
  /*p29.WYSO*/ bool OAM_B_D5;
  /*p29.XOTE*/ bool OAM_B_D6;
  /*p29.YZAB*/ bool OAM_B_D7;

  /*p31.YLOR*/ bool OAM_A_D0;
  /*p31.ZYTY*/ bool OAM_A_D1;
  /*p31.ZYVE*/ bool OAM_A_D2;
  /*p31.ZEZY*/ bool OAM_A_D3;
  /*p31.GOMO*/ bool OAM_A_D4;
  /*p31.BAXO*/ bool OAM_A_D5;
  /*p31.YZOS*/ bool OAM_A_D6;
  /*p31.DEPO*/ bool OAM_A_D7;
};

//-----------------------------------------------------------------------------

struct OamRegisters {

  OamSignals tick(const Cpu& cpu,
                  const ClkSignals& clk_sig,
                  const BusSignals& bus_sig,
                  const CartPins& cart_pins,
                  const JoypadPins& joy_pins,
                  const VclkSignals& vid_clk,
                  const DecoderSignals& dec_sig,
                  const SpriteSignals& spr_sig,
                  const DmaSignals& dma_sig,
                  const VidSignals& vid_sig,
                  const SpriteTristate& sil,
                  const VramBus& vram_tri,
                  BusTristates& bus_tri,
                  OamPins& oam_pins);

  /*p31.XYKY*/ Reg2 OAM_A_LATCH0;
  /*p31.YRUM*/ Reg2 OAM_A_LATCH1;
  /*p31.YSEX*/ Reg2 OAM_A_LATCH2;
  /*p31.YVEL*/ Reg2 OAM_A_LATCH3;
  /*p31.WYNO*/ Reg2 OAM_A_LATCH4;
  /*p31.CYRA*/ Reg2 OAM_A_LATCH5;
  /*p31.ZUVE*/ Reg2 OAM_A_LATCH6;
  /*p31.ECED*/ Reg2 OAM_A_LATCH7;

  /*p29.YDYV*/ Reg2 OAM_B_LATCH0;
  /*p29.YCEB*/ Reg2 OAM_B_LATCH1;
  /*p29.ZUCA*/ Reg2 OAM_B_LATCH2;
  /*p29.WONE*/ Reg2 OAM_B_LATCH3;
  /*p29.ZAXE*/ Reg2 OAM_B_LATCH4;
  /*p29.XAFU*/ Reg2 OAM_B_LATCH5;
  /*p29.YSES*/ Reg2 OAM_B_LATCH6;
  /*p29.ZECA*/ Reg2 OAM_B_LATCH7;

  /*p31.YLOR*/ Reg2 OAM_A_D0;
  /*p31.ZYTY*/ Reg2 OAM_A_D1;
  /*p31.ZYVE*/ Reg2 OAM_A_D2;
  /*p31.ZEZY*/ Reg2 OAM_A_D3;
  /*p31.GOMO*/ Reg2 OAM_A_D4;
  /*p31.BAXO*/ Reg2 OAM_A_D5;
  /*p31.YZOS*/ Reg2 OAM_A_D6;
  /*p31.DEPO*/ Reg2 OAM_A_D7;

  /*p29.XUSO*/ Reg2 OAM_B_D0;
  /*p29.XEGU*/ Reg2 OAM_B_D1;
  /*p29.YJEX*/ Reg2 OAM_B_D2;
  /*p29.XYJU*/ Reg2 OAM_B_D3;
  /*p29.YBOG*/ Reg2 OAM_B_D4;
  /*p29.WYSO*/ Reg2 OAM_B_D5;
  /*p29.XOTE*/ Reg2 OAM_B_D6;
  /*p29.YZAB*/ Reg2 OAM_B_D7;
};

//-----------------------------------------------------------------------------

};