#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  OamBus();
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

  Reg OAM_TRI_DA0;
  Reg OAM_TRI_DA1;
  Reg OAM_TRI_DA2;
  Reg OAM_TRI_DA3;
  Reg OAM_TRI_DA4;
  Reg OAM_TRI_DA5;
  Reg OAM_TRI_DA6;
  Reg OAM_TRI_DA7;

  Reg OAM_TRI_DB0;
  Reg OAM_TRI_DB1;
  Reg OAM_TRI_DB2;
  Reg OAM_TRI_DB3;
  Reg OAM_TRI_DB4;
  Reg OAM_TRI_DB5;
  Reg OAM_TRI_DB6;
  Reg OAM_TRI_DB7;

  /*p31.XYKY*/ Reg XYKY_LATCH_OAM_DA0;
  /*p31.YRUM*/ Reg YRUM_LATCH_OAM_DA1;
  /*p31.YSEX*/ Reg YSEX_LATCH_OAM_DA2;
  /*p31.YVEL*/ Reg YVEL_LATCH_OAM_DA3;
  /*p31.WYNO*/ Reg WYNO_LATCH_OAM_DA4;
  /*p31.CYRA*/ Reg CYRA_LATCH_OAM_DA5;
  /*p31.ZUVE*/ Reg ZUVE_LATCH_OAM_DA6;
  /*p31.ECED*/ Reg ECED_LATCH_OAM_DA7;

  /*p29.YDYV*/ Reg YDYV_LATCH_OAM_DB0;
  /*p29.YCEB*/ Reg YCEB_LATCH_OAM_DB1;
  /*p29.ZUCA*/ Reg ZUCA_LATCH_OAM_DB2;
  /*p29.WONE*/ Reg WONE_LATCH_OAM_DB3;
  /*p29.ZAXE*/ Reg ZAXE_LATCH_OAM_DB4;
  /*p29.XAFU*/ Reg XAFU_LATCH_OAM_DB5;
  /*p29.YSES*/ Reg YSES_LATCH_OAM_DB6;
  /*p29.ZECA*/ Reg ZECA_LATCH_OAM_DB7;

  Reg OAM_TRI_A0;
  Reg OAM_TRI_A1;
  Reg OAM_TRI_A2;
  Reg OAM_TRI_A3;
  Reg OAM_TRI_A4;
  Reg OAM_TRI_A5;
  Reg OAM_TRI_A6;
  Reg OAM_TRI_A7;

  Reg OAM_PIN_CLK;
  Reg OAM_PIN_OE;
  Reg OAM_PIN_WR_A;
  Reg OAM_PIN_WR_B;

  Reg OAM_PIN_A0;
  Reg OAM_PIN_A1;
  Reg OAM_PIN_A2;
  Reg OAM_PIN_A3;
  Reg OAM_PIN_A4;
  Reg OAM_PIN_A5;
  Reg OAM_PIN_A6;
  Reg OAM_PIN_A7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics