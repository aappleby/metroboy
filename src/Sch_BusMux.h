#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct BusMux {
  BusMux();

  void tick(SchematicTop& top);
  void tock(SchematicTop& top);
  SignalHash commit(SchematicTop& top);

  //-----------------------------------------------------------------------------

  void set_ext_bus(uint16_t data) {
    EXT_PIN_D0_C.set(data & 0x01);
    EXT_PIN_D1_C.set(data & 0x02);
    EXT_PIN_D2_C.set(data & 0x04);
    EXT_PIN_D3_C.set(data & 0x08);
    EXT_PIN_D4_C.set(data & 0x10);
    EXT_PIN_D5_C.set(data & 0x20);
    EXT_PIN_D6_C.set(data & 0x40);
    EXT_PIN_D7_C.set(data & 0x80);
  }

  void set_oam_bus(wire OE, uint16_t data) {
    if (OE) {
      OAM_PIN_DA0.preset_a(data & 0x0001);
      OAM_PIN_DA1.preset_a(data & 0x0002);
      OAM_PIN_DA2.preset_a(data & 0x0004);
      OAM_PIN_DA3.preset_a(data & 0x0008);
      OAM_PIN_DA4.preset_a(data & 0x0010);
      OAM_PIN_DA5.preset_a(data & 0x0020);
      OAM_PIN_DA6.preset_a(data & 0x0040);
      OAM_PIN_DA7.preset_a(data & 0x0080);

      OAM_PIN_DB0.preset_a(data & 0x0100);
      OAM_PIN_DB1.preset_a(data & 0x0200);
      OAM_PIN_DB2.preset_a(data & 0x0400);
      OAM_PIN_DB3.preset_a(data & 0x0800);
      OAM_PIN_DB4.preset_a(data & 0x1000);
      OAM_PIN_DB5.preset_a(data & 0x2000);
      OAM_PIN_DB6.preset_a(data & 0x4000);
      OAM_PIN_DB7.preset_a(data & 0x8000);
    }
    else {
      OAM_PIN_DA0.preset_a(HIZ);
      OAM_PIN_DA1.preset_a(HIZ);
      OAM_PIN_DA2.preset_a(HIZ);
      OAM_PIN_DA3.preset_a(HIZ);
      OAM_PIN_DA4.preset_a(HIZ);
      OAM_PIN_DA5.preset_a(HIZ);
      OAM_PIN_DA6.preset_a(HIZ);
      OAM_PIN_DA7.preset_a(HIZ);

      OAM_PIN_DB0.preset_a(HIZ);
      OAM_PIN_DB1.preset_a(HIZ);
      OAM_PIN_DB2.preset_a(HIZ);
      OAM_PIN_DB3.preset_a(HIZ);
      OAM_PIN_DB4.preset_a(HIZ);
      OAM_PIN_DB5.preset_a(HIZ);
      OAM_PIN_DB6.preset_a(HIZ);
      OAM_PIN_DB7.preset_a(HIZ);
    }
  }

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

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  ExtPinIn  EXT_PIN_WRp_C;   // PIN_78 -> P07.UBAL
  ExtPinIn  EXT_PIN_RDp_C;   // PIN_79 -> P07.UJYV

private:

  /*p04.MAKA*/ Reg17 MAKA_FROM_CPU5_SYNC;

  /*p28.GEKA*/ Signal GEKA_OAM_A0p;
  /*p28.ZYFO*/ Signal ZYFO_OAM_A1p;
  /*p28.YFOT*/ Signal YFOT_OAM_A2p;
  /*p28.YFOC*/ Signal YFOC_OAM_A3p;
  /*p28.YVOM*/ Signal YVOM_OAM_A4p;
  /*p28.YMEV*/ Signal YMEV_OAM_A5p;
  /*p28.XEMU*/ Signal XEMU_OAM_A6p;
  /*p28.YZET*/ Signal YZET_OAM_A7p;


  /*p08.ALOR*/ TpLatch CPU_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch CPU_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch CPU_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch CPU_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch CPU_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch CPU_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch CPU_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch CPU_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch CPU_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch CPU_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch CPU_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch CPU_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch CPU_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch CPU_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch CPU_ADDR_LATCH_14;

  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_07;

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  ExtPinIn  EXT_PIN_A00_C;   // PIN_01 -> P08.KOVA
  ExtPinIn  EXT_PIN_A01_C;   // PIN_02 -> P08.CAMU
  ExtPinIn  EXT_PIN_A02_C;   // PIN_03 -> P08.BUXU
  ExtPinIn  EXT_PIN_A03_C;   // PIN_04 -> P08.BASE
  ExtPinIn  EXT_PIN_A04_C;   // PIN_05 -> P08.AFEC
  ExtPinIn  EXT_PIN_A05_C;   // PIN_06 -> P08.ABUP
  ExtPinIn  EXT_PIN_A06_C;   // PIN_07 -> P08.CYGU
  ExtPinIn  EXT_PIN_A07_C;   // PIN_08 -> P08.COGO
  ExtPinIn  EXT_PIN_A08_C;   // PIN_09 -> P08.MUJY
  ExtPinIn  EXT_PIN_A09_C;   // PIN_10 -> P08.NENA
  ExtPinIn  EXT_PIN_A10_C;   // PIN_11 -> P08.SURA
  ExtPinIn  EXT_PIN_A11_C;   // PIN_12 -> P08.MADY
  ExtPinIn  EXT_PIN_A12_C;   // PIN_13 -> P08.LAHE
  ExtPinIn  EXT_PIN_A13_C;   // PIN_14 -> P08.LURA
  ExtPinIn  EXT_PIN_A14_C;   // PIN_15 -> P08.PEVO
  ExtPinIn  EXT_PIN_A15_C;   // PIN_16 -> P08.RAZA

  //-----------------------------------------------------------------------------
  // Ext bus

  ExtPinOut EXT_PIN_RDn_A;   // PIN_79 <- P08.UGAC
  ExtPinOut EXT_PIN_RDn_D;   // PIN_79 <- P08.URUN
  ExtPinOut EXT_PIN_WRn_A;   // PIN_78 <- P08.UVER
  ExtPinOut EXT_PIN_WRn_D;   // PIN_78 <- P08.USUF
  ExtPinOut EXT_PIN_CSn_A;   // PIN_80 <- P08.TYHO

  ExtPinOut EXT_PIN_A00_A;   // PIN_01 <- P08.KUPO
  ExtPinOut EXT_PIN_A00_D;   // PIN_01 <- P08.KOTY
  ExtPinOut EXT_PIN_A01_A;   // PIN_02 <- P08.CABA
  ExtPinOut EXT_PIN_A01_D;   // PIN_02 <- P08.COTU
  ExtPinOut EXT_PIN_A02_A;   // PIN_03 <- P08.BOKU
  ExtPinOut EXT_PIN_A02_D;   // PIN_03 <- P08.BAJO
  ExtPinOut EXT_PIN_A03_A;   // PIN_04 <- P08.BOTY
  ExtPinOut EXT_PIN_A03_D;   // PIN_04 <- P08.BOLA
  ExtPinOut EXT_PIN_A04_A;   // PIN_05 <- P08.BYLA
  ExtPinOut EXT_PIN_A04_D;   // PIN_05 <- P08.BEVO
  ExtPinOut EXT_PIN_A05_A;   // PIN_06 <- P08.BADU
  ExtPinOut EXT_PIN_A05_D;   // PIN_06 <- P08.AJAV
  ExtPinOut EXT_PIN_A06_A;   // PIN_07 <- P08.CEPU
  ExtPinOut EXT_PIN_A06_D;   // PIN_07 <- P08.CYKA
  ExtPinOut EXT_PIN_A07_A;   // PIN_08 <- P08.DEFY
  ExtPinOut EXT_PIN_A07_D;   // PIN_08 <- P08.COLO
  ExtPinOut EXT_PIN_A08_A;   // PIN_09 <- P08.MYNY
  ExtPinOut EXT_PIN_A08_D;   // PIN_09 <- P08.MEGO
  ExtPinOut EXT_PIN_A09_A;   // PIN_10 <- P08.MUNE
  ExtPinOut EXT_PIN_A09_D;   // PIN_10 <- P08.MENY
  ExtPinOut EXT_PIN_A10_A;   // PIN_11 <- P08.ROXU
  ExtPinOut EXT_PIN_A10_D;   // PIN_11 <- P08.RORE
  ExtPinOut EXT_PIN_A11_A;   // PIN_12 <- P08.LEPY
  ExtPinOut EXT_PIN_A11_D;   // PIN_12 <- P08.LYNY
  ExtPinOut EXT_PIN_A12_A;   // PIN_13 <- P08.LUCE
  ExtPinOut EXT_PIN_A12_D;   // PIN_13 <- P08.LOSO
  ExtPinOut EXT_PIN_A13_A;   // PIN_14 <- P08.LABE
  ExtPinOut EXT_PIN_A13_D;   // PIN_14 <- P08.LEVA
  ExtPinOut EXT_PIN_A14_A;   // PIN_15 <- P08.PUHE
  ExtPinOut EXT_PIN_A14_D;   // PIN_15 <- P08.PAHY
  ExtPinOut EXT_PIN_A15_A;   // PIN_16 <- P08.SUZE
  ExtPinOut EXT_PIN_A15_D;   // PIN_16 <- P08.RULO

  ExtPinOut EXT_PIN_D0_A;    // PIN_17 <- P08.RUXA
  ExtPinOut EXT_PIN_D1_A;    // PIN_18 <- P08.RUJA
  ExtPinOut EXT_PIN_D2_A;    // PIN_19 <- P08.RABY
  ExtPinOut EXT_PIN_D3_A;    // PIN_20 <- P08.RERA
  ExtPinOut EXT_PIN_D4_A;    // PIN_21 <- P08.RORY
  ExtPinOut EXT_PIN_D5_A;    // PIN_22 <- P08.RYVO
  ExtPinOut EXT_PIN_D6_A;    // PIN_23 <- P08.RAFY
  ExtPinOut EXT_PIN_D7_A;    // PIN_24 <- P08.RAVU

  ExtPinOut EXT_PIN_D0_B;    // PIN_17 <- P08.LULA
  ExtPinOut EXT_PIN_D1_B;    // PIN_18 <- P08.LULA
  ExtPinOut EXT_PIN_D2_B;    // PIN_19 <- P08.LULA
  ExtPinOut EXT_PIN_D3_B;    // PIN_20 <- P08.LULA
  ExtPinOut EXT_PIN_D4_B;    // PIN_21 <- P08.LULA
  ExtPinOut EXT_PIN_D5_B;    // PIN_22 <- P08.LULA
  ExtPinOut EXT_PIN_D6_B;    // PIN_23 <- P08.LULA
  ExtPinOut EXT_PIN_D7_B;    // PIN_24 <- P08.LULA

  ExtPinIn  EXT_PIN_D0_C;    // PIN_17 -> P08.TOVO,SOMA
  ExtPinIn  EXT_PIN_D1_C;    // PIN_18 -> P08.RUZY,RONY
  ExtPinIn  EXT_PIN_D2_C;    // PIN_19 -> P08.ROME,RAXY
  ExtPinIn  EXT_PIN_D3_C;    // PIN_20 -> P08.SAZA,SELO
  ExtPinIn  EXT_PIN_D4_C;    // PIN_21 -> P08.TEHE,SODY
  ExtPinIn  EXT_PIN_D5_C;    // PIN_22 -> P08.RATU,SAGO
  ExtPinIn  EXT_PIN_D6_C;    // PIN_23 -> P08.SOCA,RUPA
  ExtPinIn  EXT_PIN_D7_C;    // PIN_24 -> P08.RYBA,SAZY

  ExtPinOut EXT_PIN_D0_D;    // PIN_17 <- P08.RUNE
  ExtPinOut EXT_PIN_D1_D;    // PIN_18 <- P08.RYPU
  ExtPinOut EXT_PIN_D2_D;    // PIN_19 <- P08.SULY
  ExtPinOut EXT_PIN_D3_D;    // PIN_20 <- P08.SEZE
  ExtPinOut EXT_PIN_D4_D;    // PIN_21 <- P08.RESY
  ExtPinOut EXT_PIN_D5_D;    // PIN_22 <- P08.TAMU
  ExtPinOut EXT_PIN_D6_D;    // PIN_23 <- P08.ROGY
  ExtPinOut EXT_PIN_D7_D;    // PIN_24 <- P08.RYDA

  //-----------------------------------------------------------------------------
  // OAM bus

  CpuPinOut OAM_PIN_CLK;
  CpuPinOut OAM_PIN_OE;
  CpuPinOut OAM_PIN_WR_A;
  CpuPinOut OAM_PIN_WR_B;

  CpuPinOut OAM_PIN_A0;
  CpuPinOut OAM_PIN_A1;
  CpuPinOut OAM_PIN_A2;
  CpuPinOut OAM_PIN_A3;
  CpuPinOut OAM_PIN_A4;
  CpuPinOut OAM_PIN_A5;
  CpuPinOut OAM_PIN_A6;
  CpuPinOut OAM_PIN_A7;

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
};

//-----------------------------------------------------------------------------

}; // namespace Schematics