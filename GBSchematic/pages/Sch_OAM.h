#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct VramBus;
struct OamPins;
struct DmaRegisters;

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamPins {
  /*p25.COTA*/ bool CLK_A;
  /*p28.ZODO*/ bool OE;
  /*p28.ZONE*/ bool WR_A; // definitely write
  /*p28.ZOFE*/ bool WR_B; // definitely write

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

struct OamRegisters {

  void tick(const SysSignals& sys_sig,
            const CpuPins& cpu_pins,
            const ClkSignals& clk_sig,
            const BusSignals& bus_sig,
            const CartPins& cart_pins,
            const VclkSignals& vid_clk,
            const DecoderSignals& dec_sig,
            const SpriteSignals& spr_sig,
            const DmaSignals& dma_sig,
            const DmaRegisters& dma_reg,
            const VidRegisters& vid_reg,
            const SpriteTristate& sil,
            const VramBus& vram_tri,
            BusTristates& bus_tri,
            OamPins& oam_pins);

  /*p31.XYKY*/ Reg2 LATCH_OAM_A0;
  /*p31.YRUM*/ Reg2 LATCH_OAM_A1;
  /*p31.YSEX*/ Reg2 LATCH_OAM_A2;
  /*p31.YVEL*/ Reg2 LATCH_OAM_A3;
  /*p31.WYNO*/ Reg2 LATCH_OAM_A4;
  /*p31.CYRA*/ Reg2 LATCH_OAM_A5;
  /*p31.ZUVE*/ Reg2 LATCH_OAM_A6;
  /*p31.ECED*/ Reg2 LATCH_OAM_A7;

  /*p29.YDYV*/ Reg2 LATCH_OAM_B0;
  /*p29.YCEB*/ Reg2 LATCH_OAM_B1;
  /*p29.ZUCA*/ Reg2 LATCH_OAM_B2;
  /*p29.WONE*/ Reg2 LATCH_OAM_B3;
  /*p29.ZAXE*/ Reg2 LATCH_OAM_B4;
  /*p29.XAFU*/ Reg2 LATCH_OAM_B5;
  /*p29.YSES*/ Reg2 LATCH_OAM_B6;
  /*p29.ZECA*/ Reg2 LATCH_OAM_B7;

  /*p31.YLOR*/ Reg2 REG_OAM_A0;
  /*p31.ZYTY*/ Reg2 REG_OAM_A1;
  /*p31.ZYVE*/ Reg2 REG_OAM_A2;
  /*p31.ZEZY*/ Reg2 REG_OAM_A3;
  /*p31.GOMO*/ Reg2 REG_OAM_A4;
  /*p31.BAXO*/ Reg2 REG_OAM_A5;
  /*p31.YZOS*/ Reg2 REG_OAM_A6;
  /*p31.DEPO*/ Reg2 REG_OAM_A7;

  /*p29.XUSO*/ Reg2 REG_OAM_B0;
  /*p29.XEGU*/ Reg2 REG_OAM_B1;
  /*p29.YJEX*/ Reg2 REG_OAM_B2;
  /*p29.XYJU*/ Reg2 REG_OAM_B3;
  /*p29.YBOG*/ Reg2 REG_OAM_B4;
  /*p29.WYSO*/ Reg2 REG_OAM_B5;
  /*p29.XOTE*/ Reg2 REG_OAM_B6;
  /*p29.YZAB*/ Reg2 REG_OAM_B7;
};

//-----------------------------------------------------------------------------

};