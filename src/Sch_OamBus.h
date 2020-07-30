#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void tock(SchematicTop& top);

  //----------------------------------------

  uint8_t get_bus_addr() {
    return ~(uint8_t)pack(OAM_BUS_A0, OAM_BUS_A1, OAM_BUS_A2, OAM_BUS_A3,
                          OAM_BUS_A4, OAM_BUS_A5, OAM_BUS_A6, OAM_BUS_A7);
  }

  uint16_t get_bus_data() {
    return ~(uint16_t)pack(OAM_BUS_DA0, OAM_BUS_DA1, OAM_BUS_DA2, OAM_BUS_DA3,
                           OAM_BUS_DA4, OAM_BUS_DA5, OAM_BUS_DA6, OAM_BUS_DA7,
                           OAM_BUS_DB0, OAM_BUS_DB1, OAM_BUS_DB2, OAM_BUS_DB3,
                           OAM_BUS_DB4, OAM_BUS_DB5, OAM_BUS_DB6, OAM_BUS_DB7);
  }

  void preset_bus_data(bool oe, uint16_t data) {
    if (oe) {
      OAM_BUS_DA0.preset(data & 0x0001);
      OAM_BUS_DA1.preset(data & 0x0002);
      OAM_BUS_DA2.preset(data & 0x0004);
      OAM_BUS_DA3.preset(data & 0x0008);
      OAM_BUS_DA4.preset(data & 0x0010);
      OAM_BUS_DA5.preset(data & 0x0020);
      OAM_BUS_DA6.preset(data & 0x0040);
      OAM_BUS_DA7.preset(data & 0x0080);
      OAM_BUS_DB0.preset(data & 0x0100);
      OAM_BUS_DB1.preset(data & 0x0200);
      OAM_BUS_DB2.preset(data & 0x0400);
      OAM_BUS_DB3.preset(data & 0x0800);
      OAM_BUS_DB4.preset(data & 0x1000);
      OAM_BUS_DB5.preset(data & 0x2000);
      OAM_BUS_DB6.preset(data & 0x4000);
      OAM_BUS_DB7.preset(data & 0x8000);
    }
    else {
      OAM_BUS_DA0.preset(DELTA_TRIZ);
      OAM_BUS_DA1.preset(DELTA_TRIZ);
      OAM_BUS_DA2.preset(DELTA_TRIZ);
      OAM_BUS_DA3.preset(DELTA_TRIZ);
      OAM_BUS_DA4.preset(DELTA_TRIZ);
      OAM_BUS_DA5.preset(DELTA_TRIZ);
      OAM_BUS_DA6.preset(DELTA_TRIZ);
      OAM_BUS_DA7.preset(DELTA_TRIZ);
      OAM_BUS_DB0.preset(DELTA_TRIZ);
      OAM_BUS_DB1.preset(DELTA_TRIZ);
      OAM_BUS_DB2.preset(DELTA_TRIZ);
      OAM_BUS_DB3.preset(DELTA_TRIZ);
      OAM_BUS_DB4.preset(DELTA_TRIZ);
      OAM_BUS_DB5.preset(DELTA_TRIZ);
      OAM_BUS_DB6.preset(DELTA_TRIZ);
      OAM_BUS_DB7.preset(DELTA_TRIZ);
    }
  }

  //----------------------------------------

  void dump(Dumper& d) {
    d("---------- OAM Bus  ----------\n");
    d("PIN_CLK    %c\n", OAM_PIN_CLK.c());
    d("PIN_OE     %c\n", OAM_PIN_OE.c()); 
    d("PIN_WR_A   %c\n", OAM_PIN_WR_A.c());
    d("PIN_WR_B   %c\n", OAM_PIN_WR_B.c());

    d("OAM_TRI    %c%c%c%c%c%c%c%c\n", 
      OAM_TRI_A7.c(), OAM_TRI_A6.c(), OAM_TRI_A5.c(), OAM_TRI_A4.c(),  OAM_TRI_A3.c(),
      OAM_TRI_A2.c(), OAM_TRI_A1.c(), OAM_TRI_A0.c());
    d("LATCH_A    %c%c%c%c%c%c%c%c\n", 
      ECED_LATCH_OAM_DA7.c(), ZUVE_LATCH_OAM_DA6.c(), CYRA_LATCH_OAM_DA5.c(), WYNO_LATCH_OAM_DA4.c(),
      YVEL_LATCH_OAM_DA3.c(), YSEX_LATCH_OAM_DA2.c(), YRUM_LATCH_OAM_DA1.c(), XYKY_LATCH_OAM_DA0.c());
    d("LATCH_B    %c%c%c%c%c%c%c%c\n", 
      ZECA_LATCH_OAM_DB7.c(), YSES_LATCH_OAM_DB6.c(), XAFU_LATCH_OAM_DB5.c(), ZAXE_LATCH_OAM_DB4.c(),
      WONE_LATCH_OAM_DB3.c(), ZUCA_LATCH_OAM_DB2.c(), YCEB_LATCH_OAM_DB1.c(), YDYV_LATCH_OAM_DB0.c());
    d("BUS ADDR   %c%c%c%c%c%c%c%c\n", 
      OAM_BUS_A7.c(), OAM_BUS_A6.c(), OAM_BUS_A5.c(), OAM_BUS_A4.c(),
      OAM_BUS_A3.c(), OAM_BUS_A2.c(), OAM_BUS_A1.c(), OAM_BUS_A0.c());
    d("BUS DATA A %c%c%c%c%c%c%c%c\n", 
      OAM_BUS_DA7.c(), OAM_BUS_DA6.c(), OAM_BUS_DA5.c(), OAM_BUS_DA4.c(),
      OAM_BUS_DA3.c(), OAM_BUS_DA2.c(), OAM_BUS_DA1.c(), OAM_BUS_DA0.c());
    d("BUS DATA B %c%c%c%c%c%c%c%c\n", 
      OAM_BUS_DB7.c(), OAM_BUS_DB6.c(), OAM_BUS_DB5.c(), OAM_BUS_DB4.c(),
      OAM_BUS_DB3.c(), OAM_BUS_DB2.c(), OAM_BUS_DB1.c(), OAM_BUS_DB0.c());
    d("\n");
  }

  //----------------------------------------

  // -> sprite store
  /*p28.YFOT*/ wire YFOT_OAM_A2p() const { return not(OAM_TRI_A2); }
  /*p28.YFOC*/ wire YFOC_OAM_A3p() const { return not(OAM_TRI_A3); }
  /*p28.YVOM*/ wire YVOM_OAM_A4p() const { return not(OAM_TRI_A4); }
  /*p28.YMEV*/ wire YMEV_OAM_A5p() const { return not(OAM_TRI_A5); }
  /*p28.XEMU*/ wire XEMU_OAM_A6p() const { return not(OAM_TRI_A6); }
  /*p28.YZET*/ wire YZET_OAM_A7p() const { return not(OAM_TRI_A7); }

  /*p31.YLOR*/ Reg YLOR_OAM_DA0 = REG_D0C0; // sprite x bit 0, 
  /*p31.ZYTY*/ Reg ZYTY_OAM_DA1 = REG_D0C0; // sprite x bit 1, 
  /*p31.ZYVE*/ Reg ZYVE_OAM_DA2 = REG_D0C0; // sprite x bit 2, 
  /*p31.ZEZY*/ Reg ZEZY_OAM_DA3 = REG_D0C0; // sprite x bit 3, 
  /*p31.GOMO*/ Reg GOMO_OAM_DA4 = REG_D0C0; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ Reg BAXO_OAM_DA5 = REG_D0C0; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ Reg YZOS_OAM_DA6 = REG_D0C0; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ Reg DEPO_OAM_DA7 = REG_D0C0; // sprite x bit 7, sprite priority

  /*p29.XUSO*/ Reg XUSO_OAM_DB0 = REG_D0C0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ Reg XEGU_OAM_DB1 = REG_D0C0; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ Reg YJEX_OAM_DB2 = REG_D0C0; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ Reg XYJU_OAM_DB3 = REG_D0C0; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ Reg YBOG_OAM_DB4 = REG_D0C0; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ Reg WYSO_OAM_DB5 = REG_D0C0; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ Reg XOTE_OAM_DB6 = REG_D0C0; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ Reg YZAB_OAM_DB7 = REG_D0C0; // sprite y bit 7, sprite tile index bit 7

//private:

  /*p04.MAKA*/ Reg _MAKA_DV_SYNC = REG_D0C0;

  Tri OAM_PIN_CLK  = TRI_HZNP;
  Tri OAM_PIN_OE   = TRI_HZPU;
  Tri OAM_PIN_WR_A = TRI_HZPU;
  Tri OAM_PIN_WR_B = TRI_HZPU;

  Tri OAM_TRI_A0 = TRI_HZNP;
  Tri OAM_TRI_A1 = TRI_HZNP;
  Tri OAM_TRI_A2 = TRI_HZNP;
  Tri OAM_TRI_A3 = TRI_HZNP;
  Tri OAM_TRI_A4 = TRI_HZNP;
  Tri OAM_TRI_A5 = TRI_HZNP;
  Tri OAM_TRI_A6 = TRI_HZNP;
  Tri OAM_TRI_A7 = TRI_HZNP;

  /*p31.XYKY*/ Tri XYKY_LATCH_OAM_DA0 = TRI_D0NP;
  /*p31.YRUM*/ Tri YRUM_LATCH_OAM_DA1 = TRI_D0NP;
  /*p31.YSEX*/ Tri YSEX_LATCH_OAM_DA2 = TRI_D0NP;
  /*p31.YVEL*/ Tri YVEL_LATCH_OAM_DA3 = TRI_D0NP;
  /*p31.WYNO*/ Tri WYNO_LATCH_OAM_DA4 = TRI_D0NP;
  /*p31.CYRA*/ Tri CYRA_LATCH_OAM_DA5 = TRI_D0NP;
  /*p31.ZUVE*/ Tri ZUVE_LATCH_OAM_DA6 = TRI_D0NP;
  /*p31.ECED*/ Tri ECED_LATCH_OAM_DA7 = TRI_D0NP;

  /*p29.YDYV*/ Tri YDYV_LATCH_OAM_DB0 = TRI_D0NP;
  /*p29.YCEB*/ Tri YCEB_LATCH_OAM_DB1 = TRI_D0NP;
  /*p29.ZUCA*/ Tri ZUCA_LATCH_OAM_DB2 = TRI_D0NP;
  /*p29.WONE*/ Tri WONE_LATCH_OAM_DB3 = TRI_D0NP;
  /*p29.ZAXE*/ Tri ZAXE_LATCH_OAM_DB4 = TRI_D0NP;
  /*p29.XAFU*/ Tri XAFU_LATCH_OAM_DB5 = TRI_D0NP;
  /*p29.YSES*/ Tri YSES_LATCH_OAM_DB6 = TRI_D0NP;
  /*p29.ZECA*/ Tri ZECA_LATCH_OAM_DB7 = TRI_D0NP;

  Tri OAM_BUS_A0  = TRI_HZNP;
  Tri OAM_BUS_A1  = TRI_HZNP;
  Tri OAM_BUS_A2  = TRI_HZNP;
  Tri OAM_BUS_A3  = TRI_HZNP;
  Tri OAM_BUS_A4  = TRI_HZNP;
  Tri OAM_BUS_A5  = TRI_HZNP;
  Tri OAM_BUS_A6  = TRI_HZNP;
  Tri OAM_BUS_A7  = TRI_HZNP;

  Tri OAM_BUS_DA0 = TRI_HZNP;
  Tri OAM_BUS_DA1 = TRI_HZNP;
  Tri OAM_BUS_DA2 = TRI_HZNP;
  Tri OAM_BUS_DA3 = TRI_HZNP;
  Tri OAM_BUS_DA4 = TRI_HZNP;
  Tri OAM_BUS_DA5 = TRI_HZNP;
  Tri OAM_BUS_DA6 = TRI_HZNP;
  Tri OAM_BUS_DA7 = TRI_HZNP;

  Tri OAM_BUS_DB0 = TRI_HZNP;
  Tri OAM_BUS_DB1 = TRI_HZNP;
  Tri OAM_BUS_DB2 = TRI_HZNP;
  Tri OAM_BUS_DB3 = TRI_HZNP;
  Tri OAM_BUS_DB4 = TRI_HZNP;
  Tri OAM_BUS_DB5 = TRI_HZNP;
  Tri OAM_BUS_DB6 = TRI_HZNP;
  Tri OAM_BUS_DB7 = TRI_HZNP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics