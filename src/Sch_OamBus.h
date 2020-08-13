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

  uint8_t get_oam_tri_addr() const {
    return (uint8_t)pack_p(OAM_TRI_A0n.c(), OAM_TRI_A1n.c(), OAM_TRI_A2n.c(), OAM_TRI_A3n.c(),
                           OAM_TRI_A4n.c(), OAM_TRI_A5n.c(), OAM_TRI_A6n.c(), OAM_TRI_A7n.c());
  }

  uint8_t get_oam_pin_addr() const {
    return (uint8_t)pack_p(OAM_PIN_A1p.tp(), OAM_PIN_A2p.tp(), OAM_PIN_A3p.tp(), OAM_PIN_A4p.tp(),
                           OAM_PIN_A5p.tp(), OAM_PIN_A6p.tp(), OAM_PIN_A7p.tp(), 0);
  }

  uint8_t get_oam_latch_data_a() const {
    return (uint8_t)pack_p(YDYV_LATCH_OAM_DA0.c(), YCEB_LATCH_OAM_DA1.c(), ZUCA_LATCH_OAM_DA2.c(), WONE_LATCH_OAM_DA3.c(),
                           ZAXE_LATCH_OAM_DA4.c(), XAFU_LATCH_OAM_DA5.c(), YSES_LATCH_OAM_DA6.c(), ZECA_LATCH_OAM_DA7.c());
  }

  uint8_t get_oam_latch_data_b() const {
    return (uint8_t)pack_p(XYKY_LATCH_OAM_DB0.c(), YRUM_LATCH_OAM_DB1.c(), YSEX_LATCH_OAM_DB2.c(), YVEL_LATCH_OAM_DB3.c(),
                           WYNO_LATCH_OAM_DB4.c(), CYRA_LATCH_OAM_DB5.c(), ZUVE_LATCH_OAM_DB6.c(), ECED_LATCH_OAM_DB7.c());
  }

  uint8_t get_oam_pin_data_a() const {
    return (uint8_t)pack_p(OAM_PIN_DA0n.tp(), OAM_PIN_DA1n.tp(), OAM_PIN_DA2n.tp(), OAM_PIN_DA3n.tp(),
                           OAM_PIN_DA4n.tp(), OAM_PIN_DA5n.tp(), OAM_PIN_DA6n.tp(), OAM_PIN_DA7n.tp());
  }

  uint8_t get_oam_pin_data_b() const {
    return (uint8_t)pack_p(OAM_PIN_DB0n.tp(), OAM_PIN_DB1n.tp(), OAM_PIN_DB2n.tp(), OAM_PIN_DB3n.tp(),
                           OAM_PIN_DB4n.tp(), OAM_PIN_DB5n.tp(), OAM_PIN_DB6n.tp(), OAM_PIN_DB7n.tp());
  }

  uint8_t get_oam_temp_a() const {
    return (uint8_t)pack_p(XUSO_OAM_DA0p.qp(), XEGU_OAM_DA1p.qp(), YJEX_OAM_DA2p.qp(), XYJU_OAM_DA3p.qp(),
                           YBOG_OAM_DA4p.qp(), WYSO_OAM_DA5p.qp(), XOTE_OAM_DA6p.qp(), YZAB_OAM_DA7p.qp());
  }

  uint8_t get_oam_temp_b() const {
    return (uint8_t)pack_p(YLOR_OAM_DB0p.qp(), ZYTY_OAM_DB1p.qp(), ZYVE_OAM_DB2p.qp(), ZEZY_OAM_DB3p.qp(),
                           GOMO_OAM_DB4p.qp(), BAXO_OAM_DB5p.qp(), YZOS_OAM_DB6p.qp(), DEPO_OAM_DB7p.qp());
  }

  void preset_bus_data_a(uint8_t data) {
    OAM_PIN_DA0n.preset(data & 0x01);
    OAM_PIN_DA1n.preset(data & 0x02);
    OAM_PIN_DA2n.preset(data & 0x04);
    OAM_PIN_DA3n.preset(data & 0x08);
    OAM_PIN_DA4n.preset(data & 0x10);
    OAM_PIN_DA5n.preset(data & 0x20);
    OAM_PIN_DA6n.preset(data & 0x40);
    OAM_PIN_DA7n.preset(data & 0x80);
  }

  void preset_bus_data_b(uint8_t data) {
    OAM_PIN_DB0n.preset(data & 0x01);
    OAM_PIN_DB1n.preset(data & 0x02);
    OAM_PIN_DB2n.preset(data & 0x04);
    OAM_PIN_DB3n.preset(data & 0x08);
    OAM_PIN_DB4n.preset(data & 0x10);
    OAM_PIN_DB5n.preset(data & 0x20);
    OAM_PIN_DB6n.preset(data & 0x40);
    OAM_PIN_DB7n.preset(data & 0x80);
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
  /*p29.XUSO*/ RegQP XUSO_OAM_DA0p = REG_D0C0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ RegQP XEGU_OAM_DA1p = REG_D0C0; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ RegQP YJEX_OAM_DA2p = REG_D0C0; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ RegQP XYJU_OAM_DA3p = REG_D0C0; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ RegQP YBOG_OAM_DA4p = REG_D0C0; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ RegQP WYSO_OAM_DA5p = REG_D0C0; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ RegQP XOTE_OAM_DA6p = REG_D0C0; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ RegQP YZAB_OAM_DA7p = REG_D0C0; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ RegQP YLOR_OAM_DB0p = REG_D0C0; // sprite x bit 0, 
  /*p31.ZYTY*/ RegQP ZYTY_OAM_DB1p = REG_D0C0; // sprite x bit 1, 
  /*p31.ZYVE*/ RegQP ZYVE_OAM_DB2p = REG_D0C0; // sprite x bit 2, 
  /*p31.ZEZY*/ RegQP ZEZY_OAM_DB3p = REG_D0C0; // sprite x bit 3, 
  /*p31.GOMO*/ RegQP GOMO_OAM_DB4p = REG_D0C0; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ RegQP BAXO_OAM_DB5p = REG_D0C0; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ RegQP YZOS_OAM_DB6p = REG_D0C0; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ RegQP DEPO_OAM_DB7p = REG_D0C0; // sprite x bit 7, sprite priority

  Tri OAM_PIN_CLK  = TRI_D0NP;
  Tri OAM_PIN_OE   = TRI_D0PU; // FIXME why are these pulled up?
  Tri OAM_PIN_WR_B = TRI_D0PU;
  Tri OAM_PIN_WR_A = TRI_D0PU;

  Tri OAM_TRI_A0n  = TRI_D0NP;
  Tri OAM_TRI_A1n  = TRI_D0NP;
  Tri OAM_TRI_A2n  = TRI_D0NP;
  Tri OAM_TRI_A3n  = TRI_D0NP;
  Tri OAM_TRI_A4n  = TRI_D0NP;
  Tri OAM_TRI_A5n  = TRI_D0NP;
  Tri OAM_TRI_A6n  = TRI_D0NP;
  Tri OAM_TRI_A7n  = TRI_D0NP;

  Tri OAM_PIN_A1p  = TRI_D0NP;
  Tri OAM_PIN_A2p  = TRI_D0NP;
  Tri OAM_PIN_A3p  = TRI_D0NP;
  Tri OAM_PIN_A4p  = TRI_D0NP;
  Tri OAM_PIN_A5p  = TRI_D0NP;
  Tri OAM_PIN_A6p  = TRI_D0NP;
  Tri OAM_PIN_A7p  = TRI_D0NP;

  Tri OAM_PIN_DA0n = TRI_D0NP;
  Tri OAM_PIN_DA1n = TRI_D0NP;
  Tri OAM_PIN_DA2n = TRI_D0NP;
  Tri OAM_PIN_DA3n = TRI_D0NP;
  Tri OAM_PIN_DA4n = TRI_D0NP;
  Tri OAM_PIN_DA5n = TRI_D0NP;
  Tri OAM_PIN_DA6n = TRI_D0NP;
  Tri OAM_PIN_DA7n = TRI_D0NP;

  Tri OAM_PIN_DB0n = TRI_D0NP;
  Tri OAM_PIN_DB1n = TRI_D0NP;
  Tri OAM_PIN_DB2n = TRI_D0NP;
  Tri OAM_PIN_DB3n = TRI_D0NP;
  Tri OAM_PIN_DB4n = TRI_D0NP;
  Tri OAM_PIN_DB5n = TRI_D0NP;
  Tri OAM_PIN_DB6n = TRI_D0NP;
  Tri OAM_PIN_DB7n = TRI_D0NP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics