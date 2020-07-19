#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void tock(SchematicTop& top);
  SignalHash commit();
  void set_oam_data(wire OE, uint16_t data);

  // -> sprite store
  /*p28.YFOT*/ wire YFOT_OAM_A2p() const { return not(OAM_TRI_A2.q()); }
  /*p28.YFOC*/ wire YFOC_OAM_A3p() const { return not(OAM_TRI_A3.q()); }
  /*p28.YVOM*/ wire YVOM_OAM_A4p() const { return not(OAM_TRI_A4.q()); }
  /*p28.YMEV*/ wire YMEV_OAM_A5p() const { return not(OAM_TRI_A5.q()); }
  /*p28.XEMU*/ wire XEMU_OAM_A6p() const { return not(OAM_TRI_A6.q()); }
  /*p28.YZET*/ wire YZET_OAM_A7p() const { return not(OAM_TRI_A7.q()); }

private:

  Pin2 OAM_TRI_A0 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A1 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A2 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A3 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A4 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A5 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A6 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A7 = Pin2::HIZ_NP;

  /*p31.XYKY*/ Reg2 XYKY_LATCH_OAM_DA0 = Reg2::D0C0;
  /*p31.YRUM*/ Reg2 YRUM_LATCH_OAM_DA1 = Reg2::D0C0;
  /*p31.YSEX*/ Reg2 YSEX_LATCH_OAM_DA2 = Reg2::D0C0;
  /*p31.YVEL*/ Reg2 YVEL_LATCH_OAM_DA3 = Reg2::D0C0;
  /*p31.WYNO*/ Reg2 WYNO_LATCH_OAM_DA4 = Reg2::D0C0;
  /*p31.CYRA*/ Reg2 CYRA_LATCH_OAM_DA5 = Reg2::D0C0;
  /*p31.ZUVE*/ Reg2 ZUVE_LATCH_OAM_DA6 = Reg2::D0C0;
  /*p31.ECED*/ Reg2 ECED_LATCH_OAM_DA7 = Reg2::D0C0;

  /*p29.YDYV*/ Reg2 YDYV_LATCH_OAM_DB0 = Reg2::D0C0;
  /*p29.YCEB*/ Reg2 YCEB_LATCH_OAM_DB1 = Reg2::D0C0;
  /*p29.ZUCA*/ Reg2 ZUCA_LATCH_OAM_DB2 = Reg2::D0C0;
  /*p29.WONE*/ Reg2 WONE_LATCH_OAM_DB3 = Reg2::D0C0;
  /*p29.ZAXE*/ Reg2 ZAXE_LATCH_OAM_DB4 = Reg2::D0C0;
  /*p29.XAFU*/ Reg2 XAFU_LATCH_OAM_DB5 = Reg2::D0C0;
  /*p29.YSES*/ Reg2 YSES_LATCH_OAM_DB6 = Reg2::D0C0;
  /*p29.ZECA*/ Reg2 ZECA_LATCH_OAM_DB7 = Reg2::D0C0;

  Pin2 OAM_PIN_CLK  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_OE   = Pin2::HIZ_NP;
  Pin2 OAM_PIN_WR_A = Pin2::HIZ_NP;
  Pin2 OAM_PIN_WR_B = Pin2::HIZ_NP;

  Pin2 OAM_PIN_A0  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A1  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A2  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A3  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A4  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A5  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A6  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_A7  = Pin2::HIZ_NP;

  Pin2 OAM_PIN_DA0 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA1 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA2 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA3 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA4 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA5 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA6 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DA7 = Pin2::HIZ_NP;

  Pin2 OAM_PIN_DB0 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB1 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB2 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB3 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB4 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB5 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB6 = Pin2::HIZ_NP;
  Pin2 OAM_PIN_DB7 = Pin2::HIZ_NP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics