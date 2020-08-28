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
    return (uint16_t)pack_p(EXT_PIN_A00p.qp(), EXT_PIN_A01p.qp(), EXT_PIN_A02p.qp(), EXT_PIN_A03p.qp(),
                            EXT_PIN_A04p.qp(), EXT_PIN_A05p.qp(), EXT_PIN_A06p.qp(), EXT_PIN_A07p.qp(),
                            EXT_PIN_A08p.qp(), EXT_PIN_A09p.qp(), EXT_PIN_A10p.qp(), EXT_PIN_A11p.qp(),
                            EXT_PIN_A12p.qp(), EXT_PIN_A13p.qp(), EXT_PIN_A14p.qp(), EXT_PIN_A15p.qp());
  }

  uint8_t get_pin_data() {
    return (uint8_t)pack_p(EXT_PIN_D00p.qp(), EXT_PIN_D01p.qp(), EXT_PIN_D02p.qp(), EXT_PIN_D03p.qp(),
                           EXT_PIN_D04p.qp(), EXT_PIN_D05p.qp(), EXT_PIN_D06p.qp(), EXT_PIN_D07p.qp());
  }

  void set_pin_data(uint8_t data) {
    EXT_PIN_D00p = (data & 0x01) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D01p = (data & 0x02) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D02p = (data & 0x04) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D03p = (data & 0x08) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D04p = (data & 0x10) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D05p = (data & 0x20) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D06p = (data & 0x40) ? DELTA_TRI1 : DELTA_TRI0;
    EXT_PIN_D07p = (data & 0x80) ? DELTA_TRI1 : DELTA_TRI0;
  }

  void set_pin_data_z() {
    EXT_PIN_D00p = DELTA_TRIZ;
    EXT_PIN_D01p = DELTA_TRIZ;
    EXT_PIN_D02p = DELTA_TRIZ;
    EXT_PIN_D03p = DELTA_TRIZ;
    EXT_PIN_D04p = DELTA_TRIZ;
    EXT_PIN_D05p = DELTA_TRIZ;
    EXT_PIN_D06p = DELTA_TRIZ;
    EXT_PIN_D07p = DELTA_TRIZ;
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
  // Ext bus

  Pin EXT_PIN_CLK  = TRI_D0NP;   // PIN_75
  Pin EXT_PIN_WRn  = TRI_D0NP;   // PIN_78 // WRn idles high, goes low during EFG if there's a write
  Pin EXT_PIN_RDn  = TRI_D0NP;   // PIN_79 // RDn idles low, goes high on phase B for an external write
  Pin EXT_PIN_CSn  = TRI_D0NP;   // PIN_80 // CS changes on phase C if addr in [A000,FDFF]

  Pin EXT_PIN_A00p = TRI_D0NP;   // PIN_01 // Address changees on B for CPU read/write, on A for DMA read
  Pin EXT_PIN_A01p = TRI_D0NP;   // PIN_02 
  Pin EXT_PIN_A02p = TRI_D0NP;   // PIN_03 
  Pin EXT_PIN_A03p = TRI_D0NP;   // PIN_04 
  Pin EXT_PIN_A04p = TRI_D0NP;   // PIN_05 
  Pin EXT_PIN_A05p = TRI_D0NP;   // PIN_06 
  Pin EXT_PIN_A06p = TRI_D0NP;   // PIN_07 
  Pin EXT_PIN_A07p = TRI_D0NP;   // PIN_08 
  Pin EXT_PIN_A08p = TRI_D0NP;   // PIN_09 
  Pin EXT_PIN_A09p = TRI_D0NP;   // PIN_10 
  Pin EXT_PIN_A10p = TRI_D0NP;   // PIN_11 
  Pin EXT_PIN_A11p = TRI_D0NP;   // PIN_12 
  Pin EXT_PIN_A12p = TRI_D0NP;   // PIN_13 
  Pin EXT_PIN_A13p = TRI_D0NP;   // PIN_14 
  Pin EXT_PIN_A14p = TRI_D0NP;   // PIN_15 
  Pin EXT_PIN_A15p = TRI_D0NP;   // PIN_16 // A15 changes on C

  Pin EXT_PIN_D00p = TRI_HZPU;   // PIN_17
  Pin EXT_PIN_D01p = TRI_HZPU;   // PIN_18
  Pin EXT_PIN_D02p = TRI_HZPU;   // PIN_19
  Pin EXT_PIN_D03p = TRI_HZPU;   // PIN_20
  Pin EXT_PIN_D04p = TRI_HZPU;   // PIN_21
  Pin EXT_PIN_D05p = TRI_HZPU;   // PIN_22
  Pin EXT_PIN_D06p = TRI_HZPU;   // PIN_23
  Pin EXT_PIN_D07p = TRI_HZPU;   // PIN_24
};

//-----------------------------------------------------------------------------

}; // namespace Schematics