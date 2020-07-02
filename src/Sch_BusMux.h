#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusMuxSignals {

  /*p28.GEKA*/ Signal GEKA_OAM_A0p;
  /*p28.ZYFO*/ Signal ZYFO_OAM_A1p;
  /*p28.YFOT*/ Signal YFOT_OAM_A2p;
  /*p28.YFOC*/ Signal YFOC_OAM_A3p;
  /*p28.YVOM*/ Signal YVOM_OAM_A4p;
  /*p28.YMEV*/ Signal YMEV_OAM_A5p;
  /*p28.XEMU*/ Signal XEMU_OAM_A6p;
  /*p28.YZET*/ Signal YZET_OAM_A7p;

  /*p31.YLOR*/ Signal YLOR_SPRITE_X0; // to sprite store
  /*p31.ZYTY*/ Signal ZYTY_SPRITE_X1;
  /*p31.ZYVE*/ Signal ZYVE_SPRITE_X2;
  /*p31.ZEZY*/ Signal ZEZY_SPRITE_X3;
  /*p31.GOMO*/ Signal GOMO_SPRITE_X4;
  /*p31.BAXO*/ Signal BAXO_SPRITE_X5;
  /*p31.YZOS*/ Signal YZOS_SPRITE_X6;
  /*p31.DEPO*/ Signal DEPO_SPRITE_X7;

  /*p29.XUSO*/ Signal XUSO_SPRITE_Y0; // to sprite fetcher
  /*p29.XEGU*/ Signal XEGU_SPRITE_Y1;
  /*p29.YJEX*/ Signal YJEX_SPRITE_Y2;
  /*p29.XYJU*/ Signal XYJU_SPRITE_Y3;
  /*p29.YBOG*/ Signal YBOG_SPRITE_Y4;
  /*p29.WYSO*/ Signal WYSO_SPRITE_Y5;
  /*p29.XOTE*/ Signal XOTE_SPRITE_Y6;
  /*p29.YZAB*/ Signal YZAB_SPRITE_Y7;
};

//-----------------------------------------------------------------------------

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit(void);

private:

  // these sets of regs should probably move to sprite fetcher/scanner

  /*p31.YLOR*/ Reg8 YLOR_SPRITE_X0;
  /*p31.ZYTY*/ Reg8 ZYTY_SPRITE_X1;
  /*p31.ZYVE*/ Reg8 ZYVE_SPRITE_X2;
  /*p31.ZEZY*/ Reg8 ZEZY_SPRITE_X3;
  /*p31.GOMO*/ Reg8 GOMO_SPRITE_X4;
  /*p31.BAXO*/ Reg8 BAXO_SPRITE_X5;
  /*p31.YZOS*/ Reg8 YZOS_SPRITE_X6;
  /*p31.DEPO*/ Reg8 DEPO_SPRITE_X7;

  /*p29.XUSO*/ Reg8 XUSO_SPRITE_Y0;
  /*p29.XEGU*/ Reg8 XEGU_SPRITE_Y1;
  /*p29.YJEX*/ Reg8 YJEX_SPRITE_Y2;
  /*p29.XYJU*/ Reg8 XYJU_SPRITE_Y3;
  /*p29.YBOG*/ Reg8 YBOG_SPRITE_Y4;
  /*p29.WYSO*/ Reg8 WYSO_SPRITE_Y5;
  /*p29.XOTE*/ Reg8 XOTE_SPRITE_Y6;
  /*p29.YZAB*/ Reg8 YZAB_SPRITE_Y7;

  /*p31.XYKY*/ TpLatch XYKY_LATCH_OAM_A0;
  /*p31.YRUM*/ TpLatch YRUM_LATCH_OAM_A1;
  /*p31.YSEX*/ TpLatch YSEX_LATCH_OAM_A2;
  /*p31.YVEL*/ TpLatch YVEL_LATCH_OAM_A3;
  /*p31.WYNO*/ TpLatch WYNO_LATCH_OAM_A4;
  /*p31.CYRA*/ TpLatch CYRA_LATCH_OAM_A5;
  /*p31.ZUVE*/ TpLatch ZUVE_LATCH_OAM_A6;
  /*p31.ECED*/ TpLatch ECED_LATCH_OAM_A7;

  /*p29.YDYV*/ TpLatch YDYV_LATCH_OAM_B0;
  /*p29.YCEB*/ TpLatch YCEB_LATCH_OAM_B1;
  /*p29.ZUCA*/ TpLatch ZUCA_LATCH_OAM_B2;
  /*p29.WONE*/ TpLatch WONE_LATCH_OAM_B3;
  /*p29.ZAXE*/ TpLatch ZAXE_LATCH_OAM_B4;
  /*p29.XAFU*/ TpLatch XAFU_LATCH_OAM_B5;
  /*p29.YSES*/ TpLatch YSES_LATCH_OAM_B6;
  /*p29.ZECA*/ TpLatch ZECA_LATCH_OAM_B7;

  //----------
  // OAM pins

  PinOut OAM_PIN_CLK;
  PinOut OAM_PIN_OE;
  PinOut OAM_PIN_WR_A; // definitely write
  PinOut OAM_PIN_WR_B; // definitely write

  PinOut OAM_PIN_A0;
  PinOut OAM_PIN_A1;
  PinOut OAM_PIN_A2;
  PinOut OAM_PIN_A3;
  PinOut OAM_PIN_A4;
  PinOut OAM_PIN_A5;
  PinOut OAM_PIN_A6;
  PinOut OAM_PIN_A7;

  Tribuf OAM_PIN_DA0;
  Tribuf OAM_PIN_DA1;
  Tribuf OAM_PIN_DA2;
  Tribuf OAM_PIN_DA3;
  Tribuf OAM_PIN_DA4;
  Tribuf OAM_PIN_DA5;
  Tribuf OAM_PIN_DA6;
  Tribuf OAM_PIN_DA7;

  Tribuf OAM_PIN_DB0;
  Tribuf OAM_PIN_DB1;
  Tribuf OAM_PIN_DB2;
  Tribuf OAM_PIN_DB3;
  Tribuf OAM_PIN_DB4;
  Tribuf OAM_PIN_DB5;
  Tribuf OAM_PIN_DB6;
  Tribuf OAM_PIN_DB7;

  //----------
  // Cart pins

  /* PIN_79 */ PinOut EXT_PIN_RD_A;   // <- P08.UGAC
  /* PIN_79 */ PinOut EXT_PIN_RD_D;   // <- P08.URUN
  /* PIN_78 */ PinOut EXT_PIN_WR_A;   // <- P08.UVER
  /* PIN_78 */ PinOut EXT_PIN_WR_D;   // <- P08.USUF
  /* PIN_80 */ PinOut EXT_PIN_CS_A;   // <- P08.TYHO
                                  
  //----------
  // Address pins

  /* PIN_01 */ PinOut EXT_PIN_A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinOut EXT_PIN_A00_D;   // <- P08.KOTY
  /* PIN_02 */ PinOut EXT_PIN_A01_A;   // <- P08.CABA
  /* PIN_02 */ PinOut EXT_PIN_A01_D;   // <- P08.COTU
  /* PIN_03 */ PinOut EXT_PIN_A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinOut EXT_PIN_A02_D;   // <- P08.BAJO
  /* PIN_04 */ PinOut EXT_PIN_A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinOut EXT_PIN_A03_D;   // <- P08.BOLA
  /* PIN_05 */ PinOut EXT_PIN_A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinOut EXT_PIN_A04_D;   // <- P08.BEVO
  /* PIN_06 */ PinOut EXT_PIN_A05_A;   // <- P08.BADU
  /* PIN_06 */ PinOut EXT_PIN_A05_D;   // <- P08.AJAV
  /* PIN_07 */ PinOut EXT_PIN_A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinOut EXT_PIN_A06_D;   // <- P08.CYKA
  /* PIN_08 */ PinOut EXT_PIN_A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinOut EXT_PIN_A07_D;   // <- P08.COLO
  /* PIN_09 */ PinOut EXT_PIN_A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinOut EXT_PIN_A08_D;   // <- P08.MEGO
  /* PIN_10 */ PinOut EXT_PIN_A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinOut EXT_PIN_A09_D;   // <- P08.MENY
  /* PIN_11 */ PinOut EXT_PIN_A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinOut EXT_PIN_A10_D;   // <- P08.RORE
  /* PIN_12 */ PinOut EXT_PIN_A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinOut EXT_PIN_A11_D;   // <- P08.LYNY
  /* PIN_13 */ PinOut EXT_PIN_A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinOut EXT_PIN_A12_D;   // <- P08.LOSO
  /* PIN_14 */ PinOut EXT_PIN_A13_A;   // <- P08.LABE
  /* PIN_14 */ PinOut EXT_PIN_A13_D;   // <- P08.LEVA
  /* PIN_15 */ PinOut EXT_PIN_A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinOut EXT_PIN_A14_D;   // <- P08.PAHY
  /* PIN_16 */ PinOut EXT_PIN_A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinOut EXT_PIN_A15_D;   // <- P08.RULO
};

//-----------------------------------------------------------------------------

}; // namespace Schematics