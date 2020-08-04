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
    return (uint16_t)pack(EXT_PIN_A00_A.q(), EXT_PIN_A01_A.q(), EXT_PIN_A02_A.q(), EXT_PIN_A03_A.q(),
                          EXT_PIN_A04_A.q(), EXT_PIN_A05_A.q(), EXT_PIN_A06_A.q(), EXT_PIN_A07_A.q(),
                          EXT_PIN_A08_A.q(), EXT_PIN_A09_A.q(), EXT_PIN_A10_A.q(), EXT_PIN_A11_A.q(),
                          EXT_PIN_A12_A.q(), EXT_PIN_A13_A.q(), EXT_PIN_A14_A.q(), EXT_PIN_A15_A.q());
  }

  uint8_t get_pin_data_out() {
    return (uint8_t)pack(EXT_PIN_D0_A.q(), EXT_PIN_D1_A.q(), EXT_PIN_D2_A.q(), EXT_PIN_D3_A.q(),
                         EXT_PIN_D4_A.q(), EXT_PIN_D5_A.q(), EXT_PIN_D6_A.q(), EXT_PIN_D7_A.q());
  }

  void preset_pin_data_in(uint8_t data) {
    EXT_PIN_D0_C.preset(data & 0x01);
    EXT_PIN_D1_C.preset(data & 0x02);
    EXT_PIN_D2_C.preset(data & 0x04);
    EXT_PIN_D3_C.preset(data & 0x08);
    EXT_PIN_D4_C.preset(data & 0x10);
    EXT_PIN_D5_C.preset(data & 0x20);
    EXT_PIN_D6_C.preset(data & 0x40);
    EXT_PIN_D7_C.preset(data & 0x80);
  }

  void preset_pin_data_z() {
    EXT_PIN_D0_C.preset(DELTA_TRIZ);
    EXT_PIN_D1_C.preset(DELTA_TRIZ);
    EXT_PIN_D2_C.preset(DELTA_TRIZ);
    EXT_PIN_D3_C.preset(DELTA_TRIZ);
    EXT_PIN_D4_C.preset(DELTA_TRIZ);
    EXT_PIN_D5_C.preset(DELTA_TRIZ);
    EXT_PIN_D6_C.preset(DELTA_TRIZ);
    EXT_PIN_D7_C.preset(DELTA_TRIZ);
  }


  void dump(Dumper& d) const {
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

    d("EXT PIN RD_A    : %c%c%c\n", EXT_PIN_RD_A.c(), EXT_PIN_RD_C.c(), EXT_PIN_RD_D.c());
    d("EXT PIN WR_A    : %c%c%c\n", EXT_PIN_WR_A.c(), EXT_PIN_WR_C.c(), EXT_PIN_WR_D.c());
    d("EXT PIN CS_A    : %c\n",     EXT_PIN_CS_A.c());

    d("EXT PIN ADDR_A  : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      EXT_PIN_A15_A.c(), EXT_PIN_A14_A.c(), EXT_PIN_A13_A.c(), EXT_PIN_A12_A.c(),
      EXT_PIN_A11_A.c(), EXT_PIN_A10_A.c(), EXT_PIN_A09_A.c(), EXT_PIN_A08_A.c(),
      EXT_PIN_A07_A.c(), EXT_PIN_A06_A.c(), EXT_PIN_A05_A.c(), EXT_PIN_A04_A.c(),
      EXT_PIN_A03_A.c(), EXT_PIN_A02_A.c(), EXT_PIN_A01_A.c(), EXT_PIN_A00_A.c());

    d("EXT PIN DATA_A  : %c%c%c%c%c%c%c%c\n",
      EXT_PIN_D7_A.c(), EXT_PIN_D6_A.c(), EXT_PIN_D5_A.c(), EXT_PIN_D4_A.c(),
      EXT_PIN_D3_A.c(), EXT_PIN_D2_A.c(), EXT_PIN_D1_A.c(), EXT_PIN_D0_A.c());
    d("EXT PIN DATA_B  : %c%c%c%c%c%c%c%c\n",
      EXT_PIN_D7_B.c(), EXT_PIN_D6_B.c(), EXT_PIN_D5_B.c(), EXT_PIN_D4_B.c(),
      EXT_PIN_D3_B.c(), EXT_PIN_D2_B.c(), EXT_PIN_D1_B.c(), EXT_PIN_D0_B.c());
    d("EXT PIN DATA_C  : %c%c%c%c%c%c%c%c\n",
      EXT_PIN_D7_C.c(), EXT_PIN_D6_C.c(), EXT_PIN_D5_C.c(), EXT_PIN_D4_C.c(),
      EXT_PIN_D3_C.c(), EXT_PIN_D2_C.c(), EXT_PIN_D1_C.c(), EXT_PIN_D0_C.c());
    d("EXT PIN DATA_D  : %c%c%c%c%c%c%c%c\n",
      EXT_PIN_D7_D.c(), EXT_PIN_D6_D.c(), EXT_PIN_D5_D.c(), EXT_PIN_D4_D.c(),
      EXT_PIN_D3_D.c(), EXT_PIN_D2_D.c(), EXT_PIN_D1_D.c(), EXT_PIN_D0_D.c());

    d("\n");
  }

//private:

  //-----------------------------------------------------------------------------

  /*p08.ALOR*/ Tri _ALOR_EXT_ADDR_LATCH_00 = TRI_D0NP;
  /*p08.APUR*/ Tri _APUR_EXT_ADDR_LATCH_01 = TRI_D0NP;
  /*p08.ALYR*/ Tri _ALYR_EXT_ADDR_LATCH_02 = TRI_D0NP;
  /*p08.ARET*/ Tri _ARET_EXT_ADDR_LATCH_03 = TRI_D0NP;
  /*p08.AVYS*/ Tri _AVYS_EXT_ADDR_LATCH_04 = TRI_D0NP;
  /*p08.ATEV*/ Tri _ATEV_EXT_ADDR_LATCH_05 = TRI_D0NP;
  /*p08.AROS*/ Tri _AROS_EXT_ADDR_LATCH_06 = TRI_D0NP;
  /*p08.ARYM*/ Tri _ARYM_EXT_ADDR_LATCH_07 = TRI_D0NP;
  /*p08.LUNO*/ Tri _LUNO_EXT_ADDR_LATCH_08 = TRI_D0NP;
  /*p08.LYSA*/ Tri _LYSA_EXT_ADDR_LATCH_09 = TRI_D0NP;
  /*p08.PATE*/ Tri _PATE_EXT_ADDR_LATCH_10 = TRI_D0NP;
  /*p08.LUMY*/ Tri _LUMY_EXT_ADDR_LATCH_11 = TRI_D0NP;
  /*p08.LOBU*/ Tri _LOBU_EXT_ADDR_LATCH_12 = TRI_D0NP;
  /*p08.LONU*/ Tri _LONU_EXT_ADDR_LATCH_13 = TRI_D0NP;
  /*p08.NYRE*/ Tri _NYRE_EXT_ADDR_LATCH_14 = TRI_D0NP;

  /*p08.SOMA*/ Tri _SOMA_EXT_DATA_LATCH_00 = TRI_D0NP;
  /*p08.RONY*/ Tri _RONY_EXT_DATA_LATCH_01 = TRI_D0NP;
  /*p08.RAXY*/ Tri _RAXY_EXT_DATA_LATCH_02 = TRI_D0NP;
  /*p08.SELO*/ Tri _SELO_EXT_DATA_LATCH_03 = TRI_D0NP;
  /*p08.SODY*/ Tri _SODY_EXT_DATA_LATCH_04 = TRI_D0NP;
  /*p08.SAGO*/ Tri _SAGO_EXT_DATA_LATCH_05 = TRI_D0NP;
  /*p08.RUPA*/ Tri _RUPA_EXT_DATA_LATCH_06 = TRI_D0NP;
  /*p08.SAZY*/ Tri _SAZY_EXT_DATA_LATCH_07 = TRI_D0NP;

  //-----------------------------------------------------------------------------
  // Ext bus debug inputs

  Tri  EXT_PIN_WR_C  = TRI_D0NP;   // PIN_78 -> P07.UBAL
  Tri  EXT_PIN_RD_C  = TRI_D0NP;   // PIN_79 -> P07.UJYV

  Tri  EXT_PIN_A00_C = TRI_D0NP;   // PIN_01 -> P08.KOVA
  Tri  EXT_PIN_A01_C = TRI_D0NP;   // PIN_02 -> P08.CAMU
  Tri  EXT_PIN_A02_C = TRI_D0NP;   // PIN_03 -> P08.BUXU
  Tri  EXT_PIN_A03_C = TRI_D0NP;   // PIN_04 -> P08.BASE
  Tri  EXT_PIN_A04_C = TRI_D0NP;   // PIN_05 -> P08.AFEC
  Tri  EXT_PIN_A05_C = TRI_D0NP;   // PIN_06 -> P08.ABUP
  Tri  EXT_PIN_A06_C = TRI_D0NP;   // PIN_07 -> P08.CYGU
  Tri  EXT_PIN_A07_C = TRI_D0NP;   // PIN_08 -> P08.COGO
  Tri  EXT_PIN_A08_C = TRI_D0NP;   // PIN_09 -> P08.MUJY
  Tri  EXT_PIN_A09_C = TRI_D0NP;   // PIN_10 -> P08.NENA
  Tri  EXT_PIN_A10_C = TRI_D0NP;   // PIN_11 -> P08.SURA
  Tri  EXT_PIN_A11_C = TRI_D0NP;   // PIN_12 -> P08.MADY
  Tri  EXT_PIN_A12_C = TRI_D0NP;   // PIN_13 -> P08.LAHE
  Tri  EXT_PIN_A13_C = TRI_D0NP;   // PIN_14 -> P08.LURA
  Tri  EXT_PIN_A14_C = TRI_D0NP;   // PIN_15 -> P08.PEVO
  Tri  EXT_PIN_A15_C = TRI_D0NP;   // PIN_16 -> P08.RAZA

  //-----------------------------------------------------------------------------
  // Ext bus

  Tri EXT_PIN_RD_A  = TRI_HZNP;   // PIN_79 <- P08.UGAC // RDn idles low, goes high on phase B for an external write
  Tri EXT_PIN_RD_D  = TRI_HZNP;   // PIN_79 <- P08.URUN
  Tri EXT_PIN_WR_A  = TRI_HZNP;   // PIN_78 <- P08.UVER // WRn idles high, goes low during EFG if there's a write
  Tri EXT_PIN_WR_D  = TRI_HZNP;   // PIN_78 <- P08.USUF
  Tri EXT_PIN_CS_A  = TRI_HZNP;   // PIN_80 <- P08.TYHO // CS changes on phase C if addr in [A000,FDFF]

  Tri EXT_PIN_A00_A = TRI_HZNP;   // PIN_01 <- P08.KUPO // Address changees on B for CPU read/write, on A for DMA read
  Tri EXT_PIN_A01_A = TRI_HZNP;   // PIN_02 <- P08.CABA
  Tri EXT_PIN_A02_A = TRI_HZNP;   // PIN_03 <- P08.BOKU
  Tri EXT_PIN_A03_A = TRI_HZNP;   // PIN_04 <- P08.BOTY
  Tri EXT_PIN_A04_A = TRI_HZNP;   // PIN_05 <- P08.BYLA
  Tri EXT_PIN_A05_A = TRI_HZNP;   // PIN_06 <- P08.BADU
  Tri EXT_PIN_A06_A = TRI_HZNP;   // PIN_07 <- P08.CEPU
  Tri EXT_PIN_A07_A = TRI_HZNP;   // PIN_08 <- P08.DEFY
  Tri EXT_PIN_A08_A = TRI_HZNP;   // PIN_09 <- P08.MYNY
  Tri EXT_PIN_A09_A = TRI_HZNP;   // PIN_10 <- P08.MUNE
  Tri EXT_PIN_A10_A = TRI_HZNP;   // PIN_11 <- P08.ROXU
  Tri EXT_PIN_A11_A = TRI_HZNP;   // PIN_12 <- P08.LEPY
  Tri EXT_PIN_A12_A = TRI_HZNP;   // PIN_13 <- P08.LUCE
  Tri EXT_PIN_A13_A = TRI_HZNP;   // PIN_14 <- P08.LABE
  Tri EXT_PIN_A14_A = TRI_HZNP;   // PIN_15 <- P08.PUHE
  Tri EXT_PIN_A15_A = TRI_HZNP;   // PIN_16 <- P08.SUZE // A15 changes on C

  Tri EXT_PIN_A00_D = TRI_HZNP;   // PIN_01 <- P08.KOTY
  Tri EXT_PIN_A01_D = TRI_HZNP;   // PIN_02 <- P08.COTU
  Tri EXT_PIN_A02_D = TRI_HZNP;   // PIN_03 <- P08.BAJO
  Tri EXT_PIN_A03_D = TRI_HZNP;   // PIN_04 <- P08.BOLA
  Tri EXT_PIN_A04_D = TRI_HZNP;   // PIN_05 <- P08.BEVO
  Tri EXT_PIN_A05_D = TRI_HZNP;   // PIN_06 <- P08.AJAV
  Tri EXT_PIN_A06_D = TRI_HZNP;   // PIN_07 <- P08.CYKA
  Tri EXT_PIN_A07_D = TRI_HZNP;   // PIN_08 <- P08.COLO
  Tri EXT_PIN_A08_D = TRI_HZNP;   // PIN_09 <- P08.MEGO
  Tri EXT_PIN_A09_D = TRI_HZNP;   // PIN_10 <- P08.MENY
  Tri EXT_PIN_A10_D = TRI_HZNP;   // PIN_11 <- P08.RORE
  Tri EXT_PIN_A11_D = TRI_HZNP;   // PIN_12 <- P08.LYNY
  Tri EXT_PIN_A12_D = TRI_HZNP;   // PIN_13 <- P08.LOSO
  Tri EXT_PIN_A13_D = TRI_HZNP;   // PIN_14 <- P08.LEVA
  Tri EXT_PIN_A14_D = TRI_HZNP;   // PIN_15 <- P08.PAHY
  Tri EXT_PIN_A15_D = TRI_HZNP;   // PIN_16 <- P08.RULO

  Tri EXT_PIN_D0_A = TRI_HZNP;    // PIN_17 <- P08.RUXA
  Tri EXT_PIN_D1_A = TRI_HZNP;    // PIN_18 <- P08.RUJA
  Tri EXT_PIN_D2_A = TRI_HZNP;    // PIN_19 <- P08.RABY
  Tri EXT_PIN_D3_A = TRI_HZNP;    // PIN_20 <- P08.RERA
  Tri EXT_PIN_D4_A = TRI_HZNP;    // PIN_21 <- P08.RORY
  Tri EXT_PIN_D5_A = TRI_HZNP;    // PIN_22 <- P08.RYVO
  Tri EXT_PIN_D6_A = TRI_HZNP;    // PIN_23 <- P08.RAFY
  Tri EXT_PIN_D7_A = TRI_HZNP;    // PIN_24 <- P08.RAVU

  Tri EXT_PIN_D0_B = TRI_HZNP;    // PIN_17 <- P08.LULA
  Tri EXT_PIN_D1_B = TRI_HZNP;    // PIN_18 <- P08.LULA
  Tri EXT_PIN_D2_B = TRI_HZNP;    // PIN_19 <- P08.LULA
  Tri EXT_PIN_D3_B = TRI_HZNP;    // PIN_20 <- P08.LULA
  Tri EXT_PIN_D4_B = TRI_HZNP;    // PIN_21 <- P08.LULA
  Tri EXT_PIN_D5_B = TRI_HZNP;    // PIN_22 <- P08.LULA
  Tri EXT_PIN_D6_B = TRI_HZNP;    // PIN_23 <- P08.LULA
  Tri EXT_PIN_D7_B = TRI_HZNP;    // PIN_24 <- P08.LULA

  Tri EXT_PIN_D0_C = TRI_D0NP;    // PIN_17 -> P08.TOVO (debug something), SOMA (ext data latch) RALO (oam input)
  Tri EXT_PIN_D1_C = TRI_D0NP;    // PIN_18 -> P08.RUZY,RONY
  Tri EXT_PIN_D2_C = TRI_D0NP;    // PIN_19 -> P08.ROME,RAXY
  Tri EXT_PIN_D3_C = TRI_D0NP;    // PIN_20 -> P08.SAZA,SELO
  Tri EXT_PIN_D4_C = TRI_D0NP;    // PIN_21 -> P08.TEHE,SODY
  Tri EXT_PIN_D5_C = TRI_D0NP;    // PIN_22 -> P08.RATU,SAGO
  Tri EXT_PIN_D6_C = TRI_D0NP;    // PIN_23 -> P08.SOCA,RUPA
  Tri EXT_PIN_D7_C = TRI_D0NP;    // PIN_24 -> P08.RYBA,SAZY

  Tri EXT_PIN_D0_D = TRI_HZNP;    // PIN_17 <- P08.RUNE
  Tri EXT_PIN_D1_D = TRI_HZNP;    // PIN_18 <- P08.RYPU
  Tri EXT_PIN_D2_D = TRI_HZNP;    // PIN_19 <- P08.SULY
  Tri EXT_PIN_D3_D = TRI_HZNP;    // PIN_20 <- P08.SEZE
  Tri EXT_PIN_D4_D = TRI_HZNP;    // PIN_21 <- P08.RESY
  Tri EXT_PIN_D5_D = TRI_HZNP;    // PIN_22 <- P08.TAMU
  Tri EXT_PIN_D6_D = TRI_HZNP;    // PIN_23 <- P08.ROGY
  Tri EXT_PIN_D7_D = TRI_HZNP;    // PIN_24 <- P08.RYDA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics