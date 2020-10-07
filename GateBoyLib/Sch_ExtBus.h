#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ExtBus {
  void reset() {
  }

  void reset_bootrom() {
    ALOR_EXT_ADDR_LATCH_00p.reset(TRI_D0NP);
    APUR_EXT_ADDR_LATCH_01p.reset(TRI_D0NP);
    ALYR_EXT_ADDR_LATCH_02p.reset(TRI_D0NP);
    ARET_EXT_ADDR_LATCH_03p.reset(TRI_D0NP);
    AVYS_EXT_ADDR_LATCH_04p.reset(TRI_D0NP);
    ATEV_EXT_ADDR_LATCH_05p.reset(TRI_D0NP);
    AROS_EXT_ADDR_LATCH_06p.reset(TRI_D0NP);
    ARYM_EXT_ADDR_LATCH_07p.reset(TRI_D0NP);
    LUNO_EXT_ADDR_LATCH_08p.reset(TRI_D0NP);
    LYSA_EXT_ADDR_LATCH_09p.reset(TRI_D0NP);
    PATE_EXT_ADDR_LATCH_10p.reset(TRI_D0NP);
    LUMY_EXT_ADDR_LATCH_11p.reset(TRI_D0NP);
    LOBU_EXT_ADDR_LATCH_12p.reset(TRI_D0NP);
    LONU_EXT_ADDR_LATCH_13p.reset(TRI_D0NP);
    NYRE_EXT_ADDR_LATCH_14p.reset(TRI_D0NP);

    SOMA_EXT_DATA_LATCH_D0n.reset(TRI_D0NP);
    RONY_EXT_DATA_LATCH_D1n.reset(TRI_D0NP);
    RAXY_EXT_DATA_LATCH_D2n.reset(TRI_D0NP);
    SELO_EXT_DATA_LATCH_D3n.reset(TRI_D0NP);
    SODY_EXT_DATA_LATCH_D4n.reset(TRI_D0NP);
    SAGO_EXT_DATA_LATCH_D5n.reset(TRI_D0NP);
    RUPA_EXT_DATA_LATCH_D6n.reset(TRI_D0NP);
    SAZY_EXT_DATA_LATCH_D7n.reset(TRI_D0NP);

    PIN_EXT_CLK.reset(TRI_D0NP);
    PIN_EXT_WRn.reset(TRI_D0NP);
    PIN_EXT_RDn.reset(TRI_D0NP);
    PIN_EXT_CSn.reset(TRI_D0NP);

    PIN_EXT_A00p.reset(TRI_D0NP);
    PIN_EXT_A01p.reset(TRI_D0NP);
    PIN_EXT_A02p.reset(TRI_D0NP);
    PIN_EXT_A03p.reset(TRI_D0NP);
    PIN_EXT_A04p.reset(TRI_D0NP);
    PIN_EXT_A05p.reset(TRI_D0NP);
    PIN_EXT_A06p.reset(TRI_D0NP);
    PIN_EXT_A07p.reset(TRI_D0NP);
    PIN_EXT_A08p.reset(TRI_D0NP);
    PIN_EXT_A09p.reset(TRI_D0NP);
    PIN_EXT_A10p.reset(TRI_D0NP);
    PIN_EXT_A11p.reset(TRI_D0NP);
    PIN_EXT_A12p.reset(TRI_D0NP);
    PIN_EXT_A13p.reset(TRI_D0NP);
    PIN_EXT_A14p.reset(TRI_D0NP);
    PIN_EXT_A15p.reset(TRI_D0NP);

    PIN_EXT_D00p.reset(TRI_HZPU);
    PIN_EXT_D01p.reset(TRI_HZPU);
    PIN_EXT_D02p.reset(TRI_HZPU);
    PIN_EXT_D03p.reset(TRI_HZPU);
    PIN_EXT_D04p.reset(TRI_HZPU);
    PIN_EXT_D05p.reset(TRI_HZPU);
    PIN_EXT_D06p.reset(TRI_HZPU);
    PIN_EXT_D07p.reset(TRI_HZPU);
  }

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

  DelayGlitch ext_glitch;

  /*p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p = ERR_XXXX;
  /*p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p = ERR_XXXX;
  /*p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p = ERR_XXXX;
  /*p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p = ERR_XXXX;
  /*p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p = ERR_XXXX;
  /*p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p = ERR_XXXX;
  /*p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p = ERR_XXXX;
  /*p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p = ERR_XXXX;
  /*p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p = ERR_XXXX;
  /*p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p = ERR_XXXX;
  /*p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p = ERR_XXXX;
  /*p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p = ERR_XXXX;
  /*p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p = ERR_XXXX;
  /*p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p = ERR_XXXX;
  /*p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p = ERR_XXXX;

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n = ERR_XXXX;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n = ERR_XXXX;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n = ERR_XXXX;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n = ERR_XXXX;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n = ERR_XXXX;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n = ERR_XXXX;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n = ERR_XXXX;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n = ERR_XXXX;

  //-----------------------------------------------------------------------------
  // Ext bus

  Pin PIN_EXT_CLK  = ERR_XXXX;   // PIN_75
  Pin PIN_EXT_WRn  = ERR_XXXX;   // PIN_78 // WRn idles high, goes low during EFG if there's a write
  Pin PIN_EXT_RDn  = ERR_XXXX;   // PIN_79 // RDn idles low, goes high on phase B for an external write
  Pin PIN_EXT_CSn  = ERR_XXXX;   // PIN_80 // CS changes on phase C if addr in [A000,FDFF]

  Pin PIN_EXT_A00p = ERR_XXXX;   // PIN_01 // Address changees on B for CPU read/write, on A for DMA read
  Pin PIN_EXT_A01p = ERR_XXXX;   // PIN_02
  Pin PIN_EXT_A02p = ERR_XXXX;   // PIN_03
  Pin PIN_EXT_A03p = ERR_XXXX;   // PIN_04
  Pin PIN_EXT_A04p = ERR_XXXX;   // PIN_05
  Pin PIN_EXT_A05p = ERR_XXXX;   // PIN_06
  Pin PIN_EXT_A06p = ERR_XXXX;   // PIN_07
  Pin PIN_EXT_A07p = ERR_XXXX;   // PIN_08
  Pin PIN_EXT_A08p = ERR_XXXX;   // PIN_09
  Pin PIN_EXT_A09p = ERR_XXXX;   // PIN_10
  Pin PIN_EXT_A10p = ERR_XXXX;   // PIN_11
  Pin PIN_EXT_A11p = ERR_XXXX;   // PIN_12
  Pin PIN_EXT_A12p = ERR_XXXX;   // PIN_13
  Pin PIN_EXT_A13p = ERR_XXXX;   // PIN_14
  Pin PIN_EXT_A14p = ERR_XXXX;   // PIN_15
  Pin PIN_EXT_A15p = ERR_XXXX;   // PIN_16 // A15 changes on C

  Pin PIN_EXT_D00p = ERR_XXXX;   // PIN_17
  Pin PIN_EXT_D01p = ERR_XXXX;   // PIN_18
  Pin PIN_EXT_D02p = ERR_XXXX;   // PIN_19
  Pin PIN_EXT_D03p = ERR_XXXX;   // PIN_20
  Pin PIN_EXT_D04p = ERR_XXXX;   // PIN_21
  Pin PIN_EXT_D05p = ERR_XXXX;   // PIN_22
  Pin PIN_EXT_D06p = ERR_XXXX;   // PIN_23
  Pin PIN_EXT_D07p = ERR_XXXX;   // PIN_24
};

//-----------------------------------------------------------------------------

}; // namespace Schematics