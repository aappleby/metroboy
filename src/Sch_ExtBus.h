#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct ExtBus {
  void tock(SchematicTop& top);
  SignalHash commit();

  void set_ext_data(uint16_t data) {
    _EXT_PIN_D0_C.hold(data & 0x01);
    _EXT_PIN_D1_C.hold(data & 0x02);
    _EXT_PIN_D2_C.hold(data & 0x04);
    _EXT_PIN_D3_C.hold(data & 0x08);
    _EXT_PIN_D4_C.hold(data & 0x10);
    _EXT_PIN_D5_C.hold(data & 0x20);
    _EXT_PIN_D6_C.hold(data & 0x40);
    _EXT_PIN_D7_C.hold(data & 0x80);
  }

  void set_ext_rdwr(wire rd, wire wr) {
    _EXT_PIN_WRp_C.hold(rd);
    _EXT_PIN_RDp_C.hold(wr);
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

  /*p08.ALOR*/ Reg2 _ALOR_EXT_ADDR_LATCH_00 = Reg2::D0C0;
  /*p08.APUR*/ Reg2 _APUR_EXT_ADDR_LATCH_01 = Reg2::D0C0;
  /*p08.ALYR*/ Reg2 _ALYR_EXT_ADDR_LATCH_02 = Reg2::D0C0;
  /*p08.ARET*/ Reg2 _ARET_EXT_ADDR_LATCH_03 = Reg2::D0C0;
  /*p08.AVYS*/ Reg2 _AVYS_EXT_ADDR_LATCH_04 = Reg2::D0C0;
  /*p08.ATEV*/ Reg2 _ATEV_EXT_ADDR_LATCH_05 = Reg2::D0C0;
  /*p08.AROS*/ Reg2 _AROS_EXT_ADDR_LATCH_06 = Reg2::D0C0;
  /*p08.ARYM*/ Reg2 _ARYM_EXT_ADDR_LATCH_07 = Reg2::D0C0;
  /*p08.LUNO*/ Reg2 _LUNO_EXT_ADDR_LATCH_08 = Reg2::D0C0;
  /*p08.LYSA*/ Reg2 _LYSA_EXT_ADDR_LATCH_09 = Reg2::D0C0;
  /*p08.PATE*/ Reg2 _PATE_EXT_ADDR_LATCH_10 = Reg2::D0C0;
  /*p08.LUMY*/ Reg2 _LUMY_EXT_ADDR_LATCH_11 = Reg2::D0C0;
  /*p08.LOBU*/ Reg2 _LOBU_EXT_ADDR_LATCH_12 = Reg2::D0C0;
  /*p08.LONU*/ Reg2 _LONU_EXT_ADDR_LATCH_13 = Reg2::D0C0;
  /*p08.NYRE*/ Reg2 _NYRE_EXT_ADDR_LATCH_14 = Reg2::D0C0;

  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ Reg2 _SOMA_EXT_DATA_LATCH_00 = Reg2::D0C0;
  /*p08.RONY*/ Reg2 _RONY_EXT_DATA_LATCH_01 = Reg2::D0C0;
  /*p08.RAXY*/ Reg2 _RAXY_EXT_DATA_LATCH_02 = Reg2::D0C0;
  /*p08.SELO*/ Reg2 _SELO_EXT_DATA_LATCH_03 = Reg2::D0C0;
  /*p08.SODY*/ Reg2 _SODY_EXT_DATA_LATCH_04 = Reg2::D0C0;
  /*p08.SAGO*/ Reg2 _SAGO_EXT_DATA_LATCH_05 = Reg2::D0C0;
  /*p08.RUPA*/ Reg2 _RUPA_EXT_DATA_LATCH_06 = Reg2::D0C0;
  /*p08.SAZY*/ Reg2 _SAZY_EXT_DATA_LATCH_07 = Reg2::D0C0;

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  Pin2  _EXT_PIN_WRp_C = Pin2::HOLD_0;   // PIN_78 -> P07.UBAL
  Pin2  _EXT_PIN_RDp_C = Pin2::HOLD_0;   // PIN_79 -> P07.UJYV

  Pin2  _EXT_PIN_A00_C = Pin2::HOLD_0;   // PIN_01 -> P08.KOVA
  Pin2  _EXT_PIN_A01_C = Pin2::HOLD_0;   // PIN_02 -> P08.CAMU
  Pin2  _EXT_PIN_A02_C = Pin2::HOLD_0;   // PIN_03 -> P08.BUXU
  Pin2  _EXT_PIN_A03_C = Pin2::HOLD_0;   // PIN_04 -> P08.BASE
  Pin2  _EXT_PIN_A04_C = Pin2::HOLD_0;   // PIN_05 -> P08.AFEC
  Pin2  _EXT_PIN_A05_C = Pin2::HOLD_0;   // PIN_06 -> P08.ABUP
  Pin2  _EXT_PIN_A06_C = Pin2::HOLD_0;   // PIN_07 -> P08.CYGU
  Pin2  _EXT_PIN_A07_C = Pin2::HOLD_0;   // PIN_08 -> P08.COGO
  Pin2  _EXT_PIN_A08_C = Pin2::HOLD_0;   // PIN_09 -> P08.MUJY
  Pin2  _EXT_PIN_A09_C = Pin2::HOLD_0;   // PIN_10 -> P08.NENA
  Pin2  _EXT_PIN_A10_C = Pin2::HOLD_0;   // PIN_11 -> P08.SURA
  Pin2  _EXT_PIN_A11_C = Pin2::HOLD_0;   // PIN_12 -> P08.MADY
  Pin2  _EXT_PIN_A12_C = Pin2::HOLD_0;   // PIN_13 -> P08.LAHE
  Pin2  _EXT_PIN_A13_C = Pin2::HOLD_0;   // PIN_14 -> P08.LURA
  Pin2  _EXT_PIN_A14_C = Pin2::HOLD_0;   // PIN_15 -> P08.PEVO
  Pin2  _EXT_PIN_A15_C = Pin2::HOLD_0;   // PIN_16 -> P08.RAZA

  //-----------------------------------------------------------------------------
  // Ext bus

  Pin2 _EXT_PIN_RDn_A = Pin2::HIZ_NP;   // PIN_79 <- P08.UGAC
  Pin2 _EXT_PIN_RDn_D = Pin2::HIZ_NP;   // PIN_79 <- P08.URUN
  Pin2 _EXT_PIN_WRn_A = Pin2::HIZ_NP;   // PIN_78 <- P08.UVER
  Pin2 _EXT_PIN_WRn_D = Pin2::HIZ_NP;   // PIN_78 <- P08.USUF
  Pin2 _EXT_PIN_CSn_A = Pin2::HIZ_NP;   // PIN_80 <- P08.TYHO

  Pin2 _EXT_PIN_A00_A = Pin2::HIZ_NP;   // PIN_01 <- P08.KUPO
  Pin2 _EXT_PIN_A00_D = Pin2::HIZ_NP;   // PIN_01 <- P08.KOTY
  Pin2 _EXT_PIN_A01_A = Pin2::HIZ_NP;   // PIN_02 <- P08.CABA
  Pin2 _EXT_PIN_A01_D = Pin2::HIZ_NP;   // PIN_02 <- P08.COTU
  Pin2 _EXT_PIN_A02_A = Pin2::HIZ_NP;   // PIN_03 <- P08.BOKU
  Pin2 _EXT_PIN_A02_D = Pin2::HIZ_NP;   // PIN_03 <- P08.BAJO
  Pin2 _EXT_PIN_A03_A = Pin2::HIZ_NP;   // PIN_04 <- P08.BOTY
  Pin2 _EXT_PIN_A03_D = Pin2::HIZ_NP;   // PIN_04 <- P08.BOLA
  Pin2 _EXT_PIN_A04_A = Pin2::HIZ_NP;   // PIN_05 <- P08.BYLA
  Pin2 _EXT_PIN_A04_D = Pin2::HIZ_NP;   // PIN_05 <- P08.BEVO
  Pin2 _EXT_PIN_A05_A = Pin2::HIZ_NP;   // PIN_06 <- P08.BADU
  Pin2 _EXT_PIN_A05_D = Pin2::HIZ_NP;   // PIN_06 <- P08.AJAV
  Pin2 _EXT_PIN_A06_A = Pin2::HIZ_NP;   // PIN_07 <- P08.CEPU
  Pin2 _EXT_PIN_A06_D = Pin2::HIZ_NP;   // PIN_07 <- P08.CYKA
  Pin2 _EXT_PIN_A07_A = Pin2::HIZ_NP;   // PIN_08 <- P08.DEFY
  Pin2 _EXT_PIN_A07_D = Pin2::HIZ_NP;   // PIN_08 <- P08.COLO
  Pin2 _EXT_PIN_A08_A = Pin2::HIZ_NP;   // PIN_09 <- P08.MYNY
  Pin2 _EXT_PIN_A08_D = Pin2::HIZ_NP;   // PIN_09 <- P08.MEGO
  Pin2 _EXT_PIN_A09_A = Pin2::HIZ_NP;   // PIN_10 <- P08.MUNE
  Pin2 _EXT_PIN_A09_D = Pin2::HIZ_NP;   // PIN_10 <- P08.MENY
  Pin2 _EXT_PIN_A10_A = Pin2::HIZ_NP;   // PIN_11 <- P08.ROXU
  Pin2 _EXT_PIN_A10_D = Pin2::HIZ_NP;   // PIN_11 <- P08.RORE
  Pin2 _EXT_PIN_A11_A = Pin2::HIZ_NP;   // PIN_12 <- P08.LEPY
  Pin2 _EXT_PIN_A11_D = Pin2::HIZ_NP;   // PIN_12 <- P08.LYNY
  Pin2 _EXT_PIN_A12_A = Pin2::HIZ_NP;   // PIN_13 <- P08.LUCE
  Pin2 _EXT_PIN_A12_D = Pin2::HIZ_NP;   // PIN_13 <- P08.LOSO
  Pin2 _EXT_PIN_A13_A = Pin2::HIZ_NP;   // PIN_14 <- P08.LABE
  Pin2 _EXT_PIN_A13_D = Pin2::HIZ_NP;   // PIN_14 <- P08.LEVA
  Pin2 _EXT_PIN_A14_A = Pin2::HIZ_NP;   // PIN_15 <- P08.PUHE
  Pin2 _EXT_PIN_A14_D = Pin2::HIZ_NP;   // PIN_15 <- P08.PAHY
  Pin2 _EXT_PIN_A15_A = Pin2::HIZ_NP;   // PIN_16 <- P08.SUZE
  Pin2 _EXT_PIN_A15_D = Pin2::HIZ_NP;   // PIN_16 <- P08.RULO

  Pin2 _EXT_PIN_D0_A = Pin2::HIZ_NP;    // PIN_17 <- P08.RUXA
  Pin2 _EXT_PIN_D1_A = Pin2::HIZ_NP;    // PIN_18 <- P08.RUJA
  Pin2 _EXT_PIN_D2_A = Pin2::HIZ_NP;    // PIN_19 <- P08.RABY
  Pin2 _EXT_PIN_D3_A = Pin2::HIZ_NP;    // PIN_20 <- P08.RERA
  Pin2 _EXT_PIN_D4_A = Pin2::HIZ_NP;    // PIN_21 <- P08.RORY
  Pin2 _EXT_PIN_D5_A = Pin2::HIZ_NP;    // PIN_22 <- P08.RYVO
  Pin2 _EXT_PIN_D6_A = Pin2::HIZ_NP;    // PIN_23 <- P08.RAFY
  Pin2 _EXT_PIN_D7_A = Pin2::HIZ_NP;    // PIN_24 <- P08.RAVU

  Pin2 _EXT_PIN_D0_B = Pin2::HIZ_NP;    // PIN_17 <- P08.LULA
  Pin2 _EXT_PIN_D1_B = Pin2::HIZ_NP;    // PIN_18 <- P08.LULA
  Pin2 _EXT_PIN_D2_B = Pin2::HIZ_NP;    // PIN_19 <- P08.LULA
  Pin2 _EXT_PIN_D3_B = Pin2::HIZ_NP;    // PIN_20 <- P08.LULA
  Pin2 _EXT_PIN_D4_B = Pin2::HIZ_NP;    // PIN_21 <- P08.LULA
  Pin2 _EXT_PIN_D5_B = Pin2::HIZ_NP;    // PIN_22 <- P08.LULA
  Pin2 _EXT_PIN_D6_B = Pin2::HIZ_NP;    // PIN_23 <- P08.LULA
  Pin2 _EXT_PIN_D7_B = Pin2::HIZ_NP;    // PIN_24 <- P08.LULA

  Pin2 _EXT_PIN_D0_C = Pin2::HOLD_0;    // PIN_17 -> P08.TOVO,SOMA
  Pin2 _EXT_PIN_D1_C = Pin2::HOLD_0;    // PIN_18 -> P08.RUZY,RONY
  Pin2 _EXT_PIN_D2_C = Pin2::HOLD_0;    // PIN_19 -> P08.ROME,RAXY
  Pin2 _EXT_PIN_D3_C = Pin2::HOLD_0;    // PIN_20 -> P08.SAZA,SELO
  Pin2 _EXT_PIN_D4_C = Pin2::HOLD_0;    // PIN_21 -> P08.TEHE,SODY
  Pin2 _EXT_PIN_D5_C = Pin2::HOLD_0;    // PIN_22 -> P08.RATU,SAGO
  Pin2 _EXT_PIN_D6_C = Pin2::HOLD_0;    // PIN_23 -> P08.SOCA,RUPA
  Pin2 _EXT_PIN_D7_C = Pin2::HOLD_0;    // PIN_24 -> P08.RYBA,SAZY

  Pin2 _EXT_PIN_D0_D = Pin2::HIZ_NP;    // PIN_17 <- P08.RUNE
  Pin2 _EXT_PIN_D1_D = Pin2::HIZ_NP;    // PIN_18 <- P08.RYPU
  Pin2 _EXT_PIN_D2_D = Pin2::HIZ_NP;    // PIN_19 <- P08.SULY
  Pin2 _EXT_PIN_D3_D = Pin2::HIZ_NP;    // PIN_20 <- P08.SEZE
  Pin2 _EXT_PIN_D4_D = Pin2::HIZ_NP;    // PIN_21 <- P08.RESY
  Pin2 _EXT_PIN_D5_D = Pin2::HIZ_NP;    // PIN_22 <- P08.TAMU
  Pin2 _EXT_PIN_D6_D = Pin2::HIZ_NP;    // PIN_23 <- P08.ROGY
  Pin2 _EXT_PIN_D7_D = Pin2::HIZ_NP;    // PIN_24 <- P08.RYDA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics