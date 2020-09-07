#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ExtBus {
  void tock(SchematicTop& top);
  void dump(Dumper& d) const;

  uint16_t get_pin_addr() {
    return (uint16_t)pack_p(PIN_EXT_A00p.qp(), PIN_EXT_A01p.qp(), PIN_EXT_A02p.qp(), PIN_EXT_A03p.qp(),
                            PIN_EXT_A04p.qp(), PIN_EXT_A05p.qp(), PIN_EXT_A06p.qp(), PIN_EXT_A07p.qp(),
                            PIN_EXT_A08p.qp(), PIN_EXT_A09p.qp(), PIN_EXT_A10p.qp(), PIN_EXT_A11p.qp(),
                            PIN_EXT_A12p.qp(), PIN_EXT_A13p.qp(), PIN_EXT_A14p.qp(), PIN_EXT_A15p.qp());
  }

  uint8_t get_pin_data() {
    return (uint8_t)pack_p(PIN_EXT_D00p.qp(), PIN_EXT_D01p.qp(), PIN_EXT_D02p.qp(), PIN_EXT_D03p.qp(),
                           PIN_EXT_D04p.qp(), PIN_EXT_D05p.qp(), PIN_EXT_D06p.qp(), PIN_EXT_D07p.qp());
  }

  void set_pin_data(uint8_t data) {
    PIN_EXT_D00p = (data & 0x01) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D01p = (data & 0x02) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D02p = (data & 0x04) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D03p = (data & 0x08) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D04p = (data & 0x10) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D05p = (data & 0x20) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D06p = (data & 0x40) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_EXT_D07p = (data & 0x80) ? DELTA_TRI1 : DELTA_TRI0;
  }

  void set_pin_data_z() {
    PIN_EXT_D00p = DELTA_TRIZ;
    PIN_EXT_D01p = DELTA_TRIZ;
    PIN_EXT_D02p = DELTA_TRIZ;
    PIN_EXT_D03p = DELTA_TRIZ;
    PIN_EXT_D04p = DELTA_TRIZ;
    PIN_EXT_D05p = DELTA_TRIZ;
    PIN_EXT_D06p = DELTA_TRIZ;
    PIN_EXT_D07p = DELTA_TRIZ;
  }

  //-----------------------------------------------------------------------------

  /*p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p = TRI_D0NP;
  /*p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p = TRI_D0NP;
  /*p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p = TRI_D0NP;
  /*p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p = TRI_D0NP;
  /*p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p = TRI_D0NP;
  /*p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p = TRI_D0NP;
  /*p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p = TRI_D0NP;
  /*p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p = TRI_D0NP;
  /*p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p = TRI_D0NP;
  /*p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p = TRI_D0NP;
  /*p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p = TRI_D0NP;
  /*p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p = TRI_D0NP;
  /*p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p = TRI_D0NP;
  /*p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p = TRI_D0NP;
  /*p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p = TRI_D0NP;

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n = TRI_D0NP;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n = TRI_D0NP;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n = TRI_D0NP;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n = TRI_D0NP;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n = TRI_D0NP;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n = TRI_D0NP;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n = TRI_D0NP;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n = TRI_D0NP;

  //-----------------------------------------------------------------------------
  // Ext bus

  Pin PIN_EXT_CLK  = TRI_D0NP;   // PIN_75
  Pin PIN_EXT_WRn  = TRI_D0NP;   // PIN_78 // WRn idles high, goes low during EFG if there's a write
  Pin PIN_EXT_RDn  = TRI_D0NP;   // PIN_79 // RDn idles low, goes high on phase B for an external write
  Pin PIN_EXT_CSn  = TRI_D0NP;   // PIN_80 // CS changes on phase C if addr in [A000,FDFF]

  Pin PIN_EXT_A00p = TRI_D0NP;   // PIN_01 // Address changees on B for CPU read/write, on A for DMA read
  Pin PIN_EXT_A01p = TRI_D0NP;   // PIN_02 
  Pin PIN_EXT_A02p = TRI_D0NP;   // PIN_03 
  Pin PIN_EXT_A03p = TRI_D0NP;   // PIN_04 
  Pin PIN_EXT_A04p = TRI_D0NP;   // PIN_05 
  Pin PIN_EXT_A05p = TRI_D0NP;   // PIN_06 
  Pin PIN_EXT_A06p = TRI_D0NP;   // PIN_07 
  Pin PIN_EXT_A07p = TRI_D0NP;   // PIN_08 
  Pin PIN_EXT_A08p = TRI_D0NP;   // PIN_09 
  Pin PIN_EXT_A09p = TRI_D0NP;   // PIN_10 
  Pin PIN_EXT_A10p = TRI_D0NP;   // PIN_11 
  Pin PIN_EXT_A11p = TRI_D0NP;   // PIN_12 
  Pin PIN_EXT_A12p = TRI_D0NP;   // PIN_13 
  Pin PIN_EXT_A13p = TRI_D0NP;   // PIN_14 
  Pin PIN_EXT_A14p = TRI_D0NP;   // PIN_15 
  Pin PIN_EXT_A15p = TRI_D0NP;   // PIN_16 // A15 changes on C

  Pin PIN_EXT_D00p = TRI_HZPU;   // PIN_17
  Pin PIN_EXT_D01p = TRI_HZPU;   // PIN_18
  Pin PIN_EXT_D02p = TRI_HZPU;   // PIN_19
  Pin PIN_EXT_D03p = TRI_HZPU;   // PIN_20
  Pin PIN_EXT_D04p = TRI_HZPU;   // PIN_21
  Pin PIN_EXT_D05p = TRI_HZPU;   // PIN_22
  Pin PIN_EXT_D06p = TRI_HZPU;   // PIN_23
  Pin PIN_EXT_D07p = TRI_HZPU;   // PIN_24
};

//-----------------------------------------------------------------------------

}; // namespace Schematics