#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void dump(Dumper& d) const;
  void tock(SchematicTop& top);
  
  //----------------------------------------

  uint8_t get_oam_pin_addr() const {
    return (uint8_t)pack_p(PIN_OAM_A1p.tp(), PIN_OAM_A2p.tp(), PIN_OAM_A3p.tp(), PIN_OAM_A4p.tp(),
                           PIN_OAM_A5p.tp(), PIN_OAM_A6p.tp(), PIN_OAM_A7p.tp(), 0);
  }


  uint8_t get_oam_pin_data_a() const {
    return ~(uint8_t)pack_p(PIN_OAM_DA0n.tp(), PIN_OAM_DA1n.tp(), PIN_OAM_DA2n.tp(), PIN_OAM_DA3n.tp(),
                            PIN_OAM_DA4n.tp(), PIN_OAM_DA5n.tp(), PIN_OAM_DA6n.tp(), PIN_OAM_DA7n.tp());
  }

  uint8_t get_oam_pin_data_b() const {
    return ~(uint8_t)pack_p(PIN_OAM_DB0n.tp(), PIN_OAM_DB1n.tp(), PIN_OAM_DB2n.tp(), PIN_OAM_DB3n.tp(),
                            PIN_OAM_DB4n.tp(), PIN_OAM_DB5n.tp(), PIN_OAM_DB6n.tp(), PIN_OAM_DB7n.tp());
  }

  void set_pin_data_a(uint8_t data) {
    PIN_OAM_DA0n = !(data & 0x01);
    PIN_OAM_DA1n = !(data & 0x02);
    PIN_OAM_DA2n = !(data & 0x04);
    PIN_OAM_DA3n = !(data & 0x08);
    PIN_OAM_DA4n = !(data & 0x10);
    PIN_OAM_DA5n = !(data & 0x20);
    PIN_OAM_DA6n = !(data & 0x40);
    PIN_OAM_DA7n = !(data & 0x80);
  }

  void set_pin_data_b(uint8_t data) {
    PIN_OAM_DB0n = !(data & 0x01);
    PIN_OAM_DB1n = !(data & 0x02);
    PIN_OAM_DB2n = !(data & 0x04);
    PIN_OAM_DB3n = !(data & 0x08);
    PIN_OAM_DB4n = !(data & 0x10);
    PIN_OAM_DB5n = !(data & 0x20);
    PIN_OAM_DB6n = !(data & 0x40);
    PIN_OAM_DB7n = !(data & 0x80);
  }

  //----------------------------------------

  /*p04.MAKA*/ RegQP MAKA_HOLD_MEMp = REG_D0C0;
  /*p28.WUJE*/ Latch WUJE_CPU_OAM_WRn = TRI_D1NP;

  /*p31.XYKY*/ Latch XYKY_OAM_LATCH_DB0n = TRI_D0NP;
  /*p31.YRUM*/ Latch YRUM_OAM_LATCH_DB1n = TRI_D0NP;
  /*p31.YSEX*/ Latch YSEX_OAM_LATCH_DB2n = TRI_D0NP;
  /*p31.YVEL*/ Latch YVEL_OAM_LATCH_DB3n = TRI_D0NP;
  /*p31.WYNO*/ Latch WYNO_OAM_LATCH_DB4n = TRI_D0NP;
  /*p31.CYRA*/ Latch CYRA_OAM_LATCH_DB5n = TRI_D0NP;
  /*p31.ZUVE*/ Latch ZUVE_OAM_LATCH_DB6n = TRI_D0NP;
  /*p31.ECED*/ Latch ECED_OAM_LATCH_DB7n = TRI_D0NP;

  /*p29.YDYV*/ Latch YDYV_OAM_LATCH_DA0n = TRI_D0NP;
  /*p29.YCEB*/ Latch YCEB_OAM_LATCH_DA1n = TRI_D0NP;
  /*p29.ZUCA*/ Latch ZUCA_OAM_LATCH_DA2n = TRI_D0NP;
  /*p29.WONE*/ Latch WONE_OAM_LATCH_DA3n = TRI_D0NP;
  /*p29.ZAXE*/ Latch ZAXE_OAM_LATCH_DA4n = TRI_D0NP;
  /*p29.XAFU*/ Latch XAFU_OAM_LATCH_DA5n = TRI_D0NP;
  /*p29.YSES*/ Latch YSES_OAM_LATCH_DA6n = TRI_D0NP;
  /*p29.ZECA*/ Latch ZECA_OAM_LATCH_DA7n = TRI_D0NP;

  /*p30.XADU*/ RegQN XADU_SPRITE_IDX0p = REG_D0C0;
  /*p30.XEDY*/ RegQN XEDY_SPRITE_IDX1p = REG_D0C0;
  /*p30.ZUZE*/ RegQN ZUZE_SPRITE_IDX2p = REG_D0C0;
  /*p30.XOBE*/ RegQN XOBE_SPRITE_IDX3p = REG_D0C0;
  /*p30.YDUF*/ RegQN YDUF_SPRITE_IDX4p = REG_D0C0;
  /*p30.XECU*/ RegQN XECU_SPRITE_IDX5p = REG_D0C0;

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

  Bus BUS_OAM_A0n  = TRI_D0NP;
  Bus BUS_OAM_A1n  = TRI_D0NP;
  Bus BUS_OAM_A2n  = TRI_D0NP;
  Bus BUS_OAM_A3n  = TRI_D0NP;
  Bus BUS_OAM_A4n  = TRI_D0NP;
  Bus BUS_OAM_A5n  = TRI_D0NP;
  Bus BUS_OAM_A6n  = TRI_D0NP;
  Bus BUS_OAM_A7n  = TRI_D0NP;

  Tri PIN_OAM_CLK  = TRI_D0NP;
  Tri PIN_OAM_OE   = TRI_D1NP;
  Tri PIN_OAM_WR_A = TRI_D1NP;
  Tri PIN_OAM_WR_B = TRI_D1NP;

  Tri PIN_OAM_A1p  = TRI_D0NP;
  Tri PIN_OAM_A2p  = TRI_D0NP;
  Tri PIN_OAM_A3p  = TRI_D0NP;
  Tri PIN_OAM_A4p  = TRI_D0NP;
  Tri PIN_OAM_A5p  = TRI_D0NP;
  Tri PIN_OAM_A6p  = TRI_D0NP;
  Tri PIN_OAM_A7p  = TRI_D0NP;

  Tri PIN_OAM_DA0n = TRI_D0NP;
  Tri PIN_OAM_DA1n = TRI_D0NP;
  Tri PIN_OAM_DA2n = TRI_D0NP;
  Tri PIN_OAM_DA3n = TRI_D0NP;
  Tri PIN_OAM_DA4n = TRI_D0NP;
  Tri PIN_OAM_DA5n = TRI_D0NP;
  Tri PIN_OAM_DA6n = TRI_D0NP;
  Tri PIN_OAM_DA7n = TRI_D0NP;

  Tri PIN_OAM_DB0n = TRI_D0NP;
  Tri PIN_OAM_DB1n = TRI_D0NP;
  Tri PIN_OAM_DB2n = TRI_D0NP;
  Tri PIN_OAM_DB3n = TRI_D0NP;
  Tri PIN_OAM_DB4n = TRI_D0NP;
  Tri PIN_OAM_DB5n = TRI_D0NP;
  Tri PIN_OAM_DB6n = TRI_D0NP;
  Tri PIN_OAM_DB7n = TRI_D0NP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics