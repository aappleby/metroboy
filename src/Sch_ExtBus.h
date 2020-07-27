#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct ExtBus {
  void tock(SchematicTop& top);

  uint16_t get_pin_addr() {
    return (uint16_t)pack(!_EXT_PIN_A00_A, !_EXT_PIN_A01_A, !_EXT_PIN_A02_A, !_EXT_PIN_A03_A,
                          !_EXT_PIN_A04_A, !_EXT_PIN_A05_A, !_EXT_PIN_A06_A, !_EXT_PIN_A07_A,
                          !_EXT_PIN_A08_A, !_EXT_PIN_A09_A, !_EXT_PIN_A10_A, !_EXT_PIN_A11_A,
                          !_EXT_PIN_A12_A, !_EXT_PIN_A13_A, !_EXT_PIN_A14_A, !_EXT_PIN_A15_A);
  }

  uint8_t get_pin_data_out() {
    return (uint8_t)pack(!_EXT_PIN_D0_A, !_EXT_PIN_D1_A, !_EXT_PIN_D2_A, !_EXT_PIN_D3_A,
                         !_EXT_PIN_D4_A, !_EXT_PIN_D5_A, !_EXT_PIN_D6_A, !_EXT_PIN_D7_A);
  }

  void hold_pin_data_in(uint8_t data) {
    _EXT_PIN_D0_C.hold(data & 0x01);
    _EXT_PIN_D1_C.hold(data & 0x02);
    _EXT_PIN_D2_C.hold(data & 0x04);
    _EXT_PIN_D3_C.hold(data & 0x08);
    _EXT_PIN_D4_C.hold(data & 0x10);
    _EXT_PIN_D5_C.hold(data & 0x20);
    _EXT_PIN_D6_C.hold(data & 0x40);
    _EXT_PIN_D7_C.hold(data & 0x80);
  }

  void hold_pin_data_z() {
    _EXT_PIN_D0_C.hold_z();
    _EXT_PIN_D1_C.hold_z();
    _EXT_PIN_D2_C.hold_z();
    _EXT_PIN_D3_C.hold_z();
    _EXT_PIN_D4_C.hold_z();
    _EXT_PIN_D5_C.hold_z();
    _EXT_PIN_D6_C.hold_z();
    _EXT_PIN_D7_C.hold_z();
  }


  void dump(Dumper& d) {
    d("---------- Ext Bus  ----------\n");
    d("EXT BUS ADDR    : _%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      _NYRE_EXT_ADDR_LATCH_14.c(), _LONU_EXT_ADDR_LATCH_13.c(), _LOBU_EXT_ADDR_LATCH_12.c(), _LUMY_EXT_ADDR_LATCH_11.c(),
      _PATE_EXT_ADDR_LATCH_10.c(), _LYSA_EXT_ADDR_LATCH_09.c(), _LUNO_EXT_ADDR_LATCH_08.c(), _ARYM_EXT_ADDR_LATCH_07.c(),
      _AROS_EXT_ADDR_LATCH_06.c(), _ATEV_EXT_ADDR_LATCH_05.c(), _AVYS_EXT_ADDR_LATCH_04.c(), _ARET_EXT_ADDR_LATCH_03.c(),
      _ALYR_EXT_ADDR_LATCH_02.c(), _APUR_EXT_ADDR_LATCH_01.c(), _ALOR_EXT_ADDR_LATCH_00.c());
    d("EXT BUS DATA    : %c%c%c%c%c%c%c%c\n",
      _SAZY_EXT_DATA_LATCH_07.c(), _RUPA_EXT_DATA_LATCH_06.c(), _SAGO_EXT_DATA_LATCH_05.c(), _SODY_EXT_DATA_LATCH_04.c(),
      _SELO_EXT_DATA_LATCH_03.c(), _RAXY_EXT_DATA_LATCH_02.c(), _RONY_EXT_DATA_LATCH_01.c(), _SOMA_EXT_DATA_LATCH_00.c());
    d("\n");

    d("EXT PIN RD_A    : %c%c%c\n", _EXT_PIN_RD_A.c(), _EXT_PIN_RD_C.c(), _EXT_PIN_RD_D.c());
    d("EXT PIN WR_A    : %c%c%c\n", _EXT_PIN_WR_A.c(), _EXT_PIN_WR_C.c(), _EXT_PIN_WR_D.c());
    d("EXT PIN CS_A    : %c\n",     _EXT_PIN_CS_A.c());

    d("EXT PIN ADDR_A  : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      _EXT_PIN_A15_A.c(), _EXT_PIN_A14_A.c(), _EXT_PIN_A13_A.c(), _EXT_PIN_A12_A.c(),
      _EXT_PIN_A11_A.c(), _EXT_PIN_A10_A.c(), _EXT_PIN_A09_A.c(), _EXT_PIN_A08_A.c(),
      _EXT_PIN_A07_A.c(), _EXT_PIN_A06_A.c(), _EXT_PIN_A05_A.c(), _EXT_PIN_A04_A.c(),
      _EXT_PIN_A03_A.c(), _EXT_PIN_A02_A.c(), _EXT_PIN_A01_A.c(), _EXT_PIN_A00_A.c());

    d("EXT PIN DATA_A  : %c%c%c%c%c%c%c%c\n",
      _EXT_PIN_D7_A.c(), _EXT_PIN_D6_A.c(), _EXT_PIN_D5_A.c(), _EXT_PIN_D4_A.c(),
      _EXT_PIN_D3_A.c(), _EXT_PIN_D2_A.c(), _EXT_PIN_D1_A.c(), _EXT_PIN_D0_A.c());
    d("EXT PIN DATA_B  : %c%c%c%c%c%c%c%c\n",
      _EXT_PIN_D7_B.c(), _EXT_PIN_D6_B.c(), _EXT_PIN_D5_B.c(), _EXT_PIN_D4_B.c(),
      _EXT_PIN_D3_B.c(), _EXT_PIN_D2_B.c(), _EXT_PIN_D1_B.c(), _EXT_PIN_D0_B.c());
    d("EXT PIN DATA_C  : %c%c%c%c%c%c%c%c\n",
      _EXT_PIN_D7_C.c(), _EXT_PIN_D6_C.c(), _EXT_PIN_D5_C.c(), _EXT_PIN_D4_C.c(),
      _EXT_PIN_D3_C.c(), _EXT_PIN_D2_C.c(), _EXT_PIN_D1_C.c(), _EXT_PIN_D0_C.c());
    d("EXT PIN DATA_D  : %c%c%c%c%c%c%c%c\n",
      _EXT_PIN_D7_D.c(), _EXT_PIN_D6_D.c(), _EXT_PIN_D5_D.c(), _EXT_PIN_D4_D.c(),
      _EXT_PIN_D3_D.c(), _EXT_PIN_D2_D.c(), _EXT_PIN_D1_D.c(), _EXT_PIN_D0_D.c());

    d("\n");
  }

//private:

  //-----------------------------------------------------------------------------

  /*p08.ALOR*/ Pin2 _ALOR_EXT_ADDR_LATCH_00 = Pin2::LATCH_0;
  /*p08.APUR*/ Pin2 _APUR_EXT_ADDR_LATCH_01 = Pin2::LATCH_0;
  /*p08.ALYR*/ Pin2 _ALYR_EXT_ADDR_LATCH_02 = Pin2::LATCH_0;
  /*p08.ARET*/ Pin2 _ARET_EXT_ADDR_LATCH_03 = Pin2::LATCH_0;
  /*p08.AVYS*/ Pin2 _AVYS_EXT_ADDR_LATCH_04 = Pin2::LATCH_0;
  /*p08.ATEV*/ Pin2 _ATEV_EXT_ADDR_LATCH_05 = Pin2::LATCH_0;
  /*p08.AROS*/ Pin2 _AROS_EXT_ADDR_LATCH_06 = Pin2::LATCH_0;
  /*p08.ARYM*/ Pin2 _ARYM_EXT_ADDR_LATCH_07 = Pin2::LATCH_0;
  /*p08.LUNO*/ Pin2 _LUNO_EXT_ADDR_LATCH_08 = Pin2::LATCH_0;
  /*p08.LYSA*/ Pin2 _LYSA_EXT_ADDR_LATCH_09 = Pin2::LATCH_0;
  /*p08.PATE*/ Pin2 _PATE_EXT_ADDR_LATCH_10 = Pin2::LATCH_0;
  /*p08.LUMY*/ Pin2 _LUMY_EXT_ADDR_LATCH_11 = Pin2::LATCH_0;
  /*p08.LOBU*/ Pin2 _LOBU_EXT_ADDR_LATCH_12 = Pin2::LATCH_0;
  /*p08.LONU*/ Pin2 _LONU_EXT_ADDR_LATCH_13 = Pin2::LATCH_0;
  /*p08.NYRE*/ Pin2 _NYRE_EXT_ADDR_LATCH_14 = Pin2::LATCH_0;

  /*p08.SOMA*/ Pin2 _SOMA_EXT_DATA_LATCH_00 = Pin2::LATCH_0;
  /*p08.RONY*/ Pin2 _RONY_EXT_DATA_LATCH_01 = Pin2::LATCH_0;
  /*p08.RAXY*/ Pin2 _RAXY_EXT_DATA_LATCH_02 = Pin2::LATCH_0;
  /*p08.SELO*/ Pin2 _SELO_EXT_DATA_LATCH_03 = Pin2::LATCH_0;
  /*p08.SODY*/ Pin2 _SODY_EXT_DATA_LATCH_04 = Pin2::LATCH_0;
  /*p08.SAGO*/ Pin2 _SAGO_EXT_DATA_LATCH_05 = Pin2::LATCH_0;
  /*p08.RUPA*/ Pin2 _RUPA_EXT_DATA_LATCH_06 = Pin2::LATCH_0;
  /*p08.SAZY*/ Pin2 _SAZY_EXT_DATA_LATCH_07 = Pin2::LATCH_0;

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  Pin2  _EXT_PIN_WR_C  = Pin2::HOLD_0;   // PIN_78 -> P07.UBAL
  Pin2  _EXT_PIN_RD_C  = Pin2::HOLD_0;   // PIN_79 -> P07.UJYV

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

  Pin2 _EXT_PIN_RD_A  = Pin2::HIZ_NP;   // PIN_79 <- P08.UGAC // RDn idles low, goes high on phase B for an external write
  Pin2 _EXT_PIN_RD_D  = Pin2::HIZ_NP;   // PIN_79 <- P08.URUN
  Pin2 _EXT_PIN_WR_A  = Pin2::HIZ_NP;   // PIN_78 <- P08.UVER // WRn idles high, goes low during EFG if there's a write
  Pin2 _EXT_PIN_WR_D  = Pin2::HIZ_NP;   // PIN_78 <- P08.USUF
  Pin2 _EXT_PIN_CS_A  = Pin2::HIZ_NP;   // PIN_80 <- P08.TYHO // CS changes on phase C if addr in [A000,FDFF]

  Pin2 _EXT_PIN_A00_A = Pin2::HIZ_NP;   // PIN_01 <- P08.KUPO // Address changees on B for CPU read/write, on A for DMA read
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

  Pin2 _EXT_PIN_A15_A = Pin2::HIZ_NP;   // PIN_16 <- P08.SUZE // A15 changes on C
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