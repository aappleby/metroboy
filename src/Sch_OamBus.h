#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void dump(Dumper& d) const;
  void tock(SchematicTop& top);
  
  //----------------------------------------

  uint8_t get_oam_tri_data() const {
    return (uint8_t)pack_p(OAM_TRI_A0.c(), OAM_TRI_A1.c(), OAM_TRI_A2.c(), OAM_TRI_A3.c(),
                           OAM_TRI_A4.c(), OAM_TRI_A5.c(), OAM_TRI_A6.c(), OAM_TRI_A7.c());
  }

  uint8_t get_oam_bus_addr() const {
    return (uint8_t)pack_p(OAM_BUS_A1.qp(), OAM_BUS_A2.qp(), OAM_BUS_A3.qp(), OAM_BUS_A4.qp(),
                           OAM_BUS_A5.qp(), OAM_BUS_A6.qp(), OAM_BUS_A7.qp(), 0);
  }

  uint8_t get_oam_latch_data_a() const {
    return (uint8_t)pack_p(YDYV_LATCH_OAM_DA0.c(), YCEB_LATCH_OAM_DA1.c(), ZUCA_LATCH_OAM_DA2.c(), WONE_LATCH_OAM_DA3.c(),
                           ZAXE_LATCH_OAM_DA4.c(), XAFU_LATCH_OAM_DA5.c(), YSES_LATCH_OAM_DA6.c(), ZECA_LATCH_OAM_DA7.c());
  }

  uint8_t get_oam_latch_data_b() const {
    return (uint8_t)pack_p(XYKY_LATCH_OAM_DB0.c(), YRUM_LATCH_OAM_DB1.c(), YSEX_LATCH_OAM_DB2.c(), YVEL_LATCH_OAM_DB3.c(),
                           WYNO_LATCH_OAM_DB4.c(), CYRA_LATCH_OAM_DB5.c(), ZUVE_LATCH_OAM_DB6.c(), ECED_LATCH_OAM_DB7.c());
  }

  uint8_t get_oam_bus_data_a() const {
    return (uint8_t)pack_p(OAM_BUS_DA0.qp(), OAM_BUS_DA1.qp(), OAM_BUS_DA2.qp(), OAM_BUS_DA3.qp(),
                           OAM_BUS_DA4.qp(), OAM_BUS_DA5.qp(), OAM_BUS_DA6.qp(), OAM_BUS_DA7.qp());
  }

  uint8_t get_oam_bus_data_b() const {
    return (uint8_t)pack_p(OAM_BUS_DB0.qp(), OAM_BUS_DB1.qp(), OAM_BUS_DB2.qp(), OAM_BUS_DB3.qp(),
                           OAM_BUS_DB4.qp(), OAM_BUS_DB5.qp(), OAM_BUS_DB6.qp(), OAM_BUS_DB7.qp());
  }

  uint8_t get_oam_temp_a() const {
    return (uint8_t)pack_p(XUSO_OAM_DA0.qp(), XEGU_OAM_DA1.qp(), YJEX_OAM_DA2.qp(), XYJU_OAM_DA3.qp(),
                           YBOG_OAM_DA4.qp(), WYSO_OAM_DA5.qp(), XOTE_OAM_DA6.qp(), YZAB_OAM_DA7.qp());
  }

  uint8_t get_oam_temp_b() const {
    return (uint8_t)pack_p(YLOR_OAM_DB0.qp(), ZYTY_OAM_DB1.qp(), ZYVE_OAM_DB2.qp(), ZEZY_OAM_DB3.qp(),
                           GOMO_OAM_DB4.qp(), BAXO_OAM_DB5.qp(), YZOS_OAM_DB6.qp(), DEPO_OAM_DB7.qp());
  }

  void preset_bus_data_a(uint8_t data) {
    OAM_BUS_DA0.preset(data & 0x01);
    OAM_BUS_DA1.preset(data & 0x02);
    OAM_BUS_DA2.preset(data & 0x04);
    OAM_BUS_DA3.preset(data & 0x08);
    OAM_BUS_DA4.preset(data & 0x10);
    OAM_BUS_DA5.preset(data & 0x20);
    OAM_BUS_DA6.preset(data & 0x40);
    OAM_BUS_DA7.preset(data & 0x80);
  }

  void preset_bus_data_b(uint8_t data) {
    OAM_BUS_DB0.preset(data & 0x01);
    OAM_BUS_DB1.preset(data & 0x02);
    OAM_BUS_DB2.preset(data & 0x04);
    OAM_BUS_DB3.preset(data & 0x08);
    OAM_BUS_DB4.preset(data & 0x10);
    OAM_BUS_DB5.preset(data & 0x20);
    OAM_BUS_DB6.preset(data & 0x40);
    OAM_BUS_DB7.preset(data & 0x80);
  }

  //----------------------------------------

  /*p04.MAKA*/ RegQP MAKA_HOLD_MEMp = REG_D0C0;
  /*p28.WUJE*/ Tri WUJE_CPU_OAM_WRn = TRI_D1NP;

  /*p31.XYKY*/ Latch XYKY_LATCH_OAM_DB0 = TRI_D0NP;
  /*p31.YRUM*/ Latch YRUM_LATCH_OAM_DB1 = TRI_D0NP;
  /*p31.YSEX*/ Latch YSEX_LATCH_OAM_DB2 = TRI_D0NP;
  /*p31.YVEL*/ Latch YVEL_LATCH_OAM_DB3 = TRI_D0NP;
  /*p31.WYNO*/ Latch WYNO_LATCH_OAM_DB4 = TRI_D0NP;
  /*p31.CYRA*/ Latch CYRA_LATCH_OAM_DB5 = TRI_D0NP;
  /*p31.ZUVE*/ Latch ZUVE_LATCH_OAM_DB6 = TRI_D0NP;
  /*p31.ECED*/ Latch ECED_LATCH_OAM_DB7 = TRI_D0NP;

  /*p29.YDYV*/ Latch YDYV_LATCH_OAM_DA0 = TRI_D0NP;
  /*p29.YCEB*/ Latch YCEB_LATCH_OAM_DA1 = TRI_D0NP;
  /*p29.ZUCA*/ Latch ZUCA_LATCH_OAM_DA2 = TRI_D0NP;
  /*p29.WONE*/ Latch WONE_LATCH_OAM_DA3 = TRI_D0NP;
  /*p29.ZAXE*/ Latch ZAXE_LATCH_OAM_DA4 = TRI_D0NP;
  /*p29.XAFU*/ Latch XAFU_LATCH_OAM_DA5 = TRI_D0NP;
  /*p29.YSES*/ Latch YSES_LATCH_OAM_DA6 = TRI_D0NP;
  /*p29.ZECA*/ Latch ZECA_LATCH_OAM_DA7 = TRI_D0NP;

  // oam byte 0, byte 2
  /*p29.XUSO*/ RegQP XUSO_OAM_DA0 = REG_D0C0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ RegQP XEGU_OAM_DA1 = REG_D0C0; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ RegQP YJEX_OAM_DA2 = REG_D0C0; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ RegQP XYJU_OAM_DA3 = REG_D0C0; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ RegQP YBOG_OAM_DA4 = REG_D0C0; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ RegQP WYSO_OAM_DA5 = REG_D0C0; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ RegQP XOTE_OAM_DA6 = REG_D0C0; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ RegQP YZAB_OAM_DA7 = REG_D0C0; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ RegQP YLOR_OAM_DB0 = REG_D0C0; // sprite x bit 0, 
  /*p31.ZYTY*/ RegQP ZYTY_OAM_DB1 = REG_D0C0; // sprite x bit 1, 
  /*p31.ZYVE*/ RegQP ZYVE_OAM_DB2 = REG_D0C0; // sprite x bit 2, 
  /*p31.ZEZY*/ RegQP ZEZY_OAM_DB3 = REG_D0C0; // sprite x bit 3, 
  /*p31.GOMO*/ RegQP GOMO_OAM_DB4 = REG_D0C0; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ RegQP BAXO_OAM_DB5 = REG_D0C0; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ RegQP YZOS_OAM_DB6 = REG_D0C0; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ RegQP DEPO_OAM_DB7 = REG_D0C0; // sprite x bit 7, sprite priority

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