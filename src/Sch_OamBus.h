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

  uint8_t get_oam_bus_addr() const {
    return (uint8_t)pack(OAM_BUS_A1.q(), OAM_BUS_A2.q(), OAM_BUS_A3.q(), OAM_BUS_A4.q(), OAM_BUS_A5.q(), OAM_BUS_A6.q(), OAM_BUS_A7.q(), 0);
  }

  uint16_t get_oam_bus_data() const {
    return (uint16_t)pack(OAM_BUS_DA0.qn(), OAM_BUS_DA1.qn(), OAM_BUS_DA2.qn(), OAM_BUS_DA3.qn(),
                          OAM_BUS_DA4.qn(), OAM_BUS_DA5.qn(), OAM_BUS_DA6.qn(), OAM_BUS_DA7.qn(),
                          OAM_BUS_DB0.qn(), OAM_BUS_DB1.qn(), OAM_BUS_DB2.qn(), OAM_BUS_DB3.qn(),
                          OAM_BUS_DB4.qn(), OAM_BUS_DB5.qn(), OAM_BUS_DB6.qn(), OAM_BUS_DB7.qn());
  }

  uint8_t get_oam_bus_data_a() const {
    return (uint8_t)pack(OAM_BUS_DA0.qn(), OAM_BUS_DA1.qn(), OAM_BUS_DA2.qn(), OAM_BUS_DA3.qn(),
                         OAM_BUS_DA4.qn(), OAM_BUS_DA5.qn(), OAM_BUS_DA6.qn(), OAM_BUS_DA7.qn());
  }

  uint8_t get_oam_bus_data_b() const {
    return (uint8_t)pack(OAM_BUS_DB0.qn(), OAM_BUS_DB1.qn(), OAM_BUS_DB2.qn(), OAM_BUS_DB3.qn(),
                         OAM_BUS_DB4.qn(), OAM_BUS_DB5.qn(), OAM_BUS_DB6.qn(), OAM_BUS_DB7.qn());
  }

  uint8_t get_temp_a() const {
    return (uint8_t)pack(XUSO_OAM_DA0.q(), XEGU_OAM_DA1.q(), YJEX_OAM_DA2.q(), XYJU_OAM_DA3.q(),
                         YBOG_OAM_DA4.q(), WYSO_OAM_DA5.q(), XOTE_OAM_DA6.q(), YZAB_OAM_DA7.q());
  }

  uint8_t get_temp_b() const {
    return (uint8_t)pack(YLOR_OAM_DB0.q(), ZYTY_OAM_DB1.q(), ZYVE_OAM_DB2.q(), ZEZY_OAM_DB3.q(),
                         GOMO_OAM_DB4.q(), BAXO_OAM_DB5.q(), YZOS_OAM_DB6.q(), DEPO_OAM_DB7.q());
  }

  void preset_bus_data_a(bool oe, uint8_t data) {
    if (oe) {
      OAM_BUS_DA0.preset(data & 0x01);
      OAM_BUS_DA1.preset(data & 0x02);
      OAM_BUS_DA2.preset(data & 0x04);
      OAM_BUS_DA3.preset(data & 0x08);
      OAM_BUS_DA4.preset(data & 0x10);
      OAM_BUS_DA5.preset(data & 0x20);
      OAM_BUS_DA6.preset(data & 0x40);
      OAM_BUS_DA7.preset(data & 0x80);
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
    }
  }

  void preset_bus_data_b(bool oe, uint8_t data) {
    if (oe) {
      OAM_BUS_DB0.preset(data & 0x01);
      OAM_BUS_DB1.preset(data & 0x02);
      OAM_BUS_DB2.preset(data & 0x04);
      OAM_BUS_DB3.preset(data & 0x08);
      OAM_BUS_DB4.preset(data & 0x10);
      OAM_BUS_DB5.preset(data & 0x20);
      OAM_BUS_DB6.preset(data & 0x40);
      OAM_BUS_DB7.preset(data & 0x80);
    }
    else {
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

  // -> sprite store
  /*p28.YFOT*/ wire YFOT_OAM_A2n() const { return not1(OAM_TRI_A2.q()); }
  /*p28.YFOC*/ wire YFOC_OAM_A3n() const { return not1(OAM_TRI_A3.q()); }
  /*p28.YVOM*/ wire YVOM_OAM_A4n() const { return not1(OAM_TRI_A4.q()); }
  /*p28.YMEV*/ wire YMEV_OAM_A5n() const { return not1(OAM_TRI_A5.q()); }
  /*p28.XEMU*/ wire XEMU_OAM_A6n() const { return not1(OAM_TRI_A6.q()); }
  /*p28.YZET*/ wire YZET_OAM_A7n() const { return not1(OAM_TRI_A7.q()); }

  // oam byte 0, byte 2
  /*p29.XUSO*/ Reg XUSO_OAM_DA0 = REG_D0C0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ Reg XEGU_OAM_DA1 = REG_D0C0; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ Reg YJEX_OAM_DA2 = REG_D0C0; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ Reg XYJU_OAM_DA3 = REG_D0C0; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ Reg YBOG_OAM_DA4 = REG_D0C0; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ Reg WYSO_OAM_DA5 = REG_D0C0; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ Reg XOTE_OAM_DA6 = REG_D0C0; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ Reg YZAB_OAM_DA7 = REG_D0C0; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ Reg YLOR_OAM_DB0 = REG_D0C0; // sprite x bit 0, 
  /*p31.ZYTY*/ Reg ZYTY_OAM_DB1 = REG_D0C0; // sprite x bit 1, 
  /*p31.ZYVE*/ Reg ZYVE_OAM_DB2 = REG_D0C0; // sprite x bit 2, 
  /*p31.ZEZY*/ Reg ZEZY_OAM_DB3 = REG_D0C0; // sprite x bit 3, 
  /*p31.GOMO*/ Reg GOMO_OAM_DB4 = REG_D0C0; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ Reg BAXO_OAM_DB5 = REG_D0C0; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ Reg YZOS_OAM_DB6 = REG_D0C0; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ Reg DEPO_OAM_DB7 = REG_D0C0; // sprite x bit 7, sprite priority

//private:

  /*p04.MAKA*/ Reg _MAKA_DV_SYNC = REG_D0C0;
  /*p28.WUJE*/ Tri WUJE_CPU_OAM_WRn = TRI_D1NP;

  /*p28.APAR*/ Sig APAR_SCAN_OAM_RDn;
  /*p04.DUGA*/ Sig DUGA_DMA_OAM_RDn;
  /*p28.ASAM*/ Sig ASAM_CPU_OAM_RDn;
  /*p28.BETE*/ Sig BETE_PPU_OAM_RDn;

  /*p25.AVER*/ Sig AVER_SCAN_OAM_CLK;
  /*p25.XUJY*/ Sig XUJY_PPU_OAM_CLK;
  /*p25.CUFE*/ Sig CUFE_DMA_OAM_CLK;
  /*p25.BYCU*/ Sig BYCU_OAM_CLKp;
  /*p25.COTA*/ Sig COTA_OAM_CLKn;

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

  /*p31.XYKY*/ Tri XYKY_LATCH_OAM_DB0 = TRI_D0NP;
  /*p31.YRUM*/ Tri YRUM_LATCH_OAM_DB1 = TRI_D0NP;
  /*p31.YSEX*/ Tri YSEX_LATCH_OAM_DB2 = TRI_D0NP;
  /*p31.YVEL*/ Tri YVEL_LATCH_OAM_DB3 = TRI_D0NP;
  /*p31.WYNO*/ Tri WYNO_LATCH_OAM_DB4 = TRI_D0NP;
  /*p31.CYRA*/ Tri CYRA_LATCH_OAM_DB5 = TRI_D0NP;
  /*p31.ZUVE*/ Tri ZUVE_LATCH_OAM_DB6 = TRI_D0NP;
  /*p31.ECED*/ Tri ECED_LATCH_OAM_DB7 = TRI_D0NP;

  /*p29.YDYV*/ Tri YDYV_LATCH_OAM_DA0 = TRI_D0NP;
  /*p29.YCEB*/ Tri YCEB_LATCH_OAM_DA1 = TRI_D0NP;
  /*p29.ZUCA*/ Tri ZUCA_LATCH_OAM_DA2 = TRI_D0NP;
  /*p29.WONE*/ Tri WONE_LATCH_OAM_DA3 = TRI_D0NP;
  /*p29.ZAXE*/ Tri ZAXE_LATCH_OAM_DA4 = TRI_D0NP;
  /*p29.XAFU*/ Tri XAFU_LATCH_OAM_DA5 = TRI_D0NP;
  /*p29.YSES*/ Tri YSES_LATCH_OAM_DA6 = TRI_D0NP;
  /*p29.ZECA*/ Tri ZECA_LATCH_OAM_DA7 = TRI_D0NP;

  //Tri OAM_BUS_A0  = TRI_HZNP;
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