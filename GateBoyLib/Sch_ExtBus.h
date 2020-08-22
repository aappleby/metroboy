#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct ExtBus {
  void tock(SchematicTop& top);
  void dump(Dumper& d) const;

  uint16_t get_pin_addr() {
    return (uint16_t)pack_p(!EXT_PIN_A00n_A.tp(), !EXT_PIN_A01n_A.tp(), !EXT_PIN_A02n_A.tp(), !EXT_PIN_A03n_A.tp(),
                            !EXT_PIN_A04n_A.tp(), !EXT_PIN_A05n_A.tp(), !EXT_PIN_A06n_A.tp(), !EXT_PIN_A07n_A.tp(),
                            !EXT_PIN_A08n_A.tp(), !EXT_PIN_A09n_A.tp(), !EXT_PIN_A10n_A.tp(), !EXT_PIN_A11n_A.tp(),
                            !EXT_PIN_A12n_A.tp(), !EXT_PIN_A13n_A.tp(), !EXT_PIN_A14n_A.tp(), !EXT_PIN_A15n_A.tp());
  }

  uint8_t get_pin_data_out() {
    return (uint8_t)pack_p(!EXT_PIN_D0n_A.tp(), !EXT_PIN_D1n_A.tp(), !EXT_PIN_D2n_A.tp(), !EXT_PIN_D3n_A.tp(),
                           !EXT_PIN_D4n_A.tp(), !EXT_PIN_D5n_A.tp(), !EXT_PIN_D6n_A.tp(), !EXT_PIN_D7n_A.tp());
  }

  void set_pin_data_in(uint8_t data) {
    EXT_PIN_D0n_C = !(data & 0x01);
    EXT_PIN_D1n_C = !(data & 0x02);
    EXT_PIN_D2n_C = !(data & 0x04);
    EXT_PIN_D3n_C = !(data & 0x08);
    EXT_PIN_D4n_C = !(data & 0x10);
    EXT_PIN_D5n_C = !(data & 0x20);
    EXT_PIN_D6n_C = !(data & 0x40);
    EXT_PIN_D7n_C = !(data & 0x80);
  }

  void set_pin_data_z() {
    EXT_PIN_D0n_C = DELTA_TRIZ;
    EXT_PIN_D1n_C = DELTA_TRIZ;
    EXT_PIN_D2n_C = DELTA_TRIZ;
    EXT_PIN_D3n_C = DELTA_TRIZ;
    EXT_PIN_D4n_C = DELTA_TRIZ;
    EXT_PIN_D5n_C = DELTA_TRIZ;
    EXT_PIN_D6n_C = DELTA_TRIZ;
    EXT_PIN_D7n_C = DELTA_TRIZ;
  }

  //-----------------------------------------------------------------------------

  /*p08.ALOR*/ Tri ALOR_EXT_ADDR_LATCH_00p = TRI_D0NP;
  /*p08.APUR*/ Tri APUR_EXT_ADDR_LATCH_01p = TRI_D0NP;
  /*p08.ALYR*/ Tri ALYR_EXT_ADDR_LATCH_02p = TRI_D0NP;
  /*p08.ARET*/ Tri ARET_EXT_ADDR_LATCH_03p = TRI_D0NP;
  /*p08.AVYS*/ Tri AVYS_EXT_ADDR_LATCH_04p = TRI_D0NP;
  /*p08.ATEV*/ Tri ATEV_EXT_ADDR_LATCH_05p = TRI_D0NP;
  /*p08.AROS*/ Tri AROS_EXT_ADDR_LATCH_06p = TRI_D0NP;
  /*p08.ARYM*/ Tri ARYM_EXT_ADDR_LATCH_07p = TRI_D0NP;
  /*p08.LUNO*/ Tri LUNO_EXT_ADDR_LATCH_08p = TRI_D0NP;
  /*p08.LYSA*/ Tri LYSA_EXT_ADDR_LATCH_09p = TRI_D0NP;
  /*p08.PATE*/ Tri PATE_EXT_ADDR_LATCH_10p = TRI_D0NP;
  /*p08.LUMY*/ Tri LUMY_EXT_ADDR_LATCH_11p = TRI_D0NP;
  /*p08.LOBU*/ Tri LOBU_EXT_ADDR_LATCH_12p = TRI_D0NP;
  /*p08.LONU*/ Tri LONU_EXT_ADDR_LATCH_13p = TRI_D0NP;
  /*p08.NYRE*/ Tri NYRE_EXT_ADDR_LATCH_14p = TRI_D0NP;

  /*p08.SOMA*/ Tri SOMA_EXT_DATA_LATCH_D0n = TRI_D0NP;
  /*p08.RONY*/ Tri RONY_EXT_DATA_LATCH_D1n = TRI_D0NP;
  /*p08.RAXY*/ Tri RAXY_EXT_DATA_LATCH_D2n = TRI_D0NP;
  /*p08.SELO*/ Tri SELO_EXT_DATA_LATCH_D3n = TRI_D0NP;
  /*p08.SODY*/ Tri SODY_EXT_DATA_LATCH_D4n = TRI_D0NP;
  /*p08.SAGO*/ Tri SAGO_EXT_DATA_LATCH_D5n = TRI_D0NP;
  /*p08.RUPA*/ Tri RUPA_EXT_DATA_LATCH_D6n = TRI_D0NP;
  /*p08.SAZY*/ Tri SAZY_EXT_DATA_LATCH_D7n = TRI_D0NP;

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

  Tri EXT_PIN_RD_A   = TRI_D0NP;   // PIN_79 <- P08.UGAC // RDn idles low, goes high on phase B for an external write
  Tri EXT_PIN_RD_D   = TRI_D0NP;   // PIN_79 <- P08.URUN
  Tri EXT_PIN_WR_A   = TRI_D0NP;   // PIN_78 <- P08.UVER // WRn idles high, goes low during EFG if there's a write
  Tri EXT_PIN_WR_D   = TRI_D0NP;   // PIN_78 <- P08.USUF
  Tri EXT_PIN_CS_A   = TRI_D0NP;   // PIN_80 <- P08.TYHO // CS changes on phase C if addr in [A000,FDFF]

  Tri EXT_PIN_A00n_A = TRI_D0NP;   // PIN_01 <- P08.KUPO // Address changees on B for CPU read/write, on A for DMA read
  Tri EXT_PIN_A01n_A = TRI_D0NP;   // PIN_02 <- P08.CABA
  Tri EXT_PIN_A02n_A = TRI_D0NP;   // PIN_03 <- P08.BOKU
  Tri EXT_PIN_A03n_A = TRI_D0NP;   // PIN_04 <- P08.BOTY
  Tri EXT_PIN_A04n_A = TRI_D0NP;   // PIN_05 <- P08.BYLA
  Tri EXT_PIN_A05n_A = TRI_D0NP;   // PIN_06 <- P08.BADU
  Tri EXT_PIN_A06n_A = TRI_D0NP;   // PIN_07 <- P08.CEPU
  Tri EXT_PIN_A07n_A = TRI_D0NP;   // PIN_08 <- P08.DEFY
  Tri EXT_PIN_A08n_A = TRI_D0NP;   // PIN_09 <- P08.MYNY
  Tri EXT_PIN_A09n_A = TRI_D0NP;   // PIN_10 <- P08.MUNE
  Tri EXT_PIN_A10n_A = TRI_D0NP;   // PIN_11 <- P08.ROXU
  Tri EXT_PIN_A11n_A = TRI_D0NP;   // PIN_12 <- P08.LEPY
  Tri EXT_PIN_A12n_A = TRI_D0NP;   // PIN_13 <- P08.LUCE
  Tri EXT_PIN_A13n_A = TRI_D0NP;   // PIN_14 <- P08.LABE
  Tri EXT_PIN_A14n_A = TRI_D0NP;   // PIN_15 <- P08.PUHE
  Tri EXT_PIN_A15n_A = TRI_D0NP;   // PIN_16 <- P08.SUZE // A15 changes on C

  Tri EXT_PIN_A00n_D = TRI_D0NP;   // PIN_01 <- P08.KOTY
  Tri EXT_PIN_A01n_D = TRI_D0NP;   // PIN_02 <- P08.COTU
  Tri EXT_PIN_A02n_D = TRI_D0NP;   // PIN_03 <- P08.BAJO
  Tri EXT_PIN_A03n_D = TRI_D0NP;   // PIN_04 <- P08.BOLA
  Tri EXT_PIN_A04n_D = TRI_D0NP;   // PIN_05 <- P08.BEVO
  Tri EXT_PIN_A05n_D = TRI_D0NP;   // PIN_06 <- P08.AJAV
  Tri EXT_PIN_A06n_D = TRI_D0NP;   // PIN_07 <- P08.CYKA
  Tri EXT_PIN_A07n_D = TRI_D0NP;   // PIN_08 <- P08.COLO
  Tri EXT_PIN_A08n_D = TRI_D0NP;   // PIN_09 <- P08.MEGO
  Tri EXT_PIN_A09n_D = TRI_D0NP;   // PIN_10 <- P08.MENY
  Tri EXT_PIN_A10n_D = TRI_D0NP;   // PIN_11 <- P08.RORE
  Tri EXT_PIN_A11n_D = TRI_D0NP;   // PIN_12 <- P08.LYNY
  Tri EXT_PIN_A12n_D = TRI_D0NP;   // PIN_13 <- P08.LOSO
  Tri EXT_PIN_A13n_D = TRI_D0NP;   // PIN_14 <- P08.LEVA
  Tri EXT_PIN_A14n_D = TRI_D0NP;   // PIN_15 <- P08.PAHY
  Tri EXT_PIN_A15n_D = TRI_D0NP;   // PIN_16 <- P08.RULO

  Tri EXT_PIN_D0n_A = TRI_D0NP;    // PIN_17 <- P08.RUXA
  Tri EXT_PIN_D1n_A = TRI_D0NP;    // PIN_18 <- P08.RUJA
  Tri EXT_PIN_D2n_A = TRI_D0NP;    // PIN_19 <- P08.RABY
  Tri EXT_PIN_D3n_A = TRI_D0NP;    // PIN_20 <- P08.RERA
  Tri EXT_PIN_D4n_A = TRI_D0NP;    // PIN_21 <- P08.RORY
  Tri EXT_PIN_D5n_A = TRI_D0NP;    // PIN_22 <- P08.RYVO
  Tri EXT_PIN_D6n_A = TRI_D0NP;    // PIN_23 <- P08.RAFY
  Tri EXT_PIN_D7n_A = TRI_D0NP;    // PIN_24 <- P08.RAVU

  Tri EXT_PIN_D0_B  = TRI_D0NP;    // PIN_17 <- P08.LULA
  Tri EXT_PIN_D1_B  = TRI_D0NP;    // PIN_18 <- P08.LULA
  Tri EXT_PIN_D2_B  = TRI_D0NP;    // PIN_19 <- P08.LULA
  Tri EXT_PIN_D3_B  = TRI_D0NP;    // PIN_20 <- P08.LULA
  Tri EXT_PIN_D4_B  = TRI_D0NP;    // PIN_21 <- P08.LULA
  Tri EXT_PIN_D5_B  = TRI_D0NP;    // PIN_22 <- P08.LULA
  Tri EXT_PIN_D6_B  = TRI_D0NP;    // PIN_23 <- P08.LULA
  Tri EXT_PIN_D7_B  = TRI_D0NP;    // PIN_24 <- P08.LULA

  Tri EXT_PIN_D0n_C = TRI_D0NP;    // PIN_17 -> P08.TOVO (debug something), SOMA (ext data latch) RALO (oam input)
  Tri EXT_PIN_D1n_C = TRI_D0NP;    // PIN_18 -> P08.RUZY,RONY
  Tri EXT_PIN_D2n_C = TRI_D0NP;    // PIN_19 -> P08.ROME,RAXY
  Tri EXT_PIN_D3n_C = TRI_D0NP;    // PIN_20 -> P08.SAZA,SELO
  Tri EXT_PIN_D4n_C = TRI_D0NP;    // PIN_21 -> P08.TEHE,SODY
  Tri EXT_PIN_D5n_C = TRI_D0NP;    // PIN_22 -> P08.RATU,SAGO
  Tri EXT_PIN_D6n_C = TRI_D0NP;    // PIN_23 -> P08.SOCA,RUPA
  Tri EXT_PIN_D7n_C = TRI_D0NP;    // PIN_24 -> P08.RYBA,SAZY

  Tri EXT_PIN_D0n_D = TRI_D0NP;    // PIN_17 <- P08.RUNE
  Tri EXT_PIN_D1n_D = TRI_D0NP;    // PIN_18 <- P08.RYPU
  Tri EXT_PIN_D2n_D = TRI_D0NP;    // PIN_19 <- P08.SULY
  Tri EXT_PIN_D3n_D = TRI_D0NP;    // PIN_20 <- P08.SEZE
  Tri EXT_PIN_D4n_D = TRI_D0NP;    // PIN_21 <- P08.RESY
  Tri EXT_PIN_D5n_D = TRI_D0NP;    // PIN_22 <- P08.TAMU
  Tri EXT_PIN_D6n_D = TRI_D0NP;    // PIN_23 <- P08.ROGY
  Tri EXT_PIN_D7n_D = TRI_D0NP;    // PIN_24 <- P08.RYDA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics