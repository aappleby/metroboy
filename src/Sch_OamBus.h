#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void tock(SchematicTop& top);
  void set_oam_data(wire OE, uint16_t data);

  // -> sprite store
  /*p28.YFOT*/ wire YFOT_OAM_A2p() const { return not(OAM_TRI_A2.q()); }
  /*p28.YFOC*/ wire YFOC_OAM_A3p() const { return not(OAM_TRI_A3.q()); }
  /*p28.YVOM*/ wire YVOM_OAM_A4p() const { return not(OAM_TRI_A4.q()); }
  /*p28.YMEV*/ wire YMEV_OAM_A5p() const { return not(OAM_TRI_A5.q()); }
  /*p28.XEMU*/ wire XEMU_OAM_A6p() const { return not(OAM_TRI_A6.q()); }
  /*p28.YZET*/ wire YZET_OAM_A7p() const { return not(OAM_TRI_A7.q()); }

  /*p31.YLOR*/ Reg2 YLOR_OAM_DA0 = Reg2::D0C0; // sprite x bit 0, 
  /*p31.ZYTY*/ Reg2 ZYTY_OAM_DA1 = Reg2::D0C0; // sprite x bit 1, 
  /*p31.ZYVE*/ Reg2 ZYVE_OAM_DA2 = Reg2::D0C0; // sprite x bit 2, 
  /*p31.ZEZY*/ Reg2 ZEZY_OAM_DA3 = Reg2::D0C0; // sprite x bit 3, 
  /*p31.GOMO*/ Reg2 GOMO_OAM_DA4 = Reg2::D0C0; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ Reg2 BAXO_OAM_DA5 = Reg2::D0C0; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ Reg2 YZOS_OAM_DA6 = Reg2::D0C0; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ Reg2 DEPO_OAM_DA7 = Reg2::D0C0; // sprite x bit 7, sprite priority

  /*p29.XUSO*/ Reg2 XUSO_OAM_DB0 = Reg2::D0C0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ Reg2 XEGU_OAM_DB1 = Reg2::D0C0; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ Reg2 YJEX_OAM_DB2 = Reg2::D0C0; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ Reg2 XYJU_OAM_DB3 = Reg2::D0C0; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ Reg2 YBOG_OAM_DB4 = Reg2::D0C0; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ Reg2 WYSO_OAM_DB5 = Reg2::D0C0; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ Reg2 XOTE_OAM_DB6 = Reg2::D0C0; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ Reg2 YZAB_OAM_DB7 = Reg2::D0C0; // sprite y bit 7, sprite tile index bit 7

  void dump(Dumper& d) {
    d("---------- OAM Bus  ----------\n");
    d("PIN_CLK  %c\n", OAM_PIN_CLK.c());
    d("PIN_OE   %c\n", OAM_PIN_OE.c()); 
    d("PIN_WR_A %c\n", OAM_PIN_WR_A.c());
    d("PIN_WR_B %c\n", OAM_PIN_WR_B.c());

    d("OAM_TRI  %c%c%c%c%c%c%c%c\n", 
      OAM_TRI_A7.c(),
      OAM_TRI_A6.c(),
      OAM_TRI_A5.c(),
      OAM_TRI_A4.c(),
      OAM_TRI_A3.c(),
      OAM_TRI_A2.c(),
      OAM_TRI_A1.c(),
      OAM_TRI_A0.c()
    );
    d("LATCH_A  %c%c%c%c%c%c%c%c\n", 
      ECED_LATCH_OAM_DA7.c(),
      ZUVE_LATCH_OAM_DA6.c(),
      CYRA_LATCH_OAM_DA5.c(),
      WYNO_LATCH_OAM_DA4.c(),
      YVEL_LATCH_OAM_DA3.c(),
      YSEX_LATCH_OAM_DA2.c(),
      YRUM_LATCH_OAM_DA1.c(),
      XYKY_LATCH_OAM_DA0.c()
    );
    d("LATCH_B  %c%c%c%c%c%c%c%c\n", 
      ZECA_LATCH_OAM_DB7.c(),
      YSES_LATCH_OAM_DB6.c(),
      XAFU_LATCH_OAM_DB5.c(),
      ZAXE_LATCH_OAM_DB4.c(),
      WONE_LATCH_OAM_DB3.c(),
      ZUCA_LATCH_OAM_DB2.c(),
      YCEB_LATCH_OAM_DB1.c(),
      YDYV_LATCH_OAM_DB0.c()
    );

    d("PIN_A    %c%c%c%c%c%c%c%c\n", 
      OAM_PIN_A7.c(),
      OAM_PIN_A6.c(),
      OAM_PIN_A5.c(),
      OAM_PIN_A4.c(),
      OAM_PIN_A3.c(),
      OAM_PIN_A2.c(),
      OAM_PIN_A1.c(),
      OAM_PIN_A0.c()
    );

    d("PIN_DA   %c%c%c%c%c%c%c%c\n", 
      OAM_PIN_DA7.c(),
      OAM_PIN_DA6.c(),
      OAM_PIN_DA5.c(),
      OAM_PIN_DA4.c(),
      OAM_PIN_DA3.c(),
      OAM_PIN_DA2.c(),
      OAM_PIN_DA1.c(),
      OAM_PIN_DA0.c()
    );

    d("PIN_DB   %c%c%c%c%c%c%c%c\n", 
      OAM_PIN_DB7.c(),
      OAM_PIN_DB6.c(),
      OAM_PIN_DB5.c(),
      OAM_PIN_DB4.c(),
      OAM_PIN_DB3.c(),
      OAM_PIN_DB2.c(),
      OAM_PIN_DB1.c(),
      OAM_PIN_DB0.c()
    );

  }

private:

  Pin2 OAM_PIN_CLK  = Pin2::HIZ_NP;
  Pin2 OAM_PIN_OE   = Pin2::HIZ_NP;
  Pin2 OAM_PIN_WR_A = Pin2::HIZ_NP;
  Pin2 OAM_PIN_WR_B = Pin2::HIZ_NP;

  Pin2 OAM_TRI_A0 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A1 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A2 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A3 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A4 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A5 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A6 = Pin2::HIZ_NP;
  Pin2 OAM_TRI_A7 = Pin2::HIZ_NP;

  /*p31.XYKY*/ Pin2 XYKY_LATCH_OAM_DA0 = Pin2::LATCH_0;
  /*p31.YRUM*/ Pin2 YRUM_LATCH_OAM_DA1 = Pin2::LATCH_0;
  /*p31.YSEX*/ Pin2 YSEX_LATCH_OAM_DA2 = Pin2::LATCH_0;
  /*p31.YVEL*/ Pin2 YVEL_LATCH_OAM_DA3 = Pin2::LATCH_0;
  /*p31.WYNO*/ Pin2 WYNO_LATCH_OAM_DA4 = Pin2::LATCH_0;
  /*p31.CYRA*/ Pin2 CYRA_LATCH_OAM_DA5 = Pin2::LATCH_0;
  /*p31.ZUVE*/ Pin2 ZUVE_LATCH_OAM_DA6 = Pin2::LATCH_0;
  /*p31.ECED*/ Pin2 ECED_LATCH_OAM_DA7 = Pin2::LATCH_0;

  /*p29.YDYV*/ Pin2 YDYV_LATCH_OAM_DB0 = Pin2::LATCH_0;
  /*p29.YCEB*/ Pin2 YCEB_LATCH_OAM_DB1 = Pin2::LATCH_0;
  /*p29.ZUCA*/ Pin2 ZUCA_LATCH_OAM_DB2 = Pin2::LATCH_0;
  /*p29.WONE*/ Pin2 WONE_LATCH_OAM_DB3 = Pin2::LATCH_0;
  /*p29.ZAXE*/ Pin2 ZAXE_LATCH_OAM_DB4 = Pin2::LATCH_0;
  /*p29.XAFU*/ Pin2 XAFU_LATCH_OAM_DB5 = Pin2::LATCH_0;
  /*p29.YSES*/ Pin2 YSES_LATCH_OAM_DB6 = Pin2::LATCH_0;
  /*p29.ZECA*/ Pin2 ZECA_LATCH_OAM_DB7 = Pin2::LATCH_0;

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