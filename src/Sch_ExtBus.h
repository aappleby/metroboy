#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct ExtBus {
  ExtBus();
  void tock(SchematicTop& top);
  SignalHash commit();

  void set_ext_data(uint16_t data) {
    _EXT_PIN_D0_C.set(data & 0x01);
    _EXT_PIN_D1_C.set(data & 0x02);
    _EXT_PIN_D2_C.set(data & 0x04);
    _EXT_PIN_D3_C.set(data & 0x08);
    _EXT_PIN_D4_C.set(data & 0x10);
    _EXT_PIN_D5_C.set(data & 0x20);
    _EXT_PIN_D6_C.set(data & 0x40);
    _EXT_PIN_D7_C.set(data & 0x80);
  }

  void set_ext_rdwr(wire rd, wire wr) {
    _EXT_PIN_WRp_C.set(rd);
    _EXT_PIN_RDp_C.set(wr);
  }

  wire EXT_PIN_RDp_C() const { return _EXT_PIN_RDp_C; }
  wire EXT_PIN_WRp_C() const { return _EXT_PIN_WRp_C; }

  // -> oam data tri
  /*p25.RALO*/ wire RALO_EXT_D0p() const { return not(_EXT_PIN_D0_C); }
  /*p25.TUNE*/ wire TUNE_EXT_D1p() const { return not(_EXT_PIN_D1_C); }
  /*p25.SERA*/ wire SERA_EXT_D2p() const { return not(_EXT_PIN_D2_C); }
  /*p25.TENU*/ wire TENU_EXT_D3p() const { return not(_EXT_PIN_D3_C); }
  /*p25.SYSA*/ wire SYSA_EXT_D4p() const { return not(_EXT_PIN_D4_C); }
  /*p25.SUGY*/ wire SUGY_EXT_D5p() const { return not(_EXT_PIN_D5_C); }
  /*p25.TUBE*/ wire TUBE_EXT_D6p() const { return not(_EXT_PIN_D6_C); }
  /*p25.SYZO*/ wire SYZO_EXT_D7p() const { return not(_EXT_PIN_D7_C); }

private:

  /*p08.ALOR*/ TpLatch _ALOR_EXT_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch _APUR_EXT_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch _ALYR_EXT_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch _ARET_EXT_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch _AVYS_EXT_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch _ATEV_EXT_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch _AROS_EXT_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch _ARYM_EXT_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch _LUNO_EXT_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch _LYSA_EXT_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch _PATE_EXT_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch _LUMY_EXT_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch _LOBU_EXT_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch _LONU_EXT_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch _NYRE_EXT_ADDR_LATCH_14;

  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch _SOMA_EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch _RONY_EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch _RAXY_EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch _SELO_EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch _SODY_EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch _SAGO_EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch _RUPA_EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch _SAZY_EXT_DATA_LATCH_07;

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  ExtPinIn  _EXT_PIN_WRp_C;   // PIN_78 -> P07.UBAL
  ExtPinIn  _EXT_PIN_RDp_C;   // PIN_79 -> P07.UJYV

  ExtPinIn  _EXT_PIN_A00_C;   // PIN_01 -> P08.KOVA
  ExtPinIn  _EXT_PIN_A01_C;   // PIN_02 -> P08.CAMU
  ExtPinIn  _EXT_PIN_A02_C;   // PIN_03 -> P08.BUXU
  ExtPinIn  _EXT_PIN_A03_C;   // PIN_04 -> P08.BASE
  ExtPinIn  _EXT_PIN_A04_C;   // PIN_05 -> P08.AFEC
  ExtPinIn  _EXT_PIN_A05_C;   // PIN_06 -> P08.ABUP
  ExtPinIn  _EXT_PIN_A06_C;   // PIN_07 -> P08.CYGU
  ExtPinIn  _EXT_PIN_A07_C;   // PIN_08 -> P08.COGO
  ExtPinIn  _EXT_PIN_A08_C;   // PIN_09 -> P08.MUJY
  ExtPinIn  _EXT_PIN_A09_C;   // PIN_10 -> P08.NENA
  ExtPinIn  _EXT_PIN_A10_C;   // PIN_11 -> P08.SURA
  ExtPinIn  _EXT_PIN_A11_C;   // PIN_12 -> P08.MADY
  ExtPinIn  _EXT_PIN_A12_C;   // PIN_13 -> P08.LAHE
  ExtPinIn  _EXT_PIN_A13_C;   // PIN_14 -> P08.LURA
  ExtPinIn  _EXT_PIN_A14_C;   // PIN_15 -> P08.PEVO
  ExtPinIn  _EXT_PIN_A15_C;   // PIN_16 -> P08.RAZA

  //-----------------------------------------------------------------------------
  // Ext bus

  ExtPinOut _EXT_PIN_RDn_A;   // PIN_79 <- P08.UGAC
  ExtPinOut _EXT_PIN_RDn_D;   // PIN_79 <- P08.URUN
  ExtPinOut _EXT_PIN_WRn_A;   // PIN_78 <- P08.UVER
  ExtPinOut _EXT_PIN_WRn_D;   // PIN_78 <- P08.USUF
  ExtPinOut _EXT_PIN_CSn_A;   // PIN_80 <- P08.TYHO

  ExtPinOut _EXT_PIN_A00_A;   // PIN_01 <- P08.KUPO
  ExtPinOut _EXT_PIN_A00_D;   // PIN_01 <- P08.KOTY
  ExtPinOut _EXT_PIN_A01_A;   // PIN_02 <- P08.CABA
  ExtPinOut _EXT_PIN_A01_D;   // PIN_02 <- P08.COTU
  ExtPinOut _EXT_PIN_A02_A;   // PIN_03 <- P08.BOKU
  ExtPinOut _EXT_PIN_A02_D;   // PIN_03 <- P08.BAJO
  ExtPinOut _EXT_PIN_A03_A;   // PIN_04 <- P08.BOTY
  ExtPinOut _EXT_PIN_A03_D;   // PIN_04 <- P08.BOLA
  ExtPinOut _EXT_PIN_A04_A;   // PIN_05 <- P08.BYLA
  ExtPinOut _EXT_PIN_A04_D;   // PIN_05 <- P08.BEVO
  ExtPinOut _EXT_PIN_A05_A;   // PIN_06 <- P08.BADU
  ExtPinOut _EXT_PIN_A05_D;   // PIN_06 <- P08.AJAV
  ExtPinOut _EXT_PIN_A06_A;   // PIN_07 <- P08.CEPU
  ExtPinOut _EXT_PIN_A06_D;   // PIN_07 <- P08.CYKA
  ExtPinOut _EXT_PIN_A07_A;   // PIN_08 <- P08.DEFY
  ExtPinOut _EXT_PIN_A07_D;   // PIN_08 <- P08.COLO
  ExtPinOut _EXT_PIN_A08_A;   // PIN_09 <- P08.MYNY
  ExtPinOut _EXT_PIN_A08_D;   // PIN_09 <- P08.MEGO
  ExtPinOut _EXT_PIN_A09_A;   // PIN_10 <- P08.MUNE
  ExtPinOut _EXT_PIN_A09_D;   // PIN_10 <- P08.MENY
  ExtPinOut _EXT_PIN_A10_A;   // PIN_11 <- P08.ROXU
  ExtPinOut _EXT_PIN_A10_D;   // PIN_11 <- P08.RORE
  ExtPinOut _EXT_PIN_A11_A;   // PIN_12 <- P08.LEPY
  ExtPinOut _EXT_PIN_A11_D;   // PIN_12 <- P08.LYNY
  ExtPinOut _EXT_PIN_A12_A;   // PIN_13 <- P08.LUCE
  ExtPinOut _EXT_PIN_A12_D;   // PIN_13 <- P08.LOSO
  ExtPinOut _EXT_PIN_A13_A;   // PIN_14 <- P08.LABE
  ExtPinOut _EXT_PIN_A13_D;   // PIN_14 <- P08.LEVA
  ExtPinOut _EXT_PIN_A14_A;   // PIN_15 <- P08.PUHE
  ExtPinOut _EXT_PIN_A14_D;   // PIN_15 <- P08.PAHY
  ExtPinOut _EXT_PIN_A15_A;   // PIN_16 <- P08.SUZE
  ExtPinOut _EXT_PIN_A15_D;   // PIN_16 <- P08.RULO

  ExtPinOut _EXT_PIN_D0_A;    // PIN_17 <- P08.RUXA
  ExtPinOut _EXT_PIN_D1_A;    // PIN_18 <- P08.RUJA
  ExtPinOut _EXT_PIN_D2_A;    // PIN_19 <- P08.RABY
  ExtPinOut _EXT_PIN_D3_A;    // PIN_20 <- P08.RERA
  ExtPinOut _EXT_PIN_D4_A;    // PIN_21 <- P08.RORY
  ExtPinOut _EXT_PIN_D5_A;    // PIN_22 <- P08.RYVO
  ExtPinOut _EXT_PIN_D6_A;    // PIN_23 <- P08.RAFY
  ExtPinOut _EXT_PIN_D7_A;    // PIN_24 <- P08.RAVU

  ExtPinOut _EXT_PIN_D0_B;    // PIN_17 <- P08.LULA
  ExtPinOut _EXT_PIN_D1_B;    // PIN_18 <- P08.LULA
  ExtPinOut _EXT_PIN_D2_B;    // PIN_19 <- P08.LULA
  ExtPinOut _EXT_PIN_D3_B;    // PIN_20 <- P08.LULA
  ExtPinOut _EXT_PIN_D4_B;    // PIN_21 <- P08.LULA
  ExtPinOut _EXT_PIN_D5_B;    // PIN_22 <- P08.LULA
  ExtPinOut _EXT_PIN_D6_B;    // PIN_23 <- P08.LULA
  ExtPinOut _EXT_PIN_D7_B;    // PIN_24 <- P08.LULA

  ExtPinIn  _EXT_PIN_D0_C;    // PIN_17 -> P08.TOVO,SOMA
  ExtPinIn  _EXT_PIN_D1_C;    // PIN_18 -> P08.RUZY,RONY
  ExtPinIn  _EXT_PIN_D2_C;    // PIN_19 -> P08.ROME,RAXY
  ExtPinIn  _EXT_PIN_D3_C;    // PIN_20 -> P08.SAZA,SELO
  ExtPinIn  _EXT_PIN_D4_C;    // PIN_21 -> P08.TEHE,SODY
  ExtPinIn  _EXT_PIN_D5_C;    // PIN_22 -> P08.RATU,SAGO
  ExtPinIn  _EXT_PIN_D6_C;    // PIN_23 -> P08.SOCA,RUPA
  ExtPinIn  _EXT_PIN_D7_C;    // PIN_24 -> P08.RYBA,SAZY

  ExtPinOut _EXT_PIN_D0_D;    // PIN_17 <- P08.RUNE
  ExtPinOut _EXT_PIN_D1_D;    // PIN_18 <- P08.RYPU
  ExtPinOut _EXT_PIN_D2_D;    // PIN_19 <- P08.SULY
  ExtPinOut _EXT_PIN_D3_D;    // PIN_20 <- P08.SEZE
  ExtPinOut _EXT_PIN_D4_D;    // PIN_21 <- P08.RESY
  ExtPinOut _EXT_PIN_D5_D;    // PIN_22 <- P08.TAMU
  ExtPinOut _EXT_PIN_D6_D;    // PIN_23 <- P08.ROGY
  ExtPinOut _EXT_PIN_D7_D;    // PIN_24 <- P08.RYDA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics